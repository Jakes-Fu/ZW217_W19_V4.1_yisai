/*************************************************************************
 ** File Name:      watch_notifycenter_id.c                              *
 ** Author:                                                              *
 ** Date:           2021/01/12                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about winid          *
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

static const uint8 s_notifycenter_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "watch_notifycenter_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//  Description : Register notifycenter win id
//  Global resource dependence : none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_NotifyCenter_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(WATCH_MODULE_NOTIFYCENTER, s_notifycenter_id_name_arr);        /*lint !e64*/
}
