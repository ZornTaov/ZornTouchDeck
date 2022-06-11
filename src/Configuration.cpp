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
  	  	BleKeyboard* Configuration::bleKeyboard = new BleKeyboard("ZornTouchDeck", "Made by Zorn");
#endif // USEUSBHID

void Configuration::configmode() {

}

bool Configuration::loadMainConfig() {
	if (!FILESYSTEM.exists("/config/wifiConfig.json")) {
		Serial.println("[WARNING]: Config file not found!");
		return false;
	}
	File configfile = FILESYSTEM.open("/config/wifiConfig.json");

	DynamicJsonDocument doc(256);

	DeserializationError error = deserializeJson(doc, configfile);

	strlcpy(wifiConfig.ssid, doc["ssid"] | "FAILED", sizeof(wifiConfig.ssid));
	strlcpy(wifiConfig.password, doc["password"] | "FAILED", sizeof(wifiConfig.password));
	strlcpy(wifiConfig.wifimode, doc["wifimode"] | "FAILED", sizeof(wifiConfig.wifimode));
	strlcpy(wifiConfig.hostname, doc["wifihostname"] | "zorntouchdeck", sizeof(wifiConfig.hostname));

	uint8_t attempts = doc["attempts"] | 10;
	wifiConfig.attempts = attempts;

	uint16_t attemptdelay = doc["attemptdelay"] | 500;
	wifiConfig.attemptdelay = attemptdelay;

	configfile.close();

	if (error) {
		Serial.println("[ERROR]: deserializeJson() error");
		Serial.println(error.c_str());
		return false;
	}

	return true;
}
bool Configuration::checkfile(const char *filename)
{

  if (!FILESYSTEM.exists(filename))
  {
	Screen::instance()->tft.fillScreen(TFT_BLACK);
    Screen::instance()->tft.setCursor(1, 3);
    Screen::instance()->tft.setTextFont(2);
    Screen::instance()->tft.setTextSize(2);
    Screen::instance()->tft.setTextColor(TFT_WHITE, TFT_BLACK);
    Screen::instance()->tft.printf("%s not found!\n\n", filename);
    Screen::instance()->tft.setTextSize(1);
    Screen::instance()->tft.printf("If this has happend after confguration, the data on the ESP may \nbe corrupted.");
    Serialprintln("[ERROR]: %s not found!", filename);
	while (1) yield(); // Stop!
    return false;
  }
  else
  {
    return true;
  }
}
} /* namespace ZTD */
