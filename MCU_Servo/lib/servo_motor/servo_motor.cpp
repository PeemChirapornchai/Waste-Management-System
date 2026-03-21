#include <Arduino.h>
#include <Servo.h>
#include "servo_motor.h"
#include "state.h"

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

// FIXME: Change the angles for home, non-bio, and bio positions
void servo_turn(servo_dir_e dir)
{
    // FIXME: Add function for checking the servo position before moving the servo
    // Check if servo is not attached before moving
    // Checking if the servo ready to be moved or not (State)
    state_transition(SERVO_STATE_MOVING);
    switch (dir)
    {
    case servo_dir_e::SERVO_HOME:
        servo_1.write(90);
        servo_2.write(90);
        break;
    case servo_dir_e::SERVO_NON_BIO:
        servo_1.write(0);
        servo_2.write(180);
        break;
    case servo_dir_e::SERVO_BIO:
        servo_1.write(180);
        servo_2.write(0);
        break;
    default:
        break;
    }
    state_transition(SERVO_STATE_READY);
    return;
};
