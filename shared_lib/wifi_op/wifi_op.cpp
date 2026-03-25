#include <WiFi.h>
#include <PubSubClient.h>
#include "wifi_op_cfg.h"
#include "wifi_op.h"

WiFiClient ESPClient;
PubSubClient MQTTclient(ESPClient);
uint8_t u8_recv_buff[PAYLOAD_MAX] __attribute__((aligned(4)));
volatile uint8_t u8_Message_flag = 0;

void WIFI_OP_init();
void WIFI_OP_MQTT_Send(const uint8_t *u8buff);
static void WIFI_OP_MQTT_Recv(uint8_t *u8buff);
void WIFI_OP_MQTT_reconnectMQTT();
void WIFI_OP_MQTT_init();
void WIFI_OP_MQTT_connection();
static inline void callback(char* topic, byte* payload, unsigned int length);

void WIFI_OP_init()
{
    Serial.println("Connecting WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
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

static void WIFI_OP_MQTT_Recv(uint8_t *u8buff)
{
    memcpy(u8buff, (const void*)u8_recv_buff, PAYLOAD_MAX);
}

void WIFI_OP_MQTT_reconnectMQTT()
{
    while (!MQTTclient.connected())
    {
        Serial.print("Connecting to MQTT...");
        if (MQTTclient.connect(MQTT_CLIENT_ID))
        {
            Serial.println("connected");
            MQTTclient.subscribe(MQTT_TOPIC_SUBSCRIBE);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(MQTTclient.state());
            Serial.println(" retry in 2 seconds");
            delay(2000);
        }
    }
}

void WIFI_OP_MQTT_init()
{
    WIFI_OP_init();
    MQTTclient.setServer(MQTT_SERVER, MQTT_PORT);
    MQTTclient.setCallback(callback);
}

void WIFI_OP_MQTT_connection() 
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi lost");
  }
  if (!MQTTclient.connected()) {
    WIFI_OP_MQTT_reconnectMQTT();
  }
  MQTTclient.loop();
}

static inline void callback(char* topic, byte* payload, unsigned int length) 
{
  uint8_t copy_len = (length < PAYLOAD_MAX) ? length : (PAYLOAD_MAX - 1);
  memcpy((void*)u8_recv_buff, payload, copy_len);
  u8_Message_flag = 1;
}