/******************************************************************************
 ** File Name:      audio_ifc_phy.c                                           *
 ** Author:         jen.li                                                    *
 ** DATE:           6/15/2022                                                 *
 ** Copyright:      Copyright (c) 2022 Unisoc Communications Inc.             *
 ** Description:    Intelligent Flow Controller                               *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME           DESCRIPTION                                 *
 ** 6/15/2022     jen.li          Create.                                     *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_api.h"
#include "audio_drvapi.h"
#include "audio_ifc_phy.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
//#define sprd_audioIfc ((AUDIO_IFC_T *)REG_AUDIO_IFC_BASE)

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

LOCAL void __ifc_print_reg()
{
    uint32 i;
    volatile uint32 *ptr = (uint32 *)REG_AUDIO_IFC_BASE;
    AUDDRV_DEBUG("/--------ifc reg---------------------/");
    for (i = 0; i < sizeof(AUDIO_IFC_T)/sizeof(uint32); i++) {
        AUDDRV_DEBUG("    {0x%04x} -- {0x%08x}", i*4, *(ptr+i));
    }
    AUDDRV_DEBUG("/------------------------------------/");
}


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

LOCAL int32 __ifc_enable(uint32 direction)
{
    AUDIO_IFC_CONTROL_U *audio_ifc_control;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_control = (AUDIO_IFC_CONTROL_U *) REG_AUD_IFC_CH0_CONTROL;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_control = (AUDIO_IFC_CONTROL_U *) REG_AUD_IFC_CH1_CONTROL;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_control->mBits.enable = TRUE;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_disable(uint32 direction)
{
    AUDIO_IFC_CONTROL_U *audio_ifc_control;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_control = (AUDIO_IFC_CONTROL_U *) REG_AUD_IFC_CH0_CONTROL;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_control = (AUDIO_IFC_CONTROL_U *) REG_AUD_IFC_CH1_CONTROL;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_control->mBits.disable = TRUE;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_auto_disable(uint32 direction)
{
    AUDIO_IFC_CONTROL_U *audio_ifc_control;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_control = (AUDIO_IFC_CONTROL_U *) REG_AUD_IFC_CH0_CONTROL;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_control = (AUDIO_IFC_CONTROL_U *) REG_AUD_IFC_CH1_CONTROL;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_control->mBits.auto_disable = TRUE;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_get_enable_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.enable;
}

LOCAL int32 __ifc_get_fifo_empty_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.fifo_empty;
}

PUBLIC int32 __ifc_get_cause_ief_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.cause_ief;
}

PUBLIC int32 __ifc_get_cause_ihf_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.cause_ihf;
}

LOCAL int32 __ifc_get_cause_i4f_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.cause_i4f;
}

LOCAL int32 __ifc_get_cause_i3_4f_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.cause_i3_4f;
}

LOCAL int32 __ifc_get_cause_ahb_error_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.cause_ahb_error;
}

LOCAL int32 __ifc_get_ief_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.ief;
}

LOCAL int32 __ifc_get_ihf_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.ihf;
}

LOCAL int32 __ifc_get_i4f_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.i4f;
}

LOCAL int32 __ifc_get_i3_4f_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.i3_4f;
}

LOCAL int32 __ifc_get_ahb_error_status(uint32 direction)
{
    AUDIO_IFC_STATUS_U *audio_ifc_status;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH0_STATUS;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_status = (AUDIO_IFC_STATUS_U *) REG_AUD_IFC_CH1_STATUS;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_status->mBits.status_ahb_error;
}

LOCAL int32 __ifc_set_start_addr(uint32 direction,uint32 addr)
{
    AUDIO_IFC_START_ADDR_U *audio_ifc_start_addr;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_start_addr = (AUDIO_IFC_START_ADDR_U *) REG_AUD_IFC_CH0_STARTADDR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_start_addr = (AUDIO_IFC_START_ADDR_U *) REG_AUD_IFC_CH1_STARTADDR;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_start_addr->dwValue = addr;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_set_fifo_size(uint32 direction,uint32 size)
{
    AUDIO_IFC_FIFO_SIZE_U *audio_ifc_fifo_size;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_fifo_size = (AUDIO_IFC_FIFO_SIZE_U *) REG_AUD_IFC_CH0_FIFOSIZE;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_fifo_size = (AUDIO_IFC_FIFO_SIZE_U *) REG_AUD_IFC_CH1_FIFOSIZE;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_fifo_size->dwValue = size;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_set_int_mask(uint32 direction,uint32 mask)
{
    AUDIO_IFC_INT_MASK_U *audio_ifc_int_mask;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_int_mask = (AUDIO_IFC_INT_MASK_U *) REG_AUD_IFC_CH0_INTMASK;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_int_mask = (AUDIO_IFC_INT_MASK_U *) REG_AUD_IFC_CH1_INTMASK;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_int_mask->dwValue = mask;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_set_int_clear(uint32 direction,uint32 clearbits)
{
    AUDIO_IFC_INT_CLEAR_U *audio_ifc_int_clear;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH0_INTCLEAR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH1_INTCLEAR;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_int_clear->dwValue = clearbits;
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __ifc_set_int_clear_end_fifo(uint32 direction)
{
    AUDIO_IFC_INT_CLEAR_U *audio_ifc_int_clear;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH0_INTCLEAR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH1_INTCLEAR;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_int_clear->mBits.end_fifo = TRUE;
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __ifc_set_int_clear_half_fifo(uint32 direction)
{
    AUDIO_IFC_INT_CLEAR_U *audio_ifc_int_clear;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH0_INTCLEAR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH1_INTCLEAR;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_int_clear->mBits.half_fifo = TRUE;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_set_int_clear_quarter_fifo(uint32 direction)
{
    AUDIO_IFC_INT_CLEAR_U *audio_ifc_int_clear;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH0_INTCLEAR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH1_INTCLEAR;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_int_clear->mBits.quarter_fifo = TRUE;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_set_int_clear_three_quarter_fifo(uint32 direction)
{
    AUDIO_IFC_INT_CLEAR_U *audio_ifc_int_clear;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH0_INTCLEAR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH1_INTCLEAR;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_int_clear->mBits.three_quarter_fifo = TRUE;
    return AUDIO_HAL_SUCCESS;
}

LOCAL int32 __ifc_set_int_clear_ahb_error(uint32 direction)
{
    AUDIO_IFC_INT_CLEAR_U *audio_ifc_int_clear;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH0_INTCLEAR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_int_clear = (AUDIO_IFC_INT_CLEAR_U *) REG_AUD_IFC_CH1_INTCLEAR;
    else
        return AUDIO_HAL_ERROR;

    audio_ifc_int_clear->mBits.clear_ahb_error = TRUE;
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __ifc_get_cur_ahb_addr(uint32 direction)
{
    AUDIO_IFC_CUR_AHB_ADDR_U *audio_ifc_cur_ahb_addr;

    if(AUDIO_IFC_CAPTURE == direction)
        audio_ifc_cur_ahb_addr = (AUDIO_IFC_CUR_AHB_ADDR_U *) REG_AUD_IFC_CH0_CURAHBADDR;
    else if(AUDIO_IFC_PLAYBACK== direction)
        audio_ifc_cur_ahb_addr = (AUDIO_IFC_CUR_AHB_ADDR_U *) REG_AUD_IFC_CH1_CURAHBADDR;
    else
        return AUDIO_HAL_ERROR;

    return audio_ifc_cur_ahb_addr->dwValue;
}

/*****************************************************************************/
// hal_AifStream
// -----------------------------------------------------------------------------
// Play or record a stream from or to a buffer
// The buffer start address must be aligned on a 32-bit address, and the size
// must be a multiple of 32 bytes.
// @param xfer Describes the buffer and size
// @param direction Define the direction: AUDIO_IFC_PLAYBACK for play,
//         AUDIO_IFC_CAPTURE to record.
/*****************************************************************************/

PUBLIC int32 __ifc_start(uint32 addr, uint32 length, uint32 direction, uint8 is_dlul)
{
    uint32 irqMask = 0;
    int16 times_delay = 0;

    while (__ifc_get_enable_status(direction))
    {
        //sxr_Sleep(1 MS_WAITING);  //need change
        SCI_SLEEP(10);//osiDelayUS(1000);
        times_delay++;
        if (times_delay > 200)
        {
            AUDDRV_ERROR("__ifc_get_enable_status timeout ");
            AUDDRV_PASSERT(0, ("__ifc_start wait ifc ch status TimeOut!")); /*assert to do*/
            return AUDIO_HAL_ERROR;
        }
    }

    AUDDRV_DEBUG("addr:0x%x,length:0x%x,direction:%d,is_dlul:%d",
	    addr, length, direction, is_dlul);

    // Assert on word alignement
    AUDDRV_PASSERT(addr,
        ("AUDIO IFC transfer address NULL"));
    AUDDRV_PASSERT((addr) % 4 == 0,
        ("AUDIO IFC transfer start address not aligned"));

    // Size must be a multiple of 32 bytes
    AUDDRV_PASSERT((length) % 32 == 0,
        ("AUDIO IFC transfer size not mult. of 32-bits: 0x%x", length));

    __ifc_set_start_addr(direction, addr);
    __ifc_set_fifo_size(direction, length);

    irqMask = AUDIO_IFC_HALF_FIFO | AUDIO_IFC_END_FIFO;     //hxy
    //__ifc->ch[direction].int_mask = irqMask;              //hxy
    __ifc_set_int_mask(direction,irqMask);

/*
    if (is_dlul)
    {
        if (direction == AUDIO_IFC_PLAYBACK)
        {
            // Not to start the play stream
            return AUDIO_HAL_SUCCESS;
        }
        else if (direction == AUDIO_IFC_CAPTURE)
        {
            // Start the play stream just before starting the record one
            //__ifc->ch[AUDIO_IFC_PLAYBACK].control.mBits.enable = TRUE;
            __ifc_enable(AUDIO_IFC_PLAYBACK);
            //sxr_Sleep(13 MS_WAITING);
        }
    }
*/

    __ifc_enable(direction);

    AUDDRV_TRACE("ifc direction [%s] start success",
        (direction == AUDIO_IFC_PLAYBACK)?"playback":"capture");

    __ifc_print_reg();

    return AUDIO_HAL_SUCCESS;
}

// =============================================================================
// hal_AifStopPlay
// -----------------------------------------------------------------------------
/// Stop playing a buffer
// =============================================================================
PUBLIC int32 __ifc_StopPlay(void)
{
    uint32 times_delay = 0;
    int32 ret = AUDIO_HAL_SUCCESS;

    //g_halAifPlaying = FALSE;

    //__ifc->ch[AUDIO_IFC_PLAYBACK].control = AUDIO_IFC_DISABLE;
    //__ifc->ch[AUDIO_IFC_PLAYBACK].int_mask = 0;
    __ifc_disable(AUDIO_IFC_PLAYBACK);
    __ifc_set_int_mask(AUDIO_IFC_PLAYBACK,0);
    //sxr_Sleep(1 MS_WAITING);
    SCI_SLEEP(10);//osiDelayUS(1000);

    while (__ifc_get_enable_status(AUDIO_IFC_PLAYBACK))
    {
        //sxr_Sleep(1 MS_WAITING);
        SCI_SLEEP(10);//osiDelayUS(1000);
        //__ifc->ch[AUDIO_IFC_PLAYBACK].control = AUDIO_IFC_DISABLE;
        //__ifc->ch[AUDIO_IFC_PLAYBACK].int_mask = 0;
        __ifc_disable(AUDIO_IFC_PLAYBACK);
        __ifc_set_int_mask(AUDIO_IFC_PLAYBACK,0);

        times_delay++;
        if (times_delay > 10)
        {
            AUDDRV_ERROR("__ifc_get_enable_status timeout");
            //HAL_ASSERT(0, ("aud_:hal_AifStopPlay wait ifc ch status TimeOut!"));/*assert to do*/
            ret = AUDIO_HAL_ERROR;
        }
    }

    // Disable the AIF if not recording
    //UINT32 status = hal_SysEnterCriticalSection();
    //if (!g_halAifRecording)
    //{
        // To have the clock allowing the disabling.
        //hwp_aif->ctrl = AIF_PARALLEL_OUT_SET_PARA | AIF_PARALLEL_IN_SET_PARA;
    //}

    AUDDRV_TRACE("ifc stop play success");
    //hal_SysExitCriticalSection(status);

    return ret;
}

// =============================================================================
// hal_AifStopRecord
// -----------------------------------------------------------------------------
/// Stop playing a buffer
// =============================================================================
PUBLIC int32 __ifc_StopRecord(void)
{
    uint32 times_delay = 0;
    int32 ret = AUDIO_HAL_SUCCESS;

    //g_halAifRecording = FALSE;

    //__ifc->ch[AUDIO_IFC_CAPTURE].control = AUDIO_IFC_DISABLE;
    //__ifc->ch[AUDIO_IFC_CAPTURE].int_mask = 0;
    __ifc_disable(AUDIO_IFC_CAPTURE);
    __ifc_set_int_mask(AUDIO_IFC_CAPTURE,0);
    //sxr_Sleep(1 MS_WAITING);
    SCI_SLEEP(10);//osiDelayUS(1000);

    while (__ifc_get_enable_status(AUDIO_IFC_CAPTURE))
    {
        //sxr_Sleep(1 MS_WAITING);
        SCI_SLEEP(10);//osiDelayUS(1000);
        //__ifc->ch[AUDIO_IFC_CAPTURE].control = AUDIO_IFC_DISABLE;
        //__ifc->ch[AUDIO_IFC_CAPTURE].int_mask = 0;
        __ifc_disable(AUDIO_IFC_CAPTURE);
        __ifc_set_int_mask(AUDIO_IFC_CAPTURE,0);

        times_delay++;
        if (times_delay > 10)
        {
            AUDDRV_ERROR("__ifc_get_enable_status timeout");
            //AUDDRV_PASSERT(0, ("aud_:hal_AifStopRECORD wait ifc ch status TimeOut!")); /*assert to do*/
            ret = AUDIO_HAL_ERROR;
        }
    }

    // Disable the AIF if not recording
    //UINT32 status = hal_SysEnterCriticalSection();
    //if (!g_halAifPlaying)
    //{
        //hwp_aif->ctrl = AIF_PARALLEL_OUT_SET_PARA | AIF_PARALLEL_IN_SET_PARA;
    //}

    AUDDRV_TRACE("ifc stop record success");
    //hal_SysExitCriticalSection(status);

    return ret;
}

// =============================================================================
// hal_AifPlayReachedHalf
// -----------------------------------------------------------------------------
/// Check if the buffer has reached the middle and clear the status.
// =============================================================================
PUBLIC uint8 __ifc_PlayReachedHalf(void)
{
    uint8 ret = FALSE;

    if (__ifc_get_ihf_status(AUDIO_IFC_PLAYBACK))
    {
        //__ifc->ch[AUDIO_IFC_PLAYBACK].int_clear.mBits.half_fifo = TRUE;
        __ifc_set_int_clear_half_fifo(AUDIO_IFC_PLAYBACK);
        ret = TRUE;
    }

    return ret;
}

// =============================================================================
// hal_AifPlayReachedEnd
// -----------------------------------------------------------------------------
/// Check if the buffer has reached the end and clear the status.
// =============================================================================
PUBLIC uint8 __ifc_PlayReachedEnd(void)
{
    uint8 ret = FALSE;

    if (__ifc_get_ief_status(AUDIO_IFC_PLAYBACK))
    {
        __ifc_set_int_clear_end_fifo(AUDIO_IFC_PLAYBACK);
        ret = TRUE;
    }

    return ret;
}

// =============================================================================
// hal_AifRecordReachedHalf
// -----------------------------------------------------------------------------
/// Check if the buffer has reached the middle and clear the status.
// =============================================================================
PUBLIC uint8 __ifc_RecordReachedHalf(void)
{
    uint8 ret = FALSE;

    if (__ifc_get_ihf_status(AUDIO_IFC_CAPTURE))
    {
        __ifc_set_int_clear_half_fifo(AUDIO_IFC_CAPTURE);
        ret = TRUE;
    }

    return ret;
}

// =============================================================================
// hal_AifRecordReachedEnd
// -----------------------------------------------------------------------------
/// Check if the buffer has reached the end and clear the status.
// =============================================================================
PUBLIC uint8 __ifc_RecordReachedEnd(void)
{
    uint8 ret = FALSE;

    if (__ifc_get_ief_status(AUDIO_IFC_CAPTURE))
    {
        __ifc_set_int_clear_end_fifo(AUDIO_IFC_CAPTURE);
        ret = TRUE;
    }

    return ret;
}
/*
PUBLIC VOID __ifc_IrqClrStatus(uint32 direction)
{
    UINT32 status = __ifc->ch[direction].status & (AUDIO_IFC_CAUSE_IHF | AUDIO_IFC_CAUSE_IEF);
    // Clear cause
    __ifc->ch[direction].int_clear = status;
}

PUBLIC UINT32 __ifc_GetIfcStatus(uint32 direction)
{
    //return (UINT32)(__ifc->ch[direction].status & 0x00000F00);
    return (UINT32)(__ifc->ch[direction].status.dwValue & 0x00000F00);
}
*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


