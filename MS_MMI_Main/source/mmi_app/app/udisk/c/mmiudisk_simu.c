/*****************************************************************************
** File Name:      mmiudisk_simu.c                                                *
** Author:                                                                   *
** Date:           19/9/2005                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    Udisk simulation                    *
/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2005       cancan.you       Create
******************************************************************************/

#define _MMIUDISK_SIMU_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_udisk_trc.h"
#include "std_header.h"
#include "mmiudisk_simu.h"
#include "mmiudisk_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static USB_SERVICE_E s_cur_srv = USB_SERVICE_NULL;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
//PUBLIC BOOLEAN UPM_SetService (UPM_SERVICETYPE_E type)
//{
//    return TRUE;
//}
//
///*****************************************************************************/
////  Description:    Call this function to register protocol's handler to UPM;
////	Global resource dependence: 
////  Author:         Daniel.Ding
////	Note:           
///*****************************************************************************/
//PUBLIC UPM_SERVICETYPE_E UPM_GetService (void)
//{
//    return UPM_NO_SERVICE;
//}


/*****************************************************************************/
//  Description: start the pc camera
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_OpenPCCamera(void)
{
    return TRUE;
}

/*****************************************************************************/
//  Description: stop the pc camera
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_ClosePCCamera(void)
{
    return TRUE;
}

PUBLIC BOOLEAN UPCC_StartVideoStreaming (void)
{
	return TRUE;
}

PUBLIC int USB_GetNextService()
{
	
}

PUBLIC uint32 USB_GetServiceCount()
{
	return USB_SERVICE_NULL;
}

/*****************************************************************************/
//  Description: set Usb to COM
//	Global resource dependence: 
//  Author: liqing.peng
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_CreatForCOM(uint32 type)
{
    return;
}

PUBLIC BOOLEAN USB_StartService(USB_SERVICE_E servicetype)
{
	s_cur_srv = servicetype;
	switch(servicetype)
	{
    case USB_PLUGIN:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGIN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_SIMU_102_112_2_18_3_4_9_51,(uint8*)"");
        // 该消息用于汇报U盘插入。
        MMIAPIUdisk_HandleUsbCablePlugIn();
        break;
        
    case USB_PLUGOUT:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGOUT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_SIMU_108_112_2_18_3_4_9_52,(uint8*)"");
        // 该消息用于汇报U盘被拔出。
        //   MMIAPIUdisk_SetIsUse(FALSE);
        MMIAPIUdisk_HandleUsbCablePlugOut();
        MMIAPIUdisk_SetUSBDelay(FALSE);
        break;
	case USB_SERVICE_UDISK:
		//usb service started
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UDISK);
		break;
	case USB_SERVICE_UCOM:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UCOM);
		break;
	case USB_SERVICE_UPCC:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UPCC);
		break;
	case USB_SERVICE_LOG:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_LOG);
		break;
	}
	return TRUE;
}

PUBLIC BOOLEAN USB_StopService(USB_SERVICE_E servicetype)
{
	
	switch(servicetype)
	{
    case USB_PLUGIN:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGIN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_SIMU_137_112_2_18_3_4_9_53,(uint8*)"");
        // 该消息用于汇报U盘插入。
        MMIAPIUdisk_HandleUsbCablePlugIn();
        break;
        
    case USB_PLUGOUT:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGOUT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_SIMU_143_112_2_18_3_4_9_54,(uint8*)"");
        // 该消息用于汇报U盘被拔出。
        //   MMIAPIUdisk_SetIsUse(FALSE);
        MMIAPIUdisk_HandleUsbCablePlugOut();
        MMIAPIUdisk_SetUSBDelay(FALSE);
        break;
	case USB_SERVICE_UDISK:
		//usb service stopped
        MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UDISK);
		break;
		break;
	case USB_SERVICE_UCOM:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UCOM);
		break;
	case USB_SERVICE_UPCC:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UPCC);
		break;

	case USB_SERVICE_LOG:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_LOG);
		break;
	}
	s_cur_srv = USB_SERVICE_NULL;
	return TRUE;
}

PUBLIC USB_SERVICE_E USB_GetCurService(void)
{
	return s_cur_srv;
}

PUBLIC BOOLEAN SIO_UserPortBusy(void)
{
    return FALSE;
}