/******************************************************************************
 ** File Name:    dma_phy_r3p0.c                                             *
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
 ** 04/23/2012    Yong.Li         Modify for DMA r3p0 architecture            *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "mmu_drvapi.h"
#include "dma_drv_internal.h"
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
#define sign(x) ((x) >= 0 ? 0 : 1)
//#define abs(x)  ((x) >= 0 ? x : (-x)) 

#define DMA_PAUSE_NEST_MAX      10 // need check
#define DMA_PAUSE_TIME_OUT      0x2000
#define GET_DATA_STEP(_num_)    (((_num_) >= 0)?(_num_):(0x8000|abs(_num_)))
#define GET_BLOCK_STEP(_num_)   (((_num_) >= 0)?(_num_):(0x2000000|abs(_num_)))

#define GET_BLOCKDONE_RAWSTS() (dma_chn_ctl_ptr->status & ARM_AXIDMA_COUNTP_FINISH_FINISH)
#define GET_TRANSDONE_RAWSTS() (dma_chn_ctl_ptr->status & ARM_AXIDMA_COUNT_FINISH_FINISH)
#define GET_LISTDONE_RAWSTS()  (dma_chn_ctl_ptr->status & ARM_AXIDMA_SG_FINISH_FINISH)
#define GET_FRAGDONE_RAWSTS()  (1)

#define BLOCKINT_CLR()                     \
    DRV_MACRO_START                             \
    SCI_DisableIRQ();                           \
    dma_chn_ctl_ptr->status = ARM_AXIDMA_COUNTP_FINISH_FINISH;  \
    SCI_RestoreIRQ();                           \
    DRV_MACRO_END

#define TRANSINT_CLR()                     \
    DRV_MACRO_START                             \
    SCI_DisableIRQ();                           \
    dma_chn_ctl_ptr->status = ARM_AXIDMA_COUNT_FINISH_FINISH;  \
    SCI_RestoreIRQ();                           \
    DRV_MACRO_END

#define LISTINT_CLR()                      \
    DRV_MACRO_START                             \
    SCI_DisableIRQ();                           \
    dma_chn_ctl_ptr->status = ARM_AXIDMA_SG_FINISH_FINISH;  \
    SCI_RestoreIRQ();                           \
    DRV_MACRO_END

#define FRAGINT_CLR()                       \

#define ALLINT_CLR()                       \
    DRV_MACRO_START                             \
    BLOCKINT_CLR();                        \
    TRANSINT_CLR();                        \
    FRAGINT_CLR();                          \
    LISTINT_CLR();                         \
    DRV_MACRO_END
    
//int status type: 3bits value
//BIT_0: BLOCK DONE
//BIT_1: TRANS DONE
//BIT_2: LIST DONE
#define GET_INTSTS_TYPE()                                            (      \
    (dma_chn_ctl_ptr->status & ARM_AXIDMA_COUNTP_FINISH_FINISH)     |      \
    (dma_chn_ctl_ptr->status & ARM_AXIDMA_COUNT_FINISH_FINISH)|      \
    (dma_chn_ctl_ptr->status & ARM_AXIDMA_SG_FINISH_FINISH)   )
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
//LOCAL DMA_HAL_ISR_HANDLER  s_dma_chx_handler[DMA_CH_NUM];
LOCAL volatile HWP_ARM_AXIDMA_T *s_dma_gen_ctl_ptr = NULL;
typedef struct
{
    uint32 ch;
    uint32 int_type;
} DMA_INT_DATA_T;

//dma channel staus
typedef enum _DMA_CHN_STATUS
{
    DMA_CHN_STATUS_IDLE = 0 ,//idle ,can be alloced
    DMA_CHN_STATUS_BUSY,    //busy, can not be alloced
    DMA_CHN_STATUS_RESERVED//reserved for hardware, can not be alloced
} DMA_CHN_STATUS_E;

struct DMA_CHANNEL_T
{
	DMA_CHN_TRSF_TYPE_E type;
	uint32 base_addr;
	DMA_CHN_STATUS_E status;
	DMA_CHNCFG_PTR ptr;
    DMA_HAL_ISR_HANDLER  s_dma_chx_handler;
	uint32 recv_length;
};

typedef struct _DMA_STD_CHN_CFG_REG
{
    volatile uint32 conf;
    volatile uint32 map;
    volatile uint32 saddr;    
    volatile uint32 daddr;
    volatile uint32 count;
    volatile uint32 countp;
    volatile uint32 status;
    volatile uint32 sgaddr;
    volatile uint32 sgconf;
}DMA_STD_CHN_CFG_REG;

#define DMA_FULL_CHN_CFG_REG      DMA_STD_CHN_CFG_REG

LOCAL DMA_INT_DATA_T dma_int_data;

INPUT_BUFFER_INIT (DMA_INT_DATA_T, DMA_CH_NUM)

/*----------------------------------------------------------------------------*
**                         Global Function Prototype                           *
**---------------------------------------------------------------------------*/

#define DMA_CHANNEL_MIN	        0

#define DMA_CHANNEL_MAX	        (DMA_CH_NUM)

struct DMA_CHANNEL_T g_channel[DMA_CHANNEL_MAX];
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL DEVICE_HANDLE s_dev_dma = SCI_NULL;
LOCAL ISR_EXE_T DMA_IrqHandler (uint32 num);
LOCAL void  DmaHisrFunc (uint32 cnt, void *pData);
//LOCAL void DmaControlChanInt(uint32 channel, uint32 intType, BOOLEAN enable);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
LOCAL void DMA_ISR_PROCESS (uint32 ch)
{
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = 0;		   
    ch += DMA_CHANNEL_MIN;
    dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[ch].base_addr;		   
    SCI_ASSERT (NULL != g_channel[ch].s_dma_chx_handler); /*assert verified*/
    dma_int_data.ch = ch;
    dma_int_data.int_type = GET_INTSTS_TYPE();
    IsrWriteBuffer (dma_int_data);

    ALLINT_CLR();
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

    status =  s_dma_gen_ctl_ptr->axidma_irq_stat;
    
    if (0 == status)
    {
        return ISR_DONE;
    }
    else
    {

        for (i = 0; i < DMA_CH_NUM; i++)
        {
            if  (status & (0x01 << i))
            {
                    DMA_ISR_PROCESS (i);
            }
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
        g_channel[data.ch].s_dma_chx_handler (data.int_type);
    }

    return ;
}

LOCAL uint32 _DMA_PHY_Is_Full_Chn(uint32 channel)
{
    return (g_channel[channel].type==DMA_CHN_TRSF_TYPE_FULL);
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

    s_dma_gen_ctl_ptr = (volatile HWP_ARM_AXIDMA_T *) REG_DMA_BASE;    
    
    SCI_DisableIRQ();    

#if 0   /*linker*/
    /* register init */	
    SCI_ASSERT (DEVICE_Find ("DMA", &s_dev_dma) == CLOCK_SUCCESS); /*assert verified*/    
    DEVICE_Open(s_dev_dma);
#endif

    for(i = DMA_CHANNEL_MIN; i < DMA_CHANNEL_MAX; i++)	
    {		
        if(i < DMA_CHANNEL_MAX - DMA_CH_FULLTYPE_NUM)		
        {			
            g_channel[i].type = DMA_CHN_TRSF_TYPE_STANDARD;		
        }		
        else		
        {			
            g_channel[i].type = DMA_CHN_TRSF_TYPE_FULL;		
        }				
        g_channel[i].status = DMA_CHN_STATUS_IDLE;

        g_channel[i].base_addr = REG_DMA_BASE + 0x40 + (i-DMA_CHANNEL_MIN)*0x40;	
        s_dma_gen_ctl_ptr->axidma_ch[i].map = 0xFFFF;    /*default to 0xFFFF,other maybe same value*/
    }
    
    /*register dma irq handle to host*/
    status = ISR_RegHandler_Ex (TB_DMA_INT, DMA_IrqHandler, DmaHisrFunc, CHIPDRV_HISR_PRIO_0, NULL);

    if (TB_SUCCESS == status)
    {
        /*enable dma int*/
        CHIPDRV_EnableIRQINT (TB_DMA_INT);
    }

    SCI_RestoreIRQ();
}

PUBLIC void DMA_PHY_ChnRest (uint32 channel)
{

    if(g_channel[channel].type == DMA_CHN_TRSF_TYPE_STANDARD)
    {
        DMA_STD_CHN_CFG_REG *dma_chn_ctl_ptr;
        dma_chn_ctl_ptr = (DMA_STD_CHN_CFG_REG *)g_channel[channel].base_addr;
        memset(dma_chn_ctl_ptr, 0, sizeof(DMA_STD_CHN_CFG_REG));
    }
    else
    {
        DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr;
        dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;
        memset(dma_chn_ctl_ptr, 0, sizeof(DMA_FULL_CHN_CFG_REG));
    }
}
//*****************************************************************************/
//  Description:    Init DMA controller, this function can only be called once
//                  when system initialization.
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
/*****************************************************************************/
PUBLIC uint32 DMA_PHY_AllocChn(DMA_CHN_TRSF_TYPE_E type)
{
	int i;
	
	for(i = DMA_CHANNEL_MIN; i < DMA_CHANNEL_MAX; i++)
	{
	   SCI_DisableIRQ();
		if(g_channel[i].type == type && g_channel[i].status == DMA_CHN_STATUS_IDLE)
		{
			g_channel[i].status = DMA_CHN_STATUS_BUSY;
			
			SCI_RestoreIRQ();			
			return i;
		}
		SCI_RestoreIRQ();			
	}
		    
	return DMA_CHANNELID_INVALID;
}

//*****************************************************************************/
//  Description:    Init DMA controller, this function can only be called once
//                  when system initialization.
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
/*****************************************************************************/
PUBLIC void DMA_PHY_FreeChn(uint32 channel)
{
	SCI_ASSERT((channel < DMA_CHANNEL_MAX) && (channel >= DMA_CHANNEL_MIN)); /*assert to do*/
	SCI_ASSERT(g_channel[channel].status != DMA_CHN_STATUS_IDLE); /*assert to do*/
	
	SCI_DisableIRQ();
	
    DMA_PHY_ChnRest(channel);
	g_channel[channel].status = DMA_CHN_STATUS_IDLE;
	g_channel[channel].s_dma_chx_handler = PNULL;
	SCI_RestoreIRQ();
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
        s_dma_gen_ctl_ptr->axidma_conf |=  ARM_AXIDMA_STOP_STOP;
        s_dma_pause_nest++;
    }
    else if (--s_dma_pause_nest <= 0)
    {
        s_dma_gen_ctl_ptr->axidma_conf &= ~ARM_AXIDMA_STOP_STOP;
    }

    SCI_RestoreIRQ();

    time_out = 0;

    if (is_pause)
    {
        while (! (s_dma_gen_ctl_ptr->axidma_status & ARM_AXIDMA_STOP_STATUS))
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
//  Description:    pause or resume dma transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           is_pause: SCI_TRUE=pause,SCI_FALSE=resume
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnPause (uint32 channel, BOOLEAN is_pause)
{
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		   

    SCI_DisableIRQ();

    if(is_pause)
    {
        dma_chn_ctl_ptr->conf &= ~ARM_AXIDMA_START_START;
    }
    else
    {
        dma_chn_ctl_ptr->conf |= ARM_AXIDMA_START_START;
    }
    
    SCI_RestoreIRQ();
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
#if 0    /*该接口应该对应dma_sg功能，暂时不实现*/
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    SCI_DisableIRQ();
    
    dma_chn_ctl_ptr->full_chn_wrap_ptr = wrapend_addr;
    dma_chn_ctl_ptr->full_chn_wrap_to = wrapstart_addr;

    SCI_RestoreIRQ();
#endif
}

//*****************************************************************************/
//  Description:    setting dma channnel user id
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  uid:channel user id
/*****************************************************************************/
PUBLIC void DMA_PHY_SetChnUID (uint32 channel, uint32 chn_req_id)
{
	uint32 req_id = DMA_CFG_GetMReqID(chn_req_id);
    uint32 reqx_uid_base = REG_DMA_BASE + 0x40 + (req_id-1)*0x04;
	CHIP_REG_SET(reqx_uid_base, (channel));
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
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    SCI_DisableIRQ();

    switch (workmode)
    {
        case DMA_CHN_WORKMODE_NORMAL:
            dma_chn_ctl_ptr->sgconf &= ~ARM_AXIDMA_SG_EN_ENABLE;
            break;
        case DMA_CHN_WORKMODE_LINKLIST:
            dma_chn_ctl_ptr->sgconf |= ARM_AXIDMA_SG_EN_ENABLE;
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
PUBLIC void DMA_PHY_SetChnPriority (uint32 channel, DMA_CHN_PRIORITY_E priority)
{
#if 0    /*8910 dma没有每个通道的优先级配置，只有总的优先级原则，该接口暂时不实现*/
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    SCI_DisableIRQ();
   
    dma_chn_ctl_ptr->full_chn_cfg &= ~CHN_PRIORITY_MSK;
    switch(priority)
    {
        case DMA_CHN_PRIORITY_0:
        	dma_chn_ctl_ptr->full_chn_cfg |= DMA_CHx_CFG_PRIORITY0;
            break;
        case DMA_CHN_PRIORITY_1:
         	dma_chn_ctl_ptr->full_chn_cfg |= DMA_CHx_CFG_PRIORITY1;
            break;
        case DMA_CHN_PRIORITY_2:
         	dma_chn_ctl_ptr->full_chn_cfg |= DMA_CHx_CFG_PRIORITY2;
            break;
        case DMA_CHN_PRIORITY_3:
         	dma_chn_ctl_ptr->full_chn_cfg |= DMA_CHx_CFG_PRIORITY3;
            break;
        default: 
            break;
    }    
    
    SCI_RestoreIRQ();
#endif
}

LOCAL uint32 s_dma_sleep_flag = 0;
//*****************************************************************************/
//  Description:    Enable/Disable a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  is_enable: enable/disable a chn
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnEnable (uint32 channel, BOOLEAN is_enable)
{
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    SCI_DisableIRQ();

    if(is_enable) 
    {
    	dma_chn_ctl_ptr->conf |= ARM_AXIDMA_START_START;
    }
    else
    {
        dma_chn_ctl_ptr->conf &= ~ARM_AXIDMA_START_START;
        /*fixme, wait timeout*/
        while (dma_chn_ctl_ptr->status & ARM_AXIDMA_RUN_BUSY);
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

    SCI_RestoreIRQ();    
}

//*****************************************************************************/
//  Description:    Enable/Disable a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  is_enable: enable/disable a chn
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnRequest(uint32 channel)
{
#if 0
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;			

    SCI_DisableIRQ();

    dma_chn_ctl_ptr->full_chn_req |= CHN_REQ;

    SCI_RestoreIRQ();   
#endif
}


//*****************************************************************************/
//  Description:    Enable/Disable a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel: chn index
//                  is_enable: enable/disable a chn
/*****************************************************************************/
PUBLIC void DMA_PHY_ChnEnable_Ex (uint32 channel, BOOLEAN is_enable)
{
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		
    
    SCI_DisableIRQ();
    SCI_DisableFIQ();

    ALLINT_CLR(); 
    
    if (is_enable)
    {
        dma_chn_ctl_ptr->conf |= ARM_AXIDMA_START_START;
        s_dma_sleep_flag |= (1 << channel);
        SCI_DMA_EnableDeepSleep (DISABLE_DEEP_SLEEP);        
    }
    else
    {
        dma_chn_ctl_ptr->conf &= ~ARM_AXIDMA_START_START;
        s_dma_sleep_flag &= ~ (1UL << channel);

        if (!s_dma_sleep_flag)
        {
            SCI_DMA_EnableDeepSleep (ENABLE_DEEP_SLEEP);
        }        
    }
    
    SCI_RestoreFIQ();
    SCI_RestoreIRQ();
}

PUBLIC uint32 DMA_PHY_DMASleepFlag(void)
{
	return s_dma_sleep_flag;
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
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;

    SCI_DisableIRQ();

    ALLINT_CLR();
    
    if (SCI_NULL == callback)
    {
        g_channel[channel].s_dma_chx_handler = SCI_NULL;
    }

    dma_chn_ctl_ptr->conf &= ~(ARM_AXIDMA_IRQ_T | ARM_AXIDMA_IRQ_F);
   
    if (inttype & DMA_CHN_INT_TYPE_BLOCKDONE)
    {
        dma_chn_ctl_ptr->conf |= ARM_AXIDMA_IRQ_T;
    }

    if (inttype & DMA_CHN_INT_TYPE_TRANSDONE)
    {
        dma_chn_ctl_ptr->conf |= ARM_AXIDMA_IRQ_F;
    }

    if (inttype & DMA_CHN_INT_TYPE_LISTDONE)
    {
        dma_chn_ctl_ptr->sgconf |= ARM_AXIDMA_SG_FINISH_IE_ENABLE;
    }

    if (inttype & DMA_CHN_INT_TYPE_FRAGDONE)
    {
    }

    g_channel[channel].s_dma_chx_handler = callback;

    SCI_RestoreIRQ();	
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
    uint32 frag_cfg  = 0;
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;

    g_channel[channel].recv_length = chnparam->total_length;

//setting workmode and related registers
    switch (chnparam->workmode)
    {
        case DMA_CHN_WORKMODE_NORMAL:
        case DMA_CHN_WORKMODE_SOFTLIST:
            break;

        case DMA_CHN_WORKMODE_LINKLIST:
            SCI_DisableIRQ();
            dma_chn_ctl_ptr->sgconf |= ARM_AXIDMA_SG_EN_ENABLE; //Linklist_en
            dma_chn_ctl_ptr->sgconf |= ARM_AXIDMA_SG_FINISH_IE_ENABLE; //Linklist_int_en
            //dma_chn_ctl_ptr->full_chn_llist_ptr= chnparam->ll_next;
            SCI_RestoreIRQ();
            return;

        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    SCI_ASSERT((channel < DMA_CHANNEL_MAX) && (channel >= DMA_CHANNEL_MIN)); /*assert to do*/
 
   //endian_type and  endian_switchmode
    switch (chnparam->endian_switchmode)
    {
        case DMA_ENDIANSWITCHMODE_UN:
            //frag_cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_UN;
            break;
        case DMA_ENDIANSWITCHMODE_FULL:
            //frag_cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_FULL;
            break;
        case DMA_ENDIANSWITCHMODE_MODE0:
            //frag_cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_MODE0;
            break;
        case DMA_ENDIANSWITCHMODE_MODE1:
            //frag_cfg |= (uint32) DMA_CHx_ENDIANSWMODESEL_MODE1;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    //src datawidth and dest datawidth
    if (chnparam->src_datawidth != chnparam->dest_datawidth)
    {
        SCI_ASSERT (0);    /*8910 dma不支持src/dest width不一致*/
    }
    else
    {
        switch (chnparam->src_datawidth)
        {
            case DMA_DATAWIDTH_BYTE:
                frag_cfg |= (uint32) ARM_AXIDMA_DATA_TYPE_BYTE;
                break;
            case DMA_DATAWIDTH_HALFWORD:
                frag_cfg |= (uint32) ARM_AXIDMA_DATA_TYPE_HALF_WORD;
                break;
            case DMA_DATAWIDTH_WORD:
                frag_cfg |= (uint32) ARM_AXIDMA_DATA_TYPE_WORD;
                break;
            default:
                SCI_ASSERT (0);/*assert verified*/
                break;
        }
    }
    
    if(chnparam->src_step == 0)
    {
        frag_cfg |= ARM_AXIDMA_SADDR_FIX;
    }
    if(chnparam->dest_step == 0)
    {
        frag_cfg |= ARM_AXIDMA_DADDR_FIX;
    }
    	
    //request mode
    switch (chnparam->requestmode)
    {
        case DMA_CHN_REQMODE_FRAGMENT:
            break;    
        case DMA_CHN_REQMODE_NORMAL:
            break;
        case DMA_CHN_REQMODE_TRASACTION:
            frag_cfg |= (uint32) ARM_AXIDMA_SYN_IRQ;
            break;
        case DMA_CHN_REQMODE_LIST:
            //frag_cfg |= (uint32) DMA_CHx_REQMODE_LIST;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    dma_chn_ctl_ptr = (DMA_STD_CHN_CFG_REG *)g_channel[channel].base_addr;

    dma_chn_ctl_ptr->saddr = chnparam->src_address;
    dma_chn_ctl_ptr->daddr = chnparam->dest_address;
    dma_chn_ctl_ptr->conf |= frag_cfg;

    //update countp/count register
    dma_chn_ctl_ptr->countp = chnparam->block_length;
    dma_chn_ctl_ptr->count = chnparam->total_length;
    
#if 0
    else
	{
	    uint32 mode=0;
        DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr;
        dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;	
		
		SCI_ASSERT(chnparam->block_length); /*assert to do*/
		SCI_ASSERT(chnparam->total_length); /*assert to do*/

		//change the mode for FULL dma 
		switch(chnparam->requestmode)
		{
            case DMA_CHN_REQMODE_FRAGMENT  : mode= 0; break;
            case DMA_CHN_REQMODE_NORMAL    : mode= 1; break;			
            case DMA_CHN_REQMODE_TRASACTION: mode= 2; break;
            case DMA_CHN_REQMODE_LIST      : mode= 3; break;				
            default                        :  break;
		}

		dma_chn_ctl_ptr->saddr = chnparam->src_address;
		dma_chn_ctl_ptr->daddr = chnparam->dest_address;
        dma_chn_ctl_ptr->countp = chnparam->block_length;
        dma_chn_ctl_ptr->count = chnparam->total_length;
        
		dma_chn_ctl_ptr->conf  = (chnparam->src_datawidth  << 26) |
	                             (chnparam->dest_datawidth << 24) |
	                            (mode                     << 20) |
	                             (chnparam->frag_length    << 0)  ;
	    
		return;
	}	
#endif
}

PUBLIC void DMA_PHY_ChnMapSet(uint32 channel,uint32 map_value)
{
	DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;	
	dma_chn_ctl_ptr->map = map_value;
    dma_chn_ctl_ptr->conf |= (ARM_AXIDMA_SYN_IRQ | ARM_AXIDMA_SEQ_SEL);
}

PUBLIC void DMA_PHY_ChnForceSet(uint32 channel)
{
	DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;	
	dma_chn_ctl_ptr->conf |= ARM_AXIDMA_FORCE_TRANS;
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
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    if (GET_TRANSDONE_RAWSTS ())
    {
        TRANSINT_CLR ();
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
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    if (GET_BLOCKDONE_RAWSTS ())
    {
        BLOCKINT_CLR ();
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
PUBLIC BOOLEAN DMA_PHY_PollingFragDone (uint32 channel)
{
    BOOLEAN ret;
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    if (GET_FRAGDONE_RAWSTS ())
    {
        LISTINT_CLR ();
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
    DMA_FULL_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_FULL_CHN_CFG_REG *)g_channel[channel].base_addr;		

    if (GET_LISTDONE_RAWSTS ())
    {
        LISTINT_CLR ();
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

//*****************************************************************************/
//  Description:    return std chn block transmited length
//  Note:           channel: chn index
/*****************************************************************************/
PUBLIC uint32 DMA_PHY_StdChn_Transfered_Data (uint32 channel)
{
    uint32 length = 0;
    uint32 ori_length;

    DMA_STD_CHN_CFG_REG *dma_chn_ctl_ptr = (DMA_STD_CHN_CFG_REG *)g_channel[channel].base_addr;

    if (g_channel[channel].type == DMA_CHN_TRSF_TYPE_STANDARD) 
    {
        ori_length = g_channel[channel].recv_length;
        /*r3p0's dma standard chn block length is 17bit*/		
        length = dma_chn_ctl_ptr->count & 0x1ffff;
        g_channel[channel].recv_length = length;
        length = ori_length - length;	
    }
    else
    {
        SCI_ASSERT (0);/*assert verified*/
    }

    return length;
}

#ifdef   __cplusplus
}
#endif
