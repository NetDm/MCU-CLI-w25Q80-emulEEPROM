/*
 * NVM25Q80.h
 *
 *  Created on: Jun 3, 2022
 *      Author: netdm
 */

#ifndef APL_NVM25Q80_H_
#define APL_NVM25Q80_H_

typedef bool_t (*pfCallbackGetCharStream_t)(uint8_t* pChar);//ptr func typedef, return TRUE if char presented

typedef enum{
		 NO_INIT_NVM = 0 //0 - не изменять, 0 - автоматически присваивается состоянию автомата после сброса MCU
		,WAIT_INIT_NVM_AREA //ожидание определения области флеш для обработки (и колбэка, если не erease)
		,PREPARATIONS_CALC_NVM
		,COPY_SECT_NVM_TO_BUFF
		,WAIT_NVM_DATA	//работа в ядре автомата, все вводные инициализированны, идет процесс..
		,NVM_STREAM_RECIVE_COMPLITE
		,NVM_INTERN_ERROR
		,NVM_END_INDEX
}match_nvm_t;

typedef struct{
	size_t startAddrNvm;///относителный-абсолютный адрес флеш, начиная с 0
	size_t numbsWrite;	///кол-во записываемых данных
	pfCallbackGetCharStream_t passCallback;///если задается, то присваивается, иначе, логика присвоений обр. функции потока на стороне клиента
}nvm_t;

void initNVM25Q80();

void setCallbackInputStream(pfCallbackGetCharStream_t* pfArg);

bool_t pfStreamForErase(uint8_t* retByte);

uint8_t getOltByteNVM();
bool_t threadNVM25Q80(); // не блокирующая таймер-функция входа в обработчик внутренних событий неблокирующего автомата

bool_t startNvm(nvm_t* aNvm);

#endif /* APL_NVM25Q80_H_ */
