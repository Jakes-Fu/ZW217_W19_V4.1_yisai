
#ifndef _USBSERVICE_H_
#define _USBSERVICE_H_

#include "os_api.h"
#include "sci_service.h"
#include "gadget.h"



#define START_SERVICE 0xF000
#define STOP_SERVICE  0x0F00
#define UPM_DETECT_TIMER        (15000) 

typedef enum
{
	USB_PLUGIN = (USB_SERVICE << 8) | 1 ,
	USB_PLUGOUT,
	USB_PLUG_OUT_START,
	USB_SERVICE_UDISK_STARTED,
	USB_SERVICE_UDISK_STOPPED,
	USB_SERVICE_UCOM_STARTED,
	USB_SERVICE_UCOM_STOPPED,
	USB_SERVICE_UPCC_STARTED,
	USB_SERVICE_UPCC_STOPPED,
	USB_SERVICE_LOG_STARTED,
	USB_SERVICE_LOG_STOPPED,
	USB_SERVICE_MTP_STARTED,
	USB_SERVICE_MTP_STOPPED,
	USB_SERVICE_UCOM_UMS_STARTED,
	USB_SERVICE_UCOM_UMS_STOPPED,
	USB_READY,
	USB_ERROR,
	USB_MSG_MAX
} USB_MSG_SERVICE_E;


typedef enum
{   
  USB_SERVICE_NULL = 0,
	USB_SERVICE_UDISK,
	USB_SERVICE_UCOM,
	USB_SERVICE_UPCC,
	USB_SERVICE_LOG,
	USB_SERVICE_MTP,
	USB_SERVICE_UCOM_UMS,
	USB_SERVICE_MAX
}USB_SERVICE_E;

typedef enum usb_freq_lvl_tag
{
	USB_FREQ_SET_LVL_LOW,
	USB_FREQ_SET_LVL_MID,
	USB_FREQ_SET_LVL_HIGH,
	USB_FREQ_SET_LVL_MAX
}USB_FREQ_LVL_E;
	
typedef enum 
{
	USB_VBUS_PLUG_IN,
	USB_VBUS_PLUG_OUT,
	USB_HOST_PLUG_IN,
}USB_PLUG_E;

/*****************************************************************************/
//  Description: USBService init function
/*****************************************************************************/
PUBLIC int USB_InitBottomHalf(void);

PUBLIC int USB_InitTopHalf(void);

PUBLIC int USB_RegService(USB_SERVICE_E type, USB_GADGET_DRIVER_T *pDrv);

PUBLIC uint32 USB_GetServiceCount(void);

PUBLIC USB_SERVICE_E USB_GetNextService(void);

PUBLIC USB_SERVICE_E USB_GetCurService(void);

PUBLIC BOOLEAN USB_StartService(USB_SERVICE_E servicetype);

PUBLIC BOOLEAN USB_StopService(USB_SERVICE_E servicetype);

PUBLIC void USB_SetFreq(uint32 level);
PUBLIC void USB_VbusThreadInit();

PUBLIC void USB_CreateFreqHandle(void);
PUBLIC uint8 REFPARAM_GetDebugPortPhyNo(void); 
PUBLIC uint32 RM_GetDloaderMode(void);
PUBLIC uint32 getRMUartBaseAddr (void);
PUBLIC void USB_SetCharge(uint32 );
PUBLIC BOOLEAN UCOM_GetModemUbotEnable(void);//just for resoving sxudc pclint warning
PUBLIC void UsbService_SendMsgToVbusThread(USB_PLUG_E sig,uint32 sig_param);

extern USB_SERVICE_E s_cur_srv;

extern USB_GADGET_DRIVER_T *g_USBService_table[];

extern BLOCK_ID usb_main_id;
extern BLOCK_ID usb_isr_id;
extern SCI_TIMER_PTR            s_UPM_timer_ptr;

#endif
