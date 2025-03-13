#include "device_lcd.h"


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


esp_lcd_panel_io_handle_t lcd_io_handle;
esp_lcd_panel_handle_t lcd_panel_handle;


// 定义 SPI 传输完成回调函数
static bool spi_trans_done_callback(esp_lcd_panel_io_handle_t io, esp_lcd_panel_io_event_data_t *edata, void *ctx) {

    return true;
}


void device_lcd_init(void)
{   //初始化spi
     const spi_bus_config_t buscfg = ILI9341_PANEL_BUS_SPI_CONFIG(LCD_SCLK_NUM,
                                                                LCD_MOSI_NUM,
                                                                LCD_H_RES_DATA * LCD_BUFFER_H_DATA * sizeof(uint16_t));//配置SPI引脚
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));// 安装SPI驱动


    //设置其余GPIO口
    const esp_lcd_panel_io_spi_config_t io_config = 
            ILI9341_PANEL_IO_SPI_CONFIG(LCD_CS_NUM,
                                        LCD_DC_NUM,
                                        spi_trans_done_callback,
                                        NULL);
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(  
        (esp_lcd_spi_bus_handle_t)SPI2_HOST, 
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



        // 手动复位屏幕
    gpio_set_level(LCD_RST_NUM, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(LCD_RST_NUM, 1);
    vTaskDelay(pdMS_TO_TICKS(100));


     // 4. 初始化面板
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_panel_handle));
    
    // 6. 设置显示偏移（根据屏幕安装位置调整）
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(lcd_panel_handle, 0, 0)); // 常见偏移设置

    // 8. 开启显示
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_panel_handle, true));


}











