/******************************************************************************
 ** File Name:      atc_ppp.h                                                 *
 ** Author:         Shijun Cui                                                *
 ** Date:           02/08/2006                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains types and MICROS that are of use to all*
 **                 other ATC modules.                                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 02/08/2006     Shijun Cui       Created.                                  *
 ******************************************************************************/
#ifndef _ATC_PPP_H_
#define _ATC_PPP_H_
/*******************************************************************************
 **                        Common Header Files                                 *
 ******************************************************************************/
/*the type of modem status added for ATC_GetModemStatus*/
typedef enum
{
    ATC_MODEM_NOT_ONDIAL,
    ATC_MODEM_ONDIAL
}ATC_MODEM_STATUS_E;

/*******************************************************************************
 **                        Functon  declaration                                *
 ******************************************************************************/
/******************************************************************************/
// Description : clear atc state after ppp modem disconnect
// Global resource dependence : None
// Author : hyman.wu
// Note : None
/******************************************************************************/
#ifdef _MUX_ENABLE_
void ATC_DisconnectModem( uint8 cid );
void ATC_DestroyPPP( uint8 cid );
#else
void ATC_DisconnectModem(
                        void
                        );
#endif

/******************************************************************************/
// Description : This function called by SIO and MMI to know if the state is on dialing or not
// Global resource dependence : 
// Author : king.li
// Note : 
/******************************************************************************/
ATC_MODEM_STATUS_E ATC_GetModemStatus(void);

#endif // _ATC_PPP_H_