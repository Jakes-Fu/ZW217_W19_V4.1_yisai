/*****************************************************************************
** File Name:      set_ss_flow.h                                             *
** Author:         xiaoju.cheng                                              *
** Date:           04/26/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2021      xiaoju.cheng              Creat                              *
******************************************************************************/

#ifndef _SET_SS_FLOW_H_
#define _SET_SS_FLOW_H_

#include "ual_tele.h"
#include "mmiset_call_export.h"

typedef enum
{
    SETTINGS_SS_POWERON_PS_NONE,
    SETTINGS_SS_POWERON_PS_INTERROGATE,
    SETTINGS_SS_POWERON_PS_OPERATION
}SETTINGS_SS_POWERON_PS_TYPE_E;

/*****************************************************************************/
//  Description : Send stand ss request to MN
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
PUBLIC ual_ss_error_e Settings_SS_SendRequest(
                          ual_sim_number_e           dual_sys,
                          ual_ss_operation_code_e    ss_oper,
                          MMISET_SS_PARSE_INFO_T    *ss_result_ptr,
                          MMISET_SS_INFO_T          *ss_info_ptr,
                          uint8 *input_str,
                          uint8 str_len,
                          _ual_cms_client_register_callback  start_ss_callback
                          );

/*****************************************************************************/
//   Description : start call forwarding
//	Global resource dependence :
//  Author: xiaoju.cheng
//	Note:
/*****************************************************************************/
PUBLIC ual_ss_error_e Settings_SS_Start_Request(
                                       ual_sim_number_e     dual_sys,
                                       ual_ss_code_e		ss_code,
                                       ual_ss_operation_code_e   ss_oper,
                                       uint8                retry_times,
                                       MMI_STRING_T         *voice_number_ptr,
                                        _ual_cms_client_register_callback  start_ss_callback
                                       );

/*****************************************************************************/
//  Description : register dual sim call forward
//	Global resource dependence :
//  Author:xiaoju.cheng
//	Note:
/*****************************************************************************/
PUBLIC ual_ss_error_e Settings_SS_RegisterDualsysCFNRC(ual_sim_number_e dual_sys, _ual_cms_client_register_callback  start_ss_callback);

/*****************************************************************************/
//  Discription: get if SS power on ps type                                  *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC SETTINGS_SS_POWERON_PS_TYPE_E Settings_SS_Get_PowerOn_PS_Type( void );
/*****************************************************************************/
//  Discription: set if ss need power on ps                                  *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_SS_Set_PowerOn_PS_Type( SETTINGS_SS_POWERON_PS_TYPE_E poweron_ps_type );

#endif
