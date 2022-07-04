/*
 * Screen.cpp
 *
 *  Created on: Jun 5, 2022
 *      Author: ZornCo
 */

#include "Screen.h"
#include "Configuration.h"
#include <cwalk.h>

namespace ZTD {

void Screen::drawErrorMessage(String message) {
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(20, 20);
	tft.setTextFont(2);
	tft.setTextSize(1);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.println(message);
}

TFT_eSPI* Screen::getTFT() {
	return &Screen::instance()->tft;
}
void Screen::drawlatched(int b, int col, int row) {
	int offset;
	if (SCREEN_WIDTH < 480) {
		offset = 2;
	} else {
		offset = 12;
	}
	tft.fillRoundRect((KEY_X - 37 + col * (KEY_W + KEY_SPACING_X)) - offset,
			(KEY_Y - 37 + row * (KEY_H + KEY_SPACING_Y)) - offset, 18, 18, 4,
			Configuration::instance()->getGConf()->latchedColor);
}
void Screen::drawlogo(int logonumber, int col, int row, bool transparent,
		bool latch) {
	int16_t posX = KEY_X - 36 + col * (KEY_W + KEY_SPACING_X);
	int16_t posY = KEY_Y - 36 + row * (KEY_H + KEY_SPACING_Y);

	char bufferPath[FILENAME_MAX];
	char* subPath;
	cwk_path_set_style(CWK_STYLE_UNIX);

	switch (Configuration::getMenuState()) {
	case HOME: {
		subPath = Configuration::getHomeLogos()[logonumber].logo;
		cwk_path_join(Configuration::getLogoPath(), subPath, bufferPath, sizeof(bufferPath));
		//Draw Home screen logo's
		drawBmp(bufferPath, posX, posY, transparent);
		break;
	}
	case ACTION: {

		uint8_t currentMenuIndex = Configuration::getMenuIndex();
		ZTDMenu *currentMenu = &Configuration::getMenus()[currentMenuIndex];
		bool isHome = logonumber == BUTTON_COUNT - 1;
		ZTDButton *button = isHome ? Configuration::getHomeButton() : &currentMenu->buttons[logonumber];
		if (logonumber == 0) {
			if (latch == true) {
				if (strcmp(button->latchlogo, "") != 0) {
					subPath = button->latchlogo;
					cwk_path_join(Configuration::getLogoPath(), subPath, bufferPath, sizeof(bufferPath));
					drawBmp(bufferPath, posX, posY, transparent);
				} else {
					subPath = button->logo;
					cwk_path_join(Configuration::getLogoPath(), subPath, bufferPath, sizeof(bufferPath));
					drawBmp(bufferPath, posX, posY, transparent);
					drawlatched(logonumber, col, row);
				}
			} else {
				subPath = button->logo;
				cwk_path_join(Configuration::getLogoPath(), subPath, bufferPath, sizeof(bufferPath));
				drawBmp(bufferPath, posX, posY, transparent);
			}
		} else if (logonumber == 5) {
			subPath = Configuration::getHomeButton()->logo;
			cwk_path_join(Configuration::getLogoPath(), subPath, bufferPath, sizeof(bufferPath));
			drawBmp(bufferPath, posX, posY, transparent);
		}
		break;
	}
	case SETTINGS: {
		posX = S_KEY_X - 36 + col * (S_KEY_W + S_KEY_SPACING_X);
		posY = S_KEY_Y - 36 + row * (S_KEY_H + S_KEY_SPACING_Y);
		// pageNum6 contains settings logos
		if (logonumber == 5) {
			drawBmp(Configuration::getHomeButton()->logo,
					posX, posY, transparent);
		} else {
			drawBmp(Configuration::getSettingsLogos()[logonumber].logo,
					posX, posY, true);
			if (latch) {
				drawlatched(logonumber, col, row);
			}
		}
		break;
	}
	}
}

void Screen::drawHome() {
	// Draw the home screen button outlines and fill them with colours
	for (uint8_t row = 0; row < BTN_ROWS; row++) {
		for (uint8_t col = 0; col < BTN_COLS; col++) {
			uint8_t b = col + row * BTN_COLS;
			uint16_t buttonBG;
			bool drawTransparent;
			uint16_t imageBGColor = getImageBG(b);
			if (imageBGColor > 0) {
				buttonBG = imageBGColor;
				drawTransparent = false;
			} else {
				buttonBG =
						Configuration::instance()->getGConf()->menuButtonColor;
				drawTransparent = true;
			}
			drawButton(col, row, buttonBG, true, drawTransparent, false);
		}
	}
}

void Screen::drawErrorScreen() {
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0);
	tft.setTextFont(2);
	tft.setTextSize(1);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.printf("  %s failed to load and might be corrupted.\n",
			Configuration::instance()->jsonfilefail);
	tft.println(
			"  You can reset that specific file to default by opening the serial monitor");
	tft.printf("  and typing \"reset %s\"\n",
			Configuration::instance()->jsonfilefail);
	tft.println("  If you don't do this, the configurator will fail to load.");
}

void Screen::drawKeypad() {

	switch (Configuration::getMenuState()) {
	case HOME:
		// Draw the home screen button outlines and fill them with colours
		drawHome();
		return;
	case SETTINGS:
		//drawSettings();
		return;
	case ERROR:
		// Pagenum 10 means that a JSON config failed to load completely.
		drawErrorScreen();
		return;
	default:
		break;
	}
	// isn't a special menu, draw current menu buttons
	uint8_t currentMenuIndex = Configuration::getMenuIndex();
	ZTDMenu *currentMenu = &Configuration::getMenus()[currentMenuIndex];
	// Draw the button outlines and fill them with colours
	for (uint8_t row = 0; row < BTN_ROWS; row++) {
		for (uint8_t col = 0; col < BTN_COLS; col++) {

			uint8_t b = col + row * BTN_COLS;
			bool isHome = b == BUTTON_COUNT - 1;
			ZTDButton *button =
					isHome ?
							Configuration::getHomeButton() :
							&currentMenu->buttons[b];
			// Check if "home.bmp" is a transparent one

			uint16_t buttonBG;
			bool drawTransparent;
			uint16_t imageBGColor;

			bool hasLatch = !isHome && button->latch && b < BUTTON_COUNT - 1;
			if (hasLatch) {
				imageBGColor = getLatchImageBG(b);
			} else {
				imageBGColor = getImageBG(b);
			}

			if (imageBGColor > 0) {
				buttonBG = imageBGColor;
				drawTransparent = false;
			} else {
				//TODO implement color per button?
				buttonBG = isHome ?
								Configuration::instance()->getGConf()->menuButtonColor :
								Configuration::instance()->getGConf()->functionButtonColor;
				drawTransparent = true;
			}

			drawButton(col, row, buttonBG, true, drawTransparent, hasLatch);
		}
	}
}

void Screen::drawButton(uint8_t col, uint8_t row, uint16_t buttonBG, bool drawLogo, bool drawTransparent, bool hasLatch) {
	uint8_t b = col + row * BTN_COLS;
	drawBasicButton(col, row, buttonBG);
	if(drawLogo) drawlogo(b, col, row, drawTransparent, hasLatch);
}

void Screen::drawBasicButton(uint8_t col, uint8_t row, uint16_t buttonBG) {
	uint8_t b = col + row * BTN_COLS;
	tft.setFreeFont(LABEL_FONT);
	keys[b].initButton(&tft,
		KEY_X + col * (KEY_W + KEY_SPACING_X),
		KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
		KEY_W, KEY_H, TFT_WHITE, buttonBG, TFT_WHITE, "",
		KEY_TEXTSIZE);
	keys[b].drawButton();
}

/**
 * @brief This function reads chuncks of 2 bytes of data from a
 file and returns the data.
 *
 * @param &f
 *
 * @return uint16_t
 *
 * @note litte-endian
 */
uint16_t Screen::read16(fs::File &f) {
	uint16_t result;
	((uint8_t*) &result)[0] = f.read(); // LSB
	((uint8_t*) &result)[1] = f.read(); // MSB
	return result;
}

/**
 * @brief This function reads chuncks of 4 bytes of data from a
 file and returns the data.
 *
 * @param &f
 *
 * @return uint32_t
 *
 * @note litte-endian
 */
uint32_t Screen::read32(fs::File &f) {
	uint32_t result;
	((uint8_t*) &result)[0] = f.read(); // LSB
	((uint8_t*) &result)[1] = f.read();
	((uint8_t*) &result)[2] = f.read();
	((uint8_t*) &result)[3] = f.read(); // MSB
	return result;
}

/**
 * @brief This functions accepts a HTML including the # colour code
 (eg. #FF00FF)  and returns it in RGB888 format.
 *
 * @param *html char (including #)
 *
 * @return unsigned long
 *
 * @note none
 */
unsigned long Screen::convertHTMLtoRGB888(char *html) {
	char *hex = html + 1; // remove the #
	unsigned long rgb = strtoul(hex, NULL, 16);
	return rgb;
}

/**
 * @brief This function converts RGB888 to RGB565.
 *
 * @param rgb unsigned long
 *
 * @return unsigned int
 *
 * @note none
 */
unsigned int Screen::convertRGB888ToRGB565(unsigned long rgb) {
	return (((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5)
			| ((rgb & 0xf8) >> 3));
}

void Screen::drawBmp(char *filename, int16_t x, int16_t y,
		bool transparent = false) {

	if ((x >= tft.width()) || (y >= tft.height()))
		return;

	fs::File bmpFS;

	bmpFS = FILESYSTEM.open(filename, "r");

	if (!bmpFS) {

		Serial.print("File not found:");
		Serial.println(filename);
		return;
	}

	uint32_t seekOffset;
	uint16_t w, h, row;
	uint8_t r, g, b;

	if (read16(bmpFS) == 0x4D42) {
		read32(bmpFS);
		read32(bmpFS);
		seekOffset = read32(bmpFS);
		read32(bmpFS);
		w = read32(bmpFS);
		h = read32(bmpFS);

		if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24)
				&& (read32(bmpFS) == 0)) {
			y += h - 1;

			bool oldSwapBytes = tft.getSwapBytes();
			tft.setSwapBytes(true);
			bmpFS.seek(seekOffset);

			uint16_t padding = (4 - ((w * 3) & 3)) & 3;
			uint8_t lineBuffer[w * 3 + padding];

			for (row = 0; row < h; row++) {

				bmpFS.read(lineBuffer, sizeof(lineBuffer));
				uint8_t *bptr = lineBuffer;
				uint16_t *tptr = (uint16_t*) lineBuffer;
				// Convert 24 to 16 bit colours
				for (uint16_t col = 0; col < w; col++) {
					b = *bptr++;
					g = *bptr++;
					r = *bptr++;
					*tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
				}

				// Push the pixel row to screen, pushImage will crop the line if needed
				// y is decremented as the BMP image is drawn bottom up
				if (transparent) {
					tft.pushImage(x, y--, w, 1, (uint16_t*) lineBuffer,
					TFT_BLACK);
				} else {
					tft.pushImage(x, y--, w, 1, (uint16_t*) lineBuffer);
				}
			}
			tft.setSwapBytes(oldSwapBytes);
		} else
			Serial.println("[WARNING]: BMP format not recognized.");
	}
	bmpFS.close();
}

/**
 * @brief This function reads a number of bytes from the given
 file at the given position.
 *
 * @param *p_file File
 * @param position int
 * @param nBytes byte
 *
 * @return int32_t
 *
 * @note none
 */
int32_t Screen::readNbytesInt(File *p_file, int position, byte nBytes) {
	if (nBytes > 4)
		return 0;

	p_file->seek(position);

	int32_t weight = 1;
	int32_t result = 0;
	for (; nBytes; nBytes--) {
		result += weight * p_file->read();
		weight <<= 8;
	}
	return result;
}

/**
 * @brief This function reads the RGB565 colour of the first pixel for a
 given the logo number. The pagenumber is global.
 *
 * @param *filename const char
 *
 * @return uint16_t
 *
 * @note Uses readNbytesInt
 */
uint16_t Screen::getBMPColor( char *filename) {

	// Open File
	File bmpImage;
	bmpImage = FILESYSTEM.open(filename, FILE_READ);


	if (!bmpImage) {

		Serial.print("File not found:");
		Serial.println(filename);
		return 0x0000;
	}
	int32_t dataStartingOffset = readNbytesInt(&bmpImage, 0x0A, 4);
	int16_t pixelsize = readNbytesInt(&bmpImage, 0x1C, 2);

	if (pixelsize != 24) {
		Serial.println("[WARNING]: getBMPColor: Image is not 24 bpp");
		return 0x0000;
	}

	bmpImage.seek(dataStartingOffset); //skip bitmap header

	byte R, G, B;

	B = bmpImage.read();
	G = bmpImage.read();
	R = bmpImage.read();

	bmpImage.close();

	return tft.color565(R, G, B);
}

/**
 * @brief This function returns the RGB565 colour of the first pixel for a
 given the logo number. The pagenumber is global.
 *
 * @param logonumber int
 *
 * @return uint16_t
 *
 * @note Uses getBMPColor to read the actual image data.
 */
uint16_t Screen::getImageBG(int logonumber) {
	char bufferPath[FILENAME_MAX];
	char* subPath;
	cwk_path_set_style(CWK_STYLE_UNIX);

	switch (Configuration::getMenuState()) {
	case HOME:
		if (logonumber < BUTTON_COUNT) {
			subPath = Configuration::getHomeLogos()[logonumber].logo;
		} else {
			return 0x0000;
		}
		break;
	case SETTINGS:
		if (logonumber < 5) {
			subPath = Configuration::getHomeLogos()[logonumber].logo;
		} else if (logonumber == 5) {
			subPath = Configuration::getHomeButton()->logo;
		} else {
			return 0x0000;
		}
		break;
	case ACTION:
		if (logonumber < BUTTON_COUNT - 1) {
			uint8_t index = Configuration::getMenuIndex();
			subPath = Configuration::getMenus()[index].buttons[logonumber].logo;
		} else if (logonumber == BUTTON_COUNT - 1) {
			subPath = Configuration::getHomeButton()->logo;
		} else {
			return 0x0000;
		}
		break;
	default:
		return 0x0000;
	}
	cwk_path_join(Configuration::getLogoPath(), subPath, bufferPath, sizeof(bufferPath));
	return getBMPColor(bufferPath);
}

/**
 * @brief This function returns the RGB565 colour of the first pixel of the image which
 *          is being latched to for a given the logo number. The pagenumber is global.
 *
 * @param logonumber int
 *
 * @return uint16_t
 *
 * @note Uses getBMPColor to read the actual image data.
 */
uint16_t Screen::getLatchImageBG(int logonumber) {
	switch (Configuration::getMenuState()) {
	case ACTION:
		if (logonumber < BUTTON_COUNT - 1) {
			uint8_t index = Configuration::getMenuIndex();
			if (strcmp(
					Configuration::getMenus()[index].buttons[logonumber].latchlogo,
					"/logos/") == 0) {
				return getBMPColor(
						Configuration::getMenus()[index].buttons[logonumber].logo);
			}
			return getBMPColor(
					Configuration::getMenus()[index].buttons[logonumber].latchlogo);
		} else {
			return 0x0000;
		}
		break;
	case SETTINGS:
		return 0x0000;
		break;
	default:
		return 0x0000;
		break;
	}
}

/**
 * @brief This function allow for drawing a single button which is not part of the
 button class. It should be manually checked for touched.
 *
 * @param int32_t x
 int32_t y
 int32_t w
 int32_t h
 int32_t r
 uint32_t color
 uint32_t outline
 char *label
 *
 * @return none
 *
 * @note Use the X and Y coordinates and check in the loop if it was pressed.
 */

void Screen::drawSingleButton(int32_t x, int32_t y, int32_t w, int32_t h,
		uint32_t color, uint32_t outline, String label) {

	//Draw the button
	uint8_t r = min(w, h) / 4; // Corner radius
	tft.fillRoundRect(x, y, w, h, r, color);
	tft.drawRoundRect(x, y, w, h, r, outline);

	//Print the label
	tft.setTextColor(TFT_WHITE, color);
	tft.setTextSize(2);
	uint8_t tempdatum = tft.getTextDatum();
	tft.setTextDatum(MC_DATUM);
	uint16_t tempPadding = tft.getTextPadding();
	tft.setTextPadding(0);

	tft.drawString(label, x + (w / 2), y + (h / 2));
	tft.setTextDatum(tempdatum);
	tft.setTextPadding(tempPadding);

}

TFT_eSPI_Button* Screen::getKeys() {
	return keys;
}

TFT_eSPI_Button* Screen::getKey(uint8_t index) {
	return &keys[index];
}

} /* namespace ZTD */
