/******************************************************************************
 ** File Name:    atc_orange_prod.h                                           *
 ** Author:         longting.zhao                                                *
 ** DATE:           22/10/2012                                                *
 ** Copyright:     Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                    All Rights Reserved.                                      *
 **                    This software is supplied under the terms of a license    *
 **                    agreement or non-disclosure agreement with Spreadtrum.    *
 **                    Passing on and copying of this document,and communication *
 **                    of its contents is not permitted without prior written    *
 **                    authorization.                                            *
 ** Description:   This file includes declaration of all AT commands            *
**                      demmanded by orange                                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/10/2012  longting.zhao         Create.                                   *
 ******************************************************************************/
#ifndef _ATC_ORANGE_PROD_H_
#define _ATC_ORANGE_PROD_H_

#include "atc.h"
//#include "stdio.h"

// for AT+CBC command
#define AT_CBC_NO_CHARGE                 0
#define AT_CBC_CHARGE                    1
#define AT_CBC_NO                        2

/*****************************************************************************/
//  Description : This function handle the AT+GSN command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGSN );

/*****************************************************************************/
//  Description : This function handle the AT+GMR command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGMR );

/*****************************************************************************/
//  Description : This function handle the AT+CBC command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCBC );

/*****************************************************************************/
//  Description : This function handle the AT+GMI command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGMI );

/*****************************************************************************/
//  Description : This function handle the AT$NWRAT command 
//               select a new network type
//  Global resource dependence : none
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessDOLLAR_NWRAT);

/*****************************************************************************/
//  Description : This function handles the AT+CBST command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCBST);

/*****************************************************************************/
//  Description : This function handles the AT+GCAP command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGCAP);

#endif/*_ATC_ORANGE_PROD_H_*/
