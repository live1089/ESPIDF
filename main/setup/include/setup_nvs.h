#ifndef SETUP_NVS_H
#define SETUP_NVS_H

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "time.h"
#include "nvs.h"

time_t load_time_from_nvs();
void save_time_to_nvs(time_t utc);
void nvs_init();

#endif