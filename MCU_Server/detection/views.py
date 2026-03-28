import cv2
import numpy as np
import os
import paho.mqtt.client as mqtt
from django.conf import settings
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render

# --- Global variable to hold the latest data ---
latest_data = {
    "detected_type": "Waiting for camera...",
    "action_taken": "-"
}

# --- Make sure the media folder exists ---
os.makedirs(settings.MEDIA_ROOT, exist_ok=True)

@csrf_exempt
def process_waste(request):
    global latest_data
    if request.method == 'POST':
        try:
            image_file = request.FILES.get('image')
            if not image_file:
                return JsonResponse({"error": "No image provided"}, status=400)

            # 1. Decode Image
            image_bytes = image_file.read()
            np_arr = np.frombuffer(image_bytes, np.uint8)
            img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

            # 2. Mock AI Logic (Replace with real model later)
            detected_class = "biodegradable" 
            action = "servo_left"

            # 3. Draw on the image (OpenCV)
            # Put text on the top left corner of the image
            cv2.putText(img, f"Type: {detected_class}", (20, 40), 
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            
            # Save the processed image to the media folder
            image_path = os.path.join(settings.MEDIA_ROOT, 'latest_frame.jpg')
            cv2.imwrite(image_path, img)

            # 4. Update the global data for the frontend to read
            latest_data["detected_type"] = detected_class
            latest_data["action_taken"] = action

            return JsonResponse({"status": "success"})

        except Exception as e:
            return JsonResponse({"error": str(e)}, status=500)

# --- NEW: View to load the HTML Dashboard ---
def dashboard(request):
    return render(request, 'dashboard.html')

# --- NEW: API for the frontend to fetch the latest text ---
def get_latest_status(request):
    return JsonResponse(latest_data)