#define MAIN_H__EXPORT_H
#include <main.h>

static uint16_t u16_idle;
// Setup and Loop Functions
void setup() {
    Serial.begin(115200);
    delay(1000);  // Short delay to ensure serial is ready
    // check Env
    // Connect to server
    WIFI_OP_MQTT_init();
    // Init Task
}

void loop() {
    u16_idle++;
    //size_t flashSize = u16_idle;
    //Serial.printf("IDLE: %d\n", flashSize);
    WIFI_OP_MQTT_connection();
}