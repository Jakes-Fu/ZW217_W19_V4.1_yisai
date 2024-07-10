/*************************************************************************
** File Name:      ota_id.c                                          *
** Author:         minghu.mao                                           *
** Date:           2009/08/10                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2009/08/10    minghu.mao       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmiota_id.h"

#ifdef MMI_OTA_SUPPORT
#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmiota_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiota_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//     Description : Register acc menu group
//    Global resource dependence : none
//  Author: haiyang.hu
//    Note:
/*****************************************************************************/
PUBLIC void MMIOTA_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_OTA, (const uint8 **)mmiota_id_name_arr);
}

#endif
