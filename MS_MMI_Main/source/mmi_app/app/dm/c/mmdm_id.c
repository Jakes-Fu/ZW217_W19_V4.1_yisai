/*************************************************************************
 ** File Name:      mmidm_id.c                                           *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/02                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/02     jian.ma          Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#if (defined(DM_SUPPORT) || defined(DM_ONLY_SELFREG_SUPPORT))
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmidm_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmidm_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register dm menu group
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_DM, mmidm_id_name_arr); /*lint !e64*/
}
#endif