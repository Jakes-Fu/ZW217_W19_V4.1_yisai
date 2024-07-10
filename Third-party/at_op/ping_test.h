/****************************************************************************
** File Name:     ping_test.h                                              *                                           *
** Date:           2020/09/03                                              *
** Copyright:      2020 uinsoc, Incorporated. All Rights Reserved.         *
** Description:                                                            *
****************************************************************************/

#ifndef _PING_TEST_H_
#define _PING_TEST_H_

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

PUBLIC int32 AT_PingStart(
    char* ipevr,
    char* host_name,
    uint32 datalen,
    uint32          data_size,
    uint8          s_link_id,
    uint32        netid,
    uint32          times);

PUBLIC int32 AT_PingStop(void);
#ifdef   __cplusplus
    }
#endif

#endif 
