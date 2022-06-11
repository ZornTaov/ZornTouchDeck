//#pragma once

#include <iostream>
#include <sstream>
#include "CommonIncludes.h"
#include "KeyboardHelper.h"
#include "Configuration.h"
#include "Screen.h"
#include "ZTDMenu.h"
#include "Printf.h"
#include "WireframeDemo.h"

#include "ZTDJsonConvert.h"

AsyncWebServer webserver(80);

using namespace ZTD;

WireframeDemo* wireframeDemo;
void initDisplay() {
	Serial.println("[INFO]: Loading saved brightness state");
	Screen::instance()->ledBrightness = Configuration::instance()->savedStates.getInt("ledBrightness", 255);

	// Setup PWM channel and attach pin bl_pin
	ledcSetup(BL_CHANNEL, 5000, 8);
#ifdef TFT_BL
	ledcAttachPin(TFT_BL, BL_CHANNEL);
#else
	ledcAttachPin(backlightPin, BL_CHANNEL);
#endif // TFT_BL
	ledcWrite(BL_CHANNEL, Screen::instance()->ledBrightness); // Start @ initial Brightness

	// Initialise the TFT screen
	Screen::instance()->tft.init();

	// Set the rotation before we calibrate
	Screen::instance()->tft.setRotation(1);

	// Clear the screen
	Screen::instance()->tft.fillScreen(TFT_GREEN);
}

void initFileSystem() {
	if (!FILESYSTEM.begin()) {
		Serial.println("[ERROR]: FILESYSTEM initialisation failed!");
		Screen::instance()->drawErrorMessage("Failed to init FILESYSTEM! Did you upload the data folder?");
		while (1)
			yield(); // We stop here
	}
	Serial.println("[INFO]: FILESYSTEM initialised.");
	// Check for free space
	Serial.print("[INFO]: Free Space: ");
	Serial.println(FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes());
}

void initWifiConfig() {
	//------------------ Load Wifi Config ----------------------------------------------
	Serial.println("[INFO]: Loading Wifi Config");
	if (!Configuration::instance()->loadMainConfig()) {
		Serial.println("[WARNING]: Failed to load WiFi Credentials!");
	} else {
		Serial.println("[INFO]: WiFi Credentials Loaded");
	}
}

void splashScreen() {
	// ------------------- Splash screen ------------------
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause();
	// If we are woken up we do not need the splash screen
	if (wakeup_reason > 0) {
		// But we do draw something to indicate we are waking up
		Screen::instance()->tft.setTextFont(2);
		Screen::instance()->tft.println(" Waking up...");
	} else {
		// Draw a splash screen
		//drawBmp("/logos/freetouchdeck_logo.bmp", 0, 0);
		Screen::instance()->tft.setCursor(1, 3);
		Screen::instance()->tft.setTextFont(2);
		Screen::instance()->tft.setTextSize(1);
		Screen::instance()->tft.setTextColor(TFT_WHITE, TFT_BLACK);
		//Screen::instance()->tft.printf("Loading version %s\n", versionnumber);
		//Serial.printf("[INFO]: Loading version %s\n", versionnumber);
	}
}

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	Serial.println("");

	Configuration::instance()->savedStates.begin("ftd", false);
//	Serial.println("[INFO]: Reading latch stated back from memory:");
//	Configuration::instance()->savedStates.getBytes("latched", Configuration::instance()->islatched, sizeof(Configuration::instance()->islatched));
//
//	for(int i = 0; i < sizeof(islatched); i++){
//		Serial.print(islatched[i]);
//	}
//	Serial.println();

// --------------- Init Display -------------------------
	initDisplay();

// -------------- Start filesystem ----------------------
	initFileSystem();

//------------------ Load Wifi Config -------------------
	initWifiConfig();

// ----------------- Load webserver ---------------------
	//handlerSetup();

// ------------------- Splash screen --------------------
	splashScreen();

// ----------------- Calibrate Touch --------------------
#ifndef USECAPTOUCH
	Serial.println("[INFO]: Waiting for touch calibration...");
	//touch_calibrate();
	Serial.println("[INFO]: Touch calibration completed!");
#endif // USECAPTOUCH

// ----------------- Validate Files ---------------------
	Configuration::instance()->checkfile("/config/general.json");
	//Configuration::instance()->checkfile("/config/homescreen.json");
	Configuration::instance()->checkfile("/config/menu.json");
	//Configuration::instance()->checkfile("/config/general.json");

	Screen::instance()->tft.setFreeFont(LABEL_FONT);










//------------------BLE Initialization ------------------------------------------------------------------------
#ifdef USEUSBHID

	// initialize control over the keyboard:
	bleKeyboard->begin();
	USB->begin();

#else
	Serial.println("[INFO]: Starting BLE");
	Configuration::instance()->bleKeyboard->begin();
#endif //USEUSBHID

// ---------------- Printing version numbers -----------------------------------------------
#ifdef USEUSBHID
	Serial.println("[INFO]: Using USB Keyboard");
#else
	Serial.print("[INFO]: BLE Keyboard version: ");
	Serial.println(BLE_KEYBOARD_VERSION);
#endif //USEUSBHID

	Serial.print("[INFO]: ArduinoJson version: ");
	Serial.println(ARDUINOJSON_VERSION);
	Serial.print("[INFO]: TFT_eSPI version: ");
	Serial.println(TFT_ESPI_VERSION);

// ---------------- Start the first keypad -------------
	// Draw background
	Screen::instance()->tft.fillScreen(Configuration::instance()->generalConfig.backgroundColor);

	// Draw keypad
	Serial.println("[INFO]: Drawing keypad");
	//drawKeypad();

#ifdef touchInterruptPin
	if (Configuration::instance()->generalConfig.sleepEnable) {
		pinMode(touchInterruptPin, INPUT_PULLUP);
		Configuration::instance()->Interval = Configuration::instance()->generalConfig.sleepTimer * 60000;
		Serial.println("[INFO]: Sleep enabled.");
		Serial.print("[INFO]: Sleep timer = ");
		Serial.print(Configuration::instance()->generalConfig.sleepTimer);
		Serial.println(" minutes");
		//Configuration::instance()->isLatched[28] = 1;
	}
#endif // touchInterruptPin

	Serial.println("[INFO]: Boot completed and successful!");
	wireframeDemo = new WireframeDemo();
	wireframeDemo->setupDemo();
}

void loop() {
	wireframeDemo->loopDemo();
}
