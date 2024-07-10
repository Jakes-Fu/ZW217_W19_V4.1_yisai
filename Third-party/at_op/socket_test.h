/****************************************************************************
** File Name:      socket_test.h                                           *
** Author:         qin.he                                                 *
** Date:           2022/10/1                                              *
** Copyright:      2021 uinsoc, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2022/10/1   qin.heg          create
** 
****************************************************************************/

#ifndef _SOCKET_TEST_H_
#define _SOCKET_TEST_H_

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "socket_api.h"
#include "socket_types.h"
#include "in_message.h"

/*****************************************************************************/
//  Description :  main function
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
PUBLIC void socket_test_main(uint8 is_async, uint32 netid, uint32 interval);

/*****************************************************************************/
//  Description :  exit function
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
PUBLIC int socket_test_exit(void);

#ifdef   __cplusplus
    }
#endif

#endif 
