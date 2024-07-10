/*******************************************************************************
 ** FileName:      dcamera_misc.c                                              *
 ** Author:        Eddy.wei                                                    *
 ** Date:          2012/01/18                                                  *
 ** CopyRight:     2012, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2012/01/18    Eddy.wei                  Created                            *
 *******************************************************************************/

#ifndef DCAMERA_MISC_H
#define DCAMERA_MISC_H
/**----------------------------------------------------------------------------*
 **                         Dependencies                                       *
 **----------------------------------------------------------------------------*/

#include "dal_dcamera.h"
#include "img_ref_types.h"

/**----------------------------------------------------------------------------*
 **                         Compiler Flag                                      *
 **----------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**----------------------------------------------------------------------------*
 **                         Macro define                                       *
 **----------------------------------------------------------------------------*/
//event
#define DCAMERA_PARAM_CTRL_EVENT            BIT_0
#define DCAMERA_ATV_STOP_EVENT              BIT_1
//timer
#define DCAMERA_WAIT_TIME                   5000 //5s

/**----------------------------------------------------------------------------*
 **                         Data Structures                                    *
 **----------------------------------------------------------------------------*/
typedef enum
{
    DCAMERA_TASK_CMD_NONE = 0x00,
    DCAMERA_TASK_CMD_SNAPSHOT,
    DCAMERA_TASK_CMD_BURST_SNAPSHOT,
    DCAMERA_TASK_CMD_UPCC,
    DCAMERA_TASK_CMD_SCALE,
    DCAMERA_TASK_CMD_SNAPSHOT_EX,   
    DCAMERA_TASK_CMD_ATV,    
    DCAMERA_TASK_CMD_MAX

}DCAMERA_TASK_CMD_E;

typedef enum
{
    DCAMERA_ATV_CMD_SCAN_CHN_START=0x01,
    DCAMERA_ATV_CMD_SCAN_CHN,
    DCAMERA_ATV_CMD_SCAN_CHN_STOP,
    DCAMERA_ATV_CMD_RSSI,
    DCAMERA_ATV_CMD_DECT_REGION,
    DCAMERA_ATV_CMD_MAX,
}DCAMERA_ATV_CMD_E;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/******************************************************************************/
// Description: create dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_CreateTimer(void);
/******************************************************************************/
// Description: delete dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_DeleteTimer(void);
/******************************************************************************/
// Description: enable dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      timer_expire: timer timeout expire
//                      *fun)(): function for timer
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_TimerActive(uint32 timer_expire, uint32(*fun)());
/******************************************************************************/
// Description: disable dcamera timer
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_TimerDactive(void);
/******************************************************************************/
// Description: dc set event
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      uint32 set_flags : event put flags
// output parameter:
//                      None
// return:
//          None     
// Note:
/******************************************************************************/
PUBLIC void DCAMERA_SetEvent(uint32 set_flags);
/******************************************************************************/
// Description: Isp get event
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      uint32 requested_flags : event request flags
//                      uint32 wait_option : event wait option
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/******************************************************************************/
PUBLIC uint32 DCAMERA_GetEvent(uint32 requested_flags, uint32 wait_option);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /*#ifndef DC_MISC_H*/
