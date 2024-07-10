/*****************************************************************************
** File Name:      mmiqbtheme_idleexport.h                                          *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_IDLE_EXPORT_H_
#define _MMIQBTHEME_IDLE_EXPORT_H_ 


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

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIQBIDLE_CreateWin             MMIAPIQBIDLE_CreateWin
#define MMIQBIDLE_HandleMsg             MMIAPIQBIDLE_HandleMsg
#define MMIQBIDLE_GetGridPtr            MMIAPIQBIDLE_GetGridPtr
#define MMIQBIDLE_ResetFactorySetting   MMIAPIQBIDLE_ResetFactorySetting
#define MMIQBTHEME_CreateIdle           MMIAPIQBTHEME_CreateIdle
#define MMIQBTHEME_DestroyIdle          MMIAPIQBTHEME_DestroyIdle

#endif //MMI_FUNC_COMPATIBLE_SUPPORT        


/*****************************************************************************/
//  Description : create qb theme  win
//  Global resource dependence : 
//  Author:ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQBIDLE_CreateWin(void);

/*****************************************************************************/
//  Description : handle widget msg
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIQBIDLE_HandleMsg(
                                      MMI_WIN_ID_T      win_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM            param
                                      );

/*****************************************************************************/
//  Description :  return widget control ptr
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_IDLE_GRID_DATA_T *MMIAPIQBIDLE_GetGridPtr(void);

/*****************************************************************************/
//  Description : Reset Factory Setting
//  Global resource dependence : none
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQBIDLE_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : create idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQBTHEME_CreateIdle(void);

/*****************************************************************************/
//  Description : destroy idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQBTHEME_DestroyIdle(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIQBIDLE_AddMenu2Idle(MMIMAINMENU_ITEM_INFO_T shortcut_info, GUI_POINT_T start_pos);

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIGRID_GetMainAppFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIQBTHEME_GetMainAppFgInfo(LW_FG_INFO *lw_fg_info_ptr);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
