/*****************************************************************************
** File Name:      mmipub_internal.h                                         *
** Author:                                                                   *
** Date:           06/20/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe pub win internal            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011       James.Zhang           Creat                                 *
******************************************************************************/

#ifndef _MMIPUB_INTERNAL_H_
#define _MMIPUB_INTERNAL_H_ 

//禁止其他模块包含该头文件
#if !defined _MMI_PUBWIN_C_ && !defined _MMI_PUBLISTWIN_C_ && !defined _MMITHEME_PUBWIN_C_ && !defined _MMI_PUBFORMWIN_C_ && !defined _MMI_PUBEDITWIN_C_
#error ONLY mmi_pubxxxx.c CAN INCLUDE mmipub_internal.h!!!
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipub.h"
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
#define    MMIPUBWIN_NONE           0
#define    MMIPUBWIN_ALERT          (MMIPUBWIN_NONE+1)
#define    MMIPUBWIN_QUERY          (MMIPUBWIN_NONE+2)
#define    MMIPUBWIN_ALERT_TEXT     (MMIPUBWIN_NONE+3)
#define    MMIPUBWIN_QUERY_TEXT     (MMIPUBWIN_NONE+4)
#define    MMIPUBWIN_WAIT           (MMIPUBWIN_NONE+5)
#define    MMIPUBWIN_WAITING        (MMIPUBWIN_NONE+6)
#define    MMIPUBWIN_PROGRESS       (MMIPUBWIN_NONE+7)
#define    MMIPUBWIN_LIST           (MMIPUBWIN_NONE+8)
#define    MMIPUBWIN_FORM           (MMIPUBWIN_NONE+9)
//#define    MMIPUBWIN_EDIT           (MMIPUBWIN_NONE+10)
#define    MMIPUBWIN_ALARM_DIALOG   (MMIPUBWIN_NONE+12)

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/ 

typedef void (* PUBWIN_INIT_FUNC)( MMIPUB_INFO_T* win_info_ptr, MMIPUB_PARAM_T* param );

typedef struct
{
    PROCESSMSG_FUNC    proc_func;
    PUBWIN_INIT_FUNC   init_func;
    uint32             type;
    MMI_WIN_ID_T       default_win_id;
} MMIPUB_INIT_T;

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
extern const MMIPUB_INIT_T g_publistwin_init;
//extern const MMIPUB_INIT_T g_pubeditwin_init;
extern const MMIPUB_INIT_T g_pubformwin_init;
#endif

/*****************************************************************************/
//  Description : get pubwin bg rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: for MMIPUBWIN_ALERT MMIPUBWIN_QUERY MMIPUBWIN_WAIT
/*****************************************************************************/
PUBLIC void MMIPUBLayout(
                         MMIPUB_INFO_T *win_info_ptr
                         );
/*****************************************************************************/
//  Description : convert button message to softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:PDA工程,将button消息转为softkey消息,兼容
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUBHandleButtonMsg(
                                          MMIPUB_INFO_T*    win_info_ptr,
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : creat softkey control
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBCreatSoftkeyCtrl(
                                   MMI_HANDLE_T		win_handle     //win id
                                   );

/*****************************************************************************/
//  Description : set button
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBSetButton(
                            MMIPUB_INFO_T *win_info_ptr,
                            BOOLEAN is_fresh
                            );

/*****************************************************************************/
//  Description : Handle public windows softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBHandlePubwinSoftkey(
                                      BOOLEAN                  is_right_softkey,
                                      MMI_WIN_ID_T             win_id,
                                      MMIPUB_SOFTKEY_STYLE_E   softkey_style
                                      );

/*****************************************************************************/
//  Description : display public window
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBDisplayWin(
                             MMIPUB_INFO_T    *win_info_ptr
                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:处理关闭窗口消息
/*****************************************************************************/
PUBLIC void MMIPUB_DestroyWinData(
                                  MMIPUB_INFO_T* win_info_ptr
                                  );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:处理关闭窗口消息
/*****************************************************************************/
PUBLIC void MMIPUB_HandleLcdSwitch(
                                   MMIPUB_INFO_T* win_info_ptr
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:release layer
/*****************************************************************************/
PUBLIC void MMIPUB_ReleaseLayer(
                                MMIPUB_INFO_T* win_info_ptr
                                );

/*****************************************************************************/
//  Description : handle window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUBHandleWinMsg(
                                       MMIPUB_INFO_T*    win_info_ptr,
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/*****************************************************************************/
//  Description : clear client rect bg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBClearClientRectBg(
                                    MMIPUB_INFO_T* win_info_ptr,
                                    GUI_RECT_T*    rect_ptr
                                    );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
