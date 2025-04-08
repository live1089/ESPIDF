#include "weather.h"
#include "setup_wifi.h"
#include "cJSON.h"
#include <string.h>
#include "Guider_ui/generated/events_init.h"

static const char *TAG = "http_client";
static char response_data[4096];
static uint16_t response_len;
static bool second;

#define MAX_RETRY_COUNT 3
uint8_t retry_count = 0;
void parse_lives(cJSON *lives);
void parse_forecasts(cJSON *forecasts,CityWeather *output);
void parse_response(const char *json_str);
/* 客户端事件处理 */
esp_err_t http_client_event_handle(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "connected to server");
        break;
    case HTTP_EVENT_ON_DATA:
        char *user_buffer = (char *)evt->user_data;
        int16_t new_len = response_len + evt->data_len;

        /* 缓冲区溢出保护 */
        if (new_len >= sizeof(response_data))
        {
            ESP_LOGE(TAG, "Response buffer overflow (%d/%d bytes)",
                     new_len, sizeof(response_data));
            /* 截断数据保持有效 */
            new_len = sizeof(response_data) - 1;
            evt->data_len = new_len - response_len;
        }

        if (evt->data_len > 0)
        {
            memcpy(user_buffer + response_len, evt->data, evt->data_len);
            response_len += evt->data_len;
            user_buffer[response_len] = '\0';
        }
        break;

    case HTTP_EVENT_ON_FINISH:
        parse_response(response_data);
        // ESP_LOGI(TAG, "原始JSON数据: %s", response_data); 
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Disconnected from server");
        break;

    default:
        break;
    }
    return ESP_OK;
}

esp_err_t http_client_init()
{
    // 重置缓冲区和长度计数器
    memset(response_data, 0, sizeof(response_data));
    response_len = 0;

    esp_http_client_config_t config = {
        .url = "http://restapi.amap.com/v3/weather/weatherInfo?city=511503&key=74ec1c78aa5f61d12127d87ee00976db&extensions=base",
        .method = HTTP_METHOD_GET, // 请求类型（GET/POST）
        .event_handler = http_client_event_handle,
        .user_data = response_data,
        .disable_auto_redirect = true,
        .buffer_size = 4096,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client)
    {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return ESP_FAIL;
    }

    // GET
    esp_err_t err = esp_http_client_perform(client);
    esp_err_t result = ESP_OK;
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
        result = err;
    }

    memset(response_data, 0, sizeof(response_data));
    response_len = 0;

    if (second)
    {
        // 设置第二个URL
        esp_http_client_set_url(client, "http://restapi.amap.com/v3/weather/weatherInfo?city=511503&key=74ec1c78aa5f61d12127d87ee00976db&extensions=all");

        // 执行第二次请求
        err = esp_http_client_perform(client);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "预报天气请求失败: %s", esp_err_to_name(err));
            result = err;
        }
    }
    esp_http_client_cleanup(client);
    return result;
}

void parse_response(const char *json_str)
{
    cJSON *root = cJSON_Parse(json_str);
    if (!root)
        return;
    // 检测 lives 字段（实时数据）
    cJSON *lives = cJSON_GetObjectItemCaseSensitive(root, "lives");
    if (lives && cJSON_IsArray(lives))
    {
        parse_lives(lives); // 解析实时天气
    }

    // 检测 forecasts 字段（预报数据）
    cJSON *forecasts = cJSON_GetObjectItemCaseSensitive(root, "forecasts");
    if (forecasts && cJSON_IsArray(forecasts))
    {
        CityWeather forecast = {0};
        parse_forecasts(forecasts,&forecast); // 解析预报
    }

    cJSON_Delete(root);
}
RealTimeWeather parsed_weather_data = {0};
void parse_lives(cJSON *lives)
{
    ESP_LOGI(TAG,"正在解析实时天气");
    // 确保解析完整数据
    if (response_len > 0)
    {
        // 解析lives数组
        cJSON *live;
        cJSON_ArrayForEach(live, lives)
        {
            cJSON *province = cJSON_GetObjectItemCaseSensitive(live, "province");
            if (cJSON_IsString(province))
            {
                strncpy(parsed_weather_data.province, province->valuestring, sizeof(parsed_weather_data.province) - 1);
                ESP_LOGI(TAG,"省份：%s",parsed_weather_data.province);
            }

            cJSON *city = cJSON_GetObjectItemCaseSensitive(live, "city");
            if (cJSON_IsString(city))
            {
                strncpy(parsed_weather_data.city, city->valuestring, sizeof(parsed_weather_data.city) - 1);
                ESP_LOGI(TAG,"城市：%s",parsed_weather_data.city);
            }

            cJSON *weather = cJSON_GetObjectItemCaseSensitive(live, "weather");
            if (cJSON_IsString(weather))
            {
                strncpy(parsed_weather_data.weather, weather->valuestring, sizeof(parsed_weather_data.weather) - 1);
                ESP_LOGI(TAG,"天气：%s",parsed_weather_data.weather);
            }

            cJSON *temperature = cJSON_GetObjectItemCaseSensitive(live, "temperature");
            if (cJSON_IsString(temperature))
            {
                strncpy(parsed_weather_data.temperature, temperature->valuestring, sizeof(parsed_weather_data.temperature) - 1);
                ESP_LOGI(TAG,"温度：%s",parsed_weather_data.temperature);
            }

            cJSON *winddirection = cJSON_GetObjectItemCaseSensitive(live, "winddirection");
            if (cJSON_IsString(winddirection))
            {
                strncpy(parsed_weather_data.winddirection, winddirection->valuestring, sizeof(parsed_weather_data.winddirection) - 1);
            }

            cJSON *windpower = cJSON_GetObjectItemCaseSensitive(live, "windpower");
            if (cJSON_IsString(windpower))
            {
                strncpy(parsed_weather_data.windpower, windpower->valuestring, sizeof(parsed_weather_data.windpower) - 1);
            }

            cJSON *humidity = cJSON_GetObjectItemCaseSensitive(live, "humidity");
            if (cJSON_IsString(humidity))
            {
                strncpy(parsed_weather_data.humidity, humidity->valuestring, sizeof(parsed_weather_data.humidity) - 1);
            }

            cJSON *reporttime = cJSON_GetObjectItemCaseSensitive(live, "reporttime");
            if (cJSON_IsString(reporttime))
            {
                strncpy(parsed_weather_data.reporttime, reporttime->valuestring, sizeof(parsed_weather_data.reporttime) - 1);
                ESP_LOGI(TAG,"更新时间：%s",parsed_weather_data.reporttime);
            }
        }
        second = true;
    }
}

CityWeather forecast_weather_data = {0};
void parse_forecasts(cJSON *forecasts,CityWeather *output)
{
    ESP_LOGI(TAG,"正在解析预报天气");
    cJSON *city_forecasts;
    int day_index = 0;
    cJSON_ArrayForEach(city_forecasts,forecasts)
    {
        cJSON *casts = cJSON_GetObjectItemCaseSensitive(city_forecasts,"casts");
        if (!cJSON_IsArray(casts)) continue;
        

        cJSON *cast;
        cJSON_ArrayForEach(cast,casts)
        {
            if (day_index >= 4) break;

            cJSON *date = cJSON_GetObjectItemCaseSensitive(cast,"date");
            if (cJSON_IsString(date))
            {
                strncpy(forecast_weather_data.casts[day_index].date,date->valuestring,sizeof(forecast_weather_data.casts[day_index].date) - 1);
                ESP_LOGI(TAG,"时间：%s",forecast_weather_data.casts[day_index].date);
            }

            cJSON *week = cJSON_GetObjectItemCaseSensitive(cast,"week");
            if (cJSON_IsString(week))
            {
                strncpy(forecast_weather_data.casts[day_index].week,week->valuestring,sizeof(forecast_weather_data.casts[day_index].week) - 1);
            }
            
            cJSON *dayweather = cJSON_GetObjectItemCaseSensitive(cast,"dayweather");
            if (cJSON_IsString(dayweather))
            {
                strncpy(forecast_weather_data.casts[day_index].dayweather,dayweather->valuestring,sizeof(forecast_weather_data.casts[day_index].dayweather) - 1);
                ESP_LOGI(TAG,"天气：%s",forecast_weather_data.casts[day_index].dayweather);
            }

            cJSON *daytemp = cJSON_GetObjectItemCaseSensitive(cast,"daytemp");
            if (cJSON_IsString(daytemp))
            {
                strncpy(forecast_weather_data.casts[day_index].daytemp,daytemp->valuestring,sizeof(forecast_weather_data.casts[day_index].daytemp) - 1);
                ESP_LOGI(TAG,"温度：%s",forecast_weather_data.casts[day_index].daytemp);
            }

            cJSON *daywind = cJSON_GetObjectItemCaseSensitive(cast,"daywind");
            if (cJSON_IsString(daywind))
            {
                strncpy(forecast_weather_data.casts[day_index].daywind,daywind->valuestring,sizeof(forecast_weather_data.casts[day_index].daywind) - 1);
            }

            cJSON *daypower = cJSON_GetObjectItemCaseSensitive(cast,"daypower");
            if (cJSON_IsString(daypower))
            {
                strncpy(forecast_weather_data.casts[day_index].daypower,daypower->valuestring,sizeof(forecast_weather_data.casts[day_index].daypower) - 1);
            }

            day_index++;

        }
    }
}

void weather_task(void *pvParams)
{
    ESP_LOGI(TAG, "等待Wi-Fi连接成功...");
    if (net_events == NULL)
    {
        ESP_LOGE(TAG, "事件组未初始化");
        vTaskDelete(NULL);
        return;
    }
    // 等待Wi-Fi连接成功
    xEventGroupWaitBits(net_events, SYS_EVENT_WIFI_READY,
                                           pdFALSE, pdTRUE, portMAX_DELAY);

    // if (bits & SYS_EVENT_WIFI_READY)
    // {
    //     xEventGroupClearBits(net_events, SYS_EVENT_WIFI_READY);
    // }
    esp_err_t ret = http_client_init();
    while (1)
    {
        if (ret == ESP_OK)
        {
            ESP_LOGI(TAG, "天气数据获取与解析成功");
            retry_count = 0; // 重置重试计数
        }
        else
        {
            retry_count++;
            if (retry_count >= MAX_RETRY_COUNT)
            {
                ESP_LOGE(TAG, "达到最大重试次数，等待下一个更新周期");
                retry_count = 0;
                vTaskDelay(pdMS_TO_TICKS(300000)); // 等待5分钟
                continue;
            }
            ESP_LOGW(TAG, "天气数据获取失败,5秒后重试(%d/%d)",
                     retry_count, MAX_RETRY_COUNT);
            vTaskDelay(pdMS_TO_TICKS(5000));
            continue;
        }
        vTaskDelay(pdMS_TO_TICKS(300000));
    }
}



