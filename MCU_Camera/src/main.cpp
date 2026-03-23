#define MAIN_H__EXPORT_H
#include "main.h"
#include "hw_camera.h"
#include <Waste_Management_Project_inferencing.h> // เปลี่ยนเป็นชื่อไฟล์ .h ของคุณ
#include "edge-impulse-sdk/dsp/image/image.hpp"


#define TAG     "main"
// camera settings
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS           240
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS           240
#define EI_CAMERA_FRAME_BYTE_SIZE                 3
#define BMP_BUF_SIZE                             (EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS * EI_CAMERA_FRAME_BYTE_SIZE)
// static variables
static uint8_t *snapshot_buf = nullptr;
static uint16_t u16_idle;
// function prototypes
void print_memory(void);
void ei_prepare_feature(uint8_t *img_buf, signal_t *signal);
int ei_get_feature_callback(size_t offset, size_t length, float *out_ptr);

void setup()
{
    Serial.begin(115200);
    while(!Serial) { delay(10); } // wait for serial port to connect.
    
    Serial.println("\n--- WASTE MANAGEMENT SYSTEM STARTING ---");
    
    // PSRAM initialization
    if (psramInit()) {
        snapshot_buf = (uint8_t*)ps_malloc(BMP_BUF_SIZE);
        if (snapshot_buf != NULL) {
            Serial.printf("PSRAM initialized. Buffer allocated: %d bytes\n", BMP_BUF_SIZE);
        } else {
            Serial.println("PSRAM allocation FAILED!");
            while(1);
        }
    } else {
        Serial.println("PSRAM not found! System halted.");
        while(1);
    }
    
    hw_camera_init();
    print_memory();
    Serial.println("Setup Complete.");
    
    // Connect to server
    // WIFI_OP_MQTT_init();
    
}

void loop()
{
    u16_idle++;
    // size_t flashSize = u16_idle;
    // Serial.printf("IDLE: %d\n", flashSize);
    WIFI_OP_MQTT_connection();
}