/*
 * utils.c
 *
 *  Created on: 26 ���. 2022 �.
 *      Author: user
 */

#include <stdio.h>
#include <stdint.h>

#ifdef INC_FREERTOS_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/timer.h"
#endif


/*
 * \return возращает целое взаимствованое округление вверх (math.ceil) по основанию basis
 * \brief например 5 по основанию 4 = 8
 */
size_t centUp(size_t vol, size_t basis){
	return ( ((vol+(basis-1))/basis)*basis );
}
/*
 * \brief Таймер задержки, можно реализовать без ОСРВ
 */
void sleep(size_t msec){
#ifdef INC_FREERTOS_H
	vTaskDelay(msec / portTICK_PERIOD_MS);
#endif
#ifdef STM32CUBE
	HAL_Delay(msec);
#endif
}

#ifdef INC_FREERTOS_H
#if configUSE_TIMERS!=1
#error
#endif
#endif

/*
 * \brief Завести таймер времени в мс с разрядностью не ниже 32 бита
 * \brief вызывается автоматически перед запуском ApL
 */
void initTimerUtils(){
#if ESP32IDF
    timer_config_t config = {
        .divider = 0xFFFF,//80MHz/1ms
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_START,
        .alarm_en = TIMER_ALARM_DIS,
        .auto_reload = false,
    }; // default clock source is APB
    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_start(TIMER_GROUP_0, TIMER_0);
#elif STM32CUBE

#elif MILANDR
#error реализовать


#else
#warning секцию не удалять, человекам-снежинкам вход запрещен!
#error реализовать таймер времени в мс с разрядностью не ниже 32 бита в отдельной секции выше!
#endif
}
/*
 * \brief В случе отсутствия системного таймера,
 * \brief завести таймер времени в мс с разрядностью не ниже 32 бита
 * \return Возращает значение системного времени в мс
 */
size_t getTickUtils(){
	size_t count;
#if ESP32IDF
	timer_get_counter_value(TIMER_GROUP_0, TIMER_0,&count);
#elif STM32CUBE
	count=HAL_GetTick();
#elif MILANDR
#error реализовать


#else
#warning секцию не удалять, человекам-снежинкам вход запрещен!
#error реализовать таймер времени в мс с разрядностью не ниже 32 бита в отдельной секции выше!
#endif
	return (  (size_t) count  );
}
/*
 * \return функция возращает значение системного таймера, после которого наступит превышение интервала
 * \brief при использовании таймера
 */
size_t getTimeoutVolue(size_t Atime){
	return ( getTickUtils() + Atime );
}
/*
 * \brief Установка локального таймера внутри данной библиотеки утилит, проверять в isNotTimeout()
 */
static size_t timeEnd=-1;
void resetTimeoutNewVolue(size_t Atimeout){
	timeEnd=getTimeoutVolue(Atimeout);
}
/*
 * \return Возращает лог значение события переполнения таймера в библиотеке текущих утилит
 */
bool_t isNotTimeout(){
	return ( getTickUtils() < timeEnd );
}
/*
 * засекает время
 */
void timerStart(){
	resetTimeoutNewVolue(0);
}
/*
 * возращает время от засечки в мс
 */
size_t retTimeStamp(){
	return ( getTickUtils() - timeEnd );
}
/*
 * \brief HAL системный программный сброс MCU
 */
void SystemReset(){
#if ESP32IDF
	esp_restart();
#elif STM32CUBE

#elif MILANDR
#error реализовать функцию HAL soft reset MCU для текущей модели


#else
#warning секцию не удалять, человекам-снежинкам вход запрещен!
#error реализовать функцию HAL soft reset MCU для текущей модели в отдельной секции выше!
#endif
}
