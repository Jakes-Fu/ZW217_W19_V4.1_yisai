#ifndef OS_MOCOR_API_H
#define OS_MOCOR_API_H
#include "sci_api.h"
#include "os_api.h"

#define IN
#define OUT
#define INOUT

#define PUBLIC
#define PRIVATE
#define LOCAL static
//#define LOCAL 
typedef int                 boolean;
typedef unsigned char		BOOLEAN;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;
typedef unsigned int		uint;


typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;

/*
typedef struct _halo_rpc_signal_tag
{
    xSignalHeaderRec   sig; 
}HALO_RPC_SIGNAL_T;
*/
typedef xSignalHeaderRec HALO_RPC_SIGNAL_T;
typedef SCI_TMID HALO_TMID; 
typedef SCI_TICK_TIME_T HALO_TICK_TIME_T; 

typedef struct halo_lic 
{
	uint32 Module_id;
	uint8* name;
    uint8 (* verify_License)(void);
	uint8 (* Get_featureList)(uint8* F_list);
}HALO_LICENSE_MODULE_T;

typedef struct halo_factory
{
	uint32 Module_id;
	uint8* name;
    uint8 (* verify_License)(void);
	uint8 (* Get_featureList)(uint8* F_list);
}HALO_FACTORY_MODULE_T;

typedef struct halo_apcp
{
	uint32 Module_id;
	uint8* name;
    int32 (* apcpverify_init)(void);
    int32 (* apcpverify_deinit)(void);
	uint8 (*is_apcpverify_finished)(void);
}HALO_APCP_MODULE_T;

typedef struct class_data
{
    const uint32 Module_id;
	volatile uint8 calib_mode;
    volatile uint8 padding;
}HALO_CLASS_DATA_T;


#define TRUE   1
#define FALSE  0
#define SCI_TRUE                    TRUE       // Boolean true value
#define SCI_FALSE                   FALSE       // Boolean false value

//for mocor AP 
#define MODULE_ID   0x45355436
#define MAGIC_VAL   0x35279528
#define MAGICQ_VAL	0x153DEAD9
#define TOKEN_LEN (152)
#define HEAD_LEN	4
#define RAND_LEN 16 
#define TS_LEN	4
#define HSRD_LEN 20
#define THRD_LEN 24
#define ETHRD_LEN 128
#define START_DELAY_MS  12*1000
#define HALO_INIT_TIMEOUT_MS  15*1000
#define HALO_APCPVERIFY_TIMEOUT_MS  5*1000
#define HALO_SLEEP_MS 3*1000
#define HALO_STACKSIZE  4*1024
#define HALO_QUEUESIZE  32
#define HALO_PRIORITY  250
#define HALO_NO_PREEMPT SCI_NO_PREEMPT              
#define HALO_PREEMPT SCI_PREEMPT
#define HALO_NO_ACTIVATE SCI_NO_ACTIVATE
#define HALO_AUTO_ACTIVATE SCI_AUTO_ACTIVATE
#define HALO_DONT_START              SCI_DONT_START
#define HALO_AUTO_START              SCI_AUTO_START
#define HALO_NULL                    SCI_NULL
#define HALO_INVALID_BLOCK_ID        SCI_INVALID_BLOCK_ID
#define HALO_TMID     SCI_TMID
#define HALO_SUCCESS SCI_SUCCESS

#define HALOMAIN_STACKSIZE  1024
#define HALOMAIN_QUEUESIZE  32
#define HALOMAIN_PRIORITY  20
#define RAND_MAX ((1U << 31) - 1)
#define SECONDS(N) (N*1000)
#define STRFY(X) #X
//for mocor AP 
#define EFUSE_CHK_DEBUG

#ifdef EFUSE_CHK_DEBUG
#define DEBG 1
#else
#define DEBG 0
#endif
extern void Printf_Buf(uint8* str, uint8* address, int len);
#define PRTF_BLOCK(_EXP,PTR,LEN) Printf_Buf(_EXP,PTR,LEN)


#define PRTF(x...) do	\
			{ \
				if(DEBG){			\
					SCI_TRACE_LOW("@@@@HALO function[%s] ",__func__);	\
					SCI_TRACE_LOW("@@@@HALO "x);		\
				}	\
			}while(0)

#define PRTF_SILENCE(x...) do	\
			{ \
				if(DEBG){			\
					SCI_TRACE_LOW("@@@@HALO "x);		\
				}	\
			}while(0)

#define __BTOW(a,b,c,d) (uint32_t)(((uint32_t)(a)<<24) | (uint32_t)(b)<<16 | ((uint32_t)(c)<<8) | ((uint32_t)(d)))

//for mocor AP 
#define HALO_CreateTimer(STR_PTR,FUC,PARAM,TICK,ACTIVE) SCI_CreateTimer(STR_PTR,FUC,PARAM,TICK,ACTIVE)
#define HALO_IsTimerActive(ID) SCI_IsTimerActive(ID)
#define HALO_DeactiveTimer(ID) SCI_DeactiveTimer(ID)
#define HALO_ActiveTimer(ID)  SCI_ActiveTimer(ID)
#define HALO_ChangeTimer(ID,FUC,TICK) SCI_ChangeTimer(ID,FUC,TICK)
#define HALO_DeleteTimer(ID)  SCI_DeleteTimer(ID)
#define HALO_Sleep(tick) SCI_Sleep(tick)
#define HALO_GetTickCount() SCI_GetTickCount()
#define HALO_GetTickTime(t) SCI_GetTickTime(&t)
#define HALO_ALLOCA(_SIZE) SCI_ALLOCA(_SIZE)
#define HALO_Malloc(size) SCI_ALLOC(size)
#define HALO_MallocAPP(size) SCI_ALLOC_APP(size)
#define HALO_Free(ptr) SCI_FREE(ptr)
#define HALO_Memcpy(_DEST_PTR, _SRC_PTR, _SIZE)  SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE)
#define HALO_Memset(_DEST_PTR, _VALUE, _SIZE) SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE)
#define HALO_CreateThread(n,qn,entry,argc,argv,ss,qnum,prio,preepmt, ar) SCI_CreateThread(n,qn,entry,argc,argv,ss,qnum,prio,preepmt, ar)
#define HALO_SendSignal(ptr,id) SCI_SendSignal(ptr,id)
#define HALO_GetSignal(id) SCI_GetSignal(id)
#define HALO_ASSERT(_EXP) SCI_ASSERT(_EXP)
#define HALO_PASSERT(_EXP,PRINT_STR) SCI_PASSERT(_EXP,PRINT_STR)
//for mocor AP 

#endif