/*************************************************************************
 ** File Name:      mmiwiif_id.c                                         *
 ** Author:         li.li                                                *
 ** Date:           2009/11/18                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about wifi           *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/11/18     li.li            Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_module.h"
#include "mmi_modu_main.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmiwifi_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiwlan_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register wifi win id
//	Global resource dependence : none
//  Author: li.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIWIFI_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_WIFI, mmiwifi_id_name_arr);/*lint !e64*/
}