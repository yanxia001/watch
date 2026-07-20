#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "esp_log.h"

void app_main(void)
{
   
    esp_err_t ret = nvs_flash_init();//初始化nvs
   if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret ==  ESP_ERR_NVS_NEW_VERSION_FOUND )
   {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
   }
   while(1)
   {
      printf("工程配置成功\r\n");
      vTaskDelay(500);
   }
   
}

