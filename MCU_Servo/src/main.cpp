#include <Arduino.h>
#include "wifi_op.h"
#include "servo_motor.h"
#include "system_state.h"
#include "wifi_op.h"
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

    if (u8_Message_flag != 1){
        return;
    }

    if (!servo_state.is_ready())
    {
        Serial.println("Servo state machine is not ready to process commands.");
        u8_Message_flag = 0;
        return;
    }

    servo_state.step(SERVO_RECEIVE_COMMAND); // After receiving message, step to SERVO_RECEIVE_COMMAND

    WIFI_OP_MQTT_Recv(u8_recv_buff);

    if (strcmp((char *)u8_recv_buff, MQTT_CMD::BIO) == 0)
    {
        servo_turn(SERVO_BIO);
        servo_state.step(SERVO_MOVE); // After moving servo, step to SERVO_MOVE
    }
    else if (strcmp((char *)u8_recv_buff, MQTT_CMD::NON_BIO) == 0)
    {
        servo_turn(SERVO_NON_BIO);
        servo_state.step(SERVO_MOVE); // After moving servo, step to SERVO_MOVE
    }

    /*
    int ang1 = servo_get_current_angle(1);
    int ang2 = servo_get_current_angle(2);

        uint8_t tx_buffer[PAYLOAD_MAX] = {0};
        snprintf((char *)tx_buffer, PAYLOAD_MAX, "%3d,%3d", ang1, ang2);
        WIFI_OP_MQTT_Send(tx_buffer, MQTT_DATA_TOPIC);
    */
    delay(2000); // Simulate time taken for servo to move and stabilize
    servo_return_to_home();
    servo_state.step(SERVO_RETURN_HOME); // After returning home, step to SERVO_RETURN_HOME
    delay(500);
    servo_state.step(SERVO_READY); // After checking position, step to SERVO_READY

    u8_Message_flag = 0;
    memset(u8_recv_buff, 0, PAYLOAD_MAX); //Clear Flag and Buffer
}
