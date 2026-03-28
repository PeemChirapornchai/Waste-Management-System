#include "main.h"
#include "hw_camera.h"
#include "ai_handler.h"
#include "wifi_op.h"
#include "mqtt_cmd.h"

#define CONFIDENCE_THRESHOLD 0.5

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    } // wait for serial connection

    Serial.println("\n--- SYSTEM STARTING ---");

    Serial.println("Connecting to WiFi and MQTT...");
    WIFI_OP_init();        // WiFi init
    WIFI_OP_MQTT_init();   // MQTT init
    PSRAM_init();          // PSRAM init
    hw_camera_init();      // Camera init
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

    if (is_detected)
    {
        Serial.printf("Detected: %s with confidence %.2f\n", label, prob);
        Serial.printf("Location: x:%u, y:%u, w:%u, h:%u\n", x, y, w, h);

        if (strcmp(label, "B") == 0) // FIXME: Change the label "BIO" "NON-BIO" according to label output
        {
            WIFI_OP_MQTT_Send((const uint8_t *)MQTT_CMD::BIO, MQTT_COMMAND_TOPIC);
        }
        else if (strcmp(label, "N") == 0)
        {
            WIFI_OP_MQTT_Send((const uint8_t *)MQTT_CMD::NON_BIO, MQTT_COMMAND_TOPIC);
        }

        is_detected = false; // Reset for next loop
        // print_memory();
        Serial.println("--------------------------------------------");
        delay(2000);
    }
}