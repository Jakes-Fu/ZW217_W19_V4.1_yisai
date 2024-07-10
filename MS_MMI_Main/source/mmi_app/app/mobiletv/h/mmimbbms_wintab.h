/*****************************************************************************
** File Name:      mmimbbs_wintab.h                                         *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv app                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008                  Create									 *
*****************************************************************************/
#ifdef MBBMS_SUPPORT
#ifndef  _MMIMBBMS_WINTAB_H_    
#define  _MMIMBBMS_WINTAB_H_   


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mbbms_sub.h"
#include "mmimbbms_main.h"

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/**---------------------------------------------------------------------------*
**                         Macro Declaration    							  *
**---------------------------------------------------------------------------*/
#define MMIMBBMS_SEARCH_MAX_WORDS            10
#define MMIMBBMS_NOTIFY_MAX_ITEMS            4
#define MMIMBBMS_MAX_PRICE_LENS              20

#define MMIMBBMS_MAX_KEYWORD_LENS        20
#define MMIMBBMS_FORM_VERTICAL_SPACE        20
#define MMIMBBMS_MAX_CITY_NAME_LENS        60

typedef enum
{
    //主界面选项部分
    MMIMBBMS_ID_ROOT,
    MMIMBBMS_ID_RETURN_PLAYING_MENU,
    MMIMBBMS_ID_CONTENT_LIST_MENU,
    MMIMBBMS_ID_ADD_FAVORITE_SERVICE_MENU,
    MMIMBBMS_ID_DEL_FAVORITE_SERVICE_MENU,
    MMIMBBMS_ID_PURCHASE_MANAGE_MENU,
    MMIMBBMS_ID_OPERATION_OPEN_MENU,
    MMIMBBMS_ID_SET_CONTENT_ALARM_MENU,
    MMIMBBMS_ID_CLOSE_CONTENT_ALARM_MENU,
    MMIMBBMS_ID_SEARCH_CONTENT_MENU,
    //MMIMBBMS_ID_OPERATION_MANAGE_MENU,
    MMIMBBMS_ID_UPDATE_MENU,
    MMIMBBMS_ID_EB_MENU,
    MMIMBBMS_ID_MANUAL_UPDATE_SG_MENU,
    MMIMBBMS_ID_MANUAL_GBA_OPERATION_MENU,
    MMIMBBMS_ID_SETTING_MENU,
    MMIMBBMS_ID_HELP_MENU,
    MMIMBBMS_ID_EXIT_MENU,    

    MMIMBBMS_ID_ADD_FAVORITE_CONTENT_MENU,
    MMIMBBMS_ID_DEL_FAVORITE_CONTENT_MENU,
    MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID,
    MMIMBBMS_MENU_PURCHASE_MANAGE_ITEM_ID,
    MMIMBBMS_MENU_SEARCH_PROGRAM_ITEM_ID,
//    MMIMBBMS_MENU_SERVICE_MANAGE_ITEM_ID,
    MMIMBBMS_MENU_SETTING_ITEM_ID,
    MMIMBBMS_MENU_NOTIFY_ITEM_ID,
    MMIMBBMS_ID_WATCH_MENU,
    MMIMBBMS_ID_OPEN_MENU,
    MMIMBBMS_ID_OK_MENU,
    MMIMBBMS_ID_CANCEL_MENU,
    MMIMBBMS_ID_EB_SORT_BY_TIME_MENU,
    MMIMBBMS_ID_EB_SORT_BY_GRADE_MENU,
    MMIMBBMS_ID_EB_DELETE_MENU,
    MMIMBBMS_ID_EB_DELETE_ALL_MENU
}MBBMS_OPTION_MENU_NODE_E;

typedef enum
{
	MMIMBBMS_OK,
	MMIMBBMS_GET_SERVICE_INDEX_ERROR,
	MMIMBBMS_GET_SERVICE_NODE_ERROR,
	MMIMBBMS_HAVE_FAVORITE,
	MMIMBBMS_NOT_FAVORITE,
	MMIMBBMS_NOT_OPEN_WIN,
	MMIMBBMS_MAX
} MMIMBBMS_RESULT;

typedef enum
{
	MMIMBBMS_OPER_ADD,
    MMIMBBMS_OPER_DEL,
	MMIMBBMS_OPER_MAX
}MMIMBBMS_OPER;

typedef enum
{
	MMIMBBMS_TYPE_SERVICE,
    MMIMBBMS_TYPE_CONTENT,
	MMIMBBMS_TYPE_MAX
}MMIMBBMS_TYPE;


typedef enum
{
	MMIMBBMS_SUBSCRIPTE_TYPE_PURCHASED,//已定套餐
      MMIMBBMS_SUBSCRIPTE_TYPE_LOCAL_PURCHASE, //本地套餐     
	MMIMBBMS_SUBSCRIPTE_TYPE_MAX
}MMIMBBMS_SUBSCRIPTE_TYPE;

typedef enum
{
    MMIMBBMS_UPDATE_TYPE_AUTO,
    MMIMBBMS_UPDATE_TYPE_ACCORDING_TO_CITY,
    MMIMBBMS_UPDATE_TYPE_MAX
}MMIMBBMS_UPDATE_TYPE;

   
/**---------------------------------------------------------------------------*
**                         Function Declare                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMIMBBMS_OpenSearchResultMainWin
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSearchResultMainWin(SG_LIST_PTR_T list_ptr);

/*****************************************************************************/
// 	Description : MMIMBBMS_FreeComplexInfo
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeComplexInfo(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_GetNameWchar
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMBBMS_GetNameWchar(SG_LIST_PTR_T namelist_ptr,
                                    wchar* wchar_name, uint16 wstr_len);

/*****************************************************************************/
//  Description : MMIMBBMS_OpenSetting win
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSettingMainWin(void);			          

/*****************************************************************************/
//  Description : 创建TAB主界面
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_OpenMainInterfaceWin(void);

/*****************************************************************************/
// 	Description : Open startup win 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenStartUpWin(void);


/*****************************************************************************/
// 	Description : Open Watch connecting win
//	Global resource dependence : none
//  Author: lisa
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenWatchConnectingWin(void);

/*****************************************************************************/
// 	Description : Open Subscrible prompt win under play
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscriblePromptWin(void);

/*****************************************************************************/
// 	Description : get Subscrible prompt text info
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetSubscriblePromptText(
                                             wchar *str_ptr     //out
                                             );

/*****************************************************************************/
//  Description : Open program list win
//  Global resource dependence : none
//  Author: 
//  Note:    
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenProgramListWin(void);

/*****************************************************************************/
// 	Description : open emergency broadcast msg list win
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenEBListWin(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_RefreshMainInterface
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_RefreshMainInterface(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_GetSelectedDate
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetSelectedDate(SCI_DATE_T* pDate, uint16 day_index);

/*****************************************************************************/
// 	Description : 频道索引是否有效
//	Global resource dependence : 
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsServiceIndexValid(uint16 index);

/*****************************************************************************/
// 	Description : 根据Index索引, 切换频道播放
//	Global resource dependence : 
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_PlayServiceByIndex(uint16 index);

/*****************************************************************************/
// 	Description : open Purchase Operation cancel waiting win
//	Global resource dependence : none
//  Author: xiaoxiang.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscribeUnsubscribeCancelWaitWin(BOOLEAN is_unpurchase_operation);
/*****************************************************************************/
// 	Description : MMIMBBMS_AddCurPlayServiceIntoFavoriteList
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_RESULT MMIMBBMS_AddCurPlayServiceIntoFavoriteList(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_DelCurPlayServiceFromFavoriteList
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_RESULT MMIMBBMS_DelCurPlayServiceFromFavoriteList(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_IsCurPlayServiceInFavoriteList
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsCurPlayServiceInFavoriteList(void);

/*****************************************************************************/
// 	Description : 获取当前的频道索引
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIMBBMS_GetPlayingServiceIndex(void);

/*****************************************************************************/
//  Description : 获取当前的频道索引对应的频道名
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetCurrServiceName(wchar* wchar_name,  //out
                                        uint32 wchar_len    //in
                                        );

/*****************************************************************************/
// Description : MMIMBBMS_AddPurchaseDetailHead
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddPurchaseDetailHead(wchar* wchar_name, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// Description : MMIMBBMS_AddPurchaseDetailHead
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddPurchaseDetailTail(MMIMBBMS_PURCHASEITEM_DISPLAY_INFO_T* purchase_display_info,
    MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : MMIMBBMS_GetStartTimerID
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIMBBMS_GetStartTimerID(void);

/*****************************************************************************/
// 	Description : MMIMBBMS_HandleStartTimer
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_HandleStartTimer(uint8 timer_id, uint32 param);


/*****************************************************************************/
// 	Description : Open window to query whether enter into cmmb or not after sg search failed
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubcribleRequestWin(void);

/*****************************************************************************/
// 	Description : enter play from service tab
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetServiceIndexByServiceID(anyURI* pServiceID);

/*****************************************************************************/
// 	Description : enter play from service tab
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetServiceIndexByContentID(anyURI* pContentID);

/*****************************************************************************/
// 	Description : Open window to query whether enter into cmmb or not after sg search failed
//	Global resource dependence : none
//  Author: lisa lin
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenUnSubcribleRequestWin(void);

/*****************************************************************************/
// 	Description : enter play via from_type
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void  MMIMBBMS_EnterPlayOrOpen (MMIMBBMS_OBJECT_FROM_TYPE_E obj_from_type);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void  MMIMBBMS_SetServiceIdex (uint16 service_index);

/*****************************************************************************/
// 	Description : handle Subscription Query win  msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscriptionUpdateQueryWin(void);

/*****************************************************************************/
// 	Description : open subscription update query wein by text
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSubscriptionUpdateQueryWinByTextPtr(MMI_STRING_T *text_ptr);

/*****************************************************************************/
// 	Description : open cmmb mode warning win
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenCMMBModeWarningWin(void);

/*****************************************************************************/
// 	Description : open swith to mbbms mode query win
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSwitchToMBBMSQueryWin(void);

/*****************************************************************************/
// 	Description : get preview description text
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_GetPreviewText(wchar* wchar_text, MBBMSSG_PREVIEW_DATA_T *data_info);

/*****************************************************************************/
// 	Description : get object type
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMBBMS_OBJECT_FROM_TYPE_E MMIMBBMS_GetObjectType(void);

/*****************************************************************************/
// 	Description : display init waiting alert win
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_DisplayInitWaitingWin(void);

/*****************************************************************************/
// 	Description : 设置StartUpWin异常关闭标志变量
//	Global resource dependence : none
//  Author: xin.li
//	Note: 变量初始为TRUE，正常关闭窗口时设为FALSE
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetStartUpWinCloseFlag(BOOLEAN flag_state);

/*****************************************************************************/
// 	Description : StartUpWin是否异常关闭
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsStartUpWinAbnormalClose(void);

/*****************************************************************************/
// 	Description : 向频道切换栏添加频道项
//	Global resource dependence : 
//  Author: xin.li
//	Note: 依据SG中的频道列表
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddServiceItemToTabCtrl(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 套餐订购管理
//  Global resource dependence : none
//  Author: 
//  Note: 包含两项：本地套餐，已订套餐
/*****************************************************************************/
PUBLIC MMIMBBMS_RESULT MMIMBBMS_OpenSubscribeTabMainWin(void);

/*****************************************************************************/
// Description : get program list info
// Global resource dependence :
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_GetProgramList(MTVSERVICE_PROGRAM_LIST_T* list_ptr);

/*****************************************************************************/
// Description : free program list info
// Global resource dependence :
// Author: xin.li
// Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_FreeProgramList(void );

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
                                     );

/*****************************************************************************/
//  Description : 打开选卡界面
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_OpenSimSelectWin(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
void MMIMBBMS_SetIfEnterSubFromPlay(BOOLEAN ret);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
BOOLEAN MMIMBBMS_GetIfEnterSubFromPlay(void);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif /* END _MMIMBBMS_WINTAB_H_  */
#endif
