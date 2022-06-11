/*
 * Screen.h
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */

#pragma once
#include "CommonIncludes.h"
#include "ZTDJsonConvert.h"

namespace ZTD {

class Screen {
	Screen() {}

public:
	Screen(Screen const&) = delete;
	Screen& operator=(Screen const&) = delete;

	static std::shared_ptr<Screen> instance()
	{
		static std::shared_ptr<Screen> s{new Screen};
		return s;
	}
	TFT_eSPI tft = TFT_eSPI(); // @suppress("Abstract class cannot be instantiated")

	// Initial LED brightness
	int ledBrightness = 255;
	// Invoke the TFT_eSPI button class and create all the button objects
	TFT_eSPI_Button keys[BUTTON_COUNT]; // @suppress("Abstract class cannot be instantiated")

	void drawErrorMessage(String message);
};

} /* namespace ZTD */
