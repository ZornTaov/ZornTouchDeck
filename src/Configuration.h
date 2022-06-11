/*
 * Configuration.h
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */
#pragma once

#include <Preferences.h>
#include "CommonIncludes.h"
#include "Print.h"
#include "ZTDJsonConvert.h"
#include "ZTDMenu.h"
#include "GeneralConfig.h"
namespace ZTD {

class Configuration {

	Configuration(){ // @suppress("Class members should be properly initialized")

	}

public:
	Configuration(Configuration const&) = delete;
	Configuration& operator=(Configuration const&) = delete;

	static std::shared_ptr<Configuration> instance()
	{
		static std::shared_ptr<Configuration> s{new Configuration};
		return s;
	}

	static BleKeyboard*& getBleKeyboard() {
		return bleKeyboard;
	}

	void configmode();
	bool loadMainConfig();
	bool loadConfig(String value);
	bool checkfile(const char *filename);




#ifdef USEUSBHID
	static USBHIDKeyboard* bleKeyboard;

#else
	static BleKeyboard* bleKeyboard;
#endif // USEUSBHID
	Preferences savedStates;

	GeneralConfig generalConfig;

	Wificonfig wifiConfig;

	ZTDMenu menus[BUTTON_COUNT-1];

	// placeholder for the pagenumber we are on (0 indicates home)
	int pageNum = 0;

	unsigned long previousMillis = 0;
	unsigned long Interval = 0;
	bool displayinginfo;

	//path to the directory the logo are in ! including leading AND trailing / !
	char logopath[64] = "/logos/";

	// templogopath is used to hold the complete path of an image. It is empty for now.
	char templogopath[64] = "";

};

} /* namespace ZTD */
