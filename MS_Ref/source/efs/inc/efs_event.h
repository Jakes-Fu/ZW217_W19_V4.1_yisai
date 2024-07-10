/******************************************************************************
 **
 **  Copyright (C) 2001 SPREADTRUM COMMUNICATION Ltd.
 **  All rights reserved.
 **
 **  This document contains proprietary information blonging to 
 **  SPREADTRUM. Passing on and copying this document, use and 
 **  communication of its contents is not permitted without prior
 **  writed authorization.
 ******************************************************************************/


/******************************************************************************
 ** File Name:      efs_event.h                                               *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 **                                                                           *
 ** Description:    This file include the interface that communicate between  *
 **                 EFS thread and high-layer API.                            *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef EFS_EVENT_H
#define EFS_EVENT_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "efs.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

typedef struct
{
    BLOCK_ID        send;
    EFS_RESULT      ret;
    EFS_ERROR_E     error;
}EFS_RETURN_T;

// 
// Init the param_setting and convert to EFS_PARAM structure;
//
EFS_PARAM  default_param_setting( EFS_RETURN_T * ret_ptr );



//
// Complete routine, it will be called after optr completed.
// and set the event flag of the given thread.
//
void default_complete_routine(
    EFS_ERROR_E 	error_code,
    EFS_RESULT 		result, 
    EFS_PARAM 		param);


// 
// Wait a EFS event. 
// This wouldn't return until the event flag be set or wait abort.
//
BOOLEAN EFS_WaitComplete(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // End efs_event.h 