/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_label_6;
	lv_obj_t *screen_label_7;
	lv_obj_t *screen_label_8;
	lv_obj_t *screen_label_9;
	lv_obj_t *screen_label_10;
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_img_1;
	lv_obj_t *screen_1_imgbtn_2;
	lv_obj_t *screen_1_imgbtn_2_label;
	lv_obj_t *screen_1_imgbtn_3;
	lv_obj_t *screen_1_imgbtn_3_label;
	lv_obj_t *screen_1_imgbtn_4;
	lv_obj_t *screen_1_imgbtn_4_label;
	lv_obj_t *screen_2;
	bool screen_2_del;
	lv_obj_t *screen_2_img_1;
	lv_obj_t *screen_2_label_1;
	lv_obj_t *screen_2_label_2;
	lv_obj_t *screen_2_label_4;
	lv_obj_t *screen_2_label_5;
	lv_obj_t *screen_2_label_6;
	lv_obj_t *screen_2_label_7;
	lv_obj_t *screen_2_label_8;
	lv_obj_t *screen_2_label_9;
	lv_obj_t *screen_2_label_10;
	lv_obj_t *screen_2_label_11;
	lv_obj_t *screen_3;
	bool screen_3_del;
	lv_obj_t *screen_3_label_1;
	lv_obj_t *screen_3_label_2;
	lv_obj_t *screen_3_label_3;
	lv_obj_t *screen_3_label_4;
	lv_obj_t *screen_3_label_5;
	lv_obj_t *screen_3_label_6;
	lv_obj_t *screen_3_label_7;
	lv_obj_t *screen_3_label_8;
	lv_obj_t *screen_3_label_9;
	lv_obj_t *screen_3_label_10;
	lv_obj_t *screen_3_label_11;
	lv_obj_t *screen_3_label_12;
	lv_obj_t *screen_4;
	bool screen_4_del;
	lv_obj_t *screen_4_meter_1;
	lv_meter_scale_t *screen_4_meter_1_scale_0;
	lv_meter_indicator_t *screen_4_meter_1_scale_0_ndline_0;
	lv_obj_t *screen_4_btn_1;
	lv_obj_t *screen_4_btn_1_label;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
void setup_scr_screen_2(lv_ui *ui);
void setup_scr_screen_3(lv_ui *ui);
void setup_scr_screen_4(lv_ui *ui);
LV_IMG_DECLARE(_138149058_p0_master1200_alpha_240x280);
LV_IMG_DECLARE(_back_alpha_240x280);
LV_IMG_DECLARE(_hai_alpha_60x60);
LV_IMG_DECLARE(_hai_alpha_60x60);
LV_IMG_DECLARE(_zhi_alpha_60x60);
LV_IMG_DECLARE(_zhi_alpha_60x60);
LV_IMG_DECLARE(_xin_alpha_60x60);
LV_IMG_DECLARE(_xin_alpha_60x60);
LV_IMG_DECLARE(_130189906_p0_master1200_alpha_240x280);

LV_FONT_DECLARE(lv_font_333_16)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)


#ifdef __cplusplus
}
#endif
#endif
