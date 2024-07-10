/****************************************************************************
** File Name:      mmiasp_id.c                                              *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the ANSWER PHONE           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmiasp_id.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 asp_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiasp_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//     Description : Register acc menu group
//    Global resource dependence : none
//  Author: byte.guo
//    Note:
/*****************************************************************************/
PUBLIC void MMIASP_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_ASP, asp_id_name_arr); /*lint !e64*/
}

