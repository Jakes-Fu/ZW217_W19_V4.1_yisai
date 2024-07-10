/*! @file guistring.h
@brief 此文件介绍了string控件的对外接口和数据类型 
@author Xiaoqing.lu
@version 1.0
@date 07/24/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 07/24/2008     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_String String

(暂无)

*******************************************************************************/

/*! @addtogroup stringGrp String
@brief string控件
@details string控件
@{
*******************************************************************************/

#ifndef _GUI_STRINT_H_
#define _GUI_STRINT_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "guilcd.h"
#include "guicommon.h"
#include "mmi_font.h"
#include "guifont.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
/*! @typedef GUISTR_STATE_T
@brief 字符串状态
*/
typedef uint32 GUISTR_STATE_T;

/*! @def GUISTR_STATE_NONE
@brief none状态，即初始化状态，此时全部使用默认值
*/
#define GUISTR_STATE_NONE           0x00000000  // 当状态为0的时候，全部使用默认值
/*! @def GUISTR_STATE_ALIGN
@brief 使用对齐方式。如果不设置该状态，则无论对齐方式如何设置，都将使用左上角对齐
*/
#define GUISTR_STATE_ALIGN          0x00000001  // 使用对齐方式
/*! @def GUISTR_STATE_ANGLE
@brief 使用需按角度旋转。如果不设置该状态，即使设置了角度，也不会旋转
*/
#define GUISTR_STATE_ANGLE          0x00000002  // 使用需按角度旋转
/*! @def GUISTR_STATE_EFFECT
@brief 使用特殊效果。如果不设置该状态，则对应的特殊效果不起作用
*/
#define GUISTR_STATE_EFFECT         0x00000004  // 使用特殊效果
/*! @def GUISTR_STATE_WORDBREAK
@brief 对于多行文本，则会按字换行，默认不会按字换行
*/
#define GUISTR_STATE_WORDBREAK      0x00000008  // 对于多行文本，则会按字换行，默认不会按字换行
/*! @def GUISTR_STATE_SINGLE_LINE
@brief 显示单行文本，如果该位不设置，则显示多行文本
*/
#define GUISTR_STATE_SINGLE_LINE    0x00000010  // 显示单行文本，如果该位不设置，则显示多行文本

/*! @def GUISTR_STATE_BOLD
@brief 文本粗体字
*/
#define GUISTR_STATE_BOLD           0x00000040  // 文本粗体字
/*! @def GUISTR_STATE_ITALIC
@brief 文本显示斜体字
*/
#define GUISTR_STATE_ITALIC         0x00000080  // 文本显示斜体字
/*! @def GUISTR_STATE_UNDERLINE
@brief 文本显示下划线
*/
#define GUISTR_STATE_UNDERLINE      0x00000100  // 文本显示下划线
/*! @def GUISTR_STATE_BACKGROUND
@brief 文本显示背景色
*/
#define GUISTR_STATE_BACKGROUND     0x00000200  // 文本显示背景色
/*! @def GUISTR_STATE_NOT_HALF_CHAR
@brief 单行显示的时候，是否显示半个字的情况。
*/
#define GUISTR_STATE_NOT_HALF_CHAR  0x00000400  // 单行显示的时候，是否显示半个字的情况。
/*! @def GUISTR_STATE_NO_HALF_LINE
@brief 显示的时候，是否显示半行的情况。
*/
#define GUISTR_STATE_NO_HALF_LINE   0x00000800  // 显示的时候，是否显示半行的情况。
/*! @def GUISTR_STATE_ELLIPSIS
@brief 设置该状态表示显示的时候，如果显示不全,则显示渐变
*/
#define GUISTR_STATE_ELLIPSIS       0x00001000  // 设置该状态表示显示的时候，如果显示不全,则显示渐变,单行有效,多行时考虑到计算速度的问题,不予支持
/*! @def GUISTR_STATE_ALIGN_BY_DIR
@brief 根据文字显示方向改变对齐方式
*/
#define GUISTR_STATE_ALIGN_BY_DIR   0x00002000  // change alignment by direction
/*! @def GUISTR_STATE_INVERSE
@brief 反色显示
*/
#define GUISTR_STATE_INVERSE        0x00004000  // 反色显示
/*! @def GUISTR_STATE_SPEC_FONTCOLOR
@brief 特殊颜色文本
*/
#define GUISTR_STATE_SPEC_FONTCOLOR 0x00008000  // 特殊颜色文本
/*! @def GUISTR_STATE_GRADUAL_HOR
@brief 水平渐变
*/
#define GUISTR_STATE_GRADUAL_HOR      0x00010000    //水平渐变
/*! @def GUISTR_STATE_GRADUAL_VER
@brief 垂直渐变
*/
#define GUISTR_STATE_GRADUAL_VER      0x00020000    //垂直渐变
/*! @def GUISTR_STATE_WORDSPACE
@brief 词间距
*/
#define GUISTR_STATE_WORDSPACE        0x00040000    //词间距

/*! @def GUISTR_STATE_FROM_HEAD
@brief 从头部显示, 用于向左滚动
*/
#define GUISTR_STATE_FROM_HEAD      0x00080000  // 从尾部显示, 用于向右滚动

/*! @def GUISTR_STATE_FROM_TAIL
@brief 从尾部显示, 用于向右滚动
*/
#define GUISTR_STATE_FROM_TAIL      0x00100000  // 从尾部显示, 用于向右滚动

/*! @def GUISTR_STATE_ELLIPSIS_EX
@brief 设置该状态表示显示的时候，如果显示不全,则显示省略点
*/
#define GUISTR_STATE_ELLIPSIS_EX    0x00200000  // 设置该状态表示显示的时候，如果显示不全,则显示省略点,单行有效,多行时考虑到计算速度的问题,不予支持

// 下面是文本显示的方向,注意:文本显示的方向
/*! @def GUISTR_TEXT_DIR_LTR
@brief 文本显示的方向：从左向右
*/
#define GUISTR_TEXT_DIR_LTR            (0)            //从左向右
/*! @def GUISTR_TEXT_DIR_RTL
@brief 文本显示的方向：从右向左
*/
#define GUISTR_TEXT_DIR_RTL            (1)            //从右向左
/*! @def GUISTR_TEXT_DIR_AUTO
@brief 文本显示的方向：由文本自身决定，例如阿拉伯语自右向左，中英文从左到右
*/
#define GUISTR_TEXT_DIR_AUTO           (-1)           //由文本自身决定

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! @typedef GUISTR_BUF_TYPE_T
@brief 定义文本buffer的格式类型
*/
typedef uint16 GUISTR_BUF_TYPE_T;
/*! @def GUISTR_BUF_RGB565
@brief RGB565格式
*/
#define GUISTR_BUF_RGB565  0   //default
/*! @def GUISTR_BUF_ARGB888
@brief ARGB888格式
*/
#define GUISTR_BUF_ARGB888 1   //argb8888

typedef enum
{
    GUISTR_TEXT_ID,
    GUISTR_TEXT_BUFFER,
}GUISTR_TEXT_TYPE_E;

typedef struct  
{
	GUISTR_TEXT_TYPE_E text_type;
	MMI_TEXT_ID_T	   text_id;
	MMI_STRING_T       str_info;			
}GUISTR_TEXT_INFO_T;

/*! @struct  GUISTR_BUF_T 
@brief  string buffer，表示一段文字所显示在的区域的数据信息
*/
typedef struct  
{
    void             *buffer;    /*!< 文字buffer*/
    uint16            mem_width; /*!< 内存大小*/
    uint16            width;     /*!< 区域宽度*/
    uint16            height;    /*!< 区域高度*/
    GUISTR_BUF_TYPE_T type;      /*!< 文字buffer格式*/
}GUISTR_BUF_T;

/*! @struct  GUIFONT_REGION_STYLE_T 
@brief   特殊文本效果 目前只支持反显
*/
typedef struct
{
    uint16  start;							/*!< 起始点*/
    uint16  length;							/*!< 长度*/
    GUISTR_STATE_T  state;			/*!< 字符串状态*/
    GUI_COLOR_T     font_color; /*!< 特殊文本的颜色,注意，必须设置state |= GUISTR_STATE_SPEC_FONTCOLOR才有效*/
}GUIFONT_REGION_STYLE_T;

/*! @struct  GUISTR_STYLE_T 
@brief   字符串的显示风格
*/
typedef struct
{
    GUI_FONT_T	     font;       /*!< 字体大小*/
    GUI_COLOR_T	     font_color; /*!< 字体颜色*/
    GUI_ALIGN_E      align;      /*!< 对齐方式*/
    GUI_ANGLE_E      angle;      /*!< 角度*/
    GUIFONT_EFFECT_T effect;     /*!< 特效*/
    GUI_COLOR_T      edge_color; /*!< 文字边界颜色*/
    GUI_COLOR_T      bg_color;   /*!< 文字背景颜色*/
    GUI_COLOR_T      line_color; /*!< 文字下划线颜色*/
    int16            line_space; /*!< 行间距*/
    int16            char_space; /*!< 字间距*/
    int16            word_space; /*!< 词间距*/
    GUIFONT_REGION_STYLE_T *region_ptr;/*!< 特殊文字区域信息*/
    uint16                 region_num;/*!< 特殊文字的区域数*/
    uint8       begin_alpha;    /*!< 渐变特效起始alpha值*/
    uint8       end_alpha;      /*!< 渐变特效结束alpha值*/
    uint8             edge_alpha;
} GUISTR_STYLE_T;

/*! @struct  GUISTR_INFO_T 
@brief 字符串的宽/高信息
*/
typedef struct  
{
    uint16      width;/*!< 字符串的总宽度*/
    uint16      height;/*!< 字符串的最大高度*/
}GUISTR_INFO_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 往LCD buffer上画字符串，单行显示
@author xiaoqing.lu
@param dev_info_ptr [in] 设备信息
@param point_ptr [in] 起始点
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@retval void
@return void
@note 该函数用于以起始点和lcd设备信息为基础，取右下区域，显示单行信息，不显示省略号，也不支持角度旋转
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDSingleLine( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_POINT_T      *point_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T   *style_ptr,
                                           GUISTR_STATE_T         state
                                           );


/***************************************************************************//*!
@brief 往目标buffer上画文本信息，以起始点为基准，显示单行信息
@author xiaoqing.lu
@param des_buf_ptr [in] 设备信息
@param point_ptr [in] 起始点
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@retval 区域
@return 返回显示文字的实际区域
@note 起始点的位置相对于buffer的原点，取右下区域，显示单行信息，不显示省略号，也不支持角度旋转
*******************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextSingleLine( 
                                            GUISTR_BUF_T           *des_buf_ptr,
                                            const GUI_POINT_T      *point_ptr,      //start point to display the string relative des_buf
                                            const MMI_STRING_T     *str_ptr,
                                            GUISTR_STYLE_T   *style_ptr,
                                            GUISTR_STATE_T         state
                                            );

#ifdef UI_MULTILAYER_SUPPORT
/***************************************************************************//*!
@brief 往LCD上画字符串，以显示区域、剪切区域以及lcd的区域的交集为可视区域，进行多行显示
@author  
@param dev_info_ptr [in] 设备信息
@param disp_rect_ptr [in] 显示区域
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@retval void
@return void
@note 把字符串显示在显示区域上，但最终用户看到的是显示区域、剪切区域以及lcd区域的交集
/***************************************************************************/
PUBLIC void GUISTR_DrawSingleLineGradualInRect( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_RECT_T      *rect_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           );
#endif

#ifndef GUI_STR_MINI_SUPPORT
/***************************************************************************//*!
@brief 往LCD上画字符串，以起始点为基准，右下方向，多行显示
@author xiaoqing.lu
@param dev_info_ptr [in] 设备信息
@param point_ptr [in] 起始点
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@retval void
@return void
@note 起始点的位置相对于buffer的原点，取右下区域，显示多行信息，不显示省略号，也不支持角度旋转
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDMultiLine( 
                                          const GUI_LCD_DEV_INFO *dev_info_ptr,
                                          const GUI_POINT_T      *point_ptr,
                                          const MMI_STRING_T     *str_ptr,
                                          GUISTR_STYLE_T   *style_ptr,
                                          GUISTR_STATE_T         state
                                          );


/***************************************************************************//*!
@brief 往buffer上画文本信息，以起始点为基准，以buffer的宽度和高度为界限，右下方向显示多行
@author xiaoqing.lu
@param des_buf_ptr [in] 设备信息
@param point_ptr [in] 起始点
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@retval 区域
@return 文字显示的实际区域
@note  起始点的位置相对于buffer的原点
*******************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextMultiLine( 
                                           GUISTR_BUF_T           *des_buf_ptr,
                                           const GUI_POINT_T      *point_ptr,      //start point to display the string relative des_buf
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T   *style_ptr,
                                           GUISTR_STATE_T         state
                                           );
#endif

/***************************************************************************//*!
@brief 往LCD上画字符串，以显示区域、剪切区域以及lcd的区域的交集为可视区域，进行多行显示
@author  xiaoqing.lu
@param dev_info_ptr [in] 设备信息
@param disp_rect_ptr [in] 显示区域
@param clip_rect_ptr [in] 剪切区域
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@param dir [in] 字符串显示的方向，一般取-1，自适应
@retval void
@return void
@note 把字符串显示在显示区域上，但最终用户看到的是显示区域、剪切区域以及lcd区域的交集
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDInRect( 
                                       const GUI_LCD_DEV_INFO *dev_info_ptr,
                                       const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                       const GUI_RECT_T      *clip_rect_ptr,       //用户要剪切的实际区域
                                       const MMI_STRING_T    *str_ptr,
                                       GUISTR_STYLE_T        *style_ptr,
                                       GUISTR_STATE_T        state,
                                       int8                  dir
                                       );


/***************************************************************************//*!
@brief 往buffer上画文本信息，以显示区域、剪切区域以及buffer的区域的交集为可视区域，进行多行显示
@author xiaoqing.lu
@param des_buf_ptr [in] 设备信息
@param disp_rect_ptr [in] 显示区域
@param clip_rect_ptr [in] 剪切区域
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@param dir [in] 字符串显示的方向，一般取-1，自适应
@retval buffer上的实际区域
@return buffer上的实际区域
@note 把字符串显示在显示区域上，但最终用户看到的是显示区域、剪切区域以及buffer的区域的交集
*******************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextInRect( 
                                        GUISTR_BUF_T          *des_buf_ptr,
                                        const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                        const GUI_RECT_T      *clip_rect_ptr,       //用户要剪切的实际区域
                                        const MMI_STRING_T    *str_ptr,
                                        GUISTR_STYLE_T        *style_ptr,
                                        GUISTR_STATE_T        state,
                                        int8                  dir
                                        );


/***************************************************************************//*!
@brief 往LCD上画字符串
@author xiaoqing.lu
@param dev_info_ptr [in] 设备信息
@param disp_rect_ptr [in] 显示区域
@param clip_rect_ptr [in] 剪切区域
@param x_offset [in] x方向向左移动还是向右移动由系统决定
@param y_offset [in] y方向向上移动为负值,向下移动为正值
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@retval void
@return void
@note 根据x、y方向的偏移量，以及显示区域、剪切区域、lcd原始区域的交集进行显示，x、y的便宜量是相对于显示区域的原点的偏移
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDByOffset( 
                                         const GUI_LCD_DEV_INFO *dev_info_ptr,
                                         const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                         const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                         int16                 x_offset, // x方向向左移动还是向右移动由系统决定
                                         int16                 y_offset, // y方向向上移动为负值,向下移动为正值
                                         const MMI_STRING_T    *str_ptr,
                                         GUISTR_STYLE_T        *style_ptr,
                                         GUISTR_STATE_T        state
                                         );


/***************************************************************************//*!
@brief 往buffer上画文本信息，所画的区域为
@author xiaoqing.lu
@param des_buf_ptr [in] 设备信息
@param disp_rect_ptr [in] 显示区域
@param clip_rect_ptr [in] 剪切区域
@param x_offset [in] x方向向左移动还是向右移动由系统决定
@param y_offset [in] y方向向上移动为负值,向下移动为正值
@param str_ptr [in] 字符串信息
@param style_ptr [in] 字符串显示风格
@param state [in] 字符串显示的状态
@retval 区域
@return 显示的实际区域
@note 根据x、y方向的偏移量，以及显示区域、剪切区域、lcd原始区域的交集进行显示，x、y的便宜量是相对于显示区域的原点的偏移
*******************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextByOffset( 
                                          GUISTR_BUF_T          *des_buf_ptr,
                                          const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                          const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                          int16                 x_offset, // x方向向左移动还是向右移动由系统决定
                                          int16                 y_offset, // y方向向上移动为负值,向下移动为正值
                                          const MMI_STRING_T    *str_ptr,
                                          GUISTR_STYLE_T        *style_ptr,
                                          GUISTR_STATE_T        state
                                          );

#ifndef GUI_STR_MINI_SUPPORT
/***************************************************************************//*!
@brief 根据偏移量，计算剩余字符串的特殊效果字段数
@author xiaoqing.lu
@param str_ptr [in] 原始字符串
@param src_region_style_ptr [in] 原始字符串中特殊文本的区域
@param src_region_num [in] 原始字符串中特殊文本区域的个数
@param pos [in] 计算剩余字符串时的位置，该值不能大于原始字符串的长度
@param des_region_style_pptr [in/out]计算结果字符串的区域
@param des_region_num_ptr [in/out] 计算结果字符串区域的个数
@retval true/false
@return 返回成功或失败
@note 该函数内部申请内存，必须和GUISTR_DestroyRegionStyle成对使用，否则产生内存泄漏
*******************************************************************************/
PUBLIC BOOLEAN GUISTR_CreateRegionStyleByPos(
                                             const MMI_STRING_T *str_ptr, 
                                             GUIFONT_REGION_STYLE_T *src_region_style_ptr, 
                                             uint16 src_region_num,
                                             uint16 pos,
                                             GUIFONT_REGION_STYLE_T **des_region_style_pptr, 
                                             uint16 *des_region_num_ptr
                                             );
#endif


/***************************************************************************//*!
@brief 释放特效字段内存
@author xiaoqing.lu
@param region_style_ptr [in] 字符串中特殊文本的区域
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUISTR_DestroyRegionStyle(
                                         GUIFONT_REGION_STYLE_T *region_style_ptr
                                         );

 
/***************************************************************************//*!
@brief 获得字符串的宽度
@author xiaoqing.lu
@param style_ptr [in] 字符串风格
@param str_ptr [in] 字符串
@param state [in] 字符串状态
@retval uint16
@return 字符串宽度
@note 该接口计算字符串宽度时，会考虑字符串的风格，比如加粗、描边等影响宽度的因素
*******************************************************************************/
PUBLIC uint16 GUISTR_GetStringWidth(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state
                                    );

 
/***************************************************************************//*!
@brief 获得字符串的宽高
@author xiaoqing.lu
@param style_ptr [in] 字符串风格
@param str_ptr [in] 字符串
@param state [in] 字符串状态
@param info_ptr [in/out] 字符串宽高信息
@return 返回成功或失败
@note 注意该接口与guifont中的GUI_GetStringWidth及GUI_GetStringWidthEx的区别为该接口的参数
*******************************************************************************/
PUBLIC BOOLEAN GUISTR_GetStringInfo(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state,
                                    GUISTR_INFO_T        *info_ptr //out
                                    );

 
/***************************************************************************//*!
@brief 字符串断字函数，为GUI_GetStringInfoInLine的扩展, 支持所有特效
@author James.Zhang
@param dir [in] 字符显示方向
@param str_ptr [in] 字符串buffer
@param str_len [in] 字符串长度
@param style_ptr [in] 字符串风格
@param state [in] 字符串显示状态
@param width [in] 显示区域的宽度
@param char_num [in/out] 字符数
@param pixel_num [in/out] 像素数
@retval true/false
@return 返回成功或失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUISTR_LineBreak(
                                int8                 dir,
                                const wchar          *str_ptr,
                                uint16      	     str_len,
                                const GUISTR_STYLE_T *style_ptr,
                                GUISTR_STATE_T       state,
                                uint16      	     width,
                                uint16               *char_num,
                                uint16               *pixel_num
                                );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _GUI_STRINT_H_

/*@}*/
