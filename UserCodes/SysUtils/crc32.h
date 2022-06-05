/*
 * crc32.h
 *
 *  Created on: 26 июл. 2021 г.
 *      Author: me
 */

#ifndef INC_CRC32_H_
#define INC_CRC32_H_

#include <stdint.h>

void crc32begin(void);
uint32_t crc32calculate( uint32_t len, const void *data );
uint32_t getLastCrc32();

#endif // CRC32_H
