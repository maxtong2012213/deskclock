// This is the combined and updated code for your LVGL Smart Clock.
// It now performs an AUTOMATIC Over-the-Air (OTA) update check on boot.
//#include "lv_conf.h" 
//#include <NimBLEDevice.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <RAK14014_FT6336U.h>
#include "esp_heap_caps.h"
#include "ui.h"
#include "actions.h"
#include "media_control.h"
//NeoPixel
#include <Adafruit_NeoPixel.h>

// --- Networking, Time & OTA Update Libraries ---
#include <WiFi.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h> // For secure HTTPS connections
#include <HTTPUpdate.h>       // For the OTA update process
#include <time.h>             // Required for time functions
#include "cert.h"             // Include your certificate header

// -----------------------------------------------------------------------------
// --- Configuration Section
// -----------------------------------------------------------------------------

//LED config
const int LED_PIN   = 11;       
const int LED_COUNT = 30;

//SAFETY LIMIT
const int BRIGHTNESS_LIMIT = 25; // ~10% brightness limit

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);



// -- Button Config (NEW) --
// GPIO 0 is usually the "BOOT" button on ESP32 boards. 
// If using an external button, change this to your GPIO (e.g., 12, 13, 14).
#define HOME_BTN_PIN 40 

// -- Wi-Fi & API Config --
char wifi_ssid[64] = "";
char wifi_password[64] = "";
const char* PREFERENCES_NAMESPACE = "wifi-creds";
const char* infoApiUrl = "http://max2012213.pythonanywhere.com/api/info";

// -- OTA Update Config --
const char* FIRMWARE_VERSION = "1.0"; 
#define URL_FW_VERSION "https://raw.githubusercontent.com/maxtong2012213/esp32-ota-server/main/firmware/version.txt"
#define URL_FW_BIN "https://raw.githubusercontent.com/maxtong2012213/esp32-ota-server/main/firmware/smart_clock.bin"

// -- Hardware & Display Config --
#define I2C_SDA 18
#define I2C_SCL 17
#define DISP_HOR_RES 480
#define DISP_VER_RES 320

#define TOUCH_SWAP_XY   1
#define TOUCH_INVERT_X  1
#define TOUCH_INVERT_Y  0

// -----------------------------------------------------------------------------
// --- Global Variables & Object Declarations
// -----------------------------------------------------------------------------

TFT_eSPI tft = TFT_eSPI();
FT6336U ts;
Preferences preferences;
static lv_disp_draw_buf_t draw_buf;

//#define BUF_LINES DISP_VER_RES
#define BUF_LINES (DISP_VER_RES / 4)
static lv_color_t *buf1;
static lv_color_t *buf2;

static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_indev_t* indev = nullptr;

volatile bool isRtcSet = false;
String currentPublicIP = ""; 

// -----------------------------------------------------------------------------
// --- Helper Function Definitions
// -----------------------------------------------------------------------------

// UPDATED: Now restarts the device immediately after saving
extern "C" void save_wifi_credentials(const char* ssid, const char* password) {
    preferences.begin(PREFERENCES_NAMESPACE, false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
    
    Serial.println("Credentials saved. Restarting device...");
    delay(500); // Small delay to ensure memory write completes
    ESP.restart();
}

extern "C" void load_wifi_credentials(char* ssid_buf, char* pass_buf, int max_len) {
    preferences.begin(PREFERENCES_NAMESPACE, true);
    preferences.getString("ssid", ssid_buf, max_len);
    preferences.getString("password", pass_buf, max_len);
    preferences.end();
}

extern "C" bool are_credentials_saved() {
    preferences.begin(PREFERENCES_NAMESPACE, true);
    bool keyExists = preferences.isKey("ssid");
    preferences.end();
    return keyExists;
}

extern "C" void restart_device(lv_timer_t *timer) {
  ESP.restart();
}

void connectWiFi(uint32_t timeoutMs = 10000) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.printf("Connecting to %s", wifi_ssid);
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs) {
        delay(250);
        Serial.print(".");
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("WiFi OK, IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("⚠️ WiFi connect timeout.");
    }
}

String getPublicIP() {
  if (currentPublicIP != "") return currentPublicIP; 
  
  HTTPClient http;
  Serial.println("Fetching public IP from api.ipify.org...");
  if (http.begin("http://api.ipify.org/")) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        currentPublicIP = http.getString();
      }
    } else {
      Serial.printf("[HTTP] GET IP failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  return currentPublicIP;
}

// --- Display Timer Callbacks ---

void update_display_time_cb(lv_timer_t *timer) {
    if (!isRtcSet) return;

    static bool show_colon = true;
    struct tm timeinfo;

    // Array of week day names. 
    // Note: tm_wday returns 0 for Sunday, 1 for Monday, etc.
    static const char* week_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    if (getLocalTime(&timeinfo)) {
        // --- Update Time ---
        char time_buffer[6];
        sprintf(time_buffer, "%02d%c%02d",
                timeinfo.tm_hour,
                show_colon ? ':' : ' ',
                timeinfo.tm_min);
        lv_label_set_text(objects.time, time_buffer);
        
        // --- Update Date ---
        char date_buffer[4];
        // Note: added +1 to tm_mon and +1900 to tm_year for correct format
        sprintf(date_buffer, "%02d", timeinfo.tm_mday);
        lv_label_set_text(objects.date, date_buffer); 

        // --- NEW: Update Week Day ---
        // Ensure the object exists before updating to prevent crashes
        if (objects.week_date != NULL) {
            lv_label_set_text(objects.week_date, week_days[timeinfo.tm_wday]);
        }
    }
    show_colon = !show_colon;
}

// Global variables for weather data
char weather_temp_c[10] = "--";
char weather_loc[32] = "";

void fetch_weather_task(void *pvParameters) {
  const int UPDATE_INTERVAL_MS = 10 * 60 * 1000; 

  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
       String ip = getPublicIP();
       if(ip == "") {
         vTaskDelay(pdMS_TO_TICKS(5000));
         continue;
       }

       HTTPClient http;
       String url = String(infoApiUrl) + "?ip=" + ip + "&mode=temp";
       
       http.begin(url);
       int httpCode = http.GET();
       
       if(httpCode == HTTP_CODE_OK) {
         String payload = http.getString();
         StaticJsonDocument<512> doc;
         DeserializationError error = deserializeJson(doc, payload);

         if (!error) {
           const char* t = doc["temperature_c"];
           const char* l = doc["location"];
           
           if(t && l) {
             snprintf(weather_temp_c, sizeof(weather_temp_c), "%s°c", t);
             snprintf(weather_loc, sizeof(weather_loc), "%s", l);
             Serial.printf("Weather Updated: %s in %s\n", weather_temp_c, weather_loc);
           }
         } else {
           Serial.print("JSON Parse failed: "); Serial.println(error.c_str());
         }
       }
       http.end();
    }
    vTaskDelay(pdMS_TO_TICKS(UPDATE_INTERVAL_MS));
  }
}

void update_weather_ui_cb(lv_timer_t *timer) {
    lv_label_set_text(objects.temp, weather_temp_c);
    lv_label_set_text(objects.temp_tab2, weather_temp_c);
}

// --- OTA Update Functions ---

void performFirmwareUpdate() {
    Serial.println("Updating... Do not power off!");
    WiFiClientSecure client;
    client.setCACert(rootCACertificate); 
    t_httpUpdate_return ret = httpUpdate.update(client, URL_FW_BIN);
    switch (ret) {
        case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str()); break;
        case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;
        case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
    }
}

bool firmwareVersionCheck() {
    Serial.println("Checking for new firmware version...");
    WiFiClientSecure client;
    client.setCACert(rootCACertificate);
    HTTPClient https;
    if (!https.begin(client, URL_FW_VERSION)) return false;
    https.setTimeout(10000); 
    int httpCode = https.GET();
    if (httpCode != HTTP_CODE_OK) { https.end(); return false; }
    String payload = https.getString();
    https.end();
    payload.trim();
    if (payload.equals(FIRMWARE_VERSION)) { return false; } 
    else { return true; }
}

void time_sync_task(void *pvParameters) {
  const int RETRY_DELAY_MS = 60 * 1000;
  
  for (;;) { 
    if (WiFi.status() == WL_CONNECTED) {
      String ip = getPublicIP();
      if (ip == "") { vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_MS)); continue; }
      
      HTTPClient http;
      String url = String(infoApiUrl) + "?ip=" + ip + "&mode=time";
      http.begin(url);
      int httpCode = http.GET();

      if (httpCode == HTTP_CODE_OK) {
        StaticJsonDocument<512> doc;
        deserializeJson(doc, http.getString());
        const char* local_time = doc["local_time"];
        const char* date_str = doc["date"];
        
        if (local_time && date_str) {
             struct tm tm;
             sscanf(date_str, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
             sscanf(local_time, "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
             tm.tm_year -= 1900;
             tm.tm_mon -= 1;
             
             struct timeval tv = { mktime(&tm), 0 };
             settimeofday(&tv, NULL);
             isRtcSet = true;
             Serial.println("Time synchronized successfully.");
        }
      }
      http.end();
      vTaskDelay(pdMS_TO_TICKS(24 * 60 * 60 * 1000));
    } else {
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}

void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, lv_area_get_width(area), lv_area_get_height(area));
    tft.pushColors((uint16_t*)&color_p[0], lv_area_get_width(area) * lv_area_get_height(area), true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void my_touch_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
    static int16_t last_x = 0;
    static int16_t last_y = 0;
    if (ts.read_td_status() > 0) {
        int x = ts.read_touch1_x();
        int y = ts.read_touch1_y();
#if TOUCH_SWAP_XY
        { int tmp = x; x = y; y = tmp; }
#endif
#if TOUCH_INVERT_X
        x = (DISP_HOR_RES - 1) - x;
#endif
#if TOUCH_INVERT_Y
        y = (DISP_VER_RES - 1) - y;
#endif
        last_x = x;
        last_y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    data->point.x = last_x;
    data->point.y = last_y;
}

// -----------------------------------------------------------------------------
// --- Main Setup
// -----------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting up Smart Clock...");

    //Initialize LEDs
   strip.begin();           
   strip.show(); // Initialize all pixels to 'off'
  
  // CRITICAL: Set the global brightness limit here.
  // Now, even if we send (255, 255, 255), the strip only shines at level 25.
   strip.setBrightness(BRIGHTNESS_LIMIT);

    media_init();
    //Setup Settings Button pin---
    pinMode(HOME_BTN_PIN, INPUT_PULLUP);

    Wire.begin(I2C_SDA, I2C_SCL, 400000);
    ts.begin();
    tft.begin();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    
    //bluetooth
    Serial.println("Starting Bluetooth Media Service...");

    lv_init();
    
    size_t buffer_size = sizeof(lv_color_t) * DISP_HOR_RES * BUF_LINES;
    buf1 = (lv_color_t *)heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM);
    buf2 = (lv_color_t *)heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM);

    if (buf1 == NULL || buf2 == NULL) {
        Serial.println("Failed to allocate PSRAM buffers!");
        while(1);
    }

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISP_HOR_RES * BUF_LINES);
    
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touch_read;
    indev = lv_indev_drv_register(&indev_drv);
    
    ui_init();
    //set defalt tabview
    lv_scr_load(objects.main_screen);
    lv_tabview_set_act(objects.obj10, 3, LV_ANIM_OFF);
    focus_timer_mode(1);



    lv_label_set_text(objects.time, "--:--");
    lv_label_set_text(objects.temp, "--");

    lv_timer_create(update_display_time_cb, 500, NULL);
    lv_timer_create(update_weather_ui_cb, 2000, NULL);

    if (are_credentials_saved()) {
        load_wifi_credentials(wifi_ssid, wifi_password, sizeof(wifi_ssid));
        connectWiFi();
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Performing automatic update check on boot...");
            if (firmwareVersionCheck()) {
                delay(2000);
                performFirmwareUpdate();
            }
            xTaskCreatePinnedToCore(time_sync_task, "TimeSync", 8192, NULL, 1, NULL, 0);
            xTaskCreatePinnedToCore(fetch_weather_task, "WeatherSync", 8192, NULL, 1, NULL, 0);
        }
    } else {
        Serial.println("No credentials saved. Loading settings screen.");
        lv_scr_load(objects.settings);
    }
}

// -----------------------------------------------------------------------------
// --- Main Loop
// -----------------------------------------------------------------------------

void loop() {
    static uint32_t last_ms = 0;
    uint32_t now = millis();
    lv_tick_inc(now - last_ms);
    last_ms = now;
    lv_timer_handler();

    ui_tick();

    media_tick();

    
    // --- NEW: Check Button Press to Open Settings ---
    // If button is pressed (LOW due to Pullup), switch screen
    if (digitalRead(HOME_BTN_PIN) == LOW) {
        // Simple debounce
        delay(50);
        if (digitalRead(HOME_BTN_PIN) == LOW) {
            Serial.println("Physical button pressed: Loading Settings...");
            lv_scr_load(objects.home);
            
            // Wait until button is released to prevent repeated triggering
            while(digitalRead(HOME_BTN_PIN) == LOW) { delay(10); }
        }
    }

    delay(5);
}