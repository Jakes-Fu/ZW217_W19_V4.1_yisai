/*****************************************************************************
** File Name:      mmiset_internal.h                                                  *
** Author:                                                                   *
** Date:           09/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2004       Jassmine.Meng       Create
   02/2012       xiyuan.ma           edite
******************************************************************************/

#ifndef _MMISET_INTERNAL_H_
#define _MMISET_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiset_export.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "guiownerdraw.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
** Macro Declaration *
**---------------------------------------------------------------------------*/
#ifdef MMI_GUI_STYLE_MINISCREEN
#ifdef MAINLCD_DEV_SIZE_220X176
#define MMISET_DISPLAY_CONTRAST_BAR_LONG    180
#elif defined MAINLCD_DEV_SIZE_176X220
#define MMISET_DISPLAY_CONTRAST_BAR_LONG    130
#else
#define MMISET_DISPLAY_CONTRAST_BAR_LONG    90
#endif

#define MMISET_DISPLAY_CONTRAST_BAR_HIGHT    12
#define MMISET_DISPLAY_CONTRAST_HIGHLIGHT_TOP_OFFSET     3
#define MMISET_DISPLAY_CONTRAST_HIGHLIGHT_LEFT_OFFSET    2
#define MMISET_DISPLAY_CONTRAST_HIGHLIGHT_RIGHT_OFFSET  2
#ifndef TOUCH_PANEL_SUPPORT  //Touch情况下不需要三角提示
#define MMISET_DISPLAY_CONTRAST_TRIANGLE_TOP_OFFSET     2
#define MMISET_DISPLAY_CONTRAST_TRIANGLE_LEFT_OFFSET    10
#define MMISET_DISPLAY_CONTRAST_TRIANGLE_RIGHT_OFFSET   8
#endif
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 添加线路选择的radio list的item
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendLineListItem(
    MN_DUAL_SYS_E dual_sys,
    MMI_CTRL_ID_T       ctrl_id
    );

/*****************************************************************************/
//  Description : 设置中线路选择的结果
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSelectLine(
                                 MN_DUAL_SYS_E      dual_sys,                            
                                 MMISET_LINE_TYPE_E  line_type
                                 );

//@zhaohui,@cr96828
/*****************************************************************************/
//  Description : 网络选择后，等待搜网成功,这中间只响应关机键
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleWaitSearchNetworkWindow(
                                                 MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                 DPARAM             param       // 相应消息的参数
                                                 );

#ifdef MULTI_THEME_SUPPORT
/*****************************************************************************/
//  Description : set current theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetCurrentTheme(uint16 cur_selection, const wchar * theme_file_name);
#endif

#ifdef DPHONE_SUPPORT 
/************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
PUBLIC void MMISET_SetSpeedDialDelay(uint32 time);
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : set is blacklist audio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsBlacklistAudioCall(BOOLEAN is_black_audio_call); 

/*****************************************************************************/
//  Description : set is blacklist vedio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsBlacklistVedioCall(BOOLEAN is_black_vedio_call); 

/*****************************************************************************/
//  Description : set is blacklist message
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsBlacklistMessage(BOOLEAN is_black_mesage); 
#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetTTS(
                          BOOLEAN	    is_tts_open
                          );
#endif

/*****************************************************************************/
// 	Description : set call divert/bar/wating/hide id ctrl id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetDivertBarWatingHideidCtrlId(MMI_CTRL_ID_T ctri_id);

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : Display Version info window
//  Global resource dependence : 
//  Author: hongying wang
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_EnterVersionDisplayWin(void);
#endif

/*****************************************************************************/
//  Description : MMISET_ContrastBarCtrlCallbackFun
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_ContrastBarCtrlCallbackFun (GUIOWNDRAW_INFO_T *draw_ptr);

/*****************************************************************************/
//  Description : HandleContrastTPDown
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: for user touch bar support
/*****************************************************************************/
PUBLIC void MMISET_HandleContrastTPDown(
                                GUI_POINT_T     *point_ptr,
                                MMI_WIN_ID_T    win_id,
                                MMI_CTRL_ID_T   ctrl_id,
                                BOOLEAN         is_called_by_ps//是否由省电模式呼叫
                                );
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_CreateDateTimeWin(void);
                                
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
//  Description : set the time of key board light
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetKeyBoardLightTime(
                                MMISET_KEYBOARD_LIGHT_SET_T  keyboard_light_info
                                );
                                
/*****************************************************************************/
//  Description :MMISET_CloseAllAnimSet
//  Global resource dependence : 
//  Author: juan.wu 
//  Note:关闭所有动画效果，供省电模式使用
/*****************************************************************************/
PUBLIC void MMISET_CloseAllAnimSet(void);
#endif
/*****************************************************************************/
//  Description : MMISET_GetMenuStyleTxt
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T  MMISET_GetMenuStyleTxt(void);

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : MMISET_GetUIdleStyleTxt
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMISET_GetUIdleStyleTxt(void);

/*****************************************************************************/
// 	Description : set u idle effect style
//	Global resource dependence : 
//  Author: hongbo.lan
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetUIdleStyle(uint16 uidle_style);
#endif

/*****************************************************************************/
// 	Description : MMISET_SetAutoLockKeyPadList
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetAutoLockKeyPadList(void);

/*****************************************************************************/
// 	Description : MMISET_SaveAutoKeyPadList
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SaveAutoKeyPadList(void);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
