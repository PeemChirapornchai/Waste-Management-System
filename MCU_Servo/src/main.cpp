#include <Arduino.h>
#include "wifi_op.h"
#include "wifi_op_cfg.h"
#include "servo_motor.h"
#include "state.h"
<<<<<<< HEAD

extern volatile servo_state_e current_state;
extern volatile uint8_t u8_Message_flag;
extern uint8_t u8_recv_buff[PAYLOAD_MAX];

void setup()
{
    Serial.begin(115200); // Initialize serial communication for debugging (printf functions)
    state_transition(SERVO_STATE_INIT);
=======

void setup()
{
    Serial.begin(115200);

    // Wifi Connecting
    WIFI_OP_init();

    // Connecting to MQTT Server
    WIFI_OP_MQTT_init();

    // Servo Initialization
>>>>>>> 3b8a5bac0b654bf16aa1565a201d79c2b3daf265
    servo_init();
    WIFI_OP_MQTT_init();
    state_transition(SERVO_STATE_READY);
}

void loop()
{
    WIFI_OP_MQTT_connection();

<<<<<<< HEAD
    if (u8_Message_flag == 1)
    {
        // 1. Read directly from the team's buffer
        char messageStr[PAYLOAD_MAX + 1] = {0}; // +1 ensures it is a null-terminated string
        memcpy(messageStr, (const void *)u8_recv_buff, PAYLOAD_MAX);

        if (u8_Message_flag == 1)
        {
            // 2. Read directly from the team's buffer
            char messageStr[PAYLOAD_MAX + 1] = {0}; // +1 ensures it is a null-terminated string
            memcpy(messageStr, (const void *)u8_recv_buff, PAYLOAD_MAX);

            String command = String(messageStr);
            Serial.print("Command Received: ");
            Serial.println(command);

            // Check if the servo is READY to process commands
            if (current_state != SERVO_STATE_READY)
            {
                Serial.println("System not ready to process commands.");
                return; // Exit if the system is not in the READY state
            }

            // 3. Move the servos based on the payload
            if (command == "NORMAL")
            {
                servo_turn(SERVO_HOME);
            }
            else if (command == "BIO")
            {
                servo_turn(SERVO_BIO);
            }
            else if (command == "NON_BIO")
            {
                servo_turn(SERVO_NON_BIO);
            }

            // 4. Get the current physical angles of the servos
            int ang1 = servo_get_current_angle(1);
            int ang2 = servo_get_current_angle(2);

            // 5. Pack the angles strictly into the 8-byte limit (Format: " 45,135\0")
            uint8_t tx_buffer[PAYLOAD_MAX] = {0};
            snprintf((char *)tx_buffer, PAYLOAD_MAX, "%3d,%3d", ang1, ang2);

            // 6. Send the angles back using your team's send function
            WIFI_OP_MQTT_Send(tx_buffer);

            // 7. Clear the flag so we don't process the same message again
            u8_Message_flag = 0;
        }
    }
}
=======
    /*
    1) Check for MQTT messages and update state accordingly
    2) Move servo based on received MQTT messages and update state accordingly
    3) Return home and Ready for next command
    */
}
>>>>>>> 3b8a5bac0b654bf16aa1565a201d79c2b3daf265
