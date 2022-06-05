/*
 * base64.h
 *
 *  Created on: 21 июл. 2021 г.
 *      Author: me
 */

#ifndef INC_BASE64_H_
#define INC_BASE64_H_

#include <stdint.h>

#define BASE64_PAD '='
#define BASE64DE_FIRST '+'
#define BASE64DE_LAST 'z'

unsigned int base64decode(uint8_t* in,uint8_t* out,unsigned int inlen);

#endif /* INC_BASE64_H_ */
