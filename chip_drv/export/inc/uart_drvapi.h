/******************************************************************************
 ** File Name:      uart_drvapi.h                                             *
 ** Author:         Zhengjiang.Lei                                            *
 ** DATE:           11/03/2005                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                      serrial device. It manages create, send, receive     *
 **                      interface of serrial port.                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/03/2005     Zhengjiang.Lei     Create.                                 *
 ** 07/04/2007     David.Jia       CR 82866, add UART_GetRxFifoCnt.           *
 ** 02/09/2008     Tao.Zhou        driver arrangement                         *
 ******************************************************************************/
#ifndef _UART_DRVAPI_H_
#define _UART_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "com_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define UART_ALREADY_OPEN  0x53494F49
#define UART_CALLBACK_HISR_CREATED 0xa3f4568a
#define PURGE_TX_CLEAR     (1)
#define PURGE_RX_CLEAR     (2)


typedef struct
{
    uint32  baud_rate;
    uint8   parity;
    uint8   stop_bits;
    uint8   byte_size;
    uint8   flow_control;
    UART_DS_CTRL_E    ds_wakeup_en;
    UART_DS_TYPE_E    ds_wakeup_type;

    uint16 tx_watermark; // 0~127B
    uint16 rx_watermark; // 0~127B

} UART_CONTROL_T, serial_info_t;

typedef struct
{
    // volatile UART_REG_S *uart;  /* hardware registers */
    uint32          open_flag;
    uint32          dslp_func_sts;
    uint32          phy_port;
    UART_CONTROL_T  dcb;
    UartCallback    callback; //callback function, which is called by users
} UART_INFO_S;


/*****************************************************************************/
//  Description:   Get the FIFO size of UART
//  Global resource dependence : NONE
//  Author:        Tao.Zhou
//  Note:          NONE
/*****************************************************************************/
PUBLIC  uint32 UART_HAL_FifoSize (uint32 port);


/*****************************************************************************/
//  Description:    This function used to initial uart i/o pin interrupt for
//                  awaking chip in deepsleep.
//  Author:         Zhengjiang.Lei
//  Note:
/*****************************************************************************/
PUBLIC void UartDSlp_Init (void);

#ifdef   __cplusplus
}
#endif
#endif //_UART_DRVAPI_H_


