#ifndef WIFI_OP_H_
#define WIFI_OP_H_

#ifdef WIFI_OP_EXPORT_H


#endif /* WIFI_OP_EXPORT_H */
// ===== WiFi Connect =====
void WIFI_OP_init();
// ===== MQTT Send =====
void WIFI_OP_MQTT_Send(const uint8_t *u8buff);
// ===== MQTT Reconnect =====
void WIFI_OP_MQTT_reconnectMQTT();
// ===== Init Connect WIFI MQTT =====
void WIFI_OP_MQTT_init();
// ===== AUTO reconnect TCPIP =====
void WIFI_OP_MQTT_connection();
#endif /* WIFI_OP_H_ */