#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "aht20.h"
#include "iic.h"
#include "bmp280.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "qmc5883.h"
#include "jfh142.h"
#include "cst816.h"
#include "st7789.h"
#include "gatts_table_creat_demo.h"
#include "led.h"
#include "lvgl_demo.h"

void app_main(void)
{
    nvs_flash_init();
    iic_init(IIC_NUM, 7, 8);
    
    AHT21_Init();
    bmp280_init();
    qmc5883_init(qmc_mode_NormalMode, qmc_odr_100hz, qmc_range_8g, qmc_osr1_2,qmc_osr2_2);
    jfh_init();
    esp_lcd_panel_handle_t dev;
    
    
    //dev = st_init();
    //vTaskDelay(100 / portTICK_PERIOD_MS);
    uics();
    //lcd_clear_screen_line_by_line(dev, 0x001F); // 蓝

    cst816t_GPIOinit();
    gpio_init();
    //ble_gatts_init();





    int16_t temp;
    int32_t pressure;
    int16_t aaa;
    unsigned long step_count;
    
    int16_t x ;
    int16_t y ;
    int16_t z ;
    uint16_t touch_x, touch_y;
    while (1)
    {
        AHT20_Read();
        bmp280_read(&temp, &pressure);

        
        cst_getxy(&touch_x, &touch_y); // 传入变量地址，函数内部通过指针修改值
        printf("touch_x: %d, touch_y: %d\n", touch_x, touch_y);


        dmp_get_pedometer_step_count(&step_count);
        printf("当前步数%ld\n",step_count);


        qmc5883_get_xyz(&x, &y, &z);
        aaa = qmc5883_get_angle();
        printf("1111111111111   地磁数据 x = %d ,y = %d , z = %d\r\n",x,y,z);
        printf("角度 为 %d \n",aaa);
        vTaskDelay(pdMS_TO_TICKS(1000));
        //lv_timer_handler(); // 刷新屏幕显示
    }
}
