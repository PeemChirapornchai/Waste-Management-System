#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H
#include <Arduino.h>

// TODO: Changing the enum values for easier to understand.
typedef enum
{
    SERVO_LEFT = 0,   // Normal Trash position
    SERVO_MIDDLE = 1, // Default position
    SERVO_RIGHT = 2   // Recyclable Trash position
} servo_dir_e;

void servo_init();
void servo_turn(servo_dir_e dir);
void servo_calibrate_init_angle();       // Reads the init pin to adjust center
int servo_get_current_angle(uint8_t id); // Returns the current angle of servo 1 or 2

#endif // SERVO_MOTOR_H