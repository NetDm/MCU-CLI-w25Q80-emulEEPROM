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
	t_spi_data TX = arg;
	HAL_SPI_Transmit(&HSPI25Q80
			, arg
			,aNumbs/*sizeof(t_spi_data)*/
			,SPITIMEOUTMS
			);

	return ;
}

void sendByteSpi(t_spi_data arg){
	t_spi_data TX = arg;
	t_spi_data RX;
	HAL_SPI_Transmit(&HSPI25Q80
			, &TX
			,1
			,SPITIMEOUTMS
			);

	return ( RX );
}

t_spi_data getByteSpi() {
		t_spi_data RX;
		HAL_SPI_Receive(&HSPI25Q80
				, &RX
				,1
				,SPITIMEOUTMS
				);
		return ( RX );
	}
