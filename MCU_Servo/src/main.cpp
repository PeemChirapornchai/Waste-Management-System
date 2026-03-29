#include <Arduino.h>
#include "wifi_op.h"
#include "servo_motor.h"
#include "system_state.h"
#include "mqtt_cmd.h"

extern ServoState servo_state;
extern volatile uint8_t u8_Message_flag;
extern uint8_t u8_recv_buff[PAYLOAD_MAX];

TaskHandle_t servoTaskHandle = NULL;
TaskHandle_t mqttTaskHandle = NULL;
TaskHandle_t guardTaskHandle = NULL;

static void mqttTask(void *pvParameters)
{
    (void)pvParameters;

    for (;;)
    {
        WIFI_OP_MQTT_connection();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void servoTask(void *pvParameters)
{
    (void)pvParameters;
    uint8_t local_cmd[PAYLOAD_MAX];

    for (;;)
    {
        if (u8_Message_flag != 1)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        if (!servo_state.is_ready())
        {
            Serial.println("Servo not ready: clearing flag and buffer");
            memset(u8_recv_buff, 0, PAYLOAD_MAX);
            u8_Message_flag = 0;
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        // Latch command first, then clear shared flag/buffer to avoid duplicate processing.
        WIFI_OP_MQTT_Recv(local_cmd);
        u8_Message_flag = 0;
        memset(u8_recv_buff, 0, PAYLOAD_MAX);

        servo_state.step(SERVO_RECEIVE_COMMAND);

        if (strcmp((char *)local_cmd, MQTT_CMD::BIO) == 0)
        {
            Serial.println("Received command: BIO");
            servo_state.step(SERVO_MOVE);
            servo_turn(SERVO_BIO);
        }
        else if (strcmp((char *)local_cmd, MQTT_CMD::NON_BIO) == 0)
        {
            Serial.println("Received command: NON_BIO");
            servo_state.step(SERVO_MOVE);
            servo_turn(SERVO_NON_BIO);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
        servo_return_to_home();
        servo_state.step(SERVO_RETURN_HOME);
        servo_state.step(SERVO_READY);
    }
}

static void guardTask(void *pvParameters)
{
    (void)pvParameters;

    for (;;)
    {
        if (!servo_state.is_ready() && u8_Message_flag == 1)
        {
            memset(u8_recv_buff, 0, PAYLOAD_MAX);
            u8_Message_flag = 0;
        }

        // Fast guard cycle so buffered commands are dropped quickly when servo is busy.
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void setup()
{
    servo_state.step(SERVO_INIT); // Start with SERVO_INIT state
    Serial.begin(115200);
    servo_init();
    WIFI_OP_init();
    WIFI_OP_MQTT_init();
    servo_state.step(SERVO_READY); // After initialization, step to SERVO_READY

    xTaskCreate(
        servoTask,
        "ServoTask",
        4096,
        NULL,
        2,
        &servoTaskHandle);

    xTaskCreate(
        mqttTask,
        "MQTTTask",
        4096,
        NULL,
        1,
        &mqttTaskHandle);

    xTaskCreate(
        guardTask,
        "GuardTask",
        2048,
        NULL,
        3,
        &guardTaskHandle);
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}
