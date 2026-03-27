#include <Arduino.h>
#include "wifi_op.h"
#include "servo_motor.h"
#include "system_state.h"
#include "mqtt_cmd.h"

extern ServoState servo_state;
extern volatile uint8_t u8_Message_flag;
extern uint8_t u8_recv_buff[PAYLOAD_MAX];

void setup()
{
    servo_state.step(SERVO_INIT); // Start with SERVO_INIT state
    Serial.begin(115200);
    servo_init();
    WIFI_OP_init();
    WIFI_OP_MQTT_init();
    servo_state.step(SERVO_READY); // After initialization, step to SERVO_READY
}

void loop()
{
    WIFI_OP_MQTT_connection();

    if (u8_Message_flag != 1)
    {
        return;
    }

    if (!servo_state.is_ready())
    {
        Serial.println("Servo state machine is not ready to process commands.");
        u8_Message_flag = 0;
        return;
    }

    servo_state.step(SERVO_RECEIVE_COMMAND); // After receiving message, step to SERVO_RECEIVE_COMMAND

    char messageStr[PAYLOAD_MAX + 1] = {0};
    memcpy(messageStr, (const void *)u8_recv_buff, PAYLOAD_MAX);

    String command = String(messageStr);
    Serial.print("Command Received: ");
    Serial.println(command);

    servo_state.step(SERVO_MOVE); // After moving servo, step to SERVO_MOVE
    if (command == MQTT_CMD::BIO)
    {
        servo_turn(SERVO_BIO);
    }
    else if (command == MQTT_CMD::NON_BIO)
    {
        servo_turn(SERVO_NON_BIO);
    }

    servo_state.step(SERVO_MOVE); // After moving servo, step to SERVO_MOVE

    int ang1 = servo_get_current_angle(1);
    int ang2 = servo_get_current_angle(2);
    /*
        uint8_t tx_buffer[PAYLOAD_MAX] = {0};
        snprintf((char *)tx_buffer, PAYLOAD_MAX, "%3d,%3d", ang1, ang2);
        WIFI_OP_MQTT_Send(tx_buffer, MQTT_DATA_TOPIC);
    */
    servo_return_to_home();
    servo_state.step(SERVO_RETURN_HOME); // After returning home, step to SERVO_RETURN_HOME
    // TODO: Check servo position before stepping to SERVO_READY, if not in expected position, set error state
    servo_state.step(SERVO_READY); // After checking position, step to SERVO_READY
    u8_Message_flag = 0;
}
