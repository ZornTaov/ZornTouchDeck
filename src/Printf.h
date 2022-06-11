/*
 * Printf.h
 *
 *  Created on: Jun 6, 2022
 *      Author: ZornCo
 */

#ifndef _PRINTF
#define _PRINTF
#include <stdarg.h>
#include "CommonIncludes.h"

extern "C" {
	void Serialprintln(const char* input...);
}
#endif
