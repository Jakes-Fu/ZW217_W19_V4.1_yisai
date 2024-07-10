/*****************************************************************************
** File Name:      mmiwidget_export.h                                            *
** Author:                                                                   *
** Date:                                                           *
** Copyright:              *
** Description:                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMIWIDGET_INTERNAL_H_
#define _MMIWIDGET_INTERNAL_H_

#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "mmk_app.h"
#include "guifont.h"
#include "guicommon.h"
#include "guictrl.h"
#include "guianim.h"
#include "ui_layer.h"
#include "mmitheme_widget.h"
#include "mmiwidget.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#ifdef PDA_UI_SUPPORT_U_IDLE
#include "mmigrid_uidle_se.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef MMIWIDGET_SUPPORT
/*****************************************************************************/
//  Description : 删除收藏夹记录
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_DeleteFavoriteListItemByIndex(uint16 index);

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_DeleteFavoriteListItem(uint16 entry_id , uint8 group_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIDGETPB_IsExistFavoriteList(uint16 entry_id , uint8 group_id);

/*****************************************************************************/
//  Description : 更新记录的图片信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_ReloadPhotoFileName(uint16 entry_id , uint8 group_id,MMIPB_CUSTOM_PHOTO_T *photo_ptr);

/*****************************************************************************/
//  Description : 更新记录的number index 信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIWIDGETPB_ReloadNumberIndex( uint16 entry_id , uint8 group_id);

/*****************************************************************************/
//  Description :移动记录到别的分组
//  Global resource dependence : none
//  Author: 
//  Note: pb 中MMIPB_MoveNVGroup调用
/*****************************************************************************/
PUBLIC void  MMIWIDGETPB_MoveNVGroup( uint16 entry_id , uint8 from_group_id, uint8 to_group_id);

/*****************************************************************************/
//  Description : 收藏夹列表是否已打开
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGETPB_IsExistFavoriteListWin(void);

/*****************************************************************************/
//  Description : create quick contact window.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_OpenQuickContactListWin(void);
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef MMIWIDGET_SUPPORT
/*****************************************************************************/
//  Description : create the widget control.
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_Create(
                             MMI_WIN_ID_T win_id,
                             MMIWIDGET_CTRL_E  ctrl_id,
                             GUI_BOTH_RECT_T *both_rect_ptr
                             );

/*****************************************************************************/
//  Description : destroy the widget control.
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_Destroy(
                             MMIWIDGET_CTRL_E  ctrl_id
                             );

/*****************************************************************************/
//  Description :  set max item num, must be called before append item
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetMaxItemNum(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    uint32        max_item_num
                                    );

/*****************************************************************************/
//  Description : Add the item to widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_AppendItem(
                                 MMIWIDGET_CTRL_E         ctrl_id,
                                 const MMIWIDGET_ITEM_T   *item_ptr
                                 );

/*****************************************************************************/
//  Description : Add the item to widget list
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_AppendItemByPtr(
                                      MMIWIDGET_CTRL_T*       widget_ptr,
                                      const MMIWIDGET_ITEM_T* add_item_ptr
                                      );

/*****************************************************************************/
//  Description : Set the max item of list
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetMaxItemByPtr(
                                       MMIWIDGET_CTRL_T *widget_ptr,
                                       uint32        max_item_num
                                       );

/*****************************************************************************/
//  Description :  set shortcut offset
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetShortcutOffset(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    int16 shortcut_offset
                                    );

/*****************************************************************************/
//  Description : read mmi idle widget nvram.
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_ReadWidgetNV(
                        MMIWIDGET_SAVE_INFO_T *save_info_ptr
                        );


/*****************************************************************************/
//  Description : save mmi idle widget nvram.
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SaveWidgetInfoToNV(
                                 MMIWIDGET_CTRL_T *widget_ctrl_ptr
                                 );


/*****************************************************************************/
//  Description : find widget item info
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC MMIWIDGET_ITEM_INFO_T * MMIWIDGET_FindWidgetItemInfo(
                                          uint32 widget_id
                                          );

/*****************************************************************************/
//  Description :  set shortcut offset
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_SetCurPageIndex(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    int16 page_index
                                    );

/*****************************************************************************/
//  Description : create widget应用层
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_CreateApplicationLayer(void);
/*****************************************************************************/
//  Description : Destroy widget应用层
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_DestroyApplicationLayer(void);
/*****************************************************************************/
//  Description : 获取应用层(外用)
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_GetApplicationLayer(GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : MMIWIDGET_GetWinHandleByWidgetId
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIWIDGET_GetWinHandleByWidgetId(uint32 widget_id);

/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_UpdateAllScreen(void);

/*****************************************************************************/
//  Description : get idle widget style.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGET_IsIdleSetWidgetStyle(void);

/*****************************************************************************/
//  Description : MMIWIDGET_UpdateWidgetByWidgetId
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_UpdateWidgetByWidgetId(uint16 widget_id);

#endif //MMIWIDGET_SUPPORT.

#ifdef MMI_GRID_IDLE_SUPPORT
/*****************************************************************************/
//  Description : create the widget control.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_Create(
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
PUBLIC void MMIGRID_Destroy(
                             MMIWIDGET_CTRL_E  ctrl_id
                             );

/*****************************************************************************/
//  Description :  set max item num, must be called before append item
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_SetMaxItemNum(
                                    MMIWIDGET_CTRL_E ctrl_id, // control id
                                    uint32        max_item_num
                                    );


/*****************************************************************************/
//  Description : read mmi idle widget nvram.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_ReadWidgetNV(
                        MMI_GRID_SAVE_INFO_T *save_info_ptr
                        );


/*****************************************************************************/
//  Description : save mmi idle widget nvram.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_SaveWidgetInfoToNV(
                                 MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                 );

/*****************************************************************************/
//  Description :  set shortcut offset
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_SetCurPageIndex(int16 page_index);

/*****************************************************************************/
//  Description : get current tp status is moveing or not.
//  Global resource dependence : 
//  Author: hongbo.lan 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_IsForbidPaint(uint32 widget_id);

/*****************************************************************************/
//  Description : update all screen
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_UpdateAllScreen(void);


/*****************************************************************************/
//  Description : add new shortcut item to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_AddNewShortcut(MMIMAINMENU_ITEM_INFO_T *menu_info_ptr);

/////////////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description : 删除收藏夹记录
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_PB_DeleteFavoriteListItemByIndex(uint16 index);

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_PB_DeleteFavoriteListItem(uint16 entry_id , uint8 group_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/

PUBLIC void MMIGRID_PB_OpenFavoriteListWin(MMIPB_CONTACT_T  *select_entry_ptr );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIGRID_PB_IsExistFavoriteList(uint16 entry_id , uint8 group_id);

/*****************************************************************************/
//  Description : 更新记录的图片信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_PB_ReloadPhotoFileName(uint16 entry_id , uint8 group_id,MMIPB_CUSTOM_PHOTO_T *photo_ptr);

/*****************************************************************************/
//  Description : 更新记录的number index 信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIGRID_PB_ReloadNumberIndex( uint16 entry_id , uint8 group_id);

/*****************************************************************************/
//  Description :移动记录到别的分组
//  Global resource dependence : none
//  Author: 
//  Note: pb 中MMIPB_MoveNVGroup调用
/*****************************************************************************/
PUBLIC void  MMIGRID_PB_MoveNVGroup( uint16 entry_id , uint8 from_group_id, uint8 to_group_id);
#if 0
/*****************************************************************************/
//  Description : remove widget from idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_RemoveWidget(uint32 widget_id);
#endif

/*****************************************************************************/
//  Description : creat grid idle menu win.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_CreateGridMenuWin(void);

/*****************************************************************************/
//  FUNCTION:      Delete Favorite List Item
//  Description :
//  Global resource dependence : 
//  Author:        nan.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_DeleteFavoriteListItem(uint16 entry_id , uint8 group_id);

/*****************************************************************************/
//  FUNCTION:      Get Contact Num
//  Description :
//  Global resource dependence : 
//  Author:        nan.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIGRIDPB_GetContactNum(void);

/*****************************************************************************/
//  Description : 设置可更新contact的内容
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRIDPB_SetIsContactAnimEnab(uint16 entry_id , uint16 storage_id);

/*****************************************************************************/
//  Description : 是不是有可以添加的记事本
//  Global resource dependence : none
//  Author:
//  Note: TRUE表示该节点已经被添加过了，FALSE表示可以被添加
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_IsMemoStatusFull(uint32 index);
/*****************************************************************************/
// Description :MMIGRID_MEMO_IsNotesFull
// Global resource dependence :
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_MEMO_IsNotesFull(void);

/*****************************************************************************/
// Description :RESET MEMO WIDGET
// Global resource dependence :
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_MEMO_ResetFactorySettings(void);

/*****************************************************************************/
// Description :RESET PB WIDGET
// Global resource dependence :
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_PB_ResetFactorySettings(void);

/*****************************************************************************/
//  Description : MMIGRID_getpagelayersize
//  Global resource dependence : 
//  Author:ying.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIGRID_GetPageLayerSize(MMI_IDLE_GRID_DATA_T * idle_grid_ptr);


/*****************************************************************************/
//  Description : MMIGRID_get wallpaper layer size
//  Global resource dependence : 
//  Author:ying.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWIDGET_GetWallpaperLayerSize(MMI_IDLE_GRID_DATA_T * idle_grid_ptr);

/*****************************************************************************/
//  Description : set widget layer blt
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_SetBltLayer(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/*****************************************************************************/
//  Description : get widget page display rect.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void FillLayerWithMaskColor(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 mask_color);

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : cache all widgets and menu icons on prev,cur,next pages.
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_CacheItemsOnSpecificPage(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIGRID_U_PAGE_INFO_T *u_page_ptr, int32 page_idx);
#endif

#ifdef BAR_PHONE_WIDGET 
PUBLIC MMISET_IDLE_STYLE_E MMIAPISET_GetPreIdleStyle(void);
#endif

#ifdef BAR_PHONE_WIDGET 
PUBLIC BOOLEAN MMIWIDGET_SoftkeySetTextId(
                                   MMI_WIN_ID_T    win_id,
                                   MMI_TEXT_ID_T    text_id,
                                   BOOLEAN is_update
                                   );
#endif

//get log file name from path string
PUBLIC char* MMIWIDGET_LogGetFile( const char* file );

#ifdef WIN32
#define LOG_STR(str) "%s:%d: " str, MMIWIDGET_LogGetFile(__FILE__), __LINE__
#define LOG_STR_ENTER(str) "%s:%d: Enter ... " str, MMIWIDGET_LogGetFile(__FILE__), __LINE__
#define LOG_STR_LEAVE(str) "%s:%d: Leave ... " str, MMIWIDGET_LogGetFile(__FILE__), __LINE__
#else
#define LOG_STR(str) "%s:%d, %s(): " str, MMIWIDGET_LogGetFile(__FILE__), __LINE__, __func__
#define LOG_STR_ENTER(str) "%s:%d, %s(): Enter ... " str, MMIWIDGET_LogGetFile(__FILE__), __LINE__, __func__
#define LOG_STR_LEAVE(str) "%s:%d, %s(): Leave ... " str, MMIWIDGET_LogGetFile(__FILE__), __LINE__, __func__
#endif

#define MMIWIDGET_DEBUG      SCI_TRACE_LOW

#endif

#endif // _MMIWIDGET_INTERNAL_H_


