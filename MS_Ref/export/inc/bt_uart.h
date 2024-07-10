/******************************************************************************
** File Name:      bt_uart.h
** Author:         xiangxin
** DATE:           2012/8/13
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.
** Description:
******************************************************************************/

#ifndef BT_UART_H_
#define BT_UART_H_

#include "sci_types.h"

extern void UartDrv_SetUartId(uint8 id);

extern void UartDrv_Configure(uint32 baudrate);

extern void UartDrv_Rx(void);

extern uint16 UART_Write(const uint8 *buffer, uint16 length);

extern void UartDrv_Stop(void);

extern BOOLEAN UartDrv_Start(void);


#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
void UART_SetControllerBqbMode(BOOLEAN is_ctrlr_bqb_mode);
BOOLEAN UART_GetControllerBqbMode(void);

void UART_Controller_Bqb_Write(const uint8 *buffer, uint16 length);
#endif


#endif /* BT_UART_H_ */

