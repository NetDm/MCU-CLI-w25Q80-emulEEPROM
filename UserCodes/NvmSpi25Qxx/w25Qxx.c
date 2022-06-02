/*************************************************************************
 * WINBOND W25QXX FLASH CHIP LIBRARY
 *
 *	SUPPORT FOR ONLY NORMAL SPI MODE (NO BI/QUAD SPI)
 * SPI DATA FORMAT : 8 BITS, MSB FIRST
 *
 *  Created on: Jun 1, 2022
 *      Author: NetDm
 *
 ************************************************************************* */

#include "../NvmSpi25Qxx/w25Qxx.h"

#include "al-spi.h"

t_spi_data (*_pfSpiGetSend)(t_spi_data) = &sendGetDataSpi;
void (*pfSelectSpiFlash)(void) = &selectFlashSpi;
void (*pfSetFreeSpiFlash)(void) = &deselectFlashSpi;

/************************/
void initFlash25q(void){
	char pVoid[8];
	GetUniqueIdFlash25q(pVoid);
}

static void whileBusyFlash25q(){
	(*pfSelectSpiFlash)();
//	while (getBusyStatusFlash25q() != 0) {
//
}


void setResetModeFlash25q(void) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_ENABLE_RESET);
}

void doResetFlash25q(void) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_RESET);
}

uint8_t getStatusRegFlash25q(uint8_t register_num) {
	if (register_num == 1) {
		(*_pfSpiGetSend)(FLASH25Q_COMMAND_READ_STATUS_REG_1);

		(*pfSetFreeSpiFlash)();
		return (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	} else if (register_num == 2) {
		(*_pfSpiGetSend)(FLASH25Q_COMMAND_READ_STATUS_REG_2);

		(*pfSetFreeSpiFlash)();
		return (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	}
	(*pfSetFreeSpiFlash)();
	return 0;
}

uint8_t getBusyStatusFlash25q(void){

	uint8_t val = getStatusRegFlash25q(1);
	return (val & 0x01);
}

void setEnableWriteFlash25q(void) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_WRITE_ENABLE);
}

void setDisableWriteFlash25q(void) {

	whileBusyFlash25q();

	(*pfSelectSpiFlash)();

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_WRITE_DISABLE);
}


uint16_t getManufactIdFlash25q(void) {

	whileBusyFlash25q();

	(*pfSelectSpiFlash)();

	uint16_t temp;

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_MANUFACTURER_ID);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	temp = (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	temp = temp << 8;
	temp |= (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);

	return temp;
}

void GetUniqueIdFlash25q(uint8_t *pRet) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_UNIQUE_ID);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);

	pRet[0] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[1] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[2] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[3] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[4] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[5] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[6] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[7] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);

	(*pfSetFreeSpiFlash)();
}

void getJedecIdFlash25q(uint8_t *pRet) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_JEDEC_ID);
	pRet[0] = (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[1] = (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	pRet[2] = (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
}

uint8_t readByteDataFlash25q(uint32_t add) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	uint8_t aHi = (uint8_t) ((add & 0x00FF0000) >> 16);
	uint8_t aMid = (uint8_t) ((add & 0x0000FF00) >> 8);
	uint8_t aLo = (uint8_t) (add);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_READ_DATA);
	(*_pfSpiGetSend)(aHi);
	(*_pfSpiGetSend)(aMid);
	(*_pfSpiGetSend)(aLo);
	return (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
}

void readBlockDataFlash25q(uint32_t add, uint8_t *data_ptr, uint32_t read_len) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	uint8_t aHi = (uint8_t) ((add & 0x00FF0000) >> 16);
	uint8_t aMid = (uint8_t) ((add & 0x0000FF00) >> 8);
	uint8_t aLo = (uint8_t) (add);
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_READ_DATA);
	(*_pfSpiGetSend)(aHi);
	(*_pfSpiGetSend)(aMid);
	(*_pfSpiGetSend)(aLo);

	for (uint32_t i = 0; i < read_len; i++) {
		data_ptr[i] = (uint8_t) (*_pfSpiGetSend)(FLASH25Q_COMMAND_NOP);
	}
}

void writeStatusRegsFlash25q(uint8_t val_stat_reg_1, uint8_t val_stat_reg_2) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_WRITE_STATUS_REGS);
	(*_pfSpiGetSend)(val_stat_reg_1);
	(*_pfSpiGetSend)(val_stat_reg_2);
}

void eraseSector4kFlash25q(uint32_t add_sector) {
	//ERASE THE SPECIFIED SECTOR (4KB)

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	uint8_t aHi = (uint8_t) ((add_sector & 0x00FF0000) >> 16);
	uint8_t aMid = (uint8_t) ((add_sector & 0x0000FF00) >> 8);
	uint8_t aLo = (uint8_t) (add_sector);

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_SECTOR_ERASE_4KB);
	(*_pfSpiGetSend)(aHi);
	(*_pfSpiGetSend)(aMid);
	(*_pfSpiGetSend)(aLo);
}

void eraseBlock32kFlash25q(uint32_t aBlock) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	uint8_t aHi = (uint8_t) ((aBlock & 0x00FF0000) >> 16);
	uint8_t aMid = (uint8_t) ((aBlock & 0x0000FF00) >> 8);
	uint8_t aLo = (uint8_t) (aBlock);

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_BLOCK_ERASE_32KB);
	(*_pfSpiGetSend)(aHi);
	(*_pfSpiGetSend)(aMid);
	(*_pfSpiGetSend)(aLo);
}

void eraseBlock64kFlash25q(uint32_t aBlock) {

	whileBusyFlash25q();


	(*pfSelectSpiFlash)();

	uint8_t aHi = (uint8_t) ((aBlock & 0x00FF0000) >> 16);
	uint8_t aMid = (uint8_t) ((aBlock & 0x0000FF00) >> 8);
	uint8_t aLo = (uint8_t) (aBlock);

	(*_pfSpiGetSend)(FLASH25Q_COMMAND_BLOCK_ERASE_64KB);
	(*_pfSpiGetSend)(aHi);
	(*_pfSpiGetSend)(aMid);
	(*_pfSpiGetSend)(aLo);
}

void eraseAllChipFlash25q(void) {
	whileBusyFlash25q();

	(*pfSelectSpiFlash)();
	(*_pfSpiGetSend)(FLASH25Q_COMMAND_CHIP_ERASE);
}

