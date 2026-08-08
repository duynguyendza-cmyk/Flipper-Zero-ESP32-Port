#include "u8g2_glue.h"

#include <driver/i2c.h>
#include <esp_rom_sys.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>

#include "boards/board.h"

#define OLED_PORT BOARD_OLED_I2C_PORT
#define OLED_ADDR BOARD_OLED_ADDR

/*
 * SSD1306 128x64:
 * U8g2 có thể gửi những block khá lớn.
 * 128 byte là an toàn cho buffer trung gian của chúng ta.
 */
#define TXBUF_SIZE 128

static uint8_t txbuf[TXBUF_SIZE];
static uint16_t txlen = 0;

/*
 * U8G2 BYTE callback
 *
 * Quan trọng:
 * Không tự thêm 0x00 / 0x40 ở đây.
 *
 * Với SSD1306 I2C, CAD layer của U8g2
 * đã tạo byte control tương ứng.
 */
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr) {
  (void)u8x8;

  switch (msg) {
  case U8X8_MSG_BYTE_INIT:
    txlen = 0;
    return 1;

  case U8X8_MSG_BYTE_START_TRANSFER:
    txlen = 0;
    return 1;

  case U8X8_MSG_BYTE_SEND: {
    uint8_t *data = (uint8_t *)arg_ptr;

    while (arg_int > 0) {
      /*
       * Nếu buffer đầy thì gửi trước.
       */
      if (txlen >= TXBUF_SIZE) {
        esp_err_t ret = i2c_master_write_to_device(OLED_PORT, OLED_ADDR, txbuf,
                                                   txlen, pdMS_TO_TICKS(100));

        if (ret != ESP_OK) {
          txlen = 0;
          return 0;
        }

        txlen = 0;
      }

      txbuf[txlen++] = *data++;
      arg_int--;
    }

    return 1;
  }

  case U8X8_MSG_BYTE_END_TRANSFER: {
    if (txlen > 0) {
      esp_err_t ret = i2c_master_write_to_device(OLED_PORT, OLED_ADDR, txbuf,
                                                 txlen, pdMS_TO_TICKS(100));

      txlen = 0;

      if (ret != ESP_OK)
        return 0;
    }

    return 1;
  }

  case U8X8_MSG_BYTE_SET_DC:
    /*
     * SSD1306 I2C không có chân DC.
     * CAD layer xử lý control byte.
     */
    return 1;

  default:
    return 1;
  }
}

/*
 * U8G2 GPIO / DELAY callback
 */
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
    esp_rom_delay_us((uint32_t)arg_int * 10);
    return 1;

  case U8X8_MSG_DELAY_100NANO:
    /*
     * ESP32 không cần delay 100 ns ở đây.
     */
    return 1;

  case U8X8_MSG_GPIO_RESET:
    /*
     * Nếu module SSD1306 reset bằng GPIO,
     * phải xử lý GPIO thật ở đây.
     *
     * Nếu module không nối RESET:
     * bỏ qua là đúng.
     */
    return 1;

  case U8X8_MSG_GPIO_CS:
  case U8X8_MSG_GPIO_DC:
    /*
     * I2C SSD1306 không dùng CS/DC.
     */
    return 1;

  default:
    return 1;
  }
}