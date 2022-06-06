/*
 * hwConfig.h
 *
 *  Created on: Jun 1, 2022
 *      Author: me
 */

#ifndef HWCONFIG_H_
#define HWCONFIG_H_

#define DBUG (5)

/*
 * если юзается uart, необходим кольцевой буффер приема с DMA и NVIC, поставить галочки в STM32CUBE
 */

#define STM32CUBE (1)

#define HSPI25Q80 hspi1


#define HUART_CLI huart2

#define DBUG_HUART huart1

#define	CLI_PROMPT_DEFAULT_TEXT "25Q80 CLI >: " /*use size for positions under him*/



#if DBUG>=5
#warning ВНИМАНИЕ БЛОКИРУЮЩАЯ ОТЛАДКА, ДЛЯ РЕЛИЗА СНИЗИТЬ НИЖЕ 5х !!!
#endif

#endif /* HWCONFIG_H_ */
