/*************************************************************************
** File Name:      mmijava_id.c                                          *
** Author:         yihg.xu                                           *
** Date:           2011.01.25                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2011.01.25      ying.xu          Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmijava_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "mmijava_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: yihg.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_JAVA, mmijava_id_name_arr);  /*lint !e64 */
}

/*****************************************************************************/
// 	Description : get sun java module id
//	Global resource dependence : none
//  Author: yihg.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_GetModuleID(void)
{
   return MMI_MODULE_JAVA;
}