/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

//This function is used by bt_pal_extension.c when sending messages to WiFi. It is defined here to avoid dependency on CSR_LOG_ENABLE in the BT LIB

void CsrBt2WiFiSchedMessagePut(CsrSchedQid q, CsrUint16 mi, void *mv)
{
	CsrSchedMessagePut(q, mi, mv);
}


