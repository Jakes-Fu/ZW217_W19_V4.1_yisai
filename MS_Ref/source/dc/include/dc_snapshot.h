/******************************************************************************
 ** File Name:      dc_snapshot.h                                                       *
 ** Author:         Jing.Li                                                             *
 ** DATE:           2008/12/01                                                          *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.                   *
 ** Description:                                                                        *
 *******************************************************************************

 ******************************************************************************
 **                        Edit History                                                *
 ** ----------------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                        *
 ** 2008/12/01     Jing.Li          Create.                                            *
 ******************************************************************************/

#ifndef DC_SNAPSHOT_H
#define DC_SNAPSHOT_H
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "dal_dcamera.h"
#include "dc_app_6x00.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define         	                          *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 typedef struct snapshot_ops_handler{
    int32 (*check_status)(void);
    int32 (*check_param)(DCAMERA_SNAPSHOT_PARAM_T * param_ptr);
    int32 (*set_param)(DCAMERA_SNAPSHOT_PARAM_T *param_ptr);
    int32 (*start)(void);
    int32 (*set_burst_param)(DCAMERA_BURST_SNAPSHOT_PARAM_T  *param_ptr,
									DCAMERA_SNAPSHOT_RETURN_PARAM_T  *return_param_ptr);
    int32 (*start_burst)(void);
    int32 (*capture_review)(void);    
    int32 (*clean_combineparam)(void);    
        
}SNAPSHOT_OPS_HANDLER_T;

/******************************************************************************
// Description: 	get snapshot operation handlers
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          snapshot operation handler struct pointer
// Note:       		
******************************************************************************/
PUBLIC SNAPSHOT_OPS_HANDLER_T* DC_Snapshot_GetOpsHandler(void);

#ifdef __cplusplus
    }
#endif


#endif  /*#ifndef DC_SNAPSHOT_H*/
