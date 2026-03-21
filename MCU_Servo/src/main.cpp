#include <Arduino.h>
#include <wifi_op.h>
#include "servo_motor.h"

void setup()
{
    Serial.begin(115200);
    servo_init();
    WIFI_OP_MQTT_init();
};

void loop()
{
    WIFI_OP_MQTT_connection();
}
