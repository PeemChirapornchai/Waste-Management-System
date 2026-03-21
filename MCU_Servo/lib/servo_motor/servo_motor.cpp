#include <Arduino.h>
#include <Servo.h>
#include "servo.h"

// FIXME: Change the number of servos and pins later if needed
constexpr uint8_t SERVO_1_PIN = 18;
constexpr uint8_t SERVO_2_PIN = 19;

Servo servo_1;
Servo servo_2;

void servo_init()
{
    int servo_1_attached = servo_1.attach(SERVO_1_PIN);
    int servo_2_attached = servo_2.attach(SERVO_2_PIN);

    if (servo_1_attached == 0)
    {
        Serial.println("Error: Failed to attach servo_1 to pin 18");
    }

    if (servo_2_attached == 0)
    {
        Serial.println("Error: Failed to attach servo_2 to pin 19");
    }
};

typedef enum
{
    SERVO_LEFT = 0,
    SERVO_MIDDLE = 1,
    SERVO_RIGHT = 2
} servo_dir_e;

// FIXME: Change the angles for left, middle, and right positions
void servo_turn(servo_dir_e dir)
{
    switch (dir)
    {
    case servo_dir_e::SERVO_LEFT:
        servo_1.write(0);
        servo_2.write(180);
        break;
    case servo_dir_e::SERVO_MIDDLE:
        servo_1.write(90);
        servo_2.write(90);
        break;
    case servo_dir_e::SERVO_RIGHT:
        servo_1.write(180);
        servo_2.write(0);
        break;
    default:
        break;
    }
    return;
};