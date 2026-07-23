#ifndef _hxy_st_H_
#define _hxy_st_H_

#define CONFIG_MOSI_GPIO 14
#define CONFIG_SCLK_GPIO 15
#define CONFIG_CS_GPIO 16
#define CONFIG_DC_GPIO 17
#define CONFIG_RESET_GPIO 13
//屏幕分辨率和方向定义
#define CONFIG_WIDTH 240
#define CONFIG_HEIGHT 280
#define CONFIG_BL_GPIO 18
#define CONFIG_OFFSETX 0
#define CONFIG_OFFSETY 20

#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_st7789.h" // ESP-IDF 5.0+ 通常包含此驱动
#include "driver/gpio.h"
#include "esp_heap_caps.h"

esp_lcd_panel_handle_t  st_init();
void lcd_clear_screen_line_by_line(esp_lcd_panel_handle_t panel_handle, uint16_t color);
#endif