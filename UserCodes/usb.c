/*
 * usb.c
 *
 *  Created on: Aug 4, 2021
 *      Author: me
 */
#include "usb.h"
#ifdef USE_USB_FS_HAL

#include <main.h>
#include <usbd_def.h>
#include "usbd_cdc_if.h"

#include <stdint.h>

uint16_t iRx=0;
extern uint8_t UserRxBufferFS[];
extern USBD_HandleTypeDef hUsbDeviceFS;

uint32_t rxUsbLen(){
	return iRx;
}

void resetRxLength(){
	iRx=0;
	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
}
/*
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
#include <usb.h>
	return CDC_Receive_FS_endpoint_callback(Buf, Len);
}
 */

int8_t CDC_Receive_FS_endpoint_callback(uint8_t* Buf, uint32_t *Len)
{
	uint32_t len=(*Len);
	if ( (iRx+len) <= APP_RX_DATA_SIZE ) {
		USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[iRx]);
		USBD_CDC_ReceivePacket(&hUsbDeviceFS);
		iRx+=len;
		//
		return (USBD_OK);
	}
	else
	{
		return (USBD_FAIL);
	}
}

#include <usbd_def.h>
//#include <stm32l1xx_hal_pcd.h>
//	//link power manager - not work
//extern PCD_HandleTypeDef hpcd_USB_FS;

void reconectSwUsb(){
#ifdef USB_FS_PULLUP_Pin
	  if(isConnectorUSB()){
		  HAL_GPIO_WritePin(fromMX(USB_FS_PULLUP),RESET);
		  HAL_Delay(1500);
		  HAL_GPIO_WritePin(fromMX(USB_FS_PULLUP),SET);
	  }
#endif
}

int isConnectorUSB(void){
#ifdef USB_VBUS3V3_INT
	return (HAL_GPIO_ReadPin(fromMX(USB_VBUS3V3_INT))==GPIO_PIN_SET);
#else
	return (hUsbDeviceFS.dev_state != USBD_STATE_SUSPENDED );
#endif
}
void offUsb(){
	USBD_DeInit(&hUsbDeviceFS);
}
#endif
