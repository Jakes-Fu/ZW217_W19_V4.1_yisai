/*****************************************************************************
** File Name:      mmitheme_coverbar.h                                           *
** Author:                                                                   *
** Date:           06/24/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/24/2011     bin.ji              Creat
******************************************************************************/

#ifndef _MMITHEME_COVERBAR_H_
#define _MMITHEME_COVERBAR_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guifont.h"
#include "mmi_font.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#if 0

/*****************************************************************************/
//  Description : create cover bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CreateCoverBar(
                                      MMI_WIN_ID_T win_id
                                      );

/*****************************************************************************/
//  Description : destroy cover bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DestroyCoverBar(void);


/*****************************************************************************/
//  Description : change coverbar rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_ChangeCoverBarRect(
                                        GUI_RECT_T user_rect
                                        );

/*****************************************************************************/
//  Description : 根据窗口属性判断是否要coverbar效果
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsNeedCoverBar(
                                       MMI_HANDLE_T win_handle
                                       );

#endif

/*****************************************************************************/
//  Description : 在client_rect区域上下部显示蒙板效果
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayCoverBar(
                                     MMI_WIN_ID_T win_id,
                                     const GUI_LCD_DEV_INFO *dev_info_ptr,
                                     GUI_RECT_T client_rect
                                     );


/*****************************************************************************/
//  Description : get image width height
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetCoverBarWidthHeight(
                                            MMI_WIN_ID_T win_id,
                                            uint16 *top_width_ptr, 
                                            uint16 *top_height_ptr,
                                            uint16 *bottom_width_ptr, 
                                            uint16 *bottom_height_ptr
                                            );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

