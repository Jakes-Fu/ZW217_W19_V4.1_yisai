/*****************************************************************************
** File Name:      guiedit.c                                                 *
** Author:         hua.fang                                                  *
** Date:           08/20/2012                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmk_type.h"
#include "guiedit.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : is picker
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPickerCtrl(
    MMI_HANDLE_T    ctrl_handle
    )
{
    BOOLEAN     is_picker = FALSE;
    IGUICTRL_T  *iguictrl_ptr = MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != iguictrl_ptr)
    {
        switch (GUICTRL_GetCtrlGuid(iguictrl_ptr))
        {
        case SPRD_GUI_PICKER_ID:
        case SPRD_GUI_TIMEPICKER_ID:
        case SPRD_GUI_DATEPICKER_ID:
            is_picker = TRUE;
            break;
    
        default:
            break;
        }
    }

    return is_picker;
}

/*****************************************************************************/
//  Description : get date
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_GetDate(
                            MMI_CTRL_ID_T   ctrl_id,    //in
                            uint16          *year_ptr,  //in/out:may PNULL
                            uint8           *month_ptr, //in/out:may PNULL
                            uint8           *day_ptr    //in/out:may PNULL
                            )
{
    if (IsPickerCtrl(ctrl_id))
    {
        CTRLDATEPICKER_GetDate(ctrl_id, year_ptr, month_ptr, day_ptr);
    }
    else
    {
        CTRLDATEEDIT_GetDate(ctrl_id, year_ptr, month_ptr, day_ptr);
    }
}

/*****************************************************************************/
//  Description : set date
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetDate(
                            MMI_CTRL_ID_T   ctrl_id,
                            uint16          year,
                            uint8           month,
                            uint8           day
                            )
{
    if (IsPickerCtrl(ctrl_id))
    {
        CTRLDATEPICKER_SetDate(ctrl_id, year, month, day);
    }
    else
    {
        CTRLDATEEDIT_SetDate(ctrl_id, year, month, day);
    }
}

/*****************************************************************************/
//  Description : set date style and separator
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetDateStyle(
                                 MMI_CTRL_ID_T          ctrl_id,
                                 GUIEDIT_SEPARATOR_E    *separator_ptr, //may PNULL
                                 GUIEDIT_DATE_STYLE_E   *date_style_ptr,//may PNULL
                                 BOOLEAN                is_update
                                 )
{
    if (IsPickerCtrl(ctrl_id))
    {
        CTRLDATEPICKER_SetDateStyle(ctrl_id, date_style_ptr, is_update);
    }
    else
    {
        CTRLDATEEDIT_SetDateStyle(ctrl_id, separator_ptr, date_style_ptr, is_update);
    }
}

/*****************************************************************************/
//  Description : get date range
//  Global resource dependence : 
//  Author: Jassmine
//  Note:  
/*****************************************************************************/
PUBLIC GUIEDIT_DATE_YEAR_RANGE_E GUIEDIT_GetDateRange(
                                                      MMI_CTRL_ID_T     ctrl_id
                                                      )
{
    if (IsPickerCtrl(ctrl_id))
    {
        return CTRLDATEPICKER_GetDateRange(ctrl_id);
    }
    else
    {
        return CTRLDATEEDIT_GetDateRange(ctrl_id);
    }
}

/*****************************************************************************/
//  Description : set date range
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default is GUIEDIT_DATE_YEAR_RANGE_SET 
/*****************************************************************************/
PUBLIC void GUIEDIT_SetDateRange(
                                 MMI_CTRL_ID_T              ctrl_id,
                                 GUIEDIT_DATE_YEAR_RANGE_E  year_range_type
                                 )
{
    if (IsPickerCtrl(ctrl_id))
    {
        CTRLDATEPICKER_SetDateRange(ctrl_id, year_range_type);
    }
    else
    {
        CTRLDATEEDIT_SetDateRange(ctrl_id, year_range_type);
    }
}

/*****************************************************************************/
//  Description : get time
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_GetTime(
                            MMI_CTRL_ID_T   ctrl_id,    //in
                            uint8           *hour_ptr,  //in/out:may PNULL
                            uint8           *minute_ptr,//in/out:may PNULL
                            uint8           *second_ptr //in/out:may PNULL
                            )
{
    if (IsPickerCtrl(ctrl_id))
    {
        CTRLTIMEPICKER_GetTime(ctrl_id, hour_ptr, minute_ptr, second_ptr);
    }
    else
    {
        CTRLTIMEEDIT_GetTime(ctrl_id, hour_ptr, minute_ptr, second_ptr);
    }
}

/*****************************************************************************/
//  Description : set time
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetTime(
                            MMI_CTRL_ID_T   ctrl_id,
                            uint8           hour,
                            uint8           minute,
                            uint8           second
                            )
{
    if (IsPickerCtrl(ctrl_id))
    {
        CTRLTIMEPICKER_SetTime(ctrl_id, hour, minute, second);
    }
    else
    {
        CTRLTIMEEDIT_SetTime(ctrl_id, hour, minute, second);
    }
}

/*****************************************************************************/
//  Description : set time style and display style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetTimeStyle(
                                 MMI_CTRL_ID_T                  ctrl_id,
                                 GUIEDIT_SEPARATOR_E            *separator_ptr,     //may PNULL
                                 GUIEDIT_TIME_STYLE_E           *time_style_ptr,    //may PNULL
                                 GUIEDIT_TIME_DISPLAY_STYLE_E   *display_style_ptr, //may PNULL
                                 BOOLEAN                        is_update
                                 )
{
    if (IsPickerCtrl(ctrl_id))
    {
        CTRLTIMEPICKER_SetTimeStyle(ctrl_id, time_style_ptr, display_style_ptr, is_update);
    }
    else
    {
        CTRLTIMEEDIT_SetTimeStyle(ctrl_id, separator_ptr, time_style_ptr, display_style_ptr, is_update);
    }
}

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetFont(
                            MMI_CTRL_ID_T   ctrl_id,//in
                            GUI_FONT_T      font    //in
                            )
{
    if (IsPickerCtrl(ctrl_id))
    {
    }
    else
    {
        CTRLBASEEDIT_SetFont(ctrl_id, &font, PNULL);
    }
}

/*****************************************************************************/
//  Description : set edit font color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetFontColor(
                                 MMI_CTRL_ID_T  ctrl_id,    //in
                                 GUI_COLOR_T    font_color  //in
                                 )
{
    if (IsPickerCtrl(ctrl_id))
    {
    }
    else
    {
        CTRLBASEEDIT_SetFont(ctrl_id, PNULL, &font_color);
    }
}

/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence : 
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void GUIEDIT_SetDispImIcon(
                                  MMI_CTRL_ID_T     ctrl_id,                //in:
                                  BOOLEAN           *is_disp_im_icon_ptr,   //in:
                                  BOOLEAN           *is_disp_num_info_ptr   //in:
                                  )
{
    //set display im icon
    if (PNULL != is_disp_im_icon_ptr)
    {
        CTRLBASEEDIT_SetDispImIcon(ctrl_id, *is_disp_im_icon_ptr);
    }

    //set display number info
    if (PNULL != is_disp_num_info_ptr)
    {
        CTRLBASEFLEX_SetDispLeftNum(ctrl_id, *is_disp_num_info_ptr);
    }
}

