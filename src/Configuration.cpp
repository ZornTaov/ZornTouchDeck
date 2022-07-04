/*
 * Configuration.cpp
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */

#include "Configuration.h"
#include "Screen.h"
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
Wificonfig* Configuration::wifiConfig = new Wificonfig();
//char* Configuration::logopath

void Configuration::configmode() {

}

bool Configuration::loadWifiConfig() {
	if (!FILESYSTEM.exists("/config/wifiConfig.json")) {
		Serial.println("[WARNING]: Config file not found!");
		return false;
	}
	File configfile = FILESYSTEM.open("/config/wifiConfig.json");

	DynamicJsonDocument doc(256);

	DeserializationError error = deserializeJson(doc, configfile);

	if (error) {
		Serial.println("[ERROR]: deserializeJson() error");
		Serial.println(error.c_str());
		configfile.close();
		return false;
	}
	strlcpy(wifiConfig->ssid, doc["ssid"] | "FAILED", sizeof(wifiConfig->ssid));
	strlcpy(wifiConfig->password, doc["password"] | "FAILED",
			sizeof(wifiConfig->password));
	strlcpy(wifiConfig->wifimode, doc["wifimode"] | "FAILED",
			sizeof(wifiConfig->wifimode));
	strlcpy(wifiConfig->hostname, doc["wifihostname"] | "zorntouchdeck",
			sizeof(wifiConfig->hostname));

	uint8_t attempts = doc["attempts"] | 10;
	wifiConfig->attempts = attempts;

	uint16_t attemptdelay = doc["attemptdelay"] | 500;
	wifiConfig->attemptdelay = attemptdelay;

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

void Configuration::setMenuIndex(uint8_t menuIndex) {
	this->menuIndex = menuIndex;
}

MenuState Configuration::getMenuState() {
	return menuState;
}

void Configuration::setMenuState(MenuState menuState) {
	this->menuState = menuState;
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

char* Configuration::getLogoPath() {
	return Configuration::logopath;
}

} /* namespace ZTD */
