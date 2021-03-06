/*************************************************************************
* WINBOND W25QXX FLASH CHIP LIBRARY
*
*	SUPPORT FOR ONLY NORMAL SPI MODE (NO BI/QUAD SPI)
* SPI DATA FORMAT : 8 BITS, MSB FIRST
*
*  Created on: Jun 1, 2022
*      Author: NetDm
*
* ***********************************************************************/


#ifndef _FLASH25Q_
#define	_FLASH25Q_

typedef uint8_t spi_data_t;

//todo применть weak замены функц, см стандарт gcc на них, подстановка макросов gcc вместо стандартных weak
//static t_spi_data (*_pfSpiGetSend)(t_spi_data);
//static void (*pfSpiCsLow)();
//static void (*pfSpiCsHigh)();

#define FLASH25Q_COMMAND_NOP				(0x00)
#define FLASH25Q_COMMAND_READ_DATA			(0x03)
#define FLASH25Q_COMMAND_WRITE_ENABLE		(0x06)

#define FLASH25Q_COMMAND_READ_STATUS_REG_1	(0x05)
#define FLASH25Q_COMMAND_READ_STATUS_REG_2	(0x35)

#define FLASH25Q_COMMAND_PAGE_PROGRAMM		(0x02)


#define FLASH25Q_COMMAND_ENABLE_RESET		(0x66)
#define FLASH25Q_COMMAND_RESET				(0x99)
#define FLASH25Q_COMMAND_MANUFACTURER_ID	(0x90)
#define FLASH25Q_COMMAND_UNIQUE_ID			(0x4B)
#define FLASH25Q_COMMAND_JEDEC_ID			(0x9F)
#define FLASH25Q_COMMAND_WRITE_STATUS_REGS	(0x01)
#define FLASH25Q_COMMAND_WRITE_DISABLE		(0x04)
#define FLASH25Q_COMMAND_SECTOR_ERASE_4KB	(0x20)
#define FLASH25Q_COMMAND_BLOCK_ERASE_32KB	(0x52)
#define FLASH25Q_COMMAND_BLOCK_ERASE_64KB	(0xD8)
#define FLASH25Q_COMMAND_CHIP_ERASE			(0xC7)
#define FLASH25Q_COMMAND_FAST_READ_DATA     (0x0B)

#define BUSY_BIT_C0 (0x01)

typedef enum {
	SPIFLASH_REG1=1
	,SPIFLASH_REG2=2
}spi_status_reg_t;

void selectFlash25q();
void unselectFlash25q();

void setResetModeFlash25q();
void doResetFlash25q();
uint8_t getStatusRegFlash25q(spi_status_reg_t register_num);
uint8_t getBusyStatusFlash25q();

void setEnableWriteFlash25q();
void setDisableWriteFlash25q();

uint16_t getManufactIdFlash25q();
void getUniqueIdFlash25q(uint8_t* ret_ptr);
void getJedecIdFlash25q(uint8_t* ret_ptr);


void writeStatusRegsFlash25q(uint8_t val_stat_reg_1, uint8_t val_stat_reg_2);

uint8_t readByteAddrFlash25q(uint32_t aAddr);
void readBlockDataFlash25q(uint32_t aAddr, uint8_t* data_ptr, uint32_t read_len);

void eraseSector4kFlash25q(uint32_t aAddr_sector);
void eraseBlock32kFlash25q(uint32_t aAddr_block);
void eraseBlock64kFlash25q(uint32_t aAddr_block);
void eraseAllChipFlash25q();

void writeByteFlash25q( uint32_t aAddr , uint8_t aByte );
bool_t writePageFlash25q( uint32_t aPageAddr , uint8_t* pBuffer , uint32_t aNumb);
bool_t writeSectorFlash25q(uint32_t aPageAddr , uint8_t* pBuffer , uint32_t aNumbs);


void initFlash25q();

void freeSpiFlash25q(); /// освобождает линию SPI flash

#endif
