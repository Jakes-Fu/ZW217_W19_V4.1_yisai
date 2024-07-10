/****************************************************************************
** File Name:      mmidrm_nv.h                                              *
** Author:         haiwu.chen                                               *
** Date:           03/31/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to define the nv about drm             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/31/2011     haiwu.chen        Create                                 *
****************************************************************************/
#ifdef DRM_SUPPORT

#define _MMIDRM_NV_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmidrm_nv.h"
#include "mmidrm_export.h"
#include "mmi_modu_main.h"


/*the length of set nv*/
const uint16 drm_nv_len[] =
{
    sizeof(MMIDRM_NITZ_TYPE_T),         //MMIDRM_NITZ_TYPE_T
};

/*****************************************************************************/
//  Description : init drm module nv
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIDRM_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_DRM, drm_nv_len, sizeof(drm_nv_len) / sizeof(uint16));
}

#endif

