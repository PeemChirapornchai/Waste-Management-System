#include <Arduino.h>
#include "system_state.h"

ServoState servo_state;
CameraState camera_state;

servo_state_e ServoState::get_current_state() const
{
    return current_state;
}

bool ServoState::is_ready() const
{
    return current_state == SERVO_READY;
}

void ServoState::step()
{
    switch (current_state)
    {
    case SERVO_INIT:
        current_state = SERVO_READY;
        break;
    case SERVO_READY:
        current_state = SERVO_RECEIVE_COMMAND;
        break;
    case SERVO_RECEIVE_COMMAND:
        current_state = SERVO_MOVE;
        break;
    case SERVO_MOVE:
        current_state = SERVO_RETURN_HOME;
        break;
    case SERVO_RETURN_HOME:
        current_state = SERVO_READY;
        break;
    case SERVO_ERROR:
        Serial.println("Servo state machine is in ERROR state!");
        break;
    }

    Serial.printf("Servo state: %d\n", current_state);
}

void ServoState::step(servo_state_e new_state)
{
    current_state = new_state;
    Serial.printf("Servo state: %d\n", current_state);
}

void ServoState::set_error()
{
    current_state = SERVO_ERROR;
    Serial.println("Servo state machine entered ERROR state!");
}

camera_state_e CameraState::get_current_state() const
{
    return current_state;
}

bool CameraState::is_ready() const
{
    return current_state == CAMERA_READY;
}

void CameraState::step()
{
    switch (current_state)
    {
    case CAMERA_INIT:
        current_state = CAMERA_READY;
        break;
    case CAMERA_READY:
        current_state = CAMERA_CAPTURE;
        break;
    case CAMERA_CAPTURE:
        current_state = CAMERA_PROCESS;
        break;
    case CAMERA_PROCESS:
        current_state = CAMERA_SEND_DATA;
        break;
    case CAMERA_SEND_DATA:
        current_state = CAMERA_READY;
        break;
    case CAMERA_ERROR:
        Serial.println("Camera state machine is in ERROR state!");
        break;
    }

    Serial.printf("Camera state: %d\n", current_state);
}

void CameraState::set_error()
{
    current_state = CAMERA_ERROR;
    Serial.println("Camera state machine entered ERROR state!");
}