/****************************************************************************
** File Name:      mmisrvaud.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             22/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the data struct of                 
**                     AUDIO service module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 22/04/2011       Yintang.ren         Create
** 
****************************************************************************/
#ifndef _MMISRVAUD_H_
#define _MMISRVAUD_H_

#include "mmisrvmgr.h"
#include "mmisrvaud_api.h"

#define MMISRVAUD_SERVICE_NUM_MAX   50
#define MMISRVAUD_PRI_NUM_MAX   65535
#define MMISRVAUD_SAMPLE_RATE_DEFAULT    0 //32000
#define MMISRVAUD_SAMPLE_RATE_ATV    8000
#define MMISRVAUD_SAMPLE_RATE_MV     8000
typedef enum
{
    MMISRVAUD_EVENT_NONE,
    MMISRVAUD_EVENT_PLAY,
    MMISRVAUD_EVENT_PLAY_ASY,
    MMISRVAUD_EVENT_PAUSE,
    MMISRVAUD_EVENT_RESUME,
    MMISRVAUD_EVENT_STOP,
    MMISRVAUD_EVENT_PAUSE_SLIGHTLY,
    MMISRVAUD_EVENT_RESUME_SLIGHTLY,
    MMISRVAUD_EVENT_SEEK,
    MMISRVAUD_EVENT_SET_AUDIOINFO,
    MMISRVAUD_EVENT_GET_CONTENT_INFO,
    MMISRVAUD_EVENT_GET_PLAYING_INFO,
    MMISRVAUD_EVENT_GET_RECORD_INFO,
    MMISRVAUD_EVENT_GET_RECORD_SIZE, 
    MMISRVAUD_EVENT_SET_PLAY_SPEED,
    MMISRVAUD_EVENT_SET_EQ_MODE,
    MMISRVAUD_EVENT_GET_VOLUME,
    MMISRVAUD_EVENT_SET_VOLUME,
    MMISRVAUD_EVENT_GET_ROUTE,
    MMISRVAUD_EVENT_SET_ROUTE,
    MMISRVAUD_EVENT_SET_ALL_SURPPORT_ROUTE,
    MMISRVAUD_EVENT_SET_MANUAL_ROUTE,
    MMISRVAUD_EVENT_IS_SET_ROUTE_MANULLY,    
    MMISRVAUD_EVENT_SET_TIMES,
    MMISRVAUD_EVENT_IS_AUDIO_END,
    MMISRVAUD_EVENT_GET_SPECTRUM,
    MMISRVAUD_EVENT_OPEN_SPECTRUM,
    MMISRVAUD_EVENT_CLOSE_SPECTRUM,
    MMISRVAUD_EVENT_SET_VOICE_LB,
    MMISRVAUD_EVENT_GET_RECORD_DB,
    MMISRVAUD_EVENT_CHECK_IS_PLAYING,
    MMISRVAUD_EVENT_SET_ATV_SERVICE_ID,
    MMISRVAUD_EVENT_SET_ATV_REGION_ID,
    MMISRVAUD_EVENT_START_RECORD_ATV,
    MMISRVAUD_EVENT_STOP_RECORD_ATV,
    MMISRVAUD_EVENT_GET_SRV_PRI,
    MMISRVAUD_EVENT_CLOSE,
    
    MMISRVAUD_EVENT_MAX
}MMISRVAUD_EVENT_E;

typedef struct
{
    MMISRVAUD_EVENT_E event;
    uint32 data1;
    uint32 data2;
}MMISRVAUD_EVENT_T;

typedef struct
{
    MMISRVAUD_EVENT_E event;
    MMISRV_HANDLE_T handle;
    uint32 data1;
    uint32 data2;
    BOOLEAN res;    /* 或者定义一组RESUL 的枚举*/
}MMISRVAUD_EVENT_RES_T;

#if 0
typedef struct
{
    MMISRVAUD_EVENT_E event;
    uint32 offset;
}MMISRVAUD_EVENT_SEEK_T;

typedef struct
{
    MMISRVAUD_EVENT_E event;
    MMISRVAUD_TYPE_T *audio_info;
}MMISRVAUD_EVENT_SET_AUDIOINFO_T;

typedef struct
{
    MMISRVAUD_EVENT_E event;
    MMISRVAUD_CONTENT_INFO_T *audio_info;
}MMISRVAUD_EVENT_GET_CONTENT_INFO_T;

typedef struct
{
    MMISRVAUD_EVENT_E event;
    MMISRVAUD_PLAY_INFO_T *playing_info;
}MMISRVAUD_EVENT_GET_PLAYING_INFO_T;

typedef struct
{
    MMISRVAUD_EVENT_E event;
    MMISRVAUD_RECORD_INFO_T *record_info;
}MMISRVAUD_EVENT_GET_RECORD_INFO_T;

#endif



/*-----------------------------------------------------------------------------*/
typedef enum
{
    MMISRVAUD_EXT_CMD_NONE,
    MMISRVAUD_EXT_CMD_PLAY,
    MMISRVAUD_EXT_CMD_PAUSE,
    MMISRVAUD_EXT_CMD_STOP,
    MMISRVAUD_EXT_CMD_RESUME,
    MMISRVAUD_EXT_CMD_SEEK,
    MMISRVAUD_EXT_CMD_DRV_CALLBACK,

    MMISRVAUD_EXT_CMD_SET_PLAY,
    
    MMISRVAUD_EXT_CMD_SET_EQMODE,    
    MMISRVAUD_EXT_CMD_GET_VOLUME,
    MMISRVAUD_EXT_CMD_SET_VOLUME,
    MMISRVAUD_EXT_CMD_GET_ROUTE,
    MMISRVAUD_EXT_CMD_SET_ROUTE,
    MMISRVAUD_EXT_CMD_SET_SPEED,
    
    MMISRVAUD_EXT_CMD_GET_CFG,
    MMISRVAUD_EXT_CMD_GET_ACTIVE_HANDLE,
    MMISRVAUD_EXT_CMD_SET_MUTE,
    MMISRVAUD_EXT_CMD_GET_MUTE,
    MMISRVAUD_EXT_CMD_SET_UPLINK_MUTE,
    MMISRVAUD_EXT_CMD_SET_PLAY_RATE,
    MMISRVAUD_EXT_CMD_EXT_DEV_CHG,
    MMISRVAUD_EXT_CMD_BT_EVENT,
    MMISRVAUD_EXT_CMD_EARPHONE_EVENT,

    MMISRVAUD_EXT_CMD_GET_FILE_INFO,
    MMISRVAUD_EXT_CMD_GET_BUF_INFO,

    MMISRVAUD_EXT_CMD_GET_HANDLE_BY_NAME,    
    MMISRVAUD_EXT_CMD_GET_HANDLE_BY_VOLUME_TYPE,
    MMISRVAUD_EXT_CMD_VOLUME_ADJUST,
    MMISRVAUD_EXT_CMD_SET_VOLUME_BY_TYPE,    
    MMISRVAUD_EXT_CMD_GET_HANDLE_BY_DRV_HANDLE,    //通过audio驱动的handle获取mmi audio service的实例handle
    MMISRVAUD_EXT_CMD_GET_DRV_HANDLE_BY_SRV_HANDLE,  //通过mmi audio service的实例handle获取audio驱动的handle
    MMISRVAUD_EXT_CMD_MAX
}MMISRVAUD_EXT_CMD_E;

typedef struct
{
    MMISRVAUD_EXT_CMD_E cmd;
    uint32 data1;
    uint32 data2;
}MMISRVAUD_EXT_CMD_T;

typedef struct
{
    MMISRVAUD_EXT_CMD_E cmd;
    uint32 data1;
    uint32 data2;
    BOOLEAN res;
}MMISRVAUD_EXT_CMD_RES_T;


/*****************************************************************************/
//  Description : Regist audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAUD_RegistService(void);

/*****************************************************************************/
//  Description : Handle audio report message.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_HandleAudioReport(uint32 drv_handle, MMISRVAUD_REPORT_T *rpt_data);

/*****************************************************************************/
//  Description : Search drv handle by type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only to search Video, ATV, CMMB etc. type, whitch only one entity at the same time.
//           These function is only used to distinguish the different callback.
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_SearchHandleByType(MMISRVAUD_TYPE_E type);

/*****************************************************************************/
//  Description : Search drv handle by type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only to search Video, ATV, CMMB etc. type, whitch only one entity at the same time.
//           These function is only used to distinguish the different callback.
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_SearchHandleByHandle(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : change the default route of entity_ptr
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: change the default route of entity_ptr after requested handle
//           
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetDefaultRouteByHandle(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T audio_route);

/*****************************************************************************/
//  Description : set the audio state to play state  
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: this function is only used in video player
//           
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAudioPlayStateByHandle(MMISRV_HANDLE_T audio_handle) ;

/*****************************************************************************/
//  Description : set the audio state to pause state 
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: this function is only used in video player
//           
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAudioPauseStateByHandle(MMISRV_HANDLE_T audio_handle) ;


/*****************************************************************************/
//  Description : MMISRVAUD_GetDecoderDataLen
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only to search Video, ATV, CMMB etc. type, whitch only one entity at the same time.
//           These function is only used to distinguish the different callback.
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_GetDecoderDataLen(MMISRV_HANDLE_T handle);

#endif

