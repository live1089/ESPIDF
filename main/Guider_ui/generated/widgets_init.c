/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include "gui_guider.h"
#include "widgets_init.h"
#include "lvgl__lvgl/src/draw/lv_draw_private.h"
#include <stdlib.h>
#include <string.h>


__attribute__((unused)) void kb_event_cb (lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *kb = lv_event_get_target(e);
    if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

__attribute__((unused)) void ta_event_cb (lv_event_t *e) {
#if LV_USE_KEYBOARD
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);

    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_active()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if(code == LV_EVENT_READY) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);
    }
#endif
}

void clock_count(int *hour, int *min, int *sec)
{
    (*sec)++;
    if(*sec == 60)
    {
        *sec = 0;
        (*min)++;
    }
    if(*min == 60)
    {
        *min = 0;
        if(*hour < 12)
        {
            (*hour)++;
        } else {
            (*hour)++;
            *hour = *hour %12;
        }
    }
}

void digital_clock_count(int * hour, int * minute, int * seconds, char * meridiem)
{

    (*seconds)++;
    if(*seconds == 60) {
        *seconds = 0;
        (*minute)++;
    }
    if(*minute == 60) {
        *minute = 0;
        if(*hour < 12) {
            (*hour)++;
        }
        else {
            (*hour)++;
            (*hour) = (*hour) % 12;
        }
    }
    if(*hour == 12 && *seconds == 0 && *minute == 0) {
        if((lv_strcmp(meridiem, "PM") == 0)) {
            lv_strcpy(meridiem, "AM");
        }
        else {
            lv_strcpy(meridiem, "PM");
        }
    }
}


extern int screen_0_digital_clock_1_hour_value;
extern int screen_0_digital_clock_1_min_value;
extern int screen_0_digital_clock_1_sec_value;
extern char screen_0_digital_clock_1_meridiem[];

void screen_0_digital_clock_1_timer(lv_timer_t *timer)
{
    digital_clock_count(&screen_0_digital_clock_1_hour_value, &screen_0_digital_clock_1_min_value, &screen_0_digital_clock_1_sec_value, screen_0_digital_clock_1_meridiem);
    if (lv_obj_is_valid(guider_ui.screen_0_digital_clock_1))
    {
        lv_label_set_text_fmt(guider_ui.screen_0_digital_clock_1, "%d:%02d:%02d %s", screen_0_digital_clock_1_hour_value, screen_0_digital_clock_1_min_value, screen_0_digital_clock_1_sec_value, screen_0_digital_clock_1_meridiem);
    }
}
static lv_obj_t * screen_0_datetext_1_calendar;

void screen_0_datetext_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_FOCUSED) {
        char * s = lv_label_get_text(btn);
        if(screen_0_datetext_1_calendar == NULL) {
            screen_0_datetext_1_init_calendar(btn, s);
        }
    }
}

void screen_0_datetext_1_init_calendar(lv_obj_t *obj, char * s)
{
    if (screen_0_datetext_1_calendar == NULL) {
        lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
        screen_0_datetext_1_calendar = lv_calendar_create(lv_layer_top());
        lv_obj_t * scr = lv_obj_get_screen(obj);
        int32_t scr_height = lv_obj_get_height(scr);
        int32_t scr_width = lv_obj_get_width(scr);
        lv_obj_set_size(screen_0_datetext_1_calendar, scr_width * 0.8, scr_height * 0.8);
        char * year = strtok(s, "/");
        char * month = strtok(NULL, "/");
        char * day = strtok(NULL, "/");
        lv_calendar_set_showed_date(screen_0_datetext_1_calendar, atoi(year), atoi(month));
        lv_calendar_date_t highlighted_days[1];       /*Only its pointer will be saved so should be static*/
        highlighted_days[0].year = atoi(year);
        highlighted_days[0].month = atoi(month);
        highlighted_days[0].day = atoi(day);
        lv_calendar_set_highlighted_dates(screen_0_datetext_1_calendar, highlighted_days, 1);
        lv_obj_align(screen_0_datetext_1_calendar,LV_ALIGN_CENTER, 0, 0);

        lv_obj_add_event_cb(screen_0_datetext_1_calendar, screen_0_datetext_1_calendar_event_handler, LV_EVENT_ALL,NULL);
        lv_calendar_header_arrow_create(screen_0_datetext_1_calendar);
        lv_obj_update_layout(scr);
    }
}

void screen_0_datetext_1_calendar_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t date;
        lv_calendar_get_pressed_date(obj,&date);
        char buf[16];
        lv_snprintf(buf,sizeof(buf),"%d/%02d/%02d", date.year, date.month,date.day);
        lv_label_set_text(guider_ui.screen_0_datetext_1, buf);
        lv_obj_remove_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
        lv_obj_delete(screen_0_datetext_1_calendar);
        screen_0_datetext_1_calendar = NULL;
    }
}

void screen_2_calendar_1_draw_part_begin_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_task_t * draw_task = lv_event_get_param(e);
    lv_draw_dsc_base_t * base_dsc = draw_task->draw_dsc;
    if(base_dsc->part != LV_PART_ITEMS) return;

    lv_draw_label_dsc_t * label_dsc = lv_draw_task_get_label_dsc(draw_task);
    lv_draw_border_dsc_t * border_dsc = lv_draw_task_get_border_dsc(draw_task);
    lv_draw_fill_dsc_t * fill_dsc = lv_draw_task_get_fill_dsc(draw_task);

    if(base_dsc->id1 < 7) {
        if(label_dsc) label_dsc->color = lv_color_hex(0x0D3055);
        if(label_dsc) label_dsc->font = &lv_font_HYCuHeiJ_12;
    } else if (lv_buttonmatrix_has_button_ctrl(obj, base_dsc->id1, LV_BUTTONMATRIX_CTRL_DISABLED)) {
        if(label_dsc) label_dsc->color = lv_color_hex(0xA9A2A2);
        if(label_dsc) label_dsc->font = &lv_font_HYCuHeiJ_12;
        if(fill_dsc) fill_dsc->opa = 255;
        if(fill_dsc) fill_dsc->color = lv_color_hex(0xffffff);
    }
    if(lv_buttonmatrix_has_button_ctrl(obj, base_dsc->id1, LV_BUTTONMATRIX_CTRL_CUSTOM_1)) {
        if(label_dsc) label_dsc->color = lv_color_hex(0x0D3055);
        if(label_dsc) label_dsc->font = &lv_font_HYCuHeiJ_12;
        if(fill_dsc) fill_dsc->opa = 255;
        if(fill_dsc) fill_dsc->color = lv_color_hex(0x01a2b1);
        if(border_dsc) border_dsc->opa = 255;
        if(border_dsc) border_dsc->width = 1;
        if(border_dsc) border_dsc->color = lv_color_hex(0xc0c0c0);
    }
    if(lv_buttonmatrix_has_button_ctrl(obj, base_dsc->id1, LV_BUTTONMATRIX_CTRL_CUSTOM_2)) {
        if(label_dsc) label_dsc->color = lv_color_hex(0x0D3055);
        if(label_dsc) label_dsc->font = &lv_font_HYCuHeiJ_12;
        if(fill_dsc) fill_dsc->opa = 255;
        if(fill_dsc) fill_dsc->color = lv_color_hex(0x2195f6);
    } else {
    }
}

void screen_2_calendar_1_event_handler(lv_event_t * e)
{
    lv_calendar_date_t date;
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * obj = lv_event_get_current_target(e);
        lv_calendar_get_pressed_date(obj,&date);
        lv_calendar_set_highlighted_dates(obj, &date, 1);
    }
}

