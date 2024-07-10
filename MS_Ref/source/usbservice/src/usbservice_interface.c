
/******************************************************************************
 ** File Name:     *
 ** Author:        *
 ** DATE:          *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:   *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ******************************************************************************/
#ifndef _USBSERVICE_INTERFACE_C_
#define _USBSERVICE_INTERFACE_C_
    
#include "usbservice_api.h"
#include "upm_config.h"
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    


 
/**---------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/
USB_SERVICE_E s_cur_srv = 0;
LOCAL USB_SERVICE_E   s_usb_function_list=USB_SERVICE_NULL;

/*****************************************************************************/
//	Function Name:   USB_GetServiceCount        
//  Description:     Call this function to get service count.
//	Return Value: 
//  Parameters:
/*****************************************************************************/
PUBLIC uint32 USB_GetServiceCount()
{
	uint32 i;
	uint32 cnt = 0;

	for(i=0;i<USB_SERVICE_MAX;i++)
	{
		if(g_USBService_table[i] == NULL)
		{
			
		}
		else
		{
			cnt++;
		}
	}

	return cnt;
}

/*****************************************************************************/
//	Function Name:    USB_GetNextService       
//  Description:      Call this function to get next service
//	Return Value: 
//  Parameters:
/*****************************************************************************/
PUBLIC USB_SERVICE_E USB_GetNextService(void)
{
	while(s_usb_function_list<USB_SERVICE_MAX)
	{
		if(g_USBService_table[s_usb_function_list] == NULL)
		{
			s_usb_function_list++;
			continue;
		}
		else
		{
			s_usb_function_list++;
            
			return (s_usb_function_list-1);
		}
	}
	s_usb_function_list = 0;

	return USB_SERVICE_MAX;

}

/*****************************************************************************/
//	Function Name:    USB_GetCurService       
//  Description:      Call this function to get the service is using
//	Return Value: 
//  Parameters:
/*****************************************************************************/
PUBLIC USB_SERVICE_E USB_GetCurService(void)
{  
    #if 0
	if(s_cur_srv != 0)
	{
		return (s_cur_srv-1);
	}
	else
	{
		return USB_SERVICE_NULL;
	}
	#endif
	
	return s_cur_srv;
	
}

/*****************************************************************************/
//  Description:    send massage to vbus thread.
//  Author:         Lianlian.Huang
/*****************************************************************************/
PUBLIC void UsbService_SendMsgToVbusThread(USB_PLUG_E sig,uint32 sig_param)
{
	xSignalHeader signal;
	signal = SCI_ALLOC (sizeof (*signal));
	signal->SignalCode = sig;
	signal->SignalSize = sizeof (*signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_SendSignal (signal, usb_isr_id);
}

/*****************************************************************************/
//	Function Name:   USB_StartService        
//  Description:     Call this function to start selected service
//	Return Value: 
//  Parameters:
/*****************************************************************************/
PUBLIC BOOLEAN USB_StartService(USB_SERVICE_E servicetype)
{   


	xSignalHeader signal;
    s_cur_srv=servicetype;
/*	
	if (SCI_IsTimerActive (s_UPM_timer_ptr))
    {
         SCI_DeactiveTimer (s_UPM_timer_ptr);
    }

    SCI_ChangeTimer (s_UPM_timer_ptr,\
                         USB_SetCharge,\
                         UPM_DETECT_TIMER);
    SCI_ActiveTimer (s_UPM_timer_ptr);
*/

	if(USB_SERVICE_NULL == servicetype)
	{
	     return SCI_TRUE;
	}

	signal = SCI_ALLOC_APP(sizeof(*signal));
	signal->SignalCode = (servicetype | START_SERVICE);
	signal->SignalSize = sizeof(*signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signal, usb_main_id);
    
	return SCI_TRUE;
}

//just for old UPM interface used by atc_eng.c
PUBLIC BOOLEAN UPM_SetService (UPM_SERVICETYPE_E type)  
{     
      return USB_StartService((USB_SERVICE_E)type); 
}
/*****************************************************************************/
//	Function Name:    USB_StopService       
//  Description:      Call this function to stop selected service
//	Return Value: 
//  Parameters:
/*****************************************************************************/
PUBLIC BOOLEAN USB_StopService(USB_SERVICE_E servicetype)
{
	xSignalHeader signal;

	signal = SCI_ALLOC_APP(sizeof(*signal));
	signal->SignalCode = (servicetype | STOP_SERVICE);
	signal->SignalSize = sizeof(*signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signal, usb_main_id);

	return SCI_TRUE;
}

#endif
