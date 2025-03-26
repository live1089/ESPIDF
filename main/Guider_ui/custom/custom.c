/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"

/*********************
 *      DEFINES
 *********************/
#include "Digital_key.h"
#include "Guider_ui/custom/custom.h"
#include "Guider_ui/generated/gui_guider.h"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */

void custom_handle_brightness(uint8_t value) {  
    // 1. 业务逻辑：设置亮度（对接硬件或模拟）  
    // 假设这里调用硬件接口：hardware_set_brightness(value);  
    // direction_t dir = debounce_detect();

    // 2. 界面更新（示例）  
    lv_label_set_text_fmt(guider_ui.screen_2_label_4, "%d", value);
    
    
}


void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    

}

