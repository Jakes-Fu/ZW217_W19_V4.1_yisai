/*****************************************************************************
** File Name:      guigraph.c                                                *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "dal_img.h"
#include "mmk_type.h"
#include "guigraph.h"
#include "mmi_theme.h"
#include "sci_api.h"
#include "ui_layer.h"
#include "graphics_draw.h"
#include "graphics_3d_rotate.h"
#include "mmi_utility.h"
#ifdef WIN32
#include <math.h>
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define     MAXLINEWIDTH    5 //max line width, used by drawing thick line
#define     DASH_LEN        8
#define     SPACE_LEN       2

#define GET_R_COLOR(color)              (((color) & 0xf800)>>8)  //颜色分量：Red
#define GET_G_COLOR(color)              (((color) & 0x07e0)>>3)  //颜色分量：Green
#define GET_B_COLOR(color)              (((color) & 0x001f)<<3)  //颜色分量：Blue

#define GET_U32_R_COLOR(color)              (((color) & 0xf80000)>>16)  //颜色分量：Red
#define GET_U32_G_COLOR(color)              (((color) & 0xfc00)>>8)  //颜色分量：Green
#define GET_U32_B_COLOR(color)              ((color) & 0xf8)  //颜色分量：Blue

#define GUI_PI   3.14159265358979323846

#define FASTALPHABLEND16BIT(pixelvalsrc, pixelvaldest) \
((((pixelvalsrc)&0xF7DF)>>1)+(((pixelvalsrc)&0xE79C)>>2)+(((pixelvaldest)&0xE79C)>>2))

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*lint -e526*/
#ifdef WIN32
void my_mem16set_win32(uint16* pline, uint16 color, uint32 byte_num);
// #else
// extern void my_mem16set(uint16* pline, uint16 color, uint32 byte_num); 
#endif
/*lint +e526*/
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : fill argb rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void FillArgbRectU32(
                        GUI_COLOR_T *lcdbuf_ptr,
                        uint32       buf_width,
                        uint32       lcd_width,
                        uint32       lcd_height,
                        GUI_RECT_T   box,
                        uint32       color
                        );

/*****************************************************************************/
//     Description : Draw an arrow 
//    Global resource dependence : none
//  Author: Geng.Ren
//    Note: The size of the arrow is fixed (4x7 for up&down, 7x4 for left&right)
/*****************************************************************************/
LOCAL void DrawArrowU32(
                      uint32 *lcdbuf_ptr,
                      GUI_POINT_T startpoint,
                      uint32 lcd_width,
                      uint32 lcd_height,
                      uint32 buf_width,
                      uint32 buf_height,
                      MMI_ARROW_TYPE_E  arrowtype,
                      uint32       color
                      );

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description:  draw ellipse
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawEllipseU32( 
                             uint32*          buffer_ptr,
                             uint16           buf_width,
                             uint16           buf_height,
                             GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                             int              ix, 
                             int              iy, 
                             int              radius_x, 
                             int              radius_y,
                             GUI_COLOR_T      color,
                             BOOLEAN          is_fill
                             );

/*****************************************************************************/
// Description : draw a 3D style frame
// Global resource dependence : none
// Author: Geng.Ren
// Note: can not draw part of a 'button'
/*****************************************************************************/
LOCAL void Draw3DControlFrameU32(
                                   uint32       *buf_ptr,
                                   uint32            buf_width,
                                   uint32            lcd_width,
                                   uint32            lcd_height,
                                   GUI_RECT_T        box,
                                   uint32        fillc,     //color to fill the area.    
                                   BOOLEAN            updown    // 1: up; 0: down
                                   );
#endif

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawCircleU32(
                            uint32*      buffer_ptr,
                            uint16            buf_width,
                            uint16            buf_height,
                            GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                            int16             x,
                            int16             y,
                            uint16            radius,
                            uint32            color,
                            BOOLEAN           is_fill
                            );
#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description:  draw arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawArcU32(
                         uint32*          buffer_ptr,
                         uint16           buf_width,
                         uint16           buf_height,
                         GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                         int32            x,
                         int32            y,
                         int32            radius,
                         int32            start_angle,
                         int32            end_angle,
                         uint32           color,
                         BOOLEAN          is_fill
                         );
#endif

/*****************************************************************************/
//  Description:  draw line to buffer
//  Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawThickLineU32( 
                               uint32*           buffer_ptr,
                               uint16            buf_width,
                               uint16            buf_height,
                               int16             x1,
                               int16             y1,
                               int16             x2,
                               int16             y2,
                               uint32            color,
                               uint16            width, //unit: pixel
                               GUILCD_PEN_STYLE_E style
                               );

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawLineU32(
                    uint32             *buffer_ptr,
                    uint16             buf_width,
                    GUI_RECT_T         clip_rect, //必须先和buffer区域进行剪切
                    int16              x1,
                    int16              y1,
                    int16              x2,
                    int16              y2,
                    uint32             color,
                    GUILCD_PEN_STYLE_E style
                    );

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawVLineU32(
                     uint32*      buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x,
                     int16             y1,
                     int16             y2,
                     uint32             color,
                     GUILCD_PEN_STYLE_E style
                     );

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawHLineU32(
                     uint32*           buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x1,
                     int16             y,
                     int16             x2,
                     uint32             color,
                     GUILCD_PEN_STYLE_E style
                     );

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
// Description : draw a 3D style frame
// Global resource dependence : none
// Author: Geng.Ren
// Note: can not draw part of a 'button'
/*****************************************************************************/
LOCAL void Draw3DControlFrame(
                                   GUI_COLOR_T       *buf_ptr,
                                   uint32            buf_width,
                                   uint32            lcd_width,
                                   uint32            lcd_height,
                                   GUI_RECT_T        box,
                                   GUI_COLOR_T        fillc,     //color to fill the area.    
                                   BOOLEAN            updown    // 1: up; 0: down
                                   );
#endif

/*****************************************************************************/
//     Description : Draw an arrow 
//    Global resource dependence : none
//  Author: Geng.Ren
//    Note: The size of the arrow is fixed (4x7 for up&down, 7x4 for left&right)
/*****************************************************************************/
LOCAL void DrawArrow(
                      GUI_COLOR_T *lcdbuf_ptr,
                      GUI_POINT_T startpoint,
                      uint32 lcd_width,
                      uint32 lcd_height,
                      uint32 buf_width,
                      uint32 buf_height,
                      MMI_ARROW_TYPE_E  arrowtype,
                      GUI_COLOR_T       color
                      );

/*****************************************************************************/
//  Description : convert display rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T ConvertDispRect(
                                 GUILCD_CONVERT_POS_TYPE_E convert_type,
                                 GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                 GUI_RECT_T       rect       //in
                                 );

/*****************************************************************************/
//  Description : convert display point
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T ConvertDispPoint(
                                 GUILCD_CONVERT_POS_TYPE_E convert_type,
                                 GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                 GUI_POINT_T       point       //in
                                 );

/*****************************************************************************/
//  Description : This function is used exchange the value of two parameter.
//  Global resource dependence : none
//  Author: Jim.zhang
//  Note:exchange the two value.
/*****************************************************************************/
LOCAL void SwapVal(
                   uint16 *val1_ptr,    // first value pointer
                   uint16 *val2_ptr     // second value pointer
                   );

/*****************************************************************************/
//  Description : 根据lcd_dev_info获得宽，高，buf_ptr
//  Global resource dependence : none
//  Author: bin.ji
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLcdInfoByLcdDev(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              uint32 *mem_width_ptr,    //out
                              uint32 *width_ptr,    //out
                              uint32 *height_ptr,    //out
                              GUI_COLOR_T **lcd_buffer_pptr   //out
                              );

/*****************************************************************************/
//   Description : Draw square end cap for thick lines
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note:   
/*****************************************************************************/
LOCAL void DrawEndCap( 
                      GUI_LCD_DEV_INFO  *dev_info_ptr,         //ID of LCD.
                      uint16 x,             //x coordinate of one end point
                      uint16 y,             //y coordinage of one end point
                      GUI_COLOR_T    color,     //color to draw.
                      uint16            width
                      );

/*****************************************************************************/
//  Description:  draw line to buffer
//  Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawThickLine( 
                               GUI_COLOR_T*      buffer_ptr,
                               uint16            buf_width,
                               uint16            buf_height,
                               int16             x1,
                               int16             y1,
                               int16             x2,
                               int16             y2,
                               GUI_COLOR_T       color,
                               uint16            width, //unit: pixel
                               GUILCD_PEN_STYLE_E style
                               );

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawHLine(
                     GUI_COLOR_T*      buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x1,
                     int16             y,
                     int16             x2,
                     GUI_COLOR_T       color,
                     GUILCD_PEN_STYLE_E style
                     );

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawVLine(
                     GUI_COLOR_T*      buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x,
                     int16             y1,
                     int16             y2,
                     GUI_COLOR_T       color,
                     GUILCD_PEN_STYLE_E style
                     );

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawLine(
                    GUI_COLOR_T*       buffer_ptr,
                    uint16             buf_width,
                    GUI_RECT_T         clip_rect, //必须先和buffer区域进行剪切
                    int16              x1,
                    int16              y1,
                    int16              x2,
                    int16              y2,
                    GUI_COLOR_T        color,
                    GUILCD_PEN_STYLE_E style
                    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MainLcdFillRect(
                           GUI_COLOR_T* lcdbuf_ptr,
                           uint16 lcd_width,
                           uint16 lcd_height,
                           GUI_RECT_T box,
                           GUI_COLOR_T color
                           );

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//     Description : Draw an arrow 
//    Global resource dependence : none
//  Author: Geng.Ren
//    Note: The size of the arrow is fixed (4x7 for up&down, 7x4 for left&right)
/*****************************************************************************/
LOCAL void DRAW_ARROW(
                      GUI_LCD_DEV_INFO  *dev_info_ptr,
                      GUI_POINT_T       startpoint,
                      MMI_ARROW_TYPE_E  arrowtype,
                      GUI_COLOR_T       color
                      );

/*****************************************************************************/
//  Description:  draw arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawArc(
                         GUI_COLOR_T*     buffer_ptr,
                         uint16           buf_width,
                         uint16           buf_height,
                         GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                         int32            x,
                         int32            y,
                         int32            radius,
                         int32            start_angle,
                         int32            end_angle,
                         GUI_COLOR_T      color,
                         BOOLEAN          is_fill
                         );
#endif

/*****************************************************************************/
//  Description:  get point by arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:converting from (radius, angle) to (x, y)
/*****************************************************************************/
LOCAL void GetPointByArc(
                         int32 radius, 
                         int32 angle, 
                         int32 *out_x, 
                         int32 *out_y, 
                         int32 *out_q
                         );

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawCircle(
                            GUI_COLOR_T*      buffer_ptr,
                            uint16            buf_width,
                            uint16            buf_height,
                            GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                            int16             x,
                            int16             y,
                            uint16            radius,
                            GUI_COLOR_T       color,
                            BOOLEAN           is_fill
                            );

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description:  draw ellipse
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawEllipse( 
                             GUI_COLOR_T*     buffer_ptr,
                             uint16           buf_width,
                             uint16           buf_height,
                             GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                             int              ix, 
                             int              iy, 
                             int              radius_x, 
                             int              radius_y,
                             GUI_COLOR_T      color,
                             BOOLEAN          is_fill
                             );
#endif

/*****************************************************************************/
// 	Description : draw rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawRoundedRect(
                              GUI_LCD_DEV_INFO const    *dev_info_ptr,
                              BOOLEAN                   is_fill_rect,
                              GUI_RECT_T                box,
                              GUI_RECT_T                display_rect,
                              GUI_COLOR_T               color,
                              RECT_STYLE_E              style
                              );

/*****************************************************************************/
// 	Description : draw rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawRoundedRectToBuf(
                                   GUI_COLOR_T      *buf_ptr,
                                   uint16           buf_width,
                                   uint16           buf_height,
                                   BOOLEAN          is_fill_rect,
                                   GUI_RECT_T       box,
                                   GUI_COLOR_T      color,
                                   RECT_STYLE_E     style
                                   );

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//   Description : Draw dash line on ARGB565/ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDashLine(int16           	x1,             //x coordinate of first point
		                  int16           	y1,             //y coordinage of first point
		                  int16           	x2,             //x coordinate of second point
		                  int16           	y2,             //y coordinate of sconcd point
		                  uint32           	color,           //color to draw.
		                  uint16           	width,           //unit: pixel
		                  void           	*mem_ptr,
		                  uint16           	mem_width,
		                  uint16           	mem_height,
		                  uint32           	mem_type,         //0: rgb565, 1: argb888
		                  GUI_RECT_T       	*update_rect_ptr);

/*****************************************************************************/
//   Description : Draw dot line on ARGB565/ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDotLine(int16           x1,             //x coordinate of first point
                  int16           y1,             //y coordinage of first point
                  int16           x2,             //x coordinate of second point
                  int16           y2,             //y coordinate of sconcd point
                  uint32           color,           //color to draw.
                  uint16           width,           //unit: pixel
                  void           *mem_ptr,
                  uint16           mem_width,
                  uint16           mem_height,
                  uint32           mem_type,         //0: rgb565, 1: argb888
                  GUI_RECT_T       *update_rect_ptr);

/*****************************************************************************/
//   Description : Draw dash-dot line ARGB565/ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDashDotLine(int16           x1,             //x coordinate of first point
                      int16           y1,             //y coordinage of first point
                      int16           x2,             //x coordinate of second point
                      int16           y2,             //y coordinate of sconcd point
                      uint32           color,           //color to draw.
                      uint16           width,           //unit: pixel
                      void           *mem_ptr,
                      uint16           mem_width,
                      uint16           mem_height,
                      uint32           mem_type,         //0: rgb565, 1: argb888
                      GUI_RECT_T       *update_rect_ptr                      
                      );
#endif

/*****************************************************************************/
//   Description : Draw circle end cap for aa thick lines
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note:   
/*****************************************************************************/
LOCAL void DrawEndCapAA( 
                       int16 x,             //x coordinate of one end point
                       int16 y,             //y coordinage of one end point
                       uint32            color,     //color to draw.
                       uint16            width,
                       uint16           *mem_ptr,
                       uint16           mem_width,
                       uint16           mem_height,                
                       GUI_RECT_T       *update_rect_ptr
                       );
                       
/*****************************************************************************/
//   Description : Draw thick line on ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawThickLineAAU32(int16           x1,             //x coordinate of first point
                     int16           y1,             //y coordinage of first point
                     int16           x2,             //x coordinate of second point
                     int16           y2,             //y coordinate of sconcd point
                     uint32           color,           //color to draw.
                     uint16           width,           //unit: pixel
                     uint32           *mem_ptr,
                     uint16           mem_width,
                     uint16           mem_height,
                     GUI_RECT_T       *update_rect_ptr);

/*****************************************************************************/
//   Description : Draw thick line on RGB565 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawThickLineAA(int16           x1,             //x coordinate of first point
                     int16           y1,             //y coordinage of first point
                     int16           x2,             //x coordinate of second point
                     int16           y2,             //y coordinate of sconcd point
                     uint32           color,           //color to draw.
                     uint16           width,           //unit: pixel
                     uint16           *mem_ptr,
                     uint16           mem_width,
                     uint16           mem_height,
                     GUI_RECT_T       *update_rect_ptr);

/*****************************************************************************/
//   Description : Draw thick line on RGB565 buffer, with con-cap. without width limit
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/                     
LOCAL void DrawThickLineAA_NoCap(int16           x1,             //x coordinate of first point
                        int16           y1,             //y coordinage of first point
                        int16           x2,             //x coordinate of second point
                        int16           y2,             //y coordinate of sconcd point
                         uint32           color,           //color to draw.
                         uint16           width,           //unit: pixel
                         uint16           *mem_ptr,
                         uint16           mem_width,
                         uint16           mem_height,
                         GUI_RECT_T       *update_rect_ptr);
                         
/*****************************************************************************/
//   Description : Draw thick line on ARGB888 buffer, with con-cap. without width limit
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/                            
LOCAL void DrawThickLineAAU32_NoCap(int16           x1,             //x coordinate of first point
                        int16           y1,             //y coordinage of first point
                        int16           x2,             //x coordinate of second point
                        int16           y2,             //y coordinate of sconcd point
                         uint32           color,           //color to draw.
                         uint16           width,           //unit: pixel
                         uint32           *mem_ptr,
                         uint16           mem_width,
                         uint16           mem_height,
                         GUI_RECT_T       *update_rect_ptr); 
                         
/*****************************************************************************/
//   Description : Draw dash thick line on RGB565 buffer, with non cap
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDashLine_NoCap(int16           x1,             //x coordinate of first point
                  int16           y1,             //y coordinage of first point
                  int16           x2,             //x coordinate of second point
                  int16           y2,             //y coordinate of sconcd point
                  uint32           color,           //color to draw.
                  uint16           width,           //unit: pixel
                  uint16           *mem_ptr,
                  uint16           mem_width,
                  uint16           mem_height,
                  uint32           mem_type,         //0: rgb565, 1: argb888
                  GUI_RECT_T       *update_rect_ptr);



/*****************************************************************************/
//   Description : Draw dot thick line on RGB565 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDotLine_NoCap(int16           x1,             //x coordinate of first point
                  int16           y1,             //y coordinage of first point
                  int16           x2,             //x coordinate of second point
                  int16           y2,             //y coordinate of sconcd point
                  uint32           color,           //color to draw.
                  uint16           width,           //unit: pixel
                  uint16           *mem_ptr,
                  uint16           mem_width,
                  uint16           mem_height,
                  uint32           mem_type,         //0: rgb565, 1: argb888
                  GUI_RECT_T       *update_rect_ptr);

/*****************************************************************************/
//   Description : Draw dash dot thick line on RGB565 buffer with non cap. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDashDotLine_NoCap(int16           x1,             //x coordinate of first point
                      int16           y1,             //y coordinage of first point
                      int16           x2,             //x coordinate of second point
                      int16           y2,             //y coordinate of sconcd point
                      uint32           color,           //color to draw.
                      uint16           width,           //unit: pixel
                      uint16           *mem_ptr,
                      uint16           mem_width,
                      uint16           mem_height,
                      uint32           mem_type,         //0: rgb565, 1: argb888
                      GUI_RECT_T       *update_rect_ptr                      
                      );


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*lint -e737 -e578*/
#ifndef GUI_GRAPH_MINI_SUPPORT
/******************************************************************************/
/*! \brief Draw a line between 2 specific point to LCD buffer.
 *  \param[in]      lcd_id      ID of LCD
 *  \param[in]      x1          x coordinate of first point
 *  \param[in]      y1          y coordinate of first point
 *  \param[in]      x2          x coordinate of second point
 *  \param[in]      y2          y coordinate of second point
 *  \param[in]      color       color to draw.
 *  \author  Jim.zhang
 *  \note
 *          it will not validate to display
 */
/******************************************************************************/
PUBLIC void LCD_DrawLine( 
                         GUI_LCD_DEV_INFO const *dev_info_ptr,  // block information
                         uint16                 x1,             //x coordinate of first point
                         uint16                 y1,             //y coordinate of first point
                         uint16                 x2,             //x coordinate of second point
                         uint16                 y2,             //y coordinate of second point
                         GUI_COLOR_T            color           //color to draw.
                         )
{
    LCD_DrawLineExt(dev_info_ptr, x1, y1, x2, y2, color, OVER_MODE);
}
#endif
/*
*This macro write color value to lcd buffer,
*and if the pixel is not in lcd size, do nothing 
*/
#define WR_COLOR_IN_LCD(_lcd_ptr, _buf_width, _x, _y, _width, _height, _color)  \
{                                                       \
    if ( ((_x)<0) || ((_x)>=(_width)) || ((_y)<0) || ((_y)>=(_height)) ) return;    \
    *( (_lcd_ptr) + (_y)*(_buf_width) + (_x) ) = (_color);  \
}

/*
*This macro write color value to lcd buffer, in xor mode
*and if the pixel is not in lcd size, do nothing 
*/
#define WR_COLOR_IN_LCD_XOR(_lcd_ptr,_buf_width,_x, _y, _width, _height, _color)    \
{                                                       \
    if ( ((_x)<0) || ((_x)>=(_width)) || ((_y)<0) || ((_y)>=(_height)) ) return;    \
    *( (_lcd_ptr) + (_y)*(_buf_width) + (_x) ) ^= (_color);     \
}

/*
*This macro write color value to lcd buffer, in one copy mode
*and if the pixel is not in lcd size, do nothing 
*/
#define WR_COLOR_IN_LCD_BY_COPY_MODE(_lcd_ptr,_buf_width, _x, _y,_width, _height, _color, _copy_mode)   \
{                                                       \
    switch(_copy_mode)                          \
{                                               \
        case OVER_MODE:                                 \
        WR_COLOR_IN_LCD((_lcd_ptr),(_buf_width),(_x),(_y),(_width),(_height),(_color));         \
        break;                                  \
        \
        case XOR_MODE:          \
        WR_COLOR_IN_LCD_XOR((_lcd_ptr),(_buf_width),(_x),(_y),(_width),(_height),(_color));     \
        break;                  \
        \
        default:                        \
        break;                                  \
}                                       \
}

#define ABS_A(x) (((x) >= 0)? (x) : (-(x)))
#define DRAW_LINE(_lcdbuf_ptr,_buf_width,_lcd_width,_lcd_height,_x1,_y1,_x2,_y2,_color,_copy_mode)\
{                                                                       \
    int16   dx, dy, dx2, dy2, dif, de;                                  \
    int16   dwidth = 1;                                                 \
    \
    int16  abs_dx, abs_dy;                                              \
    \
    abs_dx = ABS_A(_x1 - _x2);                                          \
    abs_dy = ABS_A(_y1 - _y2);                                          \
    if (abs_dx > abs_dy)                                                    \
{                                                                       \
    if((_x1) > (_x2))                                                   \
{                                                                   \
    SwapVal(&(_x1),&(_x2));                                         \
    SwapVal(&(_y1),&(_y2));                                         \
}                                                                   \
    \
    dx = (_x2) - (_x1);                                                 \
    dy = (_y2) - (_y1);                                                 \
    if ( dy < 0 )                                                       \
{                                                                   \
    dy = -dy;                                                       \
    dwidth = -dwidth;                                               \
}                                                                   \
    \
    dx2 = dx << 1;                                                      \
    dy2 = dy << 1;                                                      \
    dif = dy2 - dx2;                                                \
    de = dy2 - dx;                                                  \
    \
    for (; dx >= 0; dx--,(_x1)++)                                   \
{                                                               \
    WR_COLOR_IN_LCD_BY_COPY_MODE((_lcdbuf_ptr),(_buf_width),(_x1),(_y1),(_lcd_width),(_lcd_height),(_color),(_copy_mode));\
    if(de > 0)                                                  \
{                                                           \
                (_y1) += dwidth;                                        \
                de += dif;                                              \
}                                                           \
    else                                                        \
{                                                           \
                de += dy2;                                              \
}                                                           \
}                                                               \
}                                                                   \
    else                                                                    \
{                                                                       \
    if((_y1) > (_y2))                                                   \
                         {                                                                  \
                         SwapVal(&(_x1),&(_x2));                                            \
                         SwapVal(&(_y1),&(_y2));                                            \
}                                                                   \
    dx = (_x2) - (_x1);                                                 \
    dy = (_y2) - (_y1);                                                 \
    if ( dx < 0 )                                                       \
{                                                                   \
    dx = -dx;                                                       \
    dwidth = -dwidth;                                               \
        }                                                                   \
        \
        dx2 = dx << 1;                                                      \
        dy2 = dy << 1;                                                      \
        dif = dx2 - dy2;                                                \
        de = dx2 - dy;                                                  \
        \
        for (; dy >= 0; dy--,(_y1)++)                                   \
        {                                                               \
        WR_COLOR_IN_LCD_BY_COPY_MODE((_lcdbuf_ptr),(_buf_width),(_x1),(_y1),(_lcd_width),(_lcd_height),(_color),(_copy_mode));\
        \
        if (de > 0)                                                 \
            {                                                           \
            _x1 += dwidth;                                          \
            de += dif;                                              \
            }                                                           \
            else                                                        \
            {                                                           \
            de += dx2;                                              \
            }                                                           \
        }                                                               \
    }                                                                   \
}

/*****************************************************************************/
//  Description : This function is used exchange the value of two parameter.
//  Global resource dependence : none
//  Author: Jim.zhang
//  Note:exchange the two value.
/*****************************************************************************/
LOCAL void SwapVal(
                   uint16 *val1_ptr,    // first value pointer
                   uint16 *val2_ptr     // second value pointer
                   )
{
    uint16 tmp = *val1_ptr;
    
    *val1_ptr = *val2_ptr;
    *val2_ptr = tmp;
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/******************************************************************************/
/*! \brief Draw a line between 2 specific point to LCD buffer by copy mode
 *  \param[in]      lcd_id      ID of LCD
 *  \param[in]      x1          x coordinate of first point
 *  \param[in]      y1          y coordinate of first point
 *  \param[in]      x2          x coordinate of second point
 *  \param[in]      y2          y coordinate of second point
 *  \param[in]      color       color to draw.
 *  \param[in]      copy_mode   copy mode
 *  \author  Jim.zhang
 *  \note
 *          it will not validate to display
 */
/******************************************************************************/
PUBLIC void LCD_DrawLineExt( 
                            GUI_LCD_DEV_INFO const *dev_info_ptr,   // block information
                            uint16                  x1,             //x coordinate of first point
                            uint16                  y1,             //y coordinate of first point
                            uint16                  x2,             //x coordinate of second point
                            uint16                  y2,             //y coordinate of second point
                            GUI_COLOR_T             color,          //color to draw.
                            LCD_COPY_MODE_E         copy_mode       //copy mode
                            )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    
    rect.left   = (int16)(MIN(x1,x2));
    rect.top    = (int16)(MIN(y1,y2));
    rect.right  = (int16)(MAX(x1,x2));
    rect.bottom = (int16)(MAX(y1,y2));
    rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, rect);
    if (((x1>x2)&&(y1<y2))||((x1<x2)&&(y1>y2)))
    {
        //左下<->右上
        x1 = rect.left;
        x2 = rect.right;
        y1 = rect.bottom;
        y2 = rect.top;
    }
    else
    {
        //左上<->右下
        x1 = rect.left;
        x2 = rect.right;
        y1 = rect.top;
        y2 = rect.bottom;
    }
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }    
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32*)block_buffer_ptr;
        uint32 u32color = ConvertRgb565ToArgb888(color, 0xff);

        DRAW_LINE(u32_block_buffer_ptr, mem_width, lcd_width, lcd_height,x1,y1,x2,y2,u32color,copy_mode);   /*lint !e685 !e568*/
    }
    else
#endif
    {
        DRAW_LINE(block_buffer_ptr, mem_width, lcd_width, lcd_height,x1,y1,x2,y2,color,copy_mode);  /*lint !e685 !e568*/
    }
    
    MMITHEME_StoreUpdateRect(
        dev_info_ptr,
        rect);
}
#endif
//yangyu add begin
PUBLIC BOOLEAN GetLcdInfoByLcdDev_All(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              uint32 *mem_width_ptr,    //out
                              uint32 *width_ptr,    //out
                              uint32 *height_ptr,    //out
                              uint32 data_type,     ////0: RGB565, 1: ARGB888
                              GUI_COLOR_T **lcd_buffer_pptr   //out
                              )
{
    GUILCD_DRAW_DC_T    draw_dc = {0};

    if (PNULL == dev_info_ptr 
        || PNULL == width_ptr 
        || PNULL == height_ptr
        || PNULL == lcd_buffer_pptr)
    {
        return FALSE;
    }
    
    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return FALSE;
    }   
    *mem_width_ptr = draw_dc.mem_width;
    *width_ptr = draw_dc.width;
    *height_ptr = draw_dc.height;
    *lcd_buffer_pptr = draw_dc.lcd_buffer_ptr;
    
    return TRUE;
}

PUBLIC GUI_COLOR_T* GetLcdInfoByLcdDev_Ex(
                              GUI_LCD_DEV_INFO const *dev_info_ptr
                              )
{
    GUI_COLOR_T *pDstData = NULL;
    GUILCD_DRAW_DC_T    draw_dc = {0};

    if (PNULL == dev_info_ptr 

       )
    {
        return NULL;
    }
    
    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return NULL;
    }   

    pDstData = draw_dc.lcd_buffer_ptr;
    
    return pDstData;
}



//yangyu end


/*****************************************************************************/
//  Description : 根据lcd_dev_info获得宽，高，buf_ptr
//  Global resource dependence : none
//  Author: bin.ji
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetLcdInfoByLcdDev(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              uint32 *mem_width_ptr,    //out
                              uint32 *width_ptr,    //out
                              uint32 *height_ptr,    //out
                              GUI_COLOR_T **lcd_buffer_pptr   //out
                              )
{
    GUILCD_DRAW_DC_T    draw_dc = {0};

    if (PNULL == dev_info_ptr 
        || PNULL == width_ptr 
        || PNULL == height_ptr
        || PNULL == lcd_buffer_pptr)
    {
        return FALSE;
    }
    
    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return FALSE;
    }   
    *mem_width_ptr = draw_dc.mem_width;
    *width_ptr = draw_dc.width;
    *height_ptr = draw_dc.height;
    *lcd_buffer_pptr = draw_dc.lcd_buffer_ptr;
    
    return TRUE;
}

/*
*this maco write color value to lcd buffer.
* lcd_ptr: lcd buffer pointer
* x: x coordinate of one pixel  
*/
#define WR_COLOR_TO_LCD(_lcd_ptr, _x, _y, _width, _color)   \
{\
    *( (_lcd_ptr) + (_y)*(_width) + (_x) ) = (_color);\
}

/* Draw a horizontal line from(x1, y ) to (x2, y) , x1 > x2 */
#define DRAW_HLINE(_lcdbuf_ptr,_buf_width,_lcd_width,_lcd_height,_x1,_y,_x2,_color)         \
{                                                                               \
    uint16  i = 0;                                                              \
                                                                                \
    if ( ((_y) < 0) || ((_y) >= (int16)(_lcd_height)) )                         \
    {                                                                           \
        return;                                                                 \
    }                                                                           \
                                                                                \
    if ( (_x1) > (_x2) )                                                        \
    {                                                                           \
        SwapVal((uint16*)(&(_x1)), (uint16*)(&(_x2)));                          \
    }                                                                           \
                                                                                \
    if ( ((_x2) < 0) || ((_x1) >= (int16)(_lcd_width)) )                        \
    {                                                                           \
        return;                                                                 \
    }                                                                           \
                                                                                \
    if ( (_x1) < 0 )                                                            \
    {                                                                           \
        (_x1) = 0;                                                              \
    }                                                                           \
                                                                                \
    if ( (_x2) >= (int16)(_lcd_width) )                                         \
    {                                                                           \
        (_x2) = (int16)(_lcd_width) - 1;                                        \
    }                                                                           \
                                                                                \
    /*Draw line from (x1, y)  to (x2, y)*/                                      \
    for ( i=(_x1); i<=(_x2); i++ )                                              \
    {                                                                           \
        WR_COLOR_TO_LCD( (_lcdbuf_ptr), i, (_y), (_buf_width), (_color));       \
    }                                                                           \
}

/******************************************************************************/
/*! \brief Draw a horizontal line between 2 specific point to LCD buffer.
 *  \param[in]      lcdID       ID of LCD
 *  \param[in]      x1          x coordinate of first point
 *  \param[in]      y           y coordinate of first point
 *  \param[in]      x2          x coordinate of second point
 *  \param[in]      color       color to draw.
 *  \author  Jim.zhang
 *  \note
 *          it will not validate to display
 */
/******************************************************************************/
PUBLIC void LCD_DrawHLine( 
                          GUI_LCD_DEV_INFO const *dev_info_ptr, // block information
                          uint16                    x1,             //x coordinate of first point
                          uint16                    y,              //y coordinate of first point
                          uint16                    x2,             //x coordinate of second point
                          GUI_COLOR_T               color           //color to draw.    
                          )
{
    int16       tmp_x1 = 0;
    int16       tmp_x2 = 0;
    int16       tmp_y = 0;
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;      
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }

    // 接口参数是uint16，这里把它转成int16
    tmp_x1 = (int16)x1;
    tmp_x2 = (int16)x2;
    tmp_y = (int16)y;

    rect.left   = (int16)(MIN(tmp_x1,tmp_x2));
    rect.top    = tmp_y;
    rect.right  = (int16)(MAX(tmp_x1,tmp_x2));
    rect.bottom = tmp_y;
    rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, rect);
    tmp_x1 = rect.left;
    tmp_x2 = rect.right;
    tmp_y = rect.top;

#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType( (GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

        DRAW_HLINE(u32_block_buffer_ptr, mem_width, lcd_width, lcd_height,tmp_x1,tmp_y,tmp_x2,ConvertRgb565ToArgb888(color, 0xff));   /*lint !e685 !e568*/
    }
    else
#endif
    {
        DRAW_HLINE(block_buffer_ptr, mem_width, lcd_width, lcd_height,tmp_x1,tmp_y,tmp_x2,color);   /*lint !e685 !e568*/
    }
    
    MMITHEME_StoreUpdateRect(
        dev_info_ptr,
        rect);
}

/* Draw a vertical line from (x0, y01) to (x0, y02)   y2 > y1 */
#define DRAW_VLINE(_lcdbuf_ptr, _buf_width, _lcd_width,_lcd_height,_x,_y1,_y2,_color)       \
{                                                                               \
    uint16  i  = 0;                                                             \
                                                                                \
    if ( ((_x) < 0) || ((_x) >= (int16)(_lcd_width)) )                          \
    {                                                                           \
        return;                                                                 \
    }                                                                           \
                                                                                \
    if ( (_y1) > (_y2) )                                                        \
    {                                                                           \
        SwapVal((uint16*)(&(_y1)), (uint16*)(&(_y2)));                          \
    }                                                                           \
                                                                                \
    if ( ((_y2) < 0) || ((_y1) >= (int16)(_lcd_height)) )                       \
    {                                                                           \
        return;                                                                 \
    }                                                                           \
                                                                                \
    if ( (_y1) < 0 )                                                            \
    {                                                                           \
        (_y1) = 0;                                                              \
    }                                                                           \
                                                                                \
    if ( (_y2) >= (int16)(_lcd_height) )                                        \
    {                                                                           \
        (_y2) = (int16)(_lcd_height) - 1;                                       \
    }                                                                           \
                                                                                \
    for ( i=(_y1); i<=(_y2); i++ )                                              \
    {                                                                           \
        WR_COLOR_TO_LCD( (_lcdbuf_ptr), (_x), i, (_buf_width), (_color) );      \
    }                                                                           \
}

/******************************************************************************/
/*! \brief Draw a vertical line between 2 specific point to LCD buffer.
 *  \param[in]      lcdID       ID of LCD to display 
 *  \param[in]      x           x coordinate of first point
 *  \param[in]      y1          y coordinate of first point
 *  \param[in]      y2          y coordinate of second point
 *  \param[in]      color       color to draw.
 *  \author  Jim.zhang
 *  \note
 *          it will not validate to display
 */
/******************************************************************************/
PUBLIC void LCD_DrawVLine(
                          GUI_LCD_DEV_INFO const *dev_info_ptr, // block information
                          uint16                    x,              //x coordinate of first point
                          uint16                    y1,             //y coordinate of first point
                          uint16                    y2,             //y coordinate of second point
                          GUI_COLOR_T               color           //color to draw the line.   
                          )                          
{
    int16       tmp_x = 0;
    int16       tmp_y1 = 0;
    int16       tmp_y2 = 0;
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;    
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }

    // 接口参数是uint16，这里把它转成int16
    tmp_x = (int16)x;
    tmp_y1 = (int16)y1;
    tmp_y2 = (int16)y2;

    rect.left   = tmp_x;
    rect.top    = (int16)(MIN(tmp_y1,tmp_y2));
    rect.right  = tmp_x;
    rect.bottom = (int16)(MAX(tmp_y1,tmp_y2));
    rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP,  (GUI_LCD_DEV_INFO *)dev_info_ptr, rect);
    tmp_x = rect.left;
    tmp_y1 = rect.top;
    tmp_y2 = rect.bottom;    
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType( (GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32*)block_buffer_ptr;

        DRAW_VLINE(u32_block_buffer_ptr,mem_width,lcd_width, lcd_height, tmp_x,tmp_y1,tmp_y2,ConvertRgb565ToArgb888(color, 0xff));    /*lint !e685 !e568*/
    }
    else
#endif
    {
        DRAW_VLINE(block_buffer_ptr,mem_width,lcd_width, lcd_height, tmp_x,tmp_y1,tmp_y2,color);    /*lint !e685 !e568*/
    }
    
    MMITHEME_StoreUpdateRect(
        dev_info_ptr,
        rect);
}

#define FIXED_PRECISION 16
#define FIXED_MASS      0xffff0000
#define FIXED_FRAC      0x0000ffff
#define FIXED_ONE       0x00010000
#define FIXED_HALF      0x00008000

typedef int fixed;

/*
*This macro read color value from lcd buffer,
*and if the pixel is not in lcd size, do nothing 
*/
#define RD_COLOR_IN_LCD(_lcd_ptr, _buf_width, _x, _y, _width, _height, _color)  \
{                                                       \
    if ( ((_x)>=(_width)) || ((_y)>=(_height)) ) return;    \
    (_color) = *( (_lcd_ptr) + (_y)*(_buf_width) + (_x) );  \
}

__inline fixed fixed_cvt_i2x(int x)         {return x<<FIXED_PRECISION;}
__inline int   fixed_cvt_x2i(fixed x)       {return x>>FIXED_PRECISION;}
__inline fixed fixed_cvt_f2x(float x)       {return (int)(x*(1<<FIXED_PRECISION));}
__inline float fixed_cvt_x2f(fixed x)       {return (float)x/(1<<FIXED_PRECISION);}

__inline fixed fixed_frac(fixed x)          {return x&FIXED_FRAC;}
__inline fixed fixed_mass(fixed x)          {return x&FIXED_MASS;}

__inline fixed fixed_ceil(fixed x)          {return (x+FIXED_FRAC)&FIXED_MASS;}
__inline fixed fixed_floor(fixed x)         {return  x&FIXED_MASS;}

__inline fixed fixed_idiv(int a, int b)     {return (a<<FIXED_PRECISION)/b;}
__inline fixed fixed_mul(fixed a, fixed b)  {return (a*b)>>FIXED_PRECISION;}
__inline fixed fixed_scale(fixed a, int b)  {return a*b;}

__inline int isqrt(int x)
{
	int root = 0;
	int seed = 1<<30;
	while(seed > x)
	{
		seed>>=2;
	}
	while(seed != 0)
	{
		if(x >= seed+root)
		{
			x    -= seed+root;
			root += seed*2;
		}
		root >>= 1;
		seed >>= 2;
	}
	return root;
}

LOCAL fixed coverage(fixed frac, fixed slope)
{
	if(slope>=0)
	{
		if(frac<slope)
		{
			return frac/2*frac/slope;
		}
		if(frac<=FIXED_ONE)
		{
			return frac-slope/2;
		}
	}
	else
	{
		frac = FIXED_ONE - frac;
		if(frac<-slope)
		{
			return -frac/2*frac/slope;
		}
		if(frac<=FIXED_ONE)
		{
			return frac+slope/2;
		}
	}
    return 0;
}


#define R_COLOR(color)          ((color) & 0xf800)
#define G_COLOR(color)          ((color) & 0x07e0)
#define B_COLOR(color)          ((color) & 0x001f)

__inline void blend_color(
                        GUI_COLOR_T *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        int16 x,
                        int16 y,
                        GUI_COLOR_T color,
                        fixed alpha
                        )
{
    GUI_COLOR_T lcd_color;
    uint16 alpha_value = alpha >> 8;
    if(0 == alpha_value) return;
    RD_COLOR_IN_LCD(buffer_ptr, (int32)buf_width, x, y, (int32)lcd_width, (int32)lcd_height, lcd_color);
    color = 
        R_COLOR((R_COLOR(color) * alpha_value + R_COLOR(lcd_color) * (256 - alpha_value)) >> 8) |
        G_COLOR((G_COLOR(color) * alpha_value + G_COLOR(lcd_color) * (256 - alpha_value)) >> 8) |
        B_COLOR((B_COLOR(color) * alpha_value + B_COLOR(lcd_color) * (256 - alpha_value)) >> 8);
    WR_COLOR_IN_LCD(buffer_ptr, (int32)buf_width, x, y, (int32)lcd_width, (int32)lcd_height, color);
}

__inline void fill_color(
                        GUI_COLOR_T *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        int16 x,
                        int16 y,
                        GUI_COLOR_T color
                        )
{
    WR_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, (int32)lcd_width, (int32)lcd_height, color);
}

#define CAP_PATTERN_MAX 5

LOCAL const fixed pattern_1[] ={
        0x0000142B,  0x000074E4,  0x0000142B,
        0x000074E4,  0x0000ffff,  0x000074E4,
        0x0000142B,  0x000074E4,  0x0000142B
    };

LOCAL const fixed pattern_2[] ={
        0x0000142B,  0x000074E4,  0x0000142B,
        0x000074E4,  0x0000ffff,  0x000074E4,
        0x0000142B,  0x000074E4,  0x0000142B
    };
LOCAL const fixed pattern_3[] ={
        0x00008B9F,  0x0000F8C3,  0x00008B9F,
        0x0000F8C3,  0x0000ffff,  0x0000F8C3,
        0x00008B9F,  0x0000F8C3,  0x00008B9F
    };
LOCAL const fixed pattern_4[] ={
        0x00000000,  0x000036BD,  0x00007A9D,  0x000036BD,  0x00000000,
        0x000036BD,  0x0000FC26,  0x0000ffff,  0x0000FC26,  0x000036BD,
        0x00007A9D,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x00007A9D,
        0x000036BD,  0x0000FC26,  0x0000ffff,  0x0000FC26,  0x000036BD,
        0x00000000,  0x000036BD,  0x00007A9D,  0x000036BD,  0x00000000
    };
LOCAL const fixed pattern_5[] ={
        0x00002309,  0x0000C4F2,  0x0000FBB5,  0x0000C4F2,  0x00002309,
        0x0000C4F2,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x0000C4F2,
        0x0000FBB5,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x0000FBB5,
        0x0000C4F2,  0x0000ffff,  0x0000ffff,  0x0000ffff,  0x0000C4F2,
        0x00002309,  0x0000C4F2,  0x0000FBB5,  0x0000C4F2,  0x00002309
    };
    
LOCAL const fixed* patterns[] ={
		pattern_1,
        pattern_2,
        pattern_3,
        pattern_4,
        pattern_5
    };

/*****************************************************************************/
//   Description : Draw circle end cap for aa thick lines
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note:   
/*****************************************************************************/
LOCAL void DrawEndCapAA( 
                       int16 x,             //x coordinate of one end point
                       int16 y,             //y coordinage of one end point
                       uint32            color,     //color to draw.
                       uint16            width,
                       uint16           *mem_ptr,
                       uint16           mem_width,
                       uint16           mem_height,                
                       GUI_RECT_T       *update_rect_ptr
                       )

{
    fixed*      pattern_ptr = PNULL;//patterns[width-1];
    int16 xx, yy, x1, y1, x2, y2, stride;

	if(width > CAP_PATTERN_MAX )
	{
		return;
	}

	pattern_ptr = (fixed*)patterns[width-1]; 

    if ( PNULL == mem_ptr) return;

    width >>= 1;
    stride = (width<<1) + 1;

    x1 = x - width;
    y1 = y - width;
    x2 = x + width;
    y2 = y + width;
    
    for(xx=x1; xx<=x2; xx++)
    {
        for(yy=y1; yy<=y2; yy++)
        {
			blend_color(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color, pattern_ptr[(xx-x1)+(yy-y1)*stride]);
        }
    } 
       
    if (NULL != update_rect_ptr)
    {
        update_rect_ptr->left   = x1;
        update_rect_ptr->top    = y1;
        update_rect_ptr->right  = x2;
        update_rect_ptr->bottom = y2;
    }
}

#define SWAP(a, b, t) {(t)=(a);(a)=(b);(b)=(t);}
//#define ABS(x)        ((x)>0?(x):-(x))

LOCAL void DrawThickLineAA(int16           x1,             //x coordinate of first point
                     int16           y1,             //y coordinage of first point
                     int16           x2,             //x coordinate of second point
                     int16           y2,             //y coordinate of sconcd point
                     uint32           color,           //color to draw.
                     uint16           width,           //unit: pixel
                     uint16           *mem_ptr,
                     uint16           mem_width,
                     uint16           mem_height,
                     GUI_RECT_T       *update_rect_ptr)
{
    GUI_RECT_T  rect = {0};

    if((x1==x2)&&(y1==y2))
        return;

    if ( PNULL == mem_ptr) return;
           
    DrawEndCapAA(x1, y1, color, width, mem_ptr, mem_width, mem_height, &rect);
    DrawEndCapAA(x2, y2, color, width, mem_ptr, mem_width, mem_height, &rect);

    {
        int dx = x2 - x1;
        int dy = y2 - y1;

        int len = isqrt(dx*dx+dy*dy);

		fixed slope, span, unit, step;
        fixed start_x, start_y, end_y, end_x, cur_x, cur_y;
        fixed start_x_align, start_y_align, end_x_align, end_y_align;

    	if(ABS(dx)>ABS(dy))
    	{
            if(x1>x2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dy,dx);
            span  = fixed_idiv(width*len,dx)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_x   = fixed_cvt_i2x(x2);
        	start_x = fixed_cvt_i2x(x1);
       		start_y = fixed_cvt_i2x(y1) - span/2 + FIXED_HALF;
            end_y   = fixed_cvt_i2x(y2) + span/2 + FIXED_HALF;

            start_x_align = fixed_floor(start_x);
            end_x_align   = fixed_floor(end_x);
            cur_y         = start_y + fixed_mul(start_x_align-start_x,slope);

        	for(cur_x=start_x_align; cur_x<end_x_align; cur_x+=FIXED_ONE, cur_y+=slope)
        	{
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_y),slope));
        		start_y_align = fixed_floor(cur_y) + unit;
        		if(fixed_mass(cur_y) != fixed_mass(cur_y+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+slope), color, FIXED_ONE - coverage(fixed_frac(cur_y+slope),slope));
        			start_y_align += unit;
        		}
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span+slope), color, coverage(fixed_frac(cur_y+span+slope),slope));
        		end_y_align = fixed_floor(cur_y+span+slope);
        		if(fixed_mass(cur_y+span) != fixed_mass(cur_y+span+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_y+span),slope));
        			end_y_align -= unit;
        		}
				
				if(slope<0)
				{
				   	for(step=start_y_align; step>end_y_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}
				else
				{
				   	for(step=start_y_align; step<end_y_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}

        	}
    	}
    	else
    	{
            if(y1>y2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dx,dy);
            span  = fixed_idiv(width*len,dy)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_y   = fixed_cvt_i2x(y2);
        	start_y = fixed_cvt_i2x(y1);
       		start_x = fixed_cvt_i2x(x1) - span/2 + FIXED_HALF;
            end_x   = fixed_cvt_i2x(x2) + span/2 + FIXED_HALF;

            start_y_align = fixed_floor(start_y);
            end_y_align   = fixed_floor(end_y);
            cur_x         = start_x + fixed_mul(start_y_align-start_y,slope);

        	for(cur_y=start_y_align; cur_y<end_y_align; cur_y+=FIXED_ONE, cur_x+=slope)
        	{
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_x),slope));
        		start_x_align = fixed_floor(cur_x) + unit;
        		if(fixed_mass(cur_x) != fixed_mass(cur_x+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+slope), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(fixed_frac(cur_x+slope),slope));
        			start_x_align += unit;
        		}
        		blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span+slope), fixed_cvt_x2i(cur_y), color, coverage(fixed_frac(cur_x+span+slope),slope));
        		end_x_align = fixed_floor(cur_x+span+slope);
        		if(fixed_mass(cur_x+span) != fixed_mass(cur_x+span+slope))
        		{
        			blend_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_x+span),slope));
        			end_x_align -= unit;
        		}
				if(slope < 0)
				{
        			for(step=start_x_align; step>end_x_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
				else
				{
        			for(step=start_x_align; step<end_x_align; step+=unit)
        			{
        				fill_color(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
        	}
    	}

        if (NULL != update_rect_ptr)
        {
            update_rect_ptr->left   = (int16)fixed_cvt_x2i(MIN(start_x,end_x))-width;
            update_rect_ptr->top    = (int16)fixed_cvt_x2i(MIN(start_y,end_y))-width;
            update_rect_ptr->right  = (int16)fixed_cvt_x2i(MAX(start_x,end_x))+width;
            update_rect_ptr->bottom = (int16)fixed_cvt_x2i(MAX(start_y,end_y))+width;
        }
    }    
}

LOCAL int enable_aa = 1;

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//   Description : Draw a line between 2 specific point to LCD buffer 
//                      whose width is WIDTH. 
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note: 1. the method for drawing thick lines here is "replicating pixels"
//            2. the end cap style of the line is square
//            3. for even-numbered width, the up-left side of the line is chosen to be thicker
/*****************************************************************************/
PUBLIC void LCD_DrawThickLine( 
                              GUI_LCD_DEV_INFO  *dev_info_ptr,         //ID of LCD.
                              uint16 x1,             //x coordinate of first point
                              uint16 y1,             //y coordinage of first point
                              uint16 x2,             //x coordinate of second point
                              uint16 y2,             //y coordinate of sconcd point
                              GUI_COLOR_T    color,     //color to draw.
                              uint16            width //unit: pixel
                              )
{
	LCD_DrawThickLineEx((GUI_LCD_DEV_INFO *)dev_info_ptr, x1, y1, x2, y2, color, width, GUILCD_PEN_STYLE_DEFAULT);
}
#endif
/*****************************************************************************/
//   Description : Draw square end cap for thick lines
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note:   
/*****************************************************************************/
LOCAL void DrawEndCap( 
                      GUI_LCD_DEV_INFO  *dev_info_ptr,         //ID of LCD.
                      uint16 x,             //x coordinate of one end point
                      uint16 y,             //y coordinage of one end point
                      GUI_COLOR_T    color,     //color to draw.
                      uint16            width
                      )
{
    GUI_RECT_T    box = {0};
    
    box.left = (int16)(x - (width>>1));
    box.right = (int16)(box.left + width - 1);
    box.top = (int16)(y - (width>>1));
    box.bottom = (int16)(box.top + width - 1);
    
    LCD_FillRect(
        dev_info_ptr,
        box,
        color);
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description:  draw thick line ex
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DrawThickLineEx( 
                                GUI_LCD_DEV_INFO  *dev_info_ptr,   //lcd info
                                int16              x1,             //x coordinate of first point
                                int16              y1,             //y coordinage of first point
                                int16              x2,             //x coordinate of second point
                                int16              y2,             //y coordinate of sconcd point
                                GUI_COLOR_T        color,          //color to draw.
                                uint16             width,          //unit: pixel
                                GUILCD_PEN_STYLE_E style
                                )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
	GUI_RECT_T  update_rect = {0};
    uint32      u32_color = 0;

    if (width <1 || width > MAXLINEWIDTH)
        return;	
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }

    //convert lcd to disp rect
    rect.left = x1;
    rect.top = y1;
    rect.right = x2;
    rect.bottom = y2;
    rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, rect);	

#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;
		
        u32_color = ConvertRgb565ToArgb888(color, 0xff);
        switch (style)
        {
        case GUILCD_PEN_STYLE_DOT:
            DrawDotLine(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    (void *)u32_block_buffer_ptr, mem_width, lcd_height, 1, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH:
            DrawDashLine(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    (void *)u32_block_buffer_ptr, mem_width, lcd_height, 1, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH_DOT:
            DrawDashDotLine(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    (void *)u32_block_buffer_ptr, mem_width, lcd_height, 1, &update_rect);			
            break;

        default:
            DrawThickLineAAU32(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    (uint32 *)u32_block_buffer_ptr, mem_width, lcd_height, &update_rect);
            break;
        }

    }
    else
#endif
    {
        switch (style)
        {
        case GUILCD_PEN_STYLE_DOT:
            DrawDotLine(rect.left, rect.top, rect.right, rect.bottom, color, width,
                                    (void *)block_buffer_ptr, mem_width, lcd_height, 0, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH:
            DrawDashLine(rect.left, rect.top, rect.right, rect.bottom, color, width,
                    (void *)block_buffer_ptr, mem_width, lcd_height, 0, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH_DOT:
            DrawDashDotLine(rect.left, rect.top, rect.right, rect.bottom, color, width,
                                    (void *)block_buffer_ptr, mem_width, lcd_height, 0, &update_rect);			
            break;

        default:
            DrawThickLineAA(rect.left, rect.top, rect.right, rect.bottom, color, width,
                                    (uint16 *)block_buffer_ptr, mem_width, lcd_height, &update_rect);
            break;
        }
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, update_rect);
}
#endif

/*****************************************************************************/
//  Description:  draw thick line ex
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void LCD_DrawThickLineAA_NoCap( 
                                GUI_LCD_DEV_INFO  *dev_info_ptr,   //lcd info
                                int16              x1,             //x coordinate of first point
                                int16              y1,             //y coordinage of first point
                                int16              x2,             //x coordinate of second point
                                int16              y2,             //y coordinate of sconcd point
                                GUI_COLOR_T        color,          //color to draw.
                                uint16             width,          //unit: pixel
                                GUILCD_PEN_STYLE_E style
                                )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
	GUI_RECT_T  update_rect = {0};
    uint32      u32_color = 0;

    if (width <1 || width > MAXLINEWIDTH)
        return;	
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }

    //convert lcd to disp rect
    rect.left = x1;
    rect.top = y1;
    rect.right = x2;
    rect.bottom = y2;
    rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, rect);	

#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;
		
        u32_color = ConvertRgb565ToArgb888(color, 0xff);
        switch (style)
        {
        case GUILCD_PEN_STYLE_DOT:
            DrawDotLine_NoCap(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    (void *)u32_block_buffer_ptr, mem_width, lcd_height, 1, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH:
            DrawDashLine_NoCap(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    (void *)u32_block_buffer_ptr, mem_width, lcd_height, 1, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH_DOT:
            DrawDashDotLine_NoCap(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    (void *)u32_block_buffer_ptr, mem_width, lcd_height, 1, &update_rect);			
            break;

        default:
            DrawThickLineAAU32_NoCap(rect.left, rect.top, rect.right, rect.bottom, u32_color, width,
                                    u32_block_buffer_ptr, mem_width, lcd_height, &update_rect);
            break;
        }
    }
    else
#endif
    {
        switch (style)
        {
        case GUILCD_PEN_STYLE_DOT:
            DrawDotLine_NoCap(rect.left, rect.top, rect.right, rect.bottom, color, width,
                                    (void *)block_buffer_ptr, mem_width, lcd_height, 0, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH:
            DrawDashLine_NoCap(rect.left, rect.top, rect.right, rect.bottom, color, width,
                    (void *)block_buffer_ptr, mem_width, lcd_height, 0, &update_rect);			
            break;

        case GUILCD_PEN_STYLE_DASH_DOT:
            DrawDashDotLine_NoCap(rect.left, rect.top, rect.right, rect.bottom, color, width,
                                    (void *)block_buffer_ptr, mem_width, lcd_height, 0, &update_rect);			
            break;

        default:
            DrawThickLineAA_NoCap(rect.left, rect.top, rect.right, rect.bottom, color, width,
                                    (uint16 *)block_buffer_ptr, mem_width, lcd_height, &update_rect);
            break;
        }
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, update_rect);
}
#ifndef GUI_GRAPH_MINI_SUPPORT
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
                                )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }

    if (width < 1)
    {
        return;
    }
    if(width > MAXLINEWIDTH)
    {
        //convert lcd to disp rect
        rect.left = x1;
        rect.top = y1;
        rect.right = x2;
        rect.bottom = y2;
        rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, rect);

#ifdef UI_MULTILAYER_SUPPORT
        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(dev_info_ptr))
        {
            uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

            rect = DrawThickLineU32( u32_block_buffer_ptr, mem_width, lcd_height, rect.left, rect.top, rect.right, rect.bottom, ConvertRgb565ToArgb888(color, 0xff), width, style );
        }
        else
#endif
        {
            rect = DrawThickLine( block_buffer_ptr, mem_width, lcd_height, rect.left, rect.top, rect.right, rect.bottom, color, width, style );
        }
        
        MMITHEME_StoreUpdateRect( dev_info_ptr, rect );
    }
    else
    {
        LCD_DrawThickLineAA_NoCap(dev_info_ptr, x1, y1, x2, y2, color, width, style);
    }
}
#endif

/*****************************************************************************/
//  Description:  draw line to buffer
//  Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawThickLineU32( 
                               uint32*           buffer_ptr,
                               uint16            buf_width,
                               uint16            buf_height,
                               int16             x1,
                               int16             y1,
                               int16             x2,
                               int16             y2,
                               uint32            color,
                               uint16            width, //unit: pixel
                               GUILCD_PEN_STYLE_E style
                               )
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height 
        && width > 0 )
    {
        GUI_RECT_T buf_rect  = {0};
        GUI_RECT_T line_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        line_rect.left   = (int16)(MIN(x1,x2));
        line_rect.top    = (int16)(MIN(y1,y2));
        line_rect.right  = (int16)(MAX(x1,x2));
        line_rect.bottom = (int16)(MAX(y1,y2));
        
        if ( width > 1 )
        {
            line_rect.left   -= width/2;
            line_rect.top    -= width/2;
            line_rect.right  += width/2;
            line_rect.bottom += width/2;
        }
        
        if ( GUI_IntersectRect( &clip_rect, line_rect, buf_rect ) )
        {
            int32 i = 0;
            
            if ( y1== y2 ) //Horizental line
            {
                for ( i = 0; i < width; i++ )
                {
                    DrawHLineU32( buffer_ptr, buf_width, clip_rect, x1, y1-width/2+i, x2, color, style );
                }
            }
            else if(x1 == x2) //Vertical line
            {
                for (i=0;i<width;i++)
                {
                    DrawVLineU32( buffer_ptr, buf_width, clip_rect, x1-width/2+i, y1, y2, color, style );
                }
            }
            else
            {
                if( abs( x1 - x2 ) > abs( y1 - y2) )
                {
                    for ( i = 0; i < width; i++ )
                    {
                        DrawLineU32( buffer_ptr, buf_width, clip_rect, x1, y1-width/2+i, x2, y2-width/2+i, color, style );
                    }
                }
                else
                {
                    for ( i = 0; i< width; i++ )
                    {
                        DrawLineU32( buffer_ptr, buf_width, clip_rect, x1-width/2+i, y1, x2-width/2+i, y2, color, style );
                    }
                }
            }
        }
    }
    
    return clip_rect;
}

/*****************************************************************************/
//  Description:  draw line to buffer
//  Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawThickLine( 
                               GUI_COLOR_T*      buffer_ptr,
                               uint16            buf_width,
                               uint16            buf_height,
                               int16             x1,
                               int16             y1,
                               int16             x2,
                               int16             y2,
                               GUI_COLOR_T       color,
                               uint16            width, //unit: pixel
                               GUILCD_PEN_STYLE_E style
                               )
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height 
        && width > 0 )
    {
        GUI_RECT_T buf_rect  = {0};
        GUI_RECT_T line_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        line_rect.left   = (int16)(MIN(x1,x2));
        line_rect.top    = (int16)(MIN(y1,y2));
        line_rect.right  = (int16)(MAX(x1,x2));
        line_rect.bottom = (int16)(MAX(y1,y2));
        
        if ( width > 1 )
        {
            line_rect.left   -= width/2;
            line_rect.top    -= width/2;
            line_rect.right  += width/2;
            line_rect.bottom += width/2;
        }
        
        if ( GUI_IntersectRect( &clip_rect, line_rect, buf_rect ) )
        {
            int32 i = 0;
            
            if ( y1== y2 ) //Horizental line
            {
                for ( i = 0; i < width; i++ )
                {
                    DrawHLine( buffer_ptr, buf_width, clip_rect, x1, y1-width/2+i, x2, color, style );
                }
            }
            else if(x1 == x2) //Vertical line
            {
                for (i=0;i<width;i++)
                {
                    DrawVLine( buffer_ptr, buf_width, clip_rect, x1-width/2+i, y1, y2, color, style );
                }
            }
            else
            {
                if( abs( x1 - x2 ) > abs( y1 - y2) )
                {
                    for ( i = 0; i < width; i++ )
                    {
                        DrawLine( buffer_ptr, buf_width, clip_rect, x1, y1-width/2+i, x2, y2-width/2+i, color, style );
                    }
                }
                else
                {
                    for ( i = 0; i< width; i++ )
                    {
                        DrawLine( buffer_ptr, buf_width, clip_rect, x1-width/2+i, y1, x2-width/2+i, y2, color, style );
                    }
                }
            }
        }
    }
    
    return clip_rect;
}

/*****************************************************************************/
//  Description:  draw point
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
#define DRAW_POINT( buffer_ptr, buf_width, clip_rect, x, y, color )\
{\
    if ( !( (x) < clip_rect.left || (x) > clip_rect.right || (y) < clip_rect.top || (y) > clip_rect.bottom ) )\
{\
    *((buffer_ptr) + (y) * (buf_width) + (x)) = (color);\
}\
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawHLine(
                     GUI_COLOR_T*      buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x1,
                     int16             y,
                     int16             x2,
                     GUI_COLOR_T       color,
                     GUILCD_PEN_STYLE_E style
                     )
{       
    int32 i   = 0;
    int32 min = MIN( x1, x2 );
    int32 max = MAX( x1, x2 );
    
    if ( GUILCD_PEN_STYLE_DEFAULT == style )
    {
        for ( i = min; i < max; i++ )
        {
            DRAW_POINT( buffer_ptr, buf_width, clip_rect, i, y, color );
        }
    }
    else
    {
        uint16 cur_len  = 0;
        uint16 draw1_min = 0;
        uint16 draw1_max = 0;
        uint16 draw2_min = 0;
        uint16 draw2_max = 0;
        uint16 index = 0;
        
        if ( GUILCD_PEN_STYLE_DOT == style )
        {
            cur_len = 2 + SPACE_LEN * 2;
            draw1_max = 1;
            draw2_min = 1 + SPACE_LEN;
            draw2_max = 2 + SPACE_LEN;
        }
        else if ( GUILCD_PEN_STYLE_DASH == style )
        {
            cur_len   = DASH_LEN*2 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN*2 + SPACE_LEN;
        }
        else
        {
            cur_len = DASH_LEN + 1 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN + 1 + SPACE_LEN;
        }
        
        for ( i = min; i < max; i++ )
        {
            index = (i - min) % cur_len;
            
            if ( ( index >= draw1_min && index < draw1_max )
                || ( index >= draw2_min && index < draw2_max ) )
            {
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, i, y, color );
            }
        }
    }
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawHLineU32(
                     uint32*           buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x1,
                     int16             y,
                     int16             x2,
                     uint32             color,
                     GUILCD_PEN_STYLE_E style
                     )
{       
    int32 i   = 0;
    int32 min = MIN( x1, x2 );
    int32 max = MAX( x1, x2 );
    
    if ( GUILCD_PEN_STYLE_DEFAULT == style )
    {
        for ( i = min; i < max; i++ )
        {
            DRAW_POINT( buffer_ptr, buf_width, clip_rect, i, y, color );
        }
    }
    else
    {
        uint16 cur_len  = 0;
        uint16 draw1_min = 0;
        uint16 draw1_max = 0;
        uint16 draw2_min = 0;
        uint16 draw2_max = 0;
        uint16 index = 0;
        
        if ( GUILCD_PEN_STYLE_DOT == style )
        {
            cur_len = 2 + SPACE_LEN * 2;
            draw1_max = 1;
            draw2_min = 1 + SPACE_LEN;
            draw2_max = 2 + SPACE_LEN;
        }
        else if ( GUILCD_PEN_STYLE_DASH == style )
        {
            cur_len   = DASH_LEN*2 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN*2 + SPACE_LEN;
        }
        else
        {
            cur_len = DASH_LEN + 1 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN + 1 + SPACE_LEN;
        }
        
        for ( i = min; i < max; i++ )
        {
            index = (i - min) % cur_len;
            
            if ( ( index >= draw1_min && index < draw1_max )
                || ( index >= draw2_min && index < draw2_max ) )
            {
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, i, y, color );
            }
        }
    }
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawVLineU32(
                     uint32*      buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x,
                     int16             y1,
                     int16             y2,
                     uint32             color,
                     GUILCD_PEN_STYLE_E style
                     )
{       
    int32 i   = 0;
    int32 min = MIN( y1, y2 );
    int32 max = MAX( y1, y2 );
    
    if ( GUILCD_PEN_STYLE_DEFAULT == style )
    {
        for ( i = min; i < max; i++ )
        {
            DRAW_POINT( buffer_ptr, buf_width, clip_rect, x, i, color );
        }
    }
    else
    {
        uint16 cur_len  = 0;
        uint16 draw1_min = 0;
        uint16 draw1_max = 0;
        uint16 draw2_min = 0;
        uint16 draw2_max = 0;
        uint16 index = 0;
        
        if ( GUILCD_PEN_STYLE_DOT == style )
        {
            cur_len = 2 + SPACE_LEN * 2;
            draw1_max = 1;
            draw2_min = 1 + SPACE_LEN;
            draw2_max = 2 + SPACE_LEN;
        }
        else if ( GUILCD_PEN_STYLE_DASH == style )
        {
            cur_len   = DASH_LEN*2 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN*2 + SPACE_LEN;
        }
        else
        {
            cur_len = DASH_LEN + 1 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN + 1 + SPACE_LEN;
        }
        
        for ( i = min; i < max; i++ )
        {
            index = (i - min) % cur_len;
            
            if ( ( index >= draw1_min && index < draw1_max )
                || ( index >= draw2_min && index < draw2_max ) )
            {
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, x, i, color );
            }
        }
    }
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawVLine(
                     GUI_COLOR_T*      buffer_ptr,
                     uint16            buf_width,
                     GUI_RECT_T        clip_rect, //必须先和buffer区域进行剪切
                     int16             x,
                     int16             y1,
                     int16             y2,
                     GUI_COLOR_T       color,
                     GUILCD_PEN_STYLE_E style
                     )
{       
    int32 i   = 0;
    int32 min = MIN( y1, y2 );
    int32 max = MAX( y1, y2 );
    
    if ( GUILCD_PEN_STYLE_DEFAULT == style )
    {
        for ( i = min; i < max; i++ )
        {
            DRAW_POINT( buffer_ptr, buf_width, clip_rect, x, i, color );
        }
    }
    else
    {
        uint16 cur_len  = 0;
        uint16 draw1_min = 0;
        uint16 draw1_max = 0;
        uint16 draw2_min = 0;
        uint16 draw2_max = 0;
        uint16 index = 0;
        
        if ( GUILCD_PEN_STYLE_DOT == style )
        {
            cur_len = 2 + SPACE_LEN * 2;
            draw1_max = 1;
            draw2_min = 1 + SPACE_LEN;
            draw2_max = 2 + SPACE_LEN;
        }
        else if ( GUILCD_PEN_STYLE_DASH == style )
        {
            cur_len   = DASH_LEN*2 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN*2 + SPACE_LEN;
        }
        else
        {
            cur_len = DASH_LEN + 1 + SPACE_LEN*2;
            draw1_max = DASH_LEN;
            draw2_min = DASH_LEN + SPACE_LEN;
            draw2_max = DASH_LEN + 1 + SPACE_LEN;
        }
        
        for ( i = min; i < max; i++ )
        {
            index = (i - min) % cur_len;
            
            if ( ( index >= draw1_min && index < draw1_max )
                || ( index >= draw2_min && index < draw2_max ) )
            {
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, x, i, color );
            }
        }
    }
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawLineU32(
                    uint32             *buffer_ptr,
                    uint16             buf_width,
                    GUI_RECT_T         clip_rect, //必须先和buffer区域进行剪切
                    int16              x1,
                    int16              y1,
                    int16              x2,
                    int16              y2,
                    uint32             color,
                    GUILCD_PEN_STYLE_E style
                    )
{                                                                       
    int16  dx,  dy, dx2, dy2, dif, de;                                  
    int16  dwidth = 1;  
    uint16 cur_len  = 0;
    uint16 draw1_min = 0;
    uint16 draw1_max = 0;
    uint16 draw2_min = 0;
    uint16 draw2_max = 0;
    uint16 index = 0;
    int32  temp = 0;
    
    if ( GUILCD_PEN_STYLE_DOT == style )
    {
        cur_len = 2 + SPACE_LEN * 2;
        draw1_max = 1;
        draw2_min = 1 + SPACE_LEN;
        draw2_max = 2 + SPACE_LEN;
    }
    else if ( GUILCD_PEN_STYLE_DASH == style )
    {
        cur_len   = DASH_LEN*2 + SPACE_LEN*2;
        draw1_max = DASH_LEN;
        draw2_min = DASH_LEN + SPACE_LEN;
        draw2_max = DASH_LEN*2 + SPACE_LEN;
    }
    else
    {
        cur_len = DASH_LEN + 1 + SPACE_LEN*2;
        draw1_max = DASH_LEN;
        draw2_min = DASH_LEN + SPACE_LEN;
        draw2_max = DASH_LEN + 1 + SPACE_LEN;
    }
    
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    
    if ( dx > dy )                                                  
    {
        if( x1 > x2)                                                    
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
            
            temp = y1;
            y1 = y2;
            y2 = temp;
        }                                                                   
        
        dx = x2 - x1;                                                   
        dy = y2 - y1;   
        
        if ( dy < 0 )                                                       
        {                                                                   
            dy = -dy;                                                       
            dwidth = -dwidth;                                               
        }   
        
        dx2 = dx << 1;                                                      
        dy2 = dy << 1;                                                      
        dif = dy2 - dx2;                                                
        de = dy2 - dx;                                                  
        
        if ( GUILCD_PEN_STYLE_DEFAULT == style )
        {
            for (; dx >= 0; dx--, x1++)                                 
            {           
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                
                if(de > 0)                                                  
                {                                                           
                    y1 += dwidth;                                       
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dy2;                                              
                }                                                           
            }   
        }
        else
        {
            temp = dx;
            for (; dx >= 0; dx--, x1++)                                 
            {       
                index = (temp - dx) % cur_len;
                
                if ( ( index >= draw1_min && index < draw1_max )
                    || ( index >= draw2_min && index < draw2_max ) )
                {
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                }
                
                if(de > 0)                                                  
                {                                                           
                    y1 += dwidth;                                       
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dy2;                                              
                }                                                           
            }
        }
    }                                                                   
    else
    {   
        if(y1 > y2)                                                 
        {                                                                   
            temp = x1;
            x1 = x2;
            x2 = temp;
            
            temp = y1;
            y1 = y2;
            y2 = temp;                                  
        }   
        
        dx = x2 - x1;                                                   
        dy = y2 - y1;   
        
        if ( dx < 0 )                                                       
        {                                                                   
            dx = -dx;                                                       
            dwidth = -dwidth;                                               
        }
        
        dx2 = dx << 1;                                                      
        dy2 = dy << 1;                                                      
        dif = dx2 - dy2;                                                
        de = dx2 - dy;                                                  
        
        if ( GUILCD_PEN_STYLE_DEFAULT == style )
        {
            for (; dy >= 0; dy--,y1++)                                  
            {       
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                
                if (de > 0)                                                 
                {                                                           
                    x1 += dwidth;                                           
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dx2;                                              
                }                                                           
            }
        }
        else
        {
            temp = dy;
            for (; dy >= 0; dy--,y1++)                                  
            {       
                index = (temp - dy) % cur_len;
                
                if ( ( index >= draw1_min && index < draw1_max )
                    || ( index >= draw2_min && index < draw2_max ) )
                {
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                }
                
                if (de > 0)
                {                                                           
                    x1 += dwidth;                                           
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dx2;                                              
                }                                                           
            }
        }
    }                                                                   
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL void DrawLine(
                    GUI_COLOR_T*       buffer_ptr,
                    uint16             buf_width,
                    GUI_RECT_T         clip_rect, //必须先和buffer区域进行剪切
                    int16              x1,
                    int16              y1,
                    int16              x2,
                    int16              y2,
                    GUI_COLOR_T        color,
                    GUILCD_PEN_STYLE_E style
                    )
{                                                                       
    int16  dx,  dy, dx2, dy2, dif, de;                                  
    int16  dwidth = 1;  
    uint16 cur_len  = 0;
    uint16 draw1_min = 0;
    uint16 draw1_max = 0;
    uint16 draw2_min = 0;
    uint16 draw2_max = 0;
    uint16 index = 0;
    int32  temp = 0;
    
    if ( GUILCD_PEN_STYLE_DOT == style )
    {
        cur_len = 2 + SPACE_LEN * 2;
        draw1_max = 1;
        draw2_min = 1 + SPACE_LEN;
        draw2_max = 2 + SPACE_LEN;
    }
    else if ( GUILCD_PEN_STYLE_DASH == style )
    {
        cur_len   = DASH_LEN*2 + SPACE_LEN*2;
        draw1_max = DASH_LEN;
        draw2_min = DASH_LEN + SPACE_LEN;
        draw2_max = DASH_LEN*2 + SPACE_LEN;
    }
    else
    {
        cur_len = DASH_LEN + 1 + SPACE_LEN*2;
        draw1_max = DASH_LEN;
        draw2_min = DASH_LEN + SPACE_LEN;
        draw2_max = DASH_LEN + 1 + SPACE_LEN;
    }
    
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    
    if ( dx > dy )                                                  
    {
        if( x1 > x2)                                                    
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
            
            temp = y1;
            y1 = y2;
            y2 = temp;
        }                                                                   
        
        dx = x2 - x1;                                                   
        dy = y2 - y1;   
        
        if ( dy < 0 )                                                       
        {                                                                   
            dy = -dy;                                                       
            dwidth = -dwidth;                                               
        }   
        
        dx2 = dx << 1;                                                      
        dy2 = dy << 1;                                                      
        dif = dy2 - dx2;                                                
        de = dy2 - dx;                                                  
        
        if ( GUILCD_PEN_STYLE_DEFAULT == style )
        {
            for (; dx >= 0; dx--, x1++)                                 
            {           
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                
                if(de > 0)                                                  
                {                                                           
                    y1 += dwidth;                                       
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dy2;                                              
                }                                                           
            }   
        }
        else
        {
            temp = dx;
            for (; dx >= 0; dx--, x1++)                                 
            {       
                index = (temp - dx) % cur_len;
                
                if ( ( index >= draw1_min && index < draw1_max )
                    || ( index >= draw2_min && index < draw2_max ) )
                {
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                }
                
                if(de > 0)                                                  
                {                                                           
                    y1 += dwidth;                                       
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dy2;                                              
                }                                                           
            }
        }
    }                                                                   
    else
    {   
        if(y1 > y2)                                                 
        {                                                                   
            temp = x1;
            x1 = x2;
            x2 = temp;
            
            temp = y1;
            y1 = y2;
            y2 = temp;                                  
        }   
        
        dx = x2 - x1;                                                   
        dy = y2 - y1;   
        
        if ( dx < 0 )                                                       
        {                                                                   
            dx = -dx;                                                       
            dwidth = -dwidth;                                               
        }
        
        dx2 = dx << 1;                                                      
        dy2 = dy << 1;                                                      
        dif = dx2 - dy2;                                                
        de = dx2 - dy;                                                  
        
        if ( GUILCD_PEN_STYLE_DEFAULT == style )
        {
            for (; dy >= 0; dy--,y1++)                                  
            {       
                DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                
                if (de > 0)                                                 
                {                                                           
                    x1 += dwidth;                                           
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dx2;                                              
                }                                                           
            }
        }
        else
        {
            temp = dy;
            for (; dy >= 0; dy--,y1++)                                  
            {       
                index = (temp - dy) % cur_len;
                
                if ( ( index >= draw1_min && index < draw1_max )
                    || ( index >= draw2_min && index < draw2_max ) )
                {
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x1, y1, color );
                }
                
                if (de > 0)
                {                                                           
                    x1 += dwidth;                                           
                    de += dif;                                              
                }                                                           
                else                                                        
                {                                                           
                    de += dx2;                                              
                }                                                           
            }
        }
    }                                                                   
}

/******************************************************************************/
/*! \brief Fill a rectangle with one color to LCD buffer.
 *  \param[in]      lcdID       ID of LCD
 *  \param[in]      box         rectangle to fill color
 *  \param[in]      color       color to draw.
 *  \author  Jim.zhang
 *  \note
 *          it will not validate to display
 */
/******************************************************************************/
PUBLIC void LCD_FillRectU32(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         uint32                     u32_color       //color to fill
                         )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  main_rect = {0};
    GUI_RECT_T  dest_rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    
    box = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, box);
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }
    
    main_rect.right  = (int16)(lcd_width - 1);
    main_rect.bottom = (int16)(lcd_height - 1);
    
    if (GUI_IntersectRect(&dest_rect, box, main_rect))
    {
#ifdef UI_MULTILAYER_SUPPORT
        MMI_MEMMONITER_START(UILAYER_GetMemCheckAddr((GUI_LCD_DEV_INFO *)dev_info_ptr));
        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr)
            || DATA_TYPE_PMARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
        {
            uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

            LCD_FillRectToBufU32(u32_block_buffer_ptr,mem_width,lcd_width, lcd_height,dest_rect,u32_color);
        }
        else
        {
            return;
        }
        MMI_MEMMONITER_END();
#endif
        //store rect
        MMITHEME_StoreUpdateRect(dev_info_ptr,box);
    }
}

/******************************************************************************/
/*! \brief Fill a rectangle with one color to LCD buffer.
 *  \param[in]      lcdID       ID of LCD
 *  \param[in]      box         rectangle to fill color
 *  \param[in]      color       color to draw.
 *  \author  Jim.zhang
 *  \note
 *          it will not validate to display
 */
/******************************************************************************/
PUBLIC void LCD_FillRect(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         GUI_COLOR_T                color           //color to fill
                         )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  main_rect = {0};
    GUI_RECT_T  dest_rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    
    box = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, box);
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }
    
    main_rect.right  = (int16)(lcd_width - 1);
//    main_rect.right  = (int16)(mem_width - 1);
    main_rect.bottom = (int16)(lcd_height - 1);
    
    if (GUI_IntersectRect(&dest_rect, box, main_rect))
    {
#ifdef UI_MULTILAYER_SUPPORT
        MMI_MEMMONITER_START(UILAYER_GetMemCheckAddr((GUI_LCD_DEV_INFO *)dev_info_ptr));
        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr)
            || DATA_TYPE_PMARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
        {
            uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

            LCD_FillRectToBufU32(u32_block_buffer_ptr,mem_width,lcd_width, lcd_height,dest_rect,ConvertRgb565ToArgb888(color, 0xff));
        }
        else
        {
            LCD_FillRectToBuf(block_buffer_ptr,mem_width,lcd_width, lcd_height,dest_rect,color);
        }
//        LCD_FillRectToBuf(block_buffer_ptr,mem_width,mem_width, lcd_height,dest_rect,color);
        MMI_MEMMONITER_END();
#else
        LCD_FillRectToBuf(block_buffer_ptr,mem_width,lcd_width, lcd_height,dest_rect,color);
#endif
        //store rect
        MMITHEME_StoreUpdateRect(dev_info_ptr,box);
    }
}

//加入ARGB888清屏接口【必须】
PUBLIC void LCD_FillRectARGB888(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         uint32                color           //color to fill
                         )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  main_rect = {0};
    GUI_RECT_T  dest_rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;

    box = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, box);
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }

    main_rect.right  = (int16)(lcd_width - 1);
//    main_rect.right  = (int16)(mem_width - 1);
    main_rect.bottom = (int16)(lcd_height - 1);

    if (GUI_IntersectRect(&dest_rect, box, main_rect))
    {
#ifdef UI_MULTILAYER_SUPPORT
        MMI_MEMMONITER_START(UILAYER_GetMemCheckAddr((GUI_LCD_DEV_INFO *)dev_info_ptr));
        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr)
            || DATA_TYPE_PMARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
        {
            uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

            LCD_FillRectToBufU32(u32_block_buffer_ptr,mem_width,lcd_width, 
            lcd_height,dest_rect, color);
        }
//        LCD_FillRectToBuf(block_buffer_ptr,mem_width,mem_width, lcd_height,dest_rect,color);
        MMI_MEMMONITER_END();
#endif
        //store rect
        MMITHEME_StoreUpdateRect(dev_info_ptr,box);
    }
}

/*****************************************************************************/
// 	Description : convert rgb565 to argb888
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC uint32 ConvertRgb565ToArgb888(
                                     uint16     color,
                                     uint16     alpha
                                     )
{
    uint32      argb888 = 0;

    //alpha
    argb888 = (alpha & 0xFFFF) << 24;

    argb888 = (argb888| ((((color) >> 8) & 0xf8)<<16)|((((color) >> 3) & 0xfc)<<8)|(((color) << 3)  & 0xf8));

    return (argb888);
}

PUBLIC void LCD_FillRectToBufU32(
                uint32 *lcdbuf_ptr,
                uint32 buf_width,
                uint32 lcd_width,
                uint32 lcd_height,
                GUI_RECT_T box,
                uint32 color
                )
{                                                                               
    uint32 *temp_lcdbuf_ptr = PNULL;
    uint32 line_bytes = 0;
    uint32 height = 0;
    
    /*make the box size fit the lcd size.*/                                 
    /*lint -e574*/
    (box).left  = ((box).left  < (lcd_width)) ? (box).left   : (lcd_width) -1; 
    (box).right = ((box).right < (lcd_width)) ? (box).right  : (lcd_width) -1;  
    (box).top   = ((box).top   < (lcd_height)) ? (box).top    : (lcd_height)-1;     
    (box).bottom= ((box).bottom < (lcd_height)) ? (box).bottom : (lcd_height)-1;    
    /*lint +e574*/
    
    line_bytes = (box.right + 1  - box.left) * 4;
    height = box.bottom  + 1 - box.top; 
    temp_lcdbuf_ptr = lcdbuf_ptr + box.top * buf_width + box.left;
    while (height)
    {
//         uint32 i = 0;
// 
//         for (i = 0; i < line_bytes/4; i++)
//         {
//             *temp_lcdbuf_ptr++ = color;
//         }
//         temp_lcdbuf_ptr += (buf_width-line_bytes/4);

        MMI_MEM32SET( temp_lcdbuf_ptr, color, line_bytes );
        
        temp_lcdbuf_ptr += buf_width;

        height--;
    }
}

PUBLIC void LCD_FillRectToBuf(
                GUI_COLOR_T *lcdbuf_ptr,
                uint32 buf_width,
                uint32 lcd_width,
                uint32 lcd_height,
                GUI_RECT_T box,
                GUI_COLOR_T color
                )
{                                                                               
    GUI_COLOR_T *temp_lcdbuf_ptr = PNULL;
    uint32 line_bytes = 0;
    uint32 height = 0;
    
    /*make the box size fit the lcd size.*/                                 
    /*lint -e574*/
    (box).left  = ((box).left  < (lcd_width)) ? (box).left   : (lcd_width) -1; 
    (box).right = ((box).right < (lcd_width)) ? (box).right  : (lcd_width) -1;  
    (box).top   = ((box).top   < (lcd_height)) ? (box).top    : (lcd_height)-1;     
    (box).bottom= ((box).bottom < (lcd_height)) ? (box).bottom : (lcd_height)-1;    
    /*lint +e574*/
    
    line_bytes = (box.right + 1  - box.left) * 2;
    height = box.bottom  + 1 - box.top; 
    temp_lcdbuf_ptr = lcdbuf_ptr + box.top * buf_width + box.left;
    while (height)
    {
#ifdef WIN32    
        my_mem16set_win32(temp_lcdbuf_ptr, color, line_bytes);
#else        
//        my_mem16set(temp_lcdbuf_ptr, color, line_bytes);
        SCI_Mem16Set(temp_lcdbuf_ptr, color, line_bytes);
#endif        
        temp_lcdbuf_ptr += buf_width;
        height--;
    }
}  

#ifdef WIN32
void my_mem16set_win32(uint16* pline, uint16 color, uint32 byte_num)
{
    uint32 *pline32 = PNULL;
    uint16 *ptail = &pline[byte_num/2-1];
    uint32 color32 = (color<<16)|color;
#ifdef WIN32
    uint32 count = 0;
#endif
    
    if (((uint32)pline%2)||(0 == byte_num) || (byte_num%2))
    {
        //SCI_TRACE_LOW:"== my_mem16set_win32 == param is error: pline=0x%x, byte_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIGRAPH_2630_112_2_18_3_18_48_173,(uint8*)"dd", pline, byte_num);
        return;
    }
    if ((uint32)pline%4)
    {
        *pline = color;
        byte_num-=2;
        pline32 = (uint32*)(pline+1);
    }
    else
    {
        pline32 = (uint32*)pline;
    }
    if (byte_num%4)
    {
        *ptail = color;
        byte_num-=2;
    }
#ifdef WIN32
    count = byte_num/4;
    while(count)
    {
        *pline32++=color32;
        //        *pline32++=0;
        count--;
    }
#else
    _memset(pline32, byte_num, color32);    //32位memset
#endif
}
#endif

/*****************************************************************************/
//  Description : Fill a rectangle with one color to buffer.
//  Global resource dependence : none
//  Author: jibin
//  Note: it will not validate to display
/*****************************************************************************/
PUBLIC void LCD_FillRectTobuffer( 
                                 GUI_COLOR_T *lcdbuff_ptr,  //out
                                 uint32 lcdbuff_size,
                                 uint16 lcd_width,
                                 uint16 lcd_height,  
                                 GUI_RECT_T box,    //rectange to fill color
                                 GUI_COLOR_T color  //color to fill
                                 )
{
    if (lcdbuff_size == (lcd_width * lcd_height))
    {
        MainLcdFillRect(lcdbuff_ptr,lcd_width,lcd_height,box,color);
    }
}

LOCAL void MainLcdFillRect(
                           GUI_COLOR_T* lcdbuf_ptr,
                           uint16 lcd_width,
                           uint16 lcd_height,
                           GUI_RECT_T box,
                           GUI_COLOR_T color
                           )
{
    uint16  i          = 0;
    uint16  j          = 0;
    uint16  offset     = 0;
    uint16  box_width  = 0;
    uint16  box_height = 0;
    
    if (box.right < box.left || box.bottom < box.top)
    {
        //SCI_TRACE_LOW:"== MainLcdFillRect == the box is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIGRAPH_2697_112_2_18_3_18_48_174,(uint8*)"");
        return;
    }
    
    /*make the box size fit the lcd size.*/                                     
    box.left    = (uint16)((box.left  < (lcd_width) ) ? box.left   : (lcd_width) -1);
    box.right   = (uint16)((box.right < (lcd_width) ) ? box.right  : (lcd_width) -1);
    box.top     = (uint16)((box.top   < (lcd_height)) ? box.top    : (lcd_height)-1);   
    box.bottom  = (uint16)((box.bottom< (lcd_height)) ? box.bottom : (lcd_height)-1);   
    
    
    box_width   = (uint16)(box.right  - box.left + 1);
    box_height  = (uint16)(box.bottom - box.top  + 1);
    offset      = (uint16)(lcd_width  - box_width); // offset的计算一定要准确
    lcdbuf_ptr += (uint16)(box.top * lcd_width + box.left);
    
    SCI_ASSERT((box_width <= lcd_width) && (box_height <= lcd_height) && (offset <= lcd_width));    /*assert verified*/
    
    for(i = 0; i < box_height; i++)        
    {     
        for(j = 0; j < box_width; j++)        
        {                                               
            *(lcdbuf_ptr++) = color;          
        }        
        
        lcdbuf_ptr += offset;
    }
}

/******************************************************************************/
/*! \brief Draw a rectangle to LCD buffer.
*  \param[in]       lcdID       ID of LCD
*  \param[in]       box         rectangle to draw
*  \param[in]       color       color to draw.
*  \author  Jim.zhang
*   \note
*           it will not validate to display
*/
/******************************************************************************/
PUBLIC void LCD_DrawRect( 
                         GUI_LCD_DEV_INFO const *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to draw
                         GUI_COLOR_T                color           //color to draw
                         )
{
    LCD_DrawHLine(dev_info_ptr, box.left, box.top, box.right, color);
    LCD_DrawHLine(dev_info_ptr, box.left, box.bottom, box.right, color);
    LCD_DrawVLine(dev_info_ptr, box.left, box.top, box.bottom, color);
    LCD_DrawVLine(dev_info_ptr, box.right, box.top, box.bottom, color);
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//     Description : Draw an arrow in the middle of a rectangular area
//    Global resource dependence : none
//  Author: Geng.Ren
//    Note: The size of the arrow is fixed (4x7 for up&down, 7x4 for left&right)
/*****************************************************************************/
PUBLIC void LCD_DrawArrowInRect( 
                                GUI_LCD_DEV_INFO        *lcd_dev_info,
                                GUI_RECT_T              rect,     //rectangle to draw
                                MMI_ARROW_TYPE_E        arrowtype,    //arrow type
                                GUI_COLOR_T             color     //color to draw
                                )
{
    GUI_POINT_T     startpoint = {0};
    uint16          width = 0, height = 0;
    
    if (ARROW_LEFT==arrowtype ||ARROW_RIGHT==arrowtype)
    {
        width=4;
        height=7;
    }
    else
    {
        width=7;
        height=4;
    }
    if (width> rect.right - rect.left)
    {
        startpoint.x = rect.left;
    }
    else
    {
        startpoint.x = (int16)(((rect.right-rect.left-width+1)>>1) +rect.left);
    }
    
    if (height > rect.bottom- rect.top)
    {
        startpoint.y = rect.top;
    }
    else
    {
        startpoint.y = (int16)(((rect.bottom-rect.top-height+1)>>1) +rect.top);
    }
    
    if ( lcd_dev_info->lcd_id == GUI_MAIN_LCD_ID )
    {
        DRAW_ARROW(lcd_dev_info,startpoint,arrowtype,color);
    }
    //#ifdef    _DUAL_LCD
    else if( lcd_dev_info->lcd_id == GUI_SUB_LCD_ID )
    {
        DRAW_ARROW(lcd_dev_info,startpoint,arrowtype,color);
    }
    //#endif    //_DUAL_LCD
    else
    {
        SCI_TRACE_HIGH("LCD_DrawRect:Wrong lcd ID");
    }
    
    rect.top = startpoint.y;
    rect.left = startpoint.x;
    rect.bottom = (int16)(startpoint.y+height - 1);
    rect.right = (int16)(startpoint.x+width - 1);
    MMITHEME_StoreUpdateRect(
        lcd_dev_info,
        rect);
}

/*****************************************************************************/
//     Description : Draw an arrow 
//    Global resource dependence : none
//  Author: Geng.Ren
//    Note: The size of the arrow is fixed (4x7 for up&down, 7x4 for left&right)
/*****************************************************************************/
LOCAL void DRAW_ARROW(
                      GUI_LCD_DEV_INFO  *dev_info_ptr,
                      GUI_POINT_T       startpoint,
                      MMI_ARROW_TYPE_E  arrowtype,
                      GUI_COLOR_T       color
                      )
{
    GUI_COLOR_T *lcdbuf_ptr = PNULL;
    uint32 lcd_width = 0;
    uint32 lcd_height = 0;
    uint32 buf_width = 0;
    uint32 buf_height = 0;
    
    if (NULL == dev_info_ptr)
    {
        return;
    }

    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &buf_width, &lcd_width, &lcd_height, &lcdbuf_ptr))
    {
        return;
    }
    
//    buf_width = lcd_width;
    buf_height = lcd_height;
    
    if (startpoint.x>lcd_width-1 || startpoint.y >lcd_height-1)/*lint !e574*/
    {
        return;
    }
    startpoint = ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, startpoint);
    
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(dev_info_ptr))
    {
        uint32 *u32_lcdbuf_ptr = (uint32 *)lcdbuf_ptr;

        DrawArrowU32(u32_lcdbuf_ptr, startpoint, lcd_width, lcd_height, buf_width, buf_height, arrowtype, ConvertRgb565ToArgb888(color, 0xff));
    }
    else
#endif
    {
        DrawArrow(lcdbuf_ptr, startpoint, lcd_width, lcd_height, buf_width, buf_height, arrowtype, color);
    }
}
#endif

/*****************************************************************************/
//     Description : Draw an arrow 
//    Global resource dependence : none
//  Author: Geng.Ren
//    Note: The size of the arrow is fixed (4x7 for up&down, 7x4 for left&right)
/*****************************************************************************/
LOCAL void DrawArrowU32(
                      uint32 *lcdbuf_ptr,
                      GUI_POINT_T startpoint,
                      uint32 lcd_width,
                      uint32 lcd_height,
                      uint32 buf_width,
                      uint32 buf_height,
                      MMI_ARROW_TYPE_E  arrowtype,
                      uint32       color
                      )
{
    uint8    dotnum = 0;
    
    int16    i = 0;
    int16    j = 0;
    uint8    loop_width = 0, loop_height = 0;

    switch(arrowtype)                        
    {
    case ARROW_UP:
        loop_width = (startpoint.x + 6) > lcd_width ? (lcd_width- startpoint.x) : 7;/*lint !e574*/
        loop_height= (startpoint.y + 3) > lcd_height ? (lcd_height- startpoint.y) : 4;/*lint !e574*/
        
        for (i=0, dotnum=1; i<loop_height; i++,dotnum+=2)
        {
            for (j=startpoint.x; j<startpoint.x+dotnum; j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, j+3-i,startpoint.y+i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    case ARROW_DOWN:
        loop_width = (startpoint.x + 6) > lcd_width ? (lcd_width- startpoint.x) : 7;/*lint !e574*/
        loop_height= (startpoint.y + 3) > lcd_height ? (lcd_height- startpoint.y) : 4;/*lint !e574*/
        
        for (i=0, dotnum=7; i<loop_height; i++,dotnum-=2)
        {
            for (j=startpoint.x; j<startpoint.x+dotnum; j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, j+i,startpoint.y+i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    case ARROW_RIGHT:
        loop_width = (startpoint.x + 3) > lcd_width ? (lcd_width- startpoint.x) : 4;/*lint !e574*/
        loop_height= (startpoint.y + 6) > lcd_height ? (lcd_height- startpoint.y) : 7;/*lint !e574*/
        
        for (i=0, dotnum=7; i<loop_width; i++,dotnum-=2)
        {
            for (j=startpoint.y; j<startpoint.y+dotnum; j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, startpoint.x+i, j+i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    case ARROW_LEFT:
        loop_width = (uint8)((startpoint.x + 3) > lcd_width ? (lcd_width- startpoint.x) : 4);/*lint !e574*/
        loop_height= (uint8)((startpoint.y + 6) > lcd_height ? (lcd_height- startpoint.y) : 7);/*lint !e574*/
        
        for (i=0, dotnum=1; i<loop_width; i++,dotnum+=2)
        {
            for (j = startpoint.y; j < (int16)(startpoint.y + dotnum); j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, startpoint.x+i,j+3-i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//     Description : Draw an arrow 
//    Global resource dependence : none
//  Author: Geng.Ren
//    Note: The size of the arrow is fixed (4x7 for up&down, 7x4 for left&right)
/*****************************************************************************/
LOCAL void DrawArrow(
                      GUI_COLOR_T *lcdbuf_ptr,
                      GUI_POINT_T startpoint,
                      uint32 lcd_width,
                      uint32 lcd_height,
                      uint32 buf_width,
                      uint32 buf_height,
                      MMI_ARROW_TYPE_E  arrowtype,
                      GUI_COLOR_T       color
                      )
{
    uint8    dotnum = 0;
    
    int16    i = 0;
    int16    j = 0;
    uint8    loop_width = 0, loop_height = 0;

    switch(arrowtype)                        
    {
    case ARROW_UP:
        loop_width = (startpoint.x + 6) > lcd_width ? (lcd_width- startpoint.x) : 7;/*lint !e574*/
        loop_height= (startpoint.y + 3) > lcd_height ? (lcd_height- startpoint.y) : 4;/*lint !e574*/
        
        for (i=0, dotnum=1; i<loop_height; i++,dotnum+=2)
        {
            for (j=startpoint.x; j<startpoint.x+dotnum; j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, j+3-i,startpoint.y+i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    case ARROW_DOWN:
        loop_width = (startpoint.x + 6) > lcd_width ? (lcd_width- startpoint.x) : 7;/*lint !e574*/
        loop_height= (startpoint.y + 3) > lcd_height ? (lcd_height- startpoint.y) : 4;/*lint !e574*/
        
        for (i=0, dotnum=7; i<loop_height; i++,dotnum-=2)
        {
            for (j=startpoint.x; j<startpoint.x+dotnum; j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, j+i,startpoint.y+i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    case ARROW_RIGHT:
        loop_width = (startpoint.x + 3) > lcd_width ? (lcd_width- startpoint.x) : 4;/*lint !e574*/
        loop_height= (startpoint.y + 6) > lcd_height ? (lcd_height- startpoint.y) : 7;/*lint !e574*/
        
        for (i=0, dotnum=7; i<loop_width; i++,dotnum-=2)
        {
            for (j=startpoint.y; j<startpoint.y+dotnum; j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, startpoint.x+i, j+i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    case ARROW_LEFT:
        loop_width = (uint8)((startpoint.x + 3) > lcd_width ? (lcd_width- startpoint.x) : 4);/*lint !e574*/
        loop_height= (uint8)((startpoint.y + 6) > lcd_height ? (lcd_height- startpoint.y) : 7);/*lint !e574*/
        
        for (i=0, dotnum=1; i<loop_width; i++,dotnum+=2)
        {
            for (j = startpoint.y; j < (int16)(startpoint.y + dotnum); j++)
            {
                WR_COLOR_IN_LCD(lcdbuf_ptr, buf_width, startpoint.x+i,j+3-i, lcd_width, lcd_height, color);/*lint !e574*/
            }
        }
        break;
        
    default:
        break;
    }
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description:  fill arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_FillArc(
                        const GUI_LCD_DEV_INFO*   dev_info_ptr,
                        GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                        int16                     x,
                        int16                     y,
                        uint16                    radius,
                        uint16                    start_angle,   //起始角度
                        uint16                    end_angle,   //结束角度
                        GUI_COLOR_T               color
                        )
{
    uint32      mem_width = 0;
    uint32      lcd_width   = 0;
    uint32      lcd_height  = 0;
    GUI_RECT_T  dest_rect   = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    GUI_POINT_T startpoint = {0};
    GUI_RECT_T  clip_rect = {0};
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }
    clip_rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, *clip_rect_ptr);
    startpoint.x = x;
    startpoint.y = y;
    startpoint = ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, startpoint);
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

        dest_rect = LCD_FillArcToBufferU32( u32_block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, start_angle, end_angle, ConvertRgb565ToArgb888(color, 0xff) );
    }
    else
#endif
    {
        dest_rect = LCD_FillArcToBuffer( block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, start_angle, end_angle, color );
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, dest_rect );
}

/*****************************************************************************/
//  Description:  fill arc to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                      )
{
    return DrawArcU32( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, start_angle, end_angle, color, TRUE );
}

/*****************************************************************************/
//  Description:  fill arc to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                      )
{
    return DrawArc( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, start_angle, end_angle, color, TRUE );
}

/*****************************************************************************/
//  Description:  draw arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawArcU32(
                         uint32*          buffer_ptr,
                         uint16           buf_width,
                         uint16           buf_height,
                         GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                         int32            x,
                         int32            y,
                         int32            radius,
                         int32            start_angle,
                         int32            end_angle,
                         uint32           color,
                         BOOLEAN          is_fill
                         )  
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height
        && 0 != radius
        && start_angle != end_angle )
    {
        GUI_RECT_T buf_rect    = {0};
        GUI_RECT_T arc_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        arc_rect.left   = x - radius;
        arc_rect.right  = x + radius;
        arc_rect.top    = y - radius;
        arc_rect.bottom = y + radius;
        
        if ( PNULL != clip_rect_ptr )
        {
            if ( !GUI_IntersectRect( &clip_rect, *clip_rect_ptr, buf_rect ) )
            {
                SCI_MEMSET( &clip_rect, 0, sizeof(GUI_RECT_T) );
                
                return clip_rect;
            }
        }
        else
        {
            clip_rect = buf_rect;
        }
        
        if ( GUI_IntersectRect( &clip_rect, arc_rect, clip_rect ) )
        {
            /* start position */
            int32 sx = 0, sy = 0;
            /* current position */
            int32 px = 0, py = 0;
            /* end position */
            int32 ex = 0, ey = 0;
            /* square of radius of circle */
            int32 rr = 0;
            /* difference between main radius squared and radius squared of three
            potential next points */
            int32 rr1 = 0, rr2 = 0, rr3 = 0;
            /* square of x and of y */
            uint32 xx = 0, yy = 0, xx_new = 0, yy_new = 0;
            /* start quadrant, current quadrant and end quadrant */
            int32 sq = 0, q = 0, qe = 0;
            /* direction of movement */
            int32 dx = 0, dy = 0;
            /* temporary variable for determining if we have reached end point */
            int32 det = 0;
            
            /* Calculate the start point and the end point. */
            /* We have to flip y because bitmaps count y coordinates downwards. */
            GetPointByArc(radius, start_angle, &sx, &sy, &q);
            px = sx;
            py = sy;
            GetPointByArc(radius, end_angle, &ex, &ey, &qe);
            
            rr = radius*radius;
            xx = px*px;
            yy = py*py - rr;
            
            sq = q;
            
            if (q > qe) 
            {
                /* qe must come after q. */
                qe += 4;
            }
            else if (q == qe) 
            {
            /* If q==qe but the beginning comes after the end, make qe be
            * strictly after q.
                */
                if ( ( end_angle  < start_angle ) || (start_angle < 90 && end_angle >= 270 ) )
                {
                    qe += 4;
                }
            }
            
            /* initial direction of movement */
            if (((q+1)&2) == 0)
            {
                dy = -1;
            }
            else
            {
                dy = 1;
            }
            if ((q&2) == 0)
            {
                dx = -1;
            }
            else
            {
                dx = 1;
            }
            
            while (TRUE) /*lint !e716*/
            {
            /* Change quadrant when needed.
            * dx and dy determine the possible directions to go in this
            * quadrant, so they must be updated when we change quadrant.
                */
                if ((q&1) == 0) 
                {
                    if (px == 0) 
                    {
                        if (qe == q)
                        {
                            break;
                        }
                        q++;
                        dy = -dy;
                    }
                }
                else 
                {
                    if (py == 0) 
                    {
                        if (qe == q)
                        {
                            break;
                        }
                        q++;
                        dx = -dx;
                    }
                }
                
                /* Are we in the final quadrant? */
                if (qe == q) 
                {
                    /* Have we reached (or passed) the end point both in x and y? */
                    det = 0;
                    
                    if (dy > 0) 
                    {
                        if (py >= ey)
                        {
                            det++;
                        }
                    }
                    else 
                    {
                        if (py <= ey)
                        {
                            det++;
                        }
                    }
                    if (dx > 0) 
                    {
                        if (px >= ex)
                        {
                            det++;
                        }
                    }
                    else 
                    {
                        if (px <= ex)
                        {
                            det++;
                        }
                    }
                    
                    if (det == 2)
                    {
                        break;
                    }
                }
                
                //proc(bmp, x+px, y+py, d);
                if ( is_fill )
                {
                    DrawThickLineU32( buffer_ptr, buf_width, buf_height, x, y, x+px, y+py, color, 3, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, x+px, y+py, color );
                }
                
                /* From here, we have only 3 possible directions of movement, eg.
                * for the first quadrant:
                *
                *    .........
                *    .........
                *    ......21.
                *    ......3*.
                *
                * These are reached by adding dx to px and/or adding dy to py.
                * We need to find which of these points gives the best
                * approximation of the (square of the) radius.
                */
                
                xx_new = (px+dx) * (px+dx);
                yy_new = (py+dy) * (py+dy) - rr;
                rr1 = xx_new + yy;
                rr2 = xx_new + yy_new;
                rr3 = xx     + yy_new;
                
                /* Set rr1, rr2, rr3 to be the difference from the main radius of the
                * three points.
                */
                if (rr1 < 0)
                {
                    rr1 = -rr1;
                }
                if (rr2 < 0)
                {
                    rr2 = -rr2;
                }
                if (rr3 < 0)
                {
                    rr3 = -rr3;
                }
                
                if (rr3 >= MIN(rr1, rr2)) 
                {
                    px += dx;
                    xx = xx_new;
                }
                if (rr1 > MIN(rr2, rr3)) 
                {
                    py += dy;
                    yy = yy_new;
                }
            }
            /* Only draw last point if it doesn't overlap with first one. */
            if ((px != sx) 
                || (py != sy) 
                || (sq == qe))
            {
                //proc(bmp, x+px, y+py, d);
                if ( is_fill )
                {
                    DrawThickLineU32( buffer_ptr, buf_width, buf_height, x, y, x+px, y+py, color, 3, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, x+px, y+py, color );
                }
            }
        }
    }
    
    return clip_rect;
}

/*****************************************************************************/
//  Description:  draw arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawArc(
                         GUI_COLOR_T*     buffer_ptr,
                         uint16           buf_width,
                         uint16           buf_height,
                         GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                         int32            x,
                         int32            y,
                         int32            radius,
                         int32            start_angle,
                         int32            end_angle,
                         GUI_COLOR_T      color,
                         BOOLEAN          is_fill
                         )  
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height
        && 0 != radius
        && start_angle != end_angle )
    {
        GUI_RECT_T buf_rect    = {0};
        GUI_RECT_T arc_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        arc_rect.left   = x - radius;
        arc_rect.right  = x + radius;
        arc_rect.top    = y - radius;
        arc_rect.bottom = y + radius;
        
        if ( PNULL != clip_rect_ptr )
        {
            if ( !GUI_IntersectRect( &clip_rect, *clip_rect_ptr, buf_rect ) )
            {
                SCI_MEMSET( &clip_rect, 0, sizeof(GUI_RECT_T) );
                
                return clip_rect;
            }
        }
        else
        {
            clip_rect = buf_rect;
        }
        
        if ( GUI_IntersectRect( &clip_rect, arc_rect, clip_rect ) )
        {
            /* start position */
            int32 sx = 0, sy = 0;
            /* current position */
            int32 px = 0, py = 0;
            /* end position */
            int32 ex = 0, ey = 0;
            /* square of radius of circle */
            int32 rr = 0;
            /* difference between main radius squared and radius squared of three
            potential next points */
            int32 rr1 = 0, rr2 = 0, rr3 = 0;
            /* square of x and of y */
            uint32 xx = 0, yy = 0, xx_new = 0, yy_new = 0;
            /* start quadrant, current quadrant and end quadrant */
            int32 sq = 0, q = 0, qe = 0;
            /* direction of movement */
            int32 dx = 0, dy = 0;
            /* temporary variable for determining if we have reached end point */
            int32 det = 0;
            
            /* Calculate the start point and the end point. */
            /* We have to flip y because bitmaps count y coordinates downwards. */
            GetPointByArc(radius, start_angle, &sx, &sy, &q);
            px = sx;
            py = sy;
            GetPointByArc(radius, end_angle, &ex, &ey, &qe);
            
            rr = radius*radius;
            xx = px*px;
            yy = py*py - rr;
            
            sq = q;
            
            if (q > qe) 
            {
                /* qe must come after q. */
                qe += 4;
            }
            else if (q == qe) 
            {
            /* If q==qe but the beginning comes after the end, make qe be
            * strictly after q.
                */
                if ( ( end_angle  < start_angle ) || (start_angle < 90 && end_angle >= 270 ) )
                {
                    qe += 4;
                }
            }
            
            /* initial direction of movement */
            if (((q+1)&2) == 0)
            {
                dy = -1;
            }
            else
            {
                dy = 1;
            }
            if ((q&2) == 0)
            {
                dx = -1;
            }
            else
            {
                dx = 1;
            }
            
            while (TRUE) /*lint !e716*/
            {
            /* Change quadrant when needed.
            * dx and dy determine the possible directions to go in this
            * quadrant, so they must be updated when we change quadrant.
                */
                if ((q&1) == 0) 
                {
                    if (px == 0) 
                    {
                        if (qe == q)
                        {
                            break;
                        }
                        q++;
                        dy = -dy;
                    }
                }
                else 
                {
                    if (py == 0) 
                    {
                        if (qe == q)
                        {
                            break;
                        }
                        q++;
                        dx = -dx;
                    }
                }
                
                /* Are we in the final quadrant? */
                if (qe == q) 
                {
                    /* Have we reached (or passed) the end point both in x and y? */
                    det = 0;
                    
                    if (dy > 0) 
                    {
                        if (py >= ey)
                        {
                            det++;
                        }
                    }
                    else 
                    {
                        if (py <= ey)
                        {
                            det++;
                        }
                    }
                    if (dx > 0) 
                    {
                        if (px >= ex)
                        {
                            det++;
                        }
                    }
                    else 
                    {
                        if (px <= ex)
                        {
                            det++;
                        }
                    }
                    
                    if (det == 2)
                    {
                        break;
                    }
                }
                
                //proc(bmp, x+px, y+py, d);
                if ( is_fill )
                {
                    DrawThickLine( buffer_ptr, buf_width, buf_height, x, y, x+px, y+py, color, 3, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, x+px, y+py, color );
                }
                
                /* From here, we have only 3 possible directions of movement, eg.
                * for the first quadrant:
                *
                *    .........
                *    .........
                *    ......21.
                *    ......3*.
                *
                * These are reached by adding dx to px and/or adding dy to py.
                * We need to find which of these points gives the best
                * approximation of the (square of the) radius.
                */
                
                xx_new = (px+dx) * (px+dx);
                yy_new = (py+dy) * (py+dy) - rr;
                rr1 = xx_new + yy;
                rr2 = xx_new + yy_new;
                rr3 = xx     + yy_new;
                
                /* Set rr1, rr2, rr3 to be the difference from the main radius of the
                * three points.
                */
                if (rr1 < 0)
                {
                    rr1 = -rr1;
                }
                if (rr2 < 0)
                {
                    rr2 = -rr2;
                }
                if (rr3 < 0)
                {
                    rr3 = -rr3;
                }
                
                if (rr3 >= MIN(rr1, rr2)) 
                {
                    px += dx;
                    xx = xx_new;
                }
                if (rr1 > MIN(rr2, rr3)) 
                {
                    py += dy;
                    yy = yy_new;
                }
            }
            /* Only draw last point if it doesn't overlap with first one. */
            if ((px != sx) 
                || (py != sy) 
                || (sq == qe))
            {
                //proc(bmp, x+px, y+py, d);
                if ( is_fill )
                {
                    DrawThickLine( buffer_ptr, buf_width, buf_height, x, y, x+px, y+py, color, 3, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, x+px, y+py, color );
                }
            }
        }
    }
    
    return clip_rect;
}
#endif

//avoid lint error
/*****************************************************************************/
//  Description:  get point by arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:converting from (radius, angle) to (x, y)
/*****************************************************************************/
LOCAL void GetPointByArc(
                         int32 radius, 
                         int32 angle, 
                         int32 *out_x, 
                         int32 *out_y, 
                         int32 *out_q
                         )
{
    double s, c;
    double double_a = angle * GUI_PI /180;
    
    /*lint -e718 -e526 -e746 -e628*/
    s = sin(double_a); /*lint -esym(526,sin) -esym(628,sin)*/
    c = cos(double_a); /*lint -esym(526,cos) -esym(628,cos)*/
    /*lint +e718 +e526 +e746 +e628*/

    s = -s * radius;
    c = c * radius;
    *out_x = (int32)((c < 0) ? (c - 0.5) : (c + 0.5));
    *out_y = (int32)((s < 0) ? (s - 0.5) : (s + 0.5));
    
    if (c >= 0) 
    {
        if (s <= 0)
        {
            *out_q = 0;  /* quadrant 0 */
        }
        else
        {
            *out_q = 3;  /* quadrant 3 */
        }
    }
    else 
    {
        if (s <= 0)
        {
            *out_q = 1;  /* quadrant 1 */
        }
        else
        {
            *out_q = 2;  /* quadrant 2 */
        }
    }
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description:  draw arc
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DrawArc(
                        const GUI_LCD_DEV_INFO*   dev_info_ptr,
                        GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                        int16                     x,
                        int16                     y,
                        uint16                    radius,
                        uint16                    start_angle,   //起始角度
                        uint16                    end_angle,   //结束角度
                        GUI_COLOR_T               color
                        )
{
    uint32      mem_width = 0;
    uint32      lcd_width   = 0;
    uint32      lcd_height  = 0;
    GUI_RECT_T  dest_rect   = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    GUI_RECT_T  clip_rect   = {0};
    GUI_POINT_T startpoint = {0};
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }
    
    clip_rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, *clip_rect_ptr);
    startpoint.x = x;
    startpoint.y = y;
    startpoint = ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, startpoint);
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

        dest_rect = LCD_DrawArcToBufferU32( u32_block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, start_angle, end_angle, ConvertRgb565ToArgb888(color, 0xff) );
    }
    else
#endif
    {
        dest_rect = LCD_DrawArcToBuffer( block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, start_angle, end_angle, color );
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, dest_rect );
}

/*****************************************************************************/
//  Description:  draw arc to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                      )
{
    return DrawArcU32( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, start_angle, end_angle, color, FALSE );
}

/*****************************************************************************/
//  Description:  draw arc to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                      )
{
    return DrawArc( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, start_angle, end_angle, color, FALSE );
}
#endif

/*****************************************************************************/
//  Description: fill circle
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_FillCircle(
                           const GUI_LCD_DEV_INFO* dev_info_ptr,
                           GUI_RECT_T*             clip_rect_ptr, //剪切区域, 可以为空
                           int16                   x,
                           int16                   y,
                           uint16                  radius,
                           GUI_COLOR_T             color
                           )
{
    uint32      mem_width = 0;
    uint32      lcd_width   = 0;
    uint32      lcd_height  = 0;
    GUI_RECT_T  dest_rect   = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    GUI_RECT_T  clip_rect   = {0};
    GUI_POINT_T startpoint = {0};
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }

    clip_rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, *clip_rect_ptr);
    startpoint.x = x;
    startpoint.y = y;
    startpoint = ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, startpoint);
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

        dest_rect = LCD_FillCircleToBufferU32( u32_block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, ConvertRgb565ToArgb888(color, 0xff) );
    }
    else
#endif
    {
        dest_rect = LCD_FillCircleToBuffer( block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, color );
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, dest_rect );
}

/*****************************************************************************/
//  Description:  fill circle to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LCD_FillCircleToBufferU32(
                                         uint32*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         uint32       color
                                         )
{
    return DrawCircleU32( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, color, TRUE );
}

/*****************************************************************************/
//  Description:  fill circle to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LCD_FillCircleToBuffer(
                                         GUI_COLOR_T*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         GUI_COLOR_T       color
                                         )
{
    return DrawCircle( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, color, TRUE );
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawCircleU32(
                            uint32*      buffer_ptr,
                            uint16            buf_width,
                            uint16            buf_height,
                            GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                            int16             x,
                            int16             y,
                            uint16            radius,
                            uint32            color,
                            BOOLEAN           is_fill
                            )
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height
        && 0 != radius )
    {
        GUI_RECT_T buf_rect    = {0};
        GUI_RECT_T circle_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        circle_rect.left   = x - radius;
        circle_rect.right  = x + radius;
        circle_rect.top    = y - radius;
        circle_rect.bottom = y + radius;
        
        if ( PNULL != clip_rect_ptr )
        {
            if ( !GUI_IntersectRect( &clip_rect, *clip_rect_ptr, buf_rect ) )
            {
                SCI_MEMSET( &clip_rect, 0, sizeof(GUI_RECT_T) );
                
                return clip_rect;
            }
        }
        else
        {
            clip_rect = buf_rect;
        }
        
        if ( GUI_IntersectRect( &clip_rect, circle_rect, clip_rect ) )
        {
            int32 i = 0;
            int32 j = radius;
            int32 d = 1 - radius;
            
            for ( ; i <= j; i ++ )
            {
                if ( is_fill )
                {
                    DrawVLineU32( buffer_ptr, buf_width, clip_rect, x + i, y + j, y - j, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawVLineU32( buffer_ptr, buf_width, clip_rect, x - i, y + j, y - j, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawVLineU32( buffer_ptr, buf_width, clip_rect, x + j, y + i, y - i, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawVLineU32( buffer_ptr, buf_width, clip_rect, x - j, y + i, y - i, color, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + i, y + j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + i, y - j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - i, y + j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - i, y - j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + j, y + i, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + j, y - i, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - j, y + i, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - j, y - i, color );
                }
                
                if (d >= 0)
                {
                    d += 2 * (i - j) + 5;
                    j --;
                }
                else
                {
                    d += i * 2 + 3;
                }
            }
        }
    }
    
    return clip_rect;
}

/*****************************************************************************/
//  Description:  draw line to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note: 之前的draw line函数不支持剪切区域
//          clip_rect必须先和buffer区域进行剪切
/*****************************************************************************/
LOCAL GUI_RECT_T DrawCircle(
                            GUI_COLOR_T*      buffer_ptr,
                            uint16            buf_width,
                            uint16            buf_height,
                            GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                            int16             x,
                            int16             y,
                            uint16            radius,
                            GUI_COLOR_T       color,
                            BOOLEAN           is_fill
                            )
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height
        && 0 != radius )
    {
        GUI_RECT_T buf_rect    = {0};
        GUI_RECT_T circle_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        circle_rect.left   = x - radius;
        circle_rect.right  = x + radius;
        circle_rect.top    = y - radius;
        circle_rect.bottom = y + radius;
        
        if ( PNULL != clip_rect_ptr )
        {
            if ( !GUI_IntersectRect( &clip_rect, *clip_rect_ptr, buf_rect ) )
            {
                SCI_MEMSET( &clip_rect, 0, sizeof(GUI_RECT_T) );
                
                return clip_rect;
            }
        }
        else
        {
            clip_rect = buf_rect;
        }
        
        if ( GUI_IntersectRect( &clip_rect, circle_rect, clip_rect ) )
        {
            int32 i = 0;
            int32 j = radius;
            int32 d = 1 - radius;
            
            for ( ; i <= j; i ++ )
            {
                if ( is_fill )
                {
                    DrawVLine( buffer_ptr, buf_width, clip_rect, x + i, y + j, y - j, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawVLine( buffer_ptr, buf_width, clip_rect, x - i, y + j, y - j, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawVLine( buffer_ptr, buf_width, clip_rect, x + j, y + i, y - i, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawVLine( buffer_ptr, buf_width, clip_rect, x - j, y + i, y - i, color, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + i, y + j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + i, y - j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - i, y + j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - i, y - j, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + j, y + i, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x + j, y - i, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - j, y + i, color );
                    DRAW_POINT( buffer_ptr, buf_width, clip_rect, x - j, y - i, color );
                }
                
                if (d >= 0)
                {
                    d += 2 * (i - j) + 5;
                    j --;
                }
                else
                {
                    d += i * 2 + 3;
                }
            }
        }
    }
    
    return clip_rect;
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description: draw circle
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DrawCircle(
                           const GUI_LCD_DEV_INFO* dev_info_ptr,
                           GUI_RECT_T*             clip_rect_ptr, //剪切区域, 可以为空
                           int16                   x,
                           int16                   y,
                           uint16                  radius,
                           GUI_COLOR_T             color
                           )
{
    uint32      mem_width = 0;
    uint32      lcd_width   = 0;
    uint32      lcd_height  = 0;
    GUI_RECT_T  dest_rect   = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    GUI_RECT_T  clip_rect   = {0};
    GUI_POINT_T startpoint = {0};
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }
    
    clip_rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, *clip_rect_ptr);
    startpoint.x = x;
    startpoint.y = y;
    startpoint = ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, startpoint);
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

        dest_rect = LCD_DrawCircleToBufferU32( u32_block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, ConvertRgb565ToArgb888(color, 0xff) );
    }
    else
#endif
    {
        dest_rect = LCD_DrawCircleToBuffer( block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius, color );
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, dest_rect );
}

/*****************************************************************************/
//  Description:  draw circle to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawCircleToBufferU32(
                                         uint32*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         uint32       color
                                         )
{
    return DrawCircleU32( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, color, FALSE );
}

/*****************************************************************************/
//  Description:  draw circle to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawCircleToBuffer(
                                         GUI_COLOR_T*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //剪切区域, 可以为空
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         GUI_COLOR_T       color
                                         )
{
    return DrawCircle( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius, color, FALSE );
}

/*****************************************************************************/
//  Description:  fill ellipse
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_FillEllipse(
                            const GUI_LCD_DEV_INFO*   dev_info_ptr,
                            GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                            int16                     x,
                            int16                     y,
                            uint16                    radius_x,
                            uint16                    radius_y,
                            GUI_COLOR_T               color
                            )
{
    uint32      mem_width = 0;
    uint32      lcd_width   = 0;
    uint32      lcd_height  = 0;
    GUI_RECT_T  dest_rect   = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    GUI_RECT_T  clip_rect   = {0};
    GUI_POINT_T startpoint = {0};
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }
    
    clip_rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, *clip_rect_ptr);
    startpoint.x = x;
    startpoint.y = y;
    startpoint = ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, startpoint);
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

        dest_rect = LCD_FillEllipseToBufferU32( u32_block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius_x, radius_y, ConvertRgb565ToArgb888(color, 0xff) );
    }
    else
#endif
    {
        dest_rect = LCD_FillEllipseToBuffer( block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius_x, radius_y, color );
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, dest_rect );
}

/*****************************************************************************/
//  Description:  fill ellipse to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                          )
{
    return DrawEllipseU32( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius_x, radius_y, color, TRUE );
}

/*****************************************************************************/
//  Description:  fill ellipse to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                          )
{
    return DrawEllipse( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius_x, radius_y, color, TRUE );
}

/*****************************************************************************/
//  Description:  draw ellipse
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawEllipse( 
                             GUI_COLOR_T*     buffer_ptr,
                             uint16           buf_width,
                             uint16           buf_height,
                             GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                             int              ix, 
                             int              iy, 
                             int              radius_x, 
                             int              radius_y,
                             GUI_COLOR_T      color,
                             BOOLEAN          is_fill
                             )
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height
        && 0 != radius_x
        && 0 != radius_y )
    {
        GUI_RECT_T buf_rect     = {0};
        GUI_RECT_T ellipse_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        ellipse_rect.left   = ix - radius_x;
        ellipse_rect.right  = ix + radius_x;
        ellipse_rect.top    = iy - radius_y;
        ellipse_rect.bottom = iy + radius_y;
        
        if ( PNULL != clip_rect_ptr )
        {
            if ( !GUI_IntersectRect( &clip_rect, *clip_rect_ptr, buf_rect ) )
            {
                SCI_MEMSET( &clip_rect, 0, sizeof(GUI_RECT_T) );
                
                return clip_rect;
            }
        }
        else
        {
            clip_rect = buf_rect;
        }
        
        if ( GUI_IntersectRect( &clip_rect, ellipse_rect, clip_rect ) )
        {
            int rx, ry;
            int x, y;
            float x_change;
            float y_change;
            float ellipse_error;
            float two_a_sq;
            float two_b_sq;
            float stopping_x;
            float stopping_y;
            int midway_x = 0;
            
            rx = MAX(radius_x, 0);
            ry = MAX(radius_y, 0);
            
            two_a_sq = 2 * (float)rx * (float)rx;
            two_b_sq = 2 * (float)ry * (float)ry;
            
            x = rx;
            y = 0;
            
            x_change = (float)ry * ry * (1 - 2 * (float)rx);
            y_change = (float)rx * rx;
            ellipse_error = 0.0;
            
            /* The following two variables decide when to stop.  It's easier than
            * solving for this explicitly.
            */
            stopping_x = two_b_sq * rx;
            stopping_y = 0.0;
            
            /* First set of points. */
            while (y <= ry) 
            {
                if ( is_fill )
                {
                    DrawHLine( buffer_ptr, buf_width, clip_rect, ix + x, iy + y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawHLine( buffer_ptr, buf_width, clip_rect, ix + x, iy - y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    //proc(bmp, ix + x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy + y, color );
                    
                    //proc(bmp, ix - x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy + y, color );
                    
                    if (y != 0) 
                    {
                        //proc(bmp, ix + x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy - y, color );
                        
                        //proc(bmp, ix - x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy - y, color );
                    }
                }
                
                
                y++;
                stopping_y += two_a_sq;
                ellipse_error += y_change;
                y_change += two_a_sq;
                midway_x = x;
                
                if (stopping_x < stopping_y && x > 1) 
                {
                    break;
                }
                
                if ((2.0f * ellipse_error + x_change) > 0.0) 
                {
                    if (x) 
                    {
                        x--;
                        stopping_x -= two_b_sq;
                        ellipse_error += x_change;
                        x_change += two_b_sq;
                    }
                }
            }
            
            /* To do the other half of the ellipse we reset to the top of it, and
            * iterate in the opposite direction.
            */
            x = 0;
            y = ry;
            
            x_change = (float)ry * ry;
            y_change = (float)rx * rx * (1 - 2 * (float)ry);
            ellipse_error = 0.0;
            
            while (x < midway_x) 
            {
                if ( is_fill )
                {
                    DrawHLine( buffer_ptr, buf_width, clip_rect, ix + x, iy + y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawHLine( buffer_ptr, buf_width, clip_rect, ix + x, iy - y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    //proc(bmp, ix + x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy + y, color );
                    
                    //proc(bmp, ix - x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy + y, color );
                    
                    if (y != 0) 
                    {
                        //proc(bmp, ix + x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy - y, color );
                        //proc(bmp, ix - x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy - y, color );
                    }
                }
                
                x++;
                ellipse_error += x_change;
                x_change += two_b_sq;
                
                if ((2.0f * ellipse_error + y_change) > 0.0) 
                {
                    if (y) 
                    {
                        y--;
                        ellipse_error += y_change;
                        y_change += two_a_sq;
                    }
                }
            }
        }
    }
    
    return clip_rect;
}

/*****************************************************************************/
//  Description:  draw ellipse
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DrawEllipseU32( 
                             uint32*          buffer_ptr,
                             uint16           buf_width,
                             uint16           buf_height,
                             GUI_RECT_T*      clip_rect_ptr, //剪切区域, 可以为空
                             int              ix, 
                             int              iy, 
                             int              radius_x, 
                             int              radius_y,
                             GUI_COLOR_T      color,
                             BOOLEAN          is_fill
                             )
{
    GUI_RECT_T clip_rect = {0};
    
    if ( PNULL != buffer_ptr
        && 0 != buf_width 
        && 0 != buf_height
        && 0 != radius_x
        && 0 != radius_y )
    {
        GUI_RECT_T buf_rect     = {0};
        GUI_RECT_T ellipse_rect = {0};
        
        buf_rect.right  = buf_width - 1;
        buf_rect.bottom = buf_height - 1;
        
        ellipse_rect.left   = ix - radius_x;
        ellipse_rect.right  = ix + radius_x;
        ellipse_rect.top    = iy - radius_y;
        ellipse_rect.bottom = iy + radius_y;
        
        if ( PNULL != clip_rect_ptr )
        {
            if ( !GUI_IntersectRect( &clip_rect, *clip_rect_ptr, buf_rect ) )
            {
                SCI_MEMSET( &clip_rect, 0, sizeof(GUI_RECT_T) );
                
                return clip_rect;
            }
        }
        else
        {
            clip_rect = buf_rect;
        }
        
        if ( GUI_IntersectRect( &clip_rect, ellipse_rect, clip_rect ) )
        {
            int rx, ry;
            int x, y;
            float x_change;
            float y_change;
            float ellipse_error;
            float two_a_sq;
            float two_b_sq;
            float stopping_x;
            float stopping_y;
            int midway_x = 0;
            
            rx = MAX(radius_x, 0);
            ry = MAX(radius_y, 0);
            
            two_a_sq = 2 * (float)rx * (float)rx;
            two_b_sq = 2 * (float)ry * (float)ry;
            
            x = rx;
            y = 0;
            
            x_change = (float)ry * ry * (1 - 2 * (float)rx);
            y_change = (float)rx * rx;
            ellipse_error = 0.0;
            
            /* The following two variables decide when to stop.  It's easier than
            * solving for this explicitly.
            */
            stopping_x = two_b_sq * rx;
            stopping_y = 0.0;
            
            /* First set of points. */
            while (y <= ry) 
            {
                if ( is_fill )
                {
                    DrawHLineU32( buffer_ptr, buf_width, clip_rect, ix + x, iy + y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawHLineU32( buffer_ptr, buf_width, clip_rect, ix + x, iy - y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    //proc(bmp, ix + x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy + y, color );
                    
                    //proc(bmp, ix - x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy + y, color );
                    
                    if (y != 0) 
                    {
                        //proc(bmp, ix + x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy - y, color );
                        
                        //proc(bmp, ix - x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy - y, color );
                    }
                }
                
                
                y++;
                stopping_y += two_a_sq;
                ellipse_error += y_change;
                y_change += two_a_sq;
                midway_x = x;
                
                if (stopping_x < stopping_y && x > 1) 
                {
                    break;
                }
                
                if ((2.0f * ellipse_error + x_change) > 0.0) 
                {
                    if (x) 
                    {
                        x--;
                        stopping_x -= two_b_sq;
                        ellipse_error += x_change;
                        x_change += two_b_sq;
                    }
                }
            }
            
            /* To do the other half of the ellipse we reset to the top of it, and
            * iterate in the opposite direction.
            */
            x = 0;
            y = ry;
            
            x_change = (float)ry * ry;
            y_change = (float)rx * rx * (1 - 2 * (float)ry);
            ellipse_error = 0.0;
            
            while (x < midway_x) 
            {
                if ( is_fill )
                {
                    DrawHLineU32( buffer_ptr, buf_width, clip_rect, ix + x, iy + y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                    DrawHLineU32( buffer_ptr, buf_width, clip_rect, ix + x, iy - y, ix - x, color, GUILCD_PEN_STYLE_DEFAULT );
                }
                else
                {
                    //proc(bmp, ix + x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy + y, color );
                    
                    //proc(bmp, ix - x, iy + y, d);
                    DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy + y, color );
                    
                    if (y != 0) 
                    {
                        //proc(bmp, ix + x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix + x, iy - y, color );
                        //proc(bmp, ix - x, iy - y, d);
                        DRAW_POINT(buffer_ptr, buf_width, clip_rect, ix - x, iy - y, color );
                    }
                }
                
                x++;
                ellipse_error += x_change;
                x_change += two_b_sq;
                
                if ((2.0f * ellipse_error + y_change) > 0.0) 
                {
                    if (y) 
                    {
                        y--;
                        ellipse_error += y_change;
                        y_change += two_a_sq;
                    }
                }
            }
        }
    }
    
    return clip_rect;
}

/*****************************************************************************/
//  Description:  draw ellipse
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DrawEllipse(
                            const GUI_LCD_DEV_INFO*   dev_info_ptr,
                            GUI_RECT_T*               clip_rect_ptr, //剪切区域, 可以为空
                            int16                     x,
                            int16                     y,
                            uint16                    radius_x,
                            uint16                    radius_y,
                            GUI_COLOR_T               color
                            )
{
    uint32      mem_width = 0;
    uint32      lcd_width   = 0;
    uint32      lcd_height  = 0;
    GUI_RECT_T  dest_rect   = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    GUI_POINT_T startpoint = {0};
    GUI_RECT_T  clip_rect = {0};
    
    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr))
    {
        return;
    }
    
    clip_rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, *clip_rect_ptr);
    startpoint.x = x;
    startpoint.y = y;
    startpoint = ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, startpoint);
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_block_buffer_ptr = (uint32 *)block_buffer_ptr;

        dest_rect = LCD_DrawEllipseToBufferU32( u32_block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius_x, radius_y, ConvertRgb565ToArgb888(color, 0xff) );
    }
    else
#endif
    {
        dest_rect = LCD_DrawEllipseToBuffer( block_buffer_ptr, mem_width, lcd_height, &clip_rect, startpoint.x, startpoint.y, radius_x, radius_y, color );
    }
    
    MMITHEME_StoreUpdateRect( dev_info_ptr, dest_rect );
}

/*****************************************************************************/
//  Description:  draw ellipse to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                          )
{
    return DrawEllipse( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius_x, radius_y, color, FALSE );
}

/*****************************************************************************/
//  Description:  draw ellipse to buffer
//    Global resource dependence:
//  Author: James.Zhang
//    Note:
/*****************************************************************************/
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
                                          )
{
    return DrawEllipseU32( buffer_ptr, buf_width, buf_height, clip_rect_ptr, x, y, radius_x, radius_y, color, FALSE );
}

/*****************************************************************************/
// Description : draw a 3D style frame
// Global resource dependence : none
// Author: Geng.Ren
// Note: can not draw part of a 'button'
/*****************************************************************************/
PUBLIC void LCD_Draw3DControlFrame(
                                   GUI_LCD_DEV_INFO      *dev_info_ptr,
                                   uint16            x0, 
                                   uint16            y0, 
                                   uint16            x1,
                                   uint16            y1, 
                                   GUI_COLOR_T        fillc,     //color to fill the area.    
                                   BOOLEAN            updown    // 1: up; 0: down
                                   )
{/*lint !e578*/
    GUI_RECT_T    box = {0};
    
    GUI_COLOR_T    *buf_ptr = NULL;
    uint32 lcd_width = 0;
    uint32 lcd_height = 0;
    uint32 buf_width = 0;
    uint32 buf_height = 0;
    
    if (NULL == dev_info_ptr)
    {
        return;
    }

    if (!GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &buf_width, &lcd_width, &lcd_height, &buf_ptr))
    {
        return;
    }
    buf_height = lcd_height;
    
    box.left = (int16)(x0< x1? x0:x1);
    box.right = (int16)(x0<x1?x1:x0);
    box.top = (int16)(y0<y1?y0:y1);
    box.bottom = (int16)(y0<y1?y1:y0);
    
    if((box.left >= (int16)lcd_width) || (box.top >= (int16)lcd_height)) 
    {
        return;
    }
    box = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, box);
    
    box.right = (int16)(box.right < (int16)lcd_width?box.right:lcd_width - 1);
    box.bottom = (int16)(box.bottom < (int16)lcd_height?box.bottom:lcd_height - 1);
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {
        uint32 *u32_buf_ptr = (uint32 *)buf_ptr;

        Draw3DControlFrameU32(u32_buf_ptr, buf_width, lcd_width, lcd_height, box, ConvertRgb565ToArgb888(fillc, 0xff), updown);
    }
    else
#endif
    {
        Draw3DControlFrame(buf_ptr, buf_width, lcd_width, lcd_height, box, fillc, updown);
    }    
    MMITHEME_StoreUpdateRect(
        dev_info_ptr,
        box);    
}

/*****************************************************************************/
// Description : draw a 3D style frame
// Global resource dependence : none
// Author: Geng.Ren
// Note: can not draw part of a 'button'
/*****************************************************************************/
LOCAL void Draw3DControlFrameU32(
                                   uint32       *buf_ptr,
                                   uint32            buf_width,
                                   uint32            lcd_width,
                                   uint32            lcd_height,
                                   GUI_RECT_T        box,
                                   uint32        fillc,     //color to fill the area.    
                                   BOOLEAN            updown    // 1: up; 0: down
                                   )
{   
    GUI_RECT_T    box_a = {0};
    GUI_COLOR_T   color = fillc;

    if (fillc !=0)
    {
        
        GUI_RECT_T    dest_rect = {0};
        dest_rect.left = (int16)(box.left + 2);
        dest_rect.right = (int16)(box.right - 2);
        dest_rect.top = (int16)(box.top + 2);
        dest_rect.bottom = (int16)(box.bottom - 2);
        
        if (dest_rect.right<dest_rect.left||dest_rect.bottom<dest_rect.top)
        {
            //            SCI_TRACE_LOW("Draw3DControlFram: no room to fill!");
        }
        else
        {
            LCD_FillRectToBufU32(buf_ptr, buf_width, lcd_width,lcd_height,dest_rect,color);
        }
    }
    /******* The Colors that Used here **********
    * MMI_WHITE_COLOR            0xFFFF
    * GRAY                        0xC618
    * MMI_DARK_GRAY_COLOR    0x8410
    * MMI_BLACK_COLOR            0x0000
    ****************************************/
    /*lint -e574 -e63 -e50 -e64*/
    if (updown)
    {
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.top, box.right, 0xC618);
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.bottom, box.right, 0x0000);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left, box.top, box.bottom, 0xC618);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.right, box.top, box.bottom, 0x0000);
        
        if (box.right<(box.left+2)||box.bottom<(box.top+2))
        {
            //            SCI_TRACE_LOW("Draw3DControlFram: no room to the inner boundary!");
        }
        else
        {
            box_a = box;
            box_a.left++;
            box_a.top++;
            box_a.right--;
            box_a.bottom--;
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.top), (box_a.right), 0xFFFF);
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.bottom), (box_a.right), 0x8410);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left), (box_a.top), (box_a.bottom), 0xFFFF);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.right), (box_a.top), (box_a.bottom), 0x8410);
        }
    }
    else
    {
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.top, box.right, 0x0000);
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.bottom, box.right, 0xFFFF);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left, box.top, box.bottom, 0x0000);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.right, box.top, box.bottom, 0xFFFF);
        
        
        if (box.right<(box.left+2)||box.bottom<(box.top+2))
        {
            //            SCI_TRACE_LOW("Draw3DControlFram: no room to the inner boundary!");
        }
        else
        {
            box_a = box;
            box_a.left++;
            box_a.top++;
            box_a.right--;
            box_a.bottom--;
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.top), (box_a.right), 0x8410);
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.bottom), (box_a.right), 0xFFFF);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left), (box_a.top), (box_a.bottom), 0x8410);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.right), (box_a.top), (box_a.bottom), 0xFFFF);
        }
    }
   /*lint +e574 +e63 +e50 +e64*/
    
    box.bottom++; //?? why?? to be fixed
}

/*****************************************************************************/
// Description : draw a 3D style frame
// Global resource dependence : none
// Author: Geng.Ren
// Note: can not draw part of a 'button'
/*****************************************************************************/
LOCAL void Draw3DControlFrame(
                                   GUI_COLOR_T       *buf_ptr,
                                   uint32            buf_width,
                                   uint32            lcd_width,
                                   uint32            lcd_height,
                                   GUI_RECT_T        box,
                                   GUI_COLOR_T        fillc,     //color to fill the area.    
                                   BOOLEAN            updown    // 1: up; 0: down
                                   )
{   
    GUI_RECT_T    box_a = {0};
    GUI_COLOR_T   color = fillc;

    if (fillc !=0)
    {
        
        GUI_RECT_T    dest_rect = {0};
        dest_rect.left = (int16)(box.left + 2);
        dest_rect.right = (int16)(box.right - 2);
        dest_rect.top = (int16)(box.top + 2);
        dest_rect.bottom = (int16)(box.bottom - 2);
        
        if (dest_rect.right<dest_rect.left||dest_rect.bottom<dest_rect.top)
        {
            //            SCI_TRACE_LOW("Draw3DControlFram: no room to fill!");
        }
        else
        {
            LCD_FillRectToBuf(buf_ptr, buf_width, lcd_width,lcd_height,dest_rect,color);
        }
    }
    /******* The Colors that Used here **********
    * MMI_WHITE_COLOR            0xFFFF
    * GRAY                        0xC618
    * MMI_DARK_GRAY_COLOR    0x8410
    * MMI_BLACK_COLOR            0x0000
    ****************************************/
    /*lint -e574 -e63 -e50 -e64*/
    if (updown)
    {
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.top, box.right, 0xC618);
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.bottom, box.right, 0x0000);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left, box.top, box.bottom, 0xC618);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.right, box.top, box.bottom, 0x0000);
        
        if (box.right<(box.left+2)||box.bottom<(box.top+2))
        {
            //            SCI_TRACE_LOW("Draw3DControlFram: no room to the inner boundary!");
        }
        else
        {
            box_a = box;
            box_a.left++;
            box_a.top++;
            box_a.right--;
            box_a.bottom--;
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.top), (box_a.right), 0xFFFF);
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.bottom), (box_a.right), 0x8410);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left), (box_a.top), (box_a.bottom), 0xFFFF);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.right), (box_a.top), (box_a.bottom), 0x8410);
        }
    }
    else
    {
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.top, box.right, 0x0000);
        DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left,box.bottom, box.right, 0xFFFF);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.left, box.top, box.bottom, 0x0000);
        DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, box.right, box.top, box.bottom, 0xFFFF);
        
        
        if (box.right<(box.left+2)||box.bottom<(box.top+2))
        {
            //            SCI_TRACE_LOW("Draw3DControlFram: no room to the inner boundary!");
        }
        else
        {
            box_a = box;
            box_a.left++;
            box_a.top++;
            box_a.right--;
            box_a.bottom--;
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.top), (box_a.right), 0x8410);
            DRAW_HLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left),(box_a.bottom), (box_a.right), 0xFFFF);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.left), (box_a.top), (box_a.bottom), 0x8410);
            DRAW_VLINE(buf_ptr, buf_width, lcd_width, lcd_height, (box_a.right), (box_a.top), (box_a.bottom), 0xFFFF);
        }
    }
   /*lint +e574 +e63 +e50 +e64*/
    
    box.bottom++; //?? why?? to be fixed
}
#endif

/*****************************************************************************/
// Description :do 50% alpha blending for bmp in LCD buffer and a color
// Global resource dependence : none
// Author: Geng.Ren
// Note: 
/*****************************************************************************/
PUBLIC void LCD_MistBmp(
                        GUI_LCD_DEV_INFO  *dev_info_ptr,
                        GUI_RECT_T        rect,
                        GUI_COLOR_T       color     //color to draw.
                        )
{
    int16           i = 0, j=0;
    uint32          offset = 0;
    uint16          loop_width, loop_height;
    IMG_DISPCTL_T   buff_info = {0};
    uint32          mem_width = 0;
    uint32          lcd_width = 0;
    uint32          lcd_height = 0;
    LCD_INFO_T      lcd_info = {0};

    if (GUILCD_GetInfo((LCD_ID_E)dev_info_ptr->lcd_id, &lcd_info) )
    {
        buff_info.bits_per_pixel = (uint16)(lcd_info.bits_per_pixel);
        buff_info.r_bitmask = (uint32)(lcd_info.r_bitmask);
        buff_info.g_bitmask = (uint32)(lcd_info.g_bitmask);
        buff_info.b_bitmask = (uint32)(lcd_info.b_bitmask);
        buff_info.width = (uint16)(lcd_info.lcd_width);
        buff_info.height = (uint16)(lcd_info.lcd_height);
        buff_info.dispmem = (uint8 *)(lcd_info.lcdbuff_ptr);
    }
    
    rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, rect);
    GetLcdInfoByLcdDev(dev_info_ptr, &mem_width, &lcd_width, &lcd_height, (GUI_COLOR_T**)&buff_info.dispmem);
    if (PNULL == buff_info.dispmem)
    {
        return;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(dev_info_ptr))
    {
        buff_info.bits_per_pixel = 24;
    }
    else
#endif
    {
        buff_info.bits_per_pixel = 16;
    }
    
//    buff_info.width = lcd_width;
    buff_info.width = mem_width;
    buff_info.height = lcd_height;
    
    //Get the loop 's width and height.
    loop_width = (uint16)(rect.right > (buff_info.width -1) ? (buff_info.width - rect.left) : (rect.right - rect.left +1));
    loop_height = (uint16)(rect.bottom > (buff_info.height -1) ? buff_info.height : rect.bottom + 1);
    
    switch (buff_info.bits_per_pixel)
    {
    case 16:  //RGB565 (RGB555 is not supported currently)
        {
            uint16 *dataptr=(uint16*)(buff_info.dispmem);
            
            for (i=rect.top; i<loop_height; i++)
            {
                offset = i*buff_info.width +rect.left;
                for (j=0; j<loop_width; j++)
                {
                    *(dataptr+offset)=FASTALPHABLEND16BIT(*(dataptr+offset),color);
                    offset++;
                }
            }
        }
        break;
        
    case 24:
        {
            //add this part later.
            uint32 *dataptr=(uint32*)(buff_info.dispmem);
            uint16 buf_color = 0;
            uint8  alpha = 0;
            
            for (i=rect.top; i<loop_height; i++)
            {
                offset = i*buff_info.width +rect.left;
                for (j=0; j<loop_width; j++)
                {
                    alpha = *(dataptr+offset)>>24;
                    buf_color = RGB8882RGB565(*(dataptr+offset));
                    buf_color=FASTALPHABLEND16BIT(buf_color,color);
                    *(dataptr+offset) = RGB5652ARGB8888(buf_color, alpha);					
                    offset++;
                }
            }            
        }
        break;
        
    default:
        // buff_info.bits_per_pixel在上已经确认过了
        SCI_PASSERT(FALSE,("LCD_MistBmp: bits_per_pixel %d is error!",buff_info.bits_per_pixel));   /*assert verified*/
        break;
    }
    MMITHEME_StoreUpdateRect(dev_info_ptr, rect);
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description : copy mem to mainlcd buf
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void LCD_CopyMemToMainLcdBuf(
                                    GUI_COLOR_T *mem_buf_ptr,
                                    uint32 mem_width,
                                    uint32 mem_height,
                                    uint32 left,
                                    uint32 top,
                                    GUI_COLOR_T key_color,
                                    BOOLEAN is_final_lcdbuf    //多层的情况下最终合并后的lcd_buf
                                    )
{
    uint16              buf_width = 0;
    uint32              iX = 0;
    uint32              iY = 0;
    GUI_COLOR_T         color = 0;
    GUI_COLOR_T         *lcdbuf_ptr = PNULL;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0, 0};
    
    if (is_final_lcdbuf)
    {
        lcdbuf_ptr = GUILCD_GetMainLcdBufPtr();
    }
    else
    {
        lcdbuf_ptr = GUIBLOCK_GetBuf(&lcd_dev_info);
    }
    if (PNULL == lcdbuf_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id,&buf_width,PNULL);
    for (iY = 0; iY < mem_height; iY++)
    {
        for (iX = 0; iX < mem_width; iX++)
        {
            color = *(mem_buf_ptr + iY*mem_width + iX);
            if (key_color != color)
            {
                *(lcdbuf_ptr+(iY+top)*buf_width+(iX+left)) = color;
            }
        }
    }
}
#endif

/*****************************************************************************/
//  Description : copy mem to mainlcd buf, no key color
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void LCD_CopyMemToMainLcdBufNoKeyColor(
                                              GUI_COLOR_T *mem_buf_ptr,
                                              uint32 mem_width,
                                              uint32 mem_height,
                                              uint32 left,
                                              uint32 top,
                                              BOOLEAN is_final_lcdbuf    //多层的情况下最终合并后的lcd_buf
                                              )
{
    GUI_COLOR_T         *lcdbuf_ptr = PNULL;
    uint32              iY = 0;
    uint16              buf_width = 0;
    uint16              buf_height = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0, 0};
    
    if (is_final_lcdbuf)
    {
        lcdbuf_ptr = GUILCD_GetMainLcdBufPtr();
    }
    else
    {
        lcdbuf_ptr = GUIBLOCK_GetBuf(&lcd_dev_info);
    }
    if (PNULL == lcdbuf_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id,&buf_width,&buf_height);
    if (mem_height > buf_height)
    {
        return;
    }
    
    if ((0 == left) &&
        (0 == top) &&
        (buf_width == mem_width))
    {
        SCI_MEMCPY(lcdbuf_ptr,mem_buf_ptr,(mem_width*mem_height*sizeof(uint16)));
    }
    else
    {
        lcdbuf_ptr += top * buf_width + left;
        for (iY = 0; iY < mem_height; iY++)
        {
            SCI_MEMCPY(lcdbuf_ptr, mem_buf_ptr, (mem_width*sizeof(uint16)));
            lcdbuf_ptr += buf_width;
            mem_buf_ptr += mem_width;
        }
    }
}

/*****************************************************************************/
//  Description : copy mainlcd buf to mem
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void LCD_CopyMainLcdBufToMem(
                                    GUI_COLOR_T *mem_buf_ptr,
                                    uint32 mem_width,
                                    uint32 mem_height,
                                    uint32 left,
                                    uint32 top,
                                    BOOLEAN is_final_lcdbuf    //多层的情况下最终合并后的lcd_buf
                                    )
{
    uint16              *lcdbuf_ptr = 0;
    uint32              i = 0;
    uint16              buf_width = 0;
    uint16              buf_height = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0, 0};
    
    if (is_final_lcdbuf)
    {
        lcdbuf_ptr = GUILCD_GetMainLcdBufPtr();
    }
    else
    {
        lcdbuf_ptr = GUIBLOCK_GetBuf(&lcd_dev_info);
    }
    if (PNULL == lcdbuf_ptr)
    {
        return;
    }

    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id,&buf_width,&buf_height);
    if (mem_height > buf_height)
    {
        return;
    }

    if ((0 == left) &&
        (0 == top) &&
        (buf_width == mem_width))
    {
        SCI_MEMCPY(mem_buf_ptr,lcdbuf_ptr,(mem_width*mem_height*sizeof(uint16)));
    }
    else
    {
        lcdbuf_ptr += top * buf_width + left;
        for (i = 0; i < mem_height; i++)
        {
            SCI_MEMCPY(mem_buf_ptr, lcdbuf_ptr, (mem_width*sizeof(uint16)));
            lcdbuf_ptr += buf_width;
            mem_buf_ptr += mem_width;
        }
    }
}

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
                                  )
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_COPYBUF_T           copy_buf = {0};
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};
    
    copy_param.rect.left   = left;
    copy_param.rect.top    = top;
    copy_param.rect.right  = (int16)(left + mem_width - 1);
    copy_param.rect.bottom = (int16)(top + mem_height - 1);

    copy_buf.data_type = DATA_TYPE_RGB565;
    copy_buf.buf_ptr   = (uint8*)mem_buf_ptr;
    copy_buf.width     = mem_width;
    copy_buf.height    = mem_height;

    UILAYER_CopyLayerToBuffer(lcd_dev_ptr,&copy_param,&copy_buf);
#else
    //////////////////////////////////////////////////////////////////////////
    // added by feng.xiao for single layer condition
    if (mem_buf_ptr && GUI_MAIN_LCD_ID == lcd_dev_ptr->lcd_id)
    {
        GUI_COLOR_T* main_lcd_buff = GUILCD_GetMainLcdBufPtr();
        uint16  lcd_buff_width = 0;
        uint16  lcd_buff_height = 0;
        GUI_RECT_T  lcd_buff_rect = {0};
        GUI_RECT_T  mem_rect = {0};
        GUI_RECT_T  cross_rect = {0};
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_buff_width, &lcd_buff_height);
        lcd_buff_rect.right = lcd_buff_width;
        lcd_buff_rect.bottom = lcd_buff_height;
        mem_rect.left = left;
        mem_rect.top = top;
        mem_rect.right = left + mem_width;
        mem_rect.bottom = top + mem_height;
        //calculate cross rect to prevent from mem overflow
        if (GUI_IntersectRect(&cross_rect, lcd_buff_rect, mem_rect))
        {
            mem_width = cross_rect.right - cross_rect.left;
            mem_height = cross_rect.bottom - cross_rect.top;
            //main lcd buffer start ptr
            main_lcd_buff += top * lcd_buff_width + left;
            while (mem_height--)
            {
                SCI_MEMCPY(mem_buf_ptr, main_lcd_buff, mem_width*sizeof(GUI_COLOR_T));
                main_lcd_buff += lcd_buff_width;
                mem_buf_ptr += mem_width;
            }
        }
    }
#endif
}

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
                                  )
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_COPYBUF_T           copy_buf = {0};
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};

    copy_param.rect.left   = left;
    copy_param.rect.top    = top;
    copy_param.rect.right  = (int16)(left + mem_width - 1);
    copy_param.rect.bottom = (int16)(top + mem_height - 1);

    copy_buf.data_type = DATA_TYPE_RGB565;
    copy_buf.buf_ptr   = (uint8*)mem_buf_ptr;
    copy_buf.width     = mem_width;
    copy_buf.height    = mem_height;

    UILAYER_CopyBufferToLayer(lcd_dev_ptr,&copy_param,&copy_buf);
#else
    //////////////////////////////////////////////////////////////////////////
    // added by feng.xiao for single layer condition
    if (mem_buf_ptr && GUI_MAIN_LCD_ID == lcd_dev_ptr->lcd_id)
    {
        GUI_COLOR_T* main_lcd_buff = GUILCD_GetMainLcdBufPtr();
        uint16  lcd_buff_width = 0;
        uint16  lcd_buff_height = 0;
        GUI_RECT_T  lcd_buff_rect = {0};
        GUI_RECT_T  mem_rect = {0};
        GUI_RECT_T  cross_rect = {0};
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_buff_width, &lcd_buff_height);
        lcd_buff_rect.right = lcd_buff_width;
        lcd_buff_rect.bottom = lcd_buff_height;
        mem_rect.left = left;
        mem_rect.top = top;
        mem_rect.right = left + mem_width;
        mem_rect.bottom = top + mem_height;
        //calculate cross rect to prevent from mem overflow
        if (GUI_IntersectRect(&cross_rect, lcd_buff_rect, mem_rect))
        {
            mem_width = cross_rect.right - cross_rect.left;
            mem_height = cross_rect.bottom - cross_rect.top;
            //main lcd buffer start ptr
            main_lcd_buff += top * lcd_buff_width + left;
            while (mem_height--)
            {
                SCI_MEMCPY(main_lcd_buff, mem_buf_ptr, mem_width*sizeof(GUI_COLOR_T));
                main_lcd_buff += lcd_buff_width;
                mem_buf_ptr += mem_width;
            }
        }
    }
#endif
}

/*****************************************************************************/
//  Description : convert display rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T *LCD_ConvertDispRect(
                                       GUILCD_CONVERT_POS_TYPE_E convert_type,
                                       GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                       const GUI_RECT_T *in_rect_ptr,       //in
                                       GUI_RECT_T       *out_rect_ptr       //in
                                       )
{
    if ((PNULL != in_rect_ptr) && (PNULL != out_rect_ptr))
    {
        *out_rect_ptr = ConvertDispRect(convert_type, (GUI_LCD_DEV_INFO *)dev_info_ptr, *in_rect_ptr);
        
        return out_rect_ptr;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : convert display rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T ConvertDispRect(
                                 GUILCD_CONVERT_POS_TYPE_E convert_type,
                                 GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                 GUI_RECT_T       rect       //in
                                 )
{
    GUI_RECT_T dst_rect = {0};
    int16 offset_x = 0;
    int16 offset_y = 0;

//    if (UILAYER_IsMultiLayerHandle(dev_info_ptr))
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(dev_info_ptr))
    {
        UILAYER_GetLayerPosition(dev_info_ptr, &offset_x, &offset_y);
        
        switch (convert_type)
        {
        //相对于图层坐标转换为相对于lcd坐标
        case GUILCD_CONVERT_POS_DISP2LCD:
            dst_rect.left = rect.left + offset_x;
            dst_rect.top = rect.top + offset_y;
            dst_rect.right = rect.right + offset_x;
            dst_rect.bottom = rect.bottom + offset_y;
            break;

        //相对于lcd坐标转换为相对于图层坐标
        case GUILCD_CONVERT_POS_LCD2DISP:
            dst_rect.left = rect.left - offset_x;
            dst_rect.top = rect.top - offset_y;
            dst_rect.right = rect.right - offset_x;
            dst_rect.bottom = rect.bottom - offset_y;
            break;
            
        default:
            dst_rect = rect;
            break;
        }
    }
    else
#endif
    {
        dst_rect = rect;
    }
    return dst_rect;
}

/*****************************************************************************/
//  Description : convert display point
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T *LCD_ConvertDispPoint(
                                       GUILCD_CONVERT_POS_TYPE_E convert_type,
                                       GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                       const GUI_POINT_T *in_point_ptr,       //in
                                       GUI_POINT_T       *out_point_ptr       //in
                                       )
{
    if ((PNULL != in_point_ptr) && (PNULL != out_point_ptr))
    {
        *out_point_ptr = ConvertDispPoint(convert_type, dev_info_ptr, *in_point_ptr);
        
        return out_point_ptr;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : convert display point
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T ConvertDispPoint(
                                 GUILCD_CONVERT_POS_TYPE_E convert_type,
                                 GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                 GUI_POINT_T       point       //in
                                 )
{
    GUI_POINT_T dst_point = {0};
    int16 offset_x = 0;
    int16 offset_y = 0;

#ifdef UI_MULTILAYER_SUPPORT
//    if (UILAYER_IsMultiLayerHandle(dev_info_ptr))
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(dev_info_ptr))
    {
        UILAYER_GetLayerPosition(dev_info_ptr, &offset_x, &offset_y);
        
        switch (convert_type)
        {
        //相对于图层坐标转换为相对于lcd坐标
        case GUILCD_CONVERT_POS_DISP2LCD:
            dst_point.x = point.x + offset_x;
            dst_point.y = point.y + offset_y;
            break;

        //相对于lcd坐标转换为相对于图层坐标
        case GUILCD_CONVERT_POS_LCD2DISP:
            dst_point.x = point.x - offset_x;
            dst_point.y = point.y - offset_y;
            break;
            
        default:
            dst_point = point;
            break;
        }
    }
    else
#endif
    {
        dst_point = point;
    }
    return dst_point;
}

/*****************************************************************************/
//     Description : fill argb rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void FillArgbRectU32(
                           GUI_COLOR_T *lcdbuf_ptr,
                           uint32       buf_width,
                           uint32       lcd_width,
                           uint32       lcd_height,
                           GUI_RECT_T   box,
                           uint32       color
                           )
{                                                                               
    uint32 *temp_lcdbuf_ptr = PNULL;
    uint32 height = 0;
    uint32 width  = 0;    
    
    /*make the box size fit the lcd size.*/                                 
    /*lint -e574 -e737*/
    (box).left  = ((box).left  < (lcd_width)) ? (box).left   : (lcd_width) -1; 
    (box).right = ((box).right < (lcd_width)) ? (box).right  : (lcd_width) -1;  
    (box).top   = ((box).top   < (lcd_height)) ? (box).top    : (lcd_height)-1;     
    (box).bottom= ((box).bottom < (lcd_height)) ? (box).bottom : (lcd_height)-1;    
    /*lint +e574 +e737*/
    
    // 检测区域的有效性
    if (GUI_IsInvalidRect(box))
    {
        return;
    }

    height = box.bottom + 1 - box.top; 
    width  = box.right + 1 - box.left;

   // temp_lcdbuf_ptr = lcdbuf_ptr + box.top * buf_width + box.left;  /*lint !e64 !e737*/
    temp_lcdbuf_ptr = (uint32 *)lcdbuf_ptr;
    temp_lcdbuf_ptr += box.top * buf_width + box.left;  /*lint !e64 !e737*/

    while (height)
    {
//         dst_ptr = temp_lcdbuf_ptr;
// 
//         for ( i = 0; i < width; i++ )
//         {
//             *dst_ptr++ = color;
//         }

        MMI_MEM32SET( temp_lcdbuf_ptr, color, width * sizeof(uint32) );
        
        temp_lcdbuf_ptr += buf_width;
        height--;
    }
}  

/*****************************************************************************/
//     Description : fill argb rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void FillArgbRect(
                         GUI_COLOR_T *lcdbuf_ptr,
                         uint32       buf_width,
                         uint32       lcd_width,
                         uint32       lcd_height,
                         GUI_RECT_T   box,
                         GUI_COLOR_T  color,
                         uint8        alpha
                         )
{                                                                               
    GUI_COLOR_T *temp_lcdbuf_ptr = PNULL;
    uint32 height = 0;
    uint32 width  = 0;
    uint32 i = 0;
    
    /*make the box size fit the lcd size.*/                                 
    /*lint -e574 -e737*/
    (box).left  = ((box).left  < (lcd_width)) ? (box).left   : (lcd_width) -1; 
    (box).right = ((box).right < (lcd_width)) ? (box).right  : (lcd_width) -1;  
    (box).top   = ((box).top   < (lcd_height)) ? (box).top    : (lcd_height)-1;     
    (box).bottom= ((box).bottom < (lcd_height)) ? (box).bottom : (lcd_height)-1;    
    /*lint +e574 +e737*/
    
    height = box.bottom + 1 - box.top; 
    width  = box.right + 1 - box.left;

    temp_lcdbuf_ptr = lcdbuf_ptr + box.top * buf_width + box.left;  /*lint !e64 !e737*/

    while (height)
    {
        GUI_COLOR_T *dst_ptr = temp_lcdbuf_ptr;

        if (0 != alpha )
        {
            if( 0xFF == alpha )
            {
                for ( i = 0; i < width; i++ )
                {
                    *dst_ptr++ = color;
                }
            }
            else
            {
                for ( i = 0; i < width; i++ )
                {
                    uint8  n = (alpha + 1) >> 3;
                    uint32 result = 0;
                    uint32 x = color;
                    uint32 y = *dst_ptr;
                    
                    x = ( x | (x << 16)) & 0x7E0F81F;
                    y = ( y | (y << 16)) & 0x7E0F81F;
                    
                    result = ( (( (x - y) * n ) >> 5 ) + y) & 0x7E0F81F;
                    
                    *dst_ptr++ = ((result & 0xFFFF) | (result >> 16));
                }
            }
        }
        
        temp_lcdbuf_ptr += buf_width;
        height--;
    }
}  

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//  Description : fill argb rect to buff
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 不支持DATA_TYPE_ARGB888
/*****************************************************************************/
PUBLIC void LCD_FillDestBuffArgbRect(
                         GUI_COLOR_T *buf_ptr,
                         uint32       buf_width,
                         uint32       lcd_width,
                         uint32       lcd_height,
                         GUI_RECT_T   box,
                         GUI_COLOR_T  color,
                         uint8        alpha
                         )
{
    FillArgbRect(
        buf_ptr,
        buf_width,
        lcd_width,
        lcd_height,
        box,
        color,
        alpha
        );
}
#endif

/*****************************************************************************/
//     Description : fill argb rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_FillArgbRect(
                             GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                             GUI_RECT_T                 box,            //rectangle to fill color
                             GUI_COLOR_T                color,          //color to fill
                             uint8                      alpha
                             )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    GUI_RECT_T  main_rect = {0};
    GUI_RECT_T  dest_rect = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    
    box = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, box);
    GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_ptr);
    if (PNULL == block_buffer_ptr)
    {
        return;
    }
    
    main_rect.right  = (int16)(lcd_width - 1);
    main_rect.bottom = (int16)(lcd_height - 1);
    
    if (GUI_IntersectRect(&dest_rect, box, main_rect))
    {
#ifdef UI_MULTILAYER_SUPPORT
        MMI_MEMMONITER_START(UILAYER_GetMemCheckAddr((GUI_LCD_DEV_INFO *)dev_info_ptr));

        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
        {
            FillArgbRectU32( block_buffer_ptr, mem_width, lcd_width, lcd_height, dest_rect, ConvertRgb565ToArgb888(color, alpha) );
        }
        else
        {
            FillArgbRect( block_buffer_ptr, mem_width, lcd_width, lcd_height, dest_rect, color, alpha );
        }

        MMI_MEMMONITER_END();
#else
        FillArgbRect( block_buffer_ptr, mem_width, lcd_width, lcd_height, dest_rect, color, alpha );
#endif
        //store rect
        MMITHEME_StoreUpdateRect(dev_info_ptr,box);
    }
}

/*****************************************************************************/
//  Description : convert display point
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LCD_PrepareDrawParam(
                                 const GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                 GUILCD_DRAW_DC_T       *draw_dc_ptr       //out
                                 )
{
    uint16 mem_width = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_COLOR_T *lcd_buffer_ptr = PNULL;
    UILAYER_INFO_T layer_info = {0};
    DISPLAY_DATA_TYPE_E data_type = DATA_TYPE_RGB565;
    BOOLEAN result = FALSE;
    
    if ((PNULL == dev_info_ptr) || (PNULL == draw_dc_ptr))
    {
        return FALSE;
    }
    
#ifdef UI_MULTILAYER_SUPPORT
    switch (UILAYER_GetHandleType(dev_info_ptr))
    {
    case UILAYER_HANDLE_BLOCK:       //block handle
        GUILCD_GetLogicWidthHeight(dev_info_ptr->lcd_id,&lcd_width,&lcd_height);
        mem_width = lcd_width;
        lcd_buffer_ptr  = (GUI_COLOR_T *)GUIBLOCK_GetBuf((GUI_LCD_DEV_INFO *)dev_info_ptr);
        break;
        
    case UILAYER_HANDLE_INVALID:     //无效handle
    case UILAYER_HANDLE_MAIN:        //主层handle
    case UILAYER_HANDLE_MULTI:        //multi handle
    default:
        UILAYER_GetLayerInfo((GUI_LCD_DEV_INFO *)dev_info_ptr, &layer_info);
        lcd_buffer_ptr = (GUI_COLOR_T*)UILAYER_GetLayerBufferPtr(dev_info_ptr);
//        lcd_buffer_ptr = (GUI_COLOR_T*)layer_info.layer_buf_ptr;
        mem_width = layer_info.mem_width;
        lcd_width = layer_info.layer_width;
        lcd_height = layer_info.layer_height;       
        data_type = layer_info.data_type;
        break;
    }
#else
	GUILCD_GetLogicWidthHeight(dev_info_ptr->lcd_id,&lcd_width,&lcd_height);
	mem_width = lcd_width;
	lcd_buffer_ptr  = (GUI_COLOR_T *)GUIBLOCK_GetBuf((GUI_LCD_DEV_INFO *)dev_info_ptr);
	data_type = DATA_TYPE_RGB565;
#endif
    /*
   if (UILAYER_IsMultiLayerHandle((GUI_LCD_DEV_INFO *)dev_info_ptr) || 
       UILAYER_IsMainLayerHandle((GUI_LCD_DEV_INFO *)dev_info_ptr))
    {        
        UILAYER_GetLayerInfo((GUI_LCD_DEV_INFO *)dev_info_ptr, &layer_info);
        lcd_buffer_ptr = (GUI_COLOR_T*)layer_info.layer_buf_ptr;
        mem_width = layer_info.mem_width;
        lcd_width = layer_info.layer_width;
        lcd_height = layer_info.layer_height;       
        data_type = layer_info.data_type;
    }
    else
    {
        GUILCD_GetLogicWidthHeight(dev_info_ptr->lcd_id,&lcd_width,&lcd_height);
        mem_width = lcd_width;
        lcd_buffer_ptr  = (GUI_COLOR_T *)GUIBLOCK_GetBuf((GUI_LCD_DEV_INFO *)dev_info_ptr);
    }
    */
    draw_dc_ptr->mem_width = mem_width;
    draw_dc_ptr->width = lcd_width;
    draw_dc_ptr->height = lcd_height;
    draw_dc_ptr->lcd_buffer_ptr = lcd_buffer_ptr;
    draw_dc_ptr->data_type = data_type;

    if (PNULL != lcd_buffer_ptr)
    {
        result = TRUE;
    }

    return result;
}

#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
// 	Description : calc alpha add
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:rgb8888和rgb565,alpha叠加
/*****************************************************************************/
PUBLIC uint32 GUI_CalcAlphaAdd( 
                               uint32 u32_color,
                               uint16 u16_color,
                               uint8 alpha
                               )
{
    //用alpha_res_decoder.c ARGB8888_Decoder_Alpha_CompoundBG_Start的算法替换
    uint32 colortable_argb_value, l_dst_argb_value;
    uint32 layer1_point_value,layer1_point_alpha,layer2_point_value,layer2_point_alpha;
    uint32 compound_argb;//,counter=0;shift_value=0,

    if( 0 == alpha )
    {
        return u32_color;
    }

    layer2_point_alpha = alpha;
    colortable_argb_value = RGB5652ARGB8888( u16_color, alpha );

    if( 255 == layer2_point_alpha )
    {
        return colortable_argb_value;
    }
    
    l_dst_argb_value = u32_color;
    layer1_point_alpha=(l_dst_argb_value>>24)&0xff;

    if(0 == layer1_point_alpha)
    {
        return colortable_argb_value;
    }
    else if(255 == layer1_point_alpha)
    {
        compound_argb=0; 
        //blue
        layer1_point_value=l_dst_argb_value&0xff;
        layer2_point_value=colortable_argb_value&0xff;
        
        layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;
        
        compound_argb|=layer1_point_value&0xff;
        
        //green
        layer1_point_value=(l_dst_argb_value>>8)&0xff;
        layer2_point_value=(colortable_argb_value>>8)&0xff;
        
        
        layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;
        
        compound_argb|=(layer1_point_value&0xff)<<8;	
        
        
        //red
        layer1_point_value=(l_dst_argb_value>>16)&0xff;
        layer2_point_value=(colortable_argb_value>>16)&0xff;
        
        
        layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;
        
        compound_argb|=(layer1_point_value&0xff)<<16;	
        
        //alpha
        compound_argb|=layer1_point_alpha<<24;	
    }
    else
    {
        
        uint32 compound_alpha = 0;
        uint32 alpha_div_factor = 0;
        compound_argb=0; 
        
        //caculate alpha
        compound_alpha = (255-layer1_point_alpha) * (255 - layer2_point_alpha) ;
        compound_alpha = (compound_alpha+128) >> 8;
        //get normal alpha & value
        compound_alpha= 255 - compound_alpha;		
        
        alpha_div_factor = (1 << 16) / compound_alpha;
        
        //blue
        layer1_point_value=l_dst_argb_value&0xff;
        layer2_point_value=colortable_argb_value&0xff;
        
        layer1_point_value = layer1_point_value * layer1_point_alpha * (255-layer2_point_alpha) + ((layer2_point_value * layer2_point_alpha)<<8);
        layer1_point_value = (layer1_point_value+32768) >> 16;
        layer1_point_value = (((layer1_point_value<<8)+(compound_alpha>>1)) * alpha_div_factor) >> 16;
        
        compound_argb|=layer1_point_value&0xff;
        
        //green
        layer1_point_value=(l_dst_argb_value>>8)&0xff;
        layer2_point_value=(colortable_argb_value>>8)&0xff;
        
        layer1_point_value = layer1_point_value * layer1_point_alpha * (255-layer2_point_alpha) + ((layer2_point_value * layer2_point_alpha)<<8);
        layer1_point_value = (layer1_point_value+32768) >> 16;
        layer1_point_value = (((layer1_point_value<<8)+(compound_alpha>>1)) * alpha_div_factor) >> 16;
        
        compound_argb|=(layer1_point_value&0xff)<<8;	
        
        //red
        layer1_point_value=(l_dst_argb_value>>16)&0xff;
        layer2_point_value=(colortable_argb_value>>16)&0xff;
        
        layer1_point_value = layer1_point_value * layer1_point_alpha * (255-layer2_point_alpha) + ((layer2_point_value * layer2_point_alpha)<<8);
        layer1_point_value = (layer1_point_value+32768) >> 16;
        layer1_point_value = (((layer1_point_value<<8)+(compound_alpha>>1)) * alpha_div_factor) >> 16;
        
        compound_argb|=(layer1_point_value&0xff)<<16;	
        
        //alpha
        compound_argb|=compound_alpha<<24;        
    }

    return compound_argb;
}
#endif
/*****************************************************************************/
// 	Description : 计算两个ARGB888颜色叠加后的值
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
//  用的是底层的算法，是否要底层提供？
/*****************************************************************************/
PUBLIC uint32 GUI_CompoundARGB888(
                                  uint32    down_color,
                                  uint32    up_color
                                  )
{
	uint32 colortable_argb_value;
	uint32 l_dst_argb_value;
	uint32 layer1_point_value;
	uint32 layer1_point_alpha;
	uint32 layer2_point_value;
	uint32 layer2_point_alpha;
	uint32 compound_argb=0;
    uint32 dst_color = 0;

	colortable_argb_value = up_color;
	layer2_point_alpha=(colortable_argb_value>>24)&0xff;

	if(0 == layer2_point_alpha)
   	{
		dst_color = down_color;
	}
	else if(255 == layer2_point_alpha)
	{
		dst_color = up_color;
	}
    else
    {
    	l_dst_argb_value = down_color;
    	layer1_point_alpha=(l_dst_argb_value>>24)&0xff;

    	if(0 == layer1_point_alpha)
    	{				
    		dst_color = up_color;
    	}
    	else if (255 == layer1_point_alpha)
    	{
    		compound_argb=0; 

           //blue
          	layer1_point_value=l_dst_argb_value&0xff;
          	layer2_point_value=colortable_argb_value&0xff;

    		layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;
    			
    		compound_argb|=layer1_point_value&0xff;

		       //green
	     	layer1_point_value=(l_dst_argb_value>>8)&0xff;
     		layer2_point_value=(colortable_argb_value>>8)&0xff;

    		layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;

    		compound_argb|=(layer1_point_value&0xff)<<8;	
    			
    		//red
	      	layer1_point_value=(l_dst_argb_value>>16)&0xff;
     		layer2_point_value=(colortable_argb_value>>16)&0xff;

    		layer1_point_value = (((layer2_point_value -layer1_point_value)* layer2_point_alpha)>>8)+ layer1_point_value;
    			
    		compound_argb|=(layer1_point_value&0xff)<<16;	
    			
			//alpha
    		compound_argb|=layer1_point_alpha<<24;	
    								
    		dst_color = compound_argb;
    	}
    	else
    	{				
			uint32 compound_alpha = 0;
			uint32 alpha_div_factor = 0;
			compound_argb=0; 
			
			//caculate alpha
			compound_alpha = (255-layer1_point_alpha) * (255 - layer2_point_alpha) ;
			compound_alpha = (compound_alpha+128) >> 8;
			//get normal alpha & value
			compound_alpha= 255 - compound_alpha;		

			alpha_div_factor = (1 << 16) / compound_alpha;             
			
			//blue
	        layer1_point_value=l_dst_argb_value&0xff;
	        layer2_point_value=colortable_argb_value&0xff;
				  
			layer1_point_value = layer1_point_value * layer1_point_alpha * (255-layer2_point_alpha) + ((layer2_point_value * layer2_point_alpha)<<8);
			layer1_point_value = (layer1_point_value+32768) >> 16;
			layer1_point_value = (((layer1_point_value<<8)+(compound_alpha>>1)) * alpha_div_factor) >> 16;

			compound_argb|=layer1_point_value&0xff;
			
			//green
	        layer1_point_value=(l_dst_argb_value>>8)&0xff;
	        layer2_point_value=(colortable_argb_value>>8)&0xff;
				  
			layer1_point_value = layer1_point_value * layer1_point_alpha * (255-layer2_point_alpha) + ((layer2_point_value * layer2_point_alpha)<<8);
			layer1_point_value = (layer1_point_value+32768) >> 16;
			layer1_point_value = (((layer1_point_value<<8)+(compound_alpha>>1)) * alpha_div_factor) >> 16;
			
			compound_argb|=(layer1_point_value&0xff)<<8;	
			
			//red
	        layer1_point_value=(l_dst_argb_value>>16)&0xff;
	        layer2_point_value=(colortable_argb_value>>16)&0xff;
			
			layer1_point_value = layer1_point_value * layer1_point_alpha * (255-layer2_point_alpha) + ((layer2_point_value * layer2_point_alpha)<<8);
			layer1_point_value = (layer1_point_value+32768) >> 16;
			layer1_point_value = (((layer1_point_value<<8)+(compound_alpha>>1)) * alpha_div_factor) >> 16;
			
			compound_argb|=(layer1_point_value&0xff)<<16;	
			
			//alpha
			compound_argb|=compound_alpha<<24;

    		dst_color = compound_argb;
    	}
    }

	return dst_color;
}

/*****************************************************************************/
// 	Description : draw common rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: line width only support 2,must width >=10,height>=12
/*****************************************************************************/
PUBLIC void LCD_DrawRoundedRect(
                                GUI_LCD_DEV_INFO const  *dev_info_ptr,
                                GUI_RECT_T              box,
                                GUI_RECT_T              display_rect,
                                GUI_COLOR_T             color
                                )
{
    //draw rounded rect
    DrawRoundedRect(dev_info_ptr,FALSE,box,display_rect,color,RECT_STYLE_NORMAL);
}

/*****************************************************************************/
// 	Description : draw common rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: line width only support 2,must width >=10,height>=12
/*****************************************************************************/
PUBLIC void LCD_DrawRoundedRectToBuf(
                                     GUI_COLOR_T    *buf_ptr,
                                     uint16         buf_width,
                                     uint16         buf_height,
                                     GUI_RECT_T     box,
                                     GUI_COLOR_T    color
                                     )
{
    //draw rounded rect
    DrawRoundedRectToBuf(buf_ptr,buf_width,buf_height,FALSE,box,color,RECT_STYLE_NORMAL);
}

/*****************************************************************************/
// 	Description : draw shadow rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: line width only support 2,must width >=10,height>=12
/*****************************************************************************/
PUBLIC void LCD_DrawShadowRoundedRect(
                                      GUI_LCD_DEV_INFO const    *dev_info_ptr,
                                      GUI_RECT_T                box,
                                      GUI_RECT_T                display_rect,
                                      GUI_COLOR_T               color
                                      )
{
    //draw rounded rect
    DrawRoundedRect(dev_info_ptr,FALSE,box,display_rect,color,RECT_STYLE_SHADOW);
}

/*****************************************************************************/
// 	Description : draw shadow rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: line width only support 2,must width >=10,height>=12
/*****************************************************************************/
PUBLIC void LCD_DrawShadowRoundedRectToBuf(
                                           GUI_COLOR_T  *buf_ptr,
                                           uint16       buf_width,
                                           uint16       buf_height,
                                           GUI_RECT_T   box,
                                           GUI_COLOR_T  color
                                           )
{
    //draw rounded rect
    DrawRoundedRectToBuf(buf_ptr,buf_width,buf_height,FALSE,box,color,RECT_STYLE_SHADOW);
}

/*****************************************************************************/
// 	Description : draw rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawRoundedRect(
                              GUI_LCD_DEV_INFO const    *dev_info_ptr,
                              BOOLEAN                   is_fill_rect,
                              GUI_RECT_T                box,
                              GUI_RECT_T                display_rect,
                              GUI_COLOR_T               color,
                              RECT_STYLE_E              style
                              )
{
    BOOLEAN             result = FALSE;
    uint32              mem_width = 0;
    uint32              lcd_width = 0;
    uint32              lcd_height = 0;
    GUI_RECT_T          cross_rect = {0};
    GUI_COLOR_T         *block_buffer_ptr = PNULL;
    DRAW_RRECT_IN_T     rrect_in = {0};
    DRAW_RRECT_OUT_T    rrect_out = {0};

    if ((GUI_IntersectRect(&cross_rect,box,display_rect)) &&
        (GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr,&mem_width,&lcd_width,&lcd_height,&block_buffer_ptr)))
    {
        //convert rect
        box = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP,(GUI_LCD_DEV_INFO *)dev_info_ptr,box);
        cross_rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP,(GUI_LCD_DEV_INFO *)dev_info_ptr,cross_rect);
        if ((cross_rect.right >= cross_rect.left) &&
            (cross_rect.bottom >= cross_rect.top))
        {
            //set rect
            rrect_in.rect.x = box.left;
            rrect_in.rect.y = box.top;
            rrect_in.rect.w = (uint16)(box.right - box.left + 1);
            rrect_in.rect.h = (uint16)(box.bottom - box.top + 1);

            //set visible rect
            rrect_in.visible_rect.x = cross_rect.left;
            rrect_in.visible_rect.y = cross_rect.top;
            rrect_in.visible_rect.w = (uint16)(cross_rect.right - cross_rect.left + 1);
            rrect_in.visible_rect.h = (uint16)(cross_rect.bottom - cross_rect.top + 1);

            rrect_in.rect_style = style;
            rrect_in.color = ConvertRgb565ToArgb888(color,0xFF);
            
            //set bk
            rrect_in.bk_size.w  = (uint16)mem_width;
            rrect_in.bk_size.h  = (uint16)lcd_height;
            rrect_in.bk_buf.ptr = block_buffer_ptr;
#ifdef UI_MULTILAYER_SUPPORT
            if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType((GUI_LCD_DEV_INFO *)dev_info_ptr))
            {
                rrect_in.bk_format = IMGREF_FORMAT_ARGB888;
                rrect_in.bk_buf.size = lcd_width*lcd_height*4;

                //set alpha
                rrect_in.alpha_option = RECT_ALPHA_MIXED;
            }
            else
#endif
            {
                rrect_in.bk_format = IMGREF_FORMAT_RGB565;
                rrect_in.bk_buf.size = lcd_width*lcd_height*2;

                //set alpha
                rrect_in.alpha_option = RECT_ALPHA_FOREGROUND;
            }

            if (is_fill_rect)
            {
                //fill rounded rect
                GRAPH_FillRoundRect(&rrect_in,&rrect_out);
            }
            else
            {
                //draw rounded rect
                GRAPH_DrawRoundRect(&rrect_in,&rrect_out);
            }

            //store rect
            MMITHEME_StoreUpdateRect(dev_info_ptr,cross_rect);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : draw rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawRoundedRectToBuf(
                                   GUI_COLOR_T      *buf_ptr,
                                   uint16           buf_width,
                                   uint16           buf_height,
                                   BOOLEAN          is_fill_rect,
                                   GUI_RECT_T       box,
                                   GUI_COLOR_T      color,
                                   RECT_STYLE_E     style
                                   )
{
    BOOLEAN             result = FALSE;
    DRAW_RRECT_IN_T     rrect_in = {0};
    DRAW_RRECT_OUT_T    rrect_out = {0};

    if ((box.right >= box.left) &&
        (box.bottom > box.top))
    {
        box.left   = (int16)((box.left   < buf_width)  ? box.left   : (buf_width - 1));
        box.right  = (int16)((box.right  < buf_width)  ? box.right  : (buf_width - 1));
        box.top    = (int16)((box.top    < buf_height) ? box.top    : (buf_height - 1));
        box.bottom = (int16)((box.bottom < buf_height) ? box.bottom : (buf_height - 1));

        //set rect
        rrect_in.rect.x = box.left;
        rrect_in.rect.y = box.top;
        rrect_in.rect.w = (uint16)(box.right - box.left + 1);
        rrect_in.rect.h = (uint16)(box.bottom - box.top + 1);

        //set visible rect
        rrect_in.visible_rect = rrect_in.rect;

        rrect_in.rect_style = style;
        rrect_in.color = ConvertRgb565ToArgb888(color,0xFF);

        //set alpha
        rrect_in.alpha_option = RECT_ALPHA_FOREGROUND;

        //set bk
        rrect_in.bk_size.w   = (uint16)buf_width;
        rrect_in.bk_size.h   = (uint16)buf_height;
        rrect_in.bk_buf.ptr  = buf_ptr;
        rrect_in.bk_format   = IMGREF_FORMAT_RGB565;
        rrect_in.bk_buf.size = buf_width*buf_height*2;

        if (is_fill_rect)
        {
            //fill rounded rect
            GRAPH_FillRoundRect(&rrect_in,&rrect_out);
        }
        else
        {
            //draw rounded rect
            GRAPH_DrawRoundRect(&rrect_in,&rrect_out);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : fill common rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: must width >=10,height>=12
/*****************************************************************************/
PUBLIC void LCD_FillRoundedRect(
                                GUI_LCD_DEV_INFO const  *dev_info_ptr,
                                GUI_RECT_T              box,
                                GUI_RECT_T              display_rect,
                                GUI_COLOR_T             color
                                )
{
    //draw rounded rect
    DrawRoundedRect(dev_info_ptr,TRUE,box,display_rect,color,RECT_STYLE_NORMAL);
}

/*****************************************************************************/
// 	Description : fill shadow rounded rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: must width >=10,height>=12
/*****************************************************************************/
PUBLIC void LCD_FillShadowRoundedRect(
                                      GUI_LCD_DEV_INFO const    *dev_info_ptr,
                                      GUI_RECT_T                box,
                                      GUI_RECT_T                display_rect,
                                      GUI_COLOR_T               color
                                      )
{
    //draw rounded rect
    DrawRoundedRect(dev_info_ptr,TRUE,box,display_rect,color,RECT_STYLE_SHADOW);
}

/*lint +e737 +e578*/
/////////////////////////////////////////////////////////////////////////////////
//Draw Dot functions
/////////////////////////////////////////////////////////////////////////////////
#define ARGB_GET_R(_color) ((uint8)((_color) >> 16))
#define ARGB_GET_G(_color) ((uint8)((_color) >> 8))
#define ARGB_GET_B(_color) ((uint8)(_color))

#define ARGB_SET_R(_color, _r) ((_color) | ((_r) << 16))
#define ARGB_SET_G(_color, _g) ((_color) | ((_g) << 8))
#define ARGB_SET_B(_color, _b) ((_color) | (_b))

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define ALPHA_BLENDING(_x1, _x2, _a) (((((_x1) - (_x2)) * (_a)) >> 8) + (_x2))

#define DOT_MIN_WIDTH 1
#define DOT_MAX_WIDTH 7

LOCAL const uint8 s_dot_tbl[DOT_MAX_WIDTH][DOT_MAX_WIDTH * DOT_MAX_WIDTH] = 
{
    // pen_width = 1
    {
        192
    },	/*lint !e785*/

    // pen_width = 2
    {
        208,    208,
        208,    208
    },	/*lint !e785*/

     // pen_width = 3
    {
        128,    240,    128,    
        240,    255,    240,    
        128,    240,    128
    },	/*lint !e785*/

    // pen_width = 4
    {
        96,     224,    224,    96,
        224,    255,    255,    224,
        224,    255,    255,    224,
        96,     224,    224,    96,
    },	/*lint !e785*/

    // pen_width = 5
    {
        32,     192,    248,    192,     32,
        192,    255,    255,    255,    192,
        248,    255,    255,    255,    248,
        192,    255,    255,    255,    192,
        32,     192,    248,    192,     32
    },	/*lint !e785*/

    // pen_width = 6
    {
        16,     144,    240,    240,     144,   16,
        144,    255,    255,    255,     255,   144,
        240,    255,    255,    255,     255,   240,
        240,    255,    255,    255,     255,   240,
        144,    255,    255,    255,     255,   144,
        16,     144,    240,    240,     144,   16
    },	/*lint !e785*/

    // pen_width = 7
    {
        0,      144,    224,    240,     224,   144,    0,
        144,    255,    255,    255,     255,   255,    144,
        224,    255,    255,    255,     255,   255,    224,
        240,    255,    255,    255,     255,   255,    240,
        224,    255,    255,    255,     255,   255,    224,
        144,    255,    255,    255,     255,   255,    144,
        0,      144,    224,    240,     224,   144,    0
    }	/*lint !e785*/
};

LOCAL void DrawDotU32(int16 x, int16 y, uint8 pen_width, uint32 pen_color,
                uint32 *mem_ptr, uint16 mem_width, uint16 mem_height)
{
    const uint8 *dot_tbl_ptr = NULL;
    int16 dot_x = 0;
    int16 dot_y = 0;
    int16 dot_w = 0;
    int16 dot_h = 0;
    uint16 i, j;

    if (NULL == mem_ptr)
    {
        return;
    }
    
    pen_width = (pen_width < DOT_MIN_WIDTH) ? DOT_MIN_WIDTH : pen_width;
    pen_width = (pen_width > DOT_MAX_WIDTH) ? DOT_MAX_WIDTH : pen_width;

    dot_w = (int16)pen_width;
    dot_h = (int16)pen_width;

    x = (x - (int16)pen_width / 2);
    y = (y - (int16)pen_width / 2);	
        
    dot_tbl_ptr = s_dot_tbl[pen_width - 1];
   
    if (x < 0)
    {
        dot_x = -x;
        dot_w -= dot_x;      
        x = 0;
    }

    if (y < 0)
    {
        dot_y = -y;
        dot_h -= dot_y;
        y = 0;
    }

    if (x + dot_w > (int16)mem_width)
    {
        dot_w = (int16)mem_width - x;

    }

    if (y + dot_h > (int16)mem_height)
    {
        dot_h = (int16)mem_height - y;
    }

    dot_w = (dot_w < 0) ? 0 : dot_w;
    dot_h = (dot_h < 0) ? 0 : dot_h;

    mem_ptr += ((int16)mem_width * y + x);
    dot_tbl_ptr += ((int16)pen_width * dot_y + dot_x);

    for (i=0; i<dot_h; i++)
    {
        for (j=0; j<dot_w; j++)
        {
            uint8 alpha = *(dot_tbl_ptr + j);
            uint32 back_color = *(mem_ptr + j);
            uint32 result = 0;
            uint8 back = 0, fore = 0;
            
            //alpha
            result = 0xff000000;     //alpha = 255

            //r
            back = ARGB_GET_R(back_color);
            fore = ARGB_GET_R(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = ARGB_SET_R(result, back);

            //g
            back = ARGB_GET_G(back_color);
            fore = ARGB_GET_G(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = ARGB_SET_G(result, back);
            
            //b
            back = ARGB_GET_B(back_color);
            fore = ARGB_GET_B(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = ARGB_SET_B(result, back);
      
            *(mem_ptr + j) = result;                        
        }

        mem_ptr += mem_width;
        dot_tbl_ptr += pen_width;
    }
}

LOCAL void DrawDot(int16 x, int16 y, uint8 pen_width, uint16 pen_color,
             uint16 *mem_ptr, uint16 mem_width, uint16 mem_height)
{
    const uint8 *dot_tbl_ptr = NULL;
    int16 dot_x = 0;
    int16 dot_y = 0;
    int16 dot_w = 0;
    int16 dot_h = 0;
    uint16 i, j;
    
    if (NULL == mem_ptr)
    {
        return;
    }

    pen_width = (pen_width < DOT_MIN_WIDTH) ? DOT_MIN_WIDTH : pen_width;
    pen_width = (pen_width > DOT_MAX_WIDTH) ? DOT_MAX_WIDTH : pen_width;

    dot_w = (int16)pen_width;
    dot_h = (int16)pen_width;

    x = (x - (int16)pen_width / 2);
    y = (y - (int16)pen_width / 2);
        
    dot_tbl_ptr = s_dot_tbl[pen_width - 1];
   
    if (x < 0)
    {
        dot_x = -x;
        dot_w -= dot_x;      
        x = 0;
    }

    if (y < 0)
    {
        dot_y = -y;
        dot_h -= dot_y;
        y = 0;
    }

    if (x + dot_w > (int16)mem_width)
    {
        dot_w = (int16)mem_width - x;
    }

    if (y + dot_h > (int16)mem_height)
    {
        dot_h = (int16)mem_height - y;
    }

    dot_w = (dot_w < 0) ? 0 : dot_w;
    dot_h = (dot_h < 0) ? 0 : dot_h;

    mem_ptr += ((int16)mem_width * y + x);
    dot_tbl_ptr += ((int16)pen_width * dot_y + dot_x);

    for (i=0; i<dot_h; i++)
    {
        for (j=0; j<dot_w; j++)
        {
            uint8 alpha = *(dot_tbl_ptr + j);
            uint16 back_color = *(mem_ptr + j);
            uint16 result= 0;
            uint8 back = 0, fore = 0;
            
            //r
            back = RGB565_GET_R(back_color);
            fore = RGB565_GET_R(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = RGB565_SET_R(result, back);

            //g
            back = RGB565_GET_G(back_color);
            fore = RGB565_GET_G(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = RGB565_SET_G(result, back);
            
            //b
            back = RGB565_GET_B(back_color);
            fore = RGB565_GET_B(pen_color);
            back = ALPHA_BLENDING(fore, back, alpha);
            result = RGB565_SET_B(result, back);
      
            *(mem_ptr + j) = result;                        
        }

        mem_ptr += mem_width;
        dot_tbl_ptr += pen_width;
    }
}


/////////////////////////////////////////////////////////////////////////////////
//Draw Line functions
/////////////////////////////////////////////////////////////////////////////////
__inline void blend_color_U32(
                                    uint32 *buffer_ptr,
                                    uint32 buf_width,
                                    uint32 lcd_width,
                                    uint32 lcd_height,
                                    int16 x,
                                    int16 y,
                                    uint32 color,
                                    fixed alpha                                  
                                  )
{
    uint8 alpha_value = alpha >> 8;
    uint32 result = 0;

    if(0 == alpha_value)	return;

	result = (color & 0xffffff) | (alpha_value << 24);
    WR_COLOR_IN_LCD(buffer_ptr, (int32)buf_width, x, y, (int32)lcd_width, (int32)lcd_height, result);	 
}

__inline void fill_color_U32(
                        uint32 *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        int16 x,
                        int16 y,
                        uint32 color
                        )
{
    WR_COLOR_IN_LCD(buffer_ptr, (int32)buf_width, x, y, (int32)lcd_width, (int32)lcd_height, color);
}

/*****************************************************************************/
//   Description : Get update rect of draw style line
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void GetLineRect(int16 x1, int16 y1, int16 x2, int16 y2, uint16 width,
                   uint16 mem_width, uint16 mem_height, GUI_RECT_T *update_rect_ptr)
{
    if (NULL != update_rect_ptr)
    {
        if (x1 > x2)
        {
            int16 temp;             
            SWAP(x1, x2, temp);
        }

        if (y1 > y2)
        {
            int16 temp;             
            SWAP(y1, y2, temp);
        }

        x1 -= width;
        x2 += width;
        y1 -= width;
        y2 += width;

        update_rect_ptr->left = x1;
        update_rect_ptr->right = x2;
        update_rect_ptr->top = y1;
        update_rect_ptr->bottom = y2;        
    }
}
#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//   Description : Draw circle end cap for aa thick lines
//   Global resource dependence : none
//   Author: Geng.Ren
//   Note:   
/*****************************************************************************/
LOCAL void DrawEndCapAAU32( 
                       int16 x,             //x coordinate of one end point
                       int16 y,             //y coordinage of one end point
                       uint32            color,     //color to draw.
                       uint16            width,
                       uint32             *mem_ptr,
                       uint16           mem_width,
                       uint16           mem_height,                
                       GUI_RECT_T       *update_rect_ptr
                       )

{
    fixed*      pattern_ptr = PNULL;//patterns[width-1];
    int16 xx, yy, x1, y1, x2, y2, stride;

	if(width > CAP_PATTERN_MAX )
	{
		return;
	}

	pattern_ptr = (fixed*)patterns[width-1]; 

    if ( PNULL == mem_ptr) return;

    width >>= 1;
    stride = (width<<1) + 1;

    x1 = x - (int32)width;
    y1 = y - (int32)width;
    x2 = x + (int32)width;
    y2 = y + (int32)width;

    for(xx=x1; xx<=x2; xx++)
    {
        for(yy=y1; yy<=y2; yy++)
        {
			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color, pattern_ptr[(xx-x1)+(yy-y1)*stride]);
        }
    }     
   
    if (NULL != update_rect_ptr)
    {
        update_rect_ptr->left   = x1;
        update_rect_ptr->top    = y1;
        update_rect_ptr->right  = x2;
        update_rect_ptr->bottom = y2;
    }
}
#endif

/*****************************************************************************/
//   Description : Draw thick line on ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawThickLineAAU32(int16           x1,             //x coordinate of first point
			                        int16           y1,             //y coordinage of first point
			                        int16           x2,             //x coordinate of second point
			                        int16           y2,             //y coordinate of sconcd point
			                         uint32           color,           //color to draw.
			                         uint16           width,           //unit: pixel
			                         uint32           *mem_ptr,
			                         uint16           mem_width,
			                         uint16           mem_height,
			                         GUI_RECT_T       *update_rect_ptr)
{
    GUI_RECT_T  rect = {0};

    if((x1==x2)&&(y1==y2))
        return;

    if ( PNULL == mem_ptr) return;
           
    DrawEndCapAAU32(x1, y1, color, width, mem_ptr, mem_width, mem_height, &rect);
    DrawEndCapAAU32(x2, y2, color, width, mem_ptr, mem_width, mem_height, &rect);

    {
        int dx = x2 - x1;
        int dy = y2 - y1;

        int len = isqrt(dx*dx+dy*dy);

		fixed slope, span, unit, step;
        fixed start_x, start_y, end_y, end_x, cur_x, cur_y;
        fixed start_x_align, start_y_align, end_x_align, end_y_align;

    	if(ABS(dx)>ABS(dy))
    	{
            if(x1>x2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dy,dx);
            span  = fixed_idiv(width*len,dx)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_x   = fixed_cvt_i2x(x2);
        	start_x = fixed_cvt_i2x(x1);
       		start_y = fixed_cvt_i2x(y1) - span/2 + FIXED_HALF;
            end_y   = fixed_cvt_i2x(y2) + span/2 + FIXED_HALF;

            start_x_align = fixed_floor(start_x);
            end_x_align   = fixed_floor(end_x);
            cur_y         = start_y + fixed_mul(start_x_align-start_x,slope);

        	for(cur_x=start_x_align; cur_x<end_x_align; cur_x+=FIXED_ONE, cur_y+=slope)
        	{
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_y),slope));
        		start_y_align = fixed_floor(cur_y) + unit;
        		if(fixed_mass(cur_y) != fixed_mass(cur_y+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+slope), color, FIXED_ONE - coverage(fixed_frac(cur_y+slope),slope));
        			start_y_align += unit;
        		}
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span+slope), color, coverage(fixed_frac(cur_y+span+slope),slope));
        		end_y_align = fixed_floor(cur_y+span+slope);
        		if(fixed_mass(cur_y+span) != fixed_mass(cur_y+span+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_y+span),slope));
        			end_y_align -= unit;
        		}
				
				if(slope<0)
				{
				   	for(step=start_y_align; step>end_y_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}
				else
				{
				   	for(step=start_y_align; step<end_y_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color);
        			}
				}

        	}
    	}
    	else
    	{
            if(y1>y2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dx,dy);
            span  = fixed_idiv(width*len,dy)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_y   = fixed_cvt_i2x(y2);
        	start_y = fixed_cvt_i2x(y1);
       		start_x = fixed_cvt_i2x(x1) - span/2 + FIXED_HALF;
            end_x   = fixed_cvt_i2x(x2) + span/2 + FIXED_HALF;

            start_y_align = fixed_floor(start_y);
            end_y_align   = fixed_floor(end_y);
            cur_x         = start_x + fixed_mul(start_y_align-start_y,slope);

        	for(cur_y=start_y_align; cur_y<end_y_align; cur_y+=FIXED_ONE, cur_x+=slope)
        	{
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_x),slope));
        		start_x_align = fixed_floor(cur_x) + unit;
        		if(fixed_mass(cur_x) != fixed_mass(cur_x+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+slope), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(fixed_frac(cur_x+slope),slope));
        			start_x_align += unit;
        		}
        		blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span+slope), fixed_cvt_x2i(cur_y), color, coverage(fixed_frac(cur_x+span+slope),slope));
        		end_x_align = fixed_floor(cur_x+span+slope);
        		if(fixed_mass(cur_x+span) != fixed_mass(cur_x+span+slope))
        		{
        			blend_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(cur_x+span), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_x+span),slope));
        			end_x_align -= unit;
        		}
				if(slope < 0)
				{
        			for(step=start_x_align; step>end_x_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
				else
				{
        			for(step=start_x_align; step<end_x_align; step+=unit)
        			{
        				fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color);
        			}
				}
        	}
    	}

        if (NULL != update_rect_ptr)
        {
            update_rect_ptr->left   = (int16)fixed_cvt_x2i(MIN(start_x,end_x))-width;
            update_rect_ptr->top    = (int16)fixed_cvt_x2i(MIN(start_y,end_y))-width;
            update_rect_ptr->right  = (int16)fixed_cvt_x2i(MAX(start_x,end_x))+width;
            update_rect_ptr->bottom = (int16)fixed_cvt_x2i(MAX(start_y,end_y))+width;
        }
    }    
}
#ifndef GUI_GRAPH_MINI_SUPPORT
/*****************************************************************************/
//   Description : Draw dash line on ARGB565/ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDashLine(int16           	x1,             //x coordinate of first point
		                  int16           	y1,             //y coordinage of first point
		                  int16           	x2,             //x coordinate of second point
		                  int16           	y2,             //y coordinate of sconcd point
		                  uint32           	color,           //color to draw.
		                  uint16           	width,           //unit: pixel
		                  void           	*mem_ptr,
		                  uint16           	mem_width,
		                  uint16           	mem_height,
		                  uint32           	mem_type,         //0: rgb565, 1: argb888
		                  GUI_RECT_T       	*update_rect_ptr)
{
    uint16  dash_len    = (8 + width * 2);
    uint16  slice_len   = dash_len + 4 + width * 2;
    fixed   f_dash_dx   = 0;
    fixed   f_dash_dy   = 0;
    fixed   f_slice_dx  = 0;
    fixed   f_slice_dy  = 0;
    int16  total_dx = 0;
    int16  total_dy = 0;
    uint16  len = 0;

    if (NULL == mem_ptr)
    {
        return;
    }

    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    len = isqrt(total_dx * total_dx + total_dy * total_dy);

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (total_dy <= total_dx)
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (x2 < x1)
        {
            int16 temp = x1;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dx, len);
        f_sin = fixed_idiv(total_dy, len);

        f_dash_dx = fixed_scale(dash_len, f_cos);        
        f_dash_dy = fixed_scale(dash_len, f_sin);   
        f_slice_dx = fixed_scale(slice_len, f_cos);
        f_slice_dy = fixed_scale(slice_len, f_sin);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (x1 < x2) 
        {
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (xn > x2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, (uint32 *)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, (uint16 *)mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }
    else
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (y2 < y1)
        {
            int16 temp;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dy, len);
        f_sin = fixed_idiv(total_dx, len);

        f_dash_dy = fixed_scale(dash_len, f_cos);
        f_dash_dx = fixed_scale(dash_len, f_sin);   
        f_slice_dy = fixed_scale(slice_len, f_cos);
        f_slice_dx = fixed_scale(slice_len, f_sin);


        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (y1 < y2) 
        {
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (yn > y2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, (uint32 *)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, (uint16 *)mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }
}


/*****************************************************************************/
//   Description : Draw dot line on ARGB565/ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDotLine(int16           		x1,             //x coordinate of first point
		                  int16           	y1,             //y coordinage of first point
		                  int16           	x2,             //x coordinate of second point
		                  int16           	y2,             //y coordinate of sconcd point
		                  uint32           	color,           //color to draw.
		                  uint16           	width,           //unit: pixel
		                  void           	*mem_ptr,
		                  uint16           	mem_width,
		                  uint16           	mem_height,
		                  uint32           	mem_type,         //0: rgb565, 1: argb888
		                  GUI_RECT_T       	*update_rect_ptr)
{
    uint16  dot_width   = width;
    uint16  slice_len   = dot_width * 2;
    fixed   f_slice_dx  = 0;
    fixed   f_slice_dy  = 0;
    int16  total_dx = 0;
    int16  total_dy = 0;
    uint16  len = 0;
    
    if (NULL == mem_ptr)
    {
        return;
    }

    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    len = isqrt(total_dx * total_dx + total_dy * total_dy);

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (total_dy <= total_dx)
    {
        fixed  f_sin, f_cos;        
        fixed  f_x1, f_y1;
        
        if (x2 < x1)
        {
            int16 temp = x1;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dx, len);
        f_sin = fixed_idiv(total_dy, len);

        f_slice_dx = fixed_scale(slice_len, f_cos);
        f_slice_dy = fixed_scale(slice_len, f_sin);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (x1 < x2) 
        {
            if (1 == mem_type)
            {
                //draw dot
                DrawDotU32(x1, y1, width, color, (uint32 *)mem_ptr, mem_width, mem_height);  
            }
            else
            {
                //draw dot
                DrawDot(x1, y1, width, color, (uint16 *)mem_ptr, mem_width, mem_height);                
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }
    else
    {
        fixed  f_sin, f_cos;       
        fixed  f_x1, f_y1;
        
        if (y2 < y1)
        {
            int16 temp;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }
        
        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dy, len);
        f_sin = fixed_idiv(total_dx, len);
        
        f_slice_dx = fixed_scale(slice_len, f_sin);
        f_slice_dy = fixed_scale(slice_len, f_cos);        

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (y1 < y2) 
        {
            if (1 == mem_type)
            {
                //draw dot
                DrawDotU32(x1, y1, width, color, (uint32 *)mem_ptr, mem_width, mem_height);  
            }
            else
            {
                //draw dot
                DrawDot(x1, y1, width, color, (uint16 *)mem_ptr, mem_width, mem_height);     
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }    
}

/*****************************************************************************/
//   Description : Draw dash-dot line ARGB565/ARGB888 buffer. width can be 1-5
//   Global resource dependence : none
//   Author: 
//   Note:   
/*****************************************************************************/
LOCAL void DrawDashDotLine(int16           		x1,             //x coordinate of first point
 		                      int16           	y1,             //y coordinage of first point
 		                      int16           	x2,             //x coordinate of second point
 		                      int16           	y2,             //y coordinate of sconcd point
 		                      uint32           	color,           //color to draw.
 		                      uint16           	width,           //unit: pixel
 		                      void           	*mem_ptr,
 		                      uint16           	mem_width,
 		                      uint16           	mem_height,
 		                      uint32           	mem_type,         //0: rgb565, 1: argb888
 		                      GUI_RECT_T       	*update_rect_ptr                      
                      )
{
    uint16  dot_width   = width + 1;
    uint16  dash_len    = (4 + width * 2);
    uint16  blank_len   = 4;
    uint16  dot_len     = dash_len + blank_len + width + dot_width / 2;        
    uint16  slice_len   = dot_len + dot_width / 2 + 1 + blank_len + width;
    fixed   f_dot_dx    = 0;
    fixed   f_dot_dy    = 0;
    fixed   f_dash_dx   = 0;
    fixed   f_dash_dy   = 0;
    fixed   f_slice_dx  = 0;
    fixed   f_slice_dy  = 0;
    int16  total_dx = 0;
    int16  total_dy = 0;
    uint16  len = 0;

    if (NULL == mem_ptr)
    {
        return;
    }

    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    len = isqrt(total_dx * total_dx + total_dy * total_dy);

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (total_dy <= total_dx)
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (x2 < x1)
        {
            int16 temp = x1;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dx, len);
        f_sin = fixed_idiv(total_dy, len);

        f_dash_dx = fixed_scale(dash_len, f_cos);        
        f_dash_dy = fixed_scale(dash_len, f_sin);   
        f_dot_dx = fixed_scale(dot_len, f_cos);        
        f_dot_dy = fixed_scale(dot_len, f_sin);
        f_slice_dx = fixed_scale(slice_len, f_cos);
        f_slice_dy = fixed_scale(slice_len, f_sin);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (x1 < x2) 
        {
            //////////////////////////////////////////////////////
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (xn > x2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, (uint32 *)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, (uint16 *)mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            f_xn = f_x1 + f_dot_dx;
            f_yn = f_y1 + f_dot_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn); 

            if (xn > x2)
            {
                break;
            }
            
            if (1 == mem_type)
            {
                DrawDotU32(xn, yn, dot_width, color, (uint32 *)mem_ptr, mem_width, mem_height);
            }
            else
            {
                DrawDot(xn, yn, dot_width, color, (uint16 *)mem_ptr, mem_width, mem_height);
            }

            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1); 
        }
    }
    else
    {
        fixed  f_sin, f_cos;
        
        fixed  f_xn, f_yn;
        int16  xn, yn;
        fixed  f_x1, f_y1;
        
        if (y2 < y1)
        {
            int16 temp;

            SWAP(x1, x2, temp);
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        total_dx = x2 - x1;
        
        f_cos = fixed_idiv(total_dy, len);
        f_sin = fixed_idiv(total_dx, len);

        f_dash_dx = fixed_scale(dash_len, f_sin);
        f_dash_dy = fixed_scale(dash_len, f_cos);  
        f_dot_dx = fixed_scale(dot_len, f_sin);        
        f_dot_dy = fixed_scale(dot_len, f_cos);        
        f_slice_dx = fixed_scale(slice_len, f_sin);
        f_slice_dy = fixed_scale(slice_len, f_cos);

        f_x1 = fixed_cvt_i2x(x1);
        f_y1 = fixed_cvt_i2x(y1);
        
        while (y1 < y2) 
        {
            f_xn = f_x1 + f_dash_dx;
            f_yn = f_y1 + f_dash_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn);

            if (yn > y2)
            {
                xn = x2;
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32(x1, y1, xn, yn, color, width, (uint32 *)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA(x1, y1, xn, yn, color, width, (uint16 *)mem_ptr, mem_width,
                                mem_height, NULL);                
            }

            f_xn = f_x1 + f_dot_dx;
            f_yn = f_y1 + f_dot_dy;
            xn = fixed_cvt_x2i(f_xn);
            yn = fixed_cvt_x2i(f_yn); 

            if (yn > y2)
            {
                break;
            }
            
            if (1 == mem_type)
            {
                DrawDotU32(xn, yn, dot_width, color, (uint32 *)mem_ptr, mem_width, mem_height);
            }
            else
            {
                DrawDot(xn, yn, dot_width, color, (uint16 *)mem_ptr, mem_width, mem_height);
            }
            
            f_x1 = f_x1 + f_slice_dx;
            f_y1 = f_y1 + f_slice_dy;
            x1 = fixed_cvt_x2i(f_x1);
            y1 = fixed_cvt_x2i(f_y1);          
        }
    }
}
#endif
////////////////////////////////////////////////////////////////////////////
//draw the v/h solid line ()
LOCAL void DrawThickLineAA_NoCap(int16           x1,             //x coordinate of first point
                        int16           y1,             //y coordinage of first point
                        int16           x2,             //x coordinate of second point
                        int16           y2,             //y coordinate of sconcd point
                         uint32           color,           //color to draw.
                         uint16           width,           //unit: pixel
                         uint16           *mem_ptr,
                         uint16           mem_width,
                         uint16           mem_height,
                         GUI_RECT_T       *update_rect_ptr)
{
    GUI_RECT_T  rect = {0};
    int dx = x2 - x1;
    int dy = y2 - y1;
    if((x1==x2)&&(y1==y2))
    {
        return;
    }        

    if(0!=(dx*dy))
    {
        return;
    }

    if ( PNULL == mem_ptr) 
    {
        return;
    }    

    if(0!=dx)
    {
        if(x1>x2)
        {
            uint16 tmp;
            SWAP(x1,x2,tmp);	
        }

        {
            int32 xx;
            int32 yy;
            rect.left = x1;
            rect.top = y1-width/2;
            rect.right = x2;
            rect.bottom = y1+width/2;

            for(yy = rect.top; yy<=rect.bottom; ++yy)
            {
                for(xx = rect.left ; xx<=rect.right; ++xx)
                {
                    fill_color(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color);
                }		        
            }	    
        } 
    }
    else if(0!=dy)
    {
        if(y1>y2)
        {
            uint16 tmp;
            SWAP(y1,y2,tmp);	
        }

        {
            int32 xx;
            int32 yy;
            rect.left = x1-width/2;
            rect.top = y1;
            rect.right = x2+width/2;
            rect.bottom = y2;		

            for(xx = rect.left ; xx<=rect.right; ++xx)
            {	
                for(yy = rect.top; yy<=rect.bottom; ++yy)
                {
                    fill_color(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color);
                }		        
            }	    
        }         
    }

    if (NULL != update_rect_ptr)
    {
        update_rect_ptr->left = rect.left;
        update_rect_ptr->top = rect.top;
        update_rect_ptr->right = rect.right;
        update_rect_ptr->bottom = rect.bottom;
    }
      
}
////////////////////////////////////////////////////////////////////////////
//draw the v/h solid line (argb888)
LOCAL void DrawThickLineAAU32_NoCap(int16           x1,             //x coordinate of first point
                        int16           y1,             //y coordinage of first point
                        int16           x2,             //x coordinate of second point
                        int16           y2,             //y coordinate of sconcd point
                         uint32           color,           //color to draw.
                         uint16           width,           //unit: pixel
                         uint32           *mem_ptr,
                         uint16           mem_width,
                         uint16           mem_height,
                         GUI_RECT_T       *update_rect_ptr)
{
    GUI_RECT_T  rect = {0};
    int dx = x2 - x1;
    int dy = y2 - y1;
    if((x1==x2)&&(y1==y2))
    {
        return;
    }        

    if(0!=(dx*dy))
    {
        return;
    }

    if ( PNULL == mem_ptr) 
    {
        return;
    }    

    if(0!=dx)
    {
        if(x1>x2)
        {
            uint16 tmp;
            SWAP(x1,x2,tmp);	
        }

        {
            int32 xx;
            int32 yy;
            rect.left = x1;
            rect.top = y1-width/2;
            rect.right = x2;
            rect.bottom = y1+width/2;

            for(yy = rect.top; yy<=rect.bottom; ++yy)
            {
                for(xx = rect.left ; xx<=rect.right; ++xx)
                {
                    fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color);
                }		        
            }	    
        } 
    }
    else if(0!=dy)
    {
        if(y1>y2)
        {
            uint16 tmp;
            SWAP(y1,y2,tmp);	
        }

        {
            int32 xx;
            int32 yy;
            rect.left = x1-width/2;
            rect.top = y1;
            rect.right = x2+width/2;
            rect.bottom = y2;		

            for(xx = rect.left ; xx<=rect.right; ++xx)
            {	
                for(yy = rect.top; yy<=rect.bottom; ++yy)
                {
                    fill_color_U32(mem_ptr, mem_width, mem_width, mem_height, xx, yy, color);
                }		        
            }	    
        }         
    }

    if (NULL != update_rect_ptr)
    {
        update_rect_ptr->left = rect.left;
        update_rect_ptr->top = rect.top;
        update_rect_ptr->right = rect.right;
        update_rect_ptr->bottom = rect.bottom;
    }
    
}

////////////////////////////////////////////////////////////////////////////
//draw the v/h dash line
LOCAL void DrawDashLine_NoCap(int16           x1,             //x coordinate of first point
                  int16           y1,             //y coordinage of first point
                  int16           x2,             //x coordinate of second point
                  int16           y2,             //y coordinate of sconcd point
                  uint32           color,           //color to draw.
                  uint16           width,           //unit: pixel
                  uint16           *mem_ptr,
                  uint16           mem_width,
                  uint16           mem_height,
                  uint32           mem_type,         //0: rgb565, 1: argb888
                  GUI_RECT_T       *update_rect_ptr)
{
    uint16  dash_len    = (8 + width * 2);
    uint16  slice_len   = dash_len + 4 + width * 2;
    int16  total_dx = 0;
    int16  total_dy = 0;
    
    if((x1==x2)&&(y1==y2))
    {
        return;
    } 
    
    if (NULL == mem_ptr)
    {
        return;
    }
    
    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    if(0!= (total_dx*total_dy))
    {
        return;
    }  
   
    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (0 !=total_dx)
    { 
        int16 xn;
        
        if (x2 < x1)
        {
            int16 temp = x1;
            SWAP(x1, x2, temp);
        }   

        total_dx = x2 - x1; 
        xn = x1;
          
        while (x1 < x2) 
        {
            xn = x1+ dash_len;   

            if (xn>x2)
            {
                int16 temp;
                SWAP(xn, x2, temp);
            }
              
            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32_NoCap(x1, y1, xn, y1, color, width, (uint32*)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA_NoCap(x1, y1, xn, y1, color, width, (uint16*)mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            x1= x1+ slice_len;        
        }
    }
    else if(0!=total_dy)
    {
        uint16 yn;
        
        if (y2 < y1)
        {
            int16 temp;
            SWAP(y1, y2, temp);
        }

        total_dy = y2 - y1;
        
        while (y1 < y2) 
        {
            yn= y1+dash_len;

            if (yn>y2)
            {
                int16 temp;
                SWAP(yn, y2, temp);
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32_NoCap(x1, y1, x1, yn, color, width, (uint32*)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA_NoCap(x1, y1, x1, yn, color, width, (uint16*)mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            y1= y1+ slice_len;        
        }
    }
}


////////////////////////////////////////////////////////////////////////////
//draw the v/h dot line
LOCAL void DrawDotLine_NoCap(int16           x1,       //x coordinate of first point
                  int16           y1,             //y coordinage of first point
                  int16           x2,             //x coordinate of second point
                  int16           y2,             //y coordinate of sconcd point
                  uint32           color,         //color to draw.
                  uint16           width,         //unit: pixel
                  uint16           *mem_ptr,
                  uint16           mem_width,
                  uint16           mem_height,
                  uint32           mem_type,         //0: rgb565, 1: argb888
                  GUI_RECT_T       *update_rect_ptr)
{
    uint16  dot_width   = width;
    uint16  slice_len   = dot_width * 2;
    int16  total_dx = 0;
    int16  total_dy = 0;
    
    if((x1==x2)&&(y1==y2))
    {
        return;
    } 
    
    if (NULL == mem_ptr)
    {
        return;
    }
    
    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    if(0!=(total_dx*total_dy))
    {
        return;
    }     

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (0!=total_dx)
    {     
        if (x2 < x1)
        {
            int16 temp;
            SWAP(x1, x2, temp);
        }
        x1 = x1+width/2;
        while (x1 < x2) 
        {
            if (1 == mem_type)
            {
                //draw dot
                DrawDotU32(x1, y1, dot_width, color, (uint32*)mem_ptr, mem_width, mem_height);  
            }
            else
            {
                //draw dot
                DrawDot(x1, y1, dot_width, color, (uint16*)mem_ptr, mem_width, mem_height);                
            }  

            x1 = x1+slice_len;       
        }
    }
    else if(0!=total_dy)
    {        
        if (y2 < y1)
        {
            int16 temp;
            SWAP(y1, y2, temp);
        }      

        y1 = y1+width/2;
        while (y1 < y2) 
        {
            if (1 == mem_type)
            {
                //draw dot
                DrawDotU32(x1, y1, width, color, (uint32*)mem_ptr, mem_width, mem_height);  
            }
            else
            {
                //draw dot
                DrawDot(x1, y1, width, color, (uint16*)mem_ptr, mem_width, mem_height);     
            }           

            y1 = y1+slice_len; 
            
        };
    }    
}


////////////////////////////////////////////////////////////////////////////////////
//only draw v/h dash dot line
void DrawDashDotLine_NoCap(int16           x1,             //x coordinate of first point
                      int16           y1,             //y coordinage of first point
                      int16           x2,             //x coordinate of second point
                      int16           y2,             //y coordinate of sconcd point
                      uint32           color,           //color to draw.
                      uint16           width,           //unit: pixel
                      uint16           *mem_ptr,
                      uint16           mem_width,
                      uint16           mem_height,
                      uint32           mem_type,         //0: rgb565, 1: argb888
                      GUI_RECT_T       *update_rect_ptr                      
                      )
{
    uint16  dot_width   = width + 1;
    uint16  dash_len    = (4 + width * 2);
    uint16  blank_len   = 4;
    uint16  dot_len     = dash_len + blank_len + width + dot_width / 2;        
    uint16  slice_len   = dot_len + dot_width / 2 + 1 + blank_len + width;
    int16  total_dx = 0;
    int16  total_dy = 0;

    if (NULL == mem_ptr)
    {
        return;
    }

    if((x1==x2)&&(y1==y2))
    {
        return;
    } 

    total_dx = ABS(x2 - x1);
    total_dy = ABS(y2 - y1);
    if(0!=(total_dx*total_dy))
    {
        return;
    }   

    GetLineRect(x1,y1, x2, y2, mem_width, mem_height, width, update_rect_ptr);

    if (0 !=total_dx)
    {
        int16 xn;
        
        if (x2 < x1)
        {
            int16 temp = x1;
            SWAP(x1, x2, temp);  
        }
        total_dx = x2 - x1;             
        while (x1 < x2) 
        {
            //////////////////////////////////////////////////////
            xn = x1 + dash_len;          
            
            if (xn > x2)
            {
                xn = x2;         
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32_NoCap(x1, y1, xn, y1, color, width, (uint32*)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA_NoCap(x1, y1, xn, y1, color, width, (uint16*)mem_ptr, mem_width,
                                mem_height, NULL);                
            }
            
            xn = x1+ dot_len;          

            if (xn > x2)
            {
                break;
            }
            
            if (1 == mem_type)
            {
                DrawDotU32(xn, y1, dot_width, color, (uint32*)mem_ptr, mem_width, mem_height);
            }
            else
            {
                DrawDot(xn, y1, dot_width, color, (uint16*)mem_ptr, mem_width, mem_height);
            }

            x1= x1+ slice_len;
        }
    }
    else if(0!=total_dy)
    {
        int16 yn;
        
        if (y2 < y1)
        {
            int16 temp;
            SWAP(y1, y2, temp);
        }
        total_dy = y2 - y1;              
        while (y1 < y2) 
        {      
            yn = y1 + dash_len;
            
            if (yn > y2)
            {
                yn = y2;
            }

            if (1 == mem_type)
            {
                //draw solid line
                DrawThickLineAAU32_NoCap(x1, y1, x1, yn, color, width, (uint32*)mem_ptr, mem_width,
                                    mem_height, NULL);    
            }
            else
            {
                //draw solid line
                DrawThickLineAA_NoCap(x1, y1, x1, yn, color, width, (uint16*)mem_ptr, mem_width,
                                mem_height, NULL);                
            }
       
            yn = y1 + dot_len;
            if (yn > y2)
            {
                break;
            }
            
            if (1 == mem_type)
            {
                DrawDotU32(x1, yn, dot_width, color, (uint32*)mem_ptr, mem_width, mem_height);
            }
            else
            {
                DrawDot(x1, yn, dot_width, color, (uint16*)mem_ptr, mem_width, mem_height);
            }

            y1 = y1+slice_len;         
        };
    }
}


//#if defined(MMI_PDA_SUPPORT)
__inline void blend_color32(
                        uint32/*GUI_COLOR_T*/ *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        uint16 x,
                        uint16 y,
                        uint32/*GUI_COLOR_T*/ color,
                        fixed alpha,
                        GUI_RECT_T  rect
                        )
{
    GUI_COLOR_T lcd_color;
    uint16 alpha_value = alpha >> 8;
    if(0 == alpha_value) return;
    RD_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, lcd_color);
    color = 
        R_COLOR((R_COLOR(color) * alpha_value + R_COLOR(lcd_color) * (256 - alpha_value)) >> 8) |
        G_COLOR((G_COLOR(color) * alpha_value + G_COLOR(lcd_color) * (256 - alpha_value)) >> 8) |
        B_COLOR((B_COLOR(color) * alpha_value + B_COLOR(lcd_color) * (256 - alpha_value)) >> 8);

    color =RGB5652ARGB8888(color, 0xff); /*lint !e648*/
    //WR_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, color);
    DRAW_POINT(buffer_ptr, buf_width,  rect, x, y, color);
}

__inline void fill_color32(
                        uint32/*GUI_COLOR_T*/ *buffer_ptr,
                        uint32 buf_width,
                        uint32 lcd_width,
                        uint32 lcd_height,
                        uint16 x,
                        uint16 y,
                        uint32/*GUI_COLOR_T*/ color,
                        GUI_RECT_T  rect
                        )
{
    color =RGB5652ARGB8888(color, 0xff); /*lint !e648*/
    //WR_COLOR_IN_LCD(buffer_ptr, buf_width, x, y, lcd_width, lcd_height, color);
    DRAW_POINT(buffer_ptr, buf_width, rect, x, y, color);
}

#ifndef GUI_GRAPH_MINI_SUPPORT
LOCAL void DrawThickLayerLineAA( 
                           GUI_LCD_DEV_INFO *dev_info_ptr,         //ID of LCD.
                           int16           x1,             //x coordinate of first point
                           int16           y1,             //y coordinage of first point
                           int16           x2,             //x coordinate of second point
                           int16           y2,             //y coordinate of sconcd point
                           uint32      color,     //color to draw.
                           uint16           width //unit: pixel
                           )
{
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    uint32 	     mem_width = 0;
    GUI_RECT_T  rect = {0};

    uint32 *block_buffer_ptr = PNULL;
    GUI_COLOR_T       *block_buffer_u16_ptr = PNULL;

    if((x1==x2)&&(y1==y2))
    {
        return;
    }
    
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(dev_info_ptr))
    {
        GetLcdInfoByLcdDev((const GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, &block_buffer_u16_ptr);

        block_buffer_ptr = (uint32 *)block_buffer_u16_ptr;

        rect.left = x1;
        rect.top = y1;
        rect.right = x2;
        rect.bottom = y2;
        rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, rect);
        x1  =  rect.left;
        y1  =  rect.top;
        x2  =  rect.right;
        y2  =   rect.bottom;

        {
            GUI_RECT_T buf_rect  = {0};
            GUI_RECT_T line_rect = {0};
            
            buf_rect.right  = mem_width -1;
            buf_rect.bottom = lcd_height -1;
            
            line_rect.left   = (int16)(MIN(x1,x2));
            line_rect.top    = (int16)(MIN(y1,y2));
            line_rect.right  = (int16)(MAX(x1,x2));
            line_rect.bottom = (int16)(MAX(y1,y2));
            
            if ( width > 1 )
            {
                line_rect.left   -= width/2;
                line_rect.top    -= width/2;
                line_rect.right  += width/2;
                line_rect.bottom += width/2;
            }
            
            if ( !GUI_IntersectRect( &rect, line_rect, buf_rect ) )
                return;
            }
    }
    else
#endif
    {
        return;
    }

    {
        int dx = x2 - x1;
        int dy = y2 - y1;

        int len = isqrt(dx*dx+dy*dy);

		fixed slope, span, unit, step;
        fixed start_x, start_y, end_y, end_x, cur_x, cur_y;
        fixed start_x_align, start_y_align, end_x_align, end_y_align;

    	if(ABS(dx)>ABS(dy))
    	{
            if(x1>x2)
            {
                uint16 tmp;
                SWAP(x1,x2,tmp);
                SWAP(y1,y2,tmp);

                dx=-dx;
                dy=-dy;
            }

            slope = fixed_idiv(dy,dx);
            span  = fixed_idiv(width*len,dx)+FIXED_HALF;
            unit  = FIXED_ONE;
            if(slope<0)
            {
                span = -span;
                unit = -unit;
            }

            end_x   = fixed_cvt_i2x(x2);
            start_x = fixed_cvt_i2x(x1);
            start_y = fixed_cvt_i2x(y1) - span/2 + FIXED_HALF;
            end_y   = fixed_cvt_i2x(y2) + span/2 + FIXED_HALF;

            start_x_align = fixed_floor(start_x);
            end_x_align   = fixed_floor(end_x);
            cur_y         = start_y + fixed_mul(start_x_align-start_x,slope);

        	for(cur_x=start_x_align; cur_x<end_x_align; cur_x+=FIXED_ONE, cur_y+=slope)
        	{
        		blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_y),slope), rect);
        		start_y_align = fixed_floor(cur_y) + unit;
        		if(fixed_mass(cur_y) != fixed_mass(cur_y+slope))
        		{
        			blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+slope), color, FIXED_ONE - coverage(fixed_frac(cur_y+slope),slope), rect);
        			start_y_align += unit;
        		}
        		blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span+slope), color, coverage(fixed_frac(cur_y+span+slope),slope), rect);
        		end_y_align = fixed_floor(cur_y+span+slope);
        		if(fixed_mass(cur_y+span) != fixed_mass(cur_y+span+slope))
        		{
        			blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y+span), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_y+span),slope), rect);
        			end_y_align -= unit;
        		}
				
				if(slope<0)
				{
				   	for(step=start_y_align; step>end_y_align; step+=unit)
        			{
        				fill_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color, rect);
        			}
				}
				else
				{
				   	for(step=start_y_align; step<end_y_align; step+=unit)
        			{
        				fill_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(step), color, rect);
        			}
				}

        	}
    	}
    	else
    	{
            if(y1>y2)
    		{
    			uint16 tmp;
    			SWAP(x1,x2,tmp);
    			SWAP(y1,y2,tmp);

    			dx=-dx;
    			dy=-dy;
    		}

            slope = fixed_idiv(dx,dy);
            span  = fixed_idiv(width*len,dy)+FIXED_HALF;
    		unit  = FIXED_ONE;
    		if(slope<0)
    		{
    			span = -span;
    			unit = -unit;
    		}

        	end_y   = fixed_cvt_i2x(y2);
        	start_y = fixed_cvt_i2x(y1);
       		start_x = fixed_cvt_i2x(x1) - span/2 + FIXED_HALF;
            end_x   = fixed_cvt_i2x(x2) + span/2 + FIXED_HALF;

            start_y_align = fixed_floor(start_y);
            end_y_align   = fixed_floor(end_y);
            cur_x         = start_x + fixed_mul(start_y_align-start_y,slope);

        	for(cur_y=start_y_align; cur_y<end_y_align; cur_y+=FIXED_ONE, cur_x+=slope)
        	{
        		blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x), fixed_cvt_x2i(cur_y), color, coverage(FIXED_ONE-fixed_frac(cur_x),slope), rect);
        		start_x_align = fixed_floor(cur_x) + unit;
        		if(fixed_mass(cur_x) != fixed_mass(cur_x+slope))
        		{
        			blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x+slope), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(fixed_frac(cur_x+slope),slope), rect);
        			start_x_align += unit;
        		}
        		blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x+span+slope), fixed_cvt_x2i(cur_y), color, coverage(fixed_frac(cur_x+span+slope),slope), rect);
        		end_x_align = fixed_floor(cur_x+span+slope);
        		if(fixed_mass(cur_x+span) != fixed_mass(cur_x+span+slope))
        		{
        			blend_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(cur_x+span), fixed_cvt_x2i(cur_y), color, FIXED_ONE - coverage(FIXED_ONE - fixed_frac(cur_x+span),slope), rect);
        			end_x_align -= unit;
        		}
				if(slope < 0)
				{
        			for(step=start_x_align; step>end_x_align; step+=unit)
        			{
        				fill_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color, rect);
        			}
				}
				else
				{
        			for(step=start_x_align; step<end_x_align; step+=unit)
        			{
        				fill_color32(block_buffer_ptr, lcd_width, lcd_width, lcd_height, fixed_cvt_x2i(step), fixed_cvt_x2i(cur_y), color, rect);
        			}
				}
        	}
    	}
        rect.left   = (int16)fixed_cvt_x2i(MIN(start_x,end_x))-width;
        rect.top    = (int16)fixed_cvt_x2i(MIN(start_y,end_y))-width;
        rect.right  = (int16)fixed_cvt_x2i(MAX(start_x,end_x))+width;
        rect.bottom = (int16)fixed_cvt_x2i(MAX(start_y,end_y))+width;
    }
    MMITHEME_StoreUpdateRect( dev_info_ptr,rect);

}
#ifdef MMI_3DUI_SUPPORT
/*****************************************************************************/
//   Description : Draw a line between 2 specific point to LCD buffer 
//                      whose width is WIDTH. 
//   Global resource dependence : none
//   Author:
//   Note:
/*****************************************************************************/
PUBLIC void LCD_DrawLayerLine( 
                              GUI_LCD_DEV_INFO  *dev_info_ptr,         //ID of LCD.
                              int16 x1,             //x coordinate of first point
                              int16 y1,             //y coordinage of first point
                              int16 x2,             //x coordinate of second point
                              int16 y2,             //y coordinate of sconcd point
                              uint32    color,     //color to draw.
                              uint16            width //unit: pixel
                              )
{
    if (width <1 || width > MAXLINEWIDTH || PNULL == dev_info_ptr)
    {
        return;
    }

    DrawThickLayerLineAA(dev_info_ptr, x1, y1, x2, y2, color, width);
}
#endif
#endif

#ifdef UI_MULTILAYER_SUPPORT
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
                              )
{
    BOOLEAN result = FALSE;
    FILL_POLYGON_EX_IN_T input_param = {0};
    FILL_POLYGON_EX_OUT_T out_param = {0};
    GUI_POINT_T points[4] = {0};
    UILAYER_INFO_T layer_info = {0};
    GUI_RECT_T rect = {0};

    rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, fill_rect);

    points[0].x = rect.left;
    points[0].y = rect.top;

    points[1].x = rect.right;
    points[1].y = rect.top;

    points[2].x = rect.right;
    points[2].y = rect.bottom;

    points[3].x = rect.left;
    points[3].y = rect.bottom;
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_GetLayerInfo(dev_info_ptr, &layer_info);
#endif
    if (DATA_TYPE_ARGB888 == layer_info.data_type)
    {
        input_param.data_format = IMGREF_FORMAT_ARGB888;
#ifdef UI_MULTILAYER_SUPPORT
        input_param.img_chn.chn0.ptr = UILAYER_GetLayerBufferPtr(dev_info_ptr);
#endif
        input_param.img_chn.chn0.size = layer_info.layer_width*layer_info.layer_height*4;
        input_param.img_size.w = layer_info.layer_width;
        input_param.img_size.h = layer_info.layer_height;
        input_param.points_ptr = &points[0];
        input_param.pt_num = 4;
        input_param.fill_param.grad_para.st_clr = start_color;
        input_param.fill_param.grad_para.ed_clr = end_color;
        input_param.fill_param.grad_para.grad_mode |= GRAPH_GRAD_DIREC_H;
        input_param.fill_param.grad_para.grad_mode |= GRAPH_GRAD_SPD_ACL;
        input_param.fill_mode = GRAPH_FILL_MODE_GRAD;
        
        input_param.clip_rect.x = 0;
        input_param.clip_rect.y = 0;
        input_param.clip_rect.w = layer_info.layer_width;
        input_param.clip_rect.h = layer_info.layer_height;
        
        if (SCI_SUCCESS == GRAPH_FillPolygonEx(&input_param, &out_param))
        {
            result = TRUE;
        }
    }

    return result;
}
#endif

#ifdef MMI_3DUI_SUPPORT
#ifndef GUI_GRAPH_MINI_SUPPORT
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
                              )
{
    uint32      mem_width = 0;
    uint32      lcd_width = 0;
    uint32      lcd_height = 0;
    FIX16_POINT_T  s_point ={0};
    FIX16_POINT_T  e_point ={0};
    uint32      color32 =RGB5652ARGB8888(alpha_line_info.color, alpha_line_info.alpha);
    IMGREF_SIZE_T layer_size ={0};
    GUI_RECT_T  rect = {0};

    uint32 *block_buffer_ptr = PNULL;

    if (PNULL == dev_info_ptr)
    {
        return;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(dev_info_ptr))
    {
        GetLcdInfoByLcdDev((GUI_LCD_DEV_INFO *)dev_info_ptr, &mem_width, &lcd_width, &lcd_height, (GUI_COLOR_T**)&block_buffer_ptr);

        rect.left = start_point.x;
        rect.top = start_point.y;
        rect.right = end_point.x;
        rect.bottom = end_point.y;
        rect = ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, dev_info_ptr, rect);

	    s_point.x =rect.left;
	    s_point.y =rect.top;
	    e_point.x =rect.right;
	    e_point.y =rect.bottom;

        {
            GUI_RECT_T buf_rect  = {0};
            GUI_RECT_T line_rect = {0};
            
            buf_rect.right  = mem_width ;
            buf_rect.bottom = lcd_height ;
            
            line_rect.left   = (int16)(MIN(rect.left,rect.right));
            line_rect.top    = (int16)(MIN(rect.top,rect.bottom));
            line_rect.right  = (int16)(MAX(rect.left,rect.right));
            line_rect.bottom = (int16)(MAX(rect.top,rect.bottom));
            
            if ( alpha_line_info.width> 1 )
            {
                line_rect.left   -= alpha_line_info.width/2;
                line_rect.top    -= alpha_line_info.width/2;
                line_rect.right  += alpha_line_info.width/2;
                line_rect.bottom += alpha_line_info.width/2;
            }
            
            if ( !GUI_IntersectRect( &rect, line_rect, buf_rect ) )
                return;
       }

        layer_size.h =lcd_height;
        layer_size.w =lcd_width;
        S2D_DrawLine(s_point, e_point, alpha_line_info.width,alpha_line_info.is_circle_end,layer_size, color32,block_buffer_ptr);

        rect.left   = (MIN(rect.left,rect.right))-alpha_line_info.width;
        rect.top    = (MIN(rect.top,rect.bottom))-alpha_line_info.width;
        rect.right  = (MAX(rect.left,rect.right))+alpha_line_info.width;
        rect.bottom = (MAX(rect.top,rect.bottom))+alpha_line_info.width;
		
        MMITHEME_StoreUpdateRect( dev_info_ptr,rect);
    }
#endif
}
#endif
#endif
//#endif

