/*****************************************************************************
** File Name:      TTS_audio.h                                                                                        *
** Author:                                                                                                                    *
** Date:           2007.5.8                                                                                               *
** Copyright:       All Rights Reserved.                                                                            *
** Description:                                                                                                             *
*****************************************************************************/


#ifndef _TTS_AUDIO_H_
#define _TTS_AUDIO_H_ 


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
// 	Description : ´´½¨PcmStreamHandle
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMITTS_CreatePcmStreamHandle(MMISET_ALL_RING_TYPE_E     ring_type,AUDIO_GETSRCDATA_CALLBACK_PFUNC pCallbackfunc);

/*****************************************************************************/
// 	Description : stop callback
//	Global resource dependence :                                

//	Note:for_tts
/*****************************************************************************/
PUBLIC BOOLEAN MMITTS_IsCreatePcmStreamHandle(void);

/*****************************************************************************/
// 	Description :  ¹ØÍ£PcmStreamHandle
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMITTS_StopAndClosePcmStreamHandle(MMISET_ALL_RING_TYPE_E     ring_type);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
