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

typedef struct{
	//	uint8_t CopyInstanceSector[2][MIN_SECTOR_FLASH_ERASE];///полные копии модифицируемых секторов, перед и во время постраничной модификации
	//	uint8_t* NvmPageQueue[2*MIN_SECTOR_FLASH_ERASE/PAGE_CACH_FLASH][PAGE_CACH_FLASH]; ///уквзатель на временное хранилище страниц/секторов для переноса в кэш внешней флеш
	uint8_t CopyInstanceSector[MIN_SECTOR_FLASH_ERASE];///копия модифицируемого сектора, перед и во время постраничной модификации, можно даже 1 байта, без 2го полубуфера, тк поток пишется в свой буфер, на фоне, через DMA и данные забираются, когда удобно автомату процесса флеш
//	сахар запутывает, проще на месте - uint8_t* NvmPageCach[MIN_SECTOR_FLASH_ERASE/PAGE_CACH_FLASH][PAGE_CACH_FLASH]; ///уквзатель на временное хранилище страниц/секторов для переноса в кэш внешней флеш
	size_t 	iSectorAddr;///текущий адрес сектора для копии, стирания, записи
// вычисляется в процессе, в глобальных ему пока не место	size_t	iPageAddr;	///текущий адрес страницы относительный в секторе
	size_t	iAddr;		///текущий относительный адрес в секторе-копии (получение нужной страницы, вычисляется по ходу процесса) BUFF в MCU для записи приходящего байта
//	char	nPage;		///выбор текущего полу-буфера для записи приходящих данных
//	char	nSector;	///индекс текущего полубуфера копии сектора, для записи приходящих
}spi_flash_proc_t;

spi_flash_proc_t FlashProc;

/*
 * @brief fCallbackGetCharStream - глобальный указатель на поток, видим только в этом файле
 *
 */
static pfCallbackGetCharStream_t pfGetCharStream = NULL;//указ функц возр данные потока
static match_nvm_t stateNvm = 0 ;//не менять! задано начальное состояние атомата
/*
 * @brief указатель-функция заглушка, если передать setCallbackInputStream указатель на нее
 * 			то вместо записи данных из потока, ячейки внешней флеш остануться затертыми
 *	@ret возращает TRUE, что-бы небыло простоя ожидания данных в автомате NVM
 */
bool_t pfStreamForErase(uint8_t* retByte){
	*retByte = CLEAR_NVM_SPI_BYTE_VOLUE;
	return TRUE;
}

static void initMatchNVM25Q80(){
	pfGetCharStream = NULL;//сбросить все и вся, если сбой! или комплит ресив
}
/*	@brief функция назначения входящего потока, для записи во флеш из UART
 * 			может использоваться для произвольно-фрагментарного истирания.
 * 			Если назначить указатель-функцию на pfERASE_NVM
 * 			вместо записи новых значений в истираемую область, состояние
 * 			ячеек NVM не будет изменено, тем-самым произвольная область
 * 			будет затерта, но значения во всем секторе будут сохранены
 */
void setCallbackInputStream(pfCallbackGetCharStream_t* pfArg){
	pfGetCharStream = pfArg;
}
/*
 *  @ret возращает последний байт потока, может использоваться для ответной печати процесса в консоль
 */
uint8_t byteToNvm = CLEAR_NVM_SPI_BYTE_VOLUE;

const char* szStateNvm[]={
		 "NO_INIT_NVM"
		,"WAIT_INIT_NVM_AREA"
		,"PREPARATIONS_CALC_NVM"
		,"COPY_SECT_NVM_TO_BUFF"
		,"WAIT_NVM_DATA"
		,"NVM_STREAM_RECIVE_COMPLITE"
		,"NVM_INTERN_ERROR"
		,0
};


/*
 *  @brief не блокирующая таймер-функция входа в обработчик внутренних событий неблокирующего автомата
 *  		необходимо организовать периодический вызов из суперцикла, задачи ОСРВ или таймера
 *	@ret bool_t - возращает 0 - если отработал, можно использовать для зацикливания, пока не отработает
 */
bool_t threadNVM25Q80(){
	static match_nvm_t mStateNvm=-1;
#if DBUG>=5
	if ( stateNvm != mStateNvm ){
		if (szStateNvm[NVM_END_INDEX]!=0){
			dbugerr("szStateNvm[] != NVM_END_INDEX ");
		}
		mStateNvm = stateNvm;
		dbugnl4("Automat new state: [%s]",szStateNvm[mStateNvm]);
	}
#endif
	switch (stateNvm){
		case NO_INIT_NVM:{
			initMatchNVM25Q80();
			dbugnl("init NVM");
			stateNvm = WAIT_INIT_NVM_AREA;
		}break;
		//
		case WAIT_INIT_NVM_AREA:{
			//ожидание назначения потока и адреса
			//состояние автомата смениться автоматически, при передаче в функцию инициации startNvm необходимых параметров для процесса связанного с NVM
		}break;
		case PREPARATIONS_CALC_NVM:{
			dbug("п.5 подготовка сектора");
			if (  ( FlashProc.iAddr !=0 )
				||  ( iNvm.numbsWrite < MIN_SECTOR_FLASH_ERASE )  ){
				dbugnl("вычитываем сектор addr = %d",FlashProc.iSectorAddr);
				readBlockDataFlash25q(FlashProc.iSectorAddr
						, FlashProc.CopyInstanceSector
						, MIN_SECTOR_FLASH_ERASE);
				eraseSector4kFlash25q(FlashProc.iSectorAddr);
			}
			stateNvm = WAIT_NVM_DATA;
		}break;
		//овобождает буф сектора, инкрементирует п.10,п.11,п.12
		void flushBuffSector(){ //используется в различных частях, дублировать нельзя, из-за возможности допустить ошибку ( сост. авт. WAIT_NVM_DATA & NVM_STREAM_RECIVE_COMPLITE)
			dbug("flushBuffSector");
				writeSectorFlash25q( FlashProc.iSectorAddr
									, FlashProc.CopyInstanceSector
									, MIN_SECTOR_FLASH_ERASE );
				FlashProc.iAddr = 0 ;
				FlashProc.iSectorAddr += MIN_SECTOR_FLASH_ERASE;
			}
		//
		case WAIT_NVM_DATA:{
			if ( pfGetCharStream != NULL ){
				while (  (*pfGetCharStream)(&byteToNvm)  ){
//					byteReadyForExtern = TRUE;
					//здесь буду решать чего с этим byteToNvm из потока делать..

					FlashProc.CopyInstanceSector[FlashProc.iAddr]=byteToNvm;
					FlashProc.iAddr++;

					dbugnl4("getchar: %02x , addr = %lu , i=%u"
							,byteToNvm,FlashProc.iAddr, iNvm.numbsWrite);

					if ( iNvm.numbsWrite != 0 ){
						iNvm.numbsWrite--;
					}
					if ( iNvm.numbsWrite >= ALL_SPI_FLASH_SIZE ){
						iNvm.numbsWrite=0;
						dbugerr("упс, iNvm.numbsWrite >= ALL_SPI_FLASH_SIZE");
						return FALSE;
					}
					if ( iNvm.numbsWrite == 0 ){
						dbugnl("iNvm.numbsWrite === 0");
						pfGetCharStream = NULL;
						stateNvm = NVM_STREAM_RECIVE_COMPLITE;
						break;
					}
					//
					if ( FlashProc.iAddr > MIN_SECTOR_FLASH_ERASE ){
						dbugerr("что-то не то с индексом iAddr");
					}
					if ( FlashProc.iAddr == MIN_SECTOR_FLASH_ERASE ){
						flushBuffSector();
						stateNvm = PREPARATIONS_CALC_NVM;
						break;
					}
				}//while
			}else{
				dbugerr("в WAIT_NVM_DATA состоянии, pfGetCharStream == 0");
				stateNvm = NVM_INTERN_ERROR ;
			}
		}break;
		//
		case NVM_STREAM_RECIVE_COMPLITE:{
			dbugnl("NVM_STREAM_RECIVE_COMPLITE");
			flushBuffSector();
			stateNvm = NO_INIT_NVM;
			return FALSE;
		}break;
		//
		case NVM_INTERN_ERROR:{
			dbugerr("NVM_INTERN_ERROR");
			stateNvm = NO_INIT_NVM;
		}break;
		//error match state
		default:{
			stateNvm = NVM_INTERN_ERROR ;
		}break;
	}//switch(..
	return (TRUE);
}

/*
 * после валидации данных для автомата процесса NVM
 * инициируется необходимый процесс и автомат NVM приступает к работе
 */
bool_t startNvm(nvm_t* aNvm){
	dbugnl("НАЧАЛО ПРОЦЕССА startNvm");
//	aNvm->numbsWrite
	if ( stateNvm != WAIT_INIT_NVM_AREA ){
		dbugerr("инициализации SPI flash");
		return FALSE;
	};
	if ( aNvm->startAddrNvm < ALL_SPI_FLASH_SIZE ){
		iNvm.startAddrNvm=aNvm->startAddrNvm;
	}else{return FALSE;}
	if (  ( ( aNvm->startAddrNvm + aNvm->numbsWrite ) <=  ALL_SPI_FLASH_SIZE )  ){
			iNvm.numbsWrite = aNvm->numbsWrite;
	}else{return FALSE;}
	if ( aNvm->passCallback != NULL ){
		iNvm.passCallback=aNvm->passCallback;
		pfGetCharStream=iNvm.passCallback;
	}

	const int NSector = MIN_SECTOR_FLASH_ERASE / PAGE_CACH_FLASH ;///кол-во стр в секторе
	//п2. Вычисляем целевой адрес сектора назначения данных SPI flash
	FlashProc.iSectorAddr = ( iNvm.startAddrNvm / MIN_SECTOR_FLASH_ERASE ) * MIN_SECTOR_FLASH_ERASE;
//	FlashProc.nSector = 0;//только для переключения на лету
	//п3. Вычисляем выравнивание адреса помещаемых данных относительно сектора, тк основная мин работа с сектором, потом уже с кэш страницей
	FlashProc.iAddr	= (iNvm.startAddrNvm - FlashProc.iSectorAddr) % MIN_SECTOR_FLASH_ERASE;

	dbugnl("SectorAddr = %lu , относит. Addr в секторе = %lu , pазм. = %u"
			,FlashProc.iSectorAddr
			,FlashProc.iAddr
			,iNvm.numbsWrite
	);


	/*
	 	 	(nSector%2)   (nSector+1)%2
	 	  iSectorAddr	 iSectorAddr
	 .....|..............|..............|.....
	 .....|..|..|..|..|..|..|..|..|..|..|..|..
	 	    ^      					 |
	  	    iAddr(% PAGE_CACH_FLASH) |
	  	    					 	 nPage (% NSector)
	*/
	stateNvm = PREPARATIONS_CALC_NVM;
	return TRUE;
}















