/****************************************************************************
** File Name:      adultwatch_bt_pair.h                                     *
** Author:         chunjuan.liang                                           *
** Date:           04/23/2021                                               *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth display menu *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 04/23/2020    chunjuan.liang     Create                                  *
**                                                                          *
****************************************************************************/

#ifndef _ADULTWATCH_BT_PAIR_H_
#define _ADULTWATCH_BT_PAIR_H_

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "ual_bt.h"

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: Open the blue tooth pair win
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note:包括BLE和dialer配对
//
/*****************************************************************************/
PUBLIC void AdultBT_Pair_Enter(void);

/*****************************************************************************/
//  Discription: BTPair_FreshWin
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note:包括BLE和dialer配对
//
/*****************************************************************************/
PUBLIC void BTPair_FreshWin(void);

/****************************************************************************/
//  Description : WATCHBT_HFUConnect
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:hfu connect，当前hfu只能连接一个
/****************************************************************************/
PUBLIC BOOLEAN WATCHBT_ConnectBoundDialerDev(void);

#endif

