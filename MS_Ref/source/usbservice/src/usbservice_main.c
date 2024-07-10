
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
#ifndef _USBSERVICE_MAIN_C_
#define _USBSERVICE_MAIN_C_

/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */
#include "ms_ref_usbservice_trc.h"
#include "usbservice_api.h"
#include "cmddef.h"
#include "dal_power.h"
#include "sio.h"
#include "usb_drv.h"
#include "priority_system.h"
#include "deepsleep_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
#define USB_STACK_SIZE                  0x1000
#define USB_QUEUE_NUM                   0x0010
#define TASK_USBSERVICE_PRI                    PRI_USB_SERVICE //20

/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/
BLOCK_ID usb_main_id;
LOCAL uint32 g_is_calibration = 0;

/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/
USB_GADGET_DRIVER_T *g_USBService_table[USB_SERVICE_MAX]= {NULL};



/*----------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                  *
 **---------------------------------------------------------------------------*/

//extern USB_SERVICE_E s_cur_srv;
extern void USB_SetChargeStatus(uint32 status);
extern void drvUsbDisable(void);
/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/

PUBLIC uint32 USB_GetCaliMode(void)
{
    return g_is_calibration;
}

/*****************************************************************************/
//  Function Name: USB_NotifyClientService
//  Description:   This function notify the client which registered to usbservice
//  Return Value:  none
//  Parameters:
/*****************************************************************************/
LOCAL void USB_NotifyClientService (uint32 servicetype, uint32 state)
{
    switch (servicetype)
    {
        case USB_SERVICE_UDISK:

            if (state==SCI_TRUE)
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UDISK_STARTED, NULL);
            }
            else
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UDISK_STOPPED, NULL);
            }

            break;
        case USB_SERVICE_UCOM:

            if (state==SCI_TRUE)
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UCOM_STARTED, NULL);
            }
            else
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UCOM_STOPPED, NULL);
            }

            break;
        case USB_SERVICE_UPCC:

            if (state==SCI_TRUE)
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UPCC_STARTED, NULL);
            }
            else
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UPCC_STOPPED, NULL);
            }

            break;
        case USB_SERVICE_LOG:

            if (state==SCI_TRUE)
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_LOG_STARTED, NULL);
            }
            else
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_LOG_STOPPED, NULL);
            }

            break;
        case USB_SERVICE_MTP:

            if (state==SCI_TRUE)
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_LOG_STARTED, NULL);
            }
            else
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_LOG_STOPPED, NULL);
            }

            break;
        case USB_SERVICE_UCOM_UMS:

            if (state==SCI_TRUE)
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UCOM_UMS_STARTED, NULL);
            }
            else
            {
                SCI_SendEventToClient (USB_SERVICE, USB_SERVICE_UCOM_UMS_STOPPED, NULL);
            }

            break;

        default:
            SCI_SendEventToClient (USB_SERVICE, USB_ERROR, NULL);
            break;
    }
}

/*****************************************************************************/
//  Function Name: USB_MainThread
//  Description:   This function is the usbservice main thread
//  Return Value:  none
//  Parameters:
/*****************************************************************************/
extern void ucom_test(void);

LOCAL void USB_MainThread (uint32 argc, void *argv)
{
    xSignalHeader signal;
    uint8 calibmode = 0;
    USB_SERVICE_E service_type;
    BLOCK_ID    tid = SCI_IdentifyThread();
    MCU_MODE_E reset_mode;
    POWER_RESTART_CONDITION_E restart_condition;
	//USB_LOG_TRACE("Enter USB_MainThread\n");

    //USB_CreateFreqHandle();
    reset_mode = POWER_GetResetMode();
    restart_condition = POWER_GetRestartCondition();

    if ( ( (CALIBRATION_POST_MODE == reset_mode) ||
            (CALIBRATION_MODE == reset_mode) ||
            (CALIBRATION_POST_NO_LCM_MODE == reset_mode)) &&
            (RM_GetDloaderMode() == 1))
    {
        SIO_CONTROL_S   m_dcb_data;
        calibmode = 1;
		
        g_is_calibration = 1;
		/*
	    //init service type
        m_dcb_data.flow_control = 0;
        m_dcb_data.baud_rate    = SIO_GetBaudRate(COM_DEBUG);
        SCI_DisableIRQ();
        SIO_Close(COM_DEBUG);
        SIO_Create(COM_DEBUG,VIR_COM0,&m_dcb_data);
        SCI_RestoreIRQ();
        */
        USB_SetChargeStatus(2); //USB_STATE_VBUS_STABLE assume vbus is stable at calimode
        //drvUsbSetAttach(TRUE);
        USB_StartService (USB_SERVICE_UCOM);
    }
    else if (CHGMNG_IsChargeConnect())
    {
        #if 0
        //UDC_HWInit();
        drvUsbInit();
        //USB_SetChargeStatus(3); //USB_STATE_VBUS_STABEL,assume vbus is stable when restart by charger
        //USB_StartService (USB_SERVICE_LOG);//for debug
        drvUsbSetAttach(TRUE);
        #else //bringup_temp
        if ((RESTART_BY_CHARGE == restart_condition)/*REFPARAM_GetDebugPortPhyNo() != VIR_COM1*/) //仅充电
        {
            SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
            USB_StartService (USB_SERVICE_NULL);
            
        }
        else
        {
            drvUsbInit();
            drvUsbSetAttach(TRUE);
        }
            
        #endif
    }

    for (;;)
    {
        signal = (xSignalHeader) SCI_GetSignal (tid);
        service_type = signal->SignalCode&0xFF;

        if ( (signal->SignalCode & START_SERVICE) == START_SERVICE)
        {
        
        	if(service_type!=USB_SERVICE_NULL){

            	UCOM_SetWorkMode(service_type, calibmode);

            	drvUsbEnable(CONFIG_USB_DETECT_DEBOUNCE_TIME);
        	}
            USB_NotifyClientService (service_type,1);
        }
        else if ( (signal->SignalCode & STOP_SERVICE) == STOP_SERVICE)
        {
        
			//Debug_Uart4_puts("USB_MainThread STOP_SERVICE\n");
            drvUsbDisable();
            USB_NotifyClientService (service_type,0);
            s_cur_srv=USB_SERVICE_NULL;
			UCOM_SetWorkMode(USB_SERVICE_NULL, 0);

        }
        else
        {
            //SCI_TRACE_LOW:"\nUSB_MainThread signal error\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USBSERVICE_MAIN_214_112_2_18_1_55_27_0,(uint8*)"");
        }

        SCI_FREE (signal);
        signal = NULL;
    }
}

/*****************************************************************************/
//  Function Name: USB_SendEvent
//  Description:   This function is the usbservice notify client last function
//  Return Value:  none
//  Parameters:
/*****************************************************************************/
LOCAL void USB_SendEvent (BLOCK_ID id, uint32 argc, void *argv)
{
    //Do nothing ;
}

/*****************************************************************************/
//  Function Name: USB_Main
//  Description:   This function is the main function to init usbservice
//  Return Value:  none
//  Parameters:
/*****************************************************************************/
LOCAL void USB_Main()
{

    /* Register upm task to soft watchdog monitor */

    usb_main_id = SCI_CreateThread ("T_USBSERVICE_HANDLER",
                                    "Q_USB",
                                    USB_MainThread,
                                    0,
                                    0,
                                    USB_STACK_SIZE,
                                    USB_QUEUE_NUM,
                                    TASK_USBSERVICE_PRI,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);


}

/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Function Name:    USB_RegService
//  Description:      This function let protocol layer to register service
//  Return Value:
//  Parameters:
/*****************************************************************************/
PUBLIC int USB_RegService (USB_SERVICE_E type, USB_GADGET_DRIVER_T *pDrv)
{
    if (NULL == pDrv)
    {
        return FALSE ;
    }

    //Copy source struct to destination pointer.
    g_USBService_table[type] = pDrv;

    return TRUE ;
}

/*****************************************************************************/
//  Function Name:    USB_InitBottomHalf
//  Description:      This function init the usbservice bottom half.
//  Return Value:
//  Parameters:
/*****************************************************************************/
PUBLIC int USB_InitBottomHalf (void)
{
    SCI_CreateClientList (USB_SERVICE, (USB_MSG_MAX & 0x0ff), (REG_CALLBACK) USB_SendEvent);

    USB_VbusThreadInit();

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Function Name:    USB_InitBottomHalf
//  Description:      This function init the usbservice top half.
//  Return Value:
//  Parameters:
/*****************************************************************************/
PUBLIC int USB_InitTopHalf (void)
{
    USB_Main();

    return SCI_SUCCESS;
}
#endif
