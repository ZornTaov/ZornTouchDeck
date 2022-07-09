/*
 * ZTDButton.cpp
 *
 *  Created on: Jun 7, 2022
 *      Author: ZornCo
 */

#include "ZTDButton.h"
#include "Screen.h"
#include "Configuration.h"

namespace ZTD {

void ZTDButton::doButtonAction() {

	for (int i = 0; i < ACTION_COUNT; ++i) {
		actions[i].bleKeyboardAction();
	}
	Configuration::getBleKeyboard()->releaseAll();
	if (hasLatch) {
		latch = !latch;
	}

	delay(10); // UI debouncing

}

} /* namespace ZTD */
