/****************************************************************************
** File Name:      speed_test.h                                           *
** Author:         qin.he                                                 *
** Date:           2020/7/9                                              *
** Copyright:      2020 uinsoc, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2020/7/9     qin.he          create
** 
****************************************************************************/

#ifndef _SPEED_TEST_H_
#define _SPEED_TEST_H_

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
PUBLIC void speed_test_main(uint8 is_async, uint32 netid, uint32 timeout);

/*****************************************************************************/
//  Description :  exit function
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
PUBLIC int speed_test_exit(void);

#ifdef   __cplusplus
    }
#endif

#endif 
