/*************************************************************************
 ** File Name:      tp_nv.c                                             *
 ** Author:         wancan.you                                           *
 ** Date:           11/21/2005                                           *
 ** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 11/21/2005     wancan.you        Create.                              *
 ** 01/05/2010     vine.yuan        modify tp_nv_len for TP self-adaptive*
*************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#if defined TOUCH_PANEL_SUPPORT
//#include "sci_types.h"
#include "tp_api.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"

/*the length of tp nv*/
const uint16 tp_nv_len[] =
{
    //for touch panel
    sizeof(TP_CALIBRATION_T),
    sizeof(TP_CALIBRATION_T),
    /*
    * for TP's self-adaptive mode
    * @vine.yuan 2010.1.5
    */
#ifdef TP_SELFADAPTIVE_ENABLE
    sizeof(TP_MODE_E),
#endif
    1
};

/*****************************************************************************/
// 	Description : register tp module nv len and max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void TP_RegTPNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_TP, tp_nv_len, sizeof(tp_nv_len) / sizeof(uint16));
}

#endif //#if defined TOUCH_PANEL_SUPPORT


