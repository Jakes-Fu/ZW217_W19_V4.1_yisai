/******************************************************************************
 ** File Name:     USB_Timer.c                                                *
 ** Author:        JiaYong.Yang                                               *
 ** DATE:          08/02/2012                                                 *
 ** Copyright:     2012 Spreadtrum, Incoporated. All Rights Reserved.          *
 ** Description:   USB Timer.c used to minitor the usb transfer of IN endpoint*
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "os_api.h"
#include "sci_api.h"
#include "udc_api.h"
#include "usb_timer.h"

#if defined(PLATFORM_UWS6121E)
#include "chip_plf_export.h"
#endif

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

LOCAL TIMER_FUN usb_timeout = NULL;
LOCAL SCI_TIMER_PTR s_usb_time_ptr[USB_EPx_NUMBER]={SCI_NULL};
LOCAL uint32 endpoint_timer[USB_EPx_NUMBER]={USB_TRANSFER_TIMER,USB_TRANSFER_TIMER,USB_TRANSFER_TIMER,USB_TRANSFER_TIMER2,USB_TRANSFER_TIMER};//lint !e808 !e785

/*****************************************************************************/
//  Description:    create timer resource for the specified endpiont
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when the initialize the IN_EP.
/*****************************************************************************/
PUBLIC void USB_CreateTimer (uint32 ep_id,TIMER_FUN fun)
{
	  char timer_name[8]={'U','S','B','T','M','R','0',0};
	  
	  if(ep_id >= sizeof(s_usb_time_ptr)/sizeof(s_usb_time_ptr[0]))
	  {
	  	return;
	  }
	  if(fun==NULL)
	  {
	  	return;
	  }
	  
	  timer_name[6] +=(char) ep_id;
    if(s_usb_time_ptr[ep_id] == SCI_NULL)
    {
    	  usb_timeout = fun;
        s_usb_time_ptr[ep_id] =  SCI_CreateTimer(timer_name,
                                              usb_timeout,
	                                            ep_id, 
	                                            endpoint_timer[ep_id], 
                                              SCI_NO_ACTIVATE);        
		}	
}
/*****************************************************************************/
//  Description:    enable the timer of specified endpoint
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when specified IN_EP start transfer.
/*****************************************************************************/
PUBLIC void USB_ActiveTimer (uint32 ep_id)
{
	  if(ep_id >= sizeof(s_usb_time_ptr)/sizeof(s_usb_time_ptr[0]))
	  {
	  	return;
	  }
	  if(s_usb_time_ptr[ep_id]==SCI_NULL)	
    {
    	return;
    }

   	SCI_ChangeTimer(s_usb_time_ptr[ep_id], usb_timeout, endpoint_timer[ep_id]);
    SCI_ActiveTimer(s_usb_time_ptr[ep_id]);
}
/*****************************************************************************/
//  Description:    close the timer of specified endpoint
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when specified IN_EP transfer ends.
/*****************************************************************************/
PUBLIC void USB_StopTimer (uint32 ep_id)
{
	  if(ep_id >= sizeof(s_usb_time_ptr)/sizeof(s_usb_time_ptr[0]))
	  {
	  	return;
	  }
	  if(s_usb_time_ptr[ep_id]==SCI_NULL)	
    {
    	return;
    }
	  if(SCI_IsTimerActive(s_usb_time_ptr[ep_id]))
	  {
	  	SCI_DeactiveTimer(s_usb_time_ptr[ep_id]);
	  }
}
/*****************************************************************************/
//  Description:    release timer resource of the specified IN_EP
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
/*****************************************************************************/
LOCAL void USB_DeleteTimer (uint32 ep_id)
{
	  if(ep_id >= sizeof(s_usb_time_ptr)/sizeof(s_usb_time_ptr[0]))
	  {
	  	return;
	  }
	  if(s_usb_time_ptr[ep_id]==SCI_NULL)	
    {
    	return;
    }
    if(SCI_IsTimerActive(s_usb_time_ptr[ep_id]))
	  {
	  	SCI_DeactiveTimer(s_usb_time_ptr[ep_id]);
	  }
	  SCI_DeleteTimer(s_usb_time_ptr[ep_id]);
	  s_usb_time_ptr[ep_id] = SCI_NULL;
}
/*****************************************************************************/
//  Description:    release timer resource of the specified IN_EP
//                  
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
//                  parameter is the endpoint ID.
//                  this function is called when USB is plug out.
/*****************************************************************************/
PUBLIC void USB_FreeAllTimers ()
{
	  uint32 ep_id;
	  for(ep_id=0; ep_id< sizeof(s_usb_time_ptr)/sizeof(s_usb_time_ptr[0]);ep_id++)
	  {
	  	USB_DeleteTimer(ep_id);
	  }
	  usb_timeout = NULL;
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
