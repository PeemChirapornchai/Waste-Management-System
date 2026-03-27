#include "main.h"
#include "hw_camera.h"
#include "ai_handler.h" 



void setup() {
    Serial.begin(115200);
    while(!Serial) { delay(10); } // wait for connection

    Serial.println("\n--- SYSTEM STARTING ---");
    PSRAM_init();        // PSRAM init
    hw_camera_init(); // Camera init
    Serial.println("System Ready!");
}



void loop() {
    const char *label;
    float prob;
    uint32_t x, y, w, h;
    bool is_detected;

    AI_run(&label, &prob, &x, &y, &w, &h, &is_detected);

    if (is_detected) {
        Serial.printf("Detected: %s with confidence %.2f\n", label, prob);
        Serial.printf("Location: x:%u, y:%u, w:%u, h:%u\n", x, y, w, h);
        // WIFI_OP_MQTT_Send((const uint8_t*)label);
    } else {
        Serial.println("Nothing detected.");
    }

    //print_memory();
    Serial.println("--------------------------------------------");
    delay(2000);
}