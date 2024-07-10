/*
云芯平台
*/
#include "zdt_app.h"

#ifdef ZDT_PLAT_SHB_SUPPORT
SHB_APP_T  g_shb_app = {0};
BOOLEAN g_shb_is_lbs_change = TRUE;
uint8       s_zdt_shb_need_alert   = 1;

PUBLIC BOOLEAN  SHB_PostEvtTo_APP(ZDT_APP_SIG_E sig_id, void* p_data, uint16 type)
{
    
    uint8 * pstr = PNULL;
    ZDT_SHB_DATA_SIG_T * psig = PNULL;    
    ZDT_SHB_DATA_T * cur_data = NULL;
    
    cur_data=(ZDT_SHB_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_SHB_DATA_T));
    if(cur_data == NULL)
    {
        ZDT_LOG("TCP POSTEvt ERR Malloc");
        return FALSE;
    }
    cur_data->data_p = p_data;
    cur_data->type = type;

    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_SHB_DATA_SIG_T),SCI_IdentifyThread());
    psig->p_data = cur_data;
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}

extern int SHB_AppInit(void)
{
    SHB_APP_T * pMe = &g_shb_app;
    ZDT_LOG("SHB_AppInit");
    pMe->m_shb_net_is_open = FALSE;
    pMe->m_shb_net_is_init = FALSE;
    pMe->m_shb_net_is_reg = FALSE;
    MMIZDT_SHB_NV_GetAlert();
    SHB_DB_Init(pMe);
    return 0;
}

extern int SHB_AppFree(void)
{
    SHB_APP_T * pMe = &g_shb_app;
    ZDT_LOG("SHB_AppFree");
    SHB_DB_Free(pMe);
    return 0;
}

extern SHB_APP_T * SHB_AppGet(void)
{
    return &g_shb_app;
}

static int SHB_HandleBatStatus(DPARAM param)
{
    uint8 level = 0;
    uint8 percent = 0;
    uint16 wParam  = 0;
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL)
    {
        wParam = p_getdata->len;
    }
    level = (wParam >> 8);
    percent = (wParam & 0x00FF);
    ZDT_LOG("SHB_HandleBatStatus wParam = 0x%x,level=%d,percent=%d",wParam,level,percent);
    if(level < 1) //为省电改为2 -> 1
    {
#ifdef ZDT_SMS_SUPPORT
        SHB_API_SMS_SendLowBattery();
#endif
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SHB_ALERT,NULL,0);
    }
    return 0;
}

static int SHB_HandleLBSChange(DPARAM param)
{
    //ZDT_LOG("SHB_HandleLBSChange");
    g_shb_is_lbs_change = TRUE;
    return 0;
}

BOOLEAN SHB_LocReport_Judge(void)
{
    return TRUE;
}

void SHB_LocReport_Reset(void)
{
    g_shb_is_lbs_change = FALSE;
    return;
}

PUBLIC uint8 MMIZDT_SHB_NV_GetAlert(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 need_alert = 0;
    MMINV_READ(MMI_ZDT_NV_SHB_ALERT,&need_alert,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        need_alert = 0;
        MMINV_WRITE(MMI_ZDT_NV_SHB_ALERT,&need_alert);
    }
    s_zdt_shb_need_alert = need_alert;
    return need_alert;
}

PUBLIC BOOLEAN  MMIZDT_SHB_NV_SetAlert(uint8 need_alert) 
{
    if(s_zdt_shb_need_alert != need_alert)
    {
        MMINV_WRITE(MMI_ZDT_NV_SHB_ALERT,&need_alert);
        s_zdt_shb_need_alert = need_alert;
        return TRUE;
    }
    return FALSE;
}

PUBLIC uint8 MMIZDT_SHB_GetAlert(void)
{
    return s_zdt_shb_need_alert;
}

extern MMI_RESULT_E SHB_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    SHB_APP_T * pMe = &g_shb_app;
    switch (msg_id)
    {
        case ZDT_APP_SIGNAL_SHB_BATSTAUTS:
                SHB_HandleBatStatus(param);
            break;
            
        case ZDT_APP_SIGNAL_SHB_LBS_CHANGE:
                SHB_HandleLBSChange(param);
            break;
            
        case ZDT_APP_SIGNAL_SHB_NET_RESET:
                SHB_Net_Reset(pMe);
            break;
            
       case ZDT_APP_SIGNAL_SHB_HEART:
            break;
            
       case ZDT_APP_SIGNAL_SHB_LOCREPORT:
            break;

       case ZDT_APP_SIGNAL_SHB_ALERT:
            break;

        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}

BOOLEAN MMISHB_SetNetDebug(void)
{
    SCI_MEMSET(&shb_DB_Set_Rec.ip_domain, 0, MAX_SHB_IP_DOMAIN_SIZE+1);
    SCI_MEMCPY(shb_DB_Set_Rec.ip_domain,"http://120.27.161.99:11120/s?",29); //测试服务器
    SHB_DB_SET_ListModify();
    return 0;
}

BOOLEAN MMISHB_SetNetRelease(void)
{
    SCI_MEMSET(&shb_DB_Set_Rec.ip_domain, 0, MAX_SHB_IP_DOMAIN_SIZE+1);
    SCI_MEMCPY(shb_DB_Set_Rec.ip_domain,"http://oldman-care.jd.com/s?",28); //正式服务器
    SHB_DB_SET_ListModify();
    return 0;
}

int SHB_RestorFactory(void)
{
    SHB_APP_T * pMe = &g_shb_app;
    ZDT_LOG("SHB_RestorFactory");
    SHB_DB_DelAll(pMe);
    MMIZDT_SHB_NV_SetAlert(1);
    return 0;
}
#endif
