/*
 * Printf.cpp
 *
 *  Created on: Jun 8, 2022
 *      Author: ZornCo
 */
#include "Printf.h"

extern "C" {
void Serialprintln(const char* input...) { // @suppress("Name convention for function")
	  va_list args;
	  va_start(args, input);
	  for(const char* i=input; *i!=0; ++i) {
		if(*i!='%') { Serial.print(*i); continue; }
		switch(*(++i)) {
		  case '%': Serial.print('%'); break;
		  case 's': Serial.print(va_arg(args, char*)); break;
		  case 'd': Serial.print(va_arg(args, int), DEC); break;
		  case 'b': Serial.print(va_arg(args, int), BIN); break;
		  case 'o': Serial.print(va_arg(args, int), OCT); break;
		  case 'x': Serial.print(va_arg(args, int), HEX); break;
		  case 'f': Serial.print(va_arg(args, double), 2); break;
		}
	  }
	  Serial.println();
	  va_end(args);
	}
}

