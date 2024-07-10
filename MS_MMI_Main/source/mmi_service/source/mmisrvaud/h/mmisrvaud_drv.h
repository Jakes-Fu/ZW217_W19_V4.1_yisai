/****************************************************************************
** File Name:      mmisrvaud_drv.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             27/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is to declare some definition about the audio package layer.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 27/04/2011       Yintang.ren         Create
** 
****************************************************************************/
#ifndef _MMISRVAUD_DRV_H_
#define _MMISRVAUD_DRV_H_

#include "mmisrvmgr.h"
#include "audio_api.h"
#include "mmisrvaud_api.h"

#define DRVAUD_HANDLE HAUDIO




/*****************************************************************************/
//  Description : Create a audio handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC DRVAUD_HANDLE DRVAUD_CreateHandle(MMISRVAUD_TYPE_U *audio_data, BOOLEAN is_a2dp, uint32 *buff, uint32 buff_size);

/*****************************************************************************/
//  Description : Create an audio handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_CloseHandle(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr);

/*****************************************************************************/
//  Description : Play audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_Play(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr, uint32 offset, MMISRVAUD_ROUTE_T route);

/*****************************************************************************/
//  Description : Play audio asynchronus..
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
#if 0
PUBLIC BOOLEAN DRVAUD_PlayAsy(DRVAUD_HANDLE handle, uint32 offset);
#else
PUBLIC BOOLEAN DRVAUD_PlayAsy(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr, uint32 offset);
#endif

/*****************************************************************************/
//  Description : Pause audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_Pause(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr);

/*****************************************************************************/
//  Description : Stop playing audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_Stop(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr);

/*****************************************************************************/
//  Description : Seek audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used for video.
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_PauseSlight(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr);

/*****************************************************************************/
//  Description : Seek audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used for video.
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_ResumeSlight(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr);

/*****************************************************************************/
//  Description : Resume audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_Resume(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr);

/*****************************************************************************/
//  Description : Seek audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_Seek(DRVAUD_HANDLE handle, uint32 offset);

/*****************************************************************************/
//  Description : Get content info
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetContentInfo(DRVAUD_HANDLE handle, MMISRVAUD_CONTENT_INFO_T *content_info);

/*****************************************************************************/
//  Description : Get playing info
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetPlayingInfo(DRVAUD_HANDLE handle, MMISRVAUD_PLAY_INFO_T *play_info);

/*****************************************************************************/
//  Description : Get record info
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetRecordInfo(DRVAUD_HANDLE handle, MMISRVAUD_RECORD_INFO_T *record_info);

/*****************************************************************************/
//  Description : Set play speed.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetPlaySpeed(DRVAUD_HANDLE handle, MMISRVAUD_SPEED_T speed);

/*****************************************************************************/
//  Description : Set EQ mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetEQMode(DRVAUD_HANDLE handle, MMISRVAUD_EQ_MODE_E eq_mode);

/*****************************************************************************/
//  Description : Get EQ mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_EQ_MODE_E DRVAUD_GetEQMode(DRVAUD_HANDLE handle);

/*****************************************************************************/
//  Description : Set volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetVolume(DRVAUD_HANDLE handle, MMISRVAUD_VOLUME_T volume);

/*****************************************************************************/
//  Description : Set volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetRoute(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr, MMISRVAUD_ROUTE_T route);

/*****************************************************************************/
//  Description : Get volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetVolume(DRVAUD_HANDLE handle, uint32 *volume);

/*****************************************************************************/
//  Description : Get  route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetRoute(DRVAUD_HANDLE handle, MMISRVAUD_ROUTE_T *route);

/*****************************************************************************/
//  Description : Get current audio configure.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetAudCfg(DRVAUD_HANDLE handle, MMISRVAUD_AUD_CFG_T *cfg);

/*****************************************************************************/
//  Description : Get current audio configure.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetSpectrum(DRVAUD_HANDLE handle, MMISRVAUD_SPECTRUM_INFO_T *info);

/*****************************************************************************/
//  Description : Get current audio configure.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_OpenSpectrum(DRVAUD_HANDLE handle);

/*****************************************************************************/
//  Description : Get current audio configure.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_CloseSpectrum(DRVAUD_HANDLE handle);

/*****************************************************************************/
//  Description : Get current audio configure.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetVoiceLB(DRVAUD_HANDLE handle, BOOLEAN is_enable);

/*****************************************************************************/
//  Description : Set all route mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetMute(DRVAUD_HANDLE handle, BOOLEAN is_mute);

/*****************************************************************************/
//  Description : Get if all route is mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetMute(DRVAUD_HANDLE handle, BOOLEAN *is_mute);

/*****************************************************************************/
//  Description : Set up link mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetUplinkMute(DRVAUD_HANDLE handle, BOOLEAN is_mute);

/*****************************************************************************/
//  Description : Set play rate.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_SetPlayRate(DRVAUD_HANDLE handle, MMISRVAUD_PLAY_RATE_E rate);

/*****************************************************************************/
//  Description : Get record db by handle.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetRecordDb(DRVAUD_HANDLE handle, uint32 *db);

/*****************************************************************************/
//  Description : Get record info
//  Global resource dependence : 
//  Author:lei.he
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_GetRecordSize(DRVAUD_HANDLE handle, MMISRVAUD_RECORD_SIZE_T *record_info);

/*****************************************************************************/
//  Description : Play MV.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_StartRecordAtv(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr, void *param);

/*****************************************************************************/
//  Description : Play MV.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN DRVAUD_StopRecordAtv(DRVAUD_HANDLE handle, MMISRVAUD_TYPE_U *type_ptr);

/*****************************************************************************/
//  Description : Handle audio callback message.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_HandleDrvCallBackMsg(
                    void *msg_ptr
                    );

#endif

