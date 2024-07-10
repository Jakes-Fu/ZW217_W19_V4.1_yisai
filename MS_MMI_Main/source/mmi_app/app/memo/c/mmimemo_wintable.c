/*****************************************************************************
** File Name:      mmimemo_wintable.c                                          *
** Author:          fen.xie                                                    *
** Date:            05/08/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the memo             *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_memo_trc.h"
#ifdef MMI_MEMO_SUPPORT
#include "mmk_app.h"
#include "mmipub.h"
#include "mmisms_text.h"
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "guibutton.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmimemo_id.h"
#include "mmimemo_image.h"
#include "mmimemo_text.h"
#include "mmimemo_func.h"
#include "mmimemo_menutable.h"
//#include "mmiset.h"
#include "guitext.h"
#include "guictrl_api.h"
#include "guires.h"
#include "mmifmm_export.h"
#include "mmiudisk_export.h"
#include "mmi_solarlunar.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#ifdef MMI_MEMO_SUPPORT

#define NUM_STATE_MAX   10
#define NUM_STATE_AND_TEXT_MAX   255
//@add for READ MEMO WINDOW in QQVGA and QCIF
#ifdef MMI_MEMO_MINI_SUPPORT
#ifdef MAINLCD_DEV_SIZE_176X220
	#define MMIMEMO_READ_TEXT_HEIGHT_PERCENT  65
//#elif defined(MAINLCD_DEV_SIZE_220X176)
// 	#define MMIMEMO_READ_TEXT_HEIGHT_PERCENT  56
#else
	#define MMIMEMO_READ_TEXT_HEIGHT_PERCENT  56
#endif

#define MMIMEMO_READ_TEXT_WIDTH_MARGIN   2
#define MMIMEMO_READ_LABEL_MARGIN         1
#define MEMO_FONT   SONG_FONT_12
#else 
#define MEMO_FONT   MMI_DEFAULT_SMALL_FONT
#endif

#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN   s_is_memo_closing = FALSE;    
LOCAL uint16        s_memo_del_total_num = 0;   //总共要被删除的总数
LOCAL BOOLEAN       s_del_memo_is_cancelled = FALSE;     //delete file is canceled
LOCAL MMI_STRING_T    s_edit_text_save_buffer = {0}; 
LOCAL wchar*    s_edit_text_filename_ptr = PNULL; 
LOCAL BOOLEAN       s_is_content_edit = FALSE;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle memo list win msg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoListWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );
/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoEditWinMsg(
										MMI_WIN_ID_T        win_id, 
										MMI_MESSAGE_ID_E    msg_id, 
										DPARAM              param
										);
                                           
/*****************************************************************************/
//  Description : append memo list
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void AppendMemoTextList(
                            MMI_CTRL_ID_T       ctrl_id
                            );
                            
/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoOpenMainWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );
/*****************************************************************************/
//  Description :  Handle save one memo waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoSaveWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param);

/*****************************************************************************/
//  Description : delete selected memo
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void MemoDeleteMarkedItem(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in bookmark main win id
                             );

/*****************************************************************************/
//  Description : handle delete selected memo confirm message
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleDeleteMemoCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:bookmark main win id
                              );

/*****************************************************************************/
//  Description : to handle the  delete query window    
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelMemoQueryWin(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );      
                                            
/*****************************************************************************/
//  Description : 阅读窗口处理函数
//  Global resource dependence : none
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoReadWinMsg(
										MMI_WIN_ID_T        win_id, 
										MMI_MESSAGE_ID_E    msg_id, 
										DPARAM              param
										);
                                            
/*****************************************************************************/
//  Description : the callback of memo button1 callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MemoButton1CallBack(void);

/*****************************************************************************/
//  Description : the callback of memo button2 callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MemoButton2CallBack(void);

/*****************************************************************************/
//  Description : the callback of memo button3 callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MemoButton3CallBack(void);

 /*****************************************************************************/
//  Description : set time info to labels
//  Global resource dependence : none
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void MemoSetTimeLabel(
                SCI_DATE_T sys_date_t,
                SCI_TIME_T sys_time_t,
                MMI_CTRL_ID_T label1_ctrl_id, 
                MMI_CTRL_ID_T label2_ctrl_id, 
                MMI_CTRL_ID_T label3_ctrl_id
                );

/*****************************************************************************/
//  Description : set memo option menu
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL void MemoSetOptMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
// 	Description : handle memo list mark option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );

/*****************************************************************************/
// 	Description : handle memo list mark cancel option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListUnMarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 );

/*****************************************************************************/
// 	Description : handle memo list mark all option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListUnMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  );

/*****************************************************************************/
// 	Description : handle memo list mark all cancel option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListUnUnMarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    );

/*****************************************************************************/
//  Description : handle memo popmenu window message
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoPopMenuWinMsg(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param);
/*****************************************************************************/
//  Description : MMIMEMO_IsContentEdit
//  Global resource dependence : 
//  Author:guodong.liu
//  Note: 是否在编辑界面
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_IsContentEdit(void);
/*****************************************************************************/
//  Description : get s_edit_text_filename_ptr
//  Global resource dependence : 
//  Author: guodong.liu
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIMEMO_GetEdittext_Ptr(void);
/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE( MMIMEMO_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMemoListWinMsg),
    WIN_ID(MMIMEMO_LIST_WIN_ID),
    WIN_TITLE(TXT_MEMO),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIMEMO_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NEW, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMEMO_EDIT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleMemoEditWinMsg),    
    WIN_ID( MMIMEMO_EDIT_WIN_ID),
    WIN_TITLE(TXT_MEMO),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMEMO_EDIT_FORM_CTRL_ID),
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMEMO_EDIT_CHILD1_FORM_CTRL_ID, MMIMEMO_EDIT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIMEMO_EDIT_LABEL1_CTRL_ID, MMIMEMO_EDIT_CHILD1_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, FALSE, MMIMEMO_EDIT_LABEL3_CTRL_ID, MMIMEMO_EDIT_CHILD1_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE, 0, MMIMEMO_EDITBOX_CTRL_ID, MMIMEMO_EDIT_FORM_CTRL_ID),
        CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_RETURN, MMIMEMO_EDIT_SOFTKEY_CTRL_ID, MMIMEMO_EDIT_FORM_CTRL_ID),
#else
    CREATE_EDIT_TEXT_CTRL(MMIMEMO_CONTENT_MAX_LEN,MMIMEMO_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMEMO_READ_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleMemoReadWinMsg),    
    WIN_ID( MMIMEMO_READ_WIN_ID),
    WIN_TITLE(TXT_MEMO),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIMEMO_READ_FORM_CTRL_ID),
#ifdef MMI_MEMO_MINI_SUPPORT
        CHILD_TEXT_CTRL(TRUE, MMIMEMO_READ_TEXT_CTRL_ID, MMIMEMO_READ_FORM_CTRL_ID),
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMEMO_READ_CHILD1_FORM_CTRL_ID, MMIMEMO_READ_FORM_CTRL_ID),
	#ifdef MAINLCD_DEV_SIZE_160X128  //2行显示日期
			CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMEMO_READ_CHILD1_0_FORM_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIMEMO_READ_LABEL1_CTRL_ID, MMIMEMO_READ_CHILD1_0_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, FALSE, MMIMEMO_READ_LABEL2_CTRL_ID, MMIMEMO_READ_CHILD1_0_FORM_CTRL_ID),
			CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMEMO_READ_CHILD1_1_FORM_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID),
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIMEMO_READ_LABEL3_CTRL_ID, MMIMEMO_READ_CHILD1_1_FORM_CTRL_ID),
	#else
			CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIMEMO_READ_LABEL1_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIMEMO_READ_LABEL2_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIMEMO_READ_LABEL3_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID),
    #endif  
#else
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMEMO_READ_CHILD1_FORM_CTRL_ID, MMIMEMO_READ_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIMEMO_READ_LABEL1_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, FALSE, MMIMEMO_READ_LABEL3_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID),
        CHILD_TEXT_CTRL(TRUE, MMIMEMO_READ_TEXT_CTRL_ID, MMIMEMO_READ_FORM_CTRL_ID),
#endif
#ifdef MMI_PDA_SUPPORT
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMEMO_READ_CHILD2_FORM_CTRL_ID, MMIMEMO_READ_FORM_CTRL_ID),
            CHILD_BUTTON_CTRL(TRUE, IMAGE_MEMO_PREV, MMIMEMO_READ_BUTTON1_CTRL_ID, MMIMEMO_READ_CHILD2_FORM_CTRL_ID),
            CHILD_BUTTON_CTRL(TRUE, IMAGE_MEMO_DEL, MMIMEMO_READ_BUTTON2_CTRL_ID, MMIMEMO_READ_CHILD2_FORM_CTRL_ID),
            CHILD_BUTTON_CTRL(TRUE, IMAGE_MEMO_NEXT, MMIMEMO_READ_BUTTON3_CTRL_ID, MMIMEMO_READ_CHILD2_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_DELETE, TXT_EDIT, STXT_RETURN), 
#endif
   END_WIN
};

WINDOW_TABLE(MMIMEMO_POPMENU_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleMemoPopMenuWinMsg),
    WIN_ID(MMIMEMO_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MEMO_OPTION,MMIMEMO_POPMEMU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : merge the text id and num state("bookmark(m/n)".etc)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetMemoListWinTitle(void)
{
    MMI_HANDLE_T win_handle= MMIMEMO_LIST_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIMEMO_LIST_CTRL_ID;
    uint16        total_item_num = GUILIST_GetTotalItemNum(ctrl_id);
    MMI_STRING_T    text_title_id_str = {0};
    
    MMI_GetLabelTextByLang(TXT_MEMO, &text_title_id_str);
    if (total_item_num > 0)
    {
        MMI_STRING_T    text_title_str = {0};
        MMI_STRING_T    text_title_num_str = {0};
        uint8    num[NUM_STATE_MAX+1] = {0};
        wchar   num_wstr[NUM_STATE_MAX + 1] = {0};
        wchar   title_str[NUM_STATE_AND_TEXT_MAX + 1] = {0};
        uint16  num_len = 0;
                
        sprintf((char *)num, "(%d)", total_item_num);
        num_len = SCI_STRLEN((char *)num);
        MMI_STRNTOWSTR(num_wstr, NUM_STATE_MAX, num, NUM_STATE_MAX, MIN(num_len,NUM_STATE_MAX));
        text_title_num_str.wstr_ptr = num_wstr;
        text_title_num_str.wstr_len = MMIAPICOM_Wstrlen(num_wstr);
        
        text_title_str.wstr_ptr = title_str;
        
        MMIAPICOM_MergeTwoString(
                &text_title_id_str,
                &text_title_num_str,
                &text_title_str,
                NUM_STATE_AND_TEXT_MAX
                );
                
        GUIWIN_SetTitleText(win_handle, text_title_str.wstr_ptr, text_title_str.wstr_len, FALSE);
    }
    else
    {
        GUIWIN_SetTitleText(win_handle, text_title_id_str.wstr_ptr, text_title_id_str.wstr_len, FALSE);
    }
    
}

/*****************************************************************************/
//  Description : append memo list
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void AppendMemoTextList(
                            MMI_CTRL_ID_T       ctrl_id
                            )
{
    uint32          i = 0;
    GUILIST_ITEM_T  item = {0};
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
    GUILIST_SetMaxItem( ctrl_id, MMIMEMO_LIST_MAX_NUM, TRUE );
    GUILIST_SetTextFont(ctrl_id, MEMO_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0));
    
    //set item style
#ifdef MMI_PDA_SUPPORT
    item.item_style =  GUIITEM_STYLE_ONE_LINE_TEXT_TEXT_ICON;
#else
#ifdef MMI_MEMO_MINI_SUPPORT
    item.item_style =  GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
#else
	#if (defined MMI_GUI_STYLE_MINISCREEN ||defined MAINLCD_SIZE_320X480)
    item.item_style =  GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_ICON;
	#else
    item.item_style =  GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT;
	#endif
#endif
#endif
    //item.item_state = GUIITEM_STATE_CONTENT_CHECK;//NEWMS00121925
    for ( i = 0; i < MMIMEMO_GetTotalNum() ; i++ )
    {
        GUILIST_AppendItem( ctrl_id, &item );
    } 
    
}


/*****************************************************************************/
//  Description : append memo list
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void AppendMemoTextListItem(
                            MMI_CTRL_ID_T       ctrl_id,
                            uint16                  item_index
                            )
{
    GUILIST_ITEM_T  item = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T item_string = {0};
    SCI_DATE_T  modify_date = {0};//日期
    uint8   date_len = 0;
    wchar   date_wstr[MMISET_DATE_STR_LEN+1] = {0};
    uint8   date[MMISET_DATE_STR_LEN] = {0};
    uint32  max_wstr_len = 0;
    
    item.item_data_ptr = &item_data;    

    if(0 < GUILIST_GetTotalItemNum(ctrl_id))
    {
        item_data.softkey_id[0] = STXT_OPTION;
#ifdef MMI_MEMO_MINI_SUPPORT
#ifdef GUI_MID_SOFTKEY_USE_IMG
        item_data.softkey_id[1] = IMAGE_COMMON_SOFTKEY_START_ICON; //替代view
#else
        item_data.softkey_id[1] = TXT_VIEW;
#endif
#else
#ifndef MMI_GUI_STYLE_MINISCREEN
        item_data.softkey_id[1] = TXT_NULL;
#endif
#endif
        item_data.softkey_id[2] = STXT_RETURN; 
    }
    else
    {
        item_data.softkey_id[0] = TXT_NEW;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN; 
    }

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	max_wstr_len = MMIMEMO_CONTENT_MAX_LEN;
    item_string.wstr_ptr = SCI_ALLOC_APP((max_wstr_len + 1) * sizeof(wchar));
    
    if (PNULL == item_string.wstr_ptr)
    {
        SCI_TRACE_LOW("[mmimemo] AppendMemoTextListItem item_string.wstr_ptr = PNULL");
        return;
    }

    SCI_MEMSET(item_string.wstr_ptr, 0x00, (max_wstr_len + 1) * sizeof(wchar));
    MMIMEMO_GetContent(item_index, &item_string, max_wstr_len, &modify_date, PNULL);
    item_data.item_content[0].item_data.text_buffer.wstr_len = item_string.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = item_string.wstr_ptr;
    
    MMIAPISET_FormatDateStrByDateStyle(modify_date.year,modify_date.mon, modify_date.mday,
        '/',date,MMISET_DATE_STR_LEN);
    date_len = strlen( (char*)date );
    MMI_STRNTOWSTR( 
            date_wstr,
            date_len,
            (const uint8 *)date,
            date_len,
            date_len
            );	
#ifdef MMI_MEMO_MINI_SUPPORT
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = IMAGE_NULL;
#else
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len =MMIAPICOM_Wstrlen(date_wstr);
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = date_wstr;
#if (defined MMI_GUI_STYLE_MINISCREEN ||defined MAINLCD_SIZE_320X480)
    item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[2].item_data.image_id = IMAGE_MEMO_MAIN_ARROW;
#endif
#endif
    GUILIST_SetItemData( ctrl_id, &item_data, item_index );
    if (item_string.wstr_ptr != PNULL)
    {
       SCI_FREE(item_string.wstr_ptr);
    }

}

/*****************************************************************************/
//  Description : replace or delete item
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void AddMemoTex(
                            MMI_CTRL_ID_T       ctrl_id
                            )
{
    uint32  list_total_num = GUILIST_GetTotalItemNum(ctrl_id);

    if (list_total_num >= MMIMEMO_LIST_MAX_NUM)
    {
        MMIPUB_OpenAlertWarningWin(TXT_SMS_FULL_LIST);
        return;
    }
    
    MMK_CreateWin((uint32*)MMIMEMO_EDIT_WIN_TAB, (ADD_DATA)TRUE);
}

/*****************************************************************************/
//  Description : the callback of memo button1 callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MemoButton1CallBack(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
    uint16 current_index = GUILIST_GetCurItemIndex(list_ctrl_id);

    //pre memo
    if (current_index > 0)
    {
        current_index--;
        GUILIST_SetCurItemIndex(list_ctrl_id, current_index);
        MMK_CloseWin(MMIMEMO_READ_WIN_ID);
        MMK_CreateWin((uint32*)MMIMEMO_READ_WIN_TAB, PNULL);
    }
    else
    {
        //do nothing
    }
    return result;
}

/*****************************************************************************/
//  Description : the callback of memo button3 callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MemoButton2CallBack(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        query_win_id = MMIMEMO_QUERY_WIN_ID;

    //delete one memo
    MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY, &query_win_id, HandleDelMemoQueryWin);

    return result;
}

/*****************************************************************************/
//  Description : the callback of memo button4 callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MemoButton3CallBack(void)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
    uint16 current_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    uint16 total_num = GUILIST_GetTotalItemNum(list_ctrl_id);

    //next memo
    if (current_index < (total_num - 1))
    {
        current_index++;
        GUILIST_SetCurItemIndex(list_ctrl_id, current_index);
        MMK_CloseWin(MMIMEMO_READ_WIN_ID);
        MMK_CreateWin((uint32*)MMIMEMO_READ_WIN_TAB, PNULL);
    }
    else
    {
        //do nothing
    }
    return result;
}

/*****************************************************************************/
//  Description : 阅读窗口内容设置函数
//  Global resource dependence : none
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void MemoSetReadInfo(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIMEMO_READ_TEXT_CTRL_ID;
    MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
    uint16    cur_memo_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    MMI_STRING_T    text_buffer = {0};
    SCI_DATE_T  date_t = {0};//日期
    SCI_TIME_T  time_t = {0};//时间
    uint32  max_wstr_len = 0;

    max_wstr_len = MMIMEMO_CONTENT_MAX_LEN;
    text_buffer.wstr_ptr = SCI_ALLOC_APP((max_wstr_len + 1) * sizeof(wchar));
    if (PNULL == text_buffer.wstr_ptr)
    {
        SCI_TRACE_LOW("[mmimemo] MemoSetReadInfo text_buffer.wstr_ptr = PNULL");
        return;
    }

    SCI_MEMSET(text_buffer.wstr_ptr, 0x00, (max_wstr_len + 1) * sizeof(wchar));
    MMIMEMO_GetContent(cur_memo_index, &text_buffer, max_wstr_len, &date_t, &time_t);
    GUITEXT_SetString(ctrl_id, text_buffer.wstr_ptr, text_buffer.wstr_len, FALSE);

    if (text_buffer.wstr_ptr != PNULL)
    {
       SCI_FREE(text_buffer.wstr_ptr);
    }
    
    MemoSetTimeLabel(date_t, time_t,MMIMEMO_READ_LABEL1_CTRL_ID, MMIMEMO_READ_LABEL2_CTRL_ID, MMIMEMO_READ_LABEL3_CTRL_ID);
}
/*****************************************************************************/
//  Description : 阅读窗口的控件设置
//  Global resource dependence : none
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void MemoInitReadCtrl(MMI_WIN_ID_T win_id)
{
    GUI_BORDER_T    text_border = {0};
#ifdef MMI_PDA_SUPPORT
    MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
    uint16 total_num = GUILIST_GetTotalItemNum(list_ctrl_id);
    uint16    cur_memo_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    GUI_BG_T gray_bg = {0};
    GUI_BG_T fg_bg = {0};
    GUIFORM_CHILD_HEIGHT_T  height = {0};
    uint16  img_height = 0;
#elif defined MMI_MEMO_MINI_SUPPORT
    GUIFORM_CHILD_HEIGHT_T  height = {0};
    //uint16    time_space = 0;
#endif    
    
    MemoSetReadInfo();

    GUIFORM_PermitChildBg(MMIMEMO_READ_FORM_CTRL_ID,FALSE);
    GUIFORM_PermitChildBorder(MMIMEMO_READ_FORM_CTRL_ID,FALSE);
#ifdef MMI_MEMO_MINI_SUPPORT
    GUIFORM_SetType(MMIMEMO_READ_FORM_CTRL_ID,GUIFORM_TYPE_TP);
    GUIFORM_PermitChildFont(MMIMEMO_READ_FORM_CTRL_ID, FALSE);
    height.type = GUIFORM_CHILD_HEIGHT_PERCENT;
    height.add_data = 100 - MMIMEMO_READ_TEXT_HEIGHT_PERCENT;
    GUIFORM_SetChildHeight(MMIMEMO_READ_FORM_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID, &height);
    height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
    GUIFORM_SetChildHeight(MMIMEMO_READ_FORM_CTRL_ID, MMIMEMO_READ_TEXT_CTRL_ID, &height);
    GUITEXT_SetMargin(MMIMEMO_READ_TEXT_CTRL_ID, MMIMEMO_READ_TEXT_WIDTH_MARGIN);
    {
		uint16 space = 0;
#ifdef MAINLCD_DEV_SIZE_160X128  //2//行显示日期
		height.add_data = 50;
		GUIFORM_SetChildHeight(MMIMEMO_READ_CHILD1_FORM_CTRL_ID, MMIMEMO_READ_CHILD1_0_FORM_CTRL_ID, &height);
		GUIFORM_SetChildHeight(MMIMEMO_READ_CHILD1_FORM_CTRL_ID, MMIMEMO_READ_CHILD1_1_FORM_CTRL_ID, &height);
#endif
		GUIFORM_SetSpace(MMIMEMO_READ_CHILD1_FORM_CTRL_ID, PNULL, &space);
	}
    MMK_SetAtvCtrl(win_id, MMIMEMO_READ_TEXT_CTRL_ID);
    GUILABEL_SetMargin(MMIMEMO_READ_LABEL1_CTRL_ID, MMIMEMO_READ_LABEL_MARGIN);
    GUILABEL_SetMargin(MMIMEMO_READ_LABEL2_CTRL_ID, MMIMEMO_READ_LABEL_MARGIN);
    GUILABEL_SetMargin(MMIMEMO_READ_LABEL3_CTRL_ID, MMIMEMO_READ_LABEL_MARGIN);
#endif
    
    GUILABEL_SetFont(MMIMEMO_READ_LABEL1_CTRL_ID, MEMO_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
    GUILABEL_SetFont(MMIMEMO_READ_LABEL2_CTRL_ID, MEMO_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
    GUILABEL_SetFont(MMIMEMO_READ_LABEL3_CTRL_ID, MEMO_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
    
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)  
    if (MMITHEME_IsIstyle())
    {
        GUI_BG_T bg = {0};
        bg.bg_type = GUI_BG_IMG;
        bg.img_id = IMAGE_MEMO_MAIN_BG;
        GUIFORM_SetBg(MMIMEMO_READ_FORM_CTRL_ID, &bg);
        GUIWIN_SetTitleBackground(win_id, IMAGE_MEMO_MAIN_TITLE_BG, 0);
        GUIWIN_SetTitleTextAlign(win_id, ALIGN_HMIDDLE);
    }
   // GUIFORM_IsSlide(MMIMEMO_READ_FORM_CTRL_ID,FALSE);
#endif    
    text_border.type = GUI_BORDER_NONE;
    GUITEXT_SetBorder(&text_border, MMIMEMO_READ_TEXT_CTRL_ID);
    
#ifdef MMI_PDA_SUPPORT
    if (!MMITHEME_IsIstyle())
    {
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        //GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_ISTYLE_TITLE_ADD, IMAGE_ISTYLE_TITLE_ADD_DOWN, FALSE);
        GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_MEMO_TITLE_ADD_BUTTON, IMAGE_MEMO_TITLE_ADD_BUTTON_FOCUS, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
    }
    GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_LEFT, IMAGE_MEMO_TITLE_LEFT_BUTTON, IMAGE_MEMO_TITLE_LEFT_BUTTON_FOCUS, FALSE);

    GUIRES_GetImgWidthHeight(PNULL, &img_height, IMAGE_MEMO_PREV, win_id);
    height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    height.add_data = img_height + 4;
    GUIFORM_SetChildHeight(MMIMEMO_READ_FORM_CTRL_ID, MMIMEMO_READ_CHILD2_FORM_CTRL_ID, &height);

    GUIBUTTON_SetCallBackFunc(MMIMEMO_READ_BUTTON1_CTRL_ID, MemoButton1CallBack);
    GUIBUTTON_SetCallBackFunc(MMIMEMO_READ_BUTTON2_CTRL_ID, MemoButton2CallBack);
    GUIBUTTON_SetCallBackFunc(MMIMEMO_READ_BUTTON3_CTRL_ID, MemoButton3CallBack);

    gray_bg.bg_type = GUI_BG_IMG;
    gray_bg.img_id  = IMAGE_MEMO_PREV_GRAY;
    GUIBUTTON_SetGrayed(MMIMEMO_READ_BUTTON1_CTRL_ID,  FALSE,&gray_bg,FALSE);
    IGUICTRL_SetState(MMK_GetCtrlPtr(MMIMEMO_READ_BUTTON1_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
    gray_bg.img_id  = IMAGE_MEMO_DEL_GRAY;
    GUIBUTTON_SetGrayed(MMIMEMO_READ_BUTTON2_CTRL_ID,  FALSE,&gray_bg,FALSE);
    IGUICTRL_SetState(MMK_GetCtrlPtr(MMIMEMO_READ_BUTTON2_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
    gray_bg.img_id  = IMAGE_MEMO_NEXT_GRAY;
    GUIBUTTON_SetGrayed(MMIMEMO_READ_BUTTON3_CTRL_ID,  FALSE,&gray_bg,FALSE);
    IGUICTRL_SetState(MMK_GetCtrlPtr(MMIMEMO_READ_BUTTON3_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, FALSE);
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        GUIBUTTON_SetRunSheen(MMIMEMO_READ_BUTTON1_CTRL_ID, FALSE);
        GUIBUTTON_SetRunSheen(MMIMEMO_READ_BUTTON2_CTRL_ID, FALSE);
        GUIBUTTON_SetRunSheen(MMIMEMO_READ_BUTTON3_CTRL_ID, FALSE);
        fg_bg.bg_type = GUI_BG_IMG;
        fg_bg.img_id  = IMAGE_MEMO_PREV_FOCUS;
        GUIBUTTON_SetPressedBg(MMIMEMO_READ_BUTTON1_CTRL_ID,  &fg_bg);
        fg_bg.img_id  = IMAGE_MEMO_DEL_FOCUS;
        GUIBUTTON_SetPressedBg(MMIMEMO_READ_BUTTON2_CTRL_ID,  &fg_bg);
        fg_bg.img_id  = IMAGE_MEMO_NEXT_FOCUS;
        GUIBUTTON_SetPressedBg(MMIMEMO_READ_BUTTON3_CTRL_ID,  &fg_bg);
    }
#endif    
    if (total_num<= 1)
    {
        GUIBUTTON_SetGrayed(MMIMEMO_READ_BUTTON1_CTRL_ID, TRUE,PNULL,FALSE);
        GUIBUTTON_SetGrayed(MMIMEMO_READ_BUTTON3_CTRL_ID, TRUE, PNULL, FALSE);//next memo
    }
    else
    {
        if (0 == cur_memo_index)
        {   
            GUIBUTTON_SetGrayed(MMIMEMO_READ_BUTTON1_CTRL_ID, TRUE, PNULL, FALSE);//pre memo
        }
        if (cur_memo_index >= (total_num - 1))
        {   
            GUIBUTTON_SetGrayed(MMIMEMO_READ_BUTTON3_CTRL_ID, TRUE, PNULL, FALSE);//next memo
        }
    }
	GUIFORM_SetChildDock(MMIMEMO_READ_FORM_CTRL_ID, MMIMEMO_READ_CHILD1_FORM_CTRL_ID, GUIFORM_CHILD_DOCK_TOP);

    GUIFORM_SetChildDock(MMIMEMO_READ_FORM_CTRL_ID, MMIMEMO_READ_CHILD2_FORM_CTRL_ID, GUIFORM_CHILD_DOCK_BOTTOM);


#endif
}

/*****************************************************************************/
//  Description : set memo time info to labels
//  Global resource dependence : none
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void MemoSetTimeLabel(
                SCI_DATE_T sys_date_t,
                SCI_TIME_T sys_time_t,
                MMI_CTRL_ID_T label1_ctrl_id, 
                MMI_CTRL_ID_T label2_ctrl_id, 
                MMI_CTRL_ID_T label3_ctrl_id
                )
{
    uint8       time_len = 0;
    wchar       time_string[MMISET_DATE_STR_LEN+1] = {0};
    uint8       date[MMISET_DATE_STR_LEN] = {0};
    uint8       time_str[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    MMI_STRING_T    string_t = {0};        
    
#ifdef MMI_MEMO_MINI_SUPPORT
    MMI_TEXT_ID_T   week_table[] = {
                                        TXT_SHORT_IDLE_SUNDAY,
                                        TXT_SHORT_IDLE_MONDAY,
                                        TXT_SHORT_IDLE_TUESDAY,
                                        TXT_SHORT_IDLE_WEDNESDAY,
                                        TXT_SHORT_IDLE_THURSDAY,
                                        TXT_SHORT_IDLE_FRIDAY,
                                        TXT_SHORT_IDLE_SATURDAY
                                    };
    uint8   calendar_first_day[MONTHNUM] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
    uint8   calendar_first_day_leap[MONTHNUM] = {0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
    uint8   wday = 0;
    uint8   ret_val = 0;
    uint16  year_plus = 0;
    uint16  year = sys_date_t.year;
    uint8   month = sys_date_t.mon;
    uint8   mday = sys_date_t.mday;
    //TM_GetWeekDayOfDate(sys_date_t, &wday);//该函数存在问题，返回值错误；
    //wday = MMICALENDAR_GetWeekday(sys_date_t.year, sys_date_t.mon, sys_date_t.mday);

    year_plus = (uint16)((year-1) + (year-1)/4 - (year-1)/100 + (year-1)/400);

    if (MMIAPICOM_IsLeapYear(year))
    {
        wday = (uint8)(( mday + calendar_first_day_leap[month - 1] + year_plus ) % WEEKDAY_NUM);
    }
    else
    {
        wday = (uint8)(( mday + calendar_first_day[month - 1] + year_plus ) % WEEKDAY_NUM);
    }


    //TM_GetSysDate(&sys_date_t);
    //wday = sys_date_t.wday;
    if(7> wday)
    {
        MMI_GetLabelTextByLang(week_table[wday], &string_t);
        GUILABEL_SetText(label2_ctrl_id, &string_t, FALSE);
    }  
#endif

    MMIAPISET_FormatDateStrByDateStyle(sys_date_t.year,sys_date_t.mon, sys_date_t.mday,
        '/',date,MMISET_DATE_STR_LEN);

    time_len = strlen( (char*)date );
    MMI_STRNTOWSTR( 
            time_string,
            time_len,
            (const uint8 *)date,
            time_len,
            time_len
            );	
    string_t.wstr_len = MMIAPICOM_Wstrlen(time_string);
    string_t.wstr_ptr = time_string;
    GUILABEL_SetText(label1_ctrl_id, &string_t, FALSE);
        
    MMIAPISET_FormatTimeStrByTime(sys_time_t.hour, sys_time_t.min, time_str,MMISET_TIME_STR_12HOURS_LEN + 1);
    //Add item SMS time
    //sprintf(time, "%s             %s", date, time_str);
    time_len = strlen( (char*)time_str );
    
    SCI_MEMSET(time_string, 0x00, sizeof(time_string));
    MMI_STRNTOWSTR( 
            time_string,
            time_len,
            (const uint8 *)time_str,
            time_len,
            time_len
            );	
    string_t.wstr_len = MMIAPICOM_Wstrlen(time_string);
    string_t.wstr_ptr = time_string;
    GUILABEL_SetText(label3_ctrl_id, &string_t, FALSE);   
}

/*****************************************************************************/
//  Description : from read one memo to edti it
//  Global resource dependence : none
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void MemoReadOne2Edit(void)
{
    MMI_STRING_T    text_buffer = {0};    
    MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
    uint16    cur_memo_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    uint32  max_wstr_len = 0;

	max_wstr_len = MMIMEMO_CONTENT_MAX_LEN;
    text_buffer.wstr_ptr = SCI_ALLOC_APP((max_wstr_len + 1) * sizeof(wchar));
    if (PNULL == text_buffer.wstr_ptr)
    {
        SCI_TRACE_LOW("[mmimemo] MemoReadOne2Edit text_buffer.wstr_ptr = PNULL");
        return;
    }
    
    SCI_MEMSET(text_buffer.wstr_ptr, 0x00, (max_wstr_len + 1) * sizeof(wchar));
    MMK_CreateWin((uint32*)MMIMEMO_EDIT_WIN_TAB, PNULL);
    
    GUIEDIT_SetTextMaxLen(MMIMEMO_EDITBOX_CTRL_ID,MMIMEMO_CONTENT_MAX_LEN,MMIMEMO_CONTENT_MAX_LEN);
    MMIMEMO_GetContent(cur_memo_index, &text_buffer, max_wstr_len, PNULL, PNULL);
    GUIEDIT_SetString(MMIMEMO_EDITBOX_CTRL_ID, text_buffer.wstr_ptr, text_buffer.wstr_len);
    if (text_buffer.wstr_ptr != PNULL)
    {
       SCI_FREE(text_buffer.wstr_ptr);
    }
}
/*****************************************************************************/
//  Description : open memo list win msg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMEMO_OpenListWin(void)
{
    MMK_CreateWin((uint32*)MMIMEMO_LIST_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : open memo main win msg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_OpenMain(void)
{
    MMI_WIN_ID_T waiting_win_id = MMIMEMO_OPEN_MAIN_WAIT_WIN_ID;
    uint32       waiting_time_out = 0;

    if(MMIAPIFMM_GetFirstValidDevice() >= MMI_DEVICE_NUM)
    {
        if(MMIAPIUDISK_UdiskIsRun()) //U盘使用中,@CR NEWMS00178360
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            return FALSE;
        }
        /* Modify by michael on 2/27/2012 for NEWMS00172443 : Modify the popup msg*/
//        MMIPUB_OpenAlertWarningWin(TXT_UDISK_SD_NOT_EXIST);
        MMIPUB_OpenAlertWarningWin(TXT_PLEASE_INSERT_SD);
        /************************Modify over*************************/
        return FALSE;
    }

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }

    MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
        IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleMemoOpenMainWaitingWin);

    MMK_PostMsg(waiting_win_id, MSG_MEMO_OPEN_MAIN, PNULL, NULL);

    return TRUE;
}

/*****************************************************************************/
//  Description : stop open memo main win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMEMO_StopOpenMain(void)
{
    s_is_memo_closing = TRUE;
    MMIAPIFMM_SearchFileStop();
    MMIMEMO_DestroyFileArray();
}

/*****************************************************************************/
//  Description : MMIMEMO_StartInit
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMEMO_StartInit(
                MMI_WIN_ID_T      win_id, 
                MMI_MESSAGE_ID_E  msg_id
                )
{
    MMIMEMO_Init(win_id, msg_id);//异步初始化memo的file array
    //MMK_PostMsg(win_id, msg_id, PNULL, NULL);//同步初始化memo的file array
}

/*****************************************************************************/
//  Description : open memo main win msg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void MemoOpenSaveWaitingWin(MMI_STRING_T    str_info)
{
    MMI_WIN_ID_T waiting_win_id = MMIMEMO_SAVE_WAIT_WIN_ID;
    uint32       waiting_time_out = 0;

    MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
        IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleMemoSaveWaitingWin);

    MMK_PostMsg(waiting_win_id, MSG_MEMO_SAVE_FILE, (DPARAM)str_info.wstr_ptr, (str_info.wstr_len + 1) * sizeof(wchar));
}

/*****************************************************************************/
//  Description :  Handle open memo main waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoOpenMainWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static BOOLEAN  s_is_lose_focus = FALSE;
    static BOOLEAN  s_is_start_load = FALSE;

    //SCI_TRACE_LOW:"HandleMemoOpenMainWaitingWin, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMEMO_WINTABLE_836_112_2_18_2_32_44_3,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_is_memo_closing = FALSE;
        break;
    
    case MSG_MEMO_OPEN_MAIN:
        if (!s_is_memo_closing)//protect:normal to start init
        {
            s_is_start_load = TRUE;
            MMIMEMO_StartInit(win_id, MSG_MEMO_OPEN_MAIN_FINISH);
        }
        break;
        
    case MSG_MEMO_OPEN_MAIN_FINISH:
        if (PNULL == param) //MMIAPIFMM_SearchFileInPath is successful 
        {
            s_is_start_load = FALSE;
            MMIMEMO_LoadFinish();
            if(MMK_IsFocusWin(win_id))
            {
                MMIMEMO_OpenListWin();
                MMK_CloseWin(win_id);
            }
            else
            {
                s_is_lose_focus = TRUE;
            }
        }
        else//protect:error
        {
            //do nothing
        }
        break;

    case MSG_GET_FOCUS:
        if (s_is_start_load)
        {
            s_is_start_load = TRUE;
            MMIMEMO_StartInit(win_id, MSG_MEMO_OPEN_MAIN_FINISH);
            //MMK_CloseWin(win_id);
        }
        else if(s_is_lose_focus)
        {
            s_is_lose_focus = FALSE;
            MMIMEMO_OpenListWin();
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_RED:
        MMIMEMO_StopOpenMain();
        result = MMI_RESULT_FALSE;
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    //SCI_TRACE_LOW:"HandleMemoOpenMainWaitingWin, msg_id = 0x%x end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMEMO_WINTABLE_890_112_2_18_2_32_44_4,(uint8*)"d", msg_id);

    return result;
}

/*****************************************************************************/
//  Description :  Handle save one memo waiting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoSaveWaitingWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    LOCAL BOOLEAN  s_is_lose_focus = FALSE;
    LOCAL BOOLEAN   s_save_result = FALSE;

    switch (msg_id)
    {
    case MSG_MEMO_SAVE_FILE:
    {
        MMI_STRING_T    data_str = {0};
        BOOLEAN save_result = FALSE;

        if (param != PNULL)
        {
            data_str.wstr_ptr = (wchar *)param;
            data_str.wstr_len = MMIAPICOM_Wstrlen(data_str.wstr_ptr);
            save_result = MMIMEMO_AddOne(data_str, FALSE, PNULL);
        }
		SCI_TRACE_LOW("HandleMemoSaveWaitingWin save_result = %d", save_result);
        if (MMK_IsOpenWin(MMIMEMO_LIST_WIN_ID))
        {
            MMIMEMO_LoadFinish();//排序等
            AppendMemoTextList(MMIMEMO_LIST_CTRL_ID);
            SetMemoListWinTitle();
        }

        MMK_PostMsg(win_id, MSG_MEMO_SAVE_FILE_FINISH, &save_result, sizeof(save_result));
    }
        break;
        
    case MSG_MEMO_SAVE_FILE_FINISH:
    {
        s_save_result = FALSE;//reset
        
        if (param != PNULL)
        {
            s_save_result = *((BOOLEAN *)param);
        }
		SCI_TRACE_LOW("HandleMemoSaveWaitingWin MSG_MEMO_SAVE_FILE_FINISH s_save_result: %d", s_save_result);
        if(MMK_IsFocusWin(win_id))
        {
            if (s_save_result)
            {
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            }
            else
            {   if(MMIMEMO_IsNoSpace())//NEWMS00204207
                {
						if (s_is_content_edit)
					{
						MMIMEMO_AddOne(s_edit_text_save_buffer, FALSE, PNULL);
						s_is_content_edit = FALSE;
						if (MMK_IsOpenWin(MMIMEMO_LIST_WIN_ID))
						{
							MMIMEMO_LoadFinish();//排序等
							AppendMemoTextList(MMIMEMO_LIST_CTRL_ID);
							SetMemoListWinTitle();
						}
					}	
                    MMIPUB_OpenAlertFailWin(TXT_NO_SPACE);
                }
                else
                {
					SCI_TRACE_LOW("HandleMemoSaveWaitingWin MSG_MEMO_SAVE_FILE_FINISH save fail");
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
            MMK_CloseWin(MMIMEMO_EDIT_WIN_ID);
            MMK_CloseWin(win_id);
            
        }
        else
        {
            s_is_lose_focus = TRUE;
        }
    }
        break;

    case MSG_GET_FOCUS:
        if(s_is_lose_focus)
        {
            s_is_lose_focus = FALSE;
			SCI_TRACE_LOW("HandleMemoSaveWaitingWin MSG_GET_FOCUS s_save_result : %d", s_save_result);
            if (s_save_result)
            {
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(MMIMEMO_EDIT_WIN_ID);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_RED://can't stop it
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}


/*****************************************************************************/
//  Description : handle memo list win msg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoListWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E                  result            = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                 ctrl_id           = MMIMEMO_LIST_CTRL_ID;

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            GUILIST_SetBgImage(ctrl_id, IMAGE_MEMO_MAIN_BG, FALSE);
            GUIWIN_SetTitleTextAlign(win_id, ALIGN_HMIDDLE);
            GUIWIN_SetTitleBackground(win_id, IMAGE_MEMO_MAIN_TITLE_BG, 0);
        }
#endif        
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_MEMO_TITLE_ADD_BUTTON, IMAGE_MEMO_TITLE_ADD_BUTTON_FOCUS, FALSE);
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, ctrl_id);
        GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_LEFT, IMAGE_MEMO_TITLE_LEFT_BUTTON, IMAGE_MEMO_TITLE_LEFT_BUTTON_FOCUS, FALSE);
#endif
        AppendMemoTextList(ctrl_id);
        SetMemoListWinTitle();
        if(0 < GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEW, TXT_NULL, STXT_RETURN, FALSE);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_GET_FOCUS:
        if(0 < GUILIST_GetTotalItemNum(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NEW, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T    *need_data_ptr = PNULL;

            need_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
            //SCI_ASSERT(PNULL != need_data_ptr);/*assert verified*/
            if(PNULL != need_data_ptr)
            {
                AppendMemoTextListItem(ctrl_id, need_data_ptr->item_index);
            }
        }
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        if(0 < GUILIST_GetTotalItemNum(ctrl_id))
        {
            MMK_CreateWin((uint32*)MMIMEMO_READ_WIN_TAB, PNULL);
        }
        break;
        
    case MSG_NOTIFY_LIST_MARK_ITEM:
        {
            uint16 marked_num = GUILIST_GetSelectedItemNum(ctrl_id);
            if(0 == marked_num)
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            }
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(0 < GUILIST_GetTotalItemNum(ctrl_id))
        {            
            MMK_CreateWin((uint32*)MMIMEMO_POPMENU_WIN_TAB,PNULL);    
        }
        else
        {
            AddMemoTex(ctrl_id);
        }
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        AddMemoTex(ctrl_id);
        break;
 #endif
 
    case MSG_APP_MENU:   
        MMK_CreateWin((uint32*)MMIMEMO_POPMENU_WIN_TAB,PNULL);    
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    case MSG_MEMO_DELETE_FILE:
        {
            uint16 select_num = 0;
            uint16 deleted_num = 0;
            MMI_WIN_ID_T     wait_win_id =  MMIMEMO_DEL_WAIT_WIN_ID;
            
            HandleDeleteMemoCnf(*((BOOLEAN*)param),ctrl_id,wait_win_id,win_id);
            select_num = GUILIST_GetSelectedItemIndex(ctrl_id,PNULL,0);
        
            deleted_num = ((s_memo_del_total_num > select_num) ? (s_memo_del_total_num - select_num) : 0);
            MMIPUB_UpdateProgressBar(&wait_win_id, deleted_num);
        }
        break;        
        
    case MSG_CLOSE_WINDOW:
        MMIMEMO_DestroyFileArray();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoEditWinMsg(
										MMI_WIN_ID_T        win_id, 
										MMI_MESSAGE_ID_E    msg_id, 
										DPARAM              param
										)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    BOOLEAN         is_add_new = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T   ctrl_id = MMIMEMO_EDITBOX_CTRL_ID;
    MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
    MMI_STRING_T    str_info = {0};
    LOCAL BOOLEAN   s_is_content_modify = FALSE;
    uint16          cur_memo_index = GUILIST_GetCurItemIndex(list_ctrl_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            SCI_DATE_T sys_date_t = {0};
            SCI_TIME_T sys_time_t = {0};

            s_is_content_modify = FALSE;
            if (is_add_new)
            {
                GUIEDIT_SetTextMaxLen(MMIMEMO_EDITBOX_CTRL_ID,MMIMEMO_CONTENT_MAX_LEN,MMIMEMO_CONTENT_MAX_LEN);
            }

            GUIFORM_PermitChildBg(MMIMEMO_EDIT_FORM_CTRL_ID,FALSE);
            GUIEDIT_PermitBorder(ctrl_id, FALSE);
            GUIEDIT_SetDividingLine(ctrl_id,1,MMI_DARK_GRAY_COLOR);
            GUIEDIT_SetHandleRedKey(FALSE, ctrl_id);
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
            if (MMITHEME_IsIstyle())
            {
                GUI_BG_T bg = {0};
                
                bg.bg_type = GUI_BG_IMG;
                bg.img_id = IMAGE_MEMO_MAIN_BG;
                GUIFORM_SetBg(MMIMEMO_EDIT_FORM_CTRL_ID, &bg);
                GUIWIN_SetTitleBackground(win_id, IMAGE_MEMO_MAIN_TITLE_BG, 0);
                GUIWIN_SetTitleTextAlign(win_id, ALIGN_HMIDDLE);
                
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_FINISH, FALSE);
                GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_MEMO_TITLE_RIGHT_BUTTON, IMAGE_MEMO_TITLE_RIGHT_BUTTON_FOCUS, FALSE);
                GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_LEFT, IMAGE_MEMO_TITLE_LEFT_BUTTON, IMAGE_MEMO_TITLE_LEFT_BUTTON_FOCUS, FALSE);

                GUIFORM_SetChildDisplay(MMIMEMO_EDIT_FORM_CTRL_ID, MMIMEMO_EDIT_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            }
#endif

            //time info
            TM_GetSysDate(&sys_date_t);
            TM_GetSysTime(&sys_time_t);
            MemoSetTimeLabel(sys_date_t, sys_time_t,MMIMEMO_EDIT_LABEL1_CTRL_ID,MMIMEMO_EDIT_LABEL2_CTRL_ID, MMIMEMO_EDIT_LABEL3_CTRL_ID);
            GUILABEL_SetFont(MMIMEMO_EDIT_LABEL1_CTRL_ID, MEMO_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
            GUILABEL_SetFont(MMIMEMO_EDIT_LABEL2_CTRL_ID, MEMO_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
            GUILABEL_SetFont(MMIMEMO_EDIT_LABEL3_CTRL_ID, MEMO_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T       src_ctrl_id = 0;

            if (PNULL != param)
            {
                src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif  
#endif
	   GUIEDIT_GetString(ctrl_id, &str_info);	
        if (is_add_new)
        {
            if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
            {
                //open the waiting window for save
                MemoOpenSaveWaitingWin(str_info);
            }
            else
            {
                //don't need to save it
                MMK_CloseWin(win_id);
            }
        }
        else
        {
            if (s_is_content_modify)
            {
                if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
                {
                    //save the file ,and need to update file array and list
					uint32  max_wstr_len = 0;
					max_wstr_len = MMIMEMO_CONTENT_MAX_LEN;
                    s_edit_text_save_buffer.wstr_ptr =SCI_ALLOC_APP((max_wstr_len + 1) * sizeof(wchar));
                    if (PNULL == s_edit_text_save_buffer.wstr_ptr)
                    {
                        SCI_TRACE_LOW("[mmimemo] HandleMemoEditWinMsg s_edit_text_save_buffer.wstr_ptr = PNULL");
						MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
						MMK_CloseWin(win_id);
                        return MMI_RESULT_TRUE;
                    }
					SCI_MEMSET(s_edit_text_save_buffer.wstr_ptr, 0x00, (max_wstr_len + 1) * sizeof(wchar));
                    MMIMEMO_GetContent(cur_memo_index, &s_edit_text_save_buffer,max_wstr_len, PNULL, PNULL);
					
					s_edit_text_filename_ptr = SCI_ALLOC_APP((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
                    if (PNULL == s_edit_text_filename_ptr)
                    {
                        SCI_TRACE_LOW("[mmimemo] HandleMemoEditWinMsg s_edit_text_filename_ptr = PNULL");
						MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
						MMK_CloseWin(win_id);
                        return MMI_RESULT_TRUE;
                    }
					SCI_MEMSET(s_edit_text_filename_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar));
                    MMIMEMO_GetFileName(cur_memo_index, s_edit_text_filename_ptr, MMIFILE_FULL_PATH_MAX_LEN + 1);
                    MMIMEMO_DelOne(cur_memo_index);
					s_is_content_edit = TRUE;
                    MemoOpenSaveWaitingWin(str_info);
                }
                else
                {
                    //the content is null, delete it
                    if (MMIMEMO_DelOne(cur_memo_index))
                    {
                        GUILIST_RemoveItem(list_ctrl_id, cur_memo_index);
                        SetMemoListWinTitle();
                    }
                    MMK_CloseWin(win_id);
                }
            }
            else
            {
                //don't need to save it
               MMK_CloseWin(win_id);
            }
        }
        break;


    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
			
    case MSG_CLOSE_WINDOW:
        s_is_content_modify = FALSE;
		s_is_content_edit = FALSE;
		if (s_edit_text_save_buffer.wstr_ptr != PNULL)
		{
			SCI_FREE(s_edit_text_save_buffer.wstr_ptr);
		}
		if (s_edit_text_filename_ptr != PNULL)
		{
			SCI_FREE(s_edit_text_filename_ptr);
		}
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        s_is_content_modify = TRUE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : 阅读窗口处理函数
//  Global resource dependence : none
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoReadWinMsg(
										MMI_WIN_ID_T        win_id, 
										MMI_MESSAGE_ID_E    msg_id, 
										DPARAM              param
										)
{
    MMI_RESULT_E              result   = MMI_RESULT_TRUE;
    //MMI_CTRL_ID_T   ctrl_id = MMIMEMO_READ_TEXT_CTRL_ID;
    //MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
    //MMI_STRING_T    str_info = {0};
    //uint16    cur_memo_index = GUILIST_GetCurItemIndex(list_ctrl_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
       MemoInitReadCtrl(win_id); 
#ifdef GUI_MID_SOFTKEY_USE_IMG
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_EDIT_ICON, 1, FALSE);
#endif
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T       src_ctrl_id = 0;

            if (PNULL != param)
            {
                src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (MMIMEMO_READ_TEXT_CTRL_ID == src_ctrl_id)
                {
                    MemoReadOne2Edit();
                    MMK_CloseWin(win_id);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif /* MMI_PDA_SUPPORT */

    case MSG_APP_OK:
    case MSG_CTL_OK:
        MemoButton2CallBack();
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif 
        MemoReadOne2Edit( );
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_LEFT:
        MemoButton1CallBack();
        break;

    case MSG_APP_RIGHT:
        MemoButton3CallBack();
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

//#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
//    case MSG_TP_PRESS_UP:
//        {
//            GUI_POINT_T	point = {0};
//            GUI_RECT_T	ctrl_rect = {0};
//            
//            point.x = MMK_GET_TP_X(param);
//            point.y = MMK_GET_TP_Y(param);
//            if (!MMK_GET_TP_SLIDE(param))
//            {
//                GUIAPICTRL_GetRect(MMIMEMO_READ_TEXT_CTRL_ID, &ctrl_rect);
//                if (GUI_PointIsInRect(point, ctrl_rect))
//                {
//                    MemoReadOne2Edit( );
//                    MMK_CloseWin(win_id);
//                }
//            }
//        }
//        break;
//#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleProcessWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:          
        s_del_memo_is_cancelled = FALSE;
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE); 
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;

    case MSG_APP_CANCEL: 
        s_del_memo_is_cancelled = TRUE;
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : delete selected memo
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void MemoDeleteMarkedItem(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in bookmark main win id
                             )
{
    uint16              item_index = 0;
    BOOLEAN             result    = FALSE;
    
    //get first mark items
    GUILIST_GetSelectedItemIndex(list_ctrl_id,&item_index,1);
        
    if(MMIMEMO_DelOne(item_index))
    {
        GUILIST_RemoveItem(list_ctrl_id, item_index);
        result = TRUE;
        MMK_PostMsg(win_id, MSG_MEMO_DELETE_FILE, &result, sizeof(result));
    }
    else
    {
        //SCI_TRACE_LOW:"MemoDeleteMarkedItem error;item_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMEMO_WINTABLE_1439_112_2_18_2_32_45_5,(uint8*)"d",item_index);
        result = FALSE;
    }

    if ( !result )
    {
        //prompt fail
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        
        //close wait win
        MMK_CloseWin(wait_win_id);
    }

    return;
}

/*****************************************************************************/
//  Description : handle delete selected memo confirm message
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleDeleteMemoCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:bookmark main win id
                              )
{
    uint16 mark_num = 0;

    if (is_success)
    {
        if(s_del_memo_is_cancelled)
        {
            //stop delete            
            SetMemoListWinTitle();
            MemoListUnUnMarkedAll(list_ctrl_id);
            MMK_CloseWin(wait_win_id);
        }
        else
        {
            //get mark num
            mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            if (0 == mark_num)
            {                
                SetMemoListWinTitle();                
                //set list not enable mark
                 GUILIST_SetTextListMarkable(list_ctrl_id,FALSE); 
                //prompt success
                 //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                  //close wait win
                MMK_CloseWin(wait_win_id);
            }
            else
            {
                //delete the next memo
                MemoDeleteMarkedItem(list_ctrl_id,wait_win_id,win_id);
            }
        }
    }
    else
    {
        //prompt fail
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
         //close wait win
        MMK_CloseWin(wait_win_id);
    }
}

/*****************************************************************************/
//  Description : to handle the  delete query window    
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelMemoQueryWin(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_CTRL_ID_T   list_ctrl_id = MMIMEMO_LIST_CTRL_ID;
            uint16 current_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            uint16 mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
            MMI_WIN_ID_T     wait_win_id =  MMIMEMO_DEL_WAIT_WIN_ID;

            if(0 == mark_num)
            {            
                if (MMIMEMO_DelOne(current_index))
                {
                    GUILIST_RemoveItem(list_ctrl_id, current_index);
                    SetMemoListWinTitle();
                    
                    //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                }
                else
                {
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                }
            }
            else
            {
                s_memo_del_total_num = mark_num;
                MMIPUB_OpenProgressWinByTextId(TXT_PUB_DELETING, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleProcessWinMsg);
                MMIPUB_SetProgressTotalSize(&wait_win_id, mark_num);
            
                MemoDeleteMarkedItem(list_ctrl_id,wait_win_id,MMIMEMO_LIST_WIN_ID);
            }
            MMK_CloseWin(MMIMEMO_READ_WIN_ID);
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : exit the memo module
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMEMO_Exit(void)
{
    //SCI_TRACE_LOW:"MMIAPIMEMO_Exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMEMO_WINTABLE_1577_112_2_18_2_32_46_6,(uint8*)"");

    MMK_CloseMiddleWin(MMIMEMO_WIN_ID_START, MMIMEMO_MAX_WIN_ID);
}

/*****************************************************************************/
//  Description : init memo module
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMEMO_InitModule(void)
{
    MMIMEMO_RegMenuGroup();
}

/*****************************************************************************/
//  Description : set memo option menu
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL void MemoSetOptMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,MMI_CTRL_ID_T list_ctrl_id)
{
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    uint16                  cur_index = 0;
    uint16                  mark_num = 0;
    MMI_MENU_GROUP_ID_T     mark_group_id = MENU_MEMO_MARK;
    const GUILIST_ITEM_T    *item_ptr = PNULL;

    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

    if (0 != mark_num)//only:delete and mark
    {
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_NEW_MEMO,   FALSE);
        GUIMENU_SetItemVisible(ctrl_id, group_id, ID_POPUP_MENU_EDIT_MEMO,   FALSE);
    }

    //mark sub menu
    item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_index);
    if (PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"MemoSetOptMenu get item error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMEMO_WINTABLE_1618_112_2_18_2_32_46_7,(uint8*)"");
        return;
    }
    if (GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
    {
        GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_MEMO_MARKED,TRUE);
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_MEMO_UNMARKED,TRUE);
    }

    if (mark_num > 0)
    {
        if (mark_num >= GUILIST_GetTotalItemNum(list_ctrl_id))
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_MEMO_MARKED_ALL,TRUE);
        }
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_POPUP_MENU_MEMO_UNMARKED_ALL,TRUE);
    }
}

/*****************************************************************************/
// 	Description : handle memo list mark option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    uint16      cur_index = 0;
    uint32      list_num = GUILIST_GetTotalItemNum(list_ctrl_id);
    uint16      mark_num =0;
    
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);//get mark num
    if (0 == mark_num)
    {
        GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);//set list enable mark
        GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)list_num);//set mark max number
    }
    
    //mark current item index
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    GUILIST_SetSelectedItem(list_ctrl_id,cur_index,TRUE);
}

/*****************************************************************************/
// 	Description : handle memo list mark cancel option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListUnMarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 )
{
    uint16      cur_index = 0;
    uint16      mark_num = 0;

    //get item index
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

    //cancel mark item
    GUILIST_SetSelectedItem(list_ctrl_id,cur_index,FALSE);

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
    }
}

/*****************************************************************************/
// 	Description : handle memo list mark all option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListUnMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  )
{
    uint16      mark_num = 0;
    uint32      list_num = GUILIST_GetTotalItemNum(list_ctrl_id);

    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);

        //set mark max number
        GUILIST_SetMaxSelectedItem(list_ctrl_id,(uint16)list_num);
    }

    //mark all item
    GUILIST_SetAllSelected(list_ctrl_id,TRUE);
}

/*****************************************************************************/
// 	Description : handle memo list mark all cancel option
//	Global resource dependence : none
//  Author: paul.huang
//	Note:
/*****************************************************************************/
LOCAL void MemoListUnUnMarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    )
{
    GUILIST_SetAllSelected(list_ctrl_id,FALSE);

    //set list enable mark
    GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
}

/*****************************************************************************/
//  Description : handle memo popmenu window message
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoPopMenuWinMsg(MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param)
{
    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIMEMO_POPMEMU_CTRL_ID;    
    MMI_CTRL_ID_T               list_ctrl_id           = MMIMEMO_LIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MemoSetOptMenu(win_id, ctrl_id, list_ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);						
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch(menu_id) 
        {
            case ID_POPUP_MENU_NEW_MEMO:                
                AddMemoTex(list_ctrl_id);
                break;

            case ID_POPUP_MENU_EDIT_MEMO:                
                MemoReadOne2Edit();
                break;

            case ID_POPUP_MENU_DEL_MEMO:                
                MemoButton2CallBack();
                break;

            case ID_POPUP_MENU_MEMO_MARKED:
                MemoListMarked(list_ctrl_id);
                break;

            case ID_POPUP_MENU_MEMO_UNMARKED:
                MemoListUnMarked(list_ctrl_id);
                break;

            case ID_POPUP_MENU_MEMO_MARKED_ALL:
                MemoListUnMarkedAll(list_ctrl_id);
                break;

            case ID_POPUP_MENU_MEMO_UNMARKED_ALL:
                MemoListUnUnMarkedAll(list_ctrl_id);
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
//  Description : MMIMEMO_IsContentEdit
//  Global resource dependence : 
//  Author:guodong.liu
//  Note: 是否在编辑界面
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_IsContentEdit(void)
{
    return s_is_content_edit;
}

/*****************************************************************************/
//  Description : get s_edit_text_filename_ptr
//  Global resource dependence : 
//  Author: guodong.liu
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIMEMO_GetEdittext_Ptr(void)
{
	
    return s_edit_text_filename_ptr;
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif/*MMI_MEMO_SUPPORT*/




/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
