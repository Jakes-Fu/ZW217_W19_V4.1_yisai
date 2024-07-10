/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 29/06/2009           Create
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef SEARCH_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmipub.h"
#include "guicommon.h"
#include "guilcd.h"
//#include "guilabel.h"
#include "guires.h"
#include "guiedit.h"
//#include "sci_types.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guitext.h"
#include "mmiidle_export.h"
#include "guirichtext.h"
#include "gui_ucs2b_converter.h"
#include "mmifmm_image.h"
#include "mmifmm_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmipicview_text.h"
#include "mmipb_export.h"
#include "mmi_wallpaper_export.h"
#include "mmimms_export.h"
#include "mmiudisk_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmisearch_export.h"
#include "mmisearch_id.h" 

#include "mmicl_export.h"
#include "mmisms_export.h"
//#include "mmiebook.h"
#include "mmiebook_export.h"

#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmisearch_internal.h"
#if  !defined MMI_PDA_SUPPORT // hermann liu
#include "mmisearch_menutable.h"
#endif
#include "mmifmm_id.h"
#include "guibutton.h"
#include "MMK_tp.h"
#include "mmisearch_image.h"
#ifdef BROWSER_SUPPORT_DORADO
#include "mmibrowser_wintable.h"
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

#define MMISEARCH_SEARCH_TYPE_ICON_ROW_NUM 6
#define MMISEARCH_TEXT_HEIGHT  18
#define MMISEARCH_PB_UPDATE_TIME  400
#define MMISEARCH_FILE_UPDATE_TIME  400
//#define MMISEARCH_UPDATE_DISPLAY_WAIT_TIME  600
#if defined MAINLCD_SIZE_240X320
#if defined MMI_PDA_SUPPORT
#define MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT_H  10
#define MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT  15
#else
#define MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT_H  13
#define MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT  18
#endif
#else
#define MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT_H   8
#define MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT  15
#endif 

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct
{
    uint16 list_index;
    MMISEARCH_TYPE_E search_type;
}MMISEARCH_SPLIT_ITEM_T;

typedef struct
{
    MMI_HANDLE_T win_handle;
    MMI_IMAGE_ID_T box_bg_id;
    GUI_RECT_T box_rect;
    MMI_CTRL_ID_T type_button_ctrl_id[MMISEARCH_TYPE_MAX];
    MMI_IMAGE_ID_T type_button_image_id[MMISEARCH_TYPE_MAX];
    uint32 each_type_march_num[MMISEARCH_TYPE_MAX];
    uint16 type_button_height;
    uint16 type_button_width;
#if !defined(MMI_PDA_SUPPORT)
    MMI_TEXT_ID_T type_button_text_id[MMISEARCH_TYPE_MAX];
#endif
} MMISEARCH_TYPE_OPT_T;


LOCAL MMISEARCH_TYPE_OPT_T *s_search_type_opt_ptr;

LOCAL uint32 s_file_current_matched_num;
LOCAL uint8 s_file_search_update_time_id;
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
LOCAL uint8 s_pb_search_update_time_id;
#endif
LOCAL MMISEARCH_SPLIT_ITEM_T s_split_item_info[MMISEARCH_TYPE_ALL];
LOCAL uint8 s_split_item_num;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle search type option window msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMISEARCH_HandleSearchTypeOptWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param);

/*****************************************************************************/
//  Description : set list item content  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void SetSearchListItemContent(GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr);

/*****************************************************************************/
//  Description : set list item data  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void SetSearchListItemData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr );

/*****************************************************************************/
//  Description : handle list pen ok and open detail  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSearchListControlPenOK(MMI_CTRL_ID_T list_id);

#if  !defined MMI_PDA_SUPPORT // hermann liu
/*****************************************************************************/
//  Description : handle search type option window msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMISEARCH_HandleSearchTypeOptMenuWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param);
#endif
/*****************************************************************************/
//  Description : handle search  window msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMISEARCH_HandleMainSmartSearchWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param);
/*---------------------------------------------------------------------------*/
/*                          WINDOW TABLE DECLARE                           */
/*---------------------------------------------------------------------------*/
/*service management wintable start*/

WINDOW_TABLE(MMISEARCH_MAIN_WIN_TAB) = 
{
    WIN_ID(MMISEARCH_MAIN_WIN_ID),
//	WIN_TITLE(TXT_SEARCH),
    WIN_FUNC( (uint32)MMISEARCH_HandleMainSmartSearchWinMsg),
//	WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISEARCH_KEYWORD_EDITBOX_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS,MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID,MMISEARCH_KEYWORD_EDITBOX_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH_ALL, MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID, MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMISEARCH_MAX_KEYWORD_LENS, MMISEARCH_EDITBOX_ID, MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMISEARCH_WAITING_ANIM_CTRL_ID, MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH, MMISEARCH_KEYWORD_FORM_SEARCH_BUTTON_CTRL_ID, MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISEARCH_LIST_ID, MMISEARCH_KEYWORD_EDITBOX_FORM_CTRL_ID),
 //    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISEARCH_RESOULT_LISTBOX_CTRL_ID),
#if  !defined MMI_PDA_SUPPORT // hermann liu
     WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

WINDOW_TABLE(MMISEARCH_TYPE_OPT_WIN_TAB) = 
{
    WIN_ID(MMISEARCH_TYPE_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
      // WIN_TITLE(TXT_SEARCH),
    WIN_FUNC( (uint32)MMISEARCH_HandleSearchTypeOptWinMsg),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMISEARCH_TYPE_OPT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH_ALL_B, MMISEARCH_TYPE_BUTTON_ALL_CTRL_ID, MMISEARCH_TYPE_OPT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH_APP_B, MMISEARCH_TYPE_BUTTON_APP_CTRL_ID, MMISEARCH_TYPE_OPT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH_CALLLOG_B, MMISEARCH_TYPE_BUTTON_CL_CTRL_ID, MMISEARCH_TYPE_OPT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH_PB_B, MMISEARCH_TYPE_BUTTON_PB_CTRL_ID, MMISEARCH_TYPE_OPT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH_MESSAGE_B, MMISEARCH_TYPE_BUTTON_SMS_CTRL_ID, MMISEARCH_TYPE_OPT_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_SEARCH_FILE_B, MMISEARCH_TYPE_BUTTON_FILE_CTRL_ID, MMISEARCH_TYPE_OPT_FORM_CTRL_ID),
    END_WIN
};

#if  !defined MMI_PDA_SUPPORT // hermann liu

WINDOW_TABLE(MMISEARCH_TYPE_OPT_MENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)MMISEARCH_HandleSearchTypeOptMenuWinMsg),
    WIN_ID(MMISEARCH_TYPE_OPT_MENU_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_SEARCH_TYPE_OPT, MMISEARCH_TYPE_OPT_MENU_CTRL_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
//    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    END_WIN
};

#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
// 	Description : MMISEARCH_OpenMainWin
//	Global resource dependence :
//  Author:        tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_OpenMainWin(void)
{
    if(PNULL == s_search_type_opt_ptr)
    {
        s_search_type_opt_ptr = SCI_ALLOCAZ(sizeof(MMISEARCH_TYPE_OPT_T));
    }
    
    if (PNULL == s_search_type_opt_ptr)
    {
        return FALSE;
    }
    
    if (!MMK_IsOpenWin(MMISEARCH_MAIN_WIN_ID))
    {
        MMK_CreateWin((uint32 *)MMISEARCH_MAIN_WIN_TAB,PNULL);
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:        tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMISEARCH_ResetSplitNum(void)
{
    s_split_item_num = 0;
}

#if 0
/*****************************************************************************/
// 	Description : append match log to list
//	Global resource dependence : none
//  Author: tao.xue
//	Note:
/*****************************************************************************/
LOCAL void AppendMatchLogListItem(void* param);
#endif
/*****************************************************************************/
//  Description : init type option box bg and buttons 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/

LOCAL void SearchTypeOptBoxInit(
                           MMI_WIN_ID_T		win_id,
                           MMISEARCH_TYPE_OPT_T *search_type_opt_ptr
                           )
{
    GUI_RECT_T          disp_rect = {0};
    uint16              box_width = 0;
    uint16              box_height = 0;
    uint16              win_width = 0;
    uint16              win_height = 0;
    uint16              icon_width = 0;
    uint16              icon_height = 0;
    uint16              edit_bg_width = 0;
    uint16              edit_bg_height = 0; 

    MMI_WIN_ID_T   search_win_handle = MMISEARCH_GetSearchWinHandle();
    
    if ( PNULL == search_type_opt_ptr)
    {
        return;
    }
        
    search_type_opt_ptr->win_handle = MMK_ConvertIdToHandle(win_id);
    search_type_opt_ptr->box_bg_id = IMAGE_SEARCH_BOX_BG;

   
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &win_width, &win_height);
    GUIRES_GetImgWidthHeight(&box_width, &box_height, IMAGE_SEARCH_BOX_BG,win_id);
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_SEARCH_ALL_B,MMISEARCH_MAIN_WIN_ID);
    GUIRES_GetImgWidthHeight(&edit_bg_width, &edit_bg_height, IMAGE_SEARCH_BG,MMISEARCH_MAIN_WIN_ID);
    if(MMK_IsIncludeStatusBar(search_win_handle))
    {
        disp_rect.top = MMI_STATUSBAR_HEIGHT + edit_bg_height;
    }
    else
    {
        disp_rect.top = edit_bg_height;
    }

    disp_rect.left = 0;
    disp_rect.right = win_width;
    search_type_opt_ptr->type_button_height = icon_height;
    search_type_opt_ptr->type_button_width = win_width/MMISEARCH_SEARCH_TYPE_ICON_ROW_NUM;
    //一行
    if ( MMISEARCH_TYPE_MAX <= MMISEARCH_SEARCH_TYPE_ICON_ROW_NUM )/*lint !e774 !e506*/
    {
        disp_rect.bottom = disp_rect.top+box_height;
    }
    //两行
    else
    {
        disp_rect.bottom = disp_rect.top+box_height*2;
    }
    
    search_type_opt_ptr->box_rect = disp_rect;   
    
    search_type_opt_ptr->type_button_ctrl_id[MMISEARCH_TYPE_ALL] = MMISEARCH_TYPE_BUTTON_ALL_CTRL_ID;
    search_type_opt_ptr->type_button_ctrl_id[MMISEARCH_TYPE_FILE] = MMISEARCH_TYPE_BUTTON_FILE_CTRL_ID;
    search_type_opt_ptr->type_button_ctrl_id[MMISEARCH_TYPE_PB] = MMISEARCH_TYPE_BUTTON_PB_CTRL_ID;
    search_type_opt_ptr->type_button_ctrl_id[MMISEARCH_TYPE_SMS] = MMISEARCH_TYPE_BUTTON_SMS_CTRL_ID;
    search_type_opt_ptr->type_button_ctrl_id[MMISEARCH_TYPE_APP] = MMISEARCH_TYPE_BUTTON_APP_CTRL_ID;
    search_type_opt_ptr->type_button_ctrl_id[MMISEARCH_TYPE_CL] = MMISEARCH_TYPE_BUTTON_CL_CTRL_ID;

    search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_ALL] = IMAGE_SEARCH_ALL;
    search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_FILE] = IMAGE_SEARCH_FILE;
    search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_PB] = IMAGE_SEARCH_PB;
    search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_SMS] = IMAGE_SEARCH_MESSAGE;
    search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_APP] = IMAGE_SEARCH_APP;
    search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_CL] = IMAGE_SEARCH_CALLLOG;
}

/*****************************************************************************/
//  Description : search type option button call back funtion 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmartSearchTypeButtonCallBack(void)
{
#ifdef MMISEARCH_SUPPORT_ONLYMMS     //if mms only,couldn't changed type
    if (MMISEARCH_isOnlyMMSSearch())
    {
        return MMI_RESULT_FALSE;
    }
#endif    
    if (!MMK_IsOpenWin(MMISEARCH_TYPE_OPT_WIN_ID))
    {
       MMK_CreateWin((uint32 *)MMISEARCH_TYPE_OPT_WIN_TAB,PNULL);
    }
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : change search button image between search and stop
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateSearchButtonStatus(void)
{  
    MMI_CTRL_ID_T search_button_id = MMISEARCH_KEYWORD_FORM_SEARCH_BUTTON_CTRL_ID;   
    GUI_BG_T button_bg = {0};
    GUI_BG_T button_press_bg = {0};
    MMI_CTRL_ID_T waiting_anim_id = MMISEARCH_WAITING_ANIM_CTRL_ID;
    BOOLEAN is_update = MMK_IsFocusWin(MMISEARCH_MAIN_WIN_ID)
                                        ||MMK_IsFocusWin(MMISEARCH_TYPE_OPT_WIN_ID)
#if  !defined MMI_PDA_SUPPORT // hermann liu
                                        ||MMK_IsFocusWin(MMISEARCH_TYPE_OPT_MENU_WIN_ID)
#endif
                                        ;
    
    button_bg.bg_type = GUI_BG_IMG;
    button_press_bg.bg_type = GUI_BG_IMG;
    if(!MMISEARCH_IsSearchEnd())
    {
        button_bg.img_id = IMAGE_SEARCH_STOP;  
        button_press_bg.img_id = IMAGE_SEARCH_STOP_HIGHLIGHT;  
        GUIANIM_SetVisible(waiting_anim_id, TRUE, is_update);
#if  !defined MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(MMISEARCH_MAIN_WIN_ID, STXT_STOP, TXT_NULL,STXT_RETURN, TRUE);
#endif
    }
    else
    {
        button_bg.img_id = IMAGE_SEARCH;
        button_press_bg.img_id = IMAGE_SEARCH_HIGHLIGHT;  
        GUIANIM_SetVisible(waiting_anim_id, FALSE, is_update);
        if (MMISEARCH_GetMatchItemsNum() <= 1 )
        {
            MMISEARCH_InsertPromptNoMatchesItem();
        }
    }

    GUIBUTTON_SetBg (search_button_id,  &button_bg);
    GUIBUTTON_SetPressedBg(search_button_id, &button_press_bg);
    
    if (is_update)
    {
        MMK_SendMsg(search_button_id,MSG_CTL_PAINT,0);
    }
}
/*****************************************************************************/
//  Description : handle search type option window msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T  GetTypeTextId(MMISEARCH_TYPE_E type)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    
#ifdef MMISEARCH_SUPPORT_ONLYMMS
    type = MMISEARCH_TYPE_SMS;
#endif

    switch (type)
    {
        case MMISEARCH_TYPE_ALL:
            text_id = TXT_COMMON_ALL;
            break;
            
        case MMISEARCH_TYPE_PB:
            text_id = STXT_MAIN_PB;
            break;
            
        case MMISEARCH_TYPE_CL:
            text_id = STXT_MAIN_CL;
            break;
            
        case MMISEARCH_TYPE_SMS:
            text_id = STXT_MAIN_MESSAGE;
            break;
            
        case MMISEARCH_TYPE_APP:
            text_id = TXT_APPLICATION;
            break;
            
        case MMISEARCH_TYPE_FILE:
            text_id = TXT_FILE;
            break;

        case MMISEARCH_TYPE_WEB:
            text_id = TXT_NETWORK_SEARCHING;
            break;

        default:
            text_id = TXT_BLANK;
            break;
    }

    return text_id;
}
/*****************************************************************************/
//  Description : handle search type option window msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMISEARCH_HandleSearchTypeOptWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param)
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  editbox_id = MMISEARCH_EDITBOX_ID;
    GUI_BG_T bg = {0};
    MMI_CTRL_ID_T form_id = MMISEARCH_TYPE_OPT_FORM_CTRL_ID;
    uint16              box_width = 0;
    uint16              box_height = 0;
    GUI_RECT_T          disp_rect = {0};   
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            int32 i = 0;
            if(PNULL == s_search_type_opt_ptr)
            {
                MMK_CloseWin(win_id);
            }
            // 去掉退出窗口的特效
            MMK_SetWinMoveStyle(MMK_ConvertIdToHandle(win_id),MOVE_FORBIDDEN);
            
            SearchTypeOptBoxInit(win_id, s_search_type_opt_ptr);

            GUIFORM_PermitChildBorder(form_id, FALSE);   
            GUIFORM_IsSlide(form_id, FALSE);

            for(i = 0; i < MMISEARCH_TYPE_MAX; i ++)
            {
                GUIBUTTON_PermitBorder(s_search_type_opt_ptr->type_button_ctrl_id[i] , FALSE);
            }
            
            GUIFORM_SetRect(form_id, &(s_search_type_opt_ptr->box_rect));
            SCI_MEMSET(&bg, 0x00, sizeof(bg));
            bg.bg_type = GUI_BG_IMG;
            bg.img_id = s_search_type_opt_ptr->box_bg_id;
            GUIFORM_SetBg( form_id, &bg);
            
            GUIFORM_SetAlign(form_id,GUIFORM_CHILD_ALIGN_LEFT);

        }
        break;

        case MSG_LCD_SWITCH:
        {
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &box_width, &box_height);
            disp_rect = s_search_type_opt_ptr->box_rect;
            disp_rect.right = box_width;
            GUIFORM_SetRect(form_id, &disp_rect);
          
        }
        break;
//    case MSG_CTL_OK:

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_NOTIFY_T notify = {0};
            GUI_BG_T button_bg = {0};
        
            button_bg.bg_type = GUI_BG_IMG;
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));

            if(PNULL == s_search_type_opt_ptr)
            {
                break;
            }
            
            if(MMISEARCH_TYPE_BUTTON_ALL_CTRL_ID == notify.src_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_ALL];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_ALL);
                GUIEDIT_SetDefaultStringById(editbox_id, TXT_COMMON_ALL);
            }
            else if(MMISEARCH_TYPE_BUTTON_PB_CTRL_ID == notify.src_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_PB];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_PB);
                GUIEDIT_SetDefaultStringById(editbox_id, STXT_MAIN_PB);
            }
            else if(MMISEARCH_TYPE_BUTTON_CL_CTRL_ID == notify.src_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_CL];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_CL);
                GUIEDIT_SetDefaultStringById(editbox_id, STXT_MAIN_CL);
            }
            else if(MMISEARCH_TYPE_BUTTON_SMS_CTRL_ID == notify.src_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_SMS];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_SMS);
                GUIEDIT_SetDefaultStringById(editbox_id, STXT_MAIN_MESSAGE);
            }
            else if(MMISEARCH_TYPE_BUTTON_APP_CTRL_ID == notify.src_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_APP];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_APP);
                GUIEDIT_SetDefaultStringById(editbox_id, TXT_APPLICATION);
            }
            else if(MMISEARCH_TYPE_BUTTON_FILE_CTRL_ID == notify.src_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_FILE];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_FILE);
                GUIEDIT_SetDefaultStringById(editbox_id, TXT_FILE);
            }
            else
            {
                break;
            }
            
            GUIBUTTON_SetBg (MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID,  &button_bg);
            GUIBUTTON_SetPressedBg(MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID, &button_bg);
            GUIBUTTON_Update(MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID);
            MMK_CloseWin(win_id);
        }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            MMI_TP_STATUS_E state = MMI_TP_NONE;
            MMK_GetLogicTPMsg(&state, &point);
            
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &box_width, &box_height);
            disp_rect = s_search_type_opt_ptr->box_rect;
            disp_rect.right = box_width;         
            
            if(!GUI_PointIsInRect(point, disp_rect))
            {
                MMK_CloseWin(win_id);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
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
#if  !defined MMI_PDA_SUPPORT // hermann liu
/*****************************************************************************/
//  Description : handle search type option window msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMISEARCH_HandleSearchTypeOptMenuWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param)
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  editbox_id = MMISEARCH_EDITBOX_ID;
    MMI_CTRL_ID_T   menu_ctl_id = MMISEARCH_TYPE_OPT_MENU_CTRL_ID;
    uint16 cur_index = 0;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMISEARCH_TYPE_E type = MMISEARCH_GetSearchType();

            // 去掉退出窗口的特效
          //  MMK_SetWinMoveStyle(MMK_ConvertIdToHandle(win_id),MOVE_FORBIDDEN);
            switch(type)
            {
                case MMISEARCH_TYPE_ALL:
                    pop_item_info.menu_id = ID_SEARCH_TYPE_ALL;
                    cur_index = 0;
                    break;
                case MMISEARCH_TYPE_APP:
                    pop_item_info.menu_id = ID_SEARCH_TYPE_APP;
                    cur_index = (uint32)ID_SEARCH_TYPE_APP - (uint32)ID_SEARCH_TYPE_ALL;
                    break;
                case MMISEARCH_TYPE_FILE:
                    pop_item_info.menu_id = ID_SEARCH_TYPE_FILE;
                    cur_index = (uint32)ID_SEARCH_TYPE_FILE - (uint32)ID_SEARCH_TYPE_ALL;
                    break;
                case MMISEARCH_TYPE_PB:
                    pop_item_info.menu_id = ID_SEARCH_TYPE_PB;
                    cur_index = (uint32)ID_SEARCH_TYPE_PB - (uint32)ID_SEARCH_TYPE_ALL;
                    break;
                case MMISEARCH_TYPE_SMS:
                    pop_item_info.menu_id = ID_SEARCH_TYPE_MSG;
                    cur_index = (uint32)ID_SEARCH_TYPE_MSG - (uint32)ID_SEARCH_TYPE_ALL;
                    break;
                case MMISEARCH_TYPE_CL:
                    pop_item_info.menu_id = ID_SEARCH_TYPE_CL;
                    cur_index = (uint32)ID_SEARCH_TYPE_CL - (uint32)ID_SEARCH_TYPE_ALL;
                    break;
                default:
                    break;
            }
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = menu_ctl_id;
            pop_item_info.group_id = MENU_SEARCH_TYPE_OPT;

            GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
            GUIMENU_SetSelectedItem(menu_ctl_id, cur_index);
            MMK_SetAtvCtrl( win_id,  menu_ctl_id);
        }
        break;


//    case MSG_CTL_OK:

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:    
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            GUI_BG_T button_bg = {0};
            MMI_MENU_GROUP_ID_T         group_id = 0;
            MMI_MENU_ID_T               menu_id = 0;            

            GUIMENU_GetId(menu_ctl_id,&group_id,&menu_id);        
            button_bg.bg_type = GUI_BG_IMG;

            if(PNULL == s_search_type_opt_ptr)
            {
                break;
            }
            
            if(ID_SEARCH_TYPE_ALL == menu_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_ALL];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_ALL);
                GUIEDIT_SetDefaultStringById(editbox_id, TXT_COMMON_ALL);
            }
            else if(ID_SEARCH_TYPE_PB == menu_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_PB];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_PB);
                GUIEDIT_SetDefaultStringById(editbox_id, STXT_MAIN_PB);
            }
            else if(ID_SEARCH_TYPE_CL == menu_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_CL];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_CL);
                GUIEDIT_SetDefaultStringById(editbox_id, STXT_MAIN_CL);
            }
            else if(ID_SEARCH_TYPE_MSG == menu_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_SMS];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_SMS);
                GUIEDIT_SetDefaultStringById(editbox_id, STXT_MAIN_MESSAGE);
            }
            else if(ID_SEARCH_TYPE_APP == menu_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_APP];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_APP);
                GUIEDIT_SetDefaultStringById(editbox_id, TXT_APPLICATION);
            }
            else if(ID_SEARCH_TYPE_FILE == menu_id )
            {
                button_bg.img_id =s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_FILE];
                MMISEARCH_SetSearchType(MMISEARCH_TYPE_FILE);
                GUIEDIT_SetDefaultStringById(editbox_id, TXT_FILE);
            }
            else
            {
                break;
            }
            
            {
                MMISEARCH_TYPE_E type = MMISEARCH_GetSearchType();
                GUIEDIT_SetSoftkey(editbox_id,0,1,s_search_type_opt_ptr->type_button_text_id[type] ,s_search_type_opt_ptr->type_button_text_id[type] ,PNULL);
            }
            
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = menu_ctl_id;
            pop_item_info.menu_id = menu_id;
            pop_item_info.group_id = MENU_SEARCH_TYPE_OPT;

            GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
            MMK_SendMsg(menu_ctl_id,MSG_CTL_PAINT,PNULL);
            
            GUIBUTTON_SetBg (MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID,  &button_bg);
            GUIBUTTON_SetPressedBg(MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID, &button_bg);
            GUIBUTTON_Update(MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID);
            MMK_CloseWin(win_id);
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
#endif
/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_ClearCurrFileMatchNum(void)
{
    s_file_current_matched_num = 0;
}
/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateFileSearching(void)
{
    MMISEARCH_DATA_T match_data = {0};
    uint32 i = 0;
    uint32 new_match_num = MMIAPIFMM_GetFileSearchMatchedNum();

    if (new_match_num >= s_file_current_matched_num)
    {
        match_data.match_num= new_match_num - s_file_current_matched_num;
    }
    else
    {
        SCI_TRACE_LOW("warning! UpdateFileSearching new_match_num < s_file_current_matched_num");
        return ;
    }
    match_data.match_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(match_data.match_num*sizeof(MMISEARCH_MATCH_ITEM_T));

    if (PNULL == match_data.match_item_ptr)
    {
        return ;
    }    

    for( i = 0; i < match_data.match_num; i++ )
    {
        match_data.match_item_ptr[i].match_item_type = MMISEARCH_TYPE_FILE;
        match_data.match_item_ptr[i].match_item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2;
        match_data.match_item_ptr[i].user_data = i + s_file_current_matched_num;
        match_data.match_item_ptr[i].open_pfunc = MMIAPIFMM_OpenMatchedItem;
        match_data.match_item_ptr[i].set_data_pfunc = MMIAPIFMM_SetMatchItemData;
    }
   
    if (match_data.match_num != 0 && match_data.match_item_ptr != PNULL)
    {
        if(!MMIAPISEARCH_SaveMatchItems(match_data))
        {
            MMISEARCH_StopAsyncSearch();
            s_file_current_matched_num = 0;
            SCI_FREE(match_data.match_item_ptr);
            return;
        }
    }    
    if (PNULL != match_data.match_item_ptr)
    {
        SCI_FREE(match_data.match_item_ptr);
    }  
    s_file_current_matched_num = new_match_num;
}

/*****************************************************************************/
//  Description : show notification item
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void SetUsbNotificationItem(
                            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr,
                            uint32 user_data
                            )
{
    GUILIST_ITEM_DATA_T item_data = {0};

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = (MMI_TEXT_ID_T)user_data;    
    
    item_data.softkey_id[LEFT_BUTTON]      = TXT_NULL;
    item_data.softkey_id[MIDDLE_BUTTON]    = TXT_NULL;
    item_data.softkey_id[RIGHT_BUTTON]     = STXT_RETURN;
	
    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
    return;

}
/*****************************************************************************/
//  Description : use notification item
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISearch_AddNotificationItemToFile(MMI_TEXT_ID_T text_id)
{
    MMISEARCH_DATA_T match_data = {0};
    match_data.match_num= 1;
    match_data.match_item_ptr = (MMISEARCH_MATCH_ITEM_T*)SCI_ALLOCAZ(sizeof(MMISEARCH_MATCH_ITEM_T));
    if(PNULL != match_data.match_item_ptr)
    {
        match_data.match_item_ptr->match_item_type = MMISEARCH_TYPE_FILE;
        match_data.match_item_ptr->match_item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        match_data.match_item_ptr->user_data = text_id;
        match_data.match_item_ptr->open_pfunc = PNULL;
        match_data.match_item_ptr->set_data_pfunc = SetUsbNotificationItem;
        
        MMIAPISEARCH_SaveMatchItems(match_data);
    }
    
    if (PNULL != match_data.match_item_ptr)
    {
        SCI_FREE(match_data.match_item_ptr);
    } 
}

/*****************************************************************************/
//  Description : to stop update timer of search window for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_IsAsyncUpdataTimerStopped(void)
{
    return (s_file_search_update_time_id == 0 
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    && s_pb_search_update_time_id == 0 
#endif
    );
}

/*****************************************************************************/
//  Description : to stop update timer of search window for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_StopAsyncUpdataTimer(void)
{
    MMK_StopTimer(s_file_search_update_time_id);
    s_file_search_update_time_id = 0;
}
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
/*****************************************************************************/
//  Description : to stop update timer of search window for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_StopPbAsyncUpdataTimer(void)
{
    MMK_StopTimer(s_pb_search_update_time_id);
    s_pb_search_update_time_id = 0;
}
#endif
/*****************************************************************************/
//  Description : set data of split item
 //  Global resource dependence : 
 //  Author: tao.xue
 //  Note: 
/*****************************************************************************/
PUBLIC void SetSplitData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr)
{
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16 index = 0;
    MMISEARCH_TYPE_E search_type = MMISEARCH_TYPE_ALL;
    
    for (; index < s_split_item_num; index++)
    {
        if (need_item_data_ptr->item_index == s_split_item_info[index].list_index)
        {
            search_type = s_split_item_info[index].search_type;
            break;
        }
    }
 
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = GetTypeTextId(search_type);
    
    GUILIST_SetItemStyle(need_item_data_ptr->ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT, need_item_data_ptr->item_index);
    GUILIST_SetItemData(need_item_data_ptr->ctrl_id, &item_data, need_item_data_ptr->item_index);
}

/*****************************************************************************/
//  Description : set list item data  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL uint16 DecSplitNum(uint16 list_index)
{
    uint16 pure_list_index = list_index;
    uint16 index = 0;

    if (list_index < s_split_item_info[0].list_index)
    {
        return pure_list_index;
    }
    else if (list_index > s_split_item_info[s_split_item_num - 1].list_index)
    {
        return pure_list_index - s_split_item_num;
    }
    else
    {
        for (; index < s_split_item_num; index++)
        {
            if (list_index < s_split_item_info[index].list_index)
            {
                return pure_list_index - index;
            }
        }
    }

    return pure_list_index;
}
/*****************************************************************************/
//  Description : set list item data  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSplitItem(uint16 list_index)
{
    uint16 index = 0;

    for (; index < s_split_item_num; index++)
    {
        if (list_index == s_split_item_info[index].list_index)
        {
            return TRUE;
        }
    }

    return FALSE;
}
/*****************************************************************************/
//  Description : set list item content  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void SetSearchListItemContent(GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr)
{
    MMISEARCH_MATCH_ITEM_T *match_item_ptr = PNULL;
    uint16 pure_list_index = 0;
    
    if(PNULL == need_item_content_ptr)
    {
        return;
    }

    if (IsSplitItem(need_item_content_ptr->item_index))
    {
        return ;
    }
    
    pure_list_index = DecSplitNum(need_item_content_ptr->item_index);

    match_item_ptr = MMISEARCH_GetOneMatchItemPtr(pure_list_index);

    if (PNULL != match_item_ptr && PNULL != match_item_ptr->set_data_pfunc)
    {
        match_item_ptr->set_content_pfunc(need_item_content_ptr, match_item_ptr->user_data);
    }
}
/*****************************************************************************/
//  Description : set list item data  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void SetSearchListItemData(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr )
{
    MMISEARCH_MATCH_ITEM_T *match_item_ptr = PNULL;
    uint16 pure_list_index = 0;

    if(PNULL == need_item_data_ptr)
    {
        return;
    }

    if (IsSplitItem(need_item_data_ptr->item_index))
    {
        SetSplitData(need_item_data_ptr);
        return ;
    }
    
    pure_list_index = DecSplitNum(need_item_data_ptr->item_index);
    
    match_item_ptr = MMISEARCH_GetOneMatchItemPtr(pure_list_index);
    
    if (PNULL != match_item_ptr && PNULL != match_item_ptr->set_data_pfunc)
    {
        match_item_ptr->set_data_pfunc(need_item_data_ptr, match_item_ptr->user_data);
    }
}

/*****************************************************************************/
//  Description : handle list pen ok and open detail  
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSearchListControlPenOK(MMI_CTRL_ID_T list_id)
{
    uint16 sel_item_index =0;
    MMISEARCH_MATCH_ITEM_T *match_item_ptr = PNULL;
    uint16 pure_list_index = 0;

    sel_item_index = GUILIST_GetCurItemIndex(list_id);

    SCI_TRACE_LOW("HandleSearchListControlPenOK index = %d", sel_item_index);

    if (IsSplitItem(sel_item_index))
    {
        return ;
    }
    
    pure_list_index = DecSplitNum(sel_item_index);

    match_item_ptr = MMISEARCH_GetOneMatchItemPtr(pure_list_index);
    
    SCI_TRACE_LOW("HandleSearchListControlPenOK match_item_ptr = %x", match_item_ptr);

    if (PNULL != match_item_ptr && PNULL != match_item_ptr->open_pfunc)
    {
        SCI_TRACE_LOW("HandleSearchListControlPenOK open_pfunc = %x", match_item_ptr->open_pfunc);

        if (MMIAPISEARCH_IsAsyncSearching())
        {
            SCI_TRACE_LOW("HandleSearchListControlPenOK MMISEARCH_StopAsyncSearch");
            MMISEARCH_SetSearchStoppedCallBack(match_item_ptr->open_pfunc, match_item_ptr->user_data);
            MMISEARCH_StopAsyncSearch();
        } 
        else
        {
            match_item_ptr->open_pfunc(match_item_ptr->user_data);
        }
    }
}

/*****************************************************************************/
//  Description : handle search contrl msg 
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: search control window to separate view and mesaage flow
/*****************************************************************************/
PUBLIC MMI_RESULT_E  MMIAPISEARCH_HandleControlMsg(
                                               MMI_WIN_ID_T		win_id, 	
                                               MMI_MESSAGE_ID_E	msg_id,
                                               DPARAM				param
                                               )
{
    return MMISEARCH_HandleMainSmartSearchWinMsg(win_id,msg_id,param);
}

/*****************************************************************************/
//  Description : handle search contrl msg 
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: search control window to separate view and mesaage flow
/*****************************************************************************/
LOCAL void HandleSearchButtonPenOk(void)
{
    MMI_CTRL_ID_T  editbox_id = MMISEARCH_EDITBOX_ID;
    MMI_CTRL_ID_T list_id = MMISEARCH_LIST_ID;
    MMI_STRING_T   keyword_str = {0};

    MMI_CTRL_ID_T   search_list_handle = MMK_GetCtrlHandleByWin(MMISEARCH_GetSearchWinHandle(), list_id);
    
    if(MMIAPISEARCH_IsAsyncSearching())
    {
        MMISEARCH_StopAsyncSearch();
    }
    else
    {
    // get string info
        GUIEDIT_GetString(editbox_id, &keyword_str);
        GUILIST_RemoveAllItems(search_list_handle);
        
        if(keyword_str.wstr_len == 0)
        { 
            MMK_PostMsg(list_id,MSG_CTL_PAINT,PNULL, 0); 
        }
        else
        {
            MMISEARCH_EntrySearching(&keyword_str);
        }
    }
}

/*****************************************************************************/
//  Description : Get Edit Box String
//  Global resource dependence : 
//  Author: hermann liu
//  Note: search control window to separate view and mesaage flow
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_GetEditBoxString(MMI_STRING_T   *str_ptr )
{
    MMI_WIN_ID_T   search_win_handle = MMISEARCH_GetSearchWinHandle();
    MMI_CTRL_ID_T   search_editbox_handle = MMK_GetCtrlHandleByWin(search_win_handle, MMISEARCH_EDITBOX_ID);
    
    if(MMK_IsOpenWin(search_win_handle))
    {
        GUIEDIT_GetString(search_editbox_handle, str_ptr);
    }
}

/*****************************************************************************/
//  Description :  Is Enter From BrwSearch Item
//  Global resource dependence : 
//  Author: hermann liu
//  Note: search control window to separate view and mesaage flow
/*****************************************************************************/
/*PUBLIC BOOLEAN MMISEARCH_IsEnterFromBrwSearchItem(void)
{
    MMI_CTRL_ID_T list_id = MMISEARCH_LIST_ID;
    uint16 index = GUILIST_GetCurItemIndex(list_id);

    if(0 == index && MMK_IsOpenWin(MMISEARCH_MAIN_WIN_ID))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}
*/
/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMISEARCH_HandleMainSmartSearchWinMsg(
                                            MMI_WIN_ID_T		win_id, 	
                                            MMI_MESSAGE_ID_E	msg_id,
                                            DPARAM				param)
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  editbox_id = MMISEARCH_EDITBOX_ID;
    MMI_CTRL_ID_T list_id = MMISEARCH_LIST_ID;
    MMI_CTRL_ID_T win_form_id = MMISEARCH_KEYWORD_EDITBOX_FORM_CTRL_ID;
    MMI_CTRL_ID_T type_button_id = MMISEARCH_KEYWORD_FORM_TYPE_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T search_button_id = MMISEARCH_KEYWORD_FORM_SEARCH_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T waiting_anim_id = MMISEARCH_WAITING_ANIM_CTRL_ID;
    
    MMI_STRING_T   keyword_str = {0};
    GUIFORM_CHILD_HEIGHT_T      list_height = {0};
#if defined MAINLCD_SIZE_320X480
    GUIFORM_CHILD_HEIGHT_T      child_height = {0};
#endif
    GUIFORM_CHILD_WIDTH_T		searchbutton_width ={0};
    GUIFORM_CHILD_HEIGHT_T		searchbutton_height ={0};
    GUIFORM_CHILD_WIDTH_T               anim_width = {0};
    GUI_BG_T bg = {0};
    wchar                               forbid_char_value[]= {0x0d, 0x0a};//禁止换行回车的输入
    uint32 i = 0;

    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};

    MMI_WIN_ID_T   search_win_handle = MMISEARCH_GetSearchWinHandle();
    MMI_CTRL_ID_T   search_list_handle = MMK_GetCtrlHandleByWin(search_win_handle, list_id);
    
    SCI_TRACE_LOW("MMISEARCH_HandleMainSmartSearchWinMsg msg_id = %d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BG_T button_bg = {0};
            GUI_BG_T button_p_bg = {0};
#if defined MAINLCD_SIZE_320X480
            uint16 edit_bg_width = 0;
            uint16 edit_bg_height = 0;
#endif
            uint16 pic_width = 0;
            uint16 pic_height = 0;
            uint16 hor_space = 0;

            GUIFORM_SetMargin(win_form_id, 0);
 
            GUIFORM_PermitChildBorder(win_form_id, FALSE);   
            GUIFORM_PermitChildBg(win_form_id, FALSE);
            GUIFORM_IsSlide(win_form_id, FALSE);
            hor_space = 0;
            GUIFORM_SetSpace(MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, &hor_space, PNULL);

            SCI_MEMSET(&bg, 0x00, sizeof(bg));
            bg.bg_type = GUI_BG_IMG;
            bg.img_id = IMAGE_SEARCH_BG;
            GUIFORM_SetBg( MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, &bg);
#if defined MAINLCD_SIZE_320X480
            GUIRES_GetImgWidthHeight(&edit_bg_width, &edit_bg_height,IMAGE_SEARCH_BG, win_id);
            child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            child_height.add_data = edit_bg_height;
            GUIFORM_SetChildHeight(win_form_id,MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, &child_height);
#endif
            searchbutton_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
             if(MMK_IsWindowLandscape(win_id))
            {
                searchbutton_width.add_data = MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT_H;
            }
             else
            {
                searchbutton_width.add_data = MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT;
            }
            GUIFORM_SetChildWidth(MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, type_button_id, &searchbutton_width);

            searchbutton_height.type = GUIFORM_CHILD_HEIGHT_PERCENT;
            searchbutton_height.add_data = 100;
            GUIFORM_SetChildHeight(MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, type_button_id, &searchbutton_height);

            GUIRES_GetImgWidthHeight(&pic_width, &pic_height,IMAGE_SEARCH_ANIM_GIF, win_id);
            anim_width.type = GUIFORM_CHILD_WIDTH_FIXED;//GUIFORM_CHILD_WIDTH_PERCENT;
            anim_width.add_data = pic_width;//10;
            GUIFORM_SetChildWidth(MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, waiting_anim_id,&anim_width);

            GUIRES_GetImgWidthHeight(&pic_width, &pic_height,IMAGE_SEARCH, win_id);
            searchbutton_width.type = GUIFORM_CHILD_WIDTH_FIXED;//GUIFORM_CHILD_WIDTH_PERCENT;
            searchbutton_width.add_data = pic_width;//10;
            GUIFORM_SetChildWidth(MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, search_button_id, &searchbutton_width);


            GUIEDIT_SetMaxLen(editbox_id, MMISEARCH_MAX_KEYWORD_LENS);
            GUIEDIT_ConfigDisableChar(editbox_id, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            GUIEDIT_SetStyle(editbox_id,GUIEDIT_STYLE_SINGLE);
#if defined MAINLCD_SIZE_320X480
			//background of edit is an image(IMAGE_SEARCH_BG), set margin to avoid cursor display outside
			//the edit rect(actually the edit rect of image)
			GUIEDIT_SetMargin(editbox_id,10, 5);
#endif

            list_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUIFORM_SetChildHeight(win_form_id,list_id,&list_height);
            GUILIST_PermitBorder(list_id, FALSE);
            GUILIST_SetUserBg(list_id, FALSE);

            GUIBUTTON_PermitBorder(type_button_id, FALSE);
            GUIBUTTON_SetRunSheen(search_button_id, FALSE);
            GUIBUTTON_PermitBorder(search_button_id, FALSE);
            button_bg.bg_type = GUI_BG_IMG;
            button_p_bg.bg_type = GUI_BG_IMG;
            button_p_bg.img_id = IMAGE_SEARCH_HIGHLIGHT;
            GUIBUTTON_SetPressedBg(search_button_id, &button_p_bg);
            
            ctrl_info.is_ctrl_id     = TRUE;
            ctrl_info.ctrl_id        = waiting_anim_id;
            ctrl_info.ctrl_ptr = PNULL;
            data_info.img_id         = IMAGE_SEARCH_ANIM_GIF;
            display_info.is_update   = TRUE;
            display_info.is_disp_one_frame = FALSE;
            display_info.align_style = GUIANIM_ALIGN_RIGHT_VMIDDLE;
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
            GUIANIM_SetVisible(waiting_anim_id, FALSE, TRUE);
            
            GUIEDIT_SetDefaultStringById(editbox_id, TXT_COMMON_ALL);

            GUILIST_SetMaxItem(list_id, (MMISEARCH_MATCHED_NUM_MAX+1), TRUE );
			
            GUILIST_SetForbidTip(list_id,TRUE);

            MMIAPISEARCH_SetSearchWinHandle(win_id);
        }
#if  !defined MMI_PDA_SUPPORT // hermann liu	
        if (PNULL == s_search_type_opt_ptr)
        {
            return FALSE;
        }
        s_search_type_opt_ptr->type_button_text_id[MMISEARCH_TYPE_ALL] = TXT_COMMON_ALL;
        s_search_type_opt_ptr->type_button_text_id[MMISEARCH_TYPE_FILE] = TXT_FILE;
        s_search_type_opt_ptr->type_button_text_id[MMISEARCH_TYPE_PB] = STXT_MAIN_PB;
        s_search_type_opt_ptr->type_button_text_id[MMISEARCH_TYPE_SMS] = STXT_MAIN_MESSAGE;
        s_search_type_opt_ptr->type_button_text_id[MMISEARCH_TYPE_APP] = TXT_APPLICATION;    
        s_search_type_opt_ptr->type_button_text_id[MMISEARCH_TYPE_CL] = STXT_MAIN_CL;   
        
        s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_ALL] = IMAGE_SEARCH_ALL;
        s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_FILE] = IMAGE_SEARCH_FILE;
        s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_PB] = IMAGE_SEARCH_PB;
        s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_SMS] = IMAGE_SEARCH_MESSAGE;
        s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_APP] = IMAGE_SEARCH_APP;
        s_search_type_opt_ptr->type_button_image_id[MMISEARCH_TYPE_CL] = IMAGE_SEARCH_CALLLOG;


        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ALL, STXT_SEARCH,STXT_RETURN, FALSE);

#ifdef MMISEARCH_SUPPORT_ONLYMMS     //if mms only,couldn't changed type
        if (MMISEARCH_isOnlyMMSSearch())
        {
            GUIEDIT_SetSoftkey(editbox_id,0,1,TXT_NULL,TXT_NULL ,PNULL);
        }
        else
#endif
        {
            MMISEARCH_TYPE_E type = MMISEARCH_GetSearchType();
            GUIEDIT_GetString(editbox_id, &keyword_str);
        //      GUIWIN_SetSoftkeyTextId(win_id, s_search_type_opt_ptr->type_button_text_id[type] , STXT_SEARCH,STXT_RETURN, TRUE);
            GUIEDIT_SetSoftkey(editbox_id,0,1,s_search_type_opt_ptr->type_button_text_id[type] ,s_search_type_opt_ptr->type_button_text_id[type] ,PNULL);
            GUIEDIT_SetSoftkey(editbox_id,2,1,STXT_RETURN ,TXT_COMMON_CLEAR ,PNULL);
        }

#endif
            
        MMK_SetAtvCtrl( win_id,  editbox_id);
        break;

        case MSG_LCD_SWITCH:
        {
            searchbutton_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            if(MMK_IsWindowLandscape(win_id))
            {
                searchbutton_width.add_data = MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT_H;
            }
            else
            {
                searchbutton_width.add_data = MMISEARCH_SEARCH_BUTTON_WIDTH_PERCENT;
            }
            GUIFORM_SetChildWidth(MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID, type_button_id, &searchbutton_width);
#ifdef TOUCH_PANEL_SUPPORT            
            GUIFORM_ForceLayout(MMISEARCH_KEYWORD_FORM_FORM_CTRL_ID);
#endif
        }
        break;
        
#if  !defined MMI_PDA_SUPPORT // hermann liu
        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
            MMI_CTRL_ID_T curr_id = MMK_GetActiveCtrlId(win_id);
            
            if(curr_id == list_id && 0 < GUILIST_GetTotalItemNum(list_id))
            {
               
            }            
            else if(MMIAPISEARCH_IsAsyncSearching())
            {
                MMISEARCH_TYPE_E type = MMISEARCH_GetSearchType();
                MMISEARCH_StopAsyncSearch();
                GUIWIN_SetSoftkeyTextId(win_id, s_search_type_opt_ptr->type_button_text_id[type] , STXT_SEARCH,TXT_DELETE, TRUE);
            }
            else
            {
#ifdef MMISEARCH_SUPPORT_ONLYMMS     //if mms only,couldn't changed type
                if (MMISEARCH_isOnlyMMSSearch())
                {
                    break;
                }
#endif
                MMK_CreateWin((uint32*)MMISEARCH_TYPE_OPT_MENU_WIN_TAB,PNULL);
            }
         }    
        break;

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T curr_id = MMK_GetActiveCtrlId(win_id);
            SCI_TRACE_LOW("MMISEARCH_HandleMainSmartSearchWinMsg curr_id = %d, list_id =%d", curr_id, list_id);
            if(curr_id == list_id && 0 < GUILIST_GetTotalItemNum(list_id))
            {
                HandleSearchListControlPenOK(list_id);
            }
            else
            {
                if(!MMIAPISEARCH_IsAsyncSearching())
                {
                    GUIEDIT_GetString(editbox_id, &keyword_str);
                    GUILIST_RemoveAllItems(search_list_handle);
        
                    if(keyword_str.wstr_len == 0)
                    { 
                        MMK_PostMsg(list_id,MSG_CTL_PAINT,PNULL, 0); 
                    }
                    else
                    {
                        MMISEARCH_EntrySearching(&keyword_str);
                    }
                 }
            }
         }            
        break;
#endif
     case MSG_SEARCH_OPEN_TYPE_BOX:
        HandleSmartSearchTypeButtonCallBack();
        break;
        
     case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        SetSearchListItemContent((GUILIST_NEED_ITEM_CONTENT_T*)param);
        break;
     
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        SetSearchListItemData((GUILIST_NEED_ITEM_DATA_T*)param);
        break;     
        
    case MSG_GET_FOCUS:
#if  !defined MMI_PDA_SUPPORT       
        MMK_SetAtvCtrl( win_id,  editbox_id);
#ifdef MMISEARCH_SUPPORT_ONLYMMS     //if mms only,couldn't changed type
        if (MMISEARCH_isOnlyMMSSearch())
        {
            GUIEDIT_SetSoftkey(editbox_id,0,1,TXT_NULL,TXT_NULL ,PNULL);
        }
        else
#endif
        {
            MMISEARCH_TYPE_E type = MMISEARCH_GetSearchType();
            GUIEDIT_GetString(editbox_id, &keyword_str);
      //      GUIWIN_SetSoftkeyTextId(win_id, s_search_type_opt_ptr->type_button_text_id[type] , STXT_SEARCH,STXT_RETURN, TRUE);
            GUIEDIT_SetSoftkey(editbox_id,0,1,s_search_type_opt_ptr->type_button_text_id[type] ,s_search_type_opt_ptr->type_button_text_id[type] ,PNULL);
            GUIEDIT_SetSoftkey(editbox_id,2,1,STXT_RETURN ,TXT_COMMON_CLEAR ,PNULL);
        }
#endif
        if (MMIAPISEARCH_IsNeedSearchAgain())
        {
            MMIAPISEARCH_ResetAllIsNeedRestart();
            HandleSearchButtonPenOk();
        }
        break;

    case MSG_LOSE_FOCUS:
        if (MMIAPISEARCH_IsAsyncSearching())
        {
            MMISEARCH_StopAsyncSearch();
        } 
        break;

	case MSG_FULL_PAINT:
        if (MMIAPISEARCH_IsNeedSearchAgain())
        {
            MMIAPISEARCH_ResetAllIsNeedRestart();
            HandleSearchButtonPenOk();
        }
        break;
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_NOTIFY_T notify = {0};
            if(param == 0)
            {
                break;
            }
            SCI_MEMCPY(&notify, param, sizeof(MMI_NOTIFY_T));
            if(search_button_id == notify.src_id )
            {
                HandleSearchButtonPenOk();
#if !defined MMI_PDA_SUPPORT        
                MMK_SetAtvCtrl( win_id,  editbox_id);
#endif
             } 
            else if(type_button_id == notify.src_id )
            {
                HandleSmartSearchTypeButtonCallBack();
            }
            else if(search_list_handle == notify.src_handle )
            {
                HandleSearchListControlPenOK(search_list_handle);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_TIMER:
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
        if (s_pb_search_update_time_id == *((uint8 *) param))
        {
            MMK_StopTimer(s_pb_search_update_time_id);
            s_pb_search_update_time_id = 0;
            if (!MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_SMS))
            {
                MMIAPIPB_UpdatePbSearching();
                if (MMISEARCH_STATUS_START == MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_PB))
                {
                    s_pb_search_update_time_id = MMK_CreateWinTimer(win_id, MMISEARCH_FILE_UPDATE_TIME, FALSE);
                }
                else if (MMISEARCH_IsSearchEnd())
                {
                    MMK_PostMsg(search_win_handle, MSG_SEARCH_UPDATE_BUTTON_VIEW, PNULL, 0);
                }
            }
            else
            {
                s_pb_search_update_time_id = MMK_CreateWinTimer(win_id, MMISEARCH_PB_UPDATE_TIME, FALSE);
            }
        }
        else 
#endif
        if(s_file_search_update_time_id == *((uint8 *) param))
        {
            MMK_StopTimer(s_file_search_update_time_id);
            s_file_search_update_time_id = 0;
            if (!MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_SMS)
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
            &&!MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_PB)
#endif
            )
            {
                UpdateFileSearching();
                if (MMISEARCH_STATUS_START == MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_FILE))
                {
                    s_file_search_update_time_id = MMK_CreateWinTimer(win_id, MMISEARCH_FILE_UPDATE_TIME, FALSE);
                }
                else if (MMISEARCH_IsSearchEnd())
                {
                    MMK_PostMsg(search_win_handle, MSG_SEARCH_UPDATE_BUTTON_VIEW, PNULL, 0);
                }
            }
            else
            {
                s_file_search_update_time_id = MMK_CreateWinTimer(win_id, MMISEARCH_FILE_UPDATE_TIME, FALSE);
            }
        }    
        break;

    case MSG_SEARCH_LIST_APPEND_MATCH_ITEM:
        {
            GUILIST_ITEM_T item_t  = {0};
            GUILIST_ITEM_T split_item_t  = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            uint32 displayed_item_num = MMISEARCH_GetDisplayItemsNum();
            uint32 matched_item_num = MMISEARCH_GetMatchItemsNum();
            MMISEARCH_MATCH_ITEM_T *match_item_ptr = PNULL;
            MMISEARCH_MATCH_ITEM_T *pre_match_item_ptr = PNULL;
                
            MMISEARCH_ResetAppendFlag();
            item_t.item_data_ptr = &item_data;

            if (displayed_item_num < matched_item_num)
            {
                for (i = displayed_item_num; i < matched_item_num; i++)
                { 
                    pre_match_item_ptr = MMISEARCH_GetOneMatchItemPtr(i - 1);
                    match_item_ptr = MMISEARCH_GetOneMatchItemPtr(i);

                    if ((PNULL != match_item_ptr && PNULL != pre_match_item_ptr && match_item_ptr->match_item_type != pre_match_item_ptr->match_item_type)
                        || (PNULL == pre_match_item_ptr && PNULL != match_item_ptr))
                    {
                        s_split_item_info[s_split_item_num].list_index = s_split_item_num + i;
                        s_split_item_info[s_split_item_num].search_type = match_item_ptr->match_item_type;
                        s_split_item_num++;

                        split_item_t.item_state |=GUIITEM_STATE_SPLIT;  
                        // GUILIST_SetSplitOnside(display_list_handle, TRUE);
                        GUILIST_AppendItem( search_list_handle, &split_item_t );
                    }
                    GUILIST_AppendItem( search_list_handle, &item_t );
			        if(PNULL != match_item_ptr )
			        {
                        GUILIST_SetItemStyle(search_list_handle, match_item_ptr->match_item_style, s_split_item_num + i);
			        }

                }

                MMISEARCH_SetDisplayItemsNum(matched_item_num);
                if (MMK_IsFocusWin(win_id))
                {
                    MMK_SendMsg(search_list_handle, MSG_CTL_PAINT, PNULL);
                }
                if (MMISEARCH_IsSearchEnd() && MMISEARCH_IsDisplayItemsEnd())
                {
                    MMK_PostMsg(search_win_handle, MSG_SEARCH_UPDATE_BUTTON_VIEW, PNULL, 0);
                }
            }
        }
        break;

    case MSG_SEARCH_SET_PROMPT_ITEM_INVALID:
        {
            uint32 index = 0;
            BOOLEAN is_exist_prompt_item = FALSE;
            
            for (index = 0; index < MMISEARCH_GetMatchItemsNum(); index++)
            {
                if (MMISEARCH_GetOneMatchItemPtr(index)->match_item_type == MMISEARCH_TYPE_PROMPT_EMPTY)
                {
                    is_exist_prompt_item = TRUE;
//                    GUILIST_SetItemGray(search_list_handle, index, TRUE);
                }
            }
            
            if (is_exist_prompt_item)
            {
#ifdef MMISEARCH_SUPPORT_ONLYMMS     //if mms only,couldn't changed type
                if (MMISEARCH_isOnlyMMSSearch())
                {
                    GUILIST_SetItemGray(search_list_handle, 1, TRUE);
                }
                else
#endif
                {
                    GUILIST_SetItemGray(search_list_handle, 3, TRUE);
                }
                MMK_SendMsg(search_list_handle, MSG_CTL_PAINT, 0);
            }
        }
        break;
        
    case MSG_SEARCH_LIST_REAPPEND_MATCH_ITEM:
        GUILIST_RemoveAllItems(search_list_handle);
        MMK_SendMsg(win_id, MSG_SEARCH_LIST_APPEND_MATCH_ITEM, PNULL);
        break;
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    case MSG_SEARCH_PB_START_UPDATE_TIMER:
        s_pb_search_update_time_id = MMK_CreateWinTimer(win_id, MMISEARCH_PB_UPDATE_TIME, FALSE);
        break;
#endif
    case MSG_SEARCH_FILE_START_UPDATE_TIMER:
        s_file_search_update_time_id = MMK_CreateWinTimer(win_id, MMISEARCH_FILE_UPDATE_TIME, FALSE);
        break;
        
    case MSG_SEARCH_FILE_LOAD_ALL_LIST_FINISH:
        MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_FILE);

#if  !defined MMI_PDA_SUPPORT // hermann liu
        {
            MMISEARCH_TYPE_E type = MMISEARCH_GetSearchType();
      
            if(list_id !=MMK_GetActiveCtrlId(win_id))
            {
                GUIWIN_SetSoftkeyTextId(win_id, s_search_type_opt_ptr->type_button_text_id[type] , STXT_SEARCH,TXT_DELETE, TRUE);
            }
        }
#endif
        break;
        
    case MSG_SEARCH_START_SEARCH:
        {
            keyword_str.wstr_ptr = (wchar*)param;
            keyword_str.wstr_len = MMIAPICOM_Wstrlen((wchar*)param);
            // set string info
            GUIEDIT_SetString(editbox_id, keyword_str.wstr_ptr, keyword_str.wstr_len);

            MMISEARCH_EntrySearching(&keyword_str);
        }
        break;

    case MSG_SEARCH_UPDATE_BUTTON_VIEW:
        UpdateSearchButtonStatus();
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break; 
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != param)
        {
            SCI_FREE(param);
        }
        
        MMISEARCH_StopAsyncSearch(); 
        MMISEARCH_Free();
        if (PNULL != s_search_type_opt_ptr)
        {
            SCI_FREE(s_search_type_opt_ptr);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif


/*Edit by script, ignore 1 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
