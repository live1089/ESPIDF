#ifndef SETUP_GPIO_H_
#define SETUP_GPIO_H_

#include "driver/gpio.h"


//lcd pin set
#define LCD_BLK_NUM     GPIO_NUM_17
#define LCD_SCLK_NUM    GPIO_NUM_18
#define LCD_MOSI_NUM    GPIO_NUM_23
#define LCD_MISO_NUM    GPIO_NUM_19
#define LCD_RST_NUM     GPIO_NUM_4
#define LCD_DC_NUM      GPIO_NUM_2
#define LCD_CS_NUM      GPIO_NUM_15

/* LCD touch pins */
#define TOUCH_SPI_SCLK       (GPIO_NUM_18)
#define TOUCH_SPI_MISO       (GPIO_NUM_19)
#define TOUCH_GPIO_MOSI      (GPIO_NUM_23)
#define TOUCH_GPIO_CS        (GPIO_NUM_5)


void setup_gpio_init(void);


#endif