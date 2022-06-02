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

#define SPITIMEOUTMS (10UL)

void selectFlashSpi(void){
	HAL_GPIO_WritePin(fromMX(SPI_FLASH_CS),RESET);
	HAL_Delay(10);
}
void deselectFlashSpi(void){
	HAL_GPIO_WritePin(fromMX(SPI_FLASH_CS),SET);
	HAL_Delay(10);
}


t_spi_data sendGetDataSpi(t_spi_data arg){
	t_spi_data TX = arg;
	t_spi_data RX;
	size_t t=SPITIMEOUTMS;
	while(  (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY) && (--t!=0)  ){
		HAL_Delay(1);
	}
		#if DBUG>=1
	if (t==0){
			//		dbugerr("timeout SPI");
			#if DBUG>=2
		while(1==1);
			#endif
	}
		#endif

	HAL_SPI_TransmitReceive(&hspi3
			, &TX
			, &RX
			,1/*sizeof(t_spi_data)*/
			,100
			);

	return ( RX );
}


