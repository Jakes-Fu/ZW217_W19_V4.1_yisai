
#include "zdt_app.h"
#include "zdthttp_app.h"
#include "cJSON.h"
#include "mmialarm_export.h"

#ifdef ZDT_PLAT_SHB_SUPPORT

#ifdef SOS_KEYLONGWEB
//#define MMI_SHB_NUM_SAVE
#endif

static uint32 s_shb_send_idx = 0;
static uint8 s_shb_reg_times = 0;
static uint8 s_shb_load_times = 0;
static MN_DUAL_SYS_E       s_shb_http_cur_sys   = MN_DUAL_SYS_MAX;
static uint32 s_shb_check_times = 0;
static uint32 s_shb_cr_check_times = 0;
static BOOLEAN s_shb_is_in_cr = FALSE;
uint8 g_shb_cr_time_id[SHB_DB_TIME_MAX_ID_SIZE+1] = {0};

static uint32 s_shb_wl_check_times = 0;
static BOOLEAN s_shb_is_in_wl = FALSE;
static BOOLEAN s_shb_is_send_sos = FALSE;
static BOOLEAN s_shb_sos_need_sms = FALSE;
uint8 g_shb_wl_time_id[SHB_DB_TIME_MAX_ID_SIZE+1] = {0};

uint8 g_shb_is_land_ok = 0;
static EVENT_CURRENT_DATA_T  s_shb_current_event_info = {0, 0, {0}};
extern int SHB_HTTP_ZTE2100_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id);
extern BOOLEAN SHB_Net_Bind_Send(void);

static uint32 SHB_Get_Send_Index(void)
{
    s_shb_send_idx++;
    if(s_shb_send_idx > 99999)
    {
        s_shb_send_idx = 0;
    }
    return s_shb_send_idx;
}

static uint32 SHB_Get_Send_Index_Str(char * pOut)
{
    char tmp_buf[100] = {0};
    uint16 tmp_len = 0;
    uint32 cur_idx = SHB_Get_Send_Index();
    sprintf(tmp_buf,"%d",cur_idx);
    tmp_len = strlen(tmp_buf);
    SCI_MEMCPY(pOut,tmp_buf,tmp_len);
    pOut[tmp_len] = 0;
    return tmp_len;
}

static uint32 SHB_Get_Version_Str(char * pOut)
{
    char tmp_buf[100] = {0};
    uint16 tmp_len = 0;
    uint32 cur_idx = SHB_Get_Send_Index();
    sprintf(tmp_buf,"%d",cur_idx);
    tmp_len = strlen(tmp_buf);
    SCI_MEMCPY(pOut,tmp_buf,tmp_len);
    pOut[tmp_len] = 0;
    return tmp_len;
}

char * SHB_Get_CurIMEI(void)
{
    if(MMIZDT_Net_GetActiveSys() == MN_DUAL_SYS_2)
    {
        return g_zdt_phone_imei_2;
    }
    return g_zdt_phone_imei;
}

static void SHB_Heart_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SHB_HEART,NULL,0);
    return;
}

void SHB_Heart_TimeStart(SHB_APP_T *pMe,uint16 time_s)
{
    if(0 != pMe->m_shb_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_heart_timer_id);
        pMe->m_shb_heart_timer_id = 0;
    }
    
    pMe->m_shb_heart_timer_id = MMK_CreateTimerCallback(time_s*999, 
                                                                        SHB_Heart_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

void SHB_Heart_TimeStop(SHB_APP_T *pMe)
{
    if(0 != pMe->m_shb_heart_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_heart_timer_id);
        pMe->m_shb_heart_timer_id = 0;
    }
}

static void SHB_LocReport_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    SHB_APP_T *pMe = (SHB_APP_T *)param;
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SHB_LOCREPORT,NULL,0);
}

void SHB_LocReport_TimeStart(SHB_APP_T *pMe,uint16 time_s)
{
    if(0 != pMe->m_shb_pos_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_pos_timer_id);
        pMe->m_shb_pos_timer_id = 0;
    }
    if(time_s == 0)
    {
        return;
    }
    
    pMe->m_shb_pos_timer_id = MMK_CreateTimerCallback(time_s*990, 
                                                                        SHB_LocReport_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void SHB_LocReport_TimeStop(SHB_APP_T *pMe)
{
    if(0 != pMe->m_shb_pos_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_pos_timer_id);
        pMe->m_shb_pos_timer_id = 0;
    }
}


static void SHB_Find_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    SHB_HTTP_ZTE2100_Send(SHB_HTTP_ZTE2100_Send_CB);
}

void SHB_Find_TimeStart(SHB_APP_T *pMe,uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 60;
    }
    if(0 != pMe->m_shb_find_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_find_timer_id);
        pMe->m_shb_find_timer_id = 0;
    }
    
    pMe->m_shb_find_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        SHB_Find_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void SHB_Find_TimeStop(SHB_APP_T *pMe)
{
    if(0 != pMe->m_shb_find_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_find_timer_id);
        pMe->m_shb_find_timer_id = 0;
    }
}

static void SHB_LocCR_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    SHB_APP_T *pMe = (SHB_APP_T *)param;
    if(MMIAPISMS_IsOrderOk())
    {
        SHB_Net_Bind_Send();
    }
    else
    {
        if(0 != pMe->m_shb_cr_timer_id)
        {
            MMK_StopTimer(pMe->m_shb_cr_timer_id);
            pMe->m_shb_cr_timer_id = 0;
        }
        
        pMe->m_shb_cr_timer_id = MMK_CreateTimerCallback(10*1000, 
                                                                            SHB_LocCR_TimeOut, 
                                                                            (uint32)pMe, 
                                                                            FALSE);
    }
}

void SHB_LocCR_TimeStart(SHB_APP_T *pMe,uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 30;
    }
    if(0 != pMe->m_shb_cr_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_cr_timer_id);
        pMe->m_shb_cr_timer_id = 0;
    }
    
    pMe->m_shb_cr_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        SHB_LocCR_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    
}

void SHB_LocCR_TimeStop(SHB_APP_T *pMe)
{
    if(0 != pMe->m_shb_cr_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_cr_timer_id);
        pMe->m_shb_cr_timer_id = 0;
    }
}

static void SHB_Land_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    s_shb_reg_times++;
    if(s_shb_reg_times < 3)
    {
        SHB_HTTP_ZTE2200_Send();
    }
    return;
}

static void SHB_Land_TimeStart(SHB_APP_T *pMe)
{
    if(0 != pMe->m_shb_land_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_land_timer_id);
        pMe->m_shb_land_timer_id = 0;
    }
    
    pMe->m_shb_land_timer_id = MMK_CreateTimerCallback(5000, 
                                                                        SHB_Land_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
    return;
}

static void SHB_Land_TimeStop(SHB_APP_T *pMe)
{
    if(0 != pMe->m_shb_land_timer_id)
    {
        MMK_StopTimer(pMe->m_shb_land_timer_id);
        pMe->m_shb_land_timer_id = 0;
    }
    
    return;
}

extern void SHB_CloseAllTimer(SHB_APP_T *pMe)
{
    SHB_Land_TimeStop(pMe);
    SHB_Heart_TimeStop(pMe);
    SHB_LocReport_TimeStop(pMe);
    SHB_Find_TimeStop(pMe);
    return;
}

BOOLEAN SHB_Net_Is_land(void)
{
    if(shb_DB_Set_Rec.net_open && SCI_STRLEN((char *)shb_DB_AppSet_Rec.app_domain) > 0 && SCI_STRLEN((char *)shb_DB_AppSet_Rec.slef_num) > 0)
    {
        return TRUE;
    }

    return FALSE;
}

uint8 SHB_HTTP_ZTE2200_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    uint8 ret = 0;
    uint8 * decrypt_char;
    uint32 decrypt_len = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *content_json = PNULL;
    cJSON *cur_json = PNULL;

    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"resultType");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("resultType %s",msg_char);
                if(msg_len != 6 || strncmp( (char *)msg_char, "AC2200", 6 ) != 0)
                {
                    ZDT_LOG("AC2200 resultType Err");
                    cJSON_Delete(json);
                    return 0;
                }

            }
        }
        else
        {
            ZDT_LOG("resultType NONE");
            cJSON_Delete(json);
            return 0;
        }
        item_json = cJSON_GetObjectItem(json,"encryptCode");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("encryptCode=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("encryptCode NONE");
        }
        item_json = cJSON_GetObjectItem(json,"resContent");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("resContent=%s",msg_char);
                msg_len = strlen(msg_char);
                decrypt_char = SHB_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                if(decrypt_char != NULL)
                {
                    content_json=cJSON_Parse(decrypt_char);
                    if(content_json)
                    {
                        cur_json = cJSON_GetObjectItem(content_json,"code");
                        if(cur_json)
                        {
                            msg_char = cJSON_GetObjectString(cur_json);
                            if(msg_char != PNULL)
                            {
                                val = atoi(msg_char);
                                ZDT_LOG("code=%d,%s",val,msg_char);
                            }
                        }
                        else
                        {
                            ZDT_LOG("code NONE");
                        }
                        if(val == 200)
                        {                            
                            cur_json = cJSON_GetObjectItem(content_json,"ctSMS");
                            if(cur_json)
                            {
                                msg_char = cJSON_GetObjectString(cur_json);
                                if(msg_char != PNULL)
                                {
                                    msg_len = strlen(msg_char);
                                    ZDT_LOG("ctSMS=%d,%s",msg_len,msg_char);
                                    if(msg_len > 0)
                                    {
                                        if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1') 
                                        {
                                            ZDT_LOG("No ctSMS num");
                                        }
                                        else
                                        {
                                            if(msg_len > MAX_SHB_SMS_NUMBER_SIZE)
                                            {
                                                msg_len = MAX_SHB_SMS_NUMBER_SIZE;
                                            }
                                            SCI_MEMSET(shb_DB_Set_Rec.sms_ct,0,MAX_SHB_SMS_NUMBER_SIZE+1);
                                            SCI_MEMCPY(shb_DB_Set_Rec.sms_ct,msg_char,msg_len);
                                            ret = 1;
                                        }
                                    }
                                    
                                }
                            }
                            else
                            {
                                ZDT_LOG("ctSMS NONE");
                            }
                            
                            cur_json = cJSON_GetObjectItem(content_json,"cmSMS");
                            if(cur_json)
                            {
                                msg_char = cJSON_GetObjectString(cur_json);
                                if(msg_char != PNULL)
                                {
                                    msg_len = strlen(msg_char);
                                    ZDT_LOG("cmSMS=%d,%s",msg_len,msg_char);
                                    if(msg_len > 0)
                                    {
                                        if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1') 
                                        {
                                            ZDT_LOG("No cmSMS num");
                                        }
                                        else
                                        {
                                            if(msg_len > MAX_SHB_SMS_NUMBER_SIZE)
                                            {
                                                msg_len = MAX_SHB_SMS_NUMBER_SIZE;
                                            }
                                            SCI_MEMSET(shb_DB_Set_Rec.sms_cmcc,0,MAX_SHB_SMS_NUMBER_SIZE+1);
                                            SCI_MEMCPY(shb_DB_Set_Rec.sms_cmcc,msg_char,msg_len);
                                            ret = 2;
                                        }
                                    }
                                    
                                }
                            }
                            else
                            {
                                ZDT_LOG("cmSMS NONE");
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"unSMS");
                            if(cur_json)
                            {
                                msg_char = cJSON_GetObjectString(cur_json);
                                if(msg_char != PNULL)
                                {
                                    msg_len = strlen(msg_char);
                                    ZDT_LOG("unSMS=%d,%s",msg_len,msg_char);
                                    if(msg_len > 0)
                                    {
                                        if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1') 
                                        {
                                            ZDT_LOG("No unSMS num");
                                        }
                                        else
                                        {
                                            if(msg_len > MAX_SHB_SMS_NUMBER_SIZE)
                                            {
                                                msg_len = MAX_SHB_SMS_NUMBER_SIZE;
                                            }
                                            SCI_MEMSET(shb_DB_Set_Rec.sms_cu,0,MAX_SHB_SMS_NUMBER_SIZE+1);
                                            SCI_MEMCPY(shb_DB_Set_Rec.sms_cu,msg_char,msg_len);
                                            ret = 3;
                                        }
                                    }
                                    
                                }
                            }
                            else
                            {
                                ZDT_LOG("unSMS NONE");
                            }
                            shb_DB_Set_Rec.is_reg = 1;
                             SHB_DB_SET_ListModify();
                        }
                        cJSON_Delete(content_json);
                    }
                    else
                    {
                        ZDT_LOG("content_json NONE");
                    }

                    SCI_FREE(decrypt_char);
                }
                
            }
        }
        else
        {
            ZDT_LOG("resContent NONE");
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YJ_HTTP_Send_PowerOn_CB ERR  cJSON_Parse");
    }
    return ret;
}

int SHB_HTTP_ZTE2200_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    ZDT_LOG("SHB_HTTP_Send_ZTE2200_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        g_shb_app.m_shb_net_is_reg = TRUE;
        if(pRcv != NULL && Rcv_len > 2)
        {
            if(SHB_HTTP_ZTE2200_Rcv_Handle(pRcv,Rcv_len))
            {
                if(MMIAPISMS_IsOrderOk())
                {
                    SHB_Net_Bind_Send();
                }
                else
                {
                    SHB_LocCR_TimeStart(&g_shb_app,10);
                }
            }
            else
            {
                SHB_Land_TimeStart(&g_shb_app);
            }
        }
        else
        {
            SHB_Land_TimeStart(&g_shb_app);
        }
    }
    else
    {
        SHB_Land_TimeStart(&g_shb_app);
    }
    return 0;
}

BOOLEAN  SHB_HTTP_ZTE2200_Send(void)
{
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * out = PNULL;
    char * pContent = PNULL;
    char act_str[51] = {0};
    uint32 act_len = 0;
    char md5_key_str[51] = {0};
    uint32 md5_key_len = 0;
    char app_domain[MAX_SHB_APP_DOMAIN_SIZE+1] = {0};
    uint16 len = 0;
    char content_buf[1024] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;
    uint8  * md5_data_in = PNULL;
    uint32 md5_len_in = 0;
    char md5_str[32] = {0};

    ZDT_LOG("SHB_HTTP_ZTE2200_Send");
    //加密
    reqContent=cJSON_CreateObject();
    if(reqContent)
    {
        content_len = SHB_Get_Send_Index_Str(content_buf);
        cJSON_AddItemToObject(reqContent, "seqNum", cJSON_CreateString(content_buf));
        cJSON_AddItemToObject(reqContent, "ver", cJSON_CreateString(SHB_SOFT_VERSION));
        cJSON_AddItemToObject(reqContent, "ICCID", cJSON_CreateString(g_zdt_sim_iccid));
        cJSON_AddItemToObject(reqContent, "IMEI", cJSON_CreateString(g_zdt_phone_imei));
        content_len = SHB_Func_GetDateTime(content_buf);
        cJSON_AddItemToObject(reqContent, "timestamp", cJSON_CreateString(content_buf));
        pContent=cJSON_PrintUnformatted(reqContent);
        if(pContent)
        {
            content_len = strlen(pContent);
            crypt_data = SHB_Func_Encrypt(pContent,content_len,&crypt_len);
            SCI_FREE(pContent);
        }
        cJSON_Delete(reqContent);
    }
    
    if(crypt_data != PNULL)
    {
        //MD5 签权
        strcpy(act_str,"AC2200");
        act_len = strlen(act_str);
        strcpy(md5_key_str,SHB_MD5_SECKEY_STR);
        md5_key_len = strlen(md5_key_str);
        md5_len_in = act_len + md5_key_len + crypt_len;
        md5_data_in = SCI_ALLOC_APPZ(md5_len_in);
        ZDT_LOG("[CRYPT]:md5_key_len=%d,act_len=%d,act_str=%s,%s",md5_key_len,act_len,act_str,md5_key_str);
        if(md5_data_in != PNULL)
        {
            SCI_MEMCPY(md5_data_in,act_str,act_len);
            SCI_MEMCPY(md5_data_in+act_len,crypt_data,crypt_len);
            SCI_MEMCPY(md5_data_in+act_len+crypt_len,md5_key_str,md5_key_len);
            SHB_Func_MD5_Generate((uint8 *)md5_data_in,md5_len_in,md5_str);
            SCI_FREE(md5_data_in);
            ZDT_LOG("[CRYPT]:MD5=%s",md5_str);
        }
        root=cJSON_CreateObject();
        if(root)
        {
            sprintf(app_domain,"%s",shb_DB_Set_Rec.ip_domain);
            cJSON_AddItemToObject(root, "actType", cJSON_CreateString(act_str));
            cJSON_AddItemToObject(root, "reqContent", cJSON_CreateString(crypt_data));
            cJSON_AddItemToObject(root, "encryptCode", cJSON_CreateString(md5_str));
            out=cJSON_PrintUnformatted(root);
            if(out)
            {
                len = strlen(out);
                ZDT_CryptTrace(out,len);
                MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)out,len,2200,0,0,0,0,0,SHB_HTTP_ZTE2200_Send_CB);
                SCI_FREE(out);
            }
            cJSON_Delete(root);
        }
        SCI_FREE(crypt_data);            
    }
    return TRUE;
}

BOOLEAN  SHB_HTTP_ZTE2200_Start(void)
{
    s_shb_load_times = 0;
    s_shb_reg_times = 0;
    SHB_LocCR_TimeStop(&g_shb_app);
    SHB_Find_TimeStop(&g_shb_app);
    SHB_Land_TimeStop(&g_shb_app);
    SHB_HTTP_ZTE2200_Send();
    return TRUE;
}

uint8 SHB_HTTP_ZTE2100_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    uint8 ret = 0;
    uint8 * decrypt_char;
    uint32 decrypt_len = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *content_json = PNULL;
    cJSON *cur_json = PNULL;

    if(pContent == PNULL || ContentLen < 2)
    {
        return 0;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"resultType");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("resultType %s",msg_char);
                if(msg_len != 6 || strncmp( (char *)msg_char, "AC2100", 6 ) != 0)
                {
                    ZDT_LOG("AC2100 resultType Err");
                    cJSON_Delete(json);
                    return 0;
                }

            }
        }
        else
        {
            ZDT_LOG("resultType NONE");
            cJSON_Delete(json);
            return 0;
        }
        item_json = cJSON_GetObjectItem(json,"encryptCode");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("encryptCode=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("encryptCode NONE");
        }
        item_json = cJSON_GetObjectItem(json,"resContent");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("resContent=%s",msg_char);
                msg_len = strlen(msg_char);
                decrypt_char = SHB_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                if(decrypt_char != NULL)
                {
                    content_json=cJSON_Parse(decrypt_char);
                    if(content_json)
                    {
                        cur_json = cJSON_GetObjectItem(content_json,"code");
                        if(cur_json)
                        {
                            msg_char = cJSON_GetObjectString(cur_json);
                            if(msg_char != PNULL)
                            {
                                val = atoi(msg_char);
                                ZDT_LOG("code=%d,%s",val,msg_char);
                            }
                        }
                        else
                        {
                            ZDT_LOG("code NONE");
                        }
                        if(val == 200)
                        {
                            cur_json = cJSON_GetObjectItem(content_json,"mobSite");
                            if(cur_json)
                            {
                                msg_char = cJSON_GetObjectString(cur_json);
                                if(msg_char != PNULL)
                                {
                                    msg_len = strlen(msg_char);
                                    ZDT_LOG("mobSite=%d,%s",msg_len,msg_char);   
                                    if(msg_len > 0)
                                    {
                                        SCI_MEMSET(shb_DB_AppSet_Rec.app_domain,0,MAX_SHB_APP_DOMAIN_SIZE+1);
                                        if(msg_len > MAX_SHB_APP_DOMAIN_SIZE-2)
                                        {
                                            msg_len = MAX_SHB_APP_DOMAIN_SIZE-2;
                                        }
                                        SCI_MEMCPY(shb_DB_AppSet_Rec.app_domain,msg_char,msg_len);
                                        shb_DB_AppSet_Rec.app_domain[msg_len++] = '/';
                                        shb_DB_AppSet_Rec.app_domain[msg_len++] = 'n';
                                        ret = 1;
                                    }
                                }
                            }
                            else
                            {
                                ZDT_LOG("mobSite NONE");
                            }
                            
                            cur_json = cJSON_GetObjectItem(content_json,"mobile");
                            if(cur_json)
                            {
                                msg_char = cJSON_GetObjectString(cur_json);
                                if(msg_char != PNULL)
                                {
                                    msg_len = strlen(msg_char);
                                    ZDT_LOG("mobile=%d,%s",msg_len,msg_char);
                                    if(msg_len > 0)
                                    {
                                        if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1') 
                                        {
                                            ZDT_LOG("No mobile num");
                                        }
                                        else
                                        {
                                            if(msg_len > MAX_SHB_APP_NUMBER_SIZE)
                                            {
                                                msg_len = MAX_SHB_APP_NUMBER_SIZE;
                                            }
                                            SCI_MEMSET(shb_DB_AppSet_Rec.slef_num,0,MAX_SHB_APP_NUMBER_SIZE+1);
                                            SCI_MEMCPY(shb_DB_AppSet_Rec.slef_num,msg_char,msg_len);
                                            SCI_MEMSET(shb_DB_AppSet_Rec.iccid,0,MAX_SHB_APP_ICCID_SIZE+1);
                                            SCI_STRCPY((char *)shb_DB_AppSet_Rec.iccid,g_zdt_sim_iccid);
                                            ret += 2;
                                        }
                                    }
                                    
                                }
                            }
                            else
                            {
                                ZDT_LOG("mobile NONE");
                            }
                            
                            cur_json = cJSON_GetObjectItem(content_json,"platNo");
                            if(cur_json)
                            {
                                msg_char = cJSON_GetObjectString(cur_json);
                                if(msg_char != PNULL)
                                {
                                    msg_len = strlen(msg_char);
                                    ZDT_LOG("platNo=%d,%s",msg_len,msg_char);
                                    if(msg_len > 0)
                                    {
                                        if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1') 
                                        {
                                            ZDT_LOG("No platNo num");
                                        }
                                        else
                                        {
                                            if(msg_len > MAX_SHB_APP_NUMBER_SIZE)
                                            {
                                                msg_len = MAX_SHB_APP_NUMBER_SIZE;
                                            }
                                            SCI_MEMSET(shb_DB_AppSet_Rec.shb_num,0,MAX_SHB_APP_NUMBER_SIZE+1);
                                            SCI_MEMCPY(shb_DB_AppSet_Rec.shb_num,msg_char,msg_len);
                                        }
                                    }
                                    
                                }
                            }
                            else
                            {
                                ZDT_LOG("platNo NONE");
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"chatSite");
                            if(cur_json)
                            {
                                msg_char = cJSON_GetObjectString(cur_json);
                                if(msg_char != PNULL)
                                {
                                    msg_len = strlen(msg_char);
                                    ZDT_LOG("chatSite=%d,%s",msg_len,msg_char);   
                                    if(msg_len > 0)
                                    {
                                        SCI_MEMSET(shb_DB_AppSet_Rec.wchat_domain,0,MAX_SHB_APP_DOMAIN_SIZE+1);
                                        if(msg_len > MAX_SHB_APP_DOMAIN_SIZE)
                                        {
                                            msg_len = MAX_SHB_APP_DOMAIN_SIZE;
                                        }
                                        SCI_MEMCPY(shb_DB_AppSet_Rec.wchat_domain,msg_char,msg_len);
                                    }
                                }
                            }
                            else
                            {
                                ZDT_LOG("chatSite NONE");
                            }
                            SHB_DB_APPSET_ListModify();
                        }
                        cJSON_Delete(content_json);
                    }
                    else
                    {
                        ZDT_LOG("content_json NONE");
                    }

                    SCI_FREE(decrypt_char);
                }
                
            }
        }
        else
        {
            ZDT_LOG("resContent NONE");
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YJ_HTTP_Send_PowerOn_CB ERR  cJSON_Parse");
    }
    return ret;
}

int SHB_HTTP_ZTE2100_ReSend(void)
{
    s_shb_load_times++;
    ZDT_LOG("SHB_HTTP_ZTE2100_ReSend s_shb_load_times=%d",s_shb_load_times);
    if(s_shb_load_times < 3)
    {
        //SHB_API_SMS_SendReg();
        SHB_Find_TimeStart(&g_shb_app,60);
    }
    return 0;
}

int SHB_HTTP_ZTE2100_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    uint8 ret = 0;
    ZDT_LOG("SHB_HTTP_ZTE2100_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            ret = SHB_HTTP_ZTE2100_Rcv_Handle(pRcv,Rcv_len);
            if(ret > 1)
            {
                SHB_HTTP_ZTE1000_Send();
            }
            else
            {
                SHB_HTTP_ZTE2100_ReSend();
            }
        }
        else
        {
            SHB_HTTP_ZTE2100_ReSend();
        }
    }
    else
    {
        SHB_HTTP_ZTE2100_ReSend();
    }
    return 0;
}

int SHB_HTTP_ZTE2100_Send_CheckCB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    uint8 ret = 0;
    ZDT_LOG("SHB_HTTP_ZTE2100_Send_CheckCB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            ret = SHB_HTTP_ZTE2100_Rcv_Handle(pRcv,Rcv_len);
            if(ret > 1)
            {
                SHB_HTTP_ZTE1000_Send();
            }
            else
            {
                SHB_HTTP_ZTE2200_Start();
            }
        }
        else
        {
            SHB_HTTP_ZTE2200_Start();
        }
    }
    else
    {
        SHB_HTTP_ZTE2200_Start();
    }
    return 0;
}

BOOLEAN  SHB_HTTP_ZTE2100_Send(HTTPRCVHANDLER rcv_handle)
{
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * out = PNULL;
    char * pContent = PNULL;
    char act_str[51] = {0};
    uint32 act_len = 0;
    char md5_key_str[51] = {0};
    uint32 md5_key_len = 0;
    char app_domain[MAX_SHB_APP_DOMAIN_SIZE+1] = {0};
    uint16 len = 0;
    char content_buf[1024] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;
    uint8  * md5_data_in = PNULL;
    uint32 md5_len_in = 0;
    char md5_str[32] = {0};

    ZDT_LOG("SHB_HTTP_ZTE2100_Send");
    //加密
    reqContent=cJSON_CreateObject();
    if(reqContent)
    {
        content_len = SHB_Get_Send_Index_Str(content_buf);
        cJSON_AddItemToObject(reqContent, "seqNum", cJSON_CreateString(content_buf));
        cJSON_AddItemToObject(reqContent, "ver", cJSON_CreateString(SHB_SOFT_VERSION));
        cJSON_AddItemToObject(reqContent, "ICCID", cJSON_CreateString(g_zdt_sim_iccid));
        cJSON_AddItemToObject(reqContent, "IMEI", cJSON_CreateString(g_zdt_phone_imei));
        content_len = SHB_Func_GetDateTime(content_buf);
        cJSON_AddItemToObject(reqContent, "timestamp", cJSON_CreateString(content_buf));
        pContent=cJSON_PrintUnformatted(reqContent);
        if(pContent)
        {
            content_len = strlen(pContent);
            crypt_data = SHB_Func_Encrypt(pContent,content_len,&crypt_len);
            SCI_FREE(pContent);
        }
        cJSON_Delete(reqContent);
    }
    
    if(crypt_data != PNULL)
    {
        //MD5 签权
        strcpy(act_str,"AC2100");
        act_len = strlen(act_str);
        strcpy(md5_key_str,SHB_MD5_SECKEY_STR);
        md5_key_len = strlen(md5_key_str);
        md5_len_in = act_len + md5_key_len + crypt_len;
        md5_data_in = SCI_ALLOC_APPZ(md5_len_in);
        ZDT_LOG("[CRYPT]:md5_key_len=%d,act_len=%d,act_str=%s,%s",md5_key_len,act_len,act_str,md5_key_str);
        if(md5_data_in != PNULL)
        {
            SCI_MEMCPY(md5_data_in,act_str,act_len);
            SCI_MEMCPY(md5_data_in+act_len,crypt_data,crypt_len);
            SCI_MEMCPY(md5_data_in+act_len+crypt_len,md5_key_str,md5_key_len);
            SHB_Func_MD5_Generate((uint8 *)md5_data_in,md5_len_in,md5_str);
            SCI_FREE(md5_data_in);
            ZDT_LOG("[CRYPT]:MD5=%s",md5_str);
        }
        root=cJSON_CreateObject();
        if(root)
        {
            sprintf(app_domain,"%s",shb_DB_Set_Rec.ip_domain);
            cJSON_AddItemToObject(root, "actType", cJSON_CreateString(act_str));
            cJSON_AddItemToObject(root, "reqContent", cJSON_CreateString(crypt_data));
            cJSON_AddItemToObject(root, "encryptCode", cJSON_CreateString(md5_str));
            out=cJSON_PrintUnformatted(root);
            if(out)
            {
                len = strlen(out);
                ZDT_CryptTrace(out,len);
                MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)out,len,2100,0,0,0,0,0,rcv_handle);
                SCI_FREE(out);
            }
            cJSON_Delete(root);
        }
        SCI_FREE(crypt_data);            
    }
    return TRUE;
}

BOOLEAN SHB_HTTP_Change_SPD(
                                                             uint8 index,
                                                             uint8* number_ptr,
                                                             uint8 number_len
                                                            )
{
#ifdef MMI_SHB_NUM_SAVE
        if(g_is_user_update_cfg)
        {
            //ZdtSetSpeedDialItemNew(number_ptr,100,number_len,index);
        }
#endif
    return TRUE;
}

BOOLEAN SHB_HTTP_Change_SOS(
                                                             uint8 index,
                                                             uint8* number_ptr
                                                            )
{
#ifdef MMI_SHB_NUM_SAVE
    if(g_is_user_update_cfg)
    {
        if(index == 0)
        {
            MMIAPISet_SosCall_Num1(number_ptr);
        }
        else if(index == 1)
        {
            MMIAPISet_SosCall_Num2(number_ptr);
        }
        else if(index == 2)
        {
            MMIAPISet_SosCall_Num3(number_ptr);
        }
        else if(index == 3)
        {
            MMIAPISet_SosCall_Num4(number_ptr);
        }
        else if(index == 4)
        {
            MMIAPISet_SosCall_Num5(number_ptr);
        }
    }
#endif
    return TRUE;
}

//系统配置
uint8 SHB_HTTP_ZTE1000_Rcv_Handle_f00(cJSON *pObject)
{
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f00");

    json=pObject;
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"platNum");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("platNum len=%d,%s",msg_len,msg_char);
                if(msg_len > 0)
                {
                    if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1') 
                    {
                        ZDT_LOG("No mobile num");
                        SCI_MEMSET(shb_DB_PlatSet_Rec.platNum,0,MAX_SHB_APP_NUMBER_SIZE+1);
                    }
                    else
                    {
                        SCI_MEMSET(shb_DB_PlatSet_Rec.platNum,0,MAX_SHB_APP_NUMBER_SIZE+1);
                        if(msg_len > MAX_SHB_APP_NUMBER_SIZE)
                        {
                            msg_len = MAX_SHB_APP_NUMBER_SIZE;
                        }
                        SCI_MEMCPY(shb_DB_PlatSet_Rec.platNum,msg_char,msg_len);
                        ret = 1;
                    }
                }
            }
        }
        
        item_json = cJSON_GetObjectItem(json,"mobile");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("mobile len=%d,%s",msg_len,msg_char);
                if(msg_len > 0)
                {
                    if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1') 
                    {
                        ZDT_LOG("No mobile num");
                        SCI_MEMSET(shb_DB_PlatSet_Rec.mobile,0,MAX_SHB_APP_NUMBER_SIZE+1);
                    }
                    else
                    {
                        SCI_MEMSET(shb_DB_PlatSet_Rec.mobile,0,MAX_SHB_APP_NUMBER_SIZE+1);
                        if(msg_len > MAX_SHB_APP_NUMBER_SIZE)
                        {
                            msg_len = MAX_SHB_APP_NUMBER_SIZE;
                        }
                        SCI_MEMCPY(shb_DB_PlatSet_Rec.mobile,msg_char,msg_len);
                        ret += 2;
                    }
                }
            }
        }
        item_json = cJSON_GetObjectItem(json,"HBInterval");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("HBInterval len=%d,%s",msg_len,msg_char);
                if(msg_len > 0)
                {
                    val = atoi(msg_char);
                    shb_DB_Set_Rec.time_interval =val/60;
                    ret += 3;
                }
            }
        }        
        SHB_DB_PLATSET_ListModify();
    }
    return ret;
}

//白名单
uint8 SHB_HTTP_ZTE1000_Rcv_Handle_f01(cJSON *pObject)
{
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *arry_json = PNULL;
    int i = 0;
    int arry_size = 0;

    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f01");

    json=pObject;
    if (json) 
    {
        arry_size = cJSON_GetArraySize(json);
        ZDT_LOG("F01 arry_size=%d",arry_size);
        if(arry_size > SHB_DB_WHITE_MAX_SUM)
        {
            arry_size = SHB_DB_WHITE_MAX_SUM;
        }
        if(arry_size == 0)
        {
            SHB_DB_WHITE_ListDelAllBuf();
        }
        for(i = 0; i < arry_size; i++)
        {
            arry_json = cJSON_GetArrayItem(json,i);
            if(arry_json)
            {
                item_json = cJSON_GetObjectItem(arry_json,"whiteNum");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("whiteNum[%d] len=%d,%s",i,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            SHB_DB_WHITE_ListModifyBuf(i,msg_char,msg_len);
                            ret++;
                        }
                    }
                }
            }
        }
        SHB_DB_WHITE_ListSaveBuf();
    }
    return ret;
}

//工作模式设置
uint8 SHB_HTTP_ZTE1000_Rcv_Handle_f02(cJSON *pObject)
{
    int result = 0;
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *arry_json = PNULL;
    int i = 0;
    int j = 0;
    int arry_size = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    uint8 time_set = 0;
    uint32 timer_start = 0;
    uint32 timer_end = 0;
    uint8 * pTime = NULL;
    uint32 time_len = 0;
    char buf[101] = {0};
    
    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f02");

    json=pObject;
    if (json) 
    {
        arry_size = cJSON_GetArraySize(json);
        ZDT_LOG("F02 arry_size=%d",arry_size);
        if(arry_size > 4)
        {
            arry_size = 4;
        }
        if(arry_size == 0)
        {
            for(j = SHB_TIME_TYPE_SLIENT_1; j <= SHB_TIME_TYPE_SLIENT_4; j++)
            {
                SHB_DB_TIMER_ListDel(j);
            }
            SHB_DB_TIMER_ListSave();
        }
        for(i = SHB_TIME_TYPE_SLIENT_1; i < (arry_size+SHB_TIME_TYPE_SLIENT_1); i++)
        {
            start_hour = 0;
            start_min = 0;
            end_hour = 0;
            end_min = 0;
            time_set = 0x00;
            SHB_DB_TIMER_ListDel(i);
            
            arry_json = cJSON_GetArrayItem(json,i-SHB_TIME_TYPE_SLIENT_1);
            if(arry_json)
            {
                item_json = cJSON_GetObjectItem(arry_json,"startTime");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("startTime[%d] len=%d,%s",i-SHB_TIME_TYPE_SLIENT_1,msg_len,msg_char);
                        if(msg_len > 3)
                        {
                            time_set |= 0x01;
                            buf[0] = msg_char[0];
                            buf[1] = msg_char[1];
                            buf[2] = 0;
                            start_hour = atoi(buf);
                            buf[0] = msg_char[2];
                            buf[1] = msg_char[3];
                            start_min = atoi(buf);
                            timer_start = (start_hour * 3600) + (start_min * 60);
                            ret++;
                        }
                    }
                }
               item_json = cJSON_GetObjectItem(arry_json,"endTime");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("endTime[%d] len=%d,%s",i-SHB_TIME_TYPE_SLIENT_1,msg_len,msg_char);
                        if(msg_len > 3)
                        {
                            time_set |= 0x01;
                            buf[0] = msg_char[0];
                            buf[1] = msg_char[1];
                            buf[2] = 0;
                            end_hour = atoi(buf);
                            buf[0] = msg_char[2];
                            buf[1] = msg_char[3];
                            end_min = atoi(buf);
                            timer_end = (end_hour * 3600) + (end_min * 60);
                            ret++;
                        }
                    }
                }
                
                item_json = cJSON_GetObjectItem(arry_json,"modelWeek");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("modelWeek[%d] len=%d,%s",i-SHB_TIME_TYPE_SLIENT_1,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            if(msg_char[0] == '*')
                            {
                                time_set = 0xFF;
                            }
                            else
                            {
                                time_set |= 0x01;
                                for(j = 0; j < msg_len; j++)
                                {
                                    if(msg_char[j] >= '1' && msg_char[j] <= '7')
                                    {
                                        uint8 tmp_val = msg_char[j] - '0';
                                        time_set |= (0x01 << tmp_val);
                                    }
                                }
                            }
                            ret++;
                        }
                    }
                }
                SHB_DB_TIMER_ListModify(i,0,time_set,timer_start,timer_end,0);
            }
        }
        SHB_DB_TIMER_ListSave();
    }
    return ret;
}

//无功能
uint8 SHB_HTTP_ZTE1000_Rcv_Handle_f03(cJSON *pObject)
{
    uint8 ret = 0;
    
    return ret;
}

//亲情号码设置
uint8 SHB_HTTP_ZTE1000_Rcv_Handle_f04(cJSON *pObject)
{
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    int i = 0;
    int j = 0;
    char key_str[5] = {'k','e','y','1',0};
    
    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f04");
    json=pObject;
    if (json) 
    {
        SHB_DB_SPD_ListDelAllBuf();
        for(i = 0; i < SHB_DB_SPD_MAX_SUM; i++)
        {
            key_str[3] = i+'1';
            item_json = cJSON_GetObjectItem(json,key_str);
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    ZDT_LOG("%s len=%d,%s",key_str,msg_len,msg_char);
                    if(msg_len > 0)
                    {
                        if(msg_char[0] == '-')
                        {
                            SHB_DB_SPD_ListDelBuf(i);
                            SHB_HTTP_Change_SPD(i,"",0);
                        }
                        else
                        {
                            SHB_DB_SPD_ListModifyBuf(i,msg_char,msg_len);
                            SHB_HTTP_Change_SPD(i,msg_char,msg_len);
                        }
                        ret++;
                    }
                }
            }
        }
        SHB_DB_SPD_ListSaveBuf();
    }
    return ret;
}

//SOS 设置
BOOLEAN SHB_HTTP_ZTE1000_Rcv_Handle_f05(cJSON *pObject)
{
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    int i = 0;
    int j = 0;
    char sos_str[5] = {'s','o','s','1',0};
    
    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f05");
    json=pObject;
    if (json) 
    {
        SHB_DB_SOS_ListDelAllBuf();
        for(i = 0; i < SHB_DB_SOS_MAX_SUM; i++)
        {
            SHB_HTTP_Change_SOS(i,"");
            sos_str[3] = i+'1';
            item_json = cJSON_GetObjectItem(json,sos_str);
            if(item_json)
            {
                msg_char = cJSON_GetObjectString(item_json);
                if(msg_char != PNULL)
                {
                    msg_len = strlen(msg_char);
                    ZDT_LOG("%s len=%d,%s",sos_str,msg_len,msg_char);
                    if(msg_len > 0)
                    {
                        if(msg_char[0] == '-')
                        {
                            //SHB_DB_SOS_ListDelBuf(i);
                            //SHB_HTTP_Change_SOS(i,"");
                        }
                        else
                        {
                            SHB_DB_SOS_ListModifyBuf(i,msg_char,msg_len);
                            SHB_HTTP_Change_SOS(i,msg_char);
                        }
                        ret++;
                    }
                }
            }
        }
        SHB_DB_SOS_ListSaveBuf();
    }
    return ret;
}

//运动模式配置
BOOLEAN SHB_HTTP_ZTE1000_Rcv_Handle_f06(cJSON *pObject)
{
    int result = 0;
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *arry_json = PNULL;
    int i = 0;
    int j = 0;
    int arry_size = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    uint8 time_set = 0;
    uint32 timer_start = 0;
    uint32 timer_end = 0;
    uint32 timer_interval_min = 0;
    char buf[101] = {0};
    
    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f06");

    json=pObject;
    if (json) 
    {
        arry_size = cJSON_GetArraySize(json);
        ZDT_LOG("F06 arry_size=%d",arry_size);
        if(arry_size > 3)
        {
            arry_size = 3;
        }
        if(arry_size == 0)
        {
            for(j = SHB_TIME_TYPE_CR_1; j <= SHB_TIME_TYPE_CR_3; j++)
            {
                SHB_DB_TIMER_ListDel(j);
            }
            SHB_DB_TIMER_ListSave();
        }
        for(i = SHB_TIME_TYPE_CR_1; i < (arry_size+SHB_TIME_TYPE_CR_1); i++)
        {
            start_hour = 0;
            start_min = 0;
            end_hour = 0;
            end_min = 0;
            time_set = 0xFF;
            timer_interval_min = 0;
            
            SHB_DB_TIMER_ListDel(i);
            
            arry_json = cJSON_GetArrayItem(json,i-SHB_TIME_TYPE_CR_1);
            if(arry_json)
            {
                item_json = cJSON_GetObjectItem(arry_json,"checkFlag");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("checkFlag[%d] len=%d,%s",i-SHB_TIME_TYPE_CR_1,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            val = atoi(msg_char);
                            if(val == 0)
                            {
                                time_set &= 0xFE;
                            }
                            else
                            {
                                time_set |= 0x01;
                            }
                            ret++;
                        }
                    }
                }
                
                item_json = cJSON_GetObjectItem(arry_json,"startTime");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("startTime[%d] len=%d,%s",i-SHB_TIME_TYPE_CR_1,msg_len,msg_char);
                        if(msg_len > 3)
                        {
                            buf[0] = msg_char[0];
                            buf[1] = msg_char[1];
                            buf[2] = 0;
                            start_hour = atoi(buf);
                            buf[0] = msg_char[2];
                            buf[1] = msg_char[3];
                            start_min = atoi(buf);
                            timer_start = (start_hour * 3600) + (start_min * 60);
                            ret++;
                        }
                    }
                }
               item_json = cJSON_GetObjectItem(arry_json,"endTime");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("endTime[%d] len=%d,%s",i-SHB_TIME_TYPE_CR_1,msg_len,msg_char);
                        if(msg_len > 3)
                        {
                            buf[0] = msg_char[0];
                            buf[1] = msg_char[1];
                            buf[2] = 0;
                            end_hour = atoi(buf);
                            buf[0] = msg_char[2];
                            buf[1] = msg_char[3];
                            end_min = atoi(buf);
                            timer_end = (end_hour * 3600) + (end_min * 60);
                            ret++;
                        }
                    }
                }
                
                item_json = cJSON_GetObjectItem(arry_json,"interval");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("interval[%d] len=%d,%s",i-SHB_TIME_TYPE_CR_1,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            val = atoi(msg_char);
                            timer_interval_min = val;
                            ret++;
                        }
                    }
                }
                
                SHB_DB_TIMER_ListModify(i,0,time_set,timer_start,timer_end,timer_interval_min);
            }
        }
        SHB_DB_TIMER_ListSave();
    }
    return ret;
}

//电子围 栏定位
uint8 SHB_HTTP_ZTE1000_Rcv_Handle_f07(cJSON *pObject)
{
    int result = 0;
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *arry_json = PNULL;
    int i = 0;
    int j = 0;
    int arry_size = 0;
    uint8 start_hour = 0;
    uint8 start_min = 0;
    uint8 end_hour = 0;
    uint8 end_min = 0;
    uint8 time_set = 0;
    uint32 timer_start = 0;
    uint32 timer_end = 0;
    uint32 timer_interval_min = 0;
    char time_id_buf[SHB_DB_TIME_MAX_ID_SIZE+1] = {0};
    char buf[101] = {0};
    
    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f07");

    json=pObject;
    if (json) 
    {
        arry_size = cJSON_GetArraySize(json);
        ZDT_LOG("F07 arry_size=%d",arry_size);
        if(arry_size > 8)
        {
            arry_size = 8;
        }
        if(arry_size == 0)
        {
            for(j = SHB_TIME_TYPE_LOC_1; j <= SHB_TIME_TYPE_LOC_8; j++)
            {
                SHB_DB_TIMER_ListDel(j);
            }
            SHB_DB_TIMER_ListSave();
        }
        for(i = SHB_TIME_TYPE_LOC_1; i < (arry_size+SHB_TIME_TYPE_LOC_1); i++)
        {
            start_hour = 0;
            start_min = 0;
            end_hour = 0;
            end_min = 0;
            time_set = 0xFF;
            timer_interval_min = 0;
            SCI_MEMSET(time_id_buf,0,SHB_DB_TIME_MAX_ID_SIZE+1);
            SHB_DB_TIMER_ListDel(i);
            
            arry_json = cJSON_GetArrayItem(json,i-SHB_TIME_TYPE_LOC_1);
            if(arry_json)
            {
                item_json = cJSON_GetObjectItem(arry_json,"checkFlag");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("checkFlag[%d] len=%d,%s",i-SHB_TIME_TYPE_LOC_1,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            val = atoi(msg_char);
                            if(val == 0)
                            {
                                time_set &= 0xFE;
                            }
                            else
                            {
                                time_set |= 0x01;
                            }
                            ret++;
                        }
                    }
                }
                item_json = cJSON_GetObjectItem(arry_json,"alarmID");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        if(msg_len > 0)
                        {
                            if(msg_len > SHB_DB_TIME_MAX_ID_SIZE)
                            {
                                msg_len = SHB_DB_TIME_MAX_ID_SIZE;
                            }
                            SCI_MEMCPY(time_id_buf,msg_char,msg_len);
                            ret++;
                        }
                        ZDT_LOG("alarmID[%d] len=%d[%s],%s",i-SHB_TIME_TYPE_LOC_1,msg_len,time_id_buf,msg_char);
                    }
                }
                item_json = cJSON_GetObjectItem(arry_json,"startTime");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("startTime[%d] len=%d,%s",i-SHB_TIME_TYPE_LOC_1,msg_len,msg_char);
                        if(msg_len > 3)
                        {
                            buf[0] = msg_char[0];
                            buf[1] = msg_char[1];
                            buf[2] = 0;
                            start_hour = atoi(buf);
                            buf[0] = msg_char[2];
                            buf[1] = msg_char[3];
                            start_min = atoi(buf);
                            timer_start = (start_hour * 3600) + (start_min * 60);
                            ret++;
                        }
                    }
                }
               item_json = cJSON_GetObjectItem(arry_json,"endTime");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("endTime[%d] len=%d,%s",i-SHB_TIME_TYPE_LOC_1,msg_len,msg_char);
                        if(msg_len > 3)
                        {
                            buf[0] = msg_char[0];
                            buf[1] = msg_char[1];
                            buf[2] = 0;
                            end_hour = atoi(buf);
                            buf[0] = msg_char[2];
                            buf[1] = msg_char[3];
                            end_min = atoi(buf);
                            timer_end = (end_hour * 3600) + (end_min * 60);
                            ret++;
                        }
                    }
                }
                
                item_json = cJSON_GetObjectItem(arry_json,"interval");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("interval[%d] len=%d,%s",i-SHB_TIME_TYPE_LOC_1,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            val = atoi(msg_char);
                            timer_interval_min = val;
                            ret++;
                        }
                    }
                }
                
                SHB_DB_TIMER_ListModify(i,time_id_buf,time_set,timer_start,timer_end,timer_interval_min);
            }
        }
        SHB_DB_TIMER_ListSave();
    }
    return ret;
}

static BOOLEAN SHB_Clock_SaveNV(uint16 idx,uint16 Hour,uint16 Minute,uint8 onoff,uint8 clock_week,uint8 * pConten, uint16 content_len)
{
    uint8 i = 0;
    uint8 clock_type;
    uint32 pwrofftime = 0;
    uint8 is_on = onoff;
    uint16 cur_id = idx+EVENT_ALARM_0;
    uint32          cur_seconds = 0;
    uint32          res_seconds = 0;
    SCI_TIME_T          time;
    SCI_DATE_T          date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    
    if(clock_week >= 0xFE)
    {
        clock_type = 2;
    }
    else
    {
        clock_type = 3;
    }
    
    if(Hour >= 24)
    {
        return FALSE;
    }
    if(Minute >= 60)
    {
        return FALSE;
    }
    if(clock_week & 0x80)
    {
        clock_week |= 0x01;
        clock_week &= 0x7F;
    }
    else
    {
        clock_week &= 0xFE;
    }
    SCI_MEMSET(&s_shb_current_event_info,0,sizeof(EVENT_CURRENT_DATA_T));
    
    s_shb_current_event_info.id = cur_id;

    MMIAPIALM_FormInteEventInfo(s_shb_current_event_info.id,&s_shb_current_event_info.event);
    
    s_shb_current_event_info.type = SMART_EVENT_ALARM;
    s_shb_current_event_info.event.event_fast_info.is_valid = TRUE;
    if(onoff)
    {
        s_shb_current_event_info.event.event_fast_info.is_on = TRUE;
    }
    else
    {
        s_shb_current_event_info.event.event_fast_info.is_on = FALSE;
    }
    s_shb_current_event_info.event.event_fast_info.snooze_type = ALM_SNOOZE_TIME_10_MIN;
    s_shb_current_event_info.event.event_fast_info.hour = Hour;
    s_shb_current_event_info.event.event_fast_info.minute = Minute;

#if 0
    cur_seconds = MMIAPICOM_Tm2Second(time.sec, time.min, time.hour, date.mday, date.mon, date.year);
    res_seconds = MMIAPICOM_Tm2Second(0, Minute, Hour, date.mday, date.mon, date.year);

    if(cur_seconds > res_seconds)
    {
        //推迟一天
        
    }
#endif

    s_shb_current_event_info.event.event_fast_info.year = date.year;
    s_shb_current_event_info.event.event_fast_info.mon = date.mon;
    s_shb_current_event_info.event.event_fast_info.day = date.mday;
    
    if(clock_type == 1)
    {
        //一次
        s_shb_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
        
    }
    else if(clock_type == 2)
    {
        //每天
        s_shb_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
    }
    else if(clock_type == 3)
    {
        //自定义
        s_shb_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
        s_shb_current_event_info.event.event_fast_info.fre = clock_week;
    }
    else
    {
        //一次
        s_shb_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_ONCE;
    }

    s_shb_current_event_info.event.event_content_info.ring = 1;
    s_shb_current_event_info.event.event_content_info.is_add = TRUE;
    
    if(pConten != PNULL && content_len > 0)
    {
        s_shb_current_event_info.event.event_content_info.w_length = GUI_UTF8ToWstr(s_shb_current_event_info.event.event_content_info.w_content, MMIACC_EVENT_TEXT_MAX_LEN,(uint8 *)pConten, content_len);
    }
    else
    {
        s_shb_current_event_info.event.event_content_info.w_length = 3;
        s_shb_current_event_info.event.event_content_info.w_content[0] = 0x95F9;
        s_shb_current_event_info.event.event_content_info.w_content[1] = 0x949F;
        s_shb_current_event_info.event.event_content_info.w_content[2] = 0x0031+idx;
    }
     SetDelayAlarmExt(s_shb_current_event_info.id, &s_shb_current_event_info.event, FALSE);
     // 修改闹钟设置
     MMIAPIALM_SaveInteEventInfo(s_shb_current_event_info.id,&s_shb_current_event_info.event);    
    return TRUE;
}
//闹钟设置
uint8 SHB_HTTP_ZTE1000_Rcv_Handle_f08(cJSON *pObject)
{
    int result = 0;
    uint8 ret = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *arry_json = PNULL;
    int i = 0;
    int j = 0;
    int arry_size = 0;
    uint8 clock_hour = 0;
    uint8 clock_min = 0;
    uint32 clock_id= 0;
    uint8 clock_onoff= 0;
    uint8 clock_type= 0;
    uint8 clock_week= 0;
    uint32 clock_time= 0;
    uint8  clock_content[SHB_DB_CLOCK_MAX_CONTENT_SIZE+1];
    uint16 content_len= 0;
    char buf[101] = {0};
    
    ZDT_LOG("SHB_HTTP_ZTE1000_Rcv_Handle_f08");

    json=pObject;
    if (json) 
    {
        arry_size = cJSON_GetArraySize(json);
        ZDT_LOG("F08 arry_size=%d",arry_size);
        if(arry_size > SHB_DB_CLOCK_MAX_SUM)
        {
            arry_size = SHB_DB_CLOCK_MAX_SUM;
        }

        SHB_DB_CLOCK_ListDelAllBuf();
        for(i = 0; i < arry_size; i++)
        {
            clock_hour = 0;
            clock_min = 0;
            clock_id= 0;
            clock_onoff = 0;
            clock_type= 0;
            clock_week= 0;
            clock_time= 0;
            content_len = 0;
            
            arry_json = cJSON_GetArrayItem(json,i);
            if(arry_json)
            {
                item_json = cJSON_GetObjectItem(arry_json,"checkFlag");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("checkFlag[%d] len=%d,%s",i,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            val = atoi(msg_char);
                            clock_onoff = val;
                            ret++;
                        }
                    }
                }
                item_json = cJSON_GetObjectItem(arry_json,"alarmID");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("alarmID[%d] len=%d,%s",i,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            val = atoi(msg_char);
                            clock_id = val;
                            ret++;
                        }
                    }
                }
                item_json = cJSON_GetObjectItem(arry_json,"alarmWeek");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("alarmWeek[%d] len=%d,%s",i,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            if(msg_char[0] == '*')
                            {
                                clock_week = 0xFF;
                            }
                            else if(msg_char[0] == '@')
                            {
                                clock_week = 0x00;
                            }
                            else
                            {
                                clock_week = 0x01;
                                for(j = 0; j < msg_len; j++)
                                {
                                    if(msg_char[j] >= '1' && msg_char[j] <= '7')
                                    {
                                        uint8 tmp_val = msg_char[j] - '0';
                                        clock_week |= (0x01 << tmp_val);
                                    }
                                }
                            }
                            ret++;
                        }
                    }
                }
                
                item_json = cJSON_GetObjectItem(arry_json,"alarmTime");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("alarmTime[%d] len=%d,%s",i,msg_len,msg_char);
                        if(msg_len > 0)
                        {
                            if(msg_len == 12)
                            {
                                buf[0] = msg_char[8];
                                buf[1] = msg_char[9];
                                buf[2] = 0;
                                clock_hour = atoi(buf);
                                buf[0] = msg_char[10];
                                buf[1] = msg_char[11];
                                clock_min = atoi(buf);
                                clock_time = (clock_hour * 3600) + (clock_min * 60);
                            }
                            else if(msg_len > 3)
                            {
                                buf[0] = msg_char[0];
                                buf[1] = msg_char[1];
                                buf[2] = 0;
                                clock_hour = atoi(buf);
                                buf[0] = msg_char[2];
                                buf[1] = msg_char[3];
                                clock_min = atoi(buf);
                                clock_time = (clock_hour * 3600) + (clock_min * 60);
                            }
                            ret++;
                        }
                    }
                }

                
                item_json = cJSON_GetObjectItem(arry_json,"alarmTag");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("alarmTag[%d] len=%d,%s",i,msg_len,msg_char);
                        if(msg_len > 0 )
                        {
                            if(msg_len == 2 && msg_char[0] == '-' && msg_char[1] == '1')
                            {
                            }
                            else
                            {
                                content_len = msg_len;
                                if(content_len > SHB_DB_CLOCK_MAX_CONTENT_SIZE)
                                {
                                    content_len = SHB_DB_CLOCK_MAX_CONTENT_SIZE;
                                }
                                SCI_MEMCPY(clock_content,msg_char,content_len);
                            }
                            ret++;
                        }
                    }
                }

                item_json = cJSON_GetObjectItem(arry_json,"timeStamp");
                if(item_json)
                {
                    msg_char = cJSON_GetObjectString(item_json);
                    if(msg_char != PNULL)
                    {
                        msg_len = strlen(msg_char);
                        ZDT_LOG("timeStamp[%d] len=%d,%s",i,msg_len,msg_char);
                        if(msg_len > 13)
                        {
                            clock_type = SHB_Func_StrToDateTime(msg_char);
                            ret++;
                        }
                    }
                }
                SHB_DB_CLOCK_ListModifyBuf(i,clock_id,clock_onoff,clock_type,clock_week,clock_time,clock_content, content_len);
                SHB_Clock_SaveNV(i,clock_hour,clock_min,clock_onoff,clock_week,clock_content,content_len);
            }
        }
        SHB_DB_CLOCK_ListSaveBuf();
    }
    return ret;
}

BOOLEAN SHB_HTTP_ZTE1000_Rcv_Handle(uint8 * pContent,uint16 ContentLen)
{
    uint8 ret = 0;
    uint8 * decrypt_char;
    uint32 decrypt_len = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *content_json = PNULL;
    cJSON *cur_json = PNULL;

    if(pContent == PNULL || ContentLen < 2)
    {
        return FALSE;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"resultType");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("resultType %s",msg_char);
                if(msg_len != 6 || strncmp( (char *)msg_char, "AC1000", 6 ) != 0)
                {
                    ZDT_LOG("AC1000 resultType Err");
                    cJSON_Delete(json);
                    return 0;
                }

            }
        }
        else
        {
            ZDT_LOG("resultType NONE");
            cJSON_Delete(json);
            return 0;
        }
        item_json = cJSON_GetObjectItem(json,"encryptCode");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("encryptCode=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("encryptCode NONE");
        }
        item_json = cJSON_GetObjectItem(json,"resContent");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("resContent=%s",msg_char);
                msg_len = strlen(msg_char);
                decrypt_char = SHB_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                if(decrypt_char != NULL)
                {
                    content_json=cJSON_Parse(decrypt_char);
                    if(content_json)
                    {
                        cur_json = cJSON_GetObjectItem(content_json,"code");
                        if(cur_json)
                        {
                            msg_char = cJSON_GetObjectString(cur_json);
                            if(msg_char != PNULL)
                            {
                                val = atoi(msg_char);
                                ZDT_LOG("code=%d,%s",val,msg_char);
                            }
                        }
                        else
                        {
                            ZDT_LOG("code NONE");
                        }
                        if(val == 200)
                        {   
                            cur_json = cJSON_GetObjectItem(content_json,"f00");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f00(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f01");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f01(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f02");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f02(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f03");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f03(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f04");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f04(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f05");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f05(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f06");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f06(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f07");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f07(cur_json);
                            }
                            cur_json = cJSON_GetObjectItem(content_json,"f08");
                            if(cur_json)
                            {
                                SHB_HTTP_ZTE1000_Rcv_Handle_f08(cur_json);
                            }
                            ret = 1;
                            shb_DB_Set_Rec.is_reg = 1;
                             SHB_DB_SET_ListModify();
                        }
                        cJSON_Delete(content_json);
                    }
                    else
                    {
                        ZDT_LOG("content_json NONE");
                    }

                    SCI_FREE(decrypt_char);
                }
                
            }
        }
        else
        {
            ZDT_LOG("resContent NONE");
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YJ_HTTP_Send_PowerOn_CB ERR  cJSON_Parse");
    }
    return ret;
}
int SHB_HTTP_ZTE1000_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    static BOOLEAN s_is_zte1000_need_reg = FALSE;
    ZDT_LOG("SHB_HTTP_ZTE1000_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            if(SHB_HTTP_ZTE1000_Rcv_Handle(pRcv,Rcv_len))
            {
                if(strlen(shb_DB_PlatSet_Rec.mobile) == 0)
                {
                    if(s_is_zte1000_need_reg == FALSE)
                    {
                        SHB_HTTP_ZTE2200_Send();
                        s_is_zte1000_need_reg = TRUE;
                    }
                }
                else
                {
                    if(g_shb_is_land_ok == 0)
                    {
                        SHB_HTTP_ZTE1001_Send(1);
                        g_shb_is_land_ok = 1;
                    }
                }
            }
        }
    }
    return 0;
}

BOOLEAN  SHB_HTTP_ZTE1000_Send(void)
{
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * out = PNULL;
    char * pContent = PNULL;
    char act_str[51] = {0};
    uint32 act_len = 0;
    char md5_key_str[51] = {0};
    uint32 md5_key_len = 0;
    char app_domain[MAX_SHB_APP_DOMAIN_SIZE+1] = {0};
    uint16 len = 0;
    char content_buf[1024] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;
    uint8  * md5_data_in = PNULL;
    uint32 md5_len_in = 0;
    char md5_str[32] = {0};

    ZDT_LOG("SHB_HTTP_ZTE1000_Send");
    if(FALSE == SHB_Net_Is_land())
    {
        ZDT_LOG("SHB_HTTP_ZTE1000_Send Err NO Land");
        return FALSE;
    }
    //加密
    reqContent=cJSON_CreateObject();
    if(reqContent)
    {
        content_len = SHB_Get_Send_Index_Str(content_buf);
        cJSON_AddItemToObject(reqContent, "seqNum", cJSON_CreateString(content_buf));
        cJSON_AddItemToObject(reqContent, "ICCID", cJSON_CreateString(g_zdt_sim_iccid));
        cJSON_AddItemToObject(reqContent, "IMEI", cJSON_CreateString(g_zdt_phone_imei));
        cJSON_AddItemToObject(reqContent, "type", cJSON_CreateString("f0000"));
        content_len = SHB_Func_GetDateTime(content_buf);
        cJSON_AddItemToObject(reqContent, "timestamp", cJSON_CreateString(content_buf));
        pContent=cJSON_PrintUnformatted(reqContent);
        if(pContent)
        {
            content_len = strlen(pContent);
            crypt_data = SHB_Func_Encrypt(pContent,content_len,&crypt_len);
            SCI_FREE(pContent);
        }
        cJSON_Delete(reqContent);
    }
    
    if(crypt_data != PNULL)
    {
        //MD5 签权
        strcpy(act_str,"AC1000");
        act_len = strlen(act_str);
        strcpy(md5_key_str,SHB_MD5_SECKEY_STR);
        md5_key_len = strlen(md5_key_str);
        md5_len_in = act_len + md5_key_len + crypt_len;
        md5_data_in = SCI_ALLOC_APPZ(md5_len_in);
        ZDT_LOG("[CRYPT]:md5_key_len=%d,act_len=%d,act_str=%s,%s",md5_key_len,act_len,act_str,md5_key_str);
        if(md5_data_in != PNULL)
        {
            SCI_MEMCPY(md5_data_in,act_str,act_len);
            SCI_MEMCPY(md5_data_in+act_len,crypt_data,crypt_len);
            SCI_MEMCPY(md5_data_in+act_len+crypt_len,md5_key_str,md5_key_len);
            SHB_Func_MD5_Generate((uint8 *)md5_data_in,md5_len_in,md5_str);
            SCI_FREE(md5_data_in);
            ZDT_LOG("[CRYPT]:MD5=%s",md5_str);
        }
        root=cJSON_CreateObject();
        if(root)
        {
            sprintf(app_domain,"%s",shb_DB_AppSet_Rec.app_domain);
            cJSON_AddItemToObject(root, "actType", cJSON_CreateString(act_str));
            cJSON_AddItemToObject(root, "reqContent", cJSON_CreateString(crypt_data));
            cJSON_AddItemToObject(root, "encryptCode", cJSON_CreateString(md5_str));
            out=cJSON_PrintUnformatted(root);
            if(out)
            {
                len = strlen(out);
                ZDT_CryptTrace(out,len);
                MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)out,len,1000,0,0,0,0,0,SHB_HTTP_ZTE1000_Send_CB);
                SCI_FREE(out);
            }
            cJSON_Delete(root);
        }
        SCI_FREE(crypt_data);            
    }
    return TRUE;
}

int SHB_HTTP_ZTE1001_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    ZDT_LOG("SHB_HTTP_ZTE1001_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
        }
    }
    return 0;
}

BOOLEAN  SHB_HTTP_ZTE1001_Send(uint8 type)
{
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    char * out = PNULL;
    char * pContent = PNULL;
    char act_str[51] = {0};
    uint32 act_len = 0;
    char md5_key_str[51] = {0};
    uint32 md5_key_len = 0;
    char app_domain[MAX_SHB_APP_DOMAIN_SIZE+1] = {0};
    char send_buf[200] = {0};
    uint16 len = 0;
    char content_buf[1024] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;
    uint8  * md5_data_in = PNULL;
    uint32 md5_len_in = 0;
    char md5_str[32] = {0};

    ZDT_LOG("SHB_HTTP_ZTE1001_Send");

    if(FALSE == SHB_Net_Is_land())
    {
        ZDT_LOG("SHB_HTTP_ZTE1001_Send Err NO Land");
        return FALSE;
    }
    //加密
    reqContent=cJSON_CreateObject();
    if(reqContent)
    {
        content_len = SHB_Get_Send_Index_Str(content_buf);
        cJSON_AddItemToObject(reqContent, "seqNum", cJSON_CreateString(content_buf));
        cJSON_AddItemToObject(reqContent, "IMEI", cJSON_CreateString(g_zdt_phone_imei));
        sprintf(send_buf,"%d",type);
        cJSON_AddItemToObject(reqContent, "pwr", cJSON_CreateString(send_buf));
        if(ZDT_GetIsBatLow())
        {
            cJSON_AddItemToObject(reqContent, "lowBatt", cJSON_CreateString("1"));
        }
        else
        {
            cJSON_AddItemToObject(reqContent, "lowBatt", cJSON_CreateString("0"));
        }
        SHB_Func_GetBatPercent(send_buf);
        cJSON_AddItemToObject(reqContent, "battery", cJSON_CreateString(send_buf));
        SHB_Func_GetDateTime(send_buf);
        cJSON_AddItemToObject(reqContent, "timestamp", cJSON_CreateString(send_buf));
        
        pContent=cJSON_PrintUnformatted(reqContent);
        if(pContent)
        {
            content_len = strlen(pContent);
            crypt_data = SHB_Func_Encrypt(pContent,content_len,&crypt_len);
            SCI_FREE(pContent);
        }
        cJSON_Delete(reqContent);
    }
    
    if(crypt_data != PNULL)
    {
        //MD5 签权
        strcpy(act_str,"AC4100");
        act_len = strlen(act_str);
        strcpy(md5_key_str,SHB_MD5_SECKEY_STR);
        md5_key_len = strlen(md5_key_str);
        md5_len_in = act_len + md5_key_len + crypt_len;
        md5_data_in = SCI_ALLOC_APPZ(md5_len_in);
        ZDT_LOG("[CRYPT]:md5_key_len=%d,act_len=%d,act_str=%s,%s",md5_key_len,act_len,act_str,md5_key_str);
        if(md5_data_in != PNULL)
        {
            SCI_MEMCPY(md5_data_in,act_str,act_len);
            SCI_MEMCPY(md5_data_in+act_len,crypt_data,crypt_len);
            SCI_MEMCPY(md5_data_in+act_len+crypt_len,md5_key_str,md5_key_len);
            SHB_Func_MD5_Generate((uint8 *)md5_data_in,md5_len_in,md5_str);
            SCI_FREE(md5_data_in);
            ZDT_LOG("[CRYPT]:MD5=%s",md5_str);
        }
        root=cJSON_CreateObject();
        if(root)
        {
            sprintf(app_domain,"%s",shb_DB_AppSet_Rec.app_domain);
            cJSON_AddItemToObject(root, "actType", cJSON_CreateString(act_str));
            cJSON_AddItemToObject(root, "reqContent", cJSON_CreateString(crypt_data));
            cJSON_AddItemToObject(root, "encryptCode", cJSON_CreateString(md5_str));
            out=cJSON_PrintUnformatted(root);
            if(out)
            {
                len = strlen(out);
                ZDT_CryptTrace(out,len);
                MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)out,len,1001,0,0,0,0,0,SHB_HTTP_ZTE1001_Send_CB);
                SCI_FREE(out);
            }
            cJSON_Delete(root);
        }
        SCI_FREE(crypt_data);            
    }
    return TRUE;
}

uint8 SHB_HTTP_Loction_Rcv_Handle(uint8 * pContent,uint32 ContentLen)
{
    BOOLEAN is_sos = FALSE;
    uint8 ret = 0;
    uint8 * decrypt_char;
    uint32 decrypt_len = 0;
    char *msg_char;
    uint32 msg_len = 0;
    int val = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    cJSON *content_json = PNULL;
    cJSON *cur_json = PNULL;

    if(pContent == PNULL || ContentLen < 2)
    {
        return FALSE;
    }
    json=cJSON_Parse(pContent);
    if (json) 
    {
        item_json = cJSON_GetObjectItem(json,"resultType");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                msg_len = strlen(msg_char);
                ZDT_LOG("resultType %s",msg_char);
                if(msg_len == 6 && strncmp( (char *)msg_char, "AC1003", 6 ) == 0)
                {
                    //PDW 短信定位
                    ZDT_LOG("Loction CB  AC1003");
                    ret = SHB_HTTP_LOC_TYPE_DW;
                }
                else if(msg_len == 6 && strncmp( (char *)msg_char, "AC1006", 6 ) == 0)
                {
                    //SOS 定位
                    ZDT_LOG("Loction CB  AC1006");
                    ret = SHB_HTTP_LOC_TYPE_SOS;
                    is_sos = TRUE;
                }
                else if(msg_len == 6 && strncmp( (char *)msg_char, "AC1008", 6 ) == 0)
                {
                    //运动
                    ZDT_LOG("Loction CB  AC1008");
                    ret = SHB_HTTP_LOC_TYPE_LONG;
                }
                else if(msg_len == 6 && strncmp( (char *)msg_char, "AC1010", 6 ) == 0)
                {
                    //电子围柆
                    ZDT_LOG("Loction CB  AC1010");
                    ret = SHB_HTTP_LOC_TYPE_WL;
                }
            }
        }
        else
        {
            ZDT_LOG("resultType NONE");
            cJSON_Delete(json);
            return 0;
        }
        item_json = cJSON_GetObjectItem(json,"encryptCode");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("encryptCode=%s",msg_char);
            }
        }
        else
        {
            ZDT_LOG("encryptCode NONE");
        }
        item_json = cJSON_GetObjectItem(json,"resContent");
        if(item_json)
        {
            msg_char = cJSON_GetObjectString(item_json);
            if(msg_char != PNULL)
            {
                ZDT_LOG("resContent=%s",msg_char);
                msg_len = strlen(msg_char);
                decrypt_char = SHB_Func_Decrypt(msg_char,msg_len,&decrypt_len);
                if(decrypt_char != NULL)
                {
                    content_json=cJSON_Parse(decrypt_char);
                    if(content_json)
                    {
                        cur_json = cJSON_GetObjectItem(content_json,"code");
                        if(cur_json)
                        {
                            msg_char = cJSON_GetObjectString(cur_json);
                            if(msg_char != PNULL)
                            {
                                val = atoi(msg_char);
                                ZDT_LOG("code=%d,%s",val,msg_char);
                            }
                        }
                        else
                        {
                            ZDT_LOG("code NONE");
                        }

                        cur_json = cJSON_GetObjectItem(content_json,"position");
                        if(cur_json)
                        {
                            msg_char = cJSON_GetObjectString(cur_json);
                            if(msg_char != PNULL)
                            {
                                msg_len = strlen(msg_char);
                                if(msg_len > 0)
                                {
                                    ZDT_LOG("position=%d,%s",msg_len,msg_char);
                                    if(is_sos)
                                    {
                                        ret = SHB_HTTP_LOC_TYPE_SOS_OK;
                                        if(s_shb_sos_need_sms)
                                        {
                                            SHB_API_SMS_SendSOS(msg_char,msg_len);
                                            s_shb_sos_need_sms = FALSE;
                                        }
                                    }
                                }
                                
                            }
                        }
                        else
                        {
                            ZDT_LOG("position NONE");
                        }
                        cJSON_Delete(content_json);
                    }
                    else
                    {
                        ZDT_LOG("content_json NONE");
                    }

                    SCI_FREE(decrypt_char);
                }
                
            }
        }
        else
        {
            ZDT_LOG("resContent NONE");
        }
        cJSON_Delete(json);
    }
    else
    {
        ZDT_LOG("YJ_HTTP_Send_PowerOn_CB ERR  cJSON_Parse");
    }
    return ret;
}
int SHB_HTTP_Send_Loction_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    ZDT_LOG("SHB_HTTP_Send_Loction_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            uint8 ret = SHB_HTTP_Loction_Rcv_Handle(pRcv,Rcv_len);
            if(ret == SHB_HTTP_LOC_TYPE_SOS)
            {
                if(s_shb_sos_need_sms)
                {
                    SHB_API_SMS_SendSOSExt();
                    s_shb_sos_need_sms = FALSE;
                }
            }
        }
    }
    else
    {
        if(s_shb_is_send_sos)
        {
            if(s_shb_sos_need_sms)
            {
                SHB_API_SMS_SendSOSExt();
                s_shb_sos_need_sms = FALSE;
            }
        }
    }
    if(s_shb_is_send_sos)
    {
        s_shb_is_send_sos = FALSE;
    }
    return 0;
}
BOOLEAN  SHB_HTTP_Send_Loction(uint8 is_loc_ok,const char * loc_type,uint8 * alarmID)
{
    BOOLEAN send_result = FALSE;
    cJSON *root = PNULL;
    cJSON *reqContent = PNULL;
    cJSON *wifi_json = PNULL;
    cJSON *wifi_item = PNULL;
    cJSON *lbs_json = PNULL;
    cJSON *lbs_item = PNULL;
    char * out = PNULL;
    char * pContent = PNULL;
    char act_str[51] = {0};
    uint32 act_len = 0;
    char md5_key_str[51] = {0};
    uint32 md5_key_len = 0;
    char app_domain[MAX_SHB_APP_DOMAIN_SIZE+1] = {0};
    char send_buf[200] = {0};
    uint16 len = 0;
    char content_buf[1024] = {0};
    uint16 content_len = 0;
    uint8  * crypt_data = PNULL;
    uint32 crypt_len = 0;
    uint8  * md5_data_in = PNULL;
    uint32 md5_len_in = 0;
    int i = 0;
    char md5_str[32] = {0};
    char md5_str2[33] = {0};
    ZDT_LOG("SHB_HTTP_Send_Loction %s",loc_type);
    if(FALSE == SHB_Net_Is_land())
    {
        ZDT_LOG("SHB_HTTP_Send_Loction Err NO Land");
        return FALSE;
    }
    
    MMIZDT_Station_Update();
    //加密
    reqContent=cJSON_CreateObject();
    if(reqContent)
    {
        content_len = SHB_Get_Send_Index_Str(content_buf);
        cJSON_AddItemToObject(reqContent, "seqNum", cJSON_CreateString(content_buf));
        cJSON_AddItemToObject(reqContent, "ver", cJSON_CreateString(SHB_SOFT_VERSION));
        cJSON_AddItemToObject(reqContent, "IMEI", cJSON_CreateString(g_zdt_phone_imei));
        if(is_loc_ok & 0x01)
        {
            uint8 temp_buf[100] = {0};
            uint16 temp_len = 0;
            GPS_DATA_T gps_data = {0};
            
            GPS_API_GetLastPos(&gps_data);
            
            temp_len = SHB_Func_GetGPSLat(temp_buf,gps_data.Latitude,gps_data.Lat_m,gps_data.Lat_Dir);
            cJSON_AddItemToObject(reqContent, "lat", cJSON_CreateString(temp_buf));
            
            SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
            temp_len = SHB_Func_GetGPSLong(temp_buf,gps_data.Longitude,gps_data.Long_Dir);
            cJSON_AddItemToObject(reqContent, "lng", cJSON_CreateString(temp_buf));
            cJSON_AddItemToObject(reqContent, "pm", cJSON_CreateString("1"));
        }
        else
        {
            cJSON_AddItemToObject(reqContent, "lat", cJSON_CreateString(""));
            cJSON_AddItemToObject(reqContent, "lng", cJSON_CreateString(""));
            if(is_loc_ok & 0x02)
            {
                cJSON_AddItemToObject(reqContent, "pm", cJSON_CreateString("4"));
            }
            else
            {
                cJSON_AddItemToObject(reqContent, "pm", cJSON_CreateString("3"));
            }
        }

        SCI_MEMSET(send_buf,0,sizeof(send_buf));
        sprintf(send_buf,"%d",g_zdt_station_data.s_cell.mcc);
        cJSON_AddItemToObject(reqContent, "mcc", cJSON_CreateString(send_buf));

        SCI_MEMSET(send_buf,0,sizeof(send_buf));
        sprintf(send_buf,"%d",g_zdt_station_data.s_cell.mnc);
        cJSON_AddItemToObject(reqContent, "mnc", cJSON_CreateString(send_buf));

        //基站信息
        lbs_json = cJSON_CreateArray();
        if(lbs_json)
        {
            if(g_zdt_station_data.s_cell.is_valid)
            {
                lbs_item = cJSON_CreateObject();
                if(lbs_item)
                {
                    SCI_MEMSET(send_buf,0,sizeof(send_buf));
                    sprintf(send_buf,"%d",g_zdt_station_data.s_cell.lac);
                    cJSON_AddItemToObject(lbs_item, "lac", cJSON_CreateString(send_buf));
                    
                    SCI_MEMSET(send_buf,0,sizeof(send_buf));
                    sprintf(send_buf,"%d",g_zdt_station_data.s_cell.cell_id);
                    cJSON_AddItemToObject(lbs_item, "cellID", cJSON_CreateString(send_buf));
                    
                    SCI_MEMSET(send_buf,0,sizeof(send_buf));
                    sprintf(send_buf,"%d",g_zdt_station_data.s_cell.rssiLevel);
                    cJSON_AddItemToObject(lbs_item, "rssi", cJSON_CreateString(send_buf));

                    cJSON_AddItemToArray(lbs_json, lbs_item);
                }                
                for (i = 0; i < ZDT_N_STATION_NUM; i++)		// this is for example for simulator
                {
                    if(g_zdt_station_data.n_cell[i].is_valid)
                    {
                        lbs_item = cJSON_CreateObject();
                        if(lbs_item)
                        {
                            SCI_MEMSET(send_buf,0,sizeof(send_buf));
                            sprintf(send_buf,"%d",g_zdt_station_data.s_cell.lac);
                            cJSON_AddItemToObject(lbs_item, "lac", cJSON_CreateString(send_buf));
                            
                            SCI_MEMSET(send_buf,0,sizeof(send_buf));
                            sprintf(send_buf,"%d",g_zdt_station_data.n_cell[i].cell_id);
                            cJSON_AddItemToObject(lbs_item, "cellID", cJSON_CreateString(send_buf));
                            
                            SCI_MEMSET(send_buf,0,sizeof(send_buf));
                            sprintf(send_buf,"%d",g_zdt_station_data.n_cell[i].rssiLevel);
                            cJSON_AddItemToObject(lbs_item, "rssi", cJSON_CreateString(send_buf));

                            cJSON_AddItemToArray(lbs_json, lbs_item);
                        }
                    }
                }
            }
            cJSON_AddItemToObject(reqContent, "stationInfo", lbs_json);
        }
        
        
        //WIFI信息
        wifi_json = cJSON_CreateArray();
        if(wifi_json)
        {
            if(is_loc_ok & 0x02)
            {
                ZDT_WIFI_DATA_T last_wifi = {0};
                uint8 wifi_num = 0;
                wifi_num = ZDT_WIFI_Get_DataNum();
                if(wifi_num > 3)
                {
                    wifi_num = 3;
                }
                
                for (i = 0; i < wifi_num; i++)		// this is for example for simulator
                {
                    if(ZDT_WIFI_Get_Data(i,&last_wifi))
                    {
                        wifi_item = cJSON_CreateObject();
                        if(wifi_item)
                        {
                            SCI_MEMSET(send_buf,0,sizeof(send_buf));
                            sprintf(send_buf,"%02x:%02x:%02x:%02x:%02x:%02x",last_wifi.bssid[0],last_wifi.bssid[1],last_wifi.bssid[2],last_wifi.bssid[3],last_wifi.bssid[4],last_wifi.bssid[5]);
                            cJSON_AddItemToObject(wifi_item, "wifiAddr", cJSON_CreateString(send_buf));
                            
                            SCI_MEMSET(send_buf,0,sizeof(send_buf));
                            sprintf(send_buf,"%d",last_wifi.rssi);
                            cJSON_AddItemToObject(wifi_item, "signal", cJSON_CreateString(send_buf));
                            
                            cJSON_AddItemToArray(wifi_json, wifi_item);
                        }
                    }
                }
            }
            cJSON_AddItemToObject(reqContent, "macInfo", wifi_json);
        }
        
        SCI_MEMSET(send_buf,0,sizeof(send_buf));
        if(alarmID != PNULL)
        {
            sprintf(send_buf,"%s",alarmID);
        }
        else
        {
            sprintf(send_buf,"0");
        }
        cJSON_AddItemToObject(reqContent, "relaAlarmID", cJSON_CreateString(send_buf));
        
        SHB_Func_GetBatPercent(send_buf);
        cJSON_AddItemToObject(reqContent, "battery", cJSON_CreateString(send_buf));
        SHB_Func_GetDateTime(send_buf);
        cJSON_AddItemToObject(reqContent, "timestamp", cJSON_CreateString(send_buf));
        
        pContent=cJSON_PrintUnformatted(reqContent);
        if(pContent)
        {
            content_len = strlen(pContent);
            crypt_data = SHB_Func_Encrypt(pContent,content_len,&crypt_len);
            SCI_FREE(pContent);
        }
        cJSON_Delete(reqContent);
    }
    
    if(crypt_data != PNULL)
    {
        //MD5 签权
        strcpy(act_str,loc_type);
        act_len = strlen(act_str);
        strcpy(md5_key_str,SHB_MD5_SECKEY_STR);
        md5_key_len = strlen(md5_key_str);
        md5_len_in = act_len + md5_key_len + crypt_len;
        md5_data_in = SCI_ALLOC_APPZ(md5_len_in);
        ZDT_LOG("[CRYPT]:md5_key_len=%d,act_len=%d,act_str=%s,%s",md5_key_len,act_len,act_str,md5_key_str);
        if(md5_data_in != PNULL)
        {
            SCI_MEMCPY(md5_data_in,act_str,act_len);
            SCI_MEMCPY(md5_data_in+act_len,crypt_data,crypt_len);
            SCI_MEMCPY(md5_data_in+act_len+crypt_len,md5_key_str,md5_key_len);
            SHB_Func_MD5_Generate((uint8 *)md5_data_in,md5_len_in,md5_str);
            SCI_MEMCPY(md5_str2,md5_str,32);
            SCI_FREE(md5_data_in);
            ZDT_LOG("[CRYPT]:MD5=%s",md5_str2);
        }
        root=cJSON_CreateObject();
        if(root)
        {
            sprintf(app_domain,"%s",shb_DB_AppSet_Rec.app_domain);
            cJSON_AddItemToObject(root, "actType", cJSON_CreateString(act_str));
            cJSON_AddItemToObject(root, "reqContent", cJSON_CreateString(crypt_data));
            cJSON_AddItemToObject(root, "encryptCode", cJSON_CreateString(md5_str2));
            out=cJSON_PrintUnformatted(root);
            if(out)
            {
                len = strlen(out);
                ZDT_CryptTrace(out,len);
                send_result = MMIZDT_HTTP_AppSend(FALSE,app_domain,(uint8 *)out,len,1001,0,0,0,0,0,SHB_HTTP_Send_Loction_CB);
                SCI_FREE(out);
            }
            cJSON_Delete(root);
        }
        SCI_FREE(crypt_data);            
    }
    return send_result;
}


BOOLEAN  SHB_HTTP_ZTE1003_Send(uint8 is_loc_ok)
{
    //PDW 短信定位
    if(SHB_Net_Is_land())
    {
        ZDT_LOG("SHB_HTTP_ZTE1003_Send is_loc_ok = %d",is_loc_ok);
        SHB_HTTP_Send_Loction(is_loc_ok,"AC1003",0);
    }
    return TRUE;
}

BOOLEAN  SHB_HTTP_ZTE1006_Send(uint8 is_loc_ok)
{
    //SOS 定位消息
    if(SHB_Net_Is_land())
    {
        ZDT_LOG("SHB_HTTP_ZTE1006_Send is_loc_ok=%d",is_loc_ok);
        if(SHB_HTTP_Send_Loction(is_loc_ok,"AC1006",0))
        {
            s_shb_is_send_sos = TRUE;
            return TRUE;
        }
        else
        {
            SHB_API_SMS_SendSOSExt();
        }
    }
    else
    {
        SHB_API_SMS_SendSOSExt();
    }
    return FALSE;
}

BOOLEAN  SHB_HTTP_ZTE1008_Send(uint8 is_loc_ok)
{
    //连续定位
    if(SHB_Net_Is_land())
    {
        ZDT_LOG("SHB_HTTP_ZTE1008_Send is_loc_ok",is_loc_ok);
        SHB_HTTP_Send_Loction(is_loc_ok,"AC1008",g_shb_cr_time_id);
    }
    return TRUE;
}

BOOLEAN  SHB_HTTP_ZTE1010_Send(uint8 is_loc_ok)
{
    //电子围栏定位
    if(SHB_Net_Is_land())
    {
        ZDT_LOG("SHB_HTTP_ZTE1010_Send is_loc_ok = %d",is_loc_ok);
        SHB_HTTP_Send_Loction(is_loc_ok,"AC1010",g_shb_wl_time_id);
    }
    return TRUE;
}

int SHB_HTTP_ZTE1211_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    ZDT_LOG("SHB_HTTP_ZTE1008_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
        }
    }
    return 0;
}

BOOLEAN  SHB_HTTP_ZTE1211_Send(char* num, uint8 result)
{
    #if 0
    char send_buf[512] = {0};
    uint16 len = 0;
    char time_buf[100] = {0};
    ZDT_LOG("SHB_HTTP_ZTE1211_Send");
    SHB_Func_GetDateTime(time_buf);
    sprintf(send_buf,"actType=ZTE1211&reqContent=%05d|%s|%s|%s|%d",SHB_Get_Send_Index(),shb_DB_AppSet_Rec.slef_num,num,time_buf,result);
    len = strlen(send_buf);
    MMIZDT_HTTP_AppSend(TRUE,(char *)shb_DB_Set_Rec.ip_domain,(uint8 *)send_buf,len,1211,0,0,0,0,0,SHB_HTTP_ZTE1211_Send_CB);
    #endif
    return TRUE;
}

BOOLEAN  SHB_HTTP_SOS_Send(void)
{
#ifdef ZDT_LED_SUPPORT
        ZDT_Led_Start_Call_Incoming_Over();
#endif
    if(SHB_Net_IsInit() && SHB_Net_IsAllow())
    {
        if(SHB_HTTP_ZTE1006_Send(0))
        {
            s_shb_sos_need_sms = TRUE;
        }
        MMIZDT_SHB_GPS_Loction_SOS_Start();
    }
    else
    {
        SHB_API_SMS_SendSOSExt();
    }
    return TRUE;
}


BOOLEAN  SHB_HTTP_RegCheck(void)
{
    if(shb_DB_AppSet_Rec.db_id == 0 || SCI_STRLEN((char *)shb_DB_AppSet_Rec.app_domain) == 0)
    {
        ZDT_LOG("SHB_HTTP_RegCheck  1");
        SHB_HTTP_ZTE2100_Send(SHB_HTTP_ZTE2100_Send_CheckCB);
    }
    else
    {
        if(strncmp((char *)shb_DB_AppSet_Rec.iccid,g_zdt_sim_iccid,20) == 0
            && SCI_STRLEN((char *)shb_DB_AppSet_Rec.slef_num) > 0
            && SCI_STRLEN((char *)shb_DB_AppSet_Rec.app_domain) > 0)
        {
            //相同不用重新注册
            ZDT_LOG("SHB_HTTP_RegCheck  2");
            shb_DB_Set_Rec.is_land = 1;
            SHB_DB_SET_ListModify();
            SHB_HTTP_ZTE1000_Send();
        }
        else
        {
            ZDT_LOG("SHB_HTTP_RegCheck  3");
            SHB_HTTP_ZTE2100_Send(SHB_HTTP_ZTE2100_Send_CheckCB);
        }
    }
    return TRUE;
}
extern BOOLEAN SHB_Net_IsAllow(void)
{
    if(shb_DB_Set_Rec.net_open)
    {
        return TRUE;
    }
    ZDT_LOG("SHB_Net_IsAllow FALSE");
    return FALSE;
}

extern BOOLEAN SHB_Net_IsOpen(SHB_APP_T* pMe)
{
    return pMe->m_shb_net_is_open;
}

extern int SHB_Net_Open(SHB_APP_T* pMe)
{
    ZDT_LOG("SHB_Net_Open");
    pMe->m_shb_net_is_open = TRUE;
    return 0;
}

extern int SHB_Net_Close(SHB_APP_T* pMe)
{
    ZDT_LOG("SHB_Net_Close");
    pMe->m_shb_net_is_open = FALSE;
   return 0;
}

extern int SHB_Net_Init(void)
{
    SHB_APP_T * pMe = &g_shb_app;
    MN_DUAL_SYS_E cur_sys = MMIZDT_Net_GetActiveSys();
    ZDT_LOG("SHB_Net_Init cur_sys=%d,dest_sys=%d",s_shb_http_cur_sys,cur_sys);
    pMe->m_shb_net_is_init = TRUE;
    //ZdtSHB_FirstAlertWin();
    MMIZDT_SHB_NV_SetAlert(0);
    if(s_shb_http_cur_sys != cur_sys)
    {
        s_shb_http_cur_sys = cur_sys;
        if(SHB_Net_IsAllow() && 0 == MMIZDT_SHB_GetAlert())
        {
            SHB_HTTP_RegCheck();
        }
    }
    return 0;
}

BOOLEAN SHB_Net_IsInit(void)
{
    return g_shb_app.m_shb_net_is_init;
}

extern int SHB_Net_Reset(SHB_APP_T* pMe)
{
    ZDT_LOG("SHB_Net_Reset");
    if(FALSE == SHB_Net_IsInit())
    {
        return 1;
    }
    if(SHB_Net_IsAllow())
    {
        SHB_Net_Open(pMe);
    }
    else
    {
        SHB_Net_Close(pMe);
    }
    return 0;
}

static int SHB_Net_TimerCheck_CR(SCI_DATE_T dt, SCI_TIME_T tm)
{
    uint32 interval_min = 0;
    BOOLEAN need_send = FALSE;
   if(SHB_DB_TIMER_IsValid_CR(&interval_min,g_shb_cr_time_id))
   {
        s_shb_cr_check_times++;
        if(s_shb_cr_check_times >= 0xFFFFFFF0)
        {
            s_shb_cr_check_times = 0;
        }
        ZDT_LOG("SHB_Net_TimerCheck_CR WL Valid time_id=%s,time_interval=%d,s_shb_is_in_wl=%d",g_shb_cr_time_id,interval_min,s_shb_is_in_wl);
        if(s_shb_is_in_cr == FALSE)
        {
            need_send = TRUE;
            s_shb_is_in_cr = TRUE;
        }
        else
        {
            if(interval_min == 0)
            {
                need_send = TRUE;
            }
            else if(0 == (s_shb_cr_check_times%interval_min))
            {
                need_send = TRUE;
            }
        }
        if(need_send)
        {
            MMIZDT_SHB_GPS_Loction_CR_Start();
        }
   }
   else
   {
        if(s_shb_is_in_cr)
        {
            MMIZDT_SHB_GPS_Loction_CR_Start();
            s_shb_cr_check_times = 0;
            s_shb_is_in_cr = FALSE;
        }
   }
    return 0;
}

//围栏
static int SHB_Net_TimerCheck_WL(SCI_DATE_T dt, SCI_TIME_T tm)
{
    BOOLEAN need_send = FALSE;
    uint32 interval_min = 0;
   if(SHB_DB_TIMER_IsValid_Loc(&interval_min,g_shb_wl_time_id))
   {
        s_shb_wl_check_times++;
        if(s_shb_wl_check_times >= 0xFFFFFFF0)
        {
            s_shb_wl_check_times = 0;
        }
        ZDT_LOG("SHB_Net_TimerCheck WL Valid time_id=%s,time_interval=%d,s_shb_is_in_wl=%d",g_shb_wl_time_id,interval_min,s_shb_is_in_wl);
        if(s_shb_is_in_wl == FALSE)
        {
            need_send = TRUE;
            s_shb_is_in_wl = TRUE;
        }
        else
        {
            if(interval_min == 0)
            {
                need_send = TRUE;
            }
            else if(0 == (s_shb_wl_check_times%interval_min))
            {
                need_send = TRUE;
            }
        }
        if(need_send)
        {
            MMIZDT_SHB_GPS_Loction_WL_Start();
        }
   }
   else
   {
        if(s_shb_is_in_wl)
        {
            MMIZDT_SHB_GPS_Loction_WL_Start();
            s_shb_wl_check_times = 0;
            s_shb_is_in_wl = FALSE;
        }
   }
    return 0;
}
extern int SHB_Net_TimerCheck(SCI_DATE_T dt, SCI_TIME_T tm)
{
    BOOLEAN need_send = FALSE;
    s_shb_check_times++;
    //ZDT_LOG("SHB_Net_TimerCheck s_shb_check_times=%d",s_shb_check_times);
    if(s_shb_check_times >= 0xFFFFFFF0)
    {
        s_shb_check_times = 0;
    }
    SHB_Net_TimerCheck_CR(dt,tm);
    SHB_Net_TimerCheck_WL(dt,tm);
    return 0;
}

BOOLEAN SHB_Net_Bind_Send(void)
{
    static BOOLEAN s_is_bind_sms_send = FALSE;
    ZDT_LOG("SHB_Net_Bind_Send s_is_bind_sms_send=%d",s_is_bind_sms_send);
    if(s_is_bind_sms_send == FALSE)
    {
        SHB_API_SMS_SendReg();
        s_is_bind_sms_send = TRUE;
    }
    SHB_Find_TimeStart(&g_shb_app,60);
    return TRUE;
}

extern int SHB_API_Net_Reset(void)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_SHB_NET_RESET,NULL,0);
    return 0;
}

extern int SHB_API_SeNetOpen(void)
{
    if(shb_DB_Set_Rec.net_open == 0)
    {
        shb_DB_Set_Rec.net_open = 1;
        SHB_DB_SET_ListModify();
        SHB_API_Net_Reset();
        return 1;
    }
    return 0;
}

extern int SHB_API_SeNetClose(void)
{
    if(shb_DB_Set_Rec.net_open > 0)
    {
        shb_DB_Set_Rec.net_open = 0;
        SHB_DB_SET_ListModify();
        SHB_API_Net_Reset();
        return 1;
    }
    return 0;
}

#endif
