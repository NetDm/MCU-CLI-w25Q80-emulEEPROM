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

static nvm_t iNvm={0}; //instance NVM принятые на исполнение

uint8_t NvmPageQueue[2][PAGE_CACH_FLASH]; // временное хранилище страниц для переноса в кэш внешней флеш
uint8_t CopyInstanceSector[2][MIN_SECTOR_FLASH_ERASE];//полные копии модифицируемых секторов, перед и во время постраничной модификации

/*
 * @brief fCallbackGetCharStream - глобальный указатель на поток, видим только в этом файле
 *
 */
static pfCallbackGetCharStream_t fGetCharStream = NULL;
static match_nvm_t stateNvm = 0 ;//не менять! задано начальное состояние атомата
/*
 * @brief указатель-функция заглушка, если передать setCallbackInputStream указатель на нее
 * 			то вместо записи данных из потока, ячейки внешней флеш остануться затертыми
 *	@ret возращает TRUE, что-бы небыло простоя ожидания данных в автомате NVM
 */
volatile bool_t pfERASE_NVM(uint8_t* retByte){
	*retByte = CLEAR_NVM_SPI_BYTE_VOLUE;
	return TRUE;
}

static void initMatchNVM25Q80(){
	fGetCharStream = NULL;//сбросить все и вся, если сбой! или комплит ресив
}
/*	@brief функция назначения входящего потока, для записи во флеш из UART
 * 			может использоваться для произвольно-фрагментарного истирания.
 * 			Если назначить указатель-функцию на pfERASE_NVM
 * 			вместо записи новых значений в истираемую область, состояние
 * 			ячеек NVM не будет изменено, тем-самым произвольная область
 * 			будет затерта, но значения во всем секторе будут сохранены
 */
void setCallbackInputStream(pfCallbackGetCharStream_t* pfArg){
	fGetCharStream = pfArg;
}
/*
 *  @ret возращает последний байт потока, может использоваться для ответной печати процесса в консоль
 */
static uint8_t byteToNvm = CLEAR_NVM_SPI_BYTE_VOLUE;
static bool_t byteReadyForExtern = FALSE;
bool_t getOltByteNVM(uint8_t* retByte){
	if (byteReadyForExtern){
		*retByte=byteToNvm;
		byteReadyForExtern = FALSE;
		return TRUE;
	}else{
		return FALSE;
	}
}

/*
 *  @brief не блокирующая таймер-функция входа в обработчик внутренних событий неблокирующего автомата
 *  		необходимо организовать периодический вызов из суперцикла, задачи ОСРВ или таймера
 */
match_nvm_t threadNVM25Q80(){
	switch (stateNvm){
		case NO_INIT_NVM:{
			initMatchNVM25Q80();
			stateNvm++;
		}break;
		//
		case WAIT_INIT_NVM_AREA:{
			//состояние автомата смениться автоматически, при передаче в функцию инициации startNvm необходимых параметров для процесса связанного с NVM
		}break;
		//
		case WAIT_NVM_DATA:{
			if ( fGetCharStream != NULL){
				while (  (*fGetCharStream)(&byteToNvm)  ){
					byteReadyForExtern = TRUE;
					//здесь буду решать чего с этим byteToNvm из потока делать..

					////////////addByteToNvm(byteToNvm);

					if ( iNvm.numbsWrite != 0 ){
						iNvm.numbsWrite--;
					}
					if ( iNvm.numbsWrite >= ALL_SPI_FLASH_SIZE ){
						iNvm.numbsWrite=0;
					}
					if ( iNvm.numbsWrite == 0 ){
						stateNvm = NVM_STREAM_RECIVE_COMPLITE;
					}
				}
			}else{
				stateNvm = NVM_INTERN_ERROR ;
			}
		}break;
		//
		case NVM_STREAM_RECIVE_COMPLITE:{
			initMatchNVM25Q80();
			stateNvm = WAIT_INIT_NVM_AREA;
		}break;
		//
		case NVM_INTERN_ERROR:{
			stateNvm = NO_INIT_NVM;
		}break;
		//error match state
		default:{
			stateNvm = NVM_INTERN_ERROR ;
		}break;
	}//switch(..
	return (stateNvm);
}

/*
 * после валидации данных для автомата процесса NVM
 * инициируется необходимый процесс и автомат NVM приступает к работе
 */
bool_t startNvm(nvm_t* aNvm){
//	aNvm->numbsWrite
	if ( aNvm->startAddrNvm < ALL_SPI_FLASH_SIZE ){
		iNvm.startAddrNvm=aNvm->startAddrNvm;
	}else{return FALSE;}
	if (  ( ( aNvm->startAddrNvm + aNvm->numbsWrite ) <  ALL_SPI_FLASH_SIZE )  ){
			iNvm.numbsWrite = aNvm->numbsWrite;
	}else{return FALSE;}
	if ( aNvm->passCallback != NULL ){
		iNvm.passCallback=aNvm->passCallback;
		fGetCharStream=iNvm.passCallback;
	}
	stateNvm = WAIT_NVM_DATA;
	return TRUE;
}
