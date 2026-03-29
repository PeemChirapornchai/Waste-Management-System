import os
import cv2
import numpy as np
import paho.mqtt.publish as publish
from django.http import JsonResponse
from django.shortcuts import render
from django.views.decorators.csrf import csrf_exempt
from django.core.files.storage import FileSystemStorage
from django.conf import settings
from ultralytics import YOLO

# 1. LAZY LOADING THE YOLO NANO MODEL
AI_MODEL = None

# 1. Update the model loading to use your custom .pt file
def get_yolo_model():
    global AI_MODEL
    if AI_MODEL is None:
        print("Loading custom GitHub waste model...")
        # Point this to your models folder. Update 'waste.pt' to your actual file name!
        model_path = os.path.join(settings.BASE_DIR, 'detection', 'models', 'last.pt')
        AI_MODEL = YOLO(model_path) 
    return AI_MODEL

# 2. MAPPING DICTIONARY: Convert everyday objects to your Waste Categories
WASTE_MAPPING = {
    # BIO Items (Paper, Cardboard, Wood)
    'cardboard_bowl': 'BIO',
    'cardboard_box': 'BIO',
    'reuseable_paper': 'BIO',
    'scrap_paper': 'BIO',
    'stick': 'BIO',
    
    # NON-BIO Items (Plastics, Metals, Hazardous, etc.)
    'battery': 'NON-BIO',
    'can': 'NON-BIO',
    'chemical_plastic_bottle': 'NON-BIO',
    'chemical_plastic_gallon': 'NON-BIO',
    'chemical_spray_can': 'NON-BIO',
    'light_bulb': 'NON-BIO',
    'paint_bucket': 'NON-BIO',
    'plastic_bag': 'NON-BIO',
    'plastic_bottle': 'NON-BIO',
    'plastic_bottle_cap': 'NON-BIO',
    'plastic_box': 'NON-BIO',
    'plastic_cultery': 'NON-BIO', # Keeping the exact typo from the model!
    'plastic_cup': 'NON-BIO',
    'plastic_cup_lid': 'NON-BIO',
    'scrap_plastic': 'NON-BIO',
    'snack_bag': 'NON-BIO',
    'straw': 'NON-BIO'
}

# MQTT Settings (Change this to your laptop's hotspot IP!)
MQTT_BROKER_IP = "192.168.137.1" 
MQTT_PORT = 1883
MQTT_TOPIC = "waste/servo/command"

LATEST_DATA = {
    "label": "Waiting...",
    "confidence": 0.0,
    "image_url": None,
    "action_taken": "None"
}


def dashboard(request):
    return render(request, "dashboard.html")


def get_latest_status(request):
    return JsonResponse(
        {
            "detected_type": LATEST_DATA["label"],
            "action_taken": LATEST_DATA["action_taken"],
        }
    )


@csrf_exempt
def process_waste(request):
    global LATEST_DATA

    if request.method == 'POST' and 'image' in request.FILES:
        image_file = request.FILES['image']
        
        # Save Image for the Dashboard
        fs = FileSystemStorage()
        filename = "latest_capture.jpg"
        if fs.exists(filename):
            os.remove(fs.path(filename))
        saved_name = fs.save(filename, image_file)
        image_url = fs.url(saved_name)

        # Decode Image for YOLO
        image_bytes = image_file.read()
        np_arr = np.frombuffer(image_bytes, np.uint8)
        img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

        # Run YOLOv8 AI
        model = get_yolo_model()
        results = model(img)

        detected_label = "Unknown"
        confidence_score = 0.0
        mqtt_payload = None
        action_str = "No action taken"

        # Check if YOLO found anything
        if len(results) > 0 and len(results[0].boxes) > 0:
            top_box = results[0].boxes[0]
            confidence_score = float(top_box.conf[0])
            class_id = int(top_box.cls[0])
            
            # Get the name (e.g., 'bottle', 'apple')
            raw_class_name = model.names[class_id].lower() 
            print(f"AI Detected: {raw_class_name} with confidence {confidence_score}")
            
            # Map the object to BIO or NON-BIO if confidence is high enough
            if confidence_score >= 0.50 and raw_class_name in WASTE_MAPPING:
                detected_category = WASTE_MAPPING[raw_class_name]
                
                if detected_category == 'BIO':
                    detected_label = "BIO"
                    mqtt_payload = "BIO" # The exact string your ESP32 servo needs
                elif detected_category == 'NON-BIO':
                    detected_label = "NON-BIO"
                    mqtt_payload = "NON_BIO" # The exact string your ESP32 servo needs

        # If a valid object was found, send MQTT command
        if mqtt_payload:
            try:
                publish.single(MQTT_TOPIC, payload=mqtt_payload, hostname=MQTT_BROKER_IP, port=MQTT_PORT)
                action_str = f"Published '{mqtt_payload}' to Servo"
                print(action_str)
            except Exception as e:
                action_str = f"MQTT Failed: {str(e)}"
                print(action_str)
        else:
            action_str = "Skipped (Unknown object or low confidence)"

        # Update global memory for the Dashboard
        LATEST_DATA.update({
            "label": detected_label,
            "confidence": round(confidence_score, 2),
            "image_url": image_url,
            "action_taken": action_str
        })

        return JsonResponse({"status": "success", "data": LATEST_DATA})

    elif request.method == 'GET':
        return JsonResponse(LATEST_DATA)

    return JsonResponse({"status": "error", "message": "Invalid request"})