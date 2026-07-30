/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_4(lv_ui *ui)
{
    //Write codes screen_4
    ui->screen_4 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_4, 240, 280);
    lv_obj_set_scrollbar_mode(ui->screen_4, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_4_meter_1
    ui->screen_4_meter_1 = lv_meter_create(ui->screen_4);
    // add scale ui->screen_4_meter_1_scale_0
    ui->screen_4_meter_1_scale_0 = lv_meter_add_scale(ui->screen_4_meter_1);
    lv_meter_set_scale_ticks(ui->screen_4_meter_1, ui->screen_4_meter_1_scale_0, 41, 2, 10, lv_color_hex(0xff0000));
    lv_meter_set_scale_range(ui->screen_4_meter_1, ui->screen_4_meter_1_scale_0, 0, 360, 360, 90);

    // add needle line for ui->screen_4_meter_1_scale_0.
    ui->screen_4_meter_1_scale_0_ndline_0 = lv_meter_add_needle_line(ui->screen_4_meter_1, ui->screen_4_meter_1_scale_0, 5, lv_color_hex(0x000000), -10);
    lv_meter_set_indicator_value(ui->screen_4_meter_1, ui->screen_4_meter_1_scale_0_ndline_0, 0);
    lv_obj_set_pos(ui->screen_4_meter_1, 49, 82);
    lv_obj_set_size(ui->screen_4_meter_1, 142, 142);

    //Write style for screen_4_meter_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_4_meter_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_4_meter_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_4_meter_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_4_meter_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_4_meter_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_4_meter_1, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_4_meter_1, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_4_meter_1, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_4_meter_1, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_4_meter_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_4_meter_1, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_4_meter_1, lv_color_hex(0xff0000), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_4_meter_1, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_4_meter_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write style for screen_4_meter_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_4_meter_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_4_meter_1, lv_color_hex(0x000000), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_4_meter_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //The custom code of screen_4.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_4);

}
