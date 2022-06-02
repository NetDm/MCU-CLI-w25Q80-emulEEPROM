//#include <sys/termios.h>
#include <CLIcommands.h>
#include <stdio.h>
#include <fcntl.h>
//#include <sys/ioctl.h>
#include <unistd.h> 
#include <string.h>
#include "src/config.h"
#include <microrl.h>
#include <main.h>
#include <stm32CLImisc.h>

#include "CLIcommands.h"

#ifdef USE_USB_FS_HAL
#include <usbd_def.h>
#include <usbd_cdc.h>
#include <usbd_cdc_if.h>
#include <usb.h>
#endif

#define RX_SIZE 4096*2

uint8_t bRx[RX_SIZE] = {0};
uint8_t bRxUsb[RX_SIZE] = {0};
#ifdef HUART_CLI
extern UART_HandleTypeDef HUART_CLI;
#endif
// create microrl object and pointer on it
microrl_t rl;
microrl_t * prl = &rl;
int cli_enable=0;
size_t p_old;

#ifndef __SW_CLI_HTREAD
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */

  /* USER CODE BEGIN Callback 1 */

	//!! ПРИОРИТЕТ ПОСТАВИТЬ НИЖЕ SYSTICK
  if (htim->Instance == TIM7) {
	  CLIthread();
  }
  /* USER CODE END Callback 1 */
}
#endif



//*****************************************************************************
//dummy function, no need on linux-PC
void initCLI(void){
#ifdef HUART_CLI
	HAL_UART_Receive_DMA(&HUART_CLI, bRx, RX_SIZE);
#endif
	// call init with ptr to microrl instance and print callback
	microrl_init (prl, printCli);
	// set callback for execute
	microrl_set_execute_callback (prl, execute_list_commands);

#ifdef _USE_COMPLETE
	// set callback for completion
	microrl_set_complete_callback (prl, complet);
#endif
	// set callback for Ctrl+C
	microrl_set_sigint_callback (prl, sigint);

//	extern exec_cmds_t list_cmds[];
//	exec_cmds_t*** p_list_cmds;
//	p_list_cmds=&list_cmds;
//	setListComs(prl, p_list_cmds);
#ifndef __SW_CLI_HTREAD
	extern TIM_HandleTypeDef htim7;
	HAL_TIM_Base_Start_IT(&htim7);
#endif
	cli_enable=1;
};

//*****************************************************************************
// non blocked CLI command line interface thread, periodically call 1 each system pass
void CLIthread(){
	if  (cli_enable) {
		#ifdef USE_USB_FS_HAL
		if (rxUsbLen()!=0){
			getRxCDC();
		}
		#endif
		#ifdef HUART_CLI
		if (rxCharLen()!=0) {
			microrl_insert_char (prl, get_char());
		}
		#endif
	#ifdef USE_USB_FS_HAL
	deferredCliTask();
	#endif
	}
}

#ifdef USE_USB_FS_HAL
void deferredCliTask(){
	usbPush();
}
#endif

#ifdef HUART_CLI
uint16_t rxCharLen(){ //проверяет наличие байтов, не 0 - можно читать, побайтно
	uint16_t p_rx = RX_SIZE - HUART_CLI.hdmarx->Instance->CNDTR;
	uint16_t dataSizeReady=(p_rx - p_old);
	return dataSizeReady;
}
#endif

#ifndef HUART_CLI
#ifndef USE_USB_FS_HAL
#warning CLI без обратного вывода
} //выше функция сломается!!
#endif
#endif


#ifdef USE_USB_FS_HAL
uint8_t txUsbReady(){
	extern USBD_HandleTypeDef hUsbDeviceFS;
	USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
	return (hcdc->TxState);
}
extern uint8_t UserTxBufferFS[];
static uint8_t* dataTx=UserTxBufferFS;
static uint16_t lenUsb=0;

void usbPush(void){
	extern USBD_HandleTypeDef hUsbDeviceFS;
	USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
	if ((hcdc->TxState == 0)&&(lenUsb!=0)){
			CDC_Transmit_FS(UserTxBufferFS,lenUsb);//перезаписываем длинну
			lenUsb=0;
			dataTx=UserTxBufferFS;
	}
}

extern uint8_t UserRxBufferFS[];
void getRxCDC(void)
{
	uint32_t len=rxUsbLen();
//	USBD_CDC_ReceivePacket
	//todo в случае быстрой надежной передачи, требуется проверка на занятость
	int i=0;
	while(len--)
		microrl_insert_char (prl, UserRxBufferFS[i++]);
	resetRxLength();
}

#endif

//*****************************************************************************
// print callback for microrl library
void printCli(const char * str)
{
	size_t len=strlen(str);
#ifdef USE_USB_FS_HAL
	extern USBD_HandleTypeDef hUsbDeviceFS;
	if (hUsbDeviceFS.pClassData!=NULL){
		int timestamp=HAL_GetTick();
//		if ((lenUsb+len)>=APP_TX_DATA_SIZE){
//			while(txUsbReady()&&( (HAL_GetTick()-timestamp) < APP_TX_DATA_SIZE ) );
//			HAL_Delay(10);
//			lenUsb=0;
//		}
		if (lenUsb<APP_TX_DATA_SIZE){ //защита от переполнения Tx Usb
			memcpy(dataTx,str,len);//если занят передачей, добить
			dataTx+=len;
			lenUsb+=len;
		}
	}
#endif

#ifdef HUART_CLI
#warning	//todo tx  HUART_CLI.hdmatx->Instance->CNDTR;
//    while ( (HAL_UART_GetState(&HUART_CLI)==HAL_UART_STATE_BUSY_TX_RX));
//    HAL_UART_Transmit_DMA(&HUART_CLI,(uint8_t*)str,strlen(str));
    HAL_UART_Transmit(&HUART_CLI,(uint8_t*)str,len,100);
#else
#warning CLI без обратного вывода
#endif
}

#ifdef HUART_CLI
//*****************************************************************************
// get char user pressed, no waiting Enter input
char get_char(void)
{
	char ret;
	while(rxCharLen()==0);
	ret=(char)bRx[p_old];
	p_old++;
	if (p_old == RX_SIZE){
		p_old = 0;
	}
	return ret;
}
#endif

//// definition commands word
//#define _CMD_HELP  "help"
//#define _CMD_CLEAR "clear"
//#define _CMD_LIST  "list"
//#define _CMD_LISP  "lisp" // for demonstration completion on 'l + <TAB>'
//#define _CMD_NAME  "name"
//#define _CMD_VER   "version"
//// sub commands for version command
//	#define _SCMD_MRL  "microrl"
//	#define _SCMD_DEMO "demo"
//
//#define _NUM_OF_CMD 6
//#define _NUM_OF_VER_SCMD 2

////available  commands
//char * keyworld [] = {_CMD_HELP, _CMD_CLEAR, _CMD_LIST, _CMD_NAME, _CMD_VER, _CMD_LISP};
//// version subcommands
//char * ver_keyworld [] = {_SCMD_MRL, _SCMD_DEMO};
//
//// array for comletion
//char * compl_world [_NUM_OF_CMD + 1];
//
//// 'name' var for store some string
//#define _NAME_LEN 8
//char name [_NAME_LEN];
//int val;


////*****************************************************************************
//void print_help ()
//{
//	print ("Use TAB key for completion\n\rCommand:\n\r");
//	print ("\tversion {microrl | demo} - print version of microrl lib or version of this demo src\n\r");
//	print ("\thelp  - this message\n\r");
//	print ("\tclear - clear screen\n\r");
//	print ("\tlist  - list all commands in tree\n\r");
//	print ("\tname [string] - print 'name' value if no 'string', set name value to 'string' if 'string' present\n\r");
//	print ("\tlisp - dummy command for demonstation auto-completion, while inputed 'l+<TAB>'\n\r");
//}

//*****************************************************************************
// execute callback for microrl library
// do what you want here, but don't write to argv!!! read only!!
int execute_list_commands(int argc, const char * const * argv)
{
	int i = 0;
	extern exec_cmds_t list_cmds[];
	void (*fexec)(int argc, const char * const * argv);
	int flagexec=0;
	// just iterate through argv word and compare it with your commands
	while (i < argc) {
		int ii= 0;
		while (list_cmds[ii].name[0]){
			if (strcmp(argv[i],list_cmds[ii].name)==0){
				fexec=list_cmds[ii].f_exec;
				int ac=argc-1-i;	//кол-во аргументов передаваемых командам
				if (( ac>0 )&&( argv[i+1][0]=='&' )) {	//для случая когда аргумент за командой &
					ac=0;
				}
				fexec(ac,&argv[i+1]);
				printCli ("\n\r");
				flagexec++;
				break;
			}
			ii++;
		}
		while (  (argv[i++][0]!='&') && (i<argc) );
	}
	if (flagexec==0){
		printCli ("Valid command absent!\n\r");
	}
	return 0;
}


//int execute (int argc, const char * const * argv)
//{
//	int i = 0;
//	// just iterate through argv word and compare it with your commands
//	while (i < argc) {
//		if (strcmp (argv[i], _CMD_HELP) == 0) {
//			print ("microrl library based shell v 1.0\n\r");
//			print_help ();        // print help
//		} else if (strcmp (argv[i], _CMD_NAME) == 0) {
//			if ((++i) < argc) { // if value preset
//				if (strlen (argv[i]) < _NAME_LEN) {
//					strcpy (name, argv[i]);
//				} else {
//					print ("name value too long!\n\r");
//				}
//			} else {
//				print (name);
//				print ("\n\r");
//			}
//		} else if (strcmp (argv[i], _CMD_VER) == 0) {
//			if (++i < argc) {
//				if (strcmp (argv[i], _SCMD_DEMO) == 0) {
//					print ("demo v 1.0\n\r");
//				} else if (strcmp (argv[i], _SCMD_MRL) == 0) {
//					print ("microrl v 1.2\n\r");
//				} else {
//					print ((char*)argv[i]);
//					print (" wrong argument, see help\n\r");
//				}
//			} else {
//				print ("version needs 1 parametr, see help\n\r");
//			}
//		} else if (strcmp (argv[i], _CMD_CLEAR) == 0) {
//			print ("\033[2J");    // ESC seq for clear entire screen
//			print ("\033[H");     // ESC seq for move cursor at left-top corner
//		} else if (strcmp (argv[i], _CMD_LIST) == 0) {
//			print ("available command:\n");// print all command per line
//			for (int i = 0; i < _NUM_OF_CMD; i++) {
//				print ("\t");
//				print (keyworld[i]);
//				print ("\n\r");
//			}
//		} else {
//			print ("command: '");
//			print ((char*)argv[i]);
//			print ("' Not found.\n\r");
//		}
//		i++;
//	}
//	return 0;
//}

#ifdef _USE_COMPLETE
//*****************************************************************************
// completion callback for microrl library
char ** complet (int argc, const char * const * argv)
{
	int j = 0;

	compl_world [0] = NULL;

	// if there is token in cmdline
	if (argc == 1) {
		// get last entered token
		char * bit = (char*)argv [argc-1];
		// iterate through our available token and match it
		for (int i = 0; i < _NUM_OF_CMD; i++) {
			// if token is matched (text is part of our token starting from 0 char)
			if (strstr(keyworld [i], bit) == keyworld [i]) {
				// add it to completion set
				compl_world [j++] = keyworld [i];
			}
		}
	}	else if ((argc > 1) && (strcmp (argv[0], _CMD_VER)==0)) { // if command needs subcommands
		// iterate through subcommand for command _CMD_VER array
		for (int i = 0; i < _NUM_OF_VER_SCMD; i++) {
			if (strstr (ver_keyworld [i], argv [argc-1]) == ver_keyworld [i]) {
				compl_world [j++] = ver_keyworld [i];
			}
		}
	} else { // if there is no token in cmdline, just print all available token
		for (; j < _NUM_OF_CMD; j++) {
			compl_world[j] = keyworld [j];
		}
	}

	// note! last ptr in array always must be NULL!!!
	compl_world [j] = NULL;
	// return set of variants
	return compl_world;
}
#endif

//*****************************************************************************
void sigint (void)
{
	printCli ("^C catched!\n\r");
}
