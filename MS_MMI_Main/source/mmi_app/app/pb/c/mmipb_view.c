/****************************************************************************
** File Name:      mmipb_view.c                                                                  *
** Author:                                                                                              *
** Date:           06/11/2004                                                                      *
** Copyright:      2004 SpreadTrum, Incorporated. All Rights Reserved.              *
** Description:    This file is used to describe the window for phonebook            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2011        maryxiao        Create
** 
****************************************************************************/
#define _MMIPB_VIEW_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "guilistbox.h"
#include "guilabel.h"
#include "guitoolbar.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_timer.h"
//#include "mmipb_internal.h"
#include "mn_api.h"
#include "mmi_text.h"
#include "mmifmm_text.h"
#include "mmisms_text.h"
#include "mmidisplay_data.h"
#include "mmisms_export.h"
#include "mmicc_export.h"
#include "mmicc_image.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmimms_export.h"
#include "guitext.h"
#include "mmipb_search.h"
//#include "mmipb_menutable.h"
#include "mmipb_id.h"
#include "mmipb_image.h"
#include "mmipb_text.h"
#include "guibutton.h"
#include "mmipb_id.h"
#include "mmipub.h"
#include "mmifmm_export.h"
//#include "mmipb_position.h"
#include "guirichtext.h"
#include "mmivcard_export.h"
#include "mmiocr_export.h"
#include "guitab.h"
#include "mmiudisk_export.h"
#include "mmibt_text.h"
#include "mmicc_text.h"
#include "mmiidle_export.h"
#include "mmivcard_export.h"
#include "mmisd_export.h"
#include "mmi_mainmenu_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "guitips.h"
#ifdef FDN_SUPPORT
#include "mmifdn_export.h"
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "window_parse.h"
#include "mmi_default.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmimms_text.h"
#include "mmi_appmsg.h"
#if ((!defined(MMI_PDA_SUPPORT)) && defined(MMIPB_TOOLBAR_SUPPORT)) ||( defined TOUCH_PANEL_SUPPORT)
#include "guictrl_api.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef PIM_SUPPORT
#include "mmipim_export.h"
#endif
#include "guiform.h"
#include "guires.h"
#include "guistring.h"

//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#if (defined MMIWIDGET_SUPPORT ) || (defined MMI_GRID_IDLE_SUPPORT )
#include "mmiwidget_export.h"
#include "mmiwidget_image.h"
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT 
#include "mmiphonesearch_export.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#include "guiurl.h"
#include "mmicl_export.h"

//+CR206252
#ifdef PUSH_EMAIL_SUPPORT
#include "mmimail_text.h"
#include "mmimail_export.h"
#include "mmimail_internal.h"
#endif
#include "mmiim.h"
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#include "mmivc_common.h"
#include "mmisms_id.h"
#include "mmiset_text.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef MMI_PDA_SUPPORT  
#include "mmicc_id.h"
#include "mmicc_image.h"
#endif
#include "guiurl.h"

#include "mmipb_set.h"
#include "mmipb_view.h"
#include "mmipb_storage.h"
#include "mmi_image.h"
#include "mmi_module.h"
#include "mmieng_text.h"
#include "guidropdownlist.h"
#ifdef ATV_SUPPORT
#include "mmiatv_text.h"
#endif
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#include "mmisns_sqlite.h"
#endif
#include "mmipb_menu.h"
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif

#include "mmipb_common.h"
#include "mmibt_text.h"
#ifdef BROWSER_SUPPORT
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h" 
#endif
#ifdef BROWSER_SUPPORT_DORADO
#include "browser_image.h"
#include "browser_text.h"
#endif
#endif
#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif
#include "mmi_resource.h"
#include "mmipb_mem.h"
#include "mmicc_image.h"
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
#include "mmipb_task.h"
#endif

#include "mmibt_id.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPB_TOOLBAR_ICON_GAP_TOP      5
#define MMIPB_TOOLBAR_ICON_GAP_RIGHT    10
#define MMIPB_TOOLBAR_PANEL_GAP         25
#define  MMIPB_VIBRA_DEFAULT_TIME           1500

BOOLEAN g_is_email_by_pb = FALSE;

#ifdef BT_DIALER_SUPPORT
#ifdef BT_PBAP_SUPPORT
#define BT_PBAP_TIMER_OUT            (30*1000)       //同步联系人请求响应过时保护时长
LOCAL uint8 s_pbap_timeout_timer = 0;
LOCAL MMI_WIN_ID_T s_bt_tab_win_id;//bt tab win id
LOCAL BOOLEAN s_is_focus_bt_contacts = FALSE;
LOCAL BOOLEAN s_is_sync_contacts = TRUE;
LOCAL BOOLEAN s_is_fullpaint_sync_once = FALSE;
#endif
#endif

typedef enum
{
    MMIPB_NEED_LIST_ITEM_DATA_TYPE,       //need data
    MMIPB_NEED_LIST_ITEM_CONTENT_TYPE    //need content
} MMIPB_LIST_ITEM_TYPE_E;

typedef enum
{
    MMIPB_FILTER_ALL,
    MMIPB_FILTER_NONE,

#ifdef CMCC_UI_STYLE
    MMIPB_FILTER_SIM,
    MMIPB_FILTER_PHONE,
#endif

    MMIPB_FILTER_MAX
} MMIPB_FILTER_TYPE_E;

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
//SIM特定特定号码列表信息
typedef struct  
{
    MN_DUAL_SYS_E       dual_sys;//第几个卡，如果是MMI_DUAL_SYS_MAX表示所有卡
    PHONEBOOK_STORAGE_E sim_contact_type;//sim卡号码类型:MSISDN，FDN，SDN...
    BOOLEAN             is_need_pin_verify;//是否需要PIN2码，主要用在fdn操作，其他情况都为false
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    BOOLEAN     is_enter_from_callsetting;//是否从callsetting模块进入MSISDN
#endif
}MMIPB_SIM_NUMBER_LIST_INFO_T;
#endif
LOCAL const MMI_IMAGE_ID_T      sim_icon_id_arr[] = {
                                        #ifndef MMI_MULTI_SIM_SYS_SINGLE
                                        IMAGE_SIM1, 
                                        IMAGE_SIM2, 
                                        #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
                                        IMAGE_SIM3,
                                       #endif
                                       #if defined(MMI_MULTI_SIM_SYS_QUAD)
                                        IMAGE_SIM4,
                                       #endif
                                        #else
                                        IMAGE_PHONEBOOK_SIM,
                                        #endif                                   
                                        };
LOCAL const MMI_TEXT_ID_T s_number_text_id[MMIPB_MAX_NV_PHONE_NUM] = 
{
    TXT_PB_ITEM_DETAIL_MOBILE_PHONE,
    TXT_PB_ITEM_DETAIL_HOME_PHONE, 
#ifndef PB_SUPPORT_LOW_MEMORY    
	TXT_PB_ITEM_DETAIL_OFFICE_PHONE,
    TXT_PB_FAX_NUMBER,
    TXT_PB_PHONE_NUMBER,
#endif

};

const MMI_IMAGE_ID_T s_photo_image_id[MMIPB_MAX_GROUP_NUM] =
{
    IMAGE_PB_LIST_NV_ICON,  
//    IMAGE_PHONEBOOK_GROUP_UNCLASS, 
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    IMAGE_PB_LIST_SIM1_ICON, 
    IMAGE_PB_LIST_SIM2_ICON, 
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    IMAGE_PB_LIST_SIM3_ICON,  
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    IMAGE_PB_LIST_SIM4_ICON,  
#endif
#endif
#else
    IMAGE_PB_LIST_SIM_ICON,
#endif
    IMAGE_PB_LIST_NV_ICON,////家庭
    IMAGE_PB_LIST_NV_ICON, ////朋友  
    IMAGE_PB_LIST_NV_ICON, //同事
    IMAGE_PB_LIST_NV_ICON, //同学
#ifdef MMIPB_MOST_USED_SUPPORT     
    IMAGE_PB_LIST_NV_ICON,//mostused
    IMAGE_PB_LIST_NV_ICON,//favourite
#endif
#ifdef BT_PBAP_SUPPORT
    IMAGE_PB_LIST_NV_ICON, //蓝牙联系人
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT 
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
#ifndef MMI_PDA_SUPPORT
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
    IMAGE_PB_LIST_NV_ICON,//自定义群组图标
#endif
#endif
#ifdef SNS_SUPPORT
    IMAGE_PB_LIST_SINA_ICON, //sina  
    IMAGE_PB_LIST_TWITTER_ICON, //twitter
    IMAGE_PB_LIST_FACEBOOK_ICON,// facebook
#endif
}; 



const MMI_IMAGE_ID_T s_big_image_id[MMIPB_MAX_GROUP_NUM] =
{
    IAMGE_PB_DEFAULT_PHOTO,  
//    IMAGE_PHONEBOOK_GROUP_UNCLASS, 
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    IMAGE_PHONEBOOK_SIM1, 
    IMAGE_PHONEBOOK_SIM2, 
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    IMAGE_PHONEBOOK_SIM3,  
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    IMAGE_PHONEBOOK_SIM4,  
#endif
#endif
#else
    IMAGE_PHONEBOOK_SIM,
#endif
    IAMGE_PB_DEFAULT_PHOTO,////家庭
    IAMGE_PB_DEFAULT_PHOTO, ////朋友  
    IAMGE_PB_DEFAULT_PHOTO, //同事
    IAMGE_PB_DEFAULT_PHOTO, //同学
#ifdef MMIPB_MOST_USED_SUPPORT     
    IAMGE_PB_DEFAULT_PHOTO,//mostused
    IAMGE_PB_DEFAULT_PHOTO,//favourite
#endif
#ifdef BT_PBAP_SUPPORT
    IAMGE_PB_DEFAULT_PHOTO, //蓝牙联系人
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT 
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
#ifndef MMI_PDA_SUPPORT
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
    IAMGE_PB_DEFAULT_PHOTO,//自定义群组图标
#endif
#endif
#ifdef SNS_SUPPORT
    IMAGE_PHONEBOOK_SINA, //sina  
    IMAGE_PHONEBOOK_TWITTER, //twitter
    IMAGE_PHONEBOOK_FACEBOOK,// facebook
#endif
}; //pclint error 785



const MMI_IMAGE_ID_T s_group_image_id[MMIPB_MAX_GROUP_NUM] =
{
    IMAGE_PHONEBOOK_GROUP1,  
//    IMAGE_PHONEBOOK_GROUP_UNCLASS, 
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    IMAGE_SIM1, 
    IMAGE_SIM2, 
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    IMAGE_SIM3,  
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    IMAGE_SIM4,  
#endif
#endif
#else
    IMAGE_PHONEBOOK_SIM,
#endif
    IMAGE_PHONEBOOK_GROUP1,//家庭
    IMAGE_PHONEBOOK_GROUP2, //朋友    
    IMAGE_PHONEBOOK_GROUP3, //同事
    IMAGE_PHONEBOOK_GROUP4, //同学
#ifdef MMIPB_MOST_USED_SUPPORT     
    IMAGE_PHONEBOOK_GROUP_MOSTUSED,//mostused
    IMAGE_PHONEBOOK_GROUP_MOSTUSED,//favourite
#endif
#ifdef BT_PBAP_SUPPORT
    IMAGE_PHONEBOOK_GROUP1,  //蓝牙联系人
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT 
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
#ifndef MMI_PDA_SUPPORT
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
    IMAGE_PHONEBOOK_GROUP_OWNDEF,//自定义群组图标
#endif
#endif
#ifdef SNS_SUPPORT
    IMAGE_PHONEBOOK_GROUP_SINA, //sina  
    IMAGE_PHONEBOOK_GROUP_TWITTER, //twitter
    IMAGE_PHONEBOOK_GROUP_FACEBOOK,// facebook
#endif

}; //pclint error 785

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error

LOCAL MMIPB_WINDOW_TAB_INFO_T s_mmipb_detail_win_tab[] = 
{

    {
	   MMIPB_ENTRY_DETAILS_WIN_ID,
           TXT_PB_DETAIL_BASICDATA,
	   IMG_PB_ALL_CONTACT_ICON_ON,
	   IMG_PB_ALL_CONTACT_ICON_OFF,
	   0,
	   MMIPB_CreateDetailBasicDataChildWin
	},


	//CallLog:通话纪录页
	{
	   MMICL_PB_DETAIL_CALLLOG_WIN_ID,
          STXT_MAIN_CL,
	   IMG_PB_CALLLOG_ICON_ON,
	   IMG_PB_CALLLOG_ICON_OFF,
	   0,
	   MMICL_CreatePBDetailLogChildWin
	},	
	//Message:信息页
	{
            MMIPB_DETAILS_MESSAGE_LIST_TAB_WIN_ID,
            TXT_PB_DETAIL_MESSAGE,
            IMAGE_PHONEBOOK_TAB_GROUP_ON,
            IMAGE_PHONEBOOK_TAB_GROUP_OFF,
            0,
            MMICL_CreateDetailMessageChildWin
	},
};
#ifdef SNS_SUPPORT
LOCAL MMIPB_WINDOW_TAB_INFO_T s_mmipb_sns_detail_win_tab[] = 
{
    //Basic data:基本资料页
    {
        MMIPB_SNS_ENTRY_DETAILS_WIN_ID,
        TXT_PB_DETAIL_BASICDATA,
        IMG_PB_ALL_CONTACT_ICON_ON,
        IMG_PB_ALL_CONTACT_ICON_OFF,
        0,
        MMIPB_CreateSNSDetailBasicDataChildWin
	},
	
    //Activity data:活动信息页
	{
	   MMIPB_SNS_ACTIVEINFO_WIN_ID,
          TXT_PB_SNS_ACTIVITY,
	   IMG_PB_MENU_SEARCH_ICON,
	   IMG_PB_MENU_SEARCH_ICON,
	   0,
	   MMIPB_CreatePBSnsActiveInfoChildWin
	},

};
#endif
LOCAL MMIPB_WINDOW_TAB_INFO_T s_mmipb_special_detail_win_tab[] = 
{
    //Basic data:基本资料页
    {
	   MMIPB_SPECIAL_ENTRY_DETAILS_WIN_ID,
       TXT_PB_DETAIL_BASICDATA,
	   IMG_PB_ALL_CONTACT_ICON_ON,
	   IMG_PB_ALL_CONTACT_ICON_OFF,
	   0,
	   MMIPB_CreateDetailBasicDataChildWin
	}


};

LOCAL MMIPB_WINDOW_TAB_INFO_T s_mmipb_special_other_detail_win_tab[] = 
{
    //Basic data:基本资料页
    {
	   MMIPB_SPECIAL_ENTRY_DETAILS_WIN_ID,
       TXT_PB_DETAIL_BASICDATA,
	   IMG_PB_ALL_CONTACT_ICON_ON,
	   IMG_PB_ALL_CONTACT_ICON_OFF,
	   0,
	   MMIPB_CreateOtherDetailBasicDataChildWin
	}


};
#endif

#ifdef MMI_PDA_SUPPORT
//职能风格的通讯录窗口页数组
MMIPB_WINDOW_TAB_INFO_T s_mmipb_pda_win_tab[] = 
{

	//Dial Pannel:拨号盘页
	{
	   MMIIDLE_DIAL_WIN_ID,
       TXT_DIAL,
	   IMG_PB_DIAL_ICON_ON,
	   IMG_PB_DIAL_ICON_OFF,
	   MMIPB_DIAL_APPLICATION,
	   MMIAPIIDLE_CreateDialWin
	},
	//CallLog:通话纪录页
	{
	   MMICL_ALL_CHILD_WIN_ID,
       STXT_MAIN_CL,
	   IMG_PB_CALLLOG_ICON_ON,
	   IMG_PB_CALLLOG_ICON_OFF,
	   MMIPB_CALLLOG_APPLICATION,
	   MMICL_CreateAllLogChildWin
	},	

	//All Contact:所有联系人页
    {
	   MMIPB_ENTRY_LIST_TAB_WIN_ID,
       STXT_MAIN_PB,
	   IMG_PB_ALL_CONTACT_ICON_ON,
	   IMG_PB_ALL_CONTACT_ICON_OFF,
	   MMIPB_ALL_CONTACT,
	   MMIPB_CreateAllContactTabWin
	}
#ifdef MMIPB_MOST_USED_SUPPORT
    ,
	//MOSTUSED Contact:常用联系人页
    {
		MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID,
	    TXT_PB_FAVOURITE,
        IMG_PB_MOSTUSED_CONTACT_ICON_ON,
		IMG_PB_MOSTUSED_CONTACT_ICON_OFF,
		MMIPB_MOSTUSED_CONTACT,
		MMIPB_CreateMostUsedContactTabWin
    }
#endif
};
MMIPB_WINDOW_TAB_INFO_T s_mmipb_pda_email_select_win_tab[] = 
{
	//All Contact:所有联系人页
    {
	   MMIPB_ENTRY_LIST_TAB_WIN_ID,
       STXT_MAIN_PB,
	   IMG_PB_ALL_CONTACT_ICON_ON,
	   IMG_PB_ALL_CONTACT_ICON_OFF,
	   MMIPB_ALL_CONTACT,
	   MMIPB_CreateAllContactTabWin
	},
};

MMIPB_WINDOW_TAB_INFO_T s_mmipb_pda_select_win_tab[] = 
{
	//All Contact:所有联系人页
    {
	   MMIPB_ENTRY_LIST_TAB_WIN_ID,
       STXT_MAIN_PB,
	   IMG_PB_ALL_CONTACT_ICON_ON,
	   IMG_PB_ALL_CONTACT_ICON_OFF,
	   MMIPB_ALL_CONTACT,
	   MMIPB_CreateAllContactTabWin
	},
	//CallLog:通话纪录页
	{
	   MMICL_ALL_SELECT_CHILD_WIN_ID,
       STXT_MAIN_CL,
	   IMG_PB_CALLLOG_ICON_ON,
	   IMG_PB_CALLLOG_ICON_OFF,
	   MMIPB_CALLLOG_APPLICATION,
	   MMICL_CreateAllLogSelectChildWin  //MMICL_CreateAllLogChildWin
	},	
	//Group:联系人分组也
	{
		MMIPB_GROUP_LIST_WIN_ID,
		TXT_PB_GROUP,
		IMAGE_PHONEBOOK_TAB_GROUP_ON,
		IMAGE_PHONEBOOK_TAB_GROUP_OFF,
		MMIPB_GROUP_CONTACT,
		MMIPB_CreateGroupListTabWin
	}
#ifdef MMIPB_MOST_USED_SUPPORT
    ,
	//MOSTUSED Contact:常用联系人页
    {
		MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID,
	    TXT_PB_FAVOURITE,
        IMG_PB_MOSTUSED_CONTACT_ICON_ON,
		IMG_PB_MOSTUSED_CONTACT_ICON_OFF,
		MMIPB_MOSTUSED_CONTACT,
		MMIPB_CreateMostUsedContactTabWin
    }
#endif
};
#if defined MMI_ISTYLE_SUPPORT
//职能单键风格的通讯录窗口页数组
MMIPB_WINDOW_TAB_INFO_T s_mmipb_istyle_win_tab[] = 
{
#ifdef MMIPB_MOST_USED_SUPPORT
    //MOSTUSED Contact:常用联系人页
    {
        MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID,
        TXT_PB_FAVOURITE,
        ISTYLE_IMG_PB_MOSTUSED_CONTACT_ICON_ON,
        ISTYLE_IMG_PB_MOSTUSED_CONTACT_ICON_OFF,
        MMIPB_MOSTUSED_CONTACT,
        MMIPB_CreateMostUsedContactTabWin
    },
#endif
    //CallLog:通话纪录页
    {
        MMICL_ALL_CHILD_WIN_ID,
        STXT_MAIN_CL,
        ISTYLE_IMG_PB_CALLLOG_ICON_ON,
        ISTYLE_IMG_PB_CALLLOG_ICON_OFF,
        MMIPB_CALLLOG_APPLICATION,
        MMICL_CreateAllLogChildWin
    },
    //All Contact:所有联系人页
    {
        MMIPB_ENTRY_LIST_TAB_WIN_ID,
        TXT_COMMON_ALL,
        ISTYLE_IMG_PB_ALL_CONTACT_ICON_ON,
        ISTYLE_IMG_PB_ALL_CONTACT_ICON_OFF,
        MMIPB_ALL_CONTACT,
        MMIPB_CreateAllContactTabWin
    },  
    //Dial Pannel:拨号盘页
    {
        MMIIDLE_DIAL_WIN_ID,
        TXT_DIAL,
        ISTYLE_IMG_PB_DIAL_ICON_ON,
        ISTYLE_IMG_PB_DIAL_ICON_OFF,
        MMIPB_DIAL_APPLICATION,
        MMIAPIIDLE_CreateDialWin
    }
};
#endif
#else
//普通风格的通讯录窗口页数组
MMIPB_WINDOW_TAB_INFO_T s_mmipb_normal_win_tab[] = 
{
	//All Contact:所有联系人页
    {
	   MMIPB_ENTRY_LIST_TAB_WIN_ID,
       TXT_COMMON_ALL,
	   IMAGE_PHONEBOOK_TAB_ALL_ON,
	   IMAGE_PHONEBOOK_TAB_ALL_OFF,
	   MMIPB_ALL_CONTACT,
	   MMIPB_CreateAllContactTabWin
	},
#ifdef MMIPB_MOST_USED_SUPPORT
	//MOSTUSED Contact:常用联系人页
    {
		MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID,
	    TXT_PB_MOSTUSED_GROUP,
        IMAGE_PHONEBOOK_TAB_MOSTUSED_ON,
		IMAGE_PHONEBOOK_TAB_MOSTUSED_OFF,
		MMIPB_MOSTUSED_CONTACT,
		MMIPB_CreateMostUsedContactTabWin
    },
#endif
#ifdef BT_PBAP_SUPPORT
    //BT Contact:蓝牙联系人页
	{
		MMIPB_BT_CONTACTS_LIST_TAB_WIN_ID,
		TXT_PB_BT,
		IMAGE_BT_CONTACT_TAB_ON,
		IMAGE_BT_CONTACT_TAB_OFF,
		MMIPB_BT_CONTACT,
		MMIPB_CreateBtContactTabWin
	},
#endif
	//Group:联系人分组也
	{
		MMIPB_GROUP_LIST_WIN_ID,
		TXT_PB_GROUP,
		IMAGE_PHONEBOOK_TAB_GROUP_ON,
		IMAGE_PHONEBOOK_TAB_GROUP_OFF,
		MMIPB_GROUP_CONTACT,
		MMIPB_CreateGroupListTabWin
    }

};
#endif
    //CallLog:通话纪录页
	//Group:联系人分组也
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void AppendTextAndtEXTListItem(MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            MMI_STRING_T     text_str2,
                            uint32           item_type,
							uint16           item_group,
							BOOLEAN          is_content_check,
							BOOLEAN          is_expand
                            );
#endif

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : 根据操作类型得到title
//  Global resource dependence : g
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTitleFromOperate(MMIPB_OPERATE_TYPE_E operate_type);

/*****************************************************************************/
//  Description : 根据号码类型得到该类型的text id
//  Global resource dependence 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetContactTypeText(PHONEBOOK_STORAGE_E contact_type);

/*****************************************************************************/
// 	Description : 处理操作回传结果后的刷新和提示操作
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
LOCAL void s_mmipb_update_callback(MMI_HANDLE_T applet_handle, uint16  msg_id,MMIPB_ERROR_E error,MMIPB_OPC_E opc_type, uint16 count);

/*****************************************************************************/
// 	Description : 处理操作回传结果后的刷新和提示操作
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void UpdateCallbackWithCloseApplet(MMI_HANDLE_T applet_handle, uint16  msg_id,MMIPB_ERROR_E error,MMIPB_OPC_E opc_type, uint16 count);

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : 将电话簿记录中的姓名设置为LISTBOX控件中的一个ITEM
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

LOCAL void MMIPB_SetEntryNameToListItem
(
                                         MMI_CTRL_ID_T       ctrl_id,
                                         MMI_TEXT_ID_T       left_softkey_id,
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
                                         BOOLEAN  is_enter_from_callsetting, 
#endif
                                         MMIPB_NAME_T        *name_ptr,
                                         uint32              user_data
);

/*****************************************************************************/
//  Description : 打开sim dail number的列表窗口
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSIMDNListWin(MMI_HANDLE_T  applet_handle);

/*****************************************************************************/
//  Description : 加载SIM 特定号码列表
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void LoadSIMDailNumberList(MN_DUAL_SYS_E dual_sys, //if dual_sys ==MMI_DUAL_SYS_MAX, means all sim 
                                 PHONEBOOK_STORAGE_E contact_type, 
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
                                 BOOLEAN is_enter_from_callsetting,
#endif
                                 MMI_CTRL_ID_T ctrl_id);

#endif
#ifdef MMI_PDA_SUPPORT
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
//  Description : CreateNewWinSoftKey
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateNewWinSoftKey(MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id);
#endif
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 通话记录中设置号码类型的列表控件
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetNumTypeListboxForCalllog(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
#endif
/*****************************************************************************/
//  Description : 设置用于分组管理列表控件的item
//  Global resource dependence :
//  Author: MARYXIAO
//  Note: 目前为一次将所有记录装入
/*****************************************************************************/
LOCAL void SetEntryListItemForEx
(
    MMI_WIN_ID_T    win_id,  
    MMI_HANDLE_T    applet_handle,  
    MMI_CTRL_ID_T    ctrl_id,       //IN: 对应控件的ID
    void *item_ptr,
    MMIPB_LIST_ITEM_TYPE_E dync_type,
    BOOLEAN is_need_number
);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByDualSys(MN_DUAL_SYS_E dual_sys);

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 为通话记录打开的选择号码类型的窗口
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL void OpenSelectNumTypeWin
(
    MMI_HANDLE_T applet_hanlde,
    MMI_CTRL_ID_T ctrl_id
);
#endif

/*****************************************************************************/
//  Description : 处理toolbar 点击后的操作
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void HandleToolBarAction(MMI_WIN_ID_T win_id, 
                               MMI_HANDLE_T list_handle,
                               PB_OPTMENU_NODE_E node_id,
                               MMIPB_CONTACT_T*  detail_contact_ptr,
                               BOOLEAN is_use_first);

/*****************************************************************************/
//  Description : update
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
LOCAL void HandleResultForEntryEx(MMI_HANDLE_T applet_handle,  uint16  msg_id, MMIPB_ERROR_E error ,MMIPB_OPC_E opc_type, uint16 count);
#endif
/*****************************************************************************/
//  Description : 在通话记录中将号码加到已有姓名列表窗口的处理
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryListForExWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOperateSelWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 处理全选按钮操作
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL   void HandleSelectAllFunc(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, MMI_HANDLE_T select_all_button_handle);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note:  OUT :contact_ptr
//          IN win_id
//          IN ctrl_id
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_GetContactInfoFromList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIPB_CONTACT_T* contact_ptr);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : Get  Valid Number count
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetValidPhoneNumberCount(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count);
#endif

#if defined (TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DealWithTPButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, PB_CONTROL_ID_E button_id);
#endif
/*****************************************************************************/
//  Description : Get  Valid Number count
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetValidNumberCount(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL wchar* MMIPB_Wstrupper( MMI_STRING_T * string );
#endif

/*****************************************************************************/
//  Description : 从电话簿中获得显示的字符串
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 用于装载列表控件的item时使用
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayStringFromPhonebookName(
    MMI_STRING_T        *dist_ptr,  //OUT: 获得应该显示的字符串
    const MMIPB_NAME_T  *name_ptr   //IN: entry data
);

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : FDN记录列表显示窗口
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMDNListWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif

#if defined (MMI_PDA_SUPPORT) && defined (HOME_LOCATION_INQUIRE_SUPPORT)
/*****************************************************************************/
//  Description :CC_FindLocationInforl 
//  Global resource dependence : none
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetNumberLocation(wchar *str, //OUT:
							 const uint8* tel_num, //IN
							 uint8 tel_num_len //IN
							 );
#endif

#ifdef MMI_PDA_SUPPORT
 /*****************************************************************************/
//  Description : 切换到相应应用的窗口 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SwitchToAppTab(MMI_HANDLE_T applet_handle,MMIPB_SUB_FUNCTION_TYPE_E app_type);
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : get curren application type 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_SUB_FUNCTION_TYPE_E GetCurAppType(MMI_HANDLE_T applet_handle, uint32 cur_index);
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description :SET WIN TITLE RIGHT ICON
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetPbTitleIcon(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T image_up, MMI_IMAGE_ID_T image_down, BOOLEAN is_visiable);
#endif
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 取得下一个group id
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetNextGroupID(uint8 cur_group_id);
/*****************************************************************************/
//  Description : 取得上一个group id
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetPreviousGroupID(uint8 cur_group_id);
#endif
/*****************************************************************************/
//  Description :append detail list anim
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
//        
/*****************************************************************************/
LOCAL BOOLEAN  AppendDetailDyncPhoto(MMI_HANDLE_T ctrl_id, uint16 index,MMIPB_CONTACT_T* contact_ptr, void *item_ptr);

#if defined(TOUCH_PANEL_SUPPORT) && defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
/*****************************************************************************/
//  Description : Update EntryList Toolbar State 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdateEntryListToolbarState( 
                                 MMI_HANDLE_T        ctrl_handle,
                                 void*               param_ptr
                                 );

#ifdef GUIF_TOOLBAR
/*****************************************************************************/
//  Description : CREATE PB Toolbar 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void PbCreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T list_id);    

#endif
/*****************************************************************************/
//  Description : UPDATE PB Toolbar STATUS
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdatePbListWinToolbar(
                            MMI_CTRL_ID_T      list_ctrl,
                            MMI_HANDLE_T      toolbar_ctrl_handle
                            );                             
  
#endif
/*****************************************************************************/
//  Description : 配置tab窗口所有控件
//  Global resource dependence : g_mmipb_table_info
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitSubTabControls(MMI_WIN_ID_T win_id, //IN:tab窗口id
								MMIPB_LIST_HANDLE   *handle_ptr//纪录列表信息
								);                               
 /*****************************************************************************/
//  Description : 处理PB的操作的消息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );
/*****************************************************************************/
//  Description : 设置多选列表控件的item
//  Global resource dependence : g_mmipb_table_info
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetListItem //RETURN: 实际设置的个数
(
    MMI_WIN_ID_T       win_id,
    uint32             total_count,//IN:
    uint16             storage_id, //IN:
    uint16             search_len,//IN:
    MMI_CTRL_ID_T      ctrl_id        //IN: 对应控件的ID
);

/*****************************************************************************/
//  Description : 搜索新增pb contacts条目
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_SearchAndAppendItem(MMI_HANDLE_T    list_handle, 
                                     MMI_HANDLE_T    applet_handle ,
                                     uint8            group_id,
                                     uint16           storage_id,
                                     GUIITEM_STYLE_E  listitem_type,
                                     MMIPB_LIST_FIELD_TYPE_E field_type);
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 搜索mostused contacts条目
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_MostUsedSearchAndAppendItem(MMI_HANDLE_T list_handle, 
                                             MMI_HANDLE_T applet_handle, 
                                             GUIITEM_STYLE_E  listitem_type);


#endif
/*****************************************************************************/
//  Description : 得到pb list的显示样式
//  Global resource dependence : g_mmipb_table_info
//  Author: MARY.XIAO
//  Note: 
/*****************************************************************************/
LOCAL GUIITEM_STYLE_E MMIPB_GetListStyle(MMIPB_INSTANCE_T *pb_instance_ptr);
/*****************************************************************************/
//  Description : 根据要显示的联系人列表的类型，获得一条联系人要显示成多少条的个数
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetValidContactNum(
                                 MMIPB_LIST_FIELD_TYPE_E type, //IN
                                 MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T contact_valid_num_mail_info, //IN
                                 MMIPB_CONTACT_BASE_INFO_T contact_node, ////IN
                                 uint8* num_arry// //OUT : VALID NUM OR MAIL INDEX
                                 );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : pda PB列表的窗口处理函数
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandlePDAEntryListWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );
#else
/*****************************************************************************/
//  Description : PB列表的窗口处理函数
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleEntryListWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );
#endif

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : Registers the Phonebook Access Client
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS MMIPB_PbapClientRegister(void);

/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_PbapClientDeregister(void);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinConnectWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : MMIPB_CreatePBAPTimer
//  Global resource dependence : 
//  Author: 
//  Note: 创建超时定时器
/*****************************************************************************/
LOCAL void MMIPB_CreatePBAPTimer(void);

/******************************************************************************/
// Description: MMIBT_ProcessPBAPTimerOver
// Global resource dependence: 
// Author: 
// Note: 超时保护机制
/******************************************************************************/
LOCAL void MMIPB_ProcessPBAPTimerOver(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : to initiate a transport disconnection. 
//  Global resource dependence :                                
//  Author: 
//  Note: BT_PbapDisconnect will cause the transport to be brought down automatically, 
//  but if the transport must be brought down immediately, perhaps due an error condition, 
//  this routine may be used. 
/*****************************************************************************/
LOCAL void MMIPB_PbapClientTpDisconnect(void);

/*****************************************************************************/
//  Description : Create a transport connection to the specified device 
//                and issue an OBEX Connect Request
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS MMIPB_PbapConnect(const BT_ADDRESS *addr);

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_PbapDisconnect(void);

/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: 
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
LOCAL void MMIPB_PbapAbortClient(void);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinSyncWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
#endif

/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void PbCreateChildWin(
                            MMI_HANDLE_T applet_handle,
                            MMI_WIN_ID_T    win_id,
							MMIPB_SUB_FUNCTION_TYPE_E  app_type
                            );                             
/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitAndEnterPBListWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param);


/*****************************************************************************/
//  Description : 群组窗口处理函数
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGroupListWinMsg
(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description :get group icon
//  Global resource dependence : 
//  Author:baokun
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetGroupIcon(uint8 group_id);
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendTwoLineTextIconSmallTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id1,
                            MMI_IMAGE_ID_T   imgae_id,
                            MMI_STRING_T     str_info,
                            uint16           pos,
                            BOOLEAN          is_update
                            );
/*****************************************************************************/
//  Discription: open one group records
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void OpenGroupEntryList(MMI_HANDLE_T applet_handle, uint16 group_id);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsEntryExistContent(MMIPB_CONTACT_T * contact_ptr);
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 处理全选按钮 pic refresh
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void UpdateSelectAllPic(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, MMI_HANDLE_T select_all_button_handle);
#endif
/*****************************************************************************/
//  Description : 设置控件的title
//  Global resource dependence :
//  Author: MARYXIAO
//  Note: 目前为一次将所有记录装入
/*****************************************************************************/
//LOCAL void SetOperateTitle(MMI_HANDLE_T label_handle,MMIPB_OPERATE_TYPE_E  oper_type);
/*****************************************************************************/
//  Description : 操作前打开选择窗口的的子控件初始化
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitOperateSelControl(MMI_WIN_ID_T win_id, MMIPB_OPERATE_TYPE_E oper_type);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 在手机中打开的窗口处理函数
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleEntryDetailPdaWinMsg
(
 MMI_WIN_ID_T       win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 );

#ifdef MMIPB_MOST_USED_SUPPORT
LOCAL MMI_RESULT_E  HandleMostUsedDetailWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );
#endif
#else
/*****************************************************************************/
//  Description : PB详情的窗口处理函数
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryDetailWinMsg
(
 MMI_WIN_ID_T       win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 );
#endif
/*****************************************************************************/
//  Description : 将需要操作的项放入选项列表中
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void AddCheckItemToOperateList(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, BOOLEAN is_operate);

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 将详情内容加到list中
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryDetails
(
 MMI_WIN_ID_T            win_id,
 MMI_CTRL_ID_T           ctrl_id,    //IN:
 MMIPB_CONTACT_T *contact_ptr,   //IN:
 uint16         search_entry_id,
 uint16         search_storage_id,
 uint32         search_group
 );
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 通话记录中号码类型选择窗口的处理
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSelectNumTypeWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif
/*****************************************************************************/
//  Description : to handle the memory detail window
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMemDetailWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

/*****************************************************************************/
//  Description :get group icon
//  Global resource dependence : 
//  Author:baokun
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetImageIcon(uint8 group_id);

/*****************************************************************************/
//  Description :get group icon
//  Global resource dependence : 
//  Author:baokun
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetBigIcon(uint8 group_id);
/*****************************************************************************/
//  Description : 通过win_id 取得对应窗口的list handle
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T MMIPB_GetListHandleByWin(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_HandleListFilter(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIPB_FILTER_TYPE_E type);

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : 新增群组窗口消息处理函数
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E HandleAddNewGroupWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleEditGroupNameMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleCopySelectWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenCopyLocationWin(MMI_HANDLE_T applet_handle, uint16 index);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleCopyLocationWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :  详情主窗口 处理函数 
//  Global resource dependence : 
//  Author: gaoping
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDetailMainWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );
#endif
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : SNS活动信息窗口的处理函数
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSnsActiveInfoWinMsg(
                            MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );
#endif



#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
WINDOW_TABLE( MMIPB_DETAILS_MAIN_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleDetailMainWinMsg ),    
    WIN_ID( MMIPB_DETAILS_MAIN_WIN_ID),

    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMIPB_DETAIL_MAIN_TAB_CTRL_ID),   
    END_WIN
};
#endif

/*****************************************************************************/
//  Description : 通过按拨号键值获得对应的sim卡dual_sys
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetDualSysFromGreenKey(MMI_MESSAGE_ID_E msg_id);

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : 搜索bt contacts条目
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_BtContactSearchAndAppendItem(
											  MMI_HANDLE_T list_handle, 
											  MMI_HANDLE_T applet_handle, 
											  GUIITEM_STYLE_E  listitem_type
											  );
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

WINDOW_TABLE(MMIPB_COPY_WIN_TAB) = 
{ 
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
        WIN_FUNC((uint32) HandleCopySelectWinMsg ),    
        WIN_ID(MMIPB_COPY_SELECT_WIN_ID),
        WIN_TITLE(TXT_PB_COPY_CONTACT),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_COPY_SELECT_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_COPY, TXT_NULL, STXT_RETURN),
        END_WIN    
        
};

WINDOW_TABLE(MMIPB_COPY_LOCATION_WIN_TAB) = 
{ 
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC((uint32) HandleCopyLocationWinMsg ),    
    WIN_ID(MMIPB_COPY_LOCATION_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPB_COPY_LOCATION_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
    END_WIN    
        
};

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
WINDOW_TABLE(MMIPB_MODIFY_GROUP_NAME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditGroupNameMsg),    
    WIN_ID(MMIBT_EDIT_GROUP_NAME_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_TEXT_CTRL(0,MMIBT_EDIT_GROUP_NAME_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),  
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
// 修改组名的窗口
WINDOW_TABLE( MMIPB_ADD_NEW_GROUP_WIN_TAB ) = 
{

    WIN_TITLE( TXT_PB_NEW_GROUP),
    WIN_FUNC( (uint32)HandleAddNewGroupWinMsg), 
    WIN_ID( MMIPB_ADD_NEW_GROUP_WIN_ID),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ADD_NEW_GROUP_FORM_ID),
   CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMIPB_ADD_NEW_GROUP_LISTBOX_CTRL_ID, MMIPB_ADD_NEW_GROUP_FORM_ID),    
#ifdef MMI_PDA_SUPPORT 
    CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_NULL, STXT_RETURN,MMIPB_ADD_NEW_GROUP_SOFTKEY_CTRL_ID,MMIPB_ADD_NEW_GROUP_FORM_ID),
#else
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#endif
   
    END_WIN
};
#endif
#endif
// 容量状态
WINDOW_TABLE( MMIPB_MEMDETAIL_WIN_TAB ) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_COMMON_DETAIL_MEMORY ),
    WIN_FUNC((uint32) HandleMemDetailWinMsg ),    
    WIN_ID( MMIPB_MEMDETAIL_WIN_ID ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIPB_MEMDETAIL_LABEL1_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPB_MEMDETAIL_LABEL6_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_MEMDETAIL_ANIM1_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPB_MEMDETAIL_LABEL2_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPB_MEMDETAIL_LABEL7_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),    
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_MEMDETAIL_ANIM2_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),

    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIPB_MEMDETAIL_LABEL3_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPB_MEMDETAIL_LABEL8_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    #ifdef DPHONE_SUPPORT
	#else    
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_MEMDETAIL_ANIM3_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    #endif
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPB_MEMDETAIL_LABEL4_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPB_MEMDETAIL_LABEL9_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID), 
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_MEMDETAIL_ANIM4_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIPB_MEMDETAIL_LABEL5_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIPB_MEMDETAIL_LABEL10_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),   
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_MEMDETAIL_ANIM5_CTRL_ID,MMIPB_MEMDETAIL_FORM_CTRL_ID),
#endif
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
// 提供给通话记录的选择号码类型的窗口
WINDOW_TABLE( MMIPB_SELECT_NUM_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_PB_SELECT_NUMBER  ),
    WIN_FUNC( (uint32)HandleSelectNumTypeWinMsg ),    
    WIN_ID( MMIPB_SELECT_NUM_TYPE_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMIPB_SELECT_NUM_TYPE_LISTBOX_CTRL_ID),
    END_WIN
};
#endif
//外部应用选择的窗口
WINDOW_TABLE( MMIPB_ENTRY_LIST_FOR_EX_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( STXT_MAIN_PB ),
    WIN_FUNC( (uint32)HandleEntryListForExWinMsg ),    
    WIN_ID( MMIPB_ENTRY_LIST_FOR_EX_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ENTRY_LIST_EX_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_ENTRY_LIST_EX_FORM_CTRL_ID,MMIPB_ENTRY_LIST_EX_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_ENTRY_EX_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_ENTRY_LIST_EX_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_ENTRY_EX_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_ENTRY_LIST_EX_FORM_CTRL_ID),

    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMIPB_ENTRY_LIST_FOR_EX_LISTBOX_CTRL_ID, MMIPB_ENTRY_LIST_EX_FORM_CTRL_ID),
    END_WIN
};
//操作前的选择窗口
WINDOW_TABLE( MMIPB_OPERATION_SEL_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_NULL ),
    WIN_FUNC( (uint32)HandleOperateSelWinMsg),    
    WIN_ID( MMIPB_OPERATE_SEL_LIST_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_OPERATE_SELECT_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_TITLE_OPERATE_SELECT_FORM_CTRL_ID,MMIPB_OPERATE_SELECT_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_SEARCH_OPERATE_SELECT_FORM_CTRL_ID,MMIPB_CHILD_TITLE_OPERATE_SELECT_FORM_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_SEARCH_OPERATE_SELECT_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_OPERATE_SEL_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_SEARCH_OPERATE_SELECT_FORM_CTRL_ID),
#else
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_OPERATE_SEL_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_SEARCH_OPERATE_SELECT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_SEARCH_OPERATE_SELECT_FORM_CTRL_ID),    
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_TITLE_CTRL_ID, MMIPB_CHILD_TITLE_OPERATE_SELECT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_CHECK_UNSELECTED_ICON, MMIPB_SELECT_ALL_BUTTON_CTRL_ID, MMIPB_CHILD_TITLE_OPERATE_SELECT_FORM_CTRL_ID),
#endif
   

    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID, MMIPB_OPERATE_SELECT_FORM_CTRL_ID),

    END_WIN
};


#ifdef MMI_PDA_SUPPORT

// 打开菜单窗口
WINDOW_TABLE( MMIPB_ENTRY_DETAILS_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleEntryDetailPdaWinMsg ),    

    WIN_ID( MMIPB_ENTRY_DETAILS_WIN_ID),
    //WIN_TITLE(STXT_DETAIL),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_DETAIL_FORM_CTRL_ID,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_ENTRY_DETAIL_NAME_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMG_PB_FAVBRATE_ICON_ON, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_PB_EDIT_CONTACT_ICON, MMIPB_ENTRY_DETAIL_EDIT_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
   // CHILD_ANIM_CTRL(FALSE,TRUE,MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
   
    END_WIN
};

#if defined (MMIPB_SIMDN_SUPPORT)
// 打开菜单窗口
WINDOW_TABLE( MMIPB_ENTRY_SIMDN_DETAILS_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleEntryDetailPdaWinMsg ),    

    WIN_ID( MMIPB_ENTRY_DETAILS_WIN_ID),
    WIN_TITLE(STXT_DETAIL),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_DETAIL_FORM_CTRL_ID,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_ENTRY_DETAIL_NAME_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMG_PB_MENU_EDIT_ICON, MMIPB_ENTRY_DETAIL_EDIT_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
   // CHILD_ANIM_CTRL(FALSE,TRUE,MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
   
    END_WIN
};
#endif
#ifdef SNS_SUPPORT
WINDOW_TABLE( MMIPB_SNS_ENTRY_DETAILS_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleEntryDetailPdaWinMsg ),    

    WIN_ID( MMIPB_SNS_ENTRY_DETAILS_WIN_ID),
    //WIN_TITLE(STXT_DETAIL),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_DETAIL_FORM_CTRL_ID,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_ENTRY_DETAIL_NAME_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
   // CHILD_ANIM_CTRL(FALSE,TRUE,MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
   
    END_WIN
};
#endif
// 打开菜单窗口
WINDOW_TABLE( MMIPB_ENTRY_DETAILS_OTHER_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleEntryDetailPdaWinMsg ),    
    WIN_ID( MMIPB_ENTRY_DETAILS_WIN_ID),
   // WIN_TITLE(STXT_DETAIL),
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_DETAIL_FORM_CTRL_ID,MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_ENTRY_DETAIL_NAME_CTRL_ID, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_TEXTLIST_E, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID),
   
    END_WIN
};
#else
// 打开菜单窗口
WINDOW_TABLE( MMIPB_ENTRY_DETAILS_WIN_TAB ) = 
{
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleEntryDetailWinMsg ),    
    WIN_ID( MMIPB_ENTRY_DETAILS_WIN_ID),
    WIN_TITLE(STXT_DETAIL),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),   
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIPB_ENTRY_DETAIL_LISTBOX_CTRL_ID),            
    END_WIN
};
#endif




WINDOW_TABLE(MMIPB_GROUP_LIST_WIN_TAB) = 
{

    WIN_TITLE(TXT_PB_GROUP),
   // WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleGroupListWinMsg),    
    WIN_ID( MMIPB_GROUP_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMIPB_GROUP_ENTRY_LIST_CTRL_ID),
// #ifdef MMI_PDA_SUPPORT
//     CREATE_BUTTON_CTRL(IMAGE_PHONEBOOK_GROUP_FAMILY,MMIPB_GROUP_ENTRY_ADD_CTRL_ID),
// #endif
    END_WIN
};

WINDOW_TABLE(MMIPB_GROUP_LIST_EX_WIN_TAB) = 
{
   // WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleGroupListWinMsg),    
    WIN_ID( MMIPB_GROUP_LIST_WIN_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMIPB_GROUP_ENTRY_LIST_CTRL_ID),
    END_WIN
};
#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
WINDOW_TABLE( MMIPB_SIMDN_LIST_WIN_TAB ) = //FDN记录列表显示窗口
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSIMDNListWinMsg ),    
    WIN_ID( MMIPB_FDN_LIST_WIN_ID ),
    WIN_TITLE( TXT_PB_FDN_NUM),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN), 
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMIPB_FDN_LIST_CTRL_ID),
    END_WIN
};
#endif
// 查找后显示号码列表的窗口
WINDOW_TABLE( MMIPB_MAIN_WIN_TAB ) = 
{

    WIN_FUNC((uint32)HandleMainWinMsg),    
    WIN_ID( MMIPB_MAIN_WIN_ID ),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#if defined MMI_GUI_STYLE_TYPICAL || defined MMI_GUI_STYLE_MINISCREEN
    CREATE_TAB_CTRL(GUI_TAB_ITEM_MIDDLE, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_NULL, MMIPB_SEARCH_LIST_TAB_CTRL_ID),
#else
 	CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMIPB_SEARCH_LIST_TAB_CTRL_ID),
#endif
	WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),	 
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
#ifdef MMIPB_MOST_USED_SUPPORT
WINDOW_TABLE( MMIPB_MOSTUSED_DETAIL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMostUsedDetailWinMsg),    
    WIN_ID( MMIPB_MOST_USED_WIN_ID ),
#ifndef MMI_PDA_SUPPORT
    WIN_TITLE(TXT_NULL),
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_MOSTUSED_DETAIL_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIPB_MOSTUSED_DETAIL_TITLE_FORM_CTRL_ID, MMIPB_MOSTUSED_DETAIL_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE, MMIPB_MOSTUSED_DETAIL_PHOTO_CTRL_ID, MMIPB_MOSTUSED_DETAIL_TITLE_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE, MMIPB_MOSTUSED_DETAIL_LABEL_CTRL_ID, MMIPB_MOSTUSED_DETAIL_TITLE_FORM_CTRL_ID),
#endif
#ifdef TOUCH_PANEL_SUPPORT     
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS,  MMIPB_MOSTUSED_DETAIL_BTN_FORM_CTRL_ID, MMIPB_MOSTUSED_DETAIL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CL_DETAIL_CALL_BUTTON, MMIPB_MOSTUSED_DETAI_BTN_CALL_CTRL_ID ,MMIPB_MOSTUSED_DETAIL_BTN_FORM_CTRL_ID),
#ifdef MMI_IP_CALL_SUPPORT
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CL_DETAIL_IPCALL_BUTTON, MMIPB_MOSTUSED_DETAI_BTN_IPCALL_CTRL_ID ,MMIPB_MOSTUSED_DETAIL_BTN_FORM_CTRL_ID),
#endif
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CL_DETAIL_MESSAGE_BUTTON, MMIPB_MOSTUSED_DETAI_MSG_CTRL_ID ,MMIPB_MOSTUSED_DETAIL_BTN_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE,IMAGE_CL_DETAIL_DELETE_BUTTON, MMIPB_MOSTUSED_DETAI_BTN_DEL_CTRL_ID ,MMIPB_MOSTUSED_DETAIL_BTN_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIPB_MOSTUSED_DETAIL_LIST_FORM_CTRL_ID, MMIPB_MOSTUSED_DETAIL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,  MMIPB_MOSTUSED_DETAIL_LIST_CTRL_ID, MMIPB_MOSTUSED_DETAIL_LIST_FORM_CTRL_ID),

    END_WIN
};
#endif
#endif
//
#ifdef MMI_PDA_SUPPORT  
//alll entry list 
WINDOW_TABLE( MMIPB_ISTYLE_ENTRY_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePDAEntryListWinMsg),    
    WIN_ID( MMIPB_ENTRY_LIST_TAB_WIN_ID), 
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_ALL_CONTACT_SEARCH_EDIT_FORM_ID,MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_ALL_CONTACT_SEARCH_EDIT_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_ALL_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_ALL_CONTACT_SEARCH_EDIT_FORM_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_CHILD_ALL_SELECT_TITLE_CTRL_ID, MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_CHECK_UNSELECTED_ICON, MMIPB_CHILD_ALL_SELECT_BUTTON_CTRL_ID, MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID,MMIPB_ALL_CONTACT_SEARCH_WIN_FORM_ID),  
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};
#ifdef MMIPB_MOST_USED_SUPPORT
//mostusedful entry list 
WINDOW_TABLE( MMIPB_ISTYLE_MOSTUSED_CONTACTS_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePDAEntryListWinMsg),    
    WIN_ID( MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID),
   
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMIPB_MOSTUSED_CONTACT_DROPDOWNLIST_CTRL_ID,MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_CHILD_MOSTUSED_SELECT_TITLE_CTRL_ID, MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_CHECK_UNSELECTED_ICON, MMIPB_CHILD_MOSTUSED_SELECT_BUTTON_CTRL_ID, MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID,MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),    
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};


#endif
//group entry list
WINDOW_TABLE(MMIPB_ISTYLE_GROUP_ENTRYL_LIST_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePDAEntryListWinMsg),    
    WIN_ID( MMIPB_GROUP_ENTRYL_LIST_WIN_ID),
	WIN_TITLE(TXT_NULL),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_GROUP_CONTACT_SEARCH_EDIT_FORM_ID,MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_GROUP_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_GROUP_CONTACT_SEARCH_EDIT_FORM_ID),    
	CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_GROUP_CONTACT_SEARCH_EDIT_FORM_ID),
    
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_CHILD_GROUP_SELECT_TITLE_CTRL_ID, MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_CHECK_UNSELECTED_ICON, MMIPB_CHILD_GROUP_SELECT_BUTTON_CTRL_ID, MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    
    
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_GROUP_ENTRY_LISTBOX_CTRL_ID,MMIPB_GROUP_CONTACT_SEARCH_WIN_FORM_ID),  
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};

WINDOW_TABLE( MMIPB_PDA_ENTRY_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePDAEntryListWinMsg),    
    WIN_ID( MMIPB_ENTRY_LIST_TAB_WIN_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, MMIPB_CONTACT_ADD, MMIPB_ALL_SEARCH_ADD_BUTTON_CTRL_ID, MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_ALL_CONTACT_SEARCH_EDIT_FORM_ID,MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_ALL_CONTACT_SEARCH_EDIT_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_ALL_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_ALL_CONTACT_SEARCH_EDIT_FORM_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_CHILD_ALL_SELECT_TITLE_CTRL_ID, MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_CHECK_UNSELECTED_ICON, MMIPB_CHILD_ALL_SELECT_BUTTON_CTRL_ID, MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID),

    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID,MMIPB_ALL_CONTACT_SEARCH_WIN_FORM_ID),  
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};

//useful entry list 
#ifdef MMIPB_MOST_USED_SUPPORT
WINDOW_TABLE( MMIPB_PDA_MOSTUSED_CONTACTS_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePDAEntryListWinMsg),    
    WIN_ID( MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_DROPDOWNLIST_CTRL(TRUE, MMIPB_MOSTUSED_CONTACT_DROPDOWNLIST_CTRL_ID,MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_CHILD_MOSTUSED_SELECT_TITLE_CTRL_ID, MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_CHECK_UNSELECTED_ICON, MMIPB_CHILD_MOSTUSED_SELECT_BUTTON_CTRL_ID, MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID,MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID),  
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};
#endif
WINDOW_TABLE(MMIPB_PDA_GROUP_ENTRYL_LIST_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandlePDAEntryListWinMsg),    
    WIN_ID( MMIPB_GROUP_ENTRYL_LIST_WIN_ID),
	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_GROUP_CONTACT_SEARCH_EDIT_FORM_ID,MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_GROUP_CONTACT_SEARCH_EDIT_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_GROUP_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_GROUP_CONTACT_SEARCH_EDIT_FORM_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_CHILD_GROUP_SELECT_TITLE_CTRL_ID, MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_CHECK_UNSELECTED_ICON, MMIPB_CHILD_GROUP_SELECT_BUTTON_CTRL_ID, MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID),

    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_GROUP_ENTRY_LISTBOX_CTRL_ID,MMIPB_GROUP_CONTACT_SEARCH_WIN_FORM_ID),  
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};

#else

WINDOW_TABLE( MMIPB_ENTRY_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleEntryListWinMsg),    
    WIN_ID( MMIPB_ENTRY_LIST_TAB_WIN_ID),  
#ifdef MMI_GUI_STYLE_MINISCREEN 
    WIN_TITLE(TXT_COMMON_ALL),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_ENTRY_LISTBOX_CTRL_ID,MMIPB_CONTACT_SEARCH_WIN_FORM_ID), 

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_CONTACT_SEARCH_WIN_FORM_ID),
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_CONTACT_SEARCH_EDIT_FORM_ID,MMIPB_CHILD_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_ENTRY_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_CONTACT_SEARCH_EDIT_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_CONTACT_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_CONTACT_SEARCH_EDIT_FORM_ID),
   
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
   // WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};

#ifdef MMIPB_MOST_USED_SUPPORT
WINDOW_TABLE( MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleEntryListWinMsg),    
    WIN_ID( MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID),      

    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};
#endif

//group entry list
WINDOW_TABLE(MMIPB_GROUP_ENTRYL_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleEntryListWinMsg),    
    WIN_ID( MMIPB_GROUP_ENTRYL_LIST_WIN_ID),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_SINGLE_GROUP_ENTRY_LISTBOX_CTRL_ID,MMIPB_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID), 
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_EDIT_FORM_ID,MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_SINGLE_GROUP_ENTRY_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_EDIT_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_SINGLE_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_EDIT_FORM_ID),
	WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#ifdef MMIPB_TIPS_OPEN
    WIN_TIPS,
#endif
    END_WIN
};
#endif

#ifdef SNS_SUPPORT
// 打开菜单窗口
WINDOW_TABLE( MMIPB_SNS_ACTIVEINFO_WIN_TAB ) = 
{
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_FUNC( (uint32)HandleSnsActiveInfoWinMsg ),    
    WIN_ID( MMIPB_SNS_ACTIVEINFO_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    CREATE_RICHTEXT_CTRL(MMIPB_SNS_ACTIVEINFO_LIST_CTRL_ID),    
    WIN_SOFTKEY(TXT_NULL, TXT_COMMON_UPDATE, TXT_NULL),
    END_WIN
};
#endif

//Bt contacts entry list
#ifdef BT_PBAP_SUPPORT
WINDOW_TABLE( MMIPB_BT_CONTACTS_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleEntryListWinMsg),    
	WIN_ID( MMIPB_BT_CONTACTS_LIST_TAB_WIN_ID),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_BT_CONTACT_SEARCH_WIN_FORM_ID),
	CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E,MMIPB_BT_ENTRY_LISTBOX_CTRL_ID,MMIPB_BT_CONTACT_SEARCH_WIN_FORM_ID),      

	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_BT_CONTACT_SEARCH_WIN_FORM_ID,MMIPB_BT_CONTACT_SEARCH_WIN_FORM_ID),
	
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIPB_CHILD_BT_CONTACT_SEARCH_EDIT_FORM_ID,MMIPB_CHILD_BT_CONTACT_SEARCH_WIN_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE, IMG_PB_MENU_SEARCH_ICON, MMIPB_BT_ENTRY_SEARCH_BUTTON_CTRL_ID, MMIPB_CHILD_BT_CONTACT_SEARCH_EDIT_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIPB_NAME_MAX_LEN,MMIPB_BT_CONTACT_SEARCH_EDITBOX_CTRL_ID,MMIPB_CHILD_BT_CONTACT_SEARCH_EDIT_FORM_ID),

	WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#ifdef MMIPB_TIPS_OPEN
	WIN_TIPS,
#endif
	END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 创建常用联系人页
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateMostUsedContactTabWin(MMI_WIN_ID_T win_id)
{
	MMIPB_LIST_HANDLE* win_tab_infor_ptr = PNULL;
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIPB_CreateMostUsedContactTabWin PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_1765_112_2_18_2_44_17_305,(uint8*)"");
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle; 	
	win_tab_infor_ptr = (MMIPB_LIST_HANDLE *)SCI_ALLOC_APP(sizeof(MMIPB_LIST_HANDLE));
	if(win_tab_infor_ptr == PNULL)
	{
		//SCI_TRACE_LOW:"[MMIPB]MMIPB_CreateMostUsedContactTabWin group_infor_ptr == PNULL !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_1772_112_2_18_2_44_17_306,(uint8*)"");
		return;
	}
	SCI_MEMSET(win_tab_infor_ptr, 0x00, sizeof(MMIPB_LIST_HANDLE));
	win_tab_infor_ptr->win_id = MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID);
	win_tab_infor_ptr->search_str.wstr_len = 0;
	SCI_MEMSET(win_tab_infor_ptr->search_str.wstr, 0x00, sizeof(win_tab_infor_ptr->search_str.wstr));
	win_tab_infor_ptr->contacts_type = MMIPB_MOSTUSED_CONTACTS;
#ifdef MMI_PDA_SUPPORT	
    win_tab_infor_ptr->group_id = PB_GROUP_FAVROUTE;
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		MMK_CreateChildWin(win_id, MMIPB_ISTYLE_MOSTUSED_CONTACTS_LIST_TAB_WIN_TAB, win_tab_infor_ptr);/*lint !e605*/
	}
	else
#endif		
	{
		MMK_CreateChildWin(win_id, MMIPB_PDA_MOSTUSED_CONTACTS_LIST_TAB_WIN_TAB, win_tab_infor_ptr);/*lint !e605*/ 	
	}
#else
        win_tab_infor_ptr->group_id = PB_GROUP_MOSTUSED;
	MMK_CreateChildWin(win_id, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_TAB, win_tab_infor_ptr);/*lint !e605*/ 	
#endif
}
#endif

/*****************************************************************************/
//  Description : 进入群组窗口
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenGroupWin(MMI_HANDLE_T applet_handle)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIPB_OpenGroupWin PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_1806_112_2_18_2_44_17_307,(uint8*)"");
        return;
    }
    MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
    MMK_CreateWinByApplet(applet_handle,(uint32 *)MMIPB_GROUP_LIST_WIN_TAB, PNULL);
}
/*****************************************************************************/
//  Description : 创建全部联系人页
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateAllContactTabWin(MMI_WIN_ID_T win_id)
{
	MMIPB_LIST_HANDLE* win_tab_infor_ptr = PNULL;
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIPB_CreateAllContactTabWin PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_1825_112_2_18_2_44_17_308,(uint8*)"");
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle; 

	win_tab_infor_ptr = (MMIPB_LIST_HANDLE *)SCI_ALLOC_APP(sizeof(MMIPB_LIST_HANDLE));
	
	if(win_tab_infor_ptr == PNULL)
	{
		//SCI_TRACE_LOW:"[MMIPB]MMIPB_CreateAllContactTabWin group_infor_ptr == PNULL !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_1834_112_2_18_2_44_17_309,(uint8*)"");
		return;
	}
	SCI_MEMSET(win_tab_infor_ptr, 0x00, sizeof(MMIPB_LIST_HANDLE));
	win_tab_infor_ptr->win_id = MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID);
	win_tab_infor_ptr->contacts_type = MMIPB_ALL_GROUP_CONTACTS;
    win_tab_infor_ptr->group_id = PB_GROUP_ALL;
	SCI_MEMSET(&win_tab_infor_ptr->search_str, 0x00, sizeof(win_tab_infor_ptr->search_str));
#ifdef MMI_PDA_SUPPORT	
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		MMK_CreateChildWin(win_id, MMIPB_ISTYLE_ENTRY_LIST_TAB_WIN_TAB, win_tab_infor_ptr);/*lint !e605*/
	}
	else
#endif		
	{
        //uint32 temp1 = MMIPB_MAX_WIN_ID;
        //uint32 temp2 = MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID;
		MMK_CreateChildWin(win_id, MMIPB_PDA_ENTRY_LIST_TAB_WIN_TAB, win_tab_infor_ptr);/*lint !e605*/ 	
	}
#else
	MMK_CreateChildWin(win_id, MMIPB_ENTRY_LIST_TAB_WIN_TAB, win_tab_infor_ptr);/*lint !e605*/ 	
#endif
}

/*****************************************************************************/
//  Description : 创建联系人分组页
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateGroupListTabWin(MMI_WIN_ID_T win_id)
{
	MMK_CreateChildWin(win_id, MMIPB_GROUP_LIST_EX_WIN_TAB, PNULL);/*lint !e605*/
}


/*****************************************************************************/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UNMarkAll(MMI_HANDLE_T applet_handle)
{
    MMI_HANDLE_T   list_handle =NULL;
    MMIPB_APPLET_T* applet_ptr =  (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
        return;
    list_handle = MMIPB_GetListHandleByWin(applet_ptr->instance.active_win_id);

    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
        GUILIST_SetTextListMarkable(list_handle, FALSE);
    }
    MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
    GUILIST_SetMaxSelectedItem(list_handle, GUILIST_GetTotalItemNum(list_handle));
	GUILIST_SetAllSelected(list_handle, FALSE);
}
/*****************************************************************************/
//  Description : 获得列表中允许的最大可选数目
//  Global resource dependence : g_mmipb_table_info
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIPB_GetListAllowableMaxSelectedNum
(
    MMI_HANDLE_T applet_handle
)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIPB_GetListAllowableMaxSelectedNum PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_1940_112_2_18_2_44_17_310,(uint8*)"");
        return 0;
    }
    return (applet_ptr->instance.max_select_count);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void SetListSoftKey(MMI_WIN_ID_T win_id, 
                          MMI_CTRL_ID_T ctrl_id, 
                          GUILIST_ITEM_DATA_T* item_data_ptr,
                          uint16                index,
                          uint16 search_len
                          )
{
    MMI_TEXT_ID_T           left_softkey_id = TXT_NULL;
    MMI_TEXT_ID_T           right_softkey_id = TXT_NULL;
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMI_IMAGE_ID_T           mid_softkey_id = IMAGE_NULL;
#else
	MMI_TEXT_ID_T           mid_softkey_id = TXT_NULL;
#endif
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMIPB_HANDLE_T  applet_handle = NULL;
    if(PNULL == applet_ptr)
    {
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    
    {
        right_softkey_id = STXT_RETURN;
    }
	if(MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
	{
		//set softkey
		if(GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(ctrl_id, index), GUIITEM_STATE_SELECTED))
		{
            mid_softkey_id = STXT_CANCEL;
		}
		else if(MMIPB_GetSelectCount(applet_ptr->instance.handle) < applet_ptr->instance.max_select_count)
		{
            mid_softkey_id = STXT_MARK;
		}
	}    
    //set right SK
    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
        left_softkey_id = STXT_OPTION;
    }
    else if(MMIPB_LIST_RADIO == applet_ptr->instance.list_type)
    {
        left_softkey_id = TXT_COMMON_OK;
    }

    else
    {
        left_softkey_id = STXT_OPTION;
    }

    item_data_ptr->softkey_id[LEFT_BUTTON] = left_softkey_id;

    item_data_ptr->softkey_id[RIGHT_BUTTON] = right_softkey_id;
    
    if((MMIPB_LIST_CHECK== applet_ptr->instance.list_type )||(MMIPB_LIST_RADIO==applet_ptr->instance.list_type))
     {
        if(GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(ctrl_id, index), GUIITEM_STATE_SELECTED))
        {
            item_data_ptr->softkey_id[MIDDLE_BUTTON] = STXT_CANCEL;
        }
        else
        {
            item_data_ptr->softkey_id[MIDDLE_BUTTON] = STXT_SELECT;

        }
     }
     else
     {
#ifdef MMI_GUI_STYLE_MINISCREEN
          item_data_ptr->softkey_id[MIDDLE_BUTTON] = IMAGE_NULL;
#else
          item_data_ptr->softkey_id[MIDDLE_BUTTON] = TXT_NULL;
#endif         
     }
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void AppendDyncSplitItem(MMI_CTRL_ID_T ctrl_id,
                               GUILIST_ITEM_T* list_item_ptr, 
                               uint16  index, 
                               uint16 item_content_index,
                               MMIPB_LIST_ITEM_TYPE_E dync_type
                               )
{
    MMI_STRING_T            display_str   ={0};
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

    display_str.wstr_len = 1;
    display_str.wstr_ptr = &list_item_ptr->first_letter;
    if (PNULL != display_str.wstr_ptr)
	{
//#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error	
		MMIPB_Wstrupper(&display_str);
//#endif
	    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
	    item_data.item_content[0].item_data.text_buffer = display_str;

	    if(dync_type == MMIPB_NEED_LIST_ITEM_DATA_TYPE)
	    {
	        GUILIST_SetItemData( ctrl_id, &item_data, index);
	    }
	    else if(dync_type == MMIPB_NEED_LIST_ITEM_CONTENT_TYPE)
	    {
	        GUILIST_SetItemContent( ctrl_id, &item_data.item_content[item_content_index], index, item_content_index);
	    }
   }
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void GetIndexByDyncType(MMIPB_LIST_ITEM_TYPE_E dync_type,//in,
                         void *item_ptr,//in
                         uint16* index,//out
                         uint16* item_content_index//out
                         )
{

    switch(dync_type)
    {
    case MMIPB_NEED_LIST_ITEM_DATA_TYPE:       //need data
        {
             GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)item_ptr;
             if( PNULL != need_item_data_ptr)
             {
                 *index = need_item_data_ptr->item_index;
             }
        }
        break;
    case  MMIPB_NEED_LIST_ITEM_CONTENT_TYPE:    //need content
        {
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)item_ptr;
            
            if( PNULL != need_item_content_ptr)
            {
                *index = need_item_content_ptr->item_index;
                *item_content_index = need_item_content_ptr->item_content_index;
            }
        }
        break;
    }
    //SCI_TRACE_LOW:"[MMIPB]AppendDyncListItem dync_type %d index %d item_content_index %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2119_112_2_18_2_44_18_311,(uint8*)"ddd",dync_type,index,item_content_index);
}



/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void AppendDyncPhoto(MMI_WIN_ID_T win_id,//in
                           uint8 photo_id_flag, //in
                           uint16 entry_id,//in
                           uint16 storage_id,//in
                           MMI_IMAGE_ID_T     list_icon,
                           MMIPB_CUSTOM_PHOTO_T* custom_photo_ptr,
                           GUIANIM_FILE_INFO_T* anim_path_ptr,
                           int *i,
                           GUILIST_ITEM_DATA_T* item_data_ptr//out
                           )
{
    BOOLEAN            is_photo_id_exist = FALSE;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    
    if(PNULL == applet_ptr)
    {
        return;
    }
    
 	if((photo_id_flag & MMIPB_PHOTO_ID_EXIST) && MMIPB_ReadPhotoFile(entry_id, storage_id, custom_photo_ptr))
	{

		if (MMIAPIFMM_IsFileExist(
            (const uint16 *)custom_photo_ptr->file_name,
            (uint16)(custom_photo_ptr->file_name_len)
            ))
		{
			is_photo_id_exist = TRUE;
		}

	}

  //  if ( MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
#ifdef TCARD_PB_SUPPORT
        if(MMIPB_IsTCardContact(storage_id, entry_id))
        {
            item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data_ptr->item_content[0].item_data.image_id = IMAGE_PB_LIST_TCARD_ICON;            
        }
        else
#endif
        if(is_photo_id_exist)
        {
            item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            anim_path_ptr->full_path_wstr_ptr = custom_photo_ptr->file_name;
            anim_path_ptr->full_path_wstr_len = custom_photo_ptr->file_name_len;            
            item_data_ptr->item_content[0].item_data.anim_path_ptr =anim_path_ptr;

        }

        else
        {
            item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data_ptr->item_content[0].item_data.image_id = list_icon;

        }

        *i = *i+1;
    }

    


}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void AppendDyncName(MMI_WIN_ID_T win_id, MMIPB_NAME_T *entry_name_ptr, wchar* temp_wstr_1,int *i, GUILIST_ITEM_DATA_T* item_data_ptr)
{
    MMI_STRING_T    disp_str = {0};
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"AppendDyncName PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2227_112_2_18_2_44_18_312,(uint8*)"");
        return;
    }
    disp_str.wstr_ptr = temp_wstr_1;

    GetDisplayStringFromPhonebookName(&disp_str,
                                      entry_name_ptr
                                      );
    item_data_ptr->item_content[*i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data_ptr->item_content[*i].item_data.text_buffer = disp_str;
    *i=*i+1;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void AppendDyncNumber(
                           MMI_WIN_ID_T win_id,
                           MMIPB_MAIL_T   *mail_info_ptr,//in
                           MMIPB_BCD_NUMBER_T   *number_t_ptr,//in
                           wchar* temp_wstr_2,
                           int* i,//in
                           MMIPB_LIST_FIELD_TYPE_E display_field,
                           uint8*           number_index,//out                           
                           GUILIST_ITEM_DATA_T* item_data_ptr//out                           
                           )
{
    MMI_STRING_T    disp_str = {0};
    //uint8           num_type = 0;
    uint8           str[MMIPB_MAX_STRING_LEN + 2] = {0};
    MN_NUMBER_TYPE_E number_type = 0;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);/*lint !e64*/
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"AppendDyncNumber PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2262_112_2_18_2_44_18_313,(uint8*)"");
        return;
    }
    
    //cr112019 if exit number show number
    if((MMIPB_LIST_FIELD_MAIL==display_field)&&(mail_info_ptr != PNULL) && (mail_info_ptr->wstr_len >0))//only show email
    {
        disp_str.wstr_len =     mail_info_ptr->wstr_len;
        disp_str.wstr_ptr =     mail_info_ptr->wstr;
        *number_index = MMIPB_INDEX_OF_MAIL;
    }
    else if(number_t_ptr != PNULL 
        &&MMIPB_IsValidPhoneNumber(number_t_ptr->npi_ton, number_t_ptr->number_len))//show number
    {
        //exist phone number
        //cr112019 end

        number_type = MMIPB_GetNumberTypeFromUint8(number_t_ptr->npi_ton);
        disp_str.wstr_len = MMIAPICOM_GenDispNumber(
            (number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),//MMIPB_GetNumberTypeFromUint8(number_ptr->npi_ton), 
            number_t_ptr->number_len, 
            number_t_ptr->number,
            (uint8 *)str,
            MMIPB_MAX_STRING_LEN+2
            );
        disp_str.wstr_len = MIN(disp_str.wstr_len, MMIPB_MAX_STRING_LEN);
        disp_str.wstr_ptr = temp_wstr_2;
        MMI_STRNTOWSTR(disp_str.wstr_ptr, MMIPB_MAX_STRING_LEN, str, MMIPB_MAX_STRING_LEN, disp_str.wstr_len);/*lint !e64*/
    }
    //cr112019 if the item only has a mail address, show mail
    else if(mail_info_ptr != PNULL && (mail_info_ptr->wstr_len >0))//show email if without num
    {

        disp_str.wstr_len =     mail_info_ptr->wstr_len;
        disp_str.wstr_ptr =     mail_info_ptr->wstr;
        *number_index = MMIPB_INDEX_OF_MAIL;
    } 


    item_data_ptr->item_content[*i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data_ptr->item_content[*i].item_data.text_buffer = disp_str;
    *i=*i+1;

}


/*****************************************************************************/
//  Description :append detail list anim
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
//        
/*****************************************************************************/
LOCAL BOOLEAN  AppendDetailDyncPhoto(MMI_HANDLE_T ctrl_id, uint16 index, MMIPB_CONTACT_T* contact_ptr, void *item_ptr)
{

    MMIPB_CUSTOM_PHOTO_T                *custom_photo_ptr = PNULL;
    BOOLEAN            is_photo_id_exist = FALSE;

    uint16            item_content_index = 0;
    GUILIST_ITEM_DATA_T item_data = {0};

    GUIANIM_FILE_INFO_T anim_path = {0};
    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
    if(PNULL == custom_photo_ptr)
    {
        //SCI_TRACE_LOW:" [mmipb] AppendDetailDyncPhoto PNULL == custom_photo_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2372_112_2_18_2_44_18_314,(uint8*)"");
        return FALSE;
    }

    if(PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] AppendDetailDyncPhoto PNULL == item_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2378_112_2_18_2_44_18_315,(uint8*)"");
        return FALSE;
    }


    item_content_index = ((GUILIST_NEED_ITEM_CONTENT_T*)item_ptr)->item_content_index;
    
 	if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST) && MMIPB_ReadPhotoFile(contact_ptr->contact_id, contact_ptr->storage_id, custom_photo_ptr))
	{

		if (MMIAPIFMM_IsFileExist(
            (const uint16 *)custom_photo_ptr->file_name,
            (uint16)(custom_photo_ptr->file_name_len)
            ))
		{
			is_photo_id_exist = TRUE;
		}

	}


    if(is_photo_id_exist)
    {
        item_data.item_content[item_content_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
        anim_path.full_path_wstr_ptr = custom_photo_ptr->file_name;
        anim_path.full_path_wstr_len = custom_photo_ptr->file_name_len;            
        item_data.item_content[item_content_index].item_data.anim_path_ptr =&anim_path;

    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB]AppendDetailDyncPhoto is_photo_id_exist %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2410_112_2_18_2_44_18_316,(uint8*)"d", is_photo_id_exist);
        return FALSE; 

    }

     GUILIST_SetItemContent( ctrl_id, &item_data.item_content[item_content_index], index, item_content_index );
    SCI_FREE(custom_photo_ptr);

    return TRUE;
}



/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void AppendDyncListItem(MMI_WIN_ID_T win_id, 
                              MMI_CTRL_ID_T ctrl_id,
                              void *item_ptr,
                              MMIPB_LIST_ITEM_TYPE_E dync_type, 
                              uint16 search_len
                              )
{
    int                 i          = 0;
    uint16                     index = 0;
    uint16              item_content_index = 0;
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMIPB_BCD_NUMBER_T   number_t = {0};
    MMI_STRING_T    disp_str = {0};
	uint8           number_index = 0;
	//uint8           num_type = 0;
    //uint8           str[MMIPB_MAX_STRING_LEN + 2] = {0};
    wchar           *temp1_wstr_ptr = PNULL;
    wchar           *temp2_wstr_ptr = PNULL;
    wchar           *temp3_wstr_ptr = PNULL;
    MMIPB_LIST_TYPE_E   list_type  = MMIPB_LIST_NORMAL_MAIN;
    GUIANIM_FILE_INFO_T anim_path = {0};

    //PHONEBOOK_STORAGE_E storage = 0;
    MMI_IMAGE_ID_T     list_icon= 0;
    MMIPB_NAME_T      *entry_name_ptr = PNULL;
    //BOOLEAN           is_select = 0;

    GUILIST_ITEM_T* list_item_ptr = PNULL;
#ifdef MMIPB_MAIL_SUPPORT
    MMIPB_MAIL_T   *mail_info_ptr = PNULL;
#endif
    uint16 entry_id = 0;
    uint16 storage_id = 0;
    uint8 group_id = 0;
    //uint8 photo_id_flag = 0;
    //uint8 ring_id_flag = 0;
#ifdef MMIPB_MOST_USED_SUPPORT    
    MMIPB_MOSTUSED_CONTACT_NODE_T *mostused_contact_node_ptr = PNULL;
    uint16  search_entry_id = 0;
    uint16  search_storage_id = 0;
    uint32  search_group =0;
    
#endif
    MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
    //uint16          contact_index = 0;
    uint32                  list_data = 0;
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    uint16   num_index = 0;
#ifdef MMIPB_MOST_USED_SUPPORT 
    MMIPB_CONTACT_T *contact = PNULL; 
#endif
    MMIPB_LIST_FIELD_TYPE_E field_type = 0;
#ifdef MMIPB_MOST_USED_SUPPORT 
#ifdef PB_SUPPORT_LOW_MEMORY	
    MMIPB_ACCESS_POINT_T access_point = {0};
#endif
#endif
    MMIPB_CUSTOM_PHOTO_T                *custom_photo_ptr = PNULL;

    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"AppendDyncListItem PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2482_112_2_18_2_44_19_317,(uint8*)"");
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;

    if(PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] AppendDyncListItem PNULL == item_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2489_112_2_18_2_44_19_318,(uint8*)"");
        return;
    }

    list_type = applet_ptr->instance.list_type;

    GetIndexByDyncType(dync_type, item_ptr, &index, &item_content_index);

    list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,index);/*lint !e605*/
    if(PNULL ==list_item_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] AppendDyncListItem PNULL == list_item_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2500_112_2_18_2_44_19_319,(uint8*)"");
        return;
    }
    list_data=list_item_ptr->user_data;

    SetListSoftKey(win_id, ctrl_id, &item_data,index, search_len);
#ifdef MMI_PDA_SUPPORT
    if(MMIPB_ISSPLIT(list_data))//split
    {
        AppendDyncSplitItem(ctrl_id, list_item_ptr, index, item_content_index, dync_type);
        return;
    }  
#endif
    temp1_wstr_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
    temp2_wstr_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
    temp3_wstr_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
    entry_name_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_NAME_T));
#ifdef MMIPB_MAIL_SUPPORT
    mail_info_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_MAIL_T));
#endif
#ifdef MMIPB_MOST_USED_SUPPORT    
    mostused_contact_node_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T));
#endif
    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
    if(temp1_wstr_ptr == PNULL || temp2_wstr_ptr == PNULL 
        || temp3_wstr_ptr == PNULL || entry_name_ptr == PNULL
#ifdef MMIPB_MAIL_SUPPORT
        || mail_info_ptr == PNULL
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
        || mostused_contact_node_ptr == PNULL
#endif
        ||custom_photo_ptr == PNULL
        )
    {
        if(temp1_wstr_ptr != PNULL)
        {
            SCI_FREE(temp1_wstr_ptr);
        }
        if(temp2_wstr_ptr != PNULL)
        {
            SCI_FREE(temp2_wstr_ptr);
        }
        if(temp3_wstr_ptr != PNULL)
        {
            SCI_FREE(temp3_wstr_ptr);
        }
        if(entry_name_ptr != PNULL)
        {
            SCI_FREE(entry_name_ptr);
        }
#ifdef MMIPB_MAIL_SUPPORT
        if(mail_info_ptr != PNULL)
        {
            SCI_FREE(mail_info_ptr);
        }
#endif
#ifdef MMIPB_MOST_USED_SUPPORT         
        if(mostused_contact_node_ptr != PNULL)
        {
            SCI_FREE(mostused_contact_node_ptr);
        }
#endif        
        if(custom_photo_ptr != PNULL)
        {
            SCI_FREE(custom_photo_ptr);
        }        
        return;        
    }  
    if(applet_ptr->instance.app_type < MMIPB_TAB_MAX_NUM)
    {
        field_type = applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type];
    }
    disp_str.wstr_ptr = temp1_wstr_ptr;
#ifdef MMIPB_MOST_USED_SUPPORT     
    if((PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id))
    {
        contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
        SCI_MEMSET(contact, 0x00, sizeof(MMIPB_CONTACT_T));
        *mostused_contact_node_ptr = MMIPB_GetMostUsedContactBaseInfo(MMIPB_CONTACTINDEX(list_data));
        entry_id = mostused_contact_node_ptr->entry_id;
        storage_id =mostused_contact_node_ptr->storage_id;
#ifdef PB_SUPPORT_LOW_MEMORY        
        access_point.contact.contact_id = entry_id;
        access_point.contact.storage_id = storage_id;
        MMIPB_Read(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), contact);
        if(MMIPB_GetNameByNumber(&(contact->number[0]), &disp_str, MMIPB_NAME_MAX_LEN, &search_entry_id,&search_storage_id,&search_group,FALSE, FALSE))
#else
        if(MMIPB_GetNameByNumber(&(mostused_contact_node_ptr->number), &disp_str, MMIPB_NAME_MAX_LEN, &search_entry_id,&search_storage_id,&search_group,FALSE, FALSE))
#endif
        {
            entry_name_ptr->wstr_len = disp_str.wstr_len;
            if(PNULL!= disp_str.wstr_ptr)
            {
            
                SCI_MEMCPY(entry_name_ptr->wstr, disp_str.wstr_ptr, sizeof(wchar)*MMIPB_NAME_MAX_LEN)
            }
        }
        else
        {
            SCI_MEMSET(entry_name_ptr, 0x00, sizeof(MMIPB_NAME_T));
        }
       // group_id = PB_GROUP_MOSTUSED;
        MMIPB_GetAllGroupIDs(search_group, &group_id, 1);
#ifdef PB_SUPPORT_LOW_MEMORY  
        SCI_MEMCPY(&number_t, &(contact->number[0]), sizeof(MMIPB_BCD_NUMBER_T));
#else
        SCI_MEMCPY(&number_t, &(mostused_contact_node_ptr->number), sizeof(MMIPB_BCD_NUMBER_T));
#endif    
        SCI_FREE(contact);
    }
    else
#endif        
    {
        MMIPB_NAME_NUMBER_MAIL_T *contact_string_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_NUMBER_MAIL_T));

        if(contact_string_ptr != PNULL)
        {
            SCI_MEMSET(contact_string_ptr, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
            contact_node = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(list_data));
            entry_id = contact_node.contact_id;
            storage_id =contact_node.storage_id;

            MMIPB_GetAllGroupIDs(contact_node.group, &group_id, 1);
            //MMIPB_GetContactInfo(entry_id, storage_id, contact);
            MMIPB_GetContactNameNumberByIndex(contact_string_ptr, MMIPB_CONTACTINDEX(list_data));
            SCI_MEMCPY(entry_name_ptr ,&contact_string_ptr->name, sizeof(MMIPB_NAME_T));
            //photo_id_flag = contact->photo_id_flag;
            if(MMIPB_LIST_FIELD_WHOLE == field_type)
            {
                num_index = MMIPB_GetFirstValidNumberIndex(contact_string_ptr->number_t, MMIPB_MAX_NV_PHONE_NUM);
            }
            else
            {
                num_index = MMIPB_NUMINDEX(list_data);
            }
            if(num_index < MMIPB_MAX_NV_PHONE_NUM)
            {
                SCI_MEMCPY(&number_t, &(contact_string_ptr->number_t[num_index]), sizeof(MMIPB_BCD_NUMBER_T));
            }

#ifdef MMIPB_MAIL_SUPPORT
            SCI_MEMCPY(mail_info_ptr, &contact_string_ptr->mail, sizeof(contact_string_ptr->mail));
#endif
            SCI_FREE(contact_string_ptr);
        }        
        //SCI_FREE(contact);
    }
	list_icon = GetImageIcon(group_id);


    {
    
        AppendDyncPhoto(win_id, 0, entry_id, storage_id, list_icon, custom_photo_ptr, &anim_path , &i ,&item_data);
    }
   
    if(dync_type == MMIPB_NEED_LIST_ITEM_DATA_TYPE)
    {
        SCI_MEMSET(temp1_wstr_ptr, 0x00, sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
        if(0 == entry_name_ptr->wstr_len)
        {
            AppendDyncNumber(win_id, 
#ifdef MMIPB_MAIL_SUPPORT
                mail_info_ptr, 
#else
                PNULL,
#endif
                &number_t, 
                temp1_wstr_ptr,
                &i, 
                field_type, 
                &number_index, 
                &item_data);
        }
        else
        {
            AppendDyncName(win_id , entry_name_ptr, temp1_wstr_ptr,&i, &item_data);
        }
       
    
        if(MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
        {
            SCI_MEMSET(temp2_wstr_ptr, 0x00, sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
            if(num_index < MMIPB_MAX_NV_PHONE_NUM)
            {
                
                AppendDyncNumber(win_id, 
#ifdef MMIPB_MAIL_SUPPORT
                    mail_info_ptr, 
#else
                    PNULL,
#endif
                    &number_t, 
                    temp2_wstr_ptr,
                    &i,
                    field_type,
                    &number_index, 
                    &item_data);
            }
            else
            {
                 AppendDyncNumber(win_id,
#ifdef MMIPB_MAIL_SUPPORT
                    mail_info_ptr, 
#else
                    PNULL,
#endif
                    &number_t, 
                    temp2_wstr_ptr,
                    &i,
                    MMIPB_LIST_FIELD_MAIL,
                    &number_index, 
                    &item_data);
            }
        }

    }

    if(dync_type == MMIPB_NEED_LIST_ITEM_DATA_TYPE)
    {
        GUILIST_SetItemData( ctrl_id, &item_data, index);
    }
    else if(dync_type == MMIPB_NEED_LIST_ITEM_CONTENT_TYPE)
    {
        if((item_data.item_content[item_content_index].item_data_type == GUIITEM_DATA_ANIM_PATH) || (item_data.item_content[item_content_index].item_data_type == GUIITEM_DATA_ANIM_DATA))
        {
        
            GUILIST_SetItemContent( ctrl_id, &item_data.item_content[item_content_index], index, item_content_index );
        }
    }

    SCI_FREE(temp1_wstr_ptr);
    SCI_FREE(temp2_wstr_ptr);
    SCI_FREE(temp3_wstr_ptr);
    SCI_FREE(entry_name_ptr);
#ifdef MMIPB_MAIL_SUPPORT
    SCI_FREE(mail_info_ptr);
#endif
#ifdef MMIPB_MOST_USED_SUPPORT    
    SCI_FREE(mostused_contact_node_ptr);
#endif
    SCI_FREE(custom_photo_ptr);
}


/*****************************************************************************/
//  Description : 从电话簿中获得显示的字符串
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 用于装载列表控件的item时使用
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayStringFromPhonebookName
(
    MMI_STRING_T        *dist_ptr,  //OUT: 获得应该显示的字符串
    const MMIPB_NAME_T  *name_ptr   //IN: entry data
)
{
    if(PNULL == dist_ptr || (PNULL == dist_ptr->wstr_ptr) || (PNULL == name_ptr))
    {
        //SCI_TRACE_LOW:"[MMIPB] GetDisplayStringFromPhonebookName dist_ptr 0x%x name_ptr 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2757_112_2_18_2_44_19_322,(uint8*)"dd", dist_ptr,name_ptr);
        return FALSE;
    }    
    if (name_ptr->wstr_len > 0)
    {
        MMIPB_ConvertNameToString(
            dist_ptr,
            name_ptr
            );
    }
    else
    {
        MMI_STRING_T    noname_str = {0};
        // 姓名为空
        MMI_GetLabelTextByLang(
            TXT_NO_NAME, 
            &noname_str
            );
         
        //dist_ptr->is_ucs2 = noname_str.is_ucs2;
        dist_ptr->wstr_len = noname_str.wstr_len;
        MMIAPICOM_Wstrncpy(dist_ptr->wstr_ptr, noname_str.wstr_ptr, dist_ptr->wstr_len);
    }
    
    return (TRUE);
}



 /*****************************************************************************/
//  Description : 为查找列表显示list item
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

LOCAL void SetListItemForSearch
(
    MMI_HANDLE_T                  ctrl_handle,
    GUIITEM_STYLE_E               listbox_type,
    uint32                        list_data,
    uint16                        first_letter,
    BOOLEAN                       is_allow_silde
)
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    item_t.item_style    = listbox_type;
    item_t.item_state |=GUIITEM_STATE_CONTENT_CHECK;
    if(!is_allow_silde)
    {
        item_t.item_state |=GUIITEM_STATE_NOTHOR_SLIDE;
    }
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = list_data;
    item_t.first_letter  = first_letter;
    GUILIST_AppendItem(ctrl_handle, &item_t);
}

#if 1 //#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL wchar* MMIPB_Wstrupper( MMI_STRING_T * string )
{
    wchar *start = string->wstr_ptr;
    wchar  *str = string->wstr_ptr;
    uint16 len = string->wstr_len;

    while(len >0 &&str!=PNULL)
    {
       *str = MMIAPICOM_Wchartoupper( *str );/*lint !e564 */
       str++;
       len--;
    }

    return start;
}
/*****************************************************************************/
//  Discription: append spliter
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendSplitListItem(	 MMI_CTRL_ID_T    ctrl_id, wchar first_letter)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;
    item_t.first_letter = first_letter;
    item_t.item_data_ptr = &item_data;
    item_t.item_state |=GUIITEM_STATE_SPLIT;
    item_t.user_data = SPLIT_DATA;

    GUILIST_SetSplitOnside(ctrl_id, TRUE);

    GUILIST_AppendItem( ctrl_id, &item_t );

}
#endif
 
 /*****************************************************************************/
//  Description : 切换到相应应用的窗口 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SwitchToAppTab(MMI_HANDLE_T applet_handle, MMIPB_SUB_FUNCTION_TYPE_E app_type)
{
	uint32                  tab_index = 0;
	uint32                  tab_count = 0;
	uint32                  i  = 0;
	MMI_WIN_ID_T focus_sub_win_id = 0;
    MMIPB_WINDOW_TAB_INFO_T *sub_tab_ptr = PNULL;
    MMIPB_APPLET_T*          applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] SwitchToAppTab PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2887_112_2_18_2_44_19_323,(uint8*)"");
        return;
    }

#ifdef MMI_PDA_SUPPORT   
    if(applet_ptr->instance.list_type == MMIPB_LIST_CHECK)
    {
        sub_tab_ptr = s_mmipb_pda_select_win_tab;
        tab_count = ARR_SIZE(s_mmipb_pda_select_win_tab);
    }
#if defined MMI_ISTYLE_SUPPORT
	else if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle()) 
	{ 
		sub_tab_ptr = s_mmipb_istyle_win_tab;
		tab_count = ARR_SIZE(s_mmipb_istyle_win_tab);
	} 
#endif	
	else	
	{
		sub_tab_ptr = s_mmipb_pda_win_tab;
		tab_count = ARR_SIZE(s_mmipb_pda_win_tab);
	}
#else 
	sub_tab_ptr = s_mmipb_normal_win_tab;
	tab_count = ARR_SIZE(s_mmipb_normal_win_tab);
#endif
	for(i = 0; i < tab_count; i++)
	{
		//create this sub window
		if(app_type == sub_tab_ptr[i].app_type)
		{
			tab_index = i;
			focus_sub_win_id = sub_tab_ptr[i].win_id;
			break;
		}
	}
    GUITAB_SetCurSel(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMIPB_SEARCH_LIST_TAB_CTRL_ID), i);
	MMK_SetChildWinFocus(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMK_GetWinHandle(applet_handle,focus_sub_win_id));
	return;
}
/*****************************************************************************/
//  Description : get curren application type 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIPB_SUB_FUNCTION_TYPE_E GetCurAppType(MMI_HANDLE_T applet_handle, uint32 cur_index)
{
	MMIPB_SUB_FUNCTION_TYPE_E          app_type = MMIPB_ALL_CONTACT;
	uint32                  tab_count = 0;
    MMIPB_WINDOW_TAB_INFO_T *sub_tab_ptr = PNULL;
    MMIPB_APPLET_T*  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"GetCurAppType PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_2938_112_2_18_2_44_19_324,(uint8*)"");
        return app_type;
    }
 
#ifdef MMI_PDA_SUPPORT   
    if(applet_ptr->instance.list_type == MMIPB_LIST_CHECK)
    {
        sub_tab_ptr = s_mmipb_pda_select_win_tab;
        tab_count = ARR_SIZE(s_mmipb_pda_select_win_tab);
    }
#if defined MMI_ISTYLE_SUPPORT	
	else if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle()) 
	{ 
		sub_tab_ptr = s_mmipb_istyle_win_tab;
		tab_count = ARR_SIZE(s_mmipb_istyle_win_tab);
	} 
#endif	
	else
	{
		sub_tab_ptr = s_mmipb_pda_win_tab;
		tab_count = ARR_SIZE(s_mmipb_pda_win_tab);
	}
#else 
	sub_tab_ptr = s_mmipb_normal_win_tab;
	tab_count = ARR_SIZE(s_mmipb_normal_win_tab);
#endif
    if(cur_index < tab_count)
	{
         app_type = sub_tab_ptr[cur_index].app_type;
	}  
	return app_type;
} 

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 取得下一个group id
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetNextGroupID(uint8 cur_group_id)
{
    uint8 group_id = 0;
    //MN_DUAL_SYS_E daul_type = MN_DUAL_SYS_1;
    MMIPB_NAME_T name = {0};

    for(group_id = cur_group_id + 1; group_id < MMIPB_MAX_GROUP_NUM; group_id++)
    {
        if(MMIPB_IsInSIMGroup(group_id))
        {
            if(MMIAPIPHONE_GetSimExistedStatus(group_id - PB_GROUP_SIM_1 + MN_DUAL_SYS_1))
            {
                //sim group and this sim exist
                break;
            }
        }         

#ifdef SNS_SUPPORT
        else if(!MMIPB_IsSNSGroup(group_id))
#else
        else
#endif                    
        {
            if(
#ifdef MMIPB_MOST_USED_SUPPORT                
                !MMIPB_IsFixedGroup(group_id) && 
#endif
            (MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name, group_id))
            )
            {
                //phone valid group
                break;
            }
        }

    }
	if(group_id >= MMIPB_MAX_GROUP_NUM)
	{
		group_id = 0;
	}
#ifdef BT_PBAP_SUPPORT
	if(PB_GROUP_BT == group_id)
	{
		group_id = 0;
	}
#endif
	return group_id;
} 
#endif
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 取得上一个group id
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetPreviousGroupID(uint8 cur_group_id)
{
    uint8 group_id = 0;
//    MN_DUAL_SYS_E daul_type = MN_DUAL_SYS_1;
    MMIPB_NAME_T name = {0};

    if(cur_group_id > 0)
    {
        for(group_id = cur_group_id -1; group_id > 0; group_id--)
        {
            if(MMIPB_IsInSIMGroup(group_id))
            {
                if(MMIAPIPHONE_GetSimExistedStatus(group_id - PB_GROUP_SIM_1 + MN_DUAL_SYS_1))
                {
                    //sim group and this sim exist
                    break;
                }
            }

  
#ifdef SNS_SUPPORT
            else if(!MMIPB_IsSNSGroup(group_id))
#else
            else
#endif    
            {
                //phone valid group
                if(
#ifdef MMIPB_MOST_USED_SUPPORT
                !MMIPB_IsFixedGroup(group_id) && 
#endif
                (MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name, group_id)))
                {
                    break;
                }
            }            
        }
    }
    
	return group_id;
}
#endif
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note:  OUT :contact_ptr
//          IN win_id
//          IN ctrl_id
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_GetContactNameByListIndex(MMI_WIN_ID_T win_id, 
                                              MMI_CTRL_ID_T ctrl_id,
                                              uint16  index,
                                              MMIPB_NAME_T* name_ptr)
{
    uint16 contact_index = 0;
    uint16 entry_id = 0;
    uint16 storage_id = 0;
    GUILIST_ITEM_T* list_item_ptr =PNULL;
    MMIPB_APPLET_T* applet_ptr =  (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
   
    if((PNULL == applet_ptr) || (PNULL == name_ptr))
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_3065_112_2_18_2_44_20_325,(uint8*)"");
        return FALSE;
    }

    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
        list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);/*lint !e605*/
        if(PNULL  == list_item_ptr)
        {
            //SCI_TRACE_LOW:"[mmipb MMIPB_GetContactInfoByListIndex] PNULL  == list_item_ptr %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_3074_112_2_18_2_44_20_326,(uint8*)"d", index);
            return FALSE;
        }

        contact_index = MMIPB_CONTACTINDEX(list_item_ptr->user_data);
#ifdef MMIPB_MOST_USED_SUPPORT
        if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
        {
            MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node = {0};
            MMIPB_CONTACT_T *contact_ptr = PNULL;

            contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                mostused_contact_node = MMIPB_GetMostUsedContactBaseInfo(contact_index);
                entry_id = mostused_contact_node.entry_id;
                storage_id = mostused_contact_node.storage_id;

                MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
                SCI_MEMCPY(name_ptr, &contact_ptr->name, sizeof(MMIPB_NAME_T));
                SCI_FREE(contact_ptr);
            }
        }
        else
#endif
        {
            MMIPB_NAME_NUMBER_MAIL_T *string_ptr = PNULL;

            string_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_NUMBER_MAIL_T));
            if(PNULL != string_ptr)
            {
                SCI_MEMSET(string_ptr, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
                MMIPB_GetContactNameNumberByIndex(string_ptr, contact_index);
                SCI_MEMCPY(name_ptr, &string_ptr->name, sizeof(MMIPB_NAME_T));
                SCI_FREE(string_ptr);
            }
        }
  
    }
    return TRUE;
}
#endif
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT
/*****************************************************************************/
//  Description : 根据搜索结果将字符串进行颜色设置
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/ 
LOCAL void AppendSearchItemColor(MMIPB_INSTANCE_T* pb_instance, 
                                 MMI_WIN_ID_T win_id,
                                 GUILIST_EFFECT_STR_T *effect_str, 
                                 MMI_STRING_T edit_str)
{
    MMI_STRING_T name_str = {0};
    MMI_STRING_T index_str = {0};
    wchar *name_wchar = SCI_ALLOC_APPZ(MMIPB_MAX_STRING_LEN*sizeof(wchar));
    wchar index_wchar[MMIPB_MAX_STRING_LEN] = {0};
    MMIPB_NAME_T *pb_name_ptr = PNULL;
    int index = 0;	

    if(PNULL == pb_instance) 
    {
        SCI_TRACE_LOW("mmipb AppendSearchItemColor PNULL == pb_instance");
        return;
    }
    pb_name_ptr = (MMIPB_NAME_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_T));
    if(pb_name_ptr != PNULL)
	{
        SCI_MEMSET(pb_name_ptr, 0x00, sizeof(MMIPB_NAME_T));

        if (1 == effect_str->content_index)
        {               

            if(( 0 < edit_str.wstr_len))
            {
        
                //MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
                name_str.wstr_ptr = name_wchar;
                index_str.wstr_ptr = index_wchar;
                MMIPB_GetContactNameByListIndex(win_id,
                                                MMIPB_GetListHandleByWin(win_id),
                                                effect_str->item_index, 
                                                pb_name_ptr);

                MMIAPICOM_Wstrcpy(name_str.wstr_ptr, pb_name_ptr->wstr);
                name_str.wstr_len = pb_name_ptr->wstr_len;
                if(MMIAPIPB_Wstrstrpos(&name_str, &edit_str, &index_str, TRUE))
                {
        
                    if (0 < index_str.wstr_len)
                    {
                        effect_str->effect_type = GUILIST_EFFECT_REGION;
                        effect_str->region_num = index_str.wstr_len;
                        effect_str->region_ptr = SCI_ALLOC_APP((index_str.wstr_len) * sizeof(GUIFONT_REGION_STYLE_T));
            
                        for (index = 0; index < index_str.wstr_len; index++)
                        {
                            effect_str->region_ptr[index].start = index_wchar[index];
                            effect_str->region_ptr[index].length = 1;
                            effect_str->region_ptr[index].state = GUISTR_STATE_SPEC_FONTCOLOR;
                            effect_str->region_ptr[index].font_color = MMI_DARK_GREEN_COLOR;
                        }
                    }
                }
                else
                {
                    effect_str->font_color = MMI_CYAN_COLOR;
                    effect_str->state |= GUISTR_STATE_SPEC_FONTCOLOR;
                    effect_str->effect_type = GUILIST_EFFECT_STR;

                    effect_str->str_info.wstr_len = edit_str.wstr_len;
                    effect_str->str_info.wstr_ptr = edit_str.wstr_ptr;
                 }
            }
        }
        SCI_FREE(pb_name_ptr);
    }

    
    if((MMIPB_LIST_CHECK != pb_instance->list_type) || (MMIPB_OPEN_MAIN_LIST!=pb_instance->function_type ))
    {
     
        if (2 == effect_str->content_index)
        {
            if (0 < index_str.wstr_len)
            {
                effect_str->effect_type = GUILIST_EFFECT_REGION;
                effect_str->region_num = index_str.wstr_len;
                effect_str->region_ptr = SCI_ALLOC_APP((index_str.wstr_len) * sizeof(GUIFONT_REGION_STYLE_T));
    
                for (index = 0; index < index_str.wstr_len; index++)
                {
                    effect_str->region_ptr[index].start = index_wchar[index];
                    effect_str->region_ptr[index].length = 1;
                    effect_str->region_ptr[index].state = GUISTR_STATE_SPEC_FONTCOLOR;
                    effect_str->region_ptr[index].font_color = MMI_DARK_GREEN_COLOR;
                }
            }
            else
            {
                effect_str->font_color = MMI_CYAN_COLOR;
                effect_str->state |= GUISTR_STATE_SPEC_FONTCOLOR;
                effect_str->effect_type = GUILIST_EFFECT_STR;

                effect_str->str_info.wstr_len = edit_str.wstr_len;
                effect_str->str_info.wstr_ptr = edit_str.wstr_ptr;
             }
        }                     

    }
    SCI_FREE(name_wchar);    
}
#endif 
 
#if defined(TOUCH_PANEL_SUPPORT) && defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
/*****************************************************************************/
//  Description : Update EntryList Toolbar State 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdateEntryListToolbarState( 
                                 MMI_HANDLE_T        ctrl_handle,
                                 void*               param_ptr
                                 )
{
    MMI_CTRL_ID_T           ctrl_id = (MMI_CTRL_ID_T)param_ptr;
    
    
    if((GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0) >= 2)
        || (0 == GUILIST_GetTotalItemNum(ctrl_id)))
    {
        GUITOOLBAR_SetItemGray(ctrl_handle, 0, TRUE); 
        GUITOOLBAR_SetItemGray(ctrl_handle, 1, TRUE); 
        GUITOOLBAR_SetItemGray(ctrl_handle, 2, TRUE); 
        GUITOOLBAR_SetItemGray(ctrl_handle, 3, TRUE); 
#ifdef VT_SUPPORT
        GUITOOLBAR_SetItemGray(ctrl_handle, 4, TRUE); 
#endif
    }
    else 
    {
        GUITOOLBAR_SetItemGray(ctrl_handle, 0, FALSE); 
        GUITOOLBAR_SetItemGray(ctrl_handle, 1, FALSE); 
        GUITOOLBAR_SetItemGray(ctrl_handle, 2, FALSE); 
        GUITOOLBAR_SetItemGray(ctrl_handle, 3, FALSE); 
#ifdef VT_SUPPORT
        GUITOOLBAR_SetItemGray(ctrl_handle, 4, FALSE); 
#endif
    }
}
#endif

#if defined(TOUCH_PANEL_SUPPORT) && defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL  void  SetToolBarState(MMI_HANDLE_T hToolbar, MMIPB_CONTACT_T* contact_ptr)
{
    uint16 valid_num = 0;
    valid_num = GetValidNumberCount((MMIPB_BCD_NUMBER_T *)&contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM); 
    if(valid_num == 0)
    {
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_AUDIO_INDEX, TRUE);
#ifdef VT_SUPPORT
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_VIDEO_INDEX, TRUE);
#endif        
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_IP_INDEX, TRUE);
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_SMS_INDEX, TRUE);

    }
    else
    {
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_AUDIO_INDEX, FALSE);
#ifdef VT_SUPPORT
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_VIDEO_INDEX, FALSE);
#endif        
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_IP_INDEX, FALSE);
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_SMS_INDEX, FALSE);

    }
#ifdef MMIPB_MAIL_SUPPORT 
    if(contact_ptr->mail.wstr_len == 0)
    {
       
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_EMAIL_INDEX, TRUE);
    }
    else
    {     
        GUITOOLBAR_SetItemGray(hToolbar, MMIPB_PDA_TOOLBAR_EMAIL_INDEX, FALSE);

    }
#endif
}
#endif

#if defined(TOUCH_PANEL_SUPPORT) && defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
/*****************************************************************************/
//  Description : CREATE PB Toolbar 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void PbCreateToolbar(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T list_id)
{

    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    
    GUITOOLBAR_INIT_DATA_T init_data = {0};
    GUITOOLBAR_ITEM_T pb_toolbar_item_a [] = 
    {
        {PB_OPTMENU_NODE_DIAL_AUDIO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_CALL, IMAGE_TOOLBAR_CL_CALL_GRAY,0},/*lint !e785*/
#ifdef VT_SUPPORT
        {PB_OPTMENU_NODE_DIAL_VIDEO, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_VIDEOCALL, IMAGE_TOOLBAR_CL_VIDEOCALL_GRAY,0},/*lint !e785*/
#endif
#ifdef MMI_IP_CALL_SUPPORT
        {PB_OPTMENU_NODE_DIAL_IP, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_IPCALL, IMAGE_TOOLBAR_CL_IPCALL_GRAY,0},/*lint !e785*/
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        {PB_OPTMENU_NODE_SEND_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_WRITESMS, IMAGE_TOOLBAR_CL_WRITESMS_GRAY,0},/*lint !e785*/
#else
        {PB_OPTMENU_NODE_SEND_SMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_WRITESMS, IMAGE_TOOLBAR_CL_WRITESMS_GRAY,0},/*lint !e785*/
        {PB_OPTMENU_NODE_SEND_MMS, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_MMS, IMAGE_TOOLBAR_CL_MMS_GTAY,0},/*lint !e785*/
#endif
#ifdef MMIEMAIL_SUPPORT
        {PB_OPTMENU_NODE_SEND_EMAIL, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_TOOLBAR_CL_EMAIL, IMAGE_TOOLBAR_CL_EMAIL_GRAY,0},/*lint !e785*/
#endif
       // {PB_OPTMENU_NODE_ADDNEW, GUITOOLBAR_ITEM_IMAGE_ID, IMAGE_PHONEBOOK_TOOLBAR_ADD, IMAGE_PHONEBOOK_TOOLBAR_ADD_GRAY,0},/*lint !e785*/
    };  

    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"PbCreateToolbar PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_3334_112_2_18_2_44_20_327,(uint8*)"");
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;  
    
    if(MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
    {
        return;
    }
    
    MMK_GetBothRect(win_id, &init_data.both_rect);
    
    GUITOOLBAR_CreateDynamic(win_id, ctrl_id, &init_data);
    GUITOOLBAR_SetAllItem(MMK_GetCtrlHandleByWin(win_id, ctrl_id), 
        pb_toolbar_item_a, 
        sizeof(pb_toolbar_item_a)/sizeof(GUITOOLBAR_ITEM_T));
    
    GUITOOLBAR_SetPreExpandFunc(MMK_GetCtrlHandleByWin(win_id, ctrl_id), (TOOLBAR_PRE_EXPAND_FUNC)UpdateEntryListToolbarState,
        (void*)list_id); 
}
#endif

#if defined(TOUCH_PANEL_SUPPORT) && defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
/*****************************************************************************/
//  Description : UPDATE PB Toolbar STATUS
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdatePbListWinToolbar(
                            MMI_CTRL_ID_T      list_ctrl,
                            MMI_HANDLE_T      toolbar_ctrl_handle

                            )
{
   

    {
        uint16  icon_width = 0;
        uint16  icon_height = 0;
        GUI_RECT_T cur_rect = {0};
        GUI_POINT_T pt_bar = {0};
        BOOLEAN  bMark = FALSE;
        
        
        uint32 cur_index = GUILIST_GetCurItemIndex(list_ctrl);
        bMark = GUILIST_GetListStateById( list_ctrl, GUILIST_STATE_ENABLE_MARK );
        GUITOOLBAR_SetVisible(toolbar_ctrl_handle, !bMark);/*lint !e730*/
        
        GUILIST_GetItemRect(list_ctrl, cur_index, &cur_rect);
        GUITOOLBAR_GetIconWidthHeight(toolbar_ctrl_handle, &icon_width, &icon_height);
        
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            pt_bar.x = cur_rect.right;
        }
        else
        {
            pt_bar.x = cur_rect.left;
        }


        pt_bar.y = cur_rect.top - 1;//cur_rect.bottom - MMIPB_TOOLBAR_PANEL_GAP + 1; //向上
        GUITOOLBAR_SetPanelPopForm(toolbar_ctrl_handle, GUITOOLBAR_DIR_DOWN);
       
        GUITOOLBAR_SetPanelBasePt(toolbar_ctrl_handle, &pt_bar, FALSE);
        

    }
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_CreateNewSoftKey(MMI_HANDLE_T win_handle,   MMI_HANDLE_T form_id ,MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    GUIFORM_DYNA_CHILD_T child_data = {0};
    //MMI_CTRL_ID_T   softkey_ctrl_id = 0;

    child_data.is_softkey = TRUE;
    child_data.guid=SPRD_GUI_SOFTKEY_ID;
    child_data.child_handle=MMIPB_SOFTKEY_CTRL_ID;
    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;
    child_data.init_data_ptr=&softkey_data;
    GUIFORM_CreatDynaChildCtrl(win_handle,form_id,&child_data);
    return TRUE;
}
#endif



/*****************************************************************************/
//  Description : 配置tab窗口所有控件
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitSubTabControls(MMI_WIN_ID_T win_id, //IN:tab窗口id
								MMIPB_LIST_HANDLE   *handle_ptr//纪录列表信息
								)
{
#ifdef MMI_PDA_SUPPORT
    GUIPRGBOX_STYLE_E                   prg_type = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
#else
#if defined(MMIPB_MOST_USED_SUPPORT) || defined(BT_PBAP_SUPPORT)
    GUIPRGBOX_STYLE_E                   prg_type = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
#endif
#endif
    GUIFORM_CHILD_HEIGHT_T              height_info = {0};
    GUIFORM_CHILD_WIDTH_T               width_info = {0};
    GUI_BG_T                            button_bg = {0};
    MMI_HANDLE_T                        form_ctr_id = NULL;
    MMI_HANDLE_T                        subform_ctr_id = NULL;
    MMI_HANDLE_T                        search_edit_form_ctr_id = NULL;
#ifdef MMI_PDA_SUPPORT
    MMI_HANDLE_T                        add_ctr_id = NULL;
#endif
    MMI_HANDLE_T                        edit_ctr_id = NULL;
    MMI_HANDLE_T                        search_ctr_id = NULL;
    MMI_HANDLE_T                        list_ctr_id = NULL;
#ifdef MMI_PDA_SUPPORT
#ifdef MMIPB_MOST_USED_SUPPORT
    MMI_HANDLE_T                        dropdownlist_ctr_id = NULL;
#endif
#endif
    MMI_HANDLE_T                        select_label_ctr_id = NULL;
    MMI_HANDLE_T                        select_button_ctr_id = NULL;
    MMI_HANDLE_T                      toolbar_ctr_id = NULL;
    uint16                              img_width = 0;
    uint16                              img_height = 0;
    wchar                               forbid_char_value[]= {0x0d, 0x0a};//禁止换行回车的输入
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    uint16          hor_space = 0;
    uint16          ver_space = 0;
    if(handle_ptr == PNULL)
    {
        return;
    }
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)  //+ for coverity
    {
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;  

    applet_ptr->instance.cur_group_id = (PB_GROUP_LIST_E)handle_ptr->group_id;//当前群组和当前的contact_type跟applet绑定做备份
    applet_ptr->instance.contacts_type = handle_ptr->contacts_type;    
#ifdef MMI_PDA_SUPPORT

    if(MMIPB_ALL_GROUP_CONTACTS == (MMIPB_CONTACTS_TYP_E)applet_ptr->instance.contacts_type )
    {
        //全部联系人窗口
        form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_CONTACT_SEARCH_WIN_FORM_ID);
        subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_ALL_CONTACT_SEARCH_WIN_FORM_ID);
        search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_ALL_CONTACT_SEARCH_EDIT_FORM_ID);
        add_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_SEARCH_ADD_BUTTON_CTRL_ID);
        edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID);
        search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_SEARCH_BUTTON_CTRL_ID);
        list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID);
        toolbar_ctr_id = MMIPB_TOOLBAR_LISTBOX_CTRL_ID;//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
        prg_type = GUIPRGBOX_STYLE_LETTERS_SCROLL;
        select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_ALL_SELECT_TITLE_CTRL_ID);
        select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_ALL_SELECT_BUTTON_CTRL_ID);
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
        SetPbTitleIcon(win_id, IMG_PB_MANAGE_GROUP_ICON, IMG_PB_MANAGE_GROUP_ICON, FALSE);
#endif
#endif
        GUIFORM_SetSpace(subform_ctr_id, &hor_space, &ver_space);
        GUIFORM_SetSpace(search_edit_form_ctr_id, &hor_space, &ver_space);
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    else if(MMIPB_MOSTUSED_CONTACTS == applet_ptr->instance.contacts_type )
    {
        //常用联系人窗口
        uint32 i=0;
        uint32 text_id[] =
                {
                TXT_PB_FAVOURITE,
                TXT_PB_RECENT_USED,
                };

        form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID);
        subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID);
        dropdownlist_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_CONTACT_DROPDOWNLIST_CTRL_ID);
        select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_MOSTUSED_SELECT_TITLE_CTRL_ID);
        select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_MOSTUSED_SELECT_BUTTON_CTRL_ID);
        GUIDROPDOWNLIST_SetMaxItem(dropdownlist_ctr_id, ARR_SIZE(text_id));
        for(i=0; i<ARR_SIZE(text_id); i++) //pclint error 574
        {
            GUIDROPDOWNLIST_AppendItemById(dropdownlist_ctr_id, text_id[i]);
        }
        GUIDROPDOWNLIST_SetCurItemIndex(dropdownlist_ctr_id,0);
        GUIDROPDOWNLIST_SetButton(win_id, dropdownlist_ctr_id, TRUE);

        list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID);
        toolbar_ctr_id = MMIPB_MOSTUSED_TOOLBAR_LISTBOX_CTRL_ID;
        prg_type = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
        SetPbTitleIcon(win_id, IMG_PB_MANAGE_GROUP_ICON, IMG_PB_MANAGE_GROUP_ICON, FALSE);
#endif
#endif

    }
#endif
    else
    {
        //单个分组联系人窗口
        form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_CONTACT_SEARCH_WIN_FORM_ID);
        subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_GROUP_CONTACT_SEARCH_WIN_FORM_ID);
        search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_GROUP_CONTACT_SEARCH_EDIT_FORM_ID);
        edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID);
        search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_SEARCH_BUTTON_CTRL_ID);
        list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_ENTRY_LISTBOX_CTRL_ID);
        toolbar_ctr_id =MMIPB_GROUP_TOOLBAR_LISTBOX_CTRL_ID;
        prg_type = GUIPRGBOX_STYLE_LETTERS_SCROLL;
        select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_GROUP_SELECT_TITLE_CTRL_ID);
        select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_GROUP_SELECT_BUTTON_CTRL_ID);
        if((MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type) 
            &&MMIPB_IsInNvGroup(applet_ptr->instance.cur_group_id)
            )
        {
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
           SetPbTitleIcon(win_id, IMG_PB_MANAGE_GROUP_ICON, IMG_PB_MANAGE_GROUP_ICON, TRUE);
#endif
#endif
        }
        else
        {
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
                SetPbTitleIcon(win_id, IMG_PB_MANAGE_GROUP_ICON, IMG_PB_MANAGE_GROUP_ICON, FALSE);
#endif
#endif
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 
		      if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
		  	    {
				    //cancel and ok
				    //取消sk
				    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
				    GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_CANCEL, FALSE);
				    //完成sk
				    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
				    GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_FINISH, FALSE);
              }
#endif
        }


        GUIFORM_SetSpace(subform_ctr_id, &hor_space, &ver_space);
        GUIFORM_SetSpace(search_edit_form_ctr_id, &hor_space, &ver_space);
    }
#else
        if(MMIPB_ALL_GROUP_CONTACTS == applet_ptr->instance.contacts_type)
        {
            form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_SEARCH_WIN_FORM_ID);
            subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_CONTACT_SEARCH_WIN_FORM_ID);
            search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_CONTACT_SEARCH_EDIT_FORM_ID);
            edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_SEARCH_EDITBOX_CTRL_ID);
            search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_SEARCH_BUTTON_CTRL_ID);
            list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LISTBOX_CTRL_ID);
            toolbar_ctr_id = MMIPB_TOOLBAR_LISTBOX_CTRL_ID;
            select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SELECT_TITLE_CTRL_ID);
            select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SELECT_BUTTON_CTRL_ID);


            GUIFORM_SetSpace(subform_ctr_id, &hor_space, &ver_space);
            GUIFORM_SetSpace(search_edit_form_ctr_id, &hor_space, &ver_space);
        }
#ifdef MMIPB_MOST_USED_SUPPORT
        else if(MMIPB_MOSTUSED_CONTACTS == applet_ptr->instance.contacts_type )
        {
            //常用联系人窗口
		     form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID);
		     list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID);
             toolbar_ctr_id = MMIPB_MOSTUSED_TOOLBAR_LISTBOX_CTRL_ID;
             prg_type = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;

        }
#endif
#ifdef BT_PBAP_SUPPORT
        else if(MMIPB_BT_CONTACTS == applet_ptr->instance.contacts_type )
        {
            //蓝牙联系人窗口
			form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_CONTACT_SEARCH_WIN_FORM_ID);
            subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_CONTACT_SEARCH_WIN_FORM_ID);
            search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_CONTACT_SEARCH_EDIT_FORM_ID);
            edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_CONTACT_SEARCH_EDITBOX_CTRL_ID);
            search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_ENTRY_SEARCH_BUTTON_CTRL_ID);
			list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_ENTRY_LISTBOX_CTRL_ID);
			toolbar_ctr_id = MMIPB_BT_TOOLBAR_LISTBOX_CTRL_ID;
            select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_SELECT_TITLE_CTRL_ID);
            select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_SELECT_BUTTON_CTRL_ID);
			prg_type = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
			
            GUIFORM_SetSpace(subform_ctr_id, &hor_space, &ver_space);
            GUIFORM_SetSpace(search_edit_form_ctr_id, &hor_space, &ver_space);
        }
#endif
        else if(MMIPB_SINGLE_GROUP_CONTACTS == applet_ptr->instance.contacts_type)
        {
            MMIPB_NAME_T group_name = {0};
            form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID);
            subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID);
            search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_EDIT_FORM_ID);
            edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID);
            search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_ENTRY_SEARCH_BUTTON_CTRL_ID);
            list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_ENTRY_LISTBOX_CTRL_ID);
            toolbar_ctr_id = MMIPB_TOOLBAR_LISTBOX_CTRL_ID;
            select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_SELECT_TITLE_CTRL_ID);
            select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_SELECT_BUTTON_CTRL_ID);
            MMIPB_ReadGroupName(&group_name, applet_ptr->instance.cur_group_id);
            GUIWIN_SetTitleText(win_id, group_name.wstr, group_name.wstr_len, FALSE);


            GUIFORM_SetSpace(subform_ctr_id, &hor_space, &ver_space);
            GUIFORM_SetSpace(search_edit_form_ctr_id, &hor_space, &ver_space);
        }
        if(MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
        {
            GUILIST_SetOwnSofterKey(list_ctr_id,TRUE);
        }
#endif        

    //set form 
    GUIFORM_IsSlide(form_ctr_id, FALSE);
  //  GUIFORM_SetMargin(form_ctr_id, 0);
  //  ver_space = 2;
    GUIFORM_SetSpace(form_ctr_id, &hor_space, &ver_space);
    
#ifdef MMI_PDA_SUPPORT       
    GUIFORM_PermitChildBg(form_ctr_id, FALSE);

    if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
    {
        //添加按钮:"+"
        //istyle not this button
        if(MMIPB_LIST_CHECK == applet_ptr->instance.list_type)
        {
            GUIBUTTON_SetVisible(add_ctr_id, FALSE, FALSE);
            width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
            width_info.add_data = 0;
            GUIFORM_SetChildWidth(subform_ctr_id, add_ctr_id, &width_info);
        }
        else
        {  
            GUI_BG_T  button_bg_info = {0};
            GUIRES_GetImgWidthHeight(&img_width, &img_height, MMIPB_CONTACT_ADD,win_id);
            width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
            width_info.add_data = img_width;
            GUIFORM_SetChildWidth(subform_ctr_id, add_ctr_id, &width_info);
            button_bg_info.bg_type= GUI_BG_IMG;
            button_bg_info.img_id = MMIPB_CONTACT_ADD_PRESSED;
            GUIBUTTON_SetPressedBg(add_ctr_id,&button_bg_info);
            GUIBUTTON_SetRunSheen(add_ctr_id, FALSE);
        }


        //not normal set softkey
			if(MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
			{			
				MMIPB_CreateNewSoftKey(win_id, form_ctr_id,STXT_ADD, TXT_NULL, STXT_CANCEL);
                GUISOFTKEY_SetButtonGray(win_id, MMIPB_SOFTKEY_CTRL_ID,LEFT_BUTTON, TRUE);
			}
    }
    
    if(MMIPB_LIST_RADIO != applet_ptr->instance.list_type && MMIPB_LIST_CHECK != applet_ptr->instance.list_type )//不需要字母滚动条
    {
        GUILIST_SetListScrollBarStyle(list_ctr_id, prg_type);
    }     
#else
    if(MMIPB_LIST_RADIO== applet_ptr->instance.list_type)
    {
        GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_OK, TXT_NULL, STXT_CANCEL,FALSE);
    }
    else if(MMIPB_LIST_CHECK== applet_ptr->instance.list_type)
    {
        GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_OK, TXT_NULL, STXT_CANCEL,FALSE);
        GUILIST_ChangeCheckListSoftkeyIndex(list_ctr_id, MIDDLE_BUTTON);
    }
    GUIFORM_PermitChildBg(form_ctr_id, FALSE);
#endif
	

#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_MOSTUSED_CONTACTS != applet_ptr->instance.contacts_type)
#endif
    {
		//+Bug 119450
        BOOLEAN is_disp_im_icon = TRUE;
	    BOOLEAN is_disp_num_info = TRUE;
		//-Bug 119450

        //搜索输入框form控件
        IGUICTRL_T  *form_ptr = MMK_GetCtrlPtr(search_edit_form_ctr_id);
        GUI_BORDER_T  border = {0};

        GUIFORM_PermitChildBorder(subform_ctr_id,FALSE);
        GUIFORM_PermitChildBorder(search_edit_form_ctr_id,TRUE);
        if (PNULL != form_ptr )
        {
            border.type = GUI_BORDER_SOLID;
            border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_DEFAULT_FONT);
            border.width = 1;
            IGUICTRL_SetBorder(form_ptr , &border);
        }

        //搜索输入框:editbox
        GUIEDIT_SetMargin(edit_ctr_id,10, 1);
        GUIEDIT_SetDispImIcon(edit_ctr_id,&is_disp_im_icon,&is_disp_num_info);
        GUIEDIT_SetIm(edit_ctr_id, GUIIM_TYPE_ABC | GUIIM_TYPE_SMART | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH,GUIIM_TYPE_ABC);/*lint !e655*/

        GUIEDIT_SetStyle(edit_ctr_id,GUIEDIT_STYLE_SINGLE);
        GUIEDIT_ConfigDisableChar(edit_ctr_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
        GUIEDIT_SetTextMaxLen(edit_ctr_id,MMIPB_MUL_SEARCH_MAX_CHAR,MMIPB_MUL_SEARCH_MAX_CHAR);
        GUIEDIT_SetDefaultStringById(edit_ctr_id, STXT_SEARCH);
        SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
        GUIEDIT_SetString(edit_ctr_id, applet_ptr->instance.search_str, 0);
    }

#ifdef MMIPB_MOST_USED_SUPPORT      
    if(MMIPB_MOSTUSED_CONTACTS != applet_ptr->instance.contacts_type)
#endif
    {
        //搜索按钮:"O"
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_PB_MENU_SEARCH_ICON,win_id);
        width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
        width_info.add_data = img_width;
        GUIFORM_SetChildWidth(search_edit_form_ctr_id, search_ctr_id, &width_info);

        GUIBUTTON_PermitBorder(search_ctr_id, FALSE);
        GUIBUTTON_SetRunSheen(search_ctr_id, FALSE);
        button_bg.bg_type = GUI_BG_IMG;
        button_bg.img_id = IMG_PB_MENU_SEARCH_ICON;
        GUIBUTTON_SetPressedBg(search_ctr_id, &button_bg);            
    }

#ifdef MMI_PDA_SUPPORT
        //only mutiselect window show select all button 
        if(MMIPB_LIST_CHECK != applet_ptr->instance.list_type)
        {
            GUILABEL_SetVisible(select_label_ctr_id, FALSE, FALSE);
            GUIBUTTON_SetVisible(select_button_ctr_id, FALSE, FALSE);
			width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
			width_info.add_data = 0;
			GUIFORM_SetChildWidth(subform_ctr_id, select_label_ctr_id, &width_info);
            GUIFORM_SetChildWidth(subform_ctr_id, select_button_ctr_id, &width_info);
        }
        else
        {
            GUILABEL_SetTextById(select_label_ctr_id, TXT_PB_SELECT_ALL, FALSE);
            GUILABEL_SetFont(select_label_ctr_id, SONG_FONT_14, MMI_DEFAULT_TEXT_COLOR);
            width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
            width_info.add_data = SELECT_LABLE_LEN;
            GUIFORM_SetChildWidth(subform_ctr_id, select_label_ctr_id, &width_info);

            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CHECK_UNSELECTED_ICON,win_id);
            width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
            width_info.add_data = img_width;
           
            GUIFORM_SetChildWidth(subform_ctr_id, select_button_ctr_id, &width_info);
        }
#endif        

    //tool bar
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined (GUIF_TOOLBAR) && defined(TOUCH_PANEL_SUPPORT)

#ifdef MMI_PDA_SUPPORT
    if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
#endif
    {
        PbCreateToolbar(win_id, toolbar_ctr_id, list_ctr_id);
    }

#endif

#ifdef MMI_PDA_SUPPORT
    //列表控件
    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
        GUILIST_SetItemHorMove(list_ctr_id, TRUE);
    }
#endif
    
       
    height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(form_ctr_id, list_ctr_id, &height_info);
    GUILIST_PermitBorder(list_ctr_id, FALSE);
    GUILIST_SetMargin(list_ctr_id,0, 0);
    GUILIST_SetUserBg(list_ctr_id, FALSE);       
            
#ifndef MMI_PDA_SUPPORT 
#ifndef TOUCH_PANEL_SUPPORT
	MMK_SetAtvCtrl(win_id, edit_ctr_id);
#endif
#endif    
}

/*****************************************************************************/
//  Description : 处理PB的操作的消息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	MMI_WIN_ID_T alert_id = NULL;
//	MMIPB_ERROR_E error = MMIPB_ERROR_SUCCESS;
	GUITAB_ITEM_T* tab_item_ptr = PNULL;
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    //MMIPB_INSTANCE_T   pb_instance_info = {0}; 


    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] HandleMainWinMsgPNULL == applet_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_3799_112_2_18_2_44_21_328,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    applet_handle = applet_ptr->caf_applet.app_handle;
    //pb_instance_info = applet_ptr->instance;
    alert_id = MMK_GetWinHandle(applet_handle, MMIPB_WAITING_ALERT_WIN_ID); 

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:       
#if defined MMI_PDA_SUPPORT  && defined MMI_ISTYLE_SUPPORT        
	if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
		{
	        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
	        MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);
		}
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
        GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_NULL,MIDDLE_BUTTON,TRUE);
#endif
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID));
		tab_item_ptr = GUITAB_GetCurItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID));
		if(tab_item_ptr != PNULL)
		{
			//this sub win is focus, need load list
			MMK_SendMsg( tab_item_ptr->child_win_handle, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)FALSE);
		}
        break; 
#ifdef MMI_PDA_SUPPORT       
    case MSG_CTL_TAB_SWITCH:

		{

			MMIPB_SUB_FUNCTION_TYPE_E app_type = GetCurAppType(applet_handle, GUITAB_GetCurSel(MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID)));

            MMIPB_UNMarkAll(applet_handle);
            //进入非contact list，或者已经解锁，或者没有加锁，这个时候应用类型需要切换
            switch(app_type)
            {
            case MMIPB_DIAL_APPLICATION:
                applet_ptr->instance.app_type = app_type;
                break;

            case MMIPB_CALLLOG_APPLICATION:
                if (MMIPB_GetCLUnProtect()
                    || !MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CL))
                {
                    applet_ptr->instance.app_type = app_type;
                }
                break;
            
            default:
                if (MMIPB_GetProtectStatus()
                    || !MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB))
                {
                    applet_ptr->instance.app_type = app_type;
                }
                break;
            } 
           
		}

        break;
#endif
	case MSG_GET_FOCUS:
		break;		

    case MMI_PB_RELOAD_SEARCH_LIST_MSG:
        {
            MMK_SendMsg(applet_ptr->instance.active_win_id , MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE); 
        }
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
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL void UpdateCopyWinParam(MMI_HANDLE_T applet_handle ,MMI_HANDLE_T    list_handle, BOOLEAN is_update)
{
    MMIPB_APPLET_T*  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    MMI_TEXT_ID_T    location_text_id = TXT_PB_SET_PHONE;
    MMI_IMAGE_ID_T imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
    MMI_STRING_T             str_info={0};
    if(PNULL == applet_ptr)
    {
      SCI_TRACE_LOW("[mmipb] UpdateCopyWinParam PNULL == applet_ptr");
      return;
    }   
    if( applet_ptr->instance.copy_from_index == MMI_DUAL_SYS_MAX)
    {
        location_text_id = TXT_PB_SET_PHONE;
    }
    else
    {
        location_text_id = GetTextIdByDualSys( applet_ptr->instance.copy_from_index);/*lint !e64*/
    }
    MMI_GetLabelTextByLang(location_text_id, &str_info);
    AppendTwoLineTextIconSmallTextListItem(list_handle,TXT_PB_COPY_FROM_LOCATION,imgae_icon_id,str_info,MMIPB_COPY_FROM_INDEX,is_update);
    if(applet_ptr->instance.copy_to_index == MMI_DUAL_SYS_MAX)
    {
        location_text_id = TXT_PB_SET_PHONE;
        
    }
    else
    {
        location_text_id = GetTextIdByDualSys( applet_ptr->instance.copy_to_index);/*lint !e64*/
    }
    MMI_GetLabelTextByLang(location_text_id, &str_info);
    AppendTwoLineTextIconSmallTextListItem(list_handle,TXT_PB_COPY_TO_LOCATION,imgae_icon_id,str_info,MMIPB_COPY_TO_INDEX,is_update);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleCopySelectWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
  uint16 cur_index = 0;
  MMI_RESULT_E                result = MMI_RESULT_TRUE;
  MMI_HANDLE_T    list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_COPY_SELECT_LIST_CTRL_ID);
  MMIPB_APPLET_T*  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
  if(PNULL == applet_ptr)
  {
      SCI_TRACE_LOW("[mmipb] HandleCopyLocationWinMsg PNULL == applet_ptr");
      return result;
  }
  switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 exist_sim = 0;
            
            GUILIST_SetMaxItem(list_handle,2, FALSE);
            applet_ptr->instance.copy_from_index =MMI_DUAL_SYS_MAX;
            applet_ptr->instance.copy_to_index = (MMIAPIPHONE_GetSimExistedNum(&exist_sim, 1) > 0 ? exist_sim : MMI_DUAL_SYS_MAX);
            UpdateCopyWinParam(applet_ptr->caf_applet.app_handle, list_handle, FALSE);
            MMK_SetAtvCtrl(win_id, list_handle); 
        }      
        break; 

    case MMI_PB_COPY_LOCATION_CHANGED:
        UpdateCopyWinParam(applet_ptr->caf_applet.app_handle, list_handle, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
             case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);
                break;
             case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;
             case MMIPB_COPY_SELECT_LIST_CTRL_ID:
                 cur_index =  GUILIST_GetCurItemIndex(list_handle);
                 if(MMIPB_COPY_FROM_INDEX == cur_index || MMIPB_COPY_TO_INDEX == cur_index)
                 {
                    MMIPB_OpenCopyLocationWin(applet_ptr->caf_applet.app_handle, cur_index);
                 }

                 break;
            default:
                break;
            }
        }
        break;
#endif

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
         cur_index =  GUILIST_GetCurItemIndex(list_handle);
         if(MMIPB_COPY_FROM_INDEX == cur_index || MMIPB_COPY_TO_INDEX == cur_index)
         {
            MMIPB_OpenCopyLocationWin(applet_ptr->caf_applet.app_handle, cur_index);
         }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    
        if(applet_ptr->instance.copy_from_index == applet_ptr->instance.copy_to_index)
        {
            MMIPUB_OpenAlertWinByTextIdEx(applet_ptr->caf_applet.app_handle,PNULL,TXT_PB_CANT_SELECT_SAME_LOCATION,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,0);
        }
        else
        {
             MMIPB_MENU_DATA_INFOR_T* menu_ptr = SCI_ALLOC_APP(sizeof(MMIPB_MENU_DATA_INFOR_T));
             if(PNULL != menu_ptr)
             {
                 SCI_MEMSET(menu_ptr, 0X00, sizeof(MMIPB_MENU_DATA_INFOR_T));
                 menu_ptr->operate_type = MMIPB_OPERATE_TYPE_COPY;
                 menu_ptr->group = PB_GROUP_ALL;
                 if(applet_ptr->instance.copy_from_index!=MMI_DUAL_SYS_MAX)
                 {
                     menu_ptr->group= PB_GROUP_SIM_1 + applet_ptr->instance.copy_from_index;
                 }
                 if(applet_ptr->instance.copy_to_index!=MMI_DUAL_SYS_MAX)
                 {
                     menu_ptr->mask_group = PB_GROUP_SIM_1 + applet_ptr->instance.copy_to_index;
                 }
                 
                 MMIPB_OpenOperationSelectWin(applet_ptr->caf_applet.app_handle,menu_ptr);
                 SCI_FREE(menu_ptr);
             }
              MMK_CloseWin(win_id);
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
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenCopySelectWin(MMI_HANDLE_T applet_handle)
{
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
    MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32*)MMIPB_COPY_WIN_TAB, PNULL);
    return TRUE;
}

/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendOneLineRadioTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id1,
        
                            uint16           pos,
                            uint32           user_data,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    //MMI_STRING_T     str_null = {0};

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;

   // item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id1;
    item_t.user_data = user_data;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByDualSys(MN_DUAL_SYS_E dual_sys)
{
    MMI_TEXT_ID_T text_id = TXT_PB_SET_SIM;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if(MN_DUAL_SYS_1 == dual_sys)
    {
         text_id =  TXT_SIM_SEL_SIM1;
    } 
    else if(MN_DUAL_SYS_2 == dual_sys)
    {
         text_id =  TXT_SIM_SEL_SIM2;
    } 
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    else if(MN_DUAL_SYS_3 == dual_sys)
    {
         text_id =  TXT_SIM_SEL_SIM3;
    }
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    else if(MN_DUAL_SYS_4 == dual_sys)
    {
       text_id =  TXT_SIM_SEL_SIM4;
    }
#endif
#endif
    return text_id;

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleCopyLocationWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
  uint32 cur_index = 0;
  MMI_RESULT_E                result = MMI_RESULT_TRUE;
  MMI_HANDLE_T    list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_COPY_LOCATION_LIST_CTRL_ID);
  //MMI_HANDLE_T    pre_list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_COPY_SELECT_LIST_CTRL_ID);
  uint16     location_index = (uint16)(MMK_GetWinAddDataPtr(win_id));/*lint !e64 !e507*/
  //MMI_TEXT_ID_T    location_text_id = TXT_PB_SET_PHONE;
  MMIPB_APPLET_T*  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
  uint16      cur_selection[MMI_DUAL_SYS_MAX+1]   =  {0};
  //MMI_STRING_T  str_info = {0};
#ifdef FDN_SUPPORT
    uint16 storage_id = 0;
#endif
  
  if(PNULL == applet_ptr)
  {
      SCI_TRACE_LOW("[mmipb] HandleCopyLocationWinMsg PNULL == applet_ptr");
      return result;
  }
  switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            int i= 0; 
            int index = 0;
            BOOLEAN is_ok = FALSE;    
            int focus_index = 0;

            if(location_index == MMIPB_COPY_FROM_INDEX)
            {
                focus_index = applet_ptr->instance.copy_from_index;
                GUIWIN_SetTitleTextId(win_id, TXT_PB_COPY_FROM_LOCATION, FALSE);
            }
            else
            {
                focus_index = applet_ptr->instance.copy_to_index;
                GUIWIN_SetTitleTextId(win_id, TXT_PB_COPY_TO_LOCATION, FALSE);
            }

            if (focus_index >= MMI_DUAL_SYS_MAX)
            {
                focus_index = 0;
                is_ok = TRUE;
            }

            GUILIST_SetMaxItem(list_handle,MMI_DUAL_SYS_MAX+1, FALSE);
            AppendOneLineRadioTextListItem(list_handle,TXT_PB_SET_PHONE,index, MMI_DUAL_SYS_MAX,FALSE);
            index++;
            for(i=MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
            {
               if(MMIAPIPHONE_GetSimExistedStatus(i))
               {
                    if (!is_ok)
                    {
                        if (focus_index == i)
                        {
                            focus_index = index;
                            is_ok = TRUE;
                        }
                    }

                    AppendOneLineRadioTextListItem(list_handle,GetTextIdByDualSys(i),index,i,FALSE);
                    index++;
               }
            }

            GUILIST_SetSelectedItem(list_handle, focus_index, TRUE);
            GUILIST_SetCurItemIndex(list_handle, focus_index);

            MMK_SetAtvCtrl(win_id, list_handle); 
        }      
        break; 

#ifdef MMI_PDA_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#else //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        GUILIST_GetSelectedItemIndex(list_handle, cur_selection, MMI_DUAL_SYS_MAX+1);
        GUILIST_GetItemData(list_handle, cur_selection[0],&cur_index);/*lint !e64*/

        
        if(location_index == MMIPB_COPY_FROM_INDEX)
        {
            applet_ptr->instance.copy_from_index = cur_index;
        }
        else if(location_index == MMIPB_COPY_TO_INDEX)
        {
            applet_ptr->instance.copy_to_index = cur_index;
 
        }

/***bug 99494***/
#ifdef FDN_SUPPORT
		/*2Gfdn不可访问的处理*/
		storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 +applet_ptr->instance.copy_to_index,PHONEBOOK_SIM_STORAGE);				
		if ((location_index == MMIPB_COPY_TO_INDEX) && MMIAPIFDN_Is2GFdnEnable(MMIPB_GetDualSysByStorageId(storage_id)))
		{
			//失败窗口
			MMIPUB_OpenAlertWinByTextId(PNULL,TXT_FDN_ACTIVATED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);     
		}
		else
#endif
		{
			MMK_SendMsg(MMK_GetWinHandle(applet_ptr->caf_applet.app_handle, MMIPB_COPY_SELECT_WIN_ID), MMI_PB_COPY_LOCATION_CHANGED, PNULL);
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
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenCopyLocationWin(MMI_HANDLE_T applet_handle, uint16 index)
{
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        return FALSE;
    }
#ifdef MMI_PDA_SUPPORT
        MMK_CreatePubListWinEx(applet_ptr->caf_applet.app_handle,(uint32 *)MMIPB_COPY_LOCATION_WIN_TAB, (ADD_DATA)index);
#else
        MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32 *)MMIPB_COPY_LOCATION_WIN_TAB, (ADD_DATA)index);
#endif
    return TRUE;
}
                                
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_OpenRingSelWin(MMI_WIN_ID_T win_id, MMI_HANDLE_T applet_handle,uint16 group_id)
{
    MMIPB_MENU_DATA_INFOR_T* menu_ptr = SCI_ALLOC_APP(sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL != menu_ptr)
    {
        SCI_MEMSET(menu_ptr, 0X00, sizeof(MMIPB_MENU_DATA_INFOR_T));

        menu_ptr->applet_handle = applet_handle;
        menu_ptr->parent_win = win_id;


        menu_ptr->group = (1<<group_id);
        menu_ptr->need_reset_storage = FALSE;
        menu_ptr->menu_type = MMIPB_RING_OPT_MENU;                    
        menu_ptr->operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
        menu_ptr->need_reset_storage = FALSE;
        MMIPB_EnterPbTabOptMenu(menu_ptr); 
        SCI_FREE(menu_ptr);
    }   
}
#endif



/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendTwoLineTextIconSmallTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id1,
                            MMI_IMAGE_ID_T   imgae_id,
                            MMI_STRING_T     str_info,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    //MMI_STRING_T     str_null = {0};

    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;

   // item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id1;
    item_data.item_content[2].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer = str_info;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = imgae_id;

   // item_data.item_content[3].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
   // item_data.item_content[3].item_data.text_buffer = str_null;

    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}


#if defined(MMIPB_GROUP_MANAGE_SUPPORT) && defined (MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL  void UpdateGroupRingInfo(MMI_HANDLE_T list_handle,MMIPB_GROUP_T  group_info, MMIPB_CUSTOM_RING_T* ring_ptr,BOOLEAN is_update)
{
    MMI_STRING_T    str = {0};
    wchar   item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    if(group_info.ring_id_flag& MMIPB_RING_ID_EXIST)
    {
    
        uint16   file_name[MMIPB_MAX_CUSTOM_RING_PATH_LEN] = {0};
        uint16  file_name_len = 0;
        MMIPB_CUSTOM_RING_T custom_ring={0};
        if(group_info.ring_id_flag & MMIPB_FIXED_RING_FLAG)
        {
            if( (group_info.ring_id_flag & MMIPB_FIXED_RING_ID_MASK)  <= MMISET_RING_FIXED_DEFAUT_NUM )
            {
                str.wstr_len = MMIPB_GetRingName((group_info.ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
                    file_name, MMIPB_MAX_CUSTOM_RING_PATH_LEN);

                str.wstr_len = MIN(GUIRICHTEXT_TEXT_MAX_LEN, str.wstr_len);
                MMIAPICOM_Wstrncpy(item_text, file_name, str.wstr_len);
                str.wstr_ptr = item_text;
    
            }
     
        }

        else if(  PNULL != ring_ptr)
        {
            if (MMIAPIFMM_IsFileExist(
                ring_ptr->file_name,
                (uint16)(ring_ptr->file_name_len)
                ))
            {
                MMIAPIFMM_SplitFullPath(
                    ring_ptr->file_name,
                    (uint16)(ring_ptr->file_name_len),
                    PNULL,
                    PNULL,
                    PNULL,
                    PNULL,
                    file_name,
                    &file_name_len
                    );
			    
                str.wstr_len = MIN(GUIRICHTEXT_TEXT_MAX_LEN, file_name_len);
                MMIAPICOM_Wstrncpy(item_text, file_name, str.wstr_len);
                str.wstr_ptr = item_text;

            }
        }
        else if(MMIPB_ReadGroupRingFile(group_info.group_id, &custom_ring))
        {
            if (MMIAPIFMM_IsFileExist(
                custom_ring.file_name,
                (uint16)(custom_ring.file_name_len)
                ))
            {
                MMIAPIFMM_SplitFullPath(
                    custom_ring.file_name,
                    (uint16)(custom_ring.file_name_len),
                    PNULL,
                    PNULL,
                    PNULL,
                    PNULL,
                    file_name,
                    &file_name_len
                    );
				
                str.wstr_len = MIN(GUIRICHTEXT_TEXT_MAX_LEN, file_name_len);
                MMIAPICOM_Wstrncpy(item_text, file_name,str.wstr_len);
                str.wstr_ptr = item_text;

            }
        }

    }
    else
    {
        MMIRES_GetText(TXT_PB_RING_DEFAULT,0,&str);
    }

    AppendTwoLineTextIconSmallTextListItem(list_handle, TXT_RING,IMAGE_COMMON_RIGHT_ARROW, str,PB_GROUP_INDEX_RING, is_update);
}
#endif

#if defined(MMIPB_GROUP_MANAGE_SUPPORT) && defined (MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL void UpdateAddGroupWinControls(MMI_WIN_ID_T win_id, uint32 group_id, BOOLEAN is_update)
{
    MMI_HANDLE_T    main_form_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_ADD_NEW_GROUP_FORM_ID);
    MMI_HANDLE_T    list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_ADD_NEW_GROUP_LISTBOX_CTRL_ID);
    MMI_STRING_T    str = {0};
    MMIPB_NAME_T    group_name = {0};
    //uint16          pos = 0;
    MMIPB_GROUP_T   group_info = {0};
    uint16              group_num = 0;
    char                group_num_str[10] ={0}; 
    wchar               group_num_wstr[10]={0};
   // MMI_STRING_T        str_num_t = {0};
    if(!is_update)
    {
        GUIFORM_PermitChildBg(main_form_handle, FALSE);
        GUIFORM_PermitChildBorder(main_form_handle, FALSE);
        GUILIST_SetMaxItem(list_handle, PB_INDEX_MAX, FALSE);
    }
    MMIPB_ReadGroupName(&group_name,group_id);
    str.wstr_ptr = group_name.wstr;
    str.wstr_len = group_name.wstr_len;
    if(0 == str.wstr_len)
    {
        MMIRES_GetText(TXT_PB_USER_GROUP_NAME_DEFAULT,0,&str);            
    }
    AppendTwoLineTextIconSmallTextListItem(list_handle, TXT_PB_USER_GROUP_NAME,IMAGE_COMMON_RIGHT_ARROW, str,(uint16)PB_GROUP_INDEX_NAME, is_update);
    SCI_MEMSET(&str, 0x00, sizeof(MMI_STRING_T));
    group_num =  MMIPB_GetContactNumOfGroup((uint8)group_id);
    sprintf(group_num_str,"%s%d%s", "(",group_num, ")");
    MMIAPICOM_StrToWstr((const uint8*)group_num_str, group_num_wstr);
    str.wstr_ptr = group_num_wstr;
    str.wstr_len = MMIAPICOM_Wstrlen(group_num_wstr);
    AppendTwoLineTextIconSmallTextListItem(list_handle, TXT_PB_MANAGE_GROUP_MEMBER,IMAGE_COMMON_RIGHT_ARROW, str,PB_GROUP_INDEX_MANAGE, is_update);
    if(!is_update)
    {
    
        MMIPB_GetGroupInfo(group_id, &group_info);
        group_info.group_id = group_id;
        UpdateGroupRingInfo(list_handle, group_info, PNULL,FALSE);

        if(group_info.is_valid)//modify group
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL,STXT_RETURN, FALSE);
        }
        else// add new group
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_NULL,STXT_CANCEL, FALSE);

        }

    }
    if(group_id<PB_GROUP_HOME)
    {
        GUILIST_SetItemGray(list_handle, PB_GROUP_INDEX_RING, TRUE);
    }
    if(group_id<PB_GROUP_MAX)
    {
        GUILIST_SetItemGray(list_handle, PB_GROUP_INDEX_NAME, TRUE);
    }
}
#endif

#if defined(MMIPB_GROUP_MANAGE_SUPPORT) && defined (MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 新增群组窗口消息处理函数
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleAddNewGroupWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T    list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_ADD_NEW_GROUP_LISTBOX_CTRL_ID);
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T    applet_handle = NULL;
    uint16          is_group = MMK_GetWinAddDataPtr(win_id);/*lint !e507 !e64*/
    uint8         is_new_group =is_group>>8;
    uint8           group_id = is_group&0xFF;//(uint32)MMK_GetWinAddDataPtr(win_id);
    uint32 cur_index = 0;
    if(PNULL == applet_ptr)
    {
        return recode;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    
    switch (msg_id)
    {
    
    case MSG_OPEN_WINDOW:
        
        UpdateAddGroupWinControls(win_id, group_id, FALSE);
        if(!is_new_group)
        {
            GUIWIN_SetTitleTextId(win_id, TXT_PB_SETTING_GROUP_MODIFY, FALSE);
        }
#ifndef MMI_PDA_SUPPORT
        MMK_SetActiveCtrl(list_handle, FALSE);
#else
#if defined MMI_ISTYLE_SUPPORT 
        if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            if(!is_new_group)
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_SAVE, FALSE);
                GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, 0);
            }
        }
#endif		
#endif
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            switch (src_id)
            {
             case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_OK, PNULL);
                break;
             case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
                break;
             case MMIPB_ADD_NEW_GROUP_LISTBOX_CTRL_ID:
                  cur_index =GUILIST_GetCurItemIndex(list_handle);
                 if(PB_GROUP_INDEX_NAME == cur_index)
                 {
                     MMK_CreatePubEditWin((uint32 *)MMIPB_MODIFY_GROUP_NAME_WIN_TAB, (ADD_DATA)group_id);
                 }
                 if(PB_GROUP_INDEX_MANAGE == cur_index)
                 {
                     MMIPB_MENU_DATA_INFOR_T* menu_ptr = SCI_ALLOC_APP(sizeof(MMIPB_MENU_DATA_INFOR_T));
                     if(PNULL != menu_ptr)
                     {
                         SCI_MEMSET(menu_ptr, 0X00, sizeof(MMIPB_MENU_DATA_INFOR_T));
                         menu_ptr->operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
                         menu_ptr->group = (1<<group_id);
                         menu_ptr->mask_group = MMIPB_GetAllSIMGroup();
                         menu_ptr->valid_contact_count = MMIPB_GetGroupContactCount(group_id);
                         MMIPB_OpenOperationSelectWin(applet_handle,menu_ptr);
                         SCI_FREE(menu_ptr);
                     }
                 }
                 else if(PB_GROUP_INDEX_RING == cur_index)
                 {
                    MMIPB_OpenRingSelWin(win_id, applet_handle,group_id);
          
                 }

                 break;
            default:
                break;
            }
        }
       break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527       
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_MIDSK:
        cur_index =GUILIST_GetCurItemIndex(list_handle);
        if(PB_GROUP_INDEX_NAME == cur_index)
        {
         MMK_CreatePubEditWin((uint32 *)MMIPB_MODIFY_GROUP_NAME_WIN_TAB, (ADD_DATA)group_id);
        }
        if(PB_GROUP_INDEX_MANAGE == cur_index)
        {
         MMIPB_MENU_DATA_INFOR_T* menu_ptr = SCI_ALLOC_APP(sizeof(MMIPB_MENU_DATA_INFOR_T));
         if(PNULL != menu_ptr)
         {
             SCI_MEMSET(menu_ptr, 0X00, sizeof(MMIPB_MENU_DATA_INFOR_T));
             menu_ptr->operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
             menu_ptr->group = (1<<group_id);
             menu_ptr->mask_group = MMIPB_GetAllSIMGroup();
             menu_ptr->valid_contact_count = MMIPB_GetGroupContactCount(group_id);
             MMIPB_OpenOperationSelectWin(applet_handle,menu_ptr);
             SCI_FREE(menu_ptr);
         }
        }
        else if(PB_GROUP_INDEX_RING == cur_index)
        {
        MMIPB_OpenRingSelWin(win_id, applet_handle,group_id);

        }
        break;
#else
    case MSG_CTL_MIDSK:
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_NOTIFY_TITLE_MENU_ICON:
        if(is_new_group )
        {
            MMIPB_GROUP_T      group_info = {0};
            MMIPB_GetGroupInfo(group_id, &group_info);
            if(0==group_info.group_name.wstr_len)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ENVSET_EMPTY_NAME,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
            }
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_GROUP_NAME_CHANGED:
    case MMI_PB_GROUP_NUM_CHANGED:
        UpdateAddGroupWinControls(win_id, group_id, TRUE);
       // MMK_UpdateScreen();
        break;
    case MMI_PB_SET_DEFAULT_RING:
        {
            MMIPB_GROUP_T      group_info = {0};
            group_info.ring_id_flag = *(uint8*)param; 
            MMIPB_SetGroupInfo(group_id, &group_info);
            UpdateGroupRingInfo(list_handle, group_info,PNULL,TRUE);
        }
    break;
    case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
        {
            MMIPB_GROUP_T      group_info = {0};
            uint16 *ring_id_ptr = PNULL;


            MMIPB_GetGroupInfo(group_id, &group_info);
        
            ring_id_ptr = (uint16 *)param;
            //SCI_TRACE_LOW:"[MMIPB]HandleEntryAddWinMsg MSG_SET_SELECT_FIX_CALL_RING_RETURN ring_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_4221_112_2_18_2_44_22_329,(uint8*)"d", *ring_id_ptr);
                         
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_FIXED_RING_FLAG_CLEAN) + MMIPB_FIXED_RING_FLAG);
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_FIXED_RING_ID_CLEAN) + *ring_id_ptr);
            MMIPB_SetGroupInfo(group_id, &group_info);
            UpdateGroupRingInfo(list_handle, group_info,PNULL,TRUE);
          //  MMK_UpdateScreen();
        }
        
        break;

    case MSG_MULTIM_SELECTED_RETURN: 
        {
            MMIPB_GROUP_T      group_info = {0};
            MULTIM_SELECT_RETURN_T* select_ptr = (MULTIM_SELECT_RETURN_T *)param;
            MMIPB_CUSTOM_RING_T* ring_ptr =SCI_ALLOC_APP(sizeof(MMIPB_CUSTOM_RING_T));
            MMIPB_GetGroupInfo(group_id, &group_info);
                                
            if(PNULL != ring_ptr)
            {
                SCI_MEMSET(ring_ptr, 0x00, sizeof(MMIPB_CUSTOM_RING_T));
                ring_ptr->is_valid =TRUE;
                group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
                group_info.ring_id_flag = group_info.ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN;
                group_info.ring_id_flag = group_info.ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN;   
                ring_ptr->file_name_len = select_ptr->file_name_len;
                MMIAPICOM_Wstrncpy(ring_ptr->file_name, select_ptr->file_name_ptr, select_ptr->file_name_len);
                ring_ptr->file_size = select_ptr->file_size;
                ring_ptr->ring_type = select_ptr->music_type;

                MMIPB_SetGroupInfo(group_id, &group_info);
                MMIPB_SaveGroupRingInfo(group_id, ring_ptr);
                UpdateGroupRingInfo(list_handle, group_info,ring_ptr,TRUE);
              //  MMK_UpdateScreen();
                SCI_FREE(ring_ptr);
            }
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(is_new_group)
        {
            MMIPB_DeleteUserGroup(applet_handle, group_id);
            MMK_SendMsg(MMK_GetPrevWinHandle(win_id), MMI_PB_GROUP_NUM_CHANGED, PNULL);
        }
        else
        {
            MMK_SendMsg(MMK_GetPrevWinHandle(win_id), MMI_PB_RELOAD_SEARCH_LIST_MSG, PNULL);
        }
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
#endif

#if defined(MMIPB_GROUP_MANAGE_SUPPORT)
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: MARYXIAO
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditGroupNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{

    MMI_HANDLE_T edit_handle = MMK_GetCtrlHandleByWin(win_id, MMIBT_EDIT_GROUP_NAME_EDITBOX_CTRL_ID);
    //MMI_HANDLE_T    list_handle = NULL;//MMK_GetCtrlHandleByWin(win_id, MMIPB_ADD_NEW_GROUP_LISTBOX_CTRL_ID);
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 group_id = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            MMIPB_NAME_T group_name = {0};
            MMI_STRING_T str_t = {0};
			GUIEDIT_SetTextMaxLen(edit_handle,MMIPB_GROUP_NAME_MAX_LEN, MMIPB_GROUP_NAME_MAX_LEN);

            MMIPB_ReadGroupName(&group_name,group_id);
            str_t.wstr_ptr = group_name.wstr;
            str_t.wstr_len = group_name.wstr_len;

            GUIEDIT_SetString(edit_handle, str_t.wstr_ptr, str_t.wstr_len);       
#ifdef MMI_PDA_SUPPORT
            GUIEDIT_SetSoftkey(edit_handle,	0, 1, TXT_NULL,	TXT_COMMON_OK, PNULL);
#endif
            if(0 == str_t.wstr_len) //新建
            {
                GUIWIN_SetTitleTextId(win_id, TXT_PB_NEW_GROUP, TRUE);
            }
            else
            {
                GUIWIN_SetTitleTextId(win_id, TXT_PB_SETTING_GROUP_MODIFY, TRUE);
            }
            MMK_SetAtvCtrl(win_id,edit_handle);
        }
        break;
        
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
        
            MMI_STRING_T   str = {0};
            MMI_HANDLE_T    applet_handle = NULL;
            MMIPB_GROUP_T      group_info = {0};
			int 			i = 0;
			MMIPB_NAME_T    name_t = {0};
			BOOLEAN			is_existed = FALSE;

            MMIPB_GetGroupInfo(group_id, &group_info);
            SCI_MEMSET(group_info.group_name.wstr, 0x00, sizeof(wchar)*MMIPB_NAME_MAX_LEN);
            GUIEDIT_GetString(edit_handle,&str);
			
			//first check system group
			for(i = PB_GROUP_ALL + 1; i < MMIPB_SYSTEM_GROUP_NUM; i++)
			{
				if(MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name_t, (uint8)i))
				{
					if((str.wstr_len == name_t.wstr_len) && (!MMIAPICOM_Wstrcmp(str.wstr_ptr, name_t.wstr)))
					{
						SCI_TRACE_LOW("[mmipb]this group is existed in system group, str.wstr_len = %d, str.wstr_ptr = %s", str.wstr_len, str.wstr_ptr);						
						is_existed = TRUE;
						MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PB_GROUP_NAME_NOT_LEGAL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);     						
					    break;
					}
					SCI_MEMSET(&name_t, 0,sizeof(MMIPB_NAME_T));
				}
			}
			if(!is_existed)
			{
				//then check user group
				for(i = MMIPB_SYSTEM_GROUP_NUM; i < MMIPB_SYSTEM_GROUP_NUM + MMIPB_USER_GROUP_NUM; i++)
				{
					if(MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name_t, (uint8)i))
					{
						if((str.wstr_len == name_t.wstr_len) && (!MMIAPICOM_Wstrcmp(str.wstr_ptr, name_t.wstr)))
						{
			    			SCI_TRACE_LOW("[mmipb]this group is existed in user group, str.wstr_len = %d, str.wstr_ptr = %s", str.wstr_len, str.wstr_ptr);
							is_existed = TRUE;
							MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PB_GROUP_NAME_EXIST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);     						
							break;
						}
						SCI_MEMSET(&name_t, 0,sizeof(MMIPB_NAME_T));
					}
				}
			}
			if(!is_existed)
            {
	            SCI_MEMCPY(group_info.group_name.wstr, str.wstr_ptr, sizeof(wchar)*str.wstr_len);
	            group_info.group_name.wstr_len = str.wstr_len;
	            group_info.is_valid = TRUE;
	            if(group_info.group_name.wstr_len>0)
	            {

	                MMIPB_SetGroupInfo(group_id, &group_info);
                
	            }
	            else
	            {
	                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ENVSET_EMPTY_NAME,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	                break;
	            }

            
	            MMIPB_GetCurAppletHandle(&applet_handle);
	            MMK_SendMsg(MMK_GetWinHandle(applet_handle,MMIPB_ADD_NEW_GROUP_WIN_ID), MSG_GROUP_NAME_CHANGED, &group_id);
	            MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID), MSG_GROUP_NAME_CHANGED, &group_id);


	            MMK_CloseWin(win_id);
            }
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
#endif

/*****************************************************************************/
//  Description :append one line icon text list item
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AppendGroupListItem(MMI_WIN_ID_T    win_id,
                          MMI_CTRL_ID_T       ctrl_id,
						  uint32              item_id,
                          const MMI_STRING_T*  str_ptr,
                          const MMI_STRING_T*  str_ptr2,
                          MMI_TEXT_ID_T       default_text_id
                          )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64 */
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64 */
    uint16               content_index = 0;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return;
    }



    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = item_id;

#ifdef MMI_PDA_SUPPORT    
    item_data.softkey_id[LEFT_BUTTON] = TXT_COMMON_OK;
#else
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
        item_data.softkey_id[LEFT_BUTTON] = STXT_OPTION;   
    }
    else
    {
        item_data.softkey_id[LEFT_BUTTON] = TXT_COMMON_OK;  
    }
     
#else
    item_data.softkey_id[LEFT_BUTTON] = TXT_NULL;    
#endif
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    item_data.softkey_id[MIDDLE_BUTTON] = IMAGE_NULL;
#else
    item_data.softkey_id[MIDDLE_BUTTON] = TXT_NULL;
#endif
    item_data.softkey_id[RIGHT_BUTTON] = STXT_RETURN;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = GetGroupIcon(item_id);
    content_index++;

    if ( PNULL != str_ptr && 0 < str_ptr->wstr_len && PNULL != str_ptr->wstr_ptr )
    {
        item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[content_index].item_data.text_buffer = *str_ptr;
    }
    else
    {
        item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[content_index].item_data.text_id = default_text_id;
    }
    content_index++;

    if ( PNULL != str_ptr2 && 0 < str_ptr2->wstr_len && PNULL != str_ptr2->wstr_ptr )
    {
        item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[content_index].item_data.text_buffer = *str_ptr2;
    }
    else
    {
        item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[content_index].item_data.text_id = default_text_id;
    }
    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Description :append one line icon text list item
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(MMI_CTRL_ID_T       ctrl_id,
                          uint32              item_state,
						  uint32              item_id,
                          GUIITEM_STYLE_E     item_style,
                          MMI_TEXT_ID_T       left_softkey_id,
                          MMI_TEXT_ID_T       middle_softkey_id,
                          MMI_TEXT_ID_T       right_softkey_id,
                          MMI_IMAGE_ID_T      icon_id,
                          const MMI_STRING_T*  str_ptr,
                          MMI_TEXT_ID_T       default_text_id
                          )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64 */
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64 */
    uint16               content_index = 0;
    item_t.item_style    = item_style;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.item_state    = item_state;
    item_t.user_data     = item_id;
    item_data.softkey_id[LEFT_BUTTON] = left_softkey_id;
    item_data.softkey_id[MIDDLE_BUTTON] = middle_softkey_id;
    item_data.softkey_id[RIGHT_BUTTON] = right_softkey_id;
    
    if(GUIITEM_STYLE_ONE_LINE_ICON_TEXT == item_style)
    {
        //need icon
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = icon_id;
        content_index++;
    }
    if ( PNULL != str_ptr && 0 < str_ptr->wstr_len && PNULL != str_ptr->wstr_ptr )
    {
        item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[content_index].item_data.text_buffer = *str_ptr;
    }
    else
    {
        item_data.item_content[content_index].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[content_index].item_data.text_id = default_text_id;
    }
    GUILIST_AppendItem( ctrl_id, &item_t );
}


#if defined(MMIPB_GROUP_MANAGE_SUPPORT)
/*****************************************************************************/
//  Description : 删除用户自定义群组
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_DeleteUserGroup(MMI_HANDLE_T applet_handle, uint32 group_id)
{
    MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T    contact_valid_num_mail_info ={0};
    MMIPB_CONTACT_BASE_INFO_T  contact_node = {0};
    MMIPB_HANDLE_T        handle = 0;
    uint16          contact_index = 0;
    uint16          storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE ,MMIPB_NORMAL_PHONE_CONTACT);
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    //uint16  i  = MMIPB_GROUP_SNS_SINA;
    //uint32 exclude_group = 0;
    //MMI_HANDLE_T list_handle =MMIPB_GROUP_ENTRY_LIST_CTRL_ID;
    MMI_STRING_T   search_str = {0};
   // uint32 user_data    = 0;
    uint32  total_count = 0;
    MMIPB_NAME_T  name ={0};
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    
    if(PNULL == applet_ptr)  //+ for coverity
    {
        return;
    }
   
   
     MMIPB_SaveGroup(&name, group_id, FALSE);
    
    MMIPB_DeleteGroupRingInfo(group_id);
    total_count = MMIPB_MultiSearch(&search_str, (1<<group_id), 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));


    if(MMIPB_ERROR_SUCCESS != MMIPB_FindFirstContact((1<<group_id), storage_id, &contact_index, &contact_valid_num_mail_info,&contact_node, &handle))
    {
        //SCI_TRACE_LOW:"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_4511_112_2_18_2_44_23_330,(uint8*)"");
        return;
    }   
    do
    {
        MMIPB_SEL_DATA sel_data = {0};
        sel_data.entry_id = contact_node.contact_id;
        sel_data.storage_id = contact_node.storage_id;
        sel_data.is_in_group = (1<<4);
        sel_data.is_manage_group = TRUE;

        MMIPB_CreateSelNode(applet_ptr->instance.handle, &sel_data);
            
    }while(MMIPB_FindNextContact(handle,(1<<group_id), storage_id, &contact_index, &contact_valid_num_mail_info,&contact_node) == MMIPB_ERROR_SUCCESS);
    MMIPB_FindClose(handle);

    MMIPB_AbortPB(FALSE);
    ret = MMIPB_ManageGroupContactAsynForPB(applet_ptr->instance.handle, group_id);
    if((MMIPB_ERROR_PENDING == ret) || (MMIPB_ERROR_SUCCESS == ret))
    {
        MMIPB_OpenWaitWin(applet_ptr->caf_applet.app_handle,TXT_COMMON_WAITING);
    }
   
}
#endif

#if defined(MMIPB_GROUP_MANAGE_SUPPORT)
/*****************************************************************************/
//  Description : 新增用户自定义群组
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_AddUserGroup(MMI_HANDLE_T applet_handle)
{
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    uint8 group_id = 0;
    if(PNULL == applet_ptr)
    {
        return;
    }
    group_id = MMIPB_GetFreeUserGroupId();
    if(group_id<(MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM))
    {
#ifdef MMI_PDA_SUPPORT	
        applet_ptr->instance.cur_group_id =  group_id; 
        MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32*)MMIPB_ADD_NEW_GROUP_WIN_TAB, (ADD_DATA)((1<<8)+group_id));
#else
     MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32 *)MMIPB_MODIFY_GROUP_NAME_WIN_TAB, (ADD_DATA)group_id);
#endif
    }
    else
    {
       MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PB_USER_GROUP_REACH_MAX,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}
#endif

#if defined(MMIPB_GROUP_MANAGE_SUPPORT) && defined (MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 编辑群组
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EditUserGroup(MMI_HANDLE_T applet_handle, uint32 group_id)
{
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        return;
    }	
    applet_ptr->instance.cur_group_id =  group_id; 
    MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32*)MMIPB_ADD_NEW_GROUP_WIN_TAB, (ADD_DATA)group_id);
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : 编辑群组名称
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_EditUserGroupName(MMI_HANDLE_T applet_handle, uint32 group_id)
{
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        return;
    }
     MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32 *)MMIPB_MODIFY_GROUP_NAME_WIN_TAB, (ADD_DATA)group_id);
}
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description :SET WIN TITLE RIGHT ICON
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void SetPbTitleIcon(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T image_up, MMI_IMAGE_ID_T image_down, BOOLEAN is_visiable)
{
	if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())

	{
	    GUIWIN_SetTitleNeedButton(win_id, TRUE);
	    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);
	    if(is_visiable)
	    {
	        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
	    }
	    else
	    {
	        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
	    }
	    GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, image_up, image_down, FALSE);
	    GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, 0);
	}
}
#endif
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitGroupList(MMI_WIN_ID_T win_id,    MMI_HANDLE_T list_handle, uint8 group_id)
{

    //uint16 list_index = 0;
    MMI_STRING_T str_t = {0};
    MMI_STRING_T str_num_t = {0};
    int i =0;
    int                 index = 0;
    MMIPB_NAME_T        name_t = {0};
    uint16              group_num = 0;
    char                group_num_str[10] ={0}; 
    wchar               group_num_wstr[10]={0};
    
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return;
    }

	GUILIST_SetMaxItem(list_handle, MMIPB_MAX_GROUP_NUM, FALSE);
    for (i = PB_GROUP_ALL+1; i < (MMIPB_USER_GROUP_NUM+MMIPB_SYSTEM_GROUP_NUM); i++)
    {
         if(MMIPB_IsInSIMGroup(i))
        {
            MN_DUAL_SYS_E dual_sys = i - PB_GROUP_SIM_1 ;
            if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
            {
                continue;
            }
        }
#ifdef MMIPB_MOST_USED_SUPPORT         
        if(MMIPB_IsFixedGroup(i))
        {
            continue;
        }
#endif
#ifdef BT_PBAP_SUPPORT
		if(PB_GROUP_BT == i)
		{
			continue;
		}
#endif
        if(MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name_t, (uint8)i))
        {
            str_t.wstr_len = name_t.wstr_len;
            str_t.wstr_ptr = name_t.wstr;

            group_num =  MMIPB_GetContactNumOfGroup((uint8)i);
            sprintf(group_num_str,"%s%d%s", "(",group_num, ")");
            MMIAPICOM_StrToWstr((const uint8 *)group_num_str, group_num_wstr);
            str_num_t.wstr_ptr = group_num_wstr;
            str_num_t.wstr_len = MMIAPICOM_Wstrlen(group_num_wstr);
            AppendGroupListItem(win_id,list_handle, i,&str_t, &str_num_t,TXT_NULL);
            
            if(i==group_id)
            {
                GUILIST_SetCurItemIndex(list_handle,index);
            }
            index++;
        }
             
    }
#ifdef MMI_PDA_SUPPORT    
    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
        #ifdef SNS_SUPPORT        
        for(i=MMIPB_GROUP_SNS_SINA; i<MMIPB_GROUP_SNS_MAX; i++)
        {
            if(MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(&name_t, (uint8)i))
            {

                str_t.wstr_len = name_t.wstr_len;
                str_t.wstr_ptr = name_t.wstr;
                group_num =  MMIPB_GetContactNumOfGroup((uint8)i);
                sprintf(group_num_str,"%s%d%s", "(",group_num, ")");
                MMIAPICOM_StrToWstr((const uint8 *)group_num_str, group_num_wstr);
                str_num_t.wstr_ptr = group_num_wstr;
                str_num_t.wstr_len = MMIAPICOM_Wstrlen(group_num_wstr);                
                AppendGroupListItem(win_id, list_handle,i, &str_t, &str_num_t,TXT_NULL);
                index++;
            }        
        }
        #endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
        SetPbTitleIcon(win_id, IMAGE_PHONEBOOK_TOOLBAR_ADD, IMAGE_PHONEBOOK_TOOLBAR_ADD, TRUE);
#endif
#endif
    }
#else
    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {        
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION,  TXT_NULL,  STXT_CANCEL, TRUE);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,  TXT_NULL,  STXT_CANCEL, TRUE);
    }
#endif
}

/*****************************************************************************/
//  Description : 群组窗口处理函数
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGroupListWinMsg
(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_GROUP_ENTRY_LIST_CTRL_ID); 
#ifdef MMI_PDA_SUPPORT 
    MMIPB_MENU_DATA_INFOR_T menu_infor = {0};
#else
#ifdef MMIPB_GROUP_MANAGE_SUPPORT 
    MMIPB_MENU_DATA_INFOR_T menu_infor = {0};
#endif
#endif
	uint32              user_data = 0;
    uint16              list_index = 0;
 	
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:     
        InitGroupList(win_id, ctrl_id, 0);  
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT        
        if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle()
            && MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
            GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_ISTYLE_TITLE_ADD, IMAGE_ISTYLE_TITLE_ADD_DOWN, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, 0);
        }
#endif        
        break;
      
    case MMI_PB_GROUP_NUM_CHANGED:
        InitGroupList(win_id, ctrl_id, 0);  
        break;
    
	case MSG_GROUP_NAME_CHANGED:
        {
            uint8* group_id_ptr = (uint8*)param;
        InitGroupList(win_id, ctrl_id, *group_id_ptr);
        }
        break;

#ifdef MMI_PDA_SUPPORT
     case MSG_CTL_LIST_LONGOK:      
        if (MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
        {
            break;
        }
         list_index = (uint32)GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, list_index, &user_data);
        
#ifdef SNS_SUPPORT
        if(MMIPB_IsSNSContact(MMIPB_GetStorageIDByGroupID(user_data)))
        {
            break;
        }
#endif  
        {
            SCI_MEMSET(&menu_infor,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
            menu_infor.applet_handle = applet_ptr->caf_applet.app_handle;
            
            menu_infor.menu_type = MMIPB_GROUPLIST_LONGTP_MENU;
            menu_infor.operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
            menu_infor.group = (1<<user_data);
            menu_infor.need_reset_storage = FALSE;
            menu_infor.valid_contact_count = MMIPB_GetGroupContactCount(user_data);
            MMIPB_EnterPbTabOptMenu(&menu_infor);
        }

        break;
#endif

	case MSG_GET_FOCUS:
         //  InitGroupList(win_id, ctrl_id, );
           list_index =  GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_SetCurItemIndex(ctrl_id,list_index);
		    MMK_SetAtvCtrl(win_id, ctrl_id);
		break;
        
#ifdef MMIPB_GROUP_MANAGE_SUPPORT         
#ifndef MMI_PDA_SUPPORT
    case MSG_APP_OK:    
    case MSG_CTL_OK: 
            SCI_MEMSET(&menu_infor,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
            list_index = (uint32)GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, list_index, &user_data);
            menu_infor.group = (1<<user_data);
            applet_ptr->instance.cur_group_id = user_data;
            if(MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
            {
                OpenGroupEntryList(applet_ptr->caf_applet.app_handle, user_data);
            }
            else
            {
                menu_infor.valid_contact_count = MMIPB_GetGroupContactCount(user_data);
                menu_infor.applet_handle = applet_ptr->caf_applet.app_handle;
                menu_infor.parent_win = win_id;
                menu_infor.menu_type = MMIPB_GROUPLIST_OPT_MENU;
                menu_infor.operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
                menu_infor.need_reset_storage = FALSE;
                MMIPB_EnterPbTabOptMenu(&menu_infor);
            }
		break;
#endif
#endif
 
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#endif
         if(MMIPB_GROUP_ENTRY_LIST_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
         {
            //enter this group
            list_index = (uint32)GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, list_index, &user_data);
            OpenGroupEntryList(applet_ptr->caf_applet.app_handle, user_data);
         }
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
         else
         {
             MMIPB_AddUserGroup(applet_ptr->caf_applet.app_handle);
         }
#endif
        break;
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        MMIPB_AddUserGroup(applet_ptr->caf_applet.app_handle);
        break;

#ifndef MMI_PDA_SUPPORT
    case MMI_PB_SET_DEFAULT_RING:
        {
            MMIPB_GROUP_T      group_info = {0};
			MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);  //Bug 337793
            group_info.ring_id_flag = *(uint8*)param; 
            MMIPB_SetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
        }
        break;
    case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
        {
            MMIPB_GROUP_T      group_info = {0};
            uint16 *ring_id_ptr = PNULL;


            MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
            ring_id_ptr = (uint16 *)param;
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_FIXED_RING_FLAG_CLEAN) + MMIPB_FIXED_RING_FLAG);
            group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_FIXED_RING_ID_CLEAN) + *ring_id_ptr);
            MMIPB_SetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);

        }
        
        break;

    case MSG_MULTIM_SELECTED_RETURN: 
        {
            MMIPB_GROUP_T      group_info = {0};
            MULTIM_SELECT_RETURN_T* select_ptr = (MULTIM_SELECT_RETURN_T *)param;
            MMIPB_CUSTOM_RING_T* ring_ptr =SCI_ALLOC_APP(sizeof(MMIPB_CUSTOM_RING_T));
            MMIPB_GetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
                                
            if(PNULL != ring_ptr)
            {
                SCI_MEMSET(ring_ptr, 0x00, sizeof(MMIPB_CUSTOM_RING_T));
                ring_ptr->is_valid =TRUE;
                group_info.ring_id_flag = ((group_info.ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
                group_info.ring_id_flag = group_info.ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN;
                group_info.ring_id_flag = group_info.ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN;   
                ring_ptr->file_name_len = select_ptr->file_name_len;
                MMIAPICOM_Wstrncpy(ring_ptr->file_name, select_ptr->file_name_ptr, select_ptr->file_name_len);
                ring_ptr->file_size = select_ptr->file_size;
                ring_ptr->ring_type = select_ptr->music_type;

                MMIPB_SetGroupInfo(applet_ptr->instance.cur_group_id, &group_info);
                MMIPB_SaveGroupRingInfo(applet_ptr->instance.cur_group_id, ring_ptr);
                SCI_FREE(ring_ptr);
            }
        }
        break;
#endif
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef MMI_PDA_SUPPORT
        if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMK_CloseApplet(applet_ptr->caf_applet.app_handle);
        }
        
#else
        MMK_CloseAllWinByApplet(applet_ptr->caf_applet.app_handle);
#endif
        break;        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}



/*****************************************************************************/
//  Description : MemDetailCtrlCallbackFun1
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void MemDetailCtrlCallbackFun( GUIANIM_OWNER_DRAW_T    *draw_ptr)
{
	uint16              width,height;
    GUI_RECT_T          display_rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info =  {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	uint16 used_num = 0;
    uint16 max_num = 1;
	GUI_RECT_T          image_rect={0};
    MN_DUAL_SYS_E dual_sys  = MN_DUAL_SYS_1;
    PB_GROUP_LIST_E     group = PB_GROUP_SIM_1;
    dual_sys = (MN_DUAL_SYS_E)draw_ptr->param;
    if(dual_sys == MN_DUAL_SYS_1)
    {
        group = PB_GROUP_SIM_1;
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else if(dual_sys == MN_DUAL_SYS_2)
    {
        group = PB_GROUP_SIM_2;
    }
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    else if(dual_sys == MN_DUAL_SYS_3)
    {
        group = PB_GROUP_SIM_3;
    }           
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    else if(dual_sys == MN_DUAL_SYS_4)
    {
        group = PB_GROUP_SIM_4;
    } 
#endif
#endif
	
    if(dual_sys == MMI_DUAL_SYS_MAX)
    {
        used_num = MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
        max_num = MMIPB_GetContactStorageSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
    }
    else if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
		used_num = MMIPB_GetContactNumOfGroup(group);
        max_num = MMIPB_GetMaxContactNumOfSim(dual_sys);
    }
	if(used_num > 0)
	{
        if(max_num==0)
        {
            used_num=0;
            max_num=1;
        }
		
        //display battery image bar
        GUIRES_GetImgWidthHeight(&width,&height, IMAGE_BATTERY_CAPACIT_FG,draw_ptr->win_handle);
        display_rect.left = draw_ptr->display_x;
        display_rect.top =  draw_ptr->display_rect.top;
        display_rect.right = display_rect.left + used_num*width/max_num;
        display_rect.bottom = draw_ptr->display_rect.bottom;
		
        image_rect.left=0;
        image_rect.right=image_rect.left+ used_num*width/max_num;
        image_rect.top=draw_ptr->display_rect.top-draw_ptr->display_y;
        image_rect.bottom=image_rect.top+draw_ptr->display_rect.bottom-draw_ptr->display_rect.top;
		
		GUIRES_DisplayImg(PNULL,&display_rect,&image_rect,draw_ptr->win_handle,IMAGE_BATTERY_CAPACIT_FG,&lcd_dev_info);
	}

}

/*****************************************************************************/
//  Description : to open the memory detatil window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetMemDetailCtrlParam(MMI_WIN_ID_T  win_id)
{
    int  i = 0;
    MMI_STRING_T       title_str = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_SIM_NAME_T  sim_str = {0};
#endif
    int 				count=0;
    uint16              used_num = 0;
    uint16              max_num = 0;
    uint8               status[32] = {0};
    wchar               wstatus[32] = {0};
    wchar               labal_text[64] = {0};
    uint16 labal_text_len=0;
    uint16 text_len=0;
    MMI_CTRL_ID_T      labal_ctrl_id_arr[] ={MMIPB_MEMDETAIL_LABEL1_CTRL_ID,  
                                       MMIPB_MEMDETAIL_LABEL2_CTRL_ID, 
                                       MMIPB_MEMDETAIL_LABEL3_CTRL_ID,
                                       MMIPB_MEMDETAIL_LABEL4_CTRL_ID, 
                                       MMIPB_MEMDETAIL_LABEL5_CTRL_ID 
                                       };
    MMI_CTRL_ID_T      labal_ctrl_id_arr2[] ={MMIPB_MEMDETAIL_LABEL6_CTRL_ID,  
                                       MMIPB_MEMDETAIL_LABEL7_CTRL_ID, 
                                       MMIPB_MEMDETAIL_LABEL8_CTRL_ID ,
                                        MMIPB_MEMDETAIL_LABEL9_CTRL_ID, 
                                       MMIPB_MEMDETAIL_LABEL10_CTRL_ID     
                                       };
    MMI_CTRL_ID_T      anim_ctrl_id_arr[] ={MMIPB_MEMDETAIL_ANIM1_CTRL_ID, 
                                       MMIPB_MEMDETAIL_ANIM2_CTRL_ID, 
                                       MMIPB_MEMDETAIL_ANIM3_CTRL_ID,      
                                       MMIPB_MEMDETAIL_ANIM4_CTRL_ID,   
                                       MMIPB_MEMDETAIL_ANIM5_CTRL_ID      
                                       };
	
    MMI_IMAGE_ID_T      icon_id_arr[] = {
    	                    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    	                                   IMAGE_SIM1, 
    	                                   IMAGE_SIM2, 
    	                    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    	                                   IMAGE_SIM3, 
    	                    #endif
    	                    #if defined(MMI_MULTI_SIM_SYS_QUAD)
    	                                   IMAGE_SIM4, 
    	                    #endif
    	                    #else
    	                                   IMAGE_PHONEBOOK_SIM,
    	                    #endif
    	                                   
    	                                   IMAGE_STORAGE_PHONE_ICON};

    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    GUIANIM_CALL_BACK_T     call_back = {0};

    wchar  colon[2] = {0};
    colon[0]=':';
	
    for(i = 0; i <= MMI_DUAL_SYS_MAX; i++)
    {
        SCI_MEMSET(&title_str, 0x00, sizeof (MMI_STRING_T));
        SCI_MEMSET(&status, 0x00, sizeof (uint8)*32);
        SCI_MEMSET(&wstatus, 0x00, sizeof (wchar)*32);
        SCI_MEMSET(&labal_text, 0x00, sizeof (wchar)*64);
        labal_text_len=0;
        text_len=0;
    	
        if (MMI_DUAL_SYS_MAX == i || MMIAPIPHONE_GetSimExistedStatus(i))/*lint !e64*/
        {

            //show  memory information
            if(MMI_DUAL_SYS_MAX == i)
            {
                //phone memory
                MMI_GetLabelTextByLang(TXT_PHONE, &title_str);
            }
            else
            {
                //sim memory
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                sim_str = MMIAPISET_GetSimName((MN_DUAL_SYS_E)i);
                title_str.wstr_len = sim_str.wstr_len;
                title_str.wstr_ptr = sim_str.wstr_arr;
#else //shipeng.lv add for cr149931 at sprd
                MMI_GetLabelTextByLang(TXT_COMM_SIM,&title_str);
#endif       
            }

            //set icon
            if(MMI_DUAL_SYS_MAX == i)
            {
            	GUILABEL_SetIcon(MMK_GetCtrlHandleByWin(win_id,labal_ctrl_id_arr[count]),IMAGE_STORAGE_PHONE_ICON);
            }
            else
            {
            	GUILABEL_SetIcon(MMK_GetCtrlHandleByWin(win_id,labal_ctrl_id_arr[count]),icon_id_arr[i]);
            }

            MMI_WSTRNCPY(labal_text,64,title_str.wstr_ptr,title_str.wstr_len,title_str.wstr_len);
            labal_text_len=title_str.wstr_len;

            MMIAPICOM_Wstrcat(labal_text,colon);
            labal_text_len+=1;
            title_str.wstr_ptr = labal_text;
            title_str.wstr_len=labal_text_len;


            GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id,labal_ctrl_id_arr[count]),&title_str,FALSE);
            
            if(MMI_DUAL_SYS_MAX == i)
            {
                used_num = MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
                max_num = MMIPB_GetContactStorageSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
            }
            else
            {
                used_num = MMIPB_GetContactNumOfGroup(PB_GROUP_SIM_1 + i);
                max_num = MMIPB_GetMaxContactNumOfSim(i);
            }
            sprintf( (char*)status, "%d/%d", used_num, max_num);
            text_len= MIN(strlen((char*)status), 32);/*lint !e666*/
            MMI_STRNTOWSTR(wstatus, 32, status, 32, text_len);/*lint !e666*/
			
			title_str.wstr_ptr = wstatus;
			title_str.wstr_len=text_len;
            GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id,labal_ctrl_id_arr2[count]),&title_str,FALSE);

            data_info.img_id = IMAGE_BATTERY_CAPACIT_BG;
            control_info.is_ctrl_id = TRUE;

            control_info.ctrl_id = MMK_GetCtrlHandleByWin(win_id,anim_ctrl_id_arr[count]);
            control_info.ctrl_ptr = PNULL;

            display_info.is_syn_decode = TRUE;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
            call_back.OwnerDrawFunc = MemDetailCtrlCallbackFun;
            call_back.param = i;
                // NV Storage!!!
            GUIANIM_SetOwnerDrawEx(MMK_GetCtrlHandleByWin(win_id,anim_ctrl_id_arr[count]),&call_back);


            count++;
        }
    }

    for(i=MMI_DUAL_SYS_MAX;i-count>=0;i--)
    {
        GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMDETAIL_FORM_CTRL_ID), 
        				MMK_GetCtrlHandleByWin(win_id,labal_ctrl_id_arr[i]),
        				GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMDETAIL_FORM_CTRL_ID), 
        				MMK_GetCtrlHandleByWin(win_id,labal_ctrl_id_arr2[i]),
        				GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id,MMIPB_MEMDETAIL_FORM_CTRL_ID), 
        				MMK_GetCtrlHandleByWin(win_id,anim_ctrl_id_arr[i]),
        				GUIFORM_CHILD_DISP_HIDE);
    }

}


/*****************************************************************************/
//  Description : to handle the memory detail window
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMemDetailWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T applet_handle = NULL;
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"mmipb HandleMemDetailWinMsg PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_5231_112_2_18_2_44_24_331,(uint8*)"");
        return recode;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        SetMemDetailCtrlParam(win_id);
        /****bug 4222 started*******/
        //MMK_SetAtvCtrl(win_id,MMK_GetCtrlHandleByWin(win_id, MMIPB_MEMDETAIL_FORM_CTRL_ID) );
        MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIPB_MEMDETAIL_LABEL1_CTRL_ID));
        GUIFORM_PermitChildBorder(MMK_GetCtrlHandleByWin(win_id, MMIPB_MEMDETAIL_FORM_CTRL_ID), FALSE);
        /****bug 4222 end **********/
        break;
    case MSG_GET_FOCUS:
        SetMemDetailCtrlParam(win_id);
        break;
    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return ( recode );
}


/*****************************************************************************/
//  Description : to open the memory detatil window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenMemDetail(MMI_HANDLE_T  applet_handle )
{
    //create memory status window
    MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_MEMDETAIL_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void EditContact(MMIPB_APPLET_T* applet_ptr, MMIPB_CONTACT_T* contact_ptr)
{
    MMIPB_MENU_DATA_INFOR_T add_data = {0};
    add_data.applet_handle = applet_ptr->caf_applet.app_handle;
    
    add_data.operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;

    add_data.contact_ptr = contact_ptr;
    add_data.valid_contact_count = 1;
    MMIPB_AddOrEditContactOfPb(&add_data);
}
/*****************************************************************************/
//  Description : 通过按拨号键值获得对应的sim卡dual_sys
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetDualSysFromGreenKey(MMI_MESSAGE_ID_E msg_id)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    switch(msg_id)
    {
   
    case MSG_KEYUP_GREEN:
        dual_sys = MN_DUAL_SYS_1;
        break;
#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
        dual_sys = MN_DUAL_SYS_1;
        break;
#endif        
    case MSG_KEYUP_CALL2:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        dual_sys = MN_DUAL_SYS_2;
#endif
        break;
    case MSG_KEYUP_CALL3:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        dual_sys = MN_DUAL_SYS_3;
#endif
        break;
    case MSG_KEYUP_CALL4:
#if defined(MMI_MULTI_SIM_SYS_QUAD)
         dual_sys = MN_DUAL_SYS_4;
#endif
        break;
    default:
        break;
    }
    return dual_sys;
}
#if defined(MMIPB_MOST_USED_SUPPORT) && defined(MMI_PDA_SUPPORT)
LOCAL void AppendTextListItem(MMI_CTRL_ID_T    ctrl_id,
                                     MMI_STRING_T    text_str1
                                     )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

	item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
	
	item_t.item_data_ptr = &item_data;
	
	item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[0].item_data.text_buffer = text_str1;
	
    GUILIST_AppendItem( ctrl_id, &item_t );
}


/*****************************************************************************/
// 	Description : init controls of call time list window
//	Global resource dependence : 
//  Author:Sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void SetMostusedWinCtrl(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_HEIGHT_T  height_info={0};
    uint16 ver_space = 0;
    uint16 hor_space = 0;
#ifdef TOUCH_PANEL_SUPPORT
    GUI_BORDER_T    btn_border= {1,MMI_GRAY_WHITE_COLOR,GUI_BORDER_NONE};
    GUI_BG_T        bg_info = {0};
#endif
    MMI_HANDLE_T    main_form_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_FORM_CTRL_ID);
#ifdef TOUCH_PANEL_SUPPORT
    MMI_HANDLE_T    btn_form_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_BTN_FORM_CTRL_ID);
    MMI_HANDLE_T    call_crtl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAI_BTN_CALL_CTRL_ID);
#ifdef MMI_IP_CALL_SUPPORT
    MMI_HANDLE_T    ipcall_crtl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAI_BTN_IPCALL_CTRL_ID);
#endif
    MMI_HANDLE_T     msg_crtl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAI_MSG_CTRL_ID);
    MMI_HANDLE_T     delete_crtl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAI_BTN_DEL_CTRL_ID);
#endif
    MMI_HANDLE_T     list_crtl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_LIST_CTRL_ID);
    
#ifdef MMI_PDA_SUPPORT
    MMI_HANDLE_T    title_form_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_TITLE_FORM_CTRL_ID);
    MMI_HANDLE_T    photo_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_PHOTO_CTRL_ID);
    MMI_HANDLE_T    label_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_LABEL_CTRL_ID);
#endif


    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return;
    }
    
    //Set Main Form
    GUIFORM_IsSlide(main_form_handle,FALSE);
#ifdef TOUCH_PANEL_SUPPORT
    GUIFORM_IsSlide(btn_form_handle,FALSE);
#endif
    GUIFORM_PermitChildBg(main_form_handle, FALSE);
    GUIFORM_PermitChildBorder(main_form_handle, FALSE);
    GUIFORM_SetMargin(main_form_handle, 0);
    GUIFORM_SetSpace(main_form_handle, &ver_space, &hor_space);
    
#ifdef MMI_PDA_SUPPORT
    //Set Title bar
    if (!MMITHEME_IsIstyle())
    {
        //GUI_BG_T                            bg_info = {0};
        GUIFORM_CHILD_WIDTH_T               sbs_width = {0};
        GUIFORM_CHILD_HEIGHT_T              sbs_height = {0};
    
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id = IMAGE_CL_DETAIL_TITLE_BG;
        GUIFORM_SetBg(title_form_handle, &bg_info);

        sbs_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        sbs_height.add_data = PB_LIST_CONTACT_INFO_BAR_HEIGHT;
	    GUIFORM_SetChildHeight(main_form_handle, title_form_handle, &sbs_height);
        GUIFORM_SetMargin(title_form_handle, PB_LIST_CONTACT_INFO_BAR_MARGIN);
        GUIFORM_SetSpace(title_form_handle,&hor_space,&ver_space);

        sbs_height.add_data = PB_LIST_CONTACT_INFO_BAR_ICON_HEIGHT;
        GUIFORM_SetChildHeight(title_form_handle, photo_ctrl_handle, &sbs_height);
        GUIFORM_SetChildHeight(title_form_handle, label_ctrl_handle, &sbs_height); 

        //Set Width
        sbs_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        sbs_width.add_data = PB_LIST_CONTACT_INFO_BAR_ICON_HEIGHT; //Same as height
        GUIFORM_SetChildWidth(title_form_handle,photo_ctrl_handle,&sbs_width);

    }
#if defined(MMI_ISTYLE_SUPPORT)	
    else
    {
        height_info.type = GUIFORM_CHILD_HEIGHT_FIXED;
        height_info.add_data = 0;
        GUIFORM_SetChildHeight(main_form_handle, title_form_handle, &height_info);
        GUIFORM_SetVisible(title_form_handle, FALSE,FALSE);   //Istyle Mode
    }
#endif	
#else    
#endif
#ifdef TOUCH_PANEL_SUPPORT
    //Set Btn Form BG
    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color = PB_LIST_BUTTON_FORM_BG_COLOR;

    GUIFORM_SetBg(btn_form_handle,&bg_info);
    
    GUIFORM_PermitChildBorder(btn_form_handle, FALSE);
    GUIFORM_SetMargin(btn_form_handle, 0);
    GUIFORM_SetSpace(btn_form_handle, &ver_space, &hor_space);
    //Set Btn Border
    height_info.type = GUIFORM_CHILD_HEIGHT_FIXED;
    height_info.add_data = PB_LIST_BUTTON_FORM_HEIGHT;  //Gui requirements

    GUIBUTTON_SetBorder(call_crtl_handle, &btn_border,FALSE);
#ifdef MMI_IP_CALL_SUPPORT
    GUIBUTTON_SetBorder(ipcall_crtl_handle, &btn_border,FALSE);
#endif
    GUIBUTTON_SetBorder(msg_crtl_handle, &btn_border,FALSE);
    GUIBUTTON_SetBorder(delete_crtl_handle, &btn_border,FALSE);

    GUIFORM_SetChildHeight(btn_form_handle,call_crtl_handle, &height_info);
#ifdef MMI_IP_CALL_SUPPORT
    GUIFORM_SetChildHeight(btn_form_handle,ipcall_crtl_handle, &height_info);
#endif
    GUIFORM_SetChildHeight(btn_form_handle,msg_crtl_handle, &height_info);
    GUIFORM_SetChildHeight(btn_form_handle,delete_crtl_handle, &height_info);

#endif
    //Set list ctrl
    height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(main_form_handle, list_crtl_handle, &height_info);
    GUILIST_PermitBorder(list_crtl_handle, FALSE);

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InitMostusedDetailControls(MMI_WIN_ID_T  win_id , MMIPB_CONTACT_T* contact_ptr)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T applet_handle = NULL;
    MMI_STRING_T                        string_to = {0};
    uint32      search_group = 0;
    MMIPB_CUSTOM_PHOTO_T* custom_photo_ptr = PNULL; 
    MMIPB_NAME_T* name_t_ptr = PNULL;
    MMIPB_STRING_T* num_str_t_ptr = PNULL;
    wchar                               *item_text_ptr = PNULL;
    BOOLEAN is_photo_id_exist  = FALSE;
    MMI_STRING_T str_t = {0};
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    GUIANIM_FILE_INFO_T                 file_info  = {0};
    MN_NUMBER_TYPE_E                    number_type = 0;//
    uint8                               num_type = 0;

    if(PNULL == applet_ptr)
    {
        MMK_CloseWin(win_id);
        return;
    } 
    applet_handle = applet_ptr->caf_applet.app_handle;

    if(contact_ptr == PNULL)
    {
        return;
    }   
    SetMostusedWinCtrl(win_id);
    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
    name_t_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_NAME_T));
    num_str_t_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_STRING_T));
    item_text_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN +1));

    if(custom_photo_ptr == PNULL || name_t_ptr == PNULL 
        ||num_str_t_ptr == PNULL || item_text_ptr == PNULL
        )
    {
        if(custom_photo_ptr != PNULL)
        {
            SCI_FREE(custom_photo_ptr);
        }
        if(name_t_ptr != PNULL)
        {
            SCI_FREE(name_t_ptr);
        }
        if(num_str_t_ptr != PNULL)
        {
            SCI_FREE(num_str_t_ptr);
        }
        if(item_text_ptr != PNULL)
        {
            SCI_FREE(item_text_ptr);
        }
      
        return; //pclint error
    }

//////////////////////////////////set contact picture /////////////////////////////////////////////   
   // if(!MMIPB_IsMsisdnContact(contact_ptr->storage_id))
    {
 	    if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST) 
            && (MMIPB_ReadPhotoFile(contact_ptr->contact_id, contact_ptr->storage_id, custom_photo_ptr))
            )
	    {

		    if (MMIAPIFMM_IsFileExist(
                (const uint16 *)custom_photo_ptr->file_name,
                (uint16)(custom_photo_ptr->file_name_len)
                ))
		    {
			    is_photo_id_exist = TRUE;
            }
	    }
	    if (is_photo_id_exist)
	    {

            ctrl_info.is_wallpaper_ctrl = FALSE;
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID);
    
            display_info.is_update = FALSE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            display_info.is_disp_one_frame = TRUE;
            display_info.is_crop_rect = TRUE;
            display_info.is_zoom = TRUE;
            display_info.is_syn_decode = TRUE;
          
    #ifdef DRM_SUPPORT			
		    if(MMIAPIDRM_IsDRMFile( SFS_INVALID_HANDLE, custom_photo_ptr->file_name))
		    {
			    MMIAPIDRM_ConsumeRights( SFS_INVALID_HANDLE, custom_photo_ptr->file_name);
		    }
    #endif

            file_info.full_path_wstr_ptr = custom_photo_ptr->file_name;
            file_info.full_path_wstr_len = custom_photo_ptr->file_name_len;
            GUIANIM_SetParam(&ctrl_info,
                    PNULL,
                    &file_info,
                    &display_info);
	    }
	    else
	    {
		    //display default icon
		    //group_id = contact_ptr->union_t.group;
            uint8 group_id = 0;
            if(MMIPB_IsMostUsedContact(contact_ptr->storage_id))
            {
                 MMIPB_GetAllGroupIDs(search_group, &group_id, 1);
            }

		    if(group_id >= MMIPB_MAX_GROUP_NUM)
		    {
			    group_id =PB_GROUP_ALL;
		    }

            ctrl_info.is_ctrl_id     = TRUE;
            ctrl_info.ctrl_id        = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_PHOTO_CTRL_ID);
            ctrl_info.ctrl_ptr = PNULL;
            data_info.img_id         = GetBigIcon(group_id); //IAMGE_PB_DEFAULT_PHOTO;
            display_info.is_update   = FALSE;
            display_info.is_disp_one_frame = TRUE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
	    } 
    }

///////////////////////////////////////////////set contact name/////////////////////////////////////////////
    string_to.wstr_ptr = contact_ptr->name.wstr;
    if(MMIPB_GetNameByNumber(&(contact_ptr->number[0]), &string_to, MMIPB_NAME_MAX_LEN, PNULL,PNULL,PNULL,FALSE, FALSE))/*lint !e64*/
    {
            contact_ptr->name.wstr_len = string_to.wstr_len;
    }

    if(contact_ptr->name.wstr_len !=0)
    {
        str_t.wstr_len = contact_ptr->name.wstr_len;
        SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN+1));
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->name.wstr, MIN(GUIRICHTEXT_TEXT_MAX_LEN, contact_ptr->name.wstr_len));
        str_t.wstr_ptr = item_text_ptr;
        GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_LABEL_CTRL_ID), &str_t, FALSE);
    }

    else
    {
        MMI_GetLabelTextByLang(TXT_PB_SPACE_NO_NAME, &str_t);
        GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_LABEL_CTRL_ID), &str_t, FALSE);
    }
///////////////////////////////////////////////set contact name/////////////////////////////////////////////
    if(MMIPB_IsValidPhoneNumber(contact_ptr->number[0].npi_ton, contact_ptr->number[0].number_len ))
    {
        MMI_STRING_T num_str = {0};
        number_type = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[0].npi_ton);
        num_str_t_ptr->strlen = MMIAPICOM_GenDispNumber(
            (number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),//MMIPB_GetNumberTypeFromUint8(entry_ptr->number_t[i].npi_ton), 
            contact_ptr->number[0].number_len , 
            contact_ptr->number[0].number,
            num_str_t_ptr->strbuf,
            MMIPB_MAX_STRING_LEN+2
            );
       
    num_type = contact_ptr->num_type[0];
    SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN+1));
    MMI_STRNTOWSTR(item_text_ptr, GUIRICHTEXT_TEXT_MAX_LEN, num_str_t_ptr->strbuf, MIN(num_str_t_ptr->strlen, GUIRICHTEXT_TEXT_MAX_LEN), num_str_t_ptr->strlen);

    num_str.wstr_len = num_str_t_ptr->strlen;
    num_str.wstr_ptr = item_text_ptr;

    GUILIST_SetMaxItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_LIST_CTRL_ID), 1, FALSE);
    AppendTextListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_LIST_CTRL_ID),num_str);


   }
  //  if(custom_photo_ptr != PNULL)
    {
        SCI_FREE(custom_photo_ptr);
    }
  //  if(name_t_ptr != PNULL)
    {
        SCI_FREE(name_t_ptr);
    }
  //  if(num_str_t_ptr != PNULL)
    {
        SCI_FREE(num_str_t_ptr);
    }
   // if(item_text_ptr != PNULL)
    {
        SCI_FREE(item_text_ptr);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMostUsedDetailWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_DETAIL_LIST_CTRL_ID);
   // uint16 cur_selection=0;
   // MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    //CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
   // MMI_RESULT_E                        result                    = MMI_RESULT_TRUE;
  //  MMI_CTRL_ID_T                       ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID);
    //MMI_HANDLE_T                        hToolbar = MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_TOOLBAR_LISTBOX_CTRL_ID);
    //MMI_CTRL_ID_T                       src_id = NULL;
   // MMIPB_MENU_DATA_INFOR_T menu_data = {0};
    //uint16      cur_index    = 0;
    //uint16      num_index = MMIPB_MAX_NV_PHONE_NUM;
		         
   // GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    //const GUILIST_ITEM_T*  list_item_ptr      = PNULL;
    //void *anim_ctrl_ptr = PNULL;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T applet_handle = NULL;
    MMIPB_CONTACT_T * contact_ptr =	PNULL; 
    if((PNULL == applet_ptr))
    {
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    } 
    
    applet_handle = applet_ptr->caf_applet.app_handle;
    contact_ptr = 	(MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);     
    switch(msg_id)
    {   
    case MSG_OPEN_WINDOW:
        {
            applet_ptr->instance.cur_entry_id = contact_ptr->contact_id;
            applet_ptr->instance.cur_storage_id = contact_ptr->storage_id;
            InitMostusedDetailControls(win_id, contact_ptr);
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT 
   case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T src_ctrl_id = 0;
           // MMIPB_BCD_NUMBER_T bcd_number = {0};
            uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};
            MMI_STRING_T name_str={0};
            MMIPB_BCDNumberToUTF8(&contact_ptr->number[0], MMIPB_NUMBER_MAX_STRING_LEN + 2, tele_num);
            name_str.wstr_ptr =contact_ptr->name.wstr;
		    name_str.wstr_len =contact_ptr->name.wstr_len;
            src_ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            switch(src_ctrl_id)
            {
            case MMIPB_MOSTUSED_DETAI_BTN_CALL_CTRL_ID:
                MMIPB_CallNumber(MN_DUAL_SYS_MAX, 
                                 MMIPB_CALL_TYPE_MAKECALL_AUDIO,
                                 tele_num,
                                 &name_str
                                 );
                break;
#ifdef MMI_IP_CALL_SUPPORT
            case MMIPB_MOSTUSED_DETAI_BTN_IPCALL_CTRL_ID:
                MMIPB_CallNumber(MN_DUAL_SYS_MAX, 
                                 MMIPB_CALL_TYPE_MAKEIPCALL,
                                 tele_num,
                                 &name_str
                                 );
                break;
#endif

            case MMIPB_MOSTUSED_DETAI_MSG_CTRL_ID:
                MMIPB_SendSMSByNumber(contact_ptr->number[0]);
                break;

            case MMIPB_MOSTUSED_DETAI_BTN_DEL_CTRL_ID:
                MMIPB_DeleteMostContact(contact_ptr->contact_id, contact_ptr->storage_id);
                MMIPB_RemoveContactFromList(contact_ptr->contact_id, contact_ptr->storage_id); 
                MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMI_PB_RELOAD_SEARCH_LIST_MSG, PNULL);
                MMK_CloseWin( win_id );
                break;

            default:
                break;
            }       
        }
        break;
#endif
     
                
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
        case MSG_CLOSE_WINDOW:
             MMK_FreeWinAddData(win_id);
            break;

             
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#endif

#ifdef MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void AppendTextAndTextListItem(MMI_CTRL_ID_T    ctrl_id,
                                     MMI_STRING_T    text_str1,
                                     MMI_STRING_T     text_str2
                                     )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

	item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
	
	item_t.item_data_ptr = &item_data;
	
	item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[0].item_data.text_buffer = text_str1;
	
	item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[1].item_data.text_buffer = text_str2;
    GUILIST_AppendItem( ctrl_id, &item_t );
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void AppendDetailNumberListItem(MMI_CTRL_ID_T    ctrl_id,
                            MMI_STRING_T    text_str1,
                            MMI_STRING_T     text_str2,
                            MMI_STRING_T     text_str3,
                            MMI_IMAGE_ID_T   imgae_id1,
                            MMI_IMAGE_ID_T   imgae_id2,
                            uint32           item_type,
                            uint16           list_group,
                            BOOLEAN     is_sdn
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

	item_t.user_data = item_type;
#if defined MMI_ISTYLE_SUPPORT 	
    if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;
		item_t.item_group = list_group;		
		item_t.item_data_ptr = &item_data;		

		item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[0].font_color_id = MMI_BLACK_COLOR;
		item_data.item_content[0].item_data.text_buffer = text_str1;
        
		item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[0].font_color_id = MMI_BLACK_COLOR;
		item_data.item_content[1].item_data.text_buffer = text_str3;
    }
	else
#endif		
	{
		item_t.item_group = list_group;
		item_t.item_data_ptr = &item_data;
		item_t.item_state |=GUIITEM_STATE_CONTENT_CHECK;
		
		item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[0].item_data.text_buffer = text_str1;
		
            if(TRUE == is_sdn)
            {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;		
	         item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	         item_data.item_content[1].item_data.text_buffer = text_str3;
            }
            else
            {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_2TEXT_2ANIM_AND_TEXT;
		
                item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer = text_str2;	               
                item_data.item_content[2].item_data_type    = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[2].item_data.image_id = imgae_id1;
                item_data.item_content[3].item_data_type    = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[3].item_data.image_id = imgae_id2;    		
                item_data.item_content[4].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[4].item_data.text_buffer = text_str3;
            }
	}
	GUILIST_AppendItem( ctrl_id, &item_t );
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void Append2TextAndIconListItem(MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            MMI_STRING_T     text_str2,
                            MMI_IMAGE_ID_T   imgae_id1,
                            uint32           item_type,
                            uint16           list_group
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.user_data = item_type;
    item_t.item_group = list_group;
    item_t.item_data_ptr = &item_data;
    item_t.item_state |=GUIITEM_STATE_CONTENT_CHECK;
#if defined MMI_ISTYLE_SUPPORT 
    if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id;		
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;//;
        item_data.item_content[1].item_data.text_buffer = text_str2;
    }
    else
#endif		
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id;		
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = imgae_id1;		
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer = text_str2;
    }

    GUILIST_AppendItem( ctrl_id, &item_t );
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void AppendTextAndtEXTListItem(MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            MMI_STRING_T     text_str2,
                            uint32           item_type,
							uint16           item_group,
							BOOLEAN          is_content_check,
							BOOLEAN          is_expand
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		item_t.item_style = GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;
		if(is_expand)
		{
			item_t.item_state |= GUIITEM_STATE_EXPAND; 
		}
		if(is_content_check)
		{
			item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK; 
		}
	}
	else
#endif		
	{
		item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
	}

	item_t.user_data = item_type;
    item_t.item_data_ptr = &item_data;
    item_t.item_group = item_group;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer = text_str2;

    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }

}
#endif

#if defined(MMI_PDA_SUPPORT)&&defined(MMIWIDGET_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void AppendTwoButtonTextListItem(MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id1,
                            MMI_TEXT_ID_T    text_id2,
                            uint32           item_type,
							uint16           item_group
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

	item_t.user_data = item_type;
	item_t.item_style = GUIITEM_STYLE_ONE_LINE_TWO_BUTTON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.item_group = (item_group | GUIITEM_GROUP_NO_BG);
	item_t.item_state |=GUIITEM_STATE_CONTENT_CHECK;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id1;
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id2;

    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }

}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void AppendOneButtonTextListItem(MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id1,                           
                            uint32           item_type,
							uint16           item_group
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

	item_t.user_data = item_type;
	item_t.item_style = GUIITEM_STYLE_ONE_LINE_BUTTON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.item_group = (item_group | GUIITEM_GROUP_NO_BG);
	item_t.item_state |=GUIITEM_STATE_CONTENT_CHECK;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id1;
	GUILIST_AppendItem( ctrl_id, &item_t );
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsEntryExistContent(MMIPB_CONTACT_T * contact_ptr)
{
	BOOLEAN result = FALSE;

   if(PNULL != contact_ptr)
   {
      if(MMIPB_GET_STORAGE(contact_ptr->storage_id)>=MMIPB_STORAGE_SIM1)
	  {
		  //Nv entry must exist valid number or exist mail
          if(MMIPB_IsExistValidNumber(contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM)/*exist valid number*/
#ifdef MMIPB_MAIL_SUPPORT
			||(contact_ptr->mail.wstr_len > 0)/*exist mail*/
#endif
			  )
		  {
			  result = TRUE;
		  }
	  }
      else
	  {
		  result = TRUE;
	  }
   }
   return result;
}
#endif


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN   UpdateEntryDetail(MMIPB_CONTACT_T* contact_ptr, 
                          MMIPB_CONTACTS_TYP_E contact_type,           
                          MMI_WIN_ID_T win_id,
                          uint32 search_group
                          )
{
    MMIPB_CUSTOM_PHOTO_T                *custom_photo_ptr = PNULL;
    BOOLEAN                             is_photo_id_exist =FALSE;  

    //GUI_BG_T                            button_bg = {0};/*lint !e64*/
    int                                 i=0;
	uint8                               num_type = 0;

    uint32                              string_len = 0;
    MMIPB_NAME_T                        *name_t_ptr = PNULL;/*lint !e64*/
  //  GUIRICHTEXT_ITEM_T                  item_data = {0};/*lint !e64*/
    uint16                              *file_name_ptr = PNULL;
    uint16                              file_name_len = 0;  
//#ifdef ASP_SUPPORT
   // MMI_TEXT_ID_T                       text_id    = TXT_NULL;
//#endif
    MMI_STRING_T                        str_t = {0};
    MMI_STRING_T                        num_str ={0};
	MMI_STRING_T                        numlocation_str ={0};
	MMIPB_STRING_T                 *num_str_t_ptr = PNULL;
	wchar                               *item_text_ptr = PNULL;
    MMIPB_CUSTOM_RING_T                *custom_ring_ptr = PNULL; 
    uint16                              list_group = 0;
    uint8                               group_id = PB_GROUP_ALL;
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    GUIANIM_FILE_INFO_T                 file_info  = {0};
    //GUI_RECT_T                          button_rect={0};
    //LOCAL MMIPB_PB_OPER_DATA_T          pb_oper = {0};/*lint !e64*/
    GUIFORM_CHILD_WIDTH_T               sbs_width = {0};
    GUIFORM_CHILD_HEIGHT_T              sbs_height = {0};
    GUI_BG_T                            bg_info = {0};
   // GUILIST_EMPTY_INFO_T                empty_info = {0};
    MN_NUMBER_TYPE_E                    number_type = 0;//
    uint16                              image_width = 0;
    uint16                              image_height = 0;
    uint16                              icon_width = 0;
    uint16                              icon_height = 0;
   // uint16					            valid_num = 0;
#if defined (HOME_LOCATION_INQUIRE_SUPPORT)  
    wchar                               num_location[MMIPB_MAX_STRING_LEN + 2] = {0};
#endif
    BOOLEAN                             has_valid_num = FALSE;
    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
    name_t_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_NAME_T));
    file_name_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*MMIPB_MAX_CUSTOM_RING_PATH_LEN);
    num_str_t_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_STRING_T));
    item_text_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN +1));
    custom_ring_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_RING_T));

    if(custom_photo_ptr == PNULL || name_t_ptr == PNULL || file_name_ptr == PNULL
        ||num_str_t_ptr == PNULL || item_text_ptr == PNULL || custom_ring_ptr == PNULL
        )
    {
        if(custom_photo_ptr != PNULL)
        {
            SCI_FREE(custom_photo_ptr);
        }
        if(name_t_ptr != PNULL)
        {
            SCI_FREE(name_t_ptr);
        }
        if(file_name_ptr != PNULL)
        {
            SCI_FREE(file_name_ptr);
        }
        if(num_str_t_ptr != PNULL)
        {
            SCI_FREE(num_str_t_ptr);
        }
        if(item_text_ptr != PNULL)
        {
            SCI_FREE(item_text_ptr);
        }
        if(custom_ring_ptr != PNULL)
        {
            SCI_FREE(custom_ring_ptr);
        }        
        return FALSE; //pclint error
    }
    GUIFORM_IsSlide(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID), FALSE);
    GUIFORM_PermitChildBg(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID), FALSE);
    GUILIST_PermitBorder(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), FALSE);


    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color= GUI_RGB2RGB565(214, 214, 214); //MMI_GRAY_WHITE_COLOR;
    GUIFORM_SetBg(MMIPB_CHILD_DETAIL_FORM_CTRL_ID, &bg_info);

    GUIRES_GetImgWidthHeight(&image_width, &image_height, IAMGE_PB_DEFAULT_PHOTO, win_id);
    if(image_height == 0)
    {
        image_height = 90;
    }
    sbs_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    sbs_height.add_data = image_height;
    GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID), &sbs_height);
    
   // GUILIST_SetEmptyInfo(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), &empty_info);

    if(IsEntryExistContent(contact_ptr))
	{
        GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), GUIFORM_CHILD_DISP_NORMAL);
	}
	else
	{
        GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), GUIFORM_CHILD_DISP_HIDE);
	}

    sbs_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), &sbs_height);

    sbs_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    sbs_width.add_data = image_width;
    GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID),&sbs_width);

    sbs_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    sbs_height.add_data = image_height;
    GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID), &sbs_height);


    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMG_PB_FAVBRATE_ICON_OFF, win_id);
    if(MMIPB_IsInNvGroupByStorage(contact_ptr->storage_id) 
#ifdef MMIPB_MOST_USED_SUPPORT
     && !MMIPB_IsMostUsedContact(contact_ptr->storage_id)
#endif
     )
    {
        GUI_BG_T                            button_bg = {0};

      //  GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID),GUIFORM_CHILD_DISP_NORMAL);
        sbs_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        sbs_width.add_data =icon_width;
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID),&sbs_width);
        GUIBUTTON_PermitBorder(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID), FALSE);
#ifdef MMIPB_MOST_USED_SUPPORT
        if(MMIPB_IsFavouriteContact(contact_ptr->union_t.group))
        {
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMG_PB_FAVBRATE_ICON_ON;
            GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID), &button_bg);
        }
        else
#endif
        {
            button_bg.bg_type = GUI_BG_IMG;
            button_bg.img_id = IMG_PB_FAVBRATE_ICON_OFF;
            GUIBUTTON_SetBg(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID), &button_bg);
        }
    }
    else
    {
        sbs_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        sbs_width.add_data = 1;
      //  GUIFORM_SetChildDisplay(MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID),MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID),GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID),&sbs_width);
    }
    GUIBUTTON_PermitBorder(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_EDIT_CTRL_ID), FALSE);
    if(MMIPB_IsFixedStorage(contact_ptr->storage_id) 
#ifdef MMIPB_MOST_USED_SUPPORT
     || MMIPB_IsMostUsedContact(contact_ptr->storage_id)
#endif
     )
    {
          icon_width = 0;
          icon_height = 0;
    } 
    else
    {
        GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMG_PB_MENU_EDIT_ICON, win_id);
    }
    sbs_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    sbs_width.add_data = icon_width;
    GUIFORM_SetChildWidth(MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_DETAIL_FORM_CTRL_ID), MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_EDIT_CTRL_ID),&sbs_width);

///////////////////////////////////set contact picture /////////////////////////////////////////////   
   // if(!MMIPB_IsMsisdnContact(contact_ptr->storage_id))
    {
        MMI_IMAGE_ID_T default_image = GetBigIcon(group_id);;
 	    if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST) 
            && (MMIPB_ReadPhotoFile(contact_ptr->contact_id, contact_ptr->storage_id, custom_photo_ptr))
            )
	    {

		    if (MMIAPIFMM_IsFileExist(
                (const uint16 *)custom_photo_ptr->file_name,
                (uint16)(custom_photo_ptr->file_name_len)
                ))
		    {
			    is_photo_id_exist = TRUE;
            }
	    }
#ifdef TCARD_PB_SUPPORT
        if(MMIPB_IsTCardContact(contact_ptr->storage_id, contact_ptr->contact_id ))
        {
            is_photo_id_exist  = FALSE;    
            default_image = IAMGE_PB_TCARD_PHOTO;
        }
#endif
	    if (is_photo_id_exist)
	    {
		    //display user setting photo 
            ctrl_info.is_wallpaper_ctrl = FALSE;
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID);
    
            display_info.is_update = FALSE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            display_info.is_disp_one_frame = TRUE;
            display_info.is_crop_rect = TRUE;
            display_info.is_zoom = TRUE;
            display_info.is_syn_decode = TRUE;
          
    #ifdef DRM_SUPPORT			
		    if(MMIAPIDRM_IsDRMFile( SFS_INVALID_HANDLE, custom_photo_ptr->file_name))
		    {
			    MMIAPIDRM_ConsumeRights( SFS_INVALID_HANDLE, custom_photo_ptr->file_name);
		    }
    #endif

            file_info.full_path_wstr_ptr = custom_photo_ptr->file_name;
            file_info.full_path_wstr_len = custom_photo_ptr->file_name_len;
            GUIANIM_SetParam(&ctrl_info,
                    PNULL,
                    &file_info,
                    &display_info);
	    }
	    else
	    {
		    //display default icon
		    //group_id = contact_ptr->union_t.group;
#ifdef MMIPB_MOST_USED_SUPPORT
            if(MMIPB_IsMostUsedContact(contact_ptr->storage_id))
            {
                 MMIPB_GetAllGroupIDs(search_group, &group_id, 1);
            }
            else
#endif
            {
                MMIPB_GetAllGroupIDs(contact_ptr->union_t.group, &group_id, 1);
            }
		    if(group_id >= MMIPB_MAX_GROUP_NUM)
		    {
			    group_id =PB_GROUP_ALL;
		    }

            ctrl_info.is_ctrl_id     = TRUE;
            ctrl_info.ctrl_id        = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID);
            ctrl_info.ctrl_ptr = PNULL;
            data_info.img_id         =  default_image;//IAMGE_PB_DEFAULT_PHOTO;
            display_info.is_update   = FALSE;
            display_info.is_disp_one_frame = TRUE;
            display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
	    } 
    }

///////////////////////////////////////////////set contact name/////////////////////////////////////////////
    if(contact_ptr->name.wstr_len !=0)
    {
        str_t.wstr_len = contact_ptr->name.wstr_len;
        SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN+1));
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->name.wstr, MIN(GUIRICHTEXT_TEXT_MAX_LEN, contact_ptr->name.wstr_len));
        str_t.wstr_ptr = item_text_ptr;
        GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_NAME_CTRL_ID), &str_t, FALSE);
    }

    else
    {
        MMI_GetLabelTextByLang(TXT_PB_SPACE_NO_NAME, &str_t);
        GUILABEL_SetText(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_NAME_CTRL_ID), &str_t, FALSE);
    }

////////////////////////////////////////////set contact number///////////////////////////////////////////////
#if defined MMI_ISTYLE_SUPPORT 
    if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUILIST_SetListState(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), GUILIST_STATE_SPLIT_LINE, FALSE);
        list_group = 1;
    }
#endif	
    GUILIST_SetMaxItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), MMIPB_LISTBOX_INDEX_MAX, FALSE);
    for ( i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++ )
    {   
        // 从记录中获得号码
        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton, contact_ptr->number[i].number_len ))
        {
            has_valid_num = TRUE;
            number_type = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[i].npi_ton);
            num_str_t_ptr->strlen = MMIAPICOM_GenDispNumber(
                (number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),//MMIPB_GetNumberTypeFromUint8(entry_ptr->number_t[i].npi_ton), 
                contact_ptr->number[i].number_len , 
                contact_ptr->number[i].number,
                num_str_t_ptr->strbuf,
                MMIPB_MAX_STRING_LEN+2
                );
#if defined (HOME_LOCATION_INQUIRE_SUPPORT)  
            {
            
                MMISET_LANGUAGE_TYPE_E language_type=MMISET_LANGUAGE_ENGLISH;
                MMIAPISET_GetLanguageType(&language_type);
	            if(language_type ==  MMISET_LANGUAGE_SIMP_CHINESE   //中文简体 
                   || language_type == MMISET_LANGUAGE_TRAD_CHINESE)   //中文繁体)
                {
                    SCI_MEMSET(num_location, 0X00, sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
                    GetNumberLocation(num_location, num_str_t_ptr->strbuf, num_str_t_ptr->strlen);
                    numlocation_str.wstr_ptr = num_location;
                    numlocation_str.wstr_len = MMIAPICOM_Wstrlen(num_location);
                }
            }
#endif            
            num_type = contact_ptr->num_type[i];
            SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN+1));
            MMI_STRNTOWSTR(item_text_ptr, GUIRICHTEXT_TEXT_MAX_LEN, num_str_t_ptr->strbuf, MIN(num_str_t_ptr->strlen, GUIRICHTEXT_TEXT_MAX_LEN), num_str_t_ptr->strlen);
            MMIPB_GetAllGroupIDs(contact_ptr->union_t.group, &group_id, 1);
            num_str.wstr_len = num_str_t_ptr->strlen;
            num_str.wstr_ptr = item_text_ptr;
#ifdef CMCC_VCARD_SUPPORT
            if(MMIPB_IsUsim(contact_ptr->storage_id))
            {       
                MMIPB_GetNumberTypeString(contact_ptr->storage_id,num_type, &str_t);   
                AppendDetailNumberListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
    		                          str_t, numlocation_str,num_str, 
    				                  IMAGE_PB_DIAL_ICON,IMAGE_PB_SMS_ICON,
    				                  MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + i, 
    				                  list_group,
    				                  FALSE);
                
            }
            else
#endif
            if(MMIPB_IsSIMContact(contact_ptr->storage_id))
            {
                MMI_GetLabelTextByLang(TXT_PB_PHONE, &str_t);
                AppendDetailNumberListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
			                          str_t, numlocation_str,num_str, 
					                  IMAGE_PB_DIAL_ICON,IMAGE_PB_SMS_ICON, 
					                  MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + i, 
					                  list_group,
					                  FALSE);

            }
#if defined (MMIPB_SIMDN_SUPPORT)               
            else if(MMIPB_IsMsisdnContact(contact_ptr->storage_id))
            {
                MMI_GetLabelTextByLang(TXT_PB_PHONE, &str_t);
                AppendTextAndTextListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
                          str_t, 
                          num_str);
            }
#endif               
            else if(MMIPB_IsSDNContact(contact_ptr->storage_id))
            {
                MMI_GetLabelTextByLang(TXT_PB_PHONE, &str_t);
                AppendDetailNumberListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
    		                          str_t, numlocation_str,num_str, 
    				                  IMAGE_PB_DIAL_ICON,IMAGE_NULL,
    				                  MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + i, 
    				                  list_group,
    				                  TRUE);
            }      
            else
            {

                MMIPB_GetNumberTypeString(contact_ptr->storage_id,num_type, &str_t);
                AppendDetailNumberListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
    		                          str_t, numlocation_str,num_str, 
    				                  IMAGE_PB_DIAL_ICON,IMAGE_PB_SMS_ICON,
    				                  MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + i, 
    				                  list_group,
    				                  FALSE);

            }
        }
    }
    if(has_valid_num)
    {
        wchar first_letter = 0;
        AppendSplitListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), first_letter);
    }
////////////////////////////////////////////////set emailbox//////////////////////////////////////////////////////////
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		list_group++;
	}
#endif	
#ifdef MMIPB_MAIL_SUPPORT	
    if(contact_ptr->mail.wstr_len)
    {
         wchar first_letter = 0;
        string_len = contact_ptr->mail.wstr_len;
        string_len = MIN(string_len, GUIRICHTEXT_TEXT_MAX_LEN);
        

        str_t.wstr_len = string_len;
        SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN+1));
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->mail.wstr, str_t.wstr_len);
        str_t.wstr_ptr = item_text_ptr;
       Append2TextAndIconListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
		                          TXT_EMAIL_BOX, 
								  str_t,
								  IMAGE_PB_EMAIL_ICON, 
								  MMIPB_LISTBOX_INDEX_MAIL,
								  list_group);
      
        AppendSplitListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID), first_letter);

    }
#endif

#ifdef MMIPB_IM_MESSAGE_SUPPORT
////////////////////////////////////////////////set imessager//////////////////////////////////////////////////// 
    if(contact_ptr->imessager.wstr_len)
    {
        string_len = contact_ptr->imessager.wstr_len;
        string_len = MIN(string_len, GUIRICHTEXT_TEXT_MAX_LEN);
        str_t.wstr_len = string_len;
        SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*GUIRICHTEXT_TEXT_MAX_LEN);
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->imessager.wstr, string_len);
        str_t.wstr_ptr = item_text_ptr;
       AppendTextAndtEXTListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
		                          MMIPB_GetImTypeTextId(contact_ptr->im_type), 
								  str_t, 
								  MMIPB_LISTBOX_INDEX_QQ,
								  list_group,
								  FALSE,
								  FALSE);

    }
#endif

//////////////////////////////////////////////////////set group//////////////////////////////////////////////////
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
        int count = 0;
        int len =0;
        uint8   group_id_arrary[MMIPB_MAX_GROUP_NUM] = {0};
#if defined MMI_ISTYLE_SUPPORT 		
		if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
		{
			list_group++;
        }
#endif		
        SCI_MEMSET(item_text_ptr, 0x00,sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN + 1));
        count = MMIPB_GetAllGroupIDs(contact_ptr->union_t.group, group_id_arrary, MMIPB_MAX_GROUP_NUM);
        for(i=0; i<count; i++)
        {
            SCI_MEMSET(name_t_ptr, 0X00, sizeof(MMIPB_NAME_T));
#ifdef MMIPB_MOST_USED_SUPPORT 
            if((PB_GROUP_MOSTUSED != group_id_arrary[i]) && (PB_GROUP_FAVROUTE !=group_id_arrary[i]) )
#endif
            {
            
                if(MMIPB_ERROR_SUCCESS == MMIPB_ReadGroupName(name_t_ptr, group_id_arrary[i]))
                {

                   if((name_t_ptr->wstr_len>0)
                       &&((MMIAPICOM_Wstrlen(item_text_ptr)+ name_t_ptr->wstr_len +1) < GUIRICHTEXT_TEXT_MAX_LEN)
                       )
                   {
               
                       if(MMIAPICOM_Wstrlen(item_text_ptr) >0)
                       {
                            MMIAPICOM_Wstrcat(item_text_ptr,L",");
                            MMIAPICOM_Wstrcat(item_text_ptr,L" ");
                            len+=2;
                       }
                       MMIAPICOM_Wstrcat(item_text_ptr,name_t_ptr->wstr); 
                       len+=name_t_ptr->wstr_len;
                   
                   }
                }
            }
        }
        
		
		if(len)
		{
			
			str_t.wstr_len = len;
			str_t.wstr_ptr = item_text_ptr;
			

		}
        else
        {
            MMI_GetLabelTextByLang(TXT_PB_UNCLASS_GROUP, &str_t);
            
        }


           Append2TextAndIconListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
		                              TXT_PB_ITEM_DETAIL_CUSTOM_SELECT_GROUP, 
								      str_t,
								      IMAGE_CONTROL_LIST_RIGHT_ARROW, 
								      MMIPB_LISTBOX_INDEX_GROUP,
								      list_group);  
	}
/////////////////////////////////////////////////////set ring/////////////////////////////////////////////
    if(contact_ptr->ring_id_flag& MMIPB_RING_ID_EXIST)
    {
        if(contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG)
        {
            if( (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK)  <= MMISET_RING_FIXED_DEFAUT_NUM )
            {
                str_t.wstr_len = MMIPB_GetRingName((contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK), 
                    file_name_ptr, MMIPB_MAX_CUSTOM_RING_PATH_LEN);
                
                
                SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN+1));
                MMIAPICOM_Wstrncpy(item_text_ptr, file_name_ptr, MIN(GUIRICHTEXT_TEXT_MAX_LEN, str_t.wstr_len));
                str_t.wstr_ptr = item_text_ptr;
    

  
               Append2TextAndIconListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
		                                  TXT_RING, 
								          str_t,
								          IMAGE_CONTROL_LIST_RIGHT_ARROW, 
								          MMIPB_LISTBOX_INDEX_RING,
								          list_group);  
                
            }
     
        }
        else if(MMIPB_ReadRingFile(contact_ptr->contact_id, contact_ptr->storage_id, custom_ring_ptr))
        {
            if (MMIAPIFMM_IsFileExist(
                custom_ring_ptr->file_name,
                (uint16)(custom_ring_ptr->file_name_len)
                ))
            {
                MMIAPIFMM_SplitFullPath(
                    custom_ring_ptr->file_name,
                    (uint16)(custom_ring_ptr->file_name_len),
                    PNULL,
                    PNULL,
                    PNULL,
                    PNULL,
                    file_name_ptr,
                    &file_name_len
                    );
				
                str_t.wstr_len = file_name_len;
                MMIAPICOM_Wstrncpy(item_text_ptr, file_name_ptr, MIN(file_name_len, GUIRICHTEXT_TEXT_MAX_LEN));
                str_t.wstr_ptr = item_text_ptr;
    
               Append2TextAndIconListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
		                                  TXT_RING, 
								          str_t,
								          IMAGE_CONTROL_LIST_RIGHT_ARROW, 
								          MMIPB_LISTBOX_INDEX_RING,
								          list_group);  
            }
        }
    
    }
#ifndef PB_SUPPORT_LOW_MEMORY    
    ///////////////////////////////////////////////////SET URL///////////////////////////////////////////
     if(contact_ptr->url.wstr_len)
     {
        SCI_MEMSET(item_text_ptr, 0x00,sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN + 1));
        str_t.wstr_len = contact_ptr->url.wstr_len;
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->url.wstr, MIN(str_t.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN));
        str_t.wstr_ptr = item_text_ptr;
#ifdef BROWSER_SUPPORT
#ifdef BROWSER_SUPPORT_DORADO
       Append2TextAndIconListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
		                          TXT_PB_URL, 
								  str_t,
								  IMAGE_BROWSER_ADDRESS_ICON, 
								  MMIPB_LISTBOX_INDEX_URL,
								  list_group); 
#else
       Append2TextAndIconListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
		                          TXT_PB_URL, 
								  str_t,
								  0, 
								  MMIPB_LISTBOX_INDEX_URL,
								  list_group); 
#endif
#else
        AppendTextAndtEXTListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
			                      TXT_PB_URL, 
								  str_t, 
								  MMIPB_LISTBOX_INDEX_URL, 
								  list_group,
								  TRUE,
								  FALSE);
#endif       
     }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
/////////////////////////////////////////////////////set birth/////////////////////////////////////////    
    if(contact_ptr->birth.wstr_len)
    {
        SCI_MEMSET(item_text_ptr, 0x00,sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN + 1));
        str_t.wstr_len = contact_ptr->birth.wstr_len;
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->birth.wstr, MIN(GUIRICHTEXT_TEXT_MAX_LEN, str_t.wstr_len));
        str_t.wstr_ptr = item_text_ptr;

        AppendTextAndtEXTListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
			                      TXT_PB_BIRTHDAY, 
								  str_t, 
								  MMIPB_LISTBOX_INDEX_BIRTH, 
								  list_group,
								  TRUE,
								  FALSE);
    }


////////////////////////////////////////////////////////set 备忘录//////////////////////////////////////////////

    if(contact_ptr->memo.wstr_len)
    {
        SCI_MEMSET(item_text_ptr, 0x00,sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN + 1));
        str_t.wstr_len = contact_ptr->memo.wstr_len;
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->memo.wstr, MIN(str_t.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN));
        str_t.wstr_ptr = item_text_ptr;

        AppendTextAndtEXTListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
			                      TXT_MEMO, 
								  str_t, 
								  MMIPB_LISTBOX_INDEX_MEMO, 
								  list_group,
								  TRUE,
								  FALSE);
    }
#endif
#ifdef SNS_SUPPORT
/////////////////////////////////////////////////////set sign///////////////////////////////////////// 
    if(contact_ptr->sign.wstr_len)
    {
        SCI_MEMSET(item_text_ptr, 0x00,sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN + 1));
        str_t.wstr_len = contact_ptr->sign.wstr_len;
        MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->sign.wstr, MIN(GUIRICHTEXT_TEXT_MAX_LEN, str_t.wstr_len));
        str_t.wstr_ptr = item_text_ptr;

        AppendTextAndtEXTListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
			                      TXT_COMMON_SIGNATURE, 
								  str_t, 
								  MMIPB_LISTBOX_INDEX_SIGN, 
								  list_group,
								  TRUE,
								  FALSE);
    }    

   #endif
////////////////////////////////////////////别名////////////////////////////////////////////////////////////////// 

#ifdef CMCC_VCARD_SUPPORT
	//entry alias
	if(contact_ptr->alias.wstr_len> 0)
	{
            str_t.wstr_len = contact_ptr->alias.wstr_len;
            SCI_MEMSET(item_text_ptr, 0X00, sizeof(wchar)*(GUIRICHTEXT_TEXT_MAX_LEN+1));
            MMIAPICOM_Wstrncpy(item_text_ptr, contact_ptr->alias.wstr, MIN(str_t.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN));
            str_t.wstr_ptr = item_text_ptr;

            AppendTextAndtEXTListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
			                      TXT_PB_ITEM_DETAIL_ALIAS, 
								  str_t, 
								  MMIPB_LISTBOX_INDEX_ALIAS, 
								  list_group,
								  FALSE,
								  FALSE);


	}
#endif
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		if((MMIPB_GET_STORAGE(contact_ptr->storage_id) == MMIPB_STORAGE_FLASH)
		   && (MMIPB_SIM_DN_CONTACTS != contact_type)
		 )
		{
			if(0 < GetValidPhoneNumberCount(contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM))
			{
				list_group++;
				//sms ; add favorate
#ifdef MMIWIDGET_SUPPORT
				AppendTwoButtonTextListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
					TXT_SMS,
					TXT_WIDGET_PB_ADD_TO_FAVORITE,
					MMIPB_LISTBOX_INDEX_MAX,
					list_group);
#else
              AppendOneButtonTextListItem(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID),
					TXT_SMS,
					MMIPB_LISTBOX_INDEX_MAX,
					list_group);
#endif
			}
		}
	}
#endif	
    SCI_FREE(custom_photo_ptr);
    SCI_FREE(name_t_ptr);
    SCI_FREE(file_name_ptr);
    SCI_FREE(num_str_t_ptr);
    SCI_FREE(item_text_ptr);
    SCI_FREE(custom_ring_ptr);
    return TRUE;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : Get  Valid Number count
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetValidPhoneNumberCount(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count)
{
     uint8 loop_count = MIN(MMIPB_MAX_NV_PHONE_NUM, max_count);
     uint8  i = 0;
     uint8  valid_count = 0; 

     for(i = 0; i < loop_count; i++)
     {
         if(MMIPB_IsValidPhoneNumber(number_t_ptr[i].npi_ton, number_t_ptr[i].number_len))
         {
             valid_count++;
         }
     }
     return valid_count;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 处理详情页面点击item后面图标的操作
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: //    uint16                      item_index;		 /*!< 控件索引*/
           //uint16                      item_content_index; /*!< 内容项索引*/
/*****************************************************************************/

LOCAL void HandleIconCheckContent(MMI_WIN_ID_T win_id, MMIPB_MENU_DATA_INFOR_T *menu_infor_ptr, MMI_CTRL_ID_T ctrl_id ,GUILIST_ITEM_T*  list_item_ptr, GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr)
{ 
    uint32      item_id = 0;
   // uint8       valid_num = 0;
//    uint16      num_len = 0;
//	uint8 num_str[MMIPB_MAX_STRING_LEN + 2] = {0};
  //  MMI_STRING_T     name_str={0}; 
    MMI_STRING_T send_string = {0};
    //CC_CALL_SIM_TYPE_E                  sim_type = CC_MAJOR_SIM_CALL;
    
    if(PNULL ==menu_infor_ptr || list_item_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] HandleIconCheckContent menu_infor_ptr %d, list_item_ptr %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6456_112_2_18_2_44_26_332,(uint8*)"dd", menu_infor_ptr, list_item_ptr);
        return;
    }
    item_id = list_item_ptr->user_data;
    //SCI_TRACE_LOW:"MMIPB HandleIconCheckContent item_id %d item_content_ptr->item_content_index %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6460_112_2_18_2_44_26_333,(uint8*)"dd",item_id,item_content_ptr->item_content_index);
#ifdef MMIPB_MAIL_SUPPORT
    //pda        
    if(item_id == MMIPB_LISTBOX_INDEX_MAIL ) 
    {

        //email: send email
        if(menu_infor_ptr->contact_ptr->mail.wstr_len>0)
        {
 
            //send mms
            send_string.wstr_len = menu_infor_ptr->contact_ptr->mail.wstr_len;
            send_string.wstr_ptr = menu_infor_ptr->contact_ptr->mail.wstr;
            MMIAPIEMAIL_EditEmail(&send_string);    
        }  
        
    }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    if(item_id == MMIPB_LISTBOX_INDEX_URL )
    {
        //go to url -->wap
#ifdef BROWSER_SUPPORT
        MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
//        int32                  url_len = 0;
        uint8 url[GUIRICHTEXT_URL_MAX_LEN + 1] = {0};
    
        entry_param.type = MMIBROWSER_ACCESS_URL;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        entry_param.dual_sys = MMI_DUAL_SYS_MAX;
#else
        entry_param.dual_sys = MN_DUAL_SYS_1;
#endif
        MMI_WSTRNTOSTR(url, GUIRICHTEXT_URL_MAX_LEN, 
            menu_infor_ptr->contact_ptr->url.wstr, 
            GUIRICHTEXT_URL_MAX_LEN,
            menu_infor_ptr->contact_ptr->url.wstr_len);
        entry_param.url_ptr = (char *)url;
    
        entry_param.user_agent_ptr = PNULL;
        MMIAPIBROWSER_Entry(&entry_param);									
    
#endif
    }
#endif
    if((item_id >=MMIPB_LISTBOX_INDEX_NUMBER_MOBILE) && (item_id<=MMIPB_LISTBOX_INDEX_NUMBER_OTHER))
    {
#if defined MMI_ISTYLE_SUPPORT     
        if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
                menu_infor_ptr->menu_type = MMIPB_DIAL_SUB_MENU;
                menu_infor_ptr->need_reset_storage = TRUE;   
                menu_infor_ptr->valid_contact_count = 1;
                MMIPB_EnterPbTabOptMenu(menu_infor_ptr);
        }
        else
#endif			
        {
    
            if(item_content_ptr->item_content_index == 2)
            {
                menu_infor_ptr->menu_type = MMIPB_DIAL_SUB_MENU;
                menu_infor_ptr->need_reset_storage = TRUE;   
                menu_infor_ptr->valid_contact_count = 1;
                MMIPB_EnterPbTabOptMenu(menu_infor_ptr);
            }
            else
            {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIPB_SendSMSByNumber(
                    menu_infor_ptr->contact_ptr->number[menu_infor_ptr->number_index]);
#else
                menu_infor_ptr->menu_type = MMIPB_MSG_SUB_MENU;
                menu_infor_ptr->need_reset_storage = TRUE;
                menu_infor_ptr->valid_contact_count = 1;
                MMIPB_EnterPbTabOptMenu(menu_infor_ptr);
#endif
            }
        }
    }
    if(MMIPB_LISTBOX_INDEX_RING == item_id) 
    {
        MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
        uint16 group_id = PB_GROUP_ALL;
        if(PNULL == applet_ptr)
        {
            return;
        }
       
        if(PNULL!=menu_infor_ptr->contact_ptr)
        {
             MMIPB_GetAllGroupIDs(menu_infor_ptr->contact_ptr->union_t.group, &group_id, 1);/*lint !e64*/
             MMIPB_OpenRingSelWin(win_id, applet_ptr->caf_applet.app_handle ,group_id);
        }
    }
    if(MMIPB_LISTBOX_INDEX_GROUP == item_id)
    {
        MMIPB_MENU_DATA_INFOR_T* menu_data_ptr =SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
        if(PNULL == menu_data_ptr)
        {
            //SCI_TRACE_LOW:"[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6904_112_2_18_2_44_27_337,(uint8*)"");
            return;
        }
        SCI_MEMSET(menu_data_ptr, 0x00, sizeof(MMIPB_MENU_DATA_INFOR_T));
        menu_data_ptr->contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
        if(PNULL == menu_data_ptr->contact_ptr)
        {
            //SCI_TRACE_LOW:"[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr->contact_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6911_112_2_18_2_44_27_338,(uint8*)"");
            return ;            
        }
        SCI_MEMCPY( menu_data_ptr->contact_ptr , menu_infor_ptr->contact_ptr, sizeof(MMIPB_CONTACT_T));
        menu_data_ptr->operate_type = MMIPB_OPERATE_TYPE_DETAIL_GROUP;
        menu_data_ptr->valid_contact_count = 1;

        MMIPB_CreateEditGroupWin(win_id, menu_data_ptr);
    }
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : init detail window controls
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void InitDetailContols(MMI_WIN_ID_T       win_id , MMIPB_CONTACT_T* contact_ptr)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
   // MMI_HANDLE_T parent_win_id = MMK_GetParentWinHandle(win_id);
    MMI_HANDLE_T applet_handle = NULL;
    MMI_STRING_T                        string_to = {0};
    uint32      search_group = 0;
    if(PNULL == applet_ptr)
    {
        MMK_CloseWin(win_id);
        return;
    } 
    applet_handle = applet_ptr->caf_applet.app_handle;


    
    if(contact_ptr == PNULL)
    {
        return;
    }
#ifdef MMIPB_MOST_USED_SUPPORT
	if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id )
	{
         string_to.wstr_ptr = contact_ptr->name.wstr; //detail_handle->entry.union_t.entry_name.wstr;
		if(MMIPB_GetNameByNumber(&(contact_ptr->number[0]), &string_to, MMIPB_NAME_MAX_LEN,PNULL,PNULL, &search_group,FALSE, FALSE))
		{
			contact_ptr->name.wstr_len = string_to.wstr_len;
		}
		else
		{
			SCI_MEMSET(&contact_ptr->name, 0x00, sizeof(contact_ptr->name));
		}
	}
#endif
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		if(MMIPB_GET_STORAGE(contact_ptr->storage_id) == MMIPB_STORAGE_FLASH)
		{
			GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
			GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
			GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_SAVE, FALSE);
		}
		else
		{
#ifdef MMIPB_MOST_USED_SUPPORT
			if(MMIPB_MOSTUSED_CONTACTS == applet_ptr->instance.contacts_type
              || (!MMIPB_IsContactHasMenu(contact_ptr->storage_id))
             )
			{
				GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
			}
			else
#endif
			{
				GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
				GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_EDIT, FALSE);
			}
			GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
		}
	}
#endif	
    else
    {
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR) && defined(SNS_SUPPORT)
	#ifdef SNS_SUPPORT
        if(!MMIPB_IsSNSContact(contact_ptr->storage_id))
        #endif	
		{
            PbCreateToolbar(win_id, MMIPB_DETAIL_TOOLBAR_LISTBOX_CTRL_ID, MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID));
        }
#endif
    }
    UpdateEntryDetail(contact_ptr, applet_ptr->instance.contacts_type, win_id, search_group);            

}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :  详情主窗口 处理函数 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDetailMainWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return recode;
    } 
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_MAIN_TAB_CTRL_ID));
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
             MMK_FreeWinAddData(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : draw detail child window
//  Global resource dependence : 
//  Author: gaoping
//  Note: 
/*****************************************************************************/
LOCAL void PbCreateDetailChildWin(
                            MMI_HANDLE_T applet_handle,
                            MMI_WIN_ID_T   win_id,
                            uint16         storage_id,
                            BOOLEAN        is_other
                            )
{
    MMI_STRING_T			str_text = {0};
    uint32                  tab_count = 0;
    uint32                  i = 0;
    uint32                  focus_item_index = 0;
    MMI_WIN_ID_T            focus_sub_win_id = 0;
    MMIPB_WINDOW_TAB_INFO_T *sub_tab_ptr = PNULL;
    MMIPB_APPLET_T*         applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
        
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] PbCreateDetailChildWin PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6781_112_2_18_2_44_27_334,(uint8*)"");
        return;
    }
    if(is_other)
    {
        sub_tab_ptr = s_mmipb_special_other_detail_win_tab;
        tab_count = ARR_SIZE(s_mmipb_special_other_detail_win_tab);  
    }
#ifdef SNS_SUPPORT    
    else if(MMIPB_IsSNSContact(storage_id))
    {
        sub_tab_ptr = s_mmipb_sns_detail_win_tab;
        tab_count = ARR_SIZE(s_mmipb_sns_detail_win_tab);
    }
#endif
    else if(
#ifdef MMI_VCARD_SUPPORT          
        MMIPB_IsVcardContact(storage_id) || 
#endif        
            (MMIPB_IsSimRecord(storage_id) && !MMIPB_IsSIMContact(storage_id))
            )
    {
        sub_tab_ptr = s_mmipb_special_detail_win_tab;
        tab_count = ARR_SIZE(s_mmipb_special_detail_win_tab);        
    }
    else
    {
    
        sub_tab_ptr = s_mmipb_detail_win_tab;
        tab_count = ARR_SIZE(s_mmipb_detail_win_tab);
    }
    //set tab count
    GUITAB_SetMaxItemNum(MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_MAIN_TAB_CTRL_ID), tab_count);
    
    //create every sub tab
    for(i = 0; i < tab_count; i++)
    {
        //tab title
        MMI_GetLabelTextByLang(sub_tab_ptr[i].text_id, &str_text);
        GUITAB_AppendSimpleItem(
        MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_MAIN_TAB_CTRL_ID),
        &str_text,
        sub_tab_ptr[i].tab_on_img_id,
        sub_tab_ptr[i].tab_off_img_id);

    }

    for(i = 0; i < tab_count; i++)
    {
        //create this sub window        
        if(PNULL != sub_tab_ptr[i].create_func)
        {
            sub_tab_ptr[i].create_func(MMK_GetWinHandle(applet_handle, MMIPB_DETAILS_MAIN_WIN_ID));
        }
        
        //add this sub tab to main windows
        GUITAB_AddChildWindow(
            MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_MAIN_TAB_CTRL_ID),
            MMK_GetWinHandle(applet_handle, sub_tab_ptr[i].win_id), 
             i);
    }

    focus_item_index = 0;  //set first focus
    focus_sub_win_id =sub_tab_ptr[focus_item_index].win_id;//;

    GUITAB_SetCurSel(MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_MAIN_TAB_CTRL_ID), focus_item_index);  
    MMK_SetChildWinFocus(MMK_GetWinHandle(applet_handle, MMIPB_DETAILS_MAIN_WIN_ID), MMK_GetWinHandle(applet_handle, focus_sub_win_id));
}
#endif

#if defined (TOUCH_PANEL_SUPPORT) && defined (MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : handle detail window message
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL void DealWithPenDetailNumItem(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, void* param, MMIPB_CONTACT_T* contact_ptr)
{
    uint16      cur_index    = 0;
//    MMIPB_MENU_DATA_INFOR_T menu_data = {0};
    const GUILIST_ITEM_T*  list_item_ptr      = PNULL;
    uint32 item_id = 0;
   // uint8       valid_num = 0;
    uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};
    MMI_STRING_T     name_str={0}; 
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    cur_index = GUILIST_GetCurItemIndex(list_handle);
    list_item_ptr = GUILIST_GetItemPtrByIndex(list_handle, cur_index);
    if(PNULL == list_item_ptr)
    {
        //SCI_TRACE_LOW:"HandleEntryDetailPdaWinMsg list_item_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6866_112_2_18_2_44_27_335,(uint8*)"d", list_item_ptr);
        return;
    }
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"HandleEntryDetailPdaWinMsg applet_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6871_112_2_18_2_44_27_336,(uint8*)"d", applet_ptr);
        return;
    }
    item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
	item_id = list_item_ptr->user_data;
	if((MMIPB_LISTBOX_INDEX_NUMBER_MOBILE <= item_id) 
        && (item_id <(MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + MMIPB_MAX_NV_PHONE_NUM)))
	{
		//phone number	
		MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
        uint8 num_index = item_id - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE;

		MMIPB_BCDNumberToUTF8(&contact_ptr->number[num_index], MMIPB_NUMBER_MAX_STRING_LEN + 2, tele_num);
		
		name_str.wstr_ptr =contact_ptr->name.wstr;
		name_str.wstr_len =contact_ptr->name.wstr_len;
        if(MMIPB_IsSDNContact(contact_ptr->storage_id))
        {
            dual_sys = MMIPB_GetDualSysByStorageId(contact_ptr->storage_id);
        }
        MMIPB_CallNumber(dual_sys, 
                         MMIPB_CALL_TYPE_MAKECALL_AUDIO,
                         tele_num,
                         &name_str
                         );

	}
#ifdef MMIPB_MAIL_SUPPORT
    else if(item_id == MMIPB_LISTBOX_INDEX_MAIL ) 
    {
        //email: send email
        if(contact_ptr->mail.wstr_len>0)
        {

            MMI_STRING_T send_string = {0};
            //send mms
            send_string.wstr_len = contact_ptr->mail.wstr_len;
            send_string.wstr_ptr = contact_ptr->mail.wstr;
            MMIAPIEMAIL_EditEmail(&send_string);    
        }      
    }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY    
    else if(MMIPB_LISTBOX_INDEX_URL == item_id)
    {
#ifdef BROWSER_SUPPORT
        MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
     //   int32                  url_len = 0;
        uint8 url [GUIRICHTEXT_URL_MAX_LEN + 1] = {0};
        entry_param.type = MMIBROWSER_ACCESS_URL;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        entry_param.dual_sys = MMI_DUAL_SYS_MAX;
#else
        entry_param.dual_sys = MN_DUAL_SYS_1;
#endif
       MMI_WSTRNTOSTR(url, GUIRICHTEXT_URL_MAX_LEN, 
            contact_ptr->url.wstr, 
            GUIRICHTEXT_URL_MAX_LEN,
            contact_ptr->url.wstr_len);
        entry_param.url_ptr = (char *)url;        
        entry_param.user_agent_ptr = PNULL;
        MMIAPIBROWSER_Entry(&entry_param);									
        
#endif
    }
#endif 
    else if(MMIPB_LISTBOX_INDEX_RING == item_id) 
    {
        uint16 group_id = PB_GROUP_ALL;
        MMIPB_GetAllGroupIDs(contact_ptr->union_t.group, &group_id, 1);/*lint !e64*/
        MMIPB_OpenRingSelWin(win_id, applet_ptr->caf_applet.app_handle ,group_id);
    }
    else if(MMIPB_LISTBOX_INDEX_GROUP == item_id)
    {
        MMIPB_MENU_DATA_INFOR_T* menu_data_ptr =SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
        if(PNULL == menu_data_ptr)
        {
            //SCI_TRACE_LOW:"[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6904_112_2_18_2_44_27_337,(uint8*)"");
            return;
        }
        SCI_MEMSET(menu_data_ptr, 0x00, sizeof(MMIPB_MENU_DATA_INFOR_T));
        menu_data_ptr->contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
        if(PNULL == menu_data_ptr->contact_ptr)
        {
            //SCI_TRACE_LOW:"[mmipb] DealWithPenDetailNumItem PNULL == menu_data_ptr->contact_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_6911_112_2_18_2_44_27_338,(uint8*)"");
            return ;            
        }
        SCI_MEMCPY( menu_data_ptr->contact_ptr , contact_ptr, sizeof(MMIPB_CONTACT_T));
        menu_data_ptr->operate_type = MMIPB_OPERATE_TYPE_DETAIL_GROUP;
        menu_data_ptr->valid_contact_count = 1;

        MMIPB_CreateEditGroupWin(win_id, menu_data_ptr);
    }
}
#endif

#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR) && defined (MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryDetailWinHook
(
    MMI_WIN_ID_T      win_id, 
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM            param
)
{

    MMI_HANDLE_T toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_TOOLBAR_LISTBOX_CTRL_ID);
    GUI_RECT_T rc_tb = {0};
    GUI_POINT_T pt = {0};          
    

    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        pt.x = MMK_GET_TP_X(param);
        pt.y = MMK_GET_TP_Y(param);
        GUIAPICTRL_GetRect(toolbar_ctrl_handle, &rc_tb);/*lint !e718 !e18*/
        if (!GUI_PointIsInRect(pt, rc_tb))
        {
            if(GUITOOLBAR_GetShowType(toolbar_ctrl_handle)==GUITOOLBAR_EXPAND)
            {
                GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK);
            }
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    default:
        break;
    }
    return MMI_RESULT_FALSE;
   
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle detail window message
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryDetailPdaWinMsg
(
 MMI_WIN_ID_T       win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 )
{
    MMI_RESULT_E                        result                    = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID);
    MMI_HANDLE_T                        hToolbar = MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAIL_TOOLBAR_LISTBOX_CTRL_ID);
    MMI_CTRL_ID_T                       src_id = NULL;
    MMIPB_MENU_DATA_INFOR_T menu_data = {0};
    uint16      cur_index    = 0;
    uint16      num_index = MMIPB_MAX_NV_PHONE_NUM;
		         
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    const GUILIST_ITEM_T*  list_item_ptr      = PNULL;
    //void *anim_ctrl_ptr = PNULL;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    GUI_RECT_T      pb_photo_anim_rect = {0}; 
    MMI_HANDLE_T parent_win_id = MMK_GetParentWinHandle(win_id);
    MMI_HANDLE_T applet_handle = NULL;
    MMIPB_CONTACT_T * contact_ptr =	PNULL; 
    GUI_POINT_T tp_point = {0};
    if((PNULL == applet_ptr))
    {
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    } 
    
    if(parent_win_id == 0)
    {
    	parent_win_id = win_id;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    contact_ptr = 	(MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(parent_win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        applet_ptr->instance.cur_entry_id = contact_ptr->contact_id;
        applet_ptr->instance.cur_storage_id = contact_ptr->storage_id;
        InitDetailContols(win_id, contact_ptr);
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
        MMK_SetWinHookFunc( win_id, HandleEntryDetailWinHook);
#endif
        break;

    case MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG:

        if(GUILIST_GetTotalItemNum(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID)) > 0 )
        {
    
            GUILIST_RemoveAllItems(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID));
        }
         MMIPB_GetContactInfo(contact_ptr->contact_id, contact_ptr->storage_id, contact_ptr);

        UpdateEntryDetail(contact_ptr, applet_ptr->instance.contacts_type, win_id, 0);
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID)))
        {
         MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID), MMI_PB_GROUP_NUM_CHANGED, PNULL);
        }
        MMK_UpdateScreen();

        break;
#ifdef TOUCH_PANEL_SUPPORT
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
	//	anim_ctrl_ptr = GUIANIM_GetPtr(MMK_GetCtrlHandleByWin(win_id,MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID));/*lint !e64 !e718 !e746 !e628*/
      //  if(anim_ctrl_ptr != PNULL)
        {
             GUIAPICTRL_GetRect(MMK_GetCtrlHandleByWin(win_id,MMIPB_ENTRY_DETAIL_PICTURE_CTRL_ID), &pb_photo_anim_rect);
        //    GUIANIM_GetAnimRect(anim_ctrl_ptr, &pb_photo_anim_rect);/*lint !e718 !e746 !e628*/
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            if(tp_point.x>pb_photo_anim_rect.left&&tp_point.x<pb_photo_anim_rect.right&&
                tp_point.y>pb_photo_anim_rect.top&&tp_point.y<pb_photo_anim_rect.bottom)
            {
                GUILIST_SetCurItemIndex(ctrl_id, 0);
                SetToolBarState(hToolbar, contact_ptr);
                UpdatePbListWinToolbar(ctrl_id, hToolbar);
                if(GUITOOLBAR_GetShowType(hToolbar)==GUITOOLBAR_EXPAND)
                {
                    GUITOOLBAR_SetShowType(hToolbar, GUITOOLBAR_SHRINK );
                }
                else
                {
                    GUITOOLBAR_SetShowType(hToolbar, GUITOOLBAR_EXPAND); 
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_NOTIFY_LIST_LONGOK:
        {
    
            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
            if(PNULL == list_item_ptr)
            {
                //SCI_TRACE_LOW:"HandleEntryDetailPdaWinMsg list_item_ptr %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_7019_112_2_18_2_44_27_339,(uint8*)"d", list_item_ptr);
                break;
            }
            if(MMIPB_IsContactHasMenu(contact_ptr->storage_id))
            {
                if((MMIPB_LISTBOX_INDEX_NUMBER_MOBILE <= list_item_ptr->user_data) && (list_item_ptr->user_data < MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + MMIPB_MAX_NV_PHONE_NUM) ) 
                {
                    if(0 != item_content_ptr->item_content_index)
                    {
                        break;
                    }
                    menu_data.applet_handle = applet_handle;
                    menu_data.menu_type = MMIPB_DETAIL_NUMBER_ITEM_OPT_MENU;
                    num_index = list_item_ptr->user_data - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE;
                    menu_data.number_index = num_index;
                    menu_data.contact_ptr = contact_ptr;
                    menu_data.need_reset_storage = FALSE;
                    menu_data.valid_contact_count= 1;
                    MMIPB_EnterPbTabOptMenu(&menu_data);
                }
            }
        }
    break;
        
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
        {            
            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            cur_index = item_content_ptr->item_index;
            list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
            if(PNULL == list_item_ptr) 
            {
                //SCI_TRACE_LOW:"HandleEntryDetailPdaWinMsg list_item_ptr %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_7046_112_2_18_2_44_28_340,(uint8*)"d", list_item_ptr);
                break;
            }
             
            //SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
            menu_data.applet_handle = applet_ptr->caf_applet.app_handle;
           
            menu_data.parent_win = win_id;
            menu_data.valid_contact_count =1;
            menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;

            // TODO: 号码栏显示图标,需要传入号码
            if((MMIPB_LISTBOX_INDEX_NUMBER_MOBILE <= list_item_ptr->user_data) && (list_item_ptr->user_data < MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + MMIPB_MAX_NV_PHONE_NUM) ) 
            {
                num_index = list_item_ptr->user_data - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE;
            }
            menu_data.number_index = num_index;
            menu_data.contact_ptr = contact_ptr;
            HandleIconCheckContent(win_id, &menu_data,ctrl_id, list_item_ptr, item_content_ptr);/*lint !e605*/
        }

        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef SNS_SUPPORT
        if(MMIPB_IsSNSContact(contact_ptr->storage_id))
        {
            //can not be operate
            break;
        }
#endif
        src_id= ((MMI_NOTIFY_T*) param)->src_id;

        switch(src_id)
        {
			case  MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                        //save
                        if(MMIPB_GET_STORAGE(contact_ptr->storage_id) == MMIPB_STORAGE_FLASH)
                        {
                            MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
                            uint16 to_storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
                            ret= MMIPB_SaveVcard(to_storage_id, contact_ptr);
                            if(MMIPB_ERROR_SUCCESS != ret)
                            {
                                MMIPB_OpenAlertTipWin(applet_handle, ret, MMIPB_OPC_IMPORT);
                            }
                            //MMK_CloseWin(win_id);
                            if(MMK_IsOpenWin(parent_win_id))
                            {
                                MMK_CloseParentWin(parent_win_id);
                            }
                        }				
                        break;
			case  MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                        //back
                        if(MMK_IsOpenWin(parent_win_id))
                        {
                            MMK_CloseParentWin(parent_win_id);
                        }
		        break;
			case  MMIPB_ENTRY_DETAIL_EDIT_CTRL_ID:
#ifdef MMIPB_MOST_USED_SUPPORT
                if(!MMIPB_IsMostUsedContact(contact_ptr->storage_id)) //MMIPB_MOSTUSED_CONTACTS != applet_ptr->instance.contacts_type)
#endif
                {
#if defined(FDN_SUPPORT) 
                    if(MMIPB_IsFdnContact(contact_ptr->storage_id))
                    {
                        //PIN2 verify                
                        MMIAPIPHONE_SetCurHandlingPinSimID(MMIPB_GetDualSysByStorageId(contact_ptr->storage_id));
                        MMIAPIPHONE_HandlePinAndPukFlow(
                            MNSIM_PIN_VERIFY,
                            MNSIM_PIN2,
                            (uint32)MMIPB_GetDailNumberListWinTab(),
                            TRUE); 
                        applet_ptr->instance.cmd = MMIPB_OPC_EDIT;
                    }
                    else
#endif
                    {
                        EditContact(applet_ptr, contact_ptr);
                    }
                }
		break;
#ifdef MMIPB_MOST_USED_SUPPORT
            case MMIPB_ENTRY_DETAIL_FAVORITE_CTRL_ID:
                if(MMIPB_IsFavouriteContact(contact_ptr->union_t.group))
                {
                    MMIPB_RemoveUnlistContactFromFavourite(applet_ptr->instance.handle, contact_ptr->contact_id, contact_ptr->storage_id);
                }
                else
                {
                    MMIPB_AddUnlistContactFromFavourite(applet_ptr->instance.handle, contact_ptr->contact_id, contact_ptr->storage_id);;
                }
                MMIPB_OpenWaitWin(applet_ptr->caf_applet.app_handle, TXT_COMMON_WAITING);
                break;
#endif
            case MMIPB_ENTRY_DETAIL_LIST_CTRL_ID:
                {
                    DealWithPenDetailNumItem(win_id, ctrl_id, param, contact_ptr);
                }
                break;
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
            case MMIPB_DETAIL_TOOLBAR_LISTBOX_CTRL_ID:
                {
                
                    PB_OPTMENU_NODE_E node_id = (PB_OPTMENU_NODE_E)GUITOOLBAR_GetItemUserDate(hToolbar, GUITOOLBAR_GetClickItemIndex(hToolbar));
                    HandleToolBarAction(win_id, ctrl_id,node_id, contact_ptr,FALSE);
                }

                break;
#endif
				
            default:
                break;
        }
        break;

#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if(MMIPB_IsFixedContact(contact_ptr->storage_id))
        {
            break;
        }      
        if(MMIPB_GET_STORAGE(contact_ptr->storage_id) == MMIPB_STORAGE_FLASH)
        {
            uint16 to_storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
            MMIPB_SaveVcard(to_storage_id, contact_ptr);
            if(MMK_IsOpenWin(parent_win_id))
            {
                MMK_CloseParentWin(parent_win_id);
            }
            break;
        }
//        else if(applet_ptr->instance.function_type != MMIPB_OPEN_FAVOURITE_VIEW)
        {
            
            if(MMIPB_IsSimRecord(contact_ptr->storage_id) && (!MMIPB_IsSIMContact(contact_ptr->storage_id)))
            {
#if defined(FDN_SUPPORT)
                if(MMIPB_IsFdnContact(contact_ptr->storage_id))
                {

                    //PIN2 verify                
                    MMIAPIPHONE_SetCurHandlingPinSimID(MMIPB_GetDualSysByStorageId(contact_ptr->storage_id));
                    MMIAPIPHONE_HandlePinAndPukFlow(
                        MNSIM_PIN_VERIFY,
                        MNSIM_PIN2,
                        (uint32)MMIPB_GetDailNumberListWinTab(),
                        TRUE); 
                    applet_ptr->instance.cmd = MMIPB_OPC_EDIT;
                }
                else
#endif                    
                {
                    //Edit
                    EditContact(applet_ptr, contact_ptr);
                }
            }
            else
            {
				#ifdef MMIPB_MOST_USED_SUPPORT 
                if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
                {
                    break;
                }
				#endif
#if defined MMI_ISTYLE_SUPPORT 				
                if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
                {
                    EditContact(applet_ptr,contact_ptr);
                    break;
                }	
#endif				
                else
                {
                    
                    //SCI_MEMSET(&add_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
                    menu_data.applet_handle = applet_ptr->caf_applet.app_handle;
                   
                    menu_data.parent_win =MMK_GetWinHandle(applet_handle, MMIPB_DETAILS_MAIN_WIN_ID);
                    menu_data.menu_type = MMIPB_DETAIL_BASIC_OPT_MENU;
                    
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;

                    menu_data.contact_ptr = contact_ptr;
                    menu_data.need_reset_storage = FALSE;
                     menu_data.valid_contact_count =1;
                    MMIPB_EnterPbTabOptMenu(&menu_data);		
                }
            //CREATE MENU
            }
        }
        break;  
    case MMI_PB_SET_DEFAULT_RING:
        {
            contact_ptr->ring_id_flag = 0;
            MMIPB_ModifyContact(contact_ptr);
            MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);

        }
        break;
    case MSG_SET_SELECT_FIX_CALL_RING_RETURN:
        {
            uint16 *ring_id_ptr = PNULL;

        
             ring_id_ptr = (uint16 *)param;                
             contact_ptr->ring_id_flag = ((contact_ptr->ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
             contact_ptr->ring_id_flag = ((contact_ptr->ring_id_flag&MMIPB_FIXED_RING_FLAG_CLEAN) + MMIPB_FIXED_RING_FLAG);
             contact_ptr->ring_id_flag = ((contact_ptr->ring_id_flag&MMIPB_FIXED_RING_ID_CLEAN) + *ring_id_ptr);
             MMIPB_ModifyContact(contact_ptr);
             MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
          //  MMK_UpdateScreen();
        }
        
        break;

    case MSG_MULTIM_SELECTED_RETURN: 
        {
            MULTIM_SELECT_RETURN_T* select_ptr = (MULTIM_SELECT_RETURN_T *)param;
            MMIPB_CUSTOM_RING_T* ring_ptr =SCI_ALLOC_APP(sizeof(MMIPB_CUSTOM_RING_T));
                                
            if(PNULL != ring_ptr)
            {
                SCI_MEMSET(ring_ptr, 0x00, sizeof(MMIPB_CUSTOM_RING_T));
                ring_ptr->is_valid =TRUE;
                contact_ptr->ring_id_flag = ((contact_ptr->ring_id_flag&MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST);
                contact_ptr->ring_id_flag = contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN;
                contact_ptr->ring_id_flag = contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_CLEAN;   
                ring_ptr->file_name_len = select_ptr->file_name_len;
                MMIAPICOM_Wstrncpy(ring_ptr->file_name, select_ptr->file_name_ptr, select_ptr->file_name_len);
                ring_ptr->file_size = select_ptr->file_size;
                ring_ptr->ring_type = select_ptr->music_type;

                MMIPB_ModifyContact(contact_ptr);
                MMIPB_AddContact_RingInfo(contact_ptr, ring_ptr);
                MMK_SendMsg(win_id, MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
              //  MMK_UpdateScreen();
                SCI_FREE(ring_ptr);
            }
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        // 关闭当前窗口
        if(MMK_IsOpenWin(parent_win_id))
        {
            MMK_CloseParentWin(parent_win_id);
         }
        break;
        
    default:
         result = MMI_RESULT_FALSE;
        break;
       }
       return result;
}

#else
/*****************************************************************************/
//  Description :append  icon and text to list item
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItemDetail(MMI_CTRL_ID_T       ctrl_id,
    uint32              item_id,
    uint32              item_index,      
    GUIITEM_STYLE_E     item_style,
    MMI_TEXT_ID_T       left_softkey_id,
    MMI_TEXT_ID_T       middle_softkey_id,
    MMI_TEXT_ID_T       right_softkey_id,
    GUIITEM_DATA_TYPE_E icon_type,
    MMI_IMAGE_ID_T      icon_id,
    GUIANIM_FILE_INFO_T* anim_path,
    const MMI_STRING_T*  str_ptr)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16               content_index = 0;
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = item_id;
    item_data.softkey_id[LEFT_BUTTON] = left_softkey_id;
    item_data.softkey_id[MIDDLE_BUTTON] = middle_softkey_id;
    item_data.softkey_id[RIGHT_BUTTON] = right_softkey_id;
    
    if(GUIITEM_STYLE_ONE_LINE_ICON_TEXT == item_style
        || GUIITEM_STYLE_TWO_LINE_ANIM_TEXT == item_style)
    {
       // item_data.item_content[content_index].item_data_type = icon_type;
        if(GUIITEM_DATA_ANIM_PATH == icon_type && PNULL != anim_path
            && PNULL != anim_path->full_path_wstr_ptr && 0 != anim_path->full_path_wstr_len)
        {
            //need admin
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[content_index].item_data.anim_path_ptr = anim_path;
        }
        else if(GUIITEM_DATA_IMAGE_ID == icon_type)
        {
            //need icon
            item_data.item_content[content_index].item_data_type =GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[content_index].item_data.image_id = icon_id;
        }
        content_index++;
    }
    
    if ( PNULL != str_ptr && 0 < str_ptr->wstr_len && PNULL != str_ptr->wstr_ptr )
    {
        item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[content_index].item_data.text_buffer = *str_ptr;
    }
    
     GUILIST_AppendItem( ctrl_id, &item_t );
}


/*****************************************************************************/
//  Description : 将详情内容加到list中
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryDetails
(
 MMI_WIN_ID_T            win_id,
 MMI_CTRL_ID_T           ctrl_id,    //IN:
 MMIPB_CONTACT_T *contact_ptr,   //IN:
 uint16         search_entry_id,//for most used
 uint16         search_storage_id,//for most used
 uint32         search_group////for most used
 )
{     
    uint8          i = 0;
    uint32 item_index = 0;
    uint8 group_id = 0;
    MMI_STRING_T    str_t = {0};
    GUIITEM_DATA_TYPE_E icon_type = GUIITEM_DATA_IMAGE_ID;
    MMIPB_MENU_DATA_INFOR_T detail_data = {0};
    MMIPB_CUSTOM_PHOTO_T        *custom_photo_ptr = PNULL;
    MMIPB_CUSTOM_RING_T         *custom_ring_ptr = PNULL; 
    MMI_TEXT_ID_T       left_softkey_id = TXT_NULL;
#ifdef GUI_MID_SOFTKEY_USE_IMG
    MMI_IMAGE_ID_T      middle_softkey_id = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
    MMI_TEXT_ID_T       middle_softkey_id = TXT_EDIT;
#endif
    MMI_TEXT_ID_T       right_softkey_id = STXT_RETURN; 
    MMIPB_APPLET_T*     applet_ptr  =  (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);  
    GUIANIM_FILE_INFO_T anim_path = {0};
    
#ifdef DRM_SUPPORT
    MMIFMM_FILE_TYPE_E      file_type = MMIFMM_FILE_TYPE_NORMAL;
    MMIDRMFILE_HANDLE       drm_handle = 0;
    DRM_PERMISSION_MODE_E   drm_permission = DRM_PERMISSION_NONE;
    BOOLEAN                 is_valid = FALSE;
#endif
    
    if((PNULL == contact_ptr) || (PNULL == applet_ptr))
    {
        //SCI_TRACE_LOW:"SetEntryDetails contact_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_7374_112_2_18_2_44_28_341,(uint8*)"d", contact_ptr);
        return;
    }
    else
    {
        detail_data.contact_ptr = contact_ptr;
    }
    if(applet_ptr->instance.function_type == MMIPB_OPEN_VCARD)
    {
        left_softkey_id = STXT_SAVE;
    }
    else if(MMIPB_IsContactHasMenu(detail_data.contact_ptr->storage_id))
    {
        left_softkey_id = STXT_OPTION;
    }
    
    custom_ring_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_RING_T));
    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));

    if(custom_ring_ptr == PNULL || custom_photo_ptr == PNULL
        )
    {
        if(custom_ring_ptr != PNULL)
        {
            SCI_FREE(custom_ring_ptr);
        }
        if(custom_photo_ptr != PNULL)
        {
            SCI_FREE(custom_photo_ptr);
        } 
        return;
    }
    GUILIST_SetMaxItem(
        ctrl_id, 
        MMIPB_LISTBOX_INDEX_MAX,
        FALSE
        );
    
    //icon && name 
                                                                         //nv contact
    {

        if((contact_ptr->photo_id_flag & MMIPB_PHOTO_ID_EXIST) && MMIPB_ReadPhotoFile(contact_ptr->contact_id, contact_ptr->storage_id, custom_photo_ptr))
        {
            if (MMIAPIFMM_IsFileExist(
            (const uint16 *)custom_photo_ptr->file_name,
            (uint16)(custom_photo_ptr->file_name_len)
            ))
            {
                icon_type = GUIITEM_DATA_ANIM_PATH;
                anim_path.full_path_wstr_ptr = custom_photo_ptr->file_name;
                anim_path.full_path_wstr_len = custom_photo_ptr->file_name_len;
            }
            
            //判断DRM文件，是否版权过期 @Bug 114162 
#ifdef DRM_SUPPORT
            file_type = MMIAPIFMM_GetFileTypeByFileName(custom_photo_ptr->file_name, custom_photo_ptr->file_name_len);
            SCI_TRACE_LOW("[MMIPB] SetEntryDetails file_type =%d", file_type);
            if(MMIFMM_FILE_TYPE_DRM == file_type)
            {
                drm_handle = SFS_CreateFile(custom_photo_ptr->file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
                if(SFS_INVALID_HANDLE != drm_handle)
                {
                    drm_permission = MMIAPIDRM_GetFilePermission(drm_handle, PNULL);    
                    is_valid = MMIAPIDRM_IsRightsValid(drm_handle, PNULL, TRUE, drm_permission);
                    if (TRUE != is_valid)
                    {
                        icon_type = GUIITEM_DATA_IMAGE_ID;
                    }
                }
                SCI_TRACE_LOW("[MMIPB] SetEntryDetails is_valid = %d", is_valid);
                SFS_CloseFile(drm_handle);
            }       
#endif            
        }
        else

        {
             icon_type = GUIITEM_DATA_IMAGE_ID;
        }

        str_t.wstr_len = contact_ptr->name.wstr_len;
        str_t.wstr_ptr = contact_ptr->name.wstr;
         MMIPB_GetAllGroupIDs(contact_ptr->union_t.group, &group_id, 1); 
#ifdef MMIPB_MOST_USED_SUPPORT
       if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
       {
            MMIPB_GetAllGroupIDs(search_group, &group_id, 1);
#ifdef MMI_GUI_STYLE_MINISCREEN
            middle_softkey_id = IMAGE_NULL;
#else
            middle_softkey_id =  TXT_NULL;
#endif
       }
#endif         
#ifdef MMIPB_SIMDN_SUPPORT
       if(MMIPB_IsMsisdnContact(detail_data.contact_ptr->storage_id))
       {
#ifdef GUI_MID_SOFTKEY_USE_IMG
           middle_softkey_id = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
           middle_softkey_id =  TXT_EDIT;
#endif
       }
	   if (MMIPB_IsSDNContact(detail_data.contact_ptr->storage_id))
	   {
#ifdef GUI_MID_SOFTKEY_USE_IMG
           middle_softkey_id = IMAGE_NULL;
#else
           middle_softkey_id =  TXT_NULL;
#endif
       }
#endif
#ifdef MMI_VCARD_SUPPORT
        if(applet_ptr->instance.function_type == MMIPB_OPEN_VCARD)
        {
#ifdef MMI_GUI_STYLE_MINISCREEN
            middle_softkey_id = IMAGE_NULL;
#else
            middle_softkey_id =  TXT_NULL;
#endif
        }
#endif
        AppendListItemDetail(ctrl_id,MMIPB_LISTBOX_INDEX_NAME, item_index,
                                GUIITEM_STYLE_TWO_LINE_ANIM_TEXT, 
                                left_softkey_id,middle_softkey_id,right_softkey_id, 
                                icon_type, GetBigIcon(group_id), &anim_path,
                                &str_t);   
        item_index++;
    }
	
    //Alias
#ifdef CMCC_VCARD_SUPPORT
        MMIPB_SetContacItem(ctrl_id, 
                            &detail_data,
                            MMIPB_LISTBOX_INDEX_ALIAS, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            FALSE); 
        item_index++;
#endif

    // 将号码放到item中
    for ( i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++ )
    {   
        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton, contact_ptr->number[i].number_len))
        { 
			/***bug 98042***/
#ifdef MMIPB_SIMDN_SUPPORT
            if(MMIPB_IsMsisdnContact(detail_data.contact_ptr->storage_id))
            {
#ifdef GUI_MID_SOFTKEY_USE_IMG
                middle_softkey_id = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
                middle_softkey_id =  TXT_EDIT;
#endif
            }
            else
#endif
            {
#ifdef GUI_MID_SOFTKEY_USE_IMG
                middle_softkey_id = IMAGE_COMMON_SOFTKEY_CALL_ICON;
#else
                middle_softkey_id = TXT_DIAL;
#endif
            }
#ifdef MMI_VCARD_SUPPORT
            if(applet_ptr->instance.function_type == MMIPB_OPEN_VCARD)
            {
#ifdef MMI_GUI_STYLE_MINISCREEN
                middle_softkey_id = IMAGE_NULL;
#else
                middle_softkey_id =  TXT_NULL;
#endif
            }
#endif
            MMIPB_SetContacItem(ctrl_id, 
                                &detail_data,
                                MMIPB_LISTBOX_INDEX_NUMBER_MOBILE + i, 
                                item_index,
                                left_softkey_id, 
                                middle_softkey_id, //TXT_CALL,
                                right_softkey_id,
                                FALSE); 

            middle_softkey_id = TXT_EDIT;
            item_index++;
        }
    }
#ifdef MMIPB_MAIL_SUPPORT
    // 将邮箱放到item中
    if(contact_ptr->mail.wstr_len)
    {    
#ifdef MMIEMAIL_SUPPORT
#ifdef GUI_MID_SOFTKEY_USE_IMG
            middle_softkey_id =IMAGE_COMMON_SOFTKEY_SEND_ICON;
#else
            middle_softkey_id = TXT_EMAIL;
#endif
#else
#ifdef MMS_SUPPORT
#ifdef MMI_GUI_STYLE_MINISCREEN
            middle_softkey_id = IMAGE_COMMON_SOFTKEY_SEND_ICON;
#else
            middle_softkey_id = TXT_MMS;
#endif
#else
#ifdef MMI_GUI_STYLE_MINISCREEN
            middle_softkey_id = IMAGE_NULL;
#else
            middle_softkey_id = TXT_NULL;
#endif
#endif
#endif            
            MMIPB_SetContacItem(ctrl_id, 
                                &detail_data,
                                MMIPB_LISTBOX_INDEX_MAIL, 
                                item_index,
                                left_softkey_id, middle_softkey_id,right_softkey_id,
                                FALSE); 

            middle_softkey_id = TXT_EDIT;

            item_index++;
    }
   
#endif
#ifdef GUI_MID_SOFTKEY_USE_IMG
        middle_softkey_id = IMAGE_COMMON_SOFTKEY_EDIT_ICON;
#else
        middle_softkey_id = TXT_EDIT;
#endif
//////////////////////////////////////////////////////set group//////////////////////////////////////////////////
	if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
	{
            MMIPB_SetContacItem(ctrl_id, 
                                &detail_data,
                                MMIPB_LISTBOX_INDEX_GROUP, 
                                item_index,
                                left_softkey_id, middle_softkey_id,right_softkey_id,
                                FALSE); 
            item_index++; 
	}

    // 将自定义铃声放到item中
    if(contact_ptr->ring_id_flag & MMIPB_RING_ID_EXIST)
    {
        if(contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG)
        {
            if( (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_ID_MASK)  <= MMISET_RING_FIXED_DEFAUT_NUM )
            {                
                MMIPB_SetContacItem(ctrl_id, 
                                    &detail_data,
                                    MMIPB_LISTBOX_INDEX_RING, 
                                    item_index,
                                    left_softkey_id, middle_softkey_id,right_softkey_id,
                                    FALSE); 
                item_index++;
            }     
        }
        else if(MMIPB_ReadRingFile(contact_ptr->contact_id, contact_ptr->storage_id, custom_ring_ptr))
        {
            if (MMIAPIFMM_IsFileExist(
                custom_ring_ptr->file_name,
                (uint16)(custom_ring_ptr->file_name_len)
                ))
            {
                detail_data.ring_ptr = custom_ring_ptr;
                MMIPB_SetContacItem(ctrl_id, 
                                    &detail_data,
                                    MMIPB_LISTBOX_INDEX_RING, 
                                    item_index,
                                    left_softkey_id, middle_softkey_id,right_softkey_id,
                                    FALSE); 
                item_index++;       
            }
        }
    
    }

#ifdef MMIPB_IM_MESSAGE_SUPPORT
    ///////////////////////////////////////////////////SET QQ///////////////////////////////////////////
    if(contact_ptr->imessager.wstr_len>0)
    {
        MMIPB_SetContacItem(ctrl_id, 
                            &detail_data,
                            MMIPB_LISTBOX_INDEX_QQ, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            FALSE);  
        item_index++; 
    }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    ///////////////////////////////////////////////////SET URL///////////////////////////////////////////
     if(contact_ptr->url.wstr_len)
     {
#ifdef BROWSER_SUPPORT
        middle_softkey_id = TXT_CMSBRW_LINK;
#endif
        MMIPB_SetContacItem(ctrl_id, 
                            &detail_data,
                            MMIPB_LISTBOX_INDEX_URL, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            FALSE);  
        middle_softkey_id = TXT_EDIT;

        item_index++; 
     }

/////////////////////////////////////////////////////set birth/////////////////////////////////////////    
    if(contact_ptr->birth.wstr_len)
    {
        MMIPB_SetContacItem(ctrl_id, 
                            &detail_data,
                            MMIPB_LISTBOX_INDEX_BIRTH, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            FALSE); 
        item_index++; 
    }


////////////////////////////////////////////////////////set 备忘录//////////////////////////////////////////////
     if(contact_ptr->memo.wstr_len)
    {  
        MMIPB_SetContacItem(ctrl_id, 
                            &detail_data,
                            MMIPB_LISTBOX_INDEX_MEMO, 
                            item_index,
                            left_softkey_id, middle_softkey_id,right_softkey_id,
                            FALSE); 
        item_index++;
    }
#endif

    SCI_FREE(custom_ring_ptr);
    SCI_FREE(custom_photo_ptr);

}


/*****************************************************************************/
//  Description : PB详情的窗口处理函数
//  Global resource dependence : none
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryDetailWinMsg
(
 MMI_WIN_ID_T       win_id, 
 MMI_MESSAGE_ID_E    msg_id, 
 DPARAM              param
 )
{
    MMI_RESULT_E   result                    = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id                  = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LISTBOX_CTRL_ID);
               

    uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};
    uint8 tele_len = 0;
    uint8 index = 0;
#ifndef MMI_MULTI_GREEN_KEY_SINGLE
    CC_CALL_SIM_TYPE_E   sim_type            = CC_MAJOR_SIM_CALL;
#endif
    uint16 search_entry_id = 0;
    uint16 search_storage_id = 0;
    uint16 search_group = 0;

    MMIPB_CALL_TYPE_E interface_type = MMIPB_CALL_TYPE_MAKECALL_AUDIO;
    //uint16       real_len                    = 0;
    //PHONEBOOK_STORAGE_E storage = PHONEBOOK_NV_STORAGE;
    uint32 item_id = 0;
#if defined(MMIPB_MOST_USED_SUPPORT)||defined(MMIPB_MAIL_SUPPORT)
    MMI_STRING_T string_to = {0};
#endif
    MMIPB_CONTACT_T *                  contact_ptr = PNULL;
    MMIPB_APPLET_T*            applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_STRING_T	call_name = {0};
#ifdef MMI_VCARD_SUPPORT
    MMIPB_ERROR_E   ret = MMIPB_ERROR_ERROR;
#endif
    
    if(PNULL == applet_ptr)
    {
        return result;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        MMK_SetAtvCtrl(win_id, ctrl_id); 
        contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);
        if(contact_ptr == PNULL)
        {
            break;
        }
        applet_ptr->instance.cur_entry_id = contact_ptr->contact_id;
        applet_ptr->instance.cur_storage_id = contact_ptr->storage_id;
#ifdef MMIPB_MOST_USED_SUPPORT
        if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
        {
            string_to.wstr_ptr = contact_ptr->name.wstr;
            if(MMIPB_GetNameByNumber(&(contact_ptr->number[0]), &string_to, MMIPB_NAME_MAX_LEN, &search_entry_id,&search_storage_id,&search_group,FALSE, FALSE))/*lint !e64*/
            {
            	    contact_ptr->name.wstr_len = string_to.wstr_len;
            }
            else
            {
            	    SCI_MEMSET(&contact_ptr->name, 0x00, sizeof(contact_ptr->name));
            }
        }
#endif
        SetEntryDetails(win_id, ctrl_id, contact_ptr, search_entry_id,search_storage_id, search_group);
        break;

    case MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG:
        contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);
        if(contact_ptr == PNULL)
        {
            break;
        }

        if(GUILIST_GetTotalItemNum(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID)) > 0 )
        {
    
            GUILIST_RemoveAllItems(MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_DETAIL_LIST_CTRL_ID));
        }
         MMIPB_GetContactInfo(contact_ptr->contact_id, contact_ptr->storage_id, contact_ptr);

        SetEntryDetails(win_id, ctrl_id, contact_ptr, search_entry_id, search_storage_id, search_group);
        MMK_UpdateScreen();

        break;
        
                  

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
		contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);
		if(contact_ptr == PNULL)
		{
			break;
		}
        AppendDetailDyncPhoto(ctrl_id,0,contact_ptr,param);
        break;
        
    case MSG_KEYDOWN_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);
        if(contact_ptr == PNULL)
        {
            break;
        }
#ifdef MMI_VCARD_SUPPORT        
        if(MMIPB_OPEN_VCARD == applet_ptr->instance.function_type)
        {
            //vcard详情界面不允许编辑
            break;
        }
#endif
#if defined (MMIPB_SIMDN_SUPPORT)   
        if(MMIPB_IsMsisdnContact(contact_ptr->storage_id))
        {
            EditContact(applet_ptr,contact_ptr);
            break;
        }
#endif        
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &item_id);

        if(MMIPB_LISTBOX_INDEX_NUMBER_MOBILE == item_id
                || MMIPB_LISTBOX_INDEX_NUMBER_HOME == item_id          
                || MMIPB_LISTBOX_INDEX_NUMBER_OFFICE == item_id
                || MMIPB_LISTBOX_INDEX_NUMBER_FAX == item_id
                || MMIPB_LISTBOX_INDEX_NUMBER_OTHER == item_id      
                )
        {
            uint32 number_index = MIN(item_id - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE, (MMIPB_MAX_NV_PHONE_NUM -1));                
            MN_NUMBER_TYPE_E ton = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[number_index].npi_ton);    

            tele_len = MMIAPICOM_GenDispNumber(
                                    (ton == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : ton),
                                    contact_ptr->number[number_index].number_len, 
                                    contact_ptr->number[number_index].number,
                                    tele_num,
                                    MMIPB_NUMBER_MAX_STRING_LEN+2);
            if(0 != tele_len)
            {
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                dual_sys = MMI_DUAL_SYS_MAX;
#endif
				call_name.wstr_ptr = contact_ptr->name.wstr;
				call_name.wstr_len = contact_ptr->name.wstr_len;
#ifdef MMIPB_SIMDN_SUPPORT
				if(MMIPB_IsSDNContact(contact_ptr->storage_id))
				{
					dual_sys = MMIPB_GetDualSysByStorageId(contact_ptr->storage_id);
				}
#endif
                MMIPB_CallNumber(dual_sys, MMIPB_CALL_TYPE_MAKECALL_AUDIO,tele_num,&call_name);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CL_EMPTY_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);    
            }
        }
#ifdef MMIPB_MAIL_SUPPORT
        else if(MMIPB_LISTBOX_INDEX_MAIL == item_id)//邮件功能
        {        
            g_is_email_by_pb = TRUE;
            if(contact_ptr->mail.wstr_len > 0)
            { 
                MMI_STRING_T temp_email = {0};
                const wchar *quotation ="\"";
                const wchar *bracket1 ="<";
                const wchar *bracket2 =">";
                const wchar *semicolon =";";
                if(PNULL !=contact_ptr->name.wstr && 0 !=contact_ptr->name.wstr_len)
                {
                    temp_email.wstr_ptr = MMIPB_MALLOC(sizeof(wchar)*(1+contact_ptr->name.wstr_len+2+contact_ptr->mail.wstr_len+1+2));
                    temp_email.wstr_len = 1+contact_ptr->name.wstr_len+2+contact_ptr->mail.wstr_len+1;// 格式"xx"<xx>;

                    MMIAPICOM_Wstrncat(temp_email.wstr_ptr,quotation,1);
                    MMIAPICOM_Wstrncat(temp_email.wstr_ptr, contact_ptr->name.wstr, sizeof(wchar)*contact_ptr->name.wstr_len);
                    MMIAPICOM_Wstrncat(temp_email.wstr_ptr,quotation,1);
                    MMIAPICOM_Wstrncat(temp_email.wstr_ptr,bracket1,1);
                    MMIAPICOM_Wstrncat(temp_email.wstr_ptr,contact_ptr->mail.wstr,contact_ptr->mail.wstr_len);
                    MMIAPICOM_Wstrncat(temp_email.wstr_ptr,bracket2,1);
                    MMIAPICOM_Wstrncat(temp_email.wstr_ptr,semicolon,1);

                    string_to.wstr_ptr = temp_email.wstr_ptr;
                    string_to.wstr_len = temp_email.wstr_len; 
                }
                else
                {
                    string_to.wstr_ptr = contact_ptr->mail.wstr;
                    string_to.wstr_len = contact_ptr->mail.wstr_len;
                }

#ifdef MMIEMAIL_SUPPORT
                //send email
                MMIAPIEMAIL_EditEmail(&string_to);
#else
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
                MMIAPIMMS_AnswerMMS(MMI_DUAL_SYS_MAX,PNULL,&string_to);
#endif
                if(PNULL !=temp_email.wstr_ptr)
                {
                    MMIPB_MFREE(temp_email.wstr_ptr);
                    temp_email.wstr_ptr = PNULL;
                    temp_email.wstr_len =0;
                }
            }
        }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
        else if( MMIPB_LISTBOX_INDEX_URL  == item_id)//url
        {
#ifdef BROWSER_SUPPORT
            if(contact_ptr->url.wstr_len > 0)
            {
                uint8   curl_str[MMIPB_URL_MAX_LEN + 1] = {0};
                MMIBROWSER_ENTRY_PARAM entry_param = {0};
                MMI_WSTRNTOSTR(curl_str,
                    MMIPB_URL_MAX_LEN,
                    contact_ptr->url.wstr,
                    MMIPB_URL_MAX_LEN,
                    contact_ptr->url.wstr_len
                    );
                entry_param.type = MMIBROWSER_ACCESS_URL;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)curl_str;
                entry_param.user_agent_ptr = PNULL;			
                MMIAPIBROWSER_Entry(&entry_param);
            }
#endif
        }
#endif	
        else if(MMIPB_MOSTUSED_CONTACTS != applet_ptr->instance.contacts_type
#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
        &&(!MMIPB_IsFixedContact(contact_ptr->storage_id))
#endif
        )
        {
 #ifdef FDN_SUPPORT
            if(MMIPB_IsFdnContact(contact_ptr->storage_id))
            {
                //PIN2 verify                
                MMIAPIPHONE_SetCurHandlingPinSimID(MMIPB_GetDualSysByStorageId(contact_ptr->storage_id));
                MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_VERIFY,
                                                                MNSIM_PIN2,
                                                                (uint32)MMIPB_GetDailNumberListWinTab(),
                                                                TRUE); 
                applet_ptr->instance.cmd = MMIPB_OPC_EDIT;
            }
            else
   #endif                
            {
                //edit 
                EditContact(applet_ptr,contact_ptr);
            }
        }
        result = MMI_RESULT_TRUE;
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
       /* if(MMIPB_OPEN_FAVOURITE_VIEW == applet_ptr->instance.function_type)
        {
            break;
        }*/
        contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);
        if(contact_ptr == PNULL)
        {
            break;
        }
#ifdef MMI_VCARD_SUPPORT
        if(applet_ptr->instance.function_type == MMIPB_OPEN_VCARD)
        {
            uint16 to_storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
		    ret = MMIPB_SaveVcard(to_storage_id, contact_ptr); 
            SCI_TRACE_LOW("[MMIPB] MMIPB_SaveVcard ret %d", ret);
			if(MMIPB_ERROR_PENDING != ret)
			{
           		MMIPB_OpenAlertTipWin(MMK_GetAppletHandleByWin(win_id), ret, MMIPB_OPC_ADD);  
			}
		    
            MMK_CloseWin(win_id);
        }
        else 
#endif
        if(MMIPB_IsContactHasMenu(contact_ptr->storage_id))
        {

            //Option//  delay_modify
            MMIPB_MENU_DATA_INFOR_T add_data = {0};

            index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &item_id);
            add_data.number_index = item_id - MMIPB_LISTBOX_INDEX_NUMBER_MOBILE;
            add_data.applet_handle = applet_ptr->caf_applet.app_handle;
            add_data.parent_win =MMK_GetWinHandle(add_data.applet_handle, MMIPB_ENTRY_DETAILS_WIN_ID);
            add_data.menu_type = MMIPB_DETAIL_BASIC_OPT_MENU;
#ifdef FDN_SUPPORT            
            if(MMIPB_IsFdnContact(contact_ptr->storage_id))
            {
                add_data.is_need_pin_verify = TRUE;
            }
            else
#endif                
            {
                add_data.is_need_pin_verify = FALSE;
            }
            add_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
            add_data.dual_sys = MMIPB_GetDualSysByStorageId(contact_ptr->storage_id);
            
            add_data.contact_ptr = contact_ptr;
            add_data.need_reset_storage = FALSE;
            add_data.valid_contact_count =1;
            MMIPB_EnterPbTabOptMenu(&add_data);		
        }

        
        break;       
    case MSG_KEYUP_GREEN:
#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
#endif        
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
		contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);
		if(contact_ptr == PNULL)
		{
			break;
		}
        
#ifdef VT_SUPPORT
        if(MSG_KEYLONG_GREEN == msg_id)
        {
            interface_type = MMIPB_CALL_TYPE_MAKECALL_VIDEO;
        }
#endif        

#ifndef MMI_MULTI_GREEN_KEY_SINGLE
        sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX);
#endif

        index = MMIPB_GetFirstValidNumberIndex(contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM);
        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[index].npi_ton, contact_ptr->number[index].number_len))
        {
            uint8 num_str[MMIPB_MAX_STRING_LEN +2] = {0};
			MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;

            call_name.wstr_ptr = contact_ptr->name.wstr;
            call_name.wstr_len = contact_ptr->name.wstr_len;

			if(index < MMIPB_MAX_NV_PHONE_NUM)
			{
				//MakeNumberCallByBCDNumber(MMI_DUAL_SYS_MAX, interface_type,PNULL,&contact_ptr->number[index], sim_type);
                MMIPB_BCDNumberToUTF8(&contact_ptr->number[index], MMIPB_MAX_STRING_LEN + 2, num_str);/*lint !e569*/
#ifdef MMIPB_SIMDN_SUPPORT
				if(MMIPB_IsSDNContact(contact_ptr->storage_id))
				{
					dual_sys = MMIPB_GetDualSysByStorageId(contact_ptr->storage_id);
				}
#endif

                MMIPB_CallNumber(dual_sys, //GetDualSysFromGreenKey(msg_id),   //modify for bug 80154
                                 interface_type,
                                 num_str,
                                 &call_name);
				break;
			}
		}
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CL_EMPTY_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL); 

        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
  
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#endif//no PDA

/*****************************************************************************/
//  Description : 根据要显示的联系人列表的类型，获得一条联系人要显示成多少条的个数
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetValidContactNum(
                                 MMIPB_LIST_FIELD_TYPE_E type, //IN
                                 MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T contact_valid_num_mail_info, //IN
                                 MMIPB_CONTACT_BASE_INFO_T contact_node, ////IN
                                 uint8* num_arry// //OUT : VALID NUM OR MAIL INDEX
                                 )
{
    uint16 num = 0;
    int i=0;
     
    if(type == MMIPB_LIST_FIELD_WHOLE)
    {
        num = 1;
    }
    else
    {
        if(type &MMIPB_LIST_FIELD_NUMBER)/*lint !e655*/
        {
            for(i=0; i<MMIPB_MAX_NV_PHONE_NUM; i++)
            {
                
                if (contact_node.num_type[i] != MMIPB_INVLID_NUMBER 
                    
                    && contact_valid_num_mail_info.visible_num_index[i] == 1
                    )
                    
                {                    
                    num_arry[num] = i;
                    num++;
                }
                
            }
        }
        if(type & MMIPB_LIST_FIELD_MAIL)/*lint !e655*/
        {
            if((contact_node.mail_exist) && contact_valid_num_mail_info.is_mail_visible)
            {
                num++;
            }
        }
    }
    return num;
}


/*****************************************************************************/
//  Description : 得到pb list的显示样式
//  Global resource dependence : g_mmipb_table_info
//  Author: MARY.XIAO
//  Note: 
/*****************************************************************************/
LOCAL GUIITEM_STYLE_E MMIPB_GetListStyle(MMIPB_INSTANCE_T *pb_instance_ptr)
{
    GUIITEM_STYLE_E  listitem_type = GUIITEM_STYLE_ONE_LINE_TEXT;

    if(pb_instance_ptr == PNULL)
    {
        return listitem_type;
    }
    if (MMIPB_LIST_NORMAL_MAIN == pb_instance_ptr->list_type)
    {
#ifdef MMI_GUI_STYLE_MINISCREEN
        listitem_type = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
#else
        listitem_type = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT;
#endif
    }
    else if( MMIPB_LIST_RADIO == pb_instance_ptr->list_type )
    {

        listitem_type = GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT;

    }
    else
    {

        listitem_type = GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT;

    } 
    return listitem_type;
}

/*****************************************************************************/
//  Description : 搜索新增pb contacts条目
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_SearchAndAppendItem(MMI_HANDLE_T list_handle, 
                                     MMI_HANDLE_T applet_handle, 
                                     uint8            group_id,
                                     uint16           storage_id,
                                     GUIITEM_STYLE_E  listitem_type,
                                     MMIPB_LIST_FIELD_TYPE_E field_type)
{
    int            i= 0 ;
    uint8          num_index[MMIPB_MAX_NV_PHONE_NUM]= {0};
    MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T    contact_valid_num_mail_info ={0};
    uint32         list_data = 0;
    MMIPB_CONTACT_BASE_INFO_T  contact_node = {0};
    MMIPB_HANDLE_T        handle = 0;

    uint16          contact_num = 0;
    uint16          contact_index = 0;
    MMIPB_APPLET_T*  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    BOOLEAN          is_select = FALSE;
#ifdef MMI_PDA_SUPPORT
    uint16          start_char = 0;
#endif
    wchar           upper_buf[2] = {0};
    uint16          list_index = 0;
   

    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_SearchAndAppendItem PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_7990_112_2_18_2_44_29_342,(uint8*)"");
        return;
    }
    applet_ptr->instance.split_num = 0;
    if(MMIPB_ERROR_SUCCESS != MMIPB_FindFirstContact((1<<group_id), storage_id, &contact_index, &contact_valid_num_mail_info,&contact_node, &handle))
    {
        //SCI_TRACE_LOW:"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_7996_112_2_18_2_44_29_343,(uint8*)"");
        return;
    }   
    do
    {
        contact_num = GetValidContactNum(field_type, contact_valid_num_mail_info, contact_node, num_index);
        if(contact_num>0)//有需要显示的contact
        {
#ifdef MMI_PDA_SUPPORT
            
            upper_buf[0] = contact_node.first_char;//第一个联系人先显示分隔条
            upper_buf[1] = 0;
            MMIAPICOM_Wstrupper(upper_buf);
            if(applet_ptr->instance.split_num == 0)//第一个联系人没显示， 所以显示分隔条
            {
                AppendSplitListItem(list_handle, upper_buf[0]);
                applet_ptr->instance.split_num++;
                list_index++;
                
            }
            else if(contact_node.first_char!= start_char)
            {
			    //MMIAPICOM_Wstrupper(upper_buf);
                AppendSplitListItem(list_handle, upper_buf[0]);
                applet_ptr->instance.split_num++;
                list_index++;
            }
            start_char = contact_node.first_char;
#endif
            if((field_type & MMIPB_LIST_FIELD_MAIL)/*lint !e655*/
                &&(contact_node.mail_exist) 
                && contact_valid_num_mail_info.is_mail_visible)
            {
                list_data = (MMIPB_MAX_NV_PHONE_NUM)<<16 | (contact_index);
#ifdef SNS_SUPPORT 
                if(MMIPB_IsSNSContact(storage_id))
                {
                    SetListItemForSearch(list_handle, listitem_type, list_data, upper_buf[0], FALSE);//显示联系人
                }
                else
#endif
                {
                    SetListItemForSearch(list_handle, listitem_type, list_data, upper_buf[0],   TRUE);//显示联系人
                }
                
	            is_select = MMIPB_FindSelNode(applet_ptr->instance.handle, contact_node.contact_id , contact_node.storage_id, num_index[i]);
	            if(is_select != GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(list_handle, list_index), GUIITEM_STATE_SELECTED))
	            {
                    GUILIST_SetSelectedItem(list_handle, list_index, is_select);
 	            }
                contact_num --;
                list_index++;
            }
            for(i=0; i<contact_num; i++)
            {
            //第一个16位表示num的index
            //第二个16位表示记录在表中的index 


                list_data = (num_index[i])<<16 | (contact_index);
#ifdef SNS_SUPPORT 
                if(MMIPB_IsSNSContact(storage_id))
                {
                    SetListItemForSearch(list_handle, listitem_type, list_data, upper_buf[0], FALSE);//显示联系人
                }
                else
#endif
                {
                    SetListItemForSearch(list_handle, listitem_type, list_data, upper_buf[0],   TRUE);//显示联系人
                }

	            is_select = MMIPB_FindSelNode(applet_ptr->instance.handle, contact_node.contact_id , contact_node.storage_id, num_index[i]);
	            if(is_select != GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(list_handle, list_index), GUIITEM_STATE_SELECTED))
	            {
                    GUILIST_SetSelectedItem(list_handle, list_index, is_select);
 	            }
                list_index++;

            }
            
            
        }
    }while(MMIPB_FindNextContact(handle, (1<<group_id), storage_id, &contact_index,&contact_valid_num_mail_info,&contact_node) == MMIPB_ERROR_SUCCESS);

    MMIPB_FindClose(handle);
  
}

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 搜索mostused contacts条目
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_MostUsedSearchAndAppendItem(MMI_HANDLE_T list_handle, MMI_HANDLE_T applet_handle, GUIITEM_STYLE_E  listitem_type)
{
   // int            i= 0 ;
    uint32         list_data = 0;
    MMIPB_MOSTUSED_CONTACT_NODE_T  contact_node = {0};
    MMIPB_HANDLE_T        handle = 0;
     //uint16          contact_num = 0;
     uint16          contact_index = 0;
     MMIPB_APPLET_T*  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
#ifdef MMI_PDA_SUPPORT
    // uint16          start_char = 0;
    // wchar           upper_buf[2] = {0};
#endif  
     //+ for coverity
     if(PNULL == applet_ptr)   
     {
         return;
     }

     applet_ptr->instance.split_num = 0;
    if(MMIPB_ERROR_SUCCESS != MMIPB_FindFirstMostUsedContact(&contact_index, &contact_node, &handle))
    {
        //SCI_TRACE_LOW:"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8073_112_2_18_2_44_30_344,(uint8*)"");
        return;
    }   
    do
    {
        //第一个16位表示num的index
        //第二个16位表示记录在表中的index 
        list_data = MMIPB_CONTACTINDEX(contact_index);
        SetListItemForSearch(list_handle, listitem_type, list_data, 0, TRUE);//显示联系人
            
    }while(MMIPB_FindNextMostContact(handle, &contact_index,&contact_node) == MMIPB_ERROR_SUCCESS);

    MMIPB_FindClose(handle);
  
}
#endif

/*****************************************************************************/
//  Description : 设置多选列表控件的item
//  Global resource dependence : g_mmipb_table_info
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetListItem //RETURN: 实际设置的个数
(
    MMI_WIN_ID_T       win_id,
    uint32             total_count,    //IN:
    uint16             storage_id, //IN:
    uint16             search_len,//IN:
    MMI_CTRL_ID_T      ctrl_id        //IN: 对应控件的ID
)
{
     GUIITEM_STYLE_E  listitem_type = GUIITEM_STYLE_ONE_LINE_TEXT;
     GUILIST_TYPE_E   listbox_type = GUILIST_TYPE_NONE;

     MMI_HANDLE_T    applet_handle = 0;
     MMIPB_APPLET_T* applet_ptr = PNULL;
     MMI_HANDLE_T    list_handle = 0;
     MMIPB_LIST_FIELD_TYPE_E filed_type = 0;
     //int16   i          = 0;
     //int16   index      = 0;
    // BOOLEAN  is_exist_number = FALSE; 
     BOOLEAN           is_select = FALSE;
    // uint16   number_index = 0;


    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"SetListItem PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8120_112_2_18_2_44_30_345,(uint8*)"");
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;

	 applet_ptr->instance.split_num = 0;	
     if(0 == total_count)
     {
#ifdef FDN_SUPPORT        
        MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByGroupId(applet_ptr->instance.cur_group_id);
        /*2Gfdn不可访问的处理*/
        if (MMIAPIFDN_Is2GFdnEnable(dual_sys))
        {
            //失败窗口
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_FDN_ACTIVATED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);     
        }
		else
#endif
		{
			//disable mask function
			if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
			{
				GUILIST_SetTextListMarkable(ctrl_id, FALSE);
			}
		}
        return;
     }


     GUILIST_SetMaxItem(ctrl_id, total_count, TRUE);
	
    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {
        applet_ptr->instance.max_select_count = total_count;
		GUILIST_SetTextListMarkable(ctrl_id, FALSE);
		GUILIST_SetMaxSelectedItem(ctrl_id, 0);
    }
	else
	{
		is_select = MMIPB_HasSelNode(applet_ptr->instance.handle);
		if(is_select && (MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type))
		{
			GUILIST_SetTextListMarkable(ctrl_id, TRUE);
		}		
		GUILIST_SetMaxSelectedItem(ctrl_id, total_count);		
	}
	

    if(MMIPB_LIST_NORMAL_MAIN== applet_ptr->instance.list_type)

    {
        listbox_type = GUILIST_TEXTLIST_E;
    }
    else if(MMIPB_LIST_RADIO == applet_ptr->instance.list_type)
    {
        listbox_type = GUILIST_RADIOLIST_E;
    }
    else if(MMIPB_LIST_CHECK== applet_ptr->instance.list_type)
    {
        listbox_type = GUILIST_CHECKLIST_E;
    }   
    GUILIST_ChangeDisplayType(
        ctrl_id,
        listbox_type,
        FALSE
        );

   listitem_type = MMIPB_GetListStyle(&applet_ptr->instance);

   list_handle = MMK_ConvertIdToHandle(ctrl_id);
   if(applet_ptr->instance.app_type < MMIPB_TAB_MAX_NUM)
   {
       filed_type = applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type];
   }
#ifdef BT_PBAP_SUPPORT
   //寻找插入蓝牙联系人记录
   if(PB_GROUP_BT == applet_ptr->instance.cur_group_id)
   {
       MMIPB_BtContactSearchAndAppendItem(list_handle, applet_handle, listitem_type);
       //MMIPB_BtContactSearchAndAppendItem(list_handle, applet_handle, applet_ptr->instance.cur_group_id, storage_id, listitem_type, filed_type);
   }
   else
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
   //寻找插入联系人记录
   if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
   {   
       MMIPB_MostUsedSearchAndAppendItem(list_handle, applet_handle,listitem_type);      
   }
   else
#endif
   {
       //普通联系人列表
       MMIPB_SearchAndAppendItem(list_handle, applet_handle, applet_ptr->instance.cur_group_id, storage_id, listitem_type, filed_type);
   }

} 

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : Registers the Phonebook Access Client
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS MMIPB_PbapClientRegister(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;

    ret_val = BT_PbapRegisterClient();
    SCI_TRACE_LOW("[MMIBT_PbapClientRegister] ret_val = 0x%x", ret_val);
    SCI_TRACE_LOW("BT_PbapRegisterClient ret_val:%d",ret_val);
    return ret_val;
#else
	return BT_SUCCESS;
#endif
}
/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_PbapClientDeregister(void)
{
#ifndef WIN32
	BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapDeregisterClient();

	SCI_TRACE_LOW("[MMIBT_PbapClientDeregister] ret_val = 0x%x", ret_val);
	if(BT_SUCCESS != ret_val)
	{
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
	}
	
#else
	return;
#endif
}

/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_PbapClientDeregister(void)
{
    MMIPB_PbapClientDeregister();
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinConnectWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMIPB_StopPBAPTimer();
	    MMIPB_PbapClientTpDisconnect();
	    MMK_CloseWin(win_id);
	    break;
    
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;  
}

/*****************************************************************************/
//  Description : MMIPB_CreatePBAPTimer
//  Global resource dependence : 
//  Author: 
//  Note: 创建超时定时器
/*****************************************************************************/
LOCAL void MMIPB_CreatePBAPTimer(void)
{ 
    SCI_TRACE_LOW("[MMIBT] MMIPB_CreatePBAPTimer enter");

	if(s_pbap_timeout_timer != 0) 
	{
		MMIPB_StopPBAPTimer();
	}
    
	s_pbap_timeout_timer = MMK_CreateTimerCallback(BT_PBAP_TIMER_OUT, MMIPB_ProcessPBAPTimerOver, 0, FALSE);
	
    SCI_TRACE_LOW("[MMIBT] MMIPB_CreatePBAPTimer exit, timer_id = %d", s_pbap_timeout_timer);
}

/******************************************************************************/
// Description: MMIPB_StopPBAPTimer
// Global resource dependence: 
// Author: 
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIPB_StopPBAPTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIPB_StopPBAPTimer enter");
    
    if(s_pbap_timeout_timer != 0)    
    {   
        MMK_StopTimer(s_pbap_timeout_timer);   
        s_pbap_timeout_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopPBAPTimer exit, timer_id = %d", s_pbap_timeout_timer);
}

/******************************************************************************/
// Description: MMIPB_ProcessPBAPTimerOver
// Global resource dependence: 
// Author: 
// Note: 超时保护机制
/******************************************************************************/
LOCAL void MMIPB_ProcessPBAPTimerOver(uint8 timer_id, uint32 param)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_ProcessPBAPTimerOver enter");

    MMIPB_StopPBAPTimer();
    MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
    MMIPUB_OpenAlertWarningWin(TXT_BT_TIMEOUT_ALERT);
    MMIPB_PbapClientTpDisconnect();
}

/*****************************************************************************/
//  Description : to initiate a transport disconnection. 
//  Global resource dependence :                                
//  Author: 
//  Note: BT_PbapDisconnect will cause the transport to be brought down automatically, 
//  but if the transport must be brought down immediately, perhaps due an error condition, 
//  this routine may be used. 
/*****************************************************************************/
LOCAL void MMIPB_PbapClientTpDisconnect(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapClientTpDisconnect();

    SCI_TRACE_LOW("[MMIBT_PbapClientTpDisconnect] ret_val = 0x%x", ret_val);
    if(BT_SUCCESS == ret_val)
    {
        MMIPB_PbapDisconnect();
    }
    else if(BT_PENDING != ret_val)
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : Create a transport connection to the specified device 
//                and issue an OBEX Connect Request
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BT_STATUS MMIPB_PbapConnect(const BT_ADDRESS *addr)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;

    ret_val = BT_PbapConnect(addr);
    SCI_TRACE_LOW("BT_PbapConnect ret_val:%d", ret_val);
    SCI_TRACE_LOW("[MMIPB_PbapConnect] ret_val = 0x%x", ret_val);
    
    return ret_val;
#else
	return BT_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_PbapDisconnect(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapDisconnect();

	SCI_TRACE_LOW("[MMIBT_PbapDisconnect] ret_val = 0x%x", ret_val);
	if(BT_SUCCESS == ret_val)
	{
		MMIPB_PbapClientDeregister();
	}
	else if((BT_PENDING != ret_val) && (BT_BUSY != ret_val))
	{
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
	}
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: 
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
LOCAL void MMIPB_PbapAbortClient(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    MMI_STRING_T                wait_text = {0};
    ret_val = BT_PbapAbortClient();

    SCI_TRACE_LOW("[MMIPB_PbapAbortClient] ret_val = 0x%x", ret_val);
    if(BT_SUCCESS == ret_val)
    {
        MMI_GetLabelTextByLang(TXT_ST_WAIT, &wait_text); 
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinWaitingMsg); 
        MMIPUB_SetWinSoftkey(MMIBT_WAIT_WIN_ID, TXT_NULL, TXT_NULL, TRUE);
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
// #ifndef BT_DIALER_DIRECT_CONNECT
//         MMIPB_PbapDisconnect();
// #endif
    }
    else if(BT_PBAP_TYPE_SIM != MMIBT_GetPbapRequestType())
    {
        MMIPUB_OpenAlertFailWin(TXT_BT_PULL_PHONEBOOK_ABORT_ERROR);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        break;
        
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinSyncWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIPB_PbapAbortClient();
        MMIAPIBT_SetPbapCancel(TRUE);
        MMIAPIPB_RefreshList();
	MMK_CloseWin(win_id);
        
        break;
     
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
// 	Description : Get win id for bt sync win id
// 	Global resource dependence : 
// Author:
// 	Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIPB_GetActivateBTWinId(void)
{
    MMI_HANDLE_T    applet_handle = {0};
    
    if(s_is_focus_bt_contacts)
    {
        return s_bt_tab_win_id;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : Refresh list after sync finished
// 	Global resource dependence : 
// Author:
// 	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_RefreshList(void)
{
    if(s_is_focus_bt_contacts)
    {
        MMK_SendMsg(s_bt_tab_win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE); 
    }
}

/*****************************************************************************/
// 	Description : set pbap allow sync
//	Global resource dependence: 
//  Author:allen.lee
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SetAllowSync(BOOLEAN is_allow_sync)
{
    s_is_sync_contacts = is_allow_sync;
    s_is_fullpaint_sync_once = is_allow_sync;
}

#endif

/*****************************************************************************/
//  Description : draw addation text in window title on right conner 
//  Global resource dependence : 
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
LOCAL void PbCreateChildWin(
                            MMI_HANDLE_T applet_handle,
                            MMI_WIN_ID_T    win_id,
							MMIPB_SUB_FUNCTION_TYPE_E  app_type
                            )
{
	MMI_STRING_T			str_text = {0};
	uint32                  tab_count = 0;
	uint32                  i = 0;
	uint32                  focus_item_index = 0;
	MMI_WIN_ID_T            focus_sub_win_id = 0;
    MMIPB_WINDOW_TAB_INFO_T *sub_tab_ptr = PNULL;
    MMIPB_APPLET_T*         applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] PbCreateChildWin PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8236_112_2_18_2_44_30_346,(uint8*)"");
        return;
    }

#ifdef MMI_PDA_SUPPORT 
    if(applet_ptr->instance.field_disp_type[0] == MMIPB_LIST_FIELD_MAIL)
     {
         sub_tab_ptr = s_mmipb_pda_email_select_win_tab;
        tab_count = ARR_SIZE(s_mmipb_pda_email_select_win_tab);    
     }
    else if(applet_ptr->instance.list_type == MMIPB_LIST_CHECK)
    {
        sub_tab_ptr = s_mmipb_pda_select_win_tab;
        tab_count = ARR_SIZE(s_mmipb_pda_select_win_tab);
    }
#if defined MMI_ISTYLE_SUPPORT	
	else if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle()) 
	{ 
		sub_tab_ptr = s_mmipb_istyle_win_tab;
		tab_count = ARR_SIZE(s_mmipb_istyle_win_tab);
	} 
#endif	
	else
	{
		sub_tab_ptr = s_mmipb_pda_win_tab;
		tab_count = ARR_SIZE(s_mmipb_pda_win_tab);
	}
#else 
	sub_tab_ptr = s_mmipb_normal_win_tab;
	tab_count = ARR_SIZE(s_mmipb_normal_win_tab);
#endif
	//set tab count
	GUITAB_SetMaxItemNum(MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID), tab_count);
	//create every sub tab
    for(i = 0; i < tab_count; i++)
	{
		//tab title
		MMI_GetLabelTextByLang(sub_tab_ptr[i].text_id, &str_text);
		GUITAB_AppendSimpleItem(
            MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID),
            &str_text,
            sub_tab_ptr[i].tab_on_img_id,
            sub_tab_ptr[i].tab_off_img_id);

	}
	//set focus sub window
	
	for(i = 0; i < tab_count; i++)
	{
		//create this sub window
		sub_tab_ptr[i].create_func(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID));
		//add this sub tab to main windows
        GUITAB_AddChildWindow(
            MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID),
            MMK_GetWinHandle(applet_handle, sub_tab_ptr[i].win_id), 
            i);
		if(app_type == sub_tab_ptr[i].app_type)
		{
			focus_item_index = i; 
			focus_sub_win_id =sub_tab_ptr[i].win_id;//;
		}
	}
#ifdef MAINLCD_SIZE_240X400
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle()) 
    {
        GUITAB_SetFontParam(MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID), SONG_FONT_16, MMI_WHITE_COLOR, 0); /* tab's theme is Not OK*/
    }
#endif
#endif    
    GUITAB_SetCurSel(MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_LIST_TAB_CTRL_ID), focus_item_index);  
    MMK_SetChildWinFocus(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMK_GetWinHandle(applet_handle, focus_sub_win_id));
}  
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC void MMIPB_WaitAndEnterPBList(MMI_HANDLE_T applet_handle)
{
    MMI_STRING_T    prompt_str = {0};
   // MMIPB_APPLET_T* applet_ptr = PNULL;

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    MMIPUB_OpenWaitWinEx(applet_handle, 1,&prompt_str,PNULL,PNULL,MMIPB_WAITING_ALERT_WIN_ID,IMAGE_NULL,
              ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE, HandleWaitAndEnterPBListWinMsg, PNULL);
}


/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitAndEnterPBListWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param)
{
    MMI_RESULT_E          result  = MMI_RESULT_TRUE;
    LOCAL  MMI_WIN_ID_T   s_timer_id = 0;
    BOOLEAN               is_key_locked = FALSE;
   // MMIPUB_INFO_T*        add_data  =  MMIPUB_GetWinAddDataPtr(win_id );
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    
    //MMIPB_PB_LIST_DATA_T*  list_win_data_ptr = (MMIPB_PB_LIST_DATA_T*)(add_data->user_data);//list_win_data_ptr数据是malloc的，需要free

    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)  //+ for coverity
    {
        return MMI_RESULT_FALSE;
    }
    applet_handle = applet_ptr->caf_applet.app_handle; 
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
       if ((!MMIPB_IsPbReady()))
       {
           if (s_timer_id != 0)
            {
                MMK_StopTimer(s_timer_id);
                s_timer_id = 0;
            }
           s_timer_id = MMK_CreateWinTimer(win_id, MMIPB_VIBRA_DEFAULT_TIME,TRUE);
           result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
       }
       else
       {
            //pb already ready,open list window
            //SCI_TRACE_LOW:"[MMIPB]HandleWaitAndEnterPBListWinMsg MSG_OPEN_WINDOW create pb list"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8352_112_2_18_2_44_30_347,(uint8*)"");

            MMK_CreateWinByApplet(applet_handle,MMIPB_MAIN_WIN_TAB, PNULL); 
            PbCreateChildWin(applet_handle,MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMIPB_ALL_CONTACT);            
            MMK_CloseWin(win_id);

       }
       break;
    case MSG_TIMER:  
        //SCI_TRACE_LOW:"[MMIPB]HandleWaitAndEnterPBListWinMsg timeout"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8361_112_2_18_2_44_30_348,(uint8*)"");
        if(MMK_IsFocusWin(win_id) 
#ifdef SCREENSAVER_SUPPORT
            || win_id == MMK_GetPrevWinId(MMIIDLE_SCREENSAVER_WIN_ID) 
#endif
            || win_id == MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID)
            )
        {
            //current not other app occour
            if (s_timer_id == *((uint8 *) param))
            {              
                //create pb list window
                if (MMIPB_IsPbReady())
                {
#ifdef MMI_KEY_LOCK_SUPPORT
                    is_key_locked = MMIAPIKL_IsPhoneLocked();
#endif
                    //SCI_TRACE_LOW:"[MMIPB]HandleWaitAndEnterPBListWinMsg create pb list is_key_locked %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8376_112_2_18_2_44_30_349,(uint8*)"d", is_key_locked);
					
                    MMK_CreateWinByApplet(applet_handle,MMIPB_MAIN_WIN_TAB, PNULL); 
                    PbCreateChildWin(applet_handle,MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMIPB_ALL_CONTACT);   
                    MMK_CloseWin(win_id);
#ifdef MMI_KEY_LOCK_SUPPORT
                    if(is_key_locked)
                    {
                        MMIAPIKL_LockPhone();
                    }
#endif
                }
            }
        }

        break;
    case MSG_APP_RED:   
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    //case MSG_LOSE_FOCUS:
      //SCI_TRACE_LOW:"[MMIPB]HandleWaitAndEnterPBListWinMsg user exit"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8394_112_2_18_2_44_30_350,(uint8*)"");
      if(0 != s_timer_id)
      {
          MMK_StopTimer(s_timer_id);
          s_timer_id = 0;
      }

       MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:  // 不响应翻盖
           MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

     case MSG_CLOSE_WINDOW:
         if(0 != s_timer_id)
         {
             MMK_StopTimer(s_timer_id); 
             s_timer_id = 0;
         }
         result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
          break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

#if 0
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UpdateCurrentListPage(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
	uint16 item_index = 0;
	uint16 item_num = 0;
	uint16 top_index = 0;
	uint16 i = 0;
	GUILIST_ITEM_T *item_ptr = NULL;
		//replace current items
	top_index = GUILIST_GetTopItemIndex(ctrl_id);
	item_num = GUILIST_GetPageItemNum(ctrl_id);

	for(i = 0; i < item_num; i++)
	{
		item_index = top_index + i;
		item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, item_index);   /*lint !e605*/             
		GUILIST_ReplaceItem(ctrl_id, item_ptr, item_index);
	}
	MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}
#endif

/*****************************************************************************/
//  Description : Get  Valid Number count
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetValidNumberCount(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count)
{
     uint8 loop_count = MIN(MMIPB_MAX_NV_PHONE_NUM, max_count);
     uint8  i = 0;
     uint8  valid_count = 0; 

     for(i = 0; i < loop_count; i++)
     {
         if(MMIPB_IsValidPhoneNumber(number_t_ptr[i].npi_ton, number_t_ptr[i].number_len))
         {
             valid_count++;
         }
     }
     return valid_count;
}

/*****************************************************************************/
//  Description : 处理toolbar 点击后的操作
//  Global resource dependence : none
//  Author: maryxiao
//  Note:此接口不止toolbar会调用，list slide功能也会调用
/*****************************************************************************/
LOCAL void HandleToolBarAction(MMI_WIN_ID_T win_id, 
                               MMI_HANDLE_T list_handle,
                               PB_OPTMENU_NODE_E node_id,
                               MMIPB_CONTACT_T*  detail_contact_ptr,
                               BOOLEAN is_use_first)
{
    MMIPB_MENU_DATA_INFOR_T menu_infor = {0};
    
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
  //  CC_CALL_SIM_TYPE_E   sim_type            = CC_MAJOR_SIM_CALL;	
    MMIPB_CONTACT_T *contact_ptr = PNULL;  
    uint8       valid_num = 0;
    uint8       num_index = 0;
   // MMI_STRING_T     name_str={0}; 

     
    if(PNULL == applet_ptr)
    {
        return;
    }

    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] HandleToolBarAction PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8502_112_2_18_2_44_31_351,(uint8*)"");
        return;
    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    if(PNULL == detail_contact_ptr)
    {
    
        MMIPB_GetContactInfoFromList(win_id, list_handle, contact_ptr);
    }
    else
    {
        SCI_MEMCPY(contact_ptr, detail_contact_ptr, sizeof(MMIPB_CONTACT_T));
    }
#ifdef SNS_SUPPORT
    if(MMIPB_IsSNSContact(contact_ptr->storage_id))
    {
        return;
    }
#endif
    valid_num = GetValidNumberCount((MMIPB_BCD_NUMBER_T *)&contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM);

    if((0==valid_num) && (node_id != PB_OPTMENU_NODE_SEND_EMAIL))
    {
        
        MMIPUB_OpenAlertWinByTextIdEx(applet_ptr->caf_applet.app_handle, 
            PNULL,
            TXT_CL_EMPTY_NUM,
            TXT_NULL,
            IMAGE_PUBWIN_WARNING,
            PNULL,
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL,
            PNULL);
        
    }
#ifdef MMIPB_MAIL_SUPPORT
    else if((node_id == PB_OPTMENU_NODE_SEND_EMAIL) && (0==contact_ptr->mail.wstr_len))
    {
        MMIPUB_OpenAlertWinByTextIdEx(applet_ptr->caf_applet.app_handle, 
            PNULL,
            TXT_CL_EMPTY_NUM,
            TXT_NULL,
            IMAGE_PUBWIN_WARNING,
            PNULL,
            PNULL,
            MMIPUB_SOFTKEY_ONE,
            PNULL,
            PNULL);        
    }
#endif
    else
    {
        menu_infor.contact_ptr = contact_ptr;
        menu_infor.applet_handle = applet_ptr->caf_applet.app_handle;
        switch(node_id)
        {

        case PB_OPTMENU_NODE_DIAL_AUDIO:
            menu_infor.operate_type = MMIPB_OPERATE_TYPE_AUDIO_DIAL;
            break;
#ifdef VT_SUPPORT
        case PB_OPTMENU_NODE_DIAL_VIDEO:
            menu_infor.operate_type =MMIPB_OPERATE_TYPE_VIDEO_DIAL;
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case PB_OPTMENU_NODE_DIAL_IP:
            menu_infor.operate_type = MMIPB_OPERATE_TYPE_IP_DIAL;
            break;
#endif
        case PB_OPTMENU_NODE_SEND_SMS:
            menu_infor.operate_type = MMIPB_OPERATE_TYPE_WRITE_MESSAGE;
            break;
           
        case PB_OPTMENU_NODE_SEND_MMS:
            menu_infor.operate_type = MMIPB_OPERATE_TYPE_WRITE_MMS;
            break;

#ifdef MMIPB_MAIL_SUPPORT            
        case PB_OPTMENU_NODE_SEND_EMAIL:
            {
                 MMI_STRING_T send_addr = {0};
                 send_addr.wstr_ptr = contact_ptr->mail.wstr;
                 send_addr.wstr_len = contact_ptr->mail.wstr_len;
                 MMIAPIEMAIL_EditEmail(&send_addr);
            }
            break;
#endif
        case PB_OPTMENU_NODE_ADDNEW:
            {
                MMIPB_MENU_DATA_INFOR_T menu_data={0};	
                menu_data.applet_handle = applet_ptr->caf_applet.app_handle;
                menu_data.operate_type = MMIPB_OPERATE_TYPE_ADD_ONE;
                MMIPB_AddOrEditContactOfPb(&menu_data);
            }
            
            break;
        default:
            break;
        }
        if((node_id != PB_OPTMENU_NODE_SEND_EMAIL) &&(node_id != PB_OPTMENU_NODE_ADDNEW))
        {
        
            num_index = MMIPB_GetFirstValidNumberIndex(contact_ptr->number,MMIPB_MAX_NV_PHONE_NUM); 
            if(is_use_first && num_index < MMIPB_MAX_NV_PHONE_NUM)
            {
                MMIPB_HandleOneNumberAction(&menu_infor, num_index);
            }
            else
            {
                MMIPB_HandleNumberSelect(&menu_infor);
            }
        }
    }
    SCI_FREE(contact_ptr);
}



#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
LOCAL void PB_ValidatePrivacyEntryList(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMI_WIN_ID_T win_id = 0;
    MMIPB_APPLET_T*  applet_ptr=   PNULL;

    if (PNULL != param_ptr)
    {
        win_id = *(MMI_WIN_ID_T*)param_ptr;
    }
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)  //+ for coverity
    {
        return;
    }
    
	if ( is_permitted)
	{
		//密码验证成功
		applet_ptr->instance.app_type = GetCurAppType(applet_ptr->caf_applet.app_handle, GUITAB_GetCurSel(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_ptr->caf_applet.app_handle, MMIPB_MAIN_WIN_ID), MMIPB_SEARCH_LIST_TAB_CTRL_ID)));
		MMIPB_SetProtectStatus(TRUE);
		MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
	}
	else
	{
		//密码验证失败
		if(applet_ptr->instance.app_type >= MMIPB_DIAL_APPLICATION)
		{
           //从其他tab窗口进入pb的，返回前面一个tab应用
			SwitchToAppTab(applet_ptr->caf_applet.app_handle, applet_ptr->instance.app_type);
		}
		else
		{
			//直接进入pb的，关闭整个tab窗口
			MMK_CloseAllWinByApplet(applet_ptr->caf_applet.app_handle);
		}
	}  
}
#endif

/*****************************************************************************/
//  Description :  处理check消息， 加入选择列表
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void HandleMarkItem(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint8 group_id,void* param)
{

    uint16 entry_id = 0;
    uint16 index    = 0;
    uint16 storage_id = 0;
    uint16 num_index = 0;
    uint16 cur_index = 0;
    GUILIST_ITEM_T* item_ptr = PNULL;
	GUILIST_MARK_ITEM_T *list_param_ptr = (GUILIST_MARK_ITEM_T*)param;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

	if(list_param_ptr == PNULL)
	{
		return;
	}
    if(PNULL == applet_ptr)  //+ for coverity
    {
        return;
    }
    
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);/*lint !e605*/
    if(item_ptr == PNULL)
    {
        return;
    }
    index = MMIPB_CONTACTINDEX(item_ptr->user_data);
    num_index = MMIPB_NUMINDEX(item_ptr->user_data);
 #ifdef MMIPB_MOST_USED_SUPPORT
    if(group_id == PB_GROUP_MOSTUSED)
    {
        MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node_info= {0};
        mostused_contact_node_info = MMIPB_GetMostUsedContactBaseInfo(index);
        entry_id = mostused_contact_node_info.entry_id;
        storage_id =   mostused_contact_node_info.storage_id;
    }
    else
#endif
    {
        MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};
        contact_node_info = MMIPB_GetContactBaseInfo(index);
        entry_id = contact_node_info.contact_id;
        storage_id = contact_node_info.storage_id;
    }
    if(list_param_ptr->had_maked)//添加到选择列表中
    {
        MMIPB_SEL_DATA sel_data = {0};
        sel_data.entry_id = entry_id;
        sel_data.storage_id = storage_id;
        sel_data.num_index = num_index;
        if(MMIPB_LIST_FIELD_WHOLE == applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])
        {
            sel_data.sel_field = MMIPB_FEILD_FLAG_MASK_ALL;
        }
        else 
        {
            sel_data.sel_field =  MMIPB_FEILD_MASK_SINGLE_NUMBER << num_index;
        }
        MMIPB_CreateSelNode(applet_ptr->instance.handle,&sel_data);
    }
    else//从选择列表中删除
    {
        MMIPB_DeleteSelNode(applet_ptr->instance.handle,entry_id, storage_id,num_index);
        if(applet_ptr->instance.select_list != PNULL)
        {
             if(0 == applet_ptr->instance.select_list->sel_count)
             {   
                 MMIPB_MFREE(applet_ptr->instance.select_list);
                 applet_ptr->instance.select_list = PNULL;
             }
        }
    }


}




#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : TabList列表的列表刷新消息处理函数
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void HandlePdaMainReloadMsg(MMI_WIN_ID_T win_id, 
                                  MMI_HANDLE_T ctrl_id, 
                                  MMIPB_CONTACTS_TYP_E contacts_type,
                                  uint16 group_id)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    uint16          cur_index = 0;
    uint16          storage_id = 0;
    MMI_STRING_T     search_str = {0};
    uint32          total_count = 0;

    GUILIST_EMPTY_INFO_T                list_emty_info={0};
    MMI_STRING_T                        emtry_str_t={0};

    MMIPB_CONTACT_LIST_INFO_T   contact_list_info = {0};
    MMIPB_SUB_FUNCTION_TYPE_E   app_type = MMIPB_DIAL_APPLICATION;
    if(PNULL == applet_ptr)
    {
        return;
    }
    
    //update current list 
	cur_index = GUILIST_GetCurItemIndex(ctrl_id);
	//first remove all list items
	if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
	{
        if(MMIPB_LIST_CHECK == applet_ptr->instance.list_type)
        {
            AddCheckItemToOperateList(win_id, ctrl_id, FALSE);
        }
		GUILIST_RemoveAllItems(ctrl_id);
	}
    //MMK_SetAtvCtrl(win_id, ctrl_id);
    app_type = GetCurAppType(applet_ptr->caf_applet.app_handle, GUITAB_GetCurSel(MMK_GetCtrlHandleByWin(MMK_GetWinHandle(applet_ptr->caf_applet.app_handle, MMIPB_MAIN_WIN_ID), MMIPB_SEARCH_LIST_TAB_CTRL_ID)));
	//隐私检查
	if(!MMIPB_GetProtectStatus() && app_type<MMIPB_DIAL_APPLICATION)
	{
		//需要进行隐私检查
		
		if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB))
		{
          //  MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_PB, MMIPB_DoOperationWithoutProtect);
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_PB, PB_ValidatePrivacyEntryList, (void*)&win_id, sizeof(win_id));
			return;
		}
	}   

    if(MMIPB_ALL_GROUP_CONTACTS == contacts_type)
    {
        GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID),&search_str);
        SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
        SCI_MEMCPY(applet_ptr->instance.search_str, search_str.wstr_ptr, search_str.wstr_len*sizeof(wchar));
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    else if(MMIPB_MOSTUSED_CONTACTS == contacts_type)//设置常用联系人页面
    {
        SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
    }
#endif
    else if(MMIPB_SINGLE_GROUP_CONTACTS == contacts_type)//single group
    {
        GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID),&search_str);
        SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
        SCI_MEMCPY(applet_ptr->instance.search_str, search_str.wstr_ptr, search_str.wstr_len*sizeof(wchar));
    }

   search_str.wstr_ptr = applet_ptr->instance.search_str;
   search_str.wstr_len = MMIAPICOM_Wstrlen((wchar *)applet_ptr->instance.search_str);  //search_str.wstr_len = search_str.wstr_len;
   
   if(applet_ptr->instance.list_type == MMIPB_LIST_NORMAL_MAIN)
   {
        uint32 exclude_group = 0;
        if(PB_GROUP_ALL == group_id)//根据设置主页面显示sim卡或者nv或者both的contact
        {
            MMIPB_CONTACT_DISPLAY_TYPE_E display_type = MMIPB_CONTACT_DISPLAY_ALL;
            display_type = MMIPB_ReadDisplayOpt();
            if(MMIPB_CONTACT_DISPLAY_PHONE == display_type)
            {
                exclude_group = MMIPB_GetAllSIMGroup();//(MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_1));//SIM卡1

                total_count = MMIPB_MultiSearch(&search_str, MMIPB_ALL_GROUP_MASK, exclude_group, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
            }
            else if(MMIPB_CONTACT_DISPLAY_SIM1 == display_type)
            {
                total_count = MMIPB_MultiSearch(&search_str, (1<<PB_GROUP_SIM_1), 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else if(MMIPB_CONTACT_DISPLAY_SIM2 == display_type)  
            {
                total_count = MMIPB_MultiSearch(&search_str, (1<<PB_GROUP_SIM_2), 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
            }
#if (defined MMI_MULTI_SIM_SYS_TRI||defined MMI_MULTI_SIM_SYS_QUAD)
            else if(MMIPB_CONTACT_DISPLAY_SIM3 == display_type)  
            {
                total_count = MMIPB_MultiSearch(&search_str, (1<<PB_GROUP_SIM_3), 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
            }      
#endif
#ifdef MMI_MULTI_SIM_SYS_QUAD
            else if(MMIPB_CONTACT_DISPLAY_SIM4 == display_type)  
            {
                total_count = MMIPB_MultiSearch(&search_str, (1<<PB_GROUP_SIM_4), 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
            } 
#endif
#endif
            else
            {
                total_count = MMIPB_MultiSearch(&search_str, MMIPB_ALL_GROUP_MASK, 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
            }

        }
        else
        {

            total_count = MMIPB_MultiSearch(&search_str, (1<<group_id), 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
        }
   }
   else
   {
#ifdef SNS_SUPPORT
       total_count = MMIPB_MultiSearch(&search_str, (1<<group_id), MMIPB_GetAllSNSGroup(), (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
#else
       total_count = MMIPB_MultiSearch(&search_str, (1<<group_id), 0, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
#endif    
   }


//把搜索的结果显示出来
   MMIPB_ReadContactList(&contact_list_info);
   if(0 !=total_count)
   {
       SCI_TRACE_LOW("yanyan.an first_char_count:%d, number_count:%d, mail_count:%d", 
                        contact_list_info.first_char_count,
                        contact_list_info.number_count,
                        contact_list_info.mail_count);
       if(MMIPB_LIST_FIELD_WHOLE == applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])
       {
           total_count = total_count +contact_list_info.first_char_count;
       }
       else
       {
           if(MMIPB_LIST_FIELD_NUMBER & applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])/*lint !e655*/
           {
               total_count  = contact_list_info.number_count;
           }
           if(MMIPB_LIST_FIELD_MAIL & applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])/*lint !e655*/
           {
               total_count = total_count+contact_list_info.mail_count;
           }
           total_count += contact_list_info.first_char_count;
       }
   }
    storage_id = MMIPB_GetStorageIDByGroupID(group_id);
    SetListItem(win_id, total_count, storage_id, 0, ctrl_id);
    GUILIST_SetCurItemIndex(ctrl_id, (uint16)cur_index);              
    GUILIST_ValidCurItemPos(ctrl_id);

#ifdef MMIPB_MOST_USED_SUPPORT 
    if(PB_GROUP_FAVROUTE == group_id)
    {
        MMI_GetLabelTextByLang(TXT_PB_NO_FAVOURITE, &emtry_str_t);        
    }
    else
#endif
    {
#ifdef MMIPB_MOST_USED_SUPPORT 
        if((0== MMIPB_GetAllNormalContactCount())
           || (PB_GROUP_MOSTUSED == group_id)
           )
        {
            MMI_GetLabelTextByLang(TXT_PB_NO_ENTRY, &emtry_str_t);
        }
        else
#endif
        {
            MMI_GetLabelTextByLang(TXT_PB_NO_MATCHED, &emtry_str_t);
        }
        
    }

    list_emty_info.text_buffer = emtry_str_t;
    GUILIST_SetEmptyInfo(ctrl_id, &list_emty_info);
}
#endif

#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR) && defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePDAEntryListWinHook
(
    MMI_WIN_ID_T      win_id, 
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM            param
)
{
    MMI_HANDLE_T            applet_handle = {0};
    MMI_HANDLE_T            toolbar_ctrl_handle = NULL;
    MMIPB_APPLET_T*         applet_ptr = PNULL;
    GUI_RECT_T rc_tb = {0};
    GUI_POINT_T pt = {0};          

    
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;

    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))//设置联系人页面
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
            }
            else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))//设置常用联系人页面
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_TOOLBAR_LISTBOX_CTRL_ID);
            }
#ifdef BT_DIALER_SUPPORT
            else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))//设置常用联系人页面
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_TOOLBAR_LISTBOX_CTRL_ID);
            }
#endif
            else
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_TOOLBAR_LISTBOX_CTRL_ID);
            }
            pt.x = MMK_GET_TP_X(param);
            pt.y = MMK_GET_TP_Y(param);

            GUIAPICTRL_GetRect(toolbar_ctrl_handle, &rc_tb);
            if (!GUI_PointIsInRect(pt, rc_tb))
            {
                if(GUITOOLBAR_GetShowType(toolbar_ctrl_handle)==GUITOOLBAR_EXPAND)
                {
                    GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK);
                }
            }


        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    default:
        break;
    }
    return MMI_RESULT_FALSE;
   
}
#endif


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : TabList列表的窗口处理函数
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePDAEntryListWinMsg
(
    MMI_WIN_ID_T      win_id, 
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM            param
)
{
//	uint16                  cur_index = 0;
    uint32                  total_count = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID);//MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID;
   // MMIPB_NAME_T            name_t = {0};/*lint !e64*/
   // uint8                   group_id = PB_GROUP_ALL;
  //  BOOLEAN                 is_key_num = FALSE;
    MMIPB_LIST_HANDLE      *list_handle = PNULL;
	
//    BOOLEAN                 is_underchar = FALSE;
//    uint16                  current_sel_count = 0;
//	GUITAB_ITEM_T           *tab_item_ptr = PNULL;
    MMI_HANDLE_T            applet_handle = {0};
    MMIPB_APPLET_T*         applet_ptr = PNULL;
   // const GUILIST_ITEM_T *  cur_list_ptr = {0};
    MMI_STRING_T            edit_str = {0};
    MMI_STRING_T            search_str = {0};

    MMIPB_MENU_DATA_INFOR_T menu_data={0};	
    MMIPB_CONTACT_T *contact_ptr = PNULL;  
    MMI_HANDLE_T          select_button_ctr_id = MMIPB_CHILD_ALL_SELECT_BUTTON_CTRL_ID;
	MMI_CTRL_ID_T          edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID);//MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID;

	list_handle = MMK_GetWinAddDataPtr(win_id);
	applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(list_handle == PNULL||PNULL == applet_ptr)
	{
	    result = MMI_RESULT_FALSE;
		return result;
	}
    applet_handle = applet_ptr->caf_applet.app_handle;
    applet_ptr->instance.active_win_id = win_id;

    applet_ptr->instance.contacts_type = list_handle->contacts_type;
    applet_ptr->instance.cur_group_id = (PB_GROUP_LIST_E)list_handle->group_id;
    if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))
    {
        ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID);//
		edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID);//
        select_button_ctr_id =  MMIPB_CHILD_ALL_SELECT_BUTTON_CTRL_ID;
    }
#ifdef MMIPB_MOST_USED_SUPPORT    
    else if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))
    {
        ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID);//
	//	edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_CONTACT_SEARCH_EDITBOX_CTRL_ID);
        select_button_ctr_id = MMIPB_CHILD_MOSTUSED_SELECT_BUTTON_CTRL_ID;
    }
#endif    
    else
    {
        ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_ENTRY_LISTBOX_CTRL_ID);
		edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID);
        select_button_ctr_id = MMIPB_CHILD_GROUP_SELECT_BUTTON_CTRL_ID;
    }

    //SCI_TRACE_LOW:"[MMIPB]HandlePDAEntryListWinMsg win id 0x%x msg_id 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8956_112_2_18_2_44_32_352,(uint8*)"dd", win_id, msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

		GUILIST_SetListState(ctrl_id, GUILIST_STATE_EFFECT_STR, TRUE);

		applet_ptr->instance.active_win_id = win_id;
		InitSubTabControls(win_id, list_handle);
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
		 MMK_SetWinHookFunc( win_id, HandlePDAEntryListWinHook);
#endif
         if(win_id != MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID)) 
         {
            IGUICTRL_SetCircularHandleLeftRight(MMK_GetCtrlPtr(edit_ctr_id), FALSE);
         }
         
		if(MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)
		{
			//分组列表
			MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)FALSE);
#if defined MMI_ISTYLE_SUPPORT 			
            if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
#endif			
		}
        break;
    case MSG_NOTIFY_TPMOVE_LEFT_SIDE:
        HandleToolBarAction(win_id, ctrl_id, PB_OPTMENU_NODE_SEND_SMS, PNULL,TRUE);
        //SCI_TRACE_LOW:"[MMIPB] MSG_NOTIFY_TPMOVE_LEFT_SIDE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8973_112_2_18_2_44_32_353,(uint8*)"");
        break;

    case MSG_NOTIFY_TPMOVE_RIGHT_SIDE:
        HandleToolBarAction(win_id, ctrl_id, PB_OPTMENU_NODE_DIAL_AUDIO, PNULL,TRUE);
        //SCI_TRACE_LOW:"[MMIPB] MSG_NOTIFY_TPMOVE_RIGHT_SIDE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_8978_112_2_18_2_44_32_354,(uint8*)"");
        break;
        
#ifdef TOUCH_PANEL_SUPPORT    
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)        
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
        {
            MMI_HANDLE_T toolbar_ctrl_handle = MMIPB_TOOLBAR_LISTBOX_CTRL_ID;//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;



            contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL == contact_ptr)
            {
                break;
            }
            SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
            
            MMIPB_GetContactInfoFromList(win_id, ctrl_id, contact_ptr);
#ifdef SNS_SUPPORT
            if(MMIPB_IsSNSContact(contact_ptr->storage_id))
            {
                SCI_FREE(contact_ptr);
                break;
            }
#endif
            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(PNULL == item_content_ptr)
            {
                SCI_FREE(contact_ptr);
                break;
            }
            if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))//设置联系人页面
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
            }
            else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))//设置常用联系人页面
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_TOOLBAR_LISTBOX_CTRL_ID);
            }
#ifdef BT_DIALER_SUPPORT
            else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))//设置常用联系人页面
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_TOOLBAR_LISTBOX_CTRL_ID);
            }
#endif
            else
            {
                toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_TOOLBAR_LISTBOX_CTRL_ID);
            }
            SetToolBarState(toolbar_ctrl_handle, contact_ptr);
             UpdatePbListWinToolbar(ctrl_id, toolbar_ctrl_handle);
            if(GUITOOLBAR_GetShowType(toolbar_ctrl_handle)==GUITOOLBAR_EXPAND)
            {
                GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK );
            }
            else
            {
                GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_EXPAND); 
            }
            SCI_FREE(contact_ptr);                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        }
        break;     
#endif
#endif

#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT
    case MSG_NOTIFY_LIST_GET_EFFECT_STR:
        if(win_id !=  MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))
        {
            GUILIST_EFFECT_STR_T *effect_str = (GUILIST_EFFECT_STR_T *)param;
            GUIEDIT_GetString(edit_ctr_id,&edit_str);
            AppendSearchItemColor(&applet_ptr->instance, win_id, effect_str, edit_str);
        }
         break;
#endif
         
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);  
        break;
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:      
        menu_data.applet_handle = applet_handle;            
        menu_data.parent_win =win_id;            
        menu_data.operate_type = MMIPB_OPERATE_TYPE_GROUP_MANAGE;
        menu_data.group = (1<<applet_ptr->instance.cur_group_id);
        menu_data.mask_group = MMIPB_GetAllSIMGroup();
        MMIPB_OpenOperationSelectWin(applet_handle,&menu_data);                         
        break;
#endif        
	case MSG_NOTIFY_IM_SWICTH:
		{

			GUIEDIT_ClearAllStr(MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID));
		}
		break;
	case MSG_CTL_TAB_SWITCH:
	    //SCI_TRACE_LOW:"[MMIPB]HandleEntryListWinMsg MSG_CTL_TAB_SWITCH win id 0x%x "
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_9063_112_2_18_2_44_32_355,(uint8*)"d", msg_id);

        if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))
        {
            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_CONTACT_SEARCH_EDITBOX_CTRL_ID),&search_str);
            SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
            SCI_MEMCPY(applet_ptr->instance.search_str, search_str.wstr_ptr, search_str.wstr_len*sizeof(wchar));
            
        }   
        else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))//设置常用联系人页面
        {
            SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
        }   
        else
        {
            GUIEDIT_GetString(MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID),&search_str);
            SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
            SCI_MEMCPY(applet_ptr->instance.search_str, search_str.wstr_ptr, search_str.wstr_len*sizeof(wchar));
        }   

        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)FALSE);
 		break;

    case MSG_NOTIFY_LIST_LONGOK:
        {
            if (MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
            {
                break;
            }
            menu_data.applet_handle = applet_handle;            
            menu_data.parent_win =win_id;            
            contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL == contact_ptr)
            {
                break;
            }

            SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
            
            MMIPB_GetContactInfoFromList(win_id, ctrl_id, contact_ptr);
#ifdef SNS_SUPPORT
            if(!MMIPB_IsSNSContact(contact_ptr->storage_id))
#endif
            {
                applet_ptr->instance.cur_storage_id = contact_ptr->storage_id;
                applet_ptr->instance.cur_entry_id = contact_ptr->contact_id;
		        menu_data.contact_ptr = contact_ptr;
                menu_data.group = 1<<list_handle->group_id;
				    
                if(PB_GROUP_ALL == list_handle->group_id)
                {
                    //most used group menu
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    menu_data.menu_type = MMIPB_MAIN_LONGTP_MENU;
                
                }
#ifdef MMIPB_MOST_USED_SUPPORT
                else if(PB_GROUP_MOSTUSED == list_handle->group_id)
                {
                    //most used group menu
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    menu_data.menu_type = MMIPB_MOSTUSED_LONGTP_OPT_MENU;
                
                }  

                else if(PB_GROUP_FAVROUTE == list_handle->group_id)
                {
                    //favorite group menu
                    menu_data.menu_type = MMIPB_FAVORITE_LONGTP_OPT_MENU;
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                                
                }
#endif
                else
                {                
                    //normal group menu
                    if(MMIPB_IsSimRecord(contact_ptr->storage_id))
                    {
                        SCI_Free(contact_ptr);
                        break;
                    }
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    menu_data.menu_type = MMIPB_GROUP_ENTRY_LONGTP_MENU;
                }
                menu_data.need_reset_storage = TRUE;
                menu_data.valid_contact_count =1;
                //SCI_TRACE_LOW:"the applet handle is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_9143_112_2_18_2_44_33_356,(uint8*)"d",menu_data.applet_handle);
                MMIPB_EnterPbTabOptMenu(&menu_data);
            }
            SCI_Free(contact_ptr);
        }
        break;
  
    case MSG_GET_FOCUS:
        applet_ptr->instance.active_win_id = win_id;
#if defined MMI_ISTYLE_SUPPORT 
		if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())	
		{
			if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
			{
				//has menu
				if(MMIPB_MOSTUSED_CONTACTS == list_handle->contacts_type)
				{
					GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
					GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);
				}
				else
				{
					
					GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
					if((MMIPB_SINGLE_GROUP_CONTACTS != list_handle->contacts_type))
					{
                        
						GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, TXT_NULL, FALSE);
						GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_LEFT, IMAGE_ISTYLE_TITLE_ADD, IMAGE_ISTYLE_TITLE_ADD_DOWN, FALSE);
						GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_LEFT, MSG_NOTIFY_TITLE_RETURN_ICON, 0);
					}
				}
			}
			else
			{
				//cancel and ok
				//取消sk
				GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
				GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_CANCEL, FALSE);
				//完成sk
				GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
				GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_FINISH, FALSE);
				//GUIWIN_SetTitleIstyleIconMsg(win_id, TITLE_ISTYLE_MENU_ICON, MSG_NOTIFY_TITLE_MENU_ICON, 0);
			}

            GUIWIN_SetTitleIsVisible(win_id, TRUE, FALSE);
        }
#endif		
        break;

    case MMI_PB_RELOAD_SEARCH_LIST_MSG:
        HandlePdaMainReloadMsg(win_id, ctrl_id, list_handle->contacts_type, applet_ptr->instance.cur_group_id);
        //set title text
        if(MMIPB_SINGLE_GROUP_CONTACTS == applet_ptr->instance.contacts_type )
        {            
            MMIPB_NAME_T                        group_name = {0};
            uint32 group_contact_num = 0;
            char   contact_num_str[MMIPB_NAME_MAX_LEN] = {0};    
            MMIPB_ReadGroupName(&group_name, applet_ptr->instance.cur_group_id);                
            group_contact_num =  MMIPB_GetContactNumOfGroup(applet_ptr->instance.cur_group_id);
            sprintf(contact_num_str,"%s%d%s", "(",group_contact_num, ")");
            MMIAPICOM_StrToWstr(contact_num_str,group_name.wstr + group_name.wstr_len);/*lint !e64*/
            group_name.wstr_len += strlen(contact_num_str);
            GUIWIN_SetTitleText(win_id, group_name.wstr, group_name.wstr_len, FALSE);
        }
        if(MMK_IsFocusWin(win_id) && param) //cr150331
        {
            //need full paint window
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;


    case MSG_CTL_LIST_NEED_ITEM_DATA:
		AppendDyncListItem(win_id, ctrl_id,param,MMIPB_NEED_LIST_ITEM_DATA_TYPE, 0);
        break;
#if 0 
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        //dync load item content to list box
		AppendDyncListItem(win_id, ctrl_id,param,MMIPB_NEED_LIST_ITEM_CONTENT_TYPE, 0);
        break;
#endif


     case MMI_PB_RESET_LIST_MSG:
        total_count = GUILIST_GetTotalItemNum(ctrl_id);
        SetListItem(win_id, total_count, 
                    MMIPB_GetStorageIDByGroupID(list_handle->group_id), 
                    0, ctrl_id);
        
        break; 

    case MSG_APP_OK:
    case MSG_CTL_OK:    // 完成:
    case MSG_APP_MENU:
        if(MMIPB_LIST_RADIO == applet_ptr->instance.list_type )
        {
            break;
        }
        if(MMIPB_SINGLE_GROUP_CONTACTS == applet_ptr->instance.contacts_type)
        {
#ifdef SNS_SUPPORT            
            if(MMIPB_IsSNSGroup(applet_ptr->instance.cur_group_id))
            {
                break;
            }
#endif            
            if(MMIPB_IsSIMGroup(applet_ptr->instance.cur_group_id) && !MMIPB_GetGroupContactCount(applet_ptr->instance.cur_group_id))
            {
                break;
            }
        }
#ifdef MMIPB_MOST_USED_SUPPORT
        if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
        {
            if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type )
            {
                menu_data.applet_handle = applet_handle;
			    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    menu_data.menu_type = MMIPB_MOSTUSED_OPT_MENU;
                    menu_data.need_reset_storage = TRUE;
                    menu_data.valid_contact_count = GUILIST_GetTotalItemNum(ctrl_id);
				    MMIPB_EnterPbTabOptMenu(&menu_data);
                break;
            }
            else
            {
                AddCheckItemToOperateList(win_id, ctrl_id, FALSE);
                if(applet_ptr->instance.selelect_callback != PNULL)
                {
                    applet_ptr->instance.selelect_callback(applet_ptr->instance.handle);
                }
                //else
                {
                    MMK_CloseApplet(applet_handle);
                }
            }

        }
#endif
        if(!MMIPB_IsPbReady())
        {
            if (!MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_WAITING_ALERT_WIN_ID)))
            {
                MMIPUB_OpenAlertWinByTextIdEx(applet_handle, PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, 0);
            }
            break;
        }
        else
        {
            //open menu win
             
            if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
            {
                SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));           
               // MMIPB_GetContactInfoFromList(win_id, ctrl_id, contact_ptr);
                applet_ptr->instance.cur_storage_id = MMIPB_GetStorageIDByGroupID(applet_ptr->instance.cur_group_id);                
                menu_data.applet_handle = applet_handle;
                
                menu_data.parent_win = win_id;
                menu_data.group = (1<<applet_ptr->instance.cur_group_id);
                
                if(MMIPB_ALL_GROUP_CONTACTS == list_handle->contacts_type)
                {
                    menu_data.menu_type = MMIPB_MAIN_OPT_MENU;
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    //MMIPB_EnterPbTabOptMenu(&menu_data);
                }
#ifdef MMIPB_MOST_USED_SUPPORT
                else if(MMIPB_MOSTUSED_CONTACTS == list_handle->contacts_type)
                {
                    if(PB_GROUP_FAVROUTE == list_handle->group_id)
                    {

                        menu_data.menu_type = MMIPB_FAVORITE_OPT_MENU;
						menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    }
                    else
                    {
                        menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                        menu_data.menu_type = MMIPB_MOSTUSED_OPT_MENU;
                    }
                    //MMIPB_EnterPbTabOptMenu(&menu_data);
                }
#endif
                else
                {
                    menu_data.menu_type = MMIPB_GROUP_ENTRY_OPT_MENU;
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                        
                }
                menu_data.valid_contact_count = GUILIST_GetTotalItemNum(ctrl_id);
                menu_data.need_reset_storage = TRUE;
                MMIPB_EnterPbTabOptMenu(&menu_data);

            }
            else
            {
                AddCheckItemToOperateList(win_id, ctrl_id, FALSE);
                if(applet_ptr->instance.selelect_callback != PNULL)
                {
                    applet_ptr->instance.selelect_callback(applet_ptr->instance.handle);
                }
                //else
                {
                    MMK_CloseApplet(applet_handle);
                }
            }
        }
        break;
#ifdef MMIPB_MOST_USED_SUPPORT
    case MSG_NOTIFY_DROPDOWNLIST_SELECTED:
        {

            uint16 dropdown_index = 0;

            dropdown_index = GUIDROPDOWNLIST_GetCurItemIndex(MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_CONTACT_DROPDOWNLIST_CTRL_ID));
            if(dropdown_index == MMIPB_DROPDOWNLIST_FAVOURITE_INDEX)  
            {
                list_handle->group_id = PB_GROUP_FAVROUTE;
                applet_ptr->instance.cur_group_id = PB_GROUP_FAVROUTE;
            }
            else if(dropdown_index == MMIPB_DROPDOWNLIST_RECENTUSED_INDEX)
            {
                list_handle->group_id = PB_GROUP_MOSTUSED;
                applet_ptr->instance.cur_group_id = PB_GROUP_MOSTUSED;
            }            
            
            MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
        }
        
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_NOTIFY_T notify = {0};
         //   uint16       current_index = 0;

            if(PNULL == param)
            {
                //SCI_TRACE_LOW:"[MMIPB] HandlePDAEntryListWinMsg PNULL == param"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_9346_112_2_18_2_44_33_357,(uint8*)"");
                break;
            }
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
            if ( (MMIPB_TOOLBAR_LISTBOX_CTRL_ID== notify.src_id)
                || (MMIPB_MOSTUSED_TOOLBAR_LISTBOX_CTRL_ID == notify.src_id) 
#ifdef BT_DIALER_SUPPORT
                || (MMIPB_BT_TOOLBAR_LISTBOX_CTRL_ID == notify.src_id) 
#endif
                || (MMIPB_GROUP_TOOLBAR_LISTBOX_CTRL_ID == notify.src_id)
                )
            {
                PB_OPTMENU_NODE_E      node_id = 0;
                
                MMI_HANDLE_T hToolbar =  MMK_GetCtrlHandleByWin(win_id,MMIPB_TOOLBAR_LISTBOX_CTRL_ID); 

                if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))//设置联系人页面
                {
                    hToolbar =  MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID); 
                }
                else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))//设置常用联系人页面
                {
                    hToolbar =  MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_TOOLBAR_LISTBOX_CTRL_ID); 
                }
#ifdef BT_DIALER_SUPPORT
                else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))//设置常用联系人页面
                {
                    hToolbar = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_TOOLBAR_LISTBOX_CTRL_ID);
                }
#endif
                else
                {
                    hToolbar =  MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_TOOLBAR_LISTBOX_CTRL_ID);
                }               
                node_id = (PB_OPTMENU_NODE_E)GUITOOLBAR_GetItemUserDate(hToolbar, GUITOOLBAR_GetClickItemIndex(hToolbar));
                HandleToolBarAction(win_id, ctrl_id,node_id, PNULL,FALSE);
             }
            else
#endif                
            if(notify.src_id ==select_button_ctr_id)
            {
                HandleSelectAllFunc(win_id, ctrl_id, MMK_GetCtrlHandleByWin(win_id, select_button_ctr_id));
                if(GUILIST_GetSelectedItemNum(ctrl_id)>0)
                {
                    GUISOFTKEY_SetButtonGray(win_id, MMIPB_SOFTKEY_CTRL_ID,LEFT_BUTTON, FALSE);
                }
                else
                {
                     GUISOFTKEY_SetButtonGray(win_id, MMIPB_SOFTKEY_CTRL_ID,LEFT_BUTTON, TRUE);
                }
                break;
            }
            else if(notify.src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                //ok
                AddCheckItemToOperateList(win_id, ctrl_id, TRUE);
                if(applet_ptr->instance.selelect_callback != PNULL)
                {
                    applet_ptr->instance.selelect_callback(applet_ptr->instance.handle);
                }
                //else
                {
                    MMK_CloseApplet(applet_handle);
                }
            }
            else if(notify.src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }

            else
            {
                PB_CONTROL_ID_E  control_id =  (PB_CONTROL_ID_E)((MMI_NOTIFY_T*)param)->src_id;

                if(control_id == MMIPB_ALL_SEARCH_ADD_BUTTON_CTRL_ID)
                {
                    if(MMIPB_IsPbReady())
                    {  
                        SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
                        menu_data.applet_handle = applet_handle;
                        
                        menu_data.operate_type = MMIPB_OPERATE_TYPE_ADD_ONE;
						menu_data.need_reset_storage = TRUE;
                        MMIPB_AddOrEditContactOfPb(&menu_data);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWinByTextIdEx(applet_handle, 
                            PNULL,TXT_PB_NOT_READY,TXT_NULL,IMAGE_PUBWIN_WARNING,
                            PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, PNULL);
                    }

                }
                else if((control_id == MMIPB_MOSTUSED_SEARCH_BUTTON_CTRL_ID) || (control_id ==  MMIPB_ALL_SEARCH_BUTTON_CTRL_ID))
                {
                }
                else
                {
#if defined (TOUCH_PANEL_SUPPORT)                   
                    DealWithTPButton(win_id, ctrl_id, (PB_CONTROL_ID_E)((MMI_NOTIFY_T*)param)->src_id);   
#endif
                }
            }
        }
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
	
        if(MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)
        {
            MMK_CloseWin(win_id);
			MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE); 
        }
        else
        {
            MMK_CloseApplet(applet_handle);
        }
        
        break;



	case MSG_CTL_LIST_MARK_ITEM:
        {
		    GUILIST_MARK_ITEM_T *list_param_ptr = NULL;
		    list_param_ptr = (GUILIST_MARK_ITEM_T*)param;
		    if (list_param_ptr->had_maked)
		    {
			    if(GUILIST_GetSelectedItemNum(ctrl_id) ==  (GUILIST_GetTotalItemNum(ctrl_id)-applet_ptr->instance.split_num)
				    && !applet_ptr->instance.is_all_select)
				    {
                        applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;
					    UpdateSelectAllPic(win_id, ctrl_id, MMK_GetCtrlHandleByWin(win_id, select_button_ctr_id));
				    }
			    
		    }
		    else
		    {
			    if(GUILIST_GetSelectedItemNum(ctrl_id) !=  (GUILIST_GetTotalItemNum(ctrl_id)-applet_ptr->instance.split_num)
				    && applet_ptr->instance.is_all_select)
				    {
                        applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;
					    UpdateSelectAllPic(win_id, ctrl_id, MMK_GetCtrlHandleByWin(win_id, select_button_ctr_id));
				    }			
		    }
            if(GUILIST_GetSelectedItemNum(ctrl_id)>0)
            {
                GUISOFTKEY_SetButtonGray(win_id, MMIPB_SOFTKEY_CTRL_ID,LEFT_BUTTON, FALSE);
            }
            else
            {
                 GUISOFTKEY_SetButtonGray(win_id, MMIPB_SOFTKEY_CTRL_ID,LEFT_BUTTON, TRUE);
            }
        }
 		break;

        case MSG_NOTIFY_LIST_SELECTEDALL:
            if(GUILIST_GetSelectedItemNum(ctrl_id) !=  (GUILIST_GetTotalItemNum(ctrl_id)-applet_ptr->instance.split_num))
			{
                applet_ptr->instance.is_all_select = FALSE;
				UpdateSelectAllPic(win_id, ctrl_id, MMK_GetCtrlHandleByWin(win_id, select_button_ctr_id));
			}
            break;

        case MSG_NOTIFY_TITLE_RETURN_ICON:
            {
                    SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
                    menu_data.applet_handle = applet_handle;
                    
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_ADD_ONE;
 
                    MMIPB_AddOrEditContactOfPb(&menu_data);
            }
            break;

    case MSG_CLOSE_WINDOW:
#if 0//Bug55216 基本资料界面点一位的号码返回后显示空列表
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_DETAILS_WIN_ID)))//for NEWMS00128531
        {
            MMK_CloseWin(MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_DETAILS_WIN_ID));
        }
#endif

        MMK_CloseWin(MMK_GetWinHandle(applet_handle,MMIPB_WAITING_ALERT_WIN_ID));
        MMK_CloseWin(MMK_GetWinHandle(applet_handle,MMIPB_PROGRESS_WIN_ID));
        MMIPUB_CloseProgressWin(PNULL);
         MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
		MMK_FreeWinAddData(win_id);
        break;
#ifdef MMIPB_TIPS_OPEN
    case MSG_CTL_TIPS_NEED_DATA:
        if (MMIPB_ONE_LINE_TIPS == applet_ptr->instance.display_mode)
        {
            SetTipsData(ctrl_id, param);
        }
        break;
#endif
   
    default:
        result = FALSE;
        break;
    }

    return (result);
}
#endif

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : TabList列表的列表刷新消息处理函数
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void HandleMainReloadMsg(MMI_WIN_ID_T win_id, 
                                  MMI_HANDLE_T ctrl_id, 
                                  MMIPB_CONTACTS_TYP_E contacts_type,
                                  uint32 group_id)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    uint16          cur_index = 0;
    uint16          input_len = 0;
    MMI_STRING_T     search_str = {0};
    uint32          total_count = 0;
    MMIPB_NAME_T    name_t ={0};
    MMI_HANDLE_T     edit_handle = NULL;
    uint32          exclude_group = 0;
    uint32          group = 0;

    MMIPB_CONTACT_LIST_INFO_T   contact_list_info = {0};
    if(PNULL == applet_ptr)
    {
        return;
    }
    
    //update current list 
	cur_index = GUILIST_GetCurItemIndex(ctrl_id);
	//first remove all list items
	if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
	{
		GUILIST_RemoveAllItems(ctrl_id);
	}

     if(MMIPB_ALL_GROUP_CONTACTS == contacts_type)
    {
       edit_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_SEARCH_EDITBOX_CTRL_ID);
    }

    else if(MMIPB_SINGLE_GROUP_CONTACTS == contacts_type)//single group
    {
        edit_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID);
    }
//lzk mark
#ifdef BT_PBAP_SUPPORT	
    else if(MMIPB_BT_CONTACTS == contacts_type)//single group
    {
        edit_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_CONTACT_SEARCH_EDITBOX_CTRL_ID);
    }
#endif
    GUIEDIT_GetString(edit_handle,&search_str);
    SCI_MEMSET(applet_ptr->instance.search_str, 0x00, sizeof(applet_ptr->instance.search_str));
    SCI_MEMCPY(applet_ptr->instance.search_str, search_str.wstr_ptr, search_str.wstr_len*sizeof(wchar));

    
   search_str.wstr_ptr = applet_ptr->instance.search_str;
   //search_str.wstr_len = search_str.wstr_len;

#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
    if(PB_GROUP_ALL == group_id)//根据设置主页面显示sim卡或者nv或者both的contact
    {
        MMIPB_CONTACT_DISPLAY_TYPE_E display_type = MMIPB_CONTACT_DISPLAY_ALL;
        display_type = MMIPB_ReadDisplayOpt();
        group = 1<<PB_GROUP_ALL;
        if(MMIPB_CONTACT_DISPLAY_PHONE == display_type)
        {
            exclude_group = MMIPB_GetAllSIMGroup();
            group = MMIPB_ALL_GROUP_MASK;
        }
        else if(MMIPB_CONTACT_DISPLAY_SIM1 == display_type)
        {
            group = (1<<PB_GROUP_SIM_1);
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else if(MMIPB_CONTACT_DISPLAY_SIM2 == display_type)  
        {
            group = (1<<PB_GROUP_SIM_2);
        }
#if (defined MMI_MULTI_SIM_SYS_TRI || defined MMI_MULTI_SIM_SYS_QUAD)
        else if(MMIPB_CONTACT_DISPLAY_SIM3 == display_type)  
        {
            group = (1<<PB_GROUP_SIM_3);
        }
#endif
#ifdef MMI_MULTI_SIM_SYS_QUAD
        else if(MMIPB_CONTACT_DISPLAY_SIM4 == display_type)  
        {
            group = (1<<PB_GROUP_SIM_4);
        }
#endif
#endif
    }
    else
#endif
    {
        group = (1<<group_id);
        exclude_group = 0;
    }

   if(applet_ptr->instance.is_subStrSearch)//全匹配查找
   {  
      total_count = MMIPB_SearchSubStringInList(applet_ptr->instance.sub_search_str,                                                
                                               applet_ptr->instance.search_type , 
                                               TRUE,
                                               group, exclude_group);		
   }
   else
   {
      total_count = MMIPB_MultiSearch(&search_str, group, exclude_group, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
   }



//把搜索的结果显示出来
   MMIPB_ReadContactList(&contact_list_info);
#ifdef BT_PBAP_SUPPORT
    if((0 !=total_count) && (MMIPB_BT_CONTACTS != contacts_type))
#else
    if(0 !=total_count)
#endif
    {
        if(MMIPB_LIST_FIELD_WHOLE == applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])
        {
            total_count = total_count +contact_list_info.first_char_count;
        }
        else
        {
            if( MMIPB_LIST_FIELD_NUMBER & applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])/*lint !e655*/
            {
//lzk mark
#ifdef BT_PBAP_SUPPORT			
                total_count = total_count*MMIPB_MAX_NV_PHONE_NUM;
#else
                total_count = total_count*MMIPB_MAX_NUM_TYPE;
#endif            
			}
            if(MMIPB_LIST_FIELD_MAIL& applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])/*lint !e655*/
            {
                 total_count = total_count + contact_list_info.mail_count ;
            }
            total_count += contact_list_info.first_char_count;
        }

    }

    
	if(applet_ptr->instance.is_subStrSearch)
	{
		input_len = 0;
	}
	else

	{
		input_len = search_str.wstr_len;
	}

//把搜索的结果显示出来
    SetListItem(win_id, total_count,
        MMIPB_GetStorageIDByGroupID(group_id),
        input_len, ctrl_id);
//     if(MMIPB_LIST_CHECK== applet_ptr->instance.list_type)
//     {
//     
//         if(GUILIST_GetTotalItemNum(ctrl_id)>0)
//         {
//             GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_OK, TXT_NULL, STXT_CANCEL,FALSE);
// #ifdef MMI_GUI_STYLE_MINISCREEN
//             GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, FALSE);
// #endif
//         }
//         else
//         {
//             GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL, TXT_NULL, STXT_CANCEL,FALSE);
// #ifdef MMI_GUI_STYLE_MINISCREEN
//             GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, FALSE);
// #endif
//         }
//     }
    GUILIST_SetCurItemIndex(ctrl_id, (uint16)cur_index);              
    GUILIST_ValidCurItemPos(ctrl_id);
//关闭联系人toolbar
   
#ifdef TOUCH_PANEL_SUPPORT
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type
        && MMIPB_ALL_GROUP_CONTACTS == applet_ptr->instance.contacts_type)
    {
        GUITOOLBAR_SetShowType(MMIPB_TOOLBAR_LISTBOX_CTRL_ID, GUITOOLBAR_SHRINK);
        UpdatePbListWinToolbar(win_id,ctrl_id);
        
    }
#endif
#endif

//设置群组联系人显示界面      

    if(MMIPB_SINGLE_GROUP_CONTACTS == applet_ptr->instance.contacts_type)
    {
        MMI_CTRL_ID_T titile_ctrl_id = MMK_GetWinTitleCtrlId(win_id);
        GUITITLE_BUTTON_STATE_E left_arrow = TITLE_BUTTON_NORMAL;
        GUITITLE_BUTTON_STATE_E right_arrow = TITLE_BUTTON_NORMAL;

        //分组列表,设置title
        MMIPB_ReadGroupName(&name_t, (uint8)(applet_ptr->instance.cur_group_id));
        GUIWIN_SetTitleText(win_id, name_t.wstr,name_t.wstr_len, FALSE);
        GUIWIN_SetTitleRollArrow(win_id,TRUE);

        if(0 == GetPreviousGroupID(applet_ptr->instance.cur_group_id))
        {
            left_arrow = TITLE_BUTTON_GRAYED;
        } 
        else if(0 == GetNextGroupID(applet_ptr->instance.cur_group_id))
        {
            right_arrow = TITLE_BUTTON_GRAYED;
        }

        GUITITLE_SetRollArrowStatus(win_id, titile_ctrl_id, GUITITLE_BUTTON_LEFT, left_arrow);
        GUITITLE_SetRollArrowStatus(win_id, titile_ctrl_id, GUITITLE_BUTTON_RIGHT, right_arrow);
    }


}
#endif

#if (!defined(MMI_PDA_SUPPORT)) && defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryListWinHook
(
    MMI_WIN_ID_T      win_id, 
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM            param
)
{
    MMI_HANDLE_T            toolbar_ctrl_handle = NULL;
    GUI_RECT_T rc_tb = {0};
    GUI_POINT_T pt = {0};          

    switch(msg_id)
    {

    case MSG_TP_PRESS_DOWN:
        {
            pt.x = MMK_GET_TP_X(param);
            pt.y = MMK_GET_TP_Y(param);
            toolbar_ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);

            GUIAPICTRL_GetRect(toolbar_ctrl_handle, &rc_tb);/*lint !e718 !e18*/
            if (!GUI_PointIsInRect(pt, rc_tb))
            {
                if(GUITOOLBAR_GetShowType(toolbar_ctrl_handle)==GUITOOLBAR_EXPAND)
                {
                    GUITOOLBAR_SetShowType(toolbar_ctrl_handle, GUITOOLBAR_SHRINK);
                }
            }

        }
        break;

    default:
        break;
    }
    return MMI_RESULT_FALSE;
   
}   
#endif    

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : TabList列表的窗口处理函数
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryListWinMsg
(
    MMI_WIN_ID_T      win_id, 
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM            param
)
{
    uint16                  cur_index = 0;
    uint32                  total_count = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMIPB_NAME_T            name_t = {0};
    uint8                   group_id = PB_GROUP_ALL;
    uint16                  input_len = 0;
    MMIPB_MENU_DATA_INFOR_T menu_data={0};
    //BOOLEAN                 is_key_num = FALSE;
    MMIPB_LIST_HANDLE      *list_handle = PNULL;
    //static MMI_STRING_T            search_str = {0};
    //BOOLEAN                 is_underchar = FALSE;
    //uint16                  current_sel_count = 0;
    //GUITAB_ITEM_T           *tab_item_ptr = PNULL;
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;

    MMI_HANDLE_T form_ctr_id = NULL;
    MMI_HANDLE_T subform_ctr_id = NULL;
    MMI_HANDLE_T search_edit_form_ctr_id = NULL;
    MMI_HANDLE_T edit_ctr_id = NULL;
    MMI_HANDLE_T search_ctr_id = NULL;
    MMI_HANDLE_T list_ctr_id = NULL;
    MMI_HANDLE_T toolbar_ctr_id = NULL; 
    MMI_HANDLE_T  select_label_ctr_id = NULL; 
    MMI_HANDLE_T select_button_ctr_id = NULL; 

#ifdef BT_DIALER_SUPPORT
#ifdef BT_PBAP_SUPPORT
    BT_ADDRESS                  dev_addr = {0};
    BT_STATUS                   pull_ret = BT_ERROR; 
    BOOLEAN                     is_dev_connect = FALSE;
    BT_STATUS                   register_ret = BT_ERROR;
    BT_STATUS                   connect_ret = BT_ERROR;
	MMI_STRING_T                wait_text = {0};
#endif
#endif

    list_handle = MMK_GetWinAddDataPtr(win_id);
    if(list_handle == PNULL)
    {
        return result;
    }
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return result;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    applet_ptr->instance.active_win_id = win_id;
    applet_ptr->instance.cur_group_id = list_handle->group_id;
    applet_ptr->instance.contacts_type = list_handle->contacts_type;
    applet_ptr->instance.active_win_id = win_id;
    
    if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID)) 
    {
        form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_SEARCH_WIN_FORM_ID);
        subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_CONTACT_SEARCH_WIN_FORM_ID);
        search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_CONTACT_SEARCH_EDIT_FORM_ID);
        edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CONTACT_SEARCH_EDITBOX_CTRL_ID);
        search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_SEARCH_BUTTON_CTRL_ID);
        list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LISTBOX_CTRL_ID);
        toolbar_ctr_id = MMIPB_TOOLBAR_LISTBOX_CTRL_ID;
        select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SELECT_TITLE_CTRL_ID);
        select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SELECT_BUTTON_CTRL_ID);
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID)) 
    {

        //常用联系人窗口
        form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_CONTACT_SEARCH_WIN_FORM_ID);
        list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID);
        toolbar_ctr_id = MMIPB_MOSTUSED_TOOLBAR_LISTBOX_CTRL_ID;

    }  
#endif
#ifdef BT_PBAP_SUPPORT
	else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_BT_CONTACTS_LIST_TAB_WIN_ID)) 
	{
        s_bt_tab_win_id = win_id;
		//蓝牙联系人窗口
		form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_CONTACT_SEARCH_WIN_FORM_ID);
        subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_CONTACT_SEARCH_WIN_FORM_ID);
        search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_CONTACT_SEARCH_EDIT_FORM_ID);
        edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_CONTACT_SEARCH_EDITBOX_CTRL_ID);
        search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_ENTRY_SEARCH_BUTTON_CTRL_ID);
		list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_ENTRY_LISTBOX_CTRL_ID);
		toolbar_ctr_id = MMIPB_BT_TOOLBAR_LISTBOX_CTRL_ID;
        select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_SELECT_TITLE_CTRL_ID);
        select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_BT_SELECT_BUTTON_CTRL_ID);
	}  
#endif
    else if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_GROUP_ENTRYL_LIST_WIN_ID))
    {
        form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID);
        subform_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_WIN_FORM_ID);
        search_edit_form_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_CONTACT_SEARCH_EDIT_FORM_ID);
        edit_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_CONTACT_SEARCH_EDITBOX_CTRL_ID);
        search_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_ENTRY_SEARCH_BUTTON_CTRL_ID);
        list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_ENTRY_LISTBOX_CTRL_ID);
        toolbar_ctr_id = MMIPB_TOOLBAR_LISTBOX_CTRL_ID;
        select_label_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_SELECT_TITLE_CTRL_ID);
        select_button_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SINGLE_GROUP_SELECT_BUTTON_CTRL_ID);
    }

    //SCI_TRACE_LOW:"[MMIPB]HandleEntryListWinMsg win id 0x%x msg_id 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_9947_112_2_18_2_44_34_360,(uint8*)"dd", win_id, msg_id);
    switch (msg_id)
    {
	case MSG_FULL_PAINT:
#ifdef BT_PBAP_SUPPORT
        if(PB_GROUP_BT == list_handle->group_id)
        {
            s_is_focus_bt_contacts = TRUE;
            if(TRUE == s_is_fullpaint_sync_once)
            {
                s_is_fullpaint_sync_once = FALSE;
				if(TRUE == s_is_sync_contacts)
				{
					uint32       dev_service = 0;
					BT_ADDRESS   bt_addr = {0};
					MMIAPIBT_GetHfuConnectedAddress(&bt_addr);
#ifndef WIN32
					if(BT_HfuGetConnStatus(&bt_addr))
#endif
					{
						dev_service = BT_GetPairedDeviceService(&bt_addr);
#ifndef WIN32
						if(BT_SERVICE_PBAP_SERVER & dev_service)
#endif
						{
							if (BT_GetState())
							{
								if(MMIAPIBT_IsConnectDevice())
								{
									MMIBT_SetPbapRequestType(BT_PBAP_TYPE_PB);
#if !defined(WIN32)
									if(MMIAPIBT_PbapIsConnected())
									{
										/************************************************************************/
										/* whether Static Data List is empty or not                             */
										/************************************************************************/
										MMIAPIPB_InitPBAPDataList();
										MMIPB_ClearAllBtContactForPbap();
										MMK_SendMsg(win_id, MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
									}
									else
#endif
									{
										if(!MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
										{
											MMK_SendMsg(win_id, MSG_BT_CONTACT_SYNC_CNF, PNULL);
										}
									}
								}
							}
						}
#ifndef WIN32   
						else
						{
							MMIBT_OpenAlertTipWin(TXT_BT_PBAP_NOT_SUPPORT); 
						}
#endif
					} 
				}
			}
        }
        else
        {
            s_is_focus_bt_contacts = FALSE;
        }
#endif
        break;
    case MSG_OPEN_WINDOW:

		GUILIST_SetListState(list_ctr_id, GUILIST_STATE_EFFECT_STR, TRUE);
        GUILIST_ChangeCheckListSoftkeyIndex(list_ctr_id,MIDDLE_BUTTON);
		applet_ptr->instance.active_win_id = win_id;
		InitSubTabControls(win_id, list_handle);
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
		 MMK_SetWinHookFunc( win_id, HandleEntryListWinHook);
#endif
		if(MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)
		{
			//分组列表
			MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)FALSE);
		}
		if((win_id != MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))
#ifdef BT_PBAP_SUPPORT
			&& (win_id != MMK_GetWinHandle(applet_handle, MMIPB_BT_CONTACTS_LIST_TAB_WIN_ID))
#endif
			)
        {
            IGUICTRL_SetCircularHandleLeftRight(MMK_GetCtrlPtr(edit_ctr_id), FALSE);
         }
//Bug 119434 
#if 0
 #ifdef MMIPB_MOST_USED_SUPPORT
        if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))
        {
            MMK_SetAtvCtrl(win_id, list_ctr_id);
            GUILIST_SetCurItemIndex(list_ctr_id,0);
        }
        else
#endif
        MMK_SetAtvCtrl(win_id, edit_ctr_id);
#endif
		MMK_SetAtvCtrl(win_id, list_ctr_id);
        GUILIST_SetCurItemIndex(list_ctr_id,0);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT 
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
        {
            MMI_HANDLE_T toolbar_ctrl_handle = MMIPB_TOOLBAR_LISTBOX_CTRL_ID;//MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID);
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;

            MMIPB_CONTACT_T* contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL == contact_ptr)
            {
                break;
            }
            SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
            
            MMIPB_GetContactInfoFromList(win_id, list_ctr_id, contact_ptr);
#ifdef SNS_SUPPORT
            if(MMIPB_IsSNSContact(contact_ptr->storage_id))
            {
                SCI_FREE(contact_ptr);
                break;
            }
#endif
            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            if(PNULL == item_content_ptr)
            {
                SCI_FREE(contact_ptr);
                break;
            }

            SetToolBarState(MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID), contact_ptr);
            UpdatePbListWinToolbar(list_ctr_id, MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID));
            if(GUITOOLBAR_GetShowType(MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID))==GUITOOLBAR_EXPAND)
            {
                GUITOOLBAR_SetShowType(MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID), GUITOOLBAR_SHRINK );
            }
            else
            {
                GUITOOLBAR_SetShowType(MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID), GUITOOLBAR_EXPAND); 
            }
            SCI_FREE(contact_ptr);                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
        }
        break; 
#endif
#endif

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);  
        break;
        
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT
    case MSG_NOTIFY_LIST_GET_EFFECT_STR:
        {
        
            GUILIST_EFFECT_STR_T *effect_str = (GUILIST_EFFECT_STR_T *)param;
            MMI_STRING_T        edit_str ={0};
            GUIEDIT_GetString(edit_ctr_id,&edit_str);
            AppendSearchItemColor(&applet_ptr->instance, win_id, effect_str, edit_str);
        }
        break;
#endif

    case MSG_LCD_SWITCH:
#ifdef TOUCH_PANEL_SUPPORT
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
        UpdatePbListWinToolbar(win_id, list_ctr_id);
#endif
#endif
        break; 

	case MSG_CTL_TAB_SWITCH:
#ifdef BT_PBAP_SUPPORT
        SCI_TRACE_LOW("HandleEntryListWinMsg MSG_CTL_TAB_SWITCH win_id:%d, list_handle->group_id:%d",win_id,list_handle->group_id);
        if(PB_GROUP_BT == list_handle->group_id)
        {
            s_is_fullpaint_sync_once = TRUE;
        }
#endif
        //SCI_TRACE_LOW:"[MMIPB]HandleEntryListWinMsg MSG_CTL_TAB_SWITCH win id 0x%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_10030_112_2_18_2_44_35_361,(uint8*)"d", msg_id);
        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)FALSE);
        break;

    case MSG_APP_LEFT:
    case MSG_CTL_TITLE_MOVE_PREV:
        //go previous group, 
        //change to previous group entry list
        if(MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)
        {
            list_handle = MMK_GetWinAddDataPtr(win_id);
            if(PNULL == list_handle)
            {
                break;
            }
            group_id = GetPreviousGroupID(list_handle->group_id);
            if(group_id > 0)
            {
                list_handle->group_id = group_id;
                MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
            }
        }
        else
        {
            result = FALSE;
        }
        break;
        
    case MSG_APP_RIGHT:
    case MSG_CTL_TITLE_MOVE_NEXT:
        if(MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)
        {
            list_handle = MMK_GetWinAddDataPtr(win_id);
            if(PNULL == list_handle)
            {
                break;
            }
            group_id = GetNextGroupID(list_handle->group_id);
            if(group_id > 0)
            {
                list_handle->group_id = group_id;
                MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
            }
        }
        else
        {
            result = FALSE;
        }
        break;

    case MSG_GROUP_NAME_CHANGED:
        list_handle = MMK_GetWinAddDataPtr(win_id);
        if(PNULL != list_handle && MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type) 
        {
            MMIPB_ReadGroupName(&name_t, list_handle->group_id);
            GUIWIN_SetTitleText(win_id, name_t.wstr, name_t.wstr_len, FALSE);
        }
        break;
        
    case MMI_PB_RELOAD_SEARCH_LIST_MSG: 
        HandleMainReloadMsg(win_id, list_ctr_id, applet_ptr->instance.contacts_type, applet_ptr->instance.cur_group_id);
        
#if 0
        if((MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)             
            && MMIPB_IsSIMGroup(applet_ptr->instance.cur_group_id)
            )
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);            
        }
        else if(MMIPB_LIST_CHECK== applet_ptr->instance.list_type)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
        }
        else
        {
#ifdef MMIPB_MOST_USED_SUPPORT
            if((MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
                &&(win_id == MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))
                &&(0==MMIPB_GetGroupContactCount(PB_GROUP_MOSTUSED)))
            {
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            else
#endif
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
                if(edit_ctr_id != NULL)
                {
                    GUIEDIT_SetSoftkey(edit_ctr_id,0,1,STXT_OPTION,STXT_OPTION,PNULL);
                    GUIEDIT_SetSoftkey(edit_ctr_id,1,1,TXT_NULL,TXT_NULL,PNULL);
                    GUIEDIT_SetSoftkey(edit_ctr_id,2,1,STXT_RETURN,STXT_RETURN,PNULL);               
                }
            }
        }
#endif
		/*********bug 115646 ***********/
		if ((MMIPB_LIST_CHECK == applet_ptr->instance.list_type) && (GUILIST_GetTotalItemNum(list_ctr_id)))
		{
			GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION, TXT_NULL, STXT_CANCEL,FALSE);
			GUIEDIT_SetSoftkey(edit_ctr_id, LEFT_BUTTON, 1, STXT_OPTION, STXT_OPTION, PNULL);
			GUILIST_ChangeCheckListSoftkeyIndex(list_ctr_id, MIDDLE_BUTTON);	
		}
		else if ((MMIPB_LIST_CHECK == applet_ptr->instance.list_type) && (!GUILIST_GetTotalItemNum(list_ctr_id)))
		{
			GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL, TXT_NULL, STXT_CANCEL,FALSE);
			GUIEDIT_SetSoftkey(edit_ctr_id, LEFT_BUTTON, 1, TXT_NULL, TXT_NULL, PNULL);
		}
		else
		{
            if(MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)   
            {
                if(!MMIPB_IsInNvGroup(applet_ptr->instance.cur_group_id))
                {
                    if((list_ctr_id == MMK_GetActiveCtrl(win_id))&&(GUILIST_GetTotalItemNum(list_ctr_id)))
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
                    }
					else if (!GUIEDIT_IsImNeedSoftkey(edit_ctr_id))
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                    }
					
                    //edit始终显示空
                    GUIEDIT_SetSoftkey(edit_ctr_id, LEFT_BUTTON, 1, TXT_NULL, TXT_NULL, PNULL);
                }
                else
                {
                    if(!GUILIST_GetTotalItemNum(list_ctr_id))
                    {
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
                        if(MMIPB_LIST_CHECK != applet_ptr->instance.list_type)
                        {
							//modification softkey directly will lead collision with input method and show the wrong tips
                            if (!GUIEDIT_IsImNeedSoftkey(edit_ctr_id))
							{
								GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
								GUIEDIT_SetSoftkey(edit_ctr_id, LEFT_BUTTON,1, STXT_OPTION, STXT_OPTION,PNULL);
							}
                        }
#else
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                        GUIEDIT_SetSoftkey(edit_ctr_id, LEFT_BUTTON,1, TXT_NULL, TXT_NULL,PNULL);   
#endif
                    }
                    else if (!GUIEDIT_IsImNeedSoftkey(edit_ctr_id))
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
                        GUIEDIT_SetSoftkey(edit_ctr_id, LEFT_BUTTON,1, STXT_OPTION, STXT_OPTION,PNULL); 
                    }
                }
            }
#ifdef MMIPB_MOST_USED_SUPPORT
            else if((MMIPB_MOSTUSED_CONTACTS == list_handle->contacts_type) && (0 == MMIPB_GetGroupContactCount(PB_GROUP_MOSTUSED)))
            {
    			GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
#endif
            else if(MMIPB_LIST_CHECK == applet_ptr->instance.list_type)
            {
                GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_OK, TXT_NULL, STXT_CANCEL,FALSE);
            }
            else
            {
                //for crBug 92309 
                //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
                GUIEDIT_SetSoftkey(edit_ctr_id, LEFT_BUTTON,1, STXT_OPTION, STXT_OPTION,PNULL);  
            }
            /*********Bug 4716&4573 end************/
#ifdef MMI_GUI_STYLE_MINISCREEN
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, FALSE);
#endif
		}

		if(MMK_IsFocusWin(win_id) && param) //cr150331
        {
            //need full paint window
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			//  MMK_SetAtvCtrl(win_id, edit_ctr_id);
        } 
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
	{
            MMI_STRING_T search_str_t = {0};
            if(applet_ptr->instance.is_subStrSearch)
            {
                input_len = 0;
            }
            else
            {
                GUIEDIT_GetString(edit_ctr_id, &search_str_t);
                input_len = search_str_t.wstr_len;
            }
            AppendDyncListItem(win_id, list_ctr_id,param,MMIPB_NEED_LIST_ITEM_DATA_TYPE, input_len);
	}
        break;
#if 0        
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
       //  list_ctr_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LISTBOX_CTRL_ID);
        //dync load item content to list box
		if(applet_ptr->instance.is_subStrSearch)
		{
			input_len = 0;
		}
		else
		{
            MMI_STRING_T search_str_t = {0};
#ifndef PB_SEARCH_SURPPORT_NONE
            GUIEDIT_GetString(edit_ctr_id, &search_str_t);
			input_len = search_str_t.wstr_len;
#endif
		}
		AppendDyncListItem(win_id, list_ctr_id,param,MMIPB_NEED_LIST_ITEM_CONTENT_TYPE, input_len);
        break;
#endif


    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        if (GUILIST_GetCurItemIndex(list_ctr_id) == GUILIST_GetTotalItemNum(list_ctr_id) - 1)
        {
            GUILIST_SetCurItemIndex(list_ctr_id,0);
        }
        MMK_SetAtvCtrl(win_id, list_ctr_id);
        if(GUILIST_GetTotalItemNum(list_ctr_id))
        {     
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
        result = MMI_RESULT_FALSE;
        break;
   
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:   
        if (0 == GUILIST_GetCurItemIndex(list_ctr_id))
        {
            MMK_SetAtvCtrl(win_id, edit_ctr_id);
        }
        else
        {
            
            if(GUILIST_GetTotalItemNum(list_ctr_id))
            {     
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            }
            MMK_SetAtvCtrl(win_id, list_ctr_id);
        }
        result = MMI_RESULT_FALSE;
        break;

#ifdef BT_DIALER_SUPPORT
#ifdef BT_PBAP_SUPPORT     
    case MSG_BT_CONTACT_SYNC_CNF:
        {
            is_dev_connect = MMIBT_IsConnectDevice(&dev_addr);

		    if(!is_dev_connect)
		    {
			    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                break;
            }

            /************************************************************************/
		    /*Registers the Phonebook Access Client                                  */
		    /************************************************************************/

		    register_ret = MMIPB_PbapClientRegister();

            SCI_TRACE_LOW("MMIPB_PbapClientRegister register_ret:%d", register_ret);

		    if(BT_SUCCESS != register_ret)
	        {
                SCI_TRACE_LOW("MMIBT_PbapClientRegister not success register_ret:%d",register_ret);
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_REGISTER_FAIL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	            break;
	        }

            /************************************************************************/
		    /* whether Static Data List is empty or not                             */
		    /************************************************************************/
		    MMIAPIPB_InitPBAPDataList();
            MMIPB_ClearAllBtContactForPbap();

            /************************************************************************/
		    /* Create a transport connection to the specified device and issue an  
		    OBEX Connect Request */
		    /************************************************************************/

		    if(!MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
		    {
			    MMI_GetLabelTextByLang(TXT_BT_CONNECT_WAITING, &wait_text); 
			    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PBAP_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
			        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinConnectWaitingMsg); 
                MMIPUB_SetWinSoftkey(MMIBT_PBAP_CONNECT_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
            }

            MMIPB_CreatePBAPTimer();

            SCI_TRACE_LOW("dev_addr: %x %x %x %x", 
                dev_addr.addr[0], dev_addr.addr[1], dev_addr.addr[2], dev_addr.addr[3]);
		    
            connect_ret = MMIPB_PbapConnect(&dev_addr);
            SCI_TRACE_LOW("MMIPB_PbapConnect connect_ret:%d", connect_ret);

            if(BT_SUCCESS == connect_ret)
            {
                MMK_SendMsg(win_id, MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
            }
            else if(BT_PENDING != connect_ret)
            {
                SCI_TRACE_LOW("MMIPB_PbapConnect not pending");
                //MMIBT_OpenAlertTipWin(TXT_BT_PBAP_CONNECT_FAIL);       
                MMIPUB_OpenAlertWinByTextIdEx(applet_ptr->caf_applet.app_handle,PNULL,TXT_BT_PBAP_CONNECT_FAIL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL); 
            }
        }
        break;

	case MSG_BT_PBAP_TP_CONNECT_SUCCESS: 
        {
#ifdef WIN32
	        BT_MSG_T  msg_body = {0};
#endif
            SCI_TRACE_LOW("MSG_BT_PBAP_TP_CONNECT_SUCCESS");
#ifdef WIN32
            MMICL_SimuPBAPDataToList(0);
#else
            MMIPB_StopPBAPTimer();
#endif
            /************************************************************************/
            /* Initiates the OBEX "Get" operation to retrieve a phonebook object from 
		    the remote Phonebook Access Server. */
            /************************************************************************/
            pull_ret = MMIBT_PbapPullPhonebook();

            SCI_TRACE_LOW("MMIPB_PbapPullPhonebook pull_ret:%d", pull_ret);
    
            s_is_sync_contacts = FALSE;

            if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
            {
                MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
            }

		    if (BT_PENDING != pull_ret)
            {
                if(BT_PBAP_TYPE_SIM != MMIBT_GetPbapRequestType())
                {
                    MMIPUB_OpenAlertWinByTextIdEx(applet_ptr->caf_applet.app_handle,PNULL,TXT_BT_PULL_PHONEBOOK_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,PNULL);
                }
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_BT_SYNC_WAITING, &wait_text); 
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_SYNC_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinSyncWaitingMsg); 
                MMIPUB_SetWinSoftkey(MMIBT_SYNC_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
            }
#ifdef WIN32
			/*test +++*/
			MMIBT_SimuNotifyPBStartParse();
			/*test ---*/
#endif
        }
        break;
#endif
#endif
    case MSG_APP_0:
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_APP_Q:
    case 	MSG_APP_W:	
    case 	MSG_APP_E:	
    case	MSG_APP_R:	
    case 	MSG_APP_T:	
    case 	MSG_APP_Y:	
    case    MSG_APP_U:	
    case 	MSG_APP_I:	
    case 	MSG_APP_O: 
    case 	MSG_APP_P:	
    case 	MSG_APP_A:	
    case 	MSG_APP_S:	
    case 	MSG_APP_D:  
    case 	MSG_APP_F:  
    case 	MSG_APP_G: 
    case 	MSG_APP_H:	
    case 	MSG_APP_J:	
    case 	MSG_APP_K:	
    case 	MSG_APP_L:	
    case 	MSG_APP_Z:	
    case 	MSG_APP_X:	
    case 	MSG_APP_C:	
    case 	MSG_APP_V: 
    case 	MSG_APP_B:	
    case 	MSG_APP_N:	
    case 	MSG_APP_M:	
    case    MSG_APP_COMMA:	
    case    MSG_APP_PERIOD:	
    case 	MSG_APP_AT_QWERTY:	
    case    MSG_APP_SPACE:	
    case    MSG_APP_AND:	
    case	MSG_APP_QUESTION:	
    case	MSG_APP_CTRL:	
    case MSG_APP_STAR:
    case  MSG_APP_PLUS:            
    case MSG_APP_LPARENTHESIS:    
    case  MSG_APP_RPARENTHESIS:    
    case  MSG_APP_MINUS:           
    case  MSG_APP_DQUOTES:         
    case MSG_APP_SEMICOLON:       
    case  MSG_APP_COLON:           
    case  MSG_APP_SLASH:           
    case MSG_APP_SAND:            
    case  MSG_APP_EXCLAMATION:     
#endif
    case MSG_APP_HASH:
    case MSG_TIMER:
        MMK_SetAtvCtrl(win_id, edit_ctr_id);
        break;       

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    //case MSG_KEYDOWN_DEL:
		//exit pb list
		
//lzk mark
#ifdef BT_PBAP_SUPPORT
	   if(MMIPB_ALL_GROUP_CONTACTS == list_handle->contacts_type || MMIPB_MOSTUSED_CONTACTS == list_handle->contacts_type/* || MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type*/ 
           || MMIPB_BT_CONTACTS == list_handle->contacts_type)
#else
       if(MMIPB_ALL_GROUP_CONTACTS == list_handle->contacts_type || MMIPB_MOSTUSED_CONTACTS == list_handle->contacts_type)
#endif
	   {
		   MMK_CloseAllWinByApplet(applet_handle);
	   }
	   else
	   {
		   MMK_CloseWin(win_id);
	   }	
       break;

    case MMI_PB_OPERATE_SELECT_ALL:
        MMIPB_HandleListFilter(win_id, list_ctr_id, MMIPB_FILTER_ALL);
        break;

    case MMI_PB_OPERATE_UNSELECT_ALL:
        MMIPB_HandleListFilter(win_id, list_ctr_id, MMIPB_FILTER_NONE);
        break; 

#ifdef CMCC_UI_STYLE
    case MMI_PB_OPERATE_SELECT_SIM:
        MMIPB_HandleListFilter(win_id, list_ctr_id, MMIPB_FILTER_SIM);
        break;

    case MMI_PB_OPERATE_SELECT_PHONE:
        MMIPB_HandleListFilter(win_id, list_ctr_id, MMIPB_FILTER_PHONE);
        break;
#endif

    case MMI_PB_OPERATE_OK:
        MMI_CheckAllocatedMemInfo();
        if(GUILIST_GetSelectedItemNum(list_ctr_id)> MMIPB_GetListAllowableMaxSelectedNum(applet_ptr->caf_applet.app_handle))
        {
           MMIPB_OpenAlertTipWin(applet_handle, MMIPB_ERROR_MAX_LIMIT,applet_ptr->instance.cmd);
           break;
        }
        MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
        AddCheckItemToOperateList(win_id, list_ctr_id, FALSE);
       MMI_CheckAllocatedMemInfo();
        if(applet_ptr->instance.selelect_callback != PNULL)
        {
            applet_ptr->instance.selelect_callback(applet_ptr->instance.handle);
        }
        //else
        MMI_CheckAllocatedMemInfo();
        {
            MMK_CloseApplet(applet_handle);
        }  
        MMI_CheckAllocatedMemInfo();
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:    // 完成:
    case MSG_APP_MENU:
        if( MMIPB_LIST_CHECK == applet_ptr->instance.list_type)
        {
            if(GUILIST_GetTotalItemNum(list_ctr_id)>0)
            {     
                MMIPB_MENU_DATA_INFOR_T menu_info = {0};
                menu_info.parent_win = win_id;
                menu_info.menu_type = MMIPB_OPERATE_SEL_OPT_MENU;
                menu_info.applet_handle = applet_handle;
                menu_info.ctl_id = MMK_GetCtrlId(list_ctr_id);   //cr209656
                MMIPB_EnterPbTabOptMenu(&menu_info); 
                break;
                //MMK_SendMsg(win_id, MMI_PB_OPERATE_OK,PNULL);
            }
            else
            {
                break;
            }
        }
        if(MMIPB_LIST_RADIO == applet_ptr->instance.list_type )
        {
            break;
        }
		if (MMIPB_IsInSIMGroup(applet_ptr->instance.cur_group_id) && (0 == GUILIST_GetTotalItemNum(list_ctr_id)))
		{
			break;
		}

#ifdef MMIPB_MOST_USED_SUPPORT        
        if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
        {
            if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
            {
                if(GUILIST_GetTotalItemNum(list_ctr_id) > 0)
                {
                    SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T)); 
                    menu_data.applet_handle = applet_handle;
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    menu_data.menu_type = MMIPB_MOSTUSED_OPT_MENU;
                    menu_data.need_reset_storage = TRUE;
                    menu_data.valid_contact_count = GUILIST_GetTotalItemNum(list_ctr_id);
                    MMIPB_EnterPbTabOptMenu(&menu_data);
                }
                break;
            }
        }
#endif

        if(!MMIPB_IsPbReady())
        {
            if (!MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_WAITING_ALERT_WIN_ID)))
            {
                MMIPUB_OpenAlertWinByTextIdEx(applet_handle, PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL, 0);
            }
            break;
        }
        else
        {
            //open menu win             
            if( MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
            {
                MMIPB_CONTACT_T* contact_ptr = PNULL;
                contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(PNULL == contact_ptr)
                {
                    break;
                }
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));           
                MMIPB_GetContactInfoFromList(win_id, list_ctr_id, contact_ptr);
                if(MMIPB_IsContactHasMenu(contact_ptr->storage_id))
               {
                    applet_ptr->instance.cur_storage_id = contact_ptr->storage_id;
                    applet_ptr->instance.cur_entry_id = contact_ptr->contact_id;
                    menu_data.applet_handle = applet_handle;                    
                    menu_data.parent_win = win_id;
                    menu_data.group =(1 << applet_ptr->instance.cur_group_id);
                    menu_data.contact_ptr = contact_ptr;
                    menu_data.valid_contact_count = GUILIST_GetTotalItemNum(list_ctr_id);
                    #ifndef MMI_PDA_SUPPORT
                    if(list_ctr_id != MMK_GetActiveCtrl(win_id))
                    {
                        menu_data.not_support_single_operate = TRUE;
                    }
                    #endif
                    if(MMIPB_ALL_GROUP_CONTACTS == list_handle->contacts_type)
                    {
                        menu_data.menu_type = MMIPB_MAIN_OPT_MENU;
                        menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                        menu_data.need_reset_storage = TRUE;
                        MMIPB_EnterPbTabOptMenu(&menu_data);
                    }
                    #ifdef MMIPB_MOST_USED_SUPPORT
                    else if(MMIPB_MOSTUSED_CONTACTS == list_handle->contacts_type)
                    {
                        if(0 != MMIPB_GetGroupContactCount(PB_GROUP_MOSTUSED))
                        {
                            menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                            menu_data.menu_type = MMIPB_MOSTUSED_OPT_MENU;
                            menu_data.need_reset_storage = FALSE;
                            MMIPB_EnterPbTabOptMenu(&menu_data);
                        }

                    }
                    #endif
                    else if(MMIPB_SINGLE_GROUP_CONTACTS == list_handle->contacts_type)                                                
                    {
                        {
                            menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                            menu_data.menu_type = MMIPB_GROUP_ENTRY_OPT_MENU;
                        }
                        menu_data.need_reset_storage = TRUE;
                        MMIPB_EnterPbTabOptMenu(&menu_data);
                    }
#ifdef BT_PBAP_SUPPORT
					else if (MMIPB_BT_CONTACTS == list_handle->contacts_type)
					{
						menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
						menu_data.menu_type = MMIPB_BT_LIST_OPT_MENU;
						menu_data.need_reset_storage = TRUE;
						MMIPB_EnterPbTabOptMenu(&menu_data);
					}
#endif
               }
               SCI_FREE(contact_ptr);
            }
            else
            {
                AddCheckItemToOperateList(win_id, list_ctr_id, FALSE);
                if(applet_ptr->instance.selelect_callback != PNULL)
                {
                    applet_ptr->instance.selelect_callback(applet_ptr->instance.handle);
                }
                //else
                {
                    MMK_CloseApplet(applet_handle);
                }
            }
        }

        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_NOTIFY_T notify = {0};
            //uint16       current_index = 0;
            
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)
            if (MMIPB_TOOLBAR_LISTBOX_CTRL_ID == notify.src_id)
            {
                PB_OPTMENU_NODE_E      node_id = 0;
                
                MMI_HANDLE_T hToolbar =  MMK_GetCtrlHandleByWin(win_id,MMIPB_TOOLBAR_LISTBOX_CTRL_ID); 
                node_id = (PB_OPTMENU_NODE_E)GUITOOLBAR_GetItemUserDate(hToolbar, GUITOOLBAR_GetClickItemIndex(hToolbar));
                HandleToolBarAction(win_id, list_ctr_id,node_id, PNULL,FALSE);
                break;                
            }
            else 
#endif
            {
#if defined (TOUCH_PANEL_SUPPORT)
		    DealWithTPButton(win_id, list_ctr_id, (PB_CONTROL_ID_E)((MMI_NOTIFY_T*)param)->src_id);
		    MMK_SetAtvCtrl(win_id, list_ctr_id);
#endif
            }
          }
          break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527


    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //use finished select record or records or user open a record in search list
        if(MMIPB_LIST_RADIO == applet_ptr->instance.list_type)
        {
            GUILIST_SetSelectedItem(list_ctr_id, GUILIST_GetCurItemIndex(list_ctr_id), TRUE);
            //DealWithDoneAndOption(win_id,ctrl_id);
            break;
        }
        if(list_ctr_id != MMK_GetActiveCtrl(win_id))
        {
            break;
        }
		if(MMIPB_LIST_NORMAL_MAIN != applet_ptr->instance.list_type)
		{
            uint16 entry_id = 0;
            uint16 index    = 0;
            uint16 storage_id = 0;
            uint16 num_index = 0;
            GUILIST_ITEM_T* item_ptr = PNULL;
			cur_index = GUILIST_GetCurItemIndex(list_ctr_id);

            item_ptr = GUILIST_GetItemPtrByIndex(list_ctr_id, cur_index);/*lint !e605*/
            if(item_ptr == PNULL)
            {
                break;
            }
            index = MMIPB_CONTACTINDEX(item_ptr->user_data);
            num_index = MMIPB_NUMINDEX(item_ptr->user_data);
#ifdef MMIPB_MOST_USED_SUPPORT 
            if(list_handle->group_id == PB_GROUP_MOSTUSED)
            {
                MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node_info= {0};
                mostused_contact_node_info = MMIPB_GetMostUsedContactBaseInfo(index);
                entry_id = mostused_contact_node_info.entry_id;
                storage_id =   mostused_contact_node_info.storage_id;
            }
            else
#endif
            {                
                MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};
                contact_node_info = MMIPB_GetContactBaseInfo(index);
                entry_id = contact_node_info.contact_id;
                storage_id = contact_node_info.storage_id;
                
            }

			GUILIST_SetSelectedItem(list_ctr_id, cur_index, 
			(!GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(list_ctr_id, cur_index), GUIITEM_STATE_SELECTED))
			); /*lint !e730*/
			GUILIST_ReplaceItem(list_ctr_id, GUILIST_GetItemPtrByIndex(list_ctr_id, cur_index), cur_index);
			{
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}
		}
  

        if(GUILIST_GetTotalItemNum(list_ctr_id) != 0 && 
            (MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
             && (GUILIST_GetSelectedItemIndex(list_ctr_id, PNULL, 0) == 0))
        {
            // 进入显示查找详情的窗口
            uint32 user_data = 0;
            uint16 entry_id =0;
            uint16 storage_id =0;
            cur_index = GUILIST_GetCurItemIndex(list_ctr_id);
            GUILIST_GetItemData(list_ctr_id, cur_index, &user_data);
            MMIPB_GetContactIDandStorageID(applet_ptr->instance.cur_group_id,
                                           MMIPB_CONTACTINDEX(user_data),
                                           &entry_id,
                                           &storage_id);
            MMIPB_OpenContactWin(applet_ptr->caf_applet.app_handle, entry_id, storage_id);
        }
        break;

	case MSG_CTL_LIST_MARK_ITEM:
        HandleMarkItem(win_id, list_ctr_id, list_handle->group_id,param);
 		break;
        
    case MSG_KEYUP_GREEN:
#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
#endif        
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        //user make a call by press or long press green key
        if(list_ctr_id != MMK_GetActiveCtrl(win_id))
        {
            break;
        }
        if(GUILIST_GetSelectedItemIndex(list_ctr_id, PNULL, 0) < 2)
        {
		int32 index_of_num = 0;
            //uint16            current_index  = 0;	
            MMIPB_CONTACT_T *contact_info = PNULL; 
            MMIPB_CALL_TYPE_E    interface_type = MMIPB_CALL_TYPE_MAKECALL_AUDIO;
            uint8             num_str[MMIPB_MAX_STRING_LEN + 2] = {0};
            MMI_STRING_T		name_str={0};
//#ifdef MMI_MULTI_GREEN_KEY_SINGLE
//            CC_CALL_SIM_TYPE_E call_sim_type = CC_MAJOR_SIM_CALL;
//#else
//            CC_CALL_SIM_TYPE_E call_sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
//#endif
            total_count = GUILIST_GetTotalItemNum(list_ctr_id);
            if(total_count != 0 && (MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type))
            {
                interface_type = MMIPB_CALL_TYPE_MAKECALL_AUDIO;

#ifdef VT_SUPPORT
                if (MSG_KEYLONG_GREEN == msg_id)
                {
                    interface_type = MMIPB_CALL_TYPE_MAKECALL_VIDEO;//@zhaohui add
                }
#endif 
                contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
                SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
                MMIPB_GetContactInfoFromList(win_id, list_ctr_id, contact_info);
                
                name_str.wstr_ptr = contact_info->name.wstr;
				name_str.wstr_len = contact_info->name.wstr_len; 
                
        		index_of_num = MMIPB_GetFirstValidNumberIndex(contact_info->number , MMIPB_MAX_NV_PHONE_NUM);
        		if(index_of_num >=0 && index_of_num < MMIPB_MAX_NV_PHONE_NUM)
        		{
					MMIPB_BCDNumberToUTF8(&contact_info->number[index_of_num], MMIPB_MAX_STRING_LEN, num_str);

					//modify for bug80154
					MMIPB_CallNumber(MN_DUAL_SYS_MAX, //GetDualSysFromGreenKey(msg_id),
						interface_type,
						num_str,
						&name_str);
        		}  
                SCI_FREE(contact_info);      
            }
        }
        break;      

    case MSG_CLOSE_WINDOW:
		list_handle = MMK_GetWinAddDataPtr(win_id);
		if(PNULL== list_handle)
		{
			break;
		}

        applet_ptr->instance.is_subStrSearch = FALSE;

        MMK_CloseWin(MMK_GetWinHandle(applet_handle,MMIPB_WAITING_ALERT_WIN_ID));
        MMK_CloseWin(MMK_GetWinHandle(applet_handle,MMIPB_PROGRESS_WIN_ID));
        MMIPUB_CloseProgressWin(PNULL);
		MMK_FreeWinAddData(win_id);
        break;
#ifdef MMIPB_TIPS_OPEN
    case MSG_CTL_TIPS_NEED_DATA:
        ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LISTBOX_CTRL_ID);
        if (MMIPB_ONE_LINE_TIPS == applet_ptr->instance.display_mode )
        {
            SetTipsData(list_ctr_id, param);
        }
        break;
#endif
#ifdef TOUCH_PANEL_SUPPORT
#if defined(MMIPB_TOOLBAR_SUPPORT) && defined(GUIF_TOOLBAR)        
    case MSG_CTL_LIST_SLIDE_STATE_CHANGE:
        {
            list_handle = MMK_GetWinAddDataPtr(win_id);
            
            if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type
                && PNULL != list_handle
                && MMIPB_ALL_GROUP_CONTACTS == list_handle->contacts_type)
            {
                GUILIST_SLIDE_STATE_E  state = *(GUILIST_SLIDE_STATE_E*)param;
                if (GUILIST_START_SLIDE == state)
                {
                    GUITOOLBAR_SetVisible( MMK_GetCtrlHandleByWin(win_id, MMIPB_TOOLBAR_LISTBOX_CTRL_ID), FALSE);
                }

                else if (GUILIST_END_SLIDE == state)
                {
                    UpdatePbListWinToolbar(win_id, list_ctr_id);
                }

            }
        }
       break;
#endif
#endif
    default:
        result = FALSE;
        break;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : 取得所以选择的记录个数，不区分group
//  Global resource dependence : s_mmipb_uuid_list
//  Author:
//  Return: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllSelectNum( 
							       MMI_HANDLE_T applet_handle
								   )
{
    uint16 count =0;
     MMIPB_APPLET_T* applet_ptr = PNULL;
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL != applet_ptr)
    {
        count =  MMIPB_GetSelectCount(applet_ptr->instance.handle);
    }

    return count;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note:  OUT :contact_ptr
//          IN win_id
//          IN ctrl_id
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_GetContactInfoFromList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIPB_CONTACT_T* contact_ptr)
{
    uint16 current_index = 0;
    uint16 num_index = 0;
    uint16 entry_id = 0;
	uint16 contact_index = 0;
    uint16 storage_id = 0;
    GUILIST_ITEM_T* list_item_ptr =PNULL;
    MMIPB_APPLET_T* applet_ptr =  (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
#ifdef MMIPB_MOST_USED_SUPPORT 
	MMI_STRING_T		name_string_t = {0};
#endif

    if((PNULL == applet_ptr) || (PNULL == contact_ptr))
    {
        //SCI_TRACE_LOW:"[mmipb MMIPB_GetContactInfoFromListIndex] PNULL == applet_ptr || PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_10565_112_2_18_2_44_36_362,(uint8*)"");
        return FALSE;
    }

    if(MMIPB_LIST_NORMAL_MAIN == applet_ptr->instance.list_type)
    {

        if(!GUILIST_GetSelectedItemIndex(ctrl_id, &current_index, 1))
        {
            current_index = GUILIST_GetCurItemIndex(ctrl_id);
        }

        list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, current_index);/*lint !e605*/
        if(PNULL  == list_item_ptr)
        {
            //SCI_TRACE_LOW:"[mmipb MMIPB_GetContactInfoFromListIndex] PNULL  == list_item_ptr %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_10580_112_2_18_2_44_36_363,(uint8*)"d", current_index);
            return FALSE;
        }

        num_index = MMIPB_NUMINDEX(list_item_ptr->user_data);
		contact_index = MMIPB_CONTACTINDEX(list_item_ptr->user_data);
#ifdef MMIPB_MOST_USED_SUPPORT
        if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
        {
            MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node = {0};
            mostused_contact_node = MMIPB_GetMostUsedContactBaseInfo(contact_index);
            entry_id = mostused_contact_node.entry_id;
            storage_id =mostused_contact_node.storage_id;

        }
        else
#endif
        {

            MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
            contact_node = MMIPB_GetContactBaseInfo(contact_index);
            entry_id = contact_node.contact_id;
            storage_id =contact_node.storage_id;
        }

        MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
        applet_ptr->instance.cur_entry_id = entry_id;
        applet_ptr->instance.cur_storage_id = storage_id;
        applet_ptr->instance.cur_num_index =  num_index;     
		//Bug 150153    
#ifdef MMIPB_MOST_USED_SUPPORT 
 		//Get name
		if(PB_GROUP_MOSTUSED == applet_ptr->instance.cur_group_id)
		{
			name_string_t.wstr_ptr = contact_ptr->name.wstr;	
			if(MMIPB_GetNameByNumber(&(contact_ptr->number[0]), &name_string_t, MMIPB_NAME_MAX_LEN, PNULL,PNULL,PNULL,FALSE, FALSE))
			{
				contact_ptr->name.wstr_len = name_string_t.wstr_len;
			}
		}
#endif
    }
    return TRUE;      
}


#if defined (TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DealWithTPButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, PB_CONTROL_ID_E button_id)
{
    
    //uint32 total_count = 0;

    //MMIPB_LIST_HANDLE* list_handle = PNULL;
    MMIPB_APPLET_T* applet_ptr =  (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

    if(PNULL ==  applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] DealWithTPButton PNULL ==  applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_10627_112_2_18_2_44_36_364,(uint8*)"");
        return;
    }

	// 从列表控件获得已经选择的列表信息
    if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
    {
        if(MMIPB_LIST_RADIO == applet_ptr->instance.list_type)	
        {
            uint16 entry_id = 0;
            uint16 index    = 0;
            uint16 cur_index= 0;
            uint16 storage_id = 0;
            uint16 num_index = 0;
            MMIPB_SEL_DATA sel_data = {0};
            GUILIST_ITEM_T* item_ptr = PNULL;
    
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);/*lint !e605*/
            if(item_ptr == PNULL)
            {
                return;
            }
            index = MMIPB_CONTACTINDEX(item_ptr->user_data);
            num_index = MMIPB_NUMINDEX(item_ptr->user_data);
#ifdef MMIPB_MOST_USED_SUPPORT
            if(applet_ptr->instance.cur_group_id == PB_GROUP_MOSTUSED)
            {
                                MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node_info= {0};
                mostused_contact_node_info = MMIPB_GetMostUsedContactBaseInfo(index);
                entry_id = mostused_contact_node_info.entry_id;
                storage_id =   mostused_contact_node_info.storage_id;

            }
            else
#endif  
            {

                MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};
                contact_node_info = MMIPB_GetContactBaseInfo(index);
                entry_id = contact_node_info.contact_id;
                storage_id = contact_node_info.storage_id;
            }
  
            MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);//  单选框，只能选择当前的一个

            sel_data.entry_id = entry_id;
            sel_data.storage_id = storage_id;
            sel_data.num_index = num_index;
            if(MMIPB_LIST_FIELD_WHOLE == applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])
            {
                sel_data.sel_field = MMIPB_FEILD_FLAG_MASK_ALL;
            }
            else 
            {
                sel_data.sel_field =  MMIPB_FEILD_MASK_SINGLE_NUMBER << num_index;
            }
            MMIPB_CreateSelNode(applet_ptr->instance.handle,&sel_data);  
        }
        else if(MMIPB_LIST_CHECK == applet_ptr->instance.list_type)
        {
#ifndef MMI_PDA_SUPPORT
            //check or uncheck
            MMK_SendMsg(win_id, MSG_APP_WEB, NULL);
#endif
        }
        
        else if (GUILIST_GetTotalItemNum(ctrl_id) != 0 )
        {
            // 进入显示查找详情的窗口
            uint32 user_data = 0;
            uint16 entry_id = 0;
            uint16 storage_id = 0;

            GUILIST_GetItemData(ctrl_id,
                               GUILIST_GetCurItemIndex(ctrl_id),
                               &user_data); 
            MMIPB_GetContactIDandStorageID(applet_ptr->instance.cur_group_id,
                                           MMIPB_CONTACTINDEX(user_data),
                                           &entry_id,
                                           &storage_id);
            
            MMIPB_OpenContactWin(applet_ptr->caf_applet.app_handle,  entry_id, storage_id);
        }
	}
    //@maggie modify for cr73275
}
#endif

/*****************************************************************************/
//  Description : 打开pb应用的主页面
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN MMIPB_OpenPbWin(MMI_HANDLE_T  applet_handle)
{
    
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIPB_APPLET_T* applet_ptr = PNULL;
    BOOLEAN result = FALSE;

    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(applet_ptr != PNULL)
    {
        applet_ptr->instance.cur_group_id = PB_GROUP_ALL;
        
        if(applet_ptr->instance.function_type == MMIPB_SIGNLE_SELECT_LIST)
        {
            MMIPB_OpenSingleSelWin(applet_handle);
            result = TRUE;
        }
        else
        {
            
            if(MMK_CreateWinByApplet(applet_handle,MMIPB_MAIN_WIN_TAB, PNULL))/*lint !e605*/
            {
                PbCreateChildWin(applet_handle,MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), applet_ptr->instance.app_type);
                result = TRUE;
            }
        }
    }

    return result;

}
#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开fdn的列表窗口
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSIMDNListWin(MMI_HANDLE_T  applet_handle)
{

   // MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIPB_APPLET_T* applet_ptr = PNULL;
    BOOLEAN result = FALSE;
    MMIPB_SIM_NUMBER_LIST_INFO_T *dail_num_list_ptr = PNULL;
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(applet_ptr != PNULL)
    {
        dail_num_list_ptr = SCI_ALLOCA(sizeof(MMIPB_SIM_NUMBER_LIST_INFO_T));
        if(dail_num_list_ptr != PNULL)
        {
            dail_num_list_ptr->dual_sys = applet_ptr->instance.dual_sys;
            dail_num_list_ptr->sim_contact_type = applet_ptr->instance.sim_contact_type;
            dail_num_list_ptr->is_need_pin_verify = applet_ptr->instance.is_need_pin_verify ;
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
            dail_num_list_ptr->is_enter_from_callsetting = applet_ptr->instance.is_enter_from_callsetting;
#endif
            MMK_CreateWinByApplet(applet_handle,
                (uint32 *)MMIPB_SIMDN_LIST_WIN_TAB,
                (ADD_DATA)dail_num_list_ptr
                );
            result = TRUE;
        }

        
    }

    return result;

}
#endif
/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPB_AbortPB(FALSE);
        break;
    //case MSG_APP_RED:   // 不响应红键
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //SCI_TRACE_LOW:"[MMIPB]HandleWaitWinMsg red key pressed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_10829_112_2_18_2_44_36_365,(uint8*)"");
        MMIPB_AbortPB(TRUE);
	    MMK_CloseWin(win_id);	
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenWaitWin(MMI_HANDLE_T applet_handle, MMI_TEXT_ID_T text_id)
{
    MMI_STRING_T    prompt_str = {0};

    MMI_GetLabelTextByLang(text_id, &prompt_str);

    MMIPUB_OpenWaitWinEx(applet_handle, 1,&prompt_str,PNULL,PNULL,MMIPB_WAITING_ALERT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleWaitWinMsg, PNULL);
}

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    MMIPUB_INFO_T* win_info_ptr =(MMIPUB_INFO_T*)MMIPUB_GetWinAddDataPtr(win_id);
    MMI_TEXT_ID_T title_id   = TXT_NULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPB_AbortPB(FALSE);
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);	
        if(PNULL!= win_info_ptr)
        {
            title_id = (MMI_TEXT_ID_T)win_info_ptr->user_data;
        }        
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        MMIPUB_SetWinTitleTextId(win_id,title_id,FALSE);
        break;  
    case MSG_APP_CANCEL: 
         MMIPB_AbortPB(TRUE);
	     MMK_CloseWin(win_id);

        break;


    case MSG_CLOSE_WINDOW:
       
	  //  MMIPB_UNMarkAll(applet_handle);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
// 	Description : 开启进度窗口
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC void  MMIPB_OpenProcessWin(MMI_HANDLE_T  applet_handle, MMI_TEXT_ID_T text_id, MMI_TEXT_ID_T title_id)
{
    MMI_WIN_ID_T alert_id = MMIPB_PROGRESS_WIN_ID; 
    MMI_HANDLE_T  alert_handle = NULL;
    uint16        oper_count = 0;
    MMI_STRING_T  text1_str ={0};
    MMIPB_APPLET_T*   applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_OpenProcessWin PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_10918_112_2_18_2_44_36_366,(uint8*)"");
        return;
    }

    oper_count =  MMIPB_GetSelectCount(applet_ptr->instance.handle);
    MMI_GetLabelTextByLang(text_id, &text1_str);
    MMIPUB_OpenProgressWinEx(applet_handle, &text1_str, &alert_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleProcessWinMsg);
    alert_handle = MMK_GetWinHandle(applet_handle, alert_id);
    MMIPUB_SetWinAddData(alert_handle, title_id);
    MMIPUB_SetProgressTotalSize(&alert_handle,oper_count);
}

/*****************************************************************************/
// 	Description : 开启进度窗口
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/

PUBLIC void  MMIPB_CloseProcessWin(MMI_HANDLE_T  applet_handle)
{
    MMI_WIN_ID_T alert_id = MMK_GetWinHandle(applet_handle,MMIPB_WAITING_ALERT_WIN_ID);
    MMI_WIN_ID_T progress_id = MMK_GetWinHandle(applet_handle,MMIPB_PROGRESS_WIN_ID); 
    MMK_CloseWin(alert_id);
    MMIPUB_CloseProgressWin(&progress_id); 
}

/*****************************************************************************/
// 	Description : 处理操作回传结果后的刷新,同时关闭applet
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:这个callback是为非主窗口使用，主窗口操作后，不需要关闭applet，不能使用该callback
/*****************************************************************************/
LOCAL void UpdateCallbackWithCloseApplet(MMI_HANDLE_T applet_handle, uint16  msg_id, MMIPB_ERROR_E error,MMIPB_OPC_E opc_type, uint16 count)
{
   MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
   if(applet_ptr != PNULL)
   {
       MMIPB_CloseApplet(applet_ptr->caf_applet.app_handle);
   }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_OpenAlertTipWin(MMI_HANDLE_T applet_handle, MMIPB_ERROR_E error,MMIPB_OPC_E opc_type)
{
    MMI_TEXT_ID_T  text_id= TXT_NULL;
    MMI_TEXT_ID_T  title_id= TXT_NULL;
    MMI_WIN_ID_T alert_id = MMIPB_ALERT_INFO_WIN_ID; 
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;
    SCI_TRACE_LOW("[MMIPB] MMIPB_OpenAlertTipWin error %d,opc_type %d ", error,opc_type);
    
    if(MMIPB_ERROR_NO_SPACE == error)
    {
        text_id = TXT_NO_SPACE;
    }
    else if(MMIPB_ERROR_FMM_NO_SPACE == error)
    {
        text_id = TXT_NO_SPACE;
    }
    else if(MMIPB_ERROR_ERROR == error)
    {
        text_id = TXT_ERROR;
    }
    else if(MMIPB_ERROR_PHONE_NO_SPACE == error)
    {
        text_id = TXT_PB_NV_FULL;
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else if(MMIPB_ERROR_SIM1_NO_SPACE == error)
    {
        text_id = TXT_PB_SIM1_FULL;
    }
    else if(MMIPB_ERROR_SIM2_NO_SPACE == error)
    {
        text_id = TXT_PB_SIM2_FULL;
    }
    else if(MMIPB_ERROR_SIM3_NO_SPACE == error)
    {
        text_id = TXT_PB_SIM3_FULL;
    }
    else if(MMIPB_ERROR_SIM4_NO_SPACE == error)
    {
        text_id = TXT_PB_SIM4_FULL;
    }
#else
    else if(MMIPB_ERROR_SIM1_NO_SPACE == error)
    {
        text_id = TXT_PB_SIM_FULL;
    }
#endif

#ifdef FDN_SUPPORT    
    else if(MMIPB_ERROR_FDN_ENABLE == error)
    {
        text_id = TXT_FDN_ACTIVATED;
    }
#endif
    else if(MMIPB_ERROR_CANCEL == error)
    {
         text_id = TXT_CANCELED;
    }
    else if(MMIPB_ERROR_MAX_LIMIT == error)
    {
        text_id = TXT_PB_OUT_OF_MAX_NUM;
    }
    else if(MMIPB_ERROR_NO_SELECT_ENTRY == error)
    {
         text_id = TXT_PLS_SELECT_ONE_ITEM;
    }
    else if(MMIPB_ERROR_SUCCESS == error)
    {
         text_id = TXT_SUCCESS;
         image_id = IMAGE_PUBWIN_SUCCESS;
    }
    if( MMIPB_OPC_ADD== opc_type)
    {
        title_id = TXT_SAVE_TO_NEWCONTACT;
    }
    else if(MMIPB_OPC_DELETE == opc_type)
    {
        title_id = TXT_PB_DELETE_CONTACT;
    }
    else if( MMIPB_OPC_EDIT== opc_type)
    {
        title_id = TXT_PB_EDIT_CONTACT;
    }
    else if(MMIPB_OPC_COPY == opc_type)
    {
        title_id = TXT_PB_COPY_CONTACT;
    }
    else if(MMIPB_OPC_IMPORT == opc_type)
    {
        title_id = TXT_PB_IMPORT_CONTACT;
    }
    else if(MMIPB_OPC_EXPORT == opc_type)
    {
        title_id = TXT_PB_EXPORT_CONTACT;
    }
    else if( MMIPB_OPC_ADD_FAVOURITE== opc_type)
    {
        title_id = TXT_PB_ADD_FAVOURITE_CONTACT;
    }
#ifdef  SNS_SUPPORT
    else if(MMIPB_OPC_GET_SNS == opc_type)
    {
        title_id = TXT_PB_SNS_UPDATE;
    }
#endif
    MMIPUB_OpenAlertWinByTextIdEx(applet_handle,PNULL,text_id,TXT_NULL,image_id,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,0);
    MMIPUB_SetWinTitleTextId(MMK_GetWinHandle(applet_handle,alert_id),title_id,TRUE);
  // MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

    
}
/*****************************************************************************/
// 	Description : 处理操作回传结果后的刷新和提示操作
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
LOCAL void s_mmipb_update_callback(MMI_HANDLE_T applet_handle,  uint16  msg_id, MMIPB_ERROR_E error,MMIPB_OPC_E opc_type,uint16 count)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    MMI_WIN_ID_T alert_id = MMK_GetWinHandle(applet_handle,MMIPB_WAITING_ALERT_WIN_ID); 
    MMI_WIN_ID_T progress_id = MMK_GetWinHandle(applet_handle,MMIPB_PROGRESS_WIN_ID); 
    //MMI_TEXT_ID_T  text_id= TXT_NULL;

    if(MMIPB_OPC_ADD == opc_type || MMIPB_OPC_EDIT == opc_type)
    {
        //close add or edit window
        MMK_CloseWin(MMK_GetWinHandle(applet_handle, MMIPB_ADD_IN_NV_WIN_ID));
    }
    switch(msg_id)
    {
    case MMIPB_OPERATE_SUCCESS:
        if(opc_type!=MMIPB_OPC_ADD_MOSTUSED)
        {
            MMK_CloseWin(alert_id);
            MMIPUB_CloseProgressWin(&progress_id);
        }
        
         MMIPB_UpdateCurrentList(applet_handle, opc_type);
#ifdef  SNS_SUPPORT
         if(MMIPB_OPC_GET_SNS == opc_type)
         {
            MMIPB_OpenAlertTipWin(applet_handle, MMIPB_ERROR_SUCCESS,opc_type);
         }
         else if(MMIPB_OPC_UPDATE_SNS_ACTIVITY == opc_type)
         {
             SCI_TRACE_LOW("[MMIPB]s_mmipb_update_callback MMIPB_OPC_UPDATE_SNS_ACTIVITY "); 
             if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_SNS_ACTIVEINFO_WIN_ID)))
             {
                MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_SNS_ACTIVEINFO_WIN_ID),MMI_PB_SNS_DETAIL_CHANGED,PNULL);
             }
         }
#endif
        break;
    case MMIPB_OPERATE_FAIL:
        MMK_CloseWin(alert_id);
        MMIPUB_CloseProgressWin(&progress_id);
        MMIPB_OpenAlertTipWin(applet_handle,error, opc_type);
        MMIPB_UpdateCurrentList(applet_handle, opc_type);
        break;



    case MMIPB_OPERATE_UPDATE:
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_PROGRESS_WIN_ID)))
        {
            MMIPUB_UpdateProgressBarEx(&progress_id, count, TRUE );
        }
        break;

    case MMIPB_OPERATE_PARTLY_SUCCESS:
         MMK_CloseWin(alert_id);
         MMIPUB_CloseProgressWin(&progress_id);
         if((error>=MMIPB_ERROR_NO_SPACE) &&(error<= MMIPB_ERROR_FMM_NO_SPACE))
         {
            MMIPB_OpenAlertTipWin(applet_handle,error, opc_type);
         }
         MMIPB_UpdateCurrentList(applet_handle, opc_type);
        break;

    default:
        break;
    }

    if(PNULL != applet_ptr)  //+ for coverity
    {
        if((applet_ptr->instance.function_type != MMIPB_OPEN_MAIN_LIST))
        {
            MMIPB_CloseApplet(applet_ptr->caf_applet.app_handle);
        }   
    }

}

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
)
{
    MMIPB_HANDLE_T     applet_handle = NULL;
    uint32             i = 0;
    MMIPB_INSTANCE_T   *pb_instance_ptr = PNULL;
	
	//+ Bug 132534 
	if((MMIPB_LIST_NORMAL_MAIN == list_type) && (MMK_IsOpenWin(MMIPB_EDITBOX_WIN_ID)))
	{
		MMK_CloseWin(MMIPB_EDITBOX_WIN_ID);
	}
	//- Bug 132534 

    if(MMIPB_IsPermitOpen(list_type))
    {
        pb_instance_ptr = SCI_ALLOCA(sizeof(MMIPB_INSTANCE_T));
        if(pb_instance_ptr != PNULL)
        {
            SCI_MEMSET(pb_instance_ptr, 0X00, sizeof(MMIPB_INSTANCE_T));
            pb_instance_ptr->app_type = app_type;
            if(app_type == MMIPB_ALL_CONTACT)
            {
                pb_instance_ptr->is_need_protect_verify = 1;
            }
            pb_instance_ptr->list_type = list_type;
            for(i = 0; i < MMIPB_TAB_MAX_NUM; i++)
            {
                if(i == app_type)
                {
                    pb_instance_ptr->field_disp_type[i] = field_display_type;
                }
                else
                {
                    if(MMIPB_ALL_CONTACT == i 
#ifdef MMIPB_MOST_USED_SUPPORT
                        || MMIPB_MOSTUSED_CONTACT == i
#endif
                        || MMIPB_GROUP_CONTACT == i
                        )
                    {
                        //pb list
                        pb_instance_ptr->field_disp_type[i] = MMIPB_LIST_FIELD_WHOLE;
                    }
#ifdef MMI_PDA_SUPPORT
                    else if(MMIPB_DIAL_APPLICATION == i)
                    {
                        //dial
                        pb_instance_ptr->field_disp_type[i] = MMIPB_LIST_FIELD_NUMBER;
                    }
                    else if(MMIPB_CALLLOG_APPLICATION == i)
                    {
                        //call log
                        pb_instance_ptr->field_disp_type[i] = MMIPB_LIST_FIELD_NUMBER;
                    }
#endif
                }
            }
            pb_instance_ptr->search_type = search_type;
            pb_instance_ptr->max_select_count = max_select_count;
            if(pb_instance_ptr->list_type == MMIPB_LIST_RADIO)
            {
                pb_instance_ptr->function_type = MMIPB_SIGNLE_SELECT_LIST;
            }
            else if(pb_instance_ptr->list_type == MMIPB_LIST_CHECK)
            {
                pb_instance_ptr->function_type = MMIPB_MUTI_SELECT_LIST;
            }
            else
            {
                
                pb_instance_ptr->function_type = MMIPB_OPEN_MAIN_LIST;
            }
            
            pb_instance_ptr->entry_func = MMIPB_OpenPbWin;
            if((PNULL !=str_input) &&(str_input->wstr_len >0))
            {
                MMIAPICOM_Wstrncpy(pb_instance_ptr->sub_search_str, str_input->wstr_ptr, str_input->wstr_len);
            }
            pb_instance_ptr->selelect_callback = callback;
            pb_instance_ptr->update_callback = s_mmipb_update_callback;
                        
            applet_handle = MMIPB_StartApplet(pb_instance_ptr);
            SCI_FREE(pb_instance_ptr);
            return TRUE;
        }
        return FALSE;       
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_OpenMainWin not allowed to open !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11193_112_2_18_2_44_37_367,(uint8*)"");
        return FALSE;
    }
}
#if defined(FDN_SUPPORT)
/*****************************************************************************/
// 	Description : 进入fdn 联系人列表窗口
//	Global resource dependence : none
//  Author: mary.xiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenFDNContactList(MN_DUAL_SYS_E dual_sys)
{
    MMIPB_INSTANCE_T *instance_ptr = PNULL;

    instance_ptr = SCI_ALLOCA(sizeof(MMIPB_INSTANCE_T));
    if(instance_ptr != PNULL)
    {
        
        SCI_MEMSET(instance_ptr, 0X00, sizeof(MMIPB_INSTANCE_T));
        
        instance_ptr->app_type = MMIPB_ALL_CONTACT;
        instance_ptr->list_type = MMIPB_LIST_NORMAL_MAIN;
        instance_ptr->field_disp_type[MMIPB_ALL_CONTACT] = MMIPB_LIST_FIELD_WHOLE;
        instance_ptr->search_type = MMIPB_SEARCH_NONE;
        instance_ptr->max_select_count = 1;  
        instance_ptr->function_type = MMIPB_OPEN_MAIN_LIST;
        instance_ptr->contacts_type = MMIPB_SIM_DN_CONTACTS;
        instance_ptr->sim_contact_type = PHONEBOOK_FDN_STORAGE;
        instance_ptr->entry_func = OpenSIMDNListWin;
        instance_ptr->selelect_callback = PNULL;
        instance_ptr->is_need_pin_verify = TRUE;
        instance_ptr->update_callback = s_mmipb_update_callback;
        instance_ptr->dual_sys = dual_sys;
        MMIPB_StartApplet(instance_ptr);
        SCI_FREE(instance_ptr);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
   
}
#endif

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开本机号码设置窗口
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenMSISDNWin(MMI_HANDLE_T applet_handle, BOOLEAN is_enter_from_callsetting)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    MMIPB_INSTANCE_T *instance_ptr = PNULL;
    if(PNULL ==applet_handle)
    {
        //创建applet                
        instance_ptr = SCI_ALLOCA(sizeof(MMIPB_INSTANCE_T));
        if(instance_ptr == PNULL)
        {
            return FALSE;
        }
        SCI_MEMSET(instance_ptr, 0X00, sizeof(MMIPB_INSTANCE_T));
        instance_ptr->app_type = MMIPB_ALL_CONTACT;
        instance_ptr->list_type = MMIPB_LIST_NORMAL_MAIN;
        instance_ptr->field_disp_type[MMIPB_ALL_CONTACT] = MMIPB_LIST_FIELD_WHOLE;
        instance_ptr->search_type = MMIPB_SEARCH_NONE;
        instance_ptr->max_select_count = 1;  
        instance_ptr->function_type = MMIPB_OPEN_MAIN_LIST;
        instance_ptr->contacts_type = MMIPB_SIM_DN_CONTACTS;
        instance_ptr->sim_contact_type = PHONEBOOK_MSISDN_STORAGE;
        instance_ptr->is_need_pin_verify = FALSE;
        instance_ptr->entry_func = OpenSIMDNListWin;
        instance_ptr->selelect_callback = PNULL;
        instance_ptr->update_callback = s_mmipb_update_callback;
        instance_ptr->dual_sys = MMI_DUAL_SYS_MAX;
    #if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
        instance_ptr->is_enter_from_callsetting = is_enter_from_callsetting;
    #endif
        MMIPB_StartApplet(instance_ptr);
        SCI_FREE(instance_ptr);
    }
    else if(PNULL != applet_ptr)
    {
        applet_ptr->instance.contacts_type = MMIPB_SIM_DN_CONTACTS;
        applet_ptr->instance.is_need_pin_verify = FALSE;
        applet_ptr->instance.sim_contact_type = PHONEBOOK_MSISDN_STORAGE;
        applet_ptr->instance.dual_sys = MMI_DUAL_SYS_MAX;//需要列出所有卡的记录
    #if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
        applet_ptr->instance.is_enter_from_callsetting = is_enter_from_callsetting;
    #endif
        //MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32*)MMIPB_MSISDN_WIN_TAB, (ADD_DATA)FALSE);
        OpenSIMDNListWin(applet_handle);
    }
    return TRUE;
}
#endif

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开SDN 号码设置窗口
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenSDNWin(MMI_HANDLE_T applet_handle)
{
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
#ifndef CMCC_UI_STYLE
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
    BOOLEAN       is_support_sdn = FALSE;
    MMIPB_INSTANCE_T *instance_ptr = PNULL;

#ifndef CMCC_UI_STYLE
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        if(MNSIM_GetSdnStatusEx(dual_sys))
        {
            is_support_sdn = TRUE;
            break;
        }
    }    
#endif

    if(!is_support_sdn)/*lint !e774*/
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PB_SDN_NOT_SUPPORT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    if(PNULL ==applet_handle)
    {
        //创建applet
        instance_ptr = SCI_ALLOCA(sizeof(MMIPB_INSTANCE_T));
        if(instance_ptr == PNULL)
        {
            return FALSE;
        }
        SCI_MEMSET(instance_ptr, 0X00, sizeof(MMIPB_INSTANCE_T));
        instance_ptr->app_type = MMIPB_ALL_CONTACT;
        instance_ptr->list_type = MMIPB_LIST_NORMAL_MAIN;
        instance_ptr->field_disp_type[MMIPB_ALL_CONTACT] = MMIPB_LIST_FIELD_WHOLE;
        instance_ptr->search_type = MMIPB_SEARCH_NONE;
        instance_ptr->max_select_count = 1;  
        instance_ptr->function_type = MMIPB_OPEN_MAIN_LIST;
        instance_ptr->contacts_type = MMIPB_SIM_DN_CONTACTS;
        instance_ptr->sim_contact_type = PHONEBOOK_SDN_STORAGE;
        instance_ptr->is_need_pin_verify = FALSE;
        instance_ptr->entry_func = OpenSIMDNListWin;
        instance_ptr->selelect_callback = PNULL;
        instance_ptr->update_callback = s_mmipb_update_callback;
        instance_ptr->dual_sys = MMI_DUAL_SYS_MAX;
        MMIPB_StartApplet(instance_ptr);
        SCI_FREE(instance_ptr);
    }
    else if(PNULL != applet_ptr)
    {
        applet_ptr->instance.contacts_type = MMIPB_SIM_DN_CONTACTS;
        applet_ptr->instance.is_need_pin_verify = FALSE;
        applet_ptr->instance.sim_contact_type = PHONEBOOK_SDN_STORAGE;
        applet_ptr->instance.dual_sys = MMI_DUAL_SYS_MAX;//需要列出所有卡的记录
        //MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, (uint32*)MMIPB_MSISDN_WIN_TAB, (ADD_DATA)FALSE);
        OpenSIMDNListWin(applet_handle);
    }
    return TRUE;
}
#endif

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : update FDN list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_UpdateSIMDNList(void)
{
    MMIPB_HANDLE_T applet_handle = 0;
    
    MMIPB_GetCurAppletHandle(&applet_handle);
    MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_FDN_LIST_WIN_ID), MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
	MMK_SendMsg(MMK_GetWinHandle(applet_handle,MMIPB_ENTRY_DETAILS_WIN_ID), MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, PNULL);
}
#endif
/*****************************************************************************/
//  Description : 打开窗口，显示PB全部记录
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_OpenListAllGroupWin(void)
{
    #ifdef PIM_SUPPORT //add by wangxiaolin 2010.04.07
    if(MMIPIM_IsSyncProcing())
    {
    	MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIM_UPDATE_ENTER_PB_PROMPT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    #endif
    
   // MMIPB_OpenNumberList(MMIPB_LIST_FOR_SEARCH, MMIPB_LIST_NORMAL_MAIN, MMIPB_SEARCH_NONE, 1,TRUE,PNULL);

    return MMIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_NORMAL_MAIN, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_WHOLE,
                        MMIPB_SEARCH_NONE,
                        1,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        PNULL
                        ); 

}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_IsUseDefaultName(wchar* name_ptr)
{
    MMI_STRING_T    default_str = {0};
    // 姓名为空
    MMI_GetLabelTextByLang(
                        TXT_NO_NAME, 
                        &default_str
                         );
    if(0 == MMIAPICOM_Wstrcmp(default_str.wstr_ptr, name_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SwitchToPreTab(void)
{
    MMI_HANDLE_T applet_handle = NULL;
    MMIPB_APPLET_T* applet_ptr = PNULL;
    if(MMIPB_GetCurAppletHandle(&applet_handle))
    {
        applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
        if(applet_ptr != PNULL)
        {
            SwitchToAppTab(applet_handle, applet_ptr->instance.app_type);
        }
    }
    
}
#endif

#if defined (MMI_PDA_SUPPORT) && defined (HOME_LOCATION_INQUIRE_SUPPORT)
/*****************************************************************************/
//  Description :CC_FindLocationInforl 
//  Global resource dependence : none
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetNumberLocation(wchar *str, //OUT:
							 const uint8* tel_num, //IN
							 uint8 tel_num_len //IN
							 )
{
    uint8   *origin_number_ptr = PNULL;
    int16   origin_num_len  = 0;
    uint8   ip_number[MMISET_IPNUMBER_MAX_LEN+1] = {0};
    uint8   ip_number_len = 0;   
	uint16  str_length = 0; 
	MMI_STRING_T default_str = {0};
    BOOLEAN  result = FALSE;
    
#ifdef MMI_IP_CALL_SUPPORT
	MMIAPISET_GetIPNumber(MN_DUAL_SYS_1, tel_num, ip_number, &ip_number_len);/*lint !e605*/
#endif
	if(ip_number_len > 0 &&(ip_number_len < tel_num_len) && (!strncmp((char *)tel_num, (char *)ip_number, ip_number_len)))
	{
		//exist ip number, without ip number
        origin_number_ptr = &tel_num[ip_number_len]; /*lint !e605*/
		origin_num_len = tel_num_len - ip_number_len;
	}
	else
	{
		origin_number_ptr = tel_num;    /*lint !e605*/
		origin_num_len = tel_num_len;
	}

#ifdef HOME_LOCATION_INQUIRE_SUPPORT    
    str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_ptr, origin_num_len, str);
    if( str_length == 0 )
    {
        MMI_GetLabelTextByLang(TXT_UNKNOWN, &default_str);
    }
    else if( str_length == MMIPHONESEARCH_LOCALNUMBER)
    {
        MMI_GetLabelTextByLang(TXT_LOCAL, &default_str);
    }
	if(default_str.wstr_len > 0)
	{
		MMIAPICOM_Wstrncpy( str, default_str.wstr_ptr, default_str.wstr_len );
    }
	result = TRUE;
#endif
	return result;
}
#endif

/*****************************************************************************/
//  Discription: open one group records
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void OpenGroupEntryList(MMI_HANDLE_T applet_handle,  uint16 group_id)
{
	MMIPB_LIST_HANDLE *group_infor_ptr = PNULL;
	
	group_infor_ptr = (MMIPB_LIST_HANDLE *)SCI_ALLOCA(sizeof(MMIPB_LIST_HANDLE));
	if(group_infor_ptr == PNULL)
	{
		//SCI_TRACE_LOW:"[MMIPB]PbCreateChildWin group_infor_ptr == PNULL !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11494_112_2_18_2_44_37_369,(uint8*)"");
		return;
	}
	SCI_MEMSET(group_infor_ptr, 0x00, sizeof(MMIPB_LIST_HANDLE));
	group_infor_ptr->group_id = group_id;
	group_infor_ptr->contacts_type = MMIPB_SINGLE_GROUP_CONTACTS;
	group_infor_ptr->win_id = MMIPB_GROUP_ENTRYL_LIST_WIN_ID;
#ifdef MMI_PDA_SUPPORT
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
		MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_ISTYLE_GROUP_ENTRYL_LIST_WIN_TAB, group_infor_ptr);
	}
	else
#endif		
	{
		MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_PDA_GROUP_ENTRYL_LIST_WIN_TAB, group_infor_ptr);/*lint !e605*/
	}
#else
    MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_GROUP_ENTRYL_LIST_WIN_TAB, group_infor_ptr);/*lint !e605*/
#endif
}

/*****************************************************************************/
//  Description : 获得号码对应的icon id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIPB_GetIconIdOfPhone
(
    uint8 num_type  //IN:
)
{
    MMI_IMAGE_ID_T icon_id = IMAGE_PHONEBOOK_PHONE;
    
    switch (num_type)
    {
	case MMIPB_UNKNOWN_NUMBER:
		icon_id = IMAGE_PHONEBOOK_MOBILE_NUMBER;
		break;
    case MMIPB_ADN_NUMBER:
        icon_id = IMAGE_PHONEBOOK_SELF_NUMBER;
        break;

    case MMIPB_HOME_NUMBER:
        icon_id = IMAGE_PHONEBOOK_HOME_NUMBER;
        break;

#ifndef PB_SUPPORT_LOW_MEMORY      
    case MMIPB_OFFICE_NUMBER:
        icon_id = IMAGE_PHONEBOOK_COMPANY_NUMBER;
        break;
   case MMIPB_FIXED_NUMBER:
        icon_id = IMAGE_PHONEBOOK_PHONE_NUMBER;
        break;
   case MMIPB_FAX_NUMBER:
        icon_id = IMAGE_PHONEBOOK_FAX_NUMBER;
        break;
#endif

    case MMIPB_FEILD_FLAG_MASK_MAIL:
#ifdef MMIPB_MAIL_SUPPORT
        icon_id = IMAGE_PHONEBOOK_MAIL_ADDRESS;
#endif
        break;
//         
//     case 0xff:
//         icon_id = IMAGE_PHONEBOOK_SIM;
//         break;

    default:
        icon_id = IMAGE_PHONEBOOK_PHONE;    //TEMP
        break;
    }

    return (icon_id);
}


/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的小图标
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetSearchEntryIcon(uint32 group)
{
    uint8 group_id = 0;
    MMIPB_GetAllGroupIDs(group, &group_id, 1);
    return GetImageIcon(group_id);
}



/*****************************************************************************/
//  Description : 通过win_id 取得对应窗口的list handle
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
//RETURN: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T MMIPB_GetListHandleByWin(MMI_WIN_ID_T win_id)
{
#ifdef MMI_PDA_SUPPORT
    MMI_HANDLE_T list_handle =MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID);
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T   applet_handle = NULL;

    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetListHandleByWin PNULL == applet_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11599_112_2_18_2_44_38_370,(uint8*)"");
        return list_handle;
    }
   applet_handle  = applet_ptr->caf_applet.app_handle;


    if(win_id == MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_TAB_WIN_ID))
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_ALL_ENTRY_LISTBOX_CTRL_ID);//

    }
    else if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_MOSTUSED_CONTACTS_LIST_TAB_WIN_ID))
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_MOSTUSED_ENTRY_LISTBOX_CTRL_ID);//
    }
    else if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_OPERATE_SEL_LIST_WIN_ID))
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID);//
    }

    else if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_GROUP_ENTRYL_LIST_WIN_ID))
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_GROUP_ENTRY_LISTBOX_CTRL_ID);
    }
#else
    MMI_HANDLE_T list_handle =NULL;
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T   applet_handle = NULL;
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetListHandleByWin PNULL == applet_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11629_112_2_18_2_44_38_371,(uint8*)"");
        return list_handle;
    }
    applet_handle  = applet_ptr->caf_applet.app_handle;
    if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_OPERATE_SEL_LIST_WIN_ID))
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID);//
    }
    else if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_GROUP_ENTRYL_LIST_WIN_ID))
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_SINGLE_GROUP_ENTRY_LISTBOX_CTRL_ID);
    }
#ifdef BT_PBAP_SUPPORT
    else if(win_id ==  MMK_GetWinHandle(applet_handle, MMIPB_BT_CONTACTS_LIST_TAB_WIN_ID))
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_BT_ENTRY_LISTBOX_CTRL_ID);
    }
#endif
    else
    {
        list_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LISTBOX_CTRL_ID);
    }
#endif 
      
    return list_handle;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_HandleListFilter(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIPB_FILTER_TYPE_E type)
{
    switch (type)
    {
    case MMIPB_FILTER_ALL:
        GUILIST_SetAllSelected(ctrl_id, TRUE);
        break;

#ifdef CMCC_UI_STYLE
    case MMIPB_FILTER_SIM:
    case MMIPB_FILTER_PHONE:
        {
            BOOLEAN         is_sim = TRUE;
            BOOLEAN         is_filter_sim = TRUE;
            uint16          total_num = 0;
            int32           i = 0;
            GUILIST_ITEM_T  *item_ptr = PNULL;
            uint16          storage_id = 0;
            MMIPB_CONTACT_BASE_INFO_T   contact_node_info = {0};
#ifdef MMIPB_MOST_USED_SUPPORT
            MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node_info= {0};
#endif
            MMIPB_APPLET_T*    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

            if(PNULL == applet_ptr)
            {
                return;
            }

            total_num = GUILIST_GetTotalItemNum(ctrl_id);

            is_filter_sim = type == MMIPB_FILTER_SIM;

            GUILIST_SetAllSelected(ctrl_id, FALSE);
        
            for (i = 0; i < total_num;i++)
            {
                item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, i);

                if(!(item_ptr->item_state & GUIITEM_STATE_SPLIT))
                {
#ifdef MMIPB_MOST_USED_SUPPORT
                    if(applet_ptr->instance.cur_group_id != PB_GROUP_MOSTUSED)
                    {
                        contact_node_info = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
                        storage_id = contact_node_info.storage_id;
                    }
                    else
                    {
                        mostused_contact_node_info = MMIPB_GetMostUsedContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
                        storage_id = mostused_contact_node_info.storage_id;
                    }
#else
                    contact_node_info = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
                    storage_id = contact_node_info.storage_id;
#endif

                    is_sim = MMIPB_IsSIMContact(storage_id);

                    if ((is_filter_sim && is_sim) ||
                        (!is_filter_sim && !is_sim))
                    {
                        GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
                    }
                }
            }
        }
        break;
#endif

    case MMIPB_FILTER_NONE:
    default:
        GUILIST_SetAllSelected(ctrl_id, FALSE);
        break;
    }
}

/*****************************************************************************/
//  Description : 判断是否有联系人已被标记api
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsMark(MMI_HANDLE_T app_handle)
{
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(app_handle);

    if(PNULL  == applet_ptr)
    {
        return FALSE;
    }

    if(PNULL == applet_ptr->instance.select_list)
    {
        return FALSE;
    }
    return applet_ptr->instance.select_list->sel_count> 0;

}
/*****************************************************************************/
//  Description : is cur item mark
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsCurItemMark(MMI_HANDLE_T applet_handle)
{
    MMIPB_APPLET_T* applet_ptr =  (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    MMI_HANDLE_T     list_handle = NULL;
    GUILIST_ITEM_T*  list_ptr =PNULL;
    uint16           index = 0;
    if(PNULL == applet_ptr)
        return FALSE;

   list_handle =  MMIPB_GetListHandleByWin(applet_ptr->instance.active_win_id);

   index = GUILIST_GetCurItemIndex(list_handle);
   list_ptr = GUILIST_GetItemPtrByIndex(list_handle, index);/*lint !e605*/

   return GUILIST_GetItemState(list_ptr, GUIITEM_STATE_SELECTED);

}

/*****************************************************************************/
//  Description : update current list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateCurrentList(MMI_HANDLE_T applet_handle, MMIPB_OPC_E opc_type)
{

    if(opc_type == MMIPB_OPC_MANAGE_GROUP)
    {
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID)))
        {
         MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID), MMI_PB_GROUP_NUM_CHANGED, PNULL);
        }
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_ADD_NEW_GROUP_WIN_ID)))
        {
         MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_ADD_NEW_GROUP_WIN_ID), MMI_PB_GROUP_NUM_CHANGED, PNULL);
        }  
#ifndef MMI_PDA_SUPPORT
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_ENTRYL_LIST_WIN_ID)))
        {
            MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_ENTRYL_LIST_WIN_ID), MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
        }
#else
        if(MMK_GetPrevWinHandle(MMK_GetFocusWinHandle()) == MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID))
        {
            MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
        }
        
#endif        
    }
    else
    {
#ifndef MMI_PDA_SUPPORT
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID)))
        {
         MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_GROUP_LIST_WIN_ID), MMI_PB_GROUP_NUM_CHANGED, PNULL);
        }
#endif    
        MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_MAIN_WIN_ID), MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_DETAILS_WIN_ID)))
        {
            MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_DETAILS_WIN_ID), MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG, (ADD_DATA)TRUE);
        }
        if(MMK_IsOpenWin(MMK_GetWinHandle(applet_handle, MMIPB_FDN_LIST_WIN_ID)))
        {
            MMK_SendMsg(MMK_GetWinHandle(applet_handle, MMIPB_FDN_LIST_WIN_ID), MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
        }
    }

}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : 解析VCard文件,并保存到PB中
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenVcardFileContactWin(uint16  *name_ptr)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    BOOLEAN       result = FALSE;
   // MMIPB_ACCESS_POINT_T      access_point = {0};
    MMI_HANDLE_T              applet_handle = NULL;
    MMIPB_INSTANCE_T          pb_instance={0};
    MMIPB_APPLET_T*           applet_ptr = PNULL;
    MMIPB_CONTACT_T*          contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    
    if(PNULL  == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11745_112_2_18_2_44_38_372,(uint8*)"");
        return;
    }
#ifdef MMI_VCARD_SUPPORT
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    ret = MMIPB_ReadVcardToContact(name_ptr, contact_ptr);
#endif
    if(MMIPB_ERROR_SUCCESS != ret)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  RET %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11753_112_2_18_2_44_38_373,(uint8*)"d", ret);
        SCI_FREE(contact_ptr);
        contact_ptr = PNULL;
        return ;
    }
    pb_instance.function_type = MMIPB_OPEN_VCARD;
    pb_instance.entry_func = PNULL;
    pb_instance.update_callback = UpdateCallbackWithCloseApplet;
    applet_handle = MMIPB_StartApplet(&pb_instance);
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        SCI_FREE(contact_ptr);
        contact_ptr = PNULL;
        return;
    }
#ifdef MMI_PDA_SUPPORT
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
        
        result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, FALSE);
	}
	else
#endif		
	{
		result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, TRUE);
		
	}
#else
     result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, FALSE);
#endif
     if(!result)
     {
         SCI_FREE(contact_ptr);
     }
}
#endif

/*****************************************************************************/
//  Description : open widget favorite entry, just called by widget.
//  Global resource dependence :  
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenContactWin(
                                 MMI_HANDLE_T applet_handle,
                                 uint16 entry_id, 
                                 uint16 storage_id)
{
    //MMI_HANDLE_T              applet_handle = NULL;
    BOOLEAN                   result = FALSE;
    MMIPB_CONTACT_T*          contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL  == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11799_112_2_18_2_44_38_374,(uint8*)"");
        return;
    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
   MMIPB_GetContactInfo(entry_id, storage_id,contact_ptr);
   if(PNULL ==  applet_handle)
   {
        //need start a new applet
        MMIPB_INSTANCE_T  *pb_instance_ptr = (MMIPB_INSTANCE_T *)SCI_ALLOCA(sizeof(MMIPB_INSTANCE_T));
        if(PNULL != pb_instance_ptr)
        {
            SCI_MEMSET(pb_instance_ptr, 0x00, sizeof(MMIPB_INSTANCE_T));            
            pb_instance_ptr->function_type = MMIPB_OPEN_FAVOURITE_VIEW;
            pb_instance_ptr->update_callback = s_mmipb_update_callback;
            applet_handle = MMIPB_StartApplet(pb_instance_ptr);
            SCI_FREE(pb_instance_ptr);
        }
   }
   else
   {
        MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
        if(applet_ptr != PNULL)
        {
            applet_ptr->instance.cur_entry_id = entry_id;
            applet_ptr->instance.cur_storage_id = storage_id;
            applet_ptr->instance.cur_num_index = 0; 
        }
   }

#ifdef MMI_PDA_SUPPORT
#if defined MMI_ISTYLE_SUPPORT 
	if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
        result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, FALSE);
	}
	else
#endif		
	{
		result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, FALSE);
		
	}
#else
	 result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, FALSE);
#endif    
     if(!result)
     {
         SCI_FREE(contact_ptr);
     }
}

/*****************************************************************************/
//  Description :本机号码页面所点项是否存在有效号码
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsCurContactValid(uint16 contact_id, uint16 storage_id)
{
    BOOLEAN                 ret =FALSE;
    MMIPB_CONTACT_T *contact_ptr = PNULL;        

    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_IsCurContactValid PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11886_112_2_18_2_44_38_375,(uint8*)"");
        return ret;
    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));

    if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(contact_id, storage_id, contact_ptr))
    {
        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[0].npi_ton, contact_ptr->number[0].number_len) || contact_ptr->name.wstr_len > 0)
        {
            ret = TRUE;
        }
    }

    SCI_FREE(contact_ptr);
    //SCI_TRACE_LOW:"[baokun] MMIPB_IsCurMSISDNItemValid ret %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11900_112_2_18_2_44_38_376,(uint8*)"d", ret);
    return ret;
}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : 解析VCard data,并打开该文件
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenVcardContactWin(uint8  *data_ptr, uint32 data_size)
{

    MMIPB_INSTANCE_T pb_instance ={0};
    MMI_HANDLE_T     applet_handle = NULL;
    MMIPB_ERROR_E    ret = MMIPB_ERROR_ERROR;
    BOOLEAN          result = FALSE;
    MMIPB_CONTACT_T*          contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    
    if(PNULL  == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIAPIPB_OpenVcardFileContactWin  PNULL  == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11920_112_2_18_2_44_38_377,(uint8*)"");
        return;
    }

    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    ret = MMIPB_VcardToContact(data_ptr, data_size, contact_ptr);

    if(MMIPB_ERROR_SUCCESS ==  ret)
    {  
        pb_instance.contacts_type = MMIPB_WITHOUT_CONTACTS_LIST;
        pb_instance.cur_group_id = PB_GROUP_ALL;
        pb_instance.function_type = MMIPB_OPEN_VCARD;
        pb_instance.update_callback = UpdateCallbackWithCloseApplet;
        applet_handle = MMIPB_StartApplet(&pb_instance);

#ifdef MMI_PDA_SUPPORT
#if defined MMI_ISTYLE_SUPPORT 
	    if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	    {
            result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, FALSE);
	    }
	    else
#endif			
	    {
		     result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, TRUE);
		    
	    }
#else
	   result = MMIPB_OpenDetailWin(applet_handle, contact_ptr, FALSE);
#endif       
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_ReadVCardData %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11950_112_2_18_2_44_38_378,(uint8*)"d",  ret);
    }

    if(!result)
    {
        SCI_FREE(contact_ptr);
    }

}
#endif
/*****************************************************************************/
//  Description :   获得本机号码
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetPBMSISDN(MN_DUAL_SYS_E dual_sys, 
            MMIPB_CONTACT_T *pb_msisdn_ptr)
{
    int32                   i = 0;
    MMIPB_CONTACT_T *contact_ptr = PNULL;  
    MMIPB_STORAGE_INFO_T            msisdn_info  = {0};
    uint16                  entry_id=0;
    uint16                  storage_id = 0;
    BOOLEAN                 ret = FALSE;

    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == pb_msisdn_ptr || PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIPB_GetPBMSISDN pb_msisdn_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_11974_112_2_18_2_44_38_379,(uint8*)"d", pb_msisdn_ptr);
        ret =  FALSE;
    }
    else
    {
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
        msisdn_info  = MMIPB_GetContactStorage(dual_sys, PHONEBOOK_MSISDN_STORAGE);
        for (i = 1; i <= msisdn_info.max_record_num; i++)
        {
            entry_id = i;
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, PHONEBOOK_MSISDN_STORAGE);//(PHONEBOOK_MSISDN_STORAGE<<8)&0xFF00) |(MMIPB_STORAGE_SIM1 + (dual_sys-MN_DUAL_SYS_1));/*lint !e656*/
            if(MMIPB_ERROR_SUCCESS ==  MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr))
            {
                SCI_MEMCPY(pb_msisdn_ptr, contact_ptr, sizeof(MMIPB_CONTACT_T));
                ret =  TRUE;
            }
        
        }
        SCI_FREE(contact_ptr);
    }
    
    
    return ret;    
}
#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : 将电话簿记录中的姓名设置为LISTBOX控件中的一个ITEM
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

LOCAL void MMIPB_SetEntryNameToListItem
(
                                         MMI_CTRL_ID_T       ctrl_id,
                                         MMI_TEXT_ID_T       left_softkey_id,
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
                                         BOOLEAN  is_enter_from_callsetting, 
#endif
                                         MMIPB_NAME_T        *name_ptr,
                                         uint32              user_data
)
{
    MMI_STRING_T str_t = {0};
    MMIPB_BCD_NUMBER_T number_t = {0};
    //MN_NUMBER_TYPE_E number_type = 0;
   // uint8          tel_num[MMIPB_MAX_STRING_LEN + 2] = {0};
    wchar temp_wstr[MMIPB_MAX_STRING_LEN + 2] = {0};
    MN_DUAL_SYS_E    dual_sys = MN_DUAL_SYS_1;
    uint16 storage_id = user_data>>16;
    uint16 entry_id = user_data&0x0000ffff;
    MMI_IMAGE_ID_T photo_image_id[MN_DUAL_SYS_MAX] =
    {
    #if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
        IMAGE_PB_LIST_SIM1_ICON, 
        IMAGE_PB_LIST_SIM2_ICON, 
    #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_PB_LIST_SIM3_ICON,  
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_PB_LIST_SIM4_ICON,  
    #endif
    #endif
    #else
        IMAGE_PB_LIST_SIM_ICON,
    #endif
     };/*lint !e785*/
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMI_IMAGE_ID_T  middle_softkey_id = IMAGE_NULL;
#else
     MMI_TEXT_ID_T  middle_softkey_id = TXT_NULL;
#endif
     
    if( PNULL == name_ptr )
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_SetEntryNameToListItem PNULL == name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12015_112_2_18_2_44_39_380,(uint8*)"");
        return;
    }
    str_t.wstr_len = name_ptr->wstr_len;
    str_t.wstr_ptr = name_ptr->wstr;

    if(str_t.wstr_len == 0)
    {
        MMIPB_CONTACT_T contact_info= {0};
        MMIPB_GetContactInfo(entry_id, storage_id, &contact_info);
        SCI_MEMCPY(&number_t, &(contact_info.number[0]), sizeof(MMIPB_BCD_NUMBER_T));
        MMIPB_BCDNumberToWstr(&number_t, MMIPB_MAX_STRING_LEN, temp_wstr);
        str_t.wstr_ptr = temp_wstr;
        str_t.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
    }
    dual_sys = MMIPB_GetDualSysByStorageId(storage_id);
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
    if (is_enter_from_callsetting)
    {
#ifndef MID_SOFTKEY_USE_IMG_SUPPORT
        middle_softkey_id = TXT_VIEW;
#else
		middle_softkey_id = IMAGE_COMMON_SOFTKEY_VIEW_ICON;
#endif
    }
#endif
    AppendListItem(ctrl_id, 0, user_data, GUIITEM_STYLE_ONE_LINE_ICON_TEXT, left_softkey_id, middle_softkey_id, STXT_RETURN, 
                   photo_image_id[dual_sys], &str_t,
                   TXT_NO_NAME
                   );

}
#endif

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : 加载SIM 特定号码列表
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void LoadSIMDailNumberList(MN_DUAL_SYS_E dual_sys, //if dual_sys ==MMI_DUAL_SYS_MAX, means all sim 
                                 PHONEBOOK_STORAGE_E contact_type, 
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
                                 BOOLEAN is_enter_from_callsetting,
#endif
                                 MMI_CTRL_ID_T ctrl_id)
{
    uint32 user_data = 0;
    uint16 storage_id = 0;
    //uint16 contact_id = 0;
    MMIPB_CONTACT_T *contact_ptr = PNULL; 
    MMIPB_CONTACT_BASE_INFO_T find_data = {0};
    MMIPB_NAME_T pb_name = {0};
    MMI_STRING_T num_str = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMIPB_STORAGE_INFO_T storage_info = {0};
    MMIPB_HANDLE_T   handle = 0;
    MN_DUAL_SYS_E start_dual = MN_DUAL_SYS_1;
    MN_DUAL_SYS_E end_dual = MN_DUAL_SYS_1;
    MN_DUAL_SYS_E dual_index = MN_DUAL_SYS_1;
    uint16        cur_index = 0;
    uint32        mask_group = 0;
    uint16        item_count = 0;
    uint16 current_index = GUILIST_GetCurItemIndex(ctrl_id);
    
    //SCI_TRACE_LOW:"[MMIPB]LoadSIMDailNumberList: dual_sys=%d contact_type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12053_112_2_18_2_44_39_381,(uint8*)"dd", dual_sys, contact_type);
    
    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCAZ(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] LoadFDNList PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12058_112_2_18_2_44_39_382,(uint8*)"");
        return;
    }
    if(MMI_DUAL_SYS_MAX == dual_sys)
    {
        //list all sim
        start_dual = MN_DUAL_SYS_1;
        end_dual = MMI_DUAL_SYS_MAX -1;/*lint !e778*/
    }
    else
    {
        start_dual = dual_sys;
        end_dual = dual_sys;
    }
    for (dual_index = start_dual; dual_index <= end_dual; dual_index++)
    {
        //get max count of dail contacts
        storage_info = MMIPB_GetContactStorage(dual_index, contact_type);
        item_count += storage_info.max_record_num;

    }
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id, item_count, FALSE);
    for (dual_index = start_dual; dual_index <= end_dual; dual_index++)
    {   //get every sim dail contacts
        mask_group = 1<<(PB_GROUP_SIM_1 + dual_index);
        storage_id = MMIPB_GetStorageID((MMIPB_STORAGE_SIM1 + dual_index -MN_DUAL_SYS_1), contact_type);
        if(MMIPB_ERROR_SUCCESS == MMIPB_FindFirstContact(mask_group, storage_id, &cur_index, PNULL, &find_data, &handle))
        {
            do 
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                if (MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(find_data.contact_id, find_data.storage_id, contact_ptr))
                {
                    SCI_MEMSET(&pb_name, 0x00, sizeof(pb_name));
                    if(contact_ptr->name.wstr_len > 0)
                    {
                        SCI_MEMCPY(&pb_name, &contact_ptr->name, sizeof(pb_name));
                    }
                    else
                    {
                        uint16 len=MIN(pb_name.wstr_len, MMIPB_NAME_MAX_LEN-1);
                        pb_name.wstr_len = MMIPB_BCDNumberToWstr(&contact_ptr->number[0],
                            len+2,
                            pb_name.wstr); 
                    }
 
                }
                else if(MMIPB_DailNumberContactCanbeEmpty(storage_id))
                {
                    //显示空记录
                    MMI_GetLabelTextByLang(TXT_NO_NAME, &num_str);
                    pb_name.wstr_len = MIN(num_str.wstr_len, MMIPB_NAME_MAX_LEN);
                    MMIAPICOM_Wstrncpy(pb_name.wstr, num_str.wstr_ptr, pb_name.wstr_len);                    
                }
                user_data = find_data.contact_id + (find_data.storage_id << 16);
                //insert to listbox
                if(MMIPB_IsContactHasMenu(find_data.storage_id))
                {
                    text_id = STXT_OPTION;
                }
                /********CR NEWMS00207498*********/
                else if (PHONEBOOK_MSISDN_STORAGE == contact_type)
                {
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
                    if (is_enter_from_callsetting )
                    {
                        text_id = TXT_NEXT_STEP;
                    }
                    else
#endif
                    {
                        text_id = TXT_COMMON_OK;
                    }
                }
                /*********End NEWMS00207498*******/
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
                MMIPB_SetEntryNameToListItem(ctrl_id, text_id, is_enter_from_callsetting, &pb_name, user_data);
#else
                MMIPB_SetEntryNameToListItem(ctrl_id, text_id, &pb_name, user_data);
#endif
            } while(MMIPB_ERROR_SUCCESS == MMIPB_FindNextContact(handle, mask_group,storage_id, &cur_index, PNULL, &find_data));
            MMIPB_FindClose(handle);  
        }
    }
    SCI_FREE(contact_ptr);

    GUILIST_SetCurItemIndex(ctrl_id, current_index);
}
#endif

#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : PIN code verify ok
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_Pin2VerifyCnf(void)
{
    MMI_HANDLE_T applet_handle = 0;
    MMI_HANDLE_T win_handle = 0;
    MMIPB_GetCurAppletHandle(&applet_handle);
    win_handle = MMK_GetWinHandle(applet_handle, MMIPB_FDN_LIST_WIN_ID);

    MMK_SendMsg(win_handle,  MMI_PB_PIN2_VERIFY_OK,  PNULL);
}
#endif

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && defined(MMI_PDA_SUPPORT)
/*****************************************************************************/
//  Description : CreateNewWinSoftKey
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateNewWinSoftKey(MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T   softkey_ctrl_id = 0;

    MMK_DestroyControl(MMK_GetCtrlHandleByWin(win_handle , MMITHEME_GetSoftkeyCtrlId()));

    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;
    

    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr = &softkey_data;

    MMK_CreateControl( &softkey_create );
    
    MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : FDN记录列表显示窗口
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMDNListWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_FDN_LIST_CTRL_ID);
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = 0;//(uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    //uint16  list_index = 0; 
    uint32  user_data = 0;
    MMIPB_MENU_DATA_INFOR_T menu_data = {0};
    MMIPB_CONTACT_T *contact_ptr = PNULL;  
    MMIPB_APPLET_T *applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);
    CC_CALL_SIM_TYPE_E call_sim_type = CC_CALL_SIM_MAX;
    MMI_TEXT_ID_T title_id = TXT_NULL;
    MMIPB_SIM_NUMBER_LIST_INFO_T *dail_num_list_ptr = PNULL;
    uint8 num_str[MMIPB_MAX_STRING_LEN + 2] = {0};
    uint8 num_len = 0;
    uint8 group_id = 0;
    uint16 storage_id = 0;
    MMI_STRING_T name_str = {0};

    if(PNULL == applet_ptr)
    {
        recode = MMI_RESULT_FALSE;
        return recode;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        MMK_SetAtvCtrl(win_id, ctrl_id);
        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if(dail_num_list_ptr != PNULL)
        {
            title_id = GetContactTypeText(dail_num_list_ptr->sim_contact_type);
        }
        GUIWIN_SetTitleTextId(win_id, title_id, FALSE);
#ifdef MMI_PDA_SUPPORT
        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if(PNULL != dail_num_list_ptr)
        {
        
            if(MMIPB_IsContactHasMenu(MMIPB_GetStorageID(MMIPB_STORAGE_SIM1, dail_num_list_ptr->sim_contact_type)))
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            }
        }
#endif 
        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, PNULL);
        break;

//     case MSG_GET_FOCUS:
//         MMK_SendMsg(win_id, MSG_FDN_RELOAD_SEARCH_LIST, PNULL);
//         break;

    case MMI_PB_RELOAD_SEARCH_LIST_MSG:
         //load number list
        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if (dail_num_list_ptr != PNULL)
        {
           // uint16 total_num =0; 

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT) && !defined(MMI_PDA_SUPPORT)
            LoadSIMDailNumberList(dail_num_list_ptr->dual_sys, dail_num_list_ptr->sim_contact_type, dail_num_list_ptr->is_enter_from_callsetting, ctrl_id);
#else
            LoadSIMDailNumberList(dail_num_list_ptr->dual_sys, dail_num_list_ptr->sim_contact_type, ctrl_id);
#endif
            if (PHONEBOOK_FDN_STORAGE == dail_num_list_ptr->sim_contact_type
			    /*&& 0 == total_num*/)
            {            
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);                
            }          
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
            if (dail_num_list_ptr->is_enter_from_callsetting && (PHONEBOOK_MSISDN_STORAGE== dail_num_list_ptr->sim_contact_type))
            {
#ifdef MMI_PDA_SUPPORT
                GUI_BOTH_RECT_T         list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#if defined(MMI_ISTYLE_SUPPORT)                
                if (MMITHEME_IsIstyle())
                {
                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                    GUIWIN_SetTitleButtonTextId(win_id,GUITITLE_BUTTON_RIGHT,TXT_NEXT_STEP, FALSE);
                    MMK_SetWinDisplayStyleState(win_id, WS_HAS_BUTTON_SOFTKEY, FALSE);
                }
                else
#endif					
                {
                    MMK_SetWinDisplayStyleState(win_id, WS_HAS_BUTTON_SOFTKEY, TRUE);
                    CreateNewWinSoftKey(win_id, TXT_NEXT_STEP, TXT_NULL, STXT_RETURN);
                }
                
                list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
                GUIAPICTRL_SetBothRect(ctrl_id, &list_both_rect);
#else
#ifndef MID_SOFTKEY_USE_IMG_SUPPORT
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NEXT_STEP, TXT_VIEW, STXT_RETURN, TRUE);
#else
                GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_VIEW_ICON, 1, FALSE);
#endif
#endif
            }
#endif
            //if(GUILIST_GeTtO)
            if(MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        } 
        break;

    case MSG_KEYUP_GREEN:
#ifdef VT_SUPPORT
    case MSG_KEYLONG_GREEN:
#endif
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        //call number
        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if(dail_num_list_ptr != PNULL)
        {
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
            call_sim_type = CC_CALL_SIM_MAX;
#else
            call_sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
#endif
            //get current contact
            contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL == contact_ptr)
            {
                //SCI_TRACE_LOW:"[MMIPB] HandleSIMDNListWinMsg PNULL == contact_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12239_112_2_18_2_44_39_383,(uint8*)"");
                break;
            }
            SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T));
            GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &user_data);            
            if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(user_data & 0xFFFF, user_data >> 16, contact_ptr))
            {
                //make call
                name_str.wstr_len = contact_ptr->name.wstr_len;
                name_str.wstr_ptr = contact_ptr->name.wstr;
                num_len = MMIPB_BCDNumberToUTF8((MMIPB_BCD_NUMBER_T *)&contact_ptr->number[0], (uint8)(MMIPB_MAX_STRING_LEN + 2), num_str);

#ifdef MMIPB_SIMDN_SUPPORT
				if (MMIPB_IsSDNContact(contact_ptr->storage_id))
				{
					dail_num_list_ptr->dual_sys = MMIPB_GetDualSysByStorageId(contact_ptr->storage_id);
				}
#endif

#ifdef MMI_FDN_DIAL_TO_PANEL_SUPPORT
                if (PHONEBOOK_FDN_STORAGE == (PHONEBOOK_STORAGE_E)MMIPB_GET_CONTACT_TYPE(contact_ptr->storage_id))
                {
                    MMIAPICL_OpenEditBeforeDialWin(num_len, num_str);
                }
                else
#endif
                {
                    MMIPB_CallNumber(dail_num_list_ptr->dual_sys,
                        MMIPB_CALL_TYPE_MAKECALL_AUDIO,
                        num_str,
                        &name_str);
                }

            }
            SCI_FREE(contact_ptr);
        }
        break;
    case MMI_PB_PIN2_VERIFY_OK:
        //PIN2 VERIFY OK
        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if(dail_num_list_ptr != PNULL)
        {
            GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &user_data);
            dual_sys = ((dail_num_list_ptr->dual_sys < MMI_DUAL_SYS_MAX) ?dail_num_list_ptr->dual_sys:MN_DUAL_SYS_1);
            if(dail_num_list_ptr->dual_sys < MMI_DUAL_SYS_MAX)
            {
                group_id = 1<<(PB_GROUP_SIM_1+ dail_num_list_ptr->dual_sys);
            }
            else
            {
                group_id = PB_GROUP_SIM_1;
            }
            menu_data.group = (1<<group_id);
            menu_data.applet_handle = applet_ptr->caf_applet.app_handle;
            menu_data.is_need_pin_verify = dail_num_list_ptr->is_need_pin_verify;
            menu_data.dual_sys = dail_num_list_ptr->dual_sys;
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, dail_num_list_ptr->sim_contact_type);
            if(MMIPB_OPC_ADD == applet_ptr->instance.cmd)
            {
                MMIPB_AddContactWin(applet_ptr->caf_applet.app_handle, PNULL, group_id,
                    storage_id, MMIPB_ADD_TYPE_MSISDN, FALSE);
            }           
            else if(MMIPB_OPC_EDIT == applet_ptr->instance.cmd)             
            {
                //add or edit contact
                contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(contact_ptr == PNULL)
                {
                    break;
                }
                SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T)); 
                contact_ptr->storage_id = user_data >> 16;
                MMIPB_GetContactInfo(user_data & 0xFFFF, user_data >> 16, contact_ptr);
                menu_data.dual_sys = MMIPB_GetDualSysByStorageId(user_data >> 16);
                menu_data.contact_ptr = contact_ptr;  
                menu_data.operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;
                MMIPB_AddOrEditContactOfPb(&menu_data);
                SCI_FREE(contact_ptr);
                
            }
            else if(MMIPB_OPC_DELETE == applet_ptr->instance.cmd)
            {
                //delete contact
                applet_ptr->instance.cur_entry_id = user_data & 0xFFFF;
                applet_ptr->instance.cur_storage_id = user_data >> 16;
                MMIPB_DeleteContactWin(applet_ptr->caf_applet.app_handle, applet_ptr->instance.handle);  
                MMK_CloseWin(MMK_GetWinHandle(applet_ptr->caf_applet.app_handle, MMIPB_ENTRY_DETAILS_WIN_ID));
            }
        }
        break;
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //contact menu        
        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if(dail_num_list_ptr != PNULL)
        {
            GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &user_data);  
            if(MMIPB_IsContactHasMenu(user_data >> 16))
            {
                contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(contact_ptr == PNULL)
                {
                    break;
                }
                SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T));         
                if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(user_data & 0xFFFF, user_data >> 16, contact_ptr))
                {
                    menu_data.contact_ptr = contact_ptr;
                    if(dail_num_list_ptr->dual_sys < MMI_DUAL_SYS_MAX)
                    {
                        menu_data.group = (1<<(PB_GROUP_SIM_1+ dail_num_list_ptr->dual_sys));
                    }
                    else
                    {
                        menu_data.group = (1<<PB_GROUP_SIM_1);
                    }
                    applet_ptr->instance.cur_entry_id = user_data & 0xFFFF;
                    applet_ptr->instance.cur_storage_id = user_data >> 16;
                    menu_data.applet_handle = applet_ptr->caf_applet.app_handle;
                    menu_data.is_need_pin_verify = dail_num_list_ptr->is_need_pin_verify;
                    menu_data.dual_sys = dail_num_list_ptr->dual_sys;
                    menu_data.parent_win = win_id;
                    menu_data.contact_ptr = contact_ptr;
                    menu_data.menu_type = MMIPB_SIM_DN_LONGTP_MENU;
                    menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                    menu_data.need_reset_storage = FALSE;
                    menu_data.valid_contact_count  = 1;
                    MMIPB_EnterPbTabOptMenu(&menu_data);
                }
                SCI_FREE(contact_ptr);
            }   
        }
        break;
    case MSG_NOTIFY_TITLE_MENU_ICON:
    case MSG_APP_MENU:  
        //menu
        if(MMIAPIPHONE_GetAllSimStateOKCount(PNULL, 0) == 0)
        {
            break;
        }

        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if(dail_num_list_ptr != PNULL)
        {
            if(GUILIST_GetTotalItemNum(ctrl_id) == 0 
               && PHONEBOOK_FDN_STORAGE != dail_num_list_ptr->sim_contact_type
             )
            {
                break;
            }
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
            if (dail_num_list_ptr->is_enter_from_callsetting  && (PHONEBOOK_MSISDN_STORAGE== dail_num_list_ptr->sim_contact_type))
            {
                //进入下一步
                if(MMIAPISET_SetDualSIMCFNRC(win_id))
                {
                    MMK_CloseWin(win_id);
                }
                break;
            }
#endif
            if(MMIPB_IsContactHasMenu(MMIPB_GetStorageID(MMIPB_STORAGE_SIM1, dail_num_list_ptr->sim_contact_type)))
            {               
                contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(PNULL == contact_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIPB] HandleSIMDNListWinMsg PNULL == contact_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12386_112_2_18_2_44_39_384,(uint8*)"");
                    break;
                }
                SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
                menu_data.applet_handle = applet_ptr->caf_applet.app_handle;
                menu_data.parent_win = win_id;
                if(dail_num_list_ptr->dual_sys < MMI_DUAL_SYS_MAX)
                {
                    menu_data.group = (1<<(PB_GROUP_SIM_1+ dail_num_list_ptr->dual_sys));
                    contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 +dail_num_list_ptr->dual_sys, 
                                                                 dail_num_list_ptr->sim_contact_type);
                }
                else
                {
                    menu_data.group = (1<<PB_GROUP_SIM_1);
                    contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + MN_DUAL_SYS_1, 
                                                                 dail_num_list_ptr->sim_contact_type);

                }
                menu_data.menu_type = MMIPB_SIM_DN_OPT_MENU;
                menu_data.dual_sys = dail_num_list_ptr->dual_sys;
                menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                menu_data.is_need_pin_verify = dail_num_list_ptr->is_need_pin_verify;            
                menu_data.contact_ptr = contact_ptr;
                menu_data.need_reset_storage = FALSE;
                menu_data.valid_contact_count= GUILIST_GetTotalItemNum(ctrl_id);
                MMIPB_EnterPbTabOptMenu(&menu_data);
                SCI_FREE(contact_ptr);
            }
        }
    break;
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
         //option

        dail_num_list_ptr = (MMIPB_SIM_NUMBER_LIST_INFO_T *)MMK_GetWinAddDataPtr(win_id);
        if(dail_num_list_ptr == PNULL)
        {
            break;
        }
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
        if (dail_num_list_ptr->is_enter_from_callsetting && (PHONEBOOK_MSISDN_STORAGE== dail_num_list_ptr->sim_contact_type))
        {
            //进入下一步
            if(MMIAPISET_SetDualSIMCFNRC(win_id))
            {
                MMK_CloseWin(win_id);
            }
            break;
        }
#endif
        if(GUILIST_GetTotalItemNum(ctrl_id) == 0 
           && PHONEBOOK_FDN_STORAGE != dail_num_list_ptr->sim_contact_type
         )
        {
            break;
        }
        if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &user_data);
            storage_id = user_data >> 16;
        }
        else
        {
             if(dail_num_list_ptr->dual_sys < MMI_DUAL_SYS_MAX)
             {
                 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dail_num_list_ptr->dual_sys, dail_num_list_ptr->sim_contact_type);
             }
             else
             {
                 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1, dail_num_list_ptr->sim_contact_type);
             }
        }

        if(MMIPB_IsContactHasMenu(storage_id))
        {
            //选项菜单
            contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
            SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T));
            
            MMIPB_GetContactInfo(user_data & 0xFFFF, storage_id, contact_ptr);            
            
            SCI_MEMSET(&menu_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
            menu_data.applet_handle = applet_ptr->caf_applet.app_handle;
            menu_data.parent_win = win_id;
            menu_data.dual_sys = MMIPB_GetDualSysByStorageId(storage_id);
            if(menu_data.dual_sys < MMI_DUAL_SYS_MAX)
            {
                menu_data.group = (1<<(PB_GROUP_SIM_1+ menu_data.dual_sys));
            }
            else
            {
                menu_data.group = (1<<PB_GROUP_SIM_1);
            }
            menu_data.menu_type = MMIPB_SIM_DN_OPT_MENU;
            menu_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
            menu_data.is_need_pin_verify = dail_num_list_ptr->is_need_pin_verify;
            menu_data.contact_ptr = contact_ptr;
            menu_data.need_reset_storage = FALSE;
            menu_data.valid_contact_count= GUILIST_GetTotalItemNum(ctrl_id);
            MMIPB_EnterPbTabOptMenu(&menu_data);              
            SCI_FREE(contact_ptr);
        }
       else if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            if(MMIPB_IsCurContactValid(user_data & 0xFFFF, storage_id))
            {
                //查看，进入记录详情
                MMIPB_OpenContactWin(MMK_GetAppletHandleByWin(win_id), user_data & 0xFFFF, storage_id);
            }
            /********CR NEWMS00207498*********/
            else if (PHONEBOOK_MSISDN_STORAGE == dail_num_list_ptr->sim_contact_type)
            {
                MMIPB_CONTACT_T *contact_info_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(PNULL == contact_info_ptr)
                {
                    break;
                }
                SCI_MEMSET(contact_info_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                contact_info_ptr->contact_id = user_data & 0xFFFF;
                contact_info_ptr->storage_id = storage_id;
                EditContact(applet_ptr, contact_info_ptr);
                SCI_FREE(contact_info_ptr);
            }
            /********End NEWMS00207498********/
        }
        break;
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                if(MMIAPISET_SetDualSIMCFNRC(win_id))
                {
                    MMK_CloseWin(win_id);
                }
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                 MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;           
            default:
                 MMK_SendMsg(win_id, MSG_CTL_MIDSK, PNULL);
                break;
            }
        }
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        //detail:进入查看记录详情 
        if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            GUILIST_GetItemData(ctrl_id, GUILIST_GetCurItemIndex(ctrl_id), &user_data);  
            storage_id = user_data >> 16;
            if(MMIPB_IsCurContactValid(user_data & 0xFFFF, storage_id))
            {
                //查看，进入记录详情
                MMIPB_OpenContactWin(MMK_GetAppletHandleByWin(win_id), user_data & 0xFFFF, storage_id);
            }
            else
            {
                //新增
                MMIPB_CONTACT_T *contact_info_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(PNULL == contact_info_ptr)
                {
                    SCI_TRACE_LOW("[MMIPB] HandleSIMDNListWinMsg MSG_CTL_MIDSK PNULL == contact_ptr");                   
                    break;
                }
                SCI_MEMSET(contact_info_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                contact_info_ptr->storage_id = storage_id;
                contact_info_ptr->contact_id = user_data & 0xFFFF;
                EditContact(applet_ptr, contact_info_ptr);
                SCI_FREE(contact_info_ptr);            }
        }
        break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);      
        break;
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;
    default:
        recode = FALSE;
        break;
    }

    return recode;
}
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 通话记录中设置号码类型的列表控件
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

LOCAL void SetNumTypeListboxForCalllog(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id) 
{
    uint8    i  = 0;
    uint8    num_type = 0;
    uint32  item_state = 0;
    uint8 loop_count=MMIPB_MAX_NV_PHONE_NUM;
    MMI_TEXT_ID_T       text_id=TXT_UNKNOWN;
#ifdef CMCC_VCARD_SUPPORT
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMI_IMAGE_ID_T           mid_softkey_id = IMAGE_NULL;
#else
	MMI_TEXT_ID_T            mid_softkey_id = TXT_NULL;
#endif
    MMIPB_CONTACT_T*         contact_info = PNULL; 
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr) 
    {
        return;
    }
    contact_info = SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_info)
    {
        return;
    }

    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    MMIPB_GetContactInfo(applet_ptr->instance.cur_entry_id, applet_ptr->instance.cur_storage_id, contact_info);
   
#ifdef CMCC_VCARD_SUPPORT
    dual_sys = MMIPB_GetDualSysByStorageId(applet_ptr->instance.cur_storage_id);
    if(dual_sys < MMI_DUAL_SYS_MAX && MMIPB_IsUsim(applet_ptr->instance.cur_storage_id))
    {
        loop_count = MMIPB_ReadUSIMAASCount(dual_sys)+1;
    }
    else
#endif
    if(MMIPB_IsInSIMGroupByStorage(applet_ptr->instance.cur_storage_id))
    {
        loop_count = 1;
    }
    for ( i = 0; i < loop_count; i++ )
    {
        item_state = 0;
        num_type = contact_info->num_type[i];
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(applet_ptr->instance.cur_storage_id))
        {  
            switch(num_type)
            {
            case MMIPB_ADN_NUMBER:
                text_id = TXT_PB_ITEM_DETAIL_MOBILE_PHONE;
                break;
            case MMIPB_HOME_NUMBER:
                text_id = TXT_PB_ITEM_DETAIL_HOME_PHONE;
                break;
            case MMIPB_OFFICE_NUMBER:
                text_id = TXT_PB_ITEM_DETAIL_OFFICE_PHONE;
                break;
            case MMIPB_FIXED_NUMBER:
                text_id = TXT_PB_PHONE_NUMBER;
                break;
            case MMIPB_FAX_NUMBER:
                text_id = TXT_PB_FAX_NUMBER;
                break;
			case MMIPB_UNKNOWN_NUMBER:
				text_id = TXT_UNKNOWN;
				break;
            default:
                text_id = TXT_PB_ITEM_DETAIL_MOBILE_PHONE;
                break;
            }
            
        }
        else
#endif  
        {
            text_id=s_number_text_id[i];
        }
        AppendListItem(ctrl_id, item_state, MMIPB_LISTBOX_INDEX_NUMBER_MOBILE+i, GUIITEM_STYLE_ONE_LINE_ICON_TEXT, TXT_COMMON_OK, mid_softkey_id,STXT_RETURN, 
                       MMIPB_GetIconIdOfPhone(num_type), PNULL,
                       text_id
                       ); 

    }
    
    SCI_FREE(contact_info);
}
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 通话记录中号码类型选择窗口的处理
//  Global resource dependence : g_mmipb_operate_records_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleSelectNumTypeWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMK_GetCtrlHandleByWin(win_id,MMIPB_SELECT_NUM_TYPE_LISTBOX_CTRL_ID) ;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T    applet_handle = 0;
    uint16              index = 0;
    MMIPB_CONTACT_T  *contact_info = PNULL;
    //modify for coverity
	if(PNULL == applet_ptr) 
    {
        return MMI_RESULT_FALSE;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:   // 打开窗口:

        GUILIST_SetMaxItem(ctrl_id, MMIPB_MAX_NV_PHONE_NUM, FALSE );//max item 5
        MMK_SetAtvCtrl(win_id, ctrl_id);
        SetNumTypeListboxForCalllog(win_id,ctrl_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:    // 确定:
    case MSG_APP_OK:
    case MSG_APP_WEB:
            contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
            SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
           index  = GUILIST_GetCurItemIndex(ctrl_id);
            MMIPB_GetContactInfo(applet_ptr->instance.cur_entry_id, applet_ptr->instance.cur_storage_id, contact_info);
            if (MMIPB_IsValidPhoneNumber(
					contact_info->number[index].npi_ton,contact_info->number[index].number_len)
                    )
            {
                //exist number
                MMIPUB_CloseQuerytWin(PNULL);
                MMIPUB_OpenQueryWinByTextId(TXT_PB_OVERWRITE_OR_NOT, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
            }
            else
            {
                MMIPB_MENU_DATA_INFOR_T add_data = {0};
                uint32 user_data = 0;
                add_data.contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(PNULL == add_data.contact_ptr)
                {
                    //SCI_TRACE_LOW:"[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12655_112_2_18_2_44_40_385,(uint8*)"");
                    break;
                }
                SCI_MEMSET( add_data.contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                MMIPB_GetContactInfo(applet_ptr->instance.cur_entry_id, applet_ptr->instance.cur_storage_id, add_data.contact_ptr);
                add_data.contact_ptr->storage_id = applet_ptr->instance.cur_storage_id;
                SCI_MEMCPY(&add_data.contact_ptr->number[index].number, &applet_ptr->instance.add_number_t.number,applet_ptr->instance.add_number_t.number_len);
                add_data.contact_ptr->number[index].number_len = applet_ptr->instance.add_number_t.number_len;
                add_data.contact_ptr->number[index].npi_ton = applet_ptr->instance.add_number_t.npi_ton;
                GUILIST_GetItemData(ctrl_id,index,&user_data);
                add_data.contact_ptr->num_type[index] = (uint8)user_data;
                add_data.applet_handle = applet_handle;
                add_data.operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;
                MMIPB_AddOrEditContactOfPb(&add_data);

                SCI_FREE(add_data.contact_ptr);
                add_data.contact_ptr = PNULL;
               // MMK_CloseWin(win_id);
            }
        SCI_FREE(contact_info);
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    case MSG_PROMPTWIN_OK://确定覆盖已有文件
        {
            MMIPB_MENU_DATA_INFOR_T add_data = {0};
            add_data.contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL == add_data.contact_ptr)
            {
                //SCI_TRACE_LOW:"[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12681_112_2_18_2_44_40_386,(uint8*)"");
                break;
            }
            MMIPUB_CloseQuerytWin(PNULL);
            add_data.contact_ptr->storage_id = applet_ptr->instance.cur_storage_id;
            MMIPB_GetContactInfo(applet_ptr->instance.cur_entry_id, applet_ptr->instance.cur_storage_id, add_data.contact_ptr);
            index       = GUILIST_GetCurItemIndex(ctrl_id);
            SCI_MEMCPY(&(add_data.contact_ptr->number[index].number), &applet_ptr->instance.add_number_t.number,applet_ptr->instance.add_number_t.number_len);
            add_data.contact_ptr->number[index].number_len = applet_ptr->instance.add_number_t.number_len;
            add_data.contact_ptr->number[index].npi_ton = applet_ptr->instance.add_number_t.npi_ton;
            add_data.applet_handle = applet_handle;
            add_data.operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;
		    MMIPB_AddOrEditContactOfPb(&add_data);

            SCI_FREE(add_data.contact_ptr);
            add_data.contact_ptr = PNULL;
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
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 为通话记录打开的选择号码类型的窗口
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL void OpenSelectNumTypeWin
(
    MMI_HANDLE_T applet_hanlde,
    MMI_CTRL_ID_T ctrl_id
)
{
        uint16  cur_index = GUILIST_GetCurItemIndex(ctrl_id);  
        const GUILIST_ITEM_T *list_item_ptr = PNULL;
        uint32              list_data = 0;
        MMIPB_CONTACT_BASE_INFO_T    contact_node = {0}; 
        MMIPB_APPLET_T* applet_ptr =  (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_hanlde);
  
        if(PNULL == applet_ptr)
        {
            return;
        }
        list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
    
        if(PNULL == list_item_ptr)
        {
            //SCI_TRACE_LOW:"[MMIPB] OpenSelectNumTypeWin PNULL == list_item_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12735_112_2_18_2_44_40_387,(uint8*)"");
            return;
        } 
        list_data=list_item_ptr->user_data;
        contact_node = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(list_data));
        applet_ptr->instance.cur_entry_id =contact_node.contact_id;
        applet_ptr->instance.cur_storage_id = contact_node.storage_id;
        MMK_CreateWinByApplet(applet_hanlde,
                    (uint32 *)MMIPB_SELECT_NUM_TYPE_WIN_TAB,
                    PNULL
                    );

}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
//        
/*****************************************************************************/
LOCAL void SetOperateListSoftKey(MMI_WIN_ID_T win_id, 
                          MMI_CTRL_ID_T ctrl_id, 
                          GUILIST_ITEM_DATA_T* item_data_ptr,
                          uint16                index
                          )
{
    MMI_TEXT_ID_T           left_softkey_id = TXT_NULL;
    MMI_TEXT_ID_T           right_softkey_id = TXT_NULL;
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMI_IMAGE_ID_T          mid_softkey_id = IMAGE_NULL;
#else
	MMI_TEXT_ID_T           mid_softkey_id = TXT_NULL;
#endif

    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMIPB_HANDLE_T  applet_handle = NULL;
    if(PNULL == applet_ptr)
    {
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    

    right_softkey_id = STXT_RETURN;
	//set middle SK
    if(!applet_ptr->instance.function_type)
    {
	    if(GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(ctrl_id, index), GUIITEM_STATE_SELECTED))
	    {
#ifdef GUI_MID_SOFTKEY_USE_IMG
            mid_softkey_id = IMAGE_COMMON_SOFTKEY_CANCEL_ICON;
#else
            mid_softkey_id = STXT_CANCEL;
#endif
	    }
	    else if(MMIPB_GetSelectCount(applet_ptr->instance.handle) < applet_ptr->instance.max_select_count)
	    {
#ifdef GUI_MID_SOFTKEY_USE_IMG
            mid_softkey_id = IMAGE_COMMON_SOFTKEY_SELECTED_ICON;
#else
            mid_softkey_id = STXT_SELECT;
#endif
	    } 
    }    
    //set right SK
    /**cr 205491 @yanyan.an**/
    if(MMIPB_LIST_RADIO == applet_ptr->instance.list_type)  
    {
        left_softkey_id = TXT_COMMON_OK;
    }
    else
    {
        left_softkey_id = STXT_OPTION;
    }
    

    item_data_ptr->softkey_id[LEFT_BUTTON] = left_softkey_id;

    item_data_ptr->softkey_id[RIGHT_BUTTON] = right_softkey_id;

	item_data_ptr->softkey_id[MIDDLE_BUTTON] = mid_softkey_id;

#if 0    
    if(!applet_ptr->instance.function_type)
    {
    
        if(GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(ctrl_id, index), GUIITEM_STATE_SELECTED))
        {
            item_data_ptr->softkey_id[MIDDLE_BUTTON] = STXT_CANCEL;
        }
        else
        {

           item_data_ptr->softkey_id[MIDDLE_BUTTON] = STXT_SELECT;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : 设置用于分组管理列表控件的item
//  Global resource dependence :
//  Author: MARYXIAO
//  Note: 目前为一次将所有记录装入
/*****************************************************************************/

LOCAL void SetEntryListItemForEx(
                                 MMI_WIN_ID_T  win_id,
                                MMI_HANDLE_T    applet_handle,  
                                MMI_CTRL_ID_T    ctrl_id,       //IN: 对应控件的ID
                                void *item_ptr,
                                MMIPB_LIST_ITEM_TYPE_E dync_type,
                                BOOLEAN is_need_number
                                )
{

    
    uint16                     index = 0;
    uint32              list_data = 0;
    MMI_STRING_T        disp_str = {0};
    wchar               str[MMIPB_MAX_STRING_LEN + 2] = {0};
    GUILIST_ITEM_T      *list_item_ptr =PNULL;
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)item_ptr;
    MMIPB_CONTACT_BASE_INFO_T    contact_node = {0}; 
    uint16              entry_id =0;
    uint16              storage_id =0;
    uint8               group_id = 0;
    //uint8               photo_id_flag = 0;
    //MN_NUMBER_TYPE_E     number_type = 0;//
    //MMI_STRING_T         num_str ={0};
    //MMIPB_STRING_T       num_str_t = {0};   
    uint16              num_index = 0;
    //wchar                item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    //MMIPB_NAME_NUMBER_MAIL_T *name_mail_str = PNULL;  
    MMIPB_APPLET_T*       applet_ptr =(MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    MMIPB_BCD_NUMBER_T   number_t = {0};
#ifdef MMIPB_MAIL_SUPPORT
    MMIPB_MAIL_T mail_info = {0};
#endif
    uint8           number_index = 0; 
    uint16              item_content_index = 0;
    GUIANIM_FILE_INFO_T anim_path = {0};
    MMI_IMAGE_ID_T     list_icon= 0;
    int                 i =0;
    MMIPB_NAME_T *      entry_name_ptr = PNULL;
    MMIPB_CUSTOM_PHOTO_T                *custom_photo_ptr = PNULL;
    wchar* temp1_wstr_ptr =  PNULL;
    wchar* temp2_wstr_ptr =  PNULL;
    //BOOLEAN is_select = FALSE;
    MMIPB_LIST_FIELD_TYPE_E field_type = 0;
    MMIPB_MENU_DATA_INFOR_T* menu_ptr =  (MMIPB_MENU_DATA_INFOR_T*)MMK_GetWinAddDataPtr(win_id);
    
    if(PNULL == need_item_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]SetEntryListItemForEx PNULL == need_item_data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12852_112_2_18_2_44_40_388,(uint8*)"");
        return;
    }

    
    if((PNULL != menu_ptr)&&(MMIPB_OPERATE_TYPE_SEND_GROUP_MSG == menu_ptr->operate_type))
    {
        field_type = MMIPB_LIST_FIELD_NUMBER;
    }
    else if((PNULL != applet_ptr) && (applet_ptr->instance.app_type < MMIPB_TAB_MAX_NUM))
    {
        field_type = applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type];
    }
    
   
    index = need_item_data_ptr->item_index;
    disp_str.wstr_ptr = str;


    list_item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,index);/*lint !e605*/
    if(PNULL ==list_item_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] AppendDyncListItem PNULL == list_item_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12866_112_2_18_2_44_40_389,(uint8*)"");
        return;
    }  
    
    list_data=list_item_ptr->user_data;
    num_index =  (uint16)MMIPB_NUMINDEX(list_data);
    
    MMIPB_ReadContactSimpleInfo(MMIPB_CONTACTINDEX(list_data), &contact_node);  

    entry_id = contact_node.contact_id;
    storage_id = contact_node.storage_id;
    MMIPB_GetAllGroupIDs(contact_node.group, &group_id, 1);
#ifdef MMI_PDA_SUPPORT
    if(MMIPB_ISSPLIT(list_data))//split
    {
        MMI_STRING_T            display_str   ={0};
        display_str.wstr_len = 1;
        display_str.wstr_ptr = &list_item_ptr->first_letter;
        if (PNULL != display_str.wstr_ptr)
		{
			MMIPB_Wstrupper(&display_str);

	        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
	        item_data.item_content[0].item_data.text_buffer = display_str;

	        GUILIST_SetItemData( ctrl_id, &item_data, index);

       }
        return;
    }
     
#endif
    
    custom_photo_ptr = SCI_ALLOCA(sizeof(MMIPB_CUSTOM_PHOTO_T));
    if(PNULL == custom_photo_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] SetEntryListItemForEx PNULL == custom_photo_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12902_112_2_18_2_44_40_390,(uint8*)"");
        return;
    }
    SCI_MEMSET(custom_photo_ptr, 0X00, sizeof(MMIPB_CUSTOM_PHOTO_T));

    entry_name_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_T));
    if(PNULL == entry_name_ptr)
    {
        SCI_FREE(custom_photo_ptr);
        //SCI_TRACE_LOW:"[mmipb] SetEntryListItemForEx PNULL == entry_name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12911_112_2_18_2_44_40_391,(uint8*)"");
        return;
    }
    SCI_MEMSET(entry_name_ptr, 0X00, sizeof(MMIPB_NAME_T));
    temp1_wstr_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
    if(PNULL == temp1_wstr_ptr)
    {
        SCI_FREE(custom_photo_ptr);
        SCI_FREE(entry_name_ptr);
        //SCI_TRACE_LOW:"[mmipb] SetEntryListItemForEx PNULL == temp1_wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12920_112_2_18_2_44_40_392,(uint8*)"");
        return;
    }
    SCI_MEMSET(temp1_wstr_ptr, 0x00, sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));

    temp2_wstr_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
    if(PNULL == temp2_wstr_ptr)
    {
        SCI_FREE(custom_photo_ptr);
        SCI_FREE(entry_name_ptr);
        SCI_FREE(temp1_wstr_ptr);
        //SCI_TRACE_LOW:"[mmipb] SetEntryListItemForEx PNULL == temp2_wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_12931_112_2_18_2_44_40_393,(uint8*)"");
        return;
    }
     SCI_MEMSET(temp2_wstr_ptr, 0x00, sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));

    {
		MMIPB_NAME_NUMBER_MAIL_T *contact_string_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_NUMBER_MAIL_T));
		
        if(contact_string_ptr != PNULL)
        {
            SCI_MEMSET(contact_string_ptr, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));

            MMIPB_GetContactNameNumberByIndex(contact_string_ptr, MMIPB_CONTACTINDEX(list_data));

			SCI_MEMCPY(entry_name_ptr ,&contact_string_ptr->name, sizeof(MMIPB_NAME_T));

			if(MMIPB_LIST_FIELD_WHOLE == field_type)
			{
				num_index = MMIPB_GetFirstValidNumberIndex(contact_string_ptr->number_t, MMIPB_MAX_NV_PHONE_NUM);
			}
          
            if((num_index < MMIPB_MAX_NV_PHONE_NUM) && (0 < contact_string_ptr->number_t[num_index].number_len))
            {
				SCI_MEMCPY(&number_t, &(contact_string_ptr->number_t[num_index]), sizeof(MMIPB_BCD_NUMBER_T));
            }
			
#ifdef MMIPB_MAIL_SUPPORT
            SCI_MEMCPY(&mail_info, &contact_string_ptr->mail, sizeof(contact_string_ptr->mail));
#endif
            SCI_FREE(contact_string_ptr);
		}
    }

    GetIndexByDyncType(dync_type, item_ptr, &index, &item_content_index);
    list_icon = GetImageIcon(group_id);


    {
        SetOperateListSoftKey(win_id, ctrl_id, &item_data,index);



        AppendDyncPhoto(win_id, 0, entry_id, storage_id, list_icon, custom_photo_ptr, &anim_path , &i ,&item_data);
       // item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
       // item_data.item_content[0].item_data.image_id = GetGroupIcon(group_id);

        disp_str.wstr_ptr =entry_name_ptr->wstr;
        disp_str.wstr_len = entry_name_ptr->wstr_len;


        
        if((PNULL != applet_ptr) &&(applet_ptr->instance.function_type == MMIPB_OPEN_MAIN_LIST) && (entry_name_ptr->wstr_len == 0))
        {
            AppendDyncNumber(win_id, 
#ifdef MMIPB_MAIL_SUPPORT
                &mail_info, 
#else
                PNULL,
#endif
                &number_t, 
                temp2_wstr_ptr,
                &i, 
                MMIPB_LIST_FIELD_WHOLE,
                &number_index, 
                &item_data);
        }
        else
        {
            AppendDyncName(win_id , entry_name_ptr, temp1_wstr_ptr,&i,&item_data);
        }

        if(((PNULL != applet_ptr) && (applet_ptr->instance.function_type != MMIPB_OPEN_MAIN_LIST)) || is_need_number)
        {
            SCI_MEMSET(temp2_wstr_ptr, 0x00, sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
            AppendDyncNumber(win_id, 
#ifdef MMIPB_MAIL_SUPPORT
                &mail_info, 
#else
                PNULL,
#endif
                &number_t, 
                temp2_wstr_ptr,
                &i, 
                MMIPB_LIST_FIELD_WHOLE,
                &number_index, 
                &item_data);
        }

 
        
        if(dync_type == MMIPB_NEED_LIST_ITEM_DATA_TYPE)
        {
            GUILIST_SetItemData( ctrl_id, &item_data, index);
        }
        else if(dync_type == MMIPB_NEED_LIST_ITEM_CONTENT_TYPE)
        {
            GUILIST_SetItemContent( ctrl_id, &item_data.item_content[item_content_index], index, item_content_index );
        }
    }
    
  //  SCI_FREE(name_mail_str);
    SCI_FREE(entry_name_ptr);
    SCI_FREE(custom_photo_ptr);
    SCI_FREE(temp1_wstr_ptr);
    SCI_FREE(temp2_wstr_ptr);

}








/*****************************************************************************/
//  Description : 操作前打开选择窗口的的子控件初始化
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitEntryExControl(MMI_WIN_ID_T win_id)
{
    GUIFORM_CHILD_HEIGHT_T              height_info = {0};
	GUIFORM_CHILD_WIDTH_T               width_info = {0};
    wchar                               forbid_char_value[]= {0x0d, 0x0a};//禁止换行回车的输入
    uint16                              img_width = 0;
    uint16                              img_height = 0;
    MMI_HANDLE_T  main_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LIST_EX_FORM_CTRL_ID);
    MMI_HANDLE_T  child_search_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_ENTRY_LIST_EX_FORM_CTRL_ID);
    MMI_HANDLE_T  child_search_button_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_EX_SEARCH_BUTTON_CTRL_ID);
    MMI_HANDLE_T  search_editbox_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_EX_SEARCH_EDITBOX_CTRL_ID);
    MMI_HANDLE_T  list_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LIST_FOR_EX_LISTBOX_CTRL_ID);
    IGUICTRL_T  *form_ptr = MMK_GetCtrlPtr(child_search_form_handle);
    GUI_BORDER_T  border = {0};
    GUI_BG_T                            button_bg = {0};
    uint16          hor_space = 0;
    uint16          ver_space = 0;     
    //+Bug 119450
    BOOLEAN is_disp_im_icon = TRUE;
	BOOLEAN is_disp_num_info = TRUE;
	//-Bug 119450

    GUIFORM_IsSlide(main_form_handle, FALSE);

    GUIFORM_PermitChildBorder(main_form_handle,FALSE);
    GUIFORM_PermitChildBorder(child_search_form_handle, TRUE);
    if (PNULL != form_ptr )
    {
        border.type = GUI_BORDER_SROUNDED;
        border.color = MMI_BLACK_COLOR;
        border.width = 1;
        IGUICTRL_SetBorder(form_ptr , &border);
    }
/*#ifdef MMI_PDA_SUPPORT 
    MMIPB_CreateNewSoftKey(win_id, main_form_handle,STXT_ADD, TXT_NULL, STXT_CANCEL);
#endif*/
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_PB_MENU_SEARCH_ICON,win_id);
    GUIFORM_SetSpace(child_search_form_handle, &hor_space, &ver_space);
        
    //  search box set
    GUIEDIT_PermitBorder(search_editbox_handle, FALSE);
    GUIEDIT_ConfigDisableChar(search_editbox_handle, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
    GUIEDIT_SetIm(search_editbox_handle, GUIIM_TYPE_ABC | GUIIM_TYPE_SMART | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH,GUIIM_TYPE_ABC);/*lint !e655*/
 
    GUIEDIT_SetStyle(search_editbox_handle,GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetTextMaxLen(search_editbox_handle,MMIPB_MUL_SEARCH_MAX_CHAR,MMIPB_MUL_SEARCH_MAX_CHAR);
   // GUIEDIT_SetFont(search_editbox_handle,SONG_FONT_14);
    GUIEDIT_SetDefaultStringById(search_editbox_handle, STXT_SEARCH);
    GUIEDIT_SetDispImIcon(search_editbox_handle, &is_disp_im_icon, &is_disp_num_info);  //cr211520, 不显示输入法图标



    // search icon button set
  
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_PB_MENU_SEARCH_ICON,win_id);
    width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
    width_info.add_data=img_width;
    GUIFORM_SetChildWidth(child_search_form_handle, child_search_button_handle, &width_info);
    GUIBUTTON_PermitBorder(child_search_button_handle, FALSE);
    GUIBUTTON_SetRunSheen(child_search_button_handle, FALSE);
    button_bg.bg_type = GUI_BG_IMG;
    button_bg.img_id = IMG_PB_MENU_SEARCH_ICON;
    GUIBUTTON_SetPressedBg(child_search_button_handle, &button_bg);

    

    // list set
    GUILIST_SetUserBg(list_handle, FALSE);
	GUILIST_PermitBorder(list_handle, FALSE);
	GUIFORM_PermitChildBg(main_form_handle, FALSE);
    height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
	GUIFORM_SetChildHeight(main_form_handle, list_handle, &height_info);
#ifndef MMI_PDA_SUPPORT 
//#ifndef TOUCH_PANEL_SUPPORT
	MMK_SetAtvCtrl(win_id, search_editbox_handle);
//#endif
#endif   
}

/*****************************************************************************/
//  Description : 将需要操作的项放入选项列表中
//  Global resource dependence :
//  Author: maryxiao
//  Note: is_operate: TRUE: OPERRATE
//                    FALSE:RELOAD
/*****************************************************************************/
LOCAL void AddSearchItemToOperateList(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, BOOLEAN is_operate)
{
     int i =0;
     GUILIST_ITEM_T*  item_ptr = PNULL;
     uint16             num_index = 0;
     MMIPB_APPLET_T*    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
     uint16 list_count = GUILIST_GetTotalItemNum(list_handle);
     MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};
                
     if(PNULL == applet_ptr)
     {
         return;
     }
     MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
    
    for(i=0; i< list_count; i++)
    {
        item_ptr = GUILIST_GetItemPtrByIndex(list_handle, i);/*lint !e605*/
        if(!(item_ptr->item_state&GUIITEM_STATE_SPLIT))
        {
            MMIPB_SEL_DATA sel_data = {0};
            num_index = item_ptr->user_data>>16;

		contact_node_info = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
		sel_data.entry_id = contact_node_info.contact_id;
		sel_data.storage_id = contact_node_info.storage_id;
		sel_data.num_index = num_index;
                
		if(MMIPB_LIST_FIELD_WHOLE == applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])
		{
			sel_data.sel_field = MMIPB_FEILD_FLAG_MASK_ALL;
		}
		else 
		{
			sel_data.sel_field =  MMIPB_FEILD_MASK_SINGLE_NUMBER << num_index;
		}
		
		MMIPB_CreateSelNode(applet_ptr->instance.handle,&sel_data);
	
              
        }
    }

}

/*****************************************************************************/
//  Description : 在通话记录中将号码加到已有姓名列表窗口的处理
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void  HandleEntryExPenOk(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, MMIPB_LIST_FIELD_TYPE_E display_field)
{

    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T applet_handle = NULL;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT 
	MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
#endif
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] HandleEntryExPenOk PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_13112_112_2_18_2_44_41_394,(uint8*)"");
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;

        if(applet_ptr->instance.function_type == MMIPB_SIGNLE_SELECT_LIST)
        {
#ifndef MMI_PDA_SUPPORT
            if(MMK_GetActiveCtrl(win_id) !=list_handle)
            {
                MMIPUB_OpenAlertWarningWin(TXT_PLS_SELECT_ONE_ITEM);
                return;
            }
#endif
            if(applet_ptr->instance.selelect_callback != PNULL)
            {
                const GUILIST_ITEM_T* item_ptr = PNULL;
                uint16                 cur_index = GUILIST_GetCurItemIndex(list_handle);
                uint16                 num_index =0;
                MMIPB_CONTACT_BASE_INFO_T   contact_base_node = {0};
                MMIPB_SEL_DATA         sel_data = {0}; 
if(0==cur_index)
{
AddSearchItemToOperateList(win_id, list_handle, TRUE);
}
else
{
                item_ptr = GUILIST_GetItemPtrByIndex(list_handle, cur_index);
                if(PNULL == item_ptr)
                {
                    return;
                }
                MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
                num_index = item_ptr->user_data>>16;
                contact_base_node = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
                sel_data.entry_id = contact_base_node.contact_id;
                sel_data.storage_id = contact_base_node.storage_id;
                sel_data.num_index = num_index;
                if(MMIPB_LIST_FIELD_WHOLE == display_field)
                {
                    sel_data.sel_field = MMIPB_FEILD_FLAG_MASK_ALL;
                }
                else 
                {
                    sel_data.sel_field =  MMIPB_FEILD_MASK_SINGLE_NUMBER << num_index;
                }
                MMIPB_CreateSelNode(applet_ptr->instance.handle,&sel_data);                
               }
applet_ptr->instance.selelect_callback(applet_ptr->instance.handle);
            } 
            MMK_CloseWin(win_id);
        }
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT        
        else// if(applet_ptr->instance.function_type == MMIPB_ADD_TO_EXIST_ENTRY)

        {
#ifndef MMI_PDA_SUPPORT
            if(MMK_GetActiveCtrl(win_id) !=list_handle)
            {
                MMIPUB_OpenAlertWarningWin(TXT_PLS_SELECT_ONE_ITEM);
                return;
            }
#endif
        
            // 打开号码类型列表的窗口
            if(applet_ptr->instance.add_type == MMIPB_ADD_TYPE_NUMBER)
            {     
                OpenSelectNumTypeWin(applet_handle, list_handle);
            }
            else 
            {
                MMIPB_MENU_DATA_INFOR_T add_data = {0};
				GUILIST_ITEM_T* list_item_ptr =PNULL;
				uint16 current_index = GUILIST_GetCurItemIndex(list_handle);
				uint16  contact_index = 0;

				list_item_ptr = GUILIST_GetItemPtrByIndex(list_handle, current_index);/*lint !e605*/
				if(PNULL  == list_item_ptr)
				{
					//SCI_TRACE_LOW:"[mmipb MMIPB_GetContactInfoFromListIndex] PNULL  == list_item_ptr %d"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_13170_112_2_18_2_44_41_395,(uint8*)"d", current_index);
					return;
				}


				contact_index = MMIPB_CONTACTINDEX(list_item_ptr->user_data);
                SCI_MEMSET(&add_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
                add_data.applet_handle = applet_handle;
                
                add_data.operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;
        
                add_data.contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                if(PNULL == add_data.contact_ptr)
                {
                    //SCI_TRACE_LOW:"[mmipb] HandleEntryListForExWinMsg alloc add_data.contact_ptr fail"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_13184_112_2_18_2_44_41_396,(uint8*)"");
                    return;
                }
                SCI_MEMSET(add_data.contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
				contact_node = MMIPB_GetContactBaseInfo(contact_index);

				MMIPB_GetContactInfo(contact_node.contact_id, contact_node.storage_id, add_data.contact_ptr);
                add_data.contact_ptr->storage_id = contact_node.storage_id;
#ifdef MMIPB_MAIL_SUPPORT
                if(applet_ptr->instance.add_type == MMIPB_ADD_TYPE_EMAIL)
                {
                    if(add_data.contact_ptr->mail.wstr_len>0 )
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_PB_OVERWRITE_OR_NOT, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
                    }
                    else
                    {
                        SCI_MEMCPY(&add_data.contact_ptr->mail, &(applet_ptr->instance.add_mail_t), sizeof(MMIPB_MAIL_T));
					    MMIPB_AddOrEditContactOfPb(&add_data);
                    }
                
                }
                else
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
                if(applet_ptr->instance.add_type == MMIPB_ADD_TYPE_URL)
                {
                    SCI_MEMCPY(&add_data.contact_ptr->url, &(applet_ptr->instance.add_url), sizeof(MMIPB_CUSTOM_TEXT_T));
                    add_data.contact_ptr->url.wstr_len = MMIAPICOM_Wstrlen(add_data.contact_ptr->url.wstr);
					MMIPB_AddOrEditContactOfPb(&add_data);
				//	MMIPB_OpenProcessWin(applet_handle, TXT_NULL, FALSE);
                }
                else
#endif
                if(applet_ptr->instance.add_type == MMIPB_ADD_TYPE_PIC)
                {
					MMIPB_ERROR_E result_error = MMIPB_ERROR_SUCCESS;//CR189426
					applet_ptr->instance.add_pic.is_valid = TRUE;

					result_error = MMIPB_AddContact_PhotoInfo(add_data.contact_ptr, &(applet_ptr->instance.add_pic));
                    if(MMIPB_ERROR_SUCCESS == result_error)
					{
						MMIPB_ModifyUnListContact(applet_ptr->instance.handle,add_data.contact_ptr);
                        MMIPB_OpenWaitWin(applet_handle, TXT_COMMON_WAITING);
					}
					else if (MMIPB_ERROR_NO_SPACE == result_error)
					{
						MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
					}
					else
					{
						MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
					}
					
					//MMIPB_OpenProcessWin(applet_handle, TXT_COMMON_WAITING, TXT_ADD_TO_EXIST_CONTACT,FALSE);
                }
                SCI_FREE(add_data.contact_ptr);
                add_data.contact_ptr = PNULL;
            }
        }
#endif
}
/*****************************************************************************/
//  Description : 在通话记录中将号码加到已有姓名列表窗口的处理
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEntryListForExWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    //MMI_HANDLE_T  main_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LIST_EX_FORM_CTRL_ID);
    MMI_HANDLE_T  search_editbox_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_EX_SEARCH_EDITBOX_CTRL_ID);
    MMI_HANDLE_T  list_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_ENTRY_LIST_FOR_EX_LISTBOX_CTRL_ID);    
    MMIPB_APPLET_T*  applet_ptr= (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    uint16 total_count =0;
    uint16 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
    MMI_HANDLE_T applet_handle = NULL;
    GUIITEM_STYLE_E   style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
	MMIPB_LIST_FIELD_TYPE_E field_type = MMIPB_LIST_FIELD_WHOLE;
	
    if(PNULL == applet_ptr)
    {
        return result;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
	field_type = applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type];
    switch (msg_id)
    {
    
    case MSG_OPEN_WINDOW:

		GUILIST_SetListState(list_handle, GUILIST_STATE_EFFECT_STR, TRUE);
        InitEntryExControl(win_id);

        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, PNULL);

        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        /*add by yao.chen for money assert:输入字母后快速按星号进入addsymbel*/
        if(MMK_IsFocusWin(win_id))
        {
        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);  
        }
        break;
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT        
     case MSG_NOTIFY_LIST_GET_EFFECT_STR:
        {
            MMI_STRING_T  edit_str = {0};
            GUILIST_EFFECT_STR_T *effect_str = (GUILIST_EFFECT_STR_T *)param;
            GUIEDIT_GetString(search_editbox_handle,&edit_str);
            AppendSearchItemColor(&applet_ptr->instance, win_id, effect_str, edit_str);
        }
        break;
#endif        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
	     SetEntryListItemForEx(win_id, applet_handle, list_handle,param,MMIPB_NEED_LIST_ITEM_DATA_TYPE, FALSE);
		break;

    case MMI_PB_RELOAD_SEARCH_LIST_MSG:
        {
            uint32 group = 0;
            MMI_STRING_T search_str ={0};            
#ifdef SNS_SUPPORT
            group  =MMIPB_GetAllSNSGroup();
#endif 
            GUIEDIT_GetString(search_editbox_handle, &search_str);
           /* GUILIST_ChangeDisplayType(
                list_handle,
                GUILIST_RADIOLIST_E,
                FALSE
                );
            style = GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT;*/
            
            style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
            if((applet_ptr->instance.add_type == MMIPB_ADD_TYPE_EMAIL)
                ||(applet_ptr->instance.add_type == MMIPB_ADD_TYPE_URL)
                ||(applet_ptr->instance.add_type == MMIPB_ADD_TYPE_PIC))
            {
                uint32 i =0;
                for(i = PB_GROUP_ALL; i< MMIPB_SYSTEM_GROUP_NUM; i++)
                {
                    if(MMIPB_IsInSIMGroup(i))
                    {
                        group += (MMIPB_CONVERT_GROUPID(i));
                    }
                }
            
            }
#endif
            total_count = MMIPB_MultiSearch(&search_str, MMIPB_ALL_GROUP_MASK, group, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
            GUILIST_SetMaxItem(list_handle, total_count*(1+MMIPB_MAX_NV_PHONE_NUM), TRUE );

           	MMIPB_SearchAndAppendItem(list_handle, applet_handle, applet_ptr->instance.cur_group_id, storage_id, style, field_type);
			
#ifndef MMI_PDA_SUPPORT
            if(0 == GUILIST_GetTotalItemNum(list_handle))
            {
                GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL, TXT_NULL, STXT_CANCEL,FALSE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,STXT_OK, TXT_NULL, STXT_CANCEL,FALSE);
            }
#endif 
                MMK_UpdateScreen();
            }
       
        break;
#ifdef TOUCH_PANEL_SUPPORT
   case MSG_CTL_PENOK:
       /* {
            MMI_NOTIFY_T notify = {0};
            uint16       current_index = 0;

            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            if(notify.src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
               MMK_CloseWin(win_id);
              MMIPB_CloseApplet(applet_handle);
             }
             else  if(notify.src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
             {
               MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
             }
        }*/
        MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        break;
#endif
        
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    case MSG_PROMPTWIN_OK://确定覆盖已有文件
        {
            MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
            MMIPB_MENU_DATA_INFOR_T add_data = {0};
		    GUILIST_ITEM_T* list_item_ptr =PNULL;
		    uint16 current_index = GUILIST_GetCurItemIndex(list_handle);
		    uint16  contact_index = 0;

		    list_item_ptr = GUILIST_GetItemPtrByIndex(list_handle, current_index);/*lint !e605*/
		    if(PNULL  == list_item_ptr)
		    {
                SCI_TRACE_LOW("[MMIPB] HandleEntryListForExWinMsg PNULL  == list_item_ptr");
			    break;
		    }


		    contact_index = MMIPB_CONTACTINDEX(list_item_ptr->user_data);
            SCI_MEMSET(&add_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
            add_data.applet_handle = applet_handle;
        
            add_data.operate_type = MMIPB_OPERATE_TYPE_EDIT_ONE;

            add_data.contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL == add_data.contact_ptr)
            {

                break;
            }
            SCI_MEMSET(add_data.contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
		    contact_node = MMIPB_GetContactBaseInfo(contact_index);
            add_data.contact_ptr->storage_id = contact_node.storage_id ;
		    MMIPB_GetContactInfo(contact_node.contact_id, contact_node.storage_id, add_data.contact_ptr);
            
            MMIPUB_CloseQuerytWin(PNULL);
#ifdef MMIPB_MAIL_SUPPORT
            SCI_MEMCPY(&add_data.contact_ptr->mail, &(applet_ptr->instance.add_mail_t), sizeof(MMIPB_MAIL_T));
#endif
		    MMIPB_AddOrEditContactOfPb(&add_data);
        }
        break;
#endif

    case MSG_APP_0:
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_APP_Q:
    case MSG_APP_W:
    case MSG_APP_E:
    case MSG_APP_R:
    case MSG_APP_T:
    case MSG_APP_Y:
    case MSG_APP_U:
    case MSG_APP_I:
    case MSG_APP_O:
    case MSG_APP_P:
    case MSG_APP_A:
    case MSG_APP_S:
    case MSG_APP_D:
    case MSG_APP_F:
    case MSG_APP_G:
    case MSG_APP_H:
    case MSG_APP_J:
    case MSG_APP_K:
    case MSG_APP_L:
    case MSG_APP_Z:
    case MSG_APP_X:
    case MSG_APP_C:
    case MSG_APP_V:
    case MSG_APP_B:
    case MSG_APP_N:
    case MSG_APP_M:
    case MSG_APP_COMMA:
    case MSG_APP_PERIOD:
    case MSG_APP_AT_QWERTY:
    case MSG_APP_SPACE:
    case MSG_APP_AND:	
    case MSG_APP_QUESTION:
    case MSG_APP_CTRL:
    case MSG_APP_STAR:
    case MSG_APP_PLUS:
    case MSG_APP_LPARENTHESIS:
    case MSG_APP_RPARENTHESIS:
    case MSG_APP_MINUS:
    case MSG_APP_DQUOTES: 
    case MSG_APP_SEMICOLON: 
    case MSG_APP_COLON:
    case MSG_APP_SLASH:
    case MSG_APP_SAND:
    case MSG_APP_EXCLAMATION:
#endif
    case MSG_APP_HASH:
    case MSG_TIMER:
        MMK_SetAtvCtrl(win_id, search_editbox_handle);  // for bug932849
        break;       

    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if(0 == GUILIST_GetTotalItemNum(list_handle))
        {
            break;
        }
        HandleEntryExPenOk(win_id, list_handle, MMIPB_LIST_FIELD_WHOLE);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        MMIPB_CloseApplet(applet_handle);
        break;  
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description :get group icon
//  Global resource dependence : 
//  Author:baokun
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetGroupIcon(uint8 group_id)
{
    MMI_IMAGE_ID_T image_id = 0;

   // if(group_id < MMIPB_SYSTEM_GROUP_NUM)
    {
         image_id = s_group_image_id[group_id];
    }
  /*  else
    {
        image_id = IMAGE_PHONEBOOK_GROUP7;
    }*/
    return image_id;
}


/*****************************************************************************/
//  Description :get group icon
//  Global resource dependence : 
//  Author:baokun
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetImageIcon(uint8 group_id)
{
    MMI_IMAGE_ID_T image_id = 0;

    image_id = s_photo_image_id[group_id];

    return image_id;
}


/*****************************************************************************/
//  Description :get group icon
//  Global resource dependence : 
//  Author:baokun
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetBigIcon(uint8 group_id)
{
    MMI_IMAGE_ID_T image_id = 0;

    image_id = s_big_image_id[group_id];

    return image_id;
}
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : update
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/

LOCAL void HandleResultForEntryEx(MMI_HANDLE_T applet_handle,  uint16  msg_id, MMIPB_ERROR_E error, MMIPB_OPC_E opc_type,uint16 count)
{
    MMIPB_APPLET_T*         applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    MMI_WIN_ID_T alert_id = MMK_GetWinHandle(applet_handle,MMIPB_WAITING_ALERT_WIN_ID); 
    MMI_WIN_ID_T progress_id = MMK_GetWinHandle(applet_handle,MMIPB_PROGRESS_WIN_ID);

    MMK_CloseWin(alert_id);
    MMIPUB_CloseProgressWin(&progress_id);

    if(PNULL ==  applet_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] HandleResultForCallLog PNULL ==  applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_13410_112_2_18_2_44_42_397,(uint8*)"");
        return;
    }

    if(msg_id == MMIPB_OPERATE_SUCCESS)
    {//所见即所得，去掉成功提示
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ADD_FAIL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    MMK_CloseWin(MMK_GetWinHandle(applet_handle, MMIPB_SELECT_NUM_TYPE_WIN_ID));
    MMK_CloseWin(MMK_GetWinHandle(applet_handle, MMIPB_ENTRY_LIST_FOR_EX_WIN_ID));

    MMIPB_CloseApplet(applet_ptr->caf_applet.app_handle);
}
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 此函数供外部将号码, url , pic保存到已有姓名时调用。
/*****************************************************************************/
LOCAL BOOLEAN OpenAddToExistWin(MMI_HANDLE_T applet_handle)
{
    MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_ENTRY_LIST_FOR_EX_WIN_TAB, PNULL);
    return TRUE;   
}
#endif

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 此函数供外部将号码, url , pic保存到已有姓名时调用。
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 此函数供外部将号码, url , pic保存到已有姓名时调用。
/*****************************************************************************/

PUBLIC void MMIPB_AddToExistContact
(
    void                          *   data_ptr,       
    MMIPB_ADD_TYPE_T       type,
    MMIPB_RETURN_CALLBACK_FUNC callback
)
{
	MMI_WIN_ID_T alert_win_id = MMIPB_MESSAGE_ALERT_WIN_ID;
    MN_DUAL_SYS_E dual_sys  = MN_DUAL_SYS_1;
    MMI_HANDLE_T  applet_handle = NULL;
    MMIPB_INSTANCE_T *pb_instance_ptr = PNULL;
#ifdef CMCC_VCARD_SUPPORT
/*	if(!MMIPB_IsUSIMReady())
	{
	      MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PB_NOT_READY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	      return;
	}*/
#endif
    if(!MMIAPIPB_IsPbReady())
	{
	    //phonebook processing, can not enter pb agin!!
        for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
        {
            if(SIM_NOT_INITED == MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_NOT_INIT,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                return;
            }
        } 
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SIM_ISNOT_READY,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
	}
    if(PNULL == data_ptr)
    {
	    return;
    }
    if((type == MMIPB_ADD_TYPE_NUMBER)&&(!MMIPB_IsValidPhoneNumber(((MMIPB_BCD_NUMBER_T*)data_ptr)->npi_ton, ((MMIPB_BCD_NUMBER_T*)data_ptr)->number_len)))
    {
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_INVALID_NUM,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	    return;
    }

    // 保存号码
    pb_instance_ptr = SCI_ALLOCA(sizeof(MMIPB_INSTANCE_T));
    if(pb_instance_ptr != PNULL)
    {
        SCI_MEMSET(pb_instance_ptr, 0X00, sizeof(MMIPB_INSTANCE_T));
        pb_instance_ptr->app_type = MMIPB_ALL_CONTACT;
        pb_instance_ptr->function_type = MMIPB_ADD_TO_EXIST_ENTRY;
        pb_instance_ptr->field_disp_type[MMIPB_ALL_CONTACT] = MMIPB_LIST_FIELD_WHOLE;
        pb_instance_ptr->add_type = type;
        pb_instance_ptr->list_type = MMIPB_LIST_RADIO;
        pb_instance_ptr->function_type = MMIPB_ADD_TO_EXIST_ENTRY;
        pb_instance_ptr->entry_func = OpenAddToExistWin;
        pb_instance_ptr->is_need_protect_verify = TRUE;
        if(type == MMIPB_ADD_TYPE_NUMBER)
        {
            pb_instance_ptr->add_number_t.npi_ton = ((MMIPB_BCD_NUMBER_T*)data_ptr)->npi_ton;
            pb_instance_ptr->add_number_t.number_len = ((MMIPB_BCD_NUMBER_T*)data_ptr)->number_len;
            SCI_MEMCPY(pb_instance_ptr->add_number_t.number,((MMIPB_BCD_NUMBER_T*)data_ptr)->number, ((MMIPB_BCD_NUMBER_T*)data_ptr)->number_len);
        }
        else if(type == MMIPB_ADD_TYPE_EMAIL)
        {
            SCI_MEMCPY(&(pb_instance_ptr->add_mail_t), ((MMIPB_MAIL_T*)data_ptr), sizeof(MMIPB_MAIL_T));
        }
        else if(type == MMIPB_ADD_TYPE_URL)
        {
            SCI_MEMCPY(&(pb_instance_ptr->add_url), ((MMIPB_CUSTOM_TEXT_T*)data_ptr), sizeof(MMIPB_CUSTOM_TEXT_T));
            
        }
        else if(type == MMIPB_ADD_TYPE_PIC)
        {
            SCI_MEMCPY(&(pb_instance_ptr->add_pic), ((MMIPB_CUSTOM_PHOTO_T*)data_ptr), sizeof(MMIPB_CUSTOM_PHOTO_T));
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIPB] MMIPB_OpenEntryListForEx type %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_13514_112_2_18_2_44_42_398,(uint8*)"d", type);
            SCI_FREE(pb_instance_ptr);
            return;
        }
        
        pb_instance_ptr->selelect_callback = callback;
        
        pb_instance_ptr->update_callback = HandleResultForEntryEx;
                
        applet_handle = MMIPB_StartApplet(pb_instance_ptr);
        SCI_FREE(pb_instance_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : 外部应用打开单选列表窗口
//  Global resource dependence : g_mmipb_bcd_number_t
//  Author: maryxiao
//  Note: 外部应用打开单选列表窗口
/*****************************************************************************/
PUBLIC void MMIPB_OpenSingleSelWin(MMI_HANDLE_T applet_handle)
{
    MMIPB_APPLET_T *applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtr(applet_handle);
    if(applet_ptr != PNULL)
    {
        applet_ptr->instance.function_type = MMIPB_SIGNLE_SELECT_LIST;
        applet_ptr->instance.list_type = MMIPB_LIST_RADIO;
        MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_ENTRY_LIST_FOR_EX_WIN_TAB, PNULL);        
    }
}


/*****************************************************************************/
//  Description : 操作前打开选择窗口的的子控件初始化
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void InitOperateSelControl(MMI_WIN_ID_T win_id, MMIPB_OPERATE_TYPE_E oper_type)
{

    GUIFORM_CHILD_HEIGHT_T              height_info = {0};
	GUIFORM_CHILD_WIDTH_T               width_info = {0};
    wchar                               forbid_char_value[]= {0x0d, 0x0a};//禁止换行回车的输入
    uint16                              img_width = 0;
    uint16                              img_height = 0;
    MMI_HANDLE_T  main_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SELECT_FORM_CTRL_ID);
    MMI_HANDLE_T  child_title_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_TITLE_OPERATE_SELECT_FORM_CTRL_ID);
#ifdef MMI_PDA_SUPPORT
    MMI_HANDLE_T  title_label_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_TITLE_CTRL_ID);
    MMI_HANDLE_T  select_all_button_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_SELECT_ALL_BUTTON_CTRL_ID);
#endif
  //  MMI_HANDLE_T  ok_button_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OK_BUTTON_CTRL_ID);
    MMI_HANDLE_T  child_search_button_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_SEARCH_BUTTON_CTRL_ID);
    MMI_HANDLE_T  child_search_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SEARCH_OPERATE_SELECT_FORM_CTRL_ID);
    MMI_HANDLE_T  search_editbox_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_SEARCH_EDITBOX_CTRL_ID);

    MMI_HANDLE_T  list_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID);
    IGUICTRL_T  *form_ptr = MMK_GetCtrlPtr(child_search_form_handle);
    GUI_BORDER_T  border = {0};
    GUI_BG_T                            button_bg = {0};
    
	//+Bug 119450
    BOOLEAN is_disp_im_icon = TRUE;
	BOOLEAN is_disp_num_info = TRUE;
    //-Bug 119450

    //GUIMAINFORM_Set
    GUIFORM_IsSlide(main_form_handle, FALSE);
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_PB_MENU_SEARCH_ICON,win_id);

    //GUICHILDFORM2_Set
    GUIFORM_PermitChildBorder(child_title_form_handle, FALSE);
    GUIFORM_PermitChildBorder(child_search_form_handle, TRUE);
    if (PNULL != form_ptr )
    {
        border.type = GUI_BORDER_SROUNDED;
        border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_DEFAULT_FONT);//MMI_BLACK_COLOR;
        border.width = 1;
        IGUICTRL_SetBorder(form_ptr , &border);
    }
    

    
#ifdef MMI_PDA_SUPPORT
    // lable set
  //  GUIFORM_SetChildAlign(child_title_form_handle, title_label_handle, GUIFORM_CHILD_ALIGN_RIGHT);
    width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
    width_info.add_data=SELECT_LABLE_LEN;
    GUIFORM_SetChildWidth(child_title_form_handle, title_label_handle, &width_info);
    GUILABEL_SetFont(title_label_handle, SONG_FONT_14, MMI_DEFAULT_TEXT_COLOR);
    GUILABEL_SetTextById(title_label_handle, TXT_PB_SELECT_ALL, FALSE);

    // all seleted button set
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CHECK_UNSELECTED_ICON,win_id);
    width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
    width_info.add_data=img_width;//MMI_LIST_ITEM_BIG_ICON_WIDTH+MMI_LIST_TEXT_LEFT_SPACE;
  //  GUIFORM_SetChildAlign(child_title_form_handle, select_all_button_handle, GUIFORM_CHILD_ALIGN_LEFT);
    GUIFORM_SetChildWidth(child_title_form_handle, select_all_button_handle, &width_info);
        
#endif

    //  search box set
    GUIEDIT_PermitBorder(search_editbox_handle, FALSE);
    GUIEDIT_ConfigDisableChar(search_editbox_handle, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
    GUIEDIT_SetIm(search_editbox_handle, GUIIM_TYPE_ABC | GUIIM_TYPE_SMART | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH,GUIIM_TYPE_ABC);/*lint !e655*/
    GUIEDIT_SetDispImIcon(search_editbox_handle,&is_disp_im_icon,&is_disp_num_info);
 
    GUIEDIT_SetStyle(search_editbox_handle,GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetTextMaxLen(search_editbox_handle,MMIPB_MUL_SEARCH_MAX_CHAR,MMIPB_MUL_SEARCH_MAX_CHAR);
   // GUIEDIT_SetFont(search_editbox_handle,SONG_FONT_14);
    GUIEDIT_SetDefaultStringById(search_editbox_handle, STXT_SEARCH);
//    width_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
//    GUIFORM_SetChildWidth(child_search_form_handle, search_editbox_handle, &width_info);


    // search icon button set
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_PB_MENU_SEARCH_ICON,win_id);
    width_info.type = GUIFORM_CHILD_WIDTH_FIXED;
    width_info.add_data=img_width;
    GUIFORM_SetChildWidth(child_search_form_handle, child_search_button_handle, &width_info);
    GUIBUTTON_PermitBorder(child_search_button_handle, FALSE);
    GUIBUTTON_SetRunSheen(child_search_button_handle, FALSE);
    button_bg.bg_type = GUI_BG_IMG;
    button_bg.img_id = IMG_PB_MENU_SEARCH_ICON;
    GUIBUTTON_SetPressedBg(child_search_button_handle, &button_bg);
    

    // list set

	GUILIST_PermitBorder(list_handle, FALSE);
	GUIFORM_PermitChildBg(main_form_handle, FALSE);
    height_info.type = GUIFORM_CHILD_HEIGHT_LEFT;
	GUIFORM_SetChildHeight(main_form_handle, list_handle, &height_info);
#ifdef MMI_PDA_SUPPORT
    GUILIST_SetUserBg(list_handle, FALSE);
#else
    GUIFORM_PermitChildFont(main_form_handle, FALSE);
    GUILIST_SetTextFont(list_handle, MMI_DEFAULT_NORMAL_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0));        
#endif

}




/*****************************************************************************/
//  Description : 将需要操作的项放入选项列表中
//  Global resource dependence :
//  Author: maryxiao
//  Note: is_operate: TRUE: OPERRATE
//                    FALSE:RELOAD
/*****************************************************************************/
LOCAL void AddCheckItemToOperateList(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, BOOLEAN is_operate)
{
     int i =0;
     uint16 index = 0;
     GUILIST_ITEM_T*  item_ptr = PNULL;
     uint16             num_index = 0;
     MMIPB_APPLET_T*    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
     uint16 list_count = GUILIST_GetTotalItemNum(list_handle);
     MMIPB_MENU_DATA_INFOR_T* menu_ptr =  (MMIPB_MENU_DATA_INFOR_T*)MMK_GetWinAddDataPtr(win_id);
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
     MMIPB_SEL_NODE_T* sel_node_ptr = PNULL;
#endif
     if(PNULL == applet_ptr)
     {
         return;
     }
     if(menu_ptr->operate_type != MMIPB_OPERATE_TYPE_GROUP_MANAGE)
     {
         MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
     }
    
    for(i=0; i< list_count; i++)
    {
        item_ptr = GUILIST_GetItemPtrByIndex(list_handle, i);/*lint !e605*/
        if(!(item_ptr->item_state&GUIITEM_STATE_SPLIT))
        {
            MMIPB_SEL_DATA sel_data = {0};
            num_index = item_ptr->user_data>>16;
#ifdef MMIPB_MOST_USED_SUPPORT
            if(applet_ptr->instance.cur_group_id != PB_GROUP_MOSTUSED)
#endif
            {
                MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};
                contact_node_info = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
                sel_data.entry_id = contact_node_info.contact_id;
                sel_data.storage_id = contact_node_info.storage_id;
                sel_data.num_index = num_index;
            }
#ifdef MMIPB_MOST_USED_SUPPORT
            else
            {
                MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node_info= {0};
                mostused_contact_node_info = MMIPB_GetMostUsedContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
                sel_data.entry_id = mostused_contact_node_info.entry_id;
                sel_data.storage_id = mostused_contact_node_info.storage_id;
                sel_data.num_index = num_index;
            }
#endif            
            if(GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(list_handle, i), GUIITEM_STATE_SELECTED))
            {

#ifdef MMIPB_GROUP_MANAGE_SUPPORT                
                if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_GROUP_MANAGE)
                {
                    sel_node_ptr = MMIPB_GetSelNode(applet_ptr->instance.handle, sel_data.entry_id, sel_data.storage_id, sel_data.num_index);
                    if(PNULL != sel_node_ptr)
                    {
                        sel_node_ptr->is_in_group=sel_node_ptr->is_in_group | 0x01;
                    }
                }
                else
#endif
                {
                
                    if(MMIPB_LIST_FIELD_WHOLE == applet_ptr->instance.field_disp_type[applet_ptr->instance.app_type])
                    {
                        sel_data.sel_field = MMIPB_FEILD_FLAG_MASK_ALL;
                    }
                    else 
                    {
                        sel_data.sel_field =  MMIPB_FEILD_MASK_SINGLE_NUMBER << num_index;
                    }
MMI_CheckAllocatedMemInfo();
                    MMIPB_CreateSelNode(applet_ptr->instance.handle,&sel_data);
MMI_CheckAllocatedMemInfo();
                }
            }
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
            else if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_GROUP_MANAGE)
            {
                sel_node_ptr = MMIPB_GetSelNode(applet_ptr->instance.handle, sel_data.entry_id, sel_data.storage_id, sel_data.num_index);
                if((PNULL != sel_node_ptr))
                {
                    sel_node_ptr->is_in_group=sel_node_ptr->is_in_group & 0xf0;
                }

            } 
#endif
            index++;
        }
    }

}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 处理全选按钮 pic refresh
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void UpdateSelectAllPic(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, MMI_HANDLE_T select_all_button_handle)
{
    GUI_BG_T                            button_bg = {0};
    MMIPB_APPLET_T*                     applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return;
    }
    
    if(applet_ptr->instance.is_all_select)
    {
        button_bg.bg_type = GUI_BG_IMG;
        button_bg.img_id = IMAGE_CHECK_SELECTED_ICON;
        GUIBUTTON_SetBg(select_all_button_handle, &button_bg);
        GUIBUTTON_SetPressedBg(select_all_button_handle, &button_bg);
    }
    else
    {
        button_bg.bg_type = GUI_BG_IMG;
        button_bg.img_id = IMAGE_CHECK_UNSELECTED_ICON;
        GUIBUTTON_SetBg(select_all_button_handle, &button_bg);
    }
    MMK_UpdateScreen();
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 处理全选按钮操作
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL   void HandleSelectAllFunc(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, MMI_HANDLE_T select_all_button_handle)
{
    //GUI_BG_T                            button_bg = {0};
    MMIPB_APPLET_T*                     applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return;
    }
    
    applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;

    GUILIST_SetAllSelected(list_handle, applet_ptr->instance.is_all_select);
    UpdateSelectAllPic(win_id,list_handle, select_all_button_handle);
   
}
#endif

/*****************************************************************************/
//  Description : 当前群组的联系人设置选中状态
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
void SetCheckToCurGroupItem(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle)
{
    uint16 list_count = GUILIST_GetTotalItemNum(list_handle);
    GUILIST_ITEM_T* item_ptr = PNULL;
    MMIPB_CONTACT_BASE_INFO_T  contact_node = {0};
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    int             i =0;
    MMIPB_MENU_DATA_INFOR_T* menu_ptr =  (MMIPB_MENU_DATA_INFOR_T*)MMK_GetWinAddDataPtr(win_id);
    MMIPB_SEL_DATA   sel_data_info = {0};
    if((PNULL == applet_ptr) || (PNULL == menu_ptr))
    {
        return;
    }
    if(MMIPB_GetSelectCount(applet_ptr->instance.handle))
    {
        return;
    }

    for(i=0; i< list_count; i++)
    {
        item_ptr = GUILIST_GetItemPtrByIndex(list_handle, i);/*lint !e605*/
        if(!(item_ptr->item_state&GUIITEM_STATE_SPLIT))
        {
            contact_node = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(item_ptr->user_data));
            sel_data_info.entry_id = contact_node.contact_id;
            sel_data_info.storage_id = contact_node.storage_id;
            
            if(menu_ptr->group & contact_node.group)
            {
                GUILIST_SetSelectedItem(list_handle, i, TRUE);
                sel_data_info.is_in_group = (1<<4);
                
            }
            else
            {
                sel_data_info.is_in_group = 0;
            }
            sel_data_info.is_manage_group = TRUE;
            MMIPB_CreateSelNode(applet_ptr->instance.handle, &sel_data_info);
        }            
    }
}
/*****************************************************************************/
//  Description : get which groups need to show
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetPbGroupFromOperate(MMI_HANDLE_T applet_handle, uint16 group_id, MMIPB_OPERATE_TYPE_E operate_type)
{
    uint32 group = MMIPB_ALL_GROUP_MASK;
     MMIPB_APPLET_T*  applet_ptr= (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
     if(PNULL ==applet_ptr)
     {
        return group;
     }
    switch(operate_type)
    {
#ifdef MMI_VCARD_SUPPORT
       case MMIPB_OPERATE_TYPE_EXPORT_CONTACT: 
#endif
       case MMIPB_OPERATE_TYPE_ERASE_ALL:
       case MMIPB_OPERATE_TYPE_ERASE_MULTI:
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
          
           {
                MMIPB_CONTACT_DISPLAY_TYPE_E display_type = MMIPB_ReadDisplayOpt();
                switch(display_type)
                {
                case MMIPB_CONTACT_DISPLAY_ALL:
                    group = MMIPB_ALL_GROUP_MASK;
                    break;
                case MMIPB_CONTACT_DISPLAY_PHONE:
                    group = MMIPB_ALL_GROUP_MASK;
                    break;
                case MMIPB_CONTACT_DISPLAY_SIM1:
                    group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_1);
                    break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                case MMIPB_CONTACT_DISPLAY_SIM2:
                    group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_2);
                    break;
#if (defined MMI_MULTI_SIM_SYS_TRI||defined MMI_MULTI_SIM_SYS_QUAD)
                case MMIPB_CONTACT_DISPLAY_SIM3:
                    group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_3);
                    break;
#endif
#ifdef MMI_MULTI_SIM_SYS_QUAD
                case MMIPB_CONTACT_DISPLAY_SIM4:
                    group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_4);
                    break;
#endif
#endif
                  default:
                    break;
                }
           }
#else
            group = MMIPB_ALL_GROUP_MASK;
#endif
           break;

        case MMIPB_OPERATE_TYPE_GROUP_MANAGE:
#ifdef MMIPB_MOST_USED_SUPPORT
        case MMIPB_OPERATE_TYPE_FAVOURITE_ADD:
#endif
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM1:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM2:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM3:
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM4:
#endif
#endif
#endif
            group = MMIPB_ALL_GROUP_MASK;
            break;


        case MMIPB_OPERATE_TYPE_SEND_GROUP_MSG:
            group = MMIPB_CONVERT_GROUPID(group_id);
            break;
#ifdef MMIPB_MOST_USED_SUPPORT
        case MMIPB_OPERATE_TYPE_FAVOURITE_REMOVE:
            group = MMIPB_CONVERT_GROUPID(PB_GROUP_FAVROUTE);
            break;
#endif        
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM1:
            group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_1);
            break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM2:
            group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_2);
            break; 
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM3:
            group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_3);
            break; 
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM4:
            group = MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_4);
            break;
#endif
#endif
#endif  
               
        case MMIPB_OPERATE_TYPE_COPY_TO_PHONE:
            group = MMIPB_GetAllSIMGroup();
            break;

        case MMIPB_OPERATE_TYPE_COPY:
             if(applet_ptr->instance.copy_from_index!=MMI_DUAL_SYS_MAX)
             {
                 group= MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_1 + applet_ptr->instance.copy_from_index);/*lint !e64 !e504 !e665*/
             }

            break;
	default:
		break;

    }

    return group;

}
/*****************************************************************************/
//  Description : et which groups need not  to show
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL uint32  GetPbMaskGroupFromOperate(MMI_HANDLE_T applet_handle,MMIPB_OPERATE_TYPE_E operate_type)
{
#ifdef SNS_SUPPORT    
    uint32 mask_group = MMIPB_GetAllSNSGroup();
#else
    uint32 mask_group = 0;
#endif
     MMIPB_APPLET_T*  applet_ptr= (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
     if(PNULL ==applet_ptr)
     {
        return mask_group;
     }
    switch(operate_type)
    {
#ifdef MMI_VCARD_SUPPORT
        case MMIPB_OPERATE_TYPE_EXPORT_CONTACT:
#ifdef MMI_PDA_SUPPORT    
            {
                MMIPB_CONTACT_DISPLAY_TYPE_E display_type = MMIPB_ReadDisplayOpt();
                mask_group = 0;
                if(MMIPB_CONTACT_DISPLAY_PHONE == display_type)
                {
                    mask_group = mask_group |  MMIPB_GetAllSIMGroup();
                }
            }
#endif  
            break;
#endif
            
        case MMIPB_OPERATE_TYPE_ERASE_ALL:
        case MMIPB_OPERATE_TYPE_ERASE_MULTI: 
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT      
            {
                MMIPB_CONTACT_DISPLAY_TYPE_E display_type = MMIPB_ReadDisplayOpt();
                if(MMIPB_CONTACT_DISPLAY_PHONE == display_type)
                {
                    mask_group = mask_group |  MMIPB_GetAllSIMGroup();
                }
            }
#endif
            break;

        case MMIPB_OPERATE_TYPE_SEND_GROUP_MSG:
#ifdef MMIPB_MOST_USED_SUPPORT
        case MMIPB_OPERATE_TYPE_FAVOURITE_REMOVE:    
#endif
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM1:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM2:
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM3:
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_FROM_SIM4:
#endif
#endif
#endif
        case MMIPB_OPERATE_TYPE_COPY_TO_PHONE:  
            break;
        case MMIPB_OPERATE_TYPE_COPY:
            if(applet_ptr->instance.copy_from_index==MMI_DUAL_SYS_MAX)
             {
                mask_group = mask_group |  MMIPB_GetAllSIMGroup();
             }
             break;

        case MMIPB_OPERATE_TYPE_GROUP_MANAGE:
            mask_group = mask_group |  MMIPB_GetAllSIMGroup();
            break; 
#ifdef MMIPB_MOST_USED_SUPPORT
        case MMIPB_OPERATE_TYPE_FAVOURITE_ADD:
            mask_group = mask_group | MMIPB_CONVERT_GROUPID(PB_GROUP_FAVROUTE) | MMIPB_GetAllSIMGroup();
            break; 
#endif
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM1:
            mask_group = mask_group | MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_1);
            break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM2:
            mask_group = mask_group | MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_2);
            break;
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM3:
            mask_group = mask_group | MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_3);
            break;
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MMIPB_OPERATE_TYPE_COPY_TO_SIM4:
            mask_group = mask_group | MMIPB_CONVERT_GROUPID(PB_GROUP_SIM_4);
            break;
#endif
#endif
#endif
	default:
	break;
    }
    return mask_group;

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void HandleOperateReloadList(MMI_WIN_ID_T win_id, MMI_HANDLE_T list_handle, MMIPB_MENU_DATA_INFOR_T* menu_ptr)
{
    MMIPB_APPLET_T*  applet_ptr= (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    MMI_HANDLE_T  search_editbox_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_SEARCH_EDITBOX_CTRL_ID);
    uint32           cur_index = 0;  
    uint32            total_count = 0;
    MMI_STRING_T  search_str = {0};
    //int           i =0;
    //uint32 exclude_group = 0;
    uint32 group = 0;
    uint32 mask_group = 0;
    MMI_HANDLE_T     applet_handle = NULL;
    GUIITEM_STYLE_E   style = GUIITEM_STYLE_TWO_LINE_CHECK_ICON;
    MMIPB_CONTACT_LIST_INFO_T   contact_list_info = {0};
    uint16 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
    uint8  group_id = 0;

    if(PNULL == applet_ptr)
    {
        return;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;
    cur_index = GUILIST_GetCurItemIndex(list_handle);
    GUIEDIT_GetString(search_editbox_handle, &search_str);
    if(GUILIST_GetTotalItemNum(list_handle)>0)
    {

       
        AddCheckItemToOperateList(win_id, list_handle, FALSE);
        GUILIST_RemoveAllItems(list_handle);
         
    }
    //MMK_SetAtvCtrl(win_id, list_handle);
    MMIPB_GetAllGroupIDs(menu_ptr->group, &group_id, 1);
    group = GetPbGroupFromOperate(applet_handle,group_id, menu_ptr->operate_type);
    mask_group = GetPbMaskGroupFromOperate(applet_handle,menu_ptr->operate_type);
    total_count = MMIPB_MultiSearch(&search_str, group, mask_group, (MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME + MMIPB_SEARCH_EMAIL));
    GUILIST_SetMaxItem(list_handle, total_count*2, TRUE );

    MMIPB_ReadContactList(&contact_list_info);


    if(MMIPB_OPERATE_TYPE_SEND_GROUP_MSG == menu_ptr->operate_type)
    {
//lzk mark
#ifdef BT_PBAP_SUPPORT
        total_count = total_count*MMIPB_MAX_NV_PHONE_NUM+contact_list_info.first_char_count;
#else
        total_count = total_count*MMIPB_MAX_NUM_TYPE+contact_list_info.first_char_count;
#endif
         style = GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT;
       //  GUILIST_SetMaxSelectedItem(list_handle, 100);
         applet_ptr->instance.max_select_count = 100;
    }
    else
    {
#ifdef MMI_GUI_STYLE_MINISCREEN
         style = GUIITEM_STYLE_ONE_LINE_CHECK_ICON;
#else
         style = GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT;
#endif 
         applet_ptr->instance.max_select_count = total_count;
        total_count = total_count+contact_list_info.first_char_count;
        
    }
    GUILIST_SetMaxSelectedItem(list_handle, total_count);
    GUILIST_SetMaxItem(list_handle, total_count, TRUE );

    //GUILIST_ChangeDisplayType(
    //    list_handle,
    //    GUILIST_CHECKLIST_E,
    //    FALSE
    //    );


   
    GUILIST_SetTextListMarkable(list_handle, TRUE);
#ifdef MMIPB_MOST_USED_SUPPORT       
    if((1 << PB_GROUP_MOSTUSED) == menu_ptr->group)
    {

        MMIPB_MostUsedSearchAndAppendItem(list_handle, applet_handle,style);
    }
    else
#endif

#ifdef BT_PBAP_SUPPORT
	if((1 << PB_GROUP_BT) == menu_ptr->group)
	{
		MMIPB_BtContactSearchAndAppendItem(list_handle, applet_handle, style);
        //MMIPB_BtContactSearchAndAppendItem(list_handle, applet_handle, PB_GROUP_ALL, storage_id, style, list_field_type);
	}
	else
#endif
    {
        MMIPB_LIST_FIELD_TYPE_E list_field_type = MMIPB_LIST_FIELD_WHOLE;
        if(MMIPB_OPERATE_TYPE_SEND_GROUP_MSG == menu_ptr->operate_type)
        {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            list_field_type = MMIPB_LIST_FIELD_NUMBER + MMIPB_LIST_FIELD_MAIL;/*lint !e656*/
#else           
            list_field_type = MMIPB_LIST_FIELD_NUMBER;
#endif
        }

        MMIPB_SearchAndAppendItem(list_handle, applet_handle, PB_GROUP_ALL, storage_id, style, list_field_type);
    }


    GUILIST_SetCurItemIndex(list_handle, (uint16)cur_index);              
    GUILIST_ValidCurItemPos(list_handle);

    if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_GROUP_MANAGE)
    {
        SetCheckToCurGroupItem(win_id, list_handle);
    }
}

/*****************************************************************************/
//  Description : 操作前打开选择窗口
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOperateSelWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
#ifdef MMI_PDA_SUPPORT
    MMI_HANDLE_T  main_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SELECT_FORM_CTRL_ID);
#endif
#if 0
    MMI_HANDLE_T  child_title_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_TITLE_OPERATE_SELECT_FORM_CTRL_ID);
    
    MMI_HANDLE_T  child_search_form_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_CHILD_SEARCH_OPERATE_SELECT_FORM_CTRL_ID);
#endif
    MMI_HANDLE_T  search_editbox_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_SEARCH_EDITBOX_CTRL_ID);
#ifdef MMI_PDA_SUPPORT
   // MMI_HANDLE_T  title_label_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_TITLE_CTRL_ID);
    MMI_HANDLE_T  select_all_button_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_SELECT_ALL_BUTTON_CTRL_ID);
#endif
    MMI_HANDLE_T  list_handle     = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID);    
   // MMI_CTRL_ID_T    ctrl_id = MMK_GetCtrlHandleByWin(win_id, MMIPB_OPERATE_SEL_LISTBOX_CTRL_ID);
    MMIPB_APPLET_T*  applet_ptr= (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    //uint16           total_count =0;
#ifndef MMI_PDA_SUPPORT  
    uint16 cur_index = 0;
#endif
    BOOLEAN          is_need_number = FALSE;
    
    MMI_HANDLE_T     applet_handle = NULL;
    MMI_TEXT_ID_T    title_id = TXT_NULL;
    
    MMIPB_MENU_DATA_INFOR_T* menu_ptr =  (MMIPB_MENU_DATA_INFOR_T*)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == applet_ptr)
    {
        return result;
    }
   // applet_ptr->instance.active_win_id = win_id;
    applet_handle = applet_ptr->caf_applet.app_handle;
    
    switch (msg_id)
    {
    
    case MSG_OPEN_WINDOW:
		GUILIST_SetListState(list_handle, GUILIST_STATE_EFFECT_STR, TRUE);
        // 打开列表控件
        InitOperateSelControl(win_id, menu_ptr->operate_type);

        GUILIST_ChangeCheckListSoftkeyIndex(list_handle, MIDDLE_BUTTON);

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error        
        MMIPB_CreateNewSoftKey(win_id, main_form_handle,TXT_COMMON_OK, TXT_NULL, STXT_CANCEL);
#else
        GUILIST_SetOwnSofterKey(list_handle,TRUE);
        //GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION, TXT_NULL, STXT_CANCEL,FALSE);
#endif

        title_id = GetTitleFromOperate(menu_ptr->operate_type);
        GUIWIN_SetTitleTextId(win_id, title_id, FALSE);
        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, PNULL);
        MMK_SetAtvCtrl(win_id, search_editbox_handle);

        break;
#ifdef MMIPB_SEARCH_RESULT_COLOR_SUPPORT        
     case MSG_NOTIFY_LIST_GET_EFFECT_STR:
        {
            MMI_STRING_T  edit_str = {0};
            GUILIST_EFFECT_STR_T *effect_str = (GUILIST_EFFECT_STR_T *)param;
            GUIEDIT_GetString(search_editbox_handle,&edit_str);
            AppendSearchItemColor(&applet_ptr->instance, win_id, effect_str, edit_str);
        }
         break;
#endif
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:

        MMK_SendMsg(win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);  
        break;
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        if(MMIPB_OPERATE_TYPE_SEND_GROUP_MSG == menu_ptr->operate_type)
        {
            is_need_number = TRUE;
        }
        SetEntryListItemForEx(win_id, applet_handle, list_handle,param, MMIPB_NEED_LIST_ITEM_DATA_TYPE, is_need_number);
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
         if(MMIPB_OPERATE_TYPE_SEND_GROUP_MSG == menu_ptr->operate_type)
         {
             is_need_number = TRUE;
         }
        SetEntryListItemForEx(win_id, applet_handle, list_handle,param, MMIPB_NEED_LIST_ITEM_CONTENT_TYPE, is_need_number);
        break;

    case MMI_PB_RELOAD_SEARCH_LIST_MSG:
        HandleOperateReloadList(win_id, list_handle, menu_ptr);
#ifdef MMI_PDA_SUPPORT
        {
            uint32 total_list_num = GUILIST_GetTotalItemNum(list_handle);
            uint32 select_list_num = GUILIST_GetSelectedItemNum(list_handle);
            if(total_list_num!=0)
            {
            
                if((select_list_num!=0)
                    &&(select_list_num ==  (total_list_num-applet_ptr->instance.split_num))
                    && !applet_ptr->instance.is_all_select)
                    {
                        applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;
                    }
    
                if(select_list_num !=  (total_list_num-applet_ptr->instance.split_num)
                    && applet_ptr->instance.is_all_select)
                    {
                        applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;
                    }   
    
            
                UpdateSelectAllPic(win_id, list_handle, select_all_button_handle);
            }
        }
#else
        if(0 == GUILIST_GetTotalItemNum(list_handle))
        {
            //GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL, TXT_NULL, STXT_CANCEL,FALSE);
            GUIEDIT_SetSoftkey(search_editbox_handle,0,1,TXT_NULL,TXT_NULL,PNULL);
            GUIEDIT_SetSoftkey(search_editbox_handle,1,1,TXT_NULL,TXT_NULL,PNULL);
            GUIEDIT_SetSoftkey(search_editbox_handle,2,1,STXT_RETURN,TXT_COMMON_CLEAR,PNULL);   
        }
        else
        {
            //GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION, TXT_NULL, STXT_CANCEL,FALSE);
            GUIEDIT_SetSoftkey(search_editbox_handle,0,1,STXT_OPTION,STXT_OPTION,PNULL);
            GUIEDIT_SetSoftkey(search_editbox_handle,1,1,TXT_NULL,TXT_NULL,PNULL);
            GUIEDIT_SetSoftkey(search_editbox_handle,2,1,STXT_RETURN,TXT_COMMON_CLEAR,PNULL);  
        }
#ifndef TOUCH_PANEL_SUPPORT
        //MMK_SetAtvCtrl(win_id, search_editbox_handle);
#endif
#endif 
    if(MMK_IsFocusWin(win_id)) //cr150331
    {
        //need full paint window
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
    break;
        
#ifndef MMI_PDA_SUPPORT        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if(list_handle == MMK_GetActiveCtrl(win_id))
        {
        
            cur_index = GUILIST_GetCurItemIndex(list_handle);
            GUILIST_SetSelectedItem(list_handle, cur_index, 
                                !GUILIST_GetItemState(GUILIST_GetItemPtrByIndex(list_handle, cur_index), 
                                GUIITEM_STATE_SELECTED)); /*lint !e730*/

            GUILIST_ReplaceItem(list_handle, GUILIST_GetItemPtrByIndex(list_handle, cur_index), cur_index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
    case MSG_APP_OK:    
    case MSG_CTL_OK: 
    case MSG_APP_MENU:                    
        if(GUILIST_GetTotalItemNum(list_handle)>0)
        {            
           // MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
          //  AddCheckItemToOperateList(win_id, list_handle, TRUE);
            menu_ptr->parent_win = win_id;
            menu_ptr->menu_type = MMIPB_OPERATE_SEL_OPT_MENU;
            menu_ptr->applet_handle = applet_handle;
            menu_ptr->ctl_id = MMK_GetCtrlId(list_handle);  //cr209656
            MMIPB_EnterPbTabOptMenu(menu_ptr);         
        }
        break;
 #else 
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_APP_MENU: 
#endif        
    case MMI_PB_OPERATE_OK:
         if((GUILIST_GetSelectedItemNum(list_handle)==0 )&&(menu_ptr->operate_type != MMIPB_OPERATE_TYPE_GROUP_MANAGE))
        {
            MMIPB_OpenAlertTipWin(applet_handle, MMIPB_ERROR_NO_SELECT_ENTRY,applet_ptr->instance.cmd);
        }
        else
        {
            if(GUILIST_GetSelectedItemNum(list_handle)> MMIPB_GetListAllowableMaxSelectedNum(applet_ptr->caf_applet.app_handle))
            {
               MMIPB_OpenAlertTipWin(applet_handle, MMIPB_ERROR_MAX_LIMIT,applet_ptr->instance.cmd);
            }
            else if(GUILIST_GetTotalItemNum(list_handle)>0)
            {
                AddCheckItemToOperateList(win_id, list_handle, TRUE);
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
                if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_GROUP_MANAGE)
                {
                    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
                    uint8         group_id = 0;
                    MMIPB_AbortPB(FALSE);
                    MMIPB_GetAllGroupIDs(menu_ptr->group, &group_id, 1);
                    ret = MMIPB_ManageGroupContactAsynForPB(applet_ptr->instance.handle, group_id);
                    if(ret == MMIPB_ERROR_PENDING)
                    {
                       MMIPB_OpenWaitWin(applet_handle,TXT_COMMON_WAITING);
                       //MMIPB_OpenProcessWin(applet_handle,TXT_COMMON_WAITING, TXT_PB_GROUP_MANAGE,FALSE);
                    }
					MMK_CloseWin(win_id);
                }
                else if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_SEND_GROUP_MSG)
                {
                    MMIPB_SendGroupMsg(applet_ptr->instance.handle);
					MMK_CloseWin(win_id);
                } 
                else
#endif                    
                if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_COPY)
                {
                    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
                    uint16 storage_id =0;
                    if(applet_ptr->instance.copy_to_index != MMI_DUAL_SYS_MAX)
                    {
                        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 +applet_ptr->instance.copy_to_index,PHONEBOOK_SIM_STORAGE);
                    }
                    else
                    {
                        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
                    }
                    ret = MMIPB_CopyMultiContactAsyn(applet_ptr->instance.handle, storage_id);
                    if(ret == MMIPB_ERROR_PENDING)
                    {
                       MMIPB_OpenWaitWin(applet_handle,TXT_COMMON_WAITING);
                       //MMIPB_OpenProcessWin(applet_handle,TXT_COMMON_WAITING, TXT_PB_GROUP_MANAGE,FALSE);
                    }
                    else 
                    {
                        MMIPB_OpenAlertTipWin(applet_handle, ret, MMIPB_OPC_COPY);
                        MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
                    }
					MMK_CloseWin(win_id);
                }
                else
                {
                    ///menu_ptr->parent_win = win_id;
                    if(MMIPB_HandleOperateSelCallBack(menu_ptr))
                    {
                    	MMK_CloseWin(win_id);
                    }
                }
            }
        }
        
        break;

    case MMI_PB_OPERATE_SELECT_ALL:
        MMIPB_HandleListFilter(win_id, list_handle, MMIPB_FILTER_ALL);
        break;

    case MMI_PB_OPERATE_UNSELECT_ALL:
        MMIPB_HandleListFilter(win_id, list_handle, MMIPB_FILTER_NONE);
        break; 

#ifdef CMCC_UI_STYLE
    case MMI_PB_OPERATE_SELECT_SIM:
        MMIPB_HandleListFilter(win_id, list_handle, MMIPB_FILTER_SIM);
        break;

    case MMI_PB_OPERATE_SELECT_PHONE:
        MMIPB_HandleListFilter(win_id, list_handle, MMIPB_FILTER_PHONE);
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_NOTIFY_T notify = {0};
            //uint16       current_index = 0;

            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
#ifdef MMI_PDA_SUPPORT
            if(notify.src_id == MMIPB_SELECT_ALL_BUTTON_CTRL_ID)
            {
                HandleSelectAllFunc(win_id, list_handle, select_all_button_handle);

            }
            else 
#endif
            if(notify.src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
               /* if((GUILIST_GetSelectedItemNum(list_handle)==0 )&&(menu_ptr->operate_type != MMIPB_OPERATE_TYPE_GROUP_MANAGE))
                {
                    MMIPB_OpenAlertTipWin(applet_handle, MMIPB_ERROR_NO_SELECT_ENTRY,applet_ptr->instance.cmd);
                }
                else if(GUILIST_GetSelectedItemNum(list_handle)> MMIPB_GetListAllowableMaxSelectedNum(applet_ptr->caf_applet.app_handle))
                {
                   MMIPB_OpenAlertTipWin(applet_handle, MMIPB_ERROR_MAX_LIMIT,applet_ptr->instance.cmd);
                }
                else if(GUILIST_GetTotalItemNum(list_handle)>0)
                {
                   
                    AddCheckItemToOperateList(win_id, list_handle, TRUE);
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
                    if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_GROUP_MANAGE)
                    {
                        MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
                        uint8         group_id = 0;

                        MMIPB_GetAllGroupIDs(menu_ptr->group, &group_id, 1);
                        MMIPB_AbortPB(FALSE);
                        ret = MMIPB_ManageGroupContactAsynForPB(applet_ptr->instance.handle, group_id);
                        if(ret == MMIPB_ERROR_PENDING)
                        {
                           //MMIPB_OpenProcessWin(applet_handle,TXT_NULL);
                             MMIPB_OpenWaitWin(applet_handle, TXT_COMMON_WAITING);
                        }
						 MMK_CloseWin(win_id);
                    }
                    else if(menu_ptr->operate_type == MMIPB_OPERATE_TYPE_SEND_GROUP_MSG)
                    {
                        MMIPB_SendGroupMsg(applet_ptr->instance.handle);
						 MMK_CloseWin(win_id);
                    }
                    else
#endif
                    {
                        //menu_ptr->parent_win = win_id;
                        if(MMIPB_HandleOperateSelCallBack(menu_ptr))
                        {
                        	 MMK_CloseWin(win_id);
                        }
                    }
                }*/
                MMK_SendMsg(win_id, MSG_CTL_OK, NULL);
               
             }
             else if(notify.src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                 MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
                MMK_CloseWin(win_id);
            }
             else
             {
#ifndef MMI_PDA_SUPPORT
            //check or uncheck
            MMK_SendMsg(win_id, MSG_APP_WEB, NULL);
#endif
             }
            
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527



    case MSG_CTL_LIST_MARK_ITEM:
        {
            GUILIST_MARK_ITEM_T *list_param_ptr = NULL;
            list_param_ptr = (GUILIST_MARK_ITEM_T*)param;
            if (list_param_ptr->had_maked)
            {
	            if(GUILIST_GetSelectedItemNum(list_handle) ==  (GUILIST_GetTotalItemNum(list_handle)-applet_ptr->instance.split_num)
		            && !applet_ptr->instance.is_all_select)
		            {
                        applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;
		            }
	            
            }
            else
            {
	            if(GUILIST_GetSelectedItemNum(list_handle) !=  (GUILIST_GetTotalItemNum(list_handle)-applet_ptr->instance.split_num)
		            && applet_ptr->instance.is_all_select)
		            {
                        applet_ptr->instance.is_all_select = !applet_ptr->instance.is_all_select;
		            }			
            }
            #ifdef MMI_PDA_SUPPORT
			UpdateSelectAllPic(win_id, list_handle, select_all_button_handle);
            #endif
        }     
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        MMK_SetAtvCtrl(win_id, list_handle);
        result = MMI_RESULT_FALSE;
		break;

    case MSG_APP_0:
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_APP_Q:
    case 	MSG_APP_W:	
    case 	MSG_APP_E:	
    case	MSG_APP_R:	
    case 	MSG_APP_T:	
    case 	MSG_APP_Y:	
    case    MSG_APP_U:	
    case 	MSG_APP_I:	
    case 	MSG_APP_O: 
    case 	MSG_APP_P:	
    case 	MSG_APP_A:	
    case 	MSG_APP_S:	
    case 	MSG_APP_D:  
    case 	MSG_APP_F:  
    case 	MSG_APP_G: 
    case 	MSG_APP_H:	
    case 	MSG_APP_J:	
    case 	MSG_APP_K:	
    case 	MSG_APP_L:	
    case 	MSG_APP_Z:	
    case 	MSG_APP_X:	
    case 	MSG_APP_C:	
    case 	MSG_APP_V: 
    case 	MSG_APP_B:	
    case 	MSG_APP_N:	
    case 	MSG_APP_M:	
    case    MSG_APP_COMMA:	
    case    MSG_APP_PERIOD:	
    case 	MSG_APP_AT_QWERTY:	
    case    MSG_APP_SPACE:	
    case    MSG_APP_AND:	
    case	MSG_APP_QUESTION:	
    case	MSG_APP_CTRL:	
    case MSG_APP_STAR:
    case  MSG_APP_PLUS:            
	case MSG_APP_LPARENTHESIS:    
	case  MSG_APP_RPARENTHESIS:    
	case  MSG_APP_MINUS:           
	case  MSG_APP_DQUOTES:         
	case MSG_APP_SEMICOLON:       
	case  MSG_APP_COLON:           
	case  MSG_APP_SLASH:           
	case MSG_APP_SAND:            
	case  MSG_APP_EXCLAMATION:     
#endif
    case MSG_APP_HASH:
    case MSG_TIMER:
       // MMK_CreateWinByApplet(applet_ptr->caf_applet.app_handle, MMIPB_SEARCH_NAME_WIN_TAB, PNULL); 
        MMK_SetAtvCtrl(win_id, search_editbox_handle);
		break;        
#endif        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
        MMK_CloseWin(win_id);
        break;  
     case MSG_CLOSE_WINDOW:
         MMK_FreeWinAddData(win_id);
         applet_ptr->instance.is_all_select = FALSE;
         break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 根据号码类型得到该类型的text id
//  Global resource dependence 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetContactTypeText(PHONEBOOK_STORAGE_E contact_type)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;

    switch(contact_type)
    {
    case PHONEBOOK_SIM_STORAGE:
        //sim contact
        text_id =TXT_DELETE;
        break;
#if defined (MMIPB_SIMDN_SUPPORT)
    case PHONEBOOK_MSISDN_STORAGE:
        //MSISDN :本机号码
        text_id =TXT_PB_MSISDN;
        break;
#endif
#ifdef FDN_SUPPORT
    case PHONEBOOK_FDN_STORAGE:
        //FDN contact：固定拨号
        text_id =TXT_FDN_NUM;
        break;
#endif
#if defined (MMIPB_SIMDN_SUPPORT)
    case PHONEBOOK_SDN_STORAGE:
        //sdn contact：服务号码
        text_id = TXT_PB_SDN;
        break;
#endif
    default:
        text_id = TXT_NULL;
        break;
    }
    return text_id;

}

/*****************************************************************************/
//  Description : 根据操作类型得到title
//  Global resource dependence : g
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTitleFromOperate(MMIPB_OPERATE_TYPE_E operate_type)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    switch(operate_type)
    {
    case MMIPB_OPERATE_TYPE_ERASE_ONE:
        text_id =TXT_DELETE;
        break;
    case MMIPB_OPERATE_TYPE_ERASE_MULTI:    
        text_id =TXT_PB_ERASE_MULTI;
        break;

    case MMIPB_OPERATE_TYPE_COPY:
        text_id =TXT_PB_COPY_CONTACT;
        break;
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM1:
        text_id =TXT_PB_IMPORT_SIM1;
        break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM2:
        text_id =TXT_PB_IMPORT_SIM2;
        break;
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM3:
        text_id =TXT_PB_IMPORT_SIM3;
        break;
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_FROM_SIM4:
        text_id =TXT_PB_IMPORT_SIM4;
        break;
#endif
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM1:
        text_id =TXT_PB_COPY_FROM_NV_TO_SIM1;
        break;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM2:
         text_id =TXT_PB_COPY_FROM_NV_TO_SIM2;
        break;
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM3:
         text_id =TXT_PB_COPY_FROM_NV_TO_SIM3;
        break;
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMIPB_OPERATE_TYPE_COPY_TO_SIM4:
         text_id =TXT_PB_COPY_FROM_NV_TO_SIM4;
        break;
#endif

    case MMIPB_OPERATE_TYPE_COPY_TO_PHONE:
        text_id =TXT_COMMON_COPY;
        break;
#ifdef MMI_VCARD_SUPPORT
    case MMIPB_OPERATE_TYPE_EXPORT_CONTACT:
        text_id =TXT_COMM_EXPORT;
        break;
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
    case MMIPB_OPERATE_TYPE_FAVOURITE_ADD:
        text_id =TXT_PB_ADD_FAVOURITE;
        break;

    case MMIPB_OPERATE_TYPE_FAVOURITE_REMOVE:
        text_id =TXT_PB_REMOVE_FAVOURITE;
        break;
#endif        
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    case MMIPB_OPERATE_TYPE_ADD_TO_GROUP:
    case MMIPB_OPERATE_TYPE_DELETE_GROUP:
        text_id =TXT_PB_MANAGE_GROUP_MEMBER;
        break;
    case MMIPB_OPERATE_TYPE_GROUP_MANAGE:
        text_id = TXT_PB_MANAGE_GROUP_MEMBER;
        break;
    case MMIPB_OPERATE_TYPE_SEND_GROUP_MSG:
        text_id = TXT_PB_WRITE_GROUP_MESSAGE;
        break;
#endif
    default:
        text_id = TXT_NULL;
        break;
    }
    return text_id;

}


/*****************************************************************************/
//  Description : 操作前打开选择窗口
//  Global resource dependence : g
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenOperationSelectWin(MMI_HANDLE_T applet_handle, MMIPB_MENU_DATA_INFOR_T* menu_ptr)
{

    MMIPB_MENU_DATA_INFOR_T* menu_data_ptr =  SCI_ALLOCA(sizeof(MMIPB_MENU_DATA_INFOR_T));
    if(PNULL == menu_data_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb], MMIPB_OpenOperationSelectWin alloc data fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_14529_112_2_18_2_44_44_399,(uint8*)"");
        return;
    }
    SCI_MEMSET(menu_data_ptr, 0X00, sizeof(MMIPB_MENU_DATA_INFOR_T));
    SCI_MEMCPY(menu_data_ptr, menu_ptr, sizeof(MMIPB_MENU_DATA_INFOR_T));
    MMK_CreateWinByApplet(applet_handle, (uint32*)MMIPB_OPERATION_SEL_LIST_WIN_TAB, (ADD_DATA)menu_data_ptr);

}

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
    void	*data_ptr,	   //IN: 输入的电话号码
    uint8    group_id,     //In:group id,
    uint16   storage_id,   //传入0，表示需要重新选择存储位置
    MMIPB_ADD_TYPE_T  type,
    BOOLEAN  is_protect    
    
)
{
    MMIPB_INSTANCE_T  pb_instance = {0};
    MMIPB_MENU_DATA_INFOR_T add_data = {0};
    MMIPB_APPLET_T*   applet_ptr = PNULL;
    BOOLEAN           is_new_applet = FALSE;

    if(applet_handle == PNULL)
    {
        pb_instance.update_callback = UpdateCallbackWithCloseApplet;
        pb_instance.function_type = MMIPB_ADD_ENTRY;
        applet_handle = MMIPB_StartApplet(&pb_instance);
        is_new_applet = TRUE;
    }

    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_14571_112_2_18_2_44_44_400,(uint8*)"");
        if(is_new_applet)
        {
            MMIPB_CloseApplet(applet_handle);
        }
        return;
    }
    SCI_MEMSET(&add_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
    add_data.applet_handle = applet_handle;
    
    add_data.operate_type = MMIPB_OPERATE_TYPE_ADD_ONE;
    
    add_data.contact_ptr =SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == add_data.contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] ALLOC add_data.contact_ptr ERROR "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_14586_112_2_18_2_44_44_401,(uint8*)"");
        if(is_new_applet)
        {
            MMIPB_CloseApplet(applet_handle);
        }
        return;
    }
    SCI_MEMSET(add_data.contact_ptr, 0x00,  sizeof(MMIPB_CONTACT_T));
    add_data.contact_ptr->storage_id = storage_id;
    if(PNULL != data_ptr && MMIPB_ADD_TYPE_MSISDN != type)
    {
			add_data.need_reset_storage = TRUE;
        //新增，需要添加特定内容
        if(MMIPB_ADD_TYPE_NUMBER == type)
        {
            SCI_MEMCPY(&add_data.contact_ptr->number[0], data_ptr, sizeof(MMIPB_BCD_NUMBER_T));
            add_data.contact_ptr->num_type[0] =MMIPB_ADN_NUMBER; 
        }
#ifdef MMIPB_MAIL_SUPPORT
        else if(MMIPB_ADD_TYPE_EMAIL == type)
        {

            SCI_MEMCPY(&add_data.contact_ptr->mail, data_ptr, sizeof(MMIPB_MAIL_T));
            add_data.need_reset_storage = FALSE;
            add_data.contact_ptr->storage_id =  MMIPB_STORAGE_PHONE;
        }   
#endif
        #ifndef PB_SUPPORT_LOW_MEMORY   
        else if(MMIPB_ADD_TYPE_URL == type)
        {
            SCI_MEMCPY(add_data.contact_ptr->url.wstr, data_ptr, sizeof(MMIPB_CUSTOM_TEXT_T ));
            add_data.contact_ptr->url.wstr_len = MMIAPICOM_Wstrlen(add_data.contact_ptr->url.wstr);
            add_data.need_reset_storage = FALSE;
            add_data.contact_ptr->storage_id =  MMIPB_STORAGE_PHONE;
        }
        #endif
    }
//#endif
    add_data.contact_ptr->union_t.group =  0;//MMIPB_CONVERT_GROUPID(group_id);
	//隐私检查
	if ((!MMIPB_GetProtectStatus()) && (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB)))
	{
		MMIAPISET_ValidatePrivacy(MMISET_PROTECT_PB, MMIPB_AddOrEditContactOfPbExt,(DPARAM)&add_data,sizeof(MMIPB_MENU_DATA_INFOR_T));
	}
	else
    {
        MMIPB_AddOrEditContactOfPb(&add_data);
        SCI_FREE(add_data.contact_ptr);
    }
}


/*****************************************************************************/
//  Description :  一般的详情页面
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note:  
//RETURN: 
/*****************************************************************************/
PUBLIC  void MMIPB_CreateDetailBasicDataChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIPB_ENTRY_DETAILS_WIN_TAB, PNULL);
}
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : SNS的详情页面
//  Global resource dependence : none
//  Author: starmer.sun
//  Note:  
//RETURN: 
/*****************************************************************************/
PUBLIC  void MMIPB_CreateSNSDetailBasicDataChildWin(MMI_WIN_ID_T	parent_win_id)
{
#ifdef MMI_PDA_SUPPORT
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIPB_SNS_ENTRY_DETAILS_WIN_TAB, PNULL);
#endif
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : none
//  Author: starmer.sun
//  Note:  
//RETURN: 
/*****************************************************************************/
PUBLIC  void MMIPB_CreatePBSnsActiveInfoChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIPB_SNS_ACTIVEINFO_WIN_TAB, PNULL);
}
#endif
#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
/*****************************************************************************/
//  Description : vcard中带保存按钮的详情页面
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note:  
//RETURN: 
/*****************************************************************************/
PUBLIC  void MMIPB_CreateOtherDetailBasicDataChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIPB_ENTRY_DETAILS_OTHER_WIN_TAB, PNULL);
}
#endif

//get rid of the useless code
#if 0
PUBLIC  MMI_TEXT_ID_T MMIPBAPI_GetPBNumTypeTextID(uint16 field_index)
{
    if (field_index>=(uint16)MMIPB_INDEX_OF_MAX)
        {
        return TXT_NULL;
        }

    
    return s_number_text_id[field_index];/*lint !e661 !e662*/
}
#endif

// PUBLIC  MMI_HANDLE_T MMIPB_GetDialWinHandleAndSwitchTab(MMI_WIN_ID_T  parent_win_id)
// {
// 
//     MMIPB_APPLET_T*  applet_ptr= (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(parent_win_id);
//     MMI_HANDLE_T     applet_handle = NULL;
//     MMI_HANDLE_T     dialWinHandle = NULL;
// 
//     applet_handle = applet_ptr->caf_applet.app_handle;
// 
//     SwitchToAppTab(applet_handle,MMIPB_DIAL_APPLICATION);
//     dialWinHandle =  MMK_GetWinHandle(applet_handle, MMIIDLE_DIAL_WIN_ID);
// 
//     return dialWinHandle;
// 
// }

/*****************************************************************************/
//  Description : 打开pb 详情主界面
//  Global resource dependence : 
//  Author:
//  Note: is_other:  是否为非iphone风格中vcard带保存的页面
/*****************************************************************************/

PUBLIC BOOLEAN MMIPB_OpenDetailWin(MMI_HANDLE_T  applet_handle, MMIPB_CONTACT_T *contact_ptr, BOOLEAN is_other)
{
    BOOLEAN result = FALSE;

	if(PNULL == contact_ptr)
	{
		//SCI_TRACE_LOW:"[MMIPB] MMIPB_OpenDetailWin, PNULL == contact_ptr"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_14776_112_2_18_2_44_44_403,(uint8*)"");
		return result;
	}

     
#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_IsMostUsedContact(contact_ptr->storage_id))
    {
       if(MMK_CreateWinByApplet(applet_handle,(uint32 *) MMIPB_MOSTUSED_DETAIL_WIN_TAB, contact_ptr)>0)
       {
    		result = TRUE;
       }
    }
    else
#endif
#if defined (MMIPB_SIMDN_SUPPORT)
    if(MMIPB_IsMsisdnContact(contact_ptr->storage_id))
    {
       if(MMK_CreateWinByApplet(applet_handle,(uint32 *) MMIPB_ENTRY_SIMDN_DETAILS_WIN_TAB, contact_ptr)>0)
       {
    		result = TRUE;
       }
    }
    else
#endif        
    if(MMK_CreateWinByApplet(applet_handle,MMIPB_DETAILS_MAIN_WIN_TAB, (ADD_DATA)contact_ptr)>0)/*lint !e605*/
    {
        PbCreateDetailChildWin(applet_handle,MMK_GetWinHandle(applet_handle, MMIPB_DETAILS_MAIN_WIN_ID), contact_ptr->storage_id, is_other);
        result = TRUE;
    }
#else 
     if(MMK_CreateWinByApplet(applet_handle,MMIPB_ENTRY_DETAILS_WIN_TAB, (ADD_DATA)contact_ptr)>0)/*lint !e605*/
     {
         result = TRUE;
     }
#endif
    return result;

}

#ifdef SNS_SUPPORT
/*****************************************************************************/
//     Description : set message list item
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void AppendSnsActiveItem(MMI_CTRL_ID_T ctrl_id,
                                                                    uint16 index,
                                                                    uint16* text_ptr,
                                                                    uint16* date_ptr)
{  
    GUIRICHTEXT_ITEM_T item_data = {0};
    uint16 item_index  = index;    
    
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    if(PNULL != text_ptr)
    {
        item_data.text_data.buf.str_ptr = (wchar *)text_ptr;  
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_data.text_data.buf.str_ptr);
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &item_index);  
    }

    if(PNULL != date_ptr)
    {
        item_index++;
        item_data.text_data.buf.str_ptr = (wchar *)date_ptr;  
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(item_data.text_data.buf.str_ptr);
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &item_index);
    }
}
#endif

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : PB列表的窗口处理函数
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSnsActiveInfoWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                )
{
    MMI_RESULT_E                        result                    = MMI_RESULT_TRUE;  
    //GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);    
    MMI_HANDLE_T parent_win_id = MMK_GetParentWinHandle(win_id);    
    MMI_HANDLE_T    list_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_SNS_ACTIVEINFO_LIST_CTRL_ID);        
    MMIPB_CONTACT_T * contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(parent_win_id);
    PbActiveData  *activeinfo[MMI_PB_ACTIVE_TEXT_NUM] = {0};
    MMI_STRING_T    str = {0};
    uint16  str_p[MMI_DATE_LEN+1] = {0};
    uint16 item_index  = 0;
    uint8 active_num = 0;
    uint8 group_id = 0;
    uint8 i = 0;
    
    switch (msg_id)
    {

        case MSG_OPEN_WINDOW: 
        case  MMI_PB_SNS_DETAIL_CHANGED:            
            MMIPB_GetAllGroupIDs(contact_ptr->union_t.group, &group_id, 1);
            active_num = MMIPB_GetSNSActiveInfo(group_id, contact_ptr->contact_id,contact_ptr->name, &activeinfo);/*lint !e64*/
            SCI_TRACE_LOW("[mmipb] HandleSnsActiveInfoWinMsg active_num = %d",active_num);
            //active_num = MIN(active_num,MMI_PB_ACTIVE_TEXT_NUM);
            if(0 == active_num)
            {
                MMIRES_GetText(TXT_NO_ENTRY,0,&str);
                SCI_MEMSET(str_p,0x00,sizeof(str_p));
                MMIAPICOM_Wstrncpy((wchar*)str_p,str.wstr_ptr,str.wstr_len);
                AppendSnsActiveItem(list_handle,
                                        item_index,
                                        (uint16*)&str_p,
                                        PNULL);
            }
            else
            {
                for(i = 0; i < MMI_PB_ACTIVE_TEXT_NUM; i++)
                {
                    if(PNULL != activeinfo[i])
                    {
                        if(0 != MMIAPICOM_Wstrlen(activeinfo[i]->text))
                        {
                            SCI_MEMSET(str_p,0x00,sizeof(str_p));
                            MMIWidget_SNSConvertTime(activeinfo[i]->date,str_p);
                            AppendSnsActiveItem(list_handle,
                                                    item_index,
                                                    activeinfo[i]->text,
                                                    str_p);
                            item_index +=2;
                        }
                        SCI_Free(activeinfo[i]);
                    }                
                } 
            }
            MMK_SendMsg(list_handle, MSG_CTL_PAINT, 0);
            break;
       case MSG_CTL_PENOK:
           {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                MMIPB_GotoSNSActiveWin(group_id, contact_ptr->contact_id);
            }
           }
           break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:Tracy Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSnsUpdateWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPB_AbortPB(FALSE);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        SCI_TRACE_LOW("MIPB]HandleSnsUpdateWaitWinMsg cancel");        
        MMIPB_SetPBStatus(MMIPB_STATUS_READY);
        MMIPB_AbortPB(TRUE);
	  MMK_CloseWin(win_id);	
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSnsUpdateWaitWin(MMI_HANDLE_T  applet_handle)
{
    MMI_STRING_T    prompt_str = {0};
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    MMIPUB_OpenWaitWinEx(applet_handle, 1,
                                &prompt_str,PNULL,PNULL,
                                MMIPB_WAITING_ALERT_WIN_ID,IMAGE_NULL,
                                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,
                                MMIPUB_SOFTKEY_ONE,
                                HandleSnsUpdateWaitWinMsg, 
                                PNULL);
}

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SnsNotLogin(MMI_HANDLE_T  applet_handle)
{    
         MMIPUB_OpenAlertWinByTextIdEx(applet_handle,PNULL,
                            TXT_PB_SNS_ACCOUNT_NOT_LOGIN,TXT_NULL,
                            IMAGE_PUBWIN_WARNING,
                            PNULL,PNULL,
                            MMIPUB_SOFTKEY_ONE,
                            PNULL,0);
}
#endif

#ifdef FDN_SUPPORT
/*****************************************************************************/
// 	Description : 取得sim卡特定号码列表的窗口数组
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetDailNumberListWinTab(void)
{
    return MMIPB_SIMDN_LIST_WIN_TAB;/*lint !e64*/
}
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口,
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/

LOCAL void PB_OpenMatchedItem(uint32 user_data)
{
    MMIPB_CONTACT_BASE_INFO_T  contact_node = {0};

    MMIPB_ReadContactSimpleInfo(MMIPB_CONTACTINDEX(user_data), &contact_node); 
    MMIPB_OpenContactWin(PNULL, contact_node.contact_id, contact_node.storage_id);
}
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : 本地搜索中pb匹配项的显示回调函数
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
LOCAL void PB_SetMatchItemData(
                            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr,
                            uint32 user_data
                             )
{
    //uint16              item_content_index = 0;
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    MMIPB_BCD_NUMBER_T   number_t = {0};
    MMI_STRING_T    disp_str = {0};
	uint8           num_type = 0;
    wchar           *temp1_wstr_ptr = PNULL;

    //GUIANIM_FILE_INFO_T anim_path = {0};

    uint16             valid_num = 0;
    MMI_IMAGE_ID_T     list_icon= 0;
    MMIPB_NAME_T      *entry_name_ptr = PNULL;
   // BOOLEAN           is_photo_id_exist = PNULL;
    //GUILIST_ITEM_T* list_item_ptr = PNULL;
    MMIPB_MAIL_T   *mail_info_ptr = PNULL;
    uint16 entry_id = 0;
    uint16 storage_id = 0;
    uint8 group_id = 0;
   // uint8 photo_id_flag = 0;
    int   i = 0;

    MMIPB_CONTACT_BASE_INFO_T contact_node ={0};

    MMIPB_CONTACT_T *contact = PNULL;  
//#ifdef PB_SUPPORT_LOW_MEMORY	
//    MMIPB_ACCESS_POINT_T access_point = {0};
//#endif
    MMIPB_CUSTOM_PHOTO_T                *custom_photo_ptr = PNULL;

    temp1_wstr_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));
    entry_name_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_NAME_T));
    mail_info_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_MAIL_T));
    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
    if(temp1_wstr_ptr == PNULL || entry_name_ptr == PNULL
        || mail_info_ptr == PNULL||custom_photo_ptr == PNULL
        )
    {
        if(temp1_wstr_ptr != PNULL)
        {
            SCI_FREE(temp1_wstr_ptr);
        }

        if(entry_name_ptr != PNULL)
        {
            SCI_FREE(entry_name_ptr);
        }
        if(mail_info_ptr != PNULL)
        {
            SCI_FREE(mail_info_ptr);
        }

        if(custom_photo_ptr != PNULL)
        {
            SCI_FREE(custom_photo_ptr);
        }        
        return;        
    }  
    disp_str.wstr_ptr = temp1_wstr_ptr;


    contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact, 0x00, sizeof(MMIPB_CONTACT_T));

    contact_node = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(user_data));
    entry_id = contact_node.contact_id;
    storage_id =contact_node.storage_id;
    MMIPB_GetAllGroupIDs(contact_node.group, &group_id, 1);
    MMIPB_GetContactInfo(entry_id, storage_id, contact);
    SCI_MEMCPY(entry_name_ptr ,&contact->name, sizeof(MMIPB_NAME_T));
   // photo_id_flag = contact->photo_id_flag;

    valid_num = MMIPB_GetFirstValidNumberIndex(contact->number, MMIPB_MAX_NV_PHONE_NUM);
    if(valid_num<MMIPB_MAX_NV_PHONE_NUM)
    {
    
        SCI_MEMCPY(&number_t, &(contact->number[valid_num]), sizeof(MMIPB_BCD_NUMBER_T));
#ifdef MMIPB_MAIL_SUPPORT
        *mail_info_ptr = contact->mail;
#endif
        num_type= contact_node.num_type[valid_num];
    }
    SCI_FREE(contact);


//    item_data.item_content[i].item_data_type = GUIITEM_DATA_IMAGE_ID;
//    item_data.item_content[i].item_data.image_id = IMAGE_SEARCH_PB;
//    i++;

	list_icon = GetImageIcon(group_id);


    /*if((photo_id_flag & MMIPB_PHOTO_ID_EXIST) && MMIPB_ReadPhotoFile(entry_id, storage_id, custom_photo_ptr))
    {
        if (MMIFILE_IsFileExist((const uint16 *)custom_photo_ptr->file_name,(uint16)(custom_photo_ptr->file_name_len)))
        {
        	is_photo_id_exist = TRUE;
        }
    }

    // add pic
    if(is_photo_id_exist)
    {
        item_data.item_content[i].item_data_type = GUIITEM_DATA_ANIM_PATH;
    }
    else*/
    {
        item_data.item_content[i].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[i].item_data.image_id = list_icon;
    }
    i++;


    SCI_MEMSET(temp1_wstr_ptr, 0x00, sizeof(wchar)*(MMIPB_MAX_STRING_LEN + 2));   
    disp_str.wstr_ptr = temp1_wstr_ptr;

    GetDisplayStringFromPhonebookName(&disp_str,
                                      entry_name_ptr
                                      );
    item_data.item_content[i].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[i].item_data.text_buffer = disp_str;

    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_VIEW;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
        
    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);

    SCI_FREE(temp1_wstr_ptr);
    SCI_FREE(entry_name_ptr);
    SCI_FREE(mail_info_ptr);
    SCI_FREE(custom_photo_ptr);
    
}
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : 本地搜索中pb匹配项的显示回调函数
//	Global resource dependence : 
//    Author:mary.xiao
//	Note: 
/*****************************************************************************/
LOCAL void PB_SetMatchItemContent(
                            GUILIST_NEED_ITEM_CONTENT_T *need_item_data_ptr,
                            uint32 user_data
                             )
{
    GUIITEM_CONTENT_T           item_content = {0};
    BOOLEAN           is_photo_id_exist = PNULL;
    GUIANIM_FILE_INFO_T anim_path = {0};

  //  MMI_IMAGE_ID_T     list_icon= 0;
    uint16 entry_id = 0;
    uint16 storage_id = 0;
    uint8 group_id = 0;
    uint8 photo_id_flag = 0;
   // int   i = 0;

    MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
    MMIPB_CONTACT_T *contact = PNULL;  
    MMIPB_CUSTOM_PHOTO_T                *custom_photo_ptr = PNULL;

    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
    if(custom_photo_ptr == PNULL)
    {
        return;        
    }

    contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact, 0x00, sizeof(MMIPB_CONTACT_T));

    contact_node = MMIPB_GetContactBaseInfo(MMIPB_CONTACTINDEX(user_data));
    entry_id = contact_node.contact_id;
    storage_id =contact_node.storage_id;
    MMIPB_GetAllGroupIDs(contact_node.group, &group_id, 1);
    MMIPB_GetContactInfo(entry_id, storage_id, contact);
    photo_id_flag = contact->photo_id_flag;
    
    if((photo_id_flag & MMIPB_PHOTO_ID_EXIST) && MMIPB_ReadPhotoFile(entry_id, storage_id, custom_photo_ptr))
    {
        if (MMIFILE_IsFileExist((const uint16 *)custom_photo_ptr->file_name,(uint16)(custom_photo_ptr->file_name_len)))
        {
            is_photo_id_exist = TRUE;
        }
    }

    // add pic
    if(is_photo_id_exist)
    {
        item_content.item_data_type = GUIITEM_DATA_ANIM_PATH;
        
        anim_path.full_path_wstr_ptr = custom_photo_ptr->file_name;
        anim_path.full_path_wstr_len = custom_photo_ptr->file_name_len;            
        item_content.item_data.anim_path_ptr =&anim_path;

        GUILIST_SetItemContent(need_item_data_ptr->ctrl_id,
            &item_content,
            need_item_data_ptr->item_index,
            need_item_data_ptr->item_content_index);
    }
        
    SCI_FREE(custom_photo_ptr);
}
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/

LOCAL void SearchPbLocalItem(MMISEARCH_DATA_T *match_data_ptr, uint32 find_num, uint32 max_num)
{
    uint16 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
    uint16 contact_index = 0;
    MMIPB_HANDLE_T        handle = 0;
    MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T    contact_valid_num_mail_info ={0};
    MMIPB_CONTACT_BASE_INFO_T  contact_node = {0};
    if(MMIPB_ERROR_SUCCESS != MMIPB_FindFirstContact(MMIPB_ALL_GROUP_MASK, storage_id, &contact_index, &contact_valid_num_mail_info,&contact_node, &handle))
    {
        //SCI_TRACE_LOW:"[MMIPB]CAN'T FIND ANY CONTACT TO INSERT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_15125_112_2_18_2_44_45_406,(uint8*)"");
        return;
    } 
    do
    {
        
        if(find_num > max_num)
        {
            break;
        }
        match_data_ptr->match_item_ptr[match_data_ptr->match_num].match_item_type = MMISEARCH_TYPE_PB;
        match_data_ptr->match_item_ptr[match_data_ptr->match_num].match_item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT;
        match_data_ptr->match_item_ptr[match_data_ptr->match_num].user_data = (match_data_ptr->match_num<<16) +contact_index;
        match_data_ptr->match_item_ptr[match_data_ptr->match_num].open_pfunc = PB_OpenMatchedItem;
        match_data_ptr->match_item_ptr[match_data_ptr->match_num].set_data_pfunc = PB_SetMatchItemData ;
        match_data_ptr->match_item_ptr[match_data_ptr->match_num].set_content_pfunc = PB_SetMatchItemContent;

        match_data_ptr->match_num++;
            
    }while(MMIPB_FindNextContact(handle,MMIPB_ALL_GROUP_MASK, storage_id, &contact_index, &contact_valid_num_mail_info,&contact_node) == MMIPB_ERROR_SUCCESS);
    MMIPB_FindClose(handle);
}


#ifdef MMIPB_SEARCH_CONTACT_CONTENT
LOCAL BOOLEAN s_is_async_search_pb_end;
LOCAL MMISEARCH_DATA_T s_pb_match_data;
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdatePbSearching(void)
{
    if (0 != s_pb_match_data.match_num && PNULL != s_pb_match_data.match_item_ptr)
    {
        if (!MMIAPISEARCH_SaveMatchItems(s_pb_match_data))
        {
            MMIAPISEARCH_IstyleStop();
        }

        SCI_FREE(s_pb_match_data.match_item_ptr);
        s_pb_match_data.match_num = 0;
    }
}

/*****************************************************************************/
// 	Description : 本地搜索中pb异步搜索回调函数
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
LOCAL void SearchFinishCallback(MMIPB_SEARCH_DATA_T* search_data_ptr)
{
    MMISEARCH_MATCH_ITEM_T* matched_item_ptr = PNULL;

    if (MMK_IsOpenWin(PB_SEARCH_EXIT_WAITING_WIN_ID))
    {
        MMK_CloseWin(PB_SEARCH_EXIT_WAITING_WIN_ID);
    }
    else if(PNULL == search_data_ptr)
    {
        SCI_TRACE_LOW("[mmipb] PNULL == search_data_ptr");
        return ;
    }
    else
    {
        matched_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(MIN(search_data_ptr->find_num, search_data_ptr->max_num)*sizeof(MMISEARCH_MATCH_ITEM_T));
        if (PNULL != matched_item_ptr)
        {
            s_pb_match_data.match_item_ptr = matched_item_ptr;
            SearchPbLocalItem(&s_pb_match_data , search_data_ptr->find_num,search_data_ptr->max_num);
            s_pb_match_data.match_num = MIN(search_data_ptr->find_num, search_data_ptr->max_num);
        }
    }

    MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_PB);
    MMIPB_CloseApplet(search_data_ptr->applet_handle);
    SCI_FREE(search_data_ptr);
    s_is_async_search_pb_end = TRUE;
    return ;
}
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_LocalAsyncSearch(const MMI_STRING_T *key_word_str)
{
    MMIPB_HANDLE_T        handle = 0;
    MMI_HANDLE_T        applet_handle = 0;
    MMIPB_INSTANCE_T    pb_instance = {0};
    MMIPB_SEARCH_DATA_T* search_data_ptr = PNULL;
    uint16              len = 0;
    MMIPB_APPLET_T*   applet_ptr = NULL;

    SCI_TRACE_LOW("[MMIPB] Entry MMIPB_LocalSearchAsync");
               
    if((!MMIPB_IsPbReady()))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_LocalSearchAsync not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_15112_112_2_18_2_44_45_404,(uint8*)"");
        return;
    }
    
    search_data_ptr = SCI_ALLOCA(sizeof(MMIPB_SEARCH_DATA_T));
    if(PNULL == search_data_ptr)
    {
        SCI_TRACE_LOW("MMIPB_LocalSearch PNULL == search_data_ptr");
        return;
    }
    SCI_MEMSET(search_data_ptr, 0x0, sizeof(MMIPB_SEARCH_DATA_T));
//    search_data_ptr->search_win_handle = win_handle;
    search_data_ptr->callback = (MMIPB_SEARCH_CALLBACK_FUNC)SearchFinishCallback;
    len = MIN(MMISEARCH_MAX_KEYWORD_LENS, key_word_str->wstr_len);
    SCI_MEMCPY(search_data_ptr->search_str, key_word_str->wstr_ptr,sizeof(wchar)*len);
    search_data_ptr->max_num = MMIAPIPB_GetMaxContactNum();
    search_data_ptr->max_num = MIN(search_data_ptr->max_num, MMISEARCH_MATCHED_NUM_MAX);

      applet_handle = MMIPB_StartApplet(&pb_instance);
	  search_data_ptr->applet_handle = applet_handle;
      
      if(applet_handle != 0 )
      {
          MMIPB_HANDLE_NODE_T handle_node={0};
          applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
          if(PNULL != applet_ptr)  //modify for coverity
          {
              handle = applet_ptr->instance.handle;
          }
           handle_node.data_len = sizeof(MMIPB_SEARCH_DATA_T);
           handle_node.data_ptr = search_data_ptr;
           if(!MMIPB_SetHandleContent(handle, &handle_node))
           {
               SCI_TRACE_LOW("[MMIPB] MMIPB_LocalSearchAsync MMIPB_SetHandleContent FAIL");
           }
            s_is_async_search_pb_end = FALSE;
            if (PNULL != s_pb_match_data.match_item_ptr)
            {
                SCI_FREE(s_pb_match_data.match_item_ptr);
            }
            s_pb_match_data.match_num = 0;
            MMIPB_SendSignalToPBTask(MMIPB_SIG_SEARCH,handle);
      }
      else
      {
          SCI_TRACE_LOW("[MMIPB] MMIPB_LocalSearchAsync MMIPB_StartApplet FAIL");
      }

}
/*****************************************************************************/
//  Description :handle saving email window message
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitingPbSearchExitWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_FULL_PAINT:
        err_code = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);        
        break;
                
    default :
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//     Description : stop async search pb
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_StopPbAsyncSearch(void)
{
    MMI_STRING_T wait_str   = {0};
    
    if (!MMIPB_IsPbAsyncSearchEnd())
    {
        if (!MMK_IsOpenWin(PB_SEARCH_EXIT_WAITING_WIN_ID))
        {
            SCI_TRACE_LOW("mmipb_view.c MMIPB_IsPbAsyncSearchEnd is not, Opening waiting window");
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
            MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,PB_SEARCH_EXIT_WAITING_WIN_ID,IMAGE_NULL,
               ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleWaitingPbSearchExitWin);
        }
    }
    else
    {
        if (PNULL != s_pb_match_data.match_item_ptr)
        {
            SCI_FREE(s_pb_match_data.match_item_ptr);
        }
        s_pb_match_data.match_num = 0;

        MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_PB);
    }
}

/*****************************************************************************/
//     Description : is pb search end
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPbAsyncSearchEnd(void)
{
    return (s_is_async_search_pb_end  == TRUE);
}
#else
/*****************************************************************************/
// 	Description : 本地搜索中pb搜索接口
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_LocalSearch(const MMI_STRING_T *key_word_str, MMI_HANDLE_T win_handle, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num)
{
    uint16 total_count = 0;
    //uint8  group_id = PB_GROUP_ALL;
//    uint16 contact_index = 0;
   // uint16 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
  //  MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T    contact_valid_num_mail_info ={0};
  //  MMIPB_CONTACT_BASE_INFO_T  contact_node = {0};
   // MMIPB_HANDLE_T        handle = 0;

    if((!MMIPB_IsPbReady()))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_LocalSearch not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_15112_112_2_18_2_44_45_404,(uint8*)"");
        return;
    }

    if (PNULL == match_data_ptr || PNULL == match_data_ptr->match_item_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb], MMIPB_LocalSearch PNULL == match_data_ptr || PNULL == match_data_ptr->match_item_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_VIEW_15118_112_2_18_2_44_45_405,(uint8*)"");
        return ;
    }
  
    total_count = MMIPB_MultiSearch(key_word_str, MMIPB_ALL_GROUP_MASK, 0, MMIPB_SEARCH_NAME);/*lint !e605*/

    SearchPbLocalItem(match_data_ptr, 0, max_num);
}
#endif
#endif

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPB_FreeLocalSearchResource(void)
{
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    if (PNULL != s_pb_match_data.match_item_ptr)
    {
        SCI_FREE(s_pb_match_data.match_item_ptr);
    }
    s_pb_match_data.match_num = 0;
#endif
}
#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : 创建蓝牙联系人页
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CreateBtContactTabWin(MMI_WIN_ID_T win_id)
{
	MMIPB_LIST_HANDLE* win_tab_infor_ptr = PNULL;
    MMI_HANDLE_T    applet_handle = {0};
    MMIPB_APPLET_T* applet_ptr = PNULL;
    
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        SCI_TRACE_LOW("MMIPB_CreateBtContactTabWin PNULL == applet_ptr");
        return;
    }
	
    applet_handle = applet_ptr->caf_applet.app_handle; 	
	win_tab_infor_ptr = (MMIPB_LIST_HANDLE *)SCI_ALLOC_APP(sizeof(MMIPB_LIST_HANDLE));
	if(PNULL == win_tab_infor_ptr)
	{
		SCI_TRACE_LOW("MMIPB_CreateBtContactTabWin PNULL == win_tab_infor_ptr");
		return;
	}
	
	SCI_MEMSET(win_tab_infor_ptr, 0x00, sizeof(MMIPB_LIST_HANDLE));
	win_tab_infor_ptr->win_id = MMK_GetWinHandle(applet_handle, MMIPB_BT_CONTACTS_LIST_TAB_WIN_ID);
	win_tab_infor_ptr->search_str.wstr_len = 0;
	SCI_MEMSET(win_tab_infor_ptr->search_str.wstr, 0x00, sizeof(win_tab_infor_ptr->search_str.wstr));
	win_tab_infor_ptr->contacts_type = MMIPB_BT_CONTACTS;
    win_tab_infor_ptr->group_id = PB_GROUP_BT;
	MMK_CreateChildWin(win_id, MMIPB_BT_CONTACTS_LIST_TAB_WIN_TAB, win_tab_infor_ptr);/*lint !e605*/ 	
}
/*****************************************************************************/
//  Description : 搜索bt contacts条目
//  Global resource dependence : 
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_BtContactSearchAndAppendItem(
											  MMI_HANDLE_T list_handle, 
											  MMI_HANDLE_T applet_handle, 
											  GUIITEM_STYLE_E  listitem_type
											  )
{
    uint32           list_data = 0;
    MMIPB_CONTACT_BASE_INFO_T  contact_node = {0};
    MMIPB_HANDLE_T   handle = 0;
    uint16           contact_index = 0;
    MMIPB_APPLET_T*  applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    
    if(PNULL == applet_ptr)
    {
        return;
    }
	
    applet_ptr->instance.split_num = 0;
    if(MMIPB_ERROR_SUCCESS != MMIPB_FindFirstBtContact(&contact_index, &contact_node,&handle))
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_FindFirstBtContact can't find any contact to insert");
        return;
    }  
    
    do
    {
        //第一个16位表示num的index
        //第二个16位表示记录在表中的index 
        list_data = MMIPB_CONTACTINDEX(contact_index);
        SetListItemForSearch(list_handle, listitem_type, list_data, 0, TRUE);//显示联系人
		
    }while(MMIPB_FindNextBtContact(handle, &contact_index, &contact_node) == MMIPB_ERROR_SUCCESS);
	
    MMIPB_FindClose(handle);
	
}
#endif


/*Edit by script, ignore 4 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527


/*Edit by script, ignore 13 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
