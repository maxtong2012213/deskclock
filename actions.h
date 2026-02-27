#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

//LVGL funtion
extern void action_keyboard_visable(lv_event_t * e);
extern void action_keyboard_visible_password(lv_event_t * e);
extern void action_save_credentials(lv_event_t * e);
extern void action_hide_keyboard(lv_event_t * e);
extern void scroll_driven_fade_action(lv_event_t * e);
extern void scroll_driven_scale_action(lv_event_t * e);
extern void action_weather_animation(lv_event_t * e);
extern void action_play_media(lv_event_t * e);
extern void action_before(lv_event_t * e);
extern void action_skip(lv_event_t * e);
extern void action_update_rgb_value(lv_event_t * e);
extern void action_lock_tabview(lv_event_t * e);
extern void action_unlock_tabview(lv_event_t * e);
extern void action_volume_up(lv_event_t * e);
extern void action_volume_down(lv_event_t * e);
extern void action_timer_setting_done(lv_event_t * e);
extern void action_stop_timer(lv_event_t * e);
extern void action_date_pre(lv_event_t * e);
extern void action_date_next(lv_event_t * e);
//app
extern void action_app_main(lv_event_t * e);
extern void action_app_temp(lv_event_t * e);
extern void action_app_rgb(lv_event_t * e);
extern void action_app_media(lv_event_t * e);
extern void action_app_setting(lv_event_t * e);
//RGB onof
extern void action_rgb_turn_on(lv_event_t * e);
extern void action_rgb_turn_off(lv_event_t * e);

//own funtion
extern void focus_timer_mode(int mode);
extern void timer_update_callback(lv_timer_t * timer);
extern void led_blink_callback(lv_timer_t * timer);
extern void start_alarm_blink(void);




#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/