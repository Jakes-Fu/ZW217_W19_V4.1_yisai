/*! @file guigraph.h
@brief 此文件提供了基础图形的绘制
@author Jassmine
@version 1.0
@date 04/26/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Graph Graph

(暂无)

*******************************************************************************/

/*! @addtogroup graphGrp Graph
@brief 图形绘制模块
@details
该模块提供基础图形的绘制
@{
*******************************************************************************/


#ifndef _GUIGRAPH_H_
#define _GUIGRAPH_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "dal_lcd.h"
#include "dal_display.h"

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
#define RGB5652ARGB8888(rgb565,alpha) ( ((alpha)<<24) |((((rgb565) >> 8) & 0xf8)<<16)|((((rgb565) >> 3) & 0xfc)<<8)|(((rgb565) << 3)  & 0xf8))
#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @enum LCD_COPY_MODE_E
@brief 绘制方式
*/
typedef enum
{
    OVER_MODE,                      /*!< 覆盖方式 */
    XOR_MODE                        /*!< 异或方式 */
}LCD_COPY_MODE_E;   

/*! @enum GUILCD_PEN_STYLE_E
@brief 线的风格
*/
typedef enum
{
    GUILCD_PEN_STYLE_DEFAULT,       /*!< 默认值，直线 */
    GUILCD_PEN_STYLE_DOT,           /*!< 点线 */
    GUILCD_PEN_STYLE_DASH,          /*!< 划线 */
    GUILCD_PEN_STYLE_DASH_DOT,      /*!< 点划线 */
    NUM_OF_GUILCD_PEN_STYLE
}GUILCD_PEN_STYLE_E;


/*! @enum MMI_ARROW_TYPE_E
@brief 箭头类型
*/
typedef enum
{
    ARROW_LEFT,                     /*!< 左箭头 */
    ARROW_UP,                       /*!< 上箭头 */
    ARROW_RIGHT,                    /*!< 右箭头 */
    ARROW_DOWN,                     /*!< 下箭头 */
    ARROW_DOUBLE_LEFT,              /*!< 暂不支持 */
    ARROW_DOUBLE_UP,                /*!< 暂不支持 */
    ARROW_DOUBLE_RIGHT,             /*!< 暂不支持 */
    ARROW_DOUBLE_DOWN               /*!< 暂不支持 */
} MMI_ARROW_TYPE_E;        


/*! @enum GUILCD_CONVERT_POS_TYPE_E
@brief 坐标点转换类型
*/
typedef enum
{
    GUILCD_CONVERT_POS_LCD2DISP,    /*!< 相对于lcd坐标转换为相对于图层坐标 */
    GUILCD_CONVERT_POS_DISP2LCD,    /*!< 相对于图层坐标转换为相对于lcd坐标 */

    GUILCD_CONVERT_POS_MAX
} GUILCD_CONVERT_POS_TYPE_E;            


/*! @struct GUILCD_DRAW_DC_T
@brief 描述LCD buffer的信息
@details
mem_width和width的区别：\n
mem_width指的是buffer的宽度，代表一行有多少个象素，width是用户指定的显示宽度。
这两个值可以不相等，便最终只能绘制在buffer的区域之内，即会被裁剪在buffer的范围之内。\n
mem_width常用于绘制时跳转到下一行所加的偏移量。\n
宽度值都是以象素为单位，一个象素有多少个字节，是由数据格式data_type表示。
*/
typedef struct 
{
    uint32 mem_width;               /*!< buffer的宽度 */
    uint32 width;                   /*!< 需要显示的宽度 */
    uint32 height;                  /*!< buffer高度 */
    void *lcd_buffer_ptr;           /*!< buffer地址 */
    DISPLAY_DATA_TYPE_E data_type;  /*!< 数据格式 */
}GUILCD_DRAW_DC_T;

/*! @struct GUILCD_DRAW_ALPHA_LINE_T
@brief 画带alpha值的线
@details
*/
typedef struct 
{
    uint16         color;         //color to draw.
    uint16         width;         //线宽
    uint8          alpha;         //颜色alpha
    BOOLEAN        is_circle_end; //圆角线头和线尾
}GUILCD_DRAW_ALPHA_LINE_T;


/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 绘制直线
@author Jim.zhang
@param dev_info_ptr [in] 图层信息
@param x1           [in] 起点的x坐标
@param y1           [in] 起点的y坐标
@param x2           [in] 终点的x坐标
@param y2           [in] 终点的y坐标
@param color        [in] 颜色值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawLine( 
                         GUI_LCD_DEV_INFO const *dev_info_ptr,  // block information
                         uint16                 x1,             //x coordinate of first point
                         uint16                 y1,             //y coordinate of first point
                         uint16                 x2,             //x coordinate of second point
                         uint16                 y2,             //y coordinate of second point
                         GUI_COLOR_T            color           //color to draw.
                         );


/***************************************************************************//*!
@brief 画带有绘制方式的直线
@author Jim.zhang
@param dev_info_ptr [in] 图层信息
@param x1           [in] 起点的x坐标
@param y1           [in] 起点的y坐标
@param x2           [in] 终点的x坐标
@param y2           [in] 终点的y坐标
@param color        [in] 颜色值
@param copy_mode    [in] 绘制方式
@return 
@note
*******************************************************************************/
PUBLIC void LCD_DrawLineExt( 
                            GUI_LCD_DEV_INFO const *dev_info_ptr,   // block information
                            uint16                  x1,             //x coordinate of first point
                            uint16                  y1,             //y coordinage of first point
                            uint16                  x2,             //x coordinate of second point
                            uint16                  y2,             //y coordinate of sconcd point
                            GUI_COLOR_T             color,          //color to draw.
                            LCD_COPY_MODE_E         copy_mode       //copy mode
                            );
#endif

/***************************************************************************//*!
@brief 绘制水平直线
@author Jim.zhang
@param dev_info_ptr [in] 图层信息
@param x1           [in] 起点的x坐标
@param y            [in] y坐标
@param x2           [in] 终点的x坐标
@param color        [in] 颜色值
@return 
@note
*******************************************************************************/
PUBLIC void LCD_DrawHLine( 
                          GUI_LCD_DEV_INFO const *dev_info_ptr, // block information
                          uint16                    x1,             //x coordinate of first point
                          uint16                    y,              //y coordinate of first point
                          uint16                    x2,             //x coordinate of second point
                          GUI_COLOR_T               color           //color to draw.    
                          );

/***************************************************************************//*!
@brief 绘制垂直直线
@author Jim.zhang
@param dev_info_ptr [in] 图层信息
@param x            [in] x坐标
@param y1           [in] 起点的y坐标
@param y2           [in] 终点的y坐标
@param color        [in] 颜色值
@return 
@note
*******************************************************************************/
PUBLIC void LCD_DrawVLine(
                          GUI_LCD_DEV_INFO const *dev_info_ptr, // block information
                          uint16                    x,              //x coordinate of first point
                          uint16                    y1,             //y coordinate of first point
                          uint16                    y2,             //y coordinate of second point
                          GUI_COLOR_T               color           //color to draw the line.   
                          );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 绘制带线宽的直线
@author Geng.Ren
@param dev_info_ptr [in] 图层信息
@param x1           [in] 起点的x坐标
@param y1           [in] 起点的y坐标
@param x2           [in] 终点的x坐标
@param y2           [in] 终点的y坐标
@param color        [in] 颜色值
@param width        [in] 线宽
@return 
@note  
*******************************************************************************/
PUBLIC void LCD_DrawThickLine( 
                              GUI_LCD_DEV_INFO  *dev_info_ptr,         //ID of LCD.
                              uint16 x1,             //x coordinate of first point
                              uint16 y1,             //y coordinage of first point
                              uint16 x2,             //x coordinate of second point
                              uint16 y2,             //y coordinate of sconcd point
                              GUI_COLOR_T    color,     //color to draw.
                              uint16            width //unit: pixel
                              );

/***************************************************************************//*!
@brief 绘制带线宽的风格线
@author James.Zhang
@param dev_info_ptr [in] 图层信息
@param x1           [in] 起点的x坐标
@param y1           [in] 起点的y坐标
@param x2           [in] 终点的x坐标
@param y2           [in] 终点的y坐标
@param color        [in] 颜色值
@param width        [in] 线宽
@param style        [in] 线的风格
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawThickLineEx( 
                                GUI_LCD_DEV_INFO  *dev_info_ptr,   //lcd info
                                int16              x1,             //x coordinate of first point
                                int16              y1,             //y coordinage of first point
                                int16              x2,             //x coordinate of second point
                                int16              y2,             //y coordinate of sconcd point
                                GUI_COLOR_T        color,          //color to draw.
                                uint16             width,          //unit: pixel
                                GUILCD_PEN_STYLE_E style
                                );

/*****************************************************************************/
//  Description:  draw thick line ex
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DrawThickLineEx_NonCap( 
                                GUI_LCD_DEV_INFO  *dev_info_ptr,   //lcd info
                                int16              x1,             //x coordinate of first point
                                int16              y1,             //y coordinage of first point
                                int16              x2,             //x coordinate of second point
                                int16              y2,             //y coordinate of sconcd point
                                GUI_COLOR_T        color,          //color to draw.
                                uint16             width,          //unit: pixel
                                GUILCD_PEN_STYLE_E style
                                );
#endif

/***************************************************************************//*!
@brief 在指定图层中填充矩形
@author Jim.zhang
@param dev_info_ptr [in] 图层信息
@param box          [in] 绘制区域
@param color        [in] 颜色值
@return 
@note
*******************************************************************************/
PUBLIC void LCD_FillRect(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         GUI_COLOR_T                color           //color to fill
                         );

/***************************************************************************//*!
@brief RGB565转换为ARGB888
@author Jassmine
@param color [in] 颜色值
@param alpha [in] 透明度
@return ARGB888颜色值
@note 
*******************************************************************************/
PUBLIC uint32 ConvertRgb565ToArgb888(
                                     uint16     color,
                                     uint16     alpha
                                     );

/***************************************************************************//*!
@brief 在指定buffer中填充矩形
@author jibin
@param lcdbuff_ptr  [in] buffer地址
@param lcdbuff_size [in] buffer大小
@param lcd_width    [in] buffer宽度
@param lcd_height   [in] buffer高度
@param box          [in] 绘制区域
@param color        [in] 颜色值
@return 
@note
必须满足lcdbuff_size = lcd_width * lcd_height
*******************************************************************************/
PUBLIC void LCD_FillRectTobuffer( 
                                 GUI_COLOR_T *lcdbuff_ptr,  //out
                                 uint32 lcdbuff_size,
                                 uint16 lcd_width,
                                 uint16 lcd_height,  
                                 GUI_RECT_T box,    //rectange to fill color
                                 GUI_COLOR_T color  //color to fill
                                 );

/***************************************************************************//*!
@brief 绘制矩形边框
@author Jim.zhang
@param dev_info_ptr [in] 图层信息
@param box   [in] 绘制区域
@param color [in] 颜色值
@return 
@note
*******************************************************************************/
PUBLIC void LCD_DrawRect( 
                         GUI_LCD_DEV_INFO const *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to draw
                         GUI_COLOR_T                color           //color to draw
                         );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 画指定区域内绘制三角形
@author Geng.Ren
@param lcd_dev_info [in] 图层信息
@param rect         [in] 绘制区域
@param arrowtype    [in] 箭头类型
@param color        [in] 颜色值
@return 
@note
- 上下箭头，箭头所需的宽度为7，高度为4；\n
  左右箭头，箭头所需的宽度为4，高度为7
- 如果所给的绘制区域rect大于箭头所需要的大小，则居中显示
*******************************************************************************/
PUBLIC void LCD_DrawArrowInRect( 
                                GUI_LCD_DEV_INFO        *lcd_dev_info,
                                GUI_RECT_T              rect,     //rectangle to draw
                                MMI_ARROW_TYPE_E        arrowtype,    //arrow type
                                GUI_COLOR_T             color     //color to draw
                                );

/***************************************************************************//*!
@brief  在指定图层中填充弧线区域
@author James.Zhang
@param dev_info_ptr  [in] 图层信息
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param start_angle   [in] 起始角度
@param end_angle     [in] 结束角度
@param color         [in] 颜色值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillArc(
                        const GUI_LCD_DEV_INFO*   dev_info_ptr,
                        GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                        int16                     x,
                        int16                     y,
                        uint16                    radius,
                        uint16                    start_angle,   //起始角度
                        uint16                    end_angle,   //结束角度
                        GUI_COLOR_T               color
                        );

/***************************************************************************//*!
@brief 在指定buffer中填充弧线区域
@author James.Zhang
@param buffer_ptr    [in] buffer地址
@param buf_width     [in] buffer宽度
@param buf_height    [in] buffer高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param start_angle   [in] 起始角度
@param end_angle     [in] 结束角度
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillArcToBuffer(
                                      GUI_COLOR_T*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //起始角度
                                      uint16            end_angle,   //结束角度
                                      GUI_COLOR_T       color
                                      );


/***************************************************************************//*!
@brief 在指定图层中画弧线
@author James.Zhang
@param dev_info_ptr  [in] 图层信息
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param start_angle   [in] 起始角度
@param end_angle     [in] 结束角度
@param color         [in] 颜色值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawArc(
                        const GUI_LCD_DEV_INFO*   dev_info_ptr,
                        GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                        int16                     x,
                        int16                     y,
                        uint16                    radius,
                        uint16                    start_angle,   //起始角度
                        uint16                    end_angle,   //结束角度
                        GUI_COLOR_T               color
                        );

/***************************************************************************//*!
@brief 在指定buffer中画弧线
@author James.Zhang
@param buffer_ptr    [in] buffer地址
@param buf_width     [in] buffer宽度
@param buf_height    [in] buffer高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param start_angle   [in] 起始角度
@param end_angle     [in] 结束角度
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawArcToBuffer(
                                      GUI_COLOR_T*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //起始角度
                                      uint16            end_angle,   //结束角度
                                      GUI_COLOR_T       color
                                      );
#endif

/***************************************************************************//*!
@brief 在指定图层中填充圆
@author James.Zhang
@param dev_info_ptr  [in] 图层信息
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param color         [in] 颜色值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillCircle(
                           const GUI_LCD_DEV_INFO* dev_info_ptr,
                           GUI_RECT_T*             clip_rect_ptr, //剪切区域, 可以为空
                           int16                   x,
                           int16                   y,
                           uint16                  radius,
                           GUI_COLOR_T             color
                           );

/***************************************************************************//*!
@brief 在指定buffer中填充圆
@author James.Zhang
@param buffer_ptr    [in] buffer地址
@param buf_width     [in] buffer宽度
@param buf_height    [in] buffer高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillCircleToBuffer(
                                         GUI_COLOR_T*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         GUI_COLOR_T       color
                                         );
#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 在指定图层中画圆边框
@author James.Zhang
@param dev_info_ptr  [in] 图层信息
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param color         [in] 颜色值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawCircle(
                           const GUI_LCD_DEV_INFO* dev_info_ptr,
                           GUI_RECT_T*             clip_rect_ptr, //剪切区域, 可以为空
                           int16                   x,
                           int16                   y,
                           uint16                  radius,
                           GUI_COLOR_T             color
                           );


/***************************************************************************//*!
@brief 在指定buffer中画圆边框
@author James.Zhang
@param buffer_ptr    [in] buffer地址
@param buf_width     [in] buffer宽度
@param buf_height    [in] buffer高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawCircleToBuffer(
                                         GUI_COLOR_T*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         GUI_COLOR_T       color
                                         );

/***************************************************************************//*!
@brief 往指定图层中填充椭圆
@author James.Zhang
@param dev_info_ptr  [in] 图层信息
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 椭圆圆心x坐标
@param y             [in] 椭圆圆心y坐标
@param radius_x      [in] 横向半径
@param radius_y      [in] 纵向半径
@param color         [in] 颜色值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillEllipse(
                            const GUI_LCD_DEV_INFO*   dev_info_ptr,
                            GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                            int16                     x,
                            int16                     y,
                            uint16                    radius_x,
                            uint16                    radius_y,
                            GUI_COLOR_T               color
                            );

/***************************************************************************//*!
@brief 往指定buffer中填充椭圆
@author James.Zhang
@param buffer_ptr    [in] buffer地址
@param buf_width     [in] buffer宽度
@param buf_height    [in] buffer高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 椭圆圆心x坐标
@param y             [in] 椭圆圆心y坐标
@param radius_x      [in] 横向半径
@param radius_y      [in] 纵向半径
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillEllipseToBuffer(
                                          GUI_COLOR_T*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          GUI_COLOR_T       color
                                          );

/***************************************************************************//*!
@brief 往指定图层中画椭圆边框
@author James.Zhang
@param dev_info_ptr  [in] 图层信息
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 椭圆圆心x坐标
@param y             [in] 椭圆圆心y坐标
@param radius_x      [in] 横向半径
@param radius_y      [in] 纵向半径
@param color         [in] 颜色值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawEllipse(
                            const GUI_LCD_DEV_INFO*   dev_info_ptr,
                            GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                            int16                     x,
                            int16                     y,
                            uint16                    radius_x,
                            uint16                    radius_y,
                            GUI_COLOR_T               color
                            );


/***************************************************************************//*!
@brief 往指定buffer中画椭圆边框
@author James.Zhang
@param buffer_ptr    [in] buffer地址
@param buf_width     [in] buffer宽度
@param buf_height    [in] buffer高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 椭圆圆心x坐标
@param y             [in] 椭圆圆心y坐标
@param radius_x      [in] 横向半径
@param radius_y      [in] 纵向半径
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawEllipseToBuffer(
                                          GUI_COLOR_T*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          GUI_COLOR_T       color
                                          );

/***************************************************************************//*!
//brief  draw a 3D style frame
//author Geng.Ren
//param dev_info_ptr [in] 图层信息
//param x0           [in] 
//param y0           [in] 
//param x1           [in] 
//param y1           [in] 
//param fillc        [in] 
//param updown       [in] 
//return 
//note can not draw part of a 'button'
*******************************************************************************/
PUBLIC void LCD_Draw3DControlFrame(
                                   GUI_LCD_DEV_INFO      *dev_info_ptr,
                                   uint16            x0,
                                   uint16            y0,
                                   uint16            x1,
                                   uint16            y1,
                                   GUI_COLOR_T        fillc,     //color to fill the area.    
                                   BOOLEAN            updown    // 1: up; 0: down
                                   );
#endif

/***************************************************************************//*!
@brief 给指定图层混合蒙层
@author Geng.Ren
@param dev_info_ptr [in] 图层信息
@param rect         [in] 混合区域
@param color        [in] 蒙层颜色
@return 
@details 
这里的蒙层是用颜色值color的50%透明度，往目标图层上进行混合
*******************************************************************************/
PUBLIC void LCD_MistBmp(
                        GUI_LCD_DEV_INFO  *dev_info_ptr,
                        GUI_RECT_T        rect,
                        GUI_COLOR_T       color     //color to draw.
                        );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 将内存中的内容拷贝到主层中(带关键色)
@author bin.ji
@param mem_buf_ptr     [in] 内存buffer地址
@param mem_width       [in] 需要拷贝的宽度
@param mem_height      [in] 需要拷贝的高度
@param left            [in] 需要拷贝的左起点位置
@param top             [in] 需要拷贝的上起点位置
@param key_color       [in] 关键色，忽略内存中含有该颜色的点
@param is_final_lcdbuf [in] 是否拷贝到LCD buffer中
@return 
@note 
通过is_final_lcdbuf，可以控制目标buffer选择LCD，还是多层系统中的主层。主层中的数据
并没有合并其他逻辑层。
*******************************************************************************/
PUBLIC void LCD_CopyMemToMainLcdBuf(
                                    GUI_COLOR_T *mem_buf_ptr,
                                    uint32 mem_width,
                                    uint32 mem_height,
                                    uint32 left,
                                    uint32 top,
                                    GUI_COLOR_T key_color,
                                    BOOLEAN is_final_lcdbuf    //多层的情况下最终合并后的lcd_buf
                                    );
#endif

/***************************************************************************//*!
@brief 将内存中的内容拷贝到主层中
@author bin.ji
@param mem_buf_ptr     [in] 内存buffer地址
@param mem_width       [in] 需要拷贝的宽度
@param mem_height      [in] 需要拷贝的高度
@param left            [in] 需要拷贝的左起点位置
@param top             [in] 需要拷贝的上起点位置
@param is_final_lcdbuf [in] 是否拷贝到LCD buffer中
@return 
@note 
通过is_final_lcdbuf，可以控制目标buffer选择LCD，还是多层系统中的主层。主层中的数据
并没有合并其他逻辑层。
*******************************************************************************/
PUBLIC void LCD_CopyMemToMainLcdBufNoKeyColor(
                                              GUI_COLOR_T *mem_buf_ptr,
                                              uint32 mem_width,
                                              uint32 mem_height,
                                              uint32 left,
                                              uint32 top,
                                              BOOLEAN is_final_lcdbuf    //多层的情况下最终合并后的lcd_buf
                                              );

/***************************************************************************//*!
@brief  将主层内容拷贝到buffer中
@author bin.ji
@param mem_buf_ptr     [in] 内存buffer地址
@param mem_width       [in] 需要拷贝的宽度
@param mem_height      [in] 需要拷贝的高度
@param left            [in] 需要拷贝的左起点位置
@param top             [in] 需要拷贝的上起点位置
@param is_final_lcdbuf [in] 是否是LCD buffer的内容
@return 
@note 
通过is_final_lcdbuf，可以控制目标buffer选择LCD，还是多层系统中的主层。主层中的数据
并没有合并其他逻辑层。
*******************************************************************************/
PUBLIC void LCD_CopyMainLcdBufToMem(
                                    GUI_COLOR_T *mem_buf_ptr,
                                    uint32 mem_width,
                                    uint32 mem_height,
                                    uint32 left,
                                    uint32 top,
                                    BOOLEAN is_final_lcdbuf    //多层的情况下最终合并后的lcd_buf
                                    );

/*****************************************************************************/
//  Description : copy layer buf to mem
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void LCD_CopyLayerBufToMem(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr,
                                  GUI_COLOR_T       *mem_buf_ptr,
                                  uint32            mem_width,
                                  uint32            mem_height,
                                  uint32            left,
                                  uint32            top
                                  );

/*****************************************************************************/
//  Description : copy mem to layer buf
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC void LCD_CopyMemToLayerBuf(
                                  GUI_LCD_DEV_INFO  *lcd_dev_ptr,
                                  GUI_COLOR_T       *mem_buf_ptr,
                                  uint32            mem_width,
                                  uint32            mem_height,
                                  uint32            left,
                                  uint32            top
                                  );

/***************************************************************************//*!
@brief 区域相对于屏幕和图层的转换
@author bin.ji
@param convert_type [in] 转换类型
@param dev_info_ptr [in] 图层信息
@param in_rect_ptr  [in] 原区域
@param out_rect_ptr [in] 转换后的区域
@return 转换后的区域，如果in_rect_ptr或out_rect_ptr为空，则返回PNULL
@note
由于层的位置可以改变，所以坐标点相对于屏幕和图层是不一致的，可以通过该方法来相互转换。
*******************************************************************************/
PUBLIC GUI_RECT_T *LCD_ConvertDispRect(
                                       GUILCD_CONVERT_POS_TYPE_E convert_type,
                                       GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                       const GUI_RECT_T *in_rect_ptr,       //in
                                       GUI_RECT_T       *out_rect_ptr       //in
                                       );

/***************************************************************************//*!
@brief 坐标点相对于屏幕和图层的转换
@author bin.ji
@param convert_type  [in] 转换类型
@param dev_info_ptr  [in] 图层信息
@param in_point_ptr  [in] 原坐标点
@param out_point_ptr [in] 转换后的坐标点
@return 转换后的坐标点，如果in_point_ptr或out_point_ptr为空，则返回PNULL
@details
由于层的位置可以改变，所以坐标点相对于屏幕和图层是不一致的，可以通过该方法来相互转换。
*******************************************************************************/
PUBLIC GUI_POINT_T *LCD_ConvertDispPoint(
                                       GUILCD_CONVERT_POS_TYPE_E convert_type,
                                       GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                       const GUI_POINT_T *in_point_ptr,       //in
                                       GUI_POINT_T       *out_point_ptr       //in
                                       );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 在指定的buffer上填充带透明信息的矩形
@author James.Zhang
@param buf_ptr    [in] 目标buffer的地址
@param buf_width  [in] buffer一行的宽度
@param lcd_width  [in] LCD的宽度
@param lcd_height [in] LCD的高度
@param box        [in] 显示区域
@param color      [in] 颜色值
@param alpha      [in] 透明度
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillDestBuffArgbRect(
                         GUI_COLOR_T *buf_ptr,
                         uint32       buf_width,
                         uint32       lcd_width,
                         uint32       lcd_height,
                         GUI_RECT_T   box,
                         GUI_COLOR_T  color,
                         uint8        alpha
                         );
#endif

/***************************************************************************//*!
@brief  填充带透明信息的矩形
@author James.Zhang
@param dev_info_ptr [in] 图层信息
@param box          [in] 绘制区域
@param color        [in] 颜色值
@param alpha        [in] 透明值
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillArgbRect(
                             GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                             GUI_RECT_T                 box,            //rectangle to fill color
                             GUI_COLOR_T                color,          //color to fill
                             uint8                      alpha
                             );

/***************************************************************************//*!
@brief 获取LCD信息
@author  bin.ji
@param dev_info_ptr [in] 指定LCD
@param draw_dc_ptr  [in/out] 指向LCD信息的地址
@return 是否调用成功
@note
该方法一般在往buffer里绘制前调用，用于获取buffer的信息。
*******************************************************************************/
PUBLIC BOOLEAN LCD_PrepareDrawParam(
                                 const GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                 GUILCD_DRAW_DC_T       *draw_dc_ptr       //out
                                 );


#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 填充弧线区域
@author James.Zhang
@param buffer_ptr    [in] 指定目标buffer
@param buf_width     [in] 目标buffer的宽度
@param buf_height    [in] 目标buffer的高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param start_angle   [in] 起始角度
@param end_angle     [in] 结束角度
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillArcToBufferU32(
                                      uint32*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //起始角度
                                      uint16            end_angle,   //结束角度
                                      uint32       color
                                      );

/***************************************************************************//*!
@brief 绘制弧线
@author James.Zhang
@param buffer_ptr    [in] 指定目标buffer
@param buf_width     [in] 目标buffer的宽度
@param buf_height    [in] 目标buffer的高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 圆心x坐标
@param y             [in] 圆心y坐标
@param radius        [in] 半径
@param start_angle   [in] 起始角度
@param end_angle     [in] 结束角度
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawArcToBufferU32(
                                      uint32*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //起始角度
                                      uint16            end_angle,   //结束角度
                                      uint32       color
                                      );
#endif

/***************************************************************************//*!
@brief 填充圆区域
@author James.Zhang
@param buffer_ptr     [in] 指定目标buffer
@param buf_width      [in] 目标buffer的宽度
@param buf_height     [in] 目标buffer的高度
@param clip_rect_ptr  [in] 剪裁区，可以为空
@param x              [in] 圆心x坐标
@param y              [in] 圆心y坐标
@param radius         [in] 半径
@param color          [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillCircleToBufferU32(
                                         uint32*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         uint32       color
                                         );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 绘制椭圆边框
@author James.Zhang
@param buffer_ptr    [in] 指定目标buffer
@param buf_width     [in] 目标buffer的宽度
@param buf_height    [in] 目标buffer的高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 椭圆圆心x坐标
@param y             [in] 椭圆圆心y坐标
@param radius_x      [in] 横向半径
@param radius_y      [in] 纵向半径
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawEllipseToBufferU32(
                                          uint32*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          GUI_COLOR_T       color
                                          );

/***************************************************************************//*!
@brief 绘制圆边框
@author James.Zhang
@param buffer_ptr     [in] 指定目标buffer
@param buf_width      [in] 目标buffer的宽度
@param buf_height     [in] 目标buffer的高度
@param clip_rect_ptr  [in] 剪裁区，可以为空
@param x              [in] 圆心x坐标
@param y              [in] 圆心y坐标
@param radius         [in] 半径
@param color          [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawCircleToBufferU32(
                                         uint32*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         uint32       color
                                         );

/***************************************************************************//*!
@brief 填充椭圆区域
@author James.Zhang
@param buffer_ptr    [in] 指定目标buffer
@param buf_width     [in] 目标buffer的宽度
@param buf_height    [in] 目标buffer的高度
@param clip_rect_ptr [in] 剪裁区，可以为空
@param x             [in] 椭圆圆心x坐标
@param y             [in] 椭圆圆心y坐标
@param radius_x      [in] 横向半径
@param radius_y      [in] 纵向半径
@param color         [in] 颜色值
@return 实际绘制的区域
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillEllipseToBufferU32(
                                          uint32*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          uint32       color
                                          );
#endif

/***************************************************************************//*!
@brief 填充矩形区域
@author Jim.zhang
@param dev_info_ptr [in] 图层信息
@param box          [in] 绘制区域
@param u32_color        [in] 颜色值
@return 
@note
目标LCD必须是DATA_TYPE_ARGB888格式，否则调用无效
*******************************************************************************/
PUBLIC void LCD_FillRectU32(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         uint32                     u32_color       //color to fill
                         );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief 计算透明颜色的叠加值
@author James.Zhang
@param u32_color [in] ARGB888颜色值
@param u16_color [in] RGB565颜色值
@param alpha     [in] 附加给u16_color的透明度
@return 叠加之后的ARGB888颜色值
@note
u16_color与alpha形成一个带透明信息的颜色，叠加到u32_color
*******************************************************************************/
PUBLIC uint32 GUI_CalcAlphaAdd( 
                               uint32 u32_color,
                               uint16 u16_color,
                               uint8 alpha
                               );
#endif

/***************************************************************************//*!
@brief 计算两个颜色叠加后的值
@author hua.fang
@param down_color [in] 下面的ARGB888颜色值
@param up_color [in] 上面的ARGB888颜色值
@return 叠加之后的ARGB888颜色值
@note
*******************************************************************************/
PUBLIC uint32 GUI_CompoundARGB888(
                                  uint32    down_color,
                                  uint32    up_color
                                  );

/***************************************************************************//*!
@brief 绘制圆角矩形边框
@author Jassmine
@param dev_info_ptr [in] 图层信息
@param box          [in] 绘制区域
@param display_rect [in] 剪裁区
@param color        [in] 颜色值
@return 
@note
线宽为2个象素，且绘制区域的宽度>=10,高度>=12
*******************************************************************************/
PUBLIC void LCD_DrawRoundedRect(
                                GUI_LCD_DEV_INFO const  *dev_info_ptr,
                                GUI_RECT_T              box,
                                GUI_RECT_T              display_rect,
                                GUI_COLOR_T             color
                                );

/***************************************************************************//*!
@brief 绘制圆角矩形边框到指定buffer
@author Jassmine
@param buf_ptr      [in] buffer指针
@param buf_width    [in] buffer宽度
@param buf_height   [in] buffer高度
@param box          [in] 绘制区域
@param color        [in] 颜色值
@return 
@note
线宽为2个象素，且绘制区域的宽度>=10,高度>=12
*******************************************************************************/
PUBLIC void LCD_DrawRoundedRectToBuf(
                                     GUI_COLOR_T    *buf_ptr,
                                     uint16         buf_width,
                                     uint16         buf_height,
                                     GUI_RECT_T     box,
                                     GUI_COLOR_T    color
                                     );

/***************************************************************************//*!
@brief 绘制带阴影的圆角矩形边框
@author Lianxiang
@param dev_info_ptr [in] 图层信息
@param box          [in] 绘制区域
@param display_rect [in] 剪裁区
@param color        [in] 颜色值
@return 
@note
线宽为2个象素，且绘制区域的宽度>=10,高度>=12
*******************************************************************************/
PUBLIC void LCD_DrawShadowRoundedRect(
                                      GUI_LCD_DEV_INFO const    *dev_info_ptr,
                                      GUI_RECT_T                box,
                                      GUI_RECT_T                display_rect,
                                      GUI_COLOR_T               color
                                      );

/***************************************************************************//*!
@brief 绘制带阴影的圆角矩形边框到指定buffer
@author Jassmine
@param buf_ptr      [in] buffer指针
@param buf_width    [in] buffer宽度
@param buf_height   [in] buffer高度
@param box          [in] 绘制区域
@param color        [in] 颜色值
@return 
@note
线宽为2个象素，且绘制区域的宽度>=10,高度>=12
*******************************************************************************/
PUBLIC void LCD_DrawShadowRoundedRectToBuf(
                                           GUI_COLOR_T  *buf_ptr,
                                           uint16       buf_width,
                                           uint16       buf_height,
                                           GUI_RECT_T   box,
                                           GUI_COLOR_T  color
                                           );

/***************************************************************************//*!
@brief  填充圆角矩形
@author Jassmine
@param dev_info_ptr [in] 图层信息
@param box          [in] 绘制区域
@param display_rect [in] 剪裁区
@param color        [in] 颜色值
@return 
@note
绘制区域的宽度>=10,高度>=12
*******************************************************************************/
PUBLIC void LCD_FillRoundedRect(
                                GUI_LCD_DEV_INFO const  *dev_info_ptr,
                                GUI_RECT_T              box,
                                GUI_RECT_T              display_rect,
                                GUI_COLOR_T             color
                                );

/***************************************************************************//*!
@brief 填充带阴影的圆角矩形
@author Jassmine
@param dev_info_ptr [in] 图层信息
@param box          [in] 绘制区域
@param display_rect [in] 剪裁区
@param color        [in] 颜色值
@return 
@note 
绘制区域的宽度>=10,高度>=12
*******************************************************************************/
PUBLIC void LCD_FillShadowRoundedRect(
                                      GUI_LCD_DEV_INFO const    *dev_info_ptr,
                                      GUI_RECT_T                box,
                                      GUI_RECT_T                display_rect,
                                      GUI_COLOR_T               color
                                      );

//#if defined(MMI_PDA_SUPPORT)
#ifdef MMI_3DUI_SUPPORT
#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//   Description : Draw a line between 2 specific point to LCD buffer 
//                      whose width is WIDTH. 
//   Global resource dependence : none
//   Author:
//   Note: 1.???üó??úí?2??-?±??
//            2.ò?×???3y?a3Y′|àí
//            3.ê??ù?Y±3?°é?à′×???3y?a3Y′|àí
/*****************************************************************************/
PUBLIC void LCD_DrawLayerLine( 
                              GUI_LCD_DEV_INFO  *dev_info_ptr,         //ID of LCD.
                              int16 x1,             //x coordinate of first point
                              int16 y1,             //y coordinage of first point
                              int16 x2,             //x coordinate of second point
                              int16 y2,             //y coordinate of sconcd point
                              uint32    color,     //color to draw.
                              uint16            width //unit: pixel
                              );

/*****************************************************************************/
//   Description : Draw a line between 2 specific point to LCD buffer 
//                      whose width is WIDTH. 
//   Global resource dependence : none
//   Author:
//   Note:
//
//
/*****************************************************************************/
PUBLIC void LCD_DrawAlphaLine( 
                              GUI_LCD_DEV_INFO  *dev_info_ptr,
                              GUI_POINT_T start_point,
                              GUI_POINT_T end_point,
                              GUILCD_DRAW_ALPHA_LINE_T alpha_line_info
                              );
#endif
#endif
//#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LCD_FillRectToBuf(
                              GUI_COLOR_T *lcdbuf_ptr,
                              uint32 buf_width,
                              uint32 lcd_width,
                              uint32 lcd_height,
                              GUI_RECT_T box,
                              GUI_COLOR_T color
                              );

/*****************************************************************************/
//  Description:  fill roundrect
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_FillRectToBufU32(
                                 uint32 *lcdbuf_ptr,
                                 uint32 buf_width,
                                 uint32 lcd_width,
                                 uint32 lcd_height,
                                 GUI_RECT_T box,
                                 uint32 color
                                 );

/*****************************************************************************/
//   Description : Draw a line between 2 specific point to LCD buffer 
//                      whose width is WIDTH. 
//   Global resource dependence : none
//   Author:
//   Note:
//
//
/*****************************************************************************/
PUBLIC BOOLEAN LCD_FillPolygon( 
                              GUI_LCD_DEV_INFO  *dev_info_ptr,
                              GUI_RECT_T        fill_rect,
                              uint32            start_color,
                              uint32            end_color
                              );

//加入ARGB888清屏接口【必须】
PUBLIC void LCD_FillRectARGB888(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         uint32                color           //color to fill
                         );

//yangyu add begin
PUBLIC BOOLEAN GetLcdInfoByLcdDev_All(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              uint32 *mem_width_ptr,    //out
                              uint32 *width_ptr,    //out
                              uint32 *height_ptr,    //out
                              uint32 data_type,     ////0: RGB565, 1: ARGB888
                              GUI_COLOR_T **lcd_buffer_pptr   //out
                              );

PUBLIC GUI_COLOR_T* GetLcdInfoByLcdDev_Ex(
                              GUI_LCD_DEV_INFO const *dev_info_ptr
                              );                              
//yangyu end
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
