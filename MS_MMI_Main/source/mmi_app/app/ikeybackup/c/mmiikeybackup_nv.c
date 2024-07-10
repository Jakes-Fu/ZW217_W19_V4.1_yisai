/****************************************************************************
** File Name:      mmiikeybackup_nv.c                                       *
** Author:         haiwu.chen                                               *
** Date:           12/25/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to define the nv about 1-key backup    *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/25/2011     haiwu.chen        Create                                 *
****************************************************************************/
#ifdef IKEYBACKUP_SUPPORT


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmiikeybackup_nv.h"
#include "mmiikeybackup_api.h"
#include "mmi_modu_main.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
/*the length of set nv*/
const uint16 ikeybackup_nv_len[] =
{
    sizeof(MMIIKB_CONFIG_T),         //MMIIKB_CONFIG_T
};



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init 1-keybackup module nv
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_IKEYBACKUP, ikeybackup_nv_len, sizeof(ikeybackup_nv_len) / sizeof(uint16));
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(IKEYBACKUP_SUPPORT)

