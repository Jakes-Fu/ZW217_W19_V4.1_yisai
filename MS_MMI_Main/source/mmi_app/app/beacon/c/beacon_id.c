/*************************************************************************
 ** File Name:      mmicc_id.c                                          *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     haiyang.hu       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#ifndef LOW_MEMORY_SUPPORT
#define WIN_ID_DEF(win_id, win_id_name)          win_id_name,

const uint8 mmibeacon_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "beacon_id.def"    
};

#undef WIN_ID_DEF
#endif

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIBEACON_RegWinIdNameArr(void)
{
#ifndef LOW_MEMORY_SUPPORT
    MMI_RegWinIdNameArr(MMI_MODULE_BEACON, mmibeacon_id_name_arr);
#endif
	
}

