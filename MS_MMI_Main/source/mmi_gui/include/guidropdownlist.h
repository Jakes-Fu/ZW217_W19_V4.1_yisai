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
            CTRLDROPDOWNLIST_SetRect(win_id, &rect);

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
            CTRLDROPDOWNLIST_LoadItemArray(ctrl_id, item_arry, MMI_HELLO_DROPDOWNLIST_NUM);

            //设置当前选中的item
            CTRLDROPDOWNLIST_SetSelectedItemId(ctrl_id, 0);
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
            index = CTRLDROPDOWNLIST_GetSelectedItemId(ctrl_id);
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

-# @ref CTRLDROPDOWNLIST_AppendItemArray
-# @ref CTRLDROPDOWNLIST_SetCurItemIndex
-# @ref CTRLDROPDOWNLIST_GetCurItemIndex
-# @ref CTRLDROPDOWNLIST_GetCurItem
-# @ref CTRLDROPDOWNLIST_CreateDyn

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


#ifndef _GUIDROPDOWNLIST_H_
#define _GUIDROPDOWNLIST_H_ 

/*--------------------------------------------------------------------------
                         Include Files                                    
--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "sci_types.h"
#include "ctrldropdownlist_export.h"


/*--------------------------------------------------------------------------
                         Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*--------------------------------------------------------------------------
                         MACRO DEFINITION                                 
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         TYPE AND STRUCT                                
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         PUBLIC FUNCTION DECLARE                               
--------------------------------------------------------------------------*/

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
#define  GUIDROPDOWNLIST_SetCtrlType CTRLDROPDOWNLIST_SetCtrlType

/***************************************************************************//*!
@brief 获取控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCtrlType CTRLDROPDOWNLIST_GetCtrlType

/***************************************************************************//*!
@brief 
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetInlineEditHandle CTRLDROPDOWNLIST_GetInlineEditHandle

/***************************************************************************//*!
@brief 获取控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetButton CTRLDROPDOWNLIST_SetButton

/***************************************************************************//*!
@brief 动态创建接口
@author yelianna
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@param init_ptr [in] 初始化信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_CreateDyn CTRLDROPDOWNLIST_CreateDyn

/***************************************************************************//*!
@brief 设置区域风格
@author yelianna
@param ctrl_id [in] 控件ID
@param style [in] 控件类型
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetRectStyle CTRLDROPDOWNLIST_SetRectStyle

/***************************************************************************//*!
@brief 设置控件内嵌list状态
@author Lianxiang.Zhou
@param ctrl_id [in] 下拉框控件ID
@param state [in] list状态
@param is_true [in] 设置or取消设置
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetListState CTRLDROPDOWNLIST_SetListState

/***************************************************************************//*!
@brief 获得item总数
@author fen.xie
@param ctrl_id [in] 控件ID
@retval 返回控件item总数
@return 返回控件item总数
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetTotalItemNum CTRLDROPDOWNLIST_GetTotalItemNum
                                                         
/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:fen.xie
//Note:return current selected item
/*****************************************************************************/
#define GUIDROPDOWNLIST_GetMaxtemNum CTRLDROPDOWNLIST_GetMaxtemNum

/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:fen.xie
//Note:return current selected item
/*****************************************************************************/
#define GUIDROPDOWNLIST_IsFocus CTRLDROPDOWNLIST_IsFocus

/***************************************************************************//*!
@brief append item
@author yelianna
@param ctrl_id [in] 控件ID
@param item_array_ptr [in] ITEM数组
@param total_num [in] 总数
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_AppendItemArray CTRLDROPDOWNLIST_AppendItemArray

/***************************************************************************//*!
@brief append item
@author jin.wang
@param ctrl_id [in] 控件ID
@param item_ptr [in] ITEM信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_AppendItem CTRLDROPDOWNLIST_AppendItem

/*****************************************************************************//*!
@brief 传入字符串,添加dropdownlist item 的详情
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_ptr [in] ITEM text string
@param item_index [in] ITEM index
@retval true/false
@return 返回成功或失败
@note 
*****************************************************************************/
#define GUIDROPDOWNLIST_AddItemDetail CTRLDROPDOWNLIST_AddItemDetail

/***************************************************************************//*!
@brief append item
@author jin.wang
@param ctrl_id [in] 控件ID
@param text_id [in] ITEM text id
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_AppendItemById CTRLDROPDOWNLIST_AppendItemById

/*****************************************************************************//*!
@brief 传入字符串,添加dropdownlist item 的详情
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_id [in] ITEM text id
@param item_index [in] ITEM index
@retval true/false
@return 返回成功或失败
@note 
*****************************************************************************/
#define GUIDROPDOWNLIST_AddItemDetailById CTRLDROPDOWNLIST_AddItemDetailById

/*****************************************************************************//*!
@brief 设置是否显示item 的详情，如果显示，则会显示双行列表
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_disp_detail [in] 是否显示？默认不显示
@retval true/false
@return 返回成功或失败
@note 
*****************************************************************************/
#define GUIDROPDOWNLIST_SetDispItemDetail CTRLDROPDOWNLIST_SetDispItemDetail

/***************************************************************************//*!
@brief set max item
@author jin.wang
@param ctrl_id [in] 控件ID
@param max_item_num [in] item num
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetMaxItem CTRLDROPDOWNLIST_SetMaxItem

/***************************************************************************//*!
@brief 获得被选中项
@author yelianna
@param ctrl_id [in] 控件ID
@retval item结构
@return 返回被选中项的数据信息
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCurItem CTRLDROPDOWNLIST_GetCurItem

/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:yelianna
//Note:return current selected item
/*****************************************************************************/
#define GUIDROPDOWNLIST_GetItem CTRLDROPDOWNLIST_GetItem

/***************************************************************************//*!
@brief 设置当前的ITEM
@author yelianna
@param ctrl_id [in]控件ID
@param index [in]控件索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetCurItemIndex CTRLDROPDOWNLIST_SetCurItemIndex


/***************************************************************************//*!
@brief 获得当前item索引号
@author yelianna
@param ctrl_id [in]控件ID
@retval uint16
@return 返回当前索引号
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCurItemIndex CTRLDROPDOWNLIST_GetCurItemIndex


/***************************************************************************//*!
@brief 设置是否显示省略点
@author yelianna
@param ctrl_id [in]控件ID
@param is_ellipsis [in]是否显示省略号
@retval true/false
@return 返回成功或失败
@note 当控件item的文本超过控件区域时，是否使用省略号
*******************************************************************************/
#define GUIDROPDOWNLIST_SetEllipsis CTRLDROPDOWNLIST_SetEllipsis

/***************************************************************************//*!
@brief 设置控件区域
@author Jassmine
@param ctrl_handle [in]控件句柄
@param rect_ptr 控[in]件区域
@retval void
@return void
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetRect CTRLDROPDOWNLIST_SetRect

/*****************************************************************************/
//  Description : set list control background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetListBg CTRLDROPDOWNLIST_SetListBg

/***************************************************************************//*!
@brief 设置控件是否可见
@author Jassmine
@param ctrl_id    [in] 控件id
@param is_visible [in] 是否可见
@param is_update  [in] 是否立即刷新
@return 是否操作成功
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetVisible CTRLDROPDOWNLIST_SetVisible

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_OpenList CTRLDROPDOWNLIST_OpenList

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetPageNum CTRLDROPDOWNLIST_SetPageNum

/***************************************************************************//*!
@brief 设置控件的边框
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param border_ptr [in] 控件边框信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_SetBorder CTRLDROPDOWNLIST_SetBorder

/*****************************************************************************/
//  Description : 传入字符串,添加dropdownlist
//  Global resource dependence : 
//  Author: jin.wang
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
#define GUIDROPDOWNLIST_AppendItemExt CTRLDROPDOWNLIST_AppendItemExt

/*****************************************************************************/
// 	Description : close dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_CloseList CTRLDROPDOWNLIST_CloseList

/*****************************************************************************/
//  Description : 设置有效值
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetValidItemValue CTRLDROPDOWNLIST_SetValidItemValue

/*****************************************************************************/
//Description : 设置中间softkey和web键是否打开list，默认都为true
//Global resource dependence : 
//Author: xiaoqing.lu
//Note: 返回设置成功或失败
/*****************************************************************************/
#define GUIDROPDOWNLIST_SetOpenListMethod CTRLDROPDOWNLIST_SetOpenListMethod

/***************************************************************************//*!
@brief 获取控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval 
@return 
@note 
*******************************************************************************/
#define GUIDROPDOWNLIST_GetCtrlStatus CTRLDROPDOWNLIST_GetCtrlStatus

/*--------------------------------------------------------------------------
                        Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

/*@}*/


