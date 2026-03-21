#ifndef WIFI_OP_SCB_H_
#define WIFI_OP_SCB_H_

#include <Arduino.h>
#include <cstring>

#include "wifi_op.h"
#include "wifi_op_cfg.h"

extern volatile uint8_t u8_recv_buff[PAYLOAD_MAX];
extern volatile uint8_t u8_Message_flag;

static inline void callback(char *topic, byte *payload, unsigned int length)
{
    (void)topic;

    const uint8_t copy_len = (length > PAYLOAD_MAX) ? PAYLOAD_MAX : static_cast<uint8_t>(length);

    for (uint8_t i = 0; i < PAYLOAD_MAX; i++)
    {
        u8_recv_buff[i] = 0;
    }

    for (uint8_t i = 0; i < copy_len; i++)
    {
        u8_recv_buff[i] = payload[i];
    }

    u8_Message_flag = 1;
}

#endif
