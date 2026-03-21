#ifndef WIFI_OP_CFG_H_
#define WIFI_OP_CFG_H_

#include <Arduino.h>

#define PAYLOAD_MAX 8

static const char *WIFI_SSID = "********";
static const char *WIFI_PASSWORD = "********";

static const char *MQTT_SERVER = "broker.emqx.io";
static const uint16_t MQTT_PORT = 1883;

#define MQTT_TOPIC_BASE "taist/aiot/listening"
#define MQTT_TOPIC_SUBSCRIBE MQTT_TOPIC_BASE "/MCUA2B"
#define MQTT_TOPIC_PUBLISH MQTT_TOPIC_BASE

static const char *MQTT_CLIENT_ID = "camera-esp32-client";

#endif
