/******************************************************************************
 ** File Name:    winusb.h                                               *
 ** Author:       eric.xu                                                 *
 ** DATE:         2015-10-8                                                   *
 ** Copyright:    2015 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 2015-10-      eric     Create.                                     *
 ******************************************************************************/

#ifndef _WINUSB_H_
#define _WINUSB_H_

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

//Defined WinUSB Request 
#define USB_WINUSB_OS_DESCRIPTOR_TYPE             0xEE
#define bMS_VendorCode                        	  0xCC	
#define USB_REQUEST_MS_VENDOR                     bMS_VendorCode
#define USB_REQUEST_COMPATIBLE_ID                 (0x04)
#define USB_REQUEST_INTERFACE_GUID                (0x05)
//


typedef struct _WINUSB_OS_STRING_DESCRIPTOR {
    uint8    bLength;
    uint8    bDescriptorType;
    uint8    qwSigature[14];
    uint8    bMSVendorCode;
    uint8    bPad;
} WINUSB_OS_STRING_DESCRIPTOR_T, *PWINUSB_OS_STRING_DESCRIPTOR_T;

typedef struct _WINUSB_COMPATID_HEADER_SECTION {
    uint32  dwLength;
    uint16  bcdVersion;
    uint16  wIndex;
    uint8   wCount; 
    uint8   Reserved[7];
} WINUSB_COMPATID_HEADER_SECTION_T, *PWINUSB_COMPATID_HEADER_SECTION_T;

typedef struct _WINUSB_COMPATID_FUNCTION_SECTION {
    uint8   bFirstInterfaceNumber; 
    uint8   Reserved_system;  //one
    uint8   compactiableID[8];
    uint8   subCompactiableID[8];
    uint8   Reserved[6];
} WINUSB_COMPATID_FUNCTION_SECTION_T, *PWINUSB_COMPATID_FUNCTION_SECTION_T;

typedef struct _WINUSB_PROPERTY_HEADER_SECTION {
    uint32  dwLength;
    uint16  bcdVersion;
    uint16  wIndex;
    uint16  wCount;
} WINUSB_PROPERTY_HEADER_SECTION_T, *PWINUSB_PROPERTY_HEADER_SECTION_T;

/*
typedef struct _WINUSB_CUSTOM_PROPERTY_SECTION {
    uint32  dwSize;
    uint32  dwPropertyDataType;
    uint16  wPropertyNameLength;
    uint8   bPropertyName[];    // by wPropertyNameLength
    uint32  wPropertyDataLength;
    uint8   bPropertyData[];    // by wPropertyDataLength
} WINUSB_CUSTOM_PROPERTY_SECTION_T, *PWINUSB_CUSTOM_PROPERTY_SECTION_T;
*/
typedef struct _Winusb_ExtendedCompatId{
    WINUSB_COMPATID_HEADER_SECTION_T    header; // 16
    WINUSB_COMPATID_FUNCTION_SECTION_T  interface1; // 24
    WINUSB_COMPATID_FUNCTION_SECTION_T  interface2; // 24
} WINUSB_EXTENDED_COMPATID;

/**---------------------------------------------------------------------------*
 **                             Compiler Flag  End                            *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif 