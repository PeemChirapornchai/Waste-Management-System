#include "main.h"
#include "hw_camera.h"
#include "ai_handler.h"
#include "wifi_op.h"
#include "mqtt_cmd.h"
#include "base64.h"
#include <HTTPClient.h>


HTTPClient http;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;

void sendImageHTTP(camera_fb_t * fb, const char* label, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    
    
    // create URL with query parameters
    char url[150];
    snprintf(url, sizeof(url), "http://192.168.1.104:8000/upload?label=%s&x=%u&y=%u&w=%u&h=%u", 
         label, x, y, w, h);
    
    // Serial.print("Sending to: ");
    Serial.println(url); 

    http.begin(url); 
    http.addHeader("Content-Type", "image/jpeg");
    
    int httpResponseCode = http.POST(fb->buf, fb->len);
    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response: %d\n", httpResponseCode);
    }
    http.end();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    } // wait for serial connection

    Serial.println("\n--- SYSTEM STARTING ---");

    Serial.println("Connecting to WiFi and MQTT...");
    WIFI_OP_init();      // WiFi init
    WIFI_OP_MQTT_init(); // MQTT init
    PSRAM_init();        // PSRAM init
    hw_camera_init();    // Camera init
    Serial.println("System Ready!");
}


void loop()
{
    WIFI_OP_MQTT_connection();
    const char *label;
    float prob;
    uint32_t x, y, w, h;
    bool is_detected;

    AI_run(&label, &prob, &x, &y, &w, &h, &is_detected);

    // two condition: is_detect + waiting time
    if (is_detected && (millis() - lastSendTime > sendInterval))
    { 
        // save a send time
        lastSendTime = millis();

        Serial.printf("Detected: %s with confidence %.2f\n", label, prob);

        // 
        if (strcmp(label, "B") == 0) {
            WIFI_OP_MQTT_Send((const uint8_t *)MQTT_CMD::BIO, MQTT_COMMAND_TOPIC);
        }
        else if (strcmp(label, "N") == 0) {
            WIFI_OP_MQTT_Send((const uint8_t *)MQTT_CMD::NON_BIO, MQTT_COMMAND_TOPIC);
        }

        // send image 
        camera_fb_t * fb = esp_camera_fb_get();
        if (fb) {
            sendImageHTTP(fb, label, x, y, w, h); 
            esp_camera_fb_return(fb); // return memory
        }

        Serial.println("Command sent, entering 5s cooldown...");
        Serial.println("--------------------------------------------");
    }
}