/*****************************************************************************
** File Name:               mmiap_setting.c                                  *
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

#define _MMIAP_SETTING_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiap_setting.h"
#include "mmimp3_nv.h"
#include "mmiset_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMP3_DEFAULT_EQ_MODE      MMISRVAUD_EQ_REGULAR         //Mp3 default eq mode

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
//  Description : Get all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void GetMp3SettingInfo(
    MMIMP3_SETTINGS_INFO_T *set_info_ptr
);

/*****************************************************************************/
//  Description : Get Mp3 Default CodeType
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_TXT_CODE_TYPE_E GetMp3DefaultCodeType(
    void
);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : Reset all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void GetMp3DefaultSetting(
    MMIMP3_SETTINGS_INFO_T *set_info_ptr
)
{
    if(PNULL == set_info_ptr)
    {
        return;
    }

    SCI_MEMSET(set_info_ptr, 0x00, sizeof(MMIMP3_SETTINGS_INFO_T));

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    set_info_ptr->display_style    = MMIMP3_DISPLAY_ALBUM;
#else
    set_info_ptr->display_style    = MMIMP3_DISPLAY_NONE;
#endif
    set_info_ptr->eq_mode               = MMIMP3_DEFAULT_EQ_MODE;
    set_info_ptr->repeat_mode           = MMIMP3_REPEAT_OFF;
    set_info_ptr->shuffle_mode          = MMIMP3_SHUFFLE_OFF;
    set_info_ptr->code_type             = GetMp3DefaultCodeType();
}

/*****************************************************************************/
//  Description : Get Mp3 Default CodeType
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_TXT_CODE_TYPE_E GetMp3DefaultCodeType(
    void
)
{
    MMIMP3_TXT_CODE_TYPE_E code_type = MMIMP3_TXT_CODE_TYPE_GB;
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;

    MMIAPISET_GetLanguageType(&lang_type);

#if defined BIG5_SUPPORT

    if(MMISET_LANGUAGE_TRAD_CHINESE == lang_type)
    {
        code_type = MMIMP3_TXT_CODE_TYPE_BIG5;
    }
    else
#endif
    {
        code_type = MMIMP3_TXT_CODE_TYPE_GB;
    }

    return code_type;
}

/*****************************************************************************/
//  Description : Get all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void GetMp3SettingInfo(MMIMP3_SETTINGS_INFO_T *set_info_ptr)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    if(PNULL == set_info_ptr)
    {
        return;
    }

    MMINV_READ(MMINV_MP3_SET_INFO, set_info_ptr, return_value);

    if(MN_RETURN_SUCCESS != return_value)
    {
        GetMp3DefaultSetting(set_info_ptr);

        MMINV_WRITE(MMINV_MP3_SET_INFO, set_info_ptr);
    }
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetDisplayStyle(MMIMP3_DISPLAY_STYLE_E  style)
{
    MMIMP3_SETTINGS_INFO_T  settings_info = {0};    /*lint !e64*/

    GetMp3SettingInfo(&settings_info);
    settings_info.display_style = style;
    MMINV_WRITE(MMINV_MP3_SET_INFO, &settings_info);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_DISPLAY_STYLE_E APSETTING_GetDisplayStyle(void)
{
    MMIMP3_SETTINGS_INFO_T setting_info = {0};  /*lint !e64*/

    GetMp3SettingInfo(&setting_info);

    return (setting_info.display_style);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetEqMode(MMISRVAUD_EQ_MODE_E eq_mode)
{
    MMIMP3_SETTINGS_INFO_T  settings_info = {0};    /*lint !e64*/

    GetMp3SettingInfo(&settings_info);
    settings_info.eq_mode = eq_mode;
    MMINV_WRITE(MMINV_MP3_SET_INFO, &settings_info);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_EQ_MODE_E APSETTING_GetEqMode(void)
{
    MMIMP3_SETTINGS_INFO_T setting_info = {0};  /*lint !e64*/

    GetMp3SettingInfo(&setting_info);

    return (setting_info.eq_mode);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetRepeatMode(MMIMP3_REPEAT_MODE_E repeat_mode)
{
    MMIMP3_SETTINGS_INFO_T  settings_info = {0};    /*lint !e64*/

    GetMp3SettingInfo(&settings_info);
    settings_info.repeat_mode = repeat_mode;
    MMINV_WRITE(MMINV_MP3_SET_INFO, &settings_info);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_REPEAT_MODE_E APSETTING_GetRepeatMode(void)
{
    MMIMP3_SETTINGS_INFO_T setting_info = {0};  /*lint !e64*/

    GetMp3SettingInfo(&setting_info);

    return (setting_info.repeat_mode);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetShuffleMode(MMIMP3_SHUFFLE_MODE_E shuffle_mode)
{
    MMIMP3_SETTINGS_INFO_T  settings_info = {0};    /*lint !e64*/

    GetMp3SettingInfo(&settings_info);
    settings_info.shuffle_mode = shuffle_mode;
    MMINV_WRITE(MMINV_MP3_SET_INFO, &settings_info);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_SHUFFLE_MODE_E APSETTING_GetShuffleMode(void)
{
    MMIMP3_SETTINGS_INFO_T setting_info = {0};  /*lint !e64*/

    GetMp3SettingInfo(&setting_info);

    return (setting_info.shuffle_mode);
}

/*****************************************************************************/
// Description : 获得编码方式
// Global resource dependence :
// Author: xingdong.li
// Note:
/*****************************************************************************/
PUBLIC MMIMP3_TXT_CODE_TYPE_E APSETTING_GetCodeType(void)
{
    MMIMP3_SETTINGS_INFO_T setting_info = {0};  /*lint !e64*/
    //set play info
    GetMp3SettingInfo(&setting_info);

    return (setting_info.code_type);
}

/*****************************************************************************/
//  Description : get display mp3 lyric
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetCodeType(
    MMIMP3_TXT_CODE_TYPE_E code_type
)
{
    MMIMP3_SETTINGS_INFO_T  settings_info = {0};    /*lint !e64*/

    GetMp3SettingInfo(&settings_info);

    settings_info.code_type = code_type;

    MMINV_WRITE(MMINV_MP3_SET_INFO, &settings_info);
}


/*****************************************************************************/
//  Description : Reset all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_ResetMp3SettingInfo(void)
{
    MMIMP3_SETTINGS_INFO_T  setting_info = {0}; /*lint !e64*/

    GetMp3DefaultSetting(&setting_info);
    MMINV_WRITE(MMINV_MP3_SET_INFO, &setting_info);
}

/*****************************************************************************/
//  Description : Get the mp3 play mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_MODE_E MMIMP3_GetMp3PlayMode(
    void
)
{
    MMIMP3_PLAY_MODE_E play_mode = MMIMP3_PLAY_LIST_ONCE;
    MMIMP3_SHUFFLE_MODE_E  shuffle_mode = APSETTING_GetShuffleMode();

    switch(APSETTING_GetRepeatMode())
    {
    case MMIMP3_REPEAT_OFF:
        if(MMIMP3_SHUFFLE_ON == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_RANDOM_ONCE;
        }
        else if(MMIMP3_SHUFFLE_OFF == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_LIST_ONCE;
        }

        break;

    case MMIMP3_REPEAT_ONE:

        //@CR241305 2011.05.24
        if(MMIMP3_SHUFFLE_ON == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_RANDOM_REPEAT;
        }
        else
        {
            play_mode = MMIMP3_PLAY_ONE_REPEAT;
        }

        //@CR241305 2011.05.24
        break;

    case MMIMP3_REPEAT_ALL:
        if(MMIMP3_SHUFFLE_ON == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_RANDOM_REPEAT;
        }
        else if(MMIMP3_SHUFFLE_OFF == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_LIST_REPEAT;
        }

        break;

    default:
        break;
    }

    return play_mode;
}

//@CR241305 2011.05.24
/*****************************************************************************/
//  Description : get mp3 play mode.
//  Global resource dependence :
//  Author:
//  Note: 当播放完成正常结束时，获取下一首前调用此函数获取播放模式，与按左右键等
//          区别开来
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_MODE_E MMIMP3_GetMp3PlayModeAuto(void)
{
    MMIMP3_PLAY_MODE_E play_mode = MMIMP3_PLAY_LIST_ONCE;
    MMIMP3_SHUFFLE_MODE_E  shuffle_mode = APSETTING_GetShuffleMode();

    switch(APSETTING_GetRepeatMode())
    {
    case MMIMP3_REPEAT_OFF:
        if(MMIMP3_SHUFFLE_ON == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_RANDOM_ONCE;
        }
        else if(MMIMP3_SHUFFLE_OFF == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_LIST_ONCE;
        }

        break;

    case MMIMP3_REPEAT_ONE:
        play_mode = MMIMP3_PLAY_ONE_REPEAT;
        break;

    case MMIMP3_REPEAT_ALL:
        if(MMIMP3_SHUFFLE_ON == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_RANDOM_REPEAT;
        }
        else if(MMIMP3_SHUFFLE_OFF == shuffle_mode)
        {
            play_mode = MMIMP3_PLAY_LIST_REPEAT;
        }

        break;

    default:
        break;
    }

    return play_mode;
}
//@CR241305 2011.05.24

