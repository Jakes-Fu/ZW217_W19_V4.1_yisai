/****************************************************************************
** File Name:      mmisrvaud_api.c                                                              *
** Author:          Yintang.ren                                                               *
** Date:             05/05/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is the API of audio service.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 05/05/2011       Yintang.ren         Create
** 
****************************************************************************/
#include "mmi_service_trc.h"
#include "mmisrv.h"
#include "mmisrvaud.h"
#include "mmisrvaud_api.h"
#include "caf.h"
#include "mmi_common.h"
#include "mmi_ring.h"


//LOCAL MMISRV_HANDLE_T s_mmsrvaud_gen_tone_handle = 0;
//LOCAL MMISRV_HANDLE_T s_mmsrvaud_dtmf_tone_handle = 0;
//LOCAL MMISRV_HANDLE_T s_mmsrvaud_custom_tone_handle = 0;

#ifndef MMI_ETWS_SUPPORT
#define DEFAULT_TONE_VOLUME  5
#else
#define DEFAULT_TONE_VOLUME  8
#endif

/*****************************************************************************/
//  Description : Play an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RET_E MMISRVAUD_Play(MMISRV_HANDLE_T audio_handle, uint32 offset)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_PLAY, 0, 0};
    MMISRVAUD_RET_E res = MMISRVAUD_RET_ERROR;
    MMISRVAUD_EVENT_RES_T res_data = {0};

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Play() entry, audio_handle=0x%x, offset=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_34_112_2_18_3_23_39_0,(uint8*)"dd", audio_handle, offset));
    
    data.data1 = offset;
    if(MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, &res_data))
    {
        res = res_data.data1;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Play() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_42_112_2_18_3_23_39_1,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Play an audio by handle async.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RET_E MMISRVAUD_PlayAsy(MMISRV_HANDLE_T audio_handle, uint32 offset)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_PLAY_ASY, 0, 0};
    MMISRVAUD_RET_E res = MMISRVAUD_RET_ERROR;
    MMISRVAUD_EVENT_RES_T res_data = {0};

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_PlayAsy() entry, audio_handle=0x%x, offset=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_57_112_2_18_3_23_39_2,(uint8*)"dd", audio_handle, offset));
    
    data.data1 = offset;
    if(MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, &res_data))
    {
        res = res_data.data1;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_PlayAsy() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_65_112_2_18_3_23_39_3,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Stop an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Stop(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_STOP, 0, 0};
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Stop() entry, audio_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_79_112_2_18_3_23_39_4,(uint8*)"d", audio_handle));
    
    res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Stop() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_83_112_2_18_3_23_39_5,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Pause an audio  by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Pause(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_PAUSE, 0, 0};
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Pause() entry, audio_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_97_112_2_18_3_23_39_6,(uint8*)"d", audio_handle));
    
    res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Pause() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_101_112_2_18_3_23_39_7,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Resume an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Resume(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_RESUME, 0, 0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Resume() entry, audio_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_115_112_2_18_3_23_40_8,(uint8*)"d", audio_handle));

    res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Resume() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_119_112_2_18_3_23_40_9,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Pause an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used for Video, to other audio type, it's useless.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_PauseSlightly(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_PAUSE_SLIGHTLY, 0, 0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_PauseSlightly() entry, audio_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_133_112_2_18_3_23_40_10,(uint8*)"d", audio_handle));

    res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_PauseSlightly() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_137_112_2_18_3_23_40_11,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Resume an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used for Video, to other audio type, it's useless.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_ResumeSlightly(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_RESUME_SLIGHTLY, 0, 0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_ResumeSlightly() entry, audio_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_151_112_2_18_3_23_40_12,(uint8*)"d", audio_handle));

    res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_ResumeSlightly() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_155_112_2_18_3_23_40_13,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Seek an audio by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Seek(MMISRV_HANDLE_T audio_handle, uint32 offset)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Seek() entry, audio_handle=0x%x, offset=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_169_112_2_18_3_23_40_14,(uint8*)"dd", audio_handle, offset));
    
    data.event = MMISRVAUD_EVENT_SEEK;
    data.data1 = offset;
 
    res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_Seek() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_176_112_2_18_3_23_40_15,(uint8*)"d", res));

    return res;    
}

/*****************************************************************************/
//  Description : Seek an audio by time.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SeekByTime(MMISRV_HANDLE_T audio_handle, uint32 time)
{
    BOOLEAN res = FALSE;
    MMISRVAUD_PLAY_INFO_T info = {0};
    uint32 offset = 0;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SeekByTime() entry, audio_handle=0x%x,time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_191_112_2_18_3_23_40_16,(uint8*)"dd", audio_handle, time));

    if(audio_handle > 0)
    {
        if(MMISRVAUD_GetPlayingInfo(audio_handle, &info))
        {
            if(info.total_time > 0 && info.total_data_length > 0)
            {
                offset = (uint32)((uint64)time * info.total_data_length / info.total_time);
                res = MMISRVAUD_Seek(audio_handle, offset);
            }
        }
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SeekByTime() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_205_112_2_18_3_23_40_17,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Set new audio  info by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAudioInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_TYPE_U *audio_info)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAudioInfo() entry, audio_handle=0x%x, audio_info=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_219_112_2_18_3_23_40_18,(uint8*)"dd", audio_handle, (uint32)audio_info));

    if(audio_handle != 0 && audio_info != PNULL)
    {
        data.event = MMISRVAUD_EVENT_SET_AUDIOINFO;
        data.data1 = (uint32)audio_info;
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAudioInfo() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_228_112_2_18_3_23_40_19,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Get an audio's info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetContentInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_CONTENT_INFO_T	*content_info_ptr)
{
    MMISRVAUD_EVENT_T data = {0};
    MMISRVAUD_EVENT_RES_T res_data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetContentInfo() entry, audio_handle=0x%x, content_info_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_243_112_2_18_3_23_40_20,(uint8*)"dd", audio_handle, (uint32)content_info_ptr));

    if(audio_handle != PNULL && content_info_ptr != PNULL)
    {
        data.event = MMISRVAUD_EVENT_GET_CONTENT_INFO;
        data.data1 = (uint32)content_info_ptr;
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, &res_data);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetContentInfo() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_252_112_2_18_3_23_40_21,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Get an audio playing info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetPlayingInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_PLAY_INFO_T *playing_info_ptr)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetPlayingInfo() entry, audio_handle=0x%x, playing_info_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_266_112_2_18_3_23_40_22,(uint8*)"dd", audio_handle, (uint32)playing_info_ptr));

    if(audio_handle != 0 && playing_info_ptr != PNULL)
    {
        data.event = MMISRVAUD_EVENT_GET_PLAYING_INFO;
        data.data1 = (uint32)playing_info_ptr;
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetPlayingInfo() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_275_112_2_18_3_23_40_23,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Get recording info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetRecordInfo(MMISRV_HANDLE_T audio_handle, MMISRVAUD_RECORD_INFO_T	*record_info_ptr)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRecordInfo() entry, audio_handle=0x%x, record_info_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_289_112_2_18_3_23_40_24,(uint8*)"dd", audio_handle, (uint32)record_info_ptr));

    if(audio_handle != 0 && record_info_ptr != PNULL)
    {
        data.event = MMISRVAUD_EVENT_GET_RECORD_INFO;
        data.data1 = (uint32)record_info_ptr;
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRecordInfo() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_298_112_2_18_3_23_40_25,(uint8*)"d", res));
    
    return TRUE;
}
/*****************************************************************************/
//  Description : Get recording size.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetRecordSize(MMISRV_HANDLE_T audio_handle, MMISRVAUD_RECORD_SIZE_T	*record_info_ptr)//recordwork
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRecordSize() entry, audio_handle=0x%x, record_info_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_311_112_2_18_3_23_40_26,(uint8*)"dd", audio_handle, (uint32)record_info_ptr));

    if(audio_handle != 0 && record_info_ptr != PNULL)
    {
        data.event = MMISRVAUD_EVENT_GET_RECORD_SIZE;
        data.data1 = (uint32)record_info_ptr;
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRecordSize() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_320_112_2_18_3_23_40_27,(uint8*)"d", res));
    
    return TRUE;
}
/*****************************************************************************/
//  Description : Set play speed.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetPlaySpeed(MMISRV_HANDLE_T audio_handle, uint32 speed)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetPlaySpeed() entry, audio_handle=0x%x, speed=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_333_112_2_18_3_23_40_28,(uint8*)"dd", audio_handle, (uint32)speed));

    if(audio_handle != 0)
    {
        data.event = MMISRVAUD_EVENT_SET_PLAY_SPEED;
        data.data1 = (uint32)speed;
        
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetPlaySpeed() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_343_112_2_18_3_23_40_29,(uint8*)"d", res));
    
    return res;
}

#ifdef MP3_EQ_SUPPORT
/*****************************************************************************/
//  Description : Set EQ mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetEQMode(MMISRV_HANDLE_T audio_handle, MMISRVAUD_EQ_MODE_E eq_mode)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetEQMode() entry, audio_handle=0x%x, eq_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_358_112_2_18_3_23_40_30,(uint8*)"dd", audio_handle, (uint32)eq_mode));

    if(audio_handle != PNULL)
    {
        data.event = MMISRVAUD_EVENT_SET_EQ_MODE;
        data.data1 = (uint32)eq_mode;
        
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetEQMode() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_368_112_2_18_3_23_40_31,(uint8*)"d", res));

    return res;
}
#endif

/*****************************************************************************/
//  Description : Get audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_VOLUME_T MMISRVAUD_GetVolume(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {0};
    MMISRVAUD_EVENT_RES_T res_data = {0};
    MMISRVAUD_VOLUME_T vol = MMISRVAUD_VOLUME_LEVEL_NONE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetVolume() entry, audio_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_384_112_2_18_3_23_40_32,(uint8*)"d", audio_handle));

    if(audio_handle != PNULL)
    {
        data.event = MMISRVAUD_EVENT_GET_VOLUME;
        if(MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, &res_data))            
        {
            vol = (MMISRVAUD_VOLUME_T)res_data.data1;
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetVolume() exit, vol=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_395_112_2_18_3_23_40_33,(uint8*)"d", vol));

    return vol;
}

/*****************************************************************************/
//  Description : Set audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVolume(MMISRV_HANDLE_T audio_handle, MMISRVAUD_VOLUME_T volume)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetVolume() entry, audio_handle=0x%x, volume=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_409_112_2_18_3_23_40_34,(uint8*)"dd", audio_handle, volume));

    if(audio_handle != 0 && volume <= MMISRVAUD_VOLUME_LEVEL_MAX)
    {
        data.event = MMISRVAUD_EVENT_SET_VOLUME;
        data.data1 = volume;
        
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetVolume() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_419_112_2_18_3_23_40_35,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Get audio current route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T MMISRVAUD_GetRoute(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {0};
    MMISRVAUD_EVENT_RES_T res = {0};
    MMISRVAUD_ROUTE_T route = 0;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRoute() entry, audio_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_434_112_2_18_3_23_40_36,(uint8*)"d", audio_handle));
    
    data.event = MMISRVAUD_EVENT_GET_ROUTE;

    if(audio_handle != 0)
    {
        if(MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, &res))
        {
            route = (MMISRVAUD_ROUTE_T)res.data1;
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRoute() exit, route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_446_112_2_18_3_23_40_37,(uint8*)"d", route));

    return route;
}

/*****************************************************************************/
//  Description : Set audio play route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetRoute(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T audio_route)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetRoute() entry, audio_handle=0x%x, audio_route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_460_112_2_18_3_23_40_38,(uint8*)"dd", audio_handle, audio_route));

    if(audio_handle != 0 )
    {
        data.event = MMISRVAUD_EVENT_SET_ROUTE;
        data.data1 = (uint32)audio_route;
        
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetRoute() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_470_112_2_18_3_23_40_39,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Set audio all support route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAllSupportRoute(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T all_support_route)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAllSupportRoute() entry, audio_handle=0x%x, all_support_route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_484_112_2_18_3_23_40_40,(uint8*)"dd", audio_handle, all_support_route));

    if(audio_handle != 0 )
    {
        data.event = MMISRVAUD_EVENT_SET_ALL_SURPPORT_ROUTE;
        data.data1 = (uint32)all_support_route;
        
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAllSupportRoute() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_494_112_2_18_3_23_40_41,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Set audio manual route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetManualRoute(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T manual_route)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    SCI_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_SetManualRoute() entry, audio_handle=0x%x, manual_route=%d", audio_handle, manual_route);

    if(audio_handle != 0 )
    {
        data.event = MMISRVAUD_EVENT_SET_MANUAL_ROUTE;
        data.data1 = (uint32)manual_route;
        
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    SCI_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_SetManualRoute() exit, res=%d", res);
    return res;
}


/*****************************************************************************/
//  Description : Set audio play times.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
/*****************************************************************************/
//  Description : Set audio play times.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetPlayTimes(MMISRV_HANDLE_T audio_handle, uint32 times)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetPlayTimes() entry, audio_handle=0x%x, times=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_509_112_2_18_3_23_40_42,(uint8*)"dd", audio_handle, times));

    if(audio_handle != 0)
    {
        data.event = MMISRVAUD_EVENT_SET_TIMES;
        data.data1 = (uint32)times;
        
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetPlayTimes() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_519_112_2_18_3_23_41_43,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : To make a judgement if there is any audio is in playing.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsActive(void)
{
    /* This API is not used now */
    /* To be done later */
    /*
    ---------------------
    ---------------------
    ---------------------
    ---------------------
    */
    return TRUE;
}

/*****************************************************************************/
//  Description : Mute the appointed route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVMGR_SetRouteMute(MMISRVAUD_ROUTE_T route, BOOLEAN mute)
{
    /* This API is not used now */
    /* To be done later */
    /*
    ---------------------
    ---------------------
    ---------------------
    ---------------------
    */
    return FALSE;
}

#if 0
/*****************************************************************************/
//  Description : If audio played end.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsAudioEnd(MMISRV_HANDLE_T handle, BOOLEAN *result)
{
    MMISRVAUD_EVENT_T data = {0};
    MMISRVAUD_EVENT_RES_T res_data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_IsAudioEnd() entry, handle=%d, result=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_569_112_2_18_3_23_41_44,(uint8*)"dd", handle, result));

    if(handle != 0 && result != PNULL)
    {
        data.event = MMISRVAUD_EVENT_IS_AUDIO_END;
        
        if(MMISRVMGR_IoCtrl(handle, (uint32)&data, &res_data))
        {
            *result = (BOOLEAN)(res_data.data1);
            res = TRUE;
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_IsAudioEnd() exit, res=%d,result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_579_112_2_18_3_23_41_45,(uint8*)"dd", res,*result));
        }
    }

    return res;
}
#endif

/*****************************************************************************/
//  Description : To make a judgement if the audio service has setted the route manully.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsSetRouteManully(MMISRV_HANDLE_T handle)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;
    MMISRVAUD_EVENT_RES_T res_data = {0};
    BOOLEAN is_set = FALSE;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_IsSetRouteManully() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_597_112_2_18_3_23_41_46,(uint8*)"d", handle));

    if(handle != 0 )
    {
        data.event = MMISRVAUD_EVENT_IS_SET_ROUTE_MANULLY;
        
        res = MMISRVMGR_IoCtrl(handle, (uint32)&data, &res_data);
        if(res)
        {
            is_set = res_data.data1;
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_IsSetRouteManully() exit, is_set=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_610_112_2_18_3_23_41_47,(uint8*)"d", is_set));

    return is_set;
}

/*****************************************************************************/
//  Description : Get spectrum info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetSpectrum(MMISRV_HANDLE_T handle, MMISRVAUD_SPECTRUM_INFO_T *info)
{
    MMISRVAUD_EVENT_T data = {0};
    MMISRVAUD_EVENT_RES_T res_data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetSpectrum() entry, handle=0x%x, info=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_625_112_2_18_3_23_41_48,(uint8*)"dd", handle, info));

    if(handle != 0 && info != PNULL)
    {
        data.event = MMISRVAUD_EVENT_GET_SPECTRUM;
        data.data1 = (uint32)info;
        res = MMISRVMGR_IoCtrl(handle, (uint32)&data, &res_data);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetSpectrum() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_634_112_2_18_3_23_41_49,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Open spectrum.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_OpenSpectrum(MMISRV_HANDLE_T handle)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_OpenSpectrum() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_648_112_2_18_3_23_41_50,(uint8*)"d", handle));

    if(handle != 0)
    {
        data.event = MMISRVAUD_EVENT_OPEN_SPECTRUM;
        res = MMISRVMGR_IoCtrl(handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_OpenSpectrum() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_656_112_2_18_3_23_41_51,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : close spectrum.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_CloseSpectrum(MMISRV_HANDLE_T handle)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_CloseSpectrum() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_670_112_2_18_3_23_41_52,(uint8*)"d", handle));

    if(handle != 0)
    {
        data.event = MMISRVAUD_EVENT_CLOSE_SPECTRUM;
        res = MMISRVMGR_IoCtrl(handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_CloseSpectrum() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_678_112_2_18_3_23_41_53,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Enable/disable voice loopback.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVoiceLB(MMISRV_HANDLE_T handle, BOOLEAN is_enable)
{
    MMISRVAUD_EVENT_T data = {0};
    //MMISRVAUD_EVENT_RES_T res_data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetVoiceLB() entry, handle=0x%x, is_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_693_112_2_18_3_23_41_54,(uint8*)"dd", handle, is_enable));

    if(handle != 0)
    {
        data.event = MMISRVAUD_EVENT_SET_VOICE_LB;
        data.data1 = (uint32)is_enable;

        res = MMISRVMGR_IoCtrl(handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetVoiceLB() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_703_112_2_18_3_23_41_55,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Enable/disable voice loopback.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_GetRecordDB(MMISRV_HANDLE_T handle)
{
    MMISRVAUD_EVENT_T data = {0};
    MMISRVAUD_EVENT_RES_T res = {0};
    uint32 db = 0;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRecordDB() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_718_112_2_18_3_23_41_56,(uint8*)"d", handle));
    
    data.event = MMISRVAUD_EVENT_GET_RECORD_DB;

    if(handle != 0)
    {
        if(MMISRVMGR_IoCtrl(handle, (uint32)&data, &res))
        {
            db = res.data1;
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetRecordDB() exit, db=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_730_112_2_18_3_23_41_57,(uint8*)"d", db));

    return db;
}

/*****************************************************************************/
//  Description : Get an audio's info from buff.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetBuffContentInfo(uint8 *buf, uint32 buf_len, MMISRVAUD_RING_FMT_E fmt, MMISRVAUD_CONTENT_INFO_T *info_ptr)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_RING_BUF_T buf_type = {0};
    BOOLEAN res = FALSE;

    buf_type.data = buf;
    buf_type.data_len = buf_len;
    buf_type.fmt = fmt;
    buf_type.type = MMISRVAUD_TYPE_RING_BUF;

    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_BUF_INFO;
    cmd_param.data1 = (uint32)(&buf_type);
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)info_ptr);

    return res;
    
}

/*****************************************************************************/
//  Description : Get an audio's info from buff.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetFileContentInfo(wchar *file_name, uint32 name_len, MMISRVAUD_CONTENT_INFO_T *info_ptr)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_RING_FILE_T file_type = {0};
    BOOLEAN res = FALSE;

    file_type.name = file_name;
    file_type.name_len = name_len;
    file_type.fmt = MMIAPICOM_GetMusicTypeEx(file_name, name_len, TRUE);
    file_type.type = MMISRVAUD_TYPE_RING_FILE;
    
    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_FILE_INFO;
    cmd_param.data1 = (uint32)(&file_type);
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)info_ptr);

    return res;    
}

/*****************************************************************************
 The following function is to compatible the old API.
 When all the app take the new API place of the old API, these functions would be deleted.
*****************************************************************************/

/*****************************************************************************/
//  Description : Get current audio configure.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetAudCfg(MMISRVAUD_AUD_CFG_T *aud_cfg)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    //MMISRVAUD_EXT_CMD_RES_T res_data = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_CFG;
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)aud_cfg);
    return res;
}

/*****************************************************************************/
//  Description : Set mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetMuteNohandle(BOOLEAN is_mute)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_MUTE;
    cmd_param.data1 = (uint32)is_mute;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    return res;
}

/*****************************************************************************/
//  Description : Get audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint32	MMISRVAUD_GetVolumeNoHandle(void)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_VOLUME;

    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);
    if(res)
    {
        return res_data.data1;
    }
    else
    {
        return MMISRVAUD_VOLUME_LEVEL_NONE;
    }
}

/*****************************************************************************/
//  Description : Set audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVolumeNoHandle(uint32 volume)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_VOLUME;
    cmd_param.data1 = volume;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    
    return res;
}

/*****************************************************************************/
//  Description : Set uplink mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetUpLinkMute(BOOLEAN is_mute)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_UPLINK_MUTE;
    cmd_param.data1 = (uint32)is_mute;
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    return res;
}

#if 0

/*****************************************************************************/
//  Description : Set play speed.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetPlaySpeedNoHandle(int32 speed)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_SPEED;
    cmd_param.data1 = speed;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    
    return res;
}

#ifdef MP3_EQ_SUPPORT
/*****************************************************************************/
//  Description : Set EQ mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetEQModeNoHandle(MMISRVAUD_EQ_MODE_E eq_mode)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_EQMODE;
    cmd_param.data1 = eq_mode;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    
    return res;
}
#endif

/*****************************************************************************/
//  Description : Get audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint32	MMISRVAUD_GetVolumeNoHandle(void)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_VOLUME;

    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);
    if(res)
    {
        return res_data.data1;
    }
    else
    {
        return MMISRVAUD_VOLUME_LEVEL_NONE;
    }
}

/*****************************************************************************/
//  Description : Set audio volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVolumeNoHandle(uint32 volume)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_VOLUME;
    cmd_param.data1 = volume;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    
    return res;
}

/*****************************************************************************/
//  Description : Get audio current route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T MMISRVAUD_GetRouteNoHandle(void)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};        

    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_ROUTE;

    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);

    if(res)
    {
        return (MMISRVAUD_ROUTE_T)res_data.data1;
    }
    else
    {
        return MMISRVAUD_ROUTE_NONE;
    }
}

/*****************************************************************************/
//  Description : Set audio play route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetRouteNohandle(MMISRVAUD_ROUTE_T audio_route)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_ROUTE;
    cmd_param.data1 = audio_route;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    
    return res;
}

/*****************************************************************************/
//  Description : Set mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetMuteNohandle(BOOLEAN is_mute)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_MUTE;
    cmd_param.data1 = (uint32)is_mute;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    return res;
}

/*****************************************************************************/
//  Description : Get mute statues.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_GetMuteNohandle(BOOLEAN *result)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};        
    
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_MUTE;
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);
    if(res)
    {
        *result = (BOOLEAN)res_data.data1;
    }
    return res;
}

/*****************************************************************************/
//  Description : Set uplink mute.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetUpLinkMute(BOOLEAN is_mute)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_UPLINK_MUTE;
    cmd_param.data1 = (uint32)is_mute;
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    return res;
}

#ifdef AUDIO_RATE_SUPPORT
PUBLIC BOOLEAN MMISRVAUD_SetPlayRate(MMISRVAUD_PLAY_RATE_E rate)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_PLAY_RATE;
    cmd_param.data1 = (uint32)rate;
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    return res;
}
#endif

#endif

/*****************************************************************************/
//  Description : audio init
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_Init(void)
{
#if 0
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
#endif

    MMISRVAUD_RegistService();

#if 0        
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = PNULL;
    req.pri = 0;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
    audio_srv.info.type = MMISRVAUD_TYPE_TONE_DTMF;
    s_mmsrvaud_dtmf_tone_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    audio_srv.info.type = MMISRVAUD_TYPE_TONE_CUSTOM;
    s_mmsrvaud_custom_tone_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    audio_srv.info.type = MMISRVAUD_TYPE_TONE_GENERIC;
    s_mmsrvaud_gen_tone_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
#endif    
}

/*****************************************************************************/
//  Description : Get the handle by name.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_GetHandleByName(char *client_name)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};        
    
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = 0;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetHandleByName() entry, client_name=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1087_112_2_18_3_23_42_58,(uint8*)"d", client_name));

    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_HANDLE_BY_NAME;
    cmd_param.data1 = (uint32)client_name;
    
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);
    if(res)
    {
        handle = (MMISRV_HANDLE_T)res_data.data1;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetHandleByName() exit, res=%d, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1097_112_2_18_3_23_42_59,(uint8*)"dd", res, handle));

    return handle;
}

/*****************************************************************************/
//  Description : Get service's pri.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetSrvPri(MMISRV_HANDLE_T handle)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = FALSE;
    uint16 pri = 0;
    MMISRVAUD_EVENT_RES_T res_data = {0};
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido GetSrvPri() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1113_112_2_18_3_23_42_60,(uint8*)"d", handle));

    if(handle != 0)
    {
        data.event = MMISRVAUD_EVENT_GET_SRV_PRI;
        res = MMISRVMGR_IoCtrl(handle, (uint32)&data, PNULL);
        if(res)
        {
            pri = res_data.data1;
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido GetSrvPri() exit, res=%d, pri=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1125_112_2_18_3_23_42_61,(uint8*)"dd", res, pri));
    
    return res;
    
}

/*****************************************************************************/
//  Description : request tone handle.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T ReqToneSrv(
                uint16 pri,
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq,
                uint32 is_standing
                )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    MMISRVAUD_TYPE_U type_info = {0};

    handle = MMISRVAUD_GetHandleByName("TONE SRV");
    if(handle > 0)
    {
        if(pri >= GetSrvPri(handle))
        {
            MMISRVMGR_Free(handle);
        }
        else
        {
            return 0;
        }
    }

    req.is_auto_free = TRUE;
    req.pri = pri;
    req.client_name = "TONE SRV";
    switch(tone_type)
    {
    case MMISRVAUD_TYPE_TONE_GENERIC:
        type_info.tone_generic.duation = duration;
        type_info.tone_generic.type = tone_type;
        type_info.tone_generic.id = tone_id;
        break;
    case MMISRVAUD_TYPE_TONE_CUSTOM:
        type_info.tone_custom.duation = duration;
        type_info.tone_generic.type = tone_type;
        type_info.tone_custom.freq = freq;
        break;
    case MMISRVAUD_TYPE_TONE_DTMF:
        type_info.tone_dtmf.duation = duration;
        type_info.tone_dtmf.type = tone_type;
        type_info.tone_dtmf.id = tone_id;
        break;
    default:
        return 0;
    }

    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
    audio_srv.info = type_info;
    audio_srv.info.type = tone_type;
    audio_srv.is_standing = is_standing;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    return handle;
}

/*****************************************************************************/
//  Description : play tone 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PlayToneEx(
                uint16 pri,
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                )
{
    //MMISRVMGR_SERVICE_REQ_T req = {0};
    //MMISRVAUD_TYPE_T audio_srv = {0};

    MMISRV_HANDLE_T handle = 0;
    //MMISRVAUD_TYPE_U type_info = {0};
    BOOLEAN res = FALSE;
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_PlayToneEx() entry, tone_type=%d, tone_id=%d, duration=%d,freq=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1213_112_2_18_3_23_42_62,(uint8*)"dddd", tone_type, tone_id, duration, freq);

    handle = ReqToneSrv(pri, tone_type, tone_id, duration, freq, FALSE);
    
    if(handle > 0)
    {
        res = MMISRVAUD_Play(handle, 0);
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_PlayToneEx() exit, handle=0x%x, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1222_112_2_18_3_23_42_63,(uint8*)"dd", handle, res));
        
}

/*****************************************************************************/
//  Description : play tone 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PlayTone(
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                )
{
    MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_NORMAL_N10, tone_type, tone_id, duration, freq);
}

/*****************************************************************************/
//  Description : play tone  with route.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PlayToneWithRoute(
                MMISRVAUD_ROUTE_T route,
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                )
{
    //MMISRVMGR_SERVICE_REQ_T req = {0};
    //MMISRVAUD_TYPE_T audio_srv = {0};

    MMISRV_HANDLE_T handle = 0;
    //MMISRVAUD_TYPE_U type_info = {0};
    MMISRVAUD_RET_E res = FALSE;
    
    //MMISRVAUD_EXT_CMD_T cmd_param = {0};

        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_PlayToneWithRoute() entry, tone_type=%d, tone_id=%d, duration=%d,freq=%d,route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1262_112_2_18_3_23_42_64,(uint8*)"ddddd", tone_type, tone_id, duration, freq));

    handle = ReqToneSrv(MMISRVAUD_PRI_NORMAL_N10, tone_type, tone_id, duration, freq, TRUE);
    
    if(handle > 0)
    {
//        cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_ROUTE;
//        cmd_param.data1 = route;
//        MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
        MMISRVAUD_SetRoute(handle, route);
        MMISRVAUD_SetVolume(handle, DEFAULT_TONE_VOLUME);
        res = MMISRVAUD_Play(handle, 0);
    }
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: MMISRVAUD_PlayToneWithRoute() exit, handle=0x%x, res=%d", handle, res));
    
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : play tone  with route.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAUD_PlayToneWithRouteExVolume(
                MMISRVAUD_ROUTE_T route,
                MMISRVAUD_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq,
                uint32 volume
                )
{
    MMISRV_HANDLE_T handle = 0;
    MMISRVAUD_RET_E res = FALSE;

      MMISRV_TRACE_LOW("[MMISRV]: MMISRVAUD_PlayToneWithRouteExVolume() entry, tone_type=%d, tone_id=%d, duration=%d,freq=%d,route=%d, volume", tone_type, tone_id, duration, freq, route, volume);

    handle = ReqToneSrv(MMISRVAUD_PRI_NORMAL_N10, tone_type, tone_id, duration, freq, TRUE);

    if(handle > 0)
    {
        MMISRVAUD_SetRoute(handle, route);
        MMISRVAUD_SetVolume(handle, volume);
        res = MMISRVAUD_Play(handle, 0);
    }
      MMISRV_TRACE_LOW("[MMISRV]: MMISRVAUD_PlayToneWithRouteExVolume() exit, handle=%d, res=%d", handle, res);
}
#endif

/*****************************************************************************/
//  Description : tone stop 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_StopTone(
                MMISRVAUD_TYPE_E tone_type
                )
{
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_StopTone() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1289_112_2_18_3_23_42_66,(uint8*)""));
    
    handle = MMISRVAUD_GetHandleByName("TONE SRV");
    if(handle > 0)
    {
        res = MMISRVMGR_Free(handle);
    }
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_StopTone() exit, handle=0x%x, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1297_112_2_18_3_23_42_67,(uint8*)"dd", handle, res);
    
}

#ifdef VIRTUAL_VIBRATE_FUNC	
/*****************************************************************************/
//  Description : Start vibrate  by play vibrate_ring.
//  Global resource dependence : none
//  Author: Zhongjie.Liu
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_StartVibrate(uint16 pri, uint16 duation, uint16 interval, uint16 times, MMISRVAUD_VIBRATE_CB callback)
{
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_WAVE, 0, NULL};/*lint !e64*/


    handle = MMISRVAUD_GetHandleByName("VIBRATE SRV");
    if(handle > 0)
    {
        if(pri >= GetSrvPri(handle))
        {
            MMISRVMGR_Free(handle);
        }
        else
        {
            return 0;
        }
    }
    MMI_GetRingInfo( R_VIBRATE_200HZ,   &ring_data);
    req.pri = MMISRVAUD_PRI_NORMAL; //pri;
//    req.is_auto_free = TRUE;
    req.client_name = "VIBRATE SRV";
    audio_srv.info.type = MMISRVAUD_TYPE_VIBERATE;
    audio_srv.duation = duation;
    audio_srv.interval=  MMI_3SECONDS ;  //  MMI_3SECONDS   interval  
    audio_srv.play_times = times;
    audio_srv.info.vibrate.cb = callback;
    audio_srv.info.ring_buf.data=ring_data.data_ptr;
    audio_srv.info.ring_buf.fmt =  (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;//  ring_data.type;
    audio_srv.info.ring_buf.data_len = ring_data.data_len;
	audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER;
 	audio_srv.default_route =  MMISRVAUD_ROUTE_SPEAKER;
    audio_srv.volume = 7;
    audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_NONE;  // 2013 0923   change volume type


//	audio_srv.is_standing = TRUE;
	SCI_TRACE_LOW("MMISRVAUD_StartVibrate  ..ring_buf.fmt=%d..data_len=%d ", audio_srv.info.ring_buf.fmt,audio_srv.info.ring_buf.data_len);
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, (void *)&audio_srv);
    if(handle > 0)
    {
	MMISRVAUD_EnableMusicShake(TRUE);
        /* Here we regard the offset as the duation, MMISRVAUD DRV will do a special process for MMISRVAUD_TYPE_VIBERATE */
        res = MMISRVAUD_Play(handle, 0);
    }

    
    return handle;
}

/*****************************************************************************/
//  Description : Close vibrate  by free vibrate_ring handle.
//  Global resource dependence : none
//  Author: Zhongjie.Liu
//  Note: 
/*****************************************************************************/
PUBLIC  void MMISRVAUD_StopVibrate( )
{
    MMISRV_HANDLE_T vib_handle = 0;
    
    vib_handle = MMISRVAUD_GetHandleByName("VIBRATE SRV");

	SCI_TRACE_LOW("MMISRVAUD_StopVibrate  2013 0829 ");
	
    if(vib_handle != 0)
    {
		MMISRVMGR_Free(vib_handle);
    }
    MMISRVAUD_EnableMusicShake(FALSE);

}

#else

/*****************************************************************************/
//  Description : Start vibrate.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_StartVibrate(uint16 pri, uint16 duation, uint16 interval, uint16 times, MMISRVAUD_VIBRATE_CB callback)
{
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};

    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_StartVibrate() entry, pri=%d, duation=%d, interval=%d,times=%d,callback=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1312_112_2_18_3_23_42_68,(uint8*)"ddddd", pri, duation, interval, times, callback);

    handle = MMISRVAUD_GetHandleByName("VIBRATE SRV");
    if(handle > 0)
    {
        if(pri >= GetSrvPri(handle))
        {
            MMISRVMGR_Free(handle);
        }
        else
        {
            return 0;
        }
    }
    
    req.pri = pri;
    req.is_auto_free = TRUE;
    req.client_name = "VIBRATE SRV";
    audio_srv.info.type = MMISRVAUD_TYPE_VIBERATE;
    audio_srv.duation = duation;
    audio_srv.interval= interval;
    audio_srv.play_times = times;
    audio_srv.info.vibrate.cb = callback;
    
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, (void *)&audio_srv);
    if(handle > 0)
    {
        /* Here we regard the offset as the duation, MMISRVAUD DRV will do a special process for MMISRVAUD_TYPE_VIBERATE */
        res = MMISRVAUD_Play(handle, duation);
    }

    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_StartVibrate() exit, handle=0x%x,res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1342_112_2_18_3_23_42_69,(uint8*)"dd", handle,res);
    
    return handle;
}

/*****************************************************************************/
//  Description : Start vibrate.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC  void MMISRVAUD_StopVibrate(MMISRV_HANDLE_T handle)
{
    MMISRV_HANDLE_T vib_handle = 0;
    
    vib_handle = MMISRVAUD_GetHandleByName("VIBRATE SRV");

    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_StopVibrate() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1357_112_2_18_3_23_42_70,(uint8*)"d", vib_handle);

    if(vib_handle != 0)
    {
        MMISRVAUD_Stop(handle);
    }
}

#endif

/*****************************************************************************/
//  Description : headset plug in/out, BT earphone is active or switch off.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_ExtDevChg(void)
{

    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_ExtDevChg() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1375_112_2_18_3_23_42_71,(uint8*)"");
    
    cmd_param.cmd = MMISRVAUD_EXT_CMD_EXT_DEV_CHG;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    return res;
}

/*****************************************************************************/
//  Description : headset plug in/out, BT earphone is active or switch off.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_E event)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_HandleBtEvent() entry, event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1390_112_2_18_3_23_42_72,(uint8*)"d", event);
    
    cmd_param.cmd = MMISRVAUD_EXT_CMD_BT_EVENT;
    cmd_param.data1 = event;
    MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
}

/*****************************************************************************/
//  Description : Start voice loop.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_EnableVoiceLB(BOOLEAN is_enable, uint32 volume)
{
    static MMISRV_HANDLE_T lb_handle = 0;
    BOOLEAN res = FALSE;

    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_EnableVoiceLB() entry, handle=0x%x, is_enable=%d,volume=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1406_112_2_18_3_23_42_73,(uint8*)"ddd", lb_handle, is_enable, volume);
    
    if(lb_handle != 0)
    {
        MMISRVMGR_Free(lb_handle);
        lb_handle = 0;
    }
    
    if(is_enable)
    {
        MMISRVMGR_SERVICE_REQ_T req = {0};
        MMISRVAUD_TYPE_T audio_srv = {0};
        
        req.pri = MMISRVAUD_PRI_HIGH;

        audio_srv.duation = 0;
        audio_srv.eq_mode = 0;
        audio_srv.is_mixing_enable = FALSE;
        audio_srv.play_times = 1;
#if defined(XT_SPEAKER_EAR_COMBO)||defined(W18_FACTORY_LOOP_BACK_TEST)		//  #三合一喇叭回环测试要设置为喇叭输出
        audio_srv.all_support_route = MMISRVAUD_ROUTE_EARPHONE | MMISRVAUD_ROUTE_SPEAKER;		
#else
        audio_srv.all_support_route = MMISRVAUD_ROUTE_EARPHONE | MMISRVAUD_ROUTE_HANDHOLD;
#endif
        audio_srv.info.voice.type = MMISRVAUD_TYPE_VOICE_LOOPBACK;
        audio_srv.volume = volume;

        lb_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, (void *)&audio_srv);
        if(lb_handle > 0)
        {
            res = MMISRVAUD_Play(lb_handle, 0);
        }
    }
    else
    {
        res = TRUE;
    }

    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_EnableVoiceLB() exit, handle=0x%x, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1440_112_2_18_3_23_42_74,(uint8*)"dd", lb_handle, res);
    
    return res;
}

/*****************************************************************************/
//  Description : To make a judgement if the audio service is playing.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsAudPlaying(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {0};
    MMISRVAUD_EVENT_RES_T res = {0};
    BOOLEAN is_playing = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_IsAudPlaying() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1455_112_2_18_3_23_42_75,(uint8*)"d", audio_handle));
    
    data.event = MMISRVAUD_EVENT_CHECK_IS_PLAYING;

    if(audio_handle != 0)
    {
        if(MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, &res))
        {
            is_playing = res.data1;
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_IsAudPlaying() exit, is_playing=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1467_112_2_18_3_23_42_76,(uint8*)"d", is_playing));

    return is_playing;
}

/*****************************************************************************/
//  Description : Set ATV's service id.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAtvServiceId(MMISRV_HANDLE_T audio_handle, uint32 service_id)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = {0};

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAtvServiceId() entry, handle=0x%x, service_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1481_112_2_18_3_23_42_77,(uint8*)"dd", audio_handle, service_id));
    
    data.event = MMISRVAUD_EVENT_SET_ATV_SERVICE_ID;
    data.data1 = service_id;

    if(audio_handle != 0)
    {
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAtvServiceId() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1491_112_2_18_3_23_42_78,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Set ATV's service id.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAtvRegionId(MMISRV_HANDLE_T audio_handle, uint32 region_id)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = {0};

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAtvRegionId() entry, handle=0x%x, region_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1505_112_2_18_3_23_42_79,(uint8*)"dd", audio_handle, region_id));
    
    data.event = MMISRVAUD_EVENT_SET_ATV_REGION_ID;
    data.data1 = region_id;
    if(audio_handle != 0)
    {
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_SetAtvRegionId() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1514_112_2_18_3_23_42_80,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Start record ATV.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_StartRecordAtv(MMISRV_HANDLE_T audio_handle, void *param)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = {0};

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_StartRecordAtv() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1528_112_2_18_3_23_43_81,(uint8*)"d", audio_handle));
    
    data.event = MMISRVAUD_EVENT_START_RECORD_ATV;
    data.data1 = (uint32)param;
    if(audio_handle != 0)
    {
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_StartRecordAtv() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1537_112_2_18_3_23_43_82,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Stop record ATV.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_StopRecordAtv(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {0};
    BOOLEAN res = {0};

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_StopRecordAtv() entry, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1551_112_2_18_3_23_43_83,(uint8*)"d", audio_handle));
    
    data.event = MMISRVAUD_EVENT_STOP_RECORD_ATV;

    if(audio_handle != 0)
    {
        res = MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_StopRecordAtv() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1560_112_2_18_3_23_43_84,(uint8*)"d", res));

    return res;
    
}

/*****************************************************************************/
//  Description : Request a virtual audio handel by name. Generally, this function is used to occupy the
//                    audio resource to prevent or stop other audio to play.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_ReqVirtualHandle(char *client_name, MMISRVAUD_PRI_T pri)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_ReqVirtualHandle() entry, client_name=0x%x, pri=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1576_112_2_18_3_23_43_85,(uint8*)"dd", client_name, pri));

    if(client_name != PNULL)
    {
        handle = MMISRVAUD_GetHandleByName(client_name);
        if(0 == handle)
        {
            req.pri = pri;
            req.client_name = client_name;
            audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
            handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_ReqVirtualHandle() exit, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1591_112_2_18_3_23_43_86,(uint8*)"d", handle));
    
    return handle;
}

/*****************************************************************************/
//  Description : Free virtual handle by name.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_FreeVirtualHandle(char *client_name)
{
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_FreeVirtualHandle() entry, client_name=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1604_112_2_18_3_23_43_87,(uint8*)"d", client_name));
    
    if(client_name != PNULL)
    {
        handle = MMISRVAUD_GetHandleByName(client_name);
        if(handle > 0)
        {
            res = MMISRVMGR_Free(handle);
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_FreeVirtualHandle() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1614_112_2_18_3_23_43_88,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Get handle by volume type, only used to adjust the volume generally.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_GetHandleByVolumeType(MMISRVAUD_VOLUME_TYPE_E volume_type)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetHandleByVolumeType() entry, volume_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1629_112_2_18_3_23_43_89,(uint8*)"d", volume_type));
    
    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_HANDLE_BY_VOLUME_TYPE;
    cmd_param.data1 = (uint32)volume_type;

    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_GetHandleByVolumeType() exit, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1636_112_2_18_3_23_43_90,(uint8*)"d", res_data.data1));
    
    if(res)
    {
        return (MMISRV_HANDLE_T)res_data.data1;
    }
    else
    {
        return 0;
    }    
    
}

/*****************************************************************************/
//  Description : Get handle by driver handle,.
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMISRVAUD_GetHandleByDrvHandle(uint32 drv_handle)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};
    BOOLEAN res = FALSE;

   SCI_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_GetHandleByDrvHandle() entry, drv_handle=%0x", drv_handle);

    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_HANDLE_BY_DRV_HANDLE;
    cmd_param.data1 = drv_handle;

    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);

    if(res)
    {
        return (MMISRV_HANDLE_T)res_data.data1;
    }
    else
    {
        return 0;
    }

}

/*****************************************************************************/
//  Description : Get driver handle by srv handle,.
//  Global resource dependence :
//  Author:zirui.li
//  Note:
/*****************************************************************************/
PUBLIC HAUDIO MMISRVAUD_GetDrvHandleBySrvHandle(uint32 srv_handle)   //bug1989634
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};
    BOOLEAN res = FALSE;

   SCI_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_GetDrvHandleBySrvHandle() entry, srv_handle=%0x", srv_handle);

    cmd_param.cmd = MMISRVAUD_EXT_CMD_GET_DRV_HANDLE_BY_SRV_HANDLE;
    cmd_param.data1 = srv_handle;

    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);

    if(res)
    {
        return (HAUDIO)res_data.data1;
    }
    else
    {
        return 0;
    }

}

/*****************************************************************************/
//  Description : Get handle by volume type, only used to adjust the volume generally.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAUD_NotifyVolumeSelfApp(BOOLEAN is_increase)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    //BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Auido MMISRVAUD_NotifyVolumeSelfApp() entry, is_increase=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_API_1658_112_2_18_3_23_43_91,(uint8*)"d", is_increase));
    
    cmd_param.cmd = MMISRVAUD_EXT_CMD_VOLUME_ADJUST;
    cmd_param.data1 = (uint32)is_increase;

    MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);    
}

/*****************************************************************************/
//  Description : Close audio handle, but not free. It's means we can't play/stop/resume/pause it.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Should stop it at first.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_Close(MMISRV_HANDLE_T audio_handle)
{
    MMISRVAUD_EVENT_T data = {MMISRVAUD_EVENT_CLOSE, 0, 0};
    MMISRVAUD_RET_E res = MMISRVAUD_RET_ERROR;
    MMISRVAUD_EVENT_RES_T res_data = {0};

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_Close() entry, audio_handle=0x%x", audio_handle));
    
    if(MMISRVMGR_IoCtrl(audio_handle, (uint32)&data, &res_data))
    {
        res = res_data.data1;
    }

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_Close() exit, res=%d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Set volume by volume type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Each audio handle will be setted whitch is the same type.
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetVolumeByType(MMISRVAUD_VOLUME_TYPE_E type, MMISRVAUD_VOLUME_T volume)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    BOOLEAN res = FALSE;

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_SetVolumeByType() entry, type=%d", type));

    cmd_param.cmd = MMISRVAUD_EXT_CMD_SET_VOLUME_BY_TYPE;
    cmd_param.data1 = type;
    cmd_param.data2 = volume;
    res = MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_SetVolumeByType() exit, res=%d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Audio driver call back handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Each audio handle will be setted whitch is the same type.
/*****************************************************************************/
PUBLIC void MMISRVAUD_HandleCallBackMsg(
                    void *msg_ptr
                    )
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    MMISRVAUD_EXT_CMD_RES_T res_data = {0};
//    BOOLEAN res = FALSE;

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Auido MMISRVAUD_HandleCallBackMsg() entry, msg_ptr=0x%x", msg_ptr));
    
    cmd_param.cmd = MMISRVAUD_EXT_CMD_DRV_CALLBACK;
    cmd_param.data1 = (uint32)msg_ptr;
    MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, (uint32 *)&res_data);
}

/*******************************************************************/
//  Interface:		MMISRVAUD_HandleEarphoneEvent
//  Description : 	MMISRVAUD_HandleEarphoneEvent
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMISRVAUD_HandleEarphoneEvent(MMISRVAUD_EARPHONE_EVENT_ID event, uint32 keycode)
{
    MMISRVAUD_EXT_CMD_T cmd_param = {0};
    
    MMISRV_TRACE_LOW("MMISRVAUD_HandleEarphoneEvent() entry, event=%d, keycode=%d", event, keycode);
    
    if (KEY_HEADSET_BUTTIN == keycode)
    {
        cmd_param.cmd = MMISRVAUD_EXT_CMD_EARPHONE_EVENT;
        cmd_param.data1 = event + keycode;
        MMISRVMGR_SendCmd((char *)STR_SRV_AUD_NAME, (uint32)&cmd_param, PNULL);
    }
}

#ifdef _SW_ZDT_PRODUCT_
PUBLIC BOOLEAN MMISRVAUD_EnableVoiceLBWithRoute(BOOLEAN is_enable, uint32 volume, MMISRVAUD_ROUTE_T all_support_route)
{
    static MMISRV_HANDLE_T lb_handle = 0;
    BOOLEAN res = FALSE;
    
    if(lb_handle != 0)
    {
        MMISRVMGR_Free(lb_handle);
        lb_handle = 0;
    }
    
    if(is_enable)
    {
        MMISRVMGR_SERVICE_REQ_T req = {0};
        MMISRVAUD_TYPE_T audio_srv = {0};
        
        req.pri = MMISRVAUD_PRI_HIGH;

        audio_srv.duation = 0;
        audio_srv.eq_mode = 0;
        audio_srv.is_mixing_enable = FALSE;
        audio_srv.play_times = 1;
        audio_srv.all_support_route = all_support_route;
        audio_srv.info.voice.type = MMISRVAUD_TYPE_VOICE_LOOPBACK;
        audio_srv.volume = volume;

        lb_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, (void *)&audio_srv);
        if(lb_handle > 0)
        {
            res = MMISRVAUD_Play(lb_handle, 0);
        }
    }
    else
    {
        res = TRUE;
    }
    
    return res;
}
#endif

#ifdef VIRTUAL_VIBRATE_FUNC
/*****************************************************************************/
//  Description : audio shake enable function( music+vibrate)
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: this function is used for music and vibrator playing (EQ)
/*****************************************************************************/
PUBLIC void MMISRVAUD_EnableMusicShake(BOOLEAN enable)
{
	AUDIO_EnableMusicShake(enable);
}
#endif


