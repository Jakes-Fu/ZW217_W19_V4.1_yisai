/*************************************************************************
 ** File Name:      srv_sample_export.h                                 *
 ** Author:                                       *
 ** Date:                                                    *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 **                                *
*************************************************************************/

#ifndef _SRV_SAMPLE_EXPORT_H_
#define _SRV_SAMPLE_EXPORT_H_

#include "mmi_slist.h"



typedef uint32 SAMPLE_HANDLE_T;

typedef struct
{
    BOOLEAN result;
}SAMPLEDATASRV_OPERATE_RESULT_T;

PUBLIC SAMPLE_HANDLE_T SAMPLEDATASRV_Read(
        uint32  read_id,
        SrvCallbackFunc read_callback,  
        SAMPLEDATASRV_OPERATE_RESULT_T *action_data,
        void *user_data
);





#endif //_SRV_SAMPLE_EXPORT_H_

