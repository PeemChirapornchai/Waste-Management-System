#ifndef AI_HANDLER_H
#define AI_HANDLER_H

#include <Arduino.h>

// GBL
void PSRAM_init();
void AI_run(const char **bb_label, float *bb_value, uint32_t *bb_x, uint32_t *bb_y, uint32_t *bb_width, uint32_t *bb_height, bool *found_object);
void print_memory();

#endif