/*****************************************************************************
** File Name:      mmiwidget_pb.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#define _MMIWIDGET_PB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#include "mmi_app_widget_trc.h"
#ifdef MMIWIDGET_SUPPORT

#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmimms_export.h" 
#include "mmipub.h" 
//#include "mmicc_text.h"
#include "mmicc_export.h"
#include "guianim.h"
#include "mmipb_export.h"
#include "mmipb_image.h"
#include "mmipb_text.h"
#include "mmiwidget.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmiwidget_menutable.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmisms_text.h"
#include "mmiudisk_export.h"
#include "mmiwidget_position.h"
#include "mmipb_id.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#include "mmiphonesearch_export.h"
//#include "mmipb_app.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



#define MMIWIDGETPB_NUMBER_MAX_LEN   (MMIPB_BCD_NUMBER_MAX_LEN << 1)


#define MMIWIDGETPB_CALL_MENU_ITEM_DIMEN 2 
#define MMIWIDGETPB_CALL_MENU_TEXT_ID 1
#define MMIWIDGETPB_CALL_MENU_NODE_ID 0


//make call /send message win menu item
typedef enum
{
	WIDGETPB_OPTMENU_NODE_ROOT,
	WIDGETPB_OPTMENU_NODE_DIAL_AUDIO,
	WIDGETPB_OPTMENU_NODE_DIAL_VIDEO,
	WIDGETPB_OPTMENU_NODE_IPDIAL,
	WIDGETPB_OPTMENU_NODE_SEND_SMS,
	WIDGETPB_OPTMENU_NODE_SEND_MMS,
    	WIDGETPB_OPTMENU_NODE_MAX/*lint !e749*/
}WIDGETPB_OPTMENU_NODE_E/*lint !e751*/;

// send message type sms or mms
typedef enum _WIDGETPB_MESSAGE_INTERFACE_TYPE_E
{
	WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS,       // 发送信息
	WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS,       // 发送彩信
	WIDGETPB_MESSAGE_INTERFACE_TYPE_MAX
}WIDGETPB_MESSAGE_INTERFACE_TYPE_E;

//Favorites item icon
LOCAL MMI_IMAGE_ID_T  item_icon_id[MMIWIDGET_PB_WIDGET_NUM] = {
        (MMI_IMAGE_ID_T)IMAGE_NUMBER_1,
        (MMI_IMAGE_ID_T)IMAGE_NUMBER_2,
        (MMI_IMAGE_ID_T)IMAGE_NUMBER_3,
        (MMI_IMAGE_ID_T)IMAGE_NUMBER_4,
        (MMI_IMAGE_ID_T)IMAGE_NUMBER_5  
    };         


//点中区域的标识
typedef enum
{
    MMIWIDGET_PB_HIT_NONE,         //无
    MMIWIDGET_PB_HIT_CALL,        //拨号
    MMIWIDGET_PB_HIT_SMS,         //发短信

    MMIWIDGET_PB_HIT_MAX
}MMIWIDGET_PB_HIT_TYPE_E;

typedef enum _MMIWIDGET_INTERFACE_TYPE_E
{
	MMIWIDGET_INTERFACE_TYPE_MAKECALL_AUDIO,      // 语音电话,@zhaohui add
	#ifdef VT_SUPPORT
	MMIWIDGET_INTERFACE_TYPE_MAKECALL_VIDEO,//可视电话,@zhaohui add
	#endif
	//MMIGRID_INTERFACE_TYPE_MAKECALL,      // 拨号
	MMIWIDGET_INTERFACE_TYPE_MAKEIPCALL,    // IP拨号
	MMIWIDGET_INTERFACE_TYPE_SENDMSG,       // 发送信息
	MMIWIDGET_INTERFACE_TYPE_SETSPEEDKEY,   // 设置单键拨号
	MMIWIDGET_INTERFACE_TYPE_SETMTFW,       // 设置黑名单
	MMIWIDGET_INTERFACE_TYPE_MAX
}MMIWIDGET_INTERFACE_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//点中区域的标识
LOCAL uint32 s_widget_pb_hit_type = MMIWIDGET_PB_HIT_NONE;

//pb 图片显示区域

LOCAL const GUI_RECT_T s_pb_widget_arr[] = 
{
    MMIWIDGET_PB_WIDGET_0_RECT,
    MMIWIDGET_PB_WIDGET_1_RECT,
    MMIWIDGET_PB_WIDGET_2_RECT,
    MMIWIDGET_PB_WIDGET_3_RECT,
    MMIWIDGET_PB_WIDGET_4_RECT
};


//dial sub menu
LOCAL const uint32 s_widgetpb_dail_menu_sub_item[][MMIWIDGETPB_CALL_MENU_ITEM_DIMEN] =
{
    {WIDGETPB_OPTMENU_NODE_DIAL_AUDIO, TXT_CALL_AUDIO},//语音
#ifdef VT_SUPPORT
    {WIDGETPB_OPTMENU_NODE_DIAL_VIDEO, TXT_CALL_VIDEO},//视频
#endif        
#ifdef MMI_IP_CALL_SUPPORT
    {WIDGETPB_OPTMENU_NODE_IPDIAL, TXT_CALL_IPCALL}//IP拨号
#endif
};


//write messge sub men
LOCAL const uint32 s_widgetpb_sendmsg_menu_sub_item[][MMIWIDGETPB_CALL_MENU_ITEM_DIMEN] =
{
    {WIDGETPB_OPTMENU_NODE_SEND_SMS, TXT_SMS_NEW_SMS},    //短信
#ifdef MMS_SUPPORT
    {WIDGETPB_OPTMENU_NODE_SEND_MMS, TXT_SMS_NEW_MMS}    //彩信
#endif
};

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIWIDGETPB_FAVORITE_ENTRY_T  	s_widgetpb_entry_t = {0}; //记录收藏夹中所有记录在pb中entry id 和group id
LOCAL uint32  									s_favorite_curr_index = 0; //收藏夹打开时设置为高亮
//cr 193926
LOCAL BOOLEAN 									s_item_is_grayed = FALSE;  //设置收藏夹中记录的option菜单是否灰化
LOCAL BOOLEAN                           s_is_first_start = FALSE;



/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_InitFavoriteList (void);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void  MMIWIDGETPB_ReadNvFavoriteList(MMIWIDGETPB_FAVORITE_ENTRY_T *entry_t);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void  MMIWIDGETPB_WriteNvFavoriteList(MMIWIDGETPB_FAVORITE_ENTRY_T *entry_t);

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_OpenMakeCallWin( void);

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_OpenSendMessageWin(void );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 删除收藏夹中的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void RemoveFavorateOneItem(uint16 index);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_SendMessage(
                                                uint16 entry_id , 
                                                uint16 storage_id ,
                                                uint8 number_index,
                                                WIDGETPB_MESSAGE_INTERFACE_TYPE_E  message_interface_type
                                                );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_MakeCall(
                                                uint16 entry_id , 
                                                uint16 storage_id ,
                                                uint8 number_index,
                                                MMIWIDGET_INTERFACE_TYPE_E  interface_type
                                                );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_MakeNumberCallByBCD(
                                                MN_DUAL_SYS_E dual_sys,
                                                MMIWIDGET_INTERFACE_TYPE_E  interface_type, //IN:
                                                MMIPB_BCD_NUMBER_T      *bcd_number_ptr, //IN:
                                                CC_CALL_SIM_TYPE_E call_sim_type
                                                );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_MakeNumberCallByString(
                                                MN_DUAL_SYS_E dual_sys,
                                                MMIWIDGET_INTERFACE_TYPE_E  interface_type, //IN:
                                                MMIPB_STRING_T          *number_str_ptr, //IN:
                                                CC_CALL_SIM_TYPE_E call_sim_type
                                                );


/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteListDetailWindow(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteListMenuWindow(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
/*****************************************************************************/
//     Description : Handle WidgetPb make call Win Msg
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbMakeCallWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );
#endif

/*****************************************************************************/
//     Description : Handle WidgetPb send message Win Msg
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbWriteMessageWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
							);


/*****************************************************************************/
//     Description : Handle WidgetPb Favorite List Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbFavoriteListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void  AppendItemToFavoriteListBox(
                                        MMI_CTRL_ID_T ctrl_id,
                                        uint16		  index,
                                        MMIPB_PHONEBOOK_ENTRY_T* entry_ptr,
                                        MMI_TEXT_ID_T left_softkey_id,
                                        MMI_TEXT_ID_T middle_softkey_id,
                                        MMI_TEXT_ID_T right_softkey_id,
                                        BOOLEAN       is_update
                                        );  

/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void  MMIWIDGETPB_AppendItemToFavoriteListBox(
							MMIPB_PHONEBOOK_ENTRY_T  *load_entry_ptr, 	
                                                 uint8 number_index,
							uint16          		cur_index );


/*****************************************************************************/
//     Description : Handle WidgetPb Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetPbWinMsg(
                                                MMI_WIN_ID_T        win_id,        //IN:
                                                MMI_MESSAGE_ID_E    msg_id,        //IN:
                                                DPARAM            param        //IN:
                                                );


/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void DrawPbWidget(
                                                MMI_WIN_ID_T  win_id
                                                );


/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void CreateWidgetPbAnim(
                                                MMI_WIN_ID_T  win_id
                                                );

/*****************************************************************************/
// Description : get pb widget content by index
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void GetPbWidgetContentByIndex(
                                                MMI_WIN_ID_T  win_id,
                                                MMIPB_CUSTOM_PHOTO_T *custom_photo_ptr,    //out
                                                GUI_RECT_T *disp_rect_ptr,
                                                uint16 index
                                                );
/*****************************************************************************/
// Description : 更新大头贴
// Global resource dependence :
// Author: Cheng.Luo
// Note: 
/*****************************************************************************/
LOCAL void UpdateFavoriteListCustomPhoto(void);

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void SetAnimData(
                                                MMI_WIN_ID_T  win_id,
                                                uint16 index                       
                                                 );

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void AddAnimData(
                                                MMI_WIN_ID_T  win_id,
                                                uint16 index
                                                );


/*****************************************************************************/
//  Description :  TP 消息处理
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressDown(
                                                MMI_WIN_ID_T        win_id,        //IN:
                                                DPARAM            param        //IN:
                                                );


/*****************************************************************************/
// Description : TP 消息处理
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void HandleTpPressUp(
                                                MMI_WIN_ID_T  win_id,   
                                                DPARAM            param        //IN:
                                                );

/*****************************************************************************/
// Description : update favoritelist items
// Global resource dependence :
// Author:cheng.luo
// Note: 
/*****************************************************************************/
LOCAL void UpdateFavoriteListBoxItems(
                                     BOOLEAN is_update
                                     );


//cr 193926
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_SetItemGrayed( BOOLEAN  is_grayed   );

 /*****************************************************************************/
//    Description : reset pb button status.
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void ResetWidgetPb();

/*****************************************************************************/
// Description : load widget pb animation data.
// Global resource dependence :
// Author: 
// Note: 
/*****************************************************************************/
LOCAL void LoadWidgetPbAnimData(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : create pb anim draw timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateAnimDrawTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : stop pb anim draw timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopAnimDrawTimer(void);

/*****************************************************************************/
//  Description : restart pb anim draw timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartAnimDrawTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//	Description : get favorite entry name by index.
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void GetFavoriteEntryName(uint32 index, MMI_STRING_T *item_text);

/*****************************************************************************/
// Description : animation ctrl call back function to display entry name.
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void DrawEntryName(GUIANIM_OWNER_DRAW_T *owner_draw_ptr);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
/**                         Constant Variables                                */
/**---------------------------------------------------------------------------*/
//快速联系人列表窗口
LOCAL WINDOW_TABLE(MMIWIDGETPB_FAVORITE_LIST_WIN_TAB) = 
{
    WIN_TITLE(TXT_COMMON_PB ),
    WIN_FUNC((uint32) HandleWidgetPbFavoriteListWinMsg ),    
    WIN_ID( WIDGET_PB_FAVORITE_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID),
    END_WIN    
};
//收藏夹 成员选项窗口
LOCAL WINDOW_TABLE( MMIWIDGETPB_FAVORITE_LIST_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFavoriteListMenuWindow),
    WIN_ID(WIDGET_PB_FAVORITE_LIST_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_WIDGET_PB_FAVORITE_LIST_OPT,MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

// the window for FAVORITE_LIST detail menu
LOCAL WINDOW_TABLE( MMIWIDGETPB_FAVORITE_LIST_DETAIIL_WIN_TAB ) = 
{

    WIN_FUNC( (uint32)HandleFavoriteListDetailWindow ),    
    WIN_ID( WIDGET_PB_FAVORITE_LIST_DETAIIL_WIN_ID),
    WIN_TITLE( STXT_DETAIL ),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    CREATE_RICHTEXT_CTRL(MMIWIDGETPB_FAVORITE_LIST_DETAIL_CTRL_ID),
    END_WIN
};
//拨号选择窗口
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
LOCAL WINDOW_TABLE(MMIWIDGETPB_MAKECALL_WIN_TAB) = 
{
    WIN_TITLE( TXT_NULL ),
    WIN_FUNC((uint32) HandleWidgetPbMakeCallWinMsg ),    
    WIN_ID( WIDGET_PB_MAKECALL_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN    
};
#endif
//短信发送选择窗口
LOCAL WINDOW_TABLE(MMIWIDGETPB_SEND_MESSAGE_WIN_TAB) = 
{
    WIN_TITLE( TXT_NULL ),
    WIN_FUNC((uint32) HandleWidgetPbWriteMessageWinMsg ),    
    WIN_ID( WIDGET_PB_SEND_MESSAGE_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN    
};


/**--------------------------------------------------------------------------*
**                                                                                                                                   *
**--------------------------------------------------------------------------*/
PUBLIC const MMIWIDGET_ITEM_INFO_T g_pb_widget =
{
    MMIWIDGET_PB_ID, 
    HandleWidgetPbWinMsg, 
    PNULL,
    ResetWidgetPb,
    PNULL,    
    WIDGET_PB_WIN_ID, 
    IMAGE_WIDGET_PB_SHORTCUT, 
    IMAGE_WIDGET_PB_BG,
    TXT_COMMON_PB,
    50,//    50, 
    50,//    50
    FALSE
};


LOCAL uint8  s_pb_anim_timer_id = 0;
#define MMIWIDGET_PB_ANIM_DRAW_TIMEOUT           400
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : favorite contact add callback
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void FavoriteAddContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(WIDGET_PB_FAVORITE_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, handle);   
}

/*****************************************************************************/
//  Description : create pb anim draw timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateAnimDrawTimer(MMI_HANDLE_T win_id)
{
    s_pb_anim_timer_id = MMK_CreateWinTimer(win_id, MMIWIDGET_PB_ANIM_DRAW_TIMEOUT, FALSE);
}

/*****************************************************************************/
//  Description : stop pb anim draw timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopAnimDrawTimer(void)
{
    if (0 != s_pb_anim_timer_id)
    {
        MMK_StopTimer(s_pb_anim_timer_id);
        s_pb_anim_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : restart pb anim draw timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestartAnimDrawTimer(MMI_HANDLE_T win_id)
{
    StopAnimDrawTimer();
    CreateAnimDrawTimer(win_id);
}
 
 /*****************************************************************************/
//    Description : reset pb button status.
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void ResetWidgetPb()
{
    s_widget_pb_hit_type = MMIWIDGET_PB_HIT_NONE;
}

/*****************************************************************************/
//     Description : init favorite list
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_InitFavoriteList (void)
{
    	uint32 				i=0;

	for( i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++ )
	{

		s_widgetpb_entry_t.internal_entry_t[i].entry_id = 0;
		s_widgetpb_entry_t.internal_entry_t[i].storage_id = 0;
		s_widgetpb_entry_t.internal_entry_t[i].number_index = 0;
	}
	MMIWIDGETPB_ReadNvFavoriteList(&s_widgetpb_entry_t);
	

}
/*****************************************************************************/
//     Description : read favorite list from nv
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void  MMIWIDGETPB_ReadNvFavoriteList(MMIWIDGETPB_FAVORITE_ENTRY_T *entry_t)
{
    MN_RETURN_RESULT_E				return_value = MN_RETURN_FAILURE;
    uint32 				i=0;
    MMIWIDGETPB_FAVORITE_ENTRY_T  widgetpb_entry_t = {0};
	
    MMINV_READ(MMIWIDGETPB_FAVORITE_ENTRY, entry_t, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
    	for( i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++ )
	{
		widgetpb_entry_t.internal_entry_t[i].entry_id = 0;
		widgetpb_entry_t.internal_entry_t[i].storage_id_1 = 0;
		widgetpb_entry_t.internal_entry_t[i].number_index = 0;
	}

        MMINV_WRITE(MMIWIDGETPB_FAVORITE_ENTRY, &widgetpb_entry_t);
    }
    
    return ;

}
/*****************************************************************************/
//     Description : save favorite list to nv
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void  MMIWIDGETPB_WriteNvFavoriteList(MMIWIDGETPB_FAVORITE_ENTRY_T *entry_t)
{    
        MMINV_WRITE(MMIWIDGETPB_FAVORITE_ENTRY, entry_t);
}


/*****************************************************************************/
//  Description :根据entry id 和group id 获取pb 记录并发送信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_SendMessage(uint16 entry_id , uint16 storage_id ,uint8 number_index,WIDGETPB_MESSAGE_INTERFACE_TYPE_E  message_interface_type)
{

	MN_DUAL_SYS_E 	sim_type = MN_DUAL_SYS_1;
	PHONEBOOK_STORAGE_E storage = PHONEBOOK_NV_STORAGE;
       MMIPB_PHONEBOOK_ENTRY_T  sms_entry_t = {0};
	MMIPB_MAIL_T        mail_t = {0};
       BOOLEAN            is_include_email = FALSE;
	MMIPB_BCD_NUMBER_T smsnumber = {0}; 
	MMIPB_STRING_T         number_t    = {0};
       MMI_STRING_T 		string_to = {0};
	uint32        		index = 0; 


#ifndef MMI_MULTI_SIM_SYS_SINGLE
       sim_type = MN_DUAL_SYS_MAX;
#else
       sim_type = MN_DUAL_SYS_1;
#endif

#ifndef BROWSER_SUPPORT_NETFRONT
        if(WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS == message_interface_type  && (MMIAPICC_IsInState(CC_IN_CALL_STATE)))
        {
                MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
                return;
        }
#endif
        if(WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS == message_interface_type)
        {
                is_include_email = TRUE;
        }

  	sms_entry_t.entry_id  =entry_id;
  	sms_entry_t.storage_id_1 = storage_id;


  	if(sms_entry_t.entry_id == 0)
  	{
		return;
 	 }

	
     if(MMIPB_IsSIMContact( sms_entry_t.storage_id_1))
       {
            	storage = PHONEBOOK_SIM_STORAGE;
       }
       else
       {
            	storage = PHONEBOOK_NV_STORAGE;
        }					  
  
  	MMIAPIPB_ReadPhonebookEntey(&sms_entry_t);

  	if(PHONEBOOK_NV_STORAGE == storage)
        {
            if(MMIAPIPB_IsValidPhoneNumber(sms_entry_t.number_t[number_index].npi_ton, 
								       sms_entry_t.number_t[number_index].number_len)
		)
                {
                    //exist valid number
                    smsnumber = sms_entry_t.number_t[number_index];
                }
        }
       else
       {
                //get sim entry valid number
                smsnumber = sms_entry_t.number_t[0];
                index = 0;
       }

	   
      	if(sms_entry_t.mail.wstr_len >0)
       {
                //exist valid number or email address
                if(WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS == message_interface_type)
                {
                    mail_t = sms_entry_t.mail;
                }
        }											


        if(MMIAPIPB_IsValidPhoneNumber(smsnumber.npi_ton, smsnumber.number_len))
        {
            	// 将号码转换为字符串
            	number_t.strlen = MMIAPICOM_GenDispNumber(
                                                MMIPB_GetNumberTypeFromUint8(smsnumber.npi_ton),
                                                smsnumber.number_len,
                                                smsnumber.number,
                                                number_t.strbuf,
                                                MMIPB_MAX_STRING_LEN
                                                );
        }
        else if(mail_t.wstr_len > 0)
       {
            number_t.strlen = MIN(mail_t.wstr_len, MMIPB_MAX_STRING_LEN);
            MMI_WSTRNTOSTR((char *)number_t.strbuf, MMIPB_MAX_STRING_LEN, mail_t.wstr, MMIPB_MAX_STRING_LEN, number_t.strlen);/*lint !e64*/
       }
 	else
 	{ 
                MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);    
 	}

        if(number_t.strlen > 0)
        {
                if(WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS == message_interface_type)
                {
                    //send sms
                    MMIAPISMS_WriteNewMessage(
                        sim_type,
                        PNULL,
                        number_t.strbuf,
                        number_t.strlen
                        );

                }
                else
                {
                    //send mms
                    MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(number_t.strlen * sizeof(wchar));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, number_t.strlen, (char*)number_t.strbuf, number_t.strlen, number_t.strlen);/*lint !e64*/
                    string_to.wstr_len = number_t.strlen;
                    MMIAPIMMS_AnswerMMS(sim_type,PNULL,&string_to);
                    SCI_FREE(string_to.wstr_ptr);                  
                }

        }
        else
        {
                MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);    
        }



}
/*****************************************************************************/
//  Description :根据entry id 和group id 获取pb 记录并拨打电话
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_MakeCall(uint16 entry_id , uint16 storage_id , uint8 number_index,MMIWIDGET_INTERFACE_TYPE_E  interface_type)
{

	MMIPB_BCD_NUMBER_T 		callnumber = {0}; 
	MMIPB_PHONEBOOK_ENTRY_T  	call_entry_t = {0};
	PHONEBOOK_STORAGE_E 		storage = PHONEBOOK_NV_STORAGE;


	call_entry_t.entry_id = entry_id;
	call_entry_t.storage_id_1 = storage_id;
		
	if(call_entry_t.entry_id == 0)
	{
		return;
	}						
	else if(MMIAPIPB_ReadPhonebookEntey(&call_entry_t))
	{

		 if(MMIPB_IsSIMContact(call_entry_t.storage_id_1))
	         {
		          storage = PHONEBOOK_SIM_STORAGE;
	          }
	        else
	          {
		          storage = PHONEBOOK_NV_STORAGE;
	          }
		 switch (storage)
		    {
				case PHONEBOOK_SIM_STORAGE:
				        callnumber = call_entry_t.number_t[0];
				        break;

				case PHONEBOOK_NV_STORAGE:
				        {  					
							
				            
				            if(MMIAPIPB_IsValidPhoneNumber(call_entry_t.number_t[number_index].npi_ton, 
												call_entry_t.number_t[number_index].number_len)
												)
				            {
				                 callnumber = call_entry_t.number_t[number_index];               
				                 break;
				            }                           
				            else
				            {
				                // 打开提示窗口--没有号码
				                MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);    
				            }
				        }
				        break;

				default:
				        //SCI_TRACE_LOW:"mmipb_wintab: HandleInterfaceType storage = %d"
				        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_PB_851_112_2_18_3_8_25_223,(uint8*)"d",storage);
				        break;
		    }
	}

#ifndef MMI_MULTI_SIM_SYS_SINGLE						            
	                //make call by bcd number
		MMIWIDGETPB_MakeNumberCallByBCD(
								MN_DUAL_SYS_MAX,
								interface_type,
								&callnumber,
								CC_CALL_SIM_MAX
								);			
	           
#else						           
	                //make call by bcd number
		MMIWIDGETPB_MakeNumberCallByBCD(
								MN_DUAL_SYS_1,
								interface_type,
								&callnumber,
								CC_SIM1_CALL
								);						            
#endif

}
/*****************************************************************************/
//  Description :使用number 拨号
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_MakeNumberCallByBCD
(
    MN_DUAL_SYS_E dual_sys,
    MMIWIDGET_INTERFACE_TYPE_E  interface_type, //IN:
    MMIPB_BCD_NUMBER_T      *bcd_number_ptr, //IN:
    CC_CALL_SIM_TYPE_E call_sim_type
)
{
    MMIPB_STRING_T      number_t = {0};

    //SCI_ASSERT(PNULL != bcd_number_ptr);

    if (PNULL == bcd_number_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGETPB_MakeNumberCallByBCD PNULL == bcd_number_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_PB_894_112_2_18_3_8_25_224,(uint8*)"");
        return;
    }
    
    // 将号码转换为字符串
    number_t.strlen = MMIAPICOM_GenDispNumber(
                                        MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton),
                                        bcd_number_ptr->number_len,
                                        bcd_number_ptr->number,
                                        number_t.strbuf,
                                        MMIPB_MAX_STRING_LEN
                                        );
    MMIWIDGETPB_MakeNumberCallByString(dual_sys, interface_type, &number_t, call_sim_type);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_MakeNumberCallByString
(
    MN_DUAL_SYS_E dual_sys,
    MMIWIDGET_INTERFACE_TYPE_E  interface_type, //IN:
    MMIPB_STRING_T          *number_str_ptr, //IN:
    CC_CALL_SIM_TYPE_E call_sim_type
)
{
	

    if(PNULL == number_str_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MakeNumberCallByString number_str_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_PB_923_112_2_18_3_8_25_225,(uint8*)"");
        return;
    }
    switch (interface_type)
    {
    case MMIWIDGET_INTERFACE_TYPE_MAKECALL_AUDIO://语音电话
        MMIAPICC_MakeCall(
            dual_sys,
            number_str_ptr->strbuf,
            number_str_ptr->strlen,
            PNULL,
            PNULL,
            call_sim_type,
            CC_CALL_NORMAL_CALL,
            PNULL
            );
        break;
        
#ifdef VT_SUPPORT
    case MMIWIDGET_INTERFACE_TYPE_MAKECALL_VIDEO://可视电话
        MMIAPICC_MakeCall(
            dual_sys,
            number_str_ptr->strbuf,
            number_str_ptr->strlen,
            PNULL,
            PNULL,
            call_sim_type,
            CC_CALL_VIDEO_CALL,
            PNULL
            );
        break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
    case MMIWIDGET_INTERFACE_TYPE_MAKEIPCALL:
        MMIAPICC_MakeCall(
            dual_sys,
            number_str_ptr->strbuf,
            number_str_ptr->strlen,
            PNULL,
            PNULL,
            call_sim_type,
            CC_CALL_IP_CALL,
            PNULL
            );
        break;
#endif

    default:
        MMIAPICC_MakeCall(
            dual_sys,
            number_str_ptr->strbuf,
            number_str_ptr->strlen,
            PNULL,
            PNULL,
            call_sim_type,
            CC_CALL_NORMAL_CALL,
            PNULL
            );
        break;
    }
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteListDetailWindow(
														MMI_WIN_ID_T win_id,
														MMI_MESSAGE_ID_E msg_id, 
														DPARAM param
														)
{
	MMI_RESULT_E  	result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T 	ctrl_id = MMIWIDGETPB_FAVORITE_LIST_DETAIL_CTRL_ID;
	uint32	sel_item_index =0;
	MMIPB_PHONEBOOK_ENTRY_T detail_entry_ptr ={0};
	switch(msg_id)
	{

    		case MSG_OPEN_WINDOW:

			sel_item_index = GUILIST_GetCurItemIndex(MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID);
			detail_entry_ptr.entry_id = s_widgetpb_entry_t.internal_entry_t[sel_item_index].entry_id;
			detail_entry_ptr.storage_id = s_widgetpb_entry_t.internal_entry_t[sel_item_index].storage_id;
				
			if(detail_entry_ptr.entry_id >0)
			{
				if(MMIAPIPB_ReadPhonebookEntey(&detail_entry_ptr))
				{
					MMIPB_SetEntryDetail(ctrl_id,&detail_entry_ptr);
			
				}
			}
       		MMK_SetAtvCtrl(win_id, ctrl_id);
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

//cr 193926
/*****************************************************************************/
//     Description : 是否灰化option 菜单，记录为空时灰化
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_SetItemGrayed( BOOLEAN  is_grayed   )
{
	GUIMENU_SetItemGrayed(MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID,MENU_WIDGET_PB_FAVORITE_LIST_OPT, ID_WIDGET_LOG_DETAIL, is_grayed);
	GUIMENU_SetItemGrayed(MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID,MENU_WIDGET_PB_FAVORITE_LIST_OPT, ID_WIDGET_LOG_LIST_DIAL, is_grayed);
	#ifdef MMS_SUPPORT
	GUIMENU_SetItemGrayed(MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID,MENU_WIDGET_PB_FAVORITE_LIST_OPT, ID_WIDGET_LOG_LIST_SEND, is_grayed);
	#else
	GUIMENU_SetItemGrayed(MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID,MENU_WIDGET_PB_FAVORITE_LIST_OPT, ID_WIDGET_SEND_SMS, is_grayed);
	#endif
	GUIMENU_SetItemGrayed(MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID,MENU_WIDGET_PB_FAVORITE_LIST_OPT, ID_WIDGET_LOG_LIST_DELETE, is_grayed);

}
/*****************************************************************************/
//     Description :收藏夹 成员选项窗口处理函数
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteListMenuWindow(
														MMI_WIN_ID_T win_id,
														MMI_MESSAGE_ID_E msg_id, 
														DPARAM param
														)
{
	MMI_RESULT_E  						result = MMI_RESULT_TRUE;
    	uint32          							storage_id = 0, menu_id = 0;
	uint32								sel_item_index =0;
	MMI_CTRL_ID_T 						ctrl_id = MMIWIDGETPB_FAVORITE_LIST_MENU_CTRL_ID;
	MMIWIDGET_INTERFACE_TYPE_E  				interface_type = MMIWIDGET_INTERFACE_TYPE_MAKECALL_AUDIO;
	WIDGETPB_MESSAGE_INTERFACE_TYPE_E  message_interface_type = WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS;
	switch(msg_id)
	{

    		case MSG_OPEN_WINDOW:
       		MMK_SetAtvCtrl(win_id, ctrl_id);
			
			//cr 193926
			MMIWIDGETPB_SetItemGrayed(s_item_is_grayed);
			break;

		case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_MIDSK:
		case MSG_CTL_OK:
		case MSG_APP_OK:
			GUIMENU_GetId(ctrl_id,&storage_id,&menu_id);

			//获取当前记录在收藏夹中的index
			sel_item_index = GUILIST_GetCurItemIndex(MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID);
		       switch (menu_id)
		        {
				case ID_WIDGET_LOG_DETAIL:
					
           			//MMK_CreateWin((uint32 *)MMIWIDGETPB_FAVORITE_LIST_DETAIIL_WIN_TAB, PNULL);
                    
                    {
                        uint16 cur_entry_id = 0;
                        uint16 cur_storage_id = 0;
                        
                        cur_entry_id = s_widgetpb_entry_t.internal_entry_t[sel_item_index].entry_id;
                        cur_storage_id = s_widgetpb_entry_t.internal_entry_t[sel_item_index].storage_id;
                        
                        if(cur_entry_id >0)
                        {
                            MMIAPIPB_OpenContactWin(cur_entry_id, cur_storage_id);
                        } 
                        
                        MMK_CloseWin(win_id);
                    }
						break;
						
				case ID_WIDGET_SEND_SMS:

						message_interface_type = WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS;
						MMIWIDGETPB_SendMessage(
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].entry_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].storage_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].number_index,
								message_interface_type
								);
						MMK_CloseWin(win_id);
						break;
						
				case ID_CWIDGET_SEND_MMS:

						message_interface_type = WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS;
						MMIWIDGETPB_SendMessage(
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].entry_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].storage_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].number_index,
								message_interface_type
								);
						MMK_CloseWin(win_id);
						break;
						
				case ID_WIDGET_LOG_LIST_DELETE:
            					MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
						break;
						
				case ID_WIDGET_DIAL_AUDIOCALL:
						interface_type = MMIWIDGET_INTERFACE_TYPE_MAKECALL_AUDIO;
						MMIWIDGETPB_MakeCall(
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].entry_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].storage_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].number_index,
								interface_type
								);
						MMK_CloseWin(win_id);
						break;
						
#ifdef VT_SUPPORT
				case ID_WIDGET_DIAL_VIDEOCALL:
						interface_type = MMIWIDGET_INTERFACE_TYPE_MAKECALL_VIDEO;
						MMIWIDGETPB_MakeCall(
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].entry_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].storage_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].number_index,
								interface_type
								);
						MMK_CloseWin(win_id);
						break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
				case ID_WIDGET_DIAL_IPCALL:
						interface_type = MMIWIDGET_INTERFACE_TYPE_MAKEIPCALL;
						MMIWIDGETPB_MakeCall(
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].entry_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].storage_id,
								s_widgetpb_entry_t.internal_entry_t[sel_item_index].number_index,
								interface_type
								);
						MMK_CloseWin(win_id);
						break;	
#endif
						
				 default:
				              break;

			 }
			 break;

    		case MSG_PROMPTWIN_OK:
				//获取当前记录在收藏夹中的index
				sel_item_index = GUILIST_GetCurItemIndex(MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID);
				
        			//关闭记录的menu窗口和询问窗口
        			GUIMENU_GetId(ctrl_id,&storage_id,&menu_id);
			        switch (menu_id)
			        {
			            
			        	case ID_WIDGET_LOG_LIST_DELETE:
			            		MMIPUB_CloseQuerytWin(PNULL);
			            		MMIWIDGETPB_DeleteFavoriteListItemByIndex(sel_item_index);
							    MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
								MMK_CloseWin(win_id);
//								MMK_CloseWin(WIDGET_PB_FAVORITE_LIST_WIN_ID);
			            	break;
			            
			            
			        	default:
			            	break;
			        }
        		break;
	 	case MSG_PROMPTWIN_CANCEL:
            		//关闭询问窗口
            		MMIPUB_CloseQuerytWin(PNULL);
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

#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
/*****************************************************************************/
//     Description : Handle WidgetPb make call Win Msg
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbMakeCallWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
	MMI_RESULT_E  	result = MMI_RESULT_TRUE;
	uint32        		item_num = 0;  
	uint32        		i = 0; 
	MMI_STRING_T  	item_text = {0};
	GUIMENU_DYNA_ITEM_T node_item = {0};
	MMI_CTRL_ID_T 	ctrl_id = MMIWIDGETPB_MAKECALL_OPT_CTRL_ID;
	uint16        		curr_select_index =0;
	MMIWIDGET_INTERFACE_TYPE_E  	call_interface_type = MMIWIDGET_INTERFACE_TYPE_MAKECALL_AUDIO;//拨号方式
	
	switch(msg_id)
	    {
		    case MSG_OPEN_WINDOW:

			item_num = sizeof(s_widgetpb_dail_menu_sub_item)/sizeof(*s_widgetpb_dail_menu_sub_item);			
			for(i = 0; i < item_num; i++)
			{
				MMI_GetLabelTextByLang(s_widgetpb_dail_menu_sub_item[i][MMIWIDGETPB_CALL_MENU_TEXT_ID], &item_text);
				node_item.item_text_ptr = &item_text;
				GUIMENU_InsertNode(i, s_widgetpb_dail_menu_sub_item[i][MMIWIDGETPB_CALL_MENU_NODE_ID], PNULL, &node_item, ctrl_id);				
			}			
			MMK_SetAtvCtrl(win_id, ctrl_id);
			        break;

		    case MSG_APP_OK:
 		    case MSG_CTL_OK:
    		    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    		    case MSG_CTL_PENOK:   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		    case MSG_APP_WEB:
				curr_select_index = GUIMENU_GetCurNodeId(ctrl_id);
				
				switch(curr_select_index)
				{
					case WIDGETPB_OPTMENU_NODE_DIAL_AUDIO:						
					#ifdef VT_SUPPORT
					case WIDGETPB_OPTMENU_NODE_DIAL_VIDEO:
					#endif					
					case WIDGETPB_OPTMENU_NODE_IPDIAL:
							if(WIDGETPB_OPTMENU_NODE_DIAL_AUDIO == curr_select_index)
					            	{
					                call_interface_type = MMIWIDGET_INTERFACE_TYPE_MAKECALL_AUDIO;
					            	}

						#ifdef VT_SUPPORT
					       	else if(WIDGETPB_OPTMENU_NODE_DIAL_VIDEO == curr_select_index)
					            	{
					                call_interface_type = MMIWIDGET_INTERFACE_TYPE_MAKECALL_VIDEO;
					            	}
						#endif
#ifdef MMI_IP_CALL_SUPPORT
						  	else if(WIDGETPB_OPTMENU_NODE_IPDIAL == curr_select_index)
					            	{
					                call_interface_type = MMIWIDGET_INTERFACE_TYPE_MAKEIPCALL;
					            	}
#endif
							//make call 
							MMIWIDGETPB_MakeCall(
											s_widgetpb_entry_t.internal_entry_t[0].entry_id,
											s_widgetpb_entry_t.internal_entry_t[0].storage_id_1,
											s_widgetpb_entry_t.internal_entry_t[0].number_index,
											call_interface_type
											);

						break;
					default:
						break;				
				}//end of curr_select_index
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
//     Description : Handle WidgetPb send message Win Msg
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbWriteMessageWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
	MMI_RESULT_E  	result = MMI_RESULT_TRUE;
	uint32        		item_num = 0;  
	uint32        		i = 0; 
	MMI_STRING_T  	item_text = {0};
	GUIMENU_DYNA_ITEM_T node_item = {0};
	MMI_CTRL_ID_T 	ctrl_id = MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID;
	uint16        		curr_select_index =0;
	WIDGETPB_MESSAGE_INTERFACE_TYPE_E message_interface_type = WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS;

		
	switch(msg_id)
	    {
		    case MSG_OPEN_WINDOW:
			item_num = sizeof(s_widgetpb_sendmsg_menu_sub_item)/sizeof(*s_widgetpb_sendmsg_menu_sub_item);			
			for(i = 0; i < item_num; i++)
			{
				MMI_GetLabelTextByLang(s_widgetpb_sendmsg_menu_sub_item[i][MMIWIDGETPB_CALL_MENU_TEXT_ID], &item_text);
				node_item.item_text_ptr = &item_text;
				GUIMENU_InsertNode(i, s_widgetpb_sendmsg_menu_sub_item[i][MMIWIDGETPB_CALL_MENU_NODE_ID], PNULL, &node_item, ctrl_id);				
			}			
			MMK_SetAtvCtrl(win_id, ctrl_id);
			        break;


		    case MSG_APP_OK:
 		    case MSG_CTL_OK:
    		    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    		    case MSG_CTL_PENOK:   						
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
				curr_select_index = GUIMENU_GetCurNodeId(ctrl_id);
				switch(curr_select_index)
				{
				case WIDGETPB_OPTMENU_NODE_SEND_SMS:
				case WIDGETPB_OPTMENU_NODE_SEND_MMS:
			                 
					 if(curr_select_index == WIDGETPB_OPTMENU_NODE_SEND_SMS)
					 {
					 	message_interface_type = WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS;
					 }
					 else if(curr_select_index == WIDGETPB_OPTMENU_NODE_SEND_MMS)
					 {
					 	message_interface_type =  WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS;	
					 }

					 MMIWIDGETPB_SendMessage( 
					 				s_widgetpb_entry_t.internal_entry_t[0].entry_id,
					 				s_widgetpb_entry_t.internal_entry_t[0].storage_id_1,
					 				s_widgetpb_entry_t.internal_entry_t[0].number_index,
					 				message_interface_type
					 				);


					break;

				default:
				       break;		
							
				}//end of curr_select_index
				break;


		    case MSG_APP_WEB:
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
//     Description : Handle WidgetPb Favorite List Win Msg
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbFavoriteListWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
	    MMI_RESULT_E    	result = MMI_RESULT_TRUE;
	    MMI_CTRL_ID_T   	ctrl_id = MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID;
	    uint16          		list_num = 0;
	    uint16          		cur_index = 0;
	    static  uint16		copy_entry_id =0;
	    static    uint16		copy_storage_id =1;
	    static    uint16		copy_index_id =0;	
 	    MMIPB_PHONEBOOK_ENTRY_T  copy_entry_t={0};
	    MMIPB_PHONEBOOK_ENTRY_T*  select_entry_ptr = PNULL;
	    static BOOLEAN		is_add_from_pb_sim = FALSE;


	    switch(msg_id)
	    {
		    case MSG_OPEN_WINDOW:

                
                list_num = MMIWIDGET_PB_WIDGET_NUM;
                MMK_SetAtvCtrl(win_id, ctrl_id);
                GUILIST_SetMaxItem(ctrl_id, list_num, FALSE);

                UpdateFavoriteListBoxItems(FALSE);
                GUILIST_SetCurItemIndex(MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID, s_favorite_curr_index);
			        break;
            case MSG_GET_FOCUS:
                UpdateFavoriteListBoxItems(TRUE);
                break;
                
		    case MSG_APP_OK:
 		    case MSG_CTL_OK:		
            	cur_index = GUILIST_GetCurItemIndex(MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID);

				//cr 193926
				if(s_widgetpb_entry_t.internal_entry_t[cur_index].entry_id==0)
				{
					s_item_is_grayed = TRUE;
				}
				else
				{
					s_item_is_grayed = FALSE;
				}
				MMK_CreateWin((uint32*)MMIWIDGETPB_FAVORITE_LIST_MENU_WIN_TAB,  PNULL);
				break;
		    case MSG_APP_WEB:
		    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    		    case MSG_CTL_PENOK:   			
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

				select_entry_ptr = (MMIPB_PHONEBOOK_ENTRY_T*)MMK_GetWinAddDataPtr(win_id);
				
				cur_index = GUILIST_GetCurItemIndex(MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID);
				if(select_entry_ptr->entry_id == 0)
					{
						/*MMIAPIPB_OpenNumberList(
					                MMIPB_LIST_FOR_SMS, //IN: 列表是为哪个模块显示
					                MMIPB_LIST_RADIO,   //IN: 号码列表的类型
					                0,       //IN: 如果是多选时，此项限制了多选的最大个数
					                PNULL      //IN: 打开列表时的位置情况，本模块调用时有用
					                );*/
                        MMIAPIPB_OpenMainWin(
                                MMIPB_ALL_CONTACT,//
                                MMIPB_LIST_RADIO, //IN: 号码列表的类型
                                MMIPB_LIST_FIELD_WHOLE,
                                0,//如果是多选时，此项限制了多选的最大个数
                                PNULL,
                                FavoriteAddContactCallback
                                );					                
					}
				else
					{
						if(MMIPB_IsSIMContact(select_entry_ptr->storage_id))
						{
							is_add_from_pb_sim = TRUE;
							copy_entry_id = select_entry_ptr->entry_id;
							copy_storage_id = select_entry_ptr->storage_id_1;
							copy_index_id = cur_index;
							MMIPUB_OpenQueryWinByTextId(TXT_WIDGET_PB_ADDTO_FAVORITE_LIST_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
						}
						else
						{
							//从pb中添加，不知道该记录有几个号码和那个是有效号码
							MMIWIDGETPB_AppendItemToFavoriteListBox(select_entry_ptr,MMIPB_MAX_NV_PHONE_NUM,cur_index);
							MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
                       				MMK_CloseWin(win_id);

						}
						
					}
				
				break;
              case MMI_SELECT_CONTACT_CNF:
                    cur_index = GUILIST_GetCurItemIndex(MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID);
                    // get the number from the PB
                    if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param) > 0)
                    {
                        uint16                   		number_index = 0;
                    	MMIPB_CONTACT_T *contact = PNULL;  
                        
                    	contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
                        if (MMIAPIPB_GetSelectContact((MMIPB_HANDLE_T)param,  &contact,0, &number_index ))
                        {
                            if(MMIPB_IsSIMContact(contact.storage_id))
                            {
                                
                                copy_entry_id = contact->contact_id;
                                copy_storage_id = contact->storage_id;
                                copy_index_id = cur_index;
                                MMIPUB_OpenQueryWinByTextId(TXT_WIDGET_PB_ADDTO_FAVORITE_LIST_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                            }
                            else
                            {
                                MMIWIDGETPB_AppendItemToFavoriteListBox(&contact,number_index,cur_index);
                                
                            }	
                    	    SCI_FREE(contact);							
                        }			
                    }
				break;
		    case MSG_PROMPTWIN_OK:

				if(copy_entry_id==0)
				{
					return MMI_RESULT_FALSE;
				}
				else
                {
                    if(MMIAPIPB_CopyOneContact(
                        copy_entry_id, 
                        0,
                        copy_storage_id,
                        PB_GROUP_UNCLASS,
						&copy_entry_t.entry_id))
                    {
                        copy_entry_t.storage_id = 0;
                        if(MMIAPIPB_ReadPhonebookEntey(&copy_entry_t))
                        {
                            MMIWIDGETPB_AppendItemToFavoriteListBox(&copy_entry_t,0,copy_index_id);//sim 卡只有一条记录	
                            is_add_from_pb_sim = TRUE;
                        }
                    }
                    else
                    {
                        is_add_from_pb_sim = FALSE;
                    }
                    copy_entry_id = 0;
                    copy_storage_id = 1;
                    copy_index_id =0;
                    
                }
				MMIPUB_CloseQuerytWin(PNULL);
				if(is_add_from_pb_sim)
                {
                    is_add_from_pb_sim = FALSE;
                    MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin(TXT_NO_SPACE);
                }
				//MMK_CloseWin(win_id);

				break;
		    case MSG_PROMPTWIN_CANCEL:		
				MMIPUB_CloseQuerytWin(PNULL);
				break;
		    case MSG_APP_CANCEL:
		    case MSG_CTL_CANCEL:
				MMK_CloseWin(win_id);
				break;
			
            case MSG_CLOSE_WINDOW:
                select_entry_ptr = (MMIPB_PHONEBOOK_ENTRY_T*)MMK_GetWinAddDataPtr(win_id);
                if (PNULL != select_entry_ptr)
                {
                    SCI_FREE(select_entry_ptr);
                    select_entry_ptr = PNULL;
                }
                break;

		    default:
				 result = MMI_RESULT_FALSE;
				 break;		
		
	   }

	    return result;

}

LOCAL int  GetDispNamefromPbName(MMI_STRING_T *dist_ptr, const MMIPB_NAME_T  *name_ptr)
{
    if(PNULL == dist_ptr || (PNULL == dist_ptr->wstr_ptr) || (PNULL == name_ptr))
        return (-1);
    
    if (name_ptr->wstr_len > 0)
    {
        MMIPB_ConvertNameToString(dist_ptr, name_ptr);
    }
    else
    {
        MMI_STRING_T    noname_str = {0};
        
        // 姓名为空
        MMI_GetLabelTextByLang(TXT_NO_NAME,  &noname_str);
        dist_ptr->wstr_len = noname_str.wstr_len;
        
        MMIAPICOM_Wstrncpy(dist_ptr->wstr_ptr, noname_str.wstr_ptr, dist_ptr->wstr_len);
    }
    
    return 0;
}

LOCAL int GetDispTelNumberAndLocation(MMI_STRING_T *tel_ptr, MMI_STRING_T *location_ptr, MMIPB_PHONEBOOK_ENTRY_T* entry_ptr, int index)
{
    uint8   tele_prompt_len = 0;
    uint8   temp_prompt_str[MMIWIDGETPB_NUMBER_MAX_LEN + 1]= {0};
    uint8   num_index = s_widgetpb_entry_t.internal_entry_t[index].number_index;
    
    uint8   *origin_number_ptr = PNULL;
    int16   origin_num_len  = 0;
    uint8   ip_number[MMISET_IPNUMBER_MAX_LEN+1] = {0};
    uint8   ip_number_len = 0;   
    uint16  str_length = 0; 
    MMI_STRING_T default_str = {0};
    
    if (PNULL == entry_ptr
        || PNULL == tel_ptr || PNULL == tel_ptr->wstr_ptr
        || PNULL == location_ptr || PNULL == location_ptr->wstr_ptr)
    {
        return (-1);
    } 
    
    // tel
    tele_prompt_len = MMIAPICOM_GenDispNumber( 
        MMIPB_GetNumberTypeFromUint8(entry_ptr->number_t[num_index].npi_ton),
        entry_ptr->number_t[num_index].number_len,
        entry_ptr->number_t[num_index].number,
        temp_prompt_str,
        MMIPB_MAX_STRING_LEN
        );
    
    MMI_STRNTOWSTR(tel_ptr->wstr_ptr, MMIWIDGETPB_NUMBER_MAX_LEN+1,(char *)temp_prompt_str, MMIWIDGETPB_NUMBER_MAX_LEN+1,MMIWIDGETPB_NUMBER_MAX_LEN+1);/*lint !e64*/
    tel_ptr->wstr_len = tele_prompt_len;
    
    // location
    MMIAPISET_GetIPNumber(MN_DUAL_SYS_1, temp_prompt_str, ip_number, &ip_number_len);/*lint !e605*/
    if(ip_number_len > 0 &&(ip_number_len < tele_prompt_len) && (!strncmp((char *)temp_prompt_str, (char *)ip_number, ip_number_len)))
    {
        //exist ip number, without ip number
        origin_number_ptr = &temp_prompt_str[ip_number_len]; /*lint !e605*/
        origin_num_len = tele_prompt_len - ip_number_len;
    }
    else
    {
        origin_number_ptr = temp_prompt_str;    /*lint !e605*/
        origin_num_len = tele_prompt_len;
    }
    
#ifdef HOME_LOCATION_INQUIRE_SUPPORT    
    str_length = MMIAPIPHONESEARCH_FindLocation(origin_number_ptr, origin_num_len, location_ptr->wstr_ptr);
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
        MMIAPICOM_Wstrncpy(location_ptr->wstr_ptr, default_str.wstr_ptr, default_str.wstr_len );
    }
    
    location_ptr->wstr_len = MMIAPICOM_Wstrlen(location_ptr->wstr_ptr);
#endif
    
    return 0;
}


/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void  AppendItemToFavoriteListBox(
                                        MMI_CTRL_ID_T ctrl_id,
                                        uint16		  index,
                                        MMIPB_PHONEBOOK_ENTRY_T* entry_ptr,
                                        MMI_TEXT_ID_T left_softkey_id,
                                        MMI_TEXT_ID_T middle_softkey_id,
                                        MMI_TEXT_ID_T right_softkey_id,
                                        BOOLEAN       is_update
                                        )
{
    MMI_STRING_T    name_str = {0};
    MMI_STRING_T    tel_str = {0};
    MMI_STRING_T    location_str = {0};
    wchar           temp_name[MMIPB_MAX_STRING_LEN + 2] = {0};
    wchar           temp_tel[MMIPB_MAX_STRING_LEN + 2] = {0};
    wchar           temp_location[MMIPB_MAX_STRING_LEN + 2] = {0};
    MMIPB_NAME_T    entry_name = {0};
    
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = item_icon_id[index];
    
    // name
    if (0 == entry_ptr)
    {
        MMI_STRING_T		tmp_prompt = {0};
        MMI_GetLabelTextByLang(TXT_BLANK, &tmp_prompt);
        
        name_str.wstr_len = MMIAPICOM_Wstrlen(tmp_prompt.wstr_ptr);
        MMIAPICOM_Wstrncpy(temp_name, tmp_prompt.wstr_ptr, name_str.wstr_len);
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    else if(MMIPB_IsMostUsedContact(entry_ptr->storage_id))
    {     
        name_str.wstr_ptr = entry_name.wstr;
        if(MMIAPIPB_GetNameByNumber(&(entry_ptr->number_t[0]), &name_str, MMIPB_NAME_MAX_LEN, TRUE))
        {
            entry_name.wstr_len = name_str.wstr_len;
        }
        else
        {
            SCI_MEMSET(&entry_name, 0x00, sizeof(entry_name));
        }
    }
#endif
    else
    {
        entry_name = entry_ptr->union_t.entry_name;
    }
    
    name_str.wstr_ptr = temp_name;
    if (0!= entry_ptr)
    {
        name_str.wstr_len = 0;
        GetDispNamefromPbName(&name_str, &entry_name);
    }
    
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = name_str.wstr_len;   
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
    
    
    // number
    tel_str.wstr_ptr = temp_tel;
    location_str.wstr_ptr = temp_location;
    
    if (0 != entry_ptr)
    {
        GetDispTelNumberAndLocation(&tel_str, &location_str, entry_ptr, index);
    }
    
    item_data.item_content[2].item_data_type    =GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_len = tel_str.wstr_len;
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = tel_str.wstr_ptr;
    
    // location
    item_data.item_content[3].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer.wstr_len = location_str.wstr_len;
    item_data.item_content[3].item_data.text_buffer.wstr_ptr = location_str.wstr_ptr;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, index );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
    
}

/*****************************************************************************/
//	Description : get favorite entry name by index.
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void GetFavoriteEntryName(uint32 index, MMI_STRING_T *item_text)
{
    wchar              *entry_name_ptr = PNULL;
    uint16              entry_len = 0;        
    MMIPB_PHONEBOOK_ENTRY_T  temp_entry_t={0};
    uint8				tele_prompt_len = 0;
    uint8				temp_prompt_str[MMIWIDGETPB_NUMBER_MAX_LEN + 1]= {0};
	MMI_STRING_T        temp_name_str = {0};
    
    entry_name_ptr = item_text->wstr_ptr;
    		
    //get name by number from pb
    temp_entry_t.entry_id  = s_widgetpb_entry_t.internal_entry_t[index].entry_id;
    temp_entry_t.storage_id = s_widgetpb_entry_t.internal_entry_t[index].storage_id_1;
    index =	s_widgetpb_entry_t.internal_entry_t[index].number_index;
           
    if(MMIAPIPB_ReadPhonebookEntey(&temp_entry_t))
    {
        if(0 == temp_entry_t.union_t.entry_name.wstr_len)
        {
            item_text->wstr_len = 0;
        }
		else
        if(PB_GROUP_MOSTUSED == temp_entry_t.storage_id)
 	    {
    	    temp_name_str.wstr_ptr = (wchar*)SCI_ALLOCA(CC_MAX_PBNAME_LEN * sizeof(wchar));

			if (PNULL == temp_name_str.wstr_ptr)
			{
			    //SCI_TRACE_LOW:"failed to alloc name memory, display empty for exception."
			    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_PB_1828_112_2_18_3_8_27_226,(uint8*)"");
			    return;
			}
				
            if (MMIAPIPB_GetNameByNumber(&temp_entry_t.number_t[index], &temp_name_str, CC_MAX_PBNAME_LEN, TRUE))
			{
                entry_len  = MIN((uint16)temp_name_str.wstr_len, 3);
                MMIAPICOM_Wstrncpy(entry_name_ptr, temp_name_str.wstr_ptr, entry_len);				
                MMIAPICOM_Wstrncpy(&entry_name_ptr[2], L".", 1);    
                item_text->wstr_len = 3;			
			}
			else
			{
                item_text->wstr_len = 0;
			}
			SCI_FREE(temp_name_str.wstr_ptr);
 		}		
        else
        {
            entry_len  = MIN((uint16)temp_entry_t.union_t.entry_name.wstr_len, 3);
            MMIAPICOM_Wstrncpy(entry_name_ptr, temp_entry_t.union_t.entry_name.wstr, entry_len);
            MMIAPICOM_Wstrncpy(&entry_name_ptr[2], L".", 1);    
            item_text->wstr_len = 3;
        }
		
        //name is NULL ,show number
        if(item_text->wstr_len == 0)
        {
            if(MMIAPIPB_IsValidPhoneNumber(temp_entry_t.number_t[index].npi_ton,/*lint !e64*/
                temp_entry_t.number_t[index].number_len))/*lint !e64*/
            {						
                
                tele_prompt_len = MMIAPICOM_GenDispNumber( 
                    MMIPB_GetNumberTypeFromUint8(temp_entry_t.number_t[index].npi_ton),
                    temp_entry_t.number_t[index].number_len,
                    temp_entry_t.number_t[index].number,
                    temp_prompt_str,
                    MMIPB_MAX_STRING_LEN
                    );
                MMI_STRNTOWSTR(entry_name_ptr,2,(char *)temp_prompt_str,2,2);/*lint !e64*/
                MMIAPICOM_Wstrncpy(&entry_name_ptr[2], L".", 1);  
                item_text->wstr_len = 3;                
            }
        }										
    }
}

/*****************************************************************************/
// Description : update favoritelist items
// Global resource dependence :
// Author:cheng.luo
// Note: 
/*****************************************************************************/
LOCAL void UpdateFavoriteListBoxItems(BOOLEAN is_update)
{
    uint32          		i= 0;
    MMIPB_PHONEBOOK_ENTRY_T  temp_entry_t={0};
    
    for( i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++ )
    {			
        //get name by number from pb
        temp_entry_t.entry_id  = s_widgetpb_entry_t.internal_entry_t[i].entry_id;
        temp_entry_t.storage_id = s_widgetpb_entry_t.internal_entry_t[i].storage_id_1;
        
        if(MMIAPIPB_ReadPhonebookEntey(&temp_entry_t))
        {
            AppendItemToFavoriteListBox(
                MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID, 
                i,
                &temp_entry_t,
                STXT_OPTION,
                STXT_MAIN_SETTING,
                STXT_RETURN,
                is_update);
        }
        else
        {
            AppendItemToFavoriteListBox(
                MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID, 
                i,
                NULL,
                STXT_OPTION,
                STXT_MAIN_SETTING,
                STXT_RETURN,
                is_update);
        }
    }
}

/*****************************************************************************/
//	Description : to add the string to listbox
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void  MMIWIDGETPB_AppendItemToFavoriteListBox(
                                                    MMIPB_PHONEBOOK_ENTRY_T  *load_entry_ptr, 
                                                    uint8	number_index,
                                                    uint16  cur_index )
{
    uint8				record_number_index = 0;
    uint32          	i= 0;
    
    for (i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++)
    {
        if((s_widgetpb_entry_t.internal_entry_t[i].entry_id== load_entry_ptr->entry_id)	
            &&(s_widgetpb_entry_t.internal_entry_t[i].storage_id_1== load_entry_ptr->storage_id))
        {		              
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_WIDGET_PB_QUICK_CONTACT_HAS_EXIST, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, NULL);
            return;
        }
    }
    
    if(number_index == MMIPB_MAX_NV_PHONE_NUM)
    {
        record_number_index = MMIAPIPB_GetFirstValidNumberFromNumArray(load_entry_ptr->number_t,MMIPB_MAX_NV_PHONE_NUM);	
        //无有效号码
        if(record_number_index > MMIPB_MAX_NV_PHONE_NUM)
            record_number_index = 0;
    }
    else
    {
        record_number_index = number_index;
    }
    
    s_widgetpb_entry_t.internal_entry_t[cur_index].entry_id	=	load_entry_ptr->entry_id ;
    s_widgetpb_entry_t.internal_entry_t[cur_index].storage_id_1 	= 	load_entry_ptr->storage_id;
    s_widgetpb_entry_t.internal_entry_t[cur_index].number_index =  record_number_index;
    
    AppendItemToFavoriteListBox(
        MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID, 
        cur_index,
        load_entry_ptr,
        STXT_OPTION,
        STXT_MAIN_SETTING,
        STXT_RETURN,
        TRUE);
    
    SetAnimData(WIDGET_PB_WIN_ID,cur_index);     
				
    //change pb list icon chengyong_mocor_cr192457
    //MMIPB_UpdateCurrentList();
    //	if(MMK_IsOpenWin(MMIPB_ENTRY_LIST_TAB_WIN_ID))								
    //	{
    //		MMK_SendMsg(MMIPB_ENTRY_LIST_TAB_WIN_ID, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
    // 	}
    
}
/*****************************************************************************/
//     Description : Handle WidgetPb Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetPbWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E 		recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
	    case MSG_OPEN_WINDOW:		
            s_is_first_start = TRUE;
			MMIWIDGETPB_InitFavoriteList();
            CreateWidgetPbAnim(win_id);
	        break;

	    case MSG_WIDGET_TP_PRESS_DOWN:
			recode = HandleTpPressDown(win_id,param);	
			break;
        
	    case MSG_WIDGET_TP_PRESS_UP:
		 	HandleTpPressUp(win_id,param);	 
	        break;
        
        case MSG_GET_FOCUS:
            UpdateFavoriteListCustomPhoto();
            break;

        case MSG_FULL_PAINT:	 	    
            LoadWidgetPbAnimData(win_id);
            DrawPbWidget(win_id);
            break;

        case MSG_NOTIFY_ANIM_DISPLAY_IND:
            if (0 != s_pb_anim_timer_id)
            {
                break;
            }

            CreateAnimDrawTimer(win_id);
            break;

        case MSG_TIMER:
            if (s_pb_anim_timer_id == *( GUI_TIMER_ID_T*)param)
            {
                s_pb_anim_timer_id = 0;
                
#if (defined MMI_GRID_IDLE_SUPPORT)
                if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
                {
                    MMIGRID_UpdateAllScreen();
                }
#endif
#if defined MMIWIDGET_SUPPORT
                if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()) 
                {
                   MMIWIDGET_UpdateAllScreen();
                }
#endif 
				
            }
            break;
 
	    default:
		        recode = MMI_RESULT_FALSE;
	        break;
    }
    
    return recode;        
}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void DrawPbWidget(
                        MMI_WIN_ID_T  win_id
                        )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T img_rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    MMITHEME_WIDGET_PB_T pb_widget_theme = {0};
    GUI_RECT_T 		button_call_rect = MMIWIDGET_PB_WIDGET_CALL_RECT; 
    GUI_RECT_T 		button_sms_rect = MMIWIDGET_PB_WIDGET_SMS_RECT;

        
    MMITHEME_GetPBWidgetTheme(&pb_widget_theme);
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        //背景图
        GUIRES_GetImgWidthHeight(&bg_width, &bg_height, pb_widget_theme.bg_img, win_id);
        img_rect.right = (bg_width - 1);
        img_rect.bottom = (bg_height - 1);
        GUIRES_DisplayImg(
            PNULL,
            &win_rect,
            &img_rect,
            win_id,
            pb_widget_theme.bg_img,
            &lcd_dev_info
            );

       
	   
	switch (s_widget_pb_hit_type)
        {
        case MMIWIDGET_PB_HIT_CALL:
            //CALL按钮
            button_call_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_call_rect);
            GUIRES_DisplayImg(
                PNULL,
                &button_call_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_PB_DIAL_ON,
                &lcd_dev_info
                );
		//SMS按钮
            button_sms_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, button_sms_rect);
            GUIRES_DisplayImg(
                PNULL,
                &button_sms_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_PB_SENDSMS_OFF,
                &lcd_dev_info
                );
            break;
        case MMIWIDGET_PB_HIT_SMS:
            //SMS按钮
            button_sms_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, button_sms_rect);
            GUIRES_DisplayImg(
                PNULL,
                &button_sms_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_PB_SENDSMS_ON,
                &lcd_dev_info
                );
		 //CALL按钮
            button_call_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_call_rect);
            GUIRES_DisplayImg(
                PNULL,
                &button_call_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_PB_DIAL_OFF,
                &lcd_dev_info
                );
            break;
        default:
         //SMS按钮
            button_sms_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, button_sms_rect);
            GUIRES_DisplayImg(
                PNULL,
                &button_sms_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_PB_SENDSMS_OFF,
                &lcd_dev_info
                );
		 //CALL按钮
            button_call_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_call_rect);
            GUIRES_DisplayImg(
                PNULL,
                &button_call_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_PB_DIAL_OFF,
                &lcd_dev_info
                );            break;
        }	
    }
}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void CreateWidgetPbAnim(
                               MMI_WIN_ID_T  win_id
                               )
{
    uint32                  i = 0;

    for (i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++)
    {
        AddAnimData(win_id, i);
    }
}

/*****************************************************************************/
// Description : load widget pb animation data.
// Global resource dependence :
// Author: 
// Note: 
/*****************************************************************************/
LOCAL void LoadWidgetPbAnimData(MMI_WIN_ID_T  win_id)
{
    if (s_is_first_start)
    {
        int index = 0;
        
        for (index = 0; index < MMIWIDGET_PB_WIDGET_NUM; index++)
        {
            SetAnimData(win_id, index);
        }
        s_is_first_start = FALSE;
    }
}

/*****************************************************************************/
// Description : get pb widget content by index
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void GetPbWidgetContentByIndex(
                                     MMI_WIN_ID_T  win_id,
                                     MMIPB_CUSTOM_PHOTO_T *custom_photo_ptr,    //out
                                     GUI_RECT_T *disp_rect_ptr,
                                     uint16 index
                                     )
{
    MMIPB_PHONEBOOK_ENTRY_T *entry_ptr = PNULL;
    MMIPB_PHONEBOOK_ENTRY_T temp_entry = {0};
    if (index >= MMIWIDGET_PB_WIDGET_NUM)
    {
        return;
    }

	
    entry_ptr = &temp_entry;
    entry_ptr->storage_id = s_widgetpb_entry_t.internal_entry_t[index].storage_id_1;    //1为PB_GROUP_UNCLASS，未分类
    entry_ptr->entry_id  = s_widgetpb_entry_t.internal_entry_t[index].entry_id;    //entry_id=0好像无法取到
    if (PNULL != custom_photo_ptr)
    {
        MMIAPIPB_ReadPhotoFileName(entry_ptr->entry_id, entry_ptr->storage_id, custom_photo_ptr);    
    }
    if (PNULL != disp_rect_ptr)
    {
        *disp_rect_ptr = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, s_pb_widget_arr[index]);
    }
}

/*****************************************************************************/
// Description : update pb widget custom photo
// Global resource dependence :
// Author: Cheng.Luo
// Note: 
/*****************************************************************************/
LOCAL void UpdateFavoriteListCustomPhoto()
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_FILE_INFO_T file_info  = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    MMITHEME_WIDGET_PB_T    pb_widget_theme = {0};
    GUIANIM_DATA_INFO_T     data_info        = {0};
    MMIPB_CUSTOM_PHOTO_T    custom_photo = {0};
    
    uint16	entry_id = 0; 
    uint8	storage_id = 0;
    uint32	i = 0;
    
    MMITHEME_GetPBWidgetTheme(&pb_widget_theme);
 
    ctrl_info.is_wallpaper_ctrl = FALSE;
    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_ptr = PNULL;
    
    display_info.is_update = FALSE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_disp_one_frame = TRUE;
    
    for( i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++ )
    {
        ctrl_info.ctrl_id = (MMIWIDGET_PB_ANIM_CTRL_ID + i);
        
        entry_id = s_widgetpb_entry_t.internal_entry_t[i].entry_id;
        storage_id = s_widgetpb_entry_t.internal_entry_t[i].storage_id_1;

        GUIANIM_SetOwnerDraw(ctrl_info.ctrl_id, DrawEntryName);
        
        if (MMIAPIPB_ReadPhotoFileName(entry_id, storage_id, &custom_photo)
            && custom_photo.is_valid
            && MMIAPIFMM_IsFileExist(custom_photo.file_name,custom_photo.file_name_len))
        {
            
            file_info.full_path_wstr_ptr = custom_photo.file_name;
            file_info.full_path_wstr_len = custom_photo.file_name_len;  
            
            display_info.bg.bg_type = GUI_BG_COLOR;
            display_info.bg.color = MMI_BACKGROUND_COLOR;
            display_info.bg.shape = GUI_SHAPE_ROUNDED_RECT;
            
            GUIANIM_SetParam(&ctrl_info,
                PNULL,
                &file_info,
                &display_info);
            
        }
        else
        {
            if(0 == entry_id)
            {  
                if( 0 == i)
                {
                    //set default amin icon
                    data_info.img_id = IMAGE_WIDGET_PB_ANIM_EMPTY_0;
                }
                else
                {
                    data_info.img_id = pb_widget_theme.error_img;
                }
                
            }
            else
            {
                if( 0 == i)
                {
                    //set default amin icon
                    data_info.img_id = IMAGE_WIDGET_PB_ANIM_DEFAULT_0;
                }
                else
                {
                    data_info.img_id = pb_widget_theme.default_img;
                }
            }
            
            
            SCI_MEMSET((uint16*)&display_info.bg, 0, sizeof(display_info.bg));

            GUIANIM_SetParam(&ctrl_info,
                &data_info,
                PNULL,
                &display_info);
        }
    }
}

/*****************************************************************************/
// Description : animation ctrl call back function to display entry name.
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void DrawEntryName(GUIANIM_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32            i            = 0;
    GUI_RECT_T        disp_rect    = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    MMI_STRING_T      entry_name   = {0};
    GUISTR_STATE_T    text_state   = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    GUISTR_STYLE_T    str_style    = {0};
    
    str_style.effect     = FONT_EFFECT_REVERSE_EDGE; ; 
    str_style.angle      = ANGLE_0;
    str_style.align      = ALIGN_LEFT;
    str_style.font       = SONG_FONT_18;
    str_style.font_color = MMITHEME_GetDefaultWidgetThemeColor();

    MMK_GetWinLcdDevInfo(owner_draw_ptr->win_handle, &lcd_dev_info);

    entry_name.wstr_ptr = (wchar*)SCI_ALLOCA(3*sizeof(wchar));

    if (PNULL == entry_name.wstr_ptr)
    {
        return;
    }
    
    for( i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++ )
    {
        SCI_MEMSET(entry_name.wstr_ptr, 0x00, ((3)*sizeof(wchar)));
        GetFavoriteEntryName(i, &entry_name); 
        entry_name.wstr_len = 3;

        GetPbWidgetContentByIndex(owner_draw_ptr->win_handle, PNULL, &disp_rect, i); 
        disp_rect.bottom = disp_rect.top + 20;

        GUISTR_DrawTextToLCDInRect( 
    		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
    		&disp_rect,
    		&disp_rect,
            (const MMI_STRING_T     *)&entry_name,
            &str_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO);          
    }

    if (PNULL != entry_name.wstr_ptr)
    {
        SCI_FREE(entry_name.wstr_ptr);
        entry_name.wstr_ptr = PNULL;
    }     
}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void SetAnimData(
                       MMI_WIN_ID_T  win_id,
                       uint16 index                       
                       )
{
 
    	GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    	GUIANIM_FILE_INFO_T file_info  = {0};
    	GUIANIM_RESULT_E retcode = 0;
    	MMI_CTRL_ID_T ctrl_id = 0;
    	GUIANIM_DISPLAY_INFO_T display_info = {0};
    	MMIPB_CUSTOM_PHOTO_T        custom_photo = {0};
    	GUI_RECT_T disp_rect = {0};
    	MMITHEME_WIDGET_PB_T pb_widget_theme = {0};
	GUIANIM_DATA_INFO_T             data_info           = {0};


    	MMITHEME_GetPBWidgetTheme(&pb_widget_theme);
    	if (index >= MMIWIDGET_PB_WIDGET_NUM)
    	{
        	return;
    	}
    	ctrl_id = (MMIWIDGET_PB_ANIM_CTRL_ID+index);

	//get image path	
    	GetPbWidgetContentByIndex(win_id, &custom_photo, &disp_rect, index);



	ctrl_info.is_wallpaper_ctrl = FALSE;
    	ctrl_info.is_ctrl_id = TRUE;
    	ctrl_info.ctrl_id = ctrl_id;
    	ctrl_info.ctrl_ptr = PNULL;

    	//set file path
    	file_info.full_path_wstr_ptr = custom_photo.file_name;
    	file_info.full_path_wstr_len = custom_photo.file_name_len;
    
    	display_info.is_update = FALSE;
    	display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
  	    display_info.is_disp_one_frame = TRUE;
        GUIANIM_SetOwnerDraw(ctrl_id, DrawEntryName);

	if(custom_photo.is_valid
        && MMIAPIFMM_IsFileExist(custom_photo.file_name,custom_photo.file_name_len)
        && !MMIAPIUDISK_UdiskIsRun())
    	{

    	display_info.bg.bg_type = GUI_BG_COLOR;
        display_info.bg.color = MMI_BACKGROUND_COLOR;
    	display_info.bg.shape = GUI_SHAPE_ROUNDED_RECT;

   		retcode = GUIANIM_SetParam(&ctrl_info,
                             PNULL,
                            &file_info,
                              &display_info);


    	}
	else
	{

	   	if(s_widgetpb_entry_t.internal_entry_t[index].entry_id== 0 )
	   	{

			if( index == 0)
	    			//set default amin icon
	    			data_info.img_id = IMAGE_WIDGET_PB_ANIM_EMPTY_0;
			else
	    			data_info.img_id = pb_widget_theme.error_img;
			
	   	}
	   	else
	   	{
			if( index == 0)
	    		//set default amin icon
	    			data_info.img_id = IMAGE_WIDGET_PB_ANIM_DEFAULT_0;
			else
	    			data_info.img_id = pb_widget_theme.default_img;
	   	}


	    SCI_MEMSET((uint16*)&display_info.bg, 0, sizeof(display_info.bg));

   		retcode = GUIANIM_SetParam(&ctrl_info,
                             &data_info,
                            PNULL,
                              &display_info);

	}    
                              
    if (GUIANIM_RESULT_WAIT == retcode)
    {
        retcode = retcode;
    }
    MMIWIDGETPB_WriteNvFavoriteList(&s_widgetpb_entry_t);

}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: bin.ji
// Note: 
/*****************************************************************************/
LOCAL void AddAnimData(
                       MMI_WIN_ID_T  win_id,
                       uint16 index
                       )
{
    //没有就创建一个
    GUIANIM_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T create   = {0};           
    // GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    // GUIANIM_FILE_INFO_T file_info  = {0};
    // GUIANIM_RESULT_E retcode = 0;
    MMI_CTRL_ID_T ctrl_id = 0;
    void    *anim_ptr = PNULL;
    // GUIANIM_DISPLAY_INFO_T display_info = {0};
    MMIPB_CUSTOM_PHOTO_T        custom_photo = {0};
    GUI_RECT_T disp_rect = {0};
    MMITHEME_WIDGET_PB_T pb_widget_theme = {0};
    // GUIANIM_DATA_INFO_T             data_info           = {0};

    MMITHEME_GetPBWidgetTheme(&pb_widget_theme);
    if (index >= MMIWIDGET_PB_WIDGET_NUM)
    {
        return;
    }
    ctrl_id = (MMIWIDGET_PB_ANIM_CTRL_ID+index);

    GetPbWidgetContentByIndex(win_id, &custom_photo, &disp_rect, index);

    init_data.both_rect.h_rect = init_data.both_rect.v_rect = disp_rect;//full_disp_rect;
    
    create.ctrl_id = ctrl_id;//0;
    create.guid = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = win_id;
    create.parent_ctrl_handle = 0;//ctrl_id;
    create.init_data_ptr = &init_data;
    
    anim_ptr = MMK_CreateControl(&create);

    //set anim ctrl rect
    GUIANIM_SetCtrlRectByPtr(anim_ptr,&disp_rect);

    //set image display rect
    GUIANIM_SetDisplayRectByPtr(anim_ptr,&disp_rect,FALSE);
}

/*****************************************************************************/
//  Description : tp down处理
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T 		button_call_rect = MMIWIDGET_PB_WIDGET_CALL_RECT; 
    GUI_RECT_T 		button_sms_rect = MMIWIDGET_PB_WIDGET_SMS_RECT;

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    if (GUI_PointIsInRect(point, button_call_rect) && (s_widgetpb_entry_t.internal_entry_t[0].entry_id!= 0))
    {
        s_widget_pb_hit_type = MMIWIDGET_PB_HIT_CALL;
    }
    else if (GUI_PointIsInRect(point, button_sms_rect) && (s_widgetpb_entry_t.internal_entry_t[0].entry_id!= 0))
    {
        s_widget_pb_hit_type = MMIWIDGET_PB_HIT_SMS;
    }
    else
    {
        s_widget_pb_hit_type = MMIWIDGET_PB_HIT_NONE;
        recode = MMI_RESULT_FALSE;
    }

    return recode;
}


/*****************************************************************************/
//  Description : tp up 处理
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleTpPressUp(
                       MMI_WIN_ID_T  win_id,   
                       DPARAM            param        //IN:
                       )
{
    GUI_POINT_T       	point 	={0};
    uint32                  	i 		= 0;
    GUI_RECT_T 		button_call_rect = MMIWIDGET_PB_WIDGET_CALL_RECT; 
    GUI_RECT_T 		button_sms_rect = MMIWIDGET_PB_WIDGET_SMS_RECT;
    MMIPB_PHONEBOOK_ENTRY_T  temp_entry_t ={0};
    uint8     number_index =0;
    
    s_widget_pb_hit_type = MMIWIDGET_PB_HIT_NONE;
    point.x =  MMK_GET_TP_X(param);
    point.y =  MMK_GET_TP_Y(param);
    point   = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    for( i=0;i < MMIWIDGET_PB_WIDGET_NUM;i++ )//cr192896
    {
        if(GUI_PointIsInRect(point, s_pb_widget_arr[i]))
        {
            temp_entry_t.entry_id = s_widgetpb_entry_t.internal_entry_t[i].entry_id;
            temp_entry_t.storage_id= s_widgetpb_entry_t.internal_entry_t[i].storage_id_1;
            
            if(temp_entry_t.entry_id == 0)
            {
                MMIAPIWIDGETPB_OpenFavoriteListWin(&temp_entry_t);
                s_favorite_curr_index = i;
            }
            else if(MMIAPIPB_ReadPhonebookEntey(&temp_entry_t))
            {
                
                if( i == 0)//替换是点击位置1不操作cr192896
                {
                    return;
                }
                temp_entry_t.entry_id = s_widgetpb_entry_t.internal_entry_t[0].entry_id;
                temp_entry_t.storage_id= s_widgetpb_entry_t.internal_entry_t[0].storage_id_1;
                number_index = s_widgetpb_entry_t.internal_entry_t[0].number_index;
                
                s_widgetpb_entry_t.internal_entry_t[0].entry_id = s_widgetpb_entry_t.internal_entry_t[i].entry_id; 
                s_widgetpb_entry_t.internal_entry_t[0].storage_id_1 = s_widgetpb_entry_t.internal_entry_t[i].storage_id_1;
                s_widgetpb_entry_t.internal_entry_t[0].number_index = s_widgetpb_entry_t.internal_entry_t[i].number_index;
                
                s_widgetpb_entry_t.internal_entry_t[i].entry_id = temp_entry_t.entry_id; 
                s_widgetpb_entry_t.internal_entry_t[i].storage_id_1 = temp_entry_t.storage_id;
                s_widgetpb_entry_t.internal_entry_t[i].number_index = number_index;
                
                SetAnimData(win_id,i);
                SetAnimData(win_id,0);	
            }
        }
    }
    if(GUI_PointIsInRect(point, button_call_rect))
    {
        
        if(s_widgetpb_entry_t.internal_entry_t[0].entry_id== 0)
        {
            return;
        }
        else 
        {
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
            MMIWIDGETPB_OpenMakeCallWin();
#else
            MMIWIDGETPB_MakeCall(
            				s_widgetpb_entry_t.internal_entry_t[0].entry_id,
            				s_widgetpb_entry_t.internal_entry_t[0].storage_id_1,
            				s_widgetpb_entry_t.internal_entry_t[0].number_index,
            				MMIWIDGET_INTERFACE_TYPE_MAKECALL_AUDIO
            				);
#endif
        }
    }
    else if(GUI_PointIsInRect(point, button_sms_rect))
    {
        
        if(s_widgetpb_entry_t.internal_entry_t[0].entry_id== 0)
        {
            return;
        }
        else
        {
            MMIWIDGETPB_OpenSendMessageWin();
        }
        
    }
}

/*****************************************************************************/
//  Description : create quick contact window.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_OpenQuickContactListWin()
{
    MMIPB_PHONEBOOK_ENTRY_T  *entry_ptr = PNULL;
    
    entry_ptr = (MMIPB_PHONEBOOK_ENTRY_T *)SCI_ALLOC_APP(sizeof(MMIPB_PHONEBOOK_ENTRY_T));

    entry_ptr->entry_id = 0;
    entry_ptr->storage_id = 1;
    
    MMK_CreateWin((uint32*)MMIWIDGETPB_FAVORITE_LIST_WIN_TAB, (ADD_DATA)entry_ptr);  
}

/*****************************************************************************/
//  Description : 创建收藏夹窗口
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGETPB_OpenFavoriteListWin( MMIPB_PHONEBOOK_ENTRY_T  *select_entry_ptr )
{
    MMIPB_PHONEBOOK_ENTRY_T  *entry_ptr = PNULL;
    
    entry_ptr = (MMIPB_PHONEBOOK_ENTRY_T *)SCI_ALLOC_APP(sizeof(MMIPB_PHONEBOOK_ENTRY_T));
    SCI_MEMCPY(entry_ptr, select_entry_ptr, sizeof(MMIPB_PHONEBOOK_ENTRY_T));
    
    MMK_CreateWin((uint32*)MMIWIDGETPB_FAVORITE_LIST_WIN_TAB, (ADD_DATA)entry_ptr);
    
}

/*****************************************************************************/
//  Description : 创建拨号窗口
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_OpenMakeCallWin( void)
{
    	GUI_BOTH_RECT_T both_rect = {0};

	MMI_STRING_T    opt_title_t= {0};
	MMK_CreateWin((uint32*)MMIWIDGETPB_MAKECALL_WIN_TAB,PNULL);
    
       both_rect = MMITHEME_GetWinClientBothRect(WIDGET_PB_MAKECALL_WIN_ID);
	GUIMENU_CreatDynamic(&both_rect,WIDGET_PB_MAKECALL_WIN_ID,MMIWIDGETPB_MAKECALL_OPT_CTRL_ID,GUIMENU_STYLE_THIRD);

	MMI_GetLabelTextByLang(TXT_CALL, &opt_title_t);
	GUIMENU_SetMenuTitle(&opt_title_t,MMIWIDGETPB_MAKECALL_OPT_CTRL_ID);


}
/*****************************************************************************/
//  Description : 创建发短信窗口
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGETPB_OpenSendMessageWin( void)
{
	//GUI_RECT_T menu_rect = MMITHEME_GetClientRect();
    	GUI_BOTH_RECT_T both_rect = {0};
	MMI_STRING_T    opt_title_t = {0};
	MMK_CreateWin((uint32*)MMIWIDGETPB_SEND_MESSAGE_WIN_TAB, PNULL);

       both_rect = MMITHEME_GetWinClientBothRect(WIDGET_PB_SEND_MESSAGE_WIN_ID);
	GUIMENU_CreatDynamic(&both_rect,WIDGET_PB_SEND_MESSAGE_WIN_ID,MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID,GUIMENU_STYLE_THIRD);
	MMI_GetLabelTextByLang(TXT_COMMON_WRITE_MESSAGE, &opt_title_t);
	GUIMENU_SetMenuTitle(&opt_title_t,MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID);
}

/*****************************************************************************/
//  Description : 删除收藏夹记录
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_DeleteFavoriteListItemByIndex(uint16 index)
{
    if (index < MMIWIDGET_PB_WIDGET_NUM)
    {
        RemoveFavorateOneItem((uint16)index);

        s_widgetpb_entry_t.internal_entry_t[index].entry_id= 0;
        s_widgetpb_entry_t.internal_entry_t[index].storage_id_1= 0;
        s_widgetpb_entry_t.internal_entry_t[index].number_index =0;
        SetAnimData(WIDGET_PB_WIN_ID, index);
    }
}

/*****************************************************************************/
//  Description : 删除收藏夹记录
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_DeleteFavoriteListItem(uint16 entry_id , uint16 storage_id)
{
	uint32   		i = 0;
    	for (i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++)
    	{
		if((s_widgetpb_entry_t.internal_entry_t[i].entry_id== entry_id)	&&(s_widgetpb_entry_t.internal_entry_t[i].storage_id_1== storage_id))
		{
            RemoveFavorateOneItem((uint16)i);

			s_widgetpb_entry_t.internal_entry_t[i].entry_id= 0;
			s_widgetpb_entry_t.internal_entry_t[i].storage_id_1 = 0;
			s_widgetpb_entry_t.internal_entry_t[i].number_index =0;
			SetAnimData(WIDGET_PB_WIN_ID, i);
		}
   	}
       MMIWIDGETPB_WriteNvFavoriteList(&s_widgetpb_entry_t);	
}

/*****************************************************************************/
//  Description : 更新记录的图片信息
//  Global resource dependence : none
//  Author: 
//  Note: pb 中修改记录保存时调用该函数
/*****************************************************************************/
PUBLIC void MMIWIDGETPB_ReloadPhotoFileName(uint16 entry_id , uint16 storage_id,MMIPB_CUSTOM_PHOTO_T *photo_ptr)
{
    	GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    	MMI_CTRL_ID_T ctrl_id = 0;
    	GUIANIM_DISPLAY_INFO_T display_info = {0};
	uint32   		i = 0;
	GUIANIM_FILE_INFO_T        file_info = {0};
    	MMITHEME_WIDGET_PB_T pb_widget_theme = {0};
	GUIANIM_DATA_INFO_T             data_info           = {0};

    if (PNULL == photo_ptr)
    {
        return;
    }

    	MMITHEME_GetPBWidgetTheme(&pb_widget_theme);

    	for (i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++)
    	{
		if((s_widgetpb_entry_t.internal_entry_t[i].entry_id == entry_id)&&(s_widgetpb_entry_t.internal_entry_t[i].storage_id_1 == storage_id))
		{

			
				//判断是否修改了大头贴
				ctrl_id = (MMIWIDGET_PB_ANIM_CTRL_ID+i);

				ctrl_info.is_wallpaper_ctrl = FALSE;
			    	ctrl_info.is_ctrl_id = TRUE;
			    	ctrl_info.ctrl_id = ctrl_id;
			    	ctrl_info.ctrl_ptr = PNULL;
			    
			    	display_info.is_update = FALSE;
			    	display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
			    	display_info.is_disp_one_frame = TRUE;

			    	//set file path
			    	file_info.full_path_wstr_ptr = photo_ptr->file_name;
			    	file_info.full_path_wstr_len = photo_ptr->file_name_len;

                    GUIANIM_SetOwnerDraw(ctrl_info.ctrl_id, DrawEntryName);
                    
				if((PNULL != photo_ptr) && (photo_ptr->is_valid))
                {
                    display_info.bg.bg_type = GUI_BG_COLOR;
                    display_info.bg.color = MMI_BACKGROUND_COLOR;
                    display_info.bg.shape = GUI_SHAPE_ROUNDED_RECT;
                    
                    GUIANIM_SetParam(&ctrl_info,
                        PNULL,
                        &file_info,
                        &display_info);
                }
				else
				{
				   	if(s_widgetpb_entry_t.internal_entry_t[i].entry_id == 0 )
				   	{

						if( i == 0)
				    			//set default amin icon
				    			data_info.img_id = IMAGE_WIDGET_PB_ANIM_EMPTY_0;
						else
				    			data_info.img_id = pb_widget_theme.error_img;
						
				   	}
				   	else
				   	{
						if( i == 0)
				    		//set default amin icon
				    			data_info.img_id = IMAGE_WIDGET_PB_ANIM_DEFAULT_0;
						else
				    			data_info.img_id = pb_widget_theme.default_img;
				   	}


		            SCI_MEMSET((uint16*)&display_info.bg, 0, sizeof(display_info.bg));
	   				 
                    GUIANIM_SetParam(&ctrl_info,
					                             &data_info,
					                            PNULL,
					                              &display_info);

				
				}
			
		}
   	}
		


}

/*****************************************************************************/
//  Description : 更新记录的number index 信息
//  Global resource dependence : none
//  Author: 
//  Note: pb 中修改记录保存时调用该函数
/*****************************************************************************/
PUBLIC void  MMIWIDGETPB_ReloadNumberIndex( uint16 entry_id , uint16 storage_id)
{

	MMIPB_PHONEBOOK_ENTRY_T  reload_entry_t ={0};
	uint32	number_index = 0;
	uint32   		i = 0;
	
    	for (i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++)
    	{
		if((s_widgetpb_entry_t.internal_entry_t[i].entry_id == entry_id)&&(s_widgetpb_entry_t.internal_entry_t[i].storage_id_1 == storage_id))
		{
				//判断是否修改了号码
			reload_entry_t.entry_id = s_widgetpb_entry_t.internal_entry_t[i].entry_id;
			reload_entry_t.storage_id= s_widgetpb_entry_t.internal_entry_t[i].storage_id_1;
			if(MMIAPIPB_ReadPhonebookEntey(&reload_entry_t))
			{
				number_index = s_widgetpb_entry_t.internal_entry_t[i].number_index;
			
				if(MMIAPIPB_IsValidPhoneNumber(((uint8)reload_entry_t.number_t[number_index].npi_ton), reload_entry_t.number_t[number_index].number_len))/*lint !e64*/
				{
					s_widgetpb_entry_t.internal_entry_t[i].number_index = number_index;
				}
				else
				{				
					number_index= MMIAPIPB_GetFirstValidNumberFromNumArray(reload_entry_t.number_t,MMIPB_MAX_NV_PHONE_NUM);
					if(number_index < MMIPB_MAX_NV_PHONE_NUM)
						s_widgetpb_entry_t.internal_entry_t[i].number_index = number_index;
					else
						s_widgetpb_entry_t.internal_entry_t[i].number_index = 0;
				}
			}

			}
    		}
}

/*****************************************************************************/
//  Description :移动记录到别的分组
//  Global resource dependence : none
//  Author: 
//  Note: pb 中MMIPB_MoveNVGroup调用
/*****************************************************************************/
PUBLIC void  MMIWIDGETPB_MoveNVGroup( uint16 entry_id , uint16 from_storage_id, uint16 to_storage_id)
{
	uint32   		i = 0;
	
    	for (i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++)
    	{
		if((s_widgetpb_entry_t.internal_entry_t[i].entry_id == entry_id)&&(s_widgetpb_entry_t.internal_entry_t[i].storage_id_1 == from_storage_id))
		{
			s_widgetpb_entry_t.internal_entry_t[i].entry_id = entry_id;
			s_widgetpb_entry_t.internal_entry_t[i].storage_id_1 = to_storage_id;
			MMIWIDGETPB_ReloadNumberIndex(  entry_id , to_storage_id);
			SetAnimData(WIDGET_PB_WIN_ID, i);
		}
    	}
}



/*****************************************************************************/
//  Description : 判断记录是否存在于收藏夹中
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWIDGETPB_IsExistFavoriteList(uint16 entry_id , uint16 storage_id)
{
	MMI_RESULT_E  	result = MMI_RESULT_FALSE;
	uint32  i =0;
	for (i = 0; i < MMIWIDGET_PB_WIDGET_NUM; i++)
    	{
		if((s_widgetpb_entry_t.internal_entry_t[i].entry_id == entry_id)	&&(s_widgetpb_entry_t.internal_entry_t[i].storage_id_1 == storage_id))
		{
			result  =  MMI_RESULT_TRUE;
		}
   	}

	return result;
	  
}

/*****************************************************************************/
//  Description : 删除收藏夹中的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void RemoveFavorateOneItem(uint16 index)
{
	AppendItemToFavoriteListBox(
				MMIWIDGETPB_FAVORITE_LISTBOX_CTRL_ID, 
				index,
				NULL,
				STXT_OPTION,
				STXT_MAIN_SETTING,
				STXT_RETURN,
				TRUE);
    //MMIPB_UpdateCurrentList();
}

/*****************************************************************************/
//  Description : 收藏夹列表是否已打开
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWIDGETPB_IsExistFavoriteListWin()
{
    return MMK_IsExistWin(MMK_GetAppletHandleByWin(WIDGET_PB_FAVORITE_LIST_WIN_ID), \
        WIDGET_PB_FAVORITE_LIST_WIN_ID);
}
#endif



/*Edit by script, ignore 4 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
