/*! @file ctrltips_export.h
@brief 此文件介绍了tips控件的对外接口和数据类型
@author Xiaoqing.Lu
@version 1.0
@date 02/20/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 2/2009         Xiaoqing.Lu      Create
*******************************************************************************/

/*! @page page_Tips Tips(Tips)

-# @ref sec_tips_function
-# @ref sec_tips_create
-# @ref sec_tips_app

@section sec_tips_function 控件功能

Tips是窗口内容信息的提示控件，它能够帮助窗口提示一些附加信息，使得窗口的某些属性更为清晰地显示给用户。在一个窗口中，只存在一个唯一的tips控件。Tips控件不响应任何消息。

@section sec_tips_create Tips控件创建

和状态栏类似的，Tips控件也被看为窗口的一个属性，它的创建是调用WIN_TIPS宏，与状态栏一样，放置在窗口的Table中。当Tips需要显示数据的时候，它会发送MSG_CTL_TIPS_NEED_DATA消息给用户，并携带一定的参数，例如当前listbox的项索引号等，用户接收到这个消息之后填入相关数据。给Tips控件显示。

Tips控件比较简单，它不处理任何消息，也不响应用户的输入，它的作用就是显示提示信息，因此用户使用起来也比较简单。

虽然Tips是窗口的一个属性，但目前来看它和列表菜单和列表控件存在着紧密的关系，因此也只有在列表菜单和列表控件的窗口中使用Tips。

当用户接收到Tips消息时，会接收到一个消息参数，这个参数的实际类型如下：
- @ref GUITIPS_DATA_T

消息来时，这个参数携带的信息有控件的ID，当前item的索引号，用户可以决定是否显示Tips，以及在Tips上显示的icon id和字符串，用户只需要修改这个参数就可以了，而不需要自己申请额外的内存。

@section sec_tips_app 控件应用

例如，在电话本中，使用tips控件显示姓名为“高级别”的电话号码为“123456789”：
@image html tips_display.jpg
图 2 38 控件显示例图
*******************************************************************************/

/*! @addtogroup tipsGrp Tips
@brief tips控件
@details tips控件
@{
*******************************************************************************/
#ifndef _CTRLTIPS_EXPORT_H_
#define _CTRLTIPS_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmk_type.h"
#include "mmitheme_tips.h"
#include "guictrl.h"

#include "ctrlbase_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//tips handle
typedef struct _tips_obj_tag* CTRL_TIPS_HANDLE_T;

/*! @struct GUITIPS_DATA_T
@brief tips 数据结构
*/
typedef struct
{
    BOOLEAN         is_need_tips;                   /*!< 是否需要索引 */
    uint16          index;                          /*!< 序号 */
    MMI_CTRL_ID_T   ctrl_id;                        /*!< ctrl_id */
    MMI_IMAGE_ID_T  icon_id;                        /*!< icon_id */
    wchar           wstr_ptr[MMITIPS_TEXT_MAX_LEN]; /*!< 图标字符 */
    uint32          wstr_len;                       /*!< 字符长度 */
} GUITIPS_DATA_T;

/*! @struct GUITIPS_TEXT_T
@brief tips 文本信息
*/
typedef struct
{
    wchar           wstr_ptr[MMITIPS_TEXT_MAX_LEN]; /*!< 图标字符 */
    uint32          wstr_len;                       /*!< 字符长度 */
} GUITIPS_TEXT_T;

/*! @struct GUITIPS_INIT_DATA_T
@brief tips初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T           both_rect;    /*!< 区域信息 */
} GUITIPS_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief  画tips控件
@author xiaoqing.lu
@param win_handle           [in]    窗口handle
@param orig_rect_ptr        [in]    原始区域信息
@param tips_data_ptr        [in]    tips数据
@return  无
@note
*******************************************************************************/
PUBLIC void	CTRLTIPS_Update (
    MMI_HANDLE_T    win_handle,
    GUI_RECT_T	   *orig_rect_ptr,
    GUITIPS_DATA_T  *tips_data_ptr
);

/***************************************************************************//*!
@brief  设置tips显示的边界区域，返回设置之前的值
@author xiaoqing.lu
@param win_handle           [in]    窗口handle
@param edge_rect            [in]    要设置的边界区域信息
@return   设置成功，返回设置之前的值；失败，返回"0";
@note
*******************************************************************************/
PUBLIC GUI_RECT_T CTRLTIPS_SetEdgeRect (
    MMI_HANDLE_T win_handle,
    GUI_RECT_T   edge_rect
);

/***************************************************************************//*!
@brief  设置tips文本信息
@author xiaoqing.lu
@param win_handle           [in]    窗口handle
@param tips_text            [in]    文本信息
@return  设置成功返回TRUE，失败返回FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_SetTipsText (
    MMI_HANDLE_T    win_handle,
    GUITIPS_TEXT_T  tips_text
);

/***************************************************************************//*!
@brief    获取tips文本信息
@author xiyuan.ma
@param win_handle           [in]    窗口handle
@param tips_text            [in]    文本信息
@return  设置成功返回TRUE，失败返回FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_GetTipsText (
    MMI_HANDLE_T    win_handle,
    GUITIPS_TEXT_T  tips_text
);

/***************************************************************************//*!
@brief  设置tips是否可见
@author xiaoqing.lu
@param win_handle           [in]    窗口handle
@param is_valid             [in]    是否可见
@return  设置成功返回TRUE，失败返回FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_SetTipsValid (
    MMI_HANDLE_T    win_handle,
    BOOLEAN         is_valid
);

/***************************************************************************//*!
@brief  获取tips控件是否可见
@author xiaoqing.lu
@param win_handle           [in]    窗口handle
@return  tips控件可见返回TRUE，不可见返回FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_GetTipsValid (
    MMI_HANDLE_T    win_handle
);

/***************************************************************************//*!
@brief  设置tips文本颜色
@author xiaoqing.lu
@param win_handle           [in]    窗口handle
@param text_color           [in]    文本颜色
@return  设置成功返回设置的颜色，设置失败返回"0";
@note
*******************************************************************************/
PUBLIC GUI_COLOR_T	CTRLTIPS_SetTextColor (
    MMI_HANDLE_T    win_handle,
    GUI_COLOR_T     text_color
);

/***************************************************************************//*!
@brief  设置lcd信息
@author xiaoqing.lu
@param win_handle           [in]    窗口handle
@param lcd_info_ptr         [in]    lcd信息
@return  无
@note
*******************************************************************************/
PUBLIC void CTRLTIPS_SetLcdInfo (
    MMI_HANDLE_T    win_handle,
    GUI_LCD_DEV_INFO *lcd_info_ptr
);

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif//_CTRLTIPS_H_
/*@}*/
