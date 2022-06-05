#include <stm32CLImisc.h>
#include "w25Qxx.h"

#include "userapp.h"

#include <main.h>

#include <string.h>
#include <stdlib.h>

#include <usb.h>



void userApp(void)
{

	initFlash25q();

	initCLI();

	dbug ("\033[2J");    // ESC seq for clear entire screen
	dbug ("\033[H");     // ESC seq for move cursor at left-top corner
	dbugnl("Build: " __TIME__ " , " __DATE__ "\n\r");

	while (1){

		//
		threadNVM25Q80();
		//

		//!! thread - 01
	#ifdef __SW_CLI_HTREAD
		CLIthread();
	#endif
	//!! thread - 02
		HAL_Delay(1);
		static char c;
		if ((++c & 127) == 127){
			  HAL_GPIO_TogglePin(fromMX(BLINK));
		}
	}//WHILE LOOP
}
