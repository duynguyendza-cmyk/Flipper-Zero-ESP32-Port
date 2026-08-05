#include "u8g2_glue.h"

#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <string.h>
#include <esp_log.h>
#include <esp_rom_sys.h>

#include "boards/board.h"

#define OLED_I2C_PORT BOARD_OLED_I2C_PORT
#define OLED_ADDR     BOARD_OLED_ADDR


static uint8_t tx_buf[129];
static uint8_t tx_len = 0;
static uint8_t is_cmd = 0;

uint8_t u8x8_byte_hw_i2c(
    u8x8_t *u8x8,
        uint8_t msg,
            uint8_t arg_int,
                void *arg_ptr)
                {
                    (void)u8x8;

                        switch(msg) {

                            case U8X8_MSG_BYTE_INIT:
                                    return 1;

                                        case U8X8_MSG_BYTE_START_TRANSFER:
                                                tx_len = 1;
                                                        is_cmd = 1;
                                                                tx_buf[0] = 0x00;   // Command mode
                                                                        return 1;

                                                                            case U8X8_MSG_BYTE_SEND:
                                                                                {
                                                                                        uint8_t *data = (uint8_t *)arg_ptr;

                                                                                                for(uint8_t i = 0; i < arg_int; i++) {

                                                                                                            if(tx_len >= sizeof(tx_buf)) {
                                                                                                            esp_err_t err =
                                                                                                                i2c_master_write_to_device(
                                                                                                                        OLED_I2C_PORT,
                                                                                                                                OLED_ADDR,
                                                                                                                                        tx_buf,
                                                                                                                                                tx_len,
                                                                                                                                                        pdMS_TO_TICKS(100));

                                                                                                                                                        if (err != ESP_OK) {
                                                                                                                                                            ESP_LOGE("OLED", "I2C write fail %s", esp_err_to_name(err));
                                                                                                                                                            }

                                                                                                                                                                                                                                                tx_len = 1;
                                                                                                                                                                                                                                                                tx_buf[0] = is_cmd ? 0x00 : 0x40;
                                                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                                                        tx_buf[tx_len++] = data[i];
                                                                                                                                                                                                                                                                                                }

                                                                                                                                                                                                                                                                                                        return 1;
                                                                                                                                                                                                                                                                                                            }

case U8X8_MSG_BYTE_END_TRANSFER:
{
    esp_err_t err =
            i2c_master_write_to_device(
                        OLED_I2C_PORT,
                                    OLED_ADDR,
                                                tx_buf,
                                                            tx_len,
                                                                        pdMS_TO_TICKS(100));

                                                                            if(err != ESP_OK) {
                                                                                    ESP_LOGE("OLED",
                                                                                             "END_TRANSFER: %s port=%d addr=0x%02X len=%d",
                                                                                                      esp_err_to_name(err),
                                                                                                               OLED_I2C_PORT,
                                                                                                                        OLED_ADDR,
                                                                                                                                 tx_len);
                                                                                        }

                                                                                            tx_len = 0;
                                                                                                return 1;
                                                                                                }

                                                                                                                                                                                                                                                                                                                                                                                                case U8X8_MSG_BYTE_SET_DC:
                                                                                                                                                                                                                                                                                                                                                                                                        is_cmd = (arg_int == 0);
                                                                                                                                                                                                                                                                                                                                                                                                                tx_len = 1;
                                                                                                                                                                                                                                                                                                                                                                                                                        tx_buf[0] = is_cmd ? 0x00 : 0x40;
                                                                                                                                                                                                                                                                                                                                                                                                                                return 1;
                                                                                                                                                                                                                                                                                                                                                                                                                                    }

                                                                                                                                                                                                                                                                                                                                                                                                                                        return 0;
                                                                                                                                                                                                                                                                                                                                                                                                                                        }


uint8_t u8x8_gpio_and_delay_esp32(
        u8x8_t* u8x8,
            uint8_t msg,
                uint8_t arg_int,
                    void* arg_ptr)
                    {
                        switch(msg)
                            {
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

                                                                                                            case U8X8_MSG_GPIO_DC:
                                                                                                                    return 1;

                                                                                                                        default:
                                                                                                                                return 1;
                                                                                                                                    }
                                                                                                                                    }
