
#ifndef __OD_ADAPTER_HTTP_H__
#define __OD_ADAPTER_HTTP_H__

#include "od_type_def.h"

//***********************************************************//
//********************** Application include ********************//
//***********************************************************//


//***********************************************************//
//********************** Macro define *************************//
//***********************************************************//

#define OD_RECORD_SECOND_COUNT_MAX2 (20)

typedef struct
{
    OD_U8 *host_name;
    OD_U32 host_name_len;
    OD_U8 *url_link;
    OD_U32 url_link_len;
    OD_U8 *param;
    OD_U32 param_len;
    OD_U16 ser_port_num;
} OD_HTTP_URL_STRUCT2;


typedef OD_S32 (*OD_SEND_BUF_FUNC2)(OD_U8* buf_p, OD_HTTP_URL_STRUCT2 * url_info, OD_U32* rcvd_data_len);
typedef OD_BOOL (*OD_BEGIN_FUNC2)(OD_U32 size);
typedef OD_BOOL (*OD_DATA_FUNC2)(void* data, OD_S32 size);
typedef OD_BOOL (*OD_END_FUNC2)(void);
typedef void (*OD_ERROR_FUNC2)(OD_U32 erro);


//***********************************************************//
//********************** Enum define**************************//
//***********************************************************//
typedef enum
{
    OD_STREAM2_HTTP,
    OD_STREAM2_WMSP,
    OD_STREAM2_DNS
} OD_HTTP_PROTO_ENUM2;

/* reurn http state to other module*/
typedef enum
{	
	OD_HTTP_STATE2_NONE,// 0
	OD_HTTP_STATE2_START_NETWORK,// 1
	OD_HTTP_STATE2_CONNECTING,// 2		
	OD_HTTP_STATE2_REQUSTING,// 3		
	OD_HTTP_STATE2_BUFFER_0,	// 4	
	OD_HTTP_STATE2_BUFFERING,	// 5	
	OD_HTTP_STATE2_FAIL,// 6
	OD_HTTP_STATE2_EXCEPTION,// 7
	OD_HTTP_STATE2_BUFFERED	// 8
}OD_HTTP_STATE2_ENUM2;

/* the error in http module */
typedef enum
{
    OD_HTTP_SOC2_NO_ERROR, // 0
    OD_HTTP_SOC2_INVAL_PARA,	// 1			
    OD_HTTP_SOC2_BUSY, // 2
    OD_HTTP_SOC2_UNKNOWN_URL,	 // 3
    OD_HTTP_CREATE_SOC_FAIL2, // 4
    OD_HTTP_SOC2_CONNECT_FAILED, // 5
    OD_HTTP_SOC2_SEND_FAILED, // 6
    OD_HTTP_SOC2_RECV_FAILED, // 7
    OD_HTTP_STATUS_NOK2, // 8
    OD_HTTP_FILE_ACCESS_FAILED2, // 9
    OD_HTTP_GPRS_NO_BEARER2, // 10				
    OD_HTTP_SERVER_CLOSED2, // 11
    OD_HTTP_BUFFER2_EXCEEDED, // 12
    OD_HTTP_BUFFER2_UNCLOSED, // 13
    OD_HTTP_BUFFER2_INVALID, // 14 
    OD_HTTP_BUFFER2_UNKNOWN, // 15
    OD_HTTP_DATA_EMPTY2, // 16
    OD_HTTP_BAD_REQUEST2, // status : 400 404 ×´Ì¬Âë
} OD_HTTP_RESULT_ENUM2;

typedef enum
{
    OD_HTTP_IDLE2 = 0,
    OD_HTTP_SOC2_CREATING, // 1
    OD_HTTP_SOC2_CREATED,// 2
    OD_HTTP_DNS_QUERY2,// 3
    OD_HTTP_DNS_DONE2,// 4
    OD_HTTP_SOC2_CONNECTING,// 5
    OD_HTTP_SOC2_CONNECT_RETRY,// 6
    OD_HTTP_SOC2_CONNECTED,// 7
    OD_HTTP_REQ_SEND_RETRY2,// 8
    OD_HTTP_REQ_SENDING2,// 9
    OD_HTTP_REQ_SENT2,// 10
    OD_HTTP_RSP_RCVING2,// 11
    OD_HTTP_RSP_RCVD2,// 12
    OD_HTTP_TRANSMITTING2,// 13
    OD_HTTP_TRANSMITTED2,// 14
    OD_HTTP_WAIT_FOR_SERVER2,// 15
    OD_HTTP_WAIT_FOR_NEXT_REQUEST2,// 16
    OD_HTTP_WAIT_FOR_NEXT_RECV2,    // 17
    OD_HTTP_SOC2_CLOSING,// 18
    OD_HTTP_SOC2_CLOSED,// 19
}OD_HTTP_INNER_STATE_ENUM2;


//***********************************************************//
//********************** Struct define **************************//
//***********************************************************//
typedef struct
{
    OD_U32 percent;
    OD_HTTP_STATE2_ENUM2 state;
}OD_HTTP_DATA_STATUS2;

void Od_HttpOpen2(OD_U8* url, 
				OD_SEND_BUF_FUNC2 send_buf_cb, 
				OD_BEGIN_FUNC2 begin_cb, 
				OD_DATA_FUNC2 data_cb, 
				OD_END_FUNC2 end_cb,
				OD_ERROR_FUNC2 error_cb);

void Od_HttpStop2(void);
OD_BOOL Od_HttpInit2(OD_BOOL type);
void Od_HttpGetStatus2(OD_HTTP_DATA_STATUS2 * status);
#endif
