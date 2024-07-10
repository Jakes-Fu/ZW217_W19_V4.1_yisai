/****************************************************************************
** File Name:      mmitheme_slider.c                                        *
** Author:         Xiaoqing.lu                                              *
** Date:           08/13/2012                                               *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 08/2012        Xiaoqing.lu       Create
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
#include "mmitheme_slider.h"

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
// Description : Slider 控件用户信息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetSliderStyle(
                                    MMI_SLIDER_STYLE_T *slider_style_ptr
                                    )
{
    // todo...
    if (PNULL != slider_style_ptr)
    {
        slider_style_ptr->is_has_bg = FALSE;

        slider_style_ptr->channel_bg_color = 0;
        slider_style_ptr->channel_disable_bg_color = 0;
#ifndef MMI_RES_LOW_COST           
        slider_style_ptr->channel_bg_image_id = IMAGE_CONTROL_PROGRESS_BG;
#else
        slider_style_ptr->channel_bg_image_id = 0;
#endif
        slider_style_ptr->channel_disable_bg_image_id = 0;
        
        slider_style_ptr->is_disp_fg = FALSE;
        slider_style_ptr->channel_fg_color = 0;
        slider_style_ptr->channel_disable_fg_color = 0;
#ifndef MMI_RES_LOW_COST        
        slider_style_ptr->channel_fg_image_id = IMAGE_CONTROL_PROGRESS;
#else
        slider_style_ptr->channel_fg_image_id = 0;
#endif
        slider_style_ptr->channel_disable_fg_image_id = 0;
        
        slider_style_ptr->is_has_thumb = FALSE;
        slider_style_ptr->thumb_color = 0;
        slider_style_ptr->disable_thumb_color = 0;
        slider_style_ptr->thumb_image_id = 0;
        slider_style_ptr->disable_thumb_image_id = 0;
        // todo...
    }

    return;
}
