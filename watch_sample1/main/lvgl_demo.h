#ifndef _hxy_lvgl_H_
#define _hxy_lvgl_H_
#include "esp_timer.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
void lv_tick_task (void *arg);
void uics(void);
void lvgl_task(void *arg);
void lv_example_switch_1(void);
#endif