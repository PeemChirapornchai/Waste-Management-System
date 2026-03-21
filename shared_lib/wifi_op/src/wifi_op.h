#ifndef WIFI_OP_H_
#define WIFI_OP_H_

#include <Arduino.h>

void WIFI_OP_init();
void WIFI_OP_MQTT_Send(const uint8_t *u8buff);
void WIFI_OP_MQTT_reconnectMQTT();
void WIFI_OP_MQTT_init();
void WIFI_OP_MQTT_connection();

#endif
