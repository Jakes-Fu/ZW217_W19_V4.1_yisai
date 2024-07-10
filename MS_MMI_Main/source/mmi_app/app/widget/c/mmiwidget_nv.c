/*************************************************************************
** File Name:      mmiwidget_nv.c                                          *
** Author:         bin.ji                                         *
** Date:           2010/04/21                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2010/04/21     bin.ji     Create.                              *
*************************************************************************/


#define _MMIWIDGET_NV_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
//#include "sci_types.h"
#include "mmiset_nv.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget.h"
#endif
#include "mmi_modu_main.h"
#include "mmiwidget_nv.h"
/*the length of set nv*/
LOCAL const uint16 mmiwidget_nv_len[] =
{
    sizeof(MMIWIDGETPB_FAVORITE_ENTRY_T), //PB_FAVORITE_LIST
#ifdef MMIWIDGET_SUPPORT
    sizeof(MMIWIDGET_SAVE_INFO_T),      				// MMIWIDGET_SAVE_INFO  
#endif    
#ifdef MMI_GRID_IDLE_SUPPORT
    sizeof(MMI_GRID_SAVE_INFO_T),
    sizeof(MMIGRID_PB_FAVORITE_ENTRY_T),
    sizeof(MMIGRID_IMGVIEWWER_FOLDER_T),
#endif    
#ifdef WEATHER_SUPPORT
    sizeof(uint32),                       //MMIWIDGET_WEATHER_HIT_SET_INFO
    sizeof(uint32),                       //MMIWIDGET_WEATHER_BG_TEXTCOLOUR_SET_INFO
#endif
#ifdef SCH_ASSISTANT_WIDGET
    sizeof(BOOLEAN),//MMIWIDGET_ASSISTANT_NV_INFO
#endif
#ifdef QBTHEME_SUPPORT
    sizeof(MMI_GRID_SAVE_INFO_T),
#endif
#ifdef MMI_SAM_THEME
    sizeof(MMI_GRID_SAVE_INFO_T),
#endif 
#ifdef MMIWIDGET_ACLOCK_SE
    sizeof(int32),
#endif
};

/*****************************************************************************/
// 	Description : register widget module nv len and max item
//	Global resource dependence : none
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_WIDGET, mmiwidget_nv_len, sizeof(mmiwidget_nv_len) / sizeof(uint16));
}

