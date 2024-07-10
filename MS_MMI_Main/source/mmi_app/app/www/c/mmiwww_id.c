/*************************************************************************
** File Name:      mmiwww_id.c                                          *
** Author:         bin.ji                                           *
** Date:           2010/05/10                                           *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2010/05/10    bin.ji       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmiwww_id.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmiwww_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiwww_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//     Description : Register www win id name
//    Global resource dependence : none
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMIWWW_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_WWW, (const uint8 **)mmiwww_id_name_arr);
}