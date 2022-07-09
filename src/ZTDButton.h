/*
 * ZTDButton.h
 *
 *  Created on: Jun 7, 2022
 *      Author: ZornCo
 */

#ifndef ZTDBUTTON_H_
#define ZTDBUTTON_H_
#include "CommonIncludes.h"
#include "ZTDAction.h"

namespace ZTD {

class ZTDButton {
public:
	char logo[64] = "question.bmp";
	ZTDAction actions[ACTION_COUNT];
	bool latch = false;
	bool hasLatch = false;
	char latchlogo[64] = "";
	static size_t getJsonSize();
	friend void convertToJson(const ZTDButton &src, JsonVariant dst);
	friend void convertFromJson(JsonVariantConst src, ZTDButton& dst);
	void doButtonAction();
};

} /* namespace ZTD */

#endif /* ZTDBUTTON_H_ */
