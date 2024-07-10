/*! @file guiimg.h
@brief 此文件提供了图像的解码和获取图像信息
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 8/2007       Jassmine              Creat
*******************************************************************************/

/*! @page page_Img Img

(暂无)

*******************************************************************************/

/*! @addtogroup imgGrp Img
@brief img模块
@details
该模块提供图像的解码和获取图像信息
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
@brief 图像数据类型
*/
typedef enum
{
    GUIIMG_DATE_TYPE_RGB565,    /*!<RBG565*/
    GUIIMG_DATE_TYPE_ARGB888,   /*!<ARGB888*/
    GUIIMG_DATE_TYPE_JPG,       /*!<JPG,for encode*/
    GUIIMG_DATE_TYPE_MAX        /*!<保留位*/
} GUIIMG_DATE_TYPE_E;

/*! @struct GUIIMG_SRC_T
@brief 图像源信息\n
@note src_buf_ptr与full_path_ptr只能取其一，如果同时存在，那么优先用src_buf_ptr
*/
typedef struct 
{
    BOOLEAN     is_file;            /*!< 是否图像源是文件 */
    uint8       *src_buf_ptr;       /*!< 图像buffer地址 @note is_file为FALSE时有效 */
    uint32      src_data_size;      /*!< 图像buffer大小 @note is_file为FALSE时有效 */
    uint8       *alpha_buf_ptr;     /*!< alpha data buffer */
    uint32      alpha_buf_size;     /*!< alpha data size */
    wchar       *full_path_ptr;     /*!< 图像文件路径 @note is_file为TRUE时有效 */
} GUIIMG_SRC_T;

/*! @struct GUIIMG_DISPLAY_T
@brief 描述图像解码时的区域信息
*/
typedef struct 
{
    BOOLEAN     is_handle_transparent;  /*!< 是否处理透明,只针对gif,png,是则输出ARGB888,保存图片数据的buffer增加*/
    BOOLEAN		is_exist_bg;            /*!< gif是否需要背景,是背景色填入decode后的数据buffer */
    uint32      dest_width;             /*!< 图片目标宽度 */
    uint32      dest_height;            /*!< 图片目标高度 */
    GUI_RECT_T  *crop_rect_ptr;         /*!< 截取图片的区域，PNULL代表界面全图 */
} GUIIMG_DISPLAY_T;

/*! @struct GUIIMG_DEC_OUT_T
@brief 图片解码后的数据信息
*/
typedef struct 
{
    BOOLEAN             is_handle_alpha;        /*!< is handle alpha,only for png */
    uint8               *decode_data_ptr;       /*!< 解码后保存图片数据的buffer */
    uint32              decode_data_size;       /*!< 数据buffer的大小 */
    uint32              actual_width;           /*!< 图片实际宽度 */
    uint32              actual_height;          /*!< 图片实际高度 */
    GUIIMG_DATE_TYPE_E  data_type;              /*!< 解码后保存图片数据的类型*/
} GUIIMG_DEC_OUT_T;

/*! @struct GUIIMG_INFO_T
@brief 图片宽高信息
*/
typedef struct 
{
    uint16  image_width;            /*!< 图片宽度 */
    uint16  image_height;           /*!< 图片高度 */
} GUIIMG_INFO_T;

// 图像头信息(内部数据结构)
typedef struct 
{
    uint8   *head_ptr;
    uint32  len;
    uint32  offset;
} GUIIMG_HEAD_BUF_T;

/*! @struct GUIIMG_BITMAP_T
@brief 图像buffer信息
*/
typedef struct 
{
    uint16              img_width;  /*!< 图像宽度 */
    uint16              img_height; /*!< 图像高度 */
    void                *bit_ptr;   /*!< 图像数据地址 */
    GUIIMG_DATE_TYPE_E  date_type;  /*!< 图像数据类型 */
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
@brief 图片解码
@author Jassmine
@param src_ptr [in] 指向图像源信息的地址
@param display_ptr [in] 指向图像区域信息的地址
@param output_ptr [in/out] 指向图像解码后数据信息的地址，输入解码后的数据buffer及大小，输出解码的数据及图片的实际宽高
@return 是否调用成功
@note 
- 该方法可以解码jpg,bmp,wbmp,png gif等格式的图片，对于动画类型的图片，只能解一帧
- 该方法解码生成RGB565的数据
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_Decode(
                             GUIIMG_SRC_T       *src_ptr,       //in:image info
                             GUIIMG_DISPLAY_T   *display_ptr,   //in:
                             GUIIMG_DEC_OUT_T   *output_ptr     //in/out:
                             );

/***************************************************************************//*!
@brief 获取BMP图片的信息
@author Jassmine
@param data_ptr [in] 图片数据地址
@param data_size [in] 图片数据大小
@param png_info_ptr [in/out] 指向图片信息的指针
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetBmpInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *png_info_ptr   //in/out:
                                 );

/***************************************************************************//*!
@brief 获取WBMP图片的信息
@author Jassmine
@param data_ptr [in] 图片数据地址
@param data_size [in] 图片数据大小
@param wbmp_info_ptr [in/out] 指向图片信息的指针
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetWbmpInfo(
                                  uint8         *data_ptr,      //in:
                                  uint32        data_size,      //in:
                                  GUIIMG_INFO_T *wbmp_info_ptr  //in/out:
                                  );
/***************************************************************************//*!
@brief 获取PNG图片的信息
@author Jassmine
@param data_ptr [in] 图片数据地址
@param data_size [in] 图片数据大小
@param png_info_ptr [in/out] 指向图片信息的指针
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetPngInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *png_info_ptr   //in/out:
                                 );


/***************************************************************************//*!
@brief 获取GIF图片的信息
@author Jassmine
@param data_ptr [in] 图片数据地址
@param data_size [in] 图片数据大小
@param gif_info_ptr [in/out] 指向图片信息的指针
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIIMG_GetGifInfo(
                                 uint8          *data_ptr,      //in:
                                 uint32         data_size,      //in:
                                 GUIIMG_INFO_T  *gif_info_ptr   //in/out:
                                 );


/***************************************************************************//*!
@brief 显示图片
@author Jassmine
@param is_horizontal [in] 是否横向显示
@param rect_ptr [in] 在LCD上的显示位置
@param point_ptr [in] 图片的起点位置
@param bitmap_ptr [in] 图片源信息
@param lcd_dev_ptr [in] 图层信息
@return 是否调用成功
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
@brief 根据图片数据获得图片信息
@author Jassmine
@param data_ptr [in] 图片数据
@param data_size [in] 图片数据长度
@param img_info_ptr [in/out] 图片信息
@return 是否成功
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
