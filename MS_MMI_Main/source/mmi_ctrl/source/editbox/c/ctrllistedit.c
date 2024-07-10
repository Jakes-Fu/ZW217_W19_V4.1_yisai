/*****************************************************************************
** File Name:      ctrllistedit.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"

#include "ctrllistedit.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
// item state
typedef enum
{
    EDIT_ITEM_STATE_START = 0,      // 开始位置
    EDIT_ITEM_STATE_CHAR,           // 普通字符
    EDIT_ITEM_STATE_OBJECT,         // 文本对象
    EDIT_ITEM_STATE_SEPARATOR,      // 分隔符
    EDIT_ITEM_STATE_LF,             // 回车符
    EDIT_ITEM_STATE_END,            // 结束位置
    EDIT_ITEM_STATE_INVALID         // 无效字符
} EDIT_ITEM_STATE_E;

// commit function pointer
typedef BOOLEAN (*COMMIT_STR_FUNC) (
    CTRLLISTEDIT_OBJ_T       *listedit_ctrl_ptr,     // in:
    GUIEDIT_ADD_STR_T        *add_str_ptr,       // in
    GUIEDIT_ADD_ACTUAL_STR_T *actual_str_ptr,    // out
    uint16                   *commit_count_ptr,  // out
    uint16                   *underline_count_ptr,   // out
    uint16                   *item_num_ptr       // out
);

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init listedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlInitVtbl (
    CTRLLISTEDIT_VTBL_T        *listedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : listedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLLISTEDIT_INIT_PARAM_T   *listedit_param_ptr
);

/*****************************************************************************/
//  Description : listedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *listedit_init_ptr,   //in
    CTRLLISTEDIT_INIT_PARAM_T   *listedit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle listedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get listedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLLISTEDIT_OBJ_T* GetListEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ListEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E ListEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlGetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in/out:
    GUIIM_TYPE_T           *init_im_ptr,       //in/out:
    GUIIM_TYPE_T           *allow_im_ptr,      //in/out:
    GUIIM_INPUT_TYPE_T     *init_input_ptr,    //in/out:
    GUIIM_INPUT_TYPE_T     *allow_input_ptr,   //in/out:
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in/out:
    GUIIM_TAG_T            *tag_ptr            //in/out:
);

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in:may PNULL
    GUIIM_TYPE_T           *init_im_ptr,       //in:may PNULL
    GUIIM_TYPE_T           *allow_im_ptr,      //in:may PNULL
    GUIIM_INPUT_TYPE_T     *init_type_ptr,     //in:may PNULL
    GUIIM_INPUT_TYPE_T     *allow_type_ptr,    //in:may PNULL
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in:may PNULL
    GUIIM_TAG_T            *tag_ptr
);

/*****************************************************************************/
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
);

/*****************************************************************************/
//  Description : set cursor position
//  Global resource dependence :
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetCursorPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,	//in:
    uint16			    cur_pos			//in:from 0
);

/*****************************************************************************/
//  Description : set highlight position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetHighlight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //[in]
    uint16              start_pos,          //[in]
    uint16              end_pos,            //[in]
    SETHIGHLIGHT_E      highlight_type      //[in]
);

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
);

/*****************************************************************************/
//  Description : display list index info
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlDisplayLeftNum (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 在添加字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlBeforeAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
);

/*****************************************************************************/
//  Description : 在添加字符后做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlAfterAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
);

/*****************************************************************************/
//  Description : 在删除字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlBeforeDelString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr      //in:
);

/*****************************************************************************/
//  Description : 检查被删字符
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 此方法在删除时用到，因为在删除动作时，在确保当前有字符被选中，像list风格的话，
// 需要保证item object对象被整体选中
/*****************************************************************************/
LOCAL void ListEditCtrlCheckDelStr (
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 检查添加的字符
//  Global resource dependence :
//  Author: hua.fang
// TRUE: commit的字符已经拷贝到actual_str_ptr;
// FALSE: 不拷贝字符，需要后续处理
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlCheckCommitStr (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
    uint16                      *delete_len_ptr         //out:
);

/*****************************************************************************/
//  Description : 当删除文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlOnTextDeleted(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 当添加文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlOnTextAdded(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_ADD_STR_T       *add_str_ptr
);

/*****************************************************************************/
//  Description : get list item string length and start position,except underline
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemLen (//光标前面是;增加代表新增一个item
    CTRLLISTEDIT_OBJ_T*listedit_ctrl_ptr,
    uint16            *item_len_ptr,  //in/out:
    uint16            *start_pos_ptr  //in/out:may PNULL item start position
);

/*****************************************************************************/
//  Description : current cursor position is the first position in one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemFirstPos (
    CTRLLISTEDIT_OBJ_T* listedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : destroy list item info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DestroyListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : add a list item object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr, // [in]
    uint16              start,          // [in]
    uint16              len,            // [in]
    MMI_STRING_T        *user_data_ptr  // [in]
);

/*****************************************************************************/
//  Description : remove list item object between two pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,  // [in]
    uint16              start_pos,       // [in]
    uint16              end_pos          // [in]
);

/*****************************************************************************/
//  Description : adjust list item object position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr, // [in]
    uint16              adjust_start,   // [in]
    int16               adjust_len      // [in]
);

/*****************************************************************************/
//  Description : find list item object info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_OBJ_INFO_T* FindListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,    // [in]
    uint16              pos                // [in]
);

/*****************************************************************************/
//  Description : get start and end pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemPos (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,  // [in]
    uint16              pos,             // [in]
    uint16              *start_pos_ptr,  // [out]
    uint16              *end_pos_ptr     // [out]
);

/*****************************************************************************/
//  Description : check list item object state
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckListItemState (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : select current item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// this method is for GUIEDIT_ReplaceCurListItem to highlight all current item
// text and replace it.
/*****************************************************************************/
LOCAL BOOLEAN SelectCurItem (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle edit list direction msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDirectionKey (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,
    GUIEDIT_CURSOR_MOVE_E   move_direction
);

/*****************************************************************************/
//  Description : check list highlist string while adding
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckListHighlightPos (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,         // [in]
    uint16              *highlight_min_pos_ptr, // [out]
    uint16              *highlight_max_pos_ptr  // [out]
);

/*****************************************************************************/
//  Description : check list highlist string while deleting, assure list highlight string
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckListHighlightStr (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : after delete string,modify current item number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyItemNumForDel (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get state before min_highlight_pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    此方法不计算前面是否是OBJECT状态
/*****************************************************************************/
LOCAL void GetPrevCharState (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,         // in
    wchar                   separator_char,         // in
    EDIT_ITEM_STATE_E       *pre_item_state_ptr,    // out
    uint16                  *prev_char_count_ptr    // out
);

/*****************************************************************************/
//  Description : get state after max_highlight_pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetNextCharState (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,         // in
    wchar                   separator_char,         // in
    EDIT_ITEM_STATE_E       *next_item_state_ptr,   // out
    uint16                  *next_char_count_ptr    // out
);

/*****************************************************************************/
//  Description : get state of current pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL EDIT_ITEM_STATE_E GetCurrCharState (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,     // in
    wchar                   separator_char,         // in
    GUIEDIT_ADD_STR_T       *add_str_ptr,           // in
    int32                   index,                  // in
    EDIT_ITEM_STATE_E       pre_item_state,         // in
    wchar                   *cur_char_ptr           // out
);

/*****************************************************************************/
//  Description : add commit char in actual_str_ptr
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    it will check item count and actual count
/*****************************************************************************/
LOCAL void AddCommitStr (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,     // in
    GUIEDIT_ADD_ACTUAL_STR_T *actual_str_ptr,       // in/out
    uint16                   char_pos,              // in
    wchar                    cur_char,              // in
    uint16                   *cur_item_num_ptr,     // in/out
    uint16                   *actual_count_ptr      // in/out
);

/*****************************************************************************/
//  Description : get commit string when adding normal string
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    一个原则:尽量将有效的数据保存下来，如果非有效数据，那就退出
/*****************************************************************************/
LOCAL BOOLEAN GetCommitStr (
    CTRLLISTEDIT_OBJ_T          *listedit_ctrl_ptr,     // in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,       // in
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,    // out
    uint16                      *commit_count_ptr,  // out
    uint16                      *underline_count_ptr,   // out
    uint16                      *item_num_ptr       // out
);

/*****************************************************************************/
//  Description : get commit string when adding item object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCommitObject (
    CTRLLISTEDIT_OBJ_T       *listedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T        *add_str_ptr,       // in
    GUIEDIT_ADD_ACTUAL_STR_T *actual_str_ptr,    // out
    uint16                   *commit_count_ptr,  // out
    uint16                   *underline_count_ptr,   // out
    uint16                   *item_num_ptr       // out
);

/*****************************************************************************/
//  Description : check item info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    check the correction of the item object info
/*****************************************************************************/
LOCAL BOOLEAN CheckItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : remove item info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DelItemObject (
    GUIEDIT_OBJ_INFO_T *obj_info_ptr    // [in]
);

/*****************************************************************************/
//  Description : get edit list item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetListItem (
    MMI_CTRL_ID_T			ctrl_id,        //in:
    uint16					array_len,      //in
    GUIEDIT_LIST_ITEM_T		*item_array_ptr,//out
    GUIEDIT_LIST_ITEM_STR_T	*item_str_array_ptr //out
);

/*****************************************************************************/
//  Description : list add separator
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddSeparator (//is need add
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : is list char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsListChar (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,
    wchar               char_code       //in:
);

/*****************************************************************************/
//  Description : get edit list item is valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidItem (
    wchar                 *str_ptr,
    uint16                str_len,
    GUIEDIT_LIST_STYLE_E  item_style
);

/*****************************************************************************/
//  Description : 给输入法发同步消息
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 只有t9时有效
/*****************************************************************************/
LOCAL void SendImSyncMsg(
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit list item number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLLISTEDIT_GetListItemNum (
    MMI_CTRL_ID_T  ctrl_id //in:
)
{
    uint16              item_num = 0;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    listedit_ctrl_ptr = GetListEditPtr (ctrl_id);

    if (PNULL != listedit_ctrl_ptr)
    {
        item_num = listedit_ctrl_ptr->item_num;
    }

    return (item_num);
}

/*****************************************************************************/
//  Description : get edit list item string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
//    when item object, return the user data.
/*****************************************************************************/
PUBLIC void CTRLLISTEDIT_GetListItemString (
    MMI_CTRL_ID_T             ctrl_id,        //in:
    uint16                    array_len,      //in
    GUIEDIT_LIST_ITEM_STR_T   *item_array_ptr //in/out:item string array
)
{
    GetListItem (ctrl_id, array_len, PNULL, item_array_ptr);
}

/*****************************************************************************/
//  Description : add edit list item string
//  Global resource dependence :
//  Author: Jassmine
//  Note:超出最大范围则进行截取,List只能一个Item一个Item增加
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_AddListItem (
    MMI_CTRL_ID_T    ctrl_id,    //in:
    wchar            *str_ptr,   //in:
    uint16           str_len     //in:
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_UPDATE_T    update_info = {0};
    GUIEDIT_ADD_STR_T   add_str = {0};
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    if ((PNULL != str_ptr) && (0 < str_len))
    {
        //get edit pointer by control id
        listedit_ctrl_ptr = GetListEditPtr (ctrl_id);
        baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)listedit_ctrl_ptr;
        baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

        //check edit string
        if ((PNULL != listedit_ctrl_ptr) &&
            (BASEFLEX_CheckEditString (baseflex_ctrl_ptr, str_ptr, &str_len)))
        {
            //set add string info
            add_str.commit_ptr = str_ptr;
            add_str.commit_len = str_len;

            add_str.is_list_item = TRUE;

            //set cursor pos
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, baseedit_ctrl_ptr->str_len);

            //set no highlight
            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

            //add string
            result = BASEFLEX_AddString (baseflex_ctrl_ptr, &update_info, &add_str, PNULL, 0);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : 添加文本集合
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_AddListItemArray (
    MMI_CTRL_ID_T       ctrl_id,
    GUIEDIT_LIST_ITEM_T *list_item_ptr,
    uint16              list_item_count
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    if ((PNULL != list_item_ptr) && (0 < list_item_count))
    {
        //get edit pointer by control id
        listedit_ctrl_ptr = GetListEditPtr (ctrl_id);
        baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)listedit_ctrl_ptr;
        baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

        //check edit string
        if (PNULL != listedit_ctrl_ptr)
        {
            //set cursor pos
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, baseedit_ctrl_ptr->str_len);

            //set no highlight
            BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

            //add string
            result = BASEFLEX_AddString (baseflex_ctrl_ptr, &update_info, PNULL, list_item_ptr, list_item_count);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get edit list item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLISTEDIT_GetListItemArray (
    MMI_CTRL_ID_T             ctrl_id,        //in:
    uint16                    array_len,      //in
    GUIEDIT_LIST_ITEM_T       *item_array_ptr //out
)
{
    GetListItem (ctrl_id, array_len, item_array_ptr, PNULL);
}

/*****************************************************************************/
//  Description : replace current item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// this method is just for list style
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_ReplaceCurListItem (
    MMI_CTRL_ID_T         ctrl_id,        //in:
    GUIEDIT_LIST_ITEM_T   *list_item_ptr, //in:
    uint16                list_item_count
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    if (PNULL != list_item_ptr && 0 < list_item_count)
    {
        //get edit pointer by control id
        listedit_ctrl_ptr = GetListEditPtr (ctrl_id);
        baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)listedit_ctrl_ptr;
        baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

        //check edit string
        if (PNULL != listedit_ctrl_ptr)
        {
            if (baseedit_ctrl_ptr->highlight.is_underline)
            {
                GUIIM_CommitLast (baseedit_ctrl_ptr->im_ctrl_handle);
            }

            // 先将当前项选中
            if (SelectCurItem (listedit_ctrl_ptr))
            {
                // 再去替换文本
                result = BASEFLEX_AddString (baseflex_ctrl_ptr, &update_info, PNULL, list_item_ptr, list_item_count);
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : set edit max length for numlist style
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_SetMaxListItemNum (
    MMI_CTRL_ID_T      ctrl_id,
    uint16             max_num,
    uint16             item_max_len
)
{
    BOOLEAN             result = FALSE;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    listedit_ctrl_ptr = GetListEditPtr (ctrl_id);
    baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)listedit_ctrl_ptr;
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    //check edit string
    if (PNULL != listedit_ctrl_ptr
        && listedit_ctrl_ptr->max_num != max_num
        && listedit_ctrl_ptr->item_max_len != item_max_len)
    {
        //set item number and item string max len
        listedit_ctrl_ptr->max_num = max_num;
        listedit_ctrl_ptr->item_max_len = item_max_len;

        //set string max len,need separator and LF
        baseedit_ctrl_ptr->str_max_len  = (uint16) (max_num * (item_max_len + 2));
        SCI_ASSERT (0 < baseedit_ctrl_ptr->str_max_len); /*assert verified*/

        DestroyListItemObject (listedit_ctrl_ptr);

        //free sting memory and alloc
        if (PNULL != baseedit_ctrl_ptr->str_ptr)
        {
            SCI_FREE (baseedit_ctrl_ptr->str_ptr);
            baseedit_ctrl_ptr->str_len = 0;
        }

        baseedit_ctrl_ptr->str_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);

        //free line info memory and alloc
        if (PNULL != baseedit_ctrl_ptr->line_info_ptr)
        {
            SCI_FREE (baseedit_ctrl_ptr->line_info_ptr);
            baseedit_ctrl_ptr->total_line_num = 1;
            BASEEDIT_GetTopLineOffsetRange(baseedit_ctrl_ptr, &baseedit_ctrl_ptr->top_line_offset, PNULL);
        }

        baseedit_ctrl_ptr->line_info_ptr = BASEEDIT_AllocLineMemory (baseedit_ctrl_ptr->str_max_len);

        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
        VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_BOTH);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set list style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLISTEDIT_SetListStyle (
    MMI_CTRL_ID_T          ctrl_id,    //in
    GUIEDIT_LIST_STYLE_E   list_style  //in
)
{
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    listedit_ctrl_ptr = GetListEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    if (PNULL != listedit_ctrl_ptr)
    {
        //set list style
        listedit_ctrl_ptr->theme.list_style = list_style;

        BASEEDIT_ResetIm (baseedit_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : get listedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LISTEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFLEX_TYPE,                                 // parent class type
                       "listedit",                                         // control name
                       sizeof (CTRLLISTEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) ListEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) ListEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLLISTEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) ListEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLLISTEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) ListEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init listedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlInitVtbl (
    CTRLLISTEDIT_VTBL_T        *listedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) listedit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) listedit_vtbl_ptr;
    CTRLBASEFLEX_VTBL_T *baseflex_ctrl_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*) listedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = ListEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = ListEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = ListEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = ListEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = ListEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->ConfigImKey = ListEditCtrlConfigImKey;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = ListEditCtrlSetLandscapeEdit;
    baseedit_ctrl_vtbl_ptr->SetCursorPos = ListEditCtrlSetCursorPos;
    baseedit_ctrl_vtbl_ptr->SetHighlight = ListEditCtrlSetHighlight;

    baseflex_ctrl_vtbl_ptr->IsStringValid = ListEditCtrlIsStringValid;
    baseflex_ctrl_vtbl_ptr->DisplayLeftNum = ListEditCtrlDisplayLeftNum;
    baseflex_ctrl_vtbl_ptr->CheckDelStr = ListEditCtrlCheckDelStr;
    baseflex_ctrl_vtbl_ptr->CheckCommitStr = ListEditCtrlCheckCommitStr;
    baseflex_ctrl_vtbl_ptr->BeforeAddString = ListEditCtrlBeforeAddString;
    baseflex_ctrl_vtbl_ptr->AfterAddString = ListEditCtrlAfterAddString;
    baseflex_ctrl_vtbl_ptr->BeforeDelString = ListEditCtrlBeforeDelString;
    baseflex_ctrl_vtbl_ptr->OnTextDeleted = ListEditCtrlOnTextDeleted;
    baseflex_ctrl_vtbl_ptr->OnTextAdded = ListEditCtrlOnTextAdded;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void ListEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *listedit_init_ptr,   //in
    CTRLLISTEDIT_INIT_PARAM_T   *listedit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T       *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) listedit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) listedit_param_ptr;

    if (PNULL == listedit_init_ptr || PNULL == listedit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = listedit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = listedit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = listedit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = listedit_init_ptr->type;

    listedit_param_ptr->item_max_len = listedit_init_ptr->add_data.list.item_max_len;
    listedit_param_ptr->item_num = listedit_init_ptr->add_data.list.item_num;
}

/*****************************************************************************/
//  Description : listedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLLISTEDIT_INIT_PARAM_T   *listedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == listedit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetListEditTheme (&listedit_ctrl_ptr->theme);

    //set item number and item string max len
    listedit_ctrl_ptr->max_num = listedit_param_ptr->item_num;
    listedit_ctrl_ptr->item_max_len = listedit_param_ptr->item_max_len;

    //set string max len,need separator and LF
    baseedit_ctrl_ptr->str_max_len  = (uint16) (listedit_ctrl_ptr->max_num * (listedit_ctrl_ptr->item_max_len + 2));

    //set display im icon and number info
    baseedit_ctrl_ptr->is_disp_im_icon  = TRUE;
    baseflex_ctrl_ptr->is_disp_num_info = TRUE;

    BASEEDIT_Init(baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : listedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == listedit_ctrl_ptr)
    {
        return FALSE;
    }

    // destroy list item info
    DestroyListItemObject (listedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : handle listedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == listedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        result = HandleDirectionKey (listedit_ctrl_ptr, GUIEDIT_CURSOR_MOVE_UP);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        result = HandleDirectionKey (listedit_ctrl_ptr, GUIEDIT_CURSOR_MOVE_DOWN);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        result = HandleDirectionKey (listedit_ctrl_ptr, GUIEDIT_CURSOR_MOVE_LEFT);
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleDirectionKey (listedit_ctrl_ptr, GUIEDIT_CURSOR_MOVE_RIGHT);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        //judge is need add separator,need than add
        AddSeparator (listedit_ctrl_ptr);

        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_LISTEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
#endif

    // 只有t9会发这个消息
    case MSG_NOTIFY_IM_SETCURSOR:
        AddSeparator(listedit_ctrl_ptr);

        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_LISTEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_LISTEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get listedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLLISTEDIT_OBJ_T* GetListEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!ListEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== GetListEditPtr error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLLISTEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ListEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_LISTEDIT_TYPE);
}

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E ListEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLLISTEDIT_OBJ_T* listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    return listedit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLLISTEDIT_OBJ_T* listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    listedit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlGetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in/out:
    GUIIM_TYPE_T           *init_im_ptr,       //in/out:
    GUIIM_TYPE_T           *allow_im_ptr,      //in/out:
    GUIIM_INPUT_TYPE_T     *init_input_ptr,    //in/out:
    GUIIM_INPUT_TYPE_T     *allow_input_ptr,   //in/out:
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in/out:
    GUIIM_TAG_T            *tag_ptr            //in/out:
)
{
    CTRLLISTEDIT_OBJ_T* listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    switch (listedit_ctrl_ptr->theme.list_style)
    {
    case GUIEDIT_LIST_STYLE_PHONENUM:
        *is_forbid_ptr    = listedit_ctrl_ptr->theme.phonenum_im.is_forbid_symbol;
        *init_input_ptr   = listedit_ctrl_ptr->theme.phonenum_im.init_input_type;
        *allow_input_ptr  = listedit_ctrl_ptr->theme.phonenum_im.allow_input_type;
        *init_im_ptr      = listedit_ctrl_ptr->theme.phonenum_im.init_im;
        *allow_im_ptr     = listedit_ctrl_ptr->theme.phonenum_im.allow_im;
        *capital_mode_ptr = listedit_ctrl_ptr->theme.phonenum_im.capital_mode;
        *tag_ptr          = listedit_ctrl_ptr->theme.phonenum_im.tag;
        break;

    case GUIEDIT_LIST_STYLE_EMAIL:
        *is_forbid_ptr    = listedit_ctrl_ptr->theme.email_im.is_forbid_symbol;
        *init_input_ptr   = listedit_ctrl_ptr->theme.email_im.init_input_type;
        *allow_input_ptr  = listedit_ctrl_ptr->theme.email_im.allow_input_type;
        *init_im_ptr      = listedit_ctrl_ptr->theme.email_im.init_im;
        *allow_im_ptr     = listedit_ctrl_ptr->theme.email_im.allow_im;
        *capital_mode_ptr = listedit_ctrl_ptr->theme.email_im.capital_mode;
        *tag_ptr          = listedit_ctrl_ptr->theme.email_im.tag;
        break;

    case GUIEDIT_LIST_STYLE_ALL:
        *is_forbid_ptr    = listedit_ctrl_ptr->theme.all_im.is_forbid_symbol;
        *init_input_ptr   = listedit_ctrl_ptr->theme.all_im.init_input_type;
        *allow_input_ptr  = listedit_ctrl_ptr->theme.all_im.allow_input_type;
        *init_im_ptr      = listedit_ctrl_ptr->theme.all_im.init_im;
        *allow_im_ptr     = listedit_ctrl_ptr->theme.all_im.allow_im;
        *capital_mode_ptr = listedit_ctrl_ptr->theme.all_im.capital_mode;
        *tag_ptr          = listedit_ctrl_ptr->theme.all_im.tag;
        break;

    default:
        //SCI_TRACE_LOW:"GetEditImInfo:edit list style %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_1066_112_2_18_3_15_36_115, (uint8*) "d", listedit_ctrl_ptr->theme.list_style);
        break;
    }
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in:may PNULL
    GUIIM_TYPE_T           *init_im_ptr,       //in:may PNULL
    GUIIM_TYPE_T           *allow_im_ptr,      //in:may PNULL
    GUIIM_INPUT_TYPE_T     *init_type_ptr,     //in:may PNULL
    GUIIM_INPUT_TYPE_T     *allow_type_ptr,    //in:may PNULL
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in:may PNULL
    GUIIM_TAG_T            *tag_ptr
)
{
    CTRLLISTEDIT_OBJ_T* listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    switch (listedit_ctrl_ptr->theme.list_style)
    {
    case GUIEDIT_LIST_STYLE_PHONENUM:
        if (PNULL != is_forbid_ptr)
        {
            listedit_ctrl_ptr->theme.phonenum_im.is_forbid_symbol = *is_forbid_ptr;
        }

        if (PNULL != init_im_ptr)
        {
            listedit_ctrl_ptr->theme.phonenum_im.init_im = *init_im_ptr;
        }

        if (PNULL != allow_im_ptr)
        {
            listedit_ctrl_ptr->theme.phonenum_im.allow_im = *allow_im_ptr;
        }

        if (PNULL != init_type_ptr)
        {
            listedit_ctrl_ptr->theme.phonenum_im.init_input_type = *init_type_ptr;
        }

        if (PNULL != allow_type_ptr)
        {
            listedit_ctrl_ptr->theme.phonenum_im.allow_input_type = *allow_type_ptr;
        }

        if (PNULL != capital_mode_ptr)
        {
            listedit_ctrl_ptr->theme.phonenum_im.capital_mode = *capital_mode_ptr;
        }

        if (PNULL != tag_ptr)
        {
            listedit_ctrl_ptr->theme.phonenum_im.tag = *tag_ptr;
        }

        break;

    case GUIEDIT_LIST_STYLE_EMAIL:
        if (PNULL != is_forbid_ptr)
        {
            listedit_ctrl_ptr->theme.email_im.is_forbid_symbol = *is_forbid_ptr;
        }

        if (PNULL != init_im_ptr)
        {
            listedit_ctrl_ptr->theme.email_im.init_im = *init_im_ptr;
        }

        if (PNULL != allow_im_ptr)
        {
            listedit_ctrl_ptr->theme.email_im.allow_im = *allow_im_ptr;
        }

        if (PNULL != init_type_ptr)
        {
            listedit_ctrl_ptr->theme.email_im.init_input_type = *init_type_ptr;
        }

        if (PNULL != allow_type_ptr)
        {
            listedit_ctrl_ptr->theme.email_im.allow_input_type = *allow_type_ptr;
        }

        if (PNULL != capital_mode_ptr)
        {
            listedit_ctrl_ptr->theme.email_im.capital_mode = *capital_mode_ptr;
        }

        if (PNULL != tag_ptr)
        {
            listedit_ctrl_ptr->theme.email_im.tag = *tag_ptr;
        }

        break;

    case GUIEDIT_LIST_STYLE_ALL:
        if (PNULL != is_forbid_ptr)
        {
            listedit_ctrl_ptr->theme.all_im.is_forbid_symbol = *is_forbid_ptr;
        }

        if (PNULL != init_im_ptr)
        {
            listedit_ctrl_ptr->theme.all_im.init_im = *init_im_ptr;
        }

        if (PNULL != allow_im_ptr)
        {
            listedit_ctrl_ptr->theme.all_im.allow_im = *allow_im_ptr;
        }

        if (PNULL != init_type_ptr)
        {
            listedit_ctrl_ptr->theme.all_im.init_input_type = *init_type_ptr;
        }

        if (PNULL != allow_type_ptr)
        {
            listedit_ctrl_ptr->theme.all_im.allow_input_type = *allow_type_ptr;
        }

        if (PNULL != capital_mode_ptr)
        {
            listedit_ctrl_ptr->theme.all_im.capital_mode = *capital_mode_ptr;
        }

        if (PNULL != tag_ptr)
        {
            listedit_ctrl_ptr->theme.all_im.tag = *tag_ptr;
        }

        break;

    default:
        //SCI_TRACE_LOW:"SetEditImInfo:edit list style %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_1346_112_2_18_3_15_36_117, (uint8*) "d", listedit_ctrl_ptr->theme.list_style);
        break;
    }
}

/*****************************************************************************/
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void ListEditCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
)
{
    //BOOLEAN     is_config_star = FALSE;
    BOOLEAN     is_config_num1 = FALSE;
    uint16      highlight_min_pos = 0;
    uint16      highlight_max_pos = 0;
    uint16      current_item_len = 0;
    wchar       separator_char = 0;
    wchar       previous_char = 0;
    wchar       next_char = 0;
    wchar       *star_value_ptr = PNULL;
    wchar       *num1_value_ptr = PNULL;
    wchar       list_star_array1[3] = {'*', '+', '#'};
    wchar       list_star_array2[4] = {';', '*', '+', '#'};
    uint32      star_value_count = 0;
    wchar       list_num1_array1[5] = {'@', '.', '_', '-', '1'};
    wchar       list_num1_array2[6] = {';', '@', '.', '_', '-', '1'};
    uint32      num1_value_count = 0;
    CTRLLISTEDIT_OBJ_T* listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
//NEWMS00190493 [6530][64X64_320X240BAR_QW]新建信息中的联系人不能调用输入法。
//当前list能够输入任何字符
#if 0
        is_config_star = TRUE;
#endif
        is_config_num1 = TRUE;

        //key qwery not config number 1
        if (GUIIM_INPUT_TYPE_QWERT == GUIIM_GetInput (baseedit_ctrl_ptr->im_ctrl_handle))
        {
            is_config_num1 = FALSE;
        }

        GetListItemLen (listedit_ctrl_ptr, &current_item_len, PNULL);

        //set start value and count
        if (IsItemFirstPos (listedit_ctrl_ptr))
        {
            star_value_ptr   = list_star_array1;
            star_value_count = 3;

            num1_value_ptr   = list_num1_array1;
            num1_value_count = 5;
        }
        else
        {
            if (current_item_len == listedit_ctrl_ptr->item_max_len)
            {
                //only add separator
                star_value_ptr   = list_star_array2;
                star_value_count = 1;

                num1_value_ptr   = list_num1_array2;
                num1_value_count = 1;
            }
            else
            {
                if (listedit_ctrl_ptr->item_num == listedit_ctrl_ptr->max_num) //list item is full
                {
                    star_value_ptr   = list_star_array1;
                    star_value_count = 3;

                    num1_value_ptr   = list_num1_array1;
                    num1_value_count = 5;
                }
                else
                {
                    //get list separator
                    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

                    //get highlight min and max position
                    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
                    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

                    //get previous char
                    if (0 < highlight_min_pos)
                    {
                        previous_char = baseedit_ctrl_ptr->str_ptr[highlight_min_pos - 1];
                    }
                    else
                    {
                        previous_char = separator_char;
                    }

                    //get next char
                    if (highlight_max_pos < baseedit_ctrl_ptr->str_len)
                    {
                        next_char = baseedit_ctrl_ptr->str_ptr[highlight_max_pos];
                    }
                    else
                    {
                        next_char = previous_char;
                    }

                    //previous and next char is separator,not add
                    if ( (separator_char == previous_char) ||
                            (separator_char == next_char))
                    {
                        star_value_ptr   = list_star_array1;
                        star_value_count = 3;

                        num1_value_ptr   = list_num1_array1;
                        num1_value_count = 5;
                    }
                    else
                    {
                        star_value_ptr   = list_star_array2;
                        star_value_count = 4;

                        num1_value_ptr   = list_num1_array2;
                        num1_value_count = 6;
                    }
                }
            }
        }

        //if (is_config_star)
        //{
            ////config im *
            //GUIIM_SetCustomKey (
            //    baseedit_ctrl_ptr->im_ctrl_handle,
            //    GUIIM_TYPE_DIGITAL,
            //    GUIIM_CUSTOM_KEY_STAR,
            //    star_value_ptr,
            //    star_value_count);
        //}

        if (is_config_num1)
        {
            //config im number 1
            GUIIM_SetCustomKey (
                baseedit_ctrl_ptr->im_ctrl_handle,
                GUIIM_TYPE_ABC,
                GUIIM_CUSTOM_KEY_1,
                num1_value_ptr,
                num1_value_count);
        }
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    uint16                  item_num = 0;
    GUIEDIT_LIST_ITEM_T     *item_array_ptr = PNULL;
    CTRLLISTEDIT_OBJ_T      *new_edit_ctrl_ptr = GetListEditPtr (landscape_edit);
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set item number and item string max len
        new_edit_ctrl_ptr->max_num      = listedit_ctrl_ptr->max_num;
        new_edit_ctrl_ptr->item_max_len = listedit_ctrl_ptr->item_max_len;

        //set string max len
        CTRLBASEFLEX_SetMaxLen (
                landscape_edit,
                (uint16) (new_edit_ctrl_ptr->max_num* (new_edit_ctrl_ptr->item_max_len + 2)));

        //set list style
        CTRLLISTEDIT_SetListStyle (landscape_edit, listedit_ctrl_ptr->theme.list_style);

        //get list string
        item_num = CTRLLISTEDIT_GetListItemNum (base_ctrl_ptr->handle);

        if (0 < item_num)
        {
            //alloc
            item_array_ptr = SCI_ALLOC_APP (item_num * sizeof (GUIEDIT_LIST_ITEM_T));

            CTRLLISTEDIT_GetListItemArray (base_ctrl_ptr->handle,
                                      item_num,
                                      item_array_ptr);

            //set list string
            CTRLLISTEDIT_AddListItemArray (landscape_edit, item_array_ptr, item_num);

            //free
            SCI_FREE (item_array_ptr);
        }

        //set im
        new_edit_ctrl_ptr->theme.phonenum_im = listedit_ctrl_ptr->theme.phonenum_im;
        new_edit_ctrl_ptr->theme.email_im    = listedit_ctrl_ptr->theme.email_im;
        new_edit_ctrl_ptr->theme.all_im      = listedit_ctrl_ptr->theme.all_im;

        //set cursor pos
        CTRLBASEFLEX_SetCursorCurPos (landscape_edit, baseedit_ctrl_ptr->cursor.cur_pos);

        CTRLBASEFLEX_ConfigDisableChar (
            landscape_edit,
            baseflex_ctrl_ptr->disable_char_ptr,
            baseflex_ctrl_ptr->disable_char_len);

        CTRLBASEFLEX_ConfigAddEnableChar (
            landscape_edit,
            baseflex_ctrl_ptr->add_enable_char_ptr,
            baseflex_ctrl_ptr->add_enable_char_len);

        //set cursor hide
        CTRLBASEFLEX_SetCursorHideState (landscape_edit, baseedit_ctrl_ptr->cursor.is_hide);
    }
}

/*****************************************************************************/
//  Description : set cursor position
//  Global resource dependence :
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetCursorPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,	//in:
    uint16			    cur_pos			//in:from 0
)
{
    uint16  start_pos = 0;
    uint16  end_pos = 0;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (cur_pos > baseedit_ctrl_ptr->str_len)
    {
        cur_pos = baseedit_ctrl_ptr->str_len;
    }

    GetListItemPos (listedit_ctrl_ptr, cur_pos, &start_pos, &end_pos);

    // 在拖动状态下，在起始位置之前拖动下，如果落在文本对象内，需要整个选中。
    if (baseflex_ctrl_ptr->is_tp_moved && cur_pos < baseedit_ctrl_ptr->highlight.start_pos)
    {
        baseedit_ctrl_ptr->cursor.cur_pos = start_pos;
    }
    else
    {
        baseedit_ctrl_ptr->cursor.cur_pos = end_pos;
    }
}

/*****************************************************************************/
//  Description : set highlight position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlSetHighlight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //[in]
    uint16              start_pos,          //[in]
    uint16              end_pos,            //[in]
    SETHIGHLIGHT_E      highlight_type      //[in]
)
{
    uint16  item_start_pos = 0;
    uint16  item_end_pos = 0;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL == baseedit_ctrl_ptr)
    {
        return;
    }

    // 做点保护，防止设置的值超过总数
    if (start_pos > baseedit_ctrl_ptr->str_len)
    {
        start_pos = baseedit_ctrl_ptr->str_len;
    }

    if (end_pos > baseedit_ctrl_ptr->str_len)
    {
        end_pos = baseedit_ctrl_ptr->str_len;
    }

    if (SETHIGHLIGHT_START & highlight_type)    /*lint !e655*/
    {
        GetListItemPos (listedit_ctrl_ptr, start_pos, &item_start_pos, &item_end_pos);
        baseedit_ctrl_ptr->highlight.start_pos = item_start_pos;
    }

    if (SETHIGHLIGHT_END & highlight_type)      /*lint !e655*/
    {
        GetListItemPos (listedit_ctrl_ptr, end_pos, &item_start_pos, &item_end_pos);

        // 条件(edit_ctrl_ptr->highlight.start_pos > item_end_pos)
        // when select object, and start position is larger than end, set
        // end position with item_start_pos. it will work while moving stylus.
        // 条件(edit_ctrl_ptr->is_tp_moved && end_pos < edit_ctrl_ptr->highlight.start_pos)
        // 拖选状态下，往起始位置之前拖选，选中整个文本对象
        if (item_start_pos != item_end_pos
            && ((baseedit_ctrl_ptr->highlight.start_pos > item_end_pos)
                 || (baseflex_ctrl_ptr->is_tp_moved && end_pos < baseedit_ctrl_ptr->highlight.start_pos)))
        {
            baseedit_ctrl_ptr->highlight.end_pos = item_start_pos;
        }
        else
        {
            baseedit_ctrl_ptr->highlight.end_pos = item_end_pos;
        }
    }

    if ((baseedit_ctrl_ptr->highlight.is_underline) &&
        (baseedit_ctrl_ptr->highlight.start_pos == baseedit_ctrl_ptr->highlight.end_pos) &&
        (0 != baseedit_ctrl_ptr->im_ctrl_handle))
    {
        //clear im
        GUIIM_ClearState (baseedit_ctrl_ptr->im_ctrl_handle);
    }
}

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : display list index info
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlDisplayLeftNum (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    uint8           index_str[GUIEDIT_NUM_INFO_MAX_LEN] = {0};
    wchar           index_wstr[GUIEDIT_NUM_INFO_MAX_LEN] = {0};
    uint16          index_str_len = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    if ( (PNULL != baseflex_ctrl_ptr) &&
            (baseflex_ctrl_ptr->is_disp_num_info))
    {
        //get number info
        index_str_len = (uint16) sprintf ( (char *) index_str, "%d/%d", listedit_ctrl_ptr->item_num, listedit_ctrl_ptr->max_num);

        //convert wchar
        MMI_STRNTOWSTR (
                index_wstr, GUIEDIT_NUM_INFO_MAX_LEN,
                (const uint8 *) index_str, GUIEDIT_NUM_INFO_MAX_LEN,
                index_str_len);

        //set font
        GUIWIN_SetTitleSubTextParam (
                base_ctrl_ptr->win_handle,
                PNULL,
                &baseedit_ctrl_ptr->common_theme.info_font,
                PNULL);

        //display string
        GUIWIN_SetTitleSubText (
                base_ctrl_ptr->win_handle,
                TRUE,
                index_wstr,
                index_str_len,
                FALSE);
    }
}

/*****************************************************************************/
//  Description : 在添加字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlBeforeAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
)
{
    uint16      highlight_min_pos = 0;
    uint16      highlight_max_pos = 0;
    uint16      highlight_len = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    RemoveListItemObject (
            listedit_ctrl_ptr,
            highlight_min_pos,
            highlight_max_pos);

    AdjustListItemObject (
            listedit_ctrl_ptr,
            highlight_min_pos,
            (int16)(actual_str_ptr->str_len - highlight_len));
}

/*****************************************************************************/
//  Description : 在添加字符后做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlAfterAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
)
{
    uint16      highlight_min_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //add actual string
    if (0 < actual_str_ptr->str_len
        && add_str_ptr->is_list_item_object
        && PNULL != add_str_ptr->user_data_ptr)
    {
        MMI_STRING_T user_data = {0};

        user_data.wstr_ptr = add_str_ptr->user_data_ptr;
        user_data.wstr_len = add_str_ptr->user_data_len;

        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

        // 回车不算在object里
        if (actual_str_ptr->is_add_lf)
        {
            AddListItemObject (
                listedit_ctrl_ptr,
                (uint16) (highlight_min_pos + actual_str_ptr->len_before_cursor),
                (uint16) (actual_str_ptr->str_len - actual_str_ptr->len_before_cursor - 1),
                &user_data);
        }
        else
        {
            AddListItemObject (
                listedit_ctrl_ptr,
                (uint16) (highlight_min_pos + actual_str_ptr->len_before_cursor),
                (uint16) (actual_str_ptr->str_len - actual_str_ptr->len_before_cursor),
                &user_data);
        }
    }
}

/*****************************************************************************/
//  Description : 在删除字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlBeforeDelString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr      //in:
)
{
    uint16      highlight_min_pos = 0;
    uint16      highlight_max_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    RemoveListItemObject (
            listedit_ctrl_ptr,
            highlight_min_pos,
            highlight_max_pos);

    AdjustListItemObject (
            listedit_ctrl_ptr,
            highlight_min_pos,
            (int16) (highlight_min_pos - highlight_max_pos));
}

/*****************************************************************************/
//  Description : 检查被删字符
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 此方法在删除时用到，因为在删除动作时，在确保当前有字符被选中，像list风格的话，
// 需要保证item object对象被整体选中
/*****************************************************************************/
LOCAL void ListEditCtrlCheckDelStr (
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr
)
{
    uint16      delete_str_len = 0;
    uint16      start_pos = 0;
    uint16      end_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //one item for example:[LFxxx;] [xxx;] [xxx]
    //get highlight len
    delete_str_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    //have chars before cursor or highlight len is not equal 0
    if ((0 < baseedit_ctrl_ptr->cursor.cur_pos) ||
        (0 < delete_str_len))
    {
        //set delete string len
        if (0 == delete_str_len)
        {
            //set highlight start position
            SCI_ASSERT (0 < baseedit_ctrl_ptr->cursor.cur_pos); /*assert verified*/

            // 如果当前没有选中，且前面是文本对象，则删除文本对象
            if ((2 <= baseedit_ctrl_ptr->cursor.cur_pos) &&
                (PNULL != FindListItemObject (listedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 2))))
            {
                start_pos = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 2);
            }
            else
            {
                start_pos = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1);
            }

            end_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, start_pos, end_pos, SETHIGHLIGHT_BOTH);
        }

        // 判断前后字符的状态，根据这些状态来决定如何删除
        CheckListHighlightStr (listedit_ctrl_ptr);

        //modify current item number
        ModifyItemNumForDel (listedit_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 检查添加的字符
//  Global resource dependence :
//  Author: hua.fang
// TRUE: commit的字符已经拷贝到actual_str_ptr;
// FALSE: 不拷贝字符，需要后续处理
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ListEditCtrlCheckCommitStr (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
    uint16                      *delete_len_ptr         //out:
)
{
    BOOLEAN     is_ok = FALSE;
    uint16      commit_count = 0;
    uint16      underline_count = 0;
    uint16      item_num = 0;
    uint16      store_highlight_start_pos = 0;
    uint16      store_highlight_end_pos = 0;
    uint16      store_item_num;       /*!<item个数*/
    uint16      store_max_num;        /*!<最大item个数*/
    uint16      store_item_max_len;   /*!<每个item最大字符个数*/
    COMMIT_STR_FUNC commint_func = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    // 保留值，用于出错时恢复
    store_highlight_start_pos = baseedit_ctrl_ptr->highlight.start_pos;
    store_highlight_end_pos = baseedit_ctrl_ptr->highlight.end_pos;
    store_item_num = listedit_ctrl_ptr->item_num;
    store_max_num = listedit_ctrl_ptr->max_num;
    store_item_max_len = listedit_ctrl_ptr->item_max_len;

    //adjust highlight min and max position
    CheckListHighlightPos (
        listedit_ctrl_ptr,
        &baseedit_ctrl_ptr->highlight.start_pos,
        &baseedit_ctrl_ptr->highlight.end_pos);

    //modify current item number
    ModifyItemNumForDel (listedit_ctrl_ptr);

    if (add_str_ptr->is_list_item_object)
    {
        commint_func = GetCommitObject;
    }
    else
    {
        commint_func = GetCommitStr;
    }

    if (commint_func (listedit_ctrl_ptr, add_str_ptr, actual_str_ptr,
                      &commit_count, &underline_count, &item_num))        // 先获取有多个实际添加的字符
    {
        actual_str_ptr->str_ptr = SCI_ALLOC_APP (sizeof (wchar) * actual_str_ptr->str_len);
        SCI_MEMSET (actual_str_ptr->str_ptr, 0, sizeof (wchar) * actual_str_ptr->str_len);

        actual_str_ptr->len_before_cursor = 0;

        if (commint_func (listedit_ctrl_ptr, add_str_ptr, actual_str_ptr,
                          &commit_count, &underline_count, &item_num))    // 再次调用，用于获取实际的字符
        {
            add_str_ptr->commit_len = commit_count;
            add_str_ptr->underline_len = underline_count;
            listedit_ctrl_ptr->item_num = item_num;

            is_ok = TRUE;
        }
    }

    // 出错了，恢复原来的状态
    if (!is_ok)
    {
        listedit_ctrl_ptr->item_num = store_item_num;
        listedit_ctrl_ptr->max_num = store_max_num;
        listedit_ctrl_ptr->item_max_len = store_item_max_len;

        baseedit_ctrl_ptr->highlight.start_pos = store_highlight_start_pos;
        baseedit_ctrl_ptr->highlight.end_pos = store_highlight_end_pos;

        add_str_ptr->commit_len = 0;
        add_str_ptr->underline_len = 0;

        actual_str_ptr->str_len = 0;

        if (PNULL != actual_str_ptr->str_ptr)
        {
            SCI_FREE (actual_str_ptr->str_ptr);
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 当删除文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlOnTextDeleted(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    wchar       separator_char = 0;
    uint16      cur_item_len = 0;
    uint16      delete_str_len = 0;
    uint16      delete_pos = 0;
    uint16      i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //get current item string length except underline
    GetListItemLen (listedit_ctrl_ptr, &cur_item_len, PNULL);

    //删除后合并的item len是否超过最大值,是则截取前面的字符个数
    if (cur_item_len > listedit_ctrl_ptr->item_max_len)
    {
        //get list separator
        separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

        //get delete position
        for (i = baseedit_ctrl_ptr->cursor.cur_pos; i < baseedit_ctrl_ptr->str_len; i++)
        {
            if (separator_char == baseedit_ctrl_ptr->str_ptr[i])
            {
                delete_pos = i;
                break;
            }
        }

        SCI_ASSERT (delete_pos >= baseedit_ctrl_ptr->cursor.cur_pos); /*assert verified*/

        //delete excess string
        delete_str_len = (uint16) (cur_item_len - listedit_ctrl_ptr->item_max_len);
        SCI_ASSERT (delete_pos > delete_str_len); /*assert verified*/

        //move string forward
        for (i = delete_pos; i < baseedit_ctrl_ptr->str_len; i++)
        {
            baseedit_ctrl_ptr->str_ptr[i-delete_str_len] = baseedit_ctrl_ptr->str_ptr[i];
        }

        SCI_MEMSET(
                (baseedit_ctrl_ptr->str_ptr + baseedit_ctrl_ptr->str_len - delete_str_len),
                0, (delete_str_len * sizeof(wchar)));

        RemoveListItemObject (
                listedit_ctrl_ptr,
                (uint16) (delete_pos - delete_str_len),
                delete_pos);

        AdjustListItemObject (
                listedit_ctrl_ptr,
                (uint16) (delete_pos - delete_str_len),
                (int16) (-delete_str_len));

        baseedit_ctrl_ptr->str_len = (uint16) (baseedit_ctrl_ptr->str_len - delete_str_len);
    }

    // check list item state after delete
    CheckListItemState (listedit_ctrl_ptr);

    SendImSyncMsg(listedit_ctrl_ptr);
}

/*****************************************************************************/
//  Description : 当添加文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ListEditCtrlOnTextAdded(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_ADD_STR_T       *add_str_ptr
)
{
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = (CTRLLISTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    // check list item state in list
    CheckListItemState (listedit_ctrl_ptr);

    SendImSyncMsg(listedit_ctrl_ptr);
}

/*****************************************************************************/
//  Description : list add separator
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddSeparator (//is need add
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr
)
{
    BOOLEAN                 result = FALSE;
    wchar                   separator_char = 0;
    GUIIM_NOTIFY_PARAM_T    add_str = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) listedit_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)listedit_ctrl_ptr;

    //get list separator
    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    //is need add separator
    if ((0 < baseedit_ctrl_ptr->str_len) &&
        (separator_char != baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->str_len-1]))
    {
        SCI_ASSERT (baseedit_ctrl_ptr->cursor.cur_pos == baseedit_ctrl_ptr->str_len); /*assert verified*/

        listedit_ctrl_ptr->im_sync_type = CTRLLISTEDIT_IMSYNC_ADD;

        //add separator
        add_str.commit_len = 1;
        add_str.commit[0]  = separator_char;
        MMK_SendMsg (base_ctrl_ptr->handle, MSG_NOTIFY_IM_COMMIT, &add_str);

        /* -fixed by hongjun.jia for NEWMS00219424
         * -edit_ctrl_ptr->handle对MSG_NOTIFY_IM_COMMIT处理后，在separator_char后强制刷新光标；
         * -如果定时器没有到来之前，移动光标，会导致有光标残余；
         × -故此处强制将刷一下光标。
         */
        //reset cursor postion
        BASEFLEX_ResetCursor (baseflex_ctrl_ptr);
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1));

        // 移动光标后，需要同步光亮至光标处
        BASEEDIT_SetNoHighlight (baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get edit list item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetListItem (
    MMI_CTRL_ID_T			ctrl_id,        //in:
    uint16					array_len,      //in
    GUIEDIT_LIST_ITEM_T		*item_array_ptr,//out
    GUIEDIT_LIST_ITEM_STR_T	*item_str_array_ptr //out
)
{
    BOOLEAN             is_set_pos = FALSE;
    wchar               separator_char = 0;
    wchar               lf_char = LF_CHAR;
    uint16              i = 0;
    uint16              item_start_pos = 0;
    uint16              item_index = 0;
    GUIEDIT_OBJ_INFO_T  *obj_info_ptr = PNULL;
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    listedit_ctrl_ptr = GetListEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    if ((PNULL != listedit_ctrl_ptr)
        && (PNULL != baseedit_ctrl_ptr)
        && (PNULL != item_array_ptr || PNULL != item_str_array_ptr))
    {
        //get list separator
        separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

        for (i = 0; i < baseedit_ctrl_ptr->str_len; i++)
        {
            if (is_set_pos)
            {
                //set item start position
                item_start_pos = i;
                is_set_pos = FALSE;
            }

            obj_info_ptr = FindListItemObject (listedit_ctrl_ptr, i);

            if (PNULL != obj_info_ptr
                    || separator_char == baseedit_ctrl_ptr->str_ptr[i])
            {
                if (item_index < array_len)
                {
                    // 是文本对象的情况
                    if (PNULL != obj_info_ptr)
                    {
                        if (PNULL != item_array_ptr)
                        {
                            item_array_ptr[item_index].is_object = TRUE;

                            item_array_ptr[item_index].display_str_ptr = baseedit_ctrl_ptr->str_ptr + obj_info_ptr->start;
                            SCI_ASSERT (obj_info_ptr->len > 0);     /*assert verified*/
                            item_array_ptr[item_index].display_str_len = (uint16) (obj_info_ptr->len - 1); // 去掉后面的分号分隔符
                            item_array_ptr[item_index].user_str_ptr = obj_info_ptr->user_data.wstr_ptr;
                            item_array_ptr[item_index].user_str_len = obj_info_ptr->user_data.wstr_len;
                        }
                        else if (PNULL != item_str_array_ptr)
                        {
                            item_str_array_ptr[item_index].str_ptr  = obj_info_ptr->user_data.wstr_ptr;
                            item_str_array_ptr[item_index].str_len  = obj_info_ptr->user_data.wstr_len;
                        }

                        i = (uint16) (i + obj_info_ptr->len);

                        // i++ will work and point to the LF_CHAR, so set TRUE here.
                        is_set_pos = TRUE;
                    }
                    else
                    {
                        if (PNULL != item_array_ptr)
                        {
                            item_array_ptr[item_index].is_object = FALSE;

                            item_array_ptr[item_index].display_str_ptr = baseedit_ctrl_ptr->str_ptr + item_start_pos;
                            item_array_ptr[item_index].display_str_len = (uint16) (i - item_start_pos);
                            item_array_ptr[item_index].user_str_ptr = item_array_ptr[item_index].display_str_ptr;   // 与display_str_ptr保持一致，只要用user_str_ptr就可以判断号码
                            item_array_ptr[item_index].user_str_len = item_array_ptr[item_index].display_str_len;
                        }
                        else if (PNULL != item_str_array_ptr)
                        {
                            item_str_array_ptr[item_index].str_ptr  = baseedit_ctrl_ptr->str_ptr + item_start_pos;
                            item_str_array_ptr[item_index].str_len  = (uint16) (i - item_start_pos);
                        }
                    }

                    if (PNULL != item_array_ptr)
                    {
                        item_array_ptr[item_index].is_valid = IsValidItem (
                                                                  item_array_ptr[item_index].user_str_ptr,
                                                                  item_array_ptr[item_index].user_str_len,
                                                                  (GUIEDIT_LIST_STYLE_E) listedit_ctrl_ptr->theme.list_style);
                    }
                    else if (PNULL != item_str_array_ptr)
                    {
                        item_str_array_ptr[item_index].is_valid = IsValidItem (
                                    item_str_array_ptr[item_index].str_ptr,
                                    item_str_array_ptr[item_index].str_len,
                                    (GUIEDIT_LIST_STYLE_E) listedit_ctrl_ptr->theme.list_style);
                    }

                    item_index++;

                    if (item_index == array_len)
                    {
                        break;
                    }
                }
            }
            else if (lf_char == baseedit_ctrl_ptr->str_ptr[i])
            {
                //set item start position
                is_set_pos = TRUE;
            }
        }

        if ( (item_index < listedit_ctrl_ptr->item_num) &&
                (item_index < array_len))
        {
            if (PNULL != item_array_ptr)
            {
                item_array_ptr[item_index].is_object = FALSE;

                item_array_ptr[item_index].display_str_ptr  = baseedit_ctrl_ptr->str_ptr + item_start_pos;
                item_array_ptr[item_index].display_str_len  = (uint16) (i - item_start_pos);
                item_array_ptr[item_index].user_str_ptr = item_array_ptr[item_index].display_str_ptr;
                item_array_ptr[item_index].user_str_len = item_array_ptr[item_index].display_str_len;

                item_array_ptr[item_index].is_valid = IsValidItem (
                                                          item_array_ptr[item_index].user_str_ptr,
                                                          item_array_ptr[item_index].user_str_len,
                                                          (GUIEDIT_LIST_STYLE_E) listedit_ctrl_ptr->theme.list_style);
            }
            else if (PNULL != item_str_array_ptr)
            {
                item_str_array_ptr[item_index].str_ptr  = baseedit_ctrl_ptr->str_ptr + item_start_pos;
                item_str_array_ptr[item_index].str_len  = (uint16) (i - item_start_pos);

                item_str_array_ptr[item_index].is_valid = IsValidItem (
                            item_str_array_ptr[item_index].str_ptr,
                            item_str_array_ptr[item_index].str_len,
                            (GUIEDIT_LIST_STYLE_E) listedit_ctrl_ptr->theme.list_style);
            }
        }
    }
}

/*****************************************************************************/
//  Description : get list item string length and start position,except underline
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemLen (//光标前面是;增加代表新增一个item
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,
    uint16              *item_len_ptr,  //in/out:
    uint16              *start_pos_ptr  //in/out:may PNULL item start position
)
{
    BOOLEAN     is_new_item = FALSE;
    wchar       separator_char = 0;
    wchar       lf_char = LF_CHAR;
    uint16      highlight_min_pos = 0;
    uint16      highlight_max_pos = 0;
    uint16      start_pos = 0;
    uint16      item_len = 0;
    uint16      item_start_pos = 0;
    int32       i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    //get list separator
    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    CheckListHighlightPos (listedit_ctrl_ptr, &highlight_min_pos, &highlight_max_pos);

    //set start position
    start_pos = highlight_min_pos;

    //find the item string forward
    if (0 < start_pos)
    {
        for (i = (start_pos - 1); i >= 0; i--)
        {
            if (lf_char == baseedit_ctrl_ptr->str_ptr[i]
                    || separator_char == baseedit_ctrl_ptr->str_ptr[i])
            {
                break;
            }
        }

        //set item string length
        if (0 < i)
        {
            item_len = (uint16) (start_pos - i - 1);
        }
        else
        {
            item_len = start_pos;
        }

        //set item start position
        item_start_pos = (uint16) (start_pos - item_len);
    }

    //set start position except underline
    start_pos = highlight_max_pos;

    //find the item string backward
    for (i = start_pos; i < baseedit_ctrl_ptr->str_len; i++)
    {
        // 如果下一个是文本对象，那到这里结束
        if (PNULL != FindListItemObject (listedit_ctrl_ptr, (uint16) i))
        {
            i = start_pos;
            break;
        }
        else if (separator_char == baseedit_ctrl_ptr->str_ptr[i])
        {
            break;
        }
    }

    //set item string length
    SCI_ASSERT (i >= start_pos); /*assert verified*/
    item_len = (uint16) (item_len + i - start_pos);

    //set output
    if (PNULL != start_pos_ptr)
    {
        *start_pos_ptr = item_start_pos;
    }

    if (PNULL != item_len_ptr)
    {
        *item_len_ptr = item_len;
    }

    return (is_new_item);
}

/*****************************************************************************/
//  Description : current cursor position is the first position in one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemFirstPos (
    CTRLLISTEDIT_OBJ_T* listedit_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;
    wchar       separator_char = 0;
    EDIT_ITEM_STATE_E   prev_item_state = EDIT_ITEM_STATE_INVALID;

    //get list separator
    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    GetPrevCharState (listedit_ctrl_ptr, separator_char, &prev_item_state, PNULL);

    if (EDIT_ITEM_STATE_START == prev_item_state
        || EDIT_ITEM_STATE_SEPARATOR == prev_item_state
        || EDIT_ITEM_STATE_LF == prev_item_state)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get start and end pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemPos (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,  // [in]
    uint16              pos,             // [in]
    uint16              *start_pos_ptr,  // [out]
    uint16              *end_pos_ptr     // [out]
)
{
    BOOLEAN             is_item = FALSE;
    GUIEDIT_OBJ_INFO_T  *obj_info_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    if ( (PNULL == start_pos_ptr) || (PNULL == end_pos_ptr))
    {
        return is_item;
    }

    obj_info_ptr = FindListItemObject (listedit_ctrl_ptr, pos);

    if (PNULL == obj_info_ptr)  // 此位置不是OBJECT
    {
        *start_pos_ptr = pos;
        *end_pos_ptr = pos;
    }
    else                        // 此位置为OBJECT
    {
        *start_pos_ptr = obj_info_ptr->start;
        *end_pos_ptr   = (uint16) (obj_info_ptr->start + obj_info_ptr->len);

        is_item = TRUE;
    }

    if (*start_pos_ptr > baseedit_ctrl_ptr->str_len)
    {
        *start_pos_ptr = baseedit_ctrl_ptr->str_len;
    }

    if (*end_pos_ptr > baseedit_ctrl_ptr->str_len)
    {
        *end_pos_ptr = baseedit_ctrl_ptr->str_len;
    }

    return is_item;
}

/*****************************************************************************/
//  Description : check list item object state
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//   PREV |  CUR  |  NEXT
//=========================
//        |       |   E
//    X   |   S   |   C
//        |       |   O
//-------------------------
//    S   |       |
//    ;   |       |
//    C   |   E   |   X
//    O   |       |
//-------------------------
//        |       |   E
//    C   |   ;   |   LF
//-------------------------
//    O   |       |   C
//    ;   |   LF  |   O
//-------------------------
//    S   |       |   E
//    LF  |   C   |   ;
//    C   |       |   C
//-------------------------
//    S   |       |   E
//    LF  |   O   |   LF
//=========================
//
// X  : none
// S  : start
// E  : end
// C  : normal char
// O  : item object
// ;  : separator
// LF : LF_CHAR
/*****************************************************************************/
LOCAL BOOLEAN CheckListItemState (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr
)
{
    EDIT_ITEM_STATE_E   pre_item_state = EDIT_ITEM_STATE_START;
    EDIT_ITEM_STATE_E   cur_item_state = EDIT_ITEM_STATE_START;
    uint16              index = 0;
    wchar               lf_char = LF_CHAR;
    wchar               separator_char = 0;
    wchar               next_char = 0;
    uint16              start_pos = 0;
    uint16              end_pos = 0;
    uint16              item_count = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    while (EDIT_ITEM_STATE_END != cur_item_state)
    {
        // 获得当前字符的状态
        if (index < baseedit_ctrl_ptr->str_len)
        {
            if (GetListItemPos (listedit_ctrl_ptr, index, &start_pos, &end_pos))
            {
                cur_item_state = EDIT_ITEM_STATE_OBJECT;

                index = end_pos;
            }
            else
            {
                next_char = baseedit_ctrl_ptr->str_ptr[index];

                if (next_char == lf_char)
                {
                    cur_item_state = EDIT_ITEM_STATE_LF;
                }
                else if (next_char == separator_char)
                {
                    cur_item_state = EDIT_ITEM_STATE_SEPARATOR;
                }
                else
                {
                    cur_item_state = EDIT_ITEM_STATE_CHAR;
                }

                index++;
            }
        }
        else
        {
            cur_item_state = EDIT_ITEM_STATE_END;
        }

        // 根据状态机判断
        switch (pre_item_state)
        {
        case EDIT_ITEM_STATE_START:
            switch (cur_item_state)
            {
            case EDIT_ITEM_STATE_CHAR:
            case EDIT_ITEM_STATE_OBJECT:
            case EDIT_ITEM_STATE_END:
                pre_item_state = cur_item_state;
                break;

            case EDIT_ITEM_STATE_SEPARATOR:
            case EDIT_ITEM_STATE_LF:
            case EDIT_ITEM_STATE_START:
            default:
                SCI_PASSERT (0, ("pre: %d, cur: %d", pre_item_state, cur_item_state));  /*assert verified*/
                break;
            }

            break;

        case EDIT_ITEM_STATE_CHAR:
            switch (cur_item_state)
            {
            case EDIT_ITEM_STATE_END:
                item_count++;
                pre_item_state = cur_item_state;
                break;

            case EDIT_ITEM_STATE_CHAR:
            case EDIT_ITEM_STATE_SEPARATOR:
                pre_item_state = cur_item_state;
                break;

            case EDIT_ITEM_STATE_OBJECT:
            case EDIT_ITEM_STATE_LF:
            case EDIT_ITEM_STATE_START:
            default:
                SCI_PASSERT (0, ("pre: %d, cur: %d", pre_item_state, cur_item_state));  /*assert verified*/
                break;
            }

            break;

        case EDIT_ITEM_STATE_OBJECT:
            switch (cur_item_state)
            {
            case EDIT_ITEM_STATE_END:
            case EDIT_ITEM_STATE_LF:
                item_count++;
                pre_item_state = cur_item_state;
                break;

            case EDIT_ITEM_STATE_CHAR:
            case EDIT_ITEM_STATE_OBJECT:
            case EDIT_ITEM_STATE_SEPARATOR:
            case EDIT_ITEM_STATE_START:
            default:
                SCI_PASSERT (0, ("pre: %d, cur: %d", pre_item_state, cur_item_state));  /*assert verified*/
                break;
            }

            break;

        case EDIT_ITEM_STATE_SEPARATOR:
            switch (cur_item_state)
            {
            case EDIT_ITEM_STATE_END:
            case EDIT_ITEM_STATE_LF:
                item_count++;
                pre_item_state = cur_item_state;
                break;

            case EDIT_ITEM_STATE_CHAR:
            case EDIT_ITEM_STATE_OBJECT:
            case EDIT_ITEM_STATE_SEPARATOR:
            case EDIT_ITEM_STATE_START:
            default:
                SCI_PASSERT (0, ("pre: %d, cur: %d", pre_item_state, cur_item_state));  /*assert verified*/
                break;
            }

            break;

        case EDIT_ITEM_STATE_LF:
            switch (cur_item_state)
            {
            case EDIT_ITEM_STATE_CHAR:
            case EDIT_ITEM_STATE_OBJECT:
                pre_item_state = cur_item_state;
                break;

            case EDIT_ITEM_STATE_SEPARATOR:
            case EDIT_ITEM_STATE_LF:
            case EDIT_ITEM_STATE_END:
            case EDIT_ITEM_STATE_START:
            default:
                SCI_PASSERT (0, ("pre: %d, cur: %d", pre_item_state, cur_item_state));  /*assert verified*/
                break;
            }

            break;

        case EDIT_ITEM_STATE_END:
        default:
            SCI_PASSERT (0, ("pre: %d, cur: %d", pre_item_state, cur_item_state));  /*assert verified*/
            break;
        }
    }

    SCI_ASSERT (item_count == listedit_ctrl_ptr->item_num);   /*assert verified*/

    return TRUE;
}

/*****************************************************************************/
//  Description : select current item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// this method is for GUIEDIT_ReplaceCurListItem to highlight all current item
// text and replace it.
/*****************************************************************************/
LOCAL BOOLEAN SelectCurItem (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr
)
{
    wchar       separator_char = 0;
    uint16      prev_char_count = 0;
    uint16      next_char_count = 0;
    EDIT_ITEM_STATE_E   pre_item_state = EDIT_ITEM_STATE_START;
    EDIT_ITEM_STATE_E   next_item_state = EDIT_ITEM_STATE_START;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    GetPrevCharState (listedit_ctrl_ptr, separator_char, &pre_item_state, &prev_char_count);

    GetNextCharState (listedit_ctrl_ptr, separator_char, &next_item_state, &next_char_count);

    SCI_ASSERT (baseedit_ctrl_ptr->highlight.start_pos >= prev_char_count); /*assert verified*/
    SCI_ASSERT (baseedit_ctrl_ptr->highlight.end_pos + next_char_count <= baseedit_ctrl_ptr->str_len);/*assert verified*/

    baseedit_ctrl_ptr->highlight.start_pos -= prev_char_count;
    baseedit_ctrl_ptr->highlight.end_pos += next_char_count;

    CheckListHighlightStr (listedit_ctrl_ptr);

    baseedit_ctrl_ptr->cursor.cur_pos = baseedit_ctrl_ptr->highlight.end_pos;

    return TRUE;
}

/*****************************************************************************/
//  Description : handle edit list direction msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDirectionKey (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,
    GUIEDIT_CURSOR_MOVE_E   move_direction
)
{
    BOOLEAN             is_add_separator = FALSE;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    uint16              min_highlight_pos = 0;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)listedit_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    //judge is need add separator,need than add
    is_add_separator = AddSeparator (listedit_ctrl_ptr);

    //have highlight
    if (baseedit_ctrl_ptr->highlight.start_pos != baseedit_ctrl_ptr->highlight.end_pos)
    {
        min_highlight_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

        if (GUIEDIT_CURSOR_MOVE_UP == move_direction
            && PNULL != FindListItemObject (listedit_ctrl_ptr, min_highlight_pos))
        {
            //reset date cursor not display
            BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

            //reset highlight not display
            BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

            // when cursor contains object, the cursor may be at max_pos in highlight,
            // and it will be still in this highlight after GetNewCursorPosForUpDown.
            // so set value directly.
            if (0 != min_highlight_pos)
            {
                // TODO: set highlight and cursor directly
                baseedit_ctrl_ptr->highlight.start_pos = min_highlight_pos;
                baseedit_ctrl_ptr->highlight.end_pos = min_highlight_pos;
                baseedit_ctrl_ptr->cursor.cur_pos = min_highlight_pos;
            }
        }
        else if (GUIEDIT_CURSOR_MOVE_LEFT == move_direction
            && PNULL != FindListItemObject (listedit_ctrl_ptr, min_highlight_pos))
        {
            //reset date cursor not display
            BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

            //reset highlight not display
            BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

            // when cursor contains object, the cursor may be at max_pos in highlight,
            // and it will be still in this highlight after GetNewCursorPosForUpDown.
            // so set value directly.
            // TODO: set highlight and cursor directly
            baseedit_ctrl_ptr->highlight.start_pos = min_highlight_pos;
            baseedit_ctrl_ptr->highlight.end_pos = min_highlight_pos;
            baseedit_ctrl_ptr->cursor.cur_pos = min_highlight_pos;
        }
    }

    //move cursor
    if (!BASEFLEX_MoveCursor (baseflex_ctrl_ptr, move_direction))
    {
        if (is_add_separator)
        {
            //reset cursor not display
            BASEFLEX_ResetCursor (baseflex_ctrl_ptr);

            //set display cursor
            baseedit_ctrl_ptr->cursor.is_display    = TRUE;
            baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;

            //display cursor
            BASEFLEX_DisplayCursor (baseflex_ctrl_ptr);
        }

        result = MMI_RESULT_FALSE;
    }
    else
    {
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);
    }

    //config im
    VTLBASEEDIT_ConfigImKey (baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : check list highlist string while adding
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckListHighlightPos (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,         // [in]
    uint16              *highlight_min_pos_ptr, // [out]
    uint16              *highlight_max_pos_ptr  // [out]
)
{
    BOOLEAN is_changed = FALSE;
    uint16  highlight_min_pos = 0;
    uint16  highlight_max_pos = 0;
    wchar   separator_char = 0;
    wchar   lf_char = LF_CHAR;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    //get highlight min and max position
    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    //adjust highlight min and max position
    if (highlight_min_pos != highlight_max_pos)
    {
        //get list separator
        separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

        // ;|lf
        if ((lf_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos]) &&
            (0 < highlight_min_pos) &&
            (separator_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos-1]))
        {
            //the first highlight is LF
            highlight_min_pos++;
            is_changed = TRUE;
        }

        // ;|lf
        if ((0 < highlight_max_pos) &&
            (separator_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos-1]) &&
            (highlight_max_pos < baseedit_ctrl_ptr->str_len) &&
            (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos]))
        {
            SCI_ASSERT (highlight_max_pos + 1 < baseedit_ctrl_ptr->str_len); /*assert verified*/

            //the last highlight is separator
            highlight_max_pos++;

            is_changed = TRUE;
        }
    }

    if (PNULL != highlight_min_pos_ptr)
    {
        *highlight_min_pos_ptr = highlight_min_pos;
    }

    if (PNULL != highlight_max_pos_ptr)
    {
        *highlight_max_pos_ptr = highlight_max_pos;
    }

    return is_changed;
}

/*****************************************************************************/
//  Description : check list highlist string while deleting, assure list highlight string
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  to assure the correction after delete, we should check previous and next
//  char which should be one of the following:
//  ------------------
//  |prev        next|
//  ------------------
//  | X           X  |
//  | ;           ;  |
//  | lf          lf |
//  | C           C  |
//  | O           O  |
//  ------------------
//  X means the start or end position of string.string
//  ; means the separator char
//  lf means the return char
//  C means a valid char
//  O means an object
/*****************************************************************************/
LOCAL void CheckListHighlightStr (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr  // [in]
)
{
    uint16              highlight_min_pos = 0;
    uint16              highlight_max_pos = 0;
    wchar               lf_char = LF_CHAR;
    wchar               separator_char = 0;
    EDIT_ITEM_STATE_E   prev_char_state = EDIT_ITEM_STATE_START;
    EDIT_ITEM_STATE_E   next_char_state = EDIT_ITEM_STATE_END;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    if (highlight_min_pos != highlight_max_pos)
    {
        // GetPrevCharState does not check object
        if (0 == highlight_min_pos)
        {
            prev_char_state = EDIT_ITEM_STATE_START;
        }
        else if (PNULL != FindListItemObject (listedit_ctrl_ptr, (uint16) (highlight_min_pos - 1)))
        {
            prev_char_state = EDIT_ITEM_STATE_OBJECT;
        }
        else if (separator_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos - 1])
        {
            prev_char_state = EDIT_ITEM_STATE_SEPARATOR;
        }
        else if (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos - 1])
        {
            prev_char_state = EDIT_ITEM_STATE_LF;
        }
        else
        {
            prev_char_state = EDIT_ITEM_STATE_CHAR;
        }

        GetNextCharState (listedit_ctrl_ptr, separator_char, &next_char_state, PNULL);

        // all states, you can see the note in CheckListHighlightStr
        if (EDIT_ITEM_STATE_START == prev_char_state)
        {
            if (EDIT_ITEM_STATE_SEPARATOR == next_char_state)   // S|;的情况
            {
                highlight_max_pos++;

                if (highlight_max_pos != baseedit_ctrl_ptr->str_len)    // S|;lf的情况
                {
                    SCI_ASSERT (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos]);  /*assert verified*/
                    highlight_max_pos++;
                }
            }
            else if (EDIT_ITEM_STATE_LF == next_char_state)     // S|lf的情况
            {
                highlight_max_pos++;
            }
        }
        else if (EDIT_ITEM_STATE_SEPARATOR == prev_char_state)
        {
            if (EDIT_ITEM_STATE_SEPARATOR == next_char_state)   // ;|;
            {
                highlight_max_pos++;
            }
            else if (EDIT_ITEM_STATE_CHAR == next_char_state)
            {
                if (PNULL != FindListItemObject (listedit_ctrl_ptr, highlight_min_pos))   // o|c，前面的分隔符是OBJECT里的
                {
                    highlight_min_pos++;
                }
                else                                            // ;|c
                {
                    highlight_min_pos--;
                }
            }
            else if (EDIT_ITEM_STATE_OBJECT == next_char_state) // ;|o
            {
                highlight_min_pos++;
            }
        }
        else if (EDIT_ITEM_STATE_LF == prev_char_state)
        {
            if (EDIT_ITEM_STATE_END == next_char_state)         // lf|E
            {
                highlight_min_pos--;
            }
            else if (EDIT_ITEM_STATE_SEPARATOR == next_char_state)  // lf|;
            {
                // 回车前一个字符肯定是分隔符
                highlight_min_pos--;
                SCI_ASSERT (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos]);  /*assert verified*/

                // 分隔符后面要么是回车，要么这是结尾处
                highlight_max_pos++;
                SCI_ASSERT (highlight_max_pos == baseedit_ctrl_ptr->str_len /*assert verified*/
                            || lf_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos]);
            }
            else if (EDIT_ITEM_STATE_LF == next_char_state)     // lf|lf
            {
                highlight_max_pos++;
            }
        }
        else if (EDIT_ITEM_STATE_CHAR == prev_char_state)
        {
            if (EDIT_ITEM_STATE_LF == next_char_state)          // c|lf
            {
                highlight_max_pos++;

                // 如果再后面是一个OBJECT，那不能组成co这种情况，所以需要把回退
                // 到lf之前，这个肯定是分隔符
                if (highlight_max_pos < baseedit_ctrl_ptr->str_len
                    && PNULL != FindListItemObject (listedit_ctrl_ptr, highlight_max_pos))
                {
                    highlight_max_pos -= 2;;
                }
            }
            else if (EDIT_ITEM_STATE_OBJECT == next_char_state) // c|o
            {
                // 回到两个字符，至分隔符
                highlight_max_pos -= 2;;
            }
        }
        else if (EDIT_ITEM_STATE_OBJECT == prev_char_state)
        {
            if (EDIT_ITEM_STATE_SEPARATOR == next_char_state)   // o|;
            {
                // OBJECT里自带了分隔符，所以后面这个需要忽略
                highlight_max_pos++;

                SCI_ASSERT (highlight_max_pos == baseedit_ctrl_ptr->str_len /*assert verified*/
                            || lf_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos]);
            }
            else if (EDIT_ITEM_STATE_CHAR == next_char_state    // o|c
                     || EDIT_ITEM_STATE_OBJECT == next_char_state)   // o|o
            {
                // 保留o后面的回车
                SCI_ASSERT (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos]);  /*assert verified*/
                highlight_min_pos++;
            }
        }
        else
        {
            SCI_ASSERT (0); /*assert verified*/
        }

        if (highlight_max_pos != BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr))
        {
            listedit_ctrl_ptr->im_sync_type = CTRLLISTEDIT_IMSYNC_DEL;
        }
    }

    SCI_ASSERT (highlight_min_pos >= 0 && highlight_min_pos <= baseedit_ctrl_ptr->str_len); /*assert verified*/
    SCI_ASSERT (highlight_max_pos >= 0 && highlight_max_pos <= baseedit_ctrl_ptr->str_len); /*assert verified*/

    if (highlight_min_pos > baseedit_ctrl_ptr->str_len)
    {
        highlight_min_pos = baseedit_ctrl_ptr->str_len;
    }

    if (highlight_max_pos > baseedit_ctrl_ptr->str_len)
    {
        highlight_max_pos = baseedit_ctrl_ptr->str_len;
    }

    baseedit_ctrl_ptr->highlight.start_pos = highlight_min_pos;
    baseedit_ctrl_ptr->highlight.end_pos = highlight_max_pos;
}

/*****************************************************************************/
//  Description : after delete string,modify current item number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyItemNumForDel (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr
)
{
    wchar       lf_char = LF_CHAR;
    uint16      highlight_len = 0;
    uint16      highlight_min_pos = 0;
    uint16      highlight_max_pos = 0;
    uint16      i = 0;
    uint16      delete_item_num = 0;
    uint16      start_pos = 0;
    uint16      end_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    //get highlight len
    highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    //calculate delete item num
    if (0 == highlight_len)
    {
    }
    else if (highlight_len == baseedit_ctrl_ptr->str_len)
    {
        listedit_ctrl_ptr->item_num = 0;
    }
    else
    {
        //get highlight min pos
        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
        highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

        i = 0;

        while (i < highlight_len)
        {
            if (GetListItemPos (listedit_ctrl_ptr, (uint16) (highlight_min_pos + i), &start_pos, &end_pos))
            {
                // end_pos有可能大于highlight_max_pos，因为有可能会用到文本
                // 对象里的分隔符而往前一位
                i = (uint16) (end_pos - highlight_min_pos);
            }
            else
            {
                if (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos+i])
                {
                    delete_item_num++;
                }

                i++;
            }
        }

        // 这种情况，后面的回车也会被删除掉，所以不算一个item了
        if (highlight_max_pos < baseedit_ctrl_ptr->str_len
                && lf_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos]
                && (0 == highlight_min_pos || lf_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos-1]))
        {
            delete_item_num++;
        }

        SCI_ASSERT (listedit_ctrl_ptr->item_num >= delete_item_num); /*assert verified*/
        listedit_ctrl_ptr->item_num = (uint16) (listedit_ctrl_ptr->item_num - delete_item_num);
    }
}

/*****************************************************************************/
//  Description : get state before min_highlight_pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    此方法不计算前面是否是OBJECT状态
/*****************************************************************************/
LOCAL void GetPrevCharState (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,     // in
    wchar                   separator_char,         // in
    EDIT_ITEM_STATE_E       *pre_item_state_ptr,    // out
    uint16                  *prev_char_count_ptr    // out
)
{
    uint16      highlight_min_pos = 0;
    wchar       lf_char = LF_CHAR;
    int32       i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

    // find prev char state and its count
    if (0 == highlight_min_pos)
    {
        *pre_item_state_ptr = EDIT_ITEM_STATE_START;
    }
    else if (separator_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos - 1])
    {
        *pre_item_state_ptr = EDIT_ITEM_STATE_SEPARATOR;
    }
    else if (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_min_pos - 1])
    {
        *pre_item_state_ptr = EDIT_ITEM_STATE_LF;
    }
    else
    {
        // 这里并没有判断前面是否是OBJECT的状态
        *pre_item_state_ptr = EDIT_ITEM_STATE_CHAR;

        // 计算此位置至item起始位置的字符数
        if (PNULL != prev_char_count_ptr)
        {
            for (i = highlight_min_pos - 1; i >= 0; i--)
            {
                SCI_ASSERT (separator_char != baseedit_ctrl_ptr->str_ptr[i]);   /*assert verified*/

                if (lf_char == baseedit_ctrl_ptr->str_ptr[i])
                {
                    break;
                }

                (*prev_char_count_ptr) ++;
            }

            SCI_ASSERT ( (*prev_char_count_ptr) > 0); /*assert verified*/
        }
    }
}

/*****************************************************************************/
//  Description : get state after max_highlight_pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetNextCharState (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,     // in
    wchar                   separator_char,         // in
    EDIT_ITEM_STATE_E       *next_item_state_ptr,   // out
    uint16                  *next_char_count_ptr    // out
)
{
    uint16      highlight_max_pos = 0;
    wchar       lf_char = LF_CHAR;
    int32       i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    highlight_max_pos = BASEEDIT_GetHighlightMaxPos (baseedit_ctrl_ptr);

    // find next char state and its count
    if (highlight_max_pos == baseedit_ctrl_ptr->str_len)
    {
        *next_item_state_ptr = EDIT_ITEM_STATE_END;
    }
    else if (PNULL != FindListItemObject (listedit_ctrl_ptr, highlight_max_pos))
    {
        *next_item_state_ptr = EDIT_ITEM_STATE_OBJECT;
    }
    else if (lf_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos])
    {
        *next_item_state_ptr = EDIT_ITEM_STATE_LF;
    }
    else if (separator_char == baseedit_ctrl_ptr->str_ptr[highlight_max_pos])
    {
        *next_item_state_ptr = EDIT_ITEM_STATE_SEPARATOR;
    }
    else
    {
        *next_item_state_ptr = EDIT_ITEM_STATE_CHAR;

        // 计算出当前位置至此item结束的字符数
        if (PNULL != next_char_count_ptr)
        {
            for (i = highlight_max_pos; i < baseedit_ctrl_ptr->str_len; i++)
            {
                SCI_ASSERT (lf_char != baseedit_ctrl_ptr->str_ptr[i]);  /*assert verified*/

                if (separator_char == baseedit_ctrl_ptr->str_ptr[i])
                {
                    break;
                }

                (*next_char_count_ptr) ++;
            }
        }
    }
}

/*****************************************************************************/
//  Description : get state of current pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL EDIT_ITEM_STATE_E GetCurrCharState (
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr,     // in
    wchar                   separator_char,         // in
    GUIEDIT_ADD_STR_T       *add_str_ptr,           // in
    int32                   index,                  // in
    EDIT_ITEM_STATE_E       pre_item_state,         // in
    wchar                   *cur_char_ptr           // out
)
{
    wchar               cur_char = 0;
    wchar               lf_char = LF_CHAR;
    EDIT_ITEM_STATE_E   cur_item_state = EDIT_ITEM_STATE_START;

    if (index < add_str_ptr->commit_len)
    {
        cur_char = add_str_ptr->commit_ptr[index];
    }
    else
    {
        cur_char = add_str_ptr->underline_ptr[index - add_str_ptr->commit_len];
    }

    // 这里，用户传进来的数据如果是回车和分隔符，都将作为分隔符状态
    if (lf_char == cur_char || separator_char == cur_char)
    {
        cur_item_state = EDIT_ITEM_STATE_SEPARATOR;
    }
    else
    {
        // 判断是否是有效字符
        switch (pre_item_state)
        {
        case EDIT_ITEM_STATE_START:
        case EDIT_ITEM_STATE_SEPARATOR:
        case EDIT_ITEM_STATE_LF:
            if ((IsListChar (listedit_ctrl_ptr, cur_char)) ||
                ('*' == cur_char) ||
                ('+' == cur_char) ||
                ('#' == cur_char))
            {
                cur_item_state = EDIT_ITEM_STATE_CHAR;
            }
            else
            {
                cur_item_state = EDIT_ITEM_STATE_INVALID;
            }

            break;

        case EDIT_ITEM_STATE_CHAR:
            if ((IsListChar (listedit_ctrl_ptr, cur_char)) ||
                ('*' == cur_char) ||
                ('+' == cur_char) ||
                ('#' == cur_char) ||
                ('P' == cur_char) ||
                ('W' == cur_char) ||
                (separator_char == cur_char))
            {
                cur_item_state = EDIT_ITEM_STATE_CHAR;
            }
            else
            {
                cur_item_state = EDIT_ITEM_STATE_INVALID;
            }

            break;

        default:
            SCI_ASSERT (0); /*assert verified*/
            break;
        }
    }

    *cur_char_ptr = cur_char;

    return cur_item_state;
}

/*****************************************************************************/
//  Description : add commit char in actual_str_ptr
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    it will check item count and actual count
/*****************************************************************************/
LOCAL void AddCommitStr (
    CTRLLISTEDIT_OBJ_T       *listedit_ctrl_ptr,     // in:
    GUIEDIT_ADD_ACTUAL_STR_T *actual_str_ptr,       // in/out
    uint16                   char_pos,              // in
    wchar                    cur_char,              // in
    uint16                   *cur_item_num_ptr,     // in/out
    uint16                   *actual_count_ptr      // in/out
)
{
    // 如果是0，代表删除最后一个记录的数据
    if (0 == cur_char)
    {
        SCI_ASSERT ( (*actual_count_ptr) > 0);  /*assert verified*/
        (*actual_count_ptr)--;
    }

    // 拷贝数据
    if (PNULL != actual_str_ptr->str_ptr
            && char_pos < actual_str_ptr->str_len)
    {
        actual_str_ptr->str_ptr[char_pos] = cur_char;

        if (LF_CHAR == cur_char ||
            cur_char == BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style))
        {
            listedit_ctrl_ptr->im_sync_type = CTRLLISTEDIT_IMSYNC_ADD;
        }
    }

    // 以回车来判断是否新item
    if (LF_CHAR == cur_char)
    {
        (*cur_item_num_ptr) ++;
    }

    // 实际字符串长度
    if (0 != cur_char)
    {
        (*actual_count_ptr) ++;
    }
}

/*****************************************************************************/
//  Description : get commit string when adding normal string
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    一个原则:尽量将有效的数据保存下来，如果非有效数据，那就退出
/*****************************************************************************/
LOCAL BOOLEAN GetCommitStr (
    CTRLLISTEDIT_OBJ_T          *listedit_ctrl_ptr,     // in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,       // in
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,    // out
    uint16                      *commit_count_ptr,  // out
    uint16                      *underline_count_ptr,   // out
    uint16                      *item_num_ptr       // out
)
{
    BOOLEAN		result = FALSE;
    BOOLEAN     is_over = FALSE;
    uint16      commit_count = 0;
    uint16      underline_count = 0;
    uint16      actual_char_count = 0;
    uint16      cur_item_count = 0;
    uint16      max_len = 0;
    uint16      max_num = 0;
    uint16      prev_char_count = 0;
    uint16      curr_char_count = 0;
    uint16      next_char_count = 0;
    EDIT_ITEM_STATE_E   pre_item_state = EDIT_ITEM_STATE_START;
    EDIT_ITEM_STATE_E   cur_item_state = EDIT_ITEM_STATE_START;
    EDIT_ITEM_STATE_E   next_item_state = EDIT_ITEM_STATE_START;
    wchar       separator_char = 0;
    wchar       cur_char = 0;
    int32       i = 0;

    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    cur_item_count = listedit_ctrl_ptr->item_num;
    max_len = listedit_ctrl_ptr->item_max_len;
    max_num = listedit_ctrl_ptr->max_num;

    GetPrevCharState (listedit_ctrl_ptr, separator_char, &pre_item_state, &prev_char_count);

    GetNextCharState (listedit_ctrl_ptr, separator_char, &next_item_state, &next_char_count);

    // get string or its length
    while (i < add_str_ptr->commit_len + add_str_ptr->underline_len)
    {
        // 获取当前字符的状态，只有可能是字符和分隔符
        cur_item_state = GetCurrCharState (
                             listedit_ctrl_ptr,
                             separator_char,
                             add_str_ptr,
                             i,
                             pre_item_state,
                             &cur_char);

        switch (cur_item_state)
        {
        case EDIT_ITEM_STATE_CHAR:
            // 超过最大长度的情况，退出
            if (prev_char_count + curr_char_count > max_len)
            {
                is_over = TRUE;
                break;
            }

            // 加回车符
            // 现在回车符在这里添加，代表新增了一个item
            if (EDIT_ITEM_STATE_SEPARATOR == pre_item_state)
            {
                // 如果超过总数，退出
                if (cur_item_count >= max_num)
                {
                    is_over = TRUE;
                    break;
                }

                // 添加回车，里面会自动加总数
                AddCommitStr (
                    listedit_ctrl_ptr,
                    actual_str_ptr, actual_char_count, LF_CHAR,
                    &cur_item_count, &actual_char_count);

                // 这是非用户数据，里面自动添加的数据
                actual_str_ptr->len_before_cursor++;
            }

            // 从0开始新增时，由于前面没有分隔符，所以这里特殊处理，总数变1
            if (0 == cur_item_count)
            {
                cur_item_count++;
            }

            // 添加此有效数据
            AddCommitStr (
                listedit_ctrl_ptr,
                actual_str_ptr, actual_char_count, cur_char,
                &cur_item_count, &actual_char_count);

            // 这个item已经添加的用户字符数
            curr_char_count++;

            if (i < add_str_ptr->commit_len)
            {
                commit_count++;
            }
            else
            {
                underline_count++;
            }

            // 此字已经变为上次的状态，再进行下一轮比较
            pre_item_state = EDIT_ITEM_STATE_CHAR;
            break;

        case EDIT_ITEM_STATE_SEPARATOR:
            // 连续的分隔符，跳过
            if (EDIT_ITEM_STATE_START == pre_item_state
                    || EDIT_ITEM_STATE_SEPARATOR == pre_item_state
                    || EDIT_ITEM_STATE_LF == pre_item_state)
            {
                break;
            }
            else
            {
                SCI_ASSERT (EDIT_ITEM_STATE_CHAR == pre_item_state); /*assert verified*/

                // 如果超过总数，退出
                if (cur_item_count >= max_num
                        && EDIT_ITEM_STATE_END != next_item_state)
                {
                    is_over = TRUE;
                    break;
                }

                // 添加分隔符，回车在EDIT_ITEM_STATE_CHAR状态时添加
                // 这里，用户给的回车或分号，都算作分隔符
                AddCommitStr (
                    listedit_ctrl_ptr,
                    actual_str_ptr, actual_char_count, separator_char,
                    &cur_item_count, &actual_char_count);

                // 代表这个item结束
                curr_char_count = 0;

                if (i < add_str_ptr->commit_len)
                {
                    commit_count++;
                }
                else
                {
                    underline_count++;
                }

                // 前面的有效字符数变为0
                prev_char_count = 0;
                pre_item_state = EDIT_ITEM_STATE_SEPARATOR;
            }

            break;

        case EDIT_ITEM_STATE_INVALID:
        default:
            is_over = TRUE;
            break;
        }

        if (is_over)
        {
            break;
        }

        i++;
    }

    // 用户数据过长的情况
    if (prev_char_count + curr_char_count + next_char_count > max_len)
    {
        // 说明用户数据只有一项内容，并且插在原item中间
        // 如果被插断的数据前后超过了最大长度，那此次插入是非法的，不允许
        if (prev_char_count + next_char_count >= max_len)
        {
            actual_char_count = 0;
        }
        else
        {
            // unwanted string length
            curr_char_count = (uint16) (curr_char_count + next_char_count + prev_char_count - max_len);

            // 去掉非法的数据，计算得到实际的字符数
            actual_char_count = (uint16) (actual_char_count - curr_char_count);

            if (underline_count >= curr_char_count)
            {
                underline_count = (uint16) (underline_count - curr_char_count);
            }
            else
            {
                commit_count = (uint16) (commit_count + underline_count - curr_char_count);
                underline_count = 0;
            }
        }
    }

    if (actual_char_count > 0)
    {
        // 由于这里是有有效数据的，所以，肯定经过了字符查询判断，即上边的while
        // 循环，所以，pre_item_state状态只能是分隔符，或者普通字符
        // 这里分别针对这两种情况，把后面不合法的情况全罗列出来，一一做出判断
        if (EDIT_ITEM_STATE_SEPARATOR == pre_item_state)
        {
            if (EDIT_ITEM_STATE_SEPARATOR == next_item_state)   // 两个分隔符不能同时出现，去掉一个
            {
                AddCommitStr (
                    listedit_ctrl_ptr,
                    actual_str_ptr, actual_char_count, 0,
                    &cur_item_count, &actual_char_count);
            }
            else if (EDIT_ITEM_STATE_CHAR == next_item_state    // 后面是字符的情况，加上回车
                || EDIT_ITEM_STATE_OBJECT == next_item_state)
            {
                AddCommitStr (
                    listedit_ctrl_ptr,
                    actual_str_ptr, actual_char_count, LF_CHAR,
                    &cur_item_count, &actual_char_count);
            }
        }
        else if (EDIT_ITEM_STATE_CHAR == pre_item_state)
        {
            if (EDIT_ITEM_STATE_END == next_item_state          // 添加一个分隔符
                || EDIT_ITEM_STATE_LF == next_item_state)
            {
                AddCommitStr (
                    listedit_ctrl_ptr,
                    actual_str_ptr, actual_char_count, separator_char,
                    &cur_item_count, &actual_char_count);
            }
            else if (EDIT_ITEM_STATE_OBJECT == next_item_state) // 如果是文本对象，则需要添加分隔符和回车
            {
                AddCommitStr (
                    listedit_ctrl_ptr,
                    actual_str_ptr, actual_char_count, separator_char,
                    &cur_item_count, &actual_char_count);

                AddCommitStr (
                    listedit_ctrl_ptr,
                    actual_str_ptr, actual_char_count, LF_CHAR,
                    &cur_item_count, &actual_char_count);
            }
        }
        else
        {
            SCI_ASSERT (0); /*assert verified*/
        }
    }

    if (PNULL != commit_count_ptr)
    {
        *commit_count_ptr = commit_count;
    }

    if (PNULL != underline_count_ptr)
    {
        *underline_count_ptr = underline_count;
    }

    if (PNULL != item_num_ptr)
    {
        *item_num_ptr = cur_item_count;
    }

    actual_str_ptr->str_len = actual_char_count;

    if (0 < actual_char_count)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get commit string when adding item object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCommitObject (
    CTRLLISTEDIT_OBJ_T       *listedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T        *add_str_ptr,       // in
    GUIEDIT_ADD_ACTUAL_STR_T *actual_str_ptr,    // out
    uint16                   *commit_count_ptr,  // out
    uint16                   *underline_count_ptr,   // out
    uint16                   *item_num_ptr       // out
)
{
    uint16      len_before = 0;
    uint16      len_after = 0;
    uint16      str_len = 0;
    uint16      max_num = 0;
    uint16      cur_item_count = 0;
    int32       i = 0;
    wchar       separator_char = 0;
    EDIT_ITEM_STATE_E   prev_item_state = 0;
    EDIT_ITEM_STATE_E   next_item_state = 0;

    if (0 == add_str_ptr->commit_len || PNULL == add_str_ptr->commit_ptr)
    {
        return FALSE;
    }

    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style);

    str_len = add_str_ptr->commit_len;
    cur_item_count = listedit_ctrl_ptr->item_num;
    max_num = listedit_ctrl_ptr->max_num;

    GetPrevCharState (listedit_ctrl_ptr, separator_char, &prev_item_state, PNULL);
    GetNextCharState (listedit_ctrl_ptr, separator_char, &next_item_state, PNULL);

    // calculate the beginning string
    switch (prev_item_state)
    {
    case EDIT_ITEM_STATE_START:
        len_before = 0;     // 前不需要加任何东西
        break;

    case EDIT_ITEM_STATE_SEPARATOR:
        len_before = 1;     // 前面需要加回车
        break;

    case EDIT_ITEM_STATE_CHAR:
        len_before = 2;     // 前面需要加分隔符和回车
        break;

    case EDIT_ITEM_STATE_LF:
        len_before = 0;     // 前不需要加任何东西
        break;

    default:
        SCI_ASSERT (0); /*assert verified*/
        break;
    }

    // 根据后一字符状态决定添加的字符
    switch (next_item_state)
    {
    case EDIT_ITEM_STATE_END:
        len_after = 1;     // 最后还要加分隔符
        break;

    case EDIT_ITEM_STATE_SEPARATOR:
        len_after = 0;     // 不需要加任何东西
        break;

    case EDIT_ITEM_STATE_CHAR:
    case EDIT_ITEM_STATE_OBJECT:
        actual_str_ptr->is_add_lf = TRUE;
        len_after = 2;     // 需要加分隔符和回车
        break;

    case EDIT_ITEM_STATE_LF:
        len_after = 1;     // 最后还要加分隔符
        break;

    default:
        SCI_ASSERT (0); /*assert verified*/
        break;
    }

    // 以是否新增回车来判断是否需要数量
    if (0 == cur_item_count || len_before >= 1 || len_after >= 2)
    {
        cur_item_count++;

        if (cur_item_count > max_num)
        {
            return FALSE;
        }
    }

    if (PNULL != actual_str_ptr->str_ptr)
    {
        // add separator and lf_char at the beginning
        if (1 == len_before)
        {
            actual_str_ptr->str_ptr[0] = LF_CHAR;
        }
        else if (2 == len_before)
        {
            actual_str_ptr->str_ptr[0] = separator_char;
            actual_str_ptr->str_ptr[1] = LF_CHAR;
        }

        // copy commit string
        MMI_MEMCPY (&actual_str_ptr->str_ptr[len_before], sizeof (wchar) *str_len,
                    add_str_ptr->commit_ptr, sizeof (wchar) *add_str_ptr->commit_len,
                    sizeof (wchar) *add_str_ptr->commit_len);

        // 如果给的数据中有回车的，那将它转换成空格
        for (i = len_before; i < str_len + len_before; i++)
        {
            if (LF_CHAR == actual_str_ptr->str_ptr[i])
            {
                actual_str_ptr->str_ptr[i] = ' ';
            }
        }

        // add separator in the end
        if (1 == len_after)
        {
            actual_str_ptr->str_ptr[str_len + len_before] = separator_char;
        }
        else if (2 == len_after)
        {
            actual_str_ptr->str_ptr[str_len + len_before] = separator_char;
            actual_str_ptr->str_ptr[str_len + len_before + 1] = LF_CHAR;
        }
    }

    actual_str_ptr->len_before_cursor = len_before;
    actual_str_ptr->str_len = (add_str_ptr->commit_len + len_before + len_after);

    if (PNULL != item_num_ptr)
    {
        *item_num_ptr = cur_item_count;
    }

    if (commit_count_ptr)
    {
        *commit_count_ptr = add_str_ptr->commit_len;                // commit数量为全部
    }

    GUI_INVALID_PARAM (underline_count_ptr);/*lint !e522*/

    return TRUE;
}

/*****************************************************************************/
//  Description : add a list item object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr, // [in]
    uint16              start,          // [in]
    uint16              len,            // [in]
    MMI_STRING_T        *user_data_ptr  // [in]
)
{
    GUIEDIT_OBJ_INFO_T  *obj_info_ptr = PNULL;

    if (PNULL == listedit_ctrl_ptr || PNULL == user_data_ptr)
    {
        return FALSE;
    }

    obj_info_ptr = SCI_ALLOC_APP (sizeof (GUIEDIT_OBJ_INFO_T));
    SCI_MEMSET (obj_info_ptr, 0, sizeof (GUIEDIT_OBJ_INFO_T));

    obj_info_ptr->start = start;
    obj_info_ptr->len = len;

    // 添加对应的用户数据
    obj_info_ptr->user_data.wstr_len = user_data_ptr->wstr_len;
    obj_info_ptr->user_data.wstr_ptr = SCI_ALLOC_APP (user_data_ptr->wstr_len * sizeof (wchar));
    MMI_MEMCPY (
        obj_info_ptr->user_data.wstr_ptr,
        obj_info_ptr->user_data.wstr_len*sizeof (wchar),
        user_data_ptr->wstr_ptr,
        user_data_ptr->wstr_len*sizeof (wchar),
        user_data_ptr->wstr_len*sizeof (wchar));

    if (PNULL != listedit_ctrl_ptr->obj_info_ptr)
    {
        obj_info_ptr->next = listedit_ctrl_ptr->obj_info_ptr;
    }

    listedit_ctrl_ptr->obj_info_ptr = obj_info_ptr;

    // 检查合法性
    CheckItemObject (listedit_ctrl_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : remove list item object between two pos
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,  // [in]
    uint16              start_pos,       // [in]
    uint16              end_pos          // [in]
)
{
    GUIEDIT_OBJ_INFO_T *prev_obj_info_ptr = PNULL;
    GUIEDIT_OBJ_INFO_T *curr_obj_info_ptr = PNULL;

    if (PNULL == listedit_ctrl_ptr )
    {
        return FALSE;
    }

    prev_obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;
    curr_obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;

    while (PNULL != curr_obj_info_ptr)
    {
        // 如果OBJECT位置在给定区间之外的，不需要删除
        if (curr_obj_info_ptr->start >= end_pos
            || curr_obj_info_ptr->start + curr_obj_info_ptr->len <= start_pos)
        {
            if (listedit_ctrl_ptr->obj_info_ptr != curr_obj_info_ptr)
            {
                prev_obj_info_ptr = curr_obj_info_ptr;
            }

            curr_obj_info_ptr = curr_obj_info_ptr->next;
        }
        else
        {
            // 删除此对象
            if (listedit_ctrl_ptr->obj_info_ptr == curr_obj_info_ptr)
            {
                SCI_ASSERT (listedit_ctrl_ptr->obj_info_ptr == prev_obj_info_ptr); /*assert verified*/

                listedit_ctrl_ptr->obj_info_ptr = curr_obj_info_ptr->next;
                prev_obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;

                DelItemObject (curr_obj_info_ptr);

                curr_obj_info_ptr = prev_obj_info_ptr;
            }
            else
            {
                prev_obj_info_ptr->next = curr_obj_info_ptr->next;
                DelItemObject (curr_obj_info_ptr);
                curr_obj_info_ptr = prev_obj_info_ptr->next;
            }
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : adjust list item object position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr, // [in]
    uint16              adjust_start,   // [in]
    int16               adjust_len      // [in]
)
{
    GUIEDIT_OBJ_INFO_T *obj_info_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    if (PNULL == listedit_ctrl_ptr || adjust_start > baseedit_ctrl_ptr->str_len)
    {
        return FALSE;
    }

    obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;

    // 调整给定起始位置之后的OBJECT信息
    while (PNULL != obj_info_ptr)
    {
        if (obj_info_ptr->start >= adjust_start)
        {
            //kevin.lou modified:delete assert
            //SCI_ASSERT(adjust_len >= 0 || (obj_info_ptr->start >= (uint16)(-adjust_len))); /*assert verified*/
            if (adjust_len >= 0 || (obj_info_ptr->start >= (uint16) (-adjust_len)))
            {
                obj_info_ptr->start = (uint16) (obj_info_ptr->start + adjust_len);
            }
        }

        obj_info_ptr = obj_info_ptr->next;
    }

    // 检查合法性
    CheckItemObject (listedit_ctrl_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : find list item object info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_OBJ_INFO_T* FindListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,    // [in]
    uint16              pos                // [in]
)
{
    GUIEDIT_OBJ_INFO_T *obj_info_ptr = PNULL;

    if (PNULL == listedit_ctrl_ptr)
    {
        return PNULL;
    }

    obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;

    while (PNULL != obj_info_ptr)
    {
        if (pos >= obj_info_ptr->start
            && pos < obj_info_ptr->start + obj_info_ptr->len)
        {
            break;
        }

        obj_info_ptr = obj_info_ptr->next;
    }

    return obj_info_ptr;
}

/*****************************************************************************/
//  Description : destroy list item info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DestroyListItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr  // [in]
)
{
    GUIEDIT_OBJ_INFO_T *obj_info_ptr = PNULL;

    if (PNULL != listedit_ctrl_ptr)
    {
        obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;

        while (PNULL != obj_info_ptr)
        {
            listedit_ctrl_ptr->obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr->next;

            DelItemObject (obj_info_ptr);

            obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;
        }
    }
}

/*****************************************************************************/
//  Description : check item info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//    check the correction of the item object info
/*****************************************************************************/
LOCAL BOOLEAN CheckItemObject (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr  // [in]
)
{
    BOOLEAN is_ok = TRUE;
    uint16  start_pos = 0;
    uint16  end_pos = 0;
    GUIEDIT_OBJ_INFO_T *curr_obj_info_ptr = PNULL;
    GUIEDIT_OBJ_INFO_T *next_obj_info_ptr = PNULL;

    curr_obj_info_ptr = listedit_ctrl_ptr->obj_info_ptr;

    while (PNULL != curr_obj_info_ptr)
    {
        start_pos = curr_obj_info_ptr->start;
        end_pos   = (uint16) (curr_obj_info_ptr->start + curr_obj_info_ptr->len);

        next_obj_info_ptr = curr_obj_info_ptr->next;

        while (PNULL != next_obj_info_ptr)
        {
            // 每个文本对象的位置不可能重叠
            if ((next_obj_info_ptr->start >= start_pos && next_obj_info_ptr->start <= end_pos)
                || (next_obj_info_ptr->start + next_obj_info_ptr->len >= start_pos
                    && next_obj_info_ptr->start + next_obj_info_ptr->len <= end_pos))
            {
                is_ok = FALSE;
                SCI_ASSERT (0); /*assert verified*/
            }

            next_obj_info_ptr = next_obj_info_ptr->next;
        }

        curr_obj_info_ptr = curr_obj_info_ptr->next;
    }

    return is_ok;
}

/*****************************************************************************/
//  Description : remove item info
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DelItemObject (
    GUIEDIT_OBJ_INFO_T *obj_info_ptr    // [in]
)
{
    if (PNULL != obj_info_ptr)
    {
        if (PNULL != obj_info_ptr->user_data.wstr_ptr)
        {
            SCI_FREE (obj_info_ptr->user_data.wstr_ptr);
        }

        SCI_FREE (obj_info_ptr);
    }
}

/*****************************************************************************/
//  Description : is list char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsListChar (
    CTRLLISTEDIT_OBJ_T  *listedit_ctrl_ptr,
    wchar               char_code       //in:
)
{
    BOOLEAN     result = FALSE;

    switch (listedit_ctrl_ptr->theme.list_style)
    {
    case GUIEDIT_LIST_STYLE_PHONENUM:
    case GUIEDIT_LIST_STYLE_EMAIL:		// now, email and other style use this case
    case GUIEDIT_LIST_STYLE_ALL:

        //only digital
// TODO: need a new method to set
//#ifdef MMI_PDA_SUPPORT
        if (char_code == BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) listedit_ctrl_ptr->theme.separator_style)
            || char_code == LF_CHAR)
        {
            result = FALSE;
        }
        else
        {
            result = TRUE;
        }

//#else
//        result = BASEFLEX_IsDigitalChar(char_code);
//#endif
        break;

//    case GUIEDIT_LIST_STYLE_EMAIL:
//    case GUIEDIT_LIST_STYLE_ALL:
//        result = BASEFLEX_CheckTextStr (baseflex_ctrl_ptr, &char_code, 1);
//        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get edit list item is valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidItem (
    wchar                 *str_ptr,
    uint16                str_len,
    GUIEDIT_LIST_STYLE_E  item_style
)
{
    BOOLEAN     result = TRUE;
    uint16      len = 0;

    switch (item_style)
    {
    case GUIEDIT_LIST_STYLE_PHONENUM:
    case GUIEDIT_LIST_STYLE_ALL:
        len = BASEFLEX_InterceptPhoneNum (str_ptr, str_len);

        if (len != str_len)
        {
            result = FALSE;
        }

        break;

    case GUIEDIT_LIST_STYLE_EMAIL:
        break;

    default:
        //SCI_TRACE_LOW:"IsValidItem:item_style%d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_4792_112_2_18_3_15_43_124, (uint8*) "d", item_style);
        result = FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 给输入法发同步消息
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 只有t9时有效
/*****************************************************************************/
LOCAL void SendImSyncMsg(
    CTRLLISTEDIT_OBJ_T      *listedit_ctrl_ptr
    )
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)listedit_ctrl_ptr;

    switch(listedit_ctrl_ptr->im_sync_type)
    {
    case CTRLLISTEDIT_IMSYNC_ADD:
        MMK_SendMsg(baseedit_ctrl_ptr->im_ctrl_handle, MSG_NOTIFY_EDITBOX_IMSYNC_ADD, 0);
        break;

    case CTRLLISTEDIT_IMSYNC_DEL:
        MMK_SendMsg(baseedit_ctrl_ptr->im_ctrl_handle, MSG_NOTIFY_EDITBOX_IMSYNC_DEL, 0);
        break;

    default:
        break;
    }

    listedit_ctrl_ptr->im_sync_type = CTRLLISTEDIT_IMSYNC_NONE;
}


