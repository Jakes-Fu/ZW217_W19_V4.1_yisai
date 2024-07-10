
/******************************************************************************
 ** File Name:    UPM_api.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _UPM_API_H_
#define _UPM_API_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "os_api.h"
#include "upm_config.h"
#include "sci_service.h"
#include "usbservice_api.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

// Definition for Keypad message
typedef enum
{
    // UPM message.
    UPM_PLUGIN  = USB_PLUGIN ,
    UPM_PLUGOUT = USB_PLUGOUT,
    UPM_PLUGOUT_START = USB_PLUG_OUT_START,
    UPM_READY   = USB_READY,
    UPM_ERROR   = USB_ERROR,
    UPM_MSG_MAX = USB_MSG_MAX
              }
              UPM_MSG_SERVICE_E;

/*****************************************************************************/
//  Description:    Call this function to set current sevice type ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPM_SetService (UPM_SERVICETYPE_E type);

#if 0 //used only in old version, comment

typedef enum
{
    UPM_CMD_PLUG_IN = UPM_CMD_START,
    UPM_CMD_PLUG_OUT,
    UPM_CMD_CONNECT,
    UPM_CMD_DISCONNECT,
    UPM_CMD_VBUS_PLUG_IN,
    UPM_CMD_MAX
} UPM_COMMAND_E;

typedef struct  upm_isr_tag
{
    uint32 (*main) (uint32 cmd,uint32 param,void *ptr);
    void (*init) (void);
    void (*exit) (void);
    USB_ISR_PTR_T irq;
    uint32        state;
} UPM_FUNCTION_PTR_T;

typedef enum upm_freq_lvl_tag
{
    USB_FREQ_SET_LVL_LOW,
    USB_FREQ_SET_LVL_MID,
    USB_FREQ_SET_LVL_HIGH,
    USB_FREQ_SET_LVL_MAX
} UPM_FREQ_LVL_E;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Call this function to set current USB connect state ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void UPM_SetConnectState (BOOLEAN state);
/*****************************************************************************/
//  Description:    Call this function to get current USB connect state ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPM_GetConnectState (void);
/*****************************************************************************/
//  Description:    Call this function to get current service type;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC UPM_SERVICETYPE_E UPM_GetService (void);
/*****************************************************************************/
//  Description:    Call this function to set current sevice type ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPM_SetService (UPM_SERVICETYPE_E type);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void UPM_Init (void);
/*****************************************************************************/
//  Description:    Call this function to get current upm state .
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPM_GetUPMState (void);
/*****************************************************************************/
//  Description:    Call this function to get current service state .
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPM_GetServiceState (void);
/*****************************************************************************/
//  Description:    Call this function to register protocol's handler to UPM;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
BOOLEAN UPM_RegService (UPM_SERVICETYPE_E type,\
                        const UPM_FUNCTION_PTR_T *handler_ptr);
/*****************************************************************************/
//  Description:    Call this function for sending commands to manager or
//                  protoclos;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPM_SendCommand (UPM_SERVICETYPE_E receiver,
                                uint32 command,
                                uint32 param,
                                void *ptr);
/******************************************************************************/
//  Description:    U_DiskInit
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           Call this function for finishing U_disk init ;
/******************************************************************************/
PUBLIC void UPM_CHGPowerSet (BOOLEAN param);

/*****************************************************************************/
//  Description:    Call this function to register protocol's handler to UPM;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPM_GetUSBState (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void UPM_CharageIntCallback (BOOLEAN state);

/******************************************************************************/
//  Description:    UPM_SetFreq
//  Global resource dependence:
//  Author:         Liangwen
//  Note:           Call this function to set upm frequency ;
/******************************************************************************/
PUBLIC void UPM_SetFreq (uint32 level);

/*****************************************************************************/
//  Description:    Call this function to set usb state
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void UPM_SetUSBState (BOOLEAN state);
#endif

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

