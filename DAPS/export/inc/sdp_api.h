/****************************************************************************
** File Name:      sdp_api.h                                                *
** Author:         bruce.chi                                                *
** Date:           2008.4                                                   *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the SDP api                *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2008.5        bruce.chi         Modify                                   *
** -------------------------------------------------------------------------*
**                          Note: Supported SDP Method                      *
** -------------------------------------------------------------------------*
** Connection:(c=)                                                          *
** Media Description: (m=)                                                  *
** RTP map Attribute: (a=rtpmap)                                            *
** RTCP attribute: (a=rtcp)                                                 *
** Codec format parameters: (a=fmtp)                                        *
** Preconditions attributes: (a=curr/des/conf)                              *
** Connection mode: (a=sendonly/recvonly/sendrecv/inactive)                 *
** Other attribute : (a=control, or a=framesize, etc)                       *
**                      use SDP_MediaDescrGetAttrValueByName)               *
** Bandwidth: (b=)                                                          *
****************************************************************************/

#ifndef _SDP_API_H
#define _SDP_API_H

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "os_api.h"
#include "in_message.h"
#include "sip_api.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SOA_OK  0   //SDP functions return vaule


/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum {
    SDP_NETTYPE_NOTSET    = 0,
    SDP_NETTYPE_IN,                    /*  IN     */
    SDP_NETTYPE_ATM,                   /*  ATM    */
    SDP_NETTYPE_LOCAL,                 /*  LOCAL  */
    SDP_NETTYPE_OTHER,                 /*  Other  */
    SDP_NETTYPE_TN,                    /*  TN     */
    SDP_NETTYPE_ANY,                   /*  ANY    */
    SDP_NETTYPE_IGNORE,                /*  IGNORE */
    SDP_NETTYPE_UNKNOWN
}SDP_NET_TYPE_E;

typedef enum {
    SDP_ADDRTYPE_NOTSET    = 0,
    SDP_ADDRTYPE_IP4,                  /*  IP4    */
    SDP_ADDRTYPE_IP6,                  /*  IP6    */
    SDP_ADDRTYPE_ENDPOINT,             /*  epn    */
    SDP_ADDRTYPE_NSAP,                 /*  nsap   */
    SDP_ADDRTYPE_E164,                 /*  e164   */
    SDP_ADDRTYPE_GWID,                 /*  GWID   */
    SDP_ADDRTYPE_ALIAS,                /*  ALIAS  */
    SDP_ADDRTYPE_RFC2543,              /*  RFC2543*/
    SDP_ADDRTYPE_ANY,                  /*  ANY    */
    SDP_ADDRTYPE_IGNORE,               /*  IGNORE */
    SDP_ADDRTYPE_UNKNOWN
} SDP_ADDRTYPE_E;

typedef enum
{
    SDP_CONNECTMODE_NOTSET    = 0,
    SDP_CONNECTMODE_SENDONLY,       /* "sendonly" */
    SDP_CONNECTMODE_RECVONLY,       /* "recvonly" */
    SDP_CONNECTMODE_SENDRECV,       /* "sendrecv" */
    SDP_CONNECTMODE_INACTIVE        /* "inactive" */
} SDP_CONNECTION_MODE_E;

typedef enum {
    SDP_MEDIATYPE_NOTSET   = 0,
    SDP_MEDIATYPE_AUDIO,                /*  audio  */
    SDP_MEDIATYPE_NAS,                  /*  nas    */
    SDP_MEDIATYPE_VIDEO,                /*  video    */
    SDP_MEDIATYPE_APP,                  /*  application */
    SDP_MEDIATYPE_DATA,                 /*  data    */
    SDP_MEDIATYPE_IMAGE,                /*  image   */
    SDP_MEDIATYPE_CONTROL,              /*  control */
    SDP_MEDIATYPE_UNKNOWN
} SDP_MEDIATYPE_E;

typedef enum {
    SDP_PROTOCOL_NOTSET       = 0,
    SDP_PROTOCOL_RTP,           /*  RTP/AVP  */
    SDP_PROTOCOL_RTP_SAVP,      /*  RTP/SAVP  */
    SDP_PROTOCOL_LOCAL,         /*  LOCAL  */
    SDP_PROTOCOL_ATM,           /*  atm/avp  */
    SDP_PROTOCOL_UDP,
    SDP_PROTOCOL_UDP_T38,
    SDP_PROTOCOL_TCP,    
    SDP_PROTOCOL_AAL1ATMF,
    SDP_PROTOCOL_AAL1ITU, 
    SDP_PROTOCOL_AAL1CUSTOM,
    SDP_PROTOCOL_AAL2ATMF,  
    SDP_PROTOCOL_AAL2ITU,   
    SDP_PROTOCOL_AAL2CUSTOM,
    SDP_PROTOCOL_AAL5ATMF,  
    SDP_PROTOCOL_AAL5ITU,   
    SDP_PROTOCOL_AAL5CUSTOM,
    SDP_PROTOCOL_H323C,     
    SDP_PROTOCOL_UNKNOWN   
} SDP_MEDIA_PROTOCOL_E;     //SDP media protocols


typedef enum
{
    SDP_PRECOND_NAME_CURRENT_ATTR =0,
    SDP_PRECOND_NAME_DESIRED_ATTR,
    SDP_PRECOND_NAME_CONFIRMED_ATTR,
    SDP_PRECOND_NAME_ALL,
    SDP_PRECOND_NAME_UNKNOWN
}SDP_PRECOND_NAME_E;

typedef enum 
{
    SDP_PRECOND_TYPE_NOTSET = 0, 
    SDP_PRECOND_TYPE_QOS ,       /* QOS = quality Of Service */
    SDP_PRECOND_TYPE_UNKNOWN 
} SDP_PRECOND_TYPE_E;

typedef enum 
{
    SDP_PRECOND_STATUS_TYPE_NOTSET = 0, 
    SDP_PRECOND_STATUS_TYPE_E2E ,
    SDP_PRECOND_STATUS_TYPE_LOCAL ,       
    SDP_PRECOND_STATUS_TYPE_REMOTE ,       
    SDP_PRECOND_STATUS_TYPE_UNKNOWN 
} SDP_PRECOND_STATUS_TYPE_E;

typedef enum 
{
    SDP_PRECOND_STRENGTH_NOTSET = 0, 
    SDP_PRECOND_STRENGTH_NONE ,
    SDP_PRECOND_STRENGTH_OPTIONAL ,       
    SDP_PRECOND_STRENGTH_MANDATORY,  
    SDP_PRECOND_STRENGTH_FAILURE,       
    SDP_PRECOND_STRENGTH_UNKNOWN 
} SDP_PRECOND_STRENGTH_TAG_E;

typedef enum 
{
    SDP_PRECOND_DIRECTION_NOTSET = 0, 
    SDP_PRECOND_DIRECTION_NONE,
    SDP_PRECOND_DIRECTION_SEND,
    SDP_PRECOND_DIRECTION_RECV,       
    SDP_PRECOND_DIRECTION_SENDRECV,  
    SDP_PRECOND_DIRECTION_UNKNOWN 
} SDP_PRECOND_DIRECTION_TAG_E;



/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef void*   SDP_MSG_HANDLE_T;           //sdp message handle
typedef void*   SDP_MEDIA_DESCR_HANDLE_T;   //sdp media describe handle
typedef void*   SDP_CONNECTION_HANDLE_T;    //sdp connection handle
typedef void*   SDP_RTPMAP_HANDLE_T;        //sdp rtpmap handle
typedef void*   SDP_RTCP_ARR_HANDLE_T;      //sdp rtcp attribute handle
typedef void*   SDP_ATTRIBUTE_HANDLE_T;     //sdp attribute handle 
typedef void*   SDP_PRECONDITION_HANDLE_T;  //sdp precondition attribute handle
typedef void*   SDP_BANDWIDTH_HANDLE_T;     //sdp band width handle
typedef int32   SDP_ERR_T;                  //sdp function retuned error code

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         PUBLIC FUNCTION DECLARE                          *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Init SDP moudle
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SDP_Init(void);

/***************************************************************************
 * rvSdpMsgConstructParse
 * ------------------------------------------------------------------------
 * General:
 *      Parses an SDP text message and constructs an RvSdpMsg from the SDP text
 *      message.
 *
 * Return Value:
 *      A pointer to the new SDP message, or NULL if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg to be constructed. Must point to valid memory.
 *      txt - contains a pointer to the beginning of the SDP text message.
 *      len - the length of the parsed SDP message.
 *      stat - the status of parsing termination. Will be RV_SDPPARSER_STOP_ERROR
 *             if there was a parsing error, otherwise another status specifying
 *             a legal character that caused the parser to stop. For more
 *             information on RvSdpParseStatus, see the Enumerations section.
 ***************************************************************************/
PUBLIC SDP_MSG_HANDLE_T SDP_MsgConstructParse(
            char* sdp_str_ptr,  //[IN]
            int* sdp_len_ptr    //[IN]&[OUT]
            );

/***************************************************************************
 * rvSdpMsgEncodeToBuf
 * ------------------------------------------------------------------------
 * General:
 *      Takes an RvSdpMsg as input and encodes it as text into a buffer (according
 *      to the SDP syntax).
 *
 * Return Value:
 *      Returns a pointer past the end of the encoding (buf+number of encoded bytes)
 *      or NULL if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the SDP message to be encoded.
 *      buf - a pointer to a buffer where the message will be encoded.
 *      len - the length of the buffer.
 *      stat - a pointer to a variable where the status of the encoding will be set.
 *             If encoding was successful, stat should be equal to rvOk in
 *             return. Any other value means that encoding failed.
 ***************************************************************************/
PUBLIC char* SDP_MsgEncodeToBuf(
            SDP_MSG_HANDLE_T    msg_handle,     //[IN]
            char                *sdp_str_ptr,   //[IN]
            int                 sdp_str_len     //[IN]
            );


/***************************************************************************
 * rvSdpMsgDestruct
 * ------------------------------------------------------------------------
 * General:
 *      Destroys the RvSdpMsg object and frees up all internal memory.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - pointer to a RvSdpMsg object to be destructed.
 ***************************************************************************/
PUBLIC void SDP_MsgDestruct(
            SDP_MSG_HANDLE_T msg_handle);


/***************************************************************************
 * SDP_MsgGetSessionInformation
 * ------------------------------------------------------------------------
 * General:
 *      get the session information, line "i=" in the session level
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - pointer to a RvSdpMsg object.
 ***************************************************************************/
PUBLIC char* SDP_MsgGetSessionInformation(SDP_MSG_HANDLE_T msg_handle);

/***************************************************************************
 * SDP_MsgGetSessionName
 * ------------------------------------------------------------------------
 * General:
 *      get the session name, line "s=" in the session level
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - pointer to a RvSdpMsg object.
 ***************************************************************************/
PUBLIC char* SDP_MsgGetSessionName(SDP_MSG_HANDLE_T msg_handle);

/*connection*/
/***************************************************************************
 * rvSdpMsgGetNumOfConnections
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of elements in the session level connections list.
 *
 * Return Value:
 *      Size of connections list of SDP message.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 ***************************************************************************/
PUBLIC int32 SDP_MsgGetNumOfConnections(
            const SDP_MSG_HANDLE_T msg);

/***************************************************************************
 * rvSdpMsgGetConnectionByIndex
 * ------------------------------------------------------------------------
 * General:
 *      Gets a connection object by index.
 *
 * Return Value:
 *      The requested connection object.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMsgGetNumOfConnections call.
 ***************************************************************************/
PUBLIC SDP_CONNECTION_HANDLE_T SDP_MsgGetConnectionByIndex(
            const SDP_MSG_HANDLE_T sdp_msg_handle,
            int32 index);

/***************************************************************************
 * rvSdpMsgAddConnection
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new connection object at the session level.
 *
 * Return Value:
 *      Returns the pointer to the newly created RvSdpConnection object if the
 *      function succeeds or NULL pointer if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      net_type - the network type.
 *      addr_type - the address type.
 *      addr - the address, depending on the network type. For example, an IP
 *             address for an IP network, and so on.
 ***************************************************************************/
/*PUBLIC SDP_CONNECTION_HANDLE_T rvSdpMsgAddConnection(
            SDP_MSG_HANDLE_T msg,
            SDP_NET_TYPE_E net_type,
            SDP_ADDRTYPE_E addr_type,
            const char* addr);*/

/***************************************************************************
 * rvSdpMsgSetConnection
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new connection object at the session level.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code
 *      if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      net_type - the network type.
 *      addr_type - the address type.
 *      addr - the address, depending on the network type. For example, an IP
 *             address for an IP network, and so on.
 ***************************************************************************/
/*PUBLIC SDP_ERR_T rvSdpMsgSetConnection(
            SDP_MSG_HANDLE_T msg,
            SDP_NET_TYPE_E net_type,
            SDP_ADDRTYPE_E addr_type,
            const char* addr);*/

/***************************************************************************
 * rvSdpMsgRemoveConnection
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the connection object by index.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMsgGetNumOfConnections call.
 ***************************************************************************/
PUBLIC void SDP_MsgRemoveConnection(
            SDP_MSG_HANDLE_T msg,
            int32 index);

/***************************************************************************
 * rvSdpMsgClearConnection
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) all connections set in SDP message.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 ***************************************************************************/
PUBLIC void SDP_MsgClearConnection(
            SDP_MSG_HANDLE_T sdp_msg_handler);


/***************************************************************************
 * rvSdpConnectionGetNetType
 * ------------------------------------------------------------------------
 * General:
 *      Get the connection's network type.
 * Return Value:
 *      Returns the connection's network type.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 ***************************************************************************/
PUBLIC SDP_NET_TYPE_E SDP_ConnectionGetNetType(
                const SDP_CONNECTION_HANDLE_T conn_handle);

/***************************************************************************
 * rvSdpConnectionSetNetType
 * ------------------------------------------------------------------------
 * General:
 *      Sets the connection's network type.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 *      type - the network type.
 ***************************************************************************/
PUBLIC void SDP_ConnectionSetNetType(
                SDP_CONNECTION_HANDLE_T conn_handle,
                SDP_NET_TYPE_E type);

/***************************************************************************
 * rvSdpConnectionGetNetTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Get the connection's network type string.
 * Return Value:
 *      Returns the connection's network type string.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 ***************************************************************************/
/*PUBLIC const char* rvSdpConnectionGetNetTypeStr(
                SDP_CONNECTION_HANDLE_T conn);*/

/***************************************************************************
 * rvSdpConnectionSetNetTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Sets the connection's network type string.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 *      type - the network type.
 ***************************************************************************/
/*PUBLIC SDP_ERR_T rvSdpConnectionSetNetTypeStr(
                SDP_CONNECTION_HANDLE_T conn,
                const char* type);*/

/***************************************************************************
 * rvSdpConnectionGetAddrType
 * ------------------------------------------------------------------------
 * General:
 *      Get the connection's address type.
 * Return Value:
 *      Returns the connection's address type.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 ***************************************************************************/
PUBLIC SDP_ADDRTYPE_E SDP_ConnectionGetAddrType(
                const SDP_CONNECTION_HANDLE_T conn_handle);

/***************************************************************************
 * rvSdpConnectionSetAddrType
 * ------------------------------------------------------------------------
 * General:
 *      Sets the connection's address type.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 *      type - the address type.
 ***************************************************************************/
PUBLIC void SDP_ConnectionSetAddrType(
                SDP_CONNECTION_HANDLE_T conn_handle,
                SDP_ADDRTYPE_E type);

/***************************************************************************
 * rvSdpConnectionGetAddrTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Get the connection's address type string.
 * Return Value:
 *      Returns the connection's address type string.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 ***************************************************************************/
/*PUBLIC const char* rvSdpConnectionGetAddrTypeStr(
                SDP_CONNECTION_HANDLE_T conn);*/

/***************************************************************************
 * rvSdpConnectionSetAddrTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Sets the connection's address type string.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 *      type - the address type string.
 ***************************************************************************/
/*PUBLIC SDP_ERR_T rvSdpConnectionSetAddrTypeStr(
                SDP_CONNECTION_HANDLE_T conn,
                const char* type);*/

/***************************************************************************
 * rvSdpConnectionGetAddress
 * ------------------------------------------------------------------------
 * General:
 *      Get the connection's address.
 * Return Value:
 *      Returns the connection's address.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 ***************************************************************************/
PUBLIC const char* SDP_ConnectionGetAddress(
                const SDP_CONNECTION_HANDLE_T conn_handle);

/***************************************************************************
 * rvSdpConnectionSetAddress
 * ------------------------------------------------------------------------
 * General:
 *      Sets the connection's address.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 *      addr - the connection's address.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_ConnectionSetAddress(
                SDP_CONNECTION_HANDLE_T conn_handle,
                const char* addr_ptr);

/***************************************************************************
 * rvSdpConnectionGetAddressTTL
 * ------------------------------------------------------------------------
 * General:
 *      Get the connection's address TTL.
 * Return Value:
 *      Returns the connection's address TTL.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 ***************************************************************************/
/*PUBLIC int rvSdpConnectionGetAddressTTL(
                const SDP_CONNECTION_HANDLE_T conn);*/

/***************************************************************************
 * rvSdpConnectionSetAddressTTL
 * ------------------------------------------------------------------------
 * General:
 *      Sets the connection's address TTL.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 *      ttl - the connection's address TTL.
 ***************************************************************************/
/*PUBLIC void rvSdpConnectionSetAddressTTL(
                SDP_CONNECTION_HANDLE_T conn,
                int ttl);*/

/***************************************************************************
 * rvSdpConnectionGetAddressNum
 * ------------------------------------------------------------------------
 * General:
 *      Get the connection's number of subsequent addresses.
 * Return Value:
 *      Returns the connection's number of subsequent addresses.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 ***************************************************************************/
/*PUBLIC int rvSdpConnectionGetAddressNum(
                const SDP_CONNECTION_HANDLE_T conn);*/

/***************************************************************************
 * rvSdpConnectionSetAddressNum
 * ------------------------------------------------------------------------
 * General:
 *      Sets the connection's number of subsequent addresses.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      conn - a pointer to the RvSdpConnection object.
 *      num - the connection's number of subsequent addresses.
 ***************************************************************************/
/*PUBLIC void rvSdpConnectionSetAddressNum(
                SDP_CONNECTION_HANDLE_T conn,
                int num);*/

/****  --- End of Connection Functions --- ****/




/***************************************************************************
 *
 *                        M E D I A   D E S C R I P T O R S 
 *
 ***************************************************************************/

/***************************************************************************
 * rvSdpMsgGetNumOfMediaDescr
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of media descriptors set in SDP message.
 *
 * Return Value:
 *      Number of media descriptors set in SDP message.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 ***************************************************************************/
PUBLIC uint32 SDP_MsgGetNumOfMediaDescr(
            const SDP_MSG_HANDLE_T msg_handle);

/***************************************************************************
* rvSdpMsgGetMediaDescr
* ------------------------------------------------------------------------
* General:
*      Gets a media descriptor object by index.
*
* Return Value:
*      The requested media descriptor object.
* ------------------------------------------------------------------------
* Arguments:
*      msg - a pointer to the RvSdpMsg object.
*      index - the index. The index should start at zero (0) and must be smaller
*              than the number of elements in the list. The number of elements
*              in the list is retrieved by correspondent
*              rvSdpMsgGetNumOfMediaDescr call.
***************************************************************************/
PUBLIC SDP_MEDIA_DESCR_HANDLE_T SDP_MsgGetMediaDescr(
            const SDP_MSG_HANDLE_T msg_handle,
            int32 index);

/***************************************************************************
 * rvSdpMsgAddMediaDescr
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new media descriptor object to the SDP message.
 *
 * Return Value:
 *      Returns the pointer to the newly created RvSdpMediaDescr object if the
 *      function succeeds or NULL pointer if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      media_type - the type of media (audio, video, data).
 *      port - the port number or special MEGACO values RV_SDP_PORT_IGNORE/RV_SDP_PORT_ANY/RV_SDP_PORT_ATM.
 *      protocol - the protocol used to transport the media, such as RTP/RTCP.
 ***************************************************************************/
PUBLIC SDP_MEDIA_DESCR_HANDLE_T SDP_MsgAddMediaDescr(
            SDP_MSG_HANDLE_T msg_handle,
            SDP_MEDIATYPE_E media_type,
            int32 port,
            SDP_MEDIA_PROTOCOL_E protocol);

/***************************************************************************
 * rvSdpMsgInsertMediaDescr
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new media descriptor object to the SDP message as the copy
 *      of 'descr'.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object where new media descriptor will
 *            be added.
 *      descr - the new media descriptor will be copied from this one.
 ***************************************************************************/
/*PUBLIC SDP_ERR_T rvSdpMsgInsertMediaDescr(
            SDP_MSG_HANDLE_T msg,
            SDP_MEDIA_DESCR_HANDLE_T descr);*/

/***************************************************************************
 * rvSdpMsgRemoveMediaDescr
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the media descriptor object by index.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMsgGetNumOfMediaDescr call.
 ***************************************************************************/
PUBLIC void SDP_MsgRemoveMediaDescr(
            SDP_MSG_HANDLE_T msg_handle,
            int32 index);

/***************************************************************************
 * rvSdpMsgClearMediaDescr
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) all media descriptors set in SDP message.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 ***************************************************************************/
PUBLIC void SDP_MsgClearMediaDescr(
            SDP_MSG_HANDLE_T msg_handle);



/***************************************************************************
* SDP_MediaDescrGetAttrHandle
* ------------------------------------------------------------------------
* General:
*      Get the attribute value in the corresponding description part with the
*      indicated name string. This is used for those description level 
*      attributes which is NOT recognized in the common SDP library.
*
* Return Value:
*      The requested attribute value string
* ------------------------------------------------------------------------
* Arguments:
*      descr_handle - a pointer to the RvSdpMediaDescr object.
*      attr_name_ptr - the attribute name string
***************************************************************************/
PUBLIC char* SDP_MediaDescrGetAttrValueByName(
                                           const SDP_MEDIA_DESCR_HANDLE_T     descr_handle, 
                                           char                               *attr_name_ptr 
                                                           );
/***************************************************************************
* SDP_MsgGetAttrValueByName
* ------------------------------------------------------------------------
* General:
*      Get the attribute value in the session level with the
*      indicated name string. This is used for those session level 
*      attributes which is NOT recognized in the common SDP library.
*
* Return Value:
*      The requested attribute value string
* ------------------------------------------------------------------------
* Arguments:
*      msg_handle - a pointer to the RvSdpMsg object.
*      attr_name_ptr - the attribute name string
***************************************************************************/
PUBLIC char* SDP_MsgGetAttrValueByName(
                                           const SDP_MSG_HANDLE_T     msg_handle, 
                                           char                       *attr_name_ptr 
                                                           );

/*
 *	Setters And Getters
 */

/* format */

/***************************************************************************
 * rvSdpMediaDescrGetNumOfFormats
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of media descriptor codec formats.
 *
 * Return Value:
 *      Number of codec formats defined.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int32 SDP_MediaDescrGetNumOfFormats(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);


/***************************************************************************
* rvSdpMediaDescrGetFormat
* ------------------------------------------------------------------------
* General:
*      Gets a media descriptor format by index.
*
* Return Value:
*      The requested codec format name object.
* ------------------------------------------------------------------------
* Arguments:
*      descr - a pointer to the RvSdpMediaDescr object.
*      index - the index. The index should start at zero (0) and must be smaller
*              than the number of elements in the list. The number of elements
*              in the list is retrieved by correspondent
*              rvSdpMediaDescrGetNumOfFormats() call.
***************************************************************************/
PUBLIC const char* SDP_MediaDescrGetFormat(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int32 index);

/***************************************************************************
 * rvSdpMediaDescrGetMediaTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Gets the media type string of media descriptor.
 *
 * Return Value:
 *      Media type text value.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC const char* SDP_MediaDescrGetMediaTypeStr(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
 * rvSdpMediaDescrAddFormatN
 * ------------------------------------------------------------------------
 * General:
 *      Adds another codec format to the media descriptor object.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      fmt - the name of the format.
 *      len - the 'fmt' length.
***************************************************************************/
/*PUBLIC SDP_ERR_T rvSdpMediaDescrAddFormatN(
					SDP_MEDIA_DESCR_HANDLE_T descr,
					const char* fmt,
					int len);*/

/***************************************************************************
 * rvSdpMediaDescrAddFormat
 * ------------------------------------------------------------------------
 * General:
 *      Adds another codec format to the media descriptor object.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      fmt - the name of the format.
 ***************************************************************************/
/*PUBLIC SDP_ERR_T rvSdpMediaDescrAddFormat(
                    SDP_MEDIA_DESCR_HANDLE_T descr,
                    const char* fmt);*/

/***************************************************************************
 * rvSdpMediaDescrRemoveFormat
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and deallocates) the codec format name by index in the
 *      context of media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMediaDescrGetNumOfConnections call.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrRemoveFormat(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int32 index);

/***************************************************************************
 * rvSdpMediaDescrClearFormat
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) all codec formats set in the media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
/*PUBLIC void rvSdpMediaDescrClearFormat(
                    SDP_MEDIA_DESCR_HANDLE_T descr);*/


/* payload */

/***************************************************************************
 * rvSdpMediaDescrGetNumOfPayloads
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of media descriptor payloads.
 *
 * Return Value:
 *      Number of payloads defined.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int32 SDP_MediaDescrGetNumOfPayloads(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
* rvSdpMediaDescrGetPayload
* ------------------------------------------------------------------------
* General:
*      Gets a media descriptor payload by index.
*
* Return Value:
*      The requested media descriptor payload.
* ------------------------------------------------------------------------
* Arguments:
*      descr - a pointer to the RvSdpMediaDescr object.
*      index - the index. The index should start at zero (0) and must be smaller
*              than the number of elements in the list. The number of elements
*              in the list is retrieved by correspondent
*              rvSdpMediaDescrGetNumOfPayloads() call.
***************************************************************************/
PUBLIC int SDP_MediaDescrGetPayload(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int index);
//#define rvSdpMediaDescrGetPayloadNumber rvSdpMediaDescrGetPayload //later

/***************************************************************************
 * rvSdpMediaDescrAddPayloadNumber
 * ------------------------------------------------------------------------
 * General:
 *      Adds another payload number to the media descriptor object.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      payload - the payload to be added.
***************************************************************************/
PUBLIC SDP_ERR_T SDP_MediaDescrAddPayloadNumber(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int payload);

/***************************************************************************
 * rvSdpMediaDescrRemovePayloadNumber
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the codec payload number by index in the media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMediaDescrGetNumOfPayloads call.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrRemovePayloadNumber(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int32 index);

/***************************************************************************
 * rvSdpMediaDescrClearPayloads
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) all codec payload numbers set in the media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrClearPayloads(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
 * rvSdpMediaDescrGetPort
 * ------------------------------------------------------------------------
 * General:
 *      Gets the media port number.
 *
 * Return Value:
 *      The port number or special MEGACO values RV_SDP_PORT_IGNORE/RV_SDP_PORT_ANY/RV_SDP_PORT_ATM.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int32 SDP_MediaDescrGetPort(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
* rvSdpMediaDescrGetConnection
* ------------------------------------------------------------------------
* General:
*      Gets a first connection object (in the media descriptor context).
*
* Return Value:
*      The first connection object or NULL if there are no connections.
* ------------------------------------------------------------------------
* Arguments:
*      descr - a pointer to the RvSdpMediaDescr object.
***************************************************************************/
PUBLIC SDP_CONNECTION_HANDLE_T SDP_MediaDescrGetConnection(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
* SDP_MediaDescrGetProtocolStr
* ------------------------------------------------------------------------
* General:
*      To get the protocol type descripted in the indicated descprition handle
*
* Return Value:
*      The protocol type descripted in the indicated descprition handle
* ------------------------------------------------------------------------
* Arguments:
*      media_descr_handle - a pointer to the RvSdpMediaDescr object.
***************************************************************************/
PUBLIC SDP_CONNECTION_HANDLE_T SDP_MediaDescrGetProtocolStr(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
 * rvSdpMediaDescrSetPort
 * ------------------------------------------------------------------------
 * General:
 *      Sets the media port number.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      port - the new value of the media port number.
 *             or special MEGACO values RV_SDP_PORT_IGNORE/RV_SDP_PORT_ANY/RV_SDP_PORT_ATM
 ***************************************************************************/
PUBLIC void SDP_MediaDescrSetPort(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int32 port);

/***************************************************************************
 * rvSdpMediaDescrGetNumOfPorts
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of subsequent ports defined for the media descriptor.
 *
 * Return Value:
 *      Number of subsequent ports defined.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int SDP_MediaDescrGetNumOfPorts(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
 * rvSdpMediaDescrSetNumOfPorts
 * ------------------------------------------------------------------------
 * General:
 *      Sets the number of subsequent ports defined for the media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      numPorts - the new number of subsequent ports.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrSetNumOfPorts(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int numPorts);

/* Rtp map */

/***************************************************************************
 * rvSdpMsgGetNumOfRtpMaps
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of RTP map special attributes set in SDP message context.
 *
 * Return Value:
 *      Number of RTP map special attributes set in SDP message context.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr_handle - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int32 SDP_MediaDescrGetNumOfRtpMap(
            const SDP_MEDIA_DESCR_HANDLE_T descr_handle);

/***************************************************************************
 * rvSdpMsgGetFirstRtpMap
 * ------------------------------------------------------------------------
 * General:
 *      Returns the first RTP map object defined in the SDP message. Also sets
 *      the list iterator for the further use.
 *
 * Return Value:
 *      Pointer to the RvSdpRtpMap object or the NULL pointer if there are no
 *      RTP maps defined in the SDP message.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      iter - pointer to RvSdpListIter to be used for further
 *             GetNext calls.
 ***************************************************************************/
/*PUBLIC SDP_RTPMAP_HANDLE_T rvSdpMsgGetFirstRtpMap(
            SDP_MSG_HANDLE_T msg,
            RvSdpListIter* iter);*/

/***************************************************************************
 * rvSdpMsgGetNextRtpMap
 * ------------------------------------------------------------------------
 * General:
 *      Returns the next RTP map object defined in the SDP message. The 'next'
 *      object is defined based on the list iterator state.
 *
 * Return Value:
 *      Pointer to the RvSdpRtpMap object or the NULL pointer if there are no
 *      more RTP maps defined in the SDP message.
 * ------------------------------------------------------------------------
 * Arguments:
 *      iter - pointer to RvSdpListIter set/modified by previous successfull call
 *             to GetFirst/GetNext function.
 ***************************************************************************/
/*PUBLIC SDP_RTPMAP_HANDLE_T rvSdpMsgGetNextRtpMap(
            RvSdpListIter* iter);*/

/***************************************************************************
* SDP_MediaDescrGetRtpMap
* ------------------------------------------------------------------------
* General:
*      Gets an RTP map attribute in the description level
*
* Return Value:
*      The requested RTP map attribute object.
* ------------------------------------------------------------------------
* Arguments:
*      descr_handle - a pointer to the RvSdpMediaDescr object.
*      index - the index. The index should start at zero (0) and must be smaller
*              than the number of elements in the list. The number of elements
*              in the list is retrieved by rvSdpMsgGetNumOfRtpMaps() call.
***************************************************************************/
PUBLIC SDP_RTPMAP_HANDLE_T SDP_MediaDescrGetRtpMap(
            const SDP_MEDIA_DESCR_HANDLE_T descr_handle,
            int32 index);

/***************************************************************************
 * rvSdpMsgAddRtpMap
 * ------------------------------------------------------------------------
 * General:
 *      Adds a new RTP map to the description-level RTP map list.
 *
 * Return Value:
 *      Returns the pointer to the newly created RvSdpRtpMap object if the
 *      function succeeds or NULL pointer if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr_handle - a pointer to the RvSdpMediaDescr object.
 *      payload - an RTP dynamic payload number.
 *      encoding_name - the name of the codec.
 *      rate - the clock rate.
 ***************************************************************************/
PUBLIC SDP_RTPMAP_HANDLE_T SDP_MediaDescrAddRtpMap(
            SDP_MEDIA_DESCR_HANDLE_T descr_handle,
            int payload,
            const char* encoding_name,
            int rate);

/***************************************************************************
 * rvSdpMediaDescrRemoveRtpMap
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the RTP map special attribute object by index.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr_handle - a pointer to the RvSdpMediaDescr object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMsgGetNumOfRtpMaps call.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrRemoveRtpMap(
            SDP_MEDIA_DESCR_HANDLE_T descr_handle,
            int32 index);

/***************************************************************************
 * rvSdpMediaDescrClearRtpMap
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) all RTP map special attributes set in SDP message.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr_handle - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrClearRtpMap(
            SDP_MEDIA_DESCR_HANDLE_T descr_handle);


/***************************************************************************
* SDP_MediaDescrRtpMapGetEncName
* ------------------------------------------------------------------------
* General:
*      To get the encode name described in the a=rtpmap: in the description level
*
* Return Value:
*      the encode name described in the a=rtpmap: in the description level
* ------------------------------------------------------------------------
* Arguments:
*      rtpmap_handle - a pointer to the RvSdpRtpMap object.
***************************************************************************/
PUBLIC const char* SDP_MediaDescrRtpMapGetEncName(const SDP_RTPMAP_HANDLE_T rtpmap_handle);

/***************************************************************************
* SDP_MediaDescrRtpMapGetEncParam
* ------------------------------------------------------------------------
* General:
*      To get the encode parameter describe in the a=rtpmap: in the description level
*      Maybe NULL, it's optional in the rtpmap attribute
* Return Value:
*      the encode name describe in the a=rtpmap: in the description level
* ------------------------------------------------------------------------
* Arguments:
*      rtpmap_handle - a pointer to the RvSdpRtpMap object.
***************************************************************************/
PUBLIC const char* SDP_MediaDescrRtpMapGetEncParam(const SDP_RTPMAP_HANDLE_T rtpmap_handle);

/***************************************************************************
* SDP_MediaDescrRtpMapGetPayload
* ------------------------------------------------------------------------
* General:
*      To get the payload described in the a=rtpmap: in the description level
*
* Return Value:
*      the payload described in the a=rtpmap: in the description level
* ------------------------------------------------------------------------
* Arguments:
*      rtpmap_handle - a pointer to the RvSdpRtpMap object.
***************************************************************************/
PUBLIC uint32 SDP_MediaDescrRtpMapGetPayload(const SDP_RTPMAP_HANDLE_T rtpmap_handle);

/***************************************************************************
* SDP_MediaDescrRtpMapGetClockRate
* ------------------------------------------------------------------------
* General:
*      To get the clock rate described in the a=rtpmap: in the description level
*
* Return Value:
*      the clock rate described in the a=rtpmap: in the description level
* ------------------------------------------------------------------------
* Arguments:
*      rtpmap_handle - a pointer to the RvSdpRtpMap object.
***************************************************************************/
PUBLIC uint32 SDP_MediaDescrRtpMapGetClockRate(const SDP_RTPMAP_HANDLE_T rtpmap_handle);

/* 'rtcp' attribute */

/***************************************************************************
 * rvSdpMediaDescrGetRtcp
 * ------------------------------------------------------------------------
 * General:
 *      Return the rtcp special attribute of the media descriptor or NULL if 
 *      it is not set.
 *
 * Return Value:
 *      Returns the rtcp special attribute.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC SDP_RTCP_ARR_HANDLE_T SDP_MediaDescrGetRtcp(
				SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
 * rvSdpMediaDescrSetRtcp
 * ------------------------------------------------------------------------
 * General:
 *      Sets/modifies the rtcp special attribute of the media descriptor.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      port - the port number of the RTCP stream
 *      net_type - the network type.
 *      addr_type - the address type.
 *      addr - the address, depending on the network type. For example, an IP
 *             address for an IP network, and so on.
***************************************************************************/
PUBLIC SDP_ERR_T SDP_MediaDescrSetRtcp(
				SDP_MEDIA_DESCR_HANDLE_T    media_descr_handle,
                int32                       port,
                SDP_NET_TYPE_E              net_type,
                SDP_ADDRTYPE_E              addr_type,
                const char*                 addr);

/***************************************************************************
 * rvSdpMediaDescrDestroyRtcp
 * ------------------------------------------------------------------------
 * General:
 *      Destroys the rtcp special attribute of the media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrDestroyRtcp(
				SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);


/***************************************************************************
 * rvSdpRtcpGetPort
 * ------------------------------------------------------------------------
 * General:
 *      Gets the media port number.
 *
 * Return Value:
 *      The port number of rtcp stream
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 ***************************************************************************/
PUBLIC int32 SDP_RtcpGetPort(
                    const SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle);

/***************************************************************************
 * rvSdpRtcpSetPort
 * ------------------------------------------------------------------------
 * General:
 *      Sets the port number of the rtcp stream.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpRtcp object.
 *      port - the new value of the rtcp port number.
 ***************************************************************************/
PUBLIC void SDP_RtcpSetPort(
                    SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle,
                    int32 port);

/***************************************************************************
 * rvSdpRtcpGetNetType
 * ------------------------------------------------------------------------
 * General:
 *      Get the rtcp's network type.
 * Return Value:
 *      Returns the rtcp's network type.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 ***************************************************************************/
PUBLIC SDP_NET_TYPE_E SDP_RtcpGetNetType(
                const SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle);

/***************************************************************************
 * rvSdpRtcpSetNetType
 * ------------------------------------------------------------------------
 * General:
 *      Sets the rtcp's field network type.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 *      netType - the rtcp's field network type.
 ***************************************************************************/
PUBLIC void SDP_RtcpSetNetType(
                SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle,
                SDP_NET_TYPE_E netType);


/***************************************************************************
 * rvSdpRtcpGetNetTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Get the rtcp's network type string.
 * Return Value:
 *      Returns the rtcp's network type string.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 ***************************************************************************/
PUBLIC const char* SDP_RtcpGetNetTypeStr(
                SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle);

/***************************************************************************
 * rvSdpRtcpSetNetTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Sets the rtcp's field network type string.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 *      type - the rtcp's field network type string.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_RtcpSetNetTypeStr(
                SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle,
                const char* type_ptr);

/***************************************************************************
 * rvSdpRtcpGetAddressType
 * ------------------------------------------------------------------------
 * General:
 *      Get the rtcp's address type.
 * Return Value:
 *      Returns the rtcp's address type.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 ***************************************************************************/
PUBLIC SDP_ADDRTYPE_E SDP_RtcpGetAddressType(
                const SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle);

/***************************************************************************
 * rvSdpRtcpSetAddressType
 * ------------------------------------------------------------------------
 * General:
 *      Sets the rtcp's field address type.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 *      addrType - the rtcp's field address type.
 ***************************************************************************/
PUBLIC void SDP_RtcpSetAddressType(
                SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle,
                SDP_ADDRTYPE_E addrType);

/***************************************************************************
 * rvSdpRtcpGetAddressTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Get the rtcp's address type string.
 * Return Value:
 *      Returns the rtcp's address type string.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 ***************************************************************************/
/*PUBLIC const char* rvSdpRtcpGetAddressTypeStr(
				SDP_RTCP_ARR_HANDLE_T rtcp);*/

/***************************************************************************
 * rvSdpRtcpSetAddressTypeStr
 * ------------------------------------------------------------------------
 * General:
 *      Sets the rtcp's field address type string.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 *      t - the rtcp's field address type string.
 ***************************************************************************/
/*PUBLIC SDP_ERR_T rvSdpRtcpSetAddressTypeStr(
                SDP_RTCP_ARR_HANDLE_T rtcp,
                const char* t);*/

/***************************************************************************
 * rvSdpRtcpGetAddress
 * ------------------------------------------------------------------------
 * General:
 *      Get the rtcp's address.
 * Return Value:
 *      Returns the rtcp's address.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 ***************************************************************************/
PUBLIC const char* SDP_RtcpGetAddress(
                const SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle);

/***************************************************************************
 * rvSdpRtcpSetAddress
 * ------------------------------------------------------------------------
 * General:
 *      Sets the rtcp's field address.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      rtcp - a pointer to the RvSdpRtcp object.
 *      addr - the rtcp's field address.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_RtcpSetAddress(
                SDP_RTCP_ARR_HANDLE_T sdp_rtcp_handle,
                const char* addr);



/* fmtp attribute */

/***************************************************************************
 * rvSdpMediaDescrGetNumOfFmtp
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of media descriptor fmtp special attributes.
 *
 * Return Value:
 *      Number of fmtp attributes defined.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int32 SDP_MediaDescrGetNumOfFmtp(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
 * rvSdpMediaDescrGetFirstFmtp
 * ------------------------------------------------------------------------
 * General:
 *      Returns the first fmtp special attribute defined in the media descriptor.
 *      Also sets the list iterator for the further use.
 *
 * Return Value:
 *      Pointer to the RvSdpAttribute (of fmtp)  object or the NULL pointer if there
 *      are no fmtp attributes defined in the media descriptor.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      iter - pointer to RvSdpListIter to be used for subsequent
 *             rvSdpMediaDescrGetNextFmtp calls.
 ***************************************************************************/
/*PUBLIC SDP_ATTRIBUTE_HANDLE_T rvSdpMediaDescrGetFirstFmtp(
                    SDP_MEDIA_DESCR_HANDLE_T descr,
                    RvSdpListIter* iter);*/

/***************************************************************************
 * rvSdpMediaDescrGetNextFmtp
 * ------------------------------------------------------------------------
 * General:
 *      Returns the next fmtp special attribute defined in the media descriptor.
 *      The 'next' object is defined based on the list iterator state.
 *
 * Return Value:
 *      Pointer to the RvSdpAttribute (fmtp) object or the NULL pointer if there is no
 *      more fmtp attributes defined in the media descriptor.
 * ------------------------------------------------------------------------
 * Arguments:
 *      iter - pointer to RvSdpListIter set/modified by previous successfull call
 *             to rvSdpMediaDescr(GetFirst/Next)Fmtp function.
 ***************************************************************************/
/*PUBLIC SDP_ATTRIBUTE_HANDLE_T rvSdpMediaDescrGetNextFmtp(
                    RvSdpListIter* iter);*/

/***************************************************************************
* rvSdpMediaDescrGetFmtp
* ------------------------------------------------------------------------
* General:
*      Gets an fmtp special attribute object by index (in media descriptor context).
*
* Return Value:
*      The requested RvSdpAttribute (of fmtp special attribute) pointer.
* ------------------------------------------------------------------------
* Arguments:
*      descr - a pointer to the RvSdpMediaDescr object.
*      index - the index. The index should start at zero (0) and must be smaller
*              than the number of elements in the list. The number of elements
*              in the list is retrieved by correspondent
*              rvSdpMediaDescrGetNumOfFmtp() call.
***************************************************************************/
PUBLIC SDP_ATTRIBUTE_HANDLE_T SDP_MediaDescrGetFmtp(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int32 index);

/***************************************************************************
 * rvSdpMediaDescrAddFmtp
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new fmtp special attribute object to the media descriptor.
 *
 * Return Value:
 *      Returns the pointer to the newly created RvSdpAttribute object if the
 *      function succeeds or NULL pointer if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      val - the value of new fmtp special attribute.
 ***************************************************************************/
PUBLIC SDP_ATTRIBUTE_HANDLE_T SDP_MediaDescrAddFmtp(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    const char* val_ptr);

/***************************************************************************
 * rvSdpMediaDescrRemoveCurrentFmtp
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the fmtp special attribute object pointed
 *      by list iterator in the context  of media descriptor.
 *      The value of iterator becomes undefined after the function call.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      iter - pointer to RvSdpListIter set/modified by previous successfull call
 *             to GetFirst/GetNext function.
 ***************************************************************************/
/*PUBLIC void rvSdpMediaDescrRemoveCurrentFmtp(
            RvSdpListIter* iter);*/


/***************************************************************************
 * rvSdpMediaDescrRemoveFmtp
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the fmtp special attribute object by index in the
 *      context of media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMediaDescrGetNumOfFmtp call.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrRemoveFmtp(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int32 index);

/***************************************************************************
 * rvSdpMediaDescrClearFmtp
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) all fmtp special attributes set in media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrClearFmtp(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);


/* Connection Mode */

/***************************************************************************
 * rvSdpMsgGetConnectionMode
 * ------------------------------------------------------------------------
 * General:
 *      Gets the connection mode of the SDP message or RV_SDPCONNECTMODE_NOTSET
 *      if the correspondent attribute is not set.
 *
 * Return Value:
 *      Returns the connection mode.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 ***************************************************************************/
PUBLIC SDP_CONNECTION_MODE_E SDP_MsgGetConnectionMode(
            const SDP_MSG_HANDLE_T msg_handle);

/***************************************************************************
 * rvSdpMsgSetConnectionMode
 * ------------------------------------------------------------------------
 * General:
 *      Sets/modifies the connection mode of the SDP message.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      msg - a pointer to the RvSdpMsg object.
 *      mode - the new value of connection mode.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_MsgSetConnectionMode(
            SDP_MSG_HANDLE_T msg_handle,
            SDP_CONNECTION_MODE_E mode);



/* Precondition */
/*precondition-type*/
/***************************************************************************
 * rvSdpPreconditionAttrGetPrecondTypeStr
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the Precond-Type as text value of the Precondition attribute ("QOS").
 *          
 * Return Value: 
 *      The requested Precond-Type str of the Precondition attribute.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC const char* SDP_PreconditionAttrGetPrecondTypeStr(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);

/***************************************************************************
 * rvSdpPreconditionAttrGetPrecondType
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the Precond-Type as enum value of the Precondition attribute ("QOS").
 *          
 * Return Value: 
 *      The requested field of the Precondition attribute.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC SDP_PRECOND_TYPE_E SDP_PreconditionAttrGetPrecondType(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);

/***************************************************************************
 * rvSdpPreconditionAttrSetPrecondTypeStr
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the Precond-Type as str value in the Precondition attribute ("QOS").
 *          
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 *      precondTypeStr - the new value of precondition Type as text.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetPrecondTypeStr(
                                               SDP_PRECONDITION_HANDLE_T    precond_handle,
                                               const char  *                precond_type_str_ptr);

/***************************************************************************
 * rvSdpPreconditionAttrSetPrecondType
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the Precond-Type as enum value in the Precondition attribute ("QOS").
 *          
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 *      ePrecondType    - the new value of precondition Type as enum.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetPrecondType(
                                            SDP_PRECONDITION_HANDLE_T   precond_handle,
                                            SDP_PRECOND_TYPE_E          precond_type);

/*precondition-status*/

/***************************************************************************
 * rvSdpPreconditionAttrGetStatusStr
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the precond-Status as text value of the precondition attribute 
 *      ("e2e"/"remote"/"local").
 *          
 * Return Value: 
 *      The requested precond-Status str of the precondition attribute.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC const char* SDP_PreconditionAttrGetStatusStr(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);


/***************************************************************************
 * rvSdpPreconditionAttrGetStatus
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the precond-Status as enum value of the Precondition attribute 
 *      ("e2e"/"remote"/"local").
 *          
 * Return Value: 
 *      The requested enum of the precondition Status.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC SDP_PRECOND_STATUS_TYPE_E SDP_PreconditionAttrGetStatus(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);

/***************************************************************************
 * rvSdpPreconditionAttrSetStatusStr
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the precond-Status as str value in the Precondition attribute ("QOS").
 *          
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 *      precondTypeStr - the new value of precondition status as text.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetStatusStr(
                                               SDP_PRECONDITION_HANDLE_T    precond_handle,
                                               const char  *                precond_status_str_ptr);

/***************************************************************************
 * rvSdpPreconditionAttrSetStatus
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the Precond-Type as enum value in the Precondition attribute ("QOS").
 *          
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr     - a pointer to the RvSdpPreconditionAttr object.
 *      ePrecondType    - the new value of precondition status as enum.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetStatus(
                                            SDP_PRECONDITION_HANDLE_T   precond_handle,
                                            SDP_PRECOND_TYPE_E          precond_status);

/*precondition strength-tag*/
/***************************************************************************
 * rvSdpPreconditionAttrGetStrengthTagStr
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the strength-tag as text value of the precondition attribute 
 *      ("mandatory"/"optional"/"none"/"failure"/"unknown").
 *          
 * Return Value: 
 *      The requested precond strength-tag str of the precondition attribute.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC const char* SDP_PreconditionAttrGetStrengthTagStr(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);

/***************************************************************************
 * rvSdpPreconditionAttrGetStrengthTag
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the strength-tag as enum value of the Precondition attribute 
 *      ("mandatory"/"optional"/"none"/"failure"/"unknown").
 *          
 * Return Value: 
 *      The requested enum of the precondition StrengthTag.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC SDP_PRECOND_STRENGTH_TAG_E SDP_PreconditionAttrGetStrengthTag(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);

/***************************************************************************
 * rvSdpPreconditionAttrSetStrengthTagStr
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the strength-tag as str value in the Precondition attribute 
 *      ("mandatory"/"optional"/"none"/"failure"/"unknown").
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr        - a pointer to the RvSdpPreconditionAttr object.
 *      strengthStr - the new value of precondition strength-tag as text.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetStrengthTagStr(
                                               SDP_PRECONDITION_HANDLE_T    precond_handle,
                                               const char  *                strength_str_ptr);

/***************************************************************************
 * rvSdpPreconditionAttrSetStrengthTag
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the strength-tag as enum value of the Precondition attribute 
 *      ("mandatory"/"optional"/"none"/"failure"/"unknown").
 *          
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 *      eStrengthStr    - the new value of precondition strength-tag as enum.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetStrengthTag(
                                            SDP_PRECONDITION_HANDLE_T       precond_handle,
                                            SDP_PRECOND_STRENGTH_TAG_E      strength_tag);

/*precondition direction-tag*/

/***************************************************************************
 * rvSdpPreconditionAttrGetDirectionTagStr
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the direction-tag as text value of the precondition attribute 
 *      ("send"/"recv"/"none"/"sendrecv").
 *          
 * Return Value: 
 *      The requested precond direction-tag str of the precondition attribute.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC const char* SDP_PreconditionAttrGetDirectionTagStr(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);

/***************************************************************************
 * rvSdpPreconditionAttrGetDirectionTag
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the strength-tag as enum value of the Precondition attribute 
 *      ("send"/"recv"/"none"/"sendrecv").
 *          
 * Return Value: 
 *      The requested enum of the precondition Direction Tag.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr - a pointer to the RvSdpPreconditionAttr object.
 ***************************************************************************/
PUBLIC SDP_PRECOND_DIRECTION_TAG_E SDP_PreconditionAttrGetDirectionTag(
                                               const SDP_PRECONDITION_HANDLE_T precond_handle);

/***************************************************************************
 * rvSdpPreconditionAttrSetDirectionTagStr
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the direction-tag as str value of the Precondition attribute 
 *      ("send"/"recv"/"none"/"sendrecv")..
 *          
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 *      eDirection    - the new value of precondition direction-tag as enum.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetDirectionTagStr(
                                               SDP_PRECONDITION_HANDLE_T    precond_handle,
                                               const char  *                direction_str_ptr);

/***************************************************************************
 * rvSdpPreconditionAttrSetDirectionTag
 * ------------------------------------------------------------------------
 * General: 
 *      Sets the direction-tag as enum value of the Precondition attribute 
 *      ("send"/"recv"/"none"/"sendrecv")..
 *          
 * Return Value: 
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the 
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      precondAttr    - a pointer to the RvSdpPreconditionAttr object.
 *      eDirection    - the new value of precondition direction-tag as enum.
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_PreconditionAttrSetDirectionTag(
                                            SDP_PRECONDITION_HANDLE_T   precond_handle,
                                            SDP_PRECOND_DIRECTION_TAG_E direction_tag);

/***************************************************************************
 * rvSdpMediaDescrAddPrecondition
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new RvSdpPreconditionAttr object at media
 *      descriptor level.
 *
 * Return Value:
 *      Returns the pointer to the newly created RvSdpPreconditionAttr object if the
 *      function succeeds or NULL pointer if the function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      precondName  - the precondition attribute type.
 *      typeStr      - the precondition-type str.
 *      statusStr    - the precondition-status str.
 *      strengthStr  - the precondition-strength str.
 *      directionStr - the precondition-direction str.
 ***************************************************************************/
PUBLIC SDP_PRECONDITION_HANDLE_T SDP_MediaDescrAddPrecondition(
                                                 SDP_MEDIA_DESCR_HANDLE_T   media_descr_handle,
                                                 SDP_PRECOND_NAME_E         precond_name, 
                                                 const char*                type_str_ptr, 
                                                 const char*                status_str_ptr,
                                                 const char*                strength_str_ptr,
                                                 const char*                direction_str_ptr);

/***************************************************************************
 * rvSdpMediaDescrRemovePrecondition
 * ------------------------------------------------------------------------
 * General:
 *	Removes the precondition attribute by index. That is if the value of 
 *  'precondName' is RV_SDP_PRECOND_NAME_CURRENT_ATTR and is index '2' the 
 *  function will remove the third 'curr' precondition attribute appearing in 
 *  media descriptor. If called with RV_SDP_PRECOND_NAME_ALL as precondName's
 *  value the index relates to all precondition attributes.
 * 
 * Return Value:
 *      None
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      precondName  - the precondition attribute type.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMediaDescrGetNumOfPrecondition() call.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrRemovePrecondition(
                             SDP_MEDIA_DESCR_HANDLE_T   media_descr_handle,
                             SDP_PRECOND_NAME_E         precond_name,
							 int32                      index
                             );

/***************************************************************************
 * rvSdpMediaDescrClearPrecondition
 * ------------------------------------------------------------------------
 * General:
 *	Clears all precondition attributes of the type set by 'precondName' appearing
 *  in the media descriptor object.
 * 
 * Return Value:
 *      None
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      precondName  - the precondition attribute type.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrClearPrecondition(
					 SDP_MEDIA_DESCR_HANDLE_T   media_descr_handle,
					 SDP_PRECOND_NAME_E         precond_name);

/***************************************************************************
 * rvSdpMediaDescrGetNumOfPrecondition
 * ------------------------------------------------------------------------
 * General: 
 *      Gets the number of precondition special attributes in media descriptor level.
 *          
 * Return Value: 
 *      Number of precondition attributes defined.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int32 SDP_MediaDescrGetNumOfPrecondition(
                                             SDP_MEDIA_DESCR_HANDLE_T   media_descr_handle,
                                             SDP_PRECOND_NAME_E         precond_name);

/***************************************************************************
 * rvSdpMediaDescrGetFirstPrecondition
 * ------------------------------------------------------------------------
 * General:
 *      Returns the first precondition special attribute defined in the media descriptor.
 *      Also sets the list iterator for the further use.
 *
 * Return Value:
 *      Pointer to the RvSdppreconditionAttr  object or the NULL pointer if there are no
 *      precondition attriubutes defined in the media descriptor.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      iter - pointer to RvSdpListIter to be used for subsequent
 *             rvSdpMediaDescrGetNextPrecondition calls.
 ***************************************************************************/
/*PUBLIC SDP_PRECONDITION_HANDLE_T SDP_MediaDescrGetFirstPrecondition(
                                                       SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                                                       SDP_PRECOND_NAME_E       precond_name,
                                                       RvSdpListIter  * iter);*/

/***************************************************************************
 * rvSdpMediaDescrGetNextPrecondition
 * ------------------------------------------------------------------------
 * General:
 *      Returns the next precondition attribute defined in the media descriptor.
 *      The 'next' object is defined based on the list iterator state.
 *
 * Return Value:
 *      Pointer to the RvSdppreconditionAttr object or the NULL pointer if there are no
 *      more precondition attributes defined in the media descriptor.
 * ------------------------------------------------------------------------
 * Arguments:
 *      iter - pointer to RvSdpListIter set/modified by previous successfull call
 *             to rvSdpMediaDescrGetFirstPrecondition function.
 ***************************************************************************/
/*PUBLIC SDP_PRECONDITION_HANDLE_T SDP_MediaDescrGetNextPrecondition(
                                             SDP_PRECOND_NAME_E precond_name,
                                             RvSdpListIter* iter);*/

/***************************************************************************
 * rvSdpMediaDescrRemoveCurrentPrecondition
 * ------------------------------------------------------------------------
 * General:
 *      Removes the precondition attribute pointed by current iterator value.
 *      The value of iterator becomes undefined after the function call. 
 * Return Value:
 *      None
 * 
 * ------------------------------------------------------------------------
 * Arguments:
 *      iter - pointer to RvSdpListIter set/modified by previous successfull call
 *             to rvSdpMediaDescrGetFirstPrecondition function.
 ***************************************************************************/
/*PUBLIC SDP_PRECONDITION_HANDLE_T SDP_MediaDescrRemoveCurrentPrecondition(
                                             RvSdpListIter* iter);*/

/***************************************************************************
 * rvSdpMediaDescrGetPrecondition
 * ------------------------------------------------------------------------
 * General:
 *      Gets a precondition attribute object by index (in media descriptor context).
 *
 * Return Value:
 *      The requested RvSdpPreconditionAttr pointer.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMediaDescrGetNumOfPrecondition() call.
 ***************************************************************************/
PUBLIC SDP_PRECONDITION_HANDLE_T SDP_MediaDescrGetPrecondition(
                    SDP_MEDIA_DESCR_HANDLE_T    media_descr_handle,
                    SDP_PRECOND_NAME_E          precond_name,
                    int32                       index);


/* bandwidth */
/***************************************************************************
 * rvSdpMediaDescrGetNumOfBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Gets the number of media descriptor bandwidth fields.
 *
 * Return Value:
 *      Number of bandwidths defined.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC int32 SDP_MediaDescrGetNumOfBandwidth(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
 * rvSdpMediaDescrGetFirstBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Returns the first bandwidth object defined in the media descriptor.
 *      Also sets the list iterator for the further use.
 *
 * Return Value:
 *      Pointer to the RvSdpBandwidth  object or the NULL pointer if there are no
 *      bandwidths defined in the media descriptor.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      iter - pointer to RvSdpListIter to be used for subsequent
 *             rvSdpMediaDescrGetNextBandwidth calls.
 ***************************************************************************/
/*PUBLIC SDP_BANDWIDTH_HANDLE_T rvSdpMediaDescrGetFirstBandwidth(
                    SDP_MEDIA_DESCR_HANDLE_T descr,
                    RvSdpListIter* iter);*/

/***************************************************************************
 * rvSdpMediaDescrGetNextBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Returns the next bandwidth object defined in the media descriptor.
 *      The 'next' object is defined based on the list iterator state.
 *
 * Return Value:
 *      Pointer to the RvSdpBandwidth object or the NULL pointer if there is no
 *      more bandwidths defined in the media descriptor.
 * ------------------------------------------------------------------------
 * Arguments:
 *      iter - pointer to RvSdpListIter set/modified by previous successfull call
 *             to rvSdpMediaDescrGetFirstBandwidth/rvSdpMediaDescrGetNextBandwidth
 *             functions.
 ***************************************************************************/
/*PUBLIC SDP_BANDWIDTH_HANDLE_T rvSdpMediaDescrGetNextBandwidth(
                    RvSdpListIter* iter);*/

/***************************************************************************
* rvSdpMediaDescrGetBandwidth
* ------------------------------------------------------------------------
* General:
*      Gets a first bandwidth object at the media descriptor level.
*
* Return Value:
*      The first bandwidth object or NULL if there are no bandwidths.
* ------------------------------------------------------------------------
* Arguments:
*      descr - a pointer to the RvSdpMediaDescr object.
***************************************************************************/
PUBLIC SDP_BANDWIDTH_HANDLE_T SDP_MediaDescrGetBandwidth(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);

/***************************************************************************
* rvSdpMediaDescrGetBandwidthByIndex
* ------------------------------------------------------------------------
* General:
*      Gets a bandwidth object by index at the media descriptor level.
*
* Return Value:
*      The requested bandwidth object.
* ------------------------------------------------------------------------
* Arguments:
*      descr - a pointer to the RvSdpMediaDescr object.
*      index - the index. The index should start at zero (0) and must be smaller
*              than the number of elements in the list. The number of elements
*              in the list is retrieved by correspondent
*              rvSdpMediaDescrGetNumOfBandwidths() call.
***************************************************************************/
PUBLIC SDP_BANDWIDTH_HANDLE_T SDP_MediaDescrGetBandwidthByIndex(
                    const SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    int32 index);

/***************************************************************************
 * rvSdpMediaDescrAddBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new bandwidth object at media descriptor level.
 *
 * Return Value:
 *      Pointer to the added RvSdpBandwidth  object or the NULL pointer if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      bwtype - bandwidth type, such as Conference Total (CT) or Application-Specific
 *               Maximum (AS).
 *      b - bandwidth value in kilobits per second (kbps).
 ***************************************************************************/
PUBLIC SDP_BANDWIDTH_HANDLE_T SDP_MediaDescrAddBandwidth(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    const char* bw_type_str_ptr,
                    int b);

/***************************************************************************
 * rvSdpMediaDescrSetBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Adds the new bandwidth object at media descriptor level.
 *
 * Return Value:
 *      Returns RV_SDPSTATUS_OK if the function succeeds, or an error code if the
 *      function fails.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      bwtype - bandwidth type, such as Conference Total (CT) or Application-Specific
 *               Maximum (AS).
 *      b - bandwidth value in kilobits per second (kbps).
 ***************************************************************************/
PUBLIC SDP_ERR_T SDP_MediaDescrSetBandwidth(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle,
                    const char* bw_type_str_ptr,
                    int b);

/***************************************************************************
 * rvSdpMediaDescrRemoveCurrentBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the bandwidth object pointed by list iterator.
 *      The value of iterator becomes undefined after the function call.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      iter - pointer to RvSdpListIter set/modified by previous successfull call
 *             to GetFirst/GetNext function.
 ***************************************************************************/
/*PUBLIC void rvSdpMediaDescrRemoveCurrentBandwidth(
            RvSdpListIter* iter);*/

/***************************************************************************
 * rvSdpMediaDescrRemoveBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) the bandwidth object by index in the
 *      context of media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 *      index - the index. The index should start at zero (0) and must be smaller
 *              than the number of elements in the list. The number of elements
 *              in the list is retrieved by correspondent
 *              rvSdpMediaDescrGetNumOfBandwidths call.
 ***************************************************************************/
/*PUBLIC void rvSdpMediaDescrRemoveBandwidth(
                    SDP_MEDIA_DESCR_HANDLE_T descr,
                    int32 index);*/

/***************************************************************************
 * rvSdpMediaDescrClearBandwidth
 * ------------------------------------------------------------------------
 * General:
 *      Removes (and destructs) all bandwidths set in media descriptor.
 *
 * Return Value:
 *      None.
 * ------------------------------------------------------------------------
 * Arguments:
 *      descr - a pointer to the RvSdpMediaDescr object.
 ***************************************************************************/
PUBLIC void SDP_MediaDescrClearBandwidth(
                    SDP_MEDIA_DESCR_HANDLE_T media_descr_handle);



//---old
PUBLIC SDP_MSG_HANDLE_T SDP_CreateParserHandler(uint32 buf_len, char *buf_ptr);
PUBLIC void SDP_FreeParserHandler(SDP_MSG_HANDLE_T sdp_handle);













#endif
