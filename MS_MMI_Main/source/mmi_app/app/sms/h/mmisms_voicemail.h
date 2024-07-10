/*************************************************************************
 ** File Name:      mmisms_receive.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about receiving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/9/28      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMS_VOICEMAIL_H_
#define _MMISMS_VOICEMAIL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmisms_export.h"
#include "mmisms_app.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
// @baokun fix cr88995 begin
/*****************************************************************************/
//     Description : 
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetVMAddr( 
                                 MN_DUAL_SYS_E dual_sys,
                                 MMISMS_VMN_RECORD_ID_E vmn_type
                                 );
/*****************************************************************************/
//     Description : to handle the singal APP_MN_SET_VOIMESS_ADDR_CNF
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSetVoiMessAddrCnf( 
                                 DPARAM param
                                 );

/*****************************************************************************/
//     Description : to handle the singal APP_MN_GET_VOIMESS_ADDR_CNF
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleGetVoiMessAddrCnf( 
                                 DPARAM param
                                 );



/*****************************************************************************/
//     Description : to handle the singal APP_MN_GET_VOIMESS_FLAG_CNF
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleGetVoiMessFlagCnf( 
                                 DPARAM param
                                 );


/*****************************************************************************/
//     Description : to handle the singal APP_MN_VMW_FALG_IND
//    Global resource dependence : none
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleNewVMFlagInd( 
                                 DPARAM param
                                 );


/*****************************************************************************/
//     Description : to get the voice mail
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
// modify by kelly.li  for number of voice mail has increases to 4, add "'
PUBLIC BOOLEAN  MMISMS_GetVoiceMailAddr( 
                                        MN_DUAL_SYS_E   dual_sys,
                                        uint8            record_id,
                                        uint8            *voice_mail_ptr, 
                                        uint8            *mail_len_ptr,
                                        uint8            max_mail_len
                                        );

/*****************************************************************************/
//     Description : to open the window to call the voice mail
//    Global resource dependence : 
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_OpenCallVMWin(MN_DUAL_SYS_E dual_sys, MMISMS_VMN_RECORD_ID_E vmn_type);

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMI_SMS_VMN_T MMISMS_GetCurVmInfo(void);

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetCurVmInfo(MMI_SMS_VMN_T* cur_vmnum_info);

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitCurVmInfo(void);

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetVmOperType(MMISMS_VMN_OPER_TYPE_E type);

/*****************************************************************************/
//     Description : to set the voice mail cur num info
//    Global resource dependence : none
//  Author:rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_VMN_OPER_TYPE_E MMISMS_GetVmOperType(void);

/*****************************************************************************/
//     Description : Set Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_UpdateVMFlagState(void);

/*****************************************************************************/
//     Description : Set Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetVMInfo(MN_DUAL_SYS_E dual_sys, MN_SMS_VMW_IND_FLAG_T *vmw_flag);

/*****************************************************************************/
//     Description : Reset Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ResetVMInfo(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : Init Voice Mail Info
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitVMInfo(void);

#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL
PUBLIC void SaveVMAddr2SimByType(MMI_SMS_VMN_T* cur_vmnum_info,MN_DUAL_SYS_E dual_sys,MMI_STRING_T* num_string, MMISMS_VMN_RECORD_ID_E vmn_type);
#endif
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_VOICEMAIL_H_












