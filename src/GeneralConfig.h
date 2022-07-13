/*
 * GeneralConfig.h
 *
 *  Created on: Jun 8, 2022
 *      Author: ZornCo
 */

#ifndef GENERALCONFIG_H_
#define GENERALCONFIG_H_
#include "CommonIncludes.h"

namespace ZTD {

class GeneralConfig {
public:
	int menuButtonColor			 = 0x04DD;
	int functionButtonColor		 = 0x0778;
	int backgroundColor			 = 0x0000;
	int latchedColor			 = 0xF008;
	bool	 sleepEnable		 = true;
	uint16_t sleepTimer			 = 10;
	bool	 beepEnable			 = false;
	uint8_t  modifier1			 = 130;
	uint8_t  modifier2			 = 129;
	uint8_t  modifier3			 = 0;
	uint16_t helperDelay		 = 500;

	char homebutton[64]		 = "home.bmp";
	char wifiLogo[64]		 = "wifi.bmp";
	char brightDownLogo[64]	 = "brightnessdown.bmp";
	char brightUpLogo[64]	 = "brightnessup.bmp";
	char sleepLogo[64]		 = "sleep.bmp";
	char infoLogo[64]		 = "info.bmp";
	char splashscreen[64]	 = "freetouchdeck_logo.bmp";
	char* settingsLogos[6];
	char homeScreenLogos[BUTTON_COUNT][64] = {
			"music.bmp",
			"obs.bmp",
			"firefox.bmp",
			"mail.bmp",
			"youtube.bmp",
			"settings.bmp"
	};
	GeneralConfig();
	static size_t getJsonSize();
	friend void convertToJson(const GeneralConfig &src, JsonVariant dst);
	friend void convertFromJson(JsonVariantConst src, GeneralConfig& dst);

};

} /* namespace ZTD */

#endif /* GENERALCONFIG_H_ */
