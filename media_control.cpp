#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>
#include <NimBLEHIDDevice.h>
#include "media_control.h"

// --- CONFIGURATION ---
#define DEVICE_NAME "Smart Clock"
#define DEVICE_MANUFACTURER "Espressif"

// Define the missing Appearance Constant manually if needed
#ifndef HID_CONSUMER
#define HID_CONSUMER 0x03C0 // Generic Remote Control
#endif

// --- GLOBALS ---
NimBLEHIDDevice* hid = nullptr;
NimBLECharacteristic* inputMedia = nullptr;
NimBLECharacteristic* inputKeyboard = nullptr; // NEW: Keyboard Input Report
bool deviceConnected = false;

// --- HID REPORT DESCRIPTOR (Media Keys + Standard Keyboard) ---
static const uint8_t _hidReportDescriptor[] = {
  // -------------------------------------------------
  // REPORT ID 1: MEDIA KEYS (Consumer Control)
  // -------------------------------------------------
  0x05, 0x0C,        // Usage Page (Consumer)
  0x09, 0x01,        // Usage (Consumer Control)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x07,        //   Report Count (7 bits)
  0x09, 0xB5,        //   Usage (Scan Next Track)
  0x09, 0xB6,        //   Usage (Scan Previous Track)
  0x09, 0xB7,        //   Usage (Stop)
  0x09, 0xCD,        //   Usage (Play/Pause)
  0x09, 0xE2,        //   Usage (Mute)
  0x09, 0xE9,        //   Usage (Volume Increment)
  0x09, 0xEA,        //   Usage (Volume Decrement)
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x95, 0x01,        //   Report Count (1 bit padding)
  0x81, 0x03,        //   Input (Const, Var, Abs)
  0xC0,              // End Collection

  // -------------------------------------------------
  // REPORT ID 2: KEYBOARD (For Windows + L)
  // -------------------------------------------------
  0x05, 0x01,        // Usage Page (Generic Desktop)
  0x09, 0x06,        // Usage (Keyboard)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x02,        //   Report ID (2)
  0x05, 0x07,        //   Usage Page (Key Codes)
  0x19, 0xE0,        //   Usage Min (224) - Left Ctrl
  0x29, 0xE7,        //   Usage Max (231) - Right GUI
  0x15, 0x00,        //   Logical Min (0)
  0x25, 0x01,        //   Logical Max (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8) - 8 Modifier bits
  0x81, 0x02,        //   Input (Data, Var, Abs)
  0x95, 0x01,        //   Report Count (1)
  0x75, 0x08,        //   Report Size (8)
  0x81, 0x03,        //   Input (Constant) - Reserved byte
  0x95, 0x06,        //   Report Count (6) - 6 Keys
  0x75, 0x08,        //   Report Size (8)
  0x15, 0x00,        //   Logical Min (0)
  0x25, 0x65,        //   Logical Max (101)
  0x05, 0x07,        //   Usage Page (Key Codes)
  0x19, 0x00,        //   Usage Min (0)
  0x29, 0x65,        //   Usage Max (101)
  0x81, 0x00,        //   Input (Data, Array)
  0xC0               // End Collection
};

// --- CALLBACKS (NimBLE 2.0 Compatible) ---
class MyCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    deviceConnected = true;
  };

  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
    deviceConnected = false;
    NimBLEDevice::getAdvertising()->start();
  }
};

// --- INITIALIZATION ---
void media_init(void) {
  Serial.println("BLE: Starting Embedded NimBLE Keyboard...");

  // 1. Init NimBLE
  NimBLEDevice::init(DEVICE_NAME);
  
  // 2. Security
  NimBLEDevice::setSecurityAuth(true, true, true);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);

  // 3. Create Server
  NimBLEServer *pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());

  // 4. Create HID Device
  hid = new NimBLEHIDDevice(pServer);
  
  // 5. Setup Input Reports
  inputMedia = hid->getInputReport(1);    // Media Keys (ID 1)
  inputKeyboard = hid->getInputReport(2); // NEW: Keyboard (ID 2)
  
  // 6. Device Info
  hid->setManufacturer(DEVICE_MANUFACTURER);
  hid->setPnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->setHidInfo(0x00, 0x01);

  // 7. Load the Report Map
  hid->setReportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  hid->startServices();

  // 8. Advertise
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setAppearance(HID_CONSUMER); 
  pAdvertising->addServiceUUID(hid->getHidService()->getUUID());
  pAdvertising->start();
  
  Serial.println("BLE: Ready! Connect to 'Smart Clock' on Windows.");
}

// --- STATUS CHECK ---
void media_tick(void) {
    static bool oldStatus = false;
    if (deviceConnected && !oldStatus) {
        Serial.println(">>> SUCCESS: CONNECTED! <<<");
    } else if (!deviceConnected && oldStatus) {
        Serial.println(">>> DISCONNECTED <<<");
    }
    oldStatus = deviceConnected;
}

int media_is_connected(void) {
  return deviceConnected ? 1 : 0;
}

// --- SEND KEY HELPER (MEDIA) ---
void send_media_key(uint8_t bit_index) {
  if (!deviceConnected) return;
  
  uint8_t report[1] = {0};
  
  // Press
  report[0] = (1 << bit_index); 
  inputMedia->setValue(report, sizeof(report));
  inputMedia->notify();
  
  // Release
  delay(10); 
  report[0] = 0;
  inputMedia->setValue(report, sizeof(report));
  inputMedia->notify();
}

// --- PUBLIC COMMANDS ---
void media_send_next(void)       { send_media_key(0); Serial.println("CMD: Next"); }
void media_send_prev(void)       { send_media_key(1); Serial.println("CMD: Prev"); }
void media_send_stop(void)       { send_media_key(2); Serial.println("CMD: Stop"); }
void media_send_play_pause(void) { send_media_key(3); Serial.println("CMD: Play/Pause"); }
void media_send_vol_up(void)     { send_media_key(5); Serial.println("CMD: Vol+"); }
void media_send_vol_down(void)   { send_media_key(6); Serial.println("CMD: Vol-"); }

// --- NEW FUNCTION: Windows + L ---
void get_some_rest(void) {
    if (!deviceConnected) return;

    // Standard HID Keyboard Report (8 bytes)
    // Byte 0: Modifiers (0x08 = Left GUI / Windows Key)
    // Byte 1: Reserved
    // Byte 2: Keycode (0x0F = 'l')
    // Byte 3-7: Other keys
    uint8_t report[8] = {0};

    // 1. Press GUI + L
    report[0] = 0x08; // Windows key
    report[2] = 0x0F; // Key 'l'
    
    inputKeyboard->setValue(report, sizeof(report));
    inputKeyboard->notify();

    // 2. Release Keys
    delay(50); // Small delay to ensure OS registers it
    memset(report, 0, sizeof(report)); // Clear all
    
    inputKeyboard->setValue(report, sizeof(report));
    inputKeyboard->notify();

    Serial.println("CMD: Windows + L (Locking PC)");
}