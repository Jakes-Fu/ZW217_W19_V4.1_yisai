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

#ifndef _MMIWIDGET_EXPORT_H_
#define _MMIWIDGET_EXPORT_H_

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
/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef MMIWIDGET_SUPPORT

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIWIDGETPB_OpenFavoriteListWin         MMIAPIWIDGETPB_OpenFavoriteListWin
#define MMIWIDGET_Shortcutset_ResetWifiState    MMIAPIWIDGET_Shortcutset_ResetWifiState
#define MMIWIDGET_SetWallpaperLcdDevInfo        MMIAPIWIDGET_SetWallpaperLcdDevInfo
#define MMIWIDGET_CreateIdleWidget              MMIAPIWIDGET_CreateIdleWidget
#define MMIWIDGET_DestroyIdleWidget             MMIAPIWIDGET_DestroyIdleWidget
#define MMIWIDGET_HandleMsg                     MMIAPIWIDGET_HandleMsg
#define MMIWIDGET_AddDynamicItem                MMIAPIWIDGET_AddDynamicItem
#define MMIWIDGET_RemoveDynamicItem             MMIAPIWIDGET_RemoveDynamicItem
#define MMIWIDGET_GetWidgetPtr                  MMIAPIWIDGET_GetWidgetPtr
#define MMIWIDGET_IsTpMovingPage                MMIAPIWIDGET_IsTpMovingPage
#define MMIWIDGET_SetIdleWidgetStyle            MMIAPIWIDGET_SetIdleWidgetStyle

#ifdef MMI_GRID_IDLE_SUPPORT
#define MMIGRID_PB_IsContactFull        MMIAPIGRID_PB_IsContactFull
#define MMIWIDGET_HandleTimer           MMIAPIWIDGET_HandleTimer
#define MMIGRIDPB_GetWidgetId           MMIAPIGRIDPB_GetWidgetId
#define MMIGRIDPB_GetContactisNull      MMIAPIGRIDPB_GetContactisNull
#define MMIWIDGET_SetScheduleWidgetFlag MMIAPIWIDGET_SetScheduleWidgetFlag
#define MMIWIDGET_GetScheduleWidgetFlag MMIAPIWIDGET_GetScheduleWidgetFlag
#define MMIWIDGET_IsScheduleOnTop       MMIAPIWIDGET_IsScheduleOnTop
#define MMIWIDGET_SCH_Reset             MMIAPIWIDGET_SCH_Reset
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#define MMIGRID_GetMainAppFgInfo        MMIAPIGRID_GetMainAppFgInfo
#endif
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGETPB_OpenFavoriteListWin( MMIPB_PHONEBOOK_ENTRY_T  *select_entry_ptr );

/*****************************************************************************/
//     Description : Handle Widget Shortcut reset WIFI state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPIWIDGET_Shortcutset_ResetWifiState(void);

#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef MMIWIDGET_SUPPORT
/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SetWallpaperLcdDevInfo(
                                      GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                      );

/*****************************************************************************/
//  FUNCTION:      init widget module.
//  Description :
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_InitModule(void);

/*****************************************************************************/
//  Description : create idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_CreateIdleWidget(void);

/*****************************************************************************/
//  Description : destroy idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_DestroyIdleWidget(void);

/*****************************************************************************/
//  Description : handle widget msg
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIWIDGET_HandleMsg(
                                MMI_WIN_ID_T      win_id,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                );

/*****************************************************************************/
//  Description : add dynamic widget item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_E ctrl_id,
                                 uint32 widget_id,
                                 BOOLEAN is_on_shortcut,
                                 BOOLEAN is_to_update
                                  );

/*****************************************************************************/
//  Description : remove dynamic widget item
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_E ctrl_id,
                                 uint32 widget_id
                                  );

/*****************************************************************************/
//  Description :  return widget control ptr
//  Global resource dependence : 
//  Author:  bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIWIDGET_CTRL_T * MMIAPIWIDGET_GetWidgetPtr(
                                     MMIWIDGET_CTRL_E         ctrl_id
                                     );

/*****************************************************************************/
//  Description : get current tp status is moveing or not.
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_IsTpMovingPage(void);

/*****************************************************************************/
//  Description : set idle widget style.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SetIdleWidgetStyle(BOOLEAN set_value);

#endif //MMIWIDGET_SUPPORT.

#ifdef MMI_GRID_IDLE_SUPPORT

/*****************************************************************************/
//  Description : create idle widget
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_CreateIdle(void);

/*****************************************************************************/
//  Description : destroy idle widget
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_DestroyIdle(void);

/*****************************************************************************/
//  Description : handle widget msg
//  Global resource dependence : 
//  Author:  hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIGRID_HandleMsg(
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
PUBLIC MMI_IDLE_GRID_DATA_T * MMIAPIGRID_GetGridPtr(void);

/*****************************************************************************/
//  Description : add new widget to idle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_AddMenu2Idle(MMIMAINMENU_ITEM_INFO_T shortcut_info, GUI_POINT_T start_pos);
PUBLIC void MMIAPIGRID_AddNewWidget(uint32 widget_id);

/////////////////////////////////////////////////////////////////////////////////////////////////////

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
//  Description : grid pb max num is 10, judget full or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGRID_PB_IsContactFull(void);

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_HandleTimer(uint8 timer_id);

/*****************************************************************************/
//  FUNCTION:      init widget module.
//  Description :
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_InitModule(void);

/*****************************************************************************/
//  FUNCTION:      Get Widget Id
//  Description :
//  Global resource dependence : 
//  Author:        nan.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIGRIDPB_GetWidgetId(void);

/*****************************************************************************/
//  FUNCTION:      If Contact is Null
//  Description :
//  Global resource dependence : 
//  Author:        nan.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGRIDPB_GetContactisNull(uint32 index);

/*****************************************************************************/
//  Description : Reset Factory Setting
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : MMIAPIWIDGET_SetScheduleWidgetFlag
//  Global resource dependence : 
//  Author:
//  Note: juan.wu
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SetScheduleWidgetFlag(BOOLEAN is_already_add);

/*****************************************************************************/
//  Description : MMIAPIWIDGET_GetScheduleWidgetFlag
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_GetScheduleWidgetFlag(void);

/*****************************************************************************/
//  Description : MMIAPIWIDGET_IsScheduleOnTop
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 判断当前页面是否有日程widget
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_IsScheduleOnTop(void);

/*****************************************************************************/
//  Description : MMIAPIWIDGET_SCH_Reset
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 恢复出厂设置时调用
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SCH_Reset(void);

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIGRID_GetSlideTimerID
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:  return timer id
/*****************************************************************************/
PUBLIC uint8 MMIAPIGRID_GetSlideTimerID(void);

/*****************************************************************************/
//  Description : MMIAPIGRID_GetMainAppFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIGRID_GetMainAppFgInfo(LW_FG_INFO *lw_fg_info_ptr);
#endif
#endif

#ifdef MMI_WIDGET_TIME
/*****************************************************************************/
//    Description :处理时间更新消息
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWIDGET_Time_HandleTimeUpdate(void);
#endif



#ifdef SNS_SUPPORT
PUBLIC MMI_RESULT_E MMIWidget_SNSConvertTime(const wchar *ps_time, wchar *pd_time);
#endif

#if defined MMIWIDGET_WEATHER_SE 
PUBLIC void MMIWIDGET_SetCityIndexFlag(uint8 cur_city);
#endif

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut set WIFI state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPIWIDGET_Shortcutset_SetWifiState(void);
#endif

#endif // _MMIWIDGET_EXPORT_H_=======

