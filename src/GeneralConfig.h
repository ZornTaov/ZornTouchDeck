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
	int menuButtonColor	 = 0x009bf4;
	int functionButtonColor = 0x00efcb;
	int backgroundColor	 = 0x000000;
	int latchedColor		 = 0xfe0149;
	bool	 sleepEnable		 = true;
	uint16_t sleepTimer			 = 10;
	bool	 beepEnable			 = false;
	uint8_t  modifier1			 = 130;
	uint8_t  modifier2			 = 129;
	uint8_t  modifier3			 = 0;
	uint16_t helperDelay		 = 500;

	char 	 homebutton[64]		 = "/logos/home.bmp";
	char 	 configurator[64]	 = "/logos/wifi.bmp";
	char 	 splashscreen[64]	 = "/logos/freetouchdeck_logo.bmp";

	char homeScreenLogos[BUTTON_COUNT][64] = {
			"music.bmp",
			"obs.bmp",
			"firefox.bmp",
			"mail.bmp",
			"youtube.bmp",
			"settings.bmp"
	};

	static size_t getJsonSize();
	friend void convertToJson(const GeneralConfig &src, JsonVariant dst);
	friend void convertFromJson(JsonVariantConst src, GeneralConfig& dst);

};

} /* namespace ZTD */

#endif /* GENERALCONFIG_H_ */
