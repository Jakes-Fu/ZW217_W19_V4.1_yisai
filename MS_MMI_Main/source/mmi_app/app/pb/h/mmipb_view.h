/*****************************************************************************
** File Name:      mmipb_app.h                                                   *
** Author:                                                                   *
** Date:           11/09/2011                                               *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_VIEW_H_
#define _MMIPB_VIEW_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_queue.h"
#include "mmipb_export.h"
#include "mmipb_adapt.h"
#include "mmipb_app.h"
#include "mmipb_menu.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
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
#define MMIPB_MAX_APPLET_COUNT         5//允许同时存在5个pb applet实体

#define MMIPB_MAX_ADD_IN_SIM_ITEM   2

#define MMIPB_MSISDN_MAX_COUNT  2

#define MMIPB_DROPDOWNLIST_FAVOURITE_INDEX   0
#define MMIPB_DROPDOWNLIST_RECENTUSED_INDEX   1
#define MMIPB_MOSTUSED_DROPDOWNLIST_ITEM_COUNT   2


#define MMIPB_CONTACTINDEX(X)   (X&0x0000FFFF) 
#define MMIPB_NUMINDEX(X)   (X>>16)

#define MMIPB_ENTRYID(X)   (X&0x0000FFFF) 
#define MMIPB_STORAGEID(X)   (X>>16)

#define MMIPB_ISSPLIT(X)     (X == 0XFFFFFFFF)
#define SPLIT_DATA          0XFFFFFFFF
#define SNS_DATA          0XFFFF0000
#define MMIPB_ISSNS(X)     ((X&SNS_DATA) == SNS_DATA)

#define MMIPB_CONVERT_GROUPID(X) 1<<X
#define SELECT_LABLE_LEN 55


        
#define SEL_ALL_BUTTON_WIDTH     50
#define SEL_ALL_TEXT_WIDTH       70        


#define PB_LIST_BUTTON_FORM_BG_COLOR  GUI_RGB2RGB565(234,234,234)
#ifdef MAINLCD_SIZE_320X480
#define PB_LIST_CONTACT_INFO_BAR_HEIGHT      62
#define PB_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 52
#define PB_LIST_CONTACT_INFO_BAR_MARGIN 5
#define PB_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#define PB_LIST_BUTTON_FORM_HEIGHT     52
#define PB_LIST_BUTTON_HEIGHT          40   
#elif (defined MAINLCD_SIZE_240X320) || (defined MAINLCD_SIZE_240X400)
#define PB_LIST_CONTACT_INFO_BAR_HEIGHT     50
#define PB_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 43
#define PB_LIST_CONTACT_INFO_BAR_MARGIN 3
#define PB_LIST_BUTTON_FORM_HEIGHT     45
#define PB_LIST_BUTTON_HEIGHT          40   
#define PB_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#else
#define PB_LIST_CONTACT_INFO_BAR_HEIGHT     50
#define PB_LIST_CONTACT_INFO_BAR_ICON_HEIGHT 43
#define PB_LIST_CONTACT_INFO_BAR_MARGIN 3
#define PB_LIST_BUTTON_FORM_HEIGHT     45
#define PB_LIST_BUTTON_HEIGHT          40   
#define PB_LIST_CONTACT_INFO_BAR_LABEL_WID (MMI_MAINSCREEN_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_HEIGHT-MMI_LIST_ITEM_SELECT_ICON_WIDTH-CALLTIME_LIST_CONTACT_INFO_BAR_MARGIN*2)
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIPB_COPY_FROM_INDEX,
    MMIPB_COPY_TO_INDEX 
}MMIPB_COPY_LOCATION_INDEX/*lint !e751*/;

typedef enum
{
  PB_GROUP_INDEX_NAME =0,
  PB_GROUP_INDEX_MANAGE,
  PB_GROUP_INDEX_RING,
  PB_INDEX_MAX
}MMIPB_PDA_INDEX_E;

typedef enum
{
      MMIPB_PDA_TOOLBAR_AUDIO_INDEX,
#ifdef VT_SUPPORT
      MMIPB_PDA_TOOLBAR_VIDEO_INDEX,
#endif        
      MMIPB_PDA_TOOLBAR_IP_INDEX,
      MMIPB_PDA_TOOLBAR_SMS_INDEX,
#ifdef MMIPB_MAIL_SUPPORT //MMIEMAIL_SUPPORT       
      MMIPB_PDA_TOOLBAR_EMAIL_INDEX,
#endif
}MMIPB_PDA_TOOLBAR_INDEX_E;
typedef enum
{
    MMIPB_FDN_ADD,
    MMIPB_FDN_EDIT,
    MMIPB_FDN_DETAIL,
    MMIPB_FDN_DEL
}MMIPB_FDN_ADD_MODE_E;
#ifdef SEARCH_SUPPORT
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
typedef struct _MMMIPB_LOCAL_SEARCH_DATA_T
{
    MMISEARCH_DATA_T searchdata;
    wchar dearch_str[255+1];
}MMIPB_LOCAL_SEARCH_DATA_T;
#endif
#endif

typedef struct _MMIPB_LIST_HANDLE
{
	MMI_WIN_ID_T win_id;

	MMIPB_CONTACTS_TYP_E contacts_type;
	MMIPB_NAME_T search_str;
	uint32 user_data;
	uint16 group_id;
}MMIPB_LIST_HANDLE;


typedef struct _MMIPB_NUMBER_LIST_T
{
    //MMIPB_PBLIST_TYPE_E    pblist_type;
    MMIPB_BCD_NUMBER_T number_t[MMIPB_MAX_NV_PHONE_NUM];
	uint16             opc_type;          
}MMIPB_NUMBER_LIST_T;


typedef struct _MMIPB_PB_LIST_DATA_T
{
    uint32*            win_table_ptr;
	MMI_WIN_ID_T	   win_id;
    ADD_DATA      add_data_ptr;        // the additional data
    BOOLEAN        b_need_parent_win;
}MMIPB_PB_LIST_DATA_T;


typedef void (* CREATE_WIN_FUNC)(MMI_WIN_ID_T win_id);

 
typedef struct _MMIPB_WINDOW_TAB_INFO_T
{
	MMI_WIN_ID_T    win_id;
	MMI_TEXT_ID_T   text_id;
	MMI_IMAGE_ID_T  tab_on_img_id;
	MMI_IMAGE_ID_T  tab_off_img_id;
	MMIPB_SUB_FUNCTION_TYPE_E  app_type;
	CREATE_WIN_FUNC create_func;
	
}MMIPB_WINDOW_TAB_INFO_T; 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : PB主入口函数，进入pb主窗口，根据参数，显示PB主列表
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenMainWin(
                                 MMIPB_SUB_FUNCTION_TYPE_E app_type,
                                 MMIPB_LIST_TYPE_E list_type, 
                                 MMIPB_LIST_FIELD_TYPE_E field_display_type,
                                 MMIPB_SEARCH_TYPE_T             search_type, 
                                 uint16 		max_select_count,                     
                                 const MMI_STRING_T  *str_input,
                                 MMIPB_RETURN_CALLBACK_FUNC callback
);

/*****************************************************************************/
// 	Description : 进入fdn 联系人列表窗口
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenFDNContactList(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : 打开pb应用的主页面
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN MMIPB_OpenPbWin(MMI_HANDLE_T  applet_handle);

/*****************************************************************************/
//  Description : update current list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateCurrentList(MMI_HANDLE_T applet_handle, MMIPB_OPC_E opc_type);

/*****************************************************************************/
//  Description : 判断是否有联系人已被标记api
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsMark(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIPB_SetMarkOne(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UNMarkAll(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 创建全部联系人页
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateAllContactTabWin(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 创建常用联系人页
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateMostUsedContactTabWin(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 创建联系人分组页
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateGroupListTabWin(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : write list mode
//	Global resource dependence : 
//  Author:maryxiao
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_ReadListMode(MMI_HANDLE_T applet_handle);

/*****************************************************************************/
// 	Description : write list mode
//	Global resource dependence : 
//  Author:maryxiao
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_WriteListMode(MMI_HANDLE_T applet_handle, uint32 mode);

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的小图标
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetSearchEntryIcon(uint32 group);


/*****************************************************************************/
//  Description : 获得列表中允许的最大可选数目
//  Global resource dependence : g_mmipb_table_info
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetListAllowableMaxSelectedNum
(
    MMI_HANDLE_T applet_handle
);

/*****************************************************************************/
//  Description :本机号码页面所点项是否存在有效号码
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsCurContactValid(uint16 contact_id, uint16 storage_id);

/*****************************************************************************/
//  Description : open widget favorite entry, just called by widget.
//  Global resource dependence :  g_mmipb_table_info
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenContactWin(MMI_HANDLE_T applet_handle, uint16 entry_id, uint16 storage_id);

/*****************************************************************************/
// Description : 解析VCard文件,并保存到PB中
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenVcardFileContactWin(uint16  *name_ptr);

/*****************************************************************************/
//  Description : 密码验证后，PB需要做的操作
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPB_DoOperationWithoutProtect
(
	void
);
/*****************************************************************************/
//  Description : 为MMS,sms打开记录列表窗口。
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 此函数供通话记录将号码保存到已有姓名时调用。
/*****************************************************************************/


PUBLIC void MMIPB_AddToExistContact
(
    void                          *   data_ptr,        //IN: 输入的电话号码
    MMIPB_ADD_TYPE_T       type,
    MMIPB_RETURN_CALLBACK_FUNC callback
);
/*****************************************************************************/
//  Description : 获得号码对应的icon id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIPB_GetIconIdOfPhone
(
    uint8 num_type  //IN:
);

/*****************************************************************************/
// 	Description : 开启进度窗口
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/

PUBLIC void  MMIPB_CloseProcessWin(MMI_HANDLE_T  applet_handle);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetCLUnProtect(void);
#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : PIN code verify ok
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_Pin2VerifyCnf(void);
#endif
 #ifdef FDN_SUPPORT
/*****************************************************************************/
// 	Description : 取得sim卡特定号码列表的窗口数组
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetDailNumberListWinTab(void);
#endif
/*****************************************************************************/
// 	Description : 启动不进入pb主页面的applet
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/

PUBLIC MMI_HANDLE_T MMIPB_START_APPLET_WITHOUT_WIN(MMIPB_INSTANCE_T* instance_ptr);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetCLUnProtect(BOOLEAN is_unprotect);
/*****************************************************************************/
//  Description : 打开窗口，显示PB全部记录
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenListAllGroupWin(void);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SwitchToPreTab(void);
#endif

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_IsUseDefaultName(wchar* name_ptr);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : Add contact to black list
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddToBlackList(MMI_HANDLE_T applet_handle);
#endif

/*****************************************************************************/
//  Description : 打开添加菜单的窗口。
//  Global resource dependence : g_mmipb_bcd_number_t
//  Author: bruce.chi
//  Note: 此函数供短消息和彩信提取号码并保存的时候调用；
//        供通话记录号码保存到电话簿中调用；供本模块调用。
//        供其它模块调用时，需要传入号码；供本模块调用时，不用传号码。
/*****************************************************************************/
PUBLIC void MMIPB_AddContactWin
(
    MMI_HANDLE_T applet_handle,//applet_handle为0，要重新创建新的applet,否则使用该applet
    void	*data_ptr,	    //IN: 输入的电话号码
    uint8   group_id,       //In:group id,
    uint16  storage_id,     //传入0，表示需要重新选择存储位置
    MMIPB_ADD_TYPE_T type,
    BOOLEAN     is_protect  
    
);
/*****************************************************************************/
// 	Description : 开启进度窗口
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC void  MMIPB_OpenProcessWin(MMI_HANDLE_T  applet_handle, 
                                  MMI_TEXT_ID_T text_id, 
                                  MMI_TEXT_ID_T title_id);

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenWaitWin(MMI_HANDLE_T applet_handle, 
                              MMI_TEXT_ID_T text_id);

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSnsUpdateWaitWin(MMI_HANDLE_T  applet_handle);
/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SnsNotLogin(MMI_HANDLE_T  applet_handle);
#endif
/*****************************************************************************/
//  Description : to open the memory detatil window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenMemDetail(MMI_HANDLE_T  applet_handle );
/*****************************************************************************/
//  Description : is cur item mark
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsCurItemMark(MMI_HANDLE_T applet_handle);

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开本机号码设置窗口
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenMSISDNWin(MMI_HANDLE_T applet_handle, BOOLEAN is_enter_from_callsetting);

#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenCopySelectWin(MMI_HANDLE_T applet_handle);
/*****************************************************************************/
//  Description : 进入群组窗口
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenGroupWin(MMI_HANDLE_T applet_handle);
/*****************************************************************************/
//  Description : maryxiao
//  Global resource dependence : g_mmipb_bcd_number_t
//  Author: maryxiao
//  Note: 打开单选列表窗口
/*****************************************************************************/
PUBLIC void MMIPB_OpenSingleSelWin(MMI_HANDLE_T applet_handle);

/*****************************************************************************/
//  Description :   获得本机号码
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetPBMSISDN(MN_DUAL_SYS_E dual_sys, 
            MMIPB_CONTACT_T *pb_msisdn_ptr);

#ifdef MMIPB_SIMDN_SUPPORT
/*****************************************************************************/
//  Description : 打开SDN 号码设置窗口
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenSDNWin(MMI_HANDLE_T applet_handle);
#endif
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : 解析VCard data,并打开该文件
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenVcardContactWin(uint8  *data_ptr, uint32 data_size);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 进入设置窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSetWin(MMI_HANDLE_T applet_handle);
#endif

/*****************************************************************************/
//  Description : 操作前打开选择窗口
//  Global resource dependence : g
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenOperationSelectWin(MMI_HANDLE_T applet_handle, MMIPB_MENU_DATA_INFOR_T *menu_ptr);

/*****************************************************************************/
//  Description : 创建详情基本资料子窗口
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC  void MMIPB_CreateDetailBasicDataChildWin(MMI_WIN_ID_T	parent_win_id);
#ifdef SNS_SUPPORT
PUBLIC  void MMIPB_CreateSNSDetailBasicDataChildWin(MMI_WIN_ID_T	parent_win_id);
PUBLIC  void MMIPB_CreatePBSnsActiveInfoChildWin(MMI_WIN_ID_T	parent_win_id);
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//	Description : Open contact msg list select win API
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_EnterDetailMsgListSelectWin(MMI_HANDLE_T app_handle);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 编辑群组
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EditUserGroup(MMI_HANDLE_T applet_handle, uint32 group_id);
#endif
/*****************************************************************************/
//  Description : 编辑群组名称
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EditUserGroupName(MMI_HANDLE_T applet_handle, uint32 group_id);
/*****************************************************************************/
//  Description : vcard中带保存按钮的详情页面
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note:  
//RETURN: 
/*****************************************************************************/
PUBLIC  void MMIPB_CreateOtherDetailBasicDataChildWin(MMI_WIN_ID_T	parent_win_id);
/*****************************************************************************/
//  Description : 打开pb 详情主界面
//  Global resource dependence : 
//  Author:
//  Note: is_other:  是否为非iphone风格中vcard带保存的页面
/*****************************************************************************/

PUBLIC BOOLEAN MMIPB_OpenDetailWin(MMI_HANDLE_T  applet_handle, MMIPB_CONTACT_T *contact_ptr, BOOLEAN is_other);

/*****************************************************************************/
//  Description : 删除用户自定义群组
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_DeleteUserGroup(MMI_HANDLE_T applet_handle, uint32 group_id);
/*****************************************************************************/
//  Description : 新增用户自定义群组
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_AddUserGroup(MMI_HANDLE_T applet_handle);

#ifdef MMIPB_SEARCH_CONTACT_CONTENT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdatePbSearching(void);

/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_LocalAsyncSearch(const MMI_STRING_T *key_word_str);

/*****************************************************************************/
//     Description : stop async search pb
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_StopPbAsyncSearch(void);
/*****************************************************************************/
//     Description : is pb search end
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPbAsyncSearchEnd(void);
#else
#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_LocalSearch(const MMI_STRING_T *key_word_str, MMI_HANDLE_T win_handle, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num);
#endif
#endif
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_FreeLocalSearchResource(void);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenAlertTipWin(MMI_HANDLE_T applet_handle, MMIPB_ERROR_E error,MMIPB_OPC_E opc_type);

/*****************************************************************************/
//  Description : 取得所以选择的记录个数，不区分group
//  Global resource dependence : s_mmipb_uuid_list
//  Author:
//  Return: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllSelectNum( 
							       MMI_HANDLE_T applet_handle
								   );

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : 创建蓝牙联系人页
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateBtContactTabWin(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : Get win id for bt sync win id
// 	Global resource dependence : 
// Author:
// 	Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIPB_GetActivateBTWinId(void);

/*****************************************************************************/
// 	Description : Refresh list after sync finished
// 	Global resource dependence : 
// Author:
// 	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_RefreshList(void);

/*****************************************************************************/
// 	Description : set pbap allow sync
//	Global resource dependence: 
//  Author:allen.lee
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SetAllowSync(BOOLEAN is_allow_sync);

/******************************************************************************/
// Description: MMIPB_StopPBAPTimer
// Global resource dependence: 
// Author: 
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIPB_StopPBAPTimer(void);
#endif
#ifdef   __cplusplus
    }
#endif

#endif
