/*
* Copyright 2025 NXP
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



void setup_scr_screen_3(lv_ui *ui)
{
    //Write codes screen_3
    ui->screen_3 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_3, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_3, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_3_btn_1
    ui->screen_3_btn_1 = lv_button_create(ui->screen_3);
    lv_obj_set_pos(ui->screen_3_btn_1, 239, 200);
    lv_obj_set_size(ui->screen_3_btn_1, 63, 23);
    lv_obj_add_flag(ui->screen_3_btn_1, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    ui->screen_3_btn_1_label = lv_label_create(ui->screen_3_btn_1);
    lv_label_set_text(ui->screen_3_btn_1_label, "退出");
    lv_label_set_long_mode(ui->screen_3_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_3_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_3_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_3_btn_1_label, LV_PCT(100));

    //Write style for screen_3_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_3_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_3_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_3_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_3_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_3_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_3_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_3_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_3_btn_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_3_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_3_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_3_btn_1, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_bg_opa(ui->screen_3_btn_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->screen_3_btn_1, lv_color_hex(0xff0027), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->screen_3_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_3_btn_1, 0, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->screen_3_btn_1, 5, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui->screen_3_btn_1, 0, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_color(ui->screen_3_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_font(ui->screen_3_btn_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(ui->screen_3_btn_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);

    //Write codes screen_3_spangroup_1
    ui->screen_3_spangroup_1 = lv_spangroup_create(ui->screen_3);
    lv_obj_set_pos(ui->screen_3_spangroup_1, 70, 41);
    lv_obj_set_size(ui->screen_3_spangroup_1, 182, 139);
    lv_spangroup_set_align(ui->screen_3_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_3_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_3_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_3_spangroup_1_span = lv_spangroup_new_span(ui->screen_3_spangroup_1);
    lv_span_set_text(ui->screen_3_spangroup_1_span, "温度：   36 度\n\n\n存储：   25 kb\n\n\n内存：   23 kb");
    lv_style_set_text_color(lv_span_get_style(ui->screen_3_spangroup_1_span), lv_color_hex(0xffffff));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_3_spangroup_1_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_3_spangroup_1_span), &lv_font_HYZiKuTangYiShanKaiW_19);

    //Write style state: LV_STATE_DEFAULT for &style_screen_3_spangroup_1_main_main_default
    static lv_style_t style_screen_3_spangroup_1_main_main_default;
    ui_init_style(&style_screen_3_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_3_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_3_spangroup_1, &style_screen_3_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_3_spangroup_1);

    //Write codes screen_3_btn_2
    ui->screen_3_btn_2 = lv_button_create(ui->screen_3);
    lv_obj_set_pos(ui->screen_3_btn_2, 21, 200);
    lv_obj_set_size(ui->screen_3_btn_2, 63, 23);
    ui->screen_3_btn_2_label = lv_label_create(ui->screen_3_btn_2);
    lv_label_set_text(ui->screen_3_btn_2_label, "关机");
    lv_label_set_long_mode(ui->screen_3_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_3_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_3_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_3_btn_2_label, LV_PCT(100));

    //Write style for screen_3_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_3_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_3_btn_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_3_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_3_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_3_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_3_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_3_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_3_btn_2, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_3_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_3_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_3.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_3);

    //Init events for screen.
    events_init_screen_3(ui);
}
