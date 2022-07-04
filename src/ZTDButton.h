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
	ZTDAction actions[ACTION_COUNT];
	bool latch;
	char logo[32] = "question.bmp";
	char latchlogo[32] = "";
	static size_t getJsonSize();
	friend void convertToJson(const ZTDButton &src, JsonVariant dst);
	friend void convertFromJson(JsonVariantConst src, ZTDButton& dst);
};

} /* namespace ZTD */

#endif /* ZTDBUTTON_H_ */
