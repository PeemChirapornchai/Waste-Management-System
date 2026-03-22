#include <Arduino.h>
<<<<<<< HEAD
#include <ESP32Servo.h>
=======
#include <ESP32Servo.h> //<Servo.h>
>>>>>>> 2d57770749f586138550136f0a9f31d9251d77f5
#include "servo_motor.h"

constexpr uint8_t SERVO_1_PIN = 18;
constexpr uint8_t SERVO_2_PIN = 19;
constexpr uint8_t INIT_ADJUST_PIN = 4; // Analog pin connected to a potentiometer for calibration

Servo servo_1;
Servo servo_2;

// The baseline neutral angle (modified by the INIT_ADJUST_PIN)
int base_angle_1 = 90;
int base_angle_2 = 90;

void servo_init()
{
    // Required for ESP32 hardware PWM
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
<<<<<<< HEAD

    servo_1.setPeriodHertz(50);
    servo_2.setPeriodHertz(50);

    int servo_1_attached = servo_1.attach(SERVO_1_PIN, 500, 2400);
    int servo_2_attached = servo_2.attach(SERVO_2_PIN, 500, 2400);

    if (servo_1_attached == 0)
        Serial.println("Error: Failed to attach servo_1");
    if (servo_2_attached == 0)
        Serial.println("Error: Failed to attach servo_2");
=======
    
    servo_1.setPeriodHertz(50);
    servo_2.setPeriodHertz(50);

    int servo_1_attached = servo_1.attach(SERVO_1_PIN, 500, 2400);
    int servo_2_attached = servo_2.attach(SERVO_2_PIN, 500, 2400);

    if (servo_1_attached == 0) Serial.println("Error: Failed to attach servo_1");
    if (servo_2_attached == 0) Serial.println("Error: Failed to attach servo_2");
>>>>>>> 2d57770749f586138550136f0a9f31d9251d77f5

    pinMode(INIT_ADJUST_PIN, INPUT);

    // Initial calibration and set to middle
    servo_calibrate_init_angle();
    servo_turn(SERVO_MIDDLE);
}

void servo_calibrate_init_angle()
{
    // Read the analog pin (ESP32-S2 standard Arduino map is 0-4095)
    int adc_val = analogRead(INIT_ADJUST_PIN);
<<<<<<< HEAD

    // Map the ADC reading to a fine-tuning offset (-20 to +20 degrees)
    int offset = map(adc_val, 0, 4095, -20, 20);

=======
    
    // Map the ADC reading to a fine-tuning offset (-20 to +20 degrees)
    int offset = map(adc_val, 0, 4095, -20, 20);
    
    // Apply offset (assuming servos face opposite directions mechanically)
    base_angle_1 = 90 + offset;
    base_angle_2 = 90 - offset;
}

void servo_calibrate_init_angle()
{
    // Read the analog pin (ESP32-S2 standard Arduino map is 0-4095)
    int adc_val = analogRead(INIT_ADJUST_PIN);
    
    // Map the ADC reading to a fine-tuning offset (-20 to +20 degrees)
    int offset = map(adc_val, 0, 4095, -20, 20);
    
>>>>>>> 2d57770749f586138550136f0a9f31d9251d77f5
    // Apply offset (assuming servos face opposite directions mechanically)
    base_angle_1 = 90 + offset;
    base_angle_2 = 90 - offset;
}

void servo_turn(servo_dir_e dir)
{
    // Always check for physical knob adjustments before moving
<<<<<<< HEAD
    servo_calibrate_init_angle();
=======
    servo_calibrate_init_angle(); 
>>>>>>> 2d57770749f586138550136f0a9f31d9251d77f5

    switch (dir)
    {
    case SERVO_MIDDLE:
        servo_1.write(base_angle_1);
        servo_2.write(base_angle_2);
        break;
    case SERVO_LEFT: // Normal Trash
        servo_1.write(base_angle_1 - 45);
        servo_2.write(base_angle_2 + 45);
        break;
    case SERVO_RIGHT: // Recyclable/Bio Trash
        servo_1.write(base_angle_1 + 45);
        servo_2.write(base_angle_2 - 45);
        break;
    default:
        break;
    }
}

int servo_get_current_angle(uint8_t id)
{
<<<<<<< HEAD
    if (id == 1)
        return servo_1.read();
    if (id == 2)
        return servo_2.read();
=======
    if (id == 1) return servo_1.read();
    if (id == 2) return servo_2.read();
>>>>>>> 2d57770749f586138550136f0a9f31d9251d77f5
    return -1; // Error
}