/*****************************************************************************
** File Name:      ctrlbaseflex_add_del.c                                    *
** Author:         hua.fang                                                  *
** Date:           09/14/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "mmi_gui_trc.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define GUIEDIT_LIST_ADD_CHAR_MAX       4   //一次添加LF and separator的最大长度

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
//  Description : add commit string and underline string and modify cursor info
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddString (//is change
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr,   //in:
    BOOLEAN             *is_delete_ptr  //in:
);

/*****************************************************************************/
//  Description : get commit and underline length by string max length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
//    while list style, it will get string in GUIEDIT_GetListCommitStr
/*****************************************************************************/
LOCAL void GetCommitString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T    *add_str_ptr,       //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T *actual_str_ptr,// out
    uint16               *delete_len_ptr     //in/out:
);

/*****************************************************************************/
//  Description : get add actual string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetAddActualString (
    GUIEDIT_ADD_STR_T         *add_str_ptr,   //in:
    GUIEDIT_ADD_ACTUAL_STR_T  *actual_str_ptr //in/out:
);

/*****************************************************************************/
//  Description : move sting for add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveStringForAdd (//is add
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               *add_str_ptr,
    uint16              add_str_len,    //in:include LF and separator total len
    uint16              *move_len_ptr   //in/out:
);

/*****************************************************************************/
//  Description : get edit update info for add/delete string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetEditUpdateInfo (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    BOOLEAN             is_update_all,          //in:
    uint16              start_line_index,       //in:
    int32               pre_top_line_offset,    //in:
    uint32              pre_total_height,       //in:
    GUIEDIT_UPDATE_T    *update_ptr             //in/out:
);

/*****************************************************************************/
//  Description : delete string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 DeleteString (//delete string actual len
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle edit delete string after full
//  Global resource dependence :
//  Author: Jassmine
//  Note:
//    only effective in phonenum style
/*****************************************************************************/
LOCAL BOOLEAN DeleteStringAfterFull (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              str_delete_len
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : edit add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_AddString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    GUIEDIT_UPDATE_T    *update_ptr,        //in/out:
    GUIEDIT_ADD_STR_T   *add_str_ptr,       //in:
    GUIEDIT_LIST_ITEM_T *list_item_ptr,
    uint16              list_item_count
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_update_all = FALSE;
    uint16      highlight_min_pos = 0;
    uint16      highlight_line_index = 0;
    uint16      pre_total_line_num = 0;
    uint16      pre_page_line_num = 0;
    uint16      start_line_index = 0;
    uint16      top_line_index = 0;
    uint16      page_line_num = 0;
    uint16      pre_str_len = 0;
    int32       pre_top_line_offset = 0;
    uint32      pre_total_height = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get add string len
    if ( ( (PNULL != add_str_ptr) &&
            ( (PNULL != add_str_ptr->commit_ptr) || (PNULL != add_str_ptr->underline_ptr)) &&
            (0 < (add_str_ptr->commit_len + add_str_ptr->underline_len)))
        || (PNULL != list_item_ptr && list_item_count > 0))
    {
        //get highlight line index
        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
        highlight_line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, highlight_min_pos);

        //get current page top line index
        pre_top_line_offset = baseedit_ctrl_ptr->top_line_offset;

        //get current page number
        pre_total_height = BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr);

        //get current total line number
        pre_total_line_num = baseedit_ctrl_ptr->total_line_num;

        //pre_page_line_num = BASEEDIT_GetPageLineNum(baseedit_ctrl_ptr);
        //get current page number
        top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);
        page_line_num = BASEEDIT_GetPageLineNum(baseedit_ctrl_ptr);
        pre_page_line_num = (uint16)MIN((baseedit_ctrl_ptr->total_line_num - top_line_index), page_line_num);

        pre_str_len = baseedit_ctrl_ptr->str_len;

        if (PNULL != list_item_ptr && 0 < list_item_count)
        {
            int32 i = 0;

            for (i = 0; i < list_item_count; i++)
            {
                // 必须放在这里，因为每次add_str的值必须清空
                GUIEDIT_ADD_STR_T   add_str = {0};

                add_str.commit_ptr = list_item_ptr[i].display_str_ptr;
                add_str.commit_len = list_item_ptr[i].display_str_len;

                if (list_item_ptr[i].user_str_len > 0
                        //&& list_item_ptr[i].user_str_len <= edit_ctrl_ptr->add_data.list.item_max_len
                        && PNULL != list_item_ptr[i].user_str_ptr
                        && list_item_ptr[i].is_object)
                {
                    add_str.is_list_item_object = TRUE;

                    add_str.user_data_ptr = list_item_ptr[i].user_str_ptr;
                    add_str.user_data_len = list_item_ptr[i].user_str_len;
                }
                else
                {
                    add_str.is_list_item = TRUE;
                }

                // TODO: is_update_all是否会冲掉呢?
                if (AddString (baseflex_ctrl_ptr, &add_str, &is_update_all))
                {
                    result = TRUE;
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            result = AddString (baseflex_ctrl_ptr, add_str_ptr, &is_update_all);
        }

        if ( (PNULL != add_str_ptr) &&
                (0 < add_str_ptr->underline_len) &&
                (!baseedit_ctrl_ptr->highlight.is_underline) &&
                (0 != baseedit_ctrl_ptr->im_ctrl_handle))
        {
            //clear im
            GUIIM_ClearState (baseedit_ctrl_ptr->im_ctrl_handle);
        }

        //add string
        if (result)
        {
            VTLBASEFLEX_OnTextAdded(
                    baseflex_ctrl_ptr,
                    add_str_ptr);

            //1. progress is change,creat or destroy progress control
            //2. when add item object in list, need reparse text
            //3. when have default text which is two line length, redraw all text
            if ((BASEFLEX_CreateOrDestroyPrgCtrl (baseflex_ctrl_ptr))
                || (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir)
                || (GUIEDIT_TYPE_LIST == baseedit_ctrl_ptr->type &&
                    baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
                || (0 == pre_str_len && 0 != baseedit_ctrl_ptr->str_len && 0 != baseflex_ctrl_ptr->default_str_len))
            {
                is_update_all = TRUE;
            }

            //get need update line info start line index
            if (is_update_all)
            {
                start_line_index = 0;
            }
            else
            {
                start_line_index = highlight_line_index;

                //换行的处理需要根据上一行重新计算
                if (0 < start_line_index)
                {
                    start_line_index--;
                }
            }

            //set line info,include start position,total line number and top line index
            if (VTLBASEFLEX_ParseText (baseflex_ctrl_ptr, start_line_index))
            {
                //cr221179,横竖屏切换后,添加字符,rect还没有更新,不做刷新
                if ((baseedit_ctrl_ptr->is_rect_adaptive) &&
                    (pre_page_line_num != pre_total_line_num))
                {
                    update_ptr->update_mode = GUIEDIT_UPDATE_NONE;
                    //SCI_TRACE_LOW:"EditAddStr: edit rect is wrong,not update!"
                    SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_2929_112_2_18_3_15_40_119, (uint8*) "");
                }
                else if (BASEEDIT_NotifyLineMsg (baseedit_ctrl_ptr, pre_total_line_num))
                {
                    //notify parent edit line number change,not update
                    update_ptr->update_mode = GUIEDIT_UPDATE_NONE;
                }
                else
                {
                    //get edit update info
                    GetEditUpdateInfo (
                            baseflex_ctrl_ptr,
                            is_update_all,
                            start_line_index,
                            pre_top_line_offset,
                            pre_total_height,
                            update_ptr);
                }
            }

            //reset cursor x coordinate
            baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

            //config im
            VTLBASEEDIT_ConfigImKey (baseedit_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
            // 恢复到NORMAL状态
            BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
#endif

            //notify parent change string len
            BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_EDITBOX_UPDATE_STRNUM);
        }
    }

#ifdef CMCC_UI_STYLE
    if(!result && (baseedit_ctrl_ptr->str_len >= baseedit_ctrl_ptr->str_max_len))
    {
        baseflex_ctrl_ptr->flex_theme.exceedMaxLenFun();
    }
#endif

    return (result);
}

/*****************************************************************************/
//  Description : edit delete string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_DelString (
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr, //in:
    GUIEDIT_UPDATE_T       *update_ptr     //in/out:
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_update_all = FALSE;
    uint16      highlight_len = 0;
    uint16      highlight_min_pos = 0;
    uint16      pre_total_line_num = 0;
    uint16      delete_str_len = 0;
    uint16      start_line_index = 0;
    int32       pre_top_line_offset = 0;
    uint32      pre_total_height = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get highlight len and min pos
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    //have chars before cursor or highlight len is not equal 0
    if ((0 < baseedit_ctrl_ptr->cursor.cur_pos) || (0 < highlight_len))
    {
        //get current page top line index
        pre_top_line_offset = baseedit_ctrl_ptr->top_line_offset;

        pre_total_height = BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr);

        //get current total line number
        pre_total_line_num = baseedit_ctrl_ptr->total_line_num;

        //delet string
        delete_str_len = DeleteString (baseflex_ctrl_ptr);

        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

        //set cursor position and string length
        baseedit_ctrl_ptr->str_len = (uint16) (baseedit_ctrl_ptr->str_len - delete_str_len);

		if (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type)
		{
			if ((baseedit_ctrl_ptr->is_first_plus) && 
				(1 == baseedit_ctrl_ptr->cursor.cur_pos) && 
				('+' != baseedit_ctrl_ptr->str_ptr[0]))
			{
				baseedit_ctrl_ptr->is_first_plus = 0;		
			}
		}

        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, highlight_min_pos);

        //set no highlight
        BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, highlight_min_pos);

        VTLBASEFLEX_OnTextDeleted(baseflex_ctrl_ptr);

        //progress is change,creat or destroy progress control
        // in list style, the current cursor position will in object, and it may not be
        // the start of deleted string, it will make ParseEditText wrong. so reparse it.
        if ((BASEFLEX_CreateOrDestroyPrgCtrl (baseflex_ctrl_ptr)) ||
            (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir) ||
            (GUIEDIT_TYPE_LIST == baseedit_ctrl_ptr->type &&
                 baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos))
        {
            is_update_all = TRUE;
        }

        //get need update line info start line index
        if (is_update_all)
        {
            start_line_index = 0;
        }
        else
        {
            start_line_index = BASEEDIT_GetCursorLineIndex (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

            //换行的处理需要根据上一行重新计算
            if (0 < start_line_index)
            {
                start_line_index--;
            }
        }

        //set line info,include start position,total line number and top line index
        VTLBASEFLEX_ParseText (baseflex_ctrl_ptr, start_line_index);

        //notify parent edit line number change
        if (BASEEDIT_NotifyLineMsg (baseedit_ctrl_ptr, pre_total_line_num))
        {
            //not update
            update_ptr->update_mode = GUIEDIT_UPDATE_NONE;
        }
        else
        {
            //get edit update info
            GetEditUpdateInfo (
                    baseflex_ctrl_ptr,
                    is_update_all,
                    start_line_index,
                    pre_top_line_offset,
                    pre_total_height,
                    update_ptr);
        }

        //reset cursor x coordinate
        baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

        //config im
        VTLBASEEDIT_ConfigImKey (baseedit_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
        // 恢复到NORMAL状态
        BASEFLEX_ChangeDragState (baseflex_ctrl_ptr, GUIEDIT_DRAGSTATE_NORMAL);
#endif

        //notify parent change string len
        BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_EDITBOX_UPDATE_STRNUM);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Check if current character is separate token
//  Global resource dependence : 
//  Author: Cheney Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPreviousCharSeperateToken (CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr)
{
    wchar cursor_char = 0;
    uint16 cursor_pos = 0;

    cursor_pos = baseflex_ctrl_ptr->baseedit_ctrl.cursor.cur_pos;
    if ((cursor_pos == 0))
    {
        return TRUE;
    }

    cursor_pos--;

    if (cursor_pos > 1 && cursor_pos <=  baseflex_ctrl_ptr->baseedit_ctrl.str_len)
    {
        cursor_char = baseflex_ctrl_ptr->baseedit_ctrl.str_ptr[cursor_pos];
    }

    while( cursor_pos > 1)
    {
        if (MMIAPIIM_WordSeperateChar(cursor_char))
        {
            return TRUE;
        }
        else if(cursor_char == ' ')
        {
            cursor_pos--;
            if (cursor_pos <= baseflex_ctrl_ptr->baseedit_ctrl.str_len)
            {
                cursor_char = baseflex_ctrl_ptr->baseedit_ctrl.str_ptr[cursor_pos];
            }
        }
        else
        {
            return FALSE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : Check if current input mode is Abc mode
//  Global resource dependence : 
//  Author: Cheney Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsModeLeadUpper(CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr)
{
    if(GUIIM_MODE_LEAD_UPPER == GUIIM_GetCapital(baseflex_ctrl_ptr->baseedit_ctrl.im_ctrl_handle))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : add commit string and underline string and modify cursor info
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddString (//is change
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr,   //in:
    BOOLEAN             *is_delete_ptr  //in:
)
{
    BOOLEAN                     result = FALSE;
    BOOLEAN                     is_add_str = FALSE;
    uint16                      original_commit_len = 0;
    uint16                      original_underline_len = 0;
    uint16                      add_underline_len = 0;
    uint16                      str_delete_len = 0;
    uint16                      highlight_min_pos = 0;
    uint16                      move_str_len = 0;
    GUIEDIT_ADD_ACTUAL_STR_T    add_actual_str = {0};
    CTRLBASE_OBJ_T              *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T          *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != add_str_ptr); /*assert verified*/
    if (PNULL == add_str_ptr)
    {
        return result;
    }

    //set commit underline len
    original_commit_len = add_str_ptr->commit_len;
    original_underline_len = add_str_ptr->underline_len;

    //check edit string
    if ((BASEFLEX_CheckEditString (baseflex_ctrl_ptr, add_str_ptr->commit_ptr, &add_str_ptr->commit_len)) &&
        (BASEFLEX_CheckEditString (baseflex_ctrl_ptr, add_str_ptr->underline_ptr, &add_str_ptr->underline_len)))
    {
        //get commit length and underline length
        GetCommitString (baseflex_ctrl_ptr, add_str_ptr, &add_actual_str, &str_delete_len);

        //set add underline len
        add_underline_len = add_str_ptr->underline_len;

        //add
        if ((0 < add_str_ptr->commit_len) || (0 < add_str_ptr->underline_len))
        {

            // make the first leader character Upper case.
            if(TRUE == IsModeLeadUpper(baseflex_ctrl_ptr))
            {
                if(IsPreviousCharSeperateToken(baseflex_ctrl_ptr))
                {
                    add_actual_str.str_ptr[0] = MMIAPICOM_Wchartoupper(add_actual_str.str_ptr[0]);                
                }
            }
            
            //delete string after edit full
            *is_delete_ptr = DeleteStringAfterFull (baseflex_ctrl_ptr, str_delete_len);

            //get highlight min pos and len
            highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

            //get move direction and move string length
            VTLBASEFLEX_BeforeAddString(baseflex_ctrl_ptr, &add_actual_str);

            is_add_str = MoveStringForAdd (
                    baseflex_ctrl_ptr,
                    add_actual_str.str_ptr,
                    add_actual_str.str_len,
                    &move_str_len);

            VTLBASEFLEX_AfterAddString(baseflex_ctrl_ptr, add_str_ptr, &add_actual_str);

            //set edit string length
            if (is_add_str)
            {
                baseedit_ctrl_ptr->str_len = (uint16) (baseedit_ctrl_ptr->str_len + move_str_len);
            }
            else
            {
                baseedit_ctrl_ptr->str_len = (uint16) (baseedit_ctrl_ptr->str_len - move_str_len);
            }

            //set cursor position
            if (add_str_ptr->is_list_item || add_str_ptr->is_list_item_object)
            {
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (highlight_min_pos + move_str_len));
            }
            else
            {
                // 确保单个输入时放到;号之前
                VTLBASEEDIT_SetCursorPos (
                    baseedit_ctrl_ptr,
                    (uint16) (highlight_min_pos + add_actual_str.len_before_cursor + add_str_ptr->commit_len + add_str_ptr->underline_len));
            }

            //set highlight
            SCI_ASSERT (baseedit_ctrl_ptr->cursor.cur_pos >= add_str_ptr->underline_len); /*assert verified*/

            if (0 < add_str_ptr->underline_len)
            {
                baseedit_ctrl_ptr->highlight.is_underline = TRUE;
                //SCI_TRACE_LOW:"AddString:add underline string!"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_3092_112_2_18_3_15_40_120, (uint8*) "");
            }
            else
            {
                baseedit_ctrl_ptr->highlight.is_underline = FALSE;
            }

            VTLBASEEDIT_SetHighlight (
                baseedit_ctrl_ptr,
                (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - add_str_ptr->underline_len),
                baseedit_ctrl_ptr->cursor.cur_pos,
                SETHIGHLIGHT_BOTH);

            result = TRUE;
        }

        // BUGBUG: 录取的个数大于传入的个数，有可能是被拦截的字符，暂时只给短信
        // 编辑框使用，这样就足够了
        if (original_commit_len + original_underline_len > add_actual_str.str_len)
        {
            BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_EDITBOX_MAXTEXT);
        }

        //free memory
        if (PNULL != add_actual_str.str_ptr)
        {
            SCI_FREE (add_actual_str.str_ptr);
        }
    }
	if (GUIEDIT_TYPE_PHONENUM == baseedit_ctrl_ptr->type)
	{
		if ('+' == baseedit_ctrl_ptr->str_ptr[0])
		{
			baseedit_ctrl_ptr->is_first_plus = TRUE;
		}
		else
		{
			baseedit_ctrl_ptr->is_first_plus = FALSE;
		}
	}

    return (result);
}

/*****************************************************************************/
//  Description : get commit and underline length by string max length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
//    while list style, it will get string in GUIEDIT_GetListCommitStr
/*****************************************************************************/
LOCAL void GetCommitString (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,       //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,// out
    uint16                      *delete_len_ptr     //in/out:
)
{
    BOOLEAN is_got_string = FALSE;

    is_got_string = VTLBASEFLEX_CheckCommitStr (
            baseflex_ctrl_ptr, add_str_ptr, actual_str_ptr, delete_len_ptr);

    if (!is_got_string)
    {
        //get add actual string
        GetAddActualString (add_str_ptr, actual_str_ptr);
    }
}

/*****************************************************************************/
//  Description : get add actual string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetAddActualString (
    GUIEDIT_ADD_STR_T         *add_str_ptr,   //in:
    GUIEDIT_ADD_ACTUAL_STR_T  *actual_str_ptr //in/out:
)
{
    wchar       *add_start_ptr = PNULL;
    uint16      add_max_len = 0;
    uint16      left_str_len = 0;

    //set add string max len
    add_max_len = (uint16) (add_str_ptr->commit_len + add_str_ptr->underline_len + GUIEDIT_LIST_ADD_CHAR_MAX);

    //alloc memory for actual string
    actual_str_ptr->str_ptr = SCI_ALLOC_APP (add_max_len * sizeof (wchar));
    SCI_MEMSET (actual_str_ptr->str_ptr, 0, add_max_len*sizeof (wchar));

    //set add string start pointer and left string len
    add_start_ptr = actual_str_ptr->str_ptr;
    left_str_len  = add_max_len;

    //copy commit string
    if (0 < add_str_ptr->commit_len)
    {
        MMI_MEMCPY (add_start_ptr, left_str_len*sizeof (wchar),
                    add_str_ptr->commit_ptr, add_str_ptr->commit_len*sizeof (wchar),
                    add_str_ptr->commit_len*sizeof (wchar));
        actual_str_ptr->str_len = (uint16) (actual_str_ptr->str_len + add_str_ptr->commit_len);

        //set add string start pointer and left string len
        add_start_ptr = add_start_ptr + add_str_ptr->commit_len;
        left_str_len  = (uint16) (left_str_len - add_str_ptr->commit_len);
    }

    //copy underline string
    if (0 < add_str_ptr->underline_len)
    {
        MMI_MEMCPY (add_start_ptr, left_str_len*sizeof (wchar),
                    add_str_ptr->underline_ptr, add_str_ptr->underline_len*sizeof (wchar),
                    add_str_ptr->underline_len*sizeof (wchar));
        actual_str_ptr->str_len = (uint16) (actual_str_ptr->str_len + add_str_ptr->underline_len);
    }

    SCI_ASSERT (actual_str_ptr->str_len <= add_max_len); /*assert verified*/
}

/*****************************************************************************/
//  Description : move sting for add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MoveStringForAdd (//is add
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               *add_str_ptr,
    uint16              add_str_len,    //in:include LF and separator total len
    uint16              *move_len_ptr   //in/out:
)
{
    BOOLEAN     result = FALSE;
    uint16      move_str_len = 0;
    uint16      highlight_len = 0;
    uint16      highlight_min_pos = 0;
    uint16      highlight_max_pos = 0;
    int32       i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //get highlight len and max pos
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);
    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    if (highlight_len < add_str_len)
    {
        //move string backward behind the cursor
        move_str_len = (uint16) (add_str_len - highlight_len);

        if (0 < baseedit_ctrl_ptr->str_len)
        {
            for (i = (baseedit_ctrl_ptr->str_len - 1); i >= highlight_max_pos; i--)
            {
                baseedit_ctrl_ptr->str_ptr[i+move_str_len] = baseedit_ctrl_ptr->str_ptr[i];
            }
        }

        result = TRUE;
    }
    else
    {
        //move string forward behind the cursor
        move_str_len = (uint16) (highlight_len - add_str_len);

        if (0 < move_str_len)
        {
            if (baseedit_ctrl_ptr->str_len > highlight_max_pos)
            {
                for (i = highlight_max_pos; i < baseedit_ctrl_ptr->str_len; i++)
                {
                    baseedit_ctrl_ptr->str_ptr[i-move_str_len] = baseedit_ctrl_ptr->str_ptr[i];
                }
            }

            SCI_MEMSET ( (baseedit_ctrl_ptr->str_ptr + baseedit_ctrl_ptr->str_len - move_str_len), 0, (move_str_len*sizeof (wchar)));
        }
    }

    if (0 < add_str_len)
    {
        MMI_MEMCPY (
            baseedit_ctrl_ptr->str_ptr + highlight_min_pos,
            (baseedit_ctrl_ptr->str_max_len + highlight_len - baseedit_ctrl_ptr->str_len) *sizeof (wchar),
            add_str_ptr,
            add_str_len*sizeof (wchar),
            add_str_len*sizeof (wchar));
    }

    //set move string length
    if (PNULL != move_len_ptr)
    {
        *move_len_ptr = move_str_len;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get edit update info for add/delete string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetEditUpdateInfo (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    BOOLEAN             is_update_all,          //in:
    uint16              start_line_index,       //in:
    int32               pre_top_line_offset,    //in:
    uint32              pre_total_height,       //in:
    GUIEDIT_UPDATE_T    *update_ptr             //in/out:
)
{
    uint16      end_line_index = 0;
    uint16      top_line_index = 0;
    uint16      display_height = 0;
    uint32      total_height = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    SCI_ASSERT (PNULL != update_ptr); /*assert verified*/

    top_line_index = BASEEDIT_GetTopLineIndex(baseedit_ctrl_ptr);

    //is the same page
    if (baseedit_ctrl_ptr->top_line_offset == pre_top_line_offset)
    {
        //adjust update start line index
        start_line_index = (uint16) MAX (top_line_index, start_line_index);
    }
    else
    {
        start_line_index = top_line_index;
    }

    //get update end line index
    if (top_line_index < baseedit_ctrl_ptr->total_line_num)
    {
        display_height = BASEEDIT_GetDispHeight(baseedit_ctrl_ptr);
        total_height = BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr);

        //multi pages
        if ( total_height > (uint32)display_height)
        {
            end_line_index = BASEEDIT_GetBottomLineIndex(baseedit_ctrl_ptr);
        }
        else
        {
            //one page
            SCI_ASSERT (0 < baseedit_ctrl_ptr->total_line_num); /*assert verified*/

            if (pre_total_height > total_height)
            {
                is_update_all = TRUE;
            }
            else
            {
                end_line_index = BASEEDIT_GetBottomLineIndex(baseedit_ctrl_ptr);
            }
        }
    }
    else
    {
        SCI_PASSERT (FALSE, ("GetEditUpdateInfoForDelete: top index %d and total line %d is error!", baseedit_ctrl_ptr->top_line_offset, baseedit_ctrl_ptr->total_line_num)); /*assert verified*/
    }

    //set update info
    if (is_update_all)
    {
        update_ptr->update_mode = GUIEDIT_UPDATE_ALL;
    }
    else
    {
        update_ptr->update_mode = GUIEDIT_UPDATE_PART;
    }

    update_ptr->start_line_index = start_line_index;
    update_ptr->end_line_index   = end_line_index;
}

/*****************************************************************************/
//  Description : delete string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 DeleteString (//delete string actual len
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr
)
{
    uint16      delete_str_len = 0;
    uint16      highlight_max_pos = 0;
    int32       i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    VTLBASEFLEX_CheckDelStr (baseflex_ctrl_ptr);

    VTLBASEFLEX_BeforeDelString(baseflex_ctrl_ptr);

    //get highlight len
    delete_str_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    //get highlight min and max position
    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    //move string forward behind the cursor
    if (baseedit_ctrl_ptr->str_len > highlight_max_pos)
    {
        SCI_ASSERT (highlight_max_pos >= delete_str_len); /*assert verified*/

        for (i = highlight_max_pos; i < baseedit_ctrl_ptr->str_len; i++)
        {
            baseedit_ctrl_ptr->str_ptr[i-delete_str_len] = baseedit_ctrl_ptr->str_ptr[i];
        }
    }

    SCI_MEMSET ( (baseedit_ctrl_ptr->str_ptr + baseedit_ctrl_ptr->str_len - delete_str_len), 0, (delete_str_len*sizeof (wchar)));

    return (delete_str_len);
}

/*****************************************************************************/
//  Description : handle edit delete string after full
//  Global resource dependence :
//  Author: Jassmine
//  Note:
//    only effective in phonenum style
/*****************************************************************************/
LOCAL BOOLEAN DeleteStringAfterFull (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    uint16              str_delete_len
)
{
    BOOLEAN     result = FALSE;
    uint16      cursor_pos = 0;
    uint16      highlight_start = 0;
    uint16      highlight_end = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    //kevin.lou modified:delete assert
    if ( (0 < str_delete_len) && (PNULL != baseflex_ctrl_ptr) && (baseflex_ctrl_ptr->is_del_after_full))
    {
        //set cursor and highlight position
        cursor_pos      = baseedit_ctrl_ptr->cursor.cur_pos;
        highlight_start = baseedit_ctrl_ptr->highlight.start_pos;
        highlight_end   = baseedit_ctrl_ptr->highlight.end_pos;

        //delet string
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, str_delete_len);
        VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, str_delete_len, SETHIGHLIGHT_BOTH);

        //delet string
        DeleteString (baseflex_ctrl_ptr);

        //set cursor position and string length
        baseedit_ctrl_ptr->str_len = (uint16) (baseedit_ctrl_ptr->str_len - str_delete_len);

        //reset cursor position
        if (cursor_pos >= str_delete_len)
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (cursor_pos - str_delete_len));
        }
        else
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
        }

        //set highlight
        if (highlight_start == highlight_end)
        {
            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);
        }
        else
        {
            if (highlight_start >= str_delete_len)
            {
                VTLBASEEDIT_SetHighlight (
                    baseedit_ctrl_ptr,
                    (uint16) (highlight_start - str_delete_len),
                    0, SETHIGHLIGHT_START);
            }
            else
            {
                VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_START);
            }

            if (highlight_end >= str_delete_len)
            {
                VTLBASEEDIT_SetHighlight (
                    baseedit_ctrl_ptr, 0,
                    (uint16) (highlight_end - str_delete_len), SETHIGHLIGHT_END);
            }
            else
            {
                VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_END);
            }

            if (baseedit_ctrl_ptr->highlight.start_pos == baseedit_ctrl_ptr->highlight.end_pos)
            {
                baseedit_ctrl_ptr->highlight.is_underline = FALSE;
            }
        }

        result = TRUE;
    }

    return (result);
}

