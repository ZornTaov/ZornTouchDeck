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

bool loadingDone = false;
WireframeDemo *wireframeDemo;

void initDisplay() {
	Serial.println("[INFO]: Loading saved brightness state");
	Screen::instance()->ledBrightness =
			Configuration::instance()->savedStates->getInt("ledBrightness", 255);

	// Setup PWM channel and attach pin bl_pin
	ledcSetup(BL_CHANNEL, 5000, 8);
#ifdef TFT_BL
	ledcAttachPin(TFT_BL, BL_CHANNEL);
#else
	ledcAttachPin(backlightPin, BL_CHANNEL);
#endif // TFT_BL
	ledcWrite(BL_CHANNEL, Screen::instance()->ledBrightness); // Start @ initial Brightness

	// Initialise the TFT screen
	Screen::getTFT()->init();

	// Set the rotation before we calibrate
	Screen::getTFT()->setRotation(1);

	// Clear the screen
	Screen::getTFT()->fillScreen(TFT_BLACK);
}

void initFileSystem() {
	if (!FILESYSTEM.begin()) {
		Serial.println("[ERROR]: FILESYSTEM initialisation failed!");
		Screen::instance()->drawErrorMessage(
				"Failed to init FILESYSTEM! Did you upload the data folder?");
		while (1)
			yield(); // We stop here
	}
	Serial.println("[INFO]: FILESYSTEM initialised.");
	// Check for free space
	Serial.print("[INFO]: Free Space: ");
	Serial.println(FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes());
}

void initWifiConfig() {
	Serial.println("[INFO]: Loading Wifi Config");
	if (!Configuration::instance()->loadWifiConfig()) {
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
		Screen::getTFT()->setTextFont(2);
		Screen::getTFT()->println(" Waking up...");
	} else {
		//TODO Draw a splash screen
		//drawBmp("/logos/freetouchdeck_logo.bmp", 0, 0);
		Screen::getTFT()->setCursor(1, 3);
		Screen::getTFT()->setTextFont(2);
		Screen::getTFT()->setTextSize(1);
		Screen::getTFT()->setTextColor(TFT_WHITE, TFT_BLACK);
		//Screen::getTFT()->printf("Loading version %s\n", versionnumber);
		//Serial.printf("[INFO]: Loading version %s\n", versionnumber);
	}
}

void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	Serial.println("");

	Configuration::instance()->savedStates->begin("ftd", false);

// --------------- Init Display -------------------------
	initDisplay();

	wireframeDemo = new WireframeDemo();
	wireframeDemo->setupDemo();

// -------------- Start filesystem ----------------------
	initFileSystem();

//------------------ Load Wifi Config -------------------
	initWifiConfig();

// ----------------- Load webserver ---------------------
	//TODO Wifi
	//handlerSetup();
// ------------------- Splash screen --------------------
	splashScreen();

// ----------------- Calibrate Touch --------------------
#ifndef USECAPTOUCH
	Serial.println("[INFO]: Waiting for touch calibration...");

	Configuration::instance()->touch_calibrate();
	Serial.println("[INFO]: Touch calibration completed!");
#endif // USECAPTOUCH

// ----------------- Validate Files ---------------------
	Configuration::instance()->checkfile("/config/general.json");
	//Configuration::instance()->checkfile("/config/homescreen.json");
	Configuration::instance()->checkfile("/config/menu.json");
	//Configuration::instance()->checkfile("/config/general.json");

	Screen::getTFT()->setFreeFont(LABEL_FONT);

// --------------------- Load Files ---------------------

	//	Serial.println("[INFO]: Reading latch stated back from memory:");
	//	Configuration::instance()->savedStates.getBytes("latched", Configuration::instance()->islatched, sizeof(Configuration::instance()->islatched));
	//
	//	for(int i = 0; i < sizeof(islatched); i++){
	//		Serial.print(islatched[i]);
	//	}
	//	Serial.println();

//----------------- BLE Initialization ------------------
#ifdef USEUSBHID

	// initialize control over the keyboard:
	bleKeyboard->begin();
	USB->begin();

#else
	Serial.println("[INFO]: Starting BLE");
	Configuration::getBleKeyboard()->begin();
#endif //USEUSBHID

//------------- Speaker Initialization ------------------
	Serial.println("[INFO]: Init speakerPin");
#ifdef speakerPin
  ledcSetup(2, 500, 8);

  if(generalconfig.beep){
    ledcAttachPin(speakerPin, 2);
    ledcWriteTone(2, 600);
    delay(150);
    ledcDetachPin(speakerPin);
    ledcWrite(2, 0);

    ledcAttachPin(speakerPin, 2);
    ledcWriteTone(2, 800);
    delay(150);
    ledcDetachPin(speakerPin);
    ledcWrite(2, 0);

    ledcAttachPin(speakerPin, 2);
    ledcWriteTone(2, 1200);
    delay(150);
    ledcDetachPin(speakerPin);
    ledcWrite(2, 0);
  }

#endif // defined(speakerPin)

// ---------------- Printing version numbers ------------
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
	Screen::getTFT()->fillScreen(
			Configuration::instance()->getGConf()->backgroundColor);

	// Draw keypad
	Serial.println("[INFO]: Drawing keypad");
	Screen::instance()->drawKeypad();

#ifdef touchInterruptPin
	if (Configuration::instance()->getGConf()->sleepEnable) {
		pinMode(touchInterruptPin, INPUT_PULLUP);
		Configuration::instance()->Interval =
				Configuration::instance()->getGConf()->sleepTimer * 60000;
		Serial.println("[INFO]: Sleep enabled.");
		Serial.print("[INFO]: Sleep timer = ");
		Serial.print(Configuration::instance()->getGConf()->sleepTimer);
		Serial.println(" minutes");
		//Configuration::instance()->isLatched[28] = 1;
	}
#endif // touchInterruptPin

	loadingDone = true;
	Serial.println("[INFO]: Boot completed and successful!");
}

void readSerial() {
	if (Serial.available()) {
		String command = Serial.readStringUntil(' ');
		if (command == "cal") {
			FILESYSTEM.remove(CALIBRATION_FILE);
			ESP.restart();
		} else if (command == "restart") {
			Serial.println("[WARNING]: Restarting");
			ESP.restart();
		}
		/* else if (command == "setssid") {
		 String value = Serial.readString();
		 if (saveWifiSSID(value)) {
		 Serial.printf("[INFO]: Saved new SSID: %s\n", value.c_str());
		 loadMainConfig();
		 Serial.println("[INFO]: New configuration loaded");
		 }
		 } else if (command == "setpassword") {
		 String value = Serial.readString();
		 if (saveWifiPW(value)) {
		 Serial.printf("[INFO]: Saved new Password: %s\n",
		 value.c_str());
		 loadMainConfig();
		 Serial.println("[INFO]: New configuration loaded");
		 }
		 } else if (command == "setwifimode") {
		 String value = Serial.readString();
		 if (saveWifiMode(value)) {
		 Serial.printf("[INFO]: Saved new WiFi Mode: %s\n",
		 value.c_str());
		 loadMainConfig();
		 Serial.println("[INFO]: New configuration loaded");
		 }
		 } else if (command == "reset") {
		 String file = Serial.readString();
		 Serial.printf("[INFO]: Resetting %s.json now\n", file.c_str());
		 resetconfig(file);
		 } */
		//Serial.println(command);
	}
}

void saveLatches() {
	for (uint8_t menu = 0; menu < BUTTON_COUNT - 1; menu++) {
		for (uint8_t button = 0; button < BUTTON_COUNT - 1; button++) {
			char key[] = "latched-00-00";
			key[8] += menu / 10;
			key[9] += menu % 10;
			key[11] += button / 10;
			key[12] += button % 10;
			Configuration::instance()->savedStates->putBool(key,
					&Configuration::getMenus()[menu].buttons[button].latch);
		}
	}
}

#ifdef touchInterruptPin
void checkSleepTimer() {
	if (Configuration::instance()->getGConf()->sleepEnable) {
		if (millis()
				> Configuration::instance()->previousMillis
						+ Configuration::instance()->Interval) {

			// The timer has ended and we are going to sleep.
			Screen::getTFT()->fillScreen(TFT_BLACK);
			Serial.println("[INFO]: Going to sleep.");
#ifdef speakerPin
        if(generalconfig.beep){
        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 1200);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);

        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 800);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);

        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 600);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);
        }
#endif // defined(speakerPin)
			Serial.println("[INFO]: Saving latched states");

			saveLatches();
			esp_sleep_enable_ext0_wakeup(touchInterruptPin, 0);
			esp_deep_sleep_start();
		}
	}
}
#endif // defined(touchInterruptPin)

bool getPressed(uint16_t *t_x, uint16_t *t_y) {
#ifdef USECAPTOUCH
    if (ts.touched())
    {

      // Retrieve a point
      TS_Point p = ts.getPoint();

      //Flip things around so it matches our screen rotation
      p.x = map(p.x, 0, 320, 320, 0);
      t_y = p.x;
      t_x = p.y;

      return true;
    }

#else
	return Screen::getTFT()->getTouch(t_x, t_y);
#endif
}

void loop() {

	readSerial();

	//wireframeDemo->loopDemo();

#ifdef touchInterruptPin
	checkSleepTimer();
#endif // defined(touchInterruptPin)

	// Touch coordinates are stored here
	uint16_t t_x = 0, t_y = 0;
	bool pressed = getPressed(&t_x, &t_y);
	uint8_t btnCount = BUTTON_COUNT;
	uint8_t rows = BTN_ROWS;
	uint8_t cols = BTN_COLS;
	if(Configuration::getMenuState() == SETTINGS)
	{
		btnCount = 6;
		rows = S_BTN_ROWS;
		cols = S_BTN_COLS;
	}
	// Check if the X and Y coordinates of the touch are within one of our buttons
	for (uint8_t b = 0; b < btnCount; b++) {
		if (pressed && Screen::instance()->getKey(b)->contains(t_x, t_y)) {
			Screen::instance()->getKey(b)->press(true); // tell the button it is pressed

			// After receiving a vallid touch reset the sleep timer
			Configuration::instance()->previousMillis = millis();
		} else {
			Screen::instance()->getKey(b)->press(false); // tell the button it is NOT pressed
		}
	}

	for (uint8_t row = 0; row < rows; row++) {
		for (uint8_t col = 0; col < cols; col++) {
			uint8_t b = col + row * cols;
			if (Screen::instance()->getKey(b)->justReleased()) {
				//TODO re-draw logo when released
				Screen::instance()->drawButton(col, row);
			}
			if (Screen::instance()->getKey(b)->justPressed()) {
				//TODO do keypad action

				// Beep
				#ifdef speakerPin
				if(generalconfig.beep){
				  ledcAttachPin(speakerPin, 2);
				  ledcWriteTone(2, 600);
				  delay(50);
				  ledcDetachPin(speakerPin);
				  ledcWrite(2, 0);
				}
				#endif
				//set button to white for "pressed" state
				Screen::instance()->drawBasicButton(col, row, TFT_WHITE);

				ZTDMenu::doButton(b);
			}
		}
	}
}

