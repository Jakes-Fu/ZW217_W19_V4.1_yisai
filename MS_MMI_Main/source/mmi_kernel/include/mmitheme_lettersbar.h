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

#ifndef _MMITHEME_LETTERSBAR_H_
#define _MMITHEME_LETTERSBAR_H_ 

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
#define MMI_LETTER_TIP_HIDE_TICK    2000
#define MMI_LETTER_TIP_FONT_COLOR   0xef7d
#define MMI_LETTER_FONT             CAF_FONT_8
#define MMI_LETTER_SCALE            3

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

// MMI_PROGRESS_STYLE_T
typedef struct
{
    GUI_FONT_T          letter_font;
    GUI_FONT_T          letter_font_color;

    uint16              letter_scale;

}MMI_LETTERS_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : progress 控件用户信息
// Global resource dependence : 
// Author: nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetLettersStyle(
                                     MMI_LETTERS_STYLE_T *letters_style_ptr
                                     );

/*****************************************************************************/
// Description : letters bar 控件用户信息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetLettersBg(
                                  GUI_BG_T *letters_bg_ptr
                                  );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_LETTERSBAR_H_
/*! @} */
