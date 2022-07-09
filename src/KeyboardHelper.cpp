/*
 * KeyboardHelper.cpp
 *
 *  Created on: Jun 6, 2022
 *      Author: ZornCo
 */

#include "KeyboardHelper.h"
#include "Configuration.h"
#include "Screen.h"
#include "Action.h"

namespace ZTD {

void KeyboardHelper::pressKey(uint8_t key) {
	if (key != 0) {
		Configuration::getBleKeyboard()->press(key);
	}
}
void KeyboardHelper::writeKey(uint8_t key) {
	if (key != 0) {
		Configuration::getBleKeyboard()->write(key);
	}
}

void KeyboardHelper::sendNavigationKey(int value) {
	switch (value) {
	case 1:
		writeKey(KEY_UP_ARROW);
		break;
	case 2:
		writeKey(KEY_DOWN_ARROW);
		break;
	case 3:
		writeKey(KEY_LEFT_ARROW);
		break;
	case 4:
		writeKey(KEY_RIGHT_ARROW);
		break;
	case 5:
		writeKey(KEY_BACKSPACE);
		break;
	case 6:
		writeKey(KEY_TAB);
		break;
	case 7:
		writeKey(KEY_RETURN);
		break;
	case 8:
		writeKey(KEY_PAGE_UP);
		break;
	case 9:
		writeKey(KEY_PAGE_DOWN);
		break;
	case 10:
		writeKey(KEY_DELETE);
		break;
	case 11:
		writeKey(KEY_PRTSC);
		break;
	case 12:
		writeKey(KEY_ESC);
		break;
	case 13:
		writeKey(KEY_HOME);
		break;
	case 14:
		writeKey(KEY_END);
		break;
	default:
		//if nothing matches do nothing
		break;
	}
}

void KeyboardHelper::sendMediaKey(int value) {
	switch (value) {
	case 1:
		Configuration::getBleKeyboard()->write(KEY_MEDIA_MUTE);
		break;
	case 2:
		Configuration::getBleKeyboard()->write(KEY_MEDIA_VOLUME_DOWN);
		break;
	case 3:
		Configuration::getBleKeyboard()->write(KEY_MEDIA_VOLUME_UP);
		break;
	case 4:
		Configuration::getBleKeyboard()->write(KEY_MEDIA_PLAY_PAUSE);
		break;
	case 5:
		Configuration::getBleKeyboard()->write(KEY_MEDIA_STOP);
		break;
	case 6:
		Configuration::getBleKeyboard()->write(KEY_MEDIA_NEXT_TRACK);
		break;
	case 7:
		Configuration::getBleKeyboard()->write(KEY_MEDIA_PREVIOUS_TRACK);
		break;
	default:
		//if nothing matches do nothing
		break;
	}
}

void KeyboardHelper::sendOptionKey(int value) {
	switch (value) {
	case 1:
		pressKey(KEY_LEFT_CTRL);
		break;
	case 2:
		pressKey(KEY_LEFT_SHIFT);
		break;
	case 3:
		pressKey(KEY_LEFT_ALT);
		break;
	case 4:
		pressKey(KEY_LEFT_GUI);
		break;
	case 5:
		pressKey(KEY_RIGHT_CTRL);
		break;
	case 6:
		pressKey(KEY_RIGHT_SHIFT);
		break;
	case 7:
		pressKey(KEY_RIGHT_ALT);
		break;
	case 8:
		pressKey(KEY_RIGHT_GUI);
		break;
	case 9:
		Configuration::getBleKeyboard()->releaseAll();
		break;
	default:
		//if nothing matches do nothing
		break;
	}
}

void KeyboardHelper::sendFnKey(int value) {
	switch (value) {
	case 1:
		pressKey(KEY_F1);
		break;
	case 2:
		pressKey(KEY_F2);
		break;
	case 3:
		pressKey(KEY_F3);
		break;
	case 4:
		pressKey(KEY_F4);
		break;
	case 5:
		pressKey(KEY_F5);
		break;
	case 6:
		pressKey(KEY_F6);
		break;
	case 7:
		pressKey(KEY_F7);
		break;
	case 8:
		pressKey(KEY_F8);
		break;
	case 9:
		pressKey(KEY_F9);
		break;
	case 10:
		pressKey(KEY_F10);
		break;
	case 11:
		pressKey(KEY_F11);
		break;
	case 12:
		pressKey(KEY_F12);
		break;
	case 13:
		pressKey(KEY_F13);
		break;
	case 14:
		pressKey(KEY_F14);
		break;
	case 15:
		pressKey(KEY_F15);
		break;
	case 16:
		pressKey(KEY_F16);
		break;
	case 17:
		pressKey(KEY_F17);
		break;
	case 18:
		pressKey(KEY_F18);
		break;
	case 19:
		pressKey(KEY_F19);
		break;
	case 20:
		pressKey(KEY_F20);
		break;
	case 21:
		pressKey(KEY_F21);
		break;
	case 22:
		pressKey(KEY_F22);
		break;
	case 23:
		pressKey(KEY_F23);
		break;
	case 24:
		pressKey(KEY_F24);
		break;
	default:
		//if nothing matches do nothing
		break;
	}
}

void KeyboardHelper::sendOptionCombo(int value) {
	switch (value) {
	case 1:
		pressKey(KEY_LEFT_CTRL);
		pressKey(KEY_LEFT_SHIFT);
		break;
	case 2:
		pressKey(KEY_LEFT_ALT);
		pressKey(KEY_LEFT_SHIFT);
		break;
	case 3:
		pressKey(KEY_LEFT_GUI);
		pressKey(KEY_LEFT_SHIFT);
		break;
	case 4:
		pressKey(KEY_LEFT_CTRL);
		pressKey(KEY_LEFT_GUI);
		break;
	case 5:
		pressKey(KEY_LEFT_ALT);
		pressKey(KEY_LEFT_GUI);
		break;
	case 6:
		pressKey(KEY_LEFT_CTRL);
		pressKey(KEY_LEFT_ALT);
		break;
	case 7:
		pressKey(KEY_LEFT_CTRL);
		pressKey(KEY_LEFT_ALT);
		pressKey(KEY_LEFT_GUI);
		break;
	case 8:
		pressKey(KEY_RIGHT_CTRL);
		pressKey(KEY_RIGHT_SHIFT);
		break;
	case 9:
		pressKey(KEY_RIGHT_ALT);
		pressKey(KEY_RIGHT_SHIFT);
		break;
	case 10:
		pressKey(KEY_RIGHT_GUI);
		pressKey(KEY_RIGHT_SHIFT);
		break;
	case 11:
		pressKey(KEY_RIGHT_CTRL);
		pressKey(KEY_RIGHT_GUI);
		break;
	case 12:
		pressKey(KEY_RIGHT_ALT);
		pressKey(KEY_RIGHT_GUI);
		break;
	case 13:
		pressKey(KEY_RIGHT_CTRL);
		pressKey(KEY_RIGHT_ALT);
		break;
	case 14:
		pressKey(KEY_RIGHT_CTRL);
		pressKey(KEY_RIGHT_ALT);
		pressKey(KEY_RIGHT_GUI);
		break;
	}
}

void KeyboardHelper::specialFunction(int value) {
	switch (value) {
	case 1: // Enter config mode
		Configuration::instance()->configmode();
		break;
	case 2: // Display Brightness Down
		if (Screen::instance()->ledBrightness > 25) {
			Screen::instance()->ledBrightness = Screen::instance()->ledBrightness - 25;
			ledcWrite(BL_CHANNEL, Screen::instance()->ledBrightness);
			Configuration::instance()->savedStates->putInt("ledBrightness", Screen::instance()->ledBrightness);
		}
		break;
	case 3: // Display Brightness Up
		if (Screen::instance()->ledBrightness < 230) {
			Screen::instance()->ledBrightness = Screen::instance()->ledBrightness + 25;
			ledcWrite(BL_CHANNEL, Screen::instance()->ledBrightness);
			Configuration::instance()->savedStates->putInt("ledBrightness", Screen::instance()->ledBrightness);
		}
		break;
#ifdef touchInterruptPin
	case 4: // Sleep Enabled
		if (Configuration::instance()->getGConf()->sleepEnable) {
			Configuration::instance()->getGConf()->sleepEnable = false;
			Serial.println("[INFO]: Sleep disabled.");
		} else {
			Configuration::instance()->getGConf()->sleepEnable = true;
			Configuration::instance()->Interval = Configuration::instance()->getGConf()->sleepTimer * 60000;
			Serial.println("[INFO]: Sleep enabled.");
			Serial.print("[INFO]: Timer set to: ");
			Serial.println(Configuration::instance()->getGConf()->sleepTimer);
		}
		break;
#endif
	}
}

void KeyboardHelper::sendNumpad(int value) {
	switch (value) {
	case 0:
		writeKey(KEY_NUM_0);
		break;
	case 1:
		writeKey(KEY_NUM_1);
		break;
	case 2:
		writeKey(KEY_NUM_2);
		break;
	case 3:
		writeKey(KEY_NUM_3);
		break;
	case 4:
		writeKey(KEY_NUM_4);
		break;
	case 5:
		writeKey(KEY_NUM_5);
		break;
	case 6:
		writeKey(KEY_NUM_6);
		break;
	case 7:
		writeKey(KEY_NUM_7);
		break;
	case 8:
		writeKey(KEY_NUM_8);
		break;
	case 9:
		writeKey(KEY_NUM_9);
		break;
	case 10:
		writeKey(KEY_NUM_SLASH);
		break;
	case 11:
		writeKey(KEY_NUM_ASTERISK);
		break;
	case 12:
		writeKey(KEY_NUM_MINUS);
		break;
	case 13:
		writeKey(KEY_NUM_PLUS);
		break;
	case 14:
		writeKey(KEY_NUM_ENTER);
		break;
	case 15:
		writeKey(KEY_NUM_PERIOD);
		break;
	}
}

void KeyboardHelper::sendUserAction(int value) {
	switch (value) {
	case 1:
		userAction1();
		break;
	case 2:
		userAction2();
		break;
	case 3:
		userAction3();
		break;
	case 4:
		userAction4();
		break;
	case 5:
		userAction5();
		break;
	case 6:
		userAction6();
		break;
	case 7:
		userAction7();
		break;
	}
}

void KeyboardHelper::bleKeyboardAction(int action, int value, char *symbol) {

	Serial.println("[INFO]: BLE Keyboard action received");
	Serialprintln("A:%d, V:%d, S:%s", action, value, symbol);

	switch (action) {
	case 0:
		// No Action
		break;
	case 1: // Delay
		delay(value);
		break;
	case 2: // Send TAB ARROW etc
		sendNavigationKey(value);
		break;
	case 3: // Send Media Key
#ifndef USEUSBHID
		sendMediaKey(value);
#endif //if defined(USEUSBHID)
		break;
	case 4: // Send Character
		Configuration::getBleKeyboard()->print(symbol);
		break;
	case 5: // Option Keys
		sendOptionKey(value);
		break;
	case 6: // Function Keys
		sendFnKey(value);
		break;
	case 7: // Send Number
		Configuration::getBleKeyboard()->print(value);
		break;
	case 8: // Send Special Character
		Configuration::getBleKeyboard()->print(symbol);
		break;
	case 9: // Combos
		sendOptionCombo(value);
		break;
	case 10: // Helpers
		pressKey(Configuration::instance()->getGConf()->modifier1);
		pressKey(Configuration::instance()->getGConf()->modifier2);
		pressKey(Configuration::instance()->getGConf()->modifier3);
		sendFnKey(value);
		Configuration::getBleKeyboard()->releaseAll();
		delay(Configuration::instance()->getGConf()->helperDelay);
		break;
	case 11: // Special functions
		specialFunction(value);

		break;
	case 12: // Numpad
		sendNumpad(value);
		break;
	case 13: // Custom functions
		sendUserAction(value);
		break;
	default:
		//If nothing matches do nothing
		break;
	}
}
} /* namespace ZTD */
