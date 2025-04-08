#include "setup_rtc.h"
#include "esp_pm.h"

static const char *TAG = "RTC";
RTC_DATA_ATTR static time_t rtc_time;

/* 系统运行时间 */
void sys_run_time()
{
    int64_t sec = esp_timer_get_time();
    time_t times = sec / 1000000;
    struct tm *time_info;
    time_info = localtime (&times);
    char formatted_time[30];
    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", time_info);
    ESP_LOGI(TAG,"系统运行时间：%s",formatted_time);
}

/* 获取当前系统时间并保存 */
void save_time_to_rtc()
{
    time(&rtc_time);
}

/* 深度睡眠/复位后恢复时间 */
void recover_time_from_rtc()
{
    struct timeval tv = {
        .tv_sec = rtc_time,
        .tv_usec = 0};
    settimeofday(&tv, NULL); // 恢复系统时间
}

/* 初始化SNTP服务获取时间 */
void sntp_init_get_time(void)
{
    ESP_LOGI(TAG,"进入SNTP初始化，获取时间");
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("cn.pool.ntp.org");
    esp_netif_sntp_init(&config);
    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK)
    {
        ESP_LOGE(TAG, "无法在 10 秒超时内更新系统时间。");
    }else{
        ESP_LOGI(TAG,"SNTP时间获取成功");
    }
}

bool sntp_connection = false;
/* 验证是否同步成功 */
bool sync_time_ntp()
{
    time_t now = 0;
    int retry = 0;
    const int max_retries = 15;

    while (retry < max_retries)
    {
        now = time(NULL); // 获取当前时间
        if (now > 1000000000)
        { // 检查时间是否已经设置
            ESP_LOGI(TAG, "时间已成功同步，当前时间戳: %u", (unsigned int)now);
            save_time_to_nvs(now); // 将时间保存到NVS
            esp_netif_sntp_deinit();
            sntp_connection = true;
            return true;
        }
        else
        {
            ESP_LOGI(TAG, "时间尚未设置，正在等待...");
            vTaskDelay(2000 / portTICK_PERIOD_MS); // 等待2秒
            sntp_init_get_time();
        }
        retry++;
    }
    ESP_LOGE(TAG, "时间同步失败，尝试了%d次", max_retries);
    return false;
}

/* 获取最后NTP更新时间 */
time_t get_last_update_time()
{
    time_t last_utc = load_time_from_nvs();
    return last_utc;
}

/* 获取上次系统关机时间 */
time_t get_last_system_su_time()
{
    time_t now;
    struct timeval tv = {
        .tv_usec = 0};
    gettimeofday(&tv, NULL);
    now = tv.tv_sec;
    struct tm *timeinfo = localtime(&now);
    char local_buffer[80];
    strftime(local_buffer, sizeof(local_buffer), "本地时间: %Y-%m-%d %H:%M:%S", timeinfo);

    return now;
}

/* 关闭系统，进入低功耗状态 */
void rtc_config_init()
{
    rtc_config_t cfg = RTC_CONFIG_DEFAULT();
    rtc_init(cfg);
    rtc_clk_config_t config = RTC_CLK_CONFIG_DEFAULT();
    rtc_clk_slow_src_set(SOC_RTC_SLOW_CLK_SRC_RC_SLOW);
    rtc_clk_init(config);

    esp_wifi_stop();
    // esp_bt_controller_disable();关闭蓝牙

    // 2. 配置GPIO4为RTC唤醒源
    const gpio_num_t wake_pin = GPIO_NUM_26;
    rtc_gpio_pullup_dis(wake_pin);
    rtc_gpio_pulldown_en(wake_pin);
    esp_sleep_enable_ext0_wakeup(wake_pin, 0); // 低电平唤醒

    rtc_sleep_config_t sleep_cfg;
    uint32_t pd_flags = RTC_SLEEP_PD_VDDSDIO |
                        RTC_SLEEP_PD_MODEM |
                        RTC_SLEEP_PD_XTAL |
                        RTC_SLEEP_PD_INT_8M;
    rtc_sleep_get_default_config(pd_flags, &sleep_cfg);
    sleep_cfg.rtc_fastmem_pd_en = 0; // 保持RTC_FAST内存供电
    sleep_cfg.rtc_slowmem_pd_en = 0; // 保持RTC_SLOW内存供电
    rtc_sleep_init(sleep_cfg);
    save_time_to_rtc();
    esp_deep_sleep_start();
}

/* 打印已配置的 NTP 服务器列表 */
static void print_servers(void)
{
    ESP_LOGI(TAG, "已配置的 NTP 服务器列表:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i)
    {
        if (esp_sntp_getservername(i))
        {
            ESP_LOGI(TAG, "server %d: %s", i, esp_sntp_getservername(i));
        }
        else
        {
            // we have either IPv4 or IPv6 address, let's print it
            char buff[INET6_ADDRSTRLEN];
            ip_addr_t const *ip = esp_sntp_getserver(i);
            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
                ESP_LOGI(TAG, "server %d: %s", i, buff);
        }
    }
}

/* 时间戳转换为本地时间 */
void convert_to_datetime(time_t timestamp, DateTime *dt)
{
    struct tm timeinfo;
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&timestamp, &timeinfo);
    dt->year = timeinfo.tm_year + 1900;
    dt->month = timeinfo.tm_mon + 1;
    dt->day = timeinfo.tm_mday;
    dt->hour = timeinfo.tm_hour;
    dt->minute = timeinfo.tm_min;
    dt->second = timeinfo.tm_sec;
}

// DateTime current_time;
// convert_to_datetime(time(NULL), &current_time);
DateTime current_time = {0};
/* 时间同步任务 */
void periodic_sync_task(void *pvParams)
{
    ESP_LOGI(TAG, "进入时间同步任务");
    if (net_events == NULL)
    {
        ESP_LOGE(TAG, "事件组未初始化");
        vTaskDelete(NULL);
        return;
    }
    // 等待Wi-Fi连接成功
    xEventGroupWaitBits(net_events, Notice_nvs,
                        pdFALSE, pdTRUE, portMAX_DELAY);

    sntp_init_get_time();
    if (sync_time_ntp())
    {
       
        convert_to_datetime(time(NULL), &current_time);
        ESP_LOGI(TAG,"当前时间：%d/%02d/%02d %d:%02d:%02d",current_time.year,current_time.month,
                                                        current_time.day,current_time.hour,
                                                        current_time.minute,current_time.second);
    }
    
    
    while (1)
    {
        vTaskDelay(3600 * 1000 / portTICK_PERIOD_MS); // 每小时同步一次
        sntp_connection = false;
        if (wifi_connected())
        {
            if (sync_time_ntp())
            {
                convert_to_datetime(time(NULL), &current_time);
                ESP_LOGI(TAG, " 每小时时间同步成功");
                
            }
        }
    }
}
