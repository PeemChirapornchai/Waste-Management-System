#define WIFI_OP_EXPORT_H
#include "wifi_op_cfg.h"
#include "wifi_op.h"

WiFiClient ESPClient;
PubSubClient MQTTclient(ESPClient);

// Use heap_caps_malloc if PAYLOAD_MAX is large, or just static for small buffers
// Use alignment for the S3's 32-bit bus efficiency
uint8_t u8_recv_buff[PAYLOAD_MAX] __attribute__((aligned(4)));
volatile uint8_t u8_Message_flag = 0;

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
// ===== MQTT Callback =====
static inline void callback(char* topic, byte* payload, unsigned int length);

// ===== WiFi Connect =====
void WIFI_OP_init() 
{

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


// ===== MQTT Send =====
void WIFI_OP_MQTT_Send(const uint8_t *u8buff)
{
  MQTTclient.publish(MQTT_TOPIC_PUBLISH, u8buff, PAYLOAD_MAX);
}


// ===== MQTT Recv =====
void WIFI_OP_MQTT_Recv(uint8_t *u8buff)
{
    // Directly copy the 8 bytes to the destination
    memcpy(u8buff, (const void*)u8_recv_buff, PAYLOAD_MAX);
  
}


// ===== MQTT Reconnect =====
void WIFI_OP_MQTT_reconnectMQTT() {

  while (!MQTTclient.connected()) {
    // Get the MAC address to create a unique ID
    String clientId = "ESP32S3-";
    clientId += String(WiFi.macAddress()); 

    Serial.print("Attempting MQTT connection as ");
    Serial.println(clientId);

    // Attempt to connect with the UNIQUE ID
    if (MQTTclient.connect(clientId.c_str())) {
      Serial.println("connected");
      MQTTclient.subscribe(MQTT_TOPIC_SUBSCRIBE);
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      delay(5000);
    }
  }
}


// ===== Init Connect WIFI MQTT =====
void WIFI_OP_MQTT_init() {

  WIFI_OP_init();

  MQTTclient.setServer(MQTT_SERVER, MQTT_PORT);

#if RECV == 1
  MQTTclient.setCallback(callback);
#endif

}


// ===== AUTO reconnect MQTT =====
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


// ===== MQTT Callback =====
static inline void callback(char* topic, byte* payload, unsigned int length) 
{
  //Serial.print("Message received: ");
  // Safety check for buffer size
  uint8_t copy_len = (length < PAYLOAD_MAX) ? length : (PAYLOAD_MAX - 1); // prevent overflow

  //Direct Copy
  memcpy((void*)u8_recv_buff, payload, copy_len);
  //u8_recv_buff[copy_len] = '\0';

  //WIFI_OP_MQTT_Send(u8_recv_buff); 
  u8_Message_flag = 1;
}