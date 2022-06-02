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

//t_spi_data (*_pfSpiGetSendByte)(t_spi_data) = &sendGetByteSpi;

static void sleep(uint32_t a){
	HAL_Delay(a);
}

void sendSpiData(t_spi_data* pArg,uint16_t numbs) {
	sendDataSpi(pArg,numbs);
}

void sendSpiFlashByte(t_spi_data pArg) {
	sendByteSpi(pArg);
}

t_spi_data getSpiByte(){
	return (getByteSpi());
}


void selectFlash25q(){
	selectFlashSpi();
}

void unselectFlash25q(){
	deselectFlashSpi();
}

void initFlash25q(){
	unselectFlash25q();
	HAL_Delay(100);
	setResetModeFlash25q();
	doResetFlash25q();
	uint8_t pVoid[3];
	getJedecIdFlash25q(pVoid);
}

void setResetModeFlash25q() {
	selectFlash25q();
	sendSpiFlashByte(FLASH25Q_COMMAND_ENABLE_RESET);
}

void doResetFlash25q() {
	selectFlash25q();
	sendSpiFlashByte(FLASH25Q_COMMAND_RESET);
	unselectFlash25q();
}

void freeSpiFlash25q(){
	unselectFlash25q();
}


uint8_t getStatusRegFlash25q(spi_status_reg_t register_num) {
	selectFlash25q();
	uint8_t retStatus=0;
	if (register_num == SPIFLASH_REG1) {
		sendSpiFlashByte(FLASH25Q_COMMAND_READ_STATUS_REG_1);
		retStatus= getSpiByte();
	} else if (register_num == SPIFLASH_REG2) {
		sendSpiFlashByte(FLASH25Q_COMMAND_READ_STATUS_REG_2);
		retStatus= (uint8_t) getSpiByte();
	}
	return retStatus;
}

uint16_t getManufactIdFlash25q() {

	selectFlash25q();

	uint16_t temp;

	sendSpiFlashByte(FLASH25Q_COMMAND_MANUFACTURER_ID);
	sendSpiFlashByte(FLASH25Q_COMMAND_NOP);
	sendSpiFlashByte(FLASH25Q_COMMAND_NOP);
	sendSpiFlashByte(FLASH25Q_COMMAND_NOP);
	temp = getSpiByte();
	temp = temp * 256;
	temp |= getSpiByte();

	freeSpiFlash25q();
	return temp;
}

void getUniqueIdFlash25q(uint8_t *pRet) {

	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_UNIQUE_ID);
	sendSpiFlashByte(FLASH25Q_COMMAND_NOP);
	sendSpiFlashByte(FLASH25Q_COMMAND_NOP);
	sendSpiFlashByte(FLASH25Q_COMMAND_NOP);
	sendSpiFlashByte(FLASH25Q_COMMAND_NOP);

	pRet[0] = (uint8_t) getSpiByte();
	pRet[1] = (uint8_t) getSpiByte();
	pRet[2] = (uint8_t) getSpiByte();
	pRet[3] = (uint8_t) getSpiByte();
	pRet[4] = (uint8_t) getSpiByte();
	pRet[5] = (uint8_t) getSpiByte();
	pRet[6] = (uint8_t) getSpiByte();
	pRet[7] = (uint8_t) getSpiByte();

	freeSpiFlash25q();
}

void getJedecIdFlash25q(uint8_t *pRet) {
	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_JEDEC_ID);
	pRet[0] = getSpiByte();
	pRet[1] = getSpiByte();
	pRet[2] = getSpiByte();
	freeSpiFlash25q();
}

static void sendAddr24Flash25q(uint32_t aAddr){
	//	if(ID >= W25Q256)	sendSpiFlashByte((aAddr & 0xFF000000) >> 24);
	sendSpiFlashByte((aAddr/65536)%256);
	sendSpiFlashByte((aAddr/256)%256);
	sendSpiFlashByte(aAddr%256);
}

uint8_t readByteAddrFlash25q(uint32_t aAddr) {
	selectFlash25q();
	sendSpiFlashByte(FLASH25Q_COMMAND_READ_DATA);
	sendAddr24Flash25q(aAddr);
	uint8_t retByte=getSpiByte();
	unselectFlash25q();
	return retByte;
}

void readBlockDataFlash25q(uint32_t aAddr, uint8_t *data_ptr, uint32_t read_len) {

	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_READ_DATA);
	sendAddr24Flash25q(aAddr);

	for (uint32_t i = 0; i < read_len; i++) {
		data_ptr[i] = (uint8_t) getSpiByte();
	}
}

void writeStatusRegsFlash25q(uint8_t val_stat_reg_1, uint8_t val_stat_reg_2) {
	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_WRITE_STATUS_REGS);
	sendSpiFlashByte(val_stat_reg_1);
	sendSpiFlashByte(val_stat_reg_2);
}

void setEnableWriteFlash25q() {
	selectFlash25q();
		sendSpiFlashByte(FLASH25Q_COMMAND_WRITE_ENABLE);
	unselectFlash25q();
}

void setDisableWriteFlash25q() {
	selectFlash25q();
	sendSpiFlashByte(FLASH25Q_COMMAND_WRITE_DISABLE);
	unselectFlash25q();
}

uint8_t getBusyStatusFlash25q(){
#define S0BIT (1<<0)
	uint8_t val = getStatusRegFlash25q(SPIFLASH_REG1);
	return ( val & S0BIT );
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

static void waitEndWriteFlash25q(){
	uint8_t StatusRegister1;
	selectFlash25q();;
	do{
		sendSpiFlashByte(FLASH25Q_COMMAND_READ_STATUS_REG_1);
		StatusRegister1 = getSpiByte();
		sleep(1);
	}
	while((StatusRegister1 & BUSY_BIT_C0) == BUSY_BIT_C0);

	unselectFlash25q();
}

//isWriteEndFlash25q()
//sendAddr24Flash25q(aAddr);

void writeByteFlash25q( uint32_t aAddr , uint8_t aByte ){
	unselectFlash25q();
	waitEndWriteFlash25q();
	setEnableWriteFlash25q();

	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_PAGE_PROGRAMM);

	sendAddr24Flash25q(aAddr);

	sendSpiFlashByte(aByte);

	unselectFlash25q();

	waitEndWriteFlash25q();

}
/*
 * @arg aNumb - кол-во записываемых данных, !не должен вылазить за страницу вн. кэша вн. флеша (256-aOffset)
 */

void startWritePageFlash25q(uint8_t *pBuffer, uint32_t aPageAddr, uint32_t aNumb)
{

//	if(((aNumb + aOffset) > PAGE_CACH_FLASH) || (aNumb == 0))
//		aNumb = PAGE_CACH_FLASH - aOffset;
//
//	if((aOffset + aNumb) > PAGE_CACH_FLASH)
//		aNumb = PAGE_CACH_FLASH - aOffset;
//

	waitEndWriteFlash25q();

	setEnableWriteFlash25q();

	selectFlash25q();;

	sendSpiFlashByte(FLASH25Q_COMMAND_PAGE_PROGRAMM);

//	aPageAddr = (aPageAddr * PAGE_CACH_FLASH) + aOffset;

	sendAddr24Flash25q(aPageAddr);

	sendSpiData( pBuffer, aNumb );

	unselectFlash25q();

	waitEndWriteFlash25q();

	sleep(1);
}


void eraseSector4kFlash25q(uint32_t aAddr_sector) {
	//ERASE THE SPECIFIED SECTOR (4KB)
	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_SECTOR_ERASE_4KB);
	sendAddr24Flash25q(aAddr_sector);
}

void eraseBlock32kFlash25q(uint32_t aBlock) {
	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_BLOCK_ERASE_32KB);
	sendAddr24Flash25q(aBlock);
}

void eraseBlock64kFlash25q(uint32_t aBlock) {
	selectFlash25q();

	sendSpiFlashByte(FLASH25Q_COMMAND_BLOCK_ERASE_64KB);
	sendAddr24Flash25q(aBlock);
}

void eraseAllChipFlash25q() {

	unselectFlash25q();

	setEnableWriteFlash25q();

	selectFlash25q();
	sendSpiFlashByte(FLASH25Q_COMMAND_CHIP_ERASE);
	unselectFlash25q();

	waitEndWriteFlash25q();

		sleep(10);
}

