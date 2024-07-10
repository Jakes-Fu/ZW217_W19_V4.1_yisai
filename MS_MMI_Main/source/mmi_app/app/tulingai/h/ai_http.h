#ifndef __AI_HTTP_H__
#define __AI_HTTP_H__

#include "ai_api.h"
#include "ai_net.h"
#include "ai_func.h"
#include "ai_tcp.h"


extern void AiSetHttpUrl(char* url);
extern void AIHttpDefaultParamsInit();
extern void StartAiHttpRequest();
extern MMI_RESULT_E HandleAiHttpMsgExt(PWND app_ptr, uint16 msg_id, DPARAM param);
extern void AI_HttpClose();
extern void AI_HttpForceClose();
extern BOOLEAN MMIAI_DataRcvIsBreak();
extern BOOLEAN MMIAI_IsDownloadFinish();
extern void MMIAI_SetContinueAgain();





#endif
