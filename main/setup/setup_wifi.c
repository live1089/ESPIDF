#include "setup_wifi.h"
#include <string.h>

static const char *TAG = "wifi";
// 定义事件组和连接状态标志位
EventGroupHandle_t s_wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0 // 连接成功标志
#define WIFI_FAIL_BIT BIT1      // 连接失败标志

#define WIFI_MAXIMUM_RETRY 5    // WIFI最大连接次数
uint8_t s_retry_num = 0;

#if 1
/* Wi-Fi 连接状态检查 */
bool wifi_connected()
{
    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK)
    {
        return false;
    }

    /* 检查是否获取到 IP 地址 */
    esp_netif_ip_info_t ip_info;
    if (esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info) != ESP_OK)
    {
        return false;
    }

    return ip_info.ip.addr != 0;
}
#endif

static void wifi_event_handle(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "WiFi station started");
            esp_wifi_connect(); // 自动触发连接
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
        {
            ESP_LOGI(TAG, "WiFi connection lost");
            wifi_event_sta_disconnected_t *disconnect =
                (wifi_event_sta_disconnected_t *)event_data;

            // 根据断开原因输出调试信息
            ESP_LOGE(TAG, "Disconnected reason: %d", disconnect->reason);
            // 根据错误类型处理
            if (disconnect->reason == WIFI_REASON_NO_AP_FOUND)
            {
                ESP_LOGE(TAG, "AP not found, check SSID");
            }
            else if (disconnect->reason == WIFI_REASON_AUTH_FAIL)
            {
                ESP_LOGE(TAG, "Authentication failed, check password");
            }

            if (s_retry_num < WIFI_MAXIMUM_RETRY)
            {
                // 指数退避策略：重试间隔逐步增加（2^retry_num秒）
                uint32_t delay_ms = (1 << s_retry_num) * 1000;
                ESP_LOGI(TAG, "Retrying in %d seconds...", (int)(delay_ms / 1000));
                vTaskDelay(pdMS_TO_TICKS(delay_ms));

                esp_wifi_connect();
                s_retry_num++;
            }
            else
            {
                xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
                ESP_LOGE(TAG, "Maximum retries reache");
                esp_restart(); // 达到最大重试后重启设备
            }
            break;
        }
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        // 设置连接成功标志
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/* Wi-Fi 初始化 */
esp_err_t wifi_init_sta(const char *ssid, const char *password)
{
    // 确保只初始化一次
    static bool initialized = false;
    if (initialized)
        return ESP_OK;
    initialized = true;

    /*初始化网络接口层(TCP/IP协议栈)*/
    ESP_ERROR_CHECK(esp_netif_init());

    /* 创建默认的station模式网络接口 */
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();
    if (!netif)
    {
        ESP_LOGE(TAG, "Failed to create default STA interface");
        return ESP_OK;
    }

    /* 创建默认事件循环,用于处理系统事件 */
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Wi-Fi 配置
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册事件处理器
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handle, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handle, NULL));

    // 设置 Wi-Fi 配置
    wifi_config_t wifi_config = {0};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;

    // 启动 Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    return ESP_OK;
}

void scan_wifi()
{
    wifi_scan_config_t wifi_scan_cfg = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = {
            .active = {.min = 100, .max = 150},
            .passive = 360
        }
    };

    esp_err_t err = esp_wifi_scan_start(&wifi_scan_cfg, true);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "WiFi scan start failed: %s", esp_err_to_name(err));
        return;
    }

    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);

    wifi_ap_record_t *ap_records = malloc(sizeof(wifi_ap_record_t) * ap_count);
    if (ap_records == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for AP records");
        return;
    }

    esp_wifi_scan_get_ap_records(&ap_count, ap_records);

    for (int i = 0; i < ap_count; i++)
    {
        ESP_LOGI(TAG, "SSID: %s, RSSI: %d", ap_records[i].ssid, ap_records[i].rssi);
    }

    free(ap_records);
    esp_wifi_clear_ap_list();
}

/*
 Wi-Fi 连接任务
• ​功能：维护网络连接，为天气更新提供支持。
• ​优先级：中等，保证断线自动重连。
• ​通信方式：使用 ​事件组（Event Group）​ 通知天气任务。
*/
void wifi_task(void *pvParams)
{
    const char *ssid = wifi_ssid;
    const char *password = wifi_password;

    /*创建WiFi事件组,用于同步WiFi连接状态*/
    s_wifi_event_group = xEventGroupCreate();

    // 初始化 NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 初始化 Wi-Fi
    wifi_init_sta(ssid, password);

    // 扫描WIFI
    scan_wifi();

    // 主循环处理连接状态
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                               WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                               pdFALSE, pdFALSE, portMAX_DELAY);

        // 连接成功
        if (bits & WIFI_CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "Connected to AP");

            // 触发其他任务（例如启动 HTTP 客户端）
            xEventGroupSetBits(s_wifi_event_group, NETWORK_READY_BIT);

            // 清除事件标志
            xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        }

        // 连接失败
        if (bits & WIFI_FAIL_BIT)
        {
            ESP_LOGW(TAG, "Wait for 5 seconds and try again...");
            vTaskDelay(5000 / portTICK_PERIOD_MS);  // 等待 5 秒
            esp_wifi_connect();
            xEventGroupClearBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // 防止 CPU 占用过高
    }

    // 任务退出前清理
    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handle);
    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handle);
    vEventGroupDelete(s_wifi_event_group);
    vTaskDelete(NULL);
}
