#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "aht20.h"
#include "iic.h"

void app_main(void)
{
    // 1. 初始化 NVS
    nvs_flash_init();

    // 2. 初始化 I2C 总线（AHT20 使用 I2C_NUM_1，SDA=GPIO7，SCL=GPIO8）
    iic_init(IIC_NUM, 7, 8);

    // 3. 初始化 AHT20 温湿度传感器
    AHT21_Init();

    
    while (1)
    {
        printf("aht20配置成功\r\n");
        AHT20_Read();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

