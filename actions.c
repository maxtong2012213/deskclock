//this code is a base cover for any damage to the other files UwU
#include "actions.h"
#include "ui.h"
#include "persistence.h"
#include "media_control.h"
#include <stdio.h>

void update_led_strip_cpp(uint8_t r, uint8_t g, uint8_t b);

// Forward declarations for functions assumed to exist elsewhere
void save_wifi_credentials(const char *ssid, const char *password);
void __attribute__((weak)) ESP_restart() {}
void __attribute__((weak)) restart_device(lv_timer_t * timer) { ESP_restart(); }

// Dummy function to prevent compiler errors until you write the logic
void get_some_rest() {
    // Add lock screen logic here later
}

// --- Functions to Show the Keyboard ---

void action_keyboard_visable(lv_event_t * e) {
    lv_obj_t * keyboard = objects.keyboard;
    lv_obj_t * textarea = objects.ssid_text;

    if (keyboard && textarea) {
        lv_keyboard_set_textarea(keyboard, textarea);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

void action_keyboard_visible_password(lv_event_t * e) {
    lv_obj_t * keyboard = objects.keyboard;
    lv_obj_t * textarea = objects.password_text;

    if (keyboard && textarea) {
        lv_keyboard_set_textarea(keyboard, textarea);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

// --- Function to Hide the Keyboard ---

void action_hide_keyboard(lv_event_t * e) {
    lv_obj_t * keyboard = objects.keyboard;

    if (keyboard) {
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(keyboard, NULL);
    }
}

// --- Function to Save Data ---

void action_save_credentials(lv_event_t * e) {
    const char *ssid = lv_textarea_get_text(objects.ssid_text);
    const char *password = lv_textarea_get_text(objects.password_text);

    save_wifi_credentials(ssid, password);

    // --- HIDE ALL OTHER UI ELEMENTS ---
    lv_obj_add_flag(objects.ssid, LV_OBJ_FLAG_HIDDEN);  
    lv_obj_add_flag(objects.password, LV_OBJ_FLAG_HIDDEN);  
    lv_obj_add_flag(objects.ssid_text, LV_OBJ_FLAG_HIDDEN);  
    lv_obj_add_flag(objects.password_text, LV_OBJ_FLAG_HIDDEN);  
    lv_obj_add_flag(objects.if_empty, LV_OBJ_FLAG_HIDDEN);  
    lv_obj_add_flag(objects.save_button, LV_OBJ_FLAG_HIDDEN); 
    lv_obj_add_flag(objects.keyboard, LV_OBJ_FLAG_HIDDEN); 
 
    // --- UPDATE AND SHOW THE STATUS TEXT ---
    lv_label_set_text(objects.setting_text, "Saved! Restarting...");
    lv_obj_align(objects.setting_text, LV_ALIGN_CENTER, 0, 0); 

    // Call the wrapper function to restart the device after 1 second
    lv_timer_create(restart_device, 1000, NULL);
}

// --- Function to Fade Tabs on Scroll ---

void scroll_driven_fade_action(lv_event_t * e) {
    lv_obj_t * content_panel = lv_event_get_target(e);
    uint32_t tab_count = lv_obj_get_child_cnt(content_panel);

    lv_coord_t scroll_y = lv_obj_get_scroll_y(content_panel);
    lv_coord_t page_height = lv_obj_get_height(content_panel);

    if (page_height == 0) return;
    
    const lv_opa_t opa_max = LV_OPA_COVER;
    const lv_opa_t opa_min = LV_OPA_0;

    for (uint32_t i = 0; i < tab_count; i++) {
        lv_obj_t * tab = lv_obj_get_child(content_panel, i);
        
        lv_coord_t tab_y = i * page_height;
        lv_coord_t dist_from_scroll = tab_y - scroll_y;
        lv_coord_t abs_dist = LV_ABS(dist_from_scroll);

        if (abs_dist < page_height) {
            lv_opa_t opa_value = lv_map(abs_dist, 0, page_height, opa_max, opa_min);
            lv_obj_set_style_opa(tab, opa_value, LV_STATE_DEFAULT);
        } else {
            lv_obj_set_style_opa(tab, opa_min, LV_STATE_DEFAULT);
        }
    }
}

// --- Animations ---

static void set_zoom_anim_cb(void * var, int32_t v) {
    lv_obj_set_style_transform_zoom((lv_obj_t *)var, v, 0);
}

static void set_opa_anim_cb(void * var, int32_t v) {
    lv_obj_set_style_opa((lv_obj_t *)var, (lv_opa_t)v, 0);
}

static void hide_on_ready_cb(lv_anim_t * a) {
    lv_obj_add_flag((lv_obj_t *)a->var, LV_OBJ_FLAG_HIDDEN);
}

typedef struct {
    int is_triggered;        
    lv_coord_t original_y;   
} animation_state_t;

void action_weather_animation(lv_event_t * e) {
    lv_obj_t * target_label = lv_event_get_target(e); 
    lv_obj_t * forecast_cont = objects.weather_forcast;
    
    static animation_state_t state = {0, 0}; 
    const uint32_t ANIM_TIME = 500;

    lv_anim_del(target_label, NULL);
    lv_anim_del(forecast_cont, NULL);

    int32_t zoom_start, zoom_end, y_start, y_end, opa_start, opa_end;

    if (state.is_triggered == 0) { 
        if (state.original_y == 0) state.original_y = lv_obj_get_y(target_label);

        zoom_start = 256; zoom_end = 200;
        y_start = state.original_y; y_end = y_start - 60;
        opa_start = 0; opa_end = 255;

        lv_obj_clear_flag(forecast_cont, LV_OBJ_FLAG_HIDDEN);
        state.is_triggered = 1;
    } else { 
        zoom_start = 200; zoom_end = 256;
        y_start = lv_obj_get_y(target_label); y_end = state.original_y;
        opa_start = 255; opa_end = 0;

        state.is_triggered = 0;
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, target_label);
    lv_anim_set_time(&a, ANIM_TIME);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);

    lv_anim_set_values(&a, zoom_start, zoom_end);
    lv_anim_set_exec_cb(&a, set_zoom_anim_cb);
    lv_anim_start(&a);

    lv_anim_set_values(&a, y_start, y_end);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_start(&a);

    lv_anim_set_var(&a, forecast_cont);
    lv_anim_set_path_cb(&a, lv_anim_path_linear); 
    lv_anim_set_values(&a, opa_start, opa_end);
    lv_anim_set_exec_cb(&a, set_opa_anim_cb);
    
    if (state.is_triggered == 0) lv_anim_set_ready_cb(&a, hide_on_ready_cb);
    else lv_anim_set_ready_cb(&a, NULL);
    
    lv_anim_start(&a);
}

// --- Media control ---

void action_play_media(lv_event_t * e){
    media_send_play_pause();
    printf("Action: Play sent via Bluetooth\n");
}
  
void action_before(lv_event_t * e){
    media_send_prev(); 
    printf("Action: prev sent via Bluetooth\n");      
}

void action_skip(lv_event_t * e){
    media_send_next();
    printf("Action: skip sent via Bluetooth\n"); 
}

void action_volume_up(lv_event_t * e){
     media_send_vol_up();
     printf("Action: vol up");
}

void action_volume_down(lv_event_t * e){
     media_send_vol_down();
     printf("Action: vol down");
}

// --- LED Variables ---
static lv_timer_t * led_blink_timer = NULL;
static bool blink_state = false;
static int blink_count = 0; 

void led_blink_callback(lv_timer_t * timer) {
    blink_state = !blink_state; 

    if (blink_state) {
        update_led_strip_cpp(255, 0, 0); 
    } else {
        update_led_strip_cpp(0, 0, 0);   
        blink_count++;

        if (blink_count >= 5) {
            lv_timer_del(timer);     
            led_blink_timer = NULL;  
            blink_state = false;     
        }
    }
}

void start_alarm_blink(void) {
    if (led_blink_timer != NULL) return; 

    blink_count = 0;
    blink_state = false; 
    
    led_blink_timer = lv_timer_create(led_blink_callback, 500, NULL);
}

// --- RGB control ---
void action_update_rgb_value(lv_event_t*e){
    lv_obj_t * colorwheel = lv_event_get_target(e);
    lv_color_t c = lv_colorwheel_get_rgb(colorwheel);
    uint32_t color32 = lv_color_to32(c);

    uint8_t r = (color32 >> 16) & 0xFF;
    uint8_t g = (color32 >> 8) & 0xFF;
    uint8_t b = (color32) & 0xFF;

    update_led_strip_cpp(r, g, b);
}

void action_lock_tabview(lv_event_t * e){
    lv_obj_clear_flag(objects.main_tab, LV_OBJ_FLAG_SCROLLABLE);
    printf("tabview locked");
}

void action_unlock_tabview(lv_event_t * e){
    lv_obj_add_flag(objects.main_tab, LV_OBJ_FLAG_SCROLLABLE);
    printf("tabview unlocked");
}

void action_rgb_turn_on(lv_event_t * e){
    lv_obj_add_state(objects.on_off_symbol_main, LV_STATE_CHECKED);
    lv_obj_add_state(objects.on_off_symbol_pri, LV_STATE_CHECKED);

    update_led_strip_cpp(255, 0, 0);
}

void action_rgb_turn_off(lv_event_t * e){
    lv_obj_clear_state(objects.on_off_symbol_main, LV_STATE_CHECKED);
    lv_obj_clear_state(objects.on_off_symbol_pri, LV_STATE_CHECKED);

    update_led_strip_cpp(0, 0, 0);
}

// --- Focus timer ---
static int32_t total_seconds = 0;
static int32_t current_seconds = 0;
static lv_timer_t * countdown_timer = NULL;

void timer_update_callback(lv_timer_t * timer) {
    if (current_seconds > 0) {
        current_seconds--;

        int H = (current_seconds / 3600);
        int M = (current_seconds % 3600) / 60;
        int S = (current_seconds % 60);

        if (H > 0) {
            lv_label_set_text_fmt(objects.obj0, "%02d:%02d", H, M);
        } else {
            lv_label_set_text_fmt(objects.obj0, "%02d:%02d", M, S);
        }

        if (total_seconds > 0) {
            int32_t percent = 100 - (current_seconds * 100) / total_seconds;
            lv_bar_set_value(objects.timer_bar, percent, LV_ANIM_ON);
        }
    } else {
        action_stop_timer(NULL);

        if (lv_obj_has_state(objects.timer_swithch, LV_STATE_CHECKED)) {
            get_some_rest(); 
            printf("Timer finished: Device Locked.\n");
        }
    }
}

void focus_timer_mode(int mode) {
    if (mode == 1) {
        if(objects.setting_timer) lv_obj_clear_flag(objects.setting_timer, LV_OBJ_FLAG_HIDDEN);
        if(objects.count_down)    lv_obj_add_flag(objects.count_down, LV_OBJ_FLAG_HIDDEN);
        current_seconds = 0;
    } else {
        if(objects.setting_timer) lv_obj_add_flag(objects.setting_timer, LV_OBJ_FLAG_HIDDEN);
        if(objects.count_down)    lv_obj_clear_flag(objects.count_down, LV_OBJ_FLAG_HIDDEN);
    }
}

void action_timer_setting_done(lv_event_t * e){
     uint8_t selected_index_hour = (uint8_t)lv_roller_get_selected(objects.hour);
     uint8_t selected_index_minute = (uint8_t)lv_roller_get_selected(objects.minute);

     if ((selected_index_hour == 0) && (selected_index_minute == 0)){
        printf("Time can not be less then 0 sec!!! ");
        return;
     }

     focus_timer_mode(0);
     printf("Timer set for %d hours and %d minutes.\n", selected_index_hour, selected_index_minute);
     total_seconds = (selected_index_hour * 3600) + (selected_index_minute * 60);

     current_seconds = total_seconds;
    
     lv_bar_set_range(objects.timer_bar, 0, 100);
     lv_bar_set_value(objects.timer_bar, 0, LV_ANIM_OFF);
    
     lv_label_set_text_fmt(objects.obj0, "%02d:%02d", selected_index_hour, selected_index_minute);
     
     if (countdown_timer == NULL) {
        countdown_timer = lv_timer_create(timer_update_callback, 1000, NULL);
     } else {
        lv_timer_resume(countdown_timer);
     }
}

void action_stop_timer(lv_event_t * e){
    focus_timer_mode(1);

    if (countdown_timer != NULL) {
        if (current_seconds != 0) {
            lv_timer_pause(countdown_timer);
        } else {
            lv_timer_pause(countdown_timer);
            start_alarm_blink();
        }
    }
    printf("timer stoped");
}

// --- Home page navigations ---

void action_app_main(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) { 
        lv_scr_load_anim(objects.main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
        lv_tabview_set_act(objects.main_tab, 3, LV_ANIM_OFF); 
    }
}

void action_app_temp(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) { 
        lv_scr_load_anim(objects.main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
        lv_tabview_set_act(objects.main_tab, 2, LV_ANIM_OFF); 
    }
}

void action_app_rgb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) { 
        lv_scr_load_anim(objects.main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
        lv_tabview_set_act(objects.main_tab, 5, LV_ANIM_OFF); 
    }
}

void action_app_media(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) { 
        lv_scr_load_anim(objects.main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
        lv_tabview_set_act(objects.main_tab, 1, LV_ANIM_OFF); 
    }
}

void action_app_setting(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) { 
        lv_scr_load_anim(objects.settings, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    }
}

// --- Calendar Navigation Functions ---

void action_date_next(lv_event_t * e) {
    lv_obj_t * calendar = objects.calender_date;
    const lv_calendar_date_t * d = lv_calendar_get_showed_date(calendar);
    
    uint32_t year = d->year;
    uint32_t month = d->month;
    
    month++;
    if (month > 12) {
        month = 1;
        year++;
    }
    
    lv_calendar_set_showed_date(calendar, year, month);
}

void action_date_pre(lv_event_t * e) {
    lv_obj_t * calendar = objects.calender_date;
    const lv_calendar_date_t * d = lv_calendar_get_showed_date(calendar);
    
    uint32_t year = d->year;
    uint32_t month = d->month;
    
    if (month == 1) {
        month = 12;
        year--;
    } else {
        month--;
    }
    
    lv_calendar_set_showed_date(calendar, year, month);
}