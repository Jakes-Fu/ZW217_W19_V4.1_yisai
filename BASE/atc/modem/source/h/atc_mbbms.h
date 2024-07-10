/******************************************************************************
 ** File Name:      atc_mbbms.h                                               *
 ** Author:         Tracy Zhang                                               *
 ** Date:           2011-03-28                                                *
 ** Copyright:      Copyright 2001-2011 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains functions definitions about MBBMS ATC. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 2011-03-28     Tracy Zhang      Create.                                   *
 ******************************************************************************/
#ifndef _ATC_MBBMS_H
#define _ATC_MBBMS_H

/*****************************************************************************/
//  Description : This function handles the AT+ERGA command
//  Global resource dependence :
//  Author:    minqian.qian
//  Note:   This command is used to implement "RUN GSM ALGORITHM" in SIM
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessERGA);

/*****************************************************************************/
//  Description : This function handles the AT+ERTCA command
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:       This command is used to implement 3G Context authentication
//              mechanism in USIM
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessERTCA);

/*****************************************************************************/
//  Description : This function handles the AT^MBCELLID command
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_MBCELLID);

/*****************************************************************************/
//  Description : This function handles the AT^MBAU command
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_MBAU);

/*****************************************************************************/
//  Description : This function porcess APP_MN_SIM_MBBMS_ALGO_CNF 
//                at AT+ERGA or AT+ERTCA
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSIMmbbmsAlgoCnf(
    ATC_CONFIG_T        *atc_config_ptr,
    APP_MN_SIM_MBBMS_ALGO_CNF_T  *sig_ptr
);

/*****************************************************************************/
//  Description : This function porcess APP_MN_SIM_MBBMS_ALGO_CNF at AT^MBAU
//  Global resource dependence :
//  Author:     chong.liu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSIMmbbmsAlgoCnf_Mbau(
                ATC_CONFIG_T        *atc_config_ptr,  // point to the ATC_CONFIG_T struct.
                APP_MN_SIM_MBBMS_ALGO_CNF_T  *sig_ptr
                );

#endif
