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

#ifndef _USB_SERIAL_H_
#define _USB_SERIAL_H_

#include "usb_device_uws6121e.h"
#include "usb_composite_device.h"
#include <stdint.h>
#include <stddef.h>
#include "../inc/usb_cdc.h"
#include "com_drvapi.h"
#ifdef __cplusplus
extern "C" {
#endif

struct usbSerial;

typedef struct
{
    int (*bind)(uint32_t name, struct usbSerial *serial);
    void (*unbind)(struct usbSerial *serial);
    int (*enable)(struct usbSerial *serial);
    void (*disable)(struct usbSerial *serial);
    bool (*open)(struct usbSerial *serial);
    void (*close)(struct usbSerial *serial);
	void (*resume)(struct usbSerial *serial);
    void (*suspend)(struct usbSerial *serial);
} usbSerialDriverOps_t;

/**
 * @brief struct of usb serial
 */
typedef struct usbSerial
{
    usb_endpoint_descriptor_t *epin_desc;  ///< usb in endpoint descriptor
    usb_endpoint_descriptor_t *epout_desc; ///< usb out endpoint descriptor
    copsFunc_t *func;                      ///< the composite function
    const usbSerialDriverOps_t *ops;       ///< driver implementation ops
    unsigned long priv;                    ///< private data for usb serial;
} usbSerial_t;

const usbSerialDriverOps_t *usbSerialGetOps(void);
const usbSerialDriverOps_t *usbDebugSerialGetOps(void);
const usbSerialDriverOps_t *usbVcomGetOps(void);

#define CDC_PTR_SANE(cdc) (cdc != NULL && cdc->func != NULL && cdc->func->controller != NULL)
#define CDC_PRIV_PTR_SANE(cdc) (cdc != NULL && cdc->priv != 0 && cdc->func != NULL && cdc->func->controller != NULL)

#define DRV_NAME_USRL_COM0 USB_COM0 //OSI_MAKE_TAG('U', 'S', 'L', '0')     // AT
#define DRV_NAME_USRL_COM1 USB_COM1 //OSI_MAKE_TAG('U', 'S', 'L', '1')     // diag = ap log
#define DRV_NAME_USRL_COM2 USB_COM2 //OSI_MAKE_TAG('U', 'S', 'L', '2')     // debug host(coolwatcher)
#define DRV_NAME_USRL_COM3 USB_COM3 //OSI_MAKE_TAG('U', 'S', 'L', '3')     // modem log
#define DRV_NAME_USRL_COM4 USB_COM4 //OSI_MAKE_TAG('U', 'S', 'L', '4')     // PPP_AT

#ifdef __cplusplus
}
#endif

#endif
