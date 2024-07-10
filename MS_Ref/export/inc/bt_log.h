/******************************************************************************
 ** File Name:      sio.h                                                     *
 ** Author:         Richard Yang                                              *
 ** DATE:           04/02/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This driver is for development board, Integrator/AP.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Richard.Yang     Create.                                   *
 ** 05/13/2002     Richard.Yang     Add sio_send_packet, sio_send_log_packet  *
 ** 05/30/2003     Eric.zhou        Add AT through channel server associated  *
 ** functions.                                                                    *
 ** 08/25/2003     Zhemin.Lin     Add MIX_MODE for COM_USER port, according MS00004213 *
 ** 11/03/2010    fei.zhang     Add BT USB Log function        *
 ******************************************************************************/

#ifndef _BT_LOG_H_
#define _BT_LOG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sio.h"
//#include "com_drvapi.h"
//#include "chip.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#define BT_LOG_SUBTYPE_TP   0x00 //0x10//0x50

#define BT_LOG_FRAME_BUFF_GET_SIZE         2000   //size get from share log mem per frame

#define BT_LOG_IDX_TYPE   2  //index step:  0->1 byte;  2->4 bytes(Dword)

#define MAX_SIO_BT_PPP_FRAME_SIZE   20
#define MAX_SIO_BT_PPP_FRAME_BUFF_SIZE  2048
#define DEBUG_PORT_BT_SEND_BUF_SIZE   61440   //60000 ==>30*2048 //for usb BT log DMA mode

#define DUMP_BT_MAX_BLK    (8)

typedef enum
{
    BT_LOG_OFF = 0,
    BT_LOG_UART1 = 1,
    BT_LOG_ARM_COM_DEBUG = 2
} BT_LOG_SEL_E;

typedef struct
{
    int32  start_addr;
    int32  len;  //dump len by BYTE.
} SIO_DUMP_BT_BLK_INFO;

typedef struct
{
    volatile uint32  TPWrite_idx;
    volatile uint32  TPRead_idx;
    volatile uint32  TPBuffBaseAddr;
    volatile uint32  TPBuffSize;

    volatile uint32  statusFlag;   
    volatile uint32  BTNvSet;      //BT log set   0:disable   1:Uart1     2:from ARM COM_Debug   3:from ARM + Uart1(only for BT debug use)
} SIO_BT_LOG_INFO_S;

typedef struct SIO_BT_FRAME_CTRL_Tag
{
    uint16 count;
    uint16 head;
    uint16 tail;
    uint16 list_size;
    DATA_FRAME_T  frame_list[MAX_SIO_BT_PPP_FRAME_SIZE];
} SIO_BT_FRAME_CTRL_T;

typedef struct
{
    uint32 seq_num;
    uint16 len;
    uint8  type;
    uint8  subtype;
} BTLOG_FRM_HEAD_T;

typedef struct BTLOG_BUF_Tag
{
	uint32 buf_len;
	uint8* buf;
}BTLOG_BUF_T;

/*****************************************************************************/
// Description: BT log task creat
// Global resource dependence: BT_LOG_semaphore, BT_LOG_thread_id, s_sio_BT_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_CreateBTLOGThread (void);

/*****************************************************************************/
// Description: Init  Head Info
// Global resource dependence: sio_BT_LOG_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_BT_LOG_Info_Notify (uint32 buff_info_address);

/*****************************************************************************/
// Description: set BT log from com_debug enable status
// Global resource dependence: is_BT_LOG_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void  SIO_BTLOGUSBEnableSet (
    BOOLEAN en  //[IN]set value
);


/*****************************************************************************/
// Description: BT log ISR handler
// Global resource dependence: BT_LOG_semaphore
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_BT_ISR_Handler (
    uint32 int_num  //[IN]interrupt number
);


/*****************************************************************************/
// Description: enable BT log save to T card
// Global resource dependence: BTlogfile_num, sfs_BT_LOG
// Author: jackey.ling
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_BTCardLogOpen (void);

/*****************************************************************************/
// Description: disable BT log save to T card
// Global resource dependence: sfs_BT_LOG
// Author: jackey.ling
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_BTCardLogClose (void);

/*begin add cm4 check bug:620209*/
/*****************************************************************************/
// Description: start check cm4 status
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_BTOpenCheckCm4Status (BOOLEAN open);
/*end add cm4 check bug:620209*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /*End BT_LOG.h*/
#endif

