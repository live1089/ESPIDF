#include "weather.h"
#include "setup_wifi.h"
#include "cJSON.h"
#include <string.h>
/* 
[开始任务]
  |
  v
等待WiFi连接 --超时--> [任务退出]
  |
  v
初始化HTTP客户端 --失败--> 重试(MAX_RETRY)
  |
  v
执行请求 --失败--> 记录错误
  |           |
  |           v
  |       清理资源
  |
  v
解析响应数据 --失败--> 记录解析错误
  |
  v
等待5分钟
  |
  +--循环--<
 */
static const char *TAG = "http_client";
static char response_data[4096];
static uint16_t response_len;

#define MAX_RETRY_COUNT 3
uint8_t retry_count = 0;
void parse_json_data(const char *json_data);
/* 客户端事件处理 */
esp_err_t http_client_event_handle(esp_http_client_event_t *evt)
{
    switch (evt -> event_id)
    {
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG,"connected to server");
        break;
    case HTTP_EVENT_ON_DATA:
        char *user_buffer = (char *)evt->user_data;
        int16_t new_len  = response_len + evt->data_len;

        /* 缓冲区溢出保护 */
        if (new_len >= sizeof(response_data))
        {
            ESP_LOGE(TAG,"Response buffer overflow (%d/%d bytes)",
                new_len,sizeof(response_data));
            /* 截断数据保持有效 */
            new_len= sizeof(response_data) - 1;
            evt->data_len = new_len - response_len;
        }

        if (evt->data_len > 0)
        {
            memcpy(user_buffer + response_len,evt->data,evt->data_len);
            response_len += evt->data_len;
            user_buffer[response_len] = '\0';
        }
        break;
        
        case HTTP_EVENT_ON_FINISH:
            // 确保解析完整数据
            if (response_len > 0) {
            parse_json_data(response_data);
            }
            ESP_LOGI(TAG, "Request finished");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from server");
            break;
    
    default:
        break;
    }
    return ESP_OK;
}






esp_err_t  http_client_init()
{
    // 重置缓冲区和长度计数器
    memset(response_data, 0, sizeof(response_data));
    response_len = 0;

    esp_http_client_config_t config = {
        .url = "http://restapi.amap.com/v3/weather/weatherInfo?city=511503&key=74ec1c78aa5f61d12127d87ee00976db",  // 目标网址
        .method = HTTP_METHOD_GET,      // 请求类型（GET/POST）
        .event_handler = http_client_event_handle,
        .user_data = response_data,
        .disable_auto_redirect = true,
        .buffer_size = 4096,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client)
    {
        ESP_LOGE(TAG,"Failed to initialize HTTP client");
        return ESP_FAIL;
    }
    
    // GET
    esp_err_t err = esp_http_client_perform(client);
    esp_err_t result = ESP_OK;
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
        result = err;
    }
    esp_http_client_cleanup(client);
    return result;
}



void parse_json_data(const char *json_data) 
{
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        const char *error_ptr = cJSON_GetErrorPtr();
        ESP_LOGE(TAG, "JSON解析错误: %s", error_ptr ? error_ptr : "未知错误");
        return;
    }

    // 验证API响应状态
    cJSON *status = cJSON_GetObjectItem(root, "status");
    if (!cJSON_IsString(status) || strcmp(status->valuestring, "1") != 0) {
        ESP_LOGE(TAG, "API返回错误状态: %s", 
                status ? status->valuestring : "无状态字段");
        cJSON_Delete(root);
        return;
    }

    // 解析lives数组
    cJSON *lives = cJSON_GetObjectItem(root, "lives");
    if (cJSON_IsArray(lives) && cJSON_GetArraySize(lives) > 0) {
        cJSON *first_live = cJSON_GetArrayItem(lives, 0);
        
        // 结构化解析
        struct {
            const char *field;
            const char *desc;
        } fields[] = {
            {"province", "省份"},
            {"city", "城市"},
            {"weather", "天气"},
            {"temperature", "温度"},
            {"winddirection", "风向"}
        };

        for (size_t i = 0; i < sizeof(fields)/sizeof(fields[0]); i++) {
            cJSON *item = cJSON_GetObjectItem(first_live, fields[i].field);
            if (cJSON_IsString(item)) {
                ESP_LOGI(TAG, "%s: %s", fields[i].desc, item->valuestring);
            }
        }
    } else {
        ESP_LOGW(TAG, "无有效天气数据");
    }

    cJSON_Delete(root);
}





void weather_task(void *pvParams) {
    ESP_LOGI(TAG, "等待Wi-Fi连接成功...");
    if (s_wifi_event_group == NULL) 
    {
    ESP_LOGE(TAG, "事件组未初始化");
    vTaskDelete(NULL);
    return;
    }
    // 等待Wi-Fi连接成功
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, NETWORK_READY_BIT, 
                                            pdFALSE, pdTRUE, portMAX_DELAY);

    if (bits & NETWORK_READY_BIT)
    {
        xEventGroupClearBits(s_wifi_event_group, NETWORK_READY_BIT);
    }
    
    while(1) {
        esp_err_t ret = http_client_init();
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "天气数据获取成功！");
            retry_count = 0;  // 重置重试计数
        } else {
            retry_count++;
            if (retry_count >= MAX_RETRY_COUNT) {
                ESP_LOGE(TAG, "达到最大重试次数，等待下一个更新周期");
                retry_count = 0;
                vTaskDelay(pdMS_TO_TICKS(300000));  // 等待5分钟
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

