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

#ifndef _HWREG_ACCESS_H_
#define _HWREG_ACCESS_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

//#ifdef CONFIG_SOC_8850
//#define HAL_SYSIRQ_NUM(irq) ((irq) + 32)
//#define REG_ADDRESS_FOR_ARM
#define REG_ACCESS_ADDRESS(addr) (addr)
//#endif

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

#endif
