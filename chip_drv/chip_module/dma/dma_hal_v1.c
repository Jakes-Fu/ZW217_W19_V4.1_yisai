/******************************************************************************
 ** File Name:    dma_hal_v1.c                                                *
 ** Author:       weihua.wang                                                 *
 ** DATE:         2005-9-17                                                   *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2005-9-17     weihua.wang     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "chip_plf_export.h"
#include "dma_hal.h"
#include "dma_drvapi.h"
#include "deepsleep_drvapi.h"
#include "chip.h"
#include "chng_freq.h"
#include "misc.h"
//#include "mmu_drvapi.h"
#include "task_monitor.h "

#ifdef VM_SUPPORT
#include "mmu_drvapi.h"
#endif //VM_SUPPORT
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
#define DMA_STS_IDLE      (0x00)
#define DMA_STS_BUSY      (0x01)


#define DMA_SOFT_BURSTSIZE          (0x100)

#ifdef PLATFORM_SC6600L
#define DMA_CLR_LISTDONE_INT(chn)
#define MMU_INVALIDE_DCACHE
#elif defined (PLATFORM_SC8800H)
#define MMU_INVALIDE_DCACHE    do {MMU_InvalideDCACHE();}while(0);
#define DMA_CLR_LISTDONE_INT(chn) { REG32(DMA_LISTDONE_INT_CLR) |= (1<<chn);}
#endif
#define DMA_PAUSE_TIME_OUT 0x200

#ifdef PLATFORM_SC6600L
#define DMA_PAUSE_BIT BIT_2
#elif defined (PLATFORM_SC8800H)
#define DMA_PAUSE_BIT BIT_4
#endif

//set soft dma request
#ifdef PLATFORM_SC6600L
#define DMA_CHN_SOFT_REQ(chn,n) {\
        if(n){\
            REG32(DMA_SOFT_REQ) |= 1<<chn;\
        }\
        else { \
            REG32(DMA_SOFT_REQ) &= ~(1<<chn);\
        }\
    }
//set dma chnen register
#define DMA_CHNEN(chn,n)  {\
        if(n) {\
            REG32(DMA_CHx_EN) |=  1<<chn;\
        }\
        else  {\
            REG32(DMA_CHx_FC) |=  1<<chn;\
        }\
    }


#define DMA_HWEN(chn,n)   {\
        if(n) {\
            REG32(DMA_HW_EN) |=  1<<chn;\
        }\
        else {\
            \
        }\
    }

#define DMA_HW_STS(chn)     ((REG32DMA_CHx_STS)&(1<<chn))

#elif defined(PLATFORM_SC8800H)
#define DMA_CHN_SOFT_REQ(chn,n) {\
        if(n){\
            REG32(DMA_SOFT_REQ) |= 1<<chn;\
        }\
        else { \
            REG32(DMA_SOFT_REQ) &= ~(1<<chn);\
        }\
    }
//set dma chnen register
#define DMA_CHNEN(chn,n)  {\
        DMA_Setpause(TRUE);\
        if(n) {\
            REG32(DMA_CHx_EN) |=  1<<chn;\
        }\
        else  {\
            REG32(DMA_CHx_EN) &=  ~(1<<chn);\
        }\
        DMA_Setpause(FALSE);\
    }
#define DMA_HWEN(chn,n)
#define DMA_HW_STS(chn)

#endif


#define DMA_CLR_INT(chn) {\
        REG32(DMA_BURST_INT_CLR) |= 1<<chn;\
        REG32(DMA_TRANSF_INT_CLR) |= 1<<chn;\
        DMA_CLR_LISTDONE_INT(chn);\
    }


/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*link record struct*/
typedef struct _DMA_LINK_T_
{
    DMA_CHx_CTL_T dma_node;
    struct _DMA_LINK_T_ *free_next, *free_prev;
} DMA_LINK_T;

typedef struct _DMA_INDEX_TBL
{
    uint32 index_max;            /* max software dma channel*/
    uint32 channel[DMA_CH_NUM];  /* to restore the channel  */
} DMA_INDEX_TBL_S;

typedef struct _DMA_MNG
{
    uint32 channel;              /* channel number               */
    uint32 type;                 /* hardware/ software           */
    uint32 status;               /* channel status , idle/using/ */
    uint32 reserved;
} DMA_MNG_S;

/*--------------------------- Local Data ------------------------------------*/
LOCAL DMA_MNG_S        s_dma_chn_ctl[DMA_CH_NUM];
LOCAL DMA_INDEX_TBL_S  s_dma_index;
//flag represents DMA usage status
LOCAL VOLATILE uint32 s_dma_chn_uesd_flag = 0;

LOCAL DMA_LINK_T         *s_dma_linkfreelist;
LOCAL __align (4) uint8  s_dma_linkbuf[ (DMA_MAX_LINK_NUM+1) * DMA_LINK_NODE_SIZE + 4];
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
const DMA_CFG_TBL_S dma_cfg_data[DMA_CH_NUM] =
{
#if defined (PLATFORM_SC6600L)
    {0 , DMA_TYPE_HARD  },
    {1 , DMA_TYPE_HARD  },
    {2 , DMA_TYPE_HARD  },
    {3 , DMA_TYPE_HARD  },
    {4 , DMA_TYPE_HARD  },
    {5 , DMA_TYPE_HARD  },
    {6 , DMA_TYPE_HARD  },
    {7 , DMA_TYPE_HARD  },
    {8 , DMA_TYPE_HARD  },
    {9 , DMA_TYPE_HARD  },
    {10, DMA_TYPE_HARD  },
    {11, DMA_TYPE_HARD  },
    {12, DMA_TYPE_HARD  },
    {13, DMA_TYPE_HARD  },
    {14, DMA_TYPE_HARD  },
    {15, DMA_TYPE_HARD  },
    {16, DMA_TYPE_SOFT  },
    {17, DMA_TYPE_SOFT  },
    {18, DMA_TYPE_HARD  },
    {19, DMA_TYPE_HARD  },
    {20, DMA_TYPE_HARD  },
    {21, DMA_TYPE_SOFT  },
#elif defined (PLATFORM_SC8800H)
    {0 , DMA_TYPE_SOFT  },
    {1 , DMA_TYPE_HARD  },
    {2 , DMA_TYPE_HARD  },
    {3 , DMA_TYPE_HARD  },
    {4 , DMA_TYPE_HARD  },
    {5 , DMA_TYPE_HARD  },
    {6 , DMA_TYPE_HARD  },
    {7 , DMA_TYPE_HARD  },
    {8 , DMA_TYPE_HARD  },
    {9 , DMA_TYPE_HARD  },
    {10, DMA_TYPE_HARD  },
    {11, DMA_TYPE_HARD  },
    {12, DMA_TYPE_HARD  },
    {13, DMA_TYPE_HARD  },
    {14, DMA_TYPE_HARD  },
    {15, DMA_TYPE_HARD  },
    {16, DMA_TYPE_HARD  },
    {17, DMA_TYPE_HARD  },
    {18, DMA_TYPE_HARD  },
    {19, DMA_TYPE_HARD  },
    {20, DMA_TYPE_HARD  },
    {21, DMA_TYPE_HARD  },
    {22, DMA_TYPE_HARD  },
    {23, DMA_TYPE_HARD  },
    {24, DMA_TYPE_HARD  },
    {25, DMA_TYPE_HARD  },
    {26, DMA_TYPE_HARD  },
    {27, DMA_TYPE_HARD  },
    {28, DMA_TYPE_HARD  },
    {29, DMA_TYPE_SOFT  },
    {30, DMA_TYPE_SOFT  },
    //   {31, DMA_TYPE_SOFT  },

#endif
};


/*****************************************************************************/
//  Description:    dma link list init handle
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void DMA_LinkInit (void);

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void DMA_init (void)
{
    uint32 i;
    uint32 soft_max;

    //trace output
    //SCI_TRACE_LOW:"DMA_init\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DMA_HAL_V1_237_112_2_17_23_4_7_945,(uint8*)"");

    //local var init
    soft_max = 0;

    for (i = 0 ; i < DMA_CH_NUM ; i++)
    {
        s_dma_chn_ctl[i].channel  = i;
        s_dma_chn_ctl[i].type     = dma_cfg_data[i].type;
        s_dma_chn_ctl[i].status   = DMA_STS_IDLE;
        s_dma_chn_ctl[i].reserved = 0;

        if (dma_cfg_data[i].type == DMA_TYPE_SOFT)
        {
            s_dma_index.channel[soft_max] = i;
            soft_max++;
        }
    }

    s_dma_index.index_max = soft_max;

    //linklist node init
    DMA_LinkInit();

    //dma hal init
    DMA_HALInit();
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC  uint32  DMA_RegCallBack (uint32 channel, DMA_ISR_HANDLER callback)
{
    return (DMA_HALRegCallBack (channel,callback));
}

/*****************************************************************************/
//  Description:    dma hw channel config
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           channel : dma channel no
//                  dma_type: hard dma/soft dma
//                  int_type: burst/alltransfer/none
//                  chn_param: pointer to the dma channel param
/*****************************************************************************/
PUBLIC uint32 DMA_ChannelCfg (uint32 channel, uint32 dma_type,uint32 int_type, DMA_CTL_CFG_PTR chn_param)
{
    if ( (channel >= DMA_CH_NUM) || (dma_type >= DMA_TYPE_MAX) || (int_type >= DMA_INT_SEL_MAX) || (chn_param == NULL))
    {
        return ERR_DMA_PARAM_ERR;
    }

    DMA_HALChannelCfg (channel,dma_type,int_type,chn_param);

    return ERR_DMA_NONE;

}

/*****************************************************************************/
//  Description:    dma pause&resume
//  Global resource dependence:
//  Author:         Younger.yang
//  Note:           Pause & resume dma
/*****************************************************************************/
void DMA_Setpause (BOOLEAN is_pause)
{
    uint32 time_out = 0;


    if (is_pause)
    {
        if (!s_dma_chn_uesd_flag)
        {
            return;
        }

#if defined(PLATFORM_SC8800H)
        * (volatile uint32 *) (AHB_CTL0) |= BIT_6;
        * (volatile uint32 *) (DMA_CFG) |= BIT_8;

        while (! ( (* (volatile uint32 *) (DMA_TRANS_STS)) & BIT_30))

#elif  defined(PLATFORM_SC6600L)
            * (volatile uint32 *) (AHB_CTL0) = (* (volatile uint32 *) (AHB_CTL0) & (~BIT_4)) |BIT_3;

        * (volatile uint32 *) (DMA_CFG) |= BIT_0;

        while (! ( (* (volatile uint32 *) (DMA_CFG)) & DMA_PAUSE_BIT))
#endif
        {
            time_out++;

            if (time_out > DMA_PAUSE_TIME_OUT)
            {
                SCI_PASSERT (0, ("DMA PAUSE TIME OUT!"));/*assert verified*/
                break;
            }
        }
    }
    else
    {
#if defined(PLATFORM_SC8800H)
        * (volatile uint32 *) (DMA_CFG) &= ~BIT_8;

#elif defined(PLATFORM_SC6600L)
        * (volatile uint32 *) (DMA_CFG) &= 0xfffffffe;
#endif
    }

}

/*****************************************************************************/
//  Description:    dma request enable/disable
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           channel :  channel No
//                  is_enable: enable/disable
/*****************************************************************************/
PUBLIC uint32 DMA_ChannelEnable (uint32 channel, BOOLEAN is_enable)
{

    SCI_ASSERT (channel < DMA_CH_NUM);/*assert verified*/
    /*
        if dma is used, it is not permit to change frequency of ahb(sdram),we clear
        dma event of FREQ_CHNG instead.
    */

    SCI_DisableIRQ();
    SCI_DisableFIQ();

    //add DMA protect when enters deepsleep
    if (is_enable)
    {
        s_dma_chn_uesd_flag |= (1 << channel);
        SCI_DMA_EnableDeepSleep (DISABLE_DEEP_SLEEP);

    }

    DMA_CLR_INT (channel);

    if (is_enable)
    {
        MMU_INVALIDE_DCACHE
    }

    if (s_dma_chn_ctl[channel].type == DMA_TYPE_HARD)
    {
        DMA_HWEN (channel,is_enable);
        DMA_CHNEN (channel,is_enable);
    }
    else
    {
        DMA_CHNEN (channel,is_enable);
        DMA_CHN_SOFT_REQ (channel,is_enable);
    }

    /*
        after dma copy was finished, we set dma event of FREQ_CHNG,
        which will permit the change of ahb(sdram) frequency.
    */
    if (!is_enable)
    {
        s_dma_chn_uesd_flag &= ~ (1 << channel);

        if (!s_dma_chn_uesd_flag)
        {
            SCI_DMA_EnableDeepSleep (ENABLE_DEEP_SLEEP);
        }
    }

    SCI_RestoreFIQ();
    SCI_RestoreIRQ();

    return ERR_DMA_NONE;
}

/*****************************************************************************/
//  Description:    dma memcpy
//  Global resource dependence:
//  Author:         weihua.wang
//  input param     des_address: New buffer
//                  src_address: Buffer to copy from
//                  count      : Number of characters to copy in bytes
//  return value    zero       : param error or no available channel
//                  other value: writed in bytes
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_memcpy (void *des, void *src, uint32 count)
{
    uint32 desleft;
    uint32 srcleft;
    uint32 presend;           //before dma transfer
    uint32 postsend;          //after dma transfer
    uint32 midsend;           //is 4 aligned
    uint32 cnt;
    uint32 i;
    uint32 channel;
    uint32 cfg_flag;
    uint8 *srcptr = NULL;
    uint8 *desptr = NULL;
    DMA_CTL_CFG_T chn_param = {0};
    uint32 dma_type_back_up = 0;
    uint8 *des_addr = (uint8 *) des;
    uint8 *src_addr = (uint8 *) src;

    SCI_ASSERT (des_addr != NULL);/*assert verified*/

    if (src_addr == NULL)
    {
        return 0;
    }

    if (count <= 16)
    {
        while (count > 0)
        {

            * (des_addr++) = * (src_addr++);
            count--;
        }

        return (uint32) des_addr;
    }

#ifdef VM_SUPPORT

    if (VM_IsDPSection ( (uint32) src_addr))
    {
        SCI_MEMCPY (des_addr, src_addr, count);
        return (uint32) des_addr;
    }

#endif

    //judge the des address/src address
    desleft = (uint32) des_addr & 0x00000003;
    srcleft = (uint32) src_addr & 0x00000003;
    cfg_flag = 0;

    //only when src address and des address are the same type
    //we can do presend and post send
    presend  = 0;
    postsend = 0;
    midsend  = count;

    if (desleft == srcleft)
    {

        if (desleft != 0)
        {
            presend  = 4 - desleft;
            postsend = (count - presend) & 0x03;
            midsend  = count - presend - postsend;
        }
        else
        {
            postsend = count & 0x03;
            midsend  = count - postsend;
        }

        cfg_flag |= (CHN_SRC_SIZE_WORD | CHN_DES_SIZE_WORD);
    }
    else
    {

        if (desleft%2)
        {
            cfg_flag |= CHN_DES_SIZE_BYTE;
        }
        else if (desleft & 0x2)                     //Nick Modified
        {
            cfg_flag |= CHN_DES_SIZE_HALFWORD;
        }
        else
        {
            cfg_flag |= CHN_DES_SIZE_WORD;
        }

        if (srcleft%2)
        {
            cfg_flag |= CHN_SRC_SIZE_BYTE;
        }
        else if (srcleft & 0x2)                     //Nick Modified
        {
            cfg_flag |= CHN_SRC_SIZE_HALFWORD;
        }
        else
        {
            cfg_flag |= CHN_SRC_SIZE_WORD;
        }

        postsend = count & 0x03;
        midsend  = count - postsend;
    }

    //request dma soft channel, if current no channal is available
    //then sleep some time and retry
    while ( (channel = DMA_AllocChn()) == 0xffffffff)
    {
        SCI_Sleep (20);
    }

    TM_SendTestPointRequest ( (0x09096000 + channel), count);
    dma_type_back_up = s_dma_chn_ctl[channel].type;
    s_dma_chn_ctl[channel].type = DMA_TYPE_SOFT;

    //pre send before dma
    if (presend > 0)
    {
        cnt = presend;

        while (cnt > 0)
        {
            * (des_addr++) = * (src_addr++);
            cnt--;
        }
    }

    if (midsend > 0)
    {
        chn_param.dma_workmode = DMA_MODE_NORMAL;
        chn_param.burst_size   = DMA_SOFT_BURSTSIZE;
        chn_param.linklistptr  = 0;
#ifdef PLATFORM_SC8800H
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        chn_param.flag         = cfg_flag | CHN_TRANS_MODE | CHN_SRC_INCREAMENT | CHN_DES_INCREAMENT | CHN_ENDIAN_SEL_LITTLE;
#else
        chn_param.flag         = cfg_flag | CHN_TRANS_MODE | CHN_SRC_INCREAMENT | CHN_DES_INCREAMENT;
#endif
        chn_param.src_burst_mode = DMA_BURSTMODE_INCR16;
        chn_param.des_burst_mode = DMA_BURSTMODE_INCR16;
#elif defined (PLATFORM_SC6600L)
        chn_param.flag         = cfg_flag | CHN_ALL_TRANS_EN | CHN_SRC_INCREAMENT | CHN_DES_INCREAMENT;
#endif
        chn_param.src_burst_step = 0; //burst continous
        chn_param.des_burst_step = 0; //burst continous

        i = 0;   //for the src/des address modify
        cnt = midsend;

        while (cnt > DMA_MAX_TRANSSIZE)
        {
            //dma transfer
            //set dma config
            chn_param.src_address  = (uint32) src_addr + i*DMA_MAX_TRANSSIZE;
            chn_param.des_address  = (uint32) des_addr + i*DMA_MAX_TRANSSIZE;
            chn_param.total_size   = DMA_MAX_TRANSSIZE;

            chn_param.priority = DMA_PRIORITY_1;
            DMA_ChannelCfg (channel,DMA_TYPE_SOFT,DMA_INT_SEL_NONE,&chn_param);

            //soft dma enable
            DMA_ChannelEnable (channel, SCI_TRUE);

            //wait dma transfer end
            while (! (DMA_CHNDONE_STS (channel)))
            {
                ;
            }

            cnt -= DMA_MAX_TRANSSIZE;
            i++;
        }

        if (cnt)
        {
            //set remain dma transfer
            chn_param.src_address  = (uint32) src_addr + i*DMA_MAX_TRANSSIZE;
            chn_param.des_address  = (uint32) des_addr + i*DMA_MAX_TRANSSIZE;
            chn_param.total_size   = cnt;

            DMA_ChannelCfg (channel,DMA_TYPE_SOFT,DMA_INT_SEL_NONE,&chn_param);

            //soft dma enable
            DMA_ChannelEnable (channel, SCI_TRUE);

            //wait dma transfer end
            while (! (DMA_CHNDONE_STS (channel)))
            {
                ;
            }
        }
    }

    s_dma_chn_ctl[channel].type = dma_type_back_up;
    DMA_ChannelEnable (channel,FALSE);

    //post send after dma
    if (postsend > 0)
    {
        cnt = postsend;

        desptr = (uint8 *) des_addr +  midsend;
        srcptr = (uint8 *) src_addr +  midsend;

        while (cnt > 0)
        {
            *desptr++ = *srcptr++;

            cnt--;
        }
    }

    DMA_FreeChn (channel);

    return (uint32) des_addr;
}

/*****************************************************************************/
//  Description:    dma memset
//  Global resource dependence:
//  Author:         weihua.wang
//  input param     des_address: New buffer
//                  value      : value to set
//                  count      : Number of characters to copy in bytes
//  return value    zero       : param error or no available channel
//                  other value: writed in bytes
//  Note:           soft dma operation
/*****************************************************************************/
PUBLIC uint32 DMA_memset (void *des, uint8 value, uint32 count)
{
    uint32 desleft;
    uint32 presend;           //before dma transfer
    uint32 midsend;          //is 4 aligned
    uint32 postsend;
    uint32 cnt;
    uint32 i;
    uint32 channel;
    DMA_CTL_CFG_T chn_param;
    uint32 dma_type_back_up = 0;
    uint8 *des_addr = (uint8 *) des;

    SCI_ASSERT (des_addr != NULL);/*assert verified*/

    if (count <= 16)
    {
        while (count > 0)
        {
            * (des_addr++) = value;
            count--;
        }

        return (uint32) des_addr;
    }

    //judge the des address/src address to handle word align
    desleft = (uint32) des_addr & 0x00000003;

    if (desleft != 0)
    {
        presend  = 4 - desleft;
        postsend = (count - presend) & 0x03;
        midsend  = count - presend - postsend;
    }
    else
    {
        presend  = 0;
        postsend = count & 0x03;
        midsend  = count - postsend;
    }

    //request dma soft channel, if current no channal is available
    //then sleep some time and retry
    while ( (channel = DMA_AllocChn()) == 0xffffffff)
    {
        SCI_Sleep (20);
    }

    TM_SendTestPointRequest ( (0x09095000 + channel), count);

    dma_type_back_up = s_dma_chn_ctl[channel].type;
    s_dma_chn_ctl[channel].type = DMA_TYPE_SOFT;

    //src data value, word size
    s_dma_chn_ctl[channel].reserved = (value << 24) | (value << 16) | (value << 8) | (value & 0xff);

    //pre send before dma , less than 4 byte
    if (presend > 0)
    {
        cnt = presend;

        while (cnt > 0)
        {
            * (des_addr++) = value;
            cnt--;
        }
    }

    //middle send by dma, word align
    if (midsend > 0)
    {
        chn_param.dma_workmode = DMA_MODE_NORMAL;
        chn_param.burst_size   = DMA_SOFT_BURSTSIZE;
        chn_param.linklistptr  = 0;
#ifdef PLATFORM_SC8800H
        chn_param.flag         = CHN_TRANS_MODE | CHN_SRC_SIZE_WORD | CHN_DES_SIZE_WORD| CHN_SRC_NOCHANGE | CHN_DES_INCREAMENT;
        chn_param.src_burst_mode = DMA_BURSTMODE_INCR16;
        chn_param.des_burst_mode = DMA_BURSTMODE_INCR16;
#elif defined (PLATFORM_SC6600L)
        chn_param.flag         = CHN_ALL_TRANS_EN | CHN_SRC_SIZE_WORD | CHN_DES_SIZE_WORD| CHN_SRC_NOCHANGE | CHN_DES_INCREAMENT;
#endif
        chn_param.src_burst_step = 0; //burst continous
        chn_param.des_burst_step = 0; //burst continous

        i = 0;   //for the src/des address modify
        cnt = midsend;

        while (cnt > DMA_MAX_TRANSSIZE)
        {
            //dma transfer, set dma config param
            chn_param.src_address  = (uint32) &s_dma_chn_ctl[channel].reserved ;
            chn_param.des_address  = (uint32) des_addr + i*DMA_MAX_TRANSSIZE;
            chn_param.total_size   = DMA_MAX_TRANSSIZE;

            DMA_ChannelCfg (channel,DMA_TYPE_SOFT,DMA_INT_SEL_NONE,&chn_param);

            //soft dma enable
            DMA_ChannelEnable (channel, SCI_TRUE);

            //wait dma transfer end
            while (! (DMA_CHNDONE_STS (channel)))
            {
                ;
            }

            cnt -= DMA_MAX_TRANSSIZE;
            i++;
        }

        if (cnt)
        {
            //set remain dma transfer
            chn_param.src_address  = (uint32) &s_dma_chn_ctl[channel].reserved;
            chn_param.des_address  = (uint32) des_addr + i*DMA_MAX_TRANSSIZE;
            chn_param.total_size   = cnt;

            DMA_ChannelCfg (channel,DMA_TYPE_SOFT,DMA_INT_SEL_NONE,&chn_param);

            //soft dma enable
            DMA_ChannelEnable (channel, SCI_TRUE);

            //wait dma transfer end
            while (! (DMA_CHNDONE_STS (channel)))
            {
                ;
            }
        }
    }

    if (postsend > 0)
    {
        des_addr = (uint8 *) des_addr +  midsend;

        cnt = postsend;

        while (cnt > 0)
        {
            * (des_addr++) = value;
            cnt--;
        }
    }

    s_dma_chn_ctl[channel].type = dma_type_back_up;
    //disable DMA channel
    DMA_ChannelEnable (channel, SCI_FALSE);
    DMA_FreeChn (channel);

    return (uint32) des_addr;

}

/*****************************************************************************/
//  Description:    dma link list init handle
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void DMA_LinkInit (void)
{
    uint32 i;

    //SCI_TRACE_LOW:"DMA linkinit\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DMA_HAL_V1_807_112_2_17_23_4_8_946,(uint8*)"");

    s_dma_linkfreelist = (DMA_LINK_T *) s_dma_linkbuf ;

    /*first record is not used, it used as list head*/
    /*send all record to free list*/
    s_dma_linkfreelist->free_next = &s_dma_linkfreelist[1];
    s_dma_linkfreelist->free_prev = &s_dma_linkfreelist[DMA_MAX_LINK_NUM];

    for (i=1; i<= DMA_MAX_LINK_NUM; i++)
    {
        s_dma_linkfreelist[i].free_prev = &s_dma_linkfreelist[i-1];
        s_dma_linkfreelist[i].free_next = &s_dma_linkfreelist[i+1];
    }

    s_dma_linkfreelist[DMA_MAX_LINK_NUM].free_next = s_dma_linkfreelist;
}

/*****************************************************************************/
//  Description:    apply for node of dma link list
//  Global resource dependence:
//  Author:         weihua.wang
//  return value    pointer to dma_link_list,if zero: error occur
//  Note:
/*****************************************************************************/
PUBLIC DMA_LINK_PTR DMA_LinkAlloc (void)
{
    DMA_LINK_T *tmp;

    if (s_dma_linkfreelist->free_next == s_dma_linkfreelist->free_prev)
    {
        return 0;
    }

    tmp = s_dma_linkfreelist->free_next;

    SCI_DisableIRQ();
    s_dma_linkfreelist->free_next = tmp->free_next;
    tmp->free_next->free_prev = s_dma_linkfreelist;
    SCI_RestoreIRQ();

    tmp->free_next = tmp;
    tmp->free_prev = tmp;

    return ( (DMA_LINK_PTR) tmp);
}

/*****************************************************************************/
//  Description:    release node of dma link list
//  Global resource dependence:
//  Author:         weihua.wang
//  input param     plink --> node to be released
//  return value    ERR_DMA_NONE : success
//  Note:
/*****************************************************************************/
PUBLIC uint32 DMA_LinkFree (DMA_LINK_PTR plink)
{
    DMA_LINK_T *_plink = (DMA_LINK_T *) plink;

    SCI_ASSERT (plink != NULL);/*assert verified*/

    SCI_DisableIRQ();
    _plink->free_next = s_dma_linkfreelist;
    _plink->free_prev = s_dma_linkfreelist->free_prev;
    s_dma_linkfreelist->free_prev->free_next = _plink;
    s_dma_linkfreelist->free_prev = _plink;
    SCI_RestoreIRQ();

    return ERR_DMA_NONE;
}

/*****************************************************************************/
//  Description:    set the node and link to the last node
//  Global resource dependence:
//  Author:         weihua.wang
//  input param     plink --> list list current node
//                  src_address : src address
//                  des_address : des address
//                  totla num   : total transfer size
//                  flag        : dma config param
//                  prelink     : link list last node
//  return value    ERR_DMA_NONE : success
//  Note:
/*****************************************************************************/
PUBLIC uint32 DMA_LinkSet (DMA_LINK_PTR linkptr, DMA_CTL_CFG_PTR nodeinfo_ptr,DMA_LINK_PTR nextlink)
{
    DMA_LINK_T *plink = (DMA_LINK_T *) linkptr;
    DMA_CTL_CFG_T chn_param;

    SCI_ASSERT (linkptr != NULL);/*assert verified*/
    SCI_ASSERT (nextlink != NULL);/*assert verified*/
    SCI_ASSERT (nodeinfo_ptr != NULL);/*assert verified*/

    SCI_MEMCPY (&chn_param,nodeinfo_ptr,sizeof (DMA_CTL_CFG_T));
    chn_param.dma_workmode = DMA_MODE_NORMAL;

    return DMA_HALlinklistCfg (&chn_param,&plink->dma_node);
}

/*****************************************************************************/
//  Description:    alloc soft dma channel
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           return value : channel id, 0xffffffff -> no available
/*****************************************************************************/
uint32 DMA_AllocChn (void)
{
    uint32 i;
    uint32 max_val;

    max_val = s_dma_index.index_max;

    SCI_DisableIRQ();

    for (i = 0; i < max_val; i++)
    {
        if (s_dma_chn_ctl[s_dma_index.channel[i]].status == DMA_STS_IDLE)
        {
            s_dma_chn_ctl[s_dma_index.channel[i]].status = DMA_STS_BUSY;
            break;
        }
    }

    SCI_RestoreIRQ();

    if (i >= max_val)  //not available
    {
        return 0xffffffff;
    }
    else                 //find the idle chn
    {
        return s_dma_index.channel[i];
    }
}


/*****************************************************************************/
//  Description:    release channel
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
void DMA_FreeChn (uint32 channel)
{
    if (channel >= DMA_CH_NUM)
    {
        SCI_ASSERT (0);/*assert verified*/
    }
    else
    {
        s_dma_chn_ctl[channel].status = DMA_STS_IDLE;
    }
}


#ifdef   __cplusplus
}
#endif
