#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H
#include <Arduino.h>

// Positions for the servo motor (Home, Non-Bio, Bio)
typedef enum
{
    SERVO_HOME = 0,
    SERVO_NON_BIO = 1,
    SERVO_BIO = 2 SERVO_HOME = 0,
    SERVO_NON_BIO = 1,
    SERVO_BIO = 2
} servo_dir_e;

void servo_init();
void servo_turn(servo_dir_e dir);
void servo_calibrate_init_angle();       // Reads the init pin to adjust center
void servo_return_to_home();             // Returns the servos to the home position after each operation
int servo_get_current_angle(uint8_t id); // Returns the current angle of servo 1 or 2

#endif // SERVO_MOTOR_H