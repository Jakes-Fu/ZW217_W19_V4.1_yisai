/******************************************************************************
 ** File Name:      atc_phonebook.h                                           *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           17/10/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains the functions that handle the AT       *
 **                 command for phonebook                                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 17/10/2002     Xueli.lv         Create.                                   *
 ** 25/06/2003     Liuhan           Modify.                                   *
 ******************************************************************************/
#ifndef _ATC_PHONEBOOK_H
#define _ATC_PHONEBOOK_H

 /**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*****************************************************************************
**                    macro variables
******************************************************************************/
#define ATC_INVALID_INDEX             0

#define AT_CPBN_FIRST                 0 
#define AT_CPBN_LAST                  1
#define AT_CPBN_NEXT                  2
#define AT_CPBN_PREVIOUS              3
#define AT_CPBN_LATESTREAD            4
#define AT_CPBN_LATESTWRITTEN         5

#define AT_CSVM_DISABLE               0 
#define AT_CSVM_ENABLE                1

#define AT_STORAGE_SM                 0 
#define AT_STORAGE_FD                 1
#define AT_STORAGE_ME                 2
#define AT_STORAGE_LD                 3
#define AT_STORAGE_MC                 4
#define AT_STORAGE_RC                 5

#define AT_CPBS_MAXLENGTH             0X09

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function initialise the static variable in phonebook
//  Global resource dependence : 
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void ATC_InitPhonebookStaticVar(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//  Description : This function handles the AT+CPBS command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBS );

/*****************************************************************************/
//  Description : This function handles the AT+CPBR command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBR );

/*****************************************************************************/
//  Description : This function handles the AT+CPBF command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBF );

/*****************************************************************************/
//  Description : This function handles the AT+CPBW command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBW );
/*****************************************************************************/
//  Description : This function handles the AT+CPBP command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBP );

/*****************************************************************************/
//  Description : This function handles the AT+CPBN command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBN );


#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handles the APP_MN_ADN_UPDATE_CNF signal in 
//  deleting only one item.
//  Global resource dependence : 
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessDelItemCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_ADN_UPDATE_CNF_T    *sig_ptr
                );
#endif

/*****************************************************************************/
//  Description : This function handles the AT+CNUM command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCNUM );

#if 0
/*****************************************************************************/
//  Description : This function handles the AT+CSVM command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSVM );

/*****************************************************************************/
//  Description : This function handles the AT+WDCP command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSDCP );
#endif

 /**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
