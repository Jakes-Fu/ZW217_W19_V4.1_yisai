/*************************************************************************
 ** File Name:      watch_sensor_id.c                                    *
 ** Author:         longwei.qiao                                         *
 ** Date:           2021/04/15                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about winid          *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2021/04/15     longwei.qiao       Create.                            *
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

static const uint8 s_sensor_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "watch_sensor_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
//  Description : Register notifycenter win id
//  Global resource dependence : none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WATCH_Sensor_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(WATCH_MODULE_SENSOR, (const uint8 **)s_sensor_id_name_arr);        /*lint !e64*/
}
