/******************************************************************************
 ** File Name:      audio_codec_lm49370.c                                    *
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
#include "audio_hal.h"

#include "LM49370_v3.h"

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
#define LM49370_DBG       SCI_TRACE_LOW
#define LM49370_DBG_ASS   SCI_ASSERT
#else
#define LM49370_DBG(...)
#define LM49370_DBG_ASS(x)
#endif

#define LM49370_PRT       SCI_TRACE_LOW

#define LM49370_ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

LOCAL INTERFACE_PROPERTY InterfacePro;
LOCAL IIS_CONFIG IisConfig;
LOCAL PCM_CONFIG PcmConfig;


#define IIS_PCM_SLAVE

void LM49370_Config(uint32 SampleRate)
{
    {
        InterfacePro.pIisConfig = &IisConfig;
        InterfacePro.pPcmConfig = &PcmConfig;

        //InterfacePro.Interface = INTERFACE_PCM;

        switch(SampleRate)
        {
        case 48000:
            InterfacePro.SampleRate = SAM_48000;
            break;

        case 44100:
            InterfacePro.SampleRate = SAM_44100;
            break;

        case 32000:
            InterfacePro.SampleRate = SAM_32000;
            break;
        case 22000:    
        case 22050:
            InterfacePro.SampleRate = SAM_22050;
            break;
        case 16000:
            InterfacePro.SampleRate = SAM_16000;
            break;

        case 8000:
            InterfacePro.SampleRate = SAM_8000;
            break;

        default:
            InterfacePro.SampleRate = SAM_44100;
            //LM49370_PRT:"LM49370_Config : Unconfigurable parameter!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_LM49370_100_112_2_17_23_2_46_150,(uint8*)"");
            break;
            
        }

        // ----- InterfacePro.pIisConfig
        //InterfacePro.pIisConfig->IisWSMode = BITS_PER_WORD_16;
    //ken.kuang modifyfor support 24bit.
        InterfacePro.pIisConfig->IisWSMode = BITS_PER_WORD_32;
        InterfacePro.pIisConfig->IisGrouping = IIS_GROUPING_RL;

        InterfacePro.pIisConfig->IisInputEnable = TRUE;
        InterfacePro.pIisConfig->IisOutputEnable = TRUE;

        InterfacePro.pIisConfig->IisMode = IIS_MODE_IIS_COMPATIBLE;

        InterfacePro.pIisConfig->IisClockMaster = TRUE;
        InterfacePro.pIisConfig->IisWSMaster = TRUE;

        InterfacePro.pIisConfig->IntOSCEnable = FALSE;				// Default = FALSE
        InterfacePro.pIisConfig->IisStereoReverse = FALSE;			// Default = FALSE

        // ----- InterfacePro.pPcmConfig
        InterfacePro.pPcmConfig->PcmBitPerChannel = PCM_BIT_16;

        InterfacePro.pPcmConfig->PcmClockMaster = TRUE;
        InterfacePro.pPcmConfig->PcmSyncMaster = TRUE;

        InterfacePro.pPcmConfig->PcmInputEnable = TRUE;
        InterfacePro.pPcmConfig->PcmOutputEnable = TRUE;

        InterfacePro.pPcmConfig->PcmSyncWidth = PCM_SYNC_WIDTH_1BIT;
        // InterfacePro.pPcmConfig->PcmSdoLSBHZEnable = FALSE;
        // InterfacePro.pPcmConfig->PcmMode = PCM_MODE_ALAW
        // InterfacePro.pPcmConfig->PcmCompandOn = FALSE;

        // ----- InterfacePro.BasicConfig
        InterfacePro.BasicConfig.ChipMode = ACTIVE_WTO_HP_DET;

        InterfacePro.BasicConfig.AUXLeft = VOLUME_LEVEL_MAX;
        InterfacePro.BasicConfig.AUXRight = VOLUME_LEVEL_MAX;
        InterfacePro.BasicConfig.DACLevel = VOLUME_LEVEL_MAX;

        InterfacePro.BasicConfig.LeftSrc = LEFT_SRC_DAC;
        InterfacePro.BasicConfig.RightSrc = RIGHT_SRC_DAC;

        InterfacePro.BasicConfig.EnableLS = TRUE;
        InterfacePro.BasicConfig.EnableHP = TRUE;
        InterfacePro.BasicConfig.ADCEnable = TRUE;

        InterfacePro.BasicConfig.StereoModeEnable = TRUE;

        // ----- InterfacePro.HeadsetMic
        InterfacePro.HeadsetMic.MICSelct = MIC_SEL_EXT;

        // ----- Bridge Configurations
        InterfacePro.IisPcmBridge.DacInputSel = DAC_INPUT_SEL_IIS_RECEIVER;
	}

#ifdef PCM_Test

        //InterfacePro.pIisConfig->IisInputEnable = FALSE;
        //InterfacePro.pIisConfig->IisOutputEnable = FALSE;
        //InterfacePro.pIisConfig->IisClockMaster = FALSE;
        //InterfacePro.pIisConfig->IisWSMaster = FALSE;

        InterfacePro.IisPcmBridge.DacInputSel = DAC_INPUT_SEL_PCM_RECEIVER;

#ifdef IIS_PCM_SLAVE
        InterfacePro.pPcmConfig->PcmClockMaster = TRUE;
        InterfacePro.pPcmConfig->PcmSyncMaster = TRUE;

#else
        InterfacePro.pPcmConfig->PcmClockMaster = FALSE;
        InterfacePro.pPcmConfig->PcmSyncMaster = FALSE;
#endif

#else       // PCM_Test

        InterfacePro.pPcmConfig->PcmInputEnable = FALSE;
        InterfacePro.pPcmConfig->PcmOutputEnable = FALSE;

#ifdef IIS_PCM_SLAVE
        InterfacePro.pIisConfig->IisClockMaster = TRUE;
        InterfacePro.pIisConfig->IisWSMaster = TRUE;


#else
        InterfacePro.pIisConfig->IisClockMaster = FALSE;
        InterfacePro.pIisConfig->IisWSMaster = FALSE;

#endif
	
#endif

	if(LM49370_SUCCESS != LM49370_SetMode(&InterfacePro))
	{
		//LM49370_PRT:"LM49370 set Fail!\r\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_LM49370_197_112_2_17_23_2_46_151,(uint8*)"");
		
		//return FALSE;
	}
}

/*****************************************************************************/
// Description :    LM49370 Init.
//                  this is Codec Init.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _LM49370_Init(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //LM49370_DBG:"[AUDDRV][LM49370]init lm49370"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_LM49370_211_112_2_17_23_2_46_152,(uint8*)"");
    
    return ret;
}

PUBLIC void     __vbc_dac_sel_iis(uint32 port);
PUBLIC void     __vbc_adc_sel_iis(uint32 port);

/*****************************************************************************/
// Description :    LM49370 Open.
//                  this Codec Power On.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _LM49370_Open(uint32 fun_flg,uint32 fs)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //LM49370_DBG:"[AUDDRV][LM49370]open lm49370 0x%x,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_LM49370_229_112_2_17_23_2_46_153,(uint8*)"dd",fun_flg,fs);

    __vbc_dac_sel_iis(2);
    __vbc_adc_sel_iis(2);

    if( fs )
    {
        LM49370_Config(fs);
    }
    
    __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_READY);
    __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_OPENED);

    return ret;
}

/*****************************************************************************/
// Description :    LM49370 Close.
//                  this is Codec Power Off.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _LM49370_Close(uint32 fun_flg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //LM49370_DBG:"[AUDDRV][LM49370]close lm49370 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_LM49370_254_112_2_17_23_2_46_154,(uint8*)"d",fun_flg);
    
    __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_CLOSEED);
    __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_Z);
    return ret;
}



/*****************************************************************************/
// Description :    LM49370 Ioctl.
//                  this is Codec Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _LM49370_Ioctl(uint32 cmd, uint32 arg)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    //LM49370_DBG:"[AUDDRV][LM49370]ioctl lm49370 %d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_LM49370_274_112_2_17_23_2_46_155,(uint8*)"dd",cmd,arg);

    switch(cmd)
    {
        case AUDIO_CTL_PGA:
        {
        }break;
        case AUDIO_CTL_SWITCH_DAC:
        {
        }break;
        case AUDIO_CTL_SWITCH_DRV_OUT:
        {
        }break;
        case AUDIO_CTL_SWITCH_DRV_IN:
        {
        }break;
        case AUDIO_GET_SUPPORT_FS:
        {
        }break;
        default:
        {
            //LM49370_PRT:"[AUDDRV][LM49370]ioctl param"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_CODEC_LM49370_295_112_2_17_23_2_46_156,(uint8*)"");
            ret = AUDIO_HAL_ERROR;
        }break;
    }

    return ret;
}

/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL const AUD_CODEC_OPS_T s_audio_codec_lm49370_ops=
{
    _LM49370_Init,
    _LM49370_Open,
    _LM49370_Close,
    _LM49370_Ioctl,
};


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_CODEC_OPS_T* AUDIO_CODEC_Get_LM49370_OPS(uint32 param)
{
    return (AUD_CODEC_OPS_T*)(&s_audio_codec_lm49370_ops);
}


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
