/*************************************************************************
 ** File Name:      mmidropdownwin_id.c                                  *
 ** Author:                                                              *
 ** Date:           2011/07/03                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2011/07/03     paul.huang       Create.                              *
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

static const uint8 s_mmidropdownwin_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmidropdownwin_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//  Description : Register dropdown win id
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDROPDOWNWIN_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_DROPDOWNWIN, s_mmidropdownwin_id_name_arr);        /*lint !e64*/
}
