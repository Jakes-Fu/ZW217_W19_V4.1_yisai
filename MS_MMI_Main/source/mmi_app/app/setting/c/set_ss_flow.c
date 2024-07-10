/*****************************************************************************
** File Name:      set_ss_flow.c                                             *
** Author:                                                                   *
** Date:           04/26/2021                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2021       XIAOJU.CHENG       Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef UAL_SS_SUPPORT
#include "mn_type.h"
#include "watch_commonwin_export.h"
#include "ual_cms_export.h"
#include "ual_tele.h"
#include "mmipb_export.h"
#include "ual_cms_export.h"
#include "set_ss_flow.h"
#include "mmicom_trace.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
extern MMISET_SS_PWD_INFO_T g_mmiset_ss_pwd;
#define SET_SS_MAX_TELE_NUM_LEN     (40)

LOCAL SETTINGS_SS_POWERON_PS_TYPE_E g_set_ss_poweron_ps_type = SETTINGS_SS_POWERON_PS_NONE;

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
                          )
{
    ual_ss_parameter_t ss_param = {0};
    ual_ss_error_e start_ret = UAL_SS_ERROR_MAX;
    ual_ss_pwd_info_t ss_pwd = {0};

    if(PNULL == ss_result_ptr||PNULL == ss_info_ptr||PNULL == input_str)
        return  start_ret;

    ss_param.ss_oper = ss_oper;
    ss_param.ss_code = ss_result_ptr->code;
    ss_param.basic_service.service_type = ss_info_ptr->basic_service.service_type;
    ss_param.basic_service.service_code = ss_info_ptr->basic_service.service_code;
	ss_param.register_param.no_reply_time = ss_info_ptr->register_param.cfnry_param.no_reply_time;
	ss_param.register_param.forward_num.number_plan = ss_info_ptr->register_param.forward_num.number_plan;
	ss_param.register_param.forward_num.number_type = ss_info_ptr->register_param.forward_num.number_type;
	ss_param.register_param.forward_num.num_len = ss_info_ptr->register_param.forward_num.num_len;
	SCI_MEMCPY(ss_param.register_param.forward_num.party_num,ss_info_ptr->register_param.forward_num.party_num,MN_MAX_ADDR_BCD_LEN);
	ss_param.register_param.cfnry_num.number_plan = ss_info_ptr->register_param.cfnry_param.forward_num.number_plan;
	ss_param.register_param.cfnry_num.number_type = ss_info_ptr->register_param.cfnry_param.forward_num.number_type;
	ss_param.register_param.cfnry_num.num_len = ss_info_ptr->register_param.cfnry_param.forward_num.num_len;
	SCI_MEMCPY(ss_param.register_param.cfnry_num.party_num,ss_info_ptr->register_param.cfnry_param.forward_num.party_num,MN_MAX_ADDR_BCD_LEN);
	SCI_MEMCPY(ss_pwd.new_pwd, g_mmiset_ss_pwd.new_pwd,MMISET_SS_PWD_LEN);
    SCI_MEMCPY(ss_pwd.new_pwd_two, g_mmiset_ss_pwd.new_pwd_two,MMISET_SS_PWD_LEN);
    SCI_MEMCPY(ss_pwd.old_pwd, g_mmiset_ss_pwd.old_pwd,MMISET_SS_PWD_LEN);
    //bug1414256
    //it is only for barring to set pwd
    ual_tele_ss_set_barring_password(&ss_pwd);

    switch (ss_oper)
    {
        case UAL_SS_REGISTERSS:
        case UAL_SS_ERASESS:
        case UAL_SS_ACTIVATESS:
        case UAL_SS_DEACTIVATESS:
        case UAL_SS_INTERROGATESS:
        {
            start_ret = ual_tele_ss_start_request(dual_sys,UAL_SS_REQUEST_ONE_SIM,UAL_SS_MAX_RETRYTIMES,ss_param,start_ss_callback);
            SCI_TRACE_LOW("[SSFLOW]:Settings_SS_SendRequest,send req to ss ss_oper:%d,start_ret:%d\n",ss_oper,start_ret);
        }
            break;
        case UAL_SS_REGISTERPASSWORD:
        {
            ss_param.ss_code = g_mmiset_ss_pwd.code;
            start_ret = ual_tele_ss_start_request( dual_sys, UAL_SS_REQUEST_ONE_SIM, UAL_SS_MAX_RETRYTIMES, ss_param, start_ss_callback);
            SCI_TRACE_LOW("[SSFLOW]:Settings_SS_SendRequest,send req to ss ss_oper:%d,start_ret:%d\n",ss_oper,start_ret);
        }
            break;
        default:
            break;
    }

    return (start_ret);
}

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
                                       )
{
    MMI_PARTY_NUMBER_T          party_num = {0};
    ual_ss_number_t             number = {0};
    uint8  phonenum[SET_SS_MAX_TELE_NUM_LEN + 1];
    ual_ss_error_e ss_err_code = UAL_SS_ERROR_MAX;
    ual_ss_parameter_t ss_param = {0};
    ual_ss_pwd_info_t ss_pwd = {0};
    ual_ss_request_type_e request_type = UAL_SS_REQUEST_ONE_SIM;

    SCI_TRACE_LOW("[SSFLOW]:Settings_SS_Start_Request dual_sys:%d,ss_code:%d,ss_oper:%d,retry_times:%d",
        dual_sys,ss_code,ss_oper,retry_times);

    if(PNULL != voice_number_ptr)
    {
        MMI_WSTRNTOSTR((uint8 *)phonenum, SET_SS_MAX_TELE_NUM_LEN, voice_number_ptr->wstr_ptr, voice_number_ptr->wstr_len, voice_number_ptr->wstr_len );

        // 将语音信箱号码或者电话号码转换为所需要的格式，存入number。
        MMIAPICOM_GenPartyNumber(phonenum, /*voice_number_ptr->wstr_ptr,*/voice_number_ptr->wstr_len, &party_num);

        //set register_param value
        number.number_type = party_num.number_type;
        number.number_plan = MN_NUM_PLAN_ISDN_TELE;
        number.num_len = party_num.num_len;
        MMI_MEMCPY(number.party_num, MN_MAX_ADDR_BCD_LEN, party_num.bcd_num, MN_MAX_ADDR_BCD_LEN, party_num.num_len);

        if (MN_SS_CFNRY == ss_code)//无应答转移
        {
    		SCI_MEMCPY(&ss_param.register_param.cfnry_num,&number,sizeof(ual_ss_number_t));
            ss_param.register_param.no_reply_time = 5;//此处暂时使用默认值5，需要具体的APP实现。
        }
        else
        {
    		SCI_MEMCPY(&ss_param.register_param.forward_num,&number,sizeof(ual_ss_number_t));
        }
    	SCI_TRACE_LOW("[SSFLOW]:Settings_SS_Start_Request,number_type:%d,number_plan:%d,num_len:%d",
    		ss_param.register_param.forward_num.number_type,ss_param.register_param.forward_num.number_plan,ss_param.register_param.forward_num.num_len);
    }

    //set ss param
    ss_param.ss_code = ss_code;
	ss_param.ss_oper = ss_oper;
    ss_param.basic_service.service_code = UAL_SS_ALL_SPEECH_TRANS_SERVICES;
    if(UAL_SS_INTERROGATESS == ss_oper)
    {
        ss_param.basic_service.service_type = UAL_SS_ALL_SERVICE;
    }
    else
    {
        ss_param.basic_service.service_type = UAL_SS_TELE_SERVICE;
    }

    SCI_MEMCPY(ss_pwd.new_pwd, g_mmiset_ss_pwd.new_pwd,MMISET_SS_PWD_LEN);
    SCI_MEMCPY(ss_pwd.new_pwd_two, g_mmiset_ss_pwd.new_pwd_two,MMISET_SS_PWD_LEN);
    SCI_MEMCPY(ss_pwd.old_pwd, g_mmiset_ss_pwd.old_pwd,MMISET_SS_PWD_LEN);
    //bug1414256
    //it is only for barring to set pwd
    ual_tele_ss_set_barring_password(&ss_pwd);

    if( dual_sys == MMI_DUAL_SYS_MAX)
    {
        request_type = UAL_SS_REQUEST_DUAL_SIM;
    }

    ss_err_code = ual_tele_ss_start_request( dual_sys, request_type, retry_times, ss_param, start_ss_callback);

    return ss_err_code;
}

/*****************************************************************************/
//   Description :register dual sim call forward
//	Global resource dependence :
//   Author:xiaoju.cheng
//   Note:
/*****************************************************************************/
PUBLIC ual_ss_error_e Settings_SS_RegisterDualsysCFNRC(ual_sim_number_e dual_sys, _ual_cms_client_register_callback  start_ss_callback)
{
    //开机注册呼叫转移
    ual_ss_string_t ss_string = {0};
    MMIPB_CONTACT_T local_num = {0};
    uint8 sim_num1[SET_SS_MAX_TELE_NUM_LEN] = {0};
    uint8 sim_num2[SET_SS_MAX_TELE_NUM_LEN] = {0};
    BOOLEAN get_name = FALSE;
    ual_ss_number_t called_number1 = {0};
    ual_ss_number_t called_number2 = {0};
    MMI_PARTY_NUMBER_T party_num = {0};
    ual_sim_number_e another_sys = UAL_SIM_NUMBER_MAX;
    ual_ss_error_e  ss_err_code = UAL_SS_ERROR_MAX;
    uint16  i = 0;
    ual_ss_parameter_t ss_param = {0};

    //SCI_ASSERT(dual_sys < MMI_DUAL_SYS_MAX);/* assert to do*/
    if(dual_sys >= UAL_SIM_NUMBER_MAX)
        return  ss_err_code;

#if defined(MMI_MULTI_SIM_SYS_DUAL)
    another_sys = (dual_sys == UAL_SIM_NUMBER_1)? UAL_SIM_NUMBER_2: UAL_SIM_NUMBER_1;
#endif

    SCI_TRACE_LOW("[SSFLOW]:Settings_SS_RegisterDualsysCFNRC,dual_sys:%d", dual_sys);

    //获取另一张卡的号码
    get_name = MMIAPIPB_ReadMsisdnContact(another_sys, &local_num);
    if (get_name)
    {
        called_number1.num_len = MMIAPIPB_BCDNumberToString(&local_num.number[0], SET_SS_MAX_TELE_NUM_LEN, sim_num1);
    }
    else
    {
        return ss_err_code;
    }
    MMIAPICOM_GenPartyNumber(sim_num1, strlen((const char*)sim_num1), &party_num);
    //set register_param value
    called_number1.number_type = party_num.number_type;
    called_number1.number_plan = MN_NUM_PLAN_ISDN_TELE;
    called_number1.num_len = party_num.num_len;
    MMI_MEMCPY(called_number1.party_num, MN_MAX_ADDR_BCD_LEN, party_num.bcd_num, MN_MAX_ADDR_BCD_LEN, party_num.num_len);
	SCI_MEMCPY(&ss_param.register_param.cfnry_num,&called_number1,sizeof(ual_ss_number_t));

    //获取第一张卡的号码
    SCI_MEMSET(&local_num, 0, sizeof(MMIPB_CONTACT_T));
    get_name = MMIAPIPB_ReadMsisdnContact(dual_sys, &local_num);
    if (get_name)
    {
        called_number2.num_len = MMIAPIPB_BCDNumberToString(&local_num.number[0], SET_SS_MAX_TELE_NUM_LEN, sim_num2);
    }
    else
    {
        return ss_err_code;
    }
    MMIAPICOM_GenPartyNumber(sim_num2, strlen((const char*)sim_num2), &party_num);
    //set register_param value
    called_number2.number_type = party_num.number_type;
    called_number2.number_plan = MN_NUM_PLAN_ISDN_TELE;
    called_number2.num_len = party_num.num_len;
    MMI_MEMCPY(called_number2.party_num, MN_MAX_ADDR_BCD_LEN, party_num.bcd_num, MN_MAX_ADDR_BCD_LEN, party_num.num_len);
	SCI_MEMCPY(&ss_param.register_param.dual_sys_forward_num,&called_number2,sizeof(ual_ss_number_t));

    //set ss_param
    ss_param.register_param.no_reply_time = 5;//此处暂时使用默认值5，需要具体的APP实现。
    SCI_MEMCPY( &ss_param.ss_string, &ss_string, sizeof(ual_ss_string_t));
    ss_param.ss_code = UAL_SS_CFNRC;
    ss_param.ss_oper = UAL_SS_REGISTERSS;
    ss_param.basic_service.service_code = UAL_SS_ALL_SPEECH_TRANS_SERVICES;
    ss_param.basic_service.service_type = UAL_SS_TELE_SERVICE;

    //在一张卡上注册另一张卡的呼叫转移
    SCI_TRACE_LOW("[SSFLOW]:MMISET_SendDualsysCFNRC begin,ss_code = %d",ss_param.ss_code);
	ss_err_code = ual_tele_ss_start_request( dual_sys,
        UAL_SS_REQUEST_DUAL_SIM_TRANSFER,
        UAL_SS_MIN_RETRYTIMES,
        ss_param,
        start_ss_callback);

    return ss_err_code;
}

/*****************************************************************************/
//  Discription: get if divert power on ps                                   *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_SS_Set_PowerOn_PS_Type( SETTINGS_SS_POWERON_PS_TYPE_E poweron_ps_type )
{
	SCI_TRACE_LOW("[SSFLOW]:Settings_SS_Set_PowerOn_PS_Type,poweron_ps = %d",poweron_ps_type);
    // TODO:need add poweron ps logic when power on
    g_set_ss_poweron_ps_type = poweron_ps_type;
}

/*****************************************************************************/
//  Discription: set if divert power on ps                                   *
//  Global resource dependence:                                              *
//  Author: xiaoju.cheng                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC SETTINGS_SS_POWERON_PS_TYPE_E Settings_SS_Get_PowerOn_PS_Type( void )
{
	SCI_TRACE_LOW("[SSFLOW]:Settings_SS_Get_PowerOn_PS_Type,s_is_callforwarding_poweron_ps = %d",g_set_ss_poweron_ps_type);
	return g_set_ss_poweron_ps_type;
}

#endif

