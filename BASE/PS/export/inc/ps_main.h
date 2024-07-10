/******************************************************************************
 ** File Name:      ps_main.h                                                 *
 ** Author:         Xueliang.Wang                                             *
 ** Date:           03/11/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *         
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 12/16/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/
#ifndef _PS_MAIN_H
#define _PS_MAIN_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/* mn atc and sim task priority should be same.
   the task priority should higher than other MMI or REF app tasks
 */
#define MN_SIM_ATC_PRIORITY    22   //SCI_PRIORITY_BELOW_NORMAL-2

/*****************************************************************************/
//  Description : Get sys_id by Gmm Task id
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
uint32  SCI_GetSysIdByGmmTask(
                              BLOCK_ID  gmm_id
                              );

/*****************************************************************************/
//  Description : Get sys_id by Mm Task id
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
uint32  SCI_GetSysIdByMmTask(
                              BLOCK_ID  mm_id
                              );

/*****************************************************************************/
//  Description : Get sys_id by RR Task id
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
uint32  SCI_GetSysIdByRrTask(
                              BLOCK_ID  rr_id
                              );


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // End ps_main.h