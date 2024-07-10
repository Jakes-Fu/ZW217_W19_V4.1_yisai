#ifndef BRWENG_TF2HZ_EMAIL_H
#define BRWENG_TF2HZ_EMAIL_H
#include "os_api.h"
#include "tasks_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   SIGNAL_VARS
   void    *pMsg;
}MMIEMAIL_SIG_T;

enum
{
   BRWENG_EMAIL_INVALIED = 0,

   BRWENG_EMAIL_MMI_APP,
   BRWENG_EMAIL_MMS_ENG,

   BRWENG_EMAIL_COUNT,
   BRWENG_EMAIL_END = 0x7fffffff
};

enum
{
	BRWENG_EVT_INVALIED = 0,
	BRWENG_EVT_RESUME   = 0x201,
	BRWENG_EVT_DEQUEUE_TRANS,
	BRWENG_EVT_ENQUEUE_TRANS,
	BRWENG_EVT_NEW_PUSH,

	BRWENG_EVT_COUNT,
	BRWENG_EVT_END = 0x7fffffff
};

unsigned int BRWENG20_CreateEmailTaskThread(void);
unsigned int BRWENG20_DeleteEmailTaskThread(void);
unsigned int BRWENG20_0EmailEventInterface(int vAppType , int vEventType, void* pMsg, void *hWnd);
 
#define TS_HZ_EMAIL_EVENT_CALLBACK(vAppType ,vEventType,pMsg,hWnd) BRWENG20_0EmailEventInterface(vAppType ,vEventType,pMsg, hWnd)

#ifdef __cplusplus
}
#endif

#endif