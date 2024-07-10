/*****************************************************************************
** File Name:      mmiwidget_display.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#define _MMIWIDGET_DISPLAY_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)

#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmi_nv.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_image.h"
#include "mmiwidget_menutable.h"
//#include "guires.h"
#include "mmi_modu_main.h"
#include "mmi_id.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"

/*****************************************************************************/
//  FUNCTION:      MMIAPIWIDGET_InitModule
//  Description :
//  Global resource dependence : 
//  Author:        bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_InitModule(void)
{/*lint !e18*/
//    MMIRECORD_RegMenuGroup();  
    MMIWIDGET_RegMenuGroup();
    MMIWIDGET_RegNv();  
    MMIWIDGET_RegWinIdNameArr();
}

#endif


