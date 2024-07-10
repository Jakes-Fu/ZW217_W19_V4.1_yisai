/*****************************************************************************
** File Name:      watch_set_notesound.c                                     *
** Author:         bin.wang1                                                 *
** Date:           02/28/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      bin.wang1              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "guilabel.h"
#include "guibutton.h"
#include "mmicom_trace.h"
#include "mmidisplay_data.h"

#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "mmienvset_export.h"

#include "watch_set_notesound.h"
#include "watch_set_sound.h"
#include "watch_set_position.h"
#include "watch_common.h"
#include "watch_common_btn.h"

#define SETTINGS_NOTESOUND_VOLUME_INTERVAL            (3)

MMI_IMAGE_ID_T s_settings_notesound_icon[] = {  
                                                IMAGE_SET_SOUND_BG_VOLUME_00,
                                                IMAGE_SET_SOUND_BG_VOLUME_01,
                                                IMAGE_SET_SOUND_BG_VOLUME_02,
                                                IMAGE_SET_SOUND_BG_VOLUME_03
                                              };

/**--------------------------------------------------------------------------*
 **                         RECT DEFINITION                                  *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL void SettingsNoteSoundSetBtnStatus( MMI_CTRL_ID_T left_btn, MMI_CTRL_ID_T right_btn )
{
    uint8 volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );

    if( volume > MMISET_VOL_MAX )
    {
        TRACE_APP_SETTINGS("volume error %u change to default value", volume);
        volume = MMISET_VOL_DEFAULT_MULTIM;
    }
    if( MMISET_VOL_MAX > volume && MMISET_VOL_ZERO < volume )
    {
        GUIBUTTON_SetGrayed( left_btn, FALSE, NULL, FALSE );
        GUIBUTTON_SetGrayed( right_btn, FALSE, NULL, FALSE );
    }
    else if( MMISET_VOL_MAX == volume )
    {
        GUIBUTTON_SetGrayed( left_btn, FALSE, NULL, FALSE);
        GUIBUTTON_SetGrayed( right_btn, TRUE, NULL, FALSE);
    }
    else
    {
        GUIBUTTON_SetGrayed(left_btn, TRUE, NULL, FALSE);
        GUIBUTTON_SetGrayed(right_btn, FALSE, NULL, FALSE);
    }
}

LOCAL void SettingsNoteSoundSetIconId( MMI_CTRL_ID_T ctrl_id )
{
    uint8 volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );

    if( volume > MMISET_VOL_MAX )
    {
        TRACE_APP_SETTINGS("MMVolume error %u change to default value", volume);
        volume = MMISET_VOL_DEFAULT_MULTIM;
    }

    // volume/SETTINGS_NOTESOUND_VOLUME_INTERVAL < 4
    GUILABEL_SetIcon( ctrl_id, s_settings_notesound_icon[volume/SETTINGS_NOTESOUND_VOLUME_INTERVAL] );
}

LOCAL void SettingsNoteSoundUpdate( uint8 volume )
{
    uint16 ringId = MMIAPIENVSET_GetFixCallRingId( MN_DUAL_SYS_1, MMIAPIENVSET_GetActiveModeId() );

    if( volume > MMISET_VOL_MAX )
    {
        TRACE_APP_SETTINGS("volume error %d", volume);
        return;
    }

    volume = volume / SETTINGS_NOTESOUND_VOLUME_INTERVAL * SETTINGS_NOTESOUND_VOLUME_INTERVAL;

    MMIENVSET_SetActiveModeOptValue( CALL_RING_VOL, volume, MN_DUAL_SYS_1 );
    MMIENVSET_SetActiveModeOptValue( MSG_RING_VOL, volume, MN_DUAL_SYS_1 );
    MMIENVSET_SetActiveModeOptValue( ALARM_RING_VOL, volume, MN_DUAL_SYS_1 );

    Settings_PreviewRing( ringId, MMISET_RING_TYPE_CALL, volume, FALSE );

    MMK_SendMsg( MMISET_NOTE_SOUND_WIN_ID, MSG_FULL_PAINT, PNULL );
}

LOCAL MMI_RESULT_E HandleSettingsNoteSoundBtnLeft( void )
{
    uint8 volume =  MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );
    if( volume > MMISET_VOL_MAX )
    {
        TRACE_APP_SETTINGS("volume error %u change to default value", volume);
        volume = MMISET_VOL_DEFAULT_MULTIM;
    }
    if( volume >= MMISET_VOL_THREE )
    {
        volume = volume - SETTINGS_NOTESOUND_VOLUME_INTERVAL;
        SettingsNoteSoundUpdate( volume );
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleSettingsNoteSoundBtnRight( void )
{
    uint8 volume = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL );
    if( volume > MMISET_VOL_MAX )
    {
        TRACE_APP_SETTINGS("volume error %u change to default value", volume);
        volume = MMISET_VOL_DEFAULT_MULTIM;
    }
    if( volume <= MMISET_VOL_SIX )
    {
        volume = volume + SETTINGS_NOTESOUND_VOLUME_INTERVAL;
        SettingsNoteSoundUpdate( volume );
    }

    return MMI_RESULT_TRUE;
}

LOCAL void Settings_NoteSound_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T labelRect = { WATCH_SET_DEFAULT_TITLE_X, WATCH_SET_DEFAULT_TITLE_Y, WATCH_SET_DEFAULT_TITLE_X+WATCH_SET_DEFAULT_TITLE_W, WATCH_SET_DEFAULT_TITLE_Y+WATCH_SET_DEFAULT_TITLE_H };
    MMI_CTRL_ID_T ctrlLabel = MMISET_NOTE_SOUND_TITLE_LABEL_CTRL_ID;

    GUI_RECT_T iconRect = { WATCH_SET_DEFAULT_ICON_X, WATCH_SET_DEFAULT_ICON_Y, WATCH_SET_DEFAULT_ICON_X+WATCH_SET_DEFAULT_ICON_W, WATCH_SET_DEFAULT_ICON_Y+WATCH_SET_DEFAULT_ICON_H };
    MMI_CTRL_ID_T ctrlIcon = MMISET_NOTE_SOUND_ICON_LABEL_CTRL_ID;

    MMI_CTRL_ID_T leftBtn = MMISET_NOTE_SOUND_L_BTN_CTRL_ID;
    MMI_CTRL_ID_T rightBtn = MMISET_NOTE_SOUND_R_BTN_CTRL_ID;

    BOOLEAN reCode = TRUE;

    //title label
    GUILABEL_SetRect( ctrlLabel, &labelRect, FALSE );
#ifdef SCREEN_SHAPE_CIRCULAR
    GUILABEL_SetFont( ctrlLabel, WATCH_FONT_22, MMI_WHITE_COLOR );
#else
    GUILABEL_SetFont( ctrlLabel, WATCH_DEFAULT_BIG_FONT, MMI_WHITE_COLOR );
#endif
    GUILABEL_SetTextById( ctrlLabel, TXT_SET_SOUND_LIST_NOTE_SOUND, FALSE );

    //icon label
    GUILABEL_SetRect( ctrlIcon, &iconRect, FALSE );

    //button
    WatchCOM_Btn_SetPos_2Btn( leftBtn, rightBtn );
    reCode= GUIBUTTON_SetCallBackFunc( leftBtn, HandleSettingsNoteSoundBtnLeft );
    if( TRUE != reCode )
    {
        TRACE_APP_SETTINGS("set left btn callback error");
    }
    reCode= GUIBUTTON_SetCallBackFunc( rightBtn, HandleSettingsNoteSoundBtnRight );
    if( TRUE != reCode )
    {
        TRACE_APP_SETTINGS("set right btn callback error");
    }
}

LOCAL void Settings_NoteSound_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    //bg color
    WATCHCOM_DisplayBackground( win_id );

    //icon label
    SettingsNoteSoundSetIconId( MMISET_NOTE_SOUND_ICON_LABEL_CTRL_ID );

    //refer to spec not gray buttion
    //button
    //SettingsNoteSoundSetBtnStatus( MMISET_NOTE_SOUND_L_BTN_CTRL_ID, MMISET_NOTE_SOUND_R_BTN_CTRL_ID );
}

LOCAL void Settings_NoteSound_APP_LEFT_RIGHT( MMI_MESSAGE_ID_E msg_id )
{
    if( MSG_APP_LEFT == msg_id )
    {
        HandleSettingsNoteSoundBtnLeft();
    }
    else
    {
        HandleSettingsNoteSoundBtnRight();
    }
}

LOCAL void Settings_NoteSound_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_NoteSound_CLOSE_WINDOW( MMI_WIN_ID_T win_id )
{
    Settings_StopPreviewRing();
}

LOCAL MMI_RESULT_E  HandleSettingsNoteSoundWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Settings_NoteSound_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            Settings_NoteSound_FULL_PAINT( win_id );
            break;
        }

        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
        {
            Settings_NoteSound_APP_LEFT_RIGHT( msg_id );
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_NoteSound_APP_CANCEL( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            Settings_NoteSound_CLOSE_WINDOW( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> sound -> note sound
WINDOW_TABLE( MMISET_NOTE_SOUND_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsNoteSoundWindow ),
    WIN_ID( MMISET_NOTE_SOUND_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMISET_NOTE_SOUND_TITLE_LABEL_CTRL_ID ),
    CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMISET_NOTE_SOUND_ICON_LABEL_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_DECREASE, MMISET_NOTE_SOUND_L_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_INCREASE, MMISET_NOTE_SOUND_R_BTN_CTRL_ID ),
    WIN_SOFTKEY( TXT_NULL, TXT_NULL, STXT_RETURN ),
    END_WIN
};

PUBLIC void Settings_NoteSoundWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_NOTE_SOUND_WIN_TAB, PNULL );
}

