#include "u8g2_glue.h"

#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <string.h>

#define OLED_I2C_PORT I2C_NUM_0
#define OLED_ADDR 0x3C

static i2c_cmd_handle_t cmd = NULL;

uint8_t u8x8_byte_hw_i2c(
        u8x8_t* u8x8,
            uint8_t msg,
                uint8_t arg_int,
                    void* arg_ptr)
                    {
                        switch(msg)
                            {
                                case U8X8_MSG_BYTE_INIT:
                                        return 1;

                                            case U8X8_MSG_BYTE_START_TRANSFER:
                                                    cmd = i2c_cmd_link_create();
                                                            i2c_master_start(cmd);
                                                                    i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
                                                                            i2c_master_write_byte(cmd, 0x40, true);
                                                                                    return 1;

                                                                                        case U8X8_MSG_BYTE_SEND:
                                                                                                i2c_master_write(cmd, (uint8_t*)arg_ptr, arg_int, true);
                                                                                                        return 1;

                                                                                                            case U8X8_MSG_BYTE_END_TRANSFER:
                                                                                                                    i2c_master_stop(cmd);
                                                                                                                            i2c_master_cmd_begin(OLED_I2C_PORT, cmd, pdMS_TO_TICKS(100));
                                                                                                                                    i2c_cmd_link_delete(cmd);
                                                                                                                                            cmd = NULL;
                                                                                                                                                    return 1;
                                                                                                                                                        }

                                                                                                                                                            return 0;
                                                                                                                                                            }
)
uint8_t u8x8_gpio_and_delay_esp32(
u8x8_t* u8x8,
uint8_t msg,
uint8_t arg_int,
void* arg_ptr)
{
switch(msg)
{
case U8X8_MSG_DELAY_MILLI:
vTaskDelay(pdMS_TO_TICKS(arg_int));
return 1;
case U8X8_MSG_GPIO_AND_DELAY_INIT:
return 1;
}
return 1;
}