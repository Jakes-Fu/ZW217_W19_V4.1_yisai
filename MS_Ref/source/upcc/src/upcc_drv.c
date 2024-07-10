/******************************************************************************
 ** File Name:    UPCC_drv.c                                                  *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/22/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file contains the driver procedure for pc camera.      *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/22/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_DRV_C_
#define _UPCC_DRV_C_
    
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    
#include "ms_ref_upcc_trc.h"
#include "upcc_camera.h"
#include "upcc_drv.h"
#include "sc_reg.h"
#include "dal_dcamera.h"
#include "usb_drv.h"
#include "udc_api.h"
#include "usb_device.h"
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
#define UPCC_UVC_SUPPORT

#define UPCC_EVENT_CACHE_LOCK	0x01
#define UPCC_EVENT_TIMER_OUT	10
#define VALID_PAYLOAD_LEN       (UPCC_PC_RECEIVE_BLOCK_SIZE - UPCC_FRAME_LEN)
#define UPCC_FEATURE_NUM		0x11
/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                  *
 **---------------------------------------------------------------------------*/
extern UPCC_MANAGER_T g_UPCC_manager;
extern const uint8 g_UPCC_DeviceDescr[];
extern uint8 g_UPCC_ConfigDescr[];
extern const USB_STRING_LANGUAGE_DESCRIPTOR_T      g_UPCC_LanguageStr[];
extern const UPCC_STRING_SERIALNUMBER_DESCRIPTOR_T g_UPCC_SerialNumStr[];
extern const UPCC_STRING_MANUFACTURER_DESCRIPTOR_T g_UPCC_StrManufacturer[];
extern const UPCC_STRING_PRODUCT_DESCRIPTOR_T      g_UPCC_StrProduct[];
extern SCI_SEMAPHORE_PTR g_UPCC_jpeg_buf_semaphore;
extern BLOCK_ID s_upcc_task_id;

extern BOOLEAN g_UPCC_is_camera_opened;
extern uint32 g_upcc_setcur_flag;
extern uint32 upcc_interface;
/**---------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                  *
 **---------------------------------------------------------------------------*/
LOCAL SCI_EVENT_GROUP_PTR s_upcc_event_ptr = PNULL;

/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/
SCI_SEMAPHORE_PTR g_UPCC_capture_semaphore = NULL;
SCI_SEMAPHORE_PTR g_UPCC_setcur_semaphore;
LOCAL volatile uint8 g_fea_cur_array[UPCC_FEATURE_NUM];
LOCAL volatile uint8 g_fea_max_array[UPCC_FEATURE_NUM];
BOOLEAN g_UPCC_is_video_streaming_allowed_transmit=FALSE;
LOCAL const uint8 halfchr_bitcnt_map[]=
{
	0x00,					// value=0  no bit      = 1
	0x01,					// value=1  bit 0       = 1
	0x01,					// value=2  bit 1       = 1
	0x02,					// value=3  bit 0,1     = 1
	0x01,					// value=4  bit 2       = 1
	0x02,					// value=5  bit 0,2     = 1
	0x02,					// value=6  bit 1,2     = 1
	0x03,					// value=7  bit 0,1,2   = 1
	0x01,					// value=8  bit 3       = 1
	0x02,					// value=9  bit 0,3     = 1
	0x02,					// value=A  bit 1,3     = 1
	0x03,					// value=B  bit 0,1,3   = 1
	0x02,					// value=C  bit 2,3     = 1
	0x03,					// value=D  bit 0,2,3   = 1
	0x03,					// value=E  bit 1,2,3   = 1
	0x04,					// value=F  bit 0,1,2,3 = 1
};

__align(4) LOCAL __packed    const uint8 g_UPCC_SI_VideoProbe[]= /*lint !e140 */
{
	0x00,0x00,0x01,0x01,0x20,0xA1,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x04,0x00,0x00
};

__align(4) LOCAL  __packed  const uint8 g_UPCC_SI_VideoStillProbe[]=  /*lint !e140 */
{
	0x01,0x01,0x01,0x00,0xC0,0x00,0x00,0x00,0x04,0x00,0x00
};

/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/
LOCAL uint32 ComputerCacheNumber(uint32 data_len);


/**---------------------------------------------------------------------------*
 **                        PUBLIC FUNCTION DEFINITION                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Endian swap
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
LOCAL BOOLEAN UPCC_DWordBufEndianSwap (uint32 * src ,uint32 * des ,uint len)
{
#if defined( _UPCC_SC6600I_)
	int i;     
	uint32 *pSrc = src,*pDes = des;  

	for (i=0;i<len ;i++){
	    __asm {
	        ldr      r5,[pSrc]
	        and      r6,r5,#0xff00
	        mov      r6,r6,lsl #8
	        orr      r6,r6,r5,lsl #24
	        orr      r6,r6,r5,lsr #24
	        bic      r5,r5,#0xff000000
	        bic      r5,r5,#0xff00
	        orr      r6,r6,r5,lsr #8
	        str      r6,[pDes]
	    }
	    pSrc++;
	    pDes++;
	}
#endif	
	return TRUE ;
}


/*****************************************************************************/
//  Description:    Function to deal with PU's get info request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetInf(uint8 selector,uint8 length)
{
	char data = 0x07;
	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			break;
		case 0x03:			//PU_CONTRACT_CONTROL
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
	UPCC_EPxSendData (USB2011_EP0_IN,(uint8 *)&data,length);
}

/*****************************************************************************/
//  Description:    Function to deal with PU's get minim request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetMin(uint8 selector,uint8 length)
{
	uint16 data = 0;
	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			break;
		case 0x03:			//PU_CONTRACT_CONTROL
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
#ifdef CHIP_ENDIAN_BIG
	data = ((data>>8)&0x00FF) | ((data<<8)&0xFF00);
#endif
	UPCC_EPxSendData (USB2011_EP0_IN,(uint16 *)&data,length);
}

/*****************************************************************************/
//  Description:    Function to deal with PU's get default request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetDef(uint8 selector,uint8 length)
{
	uint16 data;

	data = g_fea_max_array[selector]/2;
	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			break;
		case 0x03:			//PU_CONTRAST_CONTROL
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
	g_fea_cur_array[selector] = data;
#ifdef CHIP_ENDIAN_BIG
	data = ((data>>8)&0x00FF) | ((data<<8)&0xFF00);
#endif
	UPCC_EPxSendData (USB2011_EP0_IN,(uint16 *)&data,length);
}


/*****************************************************************************/
//  Description:    Function to deal with PU's get length request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetLen(uint8 selector,uint8 length)
{
	uint16 data = 0x01;
	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			break;
		case 0x03:			//PU_CONTRAST_CONTROL
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
#ifdef CHIP_ENDIAN_BIG
	data = ((data>>8)&0x00FF) | ((data<<8)&0xFF00);
#endif
	UPCC_EPxSendData (USB2011_EP0_IN,(uint16 *)&data,length);
}

/*****************************************************************************/
//  Description:    Function to deal with PU's get maxam request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetMax(uint8 selector,uint8 length)
{
	uint32 data = 0x00;
	uint16 word = 0x00;
	uint8  byte = 0x00;
	uint32 i = 0x00;
	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			DCAMERA_GetInfo(DCAMERA_INFO_BRIGHTNESS,&data);
			break;
		case 0x03:			//PU_CONTRAST_CONTROL
			DCAMERA_GetInfo(DCAMERA_INFO_CONTRAST,&data);
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			DCAMERA_GetInfo(DCAMERA_INFO_SATURATION,&data);
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			DCAMERA_GetInfo(DCAMERA_INFO_SHARPNESS,&data);
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			DCAMERA_GetInfo(DCAMERA_INFO_WB_MODE,&data);
			data &= 0xFFFFFFFE;
			for(i=0;i<8;i++)
			{
				byte = data & 0x0000000F;
				data = data >> 4;
				word += halfchr_bitcnt_map[byte];
			}
			data = word;
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
	if(data != 0)
	{
		data = data - 1;		//level calculate from 0;
	}
	g_fea_max_array[selector] = data&0xFF;
	word = (data&0xFFFF);
#ifdef CHIP_ENDIAN_BIG
	word = ((word>>8)&0x00FF) | ((word<<8)&0xFF00);
#endif
	UPCC_EPxSendData (USB2011_EP0_IN,(uint16 *)&word,length);
}

/*****************************************************************************/
//  Description:    Function to deal with PU's get resolution request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetRes(uint8 selector,uint8 length)
{
	uint16 data = 0x01;
	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			break;
		case 0x03:			//PU_CONTRACT_CONTROL
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
#ifdef CHIP_ENDIAN_BIG
	data = ((data>>8)&0x00FF) | ((data<<8)&0xFF00);
#endif
	UPCC_EPxSendData (USB2011_EP0_IN,(uint16 *)&data,length);
}

/*****************************************************************************/
//  Description:    Function to deal with PU's get current request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetCur(uint8 selector,uint8 length)
{
	uint16 data = 0;
	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			break;
		case 0x03:			//PU_CONTRACT_CONTROL
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
	data = g_fea_cur_array[selector];
#ifdef CHIP_ENDIAN_BIG
	data = ((data>>8)&0x00FF) | ((data<<8)&0xFF00);
#endif
	UPCC_EPxSendData (USB2011_EP0_IN,(uint16 *)&data,length);
}

/*****************************************************************************/
//  Description:    Function to deal with stream interface's get current request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_StreamingInterface_GetCur(uint16 selector,uint16 length)
{
	char *pData = 0;
	switch (selector)
	{
		case 0x0100://VS_PROBE_CONTROL
			pData = (char *)&g_UPCC_SI_VideoProbe;
			break;
		case 0x0300://VS_STILL_PROBE_CONTROL
			pData = (char *)&g_UPCC_SI_VideoStillProbe;
			break;
		case 0x0500://VS_STILL_IMAGE_TRIGGER_CONTROL
			switch(length)
			{
				case 0x0000:	//Normal operation
					break;
				case 0x0100:	//Transmit still image
					break;
				case 0x0200:	//Transmit still image via dedicate bulk pipe -- Only for Method 3
					break;
				case 0x0300:	//Abort still image transmit
					break;
				default :
					break;
			}
			break;
		default :
			pData = (char *)&g_UPCC_SI_VideoProbe;
		    break;                
	}
	UPCC_EPxSendData (USB2011_EP0_IN,(uint32 *)pData,length);
}

/*****************************************************************************/
//  Description:    Function to deal with stream interface's set current request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_StreamingInterface_SetCur(uint16 selector,uint16 length)
{
	//char *pData = 0;

	switch (selector)
	{
		case 0x0100://VS_PROBE_CONTROL
			break;
		case 0x0300://VS_STILL_PROBE_CONTROL
			break;
		case 0x0500://VS_STILL_IMAGE_TRIGGER_CONTROL
			switch(length)
			{
				case 0x0000:	//Normal operation
					break;
				case 0x0001:	//Transmit still image
					if(NULL != g_UPCC_capture_semaphore)
					{
						SCI_PutSemaphore(g_UPCC_capture_semaphore);
					}
					//UPCC_SetStillImageFlag();
					break;
				case 0x0002:	//Transmit still image via dedicate bulk pipe -- Only for Method 3
					break;
				case 0x0003:	//Abort still image transmit
					break;
				default :
					break;
			}
			break;
		default :
		    break;                
	}
	return;
}

/*****************************************************************************/
//  Description:    Function to deal with get device descriptor request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/

PUBLIC void UPCC_GetDevDescriptor (uint8 bDirection,uint16 wValue,\
                            uint16 wIndex ,uint16 wLen)
{
	USB_HWORD_VALUE_U value;
	uint16 length = 0;
	char *pData = 0;

	value.wValue  = wValue;
	switch (value.bmDescriptor.type){
 	
 	case USB_DEVICE_DESCRIPTOR_TYPE:
		if (wLen >= UPCC_DEVICE_DESC_MAX_LEN){
			length = UPCC_DEVICE_DESC_MIN_LEN;			
		}
		else{
            length = wLen;
        }
        pData 	= (char *)&g_UPCC_DeviceDescr;
        break;
    
    case USB_CONFIGURATION_DESCRIPTOR_TYPE:  	
		if (wLen >= UPCC_CONFIG_DESC_MAX_LEN){
			length  = 0xE7;	
		}
		else{
		    length  = wLen;
		}
        pData   = (char *)&g_UPCC_ConfigDescr;     
        break;
    
    case USB_STRING_DESCRIPTOR_TYPE: 
        switch (value.bmDescriptor.index)
		{
            //USB 2.0 spec chapter 9.6.7 
            case USB_STR_INDEX_LANGUAGE :
                if (wLen > sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T))
				{
                    length  = sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T);        
                }
                else
				{
                    length  = wLen;
                }
                pData   = (char *)&g_UPCC_LanguageStr;
                break;
            //USB 2.0 spec chapter 9.6.1
            case USB_STR_INDEX_SERIALNUMBER:
                if (wLen > sizeof(UPCC_STRING_SERIALNUMBER_DESCRIPTOR_T))
				{
                    length  = sizeof(UPCC_STRING_SERIALNUMBER_DESCRIPTOR_T);        
                }
                else
				{
                    length  = wLen;
                }
                pData   = (char *)&g_UPCC_SerialNumStr;
                break;
    
            case USB_STR_INDEX_MANUFACTURER:
                if (wLen > sizeof(UPCC_STRING_MANUFACTURER_DESCRIPTOR_T))
				{
                    length  = sizeof(UPCC_STRING_MANUFACTURER_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }
                pData   = (char *)&g_UPCC_StrManufacturer;
                break;

            case USB_STR_INDEX_PRODUCT:
                if (wLen > sizeof(UPCC_STRING_PRODUCT_DESCRIPTOR_T))
				{
                    length  = sizeof(UPCC_STRING_PRODUCT_DESCRIPTOR_T);        
                }
                else
				{
                    length  = wLen;
                }
                pData   = (char *)&g_UPCC_StrProduct;
                break;
            default:
				pData = NULL;
				length = 0;
                break;
        }       
        break;
    default:
        break;
	}
	
	//Write data to EP in & send data to host  ;
    //g_upcc_senddata_ptr = pData;
    //g_upcc_total_length = length;
    //g_upcc_send_packet = 1;
    //g_upcc_total_packet = (length+USB_FIFO_MAX_BYTE-1)/USB_FIFO_MAX_BYTE;

   	UPCC_EPxSendData (USB2011_EP0_IN,(uint32 *)pData,length);
}

/*****************************************************************************/
//  Description:    Callback of PU's set current request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
LOCAL uint32 UPCC_PU_SetCur_CALLBACK(void* param_ptr, uint32 param_size)
{
	if(NULL != g_UPCC_setcur_semaphore)
	{
		SCI_PutSemaphore(g_UPCC_setcur_semaphore);
	}

	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    Function to deal with PU's set current request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_SetCur(uint8 selector, uint8 length)
{
	//uint32 data = 0x00;

	if(length != g_fea_cur_array[selector])
	{
		return;
	}

	switch (selector)
	{
		case 0x00:			//PU_CONTROL_UNDEFINED
			break;
		case 0x01:			//PU_BACKLIGHT_COMPENSATION_CONTROL
			break;
		case 0x02:			//PU_BRIGHTNESS_CONTROL
			if(TRUE==g_UPCC_is_camera_opened)
			{
			     DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS,length,PNULL);
			}
			break;
		case 0x03:			//PU_CONTRAST_CONTROL
			if(TRUE==g_UPCC_is_camera_opened)
			{
			    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST,length,PNULL);
			}
			break;
		case 0x04:			//PU_GAIN_CONTROL
			break;
		case 0x05:			//PU_POWER_LINE_FREQUENCY_CONTROL
			break;
		case 0x06:			//PU_HUE_CONTROL
			break;
		case 0x07:			//PU_SATURATION_CONTROL
			if(TRUE==g_UPCC_is_camera_opened)
			{
			    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_SATURATION,length,PNULL);
		  }
			break;
		case 0x08:			//PU_SHARPNESS_CONTROL
			if(TRUE==g_UPCC_is_camera_opened)
			{	
			    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_SHARPNESS,length,PNULL);
			}
			break;
		case 0x09:			//PU_GAMMA_CONTROL
			break;
		case 0x0A:			//PU_WHITE_BALANCE_TEMPERATURE_CONTROL
			if(TRUE==g_UPCC_is_camera_opened)
			{	
			    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE,length,PNULL);
			}
			break;
		case 0x0B:			//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL
			if(length != 0)
			{
				if(TRUE==g_UPCC_is_camera_opened)
				{
				    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_AWB_CAL_ENABLE,length,PNULL);
			  }
			}
			break;
		case 0x0C:			//PU_WHITE_BALANCE_COMPONENT_CONTROL
			break;
		case 0x0D:			//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL
			break;
		case 0x0E:			//PU_DIGITAL_MULTIPLER_CONTROL
			break;
		case 0x0F:			//PU_DIGITAL_MULTIPLER_LIMIT_CONTROL
			break;
		case 0x10:			//PU_HUE_AUTO_CONTROL
			break;
		default :
		    break;                
	}
}

/*****************************************************************************/
//  Description:    Function to deal with dc's feature request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_Fea_Control(uint8 request, uint8 selector, uint8 length)
{
	if(request == 0x01)												//set op
	{
		g_fea_cur_array[selector] = length;							// update the data array.
	}
	UPCC_SendCommand(request,selector,length);
}

/*****************************************************************************/
//  Description:    Call this function to send data to special end point.
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           pBuf  must be aligned  4  ;
//                  len   must be less than 64 ;
//                  ep_id must be less than 6  ;
/*****************************************************************************/
PUBLIC int UPCC_EPxSendData (uint8 ep_id ,void * pBuf,uint8 len)
{

    UsbDevice_Ctrl_Transfer(pBuf,len);
    return len ;
}


/*****************************************************************************/
//  Description:    Reset upcc's cache
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_ResetCache (void)
{
    g_UPCC_manager.cache.num   = NULL;
    g_UPCC_manager.cache.lock  = FALSE;
	g_UPCC_manager.data_out_len = NULL;
	g_UPCC_manager.cache.cur_index = NULL;

    return TRUE;
}

/*****************************************************************************/
//  Description:    set the lock flag of cache 
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_SetCacheLockFlag(BOOLEAN flag)
{
	uint32 ret;
	uint32 actual_event = 0;
	
    g_UPCC_manager.cache.lock = flag;

    if(NULL == s_upcc_event_ptr)
    {
    	s_upcc_event_ptr = SCI_CreateEvent("UPCC_EVENT");    	
    }
	
    if(flag)
    {
        if(SCI_InThreadContext())
		{
			ret = SCI_GetEvent(
							s_upcc_event_ptr,
							UPCC_EVENT_CACHE_LOCK,
							SCI_OR_CLEAR,
							&actual_event,
							UPCC_EVENT_TIMER_OUT
							);

			if(SCI_SUCCESS != ret)
			{
				//SCI_TRACE_LOW:"UPCC_SetCacheLockFlag: Get event Timer out!!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_DRV_873_112_2_18_1_55_20_36,(uint8*)"");
			}
       	}
			
    }
    else
    {  
        SCI_SetEvent( s_upcc_event_ptr, UPCC_EVENT_CACHE_LOCK, SCI_OR);    
    }
}

/*****************************************************************************/
//  Description:    get the lock flag of cache 
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_GetCacheLockFlag(void)
{
    return g_UPCC_manager.cache.lock;
}

/*****************************************************************************/
//  Description: computer the size of transfer data
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
/*****************************************************************************/
//   frame transfer structure, block size = 1024 bytes(UPCC_PC_RECEIVE_BLOCK_SIZE)
//    ----------------------------------------------------
//	 |   Frame Len(4bytes)   |    Frame Data(1020bytes)   |
//    ----------------------------------------------------
//    ----------------------------------------------------
//	 |                Frame Data(1024bytes)               |
//    ----------------------------------------------------         
//                          ......
//    ----------------------------------------------------
//	 |                Frame Data(1024bytes)               |
//    ----------------------------------------------------  
//    ----------------------------------------------------
//	 | Frame Data(n bytes) | Redundant Data(1024-n bytes) |
//    ----------------------------------------------------  

/*****************************************************************************/
LOCAL uint32 ComputerCacheNumber(uint32 data_len)
{
	uint32 cache_len = 0;
	
	//UPCC_ASSERT(NULL != data_len);  /*assert verified*/ //had check the parameter before call this function
	
	if(data_len <= (UPCC_PC_RECEIVE_BLOCK_SIZE - UPCC_FRAME_LEN))
	{
	    //数据不满一块大小时，补足冗余数据至一块
		cache_len = UPCC_PC_RECEIVE_BLOCK_SIZE;
	}
	else
	{
		if(0 == ((data_len - (UPCC_PC_RECEIVE_BLOCK_SIZE - UPCC_FRAME_LEN)) \
				 % UPCC_PC_RECEIVE_BLOCK_SIZE))
		{
		    //数据大小正好是块大小整数倍，不用补足冗余数据
		    cache_len = data_len + UPCC_FRAME_LEN;
		}
		else
		{
		    //数据大小不是块大小整数倍，且大于一块，最后多余数据补足一块
			cache_len = UPCC_PC_RECEIVE_BLOCK_SIZE + \
						(((data_len - (UPCC_PC_RECEIVE_BLOCK_SIZE - UPCC_FRAME_LEN)) \
						 / UPCC_PC_RECEIVE_BLOCK_SIZE) + 1) * UPCC_PC_RECEIVE_BLOCK_SIZE;
		}
	}
	
	UPCC_ASSERT(NULL != cache_len);   /*assert to do*/ //I think this situation will not happen 
	
	return cache_len;
}

/*****************************************************************************/
//  Description: make the JPEG data the store in cache
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_MakeCacheData(UPCC_JPG_FRAME_T *p_frame)
{
	uint8		*buf_ptr = PNULL;
	uint8		*buf_inc_ptr = PNULL;
	uint32		buf_len = 0;
    uint8      *buf_head_ptr = PNULL;
    
    if(PNULL == p_frame)
    {
        return FALSE;
    }
    
    buf_len = p_frame->len;

	if(0 == buf_len)
    {
        return FALSE;
	}
	
	if(UPCC_GetCacheLockFlag())
	{
	    //the cache is locked because DMA transfer not complete 
	    //SCI_TRACE_LOW:"UPCC_drv.c:UPCC_MakeCacheData:the cache is locked."
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_DRV_972_112_2_18_1_55_20_37,(uint8*)"");
	    return FALSE;
	}
	
	//reset cache
	UPCC_ResetCache();
	
		
	if(buf_len <= UPCC_JPEG_BUF_MAX_SIZE)
	{
		//computer the cache data size
		g_UPCC_manager.cache.num = ComputerCacheNumber(buf_len);
		
		UPCC_ASSERT((0 != g_UPCC_manager.cache.num) && (g_UPCC_manager.cache.num <= UPCC_MAX_TRANSFER_LEN));  /*assert verified*/ 
		
		buf_ptr = (uint8*)g_UPCC_manager.cache.buf + 12;
		
		//merge the 1 part of one JPEG
		buf_inc_ptr = buf_ptr;

		SCI_MEMCPY(buf_inc_ptr, p_frame->p_head, p_frame->len);
		buf_inc_ptr += p_frame->len;

	    //set the data len
        buf_head_ptr = (uint8*)g_UPCC_manager.cache.buf;
        buf_head_ptr[0] = 0x0c;

	}
	else
	{
		UPCC_ASSERT(FALSE);   /*assert verified*/
	}
	
	return TRUE;
}

/*****************************************************************************/
//  Description: make the JPEG data the store in cache
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL BOOLEAN UPCC_MakeData(UPCC_JPG_FRAME_T *p_frame, int index, uint32 time, uint8 flag, int stuff_num)
{
    uint8  *buf_ptr      = PNULL;
    uint8  *buf_inc_ptr  = PNULL;
    uint8  *buf_head_ptr = PNULL;
    uint32 current_time  = SCI_GetTickCount();  
    uint16 sof           = (uint16)(*(volatile uint32*)(0x20300024));
    uint8* temp_buf_ptr  = (uint8*)(p_frame->p_head+index*VALID_PAYLOAD_LEN);
	uint8* frame_buf     = PNULL;
    
    //g_UPCC_manager.cache.buf = &g_UPCC_manager.cache.align_buf[g_UPCC_manager.cache.cur_index];
    frame_buf = &g_UPCC_manager.cache.align_buf[g_UPCC_manager.cache.cur_index];  /*lint !e605 */
    
    if((g_UPCC_manager.cache.cur_index + UPCC_PC_RECEIVE_BLOCK_SIZE) < (UPCC_CACHE_SIZE-UPCC_PC_RECEIVE_BLOCK_SIZE))
    {
    	g_UPCC_manager.cache.cur_index += UPCC_PC_RECEIVE_BLOCK_SIZE;
    }
    else
    {
    	g_UPCC_manager.cache.cur_index = 0;
    }

	if(g_UPCC_manager.cache.num == 0)
	{
		g_UPCC_manager.cache.buf = frame_buf;
	}
	
    //computer the cache data size
    g_UPCC_manager.cache.num += UPCC_PC_RECEIVE_BLOCK_SIZE;

	
    
    buf_ptr = (uint8*)frame_buf + UPCC_FRAME_LEN;
    
    //merge the 1 part of one JPEG
    buf_inc_ptr = buf_ptr;
    
    SCI_MEMCPY(buf_inc_ptr, temp_buf_ptr, VALID_PAYLOAD_LEN);
    
    if((flag&0x2)&&stuff_num)
    {    	
    	SCI_MEMSET(buf_inc_ptr+VALID_PAYLOAD_LEN-stuff_num,0xff,stuff_num);/*lint !e516 -e516*/
    }  	
    //set the data len
    //buf_head_ptr = (uint8*)g_UPCC_manager.cache.buf;
    buf_head_ptr = (uint8*)frame_buf;
    buf_head_ptr[0] = 0x0C;
    buf_head_ptr[1] = flag;
    buf_head_ptr[2] = time&0xff;
    buf_head_ptr[3] = (time>>8)&0xff;
    buf_head_ptr[4] = (time>>16)&0xff;
    buf_head_ptr[5] = (time>>24)&0xff;
    buf_head_ptr[6] = current_time&0xff;
    buf_head_ptr[7] = (current_time>>8)&0xff;
    buf_head_ptr[8] = (current_time>>16)&0xff;
    buf_head_ptr[9] = (current_time>>24)&0xff;
    buf_head_ptr[10] = (sof)&0xff;
    buf_head_ptr[11] = (sof>>8)&0xff;

    return TRUE;
}


/*****************************************************************************/
//  Description: send the cache data           
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SendCache(void)
{
    uint32 num   = g_UPCC_manager.cache.num ;
	uint32 * buf = (uint32 *)g_UPCC_manager.cache.buf;
	
    UPCC_ASSERT(num <= UPCC_MAX_TRANSFER_LEN);     /*assert verified*/

	if(num > UPCC_MAX_TRANSFER_LEN)
	{
		return SCI_FALSE;
	}
    
    if (!UDC_GetConnect())
    {
    	//SCI_TRACE_LOW:"UPCC_drv.c:UPCC_SendCache:fail."
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_DRV_1093_112_2_18_1_55_21_38,(uint8*)"");
		return SCI_FALSE;
	}

	//UPCC_TRACE_LOW:"UPCC_SendCache: size = 0x%x, buf_addr = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_DRV_1097_112_2_18_1_55_21_39,(uint8*)"dd", num, (uint32)buf);
	
	UPCC_SetCacheLockFlag(SCI_TRUE);

    /*for 8800s little endian, should do swap by software*/
#ifdef CHIP_ENDIAN_LITTLE
    USB_FIFODWordBufEndianSwap((uint32*)g_UPCC_manager.cache.buf,
                            (uint32*)g_UPCC_manager.cache.buf,
                            ((g_UPCC_manager.cache.num + 3) >>2) );
#endif

	UsbDevice_Start_send(upcc_interface,buf,num);

    return TRUE ;
}


/*****************************************************************************/
//  Description: get a frame data and send it to pc
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_SendAFrame(UPCC_JPG_FRAME_T *p_frame)
{
    int data_len = p_frame->len;
    int tran_num = (data_len+VALID_PAYLOAD_LEN -1)/VALID_PAYLOAD_LEN;
    int need_stuff = data_len%VALID_PAYLOAD_LEN;
    int i = 0;
   // int tran_len = 0;
    static uint8 toggle = 0xD;
    uint8 flag = 0;
    uint32 time = SCI_GetTickCount();
    toggle^=0x1;
    need_stuff = VALID_PAYLOAD_LEN - need_stuff;

	if(UPCC_GetCacheLockFlag())
	{
	    //the cache is locked because DMA transfer not complete 
	    SCI_PutSemaphore(g_UPCC_jpeg_buf_semaphore);
	    //SCI_TRACE_LOW:"UPCC_drv.c:UPCC_MakeCacheData:the cache is locked."
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_DRV_1135_112_2_18_1_55_21_40,(uint8*)"");
	    return;
	}
	
	UPCC_ResetCache();

	flag = toggle;

	if(NULL == g_UPCC_capture_semaphore)
	{
		return;
	}

	if(SCI_GetSemaphore(g_UPCC_capture_semaphore,SCI_NO_WAIT) == SCI_SUCCESS)
	{
		flag |= 0x20;
	}
	else
	{
	
	}

    for(i=0;i<tran_num;i++)
    {
    	if((i+1)==tran_num)
        {
    	    flag |= 0x2;
    	}
    	else
        {
        
    	}
    
    	UPCC_MakeData(p_frame,i,time,flag,need_stuff);		
    }
    
	if(NULL == g_UPCC_jpeg_buf_semaphore)
	{
		return;
	}

	SCI_PutSemaphore(g_UPCC_jpeg_buf_semaphore);

    if (!UPCC_SendCache())
    {
        //SCI_TRACE_LOW:"UPCC_drv.c:UPCC_SendAFrame:send cache fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_DRV_1180_112_2_18_1_55_21_41,(uint8*)"");
    }
    
}

/*****************************************************************************/
//  Description:    Initial the feature value array
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_InitFeaValArray()
{
	int i=0;

	for(i=0;i<UPCC_FEATURE_NUM;i++)
	{
		g_fea_cur_array[i] = 0x00;
		g_fea_max_array[i] = 0x00;
	}
}

/*****************************************************************************/
//  Description:    Function is used to send command to upcc's drv thread
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/ 
PUBLIC BOOLEAN UPCC_SendCommand (uint8 request,uint8 command,uint8 param)
{
	UPCC_SIGNAL_T * 	sig_ptr = NULL;
	BLOCK_ID            sender = SCI_IdentifyThread();

	sig_ptr = (UPCC_SIGNAL_T *)SCI_ALLOC_APP(sizeof(UPCC_SIGNAL_T));
	SCI_ASSERT(SCI_NULL != sig_ptr); /*assert verified*/
	sig_ptr->sig.SignalSize = sizeof(UPCC_SIGNAL_T);
	sig_ptr->sig.Sender     = sender;
//	sig_ptr->sig.SignalCode = receiver & 0x0FFFFUL;
	sig_ptr->request		= request;
	sig_ptr->command        = command ;
	sig_ptr->param          = param ;
	if(SET_CUR == sig_ptr->request)
	{ 
        if(FALSE == g_upcc_setcur_flag)
        {
		    if (SCI_SUCCESS == SCI_SendSignal((xSignalHeader)sig_ptr, s_upcc_task_id))
		    {
			    return TRUE;
		    }
		    else
		    {
			    return SCI_FALSE;
		    }
        }
		else
		{
		    return SCI_FALSE;
		}
	 }
	 else 
	 {
         if (SCI_SUCCESS == SCI_SendSignal((xSignalHeader)sig_ptr, s_upcc_task_id))
		 {
			return TRUE;
		 }
		 else
		 {
			return SCI_FALSE;
		 }
	 }
}

/**---------------------------------------------------------------------------*/
#endif //_UPCC_DRV_C_
