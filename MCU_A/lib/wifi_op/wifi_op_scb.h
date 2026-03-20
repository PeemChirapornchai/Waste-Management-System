#ifndef WIFI_OP_SCB_H_
#define WIFI_OP_SCB_H_

#ifdef WIFI_OP_EXPORT_H

// ===== MQTT Callback =====
static inline void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message received: ");

  char buf[128];
  uint8_t u8_abuff[PAYLOAD_MAX];
  if (length >= sizeof(buf)) length = sizeof(buf) - 1; // prevent overflow

  memcpy(buf, payload, length);
  buf[length] = '\0';

  Serial.println(buf);

  if (buf[0] == '1') {
    digitalWrite(2, LOW);
  } else {
    digitalWrite(2, HIGH);
  }
  u8_abuff[0] = 0x6f;
  u8_abuff[1] = 0x6b;
  u8_abuff[2] = 0x61;
  u8_abuff[3] = 0x79;
  u8_abuff[4] = 0x79;
  u8_abuff[5] = 0x79;
  u8_abuff[6] = 0x79;
  u8_abuff[7] = 0x79;

  WIFI_OP_MQTT_Send(u8_abuff);
}

#endif /* WIFI_OP_EXPORT_H */



#endif /* WIFI_OP_SCB_H_ */