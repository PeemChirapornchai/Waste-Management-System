#ifndef WIFI_OP_H
#define WIFI_OP_H

#include <Arduino.h>

/**
 @brief Initializes the WiFi module.

 This function sets up the WiFi connection using the configured SSID and password.
 It also handles reconnection attempts if the connection is lost.
 */
void WIFI_OP_init();

/**
 @brief Sends data over MQTT.

 @param u8buff A pointer to the buffer containing the data to be sent. The buffer should be of size PAYLOAD_MAX.
 @param topic A pointer to the topic string to which the data should be published.
 */
void WIFI_OP_MQTT_Send(const uint8_t *u8buff, const char *topic);

/**
 @brief Receive data over MQTT.

 @param u8buff A pointer to the buffer containing the data to be receive. The buffer should be of size PAYLOAD_MAX.
 */
void WIFI_OP_MQTT_Recv(const uint8_t *u8buff);

/**
 @brief Reconnects to the MQTT broker if the connection is lost.
 */
void WIFI_OP_MQTT_reconnectMQTT();

/**
 @brief Initializes the MQTT connection.
 */
void WIFI_OP_MQTT_init();

/**
 @brief Establishes a connection to the MQTT broker.
 */
void WIFI_OP_MQTT_connection();

#endif