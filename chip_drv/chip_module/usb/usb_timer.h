/******************************************************************************
 ** File Name:     USB_Timer.h                                                *
 ** Author:        JiaYong.Yang                                               *
 ** DATE:          08/02/2012                                                 *
 ** Copyright:     2012 Spreadtrum, Incoporated. All Rights Reserved.          *
 ** Description:   USB Timer.c used to minitor the usb transfer of IN endpoint*
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef _USB_TIMER_H_
#define _USB_TIMER_H_

#include "sci_types.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define USB_TRANSFER_TIMER    3000
#define USB_TRANSFER_TIMER2   180

/*****************************************************************************/
//  Description:    create timer resource for the specified endpiont
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when the initialize the IN_EP.
/*****************************************************************************/
PUBLIC void USB_CreateTimer (uint32 ep_id,TIMER_FUN fun);

/*****************************************************************************/
//  Description:    enable the timer of specified endpoint
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when specified IN_EP start transfer.
/*****************************************************************************/
PUBLIC void USB_ActiveTimer (uint32 ep_id);
/*****************************************************************************/
//  Description:    close the timer of specified endpoint
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when specified IN_EP transfer ends.
/*****************************************************************************/
PUBLIC void USB_StopTimer (uint32 ep_id);


/*****************************************************************************/
//  Description:    release timer resource of the specified IN_EP
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when USB is plug out.
/*****************************************************************************/
PUBLIC void USB_FreeAllTimers (void);

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
#endif //_USB_TIMER_H_ 
