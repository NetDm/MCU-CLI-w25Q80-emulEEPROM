/*
 * utils.h
 *
 *  Created on: 26 ���. 2022 �.
 *      Author: user
 */

#ifndef MAIN_USERCODES_INC_UTILS_H_
#define MAIN_USERCODES_INC_UTILS_H_

#include <stdio.h>
#include "options.h"

void vSleep(int);

void initTimerUtils();
size_t getTickUtils();

void SystemReset();
void onDeepSleep();

size_t centUp(size_t vol, size_t basis);

void resetTimeoutNewVolue(size_t Atimeout);
bool_t isNotTimeout();

size_t getTimeoutVolue(size_t Atime);

void timerStart();

size_t retTimeStamp();

#endif /* MAIN_USERCODES_INC_UTILS_H_ */
