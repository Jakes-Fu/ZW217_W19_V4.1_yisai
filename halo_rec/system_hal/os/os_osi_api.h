#ifndef OS_OSI_API_H
#define OS_OSI_API_H

#include "_os_osi_api.h"
#include "sxs_io.h"
#include "pccdef.h"
#include "platform_common.h"

typedef T_osi_HANDLE        HALO_OSI_HANDLE_T;
typedef T_osi_STATUS        HALO_OSI_STATUS_T;
//typedef int                 boolean;
typedef unsigned char		BOOLEAN;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
typedef unsigned int		uint;


typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;

#define IN
#define OUT
#define INOUT
//for osi CP 



enum
{
  ERR_COMMON = -1,
  ERR_TIMEOUT1 = -2,
  ERR_TIMEOUT2 = -3
};

typedef struct _auth_cmd_req
{
    uint authId;             // auth ipc id
    uint cmdId;            // cmd type
    uint para0;
    uint para1;
    uint para2;
}AUTH_CMD_REQ;

typedef struct _ap_pkt
{
	unsigned char head[4];
	unsigned char timestamp[4];
	unsigned char code[144];
}ap_pkt_t;

#define PUBLIC
#define PRIVATE
#define LOCAL static
#define TOKEN_LEN (152)
#define MAGIC_VAL 0x35279528
#define AUTH_STACKSIZE  1024*4
#define AUTH_PRIORITY  250
#define AUTH_TIMESLICE  20
#define AUTH_MESSAGE_QUEUE_LEN 8
#define SECOND  1000

#define HALO_FIFO OSI_FIFO
#define HALO_PREEMPT OSI_PREEMPT
#define HALO_START OSI_START
#define HALO_STACK_CORE OSI_STACK_CORE
#define HALO_INVALID_HANDLE OSI_INVALID_HANDLE
#define HALO_SUSPEND OSI_SUSPEND
#define HALO_NO_SUSPEND OSI_NO_SUSPEND

#define AUTH_CMD_VERIFYAP         1
#define AUTH_CMD_VERIFYCP         2
#define AUTH_CMD_VERICPRET        3
#define AUTH_ID    0xF9
//for osi CP 
#define EFUSE_CHK_DEBUG


#ifdef EFUSE_CHK_DEBUG
#define DEBG 1
#else
#define DEBG 0
#endif

#define PRTF(pFormat, ...) do	\
			{ \
				if(DEBG){			\
					sxs_fprintf((uint)12, (const ascii *)"@@@@auth Line[%d]   "pFormat,__LINE__,##__VA_ARGS__);  \
				}	\
			}while(0)

		
	
#define __BTOW(a,b,c,d) (uint)(((uint)(a)<<24) | (uint)(b)<<16 | ((uint)(c)<<8) | ((uint)(d)))

//for osi CP 
#define HALO_GetTickCount() SCI_GetTickCount()
#define HALO_GetTickTime(t) SCI_GetTickTime(&t)
#define HALO_ALLOCA(_SIZE) SCI_ALLOCA(_SIZE)
#define HALO_Malloc(size) SCI_ALLOC(size)
#define HALO_Free(ptr) SCI_FREE(ptr)
#define HALO_Memcpy(_DEST_PTR, _SRC_PTR, _SIZE)  SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE)
#define HALO_Memset(_DEST_PTR, _VALUE, _SIZE) SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE)
#define HALO_AUTH_MEMALLOC(PTR,SIZE,SUSPEND) osi_allocate_block_memory(PTR,SIZE,SUSPEND)
#define HALO_AUTH_MEMFREE(PTR) osi_free_block_memory(PTR)
#define HALO_AUTH_CREATEMESSAGE(PTR,NAME,SIZE,TYPE)  osi_create_message(PTR,NAME,SIZE,TYPE)
#define HALO_AUTH_CREATETASK(PTR,NAME,ENTRY,ARGC,ARGV,STACKSIZE,PRIO,TSLICE,PREEMPT,AUTOSTART,STACKFLAG) osi_create_task(PTR, NAME,ENTRY, 0, NULL, STACKSIZE, PRIO, TSLICE, PREEMPT, AUTOSTART, STACKFLAG)
#define HALO_AUTH_CREATESEMA(PTR,NAME,COUNT,SUSPEND)  osi_create_semaphore(PTR,NAME,COUNT,SUSPEND)
#define HALO_AUTH_RECIEVEMESSAGE(HANDLE,MESSAGE,SIZE,ACTSIZE,SUSPEND) osi_receive_message(HANDLE,MESSAGE,SIZE,ACTSIZE,SUSPEND)
#define HALO_AUTH_SENDMESSAGE(HANDLE,MESSAGE,SIZE,SUSPEND) osi_send_message(HANDLE,MESSAGE,SIZE,SUSPEND)
#define HALO_AUTH_OBTAINSEMA(HANDLE,SUSPEND) osi_obtain_semaphore(HANDLE,SUSPEND)
#define HALO_AUTH_RELEASESEMA(HANDLE) osi_release_semaphore(HANDLE)

//for osi CP 

#endif