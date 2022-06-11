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
}

void convertFromJson(JsonVariantConst src, ZTDButton &dst) {
	for (int i = 0; i < ACTION_COUNT; ++i) {
		dst.actions[i] = src["actions"][i];
	}
	dst.latch = src["latch"] | false;
	strlcpy(dst.latchlogo, src["latchlogo"] | "question.bmp", sizeof(dst.latchlogo));
}

size_t ZTDButton::getJsonSize() {
	return JSON_STRING_SIZE(32) + JSON_ARRAY_SIZE(ACTION_COUNT) + ZTDAction::getJsonSize() * ACTION_COUNT;
}

//=================================================================
// ZTDMenu Convert
//=================================================================
void convertToJson(const ZTDMenu &src, JsonVariant dst) {
	for (int i = 0; i < BUTTON_COUNT - 1; ++i) {
		dst["buttons"][i] = src.buttons[i];
		dst["logos"][i] = src.logos[i];
	}
}

void convertFromJson(JsonVariantConst src, ZTDMenu &dst) {
	for (int i = 0; i < BUTTON_COUNT - 1; ++i) {
		dst.buttons[i] = src["buttons"][i];
		strlcpy(dst.logos[i], src["logos"][i] | "question.bmp", 64);
	}
}

size_t ZTDMenu::getJsonSize() {
	size_t buttons = ZTDButton::getJsonSize() * (BUTTON_COUNT - 1);
	size_t buttonsarray = JSON_ARRAY_SIZE(BUTTON_COUNT - 1);
	size_t logos = JSON_STRING_SIZE(64) * (BUTTON_COUNT - 1);
	size_t logosarray = JSON_ARRAY_SIZE(BUTTON_COUNT - 1);
	return buttons + buttonsarray + logos + logosarray;
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
	dst["configurator"] = src.configurator;

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
	strcpy(dst.configurator, src["configurator"] | "/logos/wifi.bmp");

	for (int i = 0; i < BUTTON_COUNT; ++i) {
		if(src["homeScreenLogos"][i])
			strcpy(dst.homeScreenLogos[i], src["homeScreenLogos"][i]);
	}
}

size_t GeneralConfig::getJsonSize() {
	return 3 * JSON_STRING_SIZE(64) + JSON_OBJECT_SIZE(14) + JSON_ARRAY_SIZE(BUTTON_COUNT) + JSON_STRING_SIZE(64) * (BUTTON_COUNT);
}

size_t Wificonfig::getJsonSize() {
	return JSON_STRING_SIZE(64) + JSON_STRING_SIZE(64) + JSON_STRING_SIZE(9) + JSON_STRING_SIZE(64) + JSON_OBJECT_SIZE(2);
}

} /* namespace ZTD */
