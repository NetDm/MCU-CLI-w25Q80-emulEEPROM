/*
#include <CLIcommands.h>
 * cli_options.c
 *
 *  Created on: May 12, 2021
 *      Author: me
 */

#include <microrl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stm32CLImisc.h>
#include <usb.h>
#include "CLIcommands.h"

#include "w25Qxx.h"

exec_cmds_t list_cmds[]={
		{
				"help",
				help,
				"this command, for others use man",
				""
		}
		,{
				"clear",
				clear,
				"clear terminal screen",
				""
		}
		,{
				"list",
				list,
				"list all support commands",
				""
		}
		,{
				"man",
				man,
				"manual commands, use 'man *cmd*', example: man tpsm"
				""
		}
		,{
				"ver",
				catver,
				"cat version embedd FW"
		}

//		,{
//				"binfw",
//				binfw,
//				"Accepting a binary",
//				//man
//				" after entering a command and a response prompt '>'. \n\r"
//				"Format comands: binfw size crc32stm32\n\r"
//				"If parameter crc32stm32 is not specified, then the check is not performed\n\r\n"
//				"Data binary transmitted in blocks of 512 bytes, between which there is a mandatory pause for transfer from RAM to NVM"
//				"timeout break 2 seconds"
//		}
		
		
		,{
				"uid",
				uid,
				"get u16 manufacture unique id"
		}

		,{
				"&",
				prolongationsExec,
				"prolongations/separation command, need spase on both sides, exaple enter: clear & man tau",
				""
		}
		,{0,0,0}
};

/*
char szERROR[]	=CLBOLDRED "\n\rERROR" CLDEFAULTTEXT "\n\r";
char szWRITEOK[]=CLYELLOW "\n\rWRITEOK" CLDEFAULTTEXT "\n\r";
char szREAD[]	=CLGREEN "\n\rREAD:" CLDEFAULTTEXT " ";
char szOK[]		=CLGREEN "\n\rOK" CLDEFAULTTEXT "\n\r";
void respondOK(){
	printCli(szOK);
}
void respondWRITEOK(){
	printCli (szWRITEOK);
}
void respondREAD(){
	printCli (szREAD);
}
void respondERROR(){
	printCli(szERROR);
}
*/

void help(int argc, const char * const * argv){
	printCli ("microCLI library based shell v 1.1\n\r\n");
	int i=0;
	while (list_cmds[i].name[0]){
		printCli("[");
		printCli(list_cmds[i].name);
		printCli("] \t ");
		printCli(list_cmds[i].descriptions);
		printCli("\n\r");
		i++;
	}
	printCli ("When you enter a command without a parameter, "
			"the current set value is displayed.");
}
void clear(int argc, const char * const * argv){
	printCli ("\033[2J");    // ESC seq for clear entire screen
	printCli ("\033[H");     // ESC seq for move cursor at left-top corner
}
void list(int argc, const char * const * argv){
	int i=0;
	printCli(CLYELLOW);
	while (list_cmds[i].name[0]){
		printCli("'");
		printCli(list_cmds[i].name);
		printCli ("'\t");
		i++;
	}
	printCli(CLDEFAULTTEXT);
}

//									char isUsbReady()
//									{
//										usbPush();
//										return(txUsbReady());
//									}
/* EXAMPLE PARSE INT ARGUMENTS
void port(int argc, const char * const * argv){
	if (argc!=0){
		int port=atoi(argv[0]);
		if ((port>=0)&&(port<=65535)){
			setServerPort(port);
			respondWRITEOK();
		}else{
			respondERROR();
			printCli("Values out range from 0 to 65535\n\r");
		}
	}
	respondREAD();
	printCli("brokerPort='");
	char str[6];
	utoa(getServerPort(),str,10);
	printCli(str);
	printCli("'");
	//заключить данные в ' так-же для серийника и прпр, тк регулярка может обработать недопринятые данные и закрыть порт
}
*/
void prolongationsExec( int argc , const char * const * argv ){
	execute_list_commands ( argc , &argv[0]);
}

void catver(int argc, const char *const* argv){
	printCli ("FwVersion:");
	printCli ( "Build: " __DATE__", " __TIME__ );
}

void man(int argc, const char *const* argv){
	int i=0;
	while (list_cmds[i].name[0]){
		if (strcmp(argv[0],list_cmds[i].name)==0){
			if (list_cmds[i].man[0]!=0){
				printCli(list_cmds[i].man);
			}else{
				printCli(list_cmds[i].descriptions);
			}
			printCli("\n\r");
			return;
		}
		i++;
	}
	printCli ("Command for man absent, exec: list\n\r");
}

/*
void binfw( int argc , const char * const * argv ){
#define FW_TIMEOUT_DATA 3000
	uint32_t timestamp=HAL_GetTick();
	uint32_t aCrc32;
	char crcEnable=0;
	int size=atoi(argv[0]);
	if (argc>1){
		aCrc32=atol(argv[1]);
		crcEnable=1;
	}
//	if (crcEnable) crc32begin();
	startSavingNewFw(size);
	waitLastNvmOperations();
	HAL_Delay(10);
	resetRxLength();
	printCli("ready>");
	isUsbReady();
	uint32_t i=0;
	uint32_t ii=0;
	do{
		if ( (HAL_GetTick()-timestamp) > FW_TIMEOUT_DATA ){
			printCli("timeout data flow");
			return;
		}

		uint32_t l;
		do{
			l=rxUsbLen();
			HAL_Delay(20);
		}while(l<rxUsbLen());
		resetRxLength();


		if (l>0){
			timestamp=HAL_GetTick();
			i+=l;
			if (i>size){
				printCli("Over size error");
				return;
			}

			if ( (ii+l)>BLOCK_FW_SIZE ){
				printCli("Block size error");
				return;
			}else{
				extern uint8_t UserRxBufferFS[];
				memcpy(((uint8_t*)getPtrBuffNvm()+ii),UserRxBufferFS,l);
				ii+=l;
				if ( (ii==BLOCK_FW_SIZE) || (i=size) ){
//					if (crcEnable){
//						crc32calculate(ii, getPtrBuffNvm());
//					}
					writeBuffNvmPage();
					if (i==size) {
						if (crcEnable){
							crc32begin();
							crc32calculate(size, (uint8_t*)FLASH_BANK_FW_LOAD);
							if (getLastCrc32()!=aCrc32){
								printCli("Crc no correct! binary break");
								return;
							}
						}
						if (isFwWriteComplite()==0){
							printCli("Size did not match! toggle break");
							return;
						}
						printCli ("FW successfully copied and restarted");
						isUsbReady();
						HAL_Delay(200);
						copyBank2toBank1andReset(size);
					}
					ii=0;
					printCli("next>");
					isUsbReady();
				}


			}

	//		printCli(Rx_tmp_buff);

		}else{
			HAL_Delay(10);
		}

	}while (i<size);
}
*/

void uid(int argc, const char *const* argv){
	uint8_t UId[8];
	GetUniqueIdFlash25q(UId);

	const char stroutmask[]= "Manufacture Uniqui UID SPI flash: %02x%02x%02x%02x%02x%02x%02x%02x\n\r";
	char strout[sizeof(stroutmask)+2];
	sprintf(strout, stroutmask, UId[0], UId[1], UId[2], UId[3], UId[4], UId[5],
			UId[6], UId[7]);
	printCli(strout);
}

//void binfw( int argc , const char * const * argv ){
//#define FW_TIMEOUT_DATA 3000
//	uint32_t timestamp=HAL_GetTick();
//	uint32_t aCrc32;
//	char crcEnable=0;
//	int size=atoi(argv[0]);
//	if (argc>1){
//		aCrc32=atol(argv[1]);
//		crcEnable=1;
//	}
////	if (crcEnable) crc32begin();
//	startSavingNewFw(size);
//	waitLastNvmOperations();
//	HAL_Delay(10);
//	resetRxLength();
//	printCli("ready>");
//	isUsbReady();
//	uint32_t i=0;
//	uint32_t ii=0;
//	do{
//
//void readBlockDataFlash25q(uint32_t add, uint8_t* data_ptr, uint32_t read_len);
//
