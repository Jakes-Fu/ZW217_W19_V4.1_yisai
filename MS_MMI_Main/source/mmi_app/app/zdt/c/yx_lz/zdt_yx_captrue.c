/*
云芯平台
*/
#include "zdt_app.h"

static uint8 YX_Captrue_GetMemStatus(YX_APP_T * pMe)
{
    BOOLEAN is_full = ZDT_File_MemFull(FALSE);
    if(is_full)
    {
        return 0;
    }
    return 1;
}

BOOLEAN YX_Captrue_FileSendAllRemove(YX_APP_T * pMe)
{
    if(pMe->m_yx_cap_is_sending == FALSE
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
        && pMe->m_yx_eRecorderStatus == YX_RECOREDER_STOP
#endif
        )
    {
        ZDT_File_RemoveDirFile(YX_CAPTRUE_SEND_DIR);
    }
    return TRUE;
}

static BOOLEAN YX_CAP_Send_Link_Add(YX_APP_T *pMe, uint8 * filename, uint32 name_len)
{
    YX_CAP_SEND_NODE_T  * p1 = NULL;
    YX_CAP_SEND_NODE_T  * p2 = NULL;
    uint32 len = name_len;

    if(len == 0)
    {
        return FALSE;
    }
    
    p1=(YX_CAP_SEND_NODE_T *)SCI_ALLOC_APPZ(sizeof(YX_CAP_SEND_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    
    p1->data.pFilename = (uint8 *)SCI_ALLOC_APPZ(len+1);
    if(p1->data.pFilename == NULL)
    {
        SCI_FREE(p1);
        return FALSE;
    }
    
    SCI_MEMCPY(p1->data.pFilename,filename,name_len);
    p1->data.pFilename[len] = 0;

    p1->data.len = len;
    p1->next_ptr = NULL;

    if(pMe->m_yx_cap_send_link_head == NULL)
    {
        pMe->m_yx_cap_send_link_head = p1;
    }
    else
    {
        p2 = pMe->m_yx_cap_send_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  YX_CAP_Send_Link_Get(YX_APP_T *pMe,YX_CAP_SEND_DATA_T * pCapSendData)
{
    YX_CAP_SEND_NODE_T  * p1 = NULL;
    
    p1 = pMe->m_yx_cap_send_link_head;
    
    if(p1 != NULL)
    {
      *pCapSendData = p1->data;
       return TRUE;
    }
        
    return FALSE;
}

static BOOLEAN  YX_CAP_Send_Link_DelOne(YX_APP_T *pMe)
{
    YX_CAP_SEND_NODE_T  * p1 = NULL;
    YX_CAP_SEND_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_yx_cap_send_link_head;
    
    while(p1 != NULL)
    {
        if(p1 == pMe->m_yx_cap_send_link_head)
        {
            pMe->m_yx_cap_send_link_head = p1->next_ptr;
        }
        else
        {
            p2->next_ptr = p1->next_ptr;
        }
        res = TRUE;
        if(p1->data.pFilename != NULL)
        {
            SCI_FREE(p1->data.pFilename);
        }
        SCI_FREE(p1);
        break;
    }
        
    return res;
}

BOOLEAN  YX_CAP_Send_Link_DelAll(YX_APP_T *pMe)
{
    YX_CAP_SEND_NODE_T  * p1 = NULL;
    ZDT_LOG("YX_CAP_SendFile_Link_DelAll");
    while(pMe->m_yx_cap_send_link_head != NULL)
    {
        p1 = pMe->m_yx_cap_send_link_head;
        pMe->m_yx_cap_send_link_head = p1->next_ptr;
        if(p1->data.pFilename!= 0)
        {
            ZDT_File_Remove((char *)p1->data.pFilename);
            SCI_FREE(p1->data.pFilename);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}


static BOOLEAN YX_Captrue_File_Check(const char * file_name)
{
    uint32 buf_len = 0;
    
    if(file_name == NULL)
    {
        return FALSE;
    }

    buf_len = ZDT_File_GetSize(file_name);
    
    if(buf_len < 19)
    {
        return FALSE;
    }
    
    return TRUE;
}

static int YX_Net_Send_CapFile_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char buf[21] = {0};
    int ret = 0;
    YX_APP_T *pMe = (YX_APP_T *)pUser;
    cont_len = YX_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        pContent = pRcv+(g_yx_imei_len + 10);
        Content_len = cont_len;
        ret = YX_Func_GetNextPara(&pContent, &Content_len,buf,20);
        if(ret > 0)
        {
            YX_Func_strupr(buf);
        }
        if(ret == 3 &&  strncmp( (char *)buf, "IMG", ret ) == 0)
        {
            ZDT_LOG("YX_Net_Send_CapFile_CB Success");
            return AEE_SUCCESS;
        }
    }

    ZDT_LOG("YX_Net_Send_CapFile_CB  ERR cont_len=%d",cont_len);
    return AEE_EFAILED;
}

/*
语音包格式是MM_AMR_FORMAT_FIXED_4_75_AMR
AMR 头 6 个字节,每一帧13个字节发送时需要对齐
*/
int32 YX_Net_Send_CapFile_Start(YX_APP_T *pMe)
{

    ZDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint32 len = 0;
    uint32 msg_len = 0;
    uint32 i = 0;
    uint32 j = 0;
    BOOLEAN bResult = TRUE;
    uint32 buf_len = 0;
    uint32 file_len = 0;
    uint32 time_len = 0;
    uint32 content_len = 0;
    MMIFILE_HANDLE file_handle;
    uint32 cur_len = 0;
    char * file_name = NULL;
    char len_str[5] = {0};
    uint8 time_str[30] = {0};
    uint8 * tmp_buf = NULL;
    YX_CAP_SEND_DATA_T cur_filedata = {0};
#if FEATURE_YX_SEND_MD5
    char md5_str[32] = {0};
#endif
    
    if(pMe->m_yx_cap_is_sending)
    {
        ZDT_LOG("Cap is Sending");
        return 0;
    }
    
    if(FALSE == YX_CAP_Send_Link_Get(pMe,&cur_filedata))
    {
        return 0;
    }

    if(pMe->m_yx_is_land == FALSE)
    {
        return -10;
    }
    
    if(g_yx_imei_len == 0)
    {
        return -1;
    }
    time_len = YX_Func_GetDateTimeEx(time_str);
    
    file_name = (char *)cur_filedata.pFilename;
    if(file_name == NULL)
    {
        ZDT_LOG("YX_Net_Send_CapFile_Start: ERR Name");
        return -1;
    }
        
    file_len = ZDT_File_GetSize(file_name);
    if(file_len < 19)
    {
        ZDT_LOG("YX_Net_Send_CapFile_Start: ERR File Len=%d",file_len);
        return -5;
    }
    
    tmp_buf = (uint8 * )SCI_ALLOC_APPZ(file_len+10);
    if(tmp_buf == NULL)
    {
        ZDT_LOG("YX_Net_Send_CapFile_Start: Malloc File Len=%d",file_len+10);
        return -6;
    }

    ZDT_File_Read(file_name, tmp_buf, file_len,&cur_len);
    if(cur_len >  0)
    {
        ZDT_LOG("YX_Net_Send_CapFile_Start: IFILE_Read cur_len=0x%x,file_len=0x%x",cur_len,file_len);
        pMe->m_yx_cap_file_seek = cur_len;
        content_len = cur_len;
        for(i = 0; i < cur_len; i++)
        {
            if(tmp_buf[i] == 0x7D)
            {
                content_len++;
            }
            else if(tmp_buf[i] == 0x5B)
            {
                content_len++;
            }
            else if(tmp_buf[i] == 0x5D)
            {
                content_len++;
            }
            else if(tmp_buf[i] == 0x2C)
            {
                content_len++;
            }
            else if(tmp_buf[i] == 0x2A)
            {
                content_len++;
            }
        }
    }
    else
    {
        SCI_FREE(tmp_buf);
        return -7;
    }
        
    cur_data=(ZDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        ZDT_LOG("YX_Net_Send_CapFile_Start: ERR Malloc 2");
        SCI_FREE(tmp_buf);
        return -8;
    }

    content_len = content_len + time_len + 6;
#if FEATURE_YX_SEND_MD5
    buf_len = content_len + g_yx_imei_len+43;
#else
    buf_len = content_len + g_yx_imei_len+11;
#endif
    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(buf_len);
    if(cur_data->str == NULL)
    {
        ZDT_LOG("YX_Net_Send_CapFile_Start: ERR Malloc buf_len=%d",buf_len);
        SCI_FREE(tmp_buf);
        SCI_FREE(cur_data);
        return -9;
    }
    
    //MEID
    SCI_MEMCPY(cur_data->str+len,g_zdt_phone_imei,g_yx_imei_len);
    len += g_yx_imei_len;
    cur_data->str[len++] = '*';

    j = len+time_len+11;
    for(i = 0; i < cur_len; i++)
    {
        if(tmp_buf[i] == 0x7D)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x01; 
        }
        else if(tmp_buf[i] == 0x5B)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x02; 
        }
        else if(tmp_buf[i] == 0x5D)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x03; 
        }
        else if(tmp_buf[i] == 0x2C)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x04; 
        }
        else if(tmp_buf[i] == 0x2A)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x05; 
        }
        else
        {
            cur_data->str[j++] = tmp_buf[i]; 
        }
    }
    
    SCI_FREE(tmp_buf);
    
    //包长度
    msg_len = j-len-5;
    sprintf(len_str,"%04X",msg_len);
    SCI_MEMCPY(cur_data->str+len,len_str,4);
    len += 4;
    cur_data->str[len++] = '*';
    cur_data->str[len++] = 'I';
    cur_data->str[len++] = 'M';
    cur_data->str[len++] = 'G';
    cur_data->str[len++] = ',';
    if(pMe->m_yx_cap_data_idx)
    {
        cur_data->str[len++] = '6';
    }
    else
    {
        cur_data->str[len++] = '5';
    }
    
    cur_data->str[len++] = ',';
    SCI_MEMCPY(cur_data->str+len,time_str,time_len);
    //结束]
    len = j;
    
#if FEATURE_YX_SEND_MD5
    YX_Net_MD5(cur_data->str+33,len-33,md5_str);
    SCI_MEMCPY(cur_data->str+1,md5_str,32);
#endif

    cur_data->str[len++] = YX_NET_MSG_END;

    pMe->m_yx_cap_pack_num = 1;
    pMe->m_yx_cap_pack_idx = 1;

    cur_data->len = len;
    cur_data->times = 0;
    cur_data->timeout = 0;
    cur_data->priority = 150;
    cur_data->rcv_handle = YX_Net_Send_CapFile_CB;
    ZDT_API_TCP_SendDataNow(pMe->m_yx_tcp_reg_interface,cur_data);
    pMe->m_yx_cap_is_sending = TRUE;
    ZDT_LOG("YX_Net_Send_CapFile_Start Start num=%d/%d,data_idx=%d,seek=%d,file_len=%d",\
        pMe->m_yx_cap_pack_idx,pMe->m_yx_cap_pack_num,pMe->m_yx_cap_data_idx,pMe->m_yx_cap_file_seek,file_len);
    return cur_len;
}

void YX_Net_Send_CapFile_End(YX_APP_T *pMe,uint8 err_id)
{
    YX_CAP_SEND_DATA_T cur_filedata = {0};
    
    if(YX_CAP_Send_Link_Get(pMe,&cur_filedata))
    {
        ZDT_File_Remove((char *)cur_filedata.pFilename);
        YX_CAP_Send_Link_DelOne(pMe);
    }
    
    pMe->m_yx_cap_file_seek = 0;
    pMe->m_yx_cap_is_sending = FALSE;
    
    if(YX_CAPSEND_SUCCESS == err_id)
    {
        //MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_CAPTRUE_RESEND,NULL,0);
    }
    else
    {        
        if(err_id != YX_CAPSEND_ERR_FILE)
        {
            //YX_CAP_Send_Link_DelAll(pMe);
        }
        else
        {
            //MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_CAPTRUE_RESEND,NULL,0);
        }
    }
    ZDT_LOG("YX_Net_Send_CapFile_End err_id=%d",err_id);
    return;
}

BOOLEAN YX_Captrue_Send_Start(YX_APP_T * pMe)
{
#if defined(ZDT_PLAT_YX_SUPPORT_YS) || defined(ZDT_PLAT_YX_SUPPORT_FZD) || defined(ZDT_PLAT_YX_SUPPORT_TX)
    if(0 > YX_Net_Send_CapFile_Start(pMe))
    {
        YX_Net_Send_CapFile_End(pMe,YX_CAPSEND_ERR);
    }
#else
        YX_Net_Send_CapFile_End(pMe,YX_CAPSEND_ERR);
#endif
}

int YX_Captrue_Send_Handle(YX_APP_T *pMe,DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != PNULL && p_getdata->str != PNULL &&  p_getdata->len > 0)
    {
        #if 0
            SCI_MEMSET(pMe->m_yx_pFileSendCapName, 0, MAX_YX_CAP_FILENAME_SIZE+1);
            strcpy((char *)pMe->m_yx_pFileSendCapName,"D:\\Photos\\DSC_0000001.jpg");
            SCI_MEMSET(pMe->m_yx_pFileSendCapFullName, 0, (MAX_YX_CAP_FILENAME_SIZE+1)*2);
            pMe->m_yx_pFileSendCapFullName_len = GUI_GBToWstr(pMe->m_yx_pFileSendCapFullName, (const uint8*)pMe->m_yx_pFileSendCapName, SCI_STRLEN(pMe->m_yx_pFileSendCapName));
        #else
            SCI_MEMSET(pMe->m_yx_pFileSendCapName, 0, MAX_YX_CAP_FILENAME_SIZE+1);
            SCI_MEMSET(pMe->m_yx_pFileSendCapFullName, 0, (MAX_YX_CAP_FILENAME_SIZE+1)*2);
            SCI_MEMCPY(pMe->m_yx_pFileSendCapFullName,p_getdata->str,p_getdata->len);
            #ifndef WIN32
#ifdef ZDT_SMS_SUPPORT
                pMe->m_yx_pFileSendCapFullName_len = ucs2str_to_gb2312strExt(p_getdata->str, p_getdata->len,pMe->m_yx_pFileSendCapName,0);
#else
                pMe->m_yx_pFileSendCapFullName_len = p_getdata->len;
#endif
            #else
                pMe->m_yx_pFileSendCapFullName_len = p_getdata->len;
            #endif
        #endif
        ZDT_LOG("YX_Captrue_Send_Handle len=%d, %s",pMe->m_yx_pFileSendCapFullName_len,pMe->m_yx_pFileSendCapName);
        if(YX_CAP_Send_Link_Add(pMe,(uint8 *)pMe->m_yx_pFileSendCapName,MAX_YX_CAP_FILENAME_SIZE))
        {
            YX_Captrue_Send_Start(pMe);
        }
    }
    if(p_getdata != PNULL && p_getdata->str != PNULL)
    {
        SCI_FREE(p_getdata->str);
    }
    return 0;
}

int YX_Captrue_Handle(YX_APP_T *pMe,DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != PNULL && p_getdata->len > 0)
    {
        pMe->m_yx_cap_data_idx = 1;
    }
    else
    {
        pMe->m_yx_cap_data_idx = 0;
    }
    #ifdef CAMERA_SUPPORT
        //MMIAPIDC_OpenPhotoWinForYX();
    #endif
    return 0;
}

BOOLEAN YX_API_Captrue_Start(uint8 is_local)
{
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_CAPTRUE,PNULL,is_local);
    return TRUE;
}

BOOLEAN YX_API_Captrue_Send(uint16 * full_path, uint16  full_path_len)
{
    uint8 * str_path = (uint8 *)full_path;
    uint16 str_len = full_path_len * 2;
    ZDT_LOG("YX_API_Captrue_Send str_len=%d",str_len);
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_CAPTRUE_SEND,str_path,str_len);
    return TRUE;
}

