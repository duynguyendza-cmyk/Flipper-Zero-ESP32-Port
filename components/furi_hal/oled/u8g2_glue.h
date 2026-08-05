#pragma once

#include <stdint.h>
#include "u8x8.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t u8x8_byte_hw_i2c(
    u8x8_t *u8x8,
        uint8_t msg,
            uint8_t arg_int,
                void *arg_ptr
                );

                u8x8_gpio_and_delay_esp32(
                    u8x8_t *u8x8,
                        uint8_t msg,
                            uint8_t arg_int,
                                void *arg_ptr
                                );

                                #ifdef __cplusplus
                                }
                                #endif