#define MAIN_H__EXPORT_H
#include "main.h"
#include "hw_camera.h"
#include <Waste_Management_Project_inferencing.h> // เปลี่ยนเป็นชื่อไฟล์ .h ของคุณ
#include "edge-impulse-sdk/dsp/image/image.hpp"


#define TAG     "main"
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS           240
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS           240
#define EI_CAMERA_FRAME_BYTE_SIZE                 3
#define BMP_BUF_SIZE                             (EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS * EI_CAMERA_FRAME_BYTE_SIZE)


static uint16_t u16_idle;
// Setup and Loop Functions
void setup()
{
    Serial.begin(115200);
    delay(1000); // Short delay to ensure serial is ready
    // check Env
    // Connect to server
    WIFI_OP_MQTT_init();
    // Init Task
}

void loop()
{
    u16_idle++;
    // size_t flashSize = u16_idle;
    // Serial.printf("IDLE: %d\n", flashSize);
    WIFI_OP_MQTT_connection();
}