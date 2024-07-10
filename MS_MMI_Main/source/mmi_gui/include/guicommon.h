/*! @file guicommon.h
@brief 此文件介绍了common控件的对外接口和数据类型 
@author panda.luo
@version 1.0
@date 02/04/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 04/2004       panda.luo          Create        
*******************************************************************************/

/*! @page page_Commom Commom

(暂无)

*******************************************************************************/

/*! @addtogroup commomGrp Commom
@brief commom模块
@details commom模块
@{
*******************************************************************************/

#ifndef _GUICOMMON_H_
#define _GUICOMMON_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#define WIN_UNIT_TEST

#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "mmi_font.h"
#include "guilcd.h"
#include "guictrl.h"
#ifndef WIN32
#include "dal_jpeg.h"
#endif

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
/*! @def UNICODE_ASCII
@brief ascii code 最大宽度
*/
#define UNICODE_ASCII		0X23

/*! @def UNICODE_HANZI
@brief 汉字宽度都是一样的,用'一'代表
*/
#define UNICODE_HANZI		0x4e00

/*! @def GUI_INVALID_PARAM
@brief ...
*/
#define GUI_INVALID_PARAM(_PTR) (_PTR)

/*! @def GUI_RGB5652ARGB8888
@brief RGB565转换到ARGB888
*/
#define GUI_RGB5652ARGB8888(rgb565, alpha)  \
    (((alpha)<<24) | ((((rgb565) >> 8) & 0xf8)<<16) | ((((rgb565) >> 3) & 0xfc)<<8) | (((rgb565) << 3) & 0xf8))

/*! @def GUI_RGB8882RGB565
@brief RGB888转换到RGB565
*/
#define GUI_RGB8882RGB565(rgb888)           \
    ((((rgb888)&0xf80000)>>8) | (((rgb888)&0xfc00)>>5) | (((rgb888)&0xf8)>>3))

/*! @def GUI_RGB2RGB565
@brief 根据RGB值转换到RGB565
*/
#define GUI_RGB2RGB565(r, g, b)             \
    ((((r)&0xf8)<<8) | (((g)&0xfc)<<3) | (((b)&0xf8)>>3))

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @typedef GUI_TIMER_DURATION_T
@brief 用于timer的时间间隔(ms)
*/
typedef uint32				   	GUI_TIMER_DURATION_T;

/*! @typedef GUI_TIMER_ID_T
@brief timer id 
*/
typedef uint8				   	GUI_TIMER_ID_T;

/*!  @typedef GUI_RETURN_E
@brief GUI 控件的返回值
*/
typedef enum
{
    GUI_RET_SUCCESS,    /*!< 成功 */
    GUI_RET_RECT_SHORT, /*!< 不设置焦点 */
    GUI_RET_ERR_PARAM   /*!< 不设置焦点 */
}GUI_RETURN_E;

/*!  @typedef GUI_SHEEN_DEPTH_E
@brief 光晕深度
*/
typedef enum
{
    GUI_SHEEN_DEPTH_0 = 0,
    GUI_SHEEN_DEPTH_HIGH = GUI_SHEEN_DEPTH_0,
    GUI_SHEEN_DEPTH_1,
    GUI_SHEEN_DEPTH_2,
    GUI_SHEEN_DEPTH_3,
    GUI_SHEEN_DEPTH_4,
    GUI_SHEEN_DEPTH_5,
    GUI_SHEEN_DEPTH_6,
    GUI_SHEEN_DEPTH_7,
    GUI_SHEEN_DEPTH_8,
    GUI_SHEEN_DEPTH_9,
    GUI_SHEEN_DEPTH_LOW = GUI_SHEEN_DEPTH_9
} GUI_SHEEN_DEPTH_E;

/*! @def GUI_CHECK_RECT
@brief  检查给定的矩形是否可以在屏幕上显示
@warning 只有在DEBUG模式下才可以做此检查
*/
#ifdef _DEBUG
#define GUI_CHECK_RECT(rect)    GUI_CheckRectValid(rect)
#else
#define GUI_CHECK_RECT(rect)
#endif

/*! @struct GUI_BG_DISPLAY_T
@brief 背景设置
@note 可以设置背景显示的相关参数
*/
typedef struct
{
    BOOLEAN         is_transparent; /*!< 图片是否半透明 */
    BOOLEAN         is_form_bg;     /*!< 是否支持动画 */
    GUI_RECT_T      rect;           /*!< 所有背景区域 */
    GUI_RECT_T      display_rect;   /*!< 显示区域 */
    GUI_RECT_T      img_rect;       /*!< 图片区域  */
    MMI_HANDLE_T    ctrl_handle;    /*!< 控件handle  */
    MMI_HANDLE_T    win_handle;     /*!< 窗口handle  */
} GUI_BG_DISPLAY_T;

/*! @struct GUI_SHEEN_INPUT_T
@brief button光晕效果设置
*/
typedef struct
{
    GUI_LCD_DEV_INFO    lcd_dev;        /*!< lcd and block id*/
    GUI_RECT_T          rect;           /*!< 光晕区域  */
    GUI_SHEEN_DEPTH_E   center_depth;   /*!< 中心光晕深度 */ 
    GUI_SHEEN_DEPTH_E   side_depth;     /*!< 边缘光晕深度 */ 
} GUI_SHEEN_INPUT_T;

/*! @struct GUI_SHEEN_T
@brief 用在显示光晕效果
*/
typedef struct
{
    BOOLEAN             is_pixel_bit16; /*!< 象素点是否是16位  */
    uint16              dest_buf_width; /*!< 目标宽度  */
    void                *dest_buf_ptr;  /*!< 目标指针  */
    void                *copy_buf_ptr;  /*!< copy buffer */ 
    GUI_SHEEN_INPUT_T   input;          /*!< input */
} GUI_SHEEN_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 创建一块区域
@author panda.luo
@param    left    [in]  rect left
@param    top     [in]  rect top
@param    right   [in]  rect right
@param    bottom  [in]  rect bottom
@return 创建的区域坐标
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_CreateRect(
                                 uint16 				left,
                                 uint16 				top,
                                 uint16 				right,
                                 uint16 				bottom
                                 );

/***************************************************************************//*!
@brief 求区域1和区域2是否有相交部分 
@author bin.ji
@param      *rect_dest_ptr  [in]    相交部分的信息
@param      rect1           [in]    区域1
@param      rect2           [in]    区域2
@return 返回值FALSE没有相交部分，TRUE有相交部分
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IntersectRect(
                                 GUI_RECT_T    		*rect_dest_ptr,
                                 GUI_RECT_T    		rect1,
                                 GUI_RECT_T    		rect2
                                 );

/***************************************************************************//*!
@brief 判断区域是否为空 
@author James.Zhang
@param  rect  [in]  需要判断的区域坐标
@return 为空返回TRUE，不为空返回FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsRectEmpty(
                               GUI_RECT_T rect 
                               );

/***************************************************************************//*!
@brief 判断区域是否为0 
@author James.Zhang
@param  rect  [in]  需要判断的区域坐标
@return 为0返回TRUE，不为0返回FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsZeroRect( 
                              GUI_RECT_T rect 
                              );

/***************************************************************************//*!
@brief 判断区域坐标是否合法
@author xiaoqing.lu
@param  rect  [in]  需要判断的区域坐标
@return 坐标合法返回TRUE，不合法返回FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsInvalidRect( 
                                 GUI_RECT_T rect 
                                 );

/***************************************************************************//*!
@brief 判断区域坐标是否合法
@author xiaoqing.lu
@param  rect  [in]  需要判断的区域坐标
@return 坐标合法返回TRUE，不合法返回FALSE
@note  
@ 判断矩形区是否有效，这里会把右、下点包括进来，当成有效点，而GUI_IsInvalidRect
@ 则不包括。MOCOR平台的RECT其实是包括右下点的，理论上来说，GUI_IsInvalidRect
@ 的实现有问题，但现在rect={0,0,0,0}被视为无效，很多用GUI_IsInvalidRect判断，
@ 这里调用的地方太多，暂时不动GUI_IsInvalidRect这个函数，新加一个Ex类型
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsInvalidRectEx( 
                                   GUI_RECT_T rect 
                                   );

/***************************************************************************//*!
@brief 检查给定的矩形是否可以在屏幕上显示 
@author panda.luo
@param  rect  [in]  需要检查的区域信息
@return 可以显示返回TRUE，不可以显示返回FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_CheckRectValid(		
                                  GUI_RECT_T rect
                                  );

/***************************************************************************//*!
@brief 用指定颜色填充矩形框 
@author panda.luo
@param  dev_info_ptr   [in]    id of lcd
@param  rect    [in]    需要填充的区域
@param  color   [in]    填充的颜色
@return 无
@note   
*******************************************************************************/
PUBLIC void GUI_FillRect(
                         GUI_LCD_DEV_INFO const *dev_info_ptr,	// block information
                         GUI_RECT_T    			rect,
                         GUI_COLOR_T   			color
                         );

/***************************************************************************//*!
@brief 得到a除以b的商 
@author Jibin
@param    a   [in]  paramter a
@param    b   [in]  paramter b
@return 得到a除以b的商
@note   
*******************************************************************************/
PUBLIC uint16 GUI_Divide(
                         uint16 a,
                         uint16 b
                         );

/***************************************************************************//*!
@brief 比较两个字符串
@author lin.lin
@param    *str1       [in]  compared string1
@param    str1_len    [in]  string1 长度
@param    *str2       [in]  compared string2
@param    str2_len    [in]  string2 长度
@return 
- str1 > str2, return >0;
- str1 = str2, return =0;
- str1 < str2, return <0
@note   
*******************************************************************************/
PUBLIC int32 GUI_StringCompare(
                               uint8				*str1,
                               uint16 				str1_len,
                               uint8				*str2,
                               uint16 				str2_len
                               );

/***************************************************************************//*!
@brief 获得字符串的宽度 
@author Great.Tian
@param    *str_ptr    [in]  字符串指针
@param    str_len     [in]  字符串长度
@param    font        [in]  字符字体
@param    char_space	字间距
@return 返回字符串的宽度
@note  
*******************************************************************************/
PUBLIC uint16 GUI_CalculateStringPiexlNum(
                                          const wchar				*str_ptr,
                                          uint16     			str_len,
                                          GUI_FONT_T 			font,
                                          uint8      			char_space
                                          );

/***************************************************************************//*!
@brief 根据字符串(字体像素个数)得到行数
@author bin.ji
@param    pixelnum     [in] 像素个数
@param    *str_ptr     [in] String指针
@param    str_len      [in] String长度(Byte长度)
@param    font         [in] String的字体
@param    char_space   [in] 字间距
@param    is_byword    [in] 是否按词换行
@return 行数
@note 
*******************************************************************************/
PUBLIC uint16 GUI_CalculateStringLinesByPixelNum(
                                                 uint16                 pixelnum,
                                                 const wchar			*str_ptr,
                                                 uint16     			str_len, 
                                                 GUI_FONT_T 			font,
                                                 uint8      			char_space,
                                                 BOOLEAN                is_byword
                                                 );

/***************************************************************************//*!
@brief 根据字体像素个数得到字符行数
@author Jassmine
@param    pixelnum    [in]  像素个数
@param    w_char      [in]  String指针
@param    char_len    [in]  String长度(Byte长度)
@param    font        [in]  String的字体
@param    char_space  [in]  字间距
@return 字符行数
@note 
*******************************************************************************/
PUBLIC uint16 GUI_CalculateCharLinesByPixelNum(
                                               uint16       pixelnum,   //[IN] 像素个数
                                               wchar        w_char,     //[IN] String指针
                                               uint16       char_len,    //[IN] String长度(Byte长度)
                                               GUI_FONT_T   font,       //[IN] String的字体
                                               uint8        char_space  //[IN] 字间距
                                               );

/***************************************************************************//*!
@brief 多行显示时计算第一行第一个字符所在位置
@author bin.ji
@param line_num		[in]    行数
@param pixelnum		[in]    像素个数
@param *str_ptr		[in]    字符串指针
@param str_len		[in]    字符串长度
@param font			[in]    字体
@param char_space	[in]    字间距
@return  多行显示时计算第一行第一个字符所在位置
@note 
*******************************************************************************/
PUBLIC uint16 GUI_CalculateStringFirstPosByLines(
                                                 uint16   			line_num,
                                                 uint16    			pixelnum,
                                                 const wchar				*str_ptr,
                                                 uint16     			str_len,
                                                 GUI_FONT_T 			font,
                                                 uint8      			char_space
                                                 );

/***************************************************************************//*!
@brief 该函数用于断行,并能获取字符个数、像素个数
@author James.Zhang
@param  *str_ptr      [in]  字符指针
@param  str_len       [in]  字符串长度
@param  font          [in]  字体
@param  char_space    [in]  字间距
@param  width         [in]  宽度
@param  *char_num     [in]  字符个数
@param  *pixel_num    [in]  像素个数
@param  is_byword     [in]  是否按word
@return 断行成功，返回TRUE；失败返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_GetStringInfoInLine(
                                       const wchar		*str_ptr,
                                       uint16      		str_len,
                                       GUI_FONT_T  		font,
                                       uint8       		char_space,
                                       uint16      		width,
                                       uint16			*char_num,
                                       uint16			*pixel_num,
                                       BOOLEAN          is_byword
                                       );

/***************************************************************************//*!
@brief 传入文字的方向, 在有双向文字时断行才能更加准确
@author James.Zhang
@param dir          [in]    文字方向
@param *str_ptr     [in]    字符指针
@param str_len      [in]    字符串长度
@param font         [in]    字体
@param char_space   [in]    字间距
@param width        [in]    宽度
@param char_num     [in]    字符个数
@param pixel_num    [in]    像素个数
@param is_byword    [in]    是否按word
@return 获取一行的信息成功返回TRUE，失败返回FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_GetStringInfoInLineWithDir(
                                              int8             dir,
                                              const wchar		*str_ptr,
                                              uint16      		str_len,
                                              GUI_FONT_T  		font,
                                              uint8       		char_space,
                                              uint16      		width,
                                              uint16			*char_num,
                                              uint16			*pixel_num,
                                              BOOLEAN          is_byword
                                              );

/***************************************************************************//*!
@brief 获取一行中的字符信息(包括像素个数、字符个数)
@author Jassmine
@param w_char           [in]    w_char 
@param char_len         [in]    字符长度  
@param font             [in]    字体
@param char_space       [in]    字间距
@param width            [in]    宽度
@param char_num_ptr     [in]    字符个数指针
@param pixel_num_tr     [in]    像素个数指针
@return 无
@note 
*******************************************************************************/
PUBLIC void GUI_GetCharInfoInLine(
                                  wchar         w_char,
                                  uint16        char_len,
                                  GUI_FONT_T    font,
                                  uint8         char_space,
                                  uint16        width,
                                  uint16        *char_num_ptr,
                                  uint16        *pixel_num_tr
                                  );


/***************************************************************************//*!
@brief 判断该点是否在区域内部
@author  Robert.Lu
@param  point   [in]    需要判断的点
@param  rect    [in]    判断区域
@return 不在区域内返回FALSE, 在区域内返回TRUE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_PointIsInRect(
                                 GUI_POINT_T    point,
                                 GUI_RECT_T     rect 
                                 );

/***************************************************************************//*!
@brief 判断两个区域是否相等
@author Hui.Zhang
@param rect_1 [in]  区域1
@param rect_2 [in]  区域2
@return 相等返回TURE，不相等返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_EqualRect(
                             GUI_RECT_T    rect_1,
                             GUI_RECT_T    rect_2
                             );

/***************************************************************************//*!
@brief 判断是否是闰年
@author liqing.peng
@param year [in]    year
@return 是闰年，返回TRUE；不是闰年，返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsLeapYear(
                              uint32 year
                              );

/***************************************************************************//*!
@brief 判断A区域是否完全在B区域内
@author bin.ji
@param orig_rect    [in]    A区域
@param rect         [in]    B区域
@return A在B范围内，返回TRUE；否则返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_RectIsCovered(
                                 GUI_RECT_T    orig_rect,
                                 GUI_RECT_T    rect
                                 );

/***************************************************************************//*!
@brief 是否为按词断行字符
@author James Zhang
@param  ch    [in]    字符
@return 是按词断行，返回TRUE；否则，返回FALSE
@note 是否为按词断行字符,在按词断行时起作用,一般的标点符号都是
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsWordBreakChar(
                                   wchar ch 
                                   );

/***************************************************************************//*!
@brief 创建一个矩阵中每个Item的区域 
@author xiaoqing.lu
@param total_rect_ptr   [in]    整个区域
@param h_side_margin    [in]    水平边距
@param v_side_margin    [in]    垂直边距
@param h_item_space     [in]    水平间距
@param v_item_space     [in]    垂直间距
@param row_num          [in]    行数
@param col_num          [in]    列数
@param item_rect_ptr    [in]    输出的item 区域，是一个二位数组，行和列由line_num、row_num定义
@retval 
@return  无
@note 
*******************************************************************************/
PUBLIC void GUI_CreateMatrixRect(
                                 GUI_RECT_T *total_rect_ptr, 
                                 int16 h_side_margin,       
                                 int16 v_side_margin,        
                                 int16 h_item_space,        
                                 int16 v_item_space,        
                                 int16 row_num,            
                                 int16 col_num,            
                                 GUI_RECT_T *item_rect_ptr   
                                 );

/***************************************************************************//*!
@brief 显示边框
@author Jassmine
@param rect             [in]    边界区域
@param display_rect     [in]    显示区域
@param border_ptr       [in]    边界指针
@param lcd_dev_ptr      [in]    lcd信息指针
@return 无
@note 显示边框
*******************************************************************************/
PUBLIC void GUI_DisplayBorder(
                              GUI_RECT_T        rect,           //in:
                              GUI_RECT_T        display_rect,   //in:
                              GUI_BORDER_T      *border_ptr,    //in:
                              GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                              );

/***************************************************************************//*!
@brief 显示边框到指定buffer
@author Jassmine
@param buf_ptr      [in] buffer指针
@param buf_size     [in] buffer大小
@param buf_width    [in] buffer宽度
@param buf_height   [in] buffer高度
@param rect         [in] 边界区域
@param border_ptr   [in] 边界指针
@return 无
@note 显示边框
*******************************************************************************/
PUBLIC void GUI_DisplayBorderToBuf(
                                   GUI_COLOR_T      *buf_ptr,   //in:
                                   uint16           buf_width,  //in:
                                   uint16           buf_height, //in:
                                   GUI_RECT_T       rect,       //in:相对于buf
                                   GUI_BORDER_T     *border_ptr //in:
                                   );

/***************************************************************************//*!
@brief 显示背景
@author Jassmine
@param bg_ptr           [in]    背景信息指针
@param display_ptr      [in]    显示信息指针
@param lcd_dev_ptr      [in]    lcd信息指针
@return 背景是否绘制
@note 
*******************************************************************************/
PUBLIC void GUI_DisplayBg(
                          GUI_BG_T          *bg_ptr,        //in:
                          GUI_BG_DISPLAY_T  *display_ptr,   //in:
                          GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                          );

/*****************************************************************************//*!
@brief 显示背景
@author Jassmine
@param bg_ptr           [in]    背景信息指针
@param display_ptr      [in]    显示信息指针
@is_notify_update       [in]    当背景为GUI_BG_NONE时，是否无条件给父亲发送MSG_NOTIFY_UPDATE
@param lcd_dev_ptr      [in]    lcd信息指针
@return 背景是否绘制
@note 
/*****************************************************************************/
PUBLIC void GUI_DisplayBgExt(
							 GUI_BG_T          *bg_ptr,        //in:
							 GUI_BG_DISPLAY_T  *display_ptr,   //in:
							 BOOLEAN			is_notify_update, // 当背景为GUI_BG_NONE时，是否无条件给父亲发送MSG_NOTIFY_UPDATE
							 GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
							 );

/***************************************************************************//*!
@brief 获取中间区域
@author xiaoqing.lu
@param src_rect     [in]    原始区域信息
@param width        [in]    宽度
@param height       [in]    高度
@return 返回值一个区域,返回的区域不会超过原始区域
@note 根据输入的原始区域及中间区域的高度和宽度，求一个区域的中间区域
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_GetCenterRect(
                                    GUI_RECT_T src_rect,
                                    uint16     width,
                                    uint16     height
                                    );

/***************************************************************************//*!
@brief 获取中间区域
@author xiaoqing.lu
@param src_rect     [in]    原始区域信息
@param width        [in]    宽度
@param height       [in]    高度
@return 返回值一个区域，返回的区域可以超过原始区域
@note 根据输入的原始区域及中间区域的高度和宽度，求一个区域的中间区域
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_GetCenterRectExt(
                                       GUI_RECT_T src_rect,
                                       uint16     width,
                                       uint16     height
                                       );

/***************************************************************************//*!
@brief 设置横向或者纵向区域
@author Jassmine
@param ctrl_handle          [in]    ctrl_handle
@param rect_ptr             [in]    区域指针
@param both_rect_ptr        [in]    双向区域指针
@return 无
@note 设置横向或者纵向区域
*******************************************************************************/
PUBLIC void GUI_SetVOrHRect(
                            MMI_HANDLE_T        ctrl_handle,
                            GUI_RECT_T          *rect_ptr,      //in:
                            GUI_BOTH_RECT_T     *both_rect_ptr  //in/out:
                            );

/***************************************************************************//*!
@brief 根据显示方向检查显示区域
@author Jassmine
@param total_ptr    [in]  所有区域信息指针
@param rect_ptr     [in   所需检查区域信息指针
@return 操作是否成功
@note 根据显示方向检查显示区域
*******************************************************************************/
PUBLIC BOOLEAN GUI_CheckRectDirection(
                                      const GUI_RECT_T *total_ptr, //in:out, 总rect
                                      GUI_RECT_T       *rect_ptr   //in:out
                                      );

/***************************************************************************//*!
@brief 取两个区域的合集
@author xiaoqing.lu
@param rect1_ptr    [in]    区域1的指针
@param rect2_ptr    [in]    区域2的指针
@return 合并后的指针
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_AddupRect(
                                GUI_RECT_T *rect1_ptr,
                                GUI_RECT_T *rect2_ptr
                                );

/***************************************************************************//*!
@brief 显示光晕
@author Jassmine
@param sheen_input_ptr    [in]  区域1的指针
@return 返回\link #GUI_SHEEN_T \endlink指针给button
@note 显示光晕
*******************************************************************************/
PUBLIC GUI_SHEEN_T* GUI_DisplaySheen(
                                     GUI_SHEEN_INPUT_T  *sheen_input_ptr  //in:
                                     );

/***************************************************************************//*!
@brief 释放光晕信息
@author Jassmine
@param *sheen_ptr    [in]   光晕信息指针
@param is_recover    [in]   是否覆盖光晕效果
@return 无
@note 释放光晕信息
*******************************************************************************/
PUBLIC void GUI_ReleaseSheen(
                             GUI_SHEEN_T    *sheen_ptr, //in
                             BOOLEAN        is_recover  //in:
                             );

/*****************************************************************************/
//  Description : rect width and height is equal
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsRectSizeEqual(
                                   GUI_RECT_T    rect1,  //in
                                   GUI_RECT_T    rect2   //in
                                   );

/*****************************************************************************/
//  Description : 开始计时
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void GUI_StartTiming(void);

/*****************************************************************************/
//  Description : 打印计时
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void GUI_PrintTiming(
                            char    *tips
                            );

/*****************************************************************************/
//  Description : set alpha overwrite
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 在带alpha的图层上绘制时,true为覆盖,否则为合成. 返回值为old值
//        若无背景,可设为覆盖,能够提高图片文字的渲染速度
/*****************************************************************************/
PUBLIC BOOLEAN GUI_SetAlphaOverwrite(
                                     BOOLEAN is_overwrite
                                     );

/*****************************************************************************/
//  Description : is alpha overwrite
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 在带alpha的图层上绘制时,true为覆盖,否则为合成.
//        若无背景,可设为覆盖,能够提高图片文字的渲染速度
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsAlphaOverwrite(
                                    void
                                    );

/*****************************************************************************/
//  Description : dump memory data
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
// 打印内存数据，以16进制表示
// dump_len: 表示一行打印的个数
// dump_num: 表示总共要打印多少行
/*****************************************************************************/
PUBLIC void GUI_DumpMemData(
                            uint32  *data_ptr,
                            uint16  dump_len,
                            uint16  dump_num
                            );

#ifndef UI_LAYER_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC void GUI_WeekLayer(
                          GUI_LCD_DEV_INFO *layer_ptr, 
                          GUI_RECT_T *rect_ptr, 
                          uint32 alpha
                          );
#endif

/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
/*@}*/
