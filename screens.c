#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

static void event_handler_checked_cb_main_screen_rgb_on_off(lv_event_t *e) {
    lv_obj_t *ta = lv_event_get_target(e);
    if (lv_obj_has_state(ta, LV_STATE_CHECKED)) {
        action_rgb_turn_on(e);
    }
}

static void event_handler_unchecked_cb_main_screen_rgb_on_off(lv_event_t *e) {
    lv_obj_t *ta = lv_event_get_target(e);
    if (!lv_obj_has_state(ta, LV_STATE_CHECKED)) {
        action_rgb_turn_off(e);
    }
}

void create_screen_set_up() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.set_up = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Setup border
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.setup_border = obj;
            lv_obj_set_pos(obj, 13, 12);
            lv_obj_set_size(obj, 454, 296);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff747474), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // start up main
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.start_up_main = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // logo
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.logo = obj;
                    lv_obj_set_pos(obj, 190, 86);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_logo);
                }
                {
                    // start up continue
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.start_up_continue = obj;
                    lv_obj_set_pos(obj, 159, 198);
                    lv_obj_set_size(obj, 162, 47);
                    lv_obj_add_event_cb(obj, action_startup_to_wifi, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // comtinue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.comtinue = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "continue");
                        }
                    }
                }
                {
                    // Welcome
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.welcome = obj;
                    lv_obj_set_pos(obj, 128, 47);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Welcome");
                }
            }
        }
        {
            // first wifi
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.first_wifi = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Welcome_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.welcome_1 = obj;
                    lv_obj_set_pos(obj, 176, 47);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "WiFi");
                }
                {
                    // wifi dropdown
                    lv_obj_t *obj = lv_dropdown_create(parent_obj);
                    objects.wifi_dropdown = obj;
                    lv_obj_set_pos(obj, 165, 104);
                    lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
                    lv_dropdown_set_options(obj, "Don WiFi\ndon24\nDon VPN");
                    lv_dropdown_set_selected(obj, 0);
                }
                {
                    // Start up password
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.start_up_password = obj;
                    lv_obj_set_pos(obj, 165, 166);
                    lv_obj_set_size(obj, 150, 40);
                    lv_textarea_set_max_length(obj, 128);
                    lv_textarea_set_placeholder_text(obj, "Password");
                    lv_textarea_set_one_line(obj, false);
                    lv_textarea_set_password_mode(obj, true);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_MOMENTUM);
                }
                {
                    // wifi next button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.wifi_next_button = obj;
                    lv_obj_set_pos(obj, 174, 228);
                    lv_obj_set_size(obj, 133, 34);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_outline_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff787878), LV_PART_MAIN | LV_STATE_FOCUSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // Wifi next button text
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.wifi_next_button_text = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff575757), LV_PART_MAIN | LV_STATE_FOCUSED);
                            lv_label_set_text(obj, "Next");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_set_up();
}

void tick_screen_set_up() {
}

void create_screen_main_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Main tab
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_LEFT, 0);
            objects.main_tab = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Focus_timer
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Focus_timer");
                    objects.focus_timer = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // setting_timer
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.setting_timer = obj;
                            lv_obj_set_pos(obj, -16, -16);
                            lv_obj_set_size(obj, 480, 320);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // Stop Button_1
                                    lv_obj_t *obj = lv_btn_create(parent_obj);
                                    objects.stop_button_1 = obj;
                                    lv_obj_set_pos(obj, 208, 226);
                                    lv_obj_set_size(obj, 84, 31);
                                    lv_obj_add_event_cb(obj, action_timer_setting_done, LV_EVENT_PRESSED, (void *)0);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // Start button label
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.start_button_label = obj;
                                            lv_obj_set_pos(obj, -1, 1);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "START");
                                        }
                                    }
                                }
                                {
                                    // focus timer text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.focus_timer_text = obj;
                                    lv_obj_set_pos(obj, 64, 37);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Focus Timer");
                                }
                                {
                                    // minute
                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                    objects.minute = obj;
                                    lv_obj_set_pos(obj, 252, 110);
                                    lv_obj_set_size(obj, 40, 100);
                                    lv_roller_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59", LV_ROLLER_MODE_NORMAL);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff898787), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_opa(obj, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_20, LV_PART_SELECTED | LV_STATE_DEFAULT);
                                }
                                {
                                    // hour
                                    lv_obj_t *obj = lv_roller_create(parent_obj);
                                    objects.hour = obj;
                                    lv_obj_set_pos(obj, 120, 110);
                                    lv_obj_set_size(obj, 40, 100);
                                    lv_roller_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24", LV_ROLLER_MODE_NORMAL);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff6a6a6a), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_opa(obj, 0, LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_20, LV_PART_SELECTED | LV_STATE_DEFAULT);
                                }
                                {
                                    // Number blocker
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.number_blocker = obj;
                                    lv_obj_set_pos(obj, 117, 98);
                                    lv_obj_set_size(obj, 265, 48);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // timer_swithch
                                    lv_obj_t *obj = lv_switch_create(parent_obj);
                                    objects.timer_swithch = obj;
                                    lv_obj_set_pos(obj, 307, 109);
                                    lv_obj_set_size(obj, 53, 25);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_CHECKED);
                                    lv_obj_set_style_outline_width(obj, 3, LV_PART_MAIN | LV_STATE_CHECKED);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff44ff00), LV_PART_INDICATOR | LV_STATE_CHECKED);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_INDICATOR | LV_STATE_CHECKED);
                                    lv_obj_set_style_border_width(obj, 3, LV_PART_INDICATOR | LV_STATE_CHECKED);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff696969), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                }
                                {
                                    // time highlight
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.time_highlight = obj;
                                    lv_obj_set_pos(obj, 82, 146);
                                    lv_obj_set_size(obj, 316, 28);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff767272), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // hour label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.hour_label = obj;
                                    lv_obj_set_pos(obj, 154, 150);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 20);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, ":hour");
                                }
                                {
                                    // lock device text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.lock_device_text = obj;
                                    lv_obj_set_pos(obj, 160, 112);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Lock Device");
                                }
                                {
                                    // minute label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.minute_label = obj;
                                    lv_obj_set_pos(obj, 291, 150);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 20);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, ":minute");
                                }
                            }
                        }
                        {
                            // CountDown
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.count_down = obj;
                            lv_obj_set_pos(obj, -16, -16);
                            lv_obj_set_size(obj, 480, 320);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // Stop Button
                                    lv_obj_t *obj = lv_btn_create(parent_obj);
                                    objects.stop_button = obj;
                                    lv_obj_set_pos(obj, 198, 211);
                                    lv_obj_set_size(obj, 84, 31);
                                    lv_obj_add_event_cb(obj, action_stop_timer, LV_EVENT_PRESSED, (void *)0);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // STOP button text
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.stop_button_text = obj;
                                            lv_obj_set_pos(obj, 0, 1);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "STOP");
                                        }
                                    }
                                }
                                {
                                    // timer bar
                                    lv_obj_t *obj = lv_bar_create(parent_obj);
                                    objects.timer_bar = obj;
                                    lv_obj_set_pos(obj, 46, 151);
                                    lv_obj_set_size(obj, 389, 32);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff282323), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_width(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xffdedbdb), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff44ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff3bff67), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                    lv_obj_set_style_shadow_width(obj, 30, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                                }
                                {
                                    // focus timer time
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.focus_timer_time = obj;
                                    lv_obj_set_pos(obj, 45, 19);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_time_120_, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "00:00");
                                }
                            }
                        }
                    }
                }
                {
                    // Media control
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "media_control");
                    objects.media_control = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // Before
                            lv_obj_t *obj = lv_imgbtn_create(parent_obj);
                            objects.before = obj;
                            lv_obj_set_pos(obj, 43, 94);
                            lv_obj_set_size(obj, 100, 100);
                            lv_imgbtn_set_src(obj, LV_IMGBTN_STATE_RELEASED, NULL, &img_before, NULL);
                            lv_obj_add_event_cb(obj, action_before, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                            lv_obj_set_style_img_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                        }
                        {
                            // Next
                            lv_obj_t *obj = lv_imgbtn_create(parent_obj);
                            objects.next = obj;
                            lv_obj_set_pos(obj, 320, 94);
                            lv_obj_set_size(obj, 100, 100);
                            lv_imgbtn_set_src(obj, LV_IMGBTN_STATE_RELEASED, NULL, &img_skip, NULL);
                            lv_obj_add_event_cb(obj, action_skip, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                            lv_obj_set_style_img_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                        }
                        {
                            // Play
                            lv_obj_t *obj = lv_imgbtn_create(parent_obj);
                            objects.play = obj;
                            lv_obj_set_pos(obj, 180, 94);
                            lv_obj_set_size(obj, 100, 100);
                            lv_imgbtn_set_src(obj, LV_IMGBTN_STATE_RELEASED, NULL, &img_play_button, NULL);
                            lv_obj_add_event_cb(obj, action_play_media, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_img_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                        }
                        {
                            // lower
                            lv_obj_t *obj = lv_imgbtn_create(parent_obj);
                            objects.lower = obj;
                            lv_obj_set_pos(obj, 182, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, 50);
                            lv_imgbtn_set_src(obj, LV_IMGBTN_STATE_RELEASED, NULL, &img_volume_down, NULL);
                            lv_obj_add_event_cb(obj, action_volume_down, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_img_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                        }
                        {
                            // higher
                            lv_obj_t *obj = lv_imgbtn_create(parent_obj);
                            objects.higher = obj;
                            lv_obj_set_pos(obj, 230, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, 50);
                            lv_imgbtn_set_src(obj, LV_IMGBTN_STATE_RELEASED, NULL, &img_volume_up, NULL);
                            lv_obj_add_event_cb(obj, action_volume_up, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_img_opa(obj, 100, LV_PART_MAIN | LV_STATE_PRESSED);
                        }
                    }
                }
                {
                    // weather
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "weather");
                    objects.weather = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // weather forcast
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.weather_forcast = obj;
                            lv_obj_set_pos(obj, -16, -16);
                            lv_obj_set_size(obj, 480, 320);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // Day3
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.day3 = obj;
                                    lv_obj_set_pos(obj, 327, 115);
                                    lv_obj_set_size(obj, 128, 190);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_grad_stop(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1c1c1c), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff666666), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // temp3
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temp3 = obj;
                                            lv_obj_set_pos(obj, 13, 10);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "13°");
                                        }
                                        {
                                            // date3
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.date3 = obj;
                                            lv_obj_set_pos(obj, 1, 59);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_letter_space(obj, -1.5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "15/02/2026");
                                        }
                                    }
                                }
                                {
                                    // Day2
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.day2 = obj;
                                    lv_obj_set_pos(obj, 177, 115);
                                    lv_obj_set_size(obj, 128, 190);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_grad_stop(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1c1c1c), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff666666), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // temp2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temp2 = obj;
                                            lv_obj_set_pos(obj, 13, 10);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "12°");
                                        }
                                        {
                                            // date2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.date2 = obj;
                                            lv_obj_set_pos(obj, 0, 59);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_letter_space(obj, -1.5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "14/02/2026");
                                        }
                                    }
                                }
                                {
                                    // Day1
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.day1 = obj;
                                    lv_obj_set_pos(obj, 26, 115);
                                    lv_obj_set_size(obj, 128, 190);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_grad_stop(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1c1c1c), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff666666), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // temp1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temp1 = obj;
                                            lv_obj_set_pos(obj, 13, 10);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "16°");
                                        }
                                        {
                                            // date1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.date1 = obj;
                                            lv_obj_set_pos(obj, 1, 59);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_letter_space(obj, -1.5, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "13/02/2026");
                                        }
                                    }
                                }
                            }
                        }
                        {
                            // temp_tab2
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.temp_tab2 = obj;
                            lv_obj_set_pos(obj, 93, 81);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_weather_animation, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_100_no_mono, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "3.0°c");
                        }
                    }
                }
                {
                    // main clock
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "main_clock");
                    objects.main_clock = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // date icon
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.date_icon = obj;
                            lv_obj_set_pos(obj, 179, 214);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_date_icon);
                            lv_img_set_zoom(obj, 185);
                        }
                        {
                            // date
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.date = obj;
                            lv_obj_set_pos(obj, 206, 253);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_mono30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "02");
                        }
                        {
                            // temp
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.temp = obj;
                            lv_obj_set_pos(obj, 161, 5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_letter_space(obj, -8, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "3.2°C");
                        }
                        {
                            // Week_Date
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.week_date = obj;
                            lv_obj_set_pos(obj, 206, 229);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Fri");
                        }
                        {
                            // time
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.time = obj;
                            lv_obj_set_pos(obj, 45, 84);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_time_120_, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_letter_space(obj, -8, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "20:00");
                        }
                    }
                }
                {
                    // calender
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "date");
                    objects.calender = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // calender date
                            lv_obj_t *obj = lv_calendar_create(parent_obj);
                            objects.calender_date = obj;
                            lv_obj_set_pos(obj, 40, 24);
                            lv_obj_set_size(obj, 369, 240);
                            lv_calendar_header_arrow_create(obj);
                            lv_calendar_set_today_date(obj, 2026, 2, 13);
                            lv_calendar_set_showed_date(obj, 2026, 2);
                            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_letter_space(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // Pre date
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.pre_date = obj;
                            lv_obj_set_pos(obj, 42, 36);
                            lv_obj_set_size(obj, 53, 33);
                            lv_obj_add_event_cb(obj, action_date_pre, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff616161), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // button pre text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.button_pre_text = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Pre");
                                }
                            }
                        }
                        {
                            // Next date
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.next_date = obj;
                            lv_obj_set_pos(obj, 355, 36);
                            lv_obj_set_size(obj, 53, 33);
                            lv_obj_add_event_cb(obj, action_date_next, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff616161), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // button next text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.button_next_text = obj;
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Next");
                                }
                            }
                        }
                    }
                }
                {
                    // RGB_control
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "RGB_control");
                    objects.rgb_control = obj;
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // colour fixer
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.colour_fixer = obj;
                            lv_obj_set_pos(obj, 149, 39);
                            lv_obj_set_size(obj, 22, 30);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff009fff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // RGB colour wheel
                            lv_obj_t *obj = lv_colorwheel_create(parent_obj, true);
                            objects.rgb_colour_wheel = obj;
                            lv_obj_set_pos(obj, 99, 19);
                            lv_obj_set_size(obj, 250, 250);
                            lv_colorwheel_set_mode_fixed(obj, true);
                            lv_obj_add_event_cb(obj, action_update_rgb_value, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_event_cb(obj, action_unlock_tabview, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_add_event_cb(obj, action_lock_tabview, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_arc_width(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_left(obj, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_row(obj, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xff8a8a8a), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 3, LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // rgb on/off
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.rgb_on_off = obj;
                            lv_obj_set_pos(obj, 149, 69);
                            lv_obj_set_size(obj, 150, 150);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_add_event_cb(obj, event_handler_checked_cb_main_screen_rgb_on_off, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_event_cb(obj, event_handler_unchecked_cb_main_screen_rgb_on_off, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff616161), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 90, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xffded3d3), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2c2b2b), LV_PART_MAIN | LV_STATE_PRESSED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1d1c1c), LV_PART_MAIN | LV_STATE_CHECKED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // on/off symbol main
                                    lv_obj_t *obj = lv_arc_create(parent_obj);
                                    objects.on_off_symbol_main = obj;
                                    lv_obj_set_pos(obj, 19, 19);
                                    lv_obj_set_size(obj, 92, 92);
                                    lv_arc_set_value(obj, 0);
                                    lv_arc_set_rotation(obj, 180);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                                    lv_obj_add_state(obj, LV_STATE_CHECKED);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xff44ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_CHECKED);
                                }
                                {
                                    // on/off symbol pri
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.on_off_symbol_pri = obj;
                                    lv_obj_set_pos(obj, 58, 20);
                                    lv_obj_set_size(obj, 15, 55);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                                    lv_obj_add_state(obj, LV_STATE_CHECKED);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff44ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 90, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_CHECKED);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_main_screen();
}

void tick_screen_main_screen() {
}

void create_screen_home() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.home = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    {
        lv_obj_t *parent_obj = obj;
        {
            // homepage
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.homepage = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // AppA
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.app_a = obj;
            lv_obj_set_pos(obj, 52, 18);
            lv_obj_set_size(obj, 100, 142);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // app A label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.app_a_label = obj;
                    lv_obj_set_pos(obj, 24, 100);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Main");
                }
                {
                    // app a button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.app_a_button = obj;
                    lv_obj_set_pos(obj, 15, 22);
                    lv_obj_set_size(obj, 70, 70);
                    lv_obj_add_event_cb(obj, action_app_main, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // A
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.a = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "A");
                        }
                    }
                }
            }
        }
        {
            // AppB
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.app_b = obj;
            lv_obj_set_pos(obj, 121, 160);
            lv_obj_set_size(obj, 100, 142);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // App B label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.app_b_label = obj;
                    lv_obj_set_pos(obj, 24, 100);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Temp");
                }
                {
                    // App B button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.app_b_button = obj;
                    lv_obj_set_pos(obj, 15, 22);
                    lv_obj_set_size(obj, 70, 70);
                    lv_obj_add_event_cb(obj, action_app_temp, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // B
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.b = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "B");
                        }
                    }
                }
            }
        }
        {
            // AppC
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.app_c = obj;
            lv_obj_set_pos(obj, 191, 18);
            lv_obj_set_size(obj, 100, 142);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // App c label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.app_c_label = obj;
                    lv_obj_set_pos(obj, 31, 100);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "RGB");
                }
                {
                    // App c button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.app_c_button = obj;
                    lv_obj_set_pos(obj, 15, 22);
                    lv_obj_set_size(obj, 70, 70);
                    lv_obj_add_event_cb(obj, action_app_rgb, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // C
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.c = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "C");
                        }
                    }
                }
            }
        }
        {
            // AppD
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.app_d = obj;
            lv_obj_set_pos(obj, 260, 160);
            lv_obj_set_size(obj, 100, 142);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // App D label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.app_d_label = obj;
                    lv_obj_set_pos(obj, 18, 100);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Media");
                }
                {
                    // App D button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.app_d_button = obj;
                    lv_obj_set_pos(obj, 16, 22);
                    lv_obj_set_size(obj, 70, 70);
                    lv_obj_add_event_cb(obj, action_app_media, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "D");
                        }
                    }
                }
            }
        }
        {
            // AppE
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.app_e = obj;
            lv_obj_set_pos(obj, 329, 18);
            lv_obj_set_size(obj, 100, 142);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // App e label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.app_e_label = obj;
                    lv_obj_set_pos(obj, 5, 100);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Setting");
                }
                {
                    // app e button
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.app_e_button = obj;
                    lv_obj_set_pos(obj, 15, 22);
                    lv_obj_set_size(obj, 70, 70);
                    lv_obj_add_event_cb(obj, action_app_setting, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // E
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.e = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "E");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_home();
}

void tick_screen_home() {
}

void create_screen_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // setting
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_LEFT, 87);
            objects.setting = obj;
            lv_obj_set_pos(obj, 0, 58);
            lv_obj_set_size(obj, 480, 262);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // setting side bar
                    lv_obj_t *obj = lv_tabview_get_tab_btns(parent_obj);
                    objects.setting_side_bar = obj;
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff212121), LV_PART_ANY | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff323639), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff000000), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_ITEMS | LV_STATE_CHECKED);
                }
                {
                    // Tab wifi
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "WiFi");
                    objects.tab_wifi = obj;
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // SSID
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ssid = obj;
                            lv_obj_set_pos(obj, 62, 73);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "SSID:");
                        }
                        {
                            // setting_text
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.setting_text = obj;
                            lv_obj_set_pos(obj, 108, 1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_mono50, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "WiFi");
                        }
                        {
                            // password
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.password = obj;
                            lv_obj_set_pos(obj, 10, 117);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "PASSWORD:");
                        }
                        {
                            // SSID text
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.ssid_text = obj;
                            lv_obj_set_pos(obj, 130, 63);
                            lv_obj_set_size(obj, 150, 38);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "SSID");
                            lv_textarea_set_one_line(obj, false);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_keyboard_visable, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff656565), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // password text
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.password_text = obj;
                            lv_obj_set_pos(obj, 130, 107);
                            lv_obj_set_size(obj, 150, 38);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "passcode");
                            lv_textarea_set_one_line(obj, false);
                            lv_textarea_set_password_mode(obj, true);
                            lv_obj_add_event_cb(obj, action_keyboard_visible_password, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff656565), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // if empty
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.if_empty = obj;
                            lv_obj_set_pos(obj, 49, 153);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff807f7f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "If open leave empty");
                        }
                    }
                }
                {
                    // other
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "other");
                    objects.other = obj;
                }
            }
        }
        {
            // save button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.save_button = obj;
            lv_obj_set_pos(obj, 327, 13);
            lv_obj_set_size(obj, 139, 37);
            lv_obj_add_event_cb(obj, action_save_credentials, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff686c6f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // save
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.save = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Save");
                }
            }
        }
        {
            // keyboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard = obj;
            lv_obj_set_pos(obj, 0, 187);
            lv_obj_set_size(obj, 480, 131);
            lv_obj_add_event_cb(obj, action_hide_keyboard, LV_EVENT_READY, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4a4a4a), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffa4a4a4), LV_PART_ITEMS | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffa4a4a4), LV_PART_ITEMS | LV_STATE_PRESSED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb9b9b9), LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        }
        {
            // setting label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setting_label = obj;
            lv_obj_set_pos(obj, 195, 22);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_mono20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "SETTING");
        }
    }
    
    tick_screen_settings();
}

void tick_screen_settings() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_set_up,
    tick_screen_main_screen,
    tick_screen_home,
    tick_screen_settings,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_set_up();
    create_screen_main_screen();
    create_screen_home();
    create_screen_settings();
}
