/*************************************************************************
 ** File Name:      mmivt_id.c                                          *
 ** Author:         samboo.shen
 ** Date:           2010/02/18                                           *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:                                                         *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     samboo.shen       Create.                              *
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

const uint8 mmivt_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmivt_id.def"    
};

#undef WIN_ID_DEF

#ifdef VT_SUPPORT
/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_VT, mmivt_id_name_arr);/*lint !e64*/
}
#endif
