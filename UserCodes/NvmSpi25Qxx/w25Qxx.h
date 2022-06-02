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

typedef uint8_t t_spi_data;

//todo применть weak замены функц, см стандарт gcc на них, подстановка макросов gcc вместо стандартных weak
//static t_spi_data (*_pfSpiGetSend)(t_spi_data);
//static void (*pfSpiCsLow)(void);
//static void (*pfSpiCsHigh)(void);

#define FLASH25Q_COMMAND_NOP				(0x00)
#define FLASH25Q_COMMAND_ENABLE_RESET		(0x66)
#define FLASH25Q_COMMAND_RESET				(0x99)
#define FLASH25Q_COMMAND_MANUFACTURER_ID	(0x90)
#define FLASH25Q_COMMAND_UNIQUE_ID			(0x4B)
#define FLASH25Q_COMMAND_JEDEC_ID			(0x9F)
#define FLASH25Q_COMMAND_READ_STATUS_REG_1	(0x05)
#define FLASH25Q_COMMAND_READ_STATUS_REG_2	(0x35)
#define FLASH25Q_COMMAND_WRITE_STATUS_REGS	(0x01)
#define FLASH25Q_COMMAND_WRITE_ENABLE		(0x06)
#define FLASH25Q_COMMAND_WRITE_DISABLE		(0x04)
#define FLASH25Q_COMMAND_SECTOR_ERASE_4KB	(0x20)
#define FLASH25Q_COMMAND_BLOCK_ERASE_32KB	(0x52)
#define FLASH25Q_COMMAND_BLOCK_ERASE_64KB	(0xD8)
#define FLASH25Q_COMMAND_CHIP_ERASE			(0xC7)
#define FLASH25Q_COMMAND_READ_DATA			(0x03)
#define FLASH25Q_COMMAND_FAST_READ_DATA     (0x0B)

void setResetModeFlash25q(void);
void doResetFlash25q(void);
uint8_t getStatusRegFlash25q(uint8_t register_num);
uint8_t getBusyStatusFlash25q(void);

void setEnableWriteFlash25q(void);
void setDisableWriteFlash25q(void);

uint16_t getManufactIdFlash25q(void);
void GetUniqueIdFlash25q(uint8_t* ret_ptr);
void getJedecIdFlash25q(uint8_t* ret_ptr);

uint8_t readByteDataFlash25q(uint32_t add);
void readBlockDataFlash25q(uint32_t add, uint8_t* data_ptr, uint32_t read_len);

void writeStatusRegsFlash25q(uint8_t val_stat_reg_1, uint8_t val_stat_reg_2);

void eraseSector4kFlash25q(uint32_t add_sector);
void eraseBlock32kFlash25q(uint32_t add_block);
void eraseBlock64kFlash25q(uint32_t add_block);
void eraseAllChipFlash25q(void);

void initFlash25q(void);

#endif
