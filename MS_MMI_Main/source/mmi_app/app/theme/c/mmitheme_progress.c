/****************************************************************************
** File Name:      mmitheme_progress.c                                      *
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
#include "mmitheme_progress.h"

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
// Description : progress 控件用户信息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetProgressStyle(
                                      MMI_PROGRESS_STYLE_T *progress_style_ptr
                                      )
{
    // todo...
    if (PNULL != progress_style_ptr)
    {
        progress_style_ptr->bg_color = 0;
        progress_style_ptr->disable_bg_color = 0;
#ifndef MMI_RES_LOW_COST             
        progress_style_ptr->bg_image_id = IMAGE_CONTROL_PROGRESS_BG;
#else
        progress_style_ptr->bg_image_id = 0;
#endif
        progress_style_ptr->disable_bg_image_id = 0;
        
        progress_style_ptr->fg_color = 0;
        progress_style_ptr->disable_fg_color = 0;
#ifndef MMI_RES_LOW_COST          
        progress_style_ptr->fg_image_id = IMAGE_CONTROL_PROGRESS;
#else
        progress_style_ptr->fg_image_id = 0;
#endif
        progress_style_ptr->disable_fg_image_id = 0;
    }
    
    return;
}
