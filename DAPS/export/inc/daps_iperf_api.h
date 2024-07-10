/*
  * daps_iperf.h - iperf external include
  *
  * Copyright (C) 2020 Unisoc Communications Inc.
  * History:
  *         2020-11-19: <ak.wang>
  *         create.
*/

#ifndef _DAPS_IPERF_API_H_
#define _DAPS_IPERF_API_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                                                              *
 **--------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_types.h"
#include "tcpip_types.h"

 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                                                             *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                                                    *
 **--------------------------------------------------------------------------*/
 #define IPERF_MAX_BUFFER_LEN            20
#define IPERF_PARAM_MAX_LEN            80

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                                                           */
/*---------------------------------------------------------------------------*/
/**  
  *  name: IPERF_ERROR_E 
  *  descriptor: function error 
  *  note:  
*/
typedef enum _IPERF_ERROR_E
{
    IPERF_SUCCESS,
    IPERF_INVALED_INPUT,
    IPERF_NO_MEMORY,
    IPERF_ERROR,
}IPERF_ERROR_E;

/**  
  *  name: IPERF_RESULT_T 
  *  descriptor:  create
  *  note: 
*/
typedef struct _IPERF_RESULT_T_ {
/*
    int32 data_size;
    int32 send_time;*/
    char total_len[IPERF_MAX_BUFFER_LEN];
    char result[IPERF_MAX_BUFFER_LEN];
    char report_title[IPERF_MAX_BUFFER_LEN];
    char interval_tilte[IPERF_MAX_BUFFER_LEN];
    uint32 r_link_id;
} IPERF_RESULT_T;

/**  
  *  name: IPERF_ROLE_TYPE_E 
  *  descriptor:  create
  *  note: 
*/
typedef enum _IPERF_ROLE_TYEP_E
{
    IPERF_ROLE_TCP_CLIENT,
    IPERF_ROLE_TCP_SERVER,
    IPERF_ROLE_UDP_CLIENT,
    IPERF_ROLE_UDP_SERVER,
    IPERF_ROLE_MAX,
}IPERF_ROLE_TYPE_E;


typedef void (* IPERF_CALLBACK_T)(IPERF_RESULT_T* result);

/**  
  *  name: IPERF_CONTEXT_T 
  *  descriptor:  create
  *  note: 
*/
typedef struct  _IPERF_CONFIG_T_ 
{
   TCPIP_NETID_T net_id;
   IPERF_ROLE_TYPE_E role_type;
   int win_size;
   int send_time;
   int server_addr;
   int server_port;
      
    //UDP
   int data_size;
   char  bw[IPERF_MAX_BUFFER_LEN];//band withed

   IPERF_CALLBACK_T callback;
   uint8 S_link_id;
   //muti-thread
   int parallel_stream;
   uint32 interval;
}IPERF_CONFIG_T;

typedef struct
{
    uint8        param_len;
    uint8        param[IPERF_PARAM_MAX_LEN];
} IPERF_PARAM_T;

typedef struct
{
    IPERF_PARAM_T ip;
    IPERF_PARAM_T port;
    IPERF_PARAM_T time;
    IPERF_PARAM_T bw;
    IPERF_PARAM_T parallel;
    IPERF_PARAM_T interval;
} IPERF_CONFIG_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                                                *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :daps_iperf_init
//  Global resource dependence :
//  Param:       
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
PUBLIC int daps_iperf_init(void);

/*****************************************************************************/
//  Description : daps_iperf_stop
//  Global resource dependence :
//  Param:       
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
PUBLIC int daps_iperf_stop(IPERF_ROLE_TYPE_E role_type);

/*****************************************************************************/
//  Description : daps_iperf_run
//  Global resource dependence :
//  Param:       
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
PUBLIC IPERF_ERROR_E daps_iperf_run(IPERF_CONFIG_T* config);


/*****************************************************************************/
//  Description : daps_iperf_set_TCP_Client
//  Global resource dependence :
//  Param:       
//  Author: xiameng.yang
//  Note: 
/*****************************************************************************/
PUBLIC IPERF_ERROR_E daps_iperf_set_TCP_Client(IPERF_CONFIG_T *config, char * ip,char * port,char * time,char * win_size,IPERF_CALLBACK_T result_callback );

/*****************************************************************************/
//  Description : daps_iperf_set_UDP_Server
//  Global resource dependence :
//  Param:       
//  Author: xiameng.yang
//  Note: 
/*****************************************************************************/
PUBLIC IPERF_ERROR_E daps_iperf_set_UDP_Server(IPERF_CONFIG_T *config,char * port,IPERF_CALLBACK_T result_callback);


/*****************************************************************************/
//  Description : daps_iperf_set_UDP_Client
//  Global resource dependence :
//  Param:       
//  Author: xiameng.yang
//  Note: 
/*****************************************************************************/
PUBLIC IPERF_ERROR_E daps_iperf_set_UDP_Client(IPERF_CONFIG_T *config, char * ip,char * port,char * time,char * data_size,char * bw,IPERF_CALLBACK_T result_callback);

/*****************************************************************************/
//  Description : daps_iperf_set_UDP_Server
//  Global resource dependence :
//  Param:       
//  Author: xiameng.yang
//  Note: 
/*****************************************************************************/
PUBLIC IPERF_ERROR_E daps_iperf_set_UDP_Server(IPERF_CONFIG_T *config,char * port,IPERF_CALLBACK_T result_callback);

/*****************************************************************************/
//  Description :iperf_WIFI_Set_netid
//  Global resource dependence :
//  Param:       
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
PUBLIC IPERF_ERROR_E daps_iperf_WIFI_Set_netid(IPERF_CONFIG_T* config);

/*****************************************************************************/
//  Description :daps_iperf_parser_at_command
//  Global resource dependence :
//  Param:       
//  Author: ak.wang
//  Note: 
//	TCPUL:	AT+WIFI= IPERF,UP,TCP,IP=xxx,PORT=xxx,TIME=xxx
//	UDPUP:	AT+WIFI= IPERF,UP,UDP,IP=xxx,PORT=xxx,TIME=xxx,BW=xxx
//	DL:		AT+WIFI=IPERF,DOWN,TCP/UDP,PORT=xxx
/*****************************************************************************/
PUBLIC IPERF_ERROR_E daps_iperf_parser_at_command(IPERF_CONFIG_T *config, unsigned int argc, char* argv[]);

/*****************************************************************************/
//  Description :daps_iperf_is_running
//  Global resource dependence :
//  Param:       
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
PUBLIC int  daps_iperf_is_running(void);

PUBLIC void result_callback (IPERF_RESULT_T* result);

PUBLIC IPERF_ERROR_E iperf_tcp_client(IPERF_CONFIG_PARAM_T* param, uint32 netid, uint8 S_link_id);
PUBLIC IPERF_ERROR_E iperf_tcp_server(IPERF_CONFIG_PARAM_T* param, uint32 netid, uint8 S_link_id);
PUBLIC IPERF_ERROR_E iperf_udp_client(IPERF_CONFIG_PARAM_T* param, uint32 netid, uint8 S_link_id);
PUBLIC IPERF_ERROR_E iperf_udp_server(IPERF_CONFIG_PARAM_T* param, uint32 netid, uint8 S_link_id);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                                                            *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

