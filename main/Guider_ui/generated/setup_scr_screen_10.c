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



void setup_scr_screen_10(lv_ui *ui)
{
    //Write codes screen_10
    ui->screen_10 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_10, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_10, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_10, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_10_btn_1
    ui->screen_10_btn_1 = lv_button_create(ui->screen_10);
    lv_obj_set_pos(ui->screen_10_btn_1, 238, 199);
    lv_obj_set_size(ui->screen_10_btn_1, 63, 23);
    lv_obj_add_flag(ui->screen_10_btn_1, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    ui->screen_10_btn_1_label = lv_label_create(ui->screen_10_btn_1);
    lv_label_set_text(ui->screen_10_btn_1_label, "退出");
    lv_label_set_long_mode(ui->screen_10_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_10_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_10_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_10_btn_1_label, LV_PCT(100));

    //Write style for screen_10_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_10_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_10_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_10_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_10_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_10_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_10_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_10_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_10_btn_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_10_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_10_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_10_btn_1, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_bg_opa(ui->screen_10_btn_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->screen_10_btn_1, lv_color_hex(0xff0027), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->screen_10_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_10_btn_1, 0, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->screen_10_btn_1, 5, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui->screen_10_btn_1, 0, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_color(ui->screen_10_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_font(ui->screen_10_btn_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(ui->screen_10_btn_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);

    //Write codes screen_10_img_1
    ui->screen_10_img_1 = lv_image_create(ui->screen_10);
    lv_obj_set_pos(ui->screen_10_img_1, 73, 24);
    lv_obj_set_size(ui->screen_10_img_1, 47, 44);
    lv_obj_add_flag(ui->screen_10_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_10_img_1, &_7_RGB565_47x44);
    lv_image_set_pivot(ui->screen_10_img_1, 50,50);
    lv_image_set_rotation(ui->screen_10_img_1, 0);

    //Write style for screen_10_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_10_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_10_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_10_label_1
    ui->screen_10_label_1 = lv_label_create(ui->screen_10);
    lv_obj_set_pos(ui->screen_10_label_1, 143, 40);
    lv_obj_set_size(ui->screen_10_label_1, 99, 19);
    lv_label_set_text(ui->screen_10_label_1, "三轴陀螺仪");
    lv_label_set_long_mode(ui->screen_10_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_10_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_10_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_10_label_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_10_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_10_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_10_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_10_spangroup_1
    ui->screen_10_spangroup_1 = lv_spangroup_create(ui->screen_10);
    lv_obj_set_pos(ui->screen_10_spangroup_1, 88, 86);
    lv_obj_set_size(ui->screen_10_spangroup_1, 176, 106);
    lv_spangroup_set_align(ui->screen_10_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_10_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_10_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_10_spangroup_1_span = lv_spangroup_new_span(ui->screen_10_spangroup_1);
    lv_span_set_text(ui->screen_10_spangroup_1_span, "X:      56\n\n\nY:      56\n\n\nZ:      56");
    lv_style_set_text_color(lv_span_get_style(ui->screen_10_spangroup_1_span), lv_color_hex(0xffffff));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_10_spangroup_1_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_10_spangroup_1_span), &lv_font_montserratMedium_15);

    //Write style state: LV_STATE_DEFAULT for &style_screen_10_spangroup_1_main_main_default
    static lv_style_t style_screen_10_spangroup_1_main_main_default;
    ui_init_style(&style_screen_10_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_10_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_10_spangroup_1, &style_screen_10_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_10_spangroup_1);

    //Write codes screen_10_sw_1
    ui->screen_10_sw_1 = lv_switch_create(ui->screen_10);
    lv_obj_set_pos(ui->screen_10_sw_1, 30, 202);
    lv_obj_set_size(ui->screen_10_sw_1, 40, 20);

    //Write style for screen_10_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_10_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_10_sw_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_10_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_10_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_10_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_10_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_10_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_10_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_10_sw_1, lv_color_hex(0xff002d), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_10_sw_1, LV_GRAD_DIR_HOR, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(ui->screen_10_sw_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_main_stop(ui->screen_10_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_stop(ui->screen_10_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_10_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_10_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_10_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_10_sw_1, lv_color_hex(0xe73d57), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_10_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_10_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_10_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //The custom code of screen_10.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_10);

    //Init events for screen.
    events_init_screen_10(ui);
}
