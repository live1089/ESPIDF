#ifndef SOFT_DRV_LVGL_PORT_H_
#define SOFT_DRV_LVGL_PORT_H_


#include "esp_lvgl_port.h"

extern  lv_indev_t *indev;

void soft_drv_lvgl_port_init(void);
void lvgl_event_task(void *param);

#endif
