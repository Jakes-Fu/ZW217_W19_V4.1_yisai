/*************************************************************************
 ** File Name:      mmipic_id.c                                          *
 ** Author:           vincent.shang                                           *
 ** Date:           2008/01/24                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2008/01/24     vincent.shang       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef PIC_EDITOR_SUPPORT
//#include "sci_types.h"
#include "mmi_module.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmipic_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmipic_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
//warning
//PUBLIC void MMIPIC_RegWinIdNameArr(void)
//{
//    MMI_RegWinIdNameArr(MMI_MODULE_PIC, mmipic_id_name_arr);
//}
#endif
