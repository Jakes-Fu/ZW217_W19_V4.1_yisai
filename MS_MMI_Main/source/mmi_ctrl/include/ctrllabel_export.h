/*! @file ctrllabel_export.h
@brief 
此文件介绍了Label控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Label 标签(Label)

-# @ref sec_label_function
-# @ref sec_label_usage
    -# @ref ssec_label_usage_def
    -# @ref ssec_label_usage_create
    -# @ref ssec_label_usage_function_list
-# @ref sec_label_app

@section sec_label_function 控件的功能

Label控件是一个文本处理控件，能够根据文字的显示位置、显示区域、大小、字体等风格把文字通过LCD接口输出给用户。Label控件能够显示单行文本，显示的文本不能被直接编辑。当文本的宽度超过控件的显示区域时，可以滚动显示文本内容。Label控件除了显示文本之外，还可以在文本的左侧显示一个图标（ICON）。Label在焦点状态时可以滚动显示，非焦点时显示省略点，当然用户可以设置无论焦点和非焦点都进行滚动。

@section sec_label_usage 控件使用

@subsection ssec_label_usage_def 宏定义解析

@code
#define CREATE_LABEL_CTRL(_ALIGN_, _CTRL_ID_) \
        MMK_CREATE_LABEL, _ALIGN_, _CTRL_ID_
@endcode

- 用于创建标签控件。
    - _ALIGN_ 文本对齐方式
    - _CTRL_ID_ 控件ID

@subsection ssec_label_usage_create 控件创建

与列表控件相同，创建Label（标签）控件时，设置控件的ID、控件显示文本的对齐方式就可以了，但是显示区域是需要在窗口的Open消息中设置的，如果支持横竖屏，还需要设置both rect，例如：

在区域lab_rect {10, 120, 200, 150}显示一个Label控件，那么需要在窗口Table中增加：
@code
CREATE_LABEL_CTRL(GUILABEL_LEFT, ctrl_id),
@endcode
然后在窗口Open消息中增加：
@code
GUILABEL_SetRect(ctrl_id, &lab_rect, FALSE); 
@endcode
@note FALSE 表示不会立即刷新。

@subsection ssec_label_usage_function_list 接口介绍

在窗口表增加CREATE_LABEL_CTRL宏，只能说明窗口已经创建了控件的实例，但并不能显示任何内容，因为用户还没有对它设置什么信息。

-# @ref GUILABEL_SetText
    - 设置Label控件的文本一般情况下在窗口的Open消息处理时设置就可以了，此时不需要立即刷新，因为后面还会接收到Paint消息，如果立即刷新了，那么可能会导致屏幕刷新不同步，出现闪屏的现象。
    - 然而在实际的应用中，有很多情况下是要及时更新Label的显示内容的，比如按某一个键更新Label内容，这个时候需要重新设置Label显示的文本，那么也需要及时地刷新了，这个时候不会出现闪屏的现象，原因是当前窗口还处在焦点状态。
    - 【实例】
@code
MMI_STRING_T    item_str = {0};
MMI_GetLabelTextByLang(TXT_CL_TIMER_LAST_CALL,&item_str);
GUILABEL_SetText(ctrl_id,&item_str,FALSE);
@endcode
    - 其中MMI_GetLabelTextByLang接口是用来获取字符串资源TXT_CL_TIMER_LAST_CALL的文本内容的。
    - 另一种情况，例如要显示一个自定义的字符串，如“hello SPRD”，那么可以直接设置：
@code
MMI_STRING_T    item_str = {0};
wchar w_text[] = {'F', 'A', 'L', 'S', 'E', 0};
item_str.wstr_ptr = w_text;
item_str.wstr_len = 6;
GUILABEL_SetText(ctrl_id,&item_str,FALSE);
@code
-# @ref GUILABEL_SetTextById
    - 【实例】
@code
GUILABEL_SetTextById(ctrl_id,TXT_CL_TIMER_LAST_CALL, FALSE);
@endcode
-# @ref GUILABEL_SetBackgroundColor
    - Label的背景支持颜色和图片背景，但是图片背景的优先级更高，也就是说一旦已经设置了图片背景，那么Label控件会优先显示图片而忽略背景色。我们现在假设没有设置Label的背景图片。
    - Label控件默认的背景颜色为MMI_TRANSPARENCE_COLOR，颜色值是0x0，为黑色，是在创建控件的时候通过MMITHEME_GetLabelStyle获取的。在平台中，黑色在显示时是被透明的色彩，换句话说，如果用户没有重新设置Label控件的背景色的话，那么控件将直接显示在窗口的背景上，请注意，这里假设没有设置背景图片。
    - Label控件显示的时候如果发现背景颜色和默认颜色相同，那么会发送Full paint消息给窗口，让窗口统一刷新背景，因为Label的文本是直接显示在窗口的背景上的，这也会导致窗口重新刷新了一次。而如果颜色不同，那么不需要刷新窗口的背景。
    - Label是可以提供滚动的显示方式的，当字符串的宽度超过了控件的显示区域的时候，Label控件将会启动一个Timer来实现滚动显示，这个时候如果没有设置背景颜色，那么每个Timer到来的时候就会重新刷新一次窗口。在很多情况下，重新刷新窗口会导致程序的效率降低，因此我建议您尽量不要让Label的字符串宽度超过Label的区域。如果必须超过Label的区域，那么请设置和透明色不相同的背景色。请记住，暂时没有两全其美的办法。
    - 当然，如果你不需要考虑程序的效率，确实希望显示在窗口背景上的Label控件滚动显示的话，那么请千万不要把SetText的接口放在窗口的Full pain消息中来调用，因为每次刷新时都要重新设置，而重新设置之后就会从头开始显示，这依然不能达到滚动显示的效果，所以我说最好的设置Label内容的方式是在窗口Open消息的时候。
    - Label是一个比较简单的控件，但是由于背景的问题，让它变得貌似复杂了许多，其实在应用过程中，你如果使用了几次，自然也就掌握了它的规律了。这里我要自我检讨一下，这个控件我没有把它实现的更方便实用，我们将在以后的开发过程中尽量修改。
    - 【实例】设置控件的背景为白色，显示效果如图 2 20。
@code
GUILABEL_SetBackgroundColor(ctrl_id,0xFFFF);
@endcode
-# GUILABEL_SetBackgroundImage
    - Label控件在默认情况下是没有背景图片的，只有默认的背景颜色，这一点在介绍设置背景颜色接口时已经有所描述。
当用户需要显示背景图片的时候，那么在控件显示之前调用该接口，所谓在控件显示之前调用，也就是说是在控件接收到ctrl paint消息之前，后面我们讲述窗口的消息流程时，会分析消息的流程。你将会看到，创建窗口时Open消息是在paint消息之前发送的，因此在open消息中设置最为保险，这也是一个一劳永逸的办法，因为Open消息只有一次，设置好之后，以后对窗口的任何操作都不会重新设置控件的背景了。其实不仅仅对Label控件，对于其他控件也一样，例如我们前文讲述的列表控件Append列表项的时候也是放在Open消息中完成的。
    - 当然，控件的背景不是不能改变的，如果你需要在操作窗口的过程中，比如按某一个键改变控件的背景，那么在处理按键时可以重新设置设置数据。
    - 【实例】
@code
GUILABEL_SetBackgroundImage(ctrl_id, img_id);
@endcode
-# GUILABEL_SetFontParam
    - Label控件的作用就是显示一个标签，其最重要的属性就是文本，关于文本的风格，包括文本的颜色、字体、字间距等内容，默认情况值是在创建控件时调用MMITHEME_GetLabelStyle接口设置的，用户可以调用GUILABEL_SetFontParam重新设置文本的显示风格。
    - 【实例】
@code
GUILABEL_SetFontParam(ctrl_id, font, color, space);
@endcode

@section sec_label_app 控件的应用

Label是标签的意思，所标示的内容无需编辑、修改。假设这样的情形，当用户需要标示一个项，例如“姓名：”、“标题：”、“日期：”等，该项标示的功能就是提示用户下面是什么内容。此时可以使用Label控件。例如在日程的编辑界面使用了许多标签控件，如图 2 20所示，其中“标题：”、“日期：”、“时间：”、“铃声：”、“类型”、“截止日期”等。
@image html label_app_example.jpg 
图 2 20Label控件的应用例图
*******************************************************************************/

/*! @addtogroup  labelGrp Label
@brief label控件
@details 
-#  单行文本显示
@{
*******************************************************************************/


#ifndef _CTRLLABEL_EXPORT_H_
#define _CTRLLABEL_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "guictrl.h"
#include "ctrlbase_export.h"
#include "guistring.h"

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
// #ifdef MMI_PDA_SUPPORT
#define GUILABEL_ALIGN_DEFAULT  GUILABEL_ALIGN_LEFT
// #else
//     #define GUILABEL_ALIGN_DEFAULT  GUILABEL_ALIGN_MIDDLE
// #endif

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum GUILABEL_ALIGN_E
@brief 标签控件排列方式
*/
typedef enum
{
    GUILABEL_ALIGN_NONE,    /*!<保留位*/
    GUILABEL_ALIGN_LEFT,    /*!<靠左*/
    GUILABEL_ALIGN_MIDDLE,  /*!<居中*/
    GUILABEL_ALIGN_RIGHT,   /*!<靠右*/
    GUILABEL_ALIGN_MAX      /*!<保留位*/
} GUILABEL_ALIGN_E;

/*! @struct GUILABEL_INIT_DATA_T
@brief 标签控件初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖屏区域*/
    GUILABEL_ALIGN_E    align;      /*!<排列方式*/
} GUILABEL_INIT_DATA_T;

enum
{
    GUILABEL_IMAGE_NONE,
    GUILABEL_IMAGE_RES,
    GUILABEL_IMAGE_BUF,
    GUILABEL_IMAGE_FILE,

    GUILABEL_IMAGE_INVALID,
};
typedef uint8 GUILABEL_IMAGE_TYPE_E;

typedef struct
{
    MMI_IMAGE_ID_T id;
} GUILABEL_DATA_IMAGE_RES_T;

//5.1.2 ????,?bmp buf
typedef struct
{
    uint8 *buf_ptr;
    uint32 size;
    uint16 width;
    uint16 height;
} GUILABEL_DATA_IMAGE_BUF_T;

//5.1.3 ????,??????????
typedef struct
{
    wchar *filename_ptr;
    uint16 filename_len;
} GUILABEL_DATA_IMAGE_FILE_T;

typedef union
{
    GUILABEL_DATA_IMAGE_RES_T res;
    GUILABEL_DATA_IMAGE_BUF_T buf;
    GUILABEL_DATA_IMAGE_FILE_T file;
} GUILABEL_DATA_IMAGE_U;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 获得文本内容
@author Jassmine
@param ctrl_id  [in] 控件id
@param text_ptr [in/out] 文本内容
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_GetText(
                                MMI_CTRL_ID_T   ctrl_id,    //in
                                MMI_STRING_T    *text_ptr   //in/out
                                );

/***************************************************************************//*!
@brief 设置文本内容
@author Jassmine
@param ctrl_id   [in] 控件id
@param text_ptr  [in] 文本内容
@param is_update [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetText(
                                MMI_CTRL_ID_T   ctrl_id,    //in
                                MMI_STRING_T    *text_ptr,  //in
                                BOOLEAN         is_update   //in
                                );

/***************************************************************************//*!
@brief 通过文本id设置文本内容
@author Jassmine
@param ctrl_id   [in] 控件id
@param text_id   [in] 文本id
@param is_update [in] 是否立即刷新
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextById(
                                    MMI_CTRL_ID_T   ctrl_id,    //in
                                    MMI_TEXT_ID_T   text_id,    //in
                                    BOOLEAN         is_update   //in
                                    );


/***************************************************************************//*!
@brief 设置文本特效
@author Jassmine
@param ctrl_id    [in] 控件id
@param region_ptr [in] 特效
@param region_num [in] 特效个数
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextRegion(
                                      MMI_CTRL_ID_T             ctrl_id,    //in:
                                      GUIFONT_REGION_STYLE_T    *region_ptr,//in:
                                      uint16                    region_num  //in:
                                      );

/***************************************************************************//*!
@brief 设置标签控件图片
@author Jassmine
@param ctrl_id  [in] 控件id
@param icon_id  [in] 图片id
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIcon(
                                MMI_CTRL_ID_T   ctrl_id,//in
                                MMI_IMAGE_ID_T  icon_id //in
                                );

/***************************************************************************//*!
@brief 设置控件页边距
@author Jassmine
@param ctrl_id      [in] 控件id
@param margin_space [in] 边距
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetMargin(
                                  MMI_CTRL_ID_T ctrl_id,    //in
                                  uint16        margin_space//in
                                  );

/*****************************************************************************/
// 	Description : set label margin
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetMarginEx(
                                    MMI_CTRL_ID_T   ctrl_id,    //in
                                    uint16          margin_lr,  //in:left and right margin
                                    uint16          margin_tb   //in:top and bottom margin
                                    );

/***************************************************************************//*!
@brief 设置控件背景
@author Jassmine
@param ctrl_id [in] 控件id
@param bg_ptr  [in] 背景
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetBg(
                              MMI_CTRL_ID_T ctrl_id,//in
                              GUI_BG_T      *bg_ptr //in
                              );

/***************************************************************************//*!
@brief 设置控件背景为颜色,颜色值
@author Jassmine
@param ctrl_id  [in] 控件id
@param bg_color [in] 颜色
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetBackgroundColor(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           GUI_COLOR_T      bg_color
                                           );

/***************************************************************************//*!
@brief 设置控件字体
@author Jassmine
@param ctrl_id    [in] 控件id
@param font       [in] 字体大小
@param font_color [in] 字体颜色
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetFont(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_FONT_T      font,       //in:
                                GUI_COLOR_T     font_color  //in:
                                );

/***************************************************************************//*!
@brief 获得控件区域
@author Jassmine
@param ctrl_id  [in]     控件id
@param rect_ptr [in/out] 区域
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_GetRect(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_RECT_T      *rect_ptr   //in/out:
                                );


/***************************************************************************//*!
@brief 设置控件区域
@author Jassmine
@param ctrl_id   [in] 控件id
@param rect_ptr  [in] 区域
@param is_update [in] 是否立即刷新
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetRect(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_RECT_T      *rect_ptr,  //in:
                                BOOLEAN         is_update   //in
                                );


/***************************************************************************//*!
@brief 设置控件触笔响应区域
@author Jassmine
@param ctrl_id  [in] 控件id
@param rect_ptr [in] 区域
@return 是否操作成功
@note only for form
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTpRect(
                                  MMI_CTRL_ID_T ctrl_id,    //in:
                                  GUI_RECT_T    *rect_ptr   //in/out:
                                  );


/***************************************************************************//*!
@brief 设置控件是否可见
@author Jassmine
@param ctrl_id    [in] 控件id
@param is_visible [in] 是否可见
@param is_update  [in] 是否立即刷新
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetVisible(
                                   MMI_CTRL_ID_T    ctrl_id,
                                   BOOLEAN          is_visible,
                                   BOOLEAN          is_update
                                   );

/*****************************************************************************/
// 	Description : set label icon Ex
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIconEx(
                                MMI_CTRL_ID_T   ctrl_id,//in
                                BOOLEAN is_bit_map,//IN
                                GUILABEL_IMAGE_TYPE_E img_type,//IN
                                GUILABEL_DATA_IMAGE_U img_data//IN
                                );
/*****************************************************************************/
// 	Description : flush label backgroud buffer
//	Global resource dependence : 
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_FlushBg(
                                MMI_CTRL_ID_T   ctrl_id
                                );

/*****************************************************************************/
//  Description : set label align
//  Parameter: [In] ctrl_id : ctrl id
//             [In] align : align
//             [Out] None
//             [Return] result :TRUE(set success) FALSE(set failed)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetAlign (
        MMI_CTRL_ID_T   ctrl_id,
        GUILABEL_ALIGN_E align
);

/*****************************************************************************/
//  Description : string is auto scroll if string width > diaplay width whether label is active or not
//  Parameter: [In] ctrl_id : ctrl id
//             [In] is_auto_scroll : is_auto_scroll
//             [Out] None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIsAutoScroll (
        MMI_CTRL_ID_T   ctrl_id,
        BOOLEAN           is_auto_scroll
);

/*****************************************************************************/
//  Description : label string is need Ellipsis
//  Parameter: [In] ctrl_id : ctrl id
//             [In] is_ellipsis_ex : is_ellipsis_ex
//             [Out] None
//  Author: miao.liu2
//  Note:当字符串宽度超过显示区域时，设置是否显示省略点
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIsEllipsisEx (
        MMI_CTRL_ID_T   ctrl_id,
        BOOLEAN           is_ellipsis_ex
);

/***************************************************************************//*!
@brief 设置控件文本到ICON边距
@author Jassmine
@param ctrl_id      [in] 控件id
@param margin_space [in] 边距
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextMargin(
                                  MMI_CTRL_ID_T ctrl_id,    //in
                                  uint16        margin_space//in
                                  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
