# from django.shortcuts import render
# from django.conf import settings
# from django.http import HttpResponse, JsonResponse
# from django.views.decorators.csrf import csrf_exempt
# import os
# import datetime
# from pymongo import MongoClient

# # --- MongoDB Configuration ---
# MONGO_URI = "mongodb+srv://Worapob:Wannatang@cluster0.klgzff7.mongodb.net/?appName=Cluster0"
# client = MongoClient(MONGO_URI)
# db = client['WasteManagement']
# collection = db['detections']

# def dashboard(request):
#     """Serve the live dashboard; MQTT is handled in the browser via WebSockets."""
#     mqtt_server_cfg = {
#         "host": settings.MQTT_BROKER_HOST,
#         "tcpPort": settings.MQTT_BROKER_PORT,
#         "cameraTcpPort": settings.MQTT_CAMERA_TCP_PORT,
#         "servoTcpPort": settings.MQTT_SERVO_TCP_PORT,
#         "wsPort": settings.MQTT_WS_PORT,
#         "wsPath": settings.MQTT_WS_PATH,
#         "wsUseSSL": settings.MQTT_WS_USE_SSL,
#         "commandTopic": settings.MQTT_TOPIC,
#         "dataTopic": settings.MQTT_DATA_TOPIC,
#     }
#     return render(request, "dashboard.html", {"mqtt_server_cfg": mqtt_server_cfg})

# @csrf_exempt
# def upload_image(request):
#     """Handle POST requests from ESP32 camera."""
#     if request.method == 'POST':
#         image_data = request.body
#         label = request.GET.get('label', 'unknown')
#         confidence = request.GET.get('conf', '0.0')
#         x = request.GET.get('x', '0')
#         y = request.GET.get('y', '0')
#         w = request.GET.get('w', '0')
#         h = request.GET.get('h', '0')

#         # Create file name and Timestamp
#         now = datetime.datetime.now()
#         timestamp_str = now.strftime("%H%M%S")
#         filename = f"{label}_pos({x},{y})_{timestamp_str}.jpg"
        
#         # 1. Save to picture to Disk
#         upload_path = os.path.join(settings.BASE_DIR, 'static', 'detection', 'images')
#         if not os.path.exists(upload_path):
#             os.makedirs(upload_path)
            
#         filepath = os.path.join(upload_path, filename)
#         with open(filepath, 'wb') as f:
#             f.write(image_data)

#         # 2. Upload data to MongoDB
#         try:
#             log_entry = {
#                 "label": label,
#                 "confidence": float(confidence),
#                 "x": int(x),
#                 "y": int(y),
#                 "w": int(w),
#                 "h": int(h),
#                 "timestamp": now, # Store it as a datetime object to make it easy to query
#                 "image_name": filename,
#                 "status": "detected"
#             }
#             collection.insert_one(log_entry)
#             print(f">>> Logged to MongoDB: {label} at {now.strftime('%H:%M:%S')}")
#         except Exception as e:
#             print(f"!!! MongoDB Error: {e}")

#         print(f">>> Image Received: {filename}")
#         return HttpResponse("OK", status=200)
    
#     return HttpResponse("Method not allowed", status=405)

# def get_latest_image(request):
#     """API for the dashboard to get the newest image URL."""
#     path = os.path.join(settings.BASE_DIR, 'static', 'detection', 'images')
#     if not os.path.exists(path): return JsonResponse({'url': '', 'time': ''})
    
#     files = [f for f in os.listdir(path) if f.endswith('.jpg')]
#     if not files: return JsonResponse({'url': '', 'time': ''})
    
#     # look for first file
#     latest_file_path = max([os.path.join(path, f) for f in files], key=os.path.getctime)
    
#     # Take time stamp from file
#     file_t = os.path.getctime(latest_file_path)
#     file_time_str = datetime.datetime.fromtimestamp(file_t).strftime('%H:%M:%S')
    
#     return JsonResponse({
#         'url': f'/static/detection/images/{os.path.basename(latest_file_path)}',
#         'time': file_time_str
    
    
#     })

from django.shortcuts import render
from django.conf import settings
from django.http import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
import os
import datetime
from pymongo import MongoClient

# --- MongoDB Configuration ---
MONGO_URI = "mongodb+srv://Worapob:Wannatang@cluster0.klgzff7.mongodb.net/?appName=Cluster0"
client = MongoClient(MONGO_URI)
db = client['WasteManagement']
collection = db['detections']

def dashboard(request):
    """Serve the live dashboard; MQTT is handled in the browser via WebSockets."""
    mqtt_server_cfg = {
        "host": settings.MQTT_BROKER_HOST,
        "tcpPort": settings.MQTT_BROKER_PORT,
        "cameraTcpPort": settings.MQTT_CAMERA_TCP_PORT,
        "servoTcpPort": settings.MQTT_SERVO_TCP_PORT,
        "wsPort": settings.MQTT_WS_PORT,
        "wsPath": settings.MQTT_WS_PATH,
        "wsUseSSL": settings.MQTT_WS_USE_SSL,
        "commandTopic": settings.MQTT_TOPIC,
        "dataTopic": settings.MQTT_DATA_TOPIC,
    }
    return render(request, "dashboard.html", {"mqtt_server_cfg": mqtt_server_cfg})

@csrf_exempt
def upload_image(request):
    """Handle POST requests from ESP32 camera."""
    if request.method == 'POST':
        image_data = request.body
        label = request.GET.get('label', 'unknown')
        confidence = request.GET.get('conf', '0.0')
        x = request.GET.get('x', '0')
        y = request.GET.get('y', '0')
        w = request.GET.get('w', '0')
        h = request.GET.get('h', '0')

        # Create file name and Timestamp
        now = datetime.datetime.now()
        timestamp_str = now.strftime("%H%M%S")
        filename = f"{label}_pos({x},{y})_{timestamp_str}.jpg"
        
        # 1. Save to picture to Disk
        upload_path = os.path.join(settings.BASE_DIR, 'static', 'detection', 'images')
        if not os.path.exists(upload_path):
            os.makedirs(upload_path)
            
        filepath = os.path.join(upload_path, filename)
        with open(filepath, 'wb') as f:
            f.write(image_data)

        # 2. Upload data to MongoDB
        try:
            log_entry = {
                "label": label,
                "confidence": float(confidence),
                "x": int(x),
                "y": int(y),
                "w": int(w),
                "h": int(h),
                "timestamp": now, # Store it as a datetime object to make it easy to query
                "image_name": filename,
                "status": "detected"
            }
            collection.insert_one(log_entry)
            print(f">>> Logged to MongoDB: {label} at {now.strftime('%H:%M:%S')}")
        except Exception as e:
            print(f"!!! MongoDB Error: {e}")

        print(f">>> Image Received: {filename}")
        return HttpResponse("OK", status=200)
    
    return HttpResponse("Method not allowed", status=405)

def get_latest_image(request):
    """API for the dashboard to get the newest image URL."""
    path = os.path.join(settings.BASE_DIR, 'static', 'detection', 'images')
    if not os.path.exists(path): return JsonResponse({'url': '', 'time': '', 'confidence': 0})
    
    files = [f for f in os.listdir(path) if f.endswith('.jpg')]
    if not files: return JsonResponse({'url': '', 'time': '', 'confidence': 0})
    
    # look for first file
    latest_file_path = max([os.path.join(path, f) for f in files], key=os.path.getctime)
    latest_filename = os.path.basename(latest_file_path)
    
    # Take time stamp from file
    file_t = os.path.getctime(latest_file_path)
    file_time_str = datetime.datetime.fromtimestamp(file_t).strftime('%H:%M:%S')

    # --- ส่วนที่เพิ่ม: ค้นหาค่า confidence จาก MongoDB โดยใช้ชื่อไฟล์ ---
    confidence_val = 0
    try:
        latest_record = collection.find_one({"image_name": latest_filename})
        if latest_record:
            confidence_val = latest_record.get('confidence', 0)
    except Exception as e:
        print(f"!!! MongoDB Fetch Error: {e}")
    
    return JsonResponse({
        'url': f'/static/detection/images/{latest_filename}',
        'time': file_time_str,
        'confidence': confidence_val  # ส่งค่า confidence กลับไปให้ Dashboard
    })