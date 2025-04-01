/*
* Copyright 2025 NXP
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
	lv_obj_t *screen_btn_1;
	lv_obj_t *screen_btn_1_label;
	lv_obj_t *screen_btn_2;
	lv_obj_t *screen_btn_2_label;
	lv_obj_t *screen_btn_3;
	lv_obj_t *screen_btn_3_label;
	lv_obj_t *screen_cont_1;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_datetext_1;
	lv_obj_t *screen_digital_clock_1;
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_tileview_1;
	lv_obj_t *screen_1_tileview_1_tile;
	lv_obj_t *screen_1_tileview_1_tile_1;
	lv_obj_t *screen_1_spangroup_1;
	lv_span_t *screen_1_spangroup_1_span;
	lv_obj_t *screen_1_spangroup_2;
	lv_span_t *screen_1_spangroup_2_span;
	lv_obj_t *screen_1_btn_1;
	lv_obj_t *screen_1_btn_1_label;
	lv_obj_t *screen_2;
	bool screen_2_del;
	lv_obj_t *screen_2_list_1;
	lv_obj_t *screen_2_list_1_item0;
	lv_obj_t *screen_2_list_1_item1;
	lv_obj_t *screen_2_list_1_item2;
	lv_obj_t *screen_2_list_1_item3;
	lv_obj_t *screen_3;
	bool screen_3_del;
	lv_obj_t *screen_3_btn_1;
	lv_obj_t *screen_3_btn_1_label;
	lv_obj_t *screen_3_spangroup_1;
	lv_span_t *screen_3_spangroup_1_span;
	lv_obj_t *screen_3_btn_2;
	lv_obj_t *screen_3_btn_2_label;
	lv_obj_t *screen_4;
	bool screen_4_del;
	lv_obj_t *screen_4_btn_1;
	lv_obj_t *screen_4_btn_1_label;
	lv_obj_t *screen_4_sw_1;
	lv_obj_t *screen_4_led_1;
	lv_obj_t *screen_4_label_1;
	lv_obj_t *screen_4_label_2;
	lv_obj_t *screen_4_label_3;
	lv_obj_t *screen_5;
	bool screen_5_del;
	lv_obj_t *screen_5_slider_1;
	lv_obj_t *screen_5_label_1;
	lv_obj_t *screen_5_btn_1;
	lv_obj_t *screen_5_btn_1_label;
	lv_obj_t *screen_6;
	bool screen_6_del;
	lv_obj_t *screen_6_list_1;
	lv_obj_t *screen_6_list_1_item0;
	lv_obj_t *screen_6_list_1_item1;
	lv_obj_t *screen_6_list_1_item2;
	lv_obj_t *screen_6_list_1_item3;
	lv_obj_t *screen_6_list_1_item4;
	lv_obj_t *screen_7;
	bool screen_7_del;
	lv_obj_t *screen_7_img_1;
	lv_obj_t *screen_7_img_2;
	lv_obj_t *screen_7_label_1;
	lv_obj_t *screen_7_label_2;
	lv_obj_t *screen_7_btn_1;
	lv_obj_t *screen_7_btn_1_label;
	lv_obj_t *screen_7_sw_1;
	lv_obj_t *screen_8;
	bool screen_8_del;
	lv_obj_t *screen_8_img_1;
	lv_obj_t *screen_8_img_2;
	lv_obj_t *screen_8_label_1;
	lv_obj_t *screen_8_label_2;
	lv_obj_t *screen_8_btn_1;
	lv_obj_t *screen_8_btn_1_label;
	lv_obj_t *screen_8_sw_1;
	lv_obj_t *screen_9;
	bool screen_9_del;
	lv_obj_t *screen_9_btn_1;
	lv_obj_t *screen_9_btn_1_label;
	lv_obj_t *screen_9_img_1;
	lv_obj_t *screen_9_label_1;
	lv_obj_t *screen_9_sw_1;
	lv_obj_t *screen_10;
	bool screen_10_del;
	lv_obj_t *screen_10_btn_1;
	lv_obj_t *screen_10_btn_1_label;
	lv_obj_t *screen_10_img_1;
	lv_obj_t *screen_10_label_1;
	lv_obj_t *screen_10_spangroup_1;
	lv_span_t *screen_10_spangroup_1_span;
	lv_obj_t *screen_10_sw_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
void setup_scr_screen_2(lv_ui *ui);
void setup_scr_screen_3(lv_ui *ui);
void setup_scr_screen_4(lv_ui *ui);
void setup_scr_screen_5(lv_ui *ui);
void setup_scr_screen_6(lv_ui *ui);
void setup_scr_screen_7(lv_ui *ui);
void setup_scr_screen_8(lv_ui *ui);
void setup_scr_screen_9(lv_ui *ui);
void setup_scr_screen_10(lv_ui *ui);
LV_IMAGE_DECLARE(_3_RGB565_47x44);
LV_IMAGE_DECLARE(_2_RGB565_47x44);
LV_IMAGE_DECLARE(_5_RGB565_47x44);
LV_IMAGE_DECLARE(_4_RGB565_47x44);
LV_IMAGE_DECLARE(_6_RGB565_47x44);
LV_IMAGE_DECLARE(_7_RGB565_47x44);

LV_FONT_DECLARE(lv_font_HYCuHeiJ_16)
LV_FONT_DECLARE(lv_font_HYCuHeiJ_12)
LV_FONT_DECLARE(lv_font_HYCuHeiJ_10)
LV_FONT_DECLARE(lv_font_HYBaoSongJ_12)
LV_FONT_DECLARE(lv_font_HYBaoSongJ_13)
LV_FONT_DECLARE(lv_font_HYZiKuTangYiShanKaiW_19)
LV_FONT_DECLARE(lv_font_HYCuHeiJ_14)
LV_FONT_DECLARE(lv_font_montserratMedium_15)


#ifdef __cplusplus
}
#endif
#endif
