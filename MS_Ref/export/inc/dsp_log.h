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
 ** 11/03/2010    fei.zhang     Add DSP USB Log function        *
 ******************************************************************************/

#ifndef _DSP_LOG_H_
#define _DSP_LOG_H_

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

#define DSP_LOG_SUBTYPE_TP                 0
#define DSP_LOG_SUBTYPE_MEMDATA     1
#define DSP_LOG_SUBTYPE_DUMPDATA   2
#define DSP_LOG_FRAME_BUFF_GET_SIZE         2000   //size get from share log mem per frame
#define DSP_LOG_STATUS_TP_FORCE_BIT    (1<<0)  // dsp tp data force flush flag bit
#define DSP_LOG_STATUS_MEM_FORCE_BIT    (1<<1)  // dsp mem data force flush flag bit
#define DSP_LOG_STATUS_ASSERT_BUSY_BIT    (1<<7)  //this bit shows dsp in assert mode and preparing log data for assert,arm should wait 

#define DSP_LOG_TP_DEFAULT_SIZE  (75*1024)    //dsp log TP data default buffer size
#define DSP_LOG_MEM_DEFAULT_SIZE  (25*1024)  //dsp log MEM data default buffer size
#define DSP_LOG_MEM_RSV_SIZE   (8*1024)  //dsp log mem reserved size,when valid size > this value,arm can send out the mem data

#define DSP_LOG_IDX_TYPE   2  //index step:  0->1 byte;  2->4 bytes(Dword)
#define DSP_ARM_ADD_OFFSET  0xC0000000
#define DSP_SIDE_START_ADDR 0x10000000

#define MAX_SIO_DSP_PPP_FRAME_SIZE   30
#define MAX_SIO_DSP_PPP_FRAME_BUFF_SIZE  2048
#define DEBUG_PORT_DSP_SEND_BUF_SIZE   61440   //60000 ==>30*2048 //for usb DSP log DMA mode

#define DUMP_DSP_MAX_BLK    (8)

typedef enum
{
    DSP_LOG_OFF = 0,
    DSP_LOG_UART1 = 1,
    DSP_LOG_ARM_COM_DEBUG = 2
} DSP_LOG_SEL_E;

typedef struct
{
    int32  start_addr;
    int32  len;  //dump len by BYTE.
} SIO_DUMP_DSP_BLK_INFO;

typedef struct
{
    volatile uint32  TPWrite_idx;
    volatile uint32  TPRead_idx;
    volatile uint32  TPBuffBaseAddr;
    volatile uint32  TPBuffSize;
    volatile uint32  memDataWrite_idx;
    volatile uint32  memDataRead_idx;
    volatile uint32  memDataBuffBaseAddr;
    volatile uint32  memDataBuffSize;

    volatile uint32  statusFlag;    //bit 0:TP Force Flush   bit 1:MEM force flush  bit 7: DSP Assert Busy
    volatile uint32  dspNvSet;      //DSP log set   0:disable   1:Uart1     2:from ARM COM_Debug   3:from ARM + Uart1(only for DSP debug use)
    volatile uint32  dumpDspBlkNum;//D¨¨¨°adump¦Ì?dsp¨ºy?Y?¨¦¨ºy??.

    SIO_DUMP_DSP_BLK_INFO dump_dsp_blk[DUMP_DSP_MAX_BLK];
} SIO_DSP_LOG_INFO_S;

typedef struct SIO_DSP_FRAME_CTRL_Tag
{
    uint16 count;
    uint16 head;
    uint16 tail;
    uint16 list_size;
    DATA_FRAME_T  frame_list[MAX_SIO_DSP_PPP_FRAME_SIZE];
} SIO_DSP_FRAME_CTRL_T;

typedef struct
{
    uint32 seq_num;
    uint16 len;
    uint8  type;
    uint8  subtype;
} DSPLOG_FRM_HEAD_T;

typedef struct DSPLOG_BUF_Tag
{
	uint32 buf_len;
	uint8* buf;
}DSPLOG_BUF_T;

/*****************************************************************************/
// Description: Dsp log task creat
// Global resource dependence: dsp_log_semaphore, dsp_log_thread_id, s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_CreateDSPLogThread (void);

/*****************************************************************************/
// Description: Init  Head Info
// Global resource dependence: sio_dsp_log_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DSP_LOG_Info_Notify (void);

/*****************************************************************************/
// Description: set dsp log from com_debug enable status
// Global resource dependence: is_dsp_log_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void  SIO_DspLogUSBEnableSet (
    BOOLEAN en  //[IN]set value
);

/*****************************************************************************/
// Description: dsp log ISR handler
// Global resource dependence: dsp_log_semaphore
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DSP_ISR_Handler (
    uint32 int_num  //[IN]interrupt number
);

/*****************************************************************************/
// Description: Dump Dsp Data (include TP and MemData) in assert mode
// Global resource dependence: s_sio_dsp_frame_ctrl, s_sio_tx_dma_is_start, d
//                                            spLogSendContinueCnt, sio_dsp_log_headInfo_ptr, sio_port
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DumpDSPData (void);

/*****************************************************************************/
// Description: Dump Dsp Memory in assert mode
// Global resource dependence: s_sio_dsp_frame_ctrl, s_sio_tx_dma_is_start, d
//                                            spLogSendContinueCnt, sio_dsp_log_headInfo_ptr, sio_port
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DumpDSPMemory (void);

/*****************************************************************************/
// Description: enable dsp log save to T card
// Global resource dependence: dsplogfile_num, sfs_dsp_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_DSPCardLogOpen (void);

/*****************************************************************************/
// Description: disable dsp log save to T card
// Global resource dependence: sfs_dsp_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_DSPCardLogClose (void);

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}

#endif /*End dsp_log.h*/

