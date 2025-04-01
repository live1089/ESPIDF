#include "lv_ui.h"
#include "esp_log.h"
#include "soft_drv_lvgl_port.h"

void button_group(void);
lv_obj_t *scr;
lv_obj_t *ui_test;
lv_obj_t *btn;



static const char *TAG = "lv_ui";
void lv_ui_init(void)
{
    
    lvgl_port_lock(0);
    lv_init();
    // 创建一个组
    group = lv_group_create();
    lv_indev_set_group(indev, group);
    lv_group_set_default(group);

    lv_group_t* indev_group = lv_indev_get_group(indev);
    if (indev_group == group) {
        ESP_LOGI(TAG, "Input device is associated with the group.");
    } else {
        ESP_LOGE(TAG, "Input device is NOT associated with the group!");
    }

    lv_indev_t* active_indev = lv_indev_active();
    if (active_indev) {
        ESP_LOGI(TAG, "Active input device: %p", active_indev);
    } else {
        ESP_LOGE(TAG, "No active input device!");
    }

    scr=lv_scr_act();

/*     ui_test = lv_obj_create(scr);
    lv_obj_set_size(ui_test, LV_HOR_RES, LV_VER_RES);
    btn = lv_button_create(ui_test);
    lv_obj_set_width(btn, 150);
    lv_obj_set_style_button_align(btn, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0); */

    button_group();
    lvgl_port_unlock();
} 


void bit1_event(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_KEY) {
        uint32_t key = lv_indev_get_key(lv_indev_active());
        switch (key) {
        case LV_KEY_ENTER:
            printf("LV_KEY_ENTER received on btn1\n");
            break;
        default:
            printf("Unknown key received: %lu\n", key);
            break;
        }
    } else {
        printf("Unknown event received: %d\n", code);
    }
}


lv_group_t* group = NULL; // 全局变量或在结构体中传递

void button_group(void) {
    // 创建按钮 1
    lv_obj_t* btn1 = lv_button_create(lv_scr_act());
    lv_obj_set_pos(btn1, 10, 10);
    lv_obj_set_size(btn1, 100, 50);
    lv_obj_add_event_cb(btn1, bit1_event, LV_EVENT_KEY, NULL);
    lv_group_add_obj(group, btn1);

    // 创建按钮 2
    lv_obj_t* btn2 = lv_button_create(lv_scr_act());
    lv_obj_set_pos(btn2, 10, 70);
    lv_obj_set_size(btn2, 100, 50);
    lv_obj_add_event_cb(btn2, bit1_event, LV_EVENT_KEY, NULL);
    lv_group_add_obj(group, btn2);

    // 创建按钮 3
    lv_obj_t* btn3 = lv_button_create(lv_scr_act());
    lv_obj_set_pos(btn3, 10, 130);
    lv_obj_set_size(btn3, 100, 50);
    lv_obj_add_event_cb(btn3, bit1_event, LV_EVENT_KEY, NULL);
    lv_group_add_obj(group, btn3);

    // 设置聚焦样式
    static lv_style_t style_focused;
    lv_style_init(&style_focused);
    lv_style_set_border_color(&style_focused, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_focused, 2);

    lv_obj_add_style(btn1, &style_focused, LV_STATE_FOCUSED | LV_STYLE_PROP_ANY);
    lv_obj_add_style(btn2, &style_focused, LV_STATE_FOCUSED | LV_STYLE_PROP_ANY);
    lv_obj_add_style(btn3, &style_focused, LV_STATE_FOCUSED | LV_STYLE_PROP_ANY);

    // 设置焦点到按钮 2
    lv_group_focus_obj(btn2);

    // 检查聚焦状态
    lv_obj_t* focused_obj = lv_group_get_focused(group);
    if (focused_obj) {
        ESP_LOGI(TAG, "Focused object: %p", focused_obj);
    } else {
        ESP_LOGE(TAG, "No focused object!");
    }
}


// void display_task(void *pvParams) {
//     while(1) {

//         vTaskDelay(pdMS_TO_TICKS(16)); // ~60Hz
//     }
// }