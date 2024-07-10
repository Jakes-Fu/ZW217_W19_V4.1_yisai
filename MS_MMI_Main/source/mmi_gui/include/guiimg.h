/*! @file guiimg.h
@brief ���ļ��ṩ��ͼ��Ľ���ͻ�ȡͼ����Ϣ
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 8/2007       Jassmine              Creat
*******************************************************************************/

/*! @page page_Img Img

(����)

*******************************************************************************/

/*! @addtogroup imgGrp Img
@brief imgģ��
@details
��ģ���ṩͼ��Ľ���ͻ�ȡͼ����Ϣ
@{
*******************************************************************************/


#ifndef _GUIIMG_H_
#define _GUIIMG_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"

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
/*! @enum GUIIMG_DATE_TYPE_E
@brief ͼ����������
*/
typedef enum
{
    GUIIMG_DATE_TYPE_RGB565,    /*!<RBG565*/
    GUIIMG_DATE_TYPE_ARGB888,   /*!<ARGB888*/
    GUIIMG_DATE_TYPE_JPG,       /*!<JPG,for encode*/
    GUIIMG_DATE_TYPE_MAX        /*!<����λ*/
} GUIIMG_DATE_TYPE_E;

/*! @struct GUIIMG_SRC_T
@brief ͼ��Դ��Ϣ\n
@note src_buf_ptr��full_path_ptrֻ��ȡ��һ�����ͬʱ���ڣ���ô������src_buf_ptr
*/
typedef struct 
{
    BOOLEAN     is_file;            /*!< �Ƿ�ͼ��Դ���ļ� */
    uint8       *src_buf_ptr;       /*!< ͼ��buffer��ַ @note is_fileΪFALSEʱ��Ч */
    uint32      src_data_size;      /*!< ͼ��buffer��С @note is_fileΪFALSEʱ��Ч */
    uint8       *alpha_buf_ptr;     /*!< alpha data buffer */
    uint32      alpha_buf_size;     /*!< alpha data size */
    wchar       *full_path_ptr;     /*!< ͼ���ļ�·�� @note is_fileΪTRUEʱ��Ч */
} GUIIMG_SRC_T;

/*! @struct GUIIMG_DISPLAY_T
@brief ����ͼ�����ʱ��������Ϣ
*/
typedef struct 
{
    BOOLEAN     is_handle_transparent;  /*!< �Ƿ���͸��,ֻ���gif,png,�������ARGB888,����ͼƬ���ݵ�buffer����*/
    BOOLEAN		is_exist_bg;            /*!< gif�Ƿ���Ҫ����,�Ǳ���ɫ����decode�������buffer */
    uint32      dest_width;             /*!< ͼƬĿ���� */
    uint32      dest_height;            /*!< ͼƬĿ��߶� */
    GUI_RECT_T  *crop_rect_ptr;         /*!< ��ȡͼƬ������PNULL�������ȫͼ */
} GUIIMG_DISPLAY_T;

/*! @struct GUIIMG_DEC_OUT_T
@brief ͼƬ������������Ϣ
*/
typedef struct 
{
    BOOLEAN             is_handle_alpha;        /*!< is handle alpha,only for png */
    uint8               *decode_data_ptr;       /*!< ����󱣴�ͼƬ���ݵ�buffer */
    uint32              decode_data_size;       /*!< ����buffer�Ĵ�С */
    uint32              actual_width;           /*!< ͼƬʵ�ʿ�� */
    uint32              actual_height;          /*!< ͼƬʵ�ʸ߶� */
    GUIIMG_DATE_TYPE_E  data_type;              /*!< ����󱣴�ͼƬ���ݵ�����*/
} GUIIMG_DEC_OUT_T;

/*! @struct GUIIMG_INFO_T
@brief ͼƬ�����Ϣ
*/
typedef struct 
{
    uint16  image_width;            /*!< ͼƬ��� */
    uint16  image_height;           /*!< ͼƬ�߶� */
} GUIIMG_INFO_T;

// ͼ��ͷ��Ϣ(�ڲ����ݽṹ)
typedef struct 
{
    uint8   *head_ptr;
    uint32  len;
    uint32  offset;
} GUIIMG_HEAD_BUF_T;

/*! @struct GUIIMG_BITMAP_T
@brief ͼ��buffer��Ϣ
*/
typedef struct 
{
    uint16              img_width;  /*!< ͼ���� */
    uint16              img_height; /*!< ͼ��߶� */
    void                *bit_ptr;   /*!< ͼ�����ݵ�ַ */
    GUIIMG_DATE_TYPE_E  date_type;  /*!< ͼ���������� */
} GUIIMG_BITMAP_T;

//display bmp param
typedef struct 
{
    BOOLEAN                is_horizontal;  //in:is horizontal display
    GUI_RECT_T             *rect_ptr;      //in:display rect
    GUI_POINT_T            *point_ptr;     //in:display start point
    GUIIMG_BITMAP_T        *bitmap_ptr;    //in:
    GUI_LCD_DEV_INFO const *lcd_dev_ptr;    //in:
    BOOLEAN                is_transparent_mode; //in: is transparent mode
    GUI_COLOR_T            transparent_color;   //in: transparent color
}GUIIMG_DISPLAYBMP_PARAM_T;


/*---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ͼƬ����
@author Jassmine
@param src_ptr [in] ָ��ͼ��Դ��Ϣ�ĵ�ַ
@param display_ptr [in] ָ��ͼ��������Ϣ�ĵ�ַ
@param output_ptr [in/out] ָ��ͼ������������Ϣ�ĵ�ַ���������������buffer����С�������������ݼ�ͼƬ��ʵ�ʿ��
@return �Ƿ���óɹ�
@note 
- �÷������Խ���jpg,bmp,wbmp,png gif�ȸ�ʽ��ͼƬ�����ڶ������͵�ͼƬ��ֻ�ܽ�һ֡
- �÷�����������RGB565������
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_Decode(
                             GUIIMG_SRC_T       *src_ptr,       //in:image info
                             GUIIMG_DISPLAY_T   *display_ptr,   //in:
                             GUIIMG_DEC_OUT_T   *output_ptr     //in/out:
                             );

/***************************************************************************//*!
@brief ��ȡBMPͼƬ����Ϣ
@author Jassmine
@param data_ptr [in] ͼƬ���ݵ�ַ
@param data_size [in] ͼƬ���ݴ�С
@param png_info_ptr [in/out] ָ��ͼƬ��Ϣ��ָ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetBmpInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *png_info_ptr   //in/out:
                                 );

/***************************************************************************//*!
@brief ��ȡWBMPͼƬ����Ϣ
@author Jassmine
@param data_ptr [in] ͼƬ���ݵ�ַ
@param data_size [in] ͼƬ���ݴ�С
@param wbmp_info_ptr [in/out] ָ��ͼƬ��Ϣ��ָ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetWbmpInfo(
                                  uint8         *data_ptr,      //in:
                                  uint32        data_size,      //in:
                                  GUIIMG_INFO_T *wbmp_info_ptr  //in/out:
                                  );
/***************************************************************************//*!
@brief ��ȡPNGͼƬ����Ϣ
@author Jassmine
@param data_ptr [in] ͼƬ���ݵ�ַ
@param data_size [in] ͼƬ���ݴ�С
@param png_info_ptr [in/out] ָ��ͼƬ��Ϣ��ָ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetPngInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *png_info_ptr   //in/out:
                                 );


/***************************************************************************//*!
@brief ��ȡGIFͼƬ����Ϣ
@author Jassmine
@param data_ptr [in] ͼƬ���ݵ�ַ
@param data_size [in] ͼƬ���ݴ�С
@param gif_info_ptr [in/out] ָ��ͼƬ��Ϣ��ָ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetGifInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *gif_info_ptr   //in/out:
                                 );


/***************************************************************************//*!
@brief ��ʾͼƬ
@author Jassmine
@param is_horizontal [in] �Ƿ������ʾ
@param rect_ptr [in] ��LCD�ϵ���ʾλ��
@param point_ptr [in] ͼƬ�����λ��
@param bitmap_ptr [in] ͼƬԴ��Ϣ
@param lcd_dev_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_DisplayBmp(
                                 BOOLEAN                is_horizontal,  //in:is horizontal display
                                 GUI_RECT_T             *rect_ptr,      //in:display rect
                                 GUI_POINT_T            *point_ptr,     //in:display start point
                                 GUIIMG_BITMAP_T        *bitmap_ptr,    //in:
                                 GUI_LCD_DEV_INFO const *lcd_dev_ptr    //in:
                                 );

/***************************************************************************//*!
@brief ����ͼƬ���ݻ��ͼƬ��Ϣ
@author Jassmine
@param data_ptr [in] ͼƬ����
@param data_size [in] ͼƬ���ݳ���
@param img_info_ptr [in/out] ͼƬ��Ϣ
@return �Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetImgInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *img_info_ptr   //in/out:
                                 );

/*****************************************************************************/
//  Description : display bmp bitmap
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIIMG_DisplayBmpEx(
                                   GUIIMG_DISPLAYBMP_PARAM_T *param_ptr
                                   );
#ifdef IM_ENGINE_SOGOU
PUBLIC BOOLEAN GUIIMG_DisplayBmpDst(GUIIMG_DISPLAYBMP_PARAM_T *param_ptr, 
                                    void *dc_data_ptr);
#endif

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
