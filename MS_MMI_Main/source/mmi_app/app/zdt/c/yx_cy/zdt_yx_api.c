#include "zdt_app.h"
#include "cJSON.h"

#ifdef ZDT_PLAT_YX_SUPPORT
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 

#define YX_LOCTION_ON_TIME_S 50
#define YX_LOCTION_FIRST_ON_TIME_S 60

static BOOLEAN s_yx_loction_is_first_on = TRUE;
uint8 s_yx_gps_loc_short_mode = 0;
static BOOLEAN YX_LOCTION_Link_AddData(YX_APP_T *pMe,uint16 type,uint8 * pParam,uint16 ParamLen)
{
    YX_LOCTION_LINK_NODE_T  * p1 = NULL;
    YX_LOCTION_LINK_NODE_T  * p2 = NULL;
    uint16 len = ParamLen;
    
    p1=(YX_LOCTION_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(YX_LOCTION_LINK_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    
    SCI_MEMSET(p1,0,sizeof(YX_LOCTION_LINK_NODE_T));

    if(len > 0 && pParam != NULL)
    {
        p1->data.pParam = (uint8 *)SCI_ALLOC_APPZ(len+1);
        if(p1->data.pParam == NULL)
        {
            SCI_FREE(p1);
            return FALSE;
        }
        SCI_MEMCPY(p1->data.pParam,pParam,len);
        p1->data.pParam[len] = 0;
    }
    p1->data.len = len;
    p1->data.type = type;
    
    p1->next_ptr = NULL;

    if(pMe->m_yx_loction_link_head == NULL)
    {
        pMe->m_yx_loction_link_head = p1;
    }
    else
    {
        p2 = pMe->m_yx_loction_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  YX_LOCTION_Link_Get(YX_APP_T *pMe,YX_LOCTION_LINK_DATA_T * pData)
{
    YX_LOCTION_LINK_NODE_T  * p1 = NULL;
    
    p1 = pMe->m_yx_loction_link_head;
    
    if(p1 != NULL)
    {
      *pData = p1->data;
        pMe->m_yx_loction_link_head = p1->next_ptr;
        SCI_FREE(p1);
        return TRUE;
    }
        
    return FALSE;
}

static BOOLEAN  YX_LOCTION_Link_TypeExsit(YX_APP_T *pMe,uint16 type)
{
    YX_LOCTION_LINK_NODE_T  * p1 = NULL;
    YX_LOCTION_LINK_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_yx_loction_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.type == type)
        {
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
    return res;
}

static BOOLEAN  YX_LOCTION_Link_GetByType(YX_APP_T *pMe,YX_LOCTION_LINK_DATA_T * pData,uint16 type)
{
    YX_LOCTION_LINK_NODE_T  * p1 = NULL;
    YX_LOCTION_LINK_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_yx_loction_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.type == type)
        {
            *pData = p1->data;
            if(p1 == pMe->m_yx_loction_link_head)
            {
                pMe->m_yx_loction_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            SCI_FREE(p1);
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

BOOLEAN  YX_LOCTION_Link_DelAll(YX_APP_T *pMe)
{
    YX_LOCTION_LINK_NODE_T  * p1 = NULL;
    
    while(pMe->m_yx_loction_link_head != NULL)
    {
        p1 = pMe->m_yx_loction_link_head;
        pMe->m_yx_loction_link_head = p1->next_ptr;
        if(p1->data.pParam!= NULL)
        {
            SCI_FREE(p1->data.pParam);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

int YX_LOCTION_CR_CallBack (uint8 loc_ok,void * pLoctionData)
{
    YX_API_LOCTION_End(loc_ok,YX_LOCTION_TYPE_CR);
    return 0;
}

int YX_LOCTION_Net_CallBack (uint8 loc_ok,void * pLoctionData)
{
    YX_API_LOCTION_End(loc_ok,YX_LOCTION_TYPE_NET);
    return 0;
}

int YX_LOCTION_Sms_CallBack (uint8 loc_ok,void * pLoctionData)
{
    YX_API_LOCTION_End(loc_ok,YX_LOCTION_TYPE_SMS);
    return 0;
}

int YX_LOCTION_Alert_CallBack (uint8 loc_ok,void * pLoctionData)
{
    YX_API_LOCTION_End(loc_ok,YX_LOCTION_TYPE_ALERT);
    return 0;
}

int YX_LOCTION_WL_CallBack (uint8 loc_ok,void * pLoctionData)
{
    YX_API_LOCTION_End(loc_ok,YX_LOCTION_TYPE_WL);
    return 0;
}

int YX_LOCTION_Net_GPS_CallBack (uint8 loc_ok,void * pLoctionData)
{
    YX_API_LOCTION_End(loc_ok,YX_LOCTION_TYPE_NET_GPS);
    return 0;
}

BOOLEAN YX_LOCTION_Start(YX_APP_T *pMe,uint16 type,uint8 * pParam,uint16 ParamLen)
{
    BOOLEAN is_exsit = FALSE;
    uint32 time_s = YX_LOC_PRE_TIME_LONG;
    if(s_yx_gps_loc_short_mode)
    {
        time_s = YX_LOC_PRE_TIME_SHORT;
    }
    if(s_yx_loction_is_first_on)
    {
        time_s = YX_LOCTION_FIRST_ON_TIME_S;
        s_yx_loction_is_first_on = FALSE;
    }
    if(YX_LOCTION_TYPE_OFF == type)
    {
        #ifdef ZDT_GPS_SUPPORT
            MMIZDTGPS_PowerOff();
        #else
            #ifdef ZDT_WIFI_SUPPORT
            #endif
        #endif
    }
    else
    {
        if(YX_LOCTION_TYPE_CR == type)
        {
            #if 0
            if(YX_LOCTION_Link_TypeExsit(pMe,type))
            {
                is_exsit =  TRUE;
            }
            #endif
            YX_LOCTION_Link_AddData(pMe,type,pParam,ParamLen);
            if(is_exsit == FALSE)
            {
                #if 0 //def ZDT_GPS_SUPPORT
                    MMIZDTGPS_API_LocStart(time_s,YX_LOCTION_CR_CallBack);
                #else
                    #ifdef ZDT_WIFI_SUPPORT
                        MMIZDTWIFI_API_LocStart(10,YX_LOCTION_CR_CallBack);
                    #endif
                #endif
            }
        }
        else if(YX_LOCTION_TYPE_NET == type)
        {
            if(YX_LOCTION_Link_TypeExsit(pMe,type))
            {
                is_exsit =  TRUE;
            }
            YX_LOCTION_Link_AddData(pMe,type,pParam,ParamLen);
            if(is_exsit == FALSE)
            {
                #if 0 //def ZDT_GPS_SUPPORT
                    MMIZDTGPS_API_LocStart(time_s,YX_LOCTION_Net_CallBack);
                #else
                    #ifdef ZDT_WIFI_SUPPORT
                        MMIZDTWIFI_API_LocStart(10,YX_LOCTION_Net_CallBack);
                    #endif
                #endif
            }
        }
        else if(YX_LOCTION_TYPE_SMS == type)
        {
            if(YX_LOCTION_Link_TypeExsit(pMe,type))
            {
                is_exsit =  TRUE;
            }
            YX_LOCTION_Link_AddData(pMe,type,pParam,ParamLen);
            if(is_exsit == FALSE)
            {
                #ifdef ZDT_GPS_SUPPORT
                    MMIZDTGPS_API_LocStart(YX_LOCTION_ON_TIME_S,YX_LOCTION_Sms_CallBack);
                #else
                    #ifdef ZDT_WIFI_SUPPORT
                        MMIZDTWIFI_API_LocStart(10,YX_LOCTION_Sms_CallBack);
                    #endif
                #endif
            }
        }
        else if(YX_LOCTION_TYPE_ALERT == type)
        {
            if(YX_LOCTION_Link_TypeExsit(pMe,type))
            {
                return TRUE;
            }
            YX_LOCTION_Link_AddData(pMe,type,pParam,ParamLen);
                #ifdef ZDT_GPS_SUPPORT
                    MMIZDTGPS_API_LocStart(YX_LOCTION_ON_TIME_S,YX_LOCTION_Alert_CallBack);
                #else
                    #ifdef ZDT_WIFI_SUPPORT
                        MMIZDTWIFI_API_LocStart(10,YX_LOCTION_Alert_CallBack);
                    #endif
                #endif
        }
        else if(YX_LOCTION_TYPE_WL == type)
        {
            if(YX_LOCTION_Link_TypeExsit(pMe,type))
            {
                return TRUE;
            }
            YX_LOCTION_Link_AddData(pMe,type,pParam,ParamLen);
                #ifdef ZDT_GPS_SUPPORT
                    MMIZDTGPS_API_LocStart(YX_LOCTION_ON_TIME_S,YX_LOCTION_WL_CallBack);
                #else
                    #ifdef ZDT_WIFI_SUPPORT
                        MMIZDTWIFI_API_LocStart(10,YX_LOCTION_WL_CallBack);
                    #endif
                #endif
        }
        else if(YX_LOCTION_TYPE_NET_GPS == type)
        {
            if(YX_LOCTION_Link_TypeExsit(pMe,type))
            {
                is_exsit =  TRUE;
            }
            YX_LOCTION_Link_AddData(pMe,type,pParam,ParamLen);
            if(is_exsit == FALSE)
            {
                #ifdef ZDT_GPS_SUPPORT
                    MMIZDTGPS_API_LocStart(time_s,YX_LOCTION_Net_GPS_CallBack);
                #else
                    #ifdef ZDT_WIFI_SUPPORT
                        MMIZDTWIFI_API_LocStart(10,YX_LOCTION_Net_GPS_CallBack);
                    #endif
                #endif
            }
        }
    }
    return TRUE;
}

int YX_LOCTION_GPSInfo_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    uint8 ret = 0;
    int val = 1;
    double d_val = 0;
    double latitude = 0;
    double longitude = 0;
    cJSON *json = PNULL;
    cJSON *item_json = PNULL;
    char lat_buf[30] = {0};
    char long_buf[30] = {0};
    
    ZDT_LOG("YX_LOCTION_GPSInfo_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            json=cJSON_Parse(pRcv);
            if (json) 
            {
                item_json = cJSON_GetObjectItem(json,"code");
                if(item_json)
                {
                    val = cJSON_GetObjectInt(item_json);
                    ZDT_LOG("code =%d",val);
                }
                else
                {
                    ZDT_LOG("resultType NONE");
                    cJSON_Delete(json);
                    return 0;
                }
                if(val == 0)
                {
                    item_json = cJSON_GetObjectItem(json,"latitude"); //tencentLat
                    if(item_json)
                    {
                        d_val = cJSON_GetObjectDouble(item_json);
                        latitude = d_val;
                        sprintf(lat_buf,"%f",d_val);
                        ZDT_LOG("latitude=%s",lat_buf);
                    }
                    else
                    {
                        ZDT_LOG("latitude NONE");
                    }
                    item_json = cJSON_GetObjectItem(json,"longitude"); //tencentLon
                    if(item_json)
                    {
                        d_val = cJSON_GetObjectDouble(item_json);
                        longitude = d_val;
                        sprintf(long_buf,"%f",d_val);
                        ZDT_LOG("longitude=%s",long_buf);
                    }
                    else
                    {
                        ZDT_LOG("latitude NONE");
                    }
                    YX_DB_REGIONAL_Fence_CheckExt(latitude,longitude);
                }
                cJSON_Delete(json);
            }        }
    }
    return 0;
}

BOOLEAN  YX_LOCTION_GPSInfo(uint8 loc_ok)
{
    char send_buf[1001] = {0};
    uint16 len = 0;
    
    ZDT_LOG("YX_LOCTION_GPSInfo loc_ok=0x%x",loc_ok);
    
    len += YX_Func_GetSSInfoExt(send_buf);
    if(loc_ok & 0x02)
    {
        send_buf[len++] = '&';
        len += YX_Func_GetWIFIInfoExt(send_buf+len,loc_ok);
    }
    MMIZDT_HTTP_AppSend(TRUE,(char *)"http://api.test.ecellsz.com/API/location?imei=111&",(uint8 *)send_buf,len,1211,0,0,0,0,0,YX_LOCTION_GPSInfo_CB);
    return TRUE;
}

int YX_LOCTION_EndHandle(YX_APP_T *pMe,DPARAM param)
{
    uint32 dwParam = 0;
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    uint16 type = 0;
    uint8 loc_ok = 0;
    if(p_getdata != NULL)
    {
        dwParam = p_getdata->len;
    }
    loc_ok = (dwParam & 0x000000FF);
    type = (dwParam >> 16);
    ZDT_LOG("YX_LOCTION_EndHandle type=%d,loc_ok=%d",type,loc_ok);
    switch(type)
    {
        case YX_LOCTION_TYPE_CR:
            {
                YX_LOCTION_LINK_DATA_T cur_data = {0};
                while(YX_LOCTION_Link_GetByType(pMe,&cur_data,YX_LOCTION_TYPE_CR))
                {
                    YX_Net_Respond_LOCATION_INFO_GET(pMe,loc_ok,cur_data.pParam);
                    if(cur_data.pParam != NULL)
                    {
                        SCI_FREE(cur_data.pParam);
                    }
                }
                if(loc_ok == 0)
                {
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
                    YX_LOCTION_Start(pMe,YX_LOCTION_TYPE_SMS,NULL,0);
#endif
                }
            }
            break;
            
        case YX_LOCTION_TYPE_NET:
            {
                YX_LOCTION_LINK_DATA_T cur_data = {0};
                if(loc_ok == 0)
                {
                    while(YX_LOCTION_Link_GetByType(pMe,&cur_data,YX_LOCTION_TYPE_NET))
                    {
                        if(cur_data.pParam != NULL)
                        {
                            SCI_FREE(cur_data.pParam);
                        }
                    }
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
                    YX_LOCTION_Start(pMe,YX_LOCTION_TYPE_NET_GPS,NULL,0);
#endif
                }
                else
                {
                    while(YX_LOCTION_Link_GetByType(pMe,&cur_data,YX_LOCTION_TYPE_NET))
                    {
                        YX_Net_Send_REPORT_LOCATION_INFO(pMe,loc_ok);
                        if(cur_data.pParam != NULL)
                        {
                            SCI_FREE(cur_data.pParam);
                        }
                    }
                }
            }
            break;
            
        case YX_LOCTION_TYPE_SMS:
            {
                YX_LOCTION_LINK_DATA_T cur_data = {0};
                while(YX_LOCTION_Link_GetByType(pMe,&cur_data,YX_LOCTION_TYPE_SMS))
                {
                    YX_Net_Respond_LOCATION_INFO_GET(pMe,loc_ok,cur_data.pParam);
                    if(cur_data.pParam != NULL)
                    {
                        SCI_FREE(cur_data.pParam);
                    }
                }
            }
            break;
            
        case YX_LOCTION_TYPE_ALERT:
            {
                YX_LOCTION_LINK_DATA_T cur_data = {0};
                while(YX_LOCTION_Link_GetByType(pMe,&cur_data,YX_LOCTION_TYPE_ALERT))
                {
                    YX_Net_Send_REPORT_LOCATION_INFO(pMe,loc_ok);
                    if(cur_data.pParam != NULL)
                    {
                        SCI_FREE(cur_data.pParam);
                    }
                }
            }
            break;
            
        case YX_LOCTION_TYPE_WL:
            {
                YX_LOCTION_LINK_DATA_T cur_data = {0};
                BOOLEAN need_check = FALSE;
                while(YX_LOCTION_Link_GetByType(pMe,&cur_data,YX_LOCTION_TYPE_WL))
                {
                    need_check = TRUE;
                    if(cur_data.pParam != NULL)
                    {
                        SCI_FREE(cur_data.pParam);
                    }
                }
                
                if(need_check)
                {
                    if(loc_ok & 0x01)
                    {
                        GPS_DATA_T last_gps= {0};
                        GPS_API_GetLastPos(&last_gps);
                        if(last_gps.is_valid)
                        {
                            YX_DB_REGIONAL_Fence_Check(&last_gps);
                        }
                        else
                        {
                            YX_LOCTION_GPSInfo(loc_ok);
                        }
                    }
                    else
                    {
                        YX_LOCTION_GPSInfo(loc_ok);
                    }
                }
            }
            break;
            
        case YX_LOCTION_TYPE_NET_GPS:
            {
                YX_LOCTION_LINK_DATA_T cur_data = {0};
                while(YX_LOCTION_Link_GetByType(pMe,&cur_data,YX_LOCTION_TYPE_NET_GPS))
                {
                    YX_Net_Send_REPORT_LOCATION_INFO(pMe,loc_ok);
                    if(cur_data.pParam != NULL)
                    {
                        SCI_FREE(cur_data.pParam);
                    }
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

int YX_API_LOCTION_End(uint16 loc_ok,uint16 type)
{
    uint32 len = type;
    len = len << 16;
    len += loc_ok;
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCTION_END,NULL,len );
    return 0;
}

BOOLEAN YX_API_LOCTION_WL_Start(void)
{
#ifdef ZDT_GPS_SUPPORT
    YX_LOCTION_Start(&g_yx_app,YX_LOCTION_TYPE_WL,NULL,0);
#endif
    return TRUE;
}

#endif


int YX_API_Lose_Post(void)
{
    if(g_is_yx_power_off_ing == TRUE)
    {
        return 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOSE,NULL,0);
    return 0;
}

int YX_API_BatStatus_Post(uint8 level,uint8 percent)
{
    uint16 bat_status = 0;
    if(g_is_yx_power_off_ing == TRUE)
    {
        return 0;
    }
    bat_status = level;
    bat_status = bat_status << 8;
    bat_status += percent;
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_BATSTAUTS,NULL,bat_status);
    return 0;
}

int YX_API_Move_Post(void)
{
    if(g_is_yx_power_off_ing == TRUE)
    {
        return 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_MOVE,NULL,0);
    return 0;
}

int YX_API_Still_Post(void)
{
    if(g_is_yx_power_off_ing == TRUE)
    {
        return 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_STILL,NULL,0);
    return 0;
}

int YX_API_JP_Change_Post(void)
{
    //有走动
    if(g_is_yx_power_off_ing == TRUE)
    {
        return 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_JP_CHANGE,NULL,0);
    return 0;
}

int YX_API_LBS_Change_Post(void)
{
    //基站发生变化
    if(g_is_yx_power_off_ing == TRUE)
    {
        return 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LBS_CHANGE,NULL,0);
    return 0;
}

int YX_API_FallDown_Post(void)
{
    if(g_is_yx_power_off_ing == TRUE)
    {
        return 0;
    }
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_FALLDOWN,NULL,0);
    return 0;
}

BOOLEAN YX_API_SOS_Post(void)
{
    if(YX_Net_Is_Land())
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_ALERT,NULL,YX_NET_ALERT_TYPE_SOS);
        return TRUE;
    }
    else
    {
        if(ZDT_SIM_Exsit())
        {
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_PlayCustomRing(MMISET_CUST_NO_NET,1);
#endif
        }
        else
        {
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_PlayCustomRing(MMISET_CUST_NO_SIM,1);
#endif
        }
    }
    
    return FALSE;
}

BOOLEAN YX_API_SOS_PostExt(void)
{
    if(YX_Net_Is_Land())
    {
        YX_Net_Send_REPORT_LOCATION_INFO(&g_yx_app,0);
        YX_Net_Send_REPORT_SOS();
        return TRUE;
    }
    return FALSE;
}

extern BOOLEAN is_sos_outgoing_call;

BOOLEAN YX_API_SOS_PostAfter(void)
{
    is_sos_outgoing_call= FALSE;
#ifdef ZDT_LED_SUPPORT
    ZDT_Led_Start_Call_Incoming_Over();
#endif
    if(YX_Net_Is_Land())
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_LOCREPORT,NULL,0);
        return TRUE;
    }
    return FALSE;
}

BOOLEAN YX_API_Mute_IsValid(void)
{
    if(yx_DB_Set_Rec.env_mute == 1)
    {
        return TRUE;
    }
    
    if(YX_DB_TIMER_IsValid_Class())
    {
        return TRUE;
    }
    
    return FALSE;
}

BOOLEAN YX_API_Ring_IsValid(void)
{
    if(yx_DB_Set_Rec.env_ring == 1)
    {
        return TRUE;
    }
    return FALSE;
}

BOOLEAN YX_API_MtCallLimit_IsValid(void)
{
    if(yx_DB_Set_Rec.mt_call_limit == 1)
    {
        return TRUE;
    }
    
    if(YX_DB_TIMER_IsValid_Class())
    {
        return TRUE;
    }
    
    return FALSE;
}
BOOLEAN YX_API_MoCallLimit_IsValid(void)
{
    if(yx_DB_Set_Rec.mo_call_limit == 1)
    {
        return TRUE;
    }
    if(YX_DB_TIMER_IsValid_Class())
    {
        return TRUE;
    }
    return FALSE;
}

BOOLEAN YX_API_SOSMoCall_IsValid(void)
{
    if(YX_DB_TIMER_IsValid_Class())
    {
        if(yx_DB_Set_Rec.sos_mo != 1)
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOLEAN YX_API_SOSMoCall_KeyLock_IsValid(void)
{
    if(YX_DB_TIMER_IsValid_Class())
    {
        if(yx_DB_Set_Rec.sos_mo == 1)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOLEAN YX_API_SOSMtCall_IsValid(void)
{
    if(YX_DB_TIMER_IsValid_Class())
    {
        if(yx_DB_Set_Rec.sos_mt != 1)
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOLEAN YX_API_CallNumber_IsValid(char * num)
{
    BOOLEAN res = FALSE;
    uint16 num_len = 0;
    uint16 db_num_len = 0;
    uint16 i = 0;
    if(num == PNULL)
    {
        return FALSE;
    }
    
    if(YX_API_MtCallLimit_IsValid())
    {
        ZDT_LOG("YX_API_CallNumber_IsValid FALSE MtCallLimit");
        res =  FALSE;
    }
    else
    {
        num_len = SCI_STRLEN(num);
        if(YX_DB_WHITE_IsValidNumberExt(num) > 0)
        {
            ZDT_LOG("YX_API_CallNumber_IsValid TRUE Whitelist");
            res =  TRUE;
        }
    }
    
    //if(yx_DB_Set_Rec.mt_call_limit != 1)
    {
        if(YX_DB_SOS_IsValidNumber(num) > 0)
        {
            if(YX_API_SOSMtCall_IsValid())
            {
                ZDT_LOG("YX_API_CallNumber_IsValid TRUE SOS");
                res =  TRUE;
            }
        }
    }
    return res;

}

BOOLEAN YX_API_Call_NeedDisconnect(char * num, BOOLEAN is_mo_call)
{
    BOOLEAN res = TRUE;
    
    if(num == PNULL)
    {
        return TRUE;
    }
    SCI_TRACE_LOW("YX_API_Call_NeedDisconnect num = %s, is_mo_call=%d",num,is_mo_call);
    if(YX_DB_SOS_IsValidNumber(num) > 0)
    {
        SCI_TRACE_LOW("YX_API_Call_NeedDisconnect SOS sos_mt=%d,sos_mo=%d,is_mo_call=%d",yx_DB_Set_Rec.sos_mt,yx_DB_Set_Rec.sos_mo,is_mo_call);
        if(yx_DB_Set_Rec.sos_mt == 1 && is_mo_call == FALSE)
        {
            res = FALSE;
        }
        if(yx_DB_Set_Rec.sos_mo == 1 && is_mo_call == TRUE)
        {
            res = FALSE;
        }
    }
    SCI_TRACE_LOW("YX_API_Call_NeedDisconnect %d",res);
    return res;

}

uint16  g_yx_netsms_content[YX_DB_NETSMS_MAX_NAME_SIZE+1] = {0};
uint16  g_yx_net_netsms_id = 0;
uint32  g_yx_netsms_content_len = 0;

BOOLEAN YX_API_NetSms_Stop(void)
{
    ZDT_LOG("YX_API_NetSms_Stop g_yx_net_netsms_id=%d",g_yx_net_netsms_id);
#ifdef HERO_ENGINE_TTS_SUPPORT
    TTSAPI_StopPlayText();
#endif
    if(g_yx_net_netsms_id > 0)
    {
        //YX_Net_Send_Voice_5059(yx_DB_NetSms_Reclist[g_yx_net_netsms_id-1].msg_id);
        #if 0
        if(YX_DB_NETSMS_HaveNum() > 1)
        {
            YX_DB_NETSMS_ListDel(g_yx_net_netsms_id-1);
        }
        #endif
    }
    //MMIZDT_MP3_StreamStop();
    g_yx_net_netsms_id = 0;
   return TRUE;
}

BOOLEAN YX_API_NetSms_Play(uint16 * ucs_str,uint32 ucs_len,uint16 repeat_times)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = ucs_str;
        play_str.wstr_len = ucs_len;
        //TTSAPI_PlayTextForZDT(&play_str,1,repeat_times);
        TTSAPI_PlayTextForZDTSMS(ucs_str,ucs_len,repeat_times);
        return TRUE;
    #else
        return FALSE;
    #endif
}


BOOLEAN YX_API_PlayDistance(uint32 distance)
{
    MMI_STRING_T play_str = {0};
    char utf8_str[200] = {0};
    uint16 ucs_str[200] = {0};
    uint32 ucs_len = 0;

    sprintf(utf8_str,"%d",distance);
    ucs_len = GUI_UTF8ToWstr(ucs_str, 200, utf8_str, strlen(utf8_str));
    #ifdef HERO_ENGINE_TTS_SUPPORT
        play_str.wstr_ptr = ucs_str;
        play_str.wstr_len = ucs_len;
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN YX_API_PlayWL_IN(void)
{
    MMI_STRING_T play_str = {0};
    uint16 ucs_str[10] = {0x8FDB, 0x56F4, 0x680F, 0x0 };
    uint32 ucs_len = 3;

    #ifdef HERO_ENGINE_TTS_SUPPORT
        play_str.wstr_ptr = ucs_str;
        play_str.wstr_len = ucs_len;
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN YX_API_PlayWL_Out(void)
{
    MMI_STRING_T play_str = {0};
    uint16 ucs_str[10] = {0x51FA, 0x56F4, 0x680F, 0x0 };
    uint32 ucs_len = 3;

    #ifdef HERO_ENGINE_TTS_SUPPORT
        play_str.wstr_ptr = ucs_str;
        play_str.wstr_len = ucs_len;
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN YX_API_NetSms_PlayLast(void)
{
    uint16 last_id = 0;
    BOOLEAN res = FALSE;
    if(YX_API_Mute_IsValid())
    {
        ZDT_LOG("YX_API_NetSms_PlayLast ERR Is In Mute");
        return FALSE;
    }
    
    YX_API_NetSms_Stop();
    if(YX_DB_NETSMS_GetNewUnReadNum(&last_id))
    {
        SCI_MEMSET(g_yx_netsms_content,0,sizeof(g_yx_netsms_content));
        if(yx_DB_NetSms_Reclist[last_id].netsms_encoding == 2 || yx_DB_NetSms_Reclist[last_id].netsms_encoding == 0)
        {
            g_yx_netsms_content_len = GUI_UTF8ToWstr(g_yx_netsms_content,YX_DB_NETSMS_MAX_NAME_SIZE,yx_DB_NetSms_Reclist[last_id].netsms_content,yx_DB_NetSms_Reclist[last_id].netsms_content_len);
        }
        else
        {
            SCI_MEMCPY(g_yx_netsms_content,yx_DB_NetSms_Reclist[last_id].netsms_content,yx_DB_NetSms_Reclist[last_id].netsms_content_len);
	     g_yx_netsms_content_len = MMIAPICOM_Wstrlen(g_yx_netsms_content);
        }
        g_yx_net_netsms_id = last_id+1;
        if(yx_DB_NetSms_Reclist[last_id].is_read == 0)
        {
            YX_DB_NETSMS_ChangStatusToRead(last_id);
            YX_Net_Send_REPORT_SMS_READ(&g_yx_app,last_id);
        }
#ifdef ZDT_LED_SUPPORT
        ZDT_Led_Start_Call_Incoming_Over();
#endif
        ZDT_LOG("YX_API_NetSms_PlayLast last_id = %d,content_len=%d,playtimes=%d",last_id,g_yx_netsms_content_len,yx_DB_NetSms_Reclist[last_id].netsms_playtimes);
        YX_API_NetSms_Play(g_yx_netsms_content,g_yx_netsms_content_len,yx_DB_NetSms_Reclist[last_id].netsms_playtimes);
        res = TRUE;
    }
    else
    {
#ifdef ZDT_PLAY_RING_CUSTOM
        MMIAPISET_PlayCustomRing(MMISET_CUST_VOICE_NULL,1);
#endif
    }
    return res;
}

PUBLIC BOOLEAN YX_API_NetSms_PlayIndex(uint8 index)
{
    BOOLEAN res = FALSE;
    if(YX_API_Mute_IsValid())
    {
        ZDT_LOG("YX_API_NetSms_PlayIndex ERR Is In Mute");
        return FALSE;
    }
    YX_API_NetSms_Stop();
    SCI_MEMSET(g_yx_netsms_content,0,sizeof(g_yx_netsms_content));
    if(yx_DB_NetSms_Reclist[index].netsms_encoding == 2 || yx_DB_NetSms_Reclist[index].netsms_encoding == 0)
    {
        g_yx_netsms_content_len = GUI_UTF8ToWstr(g_yx_netsms_content,YX_DB_NETSMS_MAX_NAME_SIZE,yx_DB_NetSms_Reclist[index].netsms_content,yx_DB_NetSms_Reclist[index].netsms_content_len);
    }
    else
    {
        SCI_MEMCPY(g_yx_netsms_content,yx_DB_NetSms_Reclist[index].netsms_content,yx_DB_NetSms_Reclist[index].netsms_content_len);
	 g_yx_netsms_content_len = MMIAPICOM_Wstrlen(g_yx_netsms_content);
    }
    #ifdef ZDT_LED_SUPPORT
    ZDT_Led_Start_Call_Incoming_Over();
    #endif
    ZDT_LOG("YX_API_NetSms_PlayIndex last_id = %d,content_len=%d,playtimes=%d",index,g_yx_netsms_content_len,yx_DB_NetSms_Reclist[index].netsms_playtimes);
    ZDT_LOG("yx_DB_NetSms_Reclist[%d].netsms_mode=%d",index,yx_DB_NetSms_Reclist[index].netsms_mode);
    if(yx_DB_NetSms_Reclist[index].netsms_mode !=1&&yx_DB_NetSms_Reclist[index].netsms_playtimes!=0)
    {
    	YX_API_NetSms_Play(g_yx_netsms_content,g_yx_netsms_content_len,yx_DB_NetSms_Reclist[index].netsms_playtimes);
    }
    
    res = TRUE;
    return res;
}

#endif

