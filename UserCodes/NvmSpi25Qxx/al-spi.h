/*
 * al-spi.h
 *
 *  Created on: Jun 1, 2022
 *      Author: NetDm
 */

#ifndef NVMSPI25QXX_AL_SPI_H_
#define NVMSPI25QXX_AL_SPI_H_

#include "w25Qxx.h"

void selectFlashSpi(void);
void deselectFlashSpi(void);

void sendDataSpi(void* arg, uint16_t aNubs );

void sendByteSpi(t_spi_data arg);
t_spi_data getByteSpi();

#endif
