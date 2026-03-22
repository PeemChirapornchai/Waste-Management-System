#include <Arduino.h>
#include "wifi_op.h"
#include "wifi_op_cfg.h"
#include "servo_motor.h"

extern volatile uint8_t u8_Message_flag;
extern uint8_t u8_recv_buff[PAYLOAD_MAX];

void setup()
{
    servo_init();
    Serial.begin(115200);
    WIFI_OP_MQTT_init();
}

void loop()
{
    WIFI_OP_MQTT_connection();
    // Check the team's global flag
    if (u8_Message_flag == 1) 
    {
        // 2. Read directly from the team's buffer
        char messageStr[PAYLOAD_MAX + 1] = {0}; // +1 ensures it is a null-terminated string
        memcpy(messageStr, (const void*)u8_recv_buff, PAYLOAD_MAX);
        
        String command = String(messageStr);
        Serial.print("Command Received: ");
        Serial.println(command);

        // 3. Move the servos based on the payload
        if (command.startsWith("NORMAL")) {
            servo_turn(SERVO_LEFT);
        } 
        else if (command.startsWith("BIO")) {
            servo_turn(SERVO_RIGHT);
        } 
        else if (command.startsWith("IDLE")) {
            servo_turn(SERVO_MIDDLE);
        }

        // 4. Get the current physical angles of the servos
        int ang1 = servo_get_current_angle(1);
        int ang2 = servo_get_current_angle(2);
        
        // 5. Pack the angles strictly into the 8-byte limit (Format: " 45,135\0")
        uint8_t tx_buffer[PAYLOAD_MAX] = {0};
        snprintf((char*)tx_buffer, PAYLOAD_MAX, "%3d,%3d", ang1, ang2);
        
        // 6. Send the angles back using your team's send function
        WIFI_OP_MQTT_Send(tx_buffer);

        // 7. Clear the flag so we don't process the same message again
        u8_Message_flag = 0;
    }
}
