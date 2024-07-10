/*************************************************************************
 ** File Name:      mmiwidget_nv.h                                          *
 ** Author:         bin.ji                                        *
 ** Date:           2010/04/21                                          *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2010/04/21     bin.ji        Create.                              *
*************************************************************************/

#ifndef _MMIWIDGET_NV_H_
#define _MMIWIDGET_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
	MMIWIDGETPB_FAVORITE_ENTRY = MMI_MODULE_WIDGET<< 16,
#ifdef MMIWIDGET_SUPPORT	
    MMIWIDGET_SAVE_INFO,
#endif
#ifdef MMI_GRID_IDLE_SUPPORT
    MMI_GRID_SAVE_INFO,
    MMI_GRID_PB_ENTRY,
    MMI_GRID_IMGVIEWER_FOLDER,
#endif    
#ifdef WEATHER_SUPPORT
    MMIWIDGET_WEATHER_HIT_SET_INFO,
    MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_INFO,
#endif
#ifdef SCH_ASSISTANT_WIDGET
    MMIWIDGET_ASSISTANT_NV_INFO,
#endif

#ifdef QBTHEME_SUPPORT
    MMI_QBTHEME_IDLE_SAVE_INFO,
#endif
#ifdef MMI_SAM_THEME
    MMI_SAM_SAVE_INFO,
#endif 
#ifdef MMIWIDGET_ACLOCK_SE
    MMI_WIDGET_WATCH_ANGLE,
#endif
    MMIWIDGET_MAX_NV_ITEM_NUM
}MMIWIDGET_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register widget module nv len and max item
//	Global resource dependence : none
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIWIDGET_NV_H_


