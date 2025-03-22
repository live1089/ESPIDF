#ifndef DIGITAL_KEY_H_
#define DIGITAL_KEY_H_
#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


#define KEY_UP     GPIO_NUM_27
#define KEY_DOWN   GPIO_NUM_26
#define KEY_LEFT   GPIO_NUM_25
#define KEY_RIGHT  GPIO_NUM_33
#define KEY_CENTER GPIO_NUM_32
#define KEY_SET    GPIO_NUM_35

typedef enum { DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_CENTER, DIR_SET } direction_t;
// void key_init(void);
void button_task(void);
direction_t debounce_detect();



#endif
