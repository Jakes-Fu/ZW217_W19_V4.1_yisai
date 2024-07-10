/*
 * (C) Copyright 2003
 * Gerry Hamel, geh@ti.com, Texas Instruments
 *
 * Based on
 * linux/drivers/usbd/usb-function.h - USB Function
 *
 * Copyright (c) 2000, 2001, 2002 Lineo
 * Copyright (c) 2001 Hewlett Packard
 *
 * By:
 *	Stuart Lynne <sl@lineo.com>,
 *	Tom Rushworth <tbr@lineo.com>,
 *	Bruce Balden <balden@lineo.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* USB Descriptors - Create a complete description of all of the
 * function driver capabilities. These map directly to the USB descriptors.
 *
 * This heirarchy is created by the functions drivers and is passed to the
 * usb-device driver when the function driver is registered.
 *
 *  device
 *	configuration
 *	     interface
 *		alternate
 *		     class
 *		     class
 *		alternate
 *		     endpoint
 *		     endpoint
 *	     interface
 *		alternate
 *		     endpoint
 *		     endpoint
 *	configuration
 *	     interface
 *		alternate
 *		     endpoint
 *		     endpoint
 *
 *
 * The configuration structures refer to the USB Configurations that will be
 * made available to a USB HOST during the enumeration process.
 *
 * The USB HOST will select a configuration and optionally an interface with
 * the usb set configuration and set interface commands.
 *
 * The selected interface (or the default interface if not specifically
 * selected) will define the list of endpoints that will be used.
 *
 * The configuration and interfaces are stored in an array that is indexed
 * by the specified configuratin or interface number minus one.
 *
 * A configuration number of zero is used to specify a return to the unconfigured
 * state.
 *
 */


#ifndef __USBDESCRIPTORS_H__
#define __USBDESCRIPTORS_H__

//#include <asm/types.h>
#include "sci_types.h"

/*
 * communications class types
 *
 * c.f. CDC  USB Class Definitions for Communications Devices
 * c.f. WMCD USB CDC Subclass Specification for Wireless Mobile Communications Devices
 *
 */

#define CLASS_BCD_VERSION		0x0110

/* c.f. CDC 4.1 Table 14 */
#define COMMUNICATIONS_DEVICE_CLASS	0x02

/* c.f. CDC 4.2 Table 15 */
#define COMMUNICATIONS_INTERFACE_CLASS_CONTROL	0x02
#define COMMUNICATIONS_INTERFACE_CLASS_DATA		0x0A
#define COMMUNICATIONS_INTERFACE_CLASS_VENDOR	0x0FF

/* c.f. CDC 4.3 Table 16 */
#define COMMUNICATIONS_NO_SUBCLASS		0x00
#define COMMUNICATIONS_DLCM_SUBCLASS	0x01
#define COMMUNICATIONS_ACM_SUBCLASS		0x02
#define COMMUNICATIONS_TCM_SUBCLASS		0x03
#define COMMUNICATIONS_MCCM_SUBCLASS	0x04
#define COMMUNICATIONS_CCM_SUBCLASS		0x05
#define COMMUNICATIONS_ENCM_SUBCLASS	0x06
#define COMMUNICATIONS_ANCM_SUBCLASS	0x07

/* c.f. WMCD 5.1 */
#define COMMUNICATIONS_WHCM_SUBCLASS	0x08
#define COMMUNICATIONS_DMM_SUBCLASS		0x09
#define COMMUNICATIONS_MDLM_SUBCLASS	0x0a
#define COMMUNICATIONS_OBEX_SUBCLASS	0x0b

/* c.f. CDC 4.4 Table 17 */
#define COMMUNICATIONS_NO_PROTOCOL		0x00
#define COMMUNICATIONS_V25TER_PROTOCOL	0x01	/*Common AT Hayes compatible*/

/* c.f. CDC 4.5 Table 18 */
#define DATA_INTERFACE_CLASS		0x0a

/* c.f. CDC 4.6 No Table */
#define DATA_INTERFACE_SUBCLASS_NONE	0x00	/* No subclass pertinent */

/* c.f. CDC 4.7 Table 19 */
#define DATA_INTERFACE_PROTOCOL_NONE	0x00	/* No class protcol required */


/* c.f. CDC 5.2.3 Table 24 */
#define CS_INTERFACE		0x24
#define CS_ENDPOINT			0x25

/*
 * bDescriptorSubtypes
 *
 * c.f. CDC 5.2.3 Table 25
 * c.f. WMCD 5.3 Table 5.3
 */

#define USB_ST_HEADER		0x00
#define USB_ST_CMF			0x01
#define USB_ST_ACMF			0x02
#define USB_ST_DLMF			0x03
#define USB_ST_TRF			0x04
#define USB_ST_TCLF			0x05
#define USB_ST_UF			0x06
#define USB_ST_CSF			0x07
#define USB_ST_TOMF			0x08
#define USB_ST_USBTF		0x09
#define USB_ST_NCT			0x0a
#define USB_ST_PUF			0x0b
#define USB_ST_EUF			0x0c
#define USB_ST_MCMF			0x0d
#define USB_ST_CCMF			0x0e
#define USB_ST_ENF			0x0f
#define USB_ST_ATMNF		0x10

#define USB_ST_WHCM			0x11
#define USB_ST_MDLM			0x12
#define USB_ST_MDLMD		0x13
#define USB_ST_DMM			0x14
#define USB_ST_OBEX			0x15
#define USB_ST_CS			0x16
#define USB_ST_CSD			0x17
#define USB_ST_TCM			0x18

/* endpoint modifiers
 * static struct usb_endpoint_description function_default_A_1[] = {
 *
 *     {this_endpoint: 0, attributes: CONTROL,	 max_size: 8,  polling_interval: 0 },
 *     {this_endpoint: 1, attributes: BULK,	 max_size: 64, polling_interval: 0, direction: IN},
 *     {this_endpoint: 2, attributes: BULK,	 max_size: 64, polling_interval: 0, direction: OUT},
 *     {this_endpoint: 3, attributes: INTERRUPT, max_size: 8,  polling_interval: 0},
 *
 *
 */
#define OUT		0x00
#define IN		0x80

#define CONTROL		0x00
#define ISOCHRONOUS	0x01
#define BULK		0x02
#define INTERRUPT	0x03


/* configuration modifiers
 */
#define BMATTRIBUTE_RESERVED		0x80
#define BMATTRIBUTE_SELF_POWERED	0x40

/*
 * standard usb descriptor structures
 */

struct usb_endpoint_descriptor {
	uint8 bLength;
	uint8 bDescriptorType;	/* 0x5 */
	uint8 bEndpointAddress;
	uint8 bmAttributes;
	uint16 wMaxPacketSize;
	uint8 bInterval;
} __attribute__ ((packed));

struct usb_interface_descriptor {
	uint8 bLength;
	uint8 bDescriptorType;	/* 0x04 */
	uint8 bInterfaceNumber;
	uint8 bAlternateSetting;
	uint8 bNumEndpoints;
	uint8 bInterfaceClass;
	uint8 bInterfaceSubClass;
	uint8 bInterfaceProtocol;
	uint8 iInterface;
} __attribute__ ((packed));

struct usb_configuration_descriptor {
	uint8 bLength;
	uint8 bDescriptorType;	/* 0x2 */
	uint16 wTotalLength;
	uint8 bNumInterfaces;
	uint8 bConfigurationValue;
	uint8 iConfiguration;
	uint8 bmAttributes;
	uint8 bMaxPower;
} __attribute__ ((packed));

struct usb_device_descriptor {
	uint8 bLength;
	uint8 bDescriptorType;	/* 0x01 */
	uint16 bcdUSB;
	uint8 bDeviceClass;
	uint8 bDeviceSubClass;
	uint8 bDeviceProtocol;
	uint8 bMaxPacketSize0;
	uint16 idVendor;
	uint16 idProduct;
	uint16 bcdDevice;
	uint8 iManufacturer;
	uint8 iProduct;
	uint8 iSerialNumber;
	uint8 bNumConfigurations;
} __attribute__ ((packed));

#if 1 // defined(CONFIG_USBD_HS)
struct usb_qualifier_descriptor {
	uint8 bLength;
	uint8 bDescriptorType;

	uint16 bcdUSB;
	uint8 bDeviceClass;
	uint8 bDeviceSubClass;
	uint8 bDeviceProtocol;
	uint8 bMaxPacketSize0;
	uint8 bNumConfigurations;
	uint8 breserved;
} __attribute__ ((packed));
#endif

struct usb_string_descriptor {
	uint8 bLength;
	uint8 bDescriptorType;	/* 0x03 */
	//uint16 wData[0];
} __attribute__ ((packed));

struct usb_generic_descriptor {
	uint8 bLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;
} __attribute__ ((packed));


/*
 * communications class descriptor structures
 *
 * c.f. CDC 5.2 Table 25c
 */

struct usb_class_function_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;
} __attribute__ ((packed));

struct usb_class_function_descriptor_generic {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;
	uint8 bmCapabilities;
} __attribute__ ((packed));

struct usb_class_header_function_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x00 */
	uint16 bcdCDC;
} __attribute__ ((packed));

struct usb_class_call_management_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x01 */
	uint8 bmCapabilities;
	uint8 bDataInterface;
} __attribute__ ((packed));

struct usb_class_abstract_control_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x02 */
	uint8 bmCapabilities;
} __attribute__ ((packed));

struct usb_class_direct_line_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x03 */
} __attribute__ ((packed));

struct usb_class_telephone_ringer_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x04 */
	uint8 bRingerVolSeps;
	uint8 bNumRingerPatterns;
} __attribute__ ((packed));

struct usb_class_telephone_call_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x05 */
	uint8 bmCapabilities;
} __attribute__ ((packed));

struct usb_class_union_function_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x06 */
	uint8 bMasterInterface;
	/* uint8 bSlaveInterface0[0]; */
	uint8 bSlaveInterface0;
} __attribute__ ((packed));

struct usb_class_country_selection_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x07 */
	uint8 iCountryCodeRelDate;
	//uint16 wCountryCode0[0];
} __attribute__ ((packed));


struct usb_class_telephone_operational_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x08 */
	uint8 bmCapabilities;
} __attribute__ ((packed));


struct usb_class_usb_terminal_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x09 */
	uint8 bEntityId;
	uint8 bInterfaceNo;
	uint8 bOutInterfaceNo;
	uint8 bmOptions;
	//uint8 bChild0[0];
} __attribute__ ((packed));

struct usb_class_network_channel_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x0a */
	uint8 bEntityId;
	uint8 iName;
	uint8 bChannelIndex;
	uint8 bPhysicalInterface;
} __attribute__ ((packed));

struct usb_class_protocol_unit_function_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x0b */
	uint8 bEntityId;
	uint8 bProtocol;
	//uint8 bChild0[0];
} __attribute__ ((packed));

struct usb_class_extension_unit_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x0c */
	uint8 bEntityId;
	uint8 bExtensionCode;
	uint8 iName;
	//uint8 bChild0[0];
} __attribute__ ((packed));

struct usb_class_multi_channel_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x0d */
	uint8 bmCapabilities;
} __attribute__ ((packed));

struct usb_class_capi_control_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x0e */
	uint8 bmCapabilities;
} __attribute__ ((packed));

struct usb_class_ethernet_networking_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x0f */
	uint8 iMACAddress;
	uint32 bmEthernetStatistics;
	uint16 wMaxSegmentSize;
	uint16 wNumberMCFilters;
	uint8 bNumberPowerFilters;
} __attribute__ ((packed));

struct usb_class_atm_networking_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x10 */
	uint8 iEndSystermIdentifier;
	uint8 bmDataCapabilities;
	uint8 bmATMDeviceStatistics;
	uint16 wType2MaxSegmentSize;
	uint16 wType3MaxSegmentSize;
	uint16 wMaxVC;
} __attribute__ ((packed));


struct usb_class_mdlm_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x12 */
	uint16 bcdVersion;
	uint8 bGUID[16];
} __attribute__ ((packed));

struct usb_class_mdlmd_descriptor {
	uint8 bFunctionLength;
	uint8 bDescriptorType;
	uint8 bDescriptorSubtype;	/* 0x13 */
	uint8 bGuidDescriptorType;
	//uint8 bDetailData[0];
} __attribute__ ((packed));

/*
 * HID class descriptor structures
 *
 * c.f. HID 6.2.1
 */

struct usb_class_hid_descriptor {
    uint8	      bLength;
    uint8	      bDescriptorType;
    uint16	      bcdCDC;
    uint8	      bCountryCode;
    uint8	      bNumDescriptors;	/* 0x01 */
    uint8	      bDescriptorType0;
    uint16	      wDescriptorLength0;
    /* optional descriptors are not supported. */
} __attribute__((packed));

struct usb_class_report_descriptor {
    uint8	      bLength;	/* dummy */
    uint8	      bDescriptorType;
    uint16	      wLength;
    //uint8		bData[0];
} __attribute__((packed));

/*
 * descriptor union structures
 */

struct usb_descriptor {
	union {
		struct usb_generic_descriptor generic;
		struct usb_endpoint_descriptor endpoint;
		struct usb_interface_descriptor interface;
		struct usb_configuration_descriptor configuration;
		struct usb_device_descriptor device;
		struct usb_string_descriptor string;
	} descriptor;
};
//} __attribute__ ((packed));

struct usb_class_descriptor {
	union {
		struct usb_class_function_descriptor function;
		struct usb_class_function_descriptor_generic generic;
		struct usb_class_header_function_descriptor header_function;
		struct usb_class_call_management_descriptor call_management;
		struct usb_class_abstract_control_descriptor abstract_control;
		struct usb_class_direct_line_descriptor direct_line;
		struct usb_class_telephone_ringer_descriptor telephone_ringer;
		struct usb_class_telephone_operational_descriptor telephone_operational;
		struct usb_class_telephone_call_descriptor telephone_call;
		struct usb_class_union_function_descriptor union_function;
		struct usb_class_country_selection_descriptor country_selection;
		struct usb_class_usb_terminal_descriptor usb_terminal;
		struct usb_class_network_channel_descriptor network_channel;
		struct usb_class_extension_unit_descriptor extension_unit;
		struct usb_class_multi_channel_descriptor multi_channel;
		struct usb_class_capi_control_descriptor capi_control;
		struct usb_class_ethernet_networking_descriptor ethernet_networking;
		struct usb_class_atm_networking_descriptor atm_networking;
		struct usb_class_mdlm_descriptor mobile_direct;
		struct usb_class_mdlmd_descriptor mobile_direct_detail;
		struct usb_class_hid_descriptor hid;
	} descriptor;
};
//} __attribute__ ((packed));

#ifdef DEBUG
static inline void print_device_descriptor(struct usb_device_descriptor *d)
{
	rprintf("usb device descriptor \n");
	rprintf("\tbLength %2.2x\n", d->bLength);
	rprintf("\tbDescriptorType %2.2x\n", d->bDescriptorType);
	rprintf("\tbcdUSB %4.4x\n", d->bcdUSB);
	rprintf("\tbDeviceClass %2.2x\n", d->bDeviceClass);
	rprintf("\tbDeviceSubClass %2.2x\n", d->bDeviceSubClass);
	rprintf("\tbDeviceProtocol %2.2x\n", d->bDeviceProtocol);
	rprintf("\tbMaxPacketSize0 %2.2x\n", d->bMaxPacketSize0);
	rprintf("\tidVendor %4.4x\n", d->idVendor);
	rprintf("\tidProduct %4.4x\n", d->idProduct);
	rprintf("\tbcdDevice %4.4x\n", d->bcdDevice);
	rprintf("\tiManufacturer %2.2x\n", d->iManufacturer);
	rprintf("\tiProduct %2.2x\n", d->iProduct);
	rprintf("\tiSerialNumber %2.2x\n", d->iSerialNumber);
	rprintf("\tbNumConfigurations %2.2x\n", d->bNumConfigurations);
}

#else

/* stubs */
#define print_device_descriptor(d)

#endif /* DEBUG */
#endif
