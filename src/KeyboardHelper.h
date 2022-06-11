/*
 * KeyboardHelper.h
 *
 *  Created on: Jun 6, 2022
 *      Author: ZornCo
 */

#ifndef KEYBOARDHELPER_H_
#define KEYBOARDHELPER_H_

#include "CommonIncludes.h"
namespace ZTD {

class KeyboardHelper {
	static void pressKey(unsigned char key);
	static void writeKey(unsigned char key);
	static void sendNavigationKey(int value);
	static void sendMediaKey(int value);
	static void sendOptionKey(int value);
	static void sendFnKey(int value);
	static void sendOptionCombo(int value);
	static void specialFunction(int value);
	static void sendNumpad(int value);
	static void sendUserAction(int value);
public:
	static void bleKeyboardAction(int action, int value, char* symbol);
};

} /* namespace ZTD */

#endif /* KEYBOARDHELPER_H_ */
