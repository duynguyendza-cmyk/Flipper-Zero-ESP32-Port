#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <u8g2.h>
#include <u8x8.h>

extern u8g2_t g_u8g2;

void ssd1306_init(void);
void ssd1306_draw_bitmap(const uint8_t* data);
void ssd1306_clear(void);
void ssd1306_power(bool on);
void ssd1306_set_contrast(uint8_t contrast);