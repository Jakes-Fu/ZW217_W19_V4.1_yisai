/******************************************************************************
 ** File Name:      audio_dai_i2s.c                                           *
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
#ifdef I2S_DAI_DEBUG
#define I2S_DBG       SCI_TRACE_LOW
#define I2S_DBG_ASS   SCI_ASSERT
#else
#define I2S_DBG(...)
#define I2S_DBG_ASS(x)
#endif

#define I2S_PRT       SCI_TRACE_LOW


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    I2S Init.
//                  this is DAI Init etc.
// Return :         AUDIO_HAL_SUCCESS
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _I2S_Init(void)
{
}


/*****************************************************************************/
// Description :    I2S  Open.
//                  this is DAI Enable.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          fun_flg : from AUDIO_FUN_E
//                  fs : frame sample rate.
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _I2S_Open(uint32 fun_flg,uint32 fs)
{
}


/*****************************************************************************/
// Description :    I2S  Close.
//                  this is  DAI Disable.
// Return :         AUDIO_HAL_SUCCESS
// Param :          fun_flg : from AUDIO_FUN_E
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _I2S_Close(uint32 fun_flg)
{
}


/*****************************************************************************/
// Description :    I2S  Ioctl.
//                  this is  DAI Ioctl.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          cmd : from CODEC_CMD_E and DAI_CMD_E
//                  arg : some cmd maybe use NULL if no param
//                      if the cmd have param,please give a non-NULL point!
// Author :         ken.kuang
// Note :
/*****************************************************************************/
LOCAL int32 _I2S_Ioctl(uint32 cmd, uint32 arg)
{
}



/*****************************************************************************/
// Description :    I2S  Set Playback Buffer Info.
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
LOCAL int32 _I2S_DAC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 cnt)
{
}

LOCAL int32 _I2S_DAC_Set_Write_Point(uint32 pWrite)
{
}

LOCAL int32 _I2S_DAC_Get_Read_Point(uint32* pRead)
{
}


/*****************************************************************************/
// Description :    I2S  Will Trigger Playback immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _I2S_DAC_Trigger(void)
{
}


/*****************************************************************************/
// Description :    I2S  Capture Buffer Info.
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
LOCAL int32 _I2S_ADC_Set_Buf_Info(void *pLHead,void *pRHead,uint32 cnt)
{
}

LOCAL int32 _I2S_ADC_Set_Read_Point(uint32 pRead)
{
}

LOCAL int32 _I2S_ADC_Get_Write_Point(uint32* pWrite)
{
}

/*****************************************************************************/
// Description :    I2S  Will Trigger Captrue immediately.
//                  
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          
//                  
// Author :         ken.kuang
// Note :           do nothing when already trigger.
/*****************************************************************************/
LOCAL int32 _I2S_ADC_Trigger(void)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    
    
    return ret;
}

/*****************************************************************************/
// Description :    I2S  Register Playback Callback.
//                  Use to Notify User the Driver fetch data gone.
//                  User can do something like start decode audio data.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _I2S_Register_Playback(AUDIO_HAL_CALLBACK_FUN pFun)
{
}


/*****************************************************************************/
// Description :    I2S  Register Capture Callback.
//                  Use to Notify User for fetch data please.
//                  User should be move audio data ASAP.
// Return :         AUDIO_HAL_SUCCESS,AUDIO_HAL_ERROR
// Param :          pFun : this function.
//                  
// Author :         ken.kuang
// Note :  This Callback DO NOT call deep tree.
//          should be send message or set flag etc.
/*****************************************************************************/
LOCAL int32 _I2S_Register_Capture(AUDIO_HAL_CALLBACK_FUN pFun)
{
}




/**---------------------------------------------------------------------------*
 **                         LOCAL Data                                        *
 **---------------------------------------------------------------------------*/

LOCAL AUD_DAI_OPS_T s_audio_dai_i2s_ops=
{
    _I2S_Init,
    _I2S_Open,
    _I2S_Close,
    _I2S_Ioctl,
    _I2S_DAC_Set_Buf_Info,
    _I2S_DAC_Set_Write_Point,
    _I2S_DAC_Get_Read_Point,
    _I2S_DAC_Trigger,
    _I2S_ADC_Set_Buf_Info,
    _I2S_ADC_Set_Read_Point,
    _I2S_ADC_Get_Write_Point,
	_I2S_ADC_Trigger,
    _I2S_Register_Playback,
    _I2S_Register_Capture,
};

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC AUD_DAI_OPS_T* AUDIO_DAI_Get_OPS(uint32 param)
{
    return (AUD_DAI_OPS_T*)(&(s_audio_dai_i2s_ops));
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
