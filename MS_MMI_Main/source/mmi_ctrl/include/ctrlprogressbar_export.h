/*! @file ctrlprogressbar.h
@brief 此文件介绍了prgbox控件的对外接口和数据类型
@author xiyuan.ma
@version 1.0
@date 01/10/2007
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 01/2007      xiyuan.ma              Creat
*******************************************************************************/

/*! @page page_progressbar 滚动条和进度条(PrgBox)

-# @ref sec_prgbox_function
-# @ref sec_prgbox_create
-# @ref sec_prgbox_app

@section sec_prgbox_function 控件功能

- Prgbox即滚动条和进度条控件，其中prg是progress的缩写。在显示文本的时候，如果显示超过一页（即屏幕所能显示内容），通常使用滚动条控件提醒用户——本页没有显示完整，可以使用上下键切换上下页或下一行。
- 另外，当传输文件时，可以使用进度条显示文件传输的进度，例如蓝牙传输文件，web下载等。
- 滚动条和进度条控件作为滚动条使用时，一般内嵌在List、Menu、Dropdownlist控件等其他控件中，用户无需关心滚动条的使用。当然，用户也可以单独使用。
- 作为进度条控件，一般被单独应用在窗口中，例如表示下载文件的进度。
- 滚动条既可以是横向的，也可以是纵向的，它能够响应触笔消息，设计时，为滚动条设置了上下箭头（横向滚动条为左右箭头）。例如，当触笔点击上下箭头的时候，可以切换列表的上一条或下一条记录。当触笔点击在滚动条区域中滑块以外的区域时，可以切换上下页，还可以触笔拖动滑块。

@section sec_prgbox_create 控件的创建

滚动条和进度条控件（以下简称Prg）一般情况下用来嵌入其他控件中，例如Textbox、Editbox、menu、list等，不需要用户自己创建。如果用户需要单独使用该控件，创建时分为动态创建和静态创建，其创建方式和其他控件相同。
创建之后，根据需求设置相关的参数就可以了，这个控件的接口相对来说很少，也比较简单。

@section sec_prgbox_app 控件应用
下图为滚动条控件的使用：
@image html prgbox_display.jpg
图 2 42 控件显示例图
*******************************************************************************/

/*! @addtogroup groupGrp PrgBox
@brief group模块
@details group模块
@{
*******************************************************************************/

#ifndef _CTRLPROGRESSBAR_EXPORT_H_
#define _CTRLPROGRESSBAR_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmitheme_prgbox.h"
#include "guictrl.h"

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
//ownerdraw handle
typedef struct _progressbar_obj_tag* CTRL_PROGRESSBAR_HANDLE_T;

/*!  @typedef GUIPRGBOX_STYLE_E
@brief 滚动条显示方式
*/
typedef enum
{
    GUIPRGBOX_STYLE_VERTICAL_SCROLL,        /*!< 纵向滚动条 */
    GUIPRGBOX_STYLE_HORIZONTAL_SCROLL,      /*!< 横向滚动条 */
    GUIPRGBOX_STYLE_VERTICAL_PROCESS,       /*!< 纵向进度条 */
    GUIPRGBOX_STYLE_HORIZONTAL_PROCESS,     /*!< 纵向进度条 */
    GUIPRGBOX_STYLE_VOLUME_BAR,             /*!< 音量调节条 */
    GUIPRGBOX_STYLE_VERTICAL_WINDOWS,       /*!< 保留 */
    GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS,     /*!< 保留 */
#ifdef GUIF_PRGBOX_LETTERSCROLL
    GUIPRGBOX_STYLE_LETTERS_SCROLL,     /*字母滚动条*/
#endif
    GUIPRGBOX_STYLE_MAX                     /*!< 无 */
} GUIPRGBOX_STYLE_E;

/*!  @typedef CTRLPROGRESSBAR_RESULT_E
@brief 滚动条handle result
*/
typedef enum
{
    GUIPRGBOX_RESULT_NONE,          /*!< 无 */
    GUIPRGBOX_RESULT_HEAD,          /*!< 滚动条头部 */
    GUIPRGBOX_RESULT_TAIL,          /*!< 滚动条尾部 */
    GUIPRGBOX_RESULT_PRE_ONE,       /*!< 前一个 */
    GUIPRGBOX_RESULT_NEXT_ONE,      /*!< 后一个 */
    GUIPRGBOX_RESULT_PRE_PAGE,      /*!< 前一页 */
    GUIPRGBOX_RESULT_NEXT_PAGE,     /*!< 后一页 */
    GUIPRGBOX_RESULT_PRE_MOVE,      /*!< 上个移动 */
    GUIPRGBOX_RESULT_NEXT_MOVE,     /*!< 下个移动 */
    GUIPRGBOX_RESULT_CHANGE,          //for progress tp press&move
    GUIPRGBOX_RESULT_MAX            /*!< 无 */
} GUIPRGBOX_RESULT_E;

/*! @struct CTRLPROGRESSBAR_INIT_DATA_T
@brief 滚动条初始化数据结构
*/
typedef struct
{
    BOOLEAN                 is_forbid_paint;/*!< 是否刷新 */
    uint32                  lcd_id;         /*!< lcd_id */
    GUI_BOTH_RECT_T         both_rect;      /*!< 区域设置(可以设置横向或者纵向) */
    GUIPRGBOX_STYLE_E style;          /*!< 滚动条模式 */
    MMITHEME_PRGBOX_TYPE_E  theme_type;     /*!< 滚动条主题 */
} GUIPRGBOX_INIT_DATA_T;

typedef struct
{
    uint16          item_index;	/*!<从0开始*/
    MMI_NOTIFY_T	notify;			/*!<公用结构*/
} CTRLPRGBOX_NOTIFY_T;

#define GUIPRGBOX_LETTER_FONT CAF_FONT_8
#define GUIPRGBOX_SCROLL_SPACE     8
#define VOLUME_BRIGHTNESS_BAR_HEIGHT 64

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 根据控件id设置位置
@author Jassmine
@param  is_update        [in]   是否刷新
@param  cur_item_index   [in]   当前item序号
@param  first_item_index [in]   第一个item的序号
@param  ctrl_id          [in]   控件id
@return 无
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetPos (
    BOOLEAN          is_update,      //is update
    uint32           cur_item_index,
    uint32			first_item_index,
    MMI_CTRL_ID_T    ctrl_id         //control id
);

/***************************************************************************//*!
@brief 根据控件id刷新
@author Jassmine
@param  ctrl_id        [in] 控件id
@return 无
@note
*******************************************************************************/
void CTRLPROGRESSBAR_Update (
    MMI_CTRL_ID_T   ctrl_id //control id
);

/***************************************************************************//*!
@brief 根据控件信息重新设置区域
@author allen
@param  prgbox_ptr          [in]    滚动条控件的数据信息
@param  new_rect            [in]    区域信息
@return 设置成功返回TRUE，否则返回FALSE
@note
*******************************************************************************/
BOOLEAN CTRLPROGRESSBAR_ResizeRect (
    MMI_HANDLE_T handle,
    GUI_RECT_T          new_rect  // the new total rectangle
);

/***************************************************************************//*!
@brief 根据控件handle设置区域
@author Jassmine
@param  ctrl_handle           [in]  控件handle
@param  rect_ptr              [in]  新的区域信息
@return 无
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetRect (
    MMI_HANDLE_T      ctrl_handle,
    GUI_RECT_T        *rect_ptr
);

/***************************************************************************//*!
@brief 根据控件id设置item总数和每页item个数
@author guanyong.zhang
@param  ctrl_id              [in]   控件id
@param  total_item_count     [in]   item总数量
@param  items_per_page       [in]   每页item个数
@return 无
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetParam (
    MMI_CTRL_ID_T      ctrl_id,
    uint32             total_item_count,
    uint32             items_per_page
);

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description : set parameter: top limitation and bottom limitation 
//  Global resource dependence :
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetConstraint(
    MMI_CTRL_ID_T  ctrl_id,
    uint32         max,
    uint32         min
);
#endif

/***************************************************************************//*!
@brief 根据控件数据信息设置是否有循环滚动条
@author guanyong.zhang
@param      prgbox_ctrl_ptr     [in]    控件数据信息
@param      is_loop_scroll      [in]    是否设置循环滚动条
@return 无
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetLoopScroll (
    MMI_HANDLE_T        handle,
    BOOLEAN             is_loop_scroll
);

/***************************************************************************//*!
@brief 设置垂直滚动条的背景
@author Jassmine
@param  ctrl_id      [in]   控件id
@param  bg_id        [in]   背景图片id
@return 设置滚动条背景
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetVerGrooveBgImg (
    MMI_CTRL_ID_T    ctrl_id,
    MMI_IMAGE_ID_T   bg_id
);

/***************************************************************************//*!
@brief 根据数据信息设置是否需要滑块
@author xiaoqing.lu
@param      prgbox_ctrl_ptr     [in]    控件数据信息
@param      is_true             [in]    是否需要设置滑块
@return 返回设置前的数据
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetNeedSlideBlock (
    MMI_HANDLE_T handle,
    BOOLEAN              is_true
);

/***************************************************************************//*!
@brief 根据控件数据信息判断是否需要设置背景
@author xiaoqing.lu
@param      prgbox_ctrl_ptr [in]    控件数据信息
@param      is_true         [in]    是否需要设置背景
@return 返回设置前的数据
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetNeedBg (
    MMI_HANDLE_T handle,
    BOOLEAN              is_true
);

/***************************************************************************//*!
@brief 设置背景颜色
@author guanyong.zhang
@param      ctrl_id     [in]    控件id
@param      bg_color    [in]    背景颜色
@return 无
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetBgColor (
    MMI_CTRL_ID_T        ctrl_id,
    GUI_COLOR_T         bg_color
);

/***************************************************************************//*!
@brief 设置滚动条bar的图片
@author Jassmine
@param      ctrl_id [in]    控件id
@param      bg_id   [in]    图片id
@return 成功返回TRUE，失败返回FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetScrollBarImg (
    MMI_CTRL_ID_T    ctrl_id,
    MMI_IMAGE_ID_T   bg_id
);

/***************************************************************************//*!
@brief 获取当前滚动条的位置所对应字母的unicode
@author
@param      prgbox_ctrl_ptr：［IN]滚动条的控件指针
@param      item_pos：滚动条的当前位置
@return 返回item_pos所对应的字母的unicode
@note
*******************************************************************************/
PUBLIC wchar  CTRLPROGRESSBAR_GetWcharByPos (
    MMI_HANDLE_T handle,
    uint16  item_pos
);
/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetBothRect (
    MMI_HANDLE_T handle,
    GUI_BOTH_RECT_T     *both_rect_ptr
);

/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetFobidThumb (
    MMI_CTRL_ID_T ctrl_id
);

PUBLIC uint32 CTRLPROGRESSBAR_GetTotalItemNum (MMI_HANDLE_T handle);
PUBLIC uint32 CTRLPROGRESSBAR_GetFirstItemIndex (MMI_HANDLE_T handle);

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_CTRLPROGRESSBAR_EXPORT_H_


