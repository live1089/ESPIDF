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



void setup_scr_screen_1(lv_ui *ui)
{
    //Write codes screen_1
    ui->screen_1 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_tileview_1
    ui->screen_1_tileview_1 = lv_tileview_create(ui->screen_1);
    lv_obj_set_pos(ui->screen_1_tileview_1, 0, 0);
    lv_obj_set_size(ui->screen_1_tileview_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_1_tileview_1, LV_SCROLLBAR_MODE_OFF);
    ui->screen_1_tileview_1_tile = lv_tileview_add_tile(ui->screen_1_tileview_1, 0, 0, LV_DIR_RIGHT);
    ui->screen_1_tileview_1_tile_1 = lv_tileview_add_tile(ui->screen_1_tileview_1, 1, 0, LV_DIR_LEFT);

    //Write style for screen_1_tileview_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_tileview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_tileview_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_tileview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_tileview_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_tileview_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_tileview_1, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_tileview_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



    //Write codes screen_1_spangroup_1
    ui->screen_1_spangroup_1 = lv_spangroup_create(ui->screen_1_tileview_1_tile);
    lv_obj_set_pos(ui->screen_1_spangroup_1, 22, 15);
    lv_obj_set_size(ui->screen_1_spangroup_1, 284, 209);
    lv_spangroup_set_align(ui->screen_1_spangroup_1, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_1, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_1, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_1_span = lv_spangroup_new_span(ui->screen_1_spangroup_1);
    lv_span_set_text(ui->screen_1_spangroup_1_span, "                   实时天气\n\n---------------------------------\n\n城市/区：朝阳区\n\n天气：晴        温度：18℃\n\n风向：西北\n\n风力：≤3 级\n\n空气湿度：15\n\n发布时间：2025-03-20 22:02:22");
    lv_style_set_text_color(lv_span_get_style(ui->screen_1_spangroup_1_span), lv_color_hex(0xffffff));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_1_spangroup_1_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_1_spangroup_1_span), &lv_font_HYBaoSongJ_12);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_1_main_main_default
    static lv_style_t style_screen_1_spangroup_1_main_main_default;
    ui_init_style(&style_screen_1_spangroup_1_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_1, &style_screen_1_spangroup_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_1);



    //Write codes screen_1_spangroup_2
    ui->screen_1_spangroup_2 = lv_spangroup_create(ui->screen_1_tileview_1_tile_1);
    lv_obj_set_pos(ui->screen_1_spangroup_2, 26, 24);
    lv_obj_set_size(ui->screen_1_spangroup_2, 282, 186);
    lv_spangroup_set_align(ui->screen_1_spangroup_2, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->screen_1_spangroup_2, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->screen_1_spangroup_2, LV_SPAN_MODE_BREAK);
    //create span
    ui->screen_1_spangroup_2_span = lv_spangroup_new_span(ui->screen_1_spangroup_2);
    lv_span_set_text(ui->screen_1_spangroup_2_span, "                 预报天气\n\n------------------------------\n\n2025-03-20 晴 10~23℃\n\n\n2025-03-21 晴 13~26℃\n\n\n2025-03-22 晴 12~27℃\n\n\n2025-03-23 晴 8~25℃");
    lv_style_set_text_color(lv_span_get_style(ui->screen_1_spangroup_2_span), lv_color_hex(0xffffff));
    lv_style_set_text_decor(lv_span_get_style(ui->screen_1_spangroup_2_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->screen_1_spangroup_2_span), &lv_font_HYBaoSongJ_13);

    //Write style state: LV_STATE_DEFAULT for &style_screen_1_spangroup_2_main_main_default
    static lv_style_t style_screen_1_spangroup_2_main_main_default;
    ui_init_style(&style_screen_1_spangroup_2_main_main_default);

    lv_style_set_border_width(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_radius(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_bg_opa(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_top(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_right(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_bottom(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_pad_left(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_style_set_shadow_width(&style_screen_1_spangroup_2_main_main_default, 0);
    lv_obj_add_style(ui->screen_1_spangroup_2, &style_screen_1_spangroup_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->screen_1_spangroup_2);

    //The custom code of screen_1.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_1);

    //Init events for screen.
    events_init_screen_1(ui);
}
