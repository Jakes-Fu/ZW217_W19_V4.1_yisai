/*! @file guicommon.h
@brief ���ļ�������common�ؼ��Ķ���ӿں��������� 
@author panda.luo
@version 1.0
@date 02/04/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 04/2004       panda.luo          Create        
*******************************************************************************/

/*! @page page_Commom Commom

(����)

*******************************************************************************/

/*! @addtogroup commomGrp Commom
@brief commomģ��
@details commomģ��
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
@brief ascii code �����
*/
#define UNICODE_ASCII		0X23

/*! @def UNICODE_HANZI
@brief ���ֿ�ȶ���һ����,��'һ'����
*/
#define UNICODE_HANZI		0x4e00

/*! @def GUI_INVALID_PARAM
@brief ...
*/
#define GUI_INVALID_PARAM(_PTR) (_PTR)

/*! @def GUI_RGB5652ARGB8888
@brief RGB565ת����ARGB888
*/
#define GUI_RGB5652ARGB8888(rgb565, alpha)  \
    (((alpha)<<24) | ((((rgb565) >> 8) & 0xf8)<<16) | ((((rgb565) >> 3) & 0xfc)<<8) | (((rgb565) << 3) & 0xf8))

/*! @def GUI_RGB8882RGB565
@brief RGB888ת����RGB565
*/
#define GUI_RGB8882RGB565(rgb888)           \
    ((((rgb888)&0xf80000)>>8) | (((rgb888)&0xfc00)>>5) | (((rgb888)&0xf8)>>3))

/*! @def GUI_RGB2RGB565
@brief ����RGBֵת����RGB565
*/
#define GUI_RGB2RGB565(r, g, b)             \
    ((((r)&0xf8)<<8) | (((g)&0xfc)<<3) | (((b)&0xf8)>>3))

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @typedef GUI_TIMER_DURATION_T
@brief ����timer��ʱ����(ms)
*/
typedef uint32				   	GUI_TIMER_DURATION_T;

/*! @typedef GUI_TIMER_ID_T
@brief timer id 
*/
typedef uint8				   	GUI_TIMER_ID_T;

/*!  @typedef GUI_RETURN_E
@brief GUI �ؼ��ķ���ֵ
*/
typedef enum
{
    GUI_RET_SUCCESS,    /*!< �ɹ� */
    GUI_RET_RECT_SHORT, /*!< �����ý��� */
    GUI_RET_ERR_PARAM   /*!< �����ý��� */
}GUI_RETURN_E;

/*!  @typedef GUI_SHEEN_DEPTH_E
@brief �������
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
@brief  �������ľ����Ƿ��������Ļ����ʾ
@warning ֻ����DEBUGģʽ�²ſ������˼��
*/
#ifdef _DEBUG
#define GUI_CHECK_RECT(rect)    GUI_CheckRectValid(rect)
#else
#define GUI_CHECK_RECT(rect)
#endif

/*! @struct GUI_BG_DISPLAY_T
@brief ��������
@note �������ñ�����ʾ����ز���
*/
typedef struct
{
    BOOLEAN         is_transparent; /*!< ͼƬ�Ƿ��͸�� */
    BOOLEAN         is_form_bg;     /*!< �Ƿ�֧�ֶ��� */
    GUI_RECT_T      rect;           /*!< ���б������� */
    GUI_RECT_T      display_rect;   /*!< ��ʾ���� */
    GUI_RECT_T      img_rect;       /*!< ͼƬ����  */
    MMI_HANDLE_T    ctrl_handle;    /*!< �ؼ�handle  */
    MMI_HANDLE_T    win_handle;     /*!< ����handle  */
} GUI_BG_DISPLAY_T;

/*! @struct GUI_SHEEN_INPUT_T
@brief button����Ч������
*/
typedef struct
{
    GUI_LCD_DEV_INFO    lcd_dev;        /*!< lcd and block id*/
    GUI_RECT_T          rect;           /*!< ��������  */
    GUI_SHEEN_DEPTH_E   center_depth;   /*!< ���Ĺ������ */ 
    GUI_SHEEN_DEPTH_E   side_depth;     /*!< ��Ե������� */ 
} GUI_SHEEN_INPUT_T;

/*! @struct GUI_SHEEN_T
@brief ������ʾ����Ч��
*/
typedef struct
{
    BOOLEAN             is_pixel_bit16; /*!< ���ص��Ƿ���16λ  */
    uint16              dest_buf_width; /*!< Ŀ����  */
    void                *dest_buf_ptr;  /*!< Ŀ��ָ��  */
    void                *copy_buf_ptr;  /*!< copy buffer */ 
    GUI_SHEEN_INPUT_T   input;          /*!< input */
} GUI_SHEEN_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ����һ������
@author panda.luo
@param    left    [in]  rect left
@param    top     [in]  rect top
@param    right   [in]  rect right
@param    bottom  [in]  rect bottom
@return ��������������
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_CreateRect(
                                 uint16 				left,
                                 uint16 				top,
                                 uint16 				right,
                                 uint16 				bottom
                                 );

/***************************************************************************//*!
@brief ������1������2�Ƿ����ཻ���� 
@author bin.ji
@param      *rect_dest_ptr  [in]    �ཻ���ֵ���Ϣ
@param      rect1           [in]    ����1
@param      rect2           [in]    ����2
@return ����ֵFALSEû���ཻ���֣�TRUE���ཻ����
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IntersectRect(
                                 GUI_RECT_T    		*rect_dest_ptr,
                                 GUI_RECT_T    		rect1,
                                 GUI_RECT_T    		rect2
                                 );

/***************************************************************************//*!
@brief �ж������Ƿ�Ϊ�� 
@author James.Zhang
@param  rect  [in]  ��Ҫ�жϵ���������
@return Ϊ�շ���TRUE����Ϊ�շ���FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsRectEmpty(
                               GUI_RECT_T rect 
                               );

/***************************************************************************//*!
@brief �ж������Ƿ�Ϊ0 
@author James.Zhang
@param  rect  [in]  ��Ҫ�жϵ���������
@return Ϊ0����TRUE����Ϊ0����FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsZeroRect( 
                              GUI_RECT_T rect 
                              );

/***************************************************************************//*!
@brief �ж����������Ƿ�Ϸ�
@author xiaoqing.lu
@param  rect  [in]  ��Ҫ�жϵ���������
@return ����Ϸ�����TRUE�����Ϸ�����FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsInvalidRect( 
                                 GUI_RECT_T rect 
                                 );

/***************************************************************************//*!
@brief �ж����������Ƿ�Ϸ�
@author xiaoqing.lu
@param  rect  [in]  ��Ҫ�жϵ���������
@return ����Ϸ�����TRUE�����Ϸ�����FALSE
@note  
@ �жϾ������Ƿ���Ч���������ҡ��µ����������������Ч�㣬��GUI_IsInvalidRect
@ �򲻰�����MOCORƽ̨��RECT��ʵ�ǰ������µ�ģ���������˵��GUI_IsInvalidRect
@ ��ʵ�������⣬������rect={0,0,0,0}����Ϊ��Ч���ܶ���GUI_IsInvalidRect�жϣ�
@ ������õĵط�̫�࣬��ʱ����GUI_IsInvalidRect����������¼�һ��Ex����
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsInvalidRectEx( 
                                   GUI_RECT_T rect 
                                   );

/***************************************************************************//*!
@brief �������ľ����Ƿ��������Ļ����ʾ 
@author panda.luo
@param  rect  [in]  ��Ҫ����������Ϣ
@return ������ʾ����TRUE����������ʾ����FALSE
@note  
*******************************************************************************/
PUBLIC BOOLEAN GUI_CheckRectValid(		
                                  GUI_RECT_T rect
                                  );

/***************************************************************************//*!
@brief ��ָ����ɫ�����ο� 
@author panda.luo
@param  dev_info_ptr   [in]    id of lcd
@param  rect    [in]    ��Ҫ��������
@param  color   [in]    ������ɫ
@return ��
@note   
*******************************************************************************/
PUBLIC void GUI_FillRect(
                         GUI_LCD_DEV_INFO const *dev_info_ptr,	// block information
                         GUI_RECT_T    			rect,
                         GUI_COLOR_T   			color
                         );

/***************************************************************************//*!
@brief �õ�a����b���� 
@author Jibin
@param    a   [in]  paramter a
@param    b   [in]  paramter b
@return �õ�a����b����
@note   
*******************************************************************************/
PUBLIC uint16 GUI_Divide(
                         uint16 a,
                         uint16 b
                         );

/***************************************************************************//*!
@brief �Ƚ������ַ���
@author lin.lin
@param    *str1       [in]  compared string1
@param    str1_len    [in]  string1 ����
@param    *str2       [in]  compared string2
@param    str2_len    [in]  string2 ����
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
@brief ����ַ����Ŀ�� 
@author Great.Tian
@param    *str_ptr    [in]  �ַ���ָ��
@param    str_len     [in]  �ַ�������
@param    font        [in]  �ַ�����
@param    char_space	�ּ��
@return �����ַ����Ŀ��
@note  
*******************************************************************************/
PUBLIC uint16 GUI_CalculateStringPiexlNum(
                                          const wchar				*str_ptr,
                                          uint16     			str_len,
                                          GUI_FONT_T 			font,
                                          uint8      			char_space
                                          );

/***************************************************************************//*!
@brief �����ַ���(�������ظ���)�õ�����
@author bin.ji
@param    pixelnum     [in] ���ظ���
@param    *str_ptr     [in] Stringָ��
@param    str_len      [in] String����(Byte����)
@param    font         [in] String������
@param    char_space   [in] �ּ��
@param    is_byword    [in] �Ƿ񰴴ʻ���
@return ����
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
@brief �����������ظ����õ��ַ�����
@author Jassmine
@param    pixelnum    [in]  ���ظ���
@param    w_char      [in]  Stringָ��
@param    char_len    [in]  String����(Byte����)
@param    font        [in]  String������
@param    char_space  [in]  �ּ��
@return �ַ�����
@note 
*******************************************************************************/
PUBLIC uint16 GUI_CalculateCharLinesByPixelNum(
                                               uint16       pixelnum,   //[IN] ���ظ���
                                               wchar        w_char,     //[IN] Stringָ��
                                               uint16       char_len,    //[IN] String����(Byte����)
                                               GUI_FONT_T   font,       //[IN] String������
                                               uint8        char_space  //[IN] �ּ��
                                               );

/***************************************************************************//*!
@brief ������ʾʱ�����һ�е�һ���ַ�����λ��
@author bin.ji
@param line_num		[in]    ����
@param pixelnum		[in]    ���ظ���
@param *str_ptr		[in]    �ַ���ָ��
@param str_len		[in]    �ַ�������
@param font			[in]    ����
@param char_space	[in]    �ּ��
@return  ������ʾʱ�����һ�е�һ���ַ�����λ��
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
@brief �ú������ڶ���,���ܻ�ȡ�ַ����������ظ���
@author James.Zhang
@param  *str_ptr      [in]  �ַ�ָ��
@param  str_len       [in]  �ַ�������
@param  font          [in]  ����
@param  char_space    [in]  �ּ��
@param  width         [in]  ���
@param  *char_num     [in]  �ַ�����
@param  *pixel_num    [in]  ���ظ���
@param  is_byword     [in]  �Ƿ�word
@return ���гɹ�������TRUE��ʧ�ܷ���FALSE
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
@brief �������ֵķ���, ����˫������ʱ���в��ܸ���׼ȷ
@author James.Zhang
@param dir          [in]    ���ַ���
@param *str_ptr     [in]    �ַ�ָ��
@param str_len      [in]    �ַ�������
@param font         [in]    ����
@param char_space   [in]    �ּ��
@param width        [in]    ���
@param char_num     [in]    �ַ�����
@param pixel_num    [in]    ���ظ���
@param is_byword    [in]    �Ƿ�word
@return ��ȡһ�е���Ϣ�ɹ�����TRUE��ʧ�ܷ���FALSE
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
@brief ��ȡһ���е��ַ���Ϣ(�������ظ������ַ�����)
@author Jassmine
@param w_char           [in]    w_char 
@param char_len         [in]    �ַ�����  
@param font             [in]    ����
@param char_space       [in]    �ּ��
@param width            [in]    ���
@param char_num_ptr     [in]    �ַ�����ָ��
@param pixel_num_tr     [in]    ���ظ���ָ��
@return ��
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
@brief �жϸõ��Ƿ��������ڲ�
@author  Robert.Lu
@param  point   [in]    ��Ҫ�жϵĵ�
@param  rect    [in]    �ж�����
@return ���������ڷ���FALSE, �������ڷ���TRUE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_PointIsInRect(
                                 GUI_POINT_T    point,
                                 GUI_RECT_T     rect 
                                 );

/***************************************************************************//*!
@brief �ж����������Ƿ����
@author Hui.Zhang
@param rect_1 [in]  ����1
@param rect_2 [in]  ����2
@return ��ȷ���TURE������ȷ���FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_EqualRect(
                             GUI_RECT_T    rect_1,
                             GUI_RECT_T    rect_2
                             );

/***************************************************************************//*!
@brief �ж��Ƿ�������
@author liqing.peng
@param year [in]    year
@return �����꣬����TRUE���������꣬����FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsLeapYear(
                              uint32 year
                              );

/***************************************************************************//*!
@brief �ж�A�����Ƿ���ȫ��B������
@author bin.ji
@param orig_rect    [in]    A����
@param rect         [in]    B����
@return A��B��Χ�ڣ�����TRUE�����򷵻�FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_RectIsCovered(
                                 GUI_RECT_T    orig_rect,
                                 GUI_RECT_T    rect
                                 );

/***************************************************************************//*!
@brief �Ƿ�Ϊ���ʶ����ַ�
@author James Zhang
@param  ch    [in]    �ַ�
@return �ǰ��ʶ��У�����TRUE�����򣬷���FALSE
@note �Ƿ�Ϊ���ʶ����ַ�,�ڰ��ʶ���ʱ������,һ��ı����Ŷ���
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsWordBreakChar(
                                   wchar ch 
                                   );

/***************************************************************************//*!
@brief ����һ��������ÿ��Item������ 
@author xiaoqing.lu
@param total_rect_ptr   [in]    ��������
@param h_side_margin    [in]    ˮƽ�߾�
@param v_side_margin    [in]    ��ֱ�߾�
@param h_item_space     [in]    ˮƽ���
@param v_item_space     [in]    ��ֱ���
@param row_num          [in]    ����
@param col_num          [in]    ����
@param item_rect_ptr    [in]    �����item ������һ����λ���飬�к�����line_num��row_num����
@retval 
@return  ��
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
@brief ��ʾ�߿�
@author Jassmine
@param rect             [in]    �߽�����
@param display_rect     [in]    ��ʾ����
@param border_ptr       [in]    �߽�ָ��
@param lcd_dev_ptr      [in]    lcd��Ϣָ��
@return ��
@note ��ʾ�߿�
*******************************************************************************/
PUBLIC void GUI_DisplayBorder(
                              GUI_RECT_T        rect,           //in:
                              GUI_RECT_T        display_rect,   //in:
                              GUI_BORDER_T      *border_ptr,    //in:
                              GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                              );

/***************************************************************************//*!
@brief ��ʾ�߿�ָ��buffer
@author Jassmine
@param buf_ptr      [in] bufferָ��
@param buf_size     [in] buffer��С
@param buf_width    [in] buffer���
@param buf_height   [in] buffer�߶�
@param rect         [in] �߽�����
@param border_ptr   [in] �߽�ָ��
@return ��
@note ��ʾ�߿�
*******************************************************************************/
PUBLIC void GUI_DisplayBorderToBuf(
                                   GUI_COLOR_T      *buf_ptr,   //in:
                                   uint16           buf_width,  //in:
                                   uint16           buf_height, //in:
                                   GUI_RECT_T       rect,       //in:�����buf
                                   GUI_BORDER_T     *border_ptr //in:
                                   );

/***************************************************************************//*!
@brief ��ʾ����
@author Jassmine
@param bg_ptr           [in]    ������Ϣָ��
@param display_ptr      [in]    ��ʾ��Ϣָ��
@param lcd_dev_ptr      [in]    lcd��Ϣָ��
@return �����Ƿ����
@note 
*******************************************************************************/
PUBLIC void GUI_DisplayBg(
                          GUI_BG_T          *bg_ptr,        //in:
                          GUI_BG_DISPLAY_T  *display_ptr,   //in:
                          GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
                          );

/*****************************************************************************//*!
@brief ��ʾ����
@author Jassmine
@param bg_ptr           [in]    ������Ϣָ��
@param display_ptr      [in]    ��ʾ��Ϣָ��
@is_notify_update       [in]    ������ΪGUI_BG_NONEʱ���Ƿ������������׷���MSG_NOTIFY_UPDATE
@param lcd_dev_ptr      [in]    lcd��Ϣָ��
@return �����Ƿ����
@note 
/*****************************************************************************/
PUBLIC void GUI_DisplayBgExt(
							 GUI_BG_T          *bg_ptr,        //in:
							 GUI_BG_DISPLAY_T  *display_ptr,   //in:
							 BOOLEAN			is_notify_update, // ������ΪGUI_BG_NONEʱ���Ƿ������������׷���MSG_NOTIFY_UPDATE
							 GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in:
							 );

/***************************************************************************//*!
@brief ��ȡ�м�����
@author xiaoqing.lu
@param src_rect     [in]    ԭʼ������Ϣ
@param width        [in]    ���
@param height       [in]    �߶�
@return ����ֵһ������,���ص����򲻻ᳬ��ԭʼ����
@note ���������ԭʼ�����м�����ĸ߶ȺͿ�ȣ���һ��������м�����
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_GetCenterRect(
                                    GUI_RECT_T src_rect,
                                    uint16     width,
                                    uint16     height
                                    );

/***************************************************************************//*!
@brief ��ȡ�м�����
@author xiaoqing.lu
@param src_rect     [in]    ԭʼ������Ϣ
@param width        [in]    ���
@param height       [in]    �߶�
@return ����ֵһ�����򣬷��ص�������Գ���ԭʼ����
@note ���������ԭʼ�����м�����ĸ߶ȺͿ�ȣ���һ��������м�����
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_GetCenterRectExt(
                                       GUI_RECT_T src_rect,
                                       uint16     width,
                                       uint16     height
                                       );

/***************************************************************************//*!
@brief ���ú��������������
@author Jassmine
@param ctrl_handle          [in]    ctrl_handle
@param rect_ptr             [in]    ����ָ��
@param both_rect_ptr        [in]    ˫������ָ��
@return ��
@note ���ú��������������
*******************************************************************************/
PUBLIC void GUI_SetVOrHRect(
                            MMI_HANDLE_T        ctrl_handle,
                            GUI_RECT_T          *rect_ptr,      //in:
                            GUI_BOTH_RECT_T     *both_rect_ptr  //in/out:
                            );

/***************************************************************************//*!
@brief ������ʾ��������ʾ����
@author Jassmine
@param total_ptr    [in]  ����������Ϣָ��
@param rect_ptr     [in   ������������Ϣָ��
@return �����Ƿ�ɹ�
@note ������ʾ��������ʾ����
*******************************************************************************/
PUBLIC BOOLEAN GUI_CheckRectDirection(
                                      const GUI_RECT_T *total_ptr, //in:out, ��rect
                                      GUI_RECT_T       *rect_ptr   //in:out
                                      );

/***************************************************************************//*!
@brief ȡ��������ĺϼ�
@author xiaoqing.lu
@param rect1_ptr    [in]    ����1��ָ��
@param rect2_ptr    [in]    ����2��ָ��
@return �ϲ����ָ��
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T GUI_AddupRect(
                                GUI_RECT_T *rect1_ptr,
                                GUI_RECT_T *rect2_ptr
                                );

/***************************************************************************//*!
@brief ��ʾ����
@author Jassmine
@param sheen_input_ptr    [in]  ����1��ָ��
@return ����\link #GUI_SHEEN_T \endlinkָ���button
@note ��ʾ����
*******************************************************************************/
PUBLIC GUI_SHEEN_T* GUI_DisplaySheen(
                                     GUI_SHEEN_INPUT_T  *sheen_input_ptr  //in:
                                     );

/***************************************************************************//*!
@brief �ͷŹ�����Ϣ
@author Jassmine
@param *sheen_ptr    [in]   ������Ϣָ��
@param is_recover    [in]   �Ƿ񸲸ǹ���Ч��
@return ��
@note �ͷŹ�����Ϣ
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
//  Description : ��ʼ��ʱ
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void GUI_StartTiming(void);

/*****************************************************************************/
//  Description : ��ӡ��ʱ
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
//  Note: �ڴ�alpha��ͼ���ϻ���ʱ,trueΪ����,����Ϊ�ϳ�. ����ֵΪoldֵ
//        ���ޱ���,����Ϊ����,�ܹ����ͼƬ���ֵ���Ⱦ�ٶ�
/*****************************************************************************/
PUBLIC BOOLEAN GUI_SetAlphaOverwrite(
                                     BOOLEAN is_overwrite
                                     );

/*****************************************************************************/
//  Description : is alpha overwrite
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: �ڴ�alpha��ͼ���ϻ���ʱ,trueΪ����,����Ϊ�ϳ�.
//        ���ޱ���,����Ϊ����,�ܹ����ͼƬ���ֵ���Ⱦ�ٶ�
/*****************************************************************************/
PUBLIC BOOLEAN GUI_IsAlphaOverwrite(
                                    void
                                    );

/*****************************************************************************/
//  Description : dump memory data
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
// ��ӡ�ڴ����ݣ���16���Ʊ�ʾ
// dump_len: ��ʾһ�д�ӡ�ĸ���
// dump_num: ��ʾ�ܹ�Ҫ��ӡ������
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
