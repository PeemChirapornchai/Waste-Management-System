#ifndef WIFI_OP_H_
#define WIFI_OP_H_

#ifdef WIFI_OP_EXPORT_H


#endif /* WIFI_OP_EXPORT_H */

extern volatile uint8_t u8_Message_flag;

// ===== WiFi Connect =====
void WIFI_OP_init();
// ===== MQTT Send =====
void WIFI_OP_MQTT_Send(const uint8_t *u8buff);
// ===== MQTT Recv =====
void WIFI_OP_MQTT_Recv(uint8_t *u8buff);
// ===== MQTT Reconnect =====
void WIFI_OP_MQTT_reconnectMQTT();
// ===== Init Connect WIFI MQTT =====
void WIFI_OP_MQTT_init();
// ===== AUTO reconnect MQTT =====
void WIFI_OP_MQTT_connection();
#endif /* WIFI_OP_H_ */