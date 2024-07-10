/******************************************************************************1
 ** File Name:    UCOM_config.c                                               *
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
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ucom_config.h"
#include "usb_drv.h"
#include "ucom_api.h"
#include "ucom_main.h"
#include "bsd.h"

#undef BCD_USB
#undef EPN_MPS
#undef EP0_MPS
#undef USB_20
#undef USB_11

#define USB_20
#define EP0_MPS 64

#if defined(USB_20)
#define BCD_USB 0x0200
#define EPN_MPS 512
#elif defined(USB_11)
#define BCD_USB 0x0110
#define EPN_MPS 64
#endif

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
__align(4) USB_DEVICE_DESCRIPTOR_T g_ucom_device_desc =
{
    sizeof(USB_DEVICE_DESCRIPTOR_T),
    USB_DEVICE_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(USB_SPECIFICATION_VERSION),  // USB 1.1
    UCOM_CLASS_IN_INTERFACE,
    UCOM_SUBCLASS_IN_INTERFACE,
    UCOM_PROTOCOL_IN_INTERFACE,
    EP0_MPS,
    USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_VENDER_ID),   // SPREADTRUM VID
    USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_VCOM_ID),
    USB_WORD_SWAP_ENDIAN(UCOM_DEVICE_RELEASE_NUM),
    0x00,0x00,0x00,0x01
};

#ifdef WINUSB_SUPPORT
__align(4) const USB_DEVICE_DESCRIPTOR_T g_UCOM_DeviceDescr_threeinterfaceForWinUsb=
{
    sizeof(USB_DEVICE_DESCRIPTOR_T),
    USB_DEVICE_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(USB_SPECIFICATION_VERSION),  // USB 1.1
    UCOM_CLASS_IN_INTERFACE,
    UCOM_SUBCLASS_IN_INTERFACE,
    UCOM_PROTOCOL_IN_INTERFACE,
    EP0_MPS,
    USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_VENDER_ID),   // SPREADTRUM VID
    USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_VCOM_MASS_STORAGE_ID),
    USB_WORD_SWAP_ENDIAN(UCOM_DEVICE_RELEASE_NUM),
    0x01,0x02,0x03,0x01
};
#endif
//in release mode, only one virtual com should be available
__align(4) const UCOM_CONFIG_DESC_T g_UCOM_CfgDescr_AutoDloader =
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UCOM_CFG_DESCR_LEN),
    UCOM_INTERFACE_NUM,		        // only one interface
    1,		                        // argument
    USB_STR_INDEX_LANGUAGE,
    0xC0,                           // Self-powered ;
    UCOM_MAX_POWER,

    // Following is InterfaceDescr
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    NULL,
    NULL,
    UCOM_EPx_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP3 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_EPx_BULK_IN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is EP4 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_EP2_BULK_OUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL

};

//in release mode, only one virtual com should be available
__align(4) const UCOM_CONFIG_DESC_T g_UCOM_CfgDescDebug =
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UCOM_CFG_DESCR_LEN),
    UCOM_INTERFACE_NUM,		        // only one interface
    1,		                        // argument
    USB_STR_INDEX_LANGUAGE,
    0xC0,                           // Self-powered ;
    UCOM_MAX_POWER,

    // Following is InterfaceDescr
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    NULL,
    NULL,
    UCOM_EPx_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP3 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_EPx_BULK_IN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is EP4 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_EPx_BULK_OUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL
};

//in release mode, only one virtual com should be available
__align(4) const UCOM_CONFIG_DESC_T g_UCOM_CfgDescData =
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UCOM_CFG_DESCR_LEN),
    UCOM_INTERFACE_NUM,		        // only one interface
    1,		                        // argument
    USB_STR_INDEX_LANGUAGE,
    0xC0,                           // Self-powered ;
    UCOM_MAX_POWER,

    // Following is InterfaceDescr
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    NULL,
    NULL,
    UCOM_EPx_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_EP1_BULK_IN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_EP2_BULK_OUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL
};
#ifdef WINUSB_SUPPORT
__align(4) const UCOM_CONFIG_DESC_THREEIF_T g_UCOM_CfgDescr_threeinterfaceForWinUsb = 
{
     // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UCOM_CFG_DESCR_LEN_THREEINF),
    UCOM_INTERFACE_THREE,		        // three interface
    1,		                        // argument
    USB_STR_INDEX_LANGUAGE,
    0xC0,                           // Self-powered ;
    UCOM_MAX_POWER,
    // Following is InterfaceDescr one
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x00,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE, 
    
    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE1_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,
    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE1_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    
    // Following is InterfaceDescr two
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x01,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE, 
    
    // Following is EP3 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE2_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,
    // Following is EP4 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE2_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is InterfaceDescr three
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x02,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    USB_CLASS_CODE_MASSSTORAGE_CLASS_DEVICE,
    USB_SUBCLASS_CODE_SCSI,
    USB_PROTOCOL_CODE_BULK,
    USB_STR_INDEX_LANGUAGE, 
    
    // Following is EP5 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE3_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,
    // Following is EP6 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE3_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL

};
#endif
//in Debug mode, two virtual com should be available
__align(4) const UCOM_CONFIG_DESC_TOWIF_T g_UCOM_CfgDescr_twointerface =
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UCOM_CFG_DESCR_LEN_TWOINF),
    UCOM_INTERFACE_TWO,		        // only one interface
    1,		                        // argument
    USB_STR_INDEX_LANGUAGE,
    0xC0,                           // Self-powered ;
    UCOM_MAX_POWER,
    // Following is InterfaceDescr one
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x00,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE1_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE1_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is InterfaceDescr two
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x01,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,
};

#ifdef WINUSB_SUPPORT
__align(4) const uint8 OS_StringDescriptor[]=
{0x12, 0x03, 'M', 0, 'S', 0, 'F', 0, 'T', 0, '1', 0, '0', 0, '0', 0, bMS_VendorCode, 0};

__align(4) const uint8 WINUSB_ExtendedCompatId_Descritpor[] = 
{
    0x40, 0x00, 0x00, 0x00,                         // dwLength  
    0x00, 0x01,                                     // bcdVersion  
    0x04, 0x00,                                     // wIndex  
    0x02,                                           // bCount  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       // Reserved[7]  
    0x00,                                           // bFirstInterfaceNumber  
    0x01,                                           // RESERVED ( 0x01 )  
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,       // compactiableID[8]  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // subCompactiableID[8]  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // Reserved[6]};

    0x01,                                           // bSecondInterfaceNumber
    0x01,                                           // RESERVED ( 0x01 )  
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,       // compactiableID[8]
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // subCompactiableID[8]  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00              // Reserved[6]};

};

__align(4) const uint8 WINUSB_ExtendedProperty_InterfaceGUID_Descritpor0[] =  //AT
{  
    0x8E, 0x00, 0x00, 0x00,     // dwTotalSize = Header + All sections  
    0x00, 0x01,                 // bcdVersion  
    0x05, 0x00,                 // wIndex  
    0x01, 0x00,                 // wCount    
    0x84, 0x00, 0x00, 0x00,     // dwSize -- this section    
    0x01, 0x00, 0x00, 0x00,     // dwPropertyDataType    
    0x28, 0x00,                 // wPropertyNameLength    
    'D', 0x00, 'e', 0x00,       // bProperytName : WCHAR : L"DeviceInterfaceGUID"  
    'v', 0x00, 'i', 0x00,       // bProperytName : WCHAR  
    'c', 0x00, 'e', 0x00,       // bProperytName : WCHAR  
    'I', 0x00, 'n', 0x00,       // bProperytName : WCHAR  
    't', 0x00, 'e', 0x00,       // bProperytName : WCHAR  
    'r', 0x00, 'f', 0x00,       // bProperytName : WCHAR  
    'a', 0x00, 'c', 0x00,       // bProperytName : WCHAR  
    'e', 0x00, 'G', 0x00,       // bProperytName : WCHAR  
    'U', 0x00, 'I', 0x00,       // bProperytName : WCHAR  
    'D', 0x00, 0x00, 0x00,      // bProperytName : WCHAR    
    0x4E, 0x00, 0x00, 0x00,     // dwPropertyDataLength : 78 Bytes = 0x0000004E    
    '{', 0x00, 'E', 0x00,       // bPropertyData : WCHAR : {E7345CBB-9B3E-4C14-A726-3A8CD96CB167}
    '7', 0x00, '3', 0x00,       // bPropertyData  
    '4', 0x00, '5', 0x00,       // bPropertyData  
    'C', 0x00, 'B', 0x00,       // bPropertyData  
    'B', 0x00, '-', 0x00,       // bPropertyData  
    '9', 0x00, 'B', 0x00,       // bPropertyData  
    '3', 0x00, 'E', 0x00,       // bPropertyData  
    '-', 0x00, '4', 0x00,       // bPropertyData  
    'C', 0x00, '1', 0x00,       // bPropertyData  
    '4', 0x00, '-', 0x00,       // bPropertyData  
    'A', 0x00, '7', 0x00,       // bPropertyData  
    '2', 0x00, '6', 0x00,       // bPropertyData  
    '-', 0x00, '3', 0x00,       // bPropertyData  
    'A', 0x00, '8', 0x00,       // bPropertyData  
    'C', 0x00, 'D', 0x00,       // bPropertyData  
    '9', 0x00, '6', 0x00,       // bPropertyData  
    'C', 0x00, 'B', 0x00,       // bPropertyData  
    '1', 0x00, '6', 0x00,       // bPropertyData  
    '7', 
    0x00, '}', 0x00,       // bPropertyData  
    0x00, 0x00                  // bPropertyData
};

// L"DeviceInterfaceGUID" : wIndex = 0x0005// L"{12345678-1234-1234-1234-123456789ABC}"
__align(4) const uint8 WINUSB_ExtendedProperty_InterfaceGUID_Descritpor1[] =  //DIAG
{  
    0x8E, 0x00, 0x00, 0x00,     // dwTotalSize = Header + All sections  
    0x00, 0x01,                 // bcdVersion  
    0x05, 0x00,                 // wIndex  
    0x01, 0x00,                 // wCount    
    0x84, 0x00, 0x00, 0x00,     // dwSize -- this section    
    0x01, 0x00, 0x00, 0x00,     // dwPropertyDataType    
    0x28, 0x00,                 // wPropertyNameLength    
    'D', 0x00, 'e', 0x00,       // bProperytName : WCHAR : L"DeviceInterfaceGUID"  
    'v', 0x00, 'i', 0x00,       // bProperytName : WCHAR  
    'c', 0x00, 'e', 0x00,       // bProperytName : WCHAR  
    'I', 0x00, 'n', 0x00,       // bProperytName : WCHAR  
    't', 0x00, 'e', 0x00,       // bProperytName : WCHAR  
    'r', 0x00, 'f', 0x00,       // bProperytName : WCHAR  
    'a', 0x00, 'c', 0x00,       // bProperytName : WCHAR  
    'e', 0x00, 'G', 0x00,       // bProperytName : WCHAR  
    'U', 0x00, 'I', 0x00,       // bProperytName : WCHAR  
    'D', 0x00, 0x00, 0x00,      // bProperytName : WCHAR    
    0x4E, 0x00, 0x00, 0x00,     // dwPropertyDataLength : 78 Bytes = 0x0000004E    
    '{', 0x00, 'A', 0x00,       // bPropertyData : WCHAR : {A2B155D0-703E--49C9-9E95-209EF3630E48}
    '2', 0x00, 'B', 0x00,       // bPropertyData  
    '1', 0x00, '5', 0x00,       // bPropertyData  
    '5', 0x00, 'D', 0x00,       // bPropertyData  
    '0', 0x00, '-', 0x00,       // bPropertyData  
    '7', 0x00, '0', 0x00,       // bPropertyData  
    '3', 0x00, 'E', 0x00,       // bPropertyData  
    '-', 0x00, '4', 0x00,       // bPropertyData  
    '9', 0x00, 'C', 0x00,       // bPropertyData  
    '9', 0x00, '-', 0x00,       // bPropertyData  
    '9', 0x00, 'E', 0x00,       // bPropertyData  
    '9', 0x00, '5', 0x00,       // bPropertyData  
    '-', 0x00, '2', 0x00,       // bPropertyData  
    '0', 0x00, '9', 0x00,       // bPropertyData  
    'E', 0x00, 'F', 0x00,       // bPropertyData  
    '3', 0x00, '6', 0x00,       // bPropertyData  
    '3', 0x00, '0', 0x00,       // bPropertyData  
    'E', 0x00, '4', 0x00,       // bPropertyData  
    '8', 
    0x00, '}', 0x00,       // bPropertyData  
    0x00, 0x00                  // bPropertyData
};


LOCAL __align(4) char g_UCOM_StrProductForWinUsb[UCOM_DEV_STRING_DESC_LEN];

LOCAL __align(4) char g_UCOM_SerialNumStrForWinUsb[UCOM_DEV_STRING_DESC_LEN];

LOCAL __align(4) char  g_UCOM_StrManufacturerForWinUsb[] =
{
    0xc,
    USB_STRING_DESCRIPTOR_TYPE, 
    'N', 0, 'o', 0, 'k', 0, 'i', 0, 'a', 0, 
};

__align(4) const uint8  g_UCOM_Device_Qualifier_Descr[] = 
{
    0x0A,		//bLength
	0x06,		//bDescriptorType
    0x00,0x02,	//bcdUSB
    0xff,		//bDeviceClass
    0x00,		//bDeviceSubClass
    0x00,		//bDeviceProtocol
    0x40,		//bMaxPacketSize0
    0x00,		//bNumConfigurations , for other-speed configuration
    0x00		//bReserved
};
#endif
__align(4) const USB_STRING_LANGUAGE_DESCRIPTOR_T g_UCOM_LanguageStr []={
    sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN (USB_LANGUAGE_ENGLISH)
};

__align(4) const UCOM_STRING_MANUFACTURER_DESCRIPTOR_T  g_UCOM_StrManufacturer[] =
{
    sizeof(UCOM_STRING_MANUFACTURER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    {/*lint !e651*/
    'S', 0,'p', 0,'r', 0,'e', 0,'a', 0,'d', 0,'t', 0,
    'r', 0,'u', 0,'m', 0,' ', 0,' ', 0}
};

__align(4) const UCOM_STRING_PRODUCT_DESCRIPTOR_T g_UCOM_StrProduct[] =
{
    sizeof(UCOM_STRING_PRODUCT_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    {/*lint !e651*/
    'U', 0,'S', 0,'B', 0,'_', 0,'V', 0,'i', 0,'r', 0,'t', 0,
    'u', 0,'a', 0,'l', 0,' ', 0,'C', 0,'o', 0,'m', 0}
};

LOCAL const unsigned char s_UCOM_hex_string[]="0123456789ABCDEF";

__align(4) UCOM_STRING_SERIALNUMBER_DESCRIPTOR_T g_UCOM_SerialNumStr[] =
{
    sizeof(UCOM_STRING_SERIALNUMBER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    //UNICODE character string;
    { /*lint !e651*/
      'S',0,'p',0,'r',0,'e',0,'a',0,'d',0,'t',0,'r',0,
      'u',0,'m',0,'_',0,'M',0
    }
    //It must be modified in initialization.
};

__align(4) LOCAL const uint8 g_UCOM_VenderStr[UCOM_VENDER_STR_LEN] = {
    "Generic"
};

__align(4) const UCOM_CONFIG_DESC_TOWIF_T g_UCOM_UBOT_CfgDescr =
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UCOM_CFG_DESCR_LEN_TWOINF),
    UCOM_INTERFACE_TWO,		        //  2   two interfaces
    1,		                        // argument
    USB_STR_INDEX_LANGUAGE,
    0x80,                           //?? Self-powered ;
    UCOM_MAX_POWER,

    // Following is InterfaceDescr one  [UBOT]
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x00,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    USB_CLASS_CODE_MASSSTORAGE_CLASS_DEVICE,
    USB_SUBCLASS_CODE_SCSI,
    USB_PROTOCOL_CODE_BULK,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE1_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,
    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE1_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is InterfaceDescr two  [UCOM]
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x01,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP3 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE2_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,
    // Following is EP4 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE2_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,
};

__align(4) const UCOM_CONFIG_DESC_FOUR_IF_T g_UCOM_APCP_DATA_MODEM_CfgDesc =
{
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UCOM_CFG_DESCR_LEN_FOURIF),
    4,
    1,		                        // argument
    USB_STR_INDEX_LANGUAGE,
    0xC0,                           // Self-powered ;
    UCOM_MAX_POWER,

    // Following is InterfaceDescr one
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x00,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE1_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE1_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is InterfaceDescr two
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x01,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    // Following is EP3 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK|UCOM_INTERFACE2_BULKIN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is EP4 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UCOM_INTERFACE2_BULKOUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is Interface three
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x02,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x82,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x01,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    // Following is Interface four
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x03,
    NULL,
    UCOM_INTERFACE_EP_NUM,
    0xff,
    0x00,
    0x00,
    USB_STR_INDEX_LANGUAGE,

    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x84,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,

    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x03,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(EPN_MPS),
    NULL,
};

PUBLIC uint32 UCOM_GetCOnfigDescriptor(uint8 **para)
{
	uint32 fs = (uint32)*para;
    uint32 len = sizeof(g_UCOM_CfgDescr_twointerface);
    ucomWorkMode_t wm = UCOM_GetWorkMode();
    switch (wm) {
    case UCOM_WORK_DEBUG_DATA_MODEM:
        len = sizeof(g_UCOM_APCP_DATA_MODEM_CfgDesc);
        *para = (uint8 *)&g_UCOM_APCP_DATA_MODEM_CfgDesc;
        break;
#ifdef WINUSB_SUPPORT					
    case UCOM_WORK_DEBUG_DATA_UMS:
        len = sizeof(g_UCOM_CfgDescr_threeinterfaceForWinUsb);
        *para = (uint8 *)&g_UCOM_CfgDescr_threeinterfaceForWinUsb;
        break;
#endif					
    case UCOM_WORK_WITH_UBOT:
        *para = (uint8*)&g_UCOM_UBOT_CfgDescr;
        break;

    case UCOM_WORK_DATA_DEBUG:
        *para = (uint8 *)&g_UCOM_CfgDescr_twointerface;
        break;

    case UCOM_WORK_COM_DATA:
        len = sizeof(g_UCOM_CfgDescData);
        *para = (uint8 *)&g_UCOM_CfgDescData;
        break;

    case UCOM_WORK_AUTO_DWLD:
        len = sizeof(g_UCOM_CfgDescr_AutoDloader);
        *para = (uint8 *)&g_UCOM_CfgDescr_AutoDloader;

    case UCOM_WORK_COM_DEBUG:
    default:
        len = sizeof(g_UCOM_CfgDescDebug);
        *para = (uint8 *)&g_UCOM_CfgDescDebug;
        break;
    }

	return len;
}
#ifdef WINUSB_SUPPORT
LOCAL	uint8   s_imeiStr[(MAX_IMEI_LENGTH<<1) + 1] = {0};
LOCAL   uint8   s_rmType[PRODUCT_TYPE_LEN] = {0};

/* for stupid requirement, product name "XXX (RM-118X)"*/
// mass storage product string
LOCAL 	uint8	s_storage_product_str[] = "Unisoc-Mass Storage";

/*****************************************************************************/
//	String Descriptor Array
//	0	:for USB_STR_INDEX_LANGUAGE
//	1	:for USB_STR_INDEX_MANUFACTURER
//	2	:for USB_STR_INDEX_PRODUCT
//	3	:for USB_STR_INDEX_SERIALNUMBER
//	4 	:for USB_MS_OS_DESCRIPTOR_TYPE
/*****************************************************************************/
LOCAL UCOM_STR_DESC_T s_ucom_str_info[UCOM_DEV_STRING_INDEX]; 

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
PUBLIC PUCOM_STR_DESC_T UCOM_GetStrInfo(void)
{
	return (PUCOM_STR_DESC_T)s_ucom_str_info;
}

PUBLIC uint32 UCOM_StringConfig(uint8 *pDest, const uint8 *pSrc)
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

PUBLIC void usb_set_sn_info(void)
{	
	uint8	imei_arr[MAX_IMEI_LENGTH] = {0};

	EFS_NvitemRead(NV_IMEI_ID, MAX_IMEI_LENGTH, imei_arr);
    ConvertBcdToDigitalStr(MAX_IMEI_LENGTH, imei_arr, s_imeiStr);	
}

PUBLIC uint8* usb_get_sn_info(void)
{
	return s_imeiStr;
}

PUBLIC void usb_set_product_info(const char *str)
{
	uint32 cnt = strlen(str);

	//make up the stupid product string descriptor, like as "XXX (RM-1182)" 
	memcpy(s_rmType, str, cnt);
	
	s_rmType[cnt++] = ' ';
	s_rmType[cnt++] = '(';

	memcpy(s_rmType + cnt, PRODUCT_TYPE_Read(), RM_TYPE_LENGTH);

	cnt += RM_TYPE_LENGTH;
	s_rmType[cnt++] = ')';
	s_rmType[cnt] = '\0';	
}

PUBLIC uint8* usb_get_product_info(void)
{
	return s_rmType;
}

PUBLIC void UCOM_SetStrInfo(void)
{
	PUCOM_STR_DESC_T str = s_ucom_str_info;

	// language id
	str[USB_STR_INDEX_LANGUAGE].type	= USB_STR_INDEX_LANGUAGE;
	str[USB_STR_INDEX_LANGUAGE].len	= sizeof(g_UCOM_LanguageStr)/sizeof(uint8);
	str[USB_STR_INDEX_LANGUAGE].pStr = g_UCOM_LanguageStr;

	// manufacturer 
	str[USB_STR_INDEX_MANUFACTURER].type = USB_STR_INDEX_MANUFACTURER;
	str[USB_STR_INDEX_MANUFACTURER].len	= sizeof(g_UCOM_StrManufacturerForWinUsb)/sizeof(uint8);
	str[USB_STR_INDEX_MANUFACTURER].pStr = g_UCOM_StrManufacturerForWinUsb;

	// product
	str[USB_STR_INDEX_PRODUCT].type	= USB_STR_INDEX_PRODUCT;
	str[USB_STR_INDEX_PRODUCT].len  = UCOM_StringConfig(g_UCOM_StrProductForWinUsb, s_rmType);
	str[USB_STR_INDEX_PRODUCT].pStr = g_UCOM_StrProductForWinUsb;

	// iSerialNumber
	str[USB_STR_INDEX_SERIALNUMBER].type = USB_STR_INDEX_SERIALNUMBER;
	str[USB_STR_INDEX_SERIALNUMBER].len	= UCOM_StringConfig(g_UCOM_SerialNumStrForWinUsb, s_imeiStr);
	str[USB_STR_INDEX_SERIALNUMBER].pStr = g_UCOM_SerialNumStrForWinUsb;	


	// MS_OS_Descriptor
	str[UCOM_DEV_STRING_INDEX-1].type = USB_WINUSB_OS_DESCRIPTOR_TYPE;
	str[UCOM_DEV_STRING_INDEX-1].len  = sizeof(OS_StringDescriptor)/sizeof(uint8);
	str[UCOM_DEV_STRING_INDEX-1].pStr = OS_StringDescriptor;	
}

//change product string from charging mode to mass storage mode
PUBLIC void UCOM_ChangePrdStrInfo(void)
{
	PUCOM_STR_DESC_T str = s_ucom_str_info;

	usb_set_product_info(s_storage_product_str);

	str[USB_STR_INDEX_PRODUCT].type	= USB_STR_INDEX_PRODUCT;
	str[USB_STR_INDEX_PRODUCT].len  = UCOM_StringConfig(g_UCOM_StrProductForWinUsb, s_rmType);
	str[USB_STR_INDEX_PRODUCT].pStr = g_UCOM_StrProductForWinUsb;
}

#endif