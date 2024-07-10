/******************************************************************************
 ** File Name:    dma_hal.c                                                     *
 ** Author:       Guofu.Huang                                                 *
 ** DATE:         2010-2-9                                                   *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2010-2-9      Guofu.Huang     Created
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "dma_drvapi.h"
#include "dma_drv_internal.h"
#include "dma_phy.h"
#include "tx_port_thumb.h"
#include "task_monitor.h"
#include "ifc_drvapi.h"

#ifdef VM_SUPPORT
//#include "mmu_drvapi.h "
#endif //VM_SUPPORT

/**---------------------------------------------------------------------------*
**                             Compiler Flag                                  *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define DMA_LINKLIST_HANDLE_BASE      (100)
#define DMA_LINKLIST_HANDLE_MAX       (DMA_LINKLIST_HANDLE_BASE + DMA_CH_NUM)
#define DMA_SOFT_BURSTSIZE            (0x100)
#define DMA_CHANNELID_MAX             (DMA_CH_NUM-1)
#define DMA_TRANS_TIMEOUT         (10000)

/*----------------------------------------------------------------------------*
**                             Structures Definitions                         *
**---------------------------------------------------------------------------*/
typedef struct _DMA_LIST_MNG
{
    uint32         list_table_in;
    uint32         list_table_last;
    uint32         list_len;
    BOOLEAN        is_busy;
    BOOLEAN        is_configured;
    BOOLEAN        is_running;
} DMA_LIST_MNG;

#if 0
typedef enum DMA_APP_TYPE_TAG
{
    DMA_APP_NONE,
    DMA_APP_MEMCPY,
    DMA_APP_MEMSET,
    DMA_APP_MAX
} DMA_APP_TYPE_E;
#endif

typedef struct DMA_SOFT_CHN_TBL_TAG
{
    uint32 index_max;            /* max software dma channel*/
    uint32 channel[DMA_CH_NUM];  /* to restore the channel  */
} DMA_SOFT_CHN_TBL_T;

/*----------------------------------------------------------------------------*
**                             data prototype                                 *
**---------------------------------------------------------------------------*/
LOCAL DMA_LIST_MNG          s_dma_list_mng[DMA_CH_NUM];
LOCAL DMA_CHN_CTL_REG_T     s_list_table[DMA_CH_NUM][DMA_LIST_SIZE_MAX];
LOCAL DMA_SOFT_CHN_TBL_T    s_dma_softchn_tbl;
//used to record the disable counter of dma pause function
LOCAL uint32 s_dma_pause_disable_cnt = 0;
//used to record the dma pause status
LOCAL BOOLEAN s_is_dma_pause = FALSE;
/*----------------------------------------------------------------------------*
**                         Global Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    dma driver init
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_Init (void)
{
    //init hardware
    DMA_PHY_Init();

	hal_IfcOpen(); /* IFC DMA INIT */
}

/*****************************************************************************/
// Description :    This function is used to enable or disable dma pause function.
// Global resource dependence : s_dma_pause_disable_cnt
// Author :
// Note : is_enable :   TURE: enable dma pause function; FALSE: disable dma pause function
/*****************************************************************************/
PUBLIC void DMA_HAL_EnableDMAPause (BOOLEAN is_enable)
{
    TX_IRQ_SAVE_AREA

    TX_DISABLE_IRQ

    TM_SendTestPointRequest (0x09090900 + s_dma_pause_disable_cnt, is_enable);

    if (is_enable)
    {
        if (0 != s_dma_pause_disable_cnt)
        {
            s_dma_pause_disable_cnt--;
        }
    }
    else
    {
        s_dma_pause_disable_cnt++;
    }

    TX_RESTORE_IRQ
}

/*****************************************************************************/
// Description :    This function is used to get the function status of dma pause
// Global resource dependence : s_dma_pause_disable_cnt
// Author :
// Note : TURE: dma pause function is enabled; FALSE: dma pause function is disabled
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_IsEnableDMAPause (void)
{
    return ( (0 == s_dma_pause_disable_cnt) ? SCI_TRUE:SCI_FALSE);
}

/*****************************************************************************/
// Description :    This function is used to get the dma pause status
// Global resource dependence : s_is_dma_pause
// Author :
// Note : TURE: dma pause function is enabled; FALSE: dma pause function is disabled
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_IsDMAPause (void)
{
    return s_is_dma_pause;
}

/*****************************************************************************/
//  Description:    set dma pause
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           s_pause:SCI_TRUE,pause dma; SCI_FALSE,resume dma
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_SetPause (BOOLEAN is_pause)
{
    if (is_pause)
    {
        if (DMA_HAL_IsEnableDMAPause())
        {
            DMA_PHY_SetPause (is_pause);
            s_is_dma_pause = is_pause;
        }
    }
    else
    {
        DMA_PHY_SetPause (is_pause);
        s_is_dma_pause = is_pause;
    }
}

/*****************************************************************************/
//  Description:    dma memcpy
//  Global resource dependence:
//  Author:         hao.liu
//  input param     des        : New buffer
//                  src        : Buffer to copy from
//                  count      : Number of characters to copy in bytes
//  return value               : param error means timeout
//                             : 0xffffffff means available channel
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_Memcpy (void *des, void *src, uint32 count)
{
#ifdef PLATFORM_SC6530
    uint32                 channel;
    DMA_CHNCFG_T           channelcfg={0};
    DMA_CAHCECONFIG_T      cacheConfig[2];
    uint32                 wait_time;

    SCI_ASSERT ((des != SCI_NULL)||(src != SCI_NULL));/*assert verified*/

    channelcfg.type = DMA_CHN_TYPE_SOFT;
    channelcfg.workmode = DMA_CHN_WORKMODE_NORMAL;
    channelcfg.requestmode= DMA_CHN_REQMODE_TRASACTION;
    channelcfg.src_address = (uint32)src;
    channelcfg.dest_address = (uint32)des;
    channelcfg.total_length  = count;
    channelcfg.block_length  = 256;
    channelcfg.frag_length  = 32;
    channelcfg.priority = 0;
    channelcfg.src_datawidth = DMA_DATAWIDTH_WORD;
    channelcfg.dest_datawidth = DMA_DATAWIDTH_WORD;
    channelcfg.src_step =4;
    channelcfg.dest_step=4;

    cacheConfig[0].bufferAddr = (uint32)src;
    cacheConfig[0].bufferSize = count;
    cacheConfig[0].dir = DMABUFFER_TO_DEVICE;

    cacheConfig[1].bufferAddr = (uint32)des;
    cacheConfig[1].bufferSize = count;
    cacheConfig[1].dir = DMABUFFER_FROM_DEVICE;
    
    channel = DMA_HAL_AllocChn (DMA_CHN_TRSF_TYPE_FULL);
    if (DMA_CHANNELID_INVALID == channel)
        return DMA_CHANNELID_INVALID;

    DMA_HAL_ChnConfig(channel, &channelcfg);

    //disable all interrupt
    DMA_HAL_ChnIntConfig(channel, DMA_CHN_INT_TYPE_NONE, NULL);

    //enable dma channel
    DMA_HAL_ChnEnable_Ex(channel, SCI_TRUE, cacheConfig, 2);

    if(DMA_CHN_TYPE_SOFT == channelcfg.type)
    {
        DMA_PHY_ChnRequest(channel);
    }

    //waiting for dma transfer finished
    wait_time = 0;
    while(DMA_TRANS_TIMEOUT > wait_time)
    {
        if(DMA_HAL_PollingTransDone(channel))
        {
            break;
        }
        SCI_Sleep(1);
        wait_time += 1;
    }
    if(DMA_TRANS_TIMEOUT <= wait_time)
    {
        return 0;
    }

    //disable dma channel
    DMA_HAL_ChnEnable_Ex(channel, SCI_FALSE, cacheConfig, 2);
    DMA_HAL_FreeChn(channel);

#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    uint32                 channel;
    DMA_CHNCFG_T           channelcfg={0};
    DMA_CAHCECONFIG_T      cacheConfig[2];
    uint32                 wait_time;

    SCI_ASSERT ((des != SCI_NULL)||(src != SCI_NULL));/*assert verified*/

    channelcfg.type = DMA_CHN_TYPE_SOFT;
    channelcfg.workmode = DMA_CHN_WORKMODE_NORMAL;
    channelcfg.requestmode= DMA_CHN_REQMODE_TRASACTION;
    channelcfg.src_address = (uint32)src;
    channelcfg.dest_address = (uint32)des;
    channelcfg.total_length  = count;
    channelcfg.block_length  = 256;
    channelcfg.frag_length  = 32;
    channelcfg.priority = 0;
    channelcfg.src_datawidth = DMA_DATAWIDTH_WORD;
    channelcfg.dest_datawidth = DMA_DATAWIDTH_WORD;
    channelcfg.src_step =4;
    channelcfg.dest_step=4;

    cacheConfig[0].bufferAddr = (uint32)src;
    cacheConfig[0].bufferSize = count;
    cacheConfig[0].dir = DMABUFFER_TO_DEVICE;

    cacheConfig[1].bufferAddr = (uint32)des;
    cacheConfig[1].bufferSize = count;
    cacheConfig[1].dir = DMABUFFER_FROM_DEVICE;

#if defined(PLATFORM_UWS6121E)
    channel = DMA_HAL_AllocChn (DMA_CHN_TRSF_TYPE_STANDARD);
    if (DMA_CHANNELID_INVALID == channel)
        return DMA_CHANNELID_INVALID;
#else
    channel = DMA_HAL_AllocChn (DMA_CHN_TRSF_TYPE_FULL);
    if (DMA_CHANNELID_INVALID == channel)
        return DMA_CHANNELID_INVALID;
#endif
    DMA_HAL_ChnConfig(channel, &channelcfg);

    //disable all interrupt
    DMA_HAL_ChnIntConfig(channel, DMA_CHN_INT_TYPE_NONE, NULL);

    //enable dma channel
    DMA_HAL_ChnEnable_Ex(channel, SCI_TRUE, cacheConfig, 2);

    if(DMA_CHN_TYPE_SOFT == channelcfg.type)
    {
        DMA_PHY_ChnRequest(channel);
    }

    //waiting for dma transfer finished
    wait_time = 0;
    while(DMA_TRANS_TIMEOUT > wait_time)
    {
        if(DMA_HAL_PollingTransDone(channel))
        {
            break;
        }
        SCI_Sleep(1);
        wait_time += 1;
    }
    if(DMA_TRANS_TIMEOUT <= wait_time)
    {
        return 0;
    }

    //disable dma channel
    DMA_HAL_ChnEnable_Ex(channel, SCI_FALSE, cacheConfig, 2);
    DMA_HAL_FreeChn(channel);
	
#else
    SCI_MEMCPY (des, src, count);
#endif

    return (uint32) des;
}

/*****************************************************************************/
//  Description:    dma memset
//  Global resource dependence:
//  Author:         hao.liu
//  input param     des        : New buffer
//                  value      : poniter of value to set
//                  count      : Number of characters to copy in bytes
//  return value               : param error means timeout
//                             : 0xffffffff means available channel
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_Memset (void *des, void *value, uint32 count, DATAWIDTH_E flag)
{
#ifdef PLATFORM_SC6530
        uint32                 channel;
        DMA_CHNCFG_T           channelcfg={0};
        uint32                 wait_time;

        SCI_ASSERT ((des != SCI_NULL)||(value != SCI_NULL));/*assert verified*/

        channelcfg.type = DMA_CHN_TYPE_SOFT;
        channelcfg.workmode = DMA_CHN_WORKMODE_NORMAL;
        channelcfg.requestmode= DMA_CHN_REQMODE_TRASACTION;
        channelcfg.src_address = (uint32)value;
        channelcfg.dest_address = (uint32)des;
        channelcfg.total_length  = count;
        channelcfg.block_length  = 256;
        channelcfg.frag_length  = 32;
        channelcfg.priority = 0;
        channelcfg.src_step =0;

        switch(flag)
        {
            case DATAWIDTH_BYTE:
                channelcfg.src_datawidth = DMA_DATAWIDTH_BYTE;
                channelcfg.dest_datawidth = DMA_DATAWIDTH_BYTE;
                channelcfg.dest_step=1;
                break;
            case DATAWIDTH_HALFWORD:
                channelcfg.src_datawidth = DMA_DATAWIDTH_HALFWORD;
                channelcfg.dest_datawidth = DMA_DATAWIDTH_HALFWORD;
                channelcfg.dest_step=2;
                break;
            case DATAWIDTH_WORD:
                channelcfg.src_datawidth = DMA_DATAWIDTH_WORD;
                channelcfg.dest_datawidth = DMA_DATAWIDTH_WORD;
                channelcfg.dest_step=4;
                break;
            default:
                SCI_ASSERT(0);/*assert to do*/
                break;
        }

        channel = DMA_HAL_AllocChn (DMA_CHN_TRSF_TYPE_FULL);
        if (DMA_CHANNELID_INVALID == channel)
            return DMA_CHANNELID_INVALID;

        DMA_HAL_ChnConfig(channel, &channelcfg);

        //disable all interrupt
        DMA_HAL_ChnIntConfig(channel, DMA_CHN_INT_TYPE_NONE, NULL);

        //enable dma channel
        DMA_HAL_ChnEnable(channel, SCI_TRUE);

        if(DMA_CHN_TYPE_SOFT == channelcfg.type)
        {
            DMA_PHY_ChnRequest(channel);
        }

        //waiting for dma transfer finished
        wait_time = 0;
        while(DMA_TRANS_TIMEOUT > wait_time)
        {
            if(DMA_HAL_PollingTransDone(channel))
            {
                break;
            }
            SCI_Sleep(1);
            wait_time += 1;
        }
        if(DMA_TRANS_TIMEOUT <= wait_time)
        {
            return 0;
        }

        //disable dma channel
        DMA_HAL_ChnEnable(channel, SCI_FALSE);

        DMA_HAL_FreeChn(channel);
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        uint32                 channel;
        DMA_CHNCFG_T           channelcfg={0};
        uint32                 wait_time;

        SCI_ASSERT ((des != SCI_NULL)||(value != SCI_NULL));/*assert verified*/

        channelcfg.type = DMA_CHN_TYPE_SOFT;
        channelcfg.workmode = DMA_CHN_WORKMODE_NORMAL;
        channelcfg.requestmode= DMA_CHN_REQMODE_TRASACTION;
        channelcfg.src_address = (uint32)value;
        channelcfg.dest_address = (uint32)des;
        channelcfg.total_length  = count;
        channelcfg.block_length  = 256;
        channelcfg.frag_length  = 32;
        channelcfg.priority = 0;
        channelcfg.src_step =0;

        switch(flag)
        {
            case DATAWIDTH_BYTE:
                channelcfg.src_datawidth = DMA_DATAWIDTH_BYTE;
                channelcfg.dest_datawidth = DMA_DATAWIDTH_BYTE;
                channelcfg.dest_step=1;
                break;
            case DATAWIDTH_HALFWORD:
                channelcfg.src_datawidth = DMA_DATAWIDTH_HALFWORD;
                channelcfg.dest_datawidth = DMA_DATAWIDTH_HALFWORD;
                channelcfg.dest_step=2;
                break;
            case DATAWIDTH_WORD:
                channelcfg.src_datawidth = DMA_DATAWIDTH_WORD;
                channelcfg.dest_datawidth = DMA_DATAWIDTH_WORD;
                channelcfg.dest_step=4;
                break;
            default:
                SCI_ASSERT(0);/*assert to do*/
                break;
        }

        channel = DMA_HAL_AllocChn (DMA_CHN_TRSF_TYPE_FULL);
        if (DMA_CHANNELID_INVALID == channel)
            return DMA_CHANNELID_INVALID;

        DMA_HAL_ChnConfig(channel, &channelcfg);

        //disable all interrupt
        DMA_HAL_ChnIntConfig(channel, DMA_CHN_INT_TYPE_NONE, NULL);

        //enable dma channel
        DMA_HAL_ChnEnable(channel, SCI_TRUE);

        if(DMA_CHN_TYPE_SOFT == channelcfg.type)
        {
            DMA_PHY_ChnRequest(channel);
        }

        //waiting for dma transfer finished
        wait_time = 0;
        while(DMA_TRANS_TIMEOUT > wait_time)
        {
            if(DMA_HAL_PollingTransDone(channel))
            {
                break;
            }
            SCI_Sleep(1);
            wait_time += 1;
        }
        if(DMA_TRANS_TIMEOUT <= wait_time)
        {
            return 0;
        }

        //disable dma channel
        DMA_HAL_ChnEnable(channel, SCI_FALSE);

        DMA_HAL_FreeChn(channel);
#else
    SCI_MEMSET (des, *((uint8*)value), count);
#endif

    return (uint32) des;
}

/*****************************************************************************/
//  Description:    alloc soft dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           if success return channel id, otherwise return DMA_CHANNELID_INVALID
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_AllocChn (DMA_CHN_TRSF_TYPE_E type)
{
    return DMA_PHY_AllocChn(type);
}

/*****************************************************************************/
//  Description:    release channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_FreeChn (uint32 channel)
{
    DMA_PHY_FreeChn(channel);
}

/*****************************************************************************/
//  Description:    configure a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:channel to be configured
//                  chnparam:channel configure struct
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnConfig (uint32 channel, DMA_CHNCFG_PTR chnparam)
{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX && NULL != chnparam);/*assert verified*/

    //config dma chn priority
    DMA_PHY_SetChnPriority (channel,chnparam->priority);

#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
{
    uint32 uid;
    //config dma chn uid
    uid = ( (DMA_CHN_TYPE_SOFT == DMA_CFG_GetChnType (channel)) ?
            (DMA_UID_SOFT) : (channel));
    DMA_PHY_SetChnUID (channel,uid);
}    
#endif

    //wrap addr set
    if (chnparam->is_srcwrapen || chnparam->is_destwrapen)
    {
        DMA_PHY_SetWrapAddr (channel, chnparam->wrapstart_addr,chnparam->wrapend_addr);
    }

    //config dma chn related control register
    DMA_PHY_ChnConfig (channel, chnparam);
}

/*****************************************************************************/
//  Description:    set dma priority
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//                  priority:channel priority,0-3
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_SetChnPriority (uint32 channel, DMA_CHN_PRIORITY_E priority)
{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX && priority < DMA_CHN_PRIORITY_MAX);/*assert verified*/

    DMA_PHY_SetChnPriority (channel,priority);
}

/*****************************************************************************/
//  Description:    dma request enable/disable extend function, will do memory sync.
//  Global resource dependence:
//  Author:         steve.zhan
//  Note:           channel :  channel No
//                  is_enable: enable/disable
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnEnable_Ex (uint32 channel, BOOLEAN is_enable,
                                  DMA_CAHCECONFIG_T cfgArray[], uint32 cfgCnt)

{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX); /*assert verified*/

    if (is_enable && cfgCnt > 0  &&  NULL != cfgArray)
    {
        uint32 cnt = 0;

        while (cnt < cfgCnt)
        {
            SCI_ASSERT (0 < cfgArray[cnt].bufferSize);/*assert verified*/
            MMU_DmaCacheSync (cfgArray[cnt].bufferAddr,cfgArray[cnt].bufferSize, cfgArray[cnt].dir);
            cnt ++;
        }
    }

    DMA_PHY_ChnEnable_Ex (channel,is_enable);
}
/*****************************************************************************/
//  Description:    dma request enable/disable
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel :  channel No
//                  is_enable: enable/disable
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnEnable (uint32 channel, BOOLEAN is_enable)
{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX);/*assert verified*/

    DMA_PHY_ChnEnable (channel,is_enable);

}
/*****************************************************************************/
//  Description:    get dma channel transdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingTransDone (uint32 channel)
{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX);/*assert verified*/
    return DMA_PHY_PollingTransDone (channel);
}
/*****************************************************************************/
//  Description:    get dma channel blockdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingBlockDone (uint32 channel)
{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX);/*assert verified*/
    return DMA_PHY_PollingBlockDone (channel);
}

/*****************************************************************************/
//  Description:    get dma channel listdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingListDone (uint32 channel)
{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX);/*assert verified*/
    return DMA_PHY_PollingListDone (channel);
}

/*****************************************************************************/
//  Description:    configure dma channel interrupt
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//                  inttype:interrupt type,including all transfer done,burst done and link done.
//                  is_enable:SCI_TRUE,enable the interrupt, SCI_FALSE,disable the interrupt
//                  callback:interrupt service,if is_enable=SCI_FALSE, it is unuseful.
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnIntConfig (uint32 channel, uint32 inttype, DMA_HAL_ISR_HANDLER callback)
{
    SCI_ASSERT (channel <= DMA_CHANNELID_MAX &&           /*assert verified*/
                inttype <= (DMA_CHN_INT_TYPE_BLOCKDONE |
                            DMA_CHN_INT_TYPE_TRANSDONE |
                            DMA_CHN_INT_TYPE_LISTDONE));

    //config hw interrupt
    DMA_PHY_ChnIntConfig (channel, inttype, callback);

}

//*****************************************************************************/
//  Description:    setting dma channnel user id
//  Global resource dependence:
//  Author:         
//  Note:           channel: chn index
//                  uid:channel user id
/*****************************************************************************/
PUBLIC void DMA_HAL_SetChnUID (uint32 channel, DMA_CHN_REQ_LOGIC_ID_E chn_req_id)
{
    DMA_PHY_SetChnUID(channel,chn_req_id);
}

//*****************************************************************************/
//  Description:    return std chn block transmited length
//  Note:           channel: chn index
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_StdChn_Transfered_Data (uint32 channel)
{
    return DMA_PHY_StdChn_Transfered_Data(channel);
}

#ifdef   __cplusplus
}
#endif
