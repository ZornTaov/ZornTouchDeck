/*
 * ZTDAction.h
 *
 *  Created on: Jun 8, 2022
 *      Author: ZornCo
 */

#ifndef ZTDACTION_H_
#define ZTDACTION_H_
#include "CommonIncludes.h"

namespace ZTD {

class ZTDAction {
	public:
		uint8_t action;
		uint8_t value;
		char symbol[32];
		static size_t getJsonSize();
		friend void convertToJson(const ZTDAction &src, JsonVariant dst);
		friend void convertFromJson(JsonVariantConst src, ZTDAction& dst);
		void bleKeyboardAction();
	};

} /* namespace ZTD */

#endif /* ZTDACTION_H_ */
