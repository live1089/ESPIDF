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

}

esp_err_t init_backlight_pwm() 
{
    const ledc_channel_config_t backlight_channel = {
        .gpio_num = LCD_BLK_NUM,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LCD_LEDC_CH,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = 1,
        .duty = 0,
        .hpoint = 0,
        .flags.output_invert = true
    };

    const ledc_timer_config_t backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = 1,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    
    ESP_ERROR_CHECK(ledc_timer_config(&backlight_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&backlight_channel));

    return ESP_OK;
}