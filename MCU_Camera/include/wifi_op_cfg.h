#ifndef WIFI_OP_CFG_H_
#define WIFI_OP_CFG_H_

#include <Arduino.h>

#define PAYLOAD_MAX 8

#ifndef WIFI_SSID
#define WIFI_SSID "YourWiFiSSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "YourWiFiPassword"
#endif

#ifndef MQTT_CLIENT_ID
#define MQTT_CLIENT_ID "mcu-camera-client"
#endif

#define MQTT_TOPIC_BASE "waste-management-system"
#define MQTT_TOPIC_SUBSCRIBE MQTT_TOPIC_BASE "/servo-data"
#define MQTT_TOPIC_PUBLISH MQTT_TOPIC_BASE "/camera-data"

#endif
