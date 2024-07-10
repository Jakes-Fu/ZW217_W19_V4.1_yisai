/*! @file guitext.h
@brief 此文件介绍了text控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Text 文本(Text)

-# @ref sec_text_function
-# @ref sec_text_def
-# @ref sec_text_create
-# @ref sec_text_function_list
-# @ref sec_text_function_relationship
-# @ref sec_text_app

@section sec_text_function 控件的功能

TextBox控件是一个多行文本显示控件，能够根据文字的显示位置、显示区域、大小、字体等风格把文字通过LCD接口输出给用户，它和Label控件的主要区别就是TextBox控件能够显示多行文本。文本框是用来显示文本的，它不能用来实现交互，即不提供输入编辑功能。

Textbox控件具有如下特性：
-# 能够根据字体大小，文字颜色等属性通过LCD输出接口显示个性化文字；
-# 用户可以设置文本的显示风格，如：自动滚动、显示区域边框、行高、行间距、字间距，分割线等属性；
-# 控件能够依据用户设置，如：文字显示区域、行高、行间距、字间距等属性计算出每行显示的字数以及行数等参数；
-# 控件能够计算出显示文字的页数，超过一页会显示滚动条，用户可以进行自由翻页的操作；
-# 能显示个性化的背景图片或颜色(当不设置背景图片时，可以设置控件文字背景颜色)。可以根据用户设定显示文本边框。
-# 实现触笔和按键翻页/滚动。
-# 实现复制功能
-# 字符串解析功能，即按照字符串特征，标出url、email和phone number

@section sec_text_def 宏定义解析

@code
#define CREATE_TEXT_CTRL(_CTRL_ID_) \
        MMK_CREATE_TEXT, _CTRL_ID_
@endcode

- 用于创建一个Text Box控件。
    - _CTRL_ID_ 控件ID 

@section sec_text_create 控件创建

我们仍然将本例的代码放在sample_test_ctrl.c文件中。
- 第一步：创建一个测试控件的窗口，窗口Table名称为MMI_TEST_TEXTBOX_WIN_TAB：
@code
WINDOW_TABLE( MMI_TEST_TEXTBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_TEXTBOX_WIN_ID ),
    WIN_FUNC((uint32) HandleTestTextboxWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_TEXTBOX_TITLE),
    CREATE_TEXTBOX_CTRL(
        MMI_TEST_TEXTBOX_CTRL_ID
        ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode
    - 该窗口的元素有窗口ID MMI_TEST_TEXTBOX_WIN_ID，窗口处理函数HandleTestTextboxWinMsg，窗口标题控件，Textbox控件，窗口的Softkey控件等，其中Textbox控件的区域为窗口的客户区域（即去除标题控件和Softkey的区域）。
    - 这里需要添加标题控件的文本资源以及Textbox控件ID和窗口ID，添加方式与其它控件相同，我这里不再赘述了。
- 第二步：增加窗口的处理函数：
@code
LOCAL MMI_RESULT_E HandleTestTextboxWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_TEXTBOX_CTRL_ID;
    MMI_STRING_T        string = {0};

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMI_GetLabelTextByLang(TXT_HELLO_TEST_TEXTBOX_TEXT, &string);
        GUITEXTBOX_SetText(ctrl_id , &string, FALSE);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
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
    @note Open消息时我们设置了控件为Active控件，并设置控件显示的文本信息，关于文本资源TXT_HELLO_TEST_TEXTBOX_TEXT的添加，请读者自己完成。
- 第三步：增加创建窗口的函数，同时在头文件sample_test_ctrl.h中增加函数定义
@code
PUBLIC MMI_HANDLE_T MMIHELLO_CreateTextboxWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_TEXTBOX_WIN_TAB, PNULL);
}
@endcode
- 第四步：
    - 在hello sprd窗口中增加入口（在接口HandleHelloSprdWinMsg中增加如下代码）：
@code
case MSG_APP_1:
        MMIHELLO_CreateTextboxWin();
        break;
@endcode
    - 表示在sprd窗口中按1键打开文本框测试窗口。
- 第五步：编译运行
    - 首先编译资源，因为我们增加了两个字符串资源。
    - 其次编译运行模拟器，在hello sprd窗口中按1键，打开窗口如下：
@image html text_test_win.jpg
图 2 21Textbox控件测试窗口
    @note 这里好像没有设置控件的区域？原因是本例中的Textbox显示在整个客户区域，MMK创建窗口时会自动设置这个区域，如果Layout符合用户要求，那么就不必修改。如果不符合，那么同样需要在窗口的Open消息中设置用户想要显示的区域。

@section sec_text_function_list 关键接口介绍

-# @ref GUITEXT_SetString
@note 该接口内部会重新分配空间来保存数据，因此用户传入的指针要自己释放，以防止内存泄露；另外，调用该接口之前，如果用户需要设置其属性，最好在此接口之前，因为有些属性会引发数据的重新解析，导致执行效率的下降，比如设置字体大小和显示区域。
-# @ref GUITEXT_SetRect
@note Text默认区域为去掉Title,Softkey的中间区域。不同于默认区域，则需要调用此函数，建议在GUITEXT_SetString()函数之前调用，否则，如果Textbox控件含有文本，则会引发文本数据的重新解析，导致到执行效率的下降。 
-# @ref GUITEXT_SetBorder
@note 边框是控件的基础属性，默认没有边框。
-# @ref GUITEXT_SetBg
@note 背景是控件的基础属性，默认通用背景图片。
-# @ref GUITEXT_SetFont
@note 不同于默认字体，则需要调用此函数，建议在GUITEXT_SetString()函数之前调用，否则，如果Text有字符串，则会引发字符串的重新解析，导致到执行效率的下降。。
-# @ref GUITEXT_SetAutoScroll
-# @ref GUITEXT_SetCircularHandle
@note 默认循环处理上下按键消息，用户可以直接修改mmitheme_text.c中的属性，更改整个平台Text。
-# @ref GUITEXT_SetHandleTpMsg
@note 该接口调用会导致该Textbox控件复制粘贴功能失效，请谨慎使用。
-# @ref GUITEXT_IsDisplayPrg

@section sec_text_function_relationship 接口调用关系

文本控件的接口在被调用时不具有先后顺序，但是GUITEXT_SetString最好放在最后调用，原因是该接口会根据显示区域，字体大小，是否有滚动条等计算文本的显示信息，例如行数，否则每设置一个属性都会引起文本的重新解析，导致效率下降。

另外，如果在窗口的显示过程中，应用不需要改变文本的显示风格和内容，那么这些接口应该放在窗口的Open消息中调用就可以了。

关于文本框的显示风格，都是有默认值的，通过MMITHEME_GetTextTheme接口获取，该接口在mmitheme_text.c中实现。

@section sec_text_app 控件的应用

在实际应用中，时常会有显示多行文本的需求，比如在平台游戏的说明当中，游戏的说明文字比较多，使用Textbox控件能够将其全面的呈现给用户。如前所述，如果要显示的text比较多，可以分页显示，并且显示滚动条以便用户操作。
@image html text_app_example.jpg
图 2 22Textbox控件的实际应用
*******************************************************************************/

/*! @addtogroup textGrp Text
@brief text控件
@details 
    - 自动排版,显示文本\n
    - 支持复制功能,可以高亮显示
@{
*******************************************************************************/


#ifndef _CTRLTEXT_EXPORT_H_
#define _CTRLTEXT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guictrl.h"
#include "guiprgbox.h"
#include "mmitheme_text.h"

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
// text handle
typedef struct _text_obj_tag* CTRL_TEXT_HANDLE_T;

/*! @enum  GUITEXT_SCROLL_MODE_E 
@brief 文本滚动模式
*/
typedef enum
{
    GUITEXT_SCROLL_LINE,    /*!<按行滚动*/
    GUITEXT_SCROLL_PAGE,    /*!<按页滚动*/
    GUITEXT_SCROLL_PIXEL,   /*!<按像素点滚动*/
    GUITEXT_SCROLL_RATIO,   /*!<按控件高度的百分比滚动*/
    GUITEXT_SCROLL_MAX      /*!<保留位*/
} GUITEXT_SCROLL_MODE_E;

/*! @typedef  GUITEXT_TAG_TYPE_E 
@brief 文本解析类型
*/
typedef uint32 GUITEXT_TAG_TYPE_E;

/*! @enum  guitext_tag_type 
@brief 文本解析类型
*/
enum guitext_tag_type
{
    GUITEXT_TAG_NONE        = 0x00, /*!<不解析*/
    GUITEXT_TAG_PHONENUM    = 0x01, /*!<解析电话号码*/
    GUITEXT_TAG_EMAIL       = 0x02, /*!<解析Email*/
    GUITEXT_TAG_URL         = 0x04, /*!<解析URL*/
    GUITEXT_TAG_SET         = 0x08, /*!<应用指定*/
    GUITEXT_TAG_ALL         = GUITEXT_TAG_PHONENUM | GUITEXT_TAG_EMAIL | GUITEXT_TAG_URL,
};

/*! @struct GUITEXT_INIT_DATA_T
@brief 文本初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T   both_rect;    /*!<文本横竖屏区域*/
} GUITEXT_INIT_DATA_T;

/*! @struct GUITEXT_TAG_DATA_T
@brief 用户指定标签内容
*/
typedef struct
{
    uint16      start_pos;  /*!<标签起始位置*/
    uint16      len;        /*!<标签长度*/
} GUITEXT_TAG_DATA_T;

/*! @struct GUITEXT_SET_TAG_T
@brief 用户指定标签
*/
typedef struct
{
    uint16              tag_num;    /*!<标签个数*/
    GUITEXT_TAG_DATA_T  *tag_ptr;   /*!<标签内容数组*/
} GUITEXT_SET_TAG_T;

/*! @struct GUITEXT_TAG_INFO_T
@brief 标签内容信息
*/
typedef struct
{
    wchar               *str_ptr;   /*!<标签字符的指针 */
    uint16              str_len;    /*!<标签字符的长度 */
    GUITEXT_TAG_TYPE_E  type;       /*!<标签解析类型*/
} GUITEXT_TAG_INFO_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 获得文本字符串
@author Jassmine
@param ctrl_id   [in] 控件id
@param str_ptr   [in] 文本内容和长度
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetString(
								 MMI_CTRL_ID_T	ctrl_id,    //in:
								 MMI_STRING_T	*str_ptr    //in:/out
								 );

/***************************************************************************//*!
@brief 设置文本字符串
@author Jassmine
@param ctrl_id   [in] 控件id
@param str_ptr   [in] 文本内容
@param str_len   [in] 文本长度
@param is_update [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetString(
                                 MMI_CTRL_ID_T      ctrl_id,    //in:
                                 wchar              *str_ptr,   //in:
                                 uint16             str_len,    //in:
                                 BOOLEAN            is_update   //in:
                                 );

/***************************************************************************//*!
@brief 设置控件区域
@author Jassmine
@param ctrl_handle [in] 控件handle
@param rect_ptr    [in] 区域
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetRect(
                            MMI_HANDLE_T    ctrl_handle,
                            GUI_RECT_T      *rect_ptr
                            );

/***************************************************************************//*!
@brief 设置控件页边距
@author Jassmine
@param ctrl_id		[in] 控件id
@param margin_space [in] 页边距
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetMargin(
                                 MMI_CTRL_ID_T  ctrl_id,	//in:
                                 uint16			margin_space//in:
                                 );

/*****************************************************************************/
//  Description : set text margin
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetMarginEx(
                                   MMI_CTRL_ID_T   ctrl_id,    //in
                                   uint16          margin_left,  //in:left margin
                                   uint16          margin_right,   //in:right margin
                                   uint16          margin_top,  //in:top margin
                                   uint16          margin_bottom   //in:bottom margin
                                   );

/***************************************************************************//*!
@brief 设置控件行间距
@author Jassmine
@param ctrl_id		[in] 控件id
@param margin_space [in] 行间距
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetLineSpace(
                                    MMI_CTRL_ID_T  ctrl_id,	//in:
                                    uint16		   line_space//in:
                                    );

/***************************************************************************//*!
@brief 设置控件边框,包括边框类型,宽度以及颜色
@author Jassmine
@param border_ptr [in] 边框信息
@param ctrl_id    [in] 控件id
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetBorder(
                                 GUI_BORDER_T   *border_ptr,//in
                                 MMI_CTRL_ID_T  ctrl_id     //in:
                                 );

/***************************************************************************//*!
@brief 设置文本背景
@author Jassmine
@param ctrl_id [in] 控件id
@param bg_ptr  [in] 背景信息
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetBg(
                          MMI_CTRL_ID_T     ctrl_id,
                          GUI_BG_T          *bg_ptr
                          );

/***************************************************************************//*!
@brief 设置文本字体大小或者颜色
@author Jassmine
@param ctrl_id        [in] 控件id
@param font_ptr       [in] 字体大小,PNULL则不设置
@param font_color_ptr [in] 字体颜色,PNULL则不设置
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetFont(
                            MMI_CTRL_ID_T       ctrl_id,        //in:
                            GUI_FONT_T          *font_ptr,      //in:may PNULL
                            GUI_COLOR_T         *font_color_ptr //in:may PNULL
                            );

/***************************************************************************//*!
@brief 获得当前标签信息
@author Jassmine
@param ctrl_id              [in] 控件id
@param GUITEXT_TAG_INFO_T   [in] 标签信息
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetCurTag(
                                 MMI_CTRL_ID_T          ctrl_id,    //in:
                                 GUITEXT_TAG_INFO_T     *tag_ptr    //in/out:
                                 );

/***************************************************************************//*!
@brief 设置文本解析类型
@author Jassmine
@param ctrl_id        [in] 控件id
@param parse_type     [in] 解析类型,可以是组合
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetTagType(
                               MMI_CTRL_ID_T        ctrl_id,    //in:
                               GUITEXT_TAG_TYPE_E   tag_type,   //in:
                               GUITEXT_SET_TAG_T    *set_tag_ptr//in:may PNULL,GUITEXT_TAG_SET,must set the pointer 
                               );

/***************************************************************************//*!
@brief 设置文本排列方式,默认左上对齐
@author Jassmine
@param ctrl_id [in] 控件id
@param align   [in] 排列方式
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetAlign(
                             MMI_CTRL_ID_T  ctrl_id,//in
                             GUI_ALIGN_E    align   //in
                             );

/***************************************************************************//*!
@brief 设置文本自动滚动显示
@author Jassmine
@param is_auto_scroll    [in] 是否自动滚动文本
@param scroll_period_ptr [in] 滚动时间间隔,PNULL则使用默认时间间隔
@param ctrl_id           [in] 控件id
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetAutoScroll(
                                  BOOLEAN           is_auto_scroll,     //in:
                                  uint32            *scroll_period_ptr, //in:may PNULL default
                                  MMI_CTRL_ID_T     ctrl_id             //in:
                                  );

/***************************************************************************//*!
@brief 设置文本控件是否循环处理方向按键
@author Jassmine
@param is_circular_handle [in] 是否循环处理方向按键
@param ctrl_id            [in] 控件id
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetCircularHandle(
                                      BOOLEAN       is_circular_handle, //in:
                                      MMI_CTRL_ID_T ctrl_id             //in:
                                      );

/***************************************************************************//*!
@brief 设置文本控件是否处理触笔消息
@author Jassmine
@param is_handle [in] 是否处理触笔消息
@param ctrl_id   [in] 控件id
@note 默认处理
*******************************************************************************/
PUBLIC void CTRLTEXT_SetHandleTpMsg(
                                   BOOLEAN          is_handle,  //in:
                                   MMI_CTRL_ID_T    ctrl_id     //in:
                                   );

/***************************************************************************//*!
@brief 设置文本控件是否显示滚动条
@author Jassmine
@param is_display [in] 是否显示滚动条
@param ctrl_id    [in] 控件id
@note 默认显示
*******************************************************************************/
PUBLIC void CTRLTEXT_IsDisplayPrg(
                                 BOOLEAN            is_display, //in:
                                 MMI_CTRL_ID_T      ctrl_id     //in:
                                 );

/*****************************************************************************/
// 	Description : set text is support slide,default TRUE
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
/***************************************************************************//*!
@brief 设置文本控件是否支持滑动
@author Jassmine
@param text_handle [in] 控件handle
@param is_support  [in] 是否支持滑动
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_IsSlide(
                               MMI_HANDLE_T   text_handle,
                               BOOLEAN        is_support
                               );

/***************************************************************************//*!
@brief 设置文本控件是否支持剪贴板
@author Jassmine
@param text_handle [in] 控件handle
@param is_support  [in] 是否支持剪贴板，默认为支持
@note 必须在平台支持剪贴板的情况下才有效
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetClipboardEnabled(
                                           MMI_HANDLE_T     text_handle,
                                           BOOLEAN          is_support
                                           );

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetPosInfo(
                                  MMI_CTRL_ID_T      ctrl_id,
                                  uint16             pos,                //in
                                  GUI_RECT_T         *cursor_rect_ptr    //in/out:
                                  );

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetHighlightInfo(
                                        MMI_CTRL_ID_T      ctrl_id,
                                        uint16             *highlight_min_ptr,
                                        uint16             *highlight_max_ptr
                                        );

//zmt add start
/*****************************************************************************/
//  Description : set text string display top index
//  Global resource dependence : 
//  Author: fys
//  Note:2024.06.18
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetResetTopDisplay(
                                        MMI_CTRL_ID_T ctrl_id, 
                                        int32 top_index
                                        );
//zmt add end
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
