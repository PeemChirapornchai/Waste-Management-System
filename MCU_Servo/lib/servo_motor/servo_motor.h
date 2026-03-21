#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

typedef enum
{
    SERVO_LEFT = 0,
    SERVO_MIDDLE = 1,
    SERVO_RIGHT = 2
} servo_dir_e;

void servo_init();
void servo_turn(servo_dir_e dir);

#endif // SERVO_MOTOR_H