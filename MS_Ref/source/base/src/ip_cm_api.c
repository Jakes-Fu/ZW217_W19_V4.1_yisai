/******************************************************************************
 ** File Name:      ip_cm_api.c                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           21/02/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 channel manager thread. It codes messages from other      *
 **					threads and sends them to the serrial device, on the      *
 **					other hand, it decodes messages from serrial ports and    *
 **					send them to the corresponding threads.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 21/02/2002     Richard.Yang     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "tasks_id.h"
#include "cmddef.h"
#include "cm.h"
#include "ip_cm_api.h"
#include "sio.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

int BS_Send(char * lpData, int nSize)
{
    MSG_HEAD_T * data;
	uint32     status;

	SCI_ASSERT(lpData != SCI_NULL);/*assert verified*/

	data = (MSG_HEAD_T *) SCI_ALLOC_APP(nSize + (int)sizeof(MSG_HEAD_T));

    CM_COPY_MSG_HEAD(data, 0, (nSize + (int)sizeof(MSG_HEAD_T)), IP_PACKET_A, 0);

	memcpy((void *) (data + 1), lpData, nSize);
	
    status = SIO_SendPacket(data, nSize + (int)sizeof(MSG_HEAD_T));

    SCI_FREE(data);

	return status;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
