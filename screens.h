#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *set_up;
    lv_obj_t *main_screen;
    lv_obj_t *home;
    lv_obj_t *settings;
    lv_obj_t *rgb_on_off;
    lv_obj_t *setup_border;
    lv_obj_t *start_up_main;
    lv_obj_t *logo;
    lv_obj_t *start_up_continue;
    lv_obj_t *comtinue;
    lv_obj_t *welcome;
    lv_obj_t *first_wifi;
    lv_obj_t *welcome_1;
    lv_obj_t *wifi_dropdown;
    lv_obj_t *start_up_password;
    lv_obj_t *wifi_next_button;
    lv_obj_t *wifi_next_button_text;
    lv_obj_t *main_tab;
    lv_obj_t *focus_timer;
    lv_obj_t *setting_timer;
    lv_obj_t *stop_button_1;
    lv_obj_t *start_button_label;
    lv_obj_t *focus_timer_text;
    lv_obj_t *minute;
    lv_obj_t *hour;
    lv_obj_t *number_blocker;
    lv_obj_t *timer_swithch;
    lv_obj_t *time_highlight;
    lv_obj_t *hour_label;
    lv_obj_t *lock_device_text;
    lv_obj_t *minute_label;
    lv_obj_t *count_down;
    lv_obj_t *stop_button;
    lv_obj_t *stop_button_text;
    lv_obj_t *timer_bar;
    lv_obj_t *focus_timer_time;
    lv_obj_t *media_control;
    lv_obj_t *before;
    lv_obj_t *next;
    lv_obj_t *play;
    lv_obj_t *lower;
    lv_obj_t *higher;
    lv_obj_t *weather;
    lv_obj_t *weather_forcast;
    lv_obj_t *day3;
    lv_obj_t *temp3;
    lv_obj_t *date3;
    lv_obj_t *day2;
    lv_obj_t *temp2;
    lv_obj_t *date2;
    lv_obj_t *day1;
    lv_obj_t *temp1;
    lv_obj_t *date1;
    lv_obj_t *temp_tab2;
    lv_obj_t *main_clock;
    lv_obj_t *date_icon;
    lv_obj_t *date;
    lv_obj_t *temp;
    lv_obj_t *week_date;
    lv_obj_t *time;
    lv_obj_t *calender;
    lv_obj_t *calender_date;
    lv_obj_t *pre_date;
    lv_obj_t *button_pre_text;
    lv_obj_t *next_date;
    lv_obj_t *button_next_text;
    lv_obj_t *rgb_control;
    lv_obj_t *colour_fixer;
    lv_obj_t *rgb_colour_wheel;
    lv_obj_t *on_off_symbol_main;
    lv_obj_t *on_off_symbol_pri;
    lv_obj_t *homepage;
    lv_obj_t *app_a;
    lv_obj_t *app_a_label;
    lv_obj_t *app_a_button;
    lv_obj_t *a;
    lv_obj_t *app_b;
    lv_obj_t *app_b_label;
    lv_obj_t *app_b_button;
    lv_obj_t *b;
    lv_obj_t *app_c;
    lv_obj_t *app_c_label;
    lv_obj_t *app_c_button;
    lv_obj_t *c;
    lv_obj_t *app_d;
    lv_obj_t *app_d_label;
    lv_obj_t *app_d_button;
    lv_obj_t *app_e;
    lv_obj_t *app_e_label;
    lv_obj_t *app_e_button;
    lv_obj_t *e;
    lv_obj_t *setting;
    lv_obj_t *setting_side_bar;
    lv_obj_t *tab_wifi;
    lv_obj_t *ssid;
    lv_obj_t *setting_text;
    lv_obj_t *password;
    lv_obj_t *ssid_text;
    lv_obj_t *password_text;
    lv_obj_t *if_empty;
    lv_obj_t *other;
    lv_obj_t *save_button;
    lv_obj_t *save;
    lv_obj_t *keyboard;
    lv_obj_t *setting_label;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_SET_UP = 1,
    SCREEN_ID_MAIN_SCREEN = 2,
    SCREEN_ID_HOME = 3,
    SCREEN_ID_SETTINGS = 4,
};

void create_screen_set_up();
void tick_screen_set_up();

void create_screen_main_screen();
void tick_screen_main_screen();

void create_screen_home();
void tick_screen_home();

void create_screen_settings();
void tick_screen_settings();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/