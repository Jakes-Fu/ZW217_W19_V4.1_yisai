/******************************************************************************
 ** File Name:      audio_codec_dolphin.c                                     *
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
#include "dolphin_phy_cfg.h"
#include "dol_state.h"
#include "dol_phy.h"

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
#ifdef DOL_DEBUG
#define DOLPHIN_DBG       SCI_TRACE_LOW
#define DOLPHIN_DBG_ASS   SCI_ASSERT
#else
#define DOLPHIN_DBG(...)
#define DOLPHIN_DBG_ASS(x)
#endif

#define DOLPHIN_PRT       SCI_TRACE_LOW

#define DOL_ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

enum DOLPHIN_OC_E{
    DOL_OPEN    = 0,
    DOL_CLOSE
};

typedef struct
{
    uint32                  fun_flg;
    uint32                  fun_dol;
} DOL_FUN_T;


typedef struct
{
    uint                    adc_gim:1;      //GIM:1
    uint                    adc_gi:4;       //GI:4
    uint                    lineinrec:3;    //SPRD ADD
    uint                    dac:4;
    uint                    linein:5;
    uint                    hp:5;
    uint                    ear:1;
    uint                    RSV:9;
} DOL_PGA_T;

typedef struct
{
    uint32                  is_ep_on;
    uint32                  is_hp_on;
    uint32                  is_spk_on;

} DOL_SWITCH_T;

typedef struct
{
    char                    magic_str[AUDIO_MAGIC_STR_MAX_LEN];
    uint32                  da_sample_rate;
    uint32                  ad_sample_rate;

    DOL_PGA_T               pga_0;
    DOL_PGA_T               pga_1;
    DOL_SWITCH_T            sw;
    uint32                  mic_sel;
    uint32                  is_dsp_ctl;
    uint32                  is_hp_mute;
    uint32                  is_hp_next_enabled;

    uint32                  cur_vol;
    
    uint32                  magic_num;
} DOL_T;



/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL DOL_T s_dol = {0};
LOCAL const DOL_FUN_T s_dol_support_fun[]=
{
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_PLAYBACK))
                                ,DOL_FUN_PLAYBACK   },
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_CAPTRUE))
                                ,DOL_FUN_CAPTURE    },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_PLAYBACK))
                                ,DOL_FUN_LINEIN     },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_CAPTRUE))
                                ,DOL_FUN_LINEIN_REC },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_PLAYBACK))
                                ,DOL_FUN_PLAYBACK   },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_CAPTRUE))
                                ,0                  },
};
LOCAL const uint32 s_dol_support_fun_len=DOL_ARR_SIZE(s_dol_support_fun);

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

LOCAL void _dol_update_pga(uint32 uiChannel)
{
    __dol_set_pag_lrgod(uiChannel,s_dol.pga_0.dac,s_dol.pga_1.dac);
    __dol_set_pag_lrgob(uiChannel,s_dol.pga_0.linein,s_dol.pga_1.linein);
    __dol_set_pag_lrgo(uiChannel,s_dol.pga_0.hp,s_dol.pga_1.hp);
    if( 0==uiChannel )
    {
        __dol_set_pag_gim(s_dol.pga_0.adc_gim);
        __dol_set_pag_gi(s_dol.pga_0.adc_gi);
        __dol_set_pag_golr(s_dol.pga_0.lineinrec);
        __dol_set_pag_gobtl(s_dol.pga_0.ear);
    }
}

LOCAL void _dol_update_sw(void)
{
    __dol_hp_mute((uint32)(!(s_dol.sw.is_hp_on)));
    __dol_ep_mute((uint32)(!(s_dol.sw.is_ep_on)));
    __dol_ext_pa_ctl(s_dol.sw.is_spk_on);
    __dol_mic_sel(s_dol.mic_sel);
}


LOCAL uint32 _DOLPHIN_Fun_Flg_Is_Eq(uint32 src_flg,uint32 dst_flg)
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

LOCAL int32 _DOLPHIN_Parse_Fun_Flg(uint32 fun_flg,uint32 oc)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 i;
    uint32 fun_deal = (fun_flg);

    if( (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)
        & fun_deal)    )
    {
        for(i=0;i<s_dol_support_fun_len;i++)
        {
            if( _DOLPHIN_Fun_Flg_Is_Eq((s_dol_support_fun[i].fun_flg) , fun_flg) )
            {
                if( s_dol_support_fun[i].fun_dol )
                {
                    DOL_ChangeSt((s_dol_support_fun[i].fun_dol),oc);
                }
            }
        }
    }
    else
    {
        if( AUDIO_FUN_PLAYBACK & fun_deal)
        {
            DOL_ChangeSt(DOL_FUN_PLAYBACK,oc);
        }
        if( AUDIO_FUN_CAPTRUE & fun_deal)
        {
            DOL_ChangeSt(DOL_FUN_CAPTURE,oc);
        }
    }

    
    return ret;
}


/*****************************************************************************/
// Description :    DOLPHIN Init.
//                  this is Codec Init.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _DOLPHIN_Init(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    DOLPHIN_DBG("[AUDDRV][DOL]init dolphin");
    if( AUDIO_MAGIC_NUM != s_dol.magic_num )
    {
        SCI_MEMSET(&s_dol,0,sizeof(s_dol));
        s_dol.magic_num = AUDIO_MAGIC_NUM;
        SCI_MEMCPY(s_dol.magic_str,AUDIO_MAGIC_STR,AUDIO_MAGIC_STR_MAX_LEN);
        
        __dol_create_task();
        __dol_init();
        DOLPHIN_PGA_Init_Set(param);
    }
    return ret;
}

/*****************************************************************************/
// Description :    DOLPHIN Open.
//                  this Codec Power On.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _DOLPHIN_Open(uint32 fun_flg,uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    DOLPHIN_DBG("[AUDDRV][DOL]open dolphin 0x%x,%d",fun_flg,fs);

    if( AUDIO_HAL_SUCCESS != _DOLPHIN_Parse_Fun_Flg(fun_flg,DOL_OPEN) )
    {
        //DOLPHIN_PRT:"[AUDDRV][DOL]open err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_253_112_2_17_23_2_38_62,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }

    if( AUDIO_FUN_PLAYBACK & fun_flg )
    {
        s_dol.da_sample_rate = fs;
    }
    if( AUDIO_FUN_CAPTRUE & fun_flg )
    {
        s_dol.ad_sample_rate = fs;
    }

    if( (DOL_CTRL_BY_DSP & fun_flg) )
    {
        s_dol.is_dsp_ctl = 1;
    }
    
    DOL_SM_Process();
    
    return ret;
}

/*****************************************************************************/
// Description :    DOLPHIN Close.
//                  this is Codec Power Off.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _DOLPHIN_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    DOLPHIN_DBG("[AUDDRV][DOL]close dolphin 0x%x",fun_flg);
    
    if( s_dol.is_dsp_ctl )
    {
        __dol_switch_to(1);
        s_dol.is_dsp_ctl = 0;
    }
    if( AUDIO_HAL_SUCCESS != _DOLPHIN_Parse_Fun_Flg(fun_flg,DOL_CLOSE) )
    {
        //DOLPHIN_PRT:"[AUDDRV][DOL]close err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_287_112_2_17_23_2_38_63,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }
    
    if( AUDIO_FUN_PLAYBACK & fun_flg )
    {
        SCI_MEMSET(&(s_dol.sw),0,sizeof(s_dol.sw));
    }
    if( AUDIO_FUN_CAPTRUE & fun_flg )
    {
        s_dol.mic_sel = 0;
    }


    DOL_SM_Process();
    
    return ret;
}

LOCAL int32 _DOLPHIN_Ioctl(uint32 cmd, uint32 arg);

LOCAL int32 _DOLPHIN_Set_Drv_out(AUDIO_CTL_PGA_T * pga,uint32 mute)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_SWITCH_T sw = {0};
    sw.id = pga->id;
    sw.chan = pga->chan;
    sw.mode = (pga->mode&(AUDIO_SPK|AUDIO_EAR|AUDIO_HP));
    sw.val = mute;
    ret = _DOLPHIN_Ioctl(AUDIO_CTL_SWITCH_DRV_OUT,(uint32)(&sw));
    return ret;
}

LOCAL int32 _DOLPHIN_Ioctl_PGA(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_PGA_T *pga = 0;
    DOLPHIN_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg)  && (0==ioctl_arg->pga.id)  ) //lint !e774
    {
        pga = &(ioctl_arg->pga);
        //if( (pga->level) )
        //{
            ret = DOLPHIN_PGA_Set(pga);
        //}
        //else
        //{
        //    _DOLPHIN_Set_Drv_out(pga,AUDIO_MUTE);
        //}
    }
    else
    {
        //DOLPHIN_PRT:"[AUDDRV][DOL]pga param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_329_112_2_17_23_2_38_64,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL void _DOLPHIN_Lout_St(void)
{
    if( (0==s_dol.sw.is_spk_on) && (0==s_dol.sw.is_ep_on) )
    {
        DOL_ChangeSt(DOL_FUN_LOUT,1);
    }
    else
    {
        DOL_ChangeSt(DOL_FUN_LOUT,0);
    }
    DOL_SM_Process();
}

LOCAL int32 _DOLPHIN_Ioctl_Drv_Out_Ctl(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    DOLPHIN_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg) && (0==ioctl_arg->sw.id) ) //lint !e774
    {
        DOLPHIN_DBG("[AUDDRV][DOL]drv out ctl 0x%x,%d",ioctl_arg->sw.mode,ioctl_arg->sw.val);
        switch(ioctl_arg->sw.mode)
        {
            case AUDIO_SPK:
            {
                s_dol.sw.is_spk_on = ioctl_arg->sw.val;  //1
                _DOLPHIN_Lout_St();                         //2
                DOL_SM_Process();
                if( DOL_IsOnStop() )
                {
                    __dol_ext_pa_ctl(s_dol.sw.is_spk_on);
                }
            }break;
            case AUDIO_EAR:
            {
                s_dol.sw.is_ep_on = ioctl_arg->sw.val;   //1
                _DOLPHIN_Lout_St();                         //2
                DOL_SM_Process();
                if( DOL_IsOnStop() )
                {
                    __dol_ep_mute((uint32)(!(s_dol.sw.is_ep_on)));
                }
            }break;
            case AUDIO_HP:
            {
                s_dol.sw.is_hp_on = ioctl_arg->sw.val;
                DOL_ChangeSt(DOL_FUN_HP,(uint32)(!(ioctl_arg->sw.val)));
                DOL_SM_Process();
                if( DOL_IsOnStop() )
                {
                    __dol_hp_mute((uint32)(!(s_dol.sw.is_hp_on)));
                    //__dol_ext_hp_pa_ctl(s_dol.sw.is_hp_on);
                }
            }break;
            default:
            {
                //DOLPHIN_PRT:"[AUDDRV][DOL]drv out 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_384_112_2_17_23_2_38_65,(uint8*)"d",ioctl_arg->sw.mode);
                ret = AUDIO_HAL_ERROR;
            }break;
        }
    }
    else
    {
        //DOLPHIN_PRT:"[AUDDRV][DOL]drv out param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_391_112_2_17_23_2_38_66,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _DOLPHIN_Ioctl_Drv_In_Ctl(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    DOLPHIN_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg)  && (0==ioctl_arg->sw.id) ) //lint !e774
    {
        switch(ioctl_arg->sw.mode)
        {
            case AUDIO_MIC:
            {
                if( ioctl_arg->sw.val )
                {
                    s_dol.mic_sel = 0;
                    if( DOL_IsOnStop() )
                    {
                        __dol_mic_sel(s_dol.mic_sel);
                    }
                }
            }break;
            case AUDIO_HP_MIC:
            {
                s_dol.mic_sel = ioctl_arg->sw.val;
                if( DOL_IsOnStop() )
                {
                    __dol_mic_sel(s_dol.mic_sel);
                }
            }break;
            case AUDIO_LINEIN:
            {
                //DOLPHIN_PRT:"[AUDDRV][DOL]drv in linein?"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_427_112_2_17_23_2_38_67,(uint8*)"");
                ret = AUDIO_HAL_ERROR;
            }break;
            default:
            {
                //DOLPHIN_PRT:"[AUDDRV][DOL]drv in 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_432_112_2_17_23_2_38_68,(uint8*)"d",ioctl_arg->sw.mode);
                ret = AUDIO_HAL_ERROR;
            }break;
        }
    }
    else
    {
        //DOLPHIN_PRT:"[AUDDRV][DOL]drv in param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_439_112_2_17_23_2_38_69,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _DOLPHIN_Ioctl_Dac_Mute(uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    DOLPHIN_DBG_ASS((arg==AUDIO_MUTE) || (arg==AUDIO_UNMUTE));/*assert verified*/
    if(arg==AUDIO_MUTE)
    {
        DOL_ChangeSt(DOL_FUN_DAC_UNMUTE,1);
        DOL_SM_Process();
    }
    else if(arg==AUDIO_UNMUTE)
    {
        DOL_ChangeSt(DOL_FUN_DAC_UNMUTE,0);
        DOL_SM_Process();
    }
    else
    {
        //DOLPHIN_PRT:"[AUDDRV][DOL]dac mute"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_462_112_2_17_23_2_38_70,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _DOLPHIN_Ioctl_Get_FS(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    DOLPHIN_DBG_ASS(ioctl_arg);/*assert verified*/
    if( ioctl_arg ) //lint !e774
    {
        ioctl_arg->dwVal = __dol_calc_fs(ioctl_arg->dwVal);
    }
    else
    {
        //DOLPHIN_PRT:"[AUDDRV][DOL]get fs param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_479_112_2_17_23_2_38_71,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _DOLPHIN_Ioctl_Update_LDO_VOL(uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    s_dol.cur_vol = arg;
    if( (DOL_IsPwrOn()) )
    {
        __dol_update_ldo_vol(s_dol.cur_vol);
    }
    return ret;
}



LOCAL int32 _DOLPHIN_Ioctl_HP_Mute(uint32 arg)
{
    int32 next_enabled = 0;
    int32 ret = AUDIO_HAL_SUCCESS;
    next_enabled = !!((uint32)AUDIO_NEXT_ENABLED & arg);
    arg &= ~AUDIO_NEXT_ENABLED;
    DOLPHIN_DBG_ASS((arg==AUDIO_MUTE) || (arg==AUDIO_UNMUTE));/*assert verified*/
    if((arg==AUDIO_MUTE) || (arg==AUDIO_UNMUTE))
    {
        s_dol.is_hp_mute = (arg==AUDIO_UNMUTE)?0:1;
        s_dol.is_hp_next_enabled = next_enabled;
        if( !next_enabled && DOL_IsOnStop() )
        {
            __dol_ext_hp_pa_ctl_inter((uint32)(!s_dol.is_hp_mute));
            if( !__dol_is_use_ext_hp() )
            {
                if( !s_dol.is_hp_mute )
                {
                    _dol_update_pga(0);
                    _dol_update_pga(1);
                }
                __dol_hp_mute(s_dol.is_hp_mute);
            }
        }
    }
    else
    {
        DOLPHIN_PRT("[AUDDRV][SPRD]hp unmute err");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


/*****************************************************************************/
// Description :    DOLPHIN Ioctl.
//                  this is Codec Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _DOLPHIN_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_ARG_U * ioctl_arg = (AUDIO_CTL_ARG_U *)arg;
    DOLPHIN_DBG("[AUDDRV][DOL]ioctl dolphin %d,0x%x",cmd,arg);

    switch(cmd)
    {
        case AUDIO_CTL_PGA:
        {
            ret = _DOLPHIN_Ioctl_PGA(ioctl_arg);
        }break;
        case AUDIO_CTL_SWITCH_DAC:
        {
            ret = _DOLPHIN_Ioctl_Dac_Mute(arg);
        }break;
        case AUDIO_CTL_SWITCH_DRV_OUT:
        {
            ret = _DOLPHIN_Ioctl_Drv_Out_Ctl(ioctl_arg);
        }break;
        case AUDIO_CTL_SWITCH_DRV_IN:
        {
            ret = _DOLPHIN_Ioctl_Drv_In_Ctl(ioctl_arg);
        }break;
        case AUDIO_GET_SUPPORT_FS:
        {
            ret = _DOLPHIN_Ioctl_Get_FS(ioctl_arg);
        }break;
        case AUDIO_UPDTAE_LDO_VOL:
        {
            ret = _DOLPHIN_Ioctl_Update_LDO_VOL(arg);
        }break;
        case AUDIO_HP_UNMUTE:
        {
            ret = _DOLPHIN_Ioctl_HP_Mute(arg);
        }break;
        default:
        {
            //DOLPHIN_PRT:"[AUDDRV][DOL]ioctl param"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_DOLPHIN_537_112_2_17_23_2_38_72,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }break;
    }

    return ret;
}



LOCAL int32 _DOLPHIN_Set_PGA_GOD(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    DOLPHIN_DBG("[AUDDRV][DOL]GOD %d,0x%x",pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_dol.pga_0.dac = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_dol.pga_1.dac = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    //_DOLPHIN_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( DOL_IsOnStop() )
    {
        __dol_set_pag_lrgod(0,s_dol.pga_0.dac,s_dol.pga_1.dac);
        __dol_set_pag_lrgod(1,s_dol.pga_0.dac,s_dol.pga_1.dac);
    }
    return ret;
}


LOCAL int32 _DOLPHIN_Set_PGA_GO(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    DOLPHIN_DBG("[AUDDRV][DOL]GO %d,0x%x",pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_dol.pga_0.hp = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_dol.pga_1.hp = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    //_DOLPHIN_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( DOL_IsOnStop() )
    {
        __dol_set_pag_lrgo(0,s_dol.pga_0.hp,s_dol.pga_1.hp);
        __dol_set_pag_lrgo(1,s_dol.pga_0.hp,s_dol.pga_1.hp);
    }
    return ret;
}

LOCAL int32 _DOLPHIN_Set_PGA_GOBTL(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    if(AUDIO_IS_CHAN_R_ONLY(pga->chan))
    {
        return ret;
    }
    DOLPHIN_DBG("[AUDDRV][DOL]GOBTL %d,0x%x",pga->level,pga->chan);
    s_dol.pga_0.ear = pga->level;
    s_dol.pga_1.ear = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    //_DOLPHIN_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( DOL_IsOnStop() )
    {
        __dol_set_pag_gobtl(s_dol.pga_0.ear);
    }
    return ret;
}

LOCAL int32 _DOLPHIN_Set_PGA_GOB(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    DOLPHIN_DBG("[AUDDRV][DOL]GOB %d,0x%x",pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_dol.pga_0.linein = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_dol.pga_1.linein = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    //_DOLPHIN_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( DOL_IsOnStop() )
    {
        __dol_set_pag_lrgob(0,s_dol.pga_0.linein,s_dol.pga_1.linein);
        __dol_set_pag_lrgob(1,s_dol.pga_0.linein,s_dol.pga_1.linein);
    }
    return ret;
}

LOCAL int32 _DOLPHIN_Set_PGA_GIM(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    if(AUDIO_IS_CHAN_R_ONLY(pga->chan))
    {
        return ret;
    }
    DOLPHIN_DBG("[AUDDRV][DOL]GIM %d,0x%x",pga->level,pga->chan);
    s_dol.pga_0.adc_gim = pga->level;
    s_dol.pga_1.adc_gim = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    if( DOL_IsOnStop() )
    {
        __dol_set_pag_gim(s_dol.pga_0.adc_gim);
    }
    return ret;
}

LOCAL int32 _DOLPHIN_Set_PGA_GI(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    if(AUDIO_IS_CHAN_R_ONLY(pga->chan))
    {
        return ret;
    }
    DOLPHIN_DBG("[AUDDRV][DOL]GI %d,0x%x",pga->level,pga->chan);
    s_dol.pga_0.adc_gi = pga->level;
    s_dol.pga_1.adc_gi = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    if( DOL_IsOnStop() )
    {
        __dol_set_pag_gi(s_dol.pga_0.adc_gi);
    }
    return ret;
}

LOCAL int32 _DOLPHIN_Set_PGA_GOLR(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    if(AUDIO_IS_CHAN_R_ONLY(pga->chan))
    {
        return ret;
    }
    DOLPHIN_DBG("[AUDDRV][DOL]GOLR %d,0x%x",pga->level,pga->chan);
    s_dol.pga_0.lineinrec = pga->level;
    s_dol.pga_1.lineinrec = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    if( DOL_IsOnStop() )
    {
        __dol_set_pag_golr(s_dol.pga_0.lineinrec);
    }
    return ret;
}




/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL const AUD_CODEC_OPS_T s_audio_codec_dolphin_ops=
{
    _DOLPHIN_Init,
    _DOLPHIN_Open,
    _DOLPHIN_Close,
    _DOLPHIN_Ioctl,
};

LOCAL const DOL_PGA_FUN_T s_dol_pga_set_ops[]=
{
    {DOL_PGA_GOD           ,_DOLPHIN_Set_PGA_GOD    },
    {DOL_PGA_GO            ,_DOLPHIN_Set_PGA_GO     },
    {DOL_PGA_GOBTL         ,_DOLPHIN_Set_PGA_GOBTL  },
    {DOL_PGA_GOB           ,_DOLPHIN_Set_PGA_GOB    },
    {DOL_PGA_GIM           ,_DOLPHIN_Set_PGA_GIM    },
    {DOL_PGA_GI            ,_DOLPHIN_Set_PGA_GI     },
    {DOL_PGA_GOLR          ,_DOLPHIN_Set_PGA_GOLR   },
    {0                     ,0                       },
}; 


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_CODEC_OPS_T* AUDIO_CODEC_Get_DOLPHIN_OPS(uint32 param)
{
    return (AUD_CODEC_OPS_T*)(&s_audio_codec_dolphin_ops);
}

PUBLIC DOL_PGA_FUN_T* DOL_Get_PGA_OPS(uint32 param)
{
    return (DOL_PGA_FUN_T*)(&s_dol_pga_set_ops);
}

PUBLIC void __dol_fs_setting(void)
{
    if( s_dol.da_sample_rate )
    {
        __dol_set_da_samplerate(s_dol.da_sample_rate);
    }
    if( s_dol.ad_sample_rate )
    {
        __dol_set_ad_samplerate(s_dol.ad_sample_rate);
    }
}

PUBLIC void __dol_run_callback(uint32 is_open)
{
    if( is_open )
    {
        __dol_fs_setting();
        _dol_update_pga(0);
        _dol_update_pga(1);
        _dol_update_sw();
        __dol_update_ldo_vol(s_dol.cur_vol);
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_OPENED);
        if( s_dol.is_dsp_ctl )
        {
            __dol_switch_to(0);
        }
    }
    else
    {
        //SCI_MEMSET(&(s_dol.sw),0,sizeof(s_dol.sw));
        //s_dol.ad_sample_rate = 0;
        //s_dol.da_sample_rate = 0;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_CLOSEED);
    }
    DOLPHIN_DBG("_run_callback:%d",is_open);
    AUDIO_HAL_PRTMEM ( (0x55AA0000 
        | (is_open & 0x0000FFFF)    ));
}

PUBLIC void __dol_ext_hp_pa_ctl_with_mute(uint32 en)
{
    uint32 real_en = en;
    en = (en && !(s_dol.is_hp_mute));
    if( real_en && (en != real_en) && (s_dol.is_hp_next_enabled) )
        return;
    __dol_ext_hp_pa_ctl_inter(en);
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
