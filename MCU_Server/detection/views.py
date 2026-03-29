from django.shortcuts import render
from django.conf import settings


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

    return render(
        request,
        "dashboard.html",
        {"mqtt_server_cfg": mqtt_server_cfg},
    )
