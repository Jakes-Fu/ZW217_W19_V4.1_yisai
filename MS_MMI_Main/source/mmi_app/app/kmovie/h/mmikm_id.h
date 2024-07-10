/***************************************************************************
** File Name:      mmikm_id.h                                            *
** Author:                                                                 *
** Date:           07/14/2011                                            *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011         tonny.chen          Create                               *
**																		   *
****************************************************************************/
#ifndef _MMIKM_ID_H
#define _MMIKM_ID_H

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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

#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
    MMIKM_WIN_ID_START = (MMI_MODULE_KMOVIE << 16),

#include "mmikm_id.def"

    MMIKM_MAX_WIN_ID
}MMIKM_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIKM_CTRL_ID_START = MMIKM_MAX_WIN_ID,
    MMIKM_MAIN_WIN_MENU_CTRL_ID,
    MMIKM_MAIN_WIN_POPMENU_CTRL_ID,
    MMIKM_DIS_TYPE_INFOR_LIST_CTRL_ID,
    MMIKM_DISP_TYPE_WIN_POPMENU_CTRL_ID,
    MMIKM_DISP_MORE_INFOR_CTRL_ID,
    MMIKM_DISP_THUMBNAIL_FILENAME_CTRL_ID,
    MMIKM_DISP_THUMBNAIL_CTRL_ID,
    MMIKM_DISP_FILENAME_CTRL_ID,
    MMIKM_DISP_DETAIL_INFOR_CTRL_ID,
    MMIKM_DISP_FILENAME_EXT_INFOR_CTRL_ID,
    MMIKM_DISP_TIME_INFOR_CTRL_ID,
    MMIKM_DISP_YEAR_INFOR_CTRL_ID,
    MMIKM_DISP_ACTOR_INFOR_CTRL_ID,
    MMIKM_DISP_SYNOP_INFOR_CTRL_ID, 
    MMIKM_DISP_TYPE_PDA_POPMENU_CTRL_ID,
	MMIKM_FORM_CTRL_ID,
}MMIKM_CTRL_ID_E;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/


#ifdef _cplusplus
	}
#endif

#endif//_MMIKM_ID_H

