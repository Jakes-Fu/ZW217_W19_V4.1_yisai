/*****************************************************************************
** File Name:      mmiwidget_pb.c                                            *
** Author:                                                                   *
** Date:           07/22/2011                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/22/2011     hongbo.lan         Creat
******************************************************************************/

#define _MMIGRID_PB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_widget_trc.h"
#if defined MMI_GRID_CONTACT

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
//#include "mmiwidget_menutable.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmisms_text.h"
#include "mmiudisk_export.h"
#include "mmiwidget_position.h"
#include "mmipb_id.h"
#include "mmipb_app.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif

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

//点中区域的标识
typedef enum
{
    MMIWIDGET_PB_HIT_NONE,         //无
    MMIWIDGET_PB_HIT_ADD,    
    MMIWIDGET_PB_HIT_CALL,        //拨号
    MMIWIDGET_PB_HIT_SMS,         //发短信

    MMIWIDGET_PB_HIT_MAX
}MMIGRID_PB_CLICK_TYPE_E;


typedef enum _MMIGRID_INTERFACE_TYPE_E
{
	MMIGRID_INTERFACE_TYPE_MAKECALL_AUDIO,      // 语音电话,@zhaohui add
	#ifdef VT_SUPPORT
	MMIGRID_INTERFACE_TYPE_MAKECALL_VIDEO,//可视电话,@zhaohui add
	#endif
	//MMIGRID_INTERFACE_TYPE_MAKECALL,      // 拨号
	MMIGRID_INTERFACE_TYPE_MAKEIPCALL,    // IP拨号
	MMIGRID_INTERFACE_TYPE_SENDMSG,       // 发送信息
	MMIGRID_INTERFACE_TYPE_SETSPEEDKEY,   // 设置单键拨号
	MMIGRID_INTERFACE_TYPE_SETMTFW,       // 设置黑名单
	MMIGRID_INTERFACE_TYPE_MAX
}MMIGRID_INTERFACE_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//点中区域的标识
LOCAL uint32 s_pb_click_add_type     = MMIWIDGET_PB_HIT_NONE;
LOCAL uint32 s_pb_click_contact_type[MMIGRID_PB_WIDGET_NUM] = {MMIWIDGET_PB_HIT_NONE};

//dial sub menu
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
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
#endif

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
LOCAL MMIGRID_PB_FAVORITE_ENTRY_T  	s_grid_pb_entry_t = {0}; //记录收藏夹中所有记录在pb中entry id 和group id
LOCAL uint16                        s_hit_index      = 0;
LOCAL BOOLEAN                       s_is_first_read_nv = TRUE;
/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void  ReadNvFavoriteList(MMIGRID_PB_FAVORITE_ENTRY_T *entry_t);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void  WriteNvFavoriteList(MMIGRID_PB_FAVORITE_ENTRY_T *entry_t);
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void OpenMakeCallWin(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void OpenSendMessageWin(MMI_WIN_ID_T win_id);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendNewMessage(
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
LOCAL void MakeCall(
                    uint16 entry_id , 
                    uint16 storage_id ,
                    uint8 number_index,
                    MMIGRID_INTERFACE_TYPE_E  interface_type
                    );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MakeNumberCallByBCD(
                                MN_DUAL_SYS_E dual_sys,
                                MMIGRID_INTERFACE_TYPE_E  interface_type, //IN:
                                MMIPB_BCD_NUMBER_T      *bcd_number_ptr, //IN:
                                CC_CALL_SIM_TYPE_E call_sim_type
                                );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void MakeNumberCallByString(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMIGRID_INTERFACE_TYPE_E  interface_type, //IN:
                                    MMIPB_STRING_T          *number_str_ptr, //IN:
                                    CC_CALL_SIM_TYPE_E call_sim_type
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
                                                DPARAM param);

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DrawAddWin(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DrawContactWin(MMI_WIN_ID_T  win_id);
/*****************************************************************************/
// Description : draw button contact win.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DrawButtonContactWin(MMI_WIN_ID_T  win_id);
/*****************************************************************************/
// Description : get pb widget content by index
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void GetPbWidgetContentByIndex(MMI_WIN_ID_T  win_id,
                                     MMIPB_CUSTOM_PHOTO_T *custom_photo_ptr,    //out
                                     GUI_RECT_T *disp_rect_ptr,
                                     uint16 index
                                     );

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void SetAnimCtrlData(MMI_WIN_ID_T  win_id);
LOCAL void DestroyAnimCtrl(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void CreateAnimCtrl(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description :  TP 消息处理
// Global resource dependence : none
// Author: 
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAddWinTpPressDown(MMI_WIN_ID_T  win_id, DPARAM param);

/*****************************************************************************/
// Description :  TP 消息处理
// Global resource dependence : none
// Author: 
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbWinTpPressDown(MMI_WIN_ID_T  win_id, DPARAM param);

/*****************************************************************************/
// Description : TP 消息处理
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
  LOCAL void HandleAddWinTpPressUp(MMI_WIN_ID_T  win_id, DPARAM  param);

/*****************************************************************************/
// Description : TP 消息处理
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
 LOCAL void HandlePbWinTpPressUp(MMI_WIN_ID_T  win_id, DPARAM  param);

 /*****************************************************************************/
// Description : reset pb button status.
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL void ResetWidgetPb();

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
LOCAL void DrawEntryName(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Handle widget contact adding Win Msg
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetContactAddWinMsg(MMI_WIN_ID_T      win_id,
                                                 MMI_MESSAGE_ID_E  msg_id,
                                                 DPARAM            param);

/*****************************************************************************/
//     Description : Handle WidgetPb Win Msg
//    Global resource dependence : 
//  Author:hongbo.lan
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetContactWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
// Description : get current index of contact entry.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL uint32 GetCurContactIdx(MMI_WIN_ID_T win_id);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
/**                         Constant Variables                                */
/**---------------------------------------------------------------------------*/
//拨号选择窗口
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
LOCAL WINDOW_TABLE(MMIWIDGETPB_MAKECALL_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleWidgetPbMakeCallWinMsg ),    
    WIN_ID( WIDGET_PB_MAKECALL_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
    END_WIN    
};
#endif

//短信发送选择窗口
LOCAL WINDOW_TABLE(MMIWIDGETPB_SEND_MESSAGE_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleWidgetPbWriteMessageWinMsg ),    
    WIN_ID( WIDGET_PB_SEND_MESSAGE_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
    END_WIN    
};


/**--------------------------------------------------------------------------*
**                                                                                                                                   *
**--------------------------------------------------------------------------*/
PUBLIC const MMIWIDGET_ITEM_INFO_T g_contact_add_widget =
{
    MMIWIDGET_CONTACT_ADD_ID, 
    HandleWidgetContactAddWinMsg, 
    PNULL,
    ResetWidgetPb,
    PNULL,
    WIDGET_CONTACT_ADD_WIN_ID, 
    IMAGE_WIDGET_PB_SHORTCUT, 
    IMAGE_GRID_PB_BG,
    TXT_COMMON_PB,
    50,
    50,
    FALSE
};
/*lint -e651*/
PUBLIC const MMIWIDGET_ITEM_INFO_T g_contact_widget[] =
{
    {
        MMIWIDGET_CONTACT_ID0, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,
        WIDGET_CONTACT_0_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+1, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,
        WIDGET_CONTACT_1_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+2, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,        
        WIDGET_CONTACT_2_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+3, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,        
        WIDGET_CONTACT_3_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+4, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,        
        WIDGET_CONTACT_4_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    }, 
    {
        MMIWIDGET_CONTACT_ID0+5, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,        
        WIDGET_CONTACT_5_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+6, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,        
        WIDGET_CONTACT_6_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+7, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,        
        WIDGET_CONTACT_7_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+8, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,        
        WIDGET_CONTACT_8_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },
    {
        MMIWIDGET_CONTACT_ID0+9, 
        HandleWidgetContactAddWinMsg, 
        PNULL,
        ResetWidgetPb,
        PNULL,
        WIDGET_CONTACT_9_WIN_ID, 
        IMAGE_WIDGET_PB_SHORTCUT, 
        IMAGE_GRID_PB_BG,
        TXT_COMMON_PB,
        50,//    50, 
        50,//    50
        TRUE
    },     
};
/*lint +e651*/
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : add contact callback.
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void AddContactCallback(MMIPB_HANDLE_T handle)
{
    if (MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)handle) > 0)
    {
        MMIPB_CONTACT_T *contact = PNULL;  
        uint16 number_index = 0;                
        
        contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
        SCI_MEMSET(contact, 0x00, sizeof(MMIPB_CONTACT_T));
        if (MMIAPIPB_GetSelectContact((MMIPB_HANDLE_T)handle,contact, 0, &number_index))
        {    
            s_grid_pb_entry_t.internal_entry_t[s_hit_index].entry_id     = contact->contact_id;
            s_grid_pb_entry_t.internal_entry_t[s_hit_index].storage_id_1     = contact->storage_id;
            s_grid_pb_entry_t.internal_entry_t[s_hit_index].number_index = number_index;
            s_grid_pb_entry_t.internal_entry_t[s_hit_index].s_is_contact_null = CONTACT_NOTEMPTY;
            CreateAnimCtrl(s_hit_index + WIDGET_CONTACT_0_WIN_ID);            
            SetAnimCtrlData(s_hit_index + WIDGET_CONTACT_0_WIN_ID);
            WriteNvFavoriteList(&s_grid_pb_entry_t);
        }
        
        SCI_FREE(contact);
    }
}

/*****************************************************************************/
//    Description : reset pb button status.
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void ResetWidgetPb()
{
    uint32 i = 0;
    
    s_pb_click_add_type = MMIWIDGET_PB_HIT_NONE;

    for (i = 0; i < MMIGRID_PB_WIDGET_NUM; i++)
    {
        s_pb_click_contact_type[i] = MMIWIDGET_PB_HIT_NONE;
    }
}

/*****************************************************************************/
//     Description : read favorite list from nv
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void ReadNvFavoriteList(MMIGRID_PB_FAVORITE_ENTRY_T *entry_t)
{
    MN_RETURN_RESULT_E				return_value = MN_RETURN_FAILURE;
    uint32 				i=0;
    MMIGRID_PB_FAVORITE_ENTRY_T  widgetpb_entry_t = {0};

    SCI_MEMSET(entry_t, 0, sizeof(MMIGRID_PB_FAVORITE_ENTRY_T))
    MMINV_READ(MMI_GRID_PB_ENTRY, entry_t, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
		for( i = 0; i < MMIGRID_PB_WIDGET_NUM; i++ )
		{
			widgetpb_entry_t.internal_entry_t[i].entry_id = 0;
			widgetpb_entry_t.internal_entry_t[i].storage_id_1 = 0;
			widgetpb_entry_t.internal_entry_t[i].number_index = 0;
            widgetpb_entry_t.internal_entry_t[i].s_is_contact_null = CONTACT_INIT;
		}
		
        MMINV_WRITE(MMI_GRID_PB_ENTRY, &widgetpb_entry_t);
    }
    
    return ;
}
/*****************************************************************************/
// Description : save favorite list to nv
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL void  WriteNvFavoriteList(MMIGRID_PB_FAVORITE_ENTRY_T *entry_t)
{    
    MMINV_WRITE(MMI_GRID_PB_ENTRY, entry_t);
}

/*****************************************************************************/
//  Description :根据entry id 和group id 获取pb 记录并发送信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendNewMessage(uint16 entry_id , uint16 storage_id ,uint8 number_index,WIDGETPB_MESSAGE_INTERFACE_TYPE_E  message_interface_type)
{
    MN_DUAL_SYS_E 	        sim_type         = MN_DUAL_SYS_1;
#ifdef MMIPB_MAIL_SUPPORT
    MMIPB_MAIL_T            mail_t           = {0};
#endif
    BOOLEAN                 is_include_email = FALSE;
    MMIPB_BCD_NUMBER_T      smsnumber        = {0}; 
    MMIPB_STRING_T          number_t         = {0};
    MMI_STRING_T 		    string_to        = {0};
    MMIPB_CONTACT_T *contact_info = PNULL;  
    MMIPB_ERROR_E           ret         =   MMIPB_ERROR_ERROR;

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
    
    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    ret = MMIAPIPB_GetContactInfo(entry_id, storage_id, contact_info);
		
    if (ret != MMIPB_ERROR_SUCCESS)
    {
        SCI_FREE(contact_info);
        //SCI_TRACE_LOW:"[mmipb] MakeCall ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_PB_664_112_2_18_3_7_28_68,(uint8*)"d",ret);
        return;
    }	

    smsnumber = contact_info->number[0];	   
#ifdef MMIPB_MAIL_SUPPORT
    if(contact_info->mail.wstr_len >0)
    {
        //exist valid number or email address
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        mail_t = contact_info->mail;
#else
        if(WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS == message_interface_type)
        {
            mail_t = contact_info->mail;
        }
#endif
    }											
#endif

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
 #ifdef MMIPB_MAIL_SUPPORT
    else if(mail_t.wstr_len > 0)
    {
        number_t.strlen = MIN(mail_t.wstr_len, MMIPB_MAX_STRING_LEN);
        MMI_WSTRNTOSTR((char *)number_t.strbuf, MMIPB_MAX_STRING_LEN, mail_t.wstr, MMIPB_MAX_STRING_LEN, number_t.strlen);/*lint !e64*/
    }
 	else
 	{ 
        MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);    
 	}
#endif
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
    SCI_FREE(contact_info);
}

/*****************************************************************************/
//  Description :根据entry id 和group id 获取pb 记录并拨打电话
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MakeCall(uint16 entry_id , uint16 storage_id , uint8 number_index,MMIGRID_INTERFACE_TYPE_E  interface_type)
{
    MMIPB_BCD_NUMBER_T 		callnumber = {0}; 
    MMIPB_CONTACT_T *contact_info = PNULL;  
    MMIPB_ERROR_E           ret         =   MMIPB_ERROR_ERROR;

    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    
    ret = MMIAPIPB_GetContactInfo(entry_id, storage_id, contact_info);
    if (ret != MMIPB_ERROR_SUCCESS)
    {
        SCI_FREE(contact_info);
        //SCI_TRACE_LOW:"[mmipb] MakeCall ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_PB_747_112_2_18_3_7_28_69,(uint8*)"d",ret);
        return;
    }	

    if(MMIAPIPB_IsValidPhoneNumber(contact_info->number[number_index].npi_ton, 
    							contact_info->number[number_index].number_len))
    {
        callnumber = contact_info->number[0];
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE						            
    //make call by bcd number
    MakeNumberCallByBCD(
    			MN_DUAL_SYS_MAX,
    			interface_type,
    			&callnumber,
    			CC_CALL_SIM_MAX
    			);				           
#else						           
    //make call by bcd number
    MakeNumberCallByBCD(
    			MN_DUAL_SYS_1,
    			interface_type,
    			&callnumber,
    			CC_SIM1_CALL
    			);						            
#endif
    SCI_FREE(contact_info);
}

/*****************************************************************************/
//  Description :使用number 拨号
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MakeNumberCallByBCD(MN_DUAL_SYS_E dual_sys,
                                          MMIGRID_INTERFACE_TYPE_E  interface_type, //IN:
                                          MMIPB_BCD_NUMBER_T      *bcd_number_ptr, //IN:
                                          CC_CALL_SIM_TYPE_E call_sim_type)
{
    MMIPB_STRING_T      number_t = {0};

    //SCI_ASSERT(PNULL != bcd_number_ptr);

    if (PNULL == bcd_number_ptr)
    {
        //SCI_TRACE_LOW:"MakeNumberCallByBCD PNULL == bcd_number_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_PB_791_112_2_18_3_7_28_70,(uint8*)"");
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
    MakeNumberCallByString(dual_sys, interface_type, &number_t, call_sim_type);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void MakeNumberCallByString(MN_DUAL_SYS_E dual_sys,
                                             MMIGRID_INTERFACE_TYPE_E  interface_type, //IN:
                                             MMIPB_STRING_T          *number_str_ptr, //IN:
                                             CC_CALL_SIM_TYPE_E call_sim_type)
{
    if(PNULL == number_str_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MakeNumberCallByString number_str_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_PB_816_112_2_18_3_7_28_71,(uint8*)"");
        return;
    }
    
    switch (interface_type)
    {
    case MMIGRID_INTERFACE_TYPE_MAKECALL_AUDIO://语音电话
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
    case MMIGRID_INTERFACE_TYPE_MAKECALL_VIDEO://可视电话
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
    case MMIGRID_INTERFACE_TYPE_MAKEIPCALL:
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
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
/*****************************************************************************/
//  Description : create call popup menu.
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateCallPopupMenu(MMI_WIN_ID_T  win_id)
{
    uint16 i = 0;
    uint16 root_item_num = 0;
    MMI_STRING_T  string = {0};
    GUIMENU_DYNA_ITEM_T menu_item = {0};

    GUIMENU_CreatDynamicEx(200, PNULL, win_id, MMIWIDGETPB_MAKECALL_OPT_CTRL_ID, GUIMENU_STYLE_POPUP);

    MMI_GetLabelTextByLang(TXT_CALL, &string);

    GUIMENU_SetMenuTitle(&string, MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID);

	root_item_num = sizeof(s_widgetpb_dail_menu_sub_item)/sizeof(*s_widgetpb_dail_menu_sub_item);			
	
	for(i = 0; i < root_item_num; i++)
	{
		MMI_GetLabelTextByLang(s_widgetpb_dail_menu_sub_item[i][1], &string);
		menu_item.item_text_ptr = &string;
		GUIMENU_InsertNode(i, s_widgetpb_dail_menu_sub_item[i][0], PNULL, &menu_item, MMIWIDGETPB_MAKECALL_OPT_CTRL_ID);				
    }
}

/*****************************************************************************/
//     Description : Handle WidgetPb make call Win Msg
//    Global resource dependence : 
//    Author:hongbo.lan
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbMakeCallWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
	MMI_RESULT_E  	       result              = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T 	       ctrl_id             = MMIWIDGETPB_MAKECALL_OPT_CTRL_ID;
	uint16        		   curr_select_index   =0;
	MMIGRID_INTERFACE_TYPE_E call_interface_type = MMIGRID_INTERFACE_TYPE_MAKECALL_AUDIO;
    MMIWIDGETPB_FAVORITE_INTERNAL_ENTRY_T *entry_ptr = PNULL;
	
	switch(msg_id)
	{
	    case MSG_OPEN_WINDOW:
            CreateCallPopupMenu(win_id);
		    break;

	    case MSG_APP_OK:
		case MSG_CTL_OK:
		case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	    case MSG_APP_WEB:
			curr_select_index = GUIMENU_GetCurNodeId(ctrl_id);
            entry_ptr		  = MMK_GetWinAddDataPtr(win_id);
            
			if(WIDGETPB_OPTMENU_NODE_DIAL_AUDIO == curr_select_index)
	        {
	            call_interface_type = MMIGRID_INTERFACE_TYPE_MAKECALL_AUDIO;
	        }
#ifdef VT_SUPPORT
	       	else if(WIDGETPB_OPTMENU_NODE_DIAL_VIDEO == curr_select_index)
	        {
	            call_interface_type = MMIGRID_INTERFACE_TYPE_MAKECALL_VIDEO;
	        }
#endif
#ifdef MMI_IP_CALL_SUPPORT
                else if(WIDGETPB_OPTMENU_NODE_IPDIAL == curr_select_index)
	        {
	            call_interface_type = MMIGRID_INTERFACE_TYPE_MAKEIPCALL;
	        }
#endif

			MakeCall(entry_ptr->entry_id, entry_ptr->storage_id_1, entry_ptr->number_index, call_interface_type);
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
#endif
/*****************************************************************************/
//  Description : create messag popup menu.
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void CreateMessagePopupMenu(MMI_WIN_ID_T  win_id)
{
    uint16 i = 0;
    uint16 root_item_num = 0;
    MMI_STRING_T  string = {0};
    GUIMENU_DYNA_ITEM_T menu_item = {0};

              
    GUIMENU_CreatDynamicEx(200, PNULL, win_id, MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID, GUIMENU_STYLE_POPUP_AUTO);

    MMI_GetLabelTextByLang(TXT_COMMON_WRITE_MESSAGE, &string);

    GUIMENU_SetMenuTitle(&string, MMIWIDGETPB_MAKECALL_OPT_CTRL_ID);

	root_item_num = sizeof(s_widgetpb_sendmsg_menu_sub_item)/sizeof(*s_widgetpb_sendmsg_menu_sub_item);			
	
	for(i = 0; i < root_item_num; i++)
	{
		MMI_GetLabelTextByLang(s_widgetpb_sendmsg_menu_sub_item[i][1], &string);
		menu_item.item_text_ptr = &string;
		GUIMENU_InsertNode(i, s_widgetpb_sendmsg_menu_sub_item[i][0], PNULL, &menu_item, MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID);				
    }
}

/*****************************************************************************/
//     Description : Handle WidgetPb send message Win Msg
//    Global resource dependence : 
//    Author:hongbo.lan
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetPbWriteMessageWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E  	result = MMI_RESULT_TRUE;
 	MMI_CTRL_ID_T 	ctrl_id = MMIWIDGETPB_SENDMESSAGE_OPT_CTRL_ID;
 	uint16        		curr_select_index =0;
 	WIDGETPB_MESSAGE_INTERFACE_TYPE_E message_interface_type = WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS;
     MMIWIDGETPB_FAVORITE_INTERNAL_ENTRY_T *entry_ptr = PNULL;
 		
 	switch(msg_id)
    {
 	    case MSG_OPEN_WINDOW:
             CreateMessagePopupMenu(win_id);
 		    break;
 
 	    case MSG_APP_OK:
 		    case MSG_CTL_OK:
 		case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
 		case MSG_CTL_PENOK:   						
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
 			curr_select_index = GUIMENU_GetCurNodeId(ctrl_id);
             entry_ptr		  = MMK_GetWinAddDataPtr(win_id);
 						                 
 			if(WIDGETPB_OPTMENU_NODE_SEND_SMS == curr_select_index)
 			{
 			    message_interface_type = WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS;
 			}
 			else
 			if(WIDGETPB_OPTMENU_NODE_SEND_MMS == curr_select_index)
 			{
 			    message_interface_type =  WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDMMS;	
 			}
             
 			SendNewMessage(entry_ptr->entry_id,
 			 			   entry_ptr->storage_id_1,
 			 			   entry_ptr->number_index,
 			 			   message_interface_type);
			MMK_CloseWin(win_id); 			 			   
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
//	Description : get favorite entry name by index.
//	Global resource dependence : 
//	Author:hongbo.lan
//	Note:
/*****************************************************************************/
LOCAL void GetFavoriteEntryName(uint32 index, MMI_STRING_T *item_text)
{
    wchar              *entry_name_ptr = PNULL;    
  MMIPB_CONTACT_T *contact_info = PNULL;  
    uint16              entry_id = 0;
    uint16              storage_id = 0;
    uint8				tele_prompt_len = 0;
    uint8				temp_prompt_str[MMIWIDGETPB_NUMBER_MAX_LEN + 1]= {0};
    MMIPB_ERROR_E           ret         =   MMIPB_ERROR_ERROR;

 
    entry_name_ptr = item_text->wstr_ptr;
    		
    //get name by number from pb
    entry_id  = s_grid_pb_entry_t.internal_entry_t[index].entry_id;
    storage_id = s_grid_pb_entry_t.internal_entry_t[index].storage_id_1;
    index =	s_grid_pb_entry_t.internal_entry_t[index].number_index;

    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T)); 
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
   ret = MMIAPIPB_GetContactInfo(entry_id, storage_id, contact_info);
		
	if (ret != MMIPB_ERROR_SUCCESS)
	{
            SCI_FREE(contact_info);
            //SCI_TRACE_LOW:"[mmipb] MakeCall ret %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_PB_1090_112_2_18_3_7_29_72,(uint8*)"d",ret);
		return;
	}   
    if(0 == contact_info->name.wstr_len)
    {
        item_text->wstr_len = 0;
    }		
    else
    {
        MMIAPICOM_Wstrncpy(entry_name_ptr, contact_info->name.wstr, contact_info->name.wstr_len); 
        item_text->wstr_len = contact_info->name.wstr_len;
    }
	
    //name is NULL ,show number
    if(item_text->wstr_len == 0)
    {
        if(MMIAPIPB_IsValidPhoneNumber(contact_info->number[index].npi_ton,/*lint !e64*/
                                       contact_info->number[index].number_len))/*lint !e64*/
        {						
            
            tele_prompt_len = MMIAPICOM_GenDispNumber( 
                MMIPB_GetNumberTypeFromUint8(contact_info->number[index].npi_ton),
                contact_info->number[index].number_len,
                contact_info->number[index].number,
                temp_prompt_str,
                MMIPB_MAX_STRING_LEN
                );
            MMI_STRNTOWSTR(entry_name_ptr, tele_prompt_len, temp_prompt_str, 
                            tele_prompt_len, tele_prompt_len);/*lint !e64*/
            item_text->wstr_len = tele_prompt_len;//contact_info->number[index].number_len;                
        }
    }			
    SCI_FREE(contact_info);							

}

/*****************************************************************************/
//  Description : grid pb max num is 10, judget full or not.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGRID_PB_IsContactFull()
{
#if 0
    return ((s_grid_pb_entry_t.total_num == MMIGRID_PB_WIDGET_NUM) ? TRUE : FALSE);
#else
    return ((MMIGRID_PB_WIDGET_NUM == MMIGRIDPB_GetContactNum()) ? TRUE : FALSE);
#endif
}

/*****************************************************************************/
//  Description : Handle widget contact adding Win Msg
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetContactAddWinMsg(MMI_WIN_ID_T      win_id,
                                                 MMI_MESSAGE_ID_E  msg_id,
                                                 DPARAM            param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	uint32 cur_idx = GetCurContactIdx(win_id);

    if (cur_idx >= MMIGRID_PB_WIDGET_NUM)
    {
        return MMI_RESULT_TRUE;
    }
	if ((CONTACT_NOTEMPTY <= s_grid_pb_entry_t.internal_entry_t[cur_idx].s_is_contact_null)) 
	{
        recode = HandleWidgetContactWinMsg(win_id, msg_id, param);
        return recode;
	}

    switch (msg_id)
    {
	    case MSG_OPEN_WINDOW:
			if (s_is_first_read_nv)
			{
                ReadNvFavoriteList(&s_grid_pb_entry_t);
				s_is_first_read_nv = FALSE;
			}

	        break;

	    case MSG_WIDGET_TP_PRESS_DOWN:
			recode = HandleAddWinTpPressDown(win_id,param);	
            DrawAddWin(win_id);
			break;

        case MSG_WIDGET_PAGE_SLIDE_START:
        case MSG_WIDGET_TP_PRESS_LONG_DOWN:
            s_pb_click_add_type = MMIWIDGET_PB_HIT_NONE;
            recode = MMI_RESULT_FALSE;            
            break;            

	    case MSG_WIDGET_TP_PRESS_UP:
		 	HandleAddWinTpPressUp(win_id,param);	 
	        break;

        case MSG_APP_WEB: 
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    0,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    AddContactCallback
                    );								
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_GRID_ADD_WIDGET:
            s_grid_pb_entry_t.internal_entry_t[cur_idx].s_is_contact_null = CONTACT_EMPTY;
            break;
            
        case MSG_FULL_PAINT:	 	  
            //SetAnimCtrlData(win_id);
            DrawAddWin(win_id);
            break;

	   case MSG_GRID_REMOVE_WIDGET:
		   {
			   s_grid_pb_entry_t.internal_entry_t[cur_idx].entry_id     = 0;
			   s_grid_pb_entry_t.internal_entry_t[cur_idx].storage_id_1     = 0;
			   s_grid_pb_entry_t.internal_entry_t[cur_idx].number_index = 0;
			   s_grid_pb_entry_t.internal_entry_t[cur_idx].s_is_contact_null = CONTACT_INIT;
			   WriteNvFavoriteList(&s_grid_pb_entry_t);
			   DestroyAnimCtrl(win_id);
			   break;
		   }

	   default:
		        recode = MMI_RESULT_FALSE;
	        break;
    }
    
    return recode;  
}                                      

/*****************************************************************************/
//     Description : Handle WidgetPb Win Msg
//    Global resource dependence : 
//  Author:hongbo.lan
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetContactWinMsg(
                                      MMI_WIN_ID_T        win_id,
                                      MMI_MESSAGE_ID_E    msg_id,
                                      DPARAM              param  
                                      )
{
    MMI_RESULT_E 		recode = MMI_RESULT_TRUE;
	uint32 cur_idx = GetCurContactIdx(win_id);
    
    if (cur_idx >= MMIGRID_PB_WIDGET_NUM)
    {
		return MMI_RESULT_TRUE;
    }
    switch (msg_id)
    {
	    case MSG_OPEN_WINDOW:
            CreateAnimCtrl(win_id);            
            SetAnimCtrlData(win_id);
	        break;

	    case MSG_WIDGET_TP_PRESS_DOWN:
			recode = HandlePbWinTpPressDown(win_id,param);	
            //DrawContactWin(win_id);  
			DrawButtonContactWin(win_id);
            break;

			
        case MSG_WIDGET_PAGE_SLIDE_START:
        case MSG_WIDGET_TP_PRESS_LONG_DOWN:
            s_pb_click_contact_type[cur_idx] = MMIWIDGET_PB_HIT_NONE;
            recode = MMI_RESULT_FALSE;
            break;
        
	    case MSG_WIDGET_TP_PRESS_UP:
		 	HandlePbWinTpPressUp(win_id,param);	 
	        break;
        
        case MSG_GET_FOCUS:
            SetAnimCtrlData(win_id);

            //s_grid_pb_entry_t.internal_entry_t[cur_idx].s_is_contact_null = CONTACT_NOTEMPTY;
            break;
            
        case MSG_LOSE_FOCUS:
            break;
            
        case MSG_FULL_PAINT:	 	    
            DrawContactWin(win_id);
            break;

        case MSG_NOTIFY_ANIM_DISPLAY_IND:
            //DrawContactWin(win_id);
            {
                BOOLEAN  is_forbidden_paint = TRUE;    
                GUIANIM_DISPLAY_IND_T *anim_ind_ptr = (GUIANIM_DISPLAY_IND_T*)param;
                
#if defined QBTHEME_SUPPORT
                if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
                {
                    is_forbidden_paint = MMIQBIDLE_IsForbidPaint(MMIWIDGET_MP3_ID);                     
                }
#endif
                
#ifdef MMI_GRID_IDLE_SUPPORT
                if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
                {
                    is_forbidden_paint = MMIGRID_IsForbidPaint(MMIWIDGET_MP3_ID);
                }
#endif
                
                if(is_forbidden_paint)
                {
                    break;
                }  
                
				if(MMK_IsFocusWin(MMK_GetParentWinHandle(win_id)))
			    {
                    MMK_SendMsg(anim_ind_ptr->ctrl_handle,  MSG_CTL_PAINT,  0);
			    }
            }
            break;
  
        case MSG_GRID_REMOVE_WIDGET:
			{
				s_grid_pb_entry_t.internal_entry_t[cur_idx].entry_id     = 0;
				s_grid_pb_entry_t.internal_entry_t[cur_idx].storage_id_1     = PB_GROUP_ALL;
				s_grid_pb_entry_t.internal_entry_t[cur_idx].number_index = 0;
				s_grid_pb_entry_t.internal_entry_t[cur_idx].s_is_contact_null = CONTACT_INIT;
				WriteNvFavoriteList(&s_grid_pb_entry_t);
                DestroyAnimCtrl(win_id);
				break;
			}
            
	    default:
		    recode = MMI_RESULT_FALSE;
	        break;
    }
    
    return recode;        
}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DrawAddWin(MMI_WIN_ID_T  win_id)
{
    GUI_POINT_T      pt            = {0};
    GUI_RECT_T       win_rect      = {0};
    GUI_LCD_DEV_INFO lcd_dev_info  = {0};
    GUI_RECT_T       img_rect      = {0};
    uint16           bg_width      = 0;
    uint16           bg_height     = 0;
    MMI_IMAGE_ID_T   img_id        = 0;
    GUI_RECT_T       add_icon_rect = MMI_GRID_PB_ADD_RECT; 
    uint32 cur_idx = GetCurContactIdx(win_id);


    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    img_id = IMAGE_GRID_PB_BG;

    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, img_id, win_id);
    
    img_rect.right  = bg_width  - 1;
    img_rect.bottom = bg_height - 1;
    
    GUIRES_DisplayImg(
        PNULL,
        &win_rect,
        &img_rect,
        win_id,
        img_id,
        &lcd_dev_info
        );

    pt.x = win_rect.left + add_icon_rect.left;
    pt.y = win_rect.top  + add_icon_rect.top;

    if (MMIWIDGET_PB_HIT_ADD == s_pb_click_add_type && cur_idx == s_hit_index)
    {
       img_id = IMAGE_GRID_PB_ADD_HL; 
    }
    else
    {
       img_id = IMAGE_GRID_PB_ADD;     
    }

    GUIRES_DisplayImg(&pt,
                      PNULL,
                      PNULL,
                      win_id,
                      img_id,
                      &lcd_dev_info);
}

/*****************************************************************************/
// Description : get current index of contact entry.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL uint32 GetCurContactIdx(MMI_WIN_ID_T win_id)
{
    uint32 contact_idx = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    item_ptr = (MMIWIDGET_ITEM_T*)MMK_GetWinAddDataPtr(win_id);
    
    contact_idx = item_ptr->item_info.widget_id - MMIWIDGET_CONTACT_ID0;

    return contact_idx;
}

/*****************************************************************************/
// Description : draw contact win.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DrawContactWin(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T img_rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    GUI_RECT_T 		button_call_rect = MMI_GRID_PB_CALL_RECT; 
    GUI_RECT_T 		button_sms_rect  = MMI_GRID_PB_SMS_RECT;
    GUI_RECT_T 		name_rect        = MMI_GRID_PB_NAME_RECT; 
    MMI_IMAGE_ID_T  img_id           = 0;
    MMI_IMAGE_ID_T  sms_img_id       = 0;
    MMI_IMAGE_ID_T  call_img_id      = 0;  
    uint32 cur_idx  = 0;

    cur_idx = GetCurContactIdx(win_id);
        
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    img_id = IMAGE_GRID_PB_BG;
         
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, img_id, win_id);
    
    img_rect.right = (bg_width - 1);
    img_rect.bottom = (bg_height - 1);
    
    GUIRES_DisplayImg(PNULL,
                      &win_rect,
                      &img_rect,
                      win_id,
                      img_id,
                      &lcd_dev_info);
    //name.
    name_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, name_rect);

    img_id = IMAGE_GRID_PB_NAME;

    GUIRES_DisplayImg(PNULL,
                      &name_rect,
                      PNULL,
                      win_id,
                      img_id,
                      &lcd_dev_info);    

    DrawEntryName(win_id);
    
    //sms btn, call btn.
    sms_img_id  = IMAGE_GRID_PB_SMS;
    call_img_id = IMAGE_GRID_PB_CALL;
    
    if (MMIWIDGET_PB_HIT_CALL == s_pb_click_contact_type[cur_idx])
    {
        call_img_id = IMAGE_GRID_PB_CALL_HL;
    }
    else
    if (MMIWIDGET_PB_HIT_SMS == s_pb_click_contact_type[cur_idx])
    {
        sms_img_id = IMAGE_GRID_PB_SMS_HL;    
    }
	   
    button_call_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_call_rect);
    
    GUIRES_DisplayImg(PNULL,
                      &button_call_rect,
                      PNULL,
                      win_id,
                      call_img_id,
                      &lcd_dev_info);

    button_sms_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_sms_rect);
    
    GUIRES_DisplayImg(PNULL,
                      &button_sms_rect,
                      PNULL,
                      win_id,
                      sms_img_id,
                      &lcd_dev_info
                      );
}
/*****************************************************************************/
// Description : draw button contact win.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DrawButtonContactWin(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T img_rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    GUI_RECT_T 		button_call_rect = MMI_GRID_PB_CALL_RECT; 
    GUI_RECT_T 		button_sms_rect  = MMI_GRID_PB_SMS_RECT;
    GUI_RECT_T 		name_rect        = MMI_GRID_PB_NAME_RECT; 
    MMI_IMAGE_ID_T  img_id           = 0;
    MMI_IMAGE_ID_T  sms_img_id       = 0;
    MMI_IMAGE_ID_T  call_img_id      = 0;  
    uint32 cur_idx  = 0;
	
    cur_idx = GetCurContactIdx(win_id);
	
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    //sms btn, call btn.
    sms_img_id  = IMAGE_GRID_PB_SMS;
    call_img_id = IMAGE_GRID_PB_CALL;
    
    if (MMIWIDGET_PB_HIT_CALL == s_pb_click_contact_type[cur_idx])
    {
        call_img_id = IMAGE_GRID_PB_CALL_HL;
    }
    else
		if (MMIWIDGET_PB_HIT_SMS == s_pb_click_contact_type[cur_idx])
		{
			sms_img_id = IMAGE_GRID_PB_SMS_HL;    
		}
		
		button_call_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_call_rect);
		
		GUIRES_DisplayImg(PNULL,
			&button_call_rect,
			PNULL,
			win_id,
			call_img_id,
			&lcd_dev_info);
		
		button_sms_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_sms_rect);
		
		GUIRES_DisplayImg(PNULL,
			&button_sms_rect,
			PNULL,
			win_id,
			sms_img_id,
			&lcd_dev_info
			);
}
/*****************************************************************************/
// Description : get pb widget content by index
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void GetPbWidgetContentByIndex(MMI_WIN_ID_T  win_id,
                                     MMIPB_CUSTOM_PHOTO_T *custom_photo_ptr,  
                                     GUI_RECT_T *disp_rect_ptr,
                                     uint16 index)
{
    MMIPB_CONTACT_T contact = {0};
    GUI_RECT_T      img_rect   = MMI_GRID_PB_IMG_RECT;
    
    if (index >= MMIGRID_PB_WIDGET_NUM)
    {
        return;
    }

    contact.storage_id  = s_grid_pb_entry_t.internal_entry_t[index].storage_id_1;    //1为PB_GROUP_UNCLASS，未分类
    contact.contact_id  = s_grid_pb_entry_t.internal_entry_t[index].entry_id;    //entry_id=0好像无法取到
    
    if (PNULL != custom_photo_ptr)
    {
        MMIAPIPB_ReadPhotoFileName(contact.contact_id, contact.storage_id, custom_photo_ptr);    
    }
    
    if (PNULL != disp_rect_ptr)
    {
        *disp_rect_ptr = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, img_rect);
    }
}

/*****************************************************************************/
// Description : animation ctrl call back function to display entry name.
// Global resource dependence :
// Author:hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void DrawEntryName(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T        disp_rect    = MMI_GRID_PB_NAME_RECT;
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    MMI_STRING_T      entry_name   = {0};
    GUISTR_STATE_T    text_state   = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    GUISTR_STYLE_T    str_style    = {0};
    uint32            cur_idx      = GetCurContactIdx(win_id);
    
    str_style.effect     = FONT_EFFECT_REVERSE_EDGE; ; 
    str_style.angle      = ANGLE_0;
    str_style.align      = ALIGN_LEFT;
    str_style.font       = SONG_FONT_18;
    str_style.font_color = MMITHEME_GetDefaultWidgetThemeColor();

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    entry_name.wstr_ptr = (wchar*)SCI_ALLOCA(CC_MAX_PBNAME_LEN * sizeof(wchar));

    if (PNULL == entry_name.wstr_ptr)
    {
        return;
    }
    
    SCI_MEMSET(entry_name.wstr_ptr, 0x00, (CC_MAX_PBNAME_LEN*sizeof(wchar)));
    GetFavoriteEntryName(cur_idx, &entry_name); 
    entry_name.wstr_len = CC_MAX_PBNAME_LEN;

    //GetPbWidgetContentByIndex(win_id, PNULL, &disp_rect, i); 
    //disp_rect.bottom = disp_rect.top + 20;

    disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);

    GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&disp_rect,
		&disp_rect,
        (const MMI_STRING_T     *)&entry_name,
        &str_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);          

    if (PNULL != entry_name.wstr_ptr)
    {
        SCI_FREE(entry_name.wstr_ptr);
        entry_name.wstr_ptr = PNULL;
    }     
}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void SetAnimCtrlData(MMI_WIN_ID_T win_id)
{
	GUIANIM_CTRL_INFO_T     ctrl_info = {0};
	GUIANIM_FILE_INFO_T file_info  = {0};
	MMI_CTRL_ID_T ctrl_id = 0;
	GUIANIM_DISPLAY_INFO_T display_info = {0};
	MMIPB_CUSTOM_PHOTO_T        *custom_photo_ptr = PNULL;
	GUI_RECT_T disp_rect = {0};
    uint32  cur_idx = GetCurContactIdx(win_id);
    
	if (cur_idx >= MMIGRID_PB_WIDGET_NUM)
	{
    	return;
	}
	
    if(s_grid_pb_entry_t.internal_entry_t[cur_idx].s_is_contact_null != CONTACT_HASPHOTO)
    {
        return;
    }
	ctrl_id = (MMIGRID_PB_ANIM0_CTRL_ID + cur_idx);
       custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
       if(custom_photo_ptr == PNULL)
       {
            return;
       }
    //get image path	
	GetPbWidgetContentByIndex(win_id, custom_photo_ptr, &disp_rect, cur_idx);

    ctrl_info.is_wallpaper_ctrl = FALSE;
	ctrl_info.is_ctrl_id        = TRUE;
	ctrl_info.ctrl_id           = ctrl_id;
	ctrl_info.ctrl_ptr          = PNULL;

	//set file path
	file_info.full_path_wstr_ptr = custom_photo_ptr->file_name;
	file_info.full_path_wstr_len = custom_photo_ptr->file_name_len;
    display_info.is_syn_decode     = TRUE;
	display_info.is_update         = FALSE;
	display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
	display_info.is_disp_one_frame = TRUE;
	display_info.is_crop_rect      = TRUE;
    display_info.is_free_buf_lose_focus = FALSE;

	if(  custom_photo_ptr->is_valid
       && MMIAPIFMM_IsFileExist(custom_photo_ptr->file_name,custom_photo_ptr->file_name_len)
       && !MMIAPIUDISK_UdiskIsRun())
    {
        GUIANIM_SetVisible(ctrl_id, TRUE, FALSE);    
        display_info.bg.bg_type = GUI_BG_COLOR;
        display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;
    	display_info.bg.shape   = GUI_SHAPE_RECT;

   		GUIANIM_SetParam(&ctrl_info,
                         PNULL,
                         &file_info,
                         &display_info);
        GUIANIM_SetCtrlRect(ctrl_id, &disp_rect);
    }
	else
	{
        GUIANIM_SetVisible(ctrl_id, FALSE, FALSE);                       
	}    
    
    WriteNvFavoriteList(&s_grid_pb_entry_t);
    SCI_FREE(custom_photo_ptr);
}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void CreateAnimCtrl(MMI_WIN_ID_T  win_id)
{
    uint16 idx = 0;
    GUIANIM_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T create   = {0};           
    MMI_CTRL_ID_T ctrl_id = 0;
    void    *anim_ptr = PNULL;
    MMIPB_CUSTOM_PHOTO_T        *custom_photo_ptr = {0};
    GUI_RECT_T disp_rect = {0};

    idx = GetCurContactIdx(win_id);

    if (idx >= MMIGRID_PB_WIDGET_NUM)
    {
        return;
    }

    custom_photo_ptr = SCI_ALLOCA(sizeof(MMIPB_CUSTOM_PHOTO_T));
    if(custom_photo_ptr == PNULL)
    {
        return;
    }
    SCI_MEMSET(custom_photo_ptr, 0x00,sizeof(MMIPB_CUSTOM_PHOTO_T));
    ctrl_id = (MMIGRID_PB_ANIM0_CTRL_ID + idx);

    GetPbWidgetContentByIndex(win_id, custom_photo_ptr, &disp_rect, idx);

    init_data.both_rect.h_rect = init_data.both_rect.v_rect = disp_rect;
    
    create.ctrl_id = ctrl_id;
    create.guid = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = win_id;
    create.parent_ctrl_handle = 0;
    create.init_data_ptr = &init_data;
    
    anim_ptr = MMK_CreateControl(&create);

    //set anim ctrl rect
    GUIANIM_SetCtrlRectByPtr(anim_ptr,&disp_rect);

    //set image display rect
    GUIANIM_SetDisplayRectByPtr(anim_ptr,&disp_rect,FALSE);
    SCI_FREE(custom_photo_ptr);
    
    s_grid_pb_entry_t.internal_entry_t[idx].s_is_contact_null = CONTACT_HASPHOTO;
}

LOCAL void DestroyAnimCtrl(MMI_WIN_ID_T  win_id)
{
    MMK_DestroyAllControl(win_id);
}

/*****************************************************************************/
//  Description : add win tp down. 
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAddWinTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    GUI_POINT_T point = {0};
    GUI_RECT_T  pb_add_rect = MMI_GRID_PB_ADD_RECT; 
    uint32       cur_idx       = GetCurContactIdx(win_id);
	
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

	s_hit_index = cur_idx;

    if (GUI_PointIsInRect(point, pb_add_rect))// && )//(s_grid_pb_entry_t.total_num < MMIGRID_PB_WIDGET_NUM))
    {
        s_pb_click_add_type = MMIWIDGET_PB_HIT_ADD;
        ret = MMI_RESULT_TRUE;
    }
    else
    {
        s_pb_click_add_type = MMIWIDGET_PB_HIT_NONE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : tp down处理
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbWinTpPressDown(MMI_WIN_ID_T  win_id, DPARAM param)
{
    MMI_RESULT_E ret           = MMI_RESULT_FALSE;
    GUI_POINT_T  point         = {0};
    GUI_RECT_T   call_btn_rect = MMI_GRID_PB_CALL_RECT; 
    GUI_RECT_T 	 sms_btn_rect  = MMI_GRID_PB_SMS_RECT;
    uint32       cur_idx       = GetCurContactIdx(win_id);

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    s_hit_index = cur_idx;
    if (GUI_PointIsInRect(point, call_btn_rect))
    {
        s_pb_click_contact_type[cur_idx] = MMIWIDGET_PB_HIT_CALL;
        ret = MMI_RESULT_TRUE;
    }
    else if (GUI_PointIsInRect(point, sms_btn_rect))
    {
        s_pb_click_contact_type[cur_idx] = MMIWIDGET_PB_HIT_SMS;
        ret = MMI_RESULT_TRUE;
    }
    else
    {
        s_pb_click_contact_type[cur_idx] = MMIWIDGET_PB_HIT_NONE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : tp up 处理
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void HandleAddWinTpPressUp(
                       MMI_WIN_ID_T  win_id,   
                       DPARAM            param        //IN:
                       )
{
    GUI_POINT_T       	point 	={0};
    
    point.x =  MMK_GET_TP_X(param);
    point.y =  MMK_GET_TP_Y(param);
    point   = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);


    if (MMIWIDGET_PB_HIT_ADD == s_pb_click_add_type)
    {
    /*	MMIAPIPB_OpenNumberList(MMIPB_LIST_FOR_SMS, 
                                MMIPB_LIST_CHECK,  
                                0,       
                                PNULL);*/
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    0,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    AddContactCallback
                    );
                                
        s_pb_click_add_type = MMIWIDGET_PB_HIT_NONE;                                
    }
}

/*****************************************************************************/
//  Description : tp up 处理
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void HandlePbWinTpPressUp(MMI_WIN_ID_T  win_id, DPARAM param)
{
    GUI_POINT_T       	point 	={0};
    GUI_RECT_T 		button_call_rect = MMI_GRID_PB_CALL_RECT; 
    GUI_RECT_T 		button_sms_rect  = MMI_GRID_PB_SMS_RECT;
    uint32          cur_idx = GetCurContactIdx(win_id);
    
    point.x =  MMK_GET_TP_X(param);
    point.y =  MMK_GET_TP_Y(param);
    point   = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if(GUI_PointIsInRect(point, button_call_rect))
    {        
        if(s_grid_pb_entry_t.internal_entry_t[cur_idx].entry_id== 0)
        {
            return;
        }
        else if(s_pb_click_contact_type[cur_idx] == MMIWIDGET_PB_HIT_CALL)
        {
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
            OpenMakeCallWin(win_id);          
#else
            MakeCall(s_grid_pb_entry_t.internal_entry_t[cur_idx].entry_id, s_grid_pb_entry_t.internal_entry_t[cur_idx].storage_id_1, s_grid_pb_entry_t.internal_entry_t[cur_idx].number_index, MMIGRID_INTERFACE_TYPE_MAKECALL_AUDIO);
#endif
        }                
    }
    else
    if(GUI_PointIsInRect(point, button_sms_rect))
    {        
        if(s_grid_pb_entry_t.internal_entry_t[cur_idx].entry_id== 0)
        {
            return;
        }
        else  if(s_pb_click_contact_type[cur_idx] == MMIWIDGET_PB_HIT_SMS)
        {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            SendNewMessage(s_grid_pb_entry_t.internal_entry_t[cur_idx].entry_id,
                s_grid_pb_entry_t.internal_entry_t[cur_idx].storage_id_1,
                s_grid_pb_entry_t.internal_entry_t[cur_idx].number_index,
                WIDGETPB_MESSAGE_INTERFACE_TYPE_SENDSMS);
#else
            OpenSendMessageWin(win_id);
#endif
        }        
    }

    s_pb_click_contact_type[cur_idx] = MMIWIDGET_PB_HIT_NONE;
}

#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
/*****************************************************************************/
//  Description : create call screen.
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void OpenMakeCallWin(MMI_WIN_ID_T win_id)
{
    uint32 cur_idx = GetCurContactIdx(win_id);

	MMK_CreateWin((uint32*)MMIWIDGETPB_MAKECALL_WIN_TAB, &s_grid_pb_entry_t.internal_entry_t[cur_idx]);
}
#endif
/*****************************************************************************/
//  Description : 创建发短信窗口
//  Global resource dependence : none
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void OpenSendMessageWin(MMI_WIN_ID_T win_id)
{
    uint32 cur_idx = GetCurContactIdx(win_id);
    
	MMK_CreateWin((uint32*)MMIWIDGETPB_SEND_MESSAGE_WIN_TAB, &s_grid_pb_entry_t.internal_entry_t[cur_idx]);
}

/*****************************************************************************/
//  Description : 获得点击的widget id
//  Global resource dependence : none
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIGRIDPB_GetWidgetId(void)
{
	return s_hit_index + MMIWIDGET_CONTACT_ID0;
}
/*****************************************************************************/
//  Description : If Contact is Null
//  Global resource dependence : none
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGRIDPB_GetContactisNull(uint32 index)
{
	return	s_grid_pb_entry_t.internal_entry_t[index].s_is_contact_null;
}

/*****************************************************************************/
//  Description : 获得不是空的Contact的数目
//  Global resource dependence : none
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIGRIDPB_GetContactNum(void)
{
	uint32   		i = 0;
	uint32   	  sum = 0;
	for (i = 0; i < MMIGRID_PB_WIDGET_NUM; i++)
	{
		if(CONTACT_INIT != s_grid_pb_entry_t.internal_entry_t[i].s_is_contact_null)
		{
			sum++;
		}
   	}
	return sum;
}

/*****************************************************************************/
//  Description : reset to facotry settings
//  Global resource dependence : none
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_PB_ResetFactorySettings(void)
{
	SCI_MEMSET(&s_grid_pb_entry_t, 0, sizeof(s_grid_pb_entry_t));
}
#endif


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
