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
#include "widgets_init.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"

void ui_init_style(lv_style_t * style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

/* void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del)
{
    lv_obj_t * act_scr = lv_screen_active();

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "gg_external_data.h"
    if(auto_del) {
        gg_edata_task_clear(act_scr);
    }
#endif
    if (auto_del && is_clean) {
        lv_obj_remove_event_cb(act_scr, NULL);
        lv_obj_clean(act_scr);
    }
    if (new_scr_del) {
        setup_scr(ui);
    }
    lv_screen_load_anim(*new_scr, anim_type, time, delay, auto_del);
    *old_scr_del = auto_del;
} */

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del)
{
    vTaskDelay(pdMS_TO_TICKS(100));
    lvgl_port_lock(0);
    lv_obj_t * act_scr = lv_screen_active();
    lv_group_t *group = lv_obj_get_group(act_scr);

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "gg_external_data.h"
    if(auto_del) {
        gg_edata_task_clear(act_scr);
    }
#endif
    if (auto_del && is_clean && lv_obj_is_valid(act_scr)) {
        lv_obj_remove_event_cb(act_scr, NULL);
        ESP_LOGI("uI", "Cleaning old screen: %p", act_scr);
        lv_obj_clean(act_scr);
    }
    if (new_scr_del && !lv_obj_is_valid(*new_scr)) {
        setup_scr(ui);
    }

    *old_scr_del = auto_del;
    if (lv_obj_is_valid(*new_scr)) {
        time = (anim_type != LV_SCR_LOAD_ANIM_NONE) ? time : 0;
        delay = (anim_type != LV_SCR_LOAD_ANIM_NONE) ? delay : 0;
        ESP_LOGI("UI", "Loading screen %p with anim %d", *new_scr, anim_type);
        lv_screen_load_anim(*new_scr, anim_type, time, delay, auto_del);
    } else {
        ESP_LOGE("UI", "New screen is invalid!");
    }
    
    if (auto_del && lv_obj_is_valid(act_scr)) {
        if (group)
        {
            lv_group_remove_all_objs(group);
        }
        lv_obj_delete(act_scr); // 显式删除旧页面
        act_scr = NULL;      // 指针置空
    }
    
    lv_indev_t *indev = lv_indev_get_next(NULL);
    while (indev) {
        if (lv_indev_get_type(indev) == LV_INDEV_TYPE_ENCODER) {
            lv_group_t *group = lv_indev_get_group(indev);
            if (group) {
                lv_group_set_editing(group, false);  // 退出编辑模式
                lv_group_focus_obj(NULL);            // 清空焦点
                ESP_LOGI("UI", "Reset focus for group %p", group);
            }
        }
        indev = lv_indev_get_next(indev);
    }
    lvgl_port_unlock();
}

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, var);
    lv_anim_set_exec_cb(&anim, exec_cb);
    lv_anim_set_values(&anim, start_value, end_value);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_path_cb(&anim, path_cb);
    lv_anim_set_repeat_count(&anim, repeat_cnt);
    lv_anim_set_repeat_delay(&anim, repeat_delay);
    lv_anim_set_playback_time(&anim, playback_time);
    lv_anim_set_playback_delay(&anim, playback_delay);
    if (start_cb) {
        lv_anim_set_start_cb(&anim, start_cb);
    }
    if (ready_cb) {
        lv_anim_set_completed_cb(&anim, ready_cb);
    }
    if (deleted_cb) {
        lv_anim_set_deleted_cb(&anim, deleted_cb);
    }
    lv_anim_start(&anim);
}

void init_scr_del_flag(lv_ui *ui)
{

    ui->screen_del = true;
    ui->screen_1_del = true;
    ui->screen_2_del = true;
    ui->screen_3_del = true;
    ui->screen_4_del = true;
    ui->screen_5_del = true;
    ui->screen_6_del = true;
    ui->screen_7_del = true;
    ui->screen_8_del = true;
    ui->screen_9_del = true;
    ui->screen_10_del = true;
}

void setup_bottom_layer(void)
{
    lv_theme_apply(lv_layer_bottom());
}

void setup_ui(lv_ui *ui)
{
    lvgl_port_lock(0);
    setup_bottom_layer();
    init_scr_del_flag(ui);
    init_keyboard(ui);
    setup_scr_screen(ui);
    lv_screen_load(ui->screen);
    lvgl_port_unlock();
}

void video_play(lv_ui *ui)
{

}

void init_keyboard(lv_ui *ui)
{

}
