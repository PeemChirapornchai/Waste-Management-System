#ifndef WIFI_OP_CFG_H_
#define WIFI_OP_CFG_H_

#ifdef WIFI_OP_EXPORT_H
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PAYLOAD_MAX 8 //Byte

// MQTT
const char* WIFI_SSID = "********";
const char* WIFI_PASSWORD = "********";

const char* MQTT_SERVER = "broker.emqx.io";
const uint16_t MQTT_PORT = 1883;

const char* MQTT_TOPIC_SUBSCRIBE = "taist/aiot/listening/MCUA2B"; //listening MCUA2B
const char* MQTT_TOPIC_PUBLISH = "taist/aiot/listening";   //send to ALL
#endif /* WIFI_OP_EXPORT_H */

#endif /* WIFI_OP_CFG_H_ */