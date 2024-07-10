#ifndef _DUNCOM_API_H_
#define _DUNCOM_API_H_

/******************************************************************************
 *
 *   File Name:      duncom_api.h                                               
 *   Author:         Eddie Li                                                  
 *   DATE:           11/16/2007                                               
 *   Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
 *   Description:    This file defines the basic operation interfaces of
 *                   virtual DUN COM.        
 *                                   
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_Initialize
 *
 *  PARAM:
 *      port_num : Port Number.
 *      para     : UART initialize parameter.
 *      fun      : UART's callback funtion.
 *       
 *  DESCRIPTION
 *      Initialize the vitual DGCOM .
 *
 *  RETURNS
 *      UART_SUCCESS : Initialize DG COM successfully.
 *
 *----------------------------------------------------------------------------*/
PUBLIC uint32 DGCOM_Initialize(uint32 port_num, 
                               UART_INIT_PARA_T * para, 
                               UartCallback func);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_Close
 *
 *  PARAM:
 *      port_num : Port Number.
 *       
 *  DESCRIPTION
 *      Close the vitual DGCOM .
 *
 *  RETURNS
 *       UART_SUCCESS : Close DG COM successfully.
 *
 *----------------------------------------------------------------------------*/                               
PUBLIC uint32 DGCOM_Close(uint32 port_num);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_GetTxFifoCnt
 *
 *  PARAM:
 *      phy_port : Physical port.
 *       
 *  DESCRIPTION
 *      Get the virtual DGCOM's fifo size.
 *
 *  RETURNS
 *      fifo_cnt : Fifo size of the virtual DGCOM.
 *
 *----------------------------------------------------------------------------*/
PUBLIC uint32 DGCOM_GetTxFifoCnt(uint32 phy_port);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_ReadData
 *
 *  PARAM:
 *      port_num (i) : Port Number.
 *      buffer   (o) : The data read from the DGCOM should be written to the buffer.
 *      length   (i) : The max buffer size. 
 *       
 *  DESCRIPTION
 *      Read data from the virtual DGCOM.
 *
 *  RETURNS
 *      count : Acctual read data length
 *
 *----------------------------------------------------------------------------*/
PUBLIC uint32 DGCOM_ReadData(uint32 port_num,        
                                             uint8* buffer,
                                             uint32 length);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_WriteData
 *
 *  PARAM:
 *      port_num (i) : Port Number.
 *      buffer   (i) : The data that should be write to DG COM.
 *      length   (i) : The max buffer size. 
 *       
 *  DESCRIPTION
 *      Write data to the virtual DGCOM.
 *
 *  RETURNS
 *      count : Acctual write data length
 *
 *----------------------------------------------------------------------------*/
PUBLIC uint32 DGCOM_WriteData(uint32 port_num, 
                          const uint8* buffer,
                           uint32 length);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_DumpGetChar
 *
 *  PARAMETERS:
 *      phy_port : Physical port.
 *
 *  DESCRIPTION
 *      Dump a char from the DG COM.
 *
 *  RETURNS
 *      Character that read from DG COM.
 *
 *----------------------------------------------------------------------------*/
PUBLIC int32 DGCOM_DumpGetChar(uint32 phy_port);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_DumpPutChar
 *
 *  PARAMETERS:
 *      phy_port : Physical port.
 *
 *  DESCRIPTION
 *      put a char to the DG COM.
 *
 *  RETURNS
 *      N/A.
 *
 *----------------------------------------------------------------------------*/
PUBLIC void DGCOM_DumpPutChar(uint32 phy_port, uint8 nchar);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_Tx_Int_Enable
 *
 *  PARAMETERS:
 *      phy_port : Physical port.
 *      is_enable: TRUE, enable the interrupt and FALSE, disable the interrupt.
 *
 *  DESCRIPTION
 *      When SIO module has data to send, it should enable the Tx Interrupt.
 *
 *  RETURNS
 *      N/A
 *
 *----------------------------------------------------------------------------*/
PUBLIC void DGCOM_Tx_Int_Enable(uint32 phy_port,BOOLEAN is_enable);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_Rx_Int_Enable
 *
 *  PARAMETERS:
 *      phy_port : Physical port.
 *      is_enable: TRUE, enable the interrupt and FALSE, disable the interrupt.
 *
 *  DESCRIPTION
 *      When SIO module want recieve the data, it should enable the rx Interrupt.
 *
 *  RETURNS
 *      N/A
 *
 *----------------------------------------------------------------------------*/
PUBLIC void DGCOM_Rx_Int_Enable(uint32 phy_port,BOOLEAN is_enable);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DGCOM_Flush
 *
 *  PARAMETERS:
 *      port_num : Port Number.
 *
 *  DESCRIPTION
 *      Flush the fifo.
 *
 *  RETURNS
 *      N/A
 *
 *----------------------------------------------------------------------------*/
PUBLIC void DGCOM_Flush(uint32 port_num);
                             
#endif /* _DUNCOM_API_H_ */

