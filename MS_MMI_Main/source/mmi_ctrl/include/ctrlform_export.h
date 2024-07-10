/*! @file ctrlform_export.h
@brief 此文件介绍了form控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Create
*******************************************************************************/

/*! @page page_Form 容器(Form)

-# @ref sec_form_function
-# @ref sec_form_def
-# @ref sec_form_def_child
    -# @ref ssec_form_def_child_text
    -# @ref ssec_form_def_child_edit_text
    -# @ref ssec_form_def_child_edit_phonenumber
    -# @ref ssec_form_def_child_edit_digital
    -# @ref ssec_form_def_child_edit_passwd
    -# @ref ssec_form_def_child_edit_list
    -# @ref ssec_form_def_child_edit_data
    -# @ref ssec_form_def_child_edit_time
    -# @ref ssec_form_def_child_edit_ip
    -# @ref ssec_form_def_child_anim
    -# @ref ssec_form_def_child_label
    -# @ref ssec_form_def_child_button
    -# @ref ssec_form_def_child_downdroplist
    -# @ref ssec_form_def_child_setlist
    -# @ref ssec_form_def_child_owndraw
    -# @ref ssec_form_def_child_form
-# @ref sec_form_layout
-# @ref sec_form_attribute
-# @ref sec_form_child_attribute
-# @ref sec_form_function_list
-# @ref sec_form_app
-# @ref sec_form_usage_add_child

@section sec_form_function 控件功能

Form控件即容器控件，可以内嵌入平台支持的所有控件，包括容器控件本身，目前只支持Text，Edit，Anim，Label，Button，Dropdown list，Setting list，Owner draw和Form控件。
-# 内嵌子控件自动排版，自适应不同大小的LCD，显示不下，自动添加滚动条；
-# 横竖屏自动切换，不需应用修改；
-# 子控件可以灰化，可以隐藏；
-# 子控件可以获得Active，并根据Active控件显示相应的Title，Softkey；
-# 子控件可以顺序或者并排显示；

@section sec_form_def 宏定义解析

@code
#define CREATE_FORM_CTRL(_LAYOUT_, _CTRL_ID_) \
        MMK_CREATE_FORM, _LAYOUT_, _CTRL_ID_
@endcode
- 用于定义Form控件，默认显示区域除去Title和Softkey。
    - _LAYOUT_ 子控件排版方式，顺序或者并排。
    - _CTRL_ID_ 控件ID

@section sec_form_def_child 子控件宏定义解析

@subsection ssec_form_def_child_text CHILD_TEXT_CTRL

@code
#define CHILD_TEXT_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_TEXT, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Text子控件
    - _IS_GET_ACTIVE_ 是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_text CHILD_EDIT_TEXT_CTRL

@code
#define CHILD_EDIT_TEXT_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_EDIT_TEXT, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit 普通文本编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _STR_MAX_LEN_ 允许输入的最大字符个数
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_phonenumber CHILD_EDIT_PHONENUM_CTRL

@code
#define CHILD_EDIT_PHONENUM_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_PHONENUM, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit电话号码编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _STR_MAX_LEN_ 允许输入的最大字符个数
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_digital CHILD_EDIT_DIGITAL_CTRL

@code
#define CHILD_EDIT_DIGITAL_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_DIGITAL, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit数字编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _STR_MAX_LEN_ 允许输入的最大字符个数
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_passwd CHILD_EDIT_PASSWORD_CTRL

@code
#define CHILD_EDIT_PASSWORD_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_PASSWORD, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit密码编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _STR_MAX_LEN_ 允许输入的最大字符个数
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_list CHILD_EDIT_LIST_CTRL

@code
#define CHILD_EDIT_LIST_CTRL(_IS_GET_ACTIVE_, _ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_LIST, _IS_GET_ACTIVE_, _ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit列表编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _ITEM_NUM_    列表条数
    - _ITEM_STR_MAX_LEN_    每条允许输入的最大字符个数
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_data CHILD_EDIT_DATE_CTRL

@code
#define CHILD_EDIT_DATE_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_DATE, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit日期编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_time CHILD_EDIT_TIME_CTRL

@code
#define CHILD_EDIT_TIME_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_TIME, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit时间编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_edit_ip CHILD_EDIT_IP_CTRL

@code
#define CHILD_EDIT_IP_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_IP, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit IP地址编辑子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_anim CHILD_ANIM_CTRL

@code
#define CHILD_ANIM_CTRL(_IS_BG,_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_ANIM, _IS_BG, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Anim子控件
    - _IS_BG    是否是Form背景，是则不能够获得Active
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_label CHILD_LABEL_CTRL

@code
#define CHILD_LABEL_CTRL(_ALIGN_, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_LABEL, _ALIGN_, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Label子控件
    - _ALIGN_   字符排版方式
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_button CHILD_BUTTON_CTRL

@code
#define CHILD_BUTTON_CTRL(_IS_GET_ACTIVE_, _BG_IMAGE_ID, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_BUTTON, _IS_GET_ACTIVE_, _BG_IMAGE_ID, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Button子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _BG_IMAGE_ID  背景图，即release状态下的背景图片
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_downdroplist CHILD_DROPDOWNLIST_CTRL

@code
#define CHILD_DROPDOWNLIST_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_) \
        CREATE_CHILD_DROPDOWNLIST, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_
@endcode
- 下拉菜单子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_setlist CHILD_SETLIST_CTRL

@code
#define CHILD_SETLIST_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_) \
        CREATE_CHILD_SETLIST, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_
@endcode
- 设置列表子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@subsection ssec_form_def_child_owndraw CHILD_OWNDRAW_CTRL

@code
#define CHILD_OWNDRAW_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_, _CALL_BACK_) \
        CREATE_CHILD_OWNDRAW, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_, _CALL_BACK_
@endcode
- 自绘子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID
    - _CALL_BACK_   自绘回调函数

@subsection ssec_form_def_child_form CHILD_FORM_CTRL

@code
#define CHILD_FORM_CTRL(_IS_GET_ACTIVE_, _LAYOUT_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_FORM, _IS_GET_ACTIVE_, _LAYOUT_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- 容器子控件
    - _IS_GET_ACTIVE_   是否能够获得Active
    - _LAYOUT_  子控件排列方式
    - _CTRL_ID_ 控件ID
    - _PARENT_CTRL_ID_  父控件ID

@section sec_form_layout 子控件排列方式

-# GUIFORM_LAYOUT_ORDER
    -# 顺序排列
    -# 默认宽度为Form去除边框，页边距，滚动条宽度
    -# 可以通过CTRLFORM_SetChildWidth接口函数设置宽度
-# GUIFORM_LAYOUT_SBS
    -# 并排排列
    -# 默认每个子控件宽度均分
    -# 可以通过CTRLFORM_SetChildWidth接口函数设置宽度
    -# 非默认宽度，并排n个子控件，必须设置n-1个子控件宽度

@section sec_form_attribute FORM属性

-# 背景，边框，页边距；
-# 子控件水平，垂直间距
-# 图片解码的宽高比例，默认4：3；
-# 滚动一步占整个Form的比例4：5；
-# 子控件背景，边框，字体，包括灰化和Active部分

@section sec_form_child_attribute 子控件属性
-# 子控件水平，垂直间距
-# 子控件背景，边框，字体
-# 子控件宽度

@section sec_form_function_list 关键接口介绍

-# CTRLFORM_SetRect
-# CTRLFORM_SetVisible
-# CTRLFORM_SetBg
-# CTRLFORM_SetSaveBg
-# CTRLFORM_SetMargin
-# CTRLFORM_SetSpace
-# CTRLFORM_SetChildWidth
-# CTRLFORM_SetAlign
-# CTRLFORM_GetChildDisplay
-# CTRLFORM_SetChildDisplay
-# CTRLFORM_PermitChildBg
@note该函数主要用于某些控件要使用自身特殊的背景，而不是Form通用的背景，如下：
@image html form_bg_example.jpg
图 2 47Form子控件不允许设置背景
-# CTRLFORM_CreatDynaCtrl
-# CTRLFORM_CreatDynaChildCtrl

@section sec_form_app 控件应用

Form控件主要应用在多个控件组合的界面，比如闹钟编辑界面
@image html form_app_example.jpg 
图 2 48Form控件的实际应用
@code
WINDOW_TABLE( ALARM_EDITWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleAlarmEditWinMsg), 
    WIN_ID(MMIALM_EDIT_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_NAME_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    IALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_EDITWIN_CHILD_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON0_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON1_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON2_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON3_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON4_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON5_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON6_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),  
                
    END_WIN
};
@endcode

@section sec_form_usage_add_child 如何添加内嵌子控件

如何添加一个控件作为Form的子控件呢？下面以setting list为例：
-# window_parse.h 创建子控件宏
-# CTRLFORM_parse.c 解析子控件
-# guisetlist.c 实现GUICTRL_SetRect，设置区域
-# guisetlist.c 实现GUICTRL_SetBorder，设置边框
-# guisetlist.c 实现GUICTRL_SetBg，设置背景
-# guisetlist.c 实现GUICTRL_GetFont，获得字体
-# guisetlist.c 实现GUICTRL_SetFont，设置字体
-# guisetlist.c 实现GUICTRL_SetProgress，可以设置是否显示滚动条，无滚动条，则用默认函数即可
-# guisetlist.c 实现GUICTRL_SetCircularHandle，设置不循环处理上下按键，无，则用默认函数即可
-# guisetlist.c 实现GUICTRL_GetHeightByWidth，根据宽度获得控件的高度
-# guisetlist.c 实现GUICTRL_GetDisplayRect，获得显示区域
-# guisetlist.c 实现GUICTRL_SetDisplayRect，设置显示区域
-# guisetlist.c 实现GUICTRL_GetSelectRect，获得选中区域，如cursor,list，无，则用默认函数即可
-# guisetlist.c 实现GUICTRL_GetSelectIndex获得选中索引是否是第一个或者最后一个，无，则用默认函数即可

控件内部修改：
-# 背景显示，guicommon.c中有一个共用函数GUI_DisplayBg()
-# 边框显示，guicommon.c中有一个共用函数GUI_DisplayBorder
-# MSG_CTL_GET_ACTIVE，MSG_CTL_LOSE_ACTIVE需要同步通知到其父控件
-# 增加display_rect用来控制显示控件的某块区域
*******************************************************************************/

/*! @addtogroup formGrp Form
@brief form窗口控件
@details 
    - 容器控件
    - 支持自动排版
@{
*******************************************************************************/


#ifndef _CTRLFORM_EXPORT_H_
#define _CTRLFORM_EXPORT_H_ 

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "guiprgbox.h"
#include "mmitheme_form.h"

/*---------------------------------------------------------------------------
                         Compiler Flag                                      *
 ---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------
                          MACRO DEFINITION                                 
 --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                          TYPE AND STRUCT                                  
--------------------------------------------------------------------------*/
/*! @enum GUIFORM_LAYOUT_E
@brief 子控件排列方式方式
*/
typedef enum
{
    GUIFORM_LAYOUT_ERROR = -1,  //bug1941758
    GUIFORM_LAYOUT_ORDER,   /*!<默认,控件从上至下顺序排列*/
    GUIFORM_LAYOUT_SBS,     /*!<子控件并排排列*/
    GUIFORM_LAYOUT_MAX      /*!<保留位*/
} GUIFORM_LAYOUT_E;

/*! @enum GUIFORM_TYPE_E
@brief form类型
*/
typedef enum
{
    GUIFORM_TYPE_COMMON,    /*!<普通类型*/
    GUIFORM_TYPE_TP,        /*!<纯触摸类型,仿List*/
    GUIFORM_TYPE_MAX        /*!<保留位*/
} GUIFORM_TYPE_E;

/*! @enum GUIFORM_STYLE_E
@brief form风格
*/
typedef enum
{
    GUIFORM_STYLE_NORMAL,   /*!<默认,容器*/
    GUIFORM_STYLE_UNIT,     /*!<form作为一个整体,显示背景和边框*/
    GUIFORM_STYLE_MAX       /*!<保留位*/
} GUIFORM_STYLE_E;

/*! @enum GUIFORM_CHILD_WIDTH_E
@brief 子控件宽度类型
*/
typedef enum
{
    GUIFORM_CHILD_WIDTH_AUTO,       /*!<平均分配*/
    GUIFORM_CHILD_WIDTH_FIXED,      /*!<固定宽度*/
    GUIFORM_CHILD_WIDTH_PERCENT,    /*!<按照百分比计算*/
    GUIFORM_CHILD_WIDTH_MAX         /*!<保留位*/
} GUIFORM_CHILD_WIDTH_E;

/*! @enum GUIFORM_CHILD_HEIGHT_E
@brief 子控件高度类型
*/
typedef enum
{
    GUIFORM_CHILD_HEIGHT_AUTO,      /*!<按照比例根据宽度计算*/
    GUIFORM_CHILD_HEIGHT_FIXED,     /*!<固定高度*/
    GUIFORM_CHILD_HEIGHT_PERCENT,   /*!<子控件高度按照百分比计算*/
    GUIFORM_CHILD_HEIGHT_LEFT,      /*!<form剩下的高度,保持不变*/
    GUIFORM_CHILD_HEIGHT_LEFT_MIN,  /*!<大于Form剩下高度,则为自身高度;小于Form剩下高度,则为Form剩下高度*/
    GUIFORM_CHILD_HEIGHT_MAX        /*!<保留位*/
} GUIFORM_CHILD_HEIGHT_E;

/*! @enum GUIFORM_CHILD_ALIGN_E
@brief 并排子控件排列方式
*/
typedef enum
{
    GUIFORM_CHILD_ALIGN_ERROR = -1,  //bug1941758
    GUIFORM_CHILD_ALIGN_NONE,   /*!<默认,左上*/
    GUIFORM_CHILD_ALIGN_LEFT,   /*!<靠左*/
    GUIFORM_CHILD_ALIGN_HMIDDLE,/*!<水平居中*/
    GUIFORM_CHILD_ALIGN_RIGHT,  /*!<靠右*/
    GUIFORM_CHILD_ALIGN_MAX     /*!<保留位*/
} GUIFORM_CHILD_ALIGN_E;

/*! @enum GUIFORM_CHILD_DISPLAY_E
@brief 子控件显示方式
*/
typedef enum
{
    GUIFORM_CHILD_DISP_NORMAL,  /*!<正常显示*/
    GUIFORM_CHILD_DISP_GRAY,    /*!<灰化显示*/
    GUIFORM_CHILD_DISP_HIDE,    /*!<隐藏*/
    GUIFORM_CHILD_DISP_MAX      /*!<保留位*/
} GUIFORM_CHILD_DISPLAY_E;

/*! @enum GUIFORM_CHILD_DOCK_E
@brief 子控件倚靠方式
*/
typedef enum
{
    GUIFORM_CHILD_DOCK_NONE,    /*!<系统默认*/
    GUIFORM_CHILD_DOCK_TOP,     /*!<置顶*/
    GUIFORM_CHILD_DOCK_BOTTOM,  /*!<置底*/
    GUIFORM_CHILD_DOCK_LEFT,    /*!<置左*/
    GUIFORM_CHILD_DOCK_RIGHT,   /*!<置右*/
    GUIFORM_CHILD_DOCK_MAX,
} GUIFORM_CHILD_DOCK_E;

/*! @struct CTRLFORM_INIT_DATA_T
@brief form控件初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T         both_rect;  /*!<横竖区域*/
    GUIFORM_LAYOUT_E        layout_type;/*!<子控件排列方式*/
} GUIFORM_INIT_DATA_T;

/*! @struct GUIFORM_CHILD_WIDTH_T
@brief 子控件宽度信息
*/
typedef struct
{
    uint16                  add_data;   /*!<固定宽度或者百分比[0,100]*/
    GUIFORM_CHILD_WIDTH_E   type;       /*!<宽度类型*/
} GUIFORM_CHILD_WIDTH_T;

/*! @struct GUIFORM_CHILD_HEIGHT_T
@brief 子控件高度信息
*/
typedef struct
{
    uint16                  add_data;   /*!<固定高度*/
    GUIFORM_CHILD_HEIGHT_E  type;       /*!<高度类型*/
} GUIFORM_CHILD_HEIGHT_T;

/*! @struct GUIFORM_DYNA_CHILD_T
@brief 动态创建子控件信息
*/
typedef struct
{
    BOOLEAN         is_bg;          /*!<是否是form背景*/
    BOOLEAN         is_softkey;     /*!<是否是softkey*/
    BOOLEAN         is_get_active;  /*!<是否能够获得焦点*/
    CAF_GUID_T      guid;           /*!<id*/
    MMI_HANDLE_T    child_handle;   /*!<子控件handle*/
    void            *init_data_ptr; /*!<初始化数据*/
} GUIFORM_DYNA_CHILD_T;

/*! @struct GUIFORM_NOTIFY_LINE_T
@brief 行变化信息
*/
typedef struct
{
    BOOLEAN         is_set_size;        /*!<是否限制大小*/
    BOOLEAN         is_line_num_change; /*!<是否总行数变化*/
    CAF_HANDLE_T    ctrl_handle;        /*!<控件handle*/
} GUIFORM_NOTIFY_LINE_T;

/*---------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 解析form子控件
@author Jassmine
@param index      [in] 子控件索引
@param win_handle [in] 窗口handle
@param value_pptr [in] 子控件表值
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_ParseChild(
                                   uint32        index,
                                   MMI_HANDLE_T  win_handle,
                                   uint32        **value_pptr
                                   );
/***************************************************************************//*!
@brief 设置激活的子form
@author fangfang.yao
@param form_handle      [in] form handle
@param child_handle     [in] 子form handle
@return 是否操作成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetActiveChild(
                             MMI_HANDLE_T    form_handle,
                              MMI_HANDLE_T   child_handle
                             );
 /***************************************************************************//*!
 @brief 设置form reset slide frame number
 @author fangfang.yao
 @param form_handle    [in] form handle
 @param frame          [in] form reset slide frame
 @return 无
 @note
 *******************************************************************************/
 PUBLIC void CTRLFORM_SetResetSlideFrame(
                            MMI_HANDLE_T    form_handle,
                            uint16 frame
                            );
/***************************************************************************//*!
@brief 设置控件区域
@author Jassmine
@param ctrl_handle [in] 控件handle
@param rect_ptr    [in] 区域
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetRect(
                             MMI_HANDLE_T    ctrl_handle,
                             GUI_RECT_T      *rect_ptr
                             );

/***************************************************************************//*!
@brief 设置控件是否可见
@author Jassmine
@param form_handle [in] 控件handle
@param is_visible  [in] 是否可见
@param is_update   [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetVisible(
                                   MMI_HANDLE_T      form_handle,
                                   BOOLEAN           is_visible,
                                   BOOLEAN           is_update
                                   );

/***************************************************************************//*!
@brief 获得容器控件类型
@author Jassmine
@param ctrl_handle [in] 控件handle
@return 类型
@note 
*******************************************************************************/
PUBLIC GUIFORM_TYPE_E CTRLFORM_GetType(
                                       MMI_HANDLE_T ctrl_handle
                                       );

/***************************************************************************//*!
@brief 设置容器控件类型
@author Jassmine
@param ctrl_handle [in] 控件handle
@param type        [in] 类型
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetType(
                             MMI_HANDLE_T       ctrl_handle,
                             GUIFORM_TYPE_E     type
                             );

/***************************************************************************//*!
@brief 获得容器控件风格
@author Jassmine
@param ctrl_handle [in] 控件handle
@return 风格
@note 
*******************************************************************************/
PUBLIC GUIFORM_STYLE_E CTRLFORM_GetStyle(
                                         MMI_HANDLE_T   ctrl_handle
                                         );

/***************************************************************************//*!
@brief 设置容器控件风格
@author Jassmine
@param ctrl_handle [in] 控件handle
@param style       [in] 风格
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetStyle(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIFORM_STYLE_E   style
                              );

/***************************************************************************//*!
@brief 设置控件背景
@author Jassmine
@param form_handle [in] 控件handle
@param bg_ptr      [in] 背景
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetBg(
                              MMI_HANDLE_T   form_handle,//in:
                              GUI_BG_T       *bg_ptr     //in:
                              );

/***************************************************************************//*!
@brief 设置是否保存背景rgb565数据
@author Jassmine
@param form_handle [in] 控件handle
@param is_save     [in] 是否保存
@return 操作是否成功
@note 可以减少背景解码次数,提高显示速度,但是消耗内存
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSaveBg(
                                  MMI_HANDLE_T   form_handle,//in:
                                  BOOLEAN        is_save     //in:
                                  );

/***************************************************************************//*!
@brief 设置控件padding
@author Jassmine
@param form_handle [in] 控件handle
@param padding_ptr [in] 间隔
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetPadding(
                                   MMI_HANDLE_T     form_handle,
                                   GUI_PADDING_T    *padding_ptr
                                   );

/***************************************************************************//*!
@brief 设置控件padding
@author Jassmine
@param form_handle      [in] 控件handle
@param padding_space    [in] 间隔
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetPaddingEx(
                                     MMI_HANDLE_T   form_handle,
                                     uint16         padding_space
                                     );

/***************************************************************************//*!
@brief 获得指定子控件的Id
@author Jassmine
@param form_handle  [in] 控件handle
@param child_index  [in] 子控件索引
@return 控件Id
@note 
*******************************************************************************/
PUBLIC MMI_CTRL_ID_T CTRLFORM_GetChildCtrlId(
                                             MMI_HANDLE_T   form_handle,    //in:
                                             uint16         child_index     //in:
                                             );

/***************************************************************************//*!
@brief 设置Form控件的分割线
@author Jassmine
@param form_handle  [in] 控件handle
@param line_width   [in] 分割线宽度,0不显示
@param line_color   [in] 分割线颜色
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetDividingLine(
                                        MMI_HANDLE_T    form_handle,//in:
                                        uint16          line_width, //in:0 don't display dividing line
                                        GUI_COLOR_T     line_color  //in:
                                        );

/***************************************************************************//*!
@brief 设置容器控件的所有子控件水平或垂直间距
@author Jassmine
@param form_handle   [in] 控件handle
@param hor_space_ptr [in] 水平间距,可以为空
@param ver_space_ptr [in] 垂直间距,可以为空
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSpace(
                                 MMI_HANDLE_T    form_handle,    //in:
                                 uint16          *hor_space_ptr, //in:may PNULL
                                 uint16          *ver_space_ptr  //in:may PNULL
                                 );

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/***************************************************************************//*!

@brief 
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@param layout_type [in] layout_type
@return NONE
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetLayoutType(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIFORM_LAYOUT_E   layout_type
                              );

/***************************************************************************//*!
@brief 
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@return GUIFORM_LAYOUT_E
@note 
*******************************************************************************/
PUBLIC GUIFORM_LAYOUT_E CTRLFORM_GetLayoutType(
                              MMI_HANDLE_T      ctrl_handle
                              );


#endif
/***************************************************************************//*!
@brief 设置子控件相对于前一个子控件的水平或垂直间距
@author Jassmine
@param form_handle   [in] 控件handle
@param child_handle  [in] 子控件handle
@param hor_space_ptr [in] 水平间距,可以为空
@param ver_space_ptr [in] 垂直间距,可以为空
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildSpace(
                                      MMI_HANDLE_T   form_handle,    //in:
                                      MMI_HANDLE_T   child_handle,   //in:
                                      uint16         *hor_space_ptr, //in:may PNULL
                                      uint16         *ver_space_ptr  //in:may PNULL
                                      );

/***************************************************************************//*!
@brief 设置子控件宽度
@author Jassmine
@param form_handle  [in] 控件handle
@param child_handle [in] 子控件handle
@param width_ptr    [in] 宽
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildWidth(
                                      MMI_HANDLE_T           form_handle,    //in:
                                      MMI_HANDLE_T           child_handle,   //in:
                                      GUIFORM_CHILD_WIDTH_T  *width_ptr      //in:
                                      );

/***************************************************************************//*!
@brief 设置子控件高度
@author Jassmine
@param form_handle  [in] 控件handle
@param child_handle [in] 子控件handle
@param height_ptr   [in] 高
@return 操作是否成功
@note 默认高度根据宽度计算得到
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildHeight(
                                       MMI_HANDLE_T              form_handle,    //in:
                                       MMI_HANDLE_T              child_handle,   //in:
                                       GUIFORM_CHILD_HEIGHT_T    *height_ptr     //in:
                                       );

/***************************************************************************//*!
@brief 设置正常子控件的背景,边框或者字体
@author Jassmine
@param form_handle  [in] 控件handle
@param bg_ptr       [in] 背景
@param border_ptr   [in] 边框
@param font_ptr     [in] 字体
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildTheme(
                                      MMI_HANDLE_T   form_handle,    //in:
                                      GUI_BG_T       *bg_ptr,        //in/out:may PNULL
                                      GUI_BORDER_T   *border_ptr,    //in/out:may PNULL
                                      GUI_FONT_ALL_T *font_ptr       //in/out:may PNULL
                                      );

/***************************************************************************//*!
@brief 设置激活子控件的背景,边框或者字体
@author Jassmine
@param form_handle  [in] 控件handle
@param bg_ptr       [in] 背景
@param border_ptr   [in] 边框
@param font_ptr     [in] 字体
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetActiveChildTheme(
                                            MMI_HANDLE_T     form_handle,    //in:
                                            GUI_BG_T         *bg_ptr,        //in/out:may PNULL
                                            GUI_BORDER_T     *border_ptr,    //in/out:may PNULL
                                            GUI_FONT_ALL_T   *font_ptr       //in/out:may PNULL
                                            );

/***************************************************************************//*!
@brief 设置恢化子控件的背景,边框或者字体
@author Jassmine
@param form_handle  [in] 控件handle
@param bg_ptr       [in] 背景
@param border_ptr   [in] 边框
@param font_ptr     [in] 字体
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetGrayChildTheme(
                                          MMI_HANDLE_T       form_handle,    //in:
                                          GUI_BG_T           *bg_ptr,        //in/out:may PNULL
                                          GUI_BORDER_T       *border_ptr,    //in/out:may PNULL
                                          GUI_FONT_ALL_T     *font_ptr       //in/out:may PNULL
                                          );

/***************************************************************************//*!
@brief 设置设置容器控件的大小变化范围
@author Jassmine
@param form_handle  [in] 控件handle
@param min_height   [in] 最小值,默认值为0
@param max_height   [in] 最大值,默认值为0xffff
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSizeRange(
                                     MMI_HANDLE_T   form_handle,    //in:
                                     MMI_HANDLE_T   child_handle,   //in:
                                     uint16         min_height,     //in:
                                     uint16         max_height      //in:
                                     );

/***************************************************************************//*!
@brief 设置容器控件所有子控件排列方式
@author Jassmine
@param form_handle  [in] 控件handle
@param align        [in] 子控件显示排列方式
@return 操作是否成功
@note 只针对子控件并排排列的容器控件有效
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetAlign(
                                 MMI_HANDLE_T           form_handle,    //in:
                                 GUIFORM_CHILD_ALIGN_E  align           //in:
                                 );

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/***************************************************************************//*!
@brief set the align for all child, support in layout order only.
@author ningbiao.huang
@param form_handle[in] : form ctrl handle
@param child_align[in] : child_align for set
@return GUIFORM_CHILD_ALIGN_E
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetAllChildAlign(
	MMI_HANDLE_T      ctrl_handle,
	GUIFORM_CHILD_ALIGN_E  child_align           //in:
);

/***************************************************************************//*!
@brief get the align for all child,support in layout order only.
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@return GUIFORM_CHILD_ALIGN_E
@note 
*******************************************************************************/
PUBLIC GUIFORM_CHILD_ALIGN_E CTRLFORM_GetChildAlign(
                              MMI_HANDLE_T      ctrl_handle
                              );
#endif
/***************************************************************************//*!
@brief 设置子控件排列方式
@author Jassmine
@param form_handle  [in] 控件handle
@param child_handle [in] 子控件handle
@param align        [in] 子控件显示排列方式
@return 操作是否成功
@note 只针对顺序排列的子控件有效
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildAlign(
                                      MMI_HANDLE_T           form_handle,    //in:
                                      MMI_HANDLE_T           child_handle,   //in:
                                      GUIFORM_CHILD_ALIGN_E  align           //in:
                                      );

/***************************************************************************//*!
@brief 获得子控件显示方式
@author Jassmine
@param form_handle      [in] 控件handle
@param child_handle     [in] 子控件handle
@param display_type_ptr [in/out] 显示方式
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetChildDisplay(
                                        MMI_HANDLE_T             form_handle,        //in:
                                        MMI_HANDLE_T             child_handle,       //in:
                                        GUIFORM_CHILD_DISPLAY_E  *display_type_ptr   //in/out:
                                        );

/***************************************************************************//*!
@brief 设置子控件显示方式
@author Jassmine
@param form_handle  [in] 控件handle
@param child_handle [in] 子控件handle
@param display_type [in] 显示方式
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDisplay(
                                        MMI_HANDLE_T             form_handle,
                                        MMI_HANDLE_T             child_handle,
                                        GUIFORM_CHILD_DISPLAY_E  display_type
                                        );

/***************************************************************************//*!
@brief 设置子控件显示方式,只设置,不排版,不刷新
@author Jassmine
@param form_handle  [in] 控件handle
@param child_handle [in] 子控件handle
@param display_type [in] 显示方式
@return 操作是否成功
@note 在form未排版之前设置,可以提高速度,比如open,full paint msg设置
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDisplayOnly(
                                            MMI_HANDLE_T            form_handle,
                                            MMI_HANDLE_T            child_handle,
                                            GUIFORM_CHILD_DISPLAY_E display_type
                                            );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : set child control dock type
//  Global resource dependence : 
//  Author: Jassmine
//  Note: only set one control dock type
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDock(
                                     MMI_HANDLE_T           form_handle,
                                     MMI_HANDLE_T           child_handle,
                                     GUIFORM_CHILD_DOCK_E   dock_type
                                     );
#endif

/***************************************************************************//*!
@brief 设置容器控件的所有子控件是否允许FORM设置其背景
@author Jassmine
@param form_handle [in] 控件handle
@param is_permit   [in] 是否允许设置背景
@return 操作是否成功
@note 默认TRUE
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildBg(
                                      MMI_HANDLE_T   form_handle,
                                      BOOLEAN        is_permit
                                      );

/***************************************************************************//*!
@brief 设置容器控件的所有子控件是否允许FORM设置其边框
@author Jassmine
@param form_handle [in] 控件handle
@param is_permit   [in] 是否允许设置边框
@return 操作是否成功
@note 默认TRUE
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildBorder(
                                          MMI_HANDLE_T   form_handle,
                                          BOOLEAN        is_permit
                                          );

/***************************************************************************//*!
@brief 设置容器控件的所有子控件是否允许FORM设置其字体大小
@author Jassmine
@param form_handle [in] 控件handle
@param is_permit   [in] 是否允许设置字体
@return 操作是否成功
@note 默认TRUE
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildFont(
                                        MMI_HANDLE_T     form_handle,
                                        BOOLEAN          is_permit
                                        );

/*****************************************************************************/
//  Description : set form all child is permit form set it's font color
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildFontColor(
                                        MMI_HANDLE_T     form_handle,
                                        BOOLEAN          is_permit
                                        );

/***************************************************************************//*!
@brief 设置容器控件是否支持滑动
@author Jassmine
@param form_handle [in] 控件handle
@param is_support  [in] 是否允许滑动
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsSlide(
                                MMI_HANDLE_T   form_handle,
                                BOOLEAN        is_support
                                );

/***************************************************************************//*!
@brief 是否根据语言自动调整子控件显示的相对位置
@author Jassmine
@param form_handle [in] 控件handle
@param is_auto     [in] 是否自动调整显示的相对位置
@return 操作是否成功
@note 一般从左至右显示,阿拉伯等语言则从右往左显示
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsAutoDir(
                                  MMI_HANDLE_T   form_handle,
                                  BOOLEAN        is_auto
                                  );

/***************************************************************************//*!
@brief 是否循环处理上下键
@author Jassmine
@param form_handle [in] 控件handle
@param is_circular [in] 是否循环处理上下键
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetCircularHandleUpDown(
                                                MMI_HANDLE_T    form_handle,
                                                BOOLEAN         is_circular
                                                );

/***************************************************************************//*!
@brief 是否循环处理左右键
@author Jassmine
@param form_handle [in] 控件handle
@param is_circular [in] 是否循环处理左右键
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetCircularHandleLeftRight(
                                                   MMI_HANDLE_T  form_handle,
                                                   BOOLEAN       is_circular
                                                   );

/***************************************************************************//*!
@brief 获得控件的高度
@author Jassmine
@param form_handle [in] 控件handle
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC uint16 CTRLFORM_GetHeight(
                                 MMI_HANDLE_T    form_handle //in:
                                 );

/***************************************************************************//*!
@brief 强制进行排版
@author Jassmine
@param form_handle  [in] form控件handle
@param is_update    [in] 是否需要刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_ForceLayout(
                                    MMI_HANDLE_T    form_handle //in:
                                    );

/***************************************************************************//*!
@brief 动态创建容器控件
@author Jassmine
@param win_handle  [in] 窗口handle
@param form_handle [in] 控件handle
@param layout_type [in] 子控件排列方式
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CreatDynaCtrl(
                                      MMI_HANDLE_T       win_handle, //in:
                                      MMI_HANDLE_T       form_handle,//in:
                                      GUIFORM_LAYOUT_E   layout_type //in:
                                      );

/***************************************************************************//*!
@brief 动态创建容器控件子控件
@author Jassmine
@param win_handle     [in] 窗口handle
@param form_handle    [in] 父控件handle
@param child_data_ptr [in] 子控件初始数据
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CreatDynaChildCtrl(
                                           MMI_HANDLE_T          win_handle,     //in:
                                           MMI_HANDLE_T          form_handle,    //in:
                                           GUIFORM_DYNA_CHILD_T  *child_data_ptr //in:
                                           );

/***************************************************************************//*!
@brief 动态销毁容器控件子控件
@author Jassmine
@param form_handle  [in] 父控件handle
@param child_handle [in] 子控件handle
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_DestroyDynaChildCtrl(
                                             MMI_HANDLE_T   form_handle,    //in:
                                             MMI_HANDLE_T   child_handle    //in:
                                             );

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/***************************************************************************//*!
@brief Add a exist ctrl to form_ctrl
@author ningbiao.huang
@param win_handle  [in]  win handle
@param form_handle  [in] parent form handle
@param child_handle [in] child ctrl handle
@return TRUE:pass FALSE:fail
@note 
*******************************************************************************/
#endif
#ifdef GUI_DUMP_CTRL_TREE
/*****************************************************************************/
//  Description : dump control tree by form ptr
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_DumpTree(
                              MMI_HANDLE_T  form_handle
                              );
#endif

/**--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
