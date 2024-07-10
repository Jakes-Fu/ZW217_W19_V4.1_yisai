/*****************************************************************************
** File Name:      mmiidle_cstyle.c                                          *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Creat
******************************************************************************/
#ifndef _MMIIDLE_CSTYLE_H_
#define _MMIIDLE_CSTYLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmiidle_export.h"
#include "mmimp3_export.h"
#include "mmifm_internal.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)
// the mp3 info
//#define MMIMP3_NAME_MAX_LEN  (MMIFM_CHANNEL_NAME_LEN+MMIFM_FREQ_MAX_LEN)
typedef struct
{
    BOOLEAN     is_scroll; //是否滚动
    //wchar       mp3_name[MMIMP3_NAME_MAX_LEN + 1]; //mp3名字
    //uint16      length;         //mp3名字长度
    uint16      str_width;   //mp3名字的宽度
    uint16      str_index;  //mp3名字中字符的索引
} IDLE_MP3_NAME_INFO;
#elif FM_SUPPORT
#define MMIMP3_NAME_MAX_LEN (MMIFM_CHANNEL_NAME_LEN+MMIFM_FREQ_MAX_LEN)
typedef struct
{
    BOOLEAN     is_scroll; //是否滚动
    //wchar       mp3_name[MMIMP3_NAME_MAX_LEN + 1]; //mp3名字
    //uint16      length;         //mp3名字长度
    uint16      str_width;   //mp3名字的宽度
    uint16      str_index;  //mp3名字中字符的索引
} IDLE_MP3_NAME_INFO;
#endif 
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle common idle message
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_CommonHandleMsg ( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param );

/*****************************************************************************/
//  Description : set idle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBgPlayName(
                                BOOLEAN     is_display,
                                MMIBGPLAY_TYPE_E bg_type
                                );
#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : get idle mp3 info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_MP3_NAME_INFO* MMIIDLECOM_GetIdleMp3Info(void);
#endif

/*****************************************************************************/
//  Description : start mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLECOM_StartIdleMp3Timer(void);

/*****************************************************************************/
//  Description : stop mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLECOM_StopIdleMp3Timer(void);

/*****************************************************************************/
// Description : UpdateMp3NameStartIndex
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIIDLECOM_UpdateMp3NameStartIndex(void);

/*****************************************************************************/
// Description : MMIIDLECOM_GetMp3NameTimerId
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIIDLECOM_GetMp3NameTimerId(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
