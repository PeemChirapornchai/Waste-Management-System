#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <Arduino.h>

enum servo_state_e
{
    SERVO_INIT,
    SERVO_READY,
    SERVO_RECEIVE_COMMAND,
    SERVO_MOVE,
    SERVO_RETURN_HOME,
    SERVO_ERROR
};

enum camera_state_e
{
    CAMERA_INIT,
    CAMERA_READY,
    CAMERA_CAPTURE,
    CAMERA_PROCESS,
    CAMERA_SEND_DATA,
    CAMERA_ERROR
};

class ServoState
{
private:
    volatile servo_state_e current_state;

public:
    ServoState() : current_state(SERVO_INIT) {}
    servo_state_e get_current_state() const;
    bool is_ready() const;
    void step();
    void set_error();
};

class CameraState
{
private:
    volatile camera_state_e current_state;

public:
    CameraState() : current_state(CAMERA_INIT) {}
    camera_state_e get_current_state() const;
    bool is_ready() const;
    void step();
    void set_error();
};

extern ServoState servo_state;
extern CameraState camera_state;

#endif // SYSTEM_STATE_H