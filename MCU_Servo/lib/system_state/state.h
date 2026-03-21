#ifndef SERVO_MCU_STATE_H
#define SERVO_MCU_STATE_H

typedef enum servo_state
{
    SERVO_STATE_IDLE = 0,
    SERVO_STATE_READY,
    SERVO_STATE_MOVING,
    SERVO_STATE_AT_POSITION
} servo_state_t;

typedef servo_state_t servo_state_e;

extern volatile servo_state_e current_state;
void state_transition(servo_state_e new_state);

#endif // SERVO_MCU_STATE_H