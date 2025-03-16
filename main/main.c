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
#include "lv_ui.h"
#include "setup_uart.h"
#include "esp_log.h"  // ESP-IDF 日志库
#include "device_lcd.h"
#include "Digital_key.h"

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);
    
    // 1. 硬件初始化（必须最先执行）
    setup_gpio_init();
    // setup_uart_init();
    // device_lcd_init();
    // touch_init();
    
    // 2. LVGL驱动初始化
    // soft_drv_lvgl_port_init();

    // 4. 创建UI界面（必须在LVGL锁内操作）
    // lvgl_port_lock(0);
    // lv_ui_init();
    // lvgl_port_unlock();


    // 6. 主循环
    for(;;) {
        button_task();
    }
}


