/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "../Guider_ui/custom/custom.h"

/*********************
 *      DEFINES
 *********************/
#include "soft_drv_lvgl_port.h"
#include "../generated/gui_guider.h"
#include "esp_log.h"
#include "device_lcd.h"
#include "setup_wifi.h"
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/




void custom_handle_brightness(uint8_t value) {  

    hardware_set_brightness(value);  
    // 2. 界面更新  
    lv_label_set_text_fmt(guider_ui.screen_4_label_1, "%d", value);
    
    
}
static const char *TAG = "custom";
lv_group_t* group;

void setup_focus_navigation(lv_ui *ui) {
    // 创建一个组
    group = lv_group_create();
    lv_indev_set_group(indev, group);
    lv_group_set_default(group);

    lv_group_t* indev_group = lv_indev_get_group(indev);
    if (indev_group == group) {
        ESP_LOGI(TAG, "Input device is associated with the group.");
    } else {
        ESP_LOGE(TAG, "Input device is NOT associated with the group!");
    }

    if (ui->screen_btn_1) lv_group_add_obj(group, ui->screen_btn_1);
    if (ui->screen_btn_2) lv_group_add_obj(group, ui->screen_btn_2);
    if (ui->screen_btn_3) lv_group_add_obj(group, ui->screen_btn_3);
    lv_group_focus_obj(ui->screen_btn_2);  // 设置默认焦点

}




void update_net_status(lv_obj_t *net_label) {
    bool connected = wifi_connected();
    if (connected) {
        // 如果有网，显示label
        lv_obj_clear_flag(guider_ui.screen_label_1, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(guider_ui.screen_label_1, "" LV_SYMBOL_WIFI " ");
        lv_led_on(guider_ui.screen_4_led_1);
    } else {
        // 如果没网，隐藏label
        lv_obj_add_flag(guider_ui.screen_label_1, LV_OBJ_FLAG_HIDDEN);
        lv_led_off(guider_ui.screen_4_led_1);
    }
}




void custom_init(lv_ui *ui)
{
    if (ui == NULL) return;  // 添加检查
    setup_focus_navigation(ui);
}
