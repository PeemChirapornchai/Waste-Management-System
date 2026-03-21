#ifndef WIFI_OP_SCB_H_
#define WIFI_OP_SCB_H_

#ifdef WIFI_OP_EXPORT_H

extern volatile uint8_t u8_recv_buff[PAYLOAD_MAX];
extern volatile uint8_t u8_Message_flag;

// ===== MQTT Callback =====
static inline void callback(char* topic, byte* payload, unsigned int length) {

  //Serial.print("Message received: ");

  char buf[128];
  uint8_t u8_abuff[PAYLOAD_MAX];
  if (length >= sizeof(buf)) length = sizeof(buf) - 1; // prevent overflow

  memcpy(buf, payload, length);
  buf[length] = '\0';

  //Serial.println(buf);

  for (uint8_t u8i = 0; u8i < PAYLOAD_MAX; u8i++)
  {
    /* code */
    u8_recv_buff[u8i] = buf[u8i];
  }

  WIFI_OP_MQTT_Send(u8_abuff);
  u8_Message_flag = 1;
}

#endif /* WIFI_OP_EXPORT_H */



#endif /* WIFI_OP_SCB_H_ */