/*****************************************************************************
** File Name:      ctrlwfcommon.h                                            *
** Author:                                                                   *
** Date:           08/13/2021                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe watch face common API       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2021      yuming.yang          Create                                  *
******************************************************************************/

#ifndef _CTRLWFCOMMON_H_
#define _CTRLWFCOMMON_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guiimg.h"
#include "ctrlwf_export.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_IMAGE_FULL_PATH_MAX_LEN         (128)
#define  WATCHFACE_CREATE_LAYER_MARGIN             (5) //边界扩展5个像素，保证有足够的显示区域
#define  WATCHFACE_COMMON_BASE_ATTR \
                MMI_HANDLE_T      handle; \
                MMI_WIN_ID_T      win_id; \
                float             scale;  \
                GUI_LCD_DEV_INFO  layer;


#ifndef WIN32
#define CTRL_WF_TRACE(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "CTRL_WF", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#else
#define CTRL_WF_TRACE        SCI_TRACE_LOW
#endif


/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR
}WATCHFACE_COMMON_ITEM_T;


/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : calculate display rect by center point and img info
//  Parameter: [In] center_point
//             [In] img_info
//             [Out] rect_ptr
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_GetDisplayRect(GUI_POINT_T center_point,GUIIMG_INFO_T img_info, GUI_RECT_T* p_rect, float scale);

/*****************************************************************************/
//  Description : get image width and height by image path
//  Parameter: [In] p_full_path_wstr: 文件全路径
//             [Out] p_img_info: img信息
//             [Return] 成功或失败
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_GetImgInfoByPath(wchar* p_full_path_wstr,
                                                                 GUIIMG_INFO_T* p_img_info
                                                                 );

/*****************************************************************************/
//  Description : create image ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] disp_rect: 绘制区域
//             [In] p_image_name: 资源路径
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_Common_CreateImg(MMI_WIN_ID_T         win_id,
                                                           MMI_CTRL_ID_T        ctrl_id,
                                                           GUI_RECT_T*          disp_rect,
                                                           wchar*               p_image_name,
                                                           float                scale,
                                                           GUI_LCD_DEV_INFO*    p_lcd_info
                                                           );

/*****************************************************************************/
//  Description : update image ctrl
//  Parameter: [In] ctrl_id
//             [In] disp_rect: 绘制区域
//             [In] p_image_name: 资源路径
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_UpdateImg(MMI_CTRL_ID_T        ctrl_id,
                                                       GUI_RECT_T*          disp_rect,
                                                       wchar*               p_image_name,
                                                       float                scale,
                                                       GUI_LCD_DEV_INFO*    p_lcd_info
                                                       );

/*****************************************************************************/
//  Description : update image disp
//  Parameter: [In] ctrl_id
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Common_DisplayImg(MMI_CTRL_ID_T ctrl_id, GUI_LCD_DEV_INFO*    p_lcd_info);


/*****************************************************************************/
//  Description : create layer
//  Parameter: [In] win_id
//             [In] width: layer宽度
//             [In] height: layer高度
//             [Out] p_lcd_dev: layer info
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Common_CreateLayer(GUI_LCD_DEV_INFO*     p_lcd_dev,
                                                      MMI_WIN_ID_T          win_id,
                                                      uint16                width,
                                                      uint16                height
                                                      );

#endif


