/*****************************************************************************
** File Name:      mmistk_export.h                                                  *
** Author:                                                                   *
** Date:           2006/10/08                                               *
** Copyright:      2003 Spreadrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe stk api               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2006      songbin.zeng       Create
******************************************************************************/

#ifndef _MMISTK_H_
#define _MMISTK_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#include "simat_data_object.h"
#include "simat_proactive.h"
#include "simat_api.h"
#include "guiimg.h"

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
/********************************************************************************
 NAME:			MMIAPISTK_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
PUBLIC void MMIAPISTK_AppInit(void);

/********************************************************************************
 NAME:			MMIAPISTK_EnterToplevelMenu
 DESCRIPTION:	enter STK top level menu from main menu
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.08.17
********************************************************************************/
PUBLIC void MMIAPISTK_EnterToplevelMenu(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
 NAME:			MMAPIISTK_SendSMSPDUCnf
 DESCRIPTION:	send SMS Cnf to STK layer
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.01
********************************************************************************/
PUBLIC void MMIAPISTK_SendSMSPDUCnf(MN_DUAL_SYS_E dual_sys, MN_SMS_CAUSE_E send_result);

/********************************************************************************
 NAME:			MMIAPISTK_OnSendRegisterPaswdCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_OnSendRegisterPaswdCnf(void *arg);

/********************************************************************************
 NAME:			OnSendSSCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_OnSendSSCnf(void *arg);

/*****************************************************************************/
// 	Description : info sim card the language type
//	Global resource dependence : 
//   Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_CnfSimLangSel(void);

/*****************************************************************************/
// 	Description : info sim card the idle screen available
//	Global resource dependence : 
//   Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_CnfIdleScrAva(void);

/*****************************************************************************/
// 	Description : info sim card the user active
//	Global resource dependence : 
//   Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_CnfSimUserAct(void);

/********************************************************************************
 NAME:			MMIAPISTK_OnSendUSSDCnf
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		gang.tong
 DATE:			2005.11.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_OnSendUSSDCnf(void *arg);

/********************************************************************************
 NAME:			MMIAPISTK_IsStkCard
 DESCRIPTION:	whether STK card
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.18
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsStkCard(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
 NAME:			MMIAPISTK_IsSendSms
 DESCRIPTION:	whether send SMS by STK
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.11.18
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsSendSms(void);

/*****************************************************************************/
// 	Description : MMIAPISTK_IsStkRefresh
//	Global resource dependence : 
//   Author: wancna.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsStkRefresh(void);

/********************************************************************************
 NAME:      MMISTK_SetupCall
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_SetupCall(MN_DUAL_SYS_E dual_sys, 
                                        BOOLEAN is_oper_cnf, 
                                        BOOLEAN is_call_active,
                                        MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

/*****************************************************************************/
//  Description : Is Setup Call Duration
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_GetSetupCallDuration(void);

/********************************************************************************
 NAME:      MMIAPISTK_OnRelUSSD
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_OnRelUSSD(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
 NAME:      MMIAPISTK_IsInSTK
 DESCRIPTION:
 AUTHOR: wancan.you
 DATE:		
********************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_IsInSTK(void);

/********************************************************************************
 NAME:      MMIAPISTK_BrowserTermination
 DESCRIPTION:
 AUTHOR: kun.yu
 DATE:		
********************************************************************************/
PUBLIC void MMIAPISTK_BrowserTermination(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIAPISTK_CheckCloseDisplayTextIRWin
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISTK_CheckCloseDisplayTextIRWin(void);

/*****************************************************************************/
// 	Description : Get STK card existed status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPISTK_GetStkCardNum(uint16 *idx_ptr, uint16 array_len);

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : Get CMCC STK card existed status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPISTK_GetCMCCStkCardNum(uint16 *idx_ptr, uint16 array_len);
#endif

/*****************************************************************************/
//     Description : Get Stk Name
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISTK_GetStkName(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *stk_name_ptr);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
PUBLIC GUIIMG_BITMAP_T *MMIAPISTK_GetIdleTextIcon(BOOLEAN *is_icon_exist, BOOLEAN *is_self_explanatory);

/*****************************************************************************/
//  Description : Is Focus STK Win
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISTK_CloseDisplayTextWin(void);

/*****************************************************************************/
//  Description : Select SIM API of STK app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISTK_SelectSIMFunc(void);

/*****************************************************************************/
//  Description : get type of STK refresh
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC SIMAT_REFRESH_TYPE_E MMIAPISTK_GetStkRefreshType(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set type of STK refresh
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISTK_SetStkRefreshType(MN_DUAL_SYS_E dual_sys, int value);

/*****************************************************************************/
//  Description : close refresh waiting win
//  Global resource dependence : 
//  Author:bingjie.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISTK_CloseRefreshWaitingWin(void);
/*****************************************************************************/
//  Description : Get Call Param For Set Up Cal
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:s_mmistk_setup_call_cpp
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/

PUBLIC void MMISTK_GetCallCppParamForSetUpCall(MN_DUAL_SYS_E  dual_sys, SIMAT_DO_CCP_T *ccp_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
