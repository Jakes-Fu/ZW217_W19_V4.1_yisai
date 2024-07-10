/******************************************************************************
 ** File Name:      audio_codec_sprd.c                                        *
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
#include "chip_plf_export.h"

#include "audio_drvapi.h"
#include "audio_codec.h"
#include "sprd_codec_phy_cfg.h"
#include "sprd_codec_state.h"
#include "sprd_codec_phy.h"

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
#ifdef CODEC_DEBUG
#define SPRD_CODEC_DBG       SCI_TRACE_LOW
#define SPRD_CODEC_DBG_ASS   SCI_ASSERT
#else
#define SPRD_CODEC_DBG(...)
#define SPRD_CODEC_DBG_ASS(x)
#endif

#define SPRD_CODEC_PRT       SCI_TRACE_LOW

#define SPRD_CODEC_ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

enum SPRD_CODEC_OC_E{
    SPRD_CODEC_OPEN    = 0,
    SPRD_CODEC_CLOSE
};

typedef struct
{
    uint32                  fun_flg;
    uint32                  fun_sprd_codec;
} SPRD_CODEC_FUN_T;

typedef struct
{
    uint32                  is_on;
    uint32                  chan;
    uint32                  is_mute;
    uint32                  is_next_enabled;
}SPRD_CODEC_MODE_T;

typedef struct
{
    SPRD_CODEC_MODE_T       ep;
    SPRD_CODEC_MODE_T       hp;
    SPRD_CODEC_MODE_T       spk[2];
} SPRD_CODEC_PATH_DAC_T;

typedef struct
{
    SPRD_CODEC_MODE_T       mic[2];
    SPRD_CODEC_MODE_T       hp_mic;
    SPRD_CODEC_MODE_T       ai;
} SPRD_CODEC_PATH_ADC_T;

typedef struct
{
    SPRD_CODEC_PATH_DAC_T   dac;
    SPRD_CODEC_PATH_ADC_T   adc;
} SPRD_CODEC_PATH_T;

typedef struct
{
    uint                    is_classD:1;
    uint                    is_LDO:1;
    uint                    is_DEMI:1;
    uint                    is_auto_LDO:1;
    uint                    LDO_V_sel:4;
    uint                    DTRI_F_sel:3;
    uint                    RESV:21;
}SPRD_CODEC_PA_T;

typedef struct
{
    char                    magic_str[AUDIO_MAGIC_STR_MAX_LEN];
    uint32                  da_sample_rate;
    uint32                  ad_sample_rate;

    SPRD_CODEC_PGA_T        pga_0;
    SPRD_CODEC_PGA_T        pga_1;
    SPRD_CODEC_PATH_T       path;

    uint32                  is_dsp_ctl;

    uint32                  cur_vol;

    SPRD_CODEC_PA_T         pa;

#if (PGA_SMOOTH_ADJUST)
    SCI_TIMER_PTR           timer;
    SCI_MUTEX_PTR           mutex;
    BLOCK_ID                task_id;
    uint32                  adjusting;
    SPRD_CODEC_PGA_T        pga_adj_0;
    SPRD_CODEC_PGA_T        pga_adj_1;
#endif

    uint32                  magic_num;
} SPRD_CODEC_T;

#if (PGA_SMOOTH_ADJUST)
#include "priority_system.h"

#define SPRD_CODEC_PGA_ADJ_TASK_NAME           "PGA_ADJ_TASK"
#define SPRD_CODEC_PGA_ADJ_QNAME_NAME          "Q_PGA_ADJ_TASK"
#define SPRD_CODEC_PGA_ADJ_STACK_SIZE          (2*1024)
#define SPRD_CODEC_PGA_ADJ_QUEUE_NUM           (4*30) //ken.kuang: PGA is very much MSG.
#define SPRD_CODEC_PGA_ADJ_TASK_PRIV           (PRI_CODEC_DRV_TASK) //ken.kuang: this pri < AUDIO task pri.


/*
 * TASK MSG.
*/
typedef enum {
    SPRD_CODEC_PGA_SETTING  = 0x1,
    SPRD_CODEC_PGA_ADJ      = 0x2,
    SPRD_CODEC_PGA_MAX_MSG
} SPRD_CODEC_PGA_TASK_MSG_E;

#endif


/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL SPRD_CODEC_T s_sprd_codec = {0};
LOCAL const SPRD_CODEC_FUN_T s_sprd_codec_support_fun[]=
{
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_PLAYBACK))
                                ,SPRD_CODEC_FUN_PLAYBACK   },
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_CAPTRUE))
                                ,SPRD_CODEC_FUN_CAPTURE    },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_PLAYBACK))
                                ,SPRD_CODEC_FUN_LINEIN     },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_CAPTRUE))
                                ,SPRD_CODEC_FUN_LINEIN_REC },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_PLAYBACK))
                                ,SPRD_CODEC_FUN_PLAYBACK   },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_CAPTRUE))
                                ,0                         },
    {((uint32)(AUDIO_FUN_AD_FROM_DA|AUDIO_FUN_PLAYBACK))
                                ,SPRD_CODEC_FUN_PLAYBACK   },
    {((uint32)(AUDIO_FUN_AD_FROM_DA|AUDIO_FUN_CAPTRUE))
                                ,SPRD_CODEC_FUN_CAPTURE    },
    {((uint32)(AUDIO_FUN_LINEIN_DIGITAL|AUDIO_FUN_PLAYBACK))
                                , SPRD_CODEC_FUN_LININ_DIGTAL},
    {((uint32)(AUDIO_FUN_LINEIN_DIGITAL|AUDIO_FUN_CAPTRUE))
                                ,0                         },                                
};
LOCAL const uint32 s_sprd_codec_support_fun_len=SPRD_CODEC_ARR_SIZE(s_sprd_codec_support_fun);

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

LOCAL void _sprd_codec_update_pga(uint32 chan)
{
    if( AUDIO_IS_CHAN_L(chan) )
    {
        __sprd_codec_ap_pa_pga_gain(s_sprd_codec.pga_0.pa);
        __sprd_codec_ap_ear_pga_gain(s_sprd_codec.pga_0.ear);
        __sprd_codec_ap_hp_pga_gain(AUDIO_CHAN_L,s_sprd_codec.pga_0.hp);
        __sprd_codec_ap_adc_pga_gain(AUDIO_CHAN_L,s_sprd_codec.pga_0.adcpga);
    }
    if( AUDIO_IS_CHAN_R(chan) )
    {
        __sprd_codec_ap_hp_pga_gain(AUDIO_CHAN_R,s_sprd_codec.pga_1.hp);
        __sprd_codec_ap_adc_pga_gain(AUDIO_CHAN_R,s_sprd_codec.pga_1.adcpga);
    }
}

LOCAL void _sprd_codec_update_sw()
{
    if( __sprd_codec_is_use_ext_hp() || !(s_sprd_codec.path.dac.hp.is_mute) )
    {
        __sprd_codec_hp_mute((uint32)(!(s_sprd_codec.path.dac.hp.is_on)));
    }
    __sprd_codec_ep_mute((uint32)(!(s_sprd_codec.path.dac.ep.is_on)));
    __sprd_codec_pa_mute((uint32)(!(s_sprd_codec.path.dac.spk[0].is_on)));
    
    __sprd_codec_ap_main_mic(s_sprd_codec.path.adc.mic[0].is_on,s_sprd_codec.path.adc.mic[0].chan);
    __sprd_codec_ap_aux_mic(s_sprd_codec.path.adc.mic[1].is_on,s_sprd_codec.path.adc.mic[1].chan);
    __sprd_codec_ap_hp_mic(s_sprd_codec.path.adc.hp_mic.is_on,s_sprd_codec.path.adc.hp_mic.chan);
    __sprd_codec_ap_lin_adc(s_sprd_codec.path.adc.ai.is_on,s_sprd_codec.path.adc.ai.chan);
}


LOCAL uint32 _SPRD_CODEC_Fun_Flg_Is_Eq(uint32 src_flg,uint32 dst_flg)
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

LOCAL int32 _SPRD_CODEC_Parse_Fun_Flg(uint32 fun_flg,uint32 oc)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 i;
    uint32 fun_deal = (fun_flg);

    if( (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)
        & fun_deal)    )
    {
        for(i=0;i<s_sprd_codec_support_fun_len;i++)
        {
            if( _SPRD_CODEC_Fun_Flg_Is_Eq((s_sprd_codec_support_fun[i].fun_flg) , fun_flg) )
            {
                if( s_sprd_codec_support_fun[i].fun_sprd_codec )
                {
                    SPRD_CODEC_ChangeSt((s_sprd_codec_support_fun[i].fun_sprd_codec),oc);
                }
            }
        }
    }
    else
    {
        if( AUDIO_FUN_PLAYBACK & fun_deal)
        {
            SPRD_CODEC_ChangeSt(SPRD_CODEC_FUN_PLAYBACK,oc);
        }
        if( AUDIO_FUN_CAPTRUE & fun_deal)
        {
            SPRD_CODEC_ChangeSt(SPRD_CODEC_FUN_CAPTURE,oc);
        }
    }

    
    return ret;
}

LOCAL int32 _SPRD_CODEC_PA_Def_Setting(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    s_sprd_codec.pa.is_classD   = 0;            //classAB
    s_sprd_codec.pa.is_LDO      = 0;            //SW mode
    s_sprd_codec.pa.is_DEMI     = 0;            //disable DEMI
    s_sprd_codec.pa.is_auto_LDO = 0;            //disable auto Modify LDO
    s_sprd_codec.pa.LDO_V_sel   = PA_LDO_V_32;  // 3.2v
    s_sprd_codec.pa.DTRI_F_sel  = PA_DTRI_F_590;// 590k
    return ret;
}

/*****************************************************************************/
// Description :    SPRD_CODEC Init.
//                  this is Codec Init.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _SPRD_CODEC_Init(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]init sprd_codec"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_253_112_2_17_23_2_51_169,(uint8*)"");
    if( AUDIO_MAGIC_NUM != s_sprd_codec.magic_num )
    {
        SCI_MEMSET(&s_sprd_codec,0,sizeof(s_sprd_codec));
        s_sprd_codec.magic_num = AUDIO_MAGIC_NUM;
        SCI_MEMCPY(s_sprd_codec.magic_str,AUDIO_MAGIC_STR,AUDIO_MAGIC_STR_MAX_LEN);
        
        __sprd_codec_create_task();
#if (PGA_SMOOTH_ADJUST)
        __sprd_codec_pga_adj_create_task();
#endif
        __sprd_codec_init();
        SPRD_CODEC_PGA_Init_Set(param);
        _SPRD_CODEC_PA_Def_Setting();
    }
    return ret;
}

/*****************************************************************************/
// Description :    SPRD_CODEC Open.
//                  this Codec Power On.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _SPRD_CODEC_Open(uint32 fun_flg,uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]open sprd_codec 0x%x,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_278_112_2_17_23_2_51_170,(uint8*)"dd",fun_flg,fs);

    if( AUDIO_HAL_SUCCESS != _SPRD_CODEC_Parse_Fun_Flg(fun_flg,SPRD_CODEC_OPEN) )
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]open err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_282_112_2_17_23_2_51_171,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }

    if ((AUDIO_FUN_LINEIN_DIGITAL|AUDIO_FUN_PLAYBACK)==fun_flg)
    {
        fun_flg |= AUDIO_FUN_CAPTRUE;
    }
	
    SPRD_CODEC_Change_Chan(fun_flg,SPRD_CODEC_OPEN);
    
    if( AUDIO_FUN_PLAYBACK & fun_flg )
    {
        s_sprd_codec.da_sample_rate = fs;
    }
    if( AUDIO_FUN_CAPTRUE & fun_flg )
    {
        s_sprd_codec.ad_sample_rate = fs;
    }

    if( (SPRD_CODEC_CTRL_BY_DSP & fun_flg) )
    {
        s_sprd_codec.is_dsp_ctl = 1;
    }
    
    SPRD_CODEC_SM_Process();
    
    return ret;
}

/*****************************************************************************/
// Description :    SPRD_CODEC Close.
//                  this is Codec Power Off.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _SPRD_CODEC_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]close sprd_codec 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_316_112_2_17_23_2_51_172,(uint8*)"d",fun_flg);
    
    if( s_sprd_codec.is_dsp_ctl )
    {
        __sprd_codec_switch_to(1);
        s_sprd_codec.is_dsp_ctl = 0;
    }
    
    if( AUDIO_HAL_SUCCESS != _SPRD_CODEC_Parse_Fun_Flg(fun_flg,SPRD_CODEC_CLOSE) )
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]close err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_326_112_2_17_23_2_51_173,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }
    
    SPRD_CODEC_Change_Chan(fun_flg,SPRD_CODEC_CLOSE);
    
    if( AUDIO_FUN_PLAYBACK & fun_flg)
    {
        SCI_MEMSET(&(s_sprd_codec.path.dac),0,sizeof(s_sprd_codec.path.dac));
        if(( AUDIO_FUN_LINEIN & fun_flg) || ( AUDIO_FUN_LINEIN_DIGITAL & fun_flg))
        {
            SCI_MEMSET(&(s_sprd_codec.path.adc.ai),0,sizeof(s_sprd_codec.path.adc.ai));
        }
    }
    if(( AUDIO_FUN_LINEIN & fun_flg) )
    {
        SCI_MEMSET(&(s_sprd_codec.path.adc),0,sizeof(s_sprd_codec.path.adc));
    }

    SPRD_CODEC_SM_Process();
    
    return ret;
}

LOCAL int32 _SPRD_CODEC_Ioctl(uint32 cmd, uint32 arg);

LOCAL int32 _SPRD_CODEC_Set_Drv_out(AUDIO_CTL_PGA_T * pga,uint32 mute)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_SWITCH_T sw = {0};
    sw.id = pga->id;
    sw.chan = pga->chan;
    sw.mode = (pga->mode&(AUDIO_SPK|AUDIO_EAR|AUDIO_HP));
    sw.val = mute;
    ret = _SPRD_CODEC_Ioctl(AUDIO_CTL_SWITCH_DRV_OUT,(uint32)(&sw));
    return ret;
}

LOCAL int32 _SPRD_CODEC_Ioctl_PGA(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_PGA_T *pga = 0;
    SPRD_CODEC_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg)  && (ioctl_arg->pga.id<2)  ) //lint !e774
    {
        pga = &(ioctl_arg->pga);
        //if( (pga->level) )
        //{
            ret = SPRD_CODEC_PGA_Set(pga);
        //}
        //else
        //{
        //    _SPRD_CODEC_Set_Drv_out(pga,AUDIO_MUTE);
        //}
    }
    else
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]pga param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_368_112_2_17_23_2_51_174,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _SPRD_CODEC_Ioctl_Drv_Out_Ctl(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 is_all_on = 0;
    SPRD_CODEC_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg) && (ioctl_arg->pga.id<2) ) //lint !e774
    {
        //SPRD_CODEC_DBG:"[AUDDRV][SPRD]drv out ctl 0x%x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_382_112_2_17_23_2_52_175,(uint8*)"dd",ioctl_arg->sw.mode,ioctl_arg->sw.val);
        switch(ioctl_arg->sw.mode)
        {
            case AUDIO_SPK:
            {
                s_sprd_codec.path.dac.spk[ioctl_arg->sw.id].is_on = ioctl_arg->sw.val;  //1
                s_sprd_codec.path.dac.spk[ioctl_arg->sw.id].chan = ioctl_arg->sw.chan;
                is_all_on = (s_sprd_codec.path.dac.spk[0].is_on) || (s_sprd_codec.path.dac.spk[1].is_on);
                SPRD_CODEC_ChangeSt(SPRD_CODEC_FUN_LOUT,(uint32)(!(is_all_on)));
                SPRD_CODEC_SM_Process();
                //if( SPRD_CODEC_IsOnStop() )
                //{
                //    __sprd_codec_ext_pa_ctl(ioctl_arg->sw.val,ioctl_arg->sw.id);
                //}
            }break;
            case AUDIO_EAR:
            {
                s_sprd_codec.path.dac.ep.is_on = ioctl_arg->sw.val;   //1
                s_sprd_codec.path.dac.ep.chan = ioctl_arg->sw.chan;
                SPRD_CODEC_ChangeSt(SPRD_CODEC_FUN_EP,(uint32)(!(ioctl_arg->sw.val)));
                SPRD_CODEC_SM_Process();
            }break;
            case AUDIO_HP:
            {
                s_sprd_codec.path.dac.hp.is_on = ioctl_arg->sw.val;
                s_sprd_codec.path.dac.hp.chan = ioctl_arg->sw.chan;
                SPRD_CODEC_ChangeSt(SPRD_CODEC_FUN_HP,(uint32)(!(ioctl_arg->sw.val)));
                SPRD_CODEC_SM_Process();
                //if( SPRD_CODEC_IsOnStop() )
                //{
                //    __sprd_codec_ext_hp_pa_ctl(s_sprd_codec.path.dac.hp.is_on);
                //}
            }break;
            default:
            {
                //SPRD_CODEC_PRT:"[AUDDRV][SPRD]drv out 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_417_112_2_17_23_2_52_176,(uint8*)"d",ioctl_arg->sw.mode);
                ret = AUDIO_HAL_ERROR;
            }break;
        }
    }
    else
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]drv out param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_424_112_2_17_23_2_52_177,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _SPRD_CODEC_Ioctl_Drv_In_Ctl(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    SPRD_CODEC_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg) ) //lint !e774
    {
        //SPRD_CODEC_DBG:"[AUDDRV][SPRD]drv in ctl 0x%x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_437_112_2_17_23_2_52_178,(uint8*)"dd",ioctl_arg->sw.mode,ioctl_arg->sw.val);
        switch(ioctl_arg->sw.mode)
        {
            case AUDIO_MIC:
            {
                s_sprd_codec.path.adc.mic[ioctl_arg->sw.id].is_on = ioctl_arg->sw.val;
                if( 0==ioctl_arg->sw.id )
                {
                    ioctl_arg->sw.chan = AUDIO_CHAN_L;
                    if( SPRD_CODEC_IsOnStop() )
                    {
                        __sprd_codec_ap_main_mic(ioctl_arg->sw.val,ioctl_arg->sw.chan);
                    }
                }
                else if( 1==ioctl_arg->sw.id )
                {
                    ioctl_arg->sw.chan = AUDIO_CHAN_R;
                    if( SPRD_CODEC_IsOnStop() )
                    {
                        __sprd_codec_ap_aux_mic(ioctl_arg->sw.val,ioctl_arg->sw.chan);
                    }
                }
                s_sprd_codec.path.adc.mic[ioctl_arg->sw.id].chan = ioctl_arg->sw.chan;
            }break;
            case AUDIO_HP_MIC:
            {
                s_sprd_codec.path.adc.hp_mic.is_on = ioctl_arg->sw.val;
                s_sprd_codec.path.adc.hp_mic.chan = ioctl_arg->sw.chan;
                if( SPRD_CODEC_IsOnStop() )
                {
                    __sprd_codec_ap_hp_mic(ioctl_arg->sw.val,ioctl_arg->sw.chan);
                }
            }break;
            case AUDIO_LINEIN:
            {
                s_sprd_codec.path.adc.ai.is_on = ioctl_arg->sw.val;
                s_sprd_codec.path.adc.ai.chan = ioctl_arg->sw.chan;
                if( SPRD_CODEC_IsOnStop() )
                {
                    __sprd_codec_ap_lin_adc(ioctl_arg->sw.val,ioctl_arg->sw.chan);
                }
            }break;
            default:
            {
                //SPRD_CODEC_PRT:"[AUDDRV][SPRD]drv in 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_472_112_2_17_23_2_52_179,(uint8*)"d",ioctl_arg->sw.mode);
                ret = AUDIO_HAL_ERROR;
            }break;
        }
    }
    else
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]drv in param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_479_112_2_17_23_2_52_180,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _SPRD_CODEC_Ioctl_Dac_Mute(uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    SPRD_CODEC_DBG_ASS((arg==AUDIO_MUTE) || (arg==AUDIO_UNMUTE));/*assert verified*/
    if(arg==AUDIO_MUTE)
    {
        SPRD_CODEC_ChangeSt(SPRD_CODEC_FUN_DAC_UNMUTE,1);
        SPRD_CODEC_SM_Process();
    }
    else if(arg==AUDIO_UNMUTE)
    {
        SPRD_CODEC_ChangeSt(SPRD_CODEC_FUN_DAC_UNMUTE,0);
        SPRD_CODEC_SM_Process();
    }
    else
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]dac mute"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_502_112_2_17_23_2_52_181,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _SPRD_CODEC_Ioctl_Get_FS(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    SPRD_CODEC_DBG_ASS(ioctl_arg);/*assert verified*/
    if( ioctl_arg ) //lint !e774
    {
        ioctl_arg->dwVal = __sprd_codec_dp_dac_fs_mode_calc(ioctl_arg->dwVal,0);
    }
    else
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]get fs param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_519_112_2_17_23_2_52_182,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _SPRD_CODEC_Ioctl_Update_LDO_VOL(uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    s_sprd_codec.cur_vol = arg;
    if( (SPRD_CODEC_IsPwrOn()) )
    {
        __sprd_codec_update_ldo();
    }
    return ret;
}


LOCAL int32 _SPRD_CODEC_Ioctl_PA_Setting(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    SPRD_CODEC_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg)  && (0==ioctl_arg->sw.id) ) //lint !e774
    {
        s_sprd_codec.pa.is_classD   = ioctl_arg->PA_Setting.is_classD  ;
        s_sprd_codec.pa.is_LDO      = ioctl_arg->PA_Setting.is_LDO     ;
        s_sprd_codec.pa.is_DEMI     = ioctl_arg->PA_Setting.is_DEMI    ;
        s_sprd_codec.pa.is_auto_LDO = ioctl_arg->PA_Setting.is_auto_LDO;
        s_sprd_codec.pa.LDO_V_sel   = ioctl_arg->PA_Setting.LDO_V_sel  ;
        s_sprd_codec.pa.DTRI_F_sel  = ioctl_arg->PA_Setting.DTRI_F_sel ;
    }
    else
    {
        //SPRD_CODEC_PRT:"[AUDDRV][SPRD]get fs param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_550_112_2_17_23_2_52_183,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}

LOCAL int32 _SPRD_CODEC_Ioctl_HP_Mute(uint32 arg)
{
    int32 next_enabled = 0;
    int32 ret = AUDIO_HAL_SUCCESS;
    next_enabled = !!((uint32)AUDIO_NEXT_ENABLED & arg);
    arg &= ~AUDIO_NEXT_ENABLED;
    SPRD_CODEC_DBG_ASS((arg==AUDIO_MUTE) || (arg==AUDIO_UNMUTE));/*assert verified*/
    if((arg==AUDIO_MUTE) || (arg==AUDIO_UNMUTE))
    {
        s_sprd_codec.path.dac.hp.is_mute = (arg==AUDIO_UNMUTE)?0:1;
        s_sprd_codec.path.dac.hp.is_next_enabled = next_enabled;
        if( !next_enabled && SPRD_CODEC_IsOnStop() )
        {
            __sprd_codec_ext_hp_pa_ctl_inter((uint32)(!s_sprd_codec.path.dac.hp.is_mute));
            if( !__sprd_codec_is_use_ext_hp() )
            {
                if( !s_sprd_codec.path.dac.hp.is_mute )
                {
                    _sprd_codec_update_pga(AUDIO_ALL_CHAN);
                }
                __sprd_codec_hp_mute(s_sprd_codec.path.dac.hp.is_mute);
            }
        }
    }
    else
    {
        SPRD_CODEC_PRT("[AUDDRV][SPRD]hp unmute err");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


/*****************************************************************************/
// Description :    SPRD_CODEC Ioctl.
//                  this is Codec Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _SPRD_CODEC_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_ARG_U * ioctl_arg = (AUDIO_CTL_ARG_U *)arg;
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]ioctl sprd_codec %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_569_112_2_17_23_2_52_184,(uint8*)"dd",cmd,arg);

    switch(cmd)
    {
        case AUDIO_CTL_PGA:
        {
            ret = _SPRD_CODEC_Ioctl_PGA(ioctl_arg);
        }break;
        case AUDIO_CTL_SWITCH_DAC:
        {
            ret = _SPRD_CODEC_Ioctl_Dac_Mute(arg);
        }break;
        case AUDIO_CTL_SWITCH_DRV_OUT:
        {
            ret = _SPRD_CODEC_Ioctl_Drv_Out_Ctl(ioctl_arg);
        }break;
        case AUDIO_CTL_SWITCH_DRV_IN:
        {
            ret = _SPRD_CODEC_Ioctl_Drv_In_Ctl(ioctl_arg);
        }break;
        case AUDIO_GET_SUPPORT_FS:
        {
            ret = _SPRD_CODEC_Ioctl_Get_FS(ioctl_arg);
        }break;
        case AUDIO_UPDTAE_LDO_VOL:
        {
            ret = _SPRD_CODEC_Ioctl_Update_LDO_VOL(arg);
        }break;
        case AUDIO_PA_SETTING:
        {
            ret = _SPRD_CODEC_Ioctl_PA_Setting(ioctl_arg);
        }break;
        case AUDIO_HP_UNMUTE:
        {
            ret = _SPRD_CODEC_Ioctl_HP_Mute(arg);
        }break;
        default:
        {
            //SPRD_CODEC_PRT:"[AUDDRV][SPRD]ioctl param"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_606_112_2_17_23_2_52_185,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }break;
    }

    return ret;
}



LOCAL int32 _SPRD_CODEC_Set_PGA_PA(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]PA %d,%d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_619_112_2_17_23_2_52_186,(uint8*)"ddd",pga->id,pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_sprd_codec.pga_0.pa = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_sprd_codec.pga_1.pa = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    //_SPRD_CODEC_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( SPRD_CODEC_IsOnStop() )
    {
        __sprd_codec_ap_pa_pga_gain(s_sprd_codec.pga_0.pa);
    }
    return ret;
}


LOCAL int32 _SPRD_CODEC_Set_PGA_HP(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]HP %d,%d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_638_112_2_17_23_2_52_187,(uint8*)"ddd",pga->id,pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_sprd_codec.pga_0.hp = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_sprd_codec.pga_1.hp = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    //_SPRD_CODEC_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( SPRD_CODEC_IsOnStop() )
    {
        __sprd_codec_ap_hp_pga_gain(AUDIO_CHAN_L,s_sprd_codec.pga_0.hp);
        __sprd_codec_ap_hp_pga_gain(AUDIO_CHAN_R,s_sprd_codec.pga_1.hp);
    }
    return ret;
}

LOCAL int32 _SPRD_CODEC_Set_PGA_EAR(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    if(AUDIO_IS_CHAN_R_ONLY(pga->chan))
    {
        return ret;
    }
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]EP %d,%d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_665_112_2_17_23_2_52_188,(uint8*)"ddd",pga->id,pga->level,pga->chan);
    s_sprd_codec.pga_0.ear = pga->level;
    s_sprd_codec.pga_1.ear = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    //_SPRD_CODEC_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( SPRD_CODEC_IsOnStop() )
    {
        __sprd_codec_ap_ear_pga_gain(s_sprd_codec.pga_0.ear);
    }
    return ret;
}

LOCAL int32 _SPRD_CODEC_Set_ADCPGA(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //SPRD_CODEC_DBG:"[AUDDRV][SPRD]ADCPGA %d,%d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_680_112_2_17_23_2_52_189,(uint8*)"ddd",pga->id,pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_sprd_codec.pga_0.adcpga = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_sprd_codec.pga_1.adcpga = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }

    if( SPRD_CODEC_IsOnStop() )
    {
        __sprd_codec_ap_adc_pga_gain(AUDIO_CHAN_L,s_sprd_codec.pga_0.adcpga);
        __sprd_codec_ap_adc_pga_gain(AUDIO_CHAN_R,s_sprd_codec.pga_1.adcpga);
    }
    return ret;
}

#if (PGA_SMOOTH_ADJUST)

LOCAL  void _SPRD_CODEC_Pga_Adj_Send_Msg(uint32 msg)
{
    xSignalHeader   signal;

    SCI_CREATE_SIGNAL(
        signal,
        msg,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());

    SCI_SEND_SIGNAL(signal,s_sprd_codec.task_id);
}

LOCAL void _SPRD_CODEC_Timer_Pga_Adj_Callback(uint32 tmp)
{
    SCI_DeactiveTimer(s_sprd_codec.timer);
    _SPRD_CODEC_Pga_Adj_Send_Msg(SPRD_CODEC_PGA_ADJ);
}


LOCAL uint32 _SPRD_CODEC_Timer_Pga_Adj(uint32 time)
{
    if( time )
    {
        // start timer.
        SCI_ChangeTimer(s_sprd_codec.timer, _SPRD_CODEC_Timer_Pga_Adj_Callback, time);
        SCI_ActiveTimer(s_sprd_codec.timer);
        return 1;
    }
    return 0;
}

LOCAL void _SPRD_CODEC_Pga_Adj_Task_Init(void)
{
    s_sprd_codec.adjusting = 0;
    s_sprd_codec.timer = SCI_CreatePeriodTimer ("_SPRD_CODEC_PGA_ADJ_TIMER",
                       _SPRD_CODEC_Timer_Pga_Adj_Callback, 0, 5, SCI_NO_ACTIVATE);
    s_sprd_codec.mutex = SCI_CreateMutex("_SPRD_CODEC_PGA_ADJ_MUTEX", SCI_INHERIT);
}

LOCAL void _SPRD_CODEC_Pga_Setting(void)
{
    SCI_MEMCPY(&(s_sprd_codec.pga_adj_0),__sprd_codec_ap_pga_setting(AUDIO_CHAN_L),sizeof(s_sprd_codec.pga_adj_0));
    SCI_MEMCPY(&(s_sprd_codec.pga_adj_1),__sprd_codec_ap_pga_setting(AUDIO_CHAN_R),sizeof(s_sprd_codec.pga_adj_1));
    if( !s_sprd_codec.adjusting )
    {
        _SPRD_CODEC_Pga_Adj_Send_Msg(SPRD_CODEC_PGA_ADJ);
    }
}

LOCAL uint32 _SPRD_CODEC_Adc_Pga_is_mute(uint32 val)
{
    return (__sprd_codec_ap_adc_pga_gain_is_mute() 
            && (ADC_PGA_G_MUTE_SET==val) );
}
LOCAL uint32 _SPRD_CODEC_Hp_Pga_is_mute(uint32 val)
{
    return (__sprd_codec_ap_hp_pga_gain_is_mute() 
            && (DAC_HP_PGA_G_MUTE_SET==val) );
}
LOCAL uint32 _SPRD_CODEC_Ep_Pga_is_mute(uint32 val)
{
    return (__sprd_codec_ap_ep_pga_gain_is_mute() 
            && (DAC_EAR_PGA_G_MUTE_SET==val) );
}
LOCAL uint32 _SPRD_CODEC_Pa_Pga_is_mute(uint32 val)
{
    return (__sprd_codec_ap_pa_pga_gain_is_mute() 
            && (DAC_PA_PGA_G_MUTE_SET==val) );
}


LOCAL uint32 _SPRD_CODEC_Adc_Pga_Adj(uint32 chan)
{
    uint32 ret = 0;
    uint32 val;
    if(AUDIO_IS_CHAN_L(chan))
    {
        val = __sprd_codec_ap_adc_pga_gain_get(AUDIO_CHAN_L) ;
        if( val != s_sprd_codec.pga_adj_0.adcpga )
        {
            if( val < s_sprd_codec.pga_adj_0.adcpga )
            {
                __sprd_codec_ap_adc_pga_gain_set(AUDIO_CHAN_L,++val);
            }
            else if( val > s_sprd_codec.pga_adj_0.adcpga )
            {
                __sprd_codec_ap_adc_pga_gain_set(AUDIO_CHAN_L,--val);
            }
            ret = ret || (val != s_sprd_codec.pga_adj_0.adcpga);
        }
    }
    if(AUDIO_IS_CHAN_R(chan))
    {
        val = __sprd_codec_ap_adc_pga_gain_get(AUDIO_CHAN_R) ;
        if( val != s_sprd_codec.pga_adj_1.adcpga )
        {
            if( val < s_sprd_codec.pga_adj_1.adcpga )
            {
                __sprd_codec_ap_adc_pga_gain_set(AUDIO_CHAN_R,++val);
            }
            else if( val > s_sprd_codec.pga_adj_1.adcpga )
            {
                __sprd_codec_ap_adc_pga_gain_set(AUDIO_CHAN_R,--val);
            }
            ret = ret || (val != s_sprd_codec.pga_adj_1.adcpga);
        }
    }
    return ret;
}

LOCAL uint32 _SPRD_CODEC_Hp_Pga_Adj(uint32 chan)
{
    uint32 ret = 0;
    uint32 val;
    if(AUDIO_IS_CHAN_L(chan))
    {
        val = __sprd_codec_ap_hp_pga_gain_get(AUDIO_CHAN_L) ;
        if( val != s_sprd_codec.pga_adj_0.hp )
        {
            if( val < s_sprd_codec.pga_adj_0.hp )
            {
                __sprd_codec_ap_hp_pga_gain_set(AUDIO_CHAN_L,++val);
            }
            else if( val > s_sprd_codec.pga_adj_0.hp )
            {
                __sprd_codec_ap_hp_pga_gain_set(AUDIO_CHAN_L,--val);
            }
            ret = ret || (val != s_sprd_codec.pga_adj_0.hp);
        }
    }
    if(AUDIO_IS_CHAN_R(chan))
    {
        val = __sprd_codec_ap_hp_pga_gain_get(AUDIO_CHAN_R) ;
        if( val != s_sprd_codec.pga_adj_1.hp )
        {
            if( val < s_sprd_codec.pga_adj_1.hp )
            {
                __sprd_codec_ap_hp_pga_gain_set(AUDIO_CHAN_R,++val);
            }
            else if( val > s_sprd_codec.pga_adj_1.hp )
            {
                __sprd_codec_ap_hp_pga_gain_set(AUDIO_CHAN_R,--val);
            }
            ret = ret || (val != s_sprd_codec.pga_adj_1.hp);
        }
    }
    return ret;
}

LOCAL uint32 _SPRD_CODEC_Pa_Pga_Adj(uint32 chan)
{
    uint32 ret = 0;
    uint32 val;
    if(AUDIO_IS_CHAN_L(chan))
    {
        val = __sprd_codec_ap_pa_pga_gain_get(AUDIO_CHAN_L) ;
        if( val != s_sprd_codec.pga_adj_0.pa )
        {
            if( val < s_sprd_codec.pga_adj_0.pa )
            {
                __sprd_codec_ap_pa_pga_gain_set(AUDIO_CHAN_L,++val);
            }
            else if( val > s_sprd_codec.pga_adj_0.pa )
            {
                __sprd_codec_ap_pa_pga_gain_set(AUDIO_CHAN_L,--val);
            }
            ret = ret || (val != s_sprd_codec.pga_adj_0.pa);
        }
    }
    if(AUDIO_IS_CHAN_R(chan))
    {
        val = __sprd_codec_ap_pa_pga_gain_get(AUDIO_CHAN_R) ;
        if( val != s_sprd_codec.pga_adj_1.pa )
        {
            if( val < s_sprd_codec.pga_adj_1.pa )
            {
                __sprd_codec_ap_pa_pga_gain_set(AUDIO_CHAN_R,++val);
            }
            else if( val > s_sprd_codec.pga_adj_1.pa )
            {
                __sprd_codec_ap_pa_pga_gain_set(AUDIO_CHAN_R,--val);
            }
            ret = ret || (val != s_sprd_codec.pga_adj_1.pa);
        }
    }
    return ret;
}

LOCAL uint32 _SPRD_CODEC_Ear_Pga_Adj(uint32 chan)
{
    uint32 ret = 0;
    uint32 val;
    if(AUDIO_IS_CHAN_L(chan))
    {
        val = __sprd_codec_ap_ear_pga_gain_get() ;
        if( val == s_sprd_codec.pga_adj_0.ear )
        {
        }
        else if( val < s_sprd_codec.pga_adj_0.ear )
        {
            __sprd_codec_ap_ear_pga_gain_set(++val);
        }
        else if( val > s_sprd_codec.pga_adj_0.ear )
        {
            __sprd_codec_ap_ear_pga_gain_set(--val);
        }
        ret = (val != s_sprd_codec.pga_adj_0.ear);
    }
    return ret;
}

LOCAL void _SPRD_CODEC_Pga_Adj_Complete(void)
{
    uint32 val = 0;
    val = (__sprd_codec_ap_adc_pga_gain_get(AUDIO_CHAN_L) 
        || __sprd_codec_ap_adc_pga_gain_get(AUDIO_CHAN_R));
    if( _SPRD_CODEC_Adc_Pga_is_mute(val) )
    {
        __sprd_codec_ap_adc(0,AUDIO_ALL_CHAN);
        __sprd_codec_ap_adc_pga_gain_clr_mute();
    }
    
    val = (__sprd_codec_ap_hp_pga_gain_get(AUDIO_CHAN_L) 
        || __sprd_codec_ap_hp_pga_gain_get(AUDIO_CHAN_R));
    if( _SPRD_CODEC_Hp_Pga_is_mute(val) )
    {
        SCI_Sleep(CLOSE_DELAY_TIME);
        __sprd_codec_ext_hp_pa_ctl(0);
        if( !__sprd_codec_is_use_ext_hp() )
            __sprd_codec_ap_hp(0,AUDIO_ALL_CHAN);
        __sprd_codec_ap_hp_pga_gain_clr_mute();
    }

//TODO: need support two spk!
    val = (__sprd_codec_ap_pa_pga_gain_get(AUDIO_CHAN_L) 
        || __sprd_codec_ap_pa_pga_gain_get(AUDIO_CHAN_R));
    if( _SPRD_CODEC_Pa_Pga_is_mute(val) )
    {
        SCI_Sleep(CLOSE_DELAY_TIME);
        __sprd_codec_ext_pa_ctl(0,0);
        __sprd_codec_ap_lout(0);
        __sprd_codec_ap_pa_pga_gain_clr_mute();
    }

    val = __sprd_codec_ap_ear_pga_gain_get() ;
    if( _SPRD_CODEC_Ep_Pga_is_mute(val) )
    {
        __sprd_codec_ap_ear(0);
        __sprd_codec_ap_ep_pga_gain_clr_mute();
    }
    
    SPRD_CODEC_SM_Process();//ken.kuang FOR AUDIO CLOSE NOISE!!
}

LOCAL void _SPRD_CODEC_Pga_Adj(void)
{
    s_sprd_codec.adjusting = 0;
    if( _SPRD_CODEC_Adc_Pga_Adj(AUDIO_ALL_CHAN) )
    {
        s_sprd_codec.adjusting = 1;
    }
    if( _SPRD_CODEC_Hp_Pga_Adj(AUDIO_ALL_CHAN) )
    {
        s_sprd_codec.adjusting = 1;
    }
    if( _SPRD_CODEC_Pa_Pga_Adj(AUDIO_ALL_CHAN) )
    {
        s_sprd_codec.adjusting = 1;
    }
    if( _SPRD_CODEC_Ear_Pga_Adj(AUDIO_ALL_CHAN) )
    {
        s_sprd_codec.adjusting = 1;
    }
    if( s_sprd_codec.adjusting )
    {
        _SPRD_CODEC_Timer_Pga_Adj(PGA_SMOOTH_ADJUST_STEP);
    }
    else
    {
        _SPRD_CODEC_Pga_Adj_Complete();
        __sprd_codec_ap_reg_print();
    }
}


/*****************************************************************************/
//  Description:    sprd_codec pga smooth adjust task entry.
//  Author:         ken.kuang
//  Note:
/*****************************************************************************/
LOCAL void _SPRD_CODEC_Pga_Adj_Thread (uint32 argc, void *argv)
{
    xSignalHeader	signal;
    uint16          is_pga_setting = 0;
    uint16          is_pga_adj = 0;

    _SPRD_CODEC_Pga_Adj_Task_Init();

    for(;;)
    {
        SCI_RECEIVE_SIGNAL(signal, s_sprd_codec.task_id);
        while( signal )
        {
            switch(signal->SignalCode)
            {
                case SPRD_CODEC_PGA_SETTING:
                {
                    is_pga_setting = 1;
                }break;
                case SPRD_CODEC_PGA_ADJ:
                {
                    is_pga_adj = 1;
                }break;
                default:break;
            }
            SCI_FREE_SIGNAL(signal);
            signal = SCI_NULL;
            SCI_PEEK_SIGNAL(signal, s_sprd_codec.task_id);
        }
        signal = SCI_NULL;
        __sprd_codec_pga_adj_get_mutex();
        if( is_pga_setting )
        {
            _SPRD_CODEC_Pga_Setting();
            is_pga_setting = 0;
        }
        if( is_pga_adj )
        {
            _SPRD_CODEC_Pga_Adj();
            is_pga_adj = 0;
        }
        __sprd_codec_pga_adj_put_mutex();
    }
}

PUBLIC void __sprd_codec_pga_adj_setting(void)
{
    _SPRD_CODEC_Pga_Adj_Send_Msg(SPRD_CODEC_PGA_SETTING);
}

PUBLIC void __sprd_codec_pga_adj_create_task(void)
{
    static uint32 do_this = 1;
    if( do_this )
    {
        s_sprd_codec.task_id  = SCI_CreateThread(SPRD_CODEC_PGA_ADJ_TASK_NAME,
            SPRD_CODEC_PGA_ADJ_QNAME_NAME,
            _SPRD_CODEC_Pga_Adj_Thread ,
            0,
            0,
            SPRD_CODEC_PGA_ADJ_STACK_SIZE ,
            SPRD_CODEC_PGA_ADJ_QUEUE_NUM,
            SPRD_CODEC_PGA_ADJ_TASK_PRIV,
            SCI_PREEMPT,
            SCI_AUTO_START);
        do_this = 0;
    }
}

PUBLIC void __sprd_codec_pga_adj_get_mutex(void)
{
    uint32 local_ret = SCI_ERROR;
    if(SCI_InThreadContext())
    {
        if( NULL != s_sprd_codec.mutex )
        {
            local_ret = SCI_GetMutex(s_sprd_codec.mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
        }
    }
    SPRD_CODEC_DBG_ASS( local_ret == SCI_SUCCESS );/*assert verified*/
}

PUBLIC void __sprd_codec_pga_adj_put_mutex(void)
{
    uint32 local_ret = SCI_ERROR;
    if(SCI_InThreadContext())
    {
        if( NULL != s_sprd_codec.mutex )
        {
            local_ret = SCI_PutMutex( s_sprd_codec.mutex );
        }
    }
    SPRD_CODEC_DBG_ASS( local_ret == SCI_SUCCESS );/*assert verified*/
}

#endif


/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL const AUD_CODEC_OPS_T s_audio_codec_sprd_codec_ops=
{
    _SPRD_CODEC_Init,
    _SPRD_CODEC_Open,
    _SPRD_CODEC_Close,
    _SPRD_CODEC_Ioctl,
};

LOCAL const SPRD_CODEC_PGA_FUN_T s_sprd_codec_pga_set_ops[]=
{
    {SPRD_CODEC_PGA_PA              ,_SPRD_CODEC_Set_PGA_PA     },
    {SPRD_CODEC_PGA_HP              ,_SPRD_CODEC_Set_PGA_HP     },
    {SPRD_CODEC_PGA_EAR             ,_SPRD_CODEC_Set_PGA_EAR    },
    {SPRD_CODEC_ADCPGA              ,_SPRD_CODEC_Set_ADCPGA     },
    {0                              ,0                          },
}; 


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_CODEC_OPS_T* AUDIO_CODEC_Get_SPRD_CODEC_OPS(uint32 param)
{
    return (AUD_CODEC_OPS_T*)(&s_audio_codec_sprd_codec_ops);
}

PUBLIC SPRD_CODEC_PGA_FUN_T* SPRD_CODEC_Get_PGA_OPS(uint32 param)
{
    return (SPRD_CODEC_PGA_FUN_T*)(&s_sprd_codec_pga_set_ops);
}

PUBLIC void __sprd_codec_fs_setting(void)
{
    if( s_sprd_codec.da_sample_rate )
    {
        __sprd_codec_dp_dac_fs_mode_set(s_sprd_codec.da_sample_rate);
    }
    if( s_sprd_codec.ad_sample_rate )
    {
        __sprd_codec_dp_adc_fs_mode_set(s_sprd_codec.ad_sample_rate);
    }
}

PUBLIC void __sprd_codec_run_callback(uint32 is_open)
{
    if( is_open )
    {
        __sprd_codec_fs_setting();
        _sprd_codec_update_pga(AUDIO_ALL_CHAN);
        _sprd_codec_update_sw();
        __sprd_codec_update_ldo();
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_OPENED);
        if( s_sprd_codec.is_dsp_ctl )
        {
            __sprd_codec_switch_to(0);
        }
    }
    else
    {
        //SCI_MEMSET(&(s_sprd_codec.path),0,sizeof(s_sprd_codec.path));
        //s_sprd_codec.ad_sample_rate = 0;
        //s_sprd_codec.da_sample_rate = 0;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_CLOSEED);
    }
    //SPRD_CODEC_DBG:"_run_callback:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_767_112_2_17_23_2_52_190,(uint8*)"d",is_open);
    AUDIO_HAL_PRTMEM ( (0x55AA0000 
        | (is_open & 0x0000FFFF)    ));
}


PUBLIC uint32 AUDIO_PA_Ctl(uint32 id,uint32 is_open)
{
    //SPRD_CODEC_DBG:"AUDIO_PA_Ctl:%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_SPRD_775_112_2_17_23_2_52_191,(uint8*)"dd",id,is_open);
    if( id )
    {
        return (uint32)-1;
    }
    if( is_open )
    {
        __sprd_codec_ap_pa_d_en(s_sprd_codec.pa.is_classD); //class-D / -AB
        __sprd_codec_ap_pa_demi_en(s_sprd_codec.pa.is_DEMI);
        if( s_sprd_codec.pa.is_LDO )
        {
            if( 0==s_sprd_codec.pa.is_auto_LDO )
            {
                __sprd_codec_ap_pa_ldo_v_sel(s_sprd_codec.pa.LDO_V_sel);
            }
            __sprd_codec_ap_pa_ldo_en(1);
        }
        __sprd_codec_ap_pa_dtri_f_sel(s_sprd_codec.pa.DTRI_F_sel);
        __sprd_codec_ap_pa_en(1);
    }
    else
    {
        __sprd_codec_ap_pa_en(0);
        __sprd_codec_ap_pa_ldo_en(0);
    }
    return 0;
}

PUBLIC void __sprd_codec_update_vcm(void)
{
    if( (s_sprd_codec.pa.is_auto_LDO) && (!SPRD_CODEC_IsPwrOn()) )
    {
        __sprd_codec_update_vb_vbo_vcm_vol(s_sprd_codec.cur_vol);
    }
}

PUBLIC void __sprd_codec_update_ldo(void)
{
    if( (s_sprd_codec.pa.is_auto_LDO) )
    {
        __sprd_codec_update_ldo_vol(s_sprd_codec.cur_vol);
    }
}

PUBLIC uint32 __sprd_codec_ap_mic_bias_ldo(uint32 en,uint32 ldo_val)
{
    __sprd_codec_ap_mic_bias_ldo_v_sel(ldo_val);
    if( en )
    {
        __sprd_codec_register(1);
        __sprd_codec_ap_bg_ibais_en(1);
        __sprd_codec_ap_bg_en(1);
        __sprd_codec_ap_vcm_en(1);
        __sprd_codec_ap_vcm_buf_en(1);
        __sprd_codec_ap_vb_en(1);
        __sprd_codec_ap_mic_bias_ldo_set(1);
    }
    else
    {
        __sprd_codec_ap_mic_bias_ldo_set(0);
        if(!SPRD_CODEC_IsPwrOn())
        {
            __sprd_codec_ap_vcm_en(0);
            SCI_Sleep(2);
            __sprd_codec_ap_vcm_buf_en(0);
            __sprd_codec_ap_vb_en(0);
            __sprd_codec_ap_vbo_en(0);
            __sprd_codec_ap_bg_ibais_en(0);
            __sprd_codec_ap_bg_en(0);
            
            __sprd_codec_register(0);
        }
    }
    return 0;
}

PUBLIC void __sprd_codec_ext_hp_pa_ctl_with_mute(uint32 en)
{
    uint32 real_en = en;
    en = (en && !(s_sprd_codec.path.dac.hp.is_mute));
    if( real_en && (en != real_en) && (s_sprd_codec.path.dac.hp.is_next_enabled) )
        return;
    __sprd_codec_ext_hp_pa_ctl_inter(en);
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
