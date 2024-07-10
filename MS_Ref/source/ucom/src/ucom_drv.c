/******************************************************************************
 ** File Name:    UCOM_drv.c                                                  *
 ** Author:       weihua.wang                                                 *
 ** DATE:         2005-11-8                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2005-11-8     weihua.wang     Create.                                     *
 ** 2006-03-17    weihua.wang     modify the comment
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_ucom_trc.h"
#include "usb200.h"
#include "udc_api.h"
#include "ucom_common.h"
#include "ucom_main.h"
#include "ucom_drv.h"
#include "ucom_api.h"
#include "dal_chr.h"
#include "auto_download.h"
#include "usb_device.h"
#ifdef WINUSB_SUPPORT
#include "cmddef.h"
#endif
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

extern uint32 UCOM_interface[];

__align(4) LOCAL  uint8 ucom_venderres[4] =
{
	0x00,0x00,0x00,0x00
};

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    usb get descriptor handler
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:           CTL:80 06 00 xx  00 00 xx 00
/*****************************************************************************/
PUBLIC void UCOM_GetDevDescriptor (uint16 req, uint16 wValue, uint16 wIndex, uint16 wLen)
{
    ucomWorkMode_t ucom_wm = UCOM_GetWorkMode();
    USB_DEVICE_DESCRIPTOR_T *device_desc = (USB_DEVICE_DESCRIPTOR_T *)g_ucom_device_desc;
	USB_HWORD_VALUE_U value;
	uint16 length = 0;
	char *pData = NULL;
    uint32 temp;
    
#ifdef WINUSB_SUPPORT	
	PUCOM_STR_DESC_T pStr = UCOM_GetStrInfo();
#endif
	//SCI_TRACE_LOW:"\nUCOM_GetDevDescriptor\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_DRV_91_112_2_18_1_54_53_0,(uint8*)"");
	value.wValue  = wValue;
	switch (value.bmDescriptor.type){
 	case USB_DEVICE_DESCRIPTOR_TYPE:
		if (wLen >= sizeof(USB_DEVICE_DESCRIPTOR_T)) {
			length = sizeof(USB_DEVICE_DESCRIPTOR_T);
		} else {
            length = wLen;
        }

        switch (ucom_wm) {
        case UCOM_WORK_DATA_DEBUG:
            device_desc->idProduct = USB_SPREADTRUM_PID_TWOINTERFACE;
            break;

        case UCOM_WORK_WITH_UBOT:
            device_desc->idProduct = USB_SPREADTRUM_CDROM_MODEM_ID;
            break;

        case UCOM_WORK_DEBUG_DATA_MODEM:
            device_desc->idProduct = USB_SPREADTRUM_DATA_DEBUG_MODEM_ID;
            break;
#ifdef WINUSB_SUPPORT	           
        case UCOM_WORK_DEBUG_DATA_UMS:
            device_desc = (USB_DEVICE_DESCRIPTOR_T *)g_UCOM_DeviceDescr_threeinterfaceForWinUsb;
            break;
#endif
        case UCOM_WORK_COM_DEBUG:
        case UCOM_WORK_AUTO_DWLD:
        default:
            device_desc->idProduct = USB_SPREADTRUM_CALIB_ID;
            break;
        }
        pData = (char *)device_desc;
        break;

    case USB_CONFIGURATION_DESCRIPTOR_TYPE:
        switch (ucom_wm) {
        case UCOM_WORK_WITH_UBOT:
            if (wLen >= UCOM_CFG_DESCR_LEN_TWOINF) {
                length = UCOM_CFG_DESCR_LEN_TWOINF;
            } else {
                length = wLen;
            }
            pData = (char *)g_UCOM_UBOT_CfgDescr;
            break;

        case UCOM_WORK_DATA_DEBUG:
            if (wLen >= UCOM_CFG_DESCR_LEN_TWOINF) {
                length = UCOM_CFG_DESCR_LEN_TWOINF;
            } else {
                length = wLen;
            }
            pData = (char *)g_UCOM_CfgDescr_twointerface;
            break;

        case UCOM_WORK_DEBUG_DATA_MODEM:
            if (wLen >= UCOM_CFG_DESCR_LEN_FOURIF) {
                length = UCOM_CFG_DESCR_LEN_FOURIF;
            } else {
                length = wLen;
            }
            pData = (char *)g_UCOM_APCP_DATA_MODEM_CfgDesc;
            break;

#ifdef WINUSB_SUPPORT
        case UCOM_WORK_DEBUG_DATA_UMS:
                if (wLen >= UCOM_CFG_DESCR_LEN_THREEINF) {
                    length = UCOM_CFG_DESCR_LEN_THREEINF;
                } else {
                    length = wLen;
                }
                pData = (char *)g_UCOM_CfgDescr_threeinterfaceForWinUsb;
                break;

#endif
        case UCOM_WORK_AUTO_DWLD:
            if (wLen >= UCOM_CFG_DESCR_LEN) {
                length = UCOM_CFG_DESCR_LEN;
            } else {
                length = wLen;
            }
            pData = (char *)g_UCOM_CfgDescr_AutoDloader;
            break;

        case UCOM_WORK_COM_DEBUG:
            if (wLen >= UCOM_CFG_DESCR_LEN) {
                length = UCOM_CFG_DESCR_LEN;
            } else {
                length = wLen;
            }
            pData = (char *)g_UCOM_CfgDescDebug;
            break;

        case UCOM_WORK_COM_DATA:
        default:
            if (wLen >= UCOM_CFG_DESCR_LEN) {
                length = UCOM_CFG_DESCR_LEN;
            } else {
                length = wLen;
            }
            pData = (char *)g_UCOM_CfgDescData;
            break;
        }
        break;

    case USB_STRING_DESCRIPTOR_TYPE:
        switch (value.bmDescriptor.index){
            //USB 2.0 spec chapter 9.6.7
            case USB_STR_INDEX_LANGUAGE :
                if (wLen > sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T)) {
                    length = sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T);
                } else {
                    length = wLen;
                }
                pData = (char *)&g_UCOM_LanguageStr;
                break;

            //USB 2.0 spec chapter 9.6.1
            case USB_STR_INDEX_SERIALNUMBER:
#ifdef 	WINUSB_SUPPORT			
				if(REFPARAM_GetUsbServicesFlag()==0)
				{
				    temp = pStr[USB_STR_INDEX_SERIALNUMBER].len;;
                    if (wLen > temp){
                        length  = temp;        
                    }
                    else{
                        length  = wLen;
                    }
				    pData   = pStr[USB_STR_INDEX_SERIALNUMBER].pStr;
				}
				else
#endif  
                {
                    if (wLen > sizeof(UCOM_STRING_SERIALNUMBER_DESCRIPTOR_T)) {
                        length = sizeof(UCOM_STRING_SERIALNUMBER_DESCRIPTOR_T);
                    } else {
                        length = wLen;
                    }
                    pData = (char *)&g_UCOM_SerialNumStr;
                }    
                break;

            case USB_STR_INDEX_MANUFACTURER:
#ifdef 	WINUSB_SUPPORT					
				if(REFPARAM_GetUsbServicesFlag()==0)
				{
				    temp = pStr[USB_STR_INDEX_MANUFACTURER].len;
    				if (wLen > temp){
                        length  = temp;        
                    }
                    else{
                        length  = wLen;
                    }
					pData 	= pStr[USB_STR_INDEX_MANUFACTURER].pStr;
				}
				else
#endif  
                {
                    if (wLen > sizeof(UCOM_STRING_MANUFACTURER_DESCRIPTOR_T)) {
                        length = sizeof(UCOM_STRING_MANUFACTURER_DESCRIPTOR_T);
                    } else {
                        length = wLen;
                    }
    
                    pData = (char *)&g_UCOM_StrManufacturer;
                }
                break;

            case USB_STR_INDEX_PRODUCT:
#ifdef 	WINUSB_SUPPORT			
				if(REFPARAM_GetUsbServicesFlag()==0)
				{
					// if select winusb & ums mode, change the product string info
					if(UCOM_WORK_DEBUG_DATA_UMS == UCOM_GetWorkMode())
					{
						UCOM_ChangePrdStrInfo();
					}
				
				    temp = pStr[USB_STR_INDEX_PRODUCT].len;
    				if (wLen > temp){
                        length  = temp;        
                    }
                    else{
                        length  = wLen;
                    }
					pData 	= pStr[USB_STR_INDEX_PRODUCT].pStr;
				}
				else
#endif          
                {
                    if (wLen > sizeof(UCOM_STRING_PRODUCT_DESCRIPTOR_T)) {
                        length = sizeof(UCOM_STRING_PRODUCT_DESCRIPTOR_T);
                    } else {
                        length = wLen;
                    }
                    pData = (char *)&g_UCOM_StrProduct;
                }	
                break;
#ifdef 	WINUSB_SUPPORT					
			case USB_WINUSB_OS_DESCRIPTOR_TYPE:
				if(REFPARAM_GetUsbServicesFlag()==0)
				{
					temp = pStr[UCOM_DEV_STRING_INDEX-1].len;
    				if (wLen > temp){
                        length  = temp;        
                    }
                    else{
                        length  = wLen;
                    }
					pData 	= pStr[UCOM_DEV_STRING_INDEX-1].pStr;

		
				}
				break;
#endif
            default:
                break;
        }
        break;
#ifdef WINUSB_SUPPORT   /*USB-IF TEST TOOL*/      
    case USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE:
        length = wLen;
        pData   = (char *)&g_UCOM_Device_Qualifier_Descr;  

        break;
        
    //we don't support other-speed descriptor for simple
/***
    case USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR_TYPE:    
        break;
***/       
#endif        
    default:
        break;
	}

	UsbDevice_Ctrl_Transfer((uint32 *)pData,length);
}

/*****************************************************************************/
//  Description:    handle vendor specified command
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void UCOM_VendorCommand (uint16 req,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
#ifdef WINUSB_SUPPORT
	USB_HWORD_VALUE_U value;
	USB_HWORD_VALUE_U index;
	char *pData =NULL;
	uint16 length =0;
	uint16 temp;
	uint8 bRequest =(req >>8) & 0xff;
	index.wValue = wIndex;
    value.wValue = wValue;
	if(REFPARAM_GetUsbServicesFlag()==0)
	{
		if(bRequest == USB_REQUEST_MS_VENDOR)
		{
			switch (index.bmDescriptor.index)
			{
			           case USB_REQUEST_COMPATIBLE_ID:
				            temp = sizeof(WINUSB_ExtendedCompatId_Descritpor)/sizeof(uint8);
			                if(wLen > temp)
			                    length = temp;
			                else
			                    length = wLen;

			                pData = (char *)&WINUSB_ExtendedCompatId_Descritpor;
			                break; 

			            case USB_REQUEST_INTERFACE_GUID:
    
			                temp = sizeof(WINUSB_ExtendedProperty_InterfaceGUID_Descritpor1)/sizeof(uint8);
			                if(wLen > temp)
			                    length = temp;
			                else
			                    length = wLen;

                            switch(value.bmDescriptor.index)
                            {
                                case 0: //AT
                                    pData = (char *)&WINUSB_ExtendedProperty_InterfaceGUID_Descritpor0;   
                                    break;
                                case 1: //Diag
                                    pData = (char *)&WINUSB_ExtendedProperty_InterfaceGUID_Descritpor1;
                                    break;
                                case 2: //UMS
                                    pData = NULL;
                                default:    
                                    break;
                            }
                               
			                break;

			            default:
			                temp = sizeof(ucom_venderres)/sizeof(uint8);
			                if(wLen > temp)
			                    length = temp;
			                else
			                    length = wLen;

			                pData = (char *)&ucom_venderres;
			                break;

			}


		}
		UsbDevice_Ctrl_Transfer((uint32 *)pData,length);
	}
#else
	
	//SCI_TRACE_LOW:"\nUCOM_VendorCommand\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_DRV_244_112_2_18_1_54_53_1,(uint8*)"");
	if ( ((req&0xFF)&USB_DIR_IN)>>7 ) {									// direction is 1
	    UsbDevice_Ctrl_Transfer((uint32 *)ucom_venderres,wLen);
	}
#endif
}

/*****************************************************************************/
//  Description:    handle class specified command
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void UCOM_ClassCommand (uint16 req,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
    uint8 endp_addr = 0;
    uint8 b_req, b_req_type;
   	USB_HWORD_VALUE_U value;
    UCOM_DEVICE_T *ucom = NULL;
#ifdef WINUSB_SUPPORT
    MCU_MODE_E reset_mode = POWER_GetResetMode();
#endif
    uint8 enable;
    
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_DRV_263_112_2_18_1_54_53_2,(uint8*)"dd",req,wValue);

	//Data:21 22 00 00, it's indicated that the virtual com is close
	//Data:21 22 01 xx, it's indicated that the virtual com is open
	//xx is endpoint address
    b_req = (req >> 8) & 0xff;
    b_req_type = req & 0xff;
	value.wValue  = wValue;

    enable = value.bmDescriptor.index;
    endp_addr = value.bmDescriptor.type;
    if (endp_addr == 0 || b_req != 0x22) {
        return;
    }

#ifdef 0//WINUSB_SUPPORT

        if ((REFPARAM_GetUsbServicesFlag() == 0) && (USB_GetCaliMode() == 0))
        {
            return;
        }
#endif

    ucom = UCOM_getDevfromEndp(endp_addr);
    if (ucom) {
        uint8 device_id = (ucom == &g_UCOM_manager.dev[0]) ? USB_COM0 : USB_COM1;
        SCI_DisableIRQ();
        if (enable == 1) {
            ucom->state = UCOM_DEV_READY;
            ucom->tx_state = UCOM_TX_STATE_IDLE;
            osiFifoReset(&ucom->tx_fifo);
            osiFifoReset(&ucom->rx_fifo);

            if (ucom->callback) {
                ucom->callback(EVENT_WRITE_COMPLETE);
            }
        } else if (enable == 0) {
            ucom->state = UCOM_DEV_PRESENT;
        }
        SCI_RestoreIRQ();
    }
}

/*****************************************************************************/
//  Description:    handle the standard configuration of set configure
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void UCOM_SetConfigure (uint16 req,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
    uint8 i;
    for (i = 0; i < UCOM_MAX_COM_NUM; ++i) {
        UCOM_DEVICE_T *ucom = UCOM_GetDevfromPort(i);
        if (ucom == NULL || UCOM_interface[i] == 0xffffffff)
            continue;

        ucom->state = UCOM_DEV_PRESENT;
        ucom->tx_state = UCOM_TX_STATE_IDLE;
        osiFifoReset(&ucom->rx_fifo);
        osiFifoReset(&ucom->tx_fifo);
        if (ucom->rx_buf.buf_size != 0) {
            UsbDevice_Start_receive(UCOM_interface[i], (uint32 *)ucom->rx_buf.usb_buf,
                                    ucom->rx_buf.buf_size);
        }
    }
}

/*****************************************************************************/
//  Description:    It is a NULL function  to avoid useless pointer
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void DoNothing (uint16 req,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
	// do nothing;
}

/*****************************************************************************/
//  Description:    Register call back function for the usb ep0 request
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_RegCallback  (uint8 type,			//Request type;
					uint8 recipient,			//Request recipient;
					uint8 request,			//Specific request ;
					USB_FUNCTION callback ) //Call back funcition  ;
{
	UCOM_INDEX_U index ;
    uint32 i = 0;

    index.bValue          = 0;
    index.mBits.type      = type;
    index.mBits.recipient = recipient;
    index.mBits.request   = request;

    for (i = 0; i < UCOM_MAX_FUNC_NUM;i++) {
        if (g_ucom_functions[i].func_ptr == DoNothing){
            g_ucom_functions[i].index.bValue = index.bValue ;
            g_ucom_functions[i].func_ptr = callback;
            return TRUE;
        }
    }

    if (UCOM_MAX_FUNC_NUM == i)
    {
        UCOM_ASSERT (0); /*assert verified*/
    }

    return FALSE ;
}

/*****************************************************************************/
//  Description:    Find call back function base on the EP0 reqeust
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC USB_FUNCTION USB_FindCallback  (uint8 type,			//Request type;
					uint8 recipient,			//Request recipient;
					uint8 request			//Specific request ;
					)
{
	UCOM_INDEX_U index ;
    uint32 i = 0;

    index.bValue          = 0;
    index.mBits.type      = type;
    index.mBits.recipient = recipient;
    index.mBits.request   = request;

    for (i=0;i<UCOM_MAX_FUNC_NUM;i++) {
        if (g_ucom_functions[i].index.bValue == index.bValue)
          return g_ucom_functions[i].func_ptr;
    }
    return NULL;
}

/*****************************************************************************/
//  Description:    Call this function to send data to special end point.
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:           pBuf  must be aligned  4  ;
//                  len   must be less than 64 ;
//                  ep_id must be less than 6  ;
/*****************************************************************************/
PUBLIC uint32 UCOM_EPxSendData  (uint8 device_id,uint32 * pBuf,uint32 len)
{
	UsbDevice_Start_send(UCOM_interface[device_id],pBuf,len);
	return 0;
}

/*****************************************************************************/
//  Description:    open virtual com without pc driver
//	Global resource dependence: 
//  Author:         eric.xu
/*****************************************************************************/
PUBLIC void UCOM_DevReady()
{
    /** Data:21 22 01 xx,it's indicated that the virtual com is open.
    *** winusb Device has no SPRD driver, can't send class command.
    *** so must config device ready after set config
    **/
    uint8 i;
    for (i = 0; i < UCOM_MAX_COM_NUM; ++i) {
        UCOM_DEVICE_T *ucom = UCOM_GetDevfromPort(i);
        if (ucom == NULL || UCOM_interface[i] == 0xffffffff)
            continue;

        ucom->state = UCOM_DEV_PRESENT;
        ucom->tx_state = UCOM_TX_STATE_IDLE;
        osiFifoReset(&ucom->rx_fifo);
        osiFifoReset(&ucom->tx_fifo);
        if (ucom->rx_buf.buf_size != 0) {
            UsbDevice_Start_receive(UCOM_interface[i], (uint32 *)ucom->rx_buf.usb_buf,
                                    ucom->rx_buf.buf_size);
        }
    }   
}

PUBLIC void UCOM_DevStop()
{
    uint32 interface_num;
    UCOM_DEVICE_T *dev_ptr = NULL;

    for(interface_num = 0; interface_num < 2; interface_num++)
    {
        dev_ptr = &g_UCOM_manager.dev[interface_num];
        dev_ptr->state = UCOM_DEV_PRESENT;
        UsbDevice_StopTransfer(interface_num);
    }
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End
