/*****************************************************************************
** File Name:      mmiset_call_export.h                                           *
** Author:                                                                   *
** Date:           2012/02/10                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li            Create
** 2012/02/10    xiyuan.ma           Edit
******************************************************************************/

#ifndef _MMISET_CALL_EXPORT_H
#define _MMISET_CALL_EXPORT_H

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiset_security.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define DEFAULT_REDIAL                      FALSE   //自动重拨默认关闭
//用来计算是否要发提示音的中间数值
#define CC_CALL_REMINDER_TIME_OFF  0//关闭
#define CC_CALL_REMINDER_TIME_50S  50
#define CC_CALL_REMINDER_TIME_55S  55
#define CC_CALL_REMINDER_TIME_60S  60
#define CC_CALL_REMINDER_TIME_DEFAULT  CC_CALL_REMINDER_TIME_OFF //通话提示音默认为关闭
#define CC_CALL_REMINDER_TIME_MAX 4//通话提示音默认选项数
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
//the type of operate ss cnf
typedef enum
{
    MMISET_SS_REGISTER,     //登记附加业务
        MMISET_SS_ERASE,        //删除附加业务
        MMISET_SS_ACTIVATE,     //激活附加业务
        MMISET_SS_DEACTIVATE    //去活附加业务
} MMISET_SS_OPERATE_TYPE_E;

// SS 状态
typedef struct
{
    MN_DUAL_SYS_E           dual_sys;//标示所使用的sim卡
    MN_SS_CODE_E    	    ss_code;
    MN_SS_BASIC_SERVICE_T 	basic_service;
} MMISET_SS_STATUS_T;

typedef struct 
{
    MN_DUAL_SYS_E dual_sys;
    MMISET_IP_NUMBER_T number[MMISET_IPNUMBER_MAX_NUM];
    uint8 active_id;
}MMISET_IP_NUMBER_INFO_T;

typedef struct
{
    BOOLEAN is_context_valid;
    MMISET_IP_NUMBER_INFO_T  ip_num_info[MMI_DUAL_SYS_MAX];
}MMISET_IP_NUMBER_CONTEXT_T;

typedef struct 
{
	BOOLEAN is_open[MMI_DUAL_SYS_MAX];
}MMISET_FLIP_ANSWER_TYPE_T;

// ss 密码
typedef struct
{
    uint16      code;                           //ss service code
    uint8       old_pwd[MMISET_SS_PWD_LEN];     //old password
    uint8       new_pwd[MMISET_SS_PWD_LEN];     //new password
    uint8       new_pwd_two[MMISET_SS_PWD_LEN]; //new pasword
}MMISET_SS_PWD_INFO_T;

// ss parse info
typedef struct
{
    uint16  code;
    BOOLEAN sia_is_present;
    BOOLEAN sib_is_present;
    BOOLEAN sic_is_present;
    char    sia[MN_MAX_ADDR_BCD_LEN];
    uint8   sib;
    uint16  sic;
}MMISET_SS_PARSE_INFO_T;

typedef struct
{
    uint16          code;
    uint8           code_str[MMISET_SS_CODE_LEN];
    MN_SS_CODE_E    ss_code;
}MMI_SS_CODE_TRANSLATE_T;

typedef struct
{
    uint8   sib;
    MN_SS_BASIC_SERVICE_E		service_type;   
    uint16          	service_code;  
}MMI_SS_BS_TRANSFER_T;

//basic_service and register info
typedef struct
{
    MN_SS_BASIC_SERVICE_T	basic_service;
    MN_SS_REGISTER_PARAM_U	register_param;
}MMISET_SS_INFO_T;

#ifdef MV_SUPPORT
typedef enum
{
    MMISET_MV_CHANGE = 0,
    MMISET_MV_ROLE,
#ifdef MV_EFFECT_SUPPORT/*当mips不够时，去掉所有音效*/
    MMISET_MV_EFFECT,
#endif    
    MMISET_MV_EQ,
    MMISET_MV_BG,
    MMISET_MV_BG_VOL,
    MMISET_MV_MV_VOL,
    
    MMISET_MV_MAX_NUM,

} MMISET_MV_ITEM_E;

typedef struct
{
    uint32 mv_value[MMISET_MV_MAX_NUM];
} MMISET_MV_INFO_T;

#endif

typedef enum
{
    MMISET_SS_AUDIO,
    MMISET_SS_DATA,
    MMISET_SS_MAX
}MMISET_SS_TYPE_E;

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
                                     );
#endif
#ifdef VT_SUPPORT
/*****************************************************************************/
// 	Description : MMIAPISET_GetVTOneTimeRingOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT是否开启一次振铃
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetVTOneTimeRingOption(void);

/*****************************************************************************/
// 	Description : MMIAPISET_SetVTOneTimeRingOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT是否开启一次振铃，存入nv
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVTOneTimeRingOption(BOOLEAN set_option);

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRemoteShutdownCameraOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT对方关闭摄像头处理方式的位置
/*****************************************************************************/
PUBLIC MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION MMIAPISET_GetVTRemoteShutdownCameraOption(void);

/*****************************************************************************/
// 	Description : MMIAPISET_SetVTRemoteShutdownCameraOption
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:设置VT对方关闭摄像头处理方式的位置，存入nv
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVTRemoteShutdownCameraOption(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION set_option);
#endif

/*****************************************************************************/
// 	Description : Set SS Status
//	Global resource dependence : none
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetSSStatus(MN_DUAL_SYS_E dual_sys, MN_SS_CODE_E ss_code, MN_SS_BASIC_SERVICE_T basic_service);

#ifdef MV_SUPPORT
/*****************************************************************************/
// 	Description : set if magic voice open
//	Global resource dependence : none
//	Author: violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsMagicVoice(BOOLEAN is_on);
/*****************************************************************************/
// 	Description : get if magic voice open
//	Global resource dependence : none
//	Author: violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsMagicVoice(void);

/*****************************************************************************/
// 	Description : set magic voice info
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMagicVoiceInfo(MMISET_MV_INFO_T mv_info);

/*****************************************************************************/
// 	Description : get magic voice info
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetMagicVoiceInfo(MMISET_MV_INFO_T *mv_info_ptr);

/*****************************************************************************/
// 	Description : get magic voice info
//	Global resource dependence : none
//	Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPISET_GetMagicVoiceRes(void);
#endif

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// Description : open set prefer network win
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPreferNetWorkWin(void);
#endif
#ifdef MMI_CPHS_SUPPORT
/*****************************************************************************/
// 	Description : is csp valid by line
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCphsValid(
                                       MN_DUAL_SYS_E dual_sys
                                       );

#endif //MMI_CPHS_SUPPORT

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : is csp valid by line
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCspValid(
                                       MN_DUAL_SYS_E dual_sys,
                                       uint8    line
                                       );

/*****************************************************************************/
// 	Description : get csp1 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: the s_cphs_info[dual_sys].is_csp1_valid should be TRUE, else return PNULL
/*****************************************************************************/
PUBLIC MN_CSP_SERVICES_GROUP_T* MMISET_GetCsp1Service(
                                       MN_DUAL_SYS_E dual_sys
                                       );

/*****************************************************************************/
// 	Description : get csp2 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: the s_cphs_info[dual_sys].is_csp2_valid should be TRUE, else return PNULL
/*****************************************************************************/
PUBLIC MN_CSP_LINE2_SERVICES_GROUP_T* MMISET_GetCsp2Service(
                                       MN_DUAL_SYS_E dual_sys
                                       );
/*****************************************************************************/
// 	Description : get csp2 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: need receive APP_MN_LINE_LOCKSTATUS_UPDATE_IND msg
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_LockLine(
                                       MN_DUAL_SYS_E dual_sys,
                                       BOOLEAN is_lock
                                       );

/*****************************************************************************/
// 	Description : get csp2 service
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MMISET_LINE_LOCK_STATUS_E MMISET_GetLineLockStatus(
                                       MN_DUAL_SYS_E dual_sys
                                       );

/*****************************************************************************/
// 	Description : is als valid
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsAlsValid(
                                       MN_DUAL_SYS_E dual_sys
                                       ) ;

/*****************************************************************************/
// 	Description : ??????????
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetSelectLine(
                                 MN_DUAL_SYS_E      dual_sys,                            
                                 MMISET_LINE_TYPE_E  line_type
                                 );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleLineLockStatusUpdateInd(
                                       DPARAM              param
                                       );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleLineSelectedUpdateInd(
                                       DPARAM              param
                                       );

#endif //MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : Set DualSIM standby mode
//	Global resource dependence : 
//   Author: yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDualSIMStandby(BOOLEAN * is_standby_ptr, MN_DUAL_SYS_E dual_sys);

#endif
