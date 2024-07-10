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

#ifndef _RPC_VERIFY_H_
#define _RPC_VERIFY_H_

#include <stdint.h>
#include "sci_types.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ap_pkt
{
	unsigned char head[4];
	unsigned char timestamp[4];	
	unsigned char code[144];
}ap_pkt_t;

uint8 verify_ApSide(ap_pkt_t *verifycode);
uint8 verify_CpSide(ap_pkt_t *verifycode);
uint8 verify_Cp_result(uint8 result);
uint8 AcknowledgeCPshakehands(void);
	

#ifdef __cplusplus
}
#endif
#endif
