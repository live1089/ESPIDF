#include "weather.h"
#include "cJSON.h"

void http_client_init()
{
        esp_http_client_config_t config = {
        .url = "https://restapi.amap.com/v3/weather/weatherInfo?city=511503&key=74ec1c78aa5f61d12127d87ee00976db",  // 目标网址
        .method = HTTP_METHOD_GET      // 请求类型（GET/POST）
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    cJSON *json = cJSON_ParseWithLength(string, buffer_length);
}