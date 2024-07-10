/******************************************************************************
 ** File Name:    ubot_cus_config.c                                           *
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
#include "ubot_config.h"
#include "usb_drv.h"
#include "bsd.h"
#include "xsr_partition.h"
#include "sc_reg.h"
#include "usb200.h"
#include "ubot_cfg_info.h"
#include "usbservice_api.h"

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
#define	EP0_MAX_PACKET_SIZE	64
#define	EPO_MAX_PACKET_SIZE	512
#define	EPI_MAX_PACKET_SIZE	512
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*lint --e{728} */
__align(4) LOCAL const USB_DEVICE_DESCRIPTOR_T s_UBOT_DeviceDescr =
{
    sizeof(USB_DEVICE_DESCRIPTOR_T),
    USB_DEVICE_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(0x0200),
    UBOT_CLASS_IN_INTERFACE,
    UBOT_SUBCLASS_IN_INTERFACE,
    UBOT_PROTOCOL_IN_INTERFACE,
    EP0_MAX_PACKET_SIZE,
    USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_VENDER_ID),   // SPREADTRUM VID
    USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_MASS_STORAGE_ID),
    USB_WORD_SWAP_ENDIAN(UBOT_DEVICE_RELEASE_NUM),/*lint !e572 !e778 */
    USB_STR_INDEX_MANUFACTURER,
    USB_STR_INDEX_PRODUCT,
    USB_STR_INDEX_SERIALNUMBER,
    UBOT_CONFIGRATION_NUM
};

__align(4) LOCAL const UBOT_CONFIG_DESC_T s_UBOT_ConfigDescr =
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UBOT_CONFIG_DESCCIPTOR_LEN),/*lint !e572 !e778 */
    UBOT_INTERFACE_NUM,             // only one interface
    1,                              // argument
    USB_STR_INDEX_LANGUAGE,
    0x80,                           // Self-powered ;

    UBOT_MAX_POWER,
    // Following is InterfaceDescr
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    NULL,
    NULL,
    0x02,
    USB_CLASS_CODE_MASSSTORAGE_CLASS_DEVICE,
    USB_SUBCLASS_CODE_SCSI,
    USB_PROTOCOL_CODE_BULK,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x81,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPI_MAX_PACKET_SIZE),/*lint !e572 !e778 */
    NULL,

    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x02,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPO_MAX_PACKET_SIZE),/*lint !e572 !e778 */
    NULL
};

__align(4) LOCAL const UBOT_CONFIG_DESC_T s_UBOT20_ConfigDescr =
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UBOT_CONFIG_DESCCIPTOR_LEN),/*lint !e572 !e778 */
    UBOT_INTERFACE_NUM,             // only one interface
    1,                              // argument
    USB_STR_INDEX_LANGUAGE,
    0x80,                           // Self-powered ;

    UBOT_MAX_POWER,
    // Following is InterfaceDescr
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    NULL,
    NULL,
    0x02,
    USB_CLASS_CODE_MASSSTORAGE_CLASS_DEVICE,
    USB_SUBCLASS_CODE_SCSI,
    USB_PROTOCOL_CODE_BULK,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x81,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(512),/*lint !e778 */
    NULL,

    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x02,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(512),/*lint !e778 */
    NULL
};

__align(4) LOCAL const USB_STRING_LANGUAGE_DESCRIPTOR_T s_UBOT_LanguageStr []={
    sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN (USB_LANGUAGE_ENGLISH)
};

__align(4) LOCAL const USB_STRING_MANUFACTURER_DESCRIPTOR_T  s_UBOT_StrManufacturer[] =
{
    sizeof(USB_STRING_MANUFACTURER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    {/*lint !e651*/
    'S', 0,'p', 0,'r', 0,'e', 0,'a', 0,'d', 0,'t', 0,
    'r', 0,'u', 0,'m', 0,' ', 0,' ', 0}
};

__align(4) LOCAL const USB_STRING_PRODUCT_DESCRIPTOR_T  s_UBOT_StrProduct[] =
{
    sizeof(USB_STRING_PRODUCT_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    {/*lint !e651*/
    'G', 0,'e', 0,'n', 0,'e', 0,'r', 0,'i', 0,'c', 0,' ', 0,
    'D', 0,'i', 0,'s', 0,'k', 0,'1', 0,'.', 0,'0', 0}
};

__align(4) LOCAL USB_STRING_SERIALNUMBER_DESCRIPTOR_T s_UBOT_SerialNumStr []={
    sizeof(USB_STRING_SERIALNUMBER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    //UNICODE character string;
    { 'S',0,'p',0,'r',0,'e',0,'a',0,'d',0,'t',0,'r',0,/*lint !e651*/
      'u',0,'m',0,'_',0,'M',0
    }
    //It must be modified in initialization.
};
#ifdef WINUSB_SUPPORT

__align(4) LOCAL const char s_UBOT_StrManufacturerForWinUsb[] = 
{
	0x0C,
	USB_STRING_DESCRIPTOR_TYPE,
	'N', 0,'o', 0,'k', 0,'i', 0,'a', 0
};


// power off charging mode product string
LOCAL uint8 s_poweroff_product_str[] = "Unisoc-Poweroff Charging & Mass Storage";

__align(4) LOCAL char s_UBOT_StrProductForWinUsb[UBOT_DEV_STRING_DESC_LEN]; 

__align(4) LOCAL char s_UBOT_SerialNumForWinUsb[UBOT_DEV_STRING_DESC_LEN];

#endif

/* 	
**	adpter the MSC TEST TOOL begin, by Ankur Trivedi
*/
__align(4) __packed const uint8  s_UBOT_Device_Qualifier_Descr[] =
{
    0x0A,0x06,
    0x00,0x02,
    0x02,0x00,
    0x00,0x40,
    0x01,0x00
};

__align(4) __packed const uint8  s_UBOT_String_Index_Config[] =
{
    0x34,0x03,    
    0x53,0x00,
    0x61,0x00,
    0x6D,0x00,
    0x73,0x00,
    0x75,0x00,
    0x6E,0x00,
    0x67,0x00,
    0x20,0x00,
    0x50,0x00,
    0x69,0x00,
    0x63,0x00,
    0x74,0x00,
    0x42,0x00,
    0x72,0x00,
    0x69,0x00,
    0x64,0x00,
    0x67,0x00,
    0x65,0x00,
    0x20,0x00,
    0x44,0x00,
    0x65,0x00,
    0x76,0x00,
    0x69,0x00,
    0x6E,0x00,
    0x63,0x00,
    0x65,0x00
};

__align(4) __packed const uint8  s_UBOT_String_Interface_Config[] =
{
    0x28,0x03,    
    0x4d,0x00,
    0x61,0x00,
    0x73,0x00,
    0x73,0x00,
    0x20,0x00,
    0x53,0x00,
    0x74,0x00,
    0x6f,0x00,
    0x72,0x00,
    0x61,0x00,
    0x67,0x00,
    0x65,0x00,
    0x20,0x00,
    0x44,0x00,
    0x65,0x00,
    0x76,0x00,
    0x69,0x00,
    0x63,0x00,
    0x65,0x00
};

__align(4) LOCAL uint8 s_UBOT_VenderStr  [UBOT_SCSI_VENDER_STR_LEN]={
    "Generic"
};


__align(4) LOCAL uint8 s_UBOT_ProductIDStrNand\
                                         [UBOT_SCSI_PRODUCT_ID_STR_LEN]={
    "LOCAL DISK        "
};
__align(4) LOCAL uint8 s_UBOT_ProductIDStrSD\
                                         [UBOT_SCSI_PRODUCT_ID_STR_LEN]={
    "SDCard DISK       "
};

__align(4) LOCAL uint8 s_UBOT_ProductIDStrSD_1\
                                         [UBOT_SCSI_PRODUCT_ID_STR_LEN]={
    "SDCard_1 DISK    "
};

__align(4) LOCAL uint8 s_UBOT_ProductIDStrHiddenPartition\
                                         [UBOT_SCSI_PRODUCT_ID_STR_LEN]={
    "System DISK       "
};


LOCAL UBOT_DEVICE_INF_T s_UBOT_DiskInf [] = {
    //NAND Disk
    {
        (uint8*)STL_UDISK_FS_PART,
        s_UBOT_VenderStr,
        s_UBOT_ProductIDStrNand,
        {0,0,0},/*lint !e651*/
        NULL,
        512,
        NULL,
#ifdef UDISK_PC_CONNECT
        300   /*valid for usb nor disk only*/
#else
        NULL   /*not used in nandflash platform*/
#endif
    },

    //SDCard Disk
    {
        (uint8*)STL_SD_DEVICE_NAME,   //UBOT will base on this id to call BSD ;
        s_UBOT_VenderStr,
        s_UBOT_ProductIDStrSD,
        {0,0,0},
        NULL,
        NULL,
        NULL,
        NULL
    },

    //SDCard_1 Disk
    {
        (uint8*)STL_SD_1_DEVICE_NAME,   //UBOT will base on this id to call BSD ;
        s_UBOT_VenderStr,
        s_UBOT_ProductIDStrSD_1,
        {0,0,0},
        NULL,
        NULL,
        NULL,
        NULL
    },
    //Hidden Partition
    {
        (uint8*)STL_UDISK_HIDDEN_FS_PART,   //UBOT will base on this id to call BSD ;
        s_UBOT_VenderStr,
        s_UBOT_ProductIDStrHiddenPartition,
        {0x0, 0x1, 0x0},    //bHidden:TRUE, bState:FALSE
        NULL,
        NULL,
        NULL,
        NULL
    },
};


LOCAL  UBOT_CFG_INFO_T s_ubot_cfg_info =
{
	(PUSB_DEVICE_DESCRIPTOR_T)				&s_UBOT_DeviceDescr,
	(PUBOT_CONFIG_DESC_T)					&s_UBOT_ConfigDescr,
	(PUSB_STRING_LANGUAGE_DESCRIPTOR_T)		&s_UBOT_LanguageStr,
	(PUSB_STRING_MANUFACTURER_DESCRIPTOR_T)	&s_UBOT_StrManufacturer,
	(PUSB_STRING_PRODUCT_DESCRIPTOR_T)		&s_UBOT_StrProduct,
	(PUSB_STRING_SERIALNUMBER_DESCRIPTOR_T)	&s_UBOT_SerialNumStr,

	 sizeof(s_UBOT_DiskInf)/sizeof(UBOT_DEVICE_INF_T),
	(PUBOT_DEVICE_INF_T)					&s_UBOT_DiskInf
};

#ifdef WINUSB_SUPPORT
/*****************************************************************************/
//	String Descriptor Array
//	0	:for USB_STR_INDEX_LANGUAGE
//	1	:for USB_STR_INDEX_MANUFACTURER
//	2	:for USB_STR_INDEX_PRODUCT
//	3	:for USB_STR_INDEX_SERIALNUMBER
//	4 	:for USB_STR_INDEX_CONFIGURATION
//	5	:for USB_STR_INDEX_INTERFACE
/*****************************************************************************/
LOCAL UBOT_STR_DESC_T s_ubot_str_info[UBOT_DEV_STRING_INDEX];
#endif
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Get configure information
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC PUBOT_CFG_INFO_T UBOT_GetCfgInfo(void)
{
	return (PUBOT_CFG_INFO_T)&s_ubot_cfg_info;
}
/*****************************************************************************/
//  Description:    Get configure information
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void UBOT_SetCfgDescriptor(uint8 IsFS)
{
	if(!IsFS)
	{
		if(s_ubot_cfg_info.pubot_cfg_desc!= &s_UBOT20_ConfigDescr)
		{
			s_ubot_cfg_info.pubot_cfg_desc = (UBOT_CONFIG_DESC_T*)&s_UBOT20_ConfigDescr;
		}
	}
	else
	{
		if(s_ubot_cfg_info.pubot_cfg_desc != &s_UBOT_ConfigDescr)
		{
			s_ubot_cfg_info.pubot_cfg_desc = (UBOT_CONFIG_DESC_T*)&s_UBOT_ConfigDescr;
		}
	}
}
PUBLIC uint32 UBOT_GetCOnfigDescriptor(uint8 ** para)
{
	uint32 IsFS=0;

	IsFS =(uint32) *para;
	if(IsFS)
	{
		*para = (uint8*)&s_UBOT_ConfigDescr;
	}
	else
	{
		*para = (uint8*)&s_UBOT20_ConfigDescr;
	}
	UBOT_SetCfgDescriptor(IsFS);
	return sizeof(s_UBOT_ConfigDescr);
}
#ifdef WINUSB_SUPPORT
PUBLIC PUBOT_STR_DESC_T UBOT_GetStrInfo(void)
{
	return (PUBOT_STR_DESC_T)s_ubot_str_info;
}

LOCAL uint32 UBOT_StringConfig(uint8 *pDest, const uint8 *pSrc)
{
	uint32 i = 0;
	uint32 len = strlen(pSrc);
	
	pDest[i++] = len * 2 + 2;
	pDest[i++] = USB_STRING_DESCRIPTOR_TYPE;

	while(len--)
	{
		pDest[i++] = *pSrc++;
		pDest[i++] = 0;
	}

	return pDest[0];
}

PUBLIC void UBOT_SetStrInfo(void)
{
	PUBOT_STR_DESC_T str = s_ubot_str_info;

	uint8   imeiStr[(MAX_IMEI_LENGTH<<1) + 1] = {0};
	uint8	imei_arr[MAX_IMEI_LENGTH] = {0};
	
	// language id
	str[USB_STR_INDEX_LANGUAGE].type	= USB_STR_INDEX_LANGUAGE;
	str[USB_STR_INDEX_LANGUAGE].len	= sizeof(s_UBOT_LanguageStr)/sizeof(uint8);
	str[USB_STR_INDEX_LANGUAGE].pStr = s_UBOT_LanguageStr;

	// manufacturer 
	str[USB_STR_INDEX_MANUFACTURER].type = USB_STR_INDEX_MANUFACTURER;
	str[USB_STR_INDEX_MANUFACTURER].len	= sizeof(s_UBOT_StrManufacturerForWinUsb)/sizeof(uint8);
	str[USB_STR_INDEX_MANUFACTURER].pStr = s_UBOT_StrManufacturerForWinUsb;

	// product
	usb_set_product_info(s_poweroff_product_str);
	
	str[USB_STR_INDEX_PRODUCT].type	= USB_STR_INDEX_PRODUCT;
	str[USB_STR_INDEX_PRODUCT].len  =  UBOT_StringConfig(s_UBOT_StrProductForWinUsb, usb_get_product_info());
	str[USB_STR_INDEX_PRODUCT].pStr = s_UBOT_StrProductForWinUsb;

	// serial number
	str[USB_STR_INDEX_SERIALNUMBER].type = USB_STR_INDEX_SERIALNUMBER;
	str[USB_STR_INDEX_SERIALNUMBER].len	= UBOT_StringConfig(s_UBOT_SerialNumForWinUsb, usb_get_sn_info());
	str[USB_STR_INDEX_SERIALNUMBER].pStr = s_UBOT_SerialNumForWinUsb;	
	
	// index config
	str[USB_STR_INDEX_CONFIGURATION].type = USB_STR_INDEX_CONFIGURATION;
	str[USB_STR_INDEX_CONFIGURATION].len  = sizeof(s_UBOT_String_Index_Config)/sizeof(uint8);
	str[USB_STR_INDEX_CONFIGURATION].pStr = s_UBOT_String_Index_Config;
	
	// index interface
	str[USB_STR_INDEX_INTERFACE].type = USB_STR_INDEX_INTERFACE;
	str[USB_STR_INDEX_INTERFACE].len  = sizeof(s_UBOT_String_Interface_Config)/sizeof(uint8);
	str[USB_STR_INDEX_INTERFACE].pStr = s_UBOT_String_Interface_Config;

	//end
}
#endif	
/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC void UBOT_SetVenderStr (uint8 *vender_str, uint8 string_length)
{
    if(string_length > UBOT_SCSI_VENDER_STR_LEN)
    {
        string_length = UBOT_SCSI_VENDER_STR_LEN;
    }

    memcpy(s_UBOT_VenderStr, vender_str, MIN(string_length,UBOT_SCSI_VENDER_STR_LEN-1));
    s_UBOT_VenderStr[UBOT_SCSI_VENDER_STR_LEN-1] = NULL;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC uint8* UBOT_GetVenderStr (uint8 *string_length)
{
    *string_length = strlen((char*)s_UBOT_VenderStr);
    return s_UBOT_VenderStr;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC void UBOT_SetProductIDStrNand (uint8 *productid_str, uint8 string_length)
{
    if(string_length > UBOT_SCSI_PRODUCT_ID_STR_LEN)
    {
        string_length = UBOT_SCSI_PRODUCT_ID_STR_LEN;
    }

    memcpy(s_UBOT_ProductIDStrNand, productid_str, MIN(string_length,UBOT_SCSI_PRODUCT_ID_STR_LEN-1));
    s_UBOT_ProductIDStrNand[UBOT_SCSI_PRODUCT_ID_STR_LEN-1] = NULL;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC uint8* UBOT_GetProductIDStrNand (uint8 *string_length)
{
    *string_length = strlen((char*)s_UBOT_ProductIDStrNand);
    return s_UBOT_ProductIDStrNand;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC void UBOT_SetProductIDStrSD (uint8 *productid_str, uint8 string_length)
{
    if(string_length > UBOT_SCSI_PRODUCT_ID_STR_LEN)
    {
        string_length = UBOT_SCSI_PRODUCT_ID_STR_LEN;
    }

    memcpy(s_UBOT_ProductIDStrSD, productid_str, MIN(string_length,UBOT_SCSI_PRODUCT_ID_STR_LEN-1));
    s_UBOT_ProductIDStrSD[UBOT_SCSI_PRODUCT_ID_STR_LEN-1] = NULL;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC uint8* UBOT_GetProductIDStrSD (uint8 *string_length)
{
    *string_length = strlen((char*)s_UBOT_ProductIDStrSD);
    return s_UBOT_ProductIDStrSD;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC void UBOT_SetSerialNumStr(char *serial_num, uint8 string_length)
{
    if(string_length > USB_STR_SERIALNUMBER_DESCRIPTOR_MIN)
    {
        string_length = USB_STR_SERIALNUMBER_DESCRIPTOR_MIN;
    }

    memcpy(s_UBOT_SerialNumStr[0].SerialNum, serial_num,\
            MIN(string_length,USB_STR_SERIALNUMBER_DESCRIPTOR_MIN-1));
    s_UBOT_SerialNumStr[0].SerialNum[USB_STR_SERIALNUMBER_DESCRIPTOR_MIN-1] = NULL;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jing.Li
//	Note:
/*****************************************************************************/
PUBLIC char* UBOT_GetSerialNumStr(uint8 *string_length)
{
    *string_length = strlen(s_UBOT_SerialNumStr[0].SerialNum);
    return s_UBOT_SerialNumStr[0].SerialNum;
}


PUBLIC unsigned char *Ubot_Get_QualifierDesc(void)
{
    return (unsigned char *) s_UBOT_Device_Qualifier_Descr;
}

PUBLIC unsigned char *Ubot_Get_String_Index_Config(void)
{
    return (unsigned char *) s_UBOT_String_Index_Config;
}

PUBLIC unsigned char *Ubot_Get_String_Interface_Desc(void)
{
    return (unsigned char *) s_UBOT_String_Interface_Config;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End
