/******************************************************************************
 ** File Name:      cardlog_uart.h                                                 *
 ** Author:                                                      *
 ** DATE:                                                           *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:           *
 **                                                        *
 **                                                                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **                                                                                                *
 ******************************************************************************/

#ifndef _CARDLOG_UART_H_
#define _CARDLOG_UART_H_

PUBLIC  uint32 CARDLOG_Init (uint32 port_num, UART_INIT_PARA_T *config,UartCallback callback);
PUBLIC uint32 CARDLOG_Close (uint32 port_num);
PUBLIC uint32 CARDLOG_GetFifoCnt (uint32 phy_port);
PUBLIC uint32 CARDLOG_Read (uint32 port_num,uint8 *buffer,uint32 length);
PUBLIC uint32  CARDLOG_Write (uint32 port_num,const uint8 *buffer,uint32 length);
PUBLIC int32 CARDLOG_DumpGetChar (uint32 phy_port);
PUBLIC void CARDLOG_DumpPutChar (uint32 phy_port, uint8 nchar);
PUBLIC void CARDLOG_Tx_Enable (uint32 phy_port,BOOLEAN is_enable);
PUBLIC void CARDLOG_Rx_Enable (uint32 phy_port,BOOLEAN is_enable);
PUBLIC void CARDLOG_Flush (uint32 phy_port);


#endif //_CARDLOG_UART_H_
