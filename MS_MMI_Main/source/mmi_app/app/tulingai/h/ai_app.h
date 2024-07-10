#ifndef __ZDT_AI_H__
#define __ZDT_AI_H__

#include "ai_api.h"
#include "ai_net.h"
#include "ai_func.h"
#include "ai_tcp.h"


extern MMI_RESULT_E AI_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
PUBLIC int AI_AppInit(void);
extern int AI_AppFree(void);
extern int AI_RestorFactory(void);
extern AI_APP_T * AI_AppGet(void);


//============

typedef struct
{
    uint8 *        str;
    uint32         len; 
} MMI_AI_DATA_T;

typedef struct
{
    SIGNAL_VARS
    MMI_AI_DATA_T data;
} MMI_AI_SIG_T ;


/*extern void MMIZDT_AppInit(void);
extern void MMIZDT_RegWinIdNameArr(void);
extern void MMIZDT_InitModule(void);
extern void MMIZDT_RegMenuGroup(void);
extern void MMIZDT_RegNv(void);*/
extern BOOLEAN  MMIAI_SendSigTo_APP(AI_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len);



#endif
