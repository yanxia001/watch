#include "lvgl_demo.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000 ));
    lv_init();
 /* Initialize SPI or I2C bus used by the drivers */
    lv_port_disp_init();
    lv_port_indev_init();
}



static void event_handler(lv_event_t * e)
{
 lv_event_code_t code = lv_event_get_code(e);
 lv_obj_t * obj = lv_event_get_target(e);
 if(code == LV_EVENT_VALUE_CHANGED) {
 LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ?
"On" : "Off");
 }
}
void lv_example_switch_1(void)
{
 lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
 lv_obj_set_flex_align(lv_scr_act(), LV_FLEX_ALIGN_CENTER, 
LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
 lv_obj_t * sw;
 sw = lv_switch_create(lv_scr_act());
 lv_obj_add_event_cb(sw, event_handler, LV_EVENT_ALL, NULL);

 sw = lv_switch_create(lv_scr_act());
 lv_obj_add_state(sw, LV_STATE_CHECKED);
 lv_obj_add_event_cb(sw, event_handler, LV_EVENT_ALL, NULL);

 sw = lv_switch_create(lv_scr_act());
 lv_obj_add_state(sw, LV_STATE_CHECKED);
 lv_obj_add_event_cb(sw, event_handler, LV_EVENT_ALL, NULL);

 sw = lv_switch_create(lv_scr_act());
 lv_obj_add_state(sw, LV_STATE_CHECKED );

 lv_obj_add_event_cb(sw, event_handler, LV_EVENT_ALL, NULL);
}

// 这是一个专门用于刷新 LVGL 界面的任务函数
// 所有 LVGL API 调用必须在这个任务里完成（LVGL 非线程安全）
// extern volatile int gui_switch_to_screen;  // 来自 main.c
// extern lv_ui guider_ui;                    // 来自 main.c


