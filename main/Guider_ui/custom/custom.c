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
#include "weather.h"


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

char realtime_buffer[512];
char forecast_buffer[512];


static const char *TAG = "custom";
lv_group_t* group;

void setup_focus_navigation(lv_ui *ui) {
    // 创建一个组
    lvgl_port_lock(0);

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

    lvgl_port_unlock();
}


void update_net_status(lv_obj_t *net_label) {
    bool connected = wifi_connected();
    lvgl_port_lock(0);
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
    
    lvgl_port_unlock();
}


void forecast_weather()
{
    forecast(&forecast_weather_data);
    ESP_LOGI(TAG,"获取预报天气");
    lvgl_port_lock(0);
    lv_span_set_text(guider_ui.screen_1_spangroup_1_span, forecast_buffer);
    lv_spangroup_refr_mode(guider_ui.screen_1_spangroup_1);
    lvgl_port_unlock();
}

void realtime_weather()
{
    ESP_LOGI(TAG,"获取实时天气");
    realtime(&parsed_weather_data);
    lvgl_port_lock(0);
    lv_span_set_text(guider_ui.screen_1_spangroup_1_span,realtime_buffer);
    lv_spangroup_refr_mode(guider_ui.screen_1_spangroup_1);
    lvgl_port_unlock();
}

void custom_init(lv_ui *ui)
{
    if (ui == NULL) return;  // 添加检查
    setup_focus_navigation(ui);
}


void realtime(RealTimeWeather *ctx) 
{
    snprintf(realtime_buffer, sizeof(realtime_buffer), 
        "                   实时天气\n\n---------------------------------\n\n城市/区：%s%s\n\n天气：%s        温度：%s℃\n\n风向：%s\n\n风力：%s 级\n\n空气湿度：%s\n\n发布时间：%s", 
        ctx->province,ctx->city,ctx->weather,ctx->temperature,ctx->winddirection,ctx->windpower,ctx->humidity,ctx->reporttime
    );
    // ESP_LOGI("实时天气", "格式化后的天气预报:\n%s", realtime_buffer);
}

void forecast(CityWeather *f)
{
    snprintf(forecast_buffer,sizeof(forecast_buffer),
    "                   预报天气\n\n"
                                                    "------------------------------\n\n"
                                                    "%s %s %s℃\n\n\n"
                                                    "%s %s %s℃\n\n\n"
                                                    "%s %s %s℃\n\n\n"
                                                    "%s %s %s℃",
    f->casts[0].date,f->casts[0].dayweather,f->casts[0].daytemp,
    f->casts[1].date,f->casts[1].dayweather,f->casts[1].daytemp,
    f->casts[2].date,f->casts[2].dayweather,f->casts[2].daytemp,
    f->casts[3].date,f->casts[3].dayweather,f->casts[3].daytemp);

    // ESP_LOGI("预报天气", "格式化后的天气预报:\n%s", forecast_buffer);
}


