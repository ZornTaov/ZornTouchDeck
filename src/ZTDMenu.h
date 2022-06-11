/*
 * ZTDMenu.h
 *
 *  Created on: Jun 8, 2022
 *      Author: ZornCo
 */

#ifndef ZTDMENU_H_
#define ZTDMENU_H_
#include "CommonIncludes.h"
#include "ZTDButton.h"
namespace ZTD {

class ZTDMenu {
	public:
		ZTDButton buttons[BUTTON_COUNT - 1];
		char logos[BUTTON_COUNT - 1][32];
		static size_t getJsonSize();
		friend void convertToJson(const ZTDMenu &src, JsonVariant dst);
		friend void convertFromJson(JsonVariantConst src, ZTDMenu& dst);
	};

} /* namespace ZTD */

#endif /* ZTDMENU_H_ */
