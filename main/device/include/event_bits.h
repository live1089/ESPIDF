// event_bits.h
#pragma once
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/event_groups.h"

// 定义模块专属事件组
extern EventGroupHandle_t net_events;   // 网络模块事件（Wi-Fi、BLE）
// extern EventGroupHandle_t sensor_events;// 传感器模块事件（温湿度、加速度）


// 系统级事件
#define SYS_EVENT_UI_READY      (1 << 0)
#define SYS_EVENT_WIFI_READY     (1 << 0)

// 网络模块事件
#define WIFI_CONNECTED_BIT       (1 << 1) // 连接成功标志
#define WIFI_FAIL_BIT            (1 << 2) // 连接失败标志









