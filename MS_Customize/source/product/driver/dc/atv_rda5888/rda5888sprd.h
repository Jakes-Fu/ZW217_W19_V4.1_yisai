// ---------------------------------------------------------------------------
// Copyright 2009-2010 (c) RDA Microelectronics, Inc.
//
// File: rda5888sprd.h
// Revision: 0
// Author: wuhp
// Date: 2010-01-27
// Description: 
// ---------------------------------------------------------------------------

#ifndef __RDA5888SPRD_H__
#define __RDA5888SPRD_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "rda5888app.h"
#include "os_api.h"

extern SCI_TIMER_PTR g_ptRdaAtvTimer;

void RDAEXT_DelayMs(uint32 ms);

#if defined(SPRD_CUSTOMER)
#define  RDAEXT_DebugPrint    SCI_TRACE_LOW
#else
#define  RDAEXT_DebugPrint
#endif

void RDAAPP_InitVideoSize(BOOLEAN IsFullScreen);

PUBLIC void RDA5888_SPRD_Enter(void);
PUBLIC void RDA5888_SPRD_Leave(void);
PUBLIC void RDAATV_InitScanParam(int region);
PUBLIC uint32 RDAATV_ScanChn(uint32* chn_id, uint32* is_valid);

#ifdef __cplusplus
}
#endif

#endif

