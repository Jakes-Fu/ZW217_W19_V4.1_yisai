/****************************************************************************
** File Name:      rtp_api.h                                                *
** Author:         bruce.chi                                                *
** Date:           2008.4                                                   *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the RTP api                *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2008.4        alex.wang         Create                                   *
** 2008.5        bruce.chi         Modify                                   *
** 2010.1        kelly.li          Modify                                   *
****************************************************************************/
#ifndef _RTP_API_H
#define _RTP_API_H

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "cfl_list.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define RTP_MAX_IP_ADDR_STR_LEN          15     //IP address max string len
#define RTP_MAX_RTCP_APP_NAME_LEN		 4      //rtcp app packet name length

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef CFL_LIST_NODE_HANDLE  RTP_SESS_ID_T;                    //rtp session id
typedef uint16  RTP_PORT_T;                                     //rtp port type
typedef char    RTP_IP_ADDR_STR_T[RTP_MAX_IP_ADDR_STR_LEN + 1]; //rtp ip address type
typedef uint32  RTP_IP_ADDR_INT_T;                              //rtp ipv4
/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    RTP_SUCCESS,                //ok, no error
    RTP_ERROR_FAILED,           //fatal error
    RTP_ERROR_NO_MEMORY,        //input param error
    RTP_ERROR_INVALID_INPUT,    //no memory
    RTP_ERROR_MAX               //total number
} RTP_ERROR_E;
/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
typedef struct _RTP_SOCKADDR_PARAM
{         
    RTP_PORT_T          rtp_port;       //rtp port info  
    RTP_PORT_T          rtcp_port;      //rtcp port info        
    RTP_IP_ADDR_STR_T   ip_str;         //ip string info
}RTP_SOCKADDR_PARAM_T;

typedef struct _RTP_DATA_PARAM
{
    uint32          timestamp;              //timestamp: 32 bits
    uint32          payload_len;            //payload length
    uint8           *payload_buffer_ptr;    //payload buffer pointer   
    uint32          ssrc;                   //synchronization source (SSRC) :32 bits
    uint16          sequence_number;        //sequence number: 16 bits. Note: this param is ignored in RTP_SEND_REQ
    uint8           payload_type;           //payload type (PT): 7 bits
    BOOLEAN         marker;                 //marker (M): 1 bit 
}RTP_DATA_PARAM_T;                          //RTP data param. (Note: Not support Padding)

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef void (*RTP_RECEIVE_RTP_DATA_CALLBACK)(
							RTP_SESS_ID_T           session_id,         //[in]the rtp session id
							const RTP_DATA_PARAM_T  *rtp_data_param_ptr //[in]rtp data info
						   );

typedef struct _RTCP_APP_DATA_PARAM
{
    uint8           name[RTP_MAX_RTCP_APP_NAME_LEN];//name
    uint32          app_data_len;           //application data length. (Note: app_data_len  MUST be a multiple of 32 bits long. )
    uint8           *app_data_ptr;          //application data pointer
    uint8           sub_type;               //subtype: 5 bits
}RTCP_APP_DATA_PARAM_T;                     //Application-Defined RTCP Packet. (Note: Not support padding)

typedef struct _RTCP_SRRR_DATA_PARAM_
{
    uint32    fraction_lost;    //The fraction of RTP data packets that were lost since previous SR/RR packet was sent
    uint32    cumulative_lost;  //Total number of RTP data packets that have been lost since the beginning of reception
    uint16    max_seq_num;      //highest sequence number in the rtp data packet
    uint16    seq_cycles;       //count of sequence number cycles
    uint32    jitter;           //Estimate of the statistical variance of the RTP data packet inter arrival time. 
    uint32    lsr;              //The middle 32 bits of the NTP timestamp received
    uint32    dlsr;             //Delay since the last SR  
}RTCP_SRRR_DATA_PARAM_T;        //RTCP Packet SR/RR info

typedef struct _RTP_CREATE_SESSION_PARAM
{
    BOOLEAN						      is_need_srrr_send_ind;  //when srrr send mode is rtp auto send,if need send send_ind to app
    uint32                            net_id;                 //the network id
    RTP_RECEIVE_RTP_DATA_CALLBACK     rcv_rtp_callback;       //when receive new rtp data, call this function to inform app
}RTP_CREATE_SESSION_PARAM_T;                                  //the param for create new rtp session

typedef struct _RTP_CREATE_SESSION_RESULT
{
    RTP_SESS_ID_T        session_id;        //the id assigned for the session                       
    RTP_PORT_T           rtp_recv_port;     //the local port for rtp
    RTP_PORT_T           rtcp_recv_port;    //the local port for rtcp
}RTP_CREATE_SESSION_RESULT_T;               //the param for create session result

typedef struct _RTP_DATA_SEND_REQ_PARAM
{
    RTP_SESS_ID_T       session_id;         //rtp session id
    RTP_IP_ADDR_STR_T   remote_ip;          //remote ip address
    RTP_PORT_T          remote_rtp_port;    //remote rtp port
    RTP_PORT_T          remote_rtcp_port;   //remote rtcp port
    RTP_DATA_PARAM_T    rtp_data_param;     //rtp data param
}RTP_DATA_SEND_REQ_PARAM_T;                 //app set the send rtp param

typedef struct _RTCP_APP_DATA_SEND_REQ_PARAM
{
    RTP_SESS_ID_T           session_id;     //rtp session id
    RTP_IP_ADDR_STR_T       remote_ip;      //remote ip address
    RTP_PORT_T              remote_port;    //remote port
    RTCP_APP_DATA_PARAM_T   app_data_param; //Application-Defined RTCP Packet param 
}RTCP_APP_DATA_SEND_REQ_PARAM_T;            //Application-Defined RTCP Packet send request param 

typedef struct _RTCP_SRRR_SEND_REQ_PARAM
{
    RTP_SESS_ID_T           session_id;     //rtp session id
    RTP_IP_ADDR_STR_T       remote_ip;      //remote ip address
    RTP_PORT_T              remote_port;    //remote port
}RTCP_SRRR_SEND_REQ_PARAM_T;  

typedef struct _RTP_TERMINATE_SESSION_PARAM
{
    RTP_SESS_ID_T session_id ;              //rtp session id
}RTP_TERMINATE_SESSION_PARAM_T ;            //app set terminate session param

//the message body define begin
typedef struct _RTP_RTCP_SRRR_SEND_REQ_SIG
{
    _SIGNAL_VARS
    RTCP_SRRR_SEND_REQ_PARAM_T     send_rtcp_srrr_param; //app send srrr param
}RTP_RTCP_SRRR_SEND_REQ_SIG_T;

typedef struct _RTP_RTCP_SRRR_SEND_CNF_SIG
{
    _SIGNAL_VARS
    RTP_SESS_ID_T      session_id;          //rtp session id
    RTP_ERROR_E        result;              //rtp action result
}RTP_RTCP_SRRR_SEND_CNF_SIG_T;

typedef struct _RTP_RTCP_DATA_SEND_REQ_SIG
{
    _SIGNAL_VARS
    RTCP_APP_DATA_SEND_REQ_PARAM_T 	send_rtcp_app_param;    //App send app data param        
}RTP_RTCP_DATA_SEND_REQ_SIG_T;

typedef struct _RTP_RTCP_DATA_SEND_CNF_SIG
{
    _SIGNAL_VARS
    RTP_SESS_ID_T       session_id;         //rtp session id
    RTP_ERROR_E         result;             //rtp action result
}RTP_RTCP_DATA_SEND_CNF_SIG_T;

typedef struct _RTP_RTCP_SRRR_SEND_IND_SIG
{
    _SIGNAL_VARS
    RTP_SESS_ID_T       session_id;         //rtp session id
}RTP_RTCP_SRRR_SEND_IND_SIG_T;

typedef struct _RTP_RTCP_DATA_RCVD_IND_SIG
{
    _SIGNAL_VARS
    RTP_SESS_ID_T           session_id;     //rtp session id
    RTCP_APP_DATA_PARAM_T   app_data_param; //Application-Defined RTCP Packet param
}RTP_RTCP_DATA_RCVD_IND_SIG_T;              //rtcp data received indication

typedef struct _RTP_RTCP_SRRR_RCVD_IND_SIG
{
    _SIGNAL_VARS
    RTP_SESS_ID_T           session_id;     //rtp session id
    RTCP_SRRR_DATA_PARAM_T  data_param;     //RTCP Packet sr/rr param
}RTP_RTCP_SRRR_RCVD_IND_SIG_T;              //rtcp sr/rr received indication

typedef struct _RTP_RTCP_BYE_RCVD_IND_SIG
{
    _SIGNAL_VARS
    RTP_SESS_ID_T           	 session_id;//rtp session id     
    uint32  					 ssrc;      //the source that leave the session
}RTP_RTCP_BYE_RCVD_IND_SIG_T;
typedef struct _RTP_OVER_TCP_DATA_PARAM
{
    uint32 rtp_channel_id;            //rtp over tcp data channel id
    uint8 *rtp_tcp_data_ptr;            //rtp over tcp data ptr
    uint32 remote_ip;            //rtp over tcp server ip address
    uint16 remote_port;            //rtp over tcp server port
    uint16 rtp_data_len;            //rtp over tcp data len
}RTP_OVER_TCP_DATA_PARAM_T;
//the message body define end
/****************************************************************************/
//Description : rtp&rtcp Initiation
//Global resource dependence : 
//Author: kelly.li
//Note:	
/****************************************************************************/
PUBLIC void RTP_Init(void);

/****************************************************************************/
// 	Description : create rtp session
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_CreateSession(
                            const RTP_CREATE_SESSION_PARAM_T *rtp_init_param_ptr,   //[IN]create session param
                            RTP_CREATE_SESSION_RESULT_T      *rtp_rsult_param_ptr   //[OUT]result param
                            );

/****************************************************************************/
// 	Description : get max rtp payload and app data length
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC uint32 RTP_GetMaxSendSize(
                          RTP_SESS_ID_T   session_id                    //[IN]rtp session id
                          );

/****************************************************************************/
// 	Description : send data with rtp protocol
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC  RTP_ERROR_E  RTP_SendRtpData(
                         const RTP_DATA_SEND_REQ_PARAM_T *rtp_data_param_ptr //[IN]sens rtp data param
                         );

/****************************************************************************/
// 	Description : send data with rtcp protocol
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:data length should be divided exactly by 4	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_SendRtcpAppDataReq(
                          const RTCP_APP_DATA_SEND_REQ_PARAM_T *rtcp_app_data_ptr //[IN]send rtcp app param
                          );

/****************************************************************************/
// 	Description : send sr/rr with rtcp protocol
//	Global resource dependence : 
//  Author: kelly.li
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_SendRtcpSrrrReq(
                           const RTCP_SRRR_SEND_REQ_PARAM_T *rtcp_send_srrr_ptr //[IN]send rtcp srrr param
                           );

/****************************************************************************/
// 	Description : set srrr send mode(rtp auto sending or app manual sending)
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_SetSrrrSendingMode(
                            RTP_SESS_ID_T   rtp_session_id, //[IN]rtp session id
                            BOOLEAN         is_auto_send    //[IN]is rtp auto send srrr
                            );

/****************************************************************************/
// 	Description : get srrr send mode(rtp auto sending or app manual sending)
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_GetSrrrSendingMode (
                           RTP_SESS_ID_T   rtp_session_id,          //[IN]rtp session id
                           BOOLEAN         *is_rtp_auto_send_ptr    //[OUT]is rtp auto send sr/rr
                           );

/****************************************************************************/
// 	Description : set if app will receive SR/RR info
//	Global resource dependence : 
//  Author: kelly.li
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_SetSrrrReceivingMode(
                            RTP_SESS_ID_T    rtp_session_id, //[IN]rtp session id
                            BOOLEAN          is_receive_srrr //[IN]is app receive srrr
                            );

/****************************************************************************/
// 	Description : get if app will receive SR/RR info
//	Global resource dependence : 
//  Author: kelly.li
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_GetSrrrReceivingMode(
                                RTP_SESS_ID_T    rtp_session_id,        //[IN]rtp session id
                                BOOLEAN          *is_receive_srrr_ptr   //[OUT]if app willing to receive sr/rr
                                );

/****************************************************************************/
// 	Description : get remote address info by ssrc 
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_GetRemoteAddressBySsrc (
                        RTP_SESS_ID_T         session_id,   //[IN]rtp session id
                        uint32                ssrc,         //[IN]rtp source id
                        RTP_SOCKADDR_PARAM_T* socksddr_ptr  //[OUT]remote address info
                        );

/****************************************************************************/
// 	Description : terminate rtp session 
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_TerminateSession(
                    const RTP_TERMINATE_SESSION_PARAM_T *rtp_terminate_param_ptr //[IN]terminate session param
                    );

/****************************************************************************/
// 	Description : set session mode is rtp over tcp 
//	Global resource dependence : 
//  Author:dewu.jiang
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_SetRtpTcpMode(
                            const RTP_SESS_ID_T   rtp_session_id, //[IN]rtp session id
                            const BOOLEAN   is_rtp_over_tcp_mode  //[IN]is rtp over tcp mode
                            );
                            
/****************************************************************************/
// 	Description : set rtp over tcp rtp channel id
//	Global resource dependence : 
//  Author: dewu.jiang
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_SetRtpTcpChannelId(
                            const RTP_SESS_ID_T   rtp_session_id, //[IN]rtp session id
                            const uint8           rtp_channel_id,    //[IN]rtp over tcp data channel id
                            const uint8           rtcp_channel_id    //[IN]rtcp over tcp data channel id
                            );

/****************************************************************************/
// 	Description : set rtp over tcp rtp rtsp context id
//	Global resource dependence : 
//  Author:dewu.jiang
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_SetRtpTcpRtspContext(
                            const RTP_SESS_ID_T   rtp_session_id, //[IN]rtp session id
                            const uint32         rtsp_context_id  //[IN]rtp over tcp data rtsp context id
                            );

/****************************************************************************/
// 	Description : recv rtp over tcp data rtsp callback
//	Global resource dependence : 
//  Author:dewu.jiang
//	Note:	
/****************************************************************************/
PUBLIC RTP_ERROR_E RTP_RtspRevRtpOverTcpData( 
                                const RTP_OVER_TCP_DATA_PARAM_T *rtp_tcp_parm_ptr
                                );

#endif


