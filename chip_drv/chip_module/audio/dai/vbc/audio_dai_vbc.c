/******************************************************************************
 ** File Name:      audio_dai_vbc.c                                           *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of audio device. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang        Create.                                   *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "dma_drv_internal.h"

#include "chip_plf_export.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "clock_drvapi.h"

#include "audio_drvapi.h"
#include "audio_dai.h"
#include "audio_hal.h"
#include "vbc_phy.h"



/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
#ifdef VBC_DEBUG
#define VBC_DBG       SCI_TRACE_LOW
#define VBC_DBG_ASS   SCI_ASSERT
#else
#define VBC_DBG(...)
#define VBC_DBG_ASS(x)
#endif

#define VBC_PRT       SCI_TRACE_LOW

#define VBC_ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

#define VBC_CTL_BY_DSP  (0x80000000)
//open DAC & USE BUFFER.
#define VBC_DAC         (0x00000001)
//open ADC & USE BUFFER.
#define VBC_ADC         (0x00000002)
//open ADC & DAC & FM. //NOT USE BUFFER
#define VBC_FM_DA       (0x00000004)
//open ADC & AD FORM ALC &USE BUFFER
#define VBC_FM_AD       (0x00000008)
#define VBC_ST          (0x00000010)
//use AD iis port
#define VBC_USE_AD      (0x00000020)

#define AUDIO_VBC_IRQ_ENTRY() //do{ SCI_DisableIRQ(); }while(0)
#define AUDIO_VBC_IRQ_LEAVE() //do{ SCI_RestoreIRQ(); }while(0)

#define VBC_CLOSE_EVENT         (0x00000001)
#define VBC_CLOSE_WAIT_TIMEOUT  (8000)


#if defined (CHAN_L_TO_DA0)
#define DMA_VB_DAL_OFFSET DMA_VB_DA0_OFFSET
#define DMA_VB_DAR_OFFSET DMA_VB_DA1_OFFSET
#define DMA_VB_ADL_OFFSET DMA_VB_AD0_OFFSET
#define DMA_VB_ADR_OFFSET DMA_VB_AD1_OFFSET
#else
#define DMA_VB_DAL_OFFSET DMA_VB_DA1_OFFSET
#define DMA_VB_DAR_OFFSET DMA_VB_DA0_OFFSET
#define DMA_VB_ADL_OFFSET DMA_VB_AD1_OFFSET
#define DMA_VB_ADR_OFFSET DMA_VB_AD0_OFFSET
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

enum VBC_OC_E{
    VBC_CLOSE   = 0,
    VBC_OPEN
};


typedef struct
{
    uint32                  fun_flg;
    uint32                  fun_vbc;
} VBC_FUN_T;

typedef struct
{
    uint                    dac0:1;
    uint                    dac1:1;
    uint                    adc0:1;
    uint                    adc1:1;
    uint                    fm_da:1;
    uint                    fm_ad:1;
    uint                    dsp:1;
    uint                    vbc_open:1;
    uint                    vbc_en:1;
    uint                    da_trigger:1;
    uint                    ad_trigger:1;
    uint                    cus_da_trigger:1;
    uint                    send_ready:1;
    uint                    rsve:19;
} VBC_OPENED_T;

typedef struct
{
    uint32                  pLHead;
    uint32                  pRHead;
    uint32                  size;
    uint32                  pWrite;
    uint32                  pRead;
    uint32                  cnt;
    uint32                  max_cnt;
}VBC_BUFFER_T;

typedef struct
{
    uint32                  chan_id;
    DMA_CHNCFG_T            chan_cfg;
    uint32                  cmp;
}VBC_DMA_T;

typedef struct
{
    char                    magic_str[AUDIO_MAGIC_STR_MAX_LEN];
    
    VBC_OPENED_T            opened;
    uint32                  support_data_fmt;
    uint32                  data_fmt[2];
    VBC_BUFFER_T            buf[2];
    AUDIO_HAL_CALLBACK_FUN  cb[2];
    
    VBC_DMA_T               dma_L[2];
    VBC_DMA_T               dma_R[2];
    
    uint32                  save_pos[2];
    uint32                  need_call_cb[2];

    DEVICE_HANDLE           dev_audio;

    uint32                  buffer_div[2];

   
    uint32                  magic_num;
} VBC_DAI_T;



/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

LOCAL const VBC_FUN_T s_vbc_support_fun[]=
{
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_PLAYBACK))
                                ,VBC_CTL_BY_DSP             },
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_CAPTRUE))
                                ,VBC_CTL_BY_DSP             },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_PLAYBACK))
                                ,0                          },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_CAPTRUE))
                                ,VBC_ADC                    },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_PLAYBACK))
                                ,VBC_FM_DA                  },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_CAPTRUE))
                                ,VBC_ADC|VBC_FM_AD          },
    {((uint32)(AUDIO_FUN_AD_FROM_DA|AUDIO_FUN_PLAYBACK))
                                ,VBC_DAC                    },
    {((uint32)(AUDIO_FUN_AD_FROM_DA|AUDIO_FUN_CAPTRUE))
                                ,VBC_ADC|VBC_FM_AD          },
    {((uint32)(AUDIO_FUN_LINEIN_DIGITAL|AUDIO_FUN_PLAYBACK))
                                ,VBC_FM_DA|VBC_USE_AD       },
    {((uint32)(AUDIO_FUN_LINEIN_DIGITAL|AUDIO_FUN_CAPTRUE))
                                ,VBC_ADC|VBC_FM_AD          },                                
};
LOCAL const uint32 s_vbc_support_fun_len=VBC_ARR_SIZE(s_vbc_support_fun);

LOCAL VBC_DAI_T s_vbc;

LOCAL uint16 s_vbc_z_buf[VB_DA_BUF_SIZE] = {0};
LOCAL uint16 s_vbc_t_buf[VB_AD_BUF_SIZE] = {0};

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
LOCAL int32 _VBC_ADC_Set_Write_Point(uint32 pWrite);
LOCAL int32 _VBC_DAC_Set_Read_Point(uint32 pRead);
LOCAL int32 _VBC_ADC_Trigger(void);
LOCAL uint32 _VBC_Can_Close(void);


LOCAL uint32 _VBC_Get_Len(VBC_BUFFER_T * pBuf,uint32 is_write)
{
    if( (pBuf->pWrite) > (pBuf->pRead) )
    {
        return (pBuf->pWrite)-(pBuf->pRead);
    }
    else if( (pBuf->pWrite) < (pBuf->pRead) )
    {
        return ((pBuf->size)-(pBuf->pRead))+(pBuf->pWrite);
    }
    if( is_write )
    {
        return (pBuf->size);
    }
    return 0;
}

LOCAL uint32 _VBC_Get_Ava_Len(VBC_BUFFER_T * pBuf,uint32 is_send)
{
    uint32 p1 = 0;
    uint32 p2 = 0;
    uint32 cnt = 0;
    if( is_send )
    {
        p1 = (pBuf->pWrite);
        p2 = (pBuf->pRead);
    }
    else
    {
        p1 = (pBuf->pRead);
        p2 = (pBuf->pWrite);
    }
    
    if( p1 > p2 )
    {
        cnt = (pBuf->cnt);
    }
    else if( p1 < p2 )
    {
        cnt = (pBuf->size) - p2;
    }
    else
    {
        if( is_send )
        {
            if( pBuf->cnt )
            {
                cnt = (pBuf->size) - p2;
            }
        }
        else
        {
            if( 0== (pBuf->cnt) )
            {
                cnt = (pBuf->size);
            }
        }
    }
    if( cnt > (pBuf->max_cnt) )
    {
        cnt = (pBuf->max_cnt);
    }
    return cnt;
}

LOCAL uint32 _VBC_Get_Send_Len(VBC_BUFFER_T * pBuf)
{
    return _VBC_Get_Ava_Len(pBuf,1);
}

LOCAL uint32 _VBC_Get_Recv_Len(VBC_BUFFER_T * pBuf)
{
    return _VBC_Get_Ava_Len(pBuf,0);
}



LOCAL int32 _VBC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( (s_vbc.opened.vbc_open))
    {
        if( (0==s_vbc.opened.vbc_en) )
        {
            //VBC_DBG:"[AUDDRV][VBC]enable"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_266_112_2_17_23_3_10_300,(uint8*)"");
            __vbc_enable();
            s_vbc.opened.vbc_en = 1;
        }
    }
    else
    {
        //VBC_PRT:"[AUDDRV][VBC]vbc trigger,!open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_273_112_2_17_23_3_10_301,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}

LOCAL uint32 _VBC_DAC_DMA_Can_Trigger(void)
{
    uint32 tri = 0;
    AUDIO_VBC_IRQ_ENTRY();
    tri =  (s_vbc.buf[AUDIO_PLAYBACK].cnt*100)
        > (s_vbc.buf[AUDIO_PLAYBACK].size*AUDIO_PLAYBACK_AUTO_TRIGGER);
    AUDIO_VBC_IRQ_LEAVE();
        
    return tri;
}

LOCAL int32 _VBC_DAC_Get_Buffer(uint32 * pRead,uint32 *len,uint32 chan)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 read_pos = 0;
    AUDIO_VBC_IRQ_ENTRY();
    if( ( AUDIO_CHAN_L==(AUDIO_CHAN_L & (chan)) ) )
    {
        if( 0==s_vbc.buf[AUDIO_PLAYBACK].pLHead )
        {
            //VBC_PRT:"[AUDDRV][VBC]da pLHead=0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_299_112_2_17_23_3_11_302,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }
        else
        {
            read_pos  = s_vbc.buf[AUDIO_PLAYBACK].pLHead+(s_vbc.buf[AUDIO_PLAYBACK].pRead);
        }
    }
    else if( ( AUDIO_CHAN_R==(AUDIO_CHAN_R & (chan)) ) )
    {
        if( 0==s_vbc.buf[AUDIO_PLAYBACK].pRHead )
        {
            //VBC_PRT:"[AUDDRV][VBC]da pRHead=0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_311_112_2_17_23_3_11_303,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }
        else
        {
            read_pos  = s_vbc.buf[AUDIO_PLAYBACK].pRHead+(s_vbc.buf[AUDIO_PLAYBACK].pRead);
        }
    }
    else
    {
        //VBC_PRT:"[AUDDRV][VBC]da get buffer:chan=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_321_112_2_17_23_3_11_304,(uint8*)"d",chan);
        ret = AUDIO_HAL_ERROR;
    }
    if( read_pos )
    {
        *pRead  = read_pos;
        *len    = _VBC_Get_Send_Len(&(s_vbc.buf[AUDIO_PLAYBACK]));
    }
    AUDIO_VBC_IRQ_LEAVE();
    return ret;
}

LOCAL void _VBC_DAC_Update_Save_Pos(uint32 len)
{
    AUDIO_VBC_IRQ_ENTRY();
    s_vbc.save_pos[AUDIO_PLAYBACK] = (s_vbc.buf[AUDIO_PLAYBACK].pRead) + len;
    AUDIO_VBC_IRQ_LEAVE();
}

LOCAL uint32 _VBC_Need_Send_Z(void)
{
    return (uint32)(!(s_vbc.opened.send_ready));
}

LOCAL void _VBC_DMA_Send_Data_Log(uint32 is_z)
{
    static uint32 is_z_pre = 0xFF;
    if( is_z_pre != is_z )
    {
        if( is_z )
        {
            //VBC_PRT:"[AUDDRV][VBC][REF]no data send"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_352_112_2_17_23_3_11_305,(uint8*)"");
        }
        else
        {
            //VBC_PRT:"[AUDDRV][VBC]start send"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_356_112_2_17_23_3_11_306,(uint8*)"");
        }
        is_z_pre = is_z;
    }
}

LOCAL int32 _VBC_DMA_DAC_Send(VBC_DMA_T * pDMACfg,uint32 chan, int32 interrupt)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 is_z = 0;
    if( AUDIO_HAL_SUCCESS ==
        _VBC_DAC_Get_Buffer(&(pDMACfg->chan_cfg.src_address),&(pDMACfg->chan_cfg.total_length),chan))
    {
        if(( _VBC_Need_Send_Z() ) || (pDMACfg->chan_cfg.total_length)<(pDMACfg->chan_cfg.block_length) )
        {///TODO : EMPTY SEND ZERO
            pDMACfg->chan_cfg.src_address = (uint32)(&(s_vbc_z_buf));
            pDMACfg->chan_cfg.total_length = (pDMACfg->chan_cfg.block_length);
            s_vbc.need_call_cb[AUDIO_PLAYBACK] = 0;
        }
        else
        {
            uint32 div_s = (pDMACfg->chan_cfg.total_length)/(pDMACfg->chan_cfg.block_length);
            pDMACfg->chan_cfg.total_length = div_s*(pDMACfg->chan_cfg.block_length);
            _VBC_DAC_Update_Save_Pos(pDMACfg->chan_cfg.total_length);
            s_vbc.need_call_cb[AUDIO_PLAYBACK] = 1;
        }
        ret = __vbc_dma_dac_chan_en(&(pDMACfg->chan_cfg),pDMACfg->chan_id);
        if(!interrupt)/*lint !e26 -e26*/
            __vbc_dma_da_chn_en(1,chan);
        is_z = ((pDMACfg->chan_cfg.src_address == (uint32)(&(s_vbc_z_buf))));
        _VBC_DMA_Send_Data_Log(is_z);
    }
    else
    {
        //VBC_PRT:"[AUDDRV][VBC]dma send err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_389_112_2_17_23_3_11_307,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _VBC_ADC_Get_Buffer(uint32 * pWrite,uint32 *len,uint32 chan)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 write_pos = 0;
    AUDIO_VBC_IRQ_ENTRY();
    if( ( AUDIO_CHAN_L==(AUDIO_CHAN_L & (chan)) ) )
    {
        if( 0==s_vbc.buf[AUDIO_CAPTRUE].pLHead )
        {
            //VBC_PRT:"[AUDDRV][VBC]ad pLHead=0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_405_112_2_17_23_3_11_308,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }
        else
        {
            write_pos  = s_vbc.buf[AUDIO_CAPTRUE].pLHead+(s_vbc.buf[AUDIO_CAPTRUE].pWrite);
        }
    }
    else if( ( AUDIO_CHAN_R==(AUDIO_CHAN_R & (chan)) ) )
    {
        if( 0==s_vbc.buf[AUDIO_CAPTRUE].pRHead )
        {
            //VBC_PRT:"[AUDDRV][VBC]ad pRHead=0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_417_112_2_17_23_3_11_309,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }
        else
        {
            write_pos  = s_vbc.buf[AUDIO_CAPTRUE].pRHead+(s_vbc.buf[AUDIO_CAPTRUE].pWrite);
        }
    }
    else
    {
        //VBC_PRT:"[AUDDRV][VBC]ad get buffer:chan=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_427_112_2_17_23_3_11_310,(uint8*)"d",chan);
        ret = AUDIO_HAL_ERROR;
    }
    if( write_pos )
    {
        *pWrite  = write_pos;
        *len    = _VBC_Get_Recv_Len(&(s_vbc.buf[AUDIO_CAPTRUE]));
    }
    AUDIO_VBC_IRQ_LEAVE();
    return ret;
}

LOCAL void _VBC_ADC_Update_Save_Pos(uint32 len)
{
    AUDIO_VBC_IRQ_ENTRY();
    s_vbc.save_pos[AUDIO_CAPTRUE] = (s_vbc.buf[AUDIO_CAPTRUE].pWrite) + len;
    AUDIO_VBC_IRQ_LEAVE();
}

LOCAL void _VBC_DMA_Rcev_Data_Log(uint32 is_full)
{
    static uint32 is_full_pre = 0xFF;
    if( is_full_pre != is_full )
    {
        if( is_full )
        {
            //VBC_PRT:"[AUDDRV][VBC][REF]recv data not write"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_453_112_2_17_23_3_11_311,(uint8*)"");
        }
        else
        {
            //VBC_PRT:"[AUDDRV][VBC]start recv"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_457_112_2_17_23_3_11_312,(uint8*)"");
        }
        is_full_pre = is_full;
    }
}

LOCAL int32 _VBC_DMA_ADC_Recv(VBC_DMA_T * pDMACfg,uint32 chan, int32 interrupt)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( AUDIO_HAL_SUCCESS ==
        _VBC_ADC_Get_Buffer(&(pDMACfg->chan_cfg.dest_address),&(pDMACfg->chan_cfg.total_length),chan))
    {
        if( (pDMACfg->chan_cfg.total_length)<(pDMACfg->chan_cfg.block_length) )
        {///TODO : FULL RCEV WAIT.
            pDMACfg->chan_cfg.dest_address = (uint32)(&(s_vbc_t_buf));
            pDMACfg->chan_cfg.total_length = (pDMACfg->chan_cfg.block_length);
            s_vbc.need_call_cb[AUDIO_CAPTRUE] = 0;
        }
        else
        {
            uint32 div_s = (pDMACfg->chan_cfg.total_length)/(pDMACfg->chan_cfg.block_length);
            pDMACfg->chan_cfg.total_length = div_s*(pDMACfg->chan_cfg.block_length);
            _VBC_ADC_Update_Save_Pos(pDMACfg->chan_cfg.total_length);
           s_vbc.need_call_cb[AUDIO_CAPTRUE] = 1;
        }
        ret = __vbc_dma_adc_chan_en(&(pDMACfg->chan_cfg),pDMACfg->chan_id);
        if(!interrupt)
            __vbc_dma_ad_chn_en(1,chan);
        _VBC_DMA_Rcev_Data_Log((uint32)(pDMACfg->chan_cfg.dest_address == (uint32)(&(s_vbc_t_buf))));
    }
    else
    {
        //VBC_PRT:"[AUDDRV][VBC]dma recv err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_488_112_2_17_23_3_11_313,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}




LOCAL int32 _VBC_DMA_Start_Chan(uint32 path,uint32 chan, int32 interrupt)
{
#define VBC_OPENED_SEL(path,n) ((AUDIO_PLAYBACK==path)?(s_vbc.opened.dac##n):(s_vbc.opened.adc##n))
    int32 ret = AUDIO_HAL_SUCCESS;
    VBC_DMA_T * pDMACfg = 0;

    if( ( AUDIO_CHAN_L==(AUDIO_CHAN_L & (chan)) ) )
    {
        if( VBC_OPENED_SEL(path,0) )
        {
            pDMACfg    = &(s_vbc.dma_L[path]);
        }
        else
        {
            if( AUDIO_DATA_FMT_MONO == (s_vbc.data_fmt[path]) )
            {
                if( VBC_OPENED_SEL(path,1) )
                {
                    pDMACfg    = &(s_vbc.dma_R[path]);
                }
            }
        }
    }
    else if( ( AUDIO_CHAN_R==(AUDIO_CHAN_R & (chan)) ) )
    {
        if( VBC_OPENED_SEL(path,1) )
        {
            pDMACfg    = &(s_vbc.dma_R[path]);
        }
    }
    else
    {
        //VBC_PRT:"[AUDDRV][VBC]dma start:path=0x%x,chan=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_529_112_2_17_23_3_11_314,(uint8*)"dd",path,chan);
        return AUDIO_HAL_ERROR;
    }
    if( pDMACfg )
    {
        if( (AUDIO_PLAYBACK==path) )
        {
            ret = _VBC_DMA_DAC_Send(pDMACfg,chan, interrupt);
        }
        else
        {
            ret = _VBC_DMA_ADC_Recv(pDMACfg,chan, interrupt);
        }
    }
    else
    {
        //VBC_PRT:"[AUDDRV][VBC]dma start path=0x%x,xc0 %d,x1 %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_545_112_2_17_23_3_11_315,(uint8*)"ddd",path,VBC_OPENED_SEL(path,0),VBC_OPENED_SEL(path,1));
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _VBC_DMA_Start(uint32 path, int32 interrupt)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    _VBC_DMA_Start_Chan(path,AUDIO_CHAN_L, interrupt);
    if( AUDIO_DATA_FMT_LL_RR == (s_vbc.data_fmt[path]) )
    {
        _VBC_DMA_Start_Chan(path,AUDIO_CHAN_R, interrupt);
    }
    return ret;
}


LOCAL int32 _VBC_DMA_Send(int32 interrupt)
{
    return _VBC_DMA_Start(AUDIO_PLAYBACK, interrupt);
}

LOCAL int32 _VBC_DMA_Recv(int32 interrupt)
{
    return _VBC_DMA_Start(AUDIO_CAPTRUE, interrupt);
}

LOCAL int32 _VBC_DMA_ADC_Real_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    s_vbc.opened.ad_trigger = 1;
    __vbc_set_buffer_size(0,(s_vbc.dma_L[AUDIO_PLAYBACK].chan_cfg.block_length>>1));
    _VBC_DMA_Recv(0);
    if( AUDIO_HAL_SUCCESS != _VBC_Trigger() )
    {
        s_vbc.opened.ad_trigger = 0;
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}

LOCAL int32 _VBC_DMA_DAC_Real_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //VBC_DBG:"[AUDDRV][VBC]tried"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_591_112_2_17_23_3_11_316,(uint8*)"");
    s_vbc.opened.da_trigger = 1;
    __vbc_set_buffer_size((s_vbc.dma_L[AUDIO_PLAYBACK].chan_cfg.block_length>>1),0);
    _VBC_DMA_Send(0);
    if( AUDIO_HAL_SUCCESS != _VBC_Trigger() )
    {
        s_vbc.opened.da_trigger = 0;
        ret = AUDIO_HAL_ERROR;
    }
    else
    {
        AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DAC,AUDIO_UNMUTE);
    }
    return ret;
}

// return 1  : Real Trigger.
// return 0  : do none.
// return <1 : error
LOCAL int32 _VBC_DMA_Auto_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( (0==s_vbc.opened.dsp) && (0==s_vbc.opened.da_trigger) )
    {
        if( _VBC_DAC_DMA_Can_Trigger() )
        {
            ret = _VBC_DMA_DAC_Real_Trigger();
        }
    }
    return ret;
}

LOCAL void _VBC_DA_DMA_CB_Proc(void)
{
    if(! ( ((s_vbc.opened.dac0) && (0==s_vbc.dma_L[AUDIO_PLAYBACK].cmp ))
     || ((s_vbc.opened.dac1) && (0==s_vbc.dma_R[AUDIO_PLAYBACK].cmp )) )
    )
    {
        AUDIO_HAL_PRTMEM ( 0xAA000001 );
        if( (s_vbc.need_call_cb[AUDIO_PLAYBACK]) )
        {
            _VBC_DAC_Set_Read_Point(s_vbc.save_pos[AUDIO_PLAYBACK]);
        }
        s_vbc.dma_L[AUDIO_PLAYBACK].cmp = 0;
        s_vbc.dma_R[AUDIO_PLAYBACK].cmp = 0;
        _VBC_DMA_Send(1);
        AUDIO_HAL_PRTMEM ( 0xAA800001 );
    }
}

LOCAL void _VBC_DAL_DMA_Callback(uint32 param)
{
    s_vbc.dma_L[AUDIO_PLAYBACK].cmp = 1;
    _VBC_DA_DMA_CB_Proc();
}
LOCAL void _VBC_DAR_DMA_Callback(uint32 param)
{
    s_vbc.dma_R[AUDIO_PLAYBACK].cmp = 1;
    _VBC_DA_DMA_CB_Proc();
}

LOCAL void _VBC_AD_DMA_CB_Proc(void)
{
    if(! ( ((s_vbc.opened.adc0) && (0==s_vbc.dma_L[AUDIO_CAPTRUE].cmp ))
     || ((s_vbc.opened.adc1) && (0==s_vbc.dma_R[AUDIO_CAPTRUE].cmp )) )
    )
    {
        AUDIO_HAL_PRTMEM ( 0xAA000002 );
        if( (s_vbc.need_call_cb[AUDIO_CAPTRUE]) )
        {
            _VBC_ADC_Set_Write_Point(s_vbc.save_pos[AUDIO_CAPTRUE]);
        }
        s_vbc.dma_L[AUDIO_CAPTRUE].cmp = 0;
        s_vbc.dma_R[AUDIO_CAPTRUE].cmp = 0;
        _VBC_DMA_Recv(1);
        AUDIO_HAL_PRTMEM ( 0xAA800002 );
    }
}


LOCAL void _VBC_ADL_DMA_Callback(uint32 param)
{
    s_vbc.dma_L[AUDIO_CAPTRUE].cmp = 1;
    _VBC_AD_DMA_CB_Proc();
}
LOCAL void _VBC_ADR_DMA_Callback(uint32 param)
{
    s_vbc.dma_R[AUDIO_CAPTRUE].cmp = 1;
    _VBC_AD_DMA_CB_Proc();
}


LOCAL int32 _VBC_DMA_Open(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    VBC_DMA_ALLOC(DMA_VB_DAL_OFFSET,s_vbc.dma_L[AUDIO_PLAYBACK].chan_id);
    VBC_DMA_ALLOC(DMA_VB_DAR_OFFSET,s_vbc.dma_R[AUDIO_PLAYBACK].chan_id);
    DMA_HAL_ChnIntConfig (s_vbc.dma_L[AUDIO_PLAYBACK].chan_id,
        DMA_CHN_INT_TYPE_TRANSDONE,_VBC_DAL_DMA_Callback);
    DMA_HAL_ChnIntConfig (s_vbc.dma_R[AUDIO_PLAYBACK].chan_id,
        DMA_CHN_INT_TYPE_TRANSDONE,_VBC_DAR_DMA_Callback);


    VBC_DMA_ALLOC(DMA_VB_ADL_OFFSET,s_vbc.dma_L[AUDIO_CAPTRUE].chan_id);
    VBC_DMA_ALLOC(DMA_VB_ADR_OFFSET,s_vbc.dma_R[AUDIO_CAPTRUE].chan_id);
    DMA_HAL_ChnIntConfig (s_vbc.dma_L[AUDIO_CAPTRUE].chan_id,
        DMA_CHN_INT_TYPE_TRANSDONE,_VBC_ADL_DMA_Callback);
    DMA_HAL_ChnIntConfig (s_vbc.dma_R[AUDIO_CAPTRUE].chan_id,
        DMA_CHN_INT_TYPE_TRANSDONE,_VBC_ADR_DMA_Callback);
    return ret;
}

LOCAL int32 _VBC_DMA_ChnDisable(uint32 chan_id)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if(VBC_DMA_NO_CHN_VAL != chan_id )
    {
        DMA_HAL_ChnEnable(chan_id    ,SCI_FALSE);
    }
    else
    {
        VBC_PRT("DMA disable?");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}

LOCAL int32 _VBC_DMA_AD_Disable(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    ret= _VBC_DMA_ChnDisable(s_vbc.dma_L[AUDIO_CAPTRUE].chan_id);
     if(AUDIO_HAL_SUCCESS!=ret){
        return ret;
    }
    ret=_VBC_DMA_ChnDisable(s_vbc.dma_R[AUDIO_CAPTRUE].chan_id);
    return ret;
}//lint !e533

LOCAL int32 _VBC_DMA_DA_Disable(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    ret= _VBC_DMA_ChnDisable(s_vbc.dma_L[AUDIO_PLAYBACK].chan_id);
    if(AUDIO_HAL_SUCCESS!=ret){
        return ret;
    }
    ret=_VBC_DMA_ChnDisable(s_vbc.dma_R[AUDIO_PLAYBACK].chan_id);
    return ret;
}//lint !e533

LOCAL int32 _VBC_DMA_Close(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    _VBC_DMA_AD_Disable();
    _VBC_DMA_DA_Disable();

    DMA_HAL_ChnIntConfig(s_vbc.dma_L[AUDIO_PLAYBACK].chan_id   , DMA_CHN_INT_TYPE_NONE  ,NULL);
    DMA_HAL_ChnIntConfig(s_vbc.dma_R[AUDIO_PLAYBACK].chan_id   , DMA_CHN_INT_TYPE_NONE  ,NULL);
    DMA_HAL_ChnIntConfig(s_vbc.dma_L[AUDIO_CAPTRUE].chan_id    , DMA_CHN_INT_TYPE_NONE  ,NULL);
    DMA_HAL_ChnIntConfig(s_vbc.dma_R[AUDIO_CAPTRUE].chan_id    , DMA_CHN_INT_TYPE_NONE  ,NULL);

    VBC_DMA_FREE(DMA_VB_DAL_OFFSET,s_vbc.dma_L[AUDIO_PLAYBACK].chan_id   );
    VBC_DMA_FREE(DMA_VB_DAR_OFFSET,s_vbc.dma_R[AUDIO_PLAYBACK].chan_id   );
    VBC_DMA_FREE(DMA_VB_ADL_OFFSET,s_vbc.dma_L[AUDIO_CAPTRUE].chan_id    );
    VBC_DMA_FREE(DMA_VB_ADR_OFFSET,s_vbc.dma_R[AUDIO_CAPTRUE].chan_id    );
    return ret;
}





// rtx : is need open int/dma etc...
LOCAL int32 _VBC_Parse_Base_Action(uint32 fun_flg,uint32 oc,uint32 rtx)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //VBC_DBG:"[AUDDRV][VBC]base action 0x%x,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_732_112_2_17_23_3_11_317,(uint8*)"ddd",fun_flg,oc,rtx);
    if( AUDIO_FUN_PLAYBACK & fun_flg)
    {
        if( AUDIO_FUN_DAC0 & fun_flg)
        {
            if( oc!=s_vbc.opened.dac0 )
            {
                //VBC_DBG:"[AUDDRV][VBC]dac0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_739_112_2_17_23_3_12_318,(uint8*)"");
                s_vbc.opened.dac0 = oc;
                __vbc_da_enable(oc,AUDIO_CHAN_L);
                if( rtx )
                {
                    __vbc_dma_da_chn_en(oc,AUDIO_CHAN_L);
                }
            }
        }
        if( AUDIO_FUN_DAC1 & fun_flg)
        {
            if( oc!=s_vbc.opened.dac1 )
            {
                //VBC_DBG:"[AUDDRV][VBC]dac1"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_752_112_2_17_23_3_12_319,(uint8*)"");
                s_vbc.opened.dac1 = oc;
                __vbc_da_enable(oc,AUDIO_CHAN_R);
                if( rtx )
                {
                    __vbc_dma_da_chn_en(oc,AUDIO_CHAN_R);
                }
            }
        }
    }
    if( AUDIO_FUN_CAPTRUE & fun_flg)
    {
        if( AUDIO_FUN_ADC0 & fun_flg)
        {
            if( oc!=s_vbc.opened.adc0 )
            {
                if(!( s_vbc.opened.fm_da && (VBC_CLOSE==oc)))
                {
                    //VBC_DBG:"[AUDDRV][VBC]adc0"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_768_112_2_17_23_3_12_320,(uint8*)"");
                    s_vbc.opened.adc0 = oc;
                    __vbc_ad_enable(oc,AUDIO_CHAN_L);
                }
                if( rtx )
                {
                    __vbc_dma_ad_chn_en(oc,AUDIO_CHAN_L);
                }
            }
        }
        if( AUDIO_FUN_ADC1 & fun_flg)
        {
            if( oc!=s_vbc.opened.adc1 )
            {
                if(!( s_vbc.opened.fm_da && (VBC_CLOSE==oc)))
                {
                    //VBC_DBG:"[AUDDRV][VBC]adc1"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_781_112_2_17_23_3_12_321,(uint8*)"");
                    s_vbc.opened.adc1 = oc;
                    __vbc_ad_enable(oc,AUDIO_CHAN_R);
                }
                if( rtx )
                {
                    __vbc_dma_ad_chn_en(oc,AUDIO_CHAN_R);
                }
            }
        }
    }
    return ret;
}

LOCAL int32 _VBC_Parse_Fun_Conflict(uint32 fun_flg,uint32 oc)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    if( ((AUDIO_FUN_DSP & fun_flg) && (VBC_OPEN == oc)) )
    {
        if( !_VBC_Can_Close() )
        {
            //VBC_PRT:"[AUDDRV][VBC]dsp open, must close all other!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_802_112_2_17_23_3_12_322,(uint8*)"");
            return AUDIO_HAL_ERROR;
        }
    }
    
    if( (s_vbc.opened.dsp) )
    {//opened dsp function, then MUST Close dsp function.
        if( !((AUDIO_FUN_DSP & fun_flg) && (VBC_CLOSE == oc)) )
        {
            //VBC_PRT:"[AUDDRV][VBC]dsp, must close dsp,%08x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_811_112_2_17_23_3_12_323,(uint8*)"d",fun_flg);
            return AUDIO_HAL_ERROR;
        }
        else
        {
            __vbc_arm_acc_switch(1);
        }
    }
    
    if( (s_vbc.opened.fm_da) )
    {//opened fm play. Can't used for dsp.
        if( (AUDIO_FUN_DSP & fun_flg) )
        {
            //VBC_PRT:"[AUDDRV][VBC]fm da , can't dsp,%08x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_824_112_2_17_23_3_12_324,(uint8*)"d",fun_flg);
            return AUDIO_HAL_ERROR;
        }
    }
    
    if( (s_vbc.opened.fm_ad) )
    {//opened fm capture. Can't used for dsp and only ADC.
        if( (AUDIO_FUN_DSP & fun_flg) )
        {
            //VBC_PRT:"[AUDDRV][VBC]fm ad, can't dsp,%08x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_833_112_2_17_23_3_12_325,(uint8*)"d",fun_flg);
            return AUDIO_HAL_ERROR;
        }
        else if( (!(~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE) & fun_flg))
                && (AUDIO_FUN_CAPTRUE & fun_flg)
        )
        {
            //VBC_PRT:"[AUDDRV][VBC]fm ad , can't ad,%08x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_840_112_2_17_23_3_12_326,(uint8*)"d",fun_flg);
            return AUDIO_HAL_ERROR;
        }
    }
    return ret;
}

LOCAL uint32 _VBC_Parse_Ext_Action(uint32 fun_vbc,uint32 oc)
{
    uint32 rtx = 0;
    //VBC_DBG:"[AUDDRV][VBC]ext action 0x%x,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_850_112_2_17_23_3_12_327,(uint8*)"dd",fun_vbc,oc);
    if( (VBC_CTL_BY_DSP & fun_vbc) )
    {
        s_vbc.opened.dsp = oc;
        if(VBC_CLOSE == oc)
        {
            __vbc_arm_acc_switch(1);
        }
    }
    if( ((VBC_DAC) & fun_vbc) )//1
    {
        if( VBC_OPEN==oc )
        {
            __vbc_open();
        }
        __vbc_dac_fm_mix(DAPATH_NO_MIX,AUDIO_ALL_CHAN);
        __vbc_adc_sel_iis(IIS_PORT_NORMAL);
        rtx = 1;
    }
    if( ((VBC_ADC) & fun_vbc) )//2
    {
        if( VBC_OPEN==oc )
        {
            __vbc_open();
        }
        __vbc_adc_dgmux(ADPATH_NORMAL,AUDIO_ALL_CHAN);
        rtx = 1;
    }
    if( (VBC_FM_DA & fun_vbc) )//3
    {
        if( VBC_OPEN==oc )
        {
            __vbc_open();
            if( (VBC_USE_AD & fun_vbc) )
            {
                __vbc_adc_sel_iis(IIS_PORT_NORMAL);
            }
            else
            {
                __vbc_adc_sel_iis(IIS_PORT_FM);
            }
            __vbc_dac_fm_mix(DAPATH_ADD,AUDIO_ALL_CHAN);
        }
        else
        {
            __vbc_dac_fm_mix(DAPATH_NO_MIX,AUDIO_ALL_CHAN);
            __vbc_adc_sel_iis(IIS_PORT_NORMAL);
        }
        s_vbc.opened.fm_da = oc;
        _VBC_Parse_Base_Action(AUDIO_FUN_CAPTRUE,oc,0);
    }
    if( (VBC_FM_AD & fun_vbc) )//4
    {
        if( VBC_OPEN==oc )
        {
            __vbc_open();
            __vbc_adc_dgmux(ADPATH_NORMAL,AUDIO_ALL_CHAN);
        }
        else
        {
            __vbc_adc_dgmux(ADPATH_NORMAL,AUDIO_ALL_CHAN);
        }
        s_vbc.opened.fm_ad = oc;
    }
    return rtx;
}

LOCAL uint32 _VBC_Fun_Flg_Is_Eq(uint32 src_flg,uint32 dst_flg)
{
    uint32 ret_val = 0;
    uint32 com_flg = src_flg & dst_flg;
    if( com_flg &&
        ((~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE) & com_flg) ==
        (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE) & src_flg))
    )
    {
        if( (AUDIO_FUN_PLAYBACK & src_flg) )
        {
            if( ((AUDIO_FUN_PLAYBACK) & com_flg ) )
            {
                ret_val = 1;
            }
        }
        if( (AUDIO_FUN_CAPTRUE & src_flg) )
        {
            if( ((AUDIO_FUN_CAPTRUE) & com_flg ) )
            {
                ret_val = 1;
            }
        }
    }
    return ret_val;
}

LOCAL int32 _VBC_Parse_Fun_Flg(uint32 fun_flg,uint32 oc)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 i;
    uint32 rtx;
    uint32 fun_deal = (fun_flg);
    
    if( AUDIO_HAL_SUCCESS != _VBC_Parse_Fun_Conflict(fun_deal,oc) )
    {
        return AUDIO_HAL_ERROR;
    }
    
    if( (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)
        & fun_deal)    )
    {
        VBC_DBG_ASS(((AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)& fun_deal));/*assert verified*/
        for(i=0;i<s_vbc_support_fun_len;i++)
        {
            if( _VBC_Fun_Flg_Is_Eq((s_vbc_support_fun[i].fun_flg),fun_flg) )
            {
                rtx = 0;
                if( s_vbc_support_fun[i].fun_vbc )
                {
                    rtx =_VBC_Parse_Ext_Action(s_vbc_support_fun[i].fun_vbc,oc);
                    _VBC_Parse_Base_Action(((s_vbc_support_fun[i].fun_flg)&(fun_flg)),oc,rtx);
                }
            }
        }
    }
    else
    {
        _VBC_Parse_Base_Action(fun_flg,oc,1);
    }
    
    return ret;
}

LOCAL uint32 _VBC_Can_AD_Close(void)
{
    if( (s_vbc.opened.adc0  )
     || (s_vbc.opened.adc1  )
    )
    {
        return 0;
    }
    return 1;
}

LOCAL uint32 _VBC_Can_DA_Close(void)
{
    if( (s_vbc.opened.dac0  )
     || (s_vbc.opened.dac1  )
    )
    {
        return 0;
    }
    return 1;
}

LOCAL uint32 _VBC_Can_Close(void)
{
    return (_VBC_Can_AD_Close() && _VBC_Can_DA_Close());
}

LOCAL uint32 _VBC_Need_Open(void)
{
    return !(_VBC_Can_Close());
}

LOCAL void _VBC_DMA_Init(void)
{
    s_vbc.dma_L[AUDIO_PLAYBACK].chan_id    = VBC_DMA_NO_CHN_VAL;
    s_vbc.dma_L[AUDIO_CAPTRUE].chan_id     = VBC_DMA_NO_CHN_VAL;
    s_vbc.dma_R[AUDIO_PLAYBACK].chan_id    = VBC_DMA_NO_CHN_VAL;
    s_vbc.dma_R[AUDIO_CAPTRUE].chan_id     = VBC_DMA_NO_CHN_VAL;

    __vbc_dma_da_chan_cfg(&(s_vbc.dma_L[AUDIO_PLAYBACK].chan_cfg),AUDIO_CHAN_L);
    __vbc_dma_da_chan_cfg(&(s_vbc.dma_R[AUDIO_PLAYBACK].chan_cfg),AUDIO_CHAN_R);
   
    __vbc_dma_ad_chan_cfg(&(s_vbc.dma_L[AUDIO_CAPTRUE].chan_cfg),AUDIO_CHAN_L);
    __vbc_dma_ad_chan_cfg(&(s_vbc.dma_R[AUDIO_CAPTRUE].chan_cfg),AUDIO_CHAN_R);
}

/*****************************************************************************/
// Description :    VBC Init.
//                  this is DAI Init etc.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _VBC_Init(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //VBC_DBG:"[AUDDRV][VBC]init vbc"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1002_112_2_17_23_3_12_328,(uint8*)"");
    if( AUDIO_MAGIC_NUM != s_vbc.magic_num )
    {
        SCI_MEMSET(&s_vbc,0,sizeof(s_vbc));
        s_vbc.magic_num = AUDIO_MAGIC_NUM;
        SCI_MEMCPY(s_vbc.magic_str,AUDIO_MAGIC_STR,AUDIO_MAGIC_STR_MAX_LEN);
        s_vbc.support_data_fmt = VB_SUPPORT_DATA_FORMAT;
        s_vbc.data_fmt[AUDIO_PLAYBACK] = VB_DA_DATA_FORMAT;
        s_vbc.data_fmt[AUDIO_CAPTRUE] = VB_AD_DATA_FORMAT;
        s_vbc.buffer_div[AUDIO_PLAYBACK] = AUDIO_PLAYBACK_BUFFER_DIV;
        s_vbc.buffer_div[AUDIO_CAPTRUE] = AUDIO_CAPTRUE_BUFFER_DIV;
        
        // Get audio device handle
        if( CLOCK_SUCCESS != DEVICE_Find ("AUDIO", &(s_vbc.dev_audio)))
        {
            ret = AUDIO_HAL_ERROR;
            VBC_DBG_ASS(0 && "Get audio device handle Failed!");/*assert verified*/
        }

        _VBC_DMA_Init();

        __vbc_init();

        //just for pclint
        s_vbc_t_buf[0] = s_vbc_z_buf[0];
        s_vbc_t_buf[1] = s_vbc_t_buf[0];
    }
    return ret;
}


/*****************************************************************************/
// Description :    VBC  Open.
//                  this is DAI Enable.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _VBC_Open(uint32 fun_flg,uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]open vbc 0x%x,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1044_112_2_17_23_3_12_329,(uint8*)"dd",fun_flg,fs);
    
    if( AUDIO_HAL_SUCCESS != _VBC_Parse_Fun_Flg(fun_flg,VBC_OPEN) )
    {
        //VBC_PRT:"[AUDDRV][VBC]open err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1048_112_2_17_23_3_12_330,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }
    if( (_VBC_Need_Open()) && (0==s_vbc.opened.vbc_open) )
    {
        //VBC_DBG:"[AUDDRV][VBC]real open vbc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1053_112_2_17_23_3_12_331,(uint8*)"");
        
        SCI_AUD_EnableDeepSleep(DISABLE_DEEP_SLEEP);
        
        __vbc_open();

        // Open audio device
        DEVICE_Open(s_vbc.dev_audio);
        
        _VBC_DMA_Open();
        s_vbc.opened.vbc_open   = 1;

        if( !((fun_flg & AUDIO_FUN_INT_FM) || (fun_flg & AUDIO_FUN_LINEIN_DIGITAL)) )
        {
            //sometime buffer is full.
            _VBC_DMA_Auto_Trigger();
        }
    }
    
    return ret;
}


/*****************************************************************************/
// Description :    VBC  Close.
//                  this is  DAI Disable.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _VBC_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]close vbc 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1083_112_2_17_23_3_12_332,(uint8*)"d",fun_flg);


    if( AUDIO_HAL_SUCCESS != _VBC_Parse_Fun_Flg(fun_flg,VBC_CLOSE) )
    {
        //VBC_PRT:"[AUDDRV][VBC]close err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1088_112_2_17_23_3_12_333,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }
    
    if( _VBC_Can_AD_Close() || ( s_vbc.opened.fm_da && !s_vbc.opened.fm_ad ))
    {
        _VBC_DMA_AD_Disable();
        s_vbc.opened.ad_trigger = 0;
    }
    if( _VBC_Can_DA_Close() )
    {
        _VBC_DMA_DA_Disable();
        s_vbc.opened.da_trigger = 0;
        s_vbc.opened.cus_da_trigger = 0;
        __vbc_clear_da_buffer();
    }
    
    if( (s_vbc.opened.vbc_open) && (_VBC_Can_Close()) )
    {
        //VBC_DBG:"[AUDDRV][VBC]real close vbc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1105_112_2_17_23_3_12_334,(uint8*)"");
        __vbc_close();

        _VBC_DMA_Close();
        
        SCI_MEMSET(&(s_vbc.opened),0,sizeof(s_vbc.opened));
    }
    
    return ret;
}
LOCAL int32 _VBC_Init_Buf_Max_Cnt(uint32 path);


/*****************************************************************************/
// Description :    VBC  Ioctl.
//                  this is  DAI Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _VBC_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_ARG_U * ioctl_arg = (AUDIO_CTL_ARG_U *)arg;
    
    //VBC_DBG:"[AUDDRV][VBC]ioctl vbc %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1131_112_2_17_23_3_12_335,(uint8*)"dd",cmd,arg);
    switch(cmd)
    {
        case AUDIO_GET_SUPPORT_DATA_FMT:
        {
            ioctl_arg->dwVal = s_vbc.support_data_fmt;
        }break;
        case AUDIO_CTL_SET_DATA_FMT:
        {
            if( AUDIO_PLAYBACK==ioctl_arg->dat_fmt.dir )
            {
                if( (ioctl_arg->dat_fmt.fmt) & s_vbc.support_data_fmt )
                {
                    if( s_vbc.opened.da_trigger )
                    {
                        //VBC_PRT:"[AUDDRV][VBC]da used"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1146_112_2_17_23_3_12_336,(uint8*)"");
                        ret = AUDIO_HAL_ERROR;
                    }
                    else
                    {
                        s_vbc.data_fmt[AUDIO_PLAYBACK] = (ioctl_arg->dat_fmt.fmt);
                    }
                }
                else
                {
                    //VBC_PRT:"[AUDDRV][VBC]da df err"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1156_112_2_17_23_3_12_337,(uint8*)"");
                    ret = AUDIO_HAL_ERROR;
                }
            }
            else if( AUDIO_CAPTRUE==ioctl_arg->dat_fmt.dir )
            {
                if( (ioctl_arg->dat_fmt.fmt) & s_vbc.support_data_fmt )
                {
                    if( s_vbc.opened.ad_trigger )
                    {
                        //VBC_PRT:"[AUDDRV][VBC]ad used"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1166_112_2_17_23_3_12_338,(uint8*)"");
                        ret = AUDIO_HAL_ERROR;
                    }
                    else
                    {
                        s_vbc.data_fmt[AUDIO_CAPTRUE] = (ioctl_arg->dat_fmt.fmt);
                    }
                }
                else
                {
                    //VBC_PRT:"[AUDDRV][VBC]ad df err"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1176_112_2_17_23_3_12_339,(uint8*)"");
                    ret = AUDIO_HAL_ERROR;
                }
            }
            else
            {
                //VBC_PRT:"[AUDDRV][VBC]set:dir err"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1182_112_2_17_23_3_12_340,(uint8*)"");
                ret = AUDIO_HAL_ERROR;
            }
        }break;
        case AUDIO_CTL_GET_DATA_FMT:
        {
            if( AUDIO_PLAYBACK==ioctl_arg->dat_fmt.dir )
            {
                ioctl_arg->dat_fmt.fmt = s_vbc.data_fmt[AUDIO_PLAYBACK];
            }
            else if( AUDIO_CAPTRUE==ioctl_arg->dat_fmt.dir )
            {
                ioctl_arg->dat_fmt.fmt = s_vbc.data_fmt[AUDIO_CAPTRUE];
            }
            else
            {
                //VBC_PRT:"[AUDDRV][VBC]get:dir err"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1198_112_2_17_23_3_12_341,(uint8*)"");
                ret = AUDIO_HAL_ERROR;
            }
        }break;
        case AUDIO_CTL_DAI_BY_DSP:
        {
            __vbc_disable();
            __vbc_arm_acc_switch(0);
            
        }break;
        case AUDIO_CTL_DAI_BY_ARM:
        {
            __vbc_arm_acc_switch(1);
        }break;
        case AUDIO_CTL_SET_BUFFER_DIV:
        {
            //VBC_PRT:"[AUDDRV][VBC]set da buffer div:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1213_112_2_17_23_3_12_342,(uint8*)"d",arg);
            s_vbc.buffer_div[AUDIO_PLAYBACK] = arg;
            _VBC_Init_Buf_Max_Cnt(AUDIO_PLAYBACK);
        }break;
        case AUDIO_CTL_SET_AD_BUFFER_DIV:
        {
            //VBC_PRT:"[AUDDRV][VBC]set ad buffer div:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1219_112_2_17_23_3_13_343,(uint8*)"d",arg);
            s_vbc.buffer_div[AUDIO_CAPTRUE] = arg;
            _VBC_Init_Buf_Max_Cnt(AUDIO_CAPTRUE);
        }break;
        default:
        {
            ret = AUDIO_HAL_ERROR;
        }break;
    }
    return ret;
}

LOCAL int32 _VBC_Init_Buf_Max_Cnt(uint32 path)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( s_vbc.buf[path].size )
    {
        s_vbc.buf[path].max_cnt = s_vbc.buf[path].size/(s_vbc.dma_L[path].chan_cfg.block_length);
        s_vbc.buf[path].max_cnt /= s_vbc.buffer_div[path];
        s_vbc.buf[path].max_cnt *= (s_vbc.dma_L[path].chan_cfg.block_length);
    #if defined (AUDIO_PLAYBACK_MAX_COUNT)
    #if (AUDIO_PLAYBACK_MAX_COUNT!=0)
        if( (s_vbc.buf[path].max_cnt) > AUDIO_PLAYBACK_MAX_COUNT )
        {
            s_vbc.buf[path].max_cnt = AUDIO_PLAYBACK_MAX_COUNT;
        }
    #endif
    #endif
        if( s_vbc.buf[path].size < (s_vbc.dma_L[path].chan_cfg.block_length) )
        {
            (s_vbc.dma_L[path].chan_cfg.block_length) = s_vbc.buf[path].size;
            (s_vbc.dma_R[path].chan_cfg.block_length) = s_vbc.buf[path].size;
            s_vbc.buf[path].max_cnt = s_vbc.buf[path].size;
        }
        else if( (s_vbc.buf[path].max_cnt) < (s_vbc.dma_L[path].chan_cfg.block_length) )
        {
            (s_vbc.buf[path].max_cnt) = (s_vbc.dma_L[path].chan_cfg.block_length);
        }
    }
    return ret;
}

LOCAL int32 _VBC_Init_Buf_Info(uint32 path,void *pLHead,void *pRHead,uint32 size)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_VBC_IRQ_ENTRY();
    s_vbc.buf[path].pLHead  = (uint32)pLHead;
    s_vbc.buf[path].pRHead  = (uint32)pRHead;
    s_vbc.buf[path].size    = size;
    s_vbc.buf[path].pWrite  = 0;
    s_vbc.buf[path].pRead   = 0;
    s_vbc.buf[path].cnt     = 0;
    AUDIO_VBC_IRQ_LEAVE();
    VBC_DBG_ASS(0==(size%(s_vbc.dma_L[path].chan_cfg.block_length)));/*assert verified*/
    _VBC_Init_Buf_Max_Cnt(path);
    s_vbc.save_pos[path] = 0;
    return ret;
}

/*****************************************************************************/
// Description :    VBC  Set Playback Buffer Info.
//                  
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO 
//                      , will ignore.
//                  size : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _VBC_DAC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 size)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]set dac buffer 0x%x,0x%x,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1297_112_2_17_23_3_13_344,(uint8*)"ddd",pLHead,pRHead,size);
    ret = _VBC_Init_Buf_Info(AUDIO_PLAYBACK,pLHead,pRHead,size);
    
    return ret;
}

LOCAL int32 _VBC_DAC_Set_Write_Point(uint32 pWrite)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //VBC_DBG("[AUDDRV][VBC]write %d",pWrite);
    if( pWrite < s_vbc.buf[AUDIO_PLAYBACK].size )
    {
        AUDIO_VBC_IRQ_ENTRY();
        s_vbc.buf[AUDIO_PLAYBACK].pWrite  = pWrite;
        s_vbc.buf[AUDIO_PLAYBACK].cnt = _VBC_Get_Len(&(s_vbc.buf[AUDIO_PLAYBACK]),1);
        AUDIO_VBC_IRQ_LEAVE();
        _VBC_DMA_Auto_Trigger();
    }
    else
    {
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}

LOCAL int32 _VBC_DAC_Set_Read_Point(uint32 pRead)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_VBC_IRQ_ENTRY();
    if(pRead == s_vbc.buf[AUDIO_PLAYBACK].size )
    {
        pRead = 0;
    }
    s_vbc.buf[AUDIO_PLAYBACK].pRead  = pRead;
    s_vbc.buf[AUDIO_PLAYBACK].cnt = _VBC_Get_Len(&(s_vbc.buf[AUDIO_PLAYBACK]),0);
    AUDIO_VBC_IRQ_LEAVE();
    if( s_vbc.cb[AUDIO_PLAYBACK] )
    {
        s_vbc.cb[AUDIO_PLAYBACK](0);
    }
    return ret;
}

LOCAL int32 _VBC_DAC_Get_Read_Point(uint32* pRead)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( pRead )
    {
        AUDIO_VBC_IRQ_ENTRY();
        *pRead  = s_vbc.buf[AUDIO_PLAYBACK].pRead;
        AUDIO_VBC_IRQ_LEAVE();
    }
    return ret;
}

/*****************************************************************************/
// Description :    VBC  Will Trigger Playback immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _VBC_DAC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG("[AUDDRV][VBC]customer trigger");
    if( s_vbc.opened.da_trigger )
    {
        //AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DAC,AUDIO_MUTE);
    }
    else
    {
        s_vbc.opened.cus_da_trigger = 1;
        if( (1==s_vbc.opened.vbc_open) )
        {
            ret = _VBC_DMA_DAC_Real_Trigger();
        }
        else
        {
            s_vbc.opened.cus_da_trigger = 0;
            //VBC_PRT:"[AUDDRV][VBC][REF]why trigger?"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1378_112_2_17_23_3_13_345,(uint8*)"");
        }
    }
    
    return ret;
}


/*****************************************************************************/
// Description :    VBC  Capture Buffer Info.
//                  
// Return :         AUDIO_HAL_SUCCESS
// Param :          pLHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Left channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO
//                      , the data start point.
//                  pRHead : if AUDIO_DATA_FMT_LL_RR , 
//                      the Right channel data start point.
//                      if AUDIO_DATA_FMT_LR_LR or AUDIO_DATA_FMT_MONO 
//                      , will ignore.
//                  size : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _VBC_ADC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 size)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]set adc buffer 0x%x,0x%x,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1405_112_2_17_23_3_13_346,(uint8*)"ddd",pLHead,pRHead,size);
    ret = _VBC_Init_Buf_Info(AUDIO_CAPTRUE,pLHead,pRHead,size);
    
    return ret;
}

LOCAL int32 _VBC_ADC_Set_Read_Point(uint32 pRead)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //VBC_DBG("[AUDDRV][VBC]read %d",pRead);
    AUDIO_VBC_IRQ_ENTRY();
    s_vbc.buf[AUDIO_CAPTRUE].pRead  = pRead;
    s_vbc.buf[AUDIO_CAPTRUE].cnt = _VBC_Get_Len(&(s_vbc.buf[AUDIO_CAPTRUE]),0);
    AUDIO_VBC_IRQ_LEAVE();
    return ret;
}

LOCAL int32 _VBC_ADC_Set_Write_Point(uint32 pWrite)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //VBC_DBG("[AUDDRV][VBC]write %d",pWrite);
    AUDIO_VBC_IRQ_ENTRY();
    if(pWrite == s_vbc.buf[AUDIO_CAPTRUE].size )
    {
        pWrite = 0;
    }
    s_vbc.buf[AUDIO_CAPTRUE].pWrite  = pWrite;
    s_vbc.buf[AUDIO_CAPTRUE].cnt = _VBC_Get_Len(&(s_vbc.buf[AUDIO_CAPTRUE]),1);
    AUDIO_VBC_IRQ_LEAVE();
    if( s_vbc.cb[AUDIO_CAPTRUE] )
    {
        s_vbc.cb[AUDIO_CAPTRUE](0);
    }
    return ret;
}

LOCAL int32 _VBC_ADC_Get_Write_Point(uint32* pWrite)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( pWrite )
    {
        AUDIO_VBC_IRQ_ENTRY();
        *pWrite  = s_vbc.buf[AUDIO_CAPTRUE].pWrite;
        AUDIO_VBC_IRQ_LEAVE();
    }
    return ret;
}

/*****************************************************************************/
// Description :    VBC  Will Trigger Captrue immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _VBC_ADC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]adc trigger"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1464_112_2_17_23_3_13_347,(uint8*)"");
    if( 0==s_vbc.opened.ad_trigger )
    {
        ret = _VBC_DMA_ADC_Real_Trigger();
    }
    
    return ret;
}


/*****************************************************************************/
// Description :    VBC  Register Playback Callback.
//                  Use to Notify User the Driver fetch data gone.
//                  User can do something like start decode audio data.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _VBC_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]register dac CB 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1487_112_2_17_23_3_13_348,(uint8*)"d",pFun);
    s_vbc.cb[AUDIO_PLAYBACK]  = pFun;
    
    return ret;
}


/*****************************************************************************/
// Description :    VBC  Register Capture Callback.
//                  Use to Notify User for fetch data please.
//                  User should be move audio data ASAP.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _VBC_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]register adc CB 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1507_112_2_17_23_3_13_349,(uint8*)"d",pFun);
    s_vbc.cb[AUDIO_CAPTRUE]  = pFun;
    
    return ret;
}

LOCAL int32 _VBC_Codec_Notify(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    //VBC_DBG:"[AUDDRV][VBC]codec notify %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1517_112_2_17_23_3_13_350,(uint8*)"d",param);
    switch(param)
    {
        case AUDIO_HAL_CODEC_OPENED:
        {
            if( s_vbc.opened.dsp )
            {
                //VBC_DBG:"[AUDDRV][VBC]switch to dsp"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1524_112_2_17_23_3_13_351,(uint8*)"");
                __vbc_dma_da_chn_en(0,AUDIO_ALL_CHAN);
                __vbc_dma_ad_chn_en(0,AUDIO_ALL_CHAN);
                __vbc_disable();
                __vbc_arm_acc_switch(0);
            }
            else
            {
                if( !_VBC_Can_AD_Close()  )
                {
                    if( !s_vbc.opened.fm_da || s_vbc.opened.fm_ad )
                    {
                        _VBC_ADC_Trigger();
                    }
                    else
                    {
                        __vbc_enable();
                    }
                }
            }
            AUDIO_HAL_TICK_DBG("[AUDTICK] opened %d",SCI_GetTickCount());
        }break;
        case AUDIO_HAL_CODEC_CLOSEED:
        {
            if( (_VBC_Can_Close()) )
            {
                //VBC_DBG:"[AUDDRV][VBC]close all"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1542_112_2_17_23_3_13_352,(uint8*)"");

                // Close audio device
                DEVICE_Close(s_vbc.dev_audio);

                SCI_AUD_EnableDeepSleep(ENABLE_DEEP_SLEEP);
                
            }
            AUDIO_HAL_TICK_DBG("[AUDTICK] closed %d",SCI_GetTickCount());
        }break;
        case AUDIO_HAL_CODEC_SEND_READY:
        {
            s_vbc.opened.send_ready = 1;
        }break;
        case AUDIO_HAL_CODEC_SEND_Z:
        {
            s_vbc.opened.send_ready = 0;
        }break;
        default:
        {
            //VBC_PRT:"[AUDDRV][VBC]codec notfiy err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_DAI_VBC_1561_112_2_17_23_3_13_353,(uint8*)"");
        }break;
    }
    
    return ret;
}
LOCAL int32 _vbc_notch_en(uint32 en,uint32 fs)
{
    _vbc_notch_enable(en,fs);
    return AUDIO_HAL_SUCCESS;
}

LOCAL void _vbc_notch_setpara(NothPara* para)
{
    _vbc_set_notchpara(para);
}

/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL const AUD_DAI_OPS_T s_audio_dai_vbc_ops=
{
    _VBC_Init,
    _VBC_Open,
    _VBC_Close,
    _VBC_Ioctl,
    _VBC_DAC_Set_Buf_Info,
    _VBC_DAC_Set_Write_Point,
    _VBC_DAC_Get_Read_Point,
    _VBC_DAC_Trigger,
    _VBC_ADC_Set_Buf_Info,
    _VBC_ADC_Set_Read_Point,
    _VBC_ADC_Get_Write_Point,
    _VBC_ADC_Trigger,
    _VBC_Register_Playback,
    _VBC_Register_Capture,
    _VBC_Codec_Notify,
    _vbc_notch_en,
    _vbc_notch_setpara,
};

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_DAI_OPS_T* AUDIO_DAI_Get_VBC_OPS(uint32 param)
{
    return (AUD_DAI_OPS_T*)(&(s_audio_dai_vbc_ops));
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
// End
