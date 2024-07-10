#include "zdt_app.h"

#ifdef ZDT_SMS_SUPPORT
typedef enum
{
	SHB_SMS_SERVER_NULL,
	SHB_SMS_SERVER_HEAD,
	SHB_SMS_SERVER_TAIL,
}SHBSMSServerPos;

#ifdef SOS_KEYLONGWEB
extern uint8    sos_connect_idx;
#endif
uint8  g_shb_sms_bdrq_num[MAX_SHB_IP_DOMAIN_SIZE+1] = {0};
BOOLEAN    g_is_user_update_cfg = TRUE;
int32 SHB_SMS_Receive_BDRQ(uint8 * pContent,uint16 ContentLen)
{
    int ret = 0;
    char buf[MAX_SHB_IP_DOMAIN_SIZE+1] = {0};
    uint8 * str = pContent;
    uint32 len = ContentLen;
    
    if(ContentLen == 0)
    {
        return 0;
    }
    
    ret = SHB_Func_GetSubNextPara(&str, &len,buf,MAX_SHB_IP_DOMAIN_SIZE);
    if(ret > 0)
    {
        if(FALSE == ZdtSHB_IsOpenBindWin())
        {
            SCI_MEMSET(g_shb_sms_bdrq_num,0,MAX_SHB_IP_DOMAIN_SIZE+1);
            SCI_MEMCPY(g_shb_sms_bdrq_num,buf,ret);
            ZdtSHB_OpenBindWin();
        }
    }
    else
    {
        return 0;
    }

    ZDT_LOG("SHB_SMS_Receive_BDRQ ret =%d, num = %s",ret,g_shb_sms_bdrq_num);
           
    return 1;
}

static uint16 SHB_SMS_CheckHead(uint8 * pData,int DataLen)
{

    /*
    【中兴守护宝】3010 4E2D 5174 5B88 62A4 5B9D 3011
    */
    
    if(DataLen < 14)
    {
        return 0;
    }

    if(((pData[0] == 0x10 && pData[1] == 0x30)
        || (pData[0] == 0x5B && pData[1] == 0x00)
        || (pData[0] == 0x3B && pData[1] == 0xFF)
        )
        && pData[2] == 0x2D
        && pData[3] == 0x4E
        && pData[4] == 0x74
        && pData[5] == 0x51
        && pData[6] == 0x88
        && pData[7] == 0x5B
        && pData[8] == 0xA4
        && pData[9] == 0x62
        && pData[10] == 0x9D
        && pData[11] == 0x5B
        && ((pData[12] == 0x11 && pData[13] == 0x30)
        || (pData[12] == 0x5D && pData[13] == 0x00)
        || (pData[12] == 0x3D && pData[13] == 0xFF)
        )
        )
    {
        return 14;
    }
    return 0;
}

static uint16 SHB_SMS_CheckHeadShort(uint8 * pData,int DataLen)
{

    /*
    【京东】3010 4EAC 4E1C 3011 
    */
    
    if(DataLen < 8)
    {
        return FALSE;
    }

    if(((pData[0] == 0x10 && pData[1] == 0x30)
        || (pData[0] == 0x5B && pData[1] == 0x00)
        || (pData[0] == 0x3B && pData[1] == 0xFF)
        )
        && pData[2] == 0xAC
        && pData[3] == 0x4E
        && pData[4] == 0x1C
        && pData[5] == 0x4E
        && ((pData[6] == 0x11 && pData[7] == 0x30)
        || (pData[6] == 0x5D && pData[7] == 0x00)
        || (pData[6] == 0x3D && pData[7] == 0xFF)
        )
        )
    {
        return 8;
    }
    return 0;
}

static uint16 SHB_SMS_CheckTail(uint8 * pData,int DataLen)
{
    uint16 start_idx = 0;
    uint16 len = 0;

    if(DataLen < 14)
    {
        return 0;
    }
    start_idx = DataLen-14;
    len = SHB_SMS_CheckHead(pData+start_idx,14);
    
    return len;
}

static uint16 SHB_SMS_CheckTailShort(uint8 * pData,int DataLen)
{
    uint16 start_idx = 0;
    uint16 len = 0;

    if(DataLen < 8)
    {
        return 0;
    }
    
    start_idx = DataLen-8;
    len = SHB_SMS_CheckHeadShort(pData+start_idx,8);
    
    return len;
}
static SHBSMSServerPos SHB_SMS_IsFromServer(uint8 * pData,int DataLen,BOOLEAN is_ucs2,uint16 * pHeadlen)
{    
    uint16 len = 0;
    SHBSMSServerPos ret = SHB_SMS_SERVER_NULL;
    if(pData == NULL || DataLen < 9)
    {
        return SHB_SMS_SERVER_NULL;
    }
    
    if(is_ucs2 == FALSE)
    {
        return SHB_SMS_SERVER_NULL;
    }
    
    len = SHB_SMS_CheckHead(pData,DataLen);
    if(len == 0)
    {
        len = SHB_SMS_CheckHeadShort(pData,DataLen);
    }
    if(len)
    {
        ret = SHB_SMS_SERVER_HEAD;
    }
    else
    {
        len = SHB_SMS_CheckTail(pData,DataLen);
        if(len == 0)
        {
            len = SHB_SMS_CheckTailShort(pData,DataLen);
        }
        if(len)
        {
            ret = SHB_SMS_SERVER_TAIL;
        }
    }
    * pHeadlen = len;
    return ret;

}

#ifdef SHB_DB_NEED_SOS
//0: 不是亲情号码>0:亲情号码
static uint16 SHB_SMS_IsFromSOS(uint8 * pNum)
{    
    #if 0
    uint16 love_idx = 0;
    int sos_idx = 0;
    sos_idx = SHB_DB_SOS_IsValidNumber((char *)pNum);
    if(sos_idx >= 0)
    {
        love_idx = sos_idx+1;
    }
    return love_idx;
    #else
    return 1;
    #endif
}
#endif

int SHB_LOCTION_GPS_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1003_Send(loc_ok);
    }
    return 0;
}

int SHB_LOCTION_WIFI_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1003_Send(loc_ok);
    }
    else
    {
        SHB_HTTP_ZTE1003_Send(0);
    }
    return 0;
}

int SHB_SOS_GPS_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1006_Send(loc_ok);
    }
    return 0;
}

int SHB_SOS_WIFI_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1006_Send(loc_ok);
    }
    return 0;
}

int SHB_CR_GPS_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1008_Send(loc_ok);
    }
    return 0;
}

int SHB_CR_WIFI_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1008_Send(loc_ok);
    }
    else
    {
        SHB_HTTP_ZTE1008_Send(0);
    }
    return 0;
}

int SHB_WL_GPS_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1010_Send(loc_ok);
    }
    return 0;
}

int SHB_WL_WIFI_CallBack (uint8 loc_ok,void * pLoctionData)
{
    if(loc_ok)
    {
        SHB_HTTP_ZTE1010_Send(loc_ok);
    }
    else
    {
        SHB_HTTP_ZTE1010_Send(0);
    }
    return 0;
}

BOOLEAN MMIZDT_SHB_GPS_Loction_Start(void)
{
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_API_LocStart(10,SHB_LOCTION_WIFI_CallBack);
#endif
#ifdef ZDT_GPS_SUPPORT
    MMIZDTGPS_API_LocStart(60,SHB_LOCTION_GPS_CallBack);
#endif
    return TRUE;
}

BOOLEAN MMIZDT_SHB_GPS_Loction_SOS_Start(void)
{
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_API_LocStart(10,SHB_SOS_WIFI_CallBack);
#endif
#ifdef ZDT_GPS_SUPPORT
    MMIZDTGPS_API_LocStart(60,SHB_SOS_GPS_CallBack);
#endif
    return TRUE;
}

BOOLEAN MMIZDT_SHB_GPS_Loction_CR_Start(void)
{
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_API_LocStart(10,SHB_CR_WIFI_CallBack);
#endif
#ifdef ZDT_GPS_SUPPORT
    MMIZDTGPS_API_LocStart(60,SHB_CR_GPS_CallBack);
#endif
    return TRUE;
}

BOOLEAN MMIZDT_SHB_GPS_Loction_WL_Start(void)
{
#ifdef ZDT_WIFI_SUPPORT
    MMIZDTWIFI_API_LocStart(10,SHB_WL_WIFI_CallBack);
#endif
#ifdef ZDT_GPS_SUPPORT
    MMIZDTGPS_API_LocStart(60,SHB_WL_GPS_CallBack);
#endif
    return TRUE;
}

static BOOLEAN SHB_SMS_ServerValid(uint8 * pNum, uint8 * pData,int DataLen)
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
        msg_str[i] = pData[i*2];
    }
    
    msg_str[UcsDataLen] = 0;
    
    SHB_Func_strupr((char *)msg_str);
    msg_len = UcsDataLen;
    ZDT_LOG("SHB SMS Server Len=%d,%s",msg_len,msg_str);
    if(msg_len >= 3 &&  strncmp( (char *)msg_str, "PDW", 3 ) == 0)
    {
        ZDT_LOG("MT Server SMS PDW");
        if(SHB_Net_IsInit())
        {
                //SHB_HTTP_ZTE1003_Send(0);
                MMIZDT_SHB_GPS_Loction_Start();
        }
        ret = TRUE;
    }
    else if(msg_len >= 6 &&  strncmp( (char *)msg_str, "UPDATE", 6 ) == 0)
    {
        ZDT_LOG("MT Server SMS UPDATE");
        if(SHB_Net_IsInit())
        {
            if(SHB_HTTP_ZTE1000_Send())
            {
                g_is_user_update_cfg = TRUE;
            }
        }
        ret = TRUE;
    }
    else if(msg_len >= 6 &&  strncmp( (char *)msg_str, "STATUS", 6 ) == 0)
    {
        ZDT_LOG("MT Server SMS STATUS");
        if(SHB_Net_IsInit())
        {
            SHB_HTTP_ZTE1001_Send(0);
        }
        ret = TRUE;
    }
    else if(msg_len >=4 &&  strncmp( (char *)msg_str, "BDRQ", 4) == 0)
    {
        ZDT_LOG("MT Server BDRQ");
        SHB_SMS_Receive_BDRQ(msg_str+4,msg_len-4);
        ret = TRUE;
    }
    SCI_FREE(msg_str);
    return ret;
}

#ifdef SHB_DB_NEED_SOS
static BOOLEAN SHB_SMS_SOS_ContentValid(uint8 * pNum,uint8 * pData,int DataLen,BOOLEAN is_ucs2)
{
    uint8 * msg_str = NULL;
    uint16 msg_len = 0;
    int UcsDataLen = 0;
    BOOLEAN  ret = FALSE;
    uint16 i = 0;
    if(pData == NULL || DataLen == 0)
    {
        return FALSE;
    }
    
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
        #ifndef WIN32
        msg_len = ucs2str_to_gb2312strExt(pData, DataLen,msg_str,0);
        #else
        msg_len = DataLen;
        #endif
        if(FALSE == ZDT_SMS_UCS_is_All_Ascii(msg_str,msg_len))
        {
            SCI_FREE(msg_str);
            return FALSE;
        }
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
    SHB_Func_strupr((char *)msg_str);
    if(msg_len == 3 &&  strncmp( (char *)msg_str, "PDW", 3 ) == 0)
    {
        ZDT_LOG("MT SMS PDW");
        if(SHB_Net_IsInit())
        {
                //SHB_HTTP_ZTE1003_Send(0);
                MMIZDT_SHB_GPS_Loction_Start();
        }
        ret = TRUE;
    }
    else if(msg_len == 6 &&  strncmp( (char *)msg_str, "UPDATE", 6 ) == 0)
    {
        ZDT_LOG("MT SMS UPDATE");
        if(SHB_Net_IsInit())
        {
            SHB_HTTP_ZTE1000_Send();
        }
        ret = TRUE;
    }
    else if(msg_len == 7 &&  strncmp( (char *)msg_str, "SDMSOFF", 7) == 0)
    {
        ZDT_LOG("MT SMS SDMSOFF");
        ret = TRUE;
    }
    else if(msg_len >=4 &&  strncmp( (char *)msg_str, "BDRQ", 4) == 0)
    {
        ZDT_LOG("MT SMS BDRQ");
        SHB_SMS_Receive_BDRQ(msg_str+4,msg_len-4);
        ret = TRUE;
    }

    SCI_FREE(msg_str);
    return ret;
}
#endif

extern BOOLEAN SHB_SMS_IsValid(uint8 * pNum,uint8 * pData,int DataLen,BOOLEAN is_ucs2)
{
    uint16 head_len = 0;
    BOOLEAN ret = FALSE;
    SHBSMSServerPos server_status = SHB_SMS_SERVER_NULL;
    if(pNum == NULL || pData == NULL )
    {
        return TRUE;
    }

    if(shb_DB_Set_Rec.net_open == 0)
    {
        return FALSE;
    }
    
    server_status = SHB_SMS_IsFromServer(pData,DataLen,is_ucs2,&head_len);
    if(server_status == SHB_SMS_SERVER_HEAD)
    {
        ZDT_LOG("SHB SMS Server Head");
        ret = SHB_SMS_ServerValid(pNum,pData+head_len,DataLen-head_len);
    }
    else if(server_status == SHB_SMS_SERVER_TAIL)
    {
        ZDT_LOG("SHB SMS Server Tail");
        ret = SHB_SMS_ServerValid(pNum,pData,DataLen-head_len);
    }
    else
    {
    #ifdef SHB_DB_NEED_SOS
        if(0 != SHB_SMS_IsFromSOS(pNum))
        {
            ret = SHB_SMS_SOS_ContentValid(pNum,pData,DataLen,is_ucs2);
        }
    #endif
    }
        
    return ret;
}

uint16 SHB_SMS_Server_GetContentLen(SHB_APP_T *pMe)
{
    uint16 len = 0;
    
    return len;
}
int SHB_SMS_ServerParse_Reconn(SHB_APP_T *pMe)
{
    ZDT_LOG("SHB_SMS_ServerParse_Reconn");
    return AEE_SUCCESS;
}

char * SHB_SMS_GetNumber(void)
{
    return (char *)shb_DB_Set_Rec.sms_ct;
}

int SHB_API_SMS_SendLowBattery(void)
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
        
    num_len = SCI_STRLEN((char *)SHB_SMS_GetNumber());
    if(num_len == 0)
    {
        return AEE_EFAILED;
    }
    sprintf((char *)tmp_buf,"[%s]", g_zdt_phone_imei);
    MMIAPICOM_StrToWstr(tmp_buf,sms_buf);
    MMIAPICOM_Wstrcat(sms_buf,sms_tail);
    send_len = MMIAPICOM_Wstrlen(sms_buf);
    
    MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)sms_buf,send_len*2,SHB_SMS_GetNumber(),num_len,2,3);
  
    return AEE_SUCCESS;
}

int SHB_API_SMS_SendLose(void)
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
    
    num_len = SCI_STRLEN((char *)SHB_SMS_GetNumber());
    if(num_len == 0)
    {
        return AEE_EFAILED;
    }
    sprintf((char *)tmp_buf,"[%s]", g_zdt_phone_imei);
    MMIAPICOM_StrToWstr(tmp_buf,sms_buf);
    MMIAPICOM_Wstrcat(sms_buf,sms_tail);
    send_len = MMIAPICOM_Wstrlen(sms_buf);
    
    MMIZDT_SMS_AppSend(MN_DUAL_SYS_1,(uint8 *)sms_buf,send_len*2,SHB_SMS_GetNumber(),num_len,2,3);
  
    return AEE_SUCCESS;
}

int SHB_API_SMS_SendSOS(uint8* pStr, uint32 Len)
{
    uint16 num_len = 0;
    uint16 sms_buf[201] = {0};
    uint16 send_len = 0;
    uint8 * num_str = PNULL;
    uint16 i = 0;
    char tmp_buf[50] = {0};
    ZDT_LOG("SHB_API_SMS_SendSOS len = %d",Len);

    if(FALSE == MMIZDT_AppIsInit())
    {
        return AEE_EFAILED;
    }
    
    #ifndef WIN32
        send_len = GUI_UTF8ToWstr(sms_buf, 200, pStr, Len);	
    #else
        send_len = Len;
    #endif

    #if 1
        i = sos_connect_idx;
        if(i >= SHB_DB_SOS_MAX_SUM)
        {
            i = 0;
        }
        if(i == 0)
        {
            num_str = MMIAPIGET_SosCall_Num1();
        }
        else if(i == 1)
        {
            num_str = MMIAPIGET_SosCall_Num2();
        }
        else if(i == 2)
        {
            num_str = MMIAPIGET_SosCall_Num3();
        }
        else if(i == 3)
        {
            num_str = MMIAPIGET_SosCall_Num4();
        }
        else if(i == 4)
        {
            num_str = MMIAPIGET_SosCall_Num5();
        }
        if(num_str != PNULL)
        {
            num_len = SCI_STRLEN((char *)num_str);
            MMIZDT_SMS_AppSend(MMIZDT_Net_GetActiveSys(),(uint8 *)sms_buf,send_len*2,num_str,num_len,2,3);
        }
    #else
    for(i = 0; i < SHB_DB_SOS_MAX_SUM; i++)
    {
        if(shb_DB_Sos_Reclist[i].db_id != 0)
        {
            num_len = SCI_STRLEN(shb_DB_Sos_Reclist[i].sos_num);
            if(num_len > 0)
            {
                MMIZDT_SMS_AppSend(MMIZDT_Net_GetActiveSys(),(uint8 *)sms_buf,send_len*2,shb_DB_Sos_Reclist[i].sos_num,num_len,2,3);
            }
        }
    }
    #endif
    return AEE_SUCCESS;
}

int SHB_API_SMS_SendSOSExt(void)
{
    ////的设备向您发出紧急求助！7684 8BBE 5907 5411 60A8 53D1 51FA 7D27 6025 6C42 52A9  FF01 
    uint16 sms_tail[50] = {0x7684, 0x8BBE, 0x5907, 0x5411, 0x60A8, 0x53D1, 0x51FA, 0x7D27, 0x6025, 0x6C42, 0x52A9, 0xFF01, 0x0};
    uint16 num_len = 0;
    uint16 sms_buf[201] = {0};
    uint16 send_len = 0;
    uint8 * num_str = PNULL;
    uint16 i = 0;
    char tmp_buf[50] = {0};
    
    ZDT_LOG("SHB_API_SMS_SendSOSExt");

    if(SCI_STRLEN(shb_DB_AppSet_Rec.slef_num) > 0)
    {
        sprintf((char *)tmp_buf,"[%s]",shb_DB_AppSet_Rec.slef_num);
    }
    else
    {
        sprintf((char *)tmp_buf,"[%s]",g_zdt_phone_imei);
    }
    
    MMIAPICOM_StrToWstr(tmp_buf,sms_buf);
    MMIAPICOM_Wstrcat(sms_buf,sms_tail);
    send_len = MMIAPICOM_Wstrlen(sms_buf);
    #if 1
        i = sos_connect_idx;
        if(i >= SHB_DB_SOS_MAX_SUM)
        {
            i = 0;
        }
        if(i == 0)
        {
            num_str = MMIAPIGET_SosCall_Num1();
        }
        else if(i == 1)
        {
            num_str = MMIAPIGET_SosCall_Num2();
        }
        else if(i == 2)
        {
            num_str = MMIAPIGET_SosCall_Num3();
        }
        else if(i == 3)
        {
            num_str = MMIAPIGET_SosCall_Num4();
        }
        else if(i == 4)
        {
            num_str = MMIAPIGET_SosCall_Num5();
        }
        if(num_str != PNULL)
        {
            num_len = SCI_STRLEN((char *)num_str);
            MMIZDT_SMS_AppSend(MMIZDT_Net_GetActiveSys(),(uint8 *)sms_buf,send_len*2,num_str,num_len,2,3);
        }
    #else
        for(i = 0; i < SHB_DB_SOS_MAX_SUM; i++)
        {
            if(shb_DB_Sos_Reclist[i].db_id != 0)
            {
                num_len = SCI_STRLEN(shb_DB_Sos_Reclist[i].sos_num);
                if(num_len > 0)
                {
                    MMIZDT_SMS_AppSend(MMIZDT_Net_GetActiveSys(),(uint8 *)sms_buf,send_len*2,shb_DB_Sos_Reclist[i].sos_num,num_len,2,3);
                }
            }
        }
    #endif
    return AEE_SUCCESS;
}
int SHB_API_SMS_SendReg(void)
{
    //ZTEKHZTE.GA365.TH.T32ZTEKH89860311800250914398ZTEKH865563020023975
    uint16 num_len = 0;
    char sms_buf[100] = {0};
    uint16 send_buf[100] = {0};
    uint16 send_len = 0;    
    
    
    if(FALSE == MMIZDT_AppIsInit())
    {
        return AEE_EFAILED;
    }
    
    num_len = SCI_STRLEN((char *)SHB_SMS_GetNumber());
    if(num_len == 0)
    {
        return AEE_EFAILED;
    }

    sprintf(sms_buf,"ZTEKH%sZTEKH%sZTEKH%s",SHB_SOFT_VERSION,g_zdt_sim_iccid,SHB_Get_CurIMEI());
    send_len = strlen(sms_buf);
    ZDT_LOG("SHB_API_SMS_SendReg %s",sms_buf);
    MMIAPICOM_StrToWstr(sms_buf,send_buf);
    send_len = MMIAPICOM_Wstrlen(send_buf);

    MMIZDT_SMS_AppSend(MMIZDT_Net_GetActiveSys(),(uint8 *)send_buf,send_len*2,SHB_SMS_GetNumber(),num_len,2,1);
    //MMIZDT_SMS_AppSend(MMIZDT_Net_GetActiveSys(),(uint8 *)send_buf,send_len*2,"15994730009",num_len,2,3);
    return AEE_SUCCESS;
}

void SHB_SMS_Bind_Test(void)
{
    SCI_MEMSET(g_shb_sms_bdrq_num,0,MAX_SHB_IP_DOMAIN_SIZE+1);
    SCI_MEMCPY(g_shb_sms_bdrq_num,"15899663325",11);
    ZdtSHB_OpenBindWin();
}
#endif
