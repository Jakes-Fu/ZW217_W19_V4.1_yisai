/*==============================================================================
File Name: mmiim_touch_draw.h
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/


#ifndef _MMIIM_TOUCH_DRAW_H_
#define _MMIIM_TOUCH_DRAW_H_





/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "guiim_base.h"

#if defined (MMI_IM_PDA_SUPPORT)

#include "mmitheme_im.h"
#include "mmiim_internal.h"

//使用565的层绘制输入法虚拟键盘
//绘制函数除DrawPoint传入参数必须为565颜色外，其他函数参数均为888参数进入，在函数内部转换为565
//这样就不需要其他同事修改主题颜色等其他参数了
//#define MMIIM_LAYER_565     

//565透明色直接使用，不要在程序中再次转换为565颜色
//当前程序对所有的绘制色均从888转换为565，导致多
//转换了一次，所以这里加一个反向的过程来还原它  
//但是在设置colorkey的时候，不要使用这个颜色    yaoguang.chen
#ifdef MMIIM_LAYER_565
#define MMIIM_UILAYER_TRANSPARENT_COLOR (0xff801000)       //UILAYER_TRANSPARENT_COLOR 565 TO 888
#else
#define MMIIM_UILAYER_TRANSPARENT_COLOR UILAYER_TRANSPARENT_COLOR
#endif

#define DC_CHECK(dc_data_ptr, rect_ptr) \
    do {\
        SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != rect_ptr); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->left >= 0); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->top >= 0); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->right < (int32)dc_data_ptr->width); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->bottom < (int32)dc_data_ptr->height); /*assert verified*/ \
    } while (0)


/*
我们的按钮是枚举的，分为字符按钮和控制按钮
每个按钮的状态有: 正常、按下、选定和未激活，我们会对每种状态定义其风格
按钮上的字符，会支持两行。
我们默认，一个按钮上最多有3个元素，一个图片和两个文字，两个文字是上下排的
图片和席面的文字一行，左右排列
我们在这个主题里面主要定义颜色、大小和形状的信息
1 颜色
    1 面板的颜色
    2 各种类型的按钮的颜色，包括按钮上字符的颜色
    3
*/
/*! @enum SHARP_TAG
@brief 形状定义
*/
enum SHARP_TAG
{
    SHARP_RECTANGLE,        /*!< 直角矩形 */
    SHARP_ROUND_RECTANGLE,  /*!< 圆角矩形 */
};
typedef uint8 SHARP_T;

/*! @enum BORDER_RELIEF_TAG
@brief 边框的效果类型
*/
enum BORDER_RELIEF_TAG
{
    RELIEF_FLAT,            /*!< 无边框 */
    RELIEF_RAISED,          /*!< 3D突起 */
    RELIEF_SUNKEN,          /*!< 3D下沉 */
    RELIEF_SOLID,           /*!< 实线框 */
    RELIEF_RIDGE,           /*!< 山脊 */
    RELIEF_GROOVE,          /*!< 凹槽 */
    RELIEF_INSET_BRIGHT,    /*!< 内边高亮 */
    RELIEF_INSET_DARK,      /*!< 内边阴暗 */
};
typedef uint8 BORDER_RELIEF_T;

/*! @enum KEY_STATE_TAG
@brief 虚拟按键的状态
*/
enum KEY_STATE_TAG
{
    KEY_STATE_ACTIVE,       /*!< 激活的，默认 */
    KEY_STATE_PRESS,         /*!< 被按下 */
    KEY_STATE_INACTIVE,     /*!< 非激活 */

    KEY_STATE_COUNT,        /*!< 键的状态数 - 之后的不要显示*/
};
typedef uint8 KEY_STATE_T;

/*! @struct BORDER_TAG
@brief 边框的定义
*/
typedef struct BORDER_TAG
{
    BORDER_RELIEF_T relief; /*!< 边框类型 */
    uint8 width;            /*!< 宽度 */
    uint32 color;           /*!< 颜色 */
    uint32 color_bright;    /*!< 效果明亮颜色 */
    uint32 color_dark;      /*!< 效果阴暗颜色 */
} BORDER_T;

/*! @enum FILL_TAG
@brief 填充的类型定义
*/
enum FILL_TAG
{
    FILL_NONE,              /*!< 不填充 */
    FILL_PURE,              /*!< 填充成纯色 */
    FILL_GRADIENT,          /*!< 填充成渐变色 */
    FILL_GRADIENT_BLOCK,    /*!< 填充成分块的渐变色 */
    FILL_GRADIENT_PURE_BLOCK,    /*!< 填充成一部分渐变色，一部分纯色 */
};
typedef uint8 FILL_T;

/*! @struct GRADIENT_DATA_TAG
@brief 渐变色填充的数据
*/
typedef struct GRADIENT_DATA_TAG
{
    uint32 c0;              /*!< 起点色 */
    uint32 c1;              /*!< 终点色 */
    int16 a;                /*!< 变化加速度 */
} GRADIENT_DATA_T;

/*! @struct GRADIENT_BLOCK_DATA_TAG
@brief 分块填充渐变色的数据
*/
typedef struct GRADIENT_BLOCK_DATA_TAG
{
    GRADIENT_DATA_T d0;     /*!< 第一块数据 */
    GRADIENT_DATA_T d1;     /*!< 第二块数据 */
    GRADIENT_DATA_T d2;     /*!< 第三块数据 */
} GRADIENT_BLOCK_DATA_T;

/*! @union FILL_DATA_TAG
@brief 填充的数据
*/
typedef union FILL_DATA_TAG
{
    GRADIENT_BLOCK_DATA_T gradient_block;   /*!< 分块渐变色数据 */
    GRADIENT_DATA_T gradient;               /*!< 渐变色数据 */
    uint32 pure;                            /*!< 纯色数据 */
} FILL_DATA_T;



#ifdef __cplusplus
extern "C"
{
#endif

    /* 绘图 */
    PUBLIC BOOLEAN _DrawLine(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        int16 x0, int16 y0, int16 x1, int16 y1,
        int16 width,
        uint32 color
    );
    PUBLIC BOOLEAN _DrawRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color
    );
    PUBLIC BOOLEAN _FillRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color
    );
    PUBLIC BOOLEAN _FillGradientRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_top,
        uint32 color_bottom,
        int16 acceleration
    );
    PUBLIC BOOLEAN _DrawBorderUneven(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawBorderSharp(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color
    );
    PUBLIC BOOLEAN _FillRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _FillGradientRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_top,
        uint32 color_bottom,
        int16 acceleration,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _FillGradientBlockRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_0_top,
        uint32 color_0_bottom,
        int16 acceleration_0,
        uint32 color_1_top,
        uint32 color_1_bottom,
        int16 acceleration_1,
        uint32 color_2_top,
        uint32 color_2_bottom,
        int16 acceleration_2,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _FillGradientPureBlockRoundRect (
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_0_top,
        uint32 color_0_bottom,
        int16 acceleration_0,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _DrawRoundBorderUneven(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawRoundBorderSharp(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawRoundBorderInset(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color_bright,
        uint32 color_dark
    );


#ifdef __cplusplus
}
#endif


#endif //defined (MMI_IM_PDA_SUPPORT) 

#endif /*_MMIIM_TOUCH_DRAW_H_*/

