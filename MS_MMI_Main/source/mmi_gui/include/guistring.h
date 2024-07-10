/*! @file guistring.h
@brief ���ļ�������string�ؼ��Ķ���ӿں��������� 
@author Xiaoqing.lu
@version 1.0
@date 07/24/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 07/24/2008     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_String String

(����)

*******************************************************************************/

/*! @addtogroup stringGrp String
@brief string�ؼ�
@details string�ؼ�
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
@brief �ַ���״̬
*/
typedef uint32 GUISTR_STATE_T;

/*! @def GUISTR_STATE_NONE
@brief none״̬������ʼ��״̬����ʱȫ��ʹ��Ĭ��ֵ
*/
#define GUISTR_STATE_NONE           0x00000000  // ��״̬Ϊ0��ʱ��ȫ��ʹ��Ĭ��ֵ
/*! @def GUISTR_STATE_ALIGN
@brief ʹ�ö��뷽ʽ����������ø�״̬�������۶��뷽ʽ������ã�����ʹ�����ϽǶ���
*/
#define GUISTR_STATE_ALIGN          0x00000001  // ʹ�ö��뷽ʽ
/*! @def GUISTR_STATE_ANGLE
@brief ʹ���谴�Ƕ���ת����������ø�״̬����ʹ�����˽Ƕȣ�Ҳ������ת
*/
#define GUISTR_STATE_ANGLE          0x00000002  // ʹ���谴�Ƕ���ת
/*! @def GUISTR_STATE_EFFECT
@brief ʹ������Ч������������ø�״̬�����Ӧ������Ч����������
*/
#define GUISTR_STATE_EFFECT         0x00000004  // ʹ������Ч��
/*! @def GUISTR_STATE_WORDBREAK
@brief ���ڶ����ı�����ᰴ�ֻ��У�Ĭ�ϲ��ᰴ�ֻ���
*/
#define GUISTR_STATE_WORDBREAK      0x00000008  // ���ڶ����ı�����ᰴ�ֻ��У�Ĭ�ϲ��ᰴ�ֻ���
/*! @def GUISTR_STATE_SINGLE_LINE
@brief ��ʾ�����ı��������λ�����ã�����ʾ�����ı�
*/
#define GUISTR_STATE_SINGLE_LINE    0x00000010  // ��ʾ�����ı��������λ�����ã�����ʾ�����ı�

/*! @def GUISTR_STATE_BOLD
@brief �ı�������
*/
#define GUISTR_STATE_BOLD           0x00000040  // �ı�������
/*! @def GUISTR_STATE_ITALIC
@brief �ı���ʾб����
*/
#define GUISTR_STATE_ITALIC         0x00000080  // �ı���ʾб����
/*! @def GUISTR_STATE_UNDERLINE
@brief �ı���ʾ�»���
*/
#define GUISTR_STATE_UNDERLINE      0x00000100  // �ı���ʾ�»���
/*! @def GUISTR_STATE_BACKGROUND
@brief �ı���ʾ����ɫ
*/
#define GUISTR_STATE_BACKGROUND     0x00000200  // �ı���ʾ����ɫ
/*! @def GUISTR_STATE_NOT_HALF_CHAR
@brief ������ʾ��ʱ���Ƿ���ʾ����ֵ������
*/
#define GUISTR_STATE_NOT_HALF_CHAR  0x00000400  // ������ʾ��ʱ���Ƿ���ʾ����ֵ������
/*! @def GUISTR_STATE_NO_HALF_LINE
@brief ��ʾ��ʱ���Ƿ���ʾ���е������
*/
#define GUISTR_STATE_NO_HALF_LINE   0x00000800  // ��ʾ��ʱ���Ƿ���ʾ���е������
/*! @def GUISTR_STATE_ELLIPSIS
@brief ���ø�״̬��ʾ��ʾ��ʱ�������ʾ��ȫ,����ʾ����
*/
#define GUISTR_STATE_ELLIPSIS       0x00001000  // ���ø�״̬��ʾ��ʾ��ʱ�������ʾ��ȫ,����ʾ����,������Ч,����ʱ���ǵ������ٶȵ�����,����֧��
/*! @def GUISTR_STATE_ALIGN_BY_DIR
@brief ����������ʾ����ı���뷽ʽ
*/
#define GUISTR_STATE_ALIGN_BY_DIR   0x00002000  // change alignment by direction
/*! @def GUISTR_STATE_INVERSE
@brief ��ɫ��ʾ
*/
#define GUISTR_STATE_INVERSE        0x00004000  // ��ɫ��ʾ
/*! @def GUISTR_STATE_SPEC_FONTCOLOR
@brief ������ɫ�ı�
*/
#define GUISTR_STATE_SPEC_FONTCOLOR 0x00008000  // ������ɫ�ı�
/*! @def GUISTR_STATE_GRADUAL_HOR
@brief ˮƽ����
*/
#define GUISTR_STATE_GRADUAL_HOR      0x00010000    //ˮƽ����
/*! @def GUISTR_STATE_GRADUAL_VER
@brief ��ֱ����
*/
#define GUISTR_STATE_GRADUAL_VER      0x00020000    //��ֱ����
/*! @def GUISTR_STATE_WORDSPACE
@brief �ʼ��
*/
#define GUISTR_STATE_WORDSPACE        0x00040000    //�ʼ��

/*! @def GUISTR_STATE_FROM_HEAD
@brief ��ͷ����ʾ, �����������
*/
#define GUISTR_STATE_FROM_HEAD      0x00080000  // ��β����ʾ, �������ҹ���

/*! @def GUISTR_STATE_FROM_TAIL
@brief ��β����ʾ, �������ҹ���
*/
#define GUISTR_STATE_FROM_TAIL      0x00100000  // ��β����ʾ, �������ҹ���

/*! @def GUISTR_STATE_ELLIPSIS_EX
@brief ���ø�״̬��ʾ��ʾ��ʱ�������ʾ��ȫ,����ʾʡ�Ե�
*/
#define GUISTR_STATE_ELLIPSIS_EX    0x00200000  // ���ø�״̬��ʾ��ʾ��ʱ�������ʾ��ȫ,����ʾʡ�Ե�,������Ч,����ʱ���ǵ������ٶȵ�����,����֧��

// �������ı���ʾ�ķ���,ע��:�ı���ʾ�ķ���
/*! @def GUISTR_TEXT_DIR_LTR
@brief �ı���ʾ�ķ��򣺴�������
*/
#define GUISTR_TEXT_DIR_LTR            (0)            //��������
/*! @def GUISTR_TEXT_DIR_RTL
@brief �ı���ʾ�ķ��򣺴�������
*/
#define GUISTR_TEXT_DIR_RTL            (1)            //��������
/*! @def GUISTR_TEXT_DIR_AUTO
@brief �ı���ʾ�ķ������ı�������������簢����������������Ӣ�Ĵ�����
*/
#define GUISTR_TEXT_DIR_AUTO           (-1)           //���ı��������

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! @typedef GUISTR_BUF_TYPE_T
@brief �����ı�buffer�ĸ�ʽ����
*/
typedef uint16 GUISTR_BUF_TYPE_T;
/*! @def GUISTR_BUF_RGB565
@brief RGB565��ʽ
*/
#define GUISTR_BUF_RGB565  0   //default
/*! @def GUISTR_BUF_ARGB888
@brief ARGB888��ʽ
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
@brief  string buffer����ʾһ����������ʾ�ڵ������������Ϣ
*/
typedef struct  
{
    void             *buffer;    /*!< ����buffer*/
    uint16            mem_width; /*!< �ڴ��С*/
    uint16            width;     /*!< ������*/
    uint16            height;    /*!< ����߶�*/
    GUISTR_BUF_TYPE_T type;      /*!< ����buffer��ʽ*/
}GUISTR_BUF_T;

/*! @struct  GUIFONT_REGION_STYLE_T 
@brief   �����ı�Ч�� Ŀǰֻ֧�ַ���
*/
typedef struct
{
    uint16  start;							/*!< ��ʼ��*/
    uint16  length;							/*!< ����*/
    GUISTR_STATE_T  state;			/*!< �ַ���״̬*/
    GUI_COLOR_T     font_color; /*!< �����ı�����ɫ,ע�⣬��������state |= GUISTR_STATE_SPEC_FONTCOLOR����Ч*/
}GUIFONT_REGION_STYLE_T;

/*! @struct  GUISTR_STYLE_T 
@brief   �ַ�������ʾ���
*/
typedef struct
{
    GUI_FONT_T	     font;       /*!< �����С*/
    GUI_COLOR_T	     font_color; /*!< ������ɫ*/
    GUI_ALIGN_E      align;      /*!< ���뷽ʽ*/
    GUI_ANGLE_E      angle;      /*!< �Ƕ�*/
    GUIFONT_EFFECT_T effect;     /*!< ��Ч*/
    GUI_COLOR_T      edge_color; /*!< ���ֱ߽���ɫ*/
    GUI_COLOR_T      bg_color;   /*!< ���ֱ�����ɫ*/
    GUI_COLOR_T      line_color; /*!< �����»�����ɫ*/
    int16            line_space; /*!< �м��*/
    int16            char_space; /*!< �ּ��*/
    int16            word_space; /*!< �ʼ��*/
    GUIFONT_REGION_STYLE_T *region_ptr;/*!< ��������������Ϣ*/
    uint16                 region_num;/*!< �������ֵ�������*/
    uint8       begin_alpha;    /*!< ������Ч��ʼalphaֵ*/
    uint8       end_alpha;      /*!< ������Ч����alphaֵ*/
    uint8             edge_alpha;
} GUISTR_STYLE_T;

/*! @struct  GUISTR_INFO_T 
@brief �ַ����Ŀ�/����Ϣ
*/
typedef struct  
{
    uint16      width;/*!< �ַ������ܿ��*/
    uint16      height;/*!< �ַ��������߶�*/
}GUISTR_INFO_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ��LCD buffer�ϻ��ַ�����������ʾ
@author xiaoqing.lu
@param dev_info_ptr [in] �豸��Ϣ
@param point_ptr [in] ��ʼ��
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@retval void
@return void
@note �ú�����������ʼ���lcd�豸��ϢΪ������ȡ����������ʾ������Ϣ������ʾʡ�Ժţ�Ҳ��֧�ֽǶ���ת
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDSingleLine( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_POINT_T      *point_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T   *style_ptr,
                                           GUISTR_STATE_T         state
                                           );


/***************************************************************************//*!
@brief ��Ŀ��buffer�ϻ��ı���Ϣ������ʼ��Ϊ��׼����ʾ������Ϣ
@author xiaoqing.lu
@param des_buf_ptr [in] �豸��Ϣ
@param point_ptr [in] ��ʼ��
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@retval ����
@return ������ʾ���ֵ�ʵ������
@note ��ʼ���λ�������buffer��ԭ�㣬ȡ����������ʾ������Ϣ������ʾʡ�Ժţ�Ҳ��֧�ֽǶ���ת
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
@brief ��LCD�ϻ��ַ���������ʾ���򡢼��������Լ�lcd������Ľ���Ϊ�������򣬽��ж�����ʾ
@author  
@param dev_info_ptr [in] �豸��Ϣ
@param disp_rect_ptr [in] ��ʾ����
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@retval void
@return void
@note ���ַ�����ʾ����ʾ�����ϣ��������û�����������ʾ���򡢼��������Լ�lcd����Ľ���
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
@brief ��LCD�ϻ��ַ���������ʼ��Ϊ��׼�����·��򣬶�����ʾ
@author xiaoqing.lu
@param dev_info_ptr [in] �豸��Ϣ
@param point_ptr [in] ��ʼ��
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@retval void
@return void
@note ��ʼ���λ�������buffer��ԭ�㣬ȡ����������ʾ������Ϣ������ʾʡ�Ժţ�Ҳ��֧�ֽǶ���ת
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDMultiLine( 
                                          const GUI_LCD_DEV_INFO *dev_info_ptr,
                                          const GUI_POINT_T      *point_ptr,
                                          const MMI_STRING_T     *str_ptr,
                                          GUISTR_STYLE_T   *style_ptr,
                                          GUISTR_STATE_T         state
                                          );


/***************************************************************************//*!
@brief ��buffer�ϻ��ı���Ϣ������ʼ��Ϊ��׼����buffer�Ŀ�Ⱥ͸߶�Ϊ���ޣ����·�����ʾ����
@author xiaoqing.lu
@param des_buf_ptr [in] �豸��Ϣ
@param point_ptr [in] ��ʼ��
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@retval ����
@return ������ʾ��ʵ������
@note  ��ʼ���λ�������buffer��ԭ��
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
@brief ��LCD�ϻ��ַ���������ʾ���򡢼��������Լ�lcd������Ľ���Ϊ�������򣬽��ж�����ʾ
@author  xiaoqing.lu
@param dev_info_ptr [in] �豸��Ϣ
@param disp_rect_ptr [in] ��ʾ����
@param clip_rect_ptr [in] ��������
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@param dir [in] �ַ�����ʾ�ķ���һ��ȡ-1������Ӧ
@retval void
@return void
@note ���ַ�����ʾ����ʾ�����ϣ��������û�����������ʾ���򡢼��������Լ�lcd����Ľ���
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDInRect( 
                                       const GUI_LCD_DEV_INFO *dev_info_ptr,
                                       const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                       const GUI_RECT_T      *clip_rect_ptr,       //�û�Ҫ���е�ʵ������
                                       const MMI_STRING_T    *str_ptr,
                                       GUISTR_STYLE_T        *style_ptr,
                                       GUISTR_STATE_T        state,
                                       int8                  dir
                                       );


/***************************************************************************//*!
@brief ��buffer�ϻ��ı���Ϣ������ʾ���򡢼��������Լ�buffer������Ľ���Ϊ�������򣬽��ж�����ʾ
@author xiaoqing.lu
@param des_buf_ptr [in] �豸��Ϣ
@param disp_rect_ptr [in] ��ʾ����
@param clip_rect_ptr [in] ��������
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@param dir [in] �ַ�����ʾ�ķ���һ��ȡ-1������Ӧ
@retval buffer�ϵ�ʵ������
@return buffer�ϵ�ʵ������
@note ���ַ�����ʾ����ʾ�����ϣ��������û�����������ʾ���򡢼��������Լ�buffer������Ľ���
*******************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextInRect( 
                                        GUISTR_BUF_T          *des_buf_ptr,
                                        const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                        const GUI_RECT_T      *clip_rect_ptr,       //�û�Ҫ���е�ʵ������
                                        const MMI_STRING_T    *str_ptr,
                                        GUISTR_STYLE_T        *style_ptr,
                                        GUISTR_STATE_T        state,
                                        int8                  dir
                                        );


/***************************************************************************//*!
@brief ��LCD�ϻ��ַ���
@author xiaoqing.lu
@param dev_info_ptr [in] �豸��Ϣ
@param disp_rect_ptr [in] ��ʾ����
@param clip_rect_ptr [in] ��������
@param x_offset [in] x���������ƶ����������ƶ���ϵͳ����
@param y_offset [in] y���������ƶ�Ϊ��ֵ,�����ƶ�Ϊ��ֵ
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@retval void
@return void
@note ����x��y�����ƫ�������Լ���ʾ���򡢼�������lcdԭʼ����Ľ���������ʾ��x��y�ı��������������ʾ�����ԭ���ƫ��
*******************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDByOffset( 
                                         const GUI_LCD_DEV_INFO *dev_info_ptr,
                                         const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                         const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                         int16                 x_offset, // x���������ƶ����������ƶ���ϵͳ����
                                         int16                 y_offset, // y���������ƶ�Ϊ��ֵ,�����ƶ�Ϊ��ֵ
                                         const MMI_STRING_T    *str_ptr,
                                         GUISTR_STYLE_T        *style_ptr,
                                         GUISTR_STATE_T        state
                                         );


/***************************************************************************//*!
@brief ��buffer�ϻ��ı���Ϣ������������Ϊ
@author xiaoqing.lu
@param des_buf_ptr [in] �豸��Ϣ
@param disp_rect_ptr [in] ��ʾ����
@param clip_rect_ptr [in] ��������
@param x_offset [in] x���������ƶ����������ƶ���ϵͳ����
@param y_offset [in] y���������ƶ�Ϊ��ֵ,�����ƶ�Ϊ��ֵ
@param str_ptr [in] �ַ�����Ϣ
@param style_ptr [in] �ַ�����ʾ���
@param state [in] �ַ�����ʾ��״̬
@retval ����
@return ��ʾ��ʵ������
@note ����x��y�����ƫ�������Լ���ʾ���򡢼�������lcdԭʼ����Ľ���������ʾ��x��y�ı��������������ʾ�����ԭ���ƫ��
*******************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextByOffset( 
                                          GUISTR_BUF_T          *des_buf_ptr,
                                          const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                          const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                          int16                 x_offset, // x���������ƶ����������ƶ���ϵͳ����
                                          int16                 y_offset, // y���������ƶ�Ϊ��ֵ,�����ƶ�Ϊ��ֵ
                                          const MMI_STRING_T    *str_ptr,
                                          GUISTR_STYLE_T        *style_ptr,
                                          GUISTR_STATE_T        state
                                          );

#ifndef GUI_STR_MINI_SUPPORT
/***************************************************************************//*!
@brief ����ƫ����������ʣ���ַ���������Ч���ֶ���
@author xiaoqing.lu
@param str_ptr [in] ԭʼ�ַ���
@param src_region_style_ptr [in] ԭʼ�ַ����������ı�������
@param src_region_num [in] ԭʼ�ַ����������ı�����ĸ���
@param pos [in] ����ʣ���ַ���ʱ��λ�ã���ֵ���ܴ���ԭʼ�ַ����ĳ���
@param des_region_style_pptr [in/out]�������ַ���������
@param des_region_num_ptr [in/out] �������ַ�������ĸ���
@retval true/false
@return ���سɹ���ʧ��
@note �ú����ڲ������ڴ棬�����GUISTR_DestroyRegionStyle�ɶ�ʹ�ã���������ڴ�й©
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
@brief �ͷ���Ч�ֶ��ڴ�
@author xiaoqing.lu
@param region_style_ptr [in] �ַ����������ı�������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUISTR_DestroyRegionStyle(
                                         GUIFONT_REGION_STYLE_T *region_style_ptr
                                         );

 
/***************************************************************************//*!
@brief ����ַ����Ŀ��
@author xiaoqing.lu
@param style_ptr [in] �ַ������
@param str_ptr [in] �ַ���
@param state [in] �ַ���״̬
@retval uint16
@return �ַ������
@note �ýӿڼ����ַ������ʱ���ῼ���ַ����ķ�񣬱���Ӵ֡���ߵ�Ӱ���ȵ�����
*******************************************************************************/
PUBLIC uint16 GUISTR_GetStringWidth(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state
                                    );

 
/***************************************************************************//*!
@brief ����ַ����Ŀ��
@author xiaoqing.lu
@param style_ptr [in] �ַ������
@param str_ptr [in] �ַ���
@param state [in] �ַ���״̬
@param info_ptr [in/out] �ַ��������Ϣ
@return ���سɹ���ʧ��
@note ע��ýӿ���guifont�е�GUI_GetStringWidth��GUI_GetStringWidthEx������Ϊ�ýӿڵĲ���
*******************************************************************************/
PUBLIC BOOLEAN GUISTR_GetStringInfo(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state,
                                    GUISTR_INFO_T        *info_ptr //out
                                    );

 
/***************************************************************************//*!
@brief �ַ������ֺ�����ΪGUI_GetStringInfoInLine����չ, ֧��������Ч
@author James.Zhang
@param dir [in] �ַ���ʾ����
@param str_ptr [in] �ַ���buffer
@param str_len [in] �ַ�������
@param style_ptr [in] �ַ������
@param state [in] �ַ�����ʾ״̬
@param width [in] ��ʾ����Ŀ��
@param char_num [in/out] �ַ���
@param pixel_num [in/out] ������
@retval true/false
@return ���سɹ���ʧ��
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
