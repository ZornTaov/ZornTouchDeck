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

	Configuration(){}// @suppress("Class members should be properly initialized")

#ifdef USEUSBHID
	static USBHIDKeyboard* bleKeyboard;

#else
	static BleKeyboard* bleKeyboard;
#endif // USEUSBHID

	static ZTDMenu menus[BUTTON_COUNT-1];
	static ZTDButton home[BUTTON_COUNT];
	static ZTDButton settings[6];
	static ZTDButton* homeButton;

	static MenuState menuState;
	static uint8_t menuIndex;

	static GeneralConfig* generalConfig;

	//path to the directory the logo are in ! including leading AND trailing / !
	static constexpr char* logopath = "/logos/";
public:
	Configuration(Configuration const&) = delete;
	Configuration& operator=(Configuration const&) = delete;

	static std::shared_ptr<Configuration> instance()
	{
		static std::shared_ptr<Configuration> s{new Configuration};
		return s;
	}

	void configmode();
	bool loadWifiConfig();
	bool loadConfig(String value);
	bool checkfile(const char *filename);
	static BleKeyboard* getBleKeyboard();
	static ZTDButton* getHomeLogos();
	static ZTDButton* getSettingsLogos();
	static ZTDButton* getHomeButton();
	static ZTDMenu* getMenus();
	static uint8_t getMenuIndex();
	void setMenuIndex(uint8_t menuIndex);
	static MenuState getMenuState();
	void setMenuState(MenuState menuState = HOME);
	static GeneralConfig* getGConf();
	static void setGConf(GeneralConfig *generalConfig);
	static char* getLogoPath();

	static Preferences* savedStates;

	static Wificonfig* wifiConfig;

	char jsonfilefail[32] = "";

	unsigned long previousMillis = 0;
	unsigned long Interval = 0;
	bool displayinginfo;

	// templogopath is used to hold the complete path of an image. It is empty for now.
	char templogopath[64] = "";

};

} /* namespace ZTD */
