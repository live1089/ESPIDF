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



void setup_scr_screen_8(lv_ui *ui)
{
    //Write codes screen_8
    ui->screen_8 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_8, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_8, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_8_img_1
    ui->screen_8_img_1 = lv_image_create(ui->screen_8);
    lv_obj_set_pos(ui->screen_8_img_1, 68, 53);
    lv_obj_set_size(ui->screen_8_img_1, 47, 44);
    lv_obj_add_flag(ui->screen_8_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_8_img_1, &_5_RGB565_47x44);
    lv_image_set_pivot(ui->screen_8_img_1, 50,50);
    lv_image_set_rotation(ui->screen_8_img_1, 0);

    //Write style for screen_8_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_8_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_8_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_8_img_2
    ui->screen_8_img_2 = lv_image_create(ui->screen_8);
    lv_obj_set_pos(ui->screen_8_img_2, 68, 132);
    lv_obj_set_size(ui->screen_8_img_2, 47, 44);
    lv_obj_add_flag(ui->screen_8_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_8_img_2, &_4_RGB565_47x44);
    lv_image_set_pivot(ui->screen_8_img_2, 50,50);
    lv_image_set_rotation(ui->screen_8_img_2, 0);

    //Write style for screen_8_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_8_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_8_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_8_label_1
    ui->screen_8_label_1 = lv_label_create(ui->screen_8);
    lv_obj_set_pos(ui->screen_8_label_1, 143, 67);
    lv_obj_set_size(ui->screen_8_label_1, 90, 24);
    lv_label_set_text(ui->screen_8_label_1, "温度：26");
    lv_label_set_long_mode(ui->screen_8_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_8_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_8_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_8_label_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_8_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_8_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_8_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_8_label_2
    ui->screen_8_label_2 = lv_label_create(ui->screen_8);
    lv_obj_set_pos(ui->screen_8_label_2, 143, 146);
    lv_obj_set_size(ui->screen_8_label_2, 89, 24);
    lv_label_set_text(ui->screen_8_label_2, "湿度：26");
    lv_label_set_long_mode(ui->screen_8_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_8_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_8_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_8_label_2, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_8_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_8_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_8_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_8_btn_1
    ui->screen_8_btn_1 = lv_button_create(ui->screen_8);
    lv_obj_set_pos(ui->screen_8_btn_1, 238, 199);
    lv_obj_set_size(ui->screen_8_btn_1, 63, 23);
    lv_obj_add_flag(ui->screen_8_btn_1, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    ui->screen_8_btn_1_label = lv_label_create(ui->screen_8_btn_1);
    lv_label_set_text(ui->screen_8_btn_1_label, "退出");
    lv_label_set_long_mode(ui->screen_8_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_8_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_8_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_8_btn_1_label, LV_PCT(100));

    //Write style for screen_8_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_8_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_8_btn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_8_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_8_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_8_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_8_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_8_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_8_btn_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_8_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_8_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_8_btn_1, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_bg_opa(ui->screen_8_btn_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->screen_8_btn_1, lv_color_hex(0xff0027), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->screen_8_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->screen_8_btn_1, 0, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->screen_8_btn_1, 5, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui->screen_8_btn_1, 0, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_color(ui->screen_8_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_font(ui->screen_8_btn_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(ui->screen_8_btn_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);

    //Write codes screen_8_sw_1
    ui->screen_8_sw_1 = lv_switch_create(ui->screen_8);
    lv_obj_set_pos(ui->screen_8_sw_1, 40, 202);
    lv_obj_set_size(ui->screen_8_sw_1, 40, 20);

    //Write style for screen_8_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_8_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_8_sw_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_8_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_8_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_8_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_8_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_8_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_8_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_8_sw_1, lv_color_hex(0xff002d), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_8_sw_1, LV_GRAD_DIR_HOR, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(ui->screen_8_sw_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_main_stop(ui->screen_8_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_stop(ui->screen_8_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_8_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_8_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_8_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_8_sw_1, lv_color_hex(0xe73d57), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_8_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_8_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_8_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //The custom code of screen_8.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_8);

    //Init events for screen.
    events_init_screen_8(ui);
}
