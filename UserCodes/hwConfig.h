/*
 * hwConfig.h
 *
 *  Created on: Jun 1, 2022
 *      Author: me
 */

#ifndef HWCONFIG_H_
#define HWCONFIG_H_

/*
 * если юзается uart, необходим кольцевой буффер приема с DMA и NVIC настроить в STM32CUBE
 */
#define HUART_CLI huart2

#define	CLI_PROMPT_DEFAULT_TEXT "25Q80 CLI >: " /*use size for positions under him*/

#define DBUG (2)
#if DBUG>=2
#warning ВНИМАНИЕ БЛОКИРУЮЩАЯ ОТЛАДКА, ДЛЯ РЕЛИЗА СНИЗИТЬ НИЖЕ 2х !!!
#endif

#endif /* HWCONFIG_H_ */
