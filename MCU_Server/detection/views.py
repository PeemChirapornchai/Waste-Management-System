from django.shortcuts import render

# Create your views here.
import cv2
import numpy as np
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
# from ultralytics import YOLO

# Load your model once when the server starts
# model = YOLO('models/best.pt') 

@csrf_exempt
def process_waste(request):
    if request.method == 'POST':
        try:
            image_file = request.FILES.get('image')
            if not image_file:
                return JsonResponse({"error": "No image provided"}, status=400)

            # 1. Decode the image from ESP32
            image_bytes = image_file.read()
            np_arr = np.frombuffer(image_bytes, np.uint8)
            img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

            # 2. Run AI Inference (mocked for now)
            # results = model(img)
            # detected_class = results[0].names[results[0].probs.top1]
            detected_class = "biodegradable" # Mock logic
            
            # 3. Trigger MQTT / Logic for Cucumber RS
            if detected_class == "biodegradable":
                # Code to send MQTT signal to turn servo left
                action = "servo_left"
            else:
                # Code to send MQTT signal to turn servo right
                action = "servo_right"

            return JsonResponse({
                "status": "success", 
                "detected_type": detected_class,
                "action_taken": action
            })

        except Exception as e:
            return JsonResponse({"error": str(e)}, status=500)

    return JsonResponse({"error": "Only POST allowed"}, status=405)