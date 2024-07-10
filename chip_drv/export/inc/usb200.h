/******************************************************************************
 ** File Name:    usb200.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/9/2005                                                    *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 9/1/2003      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _USB200_H_
#define _USB200_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

#include "sci_types.h"
#ifdef WINUSB_SUPPORT
#include "winusb.h"
#endif
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
#define USB_SPECIFICATION_VERSION                 0x0110
#define USB_MAXIMUM_STRING_LENGTH                 255


#define USB_GETSTATUS_SELF_POWERED                0x01
#define USB_GETSTATUS_REMOTE_WAKEUP_ENABLED       0x02


#define USB_DEVICE_DESCRIPTOR_TYPE                0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE         0x02
#define USB_STRING_DESCRIPTOR_TYPE                0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE             0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE              0x05
#define USB_POWER_DESCRIPTOR_TYPE                 0x06

#define USB_STR_INDEX_LANGUAGE                        0x00
#define USB_STR_INDEX_MANUFACTURER                    0x01
#define USB_STR_INDEX_PRODUCT                         0x02
#define USB_STR_INDEX_SERIALNUMBER                    0x03
#define USB_STR_INDEX_CONFIGURATION                   0x04
#define USB_STR_INDEX_INTERFACE                       0x05

//modify for adpter the MSC TEST TOOL by usb.org
#define USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE                 0x06
#define USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR_TYPE  0x07

#define USB_LANGUAGE_ENGLISH                          0x0409

#define USB_DESCRIPTOR_MAKE_TYPE_AND_INDEX(d, i) ((uint16)((uint16)d<<8 | i))

//
// Values for bmAttributes field of an
// endpoint descriptor
//

#define USB_ENDPOINT_TYPE_MASK                    0x03

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03


//
// definitions for bits in the bmAttributes field of a
// configuration descriptor.
//
#define USB_CONFIG_POWERED_MASK                   0xc0

#define USB_CONFIG_BUS_POWERED                    0x80
#define USB_CONFIG_SELF_POWERED                   0x40
#define USB_CONFIG_REMOTE_WAKEUP                  0x20

//
// Endpoint direction bit, stored in address
//

#define USB_ENDPOINT_DIRECTION_MASK               0x80

// test direction bit in the bEndpointAddress field of
// an endpoint descriptor.
#define USB_ENDPOINT_DIRECTION_OUT(addr)          (!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define USB_ENDPOINT_DIRECTION_IN(addr)           ((addr) & USB_ENDPOINT_DIRECTION_MASK)

//
// USB defined request codes
// see chapter 9 of the USB 1.0 specifcation for
// more information.
//

// These are the correct values based on the USB 1.0
// specification

#define USB_REQUEST_GET_STATUS                    0x00
#define USB_REQUEST_CLEAR_FEATURE                 0x01

#define USB_REQUEST_SET_FEATURE                   0x03

#define USB_REQUEST_SET_ADDRESS                   0x05
#define USB_REQUEST_GET_DESCRIPTOR                0x06
#define USB_REQUEST_SET_DESCRIPTOR                0x07
#define USB_REQUEST_GET_CONFIGURATION             0x08
#define USB_REQUEST_SET_CONFIGURATION             0x09
#define USB_REQUEST_GET_INTERFACE                 0x0A
#define USB_REQUEST_SET_INTERFACE                 0x0B
#define USB_REQUEST_SYNC_FRAME                    0x0C
#define USB_REQUEST_MAX                           0x0D

//
// defined USB device classes
//


#define USB_DEVICE_CLASS_RESERVED           0x00
#define USB_DEVICE_CLASS_AUDIO              0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS     0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE    0x03
#define USB_DEVICE_CLASS_MONITOR            0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE 0x05
#define USB_DEVICE_CLASS_POWER              0x06
#define USB_DEVICE_CLASS_PRINTER            0x07
#define USB_DEVICE_CLASS_STORAGE            0x08
#define USB_DEVICE_CLASS_HUB                0x09
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC    0xFF

//
// USB defined Feature selectors
//

#define USB_FEATURE_ENDPOINT_STALL          0x0000
#define USB_FEATURE_REMOTE_WAKEUP           0x0001
#define USB_FEATURE_POWER_D0                0x0002
#define USB_FEATURE_POWER_D1                0x0003
#define USB_FEATURE_POWER_D2                0x0004
#define USB_FEATURE_POWER_D3                0x0005


//Define direction ;
#define USB_HOST_IN     0x01        //From device to host ;
#define USB_HOST_OUT    0x00        //From host to device ;

//Define request type;
#define USB_REQ_STANDARD    0x00
#define USB_REQ_CLASS       0x01
#define USB_REQ_VENDOR      0x02
#define USB_REQ_RESERVED    0x03

//Define accepter ;
#define USB_REC_DEVICE      0x00
#define USB_REC_INTERFACE   0x01
#define USB_REC_ENDPOINT    0x02
#define USB_REC_OTHER       0x03

#define USB_MAX_REQ_TYPE    0x04
#define USB_MAX_RECIPIENT   0x05
#define USB_MAX_REQUEST     0x30

#define USB_BUS_POWERED                           0x80
#define USB_SELF_POWERED                          0x40
#define USB_REMOTE_WAKEUP                         0x20

//
// USB power descriptor added to core specification
//

#define USB_SUPPORT_D0_COMMAND      0x01
#define USB_SUPPORT_D1_COMMAND      0x02
#define USB_SUPPORT_D2_COMMAND      0x04
#define USB_SUPPORT_D3_COMMAND      0x08

#define USB_SUPPORT_D1_WAKEUP       0x10
#define USB_SUPPORT_D2_WAKEUP       0x20


#define USB_CLASS_CODE_MASSSTORAGE_CLASS_DEVICE             0x08

#define USB_SUBCLASS_CODE_RBC                               0x01
#define USB_SUBCLASS_CODE_SFF8020I                          0x02
#define USB_SUBCLASS_CODE_QIC157                            0x03
#define USB_SUBCLASS_CODE_UFI                               0x04
#define USB_SUBCLASS_CODE_SFF8070I                          0x05
#define USB_SUBCLASS_CODE_SCSI                              0x06

#define USB_PROTOCOL_CODE_CBI0                              0x00
#define USB_PROTOCOL_CODE_CBI1                              0x01
#define USB_PROTOCOL_CODE_BULK                              0x50

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
//Stand USB device request command type define ;
typedef union _USB_STD_DEV_REQ_U
{
    struct
    {
        union
        {
            struct bit_map
            {
                uint direction :1;
                uint command   :2;
                uint receiver  :5;
            } mBits;
            uint8 bValue;
        } bmReqType;
        uint8  bReqCode;
        uint16 wValue;
        uint16 wIndex;
        uint16 wLength;
    } sData;
    uint8 bBuf[8];
} USB_STD_DEV_REQ_U;

typedef PACK struct _USB_DEVICE_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint16   bcdUSB;
    uint8    bDeviceClass;
    uint8    bDeviceSubClass;
    uint8    bDeviceProtocol;
    uint8    bMaxPacketSize0;
    uint16   idVendor;
    uint16   idProduct;
    uint16   bcdDevice;
    uint8    iManufacturer;
    uint8    iProduct;
    uint8    iSerialNumber;
    uint8    bNumConfigurations;
} USB_DEVICE_DESCRIPTOR_T, *PUSB_DEVICE_DESCRIPTOR_T;

//
// values for bmAttributes Field in
// USB_CONFIGURATION_DESCRIPTOR
//
typedef PACK struct _USB_CONFIGURATION_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint16   wTotalLength;
    uint8    bNumInterfaces;
    uint8    bConfigurationValue;
    uint8    iConfiguration;
    uint8    bmAttributes;
    uint8    MaxPower;
} USB_CONFIGURATION_DESCRIPTOR_T, *PUSB_CONFIGURATION_DESCRIPTOR_T;

typedef PACK struct _USB_INTERFACE_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint8    bInterfaceNumber;
    uint8    bAlternateSetting;
    uint8    bNumEndpoints;
    uint8    bInterfaceClass;
    uint8    bInterfaceSubClass;
    uint8    bInterfaceProtocol;
    uint8    iInterface;
} USB_INTERFACE_DESCRIPTOR_T, *PUSB_INTERFACE_DESCRIPTOR_T;

typedef PACK struct _USB_ENDPOINT_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint8    bEndpointAddress;
    uint8    bmAttributes;
    uint16   wMaxPacketSize;
    uint8    bInterval;
} USB_ENDPOINT_DESCRIPTOR_T, *PUSB_ENDPOINT_DESCRIPTOR_T;

typedef PACK struct _USB_STRING_LANGUAGE_DESCRIPTOR
{
    uint8  bLength;
    uint8  bDescriptorType;
    uint16 ulanguageID;
} USB_STRING_LANGUAGE_DESCRIPTOR_T,*PUSB_STRING_LANGUAGE_DESCRIPTOR_T;

typedef PACK struct _USB_POWER_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint8    bCapabilitiesFlags;
    uint16   EventNotification;
    uint16   D1LatencyTime;
    uint16   D2LatencyTime;
    uint16   D3LatencyTime;
    uint8    PowerUnit;
    uint16   D0PowerConsumption;
    uint16   D1PowerConsumption;
    uint16   D2PowerConsumption;
} USB_POWER_DESCRIPTOR_T, *PUSB_POWER_DESCRIPTOR_T;


typedef PACK struct _USB_COMMON_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
} USB_COMMON_DESCRIPTOR_T, *PUSB_COMMON_DESCRIPTOR_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
