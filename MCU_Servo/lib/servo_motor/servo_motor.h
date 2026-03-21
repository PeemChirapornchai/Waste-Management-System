#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

// Positions for the servo motor (Home, Left, Right)
typedef enum
{
    SERVO_HOME = 0,
    SERVO_NON_BIO = 1,
    SERVO_BIO = 2
} servo_dir_e;

void servo_init();
void servo_turn(servo_dir_e dir);
/*
Add function for checking the servo position
Before moving the servo
*/

#endif // SERVO_MOTOR_H