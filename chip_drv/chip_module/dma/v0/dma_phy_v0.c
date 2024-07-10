/******************************************************************************
 ** File Name:    dma_phy_v0.c                                             *
 ** Author:       weihua.wang                                                 *
 ** DATE:         2005-9-17                                                   *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:  dma register operation                                      *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2005-9-17     weihua.wang     Create.                                     *
 ** 2005-12-28    weihua.wang     Modify cfg struct to fit 6800,the meaning of*
** 2010-3-3      Guofu.Huang     update for 6800h,define new dma interface   *
**                               burst step is different whih sc6600m        *
 ** 11/06/2010    Jeff.Li         Modify for new chip driver architecture,    *
 **                               and change file name to dma_v0_module.c.    *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "dma_drv_internal.h"
#include "mmu_drvapi.h"
#include "../dma_phy.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define DMA_PAUSE_NEST_MAX      10 // need check
#define DMA_PAUSE_TIME_OUT      0x2000
#define GET_DATA_STEP(_num_)    (((_num_) >= 0)?(_num_):(0x8000|abs(_num_)))
#define GET_BLOCK_STEP(_num_)   (((_num_) >= 0)?(_num_):(0x2000000|abs(_num_)))

#define GET_BLOCKDONE_RAWSTS(_chn_) ((REG32(DMA_BURST_INT_RAW)>>(_chn_))&0x1)
#define GET_TRANSDONE_RAWSTS(_chn_) ((REG32(DMA_TRANSF_INT_RAW)>>(_chn_))&0x1)
#define GET_LISTDONE_RAWSTS(_chn_)  ((REG32(DMA_LISTDONE_INT_RAW)>>(_chn_))&0x1)

#define BLOCKINT_CLR(_chn_)                     \
    DRV_MACRO_START                             \
    SCI_DisableIRQ();                           \
    REG32(DMA_BURST_INT_CLR) |= 1<<(_chn_);     \
    SCI_RestoreIRQ();                           \
    DRV_MACRO_END

#define TRANSINT_CLR(_chn_)                     \
    DRV_MACRO_START                             \
    SCI_DisableIRQ();                           \
    REG32(DMA_TRANSF_INT_CLR) |= 1<<(_chn_);    \
    SCI_RestoreIRQ();                           \
    DRV_MACRO_END

#define LISTINT_CLR(_chn_)                      \
    DRV_MACRO_START                             \
    SCI_DisableIRQ();                           \
    REG32(DMA_LISTDONE_INT_CLR) |= 1<<(_chn_);  \
    SCI_RestoreIRQ();                           \
    DRV_MACRO_END

#define ALLINT_CLR(_chn_)                       \
    DRV_MACRO_START                             \
    BLOCKINT_CLR(_chn_);                        \
    TRANSINT_CLR(_chn_);                        \
    LISTINT_CLR(_chn_);                         \
    DRV_MACRO_END

//int status type: 3bits value
//BIT_0: BLOCK DONE
//BIT_1: TRANS DONE
//BIT_2: LIST DONE
#define GET_INTSTS_TYPE(_chn_)                              \
    ((GET_BLOCKDONE_RAWSTS(_chn_)<<DMA_INT_BIT_BLOCK)|      \
     (GET_TRANSDONE_RAWSTS(_chn_)<<DMA_INT_BIT_TRANS)|      \
     (GET_LISTDONE_RAWSTS(_chn_)<<DMA_INT_BIT_LIST))

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
LOCAL DMA_HAL_ISR_HANDLER  s_dma_chx_handler[DMA_CH_NUM];
LOCAL volatile DMA_GEN_CTL_REG_T *s_dma_gen_ctl_ptr = NULL;
typedef struct
{
    uint32 ch;
    uint32 int_type;
} DMA_INT_DATA_T;

LOCAL DMA_INT_DATA_T dma_int_data;

INPUT_BUFFER_INIT (DMA_INT_DATA_T, DMA_CH_NUM)

/*----------------------------------------------------------------------------*
**                         Global Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL ISR_EXE_T DMA_IrqHandler (uint32 num);
LOCAL void  DmaHisrFunc (uint32 cnt, void *pData);
//LOCAL void DmaControlChanInt(uint32 channel, uint32 intType, BOOLEAN enable);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
LOCAL void DMA_ISR_PROCESS (uint32 ch)
{
    SCI_ASSERT (NULL != s_dma_chx_handler[ch]); /*assert verified*/

    dma_int_data.ch = ch;
    dma_int_data.int_type = GET_INTSTS_TYPE (ch);
    IsrWriteBuffer (dma_int_data);

    ALLINT_CLR (ch);
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
/*****************************************************************************/
/*lint -e{661} -e{662} confirmed by xuepeng zhang*/
LOCAL ISR_EXE_T DMA_IrqHandler (uint32 num)
{
    uint32 i;
    int32 status;

    status = REG32 (DMA_INT_STS);

    if (0 == status)
    {
        return ISR_DONE;
    }
    else
    {

        for (i = 0; i < DMA_CH_NUM; i += 4)
        {
            switch (status & 0xf)
            {
                case 0x1:
                    DMA_ISR_PROCESS (i + 0);
                    break;
                case 0x2:
                    DMA_ISR_PROCESS (i + 1);
                    break;
                case 0x3:
                    DMA_ISR_PROCESS (i + 0);
                    DMA_ISR_PROCESS (i + 1);
                    break;
                case 0x4:
                    DMA_ISR_PROCESS (i + 2);
                    break;
                case 0x5:
                    DMA_ISR_PROCESS (i + 0);
                    DMA_ISR_PROCESS (i + 2);
                    break;
                case 0x6:
                    DMA_ISR_PROCESS (i + 1);
                    DMA_ISR_PROCESS (i + 2);
                    break;
                case 0x7:
                    DMA_ISR_PROCESS (i + 0);
                    DMA_ISR_PROCESS (i + 1);
                    DMA_ISR_PROCESS (i + 2);
                    break;
                case 0x8:
                    DMA_ISR_PROCESS (i + 3);
                    break;
                case 0x9:
                    DMA_ISR_PROCESS (i + 0);
                    DMA_ISR_PROCESS (i + 3);
                    break;
                case 0xA:
                    DMA_ISR_PROCESS (i + 1);
                    DMA_ISR_PROCESS (i + 3);
                    break;
                case 0xB:
                    DMA_ISR_PROCESS (i + 0);
                    DMA_ISR_PROCESS (i + 1);
                    DMA_ISR_PROCESS (i + 3);
                    break;
                case 0xC:
                    DMA_ISR_PROCESS (i + 2);
                    DMA_ISR_PROCESS (i + 3);
                    break;
                case 0xD:
                    DMA_ISR_PROCESS (i + 0);
                    DMA_ISR_PROCESS (i + 2);
                    DMA_ISR_PROCESS (i + 3);
                    break;
                case 0xE:
                    DMA_ISR_PROCESS (i + 1);
                    DMA_ISR_PROCESS (i + 2);
                    DMA_ISR_PROCESS (i + 3);
                    break;
                case 0xF:
                    DMA_ISR_PROCESS (i + 0);
                    DMA_ISR_PROCESS (i + 1);
                    DMA_ISR_PROCESS (i + 2);
                    DMA_ISR_PROCESS (i + 3);
                    break;
                default:
                    break;
            }

            status >>= 4;
        }

        return CALL_HISR;
    }
}

/*****************************************************************************/
//  Description:    This function is Dma 's HISR.
//                  1. THE priority is higher than normal task.
//                  2. Is not real ISR.
//  Author:         steve.zhan
//  Note:
/*****************************************************************************/
LOCAL void  DmaHisrFunc (uint32 cnt, void *pData)
{
    DMA_INT_DATA_T data;

    while (!threadReadBuffer (&data))
    {
        s_dma_chx_handler[data.ch] (data.int_type);
    }

    return ;
}

//*****************************************************************************/
//  Description:    Init DMA controller, this function can only be called once
//                  when system initialization.
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
/*****************************************************************************/
PUBLIC void DMA_PHY_Init (void)
{
    int32 i;
    uint32  status    = 0;

    SCI_DisableIRQ();

    s_dma_gen_ctl_ptr = (volatile DMA_GEN_CTL_REG_T *) DMA_REG_BASE;

    DMA_ENDIAN_SELECT;

    //register init
    s_dma_gen_ctl_ptr->cfg = 0;
    //all control disable
    if(SC6800H == CHIP_GetChipType())
    {
        s_dma_gen_ctl_ptr->chx_en_sts= 0;//only for sc6800h1 chip
    }
    else
    {
        s_dma_gen_ctl_ptr->chx_dis = 0xFFFFFFFF;
    }
    
    s_dma_gen_ctl_ptr->linklist_en = 0;
    s_dma_gen_ctl_ptr->softlist_en = 0;

    /*dma prority set*/
    for (i = 0; i < DMA_PRI_REG_COUNT; i++)
    {
        s_dma_gen_ctl_ptr->priority[i] = 0;
    }

    for (i = 0; i < DMA_UID_REG_COUNT; i++)
    {
        s_dma_gen_ctl_ptr->uid[i] = 0;
    }

    /*disable all channel interrupt*/
    s_dma_gen_ctl_ptr->transdone_int_clr = 0xFFFFFFFF;
    s_dma_gen_ctl_ptr->blockdone_int_clr = 0xFFFFFFFF;
    s_dma_gen_ctl_ptr->listdone_int_clr  = 0xFFFFFFFF;

    s_dma_gen_ctl_ptr->transdone_int_en = 0;
    s_dma_gen_ctl_ptr->blockdone_int_en = 0;
    s_dma_gen_ctl_ptr->listdone_int_en  = 0;

    //setting software&hardware block time
    s_dma_gen_ctl_ptr->cfg &= ~DMA_SOFTBLOCK_MASK;
    s_dma_gen_ctl_ptr->cfg |= (DMA_SOFT_BLOCK_WAITTIME<<DMA_SOFTBLOCK_OFFSET) &DMA_SOFTBLOCK_MASK;

    s_dma_gen_ctl_ptr->cfg &= ~DMA_HARDBLOCK_MASK;
    s_dma_gen_ctl_ptr->cfg |= (DMA_HARD_BLOCK_WAITTIME<<DMA_HARDBLOCK_OFFSET) &DMA_HARDBLOCK_MASK;

    /*register dma irq handle to host*/
    status = ISR_RegHandler_Ex (TB_DMA_INT, DMA_IrqHandler, DmaHisrFunc, CHIPDRV_HISR_PRIO_0, NULL);

    if (TB_SUCCESS == status)
    {
        /*enable dma int*/
        CHIPDRV_EnableIRQINT (TB_DMA_INT);
    }

    SCI_RestoreIRQ();

}

//*****************************************************************************/
//  Description:    pause or resume dma transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           is_pause: SCI_TRUE=pause,SCI_FALSE=resume
/*****************************************************************************/
PUBLIC uint32 DMA_PHY_AllocChn(DMA_CHN_TRSF_TYPE_E type)
{
    return 0;
}

//*****************************************************************************/
//  Description:    pause or resume dma transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           is_pause: SCI_TRUE=pause,SCI_FALSE=resume
/*****************************************************************************/
PUBLIC void DMA_PHY_FreeChn(uint32 channel)
{
    return;
}

//*****************************************************************************/
//  Description:    pause or resume dma transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           is_pause: SCI_TRUE=pause,SCI_FALSE=resume
/*****************************************************************************/
PUBLIC void DMA_PHY_SetPause (BOOLEAN is_pause)
{
    static int32 s_dma_pause_nest = 0;
    uint32 time_out;

    SCI_ASSERT (s_dma_pause_nest < DMA_PAUSE_NEST_MAX);/*assert verified*/

    SCI_DisableIRQ();

    if (is_pause)
    {
        s_dma_gen_ctl_ptr->cfg |=  DMA_PAUSE_ENABLE;
        s_dma_pause_nest ++;
    }
    else if (--s_dma_pause_nest <= 0)
    {
        s_dma_gen_ctl_ptr->cfg &= ~DMA_PAUSE_ENABLE;
    }

    SCI_RestoreIRQ();

    time_out = 0;

    if (is_pause)
    {
        while (! (s_dma_gen_ctl_ptr->transfer_sts & DMA_PAUSE_STAT_BIT))
        {
            time_out++;

            if (time_out > DMA_PAUSE_TIME_OUT)
            {
                SCI_PASSERT (0, ("DMA PAUSE TIME OUT!"));/*assert verified*/
                break; /*lint !e527 comfirmed by xuepeng*/
            }
        }
    }

}

//*****************************************************************************/
//  Description:    setting dma wrap addr
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           wrapstart_addr: start addr
//                  wrapend_addr:end addr
/*****************************************************************************/
PUBLIC void DMA_PHY_SetWrapAddr (uint32 channel, uint32 wrapstart_addr, uint32 wrapend_addr)
{
    SCI_DisableIRQ();
    s_dma_gen_ctl_ptr->wrap_startaddr &= ~DMA_WRAP_ADDR_MASK;
    s_dma_gen_ctl_ptr->wrap_startaddr |= (wrapstart_addr << DMA_WRAP_ADDR_OFFSET) & DMA_WRAP_ADDR_MASK;

    s_dma_gen_ctl_ptr->wrap_endaddr   &= ~DMA_WRAP_ADDR_MASK;
    s_dma_gen_ctl_ptr->wrap_endaddr   |= (wrapend_addr << DMA_WRAP_ADDR_OFFSET) & DMA_WRAP_ADDR_MASK;
    SCI_RestoreIRQ();
}

//*****************************************************************************/
//  Description:    setting dma channnel user id
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  uid:channel user id
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnUID (uint32 channel, uint32 uid)
{
    uint32 regindex;
    uint32 offset;

    regindex = DMA_CHx_UID_INDEX (channel);
    offset = DMA_CHx_UID_OFFSET (channel);

    SCI_DisableIRQ();
    s_dma_gen_ctl_ptr->uid[regindex] &= ~ ( (uint32) DMA_CHx_UID_MASK << offset);
    s_dma_gen_ctl_ptr->uid[regindex] |= ( (uid & DMA_CHx_UID_MASK) << offset);
    SCI_RestoreIRQ();
}
//*****************************************************************************/
//  Description:    setting dma channnel work mode
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  workmode:channel work mode
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnWorkMode (uint32 channel, DMA_CHN_WORKMODE_E workmode)
{
    SCI_DisableIRQ();

    switch (workmode)
    {
        case DMA_CHN_WORKMODE_NORMAL:
            s_dma_gen_ctl_ptr->linklist_en &= ~ (0x01UL << channel);
            s_dma_gen_ctl_ptr->softlist_en &= ~ (0x01UL << channel);
            break;
        case DMA_CHN_WORKMODE_LINKLIST:
            s_dma_gen_ctl_ptr->linklist_en |= (0x01 << channel);
            s_dma_gen_ctl_ptr->softlist_en &= ~ (0x01UL << channel);
            break;
        case DMA_CHN_WORKMODE_SOFTLIST:
            s_dma_gen_ctl_ptr->linklist_en &= ~ (0x01UL << channel);
            //only one channel can running in soft mode
            s_dma_gen_ctl_ptr->softlist_en  = (0x01 << channel);
            break;
        default:
            break;
    }

    SCI_RestoreIRQ();
}
//*****************************************************************************/
//  Description:    setting dma channel priority
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  priority: new priority value
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnPriority (uint32 chn, DMA_CHN_PRIORITY_E pri)
{
    SCI_DisableIRQ();

    if (chn <= 15)
    {
        s_dma_gen_ctl_ptr->priority[0] &= ~ (0x03) << (chn<<1);
        s_dma_gen_ctl_ptr->priority[0] |= ( (pri) &0x03) << (chn<<1);
    }
    else
    {
        s_dma_gen_ctl_ptr->priority[1] &= ~ (0x03) << ( (chn-16) <<1);
        s_dma_gen_ctl_ptr->priority[1] |= ( (pri) &0x03) << ( (chn-16) <<1);
    }

    SCI_RestoreIRQ();
}

LOCAL uint32 s_dma_sleep_flag = 0;

//*****************************************************************************/
//  Description:    Enable/Disable a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  is_enable: enable/disable a chn
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnEnable_Ex (uint32 channel, BOOLEAN is_enable)
{

    SCI_DisableIRQ();
    SCI_DisableFIQ();

    ALLINT_CLR (channel);

    if(SC6800H == CHIP_GetChipType())
    {
        DMA_PHY_SetPause (TRUE);

        if (is_enable)
        {
            s_dma_gen_ctl_ptr->chx_en_sts|= (0x01 << channel);
        }
        else
        {
            s_dma_gen_ctl_ptr->chx_en_sts&= ~ (0x01UL << channel);
        }

        DMA_PHY_SetPause (FALSE);
    }
    else
    {
        if (is_enable)
        {
            s_dma_gen_ctl_ptr->chx_en  = (0x01 << channel);
        }
        else
        {
            s_dma_gen_ctl_ptr->chx_dis = (0x01 << channel);
        }
    }

    //For a soft chn, when enable a chn, soft request is needed.
    if (is_enable && (DMA_CHN_TYPE_SOFT == DMA_CFG_GetChnType (channel)))
    {
        s_dma_gen_ctl_ptr->soft_request = (0x01 << channel);
    }

    //add DMA protect when enter deepsleep
    if (is_enable)
    {
        s_dma_sleep_flag |= (1 << channel);
        SCI_DMA_EnableDeepSleep (DISABLE_DEEP_SLEEP);
    }
    else
    {
        s_dma_sleep_flag &= ~ (1UL << channel);

        if (!s_dma_sleep_flag)
        {
            SCI_DMA_EnableDeepSleep (ENABLE_DEEP_SLEEP);
        }
    }

    SCI_RestoreFIQ();
    SCI_RestoreIRQ();
}
//*****************************************************************************/
//  Description:    Enable/Disable a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  is_enable: enable/disable a chn
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnEnable (uint32 channel, BOOLEAN is_enable)
{
    SCI_DisableIRQ();
    SCI_DisableFIQ();

    if (is_enable)
    {
        MMU_InvalideDCACHE();
    }

    ALLINT_CLR (channel);

    if(SC6800H == CHIP_GetChipType())
    {
        DMA_PHY_SetPause (TRUE);

        if (is_enable)
        {
            s_dma_gen_ctl_ptr->chx_en_sts|= (0x01 << channel);
        }
        else
        {
            s_dma_gen_ctl_ptr->chx_en_sts&= ~ (0x01UL << channel);
        }

        DMA_PHY_SetPause (FALSE);
    }
    else
    {
        if (is_enable)
        {
            s_dma_gen_ctl_ptr->chx_en  = (0x01 << channel);
        }
        else
        {
            s_dma_gen_ctl_ptr->chx_dis = (0x01 << channel);
        }
    }

    //For a soft chn, when enable a chn, soft request is needed.
    if (is_enable && (DMA_CHN_TYPE_SOFT == DMA_CFG_GetChnType (channel)))
    {
        s_dma_gen_ctl_ptr->soft_request = (0x01 << channel);
    }

    //add DMA protect when enter deepsleep
    if (is_enable)
    {
        s_dma_sleep_flag |= (1 << channel);
        SCI_DMA_EnableDeepSleep (DISABLE_DEEP_SLEEP);
    }
    else
    {
        s_dma_sleep_flag &= ~ (1UL << channel);

        if (!s_dma_sleep_flag)
        {
            SCI_DMA_EnableDeepSleep (ENABLE_DEEP_SLEEP);
        }
    }

    SCI_RestoreFIQ();
    SCI_RestoreIRQ();
}

//*****************************************************************************/
//  Description:    configure dma interrupt
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  inttype: 3 bits value
//                  is_enable:enable/disable interrupt
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnIntConfig (uint32 channel, uint32 inttype, DMA_HAL_ISR_HANDLER callback)
{
    SCI_DisableIRQ();

    s_dma_gen_ctl_ptr->transdone_int_en &= ~ (0x01UL << channel);
    s_dma_gen_ctl_ptr->blockdone_int_en &= ~ (0x01UL << channel);
    s_dma_gen_ctl_ptr->listdone_int_en  &= ~ (0x01UL << channel);

    if (NULL == inttype || NULL == callback)
    {
        s_dma_chx_handler[channel] = NULL;
    }
    else
    {
        if (inttype&DMA_CHN_INT_TYPE_BLOCKDONE)
        {
            s_dma_gen_ctl_ptr->blockdone_int_en |= (0x01 << channel);
        }

        if (inttype&DMA_CHN_INT_TYPE_TRANSDONE)
        {
            s_dma_gen_ctl_ptr->transdone_int_en |= (0x01 << channel);
        }

        if (inttype&DMA_CHN_INT_TYPE_LISTDONE)
        {
            s_dma_gen_ctl_ptr->listdone_int_en  |= (0x01 << channel);
        }

        s_dma_chx_handler[channel] = callback;
    }

    SCI_RestoreIRQ();
}

//*****************************************************************************/
//  Description:    configure dma channel related register
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           chnparam: dma chn configure structure
//                  chn_ctl_ptr: a pointer of dma chn control register array
/*****************************************************************************/
PUBLIC void DMA_PHY_FillChnCtlReg (DMA_CHNCFG_PTR chnparam, DMA_CHN_CTL_REG_T_PTR dma_chn_ctl_ptr)
{
    uint32 complementalcode = 0;
    uint32 cfg  = 0;
    uint32 sdep = 0;
    uint32 sbp  = 0;
    uint32 dbp  = 0;
    //cfg,ll_end
    cfg = 0;

    if (chnparam->is_ll_end)
    {
        cfg |= (uint32) DMA_CHx_LL_END_YES;
    }
    else
    {
        cfg |= (uint32) DMA_CHx_LL_END_NO;
    }

    //endian_type and  endian_switchmode
    switch (chnparam->endian_type)
    {
        case DMA_ENDIANTYPE_BIG:
            cfg |= (uint32) DMA_CHx_ENDIANSEL_BIG;
            break;
        case DMA_ENDIANTYPE_LITTLE:
            cfg |= (uint32) DMA_CHx_ENDIANSEL_LITTLE;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    switch (chnparam->endian_switchmode)
    {

        case DMA_ENDIANSWITCHMODE_UN:
            cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_UN;
            break;
        case DMA_ENDIANSWITCHMODE_FULL:
            cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_FULL;
            break;
        case DMA_ENDIANSWITCHMODE_MODE0:
            cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_MODE0;
            break;
        case DMA_ENDIANSWITCHMODE_MODE1:
            cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_MODE1;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    //src datawidth and des datawidth
    switch (chnparam->src_datawidth)
    {
        case DMA_DATAWIDTH_BYTE:
            cfg |= (uint32) DMA_CHx_SRC_DATAWIDTH_BYTE;
            break;
        case DMA_DATAWIDTH_HALFWORD:
            cfg |= (uint32) DMA_CHx_SRC_DATAWIDTH_HALFWORD;
            break;
        case DMA_DATAWIDTH_WORD:
            cfg |= (uint32) DMA_CHx_SRC_DATAWIDTH_WORD;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    switch (chnparam->dest_datawidth)
    {
        case DMA_DATAWIDTH_BYTE:
            cfg |= (uint32) DMA_CHx_DEST_DATAWIDTH_BYTE;
            break;
        case DMA_DATAWIDTH_HALFWORD:
            cfg |= (uint32) DMA_CHx_DEST_DATAWIDTH_HALFWORD;
            break;
        case DMA_DATAWIDTH_WORD:
            cfg |= (uint32) DMA_CHx_DEST_DATAWIDTH_WORD;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    //request mode
    switch (chnparam->requestmode)
    {
        case DMA_CHN_REQMODE_NORMAL:
            cfg |= (uint32) DMA_CHx_REQMODE_NORMAL;
            break;
        case DMA_CHN_REQMODE_TRASACTION:
            cfg |= (uint32) DMA_CHx_REQMODE_TRANSACTION;
            break;
        case DMA_CHN_REQMODE_LIST:
            cfg |= (uint32) DMA_CHx_REQMODE_LIST;
            break;
        case DMA_CHN_REQMODE_INFINITE:
            cfg |= (uint32) DMA_CHx_REQMODE_INFINITE;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    //src and des wrap enable
    if (chnparam->is_srcwrapen)
    {
        cfg |= (uint32) DMA_CHx_SRCWRAP_ENABLE;
    }
    else
    {
        cfg |= (uint32) DMA_CHx_SRCWRAP_DISABLE;
    }

    if (chnparam->is_destwrapen)
    {
        cfg |= (uint32) DMA_CHx_DESTWRAP_ENABLE;
    }
    else
    {
        cfg |= (uint32) DMA_CHx_DESTWRAP_DISABLE;
    }

    //is no auto close
    if (chnparam->is_noautoclose)
    {
        cfg |= (uint32) DMA_CHx_NOAUTO_CLOSE_YES;
    }
    else
    {
        cfg |= (uint32) DMA_CHx_NOAUTO_CLOSE_NO;
    }

    //block length
    cfg |=
        (chnparam->block_length << DMA_CHx_BLOCKLENGTH_OFFSET) & DMA_CHx_BLOCKLENGTH_MASK;

    //update cfg register
    dma_chn_ctl_ptr->cfg = cfg;

    //totla length
    dma_chn_ctl_ptr->total_length = (uint32)
                                    (chnparam->total_length << ( (uint32) DMA_CHx_TOTALLENGTH_OFFSET)) & ( (uint32) DMA_CHx_TOTALLENGTH_MASK);

    // src and des address
    dma_chn_ctl_ptr->src_addr  = MMU_VirToPhy (chnparam->src_address);
    dma_chn_ctl_ptr->dest_addr = MMU_VirToPhy (chnparam->dest_address);

    //ll_ptr
    dma_chn_ctl_ptr->ll_ptr = chnparam->ll_next;

    sdep = 0;
    //src step and des step
    complementalcode = GET_DATA_STEP (chnparam->src_step);
    sdep |= (uint32) (complementalcode << ( (uint32) DMA_CHx_SRC_STEP_OFFSET)) & ( (uint32) DMA_CHx_SRC_STEP_MASK);

    complementalcode = GET_DATA_STEP (chnparam->dest_step);
    sdep |= (complementalcode << ( (uint32) DMA_CHx_DEST_STEP_OFFSET)) & ( (uint32) DMA_CHx_DEST_STEP_MASK);
    dma_chn_ctl_ptr->sdep = sdep;

    sbp = 0;

    //src block mode and block step
    switch (chnparam->src_blockmode)
    {
        case DMA_BLOCKMODE_SINGLETRANS:
            sbp |= (uint32) DMA_CHx_BLOCKMODE_SIGNLE;
            break;
        case DMA_BLOCKMODE_INCR:
            sbp |= (uint32) DMA_CHx_BLOCKMODE_INCR;
            break;
        case DMA_BLOCKMODE_INCR4:
            sbp |= (uint32) DMA_CHx_BLOCKMODE_INCR4;
            break;
        case DMA_BLOCKMODE_INCR8:
            sbp |= (uint32) DMA_CHx_BLOCKMODE_INCR8;
            break;
        case DMA_BLOCKMODE_INCR16:
            sbp |= (uint32) DMA_CHx_BLOCKMODE_INCR16;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    complementalcode = GET_BLOCK_STEP (chnparam->src_blockstep);
    sbp |= (complementalcode << ( (uint32) DMA_CHx_BLOCKSTEP_OFFSET)) & ( (uint32) DMA_CHx_BLOCKSTEP_MASK);
    dma_chn_ctl_ptr->sbp = sbp;

    dbp = 0;

    //des block mode and block step
    switch (chnparam->dest_blockmode)
    {
        case DMA_BLOCKMODE_SINGLETRANS:
            dbp |= (uint32) DMA_CHx_BLOCKMODE_SIGNLE;
            break;
        case DMA_BLOCKMODE_INCR:
            dbp |= (uint32) DMA_CHx_BLOCKMODE_INCR;
            break;
        case DMA_BLOCKMODE_INCR4:
            dbp |= (uint32) DMA_CHx_BLOCKMODE_INCR4;
            break;
        case DMA_BLOCKMODE_INCR8:
            dbp |= (uint32) DMA_CHx_BLOCKMODE_INCR8;
            break;
        case DMA_BLOCKMODE_INCR16:
            dbp |= (uint32) DMA_CHx_BLOCKMODE_INCR16;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    complementalcode = GET_BLOCK_STEP (chnparam->dest_blockstep);
    dbp|= (complementalcode << ( (uint32) DMA_CHx_BLOCKSTEP_OFFSET)) & ( (uint32) DMA_CHx_BLOCKSTEP_MASK);
    dma_chn_ctl_ptr->dbp = dbp;

}
//*****************************************************************************/
//  Description:    configure dma channel related register
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  chnparam: dma chn configure structure
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnConfig (uint32 channel,DMA_CHNCFG_PTR chnparam)
{
    DMA_CHN_CTL_REG_T *dma_chn_ctl_ptr;

    dma_chn_ctl_ptr = (DMA_CHN_CTL_REG_T *) DMA_CHx_CTL_REG_BASE (channel);

    //setting workmode and related registers
    switch (chnparam->workmode)
    {
        case DMA_CHN_WORKMODE_NORMAL:
            SCI_DisableIRQ();
            s_dma_gen_ctl_ptr->linklist_en &= ~ (0x01UL << channel);
            s_dma_gen_ctl_ptr->softlist_en &= ~ (0x01UL << channel);
            SCI_RestoreIRQ();
            DMA_PHY_FillChnCtlReg (chnparam, dma_chn_ctl_ptr);
            break;
        case DMA_CHN_WORKMODE_LINKLIST:
            SCI_DisableIRQ();
            s_dma_gen_ctl_ptr->linklist_en |= (0x01 << channel);
            s_dma_gen_ctl_ptr->softlist_en &= ~ (0x01UL << channel);
            dma_chn_ctl_ptr->ll_ptr = chnparam->ll_next;
            SCI_RestoreIRQ();
            break;
        case DMA_CHN_WORKMODE_SOFTLIST: // not support softlist
            SCI_ASSERT (0);/*assert verified*/
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }
}
/*****************************************************************************/
//  Description:    get dma channel transdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_PollingTransDone (uint32 channel)
{
    BOOLEAN ret;

    if (GET_TRANSDONE_RAWSTS (channel))
    {
        TRANSINT_CLR (channel);
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}
/*****************************************************************************/
//  Description:    get dma channel blockdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_PollingBlockDone (uint32 channel)
{
    BOOLEAN ret;

    if (GET_BLOCKDONE_RAWSTS (channel))
    {
        BLOCKINT_CLR (channel);
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description:    get dma channel listdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_PollingListDone (uint32 channel)
{
    BOOLEAN ret;

    if (GET_LISTDONE_RAWSTS (channel))
    {
        LISTINT_CLR (channel);
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

//*****************************************************************************/
//  Description:    get next nodeptr of list when using link list or soft list
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Return Value:   return the ptr of next node
/*****************************************************************************/
PUBLIC uint32 DMA_PHY_GetLinkListLLPtr (uint32 channel)
{
    DMA_CHN_CTL_REG_T *dma_chn_ctl_ptr;

    dma_chn_ctl_ptr = (DMA_CHN_CTL_REG_T *) DMA_CHx_CTL_REG_BASE (channel);

    return dma_chn_ctl_ptr->ll_ptr;
}
//*****************************************************************************/
//  Description:    get the ll_end flag
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Return Value:   return SCI_TRUE if LL_END is 1, otherwise return 0;
/*****************************************************************************/
PUBLIC BOOLEAN DMA_PHY_IsLinkListLLEnd (uint32 channel)
{
    DMA_CHN_CTL_REG_T *dma_chn_ctl_ptr;

    dma_chn_ctl_ptr = (DMA_CHN_CTL_REG_T *) DMA_CHx_CTL_REG_BASE (channel);

    return ( (0 != (dma_chn_ctl_ptr->cfg & ( (uint32) DMA_CHx_LL_END_MASK))) ? SCI_TRUE: SCI_FALSE);
}

//*****************************************************************************/
//  Description:    Set the node is the end
//  Global resource dependence:
//  Author:         Guofu.Huang
//                  node_ptr:pointer of the node to be modified
//                  is_ll_end: is Linklist end or not
//  Note:           return the ptr of next node
/*****************************************************************************/
PUBLIC void DMA_PHY_SetLinkListNodeLLEnd (DMA_CHN_CTL_REG_T_PTR node_ptr, BOOLEAN is_ll_end)
{
    SCI_DisableIRQ();
    node_ptr->cfg &= ( (uint32) DMA_CHx_LL_END_MASK);

    if (is_ll_end)
    {
        node_ptr->cfg |= (uint32) DMA_CHx_LL_END_YES;
    }
    else
    {
        node_ptr->cfg &= (uint32) DMA_CHx_LL_END_NO;
    }

    SCI_RestoreIRQ();
}
#ifdef   __cplusplus
}
#endif
