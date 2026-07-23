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

#include "st7789.h"
void app_main(void)
{
    nvs_flash_init();
    iic_init(IIC_NUM, 7, 8);
    
    AHT21_Init();
    bmp280_init();
    qmc5883_init(qmc_mode_NormalMode, qmc_odr_100hz, qmc_range_8g, qmc_osr1_2,qmc_osr2_2);
    jfh_init();
    esp_lcd_panel_handle_t dev;
    dev=st_init();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    lcd_clear_screen_line_by_line(dev, 0x0000); // 黑
  vTaskDelay(pdMS_TO_TICKS(1000));
  lcd_clear_screen_line_by_line(dev, 0xFFFF); // 白
  vTaskDelay(pdMS_TO_TICKS(1000));
  lcd_clear_screen_line_by_line(dev, 0xF800); // 红
  vTaskDelay(pdMS_TO_TICKS(1000));
  lcd_clear_screen_line_by_line(dev, 0x07E0); // 绿
  vTaskDelay(pdMS_TO_TICKS(1000));
  lcd_clear_screen_line_by_line(dev, 0x001F); // 蓝
    printf("MPU6050 WHO_AM_I: 0x%02X\r\n", iic_read_byte(0x68, 0x75));
    printf("DMP init: %d\r\n", mpu_dmp_init());

    int16_t temp;
    int32_t pressure;
    int16_t aaa;
    unsigned long step_count;
    
    int16_t x ;
    int16_t y ;
    int16_t z ;
    while (1)
    {
        AHT20_Read();
        bmp280_read(&temp, &pressure);



        dmp_get_pedometer_step_count(&step_count);
        printf("当前步数%ld\n",step_count);


        qmc5883_get_xyz(&x, &y, &z);
        aaa = qmc5883_get_angle();
        printf("1111111111111   地磁数据 x = %d ,y = %d , z = %d\r\n",x,y,z);
        printf("角度 为 %d \n",aaa);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
