#ifndef __ZDT_HYHL_H__
#define __ZDT_HYHL_H__
/*
��ӯ�������ܴ���ͨѶЭ��2.0����GSM��.pdf
*/
#include "zdt_hyhl_api.h"
#include "zdt_hyhl_net.h"
#include "zdt_hyhl_func.h"
#include "zdt_hyhl_db.h"


extern MMI_RESULT_E HYHL_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
extern int HYHL_AppInit(void);
extern int HYHL_AppFree(void);
extern int HYHL_RestorFactory(void);
extern HYHL_APP_T * HYHL_AppGet(void);


#endif
