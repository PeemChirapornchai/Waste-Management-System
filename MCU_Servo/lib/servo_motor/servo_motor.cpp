#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_motor.h"

constexpr uint8_t servo_PIN = 17;

Servo servo;

int base_angle = 90;

void servo_init()
{
    servo.setPeriodHertz(50);
    servo.attach(servo_PIN, 500, 2400);
    servo_return_to_home();
}

void servo_turn(servo_dir_e dir)
{
    switch (dir)
    {
    case SERVO_HOME:
        servo.write(base_angle);

        break;
    case SERVO_NON_BIO:
        servo.write(base_angle - 60);

        break;
    case SERVO_BIO:
        servo.write(base_angle + 60);

        break;
    default:
        break;
    }

    delay(1000);
}

void servo_return_to_home()
{
    servo_turn(SERVO_HOME);
}

int servo_get_current_angle(uint8_t id)
{
    if (id == 1)
        return servo.read();
}