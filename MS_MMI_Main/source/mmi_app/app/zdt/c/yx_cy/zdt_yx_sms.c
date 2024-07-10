#include "zdt_app.h"
#include "version.h"

#ifdef ZDT_SMS_SUPPORT
typedef enum
{
	YX_SMS_SERVER_NULL,
	YX_SMS_SERVER_HEAD,
	YX_SMS_SERVER_TAIL,
}YXSMSServerPos;

static BOOLEAN YX_SMS_CheckHead(uint8 * pData,int DataLen)
{

    /*
    【易赛通信】3010 6613 8D5B 901A 4FE1 3011
    */
    
    if(DataLen < 12)
    {
        return FALSE;
    }

    if(((pData[0] == 0x30 && pData[1] == 0x10)
        || (pData[0] == 0x00 && pData[1] == 0x5B)
        || (pData[0] == 0xFF && pData[1] == 0x3B)
        )
        && pData[2] == 0x66
        && pData[3] == 0x13
        && pData[4] == 0x8D
        && pData[5] == 0x5B
        && pData[6] == 0x90
        && pData[7] == 0x1A
        && pData[8] == 0x4F
        && pData[9] == 0xE1
        && ((pData[10] == 0x30 && pData[11] == 0x11)
        || (pData[10] == 0x00 && pData[11] == 0x5D)
        || (pData[10] == 0xFF && pData[11] == 0x3D)
        )
        )
    {
        return TRUE;
    }
    return FALSE;
}

static YXSMSServerPos YX_SMS_IsFromServer(uint8 * pData,int DataLen,BOOLEAN is_ucs2)
{    
    YXSMSServerPos ret = YX_SMS_SERVER_NULL;
    if(pData == NULL || DataLen < 13)
    {
        return YX_SMS_SERVER_NULL;
    }
    
    if(is_ucs2 == FALSE)
    {
        return YX_SMS_SERVER_NULL;
    }
    
    if(YX_SMS_CheckHead(pData,DataLen))
    {
        ret = YX_SMS_SERVER_HEAD;
    }
    else
    {
        int pos_len = DataLen-12;
        if(YX_SMS_CheckHead(pData+pos_len,12))
        {
            ret = YX_SMS_SERVER_TAIL;
        }
    }

    return ret;

}

#ifdef YX_DB_NEED_SOS
//0: 不是亲情号码>0:亲情号码
extern BOOLEAN YX_SMS_IsFromSOSExt(uint8 * pNum)
{    
    int sos_idx = 0;
    sos_idx = YX_DB_SOS_IsValidNumber((char *)pNum);
    if(sos_idx <= 0)
    {
        sos_idx = YX_DB_SPD_IsValidNumber((char *)pNum);
    }
    if(sos_idx > 0)
    {
        return TRUE;
    }
    return FALSE;
}
extern uint16 YX_SMS_IsFromSOS(uint8 * pNum)
{    
    int sos_idx = 0;
    if(yx_DB_Set_Rec.sms_intercept == 1)
    {
        //不拦截
        return 1;
    }
    else if(yx_DB_Set_Rec.sms_intercept == 2)
    {
        if(strstr((char *)pNum,"10010"))
    	 {
    	     return 1;
    	 }
        if(strstr((char *)pNum,"10000"))
    	 {
    	     return 1;
    	 }
        if(strstr((char *)pNum,"10086"))
    	 {
    	     return 1;
    	 }
        if(strstr((char *)pNum,"10099"))
    	 {
    	     return 1;
    	 }
	#ifdef YX_DB_NEED_WHITE
        if(YX_DB_WHITE_IsValidNumberSMS((char *)pNum) > 0)//白名单
        {
            return 1;
        }
	#endif
        //保留亲情号码及白名单号码的短信
        sos_idx = YX_DB_SOS_IsValidNumber((char *)pNum);
        if(sos_idx <= 0)
        {
            sos_idx = YX_DB_SPD_IsValidNumber((char *)pNum);
        }
        if(sos_idx > 0)
        {
            return 1;
        }
    }
    else if(yx_DB_Set_Rec.sms_intercept == 3)
    {
        //全部拦截
        return 0;
    }
    return 0;
}
#endif

static BOOLEAN YX_SMS_ServerValid(uint8 * pNum, uint8 * pData,int DataLen)
{
    uint8 * msg_str = NULL;
    uint16 msg_len = 0;
    int UcsDataLen = DataLen/2;
    BOOLEAN  ret = FALSE;
    uint16 i = 0;

    msg_str = SCI_ALLOC_APPZ(UcsDataLen+1);
    if(msg_str == NULL)
    {
        return FALSE;
    }
    
    for(i = 0; i < UcsDataLen; i++)
    {
        msg_str[i] = pData[i*2+1];
    }
    
    msg_str[UcsDataLen] = 0;
    
    YX_Func_strupr((char *)msg_str);
    msg_len = UcsDataLen;
    ZDT_LOG("YX SMS Server Len=%d,%s",msg_len,msg_str);
    if(msg_len >= 6 &&  strncmp( (char *)msg_str, "SDMSON", 6 ) == 0)
    {
        YX_API_SeNetOpen();
        ret = TRUE;
    }
    else if(msg_len >= 7 &&  strncmp( (char *)msg_str, "SDMSOFF", 7 ) == 0)
    {
        YX_API_SeNetClose();
        ret = TRUE;
    }
    SCI_FREE(msg_str);
    return ret;
}

int YX_SMS_Receive_IP(uint8 * pContent,uint16 ContentLen)
{
    int ret = 0;
    char buf[MAX_YX_IP_DOMAIN_SIZE+1] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        return 0;
    }
    
    ret = YX_Func_GetNextParaM(&str, &len,buf,MAX_YX_IP_DOMAIN_SIZE);
    if(ret > 0)
    {
        memset(yx_DB_Set_Rec.ip_domain,0,MAX_YX_IP_DOMAIN_SIZE+1);
        memcpy(yx_DB_Set_Rec.ip_domain,buf,ret);
    }
    else
    {
        return 0;
    }

    memset(buf,0,MAX_YX_IP_DOMAIN_SIZE+1);
    ret = YX_Func_GetNextPara(&str, &len,buf,MAX_YX_IP_DOMAIN_SIZE);
    if(ret > 0)
    {
        yx_DB_Set_Rec.ip_port = atoi(buf);
    }
    else
    {
        return 0;

    }
    
    YX_DB_SET_ListModify();
    
    ZDT_LOG("YX_SMS_Receive_IP ret =%d, ip = %s : %d",ret,yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
    
    MMIZDT_Net_Reset(TRUE);
        
    return 1;
}

int YX_SMS_Receive_Remote(uint8 * pContent,uint16 ContentLen)
{
    int ret = 0;
    char buf[MAX_YX_IP_DOMAIN_SIZE+1] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        return 0;
    }
    
    ret = YX_Func_GetNextPara(&str, &len,buf,MAX_YX_IP_DOMAIN_SIZE);
    if(ret > 0)
    {
        memset(yx_DB_Set_Rec.ip_domain,0,MAX_YX_IP_DOMAIN_SIZE+1);
        memcpy(yx_DB_Set_Rec.ip_domain,buf,ret);
    }
    else
    {
        return 0;
    }

    memset(buf,0,MAX_YX_IP_DOMAIN_SIZE+1);
    ret = YX_Func_GetNextPara(&str, &len,buf,MAX_YX_IP_DOMAIN_SIZE);
    if(ret > 0)
    {
        yx_DB_Set_Rec.ip_port = atoi(buf);
    }
    else
    {
        return 0;

    }
    
    YX_DB_SET_ListModify();
    
    ZDT_LOG("YX_SMS_Receive_IP ret =%d, ip = %s : %d",ret,yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port);
    
    MMIZDT_Net_Reset(TRUE);
        
    return 1;
}
int YX_SMS_Get_IP(uint8 * pNum)
{
    uint16 num_len = 0;
    // 服务器地址和端口:
    uint16 sms_t_1[10] = {0x670D,0x52A1,0x5668,0x5730,0x5740,0x548C,0x7AEF,0x53E3,0x003A,0x0};
    //连接状态:
    uint16 sms_t_2[10] = {0x8FDE,0x63A5,0x72B6,0x6001,0x003A,0x0};
    //亲情号码:
    uint16 sms_t_3[10] = {0x4EB2,0x60C5,0x53F7,0x7801,0x003A,0x0};
    //版本号:
    uint16 sms_t_4[10] = {0x7248,0x672C,0x53F7,0x003A,0x0};
    uint16 sms_buf[520] = {0};
    uint16 send_len = 0;
    char tmp_buf[200] = {0};
    uint16 tmp_wbuf[200] = {0};

    MMIAPICOM_Wstrcat(sms_buf,sms_t_1);
    sprintf((char *)tmp_buf,"%s,%d\r\nIMEI:%s\r\nIMSI:%s\r\n", yx_DB_Set_Rec.ip_domain,yx_DB_Set_Rec.ip_port,g_zdt_phone_imei,g_zdt_sim_imsi);
    MMIAPICOM_StrToWstr(tmp_buf,tmp_wbuf);
    MMIAPICOM_Wstrcat(sms_buf,tmp_wbuf);
    
    MMIAPICOM_Wstrcat(sms_buf,sms_t_2);
    SCI_MEMSET(tmp_buf,0,sizeof(tmp_buf));
    SCI_MEMSET(tmp_wbuf,0,sizeof(tmp_wbuf));
    if(YX_Net_Is_Land())
    {
        sprintf((char *)tmp_buf,"on line\r\n");
    }
    else
    {
        sprintf((char *)tmp_buf,"off line\r\n");
    }
    MMIAPICOM_StrToWstr(tmp_buf,tmp_wbuf);
    MMIAPICOM_Wstrcat(sms_buf,tmp_wbuf);

    MMIAPICOM_Wstrcat(sms_buf,sms_t_3);
    SCI_MEMSET(tmp_buf,0,sizeof(tmp_buf));
    SCI_MEMSET(tmp_wbuf,0,sizeof(tmp_wbuf));
    sprintf((char *)tmp_buf,"%s,%s,%s\r\n",yx_DB_Spd_Reclist[0].spd_num,yx_DB_Spd_Reclist[1].spd_num,yx_DB_Spd_Reclist[2].spd_num);
    MMIAPICOM_StrToWstr(tmp_buf,tmp_wbuf);
    MMIAPICOM_Wstrcat(sms_buf,tmp_wbuf);

    SCI_MEMSET(tmp_buf,0,sizeof(tmp_buf));
    SCI_MEMSET(tmp_wbuf,0,sizeof(tmp_wbuf));
    sprintf((char *)tmp_buf,"SOS:%s,%s,%s,%s,%s\r\n",yx_DB_Sos_Reclist[0].sos_num,yx_DB_Sos_Reclist[1].sos_num,yx_DB_Sos_Reclist[2].sos_num,yx_DB_Sos_Reclist[3].sos_num,yx_DB_Sos_Reclist[4].sos_num);
    MMIAPICOM_StrToWstr(tmp_buf,tmp_wbuf);
    MMIAPICOM_Wstrcat(sms_buf,tmp_wbuf);

    MMIAPICOM_Wstrcat(sms_buf,sms_t_4);
    SCI_MEMSET(tmp_buf,0,sizeof(tmp_buf));
    SCI_MEMSET(tmp_wbuf,0,sizeof(tmp_wbuf));
    sprintf((char *)tmp_buf,"%s\r\n",ZDT_SFR_SW_VER);
    MMIAPICOM_StrToWstr(tmp_buf,tmp_wbuf);
    MMIAPICOM_Wstrcat(sms_buf,tmp_wbuf);
    
    SCI_MEMSET(tmp_buf,0,sizeof(tmp_buf));
    SCI_MEMSET(tmp_wbuf,0,sizeof(tmp_wbuf));
    sprintf((char *)tmp_buf,"RSSI:%d\r\n",MMIZDT_Net_GetRssiInd());
    MMIAPICOM_StrToWstr(tmp_buf,tmp_wbuf);
    MMIAPICOM_Wstrcat(sms_buf,tmp_wbuf);

    send_len = MMIAPICOM_Wstrlen(sms_buf);
    
    MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)sms_buf,send_len*2,pNum,strlen(pNum),2,3);
  
    return 1;
}

PUBLIC BOOLEAN isDigitStr(uint8 *str)
{
    BOOLEAN ret = FALSE;
    uint32 len = 0;
    int i = 0;
    if(str == NULL || strlen(str) == 0 )
    {   
        return ret;
    }
    len = strlen(str);
    ret = TRUE;
    for(i = 0; i < len; i++)
    {
        if(str[i] >= '0' && str[i] <= '9')
        {
            
        }
        else
        {
            ret = FALSE;
            break;
        }
    }
    ZDT_LOG("ZDT__SMS isDigitStr ret=%d", ret);

    return ret;
}

int YX_SMS_RestorFactory(void)
{
    MMISET_ResetFactorySetting();
    SCI_Sleep(10);
    MMIAPIPHONE_PowerReset();
    return 0;
}
#ifdef YX_DB_NEED_SOS
static BOOLEAN YX_SMS_SOS_ContentValid(uint8 * pNum,uint8 * pData,int DataLen,BOOLEAN is_ucs2)
{
    BOOLEAN is_spd_num = 0;
    uint16 need_net_send = 0;
    uint8 * msg_str = NULL;
    uint8 * utf8_str = NULL;
    uint16 msg_len = 0;
    int UcsDataLen = 0;
    BOOLEAN  ret = FALSE;
    uint16 i = 0;
    if(pData == NULL || DataLen == 0)
    {
        return FALSE;
    }
    need_net_send = YX_SMS_IsFromSOS(pNum);
    is_spd_num = YX_SMS_IsFromSOSExt(pNum);
    msg_str = SCI_ALLOC_APPZ(DataLen+1);
    if(msg_str == NULL)
    {
        return FALSE;
    }

    if(is_ucs2)
    {
        msg_str = SCI_ALLOC_APPZ(DataLen*3);
        if(msg_str == NULL)
        {
            return FALSE;
        }
        #if 1
        msg_len = GUI_WstrToUTF8(msg_str, DataLen*3, (uint16*)pData, DataLen/2);
        #else
        #ifndef WIN32
        msg_len = ucs2str_to_gb2312strExt(pData, DataLen,msg_str,0);
        #else
        msg_len = DataLen;
        #endif
        #endif
    }
    else
    {
        msg_str = SCI_ALLOC_APPZ(DataLen+1);
        if(msg_str == NULL)
        {
            return FALSE;
        }
        SCI_MEMCPY(msg_str,pData,DataLen);
        msg_len = DataLen;
    }
    //YX_Func_strupr((char *)msg_str);
    if(is_spd_num)
    {
        if(msg_len == 3 &&  strncmp( (char *)msg_str, "DW#", 3 ) == 0)
        {
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
            YX_LOCTION_Start(&g_yx_app,YX_LOCTION_TYPE_SMS,NULL,0);
#else
            YX_Net_Send_REPORT_LOCATION_INFO(&g_yx_app,0);
#endif
            ZDT_LOG("SMS DW OK");
            ret = TRUE;
        }
        else if(msg_len == 7 &&  strncmp( (char *)msg_str, "SDMSON#", 7 ) == 0)
        {
            YX_API_SeNetOpen();
            ZDT_LOG("MT SMS SDMSON");
            ret = TRUE;
        }
        else if(msg_len == 8 &&  strncmp( (char *)msg_str, "SDMSOFF#", 8) == 0)
        {
            YX_API_SeNetClose();
            ZDT_LOG("MT SMS SDMSOFF");
            ret = TRUE;
        }
        else if(msg_len > 6 &&  strncmp( (char *)msg_str, "SETIP:", 6) == 0)
        {
            ZDT_LOG("MT SMS SETIP:");
            YX_SMS_Receive_IP(msg_str+6,msg_len-6);
            ret = TRUE;
        }
        else if(msg_len == 5 &&  strncmp( (char *)msg_str, "read#", 5) == 0)
        {
            ZDT_LOG("MT SMS read#:");
            YX_SMS_Get_IP(pNum);
            ret = TRUE;
        }
        else if(msg_len == 6 &&  strncmp( (char *)msg_str, "reset#", 6) == 0)
        {
            ZDT_LOG("MT SMS RESET");
            MMIAPIPHONE_PowerReset();
            ret = TRUE;
        }
        else if(msg_len == 8 &&  strncmp( (char *)msg_str, "factory#", 8) == 0)
        {
            ZDT_LOG("MT SMS factory");
            MMISET_ResetFactorySetting();
            ret = TRUE;
        }
        else if(msg_len >= 20 &&  strncmp( (char *)msg_str, "imei,", 5) == 0)
        {
            uint8 imei[15+1] = {0};
            SCI_MEMCPY(imei, msg_str+5, 15);
            ZDT_LOG("MT SMS imei=%s", imei);
            if(isDigitStr(imei))
            {
                MMIENG_UpdateNewImei(imei);
                MMIAPIPHONE_PowerReset();
            }
            ret = TRUE;
        }
        else if(msg_len == 17 &&  strncmp( (char *)msg_str, "CTOP#WHITE#ENABLE", 17) == 0)
        {
            ZDT_LOG("MT SMS CTOP#WHITE#ENABLE");
            MMIZDT_NV_SetNeedWhiteCall(1);
            ret = TRUE;
        }
        else if(msg_len == 20 &&  strncmp( (char *)msg_str, "CTOP#WHITE#DISENABLE", 20) == 0)
        {
            ZDT_LOG("MT SMS CTOP#WHITE#DISENABLE");
            MMIZDT_NV_SetNeedWhiteCall(0);
            ret = TRUE;
        }
        #if 0
        else if(msg_len == 17 &&  strncmp( (char *)msg_str, "CTOP#ALIPAY#RESET", 17) == 0)
        {
            ZDT_LOG("MT SMS CTOP#ALIPAY#RESET");
            ret = TRUE;
        }
        #endif
        else if(msg_len > 7 &&  strncmp( (char *)msg_str, "REMOTE#", 7) == 0)
        {
            ZDT_LOG("MT SMS REMOTE#");
            YX_SMS_Receive_Remote(msg_str+7,msg_len-7);
            ret = TRUE;
        }
        else
        {
            if(need_net_send)
            {
                YX_Net_Send_SYSTEM_SMS(&g_yx_app,pNum,msg_str,msg_len);
            }
        }
    }
    else
    {
        if(msg_len > 7 &&  strncmp( (char *)msg_str, "REMOTE#", 7) == 0)
        {
            ZDT_LOG("MT SMS REMOTE#");
            YX_SMS_Receive_Remote(msg_str+7,msg_len-7);
            ret = TRUE;
        }
        else
        {
            if(need_net_send)
            {
                YX_Net_Send_SYSTEM_SMS(&g_yx_app,pNum,msg_str,msg_len);
            }
        }
    }
    SCI_FREE(msg_str);
    return ret;
}
#endif

extern BOOLEAN YX_SMS_IsValid(uint8 * pNum,uint8 * pData,int DataLen,BOOLEAN is_ucs2)
{
    BOOLEAN ret = FALSE;
    YXSMSServerPos server_status = YX_SMS_SERVER_NULL;
    if(pNum == NULL || pData == NULL )
    {
        return TRUE;
    }
    
    server_status = YX_SMS_IsFromServer(pData,DataLen,is_ucs2);
    if(server_status == YX_SMS_SERVER_HEAD)
    {
        ZDT_LOG("YX SMS Server Head");
        ret = YX_SMS_ServerValid(pNum,pData+12,DataLen-12);
    }
    else if(server_status == YX_SMS_SERVER_TAIL)
    {
        ZDT_LOG("YX SMS Server Tail");
        ret = YX_SMS_ServerValid(pNum,pData,DataLen-12);
    }
    else
    {
        ret = YX_SMS_SOS_ContentValid(pNum,pData,DataLen,is_ucs2);
    }
#if 0 //def ZDT_PLAT_YX_SUPPORT
   if(YX_DB_TIMER_IsValid_Slient())
   {
        ret = TRUE;
   }
#endif
    return ret;
}

uint16 YX_SMS_Server_GetContentLen(YX_APP_T *pMe)
{
    uint16 len = 0;
    
    return len;
}
int YX_SMS_ServerParse_Reconn(YX_APP_T *pMe)
{
    ZDT_LOG("YX_SMS_ServerParse_Reconn");
    YX_Net_Reset(pMe);
    return AEE_SUCCESS;
}

int YX_SMS_ServerParse(YX_APP_T *pMe,uint8 * pNum, uint8 * pData,uint16 DataLen)
{
    int ret = 0;
    ZDT_LOG("YX SMS Server Parse %s Len=%d,%s",pNum,DataLen,pData);
    if(DataLen > 7 &&  strncmp( (char *)pData, "&RECONN", 7 ) == 0)
    {
        ret = YX_SMS_ServerParse_Reconn(pMe);
    }
    return ret;
}

int YX_API_SMS_SendLowBattery(void)
{
    uint16 num_len = 0;
    //的设备向您发出低电报警！
    uint16 sms_tail[50] = {0x7684, 0x8BBE, 0x5907, 0x5411, 0x60A8, 0x53D1, 0x51FA, 0x4F4E, 0x7535, 0x62A5, 0x8B66, 0xFF01, 0x0};
    uint16 sms_buf[100] = {0};
    uint16 send_len = 0;
    char tmp_buf[50] = {0};
    
    if(FALSE == MMIZDT_AppIsInit())
    {
        return AEE_EFAILED;
    }
    
    if(0 == yx_DB_Set_Rec.alert_low_bat)
    {
        return AEE_EFAILED;
    }
    
    num_len = SCI_STRLEN((char *)yx_DB_Set_Rec.center_number);
    if(num_len == 0)
    {
        return AEE_EFAILED;
    }
    sprintf((char *)tmp_buf,"[%s]", g_zdt_phone_imei);
    MMIAPICOM_StrToWstr(tmp_buf,sms_buf);
    MMIAPICOM_Wstrcat(sms_buf,sms_tail);
    send_len = MMIAPICOM_Wstrlen(sms_buf);
    
    MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)sms_buf,send_len*2,yx_DB_Set_Rec.center_number,num_len,2,3);
  
    return AEE_SUCCESS;
}

int YX_API_SMS_SendLose(void)
{
    uint16 num_len = 0;
    ////的设备向您发出摘表报警！7684 8BBE 5907 5411 60A8 53D1 51FA 6458 8868 62A5 8B66 FF01 
    uint16 sms_tail[50] = {0x7684, 0x8BBE, 0x5907, 0x5411, 0x60A8, 0x53D1, 0x51FA, 0x6458, 0x8868, 0x62A5, 0x8B66, 0xFF01, 0x0};
    uint16 sms_buf[100] = {0};
    uint16 send_len = 0;
    char tmp_buf[50] = {0};
    
    if(FALSE == MMIZDT_AppIsInit())
    {
        return AEE_EFAILED;
    }
    
    if(0 == yx_DB_Set_Rec.alert_loser)
    {
        return AEE_EFAILED;
    }
    
    num_len = SCI_STRLEN((char *)yx_DB_Set_Rec.center_number);
    if(num_len == 0)
    {
        return AEE_EFAILED;
    }
    sprintf((char *)tmp_buf,"[%s]", g_zdt_phone_imei);
    MMIAPICOM_StrToWstr(tmp_buf,sms_buf);
    MMIAPICOM_Wstrcat(sms_buf,sms_tail);
    send_len = MMIAPICOM_Wstrlen(sms_buf);
    
    MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)sms_buf,send_len*2,yx_DB_Set_Rec.center_number,num_len,2,3);
  
    return AEE_SUCCESS;
}

int YX_API_SMS_SendSOS(void)
{
    uint16 num_len = 0;
    // //的设备向您发出紧急求助报警！7684 8BBE 5907 5411 60A8 53D1 51FA 7D27 6025 6C42 52A9 62A5 8B66 FF01 
    uint16 sms_tail[50] = {0x7684, 0x8BBE, 0x5907, 0x5411, 0x60A8, 0x53D1, 0x51FA, 0x7D27, 0x6025, 0x6C42, 0x52A9, 0x62A5, 0x8B66, 0xFF01, 0x0};
    uint16 sms_buf[100] = {0};
    uint16 send_len = 0;
    uint16 i = 0;
    char tmp_buf[50] = {0};
    
    if(FALSE == MMIZDT_AppIsInit())
    {
        return AEE_EFAILED;
    }
    
    if(0 == yx_DB_Set_Rec.alert_sos_sms)
    {
        return AEE_EFAILED;
    }

#ifdef ZDT_PLAT_YX_SUPPORT_SOS
    sprintf((char *)tmp_buf,"[%s]", g_zdt_phone_imei);
    MMIAPICOM_StrToWstr(tmp_buf,sms_buf);
    MMIAPICOM_Wstrcat(sms_buf,sms_tail);
    send_len = MMIAPICOM_Wstrlen(sms_buf);

    for(i = 0; i < YX_DB_SOS_MAX_SUM; i++)
    {
        if(yx_DB_Sos_Reclist[i].db_id != 0)
        {
            num_len = SCI_STRLEN(yx_DB_Sos_Reclist[i].sos_num);
            if(num_len > 0)
            {
                MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)sms_buf,send_len*2,yx_DB_Sos_Reclist[i].sos_num,num_len,2,3);
            }
        }
    }
#endif
    return AEE_SUCCESS;
}

int YX_API_SMS_SendReg(char * num_str)
{
    uint16 num_len = 0;
    char sms_buf[100] = {0};
    uint16 send_buf[100] = {0};
    uint16 send_len = 0;    
    
    if(FALSE == MMIZDT_AppIsInit())
    {
        return AEE_EFAILED;
    }
    
    num_len = SCI_STRLEN((char *)num_str);
    if(num_len == 0)
    {
        return AEE_EFAILED;
    }

    sprintf(sms_buf,"%s@%s",g_zdt_phone_imei,g_zdt_sim_iccid);
    send_len = strlen(sms_buf);
    ZDT_LOG("YX_API_SMS_SendReg %s",sms_buf);
    MMIAPICOM_StrToWstr(sms_buf,send_buf);
    send_len = MMIAPICOM_Wstrlen(send_buf);

    MMIZDT_SMS_AppSend(MMIZDT_Net_GetActiveSys(),(uint8 *)send_buf,send_len*2,num_str,num_len,2,3);
    return AEE_SUCCESS;
}
#endif
