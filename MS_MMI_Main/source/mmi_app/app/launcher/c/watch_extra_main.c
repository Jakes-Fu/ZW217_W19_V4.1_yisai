/*****************************************************************************
** File Name:      watch_extra_main.c                                        *
** Author:         xiaotong.su                                               *
** Date:           09/15/2022                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 09/15/2022           xiaotong.su            Create                        *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#ifndef ADULT_WATCH_SUPPORT
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmi_id.h"
#include "mmitheme_list.h"
#include "ctrllist_export.h"
#include "ctrlanim_export.h"
#ifdef APP_STORE_SUPPORT
#include "appstore_common.h"
#include "appstore_home_win_view.h"
#include "appstore_control.h"
#include "appstore_flow.h"
#endif
#ifdef DYNAMIC_WATCHFACE_SUPPORT
#ifdef WATCHFACE_MANAGE_SUPPORT
#include "watchface_set_flow.h"
#include "watchface_edit_win.h"
#include "watch_launcher_editclockwin.h"
#endif
#endif
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
#include "jee.h"
#endif
#include "mmidisplay_color.h"
#include "gui_ucs2b_converter.h"

#ifdef HERO_ENGINE_SUPPORT  
#include "heroEngineExport.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define ICON_FULL_PATH_MAX_LEN (200)
#define WSTR_MAX_LEN (100)
#define MAINMENU_LIST_ITEM_CONTENT_0 (0)
#define MAINMENU_LIST_ITEM_CONTENT_1 (1)

#ifdef APP_STORE_SUPPORT
	LOCAL APPSTORE_APP_LIST_T* s_p_installed_app_list = PNULL;//已安装app list
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/*****************************************************************************/
// Description : js app UMUSIC 入口函数
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void launcher_umusic_app(void);
#endif

#ifdef HERO_ENGINE_SUPPORT
#ifdef HERO_APP_WSTORE_OPEN
//LOCAL void launcher_hero_appstore()
void launcher_hero_appstore()
{
    MMIHEROAPP_AppEntry(HERO_APP_WSTORE);
}
//LOCAL void launcher_hero_appstore_manager()
void launcher_hero_appstore_manager()
{
    MMIHEROAPP_AppEntry(HERO_APP_WSTORE_MANAGE);
}
#endif
#endif

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_umusic      = { IMAGE_MAINMENU_ICON_RES_SETTINGS ,    TXT_MAINMENU_LIST_UMUSIC};
#endif
#ifdef APP_STORE_SUPPORT
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_appstore    = { IMAGE_MAINMENU_ICON_RES_APPSTORE ,    TXT_MAINMENU_LIST_APPSTORE};
#endif
#ifdef DYNAMIC_WATCHFACE_SUPPORT
#ifdef WATCHFACE_MANAGE_SUPPORT
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_watch_set   = { IMAGE_MAINMENU_ICON_RES_WATCH_SET,    TXT_MAINMENU_LIST_WATCH_SET};
#endif
#endif

#ifdef HERO_ENGINE_SUPPORT
#ifdef HERO_APP_WSTORE_OPEN
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_hero_appstore    = { IMAGE_MAINMENU_ICON_RES_APPSTORE ,    TXT_HERO_APP_WSTORE};
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_hero_appstore_manager   = { IMAGE_MAINMENU_ICON_RES_APPSTORE,    TXT_HERO_APP_WSTORE_MANAGE};
#endif
#endif

#if defined SCREEN_SHAPE_SQUARE
LOCAL WATCHCOM_LIST_ITEM__ST s_extra_main_list_data[] =
{
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
    //jee umusic
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_umusic,        launcher_umusic_app },
#endif

#ifdef APP_STORE_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_appstore,      APPSTORE_Enter },
#endif

#ifdef DYNAMIC_WATCHFACE_SUPPORT
#ifdef WATCHFACE_MANAGE_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_watch_set,     WATCHFACE_Set_Enter },
#endif
#endif

#ifdef HERO_ENGINE_SUPPORT
#ifdef HERO_APP_WSTORE_OPEN
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_hero_appstore,     launcher_hero_appstore },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_hero_appstore_manager,     launcher_hero_appstore_manager },
#endif
#endif
};

#elif defined SCREEN_SHAPE_CIRCULAR

LOCAL WATCHCOM_LIST_ITEM__ST s_extra_main_list_data[] =
{
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
    //jee umusic
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1, &list_item_umusic,        launcher_umusic_app },
#endif

#ifdef APP_STORE_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1, &list_item_appstore,      APPSTORE_Enter },
#endif

#ifdef DYNAMIC_WATCHFACE_SUPPORT
#ifdef WATCHFACE_MANAGE_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1, &list_item_watch_set,     WATCHFACE_Set_Enter },
#endif
#endif


#ifdef HERO_ENGINE_SUPPORT
#ifdef HERO_APP_WSTORE_OPEN
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1, &list_item_hero_appstore,     launcher_hero_appstore },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1, &list_item_hero_appstore_manager,     launcher_hero_appstore_manager },
#endif
#endif
};

#endif //SCREEN_SHAPE_SQUARE



/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 创建extra main menu list
// Global resource dependence :
// Author:xiaotong.su
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/

LOCAL BOOLEAN ExtraMenuList_Create( MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// Description : 给list控件传入资源
// Global resource dependence :
// Author:xiaotong.su
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN ExtraMenuListFillData(MMI_CTRL_ID_T ctrl_id, WATCHCOM_LIST_ITEM__ST* p_list_content,GUILIST_NEED_ITEM_CONTENT_T* p_need_item_content);

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/*****************************************************************************/
// Description : js app UMUSIC 入口函数
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void launcher_umusic_app()
{
    jee_start_app("com.unisoc.umusic",NULL);
}
#endif

/*****************************************************************************/
// Description : extra 菜单窗口case OK消息的处理函数
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void Extra_Main_APP_OK( void )
{
    WatchCOM_List_Item_CallBack( MMIEXTRA_MAIN_LIST_CTRL_ID );
}

/*****************************************************************************/
// Description : extra 菜单窗口case CANCEL消息的处理函数
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void Extra_Main_APP_CANCEL( void )
{
    MMK_CloseWin( MMIEXTRA_MAIN_WIN_ID );
}

/*****************************************************************************/
// Description : extra 菜单窗口case FULL_PAINT消息的处理函数
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void Extra_Main_FULL_PAINT( void)
{
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T			list_rect = WATCHLIST_RECT;
    GUILIST_SetRect(MMIEXTRA_MAIN_LIST_CTRL_ID,&list_rect);
#endif
    ExtraMenuList_Create(MMIEXTRA_MAIN_LIST_CTRL_ID);
}

/*****************************************************************************/
// Description : 创建extra main menu list
// Global resource dependence :
// Author:
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/

LOCAL BOOLEAN ExtraMenuList_Create( MMI_CTRL_ID_T ctrl_id)
{
    uint16 item_cnt = 0;  //输入总数目
    uint16 static_list_count = 0;  //静态数目
    uint32 dy_list_count = 0;  //动态数目
    uint16 idx = 0;
    uint16 curSelection   =   0;

    //读取静态菜单数据个数
    static_list_count = sizeof(s_extra_main_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    item_cnt = static_list_count;
    curSelection = GUILIST_GetCurItemIndex(ctrl_id);

#ifdef APP_STORE_SUPPORT
    APPSTORE_FullInstalledAppList(&s_p_installed_app_list);

    if(PNULL == s_p_installed_app_list)
    {
        SCI_TRACE_LOW("[MainMenuList_Create]:s_p_installed_app_list is error");
    }
    else
    {
        dy_list_count = s_p_installed_app_list->app_num;
    }

    if (dy_list_count > 0)//有动态菜单数据
    {
        item_cnt += (uint16)dy_list_count;
    }

#endif

    if (0 == item_cnt)
    {
        SCI_TRACE_LOW("[MainMenuList_Create]:error");
        return FALSE;
    }

    SCI_TRACE_LOW("[MAINMENU_WIN]: item_cnt=%d",item_cnt);
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem( ctrl_id, item_cnt, TRUE);

    for(idx = 0; idx < item_cnt; idx++)
    {
        GUILIST_ITEM_T item_info = {0};
#if defined SCREEN_SHAPE_SQUARE
        item_info.item_style = GUIITEM_STYLE_1ICON_1STR;
#elif defined SCREEN_SHAPE_CIRCULAR
        item_info.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1;
#endif
        GUILIST_AppendItem(ctrl_id, &item_info);
    }

    //不画分割线
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    //不画高亮条
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, curSelection, TRUE);
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,curSelection);
    return TRUE;
}

/*****************************************************************************/
// Description : 给list控件传入资源
// Global resource dependence :
// Author:
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN ExtraMenuListFillData(MMI_CTRL_ID_T ctrl_id, WATCHCOM_LIST_ITEM__ST* p_list_content,GUILIST_NEED_ITEM_CONTENT_T* p_need_item_content)
{
    uint16       static_item_cnt = 0;
    uint32       user_data   = 0;
    uint16       dy_item_index = 0;
    MMI_STRING_T         str1 = {0};
    GUILIST_ITEM_T       item_t    = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
#ifdef APP_STORE_SUPPORT
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
#endif
    if(PNULL == p_list_content)
    {
        SCI_TRACE_LOW("[ExtraMenuListFillData] p_list_content is PNULL");
        return FALSE;
    }
    if(PNULL == p_need_item_content)
    {
        SCI_TRACE_LOW("[ExtraMenuListFillData] p_need_item_content is PNULL");
        return FALSE;
    }
    SCI_TRACE_LOW("[[ExtraMenuListFillData]] item_index :%d",p_need_item_content->item_index);

    static_item_cnt = sizeof(s_extra_main_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    if(  p_need_item_content->item_index < static_item_cnt)
    {
#if defined SCREEN_SHAPE_SQUARE
        item_t.item_style = GUIITEM_STYLE_1ICON_1STR;
#elif defined SCREEN_SHAPE_CIRCULAR
        item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1;
#endif
        item_t.item_data_ptr = &item_data;

        //图片
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = ((WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST*)(p_list_content[p_need_item_content->item_index].data_ptr))->image_id;
        //文本
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id = ((WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST*)(p_list_content[p_need_item_content->item_index].data_ptr))->text_id;

        GUILIST_SetItemData( ctrl_id, &item_data, p_need_item_content->item_index );
        user_data = (uint32)(p_list_content[p_need_item_content->item_index].item_callback);
        GUILIST_SetItemUserData(ctrl_id, p_need_item_content->item_index, &user_data);
    }
#ifdef APP_STORE_SUPPORT
    else
    {
        dy_item_index = p_need_item_content->item_index - static_item_cnt;

        APPSTORE_GetAppNodeFromAppList(dy_item_index, *s_p_installed_app_list, &p_app_node);
        if(PNULL == p_app_node)
        {
            SCI_TRACE_LOW("[ExtraMenuListFillData] p_app_node is PNULL");
            return FALSE;
        }

        if((PNULL != p_app_node->p_app_icon_full_name) && (PNULL != p_app_node->p_app_name))
        {
            wchar icon[ICON_FULL_PATH_MAX_LEN] = {0};
            wchar string[WSTR_MAX_LEN] = {0};
            GUIANIM_FILE_INFO_T  icon_info = {0};
            GUILIST_ITEM_DATA_T  item_data = {0};

            GUI_UTF8ToWstr(icon,SCI_STRLEN(p_app_node->p_app_icon_full_name),p_app_node->p_app_icon_full_name,SCI_STRLEN(p_app_node->p_app_icon_full_name));
            icon_info.full_path_wstr_ptr = icon;
            icon_info.full_path_wstr_len = SCI_STRLEN(p_app_node->p_app_icon_full_name);
            GUI_UTF8ToWstr(string,SCI_STRLEN(p_app_node->p_app_name),p_app_node->p_app_name,SCI_STRLEN(p_app_node->p_app_name));
            str1.wstr_ptr = string;
            str1.wstr_len = SCI_STRLEN(p_app_node->p_app_name);

                item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
                item_data.item_content[0].item_data.anim_path_ptr = &icon_info;

                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                item_data.item_content[1].item_data.text_buffer.wstr_len = str1.wstr_len;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = str1.wstr_ptr;

                GUILIST_SetItemData( MMIEXTRA_MAIN_LIST_CTRL_ID, &item_data, p_need_item_content->item_index );
            }
        }
#endif

    return TRUE;
}

#ifdef APP_STORE_SUPPORT
/*****************************************************************************/
// Description : 给list控件传入图片信息
// Global resource dependence :
// Author:
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN ExtraMenuListFillContent(MMI_CTRL_ID_T ctrl_id, GUILIST_NEED_ITEM_CONTENT_T* p_need_item_content)
{
    uint16 static_item_cnt = 0;
    uint16 dy_item_index = 0;
    APPSTORE_APP_NODE_T* p_app_node = PNULL;

    if(PNULL == p_need_item_content)
    {
        SCI_TRACE_LOW("[ExtraMenuListFillContent] p_need_item_content is PNULL");
        return FALSE;
    }

    static_item_cnt = sizeof(s_extra_main_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    if (p_need_item_content->item_index < static_item_cnt)
    {
        SCI_TRACE_LOW("[ExtraMenuListFillContent] error");
        return FALSE;
    }

    //计算出动态菜单的index
    dy_item_index = p_need_item_content->item_index - static_item_cnt;
    SCI_TRACE_LOW("[ExtraMenuListFillContent] dy_item_index:%d",dy_item_index);

    APPSTORE_GetAppNodeFromAppList(dy_item_index, *s_p_installed_app_list, &p_app_node);

    if (0 == p_need_item_content->item_content_index)   //添加缩略图   //coverity bug1941697
    {
        wchar icon[ICON_FULL_PATH_MAX_LEN] = {0};
        GUILIST_ITEM_DATA_T  item_data = {0};
        GUIANIM_FILE_INFO_T  icon_info = {0};
        GUI_UTF8ToWstr(icon,SCI_STRLEN(p_app_node->p_app_icon_full_name),p_app_node->p_app_icon_full_name,SCI_STRLEN(p_app_node->p_app_icon_full_name));
        icon_info.full_path_wstr_ptr = icon;
        icon_info.full_path_wstr_len = SCI_STRLEN(p_app_node->p_app_icon_full_name);
        //图片数据
        item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
        item_data.item_content[0].item_data.anim_data_ptr = &icon_info;
        GUILIST_SetItemContent(ctrl_id, &item_data, p_need_item_content->item_index, p_need_item_content->item_content_index);
    }

    return TRUE;
}

/*****************************************************************************/
// Description : 处理按下APP后的操作
// Global resource dependence :
// Author:
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN ExtraMenuDynamicItemContentClick( uint16 item_index,uint16 item_content_index )
{
    BOOLEAN ret = FALSE;

    TRACE_APPSTORE("[ExtraMenuDynamicItemContentClick] item_content_index:%d",item_content_index);
    switch(item_content_index)
    {
        case MAINMENU_LIST_ITEM_CONTENT_0:
        case MAINMENU_LIST_ITEM_CONTENT_1:
        {
            uint16 static_item_cnt = 0;
            static_item_cnt = sizeof(s_extra_main_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
            if(item_index >= static_item_cnt)
            {
                APPSTORE_APP_NODE_T* p_app_node = PNULL;
                uint16 dy_item_index = 0;
                dy_item_index = item_index - static_item_cnt;

                APPSTORE_GetAppNodeFromAppList(dy_item_index, *s_p_installed_app_list, &p_app_node);
                if(PNULL != p_app_node)
                {
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
                   jee_start_app(p_app_node->p_app_id,NULL);
#endif
                    ret = TRUE;
                }
                SCI_TRACE_LOW("[ExtraMenuDynamicItemContentClick] get server app node is null");
            }
            break;
        }
        default:
        {
            SCI_TRACE_LOW("[MAINMENU_WIN]:parameter error！item_content_index:%d",item_content_index);
            break;
        }
    }
    return ret;
}

/*****************************************************************************/
// Description : extra 菜单窗口AppStoreExtraMenuCallback
// Global resource dependence :
// Author:
// Note:
/*****************************************************************************/
LOCAL void AppStoreExtraMenuCallback(MSG_APPSTORE_E msg_id)
{
    SCI_TRACE_LOW("[APPSTORE Extramenu callback]: msg_id:%d", msg_id);

    switch(msg_id)
    {
        case MSG_APPSTORE_UPDATE_APP:
        {
            if( MMK_IsFocusWin(MMIEXTRA_MAIN_WIN_ID))
            {
                SCI_TRACE_LOW("[APPSTORE Extramenu callback]: send full pain to Extra menu");
                MMK_SendMsg(MMIEXTRA_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
            break;
        }
        default:
        {
            break;
        }
    }
}
#endif

/*****************************************************************************/
//  Description : to handle Extra window message
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 s_extra_appstore_handle =0;
LOCAL MMI_RESULT_E  HandleExtraMainWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;
    GUILIST_NEED_ITEM_CONTENT_T* p_need_item_content = PNULL;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUILIST_SetSyncDecodeAnim(MMIEXTRA_MAIN_LIST_CTRL_ID,TRUE);
            GUILIST_SetDefaultIconEx(MMIEXTRA_MAIN_LIST_CTRL_ID,NULL,NULL);
            MMK_SetAtvCtrl(win_id,MMIEXTRA_MAIN_LIST_CTRL_ID);
            GUILIST_SetZoomAnim(MMIEXTRA_MAIN_LIST_CTRL_ID,TRUE); // list item ANIM 中icon大小自适应，不做自适应将无法显示

#ifdef APP_STORE_SUPPORT
            if(0 == s_extra_appstore_handle)
            {
                //register appstore control first
                s_extra_appstore_handle = APPSTORE_AddClient(AppStoreExtraMenuCallback);
                if(0 == s_extra_appstore_handle)
                {
                    SCI_TRACE_LOW("[EXTRAMENU_WIN]:add appstore client error");
                }
            }
#endif
            break;
        }
        case MSG_FULL_PAINT:
        {
            //display background
            WATCHCOM_DisplayBackground(win_id);   //bug2095604
            Extra_Main_FULL_PAINT();
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Extra_Main_APP_OK();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
#ifdef APP_STORE_SUPPORT
            APPSTORE_FreeAppList(s_p_installed_app_list);
            s_p_installed_app_list = PNULL;
#endif
            Extra_Main_APP_CANCEL();
            break;
        }
        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            p_need_item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            ExtraMenuListFillData(MMIEXTRA_MAIN_LIST_CTRL_ID,s_extra_main_list_data, p_need_item_content);
            break;
        }
        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
#ifdef APP_STORE_SUPPORT
            p_need_item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            ExtraMenuListFillContent(MMIEXTRA_MAIN_LIST_CTRL_ID, p_need_item_content);
#endif
            break;
        }
        case MSG_CTL_LIST_ITEM_CONTENT_CLICK:
        {
            GUILIST_ITEM_CONTENT_CLICK_T *p_item_content = (GUILIST_ITEM_CONTENT_CLICK_T*)param;
            if(PNULL == p_item_content)
            {
                recode = MMI_RESULT_FALSE;
                SCI_TRACE_LOW("[EXTRAMENU_WIN]:param error");
            }
#ifdef APP_STORE_SUPPORT
            else
            {
                //uint16 static_item_cnt = sizeof(s_extra_main_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
                ExtraMenuDynamicItemContentClick( p_item_content->item_index,p_item_content->item_content_index);
             }
#endif
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
#ifdef APP_STORE_SUPPORT
            if(0 != s_extra_appstore_handle)
            {
                APPSTORE_DeleteClient(s_extra_appstore_handle);
                s_extra_appstore_handle = 0;
            }
#endif
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

WINDOW_TABLE( MMIEXTRA_MAIN_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleExtraMainWindow ),
    WIN_ID( MMIEXTRA_MAIN_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_EXTRA_LAUNCHER),
#endif
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIEXTRA_MAIN_LIST_CTRL_ID ),
#ifndef ADULT_WATCH_SUPPORT
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : Extra window 入口函数
//	Global resource dependence :
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC void WatchEXTRA_MainWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMIEXTRA_MAIN_WIN_TAB, PNULL );
}

#endif
#endif
