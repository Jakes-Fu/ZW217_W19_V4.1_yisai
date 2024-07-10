/*************************************************************************
 ** File Name:      mmidyna_id.c                                          *
 ** Author:         peng.ch                                           *
 ** Date:           2008/09/24                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/09/24     haiyang.hu       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#ifdef DYNAMIC_MODULE_SUPPORT

#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmidyna_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmidyna_win.def"    
};

#undef WIN_ID_DEF
//extern void MMI_RegWinIdNameArr (uint16 module_id, const uint8** win_id_name_ptr);
/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDYNA_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_DYNA_APPLIST, mmidyna_id_name_arr);
}
#endif


