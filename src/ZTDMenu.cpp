/*
 * ZTDMenu.cpp
 *
 *  Created on: Jun 8, 2022
 *      Author: ZornCo
 */

#include "ZTDMenu.h"
#include "Screen.h"
#include "Configuration.h"
#include "KeyboardHelper.h"
namespace ZTD {

void ZTDMenu::doButton(uint8_t index) {
	Serialprintln("MenuState:%d, MenuIndex:%d, Index:%d", Configuration::getMenuState(), Configuration::getMenuIndex(), index);
	Serialprintln("BUTTON_COUNT:%d", BUTTON_COUNT);
	switch (Configuration::getMenuState()) {
	case HOME:
		if(index == BUTTON_COUNT-1)
		{
			Configuration::setMenuState(SETTINGS);
		} else {
			Configuration::setMenuIndex(index);
			Configuration::setMenuState(ACTION);
		}
	    Screen::instance()->drawKeypad();
		return;
	case SETTINGS:
		switch (index) {
		case 0:
			//wifi
			KeyboardHelper::bleKeyboardAction(11, 1, 0);
			break;
		case 1:
			// brightness down
			KeyboardHelper::bleKeyboardAction(11, 2, 0);
			break;
		case 2:
			// brightness up
			KeyboardHelper::bleKeyboardAction(11, 3, 0);
			break;
		case 3:
			// sleep toggle
			KeyboardHelper::bleKeyboardAction(11, 4, 0);
//			if (islatched[28])
//			{
//			  islatched[28] = 0;
//			}
//			else
//			{
//			  islatched[28] = 1;
//			}
			break;
		case 4:
			//info
			Configuration::setMenuState(INFO);
		    Screen::instance()->drawKeypad();
			break;
		case 5:
			//return home
			Configuration::setMenuState(HOME);
		    Screen::instance()->drawKeypad();
			break;
		default:
			break;
		}
		return;
	case ERROR:
		// do nothing
		return;
	default:
		if(index == BUTTON_COUNT-1)
		{
			//return home
			Configuration::setMenuState(HOME);
		    Screen::instance()->drawKeypad();
			break;
		}
		// isn't a special menu, draw current menu buttons
		uint8_t currentMenuIndex = Configuration::getMenuIndex();
		ZTDMenu *currentMenu = &Configuration::getMenus()[currentMenuIndex];
		currentMenu->buttons[index].doButtonAction();
		break;
	}
}

} /* namespace ZTD */
