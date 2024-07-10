/*****************************************************************************
** File Name:               mmiap_control.c                                  *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/

#define _MMIAP_CONTROL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiap_control.h"
#include "mmiap_setting.h"

#ifdef MMIAP_SPECTRUM_SUPPORT
#include "mmiap_spectrum.h"
#endif

#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#ifdef MMIAP_F_HISTORY_PLAYLIST
#include "mmiap_lastinfo_ctrl.h"
#endif


#include "mmiap_list_control.h"

#include "mmiap_list.h"
#include "mmimp3_export.h"
#include "mmiap_play.h"

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
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void DisplayStyleChanged(MMIMP3_DISPLAY_STYLE_E  style);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetDisplayStyle(
    MMIMP3_DISPLAY_STYLE_E  style
)
{
    if(style != APSETTING_GetDisplayStyle())
    {
        APSETTING_SetDisplayStyle(style);
        DisplayStyleChanged(style);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void DisplayStyleChanged(MMIMP3_DISPLAY_STYLE_E  style)
{
#ifdef MMIAP_SPECTRUM_SUPPORT

    if(MMIMP3_DISPLAY_SPECTRUM == style)
    {
        MMIMP3_StartSpectrum();
    }
    else
    {
        MMIMP3_StopSpectrum(FALSE);
    }

#endif
#ifdef MMIAP_LYRIC_SUPPORT

    if(MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == style)
    {
        MMIMP3_HandleLyricBackLight(TRUE);
    }
    else
    {
        MMIMP3_HandleLyricBackLight(FALSE);
    }

#endif
}

/*****************************************************************************/
//  Description : set the mode of mp3
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetShuffleMode(
    MMIMP3_SHUFFLE_MODE_E  shuffle_mode//in
)
{
    if(MMIMP3_SHUFFLE_ON > shuffle_mode || MMIMP3_SHUFFLE_MAX <= shuffle_mode)
    {
        return;
    }

    if(shuffle_mode != APSETTING_GetShuffleMode())
    {
        APSETTING_SetShuffleMode(shuffle_mode);
        MMIMP3_SetCurPlayMode(MMIMP3_GetMp3PlayMode());
    }
}

/*****************************************************************************/
//  Description : set the mode of mp3
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetRepeatMode(
    MMIMP3_REPEAT_MODE_E  repeat_mode//in
)
{
    if(MMIMP3_REPEAT_OFF > repeat_mode || MMIMP3_REPEAT_MAX <= repeat_mode)
    {
        return;
    }

    if(repeat_mode != APSETTING_GetRepeatMode())
    {
        APSETTING_SetRepeatMode(repeat_mode);
        MMIMP3_SetCurPlayMode(MMIMP3_GetMp3PlayMode());
    }
}


/*****************************************************************************/
//  Description : Reset all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_ReSetFactory(
    void
)
{
    APSETTING_ResetMp3SettingInfo();
    DisplayStyleChanged(APSETTING_GetDisplayStyle());
    MMIAPIMP3_EnableEq(APSETTING_GetEqMode());
    MMIMP3_SetCurPlayMode(MMIMP3_GetMp3PlayMode());

#ifdef MMIAP_F_HISTORY_PLAYLIST
    MMIAP_ResetLastInfoNV();
#endif
}

/*****************************************************************************/
//  Description : MMI API MP3 Restore Eq Setting
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_RestoreEqSetting(void)
{
#ifdef MP3_EQ_SUPPORT
    MMISRVAUD_EQ_MODE_E eq_mode = APSETTING_GetEqMode();

    if(MMIAPIMP3_IsPlayerPlaying())
    {
        MMISRVAUD_SetEQMode(MMIAP_GetAPAudioSrvHandle(), eq_mode);
    }

#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_EnableEq(MMISRVAUD_EQ_MODE_E  eq_mode)
{
#ifdef MP3_EQ_SUPPORT
    MMISRVAUD_SetEQMode(MMIAP_GetAPAudioSrvHandle(), eq_mode);
#endif
}

