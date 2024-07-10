#ifdef WRE_SUPPORT
#ifndef     _MMIWRE_EVENT_H_
#define     _MMIWRE_EVENT_H_

/*定义适配层系统消息*/
enum
{
	//USB 消息
	WRE_USB_PLUGIN_IND = 0,
	WRE_USB_PLUGOUT_IND,
	WRE_USB_READY_IND,
	WRE_USB_ERROR_IND,
	WRE_USB_SERVICE_UDISK_STARTED_IND,
	WRE_USB_SERVICE_UDISK_STOPPED_IND,
	WRE_USB_SERVICE_UCOM_STARTED_IND,
	WRE_USB_SERVICE_UCOM_STOPPED_IND,
	WRE_USB_SERVICE_UPCC_STARTED_IND,
	WRE_USB_SERVICE_UPCC_STOPPED_IND,
	WRE_USB_SERVICE_LOG_STARTED_IND,
	WRE_USB_SERVICE_LOG_STOPPED_IND,
	WRE_USB_SIGNAL_MAX,

	//CHARGER 消息
	WRE_CHR_MSG_CAP_IND = 0x10,      // Notify the battery's capacity
	WRE_CHR_MSG_CHARGE_START_IND,   // start the charge process.
	WRE_CHR_MSG_CHARGE_END_IND,     // the charge ended.

	WRE_CHR_MSG_WARNING_IND,        // the capacity is low, should charge.
	WRE_CHR_MSG_SHUTDOWN_IND,       // the capacity is very low and must shutdown.    

	WRE_CHR_MSG_CHARGE_FINISH_IND,      // the charge has been completed.
	WRE_CHR_MSG_CHARGE_DISCONNECT_IND,  // the charge be disconnect
	WRE_CHR_MSG_CHARGE_FAULT_IND,       // the charge fault, maybe the voltage of charge is too low.
	WRE_CHR_MSG_CHARGE_FAULT_OVERTEMP_IND  // the charge fault, maybe over temp.



};

#endif
#endif
