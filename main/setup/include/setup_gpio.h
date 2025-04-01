#ifndef SETUP_GPIO_H_
#define SETUP_GPIO_H_

#include "driver/gpio.h"
#include "driver/ledc.h"

#define LCD_SPI_NUM         (SPI2_HOST)
//lcd pin set
#define LCD_BLK_NUM     GPIO_NUM_17
#define LCD_SCLK_NUM    GPIO_NUM_18
#define LCD_MOSI_NUM    GPIO_NUM_23
#define LCD_MISO_NUM    GPIO_NUM_19
#define LCD_RST_NUM     GPIO_NUM_4
#define LCD_DC_NUM      GPIO_NUM_2
#define LCD_CS_NUM      GPIO_NUM_15
#define LCD_LEDC_CH  LEDC_CHANNEL_0  // 使用 LEDC 通道 0

/* LCD touch pins */
#define TOUCH_SPI_SCLK       (GPIO_NUM_18)
#define TOUCH_SPI_MISO       (GPIO_NUM_19)
#define TOUCH_GPIO_MOSI      (GPIO_NUM_23)
#define TOUCH_GPIO_CS        (5)
#define TOUCH_GPIO_IRQ       (GPIO_NUM_21)


/*按键 pins*/
#define KEY_UP     GPIO_NUM_27
#define KEY_DOWN   GPIO_NUM_26
#define KEY_LEFT   GPIO_NUM_25
#define KEY_RIGHT  GPIO_NUM_33
#define KEY_CENTER GPIO_NUM_32
#define KEY_SET    GPIO_NUM_35



void setup_gpio_init(void);
esp_err_t init_backlight_pwm();

#endif