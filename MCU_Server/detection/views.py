from django.shortcuts import render
from django.conf import settings
from django.http import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
import os
import datetime

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
        x = request.GET.get('x', '0')
        y = request.GET.get('y', '0')
        w = request.GET.get('w', '0')
        h = request.GET.get('h', '0')

        timestamp = datetime.datetime.now().strftime("%H%M%S")
        filename = f"{label}_pos({x},{y})_{timestamp}.jpg"
        
        # save to static/detection/images
        upload_path = os.path.join(settings.BASE_DIR, 'static', 'detection', 'images')
        if not os.path.exists(upload_path):
            os.makedirs(upload_path)
            
        filepath = os.path.join(upload_path, filename)
        with open(filepath, 'wb') as f:
            f.write(image_data)

        print(f">>> Image Received: {filename}")
        return HttpResponse("OK", status=200)
    return HttpResponse("Method not allowed", status=405)

# def get_latest_image(request):
#     """API for the dashboard to get the newest image URL."""
#     path = os.path.join(settings.BASE_DIR, 'static', 'detection', 'images')
#     if not os.path.exists(path): return JsonResponse({'url': ''})
    
#     files = [f for f in os.listdir(path) if f.endswith('.jpg')]
#     if not files: return JsonResponse({'url': ''})
    
#     latest_file = max([os.path.join(path, f) for f in files], key=os.path.getctime)
#     return JsonResponse({'url': f'/static/detection/images/{os.path.basename(latest_file)}'})

def get_latest_image(request):
    path = os.path.join(settings.BASE_DIR, 'static', 'detection', 'images')
    if not os.path.exists(path): return JsonResponse({'url': '', 'time': ''})
    
    files = [f for f in os.listdir(path) if f.endswith('.jpg')]
    if not files: return JsonResponse({'url': '', 'time': ''})
    
    # หาไฟล์ล่าสุด
    latest_file_path = max([os.path.join(path, f) for f in files], key=os.path.getctime)
    
    # Timestamp
    file_t = os.path.getctime(latest_file_path)
    file_time_str = datetime.datetime.fromtimestamp(file_t).strftime('%H:%M:%S')
    
    return JsonResponse({
        'url': f'/static/detection/images/{os.path.basename(latest_file_path)}',
        'time': file_time_str  # ส่งเวลาของรูปภาพไปให้หน้าเว็บ
    })