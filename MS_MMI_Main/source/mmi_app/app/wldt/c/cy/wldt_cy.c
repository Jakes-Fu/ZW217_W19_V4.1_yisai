/*
ÔÆÐ¾Æ½Ì¨
*/
#include "wldt_app.h"

#ifdef WLDT_PLAT_CY_SUPPORT
CY_APP_T  g_cy_app = {0};

PUBLIC BOOLEAN  CY_PostEvtTo_APP(WLDT_APP_SIG_E sig_id, void* p_data, uint16 type)
{
    
    uint8 * pstr = PNULL;
    WLDT_CY_DATA_SIG_T * psig = PNULL;    
    WLDT_CY_DATA_T * cur_data = NULL;
    
    cur_data=(WLDT_CY_DATA_T *)SCI_ALLOC_APPZ(sizeof(WLDT_CY_DATA_T));
    if(cur_data == NULL)
    {
        WLDT_TCP_LOG("TCP POSTEvt ERR Malloc");
        return FALSE;
    }
    cur_data->data_p = p_data;
    cur_data->type = type;

    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(WLDT_CY_DATA_SIG_T),SCI_IdentifyThread());
    psig->p_data = cur_data;
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}

extern int CY_AppInit(void)
{
    CY_APP_T * pMe = &g_cy_app;
    WLDT_LOG("CY_AppInit");
    pMe->m_cy_net_is_open = FALSE;
    pMe->m_cy_net_is_init = FALSE;
    pMe->m_cy_net_first_reg = FALSE;
    pMe->m_cy_is_reg = FALSE;
    pMe->m_cy_allow_rereg = FALSE;
    pMe->m_cy_is_land = FALSE;
    pMe->m_cy_lk_send_time = 0;
    pMe->m_cy_reg_ip = 0;
    pMe->m_cy_reg_port = 0;
    pMe->m_cy_reg_err_num = 0;
    pMe->m_cy_test_stop = FALSE;
    pMe->m_cy_rsp_err_num = 0;

    CY_DB_Init(pMe);
    return 0;
}

extern int CY_AppFree(void)
{
    CY_APP_T * pMe = &g_cy_app;
    WLDT_LOG("CY_AppFree");
    CY_DB_Free(pMe);
    return 0;
}

extern CY_APP_T * CY_AppGet(void)
{
    return &g_cy_app;
}

int CY_RestorFactory(void)
{
    CY_APP_T * pMe = &g_cy_app;
    WLDT_LOG("CY_RestorFactory");
    CY_DB_DelAll(pMe);
    CY_Net_Reset(pMe);
    return 0;
}
extern MMI_RESULT_E CY_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    CY_APP_T * pMe = &g_cy_app;
    switch (msg_id)
    {            
        case WLDT_APP_SIGNAL_CY_NET_RESET:
                CY_Net_Reset(pMe);
            break;
            
        case WLDT_APP_SIGNAL_CY_HEART:
            break;
            
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}
#endif
