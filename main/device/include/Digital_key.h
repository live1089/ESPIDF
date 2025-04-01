#ifndef DIGITAL_KEY_H_
#define DIGITAL_KEY_H_

#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <stdio.h>
#include <inttypes.h>
#include "esp_log.h"
#include "unity.h"
#include "iot_button.h"
#include "button_gpio.h"




// extern button_handle_t btn_up;
// extern button_handle_t btn_down;
extern button_handle_t btn_left;
extern button_handle_t btn_right;
extern button_handle_t btn_center;


// #define BUTTON_UP_IO_NUM  27
// #define BUTTON_DOWN_IO_NUM  26
#define BUTTON_LEFT_IO_NUM  25
#define BUTTON_RIGHT_IO_NUM  33
#define BUTTON_CENTER_IO_NUM  32

#define BUTTON_ACTIVE_LEVEL   0 //低电平有效



void gpio_key_init();
/* 
#define KEY_UP     GPIO_NUM_27
#define KEY_DOWN   GPIO_NUM_26
#define KEY_LEFT   GPIO_NUM_25
#define KEY_RIGHT  GPIO_NUM_33
#define KEY_CENTER GPIO_NUM_32
#define KEY_SET    GPIO_NUM_35 

typedef enum { DIR_NONE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_CENTER, DIR_SET } direction_t;
void key_init(void);
void button_task(void);
direction_t debounce_detect();
*/







#endif
