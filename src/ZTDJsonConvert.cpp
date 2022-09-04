/*
 * Structs.cpp
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */

#include "ZTDJsonConvert.h"
#include "ZTDMenu.h"
#include "GeneralConfig.h"
#include <ArduinoJson/Object/ObjectShortcuts.hpp>
#include "ZTDWifi.h"
namespace ZTD {

//=================================================================
// ZTDAction Convert
//=================================================================
void convertToJson(const ZTDAction &src, JsonVariant dst) {
	dst["action"] = src.action;
	dst["value"] = src.value;
	dst["symbol"] = src.symbol;
}

void convertFromJson(JsonVariantConst src, ZTDAction &dst) {
	dst.action = src["action"];
	dst.value = src["value"];
	strlcpy(dst.symbol, src["symbol"] | "", sizeof(dst.symbol));
}

size_t ZTDAction::getJsonSize() {
	return JSON_OBJECT_SIZE(3) + JSON_STRING_SIZE(32);
}

//=================================================================
// ZTDButton Convert
//=================================================================
void convertToJson(const ZTDButton &src, JsonVariant dst) {
	for (int i = 0; i < ACTION_COUNT; ++i) {
		dst["actions"][i] = src.actions[i];
	}
	dst["latch"] = src.latch;
	dst["latchlogo"] = src.latchlogo;
	dst["logo"] = src.logo;
}

void convertFromJson(JsonVariantConst src, ZTDButton &dst) {
	for (int i = 0; i < ACTION_COUNT; ++i) {
		dst.actions[i] = src["actions"][i];
	}
	strlcpy(dst.logo, src["logo"] | "question.bmp", 64);
	dst.latch = src["latch"] | false;
	strlcpy(dst.latchlogo, src["latchlogo"] | "question.bmp", sizeof(dst.latchlogo));
}

size_t ZTDButton::getJsonSize() {
	size_t logo = JSON_STRING_SIZE(64) + JSON_OBJECT_SIZE(1);
	return JSON_STRING_SIZE(32) + JSON_ARRAY_SIZE(ACTION_COUNT) + ZTDAction::getJsonSize() * ACTION_COUNT + logo;
}

//=================================================================
// ZTDMenu Convert
//=================================================================
void convertToJson(const ZTDMenu &src, JsonVariant dst) {
	for (int i = 0; i < BUTTON_COUNT - 1; ++i) {
		dst["buttons"][i] = src.buttons[i];
	}
}

void convertFromJson(JsonVariantConst src, ZTDMenu &dst) {
	for (int i = 0; i < BUTTON_COUNT - 1; ++i) {
		dst.buttons[i] = src["buttons"][i];
	}
}

size_t ZTDMenu::getJsonSize() {
	size_t buttons = ZTDButton::getJsonSize() * (BUTTON_COUNT - 1);
	size_t buttonsarray = JSON_ARRAY_SIZE(BUTTON_COUNT - 1);
	return buttons + buttonsarray;
}

//=================================================================
// GeneralConfig Convert
//=================================================================
void convertToJson(const GeneralConfig &src, JsonVariant dst) {
	dst["menuButtonColor"] = src.menuButtonColor;
	dst["functionButtonColor"] = src.functionButtonColor;
	dst["backgroundColor"] = src.backgroundColor;
	dst["latchedColor"] = src.latchedColor;
	dst["sleepEnable"] = src.sleepEnable;
	dst["sleepTimer"] = src.sleepTimer;
	dst["beepEnable"] = src.beepEnable;
	dst["modifier1"] = src.modifier1;
	dst["modifier2"] = src.modifier2;
	dst["modifier3"] = src.modifier3;
	dst["helperDelay"] = src.helperDelay;

	dst["homebutton"] = src.homebutton;
	dst["splashscreen"] = src.splashscreen;
	dst["configuratorLogo"] = src.wifiLogo;
	dst["brightUpLogo"] = src.brightUpLogo;
	dst["brightDownLogo"] = src.brightDownLogo;
	dst["sleepLogo"] = src.sleepLogo;
	dst["infoLogo"] = src.infoLogo;
	for (int i = 0; i < BUTTON_COUNT; ++i) {
		dst["homeScreenLogos"][i] = src.homeScreenLogos[i];
	}
}

void convertFromJson(JsonVariantConst src, GeneralConfig &dst) {
	dst.menuButtonColor = src["menuButtonColor"] | 0x009bf4;
	dst.functionButtonColor = src["functionButtonColor"] | 0x00efcb;
	dst.backgroundColor = src["backgroundColor"] | 0x000000;
	dst.latchedColor = src["latchedColor"] | 0xfe0149;
	dst.sleepEnable = src["sleepEnable"] | true;
	dst.sleepTimer = src["sleepTimer"] | 10;
	dst.beepEnable = src["beepEnable"] | false;
	dst.modifier1 = src["modifier1"] | 130;
	dst.modifier2 = src["modifier2"] | 129;
	dst.modifier3 = src["modifier3"] | 0;
	dst.helperDelay = src["helperDelay"] | 500;

	strcpy(dst.homebutton, src["homebutton"] | "/logos/home.bmp");
	strcpy(dst.splashscreen, src["splashscreen"] | "/logos/freetouchdeck_logo.bmp");
	strcpy(dst.wifiLogo, src["configurator"] | "/logos/wifi.bmp");
	strcpy(dst.brightUpLogo, src["brightUpLogo"] | "/logos/brightnessdown.bmp");
	strcpy(dst.brightDownLogo, src["brightDownLogo"] | "/logos/brightnessup.bmp");
	strcpy(dst.sleepLogo, src["sleepLogo"] | "/logos/sleep.bmp");
	strcpy(dst.infoLogo, src["infoLogo"] | "/logos/info.bmp");


	for (int i = 0; i < BUTTON_COUNT; ++i) {
		if(src["homeScreenLogos"][i])
			strcpy(dst.homeScreenLogos[i], src["homeScreenLogos"][i]);
	}
}

size_t GeneralConfig::getJsonSize() {
	return 7 * JSON_STRING_SIZE(64) + JSON_OBJECT_SIZE(18) + JSON_ARRAY_SIZE(BUTTON_COUNT) + JSON_STRING_SIZE(64) * (BUTTON_COUNT);
}

//=================================================================
// ZTDWifi Convert
//=================================================================
void convertToJson(const ZTDWifi &src, JsonVariant dst) {
	dst["ssid"] = src.ssid;
	dst["password"] = src.password;
	dst["wifimode"] = src.wifimode;
	dst["wifihostname"] = src.hostname;

	dst["attempts"] = src.attempts;
	dst["attemptdelay"] = src.attemptdelay;
}

void convertFromJson(JsonVariantConst src, ZTDWifi &dst) {
	strcpy(dst.ssid, src["ssid"] | "FAILED");
	strcpy(dst.password, src["password"] | "FAILED");
	strcpy(dst.wifimode, src["wifimode"] | "FAILED");
	strcpy(dst.hostname, src["wifihostname"] | "zorntouchdeck");

	dst.attempts = src["attempts"] | 10;

	dst.attemptdelay = src["attemptdelay"] | 500;
}

size_t ZTDWifi::getJsonSize() {
	return JSON_STRING_SIZE(64) + JSON_STRING_SIZE(64) + JSON_STRING_SIZE(9) + JSON_STRING_SIZE(64) + JSON_OBJECT_SIZE(2);
}

} /* namespace ZTD */
