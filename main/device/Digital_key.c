#include "Digital_key.h"
#include "setup_gpio.h"



static const char *TAG = "GPIO BUTTON";
#define BUTTON_IO_NUM  0        
#define BUTTON_ACTIVE_LEVEL   0


static void button_event_cb(void *arg, void *data)
{
    button_event_t event = iot_button_get_event(arg);
    uint32_t ticks = iot_button_get_ticks_time(arg);
    switch (event)
    {
    case BUTTON_PRESS_DOWN:
        
        break;
    case BUTTON_PRESS_UP:
    
    break;
    case BUTTON_PRESS_REPEAT:
    
        break;
    case BUTTON_PRESS_REPEAT_DONE:
    
        break;
    case BUTTON_SINGLE_CLICK:
    
        break;
    case BUTTON_PRESS_END:
    
        break;

    default:
        break;
    }
    ESP_LOGI(TAG, "Button %d event: %s", (int)arg, iot_button_get_event_str(event));
}

void gpio_key_init()
{
    button_handle_t btn_up = NULL;
    button_handle_t btn_down = NULL;
    button_handle_t btn_left = NULL;
    button_handle_t btn_right = NULL;
    button_handle_t btn_center = NULL;

    button_config_t btn_cfg = {
        .long_press_time = 500,
        .short_press_time = 200,
    };

    button_gpio_config_t gpio_up_cfg = {
        .gpio_num = BUTTON_UP_IO_NUM,
        .active_level = BUTTON_ACTIVE_LEVEL,
        .enable_power_save = true,
        .disable_pull = false,
    }; 

    button_gpio_config_t gpio_down_cfg = {
        .gpio_num = BUTTON_DOWN_IO_NUM,
        .active_level = BUTTON_ACTIVE_LEVEL,
        .enable_power_save = true,
        .disable_pull = false,
    };

    button_gpio_config_t gpio_left_cfg = {
        .gpio_num = BUTTON_LEFT_IO_NUM,
        .active_level = BUTTON_ACTIVE_LEVEL,
        .enable_power_save = true,
        .disable_pull = false,
    };

    button_gpio_config_t gpio_right_cfg = {
        .gpio_num = BUTTON_RIGHT_IO_NUM,
        .active_level = BUTTON_ACTIVE_LEVEL,
        .enable_power_save = true,
        .disable_pull = false,
    };

    button_gpio_config_t gpio_center_cfg = {
        .gpio_num = BUTTON_CENTER_IO_NUM,
        .active_level = BUTTON_ACTIVE_LEVEL,
        .enable_power_save = false,
        .disable_pull = false,
    };


esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &gpio_up_cfg, &btn_up);
    ret |= iot_button_new_gpio_device(&btn_cfg, &gpio_down_cfg, &btn_down);
    ret |= iot_button_new_gpio_device(&btn_cfg, &gpio_left_cfg, &btn_left);
    ret |= iot_button_new_gpio_device(&btn_cfg, &gpio_right_cfg, &btn_right);
    ret |= iot_button_new_gpio_device(&btn_cfg, &gpio_center_cfg, &btn_center);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create button devices");
        return;
    }

    iot_button_register_cb(btn_up, BUTTON_PRESS_DOWN, NULL, button_event_cb, (void *)BUTTON_UP_IO_NUM);
    iot_button_register_cb(btn_down, BUTTON_PRESS_DOWN, NULL, button_event_cb, (void *)BUTTON_DOWN_IO_NUM);
    iot_button_register_cb(btn_left, BUTTON_PRESS_DOWN, NULL, button_event_cb, (void *)BUTTON_LEFT_IO_NUM);
    iot_button_register_cb(btn_right, BUTTON_PRESS_DOWN, NULL, button_event_cb, (void *)BUTTON_RIGHT_IO_NUM);
    iot_button_register_cb(btn_center, BUTTON_PRESS_DOWN, NULL, button_event_cb, (void *)BUTTON_CENTER_IO_NUM);


}



void key_task()
{
    gpio_key_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}




































#if 0
direction_t debounce_detect() {
    static struct {
        uint8_t state : 2;   // 0=释放, 1=按下中, 2=已确认
        uint32_t timestamp;
    } db[6] = {0};
    
    const TickType_t debounce_ticks = pdMS_TO_TICKS(30); // 防抖时间加至30ms
    direction_t detected = DIR_NONE;
    
    const gpio_num_t pins[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_CENTER, KEY_SET};
    
    for(int i=0; i<6; i++) {
        bool current = !gpio_get_level(pins[i]);
        
        switch(db[i].state) {
        case 0: // 初始状态
            if(current) {
                db[i].state = 1;
                db[i].timestamp = xTaskGetTickCount();
            }
            break;
            
        case 1: // 按下待确认
            if(xTaskGetTickCount() - db[i].timestamp > debounce_ticks) {
                if(current) {
                    detected = (direction_t)(i+1);
                    db[i].state = 2; // 进入保持状态
                } else {
                    db[i].state = 0; // 抖动，复位
                }
            }
            break;
            
        case 2: // 已确认按下
            if(!current) {
                db[i].state = 0; // 释放复位
            }
            break;
        }
    }
    
    return detected;
}


void button_task(void)
{
    
        direction_t dir = debounce_detect();
        if(dir != DIR_NONE) {
            printf("Pressed: %s\n", 
                dir == DIR_UP ? "UP" :
                dir == DIR_DOWN ? "DOWN" :
                dir == DIR_LEFT ? "LEFT" :
                dir == DIR_RIGHT ? "RIGHT" : 
                dir == DIR_CENTER ? "CENTER" : "SET");
        }
        vTaskDelay(pdMS_TO_TICKS(10)); 
    
}

#endif






