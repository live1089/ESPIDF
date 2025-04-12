#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include <stdint.h>

#include "setup_gpio.h"
#include "setup_spi.h"
#include "device_lcd.h"
#include "soft_drv_lvgl_port.h"
#include "lv_ui.h"
#include "setup_uart.h"
#include "setup_wifi.h"
#include "esp_log.h"  
#include "device_lcd.h"
#include "Digital_key.h"
#include "weather.h"
#include "Digital_key.h"
#include "..\Guider_ui\generated\events_init.h"
#include "Guider_ui/custom/custom.h"
#include "setup_rtc.h"
#include "event_bits.h"
#include "setup_nvs.h"
lv_ui guider_ui; 
// static const char *TAG = "main";
EventGroupHandle_t task_events;
/*
1. 任务分配原则
任务模块	优先级	堆栈大小	执行频率	关键特性
​传感器读取	5	2~3KB	1~5秒/次	高实时性
​Wi-Fi 连接	4	4~5KB	事件驱动	网络稳定性优先
​天气更新	3	5~6KB	5~10分钟/次	依赖网络，低实时性
​屏幕刷新	6	6~8KB	30~60 FPS	高优先级，避免卡顿
​用户输入	7	2~3KB	事件驱动	即时响应（如触屏/按键）
​系统设置	2	3~4KB	按需触发	低优先级，后台处理
*/
/* 任务堆栈监控 */
void stack_monitor(void *pvParameters) {
    TaskHandle_t *task_handle = (TaskHandle_t *)pvParameters;
    lv_mem_monitor_t mon;
    while(1) {
        lv_mem_monitor(&mon);
        ESP_LOGI("MEM", "Free: %luKB", esp_get_free_heap_size()/1024);
        ESP_LOGI("STACK", "Free Stack: %u bytes", 
                (uxTaskGetStackHighWaterMark(*task_handle)*4));
        // ESP_LOGI("LVGL_MEM", "Used: %d, Frag: %d%%", mon.total_size - mon.free_size, mon.frag_pct);
        ESP_LOGI("WIFI_MEM", "Used: %d, Frag: %d%%", mon.total_size - mon.free_size, mon.frag_pct);
        // sys_run_time();
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

const char *ssid = wifi_ssid;
const char *password = wifi_password;
void Hardware_init()
{
    // 硬件初始化
    setup_gpio_init();
    init_backlight_pwm();
    gpio_key_init();
    device_lcd_init();
    soft_drv_lvgl_port_init();
    nvs_init();
    save_wifi_config(wifi_ssid,wifi_password);
    // 驱动初始化
    wifi_init_sta(ssid, password);
    http_init();
    sntp_init_get_time();
    // UI初始化
    lvgl_port_lock(0);
    lv_init();
    setup_ui(&guider_ui);
    custom_init(&guider_ui);
    lvgl_port_unlock();
}


void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);
    task_events = xEventGroupCreate();
    Hardware_init();

    TaskHandle_t lvgl_stack_handle = NULL;
    TaskHandle_t wifi_task_handle = NULL;
    TaskHandle_t weather_task_handle = NULL;
    TaskHandle_t periodic_sync_task_handle = NULL;
    xTaskCreatePinnedToCore(lvgl_event_task, "lvgl_event_task", 6144, NULL, 5, &lvgl_stack_handle, 1);
    xTaskCreatePinnedToCore(wifi_task, "wifi_task", 4096, NULL, 4, &wifi_task_handle, 0);
    xTaskCreatePinnedToCore(weather_task,"weather_task",4096,NULL,3,&weather_task_handle,0);

    xTaskCreatePinnedToCore(periodic_sync_task, "periodic_sync", 2048, NULL, 4, &periodic_sync_task_handle,0);

    // 创建低优先级监控任务
    // xTaskCreatePinnedToCore(stack_monitor, "stack_mon", 2048, &lvgl_stack_handle, 1, NULL,0);
    // xTaskCreatePinnedToCore(stack_monitor, "stack_mon", 2048, &wifi_task_handle, 1, NULL,0);
  
    // 6. 主循环
    for(;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

