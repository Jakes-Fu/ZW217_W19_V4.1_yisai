/*****************************************************************************
** File Name:      mmimbbms_wintab.c                                                   *
** Author:                                                                   *
** Date:              29/06/2009                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 29/06/2009           Create
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"

#include "mmi_app_mobiletv_trc.h"
#ifdef MBBMS_SUPPORT
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guires.h"
#include "guiedit.h"
#include "guiwin.h"

#include "mmimtv_id.h"
#include "mmimtv_main.h"
#include "mmimtv_position.h"
#include "mmimtv_image.h"
#include "mmimtv_text.h"
#include "mmitheme_pos.h"
#include "mmimtv_menutable.h"
#include "mmimtv_nv.h"
#include "mmimtv_osd_display.h"

#include "mmi_appmsg.h"
#include "mmimtv_export.h"
#include "guirichtext.h"
#include "mmimbbms_main.h"
#include "mmimbbms_wintab.h"
#include "guidropdownlist.h"

#include "sci_types.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmisms_image.h"
#include "mbbms_sg_common.h"
#include "mbbms_sg_api.h"
#include "mmimbbms_main.h"
#include "mmi_position.h"
#include "guitab.h"
#include "guibutton.h"
#include "guitext.h"
#include "gui_ucs2b_converter.h"
#include "dal_time.h"
#include "chng_freq.h "
#include "mmipb_text.h"
#include "mmiset_text.h"
#include "mmiconnection_export.h"
#include "mmialarm_service.h"
#include "mmiidle_export.h"
//#include "mmieng_win.h"
#include "mmieng_export.h"
//#include "mmiaudio_ctrl.h"
#include "mua_cmd.h"
#include "mmi_image.h"
#include "guiform.h"

#include "mtv_api.h"
#include "mmimtv_data.h"


/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

//extern BOOLEAN MUACMD_MBBMS_IsEnabled(void);

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8     s_timer_id = 0;
LOCAL BOOLEAN   timer_master_query = TRUE;
LOCAL uint8     s_switch_mbbms_timer_id = 0;

LOCAL const GUI_LCD_DEV_INFO s_mbbms_lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };

LOCAL MMIMBBMS_OBJECT_FROM_TYPE_E s_object_type = MMIMBBMS_OBJECT_FROM_NONE;

LOCAL const uint32 s_day_textid[M_MMI_MBBMS_MAX_DAY_NUM] =
{
    TXT_MBBMS_TODAY,
    TXT_MBBMS_TOMMORROW,
    TXT_MBBMS_AFTER_TOMMORROW
};


LOCAL const uint32 s_content_type[M_MMI_MBBMS_MAX_CONTENT_TYPE_NUM] =
{
    TXT_MBBMS_FINANCE,
    TXT_MBBMS_SPORTS,
    TXT_MBBMS_FASHION,
    TXT_MBBMS_TALK,
    TXT_MBBMS_TV,
    TXT_MBBMS_MOVIE,
    TXT_COMMON_MUSIC
};

LOCAL const MMI_TEXT_ID_T s_mbbms_weekday_info[] ={ 
                                          TXT_SUNDAY, 
                                          TXT_MONDAY,
                                          TXT_TUESDAY, 
                                          TXT_WEDNESDAY,
                                          TXT_THURSDAY, 
                                          TXT_FRIDAY,
                                          TXT_SATURDAY};

LOCAL const MMI_IMAGE_ID_T  s_eb_grade_icon[MMIMTV_BROADCAST_MAX_LEVEL] =
{
	IMAGE_MTV_EB_NUMBER_1,
	IMAGE_MTV_EB_NUMBER_2,
	IMAGE_MTV_EB_NUMBER_3,
	IMAGE_MTV_EB_NUMBER_4
};

LOCAL uint16       s_content_idx = 0;
LOCAL uint16       s_day_index   = 0;

/* added by jhpeng for plum.peng_cr157069 begin */
LOCAL uint16       s_main_service_index = 0;
LOCAL uint16       s_main_service_count = 0;
/* added by jhpeng for plum.peng_cr157069 end */

LOCAL int          s_radio_index = 0;
LOCAL wchar        s_wchar_search[300] = {0};
//LOCAL uint16       s_search_date_index = 0;

// 用于判断是节目单窗口还是搜索结果窗口打开了节目详情窗口
//LOCAL BOOLEAN      s_program_list_open_it = FALSE;
LOCAL MBBMSSG_CONTEXT_COMPLEX_SEARCH_T s_complex_context = {0};
LOCAL SG_LIST_PTR_T s_search_list_ptr = PNULL;

//标志startup window是否是异常关闭（异常主要指来中断事件导致的窗口ReturnToIdle情形）
LOCAL BOOLEAN      s_is_startupwin_abnormal_close = TRUE;

//msk update timer
#define MMIMBBMS_MSK_UPDATE_UNIT    (60 * 1000)
//LOCAL uint8 s_msk_timer_id = 0;

//yexiong.liu 标识套餐订购管理中是否从已订购套餐进入套餐详情界面
LOCAL BOOLEAN s_is_enter_detail_from_purchased_item = FALSE;

//mbbms2.0 MTVSERVICE频道列表数据
LOCAL MTVSERVICE_PROGRAM_LIST_T *s_program_list_ptr = PNULL;
//节目单中记录当前播放频道的index值，以进入节目单时高亮当前正在播放频道
LOCAL uint16 s_playing_content_index = 0;
//标识是否从播放中进入套餐订购管理
LOCAL BOOLEAN s_is_enter_purchase_from_play = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : MMIMBBMS_HandleSearchMainMsgWin
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleSearchMainMsgWin(
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM           param);

/*****************************************************************************/
// 	Description : MMIMBBMS_OpenSearchWaitWin
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenSearchWaitWin(void);

/*****************************************************************************/
// 	Description : HandleSearchWaitWinMsg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSearchWaitWinMsg( 
                                          MMI_WIN_ID_T      win_id, 
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );

// Description : HandleSettingWinMsg
// Global resource dependence : none
// Author:
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param);

/*****************************************************************************/
// 	Description : 增加主界面Tab信息 
//	Global resource dependence : 
//  Author: xin.li
//	Note:   
/*****************************************************************************/
LOCAL void MainWinAppendTabInfo(void);

/*****************************************************************************/
// 	Description : HandleSearchResultMainWinMsg
//	Global resource dependence : 
//  Author:        
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSearchResultMainWinMsg( 
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param);

/*****************************************************************************/
// 	Description : MMIMBBMS_LoadSearchRstList
//	Global resource dependence : 
//  Author:        
//	Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_LoadSearchRstList(SG_LIST_PTR_T list_ptr,
                                      uint32 left_index, uint32 right_index, BOOLEAN is_load_data);

/*****************************************************************************/
//  Description : HandleSearchRstOptMenuWinMsg
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSearchRstOptMenuWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM                param);

/*****************************************************************************/
// 	Description : handle create new connection set info win msg
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlenewconnectionHelpInfoWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );
/*****************************************************************************/
//     Description : handle message of purchase item detail window msg
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandlePurchaseItemDetailWinMsg(
                                                           MMI_WIN_ID_T    win_id, 
                                                           MMI_MESSAGE_ID_E    msg_id, 
                                                           DPARAM                param);

/*****************************************************************************/
//  Description : MMIMBBMS_HandlePurchaseOrderWinMsg
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandlePurchaseOrderWinMsg(
                                                      MMI_WIN_ID_T      win_id, 
                                                      MMI_MESSAGE_ID_E  msg_id, 
                                                      DPARAM            param);

/*****************************************************************************/
//     Description : handle message of subscrible list window
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenPurchaseOrderQueryWin(void);

/*****************************************************************************/
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnSubscribeRequestWinMsg( 
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  );

/*****************************************************************************/
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubscribeRequestWinMsg( 
                                                MMI_WIN_ID_T     win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param
                                                );

/*****************************************************************************/
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSubscribeQueryWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUnSubscribeQueryWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 );

/*****************************************************************************/
//     Description : handle message of progrm list window msg
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleProgramListWinMsg(
                                                    MMI_WIN_ID_T     win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM           param);

/*****************************************************************************/
//     Description : handle message of program list child window
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleProgramListChildWinMsg(
                                                         MMI_WIN_ID_T    win_id, 
                                                         MMI_MESSAGE_ID_E    msg_id, 
                                                         DPARAM                param);

/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void  MMIMBBMS_AddTabsAndChildWindows(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_GetServiceNodeByIdx
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL SG_LIST_PTR_T MMIMBBMS_GetServiceNodeByIdx(uint16 index);

/*****************************************************************************/
//  Description : handle program list option menu win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProgramListOptMenuWinMsg(
                                                   MMI_WIN_ID_T        win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM                param
                                                   );

/*****************************************************************************/
//  Description : HandleProgramDetailOptMenuWinMsg
//  Global resource dependence : 
//  Author:liming.deng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProgramDetailOptMenuWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM              param);

/*****************************************************************************/
//  Description : MMIMBBMS_GetContentNodeByIdx
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SG_LIST_PTR_T MMIMBBMS_GetContentNodeByIdx(
                                                 uint16 service_idx,
                                                 uint16 content_idx,
                                                 SG_LIST_PTR_T *contentIdList_head_ptr);

/*****************************************************************************/
//     Description : handle message of program list child window
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleProgramDetailWinMsg(
                                                      MMI_WIN_ID_T    win_id, 
                                                      MMI_MESSAGE_ID_E    msg_id, 
                                                      DPARAM                param);

LOCAL MMI_RESULT_E LeftButtonCallBack(void);

LOCAL MMI_RESULT_E RightButtonCallBack(void);

/*****************************************************************************/
//  Description : HandleMainInterfaceTabWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面TAB父窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainInterfaceTabWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param);

/*****************************************************************************/
//  Description : HandleAllServiceChildWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面所有频道子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllServiceChildWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param);

/*****************************************************************************/
//  Description : HandleAllFavoriteServiceChildWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面收藏频道子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceChildWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param);

/*****************************************************************************/
//  Description : HandleAllFavoriteServiceChildWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面收藏节目子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmContentChildWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param);

/*****************************************************************************/
//  Description : get name info from struct languageString
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetNameWcharByLangString(languageString      *name_ptr, //in
                                       MTV_SERVICE_MODE_E mode,     //service mode
                                       uint32 char_set,             //char_set while cmmb mode
                                       uint32 service_id,           //cmmb mode used while service name null
                                       wchar  *wchar_name,           //out
                                       uint16 wstr_len);

/*****************************************************************************/
// 	Description : 创建所有频道列表
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void CreateAllServiceList(void);

/*****************************************************************************/
// 	Description : 创建收藏频道列表
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void CreateFavoriteServiceList(void);

/*****************************************************************************/
// 	Description : 创建收藏节目列表
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void CreateAlarmContentList(void);

/*****************************************************************************/
// 	Description : 添加一个频道显示节点
//	Global resource dependence : 
//  Author: 
//	Note: 针对所有频道/收藏频道
/*****************************************************************************/
LOCAL void AddServiceItemToListboxCtrl(MMI_CTRL_ID_T    ctrl_id,
                                       MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T* node_ptr);

/*****************************************************************************/
// 	Description : 添加一个收藏节目显示节点 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void AddContentItemToListboxCtrl(MMI_CTRL_ID_T    ctrl_id,
                                       MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T* node_ptr,
                                       anyURI* pURI );

/*****************************************************************************/
// 	Description : 从MBBMSSG_SERVICE_INDEX_T中获取相关信息
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL SG_ERROR_E GetInfoFromServiceIndex(
                                         const anyURI*  index_ptr,   //[in]
                                         MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T* info_node_ptr   //[out]
                                         );

/*****************************************************************************/
// 	Description : 从MBBMSSG_SERVICE_T中获取相关信息
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void GetInfoFromService(anyURI* pServiceID,
                              const MBBMSSG_SERVICE_T*  service_ptr,         //[in]
                              MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T* info_node_ptr   //[out]
                              );

/*****************************************************************************/
// 	Description : 从MBBMSSG_SERVICE_T中获取节目相关信息
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void GetInfoFromContent(
                              MBBMSSG_CONTENT_T*  content_ptr,         //[in]
                              MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T* info_node_ptr   //[out]
                              );

/*****************************************************************************/
// 	Description : 显示空列表 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void SetListboxEmpty(MMI_CTRL_ID_T    ctrl_id);

/*****************************************************************************/
// 	Description : handle service list option window msg
//	Global resource dependence : none
//  Author: 
//	Note: 频道列表选项, 第二项为"收藏"
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllServiceOptWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,        
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : handle favorite service list option window msg
//	Global resource dependence : none
//  Author: 
//	Note: 频道列表选项, 第二项为"收藏"
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFavoriteServiceOptWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,        
                                                   DPARAM param
                                                   );

/*****************************************************************************/
// 	Description : handle favorite content list option window msg
//	Global resource dependence : none
//  Author: 
//	Note: 频道列表选项, 第二项为"收藏"
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmContentOptWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,        
                                                   DPARAM param
                                                   );

/*****************************************************************************/
// 	Description : insert dyna menu node by label
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void InsertMenuNode(
                          MMI_CTRL_ID_T      ctrl_id,         //控件id
                          MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                          uint16          	node_id,        //节点编号
                          uint16          	parent_node_id, //父节点编号
                          uint16          	index           //位置
                          );

/*****************************************************************************/
// 	Description : MMIMBBMS_LoadListItem
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_LoadListItem(uint16 index, SG_DATETIME_T* pdate);

/*****************************************************************************/
// 	Description : handle watch connecting win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWatchConnectingWinMsg( 
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );

/*****************************************************************************/
// 	Description : HandleSubscriblePromptWinMsg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubscriblePromptWinMsg( 
                                                MMI_WIN_ID_T     win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param
                                                );

/*****************************************************************************/
LOCAL void MBBMS_AddAllServiceToDropdownList(void);

/*****************************************************************************/
//     Description : Load all date to dropdown list ctrl
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MBBMS_AddAllDateToDropdownList(void);

/*****************************************************************************/
//     Description : MMIMBBMS_MergeDateString
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_MergeDateString(SCI_DATE_T* pdate, wchar* pwchar);

/*****************************************************************************/
// Description : Load all content type to dropdown list ctrl
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void MBBMS_AddAllContentTypeToDropdownList(void);

/*****************************************************************************/
// Description : MMIMBBMS_GetStartEndTimeString
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetStartEndTimeString(MBBMSSG_CONTENT_T* pContent, wchar* pwchar);

/*****************************************************************************/
// Description : get start/end time info from content index struct
// Global resource dependence : none
// Author: 
// Note: 从节目分片结构中获取start/end信息
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetStartEndTimeStringFromContentIndex(MBBMSSG_CONTENT_INDEX_T* pContentIndex, wchar* pwchar);

/*****************************************************************************/
// Description : get cmmb content start/end time info
// Global resource dependence : none
// Author: xin.li
// Note: 从CMMB节目结构中获取start/end信息
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetCmmbStartEndTimeString(MMIMTV_PROGRAMME_T* cmmb_prog_ptr,
                                               wchar* pwchar
                                               );

/*****************************************************************************/
// Description : get cmmb content name
// Global resource dependence : none
// Author: xin.li
// Note: 从CMMB节目结构中获取节目名
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetCmmbContentNameString(MMIMTV_PROGRAMME_T* cmmb_prog_ptr,
                                              wchar* wchar_name,
                                              uint16 wstr_len
                                              );

/*****************************************************************************/
// 	Description : Handle setting save promt win msg
//	Global resource dependence : none
//  Author: lisa lin	
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSavePromptWinMsg( 
                                                 MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 );

/*****************************************************************************/
// 	Description : Handle setting cancel promt win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSettingCancelPromptWinMsg( 
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM              param
                                                   );

/*****************************************************************************/
//     Description : MMIMBBMS_UpdateContentList
//    Global resource dependence : 
//  Author:       peng
//    Note:   
/*****************************************************************************/
LOCAL void MMIMBBMS_UpdateContentList(void);

/*****************************************************************************/
// Description : whether current service in all_service_list is encrypt
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCurrServiceInAllTabEncrypted(uint16 index);

/*****************************************************************************/
// Description : MMIMBBMS_AddPurchaseDetailHead
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCurrServiceInFavoriteTabEncrypted(void);

/*****************************************************************************/
// Description : MMIMBBMS_AddPurchaseDetailHead
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCurrContentInFavoriteTabEncrypted(void);

/*****************************************************************************/
// 	Description : handle eb list win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEBListWinMsg( 
                                      MMI_WIN_ID_T     win_id, 
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM              param
                                      );

/*****************************************************************************/
// 	Description : handle eb list opt win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEBListOptWinMsg( 
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );

/*****************************************************************************/
// 	Description : handle eb detail win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEBDetailWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : handle startup win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelpInfoWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : handle sim card select win msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimSelectWinMsg( 
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );

/*****************************************************************************/
// 	Description : whether setting item is modified
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsSettingItemModified(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIMBBMS_GetRealStartTime
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_GetRealStartTime(SCI_TM_T* pSrcTime, uint16 remindertime,
                                     MMI_TM_T* pDstTime);

/*****************************************************************************/
// 	Description : handle Subscription Query win  msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubcriptionUpdateQueryWinMsg( 
                                                      MMI_WIN_ID_T     win_id, 
                                                      MMI_MESSAGE_ID_E    msg_id, 
                                                      DPARAM              param
                                                      );

/*****************************************************************************/
// 	Description : handle cmmb mode warning win msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBModeWarningWinMsg( 
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );

/*****************************************************************************/
// 	Description : handle switch to mbbms query win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchToMBBMSQueryWinMsg( 
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  );

/*****************************************************************************/
// 	Description : open switch waithing win
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void OpenSwitchWaitingWin(void);

/*****************************************************************************/
// 	Description : handle switch waiting win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchWaithingWinMsg( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );

/*****************************************************************************/
// 	Description : handle setting save waiting win msg
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSavewaitingWinMsg( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              );
/*****************************************************************************/
// 	Description : close related win while swithing to mbbms mode
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void CloseRelatedWinWhenSwtich(void);

/*****************************************************************************/
// 	Description : start swith to mbbms process
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartSwitchToMBBMSProcess(void);

/*****************************************************************************/
// 	Description : handle switch mbbms timer
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSwitchToMBBMSTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
// 	Description : handle sg update request win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenSGUpdateRequestWin(void);

/*****************************************************************************/
// 	Description : handle sg update request win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSGUpdateRequestWinMsg( 
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );

/*****************************************************************************/
// 	Description : enter play from service tab
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromAllServiceTab(void);

/*****************************************************************************/
// 	Description : Enter Play in favorte service tab
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromFavoriteServiceTab(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromProgramListOpt(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromProgramList(void);

/*****************************************************************************/
// 	Description : Enter Play in favorte content tab
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
//LOCAL void MMIMBBMS_EnterOpenOrPlayFromAlarmContentTab(void);

/*****************************************************************************/
// 	Description : Enter Play in favorte content tab
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromProgramDetail(void);

/*****************************************************************************/
// 	Description : Enter Play in favorte content tab
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromSearchResult(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_EnterOpenOrPlayFromSearchResultOpt
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromSearchResultOpt(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_HandleSubscribeTypeWinMsg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
// LOCAL MMI_RESULT_E MMIMBBMS_HandleSubscribeTypeWinMsg(
//                                                       MMI_WIN_ID_T     win_id, 
//                                                       MMI_MESSAGE_ID_E msg_id, 
//                                                       DPARAM           param);
/*****************************************************************************/
// 	Description : handle subscription update request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubscriptionUpdateRequestWinMsg( 
                                                         MMI_WIN_ID_T     win_id, 
                                                         MMI_MESSAGE_ID_E    msg_id, 
                                                         DPARAM              param
                                                         );

/*****************************************************************************/
//  Description : MMIMBBMS_ShowFavoriteMsg
//  Global resource dependence : 
//  Author:liming.deng
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_ShowFavoriteMsg(anyURI* pGlobalId, MMIMBBMS_TYPE favoriteType, 
                                    MMIMBBMS_OPER operationType);

/*****************************************************************************/
//  Description : HandleServicePauseOptMenuWinMsg
//  Global resource dependence : 
//  Author:liming.deng
//  Note: 
/*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleServicePauseOptMenuWinMsg(
//                                                     MMI_WIN_ID_T        win_id, 
//                                                     MMI_MESSAGE_ID_E    msg_id, 
//                                                     DPARAM                param
//                                                     );

/*****************************************************************************/
// 	Description : handle Account Inquiry request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAccountInquiryRequestWinMsg( 
                                                     MMI_WIN_ID_T     win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM              param
                                                     );

/*****************************************************************************/
// Description : 根据套餐类型(本地套餐或已订套餐)和index(是指在本地套餐或已订套餐列表中的index)
//               得到对应的purchase item在所有套餐列表中对应的全局 index
// Global resource dependence : none
// Author:
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetGlobalIndexFromTwoIndex(uint32 purchase_type,
                                                  uint16 inner_index,
                                                  uint32* pGlobalIndex);

/*****************************************************************************/
//  Description : MMIMBBMS_RefreshMiddleSoftkey
//  Global resource dependence : none
//  Author: plum.peng
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_RefreshMainWinMiddleSoftkey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : MMIMBBMS_RefreshMiddleSoftkey
//  Global resource dependence : none
//  Author: plum.peng
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_RefreshProgListMiddleSoftkey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : 切换前一个或后一个频道
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
//LOCAL void MMIMBBMS_ChangeService(BOOLEAN is_next);
/*****************************************************************************/
// 	Description : 增加套餐管理Tab信息 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void MMIMBBMS_SubscribeManageWinAppendTabInfo(void);

/*****************************************************************************/
//  Description : handle message of subscrible list window
//  Global resource dependence : none
//  Author:
//  Note: 套餐订购管理 列表窗口，包含本地套餐、已订套餐两项
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleSubscribeTabWinMsg(
                                                      MMI_WIN_ID_T     win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM           param);
/*****************************************************************************/
//  Description : MMIMBBMS_HandlePurchasedItemChildWinMsg
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note: 已订购套餐子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandlePurchasedItemChildWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param);

/*****************************************************************************/
//  Description : MMIMBBMS_HandleLocalPurchaseItemChildWinMsg
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note: 本地套餐子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleLocalPurchaseItemChildWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param);

/*****************************************************************************/
//  Description : handle message of update list window
//  Global resource dependence : none
//  Author:yexiong.liu
//  Note: 刷新界面，包含自动刷新和指定城市刷新两项
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleUpdateMainWinMsg(
                                                      MMI_WIN_ID_T     win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM           param);

/*****************************************************************************/
//  Description : open update win
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenUpdateListMainWin(void);

/*****************************************************************************/
//     Description : handle message of update from city name win
//    Global resource dependence : none
//    Author:yexiong.liu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleUpdateFromCityNameWinMsg(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM                param);

/*****************************************************************************/
//     Description : set softkey of alert win for save favorite service win
//    Global resource dependence : none
//    Author:  xiaoxiang.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleAddorCancelFavoriteServiceAlertWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );

/*****************************************************************************/
// 	Description : handle open Purchase Operation cancel waiting win
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitMsg( MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                               );

/*****************************************************************************/
// 	Description : get the playing content index
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetPlayingContentIndex(MBBMSSG_CONTENT_INDEX_T* content_index_ptr);

/*****************************************************************************/
// 	Description : get the content type from genre and add it to the droplist
//	Global resource dependence : 
//  Author:       yexiong.liu
//	Note:   
/*****************************************************************************/
LOCAL void MBBMS_AddAllContentTypeToDropdownListNew(void);


/*---------------------------------------------------------------------------*/
/*                          WINDOW TABLE DECLARE                             */
/*---------------------------------------------------------------------------*/
/*Subscribe wintable start*/
LOCAL WINDOW_TABLE(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB) = 
{
    WIN_FUNC((uint32)MMIMBBMS_HandleSubscribeTabWinMsg),
    WIN_ID( MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID ),
    WIN_STATUSBAR,
    //WIN_TITLE(TXT_MBBMS_PURCHASE_MANAGE),
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_COMMON_TITLE, MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID),
    END_WIN
};


LOCAL WINDOW_TABLE(MMIMBBMS_PURCHASEDITEM_LIST_CHILD_WIN_TAB) =
{
    WIN_FUNC((uint32)MMIMBBMS_HandlePurchasedItemChildWinMsg),
    WIN_ID(MMIMBBMS_PURCHASEDITEM_CHILD_WIN_ID),
    WIN_SOFTKEY(TXT_MBBMS_UNSUBSCRIBE, TXT_MBBMS_COMBO_DETAIL, STXT_CANCEL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_PURCHASEDITEM_LISTBOX_CTRL_ID),
    END_WIN    
};

LOCAL WINDOW_TABLE(MMIMBBMS_LOCAL_PURCHASEITEM_CHILD_WIN_TAB) =
{
    WIN_FUNC((uint32)MMIMBBMS_HandleLocalPurchaseItemChildWinMsg),
    WIN_ID(MMIMBBMS_LOCAL_PURCHASEITEM_CHILD_WIN_ID),
    WIN_SOFTKEY(TXT_MBBMS_SUBSCRIBE, TXT_MBBMS_COMBO_DETAIL, STXT_CANCEL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_LOCAL_PURCHASEITEM_LISTBOX_CTRL_ID),
    END_WIN    
};

LOCAL WINDOW_TABLE(MMIMBBMS_PURCHASEITEM_ORDER_TAB) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandlePurchaseOrderWinMsg),
    WIN_ID(MMIMBBMS_PURCHASE_ORDER_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_MBBMS_PURCHASE_ORDER),  
    WIN_SOFTKEY(TXT_MBBMS_PURCHASE_SUBMIT_ORDER, TXT_NULL, STXT_RETURN),    
    CREATE_RICHTEXT_CTRL(MMIMBBMS_PURCHASE_ORDER_RICHTEXT_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_STARTUP_WIN_TAB) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandleStartUpWinMsg),
    WIN_ID(MMIMBBMS_STARTUP_WIN_ID),
    WIN_STATUSBAR,
//    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMBBMS_STARTUP_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMBBMS_ANIM_START_CTRL_ID,MMIMBBMS_STARTUP_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMBBMS_ANIM_PROGRESS_CTRL_ID,MMIMBBMS_STARTUP_FORM_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE,MMIMBBMS_TEXT_START_CTRL_ID,MMIMBBMS_STARTUP_FORM_CTRL_ID),
    WIN_STYLE(WS_DISABLE_HWICON),
    WIN_SOFTKEY(TXT_NULL, TXT_COMMON_COMMON_ENTER, STXT_RETURN),    
    END_WIN
};


LOCAL WINDOW_TABLE(MMIMBBMS_SEARCH_MAIN_TAB) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandleSearchMainMsgWin),
    WIN_ID(MMIMBBMS_SEARCH_MAIN_WIN_ID),
    WIN_TITLE(TXT_MBBMS_SEARCH_PROGRAM),
    WIN_SOFTKEY(STXT_SEARCH, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMBBMS_SEARCH_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_KEYWORD_FORM_CTRL_ID,MMIMBBMS_SEARCH_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMBBMS_LABEL_KEYWORD_CTRL_ID,MMIMBBMS_KEYWORD_FORM_CTRL_ID),
	    CHILD_EDIT_TEXT_CTRL(TRUE,MMIMBBMS_MAX_KEYWORD_LENS,MMIMBBMS_EDITBOX_KEYWORD_CTRL_ID,MMIMBBMS_KEYWORD_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_CHANNEL_FORM_CTRL_ID,MMIMBBMS_SEARCH_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMBBMS_LABEL_CHANNEL_CTRL_ID,MMIMBBMS_CHANNEL_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIMBBMS_DROPDOWNLIST_CHANNEL_CTRL_ID,MMIMBBMS_CHANNEL_FORM_CTRL_ID),
 
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_DATE_FORM_CTRL_ID,MMIMBBMS_SEARCH_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMBBMS_LABEL_DATE_CTRL_ID,MMIMBBMS_DATE_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIMBBMS_DROPDOWNLIST_DATE_CTRL_ID,MMIMBBMS_DATE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_PROGRAM_TYPE_FORM_CTRL_ID,MMIMBBMS_SEARCH_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMBBMS_LABEL_PROGRAM_TYPE_CTRL_ID,MMIMBBMS_PROGRAM_TYPE_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIMBBMS_DROPDOWNLIST_PROGRAM_TYPE_CTRL_ID,MMIMBBMS_PROGRAM_TYPE_FORM_CTRL_ID),

    END_WIN
};

LOCAL WINDOW_TABLE( MMIMBBMS_SEARCH_RESULT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSearchResultMainWinMsg ),    
    WIN_ID( MMIMBBMS_SEARCH_RESULT_WIN_ID ),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_MBBMS_PROGRAM_SEARCH_RESULT),
	WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_SEARCH_RESULT_LIST_CTRL_ID),
     END_WIN
}; 


LOCAL WINDOW_TABLE( MMIMBBMS_SETTING_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSettingWinMsg ),    
    WIN_ID( MMIMBBMS_SETTING_WIN_ID ),
    WIN_TITLE(STXT_MAIN_SETTING),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMBBMS_SETTING_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_NOTIFY_TIME_FORM_CTRL_ID,MMIMBBMS_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMBBMS_LABEL_NOTIFY_TIME_CTRL_ID,MMIMBBMS_NOTIFY_TIME_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIMBBMS_DROPDOWNLIST_NOTIFY_TIME_CTRL_ID,MMIMBBMS_NOTIFY_TIME_FORM_CTRL_ID),
 
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_NETWORK_FORM_CTRL_ID,MMIMBBMS_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMBBMS_LABEL_NETWORK_CTRL_ID,MMIMBBMS_NETWORK_FORM_CTRL_ID),      
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID,MMIMBBMS_NETWORK_FORM_CTRL_ID),      
        
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_NEW_CONNECTION_FORM_CTRL_ID,MMIMBBMS_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIMBBMS_LABEL_NEW_CONNECTION_CTRL_ID,MMIMBBMS_NEW_CONNECTION_FORM_CTRL_ID),  
        
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_NEW_CONNECTION_HELP_FORM_CTRL_ID,MMIMBBMS_SETTING_FORM_CTRL_ID), 
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIMBBMS_LABEL_NEW_CONNECTION_HELP_CTRL_ID,MMIMBBMS_NEW_CONNECTION_HELP_FORM_CTRL_ID),         
        
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_SG_DNS_FORM_CTRL_ID,MMIMBBMS_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIMBBMS_LABEL_SG_DNS_CTRL_ID,MMIMBBMS_SG_DNS_FORM_CTRL_ID),
	    CHILD_EDIT_TEXT_CTRL(TRUE,M_MMIMBBMS_MAX_SG_DOMAIN_LENS,MMIMBBMS_EDITBOX_SG_DNS_CTRL_ID,MMIMBBMS_SG_DNS_FORM_CTRL_ID),

    END_WIN
}; 
LOCAL WINDOW_TABLE(MMIMBBMS_NEW_CONNECTION_HELP_INFO_WIN_TAB) = 
{
    WIN_STATUSBAR,
    WIN_TITLE( TXT_MBBMS_NEW_CONNECTION_HELP ),
    WIN_FUNC((uint32) HandlenewconnectionHelpInfoWinMsg),    
    WIN_ID( MMIMBBMS_NEW_CONNECTION_HELP_INFO_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN ),
    
    CREATE_TEXT_CTRL(MMIMBBMS_NEW_CONNECTION_HELP_INFO_TEXTBOX_CTRL_ID ),
    END_WIN
};

LOCAL WINDOW_TABLE( MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSearchRstOptMenuWinMsg),
    WIN_ID(MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


LOCAL WINDOW_TABLE(MMIMBBMS_PURCHASEITEM_DETAIL_TAB) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandlePurchaseItemDetailWinMsg),
    WIN_ID(MMIMBBMS_PURCHASEITEM_DETAIL_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_MBBMS_COMBO_DETAIL),  
    WIN_SOFTKEY(TXT_MBBMS_SUBSCRIBE, TXT_NULL, STXT_RETURN),    
    CREATE_RICHTEXT_CTRL(MMIMBBMS_PURCHASEITEM_DETAIL_RICHTEXT_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_PROGRAM_LIST_TAB) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandleProgramListWinMsg),
    WIN_ID(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID),
    WIN_STATUSBAR,
    //WIN_TITLE(TXT_MBBMS_PROGRM_LIST),
    //WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),    
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_COMMON_TITLE, MMIMBBMS_PROGRAM_TAB_CTRL_ID),
    END_WIN
};

// 
LOCAL WINDOW_TABLE( MMIMBBMS_PROGRAM_CHILD_TAB ) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandleProgramListChildWinMsg),
    WIN_ID( MMIMBBMS_PROGRAM_CHILD_WIN_ID ),
    //WIN_BACKGROUND_ID(IMAGE_COMMON_BG),    
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_LBTN_RELEASE,MMIMBBMS_BUTTON_LEFT_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_RBTN_RELEASE,MMIMBBMS_BUTTON_RIGHT_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIMBBMS_LABEL_DAY_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE( MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleProgramListOptMenuWinMsg),
    WIN_ID(MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

LOCAL WINDOW_TABLE( MMIMBBMS_PROGRAM_DETAIL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandleProgramDetailWinMsg),
    WIN_ID(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_COMMON_PROGRAM_DETAIL),
    WIN_SOFTKEY(STXT_OPTION,TXT_NULL,STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMBBMS_PROGRAM_DETAIL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIMBBMS_PROGRAM_DETAIL_FORM1_CTRL_ID,MMIMBBMS_PROGRAM_DETAIL_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMBBMS_ANIM_DETAIL_CTRL_ID,MMIMBBMS_PROGRAM_DETAIL_FORM1_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE,MMIMBBMS_TEXT_TIME_CTRL_ID,MMIMBBMS_PROGRAM_DETAIL_FORM1_CTRL_ID),
    CHILD_TEXT_CTRL(FALSE,MMIMBBMS_TEXT_DETAIL_CTRL_ID,MMIMBBMS_PROGRAM_DETAIL_FORM_CTRL_ID),
    END_WIN
};


LOCAL WINDOW_TABLE( MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleProgramDetailOptMenuWinMsg),
    WIN_ID(MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


/*Subscribe wintable end*/

/*Main Interface wintable start*/
LOCAL WINDOW_TABLE(MMIMBBMS_MAIN_INTERFACE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainInterfaceTabWinMsg),
    WIN_ID( MMIMBBMS_MAIN_INTERFACE_WIN_ID ),
    WIN_STATUSBAR,
    //WIN_TITLE(TXT_MBBMS_IS_AUTHORIZED),
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_COMMON_TITLE, MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_ALL_SERVICE_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAllServiceChildWinMsg),
    WIN_ID( MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_EXIT ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleFavoriteServiceChildWinMsg),
    WIN_ID( MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAlarmContentChildWinMsg),
    WIN_ID( MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID),
    END_WIN
};


//all service list pop menu
LOCAL WINDOW_TABLE(MMIMBBMS_ALL_SERVICE_POPMENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleAllServiceOptWinMsg),
	WIN_ID(MMIMBBMS_MAIN_INTERFACE_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

//favorite service list pop menu
LOCAL WINDOW_TABLE(MMIMBBMS_FAVORITE_SERVICE_POPMENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleFavoriteServiceOptWinMsg),
	WIN_ID(MMIMBBMS_MAIN_INTERFACE_FS_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

//favorite content list pop menu
LOCAL WINDOW_TABLE(MMIMBBMS_FAVORITE_CONTENT_POPMENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleAlarmContentOptWinMsg),
	WIN_ID(MMIMBBMS_MAIN_INTERFACE_FC_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

/*Main Interface wintable end*/

//emergency broadcast msg win
LOCAL WINDOW_TABLE(MMIMBBMS_EB_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEBListWinMsg),
    WIN_ID( MMIMBBMS_EB_LIST_WIN_ID ),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_MBBMS_EB),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_EB_LIST_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_EB_LIST_POPMENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleEBListOptWinMsg),
	WIN_ID(MMIMBBMS_EB_LIST_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_EB_DETAIL_WIN_TAB) = 
{
    WIN_ID( MMIMBBMS_EB_DETAIL_WIN_ID ),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_MBBMS_EB),
    WIN_FUNC((uint32)HandleEBDetailWinMsg),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN ),
    CREATE_RICHTEXT_CTRL(MMIMBBMS_EB_DETAIL_RICHTEXT_CTRL_ID),
    END_WIN
};

// the window of mbbms help info
LOCAL WINDOW_TABLE(MMIMBBMS_HELP_INFO_WIN_TAB) = 
{
    WIN_STATUSBAR,
    WIN_TITLE( TXT_HELP ),
    WIN_FUNC((uint32) HandleHelpInfoWinMsg),    
    WIN_ID( MMIMBBMS_HELP_INFO_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN ),
    
    CREATE_TEXT_CTRL(MMIMBBMS_HELP_INFO_TEXTBOX_CTRL_ID ),
    END_WIN
};
//add by yexiong.liu the update win
LOCAL WINDOW_TABLE(MMIMBBMS_UPDATE_MAIN_TAB) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandleUpdateMainWinMsg),
    WIN_ID(MMIMBBMS_UPDATE_MAIN_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_MBBMS_SELECT_UPDATE_METHOD),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    CREATE_MENU_CTRL(MMIMBBMS_UPDATE_MENU, MMIMBBMS_UPDATE_MENU_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMBBMS_UPDATE_FROM_CITY_MAIN_TAB) = 
{
    WIN_FUNC( (uint32)MMIMBBMS_HandleUpdateFromCityNameWinMsg),
    WIN_ID(MMIMBBMS_UPDATE_FROM_CITY_MAIN_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_MBBMS_UPDATE_ACCORDING_TO_CITY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMBBMS_CITY_UPDATE_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMBBMS_CITY_UPDATE_FORM1_CTRL_ID,MMIMBBMS_CITY_UPDATE_FORM_CTRL_ID),
            CHILD_TEXT_CTRL(FALSE,MMIMBBMS_TEXT_CITY_UPDATE_CTRL_ID,MMIMBBMS_CITY_UPDATE_FORM1_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIMBBMS_MAX_KEYWORD_LENS,MMIMBBMS_EDITTEXT_CITY_UPDATE_CTRL_ID,MMIMBBMS_CITY_UPDATE_FORM1_CTRL_ID),
    END_WIN
};

//SIM卡选择                                                
LOCAL WINDOW_TABLE( MMIMBBMS_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSimSelectWinMsg),    
    WIN_ID(MMIMBBMS_SIM_SELECT_WIN_ID),
    WIN_TITLE(TXT_SIM_SEL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMBBMS_SIM_SELECT_LIST_CTRL_ID),
    END_WIN
};

/*****************************************************************************/
//  Description : 套餐订购管理
//  Global resource dependence : none
//  Author: 
//  Note: 包含两项：本地套餐，已订套餐
/*****************************************************************************/
PUBLIC MMIMBBMS_RESULT MMIMBBMS_OpenSubscribeTabMainWin(void)
{
    /*add for init not finished*/
    if(!MMIMBBMS_DisplayInitWaitingWin())
    {
        return MMIMBBMS_NOT_OPEN_WIN;
    }    

    if (!MMK_IsFocusWin(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID))
    {
        /* 如果查询等待窗口没有得到焦点,则不打开套餐列表窗口 */
        MMK_PostMsg(MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID, MSG_APP_MBBMS_WAIT_GET_FOCUS, 
            NULL, NULL);
            
        return MMIMBBMS_NOT_OPEN_WIN;
    }	

    if (MMK_IsOpenWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID))
    {
        MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
    }

    MMK_CreateParentWin((uint32 *)MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB,PNULL);
    MMK_CreateChildWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID, (uint32 *) MMIMBBMS_PURCHASEDITEM_LIST_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID, (uint32 *) MMIMBBMS_LOCAL_PURCHASEITEM_CHILD_WIN_TAB, NULL);		

    //MMK_SetChildWinFocus(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID, MMIMBBMS_PURCHASEDITEM_CHILD_WIN_ID);
    if(MMIMBBMS_GetIfEnterSubFromPlay())
    {
        MMK_SetChildWinFocus(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID, MMIMBBMS_LOCAL_PURCHASEITEM_CHILD_WIN_ID);
    }
    else
    {
        MMK_SetChildWinFocus(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID, MMIMBBMS_PURCHASEDITEM_CHILD_WIN_ID);
    }    

    return MMIMBBMS_OK;	
}

/*****************************************************************************/
// 	Description : Add a purchase item to rchtext
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddPurchaseContentToRchtextCtrl(
                                                     MMI_CTRL_ID_T                ctrl_id,
                                                     uint8* service_name_ptr,
                                                     uint32 string_len)
{
    uint16 lens = 0;
    uint16 index = 0;
    wchar temp_wchar[GUILIST_STRING_MAX_NUM+1] = {0};
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    lens = GUI_UTF8ToWstr(temp_wchar,
        (uint16)GUILIST_STRING_MAX_NUM,
        service_name_ptr, 
        (uint16)string_len);
    
    item_data.text_data.buf.str_ptr = temp_wchar;
    item_data.text_data.buf.len = lens;
    
    GUIRICHTEXT_AddItem(ctrl_id, 
        &item_data, &index);
    
    return;
}

/*****************************************************************************/
// 	Description : add an Purchase item to list or richtext
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddPurchaseItemToListboxCtrl(
                                                  MMI_CTRL_ID_T                ctrl_id,
                                                  MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T* purchase_display_info_ptr)
{
    uint16 lens = 0;
    MMI_STRING_T mbbms_string_time = {0};
    MMI_STRING_T mbbms_string_unit = {0};
    MMI_STRING_T mbbms_string_unit2 = {0};
    MMI_STRING_T mbbms_string_1 = {0};
    
    MMI_STRING_T string_price = {0};
    MMI_STRING_T mbbms_string_temp2 = {0};
    MMI_STRING_T mbbms_string_2 = {0};
    
    wchar tempWchar[GUILIST_STRING_MAX_NUM] = {0};
    wchar temp_wchar1[GUILIST_STRING_MAX_NUM] = {0};
    wchar temp_wchar21[GUILIST_STRING_MAX_NUM] = {0};
    wchar temp_wchar22[GUILIST_STRING_MAX_NUM] = {0};
    
    MMI_STRING_T        temp_str = {0};
    MMI_STRING_T        subscrib_str = {0};
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T	item_data_t   = {0};/*lint !e64*/
    char str_price[MMIMBBMS_MAX_PRICE_LENS] = {0};
    wchar wchar_price[MMIMBBMS_MAX_PRICE_LENS] = {0};
    char str_period[MMIMBBMS_MAX_PRICE_LENS] = {0};
    wchar wchar_period[MMIMBBMS_MAX_PRICE_LENS] = {0};
    wchar wchar_period_for_price_dis[MMIMBBMS_MAX_PRICE_LENS] = {0};
    MMI_STRING_T mbbms_string_period_month = {0};
    
    SCI_MEMSET( &item_data_t, 0, sizeof (item_data_t));      
    
    item_t.item_style    = GUIITEM_STYLE_THREE_LINE_TEXT;
    item_t.item_data_ptr = &item_data_t;
    //SCI_TRACE_LOW:"MMIMBBMS_AddPurchaseItemToListboxCtrl purchase_display_info_ptr.period = %d, purchase_display_info_ptr.scription_type = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_1497_112_2_18_2_39_49_411,(uint8*)"dd", purchase_display_info_ptr->period, purchase_display_info_ptr->scription_type);
    SCI_MEMSET(tempWchar, 0, sizeof(tempWchar));
    lens = GUI_UTF8ToWstr(tempWchar, 
        (uint16)GUILIST_STRING_MAX_NUM,
        purchase_display_info_ptr->purchase_name, 
        (uint16)GUILIST_STRING_MAX_NUM);
    
    if (purchase_display_info_ptr->is_ordered)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_SUBSRIBED, &subscrib_str);
        lens = MMIAPICOM_Wstrlen(tempWchar) + subscrib_str.wstr_len;
        MMIAPICOM_Wstrcat(tempWchar, subscrib_str.wstr_ptr);
    }else
    {
        //CR245054, 移动不要求显示“未订购”字样
        //MMI_GetLabelTextByLang(TXT_MBBMS_NOT_SUBSCRIBED, &subscrib_str);
        //lens = MMIAPICOM_Wstrlen(tempWchar) + subscrib_str.wstr_len;
        //MMIAPICOM_Wstrcat(tempWchar, subscrib_str.wstr_ptr);		
	}
    //套餐名
    item_data_t.item_content[0].item_data.text_buffer.wstr_ptr = tempWchar;
    item_data_t.item_content[0].item_data.text_buffer.wstr_len = lens;      
    item_data_t.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;  
    
    //订购时间
    mbbms_string_1.wstr_ptr = temp_wchar1;
    MMI_GetLabelTextByLang(TXT_MBBMS_SUB_TIME, &mbbms_string_time);
    
    if (purchase_display_info_ptr->scription_type == MBBMS_PURCHASE_DATA_SUBSCRIPTION_CONTENT)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_TIMES, &mbbms_string_unit);
        MMIAPICOM_MergeTwoString(&mbbms_string_time, &mbbms_string_unit, &mbbms_string_1, GUILIST_STRING_MAX_NUM);
    }
    else if(MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM_MULTI == purchase_display_info_ptr->scription_type)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddPurchaseItemToListboxCtrl purchase_display_info_ptr.period = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_1532_112_2_18_2_39_49_412,(uint8*)"d", purchase_display_info_ptr->period);
        memset(str_period, 0, sizeof(str_period));
        if(0 != purchase_display_info_ptr->period)
        {
            sprintf(str_period, "%d", purchase_display_info_ptr->period);
            MMIAPICOM_StrToWstr((uint8*)str_period, wchar_period);
            MMI_GetLabelTextByLang(TXT_MBBMS_MONTH, &mbbms_string_period_month);          
            MMIAPICOM_Wstrncat(wchar_period, mbbms_string_period_month.wstr_ptr, mbbms_string_period_month.wstr_len);
            mbbms_string_unit.wstr_ptr = wchar_period;
            mbbms_string_unit.wstr_len = MMIAPICOM_Wstrlen(wchar_period);
            MMIAPICOM_MergeTwoString(&mbbms_string_time, &mbbms_string_unit, &mbbms_string_1, GUILIST_STRING_MAX_NUM);
        }
        //SCI_TRACE_LOW:"MMIMBBMS_AddPurchaseItemToListboxCtrl str_period[0] = 0x%x, mbbms_string_unit.wstr_len = %d \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_1544_112_2_18_2_39_49_413,(uint8*)"dd", str_period[0], mbbms_string_unit.wstr_len);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_MONTHS, &mbbms_string_unit);
        MMIAPICOM_MergeTwoString(&mbbms_string_time, &mbbms_string_unit, &mbbms_string_1, GUILIST_STRING_MAX_NUM);
    }
    //订购时间信息
    item_data_t.item_content[1].item_data.text_buffer.wstr_ptr = mbbms_string_1.wstr_ptr;
    item_data_t.item_content[1].item_data.text_buffer.wstr_len = mbbms_string_1.wstr_len;      
    item_data_t.item_content[1].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;  
    
    //资费
    MMI_GetLabelTextByLang(TXT_MBBMS_PURCHASEITEM_COST, &temp_str);
    sprintf(str_price, "%2f", purchase_display_info_ptr->price);
    
    lens = strlen(str_price);
    
    while (lens > 0 && str_price[--lens] == '0')
    {
        ;
    }
    
    if (str_price[lens] == '.')
    {
        str_price[lens+2] = '\0';
    }
    else
    {
        str_price[lens+1] = '\0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_price, wchar_price);
    
    string_price.wstr_ptr = wchar_price;
    string_price.wstr_len = MMIAPICOM_Wstrlen(wchar_price);
    
    mbbms_string_temp2.wstr_ptr = temp_wchar21;
    MMIAPICOM_MergeTwoString(&temp_str, &string_price, &mbbms_string_temp2, GUILIST_STRING_MAX_NUM);
    
    mbbms_string_2.wstr_ptr = temp_wchar22;
    
    if (purchase_display_info_ptr->scription_type == MBBMS_PURCHASE_DATA_SUBSCRIPTION_CONTENT)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_UNIT_PER_TIMES, &mbbms_string_unit2);
        MMIAPICOM_MergeTwoString(&mbbms_string_temp2, &mbbms_string_unit2, 
            &mbbms_string_2, GUILIST_STRING_MAX_NUM);
    }
    else if(MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM_MULTI == purchase_display_info_ptr->scription_type)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_YUAN, &mbbms_string_unit2);
        MMIAPICOM_MergeTwoString(&mbbms_string_temp2, &mbbms_string_unit2, 
            &mbbms_string_2, GUILIST_STRING_MAX_NUM);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_UNIT_PER_MONTH, &mbbms_string_unit2);
        MMIAPICOM_MergeTwoString(&mbbms_string_temp2, &mbbms_string_unit2, 
            &mbbms_string_2, GUILIST_STRING_MAX_NUM);
    }
    //资费信息
    item_data_t.item_content[2].item_data.text_buffer.wstr_ptr = mbbms_string_2.wstr_ptr;
    item_data_t.item_content[2].item_data.text_buffer.wstr_len = mbbms_string_2.wstr_len;
    item_data_t.item_content[2].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;  
    
    if (purchase_display_info_ptr->is_ordered == TRUE)
    {
        item_data_t.softkey_id[0] = TXT_MBBMS_UNSUBSCRIBE;
    }
    else
    {
        item_data_t.softkey_id[0] = TXT_MBBMS_SUBSCRIBE;
    }
    
    item_data_t.softkey_id[1] = TXT_MBBMS_COMBO_DETAIL;
    item_data_t.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t); 
    
}

/*****************************************************************************/
//  Description : Open Service Resume win
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenSearchMainWin(void)
{
    if (MMK_IsOpenWin(MMIMBBMS_SEARCH_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_SEARCH_MAIN_WIN_ID);
    }
    
    MMK_CreateWin((uint32 *)MMIMBBMS_SEARCH_MAIN_TAB,PNULL);
}

/*****************************************************************************/
//     Description : handle message of show picture window of browser
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleSearchMainMsgWin(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM                param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIFORM_SetStyle(MMIMBBMS_KEYWORD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIMBBMS_CHANNEL_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIMBBMS_DATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIMBBMS_PROGRAM_TYPE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

            GUILABEL_SetTextById(MMIMBBMS_LABEL_KEYWORD_CTRL_ID, TXT_MBBMS_KEYWORD, FALSE);
            GUILABEL_SetTextById(MMIMBBMS_LABEL_CHANNEL_CTRL_ID, TXT_MBBMS_CHANNEL, FALSE);
            GUILABEL_SetTextById(MMIMBBMS_LABEL_DATE_CTRL_ID,  TXT_COMMON_DATE, FALSE);
            GUILABEL_SetTextById(MMIMBBMS_LABEL_PROGRAM_TYPE_CTRL_ID, TXT_MBBMS_PROGRAM_TYPE, FALSE);
            
            MBBMS_AddAllServiceToDropdownList();
            MBBMS_AddAllDateToDropdownList();
            //MBBMS_AddAllContentTypeToDropdownList();
            MBBMS_AddAllContentTypeToDropdownListNew();
            
            GUIDROPDOWNLIST_SetCurItemIndex(MMIMBBMS_DROPDOWNLIST_CHANNEL_CTRL_ID, 0);
            GUIDROPDOWNLIST_SetCurItemIndex(MMIMBBMS_DROPDOWNLIST_DATE_CTRL_ID, 0);
            GUIDROPDOWNLIST_SetCurItemIndex(MMIMBBMS_DROPDOWNLIST_PROGRAM_TYPE_CTRL_ID, 0);
            
            MMK_SetAtvCtrl(win_id,MMIMBBMS_EDITBOX_KEYWORD_CTRL_ID);
            //s_search_date_index = 0;
            break;
        }
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            uint16 index = 0;
            //SG_ERROR_E error_code = SG_ERROR_NONE;
            MMI_STRING_T text_str = {0};
            uint8 utf8_len = 0;
            uint16 wchar_len = 0;
            MBBMSSG_SERVICE_T service_info = {0};
            char strDate[MMIMBBMS_MAX_DATE_STR_LENS] = {0};
            uint8* pContent = PNULL;
            SG_LIST_PTR_T   genre_ptr = PNULL;
            contentString *content_type_ptr = PNULL;            
            
            SCI_MEMSET(s_wchar_search, 0, sizeof(s_wchar_search));
            SCI_MEMSET(&s_complex_context, 0, sizeof(s_complex_context));
            
            GUIEDIT_GetString(MMIMBBMS_EDITBOX_KEYWORD_CTRL_ID,&text_str);
            
            if (text_str.wstr_len > 0)
            {
                s_complex_context.condition_enable |= MBBMS_CONTEXT_SEARCH_KEYWORD;
                MMIAPICOM_Wstrncpy(s_wchar_search, text_str.wstr_ptr, text_str.wstr_len);
                wchar_len = text_str.wstr_len;
                
                if (wchar_len < SG_MAX_LANGUAGE_CONTENT_LEN / 3)
                {
                    SCI_MEMSET(s_complex_context.keyword.string_arr, 0, SG_MAX_LANGUAGE_CONTENT_LEN);
                    utf8_len = GUI_WstrToUTF8(s_complex_context.keyword.string_arr, SG_MAX_LANGUAGE_CONTENT_LEN, 
                        text_str.wstr_ptr, text_str.wstr_len);
                }
                else
                {
                    pContent = (uint8*)SCI_ALLOC_APP((wchar_len + 1) * 3);
                    SCI_MEMSET(pContent, 0, (wchar_len + 1) * 3);
                    
                    utf8_len = GUI_WstrToUTF8(pContent, (wchar_len + 1) * 3, 
                        text_str.wstr_ptr, text_str.wstr_len);
                    
                    if (utf8_len <= SG_MAX_LANGUAGE_CONTENT_LEN)
                    {
                        SCI_MEMSET(s_complex_context.keyword.string_arr, 0, SG_MAX_LANGUAGE_CONTENT_LEN);
                        SCI_MEMCPY(s_complex_context.keyword.string_arr,
                            pContent, utf8_len);
                    }
                    else
                    {
                        s_complex_context.keyword.content_string_ptr = pContent;
                    }
                }
                
                s_complex_context.keyword.string_len = utf8_len;
            }
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIMBBMS_DROPDOWNLIST_CHANNEL_CTRL_ID);
            
            if (index > 0)
            {
                SG_LIST_PTR_T pService = PNULL;
                
                pService = MMIMBBMS_GetServiceNodeByIdx(index - 1);
                
                if (pService != PNULL)
                {
                    s_complex_context.condition_enable |= MBBMS_CONTEXT_SEARCH_SERVICE;
                    
                    // 要不要转换为global serviceID ?
                    s_complex_context.serviceId = *((anyURI*)(pService->data));
                    MBBMSSG_GetService(s_complex_context.serviceId, &service_info);
                    
                    if (wchar_len > 0)
                    {
                        MMIAPICOM_StrToWstr((uint8*)",", s_wchar_search+wchar_len);
                        wchar_len++;
                    }
                    
                    MMIMBBMS_GetNameWchar(service_info.NameList, 
                        s_wchar_search+wchar_len, sizeof(s_wchar_search)- wchar_len - 1);
                    wchar_len = MMIAPICOM_Wstrlen(s_wchar_search);
                    MBBMSSG_FreeService(&service_info);
                }
            }
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIMBBMS_DROPDOWNLIST_DATE_CTRL_ID);
            //s_search_date_index = index;
            
            if (index > 0)
            {
                SCI_DATE_T date = {0};
                
                s_complex_context.condition_enable |= MBBMS_CONTEXT_SEARCH_DATE;
                
                MMIMBBMS_GetSelectedDate(&date, index - 1);
                s_complex_context.startTime.years = date.year;
                s_complex_context.startTime.months = date.mon;
                s_complex_context.startTime.days = date.mday;
                s_complex_context.startTime.hours = 0;
                s_complex_context.startTime.minutes = 0;
                s_complex_context.startTime.seconds = 0;
                
                if (wchar_len > 0)
                {
                    MMIAPICOM_StrToWstr((uint8*)",", s_wchar_search+wchar_len);
                    wchar_len++;
                }
                
                sprintf(strDate, "%d-%d-%d", date.year, date.mon, date.mday);
                MMIAPICOM_StrToWstr((uint8*)strDate, s_wchar_search+wchar_len);
                wchar_len = MMIAPICOM_Wstrlen(s_wchar_search);
            }
            
            index = GUIDROPDOWNLIST_GetCurItemIndex(MMIMBBMS_DROPDOWNLIST_PROGRAM_TYPE_CTRL_ID);
            
            if (index > 0)
            {
                MMI_STRING_T item_date = {0};
                
                s_complex_context.condition_enable |= MBBMS_CONTEXT_SEARCH_GENRE;
                
                item_date = GUIDROPDOWNLIST_GetCurItem(MMIMBBMS_DROPDOWNLIST_PROGRAM_TYPE_CTRL_ID);

                genre_ptr = MBBMSSG_GetContentGenreIndex();
                content_type_ptr = (contentString*)STL_LinkListGetNodeDataByIdx(genre_ptr, (index - 1));                 
#if 0                
                utf8_len = GUI_WstrToUTF8(s_complex_context.genre.string_arr, SG_MAX_CONTENT_LEN, 
                    item_date.wstr_ptr, item_date.wstr_len);
                s_complex_context.genre.string_len = utf8_len;
#endif           
                if(PNULL != content_type_ptr)
                {
                    SCI_MEMCPY(&s_complex_context.genre, content_type_ptr, sizeof(contentString));
                    SCI_TRACE_BUF("MMIMBBMS_HandleSearchMainMsgWin s_complex_context.genre.string_arr =%x", s_complex_context.genre.string_arr, sizeof(s_complex_context.genre.string_arr));
                    //SCI_TRACE_LOW:"MMIMBBMS_HandleSearchMainMsgWin genre.string_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_1810_112_2_18_2_39_50_414,(uint8*)"d", s_complex_context.genre.string_len);
                }
                
                if (wchar_len > 0)
                {
                    MMIAPICOM_StrToWstr((uint8*)",", s_wchar_search+wchar_len);
                    wchar_len++;
                }
                
                MMIAPICOM_Wstrncpy(s_wchar_search+wchar_len, item_date.wstr_ptr, item_date.wstr_len);
                wchar_len = MMIAPICOM_Wstrlen(s_wchar_search);
            }
            
            if(MMIMBBMS_FLOW_STATE_IDLE == MMIMBBMS_GetFlowInfo()->status)
            {
                //SCI_TRACE_LOW:"MMIMBBMS_HandleSearchMainMsgWin start MBBMSSG_GetComplexInfo"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_1825_112_2_18_2_39_50_415,(uint8*)"");
                MMIMBBMS_SetFlowInfo(MMIMBBMS_FLOW_SG_GET_COMPLEX, MMIMBBMS_FLOW_STATE_STARTING);
                MBBMSSG_GetComplexInfo(s_complex_context, &s_search_list_ptr);
                MMIMBBMS_OpenSearchWaitWin();
            }
            else
            {
                //SCI_TRACE_LOW:"MMIMBBMS_HandleSearchMainMsgWin is not MMIMBBMS_FLOW_STATE_IDLE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_1832_112_2_18_2_39_50_416,(uint8*)"");
            }
            
            if (pContent != PNULL)
            {
                SCI_FREE(pContent);
                pContent = PNULL;
            }
            
            break;
        }
         
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);//返回上一级
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

/*****************************************************************************/
// 	Description : MMIMBBMS_FreeComplexInfo
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeComplexInfo(void)
{
    if (s_search_list_ptr != PNULL)
    {
        MBBMSSG_FreeComplexInfo(&s_search_list_ptr);
    }
}

/*****************************************************************************/
// 	Description : MMIMBBMS_OpenSearchWaitWin
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenSearchWaitWin(void)
{
    MMI_STRING_T  prompt_str={0};
    if(MMK_IsOpenWin(MMIMBBMS_SEARCH_WAIT_WIN_ID))
        return;
    //start unscribe					
    MMI_GetLabelTextByLang(TXT_COMMON_SEARCHING, &prompt_str);	
    MMIPUB_OpenWaitWin(1,
        &prompt_str,
        PNULL,
        PNULL,
        MMIMBBMS_SEARCH_WAIT_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_ONE,
        HandleSearchWaitWinMsg);
}

/*****************************************************************************/
// 	Description : HandleSearchWaitWinMsg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSearchWaitWinMsg( 
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    //LOCAL uint8 wait_timer = 0;
    LOCAL BOOLEAN is_wait_get_focus = FALSE;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case  MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
        result = MMI_RESULT_FALSE;
        is_wait_get_focus = FALSE;
        break;

    case MSG_APP_CANCEL:
        MMIMBBMS_StopFlow();
        MMK_CloseWin(win_id);
        break;
        
    //red key直接退出手机电视        
#if 0        
    case MSG_APP_RED:
        MBBMSSG_StopGetComplexInfo();
        MMK_CloseWin(win_id);
        break;
#endif

    case MSG_APP_MBBMS_WAIT_GET_FOCUS:
        is_wait_get_focus = TRUE;
        break;
        
    case MSG_GET_FOCUS:
        if (is_wait_get_focus)
        {
            MMIMBBMS_OpenSearchResultMainWin((SG_LIST_PTR_T)(s_search_list_ptr));
        }
        
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : MMIMBBMS_OpenSearchResultMainWin
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSearchResultMainWin(SG_LIST_PTR_T list_ptr)
{
    if (MMK_IsOpenWin(MMIMBBMS_SEARCH_RESULT_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_SEARCH_RESULT_WIN_ID);
    }
    
    MMK_CreateWin((uint32 *)MMIMBBMS_SEARCH_RESULT_WIN_TAB, (ADD_DATA)list_ptr);
}

/*****************************************************************************/
// 	Description : HandleSearchResultMainWinMsg
//	Global resource dependence : 
//  Author:        
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSearchResultMainWinMsg( 
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param)
{
    uint16 index = 0;
    SG_LIST_PTR_T list_ptr = PNULL;
    SG_LIST_PTR_T pItem = PNULL;
    MMI_STRING_T  strContent1 = {0};
    MMI_STRING_T  strContent2 = {0};
    MMI_STRING_T  str_temp = {0};
    MMI_STRING_T  str_merge = {0};
    MMI_STRING_T  str_sql = {0};
    wchar wchar_temp[MMIMBBMS_COMMON_MAX_LENS+1] = {0};
    wchar wchar_merge[MMIMBBMS_COMMON_MAX_LENS+1] = {0};
    MMI_RESULT_E  recode          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T current_ctrl_id = MMIMBBMS_SEARCH_RESULT_LIST_CTRL_ID;
    MBBMSSG_CONTEXT_COMPLEX_DATA_T* pContent = PNULL;
    static uint16 s_current_index = 0;//记录当前光标位置
    static uint16 s_total_search_count = 0;//搜索结果总数    
    
    list_ptr = (SG_LIST_PTR_T)MMK_GetWinAddDataPtr(win_id);
    //SCI_TRACE_LOW:"HandleSearchResultMainWinMsg()  msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_1984_112_2_18_2_39_50_417,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:      //create check list box
        {
            if (MMK_IsOpenWin(MMIMBBMS_SEARCH_WAIT_WIN_ID))
            {
                MMK_CloseWin(MMIMBBMS_SEARCH_WAIT_WIN_ID);
            }
            
            pItem = list_ptr;
            if (PNULL == pItem)
            {
                //SetListboxEmpty(current_ctrl_id);
                
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
                
                MMI_GetLabelTextByLang(TXT_MBBMS_NO_CONTENT, &strContent1);
                str_temp.wstr_ptr = wchar_temp;
                str_sql.wstr_ptr = s_wchar_search;
                str_sql.wstr_len = MMIAPICOM_Wstrlen(s_wchar_search);
                MMIAPICOM_MergeTwoString(&strContent1, &str_sql, &str_temp, MMIMBBMS_COMMON_MAX_LENS);
                
                MMI_GetLabelTextByLang(TXT_MBBMS_NO_CONTENT2, &strContent2);
                str_merge.wstr_ptr = wchar_merge;
                MMIAPICOM_MergeTwoString(&str_temp, &strContent2, &str_merge, MMIMBBMS_COMMON_MAX_LENS);
                
                MMIPUB_OpenAlertTextWinByTextPtr(PNULL,
                    &str_merge,
                    PNULL,
                    PNULL,
                    MMIPUB_SOFTKEY_ONE,
                    PNULL); 
                break;
            }
            
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_RETURN, TRUE);
            
            GUILIST_SetMaxItem(current_ctrl_id, (uint16)(list_ptr->node_cnt - 1), TRUE);
            
            MMIMBBMS_LoadSearchRstList(list_ptr, 0, 
                0xFFFF, FALSE);
            
            if (GUILIST_GetTotalItemNum(current_ctrl_id) == 0)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
            }
            
            MMK_SetAtvCtrl(win_id,  current_ctrl_id);
            
            //MMK_PostMsg(MMIMBBMS_SEARCH_RESULT_WIN_ID, MSG_APP_MBBMS_LOAD_SEARCH_RESULT, 
            //    NULL, NULL);

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif            
            break;
        }
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            //SCI_TRACE_LOW:"HandleSearchResultMainWinMsg() MSG_CTL_LIST_NEED_ITEM_DATA msg_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2045_112_2_18_2_39_50_418,(uint8*)"d",msg_id);
            if(PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMIMBBMS HandleSearchResultMainWinMsg need_item_data_ptr is null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2048_112_2_18_2_39_50_419,(uint8*)"");
                break;
            }
            
            index = need_item_data_ptr->item_index;
            
            MMIMBBMS_LoadSearchRstList(list_ptr,
                index, index, TRUE);
            break;
        }
    case MSG_APP_MBBMS_REFRESH_LIST:
        {
            GUILIST_RemoveAllItems(current_ctrl_id);
            
            if (PNULL != list_ptr)
            {
                GUILIST_SetMaxItem(current_ctrl_id, (uint16)(list_ptr->node_cnt - 1), TRUE);
                
                MMIMBBMS_LoadSearchRstList(list_ptr, 0, 
                    0xFFFF, FALSE);
            }
            
            if (GUILIST_GetTotalItemNum(current_ctrl_id) == 0)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
            }
            
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            /*add for init not finished*/
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }    
            if(!MMIMBBMS_GetSubscriptionUpdateStatus())
            {
                MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                s_object_type=MMIMBBMS_OBJECT_FROM_SEARCH_RESULT;
                MMIMBBMS_OpenSubscriptionUpdateQueryWin();
            }
            else
            { 
                MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_SEARCH_RESULT);
            }
            
            break;
        }
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        {
            if (GUILIST_GetTotalItemNum(current_ctrl_id) > 0)
            {
                index = GUILIST_GetCurItemIndex(current_ctrl_id);
                pContent = (MBBMSSG_CONTEXT_COMPLEX_DATA_T*)STL_LinkListGetNodeDataByIdx(list_ptr, index);
                
                if (MMK_IsOpenWin(MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_ID);
                }
                
                MMK_CreateWin((uint32 *)MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_TAB, (ADD_DATA)pContent); 
            }
            
            break;
        }
    case MSG_CLOSE_WINDOW:
        if (list_ptr != PNULL)   
        {   
            MBBMSSG_FreeComplexInfo(&list_ptr);   
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_GET_FOCUS:
        {
            uint16 total_count = GUILIST_GetTotalItemNum(current_ctrl_id);
            if (s_total_search_count == total_count && s_current_index < total_count)
            {
                GUILIST_SetCurItemIndex(current_ctrl_id, s_current_index);
            }
            else
            {
                s_current_index = 0;
                GUILIST_SetCurItemIndex(current_ctrl_id, s_current_index);
            }
            
            break;
        }
    case MSG_LOSE_FOCUS:
    {
        s_total_search_count = GUILIST_GetTotalItemNum(current_ctrl_id);
        s_current_index = GUILIST_GetCurItemIndex(current_ctrl_id);
        break;
    }        
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : MMIMBBMS_LoadSearchRstList
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_LoadSearchRstList(SG_LIST_PTR_T list_ptr,
                                      uint32 left_index, uint32 right_index,
                                      BOOLEAN is_load_data)
{
    uint index = 0;
    GUILIST_ITEM_T          item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0};/*lint !e64*/
    MMI_CTRL_ID_T           current_ctrl_id = MMIMBBMS_SEARCH_RESULT_LIST_CTRL_ID;
    MBBMSSG_CONTEXT_COMPLEX_DATA_T*  pContent = PNULL;
    wchar temp_wchar[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    wchar wchar_time[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    MBBMSSG_SERVICE_T serviceItem = {0};
    SG_ERROR_E errcode = SG_ERROR_NONE;
    MBBMSSG_CONTENT_T  temp_content = {0};
    anyURI globalContentID = {0};
    char strTemp[40] = {0};
    SG_LIST_PTR_T pItem = PNULL;
    
    if (list_ptr == PNULL)
    {
        return;
    }
	
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON;
    item_t.item_data_ptr = &item_data;
    
    if (left_index >= list_ptr->node_cnt - 1)
    {
        return;
    }
    
    if (left_index > right_index)
    {
        return;
    }
    
    //for (index = 0; index < list_ptr->node_cnt - 1; index++)
    pItem = list_ptr->next_ptr;
    
    while (index < left_index && pItem != PNULL)
    {
        pItem = pItem->next_ptr;
        index++;
    }
    
    index = left_index;
    
    while (pItem != PNULL && index <= right_index)
    {
        SCI_MEMSET(temp_wchar, 0, sizeof(temp_wchar));
        //pContent = (MBBMSSG_CONTEXT_COMPLEX_DATA_T*)STL_LinkListGetNodeDataByIdx(list_ptr, index);
        pContent = (MBBMSSG_CONTEXT_COMPLEX_DATA_T*)(pItem->data);
        
        //SCI_TRACE_LOW:"pContent = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2219_112_2_18_2_39_51_420,(uint8*)"d", pContent);
        
        if (pContent == PNULL)
        {
            pItem = pItem->next_ptr;
            index++;
            continue;
        }
        
        if (is_load_data == TRUE)
        {
            item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = IMG_PRO_LIST;
            
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            
            MMIMBBMS_GetNameWchar(pContent->contextIdx_ptr->NameList, temp_wchar, 
                MMIMBBMS_CONTENT_NAME_MAX_LEN);
            
            MMIAPICOM_WstrToStr(temp_wchar, (uint8*)strTemp);
            
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_wchar;
            item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(temp_wchar);
            
            MBBMSSG_GetGlobalContentId(pContent->contextIdx_ptr->index, &globalContentID);
            
            temp_content.StartTime = pContent->contextIdx_ptr->startTime;
            temp_content.EndTime = pContent->contextIdx_ptr->endTime;
            MMIMBBMS_GetStartEndTimeString(&temp_content, wchar_time);
            errcode = MBBMSSG_GetService(*(pContent->serviceId_ptr), &serviceItem);
            if(SG_ERROR_NONE == errcode)
            {
                MMIMBBMS_GetNameWchar(serviceItem.NameList, wchar_time+11, sizeof(wchar_time) - 11);
            }
    
            if(MMIMBBMS_IsAlarmContent(&globalContentID) == TRUE)
            {
                item_data.item_content[3].item_data_type        = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[3].item_data.image_id    = IMAGE_MBBMS_NOTIFY;
            }           
            item_data.item_content[5].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[5].item_data.text_buffer.wstr_ptr = wchar_time;
            item_data.item_content[5].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wchar_time);
                       
            GUILIST_SetItemData(current_ctrl_id, &item_data, index);
            
            SCI_MEMSET(&item_data, 0, sizeof(item_data));
            
            MBBMSSG_FreeTypeAnyURI(&globalContentID);
            
            MBBMSSG_FreeService(&serviceItem);
        }
        else
        {
            
            GUILIST_AppendItem( current_ctrl_id, &item_t );
        }
        
        index++;
        pItem = pItem->next_ptr;
        
    }
    
    return;
}

/*****************************************************************************/
//  Description : MMIMBBMS_GetNameWchars
//  Global resource dependence : none
//  Author: louis 
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMBBMS_GetNameWchar(SG_LIST_PTR_T namelist_ptr,
                                    wchar* wchar_name,
                                    uint16 wstr_len)
{
    uint16 str_len = 0;
    uint16 to_len = 0;
    uint8* str_buf_ptr = PNULL;
    
    languageString* name_info_ptr = PNULL;
    
    if (namelist_ptr != PNULL
        && namelist_ptr->next_ptr != PNULL
        && namelist_ptr->next_ptr->data != PNULL)
    {
        name_info_ptr = (languageString*)namelist_ptr->next_ptr->data;
        
        if(PNULL != name_info_ptr->content_string_ptr)
        {
            str_buf_ptr = name_info_ptr->content_string_ptr;
        }
        else
        {
            str_buf_ptr = name_info_ptr->string_arr;
        }
        
        to_len = MIN(name_info_ptr->string_len, wstr_len);
        
        
        str_len = GUI_UTF8ToWstr(wchar_name, to_len,
            str_buf_ptr, to_len);
    }
    
    
    return str_len;
}

/*****************************************************************************/
// 	Description : handle create new connection set info win msg
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlenewconnectionHelpInfoWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T text={0};
            
            MMI_GetLabelTextByLang(TXT_MBBMS_NEW_CONNECTION_HELP_INFO,&text);
            GUITEXT_SetString(MMIMBBMS_NEW_CONNECTION_HELP_INFO_TEXTBOX_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
            MMK_SetAtvCtrl(win_id, MMIMBBMS_NEW_CONNECTION_HELP_INFO_TEXTBOX_CTRL_ID);
        }
        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : MMIMBBMS_OpenSetting win
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSettingMainWin(void)
{
    if (MMK_IsOpenWin(MMIMBBMS_SETTING_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_SETTING_WIN_ID);
    }
    
    MMK_CreateWin((uint32 *)MMIMBBMS_SETTING_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : HandleSettingWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleSettingWinMsg(
                                        MMI_WIN_ID_T        win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param)
{
    uint16  index         = 0;
    uint16  notify_type   = 0;
    uint16  network_index = 0;
    wchar   domain_wchar[M_MMIMBBMS_MAX_SG_DOMAIN_LENS] = {0};
    wchar   naf_wchar[M_MMIMBBMS_MAX_NAF_ADDR_LENS]     = {0};
    MMI_STRING_T   domain_str = {0};
    MMI_STRING_T   naf_str    = {0};
    MMI_RESULT_E   recode     = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T  effect_text[MMIMBBMS_NOTIFY_MAX_ITEMS] = {TXT_MBBMS_CLOSE_ALL_NOTIFICATION,
        TXT_MBBMS_ADVANCE_2_MINUTES,
        TXT_MBBMS_ADVANCE_5_MINUTES,
        TXT_MBBMS_ADVANCE_10_MINUTES};
    MMI_HANDLE_T ctrl_handle = NULL;
    MMI_STRING_T*  effect_item = PNULL;
    uint8 link_num = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_ptr = PNULL;	
    wchar           str_arry[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint16          str_len     = 0;
    MMI_STRING_T    temp_str    = {0};
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            
            GUIFORM_SetStyle(MMIMBBMS_NOTIFY_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIMBBMS_NETWORK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIMBBMS_NEW_CONNECTION_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIMBBMS_NEW_CONNECTION_HELP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIMBBMS_SG_DNS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

            GUILABEL_SetTextById(MMIMBBMS_LABEL_NOTIFY_TIME_CTRL_ID, TXT_MBBMS_PROGRAM_NOTIFY_TIME, FALSE); 
            GUILABEL_SetTextById(MMIMBBMS_LABEL_NETWORK_CTRL_ID, TXT_NETWORK_CONNECT_SET, FALSE);
            GUILABEL_SetTextById(MMIMBBMS_LABEL_NEW_CONNECTION_CTRL_ID, TXT_MBBMS_NEW_CONNECTION, FALSE);
            GUILABEL_SetTextById(MMIMBBMS_LABEL_NEW_CONNECTION_HELP_CTRL_ID, TXT_MBBMS_NEW_CONNECTION_HELP, FALSE);			
            GUILABEL_SetTextById(MMIMBBMS_LABEL_SG_DNS_CTRL_ID, TXT_MBBMS_SG_SERVER_DNS, FALSE);
            GUIEDIT_SetIm(MMIMBBMS_EDITBOX_SG_DNS_CTRL_ID,
                GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,
                GUIIM_TYPE_ENGLISH);
            
            domain_str.wstr_ptr = domain_wchar;
            naf_str.wstr_ptr = naf_wchar;
            MMIMBBMS_GetSettingInfo(MMIMBBMS_GetCurSelectSim(), &notify_type, &network_index, &domain_str, &naf_str);
            
            link_num = MMIAPICONNECTION_GetLinkSettingNum(MMIMBBMS_GetCurSelectSim());
            
            effect_item = (MMI_STRING_T*)SCI_ALLOC_APP(sizeof(MMI_STRING_T) * MAX(MMIMBBMS_NOTIFY_MAX_ITEMS, link_num));
            
            if (effect_item == PNULL)
            {
                //SCI_TRACE_LOW:"SCI_ALLOC_APP FOR effect_item FAIL!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2435_112_2_18_2_39_51_421,(uint8*)"");
                break;
            }
            
            //get effect item
            for (index = 0; index < MMIMBBMS_NOTIFY_MAX_ITEMS; index++)
            {
                MMI_GetLabelTextByLang(effect_text[index],&effect_item[index]);
            }

            //set effect drop down list
            GUIDROPDOWNLIST_AppendItemArray(MMIMBBMS_DROPDOWNLIST_NOTIFY_TIME_CTRL_ID, 
                effect_item, MMIMBBMS_NOTIFY_MAX_ITEMS);
            
            GUIDROPDOWNLIST_SetCurItemIndex(MMIMBBMS_DROPDOWNLIST_NOTIFY_TIME_CTRL_ID, notify_type);

            SCI_FREE(effect_item);
            effect_item = PNULL;
            
            GUIDROPDOWNLIST_SetMaxItem(MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID, link_num);
            for(index = 0; index < link_num; index++)
            {
                link_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIMBBMS_GetCurSelectSim(), index);

                str_len = MIN(GUILIST_STRING_MAX_NUM,link_ptr->name_len);
                temp_str.wstr_len = str_len;
                SCI_MEMSET(str_arry, 0, sizeof(str_arry));
                temp_str.wstr_ptr = str_arry;

                MMI_WSTRNCPY(temp_str.wstr_ptr,temp_str.wstr_len,
                    link_ptr->name,temp_str.wstr_len,
                    temp_str.wstr_len);

                GUIDROPDOWNLIST_AppendItem(MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID, &temp_str);
            }
            
            GUIDROPDOWNLIST_SetCurItemIndex(MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID, network_index);
            
            GUIEDIT_SetString(MMIMBBMS_EDITBOX_SG_DNS_CTRL_ID,domain_str.wstr_ptr,domain_str.wstr_len);
            
            MMK_SetAtvCtrl(win_id,MMIMBBMS_DROPDOWNLIST_NOTIFY_TIME_CTRL_ID);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:	
        ctrl_handle = MMK_GetActiveCtrlId(win_id);
        switch(ctrl_handle)
		{
            case MMIMBBMS_LABEL_NEW_CONNECTION_CTRL_ID:
                MMIAPICONNECTION_OpenSettingListWin();							
                break;
            case MMIMBBMS_LABEL_NEW_CONNECTION_HELP_CTRL_ID:
                MMK_CreateWin((uint32*)MMIMBBMS_NEW_CONNECTION_HELP_INFO_WIN_TAB,NULL);
                break;
            default:
                break;
		}
        break;
    case MSG_FULL_PAINT:
            
            domain_str.wstr_ptr = domain_wchar;
            naf_str.wstr_ptr = naf_wchar;
            MMIMBBMS_GetSettingInfo(MMIMBBMS_GetCurSelectSim(),&notify_type, &network_index, &domain_str, &naf_str);		
            link_num = MMIAPICONNECTION_GetLinkSettingNum(MMIMBBMS_GetCurSelectSim());
            
            GUIDROPDOWNLIST_SetMaxItem(MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID, link_num);
            for(index = 0; index < link_num; index++)
			{
                link_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIMBBMS_GetCurSelectSim(),index);

                str_len = MIN(GUILIST_STRING_MAX_NUM,link_ptr->name_len);
                temp_str.wstr_len = str_len;
                SCI_MEMSET(str_arry, 0, sizeof(str_arry));
                temp_str.wstr_ptr = str_arry;

                MMI_WSTRNCPY(temp_str.wstr_ptr,temp_str.wstr_len,
                    link_ptr->name,temp_str.wstr_len,
                    temp_str.wstr_len);

                GUIDROPDOWNLIST_AppendItem(MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID, &temp_str);
			}
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            if(MMIMBBMS_IsSettingItemModified(MMIMBBMS_GetCurSelectSim()))
            {
                MMI_STRING_T sg_text = {0};
                
                GUIEDIT_GetString(MMIMBBMS_EDITBOX_SG_DNS_CTRL_ID,&sg_text);
                if(sg_text.wstr_len > 0)
                {
                    MMIPUB_OpenQueryWinByTextId(
                        TXT_MBBMS_SETTING_SAVE_PROMPT, 
                        IMAGE_PUBWIN_QUERY, 
                        PNULL, 
                        HandleSettingSavePromptWinMsg);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MBBMS_SG_DNS_NULL);
                }
            }
            else
            {			
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIMBBMS_SETTING_WIN_ID);
            }
            
            break;
        }   
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            if(MMIMBBMS_IsSettingItemModified(MMIMBBMS_GetCurSelectSim()))
            {
                
                MMIPUB_OpenQueryWinByTextId(
                    TXT_MBBMS_SETTING_CANCEL_PROMPT, 
                    IMAGE_PUBWIN_QUERY, 
                    PNULL, 
                    HandleSettingCancelPromptWinMsg);
            }
            else
            {			
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIMBBMS_SETTING_WIN_ID);
            }
            break;
        }
        
    default:
        //SCI_TRACE_LOW:"HandleMbbmsSettingWinMsg() msg_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2568_112_2_18_2_39_51_422,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : HandleSearchRstOptMenuWinMsg
//  Global resource dependence : 
//  Author:liming.deng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSearchRstOptMenuWinMsg(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM                param
                                                 )
{
    uint16 i = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_SEARCH_RST_OPT_MENU_CTRL_ID;
    LOCAL anyURI  globalContentId = {0};
    SG_ERROR_E    error_code = SG_ERROR_NONE;
    uint16        node_id = 0;
    //BOOLEAN  bResult = FALSE;
    MBBMSSG_CONTEXT_COMPLEX_DATA_T* pContent = PNULL;
    
    pContent = (MBBMSSG_CONTEXT_COMPLEX_DATA_T*)MMK_GetWinAddDataPtr(win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //creat dynamic menu
        GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
        
        //get global content id
        SCI_MEMSET(&globalContentId,0,sizeof(anyURI));
        error_code =MBBMSSG_GetGlobalContentId(pContent->contextIdx_ptr->index, &globalContentId);
        //globalContentId.anyURI_arr[globalContentId.string_len] = 0;
        
        
        // 1. 观看
        InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
        
        // 2. 节目详情
        InsertMenuNode(ctrl_id, TXT_COMMON_PROGRAM_DETAIL, MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID, 0, i++);
        
        // 3. 设置提醒/取消提醒
        if(!MMIMBBMS_IsAlarmContent(&globalContentId))
        {
            InsertMenuNode(ctrl_id, TXT_MBBMS_SET_CONTENT_ALARM, MMIMBBMS_ID_SET_CONTENT_ALARM_MENU, 0, i++);
        }
        else
        {
            InsertMenuNode(ctrl_id, TXT_MBBMS_CLOSE_CONTENT_ALARM, MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU, 0, i++);
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            anyURI contentID = {0};
            //MMIMBBMS_SAVE_CONTENT_INFO_T content_info = {0};
            
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
            case MMIMBBMS_ID_WATCH_MENU:
                {
                    /*add for init not finished*/
                    if(!MMIMBBMS_DisplayInitWaitingWin())
                    {
                        break;
                    }    
                    
                    if(!MMIMBBMS_GetSubscriptionUpdateStatus())
                    {
                        MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                        s_object_type=MMIMBBMS_OBJECT_FROM_SEARCH_RESULT_OPT;
                        MMIMBBMS_OpenSubscriptionUpdateQueryWin();
                    }
                    else
                    {  
                        MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_SEARCH_RESULT_OPT);
                    }
                    break;
                }
            case MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID:
                {
                    if (MMK_IsOpenWin(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID))
                    {
                        MMK_CloseWin(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID);
                    }
                    
                    MMK_CreateWin((uint32 *)MMIMBBMS_PROGRAM_DETAIL_WIN_TAB, 
                        (ADD_DATA)&globalContentId); 
                    
                    //s_program_list_open_it = FALSE;
                    
                    break;
                }
            case MMIMBBMS_ID_SET_CONTENT_ALARM_MENU:
                {
                    MBBMSSG_CONTENT_T content_t = {0};
                    MMIMBBMS_ALARM_CONTENT_INFO_T AlarmContentInfo = {0};
                    
                    MBBMSSG_GetContentId(globalContentId, &contentID);
                    MBBMSSG_GetContent(contentID, &content_t);
                    
                    AlarmContentInfo.globalContentID = globalContentId;
                    AlarmContentInfo.starttime = content_t.StartTime;
                    MBBMSSG_FreeContent(&content_t);
                    
                    MMIMBBMS_RegisterContentAlarm(&AlarmContentInfo);
                    
                    MMIMBBMS_AddAlarmContentNode(&AlarmContentInfo);
                    
                    MBBMSSG_FreeTypeAnyURI(&contentID);
                    
                    MMK_CloseWin(win_id);
                    
                    MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_SEARCH_RESULT_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    break;
                }
                
            case MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU:
                {
                    MMIMBBMS_ALARM_CONTENT_INFO_T* pAlarmContentInfo = PNULL;
                    
                    pAlarmContentInfo = MMIMBBMS_GetAlarmContentNodeByContentID(&globalContentId);
                    if(PNULL != pAlarmContentInfo)
                    {
                        /* 取消节目提醒闹钟 */
                        MMIALMSVC_UnRegService(pAlarmContentInfo->event_id, TRUE);
                        
                        MMIMBBMS_DelAlarmContentNode(pAlarmContentInfo);
                        
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MBBMS_CONTENT_UNREGISTER_NOTIFY_INFO,TXT_NULL,IMAGE_PUBWIN_SUCCESS, 
                            PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,MMIMBBMS_HandleAddorCancelAlarmServiceAlertWinMsg);
                        
                        MMK_CloseWin(win_id);
                        
                        MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                        MMK_PostMsg(MMIMBBMS_SEARCH_RESULT_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                        MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    }
                    break;
                }
                
            default:
                break;
            }
            
            MMK_CloseWin(win_id);
            break;
        }    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_LOSE_FOCUS:
        //MBBMSSG_FreeTypeAnyURI(&globalContentId);
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/******************************************************************************
 * 函数定义:	MMIMBBMS_OpenPurchaseOrderQueryWin
 * 函数说明：	
 * 参数说明：	订购/退订提示窗口
 * 返回值：		void
********************************************************************************/
LOCAL void MMIMBBMS_OpenPurchaseOrderQueryWin(void)
{
    uint32        purchase_index = 0;
    SG_ERROR_E    err_code = SG_ERROR_NONE;
    MMI_STRING_T  prompt_str1 = {0};   
    MMI_STRING_T  prompt_str = {0};
    SG_LIST_PTR_T purchaseIndex_list_head_ptr = PNULL;
    
    MBBMSSG_PURCHASE_ITEM_T item_info = {0};
    MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr = PNULL;
    wchar   wchar_name[MMIMBBMS_PURCHASE_NAME_MAX_LEN+1] = {0};
    MBBMS_Purchase_Table_Info_t* purchase_table_info_ptr = PNULL;	
    MBBMSSG_PURCHASE_ITEM_T* purchase_item_ptr = PNULL;
    uint32 anyURI_len = 0;
    anyURI   global_purchase_item_Id = {0};//套餐标识
    SG_ERROR_E purchase_item_error_code = SG_ERROR_NONE;
    anyURI   itemId = {0};//套餐分片标识 
    wchar   wchar_query[2] = {'?', 0};
    MBBMS_Account_Inquiry_Res_t* account_inquiry_res_ptr = MMIMBBMS_GetAccountInquiryRes();
    
    if (MMIMBBMS_SDandOpenCondition() == FALSE)
    {
        return;
    }
    
    if(MMIMBBMS_GetCurrPurchaseOrdereStatus())
    {
        //enter into unsubscribe query window. should combile three string @plum
        
        //should get current purchase name and combile with str1
        MMI_GetLabelTextByLang(TXT_MBBMS_UNSUB_QUERY, &prompt_str1);
        
        SCI_MEMSET(wchar_name, 0, sizeof(wchar_name));
        
        MMIAPICOM_Wstrncpy(wchar_name, prompt_str1.wstr_ptr, 
            prompt_str1.wstr_len);
        purchase_index = MMIMBBMS_GetCurrPurchaseIndex();    
        purchase_table_info_ptr = MMIMBBMS_GetPurchasedItemByIndex(purchase_index, account_inquiry_res_ptr);
         //SCI_TRACE_LOW:"---MMIMBBMS_OpenPurchaseOrderQueryWin purchase_table_info_ptr = 0x%x\n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2787_112_2_18_2_39_52_423,(uint8*)"d", purchase_table_info_ptr);
        if(PNULL != purchase_table_info_ptr)
        {
            purchase_item_ptr = (MBBMSSG_PURCHASE_ITEM_T*)
                STL_LinkListGetNodeDataByIdx(purchase_table_info_ptr->purchase_item, 0);            
            //如果查询返回信息中套餐分片信息或者套餐详细信息中一项为空，则从SG中查询	
            if(PNULL == purchase_item_ptr)
            {
                anyURI_len = 0;
                SCI_MEMSET(&global_purchase_item_Id, 0, sizeof(anyURI));
                global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];	     
                while(0 != global_purchase_item_Id.anyURI_arr[anyURI_len] && anyURI_len < (SG_MAX_URI_LEN - 1))
                {
                    anyURI_len++;
                    global_purchase_item_Id.anyURI_arr[anyURI_len] = purchase_table_info_ptr->global_purchase_item_id[anyURI_len];
                }
                //SCI_TRACE_LOW:"MMIMBBMS_OpenPurchaseOrderQueryWin anyURI_len = %d\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2803_112_2_18_2_39_52_424,(uint8*)"d", anyURI_len);
                if(0 == anyURI_len)
                {
                    //获取到的套餐标识为空
                    MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);	
                    return;
                }
                global_purchase_item_Id.anyURI_ptr = PNULL;
                global_purchase_item_Id.string_len = anyURI_len;
                MBBMSSG_GetPurchaseItemId(global_purchase_item_Id, &itemId);
            	
                purchase_item_error_code = MBBMSSG_GetPurchaseItem(itemId, &item_info);
                if(SG_ERROR_NONE != purchase_item_error_code)	
                {
                    MBBMSSG_FreePurchaseItem(&item_info);
                    MBBMSSG_FreeTypeAnyURI(&itemId);
                    MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);
                    //SCI_TRACE_LOW:"MMIMBBMS_OpenPurchaseOrderQueryWin purchase_item_error_code =%dn"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2821_112_2_18_2_39_52_425,(uint8*)"d",purchase_item_error_code);
                    return;            
                }
                purchase_item_ptr = &item_info;	
            }   
            MMIMBBMS_GetNameWchar(purchase_item_ptr->NameList, wchar_name + prompt_str1.wstr_len,
                (MMIMBBMS_PURCHASE_NAME_MAX_LEN - prompt_str1.wstr_len));        
           
        }
        MMIAPICOM_Wstrncpy(wchar_name + MMIAPICOM_Wstrlen(wchar_name), 
            wchar_query, 
            MMIAPICOM_Wstrlen(wchar_query));   

        MBBMSSG_FreePurchaseItem(&item_info);
        MBBMSSG_FreeTypeAnyURI(&itemId);
        MBBMSSG_FreeTypeAnyURI(&global_purchase_item_Id);	
        
        prompt_str.wstr_ptr = wchar_name;
        prompt_str.wstr_len = MMIAPICOM_Wstrlen(wchar_name);
        
        MMIPUB_OpenQueryTextWinByTextPtr(&prompt_str, PNULL, HandleUnSubscribeQueryWinMsg );
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_SUB_QUERY, &prompt_str1);
        
        SCI_MEMSET(wchar_name, 0, sizeof(wchar_name));
        
        MMIAPICOM_Wstrncpy(wchar_name, prompt_str1.wstr_ptr, 
            prompt_str1.wstr_len);
        
        err_code = MBBMSSG_GetPurchaseItemIndex(&purchaseIndex_list_head_ptr);
        
        if(SG_ERROR_NONE != err_code)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_OpenPurchaseOrderQueryWin:Get Purchase Item failed\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_2856_112_2_18_2_39_52_426,(uint8*)"");
            MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
            return ;
        }
        
        purchase_index = MMIMBBMS_GetCurrPurchaseIndex();    
        //get purchase index node
        purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)STL_LinkListGetNodeDataByIdx(
            purchaseIndex_list_head_ptr,
            purchase_index);
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        
        if(purchase_index_ptr != PNULL)
        {
            //get purchase item
            MBBMSSG_GetPurchaseItem(purchase_index_ptr->index, &item_info);
            
            MMIMBBMS_GetNameWchar(item_info.NameList, wchar_name + prompt_str1.wstr_len,
                (MMIMBBMS_PURCHASE_NAME_MAX_LEN - prompt_str1.wstr_len));
            MBBMSSG_FreePurchaseItem(&item_info);
        }
        
        MMIAPICOM_Wstrncpy(wchar_name + MMIAPICOM_Wstrlen(wchar_name), 
            wchar_query, 
            MMIAPICOM_Wstrlen(wchar_query));
        
        prompt_str.wstr_ptr = wchar_name;
        prompt_str.wstr_len = MMIAPICOM_Wstrlen(wchar_name);
        
        MMIPUB_OpenQueryTextWinByTextPtr(&prompt_str, PNULL, HandleSubscribeQueryWinMsg );    
    }
    
    return;
}

/*****************************************************************************/
//  Description : handle message of subscrible list window
//  Global resource dependence : none
//  Author:
//  Note: 套餐订购管理 列表窗口，包含本地套餐、已订套餐两项
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleSubscribeTabWinMsg(
                                                      MMI_WIN_ID_T     win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;	
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        {  
            MMK_SetAtvCtrl(win_id, ctrl_id);  
            MMIMBBMS_SubscribeManageWinAppendTabInfo();
            GUITAB_SetFontParam(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, MMI_DEFAULT_NORMAL_FONT,
                MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0), 0);
            GUITAB_SetTitleTextId(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, TXT_MBBMS_PURCHASE_MANAGE);
            break;
        }
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
            break;
        }

    case MSG_CLOSE_WINDOW:
        {			
            MMIMBBMS_FreeAccountInquiryRes();
            break;
        }
        
    default:
        {
            //SCI_TRACE_LOW("MMIMBBMS_HandleSubscribeTabWinMsg() msg_id=%d", msg_id);
            recode = MMI_RESULT_FALSE;
            break;
        }
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : MMIMBBMS_HandlePurchasedItemChildWinMsg
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note: 已订购套餐子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandlePurchasedItemChildWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param)
{
    uint16 item_num = 0;
    //BOOLEAN bret = FALSE;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;	
    MMI_CTRL_ID_T  ctrl_id = MMIMBBMS_PURCHASEDITEM_LISTBOX_CTRL_ID;
    uint16 index = 0;
    //uint32 global_index = 0;
    MBBMS_Account_Inquiry_Res_t* account_inquiry_res_ptr = MMIMBBMS_GetAccountInquiryRes();
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        {
            MMK_SetAtvCtrl(win_id, ctrl_id);     
            MMIMBBMS_DisplayAllPurchasedItem(ctrl_id, account_inquiry_res_ptr);			
            
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (0 == item_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }       
            break;
        }
    case MSG_NOTIFY_TAB_SWITCH:    	
        {
            GUILIST_RemoveAllItems(ctrl_id);
            MMIMBBMS_DisplayAllPurchasedItem(ctrl_id, account_inquiry_res_ptr);
            
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (0 == item_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            
            break;
        }
    case MSG_GET_FOCUS:	
        {
            GUILIST_RemoveAllItems(ctrl_id);
            MMIMBBMS_DisplayAllPurchasedItem(ctrl_id, account_inquiry_res_ptr);			
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (0 == item_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            
            break;
        }		
    
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (item_num > 0)
            {
                s_is_enter_detail_from_purchased_item = TRUE;	
                index = GUILIST_GetCurItemIndex(ctrl_id);
                MMIMBBMS_SetCurrPurchasedItemDisplayInfo(index, account_inquiry_res_ptr);                
                //start subscrible
                if(!MMIMBBMS_SDandOpenCondition())
                {
                    MMK_CloseWin(win_id);
                    recode = MMI_RESULT_FALSE;
                    break;
                }
                
                if (MMK_IsOpenWin(MMIMBBMS_PURCHASE_ORDER_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_PURCHASE_ORDER_WIN_ID);
                }
                
                MMK_CreateWin((uint32*)MMIMBBMS_PURCHASEITEM_ORDER_TAB, PNULL);
            }
            
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:      	
        {
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (item_num > 0)
            {
                s_is_enter_detail_from_purchased_item = TRUE;	            
                index = GUILIST_GetCurItemIndex(ctrl_id);    
                MMIMBBMS_SetCurrPurchasedItemDisplayInfo(index, account_inquiry_res_ptr);                
                if (MMK_IsOpenWin(MMIMBBMS_PURCHASEITEM_DETAIL_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_PURCHASEITEM_DETAIL_WIN_ID);
                }
                
                //套餐详情
                MMK_CreateWin((uint32*)MMIMBBMS_PURCHASEITEM_DETAIL_TAB, PNULL);
            }
            
            break;
        }
        
    default:
        //SCI_TRACE_LOW("MMIMBBMS_HandlePurchasedItemChildWinMsg() msg_id=%d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
}

/*****************************************************************************/
//  Description : MMIMBBMS_HandleLocalPurchaseItemChildWinMsg
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note: 本地套餐子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleLocalPurchaseItemChildWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param)
{
    uint16          item_num = 0;
    BOOLEAN         bret = FALSE;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;	
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_LOCAL_PURCHASEITEM_LISTBOX_CTRL_ID;
    uint16          index = 0;
    uint32          global_index = 0;
    MMIMBBMS_SUBSCRIPTE_TYPE subscripte_type = MMIMBBMS_SUBSCRIPTE_TYPE_LOCAL_PURCHASE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        {
            MMK_SetAtvCtrl(win_id, ctrl_id);     
            MMIMBBMS_DisplayAllPurchaseItem(subscripte_type, ctrl_id);
            
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (0 == item_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }   
            break;
        }

    case MSG_NOTIFY_TAB_SWITCH:
        {
            GUILIST_RemoveAllItems(ctrl_id);
            MMIMBBMS_DisplayAllPurchaseItem(subscripte_type, ctrl_id);
            
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (0 == item_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            
            break;
        }
    case MSG_GET_FOCUS:
        {
            GUILIST_RemoveAllItems(ctrl_id);
            MMIMBBMS_DisplayAllPurchaseItem(subscripte_type, ctrl_id);			
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (0 == item_num)
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            
            break;
        }    
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (item_num > 0)
            {
                s_is_enter_detail_from_purchased_item = FALSE;	
                index = GUILIST_GetCurItemIndex(ctrl_id);
                
                bret = MMIMBBMS_GetGlobalIndexFromTwoIndex(subscripte_type, index, &global_index);
                
                if (TRUE == bret)
                {
                    MMIMBBMS_SetCurrPurchaseDisplayInfo(global_index);
                }
                else
                {
                    /* 异常 */
                    MMIMBBMS_SetCurrPurchaseDisplayInfo(index);
                }
                
                //start subscrible
                if(!MMIMBBMS_SDandOpenCondition())
                {
                    MMK_CloseWin(win_id);
                    recode = MMI_RESULT_FALSE;
                    break;
                }
                
                if (MMK_IsOpenWin(MMIMBBMS_PURCHASE_ORDER_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_PURCHASE_ORDER_WIN_ID);
                }
                
                MMK_CreateWin((uint32*)MMIMBBMS_PURCHASEITEM_ORDER_TAB, PNULL);
            }
            
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_NONE);
        MMK_CloseParentWin(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_WIN_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:      	
        {
            item_num = GUILIST_GetTotalItemNum(ctrl_id);
            if (item_num > 0)
            {
                s_is_enter_detail_from_purchased_item = FALSE;	
                index = GUILIST_GetCurItemIndex(ctrl_id);
                
                bret = MMIMBBMS_GetGlobalIndexFromTwoIndex(subscripte_type, index, &global_index);
                
                if (TRUE == bret)
                {
                    MMIMBBMS_SetCurrPurchaseDisplayInfo(global_index);
                }
                else
                {
                    /* 异常 */
                    MMIMBBMS_SetCurrPurchaseDisplayInfo(index);
                }
                
                if (MMK_IsOpenWin(MMIMBBMS_PURCHASEITEM_DETAIL_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_PURCHASEITEM_DETAIL_WIN_ID);
                }
                
                //套餐详情
                MMK_CreateWin((uint32*)MMIMBBMS_PURCHASEITEM_DETAIL_TAB, PNULL);
            }
            
            break;
        }
        
    default:
        //SCI_TRACE_LOW("MMIMBBMS_HandleLocalPurchaseItemChildWinMsg() msg_id=%d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
    
}

/*****************************************************************************/
// Description : 根据套餐类型(本地套餐或已订套餐)和index(是指在本地套餐或已订套餐列表中的index)
//               得到对应的purchase item在所有套餐列表中对应的全局 index
// Global resource dependence : none
// Author:
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetGlobalIndexFromTwoIndex(uint32 purchase_type,
                                                  uint16 inner_index,
                                                  uint32* pGlobalIndex)
{
    uint32 purchase_num = 0;
    uint32 local_index = 0;
    uint32 exception_index = 0;
    
    uint32 index=0;
    anyURI globalItemID = {0};
    SG_ERROR_E ret = SG_ERROR_NONE;
    BOOLEAN    is_subscribed = FALSE;
    SG_LIST_PTR_T purchaseIndex_list_head_ptr = PNULL;
    MBBMSSG_PURCHASEITEM_INDEX_T* purchase_index_ptr= PNULL;
    
    
    ret = MBBMSSG_GetPurchaseItemIndex(&purchaseIndex_list_head_ptr);
    
    if(SG_ERROR_NONE !=ret)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetGlobalIndexFromTwoIndex:Get Purchase Item failed\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_3232_112_2_18_2_39_53_427,(uint8*)"");
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        return FALSE;
    }
    
    purchase_num = STL_LinkListGetNodeDataCount(purchaseIndex_list_head_ptr);
    
    if(0==purchase_num)
    {
        //套餐列表为空
        //SCI_TRACE_LOW:"MMIMBBMS_GetGlobalIndexFromTwoIndex ---no purchaseitem\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_3242_112_2_18_2_39_53_428,(uint8*)"");
        MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
        return FALSE;
    }            
    
    //SCI_TRACE_LOW:"MMIMBBMS_GetGlobalIndexFromTwoIndex total purchase num=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_3247_112_2_18_2_39_53_429,(uint8*)"d",purchase_num);
    
    //Get every purchase item 
    for(index=0; index<purchase_num; index++)
    {
        purchase_index_ptr = (MBBMSSG_PURCHASEITEM_INDEX_T*)
            STL_LinkListGetNodeDataByIdx(purchaseIndex_list_head_ptr,index);
        
        if (PNULL == purchase_index_ptr)
        {
            exception_index ++;
            continue;
        }
        
        SCI_MEMSET(&globalItemID,0,sizeof(anyURI));
        MBBMSSG_GetGlobalPurchaseItemtId(purchase_index_ptr->index, &globalItemID);
        ret = MBBMSSG_IsPurchaseItemSubscribed(purchase_index_ptr->index, &is_subscribed);
        
        // 本地套餐显示所有套餐
        if ( !((MMIMBBMS_SUBSCRIPTE_TYPE_LOCAL_PURCHASE == purchase_type && (!is_subscribed))
            || (MMIMBBMS_SUBSCRIPTE_TYPE_PURCHASED == purchase_type && MMIMBBMS_IsNationalPurchaseItem(&globalItemID) == TRUE
            && ret == SG_ERROR_NONE && TRUE == is_subscribed)))
        {
            MBBMSSG_FreeTypeAnyURI(&globalItemID);
            continue;
        }
        
        if (inner_index == local_index)
        {
            *pGlobalIndex = index - exception_index;
            MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
            MBBMSSG_FreeTypeAnyURI(&globalItemID);
            return TRUE;
        }
        
        local_index++;
        MBBMSSG_FreeTypeAnyURI(&globalItemID);
    }
    
    MBBMSSG_FreePurchaseItemIndex(&purchaseIndex_list_head_ptr);
    return FALSE;
}

/*****************************************************************************/
//     Description : 套餐详情
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandlePurchaseItemDetailWinMsg(
                                                           MMI_WIN_ID_T    win_id, 
                                                           MMI_MESSAGE_ID_E    msg_id, 
                                                           DPARAM                param)
{
    
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;	
    MMI_CTRL_ID_T  ctrl_id = MMIMBBMS_PURCHASEITEM_DETAIL_RICHTEXT_CTRL_ID;
    MBBMS_Account_Inquiry_Res_t* account_inquiry_res_ptr = MMIMBBMS_GetAccountInquiryRes();	
    
    //SCI_TRACE_LOW:"MMIMBBMS_HandlePurchaseItemDetailWinMsg() enter:msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_3304_112_2_18_2_39_53_430,(uint8*)"d",msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(s_is_enter_detail_from_purchased_item)
        {
            MMIMBBMS_DisplayServiceOrContentInPurchasedItem(ctrl_id, account_inquiry_res_ptr);
        }
        else
        {
            MMIMBBMS_DisplayServiceOrContentInPurchase(ctrl_id);
        }
        
        if(MMIMBBMS_GetCurrPurchaseOrdereStatus())
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_MBBMS_UNSUBSCRIBE, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_MBBMS_SUBSCRIBE, TXT_NULL, STXT_RETURN, FALSE);
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIMBBMS_OpenPurchaseOrderQueryWin();	
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        //SCI_TRACE_LOW:"MMIMBBMS_HandlePurchaseItemDetailWinMsg() msg_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_3345_112_2_18_2_39_53_431,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}


/*****************************************************************************/
//     Description : MMIMBBMS_HandlePurchaseOrderWinMsg
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandlePurchaseOrderWinMsg(
                                                      MMI_WIN_ID_T      win_id, 
                                                      MMI_MESSAGE_ID_E  msg_id, 
                                                      DPARAM            param)
{
    
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;	
    MMI_CTRL_ID_T  ctrl_id = MMIMBBMS_PURCHASE_ORDER_RICHTEXT_CTRL_ID;
    MBBMS_Account_Inquiry_Res_t* account_inquiry_res_ptr = MMIMBBMS_GetAccountInquiryRes();	
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(s_is_enter_detail_from_purchased_item)
        {
            MMIMBBMS_DisplayServiceOrContentInPurchasedItem(ctrl_id, account_inquiry_res_ptr);
        }
        else
        {
            MMIMBBMS_DisplayServiceOrContentInPurchase(ctrl_id);
        }
        if(MMIMBBMS_GetCurrPurchaseOrdereStatus())
        {
            GUIWIN_SetTitleTextId(win_id, TXT_MBBMS_UN_PURCHASE_ORDER, FALSE);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_MBBMS_PURCHASE_SUBMIT_UNORDER, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetTitleTextId(win_id, TXT_MBBMS_PURCHASE_ORDER, FALSE);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_MBBMS_PURCHASE_SUBMIT_ORDER, TXT_NULL, STXT_RETURN, FALSE);
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIMBBMS_OpenPurchaseOrderQueryWin();	
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        //SCI_TRACE_LOW("MMIMBBMS_HandlePurchaseItemDetailWinMsg() msg_id=%d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubscribeRequestWinMsg( 
                                                MMI_WIN_ID_T     win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN stopflow_result = FALSE;   
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
        break;
        
    //case MSG_APP_RED:	 	
    case MSG_APP_CANCEL:
        stopflow_result = MMIMBBMS_StopFlow();
		if(stopflow_result)
		{
            MMK_CloseWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID);		
            MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitWin(FALSE);
		}
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSubscribeQueryWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
                                               
{
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMIMBBMS_StartSubscribeProcess();
            MMK_CloseWin(win_id);
            break;
        }
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
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnSubscribeRequestWinMsg( 
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	BOOLEAN stopflow_result = FALSE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
        break;
        
        //case MSG_APP_RED:
    case MSG_APP_CANCEL:	
        stopflow_result = MMIMBBMS_StopFlow();
		if(stopflow_result)
		{
            MMK_CloseWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID);		
            MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitWin(TRUE);
		}	
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandlePauseServiceAlertMsg
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUnSubscribeQueryWinMsg(
                                                 MMI_WIN_ID_T win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 )
{
    
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMIMBBMS_StartUnSubscribeProcess();
            MMK_CloseWin(win_id);
            break;
        }
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
//  Description : Open program list win
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenProgramListWin(void)
{
    if (MMK_IsOpenWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_PROGRAM_CHILD_WIN_ID);
        MMK_CloseWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID);
    }
    
    MMK_CreateParentWin((uint32 *)MMIMBBMS_PROGRAM_LIST_TAB,PNULL);
    MMK_CreateChildWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID, (uint32*)MMIMBBMS_PROGRAM_CHILD_TAB, NULL);
    
    MMK_SetChildWinFocus(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID, MMIMBBMS_PROGRAM_CHILD_WIN_ID);
    
    return;
}

/*****************************************************************************/
//     Description : handle message of progrm list window msg
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleProgramListWinMsg(
                                                    MMI_WIN_ID_T     win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM           param)
{
    MMI_CTRL_ID_T    ctrl_id = MMIMBBMS_PROGRAM_TAB_CTRL_ID;
    MMI_RESULT_E     recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUITAB_SetTitleIcon(ctrl_id, IMAGE_MTV_LOGO);
            
            MMIMBBMS_AddTabsAndChildWindows();
            
            GUITAB_SetTitleTextId(ctrl_id, TXT_MBBMS_PROGRM_LIST);
            
            GUITAB_SetFontParam(ctrl_id, MMI_DEFAULT_NORMAL_FONT,
                MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0), 0);
            break;
        }
        
        //* #键响应日期切换
    case MSG_APP_STAR:
        LeftButtonCallBack();
        break;
        
    case MSG_APP_HASH:
        RightButtonCallBack();
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(win_id);
        break;
        
    default:
        //SCI_TRACE_LOW("MMIMBBMS_HandlePurchaseItemDetailWinMsg() msg_id=%d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
    
}

/*****************************************************************************/
// 	Description : add all tabs and all child windows to parent window.
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void  MMIMBBMS_AddTabsAndChildWindows(void)
{
    uint32  i = 0;
    GUITAB_ITEM_T         item_t = {0};
    GUITAB_ITEM_CONTENT_T item_data_t = {0};
    
    MTVSERVICE_PROGRAM_LIST_T service_list_t = {0};
    MTVSERVICE_PROGRAM_T      *service_arry_ptr = PNULL;
    uint32 min_count = 0;
    
    item_t.child_win_handle = MMIMBBMS_PROGRAM_CHILD_WIN_ID;
    item_t.item_state = GUITAB_ITEMSTATE_SHOW_TEXT;
    item_t.item_data_ptr = &item_data_t;
    item_data_t.text_data.is_disp_font_bg = TRUE;
    item_data_t.text_data.text_type = TABITEM_DATA_TEXT_BUFFER;
    
    MMIMBBMS_GetProgramList(&service_list_t);
    service_arry_ptr = service_list_t.list_ptr;
    
	min_count = MIN(service_list_t.total_count, GUITAB_MAX_ITEM_NUM);
    GUITAB_SetMaxItemNum(MMIMBBMS_PROGRAM_TAB_CTRL_ID, min_count);
    GUITAB_SetItemSpace(MMIMBBMS_PROGRAM_TAB_CTRL_ID,MMIMBBMS_MAINTAB_ITEM_SPACE,FALSE);
    //GUITAB_SetState(MMIMBBMS_PROGRAM_TAB_CTRL_ID,GUITAB_STATE_NONEED_SCROLL,TRUE);    
    
    /* 取所有频道名称，并加入到TAB控件中 */
    for(i=0; i<min_count; i++)
    {
        SCI_MEMSET(item_data_t.text_data.text.text_buffer.wstr, 0, sizeof(item_data_t.text_data.text.text_buffer.wstr));
        
        //get service name
        if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
        {
            GetNameWcharByLangString(&service_arry_ptr[i].name, 
                MBBMS_SERVICE_MODE_MBBMS,
                0,
                0,
                item_data_t.text_data.text.text_buffer.wstr,
                GUITAB_MAX_ITEM_NAME_LENGTH);
        }
        else
        {
            GetNameWcharByLangString(&service_arry_ptr[i].name, 
                MBBMS_SERVICE_MODE_CMMB,
                service_arry_ptr[i].char_set,
                service_arry_ptr[i].service_id,
                item_data_t.text_data.text.text_buffer.wstr,
                GUITAB_MAX_ITEM_NAME_LENGTH);
        }
        item_data_t.text_data.text.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data_t.text_data.text.text_buffer.wstr);
        
        GUITAB_AppendItem(MMIMBBMS_PROGRAM_TAB_CTRL_ID, &item_t);
    }
    
    if (s_main_service_index < min_count)
    {
        GUITAB_SetCurSel(MMIMBBMS_PROGRAM_TAB_CTRL_ID, s_main_service_index);
    }
    else
    {
        GUITAB_SetCurSel(MMIMBBMS_PROGRAM_TAB_CTRL_ID, 0);
    }
    
    return;
}

/*****************************************************************************/
//  Description : handle message of program list child window
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleProgramListChildWinMsg(
                                                         MMI_WIN_ID_T    win_id, 
                                                         MMI_MESSAGE_ID_E    msg_id, 
                                                         DPARAM                param)

{
    MMI_RESULT_E     recode = MMI_RESULT_TRUE;    
    GUI_RECT_T       srect = {0};
    
    SG_DATETIME_T date = {0};  
    SCI_DATE_T       sysdate = {0};
    LOCAL anyURI globalContentId = {0};
    char strDate[MMIMBBMS_MAX_DATE_STR_LENS] = {0};
    wchar wchar_Date[MMIMBBMS_MAX_DATE_STR_LENS] = {0};
    MMI_STRING_T   str_date = {0};
    MMI_STRING_T   str_wday = {0};
    MMI_STRING_T   str_day3 = {0};
    MMI_STRING_T   str_temp = {0};
    MMI_STRING_T   str_merge = {0};
    wchar wchar_temp[30] = {0};
    wchar wchar_merge[50] = {0};
    static uint32 s_list_index = 0;
    static uint16 s_proglist_main_index = 0;
    //MMI_HANDLE_T     button_handle = NULL;
    GUI_BG_T         gray_bg = {0};
    GUI_RECT_T      list_rect = {0};
    GUI_RECT_T      button_rect1 = {0, (MMIMBBMS_CONTENT_LBUTTON_TOP+1), (MMIMBBMS_CONTENT_LBUTTON_WIDTH-1), MMI_CLIENT_RECT_BOTTOM};
    GUI_RECT_T      button_rect2 = {MMIMBBMS_CONTENT_RBUTTON_LEFT, (MMIMBBMS_CONTENT_LBUTTON_TOP+1), MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM};
    static uint16 s_proglist_service_index = 0; //记录失去焦点时的频道号
    static uint16 s_proglist_day_index = 0; //记录失去焦点时的日期index    
    
    srect.left = 0;
    srect.top = MMI_STATUSBAR_HEIGHT + MMI_TAB_BOTTOM;
    srect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    srect.bottom = MMI_CLIENT_RECT_BOTTOM;

    list_rect.left = 0;
    list_rect.top = MMI_STATUSBAR_HEIGHT + MMI_TAB_BOTTOM;
    list_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    list_rect.bottom = MMIMBBMS_CONTENT_LBUTTON_TOP;

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        {
            MMK_SetAtvCtrl(win_id, MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID);

            GUIBUTTON_SetRect(MMIMBBMS_BUTTON_LEFT_CTRL_ID,&button_rect1);
            GUIBUTTON_SetRect(MMIMBBMS_BUTTON_RIGHT_CTRL_ID,&button_rect2);
	        {
	        	GUI_RECT_T lab_rect = {MMIMBBMS_CONTENT_LABEL_LEFT, MMIMBBMS_CONTENT_LBUTTON_TOP + 1, MMIMBBMS_CONTENT_LABEL_RIGHT,MMI_CLIENT_RECT_BOTTOM};
				GUILABEL_SetRect(MMIMBBMS_LABEL_DAY_CTRL_ID, &lab_rect, FALSE);
	        }                
            GUILIST_SetRect(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID,&list_rect);

            s_list_index = 0;
            s_proglist_service_index = 0;
            s_proglist_day_index = 0;
            s_day_index = 0;
            s_playing_content_index = 0;

            TM_GetSysDate(&sysdate);
            date.years = sysdate.year;
            date.months = sysdate.mon;
            date.days = sysdate.mday;
            date.hours = 0;
            date.minutes = 0;
            date.seconds = 0;
            
            s_proglist_main_index = s_main_service_index;
            MMIMBBMS_LoadListItem(s_proglist_main_index, &date);
            GUILIST_SetCurItemIndex(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID, s_playing_content_index);
            
            GUILABEL_SetBackgroundColor(MMIMBBMS_LABEL_DAY_CTRL_ID, MMI_RED_COLOR);
            
            sprintf(strDate, "%d-%d ", sysdate.mon, sysdate.mday);
            
            MMIAPICOM_StrToWstr((uint8*)strDate, wchar_Date);
            str_date.wstr_ptr = wchar_Date;
            str_date.wstr_len = MMIAPICOM_Wstrlen(wchar_Date);
            
            /*MMI_GetLabelTextByLang(s_mbbms_weekday_info[sysdate.wday], &str_wday);
            
            str_temp.wstr_ptr = wchar_temp;
            MMIAPICOM_MergeTwoString(&str_date, &str_wday, &str_temp, 30);
            str_temp.wstr_len = MMIAPICOM_Wstrlen(wchar_temp);
            MMI_GetLabelTextByLang(s_day_textid[s_day_index], &str_day3);
            str_merge.wstr_ptr = wchar_merge;
            MMIAPICOM_MergeTwoString(&str_temp, &str_day3, &str_merge, 50);*/ //不必须显示周几，增加周几的显示需要考虑label不能完全显示的问题
            MMI_GetLabelTextByLang(s_day_textid[s_day_index], &str_day3);            
            str_merge.wstr_ptr = wchar_merge;
            MMIAPICOM_MergeTwoString(&str_date, &str_day3, &str_merge, 50);
            str_merge.wstr_len = MMIAPICOM_Wstrlen(wchar_merge);
			
			GUILABEL_SetFont(MMIMBBMS_LABEL_DAY_CTRL_ID, MMI_DEFAULT_NORMAL_FONT, MMI_WHITE_COLOR);            
            GUILABEL_SetText(MMIMBBMS_LABEL_DAY_CTRL_ID,
                &str_merge, FALSE);
            
            MMIMBBMS_RefreshProgListMiddleSoftkey(win_id);
            GUIBUTTON_SetCallBackFunc(MMIMBBMS_BUTTON_LEFT_CTRL_ID, LeftButtonCallBack);
            GUIBUTTON_SetCallBackFunc(MMIMBBMS_BUTTON_RIGHT_CTRL_ID, RightButtonCallBack);
            gray_bg.bg_type = GUI_BG_IMG;
            
            if(0 == s_day_index)
            {
                //gray left button
                gray_bg.img_id  = IMAGE_COMMON_LBTN_GRAY;
                GUIBUTTON_SetGrayed(MMIMBBMS_BUTTON_LEFT_CTRL_ID,TRUE,&gray_bg,FALSE);
            }
            else if(s_day_index == M_MMI_MBBMS_MAX_DAY_NUM - 1)
            {
                //gray right button
                gray_bg.img_id  = IMAGE_COMMON_RBTN_GRAY;
                GUIBUTTON_SetGrayed(MMIMBBMS_BUTTON_RIGHT_CTRL_ID,TRUE, &gray_bg,FALSE);
            }
            break;
        }
        
    case MSG_APP_MBBMS_REFRESH_LIST:
    case MSG_NOTIFY_TAB_SWITCH:
        {
            s_list_index = GUILIST_GetCurItemIndex(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID);
            MMIMBBMS_UpdateContentList();
            
            
            MMIMBBMS_RefreshProgListMiddleSoftkey(win_id);
           
            
            s_proglist_main_index = GUITAB_GetCurSel(MMIMBBMS_PROGRAM_TAB_CTRL_ID);
            //if change service then set the focus to the playing content if no playing set focus to zero
            if(MSG_NOTIFY_TAB_SWITCH == msg_id)
            {
                GUILIST_SetCurItemIndex(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID, s_playing_content_index);
            }            
            
            break;
        }
    case MSG_GET_FOCUS:
        {
            s_proglist_main_index = GUITAB_GetCurSel(MMIMBBMS_PROGRAM_TAB_CTRL_ID);;
            
            MMIMBBMS_RefreshProgListMiddleSoftkey(win_id);
            if (s_list_index < GUILIST_GetTotalItemNum(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID)
                && s_proglist_service_index == s_proglist_main_index && s_proglist_day_index == s_day_index)
            {
                GUILIST_SetCurItemIndex(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID, s_list_index);
            }

#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
             /* 如果没有节目，则直接break */
            if (0 == GUILIST_GetTotalItemNum(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID) )
            {
                break;
            }
            /*add for init not finished*/
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }    
            
            s_main_service_index = s_proglist_main_index;
            if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
            {   
                if(!MMIMBBMS_GetSubscriptionUpdateStatus())
                {
                    MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                    s_object_type=MMIMBBMS_OBJECT_FROM_PROGROM_LIST;
                    MMIMBBMS_OpenSubscriptionUpdateQueryWin();
                }
                else
                {
                    MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_PROGROM_LIST);
                }
            }
            else
            {
                //cmmb mode
                MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_PROGROM_LIST);
            }
            
            break; 
        }
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        {
            SG_LIST_PTR_T pItem = PNULL;
            MBBMSSG_CONTENT_INDEX_T *pcontentidex = PNULL;
            SG_ERROR_E error_code = SG_ERROR_NONE;
            
            SG_LIST_PTR_T listptr = PNULL;
            
            if(GUILIST_GetTotalItemNum(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID) == 0)
            {
                break;
            }
            
            s_content_idx = GUILIST_GetCurItemIndex(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID);
            
            //根据index得到对应的content节点            
            pItem = MMIMBBMS_GetContentNodeByIdx(s_proglist_main_index, s_content_idx, &listptr);
            
            if(MMK_IsOpenWin(MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_ID))
            {
                MMK_CloseWin(MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_ID);
            }
            
            if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
            {
                if(PNULL != pItem)
                {
                    //判断是否已加入收藏频道
                    SCI_MEMSET(&globalContentId,0,sizeof(anyURI));
                    pcontentidex = (MBBMSSG_CONTENT_INDEX_T*)(pItem->data);
                    error_code = MBBMSSG_GetGlobalContentId(pcontentidex->index, &globalContentId );
                    
                    MMK_CreateWin((uint32 *)MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_TAB, (ADD_DATA)&globalContentId); 
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_TAB, (ADD_DATA)PNULL); 
                }
                
                MBBMSSG_FreeContentIndex(&listptr);
            }
            else
            {
                //cmmb mode
                MMK_CreateWin((uint32 *)MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_TAB, (ADD_DATA)PNULL); 
            }
            
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID);
        //MBBMSSG_FreeTypeAnyURI(&globalContentId);
        break;

    case MSG_LOSE_FOCUS:
        {
	      //失去焦点时需要记录当前list高亮项和tab当前选项
            s_list_index = GUILIST_GetCurItemIndex(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID);
            s_proglist_service_index = GUITAB_GetCurSel(MMIMBBMS_PROGRAM_TAB_CTRL_ID);	
            s_proglist_day_index = s_day_index;
            break;
        }        
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : get selected date
//  Global resource dependence :
//  Author:plum.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetSelectedDate(SCI_DATE_T* pDate, uint16 day_index)
{
    uint32     uSeconds = 0; 
    SCI_DATE_T sysdate  = {0};
    MMI_TM_T   tm_sel   = {0};
    
    TM_GetSysDate(&sysdate);
    
    uSeconds =  MMIAPICOM_Tm2Second(0, 0, 0, sysdate.mday, sysdate.mon, sysdate.year);
    uSeconds += 3600 * 24 * day_index;
    
    tm_sel = MMIAPICOM_Second2Tm(uSeconds);
    
    pDate->mday = tm_sel.tm_mday;
    pDate->mon  = tm_sel.tm_mon;
    pDate->year = tm_sel.tm_year;
    pDate->wday = (sysdate.wday + day_index) % 7;
    
    return;
    
}

/*****************************************************************************/
//  Description : left keystoke callback
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ButtonCallBack(void)
{
    SCI_DATE_T    sysdate = {0};
    char strDate[MMIMBBMS_MAX_DATE_STR_LENS] = {0};
    wchar wchar_Date[MMIMBBMS_MAX_DATE_STR_LENS] = {0};
    MMI_STRING_T   str_date = {0};
    MMI_STRING_T   str_wday = {0};
    MMI_STRING_T   str_day3 = {0};
    MMI_STRING_T   str_temp = {0};
    MMI_STRING_T   str_merge = {0};
    wchar wchar_temp[30] = {0};
    wchar wchar_merge[50] = {0};
    
    MMIMBBMS_UpdateContentList();
    
    MMIMBBMS_GetSelectedDate(&sysdate, s_day_index);
    
    sprintf(strDate, "%d-%d ", sysdate.mon, sysdate.mday);
    
    MMIAPICOM_StrToWstr((uint8*)strDate, wchar_Date);
    str_date.wstr_ptr = wchar_Date;
    str_date.wstr_len = MMIAPICOM_Wstrlen(wchar_Date);
    
    /*MMI_GetLabelTextByLang(s_mbbms_weekday_info[sysdate.wday], &str_wday);
    
    str_temp.wstr_ptr = wchar_temp;
    MMIAPICOM_MergeTwoString(&str_date, &str_wday, &str_temp, 30);
    str_temp.wstr_len = MMIAPICOM_Wstrlen(wchar_temp);
    MMI_GetLabelTextByLang(s_day_textid[s_day_index], &str_day3);
    str_merge.wstr_ptr = wchar_merge;
    MMIAPICOM_MergeTwoString(&str_temp, &str_day3, &str_merge, 50);
    str_merge.wstr_len = MMIAPICOM_Wstrlen(wchar_merge);*/ //不必须显示周几，增加周几的显示需要考虑label不能完全显示的问题
    MMI_GetLabelTextByLang(s_day_textid[s_day_index], &str_day3);            
    str_merge.wstr_ptr = wchar_merge;
    MMIAPICOM_MergeTwoString(&str_date, &str_day3, &str_merge, 50);
    str_merge.wstr_len = MMIAPICOM_Wstrlen(wchar_merge);
	
    GUILABEL_SetText(MMIMBBMS_LABEL_DAY_CTRL_ID,
        &str_merge, TRUE);
    //if change the date then set the index
    GUILIST_SetCurItemIndex(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID, s_playing_content_index);    
    
    MMK_SetAtvCtrl(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID);
    
    MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
    
    return;
}

/*****************************************************************************/
//  Description : left keystoke callback
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LeftButtonCallBack(void)
{
    MMI_RESULT_E  result  = MMI_RESULT_TRUE;
    GUI_BG_T      gray_bg = {0};
    
    gray_bg.bg_type = GUI_BG_IMG;
    if (s_day_index > 0)
    {
        //if right button grayed, resume it 
        gray_bg.img_id  = IMAGE_COMMON_RBTN_GRAY;
        GUIBUTTON_SetGrayed(MMIMBBMS_BUTTON_RIGHT_CTRL_ID, FALSE,&gray_bg, FALSE);
        s_day_index--;   
        ButtonCallBack();
    }
    
    if(0 == s_day_index)
    {
        //gray button
        gray_bg.img_id  = IMAGE_COMMON_LBTN_GRAY;
        GUIBUTTON_SetGrayed(MMIMBBMS_BUTTON_LEFT_CTRL_ID, TRUE, &gray_bg, FALSE);
    }
    
    return result;
}

/*****************************************************************************/
//     Description : MMIMBBMS_UpdateContentList
//    Global resource dependence : 
//  Author:       peng
//    Note:   
/*****************************************************************************/
LOCAL void MMIMBBMS_UpdateContentList(void)
{
    uint16 index = 0;
    SG_DATETIME_T date = {0};  
    SCI_DATE_T       sysdate = {0};
    
    GUILIST_RemoveAllItems(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID);
    
    MMIMBBMS_GetSelectedDate(&sysdate, s_day_index);
    date.years = sysdate.year;
    date.months = sysdate.mon;
    date.days = sysdate.mday;
    
    
    index = GUITAB_GetCurSel(MMIMBBMS_PROGRAM_TAB_CTRL_ID);
    
    MMIMBBMS_LoadListItem(index, &date);
    
    return;
}

/*****************************************************************************/
//  Description : left keystoke callback
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RightButtonCallBack(void)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;
    GUI_BG_T                gray_bg = {0};

    gray_bg.bg_type = GUI_BG_IMG;
    if (s_day_index < M_MMI_MBBMS_MAX_DAY_NUM - 1)
    {
        //if left button grayed, resume it
        gray_bg.img_id  = IMAGE_COMMON_LBTN_GRAY;
        GUIBUTTON_SetGrayed(MMIMBBMS_BUTTON_LEFT_CTRL_ID, FALSE, &gray_bg, FALSE);
        s_day_index++;
        ButtonCallBack();   
    }
    
    if(s_day_index == M_MMI_MBBMS_MAX_DAY_NUM - 1)
    {
        //gray button
        gray_bg.img_id  = IMAGE_COMMON_RBTN_GRAY;
        GUIBUTTON_SetGrayed(MMIMBBMS_BUTTON_RIGHT_CTRL_ID, TRUE, &gray_bg, FALSE);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : MMIMBBMS_LoadListItemByIdx
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_LoadListItem(uint16 index, SG_DATETIME_T* pdate)
{
    uint16 content_count = 0;
    MMI_CTRL_ID_T       ctrl_id = MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID;
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    SG_ERROR_E err_code = SG_ERROR_NONE;
    SG_LIST_PTR_T pItem = PNULL;
    SG_LIST_PTR_T list_potr = PNULL;
    anyURI*  pURI_index = PNULL;
    anyURI    globalContentId = {0};
    anyURI    globalServiceId = {0};
    
    SG_LIST_PTR_T pservice_node = {0};
    //MBBMSSG_CONTENT_T*  pcontentItem = PNULL;
    MBBMSSG_CONTENT_INDEX_T *pcontentidex = PNULL;
    
    wchar temp_wchar[GUITAB_MAX_ITEM_NAME_LENGTH] = {0};
    wchar temp_wchar3[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    //wchar temp_wchar4[GUITAB_MAX_ITEM_NAME_LENGTH] = {0};
    
    //char str_price[10] = {0};
    //SG_LIST_PTR_T purchaseNode = PNULL;
    //SG_LIST_PTR_T purchaseHead = PNULL;
    //MBBMSSG_PURCHASE_DETAIL_T purDetailItem = {0};
    //SG_MONETARY_PRICE_T* priceInfo = PNULL;
    
    //MMI_STRING_T   temp_str = {0};
    BOOLEAN is_ordered = FALSE;
    BOOLEAN is_encrypted = FALSE;
    BOOLEAN is_service_collected = FALSE;
    //char    str_period[MMIMBBMS_MAX_PRICE_LENS] = {0};
    //wchar   wchar_period[MMIMBBMS_MAX_PRICE_LENS] = {0};

    uint16 content_index = 0;

    s_playing_content_index = 0;    
    
    
    MMIMTV_NewDataData();
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON;
    item_t.item_data_ptr = &item_data;    
    
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        // 根据index得到对应的service节点            
        pservice_node = MMIMBBMS_GetServiceNodeByIdx(index);
        
        if (PNULL == pservice_node)
        {
            return;
        }
        
        // 根据service接点得到所有content，并加入到listbox
        err_code = MBBMSSG_GetContentIndex(*((anyURI*)(pservice_node->data)), *pdate, &list_potr);
        
        if (err_code != SG_ERROR_NONE)
        {
            //SCI_TRACE_LOW:"MBBMSSG_GetContentIndex errcode = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4200_112_2_18_2_39_55_432,(uint8*)"d", err_code);
            return;
        }
        
        //是否是已收藏频道
        MBBMSSG_GetGlobalServiceId(*((anyURI*)(pservice_node->data)), &globalServiceId);
        if(MMIMBBMS_IsFavoriteService(&globalServiceId))
        {
            is_service_collected = TRUE;
        }
            
        if (PNULL == list_potr)
        {
            //SCI_TRACE_LOW:"MBBMSSG_GetContentIndex list_potr = PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4213_112_2_18_2_39_55_433,(uint8*)"");
            return;
        }		
                
        is_ordered = MMIMBBMS_IsServiceOrdered(*((anyURI*)(pservice_node->data)));
        is_encrypted = MMIMBBMS_IsServiceEncrypted(*((anyURI*)(pservice_node->data)));
        
        pItem = list_potr;
        
        if (list_potr->node_cnt > 0)
        {
            content_count = list_potr->node_cnt - 1;
        }
        
        GUILIST_SetMaxItem(ctrl_id, content_count, FALSE);
        
        while (pItem->next_ptr != PNULL)
        {
            pItem = pItem->next_ptr;
            pcontentidex = (MBBMSSG_CONTENT_INDEX_T*)(pItem->data);
            pURI_index = &(pcontentidex->index);
            
            if (pURI_index == PNULL)
            {
                //SCI_TRACE_LOW:"pURI_index is pnull"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4237_112_2_18_2_39_55_434,(uint8*)"");
                continue;
            }
            
            //image 1
            item_data.item_content[0].item_data_type        = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id    = IMG_PRO_LIST;   
            
            SCI_MEMSET(temp_wchar, 0, sizeof(temp_wchar));
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_wchar;
            MMIMBBMS_GetStartEndTimeStringFromContentIndex(pcontentidex, temp_wchar);
            item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wchar);
            
            SCI_MEMSET(&globalContentId,0,sizeof(anyURI));
            // get global content index
            if(PNULL != pURI_index)
            {
                err_code = MBBMSSG_GetGlobalContentId(*pURI_index, &globalContentId );
            }
            if (MMIMBBMS_IsAlarmContent(&globalContentId) == TRUE)
            {
                item_data.item_content[3].item_data_type        = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[3].item_data.image_id    = IMAGE_MBBMS_NOTIFY;
            }

            if(is_service_collected)
            {
                item_data.item_content[4].item_data_type        = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[4].item_data.image_id    = IMAGE_MBBMS_COLLECT;
            }
            
#if 0
            err_code = MBBMSSG_GetPurchaseItemByContent(*pURI_index, &purchaseHead);
            purchaseNode = purchaseHead;
            if (err_code == SG_ERROR_NONE)
            {
                pURI_index = (anyURI*)(purchaseNode->next_ptr->data);
                
                if (pURI_index != PNULL)
                {
                    SCI_MEMSET(&purDetailItem,0,sizeof(MBBMSSG_PURCHASE_DETAIL_T));
                    err_code = MBBMSSG_GetPurchaseDetail(*pURI_index, &purDetailItem);
                    if (err_code == SG_ERROR_NONE)
                    {
                        char* pChar = PNULL;
                        
                        item_data.item_content[4].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                        priceInfo = (SG_MONETARY_PRICE_T*)(purDetailItem.PriceInfo.MonetaryPriceList->next_ptr->data);
                        sprintf(str_price, "%f", priceInfo->price);
                        pChar = str_price + strlen(str_price) - 1;
                        while(*pChar == '0')
                        {
                            pChar--;
                        }
                        
                        *(pChar+1) = '\0';
                        
                        SCI_MEMSET(temp_wchar4, 0, sizeof(temp_wchar4));
                        
                        MMIAPICOM_StrToWstr((uint8*)str_price, temp_wchar4);
                        
                        if (purDetailItem.PriceInfo.subscriptionType == MBBMS_PURCHASE_DATA_SUBSCRIPTION_CONTENT)
                        {
                            MMI_GetLabelTextByLang(TXT_MBBMS_TIMES, &temp_str);
                        }
                        else
                        {
                            MMI_GetLabelTextByLang(TXT_MBBMS_MONTH, &temp_str);
                        }
                        
                        MMIAPICOM_Wstrcat(temp_wchar4, (wchar*)"/");
                        if(MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM_MULTI == purDetailItem.PriceInfo.subscriptionType)
                        {
                            //SCI_TRACE_LOW:"MMIMBBMS_LoadListItem purDetailItem.PriceInfo.period = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4311_112_2_18_2_39_55_435,(uint8*)"d", purDetailItem.PriceInfo.period);
                            memset(str_period, 0, sizeof(str_period));
                            memset(wchar_period, 0, sizeof(wchar_period));
                            if(0 != purDetailItem.PriceInfo.period)
                            {
                                sprintf(str_period, "%d", purDetailItem.PriceInfo.period);
                                MMIAPICOM_StrToWstr((uint8*)str_period, wchar_period);
                                MMIAPICOM_Wstrcat(temp_wchar4, wchar_period);
                            }
                        }
                        MMIAPICOM_Wstrcat(temp_wchar4, temp_str.wstr_ptr);
                        
                        item_data.item_content[4].item_data.text_buffer.wstr_ptr = temp_wchar4;
                        item_data.item_content[4].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wchar4);
                        
                    }
                    MBBMSSG_FreePurchaseDetail(&purDetailItem);
                }
            }
            
            MBBMSSG_FreePurchaseItemByContent(&purchaseHead);
#endif
            
            item_data.item_content[5].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            
            SCI_MEMSET(temp_wchar3, 0, sizeof(temp_wchar3));
            
            if (pcontentidex != PNULL)
            {
                MMIMBBMS_GetNameWchar(pcontentidex->NameList, temp_wchar3,
                    MMIMBBMS_SERVICE_NAME_MAX_LEN);
            }
            
            item_data.item_content[5].item_data.text_buffer.wstr_ptr = temp_wchar3;
            item_data.item_content[5].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wchar3);
            
            if(MMIMBBMS_GetPlayingContentIndex(pcontentidex))
            {
                s_playing_content_index = content_index;
            }
            content_index++; 
            
            GUILIST_AppendItem(ctrl_id, &item_t);
            
            MBBMSSG_FreeTypeAnyURI(&globalContentId);
            
            memset(&item_data, 0, sizeof(item_data));
        }
        
        MBBMSSG_FreeContentIndex(&list_potr);
    }
    else
    {
        MTVSERVICE_PROGRAM_LIST_T prog_list_t = {0};
        MTVSERVICE_PROGRAM_T      *prog_arry_ptr = PNULL;
        MMIMTV_SERVICE_T    cmmb_proglist = {0};
        MMIMTV_PROGRAMME_T* cmmb_prog_ptr = PNULL;
        MTV_DATE_T  date = {0};
        uint16  net_id = 0;
        uint16  service_id = 0;
        uint32  ret_code = 0;
        uint32  i = 0;
        
        MMIMBBMS_GetProgramList(&prog_list_t);
        prog_arry_ptr = prog_list_t.list_ptr;    
        
        //get content info from cmmb esg data
        net_id      = prog_arry_ptr[index].net_id;
        service_id  = prog_arry_ptr[index].service_id;
        
        date.year  = pdate->years;
        date.mon   = pdate->months;
        date.mday  = pdate->days;
        ret_code = CMMB_GetAllProg(net_id, service_id, date, &cmmb_proglist);
        //SCI_TRACE_LOW:"MMIMBBMS_LoadListItem date year:%d, month:%d day:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4385_112_2_18_2_39_55_436,(uint8*)"ddd", date.year, date.mon, date.mday);
        
        //SCI_TRACE_LOW:"MMIMBBMS_LoadListItem net_id=%d, service_id=%d CMMB_GetAllProg return=%d, NumOfProg=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4388_112_2_18_2_39_55_437,(uint8*)"dddd", net_id, service_id, ret_code, cmmb_proglist.NumOfProg);
        
        content_count = cmmb_proglist.NumOfProg;
        GUILIST_SetMaxItem(ctrl_id, content_count, FALSE);
        for(i = 0; i < content_count; i++)
        {
            cmmb_prog_ptr = cmmb_proglist.prog_ptr + i;
            
            //anim
            item_data.item_content[0].item_data_type        = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id    = IMG_PRO_LIST;   
            
            //start and end time
            SCI_MEMSET(temp_wchar, 0, sizeof(temp_wchar));
            MMIMBBMS_GetCmmbStartEndTimeString(cmmb_prog_ptr, temp_wchar);
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_wchar;
            item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wchar);
            
            //name
            SCI_MEMSET(temp_wchar3, 0, sizeof(temp_wchar3));
            MMIMBBMS_GetCmmbContentNameString(cmmb_prog_ptr, temp_wchar3, MMIMBBMS_SERVICE_NAME_MAX_LEN);
            item_data.item_content[5].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[5].item_data.text_buffer.wstr_ptr = temp_wchar3;
            item_data.item_content[5].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp_wchar3);
            
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
        
        CMMB_FreeAllProg(&cmmb_proglist);
    }
    
    return;
}

/*****************************************************************************/
//  Description : MMIMBBMS_GetServiceNodeByIdx
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SG_LIST_PTR_T MMIMBBMS_GetServiceNodeByIdx(uint16 index)
{
    uint16  node_idx = 0;
    SG_ERROR_E      errcode = SG_ERROR_NONE;
    SG_LIST_PTR_T pItem = PNULL;
    SG_LIST_PTR_T listptr = PNULL;
    
    errcode = MMIMBBMS_GetServiceIndex(&listptr);
    
    if (errcode != SG_ERROR_NONE)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetServiceIndex errcode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4438_112_2_18_2_39_55_438,(uint8*)"d", errcode);
        return PNULL;
    }
    
    node_idx = index;
    pItem = listptr;
    
    if (pItem->next_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetServiceNodeByIdx: only have head node"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4447_112_2_18_2_39_55_439,(uint8*)"");
        //MBBMSSG_FreeServiceIndex(&listptr);
        return PNULL;
    }
    
    pItem = pItem->next_ptr;
    
    while ((pItem != PNULL) && (node_idx != 0))
    {
        pItem = pItem->next_ptr;
        node_idx--;
    }
    
    return pItem;
}

/*****************************************************************************/
//  Description : MMIMBBMS_GetContentNodeByIdx
//  Global resource dependence : none
//  Author:
//  Note: 返回节点类型 MBBMSSG_CONTENT_INDEX_T
/*****************************************************************************/
LOCAL SG_LIST_PTR_T MMIMBBMS_GetContentNodeByIdx(uint16 service_idx, uint16 content_idx, SG_LIST_PTR_T *contentIdList_head_ptr)
{
    uint16  node_idx = 0;
    SG_ERROR_E      errcode = SG_ERROR_NONE;
    SG_LIST_PTR_T pItem = PNULL;
    SG_LIST_PTR_T pservice_node = PNULL;
    SG_DATETIME_T    date = {0};
    SCI_DATE_T    sysdate = {0};
    
    // 根据index得到对应的service节点            
    pservice_node = MMIMBBMS_GetServiceNodeByIdx(service_idx);
    
    if (PNULL == pservice_node)
    {
        return PNULL;
    }
    
    MMIMBBMS_GetSelectedDate(&sysdate, s_day_index);
    date.years = sysdate.year;
    date.months = sysdate.mon;
    date.days = sysdate.mday;
    
    // 根据service接点得到所有content
    errcode = MBBMSSG_GetContentIndex(*((anyURI*)(pservice_node->data)), date, contentIdList_head_ptr);
    
    if (errcode != SG_ERROR_NONE)
    {
        //SCI_TRACE_LOW:"MBBMSSG_GetContentIndex errcode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4494_112_2_18_2_39_55_440,(uint8*)"d", errcode);
        return PNULL;
    }
    
    node_idx = content_idx;
    pItem = *contentIdList_head_ptr;
    
    if (pItem->next_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetServiceNodeByIdx: only have head node"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4503_112_2_18_2_39_55_441,(uint8*)"");
        MBBMSSG_FreeContentIndex(contentIdList_head_ptr);
        return PNULL;
    }
    
    pItem = pItem->next_ptr;
    
    while ((pItem != PNULL) && (node_idx != 0))
    {
        pItem = pItem->next_ptr;
        node_idx--;
    }
    
    return (SG_LIST_PTR_T)pItem;
}

/*****************************************************************************/
//  Description : handle program list option menu win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProgramListOptMenuWinMsg(
                                                   MMI_WIN_ID_T        win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM                param
                                                   )
{
    uint16 i = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_PROGRAM_LIST_OPT_MENU_CTRL_ID;
    LOCAL anyURI  globalServiceId = {0};
    uint16        cur_index = 0;
    SG_LIST_PTR_T pservice_node = PNULL;
    SG_ERROR_E    error_code = SG_ERROR_NONE;
    uint16        node_id = 0;
    //BOOLEAN  bResult = FALSE;
    anyURI* pGlobalContentId = PNULL;
    
    pGlobalContentId = (anyURI*)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //creat dynamic menu
        GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
        
        if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
        {
            if (pGlobalContentId != PNULL)
            {
                // 1. 观看
                InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
                
                // 2. 设置提醒/取消提醒
                if(MMIMBBMS_IsAlarmContent(pGlobalContentId) == FALSE)
                {
                    InsertMenuNode(ctrl_id, TXT_MBBMS_SET_CONTENT_ALARM, MMIMBBMS_ID_SET_CONTENT_ALARM_MENU, 0, i++);
                }
                else
                {
                    InsertMenuNode(ctrl_id, TXT_MBBMS_CLOSE_CONTENT_ALARM, MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU, 0, i++);
                }
                
                // 3. 收藏此频道/取消收藏
                // 根据index得到对应的service节点
                cur_index = GUITAB_GetCurSel(MMIMBBMS_PROGRAM_TAB_CTRL_ID);
                pservice_node = MMIMBBMS_GetServiceNodeByIdx(cur_index);
                
                if (PNULL != pservice_node)
                {
                    //判断是否已加入收藏频道
                    SCI_MEMSET(&globalServiceId,0,sizeof(anyURI));
                    error_code = MBBMSSG_GetGlobalServiceId(*((anyURI*)(pservice_node->data)), &globalServiceId );
                    
                    if (error_code == SG_ERROR_NONE && MMIMBBMS_IsFavoriteService(&globalServiceId))
                    {
                        InsertMenuNode(ctrl_id, TXT_MBBMS_DEL_FAVORITE_SERVICE, MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU, 0, i++);
                    }
                    else
                    {
                        InsertMenuNode(ctrl_id, TXT_ADD_FAVORITE_SERVICE, MMIMBBMS_ID_ADD_FAVORITE_SERVICE_MENU, 0, i++);
                    }
                    
                    //MBBMSSG_FreeTypeAnyURI(&globalServiceId);
                }
                else
                {
                    InsertMenuNode(ctrl_id, TXT_MBBMS_DEL_FAVORITE_SERVICE, MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU, 0, i++);
                    GUIMENU_SetNodeGrayed(TRUE, 3, 0, ctrl_id);
                }
                
                // 4. 节目详情
                InsertMenuNode(ctrl_id, TXT_COMMON_PROGRAM_DETAIL, MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID, 0, i++);
                // 5. 套餐管理
                InsertMenuNode(ctrl_id, TXT_MBBMS_PURCHASE_MANAGE, MMIMBBMS_MENU_PURCHASE_MANAGE_ITEM_ID, 0, i++);				
                
            }
            else
            {
                InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
                InsertMenuNode(ctrl_id, TXT_MBBMS_SET_CONTENT_ALARM, MMIMBBMS_ID_SET_CONTENT_ALARM_MENU, 0, i++);
                InsertMenuNode(ctrl_id, TXT_MBBMS_DEL_FAVORITE_SERVICE, MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU, 0, i++);
                InsertMenuNode(ctrl_id, TXT_COMMON_PROGRAM_DETAIL, MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID, 0, i++);
                InsertMenuNode(ctrl_id, TXT_MBBMS_PURCHASE_MANAGE, MMIMBBMS_MENU_PURCHASE_MANAGE_ITEM_ID, 0, i++);
				
                GUIMENU_SetNodeGrayed(TRUE, 0, 0, ctrl_id);
                GUIMENU_SetNodeGrayed(TRUE, 1, 0, ctrl_id);
                GUIMENU_SetNodeGrayed(TRUE, 2, 0, ctrl_id);
                GUIMENU_SetNodeGrayed(TRUE, 3, 0, ctrl_id);
                GUIMENU_SetNodeGrayed(TRUE, 4, 0, ctrl_id);				
            }
            
            // 5. 搜索节目
            InsertMenuNode(ctrl_id, TXT_MBBMS_SEARCH_PROGRAM,       MMIMBBMS_MENU_SEARCH_PROGRAM_ITEM_ID, 0, i++);
        }
        else
        {
            //cmmb mode
            InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
            InsertMenuNode(ctrl_id, TXT_MBBMS_SET_CONTENT_ALARM, MMIMBBMS_ID_SET_CONTENT_ALARM_MENU, 0, i++);
            InsertMenuNode(ctrl_id, TXT_MBBMS_DEL_FAVORITE_SERVICE, MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU, 0, i++);
            InsertMenuNode(ctrl_id, TXT_COMMON_PROGRAM_DETAIL, MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID, 0, i++);
            InsertMenuNode(ctrl_id, TXT_MBBMS_SEARCH_PROGRAM,       MMIMBBMS_MENU_SEARCH_PROGRAM_ITEM_ID, 0, i++);
            InsertMenuNode(ctrl_id, TXT_MBBMS_PURCHASE_MANAGE, MMIMBBMS_MENU_PURCHASE_MANAGE_ITEM_ID, 0, i++);
            
            GUIMENU_SetNodeGrayed(TRUE, 1, 0, ctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, 2, 0, ctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, 3, 0, ctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, 4, 0, ctrl_id);
            GUIMENU_SetNodeGrayed(TRUE, 5, 0, ctrl_id);				
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:      
    case MSG_CTL_MIDSK:
        {
            //MMIMBBMS_SAVE_CONTENT_INFO_T content_info = {0};
            MMIMBBMS_SAVE_SERVICE_INFO_T service_info = {0};
            anyURI contentID = {0};
            //MBBMSSG_CONTENT_T contentItem = {0};
            
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
            case MMIMBBMS_ID_WATCH_MENU:
                {
                    /*add for init not finished*/
                    if(!MMIMBBMS_DisplayInitWaitingWin())
                    {
                        break;
                    }    
                    
                    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
                    {
                        if(!MMIMBBMS_GetSubscriptionUpdateStatus())
                        {
                            MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                            s_object_type=MMIMBBMS_OBJECT_FROM_PROGRAM_LIST_OPT;
                            MMIMBBMS_OpenSubscriptionUpdateQueryWin();
                        }
                        else
                        {
                            MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_PROGRAM_LIST_OPT);
                        }
                    }
                    else
                    {
                        //cmmb mode
                        MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_WEB, PNULL, NULL);
                    }
                    break;
                }
                
            case MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID:
                if (MMK_IsOpenWin(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID);
                }
                
                MMK_CreateWin((uint32 *)MMIMBBMS_PROGRAM_DETAIL_WIN_TAB, (ADD_DATA)pGlobalContentId); 
                
                //s_program_list_open_it = TRUE;
                
                break;
            case MMIMBBMS_ID_ADD_FAVORITE_SERVICE_MENU:
                {
                    service_info.globalServiceID = globalServiceId;
                    MMIMBBMS_AddFavoriteServiceNode(&service_info);
                    
                    MMIMBBMS_ShowFavoriteMsg(&globalServiceId, 
                        MMIMBBMS_TYPE_SERVICE, MMIMBBMS_OPER_ADD);
                    
                    MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    
                    break;
                }
            case MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU:
                {
                    MMIMBBMS_DelFavoriteServiceNodeByServiceID(&globalServiceId);
                    
                    MMIMBBMS_ShowFavoriteMsg(&globalServiceId, 
                        MMIMBBMS_TYPE_SERVICE, MMIMBBMS_OPER_DEL);
                    
                    MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    
                    break;
                }
                
            case MMIMBBMS_ID_SET_CONTENT_ALARM_MENU:
                {
                    MBBMSSG_CONTENT_T content_t = {0};
                    MMIMBBMS_ALARM_CONTENT_INFO_T AlarmContentInfo = {0};
                    
                    MBBMSSG_GetContentId(*pGlobalContentId, &contentID);
                    MBBMSSG_GetContent(contentID, &content_t);
                    
                    AlarmContentInfo.globalContentID = *pGlobalContentId;
                    AlarmContentInfo.starttime = content_t.StartTime;
                    MBBMSSG_FreeContent(&content_t);
                    
                    MMIMBBMS_RegisterContentAlarm(&AlarmContentInfo);
                    
                    MMIMBBMS_AddAlarmContentNode(&AlarmContentInfo);
                    
                    MBBMSSG_FreeTypeAnyURI(&contentID);
                    
                    MMK_CloseWin(win_id);
                    
                    MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    
                    break;
                }
            case MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU:
                {
                    MMIMBBMS_ALARM_CONTENT_INFO_T* pAlarmContentInfo = PNULL;
                    
                    pAlarmContentInfo = MMIMBBMS_GetAlarmContentNodeByContentID(pGlobalContentId);
                    if(PNULL != pAlarmContentInfo)
                    {
                        /* 取消节目提醒闹钟 */
                        MMIALMSVC_UnRegService(pAlarmContentInfo->event_id, TRUE);
                        
                        MMIMBBMS_DelAlarmContentNode(pAlarmContentInfo);
						
                         MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MBBMS_CONTENT_UNREGISTER_NOTIFY_INFO,TXT_NULL,IMAGE_PUBWIN_SUCCESS, 
                            PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,MMIMBBMS_HandleAddorCancelAlarmServiceAlertWinMsg);                       
                        
                        MMK_CloseWin(win_id);
                        
                        MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                        MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                        MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    }
                    break;
                }
                
            case MMIMBBMS_MENU_SEARCH_PROGRAM_ITEM_ID:
                MMIMBBMS_OpenSearchMainWin();
                break;
			case MMIMBBMS_MENU_PURCHASE_MANAGE_ITEM_ID:
                MMIMBBMS_OpenAccountInquiryRequestWin();				
				break;
            default:
                break;
        }
        
        MMK_CloseWin(win_id);
        break;
    }
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MBBMSSG_FreeTypeAnyURI(&globalServiceId);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//     Description : handle message of program list child window
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleProgramDetailWinMsg(
                                                      MMI_WIN_ID_T    win_id, 
                                                      MMI_MESSAGE_ID_E    msg_id, 
                                                      DPARAM                param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    SG_ERROR_E   err_code = SG_ERROR_NONE;
    //MMI_STRING_T    temp_str = {0};
    MMI_STRING_T    time_str = {0};
    MMI_STRING_T    name_str = {0};
    MMI_STRING_T    time_name_str = {0};
    MMI_STRING_T    detail_str = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info =    {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIFORM_CHILD_WIDTH_T   width = {0};
    anyURI* pURI_index = PNULL;
    MBBMSSG_CONTENT_T contentItem = {0};
    LOCAL uint8   data_size = 0;
    LOCAL uint8* data_ptr = PNULL;   
    MMI_CONTROL_CREATE_T   anim_ctrl = {0};
    GUIANIM_INIT_DATA_T    anim_init = {0};
    GUIANIM_FILE_INFO_T    file_info_ptr = {0}; 
    void*                  anim_ctrl_ptr = PNULL;
    GUI_LCD_DEV_INFO       lcd_dev={GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};    
    GUI_RECT_T          display_rect={10, 60, 100, 150};
    
   	//SCI_TRACE_LOW:"MMIMBBMS_HandleProgramDetailWinMsg() enter:msg_id=%d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4829_112_2_18_2_39_56_442,(uint8*)"d",msg_id);
    pURI_index = (anyURI*)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:	
        {
            MBBMSSG_PREVIEWDATA_SEARCH_T preview_search = {0};
            MBBMSSG_PREVIEW_DATA_T             preview_data = {0};
            SCI_DATE_T    sysdate = {0};
            char strDate[MMIMBBMS_MAX_DATE_STR_LENS] = {0};
            wchar wchar_Date[MMIMBBMS_MAX_DATE_STR_LENS] = {0};
            wchar wchar_temp[30] = {0};
            MMI_STRING_T   str_temp = {0};
            MMI_STRING_T   str_day3 = {0};
            MMI_STRING_T   str_merge = {0};
            wchar wchar_merge[50] = {0};
            MMI_STRING_T  str_detail = {0};
            MMI_STRING_T   str_date = {0};

#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

            width.type = GUIFORM_CHILD_WIDTH_FIXED;
            width.add_data = MMIMBBMS_DETAIL_IMAGE_WIDTH;
            GUIFORM_SetChildWidth(MMIMBBMS_PROGRAM_DETAIL_FORM1_CTRL_ID,MMIMBBMS_ANIM_DETAIL_CTRL_ID,&width);
            if (pURI_index == PNULL)
            {
                break;
            }
            // 是否还需要调用 MBBMSSG_GetContent
            err_code = MBBMSSG_GetContent(*pURI_index, &contentItem);
            if (err_code != SG_ERROR_NONE)
            {
                //SCI_TRACE_LOW:"MBBMSSG_GetContent errcode = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4864_112_2_18_2_39_56_443,(uint8*)"d", err_code);
                break;
            }
            time_str.wstr_ptr = SCI_ALLOC_APP((MMIMBBMS_SERVICE_NAME_MAX_LEN+1)*sizeof(wchar));
            SCI_MEMSET(time_str.wstr_ptr,0,(MMIMBBMS_SERVICE_NAME_MAX_LEN+1)*sizeof(wchar));
            MMIMBBMS_GetStartEndTimeString(&contentItem, time_str.wstr_ptr);
			time_str.wstr_len=MMIAPICOM_Wstrlen(time_str.wstr_ptr);	
			
            name_str.wstr_ptr = SCI_ALLOC_APP((MMIMBBMS_SERVICE_NAME_MAX_LEN+1)*sizeof(wchar));
            SCI_MEMSET(name_str.wstr_ptr,0,(MMIMBBMS_SERVICE_NAME_MAX_LEN+1)*sizeof(wchar));
            MMIMBBMS_GetNameWchar(contentItem.NameList, name_str.wstr_ptr,MMIMBBMS_SERVICE_NAME_MAX_LEN);
			name_str.wstr_len=MMIAPICOM_Wstrlen(name_str.wstr_ptr);			

            time_name_str.wstr_ptr = SCI_ALLOC_APP((MMIMBBMS_SERVICE_NAME_MAX_LEN*2+1)*sizeof(wchar));
            SCI_MEMSET(time_name_str.wstr_ptr,0,(MMIMBBMS_SERVICE_NAME_MAX_LEN*2+1)*sizeof(wchar));
            MMIAPICOM_MergeTwoString(&time_str, &name_str, &time_name_str, MMIMBBMS_SERVICE_NAME_MAX_LEN*2);          
            GUITEXT_SetString(MMIMBBMS_TEXT_TIME_CTRL_ID, 
                time_name_str.wstr_ptr, time_name_str.wstr_len, FALSE);
            time_name_str.wstr_len = MMIAPICOM_Wstrlen(time_name_str.wstr_ptr);
            
            detail_str.wstr_ptr = SCI_ALLOC_APP((MMIMBBMS_SERVICE_DETAIL_MAX_LEN+1)*sizeof(wchar));
            SCI_MEMSET(detail_str.wstr_ptr,0,(MMIMBBMS_SERVICE_DETAIL_MAX_LEN+1)*sizeof(wchar));
            MMIMBBMS_GetNameWchar(contentItem.DescriptionList, detail_str.wstr_ptr,
                MMIMBBMS_SERVICE_DETAIL_MAX_LEN);
            detail_str.wstr_len = MMIAPICOM_Wstrlen(detail_str.wstr_ptr);
            GUITEXT_SetString(MMIMBBMS_TEXT_DETAIL_CTRL_ID, 
                detail_str.wstr_ptr, detail_str.wstr_len, FALSE);

            SCI_FREE(time_str.wstr_ptr);
            SCI_FREE(name_str.wstr_ptr);
            SCI_FREE(time_name_str.wstr_ptr);
            SCI_FREE(detail_str.wstr_ptr);
         
            preview_search.index = *pURI_index;
            preview_search.range = MBBMS_PREVIEWDATA_RANGE_CONTENT;
            SCI_MEMSET(&preview_data,0,sizeof(preview_data));
            err_code = MBBMSSG_GetPreviewdata(preview_search,&preview_data);
            
            if (err_code != SG_ERROR_NONE)
            {
                //SCI_TRACE_LOW:"MBBMSSG_GetPreviewdata errcode = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4904_112_2_18_2_39_56_444,(uint8*)"d", err_code);
                MBBMSSG_FreeContent(&contentItem);
                break;
            }
            
            if (PNULL != preview_data.picture_ptr)
            {
                data_size = preview_data.picture_ptr->PictureData.data_len;
                if (data_size > SG_MAX_BASE64BIN_LEN)
                {
                    data_ptr = preview_data.picture_ptr->PictureData.data_ptr;
                }
                else
                {
                    data_ptr = preview_data.picture_ptr->PictureData.data_arr;
                }
                if(PNULL != preview_data.picture_ptr->file_name_ptr && 0 != preview_data.picture_ptr->file_name_len)
                {
                    uint16 i=0;
                    for(i=0; i<preview_data.picture_ptr->file_name_len; i++ )
                    {
                        //SCI_TRACE_LOW:"MMIMBBMS_HandleProgramDetailWinMsg filename[%d] = 0x%x, len = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_4925_112_2_18_2_39_56_445,(uint8*)"ddd", i, preview_data.picture_ptr->file_name_ptr[i], preview_data.picture_ptr->file_name_len);
                    }
                    //creat anim
                    anim_init.both_rect.h_rect = display_rect;
                    anim_ctrl.ctrl_id           = MMIMBBMS_PROGRAM_DETAIL_PREVIEW_DATA_CTRL_ID;
                    anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
                    anim_ctrl.init_data_ptr     = &anim_init;
                    anim_ctrl.parent_win_handle = win_id;

                    anim_ctrl_ptr = MMK_CreateControl(&anim_ctrl);
                    
                    control_info.is_ctrl_id = TRUE;
                    control_info.ctrl_id = MMIMBBMS_PROGRAM_DETAIL_PREVIEW_DATA_CTRL_ID; 
                    control_info.ctrl_ptr = anim_ctrl_ptr;
                    
                    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
                    //display_info.is_bg_img = TRUE;
                    display_info.is_disp_one_frame = TRUE;
                    //display_info.is_need_resize = TRUE;
                    display_info.is_update = TRUE;
                    display_info.is_play_once = TRUE;
                    display_info.is_syn_decode = TRUE;

                    file_info_ptr.full_path_wstr_ptr = (wchar*)preview_data.picture_ptr->file_name_ptr;//preview_data_path;
                    file_info_ptr.full_path_wstr_len =   preview_data.picture_ptr->file_name_len/2;
                    GUIANIM_SetDefaultIconByPtr(anim_ctrl_ptr,NULL,NULL);
                    GUIANIM_SetParam(&control_info,PNULL,&file_info_ptr,&display_info);    

                }
                else if (data_ptr != PNULL && data_size > 0)
                {
                    GUIRES_DisplayImgByPtr(data_ptr, data_size, PNULL, &display_rect, PNULL, &lcd_dev);
                }                
                
            }
            else
            {
                data_info.img_id = IMAGE_MBBMS_TV_LOGO;
                control_info.is_ctrl_id = TRUE;
                control_info.ctrl_id = MMIMBBMS_ANIM_DETAIL_CTRL_ID;
                control_info.ctrl_ptr = PNULL;
                display_info.is_syn_decode = TRUE;
                display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
                
                GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);                
            }

            MBBMSSG_FreePreviewData(&preview_data);
            MBBMSSG_FreeContent(&contentItem);

            /* added by jhpeng for cr157440 begin */
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_RETURN, TRUE);
 
            /* added by jhpeng for cr157440 end */
            
            MMI_GetLabelTextByLang(TXT_COMMON_PROGRAM_DETAIL, &str_detail);
            
            MMIMBBMS_GetSelectedDate(&sysdate, s_day_index);
            sprintf(strDate, " %d-%d ", sysdate.mon, sysdate.mday);
            MMIAPICOM_StrToWstr((uint8*)strDate, wchar_Date);
            str_date.wstr_ptr = wchar_Date;
            str_date.wstr_len = MMIAPICOM_Wstrlen(wchar_Date);
            
            str_temp.wstr_ptr = wchar_temp;
            MMIAPICOM_MergeTwoString(&str_detail, &str_date, &str_temp, 30);
            str_temp.wstr_len = MMIAPICOM_Wstrlen(wchar_temp);
            
            MMI_GetLabelTextByLang(s_day_textid[s_day_index], &str_day3);
            str_merge.wstr_ptr = wchar_merge;
            MMIAPICOM_MergeTwoString(&str_temp, &str_day3, &str_merge, 50);
            str_merge.wstr_len = MMIAPICOM_Wstrlen(wchar_merge);
            
            GUIWIN_SetTitleText(win_id, wchar_merge, str_merge.wstr_len, FALSE);	
            break;
        }
  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            {
                /*add for init not finished*/
                if(!MMIMBBMS_DisplayInitWaitingWin())
                {
                    break;
                }    
                
                if(!MMIMBBMS_GetSubscriptionUpdateStatus())
                {
                    MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                    s_object_type=MMIMBBMS_OBJECT_FROM_PROGROM_DETAIL;
                    MMIMBBMS_OpenSubscriptionUpdateQueryWin();
                }
                else
                { 
                    MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_PROGROM_DETAIL);
                }
                break; 
            }
           
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_MENU:
            {
                if (MMK_IsOpenWin(MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_ID);
                }
                
                MMK_CreateWin((uint32 *)MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_TAB, 
                    (ADD_DATA)pURI_index); 
                break;
            }
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
//  Description : HandleProgramDetailOptMenuWinMsg
//  Global resource dependence : 
//  Author:liming.deng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProgramDetailOptMenuWinMsg(
                                                     MMI_WIN_ID_T        win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM              param)
{
    uint16  i = 0;
    uint16  node_id = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    anyURI* pGlobalContentId = PNULL;
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_CTRL_ID;
    
    pGlobalContentId = (anyURI*)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //creat dynamic menu
        GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
        
        // 1. 观看
        InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
        
        // 2. 设置提醒/取消提醒
        if(!MMIMBBMS_IsAlarmContent(pGlobalContentId))
        {
            InsertMenuNode(ctrl_id, TXT_MBBMS_SET_CONTENT_ALARM, MMIMBBMS_ID_SET_CONTENT_ALARM_MENU, 0, i++);
        }
        else
        {
            InsertMenuNode(ctrl_id, TXT_MBBMS_CLOSE_CONTENT_ALARM, MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU, 0, i++);
        }
        
        MMK_SetAtvCtrl(win_id, MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            anyURI contentID = {0};

            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
            case MMIMBBMS_ID_WATCH_MENU:
                /*add for init not finished*/
                if(!MMIMBBMS_DisplayInitWaitingWin())
                {
                    break;
                }    
                
                if(!MMIMBBMS_GetSubscriptionUpdateStatus())
                {
                    MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                    s_object_type=MMIMBBMS_OBJECT_FROM_PROGROM_DETAIL;
                    MMIMBBMS_OpenSubscriptionUpdateQueryWin();
                }
                else
                {
                    MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_PROGROM_DETAIL);
                }
                break;

            case MMIMBBMS_ID_SET_CONTENT_ALARM_MENU:
                {
                    MBBMSSG_CONTENT_T content_t = {0};
                    MMIMBBMS_ALARM_CONTENT_INFO_T AlarmContentInfo = {0};
                    
                    MBBMSSG_GetContentId(*pGlobalContentId, &contentID);
                    MBBMSSG_GetContent(contentID, &content_t);
                    
                    AlarmContentInfo.globalContentID = *pGlobalContentId;
                    AlarmContentInfo.starttime = content_t.StartTime;
                    MBBMSSG_FreeContent(&content_t);
                    
                    MMIMBBMS_RegisterContentAlarm(&AlarmContentInfo);
                    
                    MMIMBBMS_AddAlarmContentNode(&AlarmContentInfo);
                    
                    MBBMSSG_FreeTypeAnyURI(&contentID);
                    
                    MMK_CloseWin(win_id);
                    
                    MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    MMK_PostMsg(MMIMBBMS_SEARCH_RESULT_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    break;
                }
                
            case MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU:
                {
                    MMIMBBMS_ALARM_CONTENT_INFO_T* pAlarmContentInfo = PNULL;
                    
                    pAlarmContentInfo = MMIMBBMS_GetAlarmContentNodeByContentID(pGlobalContentId);
                    if(PNULL != pAlarmContentInfo)
                    {
                        /* 取消节目提醒闹钟 */
                        MMIALMSVC_UnRegService(pAlarmContentInfo->event_id, TRUE);
                        
                        MMIMBBMS_DelAlarmContentNode(pAlarmContentInfo);
                        
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MBBMS_CONTENT_UNREGISTER_NOTIFY_INFO,TXT_NULL,IMAGE_PUBWIN_SUCCESS, 
                            PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,MMIMBBMS_HandleAddorCancelAlarmServiceAlertWinMsg);
                        
                        MMK_CloseWin(win_id);
                        
                        MMK_PostMsg(MMIMBBMS_PROGRAM_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                        MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                        MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                        MMK_PostMsg(MMIMBBMS_SEARCH_RESULT_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    }
                    break;
                }
                
            default:
                break;
            }
            
            MMK_CloseWin(win_id);
            break;
        }    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
/*****************************************************************************/
//     Description : set softkey of alert win for save favorite service win
//    Global resource dependence : none
//    Author:  xiaoxiang.huang
//    Note:
/*****************************************************************************/

LOCAL MMI_RESULT_E MMIMBBMS_HandleAddorCancelFavoriteServiceAlertWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_RETURN, FALSE);
        break;
     case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);       
        break;		
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
}
/*****************************************************************************/
//     Description : set softkey of alert win for save alarm service win
//    Global resource dependence : none
//    Author:  xiaoxiang.huang
//    Note:
/*****************************************************************************/

PUBLIC MMI_RESULT_E MMIMBBMS_HandleAddorCancelAlarmServiceAlertWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_RETURN, FALSE);
        break;
     case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);       
        break;		
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
}
/*****************************************************************************/
//  Description : MMIMBBMS_ShowFavoriteMsg
//  Global resource dependence : 
//  Author:liming.deng
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_ShowFavoriteMsg(anyURI* pGlobalId, MMIMBBMS_TYPE favoriteType, 
                                    MMIMBBMS_OPER operationType)
{
    wchar wchar_buf[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    wchar wchar_dstbuf[100] = {0};
    MMI_STRING_T strInfo = {0};
    MMI_STRING_T prompt_str = {0};
    anyURI serviceID = {0};
    uint16 content_len = 0;
    MBBMSSG_SERVICE_T service_t = {0};
    anyURI contentID = {0};
    MBBMSSG_CONTENT_T contentItem = {0};
    
    if (PNULL == pGlobalId)
    {
        return;
    }
    
    SCI_MEMSET(wchar_buf, 0, sizeof (wchar_buf));
    
    if (favoriteType == MMIMBBMS_TYPE_CONTENT)
    {
        MBBMSSG_GetContentId(*pGlobalId, &contentID);
        MBBMSSG_GetContent(contentID, &contentItem);
        content_len = MMIMBBMS_GetNameWchar(contentItem.NameList, wchar_buf, 
            MMIMBBMS_SERVICE_NAME_MAX_LEN);
        MBBMSSG_FreeContent(&contentItem);
        MBBMSSG_FreeTypeAnyURI(&contentID);
    }
    else
    {
        MBBMSSG_GetServiceId(*pGlobalId, &serviceID);
        MBBMSSG_GetService(serviceID, &service_t);
        content_len = MMIMBBMS_GetNameWchar(service_t.NameList, wchar_buf,
            MMIMBBMS_SERVICE_NAME_MAX_LEN);
        MBBMSSG_FreeService(&service_t);
        MBBMSSG_FreeTypeAnyURI(&serviceID);
    }
    
    if (operationType == MMIMBBMS_OPER_ADD)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_FAVORITE1, &prompt_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_CANCEL_FAVORITE1, &prompt_str);
    }
    
    MMIAPICOM_Wstrncpy(wchar_dstbuf, prompt_str.wstr_ptr, prompt_str.wstr_len);
    MMIAPICOM_Wstrcat(wchar_dstbuf, wchar_buf);
    
    strInfo.wstr_ptr = wchar_dstbuf;
    strInfo.wstr_len = prompt_str.wstr_len + content_len;
    
    if (operationType == MMIMBBMS_OPER_ADD)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_FAVORITE2, &prompt_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_CANCEL_FAVORITE2, &prompt_str);
    }
    
    MMIAPICOM_Wstrncpy(wchar_dstbuf+strInfo.wstr_len, prompt_str.wstr_ptr, prompt_str.wstr_len);
    //MMIAPICOM_Wstrcat(wchar_dstbuf, wchar_buf);
    
    //strInfo.wstr_ptr = wchar_dstbuf;
    strInfo.wstr_len += prompt_str.wstr_len;
    
    MMIPUB_OpenAlertTextWinByTextPtr(PNULL,
        &strInfo,
        PNULL,
        PNULL,
        MMIPUB_SOFTKEY_CUSTOMER,
        MMIMBBMS_HandleAddorCancelFavoriteServiceAlertWinMsg); 
    
    return;
}

/*****************************************************************************/
//  Description : HandleMainInterfaceTabWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面TAB父窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainInterfaceTabWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param)
{
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, ctrl_id);
			if(MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode())
                GUITAB_SetTitleIcon(ctrl_id, IMAGE_MTV_LOGO);
            
            MainWinAppendTabInfo();
            
            //进入主菜单界面说明MBBMS功能可用，更新标志
            // GUIWIN_SetTitleTextId(win_id, TXT_MBBMS_IS_AUTHORIZED, FALSE);
            GUITAB_SetTitleTextId(ctrl_id, TXT_MBBMS_IS_AUTHORIZED);
            
            MAIN_SetTVSignalState();
            
            GUITAB_SetFontParam(ctrl_id, MMI_DEFAULT_NORMAL_FONT,
                MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0), 0);
            
            break;
        }
        
    case MSG_TIMER:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        {  
            
            MMIMBBMS_CancelAllAlarm();
            //SCI_TRACE_LOW:"HandleMainInterfaceTabWinMsg MSG_CLOSE_WINDOW\n "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_5374_112_2_18_2_39_57_446,(uint8*)"");
            
            /* jhpeng modified begin for CR155220 */
            MMIMBBMS_WriteFavoriteServiceFile();
            MMIMBBMS_WriteAlarmContentFile();
            /* jhpeng modified end for CR155220 */

            MMIMBBMS_FreeProgramList();
            MMIMBBMS_FreeServiceIndex();
            MMIMBBMS_FreeAccountInquiryRes();
            
            MMIMBBMS_ExitMBBMS();         
            break;
        }
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}           

/*****************************************************************************/
//  Description : MMIMBBMS_RefreshMainWinMiddleSoftkey
//  Global resource dependence : none
//  Author: plum.peng
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_RefreshMainWinMiddleSoftkey(MMI_WIN_ID_T win_id)
{
    //if current service is not encrypted,then show watch text              
    if(s_main_service_count > 0)
    {
        if(MMIMBBMS_GetInitStatus())
        {
            //初始化完毕后才显示“观看”
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
        }
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
    }
    
    return;
}

/*****************************************************************************/
//  Description : MMIMBBMS_RefreshProgListMiddleSoftkey
//  Global resource dependence : none
//  Author: plum.peng
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_RefreshProgListMiddleSoftkey(MMI_WIN_ID_T win_id)
{
    if(GUILIST_GetTotalItemNum(MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID) == 0)
    {
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_RETURN, FALSE);
    }
    
    return;
}

/*****************************************************************************/
//  Description : HandleAllServiceChildWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面所有频道子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllServiceChildWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param)
{
    uint16 index = 0;
    
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //GUILIST_SetRect(ctrl_id,&list_rect);
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
            
#ifdef MBBMS_TEST
            MBBMS_TestParse();
            HandleSGDUParse();
#endif
            
            MMIMTV_NewDataData();
            
            MMIMBBMS_ReadFavoriteServiceFile();
            MMIMBBMS_ReadAlarmContentFile();
            
            CreateAllServiceList();
            
            s_main_service_count = GUILIST_GetTotalItemNum(ctrl_id);
            
            if (s_main_service_index < s_main_service_count)
            {
                GUILIST_SetCurItemIndex(ctrl_id, s_main_service_index);
            }
            
            MMIMBBMS_RefreshMainWinMiddleSoftkey(win_id);
            
            if(MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode())
            {
                MMIMBBMS_OpenCMMBModeWarningWin();
            }
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(s_main_service_count > 0)
        {
            /*add for init not finished*/
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }  
            
            index = GUILIST_GetCurItemIndex(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID);
            
            //播放前确认订购关系表
            if(MMIMBBMS_IsCurrServiceInAllTabEncrypted(index)
                && !MMIMBBMS_GetSubscriptionUpdateStatus())
            {
                MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                s_object_type=MMIMBBMS_OBJECT_FROM_ALL_SERVICE;
                
                MMIMBBMS_OpenSubscriptionUpdateQueryWin();
            }
            else
            {
                MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_ALL_SERVICE);
            }
        }
        
        break;
        
    case MSG_GET_FOCUS:
        {
            s_main_service_count = GUILIST_GetTotalItemNum(ctrl_id);
            if (s_main_service_index < s_main_service_count)
            {
                GUILIST_SetCurItemIndex(ctrl_id, s_main_service_index);
            }
            else
            {
                s_main_service_index = 0;
                GUILIST_SetCurItemIndex(ctrl_id, 0);
            }
            MMIMBBMS_RefreshMainWinMiddleSoftkey(win_id);

#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            break;
        }
    case MSG_NOTIFY_TAB_SWITCH:
        {
            GUILIST_RemoveAllItems(ctrl_id);
            CreateAllServiceList();
            
            s_main_service_count = GUILIST_GetTotalItemNum(ctrl_id);
            
            if (s_main_service_index < s_main_service_count)
            {
                GUILIST_SetCurItemIndex(ctrl_id, s_main_service_index);
            }
            else
            {
                s_main_service_index = 0;
                GUILIST_SetCurItemIndex(ctrl_id, 0);
            }
            
            MMIMBBMS_RefreshMainWinMiddleSoftkey(win_id);
            
            break;
        }
    case MSG_APP_MBBMS_REFRESH_LIST:
        {
            GUILIST_RemoveAllItems(ctrl_id);
            CreateAllServiceList();
            
            s_main_service_count = GUILIST_GetTotalItemNum(ctrl_id);
            
            if (s_main_service_index < s_main_service_count)
            {
                GUILIST_SetCurItemIndex(ctrl_id, s_main_service_index);
            }
            else
            {
                s_main_service_index = 0;
                GUILIST_SetCurItemIndex(ctrl_id, 0);
            }
            
            //s_main_service_index = GUILIST_GetCurItemIndex(ctrl_id);
            
            MMIMBBMS_RefreshMainWinMiddleSoftkey(win_id);
            
            break;
        }
    case MSG_LOSE_FOCUS:
        {
            s_main_service_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_main_service_count = GUILIST_GetTotalItemNum(ctrl_id);
            break;
        }
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }    
            
            if (MMK_IsOpenWin(MMIMBBMS_MAIN_INTERFACE_POPMENU_WIN_ID))
            {
                MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_POPMENU_WIN_ID);
            }
            MMK_CreateWin((uint32 *)MMIMBBMS_ALL_SERVICE_POPMENU_WIN_TAB,PNULL);
            break;
        }

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID);
        break;

    //temp test
    //case MSG_APP_UPSIDE:
        //MMIMBBMS_NotifyEventCallback(MN_DUAL_SYS_1, MMIPHONE_NOTIFY_EVENT_PS_AVAIL, PNULL);
        //break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleAllFavoriteServiceChildWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面收藏频道子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceChildWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param)
{
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        
        CreateFavoriteServiceList();
        
        if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
            if(MMIMBBMS_GetInitStatus())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
            }
        }
        break;
        
    case MSG_NOTIFY_TAB_SWITCH:
        GUILIST_RemoveAllItems(ctrl_id);
        CreateFavoriteServiceList();
        
        if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
            if(MMIMBBMS_GetInitStatus())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
            }
        }
        break;
        
    case MSG_APP_MBBMS_REFRESH_LIST:
        GUILIST_RemoveAllItems(ctrl_id);
        CreateFavoriteServiceList();
        
        if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
            if(MMIMBBMS_GetInitStatus())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
            }
        }
        break;
        
    case MSG_GET_FOCUS:
        if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
       }
        else
        {
            if(MMIMBBMS_GetInitStatus())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
            }
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            /*add for init not finished*/
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }  
            
            if(MMIMBBMS_IsCurrServiceInFavoriteTabEncrypted() && (!MMIMBBMS_GetSubscriptionUpdateStatus()))
            {
                MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                s_object_type=MMIMBBMS_OBJECT_FROM_FAVORITE_SERVICE;
                MMIMBBMS_OpenSubscriptionUpdateQueryWin();
            }
            else
            {
                MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_FAVORITE_SERVICE);
            }
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }   
            
            if (MMK_IsOpenWin(MMIMBBMS_MAIN_INTERFACE_FS_POPMENU_WIN_ID))
            {
                MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_FS_POPMENU_WIN_ID);
            }
            MMK_CreateWin((uint32 *)MMIMBBMS_FAVORITE_SERVICE_POPMENU_WIN_TAB, PNULL);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;    
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleAllFavoriteServiceChildWinMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note: 主界面收藏节目子窗口处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmContentChildWinMsg(
                                                    MMI_WIN_ID_T        win_id, 
                                                    MMI_MESSAGE_ID_E    msg_id, 
                                                    DPARAM              param)
{
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        
        CreateAlarmContentList();
        
        if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
            if(MMIMBBMS_GetInitStatus())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
            }
        }
        break;
        
//     case MSG_NOTIFY_TAB_SWITCH:
//         
//         GUILIST_RemoveAllItems(ctrl_id);
//         CreateAlarmContentList();
//         
//         if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
//         {
//             GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
//         }
//         else
//         {
//             GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
//         }
//         break;
        
    case MSG_APP_MBBMS_REFRESH_LIST:
        GUILIST_RemoveAllItems(ctrl_id);
        CreateAlarmContentList();
        
        if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
            if(MMIMBBMS_GetInitStatus())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
            }
        }
        break;
        
    case MSG_GET_FOCUS:
        if(0 ==GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_EXIT, FALSE);
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
        }
        else
        {
            if(MMIMBBMS_GetInitStatus())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MBBMS_WATCH, STXT_EXIT, FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_EXIT, FALSE);
            }
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            /*add for init not finished*/
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }  
            
            if(MMIMBBMS_IsCurrContentInFavoriteTabEncrypted() && (!MMIMBBMS_GetSubscriptionUpdateStatus()))
            {
                MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
                s_object_type= MMIMBBMS_OBJECT_FROM_FAVORITE_CONTENT;
                MMIMBBMS_OpenSubscriptionUpdateQueryWin();
            }
            else
            {
                MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_FAVORITE_CONTENT);
            }
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            if(!MMIMBBMS_DisplayInitWaitingWin())
            {
                break;
            }   
            
            if (MMK_IsOpenWin(MMIMBBMS_MAIN_INTERFACE_FC_POPMENU_WIN_ID))
            {
                MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_FC_POPMENU_WIN_ID);
            }
            MMK_CreateWin((uint32 *)MMIMBBMS_FAVORITE_CONTENT_POPMENU_WIN_TAB, PNULL);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 创建TAB主界面
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_OpenMainInterfaceWin(void)
{
    
    //SCI_TRACE_LOW:"MMIMBBMS_OpenMainInterfaceWin enter MMIMBBMS_TickCount=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_5938_112_2_18_2_39_58_447,(uint8*)"d",SCI_GetTickCount());
    
    if (MMK_IsOpenWin(MMIMBBMS_STARTUP_WIN_ID) && !MMK_IsFocusWin(MMIMBBMS_STARTUP_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMIMBBMS_OpenMainInterfaceWin signal \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_5942_112_2_18_2_39_58_448,(uint8*)"");
        
        if (0 == s_timer_id)
        {
            s_timer_id = MMK_CreateTimerCallback(1000, MMIMBBMS_HandleStartTimer, NULL,TRUE);
            timer_master_query = FALSE;
        }
        
        return FALSE;
    }
    
    if (0 != s_timer_id)
    {
        MMK_StopTimer(s_timer_id);
        s_timer_id = 0;
    }
    
    MMK_CreateParentWin((uint32 *)MMIMBBMS_MAIN_INTERFACE_WIN_TAB, PNULL);
    MMK_CreateChildWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID, (uint32 *)MMIMBBMS_ALL_SERVICE_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID, (uint32 *)MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID, (uint32 *)MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_TAB, NULL);
    
    MMK_SetChildWinFocus(MMIMBBMS_MAIN_INTERFACE_WIN_ID, MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID);
    
    //SCI_TRACE_LOW:"MMIMBBMS_OpenMainInterfaceWin end MMIMBBMS_TickCount=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_5966_112_2_18_2_39_58_449,(uint8*)"d",SCI_GetTickCount());
    return TRUE;
}

/*****************************************************************************/
// 	Description : 增加主界面Tab信息 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void MainWinAppendTabInfo(void)
{
    GUITAB_ITEM_T   item_t = {0};
    GUITAB_ITEM_CONTENT_T item_data_t = {0};
    
    GUITAB_SetMaxItemNum(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID, 3);
    GUITAB_SetItemSpace(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID,MMIMBBMS_MAINTAB_ITEM_SPACE,FALSE);
    GUITAB_SetState(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID,GUITAB_STATE_NONEED_SCROLL,TRUE);
    
    item_t.item_state = GUITAB_ITEMSTATE_SHOW_TEXT;
    item_t.item_data_ptr = &item_data_t;
    item_data_t.text_data.text_type = TABITEM_DATA_TEXT_ID;
    item_data_t.text_data.is_disp_font_bg = TRUE;
    
    //所有频道
    item_t.child_win_handle = MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID;
    item_data_t.text_data.text.text_id = TXT_MBBMS_ALL_SERVICE;
    GUITAB_AppendItem(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID, &item_t);
    
    //收藏频道
    item_t.child_win_handle = MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID;
    item_data_t.text_data.text.text_id = TXT_MBBMS_FAVORITE_SERVICE;
    GUITAB_AppendItem(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID, &item_t);
    
    //收藏节目
    item_t.child_win_handle = MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID;
    item_data_t.text_data.text.text_id = TXT_MBBMS_CONTENT_ALARM;
    GUITAB_AppendItem(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID, &item_t);
    
    GUITAB_SetCurSel(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID, 0);
}

/*****************************************************************************/
//  Description : get wchar name by languageString
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNameWcharByLangString(languageString        *name_ptr, //in
                                       MTV_SERVICE_MODE_E   mode,     //service mode
                                       uint32 char_set,             //char_set while cmmb mode
                                       uint32 service_id,           //cmmb mode used while service name null
                                       wchar  *wchar_name,           //out
                                       uint16 wstr_len)
{
    uint16  str_len = 0;
    uint16  to_len = 0;
    uint8   *str_buf_ptr = PNULL;
    uint8   str_service_name[MMIMBBMS_SERVICE_NAME_MAX_LEN] = {0};
    
    if(PNULL == name_ptr
        || PNULL == wchar_name)
    {
        return str_len;
    }
    
    if(PNULL != name_ptr->content_string_ptr)
    {
        str_buf_ptr = name_ptr->content_string_ptr;
    }
    else
    {
        str_buf_ptr = name_ptr->string_arr;
    }
    
    to_len = MIN(name_ptr->string_len, wstr_len);
    
    if(MBBMS_SERVICE_MODE_MBBMS == mode)
    {
        str_len = GUI_UTF8ToWstr(wchar_name,
                                to_len,
                                str_buf_ptr,
                                to_len);
    }
    else
    {
        //cmmb mode need char_set
        if(to_len > 0)
        {
            switch(char_set)
            {
            case MTV_STIMI_GB_13000_1:
                MMI_WSTRNCPY(wchar_name,
                             wstr_len,
                             (wchar*)str_buf_ptr,
                             name_ptr->string_len,
                             to_len);
                break;
                
            case MTV_STIMI_GB_2312:
                GUI_GBToWstr(wchar_name,
                             str_buf_ptr,
                             to_len);
                break;
                
            case MTV_STIMI_UTF8:
                str_len = GUI_UTF8ToWstr(wchar_name,
                                        to_len,
                                        str_buf_ptr,
                                        to_len);
                break;
                
            default:
                //default ascii
                MMI_STRNTOWSTR(wchar_name,
                               wstr_len,
                               str_buf_ptr,
                               name_ptr->string_len,
                               to_len);
                break;
            }
        }
        else
        {
            //频道名为空的处理
            sprintf((char*)str_service_name, "%d", service_id);
            to_len = strlen((char*)str_service_name);
            str_len = to_len;
            MMI_STRNTOWSTR(wchar_name,
                           wstr_len,
                           str_service_name,
                           to_len,
                           to_len);
        }
    }
    
    return str_len;
}

/*****************************************************************************/
// Description : get the start and end time info of content
// Global resource dependence : none
// Author: xin.li
// Note: out format 00:00-00:00
/*****************************************************************************/
LOCAL void  GetTimeInfoByStartEndTime(SG_DATETIME_T* starttime_ptr, //in
                                      SG_DATETIME_T* endtime_ptr,   //in
                                      wchar* pwchar //out
                                      )
{
    char    str_date[10] = {0};
    wchar   wchar_starttime[5] = {0};
    wchar   wchar_endtime[5] = {0};
    
    if(PNULL == starttime_ptr
        || PNULL == endtime_ptr
        || PNULL == pwchar)
    {
        return;
    }
    
    //start time
    sprintf(str_date, "%2d:%2d", starttime_ptr->hours, starttime_ptr->minutes);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_starttime);
    
    //end time
    sprintf(str_date, "%2d:%2d", endtime_ptr->hours, endtime_ptr->minutes);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_endtime);
    
    //merge
    MMIAPICOM_Wstrncpy(pwchar, wchar_starttime,  5);
    pwchar[5] = (wchar)'-';
    MMIAPICOM_Wstrncpy(pwchar+6, wchar_endtime, 5);
    pwchar[11] = (wchar)' ';
    pwchar[12] = '\0';
}

/*****************************************************************************/
// 	Description : 创建所有频道列表
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void CreateAllServiceList(void)
{
    uint32          i = 0;
    SG_ERROR_E      error_code  = SG_ERROR_NONE;
    anyURI          globalServiceId = {0};
    MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T* pinfo_node = PNULL;
    MTVSERVICE_PROGRAM_LIST_T service_list_t = {0};
    MTVSERVICE_PROGRAM_T      *service_arry_ptr = PNULL;
    
    MMIMTV_NewDataData();
    
    //获取频道列表数据
    MMIMBBMS_GetProgramList(&service_list_t);
    service_arry_ptr = service_list_t.list_ptr;
    
    //SCI_TRACE_LOW:"[MMIMBBMS]: CreateAllServiceList  list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6178_112_2_18_2_39_59_450,(uint8*)"d", service_list_t.total_count);
    GUILIST_SetMaxItem(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID, (uint16)(service_list_t.total_count), FALSE);
    
    //alloc display node
    pinfo_node = (MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T*)SCI_ALLOC_APP(sizeof(MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T));
    if(PNULL == pinfo_node)
    {
        //SCI_TRACE_LOW:"[MMIMBBMS]: CreateAllServiceList pinfo_node alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6185_112_2_18_2_39_59_451,(uint8*)"");
        return;
    }
    
    for(i=0; i<service_list_t.total_count; i++)
    {
        SCI_MEMSET(pinfo_node, 0, sizeof(MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T));
        
        if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
        {
            //频道名
            GetNameWcharByLangString(&service_arry_ptr[i].name, 
                MBBMS_SERVICE_MODE_MBBMS,
                0,
                0,
                pinfo_node->service_name,
                MMIMBBMS_SERVICE_NAME_MAX_LEN);
            
            pinfo_node->is_exist_content = service_arry_ptr[i].is_exist_content;
            if(pinfo_node->is_exist_content)
            {
                //节目名
                GetNameWcharByLangString(&service_arry_ptr[i].curcontentname,
                    MBBMS_SERVICE_MODE_MBBMS,
                    0,
                    0,
                    pinfo_node->content_info,
                    MMIMBBMS_SERVICE_NAME_MAX_LEN);
                
                //节目起始时间信息
                GetTimeInfoByStartEndTime(&service_arry_ptr[i].curcontentstartTime, &service_arry_ptr[i].curcontentendTime, pinfo_node->time_info);
            }
            
            //订购，加密，免费标志
            pinfo_node->is_ordered      = service_arry_ptr[i].is_subscribed;
            pinfo_node->is_encrypted    = service_arry_ptr[i].is_encryped;
            pinfo_node->forfree         = service_arry_ptr[i].is_forfree;
            
            //是否已加入收藏频道
            error_code = MBBMSSG_GetGlobalServiceId(service_arry_ptr[i].index, &globalServiceId );
            if(SG_ERROR_NONE != error_code)
            {
                //SCI_TRACE_LOW:"[MMIMBBMS]:MBBMSSG_GetGlobalServiceId is error = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6227_112_2_18_2_39_59_452,(uint8*)"d", error_code);
            }
            
            if(MMIMBBMS_IsFavoriteService(&globalServiceId))
            {
                pinfo_node->is_collected = TRUE;
            }
        }
        else
        {
            //CMMB模式 频道名
            GetNameWcharByLangString(&service_arry_ptr[i].name, 
                MBBMS_SERVICE_MODE_CMMB,
                service_arry_ptr[i].char_set,
                service_arry_ptr[i].service_id,
                pinfo_node->service_name,
                MMIMBBMS_SERVICE_NAME_MAX_LEN);
            
            //是否免费频道
            pinfo_node->forfree = service_arry_ptr[i].is_forfree;
        }
        
        //add item to listbox
        AddServiceItemToListboxCtrl(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID, pinfo_node);
    }
    
    SCI_FREE(pinfo_node);
    pinfo_node = PNULL;
    
    return;
}

/*****************************************************************************/
// 	Description : 创建收藏频道列表
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void CreateFavoriteServiceList(void)
{
    uint32       list_num = 0;
    uint32       index = 0;
    uint32       index_temp = 0;
    anyURI       service_id = {0};
    SG_ERROR_E   errcode = SG_ERROR_NONE;
    
    SG_LIST_PTR_T service_list_head_ptr = PNULL;
    MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T info_node = {0};
    MBBMSSG_SERVICE_T   service_node = {0};
    MBBMSSG_SERVICE_INDEX_T* service_index_ptr = PNULL;
    
    if(MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode())
    {
        GUILIST_SetMaxItem(MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID, 0, FALSE);
        return;
    }
    
    errcode = MMIMBBMS_GetServiceIndex(&service_list_head_ptr);
    
    if(SG_ERROR_NONE != errcode)
    {
        //SCI_TRACE_LOW:"CreateFavoriteServiceList:Get Service Index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6286_112_2_18_2_39_59_453,(uint8*)"");
        return;
    }
    
    list_num = MMIMBBMS_GetFavoriteServiceListNum();
    //SCI_TRACE_LOW:"[MMIMTV]: CreateFavoriteServiceList  list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6291_112_2_18_2_39_59_454,(uint8*)"d", list_num);
    
    if(list_num > 0)
    {
        GUILIST_SetMaxItem(MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID, (uint16)list_num, FALSE);
        
        while(list_num --)
        {
            SCI_MEMSET(&info_node, 0, sizeof(MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T));
            
            //由globalServiceId获取serviceId
            SCI_MEMSET(&service_id,0,sizeof(anyURI));
            MBBMSSG_GetServiceId(MMIMBBMS_GetFavoriteServiceNodeViaIndex(index)->globalServiceID, &service_id);
            
            //  service_id.anyURI_arr[service_id.string_len] = 0;
            
            //从频道分片MBBMSSG_SERVICE_T中获取相关信息
            errcode = MBBMSSG_GetService(service_id, &service_node);
            
            if (SG_ERROR_NONE != errcode)
            {
                //SCI_TRACE_LOW:"[MMIMTV]: CreateFavoriteServiceList MBBMSSG_GetService err = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6312_112_2_18_2_39_59_455,(uint8*)"d", errcode);
                index ++;
                MBBMSSG_FreeTypeAnyURI(&service_id);
                continue;
            }
            
            GetInfoFromService(&service_id, &service_node, &info_node);
            
            info_node.is_collected = TRUE;
            info_node.is_ordered = MMIMBBMS_IsServiceOrdered(service_id);
            info_node.is_encrypted = MMIMBBMS_IsServiceEncrypted(service_id);
            
            index_temp = MMIMBBMS_GetServiceIndexByServiceID(&service_id);
            
            //通过索引,在所有频道列表中查找,获取播放需要的信息, frequery, service_id,
            service_index_ptr=(MBBMSSG_SERVICE_INDEX_T*)STL_LinkListGetNodeDataByIdx(service_list_head_ptr, index_temp);
            if(PNULL!=service_index_ptr)
            {
                info_node.forfree = service_index_ptr->forfree;
            }
            
            //add to listbox
            AddServiceItemToListboxCtrl(MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID, &info_node);
            
            MBBMSSG_FreeTypeAnyURI(&service_id);
            
            //free service
            MBBMSSG_FreeService(&service_node);
            
            index ++;
        }
    }
    return;
}

/*****************************************************************************/
// 	Description : 创建节目提醒列表
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void CreateAlarmContentList(void)
{
    uint32          list_num = 0;
    uint32          index = 0;
    anyURI          content_id = {0};
    SG_ERROR_E      errcode = SG_ERROR_NONE;
	char datestr[11]={0};
    MMIMBBMS_SAVE_CONTENT_INFO_T* pAlarmContent = PNULL;
    
    MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T info_node = {0};
    MBBMSSG_CONTENT_T   content_node = {0};
    
    if(MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode())
    {
        GUILIST_SetMaxItem(MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID, 0, FALSE);
        return;
    }
    
    list_num = MMIMBBMS_GetAlarmContentListNum();
    //SCI_TRACE_LOW:"[MMIMTV]: CreateAlarmContentList  list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6370_112_2_18_2_40_0_456,(uint8*)"d", list_num);
    
    if(list_num > 0)
    {
        GUILIST_SetMaxItem(MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID, (uint16)list_num, FALSE);
        
        while(list_num --)
        {
            SCI_MEMSET(&info_node, 0, sizeof(MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T));
            
            //由globalContentId获取contentId
            SCI_MEMSET(&content_id,0,sizeof(anyURI));
            
            pAlarmContent = (MMIMBBMS_SAVE_CONTENT_INFO_T*)MMIMBBMS_GetAlarmContentNodeViaIndex(index);
            MBBMSSG_GetContentId(pAlarmContent->globalContentID, &content_id);
            
            //从节目分片MBBMSSG_CONTENT_T中获取相关信息
            errcode = MBBMSSG_GetContent(content_id, &content_node);
            
            if (SG_ERROR_NONE != errcode)
            {
                //SCI_TRACE_LOW:"[MMIMTV]: CreateAlarmContentList MBBMSSG_GetContent ERR = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6391_112_2_18_2_40_0_457,(uint8*)"d", errcode);
                index ++;
                MBBMSSG_FreeTypeAnyURI(&content_id);
                continue;                
            }
            
            GetInfoFromContent(&content_node, &info_node);
            
            //设置提醒
            info_node.is_set_alarm = TRUE;
			//设置日期
    		sprintf(datestr, "%2d/%2d/%2d",
        		pAlarmContent->starttime.years, 
        		pAlarmContent->starttime.months,
        		pAlarmContent->starttime.days);	
    
			    if ((info_node.start_date)[5] == 0x20)
			    {
			        (info_node.start_date)[5] = '0';
			    }
			    
			    if ((info_node.start_date)[8] == 0x20)
			    {
			        (info_node.start_date)[8] = '0';
			    }
    
			    MMIAPICOM_StrToWstr((uint8*)datestr, info_node.start_date);
			    info_node.start_date[10] = '\0';							

	
            //add to listbox
            AddContentItemToListboxCtrl(MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID,
                &info_node, &(content_node.serviceIdRef));
            
            MBBMSSG_FreeTypeAnyURI(&content_id);
            
            //free service
            MBBMSSG_FreeContent(&content_node);
            
            index ++;
        }
    }
    
    return;
}

/*****************************************************************************/
// 	Description : 添加一个频道显示节点
//	Global resource dependence : 
//  Author: 
//	Note: 针对所有频道/收藏频道
/*****************************************************************************/
LOCAL void AddServiceItemToListboxCtrl(MMI_CTRL_ID_T    ctrl_id,
                                       MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T* node_ptr)
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    wchar               wline2[MMIMBBMS_SERVICE_NAME_MAX_LEN + MMIMBBMS_START_TIME_MAX_LEN +1] = {0};
    
    if(PNULL == node_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMBBMS]: AddServiceItemToListboxCtrl node_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6450_112_2_18_2_40_0_458,(uint8*)"");
        return;
    }
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON;
    item_t.item_data_ptr = &item_data;
    
    //image 1
    item_data.item_content[0].item_data_type        = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id    = IMG_PRO_LIST;
    
    //text 1
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = node_ptr->service_name;
    if(MMIAPICOM_Wstrlen(node_ptr->service_name) > GUILIST_STRING_MAX_NUM)
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = GUILIST_STRING_MAX_NUM;
    }
    else
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(node_ptr->service_name);
    }
     
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        if(node_ptr->is_collected)
        {
            //icon 1   收藏图标
            item_data.item_content[3].item_data_type        = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[3].item_data.image_id    = IMAGE_MBBMS_COLLECT;
        }
        
        // 清流节目不显示图标；
        if (node_ptr->is_encrypted == TRUE)
            //if (node_ptr->forfree == FALSE)
        {
            //icon 2   订购图标
            if(node_ptr->is_ordered || node_ptr->forfree == TRUE)
            {
                item_data.item_content[4].item_data_type        = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[4].item_data.image_id    = IMAGE_MBBMS_PAIED;
            }
            else
            {
                item_data.item_content[4].item_data_type        = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[4].item_data.image_id    = IMAGE_MBBMS_UNPAIED;
            }
        }
        
        if(node_ptr->is_exist_content)
        {
            //text 2
            //合并起始时间/节目名
            MMIAPICOM_Wstrcpy(wline2, node_ptr->time_info);
            MMIAPICOM_Wstrcpy(wline2 + MMIAPICOM_Wstrlen(node_ptr->time_info), node_ptr->content_info);
            item_data.item_content[5].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[5].item_data.text_buffer.wstr_ptr = wline2;
            if(MMIAPICOM_Wstrlen(wline2) > GUILIST_STRING_MAX_NUM)
            {
                item_data.item_content[5].item_data.text_buffer.wstr_len = GUILIST_STRING_MAX_NUM;
            }
            else
            {
                item_data.item_content[5].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wline2);
            }
        }
    }
    else
    {
        //CMMB模式 非免费频道标识为加密频道
        if(!node_ptr->forfree)
        {
            item_data.item_content[4].item_data_type        = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[4].item_data.image_id    = IMAGE_MBBMS_UNPAIED;
        }
    }
    
    GUILIST_AppendItem(ctrl_id, &item_t);
    
    SCI_MEMSET(&item_data, 0, sizeof(item_data));
}

/*****************************************************************************/
// 	Description : 添加一个收藏节目显示节点 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void AddContentItemToListboxCtrl(MMI_CTRL_ID_T    ctrl_id,
                                       MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T* node_ptr,
                                       anyURI* pURI)
{
    wchar               relevant_info[MMIMBBMS_SERVICE_NAME_MAX_LEN +1] = {0};
    wchar               wstr_space[] = {' ', 0};
    uint16              length = 0;
    GUILIST_ITEM_T      item_t = {0};   /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    if(PNULL == node_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMBBMS]: AddContentItemToListboxCtrl node_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6548_112_2_18_2_40_0_459,(uint8*)"");
        return;
    }
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON;
    item_t.item_data_ptr = &item_data;
    
    //image 1
    item_data.item_content[0].item_data_type        = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id    = IMG_PRO_LIST;
    
    //text 1
    item_data.item_content[1].item_data_type        = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = node_ptr->content_name;
    if(MMIAPICOM_Wstrlen(node_ptr->content_name) > GUILIST_STRING_MAX_NUM)
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = GUILIST_STRING_MAX_NUM;
    }
    else
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(node_ptr->content_name);
    }
    
    //icon 1   收藏图标
    item_data.item_content[2].item_data_type        = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[2].item_data.image_id    = IMAGE_MBBMS_COLLECT;  //to do
    
    //icon 2   节目提醒图标
    if(node_ptr->is_set_alarm)
    {
        item_data.item_content[3].item_data_type        = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[3].item_data.image_id    = IMAGE_MBBMS_NOTIFY;  //to do
    }
	
    //text 2
    //合并 起讫时间/日期/关联频道
    //合并时间
    MMI_WSTRNCPY(relevant_info, MMIMBBMS_SERVICE_NAME_MAX_LEN,
        node_ptr->start_time, MMIMBBMS_START_TIME_MAX_LEN,
        MMIAPICOM_Wstrlen(node_ptr->start_time));
    //插入空格
    length = MMIAPICOM_Wstrlen(node_ptr->start_time);
    MMI_WSTRNCPY(relevant_info + length, MMIMBBMS_SERVICE_NAME_MAX_LEN - length,
        wstr_space, MMIAPICOM_Wstrlen(wstr_space),
    MMIAPICOM_Wstrlen(wstr_space));	
	//合并日期
	length +=1;    
	MMI_WSTRNCPY(relevant_info + length, MMIMBBMS_SERVICE_NAME_MAX_LEN- length,
	    node_ptr->start_date, MMIMBBMS_START_TIME_MAX_LEN,
	MMIAPICOM_Wstrlen(node_ptr->start_date));		
	//插入空格
	length +=MMIAPICOM_Wstrlen(node_ptr->start_date);
	MMI_WSTRNCPY(relevant_info + length, MMIMBBMS_SERVICE_NAME_MAX_LEN - length,
	    wstr_space, MMIAPICOM_Wstrlen(wstr_space),
	MMIAPICOM_Wstrlen(wstr_space));	
    //频道名
    length += MMIAPICOM_Wstrlen(wstr_space);
    MMI_WSTRNCPY(relevant_info + length, MMIMBBMS_SERVICE_NAME_MAX_LEN - length,
        node_ptr->service_name, MMIMBBMS_SERVICE_NAME_MAX_LEN,
    MMIAPICOM_Wstrlen(node_ptr->service_name));
    
    item_data.item_content[5].item_data_type        = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[5].item_data.text_buffer.wstr_ptr = relevant_info;
    if(MMIAPICOM_Wstrlen(relevant_info) > GUILIST_STRING_MAX_NUM)
    {
        item_data.item_content[5].item_data.text_buffer.wstr_len = GUILIST_STRING_MAX_NUM;
    }
    else
    {
        item_data.item_content[5].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(relevant_info);
    }
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
// 	Description : 从MBBMSSG_SERVICE_INDEX_T中获取相关信息
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL SG_ERROR_E GetInfoFromServiceIndex(
                                         const anyURI*  index_ptr,   //[in]
                                         MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T* info_node_ptr   //[out]
                                         )
{
    MBBMSSG_SERVICE_T serviceItem = {0};
    SG_ERROR_E      errcode = SG_ERROR_NONE;
    
    if(PNULL == index_ptr
        || PNULL == info_node_ptr)
    {
        return SG_ERROR_FAIL;
    }
    
    errcode = MBBMSSG_GetService(*index_ptr, &serviceItem);
    
    if (errcode != SG_ERROR_NONE)
    {
        //SCI_TRACE_LOW:"MBBMSSG_GetService errcode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6645_112_2_18_2_40_0_460,(uint8*)"d", errcode);
        return errcode;
    }
    
    MMIMBBMS_GetNameWchar(serviceItem.NameList, info_node_ptr->service_name,
        MMIMBBMS_SERVICE_NAME_MAX_LEN);
    
    info_node_ptr->is_ordered = MMIMBBMS_IsServiceOrdered(*index_ptr);
    info_node_ptr->is_encrypted = MMIMBBMS_IsServiceEncrypted(*index_ptr);
    
    MBBMSSG_FreeService(&serviceItem);
    
    return SG_ERROR_NONE;
}

/*****************************************************************************/
// 	Description : 从MBBMSSG_SERVICE_T中获取相关信息
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void GetInfoFromService(anyURI* pServiceID,
                              const MBBMSSG_SERVICE_T*  service_ptr,               //[in]
                              MMIMBBMS_SERVICEITEM_DISPLAY_INFO_T* info_node_ptr   //[out]
                              )
{
    wchar   service_name[MMIMBBMS_SERVICE_NAME_MAX_LEN +1] = {0};
    wchar   content_name[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    uint16  service_name_len = 0;
    SCI_DATE_T sysdate  = {0};
    SG_DATETIME_T   date = {0};
    SG_ERROR_E err_code = SG_ERROR_NONE;
    SG_LIST_PTR_T pItem = PNULL;
    SG_LIST_PTR_T list_potr = PNULL;
    anyURI*  pURI_index = PNULL;
    //MBBMSSG_CONTENT_T*  pcontentItem = PNULL;
    MBBMSSG_CONTENT_INDEX_T *pcontentidex = PNULL;
    SG_DATETIME_T minDatetime = {0};
    SG_DATETIME_T maxDatetime = {0};
    SG_DATETIME_T nowDatetime = {0};
    
    //SG_DATETIME_T rightDatetime = {0};
    MBBMSSG_CONTENT_T  rightContentItem = {0};
    MBBMSSG_CONTENT_T  maxContentItem = {0};
    BOOLEAN bMin = FALSE;
    BOOLEAN bRight = FALSE;
    BOOLEAN bMax = FALSE;
    SCI_TIME_T time_info = {0};
    
    TM_GetSysTime(&time_info);
    
    minDatetime.hours = 0;
    minDatetime.minutes = 0;
    
    maxDatetime.hours = 23;
    maxDatetime.minutes = 59;
    
    nowDatetime.hours = time_info.hour;
    nowDatetime.minutes = time_info.min;
    
    service_name_len = MMIMBBMS_GetNameWchar(service_ptr->NameList,
        service_name, MMIMBBMS_SERVICE_NAME_MAX_LEN);
    
    MMI_WSTRNCPY(info_node_ptr->service_name,
        MMIMBBMS_SERVICE_NAME_MAX_LEN,
        service_name,
        MMIMBBMS_SERVICE_NAME_MAX_LEN,
        service_name_len);
    
    TM_GetSysDate(&sysdate);
    
    date.years = sysdate.year;
    date.months = sysdate.mon;
    date.days     = sysdate.mday;
    date.hours    = 0;
    date.minutes = 0;
    date.seconds = 0;
    
    // 根据service接点得到所有content，并加入到listbox
    err_code = MBBMSSG_GetContentIndex(*pServiceID, date, &list_potr);
    
    if (err_code != SG_ERROR_NONE)
    {
        //SCI_TRACE_LOW:"MBBMSSG_GetContentIndex errcode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6726_112_2_18_2_40_0_461,(uint8*)"d", err_code);
        return;
    }
    
    pItem = list_potr;
    while (pItem->next_ptr != PNULL)
    {
        pItem = pItem->next_ptr;
        pcontentidex = (MBBMSSG_CONTENT_INDEX_T*)(pItem->data);
        pURI_index   = &(pcontentidex->index);
        
        if (pURI_index == PNULL)
        {
            //SCI_TRACE_LOW:"pURI_index is pnull"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6739_112_2_18_2_40_0_462,(uint8*)"");
            continue;
        }
        
        SCI_MEMSET(content_name,0,sizeof(content_name));
        MMIMBBMS_GetNameWchar(pcontentidex->NameList, content_name,
            MMIMBBMS_SERVICE_NAME_MAX_LEN);
        
        /* 如果节目开始时间比现在时间晚，则记录这一类中最早的那一个 */
        if ( pcontentidex->startTime.hours > nowDatetime.hours
            || (pcontentidex->startTime.hours == nowDatetime.hours
            &&  pcontentidex->startTime.minutes > nowDatetime.minutes))
        {
            /*  */
            if ( pcontentidex->startTime.hours < maxDatetime.hours
                || (pcontentidex->startTime.hours == maxDatetime.hours
                &&  pcontentidex->startTime.minutes < maxDatetime.minutes))
            {
                maxDatetime.hours = pcontentidex->startTime.hours;
                maxDatetime.minutes = pcontentidex->startTime.minutes;
                
                maxContentItem.StartTime.hours = pcontentidex->startTime.hours;
                maxContentItem.StartTime.minutes = pcontentidex->startTime.minutes;
                maxContentItem.EndTime.hours = pcontentidex->endTime.hours;
                maxContentItem.EndTime.minutes = pcontentidex->endTime.minutes;
                bMax = TRUE;
            }
        }
        /* 如果节目结束时间比现在时间早，则记录这一类中最晚的那一个 */
        else 
            if ( pcontentidex->endTime.hours < nowDatetime.hours
                || (pcontentidex->endTime.hours == nowDatetime.hours
                &&  pcontentidex->endTime.minutes < nowDatetime.minutes))
            {
                if ( pcontentidex->endTime.hours > minDatetime.hours
                    || (pcontentidex->endTime.hours == minDatetime.hours
                    &&  pcontentidex->endTime.minutes > minDatetime.minutes))
                {
                    minDatetime.hours = pcontentidex->startTime.hours;
                    minDatetime.minutes = pcontentidex->startTime.minutes;
                    
                    rightContentItem.StartTime.hours = pcontentidex->startTime.hours;
                    rightContentItem.StartTime.minutes = pcontentidex->startTime.minutes;
                    rightContentItem.EndTime.hours = pcontentidex->endTime.hours;
                    rightContentItem.EndTime.minutes = pcontentidex->endTime.minutes;
                    bMin = TRUE;
                }
            }
            else 
                if (( pcontentidex->startTime.hours < nowDatetime.hours
                    || (pcontentidex->startTime.hours == nowDatetime.hours
                    &&  pcontentidex->startTime.minutes < nowDatetime.minutes))
                    && (pcontentidex->endTime.hours > nowDatetime.hours
                    || (pcontentidex->endTime.hours == nowDatetime.hours
                    &&  pcontentidex->endTime.minutes > nowDatetime.minutes)))
                {
                    rightContentItem.StartTime.hours = pcontentidex->startTime.hours;
                    rightContentItem.StartTime.minutes = pcontentidex->startTime.minutes;
                    rightContentItem.EndTime.hours = pcontentidex->endTime.hours;
                    rightContentItem.EndTime.minutes = pcontentidex->endTime.minutes;
                    
                    bRight = TRUE;
                    break;
                }
    }
    
    //SCI_TRACE_LOW:"[MMIMBBMS] GetInfoFromService bRight=%d, bMax=%d, bMin=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_6805_112_2_18_2_40_0_463,(uint8*)"ddd", bRight,bMax,bMin);
    if (bRight == TRUE)
    {
        MMIMBBMS_GetStartEndTimeString(&rightContentItem, info_node_ptr->time_info);
    }
    else if (bMax == TRUE)
    {
        MMIMBBMS_GetStartEndTimeString(&maxContentItem, info_node_ptr->time_info);
    }
    else if (bMin == TRUE)
    {
        MMIMBBMS_GetStartEndTimeString(&rightContentItem, info_node_ptr->time_info);
    }
    
    MMIAPICOM_Wstrcpy(info_node_ptr->content_info, content_name);
    MBBMSSG_FreeContentIndex(&list_potr);
    //SCI_FREE(pfirstContentItem);
    
    return;
}

/*****************************************************************************/
// 	Description : 从MBBMSSG_SERVICE_T中获取节目相关信息
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void GetInfoFromContent(
                              MBBMSSG_CONTENT_T*  content_ptr,    //[in]
                              MMIMBBMS_FAVORITE_CONTENTITEM_DISPLAY_INFO_T* info_node_ptr   //[out]
                              )
{
    MBBMSSG_SERVICE_T service_info = {0};
    
    if(PNULL == content_ptr
        || PNULL == info_node_ptr)
    {
        return;
    }
    
    MMIMBBMS_GetNameWchar(content_ptr->NameList, 
        info_node_ptr->content_name, MMIMBBMS_SERVICE_NAME_MAX_LEN);
    
    MMIMBBMS_GetStartEndTimeString(content_ptr, info_node_ptr->start_time);
    
    MBBMSSG_GetService(content_ptr->serviceIdRef, &service_info);
    
    MMIMBBMS_GetNameWchar(service_info.NameList, 
        info_node_ptr->service_name, MMIMBBMS_SERVICE_NAME_MAX_LEN);
    
    MBBMSSG_FreeService(&service_info);
    return;    
}

/*****************************************************************************/
// 	Description : 显示空列表 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void SetListboxEmpty(MMI_CTRL_ID_T    ctrl_id)
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    GUILIST_SetMaxItem(ctrl_id, 1, FALSE);
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_EMPTY;
    
    item_data.softkey_id[0] = TXT_NULL;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);   
}

/*****************************************************************************/
// 	Description : handle service list option window msg
//	Global resource dependence : none
//  Author: 
//	Note: 频道列表选项, 第二项为"收藏"
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllServiceOptWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,        
                                              DPARAM param
                                              )
{
    uint16          i = 0;
    uint16          node_id = 0;
    uint16          index  = 0;
    uint16          icount = 0;
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_MAIN_INTERFACE_POPMENU_CTRL_ID;
    SG_ERROR_E      error_code = SG_ERROR_NONE;
    LOCAL anyURI    globalServiceId = {0};
    SG_LIST_PTR_T   pservice_node = PNULL;
    MMIMBBMS_SAVE_SERVICE_INFO_T service_info = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //creat dynamic menu
            GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
            
            // 1. 观看
            InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
            
            // 2. 节目单
            InsertMenuNode(ctrl_id, TXT_MBBMS_CONTENT_LIST, MMIMBBMS_ID_CONTENT_LIST_MENU, 0, i++);
            
            // 3. 收藏此频道/取消收藏
            index = GUILIST_GetCurItemIndex(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID);
            icount = GUILIST_GetTotalItemNum(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID);
            
            if (icount > 0)
            {
                // 根据index得到对应的service节点            
                pservice_node = MMIMBBMS_GetServiceNodeByIdx(index);
            }
            
            if (PNULL != pservice_node)
            {
                //判断是否已加入收藏频道
                SCI_MEMSET(&globalServiceId,0,sizeof(anyURI));
                error_code = MBBMSSG_GetGlobalServiceId(*((anyURI*)(pservice_node->data)), &globalServiceId );
                
                if (error_code == SG_ERROR_NONE && MMIMBBMS_IsFavoriteService(&globalServiceId))
                {
                    InsertMenuNode(ctrl_id, TXT_MBBMS_DEL_FAVORITE_SERVICE, MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU, 0, i++);
                }
                else
                {
                    InsertMenuNode(ctrl_id, TXT_ADD_FAVORITE_SERVICE, MMIMBBMS_ID_ADD_FAVORITE_SERVICE_MENU, 0, i++);
                }
            }
            else
            {
                InsertMenuNode(ctrl_id, TXT_ADD_FAVORITE_SERVICE, MMIMBBMS_ID_ADD_FAVORITE_SERVICE_MENU, 0, i++);
                GUIMENU_SetNodeGrayed(TRUE, 2, 0, ctrl_id);
            }
            
            // 4. 套餐管理
            InsertMenuNode(ctrl_id, TXT_MBBMS_PURCHASE_MANAGE,      MMIMBBMS_ID_PURCHASE_MANAGE_MENU, 0, i++);
            
            // 5. 搜索节目
            InsertMenuNode(ctrl_id, TXT_MBBMS_SEARCH_CONTENT,       MMIMBBMS_ID_SEARCH_CONTENT_MENU, 0, i++);
            //InsertMenuNode(ctrl_id, TXT_MBBMS_OPERATION_MANAGE,     MMIMBBMS_ID_OPERATION_MANAGE_MENU, 0, i++);
            
            // 6. 刷新
            InsertMenuNode(ctrl_id, TXT_REFRESH,     MMIMBBMS_ID_UPDATE_MENU, 0, i++);
            
            // 7. 紧急广播
            InsertMenuNode(ctrl_id, TXT_MBBMS_EB,     MMIMBBMS_ID_EB_MENU, 0, i++);
            
            // 8. 设置
            InsertMenuNode(ctrl_id, STXT_MAIN_SETTING,              MMIMBBMS_ID_SETTING_MENU, 0, i++);
            
            // 9. 帮助
            InsertMenuNode(ctrl_id, TXT_HELP,               MMIMBBMS_ID_HELP_MENU, 0, i++);
			//10.退出
            InsertMenuNode(ctrl_id, STXT_EXIT,               MMIMBBMS_ID_EXIT_MENU, 0, i++);
            
            //mbbms2.0  CMMB模式下灰化不可用菜单
            if(MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode())
            {
                if(0 == s_main_service_count)
                {
                    GUIMENU_SetNodeGrayed(TRUE, 0, 0, ctrl_id);
                }
                GUIMENU_SetNodeGrayed(TRUE, 2, 0, ctrl_id);
                GUIMENU_SetNodeGrayed(TRUE, 3, 0, ctrl_id);
                GUIMENU_SetNodeGrayed(TRUE, 4, 0, ctrl_id);
                GUIMENU_SetNodeGrayed(TRUE, 7, 0, ctrl_id);
            }
            
            //if gba test enabled,then show gba test
#ifdef ENG_SUPPORT
            if(MMIAPIENG_IsInterfaceTestingEnable())
            {
                InsertMenuNode(ctrl_id, TXT_MBBMS_GBA_OPERATION,       MMIMBBMS_ID_MANUAL_GBA_OPERATION_MENU, 0, i++);		  
            }
#endif
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        }
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
            case MMIMBBMS_ID_WATCH_MENU:
                MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_WEB, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_CONTENT_LIST_MENU:
                if(!MMIMBBMS_DisplayInitWaitingWin())
                {
                    break;
                }    
                
                MMIMBBMS_OpenProgramListWin();
                break;
                
            case MMIMBBMS_ID_ADD_FAVORITE_SERVICE_MENU:
                service_info.globalServiceID = globalServiceId;
                MMIMBBMS_AddFavoriteServiceNode(&service_info);
                
                MMIMBBMS_ShowFavoriteMsg(&globalServiceId, MMIMBBMS_TYPE_SERVICE, MMIMBBMS_OPER_ADD);
                
                MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU:
                service_info.globalServiceID = globalServiceId;
                MMIMBBMS_DelFavoriteServiceNodeByServiceID(&(service_info.globalServiceID));
                
                MMIMBBMS_ShowFavoriteMsg(&globalServiceId, MMIMBBMS_TYPE_SERVICE, MMIMBBMS_OPER_DEL);
                
                MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL); 
                break;
                
            case MMIMBBMS_ID_PURCHASE_MANAGE_MENU:
                //MMIMBBMS_OpenSubscribeTypeMainWin();
                MMIMBBMS_SetIfEnterSubFromPlay(FALSE);
                MMIMBBMS_OpenAccountInquiryRequestWin();
                break;
                
            case MMIMBBMS_ID_SEARCH_CONTENT_MENU:
                MMIMBBMS_OpenSearchMainWin();	
                break;
                
            case MMIMBBMS_ID_EB_MENU:
                MMIMBBMS_OpenEBListWin();
                break;
                
            case MMIMBBMS_ID_MANUAL_UPDATE_SG_MENU:
                MMIMBBMS_OpenSGUpdateRequestWin();    
                break;
                
            case MMIMBBMS_ID_SETTING_MENU:
                MMIMBBMS_OpenSettingMainWin();			
                break;
                
            case MMIMBBMS_ID_MANUAL_GBA_OPERATION_MENU:		
                MMIMBBMS_StartGBATest();
                break;
                
            case MMIMBBMS_ID_HELP_MENU:
                if (MMK_IsOpenWin(MMIMBBMS_HELP_INFO_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_HELP_INFO_WIN_ID);
                }
                
                MMK_CreateWin((uint32 *)MMIMBBMS_HELP_INFO_WIN_TAB, PNULL);
                break;
	    case MMIMBBMS_ID_EXIT_MENU:
		//close option menu	
               	MMK_CloseWin(win_id);	
		//exit mbmms
	            MMIMBBMS_ExitMBBMS();         					
	       break;
            case MMIMBBMS_ID_UPDATE_MENU:
                if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
                {
                    MMIMBBMS_OpenUpdateListMainWin();
                }
                else
                {
                    //cmmb mode, launch program search directly
                    MMIMBBMS_OpenSGUpdateRequestWin();
                }
                break;
                
            default:
                break;
            }
            
            MMK_CloseWin(win_id);
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //MBBMSSG_FreeTypeAnyURI(&globalServiceId);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MBBMSSG_FreeTypeAnyURI(&globalServiceId);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : handle favorite service list option window msg
//	Global resource dependence : none
//  Author: 
//	Note: 收藏频道列表 选项,
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFavoriteServiceOptWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,        
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_MAIN_INTERFACE_FS_POPMENU_CTRL_ID;
    uint16          i = 0;
    uint16          node_id = 0;
    uint16 index = 0;
    uint16 icount = 0;
    LOCAL anyURI globalServiceId = {0};
    MMIMBBMS_SAVE_SERVICE_INFO_T* pservice_node = PNULL;
    
    MMIMBBMS_SAVE_SERVICE_INFO_T service_info = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //creat dynamic menu
            GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
            
            // 1. 观看
            InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
            
            // 2. 节目单
            InsertMenuNode(ctrl_id, TXT_MBBMS_CONTENT_LIST, MMIMBBMS_ID_CONTENT_LIST_MENU, 0, i++);
            
            // 3. 收藏此频道/取消收藏
            index = GUILIST_GetCurItemIndex(MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID);
            icount = MMIMBBMS_GetFavoriteServiceListNum();
            
            if (icount > 0)
            {
                // 根据index得到对应的service节点            
                pservice_node = MMIMBBMS_GetFavoriteServiceNodeViaIndex(index);
            }
            
            if (PNULL != pservice_node)
            {
                globalServiceId = pservice_node->globalServiceID;
                InsertMenuNode(ctrl_id, TXT_MBBMS_DEL_FAVORITE_SERVICE, MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU, 0, i++);
            }
            else
            {
                InsertMenuNode(ctrl_id, TXT_MBBMS_DEL_FAVORITE_SERVICE, MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU, 0, i++);
                GUIMENU_SetNodeGrayed(TRUE, 2, 0, ctrl_id);
            }
            
            // 4. 套餐管理
            InsertMenuNode(ctrl_id, TXT_MBBMS_PURCHASE_MANAGE,      MMIMBBMS_ID_PURCHASE_MANAGE_MENU, 0, i++);
            
            // 5. 搜索节目
            InsertMenuNode(ctrl_id, TXT_MBBMS_SEARCH_CONTENT,       MMIMBBMS_ID_SEARCH_CONTENT_MENU, 0, i++);
            //InsertMenuNode(ctrl_id, TXT_MBBMS_OPERATION_MANAGE,     MMIMBBMS_ID_OPERATION_MANAGE_MENU, 0, i++);
            
            // 6. 刷新
            InsertMenuNode(ctrl_id, TXT_REFRESH,     MMIMBBMS_ID_UPDATE_MENU, 0, i++);
            
            // 7. 紧急广播
            InsertMenuNode(ctrl_id, TXT_MBBMS_EB,     MMIMBBMS_ID_EB_MENU, 0, i++);
            
            // 8. 设置
            InsertMenuNode(ctrl_id, STXT_MAIN_SETTING,              MMIMBBMS_ID_SETTING_MENU, 0, i++);
            
            //9. 帮助
            InsertMenuNode(ctrl_id, TXT_HELP,                 MMIMBBMS_ID_HELP_MENU, 0, i++);

			InsertMenuNode(ctrl_id, STXT_EXIT,                 MMIMBBMS_ID_EXIT_MENU, 0, i++);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        }

    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {  
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
            case MMIMBBMS_ID_WATCH_MENU:
                MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_WEB, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_CONTENT_LIST_MENU:
                if(!MMIMBBMS_DisplayInitWaitingWin())
                {
                    break;
                }    
                
                MMIMBBMS_OpenProgramListWin();
                break;
                
            case MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU:
                //service_info.globalServiceID = globalServiceId;
                MMIMBBMS_DelFavoriteServiceNodeByServiceID(&globalServiceId);
                
                MMIMBBMS_ShowFavoriteMsg(&globalServiceId, MMIMBBMS_TYPE_SERVICE, MMIMBBMS_OPER_DEL);
                
                MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_ADD_FAVORITE_SERVICE_MENU:
                service_info.globalServiceID = globalServiceId;
                MMIMBBMS_AddFavoriteServiceNode(&service_info);
                
                MMIMBBMS_ShowFavoriteMsg(&globalServiceId, MMIMBBMS_TYPE_SERVICE, MMIMBBMS_OPER_ADD);
                
                MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_PURCHASE_MANAGE_MENU:
                //MMIMBBMS_OpenSubscribeTypeMainWin();
                MMIMBBMS_SetIfEnterSubFromPlay(FALSE);
                MMIMBBMS_OpenAccountInquiryRequestWin();
                
                //MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_SEARCH_CONTENT_MENU:
                MMIMBBMS_OpenSearchMainWin();	
                break;
                
            case MMIMBBMS_ID_EB_MENU:
                MMIMBBMS_OpenEBListWin();
                break;
                
            case MMIMBBMS_ID_MANUAL_UPDATE_SG_MENU:
                MMIMBBMS_OpenSGUpdateRequestWin();
                break;
                
            case MMIMBBMS_ID_SETTING_MENU:
                MMIMBBMS_OpenSettingMainWin();	
                break;
                
            case MMIMBBMS_ID_HELP_MENU:
                if (MMK_IsOpenWin(MMIMBBMS_HELP_INFO_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_HELP_INFO_WIN_ID);
                }
                
                MMK_CreateWin((uint32 *)MMIMBBMS_HELP_INFO_WIN_TAB, PNULL);
                break;
		    case MMIMBBMS_ID_EXIT_MENU:
			//close option menu	
	               	MMK_CloseWin(win_id);	
			//exit mbmms			            
		            MMIMBBMS_ExitMBBMS();         					
		       break;				
            case MMIMBBMS_ID_UPDATE_MENU:
                if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
                {
                    MMIMBBMS_OpenUpdateListMainWin();
                }
                else
                {
                    //cmmb mode, launch program search directly
                    MMIMBBMS_OpenSGUpdateRequestWin();
                }
                break;				
                
            default:
                break;
            }
            
            MMK_CloseWin(win_id);
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : MMIMBBMS_RefreshMainInterface
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_RefreshMainInterface(void)
{
    MMI_WIN_ID_T win_id = MMIMBBMS_MAIN_INTERFACE_WIN_ID;
    
    if (!MMK_IsOpenWin(win_id))
    {
        //SCI_TRACE_LOW:"MMIMBBMS_RefreshMainInterface: FALSE!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7306_112_2_18_2_40_1_464,(uint8*)"");
        return;
    }
    
    if (GUITAB_GetCurSel(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID) == 0)
    {
        //SCI_TRACE_LOW("MMIMBBMS_TEST:Refresh MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID ");
        win_id = MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID;
        
        MMK_PostMsg(win_id, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
    }
    else if (GUITAB_GetCurSel(MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID) == 1)
    {
        win_id = MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID;
        MMK_PostMsg(win_id, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
        
    }
    else
    {
        win_id = MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID;
        MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
    }
    
    if(MMK_IsFocusWin(win_id))
    {
        MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, NULL);
    }
}

/*****************************************************************************/
// 	Description : handle favorite content list option window msg
//	Global resource dependence : none
//  Author: 
//	Note: 频道列表选项, 第二项为"收藏"
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmContentOptWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,        
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_MAIN_INTERFACE_FC_POPMENU_CTRL_ID;
    uint16          index = 0;
    uint16          i = 0;
    uint16          node_id = 0;
    //LOCAL anyURI    globalContentId = {0};
    MMIMBBMS_ALARM_CONTENT_INFO_T* pcontent_node = PNULL;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //creat dynamic menu
            GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
            
            // 1. 观看
            InsertMenuNode(ctrl_id, TXT_MBBMS_WATCH, MMIMBBMS_ID_WATCH_MENU, 0, i++);
            
            // 2. 节目单
            InsertMenuNode(ctrl_id, TXT_MBBMS_CONTENT_LIST, MMIMBBMS_ID_CONTENT_LIST_MENU, 0, i++);
            
            // 3. 取消提醒
            index = GUILIST_GetCurItemIndex(MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID);
            pcontent_node = MMIMBBMS_GetAlarmContentNodeViaIndex(index);
            if(PNULL != pcontent_node)
            {
                InsertMenuNode(ctrl_id, TXT_MBBMS_CLOSE_CONTENT_ALARM, MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU, 0, i++);
            }
            else
            {
                InsertMenuNode(ctrl_id, TXT_MBBMS_CLOSE_CONTENT_ALARM, MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU, 0, i++);
                GUIMENU_SetNodeGrayed(TRUE, 2, 0, ctrl_id);
            }
            
            // 4. 套餐管理
            InsertMenuNode(ctrl_id, TXT_MBBMS_PURCHASE_MANAGE,      MMIMBBMS_ID_PURCHASE_MANAGE_MENU, 0, i++);
            
            // 5. 搜索节目
            InsertMenuNode(ctrl_id, TXT_MBBMS_SEARCH_CONTENT,       MMIMBBMS_ID_SEARCH_CONTENT_MENU, 0, i++);
            //InsertMenuNode(ctrl_id, TXT_MBBMS_OPERATION_MANAGE,     MMIMBBMS_ID_OPERATION_MANAGE_MENU, 0, i++);
            
            // 6. 刷新
            InsertMenuNode(ctrl_id, TXT_REFRESH,     MMIMBBMS_ID_UPDATE_MENU, 0, i++);
            
            // 7. 紧急广播
            InsertMenuNode(ctrl_id, TXT_MBBMS_EB,     MMIMBBMS_ID_EB_MENU, 0, i++);
            
            // 8. 设置
            InsertMenuNode(ctrl_id, STXT_MAIN_SETTING,              MMIMBBMS_ID_SETTING_MENU, 0, i++);
            
            //9. 帮助
            InsertMenuNode(ctrl_id, TXT_HELP,                 MMIMBBMS_ID_HELP_MENU, 0, i++);
            InsertMenuNode(ctrl_id, STXT_EXIT,                 MMIMBBMS_ID_EXIT_MENU, 0, i++);           
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        }
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {  
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch (node_id)
            {
            case MMIMBBMS_ID_WATCH_MENU:
                MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_WEB, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_CONTENT_LIST_MENU:
                if(!MMIMBBMS_DisplayInitWaitingWin())
                {
                    break;
                }    
                
                MMIMBBMS_OpenProgramListWin();
                break;
                
            case MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU:
                {
                    MMIMBBMS_ALARM_CONTENT_INFO_T* pAlarmContentInfo = PNULL;
                    
                    index = GUILIST_GetCurItemIndex(MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID);
                    pAlarmContentInfo = MMIMBBMS_GetAlarmContentNodeViaIndex(index);
                    if(PNULL != pAlarmContentInfo)
                    {
                        /* 取消节目提醒闹钟 */
                        MMIALMSVC_UnRegService(pAlarmContentInfo->event_id, TRUE);
                        
                        MMIMBBMS_DelAlarmContentNode(pAlarmContentInfo);
                        
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MBBMS_CONTENT_UNREGISTER_NOTIFY_INFO,TXT_NULL,IMAGE_PUBWIN_SUCCESS, 
                            PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,MMIMBBMS_HandleAddorCancelAlarmServiceAlertWinMsg);
                         
                        MMK_CloseWin(win_id);
                        MMK_PostMsg(MMIMBBMS_FAVORITE_CONTENT_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                    }
                }
                break;
                
            case MMIMBBMS_ID_PURCHASE_MANAGE_MENU:
                //MMIMBBMS_OpenSubscribeTypeMainWin();
                MMIMBBMS_SetIfEnterSubFromPlay(FALSE);
                MMIMBBMS_OpenAccountInquiryRequestWin();
                
                //MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                break;
                
            case MMIMBBMS_ID_SEARCH_CONTENT_MENU:
                MMIMBBMS_OpenSearchMainWin();	
                break;
                
            case MMIMBBMS_ID_EB_MENU:
                MMIMBBMS_OpenEBListWin();
                break;
                
            case MMIMBBMS_ID_MANUAL_UPDATE_SG_MENU:
                MMIMBBMS_OpenSGUpdateRequestWin();
                break;
                
            case MMIMBBMS_ID_SETTING_MENU:
                MMIMBBMS_OpenSettingMainWin();	
                break;
                
            case MMIMBBMS_ID_HELP_MENU:
                if (MMK_IsOpenWin(MMIMBBMS_HELP_INFO_WIN_ID))
                {
                    MMK_CloseWin(MMIMBBMS_HELP_INFO_WIN_ID);
                }
                
                MMK_CreateWin((uint32 *)MMIMBBMS_HELP_INFO_WIN_TAB, PNULL);
                break;
		    case MMIMBBMS_ID_EXIT_MENU:
			//close option menu	
	               	MMK_CloseWin(win_id);	
			//exit mbmms		  
		            MMIMBBMS_ExitMBBMS();         					
		       break;							
            case MMIMBBMS_ID_UPDATE_MENU:
                if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
                {
                    MMIMBBMS_OpenUpdateListMainWin();
                }
                else
                {
                    //cmmb mode, launch program search directly
                    MMIMBBMS_OpenSGUpdateRequestWin();
                }
                break;				
                
            default:
                break;
            }
            
            MMK_CloseWin(win_id);
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 频道索引是否有效
//	Global resource dependence : 
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsServiceIndexValid(uint16 index)
{
    if (s_main_service_count == 0
        || index >= s_main_service_count)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
// 	Description : 根据Index索引, 切换频道播放
//	Global resource dependence : 
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_PlayServiceByIndex(uint16 index)
{
    //uint16 index_temp = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID;
    //MBBMSSG_SERVICE_INDEX_T* service_index_ptr = PNULL;
    //SG_ERROR_E      error_code =SG_ERROR_NONE;
    //SG_LIST_PTR_T   service_list_head_ptr = PNULL;
    
    /* modified by jhpeng for plum.peng_cr157069 begin */
    //通过当前记录的节点索引, 计算索引
    //SCI_TRACE_LOW:"MMIMBBMS_PlayServiceByIndex:Get s_main_service_count=%d, index=%d, s_main_service_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7547_112_2_18_2_40_2_465,(uint8*)"ddd", s_main_service_count, index, s_main_service_index);
    
    if (s_main_service_count == 0
        || index >= s_main_service_count)
    {
        return;
    }

    //current index service is playing
    //SCI_TRACE_LOW:"MMIMBBMS_PlayServiceByIndex cmmb_play_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7556_112_2_18_2_40_2_466,(uint8*)"d", MMIMBBMS_GetPlayInfo()->cmmb_play_status);
    if(index == s_main_service_index
        && MMIMTV_PLAY_PLAYING == MMIMBBMS_GetPlayInfo()->cmmb_play_status)
    {
        MMIMTV_DisplayServiceNumber();
        MMIMBBMS_ClearNumSwitchInfo();
        return;
    }
    
    //if playing, stop first
    MMIMBBMS_StopPlay();
    
    //re-calculate index 
    s_main_service_index = index;
    
    /*add for init not finished*/
    if(!MMIMBBMS_DisplayInitWaitingWin())
    {
        return;
    }
    
    //set index item focus
    GUILIST_SetCurItemIndex(ctrl_id, s_main_service_index);
    if(MMIMBBMS_IsCurrServiceInAllTabEncrypted(s_main_service_index) && !MMIMBBMS_GetSubscriptionUpdateStatus())
    {
        MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_BEFORE_PLAY);
        s_object_type=MMIMBBMS_OBJECT_FROM_ALL_SERVICE;
        
        MMIMBBMS_OpenSubscriptionUpdateQueryWin();
    }
    else
    {
        MMIMBBMS_EnterPlayOrOpen(MMIMBBMS_OBJECT_FROM_ALL_SERVICE);
    }
    
    return;
}

/*****************************************************************************/
// 	Description : MMIMBBMS_AddCurPlayServiceIntoFavoriteList
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_RESULT MMIMBBMS_AddCurPlayServiceIntoFavoriteList(void)
{
    anyURI        globalServiceId = {0};
    SG_ERROR_E    error_code = SG_ERROR_NONE;
    SG_LIST_PTR_T service_list_head_ptr = PNULL;
    MBBMSSG_SERVICE_INDEX_T* service_node_ptr = PNULL;
    MMIMBBMS_SAVE_SERVICE_INFO_T service_info = {0};
    
    error_code = MMIMBBMS_GetServiceIndex(&service_list_head_ptr);
    if(SG_ERROR_NONE != error_code)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service Index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7609_112_2_18_2_40_2_467,(uint8*)"");
        return MMIMBBMS_GET_SERVICE_INDEX_ERROR;
    }
    
    // 通过索引得到频道节点
    service_node_ptr =(MBBMSSG_SERVICE_INDEX_T*)STL_LinkListGetNodeDataByIdx(service_list_head_ptr, s_main_service_index);
    if(PNULL == service_node_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service NODE error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7617_112_2_18_2_40_2_468,(uint8*)"");
        return MMIMBBMS_GET_SERVICE_NODE_ERROR;
    }
    
    MBBMSSG_GetGlobalServiceId(service_node_ptr->index, &globalServiceId);
    
    if (MMIMBBMS_IsFavoriteService(&globalServiceId) == TRUE)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:have added into favorite list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7625_112_2_18_2_40_2_469,(uint8*)"");
        MBBMSSG_FreeTypeAnyURI(&globalServiceId);
        return MMIMBBMS_HAVE_FAVORITE;
    }
    
    service_info.service_next = PNULL;
    service_info.globalServiceID = globalServiceId;
    
    MMIMBBMS_AddFavoriteServiceNode(&service_info);
    
    MBBMSSG_FreeTypeAnyURI(&globalServiceId);
    
    return MMIMBBMS_OK;
}

/*****************************************************************************/
// 	Description : MMIMBBMS_DelCurPlayServiceFromFavoriteList
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_RESULT MMIMBBMS_DelCurPlayServiceFromFavoriteList(void)
{
    anyURI        globalServiceId = {0};
    SG_ERROR_E    error_code = SG_ERROR_NONE;
    SG_LIST_PTR_T service_list_head_ptr = PNULL;
    MBBMSSG_SERVICE_INDEX_T* service_node_ptr = PNULL;
    MMIMBBMS_SAVE_SERVICE_INFO_T service_info = {0};
    
    error_code = MMIMBBMS_GetServiceIndex(&service_list_head_ptr);
    if(SG_ERROR_NONE != error_code)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service Index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7655_112_2_18_2_40_2_470,(uint8*)"");
        return MMIMBBMS_GET_SERVICE_INDEX_ERROR;
    }
    
    // 通过索引得到频道节点
    service_node_ptr =(MBBMSSG_SERVICE_INDEX_T*)STL_LinkListGetNodeDataByIdx(service_list_head_ptr, s_main_service_index);
    if(PNULL == service_node_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddCurPlayServiceIntoFavoriteList:Get Service NODE error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7663_112_2_18_2_40_2_471,(uint8*)"");
        return MMIMBBMS_GET_SERVICE_NODE_ERROR;
    }
    
    MBBMSSG_GetGlobalServiceId(service_node_ptr->index, &globalServiceId);
    
    if (MMIMBBMS_IsFavoriteService(&globalServiceId) == FALSE)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_DelCurPlayServiceIntoFavoriteList:have not in favorite list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7671_112_2_18_2_40_2_472,(uint8*)"");
        MBBMSSG_FreeTypeAnyURI(&globalServiceId);
        return MMIMBBMS_NOT_FAVORITE;
    }
    
    service_info.service_next = PNULL;
    service_info.globalServiceID = globalServiceId;
    
    MMIMBBMS_DelFavoriteServiceNodeByServiceID(&globalServiceId);
    
    MBBMSSG_FreeTypeAnyURI(&globalServiceId);
    
    return MMIMBBMS_OK;
}

/*****************************************************************************/
// 	Description : MMIMBBMS_IsCurPlayServiceInFavoriteList
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsCurPlayServiceInFavoriteList(void)
{
    BOOLEAN       bResult = FALSE;
    anyURI        globalServiceId = {0};
    SG_ERROR_E    error_code = SG_ERROR_NONE;
    SG_LIST_PTR_T service_list_head_ptr = PNULL;
    MBBMSSG_SERVICE_INDEX_T* service_node_ptr = PNULL;
    
    error_code = MMIMBBMS_GetServiceIndex(&service_list_head_ptr);
    if(SG_ERROR_NONE != error_code)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_IsCurPlayServiceInFavoriteList:Get Service Index error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7701_112_2_18_2_40_2_473,(uint8*)"");
        return FALSE;
    }
    
    // 通过索引得到频道节点
    service_node_ptr =(MBBMSSG_SERVICE_INDEX_T*)STL_LinkListGetNodeDataByIdx(service_list_head_ptr, s_main_service_index);
    if(PNULL == service_node_ptr)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_IsCurPlayServiceInFavoriteList:Get Service NODE error\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7709_112_2_18_2_40_2_474,(uint8*)"");
        return FALSE;
    }
    
    MBBMSSG_GetGlobalServiceId(service_node_ptr->index, &globalServiceId);
    
    bResult = MMIMBBMS_IsFavoriteService(&globalServiceId);
    
    MBBMSSG_FreeTypeAnyURI(&globalServiceId);
    
    return bResult;
}

/*****************************************************************************/
// 	Description : 获取当前的频道索引
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIMBBMS_GetPlayingServiceIndex(void)
{
    return s_main_service_index;
}

/*****************************************************************************/
// 	Description : 从MBBMSSG_SERVICE_INDEX_T中获取频道名
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL SG_ERROR_E GetServiceNameFromServiceIndex(
                                                const anyURI*  index_ptr,   //[in]
                                                wchar* service_name_ptr   //[out]
                                                )
{
    MBBMSSG_SERVICE_T serviceItem = {0};
    SG_ERROR_E      errcode = SG_ERROR_NONE;
    
    if(PNULL == index_ptr
        || PNULL == service_name_ptr)
    {
        return SG_ERROR_FAIL;
    }
    
    errcode = MBBMSSG_GetService(*index_ptr, &serviceItem);
    
    if (errcode != SG_ERROR_NONE)
    {
        //SCI_TRACE_LOW:"MBBMSSG_GetService errcode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7753_112_2_18_2_40_2_475,(uint8*)"d", errcode);
        return errcode;
    }
    
    MMIMBBMS_GetNameWchar(serviceItem.NameList, service_name_ptr,
        MMIMBBMS_SERVICE_NAME_MAX_LEN);
    
    MBBMSSG_FreeService(&serviceItem);
    
    return SG_ERROR_NONE;
}

/*****************************************************************************/
//  Description : 获取当前的频道索引对应的频道名
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetCurrServiceName(wchar* wchar_name,  //out
                                        uint32 wchar_len    //in
                                        )
{
    uint16                    cur_service_index = s_main_service_index;
    MTVSERVICE_PROGRAM_LIST_T service_list_t = {0};
    MTVSERVICE_PROGRAM_T      *service_arry_ptr = PNULL;

    //获取频道列表数据
    MMIMBBMS_GetProgramList(&service_list_t);
    service_arry_ptr = service_list_t.list_ptr;
    
    if(cur_service_index < service_list_t.total_count 
        && PNULL != service_arry_ptr)
    {
        if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
        {
            GetNameWcharByLangString(&service_arry_ptr[cur_service_index].name,
                MBBMS_SERVICE_MODE_MBBMS,
                0,
                0,
                wchar_name,
                wchar_len);
        }
        else
        {
            GetNameWcharByLangString(&service_arry_ptr[cur_service_index].name,
                MBBMS_SERVICE_MODE_CMMB,
                service_arry_ptr[cur_service_index].char_set,
                service_arry_ptr[cur_service_index].service_id,
                wchar_name,
                wchar_len);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMBBMS_GetCurrServiceName error! index=%  total_count=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7805_112_2_18_2_40_3_476,(uint8*)"d", cur_service_index, service_list_t.total_count);
    }
}

/*****************************************************************************/
// 	Description : insert dyna menu node by label
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void InsertMenuNode(
                          MMI_CTRL_ID_T     ctrl_id,         //控件id
                          MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                          uint16          	node_id,        //节点编号
                          uint16          	parent_node_id, //父节点编号
                          uint16          	index           //位置
                          )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

/*Subscribe management end*/

/*****************************************************************************/
// 	Description : Open startup win 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenStartUpWin(void)
{
    //SCI_TRACE_LOW:"MMIMBBMS_OpenStartUpWin TickCount=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7837_112_2_18_2_40_3_477,(uint8*)"d", SCI_GetTickCount());
    MMK_CreateWin((uint32 *)MMIMBBMS_STARTUP_WIN_TAB,PNULL);
}




/*****************************************************************************/
// 	Description : Open Watch connecting win
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenWatchConnectingWin(void)
{
//     MMI_STRING_T  prompt_str={0};
//     
//     
//     MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);    
//     
//     if(!MMK_IsOpenWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID))
//     {
//         MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMIMBBMS_WATCH_CONNECTING_WIN_ID,IMAGE_NULL,
//             ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleWatchConnectingWinMsg);
//     }
}

/*****************************************************************************/
// 	Description : handle watch connecting win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWatchConnectingWinMsg( 
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:		
        //MMIMBBMS_StopFlow();
        break;

    case MSG_APP_MBBMS_PLAY_CONTENT:
        if(MMK_IsFocusWin(win_id))
        {
            //SCI_TRACE_LOW:"MMIMBBMS HandleWatchConnectingWinMsg msg MSG_APP_MBBMS_PLAY_CONTENT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_7882_112_2_18_2_40_3_478,(uint8*)"");
            MMIMBBMS_EnterPlayprocess();
        }
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : Open Subscrible prompt win under play
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscriblePromptWin(void)
{
    MBBMSSG_SERVICE_T service_item = {0};
    MMI_STRING_T      prompt_str = {0};
    MMI_STRING_T      strInfo    = {0};
    wchar wchar_dstbuf[150] = {0};
    wchar wchar_name[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    MMIMBBMS_PLAY_INFO* play_info_ptr=  PNULL;
    
    play_info_ptr = MMIMBBMS_GetPlayInfo();
    
    if (play_info_ptr->from_type == MMIMBBMS_PLAY_FROM_SERVICE)
    {
        MBBMSSG_GetService(play_info_ptr->uri, &service_item);
    }
    else
    {
        MBBMSSG_CONTENT_T content = {0};
        
        if(SG_ERROR_NONE==MBBMSSG_GetContent( play_info_ptr->uri, &content))
        {
            MBBMSSG_GetService(content.serviceIdRef, &service_item);
        }
        
        MBBMSSG_FreeContent(&content);
    }
    
    MMIMBBMS_GetNameWchar(service_item.NameList, wchar_name, 
        MMIMBBMS_SERVICE_NAME_MAX_LEN);
    MBBMSSG_FreeService(&service_item);
    
    MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRBILE_PROMPT1, &prompt_str);
    
    MMIAPICOM_Wstrncpy(wchar_dstbuf, prompt_str.wstr_ptr, prompt_str.wstr_len);
    MMIAPICOM_Wstrcat(wchar_dstbuf, wchar_name);
    
    strInfo.wstr_ptr = wchar_dstbuf;
    strInfo.wstr_len = prompt_str.wstr_len + MMIAPICOM_Wstrlen(wchar_name);
    
    MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRBILE_PROMPT2, &prompt_str);
    
    MMIAPICOM_Wstrncpy(wchar_dstbuf+strInfo.wstr_len, prompt_str.wstr_ptr, prompt_str.wstr_len);
    strInfo.wstr_len += prompt_str.wstr_len;
    
    MMIPUB_OpenQueryTextWinByTextPtr(
        &strInfo,
        PNULL,
        HandleSubscriblePromptWinMsg);
    
}

/*****************************************************************************/
// 	Description : get Subscrible prompt text info
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetSubscriblePromptText(
                                             wchar *str_ptr     //out
                                             )
{
    MBBMSSG_SERVICE_T service_item = {0};
    MMI_STRING_T      prompt_str = {0};
    MMI_STRING_T      strInfo    = {0};
    wchar wchar_dstbuf[150] = {0};
    wchar wchar_name[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    MMIMBBMS_PLAY_INFO* play_info_ptr=  PNULL;

    if(PNULL == str_ptr)
    {
        return;
    }
    
    play_info_ptr = MMIMBBMS_GetPlayInfo();
    
    if (play_info_ptr->from_type == MMIMBBMS_PLAY_FROM_SERVICE)
    {
        MBBMSSG_GetService(play_info_ptr->uri, &service_item);
    }
    else
    {
        MBBMSSG_CONTENT_T content = {0};
        
        if(SG_ERROR_NONE==MBBMSSG_GetContent( play_info_ptr->uri, &content))
        {
            MBBMSSG_GetService(content.serviceIdRef, &service_item);
        }
        
        MBBMSSG_FreeContent(&content);
    }
    
    MMIMBBMS_GetNameWchar(service_item.NameList, wchar_name, 
        MMIMBBMS_SERVICE_NAME_MAX_LEN);
    MBBMSSG_FreeService(&service_item);
    
    MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRBILE_PROMPT1, &prompt_str);
    
    MMIAPICOM_Wstrncpy(wchar_dstbuf, prompt_str.wstr_ptr, prompt_str.wstr_len);
    MMIAPICOM_Wstrcat(wchar_dstbuf, wchar_name);
    
    strInfo.wstr_ptr = wchar_dstbuf;
    strInfo.wstr_len = prompt_str.wstr_len + MMIAPICOM_Wstrlen(wchar_name);
    
    MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRBILE_PROMPT2, &prompt_str);
    
    MMIAPICOM_Wstrncpy(wchar_dstbuf+strInfo.wstr_len, prompt_str.wstr_ptr, prompt_str.wstr_len);
    strInfo.wstr_len += prompt_str.wstr_len;
    
    //copy
    MMIAPICOM_Wstrncpy(str_ptr, strInfo.wstr_ptr, strInfo.wstr_len);
}

/*****************************************************************************/
// 	Description : handle watch connecting win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubscriblePromptWinMsg( 
                                                MMI_WIN_ID_T     win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM              param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_COMMON_ENTER, TXT_NULL, STXT_CANCEL, TRUE);
            break;
        }
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            //MMIMBBMS_OpenSubscribeTypeMainWin();
            //s_is_enter_purchase_from_play = TRUE;
            MMIMBBMS_SetIfEnterSubFromPlay(TRUE);
            MMIMBBMS_OpenAccountInquiryRequestWin();
            MMK_CloseWin(win_id);
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);     
            break;
        }
    default:
        {
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
        }
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : handle setting save waiting win msg
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSavewaitingWinMsg( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint16 gateway_index = 0;
    uint16 notify_type=0;
    MMI_STRING_T sg_text = {0};
    MMI_STRING_T naf_text = {0};   
	static BOOLEAN s_is_saving = FALSE;
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	    s_is_saving = TRUE;
		MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
		break;
	case MSG_FULL_PAINT:
		if( TRUE == s_is_saving)
		{	
			MMK_PostMsg(win_id,MSG_MBBMS_SETTING_SAVE,NULL,NULL);
			s_is_saving = FALSE;
		}
		MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
		break;

	case MSG_MBBMS_SETTING_SAVE:
		notify_type = GUIDROPDOWNLIST_GetCurItemIndex(MMIMBBMS_DROPDOWNLIST_NOTIFY_TIME_CTRL_ID);
		gateway_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID);
		GUIEDIT_GetString(MMIMBBMS_EDITBOX_SG_DNS_CTRL_ID,&sg_text);
		MMIMBBMS_SaveSettingInfo(MMIMBBMS_GetCurSelectSim(), notify_type, gateway_index, sg_text.wstr_ptr, naf_text.wstr_ptr);
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MBBMS_SETTING_SAVE_OK,TXT_NULL,IMAGE_PUBWIN_SUCCESS, 
			PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);           
		MMK_CloseParentWin(MMIMBBMS_SETTING_WIN_ID);
		MMK_CloseWin(win_id);	
		break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}
/*****************************************************************************/
// 	Description : open setting save waiting win
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
LOCAL void OpenSettingSaveWaitingWin(void)
{
	MMI_WIN_ID_T  win_id = MMIMBBMS_SETTING_SAVE_WAIT_WIN_ID;
    uint32        waiting_time_out = 0;
    
    if(MMK_IsOpenWin(MMIMBBMS_SETTING_SAVE_WAIT_WIN_ID))
    {
        return;
    }

    MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_MBBMS_SETTING_SAVE_WAITING, TXT_NULL,
        IMAGE_PUBWIN_WARNING, &win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleSettingSavewaitingWinMsg);
}
/*****************************************************************************/
// 	Description : handle watch connecting win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingSavePromptWinMsg( 
                                                 MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
			MMK_CloseWin(win_id);			
			OpenSettingSaveWaitingWin();			
        }	
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMIMBBMS_SETTING_WIN_ID);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : handle watch connecting win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingCancelPromptWinMsg( 
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM              param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        MMK_CloseParentWin(MMIMBBMS_SETTING_WIN_ID);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);     
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}


LOCAL void MBBMS_AddAllServiceToDropdownList(void)
{
    uint32          list_num = 0;
    uint32          index =0;
    uint16          i = 0;
    SG_LIST_PTR_T   index_list_head_ptr = PNULL;
    SG_LIST_PTR_T   cursor_index_ptr = PNULL;
    SG_ERROR_E      error_code  = SG_ERROR_NONE;
    MBBMSSG_SERVICE_T serviceItem = {0};
    MMI_STRING_T*  peffect_item = PNULL;
    MBBMSSG_SERVICE_INDEX_T*  pServiceIndex = PNULL;
    
    error_code = MMIMBBMS_GetServiceIndex(&index_list_head_ptr);
    
    if(SG_ERROR_NONE != error_code)
    {
        peffect_item = (MMI_STRING_T*)SCI_ALLOC_APP(sizeof(MMI_STRING_T));
        
        if (PNULL == peffect_item)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: MBBMS_AddAllServiceToDropdownList  alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8212_112_2_18_2_40_4_479,(uint8*)"");
            return;
        }
        
        SCI_MEMSET(peffect_item, 0, (sizeof(MMI_STRING_T)));
        MMI_GetLabelTextByLang(TXT_COMMON_ALL, &peffect_item[0]);
        index = 1;
        
        GUIDROPDOWNLIST_AppendItemArray(MMIMBBMS_DROPDOWNLIST_CHANNEL_CTRL_ID, 
            peffect_item, (uint16)index);
        SCI_FREE(peffect_item);
        
        return;
    }
    
    list_num = index_list_head_ptr->node_cnt;
    //SCI_TRACE_LOW:"[MMIMTV]: CreateAllServiceList  list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8228_112_2_18_2_40_4_480,(uint8*)"d", list_num);
    if(list_num > 0)
    {
        peffect_item = (MMI_STRING_T*)SCI_ALLOC_APP(list_num*sizeof(MMI_STRING_T));
        
        if (PNULL == peffect_item)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: MBBMS_AddAllServiceToDropdownList  alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8235_112_2_18_2_40_4_481,(uint8*)"");
            return;
        }
        
        SCI_MEMSET(peffect_item, 0, (list_num*sizeof(MMI_STRING_T)));
        MMI_GetLabelTextByLang(TXT_COMMON_ALL, &peffect_item[0]);

        index = 1;
        cursor_index_ptr = index_list_head_ptr->next_ptr;
        while(--list_num && cursor_index_ptr != PNULL)
        {
            pServiceIndex = (MBBMSSG_SERVICE_INDEX_T*)(cursor_index_ptr->data);
            error_code = MBBMSSG_GetService(pServiceIndex->index, &serviceItem);
            if (error_code != SG_ERROR_NONE)
            {
                //SCI_TRACE_LOW:"MBBMSSG_GetService errcode = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8250_112_2_18_2_40_4_482,(uint8*)"d", error_code);
                return;
            }
            
            peffect_item[index].wstr_ptr = SCI_ALLOC_APP((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar));
            SCI_MEMSET(peffect_item[index].wstr_ptr, 0, ((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar)));
            
            MMIMBBMS_GetNameWchar(serviceItem.NameList,
                peffect_item[index].wstr_ptr, DROPDOWNLIST_STRING_MAX_NUM);
                /*GUI_UTF8ToWstr(peffect_item[index].wstr,
                MMIMBBMS_SERVICE_NAME_MAX_LEN, serviceItem.NameList->next_ptr->data,
            MMIMBBMS_SERVICE_NAME_MAX_LEN);*/

            peffect_item[index].wstr_len = MIN(GUILIST_STRING_MAX_NUM,MMIAPICOM_Wstrlen(peffect_item[index].wstr_ptr));
            
            //SCI_TRACE_LOW:"wstr_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8265_112_2_18_2_40_4_483,(uint8*)"d", peffect_item[index].wstr_len);
            
            index++;
            cursor_index_ptr = cursor_index_ptr->next_ptr;
            
            MBBMSSG_FreeService(&serviceItem);
            SCI_MEMSET(&serviceItem, 0, sizeof(serviceItem));
        }
        
        //SCI_TRACE_LOW:"index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8274_112_2_18_2_40_4_484,(uint8*)"d", index);
        
        GUIDROPDOWNLIST_AppendItemArray(MMIMBBMS_DROPDOWNLIST_CHANNEL_CTRL_ID, 
            peffect_item, (uint16)index);
        
        for(i = 1; i < index; i++) // 因为index=0的时候取的是字符串资源的地址，不需要释放
        {
            if(PNULL != peffect_item[i].wstr_ptr)
            {
                SCI_FREE(peffect_item[i].wstr_ptr);
            }
        }
        
        SCI_FREE(peffect_item);
    }
    //MBBMSSG_FreeServiceIndex(&index_list_head_ptr);
}


LOCAL void MBBMS_AddAllDateToDropdownList(void)
{
    uint32 index = 0;
    SCI_DATE_T   date = {0};
    MMI_STRING_T  item_date= {0};
    wchar         str_arry[GUILIST_STRING_MAX_NUM + 1] = {0};
    
    GUIDROPDOWNLIST_SetMaxItem(MMIMBBMS_DROPDOWNLIST_DATE_CTRL_ID, M_MMI_MBBMS_MAX_DAY_NUM + 1);

    GUIDROPDOWNLIST_AppendItemById(MMIMBBMS_DROPDOWNLIST_DATE_CTRL_ID, TXT_COMMON_ALL);
    for (index = 0; index < M_MMI_MBBMS_MAX_DAY_NUM; index++)
    {
        MMIMBBMS_GetSelectedDate(&date, (uint16)index);
        SCI_MEMSET(str_arry, 0, sizeof(str_arry));
        item_date.wstr_ptr = str_arry;
        MMIMBBMS_MergeDateString(&date, item_date.wstr_ptr);
        item_date.wstr_len = MIN(GUILIST_STRING_MAX_NUM,MMIAPICOM_Wstrlen(item_date.wstr_ptr));
        GUIDROPDOWNLIST_AppendItem(MMIMBBMS_DROPDOWNLIST_DATE_CTRL_ID, &item_date);
    }

    return;
}

LOCAL void MMIMBBMS_MergeDateString(SCI_DATE_T* pdate, wchar* pwchar)
{
    char  str_buf[5] = {0};
    wchar w_year[5] = {0};
    wchar w_mon[3] = {0};
    wchar w_day[3] = {0};
    MMI_STRING_T text_string = {0};
    
    SCI_MEMSET(str_buf, 0, sizeof(str_buf));
    MMIAPICOM_Int2Str(pdate->year, (uint8*)str_buf, 4);
    MMIAPICOM_StrToWstr((uint8*)str_buf, w_year);
    str_buf[4] = '\0';
    
    SCI_MEMSET(str_buf, 0, sizeof(str_buf));
    MMIAPICOM_Int2Str(pdate->mon, (uint8*)str_buf, 2);
    str_buf[2] = '\0';
    MMIAPICOM_StrToWstr((uint8*)str_buf, w_mon);
    
    if (0 == w_mon[1])
    {
        w_mon[1] = w_mon[0];
        w_mon[0] = (wchar)'0';
        w_mon[2] = 0;
    }
    
    SCI_MEMSET(str_buf, 0, sizeof(str_buf));
    MMIAPICOM_Int2Str(pdate->mday, (uint8*)str_buf, 2);
    str_buf[2] = '\0';
    MMIAPICOM_StrToWstr((uint8*)str_buf, w_day);
    if (0 == w_day[1])
    {
        w_day[1] = w_day[0];
        w_day[0] = (wchar)'0';
        w_day[2] = 0;
    }
    
    MMI_GetLabelTextByLang( TXT_SET_DATE_YMD, &text_string);
    MMIAPICOM_Wstrncpy(pwchar, w_year, 4);
    
    if (text_string.wstr_ptr[0] == (wchar)'y'
        || text_string.wstr_ptr[0] == (wchar)'Y')
    {
        pwchar[4] = (wchar)'-';
        MMIAPICOM_Wstrncpy(pwchar+5, w_mon, 2);
        pwchar[7] = (wchar)'-';
        MMIAPICOM_Wstrncpy(pwchar+8, w_day, 2);
        pwchar[10] = (wchar)0;
    }
    else
    {
        MMIAPICOM_Wstrncpy(pwchar+4, text_string.wstr_ptr, 1);
        MMIAPICOM_Wstrncpy(pwchar+5, w_mon, 2);
        MMIAPICOM_Wstrncpy(pwchar+7, text_string.wstr_ptr+2, 1);
        MMIAPICOM_Wstrncpy(pwchar+8, w_day, 2);
        MMIAPICOM_Wstrncpy(pwchar+10, text_string.wstr_ptr+4, 2);
    }
    
}


LOCAL void MBBMS_AddAllContentTypeToDropdownList(void)
{
    uint32 index = 0;
    MMI_STRING_T  item_date[M_MMI_MBBMS_MAX_CONTENT_TYPE_NUM + 1] = {0};
    MMI_GetLabelTextByLang(TXT_COMMON_ALL, &item_date[0]);
    for (index = 0; index < M_MMI_MBBMS_MAX_CONTENT_TYPE_NUM; index++)
    {
        MMI_GetLabelTextByLang(s_content_type[index], &item_date[index+1]);
    }
    
    // add content type to dropdown list
    GUIDROPDOWNLIST_AppendItemArray(MMIMBBMS_DROPDOWNLIST_PROGRAM_TYPE_CTRL_ID, 
        item_date, M_MMI_MBBMS_MAX_CONTENT_TYPE_NUM + 1);
    
    return;
}

/*****************************************************************************/
// Description : Load all content type to dropdown list ctrl
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetStartEndTimeString(MBBMSSG_CONTENT_T* pContent, wchar* pwchar)
{
    char str_date[10] = {0};
    wchar   wchar_starttime[5] = {0};
    wchar   wchar_endtime[5] = {0};
    
    sprintf(str_date, "%2d:%2d",
        pContent->StartTime.hours,
        pContent->StartTime.minutes);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_starttime);
    
    sprintf(str_date, "%2d:%2d",
        pContent->EndTime.hours, 
        pContent->EndTime.minutes);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_endtime);
    
    MMIAPICOM_Wstrncpy(pwchar, wchar_starttime,  5);
    pwchar[5] = (wchar)'-';
    MMIAPICOM_Wstrncpy(pwchar+6, wchar_endtime, 5);
    pwchar[11] = '\0';
    
    return;
}

/*****************************************************************************/
// Description : get start/end time info from content index struct
// Global resource dependence : none
// Author: 
// Note: 从节目分片结构中获取start/end信息
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetStartEndTimeStringFromContentIndex(MBBMSSG_CONTENT_INDEX_T* pContentIndex, wchar* pwchar)
{
    char str_date[10] = {0};
    wchar   wchar_starttime[5] = {0};
    wchar   wchar_endtime[5] = {0};

    if(PNULL == pContentIndex || PNULL == pwchar)
    {
        return;
    }
    
    sprintf(str_date, "%2d:%2d",
        pContentIndex->startTime.hours,
        pContentIndex->startTime.minutes);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_starttime);
    
    sprintf(str_date, "%2d:%2d",
        pContentIndex->endTime.hours, 
        pContentIndex->endTime.minutes);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_endtime);
    
    MMIAPICOM_Wstrncpy(pwchar, wchar_starttime,  5);
    pwchar[5] = (wchar)'-';
    MMIAPICOM_Wstrncpy(pwchar+6, wchar_endtime, 5);
    pwchar[11] = '\0';
    
    return;
}

/*****************************************************************************/
// Description : MMIMBBMS_ContentNotifyCallback
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ContentNotifyCallback(void)
{
    SCI_TM_T systime  = {0};
    MMI_TM_T realtime = {0};
    uint32 time_period = 0;
    uint16 remindertime = 0;
    anyURI contentID = {0};
    MMI_STRING_T prompt_str = {0};
    MBBMSSG_CONTENT_T sg_content = {0};
    wchar wchar_buf[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    MMI_STRING_T   strInfo = {0};
    uint16 content_len = 0;
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = PNULL; 
    MMIMBBMS_TIME_REMAINDER_E time_remainder = MMIMBBMS_REMAINDER_MAX;
    const MMIMBBMS_SETTING_INFO_T* setting_info_ptr = PNULL;
    
    setting_info_ptr = MMIMBBMS_GetConstSettingInfo();
    
    //SCI_TRACE_LOW:"MMIMBBMS_ContentNotifyCallback: "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8520_112_2_18_2_40_5_485,(uint8*)"");
    
    if (POWER_GetRestartCondition() == RESTART_BY_ALARM)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_ContentNotifyCallback: RESTART_BY_ALARM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8524_112_2_18_2_40_5_486,(uint8*)"");
        return FALSE;
    }
    
    if (MMIMBBMS_IsMBBMSOpen() == FALSE)
    {
        return FALSE;
    }
    
    cursor = MMIMBBMS_GetAlarmContentNodeViaIndex(0);
    
    time_remainder = setting_info_ptr->time_remainder;
    
    if (time_remainder == MMIMBBMS_REMAINDER_OFF
        || time_remainder >= MMIMBBMS_REMAINDER_MAX)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_ContentNotifyCallback: time_remainder is error = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8541_112_2_18_2_40_5_487,(uint8*)"d",time_remainder);
        return FALSE;
    }
    
    /* 得到节目提醒时间类型 */
    remindertime = MMIMBBMS_GetRemindTime(time_remainder);
    
    TM_GetTime(&systime);
    
    MMIMBBMS_GetRealStartTime(&systime, remindertime, &realtime);
    
    //SCI_TRACE_LOW:"MMIMBBMS_ContentNotifyCallback: remindertime = %d systime.tm_min = %d  realtime.tm_min = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8553_112_2_18_2_40_5_488,(uint8*)"ddd",remindertime, systime.tm_min, realtime.tm_min);
    
    // 循环读取所有设置了节目提醒的节目
    while (cursor != PNULL)
    {
        /* 这里只需要判断时，分 */
        if (cursor->starttime.hours == realtime.tm_hour
            && cursor->starttime.minutes == realtime.tm_min)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_ContentNotifyCallback: match content id = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8563_112_2_18_2_40_5_489,(uint8*)"s",cursor->globalContentID.anyURI_arr);
            
            MBBMSSG_GetContentId(cursor->globalContentID, &contentID);
            MBBMSSG_GetContent(contentID, &sg_content);
            
            if (sg_content.NameList == PNULL
                || sg_content.NameList->next_ptr == PNULL
                || sg_content.NameList->next_ptr->data == PNULL)
            {
                cursor = cursor->content_next;
                MBBMSSG_FreeTypeAnyURI(&contentID);
                MBBMSSG_FreeContent(&sg_content);
                continue;
            }
            
            SCI_MEMSET(wchar_buf, 0, sizeof (wchar_buf));
            MMIMBBMS_GetNameWchar(sg_content.NameList, wchar_buf,
                MMIMBBMS_SERVICE_NAME_MAX_LEN);
            MBBMSSG_FreeContent(&sg_content);
            
            strInfo.wstr_ptr = wchar_buf;
            content_len = MMIAPICOM_Wstrlen(wchar_buf);
            
            if (time_remainder == MMIMBBMS_REMAINDER_TWO_MIN)
            {
                MMI_GetLabelTextByLang(TXT_MBBMS_CONTENT_NOTIFY_INFO_2, &prompt_str);
            }
            else if (time_remainder == MMIMBBMS_REMAINDER_FIVE_MIN)
            {
                MMI_GetLabelTextByLang(TXT_MBBMS_CONTENT_NOTIFY_INFO_5, &prompt_str);
            }
            else if (time_remainder == MMIMBBMS_REMAINDER_TEN_MIN)
            {
                MMI_GetLabelTextByLang(TXT_MBBMS_CONTENT_NOTIFY_INFO_10, &prompt_str);
            }
            MMIAPICOM_Wstrcat(wchar_buf, prompt_str.wstr_ptr);
            
            strInfo.wstr_len = prompt_str.wstr_len + content_len;
            
            /* 打开提示窗口 */
            MMIPUB_OpenAlertTextWinByTextPtr(&time_period,&strInfo, 
                PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,PNULL);
            
            /* 提醒后取消alarm */
            MMIALMSVC_UnRegService(cursor->event_id, TRUE);
            
            cursor->event_id = 0;
            
            MBBMSSG_FreeTypeAnyURI(&contentID);
        }
        
        cursor = cursor->content_next;
    }
    
    return TRUE;   
}

/*****************************************************************************/
//  Description : get selected date
//  Global resource dependence :
//  Author:plum.peng
//  Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_GetRealStartTime(SCI_TM_T* pSrcTime, uint16 remindertime,
                                     MMI_TM_T* pDstTime)
{
    uint32     uSeconds = 0; 
    
    uSeconds =  MMIAPICOM_Tm2Second(pSrcTime->tm_sec,
        pSrcTime->tm_min,
        pSrcTime->tm_hour,
        pSrcTime->tm_mday,
        pSrcTime->tm_mon,
        pSrcTime->tm_year);
    
    uSeconds += 60 * remindertime;
    
    *pDstTime = MMIAPICOM_Second2Tm(uSeconds);
    
    return;
    
}

/*****************************************************************************/
// Description : 套餐详情头信息
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddPurchaseDetailHead(wchar* wchar_name, MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    MMI_STRING_T temp = {0};
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.str_ptr = wchar_name;
    item_data.text_data.buf.len = MMIAPICOM_Wstrlen(wchar_name);
    
    GUIRICHTEXT_AddItem(ctrl_id, 
        &item_data, &index);
    
    //should change to purchase item name @plum
    MMI_GetLabelTextByLang(TXT_MBBMS_PURCHASEITEM_INCLUDE_SERVICES, &temp);
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_data.buf.str_ptr = temp.wstr_ptr;
    item_data.text_data.buf.len = temp.wstr_len;
    
    GUIRICHTEXT_AddItem(ctrl_id, 
        &item_data, &index);
    
    return;
}

/*****************************************************************************/
// Description : 套餐详情尾信息
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddPurchaseDetailTail(
                                           MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T* purchase_display_info,
                                           MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    uint16 lens = 0;
    MMI_STRING_T mbbms_string_time = {0};
    MMI_STRING_T mbbms_string_unit = {0};
    MMI_STRING_T mbbms_string_unit2 = {0};
    
    MMI_STRING_T mbbms_string_cost = {0};
    MMI_STRING_T string_price = {0};
    
    MMI_STRING_T mbbms_string_1 = {0};
    MMI_STRING_T mbbms_string_2 = {0};
    MMI_STRING_T mbbms_string_temp2 = {0};
    
    wchar temp_wchar1[GUIRICHTEXT_TEXT_MAX_LEN] = {0};
    wchar temp_wchar21[GUILIST_STRING_MAX_NUM] = {0};
    wchar temp_wchar22[GUILIST_STRING_MAX_NUM] = {0};
    
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    char str_price[MMIMBBMS_MAX_PRICE_LENS] = {0};
    wchar wchar_price[MMIMBBMS_MAX_PRICE_LENS] = {0};
    char str_period[MMIMBBMS_MAX_PRICE_LENS] = {0};
    wchar wchar_period[MMIMBBMS_MAX_PRICE_LENS] = {0};   
    
    wchar wchar_period_for_price_dis[MMIMBBMS_MAX_PRICE_LENS] = {0}; 
    MMI_STRING_T mbbms_string_period_month = {0};
    
    //SCI_TRACE_LOW:"MMIMBBMS_AddPurchaseDetailTail: scription_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8710_112_2_18_2_40_5_490,(uint8*)"d",purchase_display_info->scription_type);
    
    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    
    //should change to purchase item name @plum
    mbbms_string_1.wstr_ptr = temp_wchar1;
    SCI_MEMSET(temp_wchar1, 0, sizeof(temp_wchar1));
    
    //订购时间
    MMI_GetLabelTextByLang(TXT_MBBMS_SUB_TIME, &mbbms_string_time);
    
    if (purchase_display_info->scription_type == MBBMS_PURCHASE_DATA_SUBSCRIPTION_CONTENT)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_TIMES, &mbbms_string_unit);
        MMIAPICOM_MergeTwoString(&mbbms_string_time, &mbbms_string_unit, &mbbms_string_1, GUILIST_STRING_MAX_NUM);
    }
    else if(MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM_MULTI == purchase_display_info->scription_type)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddPurchaseDetailTail.....purchase_display_info->period = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8729_112_2_18_2_40_5_491,(uint8*)"d", purchase_display_info->period);
        memset(str_period, 0, sizeof(str_period));
        if(0 != purchase_display_info->period)
        {
            sprintf(str_period, "%d", purchase_display_info->period);
            MMIAPICOM_StrToWstr((uint8*)str_period, wchar_period);
            MMI_GetLabelTextByLang(TXT_MBBMS_MONTH, &mbbms_string_period_month);
            MMIAPICOM_Wstrncat(wchar_period, mbbms_string_period_month.wstr_ptr, mbbms_string_period_month.wstr_len);
            mbbms_string_unit.wstr_ptr = wchar_period;
            mbbms_string_unit.wstr_len = MMIAPICOM_Wstrlen(wchar_period);
            MMIAPICOM_MergeTwoString(&mbbms_string_time, &mbbms_string_unit, &mbbms_string_1, GUILIST_STRING_MAX_NUM);
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_MONTHS, &mbbms_string_unit);
        MMIAPICOM_MergeTwoString(&mbbms_string_time, &mbbms_string_unit, &mbbms_string_1, GUILIST_STRING_MAX_NUM);
    }
    
    item_data.text_data.buf.str_ptr = mbbms_string_1.wstr_ptr;
    mbbms_string_1.wstr_len = MMIAPICOM_Wstrlen(temp_wchar1);
    
    item_data.text_data.buf.len = mbbms_string_1.wstr_len;
    
    GUIRICHTEXT_AddItem(ctrl_id, 
        &item_data, &index);
    
    //资费
    MMI_GetLabelTextByLang(TXT_MBBMS_PURCHASEITEM_COST, &mbbms_string_cost);
    
    sprintf(str_price, "%f", purchase_display_info->price);
    lens = strlen(str_price);
    
    while (lens > 0 && str_price[--lens] == '0')
    {
        ;
    }
    
    if (str_price[lens] == '.')
    {
        str_price[lens+2] = '\0';
    }
    else
    {
        str_price[lens+1] = '\0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_price, wchar_price);
    
    string_price.wstr_ptr = wchar_price;
    string_price.wstr_len = MMIAPICOM_Wstrlen(wchar_price);
    
    mbbms_string_temp2.wstr_ptr = temp_wchar21;
    MMIAPICOM_MergeTwoString(&mbbms_string_cost, &string_price, &mbbms_string_temp2, GUILIST_STRING_MAX_NUM);
    
    mbbms_string_2.wstr_ptr = temp_wchar22;
    
    //SCI_TRACE_LOW:"MMIMBBMS_AddPurchaseDetailTail: price scription_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8786_112_2_18_2_40_6_492,(uint8*)"d",purchase_display_info->scription_type);
    if (purchase_display_info->scription_type == MBBMS_PURCHASE_DATA_SUBSCRIPTION_CONTENT)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_UNIT_PER_TIMES, &mbbms_string_unit2);
        MMIAPICOM_MergeTwoString(&mbbms_string_temp2, &mbbms_string_unit2, 
            &mbbms_string_2, GUILIST_STRING_MAX_NUM);
    }
    else if(MBBMS_PURCHASE_DATA_SUBSCRIPTION_ITEM_MULTI == purchase_display_info->scription_type)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_YUAN, &mbbms_string_unit2);
        MMIAPICOM_MergeTwoString(&mbbms_string_temp2, &mbbms_string_unit2, 
            &mbbms_string_2, GUILIST_STRING_MAX_NUM);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_UNIT_PER_MONTH, &mbbms_string_unit2);
        MMIAPICOM_MergeTwoString(&mbbms_string_temp2, &mbbms_string_unit2, 
            &mbbms_string_2, GUILIST_STRING_MAX_NUM);
    }
    
    item_data.text_data.buf.str_ptr = mbbms_string_2.wstr_ptr;
    item_data.text_data.buf.len = mbbms_string_2.wstr_len;
    
    GUIRICHTEXT_AddItem(ctrl_id, 
        &item_data, &index);
    
    return;
}

/*****************************************************************************/
// Description : whether current service in all_service_list is encrypt
// Global resource dependence : none
// Author: xin.li
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCurrServiceInAllTabEncrypted(uint16 index)
{
    BOOLEAN is_encrypted = FALSE;
    MTVSERVICE_PROGRAM_LIST_T prog_list_t = {0};
    MTVSERVICE_PROGRAM_T      *prog_arry_ptr = PNULL;
    
    MMIMBBMS_GetProgramList(&prog_list_t);
    prog_arry_ptr = prog_list_t.list_ptr;
    
    if(index < prog_list_t.total_count 
        && PNULL != prog_arry_ptr)
    {
        is_encrypted = prog_arry_ptr[index].is_encryped;
        //SCI_TRACE_LOW:"MMIMBBMS_IsCurrServiceInAllTabEncrypted: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8832_112_2_18_2_40_6_493,(uint8*)"d",is_encrypted);
    }
    else
    {
        is_encrypted = FALSE;
        //SCI_TRACE_LOW:"MMIMBBMS_IsCurrServiceInAllTabEncrypted error!  index=%  total_count=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8837_112_2_18_2_40_6_494,(uint8*)"d", index, prog_list_t.total_count);
    }
    
    return is_encrypted;
}

/*****************************************************************************/
// Description : MMIMBBMS_AddPurchaseDetailHead
// Global resource dependence : none
// Author: lisa lin
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCurrServiceInFavoriteTabEncrypted(void)
{
    uint16 index=0;
    MMIMBBMS_SAVE_SERVICE_INFO_T*  favorite_service_ptr=PNULL;
    anyURI serviceId={0};
    SG_ERROR_E err_code=SG_ERROR_NONE;
    BOOLEAN is_encrypted=FALSE;
    
    index = GUILIST_GetCurItemIndex(MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID);
    favorite_service_ptr=MMIMBBMS_GetFavoriteServiceNodeViaIndex(index);
    if (favorite_service_ptr == PNULL)
    {
        return is_encrypted;
    }
    
    SCI_MEMSET(&serviceId,0,sizeof(anyURI));
    //change global id to id
    err_code= MBBMSSG_GetServiceId(favorite_service_ptr->globalServiceID, &serviceId);	 
    
    if(SG_ERROR_NONE==err_code)
    {
        is_encrypted=MMIMBBMS_IsServiceEncrypted(serviceId);
    }
    MBBMSSG_FreeTypeAnyURI(&serviceId);
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsCurrServiceInFavoriteTabEncrypted:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8872_112_2_18_2_40_6_495,(uint8*)"d",is_encrypted);
    
    return is_encrypted;
    
}

/*****************************************************************************/
// Description : MMIMBBMS_AddPurchaseDetailHead
// Global resource dependence : none
// Author: lisa lin
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsCurrContentInFavoriteTabEncrypted(void)
{
    
    uint16 index =0;
    MMIMBBMS_SAVE_CONTENT_INFO_T*  favorite_content_ptr=PNULL;
    anyURI contentId = {0};
    SG_ERROR_E err_code=SG_ERROR_NONE;
    BOOLEAN is_encrypted=FALSE;
    
    index = GUILIST_GetCurItemIndex(MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID);
    favorite_content_ptr=(MMIMBBMS_SAVE_CONTENT_INFO_T*)MMIMBBMS_GetAlarmContentNodeViaIndex(index);
    
    if (favorite_content_ptr == PNULL)
    {
        return is_encrypted;
    }
    //change global id to id
    SCI_MEMSET(&contentId,0,sizeof(anyURI));
    err_code= MBBMSSG_GetContentId(favorite_content_ptr->globalContentID, &contentId);
    
    
    if(SG_ERROR_NONE==err_code)
    {
        is_encrypted=MMIMBBMS_IsContentEncrypted(contentId);
    }
    
    MBBMSSG_FreeTypeAnyURI(&contentId);
    
    //SCI_TRACE_LOW:"MMIMBBMS_IsCurrContentInFavoriteTabEncrypted:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8910_112_2_18_2_40_6_496,(uint8*)"d",is_encrypted);
    
    return is_encrypted;
    
}

/*****************************************************************************/
// 	Description : add a item to eb list
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void AddEBItemToListboxCtrl(
                                  wchar *content0, 
                                  wchar *content1, 
                                  BOOLEAN is_read,
                                  uint32	eb_grade
                                  )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint32 len = MMIAPICOM_Wstrlen(content1);
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON;
    item_t.item_data_ptr = &item_data;   
    
	//title icon
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    if(is_read)
    {
        item_data.item_content[0].item_data.image_id = IMAGE_SMS_READ_SMS_ICON;
    }
    else
    {
        item_data.item_content[0].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_ICON;
    }

	//eb grade icon
	item_data.item_content[2].item_data_type	 = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[2].item_data.image_id = s_eb_grade_icon[eb_grade - 1];

	//eb info
	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    if(len > GUILIST_STRING_MAX_NUM)
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = GUILIST_STRING_MAX_NUM;
    }  
    else
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = len;
    }
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = content1;
	
	//time info
    item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(content0);
	item_data.item_content[3].item_data.text_buffer.wstr_ptr = content0;
	
    GUILIST_AppendItem(MMIMBBMS_EB_LIST_CTRL_ID, &item_t);   
}

/*****************************************************************************/
// 	Description : create eb list
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void CreateEBList(void)
{
    wchar wline1[MMIMBBMS_COMMON_MAX_LENS] = {0};
    wchar wline2[MMIMBBMS_COMMON_MAX_LENS] = {0};
    char  cline1[MMIMBBMS_COMMON_MAX_LENS] = {0};
    uint32 list_num = MMIMTV_GetEBListNumber();
    uint32 index = 0;
    MMIMTV_EB_NODE_T* cursor = PNULL;
    MMI_TM_T time = {0};
    MMIACC_RTC_DATE_T rtc_date = {0};
    MMIACC_RTC_TIME_T rtc_time = {0};
    
    //SCI_TRACE_LOW:"[MMIMBBMS]: CreateEBList eb_list_number = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_8985_112_2_18_2_40_6_497,(uint8*)"d", list_num);
    GUILIST_SetMaxItem(MMIMBBMS_EB_LIST_CTRL_ID, list_num, FALSE);
    
    //loop add eb item
    while (list_num --)
    {
        SCI_MEMSET(wline1, 0, sizeof(wline1));
        SCI_MEMSET(wline2, 0, sizeof(wline2));
        SCI_MEMSET(cline1, 0, sizeof(cline1));
        
        cursor = MMIMTV_GetEBNodeViaIndex(index);
        if(PNULL != cursor)
        {
            time = MMIAPICOM_Second2Tm(cursor->eb_save_time);
            sprintf((char*)rtc_time, "%02d:%02d", time.tm_hour, time.tm_min);
            sprintf((char*)rtc_date, "%02d/%02d", time.tm_mon, time.tm_mday);
            sprintf(cline1, "%s  %s", (char*)rtc_date, (char*)rtc_time);
            MMIAPICOM_StrToWstr((uint8*)cline1, wline1);
            MMIAPICOM_Wstrcpy(wline2, cursor->eb_type);
            MMIAPICOM_Wstrcpy(wline2 + MMIAPICOM_Wstrlen(cursor->eb_type), wline1);
            
            AddEBItemToListboxCtrl(wline2, cursor->eb_content, cursor->eb_is_read, cursor->eb_grade);
        }
        
        index ++;
    }
}

/*****************************************************************************/
// 	Description : open emergency broadcast msg list win
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenEBListWin(void)
{
    if(MMK_IsOpenWin(MMIMBBMS_EB_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_EB_LIST_WIN_ID);
    }
    MMK_CreateWin((uint32 *)MMIMBBMS_EB_LIST_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description : handle eb list win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEBListWinMsg( 
                                      MMI_WIN_ID_T     win_id, 
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM              param
                                      )
{
    MMI_RESULT_E     recode = MMI_RESULT_TRUE;	
    MMI_CTRL_ID_T    ctrl_id = MMIMBBMS_EB_LIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        CreateEBList();
        if(GUILIST_GetTotalItemNum(ctrl_id) == 0)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;
        
    case MSG_GET_FOCUS:
    case MSG_APP_MBBMS_REFRESH_LIST:
        GUILIST_RemoveAllItems(ctrl_id);
        CreateEBList();
        if(GUILIST_GetTotalItemNum(ctrl_id) == 0)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            MMK_CreateWin((uint32 *)MMIMBBMS_EB_DETAIL_WIN_TAB, PNULL); 
        }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            MMK_CreateWin((uint32 *)MMIMBBMS_EB_LIST_POPMENU_WIN_TAB, PNULL); 
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIMTV_WriteEBFile();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : handle delete eb query win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelEBQuerytWinMsg( 
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint16       index = 0;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        index = GUILIST_GetCurItemIndex(MMIMBBMS_EB_LIST_CTRL_ID);
        MMIMTV_DeleteEBNodeViaIndex(index);
        if(!MMIMTV_IsHaveTobeReadEBMsg())
        {
            //清除EB提示图标
            MAIN_SetMsgState(TRUE);
        }
        MMK_PostMsg(MMIMBBMS_EB_LIST_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, PNULL, NULL);
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
    
    return result;
}

/*****************************************************************************/
// 	Description : handle delete eb query win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllEBQuerytWinMsg( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIMTV_ClearEBList();
        MAIN_SetMsgState(TRUE);
        MMK_PostMsg(MMIMBBMS_EB_LIST_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, PNULL, NULL);
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
    
    return result;
}

/*****************************************************************************/
// 	Description : handle eb list opt win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEBListOptWinMsg( 
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_EB_LIST_CTRL_ID;
    //uint16          index = 0;
    uint16          i = 0;
    uint16          node_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);
        
        InsertMenuNode(ctrl_id, TXT_MTV_SORT_BY_TIME,   MMIMBBMS_ID_EB_SORT_BY_TIME_MENU, 0, i++);
        InsertMenuNode(ctrl_id, TXT_MTV_SORT_BY_LEVEL,  MMIMBBMS_ID_EB_SORT_BY_GRADE_MENU, 0, i++);
        InsertMenuNode(ctrl_id, TXT_DELETE,            MMIMBBMS_ID_EB_DELETE_MENU, 0, i++);
        InsertMenuNode(ctrl_id, TXT_DELALL,             MMIMBBMS_ID_EB_DELETE_ALL_MENU, 0, i++);
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch(node_id)
        {
        case MMIMBBMS_ID_EB_SORT_BY_TIME_MENU:
            MMIMTV_EBSortByTime();
            MMK_PostMsg(MMIMBBMS_EB_LIST_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, PNULL, NULL);
            break;
            
        case MMIMBBMS_ID_EB_SORT_BY_GRADE_MENU:
            MMIMTV_EBSortByGrade();
            MMK_PostMsg(MMIMBBMS_EB_LIST_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, PNULL, NULL);
            break;
            
        case MMIMBBMS_ID_EB_DELETE_MENU:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY, 
                IMAGE_PUBWIN_QUERY,
                PNULL,
                HandleDelEBQuerytWinMsg
                );
            break;
            
        case MMIMBBMS_ID_EB_DELETE_ALL_MENU:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_ALL_QUERY, 
                IMAGE_PUBWIN_QUERY,
                PNULL,
                HandleDelAllEBQuerytWinMsg
                );
            break;
            
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : handle eb detail win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEBDetailWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    uint16              index = 0;
    uint16              richtext_index = 0;
    MMI_CTRL_ID_T       ctrl_id = MMIMBBMS_EB_DETAIL_RICHTEXT_CTRL_ID;
    MMIMTV_EB_NODE_T    *node_ptr = PNULL;
    GUIRICHTEXT_ITEM_T  richtext_item = {0};
    char                str_time_info[MMIMTV_MAX_NAME_LEN +1] = {0};
    wchar               content_info[MMIMTV_EB_CONTENT_MAX +1] = {0};
    wchar               wstr_enter[MMIMTV_MAX_NAME_LEN] = {0};
    MMI_TM_T            time = {0};
    MMIACC_RTC_DATE_T   rtc_date = {0};
    MMIACC_RTC_TIME_T   rtc_time = {0};
    uint16              min_len = 0;
    uint16              valid_len = 0;
    uint16              eb_content_len = 0;

    wstr_enter[0] = 0x000D;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        
        if(MMK_IsOpenWin(MMIMBBMS_EB_LIST_WIN_ID))
        {
            index = GUILIST_GetCurItemIndex(MMIMBBMS_EB_LIST_CTRL_ID);
        }
        else
        {
            index = 0;
        }
        
        node_ptr = MMIMTV_GetEBNodeViaIndex(index);
        if(PNULL != node_ptr)
        {
            //time info
            time = MMIAPICOM_Second2Tm(node_ptr->eb_save_time);
            sprintf((char*)rtc_time, "%02d:%02d", time.tm_hour, time.tm_min);
            sprintf((char*)rtc_date, "%02d/%02d", time.tm_mon, time.tm_mday);
            sprintf(str_time_info, "%s  %s", (char*)rtc_date, (char*)rtc_time);
            MMIAPICOM_StrToWstr((uint8*)str_time_info, content_info);
            //insert "enter"
            MMIAPICOM_Wstrcpy(content_info + MMIAPICOM_Wstrlen(content_info), wstr_enter);
            //cpy eb content
            valid_len       = MMIMTV_EB_CONTENT_MAX - MMIAPICOM_Wstrlen(content_info);
            eb_content_len  = MMIAPICOM_Wstrlen(node_ptr->eb_content);
            min_len         = MIN(valid_len, eb_content_len);
            MMIAPICOM_Wstrncpy(content_info + MMIAPICOM_Wstrlen(content_info), node_ptr->eb_content, min_len);
            
            //append eb detail
            richtext_item.img_type = GUIRICHTEXT_IMAGE_RES;
            richtext_item.img_data.res.id = s_eb_grade_icon[node_ptr->eb_grade - 1];
            richtext_item.img_frame_type  = GUIRICHTEXT_FRAME_NONE;
            
            richtext_item.text_type = GUIRICHTEXT_TEXT_BUF;
            richtext_item.text_data.buf.str_ptr = content_info;
            richtext_item.text_data.buf.len		= MMIAPICOM_Wstrlen(richtext_item.text_data.buf.str_ptr);
            
            richtext_item.align_type = GUIRICHTEXT_ALIGN_DOWN;
            GUIRICHTEXT_SetImgTxtSpace(ctrl_id, 6);
            GUIRICHTEXT_SetLineSpace(ctrl_id, 2);
            GUIRICHTEXT_AddItem(ctrl_id, &richtext_item, &richtext_index);
            
            //本条EB消息已读
            node_ptr->eb_is_read = TRUE;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : handle help info win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelpInfoWinMsg( 
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T text={0};
            
            MMI_GetLabelTextByLang(TXT_MBBMS_HELP_INFO,&text);
            //GUITEXTBOX_EnableScrollbar(MMIMBBMS_HELP_INFO_TEXTBOX_CTRL_ID, TRUE);
            GUITEXT_SetString(MMIMBBMS_HELP_INFO_TEXTBOX_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
            MMK_SetAtvCtrl(win_id, MMIMBBMS_HELP_INFO_TEXTBOX_CTRL_ID);
        }
        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : handle sim card select win msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimSelectWinMsg( 
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    uint16              cur_selection = 0;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_MAX;
    MMI_CTRL_ID_T       ctrl_id = MMIMBBMS_SIM_SELECT_LIST_CTRL_ID;
    uint32              item_data = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_FULL_PAINT:
        {
            uint16 cur_selection = 0;
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_SetMaxItem(ctrl_id, (uint16)MMI_DUAL_SYS_MAX, FALSE);            
            MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, (uint32)MMI_DUAL_SYS_MAX,MMISET_APPEND_SYS_OK);
            GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        }
        break;        
          
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        if(GUILIST_IsCurrItemInvalid(ctrl_id))
        {
            break;
        }
        
        GUILIST_GetItemData(ctrl_id, cur_selection, &item_data);
        
        if(item_data < MMI_DUAL_SYS_MAX)
        {
            //保存SIM卡选择
            dual_sys = (MN_DUAL_SYS_E)item_data;
            MMIMBBMS_SetCurSelectSim(dual_sys);
            
            //设置业务模式，继续启动流程
            MMIMBBMS_SetServiceMode(MBBMS_SERVICE_MODE_MBBMS);
            
            MMIMBBMS_SetStartUpWinCloseFlag(TRUE);
            MMIMBBMS_OpenStartUpWin();
        }
        else
        {
            //SCI_TRACE_LOW:"HandleSimSelectWinMsg, item_data=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9453_112_2_18_2_40_7_498,(uint8*)"d", item_data);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : 打开选卡界面
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSimSelectWin(void)
{
    MMK_CreateWin((uint32*)MMIMBBMS_SELECT_SIM_WIN_TAB, PNULL);
}


/*****************************************************************************/
// 	Description : whether setting item is modified
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_IsSettingItemModified(MN_DUAL_SYS_E dual_sys)
{
    uint16 notify_type=0;
    uint16 new_gateway_index = 0;
    uint16 old_gateway_index=0;
    MMI_STRING_T sg_text = {0};
    uint8  sg_addr[M_MMIMBBMS_MAX_SG_DOMAIN_LENS];
    MMIMBBMS_TIME_REMAINDER_E time_remainder = MMIMBBMS_REMAINDER_MAX;
    const MMIMBBMS_SETTING_INFO_T* setting_info_ptr = PNULL;
    
    setting_info_ptr = MMIMBBMS_GetConstSettingInfo();
    time_remainder   = setting_info_ptr->time_remainder;
    
    notify_type = GUIDROPDOWNLIST_GetCurItemIndex(MMIMBBMS_DROPDOWNLIST_NOTIFY_TIME_CTRL_ID);
    
    if(notify_type!= time_remainder)
    {
        
        return TRUE;
    }
    
    new_gateway_index = GUIDROPDOWNLIST_GetCurItemIndex(MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID);
    
#if 0
    if(MN_DUAL_SYS_1==dual_sys)
    {
        old_gateway_index = g_mbbms_setting_info.link_setting.sim1_index;
    }
    else
    {
        
        old_gateway_index = g_mbbms_setting_info.link_setting.sim1_index; // 1
    }
#endif
    
    old_gateway_index = MMIMBBMS_GetNetSettingIndex(dual_sys);
    
    if(new_gateway_index!=old_gateway_index)
    {
        //APN changed, deactivate PDP
        MMIMBBMS_DeactivePDPContext();
        
        return TRUE;
    }
    GUIEDIT_GetString(MMIMBBMS_EDITBOX_SG_DNS_CTRL_ID,&sg_text);
    SCI_MEMSET(sg_addr,0,sizeof(sg_addr));
    
    if(PNULL!=sg_text.wstr_ptr)
    {
        MMIAPICOM_WstrToStr(sg_text.wstr_ptr,(uint8*)sg_addr);
    }
    
    //SCI_TRACE_LOW:"sg_addr = %s sg_domain_info = %s "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9537_112_2_18_2_40_8_499,(uint8*)"ss", sg_addr, setting_info_ptr->sg_domain_info);
    
    if (strcmp(setting_info_ptr->sg_domain_info, (char*)sg_addr) != 0) 
        return TRUE;
    
    return FALSE;
    
}

/*****************************************************************************/
// 	Description : MMIMBBMS_GetStartTimerID
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIMBBMS_GetStartTimerID(void)
{
    return s_timer_id;
}

/*****************************************************************************/
// 	Description : MMIMBBMS_HandleStartTimer
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStartTimer(uint8 timer_id, uint32 param)
{
    //SCI_TRACE_LOW:"MMIMBBMS_HandleStartTimer timer_master_query = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9561_112_2_18_2_40_8_500,(uint8*)"d", timer_master_query);

    if(!MMIMTV_IsVRunning())
    {
        //SCI_TRACE_LOW:"MMIMBBMS_HandleStartTimer app is closed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9565_112_2_18_2_40_8_501,(uint8*)"");
        //MBBMS或CMMB模块已异常关闭
        if (0 != s_timer_id)
        {
            MMK_StopTimer(s_timer_id);
            s_timer_id = 0;
        }

        return;
    }

    //mbbms2.0
    MMIMBBMS_OpenMainInterfaceWin();
}

/*****************************************************************************/
// 	Description : Open window to query whether enter into cmmb or not after sg search failed
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubcribleRequestWin(void)
{
    MMI_STRING_T  prompt_str={0};
    
    if(MMK_IsOpenWin(MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID))
        return;
    
    MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRIBE_REQUEST, &prompt_str);    
    MMIPUB_OpenWaitWin(1,
        &prompt_str,
        PNULL,
        PNULL,
        MMIMBBMS_SUBSCRIBE_REQUEST_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_NONE,
        HandleSubscribeRequestWinMsg);		 
    
}

/*****************************************************************************/
// 	Description : Open window to query whether enter into cmmb or not after sg search failed
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenUnSubcribleRequestWin(void)
{
    
    MMI_STRING_T  prompt_str={0};
    
    if(MMK_IsOpenWin(MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID))
        return;
    //start unscribe					
    MMI_GetLabelTextByLang(TXT_MBBMS_UNSUBSCRIBE_REQUEST, &prompt_str);	
    
    MMIPUB_OpenWaitWin(1,
        &prompt_str,
        PNULL,
        PNULL,
        MMIMBBMS_UNSUBSCRIBE_REQUEST_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_NONE,
        HandleUnSubscribeRequestWinMsg);
    
}

/*****************************************************************************/
// 	Description : enter play from service tab
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetServiceIndexByServiceID(anyURI* pServiceID)
{
    uint16          index = 0;
    uint32          list_num = 0;
    SG_LIST_PTR_T   index_list_head_ptr = PNULL;
    SG_LIST_PTR_T   cursor_index_ptr = PNULL;
    SG_ERROR_E      error_code  = SG_ERROR_NONE;
    MBBMSSG_SERVICE_INDEX_T* pServiceIndex = PNULL;
    
    error_code = MMIMBBMS_GetServiceIndex(&index_list_head_ptr);
    if(SG_ERROR_NONE != error_code)
    {
        //SCI_TRACE_LOW:"[MMIMBBMS]:MMIMBBMS_GetServiceIndexByServiceID MMIMBBMS_GetServiceIndex error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9648_112_2_18_2_40_8_502,(uint8*)"");
        return 0;
    }
    
    //从头节点获取list_num
    list_num = index_list_head_ptr->node_cnt;
    //SCI_TRACE_LOW:"[MMIMBBMS]: MMIMBBMS_GetServiceIndexByServiceID  list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9654_112_2_18_2_40_8_503,(uint8*)"d", list_num);
    
    if(list_num <= 1)
    {
        //SCI_TRACE_LOW:"[MMIMBBMS]:MMIMBBMS_GetServiceIndexByServiceID listnum = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9658_112_2_18_2_40_8_504,(uint8*)"d", list_num);
        
        return 0;
    }
    
    cursor_index_ptr = index_list_head_ptr->next_ptr;
    
    while(--list_num && cursor_index_ptr != PNULL)
    {
        pServiceIndex = (MBBMSSG_SERVICE_INDEX_T*)(cursor_index_ptr->data);
        
        if (MMIMBBMS_IndexCompare(&(pServiceIndex->index), pServiceID) == TRUE)
        {
            return index;
        }
        
        index++;
        cursor_index_ptr = cursor_index_ptr->next_ptr;
    }
    
    return 0;
    
}

/*****************************************************************************/
// 	Description : enter play from service tab
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromAllServiceTab(void)
{
    uint16 index=0;
    SG_ERROR_E error_code =SG_ERROR_NONE;
    MBBMSSG_SERVICE_INDEX_T   *service_index_ptr=PNULL;
    SG_LIST_PTR_T   service_list_head_ptr = PNULL;
    MTVSERVICE_PROGRAM_LIST_T prog_list_t = {0};
    MTVSERVICE_PROGRAM_T      *prog_arry_ptr = PNULL;
    MMIMBBMS_CMMB_PROG_ID_T   cmmb_prog = {0};
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID;
    
    index = GUILIST_GetCurItemIndex(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID);
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        error_code = MMIMBBMS_GetServiceIndex(&service_list_head_ptr);
        if(SG_ERROR_NONE != error_code)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_EnterPlayFromAllServiceTab:Get Service Index error\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9703_112_2_18_2_40_8_505,(uint8*)"");
            //MBBMSSG_FreeServiceIndex(&service_list_head_ptr);
            return;
        }
        index = GUILIST_GetCurItemIndex(ctrl_id);
        
        service_index_ptr=(MBBMSSG_SERVICE_INDEX_T*)STL_LinkListGetNodeDataByIdx(service_list_head_ptr, index);
        if(PNULL!=service_index_ptr)
        {
            MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_SERVICE);
            MMIMBBMS_EnterPlay(service_index_ptr->index);
        }
        //MBBMSSG_FreeServiceIndex(&service_index_ptr);
    }
    else
    {
        MMIMBBMS_GetProgramList(&prog_list_t);
        prog_arry_ptr = prog_list_t.list_ptr;
        
        if(index < prog_list_t.total_count
            && PNULL != prog_arry_ptr)
        {
            cmmb_prog.net_id     = prog_arry_ptr[index].net_id;
            cmmb_prog.service_id = prog_arry_ptr[index].service_id;
            //enter cmmb mode play
            s_main_service_index = GUILIST_GetCurItemIndex(MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID);
            MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_SERVICE);
            MMIMBBMS_EnterCMMBModePlay(&cmmb_prog);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMBBMS_EnterPlayFromAllServiceTab: error enter MMIMBBMS_EnterCMMBModePlay"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9734_112_2_18_2_40_8_506,(uint8*)"");
        }
    }
}

/*****************************************************************************/
// 	Description : Enter Play in favorte service tab
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromFavoriteServiceTab(void)
{
    
    uint16 index =0;
    MMIMBBMS_SAVE_SERVICE_INFO_T*  favorite_service_ptr=PNULL;
    anyURI serviceId={0};
    SG_ERROR_E err_code =SG_ERROR_NONE;
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID;
    
    index = GUILIST_GetCurItemIndex(ctrl_id);
    favorite_service_ptr=MMIMBBMS_GetFavoriteServiceNodeViaIndex(index);
    //change global id to id
    if(PNULL != favorite_service_ptr)
    {
        
        err_code= MBBMSSG_GetServiceId(favorite_service_ptr->globalServiceID, &serviceId);
        
        if(SG_ERROR_NONE==err_code)
        {
            MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_SERVICE);
            s_main_service_index = MMIMBBMS_GetServiceIndexByServiceID(&serviceId);
            MMIMBBMS_EnterPlay(serviceId);
        }
        MBBMSSG_FreeTypeAnyURI(&serviceId);
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromProgramList(void)
{
    SG_LIST_PTR_T   pItem = PNULL;
    //SG_ERROR_E    error_code = SG_ERROR_NONE;
    SG_LIST_PTR_T   listptr = PNULL;
    BOOLEAN         is_content_encrypt = FALSE;
    MMI_CTRL_ID_T   ctrl_id = MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID;
    MBBMSSG_CONTENT_INDEX_T   *pcontentidex = PNULL;
    MTVSERVICE_PROGRAM_LIST_T prog_list_t = {0};
    MTVSERVICE_PROGRAM_T      *prog_arry_ptr = PNULL;
    MMIMBBMS_CMMB_PROG_ID_T   cmmb_prog = {0};
    uint16  service_index = 0;
    
    s_content_idx = GUILIST_GetCurItemIndex(ctrl_id);
    
    if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
    {
        // 根据index得到对应的MBBMSSG_CONTENT_INDEX_T节点            
        pItem = MMIMBBMS_GetContentNodeByIdx(s_main_service_index, s_content_idx, &listptr);
        if(PNULL != pItem)
        {
            pcontentidex = (MBBMSSG_CONTENT_INDEX_T*)(pItem->data);
            if(PNULL != pcontentidex)
            {
                is_content_encrypt = MMIMBBMS_IsContentEncrypted(pcontentidex->index);
            }
        }
        
        if(PNULL != pItem
            && PNULL != pcontentidex)
        {   
            MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_CONTENT);
            MMIMBBMS_EnterPlay(pcontentidex->index);
        }
        
        MBBMSSG_FreeContentIndex(&listptr);
    }
    else
    {
        //cmmb mode
        service_index = s_main_service_index;
        MMIMBBMS_GetProgramList(&prog_list_t);
        prog_arry_ptr = prog_list_t.list_ptr;
        
        if(service_index < prog_list_t.total_count
            && PNULL != prog_arry_ptr)
        {
            cmmb_prog.net_id     = prog_arry_ptr[service_index].net_id;
            cmmb_prog.service_id = prog_arry_ptr[service_index].service_id;
            //enter cmmb mode play
            MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_SERVICE);
            MMIMBBMS_EnterCMMBModePlay(&cmmb_prog);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMBBMS_EnterOpenOrPlayFromProgramList: error enter MMIMBBMS_EnterCMMBModePlay"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9829_112_2_18_2_40_9_507,(uint8*)"");
        }
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromProgramListOpt(void)
{
    anyURI* pGlobalContentId = PNULL;
    anyURI  contentID  = {0};
    BOOLEAN is_content_encrypt = FALSE;
    
    pGlobalContentId = (anyURI*)MMK_GetWinAddDataPtr(MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_ID);
    MBBMSSG_GetContentId(*pGlobalContentId, &contentID);
    is_content_encrypt = MMIMBBMS_IsContentEncrypted(contentID);
    
    MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_CONTENT);
    MMIMBBMS_EnterPlay(contentID);
    
    MBBMSSG_FreeTypeAnyURI(&contentID);
    
    return;
}

/*****************************************************************************/
// 	Description : Enter Play in favorte content tab
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromFavoriteContentTab(void)
{
    uint16 index=0;
    MMIMBBMS_SAVE_CONTENT_INFO_T*  favorite_content_ptr=PNULL;       	
    anyURI contentId = {0};
    MBBMSSG_CONTENT_T content_info = {0};
    SG_ERROR_E err_code=SG_ERROR_NONE;
    MMI_CTRL_ID_T ctrl_id = MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID;
    
    index = GUILIST_GetCurItemIndex(ctrl_id);
    favorite_content_ptr=(MMIMBBMS_SAVE_CONTENT_INFO_T*)MMIMBBMS_GetAlarmContentNodeViaIndex(index);
    
    if(PNULL != favorite_content_ptr)
    {
        //change global id to id
        SCI_MEMSET(&contentId,0,sizeof(anyURI));
        err_code= MBBMSSG_GetContentId(favorite_content_ptr->globalContentID, &contentId);
        
        if(SG_ERROR_NONE==err_code)
        {
            MBBMSSG_GetContent(contentId, &content_info);
            
            MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_CONTENT);
            s_main_service_index = 
                MMIMBBMS_GetServiceIndexByServiceID(&(content_info.serviceIdRef));
            MBBMSSG_FreeContent(&content_info);
            MMIMBBMS_EnterPlay(contentId);
        }
        
        MBBMSSG_FreeTypeAnyURI(&contentId);
    }
}

/*****************************************************************************/
// 	Description : Enter Play in favorte content tab
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromProgramDetail(void)
{
    anyURI* pURI_index = PNULL;
    BOOLEAN is_content_encrypt = FALSE;
    
    pURI_index = (anyURI*)MMK_GetWinAddDataPtr(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID);
    if(pURI_index!=PNULL)
    {
        is_content_encrypt = MMIMBBMS_IsContentEncrypted(*pURI_index);
    }
    
    if(pURI_index!=PNULL)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_EnterOpenOrPlayFromProgramDetail enter play\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9910_112_2_18_2_40_9_508,(uint8*)"");
        MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_CONTENT);
        
        MMIMBBMS_EnterPlay(*pURI_index);
    }
}

/*****************************************************************************/
// 	Description : Enter Play in favorte content tab
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromSearchResult(void)
{
    uint16 index = 0;
    SG_LIST_PTR_T list_ptr = PNULL;
    MBBMSSG_CONTEXT_COMPLEX_DATA_T* pContent = PNULL;
    BOOLEAN is_content_encrypt = FALSE;
    
    list_ptr = (SG_LIST_PTR_T)MMK_GetWinAddDataPtr(MMIMBBMS_SEARCH_RESULT_WIN_ID);
    index = GUILIST_GetCurItemIndex(MMIMBBMS_SEARCH_RESULT_LIST_CTRL_ID);
    pContent = (MBBMSSG_CONTEXT_COMPLEX_DATA_T*)STL_LinkListGetNodeDataByIdx(list_ptr, index);
    if(pContent != PNULL)
    {
        is_content_encrypt = MMIMBBMS_IsContentEncrypted(pContent->contextIdx_ptr->index);
    }
    
    if(pContent != PNULL)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_EnterOpenOrPlayFromSearchResult enter play\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9938_112_2_18_2_40_9_509,(uint8*)"");
        MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_CONTENT);
        
        s_main_service_index = 
            MMIMBBMS_GetServiceIndexByServiceID(pContent->serviceId_ptr);
        MMIMBBMS_EnterPlay(pContent->contextIdx_ptr->index);
    }
    
    return;
}

/*****************************************************************************/
// 	Description : MMIMBBMS_EnterOpenOrPlayFromSearchResultOpt
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_EnterOpenOrPlayFromSearchResultOpt(void)
{
    MBBMSSG_CONTEXT_COMPLEX_DATA_T* pContent = PNULL;
    BOOLEAN is_content_encrypt = FALSE;
    
    pContent = (MBBMSSG_CONTEXT_COMPLEX_DATA_T*)MMK_GetWinAddDataPtr(MMIMBBMS_SEARCH_RST_OPT_MENU_WIN_ID);
    if(pContent!=PNULL)
    {
        is_content_encrypt = MMIMBBMS_IsContentEncrypted(pContent->contextIdx_ptr->index);
    }
    
    if(pContent!=PNULL)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_EnterOpenOrPlayFromProgramDetail enter play\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9966_112_2_18_2_40_9_510,(uint8*)"");
        MMIMBBMS_SetPlayFromType(MMIMBBMS_PLAY_FROM_CONTENT);
        
        s_main_service_index = 
            MMIMBBMS_GetServiceIndexByServiceID(pContent->serviceId_ptr);
        
        MMIMBBMS_EnterPlay(pContent->contextIdx_ptr->index);
    }
    
}

/*****************************************************************************/
// 	Description : handle Subscription Query win  msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscriptionUpdateQueryWin(void)
{
    
    MMI_WIN_ID_T query_win_id = MMIMBBMS_SUBSCRIPTION_UPDATE_QUERY_WIN_ID;
    
    //SCI_TRACE_LOW:"MMIMBBMS_OpenSubscriptionUpdateQueryWin\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_9986_112_2_18_2_40_9_511,(uint8*)"");
    
    MMIPUB_OpenQueryWinByTextId(TXT_MBBMS_UPDATE_SUBSCRIPTION_HINT, 
        IMAGE_PUBWIN_QUERY, 		
        &query_win_id,
        HandleSubcriptionUpdateQueryWinMsg);
}

/*****************************************************************************/
// 	Description : open subscription update query wein by text
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscriptionUpdateQueryWinByTextPtr(MMI_STRING_T *text_ptr)
{
    
    MMI_WIN_ID_T query_win_id = MMIMBBMS_SUBSCRIPTION_UPDATE_QUERY_WIN_ID;
    
    //SCI_TRACE_LOW:"MMIMBBMS_OpenSubscriptionUpdateQueryWinByTextPtr\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10003_112_2_18_2_40_9_512,(uint8*)"");
    
    MMIPUB_OpenQueryWinByTextPtr(text_ptr, 
        PNULL,
        IMAGE_PUBWIN_QUERY, 		
        &query_win_id,
        HandleSubcriptionUpdateQueryWinMsg);
}

/*****************************************************************************/
// 	Description : open cmmb mode warning win
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenCMMBModeWarningWin(void)
{
    MMI_WIN_ID_T    win_id = MMIMBBMS_CMMB_MODE_WARNING_WIN_ID;
    MMI_TEXT_ID_T   text_id = TXT_MBBMS_NO_SIM;
    uint32          sim_num = 0;
    uint16          sim_sys  = MN_DUAL_SYS_1;
    
    //获取可用SIM数目
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    
    if(0 == sim_num)    //没有可用的SIM卡
    {
        text_id = TXT_MBBMS_NO_SIM;
    }
    else
    {
        text_id = TXT_MBBMS_NO_SIGNAL;
    }
    
    MMIPUB_OpenQueryWinByTextId(
        text_id, 
        IMAGE_PUBWIN_WARNING, 
        &win_id, 
        HandleCMMBModeWarningWinMsg);
}

/*****************************************************************************/
// 	Description : handle cmmb mode warning win msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCMMBModeWarningWinMsg( 
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case  MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, TRUE);
        break;
        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
	MMK_CloseWin(win_id);
	MMIMBBMS_ExitMBBMS();
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : open swith to mbbms mode query win
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSwitchToMBBMSQueryWin(void)
{
        
    MMI_WIN_ID_T query_win_id = MMIMBBMS_SWITCH_MBBMS_QUERY_WIN_ID;
    
    //SCI_TRACE_LOW:"MMIMBBMS_OpenSwitchToMBBMSQueryWin  cur_flow_status=%d, cur_flow_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10092_112_2_18_2_40_9_513,(uint8*)"dd", MMIMBBMS_GetFlowInfo()->status, MMIMBBMS_GetFlowInfo()->flow_type);
    
    MMIPUB_OpenQueryWinByTextId(TXT_MBBMS_SWTICH_MBBMS_QUERY, 
        IMAGE_PUBWIN_QUERY, 		
        &query_win_id,
        HandleSwitchToMBBMSQueryWinMsg);
}

/*****************************************************************************/
// 	Description : handle switch to mbbms query win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchToMBBMSQueryWinMsg( 
                                                  MMI_WIN_ID_T     win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM              param
                                                  )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        OpenSwitchWaitingWin();
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
// 	Description : open switch waithing win
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void OpenSwitchWaitingWin(void)
{
    MMI_STRING_T  prompt_str={0};

    if(MMK_IsOpenWin(MMIMBBMS_SWITCH_WAIT_WIN_ID))
    {
        return;
    }
					
    MMI_GetLabelTextByLang(TXT_MBBMS_SWTICH_WAITING, &prompt_str);	
    MMIPUB_OpenWaitWin(1,
        &prompt_str,
        PNULL,
        PNULL,
        MMIMBBMS_SWITCH_WAIT_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_NONE,
        HandleSwitchWaithingWinMsg);
}

/*****************************************************************************/
// 	Description : handle switch to mbbms query win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSwitchWaithingWinMsg( 
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_NULL, FALSE);
        MMITHEME_UpdateRect();
        StartSwitchToMBBMSProcess();
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : close related win while swithing to mbbms mode
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void CloseRelatedWinWhenSwtich(void)
{
    MMK_CloseWin(MMIMBBMS_CMMB_MODE_WARNING_WIN_ID);
    MMK_CloseWin(MMIMBBMS_WATCH_CONNECTING_WIN_ID);
    MMK_CloseWin(MMIMBBMS_HELP_INFO_WIN_ID);
    MMK_CloseWin(MMIMBBMS_SETTING_WIN_ID);
    MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_POPMENU_WIN_ID);
    MMK_CloseWin(MMIMBBMS_MEMFULL_ALERT_WIN_ID);
    
    MMK_CloseWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID);
    MMK_CloseWin(MMIMBBMS_PROGRAM_CHILD_WIN_ID);
    MMK_CloseWin(MMIMBBMS_PROGRAM_LIST_OPT_MENU_WIN_ID);
    MMK_CloseWin(MMIMBBMS_PROGRAM_DETAIL_MAIN_WIN_ID);
    MMK_CloseWin(MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_WIN_ID);
    MMK_CloseWin(MMIMBBMS_SG_DNS_EDIT_WIN_ID);
    MMK_CloseWin(MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID);
}

/*****************************************************************************/
// 	Description : start swith to mbbms process
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartSwitchToMBBMSProcess(void)
{
    //SCI_TRACE_LOW:"StartSwitchToMBBMSProcess enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10217_112_2_18_2_40_9_514,(uint8*)"");
    
    CloseRelatedWinWhenSwtich();
    
    if(MMIMBBMS_FLOW_STATE_IDLE != MMIMBBMS_GetFlowInfo()->status)
    {
        //SCI_TRACE_LOW:"StartSwitchToMBBMSProcess cur_status=%d, cur_flow_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10224_112_2_18_2_40_9_515,(uint8*)"dd", MMIMBBMS_GetFlowInfo()->status, MMIMBBMS_GetFlowInfo()->flow_type);
        
        //stop current flow
        MMIMBBMS_StopFlow();
        
        //create timer
        if (0 == s_switch_mbbms_timer_id)
        {
            s_switch_mbbms_timer_id = MMK_CreateTimerCallback(1000, HandleSwitchToMBBMSTimer, NULL,TRUE);
        }
        
        return;
    }
    
    if(MMK_IsOpenWin(MMIMTV_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
    }
    
    if (0 != s_switch_mbbms_timer_id)
    {
        MMK_StopTimer(s_switch_mbbms_timer_id);
        s_switch_mbbms_timer_id = 0;
    }
    
    //change service mode and start mbbms flow
    MTVSERVICE_ChangeMode(MBBMS_SERVICE_MODE_MBBMS);
    MMIMBBMS_SetServiceMode(MBBMS_SERVICE_MODE_MBBMS);
    
    MMIMBBMS_SetInitStatus(FALSE);
    MMIMBBMS_StartParseSG(MMIMBBMS_SG_SEARCH_PARSE_FROM_PRE_INIT);
}

/*****************************************************************************/
// 	Description : handle switch mbbms timer
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSwitchToMBBMSTimer(uint8 timer_id, uint32 param)
{
    //SCI_TRACE_LOW:"HandleSwitchToMBBMSTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10263_112_2_18_2_40_10_516,(uint8*)"");
    
    if(!MMIMBBMS_IsMBBMSOpen())
    {
        //SCI_TRACE_LOW:"HandleSwitchToMBBMSTimer app is closed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10267_112_2_18_2_40_10_517,(uint8*)"");
        
        if (0 != s_switch_mbbms_timer_id)
        {
            MMK_StopTimer(s_switch_mbbms_timer_id);
            s_switch_mbbms_timer_id = 0;
        }
        
        return;
    }
    
    //start switch
    StartSwitchToMBBMSProcess();
}

/*****************************************************************************/
// 	Description : handle Subscription Query win  msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubcriptionUpdateQueryWinMsg( 
                                                      MMI_WIN_ID_T     win_id, 
                                                      MMI_MESSAGE_ID_E    msg_id, 
                                                      DPARAM              param
                                                      )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            MMI_STRING_T prompt_str={0};
            
            MMI_GetLabelTextByLang(TXT_MBBMS_SUBSCRIPTION_UPDATING,&prompt_str);
            //open subscription update request win
            MMIPUB_OpenWaitWin(1,
                &prompt_str,
                PNULL,
                PNULL,
                MMIMBBMS_SUBSCRIPTION_UPDATE_REQUEST_WIN_ID,
                IMAGE_NULL,
                ANIM_PUBWIN_WAIT,
                WIN_ONE_LEVEL,
                MMIPUB_SOFTKEY_ONE,
                HandleSubscriptionUpdateRequestWinMsg);
            MMK_CloseWin(win_id);
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_NONE);
            s_object_type=MMIMBBMS_OBJECT_FROM_NONE;
            MMK_CloseWin(win_id);
            break;
        }
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : handle sg update request win msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenSGUpdateRequestWin(void)
{
    MMI_STRING_T  prompt_str={0};
    
    /*add for init not finished*/
    if(!MMIMBBMS_DisplayInitWaitingWin())
    {
        return;
    }
    
    if(MMK_IsOpenWin(MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID))
    {
        return;
    }
    
    /* 为防止频繁释放service list，所以只有当更新SG，或退出MBBMS时才释放service list */
    //MMIMBBMS_FreeServiceIndex();					
    MMI_GetLabelTextByLang(TXT_MBBMS_SG_UPDATEING, &prompt_str);	
    
    MMIPUB_OpenWaitWin(1,
        &prompt_str,
        PNULL,
        PNULL,
        MMIMBBMS_SG_UPDATE_REQUEST_WIN_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_NONE,
        HandleSGUpdateRequestWinMsg);
    
    //刷新完成，成功或者失败均返回list列表
    if(MMK_IsOpenWin(MMIMBBMS_UPDATE_FROM_CITY_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_UPDATE_FROM_CITY_MAIN_WIN_ID);
    }
    
    if (MMK_IsOpenWin(MMIMBBMS_UPDATE_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_UPDATE_MAIN_WIN_ID);
    }
}

/*****************************************************************************/
// 	Description : handle sg update request win msg
//	Global resource dependence : none
//  Author: lisa.lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSGUpdateRequestWinMsg( 
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case  MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
        MMIMBBMS_StartManualUpdateSG(MMIMBBMS_SG_SEARCH_PARSE_FROM_MAINMENU);
        break;
        
    case MSG_APP_CANCEL:	    
        MMIMBBMS_StopFlow();
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : enter play via from_type
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void  MMIMBBMS_EnterPlayOrOpen (MMIMBBMS_OBJECT_FROM_TYPE_E obj_from_type)
{
    s_object_type = obj_from_type;
    
    //SCI_TRACE_LOW:"MMIMBBMS_EnterPlayOrOpen s_object_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10419_112_2_18_2_40_10_518,(uint8*)"d",s_object_type);
    //SCI_TRACE_LOW:"MMIMBBMS_TickCount MMIMBBMS_EnterPlayOrOpen enter = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10420_112_2_18_2_40_10_519,(uint8*)"d",SCI_GetTickCount());
    
    switch(obj_from_type)
    {
    case MMIMBBMS_OBJECT_FROM_ALL_SERVICE: 
        MMIMBBMS_EnterOpenOrPlayFromAllServiceTab();    
        break;
    case MMIMBBMS_OBJECT_FROM_FAVORITE_SERVICE:
        MMIMBBMS_EnterOpenOrPlayFromFavoriteServiceTab();
        break;
    case MMIMBBMS_OBJECT_FROM_FAVORITE_CONTENT:
        MMIMBBMS_EnterOpenOrPlayFromFavoriteContentTab();
        break;
    case MMIMBBMS_OBJECT_FROM_PROGROM_LIST:
        MMIMBBMS_EnterOpenOrPlayFromProgramList();
        break;
    case MMIMBBMS_OBJECT_FROM_PROGROM_DETAIL:
        MMIMBBMS_EnterOpenOrPlayFromProgramDetail();
        break;
    case MMIMBBMS_OBJECT_FROM_SEARCH_RESULT:
        MMIMBBMS_EnterOpenOrPlayFromSearchResult();
        break;
    case MMIMBBMS_OBJECT_FROM_SEARCH_RESULT_OPT:
        MMIMBBMS_EnterOpenOrPlayFromSearchResultOpt();
        break;
    case MMIMBBMS_OBJECT_FROM_PROGRAM_LIST_OPT:
        MMIMBBMS_EnterOpenOrPlayFromProgramListOpt();
        break;
    default:
        break;
    }
    
    return;
}

/*****************************************************************************/
// 	Description : handle subscription update request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSubscriptionUpdateRequestWinMsg( 
                                                         MMI_WIN_ID_T     win_id, 
                                                         MMI_MESSAGE_ID_E    msg_id, 
                                                         DPARAM              param
                                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case  MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
        //start subscription update process
        MMIMBBMS_StartUpdateSubscriptionTable(MMIMBBMS_GetSubUpdateType());
        break;
    case MSG_APP_CANCEL:	
        MMIMBBMS_StopFlow();
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : handle Account Inquiry request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenAccountInquiryRequestWin(void)
{
    MMI_STRING_T  prompt_str={0};
    
    //SCI_TRACE_LOW:"MMIMBBMS_OpenAccountInquiryRequestWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10493_112_2_18_2_40_10_520,(uint8*)"");
    if (MMIMBBMS_SDandOpenCondition() == FALSE)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_OpenAccountInquiryRequestWin MMIMBBMS_SDandOpenCondition return FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10496_112_2_18_2_40_10_521,(uint8*)"");
        return;
    }
    if(!MMIMBBMS_GetSubscriptionUpdateStatus())
    {
        MMIMBBMS_SetSubUpdateType(MMIMBBMS_SUB_UPDATE_FROM_PURCHASE_MANAGE);
        MMIMBBMS_OpenSubscriptionUpdateQueryWin();
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_ACCOUNT_INQUIRING, &prompt_str);	
    
        MMIPUB_OpenWaitWin(1,
            &prompt_str,
            PNULL,
            PNULL,
            MMIMBBMS_ACCOUNT_INQUIRY_REQUEST_WIN_ID,
            IMAGE_NULL,
            ANIM_PUBWIN_WAIT,
            WIN_ONE_LEVEL,
            MMIPUB_SOFTKEY_ONE,
            HandleAccountInquiryRequestWinMsg);
    	}
    
}
/*****************************************************************************/
// 	Description : handle Account Inquiry request win msg
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAccountInquiryRequestWinMsg( 
                                                     MMI_WIN_ID_T     win_id, 
                                                     MMI_MESSAGE_ID_E    msg_id, 
                                                     DPARAM              param
                                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    LOCAL BOOLEAN is_wait_get_focus = FALSE;
    
    switch(msg_id)
    {
    case  MSG_OPEN_WINDOW:
        is_wait_get_focus = FALSE;
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
        MMIMBBMS_StartAccoutInquiryProcess();
        break;
        
    case MSG_APP_CANCEL:	
        MMIMBBMS_StopFlow();
        break;

    case MSG_APP_MBBMS_WAIT_GET_FOCUS:
        is_wait_get_focus = TRUE;
        break;

    case MSG_GET_FOCUS:
        if(is_wait_get_focus)
        {
            MMIMBBMS_OpenSubscribeTabMainWin();
            
            MMIPUB_OpenAlertSuccessWin(TXT_MBBMS_ACCOUNT_INQUIRY_OK);        
            
            MMK_CloseWin(win_id);
        }
        
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
// 	Description : open Purchase Operation cancel waiting win
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitWin(BOOLEAN is_unpurchase_operation)
{
    MMI_STRING_T  prompt_str={0};
    
    //SCI_TRACE_LOW:"MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitWin %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10576_112_2_18_2_40_10_522,(uint8*)"d",(uint32)is_unpurchase_operation);
	
    if(is_unpurchase_operation)
        MMI_GetLabelTextByLang(TXT_MBBMS_CANCEL_UNSUBSCRIBE, &prompt_str);		
	else
        MMI_GetLabelTextByLang(TXT_MBBMS_CANCEL_SUBSCRIBE, &prompt_str);	
    
    MMIPUB_OpenWaitWin(1,
        &prompt_str,
        PNULL,
        PNULL,
        MMIMBBMS_CANCEL_PURCHASE_UNPURCHASE_WAIT_ID,
        IMAGE_NULL,
        ANIM_PUBWIN_WAIT,
        WIN_ONE_LEVEL,
        MMIPUB_SOFTKEY_NONE,
        MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitMsg);
    
}
/*****************************************************************************/
// 	Description : handle open Purchase Operation cancel waiting win
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitMsg( MMI_WIN_ID_T     win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id, 
                                                 DPARAM              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case  MSG_OPEN_WINDOW:
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_NULL, TRUE);
	        break;
        default:
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
            break;
    }		
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void  MMIMBBMS_SetServiceIdex (uint16 service_index)
{
    s_main_service_index = service_index;
    
    return;
}

PUBLIC void MMIMBBMS_GetPreviewText(wchar* wchar_text, MBBMSSG_PREVIEW_DATA_T *data_info)
{
    MBBMSSG_PREVIEWDATA_SEARCH_T search_info = {0};
    SG_ERROR_E errcode = SG_ERROR_NONE;
    uint16 str_len = 0;
    //uint16 to_len = 0;
    uint8* str_buf_ptr = PNULL;
    SG_LIST_PTR_T pServiceNode = PNULL;
    
    pServiceNode = MMIMBBMS_GetServiceNodeByIdx(s_main_service_index);
    
    //SCI_TRACE_LOW("MMIMBBMS_GetPreviewText GET service, pServiceNode = 0x%x", pServiceNode);
    
    if (pServiceNode != PNULL)
    {
        search_info.index =  *((anyURI*)(pServiceNode->data));
        //SCI_TRACE_LOW("MMIMBBMS_GetPreviewText GET service, ID LEN=%d", search_info.index.string_len);
    }
    
    search_info.usage = MBBMS_PREVIEW_CHAN_SWITCH;
    search_info.range = MBBMS_PREVIEWDATA_RANGE_SERVICE;
    errcode = MBBMSSG_GetPreviewdata(search_info,data_info);
    
    //SCI_TRACE_LOW("MMIMBBMS_GetPreviewText errcode =%d", errcode);
    
    if(PNULL != data_info->text.content_string_ptr)
    {
        str_buf_ptr = data_info->text.content_string_ptr;
        //SCI_TRACE_LOW("MMIMBBMS_GetPreviewText connect 1 str_len =%d", data_info.text.string_len);
    }
    else
    {
        str_buf_ptr = data_info->text.string_arr;
        //SCI_TRACE_LOW("MMIMBBMS_GetPreviewText connect 2 str_len =%d", data_info.text.string_len);
    }
    
    str_len = GUI_UTF8ToWstr(wchar_text, data_info->text.string_len,
        str_buf_ptr, data_info->text.string_len);
    
    //SCI_TRACE_LOW("MMIMBBMS_GetPreviewText str_len =%d", str_len);
    //MBBMSSG_FreePreviewData(&data_info);
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_OBJECT_FROM_TYPE_E MMIMBBMS_GetObjectType(void)
{
    
    return s_object_type;
    
}

/*****************************************************************************/
// 	Description : display init waiting alert win
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_DisplayInitWaitingWin(void)
{
    
    //SCI_TRACE_LOW:"MMIMBBMS_DisplayInitWaitingWin:init status=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10688_112_2_18_2_40_11_523,(uint8*)"d",MMIMBBMS_GetInitStatus());
    
    if(!MMIMBBMS_GetInitStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_MBBMS_INIT_WAITING);
        
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : 设置StartUpWin异常关闭标志变量
//	Global resource dependence : none
//  Author: 
//	Note: 变量初始为TRUE，正常关闭窗口时设为FALSE
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetStartUpWinCloseFlag(BOOLEAN flag_state)
{
    s_is_startupwin_abnormal_close = flag_state;
}

/*****************************************************************************/
// 	Description : StartUpWin是否异常关闭
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsStartUpWinAbnormalClose(void)
{
    return s_is_startupwin_abnormal_close;
}

/*****************************************************************************/
// 	Description : 增加套餐管理Tab信息 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL void MMIMBBMS_SubscribeManageWinAppendTabInfo(void)
{
    GUITAB_ITEM_T   item_t = {0};
    GUITAB_ITEM_CONTENT_T item_data_t = {0};
    
    GUITAB_SetMaxItemNum(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, MMIMBBMS_SUBSCRIPTE_TYPE_MAX);
    GUITAB_SetItemSpace(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID,MMIMBBMS_TAB_ITEM_SPACE,FALSE);
    GUITAB_SetState(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID,GUITAB_STATE_NONEED_SCROLL,TRUE);

    
    item_t.item_state = GUITAB_ITEMSTATE_SHOW_TEXT;
    item_t.item_data_ptr = &item_data_t;
    item_data_t.text_data.text_type = TABITEM_DATA_TEXT_ID;
    item_data_t.text_data.is_disp_font_bg = TRUE;
    
    //已订套餐
    item_t.child_win_handle = MMIMBBMS_PURCHASEDITEM_CHILD_WIN_ID;
    item_data_t.text_data.text.text_id = TXT_MBBMS_PURCHASED_ITEM;
    GUITAB_AppendItem(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, &item_t);
    
    //本地套餐
    item_t.child_win_handle = MMIMBBMS_LOCAL_PURCHASEITEM_CHILD_WIN_ID;
    item_data_t.text_data.text.text_id = TXT_MBBMS_UNPURCHASED_ITEM;
    GUITAB_AppendItem(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, &item_t);
    
    //GUITAB_SetCurSel(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, MMIMBBMS_SUBSCRIPTE_TYPE_PURCHASED);
    if(MMIMBBMS_GetIfEnterSubFromPlay())
    {
        //s_is_enter_purchase_from_play = FALSE;
        MMIMBBMS_SetIfEnterSubFromPlay(FALSE);
        GUITAB_SetCurSel(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, MMIMBBMS_SUBSCRIPTE_TYPE_LOCAL_PURCHASE);
    }
    else
    {
        GUITAB_SetCurSel(MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID, MMIMBBMS_SUBSCRIPTE_TYPE_PURCHASED);
    }    
}

/*****************************************************************************/
// 	Description : 向频道切换栏添加频道项
//	Global resource dependence : 
//  Author: xin.li
//	Note: 依据SG中的频道列表
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddServiceItemToTabCtrl(MMI_CTRL_ID_T ctrl_id)
{
    uint32  i = 0;
    uint16  service_count = 0;

    GUITAB_ITEM_T             item_t = {0};
    GUITAB_ITEM_CONTENT_T     item_data_t = {0};
    MTVSERVICE_PROGRAM_LIST_T service_list_t = {0};
    MTVSERVICE_PROGRAM_T      *service_arry_ptr = PNULL;
    
    item_t.item_state = GUITAB_ITEMSTATE_SHOW_TEXT;
    item_t.item_data_ptr = &item_data_t;
    item_data_t.text_data.is_disp_font_bg = TRUE;
    item_data_t.text_data.text_type = TABITEM_DATA_TEXT_BUFFER;
    
    //获取频道列表数据
    MMIMBBMS_GetProgramList(&service_list_t);
    service_arry_ptr = service_list_t.list_ptr;
    service_count = service_list_t.total_count;
    
    if(service_count > GUITAB_MAX_ITEM_NUM)
    {
        //SCI_TRACE_LOW:"MMIMBBMS_AddServiceItemToTabCtrl service_count = %d large than GUITAB_MAX_ITEM_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10786_112_2_18_2_40_11_524,(uint8*)"d", service_count);
        service_count = GUITAB_MAX_ITEM_NUM;
    }
    
    GUITAB_SetMaxItemNum(ctrl_id, service_count);
    GUITAB_SetItemSpace(ctrl_id, MMIMBBMS_TAB_ITEM_SPACE, FALSE);
    
    for(i=0; i<service_count; i++)
    {
        SCI_MEMSET(item_data_t.text_data.text.text_buffer.wstr, 0, sizeof(item_data_t.text_data.text.text_buffer.wstr));
        
        //获取频道名
        if(MBBMS_SERVICE_MODE_MBBMS == MMIMBBMS_GetServiceMode())
        {
            //频道名
            GetNameWcharByLangString(&service_arry_ptr[i].name, 
                MBBMS_SERVICE_MODE_MBBMS,
                0,
                0,
                item_data_t.text_data.text.text_buffer.wstr,
                GUITAB_MAX_ITEM_NAME_LENGTH);
        }
        else
        {
            GetNameWcharByLangString(&service_arry_ptr[i].name, 
                MBBMS_SERVICE_MODE_CMMB,
                service_arry_ptr[i].char_set,
                service_arry_ptr[i].service_id,
                item_data_t.text_data.text.text_buffer.wstr,
                GUITAB_MAX_ITEM_NAME_LENGTH);
        }
        item_data_t.text_data.text.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data_t.text_data.text.text_buffer.wstr);
        
        //append tab item
        GUITAB_AppendItem(ctrl_id, &item_t);
    }
    
    if (s_main_service_index < service_count)
    {
        GUITAB_SetCurSel(ctrl_id, s_main_service_index);
    }
    else
    {
        GUITAB_SetCurSel(ctrl_id, 0);
    }
    
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, ctrl_id);
}

/*****************************************************************************/
//  Description : handle message of update list window
//  Global resource dependence : none
//  Author:yexiong.liu
//  Note: 刷新界面，包含自动刷新和指定城市刷新两项
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleUpdateMainWinMsg(
                                                      MMI_WIN_ID_T     win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM           param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;	
    MMI_CTRL_ID_T  ctrl_id = MMIMBBMS_UPDATE_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id = 0; 
    MMI_MENU_ID_T           menu_id  = 0;
    
    //SCI_TRACE_LOW("MMIMBBMS_HandleUpdateMainWinMsg() enter:msg_id=%d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        GUIWIN_SetTitleRollArrow(win_id,FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);            
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }
 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:  
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch( menu_id )
        {
        case MTV_ID_MBBMS_AUTO_UPDATE_MENU:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateMainWinMsg  MTV_ID_MBBMS_AUTO_UPDATE_MENU"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10873_112_2_18_2_40_11_525,(uint8*)"");
            MMIMBBMS_OpenSGUpdateRequestWin();                     
            break;
            
        case MTV_ID_MBBMS_CITY_UPDATE_MENU:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateMainWinMsg  MTV_ID_MBBMS_CITY_UPDATE_MENU"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10878_112_2_18_2_40_11_526,(uint8*)"");
            if(MMK_IsOpenWin(MMIMBBMS_UPDATE_FROM_CITY_MAIN_WIN_ID))
            {
                MMK_CloseWin(MMIMBBMS_UPDATE_FROM_CITY_MAIN_WIN_ID);
            }
            MMK_CreateWin((uint32*)MMIMBBMS_UPDATE_FROM_CITY_MAIN_TAB, PNULL);            
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateMainWinMsg  ID ERROR!!!!!!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10887_112_2_18_2_40_11_527,(uint8*)"");
            break;
        }
        break;
        
    default:
        //SCI_TRACE_LOW("MMIMBBMS_HandleUpdateMainWinMsg() msg_id=%d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : open update win
//  Global resource dependence : none
//  Author: yexiong.liu
//  Note: 
/*****************************************************************************/
LOCAL void MMIMBBMS_OpenUpdateListMainWin(void)
{
    /*add for init not finished*/
    if(!MMIMBBMS_DisplayInitWaitingWin())
    {
        return;
    }    
    if (MMK_IsOpenWin(MMIMBBMS_UPDATE_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_UPDATE_MAIN_WIN_ID);
    }
        
    MMK_CreateWin((uint32 *)MMIMBBMS_UPDATE_MAIN_TAB,PNULL);
}

/*****************************************************************************/
//     Description : handle message of update from city name win
//    Global resource dependence : none
//    Author:yexiong.liu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMBBMS_HandleUpdateFromCityNameWinMsg(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM                param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T    prompt_text={0};
    MMI_STRING_T    str_city_name = {0}; 
    
    //SCI_TRACE_LOW("MMIMBBMS_HandleUpdateFromCityNameWinMsg() enter:msg_id=%d",msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:			 
        MMI_GetLabelTextByLang(TXT_MBBMS_UPDATE_PROMPT,&prompt_text);
        GUITEXT_SetString(MMIMBBMS_TEXT_CITY_UPDATE_CTRL_ID, prompt_text.wstr_ptr, prompt_text.wstr_len, FALSE);
        GUIEDIT_SetSoftkey(MMIMBBMS_EDITTEXT_CITY_UPDATE_CTRL_ID,	0, 1, TXT_NULL,	TXT_COMMON_OK, PNULL);
        MMK_SetAtvCtrl(win_id,MMIMBBMS_EDITTEXT_CITY_UPDATE_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        GUIEDIT_GetString(MMIMBBMS_EDITTEXT_CITY_UPDATE_CTRL_ID,&str_city_name);
        if(0 != str_city_name.wstr_len)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_HandleUpdateFromCityNameWinMsg()  start city update"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10952_112_2_18_2_40_11_528,(uint8*)"");
            MMIMBBMS_SetUpdateCityName(&str_city_name);
            
            MMIMBBMS_SetCityUpdateSGFlag(TRUE);
            MMIMBBMS_OpenSGUpdateRequestWin();			
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    default:
        //SCI_TRACE_LOW("MMIMBBMS_HandleUpdateFromCityNameWinMsg() msg_id=%d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
// Description : get program list info
// Global resource dependence :
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_GetProgramList(MTVSERVICE_PROGRAM_LIST_T* list_ptr)
{
    BOOLEAN recode = TRUE;
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_INVALID;
    
    //封装减少时间开销，只有数据更新时才进行调用MTVSERVICE_GetProgramList
    //数据更新的情况包括：启动MBBMS中解析子流程完成，手动更新完成
    if(PNULL == list_ptr)
    {
        return FALSE;
    }

    if(PNULL == s_program_list_ptr)
    {
        s_program_list_ptr = (MTVSERVICE_PROGRAM_LIST_T*)SCI_ALLOC_APP(sizeof(MTVSERVICE_PROGRAM_LIST_T));
        if(PNULL == s_program_list_ptr)
        {
            //SCI_TRACE_LOW:"MMIMBBMS_GetProgramList s_program_list_ptr alloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_10994_112_2_18_2_40_11_529,(uint8*)"");
            return FALSE;
        }
        SCI_MEMSET(s_program_list_ptr, 0, sizeof(MTVSERVICE_PROGRAM_LIST_T));
        
        service_ret = MTVSERVICE_GetProgramList(TRUE, MTVSERVICE_LANG_ZHO, s_program_list_ptr);
        if(MBBMS_SERVICE_FUNC_OK != service_ret)
        {
            return FALSE;
        }
        SCI_MEMCPY(list_ptr, s_program_list_ptr, sizeof(MTVSERVICE_PROGRAM_LIST_T));
    }
    else
    {
        SCI_MEMCPY(list_ptr, s_program_list_ptr, sizeof(MTVSERVICE_PROGRAM_LIST_T));
    }
    
    return recode;
}

/*****************************************************************************/
// Description : free program list info
// Global resource dependence :
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeProgramList(void)
{
    if(PNULL != s_program_list_ptr)
    {
        MTVSERVICE_FreeProgramList(s_program_list_ptr);
        
        SCI_FREE(s_program_list_ptr);
        s_program_list_ptr = PNULL;
    }
}

/*****************************************************************************/
// Description : get cmmb content start/end time info
// Global resource dependence : none
// Author: 
// Note: 从CMMB节目结构中获取start/end信息
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetCmmbStartEndTimeString(MMIMTV_PROGRAMME_T* cmmb_prog_ptr,
                                               wchar* pwchar
                                               )
{
    char    str_date[10] = {0};
    wchar   wchar_starttime[5] = {0};
    wchar   wchar_endtime[5] = {0};
    uint32  final = 0;
    MMI_TM_T end_time = {0};
    
    if(PNULL == cmmb_prog_ptr || PNULL == pwchar)
    {
        return;
    }
    
    sprintf(str_date, "%2d:%2d", cmmb_prog_ptr->StartTime.hours, cmmb_prog_ptr->StartTime.minutes);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_starttime);
    
    //calc end time
    final = MMIAPICOM_Tm2Second(cmmb_prog_ptr->StartTime.seconds,
        cmmb_prog_ptr->StartTime.minutes,
        cmmb_prog_ptr->StartTime.hours,
        cmmb_prog_ptr->StartTime.days,
        cmmb_prog_ptr->StartTime.months,
        cmmb_prog_ptr->StartTime.years) + cmmb_prog_ptr->Duration;
    end_time = MMIAPICOM_Second2Tm(final);
    
    sprintf(str_date, "%2d:%2d", end_time.tm_hour, end_time.tm_min);
    
    if (str_date[0] == 0x20)
    {
        str_date[0] = '0';
    }
    
    if (str_date[3] == 0x20)
    {
        str_date[3] = '0';
    }
    
    MMIAPICOM_StrToWstr((uint8*)str_date, wchar_endtime);
    
    MMIAPICOM_Wstrncpy(pwchar, wchar_starttime,  5);
    pwchar[5] = (wchar)'-';
    MMIAPICOM_Wstrncpy(pwchar+6, wchar_endtime, 5);
    pwchar[11] = '\0';
}

/*****************************************************************************/
// Description : get cmmb content name
// Global resource dependence : none
// Author: xin.li
// Note: 从CMMB节目结构中获取节目名
/*****************************************************************************/
LOCAL void  MMIMBBMS_GetCmmbContentNameString(MMIMTV_PROGRAMME_T* cmmb_prog_ptr,
                                              wchar* wchar_name,    //out
                                              uint16 wstr_len       //in max len
                                              )
{
    if(PNULL == cmmb_prog_ptr || PNULL == wchar_name)
    {
        return;
    }
    
    MMIMTV_Mtvstring2Unicode(&cmmb_prog_ptr->Title, wchar_name, wstr_len);
}


/*****************************************************************************/
// 	Description : get the playing content index
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIMBBMS_GetPlayingContentIndex(MBBMSSG_CONTENT_INDEX_T* content_index_ptr)
{

    SCI_DATE_T sysdate  = {0};
    SG_DATETIME_T nowDatetime = {0};
    SCI_TIME_T time_info = {0};
    BOOLEAN bRight = FALSE;

    //SCI_TRACE_LOW:"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex content year=%d, month=%d, day=%d, hour=%d, minutes=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11122_112_2_18_2_40_12_530,(uint8*)"ddddd", nowDatetime.years, nowDatetime.months, nowDatetime.days, nowDatetime.hours, nowDatetime.minutes);
    
    if(PNULL == content_index_ptr)
    {
        return FALSE;
    }
    
    TM_GetSysTime(&time_info);
    nowDatetime.hours = time_info.hour;
    nowDatetime.minutes = time_info.min;
    //SCI_TRACE_LOW:"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex SYSTEM hour=%d, minutes=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11132_112_2_18_2_40_12_531,(uint8*)"dd", nowDatetime.hours, nowDatetime.minutes);
    
    TM_GetSysDate(&sysdate);
    nowDatetime.years = sysdate.year;
    nowDatetime.months = sysdate.mon;
    nowDatetime.days     = sysdate.mday;
    //SCI_TRACE_LOW:"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex SYSTEM year=%d, month=%d, day=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11138_112_2_18_2_40_12_532,(uint8*)"ddd", nowDatetime.years, nowDatetime.months, nowDatetime.days);
    //如果非当天节目则肯定不会是在播节目
    if(0 == s_day_index)
    {
        if (( content_index_ptr->startTime.hours < nowDatetime.hours
            || (content_index_ptr->startTime.hours == nowDatetime.hours
            &&  content_index_ptr->startTime.minutes < nowDatetime.minutes))
            && (content_index_ptr->endTime.hours > nowDatetime.hours
            || (content_index_ptr->endTime.hours == nowDatetime.hours
            &&  content_index_ptr->endTime.minutes > nowDatetime.minutes)))
        {
            bRight = TRUE;
        }
    }
    //SCI_TRACE_LOW:"[MMIMBBMS] MMIMBBMS_GetPlayingContentIndex bRight=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11152_112_2_18_2_40_12_533,(uint8*)"d", bRight);
    
    return bRight;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
void MMIMBBMS_SetIfEnterSubFromPlay(BOOLEAN ret)
{
    //SCI_TRACE_LOW:"[MMIMBBMS] MMIMBBMS_SetIfEnterSubFromPlay ret=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11163_112_2_18_2_40_12_534,(uint8*)"d", ret);
    s_is_enter_purchase_from_play = ret;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
BOOLEAN MMIMBBMS_GetIfEnterSubFromPlay(void)
{
    //SCI_TRACE_LOW:"[MMIMBBMS] MMIMBBMS_GetIfEnterSubFromPlay ret=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11173_112_2_18_2_40_12_535,(uint8*)"d", s_is_enter_purchase_from_play);
    return s_is_enter_purchase_from_play;
}

/*****************************************************************************/
// 	Description : get the content type from genre and add it to the droplist
//	Global resource dependence : 
//  Author:       yexiong.liu
//	Note:   
/*****************************************************************************/
LOCAL void MBBMS_AddAllContentTypeToDropdownListNew(void)
{
    uint32 index = 0;
    //MMI_STRING_T  temp_str = {0};
    MMI_STRING_T  *item_data_ptr = PNULL;
    uint16 total_num = 0;
    SG_LIST_PTR_T   genre_ptr = PNULL;
    contentString *content_type_ptr = PNULL;
    uint8 *content_type_string_ptr = PNULL;
    uint16 to_wstr_len; // string length

    genre_ptr = MBBMSSG_GetContentGenreIndex();
    if(PNULL != genre_ptr)
    {
        total_num = genre_ptr->node_cnt;
    }
    else
    {
    //if get the genre list fail,then add one type only:TXT_COMMON_ALL
        total_num = 1;
    }
    //SCI_TRACE_LOW:"MBBMS_AddAllContentTypeToDropdownListNew genre_ptr = 0x%x, total_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11202_112_2_18_2_40_12_536,(uint8*)"dd", genre_ptr, total_num);
    item_data_ptr = SCI_ALLOCA(total_num * sizeof(MMI_STRING_T));
    
    if(PNULL == item_data_ptr)
    {
        //SCI_TRACE_LOW:"MBBMS_AddAllContentTypeToDropdownListNew item_data_ptr = PNULL. \n "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11207_112_2_18_2_40_12_537,(uint8*)"");
        return;
    }
    
    for (index = 0; index < total_num; index++)
    {
        if(0 == index)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_ALL, &item_data_ptr[0]);   
        }
        else
        {
            content_type_ptr = (contentString*)STL_LinkListGetNodeDataByIdx(genre_ptr, (index - 1)); 
            if( SG_MAX_LANGUAGE_CONTENT_LEN < content_type_ptr->string_len)
            {
                content_type_string_ptr = content_type_ptr->content_string_ptr;
            }
            else
            {
                content_type_string_ptr = content_type_ptr->string_arr;
            }
            item_data_ptr[index].wstr_ptr = SCI_ALLOC_APP((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar));
            SCI_MEMSET(item_data_ptr[index].wstr_ptr, 0, ((DROPDOWNLIST_STRING_MAX_NUM + 1) * sizeof(wchar)));            
            //SCI_TRACE_LOW:"MBBMS_AddAllContentTypeToDropdownListNew content_type_string_ptr = 0x%x, string_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMBBMS_WINTAB_11230_112_2_18_2_40_12_538,(uint8*)"dd", content_type_string_ptr, content_type_ptr->string_len);
            if(PNULL != item_data_ptr[index].wstr_ptr)
            {
                to_wstr_len = MIN(content_type_ptr->string_len, DROPDOWNLIST_STRING_MAX_NUM);
                item_data_ptr[index].wstr_len = GUI_UTF8ToWstr(item_data_ptr[index].wstr_ptr, to_wstr_len,
                        content_type_string_ptr, to_wstr_len);
            }
        }
    }
    
    // add content type to dropdown list
    GUIDROPDOWNLIST_AppendItemArray(MMIMBBMS_DROPDOWNLIST_PROGRAM_TYPE_CTRL_ID, 
        item_data_ptr, total_num);

    for(index = 1; index < total_num; index++) // 因为index=0的时候取的是字符串资源的地址，不需要释放
    {
        if(PNULL != item_data_ptr[index].wstr_ptr)
        {
            SCI_FREE(item_data_ptr[index].wstr_ptr);
        }
    }
    
    SCI_FREE(item_data_ptr);
    
    return;
}

#endif



/*Edit by script, ignore 32 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
