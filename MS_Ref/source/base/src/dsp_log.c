/******************************************************************************
 ** File Name:      dsp_log.c                                                     *
 ** Author:         Fei Zhang                                              *
 ** DATE:           07/04/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/04/2010     fei.zhang        Add DSP USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "cmddef.h"
#include "dsp_log.h"
#include "ref_param.h"
#include "mem_prod.h"
#include "ref_outport.h"
#include "sfs.h"
#include "dsp_drvapi.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*-----------------------------SIO DEBUG MACOR-------------------------------*/

//#define DSP_TRACE
#ifdef DSP_TRACE
#define SIO_DSP_TRACE   SCI_TRACE_LOW
#else
#define SIO_DSP_TRACE
#endif
/*lint -save -e551*/

#define DSPLOG_DEFAULT_MAX_SIZE (50*1024*1024) /*50M*/

#define  TASK_DSP_LOG_PRI   PRI_DSP_LOG_TASK   //31

/*-----------------------------END--------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  Static Variables & functions
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **  external Variables
 **---------------------------------------------------------------------------*/
extern SCI_MUTEX_PTR   comm_mutex;
extern uint32 s_sio_tx_dma_is_start;
//extern uint32 armLogSendContinueCnt;  //arm log continue send out count,must be cleared to 0 when other log be sent out
extern uint32 dspLogSendContinueCnt;   //dsp log continue send out count,must be cleared to 0 when other log be sent out

/**---------------------------------------------------------------------------*
 **  Protocol functions
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  User port variables & functions
 **---------------------------------------------------------------------------*/
LOCAL uint8  sio_debug_dsp_send_buf[DEBUG_PORT_DSP_SEND_BUF_SIZE];// Debug port tx buffer
LOCAL  BOOLEAN  is_usb_data_enable = FALSE;  //judge if usb data is enable
LOCAL BLOCK_ID dsp_log_thread_id = SCI_INVALID_BLOCK_ID;
PUBLIC SCI_SEMAPHORE_PTR dsp_log_semaphore = SCI_NULL;
LOCAL uint32 sioPPPFrameSeqNum = 0;  //dsp log frame sequence num
LOCAL  BOOLEAN  is_dsp_log_usb_enable = FALSE;  //DSP LOG from ARM COM_DEBUG function enable flag
LOCAL uint8  sio_dsplog_trans_buf[2048+4] = {0};// dsp log trans temp buffer

PUBLIC SIO_DSP_LOG_INFO_S *sio_dsp_log_headInfo_ptr = SCI_NULL;  //point to dsp log head info
PUBLIC SIO_DSP_FRAME_CTRL_T    s_sio_dsp_frame_ctrl;  //DSP log buf information
PUBLIC uint32 *l1c_get_dsp_log_adr (void);
BOOLEAN b_dsp_log_enabled = TRUE;
LOCAL SFS_HANDLE  sfs_dsp_log = NULL; 
LOCAL uint16 dsplogfile_num = 0, deletefile_num = 0;
LOCAL uint32 dsplog_saved_size = 0, dsplog_stepup_size = DSPLOG_DEFAULT_MAX_SIZE;
DSPLOG_BUF_T dsplog_buf;
    
/*****************************************************************************/
// Description: get Frame sequence num for dsp log
// Global resource dependence: is_dsp_log_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL uint32 SIO_GetPPPFrameSeqNum (void)   //Returns the sequence num
{
    return sioPPPFrameSeqNum++;
}


/*****************************************************************************/
// Description: get dsp log from com_debug enable status
// Global resource dependence: is_dsp_log_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  SIO_IsDspLogUSBEnable (void) //Returns the enbale status
{
    return is_dsp_log_usb_enable;
}


/*****************************************************************************/
// Description: set dsp log from com_debug enable status
// Global resource dependence: is_dsp_log_usb_enable
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void  SIO_DspLogUSBEnableSet (
    BOOLEAN en  //[IN]set value
)
{
    is_dsp_log_usb_enable =  en;
}


/*****************************************************************************/
// Description: dsp log ISR handler
// Global resource dependence: dsp_log_semaphore
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DSP_ISR_Handler (
    uint32 int_num  //[IN]interrupt number
)
{
    * (volatile uint32 *) (GR_ICLR) |= 0x04; //clear DSP Irq1

    SIO_DSP_TRACE ("!!!!!!!!!!!!!!!!!!!!!SIO DSP ISR!!!!!!!!!!!!!!!!!!!");

	if(TRUE == b_dsp_log_enabled)	// If ARM Log On, turn on DSP USB LOG. If ARM Log Close, turn off DSP USB LOG
	{
    	SCI_PutSemaphore (dsp_log_semaphore);
	}

}


/*****************************************************************************/
// Description: Init  Head Info
// Global resource dependence: sio_dsp_log_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DSP_LOG_Info_Notify (void)
{
    MEM_REGION_T DSPLogBuff_region;

    MEM_GetDSPLogBuffRegion (&DSPLogBuff_region);

    sio_dsp_log_headInfo_ptr = (SIO_DSP_LOG_INFO_S *) (l1c_get_dsp_log_adr());

    sio_dsp_log_headInfo_ptr->TPBuffBaseAddr = (uint32) DSPLogBuff_region.start_add;
    sio_dsp_log_headInfo_ptr->TPBuffSize = DSP_LOG_TP_DEFAULT_SIZE >> DSP_LOG_IDX_TYPE;
    sio_dsp_log_headInfo_ptr->TPRead_idx = sio_dsp_log_headInfo_ptr->TPBuffSize - 1;
    sio_dsp_log_headInfo_ptr->TPWrite_idx = 0;

    sio_dsp_log_headInfo_ptr->memDataBuffBaseAddr = sio_dsp_log_headInfo_ptr->TPBuffBaseAddr + DSP_LOG_TP_DEFAULT_SIZE;
    sio_dsp_log_headInfo_ptr->memDataBuffSize = DSP_LOG_MEM_DEFAULT_SIZE>>DSP_LOG_IDX_TYPE;
    sio_dsp_log_headInfo_ptr->memDataRead_idx = sio_dsp_log_headInfo_ptr->memDataBuffSize - 1;
    sio_dsp_log_headInfo_ptr->memDataWrite_idx = 0;

    sio_dsp_log_headInfo_ptr->statusFlag = 0;

    sio_dsp_log_headInfo_ptr->dspNvSet = REFPARAM_GetDspDebugPortPhyNo();

}


/*****************************************************************************/
// Description: write usb log to 2K buffer
// Global resource dependence: s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL uint32 SIO_WriteDSPFrameToBuf ( //Returns the left data size which have not written to 2K buffer
    uint8 *src, //[IN]log addr
    uint32 size //[IN]log length
)
{
    register int    data_size = 0;

    uint8 head = s_sio_dsp_frame_ctrl.head;
    uint8 *buf = s_sio_dsp_frame_ctrl.frame_list[head].addr;
    uint16 buf_size = s_sio_dsp_frame_ctrl.frame_list[head].length;

    SCI_ASSERT (buf != NULL);/*assert verified*/
    SCI_ASSERT (src != NULL);/*assert verified*/

    if (s_sio_dsp_frame_ctrl.count >= s_sio_dsp_frame_ctrl.list_size)
    {
        return 0;
    }

    while (size != 0)
    {
        switch (*src)
        {
            case FLAG_BYTE:
            case ESCAPE_BYTE:
                buf[s_sio_dsp_frame_ctrl.frame_list[head].pos++] = ESCAPE_BYTE;

                if (s_sio_dsp_frame_ctrl.frame_list[head].pos > buf_size - 1)
                {
                    return size;
                }

                buf[s_sio_dsp_frame_ctrl.frame_list[head].pos++] = *src ^ COMPLEMENT_BYTE;

                if (s_sio_dsp_frame_ctrl.frame_list[head].pos > buf_size - 1)
                {
                    return size;
                }

                data_size += 2;

                break;

            default:
                buf[s_sio_dsp_frame_ctrl.frame_list[head].pos++] = *src;

                if (s_sio_dsp_frame_ctrl.frame_list[head].pos > buf_size - 1)
                {
                    return size;
                }

                data_size++;
                break;
        }

        src++;
        size--;
    }

    return size;
}


/*****************************************************************************/
// Description: write DSP log to 2K buffer
// Global resource dependence: comm_mutex, s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL uint32 SIO_WriteDSPFrame (//Returns the data size which have been written to DSP 2K Log Buffer
    uint8 *src,             //[IN]src-log addr
    uint32 size         //[IN]log length
)
{
    uint32 write_left_size = 0;
    uint8  head = 0;
    uint8 *buf = 0;
    uint16 buf_size =  0;
    uint16 start = 0;

    head = s_sio_dsp_frame_ctrl.head;
    buf = s_sio_dsp_frame_ctrl.frame_list[head].addr;
    buf_size =  s_sio_dsp_frame_ctrl.frame_list[head].length;
    start = 0;

    SCI_ASSERT (s_sio_dsp_frame_ctrl.count <= s_sio_dsp_frame_ctrl.list_size);/*assert verified*/
    SCI_ASSERT (buf != NULL);/*assert verified*/

    //SIO_DSP_TRACE("!!!SIO  DSP  a_ctu = %d, a_cnt = %d, d_ctu = %d ,d_cnt = %d",armLogSendContinueCnt,s_sio_frame_ctrl.count,dspLogSendContinueCnt,s_sio_dsp_frame_ctrl.count);

    //When the space is FULL, then throw it away
    if (s_sio_dsp_frame_ctrl.count >= s_sio_dsp_frame_ctrl.list_size)
    {
        //send log quickly!!
        if (comm_mutex)
        {
            SCI_GetMutex (comm_mutex,  SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
        }

        SIO_SendData();

        if (comm_mutex)
        {
            SCI_PutMutex (comm_mutex);
        }

        return 0;
    }

    s_sio_dsp_frame_ctrl.frame_list[head].pos = 0;

    buf[s_sio_dsp_frame_ctrl.frame_list[head].pos++] = FLAG_BYTE;  //start

    write_left_size = SIO_WriteDSPFrameToBuf (src,size);

    if (write_left_size > 0)   //frame buffer is full,return  encoded size
    {
        SIO_DSP_TRACE ("!!!DSP LOG Left > 0");

        return size - write_left_size;
    }

    SCI_MEMSET ( ( (uint8 *) &buf[s_sio_dsp_frame_ctrl.frame_list[head].pos]),\
                 FLAG_BYTE, \
                 (buf_size - s_sio_dsp_frame_ctrl.frame_list[head].pos));

    return  size;
}


/*****************************************************************************/
// Description: dsp log send out Triger in usb DMA mode
// Global resource dependence: comm_mutex, s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL void  SIO_DspLogSendOut (
    volatile uint32 *readidx_ptr,   //[IN]addr in share memory
    uint32 read_idx                 //[IN]set value
)
{

    if (comm_mutex)
    {
        SCI_GetMutex (comm_mutex,  SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
    }

    /*must disable irq*/
    SCI_DisableIRQ();
    s_sio_dsp_frame_ctrl.count++;
    s_sio_dsp_frame_ctrl.head++ ;

    if (s_sio_dsp_frame_ctrl.head >= s_sio_dsp_frame_ctrl.list_size)
    {
        s_sio_dsp_frame_ctrl.head = 0;
    }

    *readidx_ptr = read_idx;   //modify idx in share memory
    SCI_RestoreIRQ();

    SIO_SendData();

    if (comm_mutex)
    {
        SCI_PutMutex (comm_mutex);
    }
}



/*****************************************************************************/
// Description: Dump Dsp Data (include TP and MemData) in assert mode
// Global resource dependence: s_sio_dsp_frame_ctrl, s_sio_tx_dma_is_start, d
//                                            spLogSendContinueCnt, sio_dsp_log_headInfo_ptr, sio_port
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DumpDSPData (void)
{
    uint32 i = 0;
    uint32 retry = 0;

    uint32 tpReadIdx = 0;
    uint32 tpReadIdx_tmp = 0;
    uint32 tpWriteIdx = 0;
    uint32 tpSize = 0;
    uint32 tpSize_tmp = 0;
    uint32 tpBuffSize = 0;
    uint32 tpBuffBaseAddr = 0;

    uint32 memReadIdx = 0;
    uint32 memReadIdx_tmp = 0;
    uint32 memWriteIdx = 0;
    uint32 memDataSize = 0;
    uint32 memDataSize_tmp = 0;
    uint32 memBuffSize = 0;
    uint32 memBuffBaseAddr = 0;

    uint32 seqNum = 0;
    uint32 sendSize = 0;

    uint16 head = 0;
    uint16 tail = 0;
    uint8 *buf = NULL;
    uint16 list_size = 0;
    uint16 buf_cnt = 0;
    uint32 buf_size = 0;
    uint16 start = 0;
    uint16 list_idx = 0;
    uint16 buf_idx = 0;

    const uint32 maxSendSize = 1024;
    DSPLOG_FRM_HEAD_T  dspLogFrmHead = {0,0,0,0};
    uint32  dspLogFrameHeadLen = 0;
    uint8 *outPtr = NULL;

    if (!SIO_IsDspLogUSBEnable())
    {
        return;
    }

    head = s_sio_dsp_frame_ctrl.head;
    tail = s_sio_dsp_frame_ctrl.tail;
    list_size = s_sio_dsp_frame_ctrl.list_size;
    buf = s_sio_dsp_frame_ctrl.frame_list[head].addr;
    buf_size =  s_sio_dsp_frame_ctrl.frame_list[head].length;
    start =  s_sio_dsp_frame_ctrl.frame_list[head].pos;
    buf_cnt = s_sio_dsp_frame_ctrl.count;

    //when Assert occured during dma is acive, buf count -1
    if (s_sio_tx_dma_is_start &&  dspLogSendContinueCnt && buf_cnt)
    {
        buf_cnt--;
        tail++;
    }

    /*send out frame buffer remain data*/
    OS_DebugPutChar(0x7E);

    for (list_idx=0; list_idx<buf_cnt; list_idx++)
    {
        for (buf_idx=0; buf_idx<buf_size; buf_idx++)
        {
            OS_DebugPutChar((char) (* (s_sio_dsp_frame_ctrl.frame_list[ (tail+list_idx) %list_size].addr+buf_idx)));

        }

    }

    OS_DebugPutChar(0x7E);

    //∩辰?aarm ﹞??那dsp米?bus㏒?車?角∩dump dsp information
    //AHB_CTRL1:   BIT_16, ARM enables CEVAX matrix to sleep. This bit will automatically reset once ARM tries to access CEVAX matrix.
    //DSP_BOOT_EN: BIT_2,  ARM access DSP ASHB bridge enable, active high.
    * (volatile uint32 *) (AHB_CTL1) &= ~ (0x1<<16);

    for (i=0; i<100; i++) {}

    * (volatile uint32 *) (DSP_BOOT_EN) |= (0x1<<2);

    for (i=0; i<100; i++) {}

    /*send out share trace buffer remain data*/
    retry = 0;

    do
    {

        /******************************************************************
        **   |free        |-> baseAddr
        **   |free        |
        **   |reserve   |->readIdx
        **   |data 0     |->first data to be read
        **   |data 1     |
        **      .
        **      .
        **      .
        **   |data n-1 |
        **   |data n     |
        **   |free        |->writeIdx ,first free space could be Write
        **   |free        |
        **   |free        |
        **   reserve data is used for checking the buffer is full or empty
        **  when writeIdx==readIdx,buffer is full
        **      readIdx + 1== writeIdx,buffer is empty
        **  in DSP data uint is DWORD(4 bytes), the index unit is DWORD also
        **  in ARM DATA Process is byte by byte
        ***********************************************************************/

        tpBuffSize = sio_dsp_log_headInfo_ptr->TPBuffSize << DSP_LOG_IDX_TYPE;
        tpBuffBaseAddr = (sio_dsp_log_headInfo_ptr->TPBuffBaseAddr|DSP_ARM_ADD_OFFSET) - DSP_ARM_ADD_OFFSET + CHIP_GetDSPShareMemVirBaseAddr();
        tpReadIdx = ( (sio_dsp_log_headInfo_ptr->TPRead_idx + 1) << DSP_LOG_IDX_TYPE) % tpBuffSize;
        tpWriteIdx = (sio_dsp_log_headInfo_ptr->TPWrite_idx) << DSP_LOG_IDX_TYPE;

        tpSize = (tpWriteIdx + tpBuffSize - tpReadIdx) % tpBuffSize;
        tpSize_tmp = tpSize;
        tpReadIdx_tmp = tpReadIdx;

        memBuffSize = sio_dsp_log_headInfo_ptr->memDataBuffSize << DSP_LOG_IDX_TYPE;
        memBuffBaseAddr = (sio_dsp_log_headInfo_ptr->memDataBuffBaseAddr|DSP_ARM_ADD_OFFSET) - DSP_ARM_ADD_OFFSET + CHIP_GetDSPShareMemVirBaseAddr();
        memReadIdx = ( (sio_dsp_log_headInfo_ptr->memDataRead_idx + 1) << DSP_LOG_IDX_TYPE) % memBuffSize;
        memWriteIdx = (sio_dsp_log_headInfo_ptr->memDataWrite_idx) << DSP_LOG_IDX_TYPE;

        memDataSize = (memWriteIdx + memBuffSize - memReadIdx) % memBuffSize;
        memDataSize_tmp = memDataSize;
        memReadIdx_tmp = memReadIdx;

        while (tpSize > 0)
        {

            sendSize = tpSize>maxSendSize ? maxSendSize : tpSize;

            OS_DebugPutChar(0x7E);

            seqNum = SIO_GetPPPFrameSeqNum();

            dspLogFrameHeadLen = sizeof (DSPLOG_FRM_HEAD_T);

            dspLogFrmHead.seq_num = seqNum;
            dspLogFrmHead.len = (uint16) (sendSize + dspLogFrameHeadLen);
            dspLogFrmHead.type = DIAG_MOBILETV_DSP_LOG_F;
            dspLogFrmHead.subtype = DSP_LOG_SUBTYPE_TP;

            outPtr = (uint8 *) (&dspLogFrmHead);

            for (i=0; i<dspLogFrameHeadLen; i++)
            {
                if ( (outPtr[i] == 0x7E) || (outPtr[i] == 0x7D))
                {
                    OS_DebugPutChar(0x7D);
                    OS_DebugPutChar((outPtr[i] ^ 0x20));
                }
                else
                {
                    OS_DebugPutChar(outPtr[i]);
                }

            }

            outPtr = (uint8 *) (tpBuffBaseAddr);

            for (i=0; i<sendSize; i++)
            {
                if ( (outPtr[tpReadIdx] == 0x7E) || (outPtr[tpReadIdx] == 0x7D))
                {
                    OS_DebugPutChar(0x7D);
                    OS_DebugPutChar((outPtr[tpReadIdx] ^ 0x20));
                }
                else
                {
                    OS_DebugPutChar(outPtr[tpReadIdx]);
                }

                tpReadIdx = (tpReadIdx+1) %tpBuffSize;

            }

            OS_DebugPutChar(0x7E);

            tpSize -= sendSize;

        }

        sio_dsp_log_headInfo_ptr->TPRead_idx = ( (tpReadIdx_tmp + tpBuffSize + tpSize_tmp - (1 << DSP_LOG_IDX_TYPE)) % tpBuffSize) >> DSP_LOG_IDX_TYPE;


        while (memDataSize > 0)
        {

            sendSize = memDataSize>maxSendSize ? maxSendSize : memDataSize;

            OS_DebugPutChar(0x7E);

            seqNum = SIO_GetPPPFrameSeqNum();

            dspLogFrameHeadLen = sizeof (DSPLOG_FRM_HEAD_T);

            dspLogFrmHead.seq_num = seqNum;
            dspLogFrmHead.len = (uint16) (sendSize + dspLogFrameHeadLen);
            dspLogFrmHead.type = DIAG_MOBILETV_DSP_LOG_F;
            dspLogFrmHead.subtype = DSP_LOG_SUBTYPE_MEMDATA;

            outPtr = (uint8 *) (&dspLogFrmHead);

            for (i=0; i<dspLogFrameHeadLen; i++)
            {
                if ( (outPtr[i] == 0x7E) || (outPtr[i] == 0x7D))
                {
                    OS_DebugPutChar(0x7D);
                    OS_DebugPutChar((outPtr[i] ^ 0x20));
                }
                else
                {
                    OS_DebugPutChar(outPtr[i]);
                }

            }

            outPtr = (uint8 *) (memBuffBaseAddr);

            for (i=0; i<sendSize; i++)
            {
                if ( (outPtr[memReadIdx] == 0x7E) || (outPtr[memReadIdx] == 0x7D))
                {
                    OS_DebugPutChar(0x7D);
                    OS_DebugPutChar((outPtr[memReadIdx] ^ 0x20));
                }
                else
                {
                    OS_DebugPutChar(outPtr[memReadIdx]);
                }

                memReadIdx = (memReadIdx+1) %memBuffSize;

            }

            OS_DebugPutChar(0x7E);

            memDataSize -= sendSize;

        }

        sio_dsp_log_headInfo_ptr->memDataRead_idx = ( (memReadIdx_tmp + memBuffSize + memDataSize_tmp - (1 << DSP_LOG_IDX_TYPE)) % memBuffSize) >> DSP_LOG_IDX_TYPE;

        /*check the DSP Status,if busy bit is true, DSP has new data write to the memory, ARM must send out the new data*/
        if (sio_dsp_log_headInfo_ptr->statusFlag & DSP_LOG_STATUS_ASSERT_BUSY_BIT)
        {
            retry++;

            for (i=0; i<0x1000; i++) {}
        }

    }
    while (retry--);

    OS_DumpFlush();

    return;

}


/*****************************************************************************/
// Description: Dump Dsp Memory in assert mode
// Global resource dependence: s_sio_dsp_frame_ctrl, s_sio_tx_dma_is_start, d
//                                            spLogSendContinueCnt, sio_dsp_log_headInfo_ptr, sio_port
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC void SIO_DumpDSPMemory (void)
{
    uint32 i = 0;
    uint32 retry = 0;

    uint32 dumpDspBlkNum = 0;
    uint32 dumpDspLen = 0;
    uint32 dumpBuffBaseAddr = 0;
    uint32 dumpReadIdx = 0;
    uint32 blk_tmp = 0;

    uint32 seqNum = 0;
    uint32 sendSize = 0;

    uint16 head = 0;
    uint16 tail = 0;
    uint8 *buf = NULL;
    uint16 list_size = 0;
    uint16 buf_cnt = 0;
    uint32 buf_size = 0;
    uint16 start = 0;
    uint16 list_idx = 0;
    uint16 buf_idx = 0;

    const uint32 maxSendSize = 1024;
    DSPLOG_FRM_HEAD_T  dspLogFrmHead = {0,0,0,0};
    uint32  dspLogFrameHeadLen = 0;
    uint8 *outPtr = NULL;

    if (!SIO_IsDspLogUSBEnable())
    {
        return;
    }

    head = s_sio_dsp_frame_ctrl.head;
    tail = s_sio_dsp_frame_ctrl.tail;
    list_size = s_sio_dsp_frame_ctrl.list_size;
    buf = s_sio_dsp_frame_ctrl.frame_list[head].addr;
    buf_size =  s_sio_dsp_frame_ctrl.frame_list[head].length;
    start =  s_sio_dsp_frame_ctrl.frame_list[head].pos;
    buf_cnt = s_sio_dsp_frame_ctrl.count;

    //when Assert occured during dma is acive, buf count -1
    if (s_sio_tx_dma_is_start &&  dspLogSendContinueCnt && buf_cnt)
    {
        buf_cnt--;
        tail++;
    }

    /*send out frame buffer remain data*/
    OS_DebugPutChar(0x7E);

    for (list_idx=0; list_idx<buf_cnt; list_idx++)
    {
        for (buf_idx=0; buf_idx<buf_size; buf_idx++)
        {
            OS_DebugPutChar((char) (* (s_sio_dsp_frame_ctrl.frame_list[ (tail+list_idx) %list_size].addr+buf_idx)));

        }

    }

    OS_DebugPutChar(0x7E);

    //∩辰?aarm ﹞??那dsp米?bus㏒?車?角∩dump dsp information
    //AHB_CTRL1:   BIT_16, ARM enables CEVAX matrix to sleep. This bit will automatically reset once ARM tries to access CEVAX matrix.
    //DSP_BOOT_EN: BIT_2,  ARM access DSP ASHB bridge enable, active high.
    * (volatile uint32 *) (AHB_CTL1) &= ~ (0x1<<16);

    for (i=0; i<100; i++) {}

    * (volatile uint32 *) (DSP_BOOT_EN) |= (0x1<<2);

    for (i=0; i<100; i++) {}

    /*send out share trace buffer remain data*/
    retry = 0;

    do
    {
        dumpDspBlkNum = sio_dsp_log_headInfo_ptr->dumpDspBlkNum;

        for (blk_tmp=0; blk_tmp<dumpDspBlkNum; blk_tmp++)
        {
            dumpDspLen = sio_dsp_log_headInfo_ptr->dump_dsp_blk[blk_tmp].len;
            dumpBuffBaseAddr = ( (uint32) (sio_dsp_log_headInfo_ptr->dump_dsp_blk[blk_tmp].start_addr) |DSP_ARM_ADD_OFFSET) - DSP_ARM_ADD_OFFSET;

            if (! (dumpBuffBaseAddr&DSP_SIDE_START_ADDR))  //for RAM space(0x0->0x400000), add DSP_SHAREMEM_VIR_BASE_ADDR
            {
                dumpBuffBaseAddr =  dumpBuffBaseAddr + CHIP_GetDSPShareMemVirBaseAddr();
            }

            dumpReadIdx = 0;

            while (dumpDspLen > 0)
            {

                sendSize = dumpDspLen>maxSendSize ? maxSendSize : dumpDspLen;

                OS_DebugPutChar(0x7E);

                seqNum = SIO_GetPPPFrameSeqNum();

                dspLogFrameHeadLen = sizeof (DSPLOG_FRM_HEAD_T);

                dspLogFrmHead.seq_num = seqNum;
                dspLogFrmHead.len = (uint16) (sendSize + dspLogFrameHeadLen);
                dspLogFrmHead.type =DIAG_MOBILETV_DSP_LOG_F;
                dspLogFrmHead.subtype = DSP_LOG_SUBTYPE_DUMPDATA;

                outPtr = (uint8 *) (&dspLogFrmHead);

                for (i=0; i<dspLogFrameHeadLen; i++)
                {
                    if ( (outPtr[i] == 0x7E) || (outPtr[i] == 0x7D))
                    {
                        OS_DebugPutChar(0x7D);
                        OS_DebugPutChar((outPtr[i] ^ 0x20));
                    }
                    else
                    {
                        OS_DebugPutChar(outPtr[i]);
                    }

                }

                outPtr = (uint8 *) (dumpBuffBaseAddr);

                for (i=0; i<sendSize; i++)
                {
                    if ( (outPtr[dumpReadIdx] == 0x7E) || (outPtr[dumpReadIdx] == 0x7D))
                    {
                        OS_DebugPutChar(0x7D);
                        OS_DebugPutChar((outPtr[dumpReadIdx] ^ 0x20));
                    }
                    else
                    {
                        OS_DebugPutChar(outPtr[dumpReadIdx]);
                    }

                    dumpReadIdx++;
                }

                OS_DebugPutChar(0x7E);

                dumpDspLen -= sendSize;

            }

        }

        /*check the DSP Status,if busy bit is true, DSP has new data write to the memory, ARM must send out the new data*/
        if (sio_dsp_log_headInfo_ptr->statusFlag & DSP_LOG_STATUS_ASSERT_BUSY_BIT)
        {
            retry++;

            for (i=0; i<0x1000; i++) {}
        }

    }
    while (retry--);

    OS_DumpFlush();

    return;

}

/*****************************************************************************/
// Description: Check Dsp log frame buffer is full or not
// Global resource dependence: s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_IsDSPLogFrameFull (void)     // If DSP Log Frame full, returns TRUE
// else returns FALSE
{
    if (s_sio_dsp_frame_ctrl.count >= s_sio_dsp_frame_ctrl.list_size)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

LOCAL uint16 _gb2ucs(					//return the number of UCS2 code that have been converted.
						uint16 *ucs2_ptr, 	//destination: string pointer to UCS2 string.
						const uint8 *gb_ptr,//source: string pointer to GB string
						uint16 len			//the length to convert from GB to UCS2.
						)
{
    uint16 tmp_gb, count = 0;

    SCI_ASSERT(PNULL != gb_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != ucs2_ptr); /*assert verified*/

    while((len > 0) && (len < 0xfff))
    {
        len--;
        count += 2;
        tmp_gb = (uint16)(*gb_ptr++);

        if (tmp_gb < 0x80)	//An ASCII
        {
            *ucs2_ptr++ = tmp_gb;
        }
    }

    return count;
}

LOCAL BOOLEAN _CreatFile(
                SFS_HANDLE*     fd_ptr,
                uint16*         filename,
                uint16*         device_name
                )
{
    SFS_ERROR_E device_result = SFS_ERROR_NO_SPACE;
    BOOLEAN return_result = FALSE;  
        
		device_result = SFS_GetDeviceStatus(device_name);   

    if(SFS_ERROR_NONE == device_result)
    {       
        *fd_ptr = SFS_CreateFile(filename, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);/*lint !e655*/
           
        if(NULL != *fd_ptr)     
        {
            SIO_DSP_TRACE("DSPLOG: create file success!");
            return_result = TRUE;
        }
        else
        {
            SIO_DSP_TRACE("DSPLOG: create file fail!");
            return_result = FALSE;
        }      
    }    
    else
    {
        SIO_DSP_TRACE("DSPLOG: device status(%d) error!", device_result);      
        return_result = FALSE;
    }     
    
    return return_result;
}

LOCAL BOOLEAN _CreatDspLogFile( uint16 logfile_num)
{
    BOOLEAN        ret_create_file;
    char        dsp_log_file_name_gb[SFS_MAX_PATH+1] ={0};
    uint16      dsp_log_file_name[SFS_MAX_PATH+1] ={0};
    uint16      dsp_log_file_name_len = 0;
    uint16      tcard_name[2] = {'E', 0};    

	dsp_log_file_name_len = sprintf(dsp_log_file_name_gb,"E:\\dsplog%04d.log",(uint)logfile_num);
	_gb2ucs(dsp_log_file_name,  (uint8 *) dsp_log_file_name_gb, dsp_log_file_name_len);  
	SIO_DSP_TRACE("DSPLOG: start to save dsplog in sdCard ,logfile name is %s", dsp_log_file_name_gb); 

	/* create new log file in sfs, original file */        
	ret_create_file = _CreatFile(&sfs_dsp_log,dsp_log_file_name,tcard_name);		

	if(FALSE == ret_create_file)      
	{        
	    SIO_DSP_TRACE("DSPLOG: create file %s fail, finish_time %d",dsp_log_file_name_gb, SCI_GetTickCount());               
	    return FALSE;      
	}  

    SIO_DSP_TRACE("DSPLOG: create file suc, finish_time %d", SCI_GetTickCount());    
    return TRUE;
}


/* step:50M, dsplogfile_num++, and create file  */
LOCAL BOOLEAN _WriteFile_Stepupfile(void)
{
	BOOLEAN ret;
	
	SFS_CloseFile( sfs_dsp_log);  

	dsplogfile_num++;
    ret = _CreatDspLogFile(dsplogfile_num);

    if(ret)   
    {   
        REFPARAM_SetDspCardLogNum(dsplogfile_num);
        return TRUE;       
    }
	else
	{
		return FALSE;	
	}
}

LOCAL BOOLEAN _WriteFile_DelFile(void)
{
	SFS_ERROR_E     file_ret = SFS_ERROR_NONE;
    char        file_name_gb[SFS_MAX_PATH+1] ={0};
    uint16      file_name[SFS_MAX_PATH+1] ={0};
    uint16      file_name_len = 0;	

 	for( ;deletefile_num <= dsplogfile_num; )
	{		
		if( deletefile_num == dsplogfile_num )	
		{
			/* Close log file in sfs */
	    	SFS_CloseFile( sfs_dsp_log); 
			/* reset dsplog_saved_size,or stepup one time() */
			dsplog_saved_size = 0;
		}	
				
		file_name_len = sprintf(file_name_gb,"E:\\dsplog%04d.Log",(uint)deletefile_num);	
		_gb2ucs(file_name,  (uint8 *) file_name_gb, file_name_len); 	
	
		file_ret = SFS_DeleteFile( file_name,PNULL );	
		if( SFS_ERROR_NONE == file_ret )
		{
			SIO_DSP_TRACE("DSPLOG: _WriteFile_DelFile, del(%s) ok",file_name);
			if( deletefile_num == dsplogfile_num )	
			{
				dsplogfile_num++;
				/*recreate log file in sfs*/
		    	_CreatDspLogFile( dsplogfile_num ); 
                REFPARAM_SetDspCardLogNum(dsplogfile_num);
			}	
			deletefile_num++;				
			return TRUE;			
		}
		else
		{
			deletefile_num++;
		}	
	}

 	SIO_DSP_TRACE("DSPLOG: _WriteFile_DelFile, del file fail,cur num(%d)",dsplogfile_num);
	return FALSE;	

}

LOCAL BOOLEAN _WriteDspLogFile(
                uint8*              write_buff_ptr,
                uint32              write_length
                )
{
    SFS_ERROR_E         result = SFS_ERROR_NO_SPACE;
    uint32              wrtlen;
    BOOLEAN          return_result = FALSE;

    result = SFS_WriteFile(sfs_dsp_log, (void*)write_buff_ptr, write_length, &wrtlen, PNULL);
    
    switch( result )
    {
        case SFS_ERROR_NONE:/*0:success*/
		{
			dsplog_saved_size += write_length;
			
			if( dsplog_saved_size >= dsplog_stepup_size )
			{				
				SIO_DSP_TRACE("DSPLOG: stepupfile, dsplog_saved_size(%d),dsplog_stepup_size(%d)",dsplog_saved_size,dsplog_stepup_size);/* cr208045 */
				/* step of save size is 50M */
				dsplog_saved_size = 0;
				return_result  = _WriteFile_Stepupfile();
				if( FALSE == return_result )
				{				
					/* turn off saving */
					SFS_CloseFile( sfs_dsp_log); 
					return_result = FALSE; 
				}	
				else
				{				    
			    	return_result = TRUE; 			
				}						
			}	
			else
			{
		    	return_result = TRUE; 			
			}			
		    break ;
		}  
        case SFS_ERROR_NO_SPACE:/*11:sd card is full,del 50M,*/
		{
			SIO_DSP_TRACE("DSPLOG: _WriteDspLogFile,ret = SFS_ERROR_NO_SPACE");			
			return_result  = _WriteFile_DelFile();
			if( FALSE == return_result )
			{
				/* just turn off saving, tbd more better */
				SFS_CloseFile( sfs_dsp_log); 
				return_result = FALSE; 
				break;				
			}
			else
			{
				return_result = _WriteDspLogFile(dsplog_buf.buf, dsplog_buf.buf_len); 
				if( FALSE == return_result )
				{	
					SCI_PASSERT(0, ("DSPLOG: _WriteDspLogFile, rewrite fail, fatal err!!!"));   /*assert verified*/
				}
				return_result = TRUE; 
			}
		    break ;
		} 		
		case SFS_ERROR_DEVICE:/*1: device error*/
        case SFS_ERROR_INVALID_PARAM:/*3: param you give is invalid*/
		{
			/*In UDisk mode, SFS will be unmounted, and return 3 */
			SIO_DSP_TRACE("DSPLOG: _WriteDspLogFile, result = %d",result);
		    SFS_CloseFile( sfs_dsp_log);     
			return_result = FALSE; 
			break;
		}				
		default:
        {
            SIO_DSP_TRACE("DSPLOG: _WriteDspLogFile, result = %d",result);
		    SFS_CloseFile( sfs_dsp_log);  
			return_result = FALSE;   
            break ;
        } 				
    }	 
   
    return return_result;
}

/*****************************************************************************/
// Description: enable dsp log save to T card
// Global resource dependence: dsplogfile_num, sfs_dsp_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_DSPCardLogOpen (void)
{
    BOOLEAN ret = FALSE;

    if(sfs_dsp_log)
    {
        SFS_CloseFile( sfs_dsp_log);
        sfs_dsp_log = NULL;
    }

    dsplogfile_num = REFPARAM_GetDspCardLogNum() + 1;
    ret = _CreatDspLogFile(dsplogfile_num);
    if(!ret)
    {
        sfs_dsp_log = NULL;
        SIO_DSP_TRACE ("DSPLOG: Open DSP Log file fail!");
        return FALSE;
    }

    SIO_DSP_TRACE ("DSPLOG: Open DSP Log file success!");

    REFPARAM_SetDspCardLogNum(dsplogfile_num);
    REFPARAM_SetDspCardLogFlag(TRUE);

    return TRUE;
}

/*****************************************************************************/
// Description: disable dsp log save to T card
// Global resource dependence: sfs_dsp_log
// Author: fei.zhang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN SIO_DSPCardLogClose (void)
{    
    SFS_CloseFile( sfs_dsp_log);
    sfs_dsp_log = NULL;
    SIO_DSP_TRACE ("DSPLOG: Close DSP Log file success!");

    REFPARAM_SetDspCardLogFlag(FALSE);

    return TRUE;
}

/*****************************************************************************/
// Description: Dsp log process Task
// Global resource dependence: sio_dsplog_trans_buf, dsp_log_semaphore, sio_dsp_log_headInfo_ptr
// Author: fei.zhang
// Note:
/*****************************************************************************/
LOCAL void SIO_DSPLogThread (uint32 id, void *pdata)
{
    //uint32 i = 0;
    uint32 tpReadIdx = 0;
    uint32 tpWriteIdx = 0;
    uint32 memDataReadIdx = 0;
    uint32 memDataWriteIdx = 0;
    uint32 dspLogStatusFlag = 0;
    uint32 tpDataSize = 0;
    uint32 tpBuffSize = 0;
    uint32 tpBuffBaseAddr = 0;
    uint32 memDataSize = 0;
    uint32 memBuffSize = 0;
    uint32 memBuffBaseAddr = 0;
    //uint32 status = 0;
    uint32  size  = 0;
    uint32 seqNum = 0;
    uint32 readIdx_tmp = 0;
    BOOLEAN isRetry = FALSE;
    uint32 already_write_size = 0;
    uint8 *transBuffAddr = NULL;
    uint32  dspLogFrameHeadLen = 0;
    DSPLOG_FRM_HEAD_T  *dspLogFrmHead_ptr;
    BOOLEAN ret = FALSE;

    transBuffAddr = (uint8 *) ( ( (uint32) (sio_dsplog_trans_buf + 3) >>2) <<2);

    if(REFPARAM_GetDspCardLogFlag())
    {
        dsplogfile_num = REFPARAM_GetDspCardLogNum() + 1;
        ret = _CreatDspLogFile(dsplogfile_num);
        if(!ret)
        {
            sfs_dsp_log = NULL;
            SIO_DSP_TRACE ("DSPLOG: Open DSP Log file fail!");
        }
        REFPARAM_SetDspCardLogNum(dsplogfile_num);
    }
    
    for (; ;)
    {
        SCI_GetSemaphore (dsp_log_semaphore,SCI_WAIT_FOREVER); //SCI_WAIT_FOREVER
        SIO_DSP_TRACE ("*********************Get dsp_log_semaphore*****************");

        /******************************************************************
        **   |free        |-> baseAddr
        **   |free        |
        **   |reserve   |->readIdx
        **   |data 0     |->first data to be read
        **   |data 1     |
        **      .
        **      .
        **      .
        **   |data n-1 |
        **   |data n     |
        **   |free        |->writeIdx ,first free space could be Write
        **   |free        |
        **   |free        |
        **   reserve data is used for checking the buffer is full or empty
        **  when writeIdx==readIdx,buffer is full
        **      readIdx + 1== writeIdx,buffer is empty
        **  in DSP data uint is DWORD(4 bytes), the index unit is DWORD also
        **  in ARM DATA Process is byte by byte
        ***********************************************************************/

        tpBuffSize = sio_dsp_log_headInfo_ptr->TPBuffSize << DSP_LOG_IDX_TYPE;
        memBuffSize = sio_dsp_log_headInfo_ptr->memDataBuffSize << DSP_LOG_IDX_TYPE;
        tpBuffBaseAddr = (sio_dsp_log_headInfo_ptr->TPBuffBaseAddr|DSP_ARM_ADD_OFFSET) - DSP_ARM_ADD_OFFSET + CHIP_GetDSPShareMemVirBaseAddr();
        memBuffBaseAddr = (sio_dsp_log_headInfo_ptr->memDataBuffBaseAddr|DSP_ARM_ADD_OFFSET) - DSP_ARM_ADD_OFFSET + CHIP_GetDSPShareMemVirBaseAddr();

        tpReadIdx = ( (sio_dsp_log_headInfo_ptr->TPRead_idx + 1) << DSP_LOG_IDX_TYPE) % tpBuffSize; // Read index must +1, then point to the first data to be read
        tpWriteIdx = (sio_dsp_log_headInfo_ptr->TPWrite_idx) << DSP_LOG_IDX_TYPE;
        memDataReadIdx = ( (sio_dsp_log_headInfo_ptr->memDataRead_idx + 1) << DSP_LOG_IDX_TYPE) % memBuffSize; // Read index must +1, then point to the first data to be read
        memDataWriteIdx = (sio_dsp_log_headInfo_ptr->memDataWrite_idx) << DSP_LOG_IDX_TYPE;
        dspLogStatusFlag = sio_dsp_log_headInfo_ptr->statusFlag;

        tpDataSize = (tpWriteIdx + tpBuffSize - tpReadIdx) % tpBuffSize;
        memDataSize = (memDataWriteIdx + memBuffSize - memDataReadIdx) % memBuffSize;

        SIO_DSP_TRACE ("!!!!SIO_DSPLogThread  TPRead_idx = 0x%x TPWrite_idx = 0x%x, tpsize = 0x%xbytes",sio_dsp_log_headInfo_ptr->TPRead_idx ,sio_dsp_log_headInfo_ptr->TPWrite_idx, tpDataSize);

        /*TP data*/
        while (
            tpDataSize > 0
            && (tpDataSize >= DSP_LOG_FRAME_BUFF_GET_SIZE
                || (dspLogStatusFlag & DSP_LOG_STATUS_TP_FORCE_BIT))
        )
        {
            size = MIN (tpDataSize, DSP_LOG_FRAME_BUFF_GET_SIZE);

            if(REFPARAM_GetDspCardLogFlag())
            {
                /*copy tpdata into tans buffer*/
                if (tpReadIdx + size > tpBuffSize)
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr), (uint8 *) (tpBuffBaseAddr+tpReadIdx),tpBuffSize-tpReadIdx);
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+tpBuffSize-tpReadIdx), (uint8 *) tpBuffBaseAddr,size + tpReadIdx -tpBuffSize);
                }
                else
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr), (uint8 *) (tpBuffBaseAddr+tpReadIdx),size);
                }

                dsplog_buf.buf = transBuffAddr;
                dsplog_buf.buf_len = size;

                _WriteDspLogFile(transBuffAddr, size);

                readIdx_tmp= ( (tpReadIdx + tpBuffSize + size - (1 << DSP_LOG_IDX_TYPE)) % tpBuffSize) >> DSP_LOG_IDX_TYPE;
                sio_dsp_log_headInfo_ptr->TPRead_idx = readIdx_tmp;
                tpReadIdx = (tpReadIdx + size) % tpBuffSize;
                tpDataSize -= size;                       
            }
            else
            {
                if (SIO_IsDSPLogFrameFull())
                {
                    break;
                }
    
                seqNum = SIO_GetPPPFrameSeqNum();

                dspLogFrameHeadLen = sizeof (DSPLOG_FRM_HEAD_T);
                //frame head
                dspLogFrmHead_ptr = (DSPLOG_FRM_HEAD_T *) transBuffAddr;
                dspLogFrmHead_ptr->seq_num = seqNum;
                dspLogFrmHead_ptr->len = size + dspLogFrameHeadLen;
                dspLogFrmHead_ptr->type =DIAG_MOBILETV_DSP_LOG_F;
                dspLogFrmHead_ptr->subtype = DSP_LOG_SUBTYPE_TP;

                /*copy tpdata into tans buffer*/
                if (tpReadIdx + size > tpBuffSize)
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+dspLogFrameHeadLen), (uint8 *) (tpBuffBaseAddr+tpReadIdx),tpBuffSize-tpReadIdx);
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+dspLogFrameHeadLen+tpBuffSize-tpReadIdx), (uint8 *) tpBuffBaseAddr,size + tpReadIdx -tpBuffSize);
                }
                else
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+dspLogFrameHeadLen), (uint8 *) (tpBuffBaseAddr+tpReadIdx),size);
                }

                size += dspLogFrameHeadLen;

                do
                {

                    already_write_size = SIO_WriteDSPFrame (\
                                                            transBuffAddr, \
                                                            size);


                    if (already_write_size == 0)
                    {
                        isRetry = FALSE;
                    }
                    else if (already_write_size < size)
                    {
                        SIO_DSP_TRACE ("!!!DSP LOG tp retry");
                        /*if Packet the data fail, half the size, packet again*/
                        size = ( ( (size - dspLogFrameHeadLen) >>3) <<2) + dspLogFrameHeadLen;
                        dspLogFrmHead_ptr->len = size;
                        isRetry = TRUE;
                    }
                    else
                    {
                        SIO_DSP_TRACE ("!!!DSP LOG tp success");
                        readIdx_tmp= ( (tpReadIdx + tpBuffSize + size - dspLogFrameHeadLen - (1 << DSP_LOG_IDX_TYPE)) % tpBuffSize) >> DSP_LOG_IDX_TYPE;
                        SIO_DspLogSendOut (&sio_dsp_log_headInfo_ptr->TPRead_idx,readIdx_tmp);
                        tpReadIdx = (tpReadIdx + size - dspLogFrameHeadLen) % tpBuffSize;
                        tpDataSize -= size - dspLogFrameHeadLen;
                        isRetry = FALSE;
                    }
                }
                while (isRetry);
            }
        }


        SIO_DSP_TRACE ("!!!!SIO_DSPLogThread  memDataRead_idx = 0x%x memDataWrite_idx = 0x%x, memDatasize = 0x%xbytes",sio_dsp_log_headInfo_ptr->memDataRead_idx ,sio_dsp_log_headInfo_ptr->memDataWrite_idx, memDataSize);

        /*MEM data*/
        while (
            memDataSize > 0
            && (memDataSize >= (DSP_LOG_FRAME_BUFF_GET_SIZE + DSP_LOG_MEM_RSV_SIZE)  //Memdata keep 8kB data in buffer for dsp dma trans
                || (dspLogStatusFlag & DSP_LOG_STATUS_MEM_FORCE_BIT))
        )
        {
            size = MIN (memDataSize, DSP_LOG_FRAME_BUFF_GET_SIZE);

            if(REFPARAM_GetDspCardLogFlag())
            {
                /*copy memdata into tans buffer*/
                if (memDataReadIdx + size > memBuffSize)
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr), (uint8 *) (memBuffBaseAddr+memDataReadIdx),memBuffSize-memDataReadIdx);
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+memBuffSize-memDataReadIdx), (uint8 *) memBuffBaseAddr,size + memDataReadIdx -memBuffSize);
                }
                else
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr), (uint8 *) (memBuffBaseAddr+memDataReadIdx),size);
                }

                dsplog_buf.buf = transBuffAddr;
                dsplog_buf.buf_len = size;
                
                _WriteDspLogFile(transBuffAddr, size);

                readIdx_tmp= ( (memDataReadIdx + memBuffSize + size - dspLogFrameHeadLen - (1 << DSP_LOG_IDX_TYPE)) % memBuffSize) >> DSP_LOG_IDX_TYPE;
                sio_dsp_log_headInfo_ptr->memDataRead_idx = readIdx_tmp;
                memDataReadIdx = (memDataReadIdx + size - dspLogFrameHeadLen) % memBuffSize;
                memDataSize -= size - dspLogFrameHeadLen;
            }
            else
            {
                if (SIO_IsDSPLogFrameFull())
                {
                    break;
                }

                seqNum = SIO_GetPPPFrameSeqNum();

                dspLogFrameHeadLen = sizeof (DSPLOG_FRM_HEAD_T);
                //frame head
                dspLogFrmHead_ptr = (DSPLOG_FRM_HEAD_T *) transBuffAddr;
                dspLogFrmHead_ptr->seq_num = seqNum;
                dspLogFrmHead_ptr->len = size + dspLogFrameHeadLen;
                dspLogFrmHead_ptr->type = DIAG_MOBILETV_DSP_LOG_F;
                dspLogFrmHead_ptr->subtype = DSP_LOG_SUBTYPE_MEMDATA;

                /*copy memdata into tans buffer*/
                if (memDataReadIdx + size > memBuffSize)
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+dspLogFrameHeadLen), (uint8 *) (memBuffBaseAddr+memDataReadIdx),memBuffSize-memDataReadIdx);
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+dspLogFrameHeadLen+memBuffSize-memDataReadIdx), (uint8 *) memBuffBaseAddr,size + memDataReadIdx -memBuffSize);
                }
                else
                {
                    SCI_MEMCPY ( (uint8 *) ( (uint32) transBuffAddr+dspLogFrameHeadLen), (uint8 *) (memBuffBaseAddr+memDataReadIdx),size);
                }

                size += dspLogFrameHeadLen;

                do
                {

                    already_write_size = SIO_WriteDSPFrame (\
                                                            transBuffAddr, \
                                                            size);


                    if (already_write_size == 0)
                    {
                        isRetry = FALSE;
                    }
                    else if (already_write_size < size)
                    {
                        SIO_DSP_TRACE ("!!!DSP LOG mem retry");
                        /*if Packet the data fail, half the size, packet again*/
                        size = ( ( (size - dspLogFrameHeadLen) >>3) <<2) + dspLogFrameHeadLen;
                        dspLogFrmHead_ptr->len = size;
                        isRetry = TRUE;
                    }
                    else
                    {
                        SIO_DSP_TRACE ("!!!DSP LOG mem success");
                        readIdx_tmp= ( (memDataReadIdx + memBuffSize + size - dspLogFrameHeadLen - (1 << DSP_LOG_IDX_TYPE)) % memBuffSize) >> DSP_LOG_IDX_TYPE;
                        SIO_DspLogSendOut (&sio_dsp_log_headInfo_ptr->memDataRead_idx,readIdx_tmp);
                        memDataReadIdx = (memDataReadIdx + size - dspLogFrameHeadLen) % memBuffSize;
                        memDataSize -= size - dspLogFrameHeadLen;
                        isRetry = FALSE;
                    }
                }
                while (isRetry);
            }

        }

    }
}


/*****************************************************************************/
// Description: Dsp log task creat
// Global resource dependence: dsp_log_semaphore, dsp_log_thread_id, s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/

PUBLIC void SIO_CreateDSPLogThread (void)
{
    uint16  frame_list_num = 0;

    //Initalize DSP PPP Frame Control
    s_sio_dsp_frame_ctrl.count = 0;
    s_sio_dsp_frame_ctrl.head = 0;
    s_sio_dsp_frame_ctrl.tail = 0;
    s_sio_dsp_frame_ctrl.list_size = MAX_SIO_DSP_PPP_FRAME_SIZE;

    for (frame_list_num = 0; frame_list_num < s_sio_dsp_frame_ctrl.list_size; frame_list_num++)
    {
        s_sio_dsp_frame_ctrl.frame_list[frame_list_num].addr = (uint8 *) &sio_debug_dsp_send_buf[frame_list_num*MAX_SIO_DSP_PPP_FRAME_BUFF_SIZE];
        s_sio_dsp_frame_ctrl.frame_list[frame_list_num].length = MAX_SIO_DSP_PPP_FRAME_BUFF_SIZE;
        s_sio_dsp_frame_ctrl.frame_list[frame_list_num].pos = 0;
    }

    if (SCI_NULL == dsp_log_semaphore)
    {
        dsp_log_semaphore = SCI_CreateSemaphore ("DSP_LOG_SEMAPHORE", 0);
    }

    SCI_PASSERT ( (dsp_log_semaphore != SCI_NULL), ("Create DSP_LOG_SEMAPHORE failed!"));/*assert verified*/


    if (SCI_INVALID_BLOCK_ID == dsp_log_thread_id)
    {
        dsp_log_thread_id = SCI_CreateThread ("DSP_LOG_Task", "DSP_LOG_QUEUE",
                                              SIO_DSPLogThread/*void(* entry)(uint32,void *)*/,
                                              0, NULL, 10240, 10,
                                              PRI_DSP_LOG_TASK,  // same priority as sme
                                              SCI_PREEMPT,
                                              SCI_AUTO_START);
    }

    SCI_PASSERT ( (dsp_log_thread_id != SCI_INVALID_BLOCK_ID), ("Create DSP LOG task failed!"));/*assert verified*/

    SIO_DSP_TRACE ("!!!Create SIO_DSPLogThread successfully!!!");

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif /* End of sio.c*/
