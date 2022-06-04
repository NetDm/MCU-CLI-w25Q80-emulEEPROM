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

#include "NVM25Q80.h"

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
				"manual commands, use 'man *cmd*', example: man list"
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

		,{
				"reg",
				reg,
				"Выводит состояние одного из статусного регистра"
				"CONTROL AND STATUS REGISTERS: The Read Status Register-1 and Status Register-2"
				"пример команд: 'getreg 1' или 'getreg 2'"
				,""
		}
		,{
				"eraseAll",
				eraseAll,
				"Стирает всю SPI flash"
				,""
		}
		,{
				"jedec",
				jedec,
				"Выводит winbond JedecID\n\r"
				"(page 54, https://static.chipdip.ru/lib/463/DOC002463585.pdf)"
				,""
		}
		,{
				"cathex",
				cathex,
				"Выводит содержимое сер. флешки в шеснадцатиричном формате, формат ввода: 'cathex <addr> <len>', "
				" где адрес и длинна вводятся в 10м формате"
				" Пример ввода: cathex 10 18 - выводит 18 байт начиная с 10(дес) адреса"
				,""
		}
		,{
				"cat",
				cat,
				"Выводит содержимое сер. флешки в ASCII формате, формат ввода: 'cat <addr> <len>', "
				" где адрес и длинна вводятся в 10м формате"
				" Пример ввода: cat 10 18 - выводит 18 байт начиная с 10(дес) адреса"
				,""
		}
		,{
				"echo",
				echo,
				"Записывает содержимое в сер. флешку в ASCII формате, формат ввода: 'echo <addr> <len> <str>',"
				" адрес и длинна вводятся в 10м формате. "
				" Пример ввода: echo 10 Я здесь был! - запишет строку 'Я здесь был!' начиная с 10 адреса"
				" len не может быть больше остатка от деления на 256 - addr, работает толко со страницей"
				" данные не будут записаны в занятую страницу"
				,""
		}

		,{
				"writeRaw",
				writeRaw,
				"Перезаписывает содержимое в сер. флешку в RAW (bin) формате, формат ввода: "
				"'writeRaw <addr> <len><ENTER>' - для потоковой записи,"
				" терминал переключится в бинарный потоковый прием"
				" и запишет во флеш принятые байты, после приема <len> кол-ва, "
				"автоматически вернется в CLI redline терминал ввода"
				", адрес и длинна вводятся в 10м формате."
				"\n\r Вместо <ENTER> - можно передать символ с кодом 0D(hex) / 13(dec) "
				,""
		}




		,{0,0,0}
};

char szERROR[]	=CLBOLDRED "\n\rERROR ARGUMENTS\n\r" CLDEFAULTTEXT "\n\r";
/*
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
*/
void respondERROR(){
	printCli(szERROR);
}

void respondErrorNumersArg(){
	respondERROR();
	printCli(" Количество аргументов не соответствует команде \n\r");
}

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
	printCli ("Command for man absent, exec 'man man' and exec 'list'\n\r");
}

#include "w25q80def.h"

void uid(int argc, const char *const* argv){
	uint8_t UId[8];
	getUniqueIdFlash25q(UId);

	const char stroutmask[]= "Manufacture Uniqui UID SPI flash: %02x%02x%02x%02x%02x%02x%02x%02x\n\r";
	char strout[sizeof(stroutmask)+2];
	sprintf(strout, stroutmask, UId[0], UId[1], UId[2], UId[3], UId[4], UId[5],
			UId[6], UId[7]);
	printCli(strout);
}

void reg( int argc , const char * const * argv ){

	if (argc!=1){
		respondERROR();
		return;
	}
	uint8_t status;
	size_t i=atol(argv[0]);
	if ( (i==1) || (i==2) ){
		status=getStatusRegFlash25q(i);
	}else{
		respondERROR();
		return;
	}
	const char stroutmask[]= "Текущий статус Status Register-%d : %d\n\n\r";
	char strout[sizeof(stroutmask)+2];
	sprintf( strout , stroutmask , i , status );
	printCli(strout);

}

void eraseAll( int argc , const char * const * argv ){
	eraseAllChipFlash25q();
	printCli("\n\rChip erased complite!\n\r");
}


void cathex( int argc , const char * const * argv ){

	if (argc!=2){
		respondErrorNumersArg();
		return;
	}
	size_t addr=atol(argv[0]);
	if (addr>=ALL_SPI_FLASH_SIZE){
		printCli("Адрес задан вне допустимого диапазона\n\r");
		return;
	}
	size_t numbs=atoi(argv[1]);
	const char stroutmask[]= "Вывод содержимого внешней SPI flash по адресу %u в кол-ве %u байт в HEX(16):\n\n\r";
	char strout[sizeof(stroutmask)+2];
	sprintf(strout, stroutmask,addr,numbs );
	printCli(strout);


	while (  (0!=numbs--) && (addr<ALL_SPI_FLASH_SIZE) ){
		uint8_t bSrc = readByteAddrFlash25q(addr);
		sprintf(strout,"%02X ",bSrc);
		printCli(strout);
		if ((numbs&7)==7)printCli(" ");
		if ((numbs&31)==31)printCli("\n\r");
		addr++;
	}
	printCli("\n\r-- end --\n\r");
}

void jedec( int argc , const char * const * argv ){
	char pVoid[3];
	getJedecIdFlash25q(pVoid);

	const char stroutmask[]= "winbond JedecId SPI flash (page 54, https://static.chipdip.ru/lib/463/DOC002463585.pdf) \n\r"
			"Manufacturer ID (0xEF=25Q80): %02X\n\r"
			"Memory Type ID1: %+02X\n\r"
			"Capacity ID: %+02X \n\r";
	char strout[sizeof(stroutmask)+2];
	sprintf(strout, stroutmask, pVoid[2],pVoid[1],pVoid[0] );
	printCli(strout);

}

void cat( int argc , const char * const * argv ){

	if (argc!=2){
		respondErrorNumersArg();
		return;
	}
	size_t addr=atol(argv[0]);
	if (addr>=ALL_SPI_FLASH_SIZE){
		printCli("Адрес задан вне допустимого диапазона\n\r");
		return;
	}
	size_t numbs=atoi(argv[1]);
	const char stroutmask[]= "Вывод содержимого внешней SPI flash по адресу %u в кол-ве %u байт в ASCII:\n\n\r";
	char strout[sizeof(stroutmask)+2];
	sprintf(strout, stroutmask,addr,numbs );
	printCli(strout);

	while (  (0!=numbs--) && (addr<ALL_SPI_FLASH_SIZE) ){

		uint8_t bSrc = readByteAddrFlash25q(addr);
		sprintf(strout,"%c",bSrc);
		printCli(strout);
		addr++;
	}

	unselectFlash25q();

	printCli("\n\r-- end --\n\r");
	flushKeyboard();
}

void echo( int argc , const char * const * argv ){
	if (argc<2){
		respondErrorNumersArg();
		return;
	}
	size_t addr=atol(argv[0]);
	if (addr>=ALL_SPI_FLASH_SIZE){
		printCli("Адрес задан вне допустимого диапазона\n\r");
		return;
	}
	//собираем аргументы обратно в \0 терминированную строку
	int i=argc;
	while(--argc>1){
		char* src=argv[argc]-1;
		*src=32;
	}
	size_t numbs=strlen(argv[1]);
	if (  numbs  >  ( PAGE_CACH_FLASH - (addr%PAGE_CACH_FLASH) )  ){
		respondERROR();
		printCli("Данные выходят за границы страницы кэш флеш");
		return;
	}
	const char stroutmask[]= "Ввод содержимого во внешнюю SPI flash по адресу %u в кол-ве %u байт в ASCII: '";
	char strout[sizeof(stroutmask)+2];
	sprintf( strout , stroutmask , addr , numbs );
	printCli(strout);
	printCli(argv[1]);
	printCli("'\n\r");

	startWritePageFlash25q( argv[1] , addr , numbs );

	flushKeyboard();
}

bool_t returnCliByte(uint8_t* retByte){
	if (rxCharLen()!=0){
		uint8_t chars[2]={0,0};
		chars[0]=(uint8_t)get_char();
		printCli((char*)chars);
		return TRUE;
	}
	return FALSE;
}

void writeRaw( int argc , const char * const * argv ){
	if (argc!=2){
		respondErrorNumersArg();
		return;
	}
	size_t addr=atol(argv[0]);
	if ( addr >= ALL_SPI_FLASH_SIZE){
		respondERROR();
		printCli("Адрес задан вне допустимого диапазона");
		return;
	}
	size_t numbs=atol(argv[1]);
	if (  ( numbs + addr )  >=  ALL_SPI_FLASH_SIZE  ){
		respondERROR();
		printCli("Данные выйдут за границы флеш");
		return;
	}
	const char stroutmask[]= "Ввод из потока этого терминала содержимого во внешнюю SPI flash по адресу %u в кол-ве %u байт в RAW формате: '";
	char strout[sizeof(stroutmask)+2];
	sprintf( strout , stroutmask , addr , numbs );
	printCli(strout);
	printCli(argv[1]);
	printCli("'\n\r <<");

	nvm_t tNvm;
		tNvm.numbsWrite = numbs;
		tNvm.startAddrNvm = addr;
		tNvm.passCallback = returnCliByte;
	if ( startNvm(&tNvm) ) {
		while ( threadNVM25Q80() >= WAIT_NVM_DATA );	//внимание! автомат nvm вызывается отсюда,
		}												//пока не отработает
														//прочие задачи блокируются
														//сам будет вызывать функцию драйвера
														//для изъятия данных из потока
														//returnCliByte(..)
	else{
		printCli("err: процесс NVM не начался, не удовлетворительные параметры, 2 вн. эшелон проверки");
	}
	flushKeyboard();
	printCli("\n\r ------ end recive ------");
}
