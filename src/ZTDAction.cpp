/*
 * ZTDAction.cpp
 *
 *  Created on: Jun 8, 2022
 *      Author: ZornCo
 */

#include "ZTDAction.h"
#include "KeyboardHelper.h"

namespace ZTD {

void ZTDAction::bleKeyboardAction() {
	KeyboardHelper::bleKeyboardAction(action, value, symbol);
}

} /* namespace ZTD */
