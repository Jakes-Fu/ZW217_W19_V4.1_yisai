/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "zdt_app.h"
#ifdef ZDT_PLAT_HYHL_SUPPORT
#include "zdt_hyhl.h"

HYHL_APP_T  g_hyhl_app = {0};

extern int HYHL_AppInit(void)
{
    HYHL_APP_T * pMe = &g_hyhl_app;
    ZDT_LOG("HYHL_AppInit");
    HYHL_DB_Init(pMe);
    return 0;
}

extern int HYHL_AppFree(void)
{
    HYHL_APP_T * pMe = &g_hyhl_app;
    ZDT_LOG("HYHL_AppFree");
    HYHL_DB_Free(pMe);
    return 0;
}

extern HYHL_APP_T * HYHL_AppGet(void)
{
    return &g_hyhl_app;
}

int HYHL_RestorFactory(void)
{
    HYHL_APP_T * pMe = &g_hyhl_app;
    ZDT_LOG("HYHL_RestorFactory");
    HYHL_DB_DelAll(pMe);
    HYHL_Net_Reset(pMe);
    return 0;
}

extern MMI_RESULT_E HYHL_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    HYHL_APP_T * pMe = &g_hyhl_app;
    switch (msg_id)
    {
        case ZDT_APP_SIGNAL_HYHL_NET_RESET:
                HYHL_Net_Reset(pMe);
            break;
            
         case ZDT_APP_SIGNAL_HYHL_SMS_SERVER:
                //HYHL_SMS_ServerHandle(pMe,wParam,dwParam);
            break;
            
        case ZDT_APP_SIGNAL_HYHL_SMS_SOS:
                //HYHL_SMS_SOSHandle(pMe,wParam,dwParam);
            break;
            
       case ZDT_APP_SIGNAL_HYHL_HEART:
                HYHL_Net_A4_Send(pMe);
            break;

        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}
#endif
