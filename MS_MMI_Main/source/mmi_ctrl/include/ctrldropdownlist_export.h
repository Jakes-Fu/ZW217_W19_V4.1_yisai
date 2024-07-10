/*! @file ctrldropdownlist_export.h
@brief  此文件介绍了droplist控件的对外接口和数据类型 
@author xiaoqing.lu
@version 1.0
@date 25/07/2012  
@remarks Copyright: 2012 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 25/07/2012     xiaoqing.lu         Create
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


#ifndef _CTRLDROPDOWNLIST_H_
#define _CTRLDROPDOWNLIST_H_ 

/*--------------------------------------------------------------------------
                         Include Files                                    
--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "sci_types.h"
#include "mmk_app.h"
#include "mmitheme_dropdownlist.h"
#include "ctrllist_export.h"

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
typedef struct _dropdownlist_obj_tag* CTRL_DROPDOWNLIST_HANDLE_T;

#define DROPDOWNLIST_STRING_MAX_NUM  56  //max string length of item 

/*--------------------------------------------------------------------------
                         TYPE AND STRUCT                                
--------------------------------------------------------------------------*/
typedef enum
{
    GUIDNLIST_VALID_VALUE_0,
    GUIDNLIST_VALID_VALUE_1,
    GUIDNLIST_VALID_VALUE_NONE,
    GUIDNLIST_VALID_VALUE_MAX
}GUIDROPDOWNLIST_VALID_VALUE_E;

/*! @enum GUIDROPDOWNLIST_RECT_STYLE_E
@brief DropDownList的边框风格
*/
typedef enum
{ 
    GUIDNLIST_NONE_TYPE,/*!< none*/
    GUIDNLIST_RECT_SOLID, /*!< 3D效果*/
    GUIDNLIST_RECT_PLANE  /*!< 平面效果*/
}GUIDROPDOWNLIST_RECT_STYLE_E;


/*! @enum GUIDROPDOWNLIST_POSITION_E
@brief 触笔操作控件时，点击的区域位置
*/
typedef enum
{
    GUIDNLIST_TEXT_RECT = 0,/*!< 文本区域*/
    GUIDNLIST_BUTTON_RECT,  /*!< 按钮区域*/
    GUIDNLIST_LIST_RECT,		/*!< 下拉框区域*/
    GUIDNLIST_OTHER
}GUIDROPDOWNLIST_POSITION_E;


/*! @enum GUIDROPDOWNLIST_STATUS_E
@brief 操作控件时的状态
*/
typedef enum
{ 
    GUIDNLIST_OPEN_STATUS = 0,	/*!< 下拉框处于打开状态*/
    GUIDNLIST_CLOSE_STATUS,		/*!< 下拉框处于未打开状态*/
    GUIDNLIST_ERROR_STATUS /*!< 错误状态*/
}GUIDROPDOWNLIST_STATUS_E;


/*! @enum GUIDROPDOWNLIST_BUTTON_STATUS_E
@brief 控件按钮的状态
*/
typedef enum
{
    GUIDNLIST_BUTTON_PRESSED = 0,/*!< 按下状态*/
    GUIDNLIST_BUTTON_RELEASED,	/*!< 弹起状态*/
    GUIDNLIST_BUTTON_ERROR /*!< 错误状态*/
}GUIDROPDOWNLIST_BUTTON_STATUS_E;


/*! @enum GUIDROPDOWNLIST_SHOW_TYPE_E
@brief 打开下拉列表时显示方式
*/
typedef enum
{
    GUIDNLIST_SHOW_DOWNWARD = 0,/*!< 向下打开列表框*/
    GUIDNLIST_SHOW_UPWARD,	    /*!< 向上打开列表框*/
    GUIDNLIST_SHOW_AUTOSET, /*!< 根据控件的位置自适应*/
    GUIDNLIST_SHOW_MAX
}GUIDROPDOWNLIST_SHOW_TYPE_E;

/*! @enum GUIDROPDOWNLIST_TYPE_E
@brief 控件打开时的方式
*/
typedef enum
{
    GUIDNLIST_NORMAL = 0,           /*!< 一般的下拉框*/
    GUIDNLIST_EDITABLE_TEXT,	    /*!< 可编辑的下拉框(Text)*/
    GUIDNLIST_EDITABLE_NUMLIST,	    /*!< 可编辑的下拉框(number list)*/
    GUIDNLIST_EDITABLE_PHONENUM,	/*!< 可编辑的下拉框(phone number)*/

    GUIDNLIST_STYLE_MAX
}GUIDROPDOWNLIST_TYPE_E;

/*! @struct GUIDROPDOWNLIST_TYPE_T
@brief 设置控件的类型
*/
typedef struct 
{
    GUIDROPDOWNLIST_TYPE_E     ctrl_type; /*!< 设置控件的类型*/

    uint32 editable_max_text_num;
    uint32 editable_max_listitem_num;

}GUIDROPDOWNLIST_TYPE_T;

/*! @struct GUIDROPDOWNLIST_INIT_DATA_T
@brief 控件初始化信息
*/
typedef struct 
{
    GUI_BOTH_RECT_T                 both_rect;/*!< 控件的横竖屏区域*/
    uint16                          page_item_num; /*!< 每页显示最大数，即下拉后所能显示的数*/
    GUIDROPDOWNLIST_SHOW_TYPE_E     show_type; /*!< 打开下拉列表时显示方式*/
    
}GUIDROPDOWNLIST_INIT_DATA_T;

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
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetCtrlType(
                                           MMI_HANDLE_T  win_handle,    //window id
                                           MMI_CTRL_ID_T ctrl_id,       // control id
                                           const GUIDROPDOWNLIST_TYPE_T *type_info_ptr
                                           );

/***************************************************************************//*!
@brief 获取控件的类型
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC GUIDROPDOWNLIST_TYPE_T CTRLDROPDOWNLIST_GetCtrlType(
                                                          MMI_HANDLE_T  win_handle,    //window id
                                                          MMI_CTRL_ID_T ctrl_id
                                                          );

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
                                                        );

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
                                      );

/***************************************************************************//*!
@brief 动态创建接口
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param win_handle [in] 控件的父窗口ID或句柄
@param init_ptr [in] 初始化信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_CreateDyn(
                                         MMI_CTRL_ID_T ctrl_id, // control id
                                         MMI_HANDLE_T  win_handle,//window id
                                         GUIDROPDOWNLIST_INIT_DATA_T *init_ptr
                                         );

/***************************************************************************//*!
@brief 设置区域风格
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param style [in] 控件类型
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetRectStyle( 
                                            MMI_CTRL_ID_T                ctrl_id, // control id
                                            GUIDROPDOWNLIST_RECT_STYLE_E style
                                            );

/***************************************************************************//*!
@brief 设置控件内嵌list状态
@author xiaoqing.lu
@param ctrl_id [in] 下拉框控件ID
@param state [in] list状态
@param is_true [in] 设置or取消设置
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetListState(
                                            MMI_CTRL_ID_T  ctrl_id,
                                            GUILIST_STATE_T  state,
                                            BOOLEAN is_true
                                            );
                                            

/***************************************************************************//*!
@brief 获得item总数
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval 返回控件item总数
@return 返回控件item总数
@note 
*******************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetTotalItemNum(
                                              MMI_CTRL_ID_T ctrl_id
                                              );
                                                         
/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetMaxtemNum( //return total items
                                           MMI_CTRL_ID_T ctrl_id  // IN// control id
                                           );

/*****************************************************************************/
//Description : Get Total items 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_IsFocus( //return total items
                                       MMI_CTRL_ID_T ctrl_id  // IN// control id
                                       );

/***************************************************************************//*!
@brief append item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_array_ptr [in] ITEM数组
@param total_num [in] 总数
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemArray( 
                                               MMI_CTRL_ID_T ctrl_id, // control id
                                               MMI_STRING_T *item_array_ptr,//load item array
                                               uint16 array_size //item array total num
                                               );

/***************************************************************************//*!
@brief append item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_ptr [in] ITEM信息
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItem( 
                                          MMI_CTRL_ID_T ctrl_id, // control id
                                          const MMI_STRING_T *item_ptr //item info
                                          );

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
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AddItemDetail( 
                                             MMI_CTRL_ID_T ctrl_id, // control id
                                             uint16 item_index,
                                             const MMI_STRING_T *item_ptr //item info
                                             );

/***************************************************************************//*!
@brief append item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_id [in] ITEM text id
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemById( 
                                              MMI_CTRL_ID_T ctrl_id, // control id
                                              MMI_TEXT_ID_T text_id //text id
                                              );

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
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AddItemDetailById( 
                                                 MMI_CTRL_ID_T ctrl_id, // control id
                                                 uint16 item_index,
                                                 MMI_TEXT_ID_T text_id //item info
                                                 );

/*****************************************************************************//*!
@brief 设置是否显示item 的详情，如果显示，则会显示双行列表
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_disp_detail [in] 是否显示？默认不显示
@retval true/false
@return 返回成功或失败
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetDispItemDetail(
                                                 MMI_CTRL_ID_T ctrl_id,
                                                 BOOLEAN       is_disp_detail       
                                                 );

/***************************************************************************//*!
@brief set max item
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param max_item_num [in] item num
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetMaxItem(
                                          MMI_CTRL_ID_T ctrl_id, // control id
                                          uint16 max_item_num
                                          );

/***************************************************************************//*!
@brief 获得被选中项
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval item结构
@return 返回被选中项的数据信息
@note 
*******************************************************************************/
PUBLIC MMI_STRING_T CTRLDROPDOWNLIST_GetCurItem( //return selected check item number
                                               MMI_CTRL_ID_T ctrl_id  // IN// control id
                                               );


/*****************************************************************************/
//Description : Get selected item 
//Global resource dependence : 
//Author:xiaoqing.lu
//Note:return current selected item
/*****************************************************************************/
PUBLIC MMI_STRING_T CTRLDROPDOWNLIST_GetItem( //return selected check item number
                                            MMI_CTRL_ID_T ctrl_id,  // IN// control id
                                            uint16 item_index
                                            );

/***************************************************************************//*!
@brief 设置当前的ITEM
@author xiaoqing.lu
@param ctrl_id [in]控件ID
@param index [in]控件索引号
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetCurItemIndex(
                                               MMI_CTRL_ID_T ctrl_id,
                                               uint16        index        
                                               );


/***************************************************************************//*!
@brief 获得当前item索引号
@author xiaoqing.lu
@param ctrl_id [in]控件ID
@retval uint16
@return 返回当前索引号
@note 
*******************************************************************************/
PUBLIC uint16 CTRLDROPDOWNLIST_GetCurItemIndex( 
                                              MMI_CTRL_ID_T ctrl_id  // IN// control id
                                              );


/***************************************************************************//*!
@brief 设置是否显示省略点
@author xiaoqing.lu
@param ctrl_id [in]控件ID
@param is_ellipsis [in]是否显示省略号
@retval true/false
@return 返回成功或失败
@note 当控件item的文本超过控件区域时，是否使用省略号
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetEllipsis(
                                           MMI_CTRL_ID_T ctrl_id,
                                           BOOLEAN       is_ellipsis       
                                           );


/***************************************************************************//*!
@brief 设置控件区域
@author xiaoqing.lu
@param ctrl_handle [in]控件句柄
@param rect_ptr 控[in]件区域
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetRect(
                                    MMI_HANDLE_T    ctrl_handle,
                                    GUI_RECT_T      *rect_ptr
                                    );

/*****************************************************************************/
//  Description : set list control background
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDROPDOWNLIST_SetListBg(
                                      MMI_HANDLE_T    ctrl_handle,
                                      GUI_BG_T      *bg_ptr
                                      );

/***************************************************************************//*!
@brief 设置控件是否可见
@author xiaoqing.lu
@param ctrl_id    [in] 控件id
@param is_visible [in] 是否可见
@param is_update  [in] 是否立即刷新
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_SetVisible(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          BOOLEAN           is_visible,
                                          BOOLEAN           is_update
                                          );

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_OpenList(
                                        MMI_HANDLE_T  win_handle,    //window id
                                        MMI_CTRL_ID_T ctrl_id
                                        );

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
                                          );

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
                                         );

/*****************************************************************************/
//  Description : 传入字符串,添加dropdownlist
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:添加成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_AppendItemExt( 
                                             MMI_CTRL_ID_T ctrl_id, // control id
                                             const MMI_STRING_T *item_ptr, //item info
                                             const MMI_STRING_T *item_detail_ptr //item info
                                             );

/*****************************************************************************/
// 	Description : close dropdown list
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLDROPDOWNLIST_CloseList(
                                         MMI_HANDLE_T  win_handle,    //window id
                                         MMI_CTRL_ID_T ctrl_id
                                         );

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
                                                 );

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
                                                 );

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
                                                               );

/*--------------------------------------------------------------------------
                        Compiler Flag                                    
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif // _CTRLDROPDOWNLIST_H_

/*@}*/


