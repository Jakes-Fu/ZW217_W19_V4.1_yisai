/*****************************************************************************
** File Name:      ctrldropdownlist.c                                        *
** Author:                                                                   *
** Date:           25/07/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe button                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 25/07/2012     xiaoqing         Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_app.h"
#include "ctrldropdownlist_export.h"
#include "mmk_msg_internal.h"
#include "mmk_window_internal.h"
#include "guires.h"
#include "guistring.h"
#include "mmitheme_pos.h"
#include "mmitheme_softkey.h"
#include "guiform.h"
#include "guictrl_api.h"
#include "cafctrldropdownlist.h"
#include "ctrlbase.h"
#include "ctrldropdownlist.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*/
/**                         Compiler Flag                                    */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define     GUIDDL_ITEM_INFO_E    0
#define     GUIDDL_DETAIL_INFO_E  1

/**--------------------------------------------------------------------------*/
/**                         TYPE AND STRUCT                                  */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         Local Definition                                 */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init list class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistCtrlInitVtbl(
                                        CTRLDROPDOWNLIST_VTBL_T     *list_vtbl_ptr
                                        );

/*****************************************************************************/
//  Description : pack dropdownlist init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DropDownListPackInitParam(
    GUIDROPDOWNLIST_INIT_DATA_T *list_init_ptr,
    CTRLDROPDOWNLIST_INIT_DATA_T *dropdownlist_param_ptr
    );

/*****************************************************************************/
//  Description : process MSG_TP_PRESS_UP
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownlistOnPenUp(
                                       CTRLBASE_OBJ_T *obj_ptr,//dropdownlist ptr
                                       GUI_POINT_T point,//position
									   DPARAM param
                                       );

/*****************************************************************************/
//  Description : process MSG_TP_PRESS_DOWN
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownlistOnPenDown(
                                         CTRLBASE_OBJ_T *obj_ptr,//dropdownlist ptr
                                         GUI_POINT_T point,//position
                                         DPARAM param
                                         );

/*****************************************************************************/
//  Description : process MSG_TP_PRESS_MOVE
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownlistOnPenMove(
                                         CTRLBASE_OBJ_T *obj_ptr,//dropdownlist ptr
                                         GUI_POINT_T point,//position
                                         DPARAM param
                                         );

/*****************************************************************************/
//Description : draw the Rect ,it include text_rect  button and list
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:
/*****************************************************************************/
LOCAL void DrawDropDownList(
                            CTRLBASE_OBJ_T *obj_ptr    
                            );

/*****************************************************************************/
//Description : get pointed rect
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDropdownlistRect(
                                     CTRLBASE_OBJ_T *obj_ptr ,//text rect ,it is for calculate other rect
                                     GUIDROPDOWNLIST_POSITION_E pos// pointered position
                                     );

/*****************************************************************************/
//Description : draw the button
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:
/*****************************************************************************/
LOCAL void DrawDropDownlistButton(
                                  CTRLBASE_OBJ_T *obj_ptr ,
                                  GUI_RECT_T rect,//button rect
                                  GUIDROPDOWNLIST_BUTTON_STATUS_E button_status,// pressed or released
                                  MMI_WIN_ID_T win_id
                                  );

/*****************************************************************************/
//  Description : create dynamic List
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateListBox( 
                         CTRLBASE_OBJ_T *obj_ptr,
                         BOOLEAN is_set_focus,
                         BOOLEAN is_must_create
                         );

/*****************************************************************************/
//  Description : Get DropDownList pointer from control id
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL CTRLDROPDOWNLIST_OBJ_T* GetDropDownListPtr(
                                                  MMI_HANDLE_T ctrl_handle
                                                  );

/*****************************************************************************/
//  Description : dropdownlist type of
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN DropDownListTypeOf(
                                 CTRLBASE_OBJ_T* ctrl_ptr
                                 );

/*****************************************************************************/
//Description : get pen position
//Global resource dependence : 
//Author:xiaoqing.lu
//      Note:
/*****************************************************************************/ 
LOCAL void LoadDataToList(
                          CTRLBASE_OBJ_T *obj_ptr 
                          );

/*****************************************************************************/
//Description : get pen position
//Global resource dependence : 
//Author:xiaoqing.lu
//      Note:
/*****************************************************************************/ 
LOCAL GUIDROPDOWNLIST_POSITION_E GetPenPosition(
                                                 CTRLBASE_OBJ_T *obj_ptr ,//active rect
                                                 GUI_POINT_T point
                                                 );

/*****************************************************************************/
//  Description : process lost active
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollUpList(
                                CTRLBASE_OBJ_T *obj_ptr//ctrl ptr
                                );

/*****************************************************************************/
// 	Description : drop down the list
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollDownList(
                                  CTRLBASE_OBJ_T *obj_ptr,  //ctrl ptr
                                  BOOLEAN is_set_focus,
                                  BOOLEAN is_must_create
                                  );

/*****************************************************************************/
// 	Description : drop down the list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DDLIsSetListFocus(
                                CTRLBASE_OBJ_T *obj_ptr
                                );
/*****************************************************************************/
// 	Description : drop down the list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DDLIsEditType(
                            CTRLBASE_OBJ_T *obj_ptr
                            );

/*****************************************************************************/
// 	Description : select one from drop down list
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SelectFromList(
                                  CTRLBASE_OBJ_T *obj_ptr
                                  );

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropDownListConstruct(
                                    CTRLBASE_OBJ_T          *obj_ptr,
                                    CTRLDROPDOWNLIST_INIT_DATA_T   *list_param_ptr
                                    );

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropDownListDestruct(
                                   CTRLBASE_OBJ_T *obj_ptr
                                   );

/*****************************************************************************/
//  Description : handle msg of DropDownList
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownListHandleMsg(
                                         CTRLBASE_OBJ_T *obj_ptr, //控件的指针
                                         MMI_MESSAGE_ID_E msg_id,     //事件类型
                                         DPARAM           param     //附带的处理参数
                                         );

/*****************************************************************************/
//  Description : init DropDownList
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DropDownListInit(
                            CTRLBASE_OBJ_T          *obj_ptr,
                            CTRLDROPDOWNLIST_INIT_DATA_T   *list_param_ptr
                            );

/*****************************************************************************/
//  Description : 计算下拉框，下拉时的区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalListRect(
                             CTRLBASE_OBJ_T *obj_ptr 
                             );

/*****************************************************************************/
//  Description : 计算下拉框，下拉时的区域和原始区域的合并区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetWholeRect(
                        CTRLBASE_OBJ_T *obj_ptr,   // [in]
                        GUI_RECT_T *whole_rect_ptr,                 // [out] whole rect
                        GUI_RECT_T *whole_display_rect_ptr          // [out] that not include invisible rect
                        );

/*****************************************************************************/
// 	Description : set control display rect
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDisplayRect(
                             CTRLBASE_OBJ_T *obj_ptr,  //in:
                             const GUI_RECT_T *rect_ptr,      //in:
                             BOOLEAN          is_update       //in:
                             );

/*****************************************************************************/
// 	Description : set control boder info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistSetBorder(
                        CTRLBASE_OBJ_T *obj_ptr,
                        GUI_BORDER_T  *border_ptr
                        );

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence : 
//  Author: 
//	Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistSetBackGround(
                            CTRLBASE_OBJ_T *obj_ptr, 
                            GUI_BG_T *bg_ptr
                            );

/*****************************************************************************/
// 	Description : set control font info4
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDropdownlistFont(
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  GUI_FONT_ALL_T  *font_ptr
                                  );

/*****************************************************************************/
// 	Description : get control font info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDropdownlistFont(
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  GUI_FONT_ALL_T  *font_ptr
                                  );

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDropdownlistHeight(
                                    CTRLBASE_OBJ_T *obj_ptr,  //in:
                                    uint16         width,          //in:
                                    uint16         *height_ptr     //in/out:
                                    );

/*****************************************************************************/
// 	Description : set gui control rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistSetRect(
                                  CTRLBASE_OBJ_T *obj_ptr, 
                                  const GUI_RECT_T *rect_ptr
                                  );

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownDestroyAllItemData(
                                 CTRLBASE_OBJ_T *obj_ptr
                                 );

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownDestroyItemData(MMI_STRING_T* item_info_ptr);

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_STRING_T* DropdownGetItemInfoPtr(
                                           const CTRLBASE_OBJ_T *obj_ptr,
                                           uint16 index  //from 0 to total_item_num-1
                                           );

/*****************************************************************************/
//  Description : get the appointed item detail pointer
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_STRING_T* DropdownGetItemDetailInfoPtr(
                                                 const CTRLBASE_OBJ_T *obj_ptr,
                                                 uint16 index  //from 0 to total_item_num-1
                                                 );

/*****************************************************************************/
//  Description : alloc all items memory of the list control
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownAllocAllItemsSpace(
                          CTRLBASE_OBJ_T *obj_ptr  // the list control pointer
                          );

/*****************************************************************************/
//  Description : reset the item pointer of list 
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownResetListItem(
                         CTRLBASE_OBJ_T *obj_ptr  // the list control pointer
                         );

/*****************************************************************************/
//  Description : 添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN DropdownAppendItemByPtr( 
                                      CTRLBASE_OBJ_T *obj_ptr, // control id
                                      const MMI_STRING_T *item_ptr //item info
                                      );

/*****************************************************************************/
//  Description : 传入text id ,添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN DropdownAddItemDetailByPtr( 
                                         CTRLBASE_OBJ_T *obj_ptr, // control id
                                         uint16 item_index,
                                         const MMI_STRING_T *item_ptr //item info
                                         );

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownInsertItemByPtr(
                                      CTRLBASE_OBJ_T *obj_ptr,
                                      const MMI_STRING_T* item_ptr,
                                      uint32 item_type,
                                      uint16 pos
                                      );

/*****************************************************************************/
//  Description : 根据list控件信息设置list最大值
//  Global resource dependence : 
//  Author: jin.wang
//  Note:设置成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN DropdownSetMaxItemByPtr(
                                      CTRLBASE_OBJ_T *obj_ptr, // control id
                                      uint16 max_item_num
                                      );

/*****************************************************************************/
//  Description : send notify
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void NotifyResize(
                        CTRLBASE_OBJ_T *obj_ptr
                        );

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
LOCAL MMI_STRING_T GetCurItemText( //return selected check item number
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  uint16 item_index
                                  );

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
LOCAL MMI_STRING_T GetCurItemDetailText( //return selected check item number
                                        CTRLBASE_OBJ_T *obj_ptr,
                                        uint16 item_index
                                        );

/***************************************************************************//*!
@brief 设置edit 字符串
@author xiaoqing.lu
@param list_ctrl_ptr [in] 控件ID
@return void
@note 
*******************************************************************************/
LOCAL void SetTextToEditCtrl(
                             CTRLBASE_OBJ_T *obj_ptr
                             );

/*****************************************************************************/
//  Description : get edit cursor rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownGetCursorRect(
                                    CTRLBASE_OBJ_T *obj_ptr,  //in:
                                    GUI_RECT_T      *rect_ptr   //in/out:
                                    );

/*****************************************************************************/
//  Description : get edit cursor index,is in the first or last line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownGetCursorIndex(
                                     CTRLBASE_OBJ_T *obj_ptr,      //in:
                                     BOOLEAN        *is_first_ptr,  //in/out:
                                     BOOLEAN        *is_last_ptr    //in/out:
                                     );

/*****************************************************************************/
//  Description : set edit cursor index,is in the first or last line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownSetCursorIndex(
                                     CTRLBASE_OBJ_T *obj_ptr,   //in:
                                     BOOLEAN        is_first,   //in:
                                     BOOLEAN        is_last     //in:
                                     );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get dropdownlistbox type
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DROPDOWNLIST_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BASE_TYPE,
            "dropdownlist",
            sizeof(CTRLDROPDOWNLIST_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)DropDownListConstruct,
            (OBJECT_DESTRUCTOR_FUNC)DropDownListDestruct,
            sizeof(CTRLDROPDOWNLIST_INIT_DATA_T),
            (PARAM_PACKING_FUNC)DropDownListPackInitParam,
            sizeof(CTRLDROPDOWNLIST_VTBL_T),
            (VTBL_INIT_FUNC)DropdownlistCtrlInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init list class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistCtrlInitVtbl(
                                        CTRLDROPDOWNLIST_VTBL_T *list_vtbl_ptr
                                        )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr =  (CTRLBASE_VTBL_T*)list_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent      = DropDownListHandleMsg;
    base_ctrl_vtbl_ptr->SetRect          = DropdownlistSetRect;
    base_ctrl_vtbl_ptr->SetBorder        = DropdownlistSetBorder;
    base_ctrl_vtbl_ptr->SetBg            = DropdownlistSetBackGround;
    base_ctrl_vtbl_ptr->SetFont          = SetDropdownlistFont;
    base_ctrl_vtbl_ptr->GetFont          = GetDropdownlistFont;
    base_ctrl_vtbl_ptr->GetHeightByWidth = GetDropdownlistHeight;
    base_ctrl_vtbl_ptr->SetDisplayRect   = SetDisplayRect;

    base_ctrl_vtbl_ptr->GetSelectRect    = DropdownGetCursorRect;
    base_ctrl_vtbl_ptr->GetSelectIndex   = DropdownGetCursorIndex;
    base_ctrl_vtbl_ptr->SetSelectIndex   = DropdownSetCursorIndex;

    return TRUE;
}

/*****************************************************************************/
//  Description : pack dropdownlist init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DropDownListPackInitParam(
    GUIDROPDOWNLIST_INIT_DATA_T *list_init_ptr,
    CTRLDROPDOWNLIST_INIT_DATA_T *dropdownlist_param_ptr
    )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)dropdownlist_param_ptr;

    base_ctrl_param_ptr->both_rect = list_init_ptr->both_rect;

    dropdownlist_param_ptr->page_item_num = list_init_ptr->page_item_num;
    dropdownlist_param_ptr->show_type = list_init_ptr->show_type;
}

/*****************************************************************************/
//  Description : init DropDownList
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DropDownListInit(
                            CTRLBASE_OBJ_T          *obj_ptr,
                            CTRLDROPDOWNLIST_INIT_DATA_T   *list_param_ptr
                            )
{
    CTRLDROPDOWNLIST_OBJ_T  *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)obj_ptr;
    MMI_DROPDOWNLIST_STYLE_T   dropdownlist_style = {0}; //display style
    uint16		image_height = 0;
    
    if(PNULL == list_param_ptr || PNULL == list_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DropDownListInit PNULL == init_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_537_112_2_18_3_15_27_89,(uint8*)"");
        return;
    }
    
    list_ctrl_ptr->orig_rect = list_param_ptr->base_ctrl_param.both_rect.v_rect;

    obj_ptr->is_active = FALSE;
    list_ctrl_ptr->is_focus = FALSE;
    list_ctrl_ptr->rect_style = GUIDNLIST_RECT_PLANE;

    // 默认的类型
    list_ctrl_ptr->ctrl_type_info.ctrl_type = GUIDNLIST_NORMAL;

    if (list_param_ptr->show_type >= GUIDNLIST_SHOW_MAX)
    {
        list_param_ptr->show_type = GUIDNLIST_SHOW_AUTOSET;
    }

    list_ctrl_ptr->show_type = list_param_ptr->show_type;
    list_ctrl_ptr->button_status = GUIDNLIST_BUTTON_RELEASED;
    list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;
    list_ctrl_ptr->is_ellipsis = FALSE;
    list_ctrl_ptr->current_index = 0xFFFF;
    list_ctrl_ptr->list_handle = 0;
    list_ctrl_ptr->list_state = GUILIST_STATE_NEED_TITLE | GUILIST_STATE_NEED_SOFTKEY;
    list_ctrl_ptr->all_item_ptr = PNULL;
    list_ctrl_ptr->page_item_num = list_param_ptr->page_item_num;
    list_ctrl_ptr->is_auto_size = TRUE;//自动设置大小

    list_ctrl_ptr->win_id = MMK_GetWinHandleByCtrl( base_ctrl_ptr->handle );

    //获得系统默认的风格，主要包括字体、高亮条颜色的设置
    if(MMITHEME_GetDropDownListStyle(&list_ctrl_ptr->style_info))
    {
        GUIRES_GetImgWidthHeight(PNULL,&image_height,dropdownlist_style.unpressed_arrow_icon,list_ctrl_ptr->win_id);

        if(list_ctrl_ptr->orig_rect.bottom - list_ctrl_ptr->orig_rect.top + 1 < image_height)
        {
            list_ctrl_ptr->orig_rect.bottom = (int16)(list_ctrl_ptr->orig_rect.top+image_height-1);
        }
    }

    // 如果为0，则使用默认值，自适应的时候还要重新计算
    if (0 == list_param_ptr->page_item_num)
    {
        list_ctrl_ptr->page_item_num = list_ctrl_ptr->style_info.default_item_num;
    }

    list_ctrl_ptr->cur_page_item_num = list_ctrl_ptr->page_item_num;
    
    list_ctrl_ptr->is_mid_softkey_open_list = TRUE;
    list_ctrl_ptr->is_web_key_open_list = TRUE;

    return;
}

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropDownListConstruct(
                                    CTRLBASE_OBJ_T          *obj_ptr,
                                    CTRLDROPDOWNLIST_INIT_DATA_T   *list_param_ptr
                                    )
{
    BOOLEAN result = TRUE;
    CTRLDROPDOWNLIST_OBJ_T  *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if( PNULL != list_ctrl_ptr && PNULL != list_param_ptr )
    {
        DropDownListInit( list_ctrl_ptr, list_param_ptr);
    }
    else
    {
        result = FALSE;
        //SCI_TRACE_LOW:"DropDownListConstruct PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_613_112_2_18_3_15_28_90,(uint8*)"");
    }
    
    return result;
}

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropDownListDestruct(
                                   CTRLBASE_OBJ_T *obj_ptr
                                   )
{
    BOOLEAN result = TRUE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if (PNULL != obj_ptr)
    {
        if(PNULL != list_ctrl_ptr->all_item_ptr)
        {
            DropdownDestroyAllItemData(list_ctrl_ptr);

            if (PNULL != list_ctrl_ptr->all_item_ptr)
            {
                SCI_FREE(list_ctrl_ptr->all_item_ptr);
            }

            if (PNULL != list_ctrl_ptr->all_item_detail_ptr)
            {
                SCI_FREE(list_ctrl_ptr->all_item_detail_ptr);
            }

            if (PNULL != list_ctrl_ptr->def_arry_ptr)
            {
                SCI_FREE(list_ctrl_ptr->def_arry_ptr);
            }
        }
        
        if(PNULL != list_ctrl_ptr->list_handle) 
        {
            // 这里可以直接赋值PNULL，因为此时guilist已经被释放掉
            list_ctrl_ptr->list_handle = 0;
        }
        list_ctrl_ptr->edit_handle = 0;
    }
    else
    {
        result = FALSE;
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_662_112_2_18_3_15_28_91,(uint8*)"");
    }
    return result;
}

/*****************************************************************************/
// 	Description : set control display rect
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDisplayRect(
                             CTRLBASE_OBJ_T *obj_ptr,  //in:
                             const GUI_RECT_T *rect_ptr,      //in:
                             BOOLEAN          is_update       //in:
                             )
{
    BOOLEAN     result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL != obj_ptr && DropDownListTypeOf(obj_ptr) && PNULL != rect_ptr)
    {
        obj_ptr->display_rect.left = rect_ptr->left;
        obj_ptr->display_rect.top = rect_ptr->top;
        obj_ptr->display_rect.right = rect_ptr->right;
        obj_ptr->display_rect.bottom = rect_ptr->bottom;

        result = TRUE;

        if (is_update)
        {
            DrawDropDownList(list_ctrl_ptr);
        }

        // 区域改变之后要关闭列表项
        if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
        {
            //get selected item
            if (PNULL != list_ctrl_ptr->list_handle)
            {
                list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;
        
                MMK_DestroyControl( list_ctrl_ptr->list_handle );
        
                list_ctrl_ptr->list_handle = PNULL;
            }
        }

        if (0 != list_ctrl_ptr->edit_handle && MMK_IsHandleValid(list_ctrl_ptr->edit_handle))
        {
            IGUICTRL_SetDisplayRect(MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), rect_ptr, is_update);
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control boder info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistSetBorder(
                                    CTRLBASE_OBJ_T *obj_ptr,
                                    GUI_BORDER_T  *border_ptr
                                    )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL != border_ptr && PNULL != list_ctrl_ptr)
    {
        result = TRUE;
        list_ctrl_ptr->style_info.border.color = border_ptr->color;
        list_ctrl_ptr->style_info.border.type = border_ptr->type;
        list_ctrl_ptr->style_info.border.width = border_ptr->width;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence : 
//  Author: 
//	Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistSetBackGround(
                                        CTRLBASE_OBJ_T *obj_ptr, 
                                        GUI_BG_T *bg_ptr
                                        )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL != bg_ptr && PNULL != list_ctrl_ptr)
    {
        result = TRUE;
        
        list_ctrl_ptr->style_info.bg_info = *bg_ptr;
    }

    return result;
}

/*****************************************************************************/
// 	Description : set control font info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDropdownlistFont(
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  GUI_FONT_ALL_T  *font_ptr
                                  )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL != font_ptr && PNULL != list_ctrl_ptr)
    {
        result = TRUE;
        list_ctrl_ptr->style_info.font_color = font_ptr->color;
        list_ctrl_ptr->style_info.font = font_ptr->font;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get control font info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDropdownlistFont(
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  GUI_FONT_ALL_T  *font_ptr
                                  )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL != font_ptr && PNULL != list_ctrl_ptr)
    {
        result = TRUE;

        font_ptr->color = list_ctrl_ptr->style_info.font_color;
        font_ptr->font  = list_ctrl_ptr->style_info.font;
    }

    return result;
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDropdownlistHeight(
                                    CTRLBASE_OBJ_T *obj_ptr,  //in:
                                    uint16         width,           //in:
                                    uint16         *height_ptr     //in/out:
                                    )
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_border = TRUE;
    uint16              font_height = 0;
    uint16              line_height = 0;
    uint16              image_height = 0;
    uint16              edit_width = 0;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    //label is exist border
    if (GUI_BORDER_NONE == list_ctrl_ptr->style_info.border.type)
    {
        is_border = FALSE;
    }

    //get font height
    font_height = GUI_GetFontHeight(list_ctrl_ptr->style_info.font, UNICODE_HANZI);

    if (0 != list_ctrl_ptr->edit_handle)
    {
        edit_width = width;
        if (is_border)
        {
            edit_width -= list_ctrl_ptr->style_info.border.width*2;
        }
        IGUICTRL_GetHeightByWidth(MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), edit_width, &font_height);
    }
    
    //add border
    if (is_border)
    {
        line_height = (uint16)(font_height + (list_ctrl_ptr->style_info.border.width*2) + DP2PX_VALUE(4));
    }
    
    GUIRES_GetImgWidthHeight(PNULL, &image_height,list_ctrl_ptr->style_info.unpressed_arrow_icon,list_ctrl_ptr->win_id);

    *height_ptr = MAX(line_height, image_height + (list_ctrl_ptr->style_info.border.width*2));

    GUI_INVALID_PARAM(width);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// 	Description : set gui control rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownlistSetRect(
                                  CTRLBASE_OBJ_T *obj_ptr, 
                                  const GUI_RECT_T *rect_ptr
                                  )
{
    BOOLEAN result = FALSE;
    GUI_RECT_T rect = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if ((PNULL != rect_ptr) && 
        (PNULL != list_ctrl_ptr) &&
        (!GUI_EqualRect(obj_ptr->rect,*rect_ptr)))
    {
        // 记录之前的区域
        rect = obj_ptr->rect;
        
        obj_ptr->rect         = *rect_ptr;
        obj_ptr->display_rect = *rect_ptr;
        list_ctrl_ptr->orig_rect    = *rect_ptr;

        GUI_SetVOrHRect( obj_ptr->handle, (GUI_RECT_T *)rect_ptr,&(obj_ptr->both_rect));
        
        // 区域改变之后要关闭列表项
        if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status
            && !GUI_EqualRect(obj_ptr->rect, rect))
        {
            //get selected item
            if (PNULL != list_ctrl_ptr->list_handle)
            {
                list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;
        
                MMK_DestroyControl( list_ctrl_ptr->list_handle);
        
                list_ctrl_ptr->list_handle = 0;

                //resize 
                obj_ptr->rect = list_ctrl_ptr->orig_rect;
            }   
        }

        if (0 != list_ctrl_ptr->edit_handle && MMK_IsHandleValid(list_ctrl_ptr->edit_handle))
        {
            GUI_RECT_T text_rect = {0};

            text_rect = GetDropdownlistRect(obj_ptr,GUIDNLIST_TEXT_RECT);

            if (GUI_IsInvalidRectEx(text_rect))
            {
                SCI_MEMSET(&text_rect, 0, sizeof(GUI_RECT_T));
            }

            IGUICTRL_SetRect(MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), &text_rect);
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle msg of DropDownList
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownListHandleMsg(
                                         CTRLBASE_OBJ_T *obj_ptr, //控件的指针
                                         MMI_MESSAGE_ID_E msg_id,     //事件类型
                                         DPARAM           param     //附带的处理参数
                                         )
{
    MMI_RESULT_E            recode              = MMI_RESULT_TRUE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if (PNULL == list_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DropDownListHandleMsg PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_984_112_2_18_3_15_28_92,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    switch(msg_id)
    {
    case MSG_CTL_OPEN:
        list_ctrl_ptr->is_focus = TRUE;
        break;

    case MSG_CTL_GET_ACTIVE:
	case MSG_NOTIFY_GET_ACTIVE:
        obj_ptr->is_active = TRUE;
        GUICTRL_SendNotify(obj_ptr->handle,MSG_NOTIFY_GET_ACTIVE);
        if (0 != list_ctrl_ptr->edit_handle)
        {
            MMK_SetAtvCtrl(list_ctrl_ptr->win_id, list_ctrl_ptr->edit_handle);

            if (param)
            {
                MMI_NOTIFY_T* notify_ptr = (MMI_NOTIFY_T*)param;
                if (notify_ptr->src_handle == list_ctrl_ptr->edit_handle)
                {
                    MMK_RunCtrlProc(list_ctrl_ptr->edit_handle, MSG_CTL_PAINT, PNULL);
                }
            }
        }
        break;
        
    case MSG_CTL_LOSE_ACTIVE:
        obj_ptr->is_active = FALSE;
        ScrollUpList(obj_ptr);
        GUICTRL_SendNotify(obj_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        // hide listbox when listbox lost focus.
        ScrollUpList(obj_ptr);

        // 查看dropdownlist是否是当前的焦点控件
        obj_ptr->is_active = MMK_IsActiveCtrl(obj_ptr->handle);
        if (!obj_ptr->is_active)
        {
            GUICTRL_SendNotify(obj_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
        }
        break;
        
    case MSG_CTL_GET_FOCUS: 
        list_ctrl_ptr->is_focus = TRUE;
        break;
        
    case MSG_CTL_LOSE_FOCUS:
        list_ctrl_ptr->button_status = GUIDNLIST_BUTTON_RELEASED;
        list_ctrl_ptr->is_focus = FALSE;
        if (NULL != list_ctrl_ptr->list_handle)
        {
            MMK_DestroyControl(list_ctrl_ptr->list_handle);
            list_ctrl_ptr->list_handle = 0;
            obj_ptr->rect = list_ctrl_ptr->orig_rect;
        }
        list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;
        break;

    case MSG_NOTIFY_IM_HIDE:
        break;
        
    case MSG_CTL_PAINT:
        DrawDropDownList(obj_ptr);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T point = {0};
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);     
            DropDownlistOnPenUp(obj_ptr,point, param);        
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);     
            DropDownlistOnPenDown(obj_ptr,point, param);        
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point = {0};
            
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);     
            DropDownlistOnPenMove(obj_ptr,point, param);        
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_WEB:
        if(GUIDNLIST_CLOSE_STATUS == list_ctrl_ptr->cur_status && list_ctrl_ptr->is_web_key_open_list)
        {
            ScrollDownList(obj_ptr, DDLIsSetListFocus(obj_ptr), FALSE);
        }
        else if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
        {
            SelectFromList(obj_ptr);    
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_NOTIFY_MIDSK:
        if(GUIDNLIST_CLOSE_STATUS == list_ctrl_ptr->cur_status && list_ctrl_ptr->is_mid_softkey_open_list)
        {
            ScrollDownList(obj_ptr, DDLIsSetListFocus(obj_ptr), FALSE);
        }
        else if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
        {
            SelectFromList(obj_ptr);    
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_APP_OK:
        if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
        {
            SelectFromList(obj_ptr);    
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_APP_CANCEL:
        if (GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
        {
            ScrollUpList(obj_ptr);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_NOTIFY_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
        {
            SelectFromList(obj_ptr);    
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_NOTIFY_CANCEL:
        if (DDLIsEditType(obj_ptr))
        {
            recode = MMI_RESULT_FALSE;
        }
        else 
        {
            if (GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
            {
                ScrollUpList(obj_ptr);
            }
        }
        break;

        //处理控件属性改变消息
    case MSG_CTL_PROPERTY_CHANGE:
        break;
        
     case MSG_NOTIFY_LINE_CHANGE:
        {
            // 因为form里拿的是孩子的句柄，所以这里改变下
            GUIFORM_NOTIFY_LINE_T* notify_line_ptr = (GUIFORM_NOTIFY_LINE_T*)param;

            if (PNULL != notify_line_ptr)
            {
                if ( notify_line_ptr->ctrl_handle == list_ctrl_ptr->edit_handle)
                {
                    recode = MMI_RESULT_FALSE;
                }
                else
                {
                    recode = MMI_RESULT_TRUE;
                }
                notify_line_ptr->ctrl_handle = obj_ptr->handle;
            }            
        }
        break;

    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        if (PNULL != list_ctrl_ptr->list_handle)
        {
            recode = GUIAPICTRL_HandleEvent(list_ctrl_ptr->list_handle, msg_id, PNULL);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_NOTIFY_RESIZE:
        if (GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
        {
            ScrollUpList(obj_ptr);
            if (MMK_GetParentCtrlHandle(obj_ptr->handle))
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
		break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;     
}

/*****************************************************************************/
//  Description : get edit cursor rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownGetCursorRect(
                                    CTRLBASE_OBJ_T *obj_ptr,  //in:
                                    GUI_RECT_T      *rect_ptr   //in/out:
                                    )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    // 调用edit的接口
    if (PNULL != list_ctrl_ptr && 0 != list_ctrl_ptr->edit_handle)
    {
        result = IGUICTRL_GetSelectRect(MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), rect_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : get edit cursor index,is in the first or last line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownGetCursorIndex(
                                     CTRLBASE_OBJ_T *obj_ptr,       //in:
                                     BOOLEAN        *is_first_ptr,  //in/out:
                                     BOOLEAN        *is_last_ptr    //in/out:
                                     )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    // 调用edit的接口
    if (PNULL != list_ctrl_ptr && 0 != list_ctrl_ptr->edit_handle)
    {
        result = IGUICTRL_GetSelectIndex(MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), is_first_ptr, is_last_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : set edit cursor index,is in the first or last line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownSetCursorIndex(
                                     CTRLBASE_OBJ_T *obj_ptr,   //in:
                                     BOOLEAN        is_first,   //in:
                                     BOOLEAN        is_last     //in:
                                     )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    // 调用edit的接口
    if (PNULL != list_ctrl_ptr && 0 != list_ctrl_ptr->edit_handle)
    {
        result = IGUICTRL_SetSelectIndex(MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), is_first, is_last);
    }

    return result;
}

/*****************************************************************************/
//  Description : 画背景
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawDropdownlistBg(
                              CTRLBASE_OBJ_T *obj_ptr
                              )
{
    GUI_RECT_T          image_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if(PNULL != obj_ptr
        && DropDownListTypeOf((CTRLBASE_OBJ_T*)obj_ptr)
        && GUI_IntersectRect(&cross_rect, obj_ptr->display_rect,obj_ptr->rect))
    {
        if (MMITHEME_CheckImageID(list_ctrl_ptr->style_info.bg_info.img_id))
        {
            // 计算背景图片显示的区域
            image_rect.bottom = image_rect.top + (cross_rect.bottom - cross_rect.top);
            image_rect.right  = image_rect.left + (cross_rect.right - cross_rect.left);
        }

        //set bg display info
        bg_display.is_transparent = FALSE;
        bg_display.rect           = list_ctrl_ptr->orig_rect;
        bg_display.display_rect   = cross_rect;
        bg_display.img_rect       = image_rect;
        bg_display.ctrl_handle    = obj_ptr->handle;
        bg_display.win_handle     = list_ctrl_ptr->win_id;

        GUI_DisplayBg(&list_ctrl_ptr->style_info.bg_info,
            &bg_display,
            &(obj_ptr->lcd_dev_info));
    }
}

/*****************************************************************************/
//Description : 显示下拉框,区域\背景\文字等信息
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:
/*****************************************************************************/
LOCAL void DrawDropDownList(
                            CTRLBASE_OBJ_T *obj_ptr    
                            )
{
    
    uint16              index = 0xffff;
    GUI_RECT_T          text_rect = GetDropdownlistRect(obj_ptr,GUIDNLIST_TEXT_RECT);
    GUI_COLOR_T         font_color = 0;
    GUI_RECT_T          cross_rect = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if ((PNULL != list_ctrl_ptr) && 
        (!GUIAPICTRL_GetState(obj_ptr->handle,GUICTRL_STATE_INVISIBLE)) &&
        (GUI_IntersectRect(&cross_rect,obj_ptr->display_rect,obj_ptr->rect)))	
    {
        // 画背景色
        DrawDropdownlistBg(obj_ptr);
    
        // 画边框
        GUI_DisplayBorder(list_ctrl_ptr->orig_rect, 
            obj_ptr->display_rect,
            &list_ctrl_ptr->style_info.border,
            &obj_ptr->lcd_dev_info);

        if (GUIDNLIST_NORMAL == list_ctrl_ptr->ctrl_type_info.ctrl_type)
        {
            //display selected content
            index = list_ctrl_ptr->current_index;//get current index
            if(0xffff != index && list_ctrl_ptr->total_item_num != 0)
            {
                uint16 text_rect_width = 0;
                GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
                MMI_STRING_T    text_info = {0};
                GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_ALIGN_BY_DIR;
                
                font_color = list_ctrl_ptr->style_info.font_color;
                
                // 设置文本风格
                str_style.align = ALIGN_LVMIDDLE;
                str_style.font = list_ctrl_ptr->style_info.font;
                str_style.font_color = font_color;
                str_style.char_space = list_ctrl_ptr->style_info.font_space;
                
                text_rect = GetDropdownlistRect(obj_ptr,GUIDNLIST_TEXT_RECT);
                text_rect.left += list_ctrl_ptr->style_info.left_space;
                // text
                text_info.wstr_ptr = list_ctrl_ptr->all_item_ptr[index].wstr_ptr;
                text_info.wstr_len = list_ctrl_ptr->all_item_ptr[index].wstr_len;
                // 文本宽度
                text_rect_width = text_rect.right - text_rect.left + 1;
                
                if ( list_ctrl_ptr->is_ellipsis )
                {
                    str_state |= GUISTR_STATE_ELLIPSIS;
                }
                
                GUI_ReplaceChar(&text_info, LF_CHAR, BL_CHAR);
                GUI_ReplaceChar(&text_info, CR_CHAR, BL_CHAR);
                
                // 画字
                GUISTR_DrawTextToLCDInRect(&obj_ptr->lcd_dev_info,
                    (const GUI_RECT_T*)&text_rect, 
                    (const GUI_RECT_T*)&obj_ptr->display_rect, 
                    (const MMI_STRING_T*)&text_info, 
                    &str_style, 
                    str_state, 
                    GUISTR_TEXT_DIR_AUTO);
            }
        }

        //draw button
        DrawDropDownlistButton(obj_ptr, 
            GetDropdownlistRect(obj_ptr,GUIDNLIST_BUTTON_RECT),
            list_ctrl_ptr->button_status,
            list_ctrl_ptr->win_id);

        // when update dropdownlist only, we should also paint editbox
        if ( 0 != list_ctrl_ptr->edit_handle
            && !MMK_IsWinOnPaint(list_ctrl_ptr->win_id))
        {
            IGUICTRL_HandleEvent(
                (IGUICTRL_T*)MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle),
                MSG_CTL_PAINT, PNULL);
        }
    }

    return;
}

/*****************************************************************************/
//Description : get pointed rect
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDropdownlistRect(
                                     CTRLBASE_OBJ_T *obj_ptr ,//text rect ,it is for calculate other rect
                                     GUIDROPDOWNLIST_POSITION_E pos// pointered position
                                     )
{
    GUI_RECT_T rect = {0,0,0,0};
    uint16  button_width = 0;
    GUI_RECT_T dropdownlist_rect = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (NULL != list_ctrl_ptr)
    {
        dropdownlist_rect = list_ctrl_ptr->orig_rect;
        
        if (list_ctrl_ptr->is_has_button)
        {
            GUIRES_GetImgWidthHeight(&button_width,PNULL,list_ctrl_ptr->style_info.unpressed_arrow_icon,list_ctrl_ptr->win_id);
        }
        
        switch(pos)
        {
        case GUIDNLIST_BUTTON_RECT:
            rect.top = (int16)(dropdownlist_rect.top + list_ctrl_ptr->style_info.border.width);
            rect.bottom = (int16)(dropdownlist_rect.bottom - list_ctrl_ptr->style_info.border.width);
            rect.right = (int16)(dropdownlist_rect.right - list_ctrl_ptr->style_info.border.width);
            rect.left = (int16)(rect.right - button_width + 1);
            break;
            
        case GUIDNLIST_TEXT_RECT:
            rect.top = (int16)(dropdownlist_rect.top + list_ctrl_ptr->style_info.border.width);
            rect.bottom = (int16)(dropdownlist_rect.bottom - list_ctrl_ptr->style_info.border.width);
            rect.left = (int16)(dropdownlist_rect.left + list_ctrl_ptr->style_info.border.width);
            rect.right = (int16)(dropdownlist_rect.right - button_width - list_ctrl_ptr->style_info.border.width);
            break;
            
        default:
            //SCI_TRACE_LOW:"GUI GetDropdownlistRect: pos = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1383_112_2_18_3_15_29_93,(uint8*)"d", pos);
            break;
            
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUI GetDropdownlistRect: PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1390_112_2_18_3_15_29_94,(uint8*)"");
    }

    if (GUI_IsInvalidRect(rect))
    {
        SCI_MEMSET(&rect, 0, sizeof(GUI_RECT_T));
    }
    
    return rect;
}

/*****************************************************************************/
//Description : draw the button
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:
/*****************************************************************************/
LOCAL void DrawDropDownlistButton(
                                  CTRLBASE_OBJ_T *obj_ptr ,
                                  GUI_RECT_T rect,// button rect
                                  GUIDROPDOWNLIST_BUTTON_STATUS_E button_status,//released or pressed
                                  MMI_WIN_ID_T win_id
                                  )
{
    MMI_IMAGE_ID_T      image_id            = 0;
    GUI_RECT_T          image_rect          = {0};
    GUI_RECT_T          cross_rect          = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL == list_ctrl_ptr)
    {
        return;
    }

    if (list_ctrl_ptr->is_has_button)
    {
        switch(button_status)
        {
        case GUIDNLIST_BUTTON_PRESSED:
            image_id = list_ctrl_ptr->style_info.pressed_arrow_icon;
            break;
        case GUIDNLIST_BUTTON_RELEASED:
            image_id = list_ctrl_ptr->style_info.unpressed_arrow_icon;
            break;
        default:
            break;
        }
        
        if(GUI_IntersectRect(&cross_rect, obj_ptr->display_rect, rect))
        {
            //set image rect
            image_rect.left   = (int16)(cross_rect.left - rect.left);
            image_rect.top    = (int16)(cross_rect.top - rect.top);
            image_rect.right  = (int16)(image_rect.left + cross_rect.right - cross_rect.left);
            image_rect.bottom = (int16)(image_rect.top + cross_rect.bottom - cross_rect.top);
            
            GUIRES_DisplayImg(PNULL, &cross_rect, &image_rect, win_id, image_id, &obj_ptr->lcd_dev_info);
        }
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1450_112_2_18_3_15_29_95,(uint8*)"d", list_ctrl_ptr->is_has_button);
    }
}

/*****************************************************************************/
//  Description : process MSG_TP_PRESS_UP
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownlistOnPenUp(
                                       CTRLBASE_OBJ_T *obj_ptr ,//dropdownlist ptr
                                       GUI_POINT_T point,//position
                                       DPARAM param
                                       )
{
    MMI_RESULT_E                recode       = MMI_RESULT_FALSE;
    GUIDROPDOWNLIST_POSITION_E pos          = GUIDNLIST_TEXT_RECT;
    GUIDROPDOWNLIST_POSITION_E pos_back     = GUIDNLIST_TEXT_RECT;//get position, button , text or list rect,pos_back is pos buckup
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if (PNULL == list_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DropDownlistOnPenUp: PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1470_112_2_18_3_15_29_96,(uint8*)"");
        return recode;
    }

    if (GUIDNLIST_BUTTON_PRESSED == list_ctrl_ptr->button_status)
    {
        list_ctrl_ptr->button_status = GUIDNLIST_BUTTON_RELEASED;
        DrawDropDownlistButton(obj_ptr, GetDropdownlistRect(obj_ptr,GUIDNLIST_BUTTON_RECT),
            list_ctrl_ptr->button_status,list_ctrl_ptr->win_id);
    }

    //not slide
    if (!MMK_GET_TP_SLIDE(param))
    {
        pos = GetPenPosition(obj_ptr,point);
    
        pos_back = pos;
    
        switch(pos)
        {
        case GUIDNLIST_BUTTON_RECT:
        case GUIDNLIST_TEXT_RECT:
            if(GUIDNLIST_CLOSE_STATUS == list_ctrl_ptr->cur_status)
            {
                recode = ScrollDownList(obj_ptr, DDLIsSetListFocus(obj_ptr), FALSE);
            }
            else if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
            {
                recode = ScrollUpList(obj_ptr);    
            }
            break;
        
        case GUIDNLIST_LIST_RECT:	
            //正常情况不应该进入此case, mmk会先把tp消息派发给list
            if(GUIDNLIST_OPEN_STATUS == list_ctrl_ptr->cur_status)
            {
                recode = SelectFromList(obj_ptr);    
            }
            break;
        
        default:
            break;
        }
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : process MSG_TP_PRESS_DOWN
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownlistOnPenDown(
                                         CTRLBASE_OBJ_T *obj_ptr,//dropdownlist ptr
                                         GUI_POINT_T point,//position
                                         DPARAM param
                                         )
{
    MMI_RESULT_E                recode       = MMI_RESULT_FALSE;
    GUIDROPDOWNLIST_POSITION_E pos          = GUIDNLIST_TEXT_RECT;
    GUIDROPDOWNLIST_POSITION_E pos_back     = GUIDNLIST_TEXT_RECT;//get position, button , text or list rect,pos_back is pos buckup
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if (PNULL == list_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DropDownlistOnPenDown: PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1534_112_2_18_3_15_29_97,(uint8*)"");
        return recode;
    }
    
    pos = GetPenPosition(obj_ptr, point);
    
    pos_back = pos;
    
    switch(pos)
    {
    case GUIDNLIST_BUTTON_RECT:
        list_ctrl_ptr->button_status = GUIDNLIST_BUTTON_PRESSED;
        DrawDropDownlistButton(obj_ptr, GetDropdownlistRect(obj_ptr,GUIDNLIST_BUTTON_RECT),list_ctrl_ptr->button_status,list_ctrl_ptr->win_id);
        break;

    default:
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : process MSG_TP_PRESS_MOVE
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DropDownlistOnPenMove(
                                         CTRLBASE_OBJ_T *obj_ptr,//dropdownlist ptr
                                         GUI_POINT_T point,//position
                                         DPARAM param
                                         )
{
    MMI_RESULT_E               recode       = MMI_RESULT_FALSE;
    GUIDROPDOWNLIST_POSITION_E pos          = GUIDNLIST_TEXT_RECT;
    GUIDROPDOWNLIST_POSITION_E pos_back     = GUIDNLIST_TEXT_RECT;//get position, button , text or list rect,pos_back is pos buckup
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if (PNULL == list_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"DropDownlistOnPenMove: PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1573_112_2_18_3_15_30_98,(uint8*)"");
        return recode;
    }
    
    pos = GetPenPosition(obj_ptr, point);
    
    pos_back = pos;
    
    switch(pos)
    {
    case GUIDNLIST_BUTTON_RECT:
        list_ctrl_ptr->button_status = GUIDNLIST_BUTTON_PRESSED;
        break;

    default:
        list_ctrl_ptr->button_status = GUIDNLIST_BUTTON_RELEASED;
        break;
    }

    DrawDropDownlistButton(obj_ptr, GetDropdownlistRect(obj_ptr,GUIDNLIST_BUTTON_RECT),
        list_ctrl_ptr->button_status,list_ctrl_ptr->win_id);
    
    return recode;
}


/*****************************************************************************/
// Description : 加载列表数据
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/ 
LOCAL void LoadDataToList(
                          CTRLBASE_OBJ_T *obj_ptr //ctrl ptr
                          )
{
    uint16              i           = 0;
    uint16              current_index = 0;
    CTRLLIST_ITEM_T      item        = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   = {0};/*lint !e64*/
    GUI_RECT_T          rect        = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if (PNULL == list_ctrl_ptr || 0 == list_ctrl_ptr->list_handle)
    {
        //SCI_TRACE_LOW:"LoadDataToList: PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1615_112_2_18_3_15_30_99,(uint8*)"");
        return;
    }
    
    CTRLLIST_SetMaxItem(list_ctrl_ptr->list_handle,(uint16)(list_ctrl_ptr->total_item_num + 1), FALSE);

    rect = CalListRect(obj_ptr);

    GUIAPICTRL_SetRect(list_ctrl_ptr->list_handle, &rect);

    for(i = 0; i < list_ctrl_ptr->total_item_num; i++)//load data to list
    {
        SCI_MEMSET(&item,0,sizeof(CTRLLIST_ITEM_T));
        item.data_ptr = &item_data;
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_len = list_ctrl_ptr->all_item_ptr[i].wstr_len;
        
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = list_ctrl_ptr->all_item_ptr[i].wstr_ptr;
        if (!list_ctrl_ptr->is_disp_detail)
        {
            item.style_ptr = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_TEXT);
        }
        else
        {
            item.style_ptr = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT);
            
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_len = list_ctrl_ptr->all_item_detail_ptr[i].wstr_len;
            
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = list_ctrl_ptr->all_item_detail_ptr[i].wstr_ptr;
        }
        
        CTRLLIST_AppendItem(list_ctrl_ptr->list_handle,&item );
    }
    //set current item

    if (list_ctrl_ptr->current_index >= list_ctrl_ptr->total_item_num)
    {
        list_ctrl_ptr->current_index = current_index = 0;
    }
    else
    {
        current_index = list_ctrl_ptr->current_index;
    }
    
    CTRLLIST_SetCurItemIndex( list_ctrl_ptr->list_handle, current_index );
}

/*****************************************************************************/
// Description : get pen position
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/ 
LOCAL GUIDROPDOWNLIST_POSITION_E GetPenPosition(
                                                 CTRLBASE_OBJ_T *obj_ptr ,
                                                 GUI_POINT_T point
                                                 )
{
    GUI_RECT_T                  text_rect = {0};
    GUIDROPDOWNLIST_POSITION_E pos = GUIDNLIST_OTHER;
    GUI_RECT_T rect = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;
    
    if (PNULL == list_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"GetPenPosition: PNULL INPUT PARAM!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1681_112_2_18_3_15_30_100,(uint8*)"");
        return pos;
    }
    
    rect = list_ctrl_ptr->orig_rect;
    text_rect = GetDropdownlistRect(obj_ptr,GUIDNLIST_TEXT_RECT);
    
    if(point.x >= rect.left && point.x <= rect.right )
    {
        if(point.y <= rect.bottom && point.y >=rect.top )
        {
            if(point.x < text_rect.right)
            {
                pos = GUIDNLIST_TEXT_RECT;
            }
            else
            {
                pos = GUIDNLIST_BUTTON_RECT;
            }
        }
    }
    else
    {
        pos = GUIDNLIST_OTHER;
    }
    
    return (pos);
}

/*****************************************************************************/
//  Description : 计算下拉框，下拉时的区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalListRect(
                             CTRLBASE_OBJ_T *obj_ptr 
                             )
{
    GUI_RECT_T  rect = {0};
    GUI_RECT_T  client_rect = {0};
    uint16      rect_height = 0;
    uint16      item_num  = 0;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL == list_ctrl_ptr)
    {
        //SCI_TRACE_LOW:"CalListRect error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1725_112_2_18_3_15_30_101,(uint8*)"");
        return rect;
    }

    // use the smaller value between page item num which user set and total num.
    list_ctrl_ptr->cur_page_item_num = (uint16)(MIN(list_ctrl_ptr->page_item_num,list_ctrl_ptr->total_item_num));

    rect = obj_ptr->display_rect;

    client_rect = MMITHEME_GetClientRectEx(list_ctrl_ptr->win_id);

    rect_height = client_rect.bottom - client_rect.top + 1;

    if ((GUIDNLIST_SHOW_AUTOSET == list_ctrl_ptr->show_type && rect.top > client_rect.top + rect_height/2)
        || GUIDNLIST_SHOW_UPWARD == list_ctrl_ptr->show_type)    // 向上显示
    {
        if (0 < list_ctrl_ptr->style_info.item_height)
        {
            // max item in such region
            // rect.top 应理解为rect.top - 1 + 1
            // -1是因为listbox要从dropdownlist的上一个象素开始显示
            // +1是因为rect算到height时要加1
            item_num = (rect.top - client_rect.top) / list_ctrl_ptr->style_info.item_height;
        }
        else
        {
            item_num = 0;
        }
    
        list_ctrl_ptr->cur_page_item_num = MIN(item_num, list_ctrl_ptr->cur_page_item_num);

        rect.bottom = (int16)(obj_ptr->display_rect.top-1);
        rect.top = (int16)(rect.bottom - list_ctrl_ptr->style_info.item_height*list_ctrl_ptr->cur_page_item_num - list_ctrl_ptr->style_info.border.width*2 + 1);
    }
    else if (GUIDNLIST_SHOW_DOWNWARD == list_ctrl_ptr->show_type
        || GUIDNLIST_SHOW_AUTOSET == list_ctrl_ptr->show_type)   // 向下显示
    {
        if (0 < list_ctrl_ptr->style_info.item_height)
        {
            // max item in such region
            // 之所以减2，是因为(rect.bottom + 1 + 1)，
            // 前一个+1是因为listbox要从dropdownlist下一个象素显示，
            // 后一个+1是因为rect算到高度时要加1
            item_num = (client_rect.bottom - rect.bottom - 2) / list_ctrl_ptr->style_info.item_height;
        }
        else
        {
            item_num = 0;
        }
    
        list_ctrl_ptr->cur_page_item_num = MIN(item_num, list_ctrl_ptr->cur_page_item_num);

        rect.top = (int16)(obj_ptr->display_rect.bottom+1);      
        rect.bottom = (int16)(rect.top+ list_ctrl_ptr->style_info.item_height*list_ctrl_ptr->cur_page_item_num + list_ctrl_ptr->style_info.border.width*2 - 1);
    }
    else
    {
        //SCI_TRACE_LOW:"CalListRect show type = %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1782_112_2_18_3_15_30_102,(uint8*)"d", list_ctrl_ptr->show_type);
    }

    return (rect);
}

/*****************************************************************************/
//  Description : 计算下拉框，下拉时的区域和原始区域的合并区域
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetWholeRect(
                        CTRLBASE_OBJ_T *obj_ptr ,   // [in]
                        GUI_RECT_T *whole_rect_ptr,                 // [out] whole rect
                        GUI_RECT_T *whole_display_rect_ptr          // [out] that not include invisible rect
                        )
{
    GUI_RECT_T list_rect = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    if (PNULL == list_ctrl_ptr || PNULL == whole_rect_ptr || PNULL == whole_display_rect_ptr)
    {
        return;
    }

    if (PNULL != list_ctrl_ptr->list_handle)
    {
        // get listbox's rect
        GUIAPICTRL_GetRect(list_ctrl_ptr->list_handle, &list_rect );

        *whole_rect_ptr = list_rect;
        *whole_display_rect_ptr = list_rect;

        whole_rect_ptr->left = list_ctrl_ptr->orig_rect.left;
        whole_rect_ptr->right = list_ctrl_ptr->orig_rect.right;

        whole_display_rect_ptr->left = obj_ptr->display_rect.left;
        whole_display_rect_ptr->right = obj_ptr->display_rect.right;

        if (list_rect.top >= list_ctrl_ptr->orig_rect.bottom) // 列表在下
        {
            whole_rect_ptr->top = list_ctrl_ptr->orig_rect.top;
            whole_rect_ptr->bottom += list_ctrl_ptr->style_info.border.width;

            whole_display_rect_ptr->top = obj_ptr->display_rect.top;
            whole_display_rect_ptr->bottom += list_ctrl_ptr->style_info.border.width;
        }
        else if (list_rect.bottom <= list_ctrl_ptr->orig_rect.top) // 列表在上
        {
            whole_rect_ptr->top -= list_ctrl_ptr->style_info.border.width;
            whole_rect_ptr->bottom = list_ctrl_ptr->orig_rect.bottom;

            whole_display_rect_ptr->top -= list_ctrl_ptr->style_info.border.width;
            whole_display_rect_ptr->bottom = obj_ptr->display_rect.bottom;
        }
    }
    else
    {
        *whole_rect_ptr = list_ctrl_ptr->orig_rect;
        *whole_display_rect_ptr = obj_ptr->display_rect;
    }
}
/*****************************************************************************/
//  Description : create dynamic List
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void CreateListBox( 
                         CTRLBASE_OBJ_T *obj_ptr ,//ctrl ptr
                         BOOLEAN is_set_focus,
                         BOOLEAN is_must_create
                         )
{
    GUI_RECT_T          rect = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T*)obj_ptr;

    rect = CalListRect(obj_ptr);
    
    if(PNULL != list_ctrl_ptr)
    {
        if ( 0 == list_ctrl_ptr->total_item_num)
        {
            // 这里要释放内嵌的列表实例的内存,否则会内存泄露
            if (PNULL != list_ctrl_ptr->list_handle)
            {
                MMK_DestroyControl( list_ctrl_ptr->list_handle);
                list_ctrl_ptr->list_handle = 0;
            }
            list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;
        }
        else if((GUIDNLIST_CLOSE_STATUS == list_ctrl_ptr->cur_status) // close状态时重新创建
            || (is_must_create)) // 无条件重新创建
        {
            GUILIST_INIT_DATA_T init_data = {0};
            MMI_CONTROL_CREATE_T create = {0};
            
            init_data.both_rect.h_rect = init_data.both_rect.v_rect = rect;
            init_data.type = GUILIST_TEXTLIST_E;
            init_data.max_item_num = (uint16)(list_ctrl_ptr->total_item_num + 1);
            
            // 这里要释放内嵌的列表实例的内存,否则会内存泄露
            if (PNULL != list_ctrl_ptr->list_handle)
            {
                MMK_DestroyControl( list_ctrl_ptr->list_handle);
                list_ctrl_ptr->list_handle = 0;
            }
            
            create.ctrl_id = 0;
            create.guid    = SPRD_GUI_LIST_ID;
            create.parent_win_handle = list_ctrl_ptr->win_id;
            create.parent_ctrl_handle = obj_ptr->handle;
            create.init_data_ptr = &init_data;

            list_ctrl_ptr->list_handle = GUICTRL_GetCtrlHandle(MMK_CreateControl( &create ));
            
            if(PNULL != list_ctrl_ptr->list_handle)
            {
                GUIAPICTRL_SetState(list_ctrl_ptr->list_handle,GUICTRL_STATE_TOPMOST_1,TRUE);
                
                GUIAPICTRL_SetBorder(list_ctrl_ptr->list_handle, &list_ctrl_ptr->style_info.border);
                
                //set font color
                CTRLLIST_SetTextFont(list_ctrl_ptr->list_handle, list_ctrl_ptr->style_info.font, list_ctrl_ptr->style_info.font_color);
                
                CTRLLIST_SetListState(list_ctrl_ptr->list_handle, list_ctrl_ptr->list_state, FALSE );
                // 将listbox的状态设置成用在dropdownlist中的，listbox会做特殊处理。
                
                if (GUIDNLIST_NORMAL == list_ctrl_ptr->ctrl_type_info.ctrl_type)
                {
                    GUIAPICTRL_SetBg(list_ctrl_ptr->list_handle, &list_ctrl_ptr->style_info.list_bg_info);
                    CTRLLIST_SetListState(list_ctrl_ptr->list_handle, GUILIST_STATE_DROPDOWNLIST, TRUE );
                }
                else
                {
                    CTRLLIST_SetBgImage(list_ctrl_ptr->list_handle, list_ctrl_ptr->style_info.list_bg_info.img_id , FALSE);
                    // 这里要按照正常list控件进行控制，所以不能把GUILIST_STATE_DROPDOWNLIST属性给它
                    CTRLLIST_SetListState(list_ctrl_ptr->list_handle, GUILIST_STATE_DROPDOWNLIST, FALSE );
                }
                
                if (is_set_focus)
                {
                    MMK_SetActiveCtrl(list_ctrl_ptr->list_handle, FALSE );
                }
                else
                {
                    //////////////////////////////////////////////////////////////////////////
                    // fixed by feng.xiao for NEWMS00178620
                    // -fix: first item can NOT get focused
                    GUIAPICTRL_SetState(list_ctrl_ptr->list_handle, GUICTRL_STATE_TOPMOST_1|GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
                    CTRLLIST_SetListState(list_ctrl_ptr->list_handle, GUILIST_STATE_ACTIVE, TRUE);
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"GUIDROPDOWNLIST CreateListBox failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1930_112_2_18_3_15_30_103,(uint8*)"");
        }
    }

    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetListState(
                                            MMI_CTRL_ID_T  ctrl_id,
                                            GUILIST_STATE_T  state,
                                            BOOLEAN is_true
                                            )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    if (PNULL != list_ctrl_ptr)
    {
        if( is_true )
        {
            list_ctrl_ptr->list_state |= state;
        }
        else
        {
            list_ctrl_ptr->list_state &= ~state;
        }
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : load item array   of GUIDROPDOWNLIST
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemArray( 
                                               MMI_CTRL_ID_T  ctrl_id, // control id
                                               MMI_STRING_T *item_array_ptr,//load item array
                                               uint16 array_size //item array total num
                                               )
{
    BOOLEAN result = FALSE;
    uint16 i = 0;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

	if (PNULL != list_ctrl_ptr)
	{    
		CTRLBASE_OBJ_T *obj_ptr = (CTRLDROPDOWNLIST_OBJ_T *)list_ctrl_ptr;
    
		if(PNULL != item_array_ptr)
		{
			if(array_size > 0)
			{
				if(list_ctrl_ptr->is_auto_size)
				{
					DropdownSetMaxItemByPtr(obj_ptr, array_size);
				}
            
				for(i = 0; i < array_size; i++)
				{
					CTRLDROPDOWNLIST_AppendItem(ctrl_id, &item_array_ptr[i]);
				}
            
				result = TRUE;
			}
			else
			{
				//SCI_TRACE_LOW:"CTRLDROPDOWNLIST_AppendItemArray:array_size = %d is error!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_1998_112_2_18_3_15_30_104,(uint8*)"d",array_size);
			}

		}
		else
		{
			//SCI_TRACE_LOW:"CTRLDROPDOWNLIST_AppendItemArray item_array_ptr is NULL!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_2004_112_2_18_3_15_30_105,(uint8*)"");
		}
	}


    return result;
}

/*****************************************************************************/
//  Description : 传入字符串,添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItem( 
                                          MMI_CTRL_ID_T ctrl_id, // control id
                                          const MMI_STRING_T *item_ptr //item info
                                          )
{
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *obj_ptr = (CTRLBASE_OBJ_T *)GetDropDownListPtr(ctrl_id);

    if(PNULL != obj_ptr)
    {
        result =  DropdownAppendItemByPtr(obj_ptr, item_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : 传入text id ,添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN DropdownAppendItemByPtr( 
                                      CTRLBASE_OBJ_T *obj_ptr, // control id
                                      const MMI_STRING_T *item_ptr //item info
                                      )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if(DropDownListTypeOf((CTRLBASE_OBJ_T*)list_ctrl_ptr))
    {
        result =  DropdownInsertItemByPtr(obj_ptr, item_ptr, GUIDDL_ITEM_INFO_E, list_ctrl_ptr->total_item_num);
    }

    return result;
}

/*****************************************************************************/
//  Description : 传入字符串,添加dropdownlist item 的详情
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AddItemDetail( 
                                             MMI_CTRL_ID_T ctrl_id, // control id
                                             uint16 item_index,
                                             const MMI_STRING_T *item_ptr //item info
                                             )
{
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *obj_ptr = (CTRLBASE_OBJ_T *)GetDropDownListPtr(ctrl_id);

    if(PNULL != obj_ptr)
    {
        result =  DropdownAddItemDetailByPtr(obj_ptr, item_index, item_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : 传入字符串,添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemExt( 
                                             MMI_CTRL_ID_T ctrl_id, // control id
                                             const MMI_STRING_T *item_ptr, //item info
                                             const MMI_STRING_T *item_detail_ptr //item info
                                             )
{
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *obj_ptr = (CTRLBASE_OBJ_T *)GetDropDownListPtr(ctrl_id);

    if(PNULL != obj_ptr)
    {
        result = DropdownAppendItemByPtr(obj_ptr, item_ptr);

        DropdownAddItemDetailByPtr(obj_ptr, ((CTRLDROPDOWNLIST_OBJ_T *)obj_ptr)->total_item_num - 1, item_detail_ptr);
    }

    return result;
}

/*****************************************************************************/
// brief 传入字符串,添加dropdownlist item 的详情
// @author xiaoqing.lu
// @param ctrl_id [in] 控件ID
// @param text_id [in] ITEM text id
// @param item_index [in] ITEM index
// @retval true/false
// @return 返回成功或失败
// @note 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AddItemDetailById( 
                                                 MMI_CTRL_ID_T ctrl_id, // control id
                                                 uint16 item_index,
                                                 MMI_TEXT_ID_T text_id //item info
                                                 )
{
    MMI_STRING_T item_string = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)GetDropDownListPtr(ctrl_id);

    if(PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }
    
    MMITHEME_GetResText(text_id, list_ctrl_ptr->win_id, &item_string);

    return CTRLDROPDOWNLIST_AddItemDetail(ctrl_id, item_index, &item_string);
}

/*****************************************************************************/
//  Description : 传入text id ,添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN DropdownAddItemDetailByPtr( 
                                         CTRLBASE_OBJ_T *obj_ptr, // control id
                                         uint16 item_index,
                                         const MMI_STRING_T *item_ptr //item info
                                         )
{
    BOOLEAN result = FALSE;

    if(PNULL == obj_ptr)
    {
        return FALSE;
    }
        
    if(DropDownListTypeOf(obj_ptr))
    {
        result =  DropdownInsertItemByPtr(obj_ptr, item_ptr, GUIDDL_DETAIL_INFO_E, item_index);
    }

    return result;
}

/*****************************************************************************/
//  Description : 传入text_id,添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemById( 
                                              MMI_CTRL_ID_T ctrl_id, // control id
                                              MMI_TEXT_ID_T text_id //text id
                                              )
{
    MMI_STRING_T item_string = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)GetDropDownListPtr(ctrl_id);

    if(PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }
    
    MMITHEME_GetResText(text_id, list_ctrl_ptr->win_id, &item_string);

    return CTRLDROPDOWNLIST_AppendItem(ctrl_id, &item_string);
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DropdownInsertItemByPtr(
                                      CTRLBASE_OBJ_T *obj_ptr,
                                      const MMI_STRING_T* item_ptr,
                                      uint32 item_type,
                                      uint16 pos
                                      )
{
    BOOLEAN result = FALSE;
    MMI_STRING_T* item_info_ptr = PNULL;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if(PNULL == item_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_2178_112_2_18_3_15_31_106,(uint8*)"");
        return FALSE;
    }
    
    if(PNULL == list_ctrl_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_2184_112_2_18_3_15_31_107,(uint8*)"");
        return FALSE;
    }
    if (GUIDDL_ITEM_INFO_E == item_type)
    {
        if (list_ctrl_ptr->total_item_num < list_ctrl_ptr->max_item_num)
        {
            pos = (uint16)MIN(pos, list_ctrl_ptr->total_item_num);
            
            item_info_ptr = list_ctrl_ptr->all_item_ptr + pos;
            
            if(PNULL != item_info_ptr->wstr_ptr)
            {
                DropdownDestroyItemData(item_info_ptr);
            }
            item_info_ptr->wstr_len = item_ptr->wstr_len;
            item_info_ptr->wstr_ptr = SCI_ALLOC_APP(item_info_ptr->wstr_len * sizeof(wchar));
            SCI_MEMSET(item_info_ptr->wstr_ptr, 0, (item_info_ptr->wstr_len * sizeof(wchar)));
            
            MMIAPICOM_Wstrncpy(item_info_ptr->wstr_ptr, item_ptr->wstr_ptr, item_ptr->wstr_len );
            list_ctrl_ptr->total_item_num ++;
            
            result = TRUE;
        }
    }
    else
    {
        if (pos < list_ctrl_ptr->max_item_num)
        {
            item_info_ptr = list_ctrl_ptr->all_item_detail_ptr + pos;
            
            if(PNULL != item_info_ptr->wstr_ptr)
            {
                DropdownDestroyItemData(item_info_ptr);
            }
            item_info_ptr->wstr_len = item_ptr->wstr_len;
            item_info_ptr->wstr_ptr = SCI_ALLOC_APP(item_info_ptr->wstr_len * sizeof(wchar));
            SCI_MEMSET(item_info_ptr->wstr_ptr, 0, (item_info_ptr->wstr_len * sizeof(wchar)));
            
            MMIAPICOM_Wstrncpy(item_info_ptr->wstr_ptr, item_ptr->wstr_ptr, item_ptr->wstr_len );
            
            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 设置list最大值
//  Global resource dependence : 
//  Author: jin.wang
//  Note:设置成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetMaxItem(
                                          MMI_CTRL_ID_T ctrl_id, // control id
                                          uint16 max_item_num
                                          )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)GetDropDownListPtr(ctrl_id);

    if(PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->is_auto_size = FALSE;
        result = DropdownSetMaxItemByPtr(list_ctrl_ptr, max_item_num);
    }

    return result;

}

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownDestroyAllItemData(
                                      CTRLBASE_OBJ_T *obj_ptr
                                      )
{
    uint16 i = 0;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if(PNULL != list_ctrl_ptr)
    {
        for (i = 0; i < list_ctrl_ptr->total_item_num; i++)
        {
            DropdownDestroyItemData(DropdownGetItemInfoPtr(obj_ptr, i));
            DropdownDestroyItemData(DropdownGetItemDetailInfoPtr(obj_ptr, i));
        }
    }

    return;
}

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownDestroyItemData(MMI_STRING_T* item_info_ptr)
{
    if (PNULL != item_info_ptr)
    {
        //释放text buffer
        if(PNULL != item_info_ptr->wstr_ptr)
        {
            SCI_FREE(item_info_ptr->wstr_ptr);
        }   

        item_info_ptr->wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_STRING_T* DropdownGetItemInfoPtr(
                                           const CTRLBASE_OBJ_T *obj_ptr,
                                           uint16 index  //from 0 to total_item_num-1
                                           )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if (PNULL != list_ctrl_ptr && 0 != list_ctrl_ptr->all_item_ptr && index < list_ctrl_ptr->total_item_num)
    {
        return (list_ctrl_ptr->all_item_ptr + index);
    }
    else
    {
        return PNULL;
    }
}


/*****************************************************************************/
//  Description : get the appointed item detail pointer
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_STRING_T* DropdownGetItemDetailInfoPtr(
                                                 const CTRLBASE_OBJ_T *obj_ptr,
                                                 uint16 index  //from 0 to total_item_num-1
                                                 )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if (PNULL != list_ctrl_ptr && 0 != list_ctrl_ptr->all_item_detail_ptr && index < list_ctrl_ptr->total_item_num)
    {
        return (list_ctrl_ptr->all_item_detail_ptr + index);
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : alloc all items memory of the list control
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownAllocAllItemsSpace(
                                      CTRLBASE_OBJ_T *obj_ptr  // the list control pointer
                                      )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if(PNULL != list_ctrl_ptr)
    {
        if (PNULL != list_ctrl_ptr->all_item_ptr)
        {
            SCI_FREE(list_ctrl_ptr->all_item_ptr);
        }
        
        if (PNULL != list_ctrl_ptr->all_item_detail_ptr)
        {
            SCI_FREE(list_ctrl_ptr->all_item_detail_ptr);
        }

        if (PNULL != list_ctrl_ptr->def_arry_ptr)
        {
            SCI_FREE(list_ctrl_ptr->def_arry_ptr);
        }

        if(list_ctrl_ptr->max_item_num > 0)
        {
            // alloc the space
            list_ctrl_ptr->all_item_ptr = SCI_ALLOC_APP(list_ctrl_ptr->max_item_num * sizeof(MMI_STRING_T));
            SCI_MEMSET((void*)list_ctrl_ptr->all_item_ptr, 0, (list_ctrl_ptr->max_item_num * sizeof(MMI_STRING_T)));

            // alloc the space
            list_ctrl_ptr->all_item_detail_ptr = SCI_ALLOC_APP(list_ctrl_ptr->max_item_num * sizeof(MMI_STRING_T));
            SCI_MEMSET((void*)list_ctrl_ptr->all_item_detail_ptr, 0, (list_ctrl_ptr->max_item_num * sizeof(MMI_STRING_T)));

            // alloc the space
            list_ctrl_ptr->def_arry_ptr = SCI_ALLOC_APP(list_ctrl_ptr->max_item_num * sizeof(MMI_STRING_T));
            SCI_MEMSET((void*)list_ctrl_ptr->def_arry_ptr, 0, (list_ctrl_ptr->max_item_num * sizeof(uint16)));
        }
    }   
}

/*****************************************************************************/
//  Description : reset the item pointer of list 
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL void DropdownResetListItem(
                                 CTRLBASE_OBJ_T *obj_ptr  // the list control pointer
                                 )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if(PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->total_item_num = 0;
    }
 }

/*****************************************************************************/
//  Description : 根据list控件信息设置list最大值
//  Global resource dependence : 
//  Author: jin.wang
//  Note:设置成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN DropdownSetMaxItemByPtr(
                                      CTRLBASE_OBJ_T *obj_ptr, // control id
                                      uint16 max_item_num
                                      )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;
    
    if(PNULL == list_ctrl_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_2404_112_2_18_3_15_31_108,(uint8*)"");
        return FALSE;
    }
    
    if(DropDownListTypeOf(obj_ptr))
    {
        DropdownDestroyAllItemData(obj_ptr);//清除所有item数据
        list_ctrl_ptr->max_item_num = max_item_num;

        DropdownAllocAllItemsSpace(obj_ptr);
        DropdownResetListItem(obj_ptr); 
        result = TRUE;
    }
    else
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_2418_112_2_18_3_15_31_109,(uint8*)"");
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
LOCAL MMI_STRING_T GetCurItemText( //return selected check item number
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  uint16 item_index
                                  )
{
    MMI_STRING_T item_info = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if (PNULL != list_ctrl_ptr
        && item_index < list_ctrl_ptr->total_item_num
        && PNULL != list_ctrl_ptr->all_item_ptr)
    {
        item_info = list_ctrl_ptr->all_item_ptr[item_index];
    }

    return item_info;
}

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
LOCAL MMI_STRING_T GetCurItemDetailText( //return selected check item number
                                        CTRLBASE_OBJ_T *obj_ptr,
                                        uint16 item_index
                                        )
{
    MMI_STRING_T item_info = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;
    
    if (PNULL != list_ctrl_ptr
        && item_index < list_ctrl_ptr->total_item_num
        && PNULL != list_ctrl_ptr->all_item_detail_ptr)
    {
        item_info = list_ctrl_ptr->all_item_detail_ptr[item_index];
    }

    return item_info;
}

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
LOCAL uint16 GetCurItemValue( //return selected check item number
                             CTRLBASE_OBJ_T *obj_ptr,
                             uint16 item_index
                             )
{
    uint16 arry_value = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if (PNULL != list_ctrl_ptr
        && item_index < list_ctrl_ptr->total_item_num
        && PNULL != list_ctrl_ptr->def_arry_ptr)
    {
        arry_value = list_ctrl_ptr->def_arry_ptr[item_index];
    }

    return arry_value;
}

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
LOCAL BOOLEAN SetCurItemValue( //return selected check item number
                              CTRLBASE_OBJ_T *obj_ptr,
                              uint16 item_index,
                              GUIDROPDOWNLIST_VALID_VALUE_E index
                              )
{
    BOOLEAN result = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;
    
    if (PNULL != list_ctrl_ptr
        && item_index < list_ctrl_ptr->total_item_num
        && PNULL != list_ctrl_ptr->def_arry_ptr
        && index < GUIDNLIST_VALID_VALUE_MAX)
    {
        list_ctrl_ptr->def_arry_ptr[item_index] = index;
    }

    return result;
}

/*****************************************************************************/
//  Description : 设置有效值
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetValidItemValue( 
                                                 MMI_CTRL_ID_T      ctrl_id, // control id
                                                 uint16 item_index,
                                                 GUIDROPDOWNLIST_VALID_VALUE_E index
                                                 )
{
    return SetCurItemValue(GetDropDownListPtr(ctrl_id), item_index, index);
}

/*****************************************************************************/
//  Description : create dynamic DropDownList
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_CreateDyn(
                                         MMI_CTRL_ID_T ctrl_id, // control id
                                         MMI_HANDLE_T  win_handle,//window id
                                         GUIDROPDOWNLIST_INIT_DATA_T *init_ptr
                                         )
{
    BOOLEAN               recode    = FALSE;
    MMI_CONTROL_CREATE_T  create = {0};

    if (PNULL == init_ptr)
    {
        return recode;
    }

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_DROPDOWNLIST_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = init_ptr;

    if (PNULL == MMK_CreateControl( &create ))
    {
        recode = TRUE;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 设置区域的风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetRectStyle( 
                                            MMI_CTRL_ID_T                 ctrl_id, // control id
                                            GUIDROPDOWNLIST_RECT_STYLE_E style
                                            )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->rect_style = style;   

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//Description : Get selected item indexs
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected index
/*****************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetCurItemIndex(
                                              MMI_CTRL_ID_T ctrl_id  // IN// control id
                                              )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        return (list_ctrl_ptr->current_index);
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC MMI_STRING_T CTRLDROPDOWNLIST_GetCurItem( //return selected check item number
                                               MMI_CTRL_ID_T ctrl_id  // IN// control id
                                               )
{
    MMI_STRING_T item_str = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        item_str = GetCurItemText(list_ctrl_ptr, list_ctrl_ptr->current_index);
    }

    return item_str;
}

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC MMI_STRING_T CTRLDROPDOWNLIST_GetItem( //return selected check item number
                                            MMI_CTRL_ID_T ctrl_id,  // IN// control id
                                            uint16 item_index
                                            )
{
    return GetCurItemText(GetDropDownListPtr(ctrl_id), item_index);
}

/*****************************************************************************/
//  Description : Get DropDownList pointer from control id
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL CTRLDROPDOWNLIST_OBJ_T* GetDropDownListPtr(
                                                 MMI_HANDLE_T ctrl_handle
                                                 )
{
    CTRLBASE_OBJ_T* ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr( ctrl_handle );
    
    if (PNULL != ctrl_ptr)
    {
        if(!DropDownListTypeOf( ctrl_ptr ))
        {
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLDROPDOWNLIST_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : dropdownlist type of
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN DropDownListTypeOf(
                                 CTRLBASE_OBJ_T* ctrl_ptr
                                 )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_DROPDOWNLIST_TYPE);
}

/*****************************************************************************/
//Description : SET CURRENT INDEX 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetCurItemIndex( //return selected check item number
                                               MMI_CTRL_ID_T ctrl_id,  // IN// control id
                                               uint16        index        
                                               )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    if(PNULL != list_ctrl_ptr && index < list_ctrl_ptr->total_item_num)
    {
        list_ctrl_ptr->current_index = index;
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//Description : 设置是否显示省略点
//Global resource dependence : 
//Author:xiaoqing.lu
//Note: 返回设置之前的状态
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetEllipsis( //return selected check item number
                                           MMI_CTRL_ID_T ctrl_id,  // IN// control id
                                           BOOLEAN       is_ellipsis       
                                           )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    if (PNULL != list_ctrl_ptr)
    {
        result = list_ctrl_ptr->is_ellipsis;
        list_ctrl_ptr->is_ellipsis = is_ellipsis;
    }
    
    return result;
}

/*****************************************************************************/
//Description : 设置中间softkey和web键是否打开list，默认都为true
//Global resource dependence : 
//Author: xiaoqing.lu
//Note: 返回设置成功或失败
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetOpenListMethod(
                                                 MMI_CTRL_ID_T ctrl_id,
                                                 BOOLEAN is_mid_softkey_open_list, /*!< 是否中间softkey按下时，打开下拉框*/ 
                                                 BOOLEAN is_web_key_open_list      /*!< 是否web键按下时，打开下拉框*/ 
                                                 )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    if (PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->is_mid_softkey_open_list = is_mid_softkey_open_list;
        list_ctrl_ptr->is_web_key_open_list = is_web_key_open_list;

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//Description : 设置是否显示item 的详情，如果显示，则会显示双行列表
//Global resource dependence : 
//Author:xiaoqing.lu
//Note: 返回设置成功或失败
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetDispItemDetail(
                                                 MMI_CTRL_ID_T ctrl_id,
                                                 BOOLEAN       is_disp_detail       
                                                 )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->is_disp_detail = is_disp_detail;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set list control background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetListBg(
                                      MMI_HANDLE_T ctrl_handle,
                                      GUI_BG_T     *bg_ptr
                                      )
{
    BOOLEAN result = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_handle);

    if (PNULL != bg_ptr && PNULL != list_ctrl_ptr)
    {
        result = TRUE;
        
        list_ctrl_ptr->style_info.list_bg_info = *bg_ptr;
    }
}

/*****************************************************************************/
//  Description : set dropdown list control rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetRect(
                                    MMI_HANDLE_T    ctrl_handle,
                                    GUI_RECT_T      *rect_ptr
                                    )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_handle);

    if (PNULL != list_ctrl_ptr && PNULL != rect_ptr)
    {
        DropdownlistSetRect((CTRLBASE_OBJ_T*)list_ctrl_ptr,rect_ptr);
    }
}

/*****************************************************************************/
// 	Description : set dropdown list is visible
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetVisible(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          BOOLEAN           is_visible,
                                          BOOLEAN           is_update
                                          )
{
    BOOLEAN result = TRUE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    CTRLBASE_OBJ_T* obj_ptr = PNULL;

    if(PNULL == list_ctrl_ptr)
    {
        return FALSE;
    }
    
    obj_ptr = (CTRLBASE_OBJ_T*)list_ctrl_ptr;

    //set visible
    GUIAPICTRL_SetState(obj_ptr->handle,GUICTRL_STATE_INVISIBLE,(BOOLEAN)(!is_visible));

    if ((is_update) && (MMK_IsFocusWin(list_ctrl_ptr->win_id)))
    {
        if (is_visible)
        {
            //update display
            DrawDropDownList(obj_ptr);
        }
        else
        {
            //notify parent update bg
            GUICTRL_SendNotifyEx(obj_ptr->handle,MSG_NOTIFY_UPDATE,&obj_ptr->rect);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : send notify
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void NotifyResize(
                        CTRLBASE_OBJ_T *obj_ptr
                        )
{
    MMI_HANDLE_T handle = 0;
    
    if (PNULL != obj_ptr)
    {
        handle = MMK_GetParentWinHandle(MMK_GetWinHandleByCtrl( obj_ptr->handle ));
        if (0 == handle)
        {
            handle = MMK_GetWinHandleByCtrl( obj_ptr->handle );
        }
        
        MMK_DefNotifyProc(handle, obj_ptr->handle, MSG_NOTIFY_RESIZE, &obj_ptr->rect);    
    }
}

/*****************************************************************************/
//  Description : process lost active
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollUpList(
                                CTRLBASE_OBJ_T *obj_ptr//ctrl ptr
                                )
                                
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;
    
    if (PNULL == list_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;   
    
    if( PNULL != list_ctrl_ptr->list_handle)
    {
        MMK_DestroyControl( list_ctrl_ptr->list_handle);
        
        list_ctrl_ptr->list_handle = PNULL;
        
        //resize
        obj_ptr->rect = list_ctrl_ptr->orig_rect;
        NotifyResize(obj_ptr);    
    }
    else
    {
        DrawDropDownList(obj_ptr);
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : drop down the list
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScrollDownList(
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  BOOLEAN is_set_focus,
                                  BOOLEAN is_must_create
                                  )
{
    GUI_RECT_T              cross_rect   = {0};
    GUI_RECT_T              resize_rect  = {0};
    GUI_RECT_T              whole_display_rect = {0};
    MMI_RESULT_E            recode       = MMI_RESULT_TRUE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;
    
    if(PNULL != list_ctrl_ptr
        && obj_ptr->is_active
        && GUI_IntersectRect(&cross_rect, obj_ptr->display_rect, obj_ptr->rect))
    {
        // 通知app
        MMK_SendMsg(list_ctrl_ptr->win_id, MSG_CTL_DROPDOWNLIST_PRE_PROCESS, PNULL);

        // 创建openlist
        CreateListBox(obj_ptr, is_set_focus, is_must_create);  //init list box       

        // 重新刷屏
        NotifyResize(obj_ptr);    
    
        // 最后刷列表
        if (PNULL != list_ctrl_ptr->list_handle)
        {
            CTRLLIST_SetDisable3D(list_ctrl_ptr->list_handle, TRUE );
            
            GetWholeRect(obj_ptr, &resize_rect, &whole_display_rect);
            
            //resize 
            obj_ptr->rect = resize_rect;
            
            //load data to list box
            LoadDataToList(obj_ptr);
            
            list_ctrl_ptr->cur_status = GUIDNLIST_OPEN_STATUS;

            //draw list
            GUIAPICTRL_HandleEvent(list_ctrl_ptr->list_handle, MSG_CTL_PAINT, PNULL);
        }
    }

    return recode;
}

/*****************************************************************************/
// 	Description : select one from drop down list
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SelectFromList(
                                  CTRLBASE_OBJ_T *obj_ptr
                                  )
{
    BOOLEAN      result = FALSE;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;
    
    //get selected item
    if (PNULL != list_ctrl_ptr && PNULL != list_ctrl_ptr->list_handle)
    {
        // 设置当前item index
        list_ctrl_ptr->current_index = CTRLLIST_GetCurItemIndex(list_ctrl_ptr->list_handle);
        
        // 如果内嵌edit，则要设置字符串
        SetTextToEditCtrl(obj_ptr);

        list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;
        
        if (PNULL != list_ctrl_ptr->list_handle)
        {
            MMK_DestroyControl( list_ctrl_ptr->list_handle );
            list_ctrl_ptr->list_handle = 0;
        }
        
        //resize 
        obj_ptr->rect = list_ctrl_ptr->orig_rect;
        NotifyResize(obj_ptr);

        //send msg to parent window
        result = GUICTRL_PostNotify( obj_ptr->handle, MSG_NOTIFY_DROPDOWNLIST_SELECTED );
    }
    
    if (!result)
    {
        recode = MMI_RESULT_FALSE;
    }
    
    return recode;
}
 
/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:fen.xie
//Note:return current selected item
/*****************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetTotalItemNum( //return total items
                                              MMI_CTRL_ID_T ctrl_id  // IN// control id
                                              )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    if (PNULL != list_ctrl_ptr)
    {
        return list_ctrl_ptr->total_item_num;
    }
    
    return 0;
}

/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:fen.xie
//Note:return current selected item
/*****************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetMaxtemNum( //return total items
                                           MMI_CTRL_ID_T ctrl_id  // IN// control id
                                           )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    if (PNULL != list_ctrl_ptr)
    {
        return list_ctrl_ptr->max_item_num;
    }
    
    return 0;
}

/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:fen.xie
//Note:return current selected item
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_IsFocus( //return total items
                                       MMI_CTRL_ID_T ctrl_id  // IN// control id
                                       )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    if (PNULL != list_ctrl_ptr)
    {
        return list_ctrl_ptr->is_focus;
    }
    
    return 0;
}

/***************************************************************************//*!
@brief 设置edit 字符串
@author xiaoqing.lu
@param list_ctrl_ptr [in] 控件ID
@return void
@note 
*******************************************************************************/
LOCAL void SetTextToEditCtrl(
                             CTRLBASE_OBJ_T *obj_ptr
                             )
{
    MMI_STRING_T item_info = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if (PNULL != list_ctrl_ptr)
    {
        item_info = GetCurItemText(obj_ptr, list_ctrl_ptr->current_index);

        switch(list_ctrl_ptr->ctrl_type_info.ctrl_type)
        {
        case GUIDNLIST_EDITABLE_TEXT:
        case GUIDNLIST_EDITABLE_PHONENUM:
            GUIEDIT_SetString(list_ctrl_ptr->edit_handle, item_info.wstr_ptr, item_info.wstr_len);
            IGUICTRL_HandleEvent((IGUICTRL_T*)MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), MSG_CTL_PAINT, PNULL);
            break;

        case GUIDNLIST_EDITABLE_NUMLIST:
            {
                GUIEDIT_LIST_ITEM_T list_item_info  = {0};
                MMI_STRING_T        item_detail     = {0};

                item_detail = GetCurItemDetailText(obj_ptr, list_ctrl_ptr->current_index);

                list_item_info.is_object = TRUE;
                list_item_info.is_valid = TRUE;

                list_item_info.user_str_len = item_detail.wstr_len;
                list_item_info.user_str_ptr = item_detail.wstr_ptr;
                
                if (GUIDNLIST_VALID_VALUE_0 == GetCurItemValue(obj_ptr, list_ctrl_ptr->current_index))
                {
                    list_item_info.display_str_ptr = item_info.wstr_ptr;
                    list_item_info.display_str_len = item_info.wstr_len;
                }
                else if (GUIDNLIST_VALID_VALUE_1 == GetCurItemValue(obj_ptr, list_ctrl_ptr->current_index))
                {
                    list_item_info.display_str_ptr = item_detail.wstr_ptr;
                    list_item_info.display_str_len = item_detail.wstr_len;
                }
                else
                {
                    list_item_info.display_str_ptr = PNULL;
                    list_item_info.display_str_len = 0;
                }

                if (0 != list_item_info.display_str_len && PNULL != list_item_info.display_str_ptr)
                {
                    GUIEDIT_ReplaceCurListItem(list_ctrl_ptr->edit_handle, &list_item_info, 1);
                    
                    IGUICTRL_HandleEvent((IGUICTRL_T*)MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), MSG_CTL_PAINT, PNULL);
                }
            }
            break;

        default:
            // handle default
            //SCI_TRACE_LOW:"SetTextToEditCtrl"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_3189_112_2_18_3_15_33_110,(uint8*)"");
            break;
        }
    }
}

/*****************************************************************************/
// 	Description : drop down the list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DDLIsEditType(
                            CTRLBASE_OBJ_T *obj_ptr
                            )
{
    BOOLEAN result = FALSE;

    switch(((CTRLDROPDOWNLIST_OBJ_T *)obj_ptr)->ctrl_type_info.ctrl_type)
    {
    case GUIDNLIST_EDITABLE_TEXT:
    case GUIDNLIST_EDITABLE_NUMLIST:
    case GUIDNLIST_EDITABLE_PHONENUM:
        result = TRUE;
        break;
    default:
        result = FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : drop down the list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DDLIsSetListFocus(
                                CTRLBASE_OBJ_T *obj_ptr
                                )
{
    if (PNULL != obj_ptr)
    {	
		if (GUIDNLIST_NORMAL == ((CTRLDROPDOWNLIST_OBJ_T *)obj_ptr)->ctrl_type_info.ctrl_type)
		{
			return TRUE;
		}
    }

    
    return FALSE;
}

/***************************************************************************//*!
@brief 设置控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@param type_info_ptr [in] 初始化信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
LOCAL BOOLEAN CreateInnerEditCtrl(
                                  CTRLBASE_OBJ_T *obj_ptr,
                                  GUIEDIT_TYPE_E edit_type,
                                  uint32 text_max_len,
                                  uint32 item_max_num
                                  )
{
    BOOLEAN                 result = FALSE;
    IGUICTRL_T              *edit_ptr = PNULL;
    GUIEDIT_INIT_DATA_T     init_data = {0};
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if (PNULL != list_ctrl_ptr)
    {
        
        //set init data
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = GetDropdownlistRect(obj_ptr,GUIDNLIST_TEXT_RECT);
        init_data.type = edit_type;
        init_data.str_max_len = text_max_len;
        init_data.add_data.list.item_max_len = text_max_len;
        init_data.add_data.list.item_num = item_max_num;
        
        //set control param
        ctrl_create.ctrl_id           = 0;
        ctrl_create.parent_ctrl_handle = obj_ptr->handle;
        ctrl_create.guid              = SPRD_GUI_EDITBOX_ID;
        ctrl_create.init_data_ptr     = &init_data;
        ctrl_create.parent_win_handle = list_ctrl_ptr->win_id;
        
        //creat control
        edit_ptr = MMK_CreateControl(&ctrl_create);
        list_ctrl_ptr->edit_handle = GUICTRL_GetCtrlHandle(edit_ptr);

        if (0 != list_ctrl_ptr->edit_handle)
        {
            if (GUIEDIT_TYPE_LIST == edit_type)
            {
                GUIEDIT_SetIm(
                        list_ctrl_ptr->edit_handle,
                        GUIIM_TYPE_SET_ALL,
                        GUIIM_TYPE_ABC);
            }

            MMK_SetAtvCtrl(list_ctrl_ptr->win_id, list_ctrl_ptr->edit_handle);
            IGUICTRL_SetState(MMK_GetCtrlPtr(obj_ptr->handle), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
            IGUICTRL_SetCircularHandleUpDown(MMK_GetCtrlPtr(list_ctrl_ptr->edit_handle), FALSE);
            result = TRUE;
        }

        //SCI_TRACE_LOW:"CreateInnerEditCtrl result = %d, ctrl_handle = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_3294_112_2_18_3_15_33_111,(uint8*)"dd", result, list_ctrl_ptr->edit_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"CreateInnerEditCtrl IN PARAM PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_3298_112_2_18_3_15_33_112,(uint8*)"");
    }


    return result;
}

/***************************************************************************//*!
@brief 设置控件的类型
@author xiaoqing.lu
@param list_ctrl_ptr [in] 控件ID
@param type_info_ptr [in] 初始化信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
LOCAL BOOLEAN ChangeDDLCtrlType(
                                CTRLBASE_OBJ_T *obj_ptr,
                                const GUIDROPDOWNLIST_TYPE_T *type_info_ptr
                                )
{
    BOOLEAN result = FALSE;
    GUIDROPDOWNLIST_TYPE_E old_type = GUIDNLIST_NORMAL;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = (CTRLDROPDOWNLIST_OBJ_T *)obj_ptr;

    if (PNULL != type_info_ptr && PNULL != obj_ptr)
    {
        // 记录之前的类型
        old_type = list_ctrl_ptr->ctrl_type_info.ctrl_type;

        //SCI_TRACE_LOW:"ChangeDDLCtrlType old type = %d, new type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_3326_112_2_18_3_15_33_113,(uint8*)"dd", old_type, type_info_ptr->ctrl_type);

        if (0 != list_ctrl_ptr->edit_handle && MMK_IsHandleValid(list_ctrl_ptr->edit_handle))
        {
            MMK_DestroyControl(list_ctrl_ptr->edit_handle);
            list_ctrl_ptr->edit_handle = 0;
        }

        // 处理类型
        switch(type_info_ptr->ctrl_type)
        {
        case GUIDNLIST_NORMAL:
            result = TRUE;
            //SCI_TRACE_LOW:"ChangeDDLCtrlType need not create edit handle"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDROPDOWNLIST_3339_112_2_18_3_15_33_114,(uint8*)"");
            break;

        case GUIDNLIST_EDITABLE_TEXT:
            result = CreateInnerEditCtrl(obj_ptr, GUIEDIT_TYPE_TEXT, 
                type_info_ptr->editable_max_text_num, type_info_ptr->editable_max_listitem_num);
            break;

        case GUIDNLIST_EDITABLE_NUMLIST:
            result = CreateInnerEditCtrl(obj_ptr, GUIEDIT_TYPE_LIST, 
                type_info_ptr->editable_max_text_num, type_info_ptr->editable_max_listitem_num);
            break;

        case GUIDNLIST_EDITABLE_PHONENUM:
            result = CreateInnerEditCtrl(obj_ptr, GUIEDIT_TYPE_PHONENUM, 
                type_info_ptr->editable_max_text_num, type_info_ptr->editable_max_listitem_num);
            break;

        default:
            SCI_ASSERT(0); /*assert verified*/
            break;
        }

        // 设置
        if (result)
        {
            list_ctrl_ptr->ctrl_type_info = *type_info_ptr;
        }

        ScrollDownList(obj_ptr, DDLIsSetListFocus(obj_ptr), FALSE);
    }

    return result;
}


/***************************************************************************//*!
@brief 设置控件的边框
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param border_ptr [in] 控件边框信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetBorder(
                                         MMI_CTRL_ID_T ctrl_id,       // control id
                                         GUI_BORDER_T  *border_ptr
                                         )
{
    CTRLBASE_OBJ_T *obj_ptr = (CTRLBASE_OBJ_T *)GetDropDownListPtr(ctrl_id);

    return DropdownlistSetBorder(obj_ptr, border_ptr);
}

/***************************************************************************//*!
@brief 设置控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@param type_info_ptr [in] 初始化信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetCtrlType(
                                           MMI_HANDLE_T  win_handle,    //window id
                                           MMI_CTRL_ID_T ctrl_id,       // control id
                                           const GUIDROPDOWNLIST_TYPE_T *type_info_ptr
                                           )
{
    CTRLBASE_OBJ_T *obj_ptr = (CTRLBASE_OBJ_T *)GetDropDownListPtr(ctrl_id);

    if (PNULL != type_info_ptr && PNULL != obj_ptr)
    {
        return ChangeDDLCtrlType(obj_ptr, type_info_ptr);
    }
    else
    {
        return FALSE;
    }
}


/***************************************************************************//*!
@brief 获取控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC GUIDROPDOWNLIST_STATUS_E CTRLDROPDOWNLIST_GetCtrlStatus(
                                                               MMI_CTRL_ID_T ctrl_id
                                                               )
{
    GUIDROPDOWNLIST_STATUS_E cur_status = GUIDNLIST_CLOSE_STATUS;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        cur_status = list_ctrl_ptr->cur_status;
    }

    return cur_status;
}

/***************************************************************************//*!
@brief 
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC MMI_HANDLE_T CTRLDROPDOWNLIST_GetInlineEditHandle(
                                                        MMI_HANDLE_T  win_handle,    //window id
                                                        MMI_CTRL_ID_T ctrl_id
                                                        )
{
    MMI_HANDLE_T edit_handle = 0;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        edit_handle = list_ctrl_ptr->edit_handle;
    }

    return edit_handle;
}

/***************************************************************************//*!
@brief 获取控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetButton(
                                      MMI_HANDLE_T  win_handle,    //window id
                                      MMI_CTRL_ID_T ctrl_id,
                                      BOOLEAN is_show_button
                                      )
{
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        list_ctrl_ptr->is_has_button = is_show_button;
    }
}

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_OpenList(
                                        MMI_HANDLE_T  win_handle,    //window id
                                        MMI_CTRL_ID_T ctrl_id
                                        )
{
    BOOLEAN        recode   = TRUE;
    CTRLBASE_OBJ_T *obj_ptr = (CTRLBASE_OBJ_T *)GetDropDownListPtr(ctrl_id);
    
    recode = ScrollDownList(obj_ptr, DDLIsSetListFocus(obj_ptr), TRUE);

    return recode;
}

/*****************************************************************************/
// 	Description : close dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_CloseList(
                                         MMI_HANDLE_T  win_handle,    //window id
                                         MMI_CTRL_ID_T ctrl_id
                                         )
{
    BOOLEAN                 recode       = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);
    
    // 这里要释放内嵌的列表实例的内存,否则会内存泄露
    if (PNULL != list_ctrl_ptr)
    {
        if (PNULL != list_ctrl_ptr->list_handle)
        {
            MMK_DestroyControl( list_ctrl_ptr->list_handle );
            list_ctrl_ptr->list_handle = 0;

            NotifyResize((CTRLBASE_OBJ_T*)list_ctrl_ptr); 
        }

        list_ctrl_ptr->cur_status = GUIDNLIST_CLOSE_STATUS;
        recode = TRUE;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetPageNum(
                                          MMI_HANDLE_T  win_handle,    //window id
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint16 page_item_num
                                          )
{
    BOOLEAN                 recode       = FALSE;
    CTRLDROPDOWNLIST_OBJ_T *list_ctrl_ptr = GetDropDownListPtr(ctrl_id);

    if (PNULL != list_ctrl_ptr)
    {
        recode = TRUE;
        list_ctrl_ptr->page_item_num = page_item_num;
    }

    return recode;
}

