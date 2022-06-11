/*
 * Screen.cpp
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */

#include "Screen.h"

namespace ZTD {

void Screen::drawErrorMessage(String message) {
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(20, 20);
	tft.setTextFont(2);
	tft.setTextSize(1);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.println(message);
}

} /* namespace ZTD */
