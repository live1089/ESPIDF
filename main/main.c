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
#include "Guider_ui/custom/custom.h"
#include "Guider_ui/generated/gui_guider.h"



// 声明全局UI实例（与gui_guider.h中的extern声明对应）
lv_ui guider_ui;

void app_main(void)
{
    // 1. 硬件初始化（必须最先执行）
    setup_gpio_init();
    device_lcd_init();

    // 2. LVGL驱动初始化
    soft_drv_lvgl_port_init();

    // 4. 创建UI界面（必须在LVGL锁内操作）
    lvgl_port_lock(0);
    setup_ui(&guider_ui);
    lvgl_port_unlock();

    // 6. 主循环
    for(;;) {

        vTaskDelay(pdMS_TO_TICKS(50)); // 保持50ms周期
    }
}



















