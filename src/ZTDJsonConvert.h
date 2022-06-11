/*
 * Structs.h
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */

#pragma once
#include "CommonIncludes.h"
#include "Printf.h"


namespace ZTD {

	class Wificonfig {
	public:
		char ssid[64];
		char password[64];
		char wifimode[9];
		char hostname[64];
		uint8_t attempts;
		uint16_t attemptdelay;
		static size_t getJsonSize();
		friend void convertToJson(const Wificonfig &src, JsonVariant dst);
		friend void convertFromJson(JsonVariantConst src, Wificonfig& dst);
	};

} /* namespace ZTD */
