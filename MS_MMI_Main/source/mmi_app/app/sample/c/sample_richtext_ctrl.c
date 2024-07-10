/*****************************************************************************
** File Name:      hello_test_richtext                                       *
** Author:                                                                   *
** Date:           5/8/2009                                                  *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 5/2009         James.Zhang       Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "std_header.h"

#ifdef MMI_MOCOR_SAMPLE

#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "sample_main.h"
#include "guifont.h"
#include "mmi_textfun.h"
#include "sample_text.h"
#include "sample_image.h"
#include "sample_menutable.h"
#include "sample_id.h"
#include "sample_nv.h"
#include "mmitheme_pos.h"
#include "guires.h"

#include "guirichtext.h"
#ifdef PIC_EDITOR_SUPPORT
#include "mmipic_image.h"
#endif
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmiwallpaper_image.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMI_HELLO_RICHTEXT_NUM      20
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : richtext控件测试窗口的回调函数
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestRichtextWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            );

/*****************************************************************************/
//  Description : 加入测试的说明
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddTestTips(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
//test frame
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemTestFrame(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemNull(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemImgRes(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemImgBuf(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemImgFile(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/

LOCAL void AddItemStrRes(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemStrBuf(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemSetAlign(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUIRICHTEXT_ALIGN_E align_type);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemStrParse(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemStrHyper(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemSetStrTag(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItem(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
// richtext控件测试窗口
WINDOW_TABLE( MMI_TEST_RICHTEXT_WIN_TAB ) =
{
    WIN_ID( MMI_TEST_RICHTEXT_WIN_ID ),
    WIN_FUNC((uint32)HandleTestRichtextWinMsg ),
    WIN_TITLE(TXT_HELLO_TEST_RICHTEXT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    CREATE_RICHTEXT_CTRL(MMI_TEST_RICHTEXT_CTRL_ID),
    
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建richtext测试窗口
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateRichtextWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_RICHTEXT_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : richtext控件测试窗口的回调函数
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestRichtextWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_RICHTEXT_CTRL_ID;
    uint16        index   = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        AddItem(win_id, ctrl_id);
        //GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
        GUIRICHTEXT_SetBorder(ctrl_id, GUIRICHTEXT_FRAME_NORMAL);
        
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;
                

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;

    case MSG_APP_1:
    {
        static BOOLEAN is_resized = FALSE;
        GUI_RECT_T new_rect   = MMITHEME_GetClientRect();
        is_resized = !is_resized;
        if(is_resized)
        {
            new_rect.right *= 4;
            new_rect.right /= 5;
            new_rect.bottom *= 4;
            new_rect.bottom /= 5;
        }
        GUIRICHTEXT_SetRect(ctrl_id, new_rect);
        GUIRICHTEXT_Update(ctrl_id);
        break;
    }
        
    case MSG_APP_2:
    {
        static BOOLEAN is_set_to_max = FALSE;
        uint8 space = 0;
        is_set_to_max = !is_set_to_max;
        if(is_set_to_max)
        {
            space = GUIRICHTEXT_SETTINGS_SPACE_LIMIT;
        }
        
        GUIRICHTEXT_SetCharSpace(ctrl_id, space);
        GUIRICHTEXT_SetLineSpace(ctrl_id, space);
        GUIRICHTEXT_SetImgTxtSpace(ctrl_id, space);
        GUIRICHTEXT_SetItemSpace(ctrl_id, space);
        GUIRICHTEXT_SetBorderSpace(ctrl_id, space);
        GUIRICHTEXT_SetTxtMargin(ctrl_id, space);
        GUIRICHTEXT_Update(ctrl_id);
        
        break;
    }

    case MSG_APP_3:
    {
        GUIRICHTEXT_ITEM_T item = {0};
        uint16 index            = 0;
        const wchar w_str[]     = L"The current focus item will be modified to this item after you press the key 3. Set font to song10, color to red and align to left. In rect and a FISH at right.";
        
        item.img_type = GUIRICHTEXT_IMAGE_RES;
#ifdef PIC_EDITOR_SUPPORT
        item.img_data.res.id = IMAGE_PIC_ANIMAL_FISH;
#else
        item.img_data.res.id = IMAGE_NULL;
#endif

        item.text_type = GUIRICHTEXT_TEXT_BUF;
        item.text_data.buf.str_ptr = w_str;
        item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
        item.text_set_font = TRUE;
        item.setting_font = SONG_FONT_10;
        item.text_set_font_color = TRUE;
        item.setting_font_color = MMI_RED_COLOR;
        item.align_type = GUIRICHTEXT_ALIGN_LEFT;
        item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
        
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &index);

        GUIRICHTEXT_ModifyItem(ctrl_id, index, &item);
        GUIRICHTEXT_Update(ctrl_id);
        break;
    }
    case MSG_APP_4:
    {
        uint16 index = 0;
        
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &index);
        GUIRICHTEXT_DeleteItem(ctrl_id, index);
        GUIRICHTEXT_Update(ctrl_id);
        break;
    }
    case MSG_APP_5:
    {
        static BOOLEAN is_show_scroll = FALSE;

        is_show_scroll = !is_show_scroll;
        
        //GUIRICHTEXT_SetNeedScroll(ctrl_id, is_show_scroll);
        GUIRICHTEXT_Update(ctrl_id);
        break;
    }
    case MSG_APP_6:
    {
        static BOOLEAN is_focus_mode = FALSE;

        is_focus_mode = !is_focus_mode;
        
        GUIRICHTEXT_SetFocusMode(ctrl_id, (is_focus_mode?GUIRICHTEXT_FOCUS_SHIFT_BASIC:GUIRICHTEXT_FOCUS_SHIFT_NONE));
        GUIRICHTEXT_Update(ctrl_id);
        break;
    }
    case MSG_APP_7:
    {
        GUIRICHTEXT_ITEM_T item = {0};
        uint16 index            = 0;
        
        GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &index);
        GUIRICHTEXT_GetItem(ctrl_id, index, &item);
        GUIRICHTEXT_ModifyItem(ctrl_id, 1, &item);
        GUIRICHTEXT_Update(ctrl_id);
        break;
    }
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    //case MSG_CTL_RICHTEXT_SWITCHFOCUS:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    {
        static int32 last_state = -1; //0 - key ok, 1 - switch focus 2 pen ok
        int32 cur_state = -1;
        GUIRICHTEXT_ITEM_T item = {0};
        LOCAL const wchar w_str_ko[]     = L"Key OK!";
        LOCAL const wchar w_str_sf[]     = L"Switch Focus";
        LOCAL const wchar w_str_po[]     = L"Pen OK!";
        const wchar *w_str[]    = {w_str_ko, w_str_sf, w_str_po};
        if(msg_id == MSG_CTL_PENOK)
        {
            cur_state = 2;
        }
//        else if(msg_id == MSG_CTL_RICHTEXT_SWITCHFOCUS)
//        {
//            cur_state = 1;
//        }
        else
        {
            cur_state = 0;
        }

        if(last_state == cur_state)
        {
            break;
        }

        last_state = cur_state;
        
        item.img_type = GUIRICHTEXT_IMAGE_RES;
#ifdef PIC_EDITOR_SUPPORT
        item.img_data.res.id = IMAGE_PIC_ANIMAL_CAT;
#else
        item.img_data.res.id = IMAGE_NULL;
#endif

        item.text_type = GUIRICHTEXT_TEXT_BUF;
        item.text_data.buf.str_ptr = w_str[cur_state];
        item.text_data.buf.len = MMIAPICOM_Wstrlen(item.text_data.buf.str_ptr);
        item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
        item.text_set_font = TRUE;
        item.setting_font = SONG_FONT_20;
        
        GUIRICHTEXT_ModifyItem(ctrl_id, 1, &item);
        GUIRICHTEXT_Update(ctrl_id);

        break;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


//add a test tips
/*****************************************************************************/
//  Description : 加入测试的说明
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddTestTips(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    const wchar w_str[]     = L"Welcome, the cat in next item will tell you sth. about focus and ok msg\n1 - Set rect to 16/25 or Recover\n2 - Set all space to Max or Zero\n3 - Modify focus item\n4 - Delete Focus irem\n5 - hide/show scroll bar\n6 - enable/disable focus mode\n7 - modify tip item to focus item";
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_12;
    item.text_set_font_color = TRUE;
    item.setting_font_color = MMI_YELLOW_COLOR;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}
//test frame
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemTestFrame(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    const wchar w_str[]     = L"ALL frames in item will draw. And set focus to (GUIRICHTEXT_TAG_IMG_NORMAL | GUIRICHTEXT_TAG_TXT_NORMAL)";
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_SECMENU_ICON_CL_COST;
    item.img_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_12;
    
    item.tag_type = GUIRICHTEXT_TAG_IMG_NORMAL | GUIRICHTEXT_TAG_TXT_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}

//add a null item
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemNull(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    
    item.img_type = GUIRICHTEXT_IMAGE_NONE;
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    
    item.tag_type = GUIRICHTEXT_TAG_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}

//add a res img
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemImgRes(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    const wchar w_str[]     = L"A res img. Set display size to 128x96 and we will draw the rect, the image will get focus.";
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_SECMENU_ICON_CL_COST;
    item.img_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    item.img_set_size = TRUE;
    item.img_setting_width = 128;
    item.img_setting_height = 96;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_10;
    
    item.tag_type = GUIRICHTEXT_TAG_IMG_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}


//add a buf img
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemImgBuf(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item    = {0};
    uint16 index               = 0;
    GUIRES_IMG_SRC_T img_src   = {0};
    GUIRES_IMG_INFO_T info_ptr = {0};
    const wchar w_str[]        = L"A buf img.(Resize it, or the img will be too big to diaplay next img)";
    uint16 len                 = MMIAPICOM_Wstrlen(w_str);
    
    item.img_type = GUIRICHTEXT_IMAGE_BUF;
#ifndef MMI_LOW_MEMORY_RESOURCE    
    img_src.data_ptr = MMI_GetLabelImage(IMAGE_WALLPAPER_1, win_id, &(img_src.data_size));
#else
    img_src.data_ptr = MMI_GetLabelImage(IMAGE_THEME_BG, win_id, &(img_src.data_size));
#endif
    GUIRES_GetImgInfo(&img_src, &info_ptr);
    item.img_data.buf.buf_ptr = img_src.data_ptr;
    item.img_data.buf.size = img_src.data_size;
    item.img_data.buf.width = info_ptr.width;
    item.img_data.buf.height = info_ptr.height;
    item.img_set_size = TRUE;
    item.img_setting_width = 128;
    item.img_setting_height = 96;
    item.img_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);;
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_10;
    
    item.tag_type = GUIRICHTEXT_TAG_IMG_NORMAL | GUIRICHTEXT_TAG_TXT_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}


//add a file img
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemImgFile(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item  = {0};
    uint16 index             = 0;
    const wchar w_filename[] = L"D:\\Photos\\rill.jpg";
    const wchar w_str[]      = L"A file img, D:\\Photos\\rill.jpg, if no file, will display X";
    
    item.img_type = GUIRICHTEXT_IMAGE_FILE;
    item.img_data.file.filename_ptr = w_filename;
    item.img_data.file.filename_len = MMIAPICOM_Wstrlen(w_filename);
    item.img_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);

    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_10;
    
    item.tag_type = GUIRICHTEXT_TAG_IMG_NORMAL | GUIRICHTEXT_TAG_TXT_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}


//add a res string
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemStrRes(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    
    item.text_type = GUIRICHTEXT_TEXT_RES;
    item.text_data.res.id = TXT_MOD_DYNA_ISVALID;
    

    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}

//add a long buffer string
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemStrBuf(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    const wchar w_str[]     = L"The up item is a string from resource, and this item is a string from buffer. Here we can write 4096 characters in maximal. String 01234567 will be as a phone number, and pig@hell.org will be a email address, and https://foo.cn will be a url. We will recognize some sepcial strings, but not all. The following items, we will test align style.";
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_12;
    
    item.tag_type = GUIRICHTEXT_TAG_PARSE;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}


//add a align
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemSetAlign(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUIRICHTEXT_ALIGN_E align_type)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    LOCAL const wchar w_str_0[]   = L"DOG at default.";
    LOCAL const wchar w_str_1[]   = L"DOG at left side.";
    LOCAL const wchar w_str_2[]   = L"DOG at up side.";
    LOCAL const wchar w_str_3[]   = L"DOG at right side.";
    LOCAL const wchar w_str_4[]   = L"DOG at down side.";
    LOCAL const wchar w_str_5[]   = L"Where is the dog?";
    
    const wchar *w_str[]    = {w_str_0, w_str_1, w_str_2, w_str_3, w_str_4, w_str_5};
    
    
    
    if (align_type > GUIRICHTEXT_ALIGN_INVALID)
    {
        align_type = GUIRICHTEXT_ALIGN_INVALID;
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
#ifdef PIC_EDITOR_SUPPORT
        item.img_data.res.id = IMAGE_PIC_ANIMAL_DOG;
#else
        item.img_data.res.id = IMAGE_NULL;
#endif
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str[align_type];
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str[align_type]);
    
    item.tag_type = GUIRICHTEXT_TAG_NORMAL;
    item.align_type = align_type;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}

//add a general parse sting
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemStrParse(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    const wchar w_str[]     = L"http://202.38.64.10:8080, http://202.38.64.256:8080,http://202.38.64.64.10:8080,In a Parse string, 5 elements will be mark.a email: pig@hell.org . a url: https://foo.com:80/test/test/test.html?x=y&z=z . a phone number: 13814141414. a wrong url for port: http://foo.com:88888, the wrong port set as a phone number.02158585888 will not as a phone number because it the the sixth.";
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_12;
    
    item.tag_type = GUIRICHTEXT_TAG_PARSE;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}
//add a hyper text sting
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemStrHyper(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    const wchar w_str[]     = L"the string will as a url\n(<u>a hyper url</u>)\ntest \\<s>will show all</s>";
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_12;
    
    item.tag_type = GUIRICHTEXT_TAG_HYPER;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}

//add a sting as url
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItemSetStrTag(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_ITEM_T item = {0};
    uint16 index            = 0;
    const wchar w_str[]     = L"this text will be set to a url because it set it to be, even it nothing about a url. You can set it to a phone number, email or just a string.";
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = w_str;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(w_str);

    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_10;
    
    item.tag_type = GUIRICHTEXT_TAG_URL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(ctrl_id, &item, &index);
    
    return;
}

//add item
/*****************************************************************************/
//  Description : 加入示例条目
//  Global resource dependence : NONE
//  Author:lianxiang.zhou
//  note:
/*****************************************************************************/
LOCAL void AddItem(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    AddTestTips(win_id, ctrl_id);
    //tip for switch focus and PenOk
    AddItemNull(win_id, ctrl_id);
    AddItemNull(win_id, ctrl_id);
    AddItemTestFrame(win_id, ctrl_id);
    
    AddItemImgRes(win_id, ctrl_id);
    AddItemImgBuf(win_id, ctrl_id);
    AddItemImgFile(win_id, ctrl_id);
    AddItemStrHyper(win_id, ctrl_id);
    AddItemStrParse(win_id, ctrl_id);
    AddItemSetStrTag(win_id, ctrl_id);
    
    AddItemStrRes(win_id, ctrl_id);
    AddItemStrBuf(win_id, ctrl_id);
    AddItemSetAlign(win_id, ctrl_id, 0);
    AddItemSetAlign(win_id, ctrl_id, 1);
    AddItemSetAlign(win_id, ctrl_id, 2);
    AddItemSetAlign(win_id, ctrl_id, 3);
    AddItemSetAlign(win_id, ctrl_id, 4);
    AddItemSetAlign(win_id, ctrl_id, 5);
    
    
    return;
}



#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
