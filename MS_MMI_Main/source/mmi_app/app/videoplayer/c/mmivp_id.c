/*************************************************************************
 ** File Name:      mmivp_id.c                                          *
 ** Author:         apple.zhang                                           *
 ** Date:           2010/03/26                                          *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2010/03/26     apple.zhang      Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmivp_id.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmivp_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmivp_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//  Description : Register video player menu group
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIVP_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_VP, mmivp_id_name_arr);    /*lint !e64*/
}


