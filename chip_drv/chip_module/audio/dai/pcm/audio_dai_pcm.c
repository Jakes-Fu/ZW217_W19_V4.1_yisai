/******************************************************************************
 ** File Name:      audio_dai_pcm.c                                           *
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

#include "sci_types.h"
#include "audio_drvapi.h"
#include "audio_dai.h"

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
#ifdef PCM_DAI_DEBUG
#define PCM_DBG       SCI_TRACE_LOW
#define PCM_DBG_ASS   SCI_ASSERT
#else
#define PCM_DBG(...)
#define PCM_DBG_ASS(x)
#endif

#define PCM_PRT       SCI_TRACE_LOW


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    PCM Init.
//                  this is DAI Init etc.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _PCM_Init(void)
{
}


/*****************************************************************************/
// Description :    PCM  Open.
//                  this is DAI Enable.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _PCM_Open(uint32 fun_flg,uint32 fs)
{
}


/*****************************************************************************/
// Description :    PCM  Close.
//                  this is  DAI Disable.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _PCM_Close(uint32 fun_flg)
{
}


/*****************************************************************************/
// Description :    PCM  Ioctl.
//                  this is  DAI Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _PCM_Ioctl(uint32 cmd, uint32 arg)
{
}



/*****************************************************************************/
// Description :    PCM  Set Playback Buffer Info.
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
//                  cnt : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _PCM_DAC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 cnt)
{
}

LOCAL int32 _PCM_DAC_Set_Write_Point(uint32 pWrite)
{
}

LOCAL int32 _PCM_DAC_Get_Read_Point(uint32* pRead)
{
}


/*****************************************************************************/
// Description :    PCM  Will Trigger Playback immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _PCM_DAC_Trigger(void)
{
}


/*****************************************************************************/
// Description :    PCM  Capture Buffer Info.
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
//                  cnt : the max size of pLHead. 
//                      well, pRHead MUST the same size with pLHead if exist.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _PCM_ADC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 cnt)
{
}

LOCAL int32 _PCM_ADC_Set_Read_Point(uint32 pRead)
{
}

LOCAL int32 _PCM_ADC_Get_Write_Point(uint32* pWrite)
{
}

/*****************************************************************************/
// Description :    PCM  Will Trigger Captrue immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _PCM_ADC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    
    return ret;
}

/*****************************************************************************/
// Description :    PCM  Register Playback Callback.
//                  Use to Notify User the Driver fetch data gone.
//                  User can do something like start decode audio data.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _PCM_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun)
{
}


/*****************************************************************************/
// Description :    PCM  Register Capture Callback.
//                  Use to Notify User for fetch data please.
//                  User should be move audio data ASAP.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _PCM_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun)
{
}




/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL AUD_DAI_OPS_T s_audio_dai_pcm_ops=
{
    _PCM_Init,
    _PCM_Open,
    _PCM_Close,
    _PCM_Ioctl,
    _PCM_DAC_Set_Buf_Info,
    _PCM_DAC_Set_Write_Point,
    _PCM_DAC_Get_Read_Point,
    _PCM_DAC_Trigger,
    _PCM_ADC_Set_Buf_Info,
    _PCM_ADC_Set_Read_Point,
    _PCM_ADC_Get_Write_Point,
	_PCM_ADC_Trigger,
    _PCM_Register_Playback,
    _PCM_Register_Capture,
};

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_DAI_OPS_T* AUDIO_DAI_Get_OPS(uint32 param)
{
    return (AUD_DAI_OPS_T*)(&(s_audio_dai_pcm_ops));
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_AUDIO_DAI_H_
// End
