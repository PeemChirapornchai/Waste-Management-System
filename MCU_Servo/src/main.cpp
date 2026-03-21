#include <Arduino.h>
#include <wifi_op.h>
#include "wifi_op_cfg.h"
#include "servo_motor.h"

void setup()
{
    Serial.begin(115200);
    WIFI_OP_init();
    WIFI_OP_MQTT_init();
    servo_init();
};

void loop()
{
    WIFI_OP_MQTT_connection();
}
