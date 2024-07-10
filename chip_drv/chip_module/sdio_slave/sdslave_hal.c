/*******************************************************************************
 ** File Name:      sdslave_hal.c                                              *
 ** Author:         andy.chen                                                  *
 ** Date:           12/27/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2010.12        andy.chen         Create                                    *
 ** 20xx.xx        xxxx.xxxx         update                                    *
 ******************************************************************************/

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/
#include "sci_types.h"
#include "sci_api.h"
#include "os_api.h"
#include "sdslave_phy.h"
#include "sdslave_drvapi.h"
#include "mmu_drvapi.h"
/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/
#define SDSLAVE_EVENT

// state machine
#define STATE_CLOSED        0
#define STATE_OPENED        1
#define STATE_RST           2
#define STATE_OCR_READY     3
#define STATE_WR_WAIT       4
#define STATE_RD_WAIT       5
#define STATE_WR_CMP        6
#define STATE_RD_CMP        7
#define STATE_CARD_INT      8
#define STATE_ERR           9

#define HANDLE_VERIFY(handle)   (((handle) == &s_sdslaveHalHd)?(SCI_TRUE):(SCI_FALSE))
/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
typedef struct SDSLAVE_HAL_TAG
{
    BOOLEAN open_flag;
    SDSLAVE_PHY_HANDLE sdslave_port;
    uint32 sdslave_state;
    uint32 sdslave_err;
#if defined (SDSLAVE_EVENT)
    SCI_EVENT_GROUP_PTR  sdslave_event;
#else
    volatile uint32 s_event;
#endif
} SDSLAVE_HAL_T;


/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/
LOCAL SDSLAVE_HAL_T s_sdslaveHalHd;
/*******************************************************************************
 **                    LOCAL Function Definitions                              *
 ******************************************************************************/
//-----------------------------------------------------------------------------------
//  Clear all event  happened before ,and prepare next transmission
//-----------------------------------------------------------------------------------
LOCAL void __InitEvent (SDSLAVE_HAL_HANDLE handle)
{
    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/
#if defined (SDSLAVE_EVENT)
    SCI_SetEvent (handle->sdslave_event,0,SCI_AND);
#else
    handle->s_event = 0;
#endif
}

//-----------------------------------------------------------------------------------
//  After the transmission begin , wait event during the transmission
//-----------------------------------------------------------------------------------
LOCAL void __WaitEvent (SDSLAVE_HAL_HANDLE handle,uint32 EventId)
{
    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/
#if defined (SDSLAVE_EVENT)
    {
        uint32 actualFlag;
        SCI_GetEvent (handle->sdslave_event,EventId,SCI_OR_CLEAR,&actualFlag,SCI_WAIT_FOREVER);
        SDSLAVE_PRINT ("sdio slave hal, wait event come:0x%x,0x%x", EventId,actualFlag);
    }
#else

    while (EventId != (handle->s_event&EventId));

#endif
}

//-----------------------------------------------------------------------------------
//  After the transmission begin , this funtion used to give event
//-----------------------------------------------------------------------------------
LOCAL void __SetEvent (SDSLAVE_HAL_HANDLE handle,uint32 EventId)
{
    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/
#if defined (SDSLAVE_EVENT)
    SCI_SetEvent (handle->sdslave_event,EventId,SCI_OR);
#else
    handle->s_event |= EventId;
#endif
}

LOCAL void __isrCallBack (uint32 msg)
{
    __SetEvent (&s_sdslaveHalHd, msg);
    s_sdslaveHalHd.sdslave_err = msg&SIG_SDIO_ERR;

    // state proccess
    msg &= ~SIG_SDIO_ERR;

    switch (s_sdslaveHalHd.sdslave_state)
    {
        case STATE_OPENED:
        case STATE_RST:

            if (msg& (~SIG_OCR_SET))
            {
                s_sdslaveHalHd.sdslave_err |= SIG_STATE_ERR;
            }

            break;
        case STATE_OCR_READY:
            break;
        case STATE_RD_WAIT:

            if (msg& (~SIG_WR_TRANS_CMP))
            {
                s_sdslaveHalHd.sdslave_err |= SIG_STATE_ERR;
            }

            break;
        case STATE_WR_WAIT:

            if (msg& (~SIG_RD_TRANS_CMP))
            {
                s_sdslaveHalHd.sdslave_err |= SIG_STATE_ERR;
            }

            break;
        case STATE_RD_CMP:
            break;
        case STATE_WR_CMP:
            break;
        case STATE_CARD_INT:
            break;
        case STATE_ERR:
            break;
        case STATE_CLOSED:
            break;
        default:
            SDSLAVE_ASSERT (0); /*assert verified*/
    }
}

LOCAL void __SetState (SDSLAVE_HAL_HANDLE handle, uint32 state)
{
    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/
    handle->sdslave_state = state;
}

LOCAL uint32 __GetState (SDSLAVE_HAL_HANDLE handle)
{
    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/
    return handle->sdslave_state;
}

LOCAL void __WaitOCR (SDSLAVE_HAL_HANDLE handle)
{
    uint32 ocr;

    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/

    // wait ocr set
    SDSLAVE_PRINT ("sdslave wait ocr_set.");
    __WaitEvent (handle, SIG_OCR_SET);

    // set ocr voltage
    ocr = SDSLAVE_PHY_GetOCRVaue (handle->sdslave_port);
    SDSLAVE_ASSERT (NULL != ocr);/*assert verified*/
    SDSLAVE_PHY_CfgVoltage (handle->sdslave_port, ocr);
    //SCI_SLEEP (20);
    SDSLAVE_PHY_SetOCRReady (handle->sdslave_port);
    __SetState (handle, STATE_OCR_READY);
    SDSLAVE_PRINT ("sdslave ocr set success, host set ocr:0x%x", ocr);
}
/*******************************************************************************
**                    PUBLIC Function Definitions                              *
*******************************************************************************/
PUBLIC SDSLAVE_HAL_HANDLE SDSLAVE_HAL_Open (void)
{
    SDSLAVE_DISABLE_IRQ();

    if (SCI_TRUE == s_sdslaveHalHd.open_flag)
    {
        SDSLAVE_ENABLE_IRQ();
        SDSLAVE_ASSERT (0);/*assert verified*/
    }

    SDSLAVE_ENABLE_IRQ();

    s_sdslaveHalHd.open_flag     = SCI_TRUE;
    s_sdslaveHalHd.sdslave_state = STATE_OPENED;
    s_sdslaveHalHd.sdslave_port  = SDSLAVE_PHY_Init (__isrCallBack);
#if defined (SDSLAVE_EVENT)
    s_sdslaveHalHd.sdslave_event = PNULL;
    s_sdslaveHalHd.sdslave_event = SCI_CreateEvent ("SDSLAVE_EVENT");
    s_sdslaveHalHd.sdslave_err   = 0;

    if (PNULL == s_sdslaveHalHd.sdslave_event)
    {
        SDSLAVE_ASSERT (0);/*assert verified*/
    }

#else
    s_event = 0;
#endif

    // sdio power on
    SDSLAVE_PHY_CfgVoltage (s_sdslaveHalHd.sdslave_port, SDIO_OCR_30V);
    SDSLAVE_PHY_PowrOnOff (s_sdslaveHalHd.sdslave_port, SCI_TRUE);

    // wait ocr
    __WaitOCR (&s_sdslaveHalHd);

    return &s_sdslaveHalHd;
}

PUBLIC uint32 SDSLAVE_HAL_Close (SDSLAVE_HAL_HANDLE handle)
{
    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/

    SDSLAVE_PHY_PowrOnOff (handle->sdslave_port, SCI_FALSE);
    SDSLAVE_PHY_Deinit (handle->sdslave_port);
    s_sdslaveHalHd.sdslave_state = STATE_CLOSED;
    s_sdslaveHalHd.open_flag     = SCI_FALSE;

    return SCI_TRUE;
}

PUBLIC uint32 SDSLAVE_HAL_Read (SDSLAVE_HAL_HANDLE handle, uint8 *pBuf, uint32 len)
{
    uint32 blk_len, blk_cnt;
    uint32 ret;

    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/
    SDSLAVE_PRINT ("sdslave read begain, total_len:%d, addr:0x%x", len, pBuf);

    // cfg dma_addr
    SDSLAVE_PHY_SetDmaAddr (handle->sdslave_port, (uint32) pBuf);
    MMU_InvalideDCACHE();

    // wait data
    __SetState (handle, STATE_RD_WAIT);
    __InitEvent (handle);
    __WaitEvent (handle, SIG_WR_TRANS_CMP|SIG_SDIO_ERR|SIG_STATE_ERR);  // "WR" means host wr to slave

    blk_cnt = SDSLAVE_PHY_GetBlkcnt (handle->sdslave_port);

    if (SDIO_BLK_MODE == SDSLAVE_PHY_GetTransMode (handle->sdslave_port))
    {
        blk_len = SDSLAVE_PHY_GetBlklen (handle->sdslave_port);
    }
    else
    {
        blk_len = 1;
    }

    if (NULL != handle->sdslave_err)
    {
        SDSLAVE_PRINT ("sdslave read err:0x%x, blk_len:%d, blk_cnt:%d, addr:0x%x",
                       handle->sdslave_err, blk_len, blk_cnt, pBuf);
        ret = SCI_FALSE;
    }
    else
    {

        //SDSLAVE_ASSERT (len == blk_len*blk_cnt);
        SDSLAVE_PRINT ("sdslave read success, blk_len:%d, blk_cnt:%d, addr:0x%x",
                       blk_len, blk_cnt, pBuf);
        __SetState (handle, STATE_RD_CMP);
        ret = SCI_TRUE;
    }

    return ret;
}

PUBLIC uint32 SDSLAVE_HAL_Write (SDSLAVE_HAL_HANDLE handle, uint8 *pBuf, uint32 len)
{
    uint32 blk_len, blk_cnt;
    uint32 ret;

    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/
    SDSLAVE_PRINT ("sdslave write begain, total_len:%d, addr:0x%x", len, pBuf);

    // cfg dma_addr
    SDSLAVE_PHY_SetDmaAddr (handle->sdslave_port, (uint32) pBuf);
    MMU_InvalideDCACHE();

    // wait data
    __SetState (handle, STATE_WR_WAIT);
    __InitEvent (handle);
    __WaitEvent (handle, SIG_RD_TRANS_CMP|SIG_SDIO_ERR|SIG_STATE_ERR);  // RD means host rd from slave

    if (NULL != handle->sdslave_err)
    {
        SDSLAVE_PRINT ("sdslave write err:0x%x, blk_len:%d, blk_cnt:%d, addr:0x%x",
                       handle->sdslave_err, blk_len, blk_cnt, pBuf);
        ret = SCI_FALSE;
    }
    else
    {
        blk_cnt = SDSLAVE_PHY_GetBlkcnt (handle->sdslave_port);

        if (SDIO_BLK_MODE == SDSLAVE_PHY_GetTransMode (handle->sdslave_port))
        {
            blk_len = SDSLAVE_PHY_GetBlklen (handle->sdslave_port);
        }
        else
        {
            blk_len = 1;
        }

        //SDSLAVE_ASSERT (len == blk_len*blk_cnt);
        SDSLAVE_PRINT ("sdslave write success, blk_len:%d, blk_cnt:%d, addr:0x%x",
                       blk_len, blk_cnt, pBuf);
        __SetState (handle, STATE_WR_CMP);
        ret = SCI_TRUE;
    }

    return ret;
}

PUBLIC uint32 SDSLAVE_HAL_IOctl (SDSLAVE_HAL_HANDLE handle, uint32 cmd, void *arg)
{
    uint32 ret = SCI_TRUE;

    SCI_ASSERT (HANDLE_VERIFY (handle));/*assert verified*/

    switch (cmd)
    {
        case SDSLAVE_IOCTL_CMD_RST:
            __SetState (handle, STATE_RST);
            SDSLAVE_PHY_RST (handle->sdslave_port, RST_ALL);
            __WaitOCR (handle);
            break;
        case SDSLAVE_IOCTL_CMD_SENDINT:
            __SetState (handle, STATE_CARD_INT);
            SDSLAVE_PHY_SetCardInt (handle->sdslave_port);
            break;
        default:
            SDSLAVE_ASSERT (0); /*assert to do*/
            break;
    }

    return ret;
}
/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif

