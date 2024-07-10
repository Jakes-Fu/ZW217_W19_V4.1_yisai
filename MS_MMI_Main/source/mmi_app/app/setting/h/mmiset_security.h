/*****************************************************************************
** File Name:      mmiset_security.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li          Create
******************************************************************************/

#ifndef _MMISET_SECURITY_H_
#define _MMISET_SECURITY_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISET_POWER_ON_PWD_MIN_LEN         4//@fen.xie MS00175496
#define MMISET_POWER_ON_PWD_MAX_LEN         8

#define MMISET_PRIVACY_PROTECT_PWD_MIN_LEN	4//maryxiao
#define MMISET_PRIVACY_PROTECT_PWD_MAX_LEN	8

// 用于判断开机密码输入的位数是否正确。
#define PHONE_CODE_LEN_IS_IN_RANGE(_LEN)            \
    ((MMISET_POWER_ON_PWD_MIN_LEN <= (_LEN))    \
    && (MMISET_POWER_ON_PWD_MAX_LEN >= (_LEN))) \

#define MMISET_SS_PWD_LEN                   4   //the max length of the password
#define MMISET_SS_CODE_LEN                  6

//用于判断隐私保护的密码输入的位数是否正确
#define PRIVACY_PROTECT_PWD_LEN_IS_IN_RANGE(_LEN)            \
    ((MMISET_PRIVACY_PROTECT_PWD_MIN_LEN <= (_LEN))    \
    && (MMISET_PRIVACY_PROTECT_PWD_MAX_LEN >= (_LEN))) \
    
#define     MMISET_PRIVACY_PROTECT_INIT_PWD			"1234"
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
//隐私保护的信息，包括对应项是否设置了隐私保护，以及选择保护的项目数
typedef struct  
{
    BOOLEAN		protected_item[MMISET_PROTECT_MAX];	//指定项是否开启隐私保护
    uint8		protected_item_num;							//现在隐私保护的项目数
}MMISET_PRIVACY_PROTECT_T;

//the information about power on password
typedef struct
{
    BOOLEAN     is_enable;                          //密码是否开启
    uint8       pwd[MMISET_POWER_ON_PWD_MAX_LEN];   //开机密码
    uint8       pwd_len;                            //密码长度
}MMISET_POWER_ON_PWD_T;

//the information about privacy protect password
typedef struct  
{
    uint8       pwd[MMISET_PRIVACY_PROTECT_PWD_MAX_LEN];   //隐私保护密码
    uint8       pwd_len;									//密码长度
}MMISET_PRIVACY_PROTECT_PWD_T;

//The info of security set item
typedef enum
{
    MMISET_SECURITY_PIN_CODE = 0,
    MMISET_SECURITY_PIN2_CODE,
    MMISET_SECURITY_PRIVACY,
#ifndef LCD_SLIDE_SUPPORT      
#ifdef DPHONE_SUPPORT
#else
    MMISET_SECURITY_KEYLOCK,
#endif 
    MMISET_SECURITY_RED_KEYLOCK,
#endif
    MMISET_SECURITY_POWER_ON_CODE,
#if defined(MMI_MSD_SUPPORT)
    MMISET_SECURITY_MSD,
#endif
#ifdef FDN_SUPPORT
    MMISET_SECURITY_FDN,//@zhaohui add for fdn
#endif
    MMISET_SECURITY_UPLMN, //@MS00216980 cheney
#if defined(MMI_BLACKLIST_SUPPORT)
    MMISET_BLACKLIST, 	 
#endif
    MMISET_SECURITY_MAX_NUM
}MMISET_SECURITY_ITEM_E;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set the mode of TV out
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetTVOutMmode(
//								MMI_CTRL_ID_T	    ctrl_id
// 								);

/*****************************************************************************/
// 	Description : set privacy protect information
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetPrivacyProtectInfo(
                                         MMI_CTRL_ID_T	    ctrl_id
                                         );

/*****************************************************************************/
// 	Description : if privacy protect password correct
//	Global resource dependence : 
//  Author:kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCorrectPrivacyProtectPwd(
                                                 const wchar *pwd_ptr,
                                                 uint16       pwd_len
                                                 );

/*****************************************************************************/
// 	Description : add privacy protect list item 
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendPrivacyProtectListItem(
                                                MMI_CTRL_ID_T	    ctrl_id
                                                );

/*****************************************************************************/
// 	Description : 设置开机密码
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetPowerOnPwd(
                                 const wchar *pwd_ptr,
                                 uint16     pwd_len
                                 );

/*****************************************************************************/
// 	Description : set privacy protect password
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetPrivacyProtectPwd(
                                        const wchar *pwd_ptr,
                                        uint16     pwd_len
                                        );

/*****************************************************************************/
// 	Description : 开机密码是否正确
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsCorrectPowerOnPwd(
                                          const wchar *pwd_ptr,
                                          uint16     pwd_len
                                          );

/*****************************************************************************/
// 	Description : reset the privacy protect mode 
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC  void MMISET_ResetPrivacyProtectInfo(void);

/*****************************************************************************/
// 	Description : reset the privacy protect password 
//	Global resource dependence : 
//  Author: fen.xie
//	Note:for MS00179453
/*****************************************************************************/
PUBLIC  void MMISET_ResetPrivacyProtectPwd(void);

/*****************************************************************************/
//  Description : init phone password
//  Global resource dependence : none
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC void InitPhonePwd(MMISET_POWER_ON_PWD_T *phone_code);

#endif
