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

# Create screen_0
screen_0 = lv.obj()
screen_0.set_size(320, 240)
screen_0.set_scrollbar_mode(lv.SCROLLBAR_MODE.ACTIVE)
screen_0.add_flag(lv.obj.FLAG.SCROLLABLE)
# Set style for screen_0, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_0.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_0_cont_1
screen_0_cont_1 = lv.obj(screen_0)
screen_0_cont_1.set_pos(0, 0)
screen_0_cont_1.set_size(320, 245)
screen_0_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_0_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_0_cont_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_border_color(lv.color_hex(0xc3c3c3), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_bg_image_src(load_image(r"D:\NXP\GUI-Guider-Projects\weather\generated\MicroPython\GZYVh3tbgAACvWI_320_245.png"), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_bg_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_bg_image_recolor_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_0_btn_2
screen_0_btn_2 = lv.button(screen_0_cont_1)
screen_0_btn_2_label = lv.label(screen_0_btn_2)
screen_0_btn_2_label.set_text("")
screen_0_btn_2_label.set_long_mode(lv.label.LONG.WRAP)
screen_0_btn_2_label.set_width(lv.pct(100))
screen_0_btn_2_label.align(lv.ALIGN.CENTER, 0, 0)
screen_0_btn_2.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_0_btn_2.set_pos(106, 106)
screen_0_btn_2.set_size(44, 31)
# Set style for screen_0_btn_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_0_btn_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_shadow_width(4, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_shadow_color(lv.color_hex(0xe7e7e7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_bg_image_src(load_image(r"D:\NXP\GUI-Guider-Projects\weather\generated\MicroPython\th_44_31.png"), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_bg_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_bg_image_recolor_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_0_digital_clock_1
screen_0_digital_clock_1_time = [int(8), int(25), int(24), "PM"]
screen_0_digital_clock_1 = lv.label(screen_0_cont_1)
screen_0_digital_clock_1.set_text("8:25:24 PM")
screen_0_digital_clock_1_timer = lv.timer_create_basic()
screen_0_digital_clock_1_timer.set_period(1000)
screen_0_digital_clock_1_timer.set_cb(lambda src: digital_clock_cb(screen_0_digital_clock_1_timer, screen_0_digital_clock_1, screen_0_digital_clock_1_time, True, True ))
screen_0_digital_clock_1.set_pos(111, 59)
screen_0_digital_clock_1.set_size(103, 28)
# Set style for screen_0_digital_clock_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_0_digital_clock_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_bg_opa(172, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_bg_color(lv.color_hex(0x738798), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_digital_clock_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_0_btn_1
screen_0_btn_1 = lv.button(screen_0_cont_1)
screen_0_btn_1_label = lv.label(screen_0_btn_1)
screen_0_btn_1_label.set_text("")
screen_0_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
screen_0_btn_1_label.set_width(lv.pct(100))
screen_0_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
screen_0_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_0_btn_1.set_pos(28, 106)
screen_0_btn_1.set_size(44, 31)
# Set style for screen_0_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_0_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_bg_color(lv.color_hex(0x88a0b4), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_shadow_width(4, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_shadow_color(lv.color_hex(0xe7e7e7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_bg_image_src(load_image(r"D:\NXP\GUI-Guider-Projects\weather\generated\MicroPython\xz_5x1_44_31.png"), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_bg_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_bg_image_recolor_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_text_font(test_font("HYBaoSongJ", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_0_datetext_1
screen_0_datetext_1 = lv.label(screen_0)
screen_0_datetext_1.set_text("2024/04/22")
screen_0_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
screen_0_datetext_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_0_datetext_1_calendar = None
screen_0_datetext_1.add_event_cb(lambda e: datetext_event_handler(e, screen_0_datetext_1_calendar), lv.EVENT.ALL, None)
screen_0_datetext_1.set_pos(36, 14)
screen_0_datetext_1.set_size(257, 36)
# Set style for screen_0_datetext_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_0_datetext_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_bg_opa(188, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_bg_color(lv.color_hex(0x979ca1), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_0_datetext_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_0.update_layout()
# Create screen_1
screen_1 = lv.obj()
screen_1.set_size(320, 240)
screen_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_cont_9
screen_1_cont_9 = lv.obj(screen_1)
screen_1_cont_9.set_pos(-1, 70)
screen_1_cont_9.set_size(325, 254)
screen_1_cont_9.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_cont_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_cont_9.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_1_tileview_1
screen_1_tileview_1 = lv.tileview(screen_1_cont_9)
screen_1_tileview_1_tile = screen_1_tileview_1.add_tile(0, 0, lv.DIR.RIGHT)
screen_1_tileview_1.set_pos(2, -3)
screen_1_tileview_1.set_size(317, 246)
screen_1_tileview_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_tileview_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_tileview_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_1_tileview_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
screen_1_tileview_1.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_color(lv.color_hex(0xeaeff3), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_1_tileview_1.set_style_radius(0, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create screen_1_cont_5
screen_1_cont_5 = lv.obj(screen_1_tileview_1_tile)
screen_1_cont_5.set_pos(10, 6)
screen_1_cont_5.set_size(300, 61)
screen_1_cont_5.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_cont_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_cont_5.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_border_color(lv.color_hex(0xf7f7f7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_shadow_width(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_shadow_color(lv.color_hex(0xcfcfcf), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_5.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_1_label_4
screen_1_label_4 = lv.label(screen_1_cont_5)
screen_1_label_4.set_text("雾")
screen_1_label_4.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_4.set_width(lv.pct(100))
screen_1_label_4.set_pos(110, 22)
screen_1_label_4.set_size(76, 21)
# Set style for screen_1_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_text_font(test_font("HYBaoSongJ", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_label_3
screen_1_label_3 = lv.label(screen_1_cont_5)
screen_1_label_3.set_text("13℃")
screen_1_label_3.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_3.set_width(lv.pct(100))
screen_1_label_3.set_pos(9, 7)
screen_1_label_3.set_size(100, 43)
# Set style for screen_1_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_text_color(lv.color_hex(0x5981d4), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_text_font(test_font("HYCuHeiJ", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_label_12
screen_1_label_12 = lv.label(screen_1_cont_5)
screen_1_label_12.set_text("星期五")
screen_1_label_12.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_12.set_width(lv.pct(100))
screen_1_label_12.set_pos(191, 22)
screen_1_label_12.set_size(100, 15)
# Set style for screen_1_label_12, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_12.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_text_font(test_font("HYBaoSongJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_cont_4
screen_1_cont_4 = lv.obj(screen_1_tileview_1_tile)
screen_1_cont_4.set_pos(10, 77)
screen_1_cont_4.set_size(300, 26)
screen_1_cont_4.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_cont_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_cont_4.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_border_color(lv.color_hex(0xf7f7f7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_shadow_width(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_shadow_color(lv.color_hex(0xcfcfcf), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_4.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_1_label_5
screen_1_label_5 = lv.label(screen_1_cont_4)
screen_1_label_5.set_text("湿度")
screen_1_label_5.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_5.set_width(lv.pct(100))
screen_1_label_5.set_pos(3, 0)
screen_1_label_5.set_size(58, 21)
# Set style for screen_1_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_text_font(test_font("HYZiKuTangHuiDianKaiShuW", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_label_10
screen_1_label_10 = lv.label(screen_1_cont_4)
screen_1_label_10.set_text("77%  /  60%")
screen_1_label_10.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_10.set_width(lv.pct(100))
screen_1_label_10.set_pos(183, 0)
screen_1_label_10.set_size(109, 21)
# Set style for screen_1_label_10, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_10.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_text_font(test_font("HYZiKuTangHuiDianKaiShuW", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_cont_3
screen_1_cont_3 = lv.obj(screen_1_tileview_1_tile)
screen_1_cont_3.set_pos(10, 119)
screen_1_cont_3.set_size(300, 26)
screen_1_cont_3.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_cont_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_cont_3.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_border_color(lv.color_hex(0xf7f7f7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_shadow_width(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_shadow_color(lv.color_hex(0xcfcfcf), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_3.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_1_label_9
screen_1_label_9 = lv.label(screen_1_cont_3)
screen_1_label_9.set_text("西南风  /  西北风")
screen_1_label_9.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_9.set_width(lv.pct(100))
screen_1_label_9.set_pos(145, 1)
screen_1_label_9.set_size(147, 21)
# Set style for screen_1_label_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_9.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_text_font(test_font("HYZiKuTangHuiDianKaiShuW", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_label_7
screen_1_label_7 = lv.label(screen_1_cont_3)
screen_1_label_7.set_text("风力")
screen_1_label_7.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_7.set_width(lv.pct(100))
screen_1_label_7.set_pos(3, 2)
screen_1_label_7.set_size(58, 21)
# Set style for screen_1_label_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_text_font(test_font("HYZiKuTangHuiDianKaiShuW", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_7.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_cont_2
screen_1_cont_2 = lv.obj(screen_1_tileview_1_tile)
screen_1_cont_2.set_pos(10, 160)
screen_1_cont_2.set_size(300, 26)
screen_1_cont_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_cont_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_cont_2.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_border_color(lv.color_hex(0xf7f7f7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_shadow_width(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_shadow_color(lv.color_hex(0xcfcfcf), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_2.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_1_label_6
screen_1_label_6 = lv.label(screen_1_cont_2)
screen_1_label_6.set_text("风向")
screen_1_label_6.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_6.set_width(lv.pct(100))
screen_1_label_6.set_pos(3, 2)
screen_1_label_6.set_size(58, 21)
# Set style for screen_1_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_text_font(test_font("HYZiKuTangHuiDianKaiShuW", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_label_8
screen_1_label_8 = lv.label(screen_1_cont_2)
screen_1_label_8.set_text("1~3级  /  1~3级")
screen_1_label_8.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_8.set_width(lv.pct(100))
screen_1_label_8.set_pos(158, 0)
screen_1_label_8.set_size(133, 21)
# Set style for screen_1_label_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_text_font(test_font("HYZiKuTangHuiDianKaiShuW", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_cont_6
screen_1_cont_6 = lv.obj(screen_1_tileview_1_tile)
screen_1_cont_6.set_pos(113, 214)
screen_1_cont_6.set_size(202, 29)
screen_1_cont_6.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_cont_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_cont_6.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_border_color(lv.color_hex(0xf7f7f7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_shadow_width(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_shadow_color(lv.color_hex(0xcfcfcf), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_6.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_1_label_11
screen_1_label_11 = lv.label(screen_1_cont_6)
screen_1_label_11.set_text("更新时间：2025-02-28 21:36:46")
screen_1_label_11.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_11.set_width(lv.pct(100))
screen_1_label_11.set_pos(16, 8)
screen_1_label_11.set_size(171, 11)
# Set style for screen_1_label_11, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_11.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_text_font(test_font("HYCuHeiJ", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_cont_1
screen_1_cont_1 = lv.obj(screen_1)
screen_1_cont_1.set_pos(0, 0)
screen_1_cont_1.set_size(321, 69)
screen_1_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_1_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_cont_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_border_color(lv.color_hex(0xf7f7f7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_bg_color(lv.color_hex(0xd9dee3), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_1_label_1
screen_1_label_1 = lv.label(screen_1_cont_1)
screen_1_label_1.set_text("南溪区")
screen_1_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_1.set_width(lv.pct(100))
screen_1_label_1.set_pos(8, 21)
screen_1_label_1.set_size(100, 34)
# Set style for screen_1_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_text_font(test_font("HYCuHeiJ", 27), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_label_2
screen_1_label_2 = lv.label(screen_1_cont_1)
screen_1_label_2.set_text("四川")
screen_1_label_2.set_long_mode(lv.label.LONG.WRAP)
screen_1_label_2.set_width(lv.pct(100))
screen_1_label_2.set_pos(109, 29)
screen_1_label_2.set_size(59, 18)
# Set style for screen_1_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_text_font(test_font("HYBaoSongJ", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_1_btn_1
screen_1_btn_1 = lv.button(screen_1)
screen_1_btn_1_label = lv.label(screen_1_btn_1)
screen_1_btn_1_label.set_text("X")
screen_1_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
screen_1_btn_1_label.set_width(lv.pct(100))
screen_1_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
screen_1_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_1_btn_1.set_pos(271, 14)
screen_1_btn_1.set_size(39, 22)
# Set style for screen_1_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_1_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_1_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_1.update_layout()
# Create screen_2
screen_2 = lv.obj()
screen_2.set_size(320, 240)
screen_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_2_cont_1
screen_2_cont_1 = lv.obj(screen_2)
screen_2_cont_1.set_pos(0, 0)
screen_2_cont_1.set_size(320, 240)
screen_2_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_2_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_2_cont_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_2_calendar_1
screen_2_calendar_1 = lv.calendar(screen_2_cont_1)
screen_2_calendar_1.set_today_date(time.localtime()[0], time.localtime()[1], time.localtime()[2])
screen_2_calendar_1.set_showed_date(time.localtime()[0], time.localtime()[1])
screen_2_calendar_1_highlighted_days=[
lv.calendar_date_t({'year':2025, 'month':3, 'day':7})
]
screen_2_calendar_1.set_highlighted_dates(screen_2_calendar_1_highlighted_days, len(screen_2_calendar_1_highlighted_days))
screen_2_calendar_1_header = lv.calendar_header_arrow(screen_2_calendar_1)
screen_2_calendar_1.add_event_cb(lambda e: calendar_event_handler(e,screen_2_calendar_1), lv.EVENT.ALL, None)
screen_2_calendar_1.set_pos(25, 34)
screen_2_calendar_1.set_size(265, 192)
# Set style for screen_2_calendar_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_2_calendar_1.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_border_color(lv.color_hex(0xc0c0c0), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_shadow_width(4, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_shadow_color(lv.color_hex(0xc9c9c9), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_calendar_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_2_calendar_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_2_calendar_1_extra_header_main_default = lv.style_t()
style_screen_2_calendar_1_extra_header_main_default.init()
style_screen_2_calendar_1_extra_header_main_default.set_text_color(lv.color_hex(0xffffff))
style_screen_2_calendar_1_extra_header_main_default.set_text_font(test_font("HYCuHeiJ", 12))
style_screen_2_calendar_1_extra_header_main_default.set_text_opa(255)
style_screen_2_calendar_1_extra_header_main_default.set_bg_opa(255)
style_screen_2_calendar_1_extra_header_main_default.set_bg_color(lv.color_hex(0x2195f6))
style_screen_2_calendar_1_extra_header_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_2_calendar_1_header.add_style(style_screen_2_calendar_1_extra_header_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_2_calendar_1, Part: lv.PART.ITEMS, State: lv.STATE.DEFAULT.
screen_2_calendar_1.get_btnmatrix().set_style_bg_opa(255, lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_bg_color(lv.color_hex(0xffffff), lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_border_width(1, lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_border_opa(255, lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_border_color(lv.color_hex(0xc0c0c0), lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_text_color(lv.color_hex(0x0D3055), lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_text_font(test_font("HYCuHeiJ", 12), lv.PART.ITEMS|lv.STATE.DEFAULT)
screen_2_calendar_1.get_btnmatrix().set_style_text_opa(255, lv.PART.ITEMS|lv.STATE.DEFAULT)

def screen_2_calendar_1_extra_ctrl_day_names_draw_event_cb(e):
    obj = lv.buttonmatrix.__cast__(e.get_target())
    dsc = lv.draw_task_t.__cast__(e.get_param())
    base_dsc = lv.draw_dsc_base_t.__cast__(dsc.draw_dsc)
    label_dsc = dsc.get_label_dsc()
    fill_dsc = dsc.get_fill_dsc()
    border_dsc = dsc.get_border_dsc()
    if base_dsc.id1 < 7:
        if label_dsc: label_dsc.color = lv.color_hex(0x0D3055)
        if label_dsc: label_dsc.font = test_font("HYCuHeiJ", 12)

screen_2_calendar_1.get_btnmatrix().add_event_cb(screen_2_calendar_1_extra_ctrl_day_names_draw_event_cb, lv.EVENT.DRAW_TASK_ADDED, None)

def screen_2_calendar_1_extra_ctrl_highlight_draw_event_cb(e):
    obj = lv.buttonmatrix.__cast__(e.get_target())
    dsc = lv.draw_task_t.__cast__(e.get_param())
    base_dsc = lv.draw_dsc_base_t.__cast__(dsc.draw_dsc)
    label_dsc = dsc.get_label_dsc()
    fill_dsc = dsc.get_fill_dsc()
    border_dsc = dsc.get_border_dsc()
    if base_dsc.id1 >= 7 and obj.has_button_ctrl(base_dsc.id1, lv.buttonmatrix.CTRL.CUSTOM_2):
        if label_dsc: label_dsc.color = lv.color_hex(0x0D3055)
        if label_dsc: label_dsc.font = test_font("HYCuHeiJ", 12)
        if fill_dsc: fill_dsc.opa = 255
        if fill_dsc: fill_dsc.color = lv.color_hex(0x2195f6)

screen_2_calendar_1.get_btnmatrix().add_event_cb(screen_2_calendar_1_extra_ctrl_highlight_draw_event_cb, lv.EVENT.DRAW_TASK_ADDED, None)

def screen_2_calendar_1_extra_ctrl_today_draw_event_cb(e):
    obj = lv.buttonmatrix.__cast__(e.get_target())
    dsc = lv.draw_task_t.__cast__(e.get_param())
    base_dsc = lv.draw_dsc_base_t.__cast__(dsc.draw_dsc)
    label_dsc = dsc.get_label_dsc()
    fill_dsc = dsc.get_fill_dsc()
    border_dsc = dsc.get_border_dsc()
    if base_dsc.id1 >= 7 and obj.has_button_ctrl(base_dsc.id1, lv.buttonmatrix.CTRL.CUSTOM_1):
        if label_dsc: label_dsc.color = lv.color_hex(0x0D3055)
        if label_dsc: label_dsc.font = test_font("HYCuHeiJ", 12)
        if fill_dsc: fill_dsc.opa = 255
        if fill_dsc: fill_dsc.color = lv.color_hex(0x01a2b1)
        if border_dsc: border_dsc.width = 1
        if border_dsc: border_dsc.color = lv.color_hex(0xc0c0c0)
        if border_dsc: border_dsc.opa = 255

screen_2_calendar_1.get_btnmatrix().add_event_cb(screen_2_calendar_1_extra_ctrl_today_draw_event_cb, lv.EVENT.DRAW_TASK_ADDED, None)

def screen_2_calendar_1_extra_ctrl_other_month_draw_event_cb(e):
    obj = lv.buttonmatrix.__cast__(e.get_target())
    dsc = lv.draw_task_t.__cast__(e.get_param())
    base_dsc = lv.draw_dsc_base_t.__cast__(dsc.draw_dsc)
    label_dsc = dsc.get_label_dsc()
    fill_dsc = dsc.get_fill_dsc()
    border_dsc = dsc.get_border_dsc()
    if base_dsc.id1 >= 7 and obj.has_button_ctrl(base_dsc.id1, lv.buttonmatrix.CTRL.DISABLED):
        if label_dsc: label_dsc.color = lv.color_hex(0xA9A2A2)
        if label_dsc: label_dsc.font = test_font("HYCuHeiJ", 12)
        if fill_dsc: fill_dsc.opa = 255
        if fill_dsc: fill_dsc.color = lv.color_hex(0xffffff)

screen_2_calendar_1.get_btnmatrix().add_event_cb(screen_2_calendar_1_extra_ctrl_other_month_draw_event_cb, lv.EVENT.DRAW_TASK_ADDED, None)

# Create screen_2_btn_1
screen_2_btn_1 = lv.button(screen_2_cont_1)
screen_2_btn_1_label = lv.label(screen_2_btn_1)
screen_2_btn_1_label.set_text("X")
screen_2_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
screen_2_btn_1_label.set_width(lv.pct(100))
screen_2_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
screen_2_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_2_btn_1.set_pos(251, 5)
screen_2_btn_1.set_size(47, 27)
# Set style for screen_2_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_2_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_text_font(test_font("HYCuHeiJ", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_2_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_2.update_layout()

def screen_0_btn_2_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_2, lv.SCR_LOAD_ANIM.NONE, 200, 200, False)
screen_0_btn_2.add_event_cb(lambda e: screen_0_btn_2_event_handler(e), lv.EVENT.ALL, None)

def screen_0_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_1, lv.SCR_LOAD_ANIM.NONE, 200, 200, False)
screen_0_btn_1.add_event_cb(lambda e: screen_0_btn_1_event_handler(e), lv.EVENT.ALL, None)

def screen_1_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_0, lv.SCR_LOAD_ANIM.NONE, 200, 200, False)
screen_1_btn_1.add_event_cb(lambda e: screen_1_btn_1_event_handler(e), lv.EVENT.ALL, None)

def screen_2_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_0, lv.SCR_LOAD_ANIM.NONE, 200, 200, False)
screen_2_btn_1.add_event_cb(lambda e: screen_2_btn_1_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.screen_load(screen_0)

if __name__ == '__main__':
    while True:
        lv.task_handler()
        time.sleep_ms(5)
