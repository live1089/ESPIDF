#include "device_lcd.h"
#include "lvgl.h"
// #include "esp_lcd_touch_xpt2046.h"
// #include "esp_lcd_touch.h"
#include "esp_log.h"
/*
esp_lcd_panel_reset()
硬件/软件复位屏幕

esp_lcd_panel_init()
初始化屏幕（发送初始化命令序列）

esp_lcd_panel_draw_bitmap()
向屏幕写入图像数据

esp_lcd_panel_mirror()
水平/垂直镜像显示

esp_lcd_panel_swap_xy()
交换 X/Y 轴（旋转屏幕）

esp_lcd_panel_set_gap()
设置显示偏移

esp_lcd_panel_disp_on_off()
开启/关闭显示
发送 DISP_ON (0x29) 或 DISP_OFF (0x28) 命令
*/


esp_lcd_panel_io_handle_t lcd_io_handle = NULL;
esp_lcd_panel_handle_t lcd_panel_handle = NULL;
static const char *TAG = "device_lcd";

// 定义 SPI 传输完成回调函数
// static bool spi_trans_done_callback(esp_lcd_panel_io_handle_t io, esp_lcd_panel_io_event_data_t *edata, void *ctx) {

//     return true;
// }


void device_lcd_init(void)
{   //初始化spi
     const spi_bus_config_t buscfg = ILI9341_PANEL_BUS_SPI_CONFIG(LCD_SCLK_NUM,
                                                                LCD_MOSI_NUM,
                                                                LCD_H_RES_DATA * LCD_BUFFER_H_DATA * sizeof(uint16_t));//配置SPI引脚
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO));// 安装SPI驱动


    //设置其余GPIO口
    const esp_lcd_panel_io_spi_config_t io_config = 
            ILI9341_PANEL_IO_SPI_CONFIG(LCD_CS_NUM,
                                        LCD_DC_NUM,
                                        NULL,
                                        NULL);
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(  
        (esp_lcd_spi_bus_handle_t)LCD_SPI_NUM, 
                                &io_config, 
                                &lcd_io_handle));


    // 3. 创建面板实例
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST_NUM,  // 需要定义复位引脚
        .color_space = ESP_LCD_COLOR_SPACE_RGB,
        // .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,           // RGB565格式
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(lcd_io_handle, &panel_config, &lcd_panel_handle));


     esp_lcd_panel_reset(lcd_panel_handle);
     // 4. 初始化面板
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_panel_handle));

    hardware_set_brightness(50);
    
    // 6. 设置显示偏移（根据屏幕安装位置调整）
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(lcd_panel_handle, 0, 0)); // 常见偏移设置

    // 8. 开启显示
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_panel_handle, true));


}


void lcd_deinit(void)
{
    esp_lcd_panel_del(lcd_panel_handle);
    esp_lcd_panel_io_del(lcd_io_handle);
    spi_bus_free(LCD_SPI_NUM);
    gpio_reset_pin(LCD_BLK_NUM);
}


esp_err_t hardware_set_brightness(uint8_t brightness_percent) {
    if (brightness_percent > 100) {
        brightness_percent = 100;
    } else if (brightness_percent < 0) {
        brightness_percent = 0;
    }

    ESP_LOGI(TAG, "Setting LCD backlight: %d%%", brightness_percent);
    uint32_t duty_cycle = (1023 * brightness_percent) / 100;  // 将百分比转换为占空比
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));

    return ESP_OK;
}