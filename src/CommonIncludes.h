#pragma once
#include "Arduino.h"

#include "stdint.h"
#define FOLD(name) 1

#if FOLD(includes)

#include <SPIFFS.h>
#include <pgmspace.h>
#include <FS.h>
#include <TFT_eSPI.h>

#ifdef USEUSBHID
  #include "USB.h"
  #include "USBHIDKeyboard.h"
  #include "Keydefines.h"

#else
#include <BleKeyboard.h> // BleKeyboard is used to communicate over BLE

// Checking for BLE Keyboard version
#ifndef BLE_KEYBOARD_VERSION
    #warning Old BLE Keyboard version detected. Please update.
    #define BLE_KEYBOARD_VERSION "Outdated"
  #endif // BLE_KEYBOARD_VERSION

#endif // USEUSBHID

#ifdef USE_NIMBLE
  #include "NimBLEDevice.h"   // Additional BLE functionaity using NimBLE
  #include "NimBLEUtils.h"    // Additional BLE functionaity using NimBLE
  #include "NimBLEBeacon.h"   // Additional BLE functionaity using NimBLE

#else
#include "BLEDevice.h"   // Additional BLE functionaity
#include "BLEUtils.h"    // Additional BLE functionaity
#include "BLEBeacon.h"   // Additional BLE functionaity

#endif // USE_NIMBLE

#include "esp_sleep.h"   // Additional BLE functionaity
#include "esp_bt_main.h"   // Additional BLE functionaity
#include "esp_bt_device.h" // Additional BLE functionaity

#include <ArduinoJson.h> // Using ArduinoJson to read and write config files

#include <WiFi.h> // Wifi support

#include <AsyncTCP.h>          //Async Webserver support header
#include <ESPAsyncWebServer.h> //Async Webserver support header
//#include "BluetoothSerial.h"
#include "pthread.h"
#include <ESPmDNS.h> // DNS functionality
#endif //includes

// Set the number of colomns and rows of your buttons here:
#define BTN_COLS 5
#define BTN_ROWS 3

#define ACTION_COUNT 3

// Set the width and height of your screen here:
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#if FOLD(generalDefines)

#define BUTTON_COUNT BTN_COLS * BTN_ROWS

#if BUTTON_COUNT < 2
#warning Need more than one button to function!
#endif

#if BUTTON_COUNT > 100
#warning How and WHY do you have so many buttons! This breaks saving button latches!
#endif
// Keypad start position, centre of the first button
#define KEY_X SCREEN_WIDTH / (BTN_COLS * 2)
#define KEY_Y SCREEN_HEIGHT / (BTN_ROWS * 2)

// Gaps between buttons
#define KEY_SPACING_X SCREEN_WIDTH / (BTN_COLS * 8)
#define KEY_SPACING_Y SCREEN_HEIGHT / (BTN_ROWS * 8)

// Width and height of a button
#define KEY_W (SCREEN_WIDTH / BTN_COLS) - KEY_SPACING_X
#define KEY_H (SCREEN_HEIGHT / BTN_ROWS) - KEY_SPACING_Y

// Set the number of colomns and rows of settings buttons here:
#define S_BTN_COLS 3
#define S_BTN_ROWS 2
// Settings Keypad start position, centre of the first button
#define S_KEY_X SCREEN_WIDTH / (S_BTN_COLS * 2)
#define S_KEY_Y SCREEN_HEIGHT / (S_BTN_ROWS * 2)

// Gaps between settings buttons
#define S_KEY_SPACING_X SCREEN_WIDTH / (S_BTN_COLS * 8)
#define S_KEY_SPACING_Y SCREEN_HEIGHT / (S_BTN_ROWS * 8)

// Width and height of a settings button
#define S_KEY_W (SCREEN_WIDTH / S_BTN_COLS) - S_KEY_SPACING_X
#define S_KEY_H (SCREEN_HEIGHT / S_BTN_ROWS) - S_KEY_SPACING_Y

// Font size multiplier
#define KEY_TEXTSIZE 1

// Text Button Label Font
#define LABEL_FONT &FreeSansBold12pt7b

#define touchInterruptPin GPIO_NUM_27

#define BL_CHANNEL 0
#define FILESYSTEM SPIFFS
#define CALIBRATION_FILE "/TouchCalData"
#define REPEAT_CAL false

enum MenuState : uint8_t {
	HOME,
	SETTINGS,
	ACTION,
	WIFI,
	INFO,
	ERROR,
	OTHER
};

#endif //generalDefines
extern HardwareSerial Serial; // @suppress("Abstract class cannot be instantiated")
