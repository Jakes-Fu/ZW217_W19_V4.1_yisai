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

#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('C', 'O', 'P', 'S')
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_INFO

#include "usb_ch9.h"
#include "usb_composite_device.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//#include <osi_api.h>
//#include <osi_log.h>
//#include <sys/errno.h>

#include "usb_drv.h"
#include "usb_utils.h"

#define COPS_DEFAULT_VID USB_VID_RDA_DOWNLOAD // 0x0525
#define COPS_DEFAULT_PID USB_PID_RDA_DOWNLOAD // 0xa4a7

typedef TAILQ_HEAD(, composite_func_s) copsFuncHead_t;

#define COPS_MAX_FUNCTION_COUNT 10
#define COPS_MAX_INTERFACE_COUNT 64
#define COPS_MAX_STRING_COUNT 64
#define COPS_CONFIG_MAX_SIZE 512
#define COPS_CTRL_BUFFER_SIZE 512
struct composite_device_s
{
    udevDrv_t driver; ///< keep first member
    usb_device_descriptor_t device_desc;
    usbEp_t *ep0;
    bool configed;
    uint8_t n_intf;
    uint8_t n_func;
    uint16_t vid;
    uint16_t pid;
    copsFuncHead_t funcs;
    copsFunc_t *intf_func_map[COPS_MAX_INTERFACE_COUNT];
    const usbString_t *string_map[COPS_MAX_STRING_COUNT];
    uint8_t config_desc[COPS_CONFIG_MAX_SIZE];
    copsCtrl_t ctrl;
};

#define COPS_DEV_MANUFACTURER 0
#define COPS_DEV_PRODUCT 1
static usbString_t cops_string_defs[] = {
    {
      COPS_DEV_MANUFACTURER,
      0,
      "UNISOC",
    },
    {
      COPS_DEV_PRODUCT,
      0,
      "UNISOC-uws6121e",
    },
};
	  

static  cops_t *_getCops(udevDrv_t *drv)
{
    return (cops_t *)drv;
}

static  uint8_t *_configBuffer(cops_t *c)
{
    return &c->config_desc[0];
}

static  usb_config_descriptor_t *_configDesc(cops_t *c)
{
    return (usb_config_descriptor_t *)c->config_desc;
}

static int _copsSetupOther(udevDrv_t *driver, const usb_device_request_t *ctrl)
{
    SCI_TraceLow("set other, not support yet");
    return -1;
}

static uint32_t _copsGenConfigDesc(cops_t *c, void *buf, uint32_t size)
{
    usb_config_descriptor_t cfg, *config;
    copsFunc_t *f;
    uint32_t ret;
    cfg.bLength = USB_DT_CONFIG_SIZE;
    cfg.bDescriptorType = USB_DT_CONFIG;
    cfg.wTotalLength = cfg.bLength;
    cfg.bNumInterfaces = c->n_intf;
    cfg.bConfigurationValue = 1;
    cfg.iConfiguration = 0;
    cfg.bmAttributes = (c->driver.udc->feature & UDC_FEATURE_ATTRIBUTE_MASK);
    cfg.bMaxPower = 0xC8; // 400 mA
    if (size <= USB_DT_CONFIG_SIZE)
    {
        memcpy(buf, &cfg, size);
        return size;
    }

    config = (usb_config_descriptor_t *)buf;
    *config = cfg;
    size -= config->wTotalLength;

    TAILQ_FOREACH(f, &c->funcs, anchor)
    {
        if (size <= 0)
            break;
        ret = cfFillDesc(f, (uint8_t *)buf + config->wTotalLength, size);
        config->wTotalLength += ret;
        size -= ret;
    }

    return config->wTotalLength;
}

static void _copsResetConfig(cops_t *c)
{
    copsFunc_t *f;
    USB_LOG_TRACE("cops reset config\n");

    TAILQ_FOREACH(f, &c->funcs, anchor)
    {
        cfDisable(f);
    }
}

static int _copsSetConfig(cops_t *c, uint16_t num)
{

    usb_config_descriptor_t *cfg = PNULL;
    copsFunc_t *f = PNULL;
    uint8_t i = 0;
    int r = 0;

    USB_LOG_TRACE("cops set config %u", num);

    cfg = _configDesc(c);
	
    if(cfg == PNULL)
        goto fail;
	
    if (c->configed && num == cfg->bConfigurationValue)
        return 0;

    // Reset previous configurations then config current;
    // num == 0 may means no proper configuration
    if (num == 0 || num != cfg->bConfigurationValue)
    {
        if (c->configed)
            _copsResetConfig(c);
        c->configed = false;
        return 0;
    }

    for ( i = 0; i < COPS_MAX_INTERFACE_COUNT; ++i)
    {
        f = c->intf_func_map[i];
        if (f != NULL)
        {
            r = cfSetAlt(f, i, 0);
            if (r < 0)
            {
                SCI_TraceLow( "Cops set alt fail. f %p i %d", f, i);
                _copsResetConfig(c);
                goto fail;
            }
        }
    }

    c->configed = true;
    return 0;

fail:
    USB_ERR_TRACE("cops set config %u failed", num);
    return -1;
}

static int _copsSetInterface(cops_t *c, uint16_t index, uint16_t value)
{
    copsFunc_t *f;
    int result;
    if (!c->configed || index > COPS_MAX_FUNCTION_COUNT)
    {
        USB_LOG_TRACE("cops set interface fail, invalid index %u", index);
        return -1;
    }
   USB_LOG_TRACE("cops set interface %u/%u", index, value);

   f = c->intf_func_map[index];
    if (f == NULL)
    {
        SCI_TraceLow("cops set interface, not register %u", index);
        return -1;
    }

    result = cfSetAlt(f, index, value);
    if (result < 0)
    {
       SCI_TraceLow("cops set interface fail %u/%u/%d", index, value, result);
        return result;
    }
    return 0;
}

static unsigned _ascii2Unicode(const char *ascii, char *unicode, unsigned unicode_len)
{
    unsigned i;
    unsigned len = strlen(ascii);
    TM_SendTestPointRequest (0xbbbb0222, unicode_len);
    memset(unicode, 0, unicode_len);
    for (i = 0; i < len && i < unicode_len / 2; ++i)
        unicode[i * 2] = ascii[i];
    return i * 2;
}

static const char *_copsIdString(cops_t *c, uint16_t language, uint8_t id)
{
    if (id > OSI_ARRAY_SIZE(c->string_map) || id <= 0)
        return NULL;

    return c->string_map[id - 1]->s;
}

static int _copsGetString(cops_t *c, uint16_t language, uint8_t id, char *buf, uint32_t length)
{
    const char *str;
    int r;
    usb_string_descriptor_t *desc = (usb_string_descriptor_t *)buf;
    desc->bDescriptorType = USB_DT_STRING;

    if (c==NULL || length < 2)
        return 0;

    if (length > 255) //USB_DT_STRING max 255(on win10/win7)
        length = 255;

    if (id == 0)
    {
        desc->bLength = OSI_MIN(uint32_t, length, 4);
        language = 0x0409;
        memcpy(desc->wData, &language, desc->bLength - 2);
        return desc->bLength;
    }

    str = _copsIdString(c, language, id);
    if (str)
    {
        desc->bLength = strlen(str) * 2 + 2;
        r = _ascii2Unicode(str, (char *)&desc->wData[0], length - 2);
        return (r + 2);
    }

    return -1;
}

static void _deviceQualifier(cops_t *c, void *buf, unsigned size)
{
    usb_device_qualifier_t desc;
        desc.bLength = USB_DT_DEVICE_QUALIFIER_SIZE,
        desc.bDescriptorType = USB_DT_DEVICE_QUALIFIER,
        desc.bcdUSB = c->device_desc.bcdUSB,
        desc.bDeviceClass = c->device_desc.bDeviceClass,
        desc.bDeviceSubClass = c->device_desc.bDeviceSubClass,
        desc.bDeviceProtocol = c->device_desc.bDeviceProtocol,
        desc.bMaxPacketSize0 = c->device_desc.bMaxPacketSize0,
        desc.bNumConfigurations = c->device_desc.bNumConfigurations,
        desc.bRESERVED = 0,
    memcpy(buf, &desc, size);
}

static void prvDumpSetup(const usb_device_request_t *ctrl)
{
    SCI_TraceLow("cops dump setup %x %x %x %x %x",
             ctrl->bRequestType, ctrl->bRequest,
             ctrl->wValue, ctrl->wIndex, ctrl->wLength);
}

static int _copsSetupStandard(udevDrv_t *driver, const usb_device_request_t *ctrl)
{
    cops_t *c = _getCops(driver);
    int result;
    usbXfer_t *x;

    uint32_t w_length = ctrl->wLength;
    uint32_t w_value = ctrl->wValue;
    uint32_t w_index = ctrl->wIndex;

    x = c->ctrl.xfer;
    x->zlp = 1;
    x->param = c;
    x->length = 0;
    x->buf = c->ctrl.buffer;
    x->complete = NULL;
	
    switch (ctrl->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:
        switch (w_value >> 8)
        {
        case USB_DT_DEVICE:
            x->length = OSI_MIN(uint32_t, w_length, USB_DT_DEVICE_SIZE);
            memcpy(x->buf, &c->device_desc, x->length);
            break;

        case USB_DT_DEVICE_QUALIFIER:
            x->length = OSI_MIN(uint32_t, w_length, USB_DT_DEVICE_QUALIFIER_SIZE);
            _deviceQualifier(c, x->buf, x->length);
            break;

        case USB_DT_CONFIG:
        case USB_DT_OTHER_SPEED_CONFIG:
            x->length = OSI_MIN(uint32_t, w_length, _configDesc(c)->wTotalLength);
            memcpy(x->buf, _configBuffer(c), x->length);
            break;

        case USB_DT_STRING:
            result = _copsGetString(c, w_index, w_value & 0xff, x->buf, w_length);
            if (result < 0)
                return -1;
            x->length = result;
            break;

        default:
            return -1;
        }
        break;

    case USB_REQ_SET_CONFIGURATION:
        return _copsSetConfig(c, ctrl->wValue);

    case USB_REQ_SET_INTERFACE:
        if (ctrl->bRequestType != USB_RECIP_INTERFACE)
        {
            SCI_TraceLow("cops set interface invalid");
            prvDumpSetup(ctrl);
            return -1;
        }
        return _copsSetInterface(c, ctrl->wIndex, ctrl->wValue);

    default:
        return -1;
    }

    if (x->length != 0)
    {
        if (x->length > c->ctrl.bufsize)
        {
            SCI_TraceLow("cops ctrl buffer overflow, %u/%u", x->length, c->ctrl.bufsize);
            SCI_ASSERT(0);
        }

        // do not set zlp if actual_send == setup->w_length
        if (x->length == w_length)
            x->zlp = 0;
		//Debug_Uart4_puts("udcEpQueue\n");
        return udcEpQueue(c->driver.udc, c->driver.udc->ep0, x);
    }

    return 0;
}

static int _copsSetup(udevDrv_t *driver, const usb_device_request_t *ctrl)
{

    cops_t *c = _getCops(driver);
    copsFunc_t *f;
    uint8_t req_type = ctrl->bRequestType & USB_TYPE_MASK;
    uint8_t recip = ctrl->bRequestType & USB_RECIP_MASK;
    uint16_t index = ctrl->wIndex;
    int retval = -EOPNOTSUPP;	
/*
	USB_LOG_TRACE("_copsSetup - bRequestType: 0x%x, bRequest: 0x%x,"    \
                         "wValue: 0x%x, wIndex: 0x%x, wLength: 0x%x\n", \
             ctrl->bRequestType,	\
             ctrl->bRequest, ctrl->wValue, ctrl->wIndex, ctrl->wLength);
*/
    TM_SendTestPointRequest (0xbbbb0390, ctrl->wLength);

    switch (req_type)
    {
    case USB_TYPE_STANDARD:
        retval = _copsSetupStandard(driver, ctrl);
        break;

    case USB_TYPE_CLASS:
        switch (recip)
        {
        case USB_RECIP_INTERFACE:
            if (index < COPS_MAX_INTERFACE_COUNT && c->intf_func_map[index])
            {
                f = c->intf_func_map[index];
                retval = cfSetup(f, ctrl);
                if (retval >= 0)
                {
                    break;
                }
            }

        default:
            prvDumpSetup(ctrl);
        }
        break;

    default:
        retval = _copsSetupOther(driver, ctrl);
        break;
    }

    return retval;
}

static int _copsBind(udevDrv_t *driver)
{
    cops_t *c = _getCops(driver);

	USB_LOG_TRACE("_copsBind	udc ctrl Xfer Alloc\n");

    if (c->ctrl.xfer == NULL)
        c->ctrl.xfer = udcXferAlloc(driver->udc);
    return c->ctrl.xfer != NULL ? 0 : -1;
}

static void _copsUnbind(udevDrv_t *driver)
{
    cops_t *c;
	USB_LOG_TRACE("_copsUnbind udc ctrl Xfer free\n");

    if (driver->udc)
    {
        c = _getCops(driver);
        if (c->ctrl.xfer)
        {
            udcXferFree(driver->udc, c->ctrl.xfer);
			
            c->ctrl.xfer = NULL;
        }
    }
}

static void _copsDisconnect(udevDrv_t *driver)
{
    cops_t *c = _getCops(driver);
    USB_LOG_TRACE("cops disconnect \n");

    if (c->configed)
        _copsResetConfig(c);
    c->configed = false;
}

static void _copsConnect(udevDrv_t *driver)
{
    unsigned total;
    cops_t *c = _getCops(driver);
    USB_LOG_TRACE("cops connect \n");


    total = _copsGenConfigDesc(c, _configBuffer(c), COPS_CONFIG_MAX_SIZE);

}
static void _copsSuspend(udevDrv_t *driver)
{
	copsFunc_t *f;
    cops_t *c = _getCops(driver);
    USB_LOG_TRACE("cops suspend \n");

	TAILQ_FOREACH(f, &c->funcs, anchor)
	{
		cfSuspend(f);
	}

}
static void _copsResume(udevDrv_t *driver)
{
	copsFunc_t *f;
    cops_t *c = _getCops(driver);
    USB_LOG_TRACE("cops disconnect \n");

	TAILQ_FOREACH(f, &c->funcs, anchor)
	{
		cfResume(f);
	}

}


static void _copsGadgetInit(udevDrv_t *drv)
{
    drv->ops.bind = _copsBind;
    drv->ops.unbind = _copsUnbind;
    drv->ops.setup = _copsSetup;
    drv->ops.connect = _copsConnect;
    drv->ops.disconnect = _copsDisconnect;
	drv->ops.suspend = _copsSuspend;
    drv->ops.resume = _copsResume;
}

int copsAssignStringId(cops_t *c, usbString_t *ustr)
{
    uint8_t id = 0;
    unsigned i;
    if (c == NULL || ustr == NULL)
        return 0;
    if (ustr->id == 0)
    {

        for ( i = 0; i < OSI_ARRAY_SIZE(c->string_map); ++i)
        {
            if (c->string_map[i] == NULL)
            {
                c->string_map[i] = ustr;
                id = i + 1;
                break;
            }
        }
        ustr->id = id;
    }

    if (ustr->id)
        ustr->ref_count += 1;

    return ustr->id;
}

int copsRemoveString(cops_t *c, usbString_t *ustr)
{
    if (c == NULL || ustr == NULL || ustr->id == 0)
        return 0;

    if (ustr->ref_count == 0)
    {
        USB_LOG_TRACE("Remove usb string no ref count, id %x", ustr->id);
        ustr->id = 0;
    }
    else
    {
        ustr->ref_count -= 1;
    }

    if (ustr->ref_count == 0)
    {
        if ((ustr->id - 1) >= 0)
            c->string_map[ustr->id - 1] = NULL;
        ustr->id = 0;
    }
    return 0;
}

static void _copsDeviceInit(cops_t *c)
{
    usb_device_descriptor_t *dev = &c->device_desc;

    dev->bLength = USB_DT_DEVICE_SIZE;
    dev->bDescriptorType = USB_DT_DEVICE;
    dev->bcdUSB = 0x0200;
    dev->bDeviceClass = USB_CLASS_PER_INTERFACE;
    dev->bDeviceSubClass = 0;
    dev->bDeviceProtocol = 0;
    dev->bMaxPacketSize0 = 64;
    dev->idVendor = c->vid;
    dev->idProduct = c->pid;
    dev->bcdDevice = 0x01;
    dev->iManufacturer = copsAssignStringId(c, &cops_string_defs[COPS_DEV_MANUFACTURER]);
    dev->iProduct = copsAssignStringId(c, &cops_string_defs[COPS_DEV_PRODUCT]);
    dev->iSerialNumber = 0,
    dev->bNumConfigurations = 1;
}

static void _updateDeviceDesc(cops_t *c)
{
    usb_device_descriptor_t *dev = &c->device_desc;
    dev->idVendor = c->vid;
    dev->idProduct = c->pid;
}

cops_t *copsCreate()
{
    const unsigned allocsize = sizeof(cops_t) + COPS_CTRL_BUFFER_SIZE + CONFIG_CACHE_LINE_SIZE;
    cops_t *c = (cops_t *)calloc(1, allocsize);
    if (c == NULL){
		USB_LOG_TRACE("copsCreate fail \n");
        return NULL;
    	}

    c->ctrl.buffer = (uint8_t *)OSI_ALIGN_UP((uint8_t *)c + sizeof(*c), CONFIG_CACHE_LINE_SIZE);
    c->ctrl.bufsize = COPS_CTRL_BUFFER_SIZE;
    c->vid = COPS_DEFAULT_VID;
    c->pid = COPS_DEFAULT_PID;
    TAILQ_INIT(&c->funcs);
    _configDesc(c)->wTotalLength = 0;
    _copsDeviceInit(c);
    _copsGadgetInit(&c->driver);
    return c;
}

void copsSetVendorId(cops_t *c, uint16_t vid)
{
    if (c)
    {
        c->vid = vid;
        _updateDeviceDesc(c);
    }
}

void copsSetProductId(cops_t *c, uint16_t pid)
{
    if (c)
    {
        c->pid = pid;
        _updateDeviceDesc(c);
    }
}

void copsDestroy(cops_t *c)
{
    if (c == NULL)
        return;
    copsRemoveAllFunction(c);
    free(c);
}

bool copsAddFunction(cops_t *c, copsFunc_t *f)
{
    int r;
    if (c == NULL || f == NULL)
        return false;

    if (c->n_func == COPS_MAX_FUNCTION_COUNT)
    {
        SCI_TraceLow("function count had reached the maximum, %d", COPS_MAX_FUNCTION_COUNT);
        return false;
    }

    f->cops = c;
    f->controller = c->driver.udc;
    r = cfBind(f, c, c->driver.udc);
    if (r < 0)
    {
        f->cops = NULL;
        f->controller = NULL;
        SCI_TraceLow("bind function %4c fail", f->name);
        return false;
    }

    TAILQ_INSERT_TAIL(&c->funcs, f, anchor);
    c->n_func += 1;
    return true;
}

bool copsAddFunctions(cops_t *c, copsFunc_t **funcs, unsigned count)
{
    copsFuncHead_t tmp_funcs;
    bool result = true;
    copsFunc_t *f, *tmp;
    unsigned i ;
     int r ;
    if (c == NULL || funcs == NULL || count == 0)
        return false;

    if (c->n_func + count > COPS_MAX_FUNCTION_COUNT)
    {
        SCI_TraceLow("too many functions, %d/%d", c->n_func + count, COPS_MAX_FUNCTION_COUNT);
        return false;
    }


    TAILQ_INIT(&tmp_funcs);

    for ( i = 0; i < count; ++i)
    {
        f = funcs[i];
        f->cops = c;
        f->controller = c->driver.udc;
        r = cfBind(f, c, c->driver.udc);
        if (r < 0)
        {
           SCI_TraceLow("bind function %4c fail", f->name);
            result = false;
            break;
        }
        else
        {
            TAILQ_INSERT_TAIL(&tmp_funcs, f, anchor);
        }
    }


    TAILQ_FOREACH_SAFE(f, &tmp_funcs, anchor, tmp)
    {
        TAILQ_REMOVE(&tmp_funcs, f, anchor);
        if (result)
        {
            TAILQ_INSERT_TAIL(&c->funcs, f, anchor);
            c->n_func += 1;
        }
        else
        {
            cfUnbind(f);
            f->cops = NULL;
            f->controller = NULL;
        }
    }

    return result;
}

void copsRemoveAllFunction(cops_t *c)
{
    copsFunc_t *f;
    if (c == NULL)
        return;


    while ((f = TAILQ_FIRST(&c->funcs)))
    {
        TAILQ_REMOVE(&c->funcs, f, anchor);
        cfDisable(f);
        cfUnbind(f);
        cfDestroy(f);
    }

    c->n_intf = 0;
    c->n_func = 0;
    _configDesc(c)->bNumInterfaces = 0;
    _configDesc(c)->wTotalLength = USB_DT_CONFIG_SIZE;
}

udevDrv_t *copsGetGadgetDriver(cops_t *c)
{
    return c ? &c->driver : NULL;
}

int copsAssignInterface(cops_t *c, copsFunc_t *f)
{
    uint8_t i;
    if (c == NULL)
        return -1;

    for ( i = 0; i < COPS_MAX_INTERFACE_COUNT; ++i)
    {
        if (c->intf_func_map[i] == NULL)
        {
            c->intf_func_map[i] = f;
            c->n_intf += 1;
            return i;
        }
    }

    return -1;
}

void copsRemoveInterface(cops_t *c, uint8_t index)
{
    if (!c || index >= 16 || !c->intf_func_map[index])
        return;

    c->intf_func_map[index] = NULL;
    c->n_intf -= 1;
}

copsCtrl_t *copsGetCtrl(cops_t *c)
{
    if (c && c->ctrl.xfer)
        return &c->ctrl;
    return NULL;
}
