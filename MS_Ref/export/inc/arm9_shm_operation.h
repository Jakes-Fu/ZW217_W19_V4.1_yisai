/******************************************************************************
 ** File Name:      bsc_device.h                                                    *
 ** Author:                                                     *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE          	NAME             	DESCRIPTION                               *
 **                                   *                                *
 ******************************************************************************/
#ifndef ARM9_CM4_SHM_OPERATION_H
#define ARM9_CM4_SHM_OPERATION_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "isr_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum{
    SHM_DATA_WRITE,
    SHM_DATA_READ,
    SHM_MAX_CHANNEL
}SHM_CHANNEL_T;

typedef struct OS_INT_MSG_T
{
    unsigned int message[4];
} OS_INT_MSG_T;

#define BT_OS_MSG_START  (0x100)
typedef enum
{
    OS_START_SCHED_MSG = BT_OS_MSG_START,
    OS_STOP_SCHED_MSG,
    OS_INT_HANDLE_MSG 
    //OS_TIME_EVENT_MSG,
    //OS_EXTEND_EVM_MSG,
    //OS_EXTEND_TRIG_DATA_MSG
} CSR_OS_SCHED_MSG_E;

typedef enum
{
    OS_TIME_EVENT  = 0x1000,
    //OS_BACKGROUND_IRQ = 0x1001,
    //OS_MSG_TRANSPORT = 0x1002,  // no use for iAnywhere
    OS_MSG_EXTEND_EVM    = 0X1003,
    OS_MSG_EXTEND_TRIG_DATA = 0X1004,
#ifdef BT_TEST_SAVE_PCM_DATA	
    OS_MSG_SAVE_PCM_DATA
#endif	
    //OS_MSG_EXTEND_OPC_DATA  = 0x1005
} OS_INT_MSG_TYPE_E;

typedef struct ARM9SHM_SIG_tag
{
    SIGNAL_VARS
    uint32  sig_param;
}ARM9SHM_SIG_T;

typedef struct SHM_INTERRUPT
{
    uint32 int_num;
    TB_ISR func;
} SHM_INTERRUPT_T;

typedef struct SHM_HW_CONFIG
{
    uint8 *baseAddr[2];
    uint32 *fifoSize[2];
    SHM_INTERRUPT_T inter[2];
    uint32 max_size;
} SHM_HW_CONFIG_T;

#define     SHM_RX_BUF_MAX_SIZE         (11000)//(3000)
#define     SHM_TX_BUF_MAX_SIZE         (5000)

typedef struct
{
	uint8	*tx_buf;
	uint32	tx_numbers;
	uint32	tx_out_ptr;
	uint32	tx_in_ptr;
	BOOLEAN	tx_is_full;
	BOOLEAN	tx_is_busy;
}SHM_Write_INFO_S;

typedef struct
{
	uint8	*rx_buf;
	uint32	rx_numbers;
	uint32	rx_out_ptr;
	uint32	rx_in_ptr;
	BOOLEAN	b_need_rx_data;
	SCI_TIMER_PTR shm_data_read_timer;
}SHM_Read_INFO_S;

typedef struct
{
    SHM_Write_INFO_S	write_info;
    SHM_Read_INFO_S	read_info;
    SHM_HW_CONFIG_T	*p_hw;
} SHM_CB_T;

#define SHM_LOG_TRACE_ON				0

#if	SHM_LOG_TRACE_ON
#define SHM_LOG_TRACE(_x_)			SCI_TraceLow _x_
#else
#define SHM_LOG_TRACE(_x_)			((void)0)
#endif /* SHM_LOG_TRACE_ON */

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    Arm9&Cm4 Share Memory Init.
// PUBLIC resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
PUBLIC void Shm_Data_Init(void);

/*****************************************************************************/
// Description :  API interface,user can write data to cache
// PUBLIC resource dependence : 
// PARAMETERS: buffer:write data;length:;write length
// Return: 0:write fail,cache full need to send again.
// Author :         
// Note :
/*****************************************************************************/
PUBLIC uint16 Shm_Write(const uint8 *buffer, uint16 length);

/*****************************************************************************/
// Description :  API interface,user can use to read data from cache
// PUBLIC resource dependence : 
// PARAMETERS:buffer:to store read data;length:read length
// Return: return had read data length
// Author :         
// Note :
/*****************************************************************************/
PUBLIC uint32 Shm_Read(void);//(uint8 *buffer,uint32 length);

/*****************************************************************************/
// Description :    Arm9&Cm4 Share Memory Close.
// PUBLIC resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
PUBLIC void Shm_Close(void);

/*****************************************************************************/
// Description :    Arm9&Cm4 Share Memory Open.
// PUBLIC resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
PUBLIC void Shm_Open(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif /* ARM9_CM4_SHM_OPERATION_H */

