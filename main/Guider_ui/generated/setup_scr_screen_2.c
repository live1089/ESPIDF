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
#include "..\lvgl__lvgl\src\widgets\calendar\lv_calendar_private.h"



lv_calendar_date_t screen_2_calendar_1_today;
lv_calendar_date_t screen_2_calendar_1_highlihted_days[1];
void setup_scr_screen_2(lv_ui *ui)
{
    //Write codes screen_2
    ui->screen_2 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_2, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_cont_1
    ui->screen_2_cont_1 = lv_obj_create(ui->screen_2);
    lv_obj_set_pos(ui->screen_2_cont_1, 0, 0);
    lv_obj_set_size(ui->screen_2_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_2_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_2_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_2_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_2_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_2_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_2_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_2_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_2_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_2_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_2_calendar_1
    ui->screen_2_calendar_1 = lv_calendar_create(ui->screen_2_cont_1);
    lv_obj_set_pos(ui->screen_2_calendar_1, 25, 34);
    lv_obj_set_size(ui->screen_2_calendar_1, 265, 192);
    screen_2_calendar_1_today.year = 2025;
    screen_2_calendar_1_today.month = 3;
    screen_2_calendar_1_today.day = 12;
    lv_calendar_set_today_date(ui->screen_2_calendar_1, screen_2_calendar_1_today.year, screen_2_calendar_1_today.month, screen_2_calendar_1_today.day);
    lv_calendar_set_showed_date(ui->screen_2_calendar_1, screen_2_calendar_1_today.year, screen_2_calendar_1_today.month);
    screen_2_calendar_1_highlihted_days[0].year = 2025;
    screen_2_calendar_1_highlihted_days[0].month = 3;
    screen_2_calendar_1_highlihted_days[0].day = 13;
    lv_calendar_set_highlighted_dates(ui->screen_2_calendar_1, screen_2_calendar_1_highlihted_days, 1);
    lv_obj_t *screen_2_calendar_1_header = lv_calendar_header_arrow_create(ui->screen_2_calendar_1);
    lv_calendar_t *screen_2_calendar_1 = (lv_calendar_t *)ui->screen_2_calendar_1;
    lv_obj_add_event_cb(screen_2_calendar_1->btnm, screen_2_calendar_1_draw_part_begin_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    lv_obj_add_event_cb(ui->screen_2_calendar_1, screen_2_calendar_1_event_handler, LV_EVENT_ALL, NULL);

    //Write style state: LV_STATE_DEFAULT for &style_screen_2_calendar_1_main_main_default
    static lv_style_t style_screen_2_calendar_1_main_main_default;
    ui_init_style(&style_screen_2_calendar_1_main_main_default);

    lv_style_set_border_width(&style_screen_2_calendar_1_main_main_default, 1);
    lv_style_set_border_opa(&style_screen_2_calendar_1_main_main_default, 255);
    lv_style_set_border_color(&style_screen_2_calendar_1_main_main_default, lv_color_hex(0xc0c0c0));
    lv_style_set_border_side(&style_screen_2_calendar_1_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_bg_opa(&style_screen_2_calendar_1_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_2_calendar_1_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_2_calendar_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_screen_2_calendar_1_main_main_default, 4);
    lv_style_set_shadow_color(&style_screen_2_calendar_1_main_main_default, lv_color_hex(0xc9c9c9));
    lv_style_set_shadow_opa(&style_screen_2_calendar_1_main_main_default, 255);
    lv_style_set_shadow_spread(&style_screen_2_calendar_1_main_main_default, 0);
    lv_style_set_shadow_offset_x(&style_screen_2_calendar_1_main_main_default, 0);
    lv_style_set_shadow_offset_y(&style_screen_2_calendar_1_main_main_default, 0);
    lv_style_set_radius(&style_screen_2_calendar_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_2_calendar_1, &style_screen_2_calendar_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_2_calendar_1_extra_header_main_default
    static lv_style_t style_screen_2_calendar_1_extra_header_main_default;
    ui_init_style(&style_screen_2_calendar_1_extra_header_main_default);

    lv_style_set_text_color(&style_screen_2_calendar_1_extra_header_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_screen_2_calendar_1_extra_header_main_default, &lv_font_HYCuHeiJ_12);
    lv_style_set_text_opa(&style_screen_2_calendar_1_extra_header_main_default, 255);
    lv_style_set_bg_opa(&style_screen_2_calendar_1_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_screen_2_calendar_1_extra_header_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_screen_2_calendar_1_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(screen_2_calendar_1_header, &style_screen_2_calendar_1_extra_header_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_2_calendar_1_main_items_default
    static lv_style_t style_screen_2_calendar_1_main_items_default;
    ui_init_style(&style_screen_2_calendar_1_main_items_default);

    lv_style_set_bg_opa(&style_screen_2_calendar_1_main_items_default, 255);
    lv_style_set_bg_color(&style_screen_2_calendar_1_main_items_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_2_calendar_1_main_items_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_2_calendar_1_main_items_default, 1);
    lv_style_set_border_opa(&style_screen_2_calendar_1_main_items_default, 255);
    lv_style_set_border_color(&style_screen_2_calendar_1_main_items_default, lv_color_hex(0xc0c0c0));
    lv_style_set_border_side(&style_screen_2_calendar_1_main_items_default, LV_BORDER_SIDE_FULL);
    lv_style_set_text_color(&style_screen_2_calendar_1_main_items_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_2_calendar_1_main_items_default, &lv_font_HYCuHeiJ_12);
    lv_style_set_text_opa(&style_screen_2_calendar_1_main_items_default, 255);
    lv_obj_add_style(lv_calendar_get_btnmatrix(ui->screen_2_calendar_1), &style_screen_2_calendar_1_main_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);

    //Write codes screen_2_btn_1
    ui->screen_2_btn_1 = lv_button_create(ui->screen_2_cont_1);
    lv_obj_set_pos(ui->screen_2_btn_1, 251, 5);
    lv_obj_set_size(ui->screen_2_btn_1, 47, 27);
    ui->screen_2_btn_1_label = lv_label_create(ui->screen_2_btn_1);
    lv_label_set_text(ui->screen_2_btn_1_label, "X");
    lv_label_set_long_mode(ui->screen_2_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_2_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_2_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_2_btn_1_label, LV_PCT(100));

    //Write style for screen_2_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_2_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_2_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_2_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_2_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_2_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_2_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_2_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_2_btn_1, &lv_font_HYCuHeiJ_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_2_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_2_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_2.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_2);

    //Init events for screen.
    events_init_screen_2(ui);
}
