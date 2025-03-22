# Copyright 2025 NXP
# NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import utime as time
import usys as sys
import lvgl as lv
import ustruct
import fs_driver

lv.init()

# Register display driver.
disp_drv = lv.sdl_window_create(320, 240)
lv.sdl_window_set_resizeable(disp_drv, False)
lv.sdl_window_set_title(disp_drv, "Simulator (MicroPython)")

# Regsiter input driver
mouse = lv.sdl_mouse_create()

# Add default theme for bottom layer
bottom_layer = lv.layer_bottom()
lv.theme_apply(bottom_layer)

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_scale(v)

def anim_img_rotate_cb(obj, v):
    obj.set_rotation(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.binfont_create(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.image_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = lv.calendar.__cast__(e.get_current_target())
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RESULT.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    datetext = lv.label.__cast__(e.get_target())
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = lv.screen_active()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = datetext.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, datetext), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    calendar = lv.calendar.__cast__(e.get_current_target())
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if calendar.get_pressed_date(date) == lv.RESULT.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.remove_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            calendar.delete()

# Create screen
screen = lv.obj()
screen.set_size(320, 240)
screen.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_btn_1
screen_btn_1 = lv.button(screen)
screen_btn_1_label = lv.label(screen_btn_1)
screen_btn_1_label.set_text("天气")
screen_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
screen_btn_1_label.set_width(lv.pct(100))
screen_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
screen_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_btn_1.set_pos(136, 152)
screen_btn_1.set_size(53, 35)
# Set style for screen_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_btn_2
screen_btn_2 = lv.button(screen)
screen_btn_2_label = lv.label(screen_btn_2)
screen_btn_2_label.set_text("设置")
screen_btn_2_label.set_long_mode(lv.label.LONG.WRAP)
screen_btn_2_label.set_width(lv.pct(100))
screen_btn_2_label.align(lv.ALIGN.CENTER, 0, 0)
screen_btn_2.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_btn_2.set_pos(38, 152)
screen_btn_2.set_size(53, 35)
# Set style for screen_btn_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_btn_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_bg_color(lv.color_hex(0xa600ff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_btn_3
screen_btn_3 = lv.button(screen)
screen_btn_3_label = lv.label(screen_btn_3)
screen_btn_3_label.set_text("传感器")
screen_btn_3_label.set_long_mode(lv.label.LONG.WRAP)
screen_btn_3_label.set_width(lv.pct(100))
screen_btn_3_label.align(lv.ALIGN.CENTER, 0, 0)
screen_btn_3.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_btn_3.set_pos(233, 152)
screen_btn_3.set_size(53, 35)
# Set style for screen_btn_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_btn_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_bg_color(lv.color_hex(0xff0027), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_1
screen_cont_1 = lv.obj(screen)
screen_cont_1.set_pos(0, -1)
screen_cont_1.set_size(320, 16)
screen_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_1
screen_label_1 = lv.label(screen_cont_1)
screen_label_1.set_text(""+lv.SYMBOL.WIFI+" ")
screen_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_label_1.set_width(lv.pct(100))
screen_label_1.set_pos(3, 2)
screen_label_1.set_size(21, 10)
# Set style for screen_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_font(test_font("HYCuHeiJ", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_label_2
screen_label_2 = lv.label(screen_cont_1)
screen_label_2.set_text(""+lv.SYMBOL.BATTERY_FULL+" ")
screen_label_2.set_long_mode(lv.label.LONG.WRAP)
screen_label_2.set_width(lv.pct(100))
screen_label_2.set_pos(289, -1)
screen_label_2.set_size(30, 15)
# Set style for screen_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_datetext_1
screen_datetext_1 = lv.label(screen)
screen_datetext_1.set_text("2024/04/22")
screen_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
screen_datetext_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_datetext_1_calendar = None
screen_datetext_1.add_event_cb(lambda e: datetext_event_handler(e, screen_datetext_1_calendar), lv.EVENT.ALL, None)
screen_datetext_1.set_pos(94, 43)
screen_datetext_1.set_size(130, 36)
# Set style for screen_datetext_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_datetext_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_datetext_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_digital_clock_1
screen_digital_clock_1_time = [int(11), int(25), int(50), "AM"]
screen_digital_clock_1 = lv.label(screen)
screen_digital_clock_1.set_text("11:25:50 AM")
screen_digital_clock_1_timer = lv.timer_create_basic()
screen_digital_clock_1_timer.set_period(1000)
screen_digital_clock_1_timer.set_cb(lambda src: digital_clock_cb(screen_digital_clock_1_timer, screen_digital_clock_1, screen_digital_clock_1_time, True, True ))
screen_digital_clock_1.set_pos(114, 79)
screen_digital_clock_1.set_size(96, 21)
# Set style for screen_digital_clock_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_digital_clock_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_text_font(test_font("HYCuHeiJ", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_digital_clock_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen.update_layout()
# Create screen_1
screen_1 = lv.obj()
screen_1.set_size(320, 240)
screen_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_tileview_1
screen_1_tileview_1 = lv.tileview(screen_1)
screen_1_tileview_1_tile = screen_1_tileview_1.add_tile(0, 0, lv.DIR.RIGHT)
screen_1_tileview_1_tile_1 = screen_1_tileview_1.add_tile(1, 0, lv.DIR.LEFT)
screen_1_tileview_1.set_pos(0, 0)
screen_1_tileview_1.set_size(320, 240)
screen_1_tileview_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_tileview_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_tileview_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_1_tileview_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
screen_1_tileview_1.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_color(lv.color_hex(0xeaeff3), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_radius(0, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create screen_1_spangroup_1
screen_1_spangroup_1 = lv.spangroup(screen_1_tileview_1_tile)
screen_1_spangroup_1.set_align(lv.TEXT_ALIGN.LEFT)
screen_1_spangroup_1.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_1_spangroup_1.set_mode(lv.SPAN_MODE.BREAK)
# create spans
screen_1_spangroup_1_span = screen_1_spangroup_1.new_span()
screen_1_spangroup_1_span.set_text("实时天气\n---------------------------------------\n城市/区：朝阳区\n\n天气：晴\n\n温度：18℃\n\n风向：西北\n\n风力：≤3 级\n\n空气湿度：15\n\n发布时间：2025-03-20 22:02:22")
screen_1_spangroup_1_span.style.set_text_color(lv.color_hex(0xffffff))
screen_1_spangroup_1_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
screen_1_spangroup_1_span.style.set_text_font(test_font("HYCuHeiJ", 12))
screen_1_spangroup_1.set_pos(38, 20)
screen_1_spangroup_1.set_size(247, 198)
screen_1_spangroup_1.refr_mode()
# Set style for screen_1_spangroup_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_spangroup_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_spangroup_2
screen_1_spangroup_2 = lv.spangroup(screen_1_tileview_1_tile_1)
screen_1_spangroup_2.set_align(lv.TEXT_ALIGN.LEFT)
screen_1_spangroup_2.set_overflow(lv.SPAN_OVERFLOW.CLIP)
screen_1_spangroup_2.set_mode(lv.SPAN_MODE.BREAK)
# create spans
screen_1_spangroup_2_span = screen_1_spangroup_2.new_span()
screen_1_spangroup_2_span.set_text("                  预报天气\n\n---------------------------------\n\n2025-03-20 晴 10~23℃\n\n\n2025-03-21 晴 13~26℃\n\n\n2025-03-22 晴 12~27℃\n\n\n2025-03-23 晴 8~25℃")
screen_1_spangroup_2_span.style.set_text_color(lv.color_hex(0xffffff))
screen_1_spangroup_2_span.style.set_text_decor(lv.TEXT_DECOR.NONE)
screen_1_spangroup_2_span.style.set_text_font(test_font("HYBaoSongJ", 12))
screen_1_spangroup_2.set_pos(27, 27)
screen_1_spangroup_2.set_size(282, 186)
screen_1_spangroup_2.refr_mode()
# Set style for screen_1_spangroup_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_spangroup_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_spangroup_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_1.update_layout()

def screen_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_1, lv.SCR_LOAD_ANIM.NONE, 200, 200, False)
screen_btn_1.add_event_cb(lambda e: screen_btn_1_event_handler(e), lv.EVENT.ALL, None)

def screen_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.KEY):
        pass
        lv.screen_load_anim(screen, lv.SCR_LOAD_ANIM.NONE, 200, 200, False)
screen_1.add_event_cb(lambda e: screen_1_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.screen_load(screen_1)

if __name__ == '__main__':
    while True:
        lv.task_handler()
        time.sleep_ms(5)
