/****************************************************************************
** File Name:      mmipb_view.c                                         *
** Author:                                                                 *
** Date:           06/11/2004                                              *
** Copyright:      2004 SpreaDtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the window for phonebook  *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2011        maryxiao        Create
** 
****************************************************************************/
#define _MMIPB_WINTAB_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmipb_set.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "guitoolbar.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_timer.h"
//#include "mmipb_internal.h"
#include "mmiaudio_ctrl.h" 
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
#include "mmifdn_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "window_parse.h"
#include "mmi_default.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmimms_text.h"
#include "mmi_appmsg.h"
//#include "guictrl_api.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef PIM_SUPPORT
#include "mmipim_export.h"
#endif
#include "guiform.h"
//#include "guires.h"
//#include "mmiim_multitap.h"
#include "guistring.h"
//#include "im_image.h"

//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#if (defined MMIWIDGET_SUPPORT ) || (defined MMI_GRID_IDLE_SUPPORT )
#include "mmiwidget_export.h"
#include "mmiwidget_image.h"
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

#include "mmipb_menu.h"
#include "mmipb_view.h"

#include "mmi_modu_main.h"

#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
LOCAL MMI_TEXT_ID_T s_display_opt_list[]={
    TXT_COMMON_ALL,
    TXT_PHONE,
    TXT_SIM_SEL_SIM1,
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    TXT_SIM_SEL_SIM2,             //SIM卡2
    #if (defined MMI_MULTI_SIM_SYS_TRI||defined MMI_MULTI_SIM_SYS_QUAD)
    TXT_SIM_SEL_SIM3,             //SIM卡3
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    TXT_SIM_SEL_SIM4,             //SIM卡4
    #endif
    #endif
};
#endif

LOCAL BOOLEAN WritePbSet(uint32 item_id, uint32  value);
LOCAL uint32 ReadPbSet(uint32 item_id);

#ifdef MMI_PDA_SUPPORT //ping.gao modify for non-pda compile error
#define MENU_TYPE_SIGLE_LINE   (1<<0)
#define MENU_TYPE_ICON          (1<<1)
#define MENU_TYPE_FIXED_SUB_TEXT    (1<<2)
#define MENU_TYPE_SPECIAL_SUB   (1<<3)

#define MMIPB_SET_DISPLAY_OPT_LIST_MAX (MN_DUAL_SYS_MAX+2)
#define MMIPB_SET_DISPLAY_PERIOD_LIST_MAX (4)
#define MMIPB_SET_UPDATE_PERIOD_LIST_MAX (6)



//#define MMIPB_SNS_GROUP_NUM           (uint8)(3)
#define MMIPB_SET_ACCOUNT_LIST_MAX   (7)

//#define MMIPB_ISSPLIT(X)     (X == 0XFFFFFFFF)
//#define SPLIT_DATA          0XFFFFFFFF
#define MMIPB_SNS_FLAG   (1<<31)

#define PB_NET_CONNECT_INDEX (0)
#define PB_AUTO_UPDATE_INDEX (1)


typedef enum
{
    PB_GROUP_SNS_FACEBOOK = MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM,
    PB_GROUP_SNS_TWITTER,
    PB_GROUP_SNS_SINA,
    PB_GROUP_SNS_MAX
}PB_GROUP_SNS_LIST_E;

//typedef void (*DRAW_SUBMENU)(void);

typedef struct mmi_pbset_menu_s{
    MMI_TEXT_ID_T res_id; // pb set main menu main text
    uint32 menu_type; // pb set main menu item type
    MMI_TEXT_ID_T sub_fix_id;  // pb set main menu sub fixed text
    uint32 nv_id;       // pb set main menu nv id
    MMI_TEXT_ID_T* sub_array; // pb set menu sub menu 
    uint32 sub_count; // pb set menu sub menu count
    void (*draw_func)(MMI_HANDLE_T applet_handle );  //pb set menu sub menu draw function
}MMIPB_SET_MENU_T;

LOCAL MMI_TEXT_ID_T s_pos_list[]={
    TXT_PB_MOBLIE,
   TXT_SIM_SEL_SIM1,
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    TXT_SIM_SEL_SIM2,             //SIM卡2
    #if (defined MMI_MULTI_SIM_SYS_TRI||defined MMI_MULTI_SIM_SYS_QUAD)
    TXT_SIM_SEL_SIM3,             //SIM卡3
    #endif
    #ifdef MMI_MULTI_SIM_SYS_QUAD
    TXT_SIM_SEL_SIM4,             //SIM卡4
    #endif
    #endif
   // TXT_PB_SET_TCARD,
    TXT_COMMON_ASK_EVERYTIME,
};

LOCAL MMI_TEXT_ID_T s_display_period_list[MMIPB_SET_DISPLAY_PERIOD_LIST_MAX]={
    TXT_PB_SET_DISPLAY_PERIOD_1DAY,
    TXT_PB_SET_DISPLAY_PERIOD_3DAY,
    TXT_PB_SET_DISPLAY_PERIOD_7DAY,
    TXT_PB_SET_DISPLAY_PERIOD_30DAY, 
};

LOCAL MMI_TEXT_ID_T s_update_period_list[MMIPB_SET_UPDATE_PERIOD_LIST_MAX]={
    TXT_PB_SET_UPDATE_PERIOD_NONE,
    TXT_PB_SET_UPDATE_PERIOD_30MIN,
    TXT_PB_SET_UPDATE_PERIOD_1HOUR,
    TXT_PB_SET_UPDATE_PERIOD_4HOUR,
    TXT_PB_SET_UPDATE_PERIOD_8HOUR,
    TXT_ALARM_MODE_EVERYDAY
};
#ifdef SNS_SUPPORT
typedef enum
{
     MMIPB_SNS_GROUP_ID,
     MMIPB_SNS_GROUP_TEXT,
     MMIPB_SNS_GROUP_ICON,    
     MMIPB_SNS_GROUP_ITEM_NUM
} MMIPB_SNS_GROUP_ACCPUNT_E;

LOCAL const  uint32 s_defaut_sns_list[MMIPB_SNS_GROUP_NUM][MMIPB_SNS_GROUP_ITEM_NUM] = {
    MMIPB_GROUP_SNS_SINA,TXT_MAINMENU_ICON_SINA,  IMAGE_PHONEBOOK_GROUP_SINA,
    MMIPB_GROUP_SNS_TIWTTER,TXT_COMMON_TIWTTER, IMAGE_PHONEBOOK_GROUP_TWITTER,
    MMIPB_GROUP_SNS_FACEBOOK,TXT_COMMON_FACEBOOK, IMAGE_PHONEBOOK_GROUP_FACEBOOK,
};
#endif
LOCAL uint32 s_group_id[MMIPB_MAX_GROUP_NUM] = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#if defined(MMI_SPEEDDIAL_SUPPORT) 
/*****************************************************************************/
//  Description : open speeaddial win
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL void  MMIPB_OpenSpeedDialWin(MMI_HANDLE_T applet_handle);
#endif

#ifdef SNS_SUPPORT 
LOCAL void  MMIPB_OpenSetAccountWin(MMI_HANDLE_T applet_handle);
#endif

/*****************************************************************************/
//  Description : pb设置主菜单处理函数
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetMenuWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
/*****************************************************************************/
// 	Description : pb设置新建联系人保存位置窗口处理函数
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetSavePosWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif

/*****************************************************************************/
//  Description :  详情message 处理函数 
//  Global resource dependence : 
//  Author: gaoping
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDetailMessageWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                );

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开本机号码设置
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_OpenMSISDN(MMI_HANDLE_T applet_handle);
#endif

#ifdef SNS_SUPPORT 
/*****************************************************************************/
//  Description : pb账户设置处理函数
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetAccountWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
#endif

WINDOW_TABLE( MMIPB_DETAILS_MESSAGE_LIST_TAB_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleDetailMessageWinMsg),    
    WIN_ID( MMIPB_DETAILS_MESSAGE_LIST_TAB_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID),
    
    END_WIN
};

WINDOW_TABLE( MMIPB_SET_MENUWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetMenuWinMsg),   
    WIN_TITLE(STXT_MAIN_SETTING),   
    WIN_ID( MMIPB_SET_MENUWIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SET_MENU_CTRL_ID),
    END_WIN
};
#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
WINDOW_TABLE( MMIPB_SET_SAVEPOSWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetSavePosWinMsg ),    
    WIN_TITLE( STXT_STORE_MEDIUM),
    WIN_ID( MMIPB_SET_SAVE_POS_WID_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPB_SET_SAVE_POS_LIST_CTRL_ID),
    END_WIN
};
#endif

#ifdef SNS_SUPPORT 
WINDOW_TABLE( MMIPB_SET_ACCOUNTWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetAccountWinMsg ),    
    WIN_TITLE( TXT_ACCOUNT),
    WIN_ID( MMIPB_SET_ACCOUNT_WID_ID ),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPB_SET_ACCOUNT_LIST_CTRL_ID),
    END_WIN
};
#endif


LOCAL const MMIPB_SET_MENU_T s_set_menulist[]={
#ifdef SNS_SUPPORT     
    {TXT_ACCOUNT,            MENU_TYPE_FIXED_SUB_TEXT, TXT_PB_SET_ACCOUNT_SUBSTR,0, 0, 0, MMIPB_OpenSetAccountWin},
#endif
#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
#ifdef MMI_PDA_SUPPORT
    {STXT_STORE_MEDIUM,    MENU_TYPE_ICON,                    0 ,  MMINV_PHONEBOOK_SAVE_POSTION, s_pos_list, sizeof(s_pos_list)/sizeof(s_pos_list[0]) +1 , MMIPB_OpenSetSavePosWin},
#endif
#endif
#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
{TXT_DISPLAY_OPTION, MENU_TYPE_ICON,   TXT_PB_SET_DISPLAY_OPTION_SUBSTR ,  MMINV_PHONEBOOK_DISPLAY_OPTION, s_display_opt_list, sizeof(s_display_opt_list)/sizeof(s_display_opt_list[0]) +1, MMIPB_OpenSetDisplayOptWin},
#endif
#if defined(MMI_SPEEDDIAL_SUPPORT) 
    {TXT_COMMON_SPEEDDIAL,        MENU_TYPE_FIXED_SUB_TEXT,    TXT_PB_SET_SPEEDDIAL_NUM,   0, PNULL, 0, MMIPB_OpenSpeedDialWin},   
#endif
#ifdef MMIPB_SIMDN_SUPPORT
    {TXT_PB_MSISDN,   MENU_TYPE_FIXED_SUB_TEXT,    TXT_PB_SET_LOCAL_NUMBER_SUBSTR , 0,PNULL, 0, MMIPB_OpenMSISDN}, /*lint !e64*/
    {TXT_PB_SDN,     MENU_TYPE_FIXED_SUB_TEXT,    TXT_PB_SDN ,    0,PNULL, 0, MMIPB_OpenSDNWin},/*lint !e64*/
#endif
    {TXT_COMMON_DETAIL_MEMORY,              MENU_TYPE_SIGLE_LINE,            0,                   0, PNULL, 0, MMIPB_OpenMemDetail},
};

LOCAL void AppendPbSetListItem(MMI_CTRL_ID_T       ctrl_id,
                          GUIITEM_STYLE_E     item_style,
                          MMI_IMAGE_ID_T      icon_id,
                          const MMI_STRING_T*  primary_str_ptr,
                          const MMI_STRING_T*   second_str_ptr,
                          uint32               user_data,
                          uint16               pos,
                           BOOLEAN          is_update
)
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64 */
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64 */
    uint16               content_index = 0;
    item_t.item_style    = item_style;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.user_data = user_data;
    item_t.item_data_ptr = &item_data;

    if(GUIITEM_STYLE_ONE_LINE_ICON_TEXT == item_style)
    {
        item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[content_index].item_data.image_id = icon_id;
        content_index++;

        if ( PNULL != primary_str_ptr && 0 < primary_str_ptr->wstr_len && PNULL != primary_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *primary_str_ptr;
            content_index++;
        }
    }
    else if(GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2 == item_style)
    {
        item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[content_index].item_data.image_id = icon_id;
        content_index++;

        if ( PNULL != primary_str_ptr && 0 < primary_str_ptr->wstr_len && PNULL != primary_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *primary_str_ptr;
            content_index++;
        }
        
        if ( PNULL != second_str_ptr && 0 < second_str_ptr->wstr_len && PNULL != second_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *second_str_ptr;
        }
    }
    else
    {
        if ( PNULL != primary_str_ptr && 0 < primary_str_ptr->wstr_len && PNULL != primary_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *primary_str_ptr;
            content_index++;
        }
        if(GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM == item_style)
        {
            if(icon_id != 0)
            {
            
                //need icon
                item_data.item_content[content_index].item_data_type     = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[content_index].item_data.image_id = icon_id;
            }
            content_index++;
        }

        if ( PNULL != second_str_ptr && 0 < second_str_ptr->wstr_len && PNULL != second_str_ptr->wstr_ptr )
        {
            item_data.item_content[content_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer = *second_str_ptr;
        }
    }
    
    if( is_update )
    {
        GUILIST_ReplaceItem(ctrl_id,&item_t,pos);      
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/*****************************************************************************/
//  Description : 进入设置窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSetWin(MMI_HANDLE_T applet_handle)
{
//    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
   // MMIPB_DeleteAllSelNode(applet_ptr->instance.handle); //是否需要
    MMK_CreateWinByApplet(applet_handle,(uint32 *)MMIPB_SET_MENUWIN_TAB, PNULL);
}
#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
/*****************************************************************************/
// 	Description : pb设置新建联系人保存位置窗口处理函数
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetSavePosWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_SAVE_POS_LIST_CTRL_ID);
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 save_position_nv_id = 0;
    uint32 save_position_index = 0;
    uint16 sel_index = 0;
    uint16 item_index = 0;
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:   
        GUILIST_SetMaxItem(ctrl_id, PB_SAVE_POSITION_MAX, FALSE ); 
        save_position_nv_id = ReadPbSet(MMINV_PHONEBOOK_SAVE_POSTION);
        
        for(save_position_index = 0; save_position_index < PB_SAVE_POSITION_MAX; save_position_index++)
        {
            GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
            GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

            if(save_position_index >= PB_SAVE_POSITION_SIM_BEGAIN && save_position_index  <= PB_SAVE_POSITION_SIM_END)
            {
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1 + save_position_index - PB_SAVE_POSITION_SIM_BEGAIN;
                if(!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                {
                    continue;
                }
            }
            
            item_t.item_style  = GUIITEM_STYLE_ONE_LINE_RADIO;            
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = s_pos_list[save_position_index];
            item_t.user_data = save_position_index;
            GUILIST_AppendItem( ctrl_id, &item_t );
            if(save_position_index == save_position_nv_id)
            {
                GUILIST_SetSelectedItem(ctrl_id, item_index, TRUE);
                GUILIST_SetCurItemIndex( ctrl_id, item_index);
            }
            item_index++;
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    
#ifdef MMI_PDA_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#else //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        save_position_nv_id = 0;
        sel_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id,sel_index,&save_position_nv_id);
        WritePbSet(MMINV_PHONEBOOK_SAVE_POSTION, save_position_nv_id);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}
#endif

#ifdef MMIPB_SELECT_STORE_LOCATION_SUPPORT
/*****************************************************************************/
//  Description : 创建pb设置保存位置选择窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenSetSavePosWin(MMI_HANDLE_T applet_handle)
{
    MMK_CreatePubListWinEx(applet_handle,
        (uint32*)MMIPB_SET_SAVEPOSWIN_TAB, PNULL);
}
#endif
#if defined(MMI_SPEEDDIAL_SUPPORT) 
/*****************************************************************************/
//  Description : open speeaddial win
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL void  MMIPB_OpenSpeedDialWin(MMI_HANDLE_T applet_handle)
{
    MMIAPICC_OpenSetSpeedDial(PNULL, 0, 0); 
}
#endif

#ifdef SNS_SUPPORT 
LOCAL void MMIPB_InitSNSGroup(void)    
{
     //初始化sns_group使用信息,以后需要修改
}
LOCAL void  MMIPB_OpenSetAccountWin(MMI_HANDLE_T applet_handle )
{
    MMIPB_InitSNSGroup();
    MMK_CreateWinByApplet(applet_handle,(uint32 *)MMIPB_SET_ACCOUNTWIN_TAB, PNULL);
}
#endif

/*****************************************************************************/
//  Description : 更新pb 设置菜单列表
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL void  UpdatePDAPbSetListCtrl(MMI_CTRL_ID_T ctrl_id, BOOLEAN is_init)
{
    uint32 item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    uint32 idx=0;
    uint32 local_idx = 0;
    uint32 total = sizeof(s_set_menulist)/sizeof(MMIPB_SET_MENU_T);
    MMI_IMAGE_ID_T icon_id=IMAGE_COMMON_RIGHT_ARROW;//Fixed ^ icon
    MMI_STRING_T primary_str = {0};
    MMI_STRING_T second_str = {0};
    
    for(idx = 0; idx < total; idx++)
    {
        if(s_set_menulist[idx].menu_type & MENU_TYPE_SIGLE_LINE)//单行
        {
            item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        }
        else //双行
        {
            if(s_set_menulist[idx].menu_type & MENU_TYPE_ICON)
            {
                item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
                icon_id=IMAGE_COMMON_RIGHT_ARROW;
            }
            else
            {
                item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
                icon_id = 0;
            }            

            if(s_set_menulist[idx].menu_type & MENU_TYPE_FIXED_SUB_TEXT)
            {
                MMI_GetLabelTextByLang(s_set_menulist[idx].sub_fix_id, &second_str);
            }
            else
            {
                local_idx = ReadPbSet(s_set_menulist[idx].nv_id);
                if(local_idx >= s_set_menulist[idx].sub_count)
                {
                    local_idx = 0;
                }
                MMI_GetLabelTextByLang(s_set_menulist[idx].sub_array[local_idx], &second_str);
            }
        }
        MMI_GetLabelTextByLang(s_set_menulist[idx].res_id, &primary_str);
        AppendPbSetListItem(ctrl_id,item_style,icon_id,&primary_str, &second_str, 0, idx, is_init);
    }
}

/*****************************************************************************/
//  Description : pb设置主菜单处理函数
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetMenuWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_MENU_CTRL_ID);
//    uint32 i = 0;
    uint16 cur_index = 0;
    //uint32 user_data = 0;
    uint32 total_num = sizeof(s_set_menulist)/sizeof(MMIPB_SET_MENU_T);
    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {

        return MMI_RESULT_FALSE;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;  

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, total_num, FALSE ); 
        UpdatePDAPbSetListCtrl(ctrl_id, FALSE);
#if defined MMI_ISTYLE_SUPPORT 		
        if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
	{
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
	}
#endif		
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        break;

    case MSG_GET_FOCUS:
        UpdatePDAPbSetListCtrl(ctrl_id, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        if(cur_index < total_num)
        {
            if(s_set_menulist[cur_index].draw_func)
            {
                 s_set_menulist[cur_index].draw_func(applet_handle);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
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

#ifdef SNS_SUPPORT 
LOCAL MMI_TEXT_ID_T GetSnsGroupText(uint8 group_id)
{
    uint16 i = 0;
    for(i = 0; i < MMIPB_SNS_GROUP_NUM; i++)
    {
        if(s_defaut_sns_list[i][MMIPB_SNS_GROUP_ID] == group_id)
        {
            return s_defaut_sns_list[i][MMIPB_SNS_GROUP_TEXT];
        }
    }
    return 0;
}
LOCAL MMI_TEXT_ID_T GetSnsGroupIcon(uint8 group_id)
{
    uint16 i = 0;
    for(i = 0; i < MMIPB_SNS_GROUP_NUM; i++)
    {
        if(s_defaut_sns_list[i][MMIPB_SNS_GROUP_ID] == group_id)
        {
            return s_defaut_sns_list[i][MMIPB_SNS_GROUP_ICON];
        }
    }
    return 0;
}

/*****************************************************************************/
//  Description : 更新pb 账户设置列表
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL void  UpdatePDAPbSetAccountListCtrl(MMI_CTRL_ID_T ctrl_id)
{
    uint32 idx=0;
    uint16 cur_index = 0;
    uint8   sns_group_id = 0;    
    MMI_STRING_T primary_str = {0};
    MMI_STRING_T second_str = {0};
    
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);

    //first remove all list items
    if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
    {
    	GUILIST_RemoveAllItems(ctrl_id);
    }
    
    
    GUILIST_SetMaxItem(ctrl_id, MMIPB_SNS_GROUP_NUM, FALSE ); 

    
    second_str.wstr_len = MMIPB_GetSNSAccountEmailLen();
    second_str.wstr_ptr = SCI_ALLOC_APPZ(second_str.wstr_len);
            
    //获取SNS账户信息
    for(sns_group_id = MMIPB_GROUP_SNS_SINA; sns_group_id < MMIPB_GROUP_SNS_MAX; sns_group_id++)
    {
        //if(s_group_id[sns_group_id] == sns_group_id)
        {
           MMI_GetLabelTextByLang(GetSnsGroupText(sns_group_id), &primary_str);

            SCI_MEMSET(second_str.wstr_ptr,0x00,second_str.wstr_len);
            MMIPB_GetSNSAccountEmail(sns_group_id,&second_str);
             
            AppendPbSetListItem(ctrl_id,GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2,
                                                        GetSnsGroupIcon(sns_group_id),
                                                        &primary_str, &second_str, 
                                                        MMIPB_SNS_FLAG | sns_group_id,  
                                                        idx, FALSE);
            idx++;
        }
     }
    SCI_Free(second_str.wstr_ptr);
    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
}
 
/*****************************************************************************/
//  Description : pb账户设置处理函数
//  Global resource dependence : 
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetAccountWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_ACCOUNT_LIST_CTRL_ID);
   // uint32 i = 0;
    uint16 cur_index = 0;
    uint32 user_data = 0;
//    uint32  total_num = 0;

    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T *  applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {

        return MMI_RESULT_FALSE;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;  
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        UpdatePDAPbSetAccountListCtrl(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        break;

    case MSG_GET_FOCUS:
        UpdatePDAPbSetAccountListCtrl(ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, cur_index, &user_data);
        if(!MMIPB_ISSPLIT(user_data))
        {
            if(PB_NET_CONNECT_INDEX == user_data)
            {
                //网络连接处理函数
            }
            else 
            {
                //sns 账户
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
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
#endif

/*****************************************************************************/
// 	Description : to handle the contact msg log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailMsgSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

                                    
// the window for call log icon main menu
WINDOW_TABLE( MMIPB_DETAILMSG_SELECT_WIN_TAB ) = 
{

    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePBDetailMsgSelectWindow ), 
    WIN_ID( MMIPB_DETAILMSG_SELECT_WIN_ID ),  
    WIN_TITLE(TXT_DELETE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS, MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID, MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIPB_DETAILMSG_SELECT_TITLE_LABEL_CTRL_ID, MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_CHECKBOX_CTRL(TRUE, MMIPB_DETAILMSG_SELECT_TITLE_BUTTON_CTRL_ID, MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE,GUILIST_CHECKLIST_E,MMIPB_DETAILMSG_SELECT_LIST_CTRL_ID,MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID),

    END_WIN
};

/*****************************************************************************/
//  Description : create message
//  Global resource dependence : none
//  Author: gao.ping
//  Note: 
/*****************************************************************************/
MMISMS_DETAIL_MSGLOG_DATA_T        *s_msglogData_ptr=PNULL;

/*****************************************************************************/
//  Description : handle message win message
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void AppendMsglogList(MMI_CTRL_ID_T   ctrl_id)
{
    uint32          i = 0;
    GUILIST_ITEM_T  item = {0};

    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id,s_msglogData_ptr->matched_count,TRUE);
    //set item style
    item.item_style =  GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
    //item.item_state = GUIITEM_STATE_CONTENT_CHECK;//NEWMS00121925
    for (i=0;i<s_msglogData_ptr->matched_count;i++)
    {
            GUILIST_AppendItem( ctrl_id, &item );
    }
    
}

/*****************************************************************************/
//  Description : Some APIS for Contact msg lists win
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
    //PBDetailMsg_GetMaxNum 获取最大的个数
    //PBDetailMsg_SetSelectedbyIndex        //设置当前ITEM对于的选择标记
    //PBDetailMsg_SetAllSelected            //设置All ITEM对于的选择标记
    //PBDetailMsg_DeleteSelection           //删除选择的短信
    //PBDetailMsg_StartSelection            //进行多选删除的初始化
    //PBDetailMsg_ExitSelection             //退出多选删除
    //PBDetailMsg_ExitWin                   //退出联系人的短信列表
    //PBDetailMsg_DeletedCallback           //删除完成之后的回调函数
    //PBDetailMsg_BeginSearch               //启动当前联系人的短信搜索
    //PBDetailMsg_UpdateLogList             //更新短信列表
/*****************************************************************************/
/*****************************************************************************/
//  Description :  获取最大的个数
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL uint16 PBDetailMsg_GetMaxNum(void)
{
   if (s_msglogData_ptr != PNULL)
    return s_msglogData_ptr->matched_count;
   return 0; 
}

/*****************************************************************************/
//  Description : 更新短信列表
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_UpdateLogList(MMI_HANDLE_T list_handle)
{
    if (s_msglogData_ptr == PNULL)
        return;

    if (s_msglogData_ptr->last_matched_count !=s_msglogData_ptr->matched_count)
    {
        //Reset Selected status if search result number changed
        if (s_msglogData_ptr->isSelecting )
        {
            SCI_MEMSET(s_msglogData_ptr->itemSelected,0x00,PBDETAIL_MSG_LOG_MAX_NUM);
            s_msglogData_ptr->last_matched_count = s_msglogData_ptr->matched_count;
        }
    }
    
    AppendMsglogList(list_handle);
}
/*****************************************************************************/
//  Description : //设置当前ITEM对于的选择标记
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_SetSelectedbyIndex(uint16 index)
{
    if (s_msglogData_ptr == PNULL)
        return;

    if (s_msglogData_ptr->itemSelected[index])
    {
        s_msglogData_ptr->itemSelected[index]= FALSE;
    }
    else
    {
        s_msglogData_ptr->itemSelected[index]= TRUE;
    }
}
/*****************************************************************************/
//  Description : 设置All ITEM对于的选择标记
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_SetAllSelected(BOOLEAN isSelected)
{
	uint16 i;
    if (s_msglogData_ptr == PNULL)
        return;
        
    for (i=0;i<PBDETAIL_MSG_LOG_MAX_NUM;i++)
    {
        s_msglogData_ptr->itemSelected[i] = isSelected;
    }
    return;
}
/*****************************************************************************/
//  Description : 删除选择的短信
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PBDetailMsg_DeleteSelection(void)
{
     if (s_msglogData_ptr == PNULL)
        return FALSE;
    return MMISMS_StartDeleteMatchMsgById(s_msglogData_ptr);
}
/*****************************************************************************/
//  Description : 退出多选删除
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_ExitSelection(void)
{
    if (s_msglogData_ptr == PNULL)
        return;
    s_msglogData_ptr->isSelecting = FALSE;
    SCI_MEMSET(s_msglogData_ptr->itemSelected,0x00,PBDETAIL_MSG_LOG_MAX_NUM);
    return;
}
/*****************************************************************************/
//  Description :进行多选删除的初始化
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_StartSelection(void)
{
     if (s_msglogData_ptr == PNULL)
        return;

     s_msglogData_ptr->isSelecting = TRUE;
     SCI_MEMSET(s_msglogData_ptr->itemSelected,0x00,PBDETAIL_MSG_LOG_MAX_NUM);
     return;
}
/*****************************************************************************/
//  Description : //退出联系人的短信列表
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_ExitWin(void)
{
    if (s_msglogData_ptr!=PNULL)
    {
        SCI_FREE(s_msglogData_ptr);
    }
    return;
}
/*****************************************************************************/
//  Description : //删除完成之后的回调函数
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_DeletedCallback(BOOLEAN result)
{
    if (s_msglogData_ptr==PNULL)
    {
        return;
    }
    if (result)
    {
        MMK_CloseWin(s_msglogData_ptr->msg_select_win_handle);
    }
    return;
}
/*****************************************************************************/
//  Description : //启动当前联系人的短信搜索
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void PBDetailMsg_BeginSearch(MMI_HANDLE_T win_handle)
{
    if (s_msglogData_ptr == PNULL)
        return;
    s_msglogData_ptr->msg_select_win_handle = win_handle;
    s_msglogData_ptr->callback = PBDetailMsg_DeletedCallback;
    MMIAPISMS_SearchMatchedItem(s_msglogData_ptr->contact_ptr,s_msglogData_ptr);/*lint !e718*/
    return;
}


/*****************************************************************************/
//	Description : Open contact msg list win API
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMICL_CreateDetailMessageChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMIPB_CONTACT_T * contact_ptr = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(parent_win_id);
    if (s_msglogData_ptr!=PNULL)
    {
        SCI_FREE(s_msglogData_ptr);
    }
    s_msglogData_ptr= SCI_ALLOC_APPZ(sizeof(MMISMS_DETAIL_MSGLOG_DATA_T));
    s_msglogData_ptr->contact_ptr = contact_ptr;
    MMK_CreateChildWin(parent_win_id, (uint32*)MMIPB_DETAILS_MESSAGE_LIST_TAB_WIN_TAB, PNULL);
}

/*****************************************************************************/
//	Description : Open contact msg list select win API
//	Global resource dependence : 
//	Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_EnterDetailMsgListSelectWin(MMI_HANDLE_T app_handle)
{
    MMK_CreateWinByApplet(app_handle,(uint32*)MMIPB_DETAILMSG_SELECT_WIN_TAB,PNULL);
    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void PBDetailMsg_ValidatePrivacyTabSwitch(BOOLEAN is_permitted, DPARAM param_ptr)
{
    if (is_permitted)
    {
        MMIPB_SetCLUnProtect(TRUE);
    }
}
    
/*****************************************************************************/
//  Description : handle message win message
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDetailMessageWinMsg(
                                MMI_WIN_ID_T win_id, 
                                MMI_MESSAGE_ID_E msg_id, 
                                DPARAM param
                                )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T parent_win_id = MMK_GetParentWinHandle(win_id);
    MMI_HANDLE_T    ctrl_handle = MMK_GetCtrlHandleByWin(win_id, MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID);
    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T  *applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;  
   
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            {            
                PBDetailMsg_BeginSearch(win_id);

                PBDetailMsg_UpdateLogList(ctrl_handle);

                MMK_SetAtvCtrl(win_id,MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID);
            }
            break;
        case MSG_FULL_PAINT:
            if ((!MMIPB_GetCLUnProtect())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS)))
            {
                GUILIST_RemoveAllItems(ctrl_handle);
                
            }
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, FALSE);
            if (s_msglogData_ptr->matched_count>0)
            {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_OPTION, FALSE);
            }
            else
            {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
            }
            GUIWIN_SetTitleIsVisible(win_id, TRUE, TRUE);
            
	    break;
            
        case MSG_GET_FOCUS:
                PBDetailMsg_BeginSearch(win_id);
                PBDetailMsg_UpdateLogList(ctrl_handle);
                MMK_SetAtvCtrl(win_id,MMIPB_DETAILS_MESSAGE_LIST_CTRL_ID);
            break;
        case MSG_CTL_LIST_NEED_ITEM_DATA:
            {
                GUILIST_NEED_ITEM_DATA_T    *need_data_ptr = PNULL;
        
                need_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
                need_data_ptr->ctrl_handle = ctrl_handle;
                MMIAPISMS_SetMatchItemContent(need_data_ptr,(uint32)s_msglogData_ptr->order_id_list[need_data_ptr->item_index],FALSE);
            }
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            //Get item data;
			{
                uint16 sel_item_index = 0;

                sel_item_index = GUILIST_GetCurItemIndex(ctrl_handle);

                MMIAPISMS_OpenMatchedItem((uint32)s_msglogData_ptr->order_id_list[sel_item_index]);
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_MENU:
            //MMK_CloseParentWin(parent_win_id);  
            //Enter Delete Select
//            
            if (s_msglogData_ptr->matched_count>0)
            {
                MMIPB_MENU_DATA_INFOR_T add_data = {0};
                //SCI_MEMSET(&add_data,0,sizeof(MMIPB_MENU_DATA_INFOR_T));
                add_data.applet_handle = applet_ptr->caf_applet.app_handle;
                add_data.parent_win =MMK_GetWinHandle(applet_handle, win_id);
                add_data.menu_type = MMIPB_DETAIL_MSGLOG_OPT_MENU;
                add_data.operate_type = MMIPB_OPERATE_TYPE_MAX;
                MMIPB_EnterPbTabOptMenu(&add_data);		
            }
            break;
        case MSG_CTL_TAB_SWITCH:
            //隐私检查
            if (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
            {
                if (!MMIPB_GetCLUnProtect())
                {
                    //需要进行隐私检查
                    MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, PBDetailMsg_ValidatePrivacyTabSwitch, PNULL, 0);
                }
            }    
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            //Set Back 
            MMK_CloseParentWin(parent_win_id);    
            break;

        case MSG_CLOSE_WINDOW:
            PBDetailMsg_ExitWin();
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}   

/*****************************************************************************/
// 	Description : to handle the contact msg log list select item message
//	Global resource dependence : 
//  Author:sam.hua
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePBDetailMsgSelectWindow(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    GUIFORM_CHILD_WIDTH_T  child_width = {0};
    MMI_HANDLE_T list_handle =MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_LIST_CTRL_ID);
    MMI_HANDLE_T btn_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_TITLE_BUTTON_CTRL_ID);
    MMI_HANDLE_T titleform_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_TITLE_FORM_CTRL_ID);
    MMI_HANDLE_T form_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_FORM_CTRL_ID);
    MMI_HANDLE_T label_handle = MMK_GetCtrlHandleByWin(win_id,MMIPB_DETAILMSG_SELECT_TITLE_LABEL_CTRL_ID);
    MMI_HANDLE_T    applet_handle = 0;
    MMIPB_APPLET_T  *applet_ptr = (MMIPB_APPLET_T *)MMK_GetAppletPtrByWin(win_id);

    if(PNULL == applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;  
    

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        
        
        GUILABEL_SetTextById(label_handle,TXT_PB_SELECT_ALL, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, TRUE);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = 40;
        GUIFORM_SetChildWidth(titleform_handle,btn_handle,&child_width);
        GUIFORM_PermitChildBorder(titleform_handle, FALSE);
        GUIBUTTON_PermitBorder(btn_handle, FALSE);

        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_handle,list_handle, &child_height);
        GUIFORM_IsSlide(form_handle, FALSE);
        GUIFORM_PermitChildBg(form_handle,FALSE);
        GUILIST_PermitBorder(list_handle,FALSE);
        
        GUILIST_SetMaxSelectedItem(list_handle,PBDetailMsg_GetMaxNum());
        GUIBUTTON_SetSelect(btn_handle, FALSE);                

        PBDetailMsg_BeginSearch(win_id);
        PBDetailMsg_StartSelection();

        MMK_SetAtvCtrl(win_id, list_handle);
        break;
    case MSG_FULL_PAINT:
  //      if (s_cl_need_update)   //if msg changed ,clear selected status
        {
         //PBDetailMsg_SetAllSelected(FALSE);
  //          s_cl_need_update = FALSE;
        }
        PBDetailMsg_UpdateLogList(list_handle);
        break;
    case MSG_GET_FOCUS:
        PBDetailMsg_BeginSearch(win_id);
        PBDetailMsg_UpdateLogList(list_handle);
        break;
                
    case MSG_CTL_LIST_MARK_ITEM:
        {
            uint32 sel_item_index =0;
            
			sel_item_index = GUILIST_GetCurItemIndex(list_handle);
            
            PBDetailMsg_SetSelectedbyIndex((uint16)sel_item_index);
            if(GUILIST_GetSelectedItemNum(list_handle) <  GUILIST_GetTotalItemNum(list_handle))           
            {
                if (GUIBUTTON_GetSelect(btn_handle))
                {
                    GUIBUTTON_SetSelect(btn_handle, FALSE);
					GUIBUTTON_Update(btn_handle);
                }
            }
            else
            {
                if (!GUIBUTTON_GetSelect(btn_handle) )
                {
                    GUIBUTTON_SetSelect(btn_handle, TRUE);
					GUIBUTTON_Update(btn_handle);
                }
            }
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    if (GUILIST_GetSelectedItemNum(list_handle)<1)
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_ENTRY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    }
                    else
                    {
                       MMIPUB_OpenQueryWinByTextIdEx(applet_handle,TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL,PNULL);                
                    }
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    break;
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_CloseWin(win_id);   
                    break;
                case MMIPB_DETAILMSG_SELECT_TITLE_BUTTON_CTRL_ID:
                    PBDetailMsg_SetAllSelected(GUIBUTTON_GetSelect(btn_handle));
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, 0,0);
                    break;
                default:
                    break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T    *need_data_ptr = PNULL;

            need_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
            need_data_ptr->ctrl_handle = list_handle;
            MMIAPISMS_SetMatchItemContent(need_data_ptr, (uint32)s_msglogData_ptr->order_id_list[need_data_ptr->item_index],TRUE);
            GUILIST_SetSelectedItem(list_handle, need_data_ptr->item_index,s_msglogData_ptr->itemSelected[need_data_ptr->item_index]);
        }
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWinEx(applet_handle, PNULL);
		break;
    case MSG_PROMPTWIN_OK:      //确定退出
        MMIPUB_CloseQuerytWinEx(applet_handle, PNULL);
        PBDetailMsg_DeleteSelection();  
            
		break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //Set Back 
        MMK_CloseWin(win_id);    
        break;
    case MSG_CLOSE_WINDOW:
        PBDetailMsg_ExitSelection();
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
// 	Description : write pb set nv
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  WritePbSet(uint32 item_id, uint32 value)
{
    MMI_WriteNVItemEx( item_id, sizeof(uint32), (void*)(&value) );

    return TRUE;
}

/*****************************************************************************/
// 	Description : read pb set nv
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL uint32  ReadPbSet(uint32 item_id)
{
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_SUCCESS;
    uint32 value = 0;
    
    nv_result = MMI_ReadNVItemEx( item_id, sizeof(uint32), (void*)(&value) );  

    if( MN_RETURN_SUCCESS != nv_result )
    {
        if(MMINV_PHONEBOOK_DISPLAY_OPTION == item_id)
        {
            value = MMIPB_CONTACT_DISPLAY_ALL;//phone and sim
        }
        else if(MMINV_PHONEBOOK_SAVE_POSTION == item_id)
        {
            value = PB_SAVE_POSITION_ALWAYS_QUERY;
        }
        else
        {
            value = 0; //默认list第一个
        }
        WritePbSet(item_id, value);
    }
    return value;
}

/*****************************************************************************/
// 	Description : read new contact save positon
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC PB_SAVE_POSITION_TYPE_E  MMIPB_ReadSavePos(void)
{
   return (PB_SAVE_POSITION_TYPE_E)ReadPbSet(MMINV_PHONEBOOK_SAVE_POSTION); 
}

/*****************************************************************************/
// 	Description : read contact list display option, 按bit位计算，bit0表示phone,bit1表示sim
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_DISPLAY_TYPE_E  MMIPB_ReadDisplayOpt(void)
{
   return ReadPbSet(MMINV_PHONEBOOK_DISPLAY_OPTION); 
}

#ifdef MMIPB_SELECT_DISPLAY_LOCATION_SUPPORT
/*****************************************************************************/
//  Description : handle pb SET win msg
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetDisplayOptWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

WINDOW_TABLE( MMIPB_SET_DISPOPTWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePbSetDisplayOptWinMsg ),    
    WIN_TITLE( TXT_DISPLAY_OPTION),
    WIN_ID( MMIPB_SET_DISPLAY_OPTION_WID_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIPB_SET_DISPLAY_OPTION_LIST_CTRL_ID),
    END_WIN
};

/*****************************************************************************/
// 	Description : pb设置显示选项窗口处理函数
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePbSetDisplayOptWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 display_pos_nv = 0;    
    uint16 display_pos_index = 0;
    uint16 sel_index = 0;
    uint16 item_index = 0;
    
    MMI_CTRL_ID_T ctrl_id =  MMK_GetCtrlHandleByWin(win_id,MMIPB_SET_DISPLAY_OPTION_LIST_CTRL_ID);
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtrByWin(win_id);
    if(PNULL == applet_ptr)
    {
        return recode;
    }
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        GUILIST_SetMaxItem(ctrl_id, MMIPB_DISPLAY_MAX, FALSE ); 
        display_pos_nv = ReadPbSet(MMINV_PHONEBOOK_DISPLAY_OPTION);
        
        for(display_pos_index = 0; display_pos_index < ARR_SIZE(s_display_opt_list); display_pos_index++)
        {
            GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
            GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

            if(display_pos_index >= MMIPB_CONTACT_DISPLAY_SIM1 && display_pos_index  <= MMIPB_DISPLAY_MAX - 1)
            {
                MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1 + display_pos_index - MMIPB_CONTACT_DISPLAY_SIM1;
                if(!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
                {
                    continue;
                }
            }
            
            item_t.item_style  = GUIITEM_STYLE_ONE_LINE_RADIO;            
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = s_display_opt_list[display_pos_index];/*lint !e661*/
            item_t.user_data = display_pos_index;
            GUILIST_AppendItem( ctrl_id, &item_t );
            if(display_pos_nv == display_pos_index)
            {
                GUILIST_SetSelectedItem(ctrl_id, item_index, TRUE);
                GUILIST_SetCurItemIndex(ctrl_id, item_index);
            }
            item_index++;    
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    
#ifdef MMI_PDA_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:  
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif        
        display_pos_nv = 0;
        sel_index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id,sel_index,&display_pos_nv); 
        
        WritePbSet(MMINV_PHONEBOOK_DISPLAY_OPTION, display_pos_nv);
        MMK_SendMsg(applet_ptr->instance.active_win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, PNULL);
        MMK_CloseWin(win_id);
        break;


     
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : 创建pb 设置显示选项选择窗口
//  Global resource dependence : 
//  Author: ping.gao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPB_OpenSetDisplayOptWin(MMI_HANDLE_T applet_handle)
{
    MMK_CreatePubListWinEx(applet_handle,
        (uint32*)MMIPB_SET_DISPOPTWIN_TAB, PNULL);
}
#endif

#if defined (MMIPB_SIMDN_SUPPORT)
/*****************************************************************************/
//  Description : 打开本机号码设置
//  Global resource dependence : 
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_OpenMSISDN(MMI_HANDLE_T applet_handle)
{
    return MMIPB_OpenMSISDNWin(applet_handle, FALSE);
}

#endif

/*Edit by script, ignore 6 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
