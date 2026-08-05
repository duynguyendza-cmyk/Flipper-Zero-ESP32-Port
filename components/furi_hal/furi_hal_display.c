/**
 * @file furi_hal_display.c
 * Display HAL — board-driven configuration via boards/board.h
 *
 * Flipper GUI: 128x64 mono → aspect-fit scaled and centered on display
 */

#include "furi_hal_display.h"
#include "furi_hal_light.h"
#include "furi_hal_resources.h"
#include "boards/board.h"

#include <string.h>
#include <esp_log.h>
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <driver/gpio.h>

#include <freertos/semphr.h>

#include "oled/ssd1306.h"
#include <driver/i2c.h>
#include <u8g2.h>
#include <u8x8.h>
#include "oled/u8g2_glue.h"

static u8g2_t u8g2;

static const char* TAG = "FuriHalDisplay";

/* Display dimensions from board config */
#define LCD_H_RES BOARD_LCD_H_RES
#define LCD_V_RES BOARD_LCD_V_RES

/* Flipper framebuffer dimensions */
#define FB_WIDTH  128
#define FB_HEIGHT 64

/* Always-on left/right inset (in physical LCD pixels) kept as plain background
 * color so the UI never sits flush against the screen's left/right edges. A
 * board may override this in its board.h; otherwise this default applies
 * everywhere. Set to 0 to disable. */
#ifndef BOARD_LCD_SIDE_MARGIN
#define BOARD_LCD_SIDE_MARGIN 4
#endif
#define DISPLAY_SIDE_MARGIN BOARD_LCD_SIDE_MARGIN

/* Horizontal resolution actually available to the UI after reserving the
 * left/right margins. The aspect-fit below scales into this, then the result
 * is centered across the full LCD resolution — so the leftover on each side
 * is always >= the margin and is painted with the background color. */
#define USABLE_H_RES (LCD_H_RES - 2 * DISPLAY_SIDE_MARGIN)

/* Scale the 128x64 framebuffer to the largest centered size that keeps aspect ratio. */
#if (USABLE_H_RES * FB_HEIGHT) <= (LCD_V_RES * FB_WIDTH)
#define SCALED_WIDTH  USABLE_H_RES
#define SCALED_HEIGHT ((USABLE_H_RES * FB_HEIGHT) / FB_WIDTH)
#else
#define SCALED_HEIGHT LCD_V_RES
#define SCALED_WIDTH  ((LCD_V_RES * FB_WIDTH) / FB_HEIGHT)
#endif

/* Centering margins */
#define MARGIN_X ((LCD_H_RES - SCALED_WIDTH) / 2)
#define MARGIN_Y ((LCD_V_RES - SCALED_HEIGHT) / 2)

/* Colors from board config — both are set at runtime so the user can pick
 * UI Background (fg_color, the field that fills "unset" mono pixels) and
 * UI Foreground (bg_color, what fills "set" pixels = drawn UI elements). */
static uint16_t fg_color;
static uint16_t bg_color;

/* SPI configuration from board config */
#define LCD_SPI_HOST   BOARD_LCD_SPI_HOST
#define LCD_SPI_FREQ   BOARD_LCD_SPI_FREQ_HZ
#define LCD_CMD_BITS   BOARD_LCD_CMD_BITS
#define LCD_PARAM_BITS BOARD_LCD_PARAM_BITS

/* Stripe-based rendering: render & DMA-send N lines at a time.
 * Reduces DMA buffer from full-frame (~100KB) to a small stripe (~5KB). */
#define STRIPE_HEIGHT 8


void furi_hal_display_init(void)
{
    i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
                    .sda_io_num = 15,
                            .scl_io_num = 16,
                                    .sda_pullup_en = GPIO_PULLUP_ENABLE,
                                            .scl_pullup_en = GPIO_PULLUP_ENABLE,
                                                    .master.clk_speed = 400000,
                                                        };

                                                            i2c_param_config(I2C_NUM_0, &conf);
                                                                i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);

                                                                    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
                                                                            &u8g2,
                                                                                    U8G2_R0,
                                                                                            u8x8_byte_hw_i2c,
                                                                                                    u8x8_gpio_and_delay_esp32);

                                                                                                        u8g2_InitDisplay(&u8g2);
                                                                                                            u8g2_SetPowerSave(&u8g2,0);
                                                                                                                u8g2_ClearBuffer(&u8g2);
                                                                                                                    u8g2_SendBuffer(&u8g2);
                                                                                                                    }



void furi_hal_display_commit(const uint8_t* data, uint32_t size)
{
UNUSED(size);
memcpy(u8g2_GetBufferPtr(&u8g2), data, 1024);
u8g2_SendBuffer(&u8g2);
            }

void furi_hal_display_set_backlight(uint8_t brightness) {
    UNUSED(brightness);
}

void furi_hal_display_sleep(void) {
}

uint16_t furi_hal_display_get_h_res(void)
{
    return 128;
    }

uint16_t furi_hal_display_get_v_res(void)
{
    return 64;
    }

esp_lcd_panel_handle_t furi_hal_display_get_panel_handle(void) {
    return NULL;
}

void furi_hal_display_set_fg_color(uint16_t color) {
}

uint16_t furi_hal_display_get_fg_color(void) {
    return 1;
}

void furi_hal_display_set_bg_color(uint16_t color) {
}

uint16_t furi_hal_display_get_bg_color(void) {
    return 0;
}
