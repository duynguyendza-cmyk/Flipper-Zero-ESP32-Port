#include "ssd1306.h"

#include <u8g2.h>
#include <u8x8.h>

#include "u8g2_glue.h"
#include <string.h>
#include <driver/i2c.h>

u8g2_t g_u8g2;

void ssd1306_init(void)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
            &g_u8g2,
                    U8G2_R0,
                            u8x8_byte_hw_i2c,
                                    u8x8_gpio_and_delay_esp32);
u8x8_SetI2CAddress(
        &g_u8g2.u8x8,
            0x3C<<1);

            u8g2_InitDisplay(&g_u8g2);
            u8g2_SetContrast(&g_u8g2, 255);
            u8g2_SetPowerSave(&g_u8g2, 0);
            u8g2_ClearBuffer(&g_u8g2);
            const char *text = "Khang iu";

            int w = u8g2_GetStrWidth(&g_u8g2, text);
            int h = u8g2_GetAscent(&g_u8g2) - u8g2_GetDescent(&g_u8g2);

            int x = (u8g2_GetDisplayWidth(&g_u8g2) - w) / 2;
            int y = (u8g2_GetDisplayHeight(&g_u8g2) + h) / 2;

            u8g2_DrawStr(&g_u8g2, x, y, text);
            u8g2_SendBuffer(&g_u8g2);
            /*u8g2_InitDisplay(&g_u8g2);
            u8g2_SetPowerSave(&g_u8g2, 0);

            while (1) {
                u8g2_ClearBuffer(&g_u8g2);
                    u8g2_DrawBox(&g_u8g2, 0, 0, 128, 64);
                        u8g2_SendBuffer(&g_u8g2);
                            vTaskDelay(pdMS_TO_TICKS(1000));

                                u8g2_ClearBuffer(&g_u8g2);
                                    u8g2_SendBuffer(&g_u8g2);
                                        vTaskDelay(pdMS_TO_TICKS(1000));
                                        }*/
}

void ssd1306_draw_bitmap(const uint8_t* data)
{
    memcpy(
            u8g2_GetBufferPtr(&g_u8g2),
                    data,
                            1024);
                                u8g2_SendBuffer(&g_u8g2);
                                }


                                                            void ssd1306_clear(void)
                                                            {
                                                            u8g2_ClearBuffer(&g_u8g2);
                                                            u8g2_SendBuffer(&g_u8g2);
                                                                    }

                                                                    void ssd1306_power(bool on)
                                                                    {
                                                                        u8g2_SetPowerSave(&g_u8g2, on ? 0 : 1);
                                                                        }