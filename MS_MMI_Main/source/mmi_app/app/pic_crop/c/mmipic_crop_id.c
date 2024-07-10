/*************************************************************************
 ** File Name:      mmipic_crop_id.c                                          *
 ** Author:           chunyou                                           *
 ** Date:             12/25/2011                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about id             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 12/2011       chunyou         Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef MMI_PIC_CROP_SUPPORT
#include "mmi_module.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmipic_crop_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmipic_crop_id.def"    
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
