/*****************************************************************************
** File Name:      audioplayer_demoapp_id.h                                      *
** Author:         zirui.li                                                  *
** Date:           2022/04/25                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe audioplayer app             *
**                   win and control id                                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2022/04/25     zirui.li        Create                                     *
******************************************************************************/

#ifndef _AUDIOPLAYER_DEMOAPP_ID_H_
#define _AUDIOPLAYER_DEMOAPP_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "mmimp3_id.h"

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
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

// control ID
typedef enum
{
    AP_CONTROL_ID_STRAT = MMIMP3_MAX_CTRL_ID,
    AP_SINGER_LABEL_CTRL_ID,
    AP_NAME_LABEL_CTRL_ID,
    AP_PLAY_BUTTON_CTRL_ID,
    AP_PAUSE_BUTTON_CTRL_ID,
    AP_PREV_BUTTON_CTRL_ID,
    AP_NEXT_BUTTON_CTRL_ID,
    AP_PRGBAR_CTRL_ID,
    AP_STOP_BUTTON_CTRL_ID,
    AP_SPEAKER_BUTTON_CTRL_ID,
    AP_FAV_BUTTON_CTRL_ID,
    AP_DL_BUTTON_CTRL_ID,
    AP_LISTIND_BUTTON_CTRL_ID,
    AP_IND_BUTTON_CTRL_ID,
    AP_MAINMENU_CTRL_ID,
    AP_CONTROL_ID_MAX
}AP_CONTROL_ID_E;

#ifdef   __cplusplus
    }
#endif

#endif //_AUDIOPLAYER_DEMOAPP_ID_H_

