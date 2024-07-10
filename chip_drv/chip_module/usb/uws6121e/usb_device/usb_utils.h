/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _USB__UTILS_H_
#define _USB__UTILS_H_

#include <stdlib.h>
#include <string.h>
#include "osi_compiler.h"
#include "os_api.h"
#ifndef cpu_to_le32
#define cpu_to_le32(x) (x)
#endif

#ifndef cpu_to_le16
#define cpu_to_le16(x) (x)
#endif

#ifndef le16_to_cpu
#define le16_to_cpu(x) (x)
#endif

typedef unsigned int uint32_t;

/**
 * usb connect timeout
 */
#define CONFIG_USB_CONNECT_TIMEOUT 5000

/**
 * udc features, lower 8 bits for config->bmAttributes, higher are software defined
 */
#define CONFIG_USB_DEVICE_CONTROLLER_FEATURE 0x80 //0xE0

/**
 * usb ether host mac address
 */
#define CONFIG_USB_ETHER_HOST_MAC "\xf0\x4b\xb3\xb9\xeb\xe5"

/**
 * usb ether dev mac address
 */
#define CONFIG_USB_ETHER_DEV_MAC "\xfa\x32\x47\x15\xe1\x88"

/**
 * usb ether support dynamic change host mac address
 */
#define CONFIG_USB_ETHER_DYNAMIC_HOST_MAC_SUPPORT on

#define USB_DEBUG_ENABLE  FALSE

#if USB_DEBUG_ENABLE
#include <stdarg.h>


//extern void Debug_Uart4_puts(char *Str);
PUBLIC unsigned int SCI_TraceLowly(const char * x_format, ...);


#define USB_LOG_TRACE(...) SCI_TraceLowly(__VA_ARGS__)
#define USB_ERR_TRACE(...) SCI_TraceLowly(__VA_ARGS__)

#else
#define USB_LOG_TRACE(...) 
#define USB_ERR_TRACE(...) SCI_TraceLow(__VA_ARGS__)

#endif


#endif // _USB__UTILS_H_
