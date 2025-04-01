#ifndef SETUP_WIFI_H_
#define SETUP_WIFI_H_
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/event_groups.h"




#define  wifi_ssid          "iQOO Neo9"
#define  wifi_password      "53k8w7x73zxug9w"

extern EventGroupHandle_t s_wifi_event_group;

#define NETWORK_READY_BIT BIT3


void wifi_task(void *pvParams);
bool wifi_connected();


#endif