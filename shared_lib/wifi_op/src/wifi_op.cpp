#include <WiFi.h>
#include <PubSubClient.h>

#include "wifi_op_cfg.h"
#include "wifi_op.h"
#include "wifi_op_scb.h"

WiFiClient ESPClient;
PubSubClient MQTTclient(ESPClient);

volatile uint8_t u8_recv_buff[PAYLOAD_MAX];
volatile uint8_t u8_Message_flag = 0;

static void WIFI_OP_MQTT_Recv(uint8_t *u8buff)
{
    for (uint8_t i = 0; i < PAYLOAD_MAX; i++)
    {
        u8buff[i] = u8_recv_buff[i];
    }
}

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
    uint8_t u8_recv[PAYLOAD_MAX];

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi lost");
        return;
    }

    if (!MQTTclient.connected())
    {
        WIFI_OP_MQTT_reconnectMQTT();
    }

    MQTTclient.loop();

    if (u8_Message_flag == 1)
    {
        WIFI_OP_MQTT_Recv(u8_recv);

        for (uint8_t i = 0; i < PAYLOAD_MAX; i++)
        {
            Serial.printf("%x", u8_recv[i]);
        }
        Serial.printf("\n");
        u8_Message_flag = 0;
    }
}
