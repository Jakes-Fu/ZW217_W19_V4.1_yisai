/*****************************************************************************
** File Name:      mmiqbtheme_idlewin.h                                          *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_IDLEWIN_H_
#define _MMIQBTHEME_IDLEWIN_H_ 


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
//  Description : add new shortcut item to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_AddNewShortcut(MMIMAINMENU_ITEM_INFO_T *menu_info_ptr);

/*****************************************************************************/
//  Description : add new widget to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_AddNewWidget(uint32 widget_id);


/*****************************************************************************/
//  Description : create the widget control.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_Create(
                              MMI_WIN_ID_T win_id,
                              MMIWIDGET_CTRL_E  ctrl_id,
                              GUI_BOTH_RECT_T *both_rect_ptr
                              );
/*****************************************************************************/
//  Description : destroy the widget control.
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBIDLE_Destroy(
                            MMIWIDGET_CTRL_E  ctrl_id
                            );

//set state of widget page, and refresh background
PUBLIC int32 MMIQBIDLE_SetWidgetWinMode(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                          MMIGRID_RUN_MODE_E          run_mode
                                          );
                                          

/*****************************************************************************/
//  Description :  set current page
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_SetCurPageIndex(
                                       int32 page_index
                                       );
                                       
PUBLIC BOOLEAN MMIQBIDLE_SetNewPageIndex(
                                       int32 page_index
                                       );

/*****************************************************************************/
//  Description :  slide page
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_SlidePage( int32 page_index);

/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBIDLE_UpdateAllScreen(void);


/*****************************************************************************/
//  Description : get current tp status is moveing page or not.
//  Global resource dependence : 
//  Author:  hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_IsForbidPaint(uint32 widget_id);

/*****************************************************************************/
//  Description : set widget layer blt
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBIDLE_SetBltLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

//显示widget页面的缩略图                                          
PUBLIC int32 MMIQBIDLE_DisplayGridPageThumbView(int32 page_index);

//重新设置widget win的相关状态，在从dockbar移动一个menu icon到widget win后
PUBLIC int32 MMIQBIDLE_ResetStateAfterMovingMenu2WidgetWin(GUI_POINT_T point);

/*****************************************************************************/
//  Description : save widget info to NV
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_SaveWidgetInfoToNV(
                                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                          );

/*****************************************************************************/
//  Description : read nv
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBIDLE_ReadWidgetNV(
                        MMI_GRID_SAVE_INFO_T *save_info_ptr
                        );


#ifdef   __cplusplus
    }
#endif
#endif
