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
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"


void digital_clock_count(char *meridiem , DateTime *time)
{
    ESP_LOGI("now time","现在时间是：%d : %d : %d",time->hour,time->minute,time->second);
    if(time->hour == 12 && time->second == 0 && time->minute == 0) {
        if((lv_strcmp(meridiem, "PM") == 0)) {
            lv_strcpy(meridiem, "AM");
        }
        else {
            lv_strcpy(meridiem, "PM");
        }
    }
}

static lv_obj_t * screen_datetext_1_calendar;

void screen_datetext_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_FOCUSED) {
        char * s = lv_label_get_text(btn);
        if(screen_datetext_1_calendar == NULL) {
           
        }
    }
}


bool screen_digital_clock_1_timer_enabled;

void screen_digital_clock_1_timer(lv_timer_t *timer , DateTime *times)
{
    if (!wifi_connected() || !sntp_connection)
    {
        return;
    }

    digital_clock_count(screen_digital_clock_1_meridiem,&current_time);
    
    if (lv_obj_is_valid(guider_ui.screen_digital_clock_1))
    {
        lv_label_set_text_fmt(guider_ui.screen_digital_clock_1, "%d:%02d:%02d", times->hour, times->minute, times->second);

    }
    // else{
    //     lv_timer_delete(timer);
    //     screen_digital_clock_1_timer_enabled = false;
    //     ESP_LOGI("clock","删除定时器");
    // }
}

