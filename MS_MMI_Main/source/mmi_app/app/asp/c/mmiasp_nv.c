/****************************************************************************
** File Name:      mmiasp_nv.c                                              *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the ANSWER PHONE           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiasp_cortrol_layer.h"
#define _MMIASP_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

//#include "sci_types.h"
#include "mmiasp_nv.h"

/*the length of ST nv*/
const uint16 asp_nv_len[] =
{
	sizeof(MMIASP_NV_SETTING_T),
};

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMIASP_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_ASP, asp_nv_len, sizeof(asp_nv_len) / sizeof(uint16));
}
