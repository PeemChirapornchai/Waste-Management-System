#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H
#include <Arduino.h>

typedef enum
{
    SERVO_HOME = 0,
    SERVO_NON_BIO = 1,
    SERVO_BIO = 2
} servo_dir_e;

void servo_init();
void servo_turn(servo_dir_e dir);
void servo_calibrate_init_angle();       // Reads the init pin to adjust center
int servo_get_current_angle(uint8_t id); // Returns the current angle of servo 1 or 2

#endif // SERVO_MOTOR_H