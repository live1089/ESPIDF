#ifndef DEVICE_LCD_H_
#define DEVICE_LCD_H_


#include "esp_lcd_ili9341.h"
#include "setup_gpio.h"
#include "setup_spi.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"

extern esp_lcd_panel_io_handle_t   lcd_io_handle;
extern esp_lcd_panel_handle_t      lcd_panel_handle;



void device_lcd_init(void);


#endif