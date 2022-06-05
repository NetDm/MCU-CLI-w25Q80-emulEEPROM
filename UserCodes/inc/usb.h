/*
 * usb.h
 *
 *  Created on: Aug 4, 2021
 *      Author: me
 */

#ifndef INC_USB_H_
#define INC_USB_H_

#ifdef STM32F1xx_LL_USB_H
#define USE_USB_FS_HAL


uint32_t rxUsbLen();

void resetRxLength();

extern uint16_t iRx;

int8_t CDC_Receive_FS_endpoint_callback(uint8_t* Buf, uint32_t *Len);

int isConnectorUSB(void);

void reconectSwUsb(void);

void offUsb(void);

#endif /* INC_USB_H_ */
#endif
