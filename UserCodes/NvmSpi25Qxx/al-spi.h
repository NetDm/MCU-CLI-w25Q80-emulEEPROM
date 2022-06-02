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

t_spi_data sendGetDataSpi(t_spi_data arg);

#endif
