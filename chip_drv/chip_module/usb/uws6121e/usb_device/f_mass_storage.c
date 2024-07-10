/* Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
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

//#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('M', 'S', 'G', 'F')
//#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "msg_dev.h"
#include "usb_utils.h"
#include "usb_ch9.h"
//#include "usb_device_uws6121e.h"
//#include "usb_composite_device.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/* bulk-only class specific requests */
#define US_BULK_RESET_REQUEST 0xff
#define US_BULK_GET_MAX_LUN 0xfe

struct f_msg
{
    copsFunc_t func;
    usbMsgDev_t dev;
    int8_t intf;

    usbXfer_t *tx_xfer;
    usbXfer_t *rx_xfer;
};

/* String IDs */
#define INTERFACE_STRING_INDEX 0

static usbString_t s_msg_str = {
    0,0,"Mass Storage",
};

static usb_interface_descriptor_t msg_intf_desc = {
     USB_DT_INTERFACE_SIZE,	//bLength
     USB_DT_INTERFACE,		//bDescriptorType
     0,						//bInterfaceNumber
     0,						//bAlternateSetting
     2,						//bNumEndpoints
     USB_CLASS_MASS_STORAGE,//bInterfaceClass
     USB_SUBCLASS_SCSI,		//bInterfaceSubClass
     0x50,					//bInterfaceProtocol  //USB_PROTOCOL_CODE_BULK
     0						//iInterface = DYNAMIC
};

static usb_endpoint_descriptor_t msg_ep_in_desc = {
    USB_DT_ENDPOINT_SIZE,	//bLength
    USB_DT_ENDPOINT,		//bDescriptorType
    USB_DIR_IN,				//bEndpointAddress
    USB_ENDPOINT_XFER_BULK, //bmAttributes
    cpu_to_le16(512),		//wMaxPacketSize
    0,						//bInterval
};
static usb_endpoint_descriptor_t msg_ep_out_desc = {
    USB_DT_ENDPOINT_SIZE,	//bLength
    USB_DT_ENDPOINT,		//bDescriptorType
    USB_DIR_OUT,			//bEndpointAddress
    USB_ENDPOINT_XFER_BULK, //bmAttributes
    cpu_to_le16(512),		//wMaxPacketSize
    0,						//bInterval
};

static usb_descriptor_t *s_msg_descs[] = {
    (usb_descriptor_t *)&msg_intf_desc,
    (usb_descriptor_t *)&msg_ep_in_desc,
    (usb_descriptor_t *)&msg_ep_out_desc,
    NULL,
};

static struct f_msg *f2msg(copsFunc_t *f) { return (struct f_msg *)f; }

static uint32_t _msgFillDesc(copsFunc_t *f, void *buf_, uint32_t size)
{
	uint8_t i;
	uint32_t len;

    uint8_t *buf = (uint8_t *)buf_;
    uint32_t total = size;
	usb_descriptor_t *h = PNULL;
    USB_LOG_TRACE("msg fill descriptor\n");

    if (f->controller->speed >= USB_SPEED_HIGH)
    {
        msg_ep_in_desc.wMaxPacketSize = cpu_to_le16(512);
        msg_ep_out_desc.wMaxPacketSize = cpu_to_le16(512);
    }
    else if (f->controller->speed >= USB_SPEED_FULL)
    {
        msg_ep_in_desc.wMaxPacketSize = cpu_to_le16(64);
        msg_ep_out_desc.wMaxPacketSize = cpu_to_le16(64);
    }
    else
    {
        msg_ep_in_desc.wMaxPacketSize = cpu_to_le16(8);
        msg_ep_out_desc.wMaxPacketSize = cpu_to_le16(8);
    }

    h = s_msg_descs[0];
    for (i = 1; h != PNULL && size != 0; ++i)
    {
        len = OSI_MIN(uint32_t, h->bLength, size);
        memcpy(buf, h, len);
        size -= len;
        buf += len;
        h = s_msg_descs[i];
    }
    return total - size;
}

static void _msgDestroy(copsFunc_t *f)
{
    free(f);
}

static int _msgSetup(copsFunc_t *f, const usb_device_request_t *ctrl)
{
    struct f_msg *msg = f2msg(f);
    copsCtrl_t *cc = copsGetCtrl(f->cops);
    usbXfer_t *xfer = cc->xfer;
    uint8_t b_reqtype = ctrl->bRequestType;
    uint8_t b_request = ctrl->bRequest;
    uint16_t w_indx = cpu_to_le16(ctrl->wIndex);
    uint16_t w_value = cpu_to_le16(ctrl->wValue);
    uint16_t w_length_raw = cpu_to_le16(ctrl->wLength);
    uint16_t w_length = OSI_MIN(uint16_t, w_length_raw, cc->bufsize);
    int value = -EOPNOTSUPP;
	uint8 t_uLun = 1;//(msg->dev.priv)->udisk_num;
    uint8_t nluns[1] = {0};
	int res;
	
	nluns[0] = t_uLun-1;

    xfer->zlp = 0;
    xfer->param = msg;
    xfer->length = 0;
    xfer->buf = cc->buffer;
    xfer->complete = NULL;

    switch (b_request)
    {
    case US_BULK_GET_MAX_LUN:
        if (b_reqtype != (USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE))
            break;

        if (w_indx != msg->intf || w_value != 0 || w_length != 1)
            return -EINVAL;
        USB_LOG_TRACE("msg get max lun\n");
        value = OSI_MIN(uint16_t, w_length, sizeof(nluns));
        memcpy(xfer->buf, nluns, value);
    default:
        break;
    }

    if (value >= 0)
    {
        
        xfer->zlp = value < w_length_raw;
        xfer->length = value;
        res = udcEpQueue(f->controller, f->controller->ep0, xfer);
        if (res < 0)
        {
            USB_LOG_TRACE("msg setup fail\n");
            return res;
        }
    }
    return value < 0 ? value : 0;
}

static int _msgBind(copsFunc_t *f, cops_t *cops, udc_t *udc)
{

    struct f_msg *msg = f2msg(f);
    int intfnum = -1;
    bool result = false;
	USB_LOG_TRACE("msg bind\n");
    do
    {
        msg->dev.epin = udcEpAlloc(f->controller, &msg_ep_in_desc);
        msg->dev.epout = udcEpAlloc(f->controller, &msg_ep_out_desc);
        if (!(msg->dev.epin && msg->dev.epout))
            break;

        intfnum = copsAssignInterface(f->cops, f);
        if (intfnum < 0)
            break;

        result = usbMsgDevBind(&msg->dev);
    } while (0);

    if (!result)
    {
        udcEpFree(f->controller, msg->dev.epin);
        udcEpFree(f->controller, msg->dev.epout);
        memset(&msg->dev, 0, sizeof(msg->dev));
        if (intfnum >= 0)
            copsRemoveInterface(f->cops, (uint8_t)intfnum);
        return -ENOMEM;
    }

    msg_ep_in_desc.bEndpointAddress = msg->dev.epin->ue.address;
    msg_ep_out_desc.bEndpointAddress = msg->dev.epout->ue.address;

    msg->intf = intfnum;
    msg_intf_desc.bInterfaceNumber = intfnum;
    msg_intf_desc.iInterface = copsAssignStringId(cops, &s_msg_str);

    return 0;
}

static void _msgUnbind(copsFunc_t *f)
{
    struct f_msg *msg = f2msg(f);
	USB_LOG_TRACE("msg unbind\n");
    usbMsgDevUnbind(&msg->dev);
    if (msg->intf >= 0)
    {
        copsRemoveInterface(f->cops, msg->intf);
        msg->intf = -1;
    }

    copsRemoveString(f->cops, &s_msg_str);
    udcEpFree(f->controller, msg->dev.epin);
    udcEpFree(f->controller, msg->dev.epout);
    msg->dev.epin = NULL;
    msg->dev.epout = NULL;
}

static int _msgSetAlt(copsFunc_t *f, uint8_t intf, uint8_t alt)
{
	int r;

    struct f_msg *msg = f2msg(f);
    USB_LOG_TRACE("msg set alt\n");	
    if (intf != (uint8_t)msg->intf)
        return -EINVAL;

    udcEpDisable(f->controller, msg->dev.epin);
    udcEpDisable(f->controller, msg->dev.epout);

    r = udcEpEnable(f->controller, msg->dev.epin);
    if (r < 0)
    {
        USB_LOG_TRACE("ep %x enable fail\n", msg->dev.epin->ue.address);
        return r;
    }

    r = udcEpEnable(f->controller, msg->dev.epout);
    if (r < 0)
    {
        USB_LOG_TRACE("ep %x enable fail\n", msg->dev.epout->ue.address);
        udcEpDisable(f->controller, msg->dev.epin);
        return r;
    }

    return usbMsgDevStart(&msg->dev);
}

static void _msgDisable(copsFunc_t *f)
{
    struct f_msg *msg = f2msg(f);
    USB_LOG_TRACE("msg disable\n");

    usbMsgDevStop(&msg->dev);
    udcEpDisable(f->controller, msg->dev.epin);
    udcEpDisable(f->controller, msg->dev.epout);
}

copsFunc_t *createMassStorageFunc(uint32_t type, uint32_t name)
{
    struct f_msg *msg = (struct f_msg *)calloc(1, sizeof(struct f_msg));
    if (!msg)
    {
        USB_LOG_TRACE("msg allocate fail\n");
        return NULL;
    }

    msg->intf = -1;
    msg->dev.sdio_type = type;
    msg->dev.func = &msg->func;
    msg->func.name = name;
    msg->func.ops.fill_desc = _msgFillDesc;
    msg->func.ops.destroy = _msgDestroy;
    msg->func.ops.setup = _msgSetup;
    msg->func.ops.bind = _msgBind;
    msg->func.ops.unbind = _msgUnbind;
    msg->func.ops.set_alt = _msgSetAlt;
    msg->func.ops.disable = _msgDisable;

    return &msg->func;
}
