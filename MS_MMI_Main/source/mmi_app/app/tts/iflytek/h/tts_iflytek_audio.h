/*****************************************************************************
** File Name:      tts_iflytek_audio.h                                       *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      All Rights Reserved.                                      *
** Description:    Iflytek tts                                               *
*****************************************************************************/


#ifndef _TTS_IFLYTEK_AUDIO_H_
#define _TTS_IFLYTEK_AUDIO_H_ 


#include "mmk_type.h"
#include "sci_types.h"
#include "efs.h"
#include "ffs.h"
//#include "mmi_nv.h"
#include "tb_dal.h"
//#include "mmi_id.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"

//#include "mmirecord_export.h"
#ifndef _WIN32
#include "audio_api.h"
#else
#include "mmiaudio_simu.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/*****************************************************************************/
// 	Description : 创建PcmStreamHandle
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_CreatePcmStreamHandle(TTS_ALL_RING_TYPE_E     ring_type,AUDIO_GETSRCDATA_CALLBACK_PFUNC pCallbackfunc,MMISRVAUD_ROUTE_T all_support_route);

/*****************************************************************************/
// 	Description : stop callback
//	Global resource dependence :                                

//	Note:for_tts
/*****************************************************************************/
//PUBLIC BOOLEAN TTS_IsCreatePcmStreamHandle(void);

/*****************************************************************************/
// 	Description :  关停PcmStreamHandle
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTS_StopAndClosePcmStreamHandle(void);

/*****************************************************************************/
//  Description : 获得pcm stream id 值
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint32 TTS_GetPcmStreamId(void);

/*****************************************************************************/
// 	Description : judge the audio handle
//	Global resource dependence :                                
//	Note:for_tts
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_GetAudioIsOpen(void);

/*****************************************************************************/
//  Description : 获得audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_GetAudiohandle(void);

/*****************************************************************************/
//  Description : 设置audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_SetAudiohandle(MMISRV_HANDLE_T audio_handle);

/*****************************************************************************/
//  Description : notify handle function for TTS
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_HandleNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
