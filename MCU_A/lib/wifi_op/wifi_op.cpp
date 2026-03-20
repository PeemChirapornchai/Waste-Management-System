#define WIFI_OP_EXPORT_H
#include "wifi_op_cfg.h"
#include "wifi_op.h"
#include "wifi_op_scb.h"

WiFiClient ESPClient;
PubSubClient MQTTclient(ESPClient);


// ===== WiFi Connect =====
void WIFI_OP_init() {

  Serial.println("Connecting WiFi...");

  WiFi.mode(WIFI_STA);             // station mode 
  WiFi.setAutoReconnect(true);     // enable auto reconnect
  WiFi.persistent(true);           // store credentials in flash
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void WIFI_OP_MQTT_Send(const uint8_t *u8buff)
{
  MQTTclient.publish(MQTT_TOPIC_PUBLISH, u8buff, PAYLOAD_MAX);
}




// ===== MQTT Reconnect =====
void WIFI_OP_MQTT_reconnectMQTT() {

  while (!MQTTclient.connected()) {
    Serial.print("Connecting to MQTT...");

    if (MQTTclient.connect("balalee")) { //client.connect("esp32-client"); MQTTclient.connect("balalee")
      Serial.println("connected");

      MQTTclient.subscribe(MQTT_TOPIC_SUBSCRIBE);
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" retry in 2 seconds");
      delay(2000);
    }
  }
}
// ===== Init Connect WIFI MQTT =====
void WIFI_OP_MQTT_init() {

  WIFI_OP_init();

  MQTTclient.setServer(MQTT_SERVER, MQTT_PORT);
  MQTTclient.setCallback(callback);
}

// ===== AUTO reconnect TCPIP =====
void WIFI_OP_MQTT_connection() {

  // WiFi Status
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi lost");
  }

  // Reconnect MQTT if needed
  if (!MQTTclient.connected()) {
    WIFI_OP_MQTT_reconnectMQTT();
  }
  MQTTclient.loop();

}