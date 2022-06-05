/*
 * dbug.c
 *
 *  Created on: 5 июн. 2022 г.
 *      Author: me
 */

#include "dbug.h"

#ifdef DBUG_HUART

extern UART_HandleTypeDef DBUG_HUART;

int __io_putchar(int ch){
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART1 and Loop until the end of transmission */
	HAL_UART_Transmit(&DBUG_HUART, (uint8_t*) &ch, 1, 1);

	return ch;
}

#endif
