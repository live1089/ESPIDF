#include "setup_gpio.h"

void setup_gpio_init(void)
{
    gpio_config_t gpio_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_BLK_NUM,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&gpio_cfg));

        gpio_config_t rst_gpio_cfg = {
            .pull_up_en = GPIO_PULLUP_DISABLE,          //禁止上拉
            .pull_down_en = GPIO_PULLDOWN_DISABLE,      //禁止下拉
            .mode = GPIO_MODE_OUTPUT,                   //输出模式
            .intr_type = GPIO_INTR_DISABLE,             //禁止中断
            .pin_bit_mask = (1ULL << LCD_RST_NUM)                //GPIO脚
        };
        ESP_ERROR_CHECK(gpio_config(&rst_gpio_cfg));

 
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL<<KEY_UP) | (1ULL<<KEY_DOWN) | 
                       (1ULL<<KEY_LEFT) | (1ULL<<KEY_RIGHT) |
                       (1ULL<<KEY_CENTER),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&cfg);
}