import os
import cv2
import numpy as np
from django.http import JsonResponse
from django.shortcuts import render
from django.views.decorators.csrf import csrf_exempt
from django.core.files.storage import FileSystemStorage
from django.conf import settings
from ultralytics import YOLO

# Lazy-loaded YOLO model (optional /api/waste/ uploads)
AI_MODEL = None


def get_yolo_model():
    global AI_MODEL
    if AI_MODEL is None:
        print("Loading waste model...")
        model_path = os.path.join(settings.BASE_DIR, "detection", "models", "last.pt")
        AI_MODEL = YOLO(model_path)
    return AI_MODEL


# Maps YOLO class names to categories (payloads match shared_lib/wifi_op/mqtt_cmd.h)
WASTE_MAPPING = {
    "cardboard_bowl": "BIO",
    "cardboard_box": "BIO",
    "reuseable_paper": "BIO",
    "scrap_paper": "BIO",
    "stick": "BIO",
    "battery": "N-BIO",
    "can": "N-BIO",
    "chemical_plastic_bottle": "N-BIO",
    "chemical_plastic_gallon": "N-BIO",
    "chemical_spray_can": "N-BIO",
    "light_bulb": "N-BIO",
    "paint_bucket": "N-BIO",
    "plastic_bag": "N-BIO",
    "plastic_bottle": "N-BIO",
    "plastic_bottle_cap": "N-BIO",
    "plastic_box": "N-BIO",
    "plastic_cultery": "N-BIO",
    "plastic_cup": "N-BIO",
    "plastic_cup_lid": "N-BIO",
    "scrap_plastic": "N-BIO",
    "snack_bag": "N-BIO",
    "straw": "N-BIO",
}

LATEST_DATA = {
    "label": "Waiting...",
    "confidence": 0.0,
    "image_url": None,
    "action_taken": "None",
}


def dashboard(request):
    """Serve the live dashboard; MQTT is handled in the browser via WebSockets."""
    return render(
        request,
        "dashboard.html",
        {
            "mqtt_host": settings.MQTT_BROKER_HOST,
            "mqtt_tcp_port": settings.MQTT_BROKER_PORT,
            "mqtt_camera_tcp_port": settings.MQTT_CAMERA_TCP_PORT,
            "mqtt_servo_tcp_port": settings.MQTT_SERVO_TCP_PORT,
            "mqtt_ws_port": settings.MQTT_WS_PORT,
            "mqtt_ws_path": settings.MQTT_WS_PATH,
            "mqtt_ws_use_ssl": settings.MQTT_WS_USE_SSL,
            "mqtt_command_topic": settings.MQTT_TOPIC,
            "mqtt_data_topic": settings.MQTT_DATA_TOPIC,
        },
    )


def get_latest_status(request):
    return JsonResponse(
        {
            "detected_type": LATEST_DATA["label"],
            "action_taken": LATEST_DATA["action_taken"],
        }
    )


@csrf_exempt
def process_waste(request):
    """Optional server-side inference; does not publish MQTT (ESP32 + browser handle live path)."""
    global LATEST_DATA

    if request.method == "POST" and "image" in request.FILES:
        image_file = request.FILES["image"]

        fs = FileSystemStorage()
        filename = "latest_capture.jpg"
        if fs.exists(filename):
            os.remove(fs.path(filename))
        saved_name = fs.save(filename, image_file)
        image_url = fs.url(saved_name)

        image_bytes = image_file.read()
        np_arr = np.frombuffer(image_bytes, np.uint8)
        img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

        model = get_yolo_model()
        results = model(img)

        detected_label = "Unknown"
        confidence_score = 0.0
        action_str = "No classification"

        if len(results) > 0 and len(results[0].boxes) > 0:
            top_box = results[0].boxes[0]
            confidence_score = float(top_box.conf[0])
            class_id = int(top_box.cls[0])
            raw_class_name = model.names[class_id].lower()
            print(f"AI Detected: {raw_class_name} with confidence {confidence_score}")

            if confidence_score >= 0.50 and raw_class_name in WASTE_MAPPING:
                detected_label = WASTE_MAPPING[raw_class_name]
                action_str = f"Classified as {detected_label} (server inference only)"
            else:
                action_str = "Skipped (unknown class or low confidence)"
        else:
            action_str = "No objects detected"

        LATEST_DATA.update(
            {
                "label": detected_label,
                "confidence": round(confidence_score, 2),
                "image_url": image_url,
                "action_taken": action_str,
            }
        )

        return JsonResponse({"status": "success", "data": LATEST_DATA})

    if request.method == "GET":
        return JsonResponse(LATEST_DATA)

    return JsonResponse({"status": "error", "message": "Invalid request"})
