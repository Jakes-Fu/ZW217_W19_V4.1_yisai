/*************************************************************************
 ** File Name:      mmidcd_id.c                                          *
 ** Author:         zhuo.wang                                           *
 ** Date:           2008/10/14                                          *
 ** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/10/14     zhuo.wang       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmidcd_id.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmidcd_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmidcd_id.def"    
};

#undef WIN_ID_DEF


/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCD_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_DCD, mmidcd_id_name_arr); /*lint !e64*/
}
