#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"


#include "setup_gpio.h"
#include "setup_spi.h"
#include "device_lcd.h"
#include "soft_drv_lvgl_port.h"
// #include "lv_ui.h"
#include "setup_uart.h"
#include "setup_wifi.h"
#include "esp_log.h"  // ESP-IDF 日志库
#include "device_lcd.h"
#include "Digital_key.h"
#include "Guider_ui/custom/custom.h"



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


lv_ui guider_ui; 

#if 0

// 定义全局通信资源
QueueHandle_t display_queue;
EventGroupHandle_t network_events;
SemaphoreHandle_t weather_mutex;



/*
传感器读取任务
• ​功能：读取温度、湿度等传感器数据。
• ​优先级：较高（但低于屏幕刷新和用户输入）。
• ​通信方式：通过 ​队列（Queue）​ 将数据发送到显示任务。
*/
void sensor_task(void *pvParams) {
    while(1) {
        // float temp = read_temperature();
        // xQueueSend(display_queue, &temp, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1秒更新一次
    }
}








/*
天气更新任务
• ​功能：通过 API 获取天气数据（依赖 Wi-Fi）。
• ​优先级：较低，避免阻塞关键任务。
• ​通信方式：接收 Wi-Fi 事件，通过 ​互斥锁（Mutex）​ 更新共享数据。
*/
void weather_task(void *pvParams) {
    while(1) {
        // 等待Wi-Fi连接成功
        // xEventGroupWaitBits(network_events, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        
        // 获取天气数据

        
        vTaskDelay(pdMS_TO_TICKS(300000)); // 5分钟更新一次
    }
}


/*
屏幕刷新任务
• ​优先级最高：确保显示流畅。
• ​通信方式：接收传感器和天气数据，使用 ​双缓冲 避免撕裂。
*/
void display_task(void *pvParams) {
    while(1) {




        vTaskDelay(pdMS_TO_TICKS(16)); // ~60Hz
    }
}




/*
 用户输入任务
• ​最高优先级：即时响应触屏或按键。
• ​通信方式：通过 ​队列 或 ​直接调用 触发设置界面。
*/
void input_task(void *pvParams) {
    while(1) {
        if (check_touch_event()) {
            open_settings_menu(); // 直接跳转设置界面
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // 20Hz检测频率
    }
}

/*
系统资源分配
​① 内存与堆栈
• 使用 uxTaskGetStackHighWaterMark() 监控堆栈：
*/
void debug_stack_usage() {
    UBaseType_t stack = uxTaskGetStackHighWaterMark(NULL);
    printf("Task '%s'剩余堆栈: %d\n", pcTaskGetName(NULL), stack);
}

/*
CPU 核心分配（针对双核 ESP32）​
• 将高优先级任务绑定到 Core 1，低优先级任务绑定到 Core 0：
*/
// xTaskCreatePinnedToCore(display_task, "display", 8192, NULL, 6, NULL, 1);
// xTaskCreatePinnedToCore(wifi_task, "wifi", 4096, NULL, 4, NULL, 0);


#endif





void Hardware_init()
{
    // 1. 硬件初始化
    setup_gpio_init();
    // setup_uart_init();
    // device_lcd_init();
    // 2. LVGL驱动初始化
    // soft_drv_lvgl_port_init();
}

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);
    
    Hardware_init();


        // 创建网络事件组
    // network_events = xEventGroupCreate();
    // 启动 Wi-Fi 任务（优先级高于HTTP任务）
    xTaskCreate(wifi_task, "wifi_task", 4096, NULL, 4, NULL);



    // // 4. 创建UI界面（必须在LVGL锁内操作）
    // lvgl_port_lock(0);
    // setup_ui(&guider_ui);
    // lvgl_port_unlock();


    // 6. 主循环
    for(;;) {
        // button_task();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }








    //     // 初始化通信资源
    // display_queue = xQueueCreate(10, sizeof(float));
    // network_events = xEventGroupCreate();
    // weather_mutex = xSemaphoreCreateMutex();

    // // 创建任务（按优先级从高到低）
    // xTaskCreate(display_task, "display", 8192, NULL, 6, NULL);
    // xTaskCreate(input_task, "input", 2048, NULL, 7, NULL);
    // xTaskCreate(sensor_task, "sensor", 2048, NULL, 5, NULL);
    // xTaskCreate(wifi_task, "wifi", 4096, NULL, 4, NULL);
    // xTaskCreate(weather_task, "weather", 4096, NULL, 3, NULL);

    // 主任务可结束或保留为监控任务
    // vTaskDelete(NULL);
}



/*低功耗任务*/
// void enter_sleep_mode() {
//     // screen_set_brightness(10); // 降低亮度
//     // vTaskDelay(pdMS_TO_TICKS(100)); // 10 FPS
// }
