/*
 * Wificonfig.cpp
 *
 *  Created on: Aug 6, 2022
 *      Author: ZornCo
 */

#include "ZTDWifi.h"
#include "Configuration.h"
#include "Screen.h"

namespace ZTD {

bool ZTDWifi::startWifiStation() {
	if(String(Configuration::wifiConfig->ssid) == "") return false;
	Serial.printf("[INFO]: Connecting to %s", Configuration::wifiConfig->ssid);
	if (String(WiFi.SSID()) != String(Configuration::wifiConfig->ssid)) {
		WiFi.mode(WIFI_STA);
		WiFi.begin(Configuration::wifiConfig->ssid,
				Configuration::wifiConfig->password);
		uint8_t attempts = Configuration::wifiConfig->attempts;
		while (WiFi.status() != WL_CONNECTED) {
			if (attempts == 0) {
				WiFi.disconnect();
				Serial.println("");
				return false;

			}
			delay(Configuration::wifiConfig->attemptdelay);
			Serial.print(".");
			attempts--;

		}
	}

	// Delete the task bleKeyboard had create to free memory and to not interfere with AsyncWebServer
	Configuration::getBleKeyboard()->end();

	// Stop BLE from interfering with our WIFI signal
	btStop();
	esp_bt_controller_disable();
	esp_bt_controller_deinit();
	esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

	Serial.println("");
	Serial.println("[INFO]: BLE Stopped");
	Serial.print("[INFO]: Connected! IP address: ");
	Serial.println(WiFi.localIP());

	MDNS.begin(Configuration::wifiConfig->hostname);
	MDNS.addService("http", "tcp", 80);

	// Set pageNum to 7 so no buttons are displayed and touches are ignored
	Configuration::setMenuState(WIFI);

	// Start the webserver
	webserver.begin();
	Serial.println("[INFO]: Webserver started");
	return true;
}

void ZTDWifi::startWifiAP() {

	WiFi.mode(WIFI_AP);
	WiFi.softAP(Configuration::wifiConfig->ssid,
			Configuration::wifiConfig->password);
	Serial.println("");
	Serial.print("[INFO]: Access Point Started! IP address: ");
	Serial.println(WiFi.softAPIP());

	// Delete the task bleKeyboard had create to free memory and to not interfere with AsyncWebServer
	Configuration::getBleKeyboard()->end();

	// Stop BLE from interfering with our WIFI signal
	btStop();
	esp_bt_controller_disable();
	esp_bt_controller_deinit();
	esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

	Serial.println("");
	Serial.println("[INFO]: BLE Stopped");

	MDNS.begin(Configuration::wifiConfig->hostname);
	MDNS.addService("http", "tcp", 80);

	// Set pageNum to 7 so no buttons are displayed and touches are ignored
	Configuration::setMenuState(WIFI);

	// Start the webserver
	webserver.begin();
	Serial.println("[INFO]: Webserver started");
}

void ZTDWifi::startDefaultAP() {

	const char *ssid = "FreeTouchDeck";
	const char *password = "defaultpass";

	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password);
	Serial.print("[INFO]: Access Point Started! IP address: ");
	Serial.println(WiFi.softAPIP());

	// Delete the task bleKeyboard had create to free memory and to not interfere with AsyncWebServer
	Configuration::getBleKeyboard()->end();

	// Stop BLE from interfering with our WIFI signal
	btStop();
	esp_bt_controller_disable();
	esp_bt_controller_deinit();
	esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

	Serial.println("[INFO]: BLE Stopped");

	MDNS.begin("freetouchdeck");
	MDNS.addService("http", "tcp", 80);

	// Set pageNum to 7 so no buttons are displayed and touches are ignored
	Configuration::setMenuState(WIFI);

	// Start the webserver
	webserver.begin();
	Serial.println("[INFO]: Webserver started");

}

void ZTDWifi::configmode() {

	Screen::getTFT()->fillScreen(TFT_BLACK);
	Screen::getTFT()->setCursor(0, 0);
	Screen::getTFT()->setTextFont(2);
	Screen::getTFT()->setTextSize(1);
	Screen::getTFT()->setTextColor(TFT_WHITE, TFT_BLACK);

	Serial.println("[INFO]: Entering Config Mode");
	Screen::getTFT()->println("Connecting to Wifi...");
	Serial.println("[INFO]: Connecting to Wifi...");

	if (String(Configuration::wifiConfig->ssid) == "YOUR_WIFI_SSID"
			|| String(Configuration::wifiConfig->password)
					== "YOUR_WIFI_PASSWORD") // Still default
					{
		Serial.println(
				"[WARNING]: WiFi Config still set to default! Configurator started as AP.");
		Screen::getTFT()->println(
				"WiFi Config still set to default! Starting as AP.");
		startDefaultAP();
		Screen::getTFT()->println(
				"Started as AP because WiFi settings are still set to default.");
		Screen::getTFT()->println(
				"To configure, connect to 'FreeTouchDeck' with password 'defaultpass'");
		Screen::getTFT()->println("Then go to http://freetouchdeck.local");
		Screen::getTFT()->print("The IP is: ");
		Screen::getTFT()->println(WiFi.softAPIP());
		Screen::instance()->drawSingleButton(140, 180, 200, 80,
				Configuration::getGConf()->menuButtonColor, TFT_WHITE,
				"Restart");
		return;
	}

	if (String(Configuration::wifiConfig->ssid) == "FAILED"
			|| String(Configuration::wifiConfig->password) == "FAILED"
			|| String(Configuration::wifiConfig->wifimode) == "FAILED") // The Configuration::wifiConfig->json failed to load
					{
		Serial.println(
				"[WARNING]: WiFi Config Failed to load! Configurator started as AP.");
		Screen::getTFT()->println(
				"WiFi Config Failed to load! Starting as AP.");
		startDefaultAP();
		Screen::getTFT()->println(
				"Started as AP because WiFi settings failed to load.");
		Screen::getTFT()->println(
				"To configure, connect to 'FreeTouchDeck' with password 'defaultpass'");
		Screen::getTFT()->println("Then go to http://freetouchdeck.local");
		Screen::getTFT()->print("The IP is: ");
		Screen::getTFT()->println(WiFi.softAPIP());
		Screen::instance()->drawSingleButton(140, 180, 200, 80,
				Configuration::getGConf()->menuButtonColor, TFT_WHITE,
				"Restart");
		return;
	}
	if (strcmp(Configuration::wifiConfig->wifimode, "WIFI_AP") == 0) {
		Serial.println("[INFO]: Started as AP and in config mode.");
		startWifiAP();
		Screen::getTFT()->println("Started as AP and in config mode.");
		Screen::getTFT()->println("To configure:");
		Screen::getTFT()->println("http://freetouchdeck.local");
		Screen::getTFT()->print("The IP is: ");
		Screen::getTFT()->println(WiFi.softAPIP());
		Screen::instance()->drawSingleButton(140, 180, 200, 80,
				Configuration::getGConf()->menuButtonColor, TFT_WHITE,
				"Restart");
	} else
//if (strcmp(Configuration::wifiConfig->wifimode, "WIFI_STA") == 0)
	{
		if (!startWifiStation()) {
			Serial.println("[WARNING]: Could not connect to AP, so started as AP.");
			startDefaultAP();
			Screen::getTFT()->println("Started as AP because WiFi connection failed.");
			Screen::getTFT()->println("To configure, connect to 'FreeTouchDeck' with password 'defaultpass'");
			Screen::getTFT()->println("Then go to http://freetouchdeck.local");
			Screen::getTFT()->print("The IP is: ");
			Screen::getTFT()->println(WiFi.softAPIP());
			Screen::instance()->drawSingleButton(140, 180, 200, 80,
					Configuration::getGConf()->menuButtonColor, TFT_WHITE,
					"Restart");
		} else {
			Serial.println("[INFO]: Started as STA and in config mode.");
			Screen::getTFT()->println("Started as STA and in config mode.");
			Screen::getTFT()->println("To configure:");
			Screen::getTFT()->println("http://freetouchdeck.local");
			Screen::getTFT()->print("The IP is: ");
			Screen::getTFT()->println(WiFi.localIP());
			Screen::instance()->drawSingleButton(140, 180, 200, 80,
					Configuration::getGConf()->menuButtonColor, TFT_WHITE,
					"Restart");
		}

	}
	Serial.println("[DEBUG]: configmode end");
}

bool ZTDWifi::saveWifiSSID(String ssid) {

	FILESYSTEM.remove("/config/Configuration::wifiConfig->json");
	File file = FILESYSTEM.open("/config/Configuration::wifiConfig->json", "w");

	DynamicJsonDocument doc(384);

	JsonObject wifiConfigobject = doc.to<JsonObject>();

	wifiConfigobject["ssid"] = ssid;
	wifiConfigobject["password"] = Configuration::wifiConfig->password;
	wifiConfigobject["wifimode"] = Configuration::wifiConfig->wifimode;
	wifiConfigobject["wifihostname"] = Configuration::wifiConfig->hostname;
	wifiConfigobject["attempts"] = Configuration::wifiConfig->attempts;
	wifiConfigobject["attemptdelay"] = Configuration::wifiConfig->attemptdelay;

	if (serializeJsonPretty(doc, file) == 0) {
		Serial.println("[WARNING]: Failed to write to file");
		return false;
	}
	file.close();
	return true;
}

bool ZTDWifi::saveWifiPW(String password) {

	FILESYSTEM.remove("/config/Configuration::wifiConfig->json");
	File file = FILESYSTEM.open("/config/Configuration::wifiConfig->json", "w");

	DynamicJsonDocument doc(384);

	JsonObject wifiConfigobject = doc.to<JsonObject>();

	wifiConfigobject["ssid"] = Configuration::wifiConfig->ssid;
	wifiConfigobject["password"] = password;
	wifiConfigobject["wifimode"] = Configuration::wifiConfig->wifimode;
	wifiConfigobject["wifihostname"] = Configuration::wifiConfig->hostname;
	wifiConfigobject["attempts"] = Configuration::wifiConfig->attempts;
	wifiConfigobject["attemptdelay"] = Configuration::wifiConfig->attemptdelay;

	if (serializeJsonPretty(doc, file) == 0) {
		Serial.println("[WARNING]: Failed to write to file");
		return false;
	}
	file.close();
	return true;
}

bool ZTDWifi::saveWifiMode(String wifimode) {

	if (wifimode != "WIFI_STA" && wifimode != "WIFI_AP") {
		Serial.println(
				"[WARNING]: WiFi Mode not supported. Try WIFI_STA or WIFI_AP.");
		return false;
	}

	FILESYSTEM.remove("/config/Configuration::wifiConfig->json");
	File file = FILESYSTEM.open("/config/Configuration::wifiConfig->json", "w");

	DynamicJsonDocument doc(384);

	JsonObject wifiConfigobject = doc.to<JsonObject>();

	wifiConfigobject["ssid"] = Configuration::wifiConfig->ssid;
	wifiConfigobject["password"] = Configuration::wifiConfig->password;
	wifiConfigobject["wifimode"] = wifimode;
	wifiConfigobject["wifihostname"] = Configuration::wifiConfig->hostname;
	wifiConfigobject["attempts"] = Configuration::wifiConfig->attempts;
	wifiConfigobject["attemptdelay"] = Configuration::wifiConfig->attemptdelay;

	if (serializeJsonPretty(doc, file) == 0) {
		Serial.println("[WARNING]: Failed to write to file");
		return false;
	}
	file.close();
	return true;
}

} /* namespace ZTD */
