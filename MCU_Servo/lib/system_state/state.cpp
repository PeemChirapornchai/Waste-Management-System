#include <Arduino.h>
#include "state.h"

volatile servo_state_e current_state = SERVO_STATE_IDLE;

void state_transition(servo_state_e new_state)
{
    current_state = new_state;

    switch (current_state)
    {
    case SERVO_STATE_IDLE:
        Serial.println("State: SERVO_STATE_IDLE");
        break;
    case SERVO_STATE_READY:
        Serial.println("State: SERVO_STATE_READY");
        break;
    case SERVO_STATE_MOVING:
        Serial.println("State: SERVO_STATE_MOVING");
        break;
    default:
        Serial.println("State: UNKNOWN");
        break;
    }
}
