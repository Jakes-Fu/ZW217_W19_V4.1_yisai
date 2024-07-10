/******************************************************************************
** File Name:      mmitheme_progress.h                                        *
** Author:         nan.ji                                                     *
** Date:           08/13/2012                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the data struct of           *
**                              progress control                                *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 08/13/2012    nan.ji             Create			                          *
******************************************************************************/

#ifndef _MMITHEME_PROGRESS_H_
#define _MMITHEME_PROGRESS_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
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


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

// MMI_PROGRESS_STYLE_T
typedef struct
{
    GUI_COLOR_T     bg_color;
    GUI_COLOR_T     disable_bg_color;
    MMI_IMAGE_ID_T  bg_image_id;
    MMI_IMAGE_ID_T  disable_bg_image_id;

    GUI_COLOR_T     fg_color;
    GUI_COLOR_T     disable_fg_color;
    MMI_IMAGE_ID_T  fg_image_id;
    MMI_IMAGE_ID_T  disable_fg_image_id;

}MMI_PROGRESS_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : progress 控件用户信息
// Global resource dependence : 
// Author: nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetProgressStyle(
                                    MMI_PROGRESS_STYLE_T *progress_style_ptr
                                    );


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_PROGRESS_H_
/*! @} */
