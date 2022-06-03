/*
 * NVM25Q80.c
 *
 *  Created on: Jun 3, 2022
 *      Author: NetDm
 *
 *      @brief NVM25Q80 - библиотека управлением flash SPI для дозаписи, перезаписи произвольных данных
 *
 */

#include "w25q80def.h"

#include "NVM25Q80.h"

#include <w25Qxx.h>

uint8_t NvmPageQueue[2][PAGE_CACH_FLASH]; // временное хранилище страниц для переноса в кэш внешней флеш
uint8_t CopyInstanceSector[2][MIN_SECTOR_FLASH_ERASE];//полные копии модифицируемых секторов, перед и во время постраничной модификации

/*
 * @brief fCallbackGetCharStream - глобальный указатель на поток, видим только в этом файле
 *
 */
static pfCallbackGetCharStream_t* fGetCharStream = NULL;
/*
 * @brief функция назначения входящего потока, для записи во флеш из UART
 * 			может использоваться для произвольно-фрагментарного истирания.
 * 			Если назначить функцию возращающую значения CLEAR_NVM_SPI_BYTE_VOLUE
 * 			или сбросить указатель-функцию на NULL
 * 			вместо записи новых значений в истираемую область, состояние
 * 			ячеек NVM не будет изменено, тем-самым произвольная область будет
 * 			затерта, но значения во всем секторе будут сохранены
 */
void setCallbackInputStream(pfCallbackGetCharStream_t* pfArg){
	fGetCharStream = pfArg;
}
/*
 *  @ret возращает последний байт потока, может использоваться для ответной печати процесса в консоль
 */
static uint8_t byte = CLEAR_NVM_SPI_BYTE_VOLUE;
uint8_t getOltByteNVM(){
	return byte;
}
/*
 *  @brief не блокирующая таймер-функция входа в обработчик внутренних событий неблокирующего автомата
 *  		необходимо организовать периодический вызов из суперцикла, задачи ОСРВ или таймера
 */
match_nvm_t threadNVM25Q80(){
	if (fGetCharStream != NULL){
		while ((*fGetCharStream)(&byte)){
			//здесь буду решать чего с этим делать..
			//необходима передача управления основному автомату
		}
		}
		return (byte);
	}else{
		return -1;
	}

}
