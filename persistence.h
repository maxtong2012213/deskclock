#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

	/**
	 * @brief Saves the WiFi credentials to non-volatile storage.
	 * @param ssid The WiFi network name.
	 * @param password The WiFi network password.
	 */
	void save_wifi_credentials(const char* ssid, const char* password);

	/**
	 * @brief Loads the WiFi credentials from non-volatile storage.
	 * @param ssid_buf Buffer to store the loaded SSID.
	 * @param pass_buf Buffer to store the loaded password.
	 * @param max_len The maximum size of the buffers.
	 */
	void load_wifi_credentials(char* ssid_buf, char* pass_buf, int max_len);

	/**
	 * @brief Checks if WiFi credentials have been previously saved.
	 * @return True if credentials exist, false otherwise.
	 */
	bool are_credentials_saved();

    void restart_device(lv_timer_t *timer);


#ifdef __cplusplus
}
#endif

#endif // PERSISTENCE_H
