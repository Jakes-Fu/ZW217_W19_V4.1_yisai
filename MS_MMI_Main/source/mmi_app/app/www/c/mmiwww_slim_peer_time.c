/******************************************************************************
** File Name:      mmiwww_slim_peer_time.c                                    *
** Author:                                                                    *
** Date:           06/15/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the slim time peer for WWW   *
**                 Browser                                                    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIWWW_SLIM_PEER_TIME_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "tb_dal.h"
#include "mmiwww_slim_peer.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Constant Variables                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Gets current date and time
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimGetCurrentDatePeer(slim_int *out_year, slim_int *out_mon, 
                                       slim_int *out_mday, slim_int *out_hour,
                                       slim_int *out_min,  slim_int *out_sec,
                                       slim_int *out_msec)
{
    SCI_DATE_T    cur_date = {0};
    SCI_TIME_T    cur_time = {0};
    uint32        tick_time = 0;

    PEER_DP(("slimGetCurrentDatePeer enter"));
    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    *out_year = cur_date.year;
    *out_mon  = cur_date.mon - 1; /* 0 - 11*/
    *out_mday = cur_date.mday;
    *out_hour = cur_time.hour;
    *out_min  = cur_time.min;
    *out_sec  = cur_time.sec;
    tick_time = SCI_GetTickCount();
    *out_msec = tick_time%1000;
    return SLIM_E_OK;
}

/*****************************************************************************/
// Description : Gets offset time
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int slimGetTZOffsetPeer(void)
{
//    return -(LOCALTIMEOFFSET(NULL));//平台没有，需要ref提供
    PEER_DP(("slimGetTZOffsetPeer not support, return 0"));
    return 0;
}
#endif //#ifdef BROWSER_SUPPORT_NETFRONT