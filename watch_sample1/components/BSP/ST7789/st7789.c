#include "st7789.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_st7789.h" // ESP-IDF 5.0+ 通常包含此驱动
#include "driver/gpio.h"
#include "esp_heap_caps.h"


esp_lcd_panel_handle_t  st_init()
{
    //spi设置
    spi_bus_config_t buscfg = {
    .sclk_io_num = CONFIG_SCLK_GPIO,
    .mosi_io_num = CONFIG_MOSI_GPIO,
    .miso_io_num = -1,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz =  CONFIG_HEIGHT * 80 * sizeof(uint16_t), // 单次最多可传输 80 行像素（假设像素格式为 RGB565）
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO)); // 启用 DMA //最底层的
    

    //从 SPI 总线分配一个 LCD IO 设备句柄
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
    .dc_gpio_num = CONFIG_DC_GPIO,
    .cs_gpio_num = CONFIG_CS_GPIO,
    .pclk_hz = 10 * 1000 * 1000,//设置像素时钟的频率 (Hz)，先降到10M测试
    .lcd_cmd_bits = 8,
    .lcd_param_bits = 8,//分别设置 LCD 控制器芯片可识别的命令及参数的位宽。不同芯片对位宽要求不同
    .spi_mode = 0,//设置 SPI 模式。
    .trans_queue_depth = 10,//设置 SPI 传输队列的深度。该值越大，可以排队的传输越多，但消耗的内存也越多。
    
    };
    // 将 LCD 连接到 SPI 总线
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &io_handle));


//安装 LCD 控制器驱动程序。LCD 控制器驱动程序负责向 LCD 控制器芯片发送命令和参数

    esp_lcd_panel_handle_t panel_handle = NULL;
    
    esp_lcd_panel_dev_config_t panel_config = {
    .reset_gpio_num = CONFIG_RESET_GPIO,
    .rgb_ele_order =  LCD_RGB_ELEMENT_ORDER_RGB,//设置每个颜色数据的 RGB 元素顺序。
    .bits_per_pixel = 16,// RGB565 格式 设置像素颜色数据的位宽。
    
    //.data_endian = LCD_RGB_DATA_ENDIAN_BIG,
    //.data_endian 指定传输到屏幕的数据的字节序。不超过一字节的颜色格式（如 RGB232）不需要指定数据字节序
    
    };
    // 为 ST7789 创建 LCD 面板句柄，并指定 SPI IO 设备句柄
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
    


//LCD 屏幕初始化，分辨率，显示方向
    
    // 1. 硬件复位
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));

    // 2. 初始化屏幕 (这一步会发送 ST7789 的初始化命令序列)
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));//颜色反相是对数据进行“按位取反”（把 0 变成 1，1 变成 0）。
    
    
    //调整画面显示的起始位置
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, CONFIG_OFFSETX, CONFIG_OFFSETY));


    
    //打开显示开关
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    gpio_set_direction(CONFIG_BL_GPIO, GPIO_MODE_OUTPUT);//把控制背光的引脚设为“输出模式”。
    gpio_set_level(CONFIG_BL_GPIO, 1);//输出高电平，给背光供电。

    return panel_handle;

}


void lcd_clear_screen_line_by_line(esp_lcd_panel_handle_t panel_handle, uint16_t color)
{
    // union color1
    // {
    //     uint16_t color;
    //     uint8_t a[2];
    // };
    // union color1 d = {0};
    // d.color = color;
    // printf(" %d",d.a[0]);
    // printf(" %d",d.a[1]);
    // 只申请一行的内存：宽240 * 2字节 = 480字节
    size_t line_buf_size = CONFIG_WIDTH * 2;
    uint16_t *line_buf = heap_caps_malloc(line_buf_size, MALLOC_CAP_DMA);
    
    if (line_buf == NULL) return;

    // 填充这一行数据
    for (int i = 0; i < CONFIG_WIDTH; i++) {
        line_buf[i] = (color >> 8) | (color << 8);
    }

    // 循环绘制每一行
    for (int y = 0; y < CONFIG_HEIGHT; y++) {
        // 每次绘制一行 (y, y+1)
        esp_lcd_panel_draw_bitmap(panel_handle, 0, y, CONFIG_WIDTH, y + 1, line_buf);
    }

    free(line_buf);
}

