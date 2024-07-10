/*! @file guilistbox.h
@brief 此文件介绍了listbox控件的对外接口和数据类型
@author xiaoqing.lu
@version 1.0
@date 03/23/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note
-# 03/2004        xiaoqing.lu      Create
-# 02/12/2006     Xiaoqing.Lu      Modify
*******************************************************************************/

#ifndef  _GUILISTBOX_H_
#define  _GUILISTBOX_H_

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "ctrllist_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/*----------------------------------------------------------------------------*/
/*                         Constant Variables                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Macro Declaration                                  */
/*----------------------------------------------------------------------------*/
/*! @def GUILIST_STRING_MAX_NUM
@brief 定义最大字符个数，该宏在list控件中并没有使用，但被app引用较多，用来定义字符串数组大小
*/
#define GUILIST_STRING_MAX_NUM  60



/*! @struct GUILIST_ITEM_T
@brief 控件ITEM数据
@note 第一个split之前的是第0组，之后的是第1组，第一个split本身是第1组
*/
typedef struct
{
    GUIITEM_STYLE_E              item_style;    /*!< item类型*/
#ifdef _SW_ZDT_PRODUCT_
    GUIITEM_STYLE_E              left_item_style;  /*!<LeftFocus item类型*/
    GUIITEM_STYLE_E              right_item_style; /*!<RightFocus item类型*/
#endif
    GUIITEM_STATE_T              item_state;    /*!< item状态*/
    GUILIST_ITEM_DATA_T         *item_data_ptr; /*!< item数据*/

    uint16                       first_letter;  /*!< 索引字母*/
    uint16                       item_group;    /*!< item的归类，同一个分组为一类，背景相同，从1开始，0表示不使用背景不需要分组*/
    uint16                       group_idx;     /*!< item的组索引，组即split自然分割形成的 */

    uint32                       user_data;     /*!< 用户自定义数据*/
} GUILIST_ITEM_T;




/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author xiaoqing.lu
@param ctrl_id  [in]            控件ID
@param max_item_num [in]    控件项最大数
@param is_dynamic_item_data [in]    是否动态数据
@retval true/false
@return 返回成功或失败
@note 当为动态数据时，控件显示每个item都都会向窗口要数据
*******************************************************************************/
#define  GUILIST_SetMaxItem CTRLLIST_SetMaxItem

/***************************************************************************//*!
@brief 设置最大选中的ITEM数
@author xiaoqing.lu
@param ctrl_id  [in]     控件ID
@param mark_cnt_max  [in]    最大选择项
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetMaxSelectedItem CTRLLIST_SetMaxSelectedItem

/***************************************************************************//*!
@brief 设置控件title显示风格
@author xiaoqing.lu
@param ctrl_id  [in]     控件ID
@param title_style [in] 风格
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetTitleStyle CTRLLIST_SetTitleStyle

/***************************************************************************//*!
@brief Append控件项
@author xiaoqing.lu
@param ctrl_id  [in]        控件ID
@param item_ptr Item [in]   数据指针
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_AppendItem(
    MMI_CTRL_ID_T         ctrl_id, // control id
    const GUILIST_ITEM_T *item_ptr    // 指定的Item
);


/***************************************************************************//*!
@brief Insert控件项
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param item_ptr [in]    数据指针
@param pos  [in]    制定Insert位置
@retval true/false
@return 返回成功或失败
@note pos小于最大项
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_InsertItem(
    MMI_CTRL_ID_T      ctrl_id,       // control id
    const GUILIST_ITEM_T *item_ptr,   // appointed Item
    uint16              pos               // appointed Position, from 0 to total_item_num
);

/***************************************************************************//*!
@brief Remove一个Item
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param pos  [in]    制定被删除的项的索引号
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_RemoveItem CTRLLIST_RemoveItem


/***************************************************************************//*!
@brief Remove所有Item项
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_RemoveAllItems CTRLLIST_RemoveAllItems

/***************************************************************************//*!
@brief Replace一个Item
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param replace_item_ptr [in] 用于替换的数据
@param pos  [in]    制定被替换的item索引
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_ReplaceItem(
    MMI_CTRL_ID_T        ctrl_id,            // control id
    const GUILIST_ITEM_T  *replace_item_ptr, // appointed Item
    uint16            pos                    //  appointed Position, from 0 to total_item_num-1
);


/***************************************************************************//*!
@brief 设置控件项的详细数据
@author yelianna
@param ctrl_id  [in]    控件ID
@param item_data_ptr [in]   用于设置的数据
@param pos  [in]    制定被设置的item索引
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetItemData CTRLLIST_SetItemData

/***************************************************************************//*!
@brief 设置Item数据内容
@author yelianna
@param ctrl_id  [in]    控件ID
@param item_content_ptr  [in]   用于设置的数据
@param item_index   [in]    制定被设置的item索引
@param item_content_index [in]  数据内容的坐标（item_content_ptr为数组）
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetItemContent CTRLLIST_SetItemContent

/***************************************************************************//*!
@brief 设置Item数据内容
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param item_content_ptr  [in]   用于设置的数据
@param item_index   [in]    制定被设置的item索引
@param item_content_index [in]  数据内容的坐标（item_content_ptr为数组）
@param is_syn_decode    [in]    是否异步
@param is_update [in]   是否立即刷新
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetItemContentExt CTRLLIST_SetItemContentExt

/***************************************************************************//*!
@brief 获得被选中的项的索引
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param idx_ptr [in/out] 用于存放所有索引的数组，可以为空
@param array_len    [in]    数组大小
@retval uint16类型
@return 返回被选中的总项数
@note 用户如果不确定传入的数组到底应该多大，那么可首先调用该接口获得总数，然后再分配内存获取具体被选中项的索引号
*******************************************************************************/
#define GUILIST_GetSelectedItemIndex CTRLLIST_GetSelectedItemIndex


/***************************************************************************//*!
@brief 设置某一项为选中项
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param item_pos  [in]   被设置的索引
@param is_selected  [in]    TRUE : selected; FALSE: unselected
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetSelectedItem CTRLLIST_SetSelectedItem

/***************************************************************************//*!
@brief 获得一个Item指针
@author James.Zhang
@param ctrl_id  [in]    控件ID
@param index  [in]  索引
@retval 指针
@return 返回某一项的指针
@note
*******************************************************************************/
PUBLIC const GUILIST_ITEM_T *GUILIST_GetItemPtrByIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
);


/***************************************************************************//*!
@brief 获得当前高亮的Item索引
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@retval uint16
@return 返回Item索引
@note
*******************************************************************************/
#define GUILIST_GetCurItemIndex CTRLLIST_GetCurItemIndex

/***************************************************************************//*!
@brief 设置当前高亮的索引
@author  xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param current_index    [in]    被设置的索引号
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetCurItemIndex CTRLLIST_SetCurItemIndex

/***************************************************************************//*!
@brief 改变显示类型
@author Guanyong.Zhang
@param ctrl_id  [in]    控件ID
@param type [in]    显示类型
@param is_flash [in]    是否立即刷新
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_ChangeDisplayType CTRLLIST_ChangeDisplayType


/***************************************************************************//*!
@brief 获得一页显示的项数
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@retval uint16
@return 返回项数
@note
*******************************************************************************/
#define GUILIST_GetPageItemNum CTRLLIST_GetPageItemNum


/***************************************************************************//*!
@brief 获取顶端Item索引
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@retval uint16
@return 返回索引号
@note
*******************************************************************************/
#define GUILIST_GetTopItemIndex CTRLLIST_GetTopItemIndex

/***************************************************************************//*!
@brief 设置顶端的Item
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param top_index    [in]    被设置的Item
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetTopItemIndex CTRLLIST_SetTopItemIndex

/***************************************************************************//*!
@brief I当前Item是否有效
@author Lianxiang
@param ctrl_id  [in]    控件ID
@retval true/false
@return 返回成功或失败
@note 无效的Item将灰化显示，但会执行按键及触笔操作
*******************************************************************************/
#define GUILIST_IsCurrItemInvalid CTRLLIST_IsCurrItemInvalid

/***************************************************************************//*!
@brief 是否显示Title信息
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param title_index_type [in]    Title信息显示的类型
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetTitleIndexType CTRLLIST_SetTitleIndexType

/***************************************************************************//*!
@brief 是否需要显示滚动条
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param is_need_prgbar   [in]    TRUE：显示；FALSE：不显示
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetNeedPrgbar CTRLLIST_SetNeedPrgbar

#ifdef ADULT_WATCH_SUPPORT
/***************************************************************************//*!
@brief set progressbar theme
@author fulu.song
@param ctrl_id	[in]	控件ID
@param theme 	[in]	theme to apply
@retval void
@return void
@note 
*******************************************************************************/
#define GUILIST_SetPrgbarTheme CTRLLIST_SetPrgbarTheme
#endif
/***************************************************************************//*!
@brief 获得所有Item数
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval uint16
@return 返回总项数
@note
*******************************************************************************/
#define GUILIST_GetTotalItemNum CTRLLIST_GetTotalItemNum

/***************************************************************************//*!
@brief 设置所有项被选中
@author hanbing.zhang
@param ctrl_id  [in]    控件ID
@param is_select_all    [in]    TRUE：所有被选中，FALSE：取消选中所有
@retval void
@return void
@note 如果不能选中所有，那么将根据最大数从第一个开始选中
*******************************************************************************/
#define GUILIST_SetAllSelected CTRLLIST_SetAllSelected

/***************************************************************************//*!
@brief  设置控件是否可以标记
@author hanbing.zhang
@param ctrl_id  [in]    控件ID
@param is_enable_mark   [in]    TRUE：可以被标记，FALSE：不能被标记
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetTextListMarkable CTRLLIST_SetTextListMarkable

/***************************************************************************//*!
@brief  设置控件Item前的数字的起始值
@author James.Zhang
@param ctrl_id  [in]    控件ID
@param num_start_index  [in]    起始值
@retval void
@return void
@note 系统只保留1-20个数字的图片资源ID供list控件使用
*******************************************************************************/
#define GUILIST_SetNumberListStartIndex CTRLLIST_SetNumberListStartIndex

/***************************************************************************//*!
@brief 设置Softkey是否有应用控制显示
@author hanbing.zhang
@param ctrl_id  [in]    控件ID
@param is_own_softkey   [in]    TRUE:窗口自定义softkey，FALSE:使用控件softkey
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetOwnSofterKey CTRLLIST_SetOwnSofterKey

/*****************************************************************************/
//  Description : 设置控件是否需要分割线
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_SetNeedSplidLine CTRLLIST_SetNeedSplidLine

/*****************************************************************************//*!
@brief Set the item state
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param item_index [in]      item 索引
@param src_state [in]       要设置的状态
@retval true/false
@return TRUE：设置成功或者失败
@note
*****************************************************************************/
#define GUILIST_SetItemStateById CTRLLIST_SetItemStateById

/***************************************************************************//*!
@brief 设置控件的状态
@author James.Zhang
@param list_ptr [in]    控件指针
@param src_state    [in]    要设置的状态
@param is_true  [in]     设置/取消设置
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetListState CTRLLIST_SetListState

/***************************************************************************//*!
@brief 获得控件的某种状态是否被设置
@author James.Zhang
@param list_ptr [in]    控件指针
@param src_state    [in]    要获得的状态
@retval true/false
@return TRUE：控件具有该状态，FALSE：控件没有该状态
@note
*******************************************************************************/
#define GUILIST_GetListState CTRLLIST_GetListState

/***************************************************************************//*!
@brief 获得控件的某种状态是否被设置
@author James.Zhang
@param ctrl_id  [in]    控件ID
@param src_state [in]       要获得的状态
@retval true/false
@return TRUE：控件具有该状态，FALSE：控件没有该状态
@note
*******************************************************************************/
#define GUILIST_GetListStateById CTRLLIST_GetListStateById

/***************************************************************************//*!
@brief  获得控件项的用户自定义数据
@author peng.chen
@param iguictrl_ptr [in]    控件指针
@param index     [in]   要获得的Item索引
@param user_data_ptr    [in]    用户数据，可以为任何类型，用户自定义
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_GetItemData CTRLLIST_GetItemData

/***************************************************************************//*!
@brief  设置控件项的用户自定义数据
@author peng.chen
@param iguictrl_ptr [in]    控件指针
@param index     [in]   要获得的Item索引
@param user_data_ptr    [in]    用户数据，可以为任何类型，用户自定义
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetItemUserData CTRLLIST_SetItemUserData

/***************************************************************************//*!
@brief 矫正当前Item索引
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@retval void
@return void
@note 当删除某一个项时，可能存在当前Item设置错误的情况，通过该接口可以矫正
*******************************************************************************/
#define GUILIST_ValidCurItemPos CTRLLIST_ValidCurItemPos

/***************************************************************************//*!
@brief 设置控件的背景色
@author James.Zhang
@param ctrl_id [in] 控件ID
@param back_color [in]  背景色
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetBgColor CTRLLIST_SetBgColor

/***************************************************************************//*!
@brief 设置List控件的默认图片ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param def_img_id [in]  解码过程中的默认图片
@param err_img_id [in]  解码失败后的默认图片
@retval true/false
@return 返回成功或失败
@note 两个默认图片都是为动态加载图片时，内嵌的动画控件所用
*******************************************************************************/
#define GUILIST_SetDefaultIcon CTRLLIST_SetDefaultIcon
#define GUILIST_SetDefaultIconEx CTRLLIST_SetDefaultIconEx
/***************************************************************************//*!
@brief 设置Item是否显示薄雾效果
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in] 控件索引
@param is_misted [in] 是否有薄雾效果
@retval true/false
@return 返回设置之前的状态
@note
*******************************************************************************/
#define GUILIST_SetItemMisted CTRLLIST_SetItemMisted


/***************************************************************************//*!
@brief 设置是否需要显示高亮条
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_true [in] TRUE要显示高亮条，FALSE不要显示。默认为TRUE
@note
*******************************************************************************/
#define GUILIST_SetNeedHiLightBar CTRLLIST_SetNeedHiLightBar


/***************************************************************************//*!
@brief  设置空列表信息
@author James.Zhang
@param ctrl_id [in] 控件ID
@param empty_ptr [in] 空列表信息
@retval void
@return void
@note 请在窗口open消息时设置
*******************************************************************************/
#define GUILIST_SetEmptyInfo CTRLLIST_SetEmptyInfo

/***************************************************************************//*!
@brief 设置是否显示滚动条的滑块
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_true [in] TRUE显示，FALSE不显示。默认为TRUE
@retval void
@return void
@note 这个接口设置是否需要滚动条上的滑块
*******************************************************************************/
#define GUILIST_SetNeedPrgbarBlock CTRLLIST_SetNeedPrgbarBlock


/***************************************************************************//*!
@brief 设置背景图片资源ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param image_id [in] 图片资源ID
@param is_transparent [in] 是否透明显示
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetBgImage CTRLLIST_SetBgImage

/*****************************************************************************/
//  Description : set the user bg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_SetUserBg CTRLLIST_SetUserBg

/***************************************************************************//*!
@brief 设置显示动画的时候是否同步解码，默认是异步解码
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_sync_decode [in] TRUE：同步，FALSE：异步
@retval true/false
@return 返回设置之前的状态
@note 返回设置之前的状态
*******************************************************************************/
#define GUILIST_SetSyncDecodeAnim CTRLLIST_SetSyncDecodeAnim

/*****************************************************************************//*!
@brief list上的Item动画是否需要缩放，默认情况下是不缩放的
@author fangfang.yao
@param ctrl_id [in] 控件ID
@param is_zoom [in] TRUE：缩放，FALSE：不缩放
@retval TRUE/FALSE
@return 操作是否成功
@note 操作是否成功
/*****************************************************************************/
#define GUILIST_SetZoomAnim CTRLLIST_SetZoomAnim

/***************************************************************************//*!
@brief 设置是否循环滚动
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_cyc_scroll [in] TRUE：循环，FALSE：不循环
@note 默认时循环滚动，当控件不循环滚动，焦点在最上/最下时，会发送消息给父窗口
*******************************************************************************/
#define GUILIST_SetCycScroll CTRLLIST_SetCycScroll

/***************************************************************************//*!
@brief 设置是否响应左右按键
@author James.Zhang
@param ctrl_id [in] 控件ID
@param is_handle [in] TRUE：响应，FALSE：不响应
@note 默认不响应左右按键
*******************************************************************************/
#define GUILIST_SetHandleLeftRight CTRLLIST_SetHandleLeftRight

/***************************************************************************//*!
@brief 设置是否允许显示边框
@author Jassmine
@param ctrl_id      [in] 控件ID
@param is_permit    [in] 是否允许显示
@return 操作是否成功
@note
*******************************************************************************/
#define GUILIST_PermitBorder CTRLLIST_PermitBorder

/***************************************************************************//*!
@brief 设置控件的字体和颜色
@author James.Zhang
@param ctrl_id [in] 控件ID
@param font [in] 字体
@param font_color [in] 字体颜色
@retval true/false
@return 返回成功或失败
@note  默认不需要设置.与GUILIST_STATE_TEXT_STYLE状态同时使用
*******************************************************************************/
#define GUILIST_SetTextFont CTRLLIST_SetTextFont

/*****************************************************************************//*!
@brief 设置list text font color
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@param font_color [in]      item 字体颜色
@retval true/false
@return TRUE：设置成功或者失败
@note 默认不需要设置, 与GUIITEM_STATE_COSTMER_FONT_COLOR状态同时使用
*****************************************************************************/
#define GUILIST_SetItemTextFontColor CTRLLIST_SetItemTextFontColor

/***************************************************************************//*!
@brief 设置控件区域
@author Jassmine
@param ctrl_handle [in] 控件HANDLE
@param rect_ptr [in] 区域
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetRect CTRLLIST_SetRect

/***************************************************************************//*!
@brief 设置控件是否支持*,\#切换页
@author Jassmine
@param ctrl_handle [in] 控件HANDLE
@param is_handle [in] TRUE:支持，FALSE:不支持
@retval true/false
@return 返回成功或失败
@note 默认支持
*******************************************************************************/
#define GUILIST_SetHandleStarHash CTRLLIST_SetHandleStarHash

/***************************************************************************//*!
@brief 设置是否支持slide状态
@author Xiaoqing.Lu
@param ctrl_id [in] 控件HANDLE
@param is_slide_used [in] TRUE:支持，FALSE:不支持
@retval true/false
@return 返回成功或失败
@note  默认情况，我们支持slide状态
*******************************************************************************/
#define GUILIST_SetSlideState CTRLLIST_SetSlideState

/***************************************************************************//*!
@brief 获取是否支持slide状态
@author Xiaoming.Ren
@param ctrl_id [in] 控件HANDLE
@retval true/false
*******************************************************************************/
#define GUILIST_GetSlideState CTRLLIST_GetSlideState

/***************************************************************************//*!
@brief 设置顶端Item的偏移
@author bin.ji
@param ctrl_id [in] 控件ID
@param offset_y [in] Y方向的偏移
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetTopItemOffset CTRLLIST_SetTopItemOffset

/***************************************************************************//*!
@brief get top item offset
@author xiaoqinglu
@param ctrl_id [in] 控件ID
@param offset_y_ptr [in/out] Y方向的偏移
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_GetTopItemOffset CTRLLIST_GetTopItemOffset

/***************************************************************************//*!
@brief 设置是否支持3d
@author James.Zhang
@param ctrl_id [in] 控件ID
@param is_disable_3d [in] TRUE支持，FALSE不支持
@retval true/false
@return 返回成功或失败
@note 默认支持
*******************************************************************************/
#define GUILIST_SetDisable3D CTRLLIST_SetDisable3D

/***************************************************************************//*!
@brief 设置Item风格
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_style [in] 风格
@param item_index [in] 被设置的索引
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_SetItemStyle(
    MMI_CTRL_ID_T     ctrl_id, // control id
    GUIITEM_STYLE_E item_style,
    uint16          item_index
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GUILIST_SetItemStyleEx(
    MMI_CTRL_ID_T     ctrl_id, // control id
    uint16          item_index,
    THEMELIST_ITEM_STYLE_T *item_style_ptr
);

/*****************************************************************************/
//  Description : set scroll bar style
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
#define GUILIST_SetListScrollBarStyle CTRLLIST_SetListScrollBarStyle

/***************************************************************************//*!
@brief 设置边距
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param left_space [in] 左边距
@param right_space [in] 右边距
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetMargin CTRLLIST_SetMargin

/***************************************************************************//*!
@brief 设置内嵌滚动条的背景图
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param image_id [in] 资源图片ID
@retval true/false
@return 返回成功或失败
@note
*******************************************************************************/
#define GUILIST_SetPrgBarImageId CTRLLIST_SetPrgBarImageId

/***************************************************************************//*!
@brief 设置Item是否灰化
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in] 索引
@param is_gray [in] 是否灰化
@retval true/false
@return 返回成功或失败
@note 被设置灰化的Item，将不执行按键及触笔操作
*******************************************************************************/
#define GUILIST_SetItemGray CTRLLIST_SetItemGray

/***************************************************************************//*!
@brief 设置ITEM是否有效
@author xiaoqing.lu
@param ctrl_id [in] 控件指针
@param pos [in] 索引
@param is_invalid [in] 是否有效
@retval true/false
@return 返回成功或失败
@note 被设置无效的Item，但仍执行按键及触笔操作
*******************************************************************************/
#define GUILIST_SetItemInvalid CTRLLIST_SetItemInvalid

/***************************************************************************//*!
@brief 获得Item区域
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param item_index [in]  索引
@param rect_ptr [in/out] 矩形的指针
@retval true/false
@return 返回成功或失败
@note Item区域可能随时都会改变
*******************************************************************************/
#define GUILIST_GetItemRect CTRLLIST_GetItemRect

/***************************************************************************//*!
@brief 改变多选框的默认softkey ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_cancel_id  [in] TEXT ID
@param text_select_id [in] TEXT ID
@param text_return_id [in] TEXT ID
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_ChangeCheckListDefSoftkeyID CTRLLIST_ChangeCheckListDefSoftkeyID

/***************************************************************************//*!
@brief 改变单选框的默认softkey ID
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param text_ok_id [in] TEXT ID
@param text_return_id  [in/out] TEXT ID
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_ChangeRadioListDefSoftkeyID CTRLLIST_ChangeRadioListDefSoftkeyID

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_SetSwitchItemTime CTRLLIST_SetSwitchItemTime

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_StartSwitchItem CTRLLIST_StartSwitchItem

/*****************************************************************************/
//  Description : stop auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_StopSwitchItem CTRLLIST_StopSwitchItem

/*****************************************************************************/
//  Description : 设置是否自动切换item
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认时不自动切换
/*****************************************************************************/
#define GUILIST_SetAutoSwitchItem CTRLLIST_SetAutoSwitchItem

/*****************************************************************************/
//  Description : 获得总的页数
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 每页显示的条目根据list区域及item高度计算所得
/*****************************************************************************/
#define GUILIST_GetTotalPageNum CTRLLIST_GetTotalPageNum

/*****************************************************************************/
//  Description : 获得当前页索引号
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 以当前页的最上面一个Item为准计算
/*****************************************************************************/
#define GUILIST_GetCurrentPageIndex CTRLLIST_GetCurrentPageIndex

/***************************************************************************//*!
@brief 获取纯净的item index
@author lianxiang.zhou
@param ctrl_id [in] 控件ID
@param gross_index  [in] TEXT ID
@param pure_index_ptr [in] TEXT ID
@retval true/false
@return 返回是否获得了纯净的index
@note 所谓纯净的，即不计split类的
*******************************************************************************/
#define GUILIST_GetPureIndex CTRLLIST_GetPureIndex

/***************************************************************************//*!
@brief 设置是否支持分隔符浮动效果，默认不支持
@author xiaoqing.lu
@param ctrl_id [in] 控件ID
@param is_true  [in] 设置或取消设置
@retval true/false
@return
@note
*******************************************************************************/
#define GUILIST_SetSplitOnside CTRLLIST_SetSplitOnside

/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Xiaoqing.lu
@param base_win_handle  [in] Must valid win handle, if not, create failed
@param base_ctrl_handle [in] You can set 0
@param ctrl_id          [in] You can set 0
@param init_data_ptr    [in] if null, will create ctrl by default param
@retval 0 or valid handle
@return 返回0或者有效的控件ID
@note 注意，当初始化数据init_data_ptr为空时，将创建一个空的控件，该控件不能添加Item
*******************************************************************************/
#define GUILIST_CreateListBox CTRLLIST_CreateListBox


/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Xiaoqing.lu
@param ctrl_id          [in] ctrl ID
@retval true or false
@return
@note
*******************************************************************************/
#define GUILIST_IsTpDown CTRLLIST_IsTpDown

/***************************************************************************//*!
@brief 设置控件的最大项，并申请内存
@author Xiaoqing.lu
@param ctrl_id          [in] ctrl ID
@retval item number
@return item number
@note
*******************************************************************************/
#define GUILIST_GetSelectedItemNum CTRLLIST_GetSelectedItemNum

/*****************************************************************************/
//  Description : 设置是否由用户built item layer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: 默认FALSE,控件自动built
/*****************************************************************************/
#define GUILIST_SetCusBltItemLayer CTRLLIST_SetCusBltItemLayer

/*****************************************************************************//*!
@brief 设置控件的Item显示的layer
@author Xiaoqing.lu
@param ctrl_id              [in] ctrl ID
@param lcd_dev_info_ptr     [in] lcd_dev_info_ptr ID
@retval item number
@return item number
@note
*****************************************************************************/
#define GUILIST_SetItemLayer CTRLLIST_SetItemLayer

/*****************************************************************************//*!
@brief 设置控件的Item显示的layer
@author Xiaoqing.lu
@param ctrl_id              [in] ctrl ID
@param get_data_func     [in] func ptr
@retval true/false
@return true/false
@note
*****************************************************************************/
#define GUILIST_RegGetDataFunc CTRLLIST_RegGetDataFunc

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:设置list支持横向滑动，默认不支持
/*****************************************************************************/
#define GUILIST_SetItemHorMove CTRLLIST_SetItemHorMove

/*****************************************************************************/
//  Description : GUILIST_SetItemLayer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
#define GUILIST_SetItemLayer CTRLLIST_SetItemLayer

/***************************************************************************//*!
@brief 设置禁止tip显示的状态
@author Lianxiang.Zhou
@param ctrl_id       [in] ctrl ID
@param is_forbid     [in] 是否禁止tip
@retval true/false
@return true/false
@note 默认是显示的
*******************************************************************************/
#define GUILIST_SetForbidTip CTRLLIST_SetForbidTip

/***************************************************************************//*!
@brief 获得List位置信息
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@retval
@return
@note
*******************************************************************************/
#define GUILIST_GetCurPosInfo CTRLLIST_GetCurPosInfo

/***************************************************************************//*!
@brief 设置List位置信息
@author xiaoqing.lu
@param ctrl_id  [in]    控件ID
@retval
@return
@note
*******************************************************************************/
#define GUILIST_SetCurPosInfo CTRLLIST_SetCurPosInfo

/*****************************************************************************/
//  Description : set check list default softkey id
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_ChangeCheckListSoftkeyIndex CTRLLIST_ChangeCheckListSoftkeyIndex

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:设置list支持向左滑动，默认不支持；该函数请搭配 CTRLLIST_SetItemHorMove() 配置使用
/*****************************************************************************/
#define GUILIST_SetItemLeftMove CTRLLIST_SetItemLeftMove

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:item被点击时，list是否需要高亮条状态，默认情况下不显示高亮条
/*****************************************************************************/
#define GUILIST_SetItemIsNeedHighBarIfClick CTRLLIST_SetItemIsNeedHighBarIfClick

/*****************************************************************************/
//  Description : whether support dec anim when sliding or not
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:
/*****************************************************************************/
#define GUILIST_SetIsSupportSlideDecAnim CTRLLIST_SetIsSupportSlideDecAnim

/*****************************************************************************/
//  Description : Get the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILIST_GetItemState(
    const GUILIST_ITEM_T *item_ptr,
    GUIITEM_STATE_T src_state
);

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUILIST_SetItemState(
    GUILIST_ITEM_T *item_ptr,
    GUIITEM_STATE_T  src_state,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 为了兼容
/*****************************************************************************/
PUBLIC const THEMELIST_ITEM_STYLE_T *GUILIST_GetItemStyle(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 为了兼容
/*****************************************************************************/
PUBLIC const uint32 GUILIST_GetItemStyleID(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif /* END _GUILISTBOX_H_  */


/*@}*/


