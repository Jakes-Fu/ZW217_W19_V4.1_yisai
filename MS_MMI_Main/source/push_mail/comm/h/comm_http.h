/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_http.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-1  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-1      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _COMM_HTTP_H_
#define _COMM_HTTP_H_

#include "comm_platform.h"
#include "comm_interface.h"
#include "comm_socket.h"

#define COMM_REQUEST_MAX_LEN     1024

#define COMM_NET_KEY_GET              "GET"
#define COMM_NET_KEY_POST            "POST"
#define COMM_NET_KEY_CONNECT      "CONNECT"
#define COMM_NET_KEY_HTTP            "http/1.1"
#define COMM_NET_KEY_HTTPL           "HTTP/1.0"

#define COMM_NET_PROXY_OK            "200"

#define COMM_NET_KEY_CRLF            "\r\n"

#define COMM_NET_KEY_SPACE          " "
#define COMM_NET_KEY_2SPACE        "  "
#define COMM_NET_KEY_COLON         ":"
#define COMM_NET_KEY_EQUAL         "="
#define COMM_NET_KEY_QUES           "?"
#define COMM_NET_KEY_AND             "&"
#define COMM_NET_KEY_TO               "-"

//used by command header
#define COMM_NET_KEY_RANGE           "range: bytes"
#define COMM_NET_KEY_UA                 "User-Agent:"
#define COMM_NET_KEY_MODE            "mode:"
#define COMM_NET_KEY_TIME             "TimeStep:"
#define COMM_NET_KEY_RANDKEY       "Randkey:"
#define COMM_NET_KEY_CLENTH         "Content-Length:"
#define COMM_NET_KEY_HOST             "Host:"


//used by request command
#define COMM_NET_CMD_UA                        "ua"
#define COMM_NET_CMD_FLAT                     "flat"
#define COMM_NET_CMD_VERSION               "version"
#define COMM_NET_CMD_GROUPCODE         "groupcode"
#define COMM_NET_CMD_PAGENO                "pageno"
#define COMM_NET_CMD_ITEMCOUNT          "itemcount"
#define COMM_NET_CMD_CONTENTID           "contentid"
#define COMM_NET_CMD_TITLE                    "title"
#define COMM_NET_CMD_SEARCHTYPE         "searchtype"
#define COMM_NET_CMD_NAME                    "name"
#define COMM_NET_CMD_SEND                     "send"
#define COMM_NET_CMD_TYPE                     "type"
#define COMM_NET_CMD_MDN                      "mdn"
#define COMM_NET_CMD_ATTACHID             "attachid"
#define COMM_NET_CMD_SIZE                      "size"
#define COMM_NET_CMD_PID                       "pid"

#define COMM_NET_LOCAL_UA                      "hisense-hs-n51_cmcc"
#define COMM_NET_LOCAL_MODE                 "chinamobile"
//网络操作类型
typedef enum
{
    COMM_NET_TYPE_NONE,    
    COMM_NET_TYPE_PROGRESSIVE, //在线听歌
    COMM_NET_TYPE_SERVICE_CMD, //服务器交互
    COMM_NET_TYPE_DOWNLOAD,    //下载
    COMM_NET_TYPE_MAX

}COMM_NETWORK_TYPE_E;


//网络操作步骤
typedef enum
{
    COMM_NET_STEP_NONE,
    COMM_NET_STEP_START,
    COMM_NET_STEP_SEND,
    COMM_NET_STEP_RECV,
    COMM_NET_STEP_END,
    COMM_NET_STEP_STOP,
    COMM_NET_STEP_ERR,    
    COMM_NET_STEP_MAX

}COMM_NETWORK_STEP_E;


//网络操作内容
typedef struct _comm_net_ptr
{
    char*                                  url_ptr;
    comm_wchar*                      full_name_ptr;
    comm_uint32                       start_pos;
    comm_uint32                       end_pos;
    comm_uint32                       cur_pos;
    char*                                  content_ptr;
    comm_uint32                       content_size;
    COMM_NETWORK_STEP_E     net_step;
    comm_boolean                     is_background;
    char*                                  cmd_request;    
    comm_uint32                       cmd_request_len;

}comm_net_ptr_t;


//网络操作内容
typedef struct _comm_service_ptr
{
    char*                                  url_ptr;
    char*                                  cmd_ua;
    char*                                  cmd_flat;
    char*                                  cmd_version;
    char*                                  cmd_groupcode;
    char*                                  cmd_itemcount;
    char*                                  cmd_pageno;
    char*                                  cmd_contentid;
    char*                                  cmd_attachid;
    char*                                  cmd_name;
    char*                                  cmd_title;            //used by search
    char*                                  cmd_searchtype; //used by search
    char*                                  cmd_send;
    char*                                  cmd_mdn;
    char*                                  cmd_size;
    char*                                  cmd_type;          //used by 5.1.9    
    char*                                  cmd_pid;          //used by 5.1.9   

    char*                                  cmd_request;
    comm_uint32                       cmd_request_len;
    char*                                  content_ptr;
    comm_uint32                       content_size;    
    COMM_NETWORK_STEP_E     net_step;
    comm_boolean                     is_background;

}comm_service_ptr_t;


//网络下载
typedef struct _comm_download_ptr
{
    comm_net_ptr_t                        download_ptr;
    comm_uint32                            download_index;
    struct _comm_download_ptr*      next_ptr;

}comm_download_ptr_t;


//网络下载列表
typedef struct _comm_download_list
{
    comm_download_ptr_t*   download_list;
    comm_download_ptr_t*   cur_down_ptr;
    comm_uint32                 download_num;
    comm_boolean               is_downloading;

}comm_download_list_t;


//网络操作相关内容
typedef struct _comm_network_list
{
    comm_net_ptr_t*            progressive_download;//在线听歌     ----只允许一个
    comm_service_ptr_t*        service_cmd;               //服务器交互----只允许一个
    comm_download_list_t     download_list;             //下载列表      ---- 可以有多个
    
}comm_network_list_t;



/*==========================================================
 * Function 	: comm_NetInit
 * Author		: sunguochen      
 * Description : net 部分初始化
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetInit(comm_net_ptr_t* net_ptr);


/*==========================================================
 * Function 	: comm_NetFinal
 * Author		: sunguochen      
 * Description : net部分结束处理
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetFinal(comm_net_ptr_t* net_ptr);


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
comm_boolean comm_NetStartDownload(comm_download_ptr_t* net_ptr);


/*==========================================================
 * Function 	: comm_NetComposeCmd
 * Author		: sunguochen      
 * Description : 组成网络交互命令
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
char* comm_NetComposeCmd(COMM_NETWORK_TYPE_E type, comm_network_list_t* net_list_ptr);


/*==========================================================
 * Function 	: comm_NetComposeCmd
 * Author		: sunguochen      
 * Description : 组成网络交互命令
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
comm_boolean comm_NetComposeDownloadCmd(comm_net_ptr_t* down_ptr);



/*==========================================================
 * Function 	: comm_NetComposeProxyCmd
 * Author		: sunguochen      
 * Description : 在已连接代理的情况下, 连接服务器
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
char* comm_NetComposeProxyCmd(comm_sockaddr* server_addr);



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
comm_boolean comm_NetProxyRespResult(char * buffer_ptr, char **connect_result);


#endif

