#include <Arduino.h>
#include "wifi.h"
#include "servo_motor.h"
#include "mqtt.h"

void setup()
{
    Serial.begin(115200);
    // wifi_setup();
    servo_init();
    mqtt_setup();
};

void loop()
{
    // put your main code here, to run repeatedly:
}
