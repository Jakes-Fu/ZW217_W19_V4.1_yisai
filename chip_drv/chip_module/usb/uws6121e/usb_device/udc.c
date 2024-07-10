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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// #include "usb/udc_platform_rda.h"
#include "udc_dwc.h"
#include "usb_platform_uws6121e.h"
#include "usb_utils.h"
#include "osi_compiler.h"
typedef struct
{
    udc_t udc;
    udevDrv_t *driver;
    void *notify_param;
    udcNotifier_t notifier;
} udcPriv_t;

char gDummyUsbRxBuf[USB_DUMMY_RX_BUF_SIZE] OSI_CACHE_LINE_ALIGNED;

static  udcPriv_t *_getPriv(udc_t *udc)
{
    return (udcPriv_t *)udc;
}

udc_t *udcCreate(uint32_t name)
{
    udcPriv_t *p = calloc(1, sizeof(udcPriv_t));
    if (p == NULL)
        return NULL;

    p->udc.feature = CONFIG_USB_DEVICE_CONTROLLER_FEATURE;
    // platform
    if (!udcPlatformInit(&p->udc))
    {
        USB_ERR_TRACE("udc platform init failed");
        free(p);
        return NULL;
    }

    // controller
    dwcUdcInit(&p->udc);

    return &p->udc;
}

void udcDestroy(udc_t *udc)
{
    if (udc == NULL)
        return;

    udcStop(udc);
    DwcUdcExit(udc);
    udcPlatformExit(udc);

    free(udc);
}

udevDrv_t *udcGetDriver(udc_t *udc)
{
    udcPriv_t *p;
    if (!udc)
        return NULL;

    p = _getPriv(udc);
    return p->driver;
}

void udcBindDriver(udc_t *udc, udevDrv_t *driver)
{
    udcPriv_t *p;
    int r;

    if (udc == NULL || driver == NULL)
        return;

    p = _getPriv(udc);
    p->driver = driver;
    driver->udc = udc;

    r = udevDriverBind(driver);
    if (r < 0)
        USB_LOG_TRACE("UDC bind driver fail");
}

void udcStart(udc_t *udc)
{
    udcPriv_t *p;
    //USB_LOG_TRACE("udcStart\n");
    if (!udc)
        return;

    if (CHECK_UDC_OPS(udc, start)){
        udc->ops.start(udc);
        return;
    }

    p = _getPriv(udc);
    if (p->driver)
        udevDriverStart(p->driver);
    USB_LOG_TRACE("udcStart done\n");
}

void udcStop(udc_t *udc)
{
    udcPriv_t *p;
    if (!udc)
        return;

    p = _getPriv(udc);
    if (p->driver)
    {
        udevDriverStop(p->driver);
        udevDriverDisconnect(p->driver);
    }

    if (CHECK_UDC_OPS(udc, stop))
        udc->ops.stop(udc);
}

void udcSetNotifier(udc_t *udc, udcNotifier_t n, void *param)
{
    udcPriv_t *p;
    uint32_t critical;
    if (!udc)
        return;

    p = _getPriv(udc);
    p->notifier = n;
    p->notify_param = param;
}
extern uint8_t g_usbrst_count;
void udcConnect(udc_t *udc)
{
    udcPriv_t *p;
    if (!udc)
        return;

    p = _getPriv(udc);
    if (p->driver)
        udevDriverConnect(p->driver);

    if (p->notifier)
        p->notifier(p->notify_param, UDC_CONNECTED);

    g_usbrst_count = 0;
}

void udcDisconnect(udc_t *udc)
{
    udcPriv_t *p;
    if (!udc)
        return;

    p = _getPriv(udc);
    if (p->driver)
        udevDriverDisconnect(p->driver);

    if (p->notifier)
        p->notifier(p->notify_param, UDC_DISCONNECT);
}

void udcSuspend(udc_t *udc)
{
    udcPriv_t *p;
    if (!udc)
        return;

    p = _getPriv(udc);
    if (p->driver)
        udevDriverSuspend(p->driver);
}

void udcResume(udc_t *udc)
{
    udcPriv_t *p;
    if (!udc)
        return;

    p = _getPriv(udc);
    if (p->driver)
        udevDriverResume(p->driver);
}

bool udcRemoteWakeup(udc_t *udc)
{
    if (!udc || !CHECK_UDC_OPS(udc, remote_wakeup))
        return false;

    return udc->ops.remote_wakeup(udc);
}

bool udcUsbIsSuspend(udc_t *udc)
{
    if(udc == NULL)
        return false;
    return dwcUsbIsSuspend(udc);
}