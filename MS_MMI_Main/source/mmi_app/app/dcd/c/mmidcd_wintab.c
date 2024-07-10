/*****************************************************************************
** File Name:      mmidcd_wintab.c                                           *
** Author:                                                                   *
** Date:           09/2007                                                   *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2007       violent.wei       Create
** 08/2009       jun.hu            modify
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_dcd_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "gui_ucs2b_converter.h"
#include "mmi_appmsg.h"

#ifdef DCD_SUPPORT   //!宏定义开头
#include "mmidcd_http.h"
#include "mmidcd_main.h"
#include "mmidcd_file.h"
#include "mmidcd_id.h"
#include "mmidcd_text.h"
#include "mmidcd_menutable.h"
#include "mmidcd_wintab.h"
#include "mmidcd_nv.h"
#include "mmiset_text.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmidcd_image.h"

#include "mmidcd_position.h"

#include "mmisms_image.h"
#include "mmk_app.h"
#include "guitab.h"
#include "guirichtext.h"
#include "guitext.h"
#include "mmi_appmsg.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmiidle_export.h"
#include "mmifmm_export.h"

#include "mmisms_export.h"
#include "mmisms_app.h"
#include "guires.h" 
#include "guitab.h" 
#include "guimenu.h"
#include "guilistbox.h"
#include "mmk_timer.h"
#include "mmiidle_export.h"
#include "mmidcd_export.h" 
#include "mmimms_export.h" 
#include "mmiudisk_export.h" 
#include "mmiconnection_export.h" 
#include "mn_type.h"  
#include "guicommon.h"   
#include "mmi_mainmenu_export.h"
#if defined(MMIWIDGET_SUPPORT)
#include "mmiwidget_id.h"
#include "mmiwidget.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                  *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
// dcd tab控件的区域定义，供本文件使用，也做了Get函数，供idle获取该区域
LOCAL const GUI_RECT_T s_dcd_tab_rect = {0, MMI_DCD_IDLEWIN_TAB_TOP, MMI_MAINSCREEN_WIDTH, MMI_DCD_IDLEWIN_TAB_BOTTOM};
// jun.hu把g_dcd_gui_struct_ptr 全局变量从mmidcd_main 移动到这里,作为静态变量使用....
LOCAL DCD_GUI_STRUCT_T s_dcd_gui_struct = {0};
//
LOCAL uint16  s_local_list_item_index = 0;
LOCAL uint16  s_local_list_total_num = 0;

// FOR 更新中不变DCD祥情界面中的内容.......
LOCAL BOOLEAN  s_data_has_changed = FALSE;
//更新中是否可以取消更新
LOCAL BOOLEAN  s_ifcan_cancel_update = FALSE;

//是否是由TEXT控件的timer引起的MSG_NOTIFY_TAB_SWITCH
LOCAL BOOLEAN is_text_end=FALSE; //@CR237614 2011.05.31    

#define MMIDCD_TAB_TIP_STR_START_Y  15
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle waiting  win msg  for  saving  as while coping file
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleCopyLocalFileWaitWinMsg(
                                                  MMI_WIN_ID_T        win_id,   //in
                                                  MMI_MESSAGE_ID_E    msg_id,  //in  
                                                  DPARAM              param    //in
                                                  );

/*****************************************************************************/
//  Description :  Father windows
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFatherWinMsg(
                                      MMI_WIN_ID_T        win_id, 
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM              param
                                      );
/*****************************************************************************/
//  Description : to handle dialed tab child window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildWindow(
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                      );
/*****************************************************************************/
//  Description : to handle dialed tab child window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIdleChildWindow(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM       param
                                          );
/*****************************************************************************/
//  Description : DCD主菜单的消息响应函数
//  Global resource dependence : 
//  Author:    modified by jun.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleItemMenuWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalItemMenuWinMsg(
                                             MMI_WIN_ID_T        win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             );
/*****************************************************************************/
//  Description : 订购详情界面消息处理函数
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/8/4
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdOrderDetailWinMsg(
                                              MMI_WIN_ID_T      win_id,     
                                              MMI_MESSAGE_ID_E  msg_id, 
                                              DPARAM            param
                                              );
/*****************************************************************************/
//  Description : richtext show
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdPreviewWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
/*****************************************************************************/
//  Description : DCD察看界面点菜单的消息响应函数
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdPreviewMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );
/*****************************************************************************/
//  Description : 进行DCD设置菜单的消息处理函数
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDSettingWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : 处理dcd 恢复出厂设置和清楚用户数据的等待窗口
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDStopUpdateDataWaitWin(
                                                 MMI_WIN_ID_T    win_id, // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                 DPARAM             param   // 相应消息的参数
                                                 );
/*****************************************************************************/
//  Description : to set dcd list item in 1 page
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note:  modified by jun.hu 2009.12.17
/*****************************************************************************/
LOCAL void  UpdateDCDList( MMI_CTRL_ID_T list_ctrl_id, BOOLEAN need_send_paint);
/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void DCD_UpdateMainWin(MMI_WIN_ID_T    win_id);
/*****************************************************************************/
//  Description : dcd local 主界面的消息处理函数
//  Global resource dependence : 
//  Author:
//  Note: jun.hu 2009/10/17 把local的两个father, child ,tab的做法去掉了...
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalMainWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );
/*****************************************************************************/
//  Description : 订购帮助界面消息处理
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/8/12
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdHelpWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void DeleteDcdLocalFileItem( const uint16 item_index );
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: zhuo.wang for mmi handle dcd error type
//  Note:
/*****************************************************************************/
LOCAL void DCD_Error_Handler(MMI_WIN_ID_T        *win_id_ptr, 
                             MMIDCD_ERROR_CODE_E error_type, 
                             MMI_WIN_PRIORITY_E  win_priority
                             );
/*****************************************************************************/
//  Description : 加载dcd订购详情
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
LOCAL void addDcdOrderTextItem(const MMI_CTRL_ID_T richtext_ctrl_id,
                               const MMI_TEXT_ID_T text_id, 
                               uint16 index
                               );
/*****************************************************************************/
//  Description : 加载dcd订购详情中的上次同步状态.....
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
LOCAL void addDcdLastUpdateStateDetail(const MMI_CTRL_ID_T richtext_ctrl_id,
                                       const uint16 index
                                       );
/*****************************************************************************/
//  Description : 加载dcd订购详情中的时间信息.....
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
LOCAL void addDcdUpdateTimeDetail(const MMI_CTRL_ID_T richtext_ctrl_id,
                                  const uint16  itemidx
                                  );
/*****************************************************************************/
//  Description :动态创建dcd的mark菜单.....
//  Global resource dependence : 
//  Author:jun.hu
//  Note:  2009/9/15
//  para 1: MMI_CTRL_ID_T menuctrl_id,         //要创建的meun ctrl id
//  para 2: MMI_CTRL_ID_T list_ctrl_id,        //从哪个listbox 进来的
//  para 3: DCD_ITEMMENU_NODE_E parent_menu_id // 父节点(0表示无）
/*****************************************************************************/
LOCAL void  CreateDcdMarkMenu(MMI_CTRL_ID_T menuctrl_id,  
                              MMI_CTRL_ID_T list_ctrl_id, 
                              DCD_ITEMMENU_NODE_E parent_menu_id
                              );
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/9/15
/*****************************************************************************/
LOCAL void CreateLocalItemDyMenu(const MMI_CTRL_ID_T menuctrl_id);
/*****************************************************************************/
//  Description : 进行DCD设置主菜单的消息处理函数
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/16
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDSettingMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );
/*****************************************************************************/
//  Description : DCD_QueryWinMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DCD_QueryWinMsg(
                                    MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param
                                    );
/*****************************************************************************/
//  Description : richtext show
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void DCD_UpdatePreviewWin(MMI_WIN_ID_T  win_id,
                                MMI_CTRL_ID_T ctrl_id
                               );
/*****************************************************************************/
//  Description : prew界面点击左键后动态后产生的动态菜单.....
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/9/17
/*****************************************************************************/
LOCAL void CreatePreviewDynaMenu(MMI_CTRL_ID_T menuctrl_id,
                                 MMI_CTRL_ID_T list_ctrl_id
                                 );
/*****************************************************************************/
//  Description : set text parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void DCD_SetTextParam(
                            MMI_CTRL_ID_T    ctrl_id,
                            uint16           index,
                            BOOLEAN          is_update  //是否立即刷新             
                            );
/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DCD_SetAnimParam(
                               MMI_CTRL_ID_T    ctrl_id,
                               uint16         index,
                               BOOLEAN          is_update  //是否立即刷新                           
                               );
/*****************************************************************************/
//  Description : handle dcd list item delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDcdListItemDeleteOperation (MMI_CTRL_ID_T  list_ctrl_id);
/*****************************************************************************/
//  Description : to handle dialed tab child window message
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void  DCD_UpdateIdleItem(MMI_WIN_ID_T win_id,
                               uint16  item_index,
                               BOOLEAN is_swtich_tab,
                               BOOLEAN is_need_update
                               );
/*****************************************************************************/
//  Description : 将dcd item  详情内容加到Richtext中
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/10/27 把旧的DCD_SetItemDetails一拆为二
/*****************************************************************************/
LOCAL void DCD_SetItemDetails(uint16 real_item_index) ;
/*****************************************************************************/
//  Description : 将本地保存的dcd item  详情内容加到Richtext中
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/10/27 把旧的DCD_SetItemDetails一拆为二
/*****************************************************************************/
LOCAL void DCD_SetLocalItemDetails(uint16 item_index) ;
/*****************************************************************************/
//  Description : 设置dcd详情的标题图片
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/10/27 把旧的DCD_SetItemDetails一拆为二
/*****************************************************************************/
LOCAL void DCD_SetItemTitlePicAndText(
                                      wchar * picture_full_path_ptr, 
                                      wchar * title_content_ptr//标题文字的指针
                                      ) ;
/*****************************************************************************/
//  Description : 设置DCD查看详情窗口的内容...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL void SetDCDPrewWinContent(
                                MMI_WIN_ID_T    win_id,
                                MMI_CTRL_ID_T   ctrl_id
                                );
/*****************************************************************************/
//  Description : 处理查看本地已存详情界面的item的切换问题...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL void HandDCDPrewLocalItemSwitch(
                                      MMI_WIN_ID_T    win_id,
                                      uint16          cur_item_index,
                                      BOOLEAN         prew_next     // 为TRUE ,表示查看下一条，FALSE代表查看前一条...
                                      );
/*****************************************************************************/
//  Description : 处理查看详情界面的item的切换问题...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL void HandDCDPrewItemSwitch(
                                 MMI_WIN_ID_T    win_id,
                                 uint16          cur_item_index,
                                 BOOLEAN         prew_next     // 为TRUE ,表示查看下一条，FALSE代表查看前一条...
                                 );
/*****************************************************************************/
//  Description : SetDcdMainWinTitleAddStr 更新dcd main win的title 的附加字符串
//  Global resource dependence: 
//  Author: jun.hu
//  Note:  2009/8/18
/*****************************************************************************/
LOCAL void SetDcdMainWinTitleAddStr(MMI_CTRL_ID_T  ctr_id);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void DCD_EnableGrayedItemMenu(MMI_WIN_ID_T     win_id, 
                                    MMI_CTRL_ID_T    ctrl_id
                                    );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/11/3
/*****************************************************************************/
LOCAL void HandleDcdPrewItemDeleteOperation(MMI_CTRL_ID_T ctrl_id);
/*****************************************************************************/
//  Description : 得到下一个可用PAGE的index...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL BOOLEAN GetNextUseablePageIndex(
                                      uint16 *   page_index_ptr,  // out
                                      uint16    cur_page_index
                                      );
/*****************************************************************************/
//  Description : 得到上一个可用PAGE的index...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL BOOLEAN GetPrewUseablePageIndex(
                                      uint16 *   page_index_ptr,  // out
                                      uint16    cur_page_index
                                      );
/*****************************************************************************/
//  Description : ....
//  Global resource dependence : 
//  Author:     jun.hu
//  Note: jun.hu 2009/11/12
/*****************************************************************************/
LOCAL void HandleRequestUpateByMan(void);
/*****************************************************************************/
//  Description : handle dcd local list item delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/11/13
/*****************************************************************************/
LOCAL void HandleDcdLocalListDeleteOperation(MMI_CTRL_ID_T   list_ctrl_id);
/*****************************************************************************/
//  Description : get item link adress
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void GetDcdItemLinkAdr( 
                             MMI_CTRL_ID_T  list_ctrl_id, 
                             char * link_adr_ptr,
                             uint16  max_len
                             );
/*****************************************************************************/
//  Description : appen one item text and anim
//  Global resource dependence : 
//  Author: 
//  Note: 
//       /* list position*/            
//       1 animate     2 text       3 icon   4 text 
//       different with 3 icon style
/*****************************************************************************/
LOCAL BOOLEAN AppendOneListItemWithText(
                                       uint16           item_index,     //in:item索引
                                       MMI_CTRL_ID_T    ctrl_id         //in:control id
                                       );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/12/30 写此函数为即将显示的TIEM附上显示图片的数据
//        以去掉之前旧的函数need_data和need_content都需要读取图片数据的做法...
/*****************************************************************************/
LOCAL BOOLEAN DrawListItemWithPicture(
                                      uint16           item_index,     //in:item索引
                                      uint16           content_index,  //in:item内容索引,text or anim etc.
                                      MMI_CTRL_ID_T    ctrl_id         //in:control id
                                      );
/***********************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2010/1/14 根据旧的函数改造而来,并把need_data和need_content分开处理.....
/************************************************************************************/
LOCAL BOOLEAN AppendOneLocalListItemWithText(
                                             uint16           item_index,     //in:item索引
                                             MMI_CTRL_ID_T    ctrl_id         //in:control id
                                             );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/12/30 写此函数为即将显示的TIEM附上显示图片的数据
//        以去掉之前旧的函数need_data和need_content都需要读取图片数据的做法...
/*****************************************************************************/
LOCAL BOOLEAN DrawLocalListItemWithPicture(
                                           uint16           item_index,     //in:item索引
                                           uint16           content_index,  //in:item内容索引,text or anim etc.
                                           MMI_CTRL_ID_T    ctrl_id         //in:control id
                                           );
/*****************************************************************************/
//  Description : handle father tab pic dec ind 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void DCD_HandleTabDecInd(uint16 index, GUIANIM_RESULT_E result);

/*****************************************************************************/
//  Description : handle child list pic dec ind 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void DCD_HandleListDecInd(uint16 index, GUIANIM_RESULT_E result);
/*****************************************************************************/
//  Description : 检查U盘的状态
//  Global resource dependence : 
//  Author:jun.hu
//  Note:  2010/2/1 
/*****************************************************************************/
LOCAL BOOLEAN IsUDiskRunning(void);
/*****************************************************************************/
//  Description : dcd 恢复出厂设置等待窗口
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 写此函数是不管DCD是否在更新中都起等待窗口，因为DCD有删除图片文件夹的动作，
//        以避免用户较长时间按键没反应的现象..
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDResetFactoryWaitWin(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param   
                                                );
/*****************************************************************************/
//  Description : 当发生空间不够时
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 写此函数为的是打开窗口时不点亮屏幕,故重写MSG_OPEN_WINDOW函数....
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNotEnoughtSpaceWin(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param   
                                            );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void EntryInternet(char * urlink_ptr);
/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void UpdateDcdIdleWin(MMI_WIN_ID_T  win_id);
/*****************************************************************************/
//  Description :add list item 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(
                                     MMI_CTRL_ID_T       ctrl_id,
                                     const wchar*        wstr_ptr,
                                     uint16              wstr_len,
                                     uint16              pos
                                     );
/*****************************************************************************/
//  Description :mms setting link list choose win handle fun
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLinkListWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );
/*****************************************************************************/
//  Description : the process message function of the dcd reset function
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 此函数供恢复DCD的出厂设置调用....
/*****************************************************************************/
LOCAL MMI_RESULT_E  ResetDCDApplet_HandleEvent(    
                                               IAPPLET_T*          iapplet_ptr,
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note:
/*****************************************************************************/
LOCAL void SetDataChangeFlag(BOOLEAN flag);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDataChangeFlag(void);
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmidcdreset_applet_array[] = 
{ 
    { GUID_NAME_DEF(SPRD_DCD_RESET_APPLET_ID), sizeof(CAF_APPLET_T), ResetDCDApplet_HandleEvent, 0, 0 }
};

LOCAL const CAF_STATIC_APPLET_INFO_T s_dcd_applet_info = 
{
    //AnsMathineModuleEntry,
    s_mmidcdreset_applet_array, ARR_SIZE(s_mmidcdreset_applet_array) /*lint !e605*/
};


 /*dcd main win . father windows*/
WINDOW_TABLE( MMIDCD_MAIN_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFatherWinMsg ),    
    WIN_ID( MMIDCD_MAIN_WIN_ID ),
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT,GUITAB_STATE_NULL, MMIDCD_TAB_CTRL_ID),
    CREATE_ANIM_CTRL(MMIDCD_ANIMATE_UPDATING_CTRL_ID, MMIDCD_MAIN_WIN_ID),
    END_WIN
};

// the child window for dcd main win
WINDOW_TABLE( MMIDCD_ITEM_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleChildWindow),    
    WIN_ID( MMIDCD_ITEM_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIDCD_ITEM_LIST_CTRL_ID),  
    END_WIN
};

//dcd idle win child win
WINDOW_TABLE( MMIDCD_IDLE_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleIdleChildWindow),    
    WIN_ID( MMIDCD_IDLE_CHILD_WIN_ID ),
    CREATE_ANIM_CTRL(MMIDCD_IDLEWIN_ANIM_CTRL_ID,MMIDCD_IDLE_CHILD_WIN_ID),
    CREATE_TEXT_CTRL(MMIDCD_IDLEWIN_TEXT_CTRL_ID),
    WIN_STYLE(WS_DISABLE_COMMON_BG),
    END_WIN
};

WINDOW_TABLE( MMIDCD_SETTING_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDCDSettingWinMsg ),   
    WIN_ID( MMIDCD_SETTING_WIN_ID ),
    WIN_TITLE(STXT_MAIN_SETTING),  
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIDCD_SETTING_LIST_CTRL_ID),
    END_WIN
};
/*dcd main win option pop menu*/
WINDOW_TABLE( MMIDCD_ITEM_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleItemMenuWinMsg),
    WIN_ID(MMIDCD_ITEM_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(STXT_SELECT,TXT_NULL,STXT_RETURN),
    END_WIN
};
/*dcd local main win option pop menu*/
WINDOW_TABLE( MMIDCD_LOCAL_ITEM_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleLocalItemMenuWinMsg),
    WIN_ID(MMIDCD_LOCAL_ITEM_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(STXT_SELECT,TXT_NULL,STXT_RETURN),
    END_WIN
};


WINDOW_TABLE(MMIDCD_PREVIEW_WIN_TAB) = 
{ 
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_ID(MMIDCD_PREVIEW_WIN_ID),
    WIN_TITLE(TXT_NULL), 
    WIN_FUNC((uint32)HandleDcdPreviewWinMsg),
    CREATE_RICHTEXT_CTRL(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_COMN_MORE, STXT_RETURN),
    END_WIN
};


/*modified by @jun.hu, 2009/8/4, begin +*/
WINDOW_TABLE(MMIDCD_ORDER_DETAIL_WIN_TAB) = 
{ 
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_ID(MMIDCD_DETAIL_WIN_ID),
    WIN_TITLE(TXT_COMMON_SETTINGS_ENTRIES),
    WIN_FUNC((uint32)HandleDcdOrderDetailWinMsg),
   
    CREATE_RICHTEXT_CTRL(MMIDCD_ITEM_ORER_RICHTEXT_CTRL_ID),

    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),      
    END_WIN
};
WINDOW_TABLE(MMIDCD_HELP_WIN_TAB) = 
{ 
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_ID(MMIDCD_HELP_WIN_ID),
    WIN_TITLE(TXT_HELP),
    WIN_FUNC((uint32)HandleDcdHelpWinMsg),
    CREATE_RICHTEXT_CTRL(MMIDCD_ITEM_HELP_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMN_MORE, TXT_NULL, STXT_RETURN),
    END_WIN
};
// DCD 设置主菜单wintab
WINDOW_TABLE( MMIDCD_SETTING_MAINLIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleDCDSettingMenuWinMsg),   
    WIN_ID(MMIDCD_SETTING_MENU_WIN_ID),
    WIN_TITLE(STXT_MAIN_SETTING),  
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_DCD_ITEM_SETTING_OPT, MMIDCD_SETTING_MENU_CTRL_ID),
    END_WIN
};

/*modified by @jun.hu, 2009/8/4, end-*/

//jun.hu 2009/10/17 对DCD已存的主窗口进行改造，去掉了TAB控件以及子窗口等
WINDOW_TABLE(MMIDCD_LOCAL_MAIN_WIN_TAB) = 
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_ID(MMIDCD_LOCAL_MAIN_WIN_ID),
    WIN_FUNC((uint32)HandleLocalMainWinMsg),
    WIN_TITLE( TXT_DCD_LOCAL_TITLE),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIDCD_LOCAL_ITEM_LIST_CTRL_ID),            
    END_WIN
};


WINDOW_TABLE(MMIDCD_PREVIEW_MENU_WIN_TAB) = 
{
    WIN_ID(MMIDCD_PREVIEW_MENU_WIN_ID),
    WIN_FUNC((uint32)HandleDcdPreviewMenuWinMsg),
    WIN_STYLE( WS_HAS_TRANSPARENT), // for cr00155063....
    WIN_SOFTKEY(STXT_OPTION,TXT_NULL,STXT_RETURN),                
    END_WIN
};

WINDOW_TABLE(MMIDCD_LINK_LIST_WIN_TAB) = 
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_ID(MMIDCD_LINK_LIST_WIN_ID),
    WIN_FUNC((uint32)HandleLinkListWinMsg),
    WIN_TITLE( TXT_DCD_TEXT_LINK_LIST),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIDCD_LINKLIST_CTRL_ID),            
    END_WIN
};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register DCD module applet info
//  Global resource dependence : none
// Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_DCD, &s_dcd_applet_info);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void SendBySMS(void)
{
    MMI_STRING_T            string = {0};    
    uint16         wlen = 0;    
    uint16 page_index=s_dcd_gui_struct.current_page_index;
    uint16 real_item_index=0;
    uint16 title_len = 0;
    wchar  *full_content_ptr = PNULL;
    wchar  *entry_summary_ptr = {0};

    full_content_ptr = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN * sizeof(wchar));

    if (PNULL == full_content_ptr)
    {
        return;
    }

    entry_summary_ptr = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN * sizeof(wchar));

    if (PNULL == entry_summary_ptr)
    {
        SCI_TRACE_LOW(full_content_ptr);

        return;
    }

    //get the real item index
    real_item_index = s_dcd_gui_struct.current_real_index_in_page;
    
    title_len = MMIAPIDCD_GetEntryTitle(full_content_ptr, MMISMS_MAX_MESSAGE_LEN, page_index, real_item_index);

    if(MMISMS_MAX_MESSAGE_LEN > title_len)
    {
        wlen = (uint16)(MMISMS_MAX_MESSAGE_LEN - title_len);
        MMIAPIDCD_GetEntrySummary(entry_summary_ptr, wlen, page_index, real_item_index);
        MMIAPICOM_Wstrncat(full_content_ptr, entry_summary_ptr, wlen);
    }

    string.wstr_ptr = full_content_ptr;
    string.wstr_len = (uint16)MMIAPICOM_Wstrlen(full_content_ptr);
    
    MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &string,  PNULL, 0);

    SCI_FREE(full_content_ptr);
    SCI_FREE(entry_summary_ptr);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/*****************************************************************************/
LOCAL void SendByMMS(void)
{
    wchar        full_path_arr[MMIFMM_FULL_FILENAME_LEN] = {0};
    uint16       full_path_len = 0;
    uint16       page_index=s_dcd_gui_struct.current_page_index;  
    uint16       real_item_index=0;
    MMI_STRING_T subject_ptr={0};
    MMI_STRING_T content_ptr={0}; 
    MMIFILE_FILE_INFO_T file_info = {0};

    //jun.hu 2009/11/10 for cr158431.....
    if(IsUDiskRunning())
    {
        return;
    }
    
    real_item_index = s_dcd_gui_struct.current_real_index_in_page;
    subject_ptr.wstr_ptr=MMIDCD_ALLOC((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar));  
    content_ptr.wstr_ptr=MMIDCD_ALLOC((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar));  
    if((subject_ptr.wstr_ptr != PNULL) && (content_ptr.wstr_ptr != PNULL))
    {
        SCI_MEMSET(subject_ptr.wstr_ptr, 0, ((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar)));
        SCI_MEMSET(content_ptr.wstr_ptr, 0, ((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar)));
        
        subject_ptr.wstr_len =MMIAPIDCD_GetEntryTitle(subject_ptr.wstr_ptr, MMISMS_MAX_MESSAGE_LEN, page_index, real_item_index);
        content_ptr.wstr_len =MMIAPIDCD_GetEntrySummary(content_ptr.wstr_ptr, MMISMS_MAX_MESSAGE_LEN, page_index, real_item_index);
        
        MMIDCDFILE_GetOnePictureFullPath(full_path_arr, 
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"ctxt-id"),
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"type"),
                                        MMIDCD_PICTURE_DIR_PATH
                                        );
        full_path_len = MMIAPICOM_Wstrlen(full_path_arr); 
        
        //jun.hu 2009/10/13 加上对图片是否存在的判读 for cr00156087
        if(MMIAPIFMM_IsFileExist(full_path_arr, full_path_len))
        {
            MMIAPIFMM_GetFileInfoFormFullPath(full_path_arr,full_path_len,&file_info);
            MMIAPIMMS_EditMMSFromContent(&subject_ptr,&content_ptr,&file_info,MMIFMM_FILE_TYPE_PICTURE);
        }
        else
        {
            MMIAPIMMS_EditMMSFromContent(&subject_ptr,&content_ptr, PNULL, MMIFMM_FILE_TYPE_NORMAL);
        }
        
    }

    if(PNULL != subject_ptr.wstr_ptr)
    {
        MMIDCD_FreeMem(subject_ptr.wstr_ptr);
    }

    if(PNULL != content_ptr.wstr_ptr)
    {
        MMIDCD_FreeMem(content_ptr.wstr_ptr); 
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: modified by jun.hu
/*****************************************************************************/
LOCAL void SendLocalBySMS(void)
{
    MMI_STRING_T   string = {0};    
    uint16         wlen = 0;    
    uint8 * content_ptr=PNULL;  
    uint16 item_index = s_local_list_item_index;
    uint16 str_len = 0;
    MMIDCD_LOCAL_FILE_NODE_T cur_node = {0};
    wchar  *full_content_ptr = PNULL;
    
    //因为下面要读取文件，所以加上了U盘是否在使用中的判断....
    if(IsUDiskRunning()) 
    {
        return;
    }
    
    if(MMIDCDFILE_ReadLocalItemInfo(&cur_node, item_index))
    {
        full_content_ptr = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN *sizeof(wchar));

        if (PNULL == full_content_ptr)
        {
            return;
        }
    
        content_ptr=cur_node.title_str;
        wlen=cur_node.title_len;
        
        string.wstr_len=GUI_OtherCodeToWstr(full_content_ptr,
                                            MMISMS_MAX_MESSAGE_LEN,
                                            GUI_CHAR_CODE_UCS2_BE,
                                            content_ptr,
                                            wlen   
                                            );
        
        if(MMISMS_MAX_MESSAGE_LEN > string.wstr_len)
        {
            str_len = (uint16)(MMISMS_MAX_MESSAGE_LEN - string.wstr_len);
            wlen=GUI_OtherCodeToWstr(full_content_ptr+string.wstr_len,
                                    str_len,
                                    GUI_CHAR_CODE_UCS2_BE,
                                    cur_node.summary_str,
                                    cur_node.summary_len   
                                    );
        }
        else
        {
            // 截取一下.......
            string.wstr_len = MMISMS_MAX_MESSAGE_LEN;
            wlen = 0;
        }
        
        string.wstr_ptr=full_content_ptr;
        string.wstr_len = (uint16)(string.wstr_len + wlen);
        
        MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &string,  PNULL, 0);

        SCI_FREE(full_content_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD Read fail!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_893_112_2_18_2_12_56_247,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:violent.wei
//  Note: jun.hu 09/8/27 把它从mmidcd_main.c中移动到这里
/*****************************************************************************/
PUBLIC void MMIDCD_SetData(void)
{
    //SCI_TRACE_LOW:"[MMIDCD]:MMIAPIDCD_SetData() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_902_112_2_18_2_12_56_248,(uint8*)"");

    s_dcd_gui_struct.current_page_index = 0 ;
    s_dcd_gui_struct.current_index_in_page = 0;
    s_dcd_gui_struct.current_real_index_in_page =0; 

    s_dcd_gui_struct.cur_idle_page_index = 0;       // dcd idle窗口用到的数据
    s_dcd_gui_struct.cur_idle_realitem_index = 0;   // dcd idle窗口用到的数据
    
    if(PNULL != MMIAPIDCD_GetXmlDoc_Ptr())
    {
        s_dcd_gui_struct.total_page_num   = MMIAPIDCD_GetFormalFeedNum();
        s_dcd_gui_struct.current_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(0, TRUE);
    }
    else
    {
        s_dcd_gui_struct.total_page_num   = 0;
        s_dcd_gui_struct.current_item_num = 0;
    }

    //SCI_TRACE_LOW:"[MMIDCD]:MMIAPIDCD_SetData() exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_922_112_2_18_2_12_56_249,(uint8*)"");
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/*****************************************************************************/
LOCAL void SendLocalByMMS(void)
{
    uint16      full_path_len = 0;
    MMIFILE_FILE_INFO_T file_info = {0};
    MMI_STRING_T     subject_ptr={0};
    MMI_STRING_T     content_ptr={0};   
    MMIDCD_LOCAL_FILE_NODE_T cur_node = {0};
    wchar  picture_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 item_index = s_local_list_item_index;
    
    if(IsUDiskRunning())
    {
        return;
    }

    if(MMIDCDFILE_ReadLocalItemInfo(&cur_node, item_index))
    {
        subject_ptr.wstr_ptr = MMIDCD_ALLOC((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar));  
        content_ptr.wstr_ptr = MMIDCD_ALLOC((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar));
        if((PNULL != subject_ptr.wstr_ptr) && (PNULL != content_ptr.wstr_ptr))
        {
            SCI_MEMSET(subject_ptr.wstr_ptr, 0, ((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar)));
            SCI_MEMSET(content_ptr.wstr_ptr, 0, ((MMISMS_MAX_MESSAGE_LEN)*sizeof(wchar)));
            subject_ptr.wstr_len = GUI_OtherCodeToWstr(subject_ptr.wstr_ptr, MMISMS_MAX_MESSAGE_LEN, GUI_CHAR_CODE_UCS2_BE, cur_node.title_str, cur_node.title_len);
            content_ptr.wstr_len = GUI_OtherCodeToWstr(content_ptr.wstr_ptr, MMISMS_MAX_MESSAGE_LEN, GUI_CHAR_CODE_UCS2_BE, cur_node.summary_str, cur_node.summary_len);
            
            MMIDCDFILE_GetOnePictureFullPath(picture_full_path,
                                            (char *)cur_node.content_text_id_str,
                                            (char *)cur_node.content_type_str,
                                            MMIDCD_LOCAL_PICTURE_DIR_PATH
                                            );
            full_path_len = MMIAPICOM_Wstrlen(picture_full_path);
            //jun.hu 2009/10/13 加上对图片是否存在的判读 for cr00156087
            if(MMIAPIFMM_IsFileExist(picture_full_path, full_path_len))
            {
                MMIAPIFMM_GetFileInfoFormFullPath(picture_full_path,full_path_len,&file_info);
                MMIAPIMMS_EditMMSFromContent(&subject_ptr,&content_ptr,&file_info,MMIFMM_FILE_TYPE_PICTURE);
            }
            else
            {
                MMIAPIMMS_EditMMSFromContent(&subject_ptr,&content_ptr, PNULL, MMIFMM_FILE_TYPE_NORMAL);
            }
            
        }
        
        if(PNULL != subject_ptr.wstr_ptr)
        {
            MMIDCD_FreeMem(subject_ptr.wstr_ptr);
        }
        
        if(PNULL != content_ptr.wstr_ptr)
        {
            MMIDCD_FreeMem(content_ptr.wstr_ptr);
        }
    }
     
    else
    {
        //SCI_TRACE_LOW:"MMIDCD Get index fail!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_986_112_2_18_2_12_56_250,(uint8*)"");
    }
    
}
/*****************************************************************************/
//  Description : get dcd switch idex
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDCDSwitchIndex(void)
{
    uint16 index=0;
    if (!MMIAPIDCD_GeSettingInfo()->is_dcd_open)
    {
        index = 1;
    }
    
    return index;
}
/*****************************************************************************/
//  Description : get dcd connect mode 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDCDConnectModeIndex(void)
{
    uint16 curIndex = 0;
    switch(MMIAPIDCD_GeSettingInfo()->connect_mode)
    {
        case MMIDCD_CONNECT_MODE_ALWAYS:
                curIndex = 0;               
                break;

        case MMIDCD_CONNECT_MODE_NEVER:
                curIndex = 1;               
                break;

        case MMIDCD_CONNECT_MODE_OFF_WHEN_CRUISE:
                curIndex = 2;               
                break;
            
        default:
            curIndex = 2;
            break;
    }
    
    return curIndex;
}
/*****************************************************************************/
//  Description : get dcd connect mode 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDCDScrollSpeedIndex(void)
{
    uint16 curIndex = 0;
    switch(MMIAPIDCD_GeSettingInfo()->scroll_time)
    {
        case MMIDCD_SCROLL_TIME_SLOW:
           curIndex = 2;
           break;
        case MMIDCD_SCROLL_TIME_MIDDLE:
          curIndex = 1;
            break;
        case MMIDCD_SCROLL_TIME_FAST:
            curIndex = 0;
            break;
        default:
            curIndex = 1; 
            break;
    }
    
    return curIndex;
}

/*****************************************************************************/
//  Description : get dcd SIM  idex
//  Global resource dependence : none
//  Author: koki gao
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDCDSimSelectedIndex(void)
{
    return (uint16)(MMIAPIDCD_GeSettingInfo()->sim_slected);
}

/*****************************************************************************/
//  Description : get dcd show readed  idex
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDCDShowReadIndex(void)
{
    uint16 index=0;
    if (! MMIAPIDCD_GeSettingInfo()->is_show_readed)
    {
        index = 1;
    }
    
    return index;
}

/*****************************************************************************/
//  Description : mmi dcd update start
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note: last modified 2009.11.17
/*****************************************************************************/
PUBLIC void MMIDCD_UpdateStart(MMIDCD_REQ_TYPE_E req_type)
{
    //SCI_TRACE_LOW:"[MMIDCD]: MMIDCD_UpdateStart enter, dcd update start: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1088_112_2_18_2_12_56_251,(uint8*)"d", req_type);
    /*if(req_type != HTTP_REQ_TYPE_RETRY )
    {
        MMIDCD_ErrorRetrySetDefault();
    } */   
    if((HTTP_REQ_TYPE_RETRY != req_type) && (HTTP_REQ_TYPE_MAN != req_type))
    {
        DCD_Sendsig(DCD_SIG_DEFALUT_RETRY_TIMER_IND, req_type);
    }  
    
    if(MMIAPIDCD_DisplayIsValid() && MMIDCD_IsMccNetwork())        
    {
        DCD_Sendsig(DCD_SIG_XML_DL_REQ, req_type);
    }
}
/*****************************************************************************/
//  Description : load dcd item  list into 1 page of dcd main win
//  Global resource dependence : 
//  Author:
//  Note:  modified by jun.hu 2009/10/17 对旧的代码进行了简化...
/*****************************************************************************/
LOCAL void DCD_AppendListItem( MMI_CTRL_ID_T   list_ctrl_id )
{
    uint8                   item_index                 = 0;
    GUILIST_ITEM_T          item_t                     = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data                  = {0}; /*lint !e64*/
    uint16                  item_show_num              = 0;
    uint16                  sel_item_index             = 0;
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_2TEXT_AND_ICON;  //双行字    
    item_t.item_data_ptr = &item_data;   
    
    if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == list_ctrl_id)
    {
        item_show_num = MMIDCDFILE_GetLocalItemCount();
        sel_item_index = s_local_list_item_index;
        s_local_list_total_num = item_show_num;
        GUILIST_SetTitleIndexType(list_ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
        GUILIST_SetTitleStyle(list_ctrl_id, GUILIST_TITLE_ONLY);
    }
    else if (MMIDCD_ITEM_LIST_CTRL_ID == list_ctrl_id)
    {
        uint16   page_index    = s_dcd_gui_struct.current_page_index;
        sel_item_index = s_dcd_gui_struct.current_index_in_page;
        item_show_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, TRUE);
        s_dcd_gui_struct.current_item_num = item_show_num; 
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDCD]:DCD_AppendListItem invalid ctrl_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1135_112_2_18_2_12_56_252,(uint8*)"d",list_ctrl_id);
    }


    // 把设置最大值的地方移动这了....
    GUILIST_SetMaxItem(list_ctrl_id, item_show_num, TRUE );

    for (item_index = 0; item_index <item_show_num; item_index++ )
    {             
        GUILIST_AppendItem(list_ctrl_id, &item_t );
    }

    // 重新设置光标的位置 
    GUILIST_SetCurItemIndex(list_ctrl_id, sel_item_index);

    if (0 == item_show_num) //没有未读条目
    {
        GUILIST_EMPTY_INFO_T empty_info = {0};
        MMI_STRING_T  dststring= {0};
        
        //快讯内容为空
        MMI_GetLabelTextByLang(TXT_DCD_CONTENT_IS_EMPTY, &dststring);
        
        empty_info.text_buffer = dststring;
        GUILIST_SetEmptyInfo(list_ctrl_id, &empty_info);
    }
    
}
/*****************************************************************************/
//  Description : 设置dcd详情的标题图片
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/10/27 把旧的DCD_SetItemDetails一拆为二
/*****************************************************************************/
LOCAL void DCD_SetItemTitlePicAndText(
                                      wchar * picture_full_path_ptr, 
                                      wchar * title_content_ptr     //标题文字的指针
                                      ) 
{    
    GUIRICHTEXT_ITEM_T  dcd_ui_item = {0};  /*lint !e64*/
    uint16 txt_item_index = 0;
    
    // 首先是设置title的图片.....
    if(MMIAPIFMM_IsFileExist(picture_full_path_ptr, MMIAPICOM_Wstrlen(picture_full_path_ptr)))
    {
        dcd_ui_item.img_type = GUIRICHTEXT_IMAGE_FILE;
        dcd_ui_item.img_data.file.filename_ptr = picture_full_path_ptr; 
        dcd_ui_item.img_data.file.filename_len = MMIAPICOM_Wstrlen(picture_full_path_ptr); 
    }
    else
    {
        dcd_ui_item.img_type = GUIRICHTEXT_IMAGE_RES;
        dcd_ui_item.img_data.res.id = IMAGE_DCD_DEFAULT_IMAGE;
    }
    
    dcd_ui_item.img_set_size = TRUE;
    dcd_ui_item.img_setting_width = MMIDCD_ITEM_PICTURE_WIDTH;
    dcd_ui_item.img_setting_height = MMIDCD_ITEM_PICTURE_HIGHT;
    
    // 设置标题文字
    if(PNULL != title_content_ptr)
    {
        dcd_ui_item.text_type = GUIRICHTEXT_TEXT_BUF;
        dcd_ui_item.text_data.buf.str_ptr = title_content_ptr;
        dcd_ui_item.text_data.buf.len = (uint16)MMIAPICOM_Wstrlen(title_content_ptr);
    }
    
    GUIRICHTEXT_AddItem(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID, &dcd_ui_item, &txt_item_index);
        
}
/*****************************************************************************/
//  Description : 将dcd item  详情内容加到Richtext中
//  Global resource dependence : none
//  Author: jun.hu  
//  Note: jun.hu 2009/10/27 把旧的DCD_SetItemDetails一拆为二
//        2009/11/26 规范了宏的使用，加上了保护措施..
/*****************************************************************************/
LOCAL void DCD_SetItemDetails(uint16 real_item_index) 
{
    GUIRICHTEXT_ITEM_T  dcd_ui_item = {0};  /*lint !e64*/
    uint16 page_index = s_dcd_gui_struct.current_page_index;
    uint16 txt_item_index = 0;
    wchar  buf_arr[MMIDCD_MAX_SUMMARY_STR_LEN +1] = {0};
    wchar  picture_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    //SCI_TRACE_LOW:"[MMIDCD] pageindex = %d, reaindex = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1215_112_2_18_2_12_56_253,(uint8*)"dd", page_index, real_item_index);
    
    if(!MMIAPIDCD_GetItemAlreadyRead(page_index, real_item_index))
    {   //把该条设置为已读.....
        MMIDCD_SetItemAlreadyRead(page_index, real_item_index);
    }
    
    // 先去掉控件上旧的内容....
    GUIRICHTEXT_DeleteAllItems(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID);

    // 标题文字
    MMIAPIDCD_GetEntryTitle(buf_arr, MMIDCD_MAX_SUMMARY_STR_LEN, page_index, real_item_index);
    // 标题图片....
    MMIDCDFILE_GetOnePictureFullPath(picture_full_path,
                                     MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"ctxt-id"),
                                     MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"type"),
                                     MMIDCD_PICTURE_DIR_PATH
                                    );

    DCD_SetItemTitlePicAndText(picture_full_path, buf_arr);
    dcd_ui_item.text_type = GUIRICHTEXT_TEXT_BUF;
    SCI_MEMSET(buf_arr, 0, sizeof(buf_arr));
    dcd_ui_item.text_data.buf.str_ptr = buf_arr;
    // 设置正文
    MMIAPIDCD_GetEntrySummary(buf_arr, MMIDCD_MAX_SUMMARY_STR_LEN, page_index, real_item_index);
    dcd_ui_item.text_data.buf.len = MIN(MMIDCD_MAX_SUMMARY_STR_LEN, MMIAPICOM_Wstrlen(buf_arr));/*lint !e666*/
    
    GUIRICHTEXT_AddItem(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID, &dcd_ui_item, &txt_item_index);

}
/*****************************************************************************/
//  Description : 将本地保存的dcd item  详情内容加到Richtext中
//  Global resource dependence : none
//  Author: jun.hu
//  Note: jun.hu 2009/10/27 把旧的DCD_SetItemDetails一拆为二
//        2009/11/26 规范了宏的使用，加上了保护措施..
/*****************************************************************************/
LOCAL void DCD_SetLocalItemDetails(uint16 item_index) 
{
    GUIRICHTEXT_ITEM_T  dcd_ui_item = {0};   /*lint !e64*/
    wchar  picture_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};

    uint16 txt_item_index = 0;
    uint8* content_ptr = PNULL;  
    uint16  wlen = 0;    
    wchar buf_arr[MMIDCD_MAX_SUMMARY_STR_LEN +1] = {0};
    MMIDCD_LOCAL_FILE_NODE_T cur_node = {0};
    BOOLEAN read_result = FALSE;
    
    if(MMIDCDFILE_ReadLocalItemInfo(&cur_node, item_index))
    {
        // 先去掉控件上旧的内容....
        GUIRICHTEXT_DeleteAllItems(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID);
        
        // 标题文字
        content_ptr = cur_node.title_str;
        //wlen=strlen(content_ptr);//modified by @jun.hu, 2009/9/18 for cr00152381, 带有汉字,还有格式等的不能用strlen
        wlen = cur_node.title_len;
        GUI_OtherCodeToWstr(buf_arr, MMIDCD_MAX_SUMMARY_STR_LEN, GUI_CHAR_CODE_UCS2_BE, content_ptr, wlen);
        // 标题图片....
        MMIDCDFILE_GetOnePictureFullPath(picture_full_path,
                                        (char *)cur_node.content_text_id_str,
                                        (char *)cur_node.content_type_str,
                                        MMIDCD_LOCAL_PICTURE_DIR_PATH
                                        );
        
        DCD_SetItemTitlePicAndText(picture_full_path, buf_arr);
        // 设置正文
        dcd_ui_item.text_type = GUIRICHTEXT_TEXT_BUF;
        SCI_MEMSET(buf_arr, 0, sizeof(buf_arr));
        content_ptr = cur_node.summary_str;
        
        wlen = cur_node.summary_len;
        wlen = MIN(cur_node.summary_len, MMIDCD_MAX_SUMMARY_STR_LEN * sizeof(wchar));
        GUI_OtherCodeToWstr(buf_arr, MMIDCD_MAX_SUMMARY_STR_LEN, GUI_CHAR_CODE_UCS2_BE, content_ptr, wlen);
        dcd_ui_item.text_data.buf.len = MIN(MMIAPICOM_Wstrlen(buf_arr), MMIDCD_MAX_SUMMARY_STR_LEN);/*lint !e666*/
        dcd_ui_item.text_data.buf.str_ptr = buf_arr;
        
        GUIRICHTEXT_AddItem(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID, &dcd_ui_item, &txt_item_index);
        
        read_result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD read item info fail, local index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1297_112_2_18_2_12_57_254,(uint8*)"d", item_index);
    }
    

    if(! read_result)
    {
        MMI_STRING_T        read_fail = {0};
        dcd_ui_item.img_type = GUIRICHTEXT_IMAGE_NONE;
        dcd_ui_item.text_type = GUIRICHTEXT_TEXT_BUF;
        MMI_GetLabelTextByLang(TXT_DCD_READ_FAIL, &read_fail);
        dcd_ui_item.text_data.buf.len = MIN(read_fail.wstr_len, MMIDCD_MAX_SUMMARY_STR_LEN);
        dcd_ui_item.text_data.buf.str_ptr = read_fail.wstr_ptr;
        GUIRICHTEXT_AddItem(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID, &dcd_ui_item, &txt_item_index);
    }
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void DCD_EnableGrayedItemMenu(MMI_WIN_ID_T   win_id,   MMI_CTRL_ID_T  ctrl_id)
{
    uint8 page_index = s_dcd_gui_struct.current_page_index;
    uint16           nodex_index=0;      //node index,from 0
    uint32           parent_node_id=0;    //parent node id    
    MMI_CTRL_ID_T    menu_ctrl_id=MMIDCD_PREVIEW_MENU_CTRL_ID;
    
    if(ctrl_id != MMIDCD_LOCAL_ITEM_LIST_CTRL_ID)
    {
        if (MMIDCDFILE_IsDefaultXML(page_index))
        {       
            if (MMK_IsOpenWin(MMIDCD_ITEM_MENU_WIN_ID))
            {                
                nodex_index = 1;
                menu_ctrl_id=MMIDCD_ITEM_MENU_CTRL_ID;      
            }
            else if(MMK_IsOpenWin(MMIDCD_PREVIEW_MENU_WIN_ID))
            {
                nodex_index = 0;
                menu_ctrl_id=MMIDCD_PREVIEW_MENU_CTRL_ID;       
            }
            
            GUIMENU_SetNodeGrayed(TRUE, nodex_index,parent_node_id,menu_ctrl_id);
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_OpenSettingWin(void)
{
    MMK_CreateWin((uint32*)MMIDCD_SETTING_WIN_TAB, (ADD_DATA)MMIDCD_SETTING_SELECT_DCD_OPEN);
}

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author:jibin
//  Note:
/*****************************************************************************/
LOCAL void DcdInsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                             uint16             node_id,        //节点编号
                             uint16             parent_node_id, //父节点编号
                             uint16             index           //位置
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

/*****************************************************************************/
//  Description : handle dcd list mark option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDcdListMarkOperation(
                                      MMI_CTRL_ID_T   list_ctrl_id,
                                      DCD_MARR_OP_E  mark_type
                                      )
{
    uint16      mark_num = 0;
    uint16      cur_index = 0;
    uint16      item_num = GUILIST_GetTotalItemNum(list_ctrl_id);
   
    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
    if (0 == mark_num)
    {
        //set list enable mark
        GUILIST_SetTextListMarkable(list_ctrl_id, TRUE);
        //set mark max number
        GUILIST_SetMaxSelectedItem(list_ctrl_id, (uint16)item_num);
    }
    //get item index
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    //mark item
    if(DCD_MARK_CURONE == mark_type)
    {
        GUILIST_SetSelectedItem(list_ctrl_id, cur_index, TRUE);
    }
    else if(DCD_MARK_ALL == mark_type)
    {
        GUILIST_SetAllSelected(list_ctrl_id, TRUE);
    }
}
/*****************************************************************************/
//  Description : handle dcd list unmark option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDcdListUnMarkOperation(
                                        MMI_CTRL_ID_T   list_ctrl_id,
                                        DCD_MARR_OP_E  mark_type
                                        )
{
    uint16  mark_num = 0;
    uint16  cur_index = 0;

    switch(mark_type)
    {
        case DCD_UNMARK_CURONE:
        {    //get item index
            cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            //cancel mark item
            GUILIST_SetSelectedItem(list_ctrl_id, cur_index, FALSE);
            //get mark num
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
            if (0 == mark_num)
            {
                //set list enable mark
                GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
            }
        }
        break;
        
        case DCD_UNMARK_ALL:
        {
            GUILIST_SetAllSelected(list_ctrl_id, FALSE);
            GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
        }
        break;

        default:
        break;
    }

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void DeleteDcdLocalFileItem( const uint16 item_index )
{
    MMIDCD_LOCAL_FILE_NODE_T cur_node = {0};
    uint16  local_real_index = 0;
    if(MMIDCDFILE_LocalIndexRelativeToAbs(&local_real_index, item_index))
    {
        MMIDCDFILE_DeleteOnePicture((char *)cur_node.content_text_id_str, (char *)cur_node.content_type_str, TRUE);
        
        if(!MMIDCDFILE_DeleteLocalSingleItem(local_real_index))
        {
            //SCI_TRACE_LOW:"MMIDCD delete fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1464_112_2_18_2_12_57_255,(uint8*)"");
        }
    }

}
/*****************************************************************************/
//  Description : handle dcd local list item delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/11/13
/*****************************************************************************/
LOCAL void HandleDcdLocalListDeleteOperation(MMI_CTRL_ID_T   list_ctrl_id)
{
    uint16    mark_num = 0; 
    uint16    i = 0;
    uint16    item_index = 0;
    uint16    selected_idx[DCD_LOCAL_MAX_ITEM_NUM + 1] = {0};
    uint16    real_idx[DCD_LOCAL_MAX_ITEM_NUM + 1] = {0};
    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, selected_idx, DCD_LOCAL_MAX_ITEM_NUM);
    if (0 == mark_num)
    {
        //ger current index
        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        DeleteDcdLocalFileItem(item_index);
        
    }
    else
    {    
        // 先求出它们的相对值转化成绝对值
        for(i = 0; i < mark_num; i++)
        {
            MMIDCDFILE_LocalIndexRelativeToAbs(&real_idx[i], selected_idx[i]);
        }
        
        MMIDCDFILE_DeleteLocalMultItem(real_idx, mark_num);
        
        GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
    }
}
/*****************************************************************************/
//  Description : handle dcd list item delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDcdListItemDeleteOperation(MMI_CTRL_ID_T   list_ctrl_id)
{
    uint16    mark_num = 0; 
    uint16    i = 0;
    uint16    page_index = s_dcd_gui_struct.current_page_index;
    uint16    item_index = 0;
    uint16    real_item_index = 0;
    uint16    selected_idx[MMIDCD_MAX_ITEM_NUM_PER_PAGE + 1] = {0};
    uint16    real_idx[MMIDCD_MAX_ITEM_NUM_PER_PAGE + 1] = {0};
    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, selected_idx, MMIDCD_MAX_ITEM_NUM_PER_PAGE);
    if (0 == mark_num)
    {
        //ger current index
        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        
        real_item_index = MMIDCD_GetRealItemIndex(page_index, item_index, TRUE);
        MMIDCDFILE_DeletePageSingleItem(page_index, real_item_index);
    }
    else
    {    
        // 先求出它们的相对值转化成绝对值
        for(i = 0; i < mark_num; i++)
        {
            real_idx[i] = MMIDCD_GetRealItemIndex(page_index, selected_idx[i], TRUE);
        }
        
        MMIDCDFILE_DeletePageMultItem(page_index, real_idx, mark_num);
        
        GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDcdPrewItemDeleteOperation(MMI_CTRL_ID_T ctrl_id)
{
    if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == ctrl_id)
    {
        DeleteDcdLocalFileItem(s_local_list_item_index);
    }
    else
    {
        MMIDCDFILE_DeletePageSingleItem(s_dcd_gui_struct.current_page_index, s_dcd_gui_struct.current_real_index_in_page);
    }
}
/*****************************************************************************/
//  Description :动态创建dcd的mark菜单.....
//  Global resource dependence : 
//  Author:jun.hu
//  Note:  2009/9/15
//  para 1: MMI_CTRL_ID_T menuctrl_id,         //要创建的meun ctrl id
//  para 2: MMI_CTRL_ID_T list_ctrl_id,        //从哪个listbox 进来的
//  para 3: DCD_ITEMMENU_NODE_E parent_menu_id // 父节点(0表示无）
/*****************************************************************************/
LOCAL void  CreateDcdMarkMenu(MMI_CTRL_ID_T menuctrl_id,  
                              MMI_CTRL_ID_T list_ctrl_id, 
                              DCD_ITEMMENU_NODE_E parent_menu_id
                              )
{
    uint16 selectnum = 0;
    uint16 totalnum  = 0;
    uint16 index     = 0;
    uint16 curindex = 0;
    const GUILIST_ITEM_T *item_ptr = PNULL;
    uint16 parent_node_id = (uint16)parent_menu_id;
    
    totalnum = GUILIST_GetTotalItemNum(list_ctrl_id);
        
    if(0 < totalnum)
    {
        // 得到list上选中的条目数
        selectnum = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);

        /*if(MMIDCD_ITEM_LIST_CTRL_ID == list_ctrl_id)
        {
            curindex = s_dcd_gui_struct.current_index_in_page;
        }
        else if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == list_ctrl_id)
        {
            curindex = s_local_list_item_index;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD please check list ctrl!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1589_112_2_18_2_12_57_256,(uint8*)"""MMIDCD please check list ctrl!!");
        }*/

        curindex = GUILIST_GetCurItemIndex(list_ctrl_id);
        item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id, curindex);
        if(PNULL != item_ptr)
        {
            DcdInsertNodeByLabel(menuctrl_id, STXT_MARK,            DCD_ITEMMENU_SUB_MARK,        parent_node_id, index++);
            DcdInsertNodeByLabel(menuctrl_id, TXT_CANCEL_MARK,     DCD_ITEMMENU_CANCEL_MARK,     parent_node_id, index++);
            DcdInsertNodeByLabel(menuctrl_id, TXT_MARK_ALL,        DCD_ITEMMENU_MARK_ALL,        parent_node_id, index++);
            DcdInsertNodeByLabel(menuctrl_id, TXT_CANCEL_MARK_ALL, DCD_ITEMMENU_CANCEL_ALL_MARK, parent_node_id, index++);
            
            // 如果当前的条目被选中
            if(GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
            {
                // 灰化标记
                GUIMENU_SetNodeGrayed(TRUE, 0, parent_node_id, menuctrl_id);
                if(selectnum >= totalnum)
                {   // 灰化标记全部
                    GUIMENU_SetNodeGrayed(TRUE, 2, parent_node_id, menuctrl_id);
                }
            }
            else// 如果当前的条目没有被选中
            {
                // 灰化取消标记
                GUIMENU_SetNodeGrayed(TRUE, 1, parent_node_id, menuctrl_id);
                // 如果一条也没有被选中，灰化取消全部标记
                if(0 == selectnum)
                {
                    GUIMENU_SetNodeGrayed(TRUE, 3, parent_node_id, menuctrl_id);
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDCD]:CreateDcdMarkMenu Get list data error!!!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1624_112_2_18_2_12_57_257,(uint8*)"");
        }
    }
}
/*****************************************************************************/
//  Description :call just by dcd main win
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note:  modified by jun.hu
/*****************************************************************************/
LOCAL void  DCD_OpenMainItemMenu(void)
{
    MMI_WIN_ID_T    win_id = MMIDCD_ITEM_MENU_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMIDCD_ITEM_MENU_CTRL_ID;
    MMI_TEXT_ID_T   handhold_label = TXT_NULL;
    uint16          one_level_index = 0;  // 一级菜单的index
    uint16          two_level_index = 0;  // 二级菜单的index  
    BOOLEAN         is_show_update  = FALSE;
    //creat dynamic menu
    GUIMENU_CreatDynamic(PNULL,MMIDCD_ITEM_MENU_WIN_ID,ctrl_id,GUIMENU_STYLE_POPUP);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, STXT_SELECT, TXT_NULL, STXT_RETURN);
    MMK_SetAtvCtrl(win_id, ctrl_id);

    if(MMIDCD_IsIdle()||!MMIDCD_GetUpdateStopFlag())
    {
        is_show_update = TRUE;
    }
    else // 如果DCD正在更新过程中并且是自动更新
    {
       if(HTTP_REQ_TYPE_TTL == MMIDCD_GetCurProcessReqType() || HTTP_REQ_TYPE_SVR == MMIDCD_GetCurProcessReqType())
       {
            is_show_update = TRUE;
       }
    }
    // update or cancel update
    if(is_show_update)
    {
        handhold_label = TXT_COMMON_UPDATE;
        DcdInsertNodeByLabel(ctrl_id, handhold_label, DCD_ITEMMENU_UPDATE_START, 0, one_level_index++); 
    }
    else
    {
        handhold_label = TXT_DCD_CANCEL_UPDATE;
        DcdInsertNodeByLabel(ctrl_id, handhold_label, DCD_ITEMMENU_UPDATE_STOP, 0, one_level_index++); 
    }    
    
    if(0 < s_dcd_gui_struct.current_item_num)
    {
        //delete
        DcdInsertNodeByLabel(ctrl_id, TXT_DELETE, DCD_ITEMMENU_ITEM_DELETE, 0, one_level_index++);
        
        //mark
        DcdInsertNodeByLabel(ctrl_id, STXT_MARK, DCD_ITEMMENU_MARK, 0, one_level_index++);
    }
    
    // 根据情况创建标记菜单
    CreateDcdMarkMenu(ctrl_id, MMIDCD_ITEM_LIST_CTRL_ID, DCD_ITEMMENU_MARK);
    
    //local preview
    DcdInsertNodeByLabel(ctrl_id, TXT_DCD_PREVIEW_LOCAL_ITEM, DCD_ITEMMENU_ITEM_PREVIEW_LOCAL, 0, one_level_index++);
    
    //channel management
    DcdInsertNodeByLabel(ctrl_id, TXD_DCD_CHANNEL_MANAGEMENT, DCD_ITEMMENU_CHANNEL_MANAGEMENT, 0, one_level_index++);
    
    two_level_index = 0;
    DcdInsertNodeByLabel(ctrl_id, TXT_ADD_CHANNEL, DCD_ITEMMENU_ITEM_ADD_CHANNEL, DCD_ITEMMENU_CHANNEL_MANAGEMENT, two_level_index++);
    
    DcdInsertNodeByLabel(ctrl_id, TXT_COMMON_DELETE_CHANNEL, DCD_ITEMMENU_ITEM_DELETE_CHANNEL, DCD_ITEMMENU_CHANNEL_MANAGEMENT, two_level_index++);
    
    //main preview 
    DcdInsertNodeByLabel(ctrl_id, TXT_DCD_ORDER_VIEW, DCD_ITEMMENU_ITEM_PREVIEW, 0, one_level_index++);
    
    //setting
    DcdInsertNodeByLabel(ctrl_id, STXT_MAIN_SETTING, DCD_ITEMMENU_ITEM_SETTING, 0, one_level_index++);
    
    //help
    DcdInsertNodeByLabel(ctrl_id, TXT_HELP, DCD_ITEMMENU_ITEM_SETTING_HELP, 0, one_level_index++);
    
}
/*****************************************************************************/
//  Description : ....
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/11/12
/*****************************************************************************/
LOCAL void HandleRequestUpateByMan(void)
{
    MMI_WIN_ID_T query_win_id = MMIDCD_QUERY_WIN_ID;
    MMIDCD_ERROR_CODE_E  dcd_check_error = MMIDCD_GetCheckForUpdateResult();
    
    if(dcd_check_error != DCD_ERROR_NULL)
    {
        MMIDCD_IndicationShow(HTTP_REQ_TYPE_MAN, dcd_check_error,TRUE);
    }
    else    
    {
        MMIPUB_OpenQueryWinByTextId(TXT_DCD_QUERY_UPDATE, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
    }    
}
/*****************************************************************************/
//  Description : 检查U盘的状态
//  Global resource dependence : 
//  Author:jun.hu
//  Note:  2010/2/1 
/*****************************************************************************/
LOCAL BOOLEAN IsUDiskRunning(void)
{
    BOOLEAN ret_value = TRUE;
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        ret_value = FALSE;
    }

    return ret_value;
}
/*****************************************************************************/
//  Description : 进入DCD各菜单项的通道函数....
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/7 把里面的内容从HandleItemMenuWinMsg中单独提取处理
/*****************************************************************************/
LOCAL void EnterDcdMenuItem(
                            MMI_WIN_ID_T  win_id,
                            const uint32  menuitemid
                            )
{
    
    MMI_WIN_ID_T query_win_id = MMIDCD_QUERY_WIN_ID;
    switch(menuitemid)
    {
        case DCD_ITEMMENU_UPDATE_START:
        {
            HandleRequestUpateByMan();
        }
        break;
        
        case DCD_ITEMMENU_UPDATE_STOP:
        {
            MMIPUB_OpenQueryWinByTextId(TXT_DCD_CANCEL_UPDATE, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
        }
        break;
        
        case DCD_ITEMMENU_ITEM_DELETE:
        {
            if(!IsUDiskRunning())
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
            }
        }
        break;
        
        case DCD_ITEMMENU_ITEM_PREVIEW:
        {   
            MMK_CreateWin((uint32*)MMIDCD_ORDER_DETAIL_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
        }
        break;
        
        case DCD_ITEMMENU_ITEM_PREVIEW_LOCAL:
        {
            if(!IsUDiskRunning())
            {
                if(0 < MMIDCDFILE_GetLocalItemCount())
                {
                    MMK_CreateWin((uint32*)MMIDCD_LOCAL_MAIN_WIN_TAB, PNULL);
                    MMK_CreateChildWin(MMIDCD_LOCAL_MAIN_WIN_ID, (uint32*)MMIDCD_LOCAL_MAIN_WIN_TAB, PNULL);  
                    MMK_CloseWin(win_id); 
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_CONTENT,TXT_NULL,IMAGE_PUBWIN_WARNING/*IMAGE_PUBWIN_WAIT*/,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            }
        }
        break;
        
        case DCD_ITEMMENU_SUB_MARK:
        {    
            HandleDcdListMarkOperation(MMIDCD_ITEM_LIST_CTRL_ID, DCD_MARK_CURONE);
            MMK_CloseWin(win_id);
        }
        break;
        
        case DCD_ITEMMENU_CANCEL_MARK:
        {
            HandleDcdListUnMarkOperation(MMIDCD_ITEM_LIST_CTRL_ID, DCD_UNMARK_CURONE);
            MMK_CloseWin(win_id);
        }
        break;
        
        case DCD_ITEMMENU_MARK_ALL:
        {
            HandleDcdListMarkOperation(MMIDCD_ITEM_LIST_CTRL_ID, DCD_MARK_ALL);
            MMK_CloseWin(win_id);
        }
        break;
        
        case DCD_ITEMMENU_CANCEL_ALL_MARK:
        {
            HandleDcdListUnMarkOperation(MMIDCD_ITEM_LIST_CTRL_ID, DCD_UNMARK_ALL);
            MMK_CloseWin(win_id);
        }
        break;
        
        case DCD_ITEMMENU_ITEM_ADD_CHANNEL:
        {
            char uri[DCD_MAX_HREF_LEN + 1] = {0};
            if(PNULL != MMIAPIDCD_MAIN_GetNetHomePage())
            {
                strcpy(uri, MMIAPIDCD_MAIN_GetNetHomePage()); /*lint !e668*/
                strcat(uri, DCD_ADD_CHANNEL_PART_URL);
                EntryInternet(uri);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }
        break;
        
        case DCD_ITEMMENU_ITEM_DELETE_CHANNEL:
        {
            char uri[DCD_MAX_HREF_LEN + 1] = {0};
            if(PNULL != MMIAPIDCD_MAIN_GetNetHomePage())
            {
                strcpy(uri, MMIAPIDCD_MAIN_GetNetHomePage()); /*lint !e668*/
                strcat(uri, DCD_DEL_CHANNEL_PART_URL);
                EntryInternet(uri);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }           
        break;

        case DCD_ITEMMENU_ITEM_SETTING:
        {
            MMK_CreateWin((uint32 *)MMIDCD_SETTING_MAINLIST_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
        }
        break;

        case DCD_ITEMMENU_ITEM_SETTING_HELP:
        {
            MMK_CreateWin((uint32 *)MMIDCD_HELP_WIN_TAB, PNULL);
        }   
        break;
        
       default:
        
        break;
   }
}
/*****************************************************************************/
//  Description : DCD主菜单的消息响应函数
//  Global resource dependence : 
//  Author:    修改by jun.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleItemMenuWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 node_id = 0;
    MMI_WIN_ID_T query_win_id = MMIDCD_QUERY_WIN_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            DCD_OpenMainItemMenu();
            MMK_SetAtvCtrl(win_id, MMIDCD_ITEM_MENU_CTRL_ID);
            DCD_EnableGrayedItemMenu(win_id,MMIDCD_ITEM_LIST_CTRL_ID);          
        }
        break;
        
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            node_id = GUIMENU_GetCurNodeId(MMIDCD_ITEM_MENU_CTRL_ID);
            EnterDcdMenuItem(win_id, node_id);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        node_id = GUIMENU_GetCurNodeId(MMIDCD_ITEM_MENU_CTRL_ID);
        switch(node_id)
        {
        case DCD_ITEMMENU_ITEM_DELETE: 
            {
                HandleDcdListItemDeleteOperation(MMIDCD_ITEM_LIST_CTRL_ID);
                MMIPUB_OpenAlertSuccessWin( TXT_DELETED);
                UpdateDCDList(MMIDCD_ITEM_LIST_CTRL_ID, FALSE);
                MMK_CloseWin(MMIDCD_ITEM_MENU_WIN_ID);
            }
            break;

        case DCD_ITEMMENU_UPDATE_START:
            {
                //SCI_TRACE_LOW:"MMIDCD anim has start"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_1929_112_2_18_2_12_58_258,(uint8*)"");
                if(MMIDCD_IsIdle())
                {
                    MMIDCD_UpdateStart(HTTP_REQ_TYPE_MAN);
                    MMIDCD_SetUpdateStopFlag(TRUE);
                    // 只有当连接模式为手动连接时才提示"手动更新已发起，快讯应用下次同步仍需手动发起", 其他模式不提示for cr154182..
                    if(MMIDCD_CONNECT_MODE_NEVER == MMIAPIDCD_GeSettingInfo()->connect_mode)
                    {
                        MMIPUB_OpenAlertSuccessWin( TXT_DCD_UPDATE_SEND);
                    }
                    else
                    {   
                        query_win_id = MMIDCD_MAIN_QUERY_WIN_ID; // jun.hu 2009/10/29 使用不同的窗口id for158442.....
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DCD_UPDATE_BEGIN,TXT_NULL,IMAGE_PUBWIN_WAIT,&query_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DCD_UPDATING);
                }
                MMK_CloseWin(MMIDCD_ITEM_MENU_WIN_ID);
            }
            break;

        case DCD_ITEMMENU_UPDATE_STOP:            
            if(!MMIDCD_IsIdle())
            {
                MMI_WIN_ID_T wait_win_id = MMIDCD_STOP_UPDATING_WAITING_WIN_ID;
                uint32 time_out = 0;                            
                //进入等待界面
                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,
                    &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,
                    HandleDCDStopUpdateDataWaitWin);
                
                DCD_Sendsig(DCD_SIG_DL_CANCLE_REQ,HTTP_REQ_TYPE_NULL);
                MMIDCD_SetUpdateStopFlag(FALSE);
            }
            MMK_CloseWin(MMIDCD_ITEM_MENU_WIN_ID);
            break;
            
        default:
            
            break;
        }
        break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/9/15
/*****************************************************************************/
LOCAL void CreateLocalItemDyMenu(const MMI_CTRL_ID_T menuctrl_id)
{
     
    uint16 rootindex = 0;
    uint16 subindex = 0;
    uint16 selectnum = 0;
    GUIMENU_SetDynamicMenuSoftkey(menuctrl_id, STXT_SELECT, TXT_NULL, STXT_RETURN);
    // jun.hu 2009/11/10 for cr157380, 当有标记时，则不显示发送菜单...
    selectnum = GUILIST_GetSelectedItemIndex(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID, PNULL, 0);
    // 如果没有被选中的条目....
    if(0 == selectnum)
    {   
        //创建发送的子菜单
        DcdInsertNodeByLabel(menuctrl_id, TXT_SEND, DCD_ITEMMENU_SEND, 0,  rootindex++);

        DcdInsertNodeByLabel(menuctrl_id, TXT_SMS, DCD_ITEMMENU_SEND_BY_SMS, DCD_ITEMMENU_SEND, subindex++);
        DcdInsertNodeByLabel(menuctrl_id, TXT_MMS, DCD_ITEMMENU_SEND_BY_MMS, DCD_ITEMMENU_SEND, subindex++);
    }
    DcdInsertNodeByLabel(menuctrl_id, TXT_DELETE, DCD_ITEMMENU_ITEM_DELETE, 0,     rootindex++);
    DcdInsertNodeByLabel(menuctrl_id, STXT_MARK, DCD_ITEMMENU_MARK, 0,    rootindex++);
    
    CreateDcdMarkMenu(MMIDCD_LOCAL_ITEM_MENU_CTRL_ID, MMIDCD_LOCAL_ITEM_LIST_CTRL_ID, DCD_ITEMMENU_MARK);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalItemMenuWinMsg(
                                             MMI_WIN_ID_T        win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 node_id = 0;
    MMI_WIN_ID_T query_win_id =   MMIDCD_QUERY_WIN_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIMENU_CreatDynamic(PNULL,win_id, MMIDCD_LOCAL_ITEM_MENU_CTRL_ID, GUIMENU_STYLE_POPUP);
            CreateLocalItemDyMenu(MMIDCD_LOCAL_ITEM_MENU_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMIDCD_LOCAL_ITEM_MENU_CTRL_ID);
        }    
        break;
        
    case MSG_KEYDOWN_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        node_id = GUIMENU_GetCurNodeId(MMIDCD_LOCAL_ITEM_MENU_CTRL_ID);
        switch(node_id)
        {
        case DCD_ITEMMENU_SEND_BY_SMS:
            SendLocalBySMS();
            MMK_CloseWin(win_id);
            break;
            
        case DCD_ITEMMENU_SEND_BY_MMS:
            SendLocalByMMS();
            MMK_CloseWin(win_id);
            break;

        case DCD_ITEMMENU_SUB_MARK:
            HandleDcdListMarkOperation(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID, DCD_MARK_CURONE);
            MMK_CloseWin(win_id);
            break;
            
        case DCD_ITEMMENU_CANCEL_MARK:
            HandleDcdListUnMarkOperation(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID, DCD_UNMARK_CURONE);
            MMK_CloseWin(win_id);
            break;
            
        case DCD_ITEMMENU_MARK_ALL:
            HandleDcdListMarkOperation(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID, DCD_MARK_ALL);
            MMK_CloseWin(win_id);
            break;
        case DCD_ITEMMENU_CANCEL_ALL_MARK:
            HandleDcdListUnMarkOperation(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID, DCD_UNMARK_ALL);
            MMK_CloseWin(win_id);
            break;
            
        case DCD_ITEMMENU_ITEM_DELETE:
            {
                if(!IsUDiskRunning())
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
                }
            }
            break;            

        default:
            break;
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        node_id = GUIMENU_GetCurNodeId(MMIDCD_LOCAL_ITEM_MENU_CTRL_ID);
        switch(node_id)
        {
        case DCD_ITEMMENU_ITEM_DELETE: 
            {
                HandleDcdLocalListDeleteOperation(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID);
                UpdateDCDList(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID, FALSE);
                MMIPUB_OpenAlertSuccessWin( TXT_DELETED);
                MMK_CloseWin(win_id);//z.f.add
            }
            break;
            
        default:
            
            break;
        }
        break;
        
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
/*****************************************************************************/
//  Description : open input homepage window of dcd
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_OpenMainWinFromIdle(BOOLEAN openMainmenu)
{   
   s_dcd_gui_struct.current_page_index = s_dcd_gui_struct.cur_idle_page_index;
   MMIDCD_OpenMainWin(openMainmenu);
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetDcdCurrentPageIndex()
{	
	s_dcd_gui_struct.cur_idle_page_index  = s_dcd_gui_struct.current_page_index;
	return  s_dcd_gui_struct.current_page_index;
}
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_SetDcdCurrentPageIndex(uint16 current_page_index,uint16 current_real_index_in_page)
{
	s_dcd_gui_struct.current_page_index = current_page_index;
	s_dcd_gui_struct.cur_idle_page_index = current_page_index;
	s_dcd_gui_struct.current_real_index_in_page = s_dcd_gui_struct.cur_idle_realitem_index = current_real_index_in_page;
}

/*****************************************************************************/
//  Description : open input homepage window of dcd
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_OpenMainWin(BOOLEAN openMainmenu)
{   
    uint16 useable_item_num = 0;

    if(!IsUDiskRunning()) 
    {
        if(MMIAPIDCD_DcdIsOpen())        
        {     
            useable_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(s_dcd_gui_struct.cur_idle_page_index, MMIAPIDCD_GeSettingInfo()->is_show_readed);
            // 当指定进入DCD的主菜单或者当前的DCD的条目数为0时也进入主菜单....
            if(openMainmenu || 0 == useable_item_num)
            {
                /*create father win*/   
                MMK_CreateParentWin((uint32*)MMIDCD_MAIN_WIN_TAB, PNULL);
                MMK_CreateChildWin(MMIDCD_MAIN_WIN_ID, (uint32*)MMIDCD_ITEM_CHILD_WIN_TAB, PNULL);  
                MMK_SetChildWinFocus(MMIDCD_MAIN_WIN_ID, MMIDCD_ITEM_CHILD_WIN_ID); 
            }
            else
            {   
                // 从idle界面进入preview，获取list top index
                MMK_CreateWin((uint32*)MMIDCD_PREVIEW_WIN_TAB, (ADD_DATA)MMIDCD_IDLEWIN_TEXT_CTRL_ID);
            }
        }
        else
        {           
            MMIDCD_OpenSettingWin();     
        }
    }
}


/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIDCD_GetFocusCtrlRect(void)
{
        return s_dcd_tab_rect;
}
/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_CreateIdleCtrl(MMI_WIN_ID_T win_id )
{
    BOOLEAN                 result          = FALSE;
    MMI_CTRL_ID_T           ctrl_id         = MMIDCD_IDLEWIN_TAB_CTRL_ID;
    IGUICTRL_T              *tab_ctrl_ptr  = PNULL;
    MMI_CONTROL_CREATE_T    create          = {0};
    GUITAB_INIT_DATA_T     init_data       = {0};
    GUI_RECT_T child_win_rect = {0, MMI_DCD_IDLEWIN_LISTBOX_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_DCD_IDLEWIN_LISTBOX_BOTTOM};


    
    init_data.both_rect.h_rect = init_data.both_rect.v_rect = s_dcd_tab_rect;
    init_data.state=GUITAB_STATE_SINGLE_LINE;  //single line to show tab item.
    
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_TAB_ID;     //TAB CTRL
    create.parent_win_handle = win_id;     //parent win id!!
    create.init_data_ptr = &init_data;
    
    // 如果控件已经存在，则没必要再去创建....
    if(NULL != MMK_GetCtrlPtr(ctrl_id))
    {
        return FALSE;
    }

    tab_ctrl_ptr = MMK_CreateControl( &create );
    if(PNULL == tab_ctrl_ptr)
    {
        return FALSE;
    }

    // create dcd idle child win
    if(MMK_CreateChildWin(win_id, (uint32*)MMIDCD_IDLE_CHILD_WIN_TAB, PNULL))
    {
        //added by @jun, hu, 2009/8/11
        MMK_SetWinRect(MMIDCD_IDLE_CHILD_WIN_ID, &child_win_rect);
        result = TRUE;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DCD_Error_Handler(MMI_WIN_ID_T        *win_id_ptr,
                             MMIDCD_ERROR_CODE_E error_type,
                             MMI_WIN_PRIORITY_E  win_priority
                             )
{    
    MMI_STRING_T prompt_str = {0};
    
    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_Error_Handler() enter MMIDCD_Error_Handler = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2254_112_2_18_2_12_59_259,(uint8*)"d",error_type);

    if(DCD_ERROR_NULL == error_type)
    {
        return;
    }
    if(DCD_ERROR_SIM_FAIL == error_type)
    {
         MMIDCD_SimErrorInd();
    }
    else if(DCD_ERROR_NO_MEMORY == error_type)
    {
        uint32       time = 0;
        MMI_GetLabelTextByLang(TXT_DCD_NO_SPACE, &prompt_str);
        MMIPUB_OpenAlertWinByTextPtr(&time, &prompt_str, PNULL, IMAGE_PUBWIN_WARNING, win_id_ptr, &win_priority, MMIPUB_SOFTKEY_ONE, HandleNotEnoughtSpaceWin);
    }
    else
    {
        switch(error_type)
        {
        case DCD_ERROR_NETWORK_USING:           
        case DCD_ERROR_INVALID_NETWORK:
            MMI_GetLabelTextByLang(TXT_DCD_ERROR_WRONG_PROXY, &prompt_str);
            break;
            
        case DCD_ERROR_TIMEOUT:
            MMI_GetLabelTextByLang(TXT_COMM_TIMEOUT, &prompt_str);
            break;
            
        case DCD_ERROR_XML_FORMAT:        
        case DCD_ERROR_NO_XML:
        case DCD_ERROR_WRONG_DATA:
        case DCD_ERROR_DUMMY_DATA:      
            MMI_GetLabelTextByLang(TXT_UPDATE_FAIL, &prompt_str);
            break;  
            
        case DCD_ERROR_UDISK_USING:
            MMI_GetLabelTextByLang(TXT_DCD_UDISK_USING, &prompt_str);
            break;    
        case DCD_ERROR_FLY_MODE:
            MMI_GetLabelTextByLang(TXT_CLOSE_FLY_MODE_FIRST, &prompt_str);
            break;

        case DCD_ERROR_NOT_CMCC:     
            MMI_GetLabelTextByLang(TXT_DCD_IS_INVALID, &prompt_str);
            break;
            
        case DCD_ERROR_IS_BUSY:
            MMI_GetLabelTextByLang(TXT_DCD_UPDATING, &prompt_str);
            break;

        default:
            MMI_GetLabelTextByLang(TXT_UNKNOWN_ERROR, &prompt_str);
            break;
        }
        
        MMIPUB_OpenAlertWinByTextPtr(PNULL, &prompt_str, PNULL, IMAGE_PUBWIN_WARNING, win_id_ptr, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);
    }

}
/*****************************************************************************/
//  Description : DCD  wake up indication~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_IdleWinWakeupInd(MMI_WIN_ID_T win_id )
{
    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_IdleWinWakeupInd() enter "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2320_112_2_18_2_12_59_260,(uint8*)"");
    if(MMIDCD_IsWapPushUpdate() )
    {
        //SCI_TRACE_LOW:"[MMIDCD]:dcd will be wake up !! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2323_112_2_18_2_12_59_261,(uint8*)"");
        // 自动打开快讯
        MMIPUB_OpenAlertWarningWin(TXT_DCD_WAKEUP_OPEN);
        MMIDCD_HandleDCDSwitch(TRUE, FALSE);
        
        MMIDCD_SetWapPushUpdate(FALSE); // 清变量.....
    }
    
}

/*****************************************************************************/
//  Description : DCD  wake up indication~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_Wakeup(void )
{
   //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_Wakeup() enter: "
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2339_112_2_18_2_12_59_262,(uint8*)"");
   if(!MMIAPIDCD_DcdIsOpen())
   {      
       MMK_SendMsg(MAIN_IDLE_WIN_ID,MSG_DCD_TAB_UPDATE,PNULL);          
   }    
}
/*****************************************************************************/
//  Description : DCD 最低优先级提醒~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  result : TRUE: need error retry update
//                       FALSE; no need error retry
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IndicationShow( MMIDCD_REQ_TYPE_E req_type, MMIDCD_ERROR_CODE_E  error_code, BOOLEAN is_enble_show)
{
    
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T    win_id = 0;
    uint32 time_out = 0;   
    MMI_STRING_T prompt_str = {0};
    BOOLEAN result=FALSE;      
    MMI_WIN_ID_T query_win_id = MMIDCD_INDICATION_WIN_ID;
    wchar  wsr_dst[60] = {0};
    uint8  str_str[10]= {0};
    uint16 cpy_len = 0;
    MMI_STRING_T  updatstring= {0};

    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_IndicationShow() enter: "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2364_112_2_18_2_12_59_263,(uint8*)"");
    
    if(MMIAPIDCD_DcdIsOpen())
    {  

        MMK_CloseWin(query_win_id);
        MMK_CloseWin(MMIDCD_MAIN_QUERY_WIN_ID);
        
        win_id = MMK_GetFocusWinId();
        
        if((win_id >= MMIDCD_MAIN_WIN_ID) && (win_id < MMIDCD_NULL_WIN_ID) )
        {
            win_priority = WIN_ONE_LEVEL;
        }
        
        /*错误机制 : if man update, do not start retry update and just indicate error
        the others start retry update mechanism but no indication. */        
        if(error_code != DCD_ERROR_NULL)
        {
            if(is_enble_show)
            {
                DCD_Error_Handler(&query_win_id,error_code,win_priority);
            }           
            else if(HTTP_REQ_TYPE_MAN == req_type)      
            {
                if((error_code == DCD_ERROR_UDISK_USING) || (error_code ==DCD_ERROR_NO_MEMORY))
                {   
                    DCD_Error_Handler(&query_win_id,error_code,win_priority);
                }
                else
                {   
                    /*手动更新错误提示*/
                    MMI_GetLabelTextByLang(TXT_DCD_MAN_UPDATE_AGAIN, &prompt_str);
                    MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_QUERY,&query_win_id,&win_priority,MMIPUB_SOFTKEY_OKCANCEL,DCD_QueryWinMsg);           
                }
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIDCD]:start retry update mechanism!!! "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2402_112_2_18_2_12_59_264,(uint8*)"");
                if(error_code ==DCD_ERROR_NO_MEMORY)
                {
                    DCD_Error_Handler(&query_win_id,error_code,win_priority);
                }
                result = TRUE;       
            }
            
        }
        else
        {
            if((HTTP_REQ_TYPE_MAN == req_type) || (HTTP_REQ_TYPE_MR == req_type) )// for cr158489
            {
                uint16 new_items_count = MMIDCDFILE_GetUpDateNewItemsCount(); //added by jun.hu 12/2 for 增加更新条目数的提示...
                sprintf((char*)str_str,"%d",new_items_count);
                MMIAPICOM_StrToWstr(str_str, wsr_dst);
                if(1 < new_items_count)
                {
                    MMI_GetLabelTextByLang(TXT_DCD_UPDATE_DATA, &updatstring);
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_DCD_UPDATE_SINGLE_NEW_DATA, &updatstring);
                }

                if(60 > MMIAPICOM_Wstrlen(wsr_dst))
                {
                    cpy_len = MIN(updatstring.wstr_len, (60 - MMIAPICOM_Wstrlen(wsr_dst)));/*lint !e666*/
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_IndicationShow Please check here error!!!!!!!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2433_112_2_18_2_12_59_265,(uint8*)"");
                }

                MMIAPICOM_Wstrncat(wsr_dst, updatstring.wstr_ptr, cpy_len);
                prompt_str.wstr_ptr = wsr_dst;
                prompt_str.wstr_len = MMIAPICOM_Wstrlen(wsr_dst);

                MMIPUB_OpenAlertWinByTextPtr(PNULL,&prompt_str,PNULL,IMAGE_PUBWIN_WAIT,&query_win_id,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);   
            }
        }
    }
    
    return result;
    
}


/*****************************************************************************/
//  Description : set dcd page item bg
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void  DCD_SetPageFocusBgImage(BOOLEAN is_active)
{
    if(is_active)
    {
     GUITAB_SetItemSelBgImage(MMIDCD_IDLEWIN_TAB_CTRL_ID,IMAGE_DCD_TAB_FOCUS_BGD_IMAGE,TRUE);
    }
    else
    {
     GUITAB_SetItemSelBgImage(MMIDCD_IDLEWIN_TAB_CTRL_ID,IMAGE_DCD_TAB_CUR_BGD_IMAGE,TRUE);
    }

}

/*****************************************************************************/
//  Description : set dcd page title and list
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
LOCAL void DCD_AddTabsAndChildWindows(MMI_CTRL_ID_T ctrl_id, MMI_WIN_ID_T    win_id)
{
    
    MMI_STRING_T    str_text={0};
    uint32 page_num = 0;
    uint32           pos = 0;
    uint16 page_index=0;    
    uint16          img_width = 0;
    uint16  copy_len=0; 
    GUITAB_ITEM_T item_t = {0}; 
    
    if(s_dcd_gui_struct.total_page_num > MMIDCD_MAX_PAGE_NUM)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:s_dcd_gui_struct.total_page_num out of max range  error!!!!!!!! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2484_112_2_18_2_12_59_266,(uint8*)"");
        s_dcd_gui_struct.total_page_num = MMIDCD_MAX_PAGE_NUM;   
    }  
    
    GUITAB_SetMaxItemNum(ctrl_id, MMIDCD_MAX_PAGE_NUM); 
    GUIRES_GetImgWidthHeight(&img_width,PNULL,IMAGE_DCD_TAB_CUR_BGD_IMAGE,win_id);
    GUITAB_SetItemWidth(ctrl_id, img_width);
    GUITAB_SetDefaultIcon(ctrl_id,IMAGE_SECMENU_ICON_TOOL_DCD,IMAGE_SECMENU_ICON_TOOL_DCD);    
    
    GUITAB_SetItemDefaultHeight(ctrl_id, MMIDCD_TAB_DEFAULT_HEIGHT);
    if(ctrl_id == MMIDCD_IDLEWIN_TAB_CTRL_ID)
    {
        GUITAB_SetBackGroundImage(ctrl_id,IMAGE_DCD_IDLE_TAB_BGD_IMAGE);        
    }   
    
    page_num = s_dcd_gui_struct.total_page_num;
    
    //initialize
    str_text.wstr_ptr = MMIDCD_ALLOC((GUIRICHTEXT_FILENAME_MAX_LEN)*sizeof(wchar));
    item_t.item_data_ptr =MMIDCD_ALLOC(sizeof(GUITAB_ITEM_CONTENT_T));  
    if((str_text.wstr_ptr != PNULL) && (item_t.item_data_ptr != PNULL))
    {
        SCI_MEMSET(str_text.wstr_ptr, 0, ((GUIRICHTEXT_FILENAME_MAX_LEN)*sizeof(wchar)));    
        SCI_MEMSET(item_t.item_data_ptr, 0, sizeof(GUITAB_ITEM_CONTENT_T));
        
        item_t.item_data_ptr->image_data.image_data_type = TABITEM_DATA_ANIM_PATH;
        
        item_t.item_data_ptr->image_data.comp_image_w = MMIDCD_TAB_IMG_W;
        item_t.item_data_ptr->image_data.comp_image_h = MMIDCD_TAB_IMG_H;
        
        
        item_t.item_data_ptr->bg_image_id = IMAGE_DCD_TAB_CUR_BGD_IMAGE;
        
        item_t.item_data_ptr->text_data.text_type= TABITEM_DATA_TEXT_BUFFER;
        item_t.item_state = GUITAB_ITEMSTATE_SHOW_IMG;
        
        //SCI_TRACE_LOW:"DCD_AddTabsAndChildWindows: tab id = %d, page_num = %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2520_112_2_18_2_12_59_267,(uint8*)"dd", ctrl_id, page_num);
        for(page_index=0;page_index<page_num;page_index++)
        {   
            //需要清空buf，以避免上次数据的干扰
            SCI_MEMSET(item_t.item_data_ptr->text_data.text.text_buffer.wstr, 0x00,
                sizeof(item_t.item_data_ptr->text_data.text.text_buffer.wstr));
            //only set text etc.
            copy_len = MMIDCD_GetFeedTitle((wchar *)(item_t.item_data_ptr->text_data.text.text_buffer.wstr), GUITAB_MAX_ITEM_NAME_LENGTH, page_index);
            item_t.item_data_ptr->text_data.text.text_buffer.wstr_len = copy_len;
            GUITAB_AppendItem(ctrl_id, &item_t);
            
            if( MMIDCD_IDLEWIN_TAB_CTRL_ID==ctrl_id)
            {
                if (!GUITAB_AddChildWindow(ctrl_id,MMIDCD_IDLE_CHILD_WIN_ID,page_index))
                {
                    //SCI_TRACE_LOW:"DCD_AddTabsAndChildWindows: idle tab!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2535_112_2_18_2_12_59_268,(uint8*)"");
                }
            }
            else
            {
                if (!GUITAB_AddChildWindow(ctrl_id,MMIDCD_ITEM_CHILD_WIN_ID, page_index))
                {
                    //SCI_TRACE_LOW:"DCD_AddTabsAndChildWindows: not idle tab!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2542_112_2_18_2_12_59_269,(uint8*)"");
                }
            }
            
        }
        
        if(ctrl_id == MMIDCD_IDLEWIN_TAB_CTRL_ID)
        {
            pos = s_dcd_gui_struct.cur_idle_page_index;
        }
        else
        {
            pos = s_dcd_gui_struct.current_page_index;
        }

        GUITAB_SetCurSel(ctrl_id, pos);
        
    }

    if(PNULL != str_text.wstr_ptr )
    {
        MMIDCD_FreeMem(str_text.wstr_ptr);
    }
    if(PNULL != item_t.item_data_ptr)
    {
        MMIDCD_FreeMem(item_t.item_data_ptr);
    }
}

/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: 
//  Note:  copy for PbCreateChildWin 
/*****************************************************************************/
PUBLIC void MMIDCD_AppendChildWin(
                                  MMI_WIN_ID_T    win_id
                                  )
{
    // only use in idle win
    DCD_AddTabsAndChildWindows(MMIDCD_IDLEWIN_TAB_CTRL_ID,win_id);
}
/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: 
//  Note:  jun.hu for 修改cr00165187 更新中tab上的图标全部消失等去掉了zhuo.wang旧的函数.....
/*****************************************************************************/
PUBLIC void MMIDCD_UpdateIdleWin( void)
{

    GUITAB_DeleteAllItems(MMIDCD_IDLEWIN_TAB_CTRL_ID);
    MMK_DestroyControl(MMIDCD_IDLEWIN_TAB_CTRL_ID);

    if (MMIDCD_CreateIdleCtrl(MAIN_IDLE_WIN_ID))
    {
        DCD_AddTabsAndChildWindows(MMIDCD_IDLEWIN_TAB_CTRL_ID, MAIN_IDLE_WIN_ID);
    }

}
/*****************************************************************************/
//  Description : DCD  stop udisk ,need update~
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_StopUdiskUpdateIdleWin(void )
{
   if(MMIAPIDCD_DcdIsOpen())
   {      
       MMK_SendMsg(MAIN_IDLE_WIN_ID,MSG_DCD_TAB_UPDATE,PNULL);          
   }    
}
/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void DCD_UpdateMainWin(MMI_WIN_ID_T    win_id)
{
    MMK_CloseParentWin(win_id);
    MMK_CreateParentWin((uint32*)MMIDCD_MAIN_WIN_TAB, PNULL);
    MMK_CreateChildWin(MMIDCD_MAIN_WIN_ID, (uint32*)MMIDCD_ITEM_CHILD_WIN_TAB, PNULL);  
    MMK_SetChildWinFocus(MMIDCD_MAIN_WIN_ID, MMIDCD_ITEM_CHILD_WIN_ID); 
}

/*****************************************************************************/
//  Description : SetDcdMainWinTitleAddStr 更新dcd main win的title 的附加字符串
//  Global resource dependence: 
//  Author: jun.hu
//  Note:  2009/8/18
/*****************************************************************************/
LOCAL void SetDcdMainWinTitleAddStr(MMI_CTRL_ID_T  ctr_id)
{
    wchar wsr_temp[10] = {0};
    uint8 str_ptr[10]= {0};
    GUITAB_CAPTION_T added_string = {0};
    
    if(s_dcd_gui_struct.total_page_num > 0)
    {
        sprintf((char*)str_ptr,"%d/%d", (s_dcd_gui_struct.current_page_index + 1), s_dcd_gui_struct.total_page_num);
        MMIAPICOM_StrToWstr(str_ptr, wsr_temp);

        added_string.text_type = TABITEM_DATA_TEXT_BUFFER;
        added_string.text_style.rect.left = MMI_MAINSCREEN_RIGHT_MAX_PIXEL - (MMIDCD_TITLE_FONT_WID) * 5;
        added_string.text_style.rect.top = MMIDCD_TAB_TIP_STR_START_Y;
        added_string.text_style.rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
        added_string.text_style.rect.bottom = MMI_TAB_BOTTOM;

        added_string.text.text_buffer.wstr_len = (uint16)MIN(GUITAB_MAX_ITEM_NAME_LENGTH, MMIAPICOM_Wstrlen(wsr_temp));/*lint !e666*/
        MMIAPICOM_Wstrncpy(added_string.text.text_buffer.wstr,  wsr_temp, MMIAPICOM_Wstrlen(wsr_temp));
 
        added_string.text_style.font_space = 0;
        added_string.text_style.font_color = MMI_GRAY_WHITE_COLOR;
        added_string.text_style.font_type = MMIDCD_TITLE_FONT;
        added_string.text_style.align = ALIGN_LEFT;
        GUITAB_SetAddedString(ctr_id, added_string);
    }
}
/*****************************************************************************/
//  Description : 设置DCD动画控件......
//  Global resource dependence :
//  Author: jun.hu
//  Note:   2009/9/17 state 更新动画当前需要设定的状态:1,动画;2,静止
//          modified by wz change local to public
///*****************************************************************************/
PUBLIC void  MMIDCD_SetAnimateCtrlState(MMIDCD_ANIM_STATE_E state)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = MMIDCD_ANIMATE_UPDATING_CTRL_ID;
    data_info.img_id = IMAGE_DCD_UPDATING;
   

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update = FALSE;
    
    //SCI_TRACE_LOW:"MMIDCD_SetAnimateCtrlState state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2672_112_2_18_2_13_0_270,(uint8*)"d", state);

    switch(state)
    {
        //初始化
        case MMIDCD_ANIM_INIT:
        {
            if(MMK_IsOpenWin(MMIDCD_MAIN_WIN_ID))
            {
                GUIANIM_SetParam(&ctrl_info, &data_info,PNULL, &display_info);
            }
        }
        break;
        //播放
        case MMIDCD_ANIM_PLAY:
        {
            if(MMK_IsOpenWin(MMIDCD_MAIN_WIN_ID))
            {
                GUIANIM_ResumeAnim(MMIDCD_ANIMATE_UPDATING_CTRL_ID);
            }
        }
        break;
        //stop
        case MMIDCD_ANIM_STOP:
        {
            //SCI_TRACE_LOW:"MMIDCD SetDCDAnimateCtrlState has entered"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2697_112_2_18_2_13_0_271,(uint8*)"");
            if(MMK_IsOpenWin(MMIDCD_MAIN_WIN_ID))
            {
                GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
                GUIANIM_PauseAnim(MMIDCD_ANIMATE_UPDATING_CTRL_ID);
                //SCI_TRACE_LOW:"MMIDCD pauseanim has done"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2702_112_2_18_2_13_0_272,(uint8*)"");
            }
        }
        break;
        
        default:
            break;
        
    }
    
}
/*****************************************************************************/
//  Description : handle father tab pic dec ind 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void DCD_HandleTabDecInd(uint16 index,GUIANIM_RESULT_E result)
{
    wchar  picture_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    MMIFILE_ERROR_E file_error = SFS_NO_ERROR;
    if((result == GUIANIM_RESULT_FAIL) || (result == GUIANIM_RESULT_BAD_DATA) \
        ||(result == GUIANIM_RESULT_NO_SUPPORT) )
    {
        MMIDCDFILE_GetOnePictureFullPath(picture_full_path, 
            MMIAPIDCD_GetFeedContentInfo(index,  0, (char*)"ctxt-id"),
            MMIAPIDCD_GetFeedContentInfo(index,  0, (char*)"type"),
            MMIDCD_PICTURE_DIR_PATH
            );
        file_error = MMIAPIFMM_DeleteFile(picture_full_path,PNULL);
        if(SFS_NO_ERROR == file_error)
        {
            //SCI_TRACE_LOW:"[MMIDCD]: the invalid pic file delete ok!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2732_112_2_18_2_13_0_273,(uint8*)"");
        }
        else    
        {
            //SCI_TRACE_LOW:"[MMIDCD]: the invalid pic file do not exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2736_112_2_18_2_13_0_274,(uint8*)"");
        }          
    }
}
/*****************************************************************************/
//  Description : handle child list pic dec ind 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void DCD_HandleListDecInd(uint16 index,GUIANIM_RESULT_E result)
{
    wchar  picture_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    MMIFILE_ERROR_E file_error = SFS_NO_ERROR;
    uint16 page_index = s_dcd_gui_struct.current_page_index;
    uint16      real_item_index          = 0;
    if((result == GUIANIM_RESULT_FAIL) || (result == GUIANIM_RESULT_BAD_DATA) \
        ||(result == GUIANIM_RESULT_NO_SUPPORT) )
    {
        //get the real item index
        real_item_index = MMIDCD_GetRealItemIndex(page_index, index, TRUE);
        MMIDCDFILE_GetOnePictureFullPath(picture_full_path,
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"ctxt-id"),
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"type"),
                                        MMIDCD_PICTURE_DIR_PATH
                                        );
        file_error = MMIAPIFMM_DeleteFile(picture_full_path,PNULL);
        if(SFS_NO_ERROR == file_error)
        {
            //SCI_TRACE_LOW:"[MMIDCD]: the invalid pic file delete ok!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2763_112_2_18_2_13_0_275,(uint8*)"");
        }
        else    
        {
            //SCI_TRACE_LOW:"[MMIDCD]: the invalid pic file do not exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2767_112_2_18_2_13_0_276,(uint8*)"");
        }          
    }
}

/*****************************************************************************/
//  Description :  Father windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFatherWinMsg(
                                      MMI_WIN_ID_T        win_id, 
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM              param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static BOOLEAN  s_is_need_update = FALSE;   
    GUI_RECT_T      anim_rect = {0};
    
    anim_rect.left = MMIDCD_ANIMATION_LEFT;
    anim_rect.top = MMIDCD_ANIMATION_TOP;
    anim_rect.right = MMIDCD_ANIMATION_RIGHT;
    anim_rect.bottom = MMI_CLIENT_RECT_TOP;
    
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
        {
            GUIANIM_SetCtrlRect(MMIDCD_ANIMATE_UPDATING_CTRL_ID,&anim_rect);

            MMK_SetAtvCtrl(win_id, MMIDCD_TAB_CTRL_ID);  //set father ctrl active           
            /*update dcd page*/
            DCD_AddTabsAndChildWindows(MMIDCD_TAB_CTRL_ID,win_id);
            SetDcdMainWinTitleAddStr(MMIDCD_TAB_CTRL_ID);
            s_is_need_update = FALSE;         
            //初始化DCD更新的动画控件......
            MMIDCD_SetAnimateCtrlState(MMIDCD_ANIM_INIT);
            if(MMIDCD_IsIdle())
            {   // 只有当idle的时候才停掉它
                MMIDCD_SetAnimateCtrlState(MMIDCD_ANIM_STOP);
            }
        }
        break;
        
        case MSG_FULL_PAINT:
        {   
            SetDcdMainWinTitleAddStr(MMIDCD_TAB_CTRL_ID); //for cr162145......
        }
        break; 

        case MSG_DCD_SYNCHRONIZE_DISPLAY:
        {
            GUITAB_SetCurSel(MMIDCD_TAB_CTRL_ID, s_dcd_gui_struct.current_page_index);
            UpdateDCDList(MMIDCD_ITEM_LIST_CTRL_ID, FALSE);
        }
        break;

        case MSG_NOTIFY_TAB_SWITCH:
        {
            s_dcd_gui_struct.current_page_index = (uint16)GUITAB_GetCurSel(MMIDCD_TAB_CTRL_ID);
            s_dcd_gui_struct.current_index_in_page = 0; // update to zero
            /*优化dcd  刷list 方式*/
            MMK_PostMsg(MMIDCD_ITEM_CHILD_WIN_ID, MSG_DCD_LIST_UPDATE, PNULL,0);                    
            SetDcdMainWinTitleAddStr(MMIDCD_TAB_CTRL_ID);//added by @jun.hu, 2009/8/18
        }
        break;
        
        case MSG_NOTIFY_TAB_NEED_ITEM_CONTENT:
        {
            uint16 page_index=0;
            GUITAB_NEED_ITEM_DATA_T *data_ptr = (GUITAB_NEED_ITEM_DATA_T*)param;
            page_index = (uint16)data_ptr->item_index;
            MMIDCD_AppendOneTabPage(MMIDCD_TAB_CTRL_ID,data_ptr->point,page_index);
        }
        break;
        
        case MSG_NOTIFY_TAB_ANIM_DISPLAY_IND:
            {    
                GUITAB_ANIM_DISP_RESULT_T *data_ptr = (GUITAB_ANIM_DISP_RESULT_T*)param;
                if(MMIDCD_IsIdle())
                {
                    DCD_HandleTabDecInd((uint16)data_ptr->item_index,(GUIANIM_RESULT_E)data_ptr->result);
                }
            }
            break;

        case MSG_NOTIFY_TAB_PAINTED:
            MMK_SendMsg(MMIDCD_ANIMATE_UPDATING_CTRL_ID, MSG_CTL_PAINT, PNULL);
            break;
            
        case MSG_DCD_TAB_UPDATE:
        {
            s_is_need_update=TRUE;          
            if(MMK_IsFocusWin(MMIDCD_ITEM_CHILD_WIN_ID))
            {
                DCD_UpdateMainWin(win_id);
                s_is_need_update=FALSE;    
            }           
        }
        break;

        case MSG_GET_FOCUS:
        if(s_is_need_update)
        {
            DCD_UpdateMainWin(win_id);
            s_is_need_update=FALSE;    
        }       
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseParentWin(win_id);
        }
        break; 

        case MSG_CLOSE_WINDOW:
            {
#ifdef MMIWIDGET_SUPPORT				
		  if(MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())	
                MMK_PostMsg(WIDGET_DCD_WIN_ID, MSG_DCD_SYNCHRONIZE_DISPLAY, PNULL, 0);
		  else
#endif
                MMK_PostMsg(MMIDCD_IDLE_CHILD_WIN_ID, MSG_DCD_SYNCHRONIZE_DISPLAY, PNULL, 0);

				
            }
            break;

        default:
           recode = MMI_RESULT_FALSE;
           break;      
    }
    
    return (recode);
    
}

/*****************************************************************************/
//  Description : to set dcd list item in 1 page
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note:  modified by jun.hu 2009.12.17
/*****************************************************************************/
LOCAL void  UpdateDCDList( MMI_CTRL_ID_T list_ctrl_id, BOOLEAN need_send_paint)
{
    
    GUILIST_SetSyncDecodeAnim(list_ctrl_id, TRUE); 
    /*dcd item append*/ 
    DCD_AppendListItem(list_ctrl_id);
    
    // for cr00155551 等....
    if(need_send_paint)
    {
        if(MMK_IsFocusWin(MMIDCD_ITEM_CHILD_WIN_ID))
        {
            MMK_PostMsg(MMIDCD_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
        }      
    }

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:  2009/8/7 
/*****************************************************************************/
PUBLIC void MMIDCD_SetTabListItem(void)
{
    s_dcd_gui_struct.cur_idle_page_index = (uint16) GUITAB_GetCurSel(MMIDCD_IDLEWIN_TAB_CTRL_ID);
    s_dcd_gui_struct.cur_idle_realitem_index = 0;  
    // 优化dcd  刷list 方式
    MMK_PostMsg(MMIDCD_IDLE_CHILD_WIN_ID, MSG_DCD_LIST_UPDATE, PNULL,0);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :                                
//  Author: 
//  Note:  
/*****************************************************************************/
PUBLIC void MMIDCD_UpdateFrame(BOOLEAN is_last_update)
{
    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_UpdateFrame() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2943_112_2_18_2_13_0_277,(uint8*)"");
    
    if(!is_last_update)
    {
        MMIDCD_SetData();
    }
    
    if(MMIAPIDCD_DcdIsOpen())
    { 
        if(!is_last_update)
        {
            // 打开的不是local 的详情界面........
            if(MMK_IsOpenWin(MMIDCD_PREVIEW_WIN_ID) && (!MMK_IsOpenWin(MMIDCD_LOCAL_MAIN_WIN_ID)))
            {
               SetDataChangeFlag(TRUE);
            }
        } 
        
        MMK_SendMsg(MAIN_IDLE_WIN_ID,  MSG_DCD_TAB_UPDATE, PNULL);
        MMK_SendMsg(MMIDCD_MAIN_WIN_ID,MSG_DCD_TAB_UPDATE, PNULL);       
        
    }
    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_UpdateFrame() exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_2965_112_2_18_2_13_0_278,(uint8*)"");
}
/*****************************************************************************/
//  Description : appen one item text and anim
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2009/12/30 改造直接传给底层图片数据
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_AppendOneTabPage(
                                       MMI_CTRL_ID_T    ctrl_id,         //in:control id
                                       GUI_POINT_T      point,                                                              
                                       uint16           item_index     //in:item索引
                                       )
{
    GUITAB_IMAGE_DATA_T img_data = {0};
    wchar  picture_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    uint16 page_index=item_index;
    uint16  path_len = 0;
    BOOLEAN  show_file = FALSE;
    uint8 * data_info_ptr = PNULL;
    uint32  size = 0;

    MMIDCDFILE_GetOnePictureFullPath(picture_full_path, 
                                    MMIAPIDCD_GetFeedContentInfo(page_index,  0, (char*)"ctxt-id"),
                                    MMIAPIDCD_GetFeedContentInfo(page_index,  0, (char*)"type"),
                                    MMIDCD_PICTURE_DIR_PATH
                                    );
    path_len = MMIAPICOM_Wstrlen(picture_full_path);
    
    if(MMIAPIFMM_IsFileExist(picture_full_path, path_len))
    {
       
        img_data.image_data_type = TABITEM_DATA_ANIM_DATA;
        
        data_info_ptr = MMIDCDFILE_GetPictureInfo(picture_full_path, &size);
        if(PNULL != data_info_ptr)
        {
            img_data.image_data_u.anim_data.data_ptr = data_info_ptr;
            img_data.image_data_u.anim_data.data_size = size;
            show_file = TRUE;
            //SCI_TRACE_LOW:"MMICD will show file"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_3003_112_2_18_2_13_0_279,(uint8*)"");
        }
    }

    if(!show_file)
    {
        img_data.image_data_type = TABITEM_DATA_IMAGE_ID;
        img_data.image_data_u.anim_data.img_id = IMAGE_SECMENU_ICON_TOOL_DCD;
    }
    
    img_data.comp_image_w = MMIDCD_TAB_IMG_W;
    img_data.comp_image_h = MMIDCD_TAB_IMG_H;
    
    GUITAB_SetItemImage(ctrl_id, &img_data, point, item_index);

    if(PNULL != data_info_ptr)
    {
        MMIDCD_FreeMem(data_info_ptr);
        data_info_ptr = PNULL;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description : appen one item text and anim
//  Global resource dependence : 
//  Author: 
//  Note: 
//       /* list position*/            
//       1 animate     2 text       3 icon   4 text 
//       different with 3 icon style
/*****************************************************************************/
LOCAL BOOLEAN AppendOneListItemWithText(
                                       uint16           item_index,     //in:item索引
                                       MMI_CTRL_ID_T    ctrl_id         //in:control id
                                       )
{
    GUILIST_ITEM_DATA_T     item_data              = {0};     /*lint !e64*/
    wchar  item_title[GUILIST_STRING_MAX_NUM +1]   = {0};
    wchar  item_summary[GUILIST_STRING_MAX_NUM +1] = {0};
    
    wchar *     picture_full_name_ptr    = PNULL; 
    
    BOOLEAN   result             = FALSE;
    uint16    page_index         = s_dcd_gui_struct.current_page_index;
    uint16    name_len           = 0;
    uint16    text_len           = 0;
    uint16    real_item_index    = 0;
    
    //set softkey
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    //get the real item index
    real_item_index = MMIDCD_GetRealItemIndex(page_index, item_index,TRUE);
    
    if(0 < s_dcd_gui_struct.current_item_num)
    {
        //initialize
        picture_full_name_ptr = MMIDCD_ALLOC((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
        if(PNULL != picture_full_name_ptr)
        {       
            SCI_MEMSET(picture_full_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
            SCI_MEMSET(item_title,   0,  sizeof(item_title));
            SCI_MEMSET(item_summary, 0,  sizeof(item_summary));
            
            MMIDCDFILE_GetOnePictureFullPath(picture_full_name_ptr,
                                            MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"ctxt-id"),
                                            MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"type"),
                                            MMIDCD_PICTURE_DIR_PATH
                                            );
            name_len = MMIAPICOM_Wstrlen(picture_full_name_ptr);
            
            if(0 < MMIDCDFILE_GetPictureSize(picture_full_name_ptr))
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
            }
            else
            {
                // 先用默认的图片
                item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = IMAGE_DCD_DEFAULT_IMAGE;
            }
            
            // 第一行文字
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            text_len = MMIAPIDCD_GetEntryTitle(item_title, GUILIST_STRING_MAX_NUM, page_index, real_item_index);
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_title;
            item_data.item_content[1].item_data.text_buffer.wstr_len = text_len;
            // 第二文字
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            text_len = MMIAPIDCD_GetEntrySummary(item_summary, GUILIST_STRING_MAX_NUM, page_index, real_item_index);
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = item_summary;
            item_data.item_content[2].item_data.text_buffer.wstr_len = text_len;
            
            if(!MMIAPIDCD_GetItemAlreadyRead(page_index, real_item_index))
            {
                /*4 no read icon*///jun.hu 2009/11/25 把未读的图标作为第3个，以使它显示在文字的上面...
                item_data.item_content[3].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[3].item_data.image_id = IMAGE_DCD_NOT_READ_IMAGE;
            }
            
            result = GUILIST_SetItemData(ctrl_id,&item_data,item_index);
            
            MMIDCD_FreeMem(picture_full_name_ptr);
        }   
        
    }
    else //WHEN ITEM NUM IS ZERO
    {
        GUILIST_EMPTY_INFO_T empty_info = {0};
        MMI_STRING_T  dststring= {0};
        // 显示DCD内容为空
        MMI_GetLabelTextByLang(TXT_DCD_CONTENT_IS_EMPTY, &dststring);
        empty_info.text_buffer = dststring;
        GUILIST_SetEmptyInfo(ctrl_id, &empty_info);
    }

    return (result);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/12/30 写此函数为即将显示的TIEM附上显示图片的数据
//        以去掉之前旧的函数need_data和need_content都需要读取图片数据的做法...
/*****************************************************************************/
LOCAL BOOLEAN DrawListItemWithPicture(
                                      uint16           item_index,     //in:item索引
                                      uint16           content_index,  //in:item内容索引,text or anim etc.
                                      MMI_CTRL_ID_T    ctrl_id         //in:control id
                                      )
{
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    GUIANIM_DATA_INFO_T     anim_data    = {0};      /*lint !e64*/
    GUIRES_IMG_SRC_T        img_src      = {0};      /*lint !e64*/

    wchar *     picture_full_name_ptr    = PNULL; 
    uint8 *     data_info_ptr            = PNULL;
     
    uint16      name_len                 = 0;
    uint16      real_item_index          = 0;
    uint16      page_index               = s_dcd_gui_struct.current_page_index;
    
    BOOLEAN     result                   = FALSE;
    BOOLEAN     has_set_content          = FALSE;
    
    
    //get the real item index
    real_item_index = MMIDCD_GetRealItemIndex(page_index, item_index, TRUE);

    item_content.item_data_type = GUIITEM_DATA_ANIM_DATA;
    
    //initialize
    picture_full_name_ptr = MMIDCD_ALLOC((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
    
    if(PNULL != picture_full_name_ptr)
    {       
        SCI_MEMSET(picture_full_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));

        MMIDCDFILE_GetOnePictureFullPath(picture_full_name_ptr,
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"ctxt-id"),
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"type"),
                                        MMIDCD_PICTURE_DIR_PATH
                                        );
        name_len = MMIAPICOM_Wstrlen(picture_full_name_ptr);

        
        if(MMIAPIFMM_IsFileExist(picture_full_name_ptr, name_len))
        {
            uint32  size = 0;
            item_content.item_data.anim_data_ptr = &anim_data;
            
            data_info_ptr = MMIDCDFILE_GetPictureInfo(picture_full_name_ptr, &size);
            if(PNULL != data_info_ptr)
            {
                item_content.item_data.anim_data_ptr->data_ptr = data_info_ptr;
                item_content.item_data.anim_data_ptr->data_size = size;
                result = GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);
                has_set_content = TRUE;
                //SCI_TRACE_LOW:"MMICD will show file"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_3179_112_2_18_2_13_1_280,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD find no picture "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_3184_112_2_18_2_13_1_281,(uint8*)"");
        }

        if(PNULL != data_info_ptr)
        {
            MMIDCD_FreeMem(data_info_ptr);
            data_info_ptr = PNULL;
        }
        
        MMIDCD_FreeMem(picture_full_name_ptr);
    }   

    if(!has_set_content)
    {
        img_src.data_ptr = (uint8 *)MMI_GetLabelImage(GUIITEM_DATA_IMAGE_ID, MMIDCD_MAIN_WIN_ID, &(img_src.data_size));
        if(PNULL != img_src.data_ptr && 0 < img_src.data_size)
        {
            item_content.item_data.anim_data_ptr->data_ptr = img_src.data_ptr;
            item_content.item_data.anim_data_ptr->data_size = img_src.data_size;
            result = GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);
        }
    }

    return (result);
}

/***********************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2010/1/14 根据旧的函数改造而来,并把need_data和need_content分开处理.....
/************************************************************************************/
LOCAL BOOLEAN AppendOneLocalListItemWithText(
                                             uint16           item_index,     //in:item索引
                                             MMI_CTRL_ID_T    ctrl_id         //in:control id
                                             )
{
    GUILIST_ITEM_DATA_T      item_data = {0};  /*lint !e64*/
    MMIDCD_LOCAL_FILE_NODE_T cur_node  = {0};  /*lint !e64*/

    wchar  item_title[GUILIST_STRING_MAX_NUM +1]   = {0};
    wchar  item_summary[GUILIST_STRING_MAX_NUM +1] = {0};

    wchar *picture_full_name_ptr = PNULL; 

    BOOLEAN result               = FALSE;
    
    uint16  name_len             = 0;
    uint16  text_len             = 0;
    
    if(MMIDCDFILE_ReadLocalItemInfo(&cur_node, item_index))
    {  
        picture_full_name_ptr = MMIDCD_ALLOC((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
        
        if(PNULL != picture_full_name_ptr) 
        {   
            SCI_MEMSET(picture_full_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
            SCI_MEMSET(item_title, 0,   sizeof(item_title));
            SCI_MEMSET(item_summary, 0, sizeof(item_summary));
            
            //set softkey
            item_data.softkey_id[0] = STXT_OPTION;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
            
            MMIDCDFILE_GetOnePictureFullPath(picture_full_name_ptr,
                                            (char *)cur_node.content_text_id_str,
                                            (char *)cur_node.content_type_str,
                                            MMIDCD_LOCAL_PICTURE_DIR_PATH
                                            );
            name_len = MMIAPICOM_Wstrlen(picture_full_name_ptr);
            
            if(MMIAPIFMM_IsFileExist(picture_full_name_ptr, name_len))
            {
                //双行显示，并且是图片文件，则显示缩略图
                item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
            }
            else
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = IMAGE_DCD_DEFAULT_IMAGE;
            }
            
            //2 title
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            text_len = (uint16)MIN( GUILIST_STRING_MAX_NUM, cur_node.title_len);
            GUI_OtherCodeToWstr(item_title, GUILIST_STRING_MAX_NUM, GUI_CHAR_CODE_UCS2_BE, cur_node.title_str, text_len);

            item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_title;
            item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(item_title);
            
            //3 summary
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            text_len = (uint16)MIN( GUILIST_STRING_MAX_NUM, cur_node.summary_len);
            
            GUI_OtherCodeToWstr(item_summary, GUILIST_STRING_MAX_NUM, GUI_CHAR_CODE_UCS2_BE, cur_node.summary_str, text_len);
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = item_summary;
            item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(item_summary);
            
            result = GUILIST_SetItemData(ctrl_id,&item_data,item_index);

            MMIDCD_FreeMem(picture_full_name_ptr);
            
        }

    }
    else
    {
        // 显示DCD内容为空， jun.hu for cr152331
        GUILIST_EMPTY_INFO_T empty_info = {0};
        MMI_STRING_T  dststring= {0};
        MMI_GetLabelTextByLang(TXT_DCD_CONTENT_IS_EMPTY, &dststring);
        empty_info.text_buffer = dststring;
        GUILIST_SetEmptyInfo(ctrl_id, &empty_info);
        result = TRUE;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/12/30 写此函数为即将显示的TIEM附上显示图片的数据
//        以去掉之前旧的函数need_data和need_content都需要读取图片数据的做法...
/*****************************************************************************/
LOCAL BOOLEAN DrawLocalListItemWithPicture(
                                           uint16           item_index,     //in:item索引
                                           uint16           content_index,  //in:item内容索引,text or anim etc.
                                           MMI_CTRL_ID_T    ctrl_id         //in:control id
                                           )
{
    GUIITEM_CONTENT_T       item_content = {0};      /*lint !e64*/
    GUIANIM_DATA_INFO_T     anim_data    = {0};      /*lint !e64*/
    GUIRES_IMG_SRC_T        img_src      = {0};      /*lint !e64*/
    MMIDCD_LOCAL_FILE_NODE_T cur_node    = {0};      /*lint !e64*/

    wchar *     picture_full_name_ptr    = PNULL; 
    uint8 *     data_info_ptr            = PNULL;
     
    uint16      name_len                 = 0;
    
    BOOLEAN     result                   = FALSE;
    BOOLEAN     has_set_content          = FALSE;

    item_content.item_data_type = GUIITEM_DATA_ANIM_DATA;
    
    //initialize
    picture_full_name_ptr = MMIDCD_ALLOC((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
    
    if(PNULL != picture_full_name_ptr)
    {       
        SCI_MEMSET(picture_full_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));

        if(MMIDCDFILE_ReadLocalItemInfo(&cur_node, item_index))
        {
            MMIDCDFILE_GetOnePictureFullPath(picture_full_name_ptr,
                                            (char *)cur_node.content_text_id_str,
                                            (char *)cur_node.content_type_str,
                                            MMIDCD_LOCAL_PICTURE_DIR_PATH
                                            );
            name_len = MMIAPICOM_Wstrlen(picture_full_name_ptr);

            if(MMIAPIFMM_IsFileExist(picture_full_name_ptr, name_len))
            {
                uint32  size = 0;
                item_content.item_data.anim_data_ptr = &anim_data;
                
                data_info_ptr = MMIDCDFILE_GetPictureInfo(picture_full_name_ptr, &size);
                if(PNULL != data_info_ptr)
                {
                    item_content.item_data.anim_data_ptr->data_ptr = data_info_ptr;
                    item_content.item_data.anim_data_ptr->data_size = size;
                    result = GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);
                    has_set_content = TRUE;
                    //SCI_TRACE_LOW:"MMICD will show file"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_3355_112_2_18_2_13_1_282,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDCD find no picture "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_3360_112_2_18_2_13_1_283,(uint8*)"");
            }
            
            if(PNULL != data_info_ptr)
            {
                MMIDCD_FreeMem(data_info_ptr);
                data_info_ptr = PNULL;
            }
            
        }

        MMIDCD_FreeMem(picture_full_name_ptr);
        
    }   

    if(!has_set_content)
    {
        img_src.data_ptr = (uint8 *)MMI_GetLabelImage(GUIITEM_DATA_IMAGE_ID, MMIDCD_LOCAL_MAIN_WIN_ID, &(img_src.data_size));
        if(PNULL != img_src.data_ptr && 0 < img_src.data_size)
        {
            item_content.item_data.anim_data_ptr->data_ptr = img_src.data_ptr;
            item_content.item_data.anim_data_ptr->data_size = img_src.data_size;
            result = GUILIST_SetItemContent(ctrl_id, &item_content, item_index, content_index);
        }
    }

    return (result);
}
/*****************************************************************************/
//  Description : DCD_QueryWinMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DCD_QueryWinMsg(
                                    MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param
                                    )
{
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:        
            MMIDCD_UpdateStart(HTTP_REQ_TYPE_MAN);
            MMK_CloseWin(win_id);
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        
        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : to handle dialed tab child window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChildWindow(
                                      MMI_WIN_ID_T      win_id, 
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM       param
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    MMI_CTRL_ID_T   list_ctrl_id = MMIDCD_ITEM_LIST_CTRL_ID;
    uint16          item_index = 0;
    uint16          item_content_index = 0;
    GUI_RECT_T      list_rect = {0};
   
    static BOOLEAN is_need_update = FALSE;  

    switch (msg_id)
    {
       case MSG_OPEN_WINDOW:
           {
               list_rect.left   = 0;
               list_rect.top    = MMI_TAB_CHILD_WINDOW_TOP;
               list_rect.right  = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
               list_rect.bottom = MMI_CLIENT_RECT_BOTTOM;
               GUILIST_SetRect(list_ctrl_id,&list_rect);
               
               GUILIST_SetOwnSofterKey(list_ctrl_id,TRUE);
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
               is_need_update = FALSE;
               MMK_SetAtvCtrl(win_id, list_ctrl_id);       
               GUILIST_SetDefaultIcon(list_ctrl_id, IMAGE_DCD_DEFAULT_IMAGE, IMAGE_DCD_DEFAULT_IMAGE);
               s_dcd_gui_struct.current_item_num = MMIDCD_GetFormalEntryNumInFeed(s_dcd_gui_struct.current_page_index); 
               UpdateDCDList(list_ctrl_id, FALSE);
           }
        break;
        
      case MSG_FULL_PAINT:
        {  // for cr00157115
            uint16 item_num = GUILIST_GetTotalItemNum(list_ctrl_id);
            if(0 == item_num)
            {  
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            }
        }
        break;
        
      case MSG_CTL_TAB_SWITCH:
        //清除掉原来的标记
        HandleDcdListUnMarkOperation(list_ctrl_id, DCD_UNMARK_ALL);
        break;
        
      case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            //add text into list item
            GUILIST_NEED_ITEM_CONTENT_T * need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if( PNULL != need_item_content_ptr )
            {
                item_index = need_item_content_ptr->item_index;
                AppendOneListItemWithText(item_index, list_ctrl_id);
            }
        }
        break;


      case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {            
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if( PNULL != need_item_content_ptr )
            {
                item_index = need_item_content_ptr->item_index;
                item_content_index = need_item_content_ptr->item_content_index;
                DrawListItemWithPicture(item_index,item_content_index,list_ctrl_id);
            }
        }
        break;           


      case MSG_CTL_LIST_MARK_ITEM:
        {
            GUILIST_MARK_ITEM_T* list_param_ptr = (GUILIST_MARK_ITEM_T *)param;
            
            if(PNULL != list_param_ptr)
            {
                if (list_param_ptr->had_maked)
                {
                    HandleDcdListMarkOperation(list_ctrl_id, DCD_MARK_CURONE);
                }
                else
                {
                    HandleDcdListUnMarkOperation(list_ctrl_id, DCD_UNMARK_CURONE);
                }  
            }
        }
        break;
        
      case MSG_DCD_LIST_UPDATE:
        {
            UpdateDCDList(list_ctrl_id, TRUE);
        }
        break;

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        if(win_id == MMIDCD_ITEM_CHILD_WIN_ID)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif
        break;
        
      case MSG_APP_OK:
      case MSG_CTL_OK:
      case MSG_APP_MENU:
        s_dcd_gui_struct.current_index_in_page=GUILIST_GetCurItemIndex(list_ctrl_id);
        if(win_id == MMIDCD_ITEM_CHILD_WIN_ID)
        {
            MMK_CreateWin((uint32 *)MMIDCD_ITEM_MENU_WIN_TAB, (ADD_DATA)list_ctrl_id);   
        }
        break;    
      case MSG_NOTIFY_LIST_ANIM_DISPLAY_IND:
          {    
              GUILIST_ANIM_DISP_RESULT_T *data_ptr = (GUILIST_ANIM_DISP_RESULT_T*)param;
              if(MMIDCD_IsIdle())
              {
                  DCD_HandleListDecInd((uint16)data_ptr->item_index, (GUIANIM_RESULT_E)data_ptr->result);
              }
          }
          break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_MIDSK:
      case MSG_APP_WEB:
        {
            s_dcd_gui_struct.current_index_in_page = GUILIST_GetCurItemIndex(list_ctrl_id);
            // jun.hu 2009/11/9 for cr00160308 , 当删除某一频道所有的频道项后，再去点更新，如果更新失败
            // 它还是会调用MMIDCD_SetData,把s_dcd_gui_struct.current_item_num置为非0,导致可以进详情，出现错误的界面
            s_dcd_gui_struct.current_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(s_dcd_gui_struct.current_page_index, TRUE);
            if(0 < s_dcd_gui_struct.current_item_num)
            {
                if(!IsUDiskRunning())
                {
                    MMK_CreateWin((uint32*)MMIDCD_PREVIEW_WIN_TAB, (ADD_DATA)list_ctrl_id);
                }
            }
        }
        break;
        
      case MSG_APP_CANCEL:
      case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMIDCD_MAIN_WIN_ID);
        MMK_CloseWin(win_id);
        break;

      default:
        recode = MMI_RESULT_FALSE;
        break;
    }

   return recode;
}
/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DCD_SetAnimParam(
                               MMI_CTRL_ID_T    ctrl_id,
                               uint16           index,
                               BOOLEAN          is_update  //是否立即刷新             
                               )
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    wchar  picturn_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    
    uint16  page_index    = s_dcd_gui_struct.cur_idle_page_index;  
    uint16  full_path_len = 0;
    uint16  useable_item_num = 0;
    
    MMIDCDFILE_GetOnePictureFullPath(picturn_full_path,
                                    MMIAPIDCD_GetEntryContentInfo(page_index, index, 0, (char*)"ctxt-id"),
                                    MMIAPIDCD_GetEntryContentInfo(page_index, index, 0, (char*)"type"),
                                    MMIDCD_PICTURE_DIR_PATH
                                    );
    full_path_len = MMIAPICOM_Wstrlen(picturn_full_path);
    
    GUIANIM_SetDefaultIcon(ctrl_id,IMAGE_DCD_DEFAULT_IMAGE,IMAGE_DCD_DEFAULT_IMAGE);
    
    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_syn_decode = TRUE;
    display_info.is_update = is_update;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

    useable_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, MMIAPIDCD_GeSettingInfo()->is_show_readed);      
    if((0 < useable_item_num ) && MMIAPIFMM_IsFileExist(picturn_full_path,full_path_len))
    {
        anim_info.full_path_wstr_ptr = picturn_full_path;
        anim_info.full_path_wstr_len = full_path_len;
        
        anim_result = GUIANIM_SetParam(&control_info,PNULL,&anim_info,&display_info);
    }
    else
    {
        data_info.img_id = IMAGE_DCD_DEFAULT_IMAGE;         
        
        anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    }
    
    if ((GUIANIM_RESULT_SUCC == anim_result) ||
        (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set text parameter
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void DCD_SetTextParam(
                            MMI_CTRL_ID_T    ctrl_id,
                            uint16           index,
                            BOOLEAN          is_update  //是否立即刷新             
                            )
{
    MMI_STRING_T    content_str = {0};
    const wchar     test_str[] = {L"\n"};
    uint16          test_len=0;
    uint16          title_len=0;
    uint16          wlen=0;
    uint16          page_index = s_dcd_gui_struct.cur_idle_page_index;  
    uint32          timer = MMIAPIDCD_GeSettingInfo()->scroll_time * SECOND_PER_MICROSECOND_POWER;
    GUI_COLOR_T     font_color = 0;
    GUI_FONT_T      font=MMI_DCD_IDLE_TEXT_FONT;
    GUI_BG_T        dcd_bg = {0};
    wchar           *full_content_ptr = PNULL;

    full_content_ptr = SCI_ALLOCAZ(MMISMS_MAX_MESSAGE_LEN * sizeof(wchar));

    if (PNULL == full_content_ptr)
    {
        return;
    }
    
    //set text to content_str
    if (0 == MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, TRUE))   
    {
        //快讯内容为空
        MMI_GetLabelTextByLang(TXT_DCD_CONTENT_IS_EMPTY, &content_str);
        font_color = MMI_WHITE_COLOR;
    }
    else if(0 == MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, MMIAPIDCD_GeSettingInfo()->is_show_readed))   
    {            
        //没有未读条目
        MMI_GetLabelTextByLang(TXT_DCD_BLANK, &content_str);
        font_color = MMI_WHITE_COLOR;
    }
    else    
    {
        title_len =MMIAPIDCD_GetEntryTitle(full_content_ptr, MMISMS_MAX_MESSAGE_LEN, page_index, index);
        
        title_len = MIN(title_len,MMISMS_MAX_MESSAGE_LEN);
        
        test_len= MMIAPICOM_Wstrlen(test_str);
        MMI_WSTRNCPY(full_content_ptr+title_len, MMISMS_MAX_MESSAGE_LEN,test_str,test_len,test_len);
        
        if((title_len+test_len) < MMISMS_MAX_MESSAGE_LEN )
        {
            wlen = (uint16)(MMISMS_MAX_MESSAGE_LEN - title_len-test_len);   
            MMIAPIDCD_GetEntrySummary(full_content_ptr + title_len+test_len, wlen, page_index, index);
        }
        if(!MMIAPIDCD_GetItemAlreadyRead(page_index, index))
        {   
            font_color = MMIDCD_NO_READ_COLOR;
        }       
        else
        {
            font_color = MMI_WHITE_COLOR;
        }
        
        //set text to content_str
        content_str.wstr_ptr = full_content_ptr;
        content_str.wstr_len= (uint16)MMIAPICOM_Wstrlen(full_content_ptr);
    }
    
    //set text color and font
    GUITEXT_SetFont(ctrl_id,&font,&font_color);
    
    //set bg
    dcd_bg.bg_type = GUI_BG_IMG;
    dcd_bg.img_id  = IMAGE_COMMON_BG;
    GUITEXT_SetBg(ctrl_id,&dcd_bg);
    
    //text not handle tp msg
    GUITEXT_SetHandleTpMsg(FALSE,ctrl_id);
    //set text no progress
    GUITEXT_IsDisplayPrg(FALSE,ctrl_id);
    
    //set text auto scroll 
    GUITEXT_SetAutoScroll(TRUE,&timer,ctrl_id);
    GUITEXT_SetCircularHandle(FALSE,ctrl_id);
    
    //set text
    GUITEXT_SetString(ctrl_id,content_str.wstr_ptr,content_str.wstr_len,is_update);  

    SCI_FREE(full_content_ptr);
}

/*****************************************************************************/
//  Description : to handle dialed tab child window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  DCD_UpdateIdleItem(MMI_WIN_ID_T   win_id,
                               uint16         item_index,
                               BOOLEAN        is_swtich_tab,
                               BOOLEAN        is_need_update
                               )
{      
    BOOLEAN is_update = FALSE;
    uint16  pageindex = s_dcd_gui_struct.cur_idle_page_index;
    uint16  show_item_index = 0;
    BOOLEAN show_result = FALSE;
    
    if(is_need_update)
    {
        is_update = MMK_IsFocusWin(win_id);
    }   
    
    if(MMIAPIDCD_GetUseableItemFromIndex(pageindex, item_index, &show_item_index))
    {
        s_dcd_gui_struct.cur_idle_realitem_index = show_item_index;
        DCD_SetAnimParam(MMIDCD_IDLEWIN_ANIM_CTRL_ID, show_item_index, is_update);
        DCD_SetTextParam(MMIDCD_IDLEWIN_TEXT_CTRL_ID, show_item_index, is_update);
        show_result = TRUE;
    }
    
    if(!show_result)
    {    
        if(is_swtich_tab)                   
        {
            GUITAB_SwitchToNextItem(MMIDCD_IDLEWIN_TAB_CTRL_ID, GUITAB_SWITCH_NEXT);          
        }
        else
        {
            if(MMIAPIDCD_GetUseableItemFromIndex(pageindex, 0, &show_item_index))
            {
                s_dcd_gui_struct.cur_idle_realitem_index = show_item_index;
            }
            
            // 不管有没有找到可用的item ,都要调用下面的这个函数来显示，如果没找到，则显示内容为空....
            DCD_SetAnimParam(MMIDCD_IDLEWIN_ANIM_CTRL_ID, show_item_index, is_update);
            DCD_SetTextParam(MMIDCD_IDLEWIN_TEXT_CTRL_ID, show_item_index, is_update);
        }
    }
}

/*****************************************************************************/
//  Description : to handle dialed tab child window message
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIdleChildWindow(
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM       param
                                          )
{



    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  text_ctrl_id = MMIDCD_IDLEWIN_TEXT_CTRL_ID;  
    
    const GUI_LCD_DEV_INFO  *lcd_dev_ptr = MMITHEME_GetDefaultLcdDev();
    
    GUI_RECT_T      win_rect = {0, MMI_DCD_IDLEWIN_LISTBOX_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_DCD_IDLEWIN_LISTBOX_BOTTOM};
    GUI_RECT_T      text_rect = {MMI_DCD_IDLEWIN_TEXT_LEFT,MMI_DCD_IDLEWIN_TEXT_TOP,MMI_DCD_IDLEWIN_TEXT_RIGHT,MMI_DCD_IDLEWIN_TEXT_BOTTOM};
    GUI_RECT_T      anim_rect = {MMI_DCD_IDLEWIN_ANIM_LEFT,MMI_DCD_IDLEWIN_ANIM_TOP,MMI_DCD_IDLEWIN_ANIM_RIGHT,MMI_DCD_IDLEWIN_ANIM_BOTTOM};
    #ifdef MMIWIDGET_SUPPORT
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())//JONATHAN
        {
        		return MMI_RESULT_FALSE;
        }
    #endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIANIM_SetCtrlRect(MMIDCD_IDLEWIN_ANIM_CTRL_ID,&anim_rect);

        GUITEXT_SetRect(text_ctrl_id,&text_rect);

        DCD_UpdateIdleItem(win_id, s_dcd_gui_struct.current_real_index_in_page,FALSE,FALSE);
        MMK_SetAtvCtrl(win_id, text_ctrl_id);   
        break;
        
    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL, &win_rect, &win_rect,win_id, IMAGE_COMMON_BG, lcd_dev_ptr);
        break;
        
    case MSG_NOTIFY_TEXT_END_LINE:
        {   
            is_text_end = TRUE;//@CR237614 2011.05.31
            
            s_dcd_gui_struct.cur_idle_realitem_index++;
            if(MMIDCD_MAX_ITEM_NUM_PER_PAGE < s_dcd_gui_struct.cur_idle_realitem_index)
            {
                s_dcd_gui_struct.cur_idle_realitem_index = 0;
            }
            DCD_UpdateIdleItem(win_id, s_dcd_gui_struct.cur_idle_realitem_index, TRUE, TRUE);
        }
        break;
        
    case MSG_DCD_LIST_UPDATE:
        {
            DCD_UpdateIdleItem(win_id, s_dcd_gui_struct.cur_idle_realitem_index, FALSE, TRUE);         
        }
        break;

    // for cr169538, 返回idle的时候，先显示了DCD的text, 再显示了idle的其他内容,所以最后一个参数置为FALSE...
    case MSG_DCD_SYNCHRONIZE_DISPLAY:    
        {
            DCD_UpdateIdleItem(win_id, s_dcd_gui_struct.cur_idle_realitem_index, FALSE, FALSE);         
        }
        break;   

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:         
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIAPIDCD_ExternalOpenWin(FALSE);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
}
/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(
                          MMI_CTRL_ID_T    ctrl_id,
                          MMI_TEXT_ID_T    text_id
                          )
{
    GUILIST_ITEM_T          item_t     = {0};  /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data  = {0};  /*lint !e64*/
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;

    
    GUILIST_AppendItem(ctrl_id, &item_t);

    return;
}
/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void LoadSelectListData(
                              MMI_WIN_ID_T     win_id, 
                              MMI_CTRL_ID_T    ctrl_id,
                              const MMIDCD_SETTING_SELECT_TYPE_E s_select_type
                              )
{
    uint16 i = 0, cur_index = 0;
    MMI_STRING_T title_sting = {0};
    // 快讯服务开关
    const MMI_TEXT_ID_T server_switch_txt[MMIDCDSET_DCD_SWITCH_MAX] = {TXT_DCD_TURN_ON, TXT_DCD_TURN_OFF };
    // 连接模式
    const MMI_TEXT_ID_T connet_mode_txt[NUM_OF_MMIDCD_CONNECT_MODE] = {TXT_DCD_CONNECT_MODE_ALWAYS, TXT_DCD_CONNECT_MODE_MAN, TXT_DCD_CONNECT_MODE_OFF_WHEN_CRUISE}; 
    // 滚动速度
    const MMI_TEXT_ID_T scroll_speed_txt[NUM_OF_MMIDCD_SCROLL_TIME] = {TXT_COMMON_FAST, TXT_DCD_SCROLL_TIME_MIDDLE, TXT_COMMON_SLOW};
    // 显示已阅读条目
    const MMI_TEXT_ID_T show_read_txt[MMIDCDSET_DCD_SHOWREAD_MAX]= {TXT_DCD_SHOW_READED_OPEN, TXT_HIDE};
        // 显示SIM 卡切换
    const MMI_TEXT_ID_T show_sim_txt[MMIDCDSET_DCD_SIMSWITCH_MAX]= {TXT_SIM_SEL_SIM1, TXT_SIM_SEL_SIM2};

    // 存储位置
    //const MMI_TEXT_ID_T memory_txt[MMIDCD_MEMORY_TYPE_MAX] = {TXT_DCD_STORE_ON_PHONE, TXT_COMMON_SD_CARD};
    
    
    MMK_SetAtvCtrl(win_id, ctrl_id);
    switch(s_select_type)
    {     // 快讯服务开关
        case MMIDCD_SETTING_SELECT_DCD_OPEN:
        {
            GUILIST_SetMaxItem(ctrl_id, NUM_OF_MMIDCD_SCROLL_TIME,FALSE);
            for(i = 0; i < MMIDCDSET_DCD_SWITCH_MAX; i ++)
            {
                AppendListItem(ctrl_id, server_switch_txt[i]);
            }
            
            cur_index = GetDCDSwitchIndex();   
            MMI_GetLabelTextByLang(TXT_DCD_SERVICE_SWITCH, &title_sting);        
            GUIWIN_SetTitleText(win_id, title_sting.wstr_ptr, title_sting.wstr_len, FALSE);
        }
        break;
        // 连接模式
        case MMIDCD_SETTING_SELECT_CONNECT_MODE:
        {
            GUILIST_SetMaxItem(MMIDCD_SETTING_LIST_CTRL_ID, NUM_OF_MMIDCD_CONNECT_MODE,FALSE);
            for(i = 0; i < NUM_OF_MMIDCD_CONNECT_MODE; i ++)
            {
                AppendListItem(ctrl_id, connet_mode_txt[i]);
            }
            
            cur_index = GetDCDConnectModeIndex();
            MMI_GetLabelTextByLang(TXT_DCD_CONNECT_MODE, &title_sting);        
            GUIWIN_SetTitleText(win_id, title_sting.wstr_ptr, title_sting.wstr_len, FALSE);
        }
        break;
        // 滚动速度
        case MMIDCD_SETTING_SELECT_SCROLL_TIME:
        {
            GUILIST_SetMaxItem(MMIDCD_SETTING_LIST_CTRL_ID, NUM_OF_MMIDCD_SCROLL_TIME,FALSE);
            for(i = 0; i < NUM_OF_MMIDCD_SCROLL_TIME; i ++)
            {
                AppendListItem(ctrl_id, scroll_speed_txt[i]);
            }
            cur_index = GetDCDScrollSpeedIndex();
            
            MMI_GetLabelTextByLang(TXT_DCD_SCROLL_TIME, &title_sting);        
            GUIWIN_SetTitleText(win_id, title_sting.wstr_ptr, title_sting.wstr_len, FALSE);
        }
        break;
        // 显示已阅读条目
        case MMIDCD_SETTING_SELECT_SHOW_READED:
        {
            GUILIST_SetMaxItem(MMIDCD_SETTING_LIST_CTRL_ID, MMIDCDSET_DCD_SHOWREAD_MAX,FALSE);
            for(i = 0; i < MMIDCDSET_DCD_SHOWREAD_MAX; i ++)
            {
                AppendListItem(ctrl_id, show_read_txt[i]);
            }
            cur_index = GetDCDShowReadIndex();
            
            MMI_GetLabelTextByLang(TXT_DCD_SHOW_READED, &title_sting);        
            GUIWIN_SetTitleText(win_id, title_sting.wstr_ptr, title_sting.wstr_len, FALSE);
        }
        break;

        // 显示已阅读条目
        case MMIDCD_SETTING_SELECT_SIM_INTERET:
        {
            GUILIST_SetMaxItem(MMIDCD_SETTING_LIST_CTRL_ID, MMIDCDSET_DCD_SIMSWITCH_MAX,FALSE);
            for(i = 0; i < MMIDCDSET_DCD_SIMSWITCH_MAX; i ++)
            {
                AppendListItem(ctrl_id, show_sim_txt[i]);
            }
            cur_index = GetDCDSimSelectedIndex();
            
            MMI_GetLabelTextByLang(TXT_DUAL_SIM_SWITCH, &title_sting);        
            GUIWIN_SetTitleText(win_id, title_sting.wstr_ptr, title_sting.wstr_len, FALSE);
        }
        break;        
    default:
        break;
    }
    
    GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
    
    return;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void SaveDCDSettings(const MMIDCD_SETTING_SELECT_TYPE_E s_select_type,
                           const uint16 selectIndex)
{
  switch(s_select_type)
  {   // 快讯服务开关
      case MMIDCD_SETTING_SELECT_DCD_OPEN:
      {
          if (0 == selectIndex)
          {         
              MMIAPIDCD_GeSettingInfo()->is_dcd_open= TRUE;
          }
          else
          {
              MMIAPIDCD_GeSettingInfo()->is_dcd_open = FALSE;
          }
      }
      break;
      
      // 连接模式
      case MMIDCD_SETTING_SELECT_CONNECT_MODE:
      {
         switch(selectIndex)
         {
             case 0:
                 if(MMIAPIDCD_GeSettingInfo()->connect_mode != MMIDCD_CONNECT_MODE_ALWAYS)
                 {
                     MMIAPIDCD_GeSettingInfo()->connect_mode = MMIDCD_CONNECT_MODE_ALWAYS;
                     MMIDCD_UpdateStart(HTTP_REQ_TYPE_MAN);   // 模式切换发起更新 
                 }               
                 MMIAPIDCD_GeSettingInfo()->is_auto_update = TRUE;                              
                 break;

             case 1:
                 {
                     if(MMIAPIDCD_GeSettingInfo()->connect_mode != MMIDCD_CONNECT_MODE_NEVER)                  
                     {
                         MMIAPIDCD_GeSettingInfo()->connect_mode = MMIDCD_CONNECT_MODE_NEVER;
                         //   DCD_MMIUpdateStart(HTTP_REQ_TYPE_MAN);   // 模式切换发起更新              
                     }               
                     MMIAPIDCD_GeSettingInfo()->is_auto_update = FALSE;                 
                 }                 
                 break;

             case 2:
                 if(MMIAPIDCD_GeSettingInfo()->connect_mode != MMIDCD_CONNECT_MODE_OFF_WHEN_CRUISE)
                 {
                     MMIAPIDCD_GeSettingInfo()->connect_mode = MMIDCD_CONNECT_MODE_OFF_WHEN_CRUISE;
                     MMIDCD_UpdateStart(HTTP_REQ_TYPE_MAN);   // 模式切换发起更新                
                 }       
                 MMIAPIDCD_GeSettingInfo()->is_auto_update = TRUE;                                              
                 break;

             default:
                 break;
         }
      }
      break;
      // 滚动速度
      case MMIDCD_SETTING_SELECT_SCROLL_TIME:
      {
        switch(selectIndex)
        {
            case 0:
                MMIAPIDCD_GeSettingInfo()->scroll_time = MMIDCD_SCROLL_TIME_FAST;
                break;
            case 1:
                MMIAPIDCD_GeSettingInfo()->scroll_time = MMIDCD_SCROLL_TIME_MIDDLE;
                break;
            case 2:
                MMIAPIDCD_GeSettingInfo()->scroll_time = MMIDCD_SCROLL_TIME_SLOW;
                break;
            default:
                break;
        }
      }
      break;
      // 显示已阅读条目
      case MMIDCD_SETTING_SELECT_SHOW_READED:
      {
         if(0 == selectIndex)
         {
           MMIAPIDCD_GeSettingInfo()->is_show_readed = TRUE;
         }
         else
         {
           MMIAPIDCD_GeSettingInfo()->is_show_readed = FALSE;
         }
#ifdef MMIWIDGET_SUPPORT         
         /*update main list win*/
	  if(MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())	
                MMK_SendMsg(WIDGET_DCD_WIN_ID, MSG_DCD_LIST_UPDATE, PNULL);
	  else
#endif
         	  MMK_SendMsg(MMIDCD_IDLE_CHILD_WIN_ID, MSG_DCD_LIST_UPDATE, PNULL);
		 
         MMK_SendMsg(MMIDCD_ITEM_CHILD_WIN_ID, MSG_DCD_LIST_UPDATE, PNULL);
      }
      break;

       // 显示Sim 卡切换
      case MMIDCD_SETTING_SELECT_SIM_INTERET:
      {
          MMIAPIDCD_GeSettingInfo()->sim_slected = (MN_DUAL_SYS_E)selectIndex;
      }
      break;      
      default:
        break;
  }
  
  MMIAPIDCD_WriteSettingToNv();
}
/*****************************************************************************/
//  Description : inform this function finished 
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_SetResetFinishedInd(void)
{ 
    if(MMK_IsOpenWin( MMIDCD_STOP_UPDATING_WAITING_WIN_ID))
    {
        //SCI_TRACE_LOW:"[MMIDCD] MMIDCDAPISET_FuncFinishedInd; MMIDCD_STOP_UPDATING_WAITING_WIN_ID is open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4134_112_2_18_2_13_2_284,(uint8*)"");
        MMK_PostMsg(MMIDCD_STOP_UPDATING_WAITING_WIN_ID, MSG_DCD_UPDATE_FINISH, PNULL, 0);
    }
    else if(MMK_IsOpenWin( MMIDCD_RESET_FACTORY_WAITING_WIN_ID))
    {
        //SCI_TRACE_LOW:"[MMIDCD] MMIDCDAPISET_FuncFinishedInd; MMIDCD_RESET_FACTORY_WAITING_WIN_ID is open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4139_112_2_18_2_13_2_285,(uint8*)"");
        MMK_PostMsg(MMIDCD_RESET_FACTORY_WAITING_WIN_ID, MSG_SET_RESET_FACTORY_OVER_IND, PNULL, 0);
    }

}

/*****************************************************************************/
//  Description : 取消更新的等待窗口
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDStopUpdateDataWaitWin(
                                                  MMI_WIN_ID_T       win_id,
                                                  MMI_MESSAGE_ID_E   msg_id, 
                                                  DPARAM             param 
                                                  )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_DCD_UPDATE_FINISH:
        //SCI_TRACE_LOW:"[MMIDCD]: MSG_DCD_UPDATE_FINISH is come!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4160_112_2_18_2_13_3_286,(uint8*)"");
        MMK_CloseWin(win_id);
        break;      
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}
/*****************************************************************************/
//  Description : 当发生空间不够时
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 写此函数为的是打开窗口时不点亮屏幕,故重写MSG_OPEN_WINDOW函数....
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNotEnoughtSpaceWin(
                                            MMI_WIN_ID_T       win_id, 
                                            MMI_MESSAGE_ID_E   msg_id, 
                                            DPARAM             param   
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
      case MSG_OPEN_WINDOW:
          GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
          break;
        
      default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}
/*****************************************************************************/
//  Description : dcd 恢复出厂设置等待窗口
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 写此函数是不管DCD是否在更新中都起等待窗口，因为DCD有删除图片文件夹的动作，
//        以避免用户较长时间按键没反应的现象..
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDResetFactoryWaitWin(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param   
                                                )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_DCD_RESET_START:
            // 如果当前没有处在更新中，则发起动作； 如果在更新中，则发起取消更新的请求,
            // 收到取消成功的消息后,再发起动作
            if(MMIDCD_IsIdle())
            {
                MMIDCD_RestoreFactorySetting();
                MMK_PostMsg(win_id, MSG_DCD_RESET_FINISH, PNULL, 0);
            }
            else
            {
                DCD_Sendsig(DCD_SIG_DL_CANCLE_REQ, HTTP_REQ_TYPE_NULL);
            }
            break;

        // 只有在更新中发起的恢复出厂设置请求才会走到这里来......
        case MSG_SET_RESET_FACTORY_OVER_IND:
            MMIDCD_RestoreFactorySetting();
            MMK_PostMsg(win_id, MSG_DCD_RESET_FINISH, PNULL, 0);
            break;

        case MSG_DCD_RESET_FINISH:
            MMK_CloseWin(win_id);          
            MMK_CloseWin(MMIDCD_SETTING_MENU_WIN_ID);
            MMK_CloseWin(MMIDCD_SETTING_WIN_ID);
            MMK_CloseWin(MMIDCD_ITEM_MENU_WIN_ID);
            MMIPUB_OpenAlertSuccessWin( TXT_COMPLETE);
            break;

        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }

    return recode;  
}
/*****************************************************************************/
//  Description : 进行DCD设置主菜单的消息处理函数
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/16
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDSettingMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_MENU_GROUP_ID_T   group_id    =   0;
    MMI_MENU_ID_T         menu_id     =   0;
    MMI_RESULT_E          recode      =   MMI_RESULT_TRUE; 
    MMI_WIN_ID_T         query_win_id =   MMIDCD_QUERY_WIN_ID;
    uint32               time_out     =   0;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                MMK_SetAtvCtrl(win_id, MMIDCD_SETTING_MENU_CTRL_ID);
            }
            break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                GUIMENU_GetId(MMIDCD_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
                switch (menu_id)
                {
                   case ID_DCD_ITEM_SETTING_DCD_OPEN_SWITCH:
                        MMK_CreateWin((uint32*)MMIDCD_SETTING_WIN_TAB, (ADD_DATA)MMIDCD_SETTING_SELECT_DCD_OPEN);
                        break;
                    
                    case ID_DCD_ITEM_SETTING_CONNECT_MODE:
                        MMK_CreateWin((uint32*)MMIDCD_SETTING_WIN_TAB, (ADD_DATA)MMIDCD_SETTING_SELECT_CONNECT_MODE);
                        break;
                    
                    case ID_DCD_ITEM_SETTING_SCROLL_TIME:
                        MMK_CreateWin((uint32*)MMIDCD_SETTING_WIN_TAB, (ADD_DATA)MMIDCD_SETTING_SELECT_SCROLL_TIME);           
                        break;
                    
                    case ID_DCD_ITEM_SETTING_SHOW_READED:
                        MMK_CreateWin((uint32*)MMIDCD_SETTING_WIN_TAB, (ADD_DATA)MMIDCD_SETTING_SELECT_SHOW_READED);         
                        break;          
                    case ID_DCD_ITEM_SETTING_MEMORY:
                        MMK_CreateWin((uint32*)MMIDCD_SETTING_WIN_TAB, (ADD_DATA)MMIDCD_SETTING_SELECT_MEMORY);
                        break;
                   case ID_DCD_ITEM_SETTING_SIM_INTERET:
                        if(MMIDCD_IsIdle())
                        {
                           MMK_CreateWin((uint32*)MMIDCD_SETTING_WIN_TAB, (ADD_DATA)MMIDCD_SETTING_SELECT_SIM_INTERET);
                        }
                        else
                        {
                          MMIPUB_OpenAlertWarningWin(TXT_DCD_UPDATING);
                        }
                         break;                        
                 
                    case ID_DCD_ITEM_SETTING_RECORY_FACRORTY:
                        {
                            MMIPUB_OpenQueryWinByTextId(TXT_DCD_SET_RESET_FACTORY, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
            
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;
            
        case MSG_PROMPTWIN_OK:
            {
                MMIPUB_CloseQuerytWin(&query_win_id);
                GUIMENU_GetId(MMIDCD_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
                if(ID_DCD_ITEM_SETTING_RECORY_FACRORTY == menu_id)
                {  
                    if(!IsUDiskRunning())                  
                    {
                        MMI_WIN_ID_T wait_win_id = MMIDCD_RESET_FACTORY_WAITING_WIN_ID;
                        
                        //进入等待界面
                        MMIPUB_OpenAlertWinByTextId(&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL,MMIPUB_SOFTKEY_NONE, HandleDCDResetFactoryWaitWin);
                        MMK_PostMsg(wait_win_id, MSG_DCD_RESET_START, PNULL, 0);
                        
                    }
                }
            }
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
        }
    
    return recode;  



}
/*****************************************************************************/
//  Description : 进行DCD设置（单项）的消息处理函数
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCDSettingWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static MMIDCD_SETTING_SELECT_TYPE_E s_select_type = 0;      /*lint !e64*/
    MMI_CTRL_ID_T   active_ctrl_id = MMIDCD_SETTING_LIST_CTRL_ID;
    uint16 index = 0;
    MMI_WIN_ID_T query_win_id = MMIDCD_QUERY_WIN_ID;
    s_select_type = (uint32)MMK_GetWinAddDataPtr(win_id);   /*lint !e64*/
    

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            LoadSelectListData(win_id, active_ctrl_id, s_select_type);
        }
        break;
        
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
           MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {   
                GUILIST_GetSelectedItemIndex(MMIDCD_SETTING_LIST_CTRL_ID, &index, 1);
                if(MMIDCD_SETTING_SELECT_DCD_OPEN == s_select_type)
                {  
                    if(0 == index)
                    {
                        MMIDCD_HandleDCDSwitch(TRUE, FALSE);
                        
                        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS, &query_win_id, PNULL, MMIPUB_SOFTKEY_ONE, NULL);
                        MMK_CloseWin(win_id);
                    }
                    else// 如果是关闭.....
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_DCD_QUERY_CLOSE, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
                    }
                    
                }
                else if(MMIDCD_SETTING_SELECT_RECOVER_FACTORY != s_select_type)
                {
                    SaveDCDSettings(s_select_type, index);
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS, &query_win_id, PNULL, MMIPUB_SOFTKEY_ONE, NULL);
                    MMK_CloseWin(win_id);
                }
                MMK_CloseWin(MMIDCD_ITEM_MENU_WIN_ID);
            }

            break;

        case MSG_PROMPTWIN_CANCEL:
            {
               uint16 set_index = GetDCDSwitchIndex();// 让焦点落在原来的位置上 for cr161878....
               GUILIST_SetCurItemIndex(MMIDCD_SETTING_LIST_CTRL_ID, set_index);
               GUILIST_SetSelectedItem(MMIDCD_SETTING_LIST_CTRL_ID, set_index , TRUE);
               MMIPUB_CloseQuerytWin(&query_win_id);
            }
            break;
            
        case MSG_PROMPTWIN_OK:
            {
                GUILIST_GetSelectedItemIndex(MMIDCD_SETTING_LIST_CTRL_ID, &index, 1);

                MMIDCD_HandleDCDSwitch(FALSE, FALSE);

                MMIPUB_OpenAlertSuccessWin( TXT_DCD_PROMPT_INFO);
                
                MMIPUB_CloseQuerytWin(&query_win_id);
                MMK_CloseWin(win_id);
            }
            break;
        default:
             recode = MMI_RESULT_FALSE;
             break;
     }
     
    return recode;
}
/*****************************************************************************/
//  Description : prew界面点击左键后动态后产生的动态菜单.....
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/9/17
/*****************************************************************************/
LOCAL void CreatePreviewDynaMenu(MMI_CTRL_ID_T menuctrl_id,
                                 MMI_CTRL_ID_T list_ctrl_id
                                 )
{
    
    uint16 rootindex = 0;
    uint16 subindex  = 0;

    GUIMENU_SetDynamicMenuSoftkey(menuctrl_id, STXT_SELECT, TXT_NULL, STXT_RETURN);
    
    DcdInsertNodeByLabel(menuctrl_id, TXT_DELETE, DCD_ITEMMENU_ITEM_DELETE, 0,  rootindex++);
    
    // 如果不是本地的list控件
    if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID != list_ctrl_id)
    {
        // 保存...
        DcdInsertNodeByLabel(menuctrl_id, STXT_SAVE, DCD_ITEMMENU_SAVE, 0,  rootindex++);
    }
    
    DcdInsertNodeByLabel(menuctrl_id, TXT_SEND, DCD_ITEMMENU_SEND, 0,  rootindex++);
    //创建发送的子菜单
    DcdInsertNodeByLabel(menuctrl_id, TXT_SMS, DCD_ITEMMENU_SEND_BY_SMS, DCD_ITEMMENU_SEND, subindex++);
    DcdInsertNodeByLabel(menuctrl_id, TXT_MMS, DCD_ITEMMENU_SEND_BY_MMS, DCD_ITEMMENU_SEND, subindex++);
    // 如果不是本地的list控件
    if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID != list_ctrl_id)
    {   // 频道列表....
        DcdInsertNodeByLabel(menuctrl_id, TXT_COMMON_COMMON_CHANNEL_LIST, DCD_ITEMMENU_ENTER_MAINMENU, 0,  rootindex++);
        // 如果是默认的频道项, 删除，保存，发送均不可用....
        if(MMIDCDFILE_IsDefaultXML(s_dcd_gui_struct.current_page_index))
        {
            GUIMENU_SetNodeGrayed(TRUE, 0, 0, menuctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, 1, 0, menuctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, 2, 0, menuctrl_id);
        }
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdPreviewMenuWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 node_id = 0;
    MMI_CTRL_ID_T  ctrl_id =0;
    MMI_WIN_ID_T query_win_id = MMIDCD_QUERY_WIN_ID;
    MMI_WIN_ID_T wait_win_id = MMIDCD_COPY_FILE_WAIT_WIN_ID;
    uint32 time_out = 0;
    
    ctrl_id = (MMI_CTRL_ID_T)MMK_GetWinAddDataPtr(win_id);
    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIMENU_CreatDynamic(PNULL,win_id, MMIDCD_PREVIEW_MENU_CTRL_ID, GUIMENU_STYLE_POPUP);
            CreatePreviewDynaMenu(MMIDCD_PREVIEW_MENU_CTRL_ID, ctrl_id);
            MMK_SetAtvCtrl(win_id, MMIDCD_PREVIEW_MENU_CTRL_ID);
            DCD_EnableGrayedItemMenu(win_id,ctrl_id);               
        }
        
        break;
        
    case MSG_MULTIM_COPY_RESULT_CNF:
        {
            if (*(BOOLEAN*)param)
            {
                MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
            }
        }
        break;
        
    case MSG_KEYDOWN_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        node_id = GUIMENU_GetCurNodeId(MMIDCD_PREVIEW_MENU_CTRL_ID);
        switch(node_id)
        {
        case DCD_ITEMMENU_SEND_BY_SMS:
            if(ctrl_id == MMIDCD_LOCAL_ITEM_LIST_CTRL_ID )  
            {
                SendLocalBySMS();
            }
            else
            {
                SendBySMS();
            }               
            MMK_CloseWin(win_id);
            break;
            
        case DCD_ITEMMENU_SEND_BY_MMS:
            if(ctrl_id == MMIDCD_LOCAL_ITEM_LIST_CTRL_ID )  
            {
                SendLocalByMMS();
            }
            else
            {               
                SendByMMS();
            }       
            MMK_CloseWin(win_id);
            break;
            
        case DCD_ITEMMENU_ITEM_DELETE:
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
            }
            break;
            
            // 进入DCD主菜单....
        case DCD_ITEMMENU_ENTER_MAINMENU:
            {
                MMIDCD_OpenMainWin(TRUE);
                MMK_CloseWin(MMIDCD_PREVIEW_WIN_ID);
                MMK_CloseWin(win_id);
            }
            break;
            
        case DCD_ITEMMENU_SAVE:
            {   
                if(IsUDiskRunning())
                {
                    MMK_CloseWin(win_id);
                    break; 
                }
                if(MMI_GetFreeSpace(MMIAPIFMM_GetDefaultDisk()) < (DCD_IMAGE_MAX_SIZE >> 10))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);   
                    MMK_CloseWin(win_id);
                    break;
                }
                // 是否保存的条目数已超过50条....
                if(DCD_LOCAL_MAX_ITEM_NUM <= MMIDCDFILE_GetLocalItemCount())
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DCD_SAVE_50_ITEMS);   
                    MMK_CloseWin(win_id);
                    break;
                }
               
                //弹出等待窗口
                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleCopyLocalFileWaitWinMsg);
                MMK_CloseWin(win_id);
            }
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;
            
        case MSG_PROMPTWIN_OK:
            {
                HandleDcdPrewItemDeleteOperation(ctrl_id);
                // 只有local的没有调重新更新list,故加上.
                if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == ctrl_id)
                {
                    UpdateDCDList(ctrl_id, FALSE);
                }
                MMIPUB_CloseQuerytWin(&query_win_id);
                MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIDCD_PREVIEW_WIN_ID);
            }
            break;          
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
/*****************************************************************************/
//  Description : get item link adress
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void GetDcdItemLinkAdr( 
                             MMI_CTRL_ID_T  list_ctrl_id, 
                             char * link_adr_ptr,
                             uint16  max_len
                             )
{
    uint16 page_index = s_dcd_gui_struct.current_page_index;
    uint16 real_item_index = s_dcd_gui_struct.current_real_index_in_page;
    char * link_ptr = PNULL;
    uint16  copy_len = 0;
    MMIDCD_LOCAL_FILE_NODE_T cur_node = {0};

    if(PNULL == link_adr_ptr)
    {
        return;
    }
 
    if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == list_ctrl_id)  
    {
        if(MMIDCDFILE_ReadLocalItemInfo(&cur_node, s_local_list_item_index))
        {
            link_ptr = (char *)cur_node.link_str;
        }
        
        else
        {
            //SCI_TRACE_LOW:"MMIDCD GetDcdItemLinkAdr read  fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4674_112_2_18_2_13_4_287,(uint8*)"");
        }
       
    }
    else
    {
        // 暂时没有对real_item_index等的check...
        link_ptr = MMIDCD_GetEntryLink(page_index, real_item_index, (char*)"href");
    }

    if(PNULL != link_ptr)
    {
        copy_len = strlen(link_ptr);
        if(max_len < copy_len)
        {
            copy_len = max_len;
        }
        
        strncpy(link_adr_ptr, link_ptr, copy_len);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD find no link_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4696_112_2_18_2_13_4_288,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void EntryInternet(char * urlink_ptr)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};    /*lint !e64*/
    char dcd_ua[MMIDCD_MAX_UA_LEN + 1] = {0};

    if(PNULL != urlink_ptr )
    {
    //MS00216980 cheney begin
        strcat(dcd_ua, MMIAPICOM_GetUserAgent());
        entry_param.user_agent_ptr = PNULL; 
        entry_param.type = MMIBROWSER_ACCESS_URL;
        entry_param.dual_sys = MN_DUAL_SYS_MAX;
        entry_param.url_ptr = urlink_ptr;
        entry_param.user_agent_ptr = dcd_ua;
        entry_param.is_connection_customized = TRUE;
        entry_param.connection_index = 0;
        MMIAPIBROWSER_Entry(&entry_param); 
    }
    else
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
#else
    MMIPUB_OpenAlertFailWin(TXT_ERROR);
#endif

}
/*****************************************************************************/
//  Description : richtext show
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note: 
/*****************************************************************************/
LOCAL void DCD_UpdatePreviewWin(MMI_WIN_ID_T  win_id,
                                MMI_CTRL_ID_T ctrl_id
                                )
{
    wchar title_str[MMIDCD_FEED_TITLE_LEN + 1] = {0};
    uint16  title_len = 0;
    // jun.hu 2009/10/17 修改把它们移动到下面去保持窗口的title和dcd详情的内容一致性......
    // title_len = MMIDCD_GetFeedTitle(title_str, MMIDCD_FEED_TITLE_LEN, MMIAPIDCD_GetXmlDoc_Ptr(), s_dcd_gui_struct.current_page_index );
    // GUIWIN_SetTitleText(win_id, title_str, title_len, TRUE);                             
                
    // 先去掉控件上旧的内容....
    GUIRICHTEXT_DeleteAllItems(MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID);
    // 再给richtextbox 附上新的内容......
    s_dcd_gui_struct.current_page_index=0;
    s_dcd_gui_struct.current_index_in_page = 0; 
    
    title_len = MMIDCD_GetFeedTitle(title_str, MMIDCD_FEED_TITLE_LEN, s_dcd_gui_struct.current_page_index );
    GUIWIN_SetTitleText(win_id, title_str, title_len, TRUE); 

    s_dcd_gui_struct.current_real_index_in_page = MMIDCD_GetRealItemIndex(s_dcd_gui_struct.current_page_index,s_dcd_gui_struct.current_index_in_page, TRUE);
    DCD_SetItemDetails(s_dcd_gui_struct.current_real_index_in_page);
    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
}
/*****************************************************************************/
//  Description : 设置DCD查看详情窗口的内容...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL void SetDCDPrewWinContent(
                                MMI_WIN_ID_T    win_id,
                                MMI_CTRL_ID_T   ctrl_id
                                )
{
    //窗口里面用到的richtect 控件
    MMI_CTRL_ID_T richtext_ctrl_id = MMIDCD_ITEM_PREVIEW_RICHTEXT_CTRL_ID;
    wchar title_str[MMIDCD_FEED_TITLE_LEN + 1] = {0};
    uint16 title_len = 0;

    GUIRICHTEXT_SetFont(richtext_ctrl_id, MMI_DEFAULT_NORMAL_FONT);
    GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
    // 如果是查看已存条目
    if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == ctrl_id)
    {   
        GUIWIN_SetTitleTextId(win_id, TXT_DCD_LOCAL_TITLE, FALSE);
        s_local_list_item_index= GUILIST_GetCurItemIndex(ctrl_id);
        DCD_SetLocalItemDetails(s_local_list_item_index);
    }
    else
    {
        // 对于直接从idle进来的情况,因为它用到的item是位于xml文件中的绝对值，
        // 下面左右键的处理都是基于相对值，因此需要转换一下....
        if(MMIDCD_IDLEWIN_TEXT_CTRL_ID == ctrl_id)
        {   //给这两个变量赋值是为了控制左右键移动
            s_dcd_gui_struct.current_index_in_page = MMIDCDFILE_RealIndexToShowIndex(s_dcd_gui_struct.cur_idle_page_index, s_dcd_gui_struct.cur_idle_realitem_index);
            s_dcd_gui_struct.current_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(s_dcd_gui_struct.cur_idle_page_index, TRUE);
            s_dcd_gui_struct.current_real_index_in_page = s_dcd_gui_struct.cur_idle_realitem_index;
            s_dcd_gui_struct.current_page_index = s_dcd_gui_struct.cur_idle_page_index;
        }
        else
        {
            s_dcd_gui_struct.current_index_in_page = GUILIST_GetCurItemIndex(ctrl_id);
            // 把当前正在操作的realindex记录下来....
            s_dcd_gui_struct.current_real_index_in_page = MMIDCD_GetRealItemIndex(s_dcd_gui_struct.current_page_index, s_dcd_gui_struct.current_index_in_page, TRUE);
        }

        title_len = MMIDCD_GetFeedTitle(title_str, MMIDCD_FEED_TITLE_LEN,  s_dcd_gui_struct.current_page_index);
        GUIWIN_SetTitleText(win_id, title_str, title_len, FALSE);

        DCD_SetItemDetails(s_dcd_gui_struct.current_real_index_in_page);
    }
    
    GUIWIN_SetTitleRollArrow(win_id, TRUE);
    MMK_SetAtvCtrl(win_id, richtext_ctrl_id);
}
/*****************************************************************************/
//  Description : 处理查看本地已存详情界面的item的切换问题...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL void HandDCDPrewLocalItemSwitch(
                                      MMI_WIN_ID_T    win_id,
                                      uint16          cur_item_index,
                                      BOOLEAN         prew_next     // 为TRUE ,表示查看下一条，FALSE代表查看前一条...
                                      )
{
    uint16 item_num = MMIDCDFILE_GetLocalItemCount();
    BOOLEAN is_should_switch = FALSE;

    //SCI_TRACE_LOW:"[MMIDCD] curitem,  item_num= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4821_112_2_18_2_13_4_289,(uint8*)"d", cur_item_index, item_num);

    // 查看下一条
    if(prew_next)
    {
        // 如果下一条是列表上的条目（有意义的）.....
        if((cur_item_index + 1) < s_local_list_total_num)
        {
            s_local_list_item_index = (uint16)(cur_item_index + 1);
            is_should_switch = TRUE;
        }
    }
    else
    {
        // 如果当前的条目不是列表上的第1项条目
        if(0 < cur_item_index)
        {
            s_local_list_item_index = (uint16)(cur_item_index - 1);
            is_should_switch = TRUE;
        }
    }

    if(is_should_switch)
    {
        // 给richtextbox 附上新的内容......
        DCD_SetLocalItemDetails(s_local_list_item_index);
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
    }
    
}

/*****************************************************************************/
//  Description : 得到下一个可用PAGE的index...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL BOOLEAN GetNextUseablePageIndex(
                                      uint16 *   page_index_ptr,  // out
                                      uint16    cur_page_index
                                      )
{
    uint16 page_item_num = 0;
    uint16 i = 0;
    uint16 page_num = s_dcd_gui_struct.total_page_num;
    BOOLEAN result = FALSE;

    if(PNULL == page_index_ptr)
    {
        return FALSE;
    }

    //如果当前的频道不是最后一个频道
    if(page_num > cur_page_index + 1)
    {
        for( i = cur_page_index + 1 ; i < page_num; i++)
        {
            page_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(i, TRUE);
            // 得到的频道项数不为0.
            if(0 < page_item_num)
            {
                *page_index_ptr = i;
                result = TRUE;
                break;
            }
            
        }
    }

    return result;
}
/*****************************************************************************/
//  Description : 得到上一个可用PAGE的index...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL BOOLEAN GetPrewUseablePageIndex(
                                      uint16 *   page_index_ptr,  // out
                                      uint16    cur_page_index
                                      )
{
    uint16 page_item_num = 0;
    int16 i = cur_page_index;
    BOOLEAN result = FALSE;

    if(PNULL == page_index_ptr)
    {
        return FALSE;
    }

    //如果当前的频道不是第一个频道
    if(0 < cur_page_index)
    {
        for(i = cur_page_index - 1; i >= 0; i--)
        {
            page_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(i, TRUE);
            // 得到的频道项数不为0.
            if(0 < page_item_num)
            {
                *page_index_ptr = i;
                result = TRUE;
                break;
            }
            
        }
    }

    return result;
}
/*****************************************************************************/
//  Description : 处理查看详情界面的item的切换问题...
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/28
/*****************************************************************************/
LOCAL void HandDCDPrewItemSwitch(
                                 MMI_WIN_ID_T    win_id,
                                 uint16          cur_item_index,
                                 BOOLEAN         prew_next     // 为TRUE ,表示查看下一条，FALSE代表查看前一条...
                                 )
{
    BOOLEAN is_should_switch = FALSE;
    wchar title_str[MMIDCD_FEED_TITLE_LEN + 1] = {0};
    uint16 title_len = 0;
    uint16 page_index = 0;

    //SCI_TRACE_LOW:"[MMIDCD] curitem,  item_num= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4942_112_2_18_2_13_4_290,(uint8*)"d", cur_item_index, s_dcd_gui_struct.current_item_num);

     //如果是从更新前的详情界面而来....
    if(GetDataChangeFlag())
    {
        is_should_switch = TRUE;
        //SCI_TRACE_LOW:"MMIDCD HandDCDPrewItemSwitch DATA has changed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_4948_112_2_18_2_13_4_291,(uint8*)"");
    }

    // 查看下一条
    if(prew_next)
    {
        // 如果下一条是当前频道上列表上的条目.....
        if((cur_item_index + 1) < s_dcd_gui_struct.current_item_num)
        {
            s_dcd_gui_struct.current_index_in_page = cur_item_index + 1;
            is_should_switch = TRUE;
        }
        else
        {
            //因为存在频道项的删除操作，所以不能简单的把频道的index++
            if(GetNextUseablePageIndex(&page_index, s_dcd_gui_struct.current_page_index))
            {
                s_dcd_gui_struct.current_page_index = page_index;
                // 切换到找到的频道的第一个条目上去....
                s_dcd_gui_struct.current_index_in_page = 0;
                s_dcd_gui_struct.current_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(s_dcd_gui_struct.current_page_index, TRUE);
                
                title_len = MMIDCD_GetFeedTitle(title_str, MMIDCD_FEED_TITLE_LEN, s_dcd_gui_struct.current_page_index);
                GUIWIN_SetTitleText(win_id, title_str, title_len, FALSE);

                is_should_switch = TRUE;
            }
            
        }
    }
    else
    {
        // 如果当前这一条不是当前频道上的第一个条目.....
        if(0 < cur_item_index)
        {
            s_dcd_gui_struct.current_index_in_page = (uint16) (cur_item_index - 1);
            is_should_switch = TRUE;

        }
        else
        {
            
            //如果当前的频道不是第一个频道，则切换到上一个频道上去...
            if(GetPrewUseablePageIndex(&page_index, s_dcd_gui_struct.current_page_index) )
            {
                s_dcd_gui_struct.current_page_index = page_index;
                // s_dcd_gui_struct.current_index_in_page = 0;
                s_dcd_gui_struct.current_item_num = MMIAPIDCDFILE_GetFeedUseableEntryCount(s_dcd_gui_struct.current_page_index, TRUE);
                // 按左键时切换DCD的条目应该切到前一个频道的最后一个条目上去 for cr159888.....
                if(1 < s_dcd_gui_struct.current_item_num)
                {
                    s_dcd_gui_struct.current_index_in_page = s_dcd_gui_struct.current_item_num - 1;
                }
                else
                {
                    s_dcd_gui_struct.current_index_in_page = 0;
                }
                
                title_len = MMIDCD_GetFeedTitle(title_str, MMIDCD_FEED_TITLE_LEN, s_dcd_gui_struct.current_page_index);
                GUIWIN_SetTitleText(win_id, title_str, title_len, FALSE);
                
                is_should_switch = TRUE;
            }
        }
    }

    if(is_should_switch)
    {
        s_dcd_gui_struct.current_real_index_in_page = MMIDCD_GetRealItemIndex(s_dcd_gui_struct.current_page_index, s_dcd_gui_struct.current_index_in_page, TRUE);
        if(GetDataChangeFlag())
        {
            // 都要更新title
            title_len = MMIDCD_GetFeedTitle(title_str, MMIDCD_FEED_TITLE_LEN, s_dcd_gui_struct.current_page_index);
            GUIWIN_SetTitleText(win_id, title_str, title_len, FALSE);
        }
        // 给richtextbox 附上新的内容......
        DCD_SetItemDetails(s_dcd_gui_struct.current_real_index_in_page);
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
    }

    // 恢复此变量....
    SetDataChangeFlag(FALSE);
}
/*****************************************************************************/
//  Description : richtext show
//  Global resource dependence : 
//  Author:
//  Note: 修改后：通过相对值控制左右键，通过item的绝对值设置具体的内容....
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdPreviewWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id = 0;

    ctrl_id = (MMI_CTRL_ID_T )MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {        
                SetDCDPrewWinContent(win_id, ctrl_id);
#ifdef MMI_PDA_SUPPORT
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            }
            break;
        
        case MSG_CLOSE_WINDOW:
            if(MMIDCD_ITEM_LIST_CTRL_ID == ctrl_id)
            {
                // 更新DCD主界面的TAB上的forcus项...
                MMK_PostMsg(MMIDCD_MAIN_WIN_ID, MSG_DCD_SYNCHRONIZE_DISPLAY, PNULL,0);  
            }
#ifdef MMIWIDGET_SUPPORT         
	     if(MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())	
                	MMK_PostMsg(WIDGET_DCD_WIN_ID, MSG_DCD_SYNCHRONIZE_DISPLAY, PNULL,0);
	     else
#endif
            		MMK_PostMsg(MMIDCD_IDLE_CHILD_WIN_ID, MSG_DCD_SYNCHRONIZE_DISPLAY, PNULL,0);  
            // 恢复此变量.....
            SetDataChangeFlag(FALSE);
            break;
        
        case MSG_CTL_OK:
        case MSG_KEYDOWN_OK:
        case MSG_APP_MENU:
            if(GetDataChangeFlag())
            {
                MMIPUB_OpenAlertWarningWin(TXT_DCD_DATA_CHANGED);
            }
            else
            {
                MMK_CreateWin((uint32*)MMIDCD_PREVIEW_MENU_WIN_TAB, (ADD_DATA)ctrl_id);
            }
            break;
        
        case MSG_CTL_TITLE_MOVE_PREV:    
        case MSG_KEYUP_LEFT:
            {
                 // 如果是查看已存条目
                 if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == ctrl_id)
                 {  
                     HandDCDPrewLocalItemSwitch(win_id, s_local_list_item_index, FALSE);
                     SetDataChangeFlag(FALSE);
                 }
                 else
                 {  // 对于直接从idle进来的情况,在打开窗口时已经给s_dcd_gui_struct.current_index_in_page赋值了...
                     HandDCDPrewItemSwitch(win_id, s_dcd_gui_struct.current_index_in_page, FALSE);
                 }
            }
        break;
        
        case MSG_CTL_TITLE_MOVE_NEXT:  
        case MSG_KEYUP_RIGHT:
            {
                // 如果是查看已存条目
                 if(MMIDCD_LOCAL_ITEM_LIST_CTRL_ID == ctrl_id)
                 {
                     HandDCDPrewLocalItemSwitch(win_id, s_local_list_item_index, TRUE);
                     SetDataChangeFlag(FALSE);
                 }
                 else
                 {   // 对于直接从idle进来的情况,在打开窗口时已经给s_dcd_gui_struct.current_index_in_page赋值了...
                     HandDCDPrewItemSwitch(win_id, s_dcd_gui_struct.current_index_in_page, TRUE);
                 }
            }
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK: 
        case MSG_APP_WEB: 
            if(GetDataChangeFlag())
            {
                MMIPUB_OpenAlertWarningWin(TXT_DCD_DATA_CHANGED);
            }
            else
            {
                char url_arr[DCD_MAX_HREF_LEN + 1] = {0};
                GetDcdItemLinkAdr(ctrl_id, url_arr, DCD_MAX_HREF_LEN);
                EntryInternet(url_arr);
            }
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            // 关闭当前窗口
            MMK_CloseWin(win_id);
            break;
        
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : dcd local 主界面的消息处理函数
//  Global resource dependence : 
//  Author:
//  Note: jun.hu 2009/10/17 把local的两个father, child ,tab的做法去掉了...
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLocalMainWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  list_ctrl_id = MMIDCD_LOCAL_ITEM_LIST_CTRL_ID;
    uint16          item_index = 0;
    uint16          item_content_index=0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
            GUILIST_SetDefaultIcon(list_ctrl_id, IMAGE_DCD_DEFAULT_IMAGE, IMAGE_DCD_DEFAULT_IMAGE);
            s_local_list_item_index = 0; //打开DCD local 主窗口时，定位到第1条上.....
            UpdateDCDList(list_ctrl_id, FALSE);
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            if(0 == s_local_list_total_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            }
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            //add text into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if( PNULL != need_item_content_ptr )
            {
                item_index = need_item_content_ptr->item_index;
                AppendOneLocalListItemWithText(item_index, list_ctrl_id);
            }
        }
        
        break;
        
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {            
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if( PNULL != need_item_content_ptr )
            {
                item_index = need_item_content_ptr->item_index;
                item_content_index = need_item_content_ptr->item_content_index;
                DrawLocalListItemWithPicture(item_index,item_content_index,list_ctrl_id);
            }
        }
        break;           

    case MSG_CTL_LIST_MARK_ITEM:
        {
            GUILIST_MARK_ITEM_T* list_param_ptr = (GUILIST_MARK_ITEM_T *)param;

            if(PNULL == list_param_ptr)
            {
                break;
            }
            
            if (list_param_ptr->had_maked)
            {
                HandleDcdListMarkOperation(list_ctrl_id, DCD_MARK_CURONE);
            }
            else
            {
                HandleDcdListUnMarkOperation(list_ctrl_id, DCD_UNMARK_CURONE);
            }
        }
        break;
        
        
    case MSG_DCD_LIST_UPDATE:
         s_local_list_total_num = MMIDCDFILE_GetLocalItemCount();
         if (0 < s_local_list_total_num)
         {
             UpdateDCDList(list_ctrl_id, TRUE);
         }

     break;
      
    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        s_local_list_total_num = MMIDCDFILE_GetLocalItemCount();
        if (0 < s_local_list_total_num)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        {
        	  s_local_list_total_num = MMIDCDFILE_GetLocalItemCount();
            s_local_list_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            if(0 < s_local_list_total_num)
            {
                MMK_CreateWin((uint32 *)MMIDCD_LOCAL_ITEM_MENU_WIN_TAB, (ADD_DATA)list_ctrl_id);
            }
        }
        break;          
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    		s_local_list_total_num = MMIDCDFILE_GetLocalItemCount();
        s_local_list_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        if(!IsUDiskRunning())
        {
            if(0 < s_local_list_total_num)
            {
                MMK_CreateWin((uint32*)MMIDCD_PREVIEW_WIN_TAB, (ADD_DATA)list_ctrl_id);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        s_local_list_item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        MMK_CloseParentWin(MMIDCD_LOCAL_MAIN_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
/*****************************************************************************/
//  Description : 加载dcd订购详情
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
LOCAL void addDcdOrderTextItem(const MMI_CTRL_ID_T richtext_ctrl_id,
                               const MMI_TEXT_ID_T text_id, 
                               uint16 index
                               )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};     /*lint !e64*/
    MMI_STRING_T  dststring= {0};           

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    MMI_GetLabelTextByLang(text_id, &dststring);
    item_data.text_data.buf.len = (uint16)MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
    GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &index);
}
/*****************************************************************************/
//  Description : 加载dcd订购详情中的上次同步状态.....
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
LOCAL void addDcdLastUpdateStateDetail(const MMI_CTRL_ID_T richtext_ctrl_id,
                                       const uint16 index
                                       )
{
    uint8  str_str[MMIDCD_MAX_EXTAND_TYPE_STR_LEN]={0};
    wchar  wsr_dst[MMIDCD_MAX_TITLE_STR_LEN] = {0};
    GUIRICHTEXT_ITEM_T  item_data = {0};     /*lint !e64*/
    uint16     itemindex = index;
    MMIDCD_ERROR_CODE_E error_code = MMIDCD_GetLatestUpdateState();
    MMI_STRING_T  dststring= {0};
    MMI_STRING_T tmp_prompt = {0};
    uint8 disp_num_len = 0;
    uint8 disp_str_len = 0;
    uint8 test_len=0;   
    const wchar     test_str[] = {L","};
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    addDcdOrderTextItem(richtext_ctrl_id, TXT_DCD_LAST_UPDATE_STATE, itemindex);
    itemindex++;
    // 成功
    if(DCD_ERROR_NULL == error_code)
    {
        MMI_GetLabelTextByLang(TXT_SUCCESS, &dststring);
        item_data.text_data.buf.len = (uint16)MIN(dststring.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
        GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &itemindex); 
    }
    else
    {
        disp_num_len= sprintf((char *)str_str, "%d", error_code);
        MMI_STRNTOWSTR(wsr_dst, disp_num_len, str_str, disp_num_len, disp_num_len);
        
        test_len= MMIAPICOM_Wstrlen(test_str);
        MMI_WSTRNCPY(wsr_dst+disp_num_len,MMIDCD_MAX_TITLE_STR_LEN,test_str,test_len,test_len);
        disp_num_len+= test_len;
        
        switch (error_code) 
       {
         case DCD_ERROR_XML_FORMAT:
         case DCD_ERROR_NO_XML:     
         case DCD_ERROR_WRONG_DATA:     
         case DCD_ERROR_DUMMY_DATA:             
             MMI_GetLabelTextByLang(TXT_DCD_ERROR_DATA_WRONG, &tmp_prompt);
             break;       
             
         case DCD_ERROR_NO_MEMORY:      
             MMI_GetLabelTextByLang(TXT_NO_SPACE, &tmp_prompt);     
             break;
         case DCD_ERROR_INVALID_NETWORK:        
             MMI_GetLabelTextByLang(TXT_COMMON_NET_ERROR, &tmp_prompt);        
             break;
         case DCD_ERROR_NETWORK_USING:              
             MMI_GetLabelTextByLang(TXT_DCD_ERROR_NETWORK_USING, &tmp_prompt);      
             break;     
         case DCD_ERROR_TIMEOUT:                
             MMI_GetLabelTextByLang(TXT_DCD_ERROR_DATA_TIMEOUT, &tmp_prompt);       
             break;     
             
         default:
             MMI_GetLabelTextByLang(TXT_UNKNOWN_ERROR, &tmp_prompt);        
             break;
        }
        
        // the total string wstr_len
        disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;
        
        // allocate memory
        dststring.wstr_ptr = MMIDCD_ALLOC((disp_str_len+1)*sizeof(wchar));
        SCI_MEMSET(dststring.wstr_ptr, 0x00, ((disp_str_len+1)*sizeof(wchar)));
        
        MMI_WSTRNCPY(dststring.wstr_ptr, disp_str_len, wsr_dst, disp_num_len, disp_num_len);
        
        dststring.wstr_len = disp_num_len;
        
        // copy the prompt string
        MMI_WSTRNCPY(
            &(dststring.wstr_ptr[dststring.wstr_len]),
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr, 
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
            );
        dststring.wstr_len += tmp_prompt.wstr_len;
        
        
        item_data.text_data.buf.len = dststring.wstr_len ;
        item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
        GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &itemindex); 
        
        if (PNULL != dststring.wstr_ptr)
        {
            MMIDCD_FreeMem(dststring.wstr_ptr);
            dststring.wstr_ptr = PNULL;
        }   
        
    }

}
/*****************************************************************************/
//  Description : 加载dcd订购详情中的时间信息.....
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/9/8
/*****************************************************************************/
LOCAL void addDcdUpdateTimeDetail(const MMI_CTRL_ID_T richtext_ctrl_id,
                                  const uint16  itemidx
                                  )
{
    uint8  str_str[56] = {0};
    wchar  wsr_dst[50] = {0};
    GUIRICHTEXT_ITEM_T  item_data = {0};        /*lint !e64*/
    MMIDCD_NV_SETTING_T  dcd_info = {0};      /*lint !e64*/
    uint16   index = itemidx;
    MMI_STRING_T  dststring= {0};
    uint32        next_time = 0;

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    // 上次更新时间
    addDcdOrderTextItem(richtext_ctrl_id, TXT_DCD_LAST_CONNECT_TIME, index);
    index +=1;
    
    SCI_MEMCPY(&dcd_info, MMIAPIDCD_GeSettingInfo(), sizeof(MMIDCD_NV_SETTING_T));
    sprintf((char*)str_str, "%04d/%02d/%02d   %02d:%02d:%02d",dcd_info.dcd_update_time.tm_year,dcd_info.dcd_update_time.tm_mon,\
        dcd_info.dcd_update_time.tm_mday, dcd_info.dcd_update_time.tm_hour,\
        dcd_info.dcd_update_time.tm_min, dcd_info.dcd_update_time.tm_sec);
    
    MMIAPICOM_StrToWstr(str_str, wsr_dst);
    item_data.text_data.buf.len = (uint16)MIN(MMIAPICOM_Wstrlen(wsr_dst), GUIRICHTEXT_TEXT_MAX_LEN);/*lint !e666*/
    item_data.text_data.buf.str_ptr = wsr_dst;
    GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &index); 
  
    SCI_MEMSET(str_str, 0 , sizeof(str_str));
    SCI_MEMSET(wsr_dst, 0 , sizeof(wsr_dst));
    
    // 距下次更新时间
    index += 1;
    addDcdOrderTextItem(richtext_ctrl_id, TXT_DCD_NEXT_UPDATE_TIME, index);
    index += 1;
    next_time = MMIDCD_GetNextUpdateTimeSpace();
    next_time= next_time/60;
    sprintf((char*)str_str, "%d", next_time);
    MMIAPICOM_StrToWstr(str_str, wsr_dst);
    if(1 >= next_time)
    {
        MMI_GetLabelTextByLang(TXT_TIME_MINUTE, &dststring);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_COMM_MINUTES, &dststring);
    }
    MMIAPICOM_Wstrncat(wsr_dst, dststring.wstr_ptr, dststring.wstr_len);
    item_data.text_data.buf.len = (uint16)MIN(MMIAPICOM_Wstrlen(wsr_dst), GUIRICHTEXT_TEXT_MAX_LEN);/*lint !e666*/
    item_data.text_data.buf.str_ptr = wsr_dst;
    GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &index); 
}
/*****************************************************************************/
//  Description : 加载dcd订购详情
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/8/4
/*****************************************************************************/
LOCAL void loadDcdOrderDetail(MMI_CTRL_ID_T richtext_ctrl_id)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};      /*lint !e64*/
    const wchar   wst_tab[] = {'\n', 0};
    uint16        page_num = 0;
    uint16        i = 0;
    uint16        index = 0;
    uint16        item_num = 0;
    wchar  wsr_dst[10] = {0};
    uint8  str_str[10]= {0};
    wchar wsr_title[MMIDCD_FEED_TITLE_LEN] = {0};

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    // 已定阅的内容 
    addDcdOrderTextItem(richtext_ctrl_id, TXT_DCD_ORDER_CONTENT, index);
    page_num = MMIAPIDCD_GetFormalFeedNum();   
        
    for(i=0; i < page_num; i++)
    {
        MMIDCD_GetFeedTitle(wsr_title, MMIDCD_FEED_TITLE_LEN, i);
        MMIAPICOM_Wstrcat(wsr_title, wst_tab);

        item_data.text_data.buf.len = (uint16)MIN(MMIAPICOM_Wstrlen(wsr_title), GUIRICHTEXT_TEXT_MAX_LEN); /*lint !e666*/
        item_data.text_data.buf.str_ptr = wsr_title;
        index = (uint16)(i + 1);
        GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &index);
    }
    

    //已下载的内容 ...
    index +=1;
    addDcdOrderTextItem(richtext_ctrl_id, TXT_DCD_DL_CONTENT, index);
    index +=1;
    page_num = MMIAPIDCD_GetFormalFeedNum();
    for(i=0, item_num=0; i < page_num; i++)      
    {
        item_num = (uint16)(item_num + MMIDCD_GetFormalEntryNumInFeed(i));            
    }
    sprintf((char*)str_str,"%02d",item_num);
    MMIAPICOM_StrToWstr(str_str, wsr_dst);
    item_data.text_data.buf.len = (uint16)MIN(MMIAPICOM_Wstrlen(wsr_dst), GUIRICHTEXT_TEXT_MAX_LEN);/*lint !e666*/
    item_data.text_data.buf.str_ptr = wsr_dst;
    GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &index); 

    // 上次同步状态
    index +=1;
    addDcdLastUpdateStateDetail(richtext_ctrl_id, index);
    index +=1;
    
    // 上次更新时间
    index +=1;
    addDcdUpdateTimeDetail(richtext_ctrl_id, index);
    
}
/*****************************************************************************/
//  Description : 订购详情界面消息处理
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/8/4
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdOrderDetailWinMsg(
                                             MMI_WIN_ID_T      win_id,  
                                             MMI_MESSAGE_ID_E  msg_id, 
                                             DPARAM            param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  richtext_ctrl_id = MMIDCD_ITEM_ORER_RICHTEXT_CTRL_ID;
    
    switch (msg_id) 
    {
      case MSG_OPEN_WINDOW:
      {
        GUIRICHTEXT_SetFont(richtext_ctrl_id,MMI_DEFAULT_NORMAL_FONT);
        // set all the context in richtext
        loadDcdOrderDetail(richtext_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, richtext_ctrl_id);
      }     
        break;
        
      case MSG_FULL_PAINT:
        break;
      case MSG_APP_CANCEL:
      case MSG_CTL_CANCEL:
        // 关闭当前窗口
        MMK_CloseWin(win_id);
        break;
    
      default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return (recode);
}
/*****************************************************************************/
//  Description : 订购帮助界面消息处理
//  Global resource dependence : 
//  Author: jun.hu
//  Note: 2009/8/12
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDcdHelpWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  richtext_ctrl_id = MMIDCD_ITEM_HELP_RICHTEXT_CTRL_ID;
    GUIRICHTEXT_ITEM_T  item_data = {0};             /*lint !e64*/
    MMI_STRING_T  dststring= {0};
    uint16 index = 0;
    
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            {
                item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
                item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            
                GUIRICHTEXT_SetFont(richtext_ctrl_id, MMI_DEFAULT_BIG_FONT);
                // set all the context in richtext
                MMI_GetLabelTextByLang(TXT_DCD_LOCAL_HELP_CONTENT, &dststring);
                item_data.text_data.buf.len = (uint16)MIN(dststring.wstr_len, MMIDCD_MAX_SUMMARY_STR_LEN);
                item_data.text_data.buf.str_ptr = dststring.wstr_ptr;
                GUIRICHTEXT_AddItem(richtext_ctrl_id, &item_data, &index); 
            
                MMK_SetAtvCtrl(win_id, richtext_ctrl_id);
            }       
            break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                char uri[DCD_MAX_HREF_LEN + 1] = {0};
                if(PNULL != MMIAPIDCD_MAIN_GetNetHomePage())
                {
                    strcpy(uri, MMIAPIDCD_MAIN_GetNetHomePage()); /*lint !e668*/
                    strcat(uri, DCD_HELP_PART_URL);
                    EntryInternet(uri);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            // 关闭当前窗口
            MMK_CloseWin(win_id);
            // 关闭popmenu窗口
            MMK_CloseWin(MMIDCD_ITEM_MENU_WIN_ID);
            break;
        
        default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    

    return (recode);
}
/*****************************************************************************/
//  Description : handle waiting  win msg  for  saving  as while coping file
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: modified by jun.hu 2010/1/21
/*****************************************************************************/
LOCAL  MMI_RESULT_E HandleCopyLocalFileWaitWinMsg(
                                                  MMI_WIN_ID_T        win_id,   //in
                                                  MMI_MESSAGE_ID_E    msg_id,   //in  
                                                  DPARAM              param     //in
                                                  )
{
    MMI_RESULT_E result     = MMI_RESULT_TRUE;
    BOOLEAN copy_result     = FALSE;
    char *  contenttype_ptr = PNULL;
    char *  contentId_ptr   = PNULL;
    uint16  page_index      = s_dcd_gui_struct.current_page_index; 
    uint16  item_index      = s_dcd_gui_struct.current_index_in_page;
    uint16  real_item_index = 0;
    uint16  file_name_len   = 0;
    wchar   picutre_full_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

    MMIDCD_FILE_SAVE_ITEM_RESULT_E save_result = MMIDCD_ITEM_SAVE_FAIL;

    real_item_index = MMIDCD_GetRealItemIndex(page_index, item_index, TRUE);
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        save_result = MMIDCDFILE_SaveOneItemText(page_index, real_item_index);
        if(MMIDCD_ITEM_SAVE_SUCESS == save_result)
        {
            contentId_ptr   = MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"ctxt-id");
            contenttype_ptr = MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"type");
             // 保存图片        
            if(PNULL != contentId_ptr && PNULL != contenttype_ptr)
            {
                MMIDCDFILE_GetOnePictureFullPath(picutre_full_name, contentId_ptr, contenttype_ptr, MMIDCD_PICTURE_DIR_PATH);
                file_name_len = MMIAPICOM_Wstrlen(picutre_full_name);
                if(MMIAPIFMM_IsFileExist(picutre_full_name, file_name_len))
                {
                    if(!MMIDCDFILE_SaveOnePicture(contentId_ptr, contenttype_ptr, win_id))
                    {   
                        copy_result = FALSE;
                        MMK_PostMsg(win_id, MSG_MULTIM_COPY_RESULT_CNF, (DPARAM)&copy_result, sizeof(BOOLEAN));
                    }
                }
                else
                {
                    copy_result = TRUE;
                    MMK_PostMsg(win_id, MSG_MULTIM_COPY_RESULT_CNF, (DPARAM)&copy_result, sizeof(BOOLEAN));
                }
                
            }
            else
            {
                copy_result = TRUE;
                MMK_PostMsg(win_id, MSG_MULTIM_COPY_RESULT_CNF, (DPARAM)&copy_result, sizeof(BOOLEAN));
            }

        }
        else if(MMIDCD_ITEM_SAVE_FAIL == save_result)
        {
            copy_result = FALSE;
            MMK_PostMsg(win_id, MSG_MULTIM_COPY_RESULT_CNF, (DPARAM)&copy_result, sizeof(BOOLEAN));
        }
        else // else 的情况是返回的该频道项已保存过....
        {
            copy_result = TRUE;
            MMK_PostMsg(win_id, MSG_MULTIM_COPY_RESULT_CNF, (DPARAM)&copy_result, sizeof(BOOLEAN));
        }
        // 这里不能把MMK_PostMsg提取出来放到最后，因为MMIDCDFILE_SaveOnePicture函数内部会发一个过来...
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param); // 调用默认的处理函数设置softkey 键...
        break;

    case MSG_MULTIM_COPY_RESULT_CNF:
        copy_result=*(BOOLEAN *)param;
        if (copy_result)
        {
            MMIPUB_OpenAlertSuccessWin( TXT_COMM_SAVE_SUCCESS);
        }
        else
        {
            //保存不成功
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : handle idle win message
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_HandleIdleWinMessage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    BOOLEAN result = TRUE;
    static BOOLEAN          s_is_need_update = FALSE;
    MMISET_IDLE_STYLE_E     idle_style  = MMISET_IDLE_STYLE_COMMON;
    idle_style = MMIAPISET_GetIdleStyle();

    #ifdef MMIWIDGET_SUPPORT
        if (MMISET_IDLE_STYLE_WIDGET == idle_style)//JONATHAN
        {
            return FALSE;
        }
#endif

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        case MSG_LCD_SWITCH:
            if(MMIAPIDCD_IsIdleDisplay())
            {            
                if(MMIDCD_CreateIdleCtrl(win_id))   //create dcd idle ctrl         
                {
                    MMIDCD_AppendChildWin(win_id );
                }
            }
            s_is_need_update=FALSE;      
            break;

        case MSG_FULL_PAINT:
            if(MMISET_IDLE_STYLE_COMMON == idle_style)
            {
                if(MMIAPIDCD_IsIdleDisplay())
                {
#ifdef MMI_SMART_IDLE_SUPPORT
                    MMIAPIIDLE_ActiveDcdCtrl(win_id, TRUE);
#endif
                }
            }
            break;

        case MSG_NOTIFY_TAB_NEED_ITEM_CONTENT:
            {
                uint16 page_index=0;
                GUITAB_NEED_ITEM_DATA_T *data_ptr = (GUITAB_NEED_ITEM_DATA_T*)param;
                page_index = data_ptr->item_index;
                MMIDCD_AppendOneTabPage(MMIDCD_IDLEWIN_TAB_CTRL_ID, data_ptr->point, page_index);
            }
                break;

        case MSG_NOTIFY_TAB_SWITCH:
            {
                BOOLEAN is_active = FALSE;
                
                //@CR237614 2011.05.31        
                //if(FALSE == is_text_end)
                //{
                //    MMIDLE_RestartSsTimer(win_id);
                //}
                //@CR237614 2011.05.31      
                
                MMIDCD_SetTabListItem();
                // 这里加上是因为用户在IDLE界面触摸笔点击DCD时，TAB键处理了事件，并发了个MSG_NOTIFY_TAB_SWITCH
                // 消息上来，idle界面没有截获触摸笔消息，故在这个消息里面处理....
                is_active = MMK_IsActiveCtrl(MMIDCD_IDLEWIN_TAB_CTRL_ID);
                if(is_active) // for cr170158
                {
#ifdef MMI_SMART_IDLE_SUPPORT
                    MMIAPIIDLE_FocusToDCDLine();
#endif
                }
                
               is_text_end = FALSE;//@CR237614 2011.05.31    
            }
            break;
            
        case MSG_DCD_TAB_UPDATE:
            s_is_need_update=TRUE;
            if(MMK_IsFocusWin(win_id))
            {
                UpdateDcdIdleWin(win_id);        
                s_is_need_update=FALSE;         
            }       
            break;

        case MSG_BACKLIGHT_TURN_ON:
        case MSG_GET_FOCUS:
		//idle模式切换后判断tab 控件是否存在	
 		if(NULL == MMK_GetCtrlPtr(MMIDCD_IDLEWIN_TAB_CTRL_ID))	    
		s_is_need_update=TRUE;
		
            if(s_is_need_update)
            {
                UpdateDcdIdleWin(win_id);
                s_is_need_update=FALSE;      
            }
            break;
        case MSG_APP_OK:   
        case MSG_APP_WEB:
            if(MMIAPIDCD_IsIdleDisplay())
            {
                MMIAPIDCD_ExternalOpenWin(FALSE);
            }
            else
            {
                MMIAPIMENU_CreatMainMenu();
            }
            break;
            
        default :
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}
/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
LOCAL void UpdateDcdIdleWin(MMI_WIN_ID_T  win_id)
{
    if(MMIAPIDCD_DisplayIsValid())
    {
        if(MMIAPIDCD_IsIdleDisplay())
        {
            MMIDCD_UpdateIdleWin();
        }
    }
    else
    {
        MMIDCD_IdleWinWakeupInd(win_id);
    }    	
}
/*****************************************************************************/
//  Description :add list item 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(
                                     MMI_CTRL_ID_T       ctrl_id,
                                     const wchar*        wstr_ptr,
                                     uint16              wstr_len,
                                     uint16              pos
                                     )
{
    GUILIST_ITEM_T      item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0}; /*lint !e64*/
    uint16   i                    = 0;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;

    item_t.item_data_ptr = &item_data;
     
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    
    item_data.item_content[i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[i].item_data.text_buffer.wstr_len = wstr_len;
    
    item_data.item_content[i].item_data.text_buffer.wstr_ptr = wstr_ptr; /*lint !e605*/
    
    
    GUILIST_AppendItem( ctrl_id, &item_t );
}
/*****************************************************************************/
//  Description :mms setting link list choose win handle fun
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLinkListWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMI_RESULT_E err_code                                = MMI_RESULT_TRUE;
    uint8  link_index                                    = 0;
    uint16 index                                         = 0;
    uint16 mmidcd_default_index                          = 0;
    uint16 item_num                                      = 0;
    MMI_CTRL_ID_T    link_list_ctr                       = MMIDCD_LINKLIST_CTRL_ID;

    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        item_num = MMIAPICONNECTION_GetLinkSettingNum(MMIAPIDCD_GeSettingInfo()->sim_slected);
        GUILIST_SetMaxItem(link_list_ctr, item_num, FALSE);

        for(index = 0; index < MMIAPICONNECTION_GetLinkSettingNum(MMIAPIDCD_GeSettingInfo()->sim_slected); index++) 
        {
            link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIAPIDCD_GeSettingInfo()->sim_slected,index);
            
            AppendOneLineTextListItem(link_list_ctr, link_setting_ptr->name, link_setting_ptr->name_len, index);                
            
        } 
        mmidcd_default_index = MMIDCD_Setting_GetNetSettingIndex(MMIAPIDCD_GeSettingInfo()->sim_slected);
        GUILIST_SetSelectedItem(link_list_ctr,mmidcd_default_index,TRUE);
        GUILIST_SetCurItemIndex(link_list_ctr,mmidcd_default_index);	
        MMK_SetAtvCtrl(win_id, link_list_ctr);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //todo  根据选择的配置，设置默认dcd连接参数，保存全局的连接index
        link_index = GUILIST_GetCurItemIndex(link_list_ctr);
        GUILIST_SetSelectedItem(link_list_ctr, link_index, TRUE);
        MMIDCD_Setting_SetNetSettingIndex(MMIAPIDCD_GeSettingInfo()->sim_slected,link_index);
        //提示完成设置
        MMIPUB_OpenAlertSuccessWin( TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭窗口
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_OpenLinkListWin(void)
{   
    MMK_CreateWin((uint32*)MMIDCD_LINK_LIST_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description : 处理DCD开关的函数
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 请统一调用此函数处理DCD的开关操作
/*****************************************************************************/
PUBLIC void MMIDCD_HandleDCDSwitch(
                                   BOOLEAN to_Flag,         // 开关DCD
                                   BOOLEAN is_reset_factory // 是否是恢复出厂设置
                                   )
{

  BOOLEAN need_update = FALSE;
  // 如果要打开DCD
  if(to_Flag)
  {
      // MMIDCD_CreateIdleCtrl函数内部有判断，当控件存在时，则返回FALSE
      if (MMIDCD_CreateIdleCtrl(MAIN_IDLE_WIN_ID))
      {
          DCD_AddTabsAndChildWindows(MMIDCD_IDLEWIN_TAB_CTRL_ID, MAIN_IDLE_WIN_ID);
      }
      // 如果当前DCD没有打开，则发起更新
      if((!MMIAPIDCD_GeSettingInfo()->is_dcd_open)&&MMIAPIDCD_GeSettingInfo()->is_auto_update)
      {
         need_update = TRUE;
      }
      // 如果是恢复出厂设置，并且DCD默认是打开的....
      else if(MMIAPIDCD_GeSettingInfo()->is_dcd_open && is_reset_factory)
      {
         MMIDCD_UpdateStart(HTTP_REQ_TYPE_START);   // dcd 发起同步
      }

	
 #if defined(MMIWIDGET_SUPPORT) 
    		if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
	MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_DCD_ID, FALSE, TRUE);
#endif

  }
  else
  {
      if(!MMIDCD_IsIdle())
      {
          DCD_Sendsig(DCD_SIG_DL_CANCLE_REQ,HTTP_REQ_TYPE_NULL); 
      }  
      
      // 如果控件已经存在，
      if(NULL != MMK_GetCtrlPtr(MMIDCD_IDLEWIN_TAB_CTRL_ID))
      {
#if defined MMI_SMART_IDLE_SUPPORT
            MMIAPIIDLE_DCDFocusToShortcut();
#endif
          GUITAB_DeleteAllItems(MMIDCD_IDLEWIN_TAB_CTRL_ID);
          MMK_DestroyControl(MMIDCD_IDLEWIN_TAB_CTRL_ID);
          
          MMK_CloseWin(MMIDCD_IDLE_CHILD_WIN_ID);
          MMK_CloseWin(MMIDCD_MAIN_WIN_ID);
          MMK_CloseWin(MMIDCD_ITEM_CHILD_WIN_ID);
          MMK_CloseWin(MMIDCD_SETTING_MENU_WIN_ID);

	 #if defined(MMIWIDGET_SUPPORT) 
    		if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
		MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_DCD_ID);
	#endif


      }
      
  }
  // 恢复出厂设置时不需要再去记录dcd的状态....
  if(!is_reset_factory)
  {
      MMIAPIDCD_GeSettingInfo()->is_dcd_open = to_Flag;
      MMIAPIDCD_WriteSettingToNv();
  }
  
  if(need_update)
  {
      // 要把dcd 的状态改变放在前面， 否则在发起update时,会因为dcd 的开关是关闭的，而不会向下层发更新的请求...
      MMIDCD_UpdateStart(HTTP_REQ_TYPE_START);   // dcd 发起同步
  }
}
/*****************************************************************************/
//  Description : the process message function of the dcd reset function
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 此函数供恢复DCD的出厂设置调用....
/*****************************************************************************/
LOCAL MMI_RESULT_E  ResetDCDApplet_HandleEvent(    
                                               IAPPLET_T*          iapplet_ptr,
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    CAF_APPLET_T* dcd_applet_ptr = (CAF_APPLET_T*)iapplet_ptr;
    //SCI_TRACE_LOW:"ResetDCDApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_6075_112_2_18_2_13_6_292,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
        case MSG_APPLET_START:
            {
                //SCI_TRACE_LOW:"ResetDCDApplet_HandleEventMSG_APPLET_START"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_6081_112_2_18_2_13_6_293,(uint8*)"");
                // 如果当前没有处在更新中，则发起动作； 如果在更新中，则发起取消更新的请求,
                // 收到取消成功的消息后,再发起动作
                if(MMIDCD_IsIdle())
                {
                    MMIDCD_RestoreFactorySetting();
                    MMK_CloseApplet(dcd_applet_ptr->app_handle);
                }
                else
                {
                    DCD_Sendsig(DCD_SIG_DL_CANCLE_REQ, HTTP_REQ_TYPE_NULL);
                }
            }
            break;

        // 只有在更新中发起的恢复出厂设置请求才会走到这里来......
        case MSG_SET_RESET_FACTORY_OVER_IND:
	     //SCI_TRACE_LOW:"[MMIDCD]: MSG_SET_RESET_FACTORY_OVER_IND received!"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_6098_112_2_18_2_13_6_294,(uint8*)"");
            MMIDCD_RestoreFactorySetting();
            MMK_CloseApplet(dcd_applet_ptr->app_handle);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_ResetDCDStart(void)
{
    MMI_APPLET_START_T start ={0};
    
    start.guid = SPRD_DCD_RESET_APPLET_ID;
    
    MMK_StartApplet( &start );
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 此函数专供手动更新成功时，提示用户更新完成, 其他如手动更新失败，
//        zhuo 的indiciotionshow 处理, 其他模式非手动更新，不需提示
/*****************************************************************************/
PUBLIC void MMIDCD_ShowUpdateComplete(
                                      MMIDCD_REQ_TYPE_E    req_type, 
                                      MMIDCD_ERROR_CODE_E  error_code
                                      )
{
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T query_win_id = MMIDCD_INDICATION_WIN_ID;
    MMI_WIN_ID_T    win_id = 0; 
    
    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_ShowUpdateCompelete req_type = %d, error_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_6136_112_2_18_2_13_6_295,(uint8*)"dd", req_type, error_code);
    
    if(HTTP_REQ_TYPE_MAN != req_type || DCD_ERROR_NULL != error_code)
    {
        //SCI_TRACE_LOW:"[MMIDCD]: this not need process "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_6140_112_2_18_2_13_6_296,(uint8*)"");
        return;
    }

    if(MMIAPIDCD_DcdIsOpen())
    {  
        MMK_CloseWin(query_win_id);
        MMK_CloseWin(MMIDCD_MAIN_QUERY_WIN_ID);
        
        win_id = MMK_GetFocusWinId();
        
        if((win_id >= MMIDCD_MAIN_WIN_ID) && (win_id < MMIDCD_NULL_WIN_ID) )
        {
            win_priority = WIN_ONE_LEVEL;
        }
        
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_DCD_UPDATE_COMPLEPTE, TXT_NULL, IMAGE_PUBWIN_SUCCESS, &query_win_id,&win_priority, MMIPUB_SOFTKEY_ONE,PNULL);
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note:
/*****************************************************************************/
LOCAL void SetDataChangeFlag(BOOLEAN flag)
{
    s_data_has_changed = flag;
    //SCI_TRACE_LOW:"MMIDCD s_data_has_changed = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_6166_112_2_18_2_13_6_297,(uint8*)"d", s_data_has_changed);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDataChangeFlag(void)
{
    //SCI_TRACE_LOW:"MMIDCD GetDataChangeFlag s_data_has_changed = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_WINTAB_6174_112_2_18_2_13_6_298,(uint8*)"d", s_data_has_changed);
    return s_data_has_changed;
}

/*****************************************************************************/
//  Description : 设置更新取消菜单是否显示
//  Global resource dependence : 
//  Author:koki.gao
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_SetUpdateStopFlag(BOOLEAN flag)
{
    s_ifcan_cancel_update = flag;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:koki gao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_GetUpdateStopFlag(void)
{
    return s_ifcan_cancel_update;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_ENABLE_DCD*/




/*Edit by script, ignore 1 case. Thu Apr 26 19:00:52 2012*/ //IGNORE9527


/*Edit by script, ignore 12 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
