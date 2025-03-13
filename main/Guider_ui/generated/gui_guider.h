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
  
	lv_obj_t *screen_0;
	bool screen_0_del;
	lv_obj_t *screen_0_cont_1;
	lv_obj_t *screen_0_btn_2;
	lv_obj_t *screen_0_btn_2_label;
	lv_obj_t *screen_0_digital_clock_1;
	lv_obj_t *screen_0_btn_1;
	lv_obj_t *screen_0_btn_1_label;
	lv_obj_t *screen_0_datetext_1;
	lv_obj_t *screen_1;
	bool screen_1_del;
	lv_obj_t *screen_1_cont_9;
	lv_obj_t *screen_1_tileview_1;
	lv_obj_t *screen_1_tileview_1_tile;
	lv_obj_t *screen_1_cont_5;
	lv_obj_t *screen_1_label_4;
	lv_obj_t *screen_1_label_3;
	lv_obj_t *screen_1_label_12;
	lv_obj_t *screen_1_cont_4;
	lv_obj_t *screen_1_label_5;
	lv_obj_t *screen_1_label_10;
	lv_obj_t *screen_1_cont_3;
	lv_obj_t *screen_1_label_9;
	lv_obj_t *screen_1_label_7;
	lv_obj_t *screen_1_cont_2;
	lv_obj_t *screen_1_label_6;
	lv_obj_t *screen_1_label_8;
	lv_obj_t *screen_1_cont_6;
	lv_obj_t *screen_1_label_11;
	lv_obj_t *screen_1_cont_1;
	lv_obj_t *screen_1_label_1;
	lv_obj_t *screen_1_label_2;
	lv_obj_t *screen_1_btn_1;
	lv_obj_t *screen_1_btn_1_label;
	lv_obj_t *screen_2;
	bool screen_2_del;
	lv_obj_t *screen_2_cont_1;
	lv_obj_t *screen_2_calendar_1;
	lv_obj_t *screen_2_btn_1;
	lv_obj_t *screen_2_btn_1_label;
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


void setup_scr_screen_0(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
void setup_scr_screen_2(lv_ui *ui);

LV_IMAGE_DECLARE(_GZYVh3tbgAACvWI_RGB565A8_320x245);

LV_IMAGE_DECLARE(_th_RGB565A8_44x31);

LV_IMAGE_DECLARE(_xz_5x1_RGB565A8_44x31);

LV_FONT_DECLARE(lv_font_HYCuHeiJ_16)
LV_FONT_DECLARE(lv_font_HYCuHeiJ_12)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_HYBaoSongJ_14)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_HYBaoSongJ_20)
LV_FONT_DECLARE(lv_font_HYCuHeiJ_30)
LV_FONT_DECLARE(lv_font_HYBaoSongJ_16)
LV_FONT_DECLARE(lv_font_HYZiKuTangHuiDianKaiShuW_16)
LV_FONT_DECLARE(lv_font_HYCuHeiJ_10)
LV_FONT_DECLARE(lv_font_HYCuHeiJ_27)
LV_FONT_DECLARE(lv_font_HYBaoSongJ_18)


#ifdef __cplusplus
}
#endif
#endif
