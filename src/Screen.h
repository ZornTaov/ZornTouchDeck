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

	void drawHome();
	void drawErrorScreen();

	TFT_eSPI tft = TFT_eSPI(); // @suppress("Abstract class cannot be instantiated")
	// Invoke the TFT_eSPI button class and create all the button objects
	TFT_eSPI_Button keys[BUTTON_COUNT]; // @suppress("Abstract class cannot be instantiated")

public:
	Screen(Screen const&) = delete;
	Screen& operator=(Screen const&) = delete;

	static std::shared_ptr<Screen> instance()
	{
		static std::shared_ptr<Screen> s{new Screen};
		return s;
	}

	static TFT_eSPI* getTFT();


	// Initial LED brightness
	int ledBrightness = 255;

	void drawErrorMessage(String message);
	void drawlatched(int b, int col, int row);
	void drawlogo(int logonumber, int col, int row, bool transparent, bool latch);
	void drawKeypad();
	uint16_t read16(fs::File &f);
	uint32_t read32(fs::File &f);
	unsigned long convertHTMLtoRGB888(char *html);
	unsigned int convertRGB888ToRGB565(unsigned long rgb);
	void drawBmp(char *filename, int16_t x, int16_t y, bool transparent);
	int32_t readNbytesInt(File *p_file, int position, byte nBytes);
	uint16_t getBMPColor(char *filename);
	uint16_t getImageBG(int logonumber);
	uint16_t getLatchImageBG(int logonumber);
	void drawSingleButton(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color, uint32_t outline, String label);

	TFT_eSPI_Button* getKeys();
	TFT_eSPI_Button* getKey(uint8_t index);
};

} /* namespace ZTD */
