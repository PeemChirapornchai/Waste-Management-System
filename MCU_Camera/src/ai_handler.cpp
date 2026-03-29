#include "ai_handler.h"
#include "hw_camera.h"
#include <Waste_Management_Project_inferencing.h>
#include "edge-impulse-sdk/dsp/image/image.hpp"

#define TAG "main"

// camera settings
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS 240
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS 240
#define EI_CAMERA_FRAME_BYTE_SIZE 3
#define BMP_BUF_SIZE (EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS * EI_CAMERA_FRAME_BYTE_SIZE)
#define CONFIDENCE_THRESHOLD 0.5
static uint8_t *snapshot_buf = nullptr;

// --- Internal Helper Functions ---

int ei_get_feature_callback(size_t offset, size_t length, float *out_ptr)
{
    size_t pixel_ix = offset * 3;
    size_t pixels_left = length;
    size_t out_ptr_ix = 0;
    while (pixels_left != 0)
    {
        out_ptr[out_ptr_ix] = (snapshot_buf[pixel_ix] << 16) +
                              (snapshot_buf[pixel_ix + 1] << 8) +
                              snapshot_buf[pixel_ix + 2];
        out_ptr_ix++;
        pixel_ix += 3;
        pixels_left--;
    }
    return 0;
}

void ei_prepare_feature(uint8_t *img_buf, signal_t *signal)
{
    signal->total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal->get_data = &ei_get_feature_callback;
    if ((EI_CAMERA_RAW_FRAME_BUFFER_ROWS != EI_CLASSIFIER_INPUT_WIDTH) ||
        (EI_CAMERA_RAW_FRAME_BUFFER_COLS != EI_CLASSIFIER_INPUT_HEIGHT))
    {
        ei::image::processing::crop_and_interpolate_rgb888(
            img_buf, EI_CAMERA_RAW_FRAME_BUFFER_COLS, EI_CAMERA_RAW_FRAME_BUFFER_ROWS,
            img_buf, EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT);
    }
}

// --- Public Functions ---

void PSRAM_init()
{
    if (psramInit())
    {
        snapshot_buf = (uint8_t *)ps_malloc(BMP_BUF_SIZE);
        if (snapshot_buf != NULL)
        {
            Serial.printf("AI: PSRAM Buffer allocated: %d bytes\n", BMP_BUF_SIZE);
        }
    }
}

void print_memory()
{
    Serial.printf("Free Heap: %u | Free PSRAM: %u\n", ESP.getFreeHeap(), ESP.getFreePsram());
}

void AI_run(const char **bb_label, float *bb_value, uint32_t *bb_x, uint32_t *bb_y, uint32_t *bb_width, uint32_t *bb_height, bool *found_object)
{
    uint32_t width, height;
    *found_object = false;

    hw_camera_raw_snapshot(snapshot_buf, &width, &height);
    if (width == 0 || height == 0)
        return;

    signal_t signal;
    ei_prepare_feature(snapshot_buf, &signal);

    ei_impulse_result_t result = {0};
    uint32_t Tstart = millis();
    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
    uint32_t telapsed = millis() - Tstart;

    if (err != EI_IMPULSE_OK)
        return;

    Serial.printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)\n",
                  result.timing.dsp, result.timing.classification, result.timing.anomaly);

    for (size_t ix = 0; ix < result.bounding_boxes_count; ix++)
    {
        auto bb = result.bounding_boxes[ix];
        if (bb.value > 0.3)
        {
            *bb_label = bb.label;
            *bb_value = bb.value;
            *bb_x = bb.x;
            *bb_y = bb.y;
            *bb_width = bb.width;
            *bb_height = bb.height;
            *found_object = true;
            break;
        }
    }
}