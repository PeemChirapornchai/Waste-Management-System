#ifndef WIFI_OP_CFG_H_
#define WIFI_OP_CFG_H_

#ifdef WIFI_OP_EXPORT_H
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PAYLOAD_MAX 8 //Byte

// 1 is Enable, 0 Disable
#define SEND 1
#define RECV 1

// MQTT

const uint16_t MQTT_PORT = 1883;


const char* MQTT_TOPIC_SUBSCRIBE = "taist/aiot/listening/MCUA2B"; //listening MCUA2B
const char* MQTT_TOPIC_PUBLISH = "taist/aiot/listening/MCUB2A";   //send to MCUB2A
#endif /* WIFI_OP_EXPORT_H */

#endif /* WIFI_OP_CFG_H_ */