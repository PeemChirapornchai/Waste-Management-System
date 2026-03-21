#ifndef WIFI_OP_CFG_H_
#define WIFI_OP_CFG_H_

#ifdef WIFI_OP_EXPORT_H
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PAYLOAD_MAX 8 //Byte

// 1 is Enable, 0 Disable
#define SEND 1
#define RECV 0

// MQTT

const uint16_t MQTT_PORT = 1883;


const char* MQTT_TOPIC_SUBSCRIBE = "taist/aiot/listening/MCUB2A"; //listening MCUB2A
const char* MQTT_TOPIC_PUBLISH = "taist/aiot/listening/MCUA2B";   //send to MCUA2B
#endif /* WIFI_OP_EXPORT_H */

#endif /* WIFI_OP_CFG_H_ */