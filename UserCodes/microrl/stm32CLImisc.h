#ifndef _MICRORL_MISC_H_
#define _MICRORL_MISC_H_

#include <stdint.h>

//#define HUART_CLI huart3

#define __SW_CLI_HTREAD  1

/*
Platform independent interface for implementing some specific function
for AVR, linux PC or ARM
*/

#define USB_TX_TIMEOUT	1000

// print to stream callback
void printCli (const char * str);
#ifdef HUART_CLI
// get_char from stream
char get_char (void);
uint16_t rxCharLen(void);
#endif
void getRxCDC (void);
void usbPush(void);
// execute callback
int execute_list_commands(int argc, const char * const * argv);
#ifdef _USE_COMPLETE
// completion callback
char ** complet (int argc, const char * const * argv);
#endif
// ctrl+c callback
void sigint (void);
//
uint8_t txUsbReady(void);
//
// public
//
// init platform
void initCLI(void);
// non blocked CLI command line interface thread, periodically call 1 each system pass
void CLIthread(void);
void deferredCliTask();

#endif
