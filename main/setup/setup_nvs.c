#include "setup_nvs.h"
#include "esp_log.h"
void nvs_init()
{
    // 初始化 NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}


/* 存储时间 */
void save_time_to_nvs(time_t utc) {
    nvs_handle_t handle;
    ESP_ERROR_CHECK(nvs_open("time_store", NVS_READWRITE, &handle));
    ESP_ERROR_CHECK(nvs_set_i64(handle, "last_utc", utc));
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
    ESP_LOGI("NVS","时间已储存到nvs中");
}

/* 获取存储的时间 */
time_t load_time_from_nvs() {
    nvs_handle_t handle;
    time_t last_utc = 0;
    if (nvs_open("time_store", NVS_READONLY, &handle) == ESP_OK) {
        nvs_get_i64(handle, "last_utc", &last_utc);
        nvs_close(handle);
    }
    return last_utc;
}

void save_brightness(uint8_t data)
{
    nvs_handle_t handle;
    ESP_ERROR_CHECK(nvs_open("bright",NVS_READWRITE,&handle));
    ESP_ERROR_CHECK(nvs_set_u8(handle,"brightness",data));
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
    ESP_LOGI("BRIGHT","亮度值已储存到nvs中");
}

/* 保存WIFI配置 */
void save_wifi_config(const char *ssid, const char *password) {
    nvs_handle_t handle;
    nvs_open("wifi", NVS_READWRITE, &handle);
    nvs_set_str(handle, "ssid", ssid);
    nvs_set_str(handle, "password", password);
    nvs_commit(handle);
    nvs_close(handle);
    ESP_LOGI("WIFI","WIFI已储存到nvs中");
}

/* 读取WIFI配置 */
char get_wifi_config(){
    nvs_handle_t handle;
    char wifi_data[40];
    size_t size = sizeof(wifi_data);
    if (nvs_open("wifi",NVS_READONLY,&handle) == ESP_OK)
    {
        nvs_get_str(handle, "ssid", &wifi_data, size);
        nvs_get_str(handle, "password", &wifi_data, size);
        nvs_close(handle);
    }
    ESP_LOGI("WIFI","获取WIFI配置");
    return wifi_data;
}

uint8_t get_brightness()
{
    nvs_handle_t handle;
    uint8_t data;
    if (nvs_open("bright",NVS_READONLY,&handle) == ESP_OK)
    {
        nvs_get_u8(handle,"brightness",&data);
        nvs_close(handle);
    }
    ESP_LOGI("NVS","获取亮度值");
    return data;
}















