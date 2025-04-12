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
#include "setup_rtc.h"

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
lv_timer_t *WIFI_timer;
bool screen_4_wifi_timer_enabled;
void WIFI_NOW(lv_timer_t *WIFI_timer)
{
    lvgl_port_lock(0);
    if (lv_obj_is_valid(guider_ui.screen_4_label_1) && lv_obj_is_valid(guider_ui.screen_4_led_1)) {
        if (wifi_connected()) {
            // 如果网络连接成功
            lv_led_on(guider_ui.screen_4_led_1);  // 打开 LED
            lv_label_set_text_fmt(guider_ui.screen_4_label_1, "网络连接：%s", wifi_ssid);  // 更新标签文本
        } else{
            lv_led_off(guider_ui.screen_4_led_1);  // 关闭 LED
            lv_label_set_text(guider_ui.screen_4_label_1, "网络连接: ");  // 更新标签文本
        } 
    }else{
        lv_timer_delete(WIFI_timer);
        screen_4_wifi_timer_enabled = false;
        ESP_LOGI("WIFI","删除WIFI状态定时器");
    }
    lvgl_port_unlock();
}


void update_net_status() {
    lvgl_port_lock(0);
    // 检查标签是否有效
    if (lv_obj_is_valid(guider_ui.screen_4_label_1)) {
        lv_label_set_text(guider_ui.screen_4_label_1, "网络连接: Connecting.......");
    }
    lvgl_port_unlock();
}

void updata_net_label()
{
    lvgl_port_lock(0);
    if (lv_obj_is_valid(guider_ui.screen_4_label_1) && lv_obj_is_valid(guider_ui.screen_4_led_1)) {
        lv_led_off(guider_ui.screen_4_led_1);  // 关闭 LED
        lv_label_set_text(guider_ui.screen_4_label_1, "网络连接: disconnect"); 
    }
    lvgl_port_unlock();
}

uint8_t brightness_value;
lv_timer_t * timer;
bool screen_5_slider_timer_enabled;
void Brightness_update(lv_timer_t *timer)
{
    if (lv_obj_is_valid(guider_ui.screen_5_label_1))
    {
        brightness_value = lv_slider_get_value(guider_ui.screen_5_slider_1);
        lv_label_set_text_fmt(guider_ui.screen_5_label_1, "%d", brightness_value);
        hardware_set_brightness(brightness_value);
        
    }else{
        ESP_LOGI(TAG,"删除亮度状态定时器");
        screen_5_slider_timer_enabled = false;
        lv_timer_delete(timer);
    }
}


void forecast_weather()
{
    forecast(&forecast_weather_data);
    // ESP_LOGI(TAG,"获取预报天气");
    lvgl_port_lock(0);
    lv_span_set_text(guider_ui.screen_1_spangroup_1_span, forecast_buffer);
    lv_spangroup_refr_mode(guider_ui.screen_1_spangroup_1);
    lvgl_port_unlock();
}

void realtime_weather()
{
    // ESP_LOGI(TAG,"获取实时天气");
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

char* sys_stack()
{
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    static char stack_str[64];
    sprintf(stack_str, "UI堆栈已使用: %dKB,总堆栈: %dKB 碎片率:%d%%",  mon.total_size - mon.free_size, mon.total_size , mon.frag_pct);
    return stack_str;
}
char* sys_heap()
{
    static char heap_str[64];
    sprintf(heap_str,"系统可用堆为：%luKB,总堆：%uKB",esp_get_free_heap_size()/1024,heap_caps_get_total_size(MALLOC_CAP_8BIT));
    // size_t total_heap_size = heap_caps_get_total_size(MALLOC_CAP_8BIT);
    return heap_str;
}
char* info_sys()
{
    static char info_text[256];  // 假设信息总长度不超过 256 字符
    snprintf(info_text, sizeof(info_text), "%s\n\n\n%s\n\n\n%s",
            sys_run_time(), sys_stack(), sys_heap());
    return info_text;
}


bool screen_3_timer_enabled = false;
void sysmon_update(lv_timer_t *timerss)
{
    lvgl_port_lock(0);
    if (lv_obj_is_valid(guider_ui.screen_3_btn_1) && lv_obj_is_valid(guider_ui.screen_3_btn_2)) {
        lv_span_set_text(guider_ui.screen_3_spangroup_1_span ,info_sys());
    }else{
        lv_timer_delete(timerss);
        screen_3_timer_enabled = false;
        ESP_LOGI("system","删除系统状态定时器");
    }
    lvgl_port_unlock();
}