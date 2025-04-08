#ifndef SETUP_RTC_H
#define SETUP_RTC_H

#include "freertos/FreeRTOS.h"
#include "driver/rtc_io.h"
#include "esp32/rtc.h"
#include <sys/time.h>
#include "esp_sntp.h"
#include "time.h"
#include "setup_nvs.h"
#include "esp_timer.h"
#include "setup_wifi.h"
#include "esp_netif_sntp.h"
#include <netdb.h>
// #include <arpa/inet.h>
// #include "esp32/rom/rtc.h"
#include "esp_sleep.h"
#include "esp_log.h"
// #include "esp_private/rtc_ctrl.h"
#include "soc/rtc.h"

#define MAX_RETRY 10  // 改为10次
#define DELAY_MS 1000 // 每次等待1秒

extern bool sntp_connection;

typedef struct {
    uint16_t year : 12;  // 12位存储年份（0~4095）
    uint8_t month : 4;   // 4位存储月份（0~15）
    uint8_t day : 5;     // 5位存储日（0~31）
    uint8_t hour : 5;    // 5位存储小时（0~23）
    uint8_t minute : 6;   // 6位存储分钟（0~59）
    uint8_t second : 6;   // 6位存储秒（0~59）
} DateTime;

extern DateTime current_time;

void periodic_sync_task(void *pvParams);

void sys_run_time();
void convert_to_datetime(time_t timestamp, DateTime *dt);
#endif