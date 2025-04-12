/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../Guider_ui/generated/gui_guider.h"
#include "weather.h"

extern lv_timer_t *clock_timer;
extern uint8_t brightness_value;
extern bool screen_4_wifi_timer_enabled;
extern lv_timer_t *WIFI_timer;

extern bool screen_3_timer_enabled;
extern lv_timer_t * timerss;
void sysmon_update(lv_timer_t *timerss);
void custom_init(lv_ui *ui);
void forecast_weather();
void realtime_weather();
void realtime(RealTimeWeather *ctx);
void forecast(CityWeather *f);
void update_net_status();
void updata_net_label();
void WIFI_NOW(lv_timer_t *timer);
char* sys_stack();
char* sys_heap();
char* info_sys();
#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
