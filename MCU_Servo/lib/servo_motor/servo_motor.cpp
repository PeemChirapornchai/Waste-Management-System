#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_motor.h"

constexpr uint8_t SERVO_1_PIN = 18;
constexpr uint8_t SERVO_2_PIN = 19;
constexpr uint8_t INIT_ADJUST_PIN = 4;

Servo servo_1;
Servo servo_2;

int base_angle_1 = 90;
int base_angle_2 = 90;

void servo_init()
{
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);

    servo_1.setPeriodHertz(50);
    servo_2.setPeriodHertz(50);

    int servo_1_attached = servo_1.attach(SERVO_1_PIN, 500, 2400);
    int servo_2_attached = servo_2.attach(SERVO_2_PIN, 500, 2400);

    if (servo_1_attached == 0)
        Serial.println("Error: Failed to attach servo_1");
    if (servo_2_attached == 0)
        Serial.println("Error: Failed to attach servo_2");

    pinMode(INIT_ADJUST_PIN, INPUT);
    servo_return_to_home();
}

void servo_calibrate_init_angle()
{
    int adc_val = analogRead(INIT_ADJUST_PIN);
    int offset = map(adc_val, 0, 4095, -20, 20);

    base_angle_1 = 90 + offset;
    base_angle_2 = 90 - offset;
}

void servo_turn(servo_dir_e dir)
{
    servo_calibrate_init_angle();

    switch (dir)
    {
    case SERVO_HOME:
        servo_1.write(base_angle_1);
        servo_2.write(base_angle_2);
        break;
    case SERVO_NON_BIO:
        servo_1.write(base_angle_1 - 45);
        servo_2.write(base_angle_2 + 45);
        break;
    case SERVO_BIO:
        servo_1.write(base_angle_1 + 45);
        servo_2.write(base_angle_2 - 45);
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
        return servo_1.read();
    if (id == 2)
        return servo_2.read();
    return -1; // Error
}