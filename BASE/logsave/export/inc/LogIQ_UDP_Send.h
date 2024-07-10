/****************************************************************************************
** File Name:		LogIQ_UDP_Send.h															*	
** Author:		taiqiang xu																*
** DATE:			09/11/2011																*
** CopyRight		2011 SpreadTrum Communications CORP. All Rights Reserved.							*
** Description:		This is the head file for main function Prototypes of IQ Log          						*
*****************************************************************************************


*****************************************************************************************
**					Edit History															*	
** ------------------------------------------------------------------------------------- *
**																						*
** DATE:				NAME				DESCIPTION										*
** 09/11/2011		taiqiang xu			Create.											*
*****************************************************************************************/
#ifndef LOGIQ_UDP_SEND_H_
#define LOGIQ_UDP_SEND_H_

#ifdef __cplusplus
{
#endif

#include "logsave_main.h"

#include "sci_api.h"
#include "sci_types.h"
//#include "scttypes.h"
//#include "ctypes.h"
#include "os_api.h"

//#include "tcpip_api.h"
#include "app_tcp_if.h"
//#include "socket_api.h"
//#include "tcpip_types.h"


/**---------------------------------------------------------------------------*
 **                         Macro definition                                  									    *
 **---------------------------------------------------------------------------*/
 #define UDP_MESSAGE_MAX_LENGTH		1500
 #define UDP_DATA_MAX_LENGTH		1400
 #define UDP_DATA_HALF_LENGTH		750
 #define UDP_DATA_QUARTER_LENGTH	375

#define ETHERNET_LENG		0
#define UDP_DTAT_TYPE	 		1
#define UDP_SOURCE_PORT_POS		(ETHERNET_LENG + 20)
#define UDP_DESTIN_PORT_POS		(ETHERNET_LENG +22)
#define UDP_DATA_POS			(ETHERNET_LENG + 28)

#define UDP_SOUR_IP_POS		(ETHERNET_LENG + 12)
#define UDP_DESTIN_IP_POS	(ETHERNET_LENG + 16)

#define IP_HEAD_TOTAL_LENG_POS  (ETHERNET_LENG + 2)
#define IP_IDENTIFICATION_POS	(ETHERNET_LENG + 4)

#define LogIQ_Free(ptr)	   {\
						if(NULL != (ptr))\
						    {\
							SCI_Free(ptr);\
							(ptr) = NULL;\
						    }\
                                            }

/**---------------------------------------------------------------------------*
 **                         		globle variables    					    	  					    *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                        		 Data Structures                                   							    *
 **---------------------------------------------------------------------------*/

typedef struct tag_configPara
{
	uint8 voice;
	uint8 iq_Measure;
	uint8 iq_noMeasure;
	uint8 arm_log;
	uint8 dsp_log;
}LOGIQ_CONFIG_PARA_T;

typedef struct tag_LogPara
{	
	LOGIQ_CONFIG_PARA_T config_para;
	uint8 start_stop_flag;		/* 1:start, 0:stop or init*/
	uint8 start_msg_arr[13];
	uint8 stop_msg_arr[12];
	uint8	 config_msg_arr[14];
	uint16 start_msg_len;
	uint16 stop_msg_len;
	uint16 config_msg_len;
	uint16 dst_port;
	uint16 scr_port;
	uint16 Nsapi;
	uint32 dst_ip;
	uint32 scr_ip;
}LOGIQ_UDP_PARA_T;

/* logIQ pseudo header for checksumming */
typedef struct ph_tags
{
   uint32   ph_src;     /* source address */
   uint32   ph_dest;    /* dest address */
   uint8    ph_zero;    /* zero (reserved) */
   uint8    ph_prot;    /* protocol */
   uint16   ph_len;     /* udp length */
} LOGIQ_PSEUDO_HEADER_T;

typedef struct tag_MsgReq
{
    SIGNAL_VARS
    uint8*              data_buffer;
    uint32              buffer_length;
    FM_OPERATION_TYPE_E msg_types;
}LOGIQ_UDP_MESSAGE_REQ_T;


/*------------------------------------------------------------------------------
FUNCATION NAME:		LogIQ_UDP_Modify_Data_Buffer
Description:			modify data mem buffer 
Notes:	       			taiqiang xu
------------------------------------------------------------------------------*/
extern void LogIQ_UDP_Modify_Data_Buffer(BOOLEAN result, FM_OPERATION_TYPE_E  fm_type, uint32 param1);


/*------------------------------------------------------------------------------
FUNCATION NAME:		LogIQ_UDP_Send_Data_Req
Description:			send Log IQ data req    
Notes:	       			taiqiang xu
------------------------------------------------------------------------------*/
extern BOOLEAN  LogIQ_UDP_Send_Data_Req (uint8* write_buff_ptr, uint32 write_length,  FM_OPERATION_TYPE_E file_type,  BOOLEAN  sync_mode );


/*------------------------------------------------------------------------------
FUNCATION NAME:		LogIQ_UDP_Send_ToMN
Description:			send Log IQ data to MN layer by UDP Message    
Notes:	       			taiqiang xu
------------------------------------------------------------------------------*/
extern BOOLEAN LogIQ_UDP_Send_ToMN(uint8 *pScr,  uint32 scrLen, FM_OPERATION_TYPE_E file_type);


/*------------------------------------------------------------------------------
FUNCATION NAME:		LogIQ_UDP_Recv_FromMN
Description:			recv from Venet UDP log IQ data    
Notes:	       			taiqiang xu
------------------------------------------------------------------------------*/
 extern uint32  LogIQ_UDP_Recv_FromMN(uint8 *dstIP,   uint8*pdstPort, uint16 nsapi, FM_OPERATION_TYPE_E LogIQ_type);

/*------------------------------------------------------------------------------
FUNCATION NAME:		LogIQ_UDP_Get_Para
Description:			get udp msg para   
Notes:	       			taiqiang xu
------------------------------------------------------------------------------*/
extern LOGIQ_UDP_PARA_T* LogIQ_UDP_Get_Para(void);

/*------------------------------------------------------------------------------
FUNCATION NAME:		LOGIQ_UDP_Para_Init
Description:			udp para init
Notes:	       			taiqiang xu
------------------------------------------------------------------------------*/
extern void LOGIQ_UDP_Para_Init(void);


#ifdef __cplusplus
    }
#endif

#endif
