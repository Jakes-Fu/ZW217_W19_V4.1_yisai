/****************************************************************************
** File Name:      mmitheme_lettersbar.c                                    *
** Author:         nan.ji                                                   *
** Date:           08/13/2012                                               *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 08/2012        nan.ji       Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_app_theme_trc.h"
#include "mmi_theme.h"
#include "mmidisplay_data.h"
#include "mmi_id.h"
#include "mmi_image.h"
#include "guifont.h"
#include "mmitheme_lettersbar.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : letters bar 控件用户信息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetLettersStyle(
                                     MMI_LETTERS_STYLE_T *letters_style_ptr
                                     )
{
    // todo...
    if (PNULL != letters_style_ptr)
    {
        letters_style_ptr->letter_font = CAF_FONT_8;
        letters_style_ptr->letter_font_color = MMI_BACKGROUND_COLOR;
        letters_style_ptr->letter_scale = MMI_LETTER_SCALE;
    }
    
    return;
}

/*****************************************************************************/
// Description : letters bar 控件用户信息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetLettersBg(
                                  GUI_BG_T *letters_bg_ptr
                                  )
{
    // todo...
    if (PNULL != letters_bg_ptr)
    {
#ifdef GUIF_PRGBOX_AUTOHIDE
        letters_bg_ptr->bg_type = GUI_BG_IMG;
        letters_bg_ptr->img_id = IMAGE_SCROLL_VER_BG;
#else
        letters_bg_ptr->bg_type = GUI_BG_NONE;
#endif
    }
    
    return;
}
