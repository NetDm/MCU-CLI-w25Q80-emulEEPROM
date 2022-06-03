/*
 * NVM25Q80.h
 *
 *  Created on: Jun 3, 2022
 *      Author: netdm
 */

#ifndef APL_NVM25Q80_H_
#define APL_NVM25Q80_H_

typedef enum{
		 NO_INIT=128
		,WAIT_INIT_AREA //ожидание определения области флеш для обработки (и колбэка, если не erease)
		,
}match_nvm_t;

typedef struct{
	size_t startAddrNvm;
	size_t stopAddrNvm;

};

void initNVM25Q80();

typedef bool_t (*pfCallbackGetCharStream_t)(uint8_t* pChar);//ptr func typedef, return TRUE if char presented

void setCallbackInputStream(pfCallbackGetCharStream_t* pfArg);

match_nvm_t threadNVM25Q80(); // не блокирующая таймер-функция входа в обработчик внутренних событий неблокирующего автомата

#endif /* APL_NVM25Q80_H_ */
