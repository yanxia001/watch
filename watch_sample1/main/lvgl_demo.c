#include "lvgl_demo.h"
/**
* @brief 告诉 LVGL 运行时间
* @retval 无
*/
void lv_tick_task (void *arg)
{

    //lv_tick_inc(1);
    lv_tick_inc(1);
    // printf("温度");
}
void uics(void)
{
 //lvgl 启动
    const esp_timer_create_args_t periodic_timer_args = {
    .callback = &lv_tick_task,
    .name = "periodic_gui"
 };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1 ));
    lv_init();
 /* Initialize SPI or I2C bus used by the drivers */
    lv_port_disp_init();
    lv_port_indev_init();
}