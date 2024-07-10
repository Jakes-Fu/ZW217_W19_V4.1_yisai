/*! @file guigraph.h
@brief ���ļ��ṩ�˻���ͼ�εĻ���
@author Jassmine
@version 1.0
@date 04/26/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Graph Graph

(����)

*******************************************************************************/

/*! @addtogroup graphGrp Graph
@brief ͼ�λ���ģ��
@details
��ģ���ṩ����ͼ�εĻ���
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
@brief ���Ʒ�ʽ
*/
typedef enum
{
    OVER_MODE,                      /*!< ���Ƿ�ʽ */
    XOR_MODE                        /*!< ���ʽ */
}LCD_COPY_MODE_E;   

/*! @enum GUILCD_PEN_STYLE_E
@brief �ߵķ��
*/
typedef enum
{
    GUILCD_PEN_STYLE_DEFAULT,       /*!< Ĭ��ֵ��ֱ�� */
    GUILCD_PEN_STYLE_DOT,           /*!< ���� */
    GUILCD_PEN_STYLE_DASH,          /*!< ���� */
    GUILCD_PEN_STYLE_DASH_DOT,      /*!< �㻮�� */
    NUM_OF_GUILCD_PEN_STYLE
}GUILCD_PEN_STYLE_E;


/*! @enum MMI_ARROW_TYPE_E
@brief ��ͷ����
*/
typedef enum
{
    ARROW_LEFT,                     /*!< ���ͷ */
    ARROW_UP,                       /*!< �ϼ�ͷ */
    ARROW_RIGHT,                    /*!< �Ҽ�ͷ */
    ARROW_DOWN,                     /*!< �¼�ͷ */
    ARROW_DOUBLE_LEFT,              /*!< �ݲ�֧�� */
    ARROW_DOUBLE_UP,                /*!< �ݲ�֧�� */
    ARROW_DOUBLE_RIGHT,             /*!< �ݲ�֧�� */
    ARROW_DOUBLE_DOWN               /*!< �ݲ�֧�� */
} MMI_ARROW_TYPE_E;        


/*! @enum GUILCD_CONVERT_POS_TYPE_E
@brief �����ת������
*/
typedef enum
{
    GUILCD_CONVERT_POS_LCD2DISP,    /*!< �����lcd����ת��Ϊ�����ͼ������ */
    GUILCD_CONVERT_POS_DISP2LCD,    /*!< �����ͼ������ת��Ϊ�����lcd���� */

    GUILCD_CONVERT_POS_MAX
} GUILCD_CONVERT_POS_TYPE_E;            


/*! @struct GUILCD_DRAW_DC_T
@brief ����LCD buffer����Ϣ
@details
mem_width��width������\n
mem_widthָ����buffer�Ŀ�ȣ�����һ���ж��ٸ����أ�width���û�ָ������ʾ��ȡ�
������ֵ���Բ���ȣ�������ֻ�ܻ�����buffer������֮�ڣ����ᱻ�ü���buffer�ķ�Χ֮�ڡ�\n
mem_width�����ڻ���ʱ��ת����һ�����ӵ�ƫ������\n
���ֵ����������Ϊ��λ��һ�������ж��ٸ��ֽڣ��������ݸ�ʽdata_type��ʾ��
*/
typedef struct 
{
    uint32 mem_width;               /*!< buffer�Ŀ�� */
    uint32 width;                   /*!< ��Ҫ��ʾ�Ŀ�� */
    uint32 height;                  /*!< buffer�߶� */
    void *lcd_buffer_ptr;           /*!< buffer��ַ */
    DISPLAY_DATA_TYPE_E data_type;  /*!< ���ݸ�ʽ */
}GUILCD_DRAW_DC_T;

/*! @struct GUILCD_DRAW_ALPHA_LINE_T
@brief ����alphaֵ����
@details
*/
typedef struct 
{
    uint16         color;         //color to draw.
    uint16         width;         //�߿�
    uint8          alpha;         //��ɫalpha
    BOOLEAN        is_circle_end; //Բ����ͷ����β
}GUILCD_DRAW_ALPHA_LINE_T;


/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief ����ֱ��
@author Jim.zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param x1           [in] ����x����
@param y1           [in] ����y����
@param x2           [in] �յ��x����
@param y2           [in] �յ��y����
@param color        [in] ��ɫֵ
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
@brief �����л��Ʒ�ʽ��ֱ��
@author Jim.zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param x1           [in] ����x����
@param y1           [in] ����y����
@param x2           [in] �յ��x����
@param y2           [in] �յ��y����
@param color        [in] ��ɫֵ
@param copy_mode    [in] ���Ʒ�ʽ
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
@brief ����ˮƽֱ��
@author Jim.zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param x1           [in] ����x����
@param y            [in] y����
@param x2           [in] �յ��x����
@param color        [in] ��ɫֵ
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
@brief ���ƴ�ֱֱ��
@author Jim.zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param x            [in] x����
@param y1           [in] ����y����
@param y2           [in] �յ��y����
@param color        [in] ��ɫֵ
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
@brief ���ƴ��߿��ֱ��
@author Geng.Ren
@param dev_info_ptr [in] ͼ����Ϣ
@param x1           [in] ����x����
@param y1           [in] ����y����
@param x2           [in] �յ��x����
@param y2           [in] �յ��y����
@param color        [in] ��ɫֵ
@param width        [in] �߿�
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
@brief ���ƴ��߿�ķ����
@author James.Zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param x1           [in] ����x����
@param y1           [in] ����y����
@param x2           [in] �յ��x����
@param y2           [in] �յ��y����
@param color        [in] ��ɫֵ
@param width        [in] �߿�
@param style        [in] �ߵķ��
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
@brief ��ָ��ͼ����������
@author Jim.zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param box          [in] ��������
@param color        [in] ��ɫֵ
@return 
@note
*******************************************************************************/
PUBLIC void LCD_FillRect(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         GUI_COLOR_T                color           //color to fill
                         );

/***************************************************************************//*!
@brief RGB565ת��ΪARGB888
@author Jassmine
@param color [in] ��ɫֵ
@param alpha [in] ͸����
@return ARGB888��ɫֵ
@note 
*******************************************************************************/
PUBLIC uint32 ConvertRgb565ToArgb888(
                                     uint16     color,
                                     uint16     alpha
                                     );

/***************************************************************************//*!
@brief ��ָ��buffer��������
@author jibin
@param lcdbuff_ptr  [in] buffer��ַ
@param lcdbuff_size [in] buffer��С
@param lcd_width    [in] buffer���
@param lcd_height   [in] buffer�߶�
@param box          [in] ��������
@param color        [in] ��ɫֵ
@return 
@note
��������lcdbuff_size = lcd_width * lcd_height
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
@brief ���ƾ��α߿�
@author Jim.zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param box   [in] ��������
@param color [in] ��ɫֵ
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
@brief ��ָ�������ڻ���������
@author Geng.Ren
@param lcd_dev_info [in] ͼ����Ϣ
@param rect         [in] ��������
@param arrowtype    [in] ��ͷ����
@param color        [in] ��ɫֵ
@return 
@note
- ���¼�ͷ����ͷ����Ŀ��Ϊ7���߶�Ϊ4��\n
  ���Ҽ�ͷ����ͷ����Ŀ��Ϊ4���߶�Ϊ7
- ��������Ļ�������rect���ڼ�ͷ����Ҫ�Ĵ�С���������ʾ
*******************************************************************************/
PUBLIC void LCD_DrawArrowInRect( 
                                GUI_LCD_DEV_INFO        *lcd_dev_info,
                                GUI_RECT_T              rect,     //rectangle to draw
                                MMI_ARROW_TYPE_E        arrowtype,    //arrow type
                                GUI_COLOR_T             color     //color to draw
                                );

/***************************************************************************//*!
@brief  ��ָ��ͼ������仡������
@author James.Zhang
@param dev_info_ptr  [in] ͼ����Ϣ
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param start_angle   [in] ��ʼ�Ƕ�
@param end_angle     [in] �����Ƕ�
@param color         [in] ��ɫֵ
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillArc(
                        const GUI_LCD_DEV_INFO*   dev_info_ptr,
                        GUI_RECT_T*               clip_rect_ptr, //��������, ����Ϊ��
                        int16                     x,
                        int16                     y,
                        uint16                    radius,
                        uint16                    start_angle,   //��ʼ�Ƕ�
                        uint16                    end_angle,   //�����Ƕ�
                        GUI_COLOR_T               color
                        );

/***************************************************************************//*!
@brief ��ָ��buffer����仡������
@author James.Zhang
@param buffer_ptr    [in] buffer��ַ
@param buf_width     [in] buffer���
@param buf_height    [in] buffer�߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param start_angle   [in] ��ʼ�Ƕ�
@param end_angle     [in] �����Ƕ�
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillArcToBuffer(
                                      GUI_COLOR_T*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //��ʼ�Ƕ�
                                      uint16            end_angle,   //�����Ƕ�
                                      GUI_COLOR_T       color
                                      );


/***************************************************************************//*!
@brief ��ָ��ͼ���л�����
@author James.Zhang
@param dev_info_ptr  [in] ͼ����Ϣ
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param start_angle   [in] ��ʼ�Ƕ�
@param end_angle     [in] �����Ƕ�
@param color         [in] ��ɫֵ
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawArc(
                        const GUI_LCD_DEV_INFO*   dev_info_ptr,
                        GUI_RECT_T*               clip_rect_ptr, //��������, ����Ϊ��
                        int16                     x,
                        int16                     y,
                        uint16                    radius,
                        uint16                    start_angle,   //��ʼ�Ƕ�
                        uint16                    end_angle,   //�����Ƕ�
                        GUI_COLOR_T               color
                        );

/***************************************************************************//*!
@brief ��ָ��buffer�л�����
@author James.Zhang
@param buffer_ptr    [in] buffer��ַ
@param buf_width     [in] buffer���
@param buf_height    [in] buffer�߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param start_angle   [in] ��ʼ�Ƕ�
@param end_angle     [in] �����Ƕ�
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawArcToBuffer(
                                      GUI_COLOR_T*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //��ʼ�Ƕ�
                                      uint16            end_angle,   //�����Ƕ�
                                      GUI_COLOR_T       color
                                      );
#endif

/***************************************************************************//*!
@brief ��ָ��ͼ�������Բ
@author James.Zhang
@param dev_info_ptr  [in] ͼ����Ϣ
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param color         [in] ��ɫֵ
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillCircle(
                           const GUI_LCD_DEV_INFO* dev_info_ptr,
                           GUI_RECT_T*             clip_rect_ptr, //��������, ����Ϊ��
                           int16                   x,
                           int16                   y,
                           uint16                  radius,
                           GUI_COLOR_T             color
                           );

/***************************************************************************//*!
@brief ��ָ��buffer�����Բ
@author James.Zhang
@param buffer_ptr    [in] buffer��ַ
@param buf_width     [in] buffer���
@param buf_height    [in] buffer�߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillCircleToBuffer(
                                         GUI_COLOR_T*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         GUI_COLOR_T       color
                                         );
#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief ��ָ��ͼ���л�Բ�߿�
@author James.Zhang
@param dev_info_ptr  [in] ͼ����Ϣ
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param color         [in] ��ɫֵ
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawCircle(
                           const GUI_LCD_DEV_INFO* dev_info_ptr,
                           GUI_RECT_T*             clip_rect_ptr, //��������, ����Ϊ��
                           int16                   x,
                           int16                   y,
                           uint16                  radius,
                           GUI_COLOR_T             color
                           );


/***************************************************************************//*!
@brief ��ָ��buffer�л�Բ�߿�
@author James.Zhang
@param buffer_ptr    [in] buffer��ַ
@param buf_width     [in] buffer���
@param buf_height    [in] buffer�߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawCircleToBuffer(
                                         GUI_COLOR_T*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         GUI_COLOR_T       color
                                         );

/***************************************************************************//*!
@brief ��ָ��ͼ���������Բ
@author James.Zhang
@param dev_info_ptr  [in] ͼ����Ϣ
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] ��ԲԲ��x����
@param y             [in] ��ԲԲ��y����
@param radius_x      [in] ����뾶
@param radius_y      [in] ����뾶
@param color         [in] ��ɫֵ
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_FillEllipse(
                            const GUI_LCD_DEV_INFO*   dev_info_ptr,
                            GUI_RECT_T*               clip_rect_ptr, //��������, ����Ϊ��
                            int16                     x,
                            int16                     y,
                            uint16                    radius_x,
                            uint16                    radius_y,
                            GUI_COLOR_T               color
                            );

/***************************************************************************//*!
@brief ��ָ��buffer�������Բ
@author James.Zhang
@param buffer_ptr    [in] buffer��ַ
@param buf_width     [in] buffer���
@param buf_height    [in] buffer�߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] ��ԲԲ��x����
@param y             [in] ��ԲԲ��y����
@param radius_x      [in] ����뾶
@param radius_y      [in] ����뾶
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillEllipseToBuffer(
                                          GUI_COLOR_T*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          GUI_COLOR_T       color
                                          );

/***************************************************************************//*!
@brief ��ָ��ͼ���л���Բ�߿�
@author James.Zhang
@param dev_info_ptr  [in] ͼ����Ϣ
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] ��ԲԲ��x����
@param y             [in] ��ԲԲ��y����
@param radius_x      [in] ����뾶
@param radius_y      [in] ����뾶
@param color         [in] ��ɫֵ
@return 
@note 
*******************************************************************************/
PUBLIC void LCD_DrawEllipse(
                            const GUI_LCD_DEV_INFO*   dev_info_ptr,
                            GUI_RECT_T*               clip_rect_ptr, //��������, ����Ϊ��
                            int16                     x,
                            int16                     y,
                            uint16                    radius_x,
                            uint16                    radius_y,
                            GUI_COLOR_T               color
                            );


/***************************************************************************//*!
@brief ��ָ��buffer�л���Բ�߿�
@author James.Zhang
@param buffer_ptr    [in] buffer��ַ
@param buf_width     [in] buffer���
@param buf_height    [in] buffer�߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] ��ԲԲ��x����
@param y             [in] ��ԲԲ��y����
@param radius_x      [in] ����뾶
@param radius_y      [in] ����뾶
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawEllipseToBuffer(
                                          GUI_COLOR_T*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          GUI_COLOR_T       color
                                          );

/***************************************************************************//*!
//brief  draw a 3D style frame
//author Geng.Ren
//param dev_info_ptr [in] ͼ����Ϣ
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
@brief ��ָ��ͼ�����ɲ�
@author Geng.Ren
@param dev_info_ptr [in] ͼ����Ϣ
@param rect         [in] �������
@param color        [in] �ɲ���ɫ
@return 
@details 
������ɲ�������ɫֵcolor��50%͸���ȣ���Ŀ��ͼ���Ͻ��л��
*******************************************************************************/
PUBLIC void LCD_MistBmp(
                        GUI_LCD_DEV_INFO  *dev_info_ptr,
                        GUI_RECT_T        rect,
                        GUI_COLOR_T       color     //color to draw.
                        );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief ���ڴ��е����ݿ�����������(���ؼ�ɫ)
@author bin.ji
@param mem_buf_ptr     [in] �ڴ�buffer��ַ
@param mem_width       [in] ��Ҫ�����Ŀ��
@param mem_height      [in] ��Ҫ�����ĸ߶�
@param left            [in] ��Ҫ�����������λ��
@param top             [in] ��Ҫ�����������λ��
@param key_color       [in] �ؼ�ɫ�������ڴ��к��и���ɫ�ĵ�
@param is_final_lcdbuf [in] �Ƿ񿽱���LCD buffer��
@return 
@note 
ͨ��is_final_lcdbuf�����Կ���Ŀ��bufferѡ��LCD�����Ƕ��ϵͳ�е����㡣�����е�����
��û�кϲ������߼��㡣
*******************************************************************************/
PUBLIC void LCD_CopyMemToMainLcdBuf(
                                    GUI_COLOR_T *mem_buf_ptr,
                                    uint32 mem_width,
                                    uint32 mem_height,
                                    uint32 left,
                                    uint32 top,
                                    GUI_COLOR_T key_color,
                                    BOOLEAN is_final_lcdbuf    //������������պϲ����lcd_buf
                                    );
#endif

/***************************************************************************//*!
@brief ���ڴ��е����ݿ�����������
@author bin.ji
@param mem_buf_ptr     [in] �ڴ�buffer��ַ
@param mem_width       [in] ��Ҫ�����Ŀ��
@param mem_height      [in] ��Ҫ�����ĸ߶�
@param left            [in] ��Ҫ�����������λ��
@param top             [in] ��Ҫ�����������λ��
@param is_final_lcdbuf [in] �Ƿ񿽱���LCD buffer��
@return 
@note 
ͨ��is_final_lcdbuf�����Կ���Ŀ��bufferѡ��LCD�����Ƕ��ϵͳ�е����㡣�����е�����
��û�кϲ������߼��㡣
*******************************************************************************/
PUBLIC void LCD_CopyMemToMainLcdBufNoKeyColor(
                                              GUI_COLOR_T *mem_buf_ptr,
                                              uint32 mem_width,
                                              uint32 mem_height,
                                              uint32 left,
                                              uint32 top,
                                              BOOLEAN is_final_lcdbuf    //������������պϲ����lcd_buf
                                              );

/***************************************************************************//*!
@brief  ���������ݿ�����buffer��
@author bin.ji
@param mem_buf_ptr     [in] �ڴ�buffer��ַ
@param mem_width       [in] ��Ҫ�����Ŀ��
@param mem_height      [in] ��Ҫ�����ĸ߶�
@param left            [in] ��Ҫ�����������λ��
@param top             [in] ��Ҫ�����������λ��
@param is_final_lcdbuf [in] �Ƿ���LCD buffer������
@return 
@note 
ͨ��is_final_lcdbuf�����Կ���Ŀ��bufferѡ��LCD�����Ƕ��ϵͳ�е����㡣�����е�����
��û�кϲ������߼��㡣
*******************************************************************************/
PUBLIC void LCD_CopyMainLcdBufToMem(
                                    GUI_COLOR_T *mem_buf_ptr,
                                    uint32 mem_width,
                                    uint32 mem_height,
                                    uint32 left,
                                    uint32 top,
                                    BOOLEAN is_final_lcdbuf    //������������պϲ����lcd_buf
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
@brief �����������Ļ��ͼ���ת��
@author bin.ji
@param convert_type [in] ת������
@param dev_info_ptr [in] ͼ����Ϣ
@param in_rect_ptr  [in] ԭ����
@param out_rect_ptr [in] ת���������
@return ת������������in_rect_ptr��out_rect_ptrΪ�գ��򷵻�PNULL
@note
���ڲ��λ�ÿ��Ըı䣬����������������Ļ��ͼ���ǲ�һ�µģ�����ͨ���÷������໥ת����
*******************************************************************************/
PUBLIC GUI_RECT_T *LCD_ConvertDispRect(
                                       GUILCD_CONVERT_POS_TYPE_E convert_type,
                                       GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                       const GUI_RECT_T *in_rect_ptr,       //in
                                       GUI_RECT_T       *out_rect_ptr       //in
                                       );

/***************************************************************************//*!
@brief ������������Ļ��ͼ���ת��
@author bin.ji
@param convert_type  [in] ת������
@param dev_info_ptr  [in] ͼ����Ϣ
@param in_point_ptr  [in] ԭ�����
@param out_point_ptr [in] ת����������
@return ת���������㣬���in_point_ptr��out_point_ptrΪ�գ��򷵻�PNULL
@details
���ڲ��λ�ÿ��Ըı䣬����������������Ļ��ͼ���ǲ�һ�µģ�����ͨ���÷������໥ת����
*******************************************************************************/
PUBLIC GUI_POINT_T *LCD_ConvertDispPoint(
                                       GUILCD_CONVERT_POS_TYPE_E convert_type,
                                       GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                       const GUI_POINT_T *in_point_ptr,       //in
                                       GUI_POINT_T       *out_point_ptr       //in
                                       );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief ��ָ����buffer������͸����Ϣ�ľ���
@author James.Zhang
@param buf_ptr    [in] Ŀ��buffer�ĵ�ַ
@param buf_width  [in] bufferһ�еĿ��
@param lcd_width  [in] LCD�Ŀ��
@param lcd_height [in] LCD�ĸ߶�
@param box        [in] ��ʾ����
@param color      [in] ��ɫֵ
@param alpha      [in] ͸����
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
@brief  ����͸����Ϣ�ľ���
@author James.Zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param box          [in] ��������
@param color        [in] ��ɫֵ
@param alpha        [in] ͸��ֵ
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
@brief ��ȡLCD��Ϣ
@author  bin.ji
@param dev_info_ptr [in] ָ��LCD
@param draw_dc_ptr  [in/out] ָ��LCD��Ϣ�ĵ�ַ
@return �Ƿ���óɹ�
@note
�÷���һ������buffer�����ǰ���ã����ڻ�ȡbuffer����Ϣ��
*******************************************************************************/
PUBLIC BOOLEAN LCD_PrepareDrawParam(
                                 const GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                                 GUILCD_DRAW_DC_T       *draw_dc_ptr       //out
                                 );


#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief ��仡������
@author James.Zhang
@param buffer_ptr    [in] ָ��Ŀ��buffer
@param buf_width     [in] Ŀ��buffer�Ŀ��
@param buf_height    [in] Ŀ��buffer�ĸ߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param start_angle   [in] ��ʼ�Ƕ�
@param end_angle     [in] �����Ƕ�
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillArcToBufferU32(
                                      uint32*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //��ʼ�Ƕ�
                                      uint16            end_angle,   //�����Ƕ�
                                      uint32       color
                                      );

/***************************************************************************//*!
@brief ���ƻ���
@author James.Zhang
@param buffer_ptr    [in] ָ��Ŀ��buffer
@param buf_width     [in] Ŀ��buffer�Ŀ��
@param buf_height    [in] Ŀ��buffer�ĸ߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] Բ��x����
@param y             [in] Բ��y����
@param radius        [in] �뾶
@param start_angle   [in] ��ʼ�Ƕ�
@param end_angle     [in] �����Ƕ�
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawArcToBufferU32(
                                      uint32*      buffer_ptr,
                                      uint16            buf_width,
                                      uint16            buf_height,
                                      GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                      int16             x,
                                      int16             y,
                                      uint16            radius,
                                      uint16            start_angle,   //��ʼ�Ƕ�
                                      uint16            end_angle,   //�����Ƕ�
                                      uint32       color
                                      );
#endif

/***************************************************************************//*!
@brief ���Բ����
@author James.Zhang
@param buffer_ptr     [in] ָ��Ŀ��buffer
@param buf_width      [in] Ŀ��buffer�Ŀ��
@param buf_height     [in] Ŀ��buffer�ĸ߶�
@param clip_rect_ptr  [in] ������������Ϊ��
@param x              [in] Բ��x����
@param y              [in] Բ��y����
@param radius         [in] �뾶
@param color          [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillCircleToBufferU32(
                                         uint32*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         uint32       color
                                         );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief ������Բ�߿�
@author James.Zhang
@param buffer_ptr    [in] ָ��Ŀ��buffer
@param buf_width     [in] Ŀ��buffer�Ŀ��
@param buf_height    [in] Ŀ��buffer�ĸ߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] ��ԲԲ��x����
@param y             [in] ��ԲԲ��y����
@param radius_x      [in] ����뾶
@param radius_y      [in] ����뾶
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawEllipseToBufferU32(
                                          uint32*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          GUI_COLOR_T       color
                                          );

/***************************************************************************//*!
@brief ����Բ�߿�
@author James.Zhang
@param buffer_ptr     [in] ָ��Ŀ��buffer
@param buf_width      [in] Ŀ��buffer�Ŀ��
@param buf_height     [in] Ŀ��buffer�ĸ߶�
@param clip_rect_ptr  [in] ������������Ϊ��
@param x              [in] Բ��x����
@param y              [in] Բ��y����
@param radius         [in] �뾶
@param color          [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_DrawCircleToBufferU32(
                                         uint32*      buffer_ptr,
                                         uint16            buf_width,
                                         uint16            buf_height,
                                         GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                         int16             x,
                                         int16             y,
                                         uint16            radius,
                                         uint32       color
                                         );

/***************************************************************************//*!
@brief �����Բ����
@author James.Zhang
@param buffer_ptr    [in] ָ��Ŀ��buffer
@param buf_width     [in] Ŀ��buffer�Ŀ��
@param buf_height    [in] Ŀ��buffer�ĸ߶�
@param clip_rect_ptr [in] ������������Ϊ��
@param x             [in] ��ԲԲ��x����
@param y             [in] ��ԲԲ��y����
@param radius_x      [in] ����뾶
@param radius_y      [in] ����뾶
@param color         [in] ��ɫֵ
@return ʵ�ʻ��Ƶ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T LCD_FillEllipseToBufferU32(
                                          uint32*      buffer_ptr,
                                          uint16            buf_width,
                                          uint16            buf_height,
                                          GUI_RECT_T*       clip_rect_ptr, //��������, ����Ϊ��
                                          int16             x,
                                          int16             y,
                                          uint16            radius_x,
                                          uint16            radius_y,
                                          uint32       color
                                          );
#endif

/***************************************************************************//*!
@brief ����������
@author Jim.zhang
@param dev_info_ptr [in] ͼ����Ϣ
@param box          [in] ��������
@param u32_color        [in] ��ɫֵ
@return 
@note
Ŀ��LCD������DATA_TYPE_ARGB888��ʽ�����������Ч
*******************************************************************************/
PUBLIC void LCD_FillRectU32(
                         GUI_LCD_DEV_INFO const     *dev_info_ptr,  // block information
                         GUI_RECT_T                 box,            //rectangle to fill color
                         uint32                     u32_color       //color to fill
                         );

#ifndef GUI_GRAPH_MINI_SUPPORT
/***************************************************************************//*!
@brief ����͸����ɫ�ĵ���ֵ
@author James.Zhang
@param u32_color [in] ARGB888��ɫֵ
@param u16_color [in] RGB565��ɫֵ
@param alpha     [in] ���Ӹ�u16_color��͸����
@return ����֮���ARGB888��ɫֵ
@note
u16_color��alpha�γ�һ����͸����Ϣ����ɫ�����ӵ�u32_color
*******************************************************************************/
PUBLIC uint32 GUI_CalcAlphaAdd( 
                               uint32 u32_color,
                               uint16 u16_color,
                               uint8 alpha
                               );
#endif

/***************************************************************************//*!
@brief ����������ɫ���Ӻ��ֵ
@author hua.fang
@param down_color [in] �����ARGB888��ɫֵ
@param up_color [in] �����ARGB888��ɫֵ
@return ����֮���ARGB888��ɫֵ
@note
*******************************************************************************/
PUBLIC uint32 GUI_CompoundARGB888(
                                  uint32    down_color,
                                  uint32    up_color
                                  );

/***************************************************************************//*!
@brief ����Բ�Ǿ��α߿�
@author Jassmine
@param dev_info_ptr [in] ͼ����Ϣ
@param box          [in] ��������
@param display_rect [in] ������
@param color        [in] ��ɫֵ
@return 
@note
�߿�Ϊ2�����أ��һ�������Ŀ��>=10,�߶�>=12
*******************************************************************************/
PUBLIC void LCD_DrawRoundedRect(
                                GUI_LCD_DEV_INFO const  *dev_info_ptr,
                                GUI_RECT_T              box,
                                GUI_RECT_T              display_rect,
                                GUI_COLOR_T             color
                                );

/***************************************************************************//*!
@brief ����Բ�Ǿ��α߿�ָ��buffer
@author Jassmine
@param buf_ptr      [in] bufferָ��
@param buf_width    [in] buffer���
@param buf_height   [in] buffer�߶�
@param box          [in] ��������
@param color        [in] ��ɫֵ
@return 
@note
�߿�Ϊ2�����أ��һ�������Ŀ��>=10,�߶�>=12
*******************************************************************************/
PUBLIC void LCD_DrawRoundedRectToBuf(
                                     GUI_COLOR_T    *buf_ptr,
                                     uint16         buf_width,
                                     uint16         buf_height,
                                     GUI_RECT_T     box,
                                     GUI_COLOR_T    color
                                     );

/***************************************************************************//*!
@brief ���ƴ���Ӱ��Բ�Ǿ��α߿�
@author Lianxiang
@param dev_info_ptr [in] ͼ����Ϣ
@param box          [in] ��������
@param display_rect [in] ������
@param color        [in] ��ɫֵ
@return 
@note
�߿�Ϊ2�����أ��һ�������Ŀ��>=10,�߶�>=12
*******************************************************************************/
PUBLIC void LCD_DrawShadowRoundedRect(
                                      GUI_LCD_DEV_INFO const    *dev_info_ptr,
                                      GUI_RECT_T                box,
                                      GUI_RECT_T                display_rect,
                                      GUI_COLOR_T               color
                                      );

/***************************************************************************//*!
@brief ���ƴ���Ӱ��Բ�Ǿ��α߿�ָ��buffer
@author Jassmine
@param buf_ptr      [in] bufferָ��
@param buf_width    [in] buffer���
@param buf_height   [in] buffer�߶�
@param box          [in] ��������
@param color        [in] ��ɫֵ
@return 
@note
�߿�Ϊ2�����أ��һ�������Ŀ��>=10,�߶�>=12
*******************************************************************************/
PUBLIC void LCD_DrawShadowRoundedRectToBuf(
                                           GUI_COLOR_T  *buf_ptr,
                                           uint16       buf_width,
                                           uint16       buf_height,
                                           GUI_RECT_T   box,
                                           GUI_COLOR_T  color
                                           );

/***************************************************************************//*!
@brief  ���Բ�Ǿ���
@author Jassmine
@param dev_info_ptr [in] ͼ����Ϣ
@param box          [in] ��������
@param display_rect [in] ������
@param color        [in] ��ɫֵ
@return 
@note
��������Ŀ��>=10,�߶�>=12
*******************************************************************************/
PUBLIC void LCD_FillRoundedRect(
                                GUI_LCD_DEV_INFO const  *dev_info_ptr,
                                GUI_RECT_T              box,
                                GUI_RECT_T              display_rect,
                                GUI_COLOR_T             color
                                );

/***************************************************************************//*!
@brief ������Ӱ��Բ�Ǿ���
@author Jassmine
@param dev_info_ptr [in] ͼ����Ϣ
@param box          [in] ��������
@param display_rect [in] ������
@param color        [in] ��ɫֵ
@return 
@note 
��������Ŀ��>=10,�߶�>=12
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
//   Note: 1.???����??����?2??-?��??
//            2.��?��???3y?a3Y��|����
//            3.��??��?Y��3?�㨦?�����???3y?a3Y��|����
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

//����ARGB888�����ӿڡ����롿
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
