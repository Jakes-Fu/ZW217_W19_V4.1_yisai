/*****************************************************************************
** File Name:      mmiwidget_virtualwin.c                                    *
** Author:                                                                   *
** Date:           08/10/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widget virtual window       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/08/2012     ying.xu              Creat
******************************************************************************/

#ifndef _MMIWIDGET_VIRTUALWIN_H_
#define _MMIWIDGET_VIRTUALWIN_H_ 


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
//  Description : Add the item to widget
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_AppendWidgetItem(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 const MMIWIDGET_ITEM_T   *item_ptr
                                 );
/*****************************************************************************/
//  Description : Init Shortcuts Position
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_InitShortcutsPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                    MMIGRID_SHOTRCUT_PRELOAD_POSITION_T shortcut_init_info);

PUBLIC void	MMIWIDGET_InitWidgetsPosition(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, 
                                        MMIWIDGET_ITEM_INFO_T *item_ptr, 
                                        MMIGRID_WIDGET_PRELOAD_POSITION_T grid_idle_position);

//加载所有的widget
PUBLIC void MMIWIDGET_LoadAllWidget(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                         MMIWIDGET_ITEM_INFO_T *widget_table_ptr[],
                         int32 widget_num                         
                         );
PUBLIC void MMIWIDGET_UnLoadAllWidget(
                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                         );

/*****************************************************************************/
//  Description : check the widget id to add is exist in shorcut or not.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_IsWidgetExistInAddingList(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, uint32 widget_id); 

/*****************************************************************************/
//  Description : insert shortcut
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_InsertWidget2AddingList(
                             MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                             uint32 widget_id,
                             uint32 pos
                             );

/*****************************************************************************/
//  Description : get shortcut idx.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWIDGET_GetShortCutIdx(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                    MMIMAINMENU_ITEM_INFO_T item_info);

/*****************************************************************************/
//  Description : ShortCut GetIndex 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIWIDGET_GetShortCutIdxbyMenuid(MMI_IDLE_GRID_DATA_T *idle_grid_ptr,MMI_MENU_ID_T menu_id);

/*****************************************************************************/
//  Description : remove shortcut item
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_RemoveWidgetInAddingList(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                 uint32 widget_id
                                 );
PUBLIC BOOLEAN MMIWIDGET_ResetShortCutItem(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);
/*****************************************************************************/
//  Description : remove shortcut from idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_RemoveShortcutInAddingList(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, int32 id);  

/*****************************************************************************/
//  Description : MMIGRID_getpagelayersize
//  Global resource dependence : 
//  Author:ying.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWIDGET_GetPageLayerSize(MMI_IDLE_GRID_DATA_T * idle_grid_ptr);

/*****************************************************************************/
//  Description : MMIGRID_get wallpaper layer size
//  Global resource dependence : 
//  Author:ying.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWIDGET_GetWallpaperLayerSize(MMI_IDLE_GRID_DATA_T * idle_grid_ptr);

/*****************************************************************************/
//  Description : create tp move timer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void CreateTpMoveTimer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr , MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void StopTpMoveTimer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : restart tp move timer
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void RestartTpMoveTimer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr ,MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : 设置pagelayer位置（RUNTIME_DRAW的情况记录全局变量，直接画，可以节省内存）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void SetPageLayerPosition(                                  
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 offset_x,
                                int16 offset_y
                                );

/*****************************************************************************/
//  Description : 得到pagelayer位置（RUNTIME_DRAW的情况读取全局变量）
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void GetPageLayerPosition(                                  
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 *offset_x_ptr, //output
                                int16 *offset_y_ptr //output
                                );

/*****************************************************************************/
//  Description : reset widget page layer pos
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void ResetGridPageLayerPos(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                );

/*****************************************************************************/
//  Description : reset widget page layer cahce
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC int32 SetGridPageLayerCacheInvalid(
                                         MMI_IDLE_GRID_DATA_T *idle_grid_ptr,
                                         uint32 index
                                );

PUBLIC int32 UpdateGridPageLayerCache(
                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                          );

                              
#ifdef   __cplusplus
    }
#endif
#endif
