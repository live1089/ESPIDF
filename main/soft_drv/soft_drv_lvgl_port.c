#include "soft_drv_lvgl_port.h"
#include "setup_gpio.h"
#include "setup_spi.h"
#include "device_lcd.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"

#define LCD_DRAW_BUFF_HEIGHT (40)     // 绘图缓冲区高度（行数）
#define LCD_BL_NO_LEVEL (1)           // 背光引脚电平（1 = 开启）
#define LCD_H_RES (LCD_H_RES_DATA)    // 屏幕水平分辨率（320）
#define LCD_V_RES (240)               // 屏幕垂直分辨率
#define LCD_DEAW_BUFF_DOUBLE (0)      // 是否启用双缓冲（0 = 禁用）


static lv_display_t *lvgl_disp = NULL;// LVGL 显示设备句柄

void soft_drv_lvgl_port_init(void)
{
    ESP_ERROR_CHECK(gpio_set_level(LCD_BLK_NUM,LCD_BL_NO_LEVEL));

    lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,          // LVGL 任务优先级（中等）
        .task_stack = 8 * 1024,        // 任务栈大小（8KB）
        .task_affinity = -1,         // 不绑定到特定 CPU 核心
        .task_max_sleep_ms = 500,    // 任务最大休眠时间（500ms）
        .timer_period_ms = 5         // LVGL 定时器周期（5ms）
    };
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

lvgl_port_display_cfg_t disp_cfg = {
    .io_handle = lcd_io_handle,     // LCD 底层 IO 句柄（如 SPI 句柄）
    .panel_handle = lcd_panel_handle, // LCD 面板配置句柄
    .buffer_size = LCD_H_RES * LCD_DRAW_BUFF_HEIGHT, // 缓冲区大小（320 * 24）
    .double_buffer = LCD_DEAW_BUFF_DOUBLE, // 禁用双缓冲
    .hres = LCD_H_RES,              // 水平分辨率 320
    .vres = LCD_V_RES,              // 垂直分辨率 240
    .monochrome = false,            // 彩色显示（非单色）
#if LVGL_VERSION_MAJOR >= 9
    .color_format = LV_COLOR_FORMAT_RGB565, // LVGL 9+ 颜色格式
#endif
    .rotation = {
        .swap_xy = false,           // 不交换 X/Y 轴
        .mirror_x = false,          // 不水平镜像
        .mirror_y = true            // 垂直镜像（适配屏幕物理方向）
    },
    .flags = {
        .buff_dma = true,           // 启用 DMA 传输缓冲区
#if LVGL_VERSION_MAJOR >= 9
        .swap_bytes = true          // 交换字节序（适配硬件）
#endif
    }
};
lvgl_disp = lvgl_port_add_disp(&disp_cfg); // 添加显示设备到 LVGL

}







