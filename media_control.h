#ifndef MEDIA_CONTROL_H
#define MEDIA_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the Bluetooth Keyboard/Media Service
void media_init(void);

// Checks connection status (optional to call in loop)
void media_tick(void);

// Returns 1 if connected, 0 if not
int media_is_connected(void);

// Media Control Commands
void media_send_play_pause(void);
void media_send_stop(void);
void media_send_next(void);
void media_send_prev(void);
void media_send_vol_up(void);
void media_send_vol_down(void);

// NEW: Lock Computer (Windows + L)
void get_some_rest(void);

#ifdef __cplusplus
}
#endif

#endif