/*
 * al-spi.c
 *
 *  Created on: Jun 1, 2022
 *      Author: NetDm
 */

//FUNCTION POINTERS FOR INTERNAL LIBRARY USE
//SET THESE TO YOUR APPLICATION FUNCTIONS

#include <stm32l4xx_hal.h>

#include "../../Core/Inc/spi.h"

#include "al-spi.h"

#include "w25Qxx.h"

#define SPITIMEOUTMS (3000)

void selectFlashSpi(void){
	if ( HAL_GPIO_ReadPin(fromMX(SPI_FLASH_CS)) == SET ){
		HAL_GPIO_WritePin(fromMX(SPI_FLASH_CS),RESET);
		HAL_Delay(1);
	}
}
void deselectFlashSpi(void){
	HAL_GPIO_WritePin(fromMX(SPI_FLASH_CS),SET);
	HAL_Delay(1);
}

void sendDataSpi(void* arg, uint16_t aNumbs ){
	spi_data_t TX = arg;
	HAL_SPI_Transmit(&HSPI25Q80
			, arg
			,aNumbs/*sizeof(t_spi_data)*/
			,SPITIMEOUTMS
			);

	return ;
}

void sendByteSpi(spi_data_t arg){
	spi_data_t TX = arg;
	spi_data_t RX;
	HAL_SPI_Transmit(&HSPI25Q80
			, &TX
			,1
			,SPITIMEOUTMS
			);

	return ( RX );
}

spi_data_t getByteSpi() {
		spi_data_t RX;
		HAL_SPI_Receive(&HSPI25Q80
				, &RX
				,1
				,SPITIMEOUTMS
				);
		return ( RX );
	}
