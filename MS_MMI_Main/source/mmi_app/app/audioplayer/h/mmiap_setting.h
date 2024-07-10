/*****************************************************************************
** File Name:               mmiap_setting.h                                  *
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
#ifndef _MMIAP_SETTING_H_
#define _MMIAP_SETTING_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrvaud_api.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
//Mp3 play mode
typedef enum
{
    //MMIMP3_PLAY_ONE_ONCE,      //单首播放一次
    MMIMP3_PLAY_ONE_REPEAT,    //单首重复播放
    MMIMP3_PLAY_LIST_ONCE,      //列表循环一次
    MMIMP3_PLAY_LIST_REPEAT,   //列表重复循环
    MMIMP3_PLAY_RANDOM_REPEAT, //列表重复随机
    MMIMP3_PLAY_RANDOM_ONCE,   //列表随机一次
    MMIMP3_PLAY_MAX_MODE
} MMIMP3_PLAY_MODE_E;

//mp3 display style
typedef enum
{
    MMIMP3_DISPLAY_NONE,        //不显示
#ifdef MMIAP_LYRIC_SUPPORT 
    MMIMP3_DISPLAY_LYRIC_ON,    //显示
    MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT,  //背光常亮显示
#endif    
#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIMP3_DISPLAY_SPECTRUM,    //频谱
#endif
#ifdef MMI_AUDIOPLAYER_ANIM_SUPPORT
    MMIMP3_DISPLAY_ANIM,        //动画
#endif
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    MMIMP3_DISPLAY_ALBUM,       //专辑
#endif
    MMIMP3_DISPLAY_TYPE_MAX
} MMIMP3_DISPLAY_STYLE_E;


//Mp3 play mode
typedef enum
{
    MMIMP3_REPEAT_OFF,
    MMIMP3_REPEAT_ALL,
    MMIMP3_REPEAT_ONE,
    MMIMP3_REPEAT_MAX
} MMIMP3_REPEAT_MODE_E;

//Mp3 play mode
typedef enum
{
    MMIMP3_SHUFFLE_ON,
    MMIMP3_SHUFFLE_OFF,
    MMIMP3_SHUFFLE_MAX
} MMIMP3_SHUFFLE_MODE_E;

//the type of time display
typedef enum
{
    MMIMP3_TXT_CODE_TYPE_GB,
    MMIMP3_TXT_CODE_TYPE_BIG5,
    MMIMP3_TXT_CODE_TYPE_MAX
 } MMIMP3_TXT_CODE_TYPE_E;
 
//Mp3 settings info
typedef struct
{
    MMIMP3_DISPLAY_STYLE_E          display_style;
    MMISRVAUD_EQ_MODE_E             eq_mode;            //eq mode
    MMIMP3_REPEAT_MODE_E            repeat_mode;
    MMIMP3_SHUFFLE_MODE_E           shuffle_mode;
    MMIMP3_TXT_CODE_TYPE_E          code_type;
} MMIMP3_SETTINGS_INFO_T;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Get the mp3 play mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_MODE_E MMIMP3_GetMp3PlayMode(
                                void
                                );

/*****************************************************************************/
//  Description : get mp3 play mode.
//  Global resource dependence : 
//  Author: 
//  Note: 当播放完成正常结束时，获取下一首前调用此函数获取播放模式，与按左右键等
//          区别开来
/*****************************************************************************/
PUBLIC MMIMP3_PLAY_MODE_E MMIMP3_GetMp3PlayModeAuto(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetDisplayStyle(MMIMP3_DISPLAY_STYLE_E  style);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_DISPLAY_STYLE_E APSETTING_GetDisplayStyle(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetEqMode(MMISRVAUD_EQ_MODE_E eq_mode);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_EQ_MODE_E APSETTING_GetEqMode(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetRepeatMode(MMIMP3_REPEAT_MODE_E repeat_mode);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_REPEAT_MODE_E APSETTING_GetRepeatMode(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetShuffleMode(MMIMP3_SHUFFLE_MODE_E shuffle_mode);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_SHUFFLE_MODE_E APSETTING_GetShuffleMode(void);

/*****************************************************************************/
// Description : 获得编码方式
// Global resource dependence : 
// Author: xingdong.li
// Note:
/*****************************************************************************/
PUBLIC MMIMP3_TXT_CODE_TYPE_E APSETTING_GetCodeType( void );

/*****************************************************************************/
//  Description : get display mp3 lyric
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_SetCodeType(
                    MMIMP3_TXT_CODE_TYPE_E code_type
                    );

/*****************************************************************************/
//  Description : Reset all setting info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void APSETTING_ResetMp3SettingInfo(void);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif


