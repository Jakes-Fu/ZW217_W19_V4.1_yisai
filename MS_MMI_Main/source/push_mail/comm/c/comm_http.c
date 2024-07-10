/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_http.c
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-1  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-1      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#include "mmi_push_mail.h"
#include "std_header.h"

#include "comm_http.h"
#include "comm_encode.h"
#include "comm_platform.h"
#include "mail_util.h"

static comm_network_list_t comm_net_list = {0};




/*==========================================================
 * Function 	: comm_NetInit
 * Author		: sunguochen      
 * Description : net 部分初始化
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetInit(comm_net_ptr_t* net_ptr)
{
    if(!net_ptr)
    {
        return FALSE;
    }
    
    return TRUE;
}


/*==========================================================
 * Function 	: comm_NetFinal
 * Author		: sunguochen      
 * Description : net部分结束处理
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetFinal(comm_net_ptr_t* net_ptr)
{
    if(!net_ptr)
    {
        return FALSE;
    }
    
    return TRUE;
}


/*==========================================================
 * Function 	: comm_NetGetList
 * Author		: sunguochen      
 * Description : 获取保存的网络参数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/

comm_network_list_t* comm_NetGetList(void)
{
    return &comm_net_list;
}



/*==========================================================
 * Function 	: comm_StartDownload
 * Author		: sunguochen      
 * Description : 开始下载
 * Parameter	: type          parameter       Description
                      comm_net_ptr_t* net_ptr :网络参数 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetStartDownload(comm_download_ptr_t* net_ptr)
{

    comm_network_list_t* net_list = NULL;

    if(!net_ptr)
    {
        return FALSE;
    }

    net_list = comm_NetGetList();
    if(!net_list)
    {
        return FALSE;
    }

// TODO: 更新下载列表 ,需要列表操作同步进行/*-sunguochen 2010.4.1 -*/
    //目前做测试代码
    net_list->download_list.download_list = net_ptr;
    net_list->download_list.cur_down_ptr = net_ptr;
    net_list->download_list.download_num = 1;
    net_list->download_list.is_downloading = FALSE;


    //列表为空，之前无下载队列，直接开始下载
    if(!net_list->download_list.download_num || !net_list->download_list.download_list)
    {
        //列表空
    }

    //未开始下载，直接进行下载
    if(!net_list->download_list.is_downloading)
    {
        //解析生成请求命令
        comm_NetComposeDownloadCmd(net_list->download_list.cur_down_ptr);

        //开始命令
    }
    else
    {
        //已经开始下载，直接退出即可
    }

    return TRUE;

}


/*==========================================================
 * Function 	: comm_StartServiceCmd
 * Author		: sunguochen      
 * Description : 开始网络交互
 * Parameter	: type          parameter       Description
                      comm_net_ptr_t* net_ptr :网络参数 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetStartServiceCmd(comm_service_ptr_t* service_ptr)
{
    return TRUE;
}


/*==========================================================
 * Function 	: comm_StartProgressive
 * Author		: sunguochen      
 * Description : 开始在线听歌的渐进下载
 * Parameter	: type          parameter       Description
                      comm_net_ptr_t* net_ptr :网络参数 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetStartProgressive(comm_net_ptr_t* net_ptr)
{
    return TRUE;
}


/*==========================================================
 * Function 	: comm_StartServiceCmd
 * Author		: sunguochen      
 * Description : 开始网络交互- post
 * Parameter	: type          parameter       Description
                      comm_net_ptr_t* net_ptr :网络参数 
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetPostNetCmd(comm_net_ptr_t* net_ptr)
{
    return TRUE;
}



/*==========================================================
 * Function 	: comm_NetComposeCmd
 * Author		: sunguochen      
 * Description : 组成网络交互命令
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
char* comm_NetComposeCmd(COMM_NETWORK_TYPE_E type, comm_network_list_t* net_list_ptr)
{

    switch(type)
    {
        case COMM_NET_TYPE_PROGRESSIVE:

            break;

        case COMM_NET_TYPE_SERVICE_CMD:

            break;

        case COMM_NET_TYPE_DOWNLOAD:

            break;

        default:

            break;            

    }

    return NULL;

}
   

/*==========================================================
 * Function 	: comm_NetComposeCmd
 * Author		: sunguochen      
 * Description : 组成网络交互命令
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetComposeDownloadCmd(comm_net_ptr_t* down_ptr)
{

    char* randkey_ptr = NULL;
    comm_uint32 randkey_len = 0;
    
    char* host_ptr = NULL;
    comm_uint32 host_len = 0;

    char* download_cmd = NULL;
    comm_uint32 cmd_len = 0;

    char time_step[COMM_TIMESTEP_LEN + 1]={0};

    if(!down_ptr || !down_ptr->url_ptr)
    {
        return FALSE;
    }

    download_cmd = comm_alloc(COMM_REQUEST_MAX_LEN + 1);

    if(!download_cmd)
    {
        return FALSE;
    }

    //获取时间戳
    comm_GetSysTimeStep((char *)(&time_step));

    memset(download_cmd, 0, (COMM_REQUEST_MAX_LEN + 1));

    //命令行开始
    //GET
    strcat(download_cmd, (char *)COMM_NET_KEY_GET);
    //space
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);
    //url
    strcat(download_cmd, (char *)down_ptr->url_ptr);
    //?
    strcat(download_cmd, (char *)COMM_NET_KEY_QUES);
    //ua
    strcat(download_cmd, (char *)COMM_NET_CMD_UA);
    //=
    strcat(download_cmd, (char *)COMM_NET_KEY_EQUAL);
    //local mobile ua
    strcat(download_cmd, (char *)COMM_NET_LOCAL_UA);
    //space
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);
    //HTTP/1.1
    strcat(download_cmd, (char *)COMM_NET_KEY_HTTP);
    //crlf
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    //命令行结束

    //following is create comand header
    //Header Begin
    
    //Header --TimeStep
    strcat(download_cmd, (char *)COMM_NET_KEY_TIME);
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);
    strcat(download_cmd, time_step); 
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    //Header --mode
    strcat(download_cmd, (char *)COMM_NET_KEY_MODE);
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);
    strcat(download_cmd, (char *)COMM_NET_LOCAL_MODE);
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    //Header --Randkey
    strcat(download_cmd, (char *)COMM_NET_KEY_RANDKEY);
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);
    
    comm_CreateRandkey((char *)(&time_step), &randkey_ptr, &randkey_len);
    strcat(download_cmd, randkey_ptr);
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    //Header --range
    if((down_ptr->start_pos > 0) || (down_ptr->end_pos < down_ptr->content_size))
    {
        //do not get full file
        //command :Range: bytes=??-???
        uint32 start_pos = 0;
        uint32 end_pos= 0;

        start_pos = down_ptr->start_pos;
        end_pos = COMM_MIN(down_ptr->end_pos, down_ptr->content_size);

        strcat(download_cmd, (char *)COMM_NET_KEY_RANGE);
        strcat(download_cmd, (char *)COMM_NET_KEY_EQUAL);

        cmd_len = strlen(download_cmd);        
        sprintf(download_cmd+cmd_len, "%ld", start_pos);
        
        strcat(download_cmd, (char *)COMM_NET_KEY_TO);
        
        cmd_len = strlen(download_cmd);
        sprintf(download_cmd+cmd_len, "%ld", end_pos);
        strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    }

    //Header --User-Agent
    strcat(download_cmd, (char *)COMM_NET_KEY_UA);
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);
    strcat(download_cmd, (char *)comm_GetUserAgent());
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    //Header --Content-length
    strcat(download_cmd, (char *)COMM_NET_KEY_CLENTH);
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);    
    cmd_len = strlen(download_cmd);
    sprintf(download_cmd+cmd_len, "%ld", down_ptr->content_size);
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    //Header --Host    
    strcat(download_cmd, (char *)COMM_NET_KEY_HOST);
    strcat(download_cmd, (char *)COMM_NET_KEY_SPACE);    
    comm_SockGetHostByUrl(down_ptr->url_ptr, &host_ptr, &host_len);
    
    sprintf(download_cmd+cmd_len, "%s", host_ptr);
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    //Header --Terminate indicator 
    strcat(download_cmd, (char *)COMM_NET_KEY_CRLF);

    cmd_len = strlen(download_cmd);

    if(cmd_len > COMM_REQUEST_MAX_LEN)
    {
        //cmd is to long
        //COMM_TRACE:"Mail:comm_NetProxyRespResult cmd is to long1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_HTTP_356_112_2_18_3_26_7_1,(uint8*)"");
        return FALSE;
    }

    //Header end

    //following is request content

    if(down_ptr->content_size && down_ptr->content_ptr)
    {
        cmd_len = strlen(download_cmd);

        if((cmd_len + down_ptr->content_size) > COMM_REQUEST_MAX_LEN)
        {
            //cmd is to long
            //COMM_TRACE:"Mail:comm_NetProxyRespResult cmd is to long2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_HTTP_371_112_2_18_3_26_7_2,(uint8*)"");
            return FALSE;
        }

        memcpy(download_cmd + cmd_len, down_ptr->content_ptr, down_ptr->content_size);

    }
    

    if(cmd_len > COMM_REQUEST_MAX_LEN)
    {
        //cmd is to long
        //COMM_TRACE:"Mail:comm_NetProxyRespResult cmd is to long3"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_HTTP_383_112_2_18_3_26_7_3,(uint8*)"");
        return FALSE;
    }

    //free memory
    comm_free(randkey_ptr); //lint !e774

    comm_free(host_ptr);

    down_ptr->cmd_request = download_cmd;

    return TRUE;

}



/*==========================================================
 * Function 	: comm_NetComposeProxyCmd
 * Author		: sunguochen      
 * Description : 在已连接代理的情况下, 连接服务器
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
char* comm_NetComposeProxyCmd(comm_sockaddr* server_addr)
{

    char * proxy_cmd = NULL;
    int proxy_cmd_len = 0;
    
    if(!server_addr)
    {
        return NULL;
    }
    
    proxy_cmd = comm_alloc((COMM_REQUEST_MAX_LEN+1));
    
    memset(proxy_cmd, 0, (COMM_REQUEST_MAX_LEN +1));
    
    //cmd: CONNECT *.*.*.*:* http/1.1
    strcat(proxy_cmd, (char *)COMM_NET_KEY_CONNECT);
    
    strcat(proxy_cmd, (char *)COMM_NET_KEY_SPACE);

#ifdef WIN32
    strcat(proxy_cmd, "10.0.0.172");
#else
    strcat(proxy_cmd, inet_ntoa(server_addr->ip_addr));
#endif

    strcat(proxy_cmd, (char *)COMM_NET_KEY_COLON);
    
    proxy_cmd_len = strlen(proxy_cmd);

    sprintf(proxy_cmd + proxy_cmd_len, "%d", server_addr->port);  
    
    strcat(proxy_cmd, COMM_NET_KEY_2SPACE COMM_NET_KEY_HTTPL COMM_NET_KEY_CRLF COMM_NET_KEY_CRLF);
    
    //strcat(proxy_cmd, (char *)COMM_NET_KEY_HTTPL);

    //strcat(proxy_cmd, (char *)COMM_NET_KEY_CRLF);
    //strcat(proxy_cmd, (char *)COMM_NET_KEY_CRLF);    
    
    
    return proxy_cmd;

}



/*==========================================================
 * Function 	: comm_NetProxyRespResult
 * Author		: sunguochen      
 * Description : 判断代理服务器是否就绪
                      如果有http/1.x 200-成功
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/8/2010 
 ==========================================================*/
comm_boolean comm_NetProxyRespResult(char * buffer_ptr, char **connect_result)
{

    char * buf_proxy = NULL;

    if(!buffer_ptr)
    {
        return FALSE;
    }

    buf_proxy = buffer_ptr;

    //去掉空格
    while(*buf_proxy == 0x20)
    {
        buf_proxy++;
    }

    buf_proxy = mail_uCase(buf_proxy, strlen(buf_proxy));

    //与http1.比较
    if(memcmp(buf_proxy, (char *)COMM_NET_KEY_HTTPL, strlen((char *)COMM_NET_KEY_HTTP)-1))
    {
        //COMM_TRACE:"Mail:comm_NetProxyRespResult not exit http"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_HTTP_486_112_2_18_3_26_7_4,(uint8*)"");
        return FALSE;
    }

    if(strstr(buf_proxy, (char *)COMM_NET_PROXY_OK))
    {
        char *result = buffer_ptr;
        //COMM_TRACE:"Mail:comm_NetProxyRespResult is ok 200"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_HTTP_493_112_2_18_3_26_7_5,(uint8*)"");
        while(result = strstr(result, COMM_NET_KEY_CRLF))
        {
            result += strlen(COMM_NET_KEY_CRLF);
            if((*result != 0) && (*result != '\r') && (*(result+1) != 0) && (*(result+1) != '\n'))
            {
                char *result_tmp = NULL;
                //去掉空格
                while(*result == 0x20)
                {
                    result++;
                }
                result_tmp = comm_alloc(strlen(result) + 2);
                strcpy(result_tmp, result);
                *connect_result = result_tmp;
                //COMM_TRACE:"Mail:comm_NetProxyRespResult connect result = %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_HTTP_508_112_2_18_3_26_7_6,(uint8*)"s", result_tmp);
                break;
            }
        }        
        return TRUE;
    }
    else
    {
        //COMM_TRACE:"Mail:comm_NetProxyRespResult not exit 200"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_HTTP_516_112_2_18_3_26_7_7,(uint8*)"");
        return FALSE;
    }
        
}




