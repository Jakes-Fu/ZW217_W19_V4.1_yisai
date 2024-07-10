/******************************************************************************
文件名称:mmipim_wintab.h
文件描述:UI窗口管理
文件说明:
开发人员:王晓林
首次开发日期:2007-9.26
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2007-09-26         王晓林                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/

#ifndef _MMIPIM_WINTAB_H_
#define _MMIPIM_WINTAB_H_ 
#include "mn_type.h"
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
 /*#ifdef MAINLCD_SIZE_176X220 
 #define PIM_NOTE_MSG_HEIGHT 21 // chenxiang 20080806
 #else
#define PIM_NOTE_MSG_HEIGHT 25 // chenxiang 20080806
#endif

#define PIM_NOTE_MSG_TOP MMI_TITLE_HEIGHT    
#define PIM_NOTE_MSG_BOTTOM (MMI_TITLE_HEIGHT +PIM_NOTE_MSG_HEIGHT)
#define PIM_NOTE_MSG_RIGHT (MMI_MAINSCREEN_WIDTH - 1)

#ifdef MAINLCD_SIZE_176X220   //add by wangxiaolin 2010.07.02
#define PIM_PRGBOX_LEFT 15
#define PIM_PRGBOX_TOP 175
#define PIM_PRGBOX_RIGHT MMI_MAINSCREEN_WIDTH-PIM_PRGBOX_LEFT
#define PIM_PRGBOX_BOTTOM (PIM_PRGBOX_TOP + 15)
#else
#define PIM_PRGBOX_LEFT 30
#define PIM_PRGBOX_TOP 240
#define PIM_PRGBOX_RIGHT MMI_MAINSCREEN_WIDTH-PIM_PRGBOX_LEFT
#define PIM_PRGBOX_BOTTOM (PIM_PRGBOX_TOP + 15)
#endif*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description :打开号簿管家主菜单窗口
//  Global resource dependence : none
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIM_OpenMainMenuWin(PIM_ENTRY_TYPE entry_type);

/*****************************************************************************/
//  Description :打开号簿管家调试参数设置窗口
//  Global resource dependence : none
//  Author: chenxiang
//  Note: 20080506
/*****************************************************************************/
PUBLIC void MMIPIM_OpenDebugMenuWin(void);
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: chenxiang
//  Note: 20080506
/*****************************************************************************/
PUBLIC void MMIPIM_OpenNetQueryWinFromPB(void);

/*****************************************************************************/
//  Description :设置当前使用的sim 卡
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPIM_GetSim(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

