/*
 * Configuration.cpp
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */

#include "Configuration.h"
#include "Screen.h"
#include "WifiSetup.h"
namespace ZTD {

#ifdef USEUSBHID
USBHIDKeyboard* Configuration::bleKeyboard;

#else
BleKeyboard *Configuration::bleKeyboard = new BleKeyboard("ZornTouchDeck", "Made by Zorn");
#endif // USEUSBHID

GeneralConfig* Configuration::generalConfig = new GeneralConfig();


MenuState Configuration::menuState = HOME;
uint8_t Configuration::menuIndex = 0;
ZTDButton* Configuration::homeButton = new ZTDButton();
ZTDButton Configuration::home[BUTTON_COUNT] = {};
ZTDButton Configuration::settings[6] = {};
ZTDMenu Configuration::menus[BUTTON_COUNT-1] = {};
Preferences* Configuration::savedStates = new Preferences();
ZTDWifi* Configuration::wifiConfig = new ZTDWifi();
const char* Configuration::logopath = "/logos/";

void Configuration::wifiConfigmode() {
	Configuration::wifiConfig->configmode();
}

bool Configuration::loadWifiConfig() {
	if (!FILESYSTEM.exists("/config/wificonfig.json")) {
		Serial.println("[WARNING]: Config file not found, generating default!");
		wifiConfig = new ZTDWifi();
		DynamicJsonDocument doc(ZTDWifi::getJsonSize());
		doc["wifi"] = *wifiConfig;
		File configfile = FILESYSTEM.open("/config/wificonfig.json", "w");
		serializeJson(doc, configfile);
		configfile.close();
		return true;
	}
	File configfile = FILESYSTEM.open("/config/wificonfig.json");

	DynamicJsonDocument doc(ZTDWifi::getJsonSize());

	DeserializationError error = deserializeJson(doc, configfile);

	if (error) {
		Serial.println("[ERROR]: deserializeJson() error");
		Serial.println(error.c_str());
		configfile.close();
		return false;
	}
	*wifiConfig = doc["wifi"];

	configfile.close();

	return true;
}
bool Configuration::checkfile(const char *filename) {

	if (!FILESYSTEM.exists(filename)) {
		Screen::getTFT()->fillScreen(TFT_BLACK);
		Screen::getTFT()->setCursor(1, 3);
		Screen::getTFT()->setTextFont(2);
		Screen::getTFT()->setTextSize(2);
		Screen::getTFT()->setTextColor(TFT_WHITE, TFT_BLACK);
		Screen::getTFT()->printf("%s not found!\n\n", filename);
		Screen::getTFT()->setTextSize(1);
		Screen::getTFT()->printf(
				"If this has happend after confguration, the data on the ESP may \nbe corrupted.");
		Serialprintln("[ERROR]: %s not found!", filename);
		while (1)
			yield(); // Stop!
		return false;
	} else {
		return true;
	}
}

/**
* @brief This function presents the user with 4 points to touch and saves
         that data to a claibration file.
*
* @param none
*
* @return none
*
* @note If USECAPTOUCH is defined we do not need to calibrate touch
*/

#ifndef USECAPTOUCH
void Configuration::touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check if calibration file exists and size is correct
  if (FILESYSTEM.exists(CALIBRATION_FILE))
  {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      FILESYSTEM.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = FILESYSTEM.open(CALIBRATION_FILE, "r");
      if (f)
      {
        if (f.readBytes((char *)calData, 10) == 10)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL)
  {
    // calibration data valid
	  Screen::getTFT()->setTouch(calData);
		for (uint8_t i = 0; i < 5; i++) {
			Serial.print(calData[i], HEX);
			Serial.print(", ");
		}
		Serial.println();
  }
  else
  {
    // data not valid so recalibrate
	  Screen::getTFT()->fillScreen(TFT_BLACK);
	  Screen::getTFT()->setCursor(20, 0);
	  Screen::getTFT()->setTextFont(2);
	  Screen::getTFT()->setTextSize(1);
	  Screen::getTFT()->setTextColor(TFT_WHITE, TFT_BLACK);

	  Screen::getTFT()->println("Touch corners as indicated");

	  Screen::getTFT()->setTextFont(1);
	  Screen::getTFT()->println();

    if (REPEAT_CAL)
    {
    	Screen::getTFT()->setTextColor(TFT_RED, TFT_BLACK);
    	Screen::getTFT()->println("Set REPEAT_CAL to false to stop this running again!");
    }

    Screen::getTFT()->calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    Screen::getTFT()->setTextColor(TFT_GREEN, TFT_BLACK);
    Screen::getTFT()->println("Calibration complete!");

    // store data
    File f = FILESYSTEM.open(CALIBRATION_FILE, "w");
    if (f)
    {
      f.write((const unsigned char *)calData, 10);
      f.close();
    }
  }
}
#endif //!defined(USECAPTOUCH)

BleKeyboard* Configuration::getBleKeyboard() {
	return bleKeyboard;
}

ZTDButton* Configuration::getHomeLogos() {
	return home;
}

ZTDButton* Configuration::getHomeButton() {
	return homeButton;
}

ZTDMenu* Configuration::getMenus() {
	return menus;
}

uint8_t Configuration::getMenuIndex() {
	return menuIndex;
}

void Configuration::setMenuIndex(uint8_t menuindex) {
	Serialprintln("MenuIndex O:%d N:%d", menuIndex, menuindex);
	menuIndex = menuindex;
}

MenuState Configuration::getMenuState() {
	return menuState;
}

void Configuration::setMenuState(MenuState menustate) {
	Serialprintln("MenuState O:%d N:%d", menuState, menustate);
	menuState = menustate;
}

GeneralConfig* Configuration::getGConf() {
	return generalConfig;
}

void Configuration::setGConf(GeneralConfig *config) {
	generalConfig = config;
}

bool Configuration::loadConfig(String value) {
	//TODO load Config
	return false;
}

ZTDButton* Configuration::getSettingsLogos() {
	return settings;
}

const char* Configuration::getLogoPath() {
	return Configuration::logopath;
}

ZTDButton* Configuration::getSettingsMenu() {
	return Configuration::settings;
}

void Configuration::dumpFS() {
	/*DynamicJsonDocument doc(ZTDWifi::getJsonSize());
	if (!FILESYSTEM.exists("/config/wificonfig.json")) {
		Serial.println("[WARNING]: Config file not found!");
		doc["wifi"] = *wifiConfig;
	}
	else
	{
		File configfile = FILESYSTEM.open("/config/wificonfig.json");
		DeserializationError error = deserializeJson(doc, configfile);

		if (error) {
			Serial.println("[ERROR]: deserializeJson() error");
			Serial.println(error.c_str());
			configfile.close();
			return;
		}
		configfile.close();
	}
	serializeJson(doc, Serial);*/
	// The above line prints:
	// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

	// Start a new line
	String results[2] = {
	"general.json",
	"wificonfig.json"};
	for (int i = 0; i < 2; i++) {

		File file = SPIFFS.open("/config/"+results[i]);
		if(!file){
		    Serialprintln("Failed to %s file for reading", results[i]);
		    return;
		}
		while(file.available()){
		    Serial.write(file.read());
		}
		file.close();
	}
	File root = SPIFFS.open("/");

	File file = root.openNextFile();

	while(file){

		Serial.print("FILE: ");
		Serial.println(file.name());

		file = root.openNextFile();
	}

	Serial.println();

}

} /* namespace ZTD */
