/*****************************************************************************
** File Name:      mmiss_export.h                                      *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine              Creat
******************************************************************************/

#ifndef _MMIIDLE_SCREENSAVER_H_
#define _MMIIDLE_SCREENSAVER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC uint8 MMIAPIIDLESS_StartTimer(
//                               MMI_WIN_ID_T  win_id
//                               );

/*****************************************************************************/
//  Description : stop screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPIIDLESS_StopTimer(
//                             void
//                             );
//屏保定时器改为系统级
#define MMIAPIIDLESS_StopTimer()
/*****************************************************************************/
// 	Description : open screen saver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_OpenScreenSaver(void);

/*****************************************************************************/
// 	Description : close screen saver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_CloseScreenSaver(void);

/*****************************************************************************/
// 	Description : To judge whether the screensaver is mpeg4 type.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLESS_IsScreenSaverMp4Type(void);

/*****************************************************************************/
// 	Description : Reset ssaver to default.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_SetDefaultScreenSaver(void);

/*****************************************************************************/
//  Description : restart screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMIDLE_RestartSsTimer(
//                                   MMI_WIN_ID_T  win_id
//                                   );
//屏保定时器改为系统级
#define MMIDLE_RestartSsTimer( win_id )
/*****************************************************************************/
//  Description : is screen saver timer id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIIDLESS_IsScreenSaverTimerId(DPARAM param);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


