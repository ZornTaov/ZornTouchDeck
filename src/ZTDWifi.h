/*
 * Wificonfig.h
 *
 *  Created on: Aug 6, 2022
 *      Author: ZornCo
 */

#ifndef SRC_ZTDWIFI_H_
#define SRC_ZTDWIFI_H_
#include "CommonIncludes.h"
#include "Printf.h"

namespace ZTD {

class ZTDWifi {
public:
	char ssid[64] = "";
	char password[64] = "";
	char wifimode[9] = "";
	char hostname[64] = "zorntouchdeck";
	uint8_t attempts = 10;
	uint16_t attemptdelay = 500;

	static size_t getJsonSize();
	friend void convertToJson(const ZTDWifi &src, JsonVariant dst);
	friend void convertFromJson(JsonVariantConst src, ZTDWifi& dst);

	AsyncWebServer webserver = AsyncWebServer(80);
	bool startWifiStation();

	// Start as WiFi AP

	void startWifiAP();

	// Start the default AP

	void startDefaultAP();

	/**
	* @brief This function stops Bluetooth and connects to the given
	         WiFi network. It the starts mDNS and starts the Async
	         Webserver.
	*
	* @param none
	*
	* @return none
	*
	* @note none
	*/
	void configmode();


	/**
	* @brief This function allows for saving (updating) the WiFi SSID
	*
	* @param String ssid
	*
	* @return boolean True if succeeded. False otherwise.
	*
	* @note Returns true if successful. To enable the new set SSID, you must reload the the
	         configuration using loadMainConfig()
	*/
	bool saveWifiSSID(String ssid);

	/**
	* @brief This function allows for saving (updating) the WiFi Password
	*
	* @param String password
	*
	* @return boolean True if succeeded. False otherwise.
	*
	* @note Returns true if successful. To enable the new set password, you must reload the the
	         configuration using loadMainConfig()
	*/
	bool saveWifiPW(String password);

	/**
	* @brief This function allows for saving (updating) the WiFi Mode
	*
	* @param String wifimode "WIFI_STA" of "WIFI_AP"
	*
	* @return boolean True if succeeded. False otherwise.
	*
	* @note Returns true if successful. To enable the new set WiFi Mode, you must reload the the
	         configuration using loadMainConfig()
	*/
	bool saveWifiMode(String wifimode);
};

} /* namespace ZTD */

#endif /* SRC_ZTDWIFI_H_ */
