 /* $id:MMSProl_Interface.h  V1.0 2002/09/20 */

/******************************************************************************
 *  This source code has been made available to you by CORETEK  on
 *  AS-IS.Anyone receiving this source is licensed under
 *  CORETEK copyrights to use it in any way he or she deems fit,including
 *  copying it,modifying it,compiling it,and redistributing it either with
 *  or without modifictions.
 *
 *
 *  Any person who transfers this source code or any derivative work must
 *  include the CORETEK copyright notice, this paragraph,and the preceding
 *  two paragraphs in the transferred software.
 *
 *
 *        COPYRIGHT CORETEK CORPORATION 2004
 *     LICENSED MATERIAL - PROGRAM PROPERTY OF CORETEK
 *****************************************************************************/

/******************************************************************************
 *
 *  FILE: MMSProl_Interface.h
 *
 *  MODULE: MMS Protocol
 *
 *  PURPOSE: Define interface of mms protocol ,the api and macro used by other module
 *
 *  AUTHOR(S):XXLIN
 *
 *  GROUP:PDA_GROUP
 *
 *  DATE CREATED:2002/09/20
 *
 *  REFERENCE DOCUMENT ID:
 *
 *  MODIFICATIONS:
 *  Date          user Name       Description
 *  2002/09/20    XXLIN           Create this file
 *  2004/06/10    XXLIN           Modified this file
 *********************************************************************************/

#ifndef _MMSPROL_INTERFACE_H
#define _MMSPROL_INTERFACE_H


#include "sysTypes.h"

/****************************************************
*					macro  define                   *
*****************************************************/

/* status value  */
#define MMS_EXPIRED 		0x80
#define MMS_RETRIEVED		0x81
#define MMS_REJECTED		0x82
#define MMS_DEFERRED		0x83
#define MMS_UNRECOGNISED	0x84

#define MMS_ABSOLUTE_TOKEN		0x80	/* Time flag:Absolute-token */
#define MMS_RELATIVE_TOKEN		0x81	/* Time flag:Relative-token */

/* Priority-value  */
#define MMS_PRIORITY_LOW		0x80	
#define MMS_PRIORITY_NORMAL		0x81	
#define MMS_PRIORITY_HIGN		0x82

/* message struct type define  */
#define MMS_STRUCT_MMS_SEND_REQUEST		1
#define MMS_STRUCT_MMS_SEND_CONF		2
#define MMS_STRUCT_MMS_NOTIFICATION_IND	3
#define MMS_STRUCT_MMS_RETRIEVE_CONF	4
#define MMS_STRUCT_MMS_DELIVERY_IND		5
#define MMS_STRUCT_MMS_NOTIFYRESP_IND	6
#define MMS_STRUCT_MMS_ACKNOWLEDGE_IND	7


/* Send mms Response-status   */
#define MMS_SEND_RESPONSE_OK								0
#define MMS_SEND_RESPONSE_ERROR_UNSPECIFIED					1
#define MMS_SEND_RESPONSE_ERROR_SERVICE_DENIED				2
#define MMS_SEND_RESPONSE_ERROR_MESSAGE_FORMAT_CORRUPT		3
#define MMS_SEND_RESPONSE_ERROR_SENDING_ADDRESS_UNRESOLVED	4
#define MMS_SEND_RESPONSE_ERROR_MESSAGE_NOT_FOUND			5
#define MMS_SEND_RESPONSE_ERROR_NETWORK_PROBLEM				6
#define MMS_SEND_RESPONSE_ERROR_CONTENT_NOT_ACCEPTED		7
#define MMS_SEND_RESPONSE_ERROR_UNSUPPORTED_MESSAGE			8
#define MMS_SEND_RESPONSE_ERROR_UNKNOW						9
#define MMS_SEND_RESPONSE_ERROR_FUNCTION_ERROR				10


/* Error Number  */
#define MMS_SUCCESS									0
#define MMS_ERROR_MALLOC_FAIL						-1
#define MMS_LOAD_REQUEST_ERROR						-2
#define MMS_RECEIVE_CONTENT_FAIL 					-3
#define MMS_RECEIVED_PARSE_ERROR					-4
#define MMS_SEND_ERROR								-5
#define MMS_SEND_ACK_FAIL							-6
#define MMS_RECEIVED_HTTP_ERROR						-7
#define MMS_RECEIVED_HTTP_CANNOT_FOUND 				-8
#define MMS_RECEIVED_HTTP_BAD_REQUEST				-9
#define MMS_RECEIVED_HTTP_INTERNAL_SERVER_ERROR		-10
#define MMS_RECEIVED_HTTP_BAD_GATEWAY				-11
#define MMS_RECEIVED_HTTP_GATEWAY_TIMEOUT			-12
#define MMS_HTTP_OPERATION_FORBIDDEN				-13
#define MMS_SEND_NOTIFY_RESPONSE_FAIL				-14
#define MMS_SEND_CONF_STATUS_ERROR					-15
#define MMS_PARAMETERS_ERROR						-20
#define MMS_WAP_IS_BUSY								-21


#ifdef __cplusplus
T_EXTERN "C"{
#endif




/*	M_Send.req message struct */
typedef struct T_MMS_Send_Request
{
	T_BOOL  Delivery_Report;
	T_BOOL  Read_Reply;
	T_BOOL  Sender_Visibility;
	T_BYTE *Transaction_Id;
	T_BYTE *To;
	T_BYTE *Cc;
	T_BYTE *Bcc;
	T_BYTE *Subject;
	T_UBYTE *ContentTypeAndBody;
	T_WORD  MMS_Expiry; /* expiry time,default use Relative-token value */
	T_WORD  MMS_Delivery_Time; /*Delivery time,default use Relative-token value  */
	T_WORD MMS_Priority;
	T_WORD ContentTypeAndBody_Len;
}T_MMS_Send_Request;


/* M_Notificaton.ind message struct */
typedef struct T_MMS_Notification_Ind
{
	T_UBYTE  Message_Class;
	T_UBYTE MMS_Expiry_TimeType;
	T_WORD Message_Size;
	T_WORD  MMS_Expiry;
	T_BYTE *Transaction_Id;
	T_BYTE *From;
	T_BYTE *Subject;
	T_BYTE *Content_Location;
}T_MMS_Notification_Ind;

typedef struct T_MMS_Retrieve_Conf
{
	T_BOOL  Delivery_Report;
	T_BOOL  Read_Reply;
	T_UBYTE  Message_Class;
	T_UBYTE  MMS_Prioity;
	T_UBYTE Boundary[2];   /*字节对齐 补充位 */
	T_WORD Date;
	T_WORD Content_Size;
	T_UBYTE *ContentTypeAndBody;
	T_BYTE *Transaction_Id;
	T_BYTE *Message_Id;
	T_BYTE *From;
	T_BYTE *To;
	T_BYTE *Cc;
	T_BYTE *Subject;
}T_MMS_Retrieve_Conf;

/* M_Delivery.ind message struct */
typedef struct T_MMS_Delivery_Ind
{
	T_UBYTE MMS_Status;
	T_UBYTE Boundary[3];	/* 字节对齐 补充位 */
	T_BYTE *Message_Id;
	T_BYTE *To;
	T_WORD Date;
}T_MMS_Delivery_Ind;










	/**************************************************
*					API  define                   *
***************************************************/

/**************************************************************
 * FUNCTION: fnMMS_MakeSendMessagePdu
 *
 * PURPOST: encode the send message pdu.
 *
 * PARAMETER
 * 		InPut :  
 *			T_BYTE *Uri--the uri of the Proxy-Relay.
 *			T_MMS_Send_Request *Msg--the MM's struct of m-send.req.
 *		OutPut: 
 *			T_WORD *length -- the length of the pdu.
 *		InOut : 
 * Return Value:return the pdu buffer's pointer. 
 *
 * Reentrant: none
 **************************************************************/
T_UBYTE* fnMMS_MakeSendMessagePdu(T_MMS_Send_Request *Msg,int *length);




/**************************************************************
 * FUNCTION: fnMMS_AllocMessageMem
 *
 * PURPOST: According to the message struct type to alloc mem for
 *          the message.
 *
 * PARAMETER
 * 		InPut :  
 *			T_BYTE Type--the struct type value.
 *		OutPut: none
 *		InOut : none
 *
 * Return Value: the address of the struct .
 *
 * Reentrant: none
 **************************************************************/
T_VOID *fnMMS_AllocMessageMem(T_UBYTE Type);
/**************************************************************
 * FUNCTION: fnMMS_FreeMessageMem
 *
 * PURPOST: According to the message struct type to free the  mem used by
 *          the message struct .
 *
 * PARAMETER
 * 		InPut :  
 *			T_BYTE Type--the struct type value.
 *			T_VOID *pAddr-- the address of the mem which will be freed.
 *		OutPut: none
 *		InOut : none
 *
 * Return Value:none.
 *
 * Reentrant: none
 **************************************************************/
T_VOID  fnMMS_FreeMessageMem(T_VOID *pAddr ,T_UBYTE Type);


/**************************************************************
 * FUNCTION: fnMMS_ParseNotificationInd
 *
 * PURPOST: receive the Notification information  from the proxy relay .
 *
 * PARAMETER
 * 		InPut :  
 *			T_UBYTE* Buf -- the push content data  pointer.
 *			T_WORD Content_len -- the buf's length.
 *		OutPut:none 
 *		InOut : none
 *
 * Return Value: the notificaiton ind struct pointer.
 *
 * Reentrant: none
 **************************************************************/
T_MMS_Notification_Ind* fnMMS_ParseNotificationInd(T_UBYTE *Buf,T_WORD Content_len);


/**************************************************************
 * FUNCTION: fnMMS_ParseRetrieveConf
 *
 * PURPOST: parse the MM encode content .
 *
 * PARAMETER
 * 		InPut :  
 *			T_UBYTE *Buf -- the encode MM data content's pointer.
 *			
 *		OutPut:none
 *		InOut : none
 *
 * Return Value: the retrieve.conf struct pointer.
 *
 * Reentrant: none
 **************************************************************/
T_MMS_Retrieve_Conf * fnMMS_ParseRetrieveConf(T_UBYTE *Buf,T_WORD Content_len);

/**************************************************************
 * FUNCTION: fnMMS_ParseDeliveryInd
 *
 * PURPOST: receive the delivery Indication from the proxy relay .
 *
 * PARAMETER
 * 		InPut :  
 *			T_UBYTE* Buf -- the push content data filed's pointer.
 *			
 *		OutPut: none
 *		InOut : none
 *
 * Return Value: the deliver ind struct pointer.
 *
 * Reentrant: none
 **************************************************************/
T_MMS_Delivery_Ind * fnMMS_ParseDeliveryInd(T_UBYTE *Buf,T_WORD Content_len);


/**************************************************************
 * FUNCTION: fnMMS_MakeNotifyRespPdu
 *
 * PURPOST: encode the notify response pdu.
 *
 * PARAMETER
 * 		InPut : 
 *			T_BYTE *Trans_ID --the Id of notificaiton .
 *			T_BOOL SendDevRep -- if send the delivery ind.
 *			T_BYTE Status -- the status of the mms.
 *		OutPut: 
 *			T_WORD *len -- the length of the pdu.
 *		InOut : 
 *
 * Return Value: the pdu's buffer pointer.
 *
 * Reentrant: none
 **************************************************************/		
T_UBYTE* fnMMS_MakeNotifyRespPdu(T_BYTE *Trans_ID,T_BOOL SendDevRep,T_UBYTE Status,T_WORD *len);

/**************************************************************
 * FUNCTION: fnMMS_MakeAcknowledgePdu
 *
 * PURPOST: encode the acknowledge pdu.
 *
 * PARAMETER
 * 		InPut :  
 *			T_BYTE *Trans_ID --the Id of notificaiton .
 *			T_BOOL SendDevRep -- if send the delivery ind.
 *		OutPut: none.
 *			T_WORD *len -- the length of the pdu.
 *		InOut : none.
 *
 * Return Value: the pdu buffer's pointer.
 *
 * Reentrant: none
 **************************************************************/
T_UBYTE* fnMMS_MakeAcknowledgePdu(T_BYTE *Trans_Id,T_BOOL SendDevRep,T_WORD *len);


/**************************************************************
 * FUNCTION: fnMMS_MakeReadReportPdu
 *
 * PURPOST: encode the acknowledge pdu.
 *
 * PARAMETER
 * 		InPut :  
 *			T_BYTE *Trans_ID --the Id of new mm .
 *			T_CHAR *From -- the sender of the received mm .
 *			T_CHAR *Subject -- the subject of the received mm.
 *		OutPut: none.
 *		InOut : none.
 *
 * Return Value: the read report pdu buffer's pointer.
 *
 * Reentrant: none
 **************************************************************/
T_UBYTE* fnMMS_MakeReadReportPdu(T_BYTE *Trans_Id,T_CHAR *From,T_CHAR *Subject,T_WORD *len);


/**************************************************************
 * FUNCTION: fnMMS_FreeMadeMsgPduBuf
 *
 * PURPOST: free the pdu buffer which get from fnMMS_Make* Api,these
 *          pdu buffer is malloc in protocol module.
 *
 * PARAMETER
 * 		InPut :  
 *			T_VOID *pMsgPdu -- the message pdu's buffer.
 *		OutPut: none.
 *		InOut : none.
 *
 * Return Value: none.
 *
 * Reentrant: none
 **************************************************************/
T_VOID fnMMS_FreeMadeMsgPduBuf(T_VOID *pMsgPdu);


/**************************************************************
 * FUNCTION: fnMMS_RecvSendConfData
 *
 * PURPOST: to deal with the send response data.
 *
 * PARAMETER
 * 		InPut :  
 *			pData -- the received data buffer.
 *			Size -- the buffer's size.
 *		OutPut : none
 *		InOut : none
 *
 * Return Value: the send response's status value.
 *
 * Reentrant: none
 **************************************************************/
T_HWORD fnMMS_RecvSendConfData(T_UBYTE*pData,T_WORD Size);




#ifdef __cplusplus
}
#endif

#endif
