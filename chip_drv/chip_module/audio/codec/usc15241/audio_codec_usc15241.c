/******************************************************************************
 ** File Name:      audio_codec_usc15241.c                                    *
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
#include "usc15241_phy_cfg.h"
#include "usc15241_state.h"
#include "usc15241.h"

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
#define USC15241_DBG       SCI_TRACE_LOW
#define USC15241_DBG_ASS   SCI_ASSERT
#else
#define USC15241_DBG(...)
#define USC15241_DBG_ASS(x)
#endif

#define USC15241_PRT       SCI_TRACE_LOW

#define USC15241_ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

enum USC15241_OC_E{
    USC15241_OPEN    = 0,
    USC15241_CLOSE
};

typedef struct
{
    uint32                  fun_flg;
    uint32                  fun_usc15241;
} USC15241_FUN_T;

typedef struct
{
    uint32                  adc_gim:1;      //GIM:1
    uint32                  adc_gi:4;       //GI:4
    uint32                  lineinrec:3;    //SPRD ADD
    uint32                  dac:4;
    uint32                  linein:5;
    uint32                  hp:5;
    uint32                  ear:1;
    uint32                  RSV:9;
} USC15241_PGA_T;

typedef struct
{
    uint32                  is_ep_on;
    uint32                  is_hp_on;
    uint32                  is_spk_on;

    uint32                  mic_on[2];
    uint32                  ail_on;
    uint32                  air_on;
} USC15241_SWITCH_T;

typedef struct
{
    char                    magic_str[AUDIO_MAGIC_STR_MAX_LEN];
    uint32                  da_sample_rate;
    uint32                  ad_sample_rate;

    USC15241_PGA_T          pga_0;
    USC15241_PGA_T          pga_1;
    USC15241_SWITCH_T       sw;
    
    uint32                  magic_num;
} USC15241_T;


/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL USC15241_T s_usc15241;
LOCAL const USC15241_FUN_T s_usc15241_support_fun[]=
{
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_PLAYBACK))
                                ,USC15241_FUN_PLAYBACK   },
    {((uint32)(AUDIO_FUN_DSP|AUDIO_FUN_CAPTRUE))
                                ,USC15241_FUN_CAPTURE    },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_PLAYBACK))
                                ,USC15241_FUN_LINEIN     },
    {((uint32)(AUDIO_FUN_LINEIN|AUDIO_FUN_CAPTRUE))
                                ,USC15241_FUN_LINEIN_REC },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_PLAYBACK))
                                ,USC15241_FUN_PLAYBACK   },
    {((uint32)(AUDIO_FUN_INT_FM|AUDIO_FUN_CAPTRUE))
                                ,0                  },
};
LOCAL const uint32 s_usc15241_support_fun_len=USC15241_ARR_SIZE(s_usc15241_support_fun);

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

LOCAL void _usc15241_update_pga(uint32 uiChannel)
{
}

LOCAL void _usc15241_update_sw(void)
{
    __usc15241_ext_pa_ctl(s_usc15241.sw.is_spk_on);
    __usc15241_mic_en(s_usc15241.sw.mic_on[0],0);
    __usc15241_mic_en(s_usc15241.sw.mic_on[1],1);
}


LOCAL uint32 _USC15241_Fun_Flg_Is_Eq(uint32 src_flg,uint32 dst_flg)
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

LOCAL int32 _USC15241_Parse_Fun_Flg(uint32 fun_flg,uint32 oc)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 i;
    uint32 fun_deal = (fun_flg);

    if( (SPRD_CODEC_CTRL_BY_DSP & fun_flg) )
    {
        USC15241_ChangeSt(USC15241_FUN_LOUT,oc);
        AUDIO_HAL_Ioctl(AUDIO_CTL_SWITCH_DAC,(oc?(AUDIO_MUTE):(AUDIO_UNMUTE)));
    }
    
    if( (~(AUDIO_FUN_PLAYBACK|AUDIO_FUN_CAPTRUE)
        & fun_deal)    )
    {
        for(i=0;i<s_usc15241_support_fun_len;i++)
        {
            if( _USC15241_Fun_Flg_Is_Eq((s_usc15241_support_fun[i].fun_flg) , fun_flg) )
            {
                if( s_usc15241_support_fun[i].fun_usc15241 )
                {
                    USC15241_ChangeSt((s_usc15241_support_fun[i].fun_usc15241),oc);
                }
            }
        }
    }
    else
    {
        if( AUDIO_FUN_PLAYBACK & fun_deal)
        {
            USC15241_ChangeSt(USC15241_FUN_PLAYBACK,oc);
        }
        if( AUDIO_FUN_CAPTRUE & fun_deal)
        {
            USC15241_ChangeSt(USC15241_FUN_CAPTURE,oc);
        }
    }

    
    return ret;
}


/*****************************************************************************/
// Description :    USC15241 Init.
//                  this is Codec Init.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _USC15241_Init(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //USC15241_DBG:"[AUDDRV][USC15241]init usc15241"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_212_112_2_17_23_3_1_242,(uint8*)"");
    if( AUDIO_MAGIC_NUM != s_usc15241.magic_num )
    {
        SCI_MEMSET(&(s_usc15241),0,sizeof(s_usc15241));
        s_usc15241.magic_num = AUDIO_MAGIC_NUM;
        SCI_MEMCPY(s_usc15241.magic_str,AUDIO_MAGIC_STR,AUDIO_MAGIC_STR_MAX_LEN);
        
        __usc15241_create_task();
        __usc15241_init();
        USC15241_PGA_Init_Set(param);
    }
    return ret;
}

/*****************************************************************************/
// Description :    USC15241 Open.
//                  this Codec Power On.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _USC15241_Open(uint32 fun_flg,uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //USC15241_DBG:"[AUDDRV][USC15241]open usc15241 0x%x,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_236_112_2_17_23_3_1_243,(uint8*)"dd",fun_flg,fs);

    if( AUDIO_HAL_SUCCESS != _USC15241_Parse_Fun_Flg(fun_flg,USC15241_OPEN) )
    {
        //USC15241_PRT:"[AUDDRV][USC15241]open err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_240_112_2_17_23_3_1_244,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }

    if( AUDIO_FUN_PLAYBACK & fun_flg )
    {
        s_usc15241.da_sample_rate = fs;
    }
    if( AUDIO_FUN_CAPTRUE & fun_flg )
    {
        s_usc15241.ad_sample_rate = fs;
    }

    if( (SPRD_CODEC_CTRL_BY_DSP & fun_flg) )
    {
    }

    //SC6530/SC6531 DSP AUD I2S PORT 1 CONNECT TO VBC I2S PORT 1
    if( (AUDIO_FUN_DSP & fun_flg) )
    {
        __sprd_codec_dp_i2s_sel(AUDIO_PLAYBACK,1);
        __sprd_codec_dp_i2s_sel(AUDIO_CAPTRUE,1);
    }
    else
    {
        __sprd_codec_dp_i2s_sel(AUDIO_PLAYBACK,0);
        __sprd_codec_dp_i2s_sel(AUDIO_CAPTRUE,0);
    }
    
    USC15241_SM_Process();
    return ret;
}

/*****************************************************************************/
// Description :    USC15241 Close.
//                  this is Codec Power Off.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _USC15241_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //USC15241_DBG:"[AUDDRV][USC15241]close usc15241 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_270_112_2_17_23_3_1_245,(uint8*)"d",fun_flg);
    
    if( AUDIO_HAL_SUCCESS != _USC15241_Parse_Fun_Flg(fun_flg,USC15241_CLOSE) )
    {
        //USC15241_PRT:"[AUDDRV][USC15241]close err %08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_274_112_2_17_23_3_1_246,(uint8*)"d",fun_flg);
        return AUDIO_HAL_ERROR;
    }

    USC15241_SM_Process();
    
    return ret;
}

LOCAL int32 _USC15241_Ioctl(uint32 cmd, uint32 arg);

LOCAL int32 _USC15241_Set_Drv_out(AUDIO_CTL_PGA_T * pga,uint32 mute)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_SWITCH_T sw = {0};
    sw.id = pga->id;
    sw.chan = pga->chan;
    sw.mode = (pga->mode&(AUDIO_SPK|AUDIO_EAR|AUDIO_HP));
    sw.val = mute;
    ret = _USC15241_Ioctl(AUDIO_CTL_SWITCH_DRV_OUT,(uint32)(&sw));
    return ret;
}

LOCAL int32 _USC15241_Ioctl_PGA(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_PGA_T *pga = 0;
    USC15241_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg)  && (0==ioctl_arg->pga.id)  ) //lint !e774
    {
        pga = &(ioctl_arg->pga);
        //if( (pga->level) )
        //{
            ret = USC15241_PGA_Set(pga);
        //}
        //else
        //{
        //    _USC15241_Set_Drv_out(pga,AUDIO_MUTE);
        //}
    }
    else
    {
        //USC15241_PRT:"[AUDDRV][USC15241]pga param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_316_112_2_17_23_3_1_247,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL void _USC15241_Lout_St(void)
{
    if( (0==s_usc15241.sw.is_spk_on) && (0==s_usc15241.sw.is_ep_on) )
    {
        USC15241_ChangeSt(USC15241_FUN_LOUT,1);
    }
    else
    {
        USC15241_ChangeSt(USC15241_FUN_LOUT,0);
    }
    USC15241_SM_Process();
}

LOCAL int32 _USC15241_Ioctl_Drv_Out_Ctl(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    USC15241_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg) && (0==ioctl_arg->sw.id) ) //lint !e774
    {
        //USC15241_DBG:"[AUDDRV][USC15241]drv out ctl 0x%x,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_342_112_2_17_23_3_1_248,(uint8*)"dd",ioctl_arg->sw.mode,ioctl_arg->sw.val);
        switch(ioctl_arg->sw.mode)
        {
            case AUDIO_SPK:
            {
                s_usc15241.sw.is_spk_on = ioctl_arg->sw.val;  //1
                _USC15241_Lout_St();                         //2
                if( USC15241_IsOnStop() )
                {
                    __usc15241_ext_pa_ctl(s_usc15241.sw.is_spk_on);
                }
            }break;
            case AUDIO_EAR:
            {
                s_usc15241.sw.is_ep_on = ioctl_arg->sw.val;   //1
                _USC15241_Lout_St();                         //2
            }break;
            case AUDIO_HP:
            {
                s_usc15241.sw.is_hp_on = ioctl_arg->sw.val;
                USC15241_ChangeSt(USC15241_FUN_HP,(uint32)(!(ioctl_arg->sw.val)));
                USC15241_SM_Process();
                if( USC15241_IsOnStop() )
                {
                    __usc15241_ext_hp_pa_ctl(s_usc15241.sw.is_hp_on);
                }
            }break;
            default:
            {
                //USC15241_PRT:"[AUDDRV][USC15241]drv out 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_371_112_2_17_23_3_1_249,(uint8*)"d",ioctl_arg->sw.mode);
                ret = AUDIO_HAL_ERROR;
            }break;
        }
    }
    else
    {
        //USC15241_PRT:"[AUDDRV][USC15241]drv out param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_378_112_2_17_23_3_1_250,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _USC15241_Ioctl_Drv_In_Ctl(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    USC15241_DBG_ASS(ioctl_arg);/*assert verified*/
    if( (ioctl_arg)  && (ioctl_arg->sw.id<2) ) //lint !e774
    {
        switch(ioctl_arg->sw.mode)
        {
            case AUDIO_MIC:
            {
                s_usc15241.sw.mic_on[ioctl_arg->sw.id] = ioctl_arg->sw.val;
                if( USC15241_IsOnStop() )
                {
                    __usc15241_mic_en(ioctl_arg->sw.val,ioctl_arg->sw.id);
                }
            }break;
            case AUDIO_HP_MIC:
            {
                ///TODO: LINE
                ret = AUDIO_HAL_ERROR;
            }break;
            case AUDIO_LINEIN:
            {
                ///TODO: LINE
                ret = AUDIO_HAL_ERROR;
            }break;
            default:
            {
                //USC15241_PRT:"[AUDDRV][USC15241]drv in 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_413_112_2_17_23_3_2_251,(uint8*)"d",ioctl_arg->sw.mode);
                ret = AUDIO_HAL_ERROR;
            }break;
        }
    }
    else
    {
        //USC15241_PRT:"[AUDDRV][USC15241]drv in param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_420_112_2_17_23_3_2_252,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _USC15241_Ioctl_Dac_Mute(uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    USC15241_DBG_ASS((arg==AUDIO_MUTE) || (arg==AUDIO_UNMUTE));/*assert verified*/
    if(arg==AUDIO_MUTE)
    {
        USC15241_ChangeSt(USC15241_FUN_DAC_UNMUTE,1);
        USC15241_SM_Process();
    }
    else if(arg==AUDIO_UNMUTE)
    {
        USC15241_ChangeSt(USC15241_FUN_DAC_UNMUTE,0);
        USC15241_SM_Process();
    }
    else
    {
        //USC15241_PRT:"[AUDDRV][USC15241]dac mute"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_443_112_2_17_23_3_2_253,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}


LOCAL int32 _USC15241_Ioctl_Get_FS(AUDIO_CTL_ARG_U * ioctl_arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    USC15241_DBG_ASS(ioctl_arg);/*assert verified*/
    if( ioctl_arg ) //lint !e774
    {
        ioctl_arg->dwVal = __usc15241_calc_fs(ioctl_arg->dwVal);
    }
    else
    {
        //USC15241_PRT:"[AUDDRV][USC15241]get fs param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_460_112_2_17_23_3_2_254,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    return ret;
}



/*****************************************************************************/
// Description :    USC15241 Ioctl.
//                  this is Codec Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _USC15241_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    AUDIO_CTL_ARG_U * ioctl_arg = (AUDIO_CTL_ARG_U *)arg;
    //USC15241_DBG:"[AUDDRV][USC15241]ioctl usc15241 %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_480_112_2_17_23_3_2_255,(uint8*)"dd",cmd,arg);

    switch(cmd)
    {
        case AUDIO_CTL_PGA:
        {
            ret = _USC15241_Ioctl_PGA(ioctl_arg);
        }break;
        case AUDIO_CTL_SWITCH_DAC:
        {
            ret = _USC15241_Ioctl_Dac_Mute(arg);
        }break;
        case AUDIO_CTL_SWITCH_DRV_OUT:
        {
            ret = _USC15241_Ioctl_Drv_Out_Ctl(ioctl_arg);
        }break;
        case AUDIO_CTL_SWITCH_DRV_IN:
        {
            ret = _USC15241_Ioctl_Drv_In_Ctl(ioctl_arg);
        }break;
        case AUDIO_GET_SUPPORT_FS:
        {
            ret = _USC15241_Ioctl_Get_FS(ioctl_arg);
        }break;
        default:
        {
            //USC15241_PRT:"[AUDDRV][USC15241]ioctl param"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_506_112_2_17_23_3_2_256,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }break;
    }

    return ret;
}



LOCAL int32 _USC15241_Set_PGA_GOD(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //USC15241_DBG:"[AUDDRV][USC15241]GOD %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_519_112_2_17_23_3_2_257,(uint8*)"dd",pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_usc15241.pga_0.dac = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_usc15241.pga_1.dac = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    _USC15241_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( USC15241_IsOnStop() )
    {
        //__usc15241_set_pag_lrgod(0,s_usc15241.pga_0.dac,s_usc15241.pga_1.dac);
        //__usc15241_set_pag_lrgod(1,s_usc15241.pga_0.dac,s_usc15241.pga_1.dac);
    }
    return ret;
}


LOCAL int32 _USC15241_Set_PGA_GO(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //USC15241_DBG:"[AUDDRV][USC15241]GO %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_543_112_2_17_23_3_2_258,(uint8*)"dd",pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_usc15241.pga_0.hp = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_usc15241.pga_1.hp = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    _USC15241_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( USC15241_IsOnStop() )
    {
        //__usc15241_set_pag_lrgo(0,s_usc15241.pga_0.hp,s_usc15241.pga_1.hp);
        //__usc15241_set_pag_lrgo(1,s_usc15241.pga_0.hp,s_usc15241.pga_1.hp);
    }
    return ret;
}

LOCAL int32 _USC15241_Set_PGA_GOBTL(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //USC15241_DBG:"[AUDDRV][USC15241]GOBTL %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_566_112_2_17_23_3_2_259,(uint8*)"dd",pga->level,pga->chan);
    s_usc15241.pga_0.ear = pga->level;
    s_usc15241.pga_1.ear = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    _USC15241_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( USC15241_IsOnStop() )
    {
        //__usc15241_set_pag_gobtl(s_usc15241.pga_0.ear);
    }
    return ret;
}

LOCAL int32 _USC15241_Set_PGA_GOB(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //USC15241_DBG:"[AUDDRV][USC15241]GOB %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_581_112_2_17_23_3_2_260,(uint8*)"dd",pga->level,pga->chan);
    if(AUDIO_IS_CHAN_L(pga->chan))
    {
        s_usc15241.pga_0.linein = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    if(AUDIO_IS_CHAN_R(pga->chan))
    {
        s_usc15241.pga_1.linein = pga->level;
        ret = AUDIO_HAL_SUCCESS;
    }
    _USC15241_Set_Drv_out(pga,AUDIO_UNMUTE);
    if( USC15241_IsOnStop() )
    {
        //__usc15241_set_pag_lrgob(0,s_usc15241.pga_0.linein,s_usc15241.pga_1.linein);
        //__usc15241_set_pag_lrgob(1,s_usc15241.pga_0.linein,s_usc15241.pga_1.linein);
    }
    return ret;
}

LOCAL int32 _USC15241_Set_PGA_GIM(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //USC15241_DBG:"[AUDDRV][USC15241]GIM %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_604_112_2_17_23_3_2_261,(uint8*)"dd",pga->level,pga->chan);
    s_usc15241.pga_0.adc_gim = pga->level;
    s_usc15241.pga_1.adc_gim = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    if( USC15241_IsOnStop() )
    {
        //__usc15241_set_pag_gim(s_usc15241.pga_0.adc_gim);
    }
    return ret;
}

LOCAL int32 _USC15241_Set_PGA_GI(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //USC15241_DBG:"[AUDDRV][USC15241]GI %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_618_112_2_17_23_3_2_262,(uint8*)"dd",pga->level,pga->chan);
    s_usc15241.pga_0.adc_gi = pga->level;
    s_usc15241.pga_1.adc_gi = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    if( USC15241_IsOnStop() )
    {
        //__usc15241_set_pag_gi(s_usc15241.pga_0.adc_gi);
    }
    return ret;
}

LOCAL int32 _USC15241_Set_PGA_GOLR(AUDIO_CTL_PGA_T* pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    //USC15241_DBG:"[AUDDRV][USC15241]GOLR %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_632_112_2_17_23_3_2_263,(uint8*)"dd",pga->level,pga->chan);
    s_usc15241.pga_0.lineinrec = pga->level;
    s_usc15241.pga_1.lineinrec = pga->level;
    ret = AUDIO_HAL_SUCCESS;
    if( USC15241_IsOnStop() )
    {
        //__usc15241_set_pag_golr(s_usc15241.pga_0.lineinrec);
    }
    return ret;
}




/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL const AUD_CODEC_OPS_T s_audio_codec_usc15241_ops=
{
    _USC15241_Init,
    _USC15241_Open,
    _USC15241_Close,
    _USC15241_Ioctl,
};

LOCAL const USC15241_PGA_FUN_T s_usc15241_pga_set_ops[]=
{
    {USC15241_PGA_GOD           ,_USC15241_Set_PGA_GOD    },
    {USC15241_PGA_GO            ,_USC15241_Set_PGA_GO     },
    {USC15241_PGA_GOBTL         ,_USC15241_Set_PGA_GOBTL  },
    {USC15241_PGA_GOB           ,_USC15241_Set_PGA_GOB    },
    {USC15241_PGA_GIM           ,_USC15241_Set_PGA_GIM    },
    {USC15241_PGA_GI            ,_USC15241_Set_PGA_GI     },
    {USC15241_PGA_GOLR          ,_USC15241_Set_PGA_GOLR   },
    {0                     ,0                       },
}; 


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_CODEC_OPS_T* AUDIO_CODEC_Get_USC15241_OPS(uint32 param)
{
    return (AUD_CODEC_OPS_T*)(&s_audio_codec_usc15241_ops);
}

PUBLIC USC15241_PGA_FUN_T* USC15241_Get_PGA_OPS(uint32 param)
{
    return (USC15241_PGA_FUN_T*)(&s_usc15241_pga_set_ops);
}

PUBLIC void __usc15241_run_callback(uint32 is_open)
{
    if( is_open )
    {
        if( s_usc15241.da_sample_rate )
        {
            __sprd_codec_dp_dac_fs_mode_set(s_usc15241.da_sample_rate);
        }
        if( s_usc15241.ad_sample_rate )
        {
            __sprd_codec_dp_adc_fs_mode_set(s_usc15241.ad_sample_rate);
        }
        _usc15241_update_pga(0);
        _usc15241_update_pga(1);
        _usc15241_update_sw();
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_OPENED);
    }
    else
    {
        SCI_MEMSET(&(s_usc15241.sw),0,sizeof(s_usc15241.sw));
        s_usc15241.ad_sample_rate = 0;
        s_usc15241.da_sample_rate = 0;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_CLOSEED);
    }
    //USC15241_PRINT:"_run_callback:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_USC15241_706_112_2_17_23_3_2_264,(uint8*)"d",is_open);
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
