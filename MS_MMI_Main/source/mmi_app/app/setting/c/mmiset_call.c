/*****************************************************************************
** File Name:      mmiset_other.c                                            *
** Author:                                                                   *
** Date:           03/06/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004       Jassmine.Meng       Create
******************************************************************************/

#define _MMISET_CALL_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mn_api.h"
#include "mn_type.h"
#include "window_parse.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmisms_export.h"
#include "mmicc_export.h"
#include "mmiussd_export.h"
#include "mmi_textfun.h"
#include "mmisms_text.h"
#include "mmistk_export.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmipb_export.h"
#include "mmi_appmsg.h"
#include "mmi_nv.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
#include "mmiset_display.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
#include "mmiset_text.h"
#include "mmiset_wintab.h"
#include "mmipub.h"
#include "mmi_anim.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_internal.h"
#include "mmk_timer.h"
#include "mmipb_export.h"
#include "guirichtext.h"
//#include "mmiwap.h"
#include "mmiset_func.h"
#include "guiedit.h"
#include "mmipdp_export.h"
#include "mmk_app.h"
#include "mmicc_text.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiidle_statusbar.h"

#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#ifdef MET_MEX_AQ_SUPPORT
#include "met_mex_aq.h"
#endif
#endif
#if defined (ADULT_WATCH_SUPPORT) && defined (UAL_SS_SUPPORT)
#include "ual_tele.h"
#include "set_ss_flow.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISET_DEFAULT_NO_ANSWER_TIME_ITEM   3
#define MMISET_NO_ANSWER_TIME_MAX_LEN        20

#define MMISET_SS_MAX_NO_REPLY_TIME				30
#define MMISET_SS_MIN_NO_REPLY_TIME				5
#define MMISET_SS_STEP_REPLY_TIME               5
#define MMISET_SET_PER_TIME						1000
#define MMISET_PHONE_NUMBER_MAX_LEN         (MMIPB_BCD_NUMBER_MAX_LEN << 1)
#define MMISET_MAX_TELE_NUM_LEN                 40
#define MMISET_ALL_INTERROGATE_NUM    4

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
const MMI_SS_BS_TRANSFER_T s_mmi_ss_basic_service[] = 
{
    {10,   MN_SS_TELE_SERVICE,   MN_SS_ALL_TELESERVICES},
    {11,   MN_SS_TELE_SERVICE,   MN_SS_ALL_SPEECH_TRANS_SERVICES},
    {12,   MN_SS_TELE_SERVICE,   MN_SS_ALL_DATA_TELESERVICES},
    {13,   MN_SS_TELE_SERVICE,   MN_SS_ALL_FACSIMIL_TRANS_SERVICES},
    {16,   MN_SS_TELE_SERVICE,   MN_SS_ALL_SHORT_MSG_SERVICE},
    {19,   MN_SS_TELE_SERVICE,   MN_SS_TELESERVICES_EXP_SMS},
    {20,   MN_SS_BEARER_SERVICE,   MN_SS_ALL_BEARER_SERVICES},
    {21,   MN_SS_BEARER_SERVICE,   MN_SS_ALL_ANSY_SERVICES},
    {22,   MN_SS_BEARER_SERVICE,   MN_SS_ALL_SYNC_SERVICES},
    {24,   MN_SS_BEARER_SERVICE,   MN_SS_ALL_DATA_CIRCUIT_SYNC},
    {25,   MN_SS_BEARER_SERVICE,   MN_SS_ALL_DATA_CIRCUIT_ANSY}
};

const MMI_SS_CODE_TRANSLATE_T s_mmi_ss_translate[] =
{
    {2,"002",   MN_SS_ALL_FORWARD},
    {4,"004",   MN_SS_ALL_COND_FORWARD},
    {30,"30",   MN_SS_CLIP},
    {31,"31",   MN_SS_CLIR},
    {35,"35",   MN_SS_BAIC},
    {76,"76",   MN_SS_COLP},
    {77,"77",   MN_SS_COLR},
    {21,"21",   MN_SS_CFU},
    {67,"67",   MN_SS_CFB},
    {61,"61",   MN_SS_CFNRY},
    {62,"62",   MN_SS_CFNRC},
    {43,"43",   MN_SS_CW},
    {33,"33",   MN_SS_BAOC},
    {330,"330",   MN_SS_ALL_BAR_SERVICE},
    {331,"331",   MN_SS_BOIC},
    {332,"332",   MN_SS_BOIC_EXHC},
    {333,"333",   MN_SS_BAR_OUTGOING},
    {351,"351",   MN_SS_BICROAM},
    {353,"353",   MN_SS_BAR_INCOMING}
};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    CONTENT_TYPE_SERVIEC_NAME,      //附加服务类型：无条件呼叫转移等
    CONTENT_TYPE_CALL_CLASS,        //对应的电话类型：语音电话，视频电话
    CONTENT_TYPE_SERVICE_STATUSE,   //附加服务的状态
    CONTENT_TYPE_MAX
}CONTENT_TYPE_E;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN							 s_has_query_cfu = FALSE;//the flag to mark has query cfu status
LOCAL BOOLEAN     s_dualsys_cfu_query_info[MMI_DUAL_SYS_MAX] = {FALSE}; //IDLE界面查询CFU状态图标
LOCAL int32		s_query_cf_num = 0;	//@samboo 查询所有呼叫转移时记录查询次数，0代表不是查询所有呼叫转移
LOCAL APP_MN_INTERROGATE_CNF_T  *s_cf_info_ptr = PNULL;
LOCAL uint8 s_cf_list_num[MMISET_ALL_INTERROGATE_NUM] = {0};
LOCAL BOOLEAN s_is_ss_cfu_actived[MMI_DUAL_SYS_MAX][MMISET_SS_MAX] = {0};
LOCAL MMISET_IP_NUMBER_CONTEXT_T  g_ip_number_context = {FALSE,{0,}};/*lint !e651*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMISET_SS_STATUS_T      g_mmiset_ss_status = {0, MN_SS_CLIP, {MN_SS_ALL_SERVICE, 0}};	/*lint !e64*///the status of ss
MMI_APPLICATION_T       g_mmiset_app = {0};			//Setting and ring的实体

MMISET_SS_PWD_INFO_T    g_mmiset_ss_pwd = {0};		//从idle输入的ss暗码中获得ss密码并且记录
#ifdef MMI_CSP_SUPPORT
uint8                   g_current_line[MMI_DUAL_SYS_MAX] = {0};			//the status of line
#endif
BOOLEAN                 g_is_for_cfu_icon = FALSE;  //查询网络状态是否为了显示图标
BOOLEAN                 g_is_need_update_cfu_icon = FALSE;  //查询呼叫转接同时根据网络状态更新图标
uint8                   g_send_ss_search_timer_id = 0;      // 记录定时器是否启动
BOOLEAN                 g_is_background_search_send = FALSE; //是否已经激活另一个网络
BOOLEAN                 g_is_ss_user_query= FALSE;            //用户是否正在进行call waiting状态查询业务
MMISET_SS_STATUS_T      g_background_ss_status = {0};       /*lint !e64*///激活另一个网络的ss状态

#ifdef MMI_CSP_SUPPORT
LOCAL BOOLEAN s_is_csp2_valid[MMI_DUAL_SYS_MAX] = {FALSE};
#endif

#ifdef MMI_CPHS_SUPPORT
LOCAL MMISET_CPHS_INFO_T s_cphs_info[MMI_DUAL_SYS_MAX] = {0};
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify Set
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifySetFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif

/*****************************************************************************/
// 	Description : 处理Setting call的PS消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsMsg(
                               PWND                app_ptr, 
                               uint16              msg_id, 
                               DPARAM              param
                               );
/*****************************************************************************/
// 	Description : generate interrogate report of cfu status for idle win icon
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void GenDcsStatusReport(
                              MN_DUAL_SYS_E               dual_sys,
                              uint8                       list_num,
                              MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                              );
/*****************************************************************************/
// 	Description : generate interrogate detail report of cfu status for idle win icon
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void GenDcsDetailReport(
                              MN_DUAL_SYS_E             dual_sys,
                              MN_SS_INTERROGATE_INFO_T *info_list_ptr
                              );
#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : 设置中线路选择的类型
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void MMISET_SaveLineInfoToNV(   
                                   MMISET_LINE_TYPE_E	    line_type,   // the type of line
                                   MN_DUAL_SYS_E           dual_sys
                                   );
#endif
/*****************************************************************************/
// 	Description : generate interrogate report of cfu status
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void GenDcsReport(
                        MN_DUAL_SYS_E               dual_sys,
                        uint8                       list_num,
                        MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                        );
/*****************************************************************************/
// 	Description : Reset_ClirType 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void	ResetClirType(void);

/*****************************************************************************/
// 	Description : update customer service profile
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void UpdateCustomerServiceProfile(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_GET_PASSWORD_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGetPasswordInd(
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_REGISTER_PASSWORD_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRegisterPasswordCnf(
                                             DPARAM              param
                                             );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_RELEASE_SS_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReleaseSsInd(
                                      DPARAM              param
                                      );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_REGISTER_SS_CNF,APP_MN_ERASE_SS_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOperateSsCnf(
                                      MN_DUAL_SYS_E             dual_sys,
                                      uint8                     list_num,
                                      MMISET_SS_OPERATE_TYPE_E  operate_type,
                                      MN_SS_OPERATE_RESULT_E    result_type,
                                      MN_SS_ERROR_CODE_E        error_code,
                                      MN_SS_INTERROGATE_INFO_T  *info_list_ptr
                                      );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_INTERROGATE_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInterrogateSsCnf(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : generate interrogate report of ss status
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateReport(
                                uint8                       list_num,
                                MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                                );

/*****************************************************************************/
// 	Description : generate interrogate report of ss status name
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateServiceReport(void);

/*****************************************************************************/
// 	Description : generate interrogate report of ss status
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateStatusReport(
                                      uint8                       list_num,
                                      MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                                      );
#ifdef MMI_DUALMODE_ENABLE
/*****************************************************************************/
// 	Description : generate interrogate report of ss status for VT
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateVTStatusReport(
                                        uint8                       list_num,
                                        MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                                        );
#endif
/*****************************************************************************/
// 	Description : generate interrogate detail report of ss status
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateDetailReport(
                                      MN_SS_INTERROGATE_INFO_T *info_list
                                      );

/*****************************************************************************/
// 	Description : generate interrogate detail report of call forwarding phone number
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateTelenumReport(
                                       MN_SS_INTERROGATE_INFO_T  *info_list_ptr
                                       );

/*****************************************************************************/
// 	Description : generate interrogate detail report of no answer time
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateTimeReport(
                                    MN_SS_INTERROGATE_INFO_T  *info_list_ptr
                                    );



/*****************************************************************************/
// 	Description : set ss param,include ss_code and sevice_type,service_code
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void SetSsParam(
                      MN_DUAL_SYS_E dual_sys,
                      BOOLEAN   is_interrogate
                      );

/*****************************************************************************/
// 	Description : 处理网络连接界面的消息(除PS之外)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkConnectWindow(
                                              MMI_WIN_ID_T       win_id, 	// 窗口的ID
                                              MMI_MESSAGE_ID_E 	    msg_id, 	// 窗口的内部消息ID
                                              DPARAM 		        param		// 相应消息的参数
                                              );
/*****************************************************************************/
//  Description : transfer the ss code according to the array s_ss_basic_service
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN ComputerBasicService(
                                   MMISET_SS_PARSE_INFO_T   *ss_result,
                                   MN_SS_BASIC_SERVICE_T *basic_service  
                                   );
/*****************************************************************************/
// 	Description : 获得无应答时间
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL uint8 GetNoAnswerTime(
                            MMI_CTRL_ID_T	    ctrl_id
                            );

/*****************************************************************************/
//  Description : user input string is related to SIM
//  Global resource dependence : none
//  Author: Jassmine
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsSimFunction(
                            char      *str_ptr,    //Input string
                            uint8     len         //string's length
                            );

/*****************************************************************************/
//  Description : Parse the ss data from mmi
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN ParseSsRegisterData(
                                  char* ss_data
                                  );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN DecodeSsDigitInfo(
                                char      **string_ptr,
                                char      *pos_end_ptr,
                                uint16    *code_ptr
                                );

/*****************************************************************************/
//  Description : get ss old password 
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN DecodeSsStringInfo(
                                 char   **string_ptr,
                                 char   *pos_end_ptr,
                                 char   *data,
                                 uint8  len_allowed    //The capability of the data
                                 );

/*****************************************************************************/
// 	Description : UpdateCfuStatuaAfterOperate
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void UpdateCfuStatuaAfterOperate(MN_DUAL_SYS_E dual_sys, MMISET_SS_OPERATE_TYPE_E  operate_type);

/*****************************************************************************/
//  Description : Parse the ss data from mmi
//  Global resource dependence : none
//  Author:
//  Note:Form of the ss_data:SC*SIA*SIB*SIC#
/*****************************************************************************/
LOCAL BOOLEAN ParseSsData(
                          char                      *ss_data,
                          MMISET_SS_PARSE_INFO_T    *ss_out
                          );

/*****************************************************************************/
//  Description : Adjust it if is the ss code 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustSsCode(
                           uint16   *ss_code_ptr
                           );

/*****************************************************************************/
//  Description : Handle divert numbers
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN HandleSsDivertNum(
                                MMISET_SS_PARSE_INFO_T  *ss_result_ptr,
                                MMISET_SS_INFO_T        *ss_info_ptr,
                                MN_SS_OPERATION_CODE_E ss_oper
                                );

/*****************************************************************************/
//  Description : Send stand ss request to MN
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN SendReqToSs(
                          MN_DUAL_SYS_E             dual_sys,
                          MN_SS_OPERATION_CODE_E    ss_oper,
                          MMISET_SS_PARSE_INFO_T    *ss_result_ptr,
                          MMISET_SS_INFO_T          *ss_info_ptr,
                          uint8 *input_str,
                          uint8 str_len
                          );
/*****************************************************************************/
//  Description : query all cfu handle
//  Global resource dependence : none
//  Author:
//  Note:Form:
/*****************************************************************************/
LOCAL void HandleAllQueryCF(MN_DUAL_SYS_E dual_sys, uint8 list_num, APP_MN_INTERROGATE_CNF_T ss_info_ptr);

/*****************************************************************************/
//  Description : display the result of  all cfu query
//  Global resource dependence : none
//  Author:
//  Note:Form:
/*****************************************************************************/
LOCAL void GenAllCFReport(void);

/*****************************************************************************/
// 	Description : generate a report of all cf status
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
//LOCAL void GenAllCFStatusReport(
//                                      uint8                       list_num,
//                                      MN_SS_INTERROGATE_INFO_T    *info_list_ptr
//                                      );
//
///*****************************************************************************/
//// 	Description : generate interrogate detail report of ss status
////	Global resource dependence : 
////  Author: Jassmine
////	Note:
///*****************************************************************************/
//LOCAL void GenAllCFDetailReport(
//                                      MN_SS_INTERROGATE_INFO_T  *info_list_ptr
//                                      );
//
///*****************************************************************************/
//// 	Description : generate interrogate detail report of call forwarding phone number
////	Global resource dependence : 
////  Author: Jassmine
////	Note:
///*****************************************************************************/
//LOCAL void GenAllCFTelenumReport(
//                                       MN_SS_INTERROGATE_INFO_T  *info_list_ptr
//                                       );
//
///*****************************************************************************/
//// 	Description : generate interrogate detail report of no answer time
////	Global resource dependence : 
////  Author: Jassmine
////	Note:
///*****************************************************************************/
//LOCAL void GenAllCFTimeReport(
//                                    MN_SS_INTERROGATE_INFO_T  *info_list_ptr
//                                    );

/*****************************************************************************/
// 	Description : add str to MMISET_SS_ALL_REPORT_CTRL
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
LOCAL void AddStrToReportText( MMI_STRING_T str, CONTENT_TYPE_E type);

/*****************************************************************************/
//  Description :Get Interrogate Cfu Sim Id
//  Global resource dependence : 
//  Author: Jassmine
//    Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetCfuSimId(void);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify Set
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifySetFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMISET]: HandleSIMPlugNotifySetFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_516_112_2_18_2_50_4_0,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        //no care
        break;

    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMK_CloseWin(MMISET_COM_SELECT_SIM_WIN_ID);
        MMK_CloseWin(MMISET_SET_DUALSYS_SIM_ACTIVE_TYPE_WIN_ID);                    
#endif
        //close ss service
        if(g_mmiset_ss_status.dual_sys == dual_sys)
        {
            g_is_background_search_send = FALSE;
            g_is_ss_user_query = FALSE;
            g_is_for_cfu_icon = FALSE;
            if(MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID))
            {
                MMK_CloseMiddleWin(SET_WIN_ID_START, SET_MAX_WIN_ID);
            }
        }
        break;

    default:
        break;
    }
}
#endif

/*****************************************************************************/
// 	Description : init call settings
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CallInit(void)
{
    MMISET_KEYLOCK_TYPE_E    auto_keylock_time = MMISET_DEFAULT_KEYLOCK_TIME;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
#ifdef FLIP_PHONE_SUPPORT
    MN_DUAL_SYS_E flip_answer_sys = MN_DUAL_SYS_1;
#endif
    BOOLEAN              is_red_keylock = FALSE; 
#ifdef SIM_PLUG_IN_SUPPORT    
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
#endif
    uint32 i  = 0;
    
    //set the ps callback function
    g_mmiset_app.ProcessMsg     = HandlePsMsg; 		
//    g_mmiset_app.component_type = CT_APPLICATION;
#ifdef FLIP_PHONE_SUPPORT    //flip宏控制，裁减nv MMINV_SET_IS_FLIPANSWER
    //init if is flip answer
    MMIAPISET_GetIsFlipAnswer(flip_answer_sys);
#endif
    //init auto boardkey
    MMINV_READ(MMINV_SET_AUTO_KEYLOCK_TIME,&auto_keylock_time,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        auto_keylock_time = MMISET_DEFAULT_KEYLOCK_TIME;
        MMINV_WRITE(MMINV_SET_AUTO_KEYLOCK_TIME,&auto_keylock_time);
    }
    //init red keylock
    MMINV_READ(MMINV_SET_IS_RED_KEYLOCK,&is_red_keylock,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_red_keylock = FALSE;
        MMINV_WRITE(MMINV_SET_IS_RED_KEYLOCK,&is_red_keylock);
    }
    
    MMIAPISET_GetIsAutoRedail();
    MMIAPISET_GetMinuteReminderTime();
#ifdef MMI_IP_CALL_SUPPORT
    MMISET_IPNumberContextInit();
#endif
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    if (MMIAPIPHONE_IsSameSim(MN_DUAL_SYS_1))
    {
        //init clir type
        MMIAPISET_GetClirType(MN_DUAL_SYS_1);
    }
    else 
    {
        ResetClirType();
    }
#endif
    
    //init call line info
    for(i=0; i<MMI_DUAL_SYS_MAX; i++)
    {
        MMIAPISET_GetCallLineInfo((MN_DUAL_SYS_E)i);
    }
#ifdef SIM_PLUG_IN_SUPPORT    
    sim_plug_notify_info.module_id = MMI_MODULE_SET;
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifySetFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif
}

/*****************************************************************************/
// 	Description : get clir type
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC MN_CALL_CLIR_E  MMIAPISET_GetClirType(MN_DUAL_SYS_E dual_sys)
{
    MN_CALL_CLIR_E clir_type_array[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    SCI_ASSERT(dual_sys < MMI_DUAL_SYS_MAX);/* assert to do*/
    
    //get the clir type from nv
    MMINV_READ(MMINV_SET_CLIR_TYPE, clir_type_array, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        uint32 i = 0;

        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            clir_type_array[i] = MN_CALL_CLIR_NETWORK_DECIDE;
        }
        
        MMINV_WRITE(MMINV_SET_CLIR_TYPE, clir_type_array);
    }
    SCI_TRACE_LOW("MMIAPISET_GetClirType dual_sys:%d, clir_type:%d", dual_sys, clir_type_array[dual_sys]);
    return (clir_type_array[dual_sys]);
}

/*****************************************************************************/
// 	Description : get clir type
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void ResetClirType(void)
{
    uint32 i = 0;
    MN_CALL_CLIR_E clir_type_array[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        clir_type_array[i] = MN_CALL_CLIR_NETWORK_DECIDE;
    }
    
    MMINV_WRITE(MMINV_SET_CLIR_TYPE, clir_type_array);
}

/*****************************************************************************/
// 	Description : 处理Setting call的PS消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsMsg(
                               PWND                app_ptr, 
                               uint16              msg_id, 
                               DPARAM              param
                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case APP_MN_USSD_IND:
        result = MMIAPIUSSD_HandleUssdInd(param);        
        MMIAPIUSSD_SetPlayringFlage(FALSE);
        break;
        
    case APP_MN_USSD_NOTIFY_IND:
        result = MMIAPIUSSD_HandleUssdNotifyInd(param);
        MMIAPIUSSD_SetPlayringFlage(FALSE);
        break;
        
    case APP_MN_USSD_SERVICE_CNF:
        MMIAPIUSSD_SetPlayringFlage(TRUE);
        if (!MMIAPISTK_OnSendUSSDCnf(param))
        {
            result = MMIAPIUSSD_HandleUssdServiceCnf(param);
        }
        break;
        
    case APP_MN_GET_PASSWORD_IND:
        //该消息用于发送用户输入的密码
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
        ual_tele_ss_handle_get_password_ind(param);
#endif
#else
        result = HandleGetPasswordInd(param);
#endif
        break;
        
    case APP_MN_REGISTER_PASSWORD_CNF:
        //@gang.tong add 2005.11.24
        //如果stk等待这个消息，那么stk处理完后就返回TRUE
        MMIAPISTK_OnSendRegisterPaswdCnf(param);
        //该消息用于汇报更改密码操作的结果,
        //目前只有idle输入ss暗码更改密码才会用到此消息
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
        ual_tele_ss_handle_register_password_cnf(param);
#endif
#else
        result = HandleRegisterPasswordCnf(param);
#endif
        break;
        
    case APP_MN_RELEASE_SS_IND:
        {
            APP_MN_REGISTER_SS_CNF_T ss_param = {0};
            APP_MN_USSD_SERVICE_CNF_T ussd_service_cnf = {0};
            APP_MN_RELEASE_SS_IND_T release_ss_ind = *((APP_MN_RELEASE_SS_IND_T *)param);

            ss_param.result_type = MN_SS_OPERATION_ERROR;
            ss_param.dual_sys = release_ss_ind.dual_sys;
            if (release_ss_ind.error_present)
            {
                ss_param.err_code = release_ss_ind.error_code;
            }

            MMIAPISTK_OnSendSSCnf(&ss_param);

            ussd_service_cnf.result_type = MN_SS_OPERATION_ERROR;
            ussd_service_cnf.dual_sys = release_ss_ind.dual_sys;
            if (release_ss_ind.error_present)
            {
                ussd_service_cnf.err_code = release_ss_ind.error_code;
            }

            MMIAPISTK_OnSendUSSDCnf(&ussd_service_cnf);
        }

        //SCI_TRACE_LOW:"HandlePsMsg:receive APP_MN_RELEASE_SS_IND (%d, %d, %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISET_CALL_702_112_2_18_2_50_4_1,(uint8*)"ddd", g_is_background_search_send, g_is_ss_user_query, g_is_for_cfu_icon);
        MMIAPIUSSD_SetPlayringFlage(TRUE);

        // 用于查询附加业务的现行状态和登记信息
        if (g_is_background_search_send)
        {
            g_is_background_search_send = FALSE;
            //关闭网络连接窗口
            if (MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID))
            {
                MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
            }

            MMIAPISMS_CheckTimerMSG();
        }
        else
        {
            if (g_is_ss_user_query)
            {
                g_is_ss_user_query = FALSE;
            }
        
            // 该消息用于释放当前的附加业务
            result = HandleReleaseSsInd(param);
        }
        break;
        
    case APP_MN_REGISTER_SS_CNF:
        {
            APP_MN_REGISTER_SS_CNF_T    register_ss_cnf = *((APP_MN_REGISTER_SS_CNF_T *)param);
            //@gang.tong add 2005.11.24
            //如果stk等待这个消息，那么stk处理完后就返回TRUE
            MMIAPISTK_OnSendSSCnf(param);	
            
            //SCI_TRACE_LOW:"HandlePsMsg:receive APP_MN_REGISTER_SS_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_735_112_2_18_2_50_4_2,(uint8*)"");
            // 该消息用于发送登记附加业务的请求
            
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
           ual_tele_ss_handle_operate_cnf(
                register_ss_cnf.dual_sys,
                UAL_SS_REGISTER,
                register_ss_cnf.result_type,
                register_ss_cnf.err_code);
#endif
#else
           result = HandleOperateSsCnf(register_ss_cnf.dual_sys,
                                        register_ss_cnf.list_num,
                                        MMISET_SS_REGISTER,
                                        register_ss_cnf.result_type,
                                        register_ss_cnf.err_code,
                                        register_ss_cnf.info_list);
#endif
        }
        break;
        
    case APP_MN_ERASE_SS_CNF:
        {
            APP_MN_ERASE_SS_CNF_T   erase_ss_cnf = *((APP_MN_ERASE_SS_CNF_T *)param);
            
            //@gang.tong add 2005.11.24
            //如果stk等待这个消息，那么stk处理完后就返回TRUE
            MMIAPISTK_OnSendSSCnf(param);			
            //SCI_TRACE_LOW:"HandlePsMsg:receive APP_MN_ERASE_SS_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_755_112_2_18_2_50_4_3,(uint8*)"");
            // 该消息用于发送删除附加业务的请求
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
            ual_tele_ss_handle_operate_cnf(erase_ss_cnf.dual_sys,
                UAL_SS_ERASE,
                erase_ss_cnf.result_type,
                erase_ss_cnf.err_code);
#endif
#else
            result = HandleOperateSsCnf(erase_ss_cnf.dual_sys,
                                        erase_ss_cnf.list_num,
                                        MMISET_SS_ERASE,
                                        erase_ss_cnf.result_type,
                                        erase_ss_cnf.err_code,
                                        erase_ss_cnf.info_list);
#endif
        }
        break;
        
    case APP_MN_ACTIVATE_SS_CNF:
        {
            APP_MN_ACTIVATE_SS_CNF_T   activate_ss_cnf = *((APP_MN_ACTIVATE_SS_CNF_T *)param);
            
            //@gang.tong add 2005.11.24
            //如果stk等待这个消息，那么stk处理完后就返回TRUE
            MMIAPISTK_OnSendSSCnf(param);
            //SCI_TRACE_LOW:"HandlePsMsg:receive APP_MN_ACTIVATE_SS_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_774_112_2_18_2_50_4_4,(uint8*)"");
            //该消息用于激活已提供但还处于未激活状态的附加业务
            
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
            ual_tele_ss_handle_operate_cnf(activate_ss_cnf.dual_sys,
                            UAL_SS_ACTIVATE,
                            activate_ss_cnf.result_type,
                            activate_ss_cnf.err_code);
#endif
#else
            result = HandleOperateSsCnf(activate_ss_cnf.dual_sys,
                                                    activate_ss_cnf.list_num,
                                                    MMISET_SS_ACTIVATE,
                                                    activate_ss_cnf.result_type,
                                                    activate_ss_cnf.err_code,
                                                    activate_ss_cnf.info_list);
#endif
        }
        break;
        
    case APP_MN_DEACTIVATE_SS_CNF:
        {
            APP_MN_DEACTIVATE_SS_CNF_T   deactivate_ss_cnf = *((APP_MN_DEACTIVATE_SS_CNF_T *)param);
            
            //@gang.tong add 2005.11.24
            //如果stk等待这个消息，那么stk处理完后就返回TRUE
            if (MMIAPISTK_OnSendSSCnf(param))
            {
                //如果是stk的操作，就不用下面的handleoperatesscnf的调用了。
                break;
            }
            //SCI_TRACE_LOW:"HandlePsMsg:receive APP_MN_DEACTIVATE_SS_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_797_112_2_18_2_50_4_5,(uint8*)"");
            //该消息用于去活已激活的附加业务
            
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
            ual_tele_ss_handle_operate_cnf(deactivate_ss_cnf.dual_sys,
                            UAL_SS_DEACTIVATE,
                            deactivate_ss_cnf.result_type,
                            deactivate_ss_cnf.err_code);
#endif
#else
            result = HandleOperateSsCnf(deactivate_ss_cnf.dual_sys,
                                                    deactivate_ss_cnf.list_num,
                                                    MMISET_SS_DEACTIVATE,
                                                    deactivate_ss_cnf.result_type,
                                                    deactivate_ss_cnf.err_code,
                                                    deactivate_ss_cnf.info_list);
#endif
        }
        break;
        
    case APP_MN_INTERROGATE_CNF:
        //@gang.tong add 2005.11.24
        //如果stk等待这个消息，那么stk处理完后就返回TRUE
        if (MMIAPISTK_OnSendSSCnf(param))
        {
            break;
        }	
        
        //SCI_TRACE_LOW:"HandlePsMsg:receive APP_MN_INTERROGATE_CNF (%d, %d, %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_820_112_2_18_2_50_5_6, (uint8*)"ddd", g_is_background_search_send, g_is_ss_user_query, g_is_for_cfu_icon);
        // 用于查询附加业务的现行状态和登记信息
        if (g_is_background_search_send)
        {
            g_is_background_search_send = FALSE;
            //关闭网络连接窗口
            if (MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID))
            {
                MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
            }

            MMIAPISMS_CheckTimerMSG();
        }
        else
        {
            if (g_is_ss_user_query)
            {
                g_is_ss_user_query = FALSE;
            }
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
            ual_tele_ss_handle_interrogate_cnf(param);
#endif
#else
            result = HandleInterrogateSsCnf(param);
#endif
        }
        break;

    default:
        //SCI_TRACE_LOW("HandlePsMsg: receive SS msg_id = %d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
// 	Description :关闭网络连接的窗口（for phone module）
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_CloseConnectNetworkWin(void)
{
    //关闭网络连接窗口
    MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
}

/*****************************************************************************/
// 	Description :关闭网络列表的窗口（for phone module）
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ClosePlmnListWin(void)
{
#ifdef MMI_DUALMODE_ENABLE
    MMK_CloseWin(MMISET_SET_NETWORK_METHOD_WIN_ID);
#else
    MMK_CloseWin(MMISET_SET_SELECT_NETWORK_WIN_ID);
#endif

    //关闭网络连接和网络列表窗口
    MMK_CloseWin(MMISET_SET_PLMN_LIST_WIN_ID);
}
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_GET_PASSWORD_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGetPasswordInd(
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    APP_MN_GET_PASSWORD_IND_T     ss_get_password_ind;
    
    //SCI_ASSERT(PNULL != param);/* assert to do*/
    if(PNULL == param)
        return MMI_RESULT_TRUE;
    
    ss_get_password_ind = *(APP_MN_GET_PASSWORD_IND_T *)param;

    //SCI_TRACE_LOW:"HandleGetPasswordInd:guidance_info is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_887_112_2_18_2_50_5_7,(uint8*)"d",ss_get_password_ind.guidance_info);

    if(!MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID))
    {
        //SCI_TRACE_LOW:"connect win is closed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_891_112_2_18_2_50_5_8,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    switch (ss_get_password_ind.guidance_info)
    {
    case MN_SS_ENTER_PW://输入呼叫禁止密码
        //SCI_TRACE_LOW("HandleGetPasswordInd:enter input call bar password!");
        MNSS_SendUserPasswordEx(ss_get_password_ind.dual_sys, g_mmiset_ss_pwd.old_pwd);
        break;
        
    case MN_SS_ENTER_NEW_PW://输入呼叫禁止新密码
        MNSS_SendUserPasswordEx(ss_get_password_ind.dual_sys, g_mmiset_ss_pwd.new_pwd);
        break;
        
    case MN_SS_ENTER_NEW_PW_AGAIN://输入呼叫禁止新确认密码
        MNSS_SendUserPasswordEx(ss_get_password_ind.dual_sys, g_mmiset_ss_pwd.new_pwd_two);
        break;
        
    default:
        SCI_TRACE_LOW("HandleGetPasswordInd: guidance_info is error!");
        break;
    }
    
    //关闭语音信箱、输入电话号码、无应答时间
    MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
    MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
    MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_REGISTER_PASSWORD_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRegisterPasswordCnf(
                                             DPARAM              param
                                             )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_REGISTER_PASSWORD_CNF_T register_pwd_cnf;
    
    //SCI_ASSERT(PNULL != param);/* assert to do*/
    if(param == PNULL)
        return MMI_RESULT_TRUE;
    
    register_pwd_cnf = *((APP_MN_REGISTER_PASSWORD_CNF_T *)param);


    switch (register_pwd_cnf.result_type)
    {
    case MN_SS_OPERATION_SUCC:
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
        break;
        
    case MN_SS_OPERATION_ERROR:
        switch (register_pwd_cnf.err_code)
        {
        case MN_SS_ERROR_NEGATIVE_PW_CHECK:
            //提示密码错误
            MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
            break;
            
        case MN_SS_ERROR_NUMBER_OF_PW_ATTEMPTS_VIOLATION:
            //提示密码无效
            MMIPUB_OpenAlertWarningWin(TXT_PASSWORD_INVALID);
            break;
            
        default:
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            break;
        }
        break;
        
        case MN_SS_OPERATION_REJECT:
            MMIPUB_OpenAlertWarningWin(TXT_REJECT);
            break;
            
        default:
            SCI_TRACE_LOW("HandleRegisterPasswordCnf: result_type is error!");
            break;
    }
    
    // close idle dialing win
    MMIAPIIDLE_CloseDialWin();
    //关闭网络连接窗口
    MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
    
    //输入密码界面
    MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
    MMK_CloseWin(MMISET_SET_BAR_CHANGE_PW_WIN_ID);
    MMK_CloseWin(MMISET_SET_BAR_CHANGE_PW_AGAIN_WIN_ID);
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_RELEASE_SS_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReleaseSsInd(
                                      DPARAM              param
                                      )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_RELEASE_SS_IND_T release_ss_ind = *((APP_MN_RELEASE_SS_IND_T *)param);
    //SCI_ASSERT(PNULL != param);/* assert to do*/

    //SCI_TRACE_LOW:"HandleReleaseSsInd: error_present is %d and error_code is %d, %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISET_CALL_1000_112_2_18_2_50_5_9, (uint8*)"dddd", release_ss_ind.error_present,release_ss_ind.error_code, g_is_for_cfu_icon, g_mmiset_ss_status.ss_code);
    
    if (g_is_for_cfu_icon)
    {
        g_is_for_cfu_icon = FALSE;

        if (MN_SS_CFU == g_mmiset_ss_status.ss_code)
        {
            MMIAPIPHONE_InterrogateCfuForIdle(GetCfuSimId());
        }

        MMIAPISMS_CheckTimerMSG();
    }
    else
    {
        g_is_for_cfu_icon = FALSE;

        if (!MMISET_IsOpenDualSysCfnrc()
            && (MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID)
                || MMIAPIUSSD_IsSendWinOpen()))
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_NETWORK_CONNECT_FAIL);
        }
    }
        
    MMIAPIUSSD_CloseSendWin();
    MMIAPIUSSD_CloseInputWin();
    MMIAPIUSSD_CloseShowDataWin();

    //关闭网络连接窗口
    MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
    
    //关闭语音信箱、输入电话号码、无应答时间和输入密码界面
    MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
    MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
    MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
    MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    //add for dual sys
    MMISET_CloseDualsysCFNRCWin(release_ss_ind.dual_sys, FALSE, release_ss_ind.error_code);
#endif
    
    return (result);
}

/*****************************************************************************/
// 	Description : update customer service profile
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void UpdateCustomerServiceProfile(MN_DUAL_SYS_E dual_sys)
{
    ERR_MNDATAMAG_CODE_E      ret_value = ERR_MNDATAMAG_NO_ERR;
    MN_CSP_SERVICES_GROUP_T services_group = {0};
    
    //调用接口，查询出CSP的状态
    ret_value = MNSIM_GetCustomerServiceProfileEx(dual_sys, &services_group);
    if (ERR_MNDATAMAG_NO_ERR == ret_value)
    {
        switch( g_mmiset_ss_status.ss_code ) 
        {
        case MN_SS_CFB:
            services_group.call_offering.is_cfb = TRUE;
            break;
        case MN_SS_CFNRY:
            services_group.call_offering.is_cfnry = TRUE;							
            break;
        case MN_SS_CFNRC:
            services_group.call_offering.is_cfnrc = TRUE;						
            break;
        case MN_SS_BAOC:
            services_group.call_restriction.is_boac = TRUE;						
            break;
        case MN_SS_BOIC:
            services_group.call_restriction.is_boic = TRUE;			
            break;
        case MN_SS_BOIC_EXHC:
            services_group.call_restriction.is_boic_exhc = TRUE;			
            break;
        case MN_SS_BAIC:
            services_group.call_restriction.is_baic = TRUE;
            break;
        case MN_SS_BICROAM:
            services_group.call_restriction.is_bic_roam = TRUE;		
            break; 
        case MN_SS_HOLD:
            services_group.call_completion.is_hold = TRUE;				
            break;								
        case MN_SS_CW:
            services_group.call_completion.is_cw = TRUE;				
            break;
        case MN_SS_CLIP:
            services_group.number_identif.is_clip = TRUE;						
            break;
        case MN_SS_COLP:
            services_group.number_identif.is_colp = TRUE;			
            break;
        case MN_SS_COLR:
            services_group.number_identif.is_colr = TRUE;		
            break;													
        default:
            break;
        }
    }
    MNSIM_UpdateCustomerServiceProfileEx(dual_sys, &services_group);	
}

/*****************************************************************************/
// 	Description : UpdateCfuStatuaAfterOperate
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void UpdateCfuStatuaAfterOperate(MN_DUAL_SYS_E dual_sys, MMISET_SS_OPERATE_TYPE_E  operate_type)
{
    //SCI_ASSERT(dual_sys < MMI_DUAL_SYS_MAX);/* assert to do*/
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return;
    
    //SCI_TRACE_LOW:"UpdateCfuStatuaAfterOperate ss_code=%d,service_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1116_112_2_18_2_50_5_10,(uint8*)"dd", g_mmiset_ss_status.ss_code,g_mmiset_ss_status.basic_service.service_type);
    
    //update idle cfu icon
    if (operate_type == MMISET_SS_REGISTER || operate_type == MMISET_SS_ACTIVATE)
    {
        if ((g_mmiset_ss_status.ss_code == MN_SS_CFU) || (g_mmiset_ss_status.ss_code == MN_SS_ALL_FORWARD))
        {
#ifdef MMI_CSP_SUPPORT 
            if (0 == g_current_line[dual_sys])
            {
                MNSIM_UpdateDivertedCallStatusEx(dual_sys, MN_BEARER_CAPABILITY_LINE_1, TRUE);        								
            }
            else if (1 == g_current_line[dual_sys])
            {
                MNSIM_UpdateDivertedCallStatusEx(dual_sys, MN_BEARER_CAPABILITY_LINE_2, TRUE);     
            }
#else
            MNSIM_UpdateDivertedCallStatusEx(dual_sys, MN_BEARER_CAPABILITY_LINE_1, TRUE );  
#endif//MMI_CSP_SUPPORT
            if (MN_SS_TELE_SERVICE == g_mmiset_ss_status.basic_service.service_type)
            {
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, TRUE);
            }
            else if (MN_SS_BEARER_SERVICE == g_mmiset_ss_status.basic_service.service_type)
            {
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_DATA, TRUE);
            }
            else
            {
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, TRUE);
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_DATA, TRUE);
            }
            
            //update idle icon display just for CFU
            MAIN_SetIdleForwardState(dual_sys, TRUE);
        }			    		
        UpdateCustomerServiceProfile(dual_sys);	
        
    }
    else if (operate_type == MMISET_SS_ERASE || operate_type == MMISET_SS_DEACTIVATE)
    {
        //cfu状态更新后，通知sim卡					
        if ((g_mmiset_ss_status.ss_code == MN_SS_CFU) || 
            (g_mmiset_ss_status.ss_code == MN_SS_ALL_FORWARD))
        {
#ifdef MMI_CSP_SUPPORT 
            if (0 == g_current_line[dual_sys])
            {
                MNSIM_UpdateDivertedCallStatusEx(dual_sys, MN_BEARER_CAPABILITY_LINE_1, FALSE);          	
            }
            else if (1 == g_current_line[dual_sys])
            {
                MNSIM_UpdateDivertedCallStatusEx(dual_sys, MN_BEARER_CAPABILITY_LINE_2, FALSE);       
            }
#else
            MNSIM_UpdateDivertedCallStatusEx(dual_sys, MN_BEARER_CAPABILITY_LINE_1, FALSE);    
#endif
            if (MN_SS_TELE_SERVICE == g_mmiset_ss_status.basic_service.service_type)
            {
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, FALSE);
            }
            else if (MN_SS_BEARER_SERVICE == g_mmiset_ss_status.basic_service.service_type)
            {
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_DATA, FALSE);
            }
            else
            {
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, FALSE);
                MMISET_SetIsCfuActived(dual_sys, MMISET_SS_DATA, FALSE);
            }
            
            //update idle icon display
            MAIN_SetIdleForwardState(dual_sys, MMISET_GetIsCfuActived(dual_sys, MMISET_SS_MAX));
        }				
        //调用接口，查询出CSP的状态
        UpdateCustomerServiceProfile(dual_sys);	
    }
    else
    {
        //do nothing
    }
}
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_REGISTER_SS_CNF,APP_MN_ERASE_SS_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOperateSsCnf(
                                      MN_DUAL_SYS_E             dual_sys,
                                      uint8                     list_num,
                                      MMISET_SS_OPERATE_TYPE_E  operate_type,
                                      MN_SS_OPERATE_RESULT_E    result_type,
                                      MN_SS_ERROR_CODE_E        error_code,
                                      MN_SS_INTERROGATE_INFO_T  *info_list_ptr
                                      )
{
    uint8                   i        = 0;
    MMI_RESULT_E            result   = MMI_RESULT_TRUE;
    uint8 fail_num = 0;
    
    //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
    if (PNULL == info_list_ptr)
    {
        return  MMI_RESULT_TRUE;
    }

    //SCI_TRACE_LOW:"HandleOperateSsCnf: result_type is %d,list num is = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1213_112_2_18_2_50_5_11,(uint8*)"dd",result_type,list_num);

    if (!MMISET_IsOpenDualSysCfnrc())
    {
        switch (result_type)
        {
        case MN_SS_OPERATION_SUCC:
            for (i = 0; i < list_num; i++)
            {
                if (info_list_ptr[i].ss_status_present)
                {
                    BOOLEAN is_deactive = (BOOLEAN)((info_list_ptr[i].ss_status == MN_SS_NOT_PROVITION) 
                                                 || (info_list_ptr[i].ss_status == MN_SS_NOT_REGISTERED)
                                                 || (info_list_ptr[i].ss_status == MN_SS_NOT_ACTIVE));
                    
                    switch (operate_type)
                    {
                    case MMISET_SS_REGISTER:
                    case MMISET_SS_ACTIVATE:
                        if (is_deactive)
                        {
                            fail_num++;
                        }												
                        break;
                        
                    case MMISET_SS_ERASE:
                    case MMISET_SS_DEACTIVATE:
                        if (!is_deactive)
                        {
                            fail_num++;
                        }		
                        break;
                        
                    default:
                        break;
                    }
                }
            }

            //SCI_TRACE_LOW:"HandleOperateSsCnf fail_num=%d, list_num=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1252_112_2_18_2_50_6_12,(uint8*)"dd", fail_num, list_num);

            if (fail_num > 0 && fail_num == list_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                // close idle dialing win
                MMIAPIIDLE_CloseDialWin();
                //关闭网络连接窗口
                MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
                
                //关闭语音信箱、输入电话号码、无应答时间和输入密码界面
                MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
                MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
                MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
                MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
                return (result);
            }
            
            UpdateCfuStatuaAfterOperate(dual_sys, operate_type);
            
            switch (operate_type)
            {
            case MMISET_SS_REGISTER:
            case MMISET_SS_ACTIVATE:
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
                if (MMIMEX_CheckSetDivertCall(TRUE))
                {
                    break;
                }
#endif                    
                MMIPUB_OpenAlertSuccessWin(TXT_SET_SETTING_ACTIVATED);             
                break;

            case MMISET_SS_ERASE:
            case MMISET_SS_DEACTIVATE:
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
                if (MMIMEX_CheckCancelDivertCall(TRUE))
                {
                    break;
                }
#endif                
                MMIPUB_OpenAlertSuccessWin(TXT_SET_SETTING_CANCELED);
                break;

            default:
                MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
                break;
            }
            break;
            
        case MN_SS_OPERATION_ERROR:
            //SCI_TRACE_LOW:"HandleOperateSsCnf:error_code is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1303_112_2_18_2_50_6_13,(uint8*)"d",error_code);
            switch (error_code)
            {
            case MN_SS_ERROR_NEGATIVE_PW_CHECK:
                //提示密码错误
                MMIPUB_OpenAlertWarningWin(TXT_KL_PSW_WRONG_PSW);
                break;
                
            case MN_SS_ERROR_NUMBER_OF_PW_ATTEMPTS_VIOLATION:
                //提示密码无效
                MMIPUB_OpenAlertWarningWin(TXT_PASSWORD_INVALID);
                break;
                
            default:
                switch (operate_type)
                {
                case MMISET_SS_REGISTER:
                case MMISET_SS_ACTIVATE:
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
                    if (MMIMEX_CheckSetDivertCall(FALSE))
                    {
                        break;
                    }
#endif                        
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_SETTING_FAIL);
                    break;

                case MMISET_SS_ERASE:
                case MMISET_SS_DEACTIVATE:
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
                    if (MMIMEX_CheckCancelDivertCall(FALSE))
                    {
                        break;
                    }
#endif                                       
                    MMIPUB_OpenAlertWarningWin(TXT_SET_SETTING_CANCEL_FAIL);
                    break;

                default:
                    MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                    break;
                }
                break;
            }
            break;
            
        case MN_SS_OPERATION_REJECT:
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
            if (MMIMEX_CheckSetDivertCall(FALSE))
            {
                break;
            }
            else if(MMIMEX_CheckCancelDivertCall(FALSE))
            {
                break;
            }
#endif                              
            MMIPUB_OpenAlertWarningWin(TXT_REJECT);
            break;
            
        default:
            SCI_TRACE_LOW("HandleRegisterSsCnf: result_type is error!");
            break;
        }
    }
    // close idle dialing win
    MMIAPIIDLE_CloseDialWin();
    
    //关闭网络连接窗口
    MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
    
    //关闭语音信箱、输入电话号码、无应答时间和输入密码界面
    MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
    MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
    MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
    MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
    
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    if (MN_SS_OPERATION_SUCC == result_type)
    {
        //add for dual sim
        MMISET_CloseDualsysCFNRCWin(dual_sys, TRUE, error_code);
    }
    else
    {
        //add for dual sim
        MMISET_CloseDualsysCFNRCWin(dual_sys, FALSE, error_code);
    }
#endif
    
    return (result);
}

/*****************************************************************************/
// 	Description : get line info from nv and update to mn
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetCallLineInfo(MN_DUAL_SYS_E dual_sys)
{
#ifndef MMI_CSP_SUPPORT
    return 0;
#else
    MN_BEARER_CAPABILITY_TYPE_E selected_line = MN_BEARER_CAPABILITY_LINE_1;
    ERR_MNDATAMAG_CODE_E readline_result = ERR_MNDATAMAG_NO_ERR;
    MN_RETURN_RESULT_E line_return_value = MN_RETURN_FAILURE;
    uint32 i = 0;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return 0;
    }

    MMINV_READ(MMINV_SET_LINE_TYPE, g_current_line, line_return_value);
    
    if (MN_RETURN_SUCCESS != line_return_value)
    {
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
           g_current_line[i] = 0;
        }
        
        MMINV_WRITE(MMINV_SET_LINE_TYPE, g_current_line);
    }

    //If read successfully, will use SIM Flag, otherwise use NV Storage
    readline_result = MNSIM_GetSelectedLineInSIMEx(dual_sys, &selected_line);

    if (ERR_MNDATAMAG_NO_ERR == readline_result)
    {
        if (MN_BEARER_CAPABILITY_LINE_2 == selected_line)
        {
            g_current_line[dual_sys] = 1;
        }
        else
        {
            g_current_line[dual_sys] = 0;
        }

        MMINV_WRITE(MMINV_SET_LINE_TYPE, g_current_line);
    }

    return g_current_line[dual_sys];
#endif
}
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_INTERROGATE_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInterrogateSsCnf(
                                          DPARAM              param
                                          )
{
    APP_MN_INTERROGATE_CNF_T     interrogate_ss_ind;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    
    //SCI_ASSERT(PNULL != param);/* assert to do*/
    if (PNULL == param)
    {
        return  MMI_RESULT_TRUE;
    }
    
    interrogate_ss_ind = *((APP_MN_INTERROGATE_CNF_T *)param);
    //SCI_TRACE_LOW:"HandleInterrogateSsCnf: result_type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1451_112_2_18_2_50_6_14,(uint8*)"d",interrogate_ss_ind.result_type);
    //为了显示idle win的无条件转移状态图标而查询
    if (g_is_for_cfu_icon) 
    {
        g_is_for_cfu_icon = FALSE;

        MMIAPIPHONE_InterrogateCfuForIdle(GetCfuSimId());

        switch (interrogate_ss_ind.result_type)
        {
        case MN_SS_OPERATION_SUCC:
            GenDcsReport(interrogate_ss_ind.dual_sys, interrogate_ss_ind.list_num, interrogate_ss_ind.info_list);
            break;
            
        case MN_SS_OPERATION_ERROR:			
        case MN_SS_OPERATION_REJECT:		
            break;
            
        default:
            break;
        }
        //SCI_TRACE_LOW:"HandleInterrogateSsCnf: get the status for cfu icon"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1472_112_2_18_2_50_6_15,(uint8*)"");
        
        //关闭网络连接窗口
        if (MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID))
        {
            MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        }

        MMIAPISMS_CheckTimerMSG();
    }
    else
    {
        switch (interrogate_ss_ind.result_type)
        {
        case MN_SS_OPERATION_SUCC:
            if (g_is_need_update_cfu_icon)
            {
                g_is_need_update_cfu_icon = FALSE;
                GenDcsReport(interrogate_ss_ind.dual_sys, interrogate_ss_ind.list_num, interrogate_ss_ind.info_list);
            }
            
            if (0 != s_query_cf_num)
            {
                HandleAllQueryCF(interrogate_ss_ind.dual_sys, interrogate_ss_ind.list_num, interrogate_ss_ind);
            }
            else
            {
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
                if (MMIMEX_CheckQueryDivertCall((int32)interrogate_ss_ind.info_list[0].ss_status_present, (int32)interrogate_ss_ind.info_list[0].ss_status))
                {
                    break;
                }
#endif                                    
                GenInterrogateReport(interrogate_ss_ind.list_num, interrogate_ss_ind.info_list);
            }
            break;
            
        case MN_SS_OPERATION_ERROR:
            if (0 != s_query_cf_num)
            {
                s_query_cf_num = 0;
                //do something
            }
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
            if (MMIMEX_CheckQueryDivertCall(FALSE, -1))
            {
                break;
            }
#endif                          
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            break;
            
        case MN_SS_OPERATION_REJECT:
            if (0 != s_query_cf_num)
            {
                s_query_cf_num = 0;
                //do something
            }
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
            if (MMIMEX_CheckQueryDivertCall(FALSE, -1))
            {
                break;
            }
#endif                            
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            break;
            
        default:
            SCI_TRACE_LOW("HandleInterrogateSsCnf: result_type is error!");
            if (0 != s_query_cf_num)
            {
                s_query_cf_num = 0;
                //do something
            }	
            break;
        }
        
        // close idle dialing win
        MMIAPIIDLE_CloseDialWin();
        
        //关闭网络连接窗口
        if (0 == s_query_cf_num)
        {
            MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        }
        
        //关闭语音信箱、输入电话号码、无应答时间和输入密码界面
        MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
        MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
        MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
        MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
        
        return (result);
    }
    
    return result;
                                          
}

/*****************************************************************************/
// 	Description : generate interrogate report of cfu status
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void GenDcsReport(
                                                MN_DUAL_SYS_E dual_sys,
                                                uint8  list_num,
                                                MN_SS_INTERROGATE_INFO_T *info_list_ptr
                                                )
{
  //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
  
  //SCI_TRACE_LOW:"GenInterrogateReport:list_num = %d"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1577_112_2_18_2_50_6_16,(uint8*)"d",list_num);
  if (list_num > 0)
  {
      //the window of interrogate report
      //MMK_CreateWin((uint32 *)MMISET_SS_INTERROGATE_REPORT_WIN_TAB, PNULL);
      //generate interrogate ss service name
      //GenInterrogateServiceReport();
      
      //status
      GenDcsStatusReport(dual_sys, list_num, info_list_ptr);
  }
  else
  {
      //SCI_TRACE_LOW:"GenInterrogateReport:list_num = %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1590_112_2_18_2_50_6_17,(uint8*)"d", list_num);
  }	
}

/*****************************************************************************/
// 	Description : generate interrogate detail report of cfu status for idle win icon
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void GenDcsDetailReport(
                              MN_DUAL_SYS_E dual_sys,
                              MN_SS_INTERROGATE_INFO_T *info_list_ptr
                              )
{
    //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
    if(PNULL == info_list_ptr)
        return;

    switch (info_list_ptr->ss_status)
    {
    case MN_SS_NOT_PROVITION:
    case MN_SS_NOT_REGISTERED:
    case MN_SS_NOT_ACTIVE:
        if (MN_SS_ALL_TELESERVICES == info_list_ptr->basic_service.service_code
            ||((info_list_ptr->basic_service.service_type == MN_SS_TELE_SERVICE)&&
                ((info_list_ptr->basic_service.service_code == MN_SS_ALL_TELESERVICES)||
                (info_list_ptr->basic_service.service_code == MN_SS_ALL_SPEECH_TRANS_SERVICES)||
                (info_list_ptr->basic_service.service_code == MN_SS_TELEPHONE))))
        {
            MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, FALSE);
        }
        else if (MN_SS_ALL_DATADCS_SERVICES == info_list_ptr->basic_service.service_code
            || ((info_list_ptr->basic_service.service_type == MN_SS_BEARER_SERVICE)&&
                (info_list_ptr->basic_service.service_code|0x40&0xF8 == MN_SS_ALL_DATA_CIRCUIT_SYNC)))/*lint !e514 */
        {
            MMISET_SetIsCfuActived(dual_sys, MMISET_SS_DATA, FALSE);
        }

        MAIN_SetIdleForwardState(dual_sys, FALSE);
        break;
        
    case MN_SS_PROVITION:
    case MN_SS_ACTIVE_OPERATIVE:
    case MN_SS_ACITVE_QUIESCENT:
        if (MN_SS_ALL_TELESERVICES == info_list_ptr->basic_service.service_code
            ||((info_list_ptr->basic_service.service_type == MN_SS_TELE_SERVICE)&&
                ((info_list_ptr->basic_service.service_code == MN_SS_ALL_TELESERVICES)||
                (info_list_ptr->basic_service.service_code == MN_SS_ALL_SPEECH_TRANS_SERVICES)||
                (info_list_ptr->basic_service.service_code == MN_SS_TELEPHONE))))
        {
            MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, TRUE);
        }
        else if (MN_SS_ALL_DATADCS_SERVICES == info_list_ptr->basic_service.service_code
            || ((info_list_ptr->basic_service.service_type == MN_SS_BEARER_SERVICE)&&
                (info_list_ptr->basic_service.service_code|0x40&0xF8 == MN_SS_ALL_DATA_CIRCUIT_SYNC)))/*lint !e514 */
        {
            MMISET_SetIsCfuActived(dual_sys, MMISET_SS_DATA, TRUE);
        }

        //更新icon
        MAIN_SetIdleForwardState(dual_sys, TRUE);
        break;
    }
}

/*****************************************************************************/
// 	Description : generate interrogate report of cfu status for idle win icon
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void GenDcsStatusReport(
                              MN_DUAL_SYS_E               dual_sys,
                              uint8                       list_num,
                              MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                              )
{
    uint8 i = 0;
    
    //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
    if (PNULL == info_list_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"GenDcsStatusReport:list_num = %d,service_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1654_112_2_18_2_50_6_18,(uint8*)"dd",list_num,info_list_ptr[0].basic_service.service_code);
    //返回的数据信息只含有ss_status
    if (MN_SS_ALL_TELESERVICES == info_list_ptr[0].basic_service.service_code
        || MN_SS_ALL_DATADCS_SERVICES == info_list_ptr[0].basic_service.service_code)
    {
        if (info_list_ptr[0].ss_status_present)
        {      
            GenDcsDetailReport(dual_sys, &info_list_ptr[0]);
        }
        return;
    }
    
    //其它情况，请参照spec 04.80 page40
    for (i=0; i<list_num; i++)
    {
        BOOLEAN is_ss_audio_code = FALSE;
        BOOLEAN is_ss_data_code = FALSE;

        //SCI_TRACE_LOW:"GenDcsStatusReport:service_type = %d,service_code=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1671_112_2_18_2_50_6_19,(uint8*)"dd",info_list_ptr[i].basic_service.service_type,info_list_ptr[i].basic_service.service_code);
        if ((info_list_ptr[i].basic_service.service_type == MN_SS_TELE_SERVICE)&&
                ((info_list_ptr[i].basic_service.service_code == MN_SS_ALL_TELESERVICES)||
                 (info_list_ptr[i].basic_service.service_code == MN_SS_ALL_SPEECH_TRANS_SERVICES)||
                 (info_list_ptr[i].basic_service.service_code == MN_SS_TELEPHONE)))
        {
            is_ss_audio_code = TRUE;
        }
        else if ((info_list_ptr[i].basic_service.service_type == MN_SS_BEARER_SERVICE)&&
                 (info_list_ptr[i].basic_service.service_code|0x40&0xF8 == MN_SS_ALL_DATA_CIRCUIT_SYNC))/*lint !e514 */
        {
            is_ss_data_code = TRUE;
        }

        if (is_ss_audio_code || is_ss_data_code)
        {
            if (info_list_ptr[i].ss_status_present)
            {
                GenDcsDetailReport(dual_sys, &info_list_ptr[i]);
            }
            else
            {
                if (is_ss_audio_code)
                {
                    MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, TRUE);
                }
                else
                {
                    MMISET_SetIsCfuActived(dual_sys, MMISET_SS_DATA, TRUE);
                }
                //write call forward info to nv
                MAIN_SetIdleForwardState(dual_sys, TRUE);
            }
            return;
        }
    }	
}

/*****************************************************************************/
// 	Description : generate interrogate report
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateReport(
                                uint8 list_num,
                                MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                                )
{
#ifdef VT_SUPPORT
    BOOLEAN is_audio_interrogate = FALSE;
    BOOLEAN  is_data_interrogate = FALSE;
#endif

    //SCI_ASSERT(PNULL != info_list_ptr);  /* assert to do*/  
    if (PNULL == info_list_ptr)
    {
        return;  
    }
    
    //SCI_TRACE_LOW:"GenInterrogateReport:list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1727_112_2_18_2_50_6_20, (uint8*)"d", list_num);
    if (list_num > 0)
    {
        //the window of interrogate report
        //MMK_CreateWin((uint32 *)MMISET_SS_INTERROGATE_REPORT_WIN_TAB, PNULL);
        MMK_CloseWin(MMISET_SS_ALL_REPORT_WIN_ID);

        MMK_CreateWin((uint32*)MMISET_SS_ALL_REPORT_WIN_TAB,PNULL);
        
        //generate interrogate ss service name
        GenInterrogateServiceReport();

        if (MMK_IsOpenWin(MMISET_SET_CALL_WIN_ID))
        {
#ifdef VT_SUPPORT
			MMI_CTRL_ID_T				called_fuc_id = 0;
			MMI_CTRL_ID_T       called_subfuc_id = 0;
#if defined(MMI_GUI_STYLE_TYPICAL)
            MMI_MENU_GROUP_ID_T         group_id    =   0;
#endif
            if (MMK_IsOpenWin(MMISET_SET_CALL_DIVERT_WIN_ID))
            {
#if !defined MMI_GUI_STYLE_TYPICAL
                GUILIST_GetItemData(MMISET_CALL_DIVERT_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_DIVERT_MENU_CTRL_ID), &called_fuc_id);
                GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);
#else
                GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID, &group_id, &called_fuc_id);
                GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID, &group_id, &called_subfuc_id);
#endif

            }
            else if (MMK_IsOpenWin(MMISET_SET_CALL_BAR_WIN_ID))
            {
#if !defined MMI_GUI_STYLE_TYPICAL
                GUILIST_GetItemData(MMISET_CALL_BAR_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_BAR_MENU_CTRL_ID), &called_fuc_id);
                GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);
#else
                GUIMENU_GetId(MMISET_CALL_BAR_MENU_CTRL_ID, &group_id, &called_fuc_id);
                GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID, &group_id, &called_subfuc_id);
#endif
        	}
            else 
            {
            	called_fuc_id = MMISET_CALLWATINGT_FORM_CTRL_ID;
            }
    

            switch (called_fuc_id)
            {
            case ID_SET_DIVERTS_ALL_CALLS://无条件转移
#ifdef MMI_CSP_SUPPORT            
            case ID_SET_DIVERTS_ALL_CONDITIONAL:
#endif 
            case ID_SET_DIVERTS_WHEN_BUSY://遇忙时转移
            case ID_SET_DIVERTS_NO_ANSWER://无应答转移
            case ID_SET_DIVERTS_NOT_REACHABLE://无法到达转移
            case ID_SET_BAR_ALL_MO_CALLS://所有拨出
            case ID_SET_BAR_ALL_MT_CALLS://所有来电
            case ID_SET_BAR_MT_CALLS_ROAM://漫游时来电
            case ID_SET_BAR_MO_CALLS_INTL://国际呼出
            case ID_SET_BAR_MO_CALLS_INTL_ROAM://国际漫游拨出(本国外国际呼出)
                switch (called_subfuc_id)//用来区分不同的Service code
                {
                case ID_SET_DIVERTS_AUDIO:
                    is_audio_interrogate = TRUE;
                    break;

                case ID_SET_DIVERTS_VIDEO:
                    is_data_interrogate = TRUE;
                    break;

                case ID_SET_DIVERTS_AUDIOANDVIDEO:
                    is_audio_interrogate = TRUE;
                    is_data_interrogate = TRUE;
                    break;

                default:
                    break;
                }
                break;

            case MMISET_CALLWATINGT_FORM_CTRL_ID://呼叫等待
                is_audio_interrogate = TRUE;
                break;

            default:
                break;
            }
#endif
        }
        else
        {
#ifdef VT_SUPPORT
            is_audio_interrogate = TRUE;
            is_data_interrogate = TRUE;
#endif
        }

#ifdef VT_SUPPORT
        //status
        if (is_audio_interrogate)
        {
            GenInterrogateStatusReport(list_num, info_list_ptr);
        }

        if (is_data_interrogate)
        {
            GenInterrogateVTStatusReport(list_num, info_list_ptr);
        }
#else
        GenInterrogateStatusReport(list_num, info_list_ptr);
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"GenInterrogateReport:list_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1826_112_2_18_2_50_7_21,(uint8*)"d", list_num);
    }
}

/*****************************************************************************/
// 	Description : generate interrogate report of ss status name
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateServiceReport(void)
{
    MMI_STRING_T        name_str = {0};
    
    //SCI_MEMSET(&name_str,0,sizeof(MMI_STRING_T));
    switch (g_mmiset_ss_status.ss_code)
    {
    case MN_SS_CFU://无条件转移
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_CFU");
        MMI_GetLabelTextByLang(TXT_SET_DIVERTS_ALL_CALLS,&name_str);
        break;
        
    case MN_SS_CFB://遇忙时转移
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_CFB");
        MMI_GetLabelTextByLang(TXT_SET_DIVERTS_WHEN_BUSY,&name_str);
        break;
        
    case MN_SS_CFNRY://无应答转移
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_CFNRY");
        MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NO_ANSWER,&name_str);
        break;
        
    case MN_SS_CFNRC://无法到达转移
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_CFNRC");
        MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NOT_REACHABLE,&name_str);
        break;
        
    case MN_SS_CW://呼叫等待
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_CW");
        MMI_GetLabelTextByLang(TXT_SET_CALL_WAITING,&name_str);
        break;
        
    case MN_SS_BAOC://所有拨出
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_BAOC");
        MMI_GetLabelTextByLang(TXT_SET_BAR_ALL_MO_CALLS,&name_str);
        break;
        
    case MN_SS_BAIC://所有来电
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_BAIC");
        MMI_GetLabelTextByLang(TXT_SET_BAR_ALL_MT_CALLS,&name_str);
        break;
        
    case MN_SS_BICROAM://漫游时来电
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_BICROAM");
        MMI_GetLabelTextByLang(TXT_SET_BAR_MT_CALLS_ROAM,&name_str);
        break;
        
    case MN_SS_BOIC://国际呼出
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_BOIC");
        MMI_GetLabelTextByLang(TXT_SET_BAR_MO_CALLS_INTL,&name_str);
        break;
        
    case MN_SS_BOIC_EXHC://国际漫游拨出(本国外国际呼出)
        //SCI_TRACE_LOW("GenInterrogateServiceReport: ss_code is MN_SS_BOIC_EXHC");
        MMI_GetLabelTextByLang(TXT_SET_BAR_MO_CALLS_INTL_ROAM,&name_str);
        break;
       
    case MN_SS_CLIP://显示本机号码
        MMI_GetLabelTextByLang(TXT_SET_CLIP_ID,&name_str);
        break;

    case MN_SS_CLIR://隐藏本机号码
        MMI_GetLabelTextByLang(TXT_SET_HIDE_ID,&name_str);
        break;

    case MN_SS_COLP://显示实际接听号码
        MMI_GetLabelTextByLang(TXT_SET_COLP_ID,&name_str);
        break;

    case MN_SS_COLR://隐藏实际接通号码
        MMI_GetLabelTextByLang(TXT_SET_COLR_ID,&name_str);
        break;

    default:
        break;
    }
    
    //set ss interrogate repoort about name
    //GUILABEL_SetText(MMISET_SS_INTERROGATE_NAME_CTRL_ID,&name_str,FALSE);
    AddStrToReportText(name_str, CONTENT_TYPE_SERVIEC_NAME);
}

/*****************************************************************************/
// 	Description : generate interrogate report of ss status
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateStatusReport(
                                      uint8 list_num,
                                      MN_SS_INTERROGATE_INFO_T    *info_list_ptr
                                      )
{
    uint8 i = 0;
    MMI_STRING_T status_str = {0};
    
    //SCI_MEMSET(&status_str,0,sizeof(MMI_STRING_T));
    
    //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
    if (PNULL == info_list_ptr)
    {
        return;  
    }
  
    //SCI_TRACE_LOW:"GenInterrogateStatusReport:list_num = %d,service_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1934_112_2_18_2_50_7_22, (uint8*)"dd", list_num, info_list_ptr[0].basic_service.service_code);
    
#ifdef MMI_DUALMODE_ENABLE
    MMI_GetLabelTextByLang(TXT_CALL_AUDIO, &status_str);
    AddStrToReportText(status_str, CONTENT_TYPE_CALL_CLASS);
#endif
    
    //返回的数据信息只含有ss_status
    if (MN_SS_ALL_TELESERVICES == info_list_ptr[0].basic_service.service_code)
    {
        if (info_list_ptr[0].ss_status_present)
        {
            GenInterrogateDetailReport(&info_list_ptr[0]);
        }
        else
        {
            //SCI_TRACE_LOW:"GenInterrogateStatusReport: info_list_ptr[0].ss_status_present is 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1950_112_2_18_2_50_7_23,(uint8*)"");
            MMI_GetLabelTextByLang(TXT_NO_ACTIVATION,&status_str);
            
            //set ss interrogate report about status
            //GUILABEL_SetText(MMISET_SS_INTERROGATE_STATUS_CTRL_ID,&status_str,FALSE);
            AddStrToReportText(status_str, CONTENT_TYPE_SERVICE_STATUSE);
        }
        return;
    }
    //其它情况，请参照spec 04.80 page40
    for (i = 0; i < list_num; i++)
    {
        if ((info_list_ptr[i].basic_service.service_type == MN_SS_TELE_SERVICE)  
         && ((info_list_ptr[i].basic_service.service_code == MN_SS_ALL_SPEECH_TRANS_SERVICES)
#ifdef MMI_CSP_SUPPORT
		 || (info_list_ptr[i].basic_service.service_code == MN_SS_AUXILIARY_TELEPHONY)
#endif		 
		 ))
        {
            if (info_list_ptr[i].ss_status_present)
            {
                GenInterrogateDetailReport(&info_list_ptr[i]);
            }
            else
            {
                //SCI_TRACE_LOW:"GenInterrogateStatusReport: info_list_ptr[i].ss_status_present is 0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1974_112_2_18_2_50_7_24,(uint8*)"");
                MMI_GetLabelTextByLang(TXT_ACTIVE_OPERATIVE,&status_str);
                
                //set ss interrogate repoort about status
                //GUILABEL_SetText(MMISET_SS_INTERROGATE_STATUS_CTRL_ID,&status_str,FALSE);
                AddStrToReportText(status_str, CONTENT_TYPE_SERVICE_STATUSE);
            }
            return;
        }
    }
    
    //如果查询返回的type和code，不是我们在支持的情况，直接提示用户没有启动
    if (i == list_num)
    {
        MMI_GetLabelTextByLang(TXT_NO_REGISTERED,&status_str);
        
        //SCI_TRACE_LOW:"GenInterrogateStatusReport: no value"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_1990_112_2_18_2_50_7_25,(uint8*)"");
        //set ss interrogate report about status
        //GUILABEL_SetText(MMISET_SS_INTERROGATE_STATUS_CTRL_ID,&status_str,FALSE);
        AddStrToReportText(status_str, CONTENT_TYPE_SERVICE_STATUSE);
        return;
    }
}

#ifdef MMI_DUALMODE_ENABLE
/*****************************************************************************/
// 	Description : generate interrogate report of ss status for VT
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateVTStatusReport(
                                        uint8 list_num,
                                        MN_SS_INTERROGATE_INFO_T *info_list_ptr
                                        )
{
    uint8 i = 0;
    MMI_STRING_T status_str = {0};
    
    //SCI_MEMSET(&status_str,0,sizeof(MMI_STRING_T));
    
    //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
    if(PNULL == info_list_ptr)
        return;  

    //SCI_TRACE_LOW:"GenInterrogateVTReport:list_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2017_112_2_18_2_50_7_26,(uint8*)"d",list_num);
    
    MMI_GetLabelTextByLang(TXT_CALL_VIDEO, &status_str);
    AddStrToReportText(status_str, CONTENT_TYPE_CALL_CLASS);
    
    //返回的数据信息只含有ss_status
    if (MN_SS_ALL_DATADCS_SERVICES == info_list_ptr[0].basic_service.service_code)
    {
        if(info_list_ptr[0].ss_status_present)
        {
            GenInterrogateDetailReport(&info_list_ptr[0]);
        }
        else
        {
            //SCI_TRACE_LOW:"GenInterrogateVTStatusReport: info_list_ptr[0].ss_status_present is 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2031_112_2_18_2_50_7_27,(uint8*)"");
            MMI_GetLabelTextByLang(TXT_NO_ACTIVATION,&status_str);
            
            //set ss interrogate report about status
            //GUILABEL_SetText(MMISET_SS_INTERROGATE_STATUS_CTRL_ID,&status_str,FALSE);
            AddStrToReportText(status_str, CONTENT_TYPE_SERVICE_STATUSE);
        }
        return;
    }
    
    //其它情况，请参照spec 04.80 page40
    for (i=0; i<list_num; i++)
    {
        //SCI_TRACE_LOW:"GenInterrogateVTStatusReport: i=%d, type=%d,code=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2044_112_2_18_2_50_7_28,(uint8*)"ddd", i, info_list_ptr[i].basic_service.service_type,info_list_ptr[i].basic_service.service_code);
        if ((info_list_ptr[i].basic_service.service_type == MN_SS_BEARER_SERVICE)&&
            (info_list_ptr[i].basic_service.service_code|0x40&0xF8 == MN_SS_ALL_DATA_CIRCUIT_SYNC)) /*lint !e514 */
        {
            if (info_list_ptr[i].ss_status_present)
            {
                GenInterrogateDetailReport(&info_list_ptr[i]);
            }
            else
            {
                //SCI_TRACE_LOW:"GenInterrogateVTStatusReport: info_list_ptr[i].ss_status_present is 0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2054_112_2_18_2_50_7_29,(uint8*)"");
                MMI_GetLabelTextByLang(TXT_ACTIVE_OPERATIVE,&status_str);
                
                //set ss interrogate repoort about status
                //GUILABEL_SetText(MMISET_SS_INTERROGATE_STATUS_CTRL_ID,&status_str,FALSE);
                AddStrToReportText(status_str, CONTENT_TYPE_SERVICE_STATUSE);
            }
            return;
        }
    }
    
    //如果查询返回的type和code，不是我们在支持的情况，直接提示用户没有启动
    if ( i == list_num)
    {
        MMI_GetLabelTextByLang(TXT_NO_REGISTERED,&status_str);
        
        //SCI_TRACE_LOW:"GenInterrogateVTStatusReport: no value"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2070_112_2_18_2_50_7_30,(uint8*)"");
        //set ss interrogate report about status
        //GUILABEL_SetText(MMISET_SS_INTERROGATE_STATUS_CTRL_ID,&status_str,FALSE);
        AddStrToReportText(status_str, CONTENT_TYPE_SERVICE_STATUSE);
        return;
    }
}
#endif

/*****************************************************************************/
// 	Description : set clir type by the hide id menu
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetClirType(MN_DUAL_SYS_E dual_sys, MN_CALL_CLIR_E clir_type)
{
    MN_CALL_CLIR_E clir_type_array[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    //SCI_ASSERT(dual_sys < MMI_DUAL_SYS_MAX);/* assert to do*/
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return;  

    //get the clir type from nv
    MMINV_READ(MMINV_SET_CLIR_TYPE, clir_type_array, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        uint32 i = 0;

        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            clir_type_array[i] = MN_CALL_CLIR_NETWORK_DECIDE;
        }
    }

    clir_type_array[dual_sys] = clir_type;
    
    MMINV_WRITE(MMINV_SET_CLIR_TYPE, clir_type_array);
}

/*****************************************************************************/
// 	Description : generate interrogate detail report of ss status
//	Global resource dependence : 
//  Author: Jassmine
//	Note:以下是关于SS 的注释:
//ss_status主要由四个状态比特（一个字节的低4位）构成Q P R A，根据Q P R A取0或者1构成不同的ss_status值。
//一: 对于呼叫限制，呼叫等待，号码显示/隐藏(number presentation)状态有提供/未提供，开启/关闭，没有注册和注销。 
//因此需要根据下面的要求进行提示：
// 1.当ss_status=0，1，2，3 这种P=0，表示not provided ,提示“not provided”/“未提供”
// 2.当ss_status=4，6，c, e这种P=1，A=0，表示provided, not activated,提示“provided，not activated”/“已提供，未开启”
// 3.当ss_status=5，7，d，f这种P=1，A=1,表示provided, activated,提示“activated”/“已开启”
// 二: 对于呼叫转移状态有提供/未提供，开启/关闭，注册/注销。
// 1.当ss_status=0，1，2，3 这种P=0，表示not provided ,提示“not provided”/“未提供”
// 2.当ss_status=4，5，c,d 这种P=1，R=0，A=0/1，表示provided,not registered,not activated,提示“provided, not registered”/“已提供，未注册”
// 3.当ss_status=6，e这种P=1，R=1，A=0，表示provided, registered,not activated, 提示“not activated”/“未开启”
// 4.当ss_status=7，f这种P=1，R=1，A=1,表示provided, registered,activated,提示“activated”/“已开启”
/*****************************************************************************/
LOCAL void GenInterrogateDetailReport(
                                      MN_SS_INTERROGATE_INFO_T  *info_list_ptr
                                      )
{
    MMI_STRING_T        status_str = {0};
    
    //SCI_TRACE_LOW:"GenInterrogateDetailReport:ss_code is 0x04%x, ss_status is 0x04%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2130_112_2_18_2_50_7_31,(uint8*)"dd", g_mmiset_ss_status.ss_code, info_list_ptr->ss_status);
    
    /*对于CLIP,CLIR,COLP,COLR这几种业务只有provision和 unprovision两种状态，
    对于网络返回的状态是4，5，6，7（bit3：provision bit），
    都应该显示类似“已提供”的提示语*/
    if (MN_SS_CLIP == g_mmiset_ss_status.ss_code
        || MN_SS_CLIR == g_mmiset_ss_status.ss_code
        || MN_SS_COLP == g_mmiset_ss_status.ss_code
        || MN_SS_COLR == g_mmiset_ss_status.ss_code)
    {
        if ((uint32)info_list_ptr->ss_status & (uint32)MN_SS_NOT_REGISTERED)
        {
            MMI_GetLabelTextByLang(TXT_PROVISION,&status_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_NOT_PROVISION,&status_str);
        }
    }
    else if (MN_SS_ALL_FORWARD == g_mmiset_ss_status.ss_code
             || MN_SS_CFU == g_mmiset_ss_status.ss_code
             || MN_SS_ALL_COND_FORWARD == g_mmiset_ss_status.ss_code
             || MN_SS_CFB == g_mmiset_ss_status.ss_code
             || MN_SS_CFNRY == g_mmiset_ss_status.ss_code
             || MN_SS_CFNRC == g_mmiset_ss_status.ss_code)   //for call forword
    {
        switch (info_list_ptr->ss_status)
        {
        case MN_SS_NOT_PROVITION:
            MMI_GetLabelTextByLang(TXT_NOT_PROVISION,&status_str);
            break;
            
        case MN_SS_NOT_ACTIVE:
            //SCI_TRACE_LOW("GenInterrogateDetailReport: info_list_ptr->ss_status is MN_SS_NOT_ACTIVE");
            MMI_GetLabelTextByLang(TXT_PROVISION_NOT_ACTIVED,&status_str);
            break;
        
        case MN_SS_NOT_REGISTERED:
        case MN_SS_PROVITION:
            MMI_GetLabelTextByLang(TXT_NO_REGISTERED,&status_str);
        
            if (info_list_ptr->forward_num_present)
            {              
                GenInterrogateTelenumReport(info_list_ptr);
            }
        
            if (info_list_ptr->no_reply_time_present)
            {
                GenInterrogateTimeReport(info_list_ptr);
            }
            break;
        
        case MN_SS_ACTIVE_OPERATIVE:
        case MN_SS_ACITVE_QUIESCENT:
            MMI_GetLabelTextByLang(TXT_ACTIVE_OPERATIVE,&status_str);
        
            if (info_list_ptr->forward_num_present)
            {              
                GenInterrogateTelenumReport(info_list_ptr);
            }
        
            if (info_list_ptr->no_reply_time_present)
            {
                GenInterrogateTimeReport(info_list_ptr);
            }
            break;
        }    
    }
    else
    {
        switch (info_list_ptr->ss_status)
        {
        case MN_SS_NOT_PROVITION:
            MMI_GetLabelTextByLang(TXT_NOT_PROVISION,&status_str);
            break;
            
        case MN_SS_NOT_REGISTERED:
        case MN_SS_NOT_ACTIVE:
            MMI_GetLabelTextByLang(TXT_PROVISION_NOT_ACTIVED,&status_str);
            break;
        
        case MN_SS_PROVITION:
        case MN_SS_ACTIVE_OPERATIVE:
        case MN_SS_ACITVE_QUIESCENT:
            MMI_GetLabelTextByLang(TXT_ACTIVE_OPERATIVE,&status_str);
        
            if (info_list_ptr->forward_num_present)
            {              
                GenInterrogateTelenumReport(info_list_ptr);
            }
        
            if (info_list_ptr->no_reply_time_present)
            {
                GenInterrogateTimeReport(info_list_ptr);
            }
            break;
        }
    }    

    //set ss interrogate report about status
    //GUILABEL_SetText(MMISET_SS_INTERROGATE_STATUS_CTRL_ID,&status_str,FALSE);
    AddStrToReportText(status_str, CONTENT_TYPE_SERVICE_STATUSE);
}

/*****************************************************************************/
// 	Description : generate interrogate detail report of call forwarding phone number
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateTelenumReport(
                                       MN_SS_INTERROGATE_INFO_T  *info_list_ptr
                                       )
{
    uint8           tele_num[MMISET_PHONE_NUMBER_MAX_LEN + 2] = {0};
    wchar           tele_num_wstr[MMISET_PHONE_NUMBER_MAX_LEN + 2] = {0};
    MMI_STRING_T    divert_num_str = {0};
    
    //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
    //SCI_MEMSET(&divert_num_str,0,sizeof(MMI_STRING_T));
    if (PNULL == info_list_ptr)
    {
        return;  
    }

    //get number
    MMIAPICOM_GenDispNumber(info_list_ptr->forward_num.number_type,
                            (uint8)info_list_ptr->forward_num.num_len,
                            info_list_ptr->forward_num.party_num, 
                            tele_num,
                            MMISET_PHONE_NUMBER_MAX_LEN + 2);
    
    divert_num_str.wstr_len = (uint16)strlen((char*)tele_num);
    divert_num_str.wstr_len = MIN(divert_num_str.wstr_len, MMISET_PHONE_NUMBER_MAX_LEN);
    divert_num_str.wstr_ptr = tele_num_wstr;
    MMI_STRNTOWSTR(tele_num_wstr, MMISET_PHONE_NUMBER_MAX_LEN, (uint8 *)tele_num, divert_num_str.wstr_len, divert_num_str.wstr_len);
    //divert_num_str.is_ucs2 = FALSE;
    
    //set ss interrogate repoort about call forwarding number
    //GUILABEL_SetText(MMISET_SS_INTERROGATE_NUMBER_CTRL_ID,&divert_num_str,FALSE);
    AddStrToReportText(divert_num_str, CONTENT_TYPE_SERVICE_STATUSE);
}

/*****************************************************************************/
// 	Description : generate interrogate detail report of no answer time
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GenInterrogateTimeReport(
                                    MN_SS_INTERROGATE_INFO_T  *info_list_ptr
                                    )
{
    uint8           time_str[MMISET_NO_ANSWER_TIME_MAX_LEN + 1] = {0},time_str_len = 0;
    //uint8           time_temp_str[MMISET_NO_ANSWER_TIME_MAX_LEN] = {0},time_temp_str_len = 0;
    wchar           time[MMISET_NO_ANSWER_TIME_MAX_LEN + 1] = {0};
    MMI_STRING_T    no_answer = {0},no_answer_time_str = {0},second_str = {0};
    
    //SCI_ASSERT(PNULL != info_list_ptr);/* assert to do*/
    if(PNULL == info_list_ptr)
        return;  
 
    //SCI_MEMSET(&no_answer,0,sizeof(MMI_STRING_T));
    //SCI_MEMSET(&no_answer_time_str,0,sizeof(MMI_STRING_T));
    //SCI_MEMSET(&second_str,0,sizeof(MMI_STRING_T));
    
    MMI_GetLabelTextByLang(TXT_NO_REPLY_TIME,&no_answer);
    MMI_GetLabelTextByLang(TXT_SECOND,&second_str);

    sprintf((char*)time_str, "%d",info_list_ptr->no_reply_time);
    time_str_len = (uint8)strlen((char*)time_str);
    
    MMI_WSTRNCPY( time, MMISET_NO_ANSWER_TIME_MAX_LEN, no_answer.wstr_ptr,no_answer.wstr_len, no_answer.wstr_len );
    MMI_STRNTOWSTR( time + no_answer.wstr_len,MMISET_NO_ANSWER_TIME_MAX_LEN, (uint8 *)time_str, MMISET_NO_ANSWER_TIME_MAX_LEN, time_str_len );
    MMI_WSTRNCPY(time + no_answer.wstr_len + time_str_len,MMISET_NO_ANSWER_TIME_MAX_LEN,second_str.wstr_ptr,second_str.wstr_len,second_str.wstr_len);

    no_answer_time_str.wstr_len = (uint16)(no_answer.wstr_len + time_str_len + second_str.wstr_len);
    
//     if (no_answer.is_ucs2)
//     {
//         sprintf((char*)time_str, "%d",info_list_ptr->no_reply_time);
//         time_str_len = (uint8)strlen((char*)time_str);
//         
//         //change no reply time to ucs2
//         MMIAPICOM_NumStrToUCS2(time_str,time_str_len,time_temp_str,sizeof(time_temp_str));
//         time_temp_str_len = (uint8)(time_str_len << 1);
//         
//         MMI_MEMCPY(time,MMISET_NO_ANSWER_TIME_MAX_LEN,no_answer.str_ptr,no_answer.length,no_answer.length);
//         MMI_MEMCPY(time + no_answer.length,MMISET_NO_ANSWER_TIME_MAX_LEN,time_temp_str,MMISET_NO_ANSWER_TIME_MAX_LEN,time_temp_str_len)
//             MMI_MEMCPY(time + no_answer.length + time_temp_str_len,MMISET_NO_ANSWER_TIME_MAX_LEN,second_str.str_ptr,second_str.length,second_str.length);
//         
//         no_answer_time_str.is_ucs2 = TRUE;
//         no_answer_time_str.length = (uint16)(no_answer.length + time_temp_str_len + second_str.length);
//     }
//     else
//     {
//         sprintf((char*)time, "%s:%d%s",(char *)no_answer.str_ptr,info_list_ptr->no_reply_time, (char *)second_str.str_ptr);
//         
//         no_answer_time_str.is_ucs2 = FALSE;
//         no_answer_time_str.length = (uint16)strlen((char*)time);
//     }
    no_answer_time_str.wstr_ptr = time;
    
    //set ss interrogate repoort about call forwarding no answer time
    //GUILABEL_SetText(MMISET_SS_INTERROGATE_TIME_CTRL_ID,&no_answer_time_str,FALSE);
    AddStrToReportText(no_answer_time_str, CONTENT_TYPE_SERVICE_STATUSE);
}

/*****************************************************************************/
// 	Description : call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CallForwarding(
                                  MN_DUAL_SYS_E     dual_sys,
                                  MMI_STRING_T    *voice_number_ptr
                                  )
{
    MMI_CTRL_ID_T				called_fuc_id;
#ifdef VT_SUPPORT
    MMI_CTRL_ID_T               called_subfuc_id;
#endif
#if defined(MMI_GUI_STYLE_TYPICAL)
    MMI_MENU_GROUP_ID_T         group_id    =   0;
#endif
    //GUIMENU_GetId(MMISET_GetDivertBarWatingHideidCtrlId(),&group_id,&menu_id);
    if (MMK_IsOpenWin(MMISET_SET_CALL_DIVERT_WIN_ID))
    {
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(MMISET_CALL_DIVERT_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_DIVERT_MENU_CTRL_ID), &called_fuc_id);
#ifdef VT_SUPPORT
        GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);
#endif
#else
    	GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID, &group_id, &called_fuc_id);
#ifdef VT_SUPPORT
        GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID, &group_id, &called_subfuc_id);
#endif
#endif
    }
    else if (MMK_IsOpenWin(MMISET_SET_CALL_BAR_WIN_ID))
    {
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(MMISET_CALL_BAR_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_BAR_MENU_CTRL_ID), &called_fuc_id);
#ifdef VT_SUPPORT
        GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);
#endif
#else
        GUIMENU_GetId(MMISET_CALL_BAR_MENU_CTRL_ID, &group_id, &called_fuc_id);
#ifdef VT_SUPPORT
        GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID, &group_id, &called_subfuc_id);
#endif
#endif
    }
    else
    {
    	called_fuc_id = MMISET_CALLWATINGT_FORM_CTRL_ID;
    }

        
    //set ss param,include ss_code and sevice_type,service_code
    SetSsParam(dual_sys, FALSE);
    
/*#ifdef VT_SUPPORT
    if (ID_SET_SS_OPEN == menu_id_2)//@zhaohui modify
#else
    if (ID_SET_SS_OPEN == menu_id)
#endif*/
    {
        switch (called_fuc_id)
        {
        case ID_SET_DIVERTS_ALL_CALLS://无条件转移
#ifdef MMI_CSP_SUPPORT        
        case ID_SET_DIVERTS_ALL_CONDITIONAL:
#endif     
        case ID_SET_DIVERTS_WHEN_BUSY://遇忙时转移
        case ID_SET_DIVERTS_NOT_REACHABLE://无法到达转移
            //start call forwarding
            MMISET_StartCallForwarding(dual_sys, voice_number_ptr);
            break;
            
        case ID_SET_DIVERTS_NO_ANSWER://无应答转移
            MMK_CreateWin((uint32*)MMISET_SET_NO_ANSWER_TIME_WIN_TAB,(ADD_DATA)dual_sys);
            break;
            
        default:
            break;
        }
    }

}

/*****************************************************************************/
// 	Description : set ss param,include ss_code and sevice_type,service_code
//	Global resource dependence : 
//  Author: Jassmine
//	Note:  @$discuss 未考虑双卡
/*****************************************************************************/
#ifdef VT_SUPPORT
LOCAL void SetSsParam(
                      MN_DUAL_SYS_E dual_sys,
                      BOOLEAN   is_interrogate
                      )
{
#if defined(MMI_GUI_STYLE_TYPICAL)
    MMI_MENU_GROUP_ID_T         group_id    =   0;
#endif
    //MMI_MENU_ID_T               menu_id     =   0;
    MMI_CTRL_ID_T				called_fuc_id = 0;
    MMI_CTRL_ID_T       called_subfuc_id = 0;
    //GUIMENU_GetId(MMISET_GetDivertBarWatingHideidCtrlId(),&group_id,&menu_id);
    if(MMK_IsOpenWin(MMISET_SET_CALL_DIVERT_WIN_ID))
    {
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(MMISET_CALL_DIVERT_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_DIVERT_MENU_CTRL_ID), &called_fuc_id);//bug339444

        GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);

#else
        GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID,&group_id,&called_fuc_id);

        GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID,&group_id,&called_subfuc_id);

#endif

    }
    else if(MMK_IsOpenWin(MMISET_SET_CALL_BAR_WIN_ID))
    {
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(MMISET_CALL_BAR_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_BAR_MENU_CTRL_ID), &called_fuc_id);

        GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);

#else
        GUIMENU_GetId(MMISET_CALL_BAR_MENU_CTRL_ID,&group_id,&called_fuc_id);

        GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID,&group_id,&called_subfuc_id);

#endif
	}
    else 
    {
    	called_fuc_id = MMISET_CALLWATINGT_FORM_CTRL_ID;
    }

    g_mmiset_ss_status.dual_sys = dual_sys;
    
    switch (called_fuc_id)
    {
    case ID_SET_DIVERTS_ALL_CALLS://无条件转移
        switch(called_subfuc_id)//用来区分不同的Service code
        {
#ifdef MMI_CSP_SUPPORT    
        case ID_SET_DIVERTS_AUDIO:
            if ( 0 == g_current_line[dual_sys] )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            }
            else if ( 1 == g_current_line[dual_sys] )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_AUXILIARY_TELEPHONY;
            }
#else
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            }
#endif
            break;

        case ID_SET_DIVERTS_VIDEO:
#ifdef MMI_CSP_SUPPORT 
            if ( 0 == g_current_line[dual_sys] )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            }
            else if ( 1 == g_current_line[dual_sys] )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            }
#else
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            }
#endif      
            break;

        case ID_SET_DIVERTS_AUDIOANDVIDEO:
#ifdef MMI_CSP_SUPPORT 
            if ( 0 == g_current_line[dual_sys] )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            }
            else if ( 1 == g_current_line[dual_sys] )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            }
#else
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            }

#endif   
            break;

        default:
            break;
        }
        break;
        
    case ID_SET_DIVERTS_WHEN_BUSY://遇忙时转移
        //SCI_TRACE_LOW("SetSsParam: call forwarding on mobile subscriber busy");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_CFB;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_CFB;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_CFB;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;			
        default:
            break;
        }
        break;
        
    case ID_SET_DIVERTS_NO_ANSWER://无应答转移
        //SCI_TRACE_LOW("SetSsParam: call forwarding on no reply");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_CFNRY;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_CFNRY;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_CFNRY;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            break;
            }       
        break;
            
    case ID_SET_DIVERTS_NOT_REACHABLE://无法到达转移
        //SCI_TRACE_LOW("SetSsParam: call forwarding on mobile subscriber not reachable");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_CFNRC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_CFNRC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_CFNRC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            break;
        }   
        break;
            
    case MMISET_CALLWATINGT_FORM_CTRL_ID://呼叫等待
        //SCI_TRACE_LOW("SetSsParam: call waiting");
        g_mmiset_ss_status.ss_code = MN_SS_CW;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
        //g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        break;
        
    case ID_SET_BAR_ALL_MO_CALLS://所有拨出
        //SCI_TRACE_LOW("SetSsParam: call bar all mo calls");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_BAOC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BAOC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BAOC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            break;
        }    
        break;
                    
    case ID_SET_BAR_ALL_MT_CALLS://所有来电
        //SCI_TRACE_LOW("SetSsParam: call bar all mt calls");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_BAIC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BAIC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BAIC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            break;
        }    
        break;
            
    case ID_SET_BAR_MT_CALLS_ROAM://漫游时来电
        //SCI_TRACE_LOW("SetSsParam: call bar all mt calls in roam");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_BICROAM;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BICROAM;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BICROAM;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            break;
        }            
        break;
                
    case ID_SET_BAR_MO_CALLS_INTL://国际呼出
        //SCI_TRACE_LOW("SetSsParam: call bar all mo calls in roam");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_BOIC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BOIC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BOIC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            break;
        }         
        break;
            
    case ID_SET_BAR_MO_CALLS_INTL_ROAM://国际漫游拨出(本国外国际呼出)
        //SCI_TRACE_LOW("SetSsParam: call bar all mo calls in international roam");
        switch(called_subfuc_id)//用来区分不同的Service code
        {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.ss_code = MN_SS_BOIC_EXHC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BOIC_EXHC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_DATADCS_SERVICES;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.ss_code = MN_SS_BOIC_EXHC;
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            break;
        }         
        break;
        
    default:
        break;
    }
#if 1//bug339444
    SCI_TRACE_LOW("SetSsParam: ID_SET_DIVERTS_ALL_CALLS = %d ,ID_SET_DIVERTS_AUDIO = %d",ID_SET_DIVERTS_ALL_CALLS,ID_SET_DIVERTS_AUDIO);
    SCI_TRACE_LOW("SetSsParam: called_fuc_id is %d a,called_subfuc_id is %d, ss code = %d, service_type =%d, service_code =%d",called_fuc_id,called_subfuc_id,g_mmiset_ss_status.ss_code,g_mmiset_ss_status.basic_service.service_type,g_mmiset_ss_status.basic_service.service_code);
#endif
}
#else
LOCAL void SetSsParam(MN_DUAL_SYS_E dual_sys, BOOLEAN is_interrogate)
{
    MMI_CTRL_ID_T	called_fuc_id = MMISET_SET_CALL_DIVERT_WIN_ID;
#if defined(MMI_GUI_STYLE_TYPICAL)
    MMI_MENU_GROUP_ID_T  group_id = 0;
#endif

    if (MMK_IsOpenWin(MMISET_SET_CALL_DIVERT_WIN_ID))
    {
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(MMISET_CALL_DIVERT_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_DIVERT_MENU_CTRL_ID), &called_fuc_id);
#else
    	GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID, &group_id, &called_fuc_id);
#endif
        // GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID,&group_id,&called_subfuc_id);

    }
    else if (MMK_IsOpenWin(MMISET_SET_CALL_BAR_WIN_ID))
    {
#if !defined MMI_GUI_STYLE_TYPICAL
        GUILIST_GetItemData(MMISET_CALL_BAR_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_BAR_MENU_CTRL_ID), &called_fuc_id);
#else
        GUIMENU_GetId(MMISET_CALL_BAR_MENU_CTRL_ID,&group_id,&called_fuc_id);
#endif
        // GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID,&group_id,&called_subfuc_id);
    }
    else 
    {
    	called_fuc_id = MMISET_CALLWATINGT_FORM_CTRL_ID;
    }
    
    g_mmiset_ss_status.dual_sys = dual_sys;

    //call forwarding: type and code set
    if ((ID_SET_DIVERTS_ALL_CALLS== called_fuc_id)
#ifdef MMI_CSP_SUPPORT        
        ||(ID_SET_DIVERTS_ALL_CONDITIONAL == called_fuc_id)
#endif     
        ||(ID_SET_DIVERTS_WHEN_BUSY == called_fuc_id)
        ||(ID_SET_DIVERTS_NO_ANSWER == called_fuc_id)
        ||(ID_SET_DIVERTS_NOT_REACHABLE == called_fuc_id)
        ||(ID_SET_BAR_ALL_MO_CALLS == called_fuc_id)
        ||(ID_SET_BAR_ALL_MT_CALLS == called_fuc_id)
        ||(ID_SET_BAR_MT_CALLS_ROAM == called_fuc_id)
        ||(ID_SET_BAR_MO_CALLS_INTL == called_fuc_id)
        ||(ID_SET_BAR_MO_CALLS_INTL_ROAM == called_fuc_id))
    {
#ifdef MMI_CSP_SUPPORT  
        if (0 == g_current_line[dual_sys])
        {
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        }
        else if (1 == g_current_line[dual_sys])
        {
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_AUXILIARY_TELEPHONY;
        }
#else
        {
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        }
#endif 
    }

    switch (called_fuc_id)
    {
    case ID_SET_DIVERTS_ALL_CALLS://无条件转移
        g_mmiset_ss_status.ss_code = MN_SS_CFU;
        break;
        
#ifdef MMI_CSP_SUPPORT
    case ID_SET_DIVERTS_ALL_CONDITIONAL://?????
        g_mmiset_ss_status.ss_code = MN_SS_ALL_COND_FORWARD;
        break;        
#endif
        
    case ID_SET_DIVERTS_WHEN_BUSY://遇忙时转移
        //SCI_TRACE_LOW("SetSsParam: call forwarding on mobile subscriber busy");
        g_mmiset_ss_status.ss_code = MN_SS_CFB;
        break;
        
    case ID_SET_DIVERTS_NO_ANSWER://无应答转移
        //SCI_TRACE_LOW("SetSsParam: call forwarding on no reply");
        g_mmiset_ss_status.ss_code = MN_SS_CFNRY;
        break;
        
    case ID_SET_DIVERTS_NOT_REACHABLE://无法到达转移
        //SCI_TRACE_LOW("SetSsParam: call forwarding on mobile subscriber not reachable");
        g_mmiset_ss_status.ss_code = MN_SS_CFNRC;
        break;
        
    case MMISET_CALLWATINGT_FORM_CTRL_ID://呼叫等待
        //SCI_TRACE_LOW("SetSsParam: call waiting");
        g_mmiset_ss_status.ss_code = MN_SS_CW;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
        //g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        //g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        break;
        
    case ID_SET_BAR_ALL_MO_CALLS://所有拨出
        //SCI_TRACE_LOW("SetSsParam: call bar all mo calls");
        g_mmiset_ss_status.ss_code = MN_SS_BAOC;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        break;
        
    case ID_SET_BAR_ALL_MT_CALLS://所有来电
        //SCI_TRACE_LOW("SetSsParam: call bar all mt calls");
        g_mmiset_ss_status.ss_code = MN_SS_BAIC;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        break;
        
    case ID_SET_BAR_MT_CALLS_ROAM://漫游时来电
        //SCI_TRACE_LOW("SetSsParam: call bar all mt calls in roam");
        g_mmiset_ss_status.ss_code = MN_SS_BICROAM;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        break;
        
    case ID_SET_BAR_MO_CALLS_INTL://国际呼出
        //SCI_TRACE_LOW("SetSsParam: call bar all mo calls in roam");
        g_mmiset_ss_status.ss_code = MN_SS_BOIC;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        break;
        
    case ID_SET_BAR_MO_CALLS_INTL_ROAM://国际漫游拨出(本国外国际呼出)
        //SCI_TRACE_LOW("SetSsParam: call bar all mo calls in international roam");
        g_mmiset_ss_status.ss_code = MN_SS_BOIC_EXHC;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
        g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
        break;
        
    default:
        break;
    }
    
    
    if (is_interrogate)
    {
        //!!!查询时service应该是 all service，不关心service code!!!
        g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
    }
    
    
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2815_112_2_18_2_50_9_32, (uint8*)"d", MMISET_BAR_ALLOUTGOING_LABEL_CTRL_ID);
    //SCI_TRACE_LOW:"SetSsParam: called_fuc_id is %d a, ss code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2816_112_2_18_2_50_9_33, (uint8*)"dd", called_fuc_id, g_mmiset_ss_status.ss_code);
}
#endif

/*****************************************************************************/
// 	Description :设置是否已经查询了CFU的状态
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsQueryCfu( BOOLEAN is_query,MN_DUAL_SYS_E dual_sys)
{
    s_has_query_cfu = is_query;
    
    if (!is_query)
    {
        if(dual_sys < MMI_DUAL_SYS_MAX)
        {
            s_dualsys_cfu_query_info[dual_sys] = FALSE;
        }
        else
        {
            MMIAPISET_ResetDualsysQueryInfo();
        }
    }
}

/*****************************************************************************/
// 	Description :获得是否已经完成了CFU查询
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsQueryCfu(void)
{
    return (s_has_query_cfu);
}

/*****************************************************************************/
// 	Description :Reset dual sys query info
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetDualsysQueryInfo(void)
{
    SCI_MEMSET(s_dualsys_cfu_query_info, FALSE , sizeof(s_dualsys_cfu_query_info));
}

/*****************************************************************************/
// 	Description :get cfu info
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetCfuInfo(MN_DUAL_SYS_E dual_sys)
{
    return s_dualsys_cfu_query_info[dual_sys];
}

/*****************************************************************************/
//  Description :Get Interrogate Cfu Sim Id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetCfuSimId(void)
{
    uint32 i = 0;
    MN_DUAL_SYS_E cfu_dual_sys = MMI_DUAL_SYS_MAX;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    
    if (!MMIAPISET_GetIsQueryCfu())//查找完毕
    {
        return cfu_dual_sys;
    }
    
    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if ((e_dualsys_setting & (0x01 << i)) && MMIAPIPHONE_GetSimExistedStatus(i) && !s_dualsys_cfu_query_info[i])
        {
            cfu_dual_sys = (MN_DUAL_SYS_E)i;
            
            break;
        }
    }
    
    //SCI_TRACE_LOW:"GetCfuSimId cfu_dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2896_112_2_18_2_50_9_34, (uint8*)"d", cfu_dual_sys);
    
    return cfu_dual_sys;
}

/*****************************************************************************/
// 	Description :MMIAPISET_InterrogateCfuStatusFromPs
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_InterrogateCfuStatusFromPs( 
                                                 MN_DUAL_SYS_E dual_sys,
                                                 uint16		line_index
                                                 )
{
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_NO_ERR;
    MN_CFF_T call_forward_flag = {0};/*lint !e64*/
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};

    if (MMISET_IsSSProcessing())
    {
        return;
    }
    
    if (!MMIAPISET_GetIsQueryCfu())
    {
        uint32 i = 0;
        BOOLEAN is_cfu_complete = TRUE;
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;

        //避免同一张卡重复查询
        if (s_dualsys_cfu_query_info[dual_sys])
        {
            return;
        }

        s_dualsys_cfu_query_info[dual_sys] = TRUE;

        e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if ((e_dualsys_setting & (0x01 << i)) && MMIAPIPHONE_GetSimExistedStatus(i)
                && !s_dualsys_cfu_query_info[i])
            {
                is_cfu_complete = FALSE;

                break;
            }
        }

        if (is_cfu_complete)
        {
            MMIAPISET_SetIsQueryCfu(TRUE,dual_sys);

            //SCI_TRACE_LOW:"MMIAPISET_InterrogateCfuStatusFromPs:only one sim card"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2951_112_2_18_2_50_9_35,(uint8*)"");
        }

        err_code = MNSIM_GetDivertedCallStatusEx(dual_sys, &call_forward_flag);
        
        //SCI_TRACE_LOW:"MMIAPISET_InterrogateCfuStatusFromPs:err_code=0x%x,cff1=%d,cff2=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_2956_112_2_18_2_50_9_36,(uint8*)"ddd", err_code, call_forward_flag.cff_line_1, call_forward_flag.cff_line_2);
        
        if (ERR_MNDATAMAG_NO_ERR == err_code)
        {
            BOOLEAN is_cf_state = FALSE;
            MMISET_SetIsCfuActived(dual_sys, MMISET_SS_AUDIO, TRUE);

            if (0 == line_index)
            {
                is_cf_state = call_forward_flag.cff_line_1;
            }
            else if (1 == line_index)
            {
                is_cf_state = call_forward_flag.cff_line_2;
            }


            MMIAPIPHONE_InterrogateCfuForIdle(GetCfuSimId());
        }
        else
        {
            MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;
            
#ifdef VT_SUPPORT
            //set ss param,include ss_code and sevice_type,service_code
            if ( 0 == line_index )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_TELESERVICES;
            }
            else if ( 1 == line_index )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_TELESERVICES;
            }
#else
            //set ss param,include ss_code and sevice_type,service_code
            if ( 0 == line_index )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
            }
            else if ( 1 == line_index )
            {
                g_mmiset_ss_status.ss_code = MN_SS_CFU;
                g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
                g_mmiset_ss_status.basic_service.service_code = MN_SS_AUXILIARY_TELEPHONY;
            }
#endif
            g_mmiset_ss_status.dual_sys = dual_sys;
            //interrogate status
            ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_INTERROGATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);

            if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
            {
                g_is_for_cfu_icon = TRUE;
            }
            
            //SCI_TRACE_LOW:"MMIAPISET_InterrogateCfuStatusFromPs:get the cfu status from the network, ss_err_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3002_112_2_18_2_50_9_37,(uint8*)"d", ss_err_code);
        }
    }
    else 
    {
        //SCI_TRACE_LOW:"MMIAPISET_InterrogateCfuStatusFromPs:has query already. do nothing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3007_112_2_18_2_50_9_38,(uint8*)"");
    }
}
#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : Reset Select Line
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetSelectLine(MN_DUAL_SYS_E dual_sys)
{
    MN_RETURN_RESULT_E line_return_value = MN_RETURN_FAILURE;
    uint32 i = 0;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    MMINV_READ(MMINV_SET_LINE_TYPE, g_current_line, line_return_value);
    
    if (MN_RETURN_SUCCESS != line_return_value)
    {
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
           g_current_line[i] = 0;
        }
    }

    g_current_line[dual_sys] = 0;

    MMINV_WRITE(MMINV_SET_LINE_TYPE, g_current_line);
}


/*****************************************************************************/
// 	Description : 设置中线路选择的结果
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSelectLine(
                                 MN_DUAL_SYS_E      dual_sys,                            
                                 MMISET_LINE_TYPE_E  line_type
                                 )
{
    BOOLEAN result = FALSE;
    ERR_MNDATAMAG_CODE_E writeline_result = ERR_MNDATAMAG_NO_ERR;
    MN_BEARER_CAPABILITY_TYPE_E selected_line = MN_BEARER_CAPABILITY_LINE_1;
    
    //将线路状态存入sim卡
    if ( MMISET_LINE_1 == line_type )
    {
        selected_line = MN_BEARER_CAPABILITY_LINE_1;
    }
    else if ( MMISET_LINE_2 == line_type )
    {
        selected_line = MN_BEARER_CAPABILITY_LINE_2;
    }

    writeline_result = MNSIM_SetSelectedLineInSIMEx(dual_sys,selected_line);
    
    if (ERR_MNDATAMAG_NO_ERR == writeline_result)
    {
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_ONE,
                                                                PNULL);
        result = TRUE;
    }
    else
    {
        MMK_CloseWin(MMISET_LINE_SELECT_WIN_ID);

        //Not support
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        //不支持cphs，则idle win不显示任何icon
        MAIN_SetIdleLineState(2, dual_sys);
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
// 	Description : ??????????
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetSelectLine(
                                 MN_DUAL_SYS_E      dual_sys,                            
                                 MMISET_LINE_TYPE_E  line_type
                                 )
{
    return MMISET_SetSelectLine( dual_sys,  line_type);
}

/*****************************************************************************/
// 	Description : Handle APP_MN_LINE_SELECTED_UPDATE_IND
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleLineSelectedUpdateInd(
                                     DPARAM param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef MMI_CSP_SUPPORT
    APP_MN_LINE_SELECTED_UPDATE_CNF_T line_selected_update_cnf = *(APP_MN_LINE_SELECTED_UPDATE_CNF_T *)param;
    MN_BEARER_CAPABILITY_TYPE_E selected_line = MN_BEARER_CAPABILITY_LINE_1;
    ERR_MNDATAMAG_CODE_E readline_result = ERR_MNDATAMAG_NO_ERR;

    SCI_TRACE_LOW("HandlLineSelectedUpdateCnf:dual_sys = %d, result=%d", line_selected_update_cnf.dual_sys, line_selected_update_cnf.result);

    if (line_selected_update_cnf.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        MMK_CloseWin(MMISET_LINE_SELECT_WIN_ID);
        MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);

        return MMI_RESULT_FALSE;
    }

    if (MN_SIM_FILE_NO_ERR == line_selected_update_cnf.result)
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
#endif

        readline_result = MNSIM_GetSelectedLineInSIMEx(line_selected_update_cnf.dual_sys, &selected_line);

        if (ERR_MNDATAMAG_NO_ERR == readline_result)
        {
            uint16 cur_selection = 0;

            if (MN_BEARER_CAPABILITY_LINE_2 == selected_line)
            {
                cur_selection = 1;
            }
            else
            {
                cur_selection = 0;
            }

            MMISET_SaveLineInfoToNV((MMISET_LINE_TYPE_E)cur_selection, line_selected_update_cnf.dual_sys);

            MAIN_SetIdleLineState(cur_selection, line_selected_update_cnf.dual_sys);
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
    }

    MMK_CloseWin(MMISET_LINE_SELECT_WIN_ID);
    MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
#endif

    return result;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleLineLockStatusUpdateInd(
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_LINE_LOCKSTATUS_UPDATE_CNF_T lock_status_update_cnf = *((APP_MN_LINE_LOCKSTATUS_UPDATE_CNF_T*)param);

    SCI_TRACE_LOW("MMIAPISET_HandleLineLockStatusUpdateInd result: %d", lock_status_update_cnf.result);
    if(MN_SIM_FILE_NO_ERR == lock_status_update_cnf.result)
    {
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);            
    }
    else
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
    MMIPUB_CloseWaitWin(MAIN_WAIT_WIN_ID);
    return result;
}

/*****************************************************************************/
// 	Description : 设置中线路选择的类型
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void MMISET_SaveLineInfoToNV(   
                                   MMISET_LINE_TYPE_E  line_type,   // the type of line
                                   MN_DUAL_SYS_E       dual_sys
                                   )
{
    //MN_RETURN_RESULT_E      line_return_value = MN_RETURN_FAILURE;
    //uint8					line = 2;
    
    //SCI_ASSERT(dual_sys < MMI_DUAL_SYS_MAX);/* assert to do*/
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return;  

    //change the line of system
    g_current_line[dual_sys] = (uint8)line_type;
    
    //SCI_TRACE_LOW:"MMISET_SaveLineInfoToNV: the line type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3079_112_2_18_2_50_9_39,(uint8*)"d",line_type);
    //write the type of line into NV
    MMINV_WRITE(MMINV_SET_LINE_TYPE,g_current_line);
    
    //MMINV_READ(MMINV_SET_LINE_TYPE,&line,line_return_value);
    
    //SCI_TRACE_LOW:"MMISET_SaveLineInfoToNV: read the line type is %d, the return value is %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3085_112_2_18_2_50_9_40,(uint8*)"dd",line,line_return_value);
}

/*****************************************************************************/
// 	Description : 添加线路选择的radio list的item
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendLineListItem(
                                      MN_DUAL_SYS_E dual_sys,
                                      MMI_CTRL_ID_T	    ctrl_id
                                      )
{
    uint16                  cur_selection = 0;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_LINE_1, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_LINE_2, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current line
    cur_selection = (uint16)g_current_line[dual_sys];
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
// 	Description : get csp2 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: need receive APP_MN_LINE_LOCKSTATUS_UPDATE_IND msg
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_LockLine(
                                       MN_DUAL_SYS_E dual_sys,
                                       BOOLEAN is_lock
                                       )         
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_LockLine error sim:%d ", dual_sys);
        return MMI_RESULT_FALSE;
    }
    
    result = (ERR_MNDATAMAG_NO_ERR == MNSIM_SetLockStatusOfLineInUseInSIMEx(dual_sys, is_lock))?MMI_RESULT_TRUE:MMI_RESULT_FALSE;
    SCI_TRACE_LOW("MMISET_LockLine dual_sys:%d, is_lock:%d, result:%d", dual_sys, is_lock, result);
    return result;
}

/*****************************************************************************/
// 	Description : get csp2 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MMISET_LINE_LOCK_STATUS_E MMISET_GetLineLockStatus(
                                       MN_DUAL_SYS_E dual_sys
                                       )         
{
    BOOLEAN is_lock = FALSE;
    MMISET_LINE_LOCK_STATUS_E lock_status = MMISET_LINE_IS_UNLOCK;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_IsLineLock error sim:%d ", dual_sys);
        return MMISET_LINE_LOCK_INVALID;
    }    
    if(ERR_MNDATAMAG_NO_ERR == MNSIM_GetLockStatusOfLineInUseInSIMEx(dual_sys, &is_lock))
    {
        lock_status = is_lock?MMISET_LINE_IS_LOCK:MMISET_LINE_IS_UNLOCK;
    }
    else
    {
        lock_status = MMISET_LINE_LOCK_INVALID;
    }

    SCI_TRACE_LOW("MMISET_IsLineLock status:%d ", lock_status);
    return lock_status;
}

/*****************************************************************************/
// 	Description : is als valid
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsAlsValid(MN_DUAL_SYS_E dual_sys) 
{
    BOOLEAN result = FALSE;
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_IsAlsValid err sim:%d", dual_sys);
        return FALSE;
    }
    
#ifdef MMI_CSP_SUPPORT        
    if (MMISET_IsCphsValid(dual_sys) && MMISET_IsCspValid(dual_sys, 0))
    {
        MN_CSP_SERVICES_GROUP_T* csp_info_ptr = MMISET_GetCsp1Service(dual_sys);
        if (csp_info_ptr->is_cphs_teleserv_present)
        {
            result = csp_info_ptr->cphs_teleservices.is_als;
        }
    }
#endif        
    
    return result;
}

/*****************************************************************************/
// 	Description : is csp valid by line
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCspValid(
                                       MN_DUAL_SYS_E dual_sys,
                                       uint8    line
                                       )
{
    if(line > 1 || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_IsCspValid error line:%d sim:%d ", line, dual_sys);
        return FALSE;
    }

    if(0 == line)
    {
        return s_cphs_info[dual_sys].is_csp1_valid;
    }
    else
    {
        return s_cphs_info[dual_sys].is_csp2_valid;
    }
}

/*****************************************************************************/
// 	Description : get csp1 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: the s_cphs_info[dual_sys].is_csp1_valid should be TRUE, else return PNULL
/*****************************************************************************/
PUBLIC MN_CSP_SERVICES_GROUP_T* MMISET_GetCsp1Service(
                                       MN_DUAL_SYS_E dual_sys
                                       )         
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_GetCsp1Service error sim:%d ", dual_sys);
        return PNULL;
    }

    if(!s_cphs_info[dual_sys].is_csp1_valid)
    {
        SCI_TRACE_LOW("MMISET_GetCsp1Service invalid ");
        return PNULL;
    }

    return &s_cphs_info[dual_sys].csp1_service_group;
}

/*****************************************************************************/
// 	Description : get csp2 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: the s_is_csp1_valid should be TRUE, else return PNULL
/*****************************************************************************/
PUBLIC MN_CSP_LINE2_SERVICES_GROUP_T* MMISET_GetCsp2Service(
                                       MN_DUAL_SYS_E dual_sys
                                       )         
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_GetCsp2Service error sim:%d ", dual_sys);
        return PNULL;
    }

    if(!s_cphs_info[dual_sys].is_csp2_valid)
    {
        SCI_TRACE_LOW("MMISET_GetCsp2Service invalid ");
        return PNULL;
    }

    return &s_cphs_info[dual_sys].csp2_service_group;
}
#endif //MMI_CSP_SUPPORT

#ifdef MMI_CPHS_SUPPORT
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleCPHSReadyInd(
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_CPHS_READY_IND_T cphs_ready_ind = *((APP_MN_CPHS_READY_IND_T *)param);
    MN_DUAL_SYS_E dual_sys = cphs_ready_ind.dual_sys;
    ERR_MNDATAMAG_CODE_E csp1_recode = ERR_MNDATAMAG_NO_ERR;
    ERR_MNDATAMAG_CODE_E csp2_recode = ERR_MNDATAMAG_NO_ERR;

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("HandleCPHSReadyInd err sim:%d", dual_sys);
        return MMI_RESULT_FALSE;
    }

    s_cphs_info[dual_sys].is_cphs_valid = cphs_ready_ind.is_cphs_exist;


    if(s_cphs_info[dual_sys].is_cphs_valid)
    {
#ifdef MMI_CSP_SUPPORT 
        SCI_MEMSET(&s_cphs_info[dual_sys].csp1_service_group, 0x00, sizeof(MN_CSP_SERVICES_GROUP_T));
        SCI_MEMSET(&s_cphs_info[dual_sys].csp2_service_group, 0x00, sizeof(MN_CSP_LINE2_SERVICES_GROUP_T));
        
        csp1_recode = MNSIM_GetCustomerServiceProfileEx(dual_sys, &s_cphs_info[dual_sys].csp1_service_group);
        csp2_recode = MNSIM_GetCustomerServiceProfileLine2Ex(dual_sys, &s_cphs_info[dual_sys].csp2_service_group);

        if(ERR_MNDATAMAG_NO_ERR == csp1_recode)
        {
            s_cphs_info[dual_sys].is_csp1_valid = TRUE;
        }
        else
        {
            s_cphs_info[dual_sys].is_csp1_valid = FALSE;
            SCI_MEMSET(&s_cphs_info[dual_sys].csp1_service_group, 0x00, sizeof(MN_CSP_SERVICES_GROUP_T));            
        }

        if(ERR_MNDATAMAG_NO_ERR == csp2_recode)
        {
            s_cphs_info[dual_sys].is_csp2_valid = TRUE;
        }    
        else
        {
            s_cphs_info[dual_sys].is_csp2_valid = FALSE;
            SCI_MEMSET(&s_cphs_info[dual_sys].csp2_service_group, 0x00, sizeof(MN_CSP_LINE2_SERVICES_GROUP_T));            
        }
#endif
        //handle cf icon in statusbar
        {
            MN_CFF_T call_forward_flag = {0};
            ERR_MNDATAMAG_CODE_E err_code = MNSIM_GetDivertedCallStatusEx(dual_sys, &call_forward_flag);
            
            SCI_TRACE_LOW("HandleCPHSReadyInd:err_code=0x%x,cff1=%d,cff2=%d", err_code, call_forward_flag.cff_line_1, call_forward_flag.cff_line_2);
            
            if (ERR_MNDATAMAG_NO_ERR == err_code)
            {
                s_cphs_info[dual_sys].is_cfu_saved = TRUE;
                if(1 == MMIAPISET_GetCallLineInfo(dual_sys))//line 2
                {
                    MAIN_SetIdleForwardState(dual_sys, call_forward_flag.cff_line_2);
                }
                else
                {
                    MAIN_SetIdleForwardState(dual_sys, call_forward_flag.cff_line_1);
                }
             }        
             else
            {
                s_cphs_info[dual_sys].is_cfu_saved = FALSE;
            }
        }        
    }

#ifdef MMI_CSP_SUPPORT
    //set call line
    MAIN_SetIdleLineState(MMIAPISET_GetCallLineInfo(dual_sys), dual_sys);  //????line state???
    
    SCI_TRACE_LOW("MMIAPISET_HandleCPHSReadyInd:dual_sys=%d, cphs:%d, csp1:%d, csp2:%d", dual_sys, s_cphs_info[dual_sys].is_cphs_valid, s_cphs_info[dual_sys].is_csp1_valid, s_cphs_info[dual_sys].is_csp2_valid);    
#endif    
    return result;
}

/*****************************************************************************/
// 	Description : is csp valid by line
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCphsValid(
                                       MN_DUAL_SYS_E dual_sys
                                       )
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_IsCspValid error sim:%d ", dual_sys);
        return FALSE;
    }
    else
    {
        return s_cphs_info[dual_sys].is_cphs_valid;
    }
}

#endif //MMI_CPHS_SUPPORT

/*****************************************************************************/
// 	Description : start call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_StartCallForwarding(
                                       MN_DUAL_SYS_E     dual_sys,
                                       MMI_STRING_T    *voice_number_ptr
                                       )
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MMI_PARTY_NUMBER_T          party_num = {0};/*lint !e64*/
    MN_CALLED_NUMBER_T		    number = {0};/*lint !e64*/
    uint8  phonenum[MMIDIAL_PHONENUM_LEN + 1];
    MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;

    if(PNULL == voice_number_ptr)
    {
        return;  
    }

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();

        return;
    }
#endif

    MMI_WSTRNTOSTR((uint8 *)phonenum, MMIDIAL_PHONENUM_LEN, voice_number_ptr->wstr_ptr, voice_number_ptr->wstr_len, voice_number_ptr->wstr_len );
    
    // 将语音信箱号码或者电话号码转换为所需要的格式，存入number。
    MMIAPICOM_GenPartyNumber(phonenum, /*voice_number_ptr->wstr_ptr,*/voice_number_ptr->wstr_len, &party_num);
    
    //set register_param value
    number.number_type = party_num.number_type;
    number.number_plan = MN_NUM_PLAN_ISDN_TELE;
    number.num_len = party_num.num_len;
    MMI_MEMCPY(number.party_num, MN_MAX_ADDR_BCD_LEN, party_num.bcd_num, MN_MAX_ADDR_BCD_LEN, party_num.num_len);
    
    if (MN_SS_CFNRY == g_mmiset_ss_status.ss_code)//无应答转移
    {
        register_param.cfnry_param.forward_num = number;
        register_param.cfnry_param.no_reply_time = GetNoAnswerTime(MMISET_SET_NO_ANSWER_TIME_CTRL_ID);
    }
    else
    {
        register_param.forward_num = number;
    }
    
    ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_REGISTERSS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);

    if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
    {
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                     MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                     TXT_NETWORK_CONNECT,
                                     MMIPUB_SOFTKEY_ONE,
                                     HandleNetworkConnectWindow);
    }
    else if (MNSS_FDN_FAILED == ss_err_code)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
}

/*****************************************************************************/
// 	Description : 处理网络连接界面的消息(除PS之外)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkConnectWindow(
                                              MMI_WIN_ID_T       win_id, 	// 窗口的ID
                                              MMI_MESSAGE_ID_E 	    msg_id, 	// 窗口的内部消息ID
                                              DPARAM 		        param		// 相应消息的参数
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_CANCEL:
        //release ss
        MNSS_RelActivingServiceEx(g_mmiset_ss_status.dual_sys);
        
        //关闭语音信箱、输入电话号码、无应答时间和输入密码界面
        MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
        MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
        MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
        MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
        
        //关闭网络连接窗口, 这个窗口为Focus窗口需要放到最后面，这样才不会出现
        // 关闭的窗口再收到异步消息 CR14009
        MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        //release ss
        MNSS_RelActivingServiceEx(g_mmiset_ss_status.dual_sys);
        
        //关闭语音信箱、输入电话号码、无应答时间和输入密码界面
        MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
        MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
        MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
        MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
        
        //关闭网络连接窗口, 这个窗口为Focus窗口需要放到最后面，这样才不会出现
        // 关闭的窗口再收到异步消息 CR14009
        MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        
        result = MMI_RESULT_FALSE;
        break;
#endif        
        
    case MSG_APP_RED:
        //release ss
        MNSS_RelActivingServiceEx(g_mmiset_ss_status.dual_sys);
        
        //关闭语音信箱、输入电话号码、无应答时间和输入密码界面
        MMK_CloseWin(MMISET_SET_DIVERT_VOICE_WIN_ID);
        MMK_CloseWin(MMISET_SET_DIVERT_NUMBER_WIN_ID);
        MMK_CloseWin(MMISET_SET_NO_ANSWER_TIME_WIN_ID);
        MMK_CloseWin(MMISET_SET_BAR_PASSWORD_WIN_ID);
        
        //关闭网络连接窗口, 这个窗口为Focus窗口需要放到最后面，这样才不会出现
        // 关闭的窗口再收到异步消息 CR14009
        MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        g_is_ss_user_query = FALSE;

        MMIAPISMS_CheckTimerMSG();

        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 添加无应答时间的单选list items
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendNoAnswerTimeListItem(
                                              MMI_CTRL_ID_T	    ctrl_id
                                              )
{
    uint16      cur_selection = MMISET_DEFAULT_NO_ANSWER_TIME_ITEM;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_30S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_25S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_20S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_15S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_10S, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );//cr273646
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_5S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
// 	Description : 获得无应答时间
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL uint8 GetNoAnswerTime(
                            MMI_CTRL_ID_T	    ctrl_id
                            )
{
    uint8       no_answer_time = 0;
    uint16      cur_selection = 0;
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        
    //SCI_TRACE_LOW("GetNoAnswerTime: the cur_selection is %d",cur_selection);
    switch (cur_selection)
    {
    case 0:
        no_answer_time = (uint8)(MMISET_TIME_30 / MMISET_SET_PER_TIME);
        break;
        
    case 1:
        no_answer_time = (uint8)(MMISET_TIME_25 / MMISET_SET_PER_TIME);
        break;
        
    case 2:
        no_answer_time = (uint8)(MMISET_TIME_20 / MMISET_SET_PER_TIME);
        break;
        
    case 3:
        no_answer_time = (uint8)(MMISET_TIME_15 / MMISET_SET_PER_TIME);
        break;
        
    case 4:
        no_answer_time = (uint8)(MMISET_TIME_10 / MMISET_SET_PER_TIME);
        break;
        
    case 5:
        no_answer_time = (uint8)(MMISET_TIME_5 / MMISET_SET_PER_TIME);
        break;
        
    default:
        break;
    }
    return (no_answer_time);
}

/*****************************************************************************/
// 	Description : close call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseCallForwarding(MN_DUAL_SYS_E dual_sys)
{
    MMI_CTRL_ID_T				called_fuc_id = MMISET_SET_CALL_DIVERT_WIN_ID;
#if defined(MMI_GUI_STYLE_TYPICAL)
    MMI_MENU_GROUP_ID_T         group_id    =   0;
#endif
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();

        return;
    }
#endif
    
    //get menu id and group id
    //GUIMENU_GetId(MMISET_GetDivertBarWatingHideidCtrlId(),&group_id,&menu_id);
    //SCI_TRACE_LOW("MMISET_CloseCallForwarding: group_id is %d and menu_id is %d",group_id,menu_id);
    
//#ifdef VT_SUPPORT
 //   GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID,&group_id_2,&menu_id_2);//@zhaohui add
//#endif
    
    //set ss param,include ss_code and sevice_type,service_code
    SetSsParam(dual_sys,FALSE);
#if !defined MMI_GUI_STYLE_TYPICAL
    GUILIST_GetItemData(MMISET_CALL_DIVERT_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_DIVERT_MENU_CTRL_ID), &called_fuc_id);
#else
    GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID,&group_id,&called_fuc_id);
#endif
    switch(called_fuc_id)
    {
    case ID_SET_DIVERTS_ALL_CALLS://无条件转移
#ifdef MMI_CSP_SUPPORT
    case ID_SET_DIVERTS_ALL_CONDITIONAL:
#endif    
    case ID_SET_DIVERTS_WHEN_BUSY://遇忙时转移
    case ID_SET_DIVERTS_NO_ANSWER://无法到达转移
    case ID_SET_DIVERTS_NOT_REACHABLE://无应答转移
        //disable call forwarding
        ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_ERASESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
        
        if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
        {
            g_mmiset_ss_status.dual_sys = dual_sys;

            //进入网络连接界面
            MMISET_OpenNetworkConnectWin(dual_sys,
                                         MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                         TXT_NETWORK_CONNECT,
                                         MMIPUB_SOFTKEY_ONE,
                                         HandleNetworkConnectWindow);
        }
        else if (MNSS_FDN_FAILED == ss_err_code)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        }
        break;
        
    default:
        break;
    }
        

}

/*****************************************************************************/
// 	Description : interrogate status of ss
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_InterrogateStatus(MN_DUAL_SYS_E dual_sys)
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T        ss_string = {0};
    MNSS_SENDSS_RESULT_E      ss_err_code = MNSS_SUCCESS;
    
    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif
    
    //set ss param,include ss_code and sevice_type,service_code
    SetSsParam(dual_sys,TRUE);
    
    //interrogate status
    g_is_for_cfu_icon = FALSE;
    
    ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_INTERROGATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);

    if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
    {
        //interrogate status
        g_is_ss_user_query = TRUE;
        g_mmiset_ss_status.dual_sys = dual_sys;

        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                     MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                     TXT_NETWORK_CONNECT,
                                     MMIPUB_SOFTKEY_ONE,
                                     HandleNetworkConnectWindow);
    }
    else if (MNSS_FDN_FAILED == ss_err_code)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
}

/*****************************************************************************/
// 	Description : interrogate status of ss
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_InterrogateAllCFStatus(MN_DUAL_SYS_E dual_sys)
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();

        return;
    }
#endif

    s_query_cf_num = 1;
    SCI_MEMSET(s_cf_list_num, 0, (MMISET_ALL_INTERROGATE_NUM*sizeof(uint8)));	//SCI_MEMSET(&password_info,0,sizeof(MMI_STRING_T));

    if (PNULL == s_cf_info_ptr)
    {
        s_cf_info_ptr = SCI_ALLOC_APP(MMISET_ALL_INTERROGATE_NUM*sizeof(APP_MN_INTERROGATE_CNF_T));
    }

    if (PNULL != s_cf_info_ptr)
    {
        SCI_MEMSET(s_cf_info_ptr, 0, (MMISET_ALL_INTERROGATE_NUM*sizeof(APP_MN_INTERROGATE_CNF_T)));
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        return;
    }
    
    //SCI_TRACE_LOW:"mmiset: query all cf start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3491_112_2_18_2_50_10_41,(uint8*)"");
    //SCI_TRACE_LOW:"mmiset: query all cfu"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3492_112_2_18_2_50_10_42,(uint8*)"");
    
    g_mmiset_ss_status.ss_code = MN_SS_CFU;
    g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
    g_mmiset_ss_status.dual_sys = dual_sys;
    g_is_for_cfu_icon = FALSE;

    //interrogate status
    ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_INTERROGATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);

    if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
    {
        //interrogate status
        g_is_ss_user_query = TRUE;

        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_ONE,
                                                                HandleNetworkConnectWindow);
    }
    else if (MNSS_FDN_FAILED == ss_err_code)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
}

/*****************************************************************************/
// 	Description : close all call forwarding
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseAllCallForwarding(MN_DUAL_SYS_E dual_sys)
{

    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T        ss_string = {0};
    MMI_CTRL_ID_T		    called_fuc_id = MMISET_SET_CALL_DIVERT_WIN_ID;
#if defined(MMI_GUI_STYLE_TYPICAL)
    MMI_MENU_GROUP_ID_T          group_id = 0;
#endif
    MNSS_SENDSS_RESULT_E      ss_err_code = MNSS_SUCCESS;
#ifdef VT_SUPPORT
    MMI_CTRL_ID_T        called_subfuc_id = 0;
#endif

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
        
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();

        return;
    }
#endif
    
    //get menu id and group id
    //GUIMENU_GetId(MMISET_GetDivertBarWatingHideidCtrlId(),&group_id,&menu_id);
    //SCI_TRACE_LOW("MMISET_CloseAllCallForwarding: group_id is %d and menu_id is %d",group_id,menu_id);
    
    //set ss param,include ss_code and sevice_type,service_code
    //SCI_TRACE_LOW("MMISET_CloseAllCallForwarding: calose all call forwarding!");
    g_mmiset_ss_status.ss_code = MN_SS_ALL_FORWARD;
    
#ifdef VT_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
    GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);
#else
    GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID,&group_id,&called_subfuc_id);
#endif
    //SCI_TRACE_LOW:"MMISET_CloseAllCallForwarding: group_id is %d and called_subfuc_id is %d"
#if 1//OPEN_VT_CHANGE
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3556_112_2_18_2_50_10_43,(uint8*)"dd",GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID),called_subfuc_id);
#else
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3556_112_2_18_2_50_10_43,(uint8*)"dd",group_id,called_subfuc_id);
#endif
    switch (called_subfuc_id)//用来区分不同的Service type
    {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            break;
            
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            break;
            
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
            
        default:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
    }
#else
    g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;//MN_SS_TELE_SERVICE;
#endif
    g_mmiset_ss_status.basic_service.service_code = 0;//MN_SS_ALL_SPEECH_TRANS_SERVICES;
    
#if !defined MMI_GUI_STYLE_TYPICAL
    GUILIST_GetItemData(MMISET_CALL_DIVERT_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_DIVERT_MENU_CTRL_ID), &called_fuc_id);
#else
    GUIMENU_GetId(MMISET_CALL_DIVERT_MENU_CTRL_ID,&group_id,&called_fuc_id);
#endif
    if (ID_SET_DIVERTS_CANCEL_ALL == called_fuc_id) //取消全部转移
    {
        //disable all call forwarding
        g_mmiset_ss_status.dual_sys = dual_sys;
        ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_ERASESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
        if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
        {
            //进入网络连接界面
            MMISET_OpenNetworkConnectWin(dual_sys,
                                         MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                         TXT_NETWORK_CONNECT,
                                         MMIPUB_SOFTKEY_ONE,
                                         HandleNetworkConnectWindow);
        }
        else if (MNSS_FDN_FAILED == ss_err_code)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        }
    }
    else
    {
        SCI_TRACE_LOW("MMISET_CallForwarding: menu_id is error!");
    }
}

/*****************************************************************************/
// 	Description : start call waiting or call bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_StartCallWaitOrBar(MN_DUAL_SYS_E dual_sys)
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif
    
    //set ss param,include ss_code and sevice_type,service_code
    SetSsParam(dual_sys, FALSE);
    
    g_mmiset_ss_status.dual_sys = dual_sys;
    //activate ss
    ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_ACTIVATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);

    if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
    {
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                     MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                     TXT_NETWORK_CONNECT,
                                     MMIPUB_SOFTKEY_ONE,
                                     HandleNetworkConnectWindow);
    }
    else if (MNSS_FDN_FAILED == ss_err_code)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
}

/*****************************************************************************/
// 	Description : register new pw 
//	Global resource dependence : 
//  Author: maql
//	Note:
/*****************************************************************************/
void MMISET_ChangeCallBarPassWord(MN_DUAL_SYS_E dual_sys)
{
    MN_SS_BASIC_SERVICE_T basic_service = {0};/*lint !e64*/
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();

        return;
    }
#endif
    
    ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_REGISTERPASSWORD, MN_SS_ALL_BAR_SERVICE, basic_service, register_param, ss_string);

    if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
    {
        g_mmiset_ss_status.dual_sys = dual_sys;
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_ONE,
                                                                HandleNetworkConnectWindow);
    }
    else if (MNSS_FDN_FAILED == ss_err_code)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
}


/*****************************************************************************/
// 	Description : send call bar password
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SendBarPassword(MN_DUAL_SYS_E   dual_sys)
{
    uint8           bar_password[MMISET_BAR_PASSWORD_MAX_LEN + 1] = {0};
    MMI_STRING_T   password_info = {0};

    GUIEDIT_GetString(MMISET_SET_BAR_PASSWORD_CTRL_ID,&password_info);

    if (MMISET_BAR_PASSWORD_MAX_LEN == password_info.wstr_len)
    {
        MMI_WSTRNTOSTR((uint8 *)bar_password,MMISET_BAR_PASSWORD_MAX_LEN,password_info.wstr_ptr,password_info.wstr_len,password_info.wstr_len);
        MNSS_SendUserPasswordEx(dual_sys, bar_password);
        
        g_mmiset_ss_status.dual_sys = dual_sys;
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_ONE,
                                                                HandleNetworkConnectWindow);
    }
}

/*****************************************************************************/
// 	Description : send new call bar password
//	Global resource dependence : 
//  Author: maql
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SendNewBarPassword(MN_DUAL_SYS_E dual_sys)
{
    uint8           bar_password[MMISET_BAR_PASSWORD_MAX_LEN + 1] = {0};
    MMI_STRING_T   password_info = {0};
    
    GUIEDIT_GetString(MMISET_SET_BAR_CHANGE_PW_CTRL_ID,&password_info);

    if (MMISET_BAR_PASSWORD_MAX_LEN == password_info.wstr_len)
    {
        MMI_WSTRNTOSTR((uint8 *)bar_password,MMISET_BAR_PASSWORD_MAX_LEN,password_info.wstr_ptr,password_info.wstr_len,password_info.wstr_len);
        MNSS_SendUserPasswordEx(dual_sys, bar_password);
        
        g_mmiset_ss_status.dual_sys = dual_sys;
        
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_ONE,
                                                                HandleNetworkConnectWindow);
    }
}

/*****************************************************************************/
// 	Description : close call waiting or call bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseCallWaitOrBar(MN_DUAL_SYS_E dual_sys)
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;
    //MN_SS_BASIC_SERVICE_T basic_service = {0};/*lint !e64*/

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif
    
    //set ss param,include ss_code and sevice_type,service_code
    SetSsParam(dual_sys,FALSE);
    
    //deactivate call forwarding
    g_mmiset_ss_status.dual_sys = dual_sys;
    ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_DEACTIVATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);

    if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
    {
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                     MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                     TXT_NETWORK_CONNECT,
                                     MMIPUB_SOFTKEY_ONE,
                                     HandleNetworkConnectWindow);
    }
    else if (MNSS_FDN_FAILED == ss_err_code)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
} 

/*****************************************************************************/
// 	Description : close all call bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseAllCallBar(MN_DUAL_SYS_E dual_sys)
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MMI_CTRL_ID_T				called_fuc_id = MMISET_SET_CALL_DIVERT_WIN_ID;
#if defined(MMI_GUI_STYLE_TYPICAL)
    MMI_MENU_GROUP_ID_T         group_id    =   0;
#endif
    MNSS_SENDSS_RESULT_E ss_err_code = MNSS_SUCCESS;
    #ifdef VT_SUPPORT
    MMI_CTRL_ID_T       called_subfuc_id = 0;
    #endif

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();

        return;
    }
#endif
    
    //get menu id and group id
   // GUIMENU_GetId(MMISET_GetDivertBarWatingHideidCtrlId(),&group_id,&menu_id);
    //SCI_TRACE_LOW("MMISET_CloseAllCallBar: group_id is %d and menu_id is %d",group_id,menu_id);
    
    //set ss param,include ss_code and sevice_type,service_code
    //SCI_TRACE_LOW("MMISET_CloseAllCallBar: calose all call bar!");
    g_mmiset_ss_status.ss_code = MN_SS_ALL_BAR_SERVICE;
#ifdef VT_SUPPORT
#if !defined MMI_GUI_STYLE_TYPICAL
    GUILIST_GetItemData(MMISET_SET_CALL_MENU_EX_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID), &called_subfuc_id);
#else
    GUIMENU_GetId(MMISET_SET_CALL_MENU_EX_CTRL_ID,&group_id,&called_subfuc_id);
#endif
    //SCI_TRACE_LOW:"MMISET_CloseAllCallBar: group_id is %d and called_subfuc_id is %d"
#if 1//OPEN_VT_CHANGE
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3819_112_2_18_2_50_11_44,(uint8*)"dd",GUILIST_GetCurItemIndex(MMISET_SET_CALL_MENU_EX_CTRL_ID),called_subfuc_id);
#else
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3819_112_2_18_2_50_11_44,(uint8*)"dd",group_id,called_subfuc_id);
#endif
    switch(called_subfuc_id)//用来区分不同的Service type
    {
        case ID_SET_DIVERTS_AUDIO:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
            break;
        case ID_SET_DIVERTS_VIDEO:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_BEARER_SERVICE;
            break;
        case ID_SET_DIVERTS_AUDIOANDVIDEO:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
        default:
            g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
            break;
    }
#else
    g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
#endif

    g_mmiset_ss_status.basic_service.service_code = 0;
#if !defined MMI_GUI_STYLE_TYPICAL
    GUILIST_GetItemData(MMISET_CALL_BAR_MENU_CTRL_ID, GUILIST_GetCurItemIndex(MMISET_CALL_BAR_MENU_CTRL_ID), &called_fuc_id);
#else
    GUIMENU_GetId(MMISET_CALL_BAR_MENU_CTRL_ID,&group_id,&called_fuc_id);
#endif
    if (ID_SET_BAR_CANCEL_ALL == called_fuc_id) //取消全部限制
    {
        //deactivate all call bar
        g_mmiset_ss_status.dual_sys = dual_sys;
        ss_err_code = MNSS_SendSSEx(dual_sys, MN_SS_DEACTIVATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);

        if (MNSS_SUCCESS == ss_err_code || MNSS_INFORM_STK == ss_err_code)
        {
            //进入网络连接界面
            MMISET_OpenNetworkConnectWin(dual_sys,
                                                                    MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                    TXT_NETWORK_CONNECT,
                                                                    MMIPUB_SOFTKEY_ONE,
                                                                    HandleNetworkConnectWindow);
        }
        else if (MNSS_FDN_FAILED == ss_err_code)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        }
    }
    else
    {
        SCI_TRACE_LOW("MMISET_CloseAllCallBar: menu_id is error!");
    }
}

#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
// 	Description : Get IP number
//	Global resource dependence : none
//  Author: lin.lin
//	Note: TRUE: there is active IP number  FALSE: there is no active IP number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetActiveIPNumber(
                                           MN_DUAL_SYS_E dual_sys,
                                           uint8* ipnumber_str_ptr, 
                                           uint8 len
                                           )
{
    MMISET_IP_NUMBER_INFO_T ip_info = {0};/*lint !e64*/
    uint8 number_len = 0;
    uint8 active_id = 0;
    
    //SCI_ASSERT(PNULL != ipnumber_str_ptr);/* assert to do*/
    if(PNULL == ipnumber_str_ptr)
        return  FALSE;  

    MMISET_GetIPNumberInfo(dual_sys, &ip_info);
    
    active_id =	ip_info.active_id;
    if(active_id < MMISET_IPNUMBER_MAX_NUM)
    {
        number_len = strlen((char*)ip_info.number[active_id]);/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
        
        if(len > number_len)
        {
            MMI_MEMCPY(ipnumber_str_ptr, len, ip_info.number[active_id], number_len, MIN(MMISET_IPNUMBER_MAX_LEN,number_len));
            number_len = MIN(MMISET_IPNUMBER_MAX_LEN,number_len);
            ipnumber_str_ptr[number_len] = 0;
            return TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPISET_GetActiveIPNumber error len = %d num_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_3905_112_2_18_2_50_11_45,(uint8*)"dd", len, number_len);
            return FALSE;
        }
    }
    else
    {
        SCI_MEMSET(ipnumber_str_ptr, 0, len);
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : IP Number context init.
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_IPNumberContextInit(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

    if(!g_ip_number_context.is_context_valid)
    {
        MMINV_READ(MMINV_SET_IPNUMBER, g_ip_number_context.ip_num_info,return_value);
        if(MN_RETURN_SUCCESS != return_value)
        {
            MMISET_SetDefaultIPNumberInfoToNV();
        }
        g_ip_number_context.is_context_valid = TRUE;
    }    
}

/*****************************************************************************/
// 	Description : Set default IP number info
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetDefaultIPNumberInfo(void)
{
    uint32 i = 0;
    
    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        g_ip_number_context.ip_num_info[i].dual_sys = i;
        MMISET_SetIPNumberDefaultValue(i, &(g_ip_number_context.ip_num_info[i]));
    }    
}

/*****************************************************************************/
// 	Description : Set default IP number info
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetDefaultIPNumberInfoToNV(void)
{
    MMISET_SetDefaultIPNumberInfo();
    MMINV_WRITE(MMINV_SET_IPNUMBER, (g_ip_number_context.ip_num_info));
}

/*****************************************************************************/
// 	Description : Set IP number info pointer
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC MMISET_IP_NUMBER_INFO_T* MMISET_GetIPNumberInfoPtr(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return PNULL;
    }

    MMISET_IPNumberContextInit();

    return &(g_ip_number_context.ip_num_info[dual_sys]);
    
}

/*****************************************************************************/
// 	Description : Set IP number
//	Global resource dependence : none
//  Author: lin.lin
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetIPNumberInfo(
                                   MMISET_IP_NUMBER_INFO_T *info_ptr
                                   )
{
    //SCI_ASSERT(PNULL != info_ptr);/* assert to do*/
    if(PNULL == info_ptr)
        return;  

    if(info_ptr->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    MMISET_IPNumberContextInit();
    
    SCI_MEMCPY( &(g_ip_number_context.ip_num_info[info_ptr->dual_sys]) ,info_ptr, sizeof(MMISET_IP_NUMBER_INFO_T));
    
    MMINV_WRITE(MMINV_SET_IPNUMBER, (g_ip_number_context.ip_num_info));
    
}
/*****************************************************************************/
// 	Description : Get IP number info
//	Global resource dependence : none
//  Author: lin.lin
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_GetIPNumberInfo(
                                   MN_DUAL_SYS_E dual_sys,
                                   MMISET_IP_NUMBER_INFO_T *info_ptr
                                   )
{    
    //SCI_ASSERT(PNULL != info_ptr);/* assert to do*/
    if(PNULL == info_ptr)
        return;  

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    MMISET_IPNumberContextInit();

    SCI_MEMCPY(info_ptr, &(g_ip_number_context.ip_num_info[dual_sys]), sizeof(MMISET_IP_NUMBER_INFO_T));
    info_ptr->dual_sys = dual_sys;
    
}

/*****************************************************************************/
// 	Description : Set default value
//	Global resource dependence : none
//  Author: lin.lin
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetIPNumberDefaultValue(
                                           MN_DUAL_SYS_E           dual_sys,
                                           MMISET_IP_NUMBER_INFO_T *info_ptr
                                           )
{
    const char* const default_ip_number[MMISET_IPNUMBER_MAX_NUM]= {"17951", 
        "17911",
        "17909",
        "17931",
        "193"
    };
    uint8 number_len = 0;    
    int32 i = 0;
    
    //SCI_ASSERT(PNULL != info_ptr);/* assert to do*/
    if(PNULL == info_ptr)
        return;  

    SCI_MEMSET(info_ptr, 0, sizeof(MMISET_IP_NUMBER_INFO_T));
    info_ptr->active_id = 0;
    info_ptr->dual_sys  = dual_sys;
    
    for(i=0; i<MMISET_IPNUMBER_MAX_NUM; i++)
    {
        number_len = strlen((char*)default_ip_number[i]);
        MMI_MEMCPY(info_ptr->number[i], MMISET_IPNUMBER_MAX_LEN, default_ip_number[i], number_len, number_len);
    }
}
#endif
//cr17947 parse ss code when in idle mode,Start
/*****************************************************************************/
//  Description : parse ss code which is input in idle mode.
//  Global resource dependence : none
//  Author:Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_ParsSsStringProc(
                                          MN_DUAL_SYS_E dual_sys,
                                          parsStrInfoS   *parsStrInfo
                                          )
{
    BOOLEAN                 result = FALSE,res_value = FALSE;
    char                    *input_str_ptr = parsStrInfo->inputStr;
    uint8                   str_len = parsStrInfo->strLen;
    MMISET_SS_INFO_T        ss_info = {0};/*lint !e64*/
    MN_SS_OPERATION_CODE_E  ss_oper = MN_SS_REGISTERSS;
    MMISET_SS_PARSE_INFO_T  ss_result = {0};
    uint32 i=0;
    
    //SCI_MEMSET(&ss_info, 0, sizeof(MMISET_SS_INFO_T));
    
    if (parsStrInfo->strLen <= PARS_MAX_STR_LEN - 1)
    {
        //SCI_TRACE_LOW:"mmiset:MMIAPISET_ParsSsStringProc len=%d, str=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_4081_112_2_18_2_50_11_46,(uint8*)"ds", parsStrInfo->strLen, parsStrInfo->inputStr);
    }
        
    if (IsSimFunction(input_str_ptr,str_len))
    {
        //it isn't ss service,maybe it is sim service
        return (result);
    }
    
    //determine if it is ss service
    if (str_len == 1)
    {
        return (result);
    }
    
    //it is a ss code
    switch (input_str_ptr[0])
    {
    case '*':
        switch(input_str_ptr[1])
        {
        case '*':   //registration
            if(input_str_ptr[2] == '0' && input_str_ptr[3] == '3')
            {
                //SS_REGISTERPASSWORD
                ss_oper = MN_SS_REGISTERPASSWORD;
                res_value = ParseSsRegisterData(&input_str_ptr[4]);
            }
            else
            {
                ss_oper = MN_SS_REGISTERSS;
                res_value = ParseSsData(&input_str_ptr[2],&ss_result);
            }
            break;
            
        case '#':   //Interrogation
            ss_oper = MN_SS_INTERROGATESS;
            res_value = ParseSsData(&input_str_ptr[2],&ss_result);
            break;
            
        default:    //activation or registration;
            if(input_str_ptr[1] == '0' && input_str_ptr[2] == '3')
            {
                //SS_REGISTERPASSWORD
                ss_oper = MN_SS_REGISTERPASSWORD;
                res_value = ParseSsRegisterData(&input_str_ptr[3]);
            }
            else
            {
                res_value = ParseSsData(&input_str_ptr[1],&ss_result);
                if (33  == ss_result.code     //BAOC
                    ||330 == ss_result.code     //all Barring Serv
                    ||331 == ss_result.code     //BOIC
                    ||332 == ss_result.code     //BOIC exc home
                    ||333 == ss_result.code     //Outg.Barr.Serv 
                    ||35  == ss_result.code     //BAIC
                    ||351 == ss_result.code     //BAIC roaming
                    ||353 == ss_result.code     //Inc.Barr. Serv
                    )
                {
                    ss_oper = MN_SS_ACTIVATESS;
                    //Save Password
                    if(ss_result.sia_is_present)
                    {
                        int16 ss_pwdlen = strlen((char *)ss_result.sia);
                        
                        if (MMISET_SS_PWD_LEN == ss_pwdlen)
                        {
                            SCI_MEMSET(g_mmiset_ss_pwd.old_pwd, 0, sizeof(g_mmiset_ss_pwd.old_pwd));
                            MMI_MEMCPY(g_mmiset_ss_pwd.old_pwd,MMISET_SS_PWD_LEN,
                                ss_result.sia,MN_MAX_ADDR_BCD_LEN,
                                strlen((char *)ss_result.sia));/*lint !e666*/
                        }
                        else
                        {
                            return (result);
                        }
                    }
                    else
                    {
                        return (result);
                    }
                }
                else
                {
                    ss_oper = MN_SS_REGISTERSS + MN_SS_ACTIVATESS;/*lint !e656*/
                }
            }
            break;
        }
        break;
        
    case '#':
        switch(input_str_ptr[1])
        {
        case '*':   
            return (result);
            //break;
            
        case '#':   //Erasure
            ss_oper = MN_SS_ERASESS;
            res_value = ParseSsData(&input_str_ptr[2],&ss_result);
            break;

        default:    //SS_DEACTIVATESS;
            ss_oper = MN_SS_DEACTIVATESS;
            res_value = ParseSsData(&input_str_ptr[1],&ss_result);
            if (33  == ss_result.code     //BAOC
                ||330 == ss_result.code     //all Barring Serv
                ||331 == ss_result.code     //BOIC
                ||332 == ss_result.code     //BOIC exc home
                ||333 == ss_result.code     //Outg.Barr.Serv 
                ||35  == ss_result.code     //BAIC
                ||351 == ss_result.code     //BAIC roaming
                ||353 == ss_result.code     //Inc.Barr. Serv
                )
            {
                //Save Password
                if(ss_result.sia_is_present)
                {
                    int16 ss_pwdlen = strlen((char *)ss_result.sia);
                    
                    if (MMISET_SS_PWD_LEN == ss_pwdlen)
                    {
                        SCI_MEMSET(g_mmiset_ss_pwd.old_pwd, 0, sizeof(g_mmiset_ss_pwd.old_pwd));
                        MMI_MEMCPY(g_mmiset_ss_pwd.old_pwd,MMISET_SS_PWD_LEN,
                            ss_result.sia,MN_MAX_ADDR_BCD_LEN,
                            strlen((char *)ss_result.sia));/*lint !e666*/
                    }
                    else
                    {
                        return (result);
                    }
                }
                else
                {
                    return (result);
                }
            }
            break;
        }
        break;
        
    default:
        //error ss data
        return (result);
    }
    
    //parse error
    if (!res_value)
    {
        return (result);
    }
    
    //Call waiting is a special ss
    if (43 == ss_result.code)
    {
        if (ss_oper == MN_SS_REGISTERSS + MN_SS_ACTIVATESS)/*lint !e656*/
        {
            ss_oper = MN_SS_ACTIVATESS;
        }
        if (!AdjustSsCode(&ss_result.code))
        {
            return (result);
        }
        //registration and erasure of the call waiting are not applicable
        if ((ss_oper == MN_SS_REGISTERSS) || (ss_oper == MN_SS_ERASESS))
        {
            return FALSE;
        }
		
        //check the BS
        res_value = FALSE;
        if (ss_result.sia_is_present)
        {
            for (i = 0; i < ARR_SIZE(s_mmi_ss_basic_service); i++)
            {
                int sia = atoi(ss_result.sia);
                if (sia == s_mmi_ss_basic_service[i].sib)
                {
                    ss_info.basic_service.service_type = s_mmi_ss_basic_service[i].service_type;
                    ss_info.basic_service.service_code = s_mmi_ss_basic_service[i].service_code;
                    res_value = TRUE;
                    break;
                }
            }
        }
        else
        {
            ss_info.basic_service.service_type = MN_SS_ALL_SERVICE;
            ss_info.basic_service.service_code = 0;
            res_value = TRUE;
        }
    }
    else
    {
        if (ss_oper != MN_SS_REGISTERPASSWORD)
        {
            if (ss_oper == MN_SS_REGISTERSS + MN_SS_ACTIVATESS)/*lint !e656*/
            {
                if (!ss_result.sia_is_present)
                {
                    ss_oper = MN_SS_ACTIVATESS;
                }
                else
                {
                    ss_oper = MN_SS_REGISTERSS;
                }
            }
            if (!AdjustSsCode(&ss_result.code))
            {
                return (result);
            }
            res_value = HandleSsDivertNum(&ss_result,&ss_info,ss_oper);
        }
        else
        {
            if (!AdjustSsCode(&g_mmiset_ss_pwd.code))
            {
                return (result);
            }
        }
    }
    
    if (res_value)
    {
        //Not Send SS
#ifndef MMI_NOT_SUPPORT_USSD_SS_INCC
#ifdef ADULT_WATCH_SUPPORT
        Settings_SS_SendRequest(dual_sys,ss_oper,&ss_result,&ss_info, (uint8 *)parsStrInfo->inputStr, parsStrInfo->strLen,PNULL);
#else
        SendReqToSs(dual_sys,ss_oper,&ss_result,&ss_info, (uint8 *)parsStrInfo->inputStr, parsStrInfo->strLen);
#endif
#endif
        SCI_TRACE_LOW("MMIAPISET_ParsSsStringProc is SSCODE!\n");
        result = TRUE;	
    }
    else
    {
        result = FALSE;
    }	
    return (result);
}

/*****************************************************************************/
//  Description : user input string is related to SIM
//  Global resource dependence : none
//  Author: Jassmine
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsSimFunction(
                            char      *str_ptr,    //Input string
                            uint8     len         //string's length
                            )
{
    //SCI_ASSERT(PNULL != str_ptr);/* assert to do*/
    if(PNULL == str_ptr)
        return  FALSE;  

    if(!memcmp(str_ptr,"**05*",5))
    {
        return TRUE;
    }
    if(!memcmp(str_ptr,"**04*",5))
    {
        return TRUE;
    }
    if(!memcmp(str_ptr,"**052*",6))
    {
        return TRUE;
    }
    if(!memcmp(str_ptr,"**042*",6))
    {
        return TRUE;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : Parse the ss data from mmi
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN ParseSsRegisterData(
                                  char* ss_data
                                  )
{
    char                    *pos_ptr= PNULL,*pos_end_ptr = PNULL;
    BOOLEAN                 result = FALSE;
    
    //SCI_ASSERT(PNULL != ss_data);/* assert to do*/
    if(PNULL == ss_data)
        return  FALSE;  

    SCI_MEMSET(&g_mmiset_ss_pwd,0,sizeof(MMISET_SS_PWD_INFO_T));
    
    pos_ptr = ss_data;
    pos_end_ptr = ss_data + strlen(ss_data);
    
    if( *pos_ptr++ != '*')
    {
        return (result);
    }
    
    if('*' != *pos_ptr)
    {
        //decode SC
        DecodeSsDigitInfo(&pos_ptr,pos_end_ptr,&g_mmiset_ss_pwd.code);
        
        if(*pos_ptr++ == '#' && pos_ptr == pos_end_ptr)
        {
            return (result);
        }
    }
    else
    {
        g_mmiset_ss_pwd.code = 330;
        pos_ptr++;
    }
    
    //decode old password
    if(!DecodeSsStringInfo(&pos_ptr,pos_end_ptr,(char*)g_mmiset_ss_pwd.old_pwd,sizeof(g_mmiset_ss_pwd.old_pwd)))
    {
        return (result);
    }
    if(*pos_ptr++ != '*' || pos_ptr == pos_end_ptr)
    {
        //the format of the string is error.
        return (result);
    }
    
    //decode new password
    if(!DecodeSsStringInfo(&pos_ptr,pos_end_ptr,(char *)g_mmiset_ss_pwd.new_pwd,sizeof(g_mmiset_ss_pwd.new_pwd)))
    {
        return (result);
    }
    if(*pos_ptr++ != '*' || pos_ptr == pos_end_ptr)
    {
        //the format of the string is error.
        return (result);
    }
    
    //decode new password confirm
    if(!DecodeSsStringInfo(&pos_ptr,pos_end_ptr,(char *)g_mmiset_ss_pwd.new_pwd_two,sizeof(g_mmiset_ss_pwd.new_pwd)))
    {
        return (result);
    }
    if(*pos_ptr++ != '#' || pos_ptr == pos_end_ptr)
    {
        if(pos_ptr != pos_end_ptr)
        {
            //the format of the string is error.
            return (result);
        }
    }
    
    result = TRUE;
    return (result);
}

/*****************************************************************************/
//  Description : get sc of ss 
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN DecodeSsDigitInfo(
                                char      **string_ptr,
                                char      *pos_end_ptr,
                                uint16    *code_ptr
                                )
{
    BOOLEAN     result = FALSE;
    char        *pos_ptr = PNULL;
    
    //SCI_ASSERT(PNULL != string_ptr);/* assert to do*/
    //SCI_ASSERT(PNULL != *string_ptr);/* assert to do*/
    if(PNULL == string_ptr || PNULL == *string_ptr )
        return  FALSE;  

    pos_ptr = *string_ptr;
    while(*pos_ptr != '*' && *pos_ptr != '#' && pos_ptr < pos_end_ptr)
    {
        *code_ptr = *code_ptr *10 + *pos_ptr++ - '0';
    }
    
    *string_ptr = pos_ptr;
    
    result = TRUE;
    return (result);
}

/*****************************************************************************/
//  Description : get ss old password 
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN DecodeSsStringInfo(
                                 char   **string_ptr,
                                 char   *pos_end_ptr,
                                 char   *data,
                                 uint8  len_allowed    //The capability of the data
                                 )
{
    BOOLEAN result = FALSE;
    char    *pos_ptr = PNULL;
    uint8   i = 0;
    
    //SCI_ASSERT(PNULL != string_ptr);/* assert to do*/
    //SCI_ASSERT(PNULL != *string_ptr);/* assert to do*/
    if(PNULL == string_ptr || PNULL == *string_ptr )
        return  FALSE;  

    pos_ptr = *string_ptr;
    
    if(*pos_ptr != '*' && *pos_ptr != '#' && pos_ptr < pos_end_ptr)
    {
        while(*pos_ptr != '*' && *pos_ptr != '#')
        {
            if(i > len_allowed)
            {
                return (result);
            }
            data[i++] = *pos_ptr++;
            if (MMISET_SS_PWD_LEN<=i)
            {
                break;
            }
        }
    }
    *string_ptr = pos_ptr;
    
    result = TRUE;
    return (result);
}

/*****************************************************************************/
//  Description : Parse the ss data from mmi
//  Global resource dependence : none
//  Author:
//  Note:Form of the ss_data:SC*SIA*SIB*SIC#
/*****************************************************************************/
LOCAL BOOLEAN ParseSsData(
                          char                      *ss_data,
                          MMISET_SS_PARSE_INFO_T    *ss_out
                          )
{
    char    *pos_ptr = PNULL,*pos_end_ptr = PNULL;
    char    ss_code_str[MMISET_SS_CODE_LEN] = {0};
    BOOLEAN result = FALSE,is_valid = FALSE;
    uint8   i = 0,len = 0;
    
    SCI_MEMSET(ss_out,0,sizeof(MMISET_SS_PARSE_INFO_T));
    
    pos_ptr = ss_data;
    pos_end_ptr = ss_data + strlen(ss_data);
    
    //decode SC:turn string into digit
    while(*pos_ptr != '*' && *pos_ptr != '#' && pos_ptr < pos_end_ptr && len < MMISET_SS_CODE_LEN -1)
    {
        ss_code_str[len++] = *pos_ptr++;
    }
    ss_code_str[len] = '\0';
    
    //determine if the SC code is valid
    for(i = 0; i < ARR_SIZE(s_mmi_ss_translate); i++)
    {
        if(MMIAPICOM_Stricmp((uint8*)s_mmi_ss_translate[i].code_str, (uint8 *)ss_code_str) == 0)
        {
            ss_out->code = s_mmi_ss_translate[i].code;   
            is_valid = TRUE;
            break;
        }
    }
    if(!is_valid)
    {
        return (result);
    }
    
    //determine if the next character is valid .
    switch(*pos_ptr)
    {
    case '*':
        pos_ptr++;
        break;
        
    case '#':
        if(++pos_ptr == pos_end_ptr)
        {
            result = TRUE;
            return (result);
        }
        else
        {
            return (result);
        }
        //break;
        
    default:
        return (result);
    }
    
    //decode SIA
    if(*pos_ptr != '*' && *pos_ptr != '#' && pos_ptr < pos_end_ptr)
    {
        i= 0;
        ss_out->sia_is_present = TRUE;
        while(*pos_ptr != '*' && *pos_ptr != '#')
        {
            ss_out->sia[i++] = *pos_ptr++;
            if(pos_ptr == pos_end_ptr)
            {
                //the format of the string is error.
                return (result);
            }
        }
    }
    if(*pos_ptr++ == '#' && pos_ptr == pos_end_ptr)  // // modified in cr242461
    {
        if(ss_out->sia_is_present)
        {
            result = TRUE;
        }
        return (result);
    }
    
    //decode SIB
    if(*pos_ptr != '*' && *pos_ptr != '#' && pos_ptr < pos_end_ptr)
    {
        ss_out->sib_is_present = TRUE;
        while(*pos_ptr != '*' && *pos_ptr != '#')
        {
            ss_out->sib = ss_out->sib * 10 + (*pos_ptr++) - '0';
        }
    }
    if(*pos_ptr++ == '#' && pos_ptr == pos_end_ptr)
    {
        if(ss_out->sib_is_present)
        {
            result = TRUE;
        }
        return (result);
    }
    
    //decode SIC
    if(*pos_ptr != '*' && *pos_ptr != '#' && pos_ptr < pos_end_ptr)
    {
        ss_out->sic_is_present = TRUE;
        while(*pos_ptr != '*' && *pos_ptr != '#')
        {
            ss_out->sic = ss_out->sic * 10 + (*pos_ptr++) - '0';
        }
    }
    
    if(*pos_ptr++ == '#' && pos_ptr == pos_end_ptr)
    {
        if(ss_out->sic_is_present)
        {
            result = TRUE;
        }
        return (result);
    }
    else
    {
        return (result);
    }
}

/*****************************************************************************/
//  Description : Adjust it if is the ss code 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustSsCode(
                           uint16   *ss_code_ptr
                           )
{
    uint8       i = 0;
    BOOLEAN     result = FALSE;
    
    //SCI_ASSERT(PNULL != ss_code_ptr);/* assert to do*/
    if(PNULL == ss_code_ptr )
        return  FALSE;  

    for(i=0; i<ARR_SIZE(s_mmi_ss_translate); i++)
    {
        if(*ss_code_ptr == s_mmi_ss_translate[i].code)
        {
            *ss_code_ptr = s_mmi_ss_translate[i].ss_code;
            g_mmiset_ss_status.ss_code = s_mmi_ss_translate[i].ss_code;
            result = TRUE;
            break;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : transfer the ss code according to the array s_ss_basic_service
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN ComputerBasicService(
                                   MMISET_SS_PARSE_INFO_T   *ss_result,
                                   MN_SS_BASIC_SERVICE_T *basic_service  
                                   )
{
    uint32 i = 0;
    BOOLEAN return_value = FALSE;
    
    //SCI_ASSERT(PNULL != ss_result);/* assert to do*/
    //SCI_ASSERT(PNULL != basic_service);/* assert to do*/
    if(PNULL == ss_result||PNULL == basic_service)
        return  FALSE;  
  
    if(ss_result->sib_is_present)
    {
        for(i = 0; i < ARR_SIZE(s_mmi_ss_basic_service); i++)
        {
            if(ss_result->sib == s_mmi_ss_basic_service[i].sib)
            {
                basic_service->service_type = s_mmi_ss_basic_service[i].service_type;
                basic_service->service_code = s_mmi_ss_basic_service[i].service_code;
                return_value = TRUE;
                break;
            }
        }
    }
    else
    {
        basic_service->service_type = MN_SS_ALL_SERVICE;
        return_value = TRUE;
    }
    return return_value;
}


/*****************************************************************************/
//  Description : Handle divert numbers
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN HandleSsDivertNum(
                                MMISET_SS_PARSE_INFO_T  *ss_result_ptr,
                                MMISET_SS_INFO_T        *ss_info_ptr,
                                MN_SS_OPERATION_CODE_E ss_oper
                                )
{
    BOOLEAN                 result = TRUE;
    MN_CALLED_NUMBER_T	    number = {0};/*lint !e64*/
    MMI_PARTY_NUMBER_T      party_num ={0};/*lint !e64*/
    
    //SCI_MEMSET(&number, 0, sizeof(MN_CALLED_NUMBER_T));
    //SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));
    
    if(((MN_SS_CFU == ss_result_ptr->code) || (MN_SS_ALL_FORWARD == ss_result_ptr->code)) && ss_oper != MN_SS_ERASESS )
    {
        g_is_need_update_cfu_icon = TRUE;
    }
    else
    {
        g_is_need_update_cfu_icon = FALSE;
    }
    
    switch(ss_result_ptr->code)
    {
    case MN_SS_CFU:
    case MN_SS_CFB:  
    case MN_SS_CFNRC:
        //check sic(Time)
        if(ss_result_ptr->sic_is_present)
        {
            return FALSE;
        }
        //check sia(DN)
        if(ss_oper == MN_SS_REGISTERSS)  
        {
            //DN is mandatory in register operation
            if(!ss_result_ptr->sia_is_present)
            {
                return FALSE;
            }
        }
        else
        {
            //DN is only used in register operation
            if(ss_result_ptr->sia_is_present)
            {
                return FALSE;
            }
        }
        
        //set divert number
        if (ss_result_ptr->sia_is_present)
        {
            // 将语音信箱号码或者电话号码转换为所需要的格式，存入number。
            MMIAPICOM_GenPartyNumber((uint8*)ss_result_ptr->sia, (int16)strlen((char *)ss_result_ptr->sia), &party_num);
            
            //set register_param value
            number.number_type = party_num.number_type;
            number.number_plan = MN_NUM_PLAN_ISDN_TELE;
            number.num_len = party_num.num_len;
            MMI_MEMCPY(number.party_num,MN_MAX_ADDR_BCD_LEN,party_num.bcd_num,MN_MAX_ADDR_BCD_LEN,party_num.num_len);
            
            ss_info_ptr->register_param.forward_num = number;
        }
        break;

    case MN_SS_ALL_FORWARD:
    case MN_SS_ALL_COND_FORWARD:        
    case MN_SS_CFNRY:
        //check sia(DN)
        if(ss_oper == MN_SS_REGISTERSS)  
        {
            //DN is mandatory in register CFNRY/all cf/all conditional cf
            if(!ss_result_ptr->sia_is_present)
            {
                return FALSE;
            }

            //T is mandatory for register CFNRY,but is optional for 
            //register all cf and all conditional cf
            if(ss_result_ptr->code == MN_SS_CFNRY)
            {
                if(!ss_result_ptr->sic_is_present)
                {
                    ss_result_ptr->sic_is_present = TRUE;
                    ss_result_ptr->sic = 30;//MN_SS_DEFAULT_NO_REPLY_TIME;//default 30s
                }
            }
        }
        else
        {
            //DN and T is only used in register operation
            if((ss_result_ptr->sia_is_present) || (ss_result_ptr->sic_is_present))
            {
                return FALSE;
            }
        }
        
        if (ss_result_ptr->sia_is_present)
        {
            // 将语音信箱号码或者电话号码转换为所需要的格式，存入number。
            MMIAPICOM_GenPartyNumber((uint8*)ss_result_ptr->sia, (int16)strlen((char *)ss_result_ptr->sia), &party_num);
            
            //set register_param value
            number.number_type = party_num.number_type;
            number.number_plan = MN_NUM_PLAN_ISDN_TELE;
            number.num_len = party_num.num_len;
            MMI_MEMCPY(number.party_num,MN_MAX_ADDR_BCD_LEN,party_num.bcd_num,MN_MAX_ADDR_BCD_LEN,party_num.num_len);
            
            //set divert number and no reply time
            ss_info_ptr->register_param.cfnry_param.forward_num = number;
            
            //judge no reply time (5s-30s)
            if (ss_result_ptr->sic_is_present)
            {
                if ((ss_result_ptr->sic < MMISET_SS_MIN_NO_REPLY_TIME) ||
                    (ss_result_ptr->sic > MMISET_SS_MAX_NO_REPLY_TIME) || 
                    (ss_result_ptr->sic % MMISET_SS_STEP_REPLY_TIME))  // ref:22.080 3.3,in steps of 5 seconds 
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MSGBOX_NO_REPLY_TIME);
                    
                    // close idle dialing win
                    MMIAPIIDLE_CloseDialWin();
                    //关闭网络连接窗口
                    MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
                    result = FALSE;
                    return (result);
                }
                else
                {
                    ss_info_ptr->register_param.cfnry_param.no_reply_time = ss_result_ptr->sic;
                }
            }
        }
        break;
        
    default:
        break;
    }
    
    //除了查询时service应该是 all service,其他都是tele service 
    result = ComputerBasicService(ss_result_ptr,&(ss_info_ptr->basic_service));
    // ss_info_ptr->basic_service.service_type = MN_SS_TELE_SERVICE;
    //ss_info_ptr->basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
    
    return (result);
}

/*****************************************************************************/
//  Description : Send stand ss request to MN
//  Global resource dependence : none
//  Author:
//  Note:Form:SC*SI#
/*****************************************************************************/
LOCAL BOOLEAN SendReqToSs(
                          MN_DUAL_SYS_E             dual_sys,
                          MN_SS_OPERATION_CODE_E    ss_oper,
                          MMISET_SS_PARSE_INFO_T    *ss_result_ptr,
                          MMISET_SS_INFO_T          *ss_info_ptr,
                          uint8 *input_str,
                          uint8 str_len
                          )
{
    BOOLEAN             result = FALSE;
    SIMAT_DO_SS_STRING_T ss_string = {0};
    
    //SCI_ASSERT(PNULL != ss_result_ptr);/* assert to do*/
    //SCI_ASSERT(PNULL != ss_info_ptr);/* assert to do*/
    //SCI_ASSERT(PNULL != input_str);/* assert to do*/
    
    if (PNULL == ss_result_ptr || PNULL == ss_info_ptr || PNULL == input_str)
    {
        return  FALSE;  
    }

    if (MMISET_IsSSProcessing())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
        
        return result;
    }

#ifdef MMI_CSP_SUPPORT
    if (MMISET_IsAlsValid(dual_sys) && (MMISET_LINE_IS_LOCK == MMISET_GetLineLockStatus(dual_sys)))
    {
        MMIPUB_OpenAlertWarningWin(TXT_LINE_LOCK);
        return result;
    }
#endif  

    if (!MMIAPIPHONE_IsSimOk(dual_sys))
    {
        if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (!MMIAPIPHONE_IsSimAvailable(dual_sys))//该卡槽是否有卡并且待机
        {
            MMIAPIPHONE_AlertSimNorOKStatusII(dual_sys);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_EMERGENCYONLY);
        }

        return result;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();

        return result;
    }
#endif
    
    //更改ss是从idle输入的按码操作    
    g_is_for_cfu_icon = FALSE;

    //保存操作的service type 和service code
    g_mmiset_ss_status.basic_service.service_type = ss_info_ptr->basic_service.service_type;
    g_mmiset_ss_status.basic_service.service_code = ss_info_ptr->basic_service.service_code;

    if (ss_result_ptr->sia_is_present)
    {
        ss_string.ton = ss_info_ptr->register_param.forward_num.number_type;
        ss_string.npi = ss_info_ptr->register_param.forward_num.number_plan;
    }
    else
    {
        ss_string.ton = MN_NUM_TYPE_RESERVED;
        ss_string.npi = MN_NUM_PLAN_RESERVED;
    }
    //ss_string.len= ss_info_ptr->register_param.forward_num.num_len;
    ss_string.len = str_len;
    if (0 < ss_string.len)
    {
        MMI_MEMCPY(ss_string.ss_ptr, MN_MAX_SIMAT_DATA_OBJECT_LEN, input_str, PARS_MAX_STR_LEN, ss_string.len);/*lint !e685 */
    }    
    switch (ss_oper)
    {
    case MN_SS_REGISTERSS:        
    case MN_SS_ERASESS:
    case MN_SS_ACTIVATESS:
    case MN_SS_DEACTIVATESS:
        result = MNSS_SendSSEx(dual_sys, ss_oper, ss_result_ptr->code, ss_info_ptr->basic_service, ss_info_ptr->register_param, ss_string);
        if (MNSS_FAILURE == result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            // close idle dialing win
            MMIAPIIDLE_CloseDialWin();
            return (result);
        }
        else if (MNSS_FDN_FAILED == result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);

            // close idle dialing win
            MMIAPIIDLE_CloseDialWin();
            return (result);
        }
        break;
        
    case MN_SS_INTERROGATESS:
        result = MNSS_SendSSEx(dual_sys, ss_oper, ss_result_ptr->code, ss_info_ptr->basic_service, ss_info_ptr->register_param, ss_string);
        if (MNSS_FAILURE == result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            // close idle dialing win
            MMIAPIIDLE_CloseDialWin();
            return (result);
        }
        else if (MNSS_FDN_FAILED == result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);

            // close idle dialing win
            MMIAPIIDLE_CloseDialWin();
            return (result);
        }
        else
        {
            g_is_ss_user_query = TRUE;
        }
        break;
        
    case MN_SS_REGISTERPASSWORD:
        result = MNSS_SendSSEx(dual_sys, ss_oper, g_mmiset_ss_pwd.code, ss_info_ptr->basic_service, ss_info_ptr->register_param, ss_string);
        if (MNSS_FAILURE == result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            // close idle dialing win
            MMIAPIIDLE_CloseDialWin();
            return (result);
        }
        else if (MNSS_FDN_FAILED == result)
        {
            MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);

            // close idle dialing win
            MMIAPIIDLE_CloseDialWin();
            return (result);
        }
        break;
        
    default:
        return (result);
        //break;
    }

    //进入网络连接界面
    MMISET_OpenNetworkConnectWin(dual_sys,
                                 MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                 TXT_NETWORK_CONNECT,
                                 MMIPUB_SOFTKEY_ONE,
                                 HandleNetworkConnectWindow);
    
    result = TRUE;
    
    return (result);
}
#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
// 	Description : set is flipanswer
//	Global resource dependence : none
//  Author: jibin
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIsFlipAnswer(
                                                                      MMISET_FLIP_ANSWER_TYPE_T* flip_answer
                                                                      )
{
  //write minute reminder info to nv 
  MMINV_WRITE(MMINV_SET_IS_FLIPANSWER, flip_answer);
}
#endif
/*****************************************************************************/
// 	Description : Get Send SS repeat timer id
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetSendSSRepeatTimerId(void)
{
    return(g_send_ss_search_timer_id);
}

/*****************************************************************************/
// 	Description : Stop Send SS repeat timer
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_StopSendSSRepeatTimer(void)
{
    if (0 != g_send_ss_search_timer_id)
    {
        MMK_StopTimer(g_send_ss_search_timer_id);
        g_send_ss_search_timer_id = 0;
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : get another sim card
//	Global resource dependence : 
//  Author:shengjie.liu
//	Note:

// 1. 如果定时器已经启动，则取消定时器，发送新的后台
//        当 g_send_ss_search_timer_id != 0 && g_is_background_search_send = FALSE
//           说明没有向网络发送消息，重启定时器，更新g_background_ss_status

//   2. 如果消息已经发出去，则不再发送
//        当 g_send_ss_search_timer_id != 0 && g_is_background_search_send = TRUE
//           说明消息已经发送，该请求不处理

/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISET_GetAnotherSim(MN_DUAL_SYS_E  dual_sys)
{
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
    return MN_DUAL_SYS_MAX;
#else 
    MN_DUAL_SYS_E  result = MN_DUAL_SYS_MAX;
    uint16 sys_ok[MMI_DUAL_SYS_MAX] = {0,};
    uint32 num = 0;
    uint32 i = 0;

    SCI_MEMSET(sys_ok, MMI_DUAL_SYS_MAX, sizeof(sys_ok));
    num = MMIAPISET_GetMultiSysSettingNum(sys_ok, MMI_DUAL_SYS_MAX);
    if(0 == num || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return MN_DUAL_SYS_MAX;
    }

    result = (MN_DUAL_SYS_E)sys_ok[0];
    for(i = 0; i<num; i++)
    {
        if(sys_ok[i] > dual_sys)
        {
            result = (MN_DUAL_SYS_E)sys_ok[i];
            break;
        }
    }

    if(result == dual_sys)
    {
        result = MN_DUAL_SYS_MAX;
    }
    
    return result;

//NOTE!!! dual sim flow::: get another sim card
#if 0
    MN_DUAL_SYS_E  result = MN_DUAL_SYS_MAX;

    if (MN_DUAL_SYS_1 == dual_sys)
    {
        result = MN_DUAL_SYS_2;
    }
    else if (MN_DUAL_SYS_2 == dual_sys)
    {
        result = MN_DUAL_SYS_1;
    }
    else
    {
        result = MN_DUAL_SYS_MAX;
    }
    
    return result; // 防止编译警告
#endif
#endif
}

LOCAL void HandleSendSsSearchTimer(uint8 timer_id, uint32 param)
{
    //SCI_TRACE_LOW:"VirWin_HandleMsg: received SS repeat timer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5099_112_2_18_2_50_13_47,(uint8*)"");
    // 发送SS的查询命令给服务器，触发短信
    MMIAPISET_StopSendSSRepeatTimer();
    MMIAPISET_BgInterrogateStatus();    
}


/*****************************************************************************/
// 	Description : Interrogate Status on background 
//	Global resource dependence : 
//  Author:shengjie.liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_BgInterrogateStatus(void)
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    BOOLEAN             result = FALSE;
    
    //interrogate status
    if (MMISET_IsSSProcessing())
    {
        if (g_mmiset_ss_status.dual_sys != g_background_ss_status.dual_sys)
        {
            // 用户查询和我们查询不是同一张卡，需要查询，重启定时器
            if (0 == g_send_ss_search_timer_id)
            {
                g_send_ss_search_timer_id = MMK_CreateTimerCallback(                     
                    MMIAPICOM_GetRandomTimer(), 
                    HandleSendSsSearchTimer,
                    NULL, 
                    FALSE);
            }
            else
            {
                MMK_StartTimerCallback(
                    g_send_ss_search_timer_id,
                    MMIAPICOM_GetRandomTimer(),      
                    HandleSendSsSearchTimer,
                    NULL, 
                    FALSE);
            }
        }
    }
    else
    {
        if (!MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef MMI_GPRS_SUPPORT
            && !MMIAPIPDP_PsIsPdpLinkExist()
#endif
            )
        {
            //SCI_TRACE_LOW:"MMIAPISET_BgInterrogateStatus: interrogate service"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5149_112_2_18_2_50_14_48,(uint8*)"");
            // 用户没有发送请求，可以发送
            result = MNSS_SendSSEx(g_background_ss_status.dual_sys,
                                        MN_SS_INTERROGATESS,
                                        g_background_ss_status.ss_code,
                                        g_background_ss_status.basic_service,
                                        register_param,
                                        ss_string);

            if (MNSS_SUCCESS == result || MNSS_INFORM_STK == result)
            {
                g_is_background_search_send = TRUE;
            }
        }
    }
}


/*****************************************************************************/
// 	Description : set param for Interrogate Status
//	Global resource dependence : 
//  Author:shengjie.liu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_BgSetIgStatusParam(MN_DUAL_SYS_E  dual_sys)
{
#if (0)
    //SCI_TRACE_LOW:"MMIAPISET_BgSetIgStatusParam bg_snd = %d, ss_snd = %d, g_is_for_cfu_icon = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5175_112_2_18_2_50_14_49,(uint8*)"ddd", g_is_background_search_send, g_is_ss_user_query,g_is_for_cfu_icon);
    
    if (MMIAPISET_GetMultiSysSettingNum(NULL, 0)<2 || dual_sys >= MMI_DUAL_SYS_MAX) // 必须双卡都开启
    {
        // 单卡不需要处理
        //SCI_TRACE_LOW:"MMIAPISET_BgSetIgStatusParam:isnot MMISET_DAULSYS_ALL_SIM_ON"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5180_112_2_18_2_50_14_50,(uint8*)"");
        return FALSE;
    }
    
    if (g_is_background_search_send)
    {
        if (g_background_ss_status.dual_sys == dual_sys)
        {
            // 需要发送的请求，当前正在发送
            return FALSE;
        }
    }
    else if (g_is_ss_user_query || g_is_for_cfu_icon)
    {
        if (g_mmiset_ss_status.dual_sys == dual_sys)
        {
            // 用户正在发送需要的消息
            return FALSE;
        }
    }
    
    SCI_MEMSET(&g_background_ss_status, 0 ,sizeof(g_background_ss_status));
    // 设置数据
    g_background_ss_status.dual_sys = dual_sys;
    g_background_ss_status.ss_code = MN_SS_CW;
    g_background_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
    g_background_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
    
    // 启动定时器
    if (0 == g_send_ss_search_timer_id)
    {
        g_send_ss_search_timer_id = MMK_CreateTimerCallback(             
            MMIAPICOM_GetRandomTimer(), 
            HandleSendSsSearchTimer,
            NULL, 
            FALSE);
    }
    else
    {
        MMK_StartTimerCallback(
            g_send_ss_search_timer_id,
            MMIAPICOM_GetRandomTimer(),
            HandleSendSsSearchTimer,
            NULL,
            FALSE);
    }
#endif    
    return TRUE;
}

/*****************************************************************************/
// 	Description : set if is connect prompt open
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIsConnectPrompt(
                                      BOOLEAN  is_connect_prompt
                                      )
{
    MMINV_WRITE(MMINV_SET_CONNECT_PROMPT_STATUS,&is_connect_prompt);
}

/*****************************************************************************/
// 	Description : set if is auto redial
//	Global resource dependence : none
//  Author: scv
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsAutoRedial(
                                      BOOLEAN  is_auto_redial
                                      )
{
    //write auto redial set into NV
    MMINV_WRITE(MMI_CC_REDIAL_FLAG,&is_auto_redial);
}

/*****************************************************************************/
// 	Description : get is minute reminder open
//	Global resource dependence : none
//  Author: scv
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsAutoRedail(void)
{
    BOOLEAN                 is_auto_redial = DEFAULT_REDIAL;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get if is auto redial open
    MMINV_READ(MMI_CC_REDIAL_FLAG,&is_auto_redial,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {	
        is_auto_redial = DEFAULT_REDIAL;
        MMINV_WRITE(MMI_CC_REDIAL_FLAG,&is_auto_redial);
    }
    
    return (is_auto_redial);
}

/*****************************************************************************/
// 	Description : set if is minute reminder
//	Global resource dependence : none
//  Author: scv
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMinuteReminderTime(
                                       uint8  call_reminder_time
                                       )
{
    MMINV_WRITE(MMINV_SET_CALL_REMINDER_TIME,&call_reminder_time);
}

/*****************************************************************************/
// 	Description : get is minute reminder open
//	Global resource dependence : none
//  Author: scv
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetMinuteReminderTime(void)
{
    uint8 call_reminder_time = CC_CALL_REMINDER_TIME_DEFAULT;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_CALL_REMINDER_TIME,&call_reminder_time,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        call_reminder_time = CC_CALL_REMINDER_TIME_DEFAULT;
        MMISET_SetMinuteReminderTime(call_reminder_time);
    }
    
    return (call_reminder_time); 
}

/*****************************************************************************/
// 	Description : get is connect prompt open for cc
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPISET_GetIsConnectPrompt(void)
{
    BOOLEAN                 is_connect_prompt = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get if is connect prompt open
    MMINV_READ(MMINV_SET_CONNECT_PROMPT_STATUS,&is_connect_prompt,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
#if defined(MMI_PDA_SUPPORT)
        is_connect_prompt = TRUE;
#else
        is_connect_prompt = FALSE;
#endif
        MMINV_WRITE(MMINV_SET_CONNECT_PROMPT_STATUS,&is_connect_prompt);
    }
    
    return (is_connect_prompt);
}
#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
// 	Description : get is flipanswer
//	Global resource dependence : none
//  Author: jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsFlipAnswer(MN_DUAL_SYS_E dual_sys)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMISET_FLIP_ANSWER_TYPE_T flip_answer = {0};/*lint !e64*/
    
    //get if is is minute reminder open
    MMINV_READ(MMINV_SET_IS_FLIPANSWER, &flip_answer, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_IS_FLIPANSWER, &flip_answer);
    }
    return (flip_answer.is_open[dual_sys]);
}
#endif
/*****************************************************************************/
// 	Description : append reject and any key list item
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendAnykeyRejectListItem(
                                              MMI_CTRL_ID_T ctrl_id
                                              )
{
    MMISET_ANYKEY_REJECT_SET_T  mode_info_ptr = {0};
    
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    //add items to list box
    MMIAPISET_AppendListItemByTextId( TXT_SET_SMS_REPLY, STXT_SELECT, ctrl_id, GUIITEM_STYLE_ONE_LINE_CHECK );
#endif
    MMIAPISET_AppendListItemByTextId( TXT_SET_ANYKEY_ANSWER, STXT_SELECT, ctrl_id, GUIITEM_STYLE_ONE_LINE_CHECK );
    
    mode_info_ptr = MMIAPISET_GetAnykeyRejectMode();
    
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    //set selected item
    if (mode_info_ptr.is_sms_reply_open)
    {
        GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
    }
    
    if (mode_info_ptr.is_anykey_answer)
    {
        GUILIST_SetSelectedItem(ctrl_id, 1, TRUE);
    }	
#else
    if (mode_info_ptr.is_anykey_answer)
    {
        GUILIST_SetSelectedItem(ctrl_id, 0, TRUE);
    }	
#endif
}

/*****************************************************************************/
// 	Description : whether is net work connecting now
//	Global resource dependence : 
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsNetworkConnecting(void)
{
    return MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
}
/*****************************************************************************/
// 	Description : get the any key and reject mode from nv
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC MMISET_ANYKEY_REJECT_SET_T  MMIAPISET_GetAnykeyRejectMode(void)
{
    MMISET_ANYKEY_REJECT_SET_T  mode_info = {0};
    MN_RETURN_RESULT_E			return_value = MN_RETURN_FAILURE;
    
    //get the any key and reject mode from NV
    MMINV_READ(MMINV_SET_ANYKEY_REJECT_MODE,&mode_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        mode_info.is_anykey_answer = FALSE;

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        #ifdef CMCC_UI_STYLE
        mode_info.is_sms_reply_open = TRUE;
        #else
        mode_info.is_sms_reply_open = FALSE;
        #endif
#endif
        MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&mode_info);
    }
    
    return (mode_info);
}

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
// 	Description : register call forward 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_RegisterDualsysCFNRC(MN_DUAL_SYS_E dual_sys)
{
    //开机注册呼叫转移
    MN_SS_REGISTER_PARAM_U reg_param = {0};/*lint !e64*/
    SIMAT_DO_SS_STRING_T ss_string = {0};
    MMIPB_CONTACT_T *local_num = PNULL;  
    uint8 sim_num[MMISET_MAX_TELE_NUM_LEN] = {0};
    BOOLEAN get_name = FALSE;
    MN_CALLED_NUMBER_T called_number = {0};/*lint !e64*/
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    MN_DUAL_SYS_E another_sys = MN_DUAL_SYS_MAX;
    BOOLEAN             result = FALSE;
    uint16  i = 0;
    
    //SCI_ASSERT(dual_sys < MMI_DUAL_SYS_MAX);/* assert to do*/
    if(dual_sys >= MMI_DUAL_SYS_MAX)
        return  FALSE;  

    if (MMISET_IsSSProcessing())
    {        
        return FALSE;
    }

#if defined(MMI_MULTI_SIM_SYS_DUAL)    
    another_sys = (dual_sys == MN_DUAL_SYS_1)? MN_DUAL_SYS_2: MN_DUAL_SYS_1;
#endif

    //SCI_TRACE_LOW:"mmiset:MMISET_RegisterDualsysCFNRC dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5448_112_2_18_2_50_14_51,(uint8*)"d", dual_sys);
    
    local_num = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(local_num, 0x00, sizeof(MMIPB_CONTACT_T));
    //获取另一张卡的号码
    get_name = MMIAPIPB_ReadMsisdnContact(another_sys, local_num);
		SCI_TRACE_LOW("mmiset:MMISET_RegisterDualsysCFNRCget_name = %d",get_name);
		for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM;i++)
		{
			SCI_TRACE_LOW("mmiset:MMISET_RegisterDualsysCFNRClocal_num->number[%d]  len = %d,npi_ton = %d,number = %s",i,local_num->number[i].number_len,local_num->number[i].npi_ton,local_num->number[i].number);
		}   
    if (get_name)
    {
        called_number.num_len = MMIAPIPB_BCDNumberToString(&local_num->number[0], MMISET_MAX_TELE_NUM_LEN, sim_num);
//         called_number.num_len = MMIAPICOM_GenDispNumber(
//             MMIPB_GetNumberTypeFromUint8(local_num->number[0].npi_ton), 
//             local_num->number[0].number_len, 
//             local_num->number[0].number,
//             sim_num,
//             MMISET_MAX_TELE_NUM_LEN+2
//             );
    }
    else
    {
        SCI_FREE(local_num);
        return FALSE;
    }
    
    SCI_FREE(local_num);
    MMIAPICOM_GenPartyNumber(sim_num, strlen((const char*)sim_num), &party_num);
    
    //set register_param value
    called_number.number_type = party_num.number_type;
    called_number.number_plan = MN_NUM_PLAN_ISDN_TELE;
    called_number.num_len = party_num.num_len;
    MMI_MEMCPY(called_number.party_num, MN_MAX_ADDR_BCD_LEN, party_num.bcd_num, MN_MAX_ADDR_BCD_LEN, party_num.num_len);
    
    g_mmiset_ss_status.ss_code = MN_SS_CFNRC;
    
    g_mmiset_ss_status.basic_service.service_code = MN_SS_ALL_SPEECH_TRANS_SERVICES;
    g_mmiset_ss_status.basic_service.service_type = MN_SS_TELE_SERVICE;
    
    reg_param.cfnry_param.forward_num = called_number;
    
    g_mmiset_ss_status.dual_sys = dual_sys;
    
    //在一张卡上注册另一张卡的呼叫转移
    result = MNSS_SendSSEx(dual_sys, MN_SS_REGISTERSS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, reg_param, ss_string);

    if (MNSS_FDN_FAILED == result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
        return FALSE;
    }
    else if(MNSS_SUCCESS != result && MNSS_INFORM_STK != result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
        return FALSE;
    }
    
    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : query all cfu handle
//  Global resource dependence : none
//  Author:
//  Note:Form:
/*****************************************************************************/
LOCAL void HandleAllQueryCF(MN_DUAL_SYS_E dual_sys, uint8 list_num, APP_MN_INTERROGATE_CNF_T ss_info_ptr)
{
    MN_SS_REGISTER_PARAM_U register_param = {0};/*lint !e64*/// parameter's union of register ss data operation
    SIMAT_DO_SS_STRING_T ss_string = {0};
    BOOLEAN             result = FALSE;
    
    s_cf_info_ptr[(uint32)((uint32)s_query_cf_num - 1) * sizeof(APP_MN_INTERROGATE_CNF_T)] = ss_info_ptr;
    s_cf_list_num[s_query_cf_num - 1] = list_num;
    
    switch (s_query_cf_num)
    {
    case 1:
        s_query_cf_num++;
        //发起遇忙时呼叫转移查询；
        //SCI_TRACE_LOW:"mmiset: query all cfb"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5520_112_2_18_2_50_14_52,(uint8*)"");
        
        g_mmiset_ss_status.ss_code = MN_SS_CFB;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
        g_is_for_cfu_icon = FALSE;
        //interrogate status
        result = MNSS_SendSSEx(dual_sys, MN_SS_INTERROGATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
        break;
    case 2:
        s_query_cf_num++;
        //发起无应答转移查询；
        //SCI_TRACE_LOW:"mmiset: query all cfnry"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5531_112_2_18_2_50_14_53,(uint8*)"");
        
        g_mmiset_ss_status.ss_code = MN_SS_CFNRY;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
        g_is_for_cfu_icon = FALSE;
        //interrogate status
        result = MNSS_SendSSEx(dual_sys, MN_SS_INTERROGATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
        break;
    case 3:
        s_query_cf_num++;
        //发起无法到达转移查询；
        //SCI_TRACE_LOW:"mmiset: query all cfnrc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5542_112_2_18_2_50_14_54,(uint8*)"");
        
        g_mmiset_ss_status.ss_code = MN_SS_CFNRC;
        g_mmiset_ss_status.basic_service.service_type = MN_SS_ALL_SERVICE;
        g_is_for_cfu_icon = FALSE;
        //interrogate status
        result = MNSS_SendSSEx(dual_sys, MN_SS_INTERROGATESS, g_mmiset_ss_status.ss_code, g_mmiset_ss_status.basic_service, register_param, ss_string);
        break;
    case 4:
        s_query_cf_num =0;
        //显示查询信息
        GenAllCFReport();
        //SCI_TRACE_LOW:"mmiset: query all cf end"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5554_112_2_18_2_50_14_55,(uint8*)"");
        break;
    default:
        //出错；
        break;
    }
    
    if (MNSS_SUCCESS != result && MNSS_INFORM_STK != result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }
}
/*****************************************************************************/
//  Description : display the result of  all cfu query
//  Global resource dependence : none
//  Author:
//  Note:Form:
/*****************************************************************************/
LOCAL void GenAllCFReport(void)
{
    uint32 i = 0;

    if(PNULL == s_cf_info_ptr)
    {
        //SCI_TRACE_LOW:"s_cf_info_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5576_112_2_18_2_50_14_56,(uint8*)"");
        return;
    }
    
    //创建查询结果窗口
    MMK_CreateWin((uint32*)MMISET_SS_ALL_REPORT_WIN_TAB,PNULL);
    do
    {
        MMI_STRING_T        name_str = {0};
        //GUIRICHTEXT_ITEM_T  item = {0};
        //uint16				index = 0;
        //SCI_MEMSET(&name_str,0,sizeof(MMI_STRING_T));
        
        switch(i)
        {
        case 0:
            MMI_GetLabelTextByLang(TXT_SET_DIVERTS_ALL_CALLS,&name_str);
            break;
        case 1:
            MMI_GetLabelTextByLang(TXT_SET_DIVERTS_WHEN_BUSY,&name_str);
            break;
        case 2:
            MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NO_ANSWER,&name_str);
            break;
        case 3:
            MMI_GetLabelTextByLang(TXT_SET_DIVERTS_NOT_REACHABLE,&name_str);
            break;
        default:
            //do nothing
            break;
        }
        //GUILABEL_SetText(MMISET_SS_INTERROGATE_NAME_CTRL_ID,&name_str,FALSE);
        //		item.type = GUIRICHTEXT_ITEMTYPE_TEXT;
        //		item.data.text.has_icon = FALSE;
        //		item.data.text.is_ucs2 = name_str.is_ucs2;
        //		item.data.text.str_len = name_str.length;
        //strcpy(item.data.text.str, name_str.str_ptr);
        //		SCI_MEMCPY(item.data.text.str, name_str.str_ptr, name_str.length);
        //		GUIRICHTEXT_AddItem(MMISET_SS_ALL_REPORT_CTRL_ID, &item, &index);
        AddStrToReportText(name_str, CONTENT_TYPE_SERVIEC_NAME);
        
        GenInterrogateStatusReport(s_cf_list_num[i], s_cf_info_ptr[i * sizeof(APP_MN_INTERROGATE_CNF_T)].info_list);
        
#ifdef MMI_DUALMODE_ENABLE
        GenInterrogateVTStatusReport(s_cf_list_num[i], s_cf_info_ptr[i * sizeof(APP_MN_INTERROGATE_CNF_T)].info_list);
#endif
        i++;
    }while(i < MMISET_ALL_INTERROGATE_NUM);

    SCI_FREE(s_cf_info_ptr);
    s_cf_info_ptr = PNULL;
}

/*****************************************************************************/
// 	Description : add str to MMISET_SS_ALL_REPORT_CTRL
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
LOCAL void AddStrToReportText( MMI_STRING_T str, CONTENT_TYPE_E type)
{
    GUIRICHTEXT_ITEM_T  item = {0};/*lint !e64*/
    uint16				index = 0;

    item.img_type = GUIRICHTEXT_IMAGE_NONE;
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.len = str.wstr_len;
    item.text_data.buf.str_ptr = str.wstr_ptr;

#if 0
    switch(type)
    {
        case CONTENT_TYPE_SERVIEC_NAME:
            item.text_set_font_color = TRUE;
#ifdef DPHONE_SUPPORT 
            item.setting_font_color = MMI_DEFAULT_TEXT_COLOR;
#else						
            item.setting_font_color = MMI_LIGHT_BLUE_COLOR;
#endif 
            item.text_set_font = TRUE;
            item.setting_font = MMI_DEFAULT_BIG_FONT;
    	    break;

        case CONTENT_TYPE_CALL_CLASS:
            item.text_set_font_color = TRUE;
#ifdef DPHONE_SUPPORT 
            item.setting_font_color = MMI_DEFAULT_TEXT_COLOR;
#else		
            item.setting_font_color = MMI_YELLOW_COLOR;
#endif
            item.text_set_font = TRUE;
            item.setting_font = MMI_DEFAULT_NORMAL_FONT;
            break;    

        case CONTENT_TYPE_SERVICE_STATUSE:
            item.text_set_font = TRUE;
            item.setting_font = MMI_DEFAULT_NORMAL_FONT;
            break;
            
        default:
            SCI_TRACE_LOW("AddStrToReportText error param!");/* assert to do*/
            break;
    }
#endif
    
    GUIRICHTEXT_AddItem(MMISET_SS_ALL_REPORT_CTRL_ID, &item, &index);
}
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
// 	Description : Get IP number
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: TRUE: there is active IP number  FALSE: there is no active IP number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIPNumber(
                                     MN_DUAL_SYS_E dual_sys,
                                     uint8* origin_number_ptr, 
                                     uint8* ipnumber_str_ptr, 
                                     uint8* len
                                     )
{
    uint8 ip_id = 0;
    MMISET_IP_NUMBER_INFO_T ip_info = {0};/*lint !e64*/
    
    if (PNULL == ipnumber_str_ptr || PNULL == len || PNULL == origin_number_ptr)
    {
        return FALSE;
    }
    MMISET_GetIPNumberInfo(dual_sys, &ip_info);
    
    for (ip_id = 0; ip_id < MMISET_IPNUMBER_MAX_NUM; ip_id++)
    {
        if (( strncmp((const char*)origin_number_ptr, (char *)ip_info.number[ip_id], strlen((char *)ip_info.number[ip_id])) == 0 ))
        {
            *len = strlen((char *)ip_info.number[ip_id]);
            
            MMI_MEMCPY((uint8*)ipnumber_str_ptr, *len, (uint8*)ip_info.number[ip_id], MMISET_IPNUMBER_MAX_LEN, *len);
            ipnumber_str_ptr[*len] = 0;
            return TRUE;
        }
    }
    return FALSE;
}
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get is touch vibrate open
//	Global resource dependence : none
//   Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsTouchVibrate(void)
{
    uint8    mode_id =0;

    mode_id = MMIAPIENVSET_GetActiveModeId();
    return (MMIAPIENVSET_GetModeDialTPVibrate(mode_id));
}

/*****************************************************************************/
// 	Description : set touch vibrate state
//	Global resource dependence : none
//   Author: kun.yu
//	Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetTouchVibrateState(BOOLEAN is_touch_vibrate)
//{
//    MMINV_WRITE(MMINV_SET_TOUCH_VIBRATE_FLAG,&is_touch_vibrate);
//}
#endif

#ifdef VT_SUPPORT
/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/ 
PUBLIC void MMISET_AppendFallbackListItem(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_selection = 0;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_OPTION_FALLBACK_QUERY, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_OPTION_FALLBACK_QUIT, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_OPTION_FALLBACK_AUTO, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current line
    cur_selection = (uint16)MMIAPISET_GetFallbackHandleMode();
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

}  

/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectFallback(MMI_CTRL_ID_T ctrl_id)
{
    MMISET_VT_FALLBACK_HANDLE_MODE_E cur_selection = (MMISET_VT_FALLBACK_HANDLE_MODE_E)GUILIST_GetCurItemIndex(ctrl_id);
    
    MMINV_WRITE(MMINV_SET_VT_FALLBACK_MODE, &cur_selection);

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
}

/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC MMISET_VT_FALLBACK_HANDLE_MODE_E MMIAPISET_GetFallbackHandleMode(void)
{
    MMISET_VT_FALLBACK_HANDLE_MODE_E fallback_mode =  MMISET_VT_FALLBACK_QUERY;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    

    MMINV_READ(MMINV_SET_VT_FALLBACK_MODE, &fallback_mode, return_value);

    if (MN_RETURN_SUCCESS != return_value || fallback_mode >= MMISET_VT_FALLBACK_MAX)
    {
        //SCI_TRACE_LOW:"mmivt GetFallbackHandleMode :%d ret:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5789_112_2_18_2_50_15_57,(uint8*)"dd", fallback_mode, return_value);
        fallback_mode =  MMISET_VT_FALLBACK_QUERY;
        MMINV_WRITE(MMINV_SET_VT_FALLBACK_MODE, &fallback_mode);
    }

    return fallback_mode;
} 

/*****************************************************************************/
// 	Description : MMISET_WriteReplaceInfoToNv
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_WriteReplaceInfoToNv(MMISET_VT_REPLACE_FILE_INFO_T* replace_file_info_ptr)
{    
    MMINV_WRITE(MMINV_SET_VT_REPLACE_INFO, replace_file_info_ptr);
}

/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetReplaceInfoFromNv(MMISET_VT_REPLACE_FILE_INFO_T* out_replace_file_info_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    

    MMINV_READ(MMINV_SET_VT_REPLACE_INFO, out_replace_file_info_ptr, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(out_replace_file_info_ptr, 0, MMISET_VT_REPLACE_MAX*sizeof(MMISET_VT_REPLACE_FILE_INFO_T));
        MMINV_WRITE(MMINV_SET_VT_REPLACE_INFO, out_replace_file_info_ptr);
    }
} 

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRecordRVDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT录制远端视频和混音的位置 从nv
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPISET_GetVTRecordRVDAPath(void)
{
    MMIFILE_DEVICE_E record_path = MMI_DEVICE_UDISK;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    

    MMINV_READ(MMINV_SET_VT_RECORD_RV_DA_PATH, &record_path, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        record_path = MMIAPIFMM_GetDefaultDisk();
        MMINV_WRITE(MMINV_SET_VT_RECORD_RV_DA_PATH, &record_path);
    }
    return record_path;
}

/*****************************************************************************/
// 	Description : MMISET_SetVTRecordRVDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT录制远端视频和混音的位置，存入nv
/*****************************************************************************/
PUBLIC void MMISET_SetVTRecordRVDAPath(MMIFILE_DEVICE_E record_path)
{
   // MMIFILE_DEVICE_E record_path = (MMIFILE_DEVICE_E)cur_ind;
    if(record_path < MMI_DEVICE_NUM)
    {
        MMINV_WRITE(MMINV_SET_VT_RECORD_RV_DA_PATH, &record_path);
    }
    else
    {
        //SCI_TRACE_LOW:"MMISET_SetVTRecordRVDAPath error record_path %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5857_112_2_18_2_50_15_58,(uint8*)"d", record_path);
    }
}

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRecordDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT录制混音的位置 从nv
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPISET_GetVTRecordDAPath(void)
{
    MMIFILE_DEVICE_E record_path = MMI_DEVICE_UDISK;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    

    MMINV_READ(MMINV_SET_VT_RECORD_DA_PATH, &record_path, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        record_path = MMIAPIFMM_GetDefaultDisk();
        MMINV_WRITE(MMINV_SET_VT_RECORD_DA_PATH, &record_path);
    }
    return record_path;
}

/*****************************************************************************/
// 	Description : MMISET_SetVTRecordDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT录制混音的位置，存入nv
/*****************************************************************************/
PUBLIC void MMISET_SetVTRecordDAPath(MMIFILE_DEVICE_E device)
{
    if(device < MMI_DEVICE_NUM)
    {
        MMINV_WRITE(MMINV_SET_VT_RECORD_DA_PATH, &device);
    }
    else
    {
        //SCI_TRACE_LOW:"MMISET_SetVTRecordDAPath error device %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5893_112_2_18_2_50_15_59,(uint8*)"d", device);
    }    
}

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRecordRVPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT录制远端视频的位置 从nv
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPISET_GetVTRecordRVPath(void)
{
    MMIFILE_DEVICE_E record_path = MMI_DEVICE_UDISK;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    

    MMINV_READ(MMINV_SET_VT_RECORD_RV_PATH, &record_path, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        record_path = MMIAPIFMM_GetDefaultDisk();
        MMINV_WRITE(MMINV_SET_VT_RECORD_RV_PATH, &record_path);
    }
    return record_path;
}

/*****************************************************************************/
// 	Description : MMISET_SetVTRecordRVPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT录制远端视频的位置，存入nv
/*****************************************************************************/
PUBLIC void MMISET_SetVTRecordRVPath(MMIFILE_DEVICE_E device)
{
    if(device < MMI_DEVICE_NUM)
    {
        MMINV_WRITE(MMINV_SET_VT_RECORD_RV_PATH, &device);
    }
    else
    {
        //SCI_TRACE_LOW:"MMISET_SetVTRecordRVPath error device %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_5929_112_2_18_2_50_15_60,(uint8*)"d", device);
    }        
}

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTOneTimeRingOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT是否开启一次振铃
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetVTOneTimeRingOption(void)
{
    BOOLEAN set_option = TRUE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    

    MMINV_READ(MMINV_SET_VT_ONE_TIME_RING_OPTION, &set_option, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        set_option = TRUE;
        MMINV_WRITE(MMINV_SET_VT_ONE_TIME_RING_OPTION, &set_option);
    }
    return set_option;
}


/*****************************************************************************/
// 	Description : MMIAPISET_SetVTOneTimeRingOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT是否开启一次振铃，存入nv
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVTOneTimeRingOption(BOOLEAN set_option)
{
    MMINV_WRITE(MMINV_SET_VT_ONE_TIME_RING_OPTION, &set_option);
}

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRemoteShutdownCameraOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT对方关闭摄像头处理方式的位置
/*****************************************************************************/
PUBLIC MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION MMIAPISET_GetVTRemoteShutdownCameraOption(void)
{
    MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION set_option = MMISET_VT_REPLACE_REMOTE_IMG;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    

    MMINV_READ(MMINV_SET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION, &set_option, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        set_option = MMISET_VT_REPLACE_REMOTE_IMG;
        MMINV_WRITE(MMINV_SET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION, &set_option);
    }
    return set_option;
}


/*****************************************************************************/
// 	Description : MMIAPISET_SetVTRemoteShutdownCameraOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT对方关闭摄像头处理方式的位置，存入nv
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVTRemoteShutdownCameraOption(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION set_option)
{
    //SCI_ASSERT(set_option < MMISET_VT_REMOTE_SHUTDOWN_CAMERA_MAX);/* assert to do*/
    if(set_option < MMISET_VT_REMOTE_SHUTDOWN_CAMERA_MAX)
    {
        MMINV_WRITE(MMINV_SET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION, &set_option);
    }
}

/*****************************************************************************/
// 	Description : MMISET_ResetVTSetting
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:reset nv里面的VT相关设置
/*****************************************************************************/
PUBLIC void MMISET_ResetVTSetting(void)
{
    MMISET_VT_FALLBACK_HANDLE_MODE_E fall_back_handle = MMISET_VT_FALLBACK_QUERY;
    MMISET_VT_REPLACE_FILE_INFO_T replace_file_info[MMISET_VT_REPLACE_MAX] = {0};
    MMIFILE_DEVICE_E record_path = MMI_DEVICE_UDISK;
    BOOLEAN is_one_time_ring = TRUE;
    MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION set_option = MMISET_VT_REPLACE_REMOTE_IMG;    

    record_path = MMIAPIFMM_GetDefaultDisk();    
    MMINV_WRITE(MMINV_SET_VT_FALLBACK_MODE, &fall_back_handle);
    MMINV_WRITE(MMINV_SET_VT_REPLACE_INFO, replace_file_info);
    MMINV_WRITE(MMINV_SET_VT_RECORD_RV_DA_PATH, &record_path);    
    MMINV_WRITE(MMINV_SET_VT_RECORD_DA_PATH, &record_path);    
    MMINV_WRITE(MMINV_SET_VT_RECORD_RV_PATH, &record_path);   
    MMINV_WRITE(MMINV_SET_VT_ONE_TIME_RING_OPTION, &is_one_time_ring);  
    MMINV_WRITE(MMINV_SET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION, &set_option);  

}

#endif // vt_support

/*****************************************************************************/
// 	Description : is ss processing
//	Global resource dependence : none
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsSSProcessing(void)
{
    BOOLEAN is_network_connecting = FALSE;

    is_network_connecting = MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);

    //SCI_TRACE_LOW:"MMISET_IsSSProcessing g_is_background_search_send=%d, g_is_ss_user_query=%d, g_is_for_cfu_icon=%d,is_network_connecting=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_6032_112_2_18_2_50_15_61,(uint8*)"dddd",g_is_background_search_send, g_is_ss_user_query, g_is_for_cfu_icon, is_network_connecting);

    if (g_is_background_search_send || g_is_ss_user_query || g_is_for_cfu_icon || is_network_connecting)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : Set SS Status
//	Global resource dependence : none
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetSSStatus(MN_DUAL_SYS_E dual_sys, MN_SS_CODE_E ss_code, MN_SS_BASIC_SERVICE_T basic_service)
{
    //SCI_TRACE_LOW:"MMIAPISET_SetSSStatus dual_sys=%d, ss_code=%d, service_type=%d, service_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_6049_112_2_18_2_50_15_62,(uint8*)"dddd",dual_sys, ss_code, basic_service.service_type, basic_service.service_code);

    g_mmiset_ss_status.dual_sys = dual_sys;
    g_mmiset_ss_status.ss_code = ss_code;
    g_mmiset_ss_status.basic_service.service_type = basic_service.service_type;
    g_mmiset_ss_status.basic_service.service_code = basic_service.service_code;
}

/*****************************************************************************/
//Description : Get SS SIM(Not In SS return MMI_DUAL_SYS_MAX)
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISET_GetSSSIM(void)
{
    if (MMISET_IsSSProcessing())
    {
        return g_mmiset_ss_status.dual_sys;
    }

    return MMI_DUAL_SYS_MAX;
}

/*****************************************************************************/
// 	Description : set is cfu actived
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIsCfuActived(MN_DUAL_SYS_E dual_sys, MMISET_SS_TYPE_E ss_type, BOOLEAN is_actived)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    switch (ss_type)
    {
    case MMISET_SS_AUDIO:
        s_is_ss_cfu_actived[dual_sys][ss_type] = is_actived;
        break;

    case MMISET_SS_DATA:
        s_is_ss_cfu_actived[dual_sys][ss_type] = is_actived;
        break;

    default:
        s_is_ss_cfu_actived[dual_sys][MMISET_SS_AUDIO] = is_actived;
        s_is_ss_cfu_actived[dual_sys][MMISET_SS_DATA] = is_actived;
        break;
    }

    //SCI_TRACE_LOW:"MMISET_SetIsCfuActived dual_sys=%d, ss_type=%d, is_actived=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_6098_112_2_18_2_50_15_63,(uint8*)"ddd", dual_sys, ss_type, is_actived);
}

/*****************************************************************************/
// 	Description :get is cfu actived
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetIsCfuActived(MN_DUAL_SYS_E dual_sys, MMISET_SS_TYPE_E ss_type)
{
    BOOLEAN is_ss_cfu_actived = FALSE;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    switch (ss_type)
    {
    case MMISET_SS_AUDIO:
        is_ss_cfu_actived = s_is_ss_cfu_actived[dual_sys][ss_type];
        break;

    case MMISET_SS_DATA:
        is_ss_cfu_actived = s_is_ss_cfu_actived[dual_sys][ss_type];
        break;

    default:
        is_ss_cfu_actived = (s_is_ss_cfu_actived[dual_sys][MMISET_SS_AUDIO]
#ifdef VT_SUPPORT
                                        || s_is_ss_cfu_actived[dual_sys][MMISET_SS_DATA]
#endif
                                        );
        break;
    }

    //SCI_TRACE_LOW:"MMISET_GetIsCfuActived dual_sys=%d, ss_type=%d, is_ss_cfu_actived=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_6133_112_2_18_2_50_15_64,(uint8*)"ddd", dual_sys, ss_type, is_ss_cfu_actived);

    return is_ss_cfu_actived;
}

#ifdef MV_SUPPORT
/*****************************************************************************/
// 	Description : set if magic voice open
//	Global resource dependence : none
//	Author: violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsMagicVoice(BOOLEAN is_on)
{
    MMINV_WRITE(MMINV_SET_MAGIC_VOICE_STATUS, &is_on);
}
/*****************************************************************************/
// 	Description : get if magic voice open
//	Global resource dependence : none
//	Author: violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsMagicVoice(void)
{
    BOOLEAN		is_on = FALSE;
    uint32		return_value = 0;
    
    MMINV_READ(MMINV_SET_MAGIC_VOICE_STATUS, &is_on, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_MAGIC_VOICE_STATUS, &is_on);
    }
    return is_on;
}
/*****************************************************************************/
// 	Description : set magic voice info
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMagicVoiceInfo(MMISET_MV_INFO_T mv_info)
{
    MMINV_WRITE(MMINV_SET_MAGIC_VOICE_SETTING_INFO, &mv_info);
}
/*****************************************************************************/
// 	Description : get magic voice info
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetMagicVoiceInfo(MMISET_MV_INFO_T *mv_info_ptr)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    //SCI_ASSERT(PNULL != mv_info_ptr);/* assert to do*/
    if(PNULL != mv_info_ptr)
    {
        MMINV_READ(MMINV_SET_MAGIC_VOICE_SETTING_INFO, mv_info_ptr, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(mv_info_ptr, 0, sizeof(mv_info_ptr));
            MMINV_WRITE(MMINV_SET_MAGIC_VOICE_SETTING_INFO, mv_info_ptr);
        }
    }
}

/*****************************************************************************/
// 	Description : get magic voice info
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPISET_GetMagicVoiceRes(void)
{
    MMIAUD_RING_DATA_INFO_T mv_data = {0};

    MMI_GetRingInfo(MVOICE_RES, &mv_data);

    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_GetMagicVoiceRes dat_ptr= 0x%x, data_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_CALL_6201_112_2_18_2_50_16_65,(uint8*)"dd", mv_data.data_ptr, mv_data.data_len);
    return mv_data.data_ptr;
}

#endif

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Set the any key and reject mode to nv
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetAnykeyRejectMode(MMISET_ANYKEY_REJECT_SET_T *anykey_setting_ptr)
{    
    if (PNULL != anykey_setting_ptr)
    {
        MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE, anykey_setting_ptr);

        return TRUE;
    }

    return FALSE;
}
#endif

#ifdef MMI_AUTO_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : Get auto record flag
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetAutoRecordSetting(void)
{
    BOOLEAN is_auto_record=FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_AUTO_RECORD,&is_auto_record,return_value);
    if (return_value == MN_RETURN_FAILURE)
    {
        is_auto_record = FALSE;
        MMINV_WRITE(MMINV_SET_AUTO_RECORD,&is_auto_record);
    }
    
    return is_auto_record;
}

/*****************************************************************************/
// 	Description : Set auto record flag
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetAutoRecordSetting(BOOLEAN is_autorecord)
{
    MMINV_WRITE(MMINV_SET_AUTO_RECORD,&is_autorecord);
    return;
}
#endif

/*****************************************************************************/
// 	Description : is als valid
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsCfuStatusSavedInSim(
                                       MN_DUAL_SYS_E dual_sys
                                       ) 
{
    BOOLEAN result = FALSE;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_TRACE_LOW("MMISET_IsCfuStatusSavedInSim err sim:%d", dual_sys);
        return FALSE;
    }
#ifdef MMI_CPHS_SUPPORT    
    if(MMISET_IsCphsValid(dual_sys))
    {
        result = s_cphs_info[dual_sys].is_cfu_saved;
    }
#endif    
    return result;
}


