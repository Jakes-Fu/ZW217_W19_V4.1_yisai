#ifndef  _MV_TIMER_API
#define  _MV_TIMER_API
#include "_Types.h"
#include "_Timer.h"

#define MV_STACK_SIZE         (1024*80)
#define MV_SOCKET_STACK_SIZE  (1024*20)



typedef struct  {

 _HANDLE mv_handle; 
 
 _VOID* mv_pParam ;
 
 TimerCallbackProc  mv_callback;
 
 }timer_param;



void CM_CreateTimer(_INT nElapse);

_HANDLE CM_SetTimer(_INT nElapse,TimerCallbackProc pTimerProc,_VOID* pParam);

_BOOLEAN CM_CancelTimer(_HANDLE hTimer);

/*****************************************************************************/
//  Description : MV_GetFreqHandler
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MV_GetFreqHandler(void);

/*****************************************************************************/
//  Description : MV_SetFreqHandler
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_SetFreqHandler(uint32 req_handler);

/*****************************************************************************/
//  Description : mv_init
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_init(uint32 nElapse);

/*****************************************************************************/
// 	Description : get mv task id
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MV_GetMvTaskID(void);

/*****************************************************************************/
// 	Description : get mv socket task id
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 MV_GetMVSocketTaskID(void);

/*****************************************************************************/
// 	Description : mobile video init
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MV_inittask(void);
#endif

