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
 ** 2012/07/12      liming.deng      Create.                              *
*************************************************************************/
#ifndef _MMISMS_SECURITYWIN_H_
#define _MMISMS_SECURITYWIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmisms_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN isValidate;		//use callback or not
    SMSAPI_VALIDATESECURITY_CALLBACK user_callback;
}MMISMS_VAL_SER_DATA_T;

/*****************************************************************************/
//  Description : to enter pass word win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/

/*****************************************************************************/
//  Description : 打开安全信箱验证窗口
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMS_OpenValidateSecruityBox(SMSAPI_VALIDATESECURITY_CALLBACK user_callback);

//PUBLIC void MMISMS_EnterPassWordWin(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
