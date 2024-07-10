
#include "ai_app.h"


#ifdef TULING_AI_SUPPORT
#include "ai_app.h"
#include "ai_common.h"
#include "ai_func.h"


LOCAL BOOLEAN  AI_Net_PDPTry(MN_DUAL_SYS_E sim_sys);
LOCAL BOOLEAN AI_Net_PdpDeactive(void);
PUBLIC MN_DUAL_SYS_E MMIAI_Net_GetActiveSys(void);
PUBLIC BOOLEAN MMIAI_Net_Open(void);
PUBLIC BOOLEAN MMIAI_Net_Close(void);
PUBLIC BOOLEAN MMIAI_Http_Request_SendMsg(void);







#define AI_CMD_START                                            0x55                              
#define AI_CMD_START2                                               0x55
#define AI_CMD_END                                            0x0D
#define AI_CMD_END2                                               0x0A
#define AI_HEAD_LEN 5
#define AI_DATA_MAX_LEN 2048

static uint8 ai_cmd_line_buffer[AI_DATA_MAX_LEN];     /*  buffer for AT cmd line  */
static uint16 ai_cmd_line_len = 0;
static uint16 ai_data_need_len = 0;
static uint8 *ai_cmd_ptr;                     /* next loc in buffer       */
static uint8 ai_cmd_status = 0;


LOCAL uint32                      s_ai_net_id            = 0;



static char upload_head[] = 
	"POST /speech/chat HTTP/1.1\r\n"
    "Host: %s\r\n"
    "Connection: keep-alive\r\n"
	"Content-Length: %d\r\n"
    "Cache-Control: no-cache\r\n"
    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n"
	"Content-Type: multipart/form-data; boundary=%s\r\n"
    "Accept: */*\r\n"
    "Accept-Language: en-US,en;q=0.8,zh-CN;q=0.6,zh;q=0.4,zh-TW;q=0.2,es;q=0.2\r\n"
    "\r\n";

static char upload_parameters[] = 
	"Content-Disposition: form-data; name=\"parameters\"\r\n\r\n%s";

static char upload_speech[] = 
    "Content-Disposition: form-data; name=\"speech\"; filename=\"recordspeech.amr\"\r\n"
   "Content-Type: application/octet-stream\r\n\r\n";


uint16 g_ai_net_msg_index = 0;
BOOLEAN g_is_ai_test_server = FALSE;
BOOLEAN g_is_ai_server_fixed = FALSE;
BOOLEAN g_is_ai_czsb_is_playing = FALSE;
BOOLEAN g_is_ai_power_off_ing = FALSE;

LOCAL uint8 ai_http_url_request_timer_id = 0;
LOCAL char next_url_addr[256] = {0};


char *host = "smartdevice.ai.tuling123.com";


LOCAL void MMIAI_Http_Init_Request()
{
    if(g_ai_app.m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        AI_LOG("AI__NET MMIAI_Http_Init_Request ,is recording ,stop http init");
        return ;
    }
    AIHttpDefaultParamsInit();
    StartAiHttpRequest();
}

LOCAL void AI_UrlSetNextRequestUrl(char* url)
{
    if(url && SCI_STRLEN(url) > 0)
    {
       SCI_MEMSET(next_url_addr, 0, 256*sizeof(char));
       SCI_STRCPY(next_url_addr, url);
    }
}

PUBLIC void AI_clearNextUrlAddr()
{
    SCI_MEMSET(next_url_addr, 0, 256*sizeof(char));
}

PUBLIC BOOLEAN AI_IsNextUrlAddrWaiting()
{
    BOOLEAN ret = FALSE;
    if(next_url_addr != NULL && SCI_STRLEN(next_url_addr) > 0)
    {
        ret = TRUE;  
    }
    return ret;
}

PUBLIC void AI_InitNextUrlRequest()
{
    
    if(next_url_addr && SCI_STRLEN(next_url_addr) > 0)
    {
        AI_LOG("AI__NET AI_InitNextUrlRequest \n");
        AiSetHttpUrl(next_url_addr);
        AI_clearNextUrlAddr();
        MMIAI_Http_Request_SendMsg();
        
    }
    

}


LOCAL void AI_UrlRequestTimerFunc()
{
    if(next_url_addr && SCI_STRLEN(next_url_addr) > 0)
    {
       
    }
}


LOCAL void AI_UrlRequestStartTimer(void)
{
    if(0 != ai_http_url_request_timer_id)
    {
        MMK_StopTimer(ai_http_url_request_timer_id);
        ai_http_url_request_timer_id = 0;
    }
    
    ai_http_url_request_timer_id = MMK_CreateTimerCallback(3000, 
                                                                        AI_UrlRequestTimerFunc, 
                                                                        PNULL, 
                                                                       FALSE);
}

LOCAL void AI_UrlRequestStopTimer(void)
{
    if(0 != ai_http_url_request_timer_id)
    {
        MMK_StopTimer(ai_http_url_request_timer_id);
        ai_http_url_request_timer_id = 0;
    }       
}



static int AI_Net_SendCB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
/*
    服务器地址 4
    端口 2
*/
    AI_APP_T *pMe = &g_ai_app;
    uint8 * pContent;
    uint16 flag = 0;
    int i = 0;
    uint8 * json_str;
    cJSON *rcvJsonPtr;
    int ret_code = 0;
    char* err_msg;
    char* asr_msg;
    char* tts_msg;
    char* nlp_addr;
    //char* func_str;
    char* url_str;
    char* token;
    cJSON* tmpJson = NULL;
    cJSON* funcJson = NULL;
    uint8 utf8_err_str[16] = { 0xE6, 0x9C, 0xAA, 0xE8, 0xAF, 0x86, 0xE5, 0x88, 0xAB, 0xE8, 0xAF, 0xAD, 0xE9, 0x9F, 0xB3, 0 };
    
    

	AI_LOG("AI__NET AI_Net_SendCB Rcv =%s,Rcv_len =%d\n",pRcv,Rcv_len);
    for (i = 0 ;i < SCI_STRLEN(pRcv) - 1 ; i++)
    {
        if(pRcv[i] == '\n' || pRcv[i] == '\r')
        {
            //AI_LOG("AI__NET AI_Net_SendCB  i = %d", i);
            if(pRcv[i+1] == '{')
            {
                flag = i+1;
                break;
            }
        }
    }
    json_str = pRcv+flag;
    AI_LOG("AI__NET AI_Net_SendCB json_str =%s \n", json_str);
    rcvJsonPtr = cJSON_Parse(json_str);
    
    if(rcvJsonPtr)
    {
        tmpJson = cJSON_GetObjectItem(rcvJsonPtr, "code");
        if(tmpJson)
        {
            ret_code = tmpJson->valueint;
            AI_LOG("AI__NET AI_Net jsonparse code:%d \n", ret_code);
        }
        tmpJson = cJSON_GetObjectItem(rcvJsonPtr, "err");
        if(tmpJson)
        {
            err_msg = tmpJson->valuestring;
            AI_LOG("AI__NET AI_Net jsonparse err:%s \n", err_msg);
        }
        tmpJson = cJSON_GetObjectItem(rcvJsonPtr, "asr");
        if(tmpJson)
        {
            asr_msg = tmpJson->valuestring;
            AI_LOG("AI__NET AI_Net jsonparse asr:%s \n", asr_msg);
            AppendAIListContent(asr_msg, FALSE); //xiongkai TRUE    机器答复靠左侧     手表端录音靠右侧   和微聊统一
        }
        else
        {
            if((err_msg[0]==0x61) && (err_msg[1]==0x73) && (err_msg[2]==0x72))
            {
                AppendAIListContent( (uint8*)utf8_err_str, FALSE);
            }
            else
            {
                AppendAIListContent(err_msg, FALSE); //xiongkai TRUE      机器答复靠左侧     手表端录音靠右侧   和微聊统一
            }
        }
        tmpJson = cJSON_GetObjectItem(rcvJsonPtr, "tts");
        if(tmpJson)
        {
            tts_msg = tmpJson->valuestring;
            AI_LOG("AI__NET AI_Net jsonparse tts:%s \n", tts_msg);
            AppendAIListContent(tts_msg, TRUE); //xiongkai FALSE      机器答复靠左侧     手表端录音靠右侧   和微聊统一
        }
        tmpJson = cJSON_GetObjectItem(rcvJsonPtr, "nlp");
        
        if(tmpJson)
        {
            
            nlp_addr = cJSON_GetArrayItem(tmpJson, 0)->valuestring;
            AI_LOG("AI__NET AI_Net jsonparse nlp:%s \n", nlp_addr);
            //if(nlp_addr != NULL && SCI_STRLEN(nlp_addr) > 0)
            if(nlp_addr != NULL )
            {
                AI_LOG("AI__NET AI_Net jsonparse nlp_addr start http \n");
                /*AiSetHttpUrl(nlp_addr);
                AIHttpDefaultParamsInit();
                StartAiHttpRequest(); */ //HandleAiHttpMsgExt handle msg //bug ,now is in tcp thread,should go to app module handle it
                AiSetHttpUrl(nlp_addr);
                
                MMIAI_Http_Request_SendMsg();
            }
        }
        tmpJson = cJSON_GetObjectItem(rcvJsonPtr, "token");
        if(tmpJson)
        {
            token = tmpJson->valuestring;
            AI_LOG("AI__NET AI_Net jsonparse token:%s \n", token);
            if(token)
            {
                AI_APP_T  *pMe = &g_ai_app;
                SCI_MEMSET(pMe->token, 0, 64);
                SCI_STRCPY(pMe->token, token);
                pMe->m_ai_tcp_reg_interface->m_tcp_is_sending = FALSE;
                if(err_msg && 0 == strcmp(err_msg , "token invalid value"))
                {
                    SCI_MEMSET(pMe->token, 0, 64);
                    AI_Tcp_ForceCloseAll();//close tcp socket and thread
                }
                
            }
        }

        funcJson = cJSON_GetObjectItem(rcvJsonPtr, "func");
        
        if(funcJson)
        {
            
            //url_str = cJSON_GetArrayItem(funcJson, 0)->valuestring;
            tmpJson = cJSON_GetObjectItem(funcJson, "url");
            if(tmpJson)
            {
                url_str = tmpJson->valuestring;
                AI_LOG("AI__NET AI_Net jsonparse url:%s \n", url_str);
                if(url_str != NULL )
                {
                    AI_LOG("AI__NET AI_Net jsonparse nlp_addr start http \n");
                    
                    //AiSetHttpUrl(nlp_addr);
                    //MMIAI_Http_Request_SendMsg();
                    AI_UrlSetNextRequestUrl(url_str);
                    
                }
            }
            
        }
        cJSON_Delete(rcvJsonPtr);
    }
    

    AI_LOG("AI__NET AI_Net_SendCB  end");
    return AI_TCP_RET_ERR;
}



extern uint32 AI_Net_TCPSendFile(char* file_name,AITCPRCVHANDLER rcv_handle)
{
    AI_TCP_LINK_DATA_T * cur_data = NULL;
    uint8 *fileData = NULL; 
    uint32 len = 0;
    uint32 head_len = 0;
    uint32 read_len = 0;
    uint32 file_len = 0;
    uint32 all_len = 0;
    char *boundary_header = "------AiWiFiBoundary";
    char* end = "\r\n"; 			
    char* twoHyphens = "--";		
    char s[20] = {0};
    char *boundary =PNULL;
    char firstBoundary[128]={0};
    char secondBoundary[128]={0};
    char endBoundary[128]={0};
    uint8 ai_ptr[1024]= {0};
    char *parameter_data=PNULL;
    int content_length =0;
    char header_data[4096] = {0};
    int ret = 0;
    AI_LOG("AI__NET AI_Net_TCPSendFile = %s\n",file_name);


    get_rand_str(s,19);
    AI_LOG("AI__NET %s\n",s);
    boundary = malloc(strlen(boundary_header)+strlen(s) +1);
    //char boundary[strlen(boundary_header)+strlen(s) +1];
    memset(boundary, 0, strlen(boundary_header)+strlen(s) +1);
    strcat(boundary, boundary_header);
    strcat(boundary, s);
    AI_LOG("AI__NET boundary is : %s\n", boundary);
    sprintf(firstBoundary, "%s%s%s", twoHyphens, boundary, end);
    sprintf(secondBoundary, "%s%s%s%s", end, twoHyphens, boundary, end);
    sprintf(endBoundary, "%s%s%s%s%s", end, twoHyphens, boundary, twoHyphens, end);

    //file_len = UserFile_GetFileSize("TEST",file_name);
    file_len = AI_File_GetSize(file_name);
    AI_LOG("AI__NET AI_File_GetSize file_len=%d",file_len);

    CTAIHttp_WritePostBody(ai_ptr);

    AI_LOG("AI__NET parameters is : %s\n", ai_ptr);
    parameter_data = malloc(strlen(ai_ptr)+ strlen(upload_parameters) + strlen(boundary) + strlen(end)*2 + strlen(twoHyphens) +1);
    sprintf(parameter_data, upload_parameters, ai_ptr);
    strcat(parameter_data, secondBoundary);

    content_length = len+ strlen(boundary)*2 + strlen(parameter_data) + strlen(upload_speech) + strlen(end)*3 + strlen(twoHyphens)*3 + file_len;
    ret = _snprintf(header_data,4096, upload_head, host, content_length, boundary);

    strcat(header_data,firstBoundary);

    AI_LOG("AI__NET %s\n", firstBoundary);
    strcat(header_data,parameter_data);   
    strcat(header_data,upload_speech);

    head_len = strlen(header_data);

    AI_LOG("AI__NET AI_Net_TCPSendFile: head_len=%d ", head_len);

    cur_data=(AI_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        AI_LOG("AI__NET AI_Net_TCPSend: ERR Malloc");
        return 0;
    }

    cur_data->str= (uint8 *)SCI_ALLOC_APPZ( head_len+file_len+200);
    if(cur_data->str == NULL)
    {
        AI_LOG("AI__NET AI_Net_TCPSend: ERR Malloc ");
        SCI_FREE(cur_data);
        return 0;
    }
    	

    SCI_MEMCPY(cur_data->str,header_data,head_len);
    fileData = cur_data->str+head_len;
    //UserFile_ReadExt("TEST",file_name,fileData,file_len,&read_len,PNULL);// error api use, need use path name two params
    AI_File_Read(file_name, fileData, file_len, &read_len); //only need full path
    
    len = strlen(endBoundary);
    AI_LOG("AI__NET AI_Net_TCPSendFile: file_len=%d ,read_len=%d endBoundaryLen=%d", file_len, read_len, len);
    SCI_MEMCPY(cur_data->str+head_len+read_len,endBoundary,len);
    all_len = head_len + read_len + len;

    cur_data->pMe = g_ai_app.m_ai_tcp_reg_interface;  //bug ,null
    cur_data->len = all_len;
    cur_data->times = 0;
    cur_data->timeout = 0;
    cur_data->priority = 0;
    cur_data->rcv_handle = rcv_handle;
    AI_LOG("AI__NET AI_Net_TCPSendFile: cur_data->str len=%d ", SCI_STRLEN(cur_data->str));
    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_ADD,cur_data,all_len);

    SCI_FREE(boundary);
    SCI_FREE(parameter_data);

    AI_LOG("AI__NET AI_Net_TCPSend Handle=0x%x",(uint32)cur_data);
    return (uint32)cur_data;
}

extern uint32 AI_Net_TCPSend(uint8* fileData,char *host,uint8 * pContent,uint16 ContentLen,uint8 priority,uint8 repeat_num,uint32 timeout,AITCPRCVHANDLER rcv_handle)
{


    AI_TCP_LINK_DATA_T * cur_data = NULL;
    uint16 len = 0;
    uint16 msg_len = 0;
    uint16 crc = 0;

    char *boundary_header = "------AiWiFiBoundary";
    char* end = "\r\n"; 			
    char* twoHyphens = "--";		
    char s[20] = {0};
    char *boundary =PNULL;
    char firstBoundary[128]={0};
    char secondBoundary[128]={0};
    char endBoundary[128]={0};
    uint8 ai_ptr[1024]= {0};
    char *parameter_data=PNULL;
    int content_length =0;
    char header_data[4096] = {0};
    int ret = 0;


     get_rand_str(s,19);
     AI_LOG("AI__NET %s\n",s);
     boundary = malloc(strlen(boundary_header)+strlen(s) +1);
     //char boundary[strlen(boundary_header)+strlen(s) +1];
     memset(boundary, 0, strlen(boundary_header)+strlen(s) +1);
     strcat(boundary, boundary_header);
     strcat(boundary, s);
     AI_LOG("AI__NET boundary is : %s\n", boundary);
     sprintf(firstBoundary, "%s%s%s", twoHyphens, boundary, end);
    sprintf(secondBoundary, "%s%s%s%s", end, twoHyphens, boundary, end);
    sprintf(endBoundary, "%s%s%s%s%s", end, twoHyphens, boundary, twoHyphens, end);

    CTAIHttp_WritePostBody(ai_ptr);

    AI_LOG("AI__NET parameters is : %s\n", ai_ptr);
    parameter_data = malloc(strlen(ai_ptr)+ strlen(upload_parameters) + strlen(boundary) + strlen(end)*2 + strlen(twoHyphens) +1);
    sprintf(parameter_data, upload_parameters, ai_ptr);
    strcat(parameter_data, secondBoundary);

    content_length = len+ strlen(boundary)*2 + strlen(parameter_data) + strlen(upload_speech) + strlen(end)*3 + strlen(twoHyphens)*3;
    ret = _snprintf(header_data,4096, upload_head, host, content_length, boundary);

    if(content_length == 0  )
    {
        AI_LOG("AI__NET AI_Net_TCPSend: ERR NULL");
        return 0;
    }

    cur_data=(AI_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        AI_LOG("AI__NET AI_Net_TCPSend: ERR Malloc");
        return 0;
    }


    cur_data->str= (uint8 *)SCI_ALLOC_APPZ( sizeof(header_data)*4);
    if(cur_data->str == NULL)
    {
        AI_LOG("AI__NET AI_Net_TCPSend: ERR Malloc len=%d",ContentLen);
        SCI_FREE(cur_data);
        return 0;
    }
    ContentLen = strlen(header_data);
    SCI_MEMSET(cur_data->str,0,ContentLen);
    SCI_MEMCPY(cur_data->str,header_data,ContentLen);

    AI_LOG("AI__NET %s\n", header_data);

    strcat(cur_data->str,firstBoundary);

    AI_LOG("AI__NET %s\n", firstBoundary);
    strcat(cur_data->str,parameter_data);   
    strcat(cur_data->str,upload_speech);	
    strcat(cur_data->str,fileData);	

    strcat(cur_data->str,endBoundary);	
    ContentLen = strlen(cur_data->str);
    AI_LOG("AI__NET AI_Net_TCPSend cur_data->str =%s,ContentLen=%d",cur_data->str,ContentLen);

    cur_data->pMe = g_ai_app.m_ai_tcp_reg_interface;
    cur_data->len = ContentLen;
    cur_data->times = 0;
    cur_data->timeout = 0;
    cur_data->priority = 0;
    cur_data->rcv_handle = rcv_handle;
    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_ADD,cur_data,ContentLen);

    SCI_FREE(boundary);
    SCI_FREE(parameter_data);

    AI_LOG("AI__NET AI_Net_TCPSend Handle=0x%x",(uint32)cur_data);
    return (uint32)cur_data;
}
/*
void  AI_TCPSendMessage(void)
{
	char *fileData = NULL; 
	uint32 len = 0;
	uint32 read_len = 0;
	len = Get_mp3_len();
	AI_LOG("AI__NET Get_mp3_len fileData len =%d",len);
	fileData = (uint8 *)SCI_ALLOC_APPZ(len);
	if(fileData ==NULL)
	{
		AI_LOG("AI__NET AI_TCPSendMessage fileData =NULL");
		return;
	}
	read_len = Upload_mp3_data(fileData);
	AI_LOG("AI__NET AI_TCPSendMessage fileData =%s,len =%d",fileData,read_len);
	if(read_len < 10* 1024)
	{
		AI_Net_TCPSend(fileData ,host,PNULL,0,0,0,0,AI_Net_SendCB);
	}
	if(fileData != PNULL)
	{
		SCI_FREE(fileData);
	}
}*/

void  AI_TCPSendTestFile(void)
{
	AI_Net_TCPSendFile("recordspeech.amr",AI_Net_SendCB);//not good ,change to full path
}

void  AI_TCPSendAIRecordFile()
{
    
    uint16 len = SCI_STRLEN(g_ai_app.m_ai_pFileSendVocName) ;
    AI_LOG("AI__NET AI_TCPSendAIRecordFile len=%d, %s", len, g_ai_app.m_ai_pFileSendVocName);
    if(len > 0)
    {
        if(MMIFILE_IsFileExist(g_ai_app.m_ai_pFileSendVocFullName,g_ai_app.m_ai_pFileSendVocFullName_len))
        {
           
           if(s_ai_net_id > 0 || MMIZDT_Net_GetNetID() > 0)
           {
                
               AI_Net_TCPSendFile(g_ai_app.m_ai_pFileSendVocName, AI_Net_SendCB);

           }
           
           
        }
        
        AI_File_Remove(g_ai_app.m_ai_pFileSendVocName);
        //SCI_MEMSET(g_ai_app.m_ai_pFileSendVocName, 0, MAX_AI_VOC_FILENAME_SIZE+1);
        
        
    }
    
}


void  AI_TCPSendFile(char* full_file_name)
{
	AI_Net_TCPSendFile(full_file_name,AI_Net_SendCB);
}



static int AI_Net_Reg_CallBack(void * pUser, AI_TCP_CB_TYPE_E cb_type, uint16 wParam, uint32 dwParam)
{
    AI_TCP_INTERFACE_T *pTcp = (AI_TCP_INTERFACE_T *)pUser;
    AI_APP_T *pMe = &g_ai_app;
    AI_LOG("AI__NET AI_Net_Reg_CallBack: cb_type=%d,Result=%d,Data=0x%x",cb_type,wParam,dwParam);

    switch(cb_type)
    {
    	 case TCP_CB_DISCONNECTED:
                AI_LOG("AI__NET AI_Net TCP_CB_DISCONNECTED");
                
            break;
            
    	case TCP_CB_REG:
			//AI_Net_TCPSend(host,PNULL,0,0,0,0,AI_Net_SendCB);
			///AI_TCPSendMessage();
			//AI_TCPSendFile();
		break;
            
        case TCP_CB_SEND:
            {
                uint16 ret = wParam;
                AI_TCP_LINK_DATA_T * SendData = (AI_TCP_LINK_DATA_T*)dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //发送成功
                    AI_LOG("AI__NET AI_Net_TCPSend Result Success Handle=0x%x",SendData);
                    if(SendData != NULL && SendData->str != NULL)
                    {
                       
                     
                    }
                }
                else
                {
                    //发送失败
                    AI_LOG("AI__NET AI_Net_TCPSend Result FAIL Handle=0x%x",SendData);
                   
                    
                }
            }
            break;
            
        case TCP_CB_RCV:
            {
                uint16 ret = wParam;
                AI_TCP_RCV_DATA_T * RcvData = (AI_TCP_RCV_DATA_T *) dwParam;
                if(ret == TCP_SUCCESS)
                {
                    //接收成功
                    AI_LOG("AI__NET AI_Net_Receive Over");
                }
                else
                {
                    //接收失败
                    AI_LOG("AI__NET AI_Net_Receive FAIL");
                }
            }
            break;
                        
        default:
            break;
    }
    return 0;
}

int AI_Net_Reg(void)
{
    AI_LOG("AI__NET AI_Net_Reg is_test=");
    if(FALSE == AI_Net_IsInit())
    {
        return 1;
    }

    AI_API_TCP_UnReg(g_ai_app.m_ai_tcp_reg_interface);
    
    {
        AI_API_TCP_Reg(g_ai_app.m_ai_tcp_reg_interface,"smartdevice.ai.tuling123.com",80,AI_Net_Reg_CallBack);
    }

    return 0;
}


extern BOOLEAN AI_Net_IsOpen(AI_APP_T* pMe)
{
    return pMe->m_ai_net_is_open;
}

extern int AI_Net_Open(AI_APP_T* pMe)
{
    AI_LOG("AI__NET AI_Net_Open");
    pMe->m_ai_is_reg = FALSE;
    AI_Net_Reg();
    pMe->m_ai_net_is_open = TRUE;
    return 0;
}

extern int AI_Net_Close(AI_APP_T* pMe)
{
    AI_LOG("AI__NET AI_Net_Close");
    pMe->m_ai_is_reg = FALSE;
    if(AI_Net_IsInit())
    {
        AI_API_TCP_UnReg(g_ai_app.m_ai_tcp_reg_interface);
    }
    pMe->m_ai_net_is_open = FALSE;
   return 0;
}


extern BOOLEAN AI_Net_IsAllow(AI_APP_T* pMe)
{
    if( pMe->m_ai_net_is_init)
    {
        return TRUE;
    }
    AI_LOG("AI__NET AI_Net_IsAllow FALSE");
    return FALSE;
}

int AI_Net_Init(void)
{
    AI_APP_T * pMe = &g_ai_app;
    AI_LOG("AI__NET AI_Net_Init");
    if(AITCP_NetOpen(&(g_ai_app.m_ai_tcp_reg_interface),PNULL,10240))
    {
        AI_LOG("AI__NET AI_Net_Init Reg OK ");
    }
  
    pMe->m_ai_net_is_init = TRUE;
    if(AI_Net_IsAllow(pMe))
    {
        AI_Net_Open(pMe);
    }
    return 0;
}

BOOLEAN AI_Net_IsInit(void)
{
    return g_ai_app.m_ai_net_is_init;
}

extern int AI_Net_Reset(AI_APP_T* pMe)
{
    AI_LOG("AI__NET AI_Net_Reset");
    if(FALSE == AI_Net_IsInit())
    {
        return 1;
    }
    if(AI_Net_IsAllow(pMe))
    {
       AI_Net_Open(pMe);
    }
    else
    {
       AI_Net_Close(pMe);
    }
    return 0;
}

extern int AI_API_Net_Reset(void)
{
  ///  MMIAI_SendSigTo_APP(AI_APP_SIGNAL_HYHL_NET_RESET,NULL,0);
    return 0;
}

///============================

#define AI_NET_TRACE            SCI_TRACE_LOW


#define AI_PDP_ACTIVE_TIMEOUT       30000  
#define AI_PDP_RETRY_TIMEOUT        2000 //重新尝试连接pdp，换卡连接

#define AI_PDP_DEFAULT_SIM_SYS  MN_DUAL_SYS_1

#ifdef MEID_WRITE_SUPPORT
char         g_ai_phone_meid[AI_IMEI_LEN+1] = {0};
#endif
char         g_ai_phone_imei[AI_IMEI_LEN+1] = {0};
char         g_ai_phone_imei_2[AI_IMEI_LEN+1] = {0};
char         g_ai_sim_imsi[AI_IMSI_LEN+1] = {0};
char         g_ai_sim_iccid[AI_ICCID_LEN+1] = {0};
uint32         g_ai_net_cell_id = 0;



LOCAL MN_DUAL_SYS_E       s_ai_net_set_sys   = AI_PDP_DEFAULT_SIM_SYS;
LOCAL MN_DUAL_SYS_E       s_ai_net_cur_sys   = MN_DUAL_SYS_1;
LOCAL uint8                       s_ai_net_pdp_timer_id      = 0;
LOCAL uint8                       s_ai_net_pdp_retry_timer_id = 0;

LOCAL BOOLEAN                  s_ai_net_pdp_state_is_ok        = FALSE;
LOCAL BOOLEAN                  s_ai_net_is_init        = FALSE;
LOCAL BOOLEAN                  s_ai_net_need_init        = TRUE;
LOCAL BOOLEAN                  s_ai_net_gprs_status[MN_DUAL_SYS_MAX+1] = {0};
LOCAL BOOLEAN                  s_ai_net_is_activing        = FALSE;
LOCAL BOOLEAN                  s_ai_net_is_closing        = FALSE;

LOCAL BOOLEAN                  s_ai_net_need_active_close        = FALSE;
LOCAL BOOLEAN                  s_ai_net_need_close_active        = FALSE;



/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void AI_Net_HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    AI_NET_TRACE("AI NET PdpActive ERR TimeOut");
    AI_Net_PdpDeactive();
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void AI_Net_StartPdpActiveTimer(void)
{
    if(0 != s_ai_net_pdp_timer_id)
    {
        MMK_StopTimer(s_ai_net_pdp_timer_id);
        s_ai_net_pdp_timer_id = 0;
    }
    
    s_ai_net_pdp_timer_id = MMK_CreateTimerCallback(AI_PDP_ACTIVE_TIMEOUT, 
                                                                        AI_Net_HandlePdpActiveTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}
/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void AI_Net_StopPdpActiveTimer(void)
{
    if(0 != s_ai_net_pdp_timer_id)
    {
        MMK_StopTimer(s_ai_net_pdp_timer_id);
        s_ai_net_pdp_timer_id = 0;
    }
}

LOCAL void AI_Net_HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    if(timer_id == s_ai_net_pdp_retry_timer_id && 0 != s_ai_net_pdp_retry_timer_id)
    {
        MMK_StopTimer(s_ai_net_pdp_retry_timer_id);
        s_ai_net_pdp_retry_timer_id = 0;
    }
    s_ai_net_cur_sys++;
    if(s_ai_net_cur_sys < MMI_DUAL_SYS_MAX)
    {
        for(sys_sim = s_ai_net_cur_sys; sys_sim < MMI_DUAL_SYS_MAX; sys_sim++)
        {
            if(MMIPHONE_IsSimOk(sys_sim))
            {
                s_ai_net_cur_sys = sys_sim;
                sim_ok = TRUE;
                break;
            }
        }
        if(sim_ok)
        {
            if(!AI_Net_PDPTry(s_ai_net_cur_sys))
            {
                AI_NET_TRACE("AI NET PdpActive Retry ERR cur_sys(%d),",s_ai_net_cur_sys);
        	    s_ai_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                AI_PDP_RETRY_TIMEOUT, 
                                                                                                AI_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
        }
        else
        {
            AI_NET_TRACE("AI NET PdpActive Retry ERR NO SIM");
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
    }
    else//找不到卡了
    {
        AI_NET_TRACE("AI NET PdpActive Retry ERR no_sys");
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
    }
}

/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void AI_Net_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }
    
    AI_NET_TRACE("AI NET HandlePdpMsg msg_id=%d,result=%d",msg_ptr->msg_id,msg_ptr->result);
    
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        AI_Net_StopPdpActiveTimer();
        
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            s_ai_net_id = msg_ptr->nsapi;
            AI_NET_TRACE("AI NET MMIPDP_ACTIVE_CNF OK net_id=0x%x",s_ai_net_id);
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        }
        else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
        {
            AI_NET_TRACE("AI NET MMIPDP_ACTIVE_CNF FAIL");
            AI_Net_PdpDeactive();
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }
        else 
        {
            AI_NET_TRACE("AI NET MMIPDP_ACTIVE_CNF FAIL Other");
            AI_Net_PdpDeactive();
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
            AI_NET_TRACE("AI NET MMIPDP_DEACTIVE_CNF");
            s_ai_net_id = 0;
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
            AI_NET_TRACE("AI NET MMIPDP_DEACTIVE_IND");
            s_ai_net_id = 0;
            AI_Net_PdpDeactive();
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        break;
        
    default:
        break;
    }
    
}
/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
#if 1
LOCAL BOOLEAN  AI_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
#if defined(MMI_GPRS_SUPPORT)
    BOOLEAN                 return_val      = FALSE;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_APNTYPEINDEX_T *apn_idx_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    AI_NET_TRACE("AI NET ConnectPDP");

    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&sim_sys))
    {
        SCI_TraceLow("AI NET ConnectPDP [SIM ERROR]");
        return FALSE;
    }

    active_info.app_handler         = MMI_MODULE_TULINGAI;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = NULL;
    active_info.user_name_ptr       = NULL;
    active_info.psw_ptr             = NULL;
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_TD_PREFER;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = AI_Net_HandlePdpMsg;
    active_info.ps_service_type     = BROWSER_E;
    active_info.storage             = MN_GPRS_STORAGE_ALL;

    apn_idx_arr = MMIAPICONNECTION_GetApnTypeIndex(sim_sys);
    if (apn_idx_arr == NULL)
    {
        AI_NET_TRACE("AI NET ConnectPDP [APN IDX ERROR]");
        return FALSE;
    }
    AI_NET_TRACE("AI NET  ConnectPDP [sim = %d, Net Setting = %d]", sim_sys, apn_idx_arr->internet_index[sim_sys].index);
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, apn_idx_arr->internet_index[sim_sys].index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        active_info.apn_ptr = (char*)linksetting->apn;
        active_info.user_name_ptr = (char*)linksetting->username;
        active_info.psw_ptr = (char*)linksetting->password;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type = linksetting->ip_type;
#endif
        if(MMIAPIPDP_Active(&active_info))
        {
            AI_NET_TRACE("AI NET PdpActive GPRS sim_sys=%d",sim_sys);
            AI_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
    }
    return return_val;
#endif
    return FALSE;
}
#else
LOCAL BOOLEAN  AI_Net_PDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    
    active_info.app_handler         = MMI_MODULE_TULINGAI;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = AI_Net_GetApnStr(sim_sys);
    active_info.user_name_ptr       = AI_Net_GetUserNameStr(sim_sys);
    active_info.psw_ptr             = AI_Net_GetPasswordStr(sim_sys);
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = AI_Net_HandlePdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;

    if(MMIAPIPDP_Active(&active_info))
    {
        AI_NET_TRACE("AI NET PdpActive GPRS sim_sys=%d",sim_sys);
        AI_Net_StartPdpActiveTimer();
        return_val = TRUE;
    }
    return return_val;
}
#endif

LOCAL BOOLEAN AI_Net_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    AI_NET_TRACE("AI NET PdpActive Start is_activing=%d,is_ok=%d",s_ai_net_is_activing,s_ai_net_pdp_state_is_ok);
    if(s_ai_net_is_activing)
    {
        s_ai_net_need_active_close = FALSE;
        return TRUE;
    }
    s_ai_net_is_activing = TRUE;
    
	#if 0
    if(s_ai_net_id == 0 && MMIZDT_Net_GetNetID() == 0)
    {
        s_ai_net_pdp_state_is_ok =  FALSE;
    }
    #endif
	
    if(s_ai_net_pdp_state_is_ok)
    {
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
        return TRUE;
    }

#if 0 //def WIFI_SUPPORT    
    //优先wifi
    if(s_ai_net_set_sys == MN_DUAL_SYS_MAX && MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
    {
        active_info.app_handler         = MMI_MODULE_TULINGAI;
        active_info.handle_msg_callback = AI_Net_HandlePdpMsg;
        active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
        if(MMIAPIPDP_Active(&active_info))
        {
            AI_NET_TRACE("AI NET PdpActive OK WIFI");
            AI_Net_StartPdpActiveTimer();
            return_val = TRUE;
        }
        else
        {
            AI_NET_TRACE("AI NET PdpActive ERR WIFI");
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
    }
    else
#endif
    {
        uint32 sim_ok_num = 0;
        uint16 sim_ok = 0;
        
        sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
        if(0 == sim_ok_num)
        {
            AI_NET_TRACE("AI NET PdpActive ERR NO SIM");
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }
        
        if(s_ai_net_is_init == FALSE)
        {
            AI_NET_TRACE("AI NET PdpActive ERR NO INIT");
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            return FALSE;
        }


        if(s_ai_net_set_sys < MN_DUAL_SYS_MAX)
        {
            if(MMIAPIPHONE_IsSimAvailable(s_ai_net_set_sys))
            {
                s_ai_net_cur_sys = s_ai_net_set_sys;
            }
            else
            {
                AI_NET_TRACE("AI NET PdpActive ERR set_sys(%d) NO SIM",s_ai_net_set_sys);
                MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
                return FALSE;
            }
        }
        else
        {
            s_ai_net_cur_sys = sim_ok;
        }

        return_val = AI_Net_PDPTry(s_ai_net_cur_sys);//第一张有效卡
        if(FALSE == return_val)
        {
            AI_NET_TRACE("AI NET PdpActive ERR cur_sys(%d/%d),",s_ai_net_cur_sys,sim_ok_num);
            #if 0
            if(sim_ok_num > 1 && s_ai_net_set_sys == MN_DUAL_SYS_MAX)
            {
                if(0 != s_ai_net_pdp_retry_timer_id)
                {
                    MMK_StopTimer(s_ai_net_pdp_retry_timer_id);
                    s_ai_net_pdp_retry_timer_id = 0;
                }
                s_ai_net_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                                                                AI_PDP_RETRY_TIMEOUT, 
                                                                                                AI_Net_HandlePdpRetryTimer, 
                                                                                                PNULL, 
                                                                                                FALSE
                                                                                                );
            }
            else
            #endif
            {
                MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_FAIL,PNULL,0);
            }
        }
    }

    return return_val;
}

/*****************************************************************************/
//  Description : Deactive Pdp
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AI_Net_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;
    
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_TULINGAI);

    return return_val;
}

LOCAL BOOLEAN AI_PDP_Net_Init(void)
{
    MN_DUAL_SYS_E dual_sys = MMIAI_Net_GetActiveSys();
    AI_NET_TRACE("AI_Net_Init is_init=%d, dual_sys=%d",s_ai_net_is_init,dual_sys);
    if(s_ai_net_is_init == FALSE)
    {
        MMIZDT_Get_SIM_IMSI(dual_sys,g_ai_sim_imsi);
        MMIZDT_Get_SIM_ICCID(dual_sys,g_ai_sim_iccid);
        AI_NET_TRACE("AI_Net_Init imei=%s,imsi=%s,iccid=%s",g_ai_phone_imei,g_ai_sim_imsi,g_ai_sim_iccid);

        s_ai_net_is_init = TRUE;
    }
    return TRUE;
}

// to active pdp ,connect to  pdp network
LOCAL BOOLEAN AI_PDP_Net_Open(void)
{
    AI_NET_TRACE("AI_Net_Open state=%d,is_activing=%d,is_closing=%d",s_ai_net_pdp_state_is_ok,s_ai_net_is_activing,s_ai_net_is_closing);
    if(s_ai_net_is_closing)
    {
        s_ai_net_need_close_active = TRUE;
    }
    else
    {
        #if 0
        s_ai_net_id = MMIZDT_Net_GetNetID();
        AI_NET_TRACE("AI_Net_Open s_ai_net_id=%d ", s_ai_net_id);
        if( s_ai_net_id > 0)  //zdt already active pdp and get net id
        {
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_ACTIVE_SUCCESS,PNULL,0);
            return TRUE;
        }
        #endif
        AI_Net_PdpActive();
    }
    return TRUE;
}

LOCAL BOOLEAN AI_PDP_Net_CloseEx(void)
{
    if(s_ai_net_is_closing)
    {
        s_ai_net_need_close_active = FALSE;
        return TRUE;
    }
    s_ai_net_is_closing = TRUE;
    if(s_ai_net_pdp_state_is_ok)
    {
        if(FALSE == AI_Net_PdpDeactive())
        {
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
        }
    }
    else
    {
        AI_Net_PdpDeactive();
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_CONNET_FAIL,PNULL,0);
    }
}

LOCAL BOOLEAN AI_PDP_Net_Close(void)
{
    AI_NET_TRACE("AI_Net_Close state=%d,is_activing=%d,is_closing=%d",s_ai_net_pdp_state_is_ok,s_ai_net_is_activing,s_ai_net_is_closing);
    if(s_ai_net_is_activing)
    {
        s_ai_net_need_active_close = TRUE;
    }
    else
    {
        AI_PDP_Net_CloseEx();
    }
    return TRUE;
}

LOCAL BOOLEAN AI_PDP_Net_Reset(void)
{
    AI_NET_TRACE("AI_Net_Reset state=%d",s_ai_net_pdp_state_is_ok);
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_CLOSE,PNULL,0);
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

LOCAL BOOLEAN AI_Net_PdpStateChange(BOOLEAN is_ok)
{
    AI_NET_TRACE("AI NET PDPStateChange %d",is_ok);
    return TRUE;
}

LOCAL void AI_Net_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    AI_NET_TRACE("AI_Net_PdpStateUpdate is_ok=%d,is_active=%d,activing=%d,closing=%d",is_ok,is_active,s_ai_net_is_activing,s_ai_net_is_closing);
    if(s_ai_net_pdp_state_is_ok != is_ok)
    {
        s_ai_net_pdp_state_is_ok = is_ok;
        AI_Net_PdpStateChange(is_ok);
    }
    
    if(is_active)
    {
        if(s_ai_net_is_activing)
        {
            s_ai_net_is_activing = FALSE;
            if(s_ai_net_need_active_close)
            {
                AI_NET_TRACE("AI_Net_PdpStateUpdate Active Close");
                s_ai_net_need_active_close = FALSE;
                MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_CLOSE,PNULL,0);
            }
        }
    }
    else
    {   
        s_ai_net_is_activing = FALSE;
        if(s_ai_net_is_closing)
        {
            s_ai_net_is_closing = FALSE;
            if(s_ai_net_need_close_active)
            {
                AI_NET_TRACE("AI_Net_PdpStateUpdate Close Active");
                s_ai_net_need_close_active = FALSE;
                MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_OPEN,PNULL,0);
            }
        }
    }
    return;
}

PUBLIC uint32 MMIAI_Net_GetNetID(void)
{
    return s_ai_net_id;
}

PUBLIC BOOLEAN MMIAI_Net_PdpStateGet(void)
{
    return s_ai_net_pdp_state_is_ok;
}

PUBLIC BOOLEAN MMIAI_Net_IsActiving(void)
{
    return s_ai_net_is_activing;
}

PUBLIC BOOLEAN MMIAI_Net_IsClosing(void)
{
    return s_ai_net_is_closing;
}


PUBLIC BOOLEAN MMIAI_Net_IsInit(void)
{
    return s_ai_net_is_init;
}

//send msg  ,to init open pdp ,
PUBLIC BOOLEAN MMIAI_Net_Init(void)
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_INIT,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIAI_Http_Request_SendMsg(void)
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_HTTP_REQUEST,PNULL,0);
    return TRUE;
}


PUBLIC BOOLEAN MMIAI_Net_Reset(void)
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_RESET,PNULL,0);
    return TRUE;
}

//send msg  ,to init open pdp ,
PUBLIC BOOLEAN MMIAI_Net_Open(void)
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_OPEN,PNULL,0);
    return TRUE;
}

PUBLIC BOOLEAN MMIAI_Net_Close(void)
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_NET_CLOSE,PNULL,0);
    return TRUE;
}

LOCAL MN_DUAL_SYS_E MMIAI_Net_NV_GetActiveSys(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    uint8 net_sys = AI_PDP_DEFAULT_SIM_SYS;
    //get if is flip answer
    /*MMINV_READ(MMI_ZDT_NV_NET_SYS,&net_sys,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //自动识别
        net_sys = AI_PDP_DEFAULT_SIM_SYS;
        MMINV_WRITE(MMI_ZDT_NV_NET_SYS,&net_sys);
    }*/
    s_ai_net_set_sys = net_sys;
    return net_sys;
}

LOCAL BOOLEAN  MMIAI_Net_NV_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    /*uint8 net_sys = set_sys;
    if(s_ai_net_set_sys != set_sys)
    {
        MMINV_WRITE(MMI_ZDT_NV_NET_SYS,&net_sys);
        s_ai_net_set_sys = set_sys;
        return TRUE;
    }
    return FALSE;*/
    return TRUE;
}

PUBLIC MN_DUAL_SYS_E MMIAI_Net_GetActiveSys(void)
{
    return s_ai_net_set_sys;
}

PUBLIC void MMIAI_Net_SetActiveSys(MN_DUAL_SYS_E set_sys) 
{
    if(MMIAI_Net_NV_SetActiveSys(set_sys))
    {
        if(s_ai_net_pdp_state_is_ok)
        {
            if(set_sys < MN_DUAL_SYS_MAX && s_ai_net_cur_sys != set_sys)
            {
                MMIAI_Net_Reset();
            }
        }
        else
        {
            if(s_ai_net_is_activing)
            {
                 if(set_sys < MN_DUAL_SYS_MAX && s_ai_net_cur_sys != set_sys)
                 {
                    MMIAI_Net_Reset();
                 }
            }
        }
    }
    return;
}

LOCAL BOOLEAN  MMIAI_Net_Handle_Init(DPARAM param)
{
    AI_NET_TRACE("MMIAI_Net_Handle_Init");
    AI_PDP_Net_Init();
    return TRUE;
}

LOCAL BOOLEAN  MMIAI_Net_Handle_Open(DPARAM param)
{
    AI_NET_TRACE("MMIAI_Net_Handle_Open");
    AI_PDP_Net_Open();
    return TRUE;
}

LOCAL BOOLEAN  MMIAI_Net_Handle_Close(DPARAM param)
{
    AI_NET_TRACE("MMIAI_Net_Handle_Close");
    AI_PDP_Net_Close();
    return TRUE;
}

LOCAL BOOLEAN  MMIAI_Net_Handle_Reset(DPARAM param)
{
    AI_NET_TRACE("MMIAI_Net_Handle_Reset");
    AI_PDP_Net_Reset();
    return TRUE;
}

LOCAL BOOLEAN  MMIAI_Net_Handle_PDPActiveOK(DPARAM param)
{
    AI_NET_TRACE("MMIAI_Net_Handle_PDPActiveOK net_is_activing=%d",s_ai_net_is_activing);
    AI_Net_PdpStateUpdate(TRUE,TRUE);

    MMIAI_TCP_Handle_PDPActiveOK();
    return TRUE;
}

LOCAL BOOLEAN  MMIAI_Net_Handle_PDPActiveFail(DPARAM param)
{
    AI_NET_TRACE("MMIAI_Net_Handle_PDPActiveFail net_is_activing=%d",s_ai_net_is_activing);
    AI_Net_PdpStateUpdate(FALSE,TRUE);

    MMIAI_TCP_Handle_PDPActiveFail();
    return TRUE;
}

LOCAL BOOLEAN  MMIAI_Net_Handle_PDPRespond(DPARAM param)
{
    AI_NET_TRACE("AI NET Handle_PDP_RSP net_is_activing=%d",s_ai_net_is_activing);
    AI_Net_PdpStateUpdate(FALSE,FALSE);

    MMIAI_TCP_Handle_PDPRespond();
    return TRUE;
}


PUBLIC MMI_RESULT_E  MMIAI_HTTP_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    AI_LOG("AI__NET MMIAI_HTTP_Handle_AppMsg ");
    switch(msg_id)
    {    
        case AI_APP_SIGNAL_HTTP_REQUEST:
            AI_LOG("AI__NET AI_APP_SIGNAL_HTTP_REQUEST ");
            MMIAI_Http_Init_Request();
            return MMI_RESULT_TRUE;
            
        
         default:
                res = MMI_RESULT_FALSE;
            break;
    }
    
    return res;

}



PUBLIC MMI_RESULT_E MMIAI_Net_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    AI_LOG("AI__NET MMIAI_Net_Handle_AppMsg ");

    switch(msg_id)
    {
    case AI_APP_SIGNAL_NET_INIT:
        AI_LOG("AI__NET AI_APP_SIGNAL_NET_INIT "); 
        MMIAI_Net_Handle_Init(param);  //init or open pdp
        break;
        
    case AI_APP_SIGNAL_NET_OPEN:
        AI_LOG("AI__NET AI_APP_SIGNAL_NET_OPEN ");    
        MMIAI_Net_Handle_Open(param);   // open pdp
        break;
    case AI_APP_SIGNAL_NET_CLOSE:
        AI_LOG("AI__NET AI_APP_SIGNAL_NET_CLOSE ");
        MMIAI_Net_Handle_Close(param);  // detach pdp
        break;
        
    case AI_APP_SIGNAL_NET_RESET:
        AI_LOG("AI__NET AI_APP_SIGNAL_NET_RESET ");
        MMIAI_Net_Handle_Reset(param);  //pdp reset
        break;
        
    case AI_APP_SIGNAL_NET_ACTIVE_SUCCESS:
        AI_LOG("AI__NET AI_APP_SIGNAL_NET_ACTIVE_SUCCESS ");
        MMIAI_Net_Handle_PDPActiveOK(param);
         AI_Net_Init(); // init url ip task ,should start socket thread here now
        break;
        
    case AI_APP_SIGNAL_NET_ACTIVE_FAIL:
        AI_LOG("AI__NET AI_APP_SIGNAL_NET_ACTIVE_FAIL "); 
        MMIAI_Net_Handle_PDPActiveFail(param);
        break;
        
    case AI_APP_SIGNAL_NET_CONNET_FAIL:
        AI_LOG("AI__NET AI_APP_SIGNAL_NET_CONNET_FAIL "); 
        MMIAI_Net_Handle_PDPRespond(param);
        

    
    default:
            res = MMI_RESULT_FALSE;
        break;
    }

   
    if(res == MMI_RESULT_FALSE)
    {
        res = MMIAI_TCP_Handle_AppMsg(app_ptr,msg_id,param);
    }
    if(res == MMI_RESULT_FALSE)
    {
        res = MMIAI_HTTP_Handle_AppMsg(app_ptr,msg_id,param);
       
    }
    
    return res;
}



#endif
