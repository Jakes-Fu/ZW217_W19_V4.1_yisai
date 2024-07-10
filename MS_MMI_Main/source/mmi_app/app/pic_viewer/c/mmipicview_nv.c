/*************************************************************************
 ** File Name:      mmipicview_nv.c                                      *
 ** Author:         jassmine                                             *
 ** Date:           2008/12/19                                           *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/12/19    Jassmine           Create.                             *
*************************************************************************/

#define MMIPICVIEW_NV_C

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32 
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "mmipicview_export.h"
#include "mmipicview_internal.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//the length of pic viewer nv
LOCAL const uint16 picview_nv_len[] =
{
    sizeof(MMIPICVIEW_EFFECT_TYPE_E),   //MMINV_PICVIEW_SLIDE_EFFECT
    sizeof(MMIPICVIEW_TIME_TYPE_E),     //MMINV_PICVIEW_SLIDE_TIME
    sizeof(MMIPICVIEW_VIEW_TYPE_E),     //MMINV_PICVIEW_VIEW_TYPE
    sizeof(MMIPICVIEW_DEFAULT_PATH_T)
};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register pic viewer module nv len and max item
//	Global resource dependence : none
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_PIC_VIEWER,picview_nv_len,sizeof(picview_nv_len)/sizeof(uint16));
}
#endif //#ifdef PIC_VIEWER_SUPPORT