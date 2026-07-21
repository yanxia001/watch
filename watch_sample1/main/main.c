#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "aht20.h"
#include "iic.h"
#include "bmp280.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

void app_main(void)
{
    nvs_flash_init();
    iic_init(IIC_NUM, 7, 8);

    AHT21_Init();
    bmp280_init();

    printf("MPU6050 WHO_AM_I: 0x%02X\r\n", iic_read_byte(0x68, 0x75));
    printf("DMP init: %d\r\n", mpu_dmp_init());

    int16_t temp;
    int32_t pressure;
    unsigned long step_count;
    float pitch, roll, yaw;

    while (1)
    {
        AHT20_Read();
        bmp280_read(&temp, &pressure);

        // 先读 DMP FIFO 数据（同时驱动 DMP 内部运算）
        int dmp_ok = mpu_dmp_get_data(&pitch, &roll, &yaw);
        if (dmp_ok != 0) {
            // FIFO 还没准备好，跳过这帧，给 DMP 时间积累数据
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        // DMP FIFO 读完后，再读计步器
        dmp_get_pedometer_step_count(&step_count);

        printf("T:%.1fC P:%.1fhPa P:%.0f R:%.0f Y:%.0f 步:%lu\r\n",
               temp / 10.0,
               pressure / 100.0,
               pitch, roll, yaw,
               step_count);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
