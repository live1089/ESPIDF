#include "Digital_key.h"
#include "esp_log.h"



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








