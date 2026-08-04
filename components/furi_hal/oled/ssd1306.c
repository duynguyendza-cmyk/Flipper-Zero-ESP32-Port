#include "ssd1306.h"

#include <u8g2.h>
#include <u8x8.h>

#include "u8g2_glue.h"
#include <string.h>
#include <driver/i2c.h>

static u8g2_t u8g2;

void ssd1306_init(void)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
            &u8g2,
                    U8G2_R0,
                            u8x8_byte_hw_i2c,
                                    u8x8_gpio_and_delay_esp32);

                                        u8g2_InitDisplay(&u8g2);
                                            u8g2_SetPowerSave(&u8g2, 0);
                                                u8g2_ClearBuffer(&u8g2);
                                                    u8g2_SendBuffer(&u8g2);
                                                    }

                                                    void ssd1306_draw_bitmap(const uint8_t* data)
                                                    {
                                                        memcpy(u8g2_GetBufferPtr(&u8g2), data, 1024);
                                                            u8g2_SendBuffer(&u8g2);
                                                            }

                                                            void ssd1306_clear(void)
                                                            {
                                                                u8g2_ClearBuffer(&u8g2);
                                                                    u8g2_SendBuffer(&u8g2);
                                                                    }

                                                                    void ssd1306_power(bool on)
                                                                    {
                                                                        u8g2_SetPowerSave(&u8g2, on ? 0 : 1);
                                                                        }