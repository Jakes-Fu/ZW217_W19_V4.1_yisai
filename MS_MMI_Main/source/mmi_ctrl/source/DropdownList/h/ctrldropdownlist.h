/*! @file guidropdownlist.h
@brief  此文件介绍了droplist控件的对外接口和数据类型 
@author yelianna
@version 1.0
@date 04/04/2005  
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 04/04/2005     yelianna         Create
*******************************************************************************/

/*! @page page_DropdownList 下拉框(DropdownList)

-# @ref sec_dropdownlist_function
-# @ref sec_dropdownlist_def
-# @ref sec_dropdownlist_example
-# @ref sec_dropdownlist_function_list
-# @ref sec_dropdownlist_app

@section sec_dropdownlist_function 控件功能

- Dropdownlist即下拉框控件，它将多条数据信息集合在一起，当下拉框打开时，把数据信息以列表的形式显示出来，超过一页时，下拉框显示滚动条，用户可以操作滚动条显示页面之外的数据。
- 下拉框的风格有3D风格和平面风格两种，其文本信息、背景信息等，都可以通过接口进行设置。
- 下拉框控件展开时，所显示的列表只有文本信息，即每个项都是以文本的形式显示出来的，不支持显示其它信息。
- 下拉框控件的目的是为同类信息提供一个数据显示的集合（DataSet），当下拉框收起时，所显示的即是列表中被选中的当前项；因此，下拉框是一个单项选择的控件。

@section sec_dropdownlist_def 宏定义解析

@code
#define CREATE_DROPDOWNLIST_CTRL(PAGE_ITEM_NUM, _TYPE_, _CTRL_ID_) \
        MMK_CREATE_DROPDOWNLIST, PAGE_ITEM_NUM, _TYPE_, _CTRL_ID_
@endcode
- 用于创建一个下拉框控件
    - _TYPE_ 下拉框显示的类型，可以向上或向下展开显示列表，也可以自适应
    - PAGE_ITEM_NUM 每页显示的列表项数
    - _CTRL_ID_ 控件ID，为MMI_CONTROL_ID_E枚举类型

@section sec_dropdownlist_example 控件使用示例

代码部分放在sample_test_dropdownlist.c文件中。

我们首先创建一个测试控件的窗口，窗口Table名称为MMI_TEST_DROPDOWNLIST_WIN_TAB：
@code
WINDOW_TABLE( MMI_TEST_DROPDOWNLIST_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_DROPDOWNLIST_WIN_ID ),
    WIN_FUNC((uint32)HandleTestDropdownlistWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_DROPDOWNLIST),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
CREATE_DROPDOWNLIST_CTRL(3,GUIDNLIST_SHOW_DOWNWARD, MMI_TEST_DROPDOWNLIST_CTRL_ID),
    END_WIN
};
@endcode
然后，为窗口增加处理函数：
@code
LOCAL MMI_RESULT_E HandleTestDropdownlistWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMI_TEST_DROPDOWNLIST_CTRL_ID;
    GUI_RECT_T rect =  {40, 60, 219, 85}
                       3,

                       switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_DROPDOWNLIST_ITEM_T item_arry[MMI_HELLO_DROPDOWNLIST_NUM] = {0};
            uint32 i = 0;

            //设置dropdownlist acitve
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIDROPDOWNLIST_SetRect(win_id, &rect);

            for (i = 0; i < MMI_HELLO_DROPDOWNLIST_NUM; i++)
            {
                wchar  wstr[16] = {0};
                char   str[16]  = {0};
                uint16 str_len  = 0;

                str_len = sprintf(str, "test item %d", i);

                MMI_STRNTOWSTR(wstr, 16, str, 16, str_len);

                MMI_WSTRNCPY(item_arry[i].wstr, DROPDOWNLIST_STRING_MAX_NUM, wstr, str_len, str_len);

                item_arry[i].wstr_len = str_len;
            }

            //设置item
            GUIDROPDOWNLIST_LoadItemArray(ctrl_id, item_arry, MMI_HELLO_DROPDOWNLIST_NUM);

            //设置当前选中的item
            GUIDROPDOWNLIST_SetSelectedItemId(ctrl_id, 0);
        }
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            uint16 index = 0;

            //获得当前的索引
            index = GUIDROPDOWNLIST_GetSelectedItemId(ctrl_id);
        }
        break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
@endcode

@section sec_dropdownlist_function_list 关键接口介绍

-# @ref GUIDROPDOWNLIST_AppendItemArray
-# @ref GUIDROPDOWNLIST_SetCurItemIndex
-# @ref GUIDROPDOWNLIST_GetCurItemIndex
-# @ref GUIDROPDOWNLIST_GetCurItem
-# @ref GUIDROPDOWNLIST_CreateDyn

@section sec_dropdownlist_app 控件应用

在日程创建窗口中，我们使用了下拉框来作为日程提醒类型的选择控件，如下图所示：
@image html dropdownlist_app_example.jpg 
图 2 33 控件显示例图

在类型下拉框中集合了日程提醒的类型，包括“提醒一次”、“每日提醒”、“每周提醒”、“每月提醒”、“每年提醒”、“不提醒”等内容。
*******************************************************************************/

/*! @addtogroup  dropdownlistGrp DropdownList 
@brief dropdownlist 控件
@details dropdownlist 控件
@{
*******************************************************************************/


#ifndef _GUIDROPDOWNLIST_INTERNAL_H_
#define _GUIDROPDOWNLIST_INTERNAL_H_ 

/*--------------------------------------------------------------------------
                         Include Files                                    
--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "sci_types.h"
#include "mmk_app.h"
#include "guifont.h"
#include "mmitheme_dropdownlist.h"
#include "guilcd.h"
#include "ctrldropdownlist_export.h"

/*--------------------------------------------------------------------------
                         Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*----------------------------------------------------------------------------*/
/*                         Constant Variables                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
// list type info
#define CTRL_DROPDOWNLIST_TYPE     (DROPDOWNLIST_GetType())
    
/*! @struct GUIDROPDOWNLIST_INIT_DATA_T
@brief 控件初始化信息
*/
typedef struct 
{
    CTRLBASE_INIT_PARAM_T           base_ctrl_param;

    uint16                          page_item_num; /*!< 每页显示最大数，即下拉后所能显示的数*/
    GUIDROPDOWNLIST_SHOW_TYPE_E     show_type; /*!< 打开下拉列表时显示方式*/
    
}CTRLDROPDOWNLIST_INIT_DATA_T;

/*! @struct CTRLDROPDOWNLIST_OBJ_T
@brief 控件数据结构
*/
typedef struct _dropdownlist_obj_tag
{
    CTRLBASE_OBJ_T                  base_ctrl;          /*!< 控件基类数据 */
    
    GUIDROPDOWNLIST_TYPE_T          ctrl_type_info;     /*!< 控件的类型*/
    BOOLEAN                         is_focus;           /*!< 控件是否处于Focus*/
//     BOOLEAN                         is_active;          /*!< 控件是否处于active*/
    BOOLEAN                         is_ellipsis;        /*!< 当显示不下的时候是否显示省略点"...",系统默认不显示省略点*/
    BOOLEAN                         is_has_button;      /*!< 是否有button*/
    BOOLEAN                         is_disp_detail;     /*!< 是否显示item detail*/
    GUI_RECT_T                      orig_rect;          /*!< 初始时的 rect*/
    
    uint16					        page_item_num;      /*!< 每页显示Item的条数，这个值为用户设置，或默认值*/
    uint16                          cur_page_item_num;  /*!< 当前弹出框每页显示的条数，这个值为动态计算出来的*/
    uint16					        total_item_num;     /*!< 所有ITEM的总数*/
    uint16             		        max_item_num;	    /*!< 允许输入的最大Item数*/
    uint16					        current_index;	    /*!< 当前的选中的index*/
    BOOLEAN                         is_auto_size;       /*!< 是否设置自动更新大小*/   
    MMI_STRING_T                    *all_item_ptr;	    /*!< 指向所有item的指针*/
    MMI_STRING_T                    *all_item_detail_ptr; /*!< 对应每个item的详情*/
    uint16                          *def_arry_ptr;      /*!< 该数组表示上面两个值那个是无效值，0-all_item_ptr, 1-all_item_detail_ptr*/
   
    MMI_DROPDOWNLIST_STYLE_T		style_info;         /*!< 显示风格*/
    GUIDROPDOWNLIST_SHOW_TYPE_E     show_type;          /*!< 打开下拉列表时显示方式，向上？向下？自动适应？*/
    GUIDROPDOWNLIST_STATUS_E        cur_status;	        /*!< 当前状态*/
    GUIDROPDOWNLIST_RECT_STYLE_E	rect_style;	        /*!< rect 边框显示类型*/
    GUIDROPDOWNLIST_BUTTON_STATUS_E button_status;      /*!< status of button*/ 

    BOOLEAN is_mid_softkey_open_list;                   /*!< 是否中间softkey按下时，打开下拉框*/ 
    BOOLEAN is_web_key_open_list;                       /*!< 是否web键按下时，打开下拉框*/ 
    
    MMI_HANDLE_T                    edit_handle;        /*!< edit ctrl handle*/ 
    MMI_HANDLE_T                    list_handle;        /*!< list ctrl handle*/ 
    GUILIST_STATE_T                 list_state;         /*!< 内嵌List控件的状态*/
    MMI_WIN_ID_T                    win_id;             /*!< 控件的父窗口ID*/
}CTRLDROPDOWNLIST_OBJ_T;

// dropdownlist class for public or protected method
typedef struct _dropdownlist_vtbl_tag
{
    CTRLBASE_VTBL_T             base_ctrl_vtbl;
} CTRLDROPDOWNLIST_VTBL_T;

/*--------------------------------------------------------------------------
                         PUBLIC FUNCTION DECLARE                               
--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get list type
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DROPDOWNLIST_GetType(void);

/*--------------------------------------------------------------------------
                        Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif // _GUIDROPDOWNLIST_INTERNAL_H_

/*@}*/
