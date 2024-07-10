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

#ifndef _USB__UDC_DWC_2_H_
#define _USB__UDC_DWC_2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_device_uws6121e.h"


#define MUSB_READ8(offset) OSI_REG8_READ(MUSB_REG_BASE + (offset))
#define MUSB_READ16(offset) OSI_REG16_READ(MUSB_REG_BASE + (offset))
#define MUSB_READ32(offset) OSI_REG32_READ(MUSB_REG_BASE + (offset))
#define MUSB_WRITE8(offset, value) OSI_REG8_WRITE(MUSB_REG_BASE + (offset), value)
#define MUSB_WRITE16(offset, value) OSI_REG16_WRITE(MUSB_REG_BASE + (offset), value)
#define MUSB_WRITE32(offset, value) OSI_REG32_WRITE(MUSB_REG_BASE + (offset), value)
#define MUSB_SET8(offset, set) MUSB_WRITE8(offset, MUSB_READ8(offset) | (set))
#define MUSB_SET16(offset, set) MUSB_WRITE16(offset, MUSB_READ16(offset) | (set))
#define MUSB_SET32(offset, set) MUSB_WRITE32(offset, MUSB_READ32(offset) | (set))
#define MUSB_CLR8(offset, clr) MUSB_WRITE8(offset, MUSB_READ8(offset) & ~(clr))
#define MUSB_CLR16(offset, clr) MUSB_WRITE16(offset, MUSB_READ16(offset) & ~(clr))

#define HAL_SYSIRQ_NUM(irq) ((irq) + 32)
//#define REG_ADDRESS_FOR_ARM
//#define REG_ACCESS_ADDRESS(addr) (addr)


#define  REGT_FIELD_CHANGE1(reg, type, f1, v1)\
    {\
    type _val;\
    _val.v = reg;\
    _val.b.f1 = v1;\
    reg = _val.v;\
    _val.v;\
  }

#define REGT_FIELD_CHANGE2(reg, type, f1,v1,f2,v2)\
    { \
    type _val;     \
    _val.v = reg;    \
    _val.b.f1 = v1;                   \
    _val.b.f2 = v2;                   \
    reg = _val.v;                              \
    _val.v;                                    \
}

#define REGT_FIELD_CHANGE3(reg, type, f1,v1,f2,v2,f3,v3) \
{ \
    type _val;     \
    _val.v = reg;       \
    _val.b.f1 = v1;                   \
    _val.b.f2 = v2;                   \
    _val.b.f3 = v3;                   \
    reg = _val.v;                              \
    _val.v;                                    \
}

void dwcUdcInit(udc_t *udc);
void DwcUdcExit(udc_t *udc);
bool dwcUsbIsSuspend(udc_t *udc);

#ifdef __cplusplus
}
#endif

#endif // _USB__UDC_DWC_2_H_
