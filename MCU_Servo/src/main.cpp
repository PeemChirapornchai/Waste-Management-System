#include <Arduino.h>
#include <wifi_op.h>
#include "wifi_op_cfg.h"
#include "servo_motor.h"
#include "state.h"

void setup()
{
    Serial.begin(115200);

    // Wifi Connecting
    WIFI_OP_init();

    // Connecting to MQTT Server
    WIFI_OP_MQTT_init();

    // Servo Initialization
    servo_init();
};

void loop()
{
    WIFI_OP_MQTT_connection();

    /*
    1) Check for MQTT messages and update state accordingly
    2) Move servo based on received MQTT messages and update state accordingly
    3) Return home and Ready for next command
    */
}
