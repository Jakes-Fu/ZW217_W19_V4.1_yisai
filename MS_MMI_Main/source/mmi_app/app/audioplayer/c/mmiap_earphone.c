/*****************************************************************************
** File Name:       mmimp3_earphone.c                                        *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine.Meng       Create                                  *
******************************************************************************/

#define _MMIMP3_EARPHONE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

//other module
#include "mmibt_export.h"

//local module
#include "mmiap_common.h"
#include "mmimp3_earphone.h"
#include "mmiap_list_control.h"
#include "mmimp3_export.h"
#include "mmiap_play.h"
#include "mmiap_list.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Play Or Pause
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_EventPlayOrPause(void)
{
    MMIMP3_AUDIO_STATE_E  audio_state  = MMIMP3_GetAudioState();
    uint32 offset = MMIMP3_ZERO_OFFSET;

    //SCI_TRACE_LOW:"MMIMP3 MMIMP3_EventPlayOrPause, player_state = %d, audio_state = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_BT_70_112_2_18_2_4_17_231, (uint8*)"dd", MMIAPIMP3_GetPlayerState(), MMIMP3_GetAudioState());

    switch(MMIAPIMP3_GetPlayerState())
    {
    case MMIMP3_PLAYER_STATE_NONE:
        if(MMIMP3_AUDIO_STOPPED == audio_state)
        {
            //MMIMP3_CheckModuleInfo();
            //MMIAPIMP3_PlayAudioPlayer(MMIMP3_ZERO_OFFSET);
         	offset = MMIAPIAP_GetCurPlayOffset(offset);
        	MMIAPIMP3_PlayAudioPlayer(offset);
        }

        break;

    case MMIMP3_PLAYER_AUDIO_ACTIVE:
    {
        switch(MMIMP3_GetAudioState())
        {
        case MMIMP3_AUDIO_PLAYING:
        {
            MMIAPIMP3_PauseMp3(FALSE);
        }
        break;

        case MMIMP3_AUDIO_PAUSED:
            MMIAPIMP3_ResumeMp3(FALSE);
            break;

        default:
            break;
        }
    }
    break;

    default:
        break;
    }
}
/*****************************************************************************/
//  Description : Play
//  Global resource dependence : none
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_EventPlay(void)
{
    MMIMP3_AUDIO_STATE_E  audio_state  = MMIMP3_GetAudioState();
    uint32 offset = MMIMP3_ZERO_OFFSET;

    SCI_TraceLow("MMIMP3 MMIMP3_EventPlay, player_state = %d, audio_state = %d",MMIAPIMP3_GetPlayerState(),MMIMP3_GetAudioState());
    switch(MMIAPIMP3_GetPlayerState())
    {
        case MMIMP3_PLAYER_STATE_NONE:
        {
            if(MMIMP3_AUDIO_STOPPED == audio_state)
            {
                offset = MMIAPIAP_GetCurPlayOffset(offset);
                MMIAPIMP3_PlayAudioPlayer(offset);
            }

            break;
        }

        case MMIMP3_PLAYER_AUDIO_ACTIVE:
        {
            switch(MMIMP3_GetAudioState())
            {
                case MMIMP3_AUDIO_PAUSED:
                    MMIAPIMP3_ResumeMp3(FALSE);
                    break;

                default:
                    break;
            }
            break;
        }

        default:
            break;

    }
}

/*****************************************************************************/
//  Description : Pause
//  Global resource dependence : none
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_EventPause(void)
{
    MMIMP3_AUDIO_STATE_E  audio_state  = MMIMP3_GetAudioState();
    uint32 offset = MMIMP3_ZERO_OFFSET;

    SCI_TraceLow("MMIMP3 MMIMP3_EventPause, player_state = %d, audio_state = %d",MMIAPIMP3_GetPlayerState(),MMIMP3_GetAudioState());
    switch(MMIAPIMP3_GetPlayerState())
    {
        case MMIMP3_PLAYER_AUDIO_ACTIVE:
            switch(MMIMP3_GetAudioState())
            {
                case MMIMP3_AUDIO_PLAYING:
                    MMIAPIMP3_PauseMp3(FALSE);
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

/*****************************************************************************/
//  Description : Stop
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_EventStop(void)
{
    MMIMP3_AUDIO_STATE_E  audio_state  = MMIMP3_GetAudioState();
    
	SCI_TRACE_LOW("MMIMP3_EventStop: player_state = %d, audio_state = %d ",  MMIAPIMP3_GetPlayerState(),MMIMP3_GetAudioState());    

#ifdef BLUETOOTH_SUPPORT	
    if(!MMIAPIMP3_IsThroughBt())
#endif
    {
		SCI_TRACE_LOW("MMIMP3_EventStop , Not through bt!!!!");
        return;
    }

    switch(MMIAPIMP3_GetPlayerState())
    {
		case MMIMP3_PLAYER_STATE_NONE:
			break;
			
		case MMIMP3_PLAYER_AUDIO_ACTIVE:
			MMIMP3_StopMp3();
			SCI_TRACE_LOW("MMIMP3_EventStop  MMIMP3_StopMp3 , exit");
			break;			
		default:
			break;
    }
}

/*****************************************************************************/
//  Description : MMIMP3_EventPre Bt
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_EventPre(void)
{
    //SCI_TRACE_LOW:"MMIMP3  MMIMP3_EventPre"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_BT_119_112_2_18_2_4_17_233, (uint8*)"");

#ifdef BLUETOOTH_SUPPORT
#if (defined BT_A2DP_SUPPORT || defined BT_HFG_SUPPORT)

    if(MMIAPIMP3_IsThroughBt())
    {
        if(!MMIAPIMP3_IsSupportA2DPForCurPlay())
        {
            return;
        }
    }

#endif
#endif
    {
        MMIAPIMP3_GoToPreOrNextMp3(TRUE);

        MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, TRUE);
    }
}

/*****************************************************************************/
//  Description : MMIMP3_EventNext Bt
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_EventNext(void)
{
    //SCI_TRACE_LOW:"MMIMP3  MMIMP3_EventNext"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_BT_142_112_2_18_2_4_17_235, (uint8*)"");

#ifdef BLUETOOTH_SUPPORT
#if (defined BT_A2DP_SUPPORT || defined BT_HFG_SUPPORT)

    if(MMIAPIMP3_IsThroughBt())
    {
        if(!MMIAPIMP3_IsSupportA2DPForCurPlay())
        {
            return;
        }
    }

#endif
#endif
    {
        MMIAPIMP3_GoToPreOrNextMp3(FALSE);

        MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, FALSE);
    }
}


/*****************************************************************************/
//  Description : To judge whether the music is supported to play with a2dp.
//  Global resource dependence :
//  Author:
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsSupportA2DPForCurPlay(void)
{
    BOOLEAN                  result = FALSE;
    MMIMP3_LIST_FILE_BODY_T  cur_play_info = {0};


    cur_play_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(MMIMP3_GetCurPlayFileFullPath(cur_play_info.full_path_name, &cur_play_info.full_path_len))
    {
        result = MMIAPIMP3_IsFileSupportA2DP(
                     cur_play_info.full_path_name,
                     cur_play_info.full_path_len
                 );
    }

    //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_IsSupportA2DPForCurPlay, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11901_112_2_18_2_4_14_223, (uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : Judge whether the music is supported to play with a2dp.
//  Global resource dependence :
//  Author: xingdong.li
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsFileSupportA2DP(
    const wchar *full_path_ptr,
    uint16      full_path_len
)
{
    BOOLEAN                  result = FALSE;
    MMISRVAUD_RING_FMT_E     audio_type = MMISRVAUD_RING_FMT_MAX;
#ifdef DRM_SUPPORT

    if(!MMIAPIMP3_IsDrmFile(
                full_path_ptr, full_path_len,
                &audio_type, PNULL, PNULL))
#endif
    {
        audio_type = MMIAPICOM_GetMusicType(
                         full_path_ptr,
                         full_path_len
                     );
    }

    switch(audio_type)
    {
    case MMISRVAUD_RING_FMT_MP3:
#ifdef MP3_A2DP_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_WMA:
#ifdef WMA_A2DP_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_AAC:
    case MMISRVAUD_RING_FMT_M4A:
#ifdef AAC_A2DP_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_MIDI:
#ifdef MIDI_A2DP_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_AMR:
#ifdef AMR_A2DP_SUPPORT
        result = TRUE;
#endif
        break;

    case MMISRVAUD_RING_FMT_WAVE:
#ifdef WAV_A2DP_SUPPORT
        result = TRUE;
#endif
        break;

    default:
        result = FALSE;
        break;
    }

    //SCI_TRACE_LOW:"[MMIMP3]: MMIAPIMP3_IsFileSupportA2DP, audio_type = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_11971_112_2_18_2_4_14_224, (uint8*)"dd", audio_type, result);
    return result;
}

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : check mp3 bt setting mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_CheckBtSetting(
    void
)
{
    MMIMP3_BT_SETTING_TYPE bt_setting = 0;
    BOOLEAN is_ok = FALSE;

    bt_setting = MMIMP3_GetBtSettingMode();


    SCI_TRACE_LOW("[MMIAP] MMIMP3_CheckBtSetting bt_setting=%d",\
                  bt_setting);

    /*默认蓝牙耳机连接，声音就从蓝牙耳机播放
    */

    if ((MMIMP3_BT_MODE_NONE == bt_setting) || (MMIMP3_BT_MODE_ON == bt_setting))
    {
        if (MMIAPIBT_GetActiveBTHeadset())
        {
            is_ok = MMIAP_TrySetBtAudio(TRUE);
            if (is_ok)
            {
                MMIMP3_SetBtSettingMode(MMIMP3_BT_MODE_ON);
            }

            SCI_TRACE_LOW("[MMIAP] MMIMP3_CheckBtSetting is_ok=%d",\
                          is_ok);
        }
		else
		{
			MMIMP3_SetBtSettingMode(MMIMP3_BT_MODE_NONE);
		}
    }
}

/*****************************************************************************/
//  Description : Is Through Bt
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsThroughBt(
    void
)
{
    BOOLEAN is_through_bt = FALSE;


    MMIMP3_CheckBtSetting();

    is_through_bt = (MMIMP3_BT_MODE_ON == MMIMP3_GetBtSettingMode());

    //SCI_TRACE_LOW:"[MMIMP3] MMIAPIMP3_IsThroughBt = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_1354_112_2_18_2_3_50_8, (uint8*)"d", is_through_bt);

    return is_through_bt;
}

/*****************************************************************************/
//  Description : try set bt audio
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_TrySetBtAudio(
    BOOLEAN is_bt_audio
)
{
    BOOLEAN ret = FALSE;
    MMISRVAUD_ROUTE_T route = 0;



    if (is_bt_audio)
    {
        route = MMISRVAUD_ROUTE_AUTO;
    }
    else
    {
        route = MMISRVAUD_ROUTE_AUTO_NO_BLUETOOTH;
    }

    ret = MMISRVAUD_SetAllSupportRoute(MMIAPIMP3_GetBGPlayerHandle(),
                                       route
                                      );


    SCI_TRACE_LOW("[MMIAP] MMIAP_TrySetBtAudio is_bt_audio=%d, ret=%d",\
                  is_bt_audio, ret);

    return ret;
}

#endif

