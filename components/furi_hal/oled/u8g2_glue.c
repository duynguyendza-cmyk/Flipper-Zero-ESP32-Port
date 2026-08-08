#include "u8g2_glue.h"

#include <stdint.h>
#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_rom_sys.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "boards/board.h"

#define OLED_PORT BOARD_OLED_I2C_PORT
#define OLED_ADDR BOARD_OLED_ADDR


uint8_t u8x8_byte_hw_i2c(
    u8x8_t *u8x8,
        uint8_t msg,
            uint8_t arg_int,
                void *arg_ptr)
                {
                    (void)u8x8;

                        switch (msg)
                            {
                                    case U8X8_MSG_BYTE_INIT:
                                                return 1;

                                                        case U8X8_MSG_BYTE_START_TRANSFER:
                                                                    return 1;

                                                                            case U8X8_MSG_BYTE_SEND:
                                                                                    {
                                                                                                if (arg_int == 0 || arg_ptr == NULL)
                                                                                                                return 1;

                                                                                                                            esp_err_t ret = i2c_master_write_to_device(
                                                                                                                                            OLED_PORT,
                                                                                                                                                            OLED_ADDR,
                                                                                                                                                                            (const uint8_t *)arg_ptr,
                                                                                                                                                                                            arg_int,
                                                                                                                                                                                                            pdMS_TO_TICKS(100)
                                                                                                                                                                                                                        );

                                                                                                                                                                                                                                    return (ret == ESP_OK) ? 1 : 0;
                                                                                                                                                                                                                                            }

                                                                                                                                                                                                                                                    case U8X8_MSG_BYTE_END_TRANSFER:
                                                                                                                                                                                                                                                                return 1;

                                                                                                                                                                                                                                                                        case U8X8_MSG_BYTE_SET_DC:
                                                                                                                                                                                                                                                                                    return 1;

                                                                                                                                                                                                                                                                                            default:
                                                                                                                                                                                                                                                                                                        return 1;
                                                                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                                                                                            }


                                                                                                                                                                                                                                                                                                            uint8_t u8x8_gpio_and_delay_esp32(
                                                                                                                                                                                                                                                                                                                u8x8_t *u8x8,
                                                                                                                                                                                                                                                                                                                    uint8_t msg,
                                                                                                                                                                                                                                                                                                                        uint8_t arg_int,
                                                                                                                                                                                                                                                                                                                            void *arg_ptr)
                                                                                                                                                                                                                                                                                                                            {
                                                                                                                                                                                                                                                                                                                                (void)u8x8;
                                                                                                                                                                                                                                                                                                                                    (void)arg_ptr;

                                                                                                                                                                                                                                                                                                                                        switch (msg)
                                                                                                                                                                                                                                                                                                                                            {
                                                                                                                                                                                                                                                                                                                                                    case U8X8_MSG_GPIO_AND_DELAY_INIT:
                                                                                                                                                                                                                                                                                                                                                                return 1;

                                                                                                                                                                                                                                                                                                                                                                        case U8X8_MSG_DELAY_MILLI:
                                                                                                                                                                                                                                                                                                                                                                                    vTaskDelay(pdMS_TO_TICKS(arg_int));
                                                                                                                                                                                                                                                                                                                                                                                                return 1;

                                                                                                                                                                                                                                                                                                                                                                                                        case U8X8_MSG_DELAY_10MICRO:
                                                                                                                                                                                                                                                                                                                                                                                                                    esp_rom_delay_us((uint32_t)arg_int * 10);
                                                                                                                                                                                                                                                                                                                                                                                                                                return 1;

                                                                                                                                                                                                                                                                                                                                                                                                                                        case U8X8_MSG_DELAY_100NANO:
                                                                                                                                                                                                                                                                                                                                                                                                                                                    return 1;

                                                                                                                                                                                                                                                                                                                                                                                                                                                            case U8X8_MSG_GPIO_RESET:
                                                                                                                                                                                                                                                                                                                                                                                                                                                                        return 1;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                case U8X8_MSG_GPIO_CS:
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        case U8X8_MSG_GPIO_DC:
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    return 1;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            default:
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        return 1;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            }