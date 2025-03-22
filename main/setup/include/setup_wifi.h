#ifndef SETUP_WIFI_H_
#define SETUP_WIFI_H_
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/event_groups.h"




#define  wifi_ssid          "iQOO Z3"
#define  wifi_password      "12ui678g"

void wifi_task(void *pvParams);



#endif