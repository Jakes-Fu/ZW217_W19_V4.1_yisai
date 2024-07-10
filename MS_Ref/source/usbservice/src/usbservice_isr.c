
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
#ifndef _USBSERVICE_ISR_C_
#define _USBSERVICE_ISR_C_

/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */
#include "usbservice_api.h"
#include "gpio_prod_api.h"
#include "dal_chr.h"
#include "chng_freq.h"
#include "deep_sleep.h"
#include "deepsleep_drvapi.h"
#include "tasks_id.h"
//#include "udc_api.h"
//#include "sx_udc.h"
#include "prod_param.h"
#include "chg_drvapi.h"
#include "priority_system.h"
#include "usb_drv.h"
#include "dal_power.h"

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
#define VBUS_STACK_SIZE					0x1000
#define VBUS_QUEUE_NUM					0x0010

/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/
BLOCK_ID usb_isr_id;
PUBLIC SCI_TIMER_PTR            s_UPM_timer_ptr = PNULL;
/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/
LOCAL SCI_SEMAPHORE_PTR s_USB_VBUS_semaphore;
LOCAL uint32 s_usb_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;

/*----------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/
#if 0
/******************************************************************************/
//  Description:    USB_SetCharge
//	Global resource dependence:
//  Author:
//	Note:           Call this function to set charge state
/******************************************************************************/
PUBLIC void USB_SetCharge(uint32 param)
{
    CHGMNG_ADAPTER_TYPE_E adp_type;

    SCI_DeactiveTimer (s_UPM_timer_ptr);

    if (FALSE == UDC_GetHostReq())
    {
        USB_Disconnect ();
        adp_type = CHGMNG_IdentifyAdpType();

        if (CHGMNG_IsChargeConnect())
        {
            CHGMNG_SetChargeAdapter (adp_type);
        }
        else
        {
            CHGMNG_SetChargeAdapter (CHGMNG_ADP_USB);
        }
        SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
    }
    else
    {
        if (USB_SERVICE_NULL==USB_GetCurService())
        {
            USB_Disconnect ();

            SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
            CHGMNG_SetChargeAdapter (CHGMNG_ADP_USB);
        }
        else
        {
            SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
			CHGMNG_SetChargeAdapter (CHGMNG_ADP_USB);
        }
    }
}
#endif
/******************************************************************************/
//  Description:    USB_CreateFreqHandle
//	Global resource dependence:
//  Author:
//	Note:           Call this function to create freq handle
/******************************************************************************/
PUBLIC void USB_CreateFreqHandle(void)
{
	if (CHNG_FREQ_REQ_HANDLER_NULL == s_usb_chng_freq_req_handler)
	{
		s_usb_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("usb");
		SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_usb_chng_freq_req_handler); /*assert verified*/
	}
}


/******************************************************************************/
//  Description:    USB_SetFreq
//	Global resource dependence:
//  Author:
//	Note:           Call this function to set freq
/******************************************************************************/
PUBLIC void USB_SetFreq(uint32 level)
{
	SCI_ASSERT(level < USB_FREQ_SET_LVL_MAX); /*assert verified*/
	if (CHNG_FREQ_REQ_HANDLER_NULL == s_usb_chng_freq_req_handler)
	{
		s_usb_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("usb");
		SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_usb_chng_freq_req_handler); /*assert verified*/
	}

	CHNG_FREQ_SetArmClk(s_usb_chng_freq_req_handler,FREQ_INDEX_USB_LOW+level);
}

/******************************************************************************/
//  Description:    USB_RestoreFreq
//	Global resource dependence:
//  Author:
//	Note:           Call this function to restore freq
/******************************************************************************/
LOCAL void USB_RestoreFreq(void)
{
	if(CHNG_FREQ_REQ_HANDLER_NULL != s_usb_chng_freq_req_handler)
	{
		CHNG_FREQ_RestoreARMClk(s_usb_chng_freq_req_handler);
	}
}

/*****************************************************************************/
//	Function Name : USB_VbusThread
//  Description   : This function provide usb vbus interrupt thread
//	Return Value  : none
//  Parameters    : none
/*****************************************************************************/
LOCAL void USB_VbusThread(uint32 argc, void *argv)
{
	xSignalHeader signal;
	BLOCK_ID    tid = SCI_IdentifyThread();


#if 0
	s_UPM_timer_ptr = SCI_CreateTimer ("USB Detect Timer",\
                                       USB_SetCharge,\
                                       0,UPM_DETECT_TIMER ,
                                       SCI_NO_ACTIVATE);
#endif
	for(;;)
	{
		signal = (xSignalHeader)SCI_GetSignal(tid);
		if(signal->SignalCode == USB_HOST_PLUG_IN)							// host connected. interrupt from Dp/Dm
		{
#ifndef MODEM_PLATFORM
			while(TRUE != SCI_IsClientRegisted(USB_SERVICE,P_APP))		// wait for mmi register
			{
				SCI_Sleep(500);
			}
#endif
			if (TRUE == drvUsbIsConnected())								// make sure cable is connecting
			{
				//SCI_PutSemaphore(s_USB_VBUS_semaphore);
				// USB_SetFreq(USB_FREQ_SET_LVL_LOW);
				SCI_TraceLow("USB_VbusThread USB_HOST_PLUG_IN\n");
				/*notify to APP for user to choose usb work mode:UDISK/USBLOG*/
				#if 1//defined(USB_MASS_STORAGE_SUPPORT) //user版本
				SCI_SendEventToClient(USB_SERVICE, USB_PLUGIN, NULL);
				//USB_StartService(USB_SERVICE_UDISK);
				#endif
				
			}
		}
		else if(signal->SignalCode == USB_VBUS_PLUG_IN)						// VBus connected. interrupt from gpio
		{
		
			//SCI_PutSemaphore(s_USB_VBUS_semaphore);
			SCI_TraceLow("USB_VbusThread USB_VBUS_PLUG_IN\n");
			/*we do not known it's Host connected or charger connected,start usb log to try enumerate*/
			USB_StartService(USB_SERVICE_UCOM);
	
			SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
		}
		else if(signal->SignalCode == USB_VBUS_PLUG_OUT)					// VBus removed. interrupt from gpio
		{
		
			SCI_TraceLow("USB_VbusThread USB_VBUS_PLUG_OUT\n");
			if(1/* SCI_SUCCESS == SCI_GetSemaphore(s_USB_VBUS_semaphore,SCI_NO_WAIT)*/ )
			{
#if 1//ndef MODEM_PLATFORM //bringup_temp should be 1,mmi to stop service
				SCI_SendEventToClient(USB_SERVICE, USB_PLUGOUT, NULL);
#else
				USB_StopService(USB_SERVICE_UCOM);
#endif
				SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);

			}
			POWER_ClearAutoTestMode();
		}
		SCI_FREE(signal);
		signal = NULL;
	}
}
/*****************************************************************************/
//	Function Name: USB_VbusThreadInit
//  Description:   This function create usb vbus interrupt service
//	Return Value:  none
//  Parameters:
/*****************************************************************************/
PUBLIC void USB_VbusThreadInit()
{
	s_USB_VBUS_semaphore = SCI_CreateSemaphore("USB VBUS semaphore", 0);
	usb_isr_id = SCI_CreateThread("T_USB_VBUS_HANDLER",
									"Q_VBUS",
									USB_VbusThread,
									0,
									0,
									VBUS_STACK_SIZE,
									VBUS_QUEUE_NUM,
									PRI_USB_VBUS,
									SCI_PREEMPT,
									SCI_AUTO_START);
}
#endif

