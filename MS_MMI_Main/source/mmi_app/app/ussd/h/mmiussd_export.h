/*****************************************************************************
** File Name:      mmiussd_export.h                                                   *
** Author:                                                                   *
** Date:           2004/11/19                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2004/11/19       bin.ji       Create
******************************************************************************/

#ifndef _MMIUSSD_EXPORT_H_
#define _MMIUSSD_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "mmk_type.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :to handle ussd service confirm
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
MMI_RESULT_E MMIAPIUSSD_HandleUssdServiceCnf(
                                          DPARAM param
                                          );

/*****************************************************************************/
//  Description :to parse ussd string
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
BOOLEAN MMIAPIUSSD_ParseUssdString(
                                MN_DUAL_SYS_E dual_sys,//in, 标示使用哪张sim卡拨出
                                uint8*  str_ptr,
                                uint8	str_len
                                );

/*****************************************************************************/
//  Description :to handle ussd ind
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
MMI_RESULT_E MMIAPIUSSD_HandleUssdInd(
                                   DPARAM param
                                   );

/*****************************************************************************/
//  Description :to handle ussd notify ind
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
MMI_RESULT_E MMIAPIUSSD_HandleUssdNotifyInd(
                                         DPARAM param
                                         );
                                         
/*****************************************************************************/
//  Description :to close ussd sending win
//  Global resource dependence : none
//  Author:bin.ji
//  Note: 2004.11.19
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_CloseSendWin(void);

/*****************************************************************************/
//  Description :to close ussd input string win
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_CloseInputWin(void);

/*****************************************************************************/
//  Description :is open show data win
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_CloseShowDataWin(void);

/*****************************************************************************/
//  Description : get which sim to send ussd
//  Global resource dependence : none
//  Author:bown.zhang
//  Note: 2007.9.6
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIUSSD_GetDualSys(void);

/*****************************************************************************/
//Description : Get USSD SIM(Not In USSD return MMI_DUAL_SYS_MAX)
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIUSSD_GetUssdSIM(void);

/*****************************************************************************/
//  Description :Grab to Focus
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_WinGrabFocus(void);

/*****************************************************************************/
//  Description :Is Send Win Open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUSSD_IsSendWinOpen(void);

/*****************************************************************************/
//  Description :Set playring flage
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUSSD_SetPlayringFlage(BOOLEAN isNeedRing);

/*****************************************************************************/
//  Description :Get playring flage
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUSSD_GetPlayringFlage(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
