#include "u8g2_glue.h"
#include <string.h>
#include <driver/i2c.h>
#include <esp_rom_sys.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "boards/board.h"
#define TAG "U8G2"
#define OLED_PORT BOARD_OLED_I2C_PORT
#define OLED_ADDR BOARD_OLED_ADDR
#define TXBUF_SIZE 128

static uint8_t txbuf[TXBUF_SIZE];
static uint8_t txlen = 0;

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,

  void *arg_ptr) {
  (void)u8x8;
  switch (msg) {
  case U8X8_MSG_BYTE_INIT:
    return 1;
  case U8X8_MSG_BYTE_START_TRANSFER:
    txlen = 0;
    return 1;
  case U8X8_MSG_BYTE_SEND: {
    uint8_t *data = (uint8_t *)arg_ptr;
    while (arg_int--) {
      txbuf[txlen++] = *data++;
      if (txlen >= TXBUF_SIZE) {
        i2c_master_write_to_device(OLED_PORT, OLED_ADDR, txbuf, txlen, pdMS_TO_TICKS(100));
        txlen = 0;
      }
}

    return 1;
}

  case U8X8_MSG_BYTE_END_TRANSFER:
    if (txlen) {
   i2c_master_write_to_device(OLED_PORT, OLED_ADDR, txbuf, txlen, pdMS_TO_TICKS(100));
    }
    txlen = 0;
    return 1;

  case U8X8_MSG_BYTE_SET_DC:
    return 1;
  default:
    return 0;
  }
}

uint8_t u8x8_gpio_and_delay_esp32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,

  void *arg_ptr) {
  (void)u8x8;
  (void)arg_ptr;
  switch (msg) {

  case U8X8_MSG_GPIO_AND_DELAY_INIT:
    return 1;

  case U8X8_MSG_DELAY_MILLI:
    vTaskDelay(pdMS_TO_TICKS(arg_int));
    return 1;

  case U8X8_MSG_DELAY_10MICRO:
    esp_rom_delay_us(arg_int * 10);
    return 1;

  case U8X8_MSG_DELAY_100NANO:
    return 1;

  case U8X8_MSG_GPIO_RESET:
    return 1;

  case U8X8_MSG_GPIO_CS:
    return 1;

  case U8X8_MSG_GPIO_DC:
    return 1;

  default:
    return 1;
  }
}