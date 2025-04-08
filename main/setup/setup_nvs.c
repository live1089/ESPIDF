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


