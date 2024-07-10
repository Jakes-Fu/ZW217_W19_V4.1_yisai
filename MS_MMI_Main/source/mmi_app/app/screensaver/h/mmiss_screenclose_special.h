/*****************************************************************************
** File Name:      mmiss_screenclose_special.h                               *
** Author:                                                                   *
** Date:           08/23/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe special screenclose         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/23/2011     yaye.jiang             Creat
*****************************************************************************/
#ifndef _MMI_SCREENCLOSE_SPECIAL_H_
#define _MMI_SCREENCLOSE_SPECIAL_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
//#include "sci_types.h"
#include "mmifmm_export.h"
#include "guiimg.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef __cplusplus
     extern   "C"
     {
#endif
#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
	SPE_CRT,		//类“三星”灭屏处理
	
	SPE_MAX
}MMI_CLOSE_SPECIAL_TYPE_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : public interface for screen_close_spec
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC void MMI_CreatScreenCloseWin(void);

/*****************************************************************************/
//  Description : close the display window and sleep
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMI_CloseCRTDispWin(MMI_WIN_ID_T win_id);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
                                 
#endif

#endif
