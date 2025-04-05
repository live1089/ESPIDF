#ifndef WEATHER_H_
#define WEATHER_H_
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_err.h"

// 当日天气结构体（实时数据）
typedef struct {
    char province[20];          // 省份（如 "四川"）
    char city[20];              // 城市（如 "南溪区"）
    char reporttime[20];        // 报告时间（如 "2025-04-04 15:06:46"）
    char weather[20];           // 天气现象（如 "小雨"）
    char temperature[5];        // 温度（如 22）
    char winddirection[10];     // 风向（如 "北"）
    char windpower[8];          // 风力（如 "1-3"）
    char humidity[5];           // 湿度（如 60）
} RealTimeWeather;

// 天气预报结构体（单日）
typedef struct {
    char date[12];              // 日期（如 "2025-04-04"）
    char week[4];               // 星期（如 "5"）
    char dayweather[20];        // 日间天气（如 "小雨"）
    char daytemp[5];            // 日间温度（如 22）
    char daywind[10];           // 日间风向（如 "北"）
    char daypower[8];           // 日间风力（如 "1-3"）
} DailyForecast;

// 城市预报集合
typedef struct {
    char city[20];              // 城市名
    char reporttime[20];        // 报告时间
    DailyForecast casts[4]; // 预报数组（假设固定4天）
} CityWeather;



void weather_task(void *pvParams);






#endif