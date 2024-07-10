/******************************************************************************
 ** File Name:    model.c                                                     *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "virtual_com.h"
#include "common.h"
#include "DRV_usb.h"
//#include "bsl_system.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define	USB_TIMEOUT		16000

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
__align(4) const unsigned char DeviceDescrSerial [] =
{
0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x40,
0x82, 0x17, 0x00, 0x4d, 0x02, 0x02, 0x00, 0x00, 
//0x7b, 0x06, 0x03, 0x23, 0x02, 0x02, 0x00, 0x00,
0x00, 0x01
};


__align(4) const uint8 ConfigDescrSerial[] =
{
0x09,0x02,0x20,0x00,0x01,0x01,0x00,0xc0,  
0x32,
0x09,0x04,0x00,0x00,0x02,0xff,0x00,0x00,
0x00,
0x07,0x05,0x83,0x02,0x40,0x00,0x00, //Ep 3 In
0x07,0x05,0x02,0x02,0x40,0x00,0x00  //Ep 2 OUT
};


__align(4) const unsigned char VenderRespond[] =
{
	0x00,0x00,0x00,0x00
};

char class_buf [64];


extern sio_handle   global_usb_handle;
extern USB_rx_buf_T buf_manager ;
/*--------------------------- External Data ---------------------------------*/
extern char         *recv_data_buffer_ptr;
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
void USB_Ep0_handler  (void);
void USB_Ep2_handler  (void);
void USB_Ep3_handler  (void);
//int USB_EPxReceiveData  (char ep_id ,unsigned int * pBuf,short len);
//int USB_EPxSendData  (char ep_id ,unsigned int * pBuf,int len);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
char VCOM_GetChar(USB_DOWNLOAD_STATUS_E *d_status_ptr)
{
	volatile unsigned char ch = 0;
	int condition ; 
    //INT_CTRL_T * int_ctl_ptr = (INT_CTRL_T *)INT_REG_BASE;       
	USB_DEV_REG_T * ctl = (USB_DEV_REG_T *)USB_REG_BASE;
	USB_DEV_INT_STS_U status ;
	volatile unsigned long old_tick, new_tick; 

	//if no any char, loop in here ;
	condition = TRUE ;
	
	old_tick = fdl_GetTickCount();

	do{
		if(*(volatile uint32 *)(INT1_IRQ_RAW_STS) & BIT_5){
			status.dwValue = ctl->int_sts.dwValue ;
			//dispatch handler message according to usb int status ;
			if (status.dwValue  & USB_INT_STS_MASK )
			{
				//dispatch EPx handler ;
				if (status.mBits.ep0 )
				{
					USB_Ep0_handler();
				}
				if (status.mBits.ep2 )
				{
					USB_Ep2_handler();
				}
				if (status.mBits.ep3 )
				{
					USB_Ep3_handler();
				}
			}
			//clear usb interrupt ;
			ctl->int_clr.dwValue = USB_INT_DEV_MASK ;
		}
		
		if (buf_manager.read != buf_manager.write)
		{
			ch = buf_manager.usb_rx_buf[buf_manager.read];
			buf_manager.read++;
			if (buf_manager.read >= USB_RECV_LIMIT){
				buf_manager.read = 0;
			}
			condition = FALSE ;
		}
		
		new_tick = fdl_GetTickCount();
		if(new_tick - old_tick > USB_TIMEOUT) 
		{
            if (PNULL != d_status_ptr)
            {
                *d_status_ptr = USB_DOWNLOAD_TIMEOUT;
            }
            //SetTraceValue(TRACE_USB_DOWNLOAD_TIMEOUT);
            return (char)ch ;
		}
	}while (condition);

	return (char)ch ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_GetDevDescriptor (char bDirection,short wValue,short wIndex ,short wLen)
{
	volatile VALUE_U temp;
	short wResidue = 0;
	char *pData = 0;

	temp.wValue  = wValue;
	switch (temp.sDescriptor.type){
 	case USB_DEVICE_DESCRIPTOR_TYPE:
		if (wLen >= 0x040){
			wResidue = 18;			
		}
		else{
        	wResidue = wLen;
        }
        pData 	 = (char *)&DeviceDescrSerial;
        break;
    case USB_CONFIGURATION_DESCRIPTOR_TYPE:
		if (wLen >= 0x0ff){
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)						
			wResidue = 0x20;	
#else
			wResidue = 0x27;			
#endif
		}
		else{
			wResidue = wLen;
		} 
        pData    = (char *)&ConfigDescrSerial;
        break;
    case USB_STRING_DESCRIPTOR_TYPE:
        break;
    default:
        break;
	}
	//Write data to EP in & send data to host  ;
	USB_EPxSendData (USB_EP0_IN,(unsigned int *)pData,wResidue);
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_VendorCommand (char bDirection,short wValue,short wIndex ,short wLen)
{
	char *pData = 0;
	if (bDirection){
		pData    = (char *)&VenderRespond;
		USB_EPxSendData (USB_EP0_IN,(unsigned int *)pData,wLen);
	}
	else {
		pData    = (char *)&VenderRespond;
		USB_EPxReceiveData  (USB_EP0_OUT,(unsigned int *)pData,NULL);
	}
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_ClassCommand (char bDirection,short wValue,short wIndex ,short wLen)
{
	if ( bDirection ) {
		USB_EPxSendData (USB_EP0_IN,(unsigned int *)class_buf,wLen);	
	}
	else {
		USB_EPxReceiveData  (USB_EP0_OUT,(unsigned int *)class_buf,wLen);
	}
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void VCOM_SetAddress (char bDirection,short wValue,short wIndex ,short wLen)
{
	USB_EPxSendData (USB_EP0_IN,(unsigned int *)class_buf,NULL);
}

#ifndef FOTA_SUPPORT_NONE
char VCOM_GetChar_EX(USB_DOWNLOAD_STATUS_E *d_status_ptr)
{
	volatile unsigned char ch = 0;
	int condition ; 
    //INT_CTRL_T * int_ctl_ptr = (INT_CTRL_T *)INT_REG_BASE;       
	USB_DEV_REG_T * ctl = (USB_DEV_REG_T *)USB_REG_BASE;
	USB_DEV_INT_STS_U status ;
	volatile unsigned long old_tick, new_tick; 

	//if no any char, loop in here ;
	condition = TRUE ;
	
	old_tick = fdl_GetTickCount();

	do
	{
		if(*(volatile uint32 *)(INT1_IRQ_RAW_STS) & BIT_5){
			status.dwValue = ctl->int_sts.dwValue ;
			//dispatch handler message according to usb int status ;
			if (status.dwValue  & USB_INT_STS_MASK )
			{
				//dispatch EPx handler ;
				if (status.mBits.ep0 )
				{
					USB_Ep0_handler();
				}
				if (status.mBits.ep2 )
				{
					USB_Ep2_handler();
				}
				if (status.mBits.ep3 )
				{
					USB_Ep3_handler();
				}
			}
			//clear usb interrupt ;
			ctl->int_clr.dwValue = USB_INT_DEV_MASK ;
		}
		
		if (buf_manager.read != buf_manager.write)
		{
			ch = buf_manager.usb_rx_buf[buf_manager.read];
			buf_manager.read++;
			if (buf_manager.read >= USB_RECV_LIMIT){
				buf_manager.read = 0;
			}
			condition = FALSE ;
		}
		
		new_tick = fdl_GetTickCount();
		if(new_tick - old_tick > 1) //USB_TIMEOUT  5, 20:OK
		{
            if (PNULL != d_status_ptr)
            {
                *d_status_ptr = USB_DOWNLOAD_TIMEOUT;
            }
            //SetTraceValue(TRACE_USB_DOWNLOAD_TIMEOUT);
            return (char)ch ;
		}		
	}while (condition);

	return (char)ch ;
}
#endif

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
