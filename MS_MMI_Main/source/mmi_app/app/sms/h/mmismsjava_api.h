/****************************************************************************
** File Name:      mmismsjava_api.h                                                *
** Author:                                                                 *
** Date:           03/1/2007                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2/2007         kelly,li        Create
** 
****************************************************************************/
#ifndef  _MMISMSJAVA_API_H_    
#define  _MMISMSJAVA_API_H_   

#include "sci_types.h"
#ifndef WIN32
//#include "jbedCoreInstance.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MSISDN_LEN_MAX        32

#define MMISMS_JAVA_MAX_UCS2_BYTE_LEN        134
#define MMISMS_JAVA_MAX_UCS2_SPLIT_LEN        128

#define MMISMS_JAVA_MAX_DEF_BYTE_LEN        154
#define MMISMS_JAVA_MAX_DEF_SPLIT_LEN        147

#define MMISMS_JAVA_MAX_8BIT_BYTE_LEN        134
#define MMISMS_JAVA_MAX_8BIT_SPLIT_LEN        128
#if 0
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct CPL_RegPort_s 
{     
     int32 port;
     int32 info; //cbs & sms
     struct CPL_RegPort_s *next;              
} CPL_RegPort;

struct CPL_SmsMessage_s {

    struct CPL_SmsMessage_s * prev_ptr;  //first member must a pointer used for structuring a queue
    
    struct CPL_SmsMessage_s * next;  //first member must a pointer used for structuring a queue

    uint8 msisdn[MSISDN_LEN_MAX];// in msisdn international format, i.e. with leading +, with one digit in ascii per byte. Unused for CBS.
     

    /** Length of address of received message. For CBS it is unused.  */
    int    msisdnLen;

    /** Payload of received message.  */
    uint8 *data;

    /** Payload length of received message. */
    int dataLength;

    /** Source port number of received message. For CBS it is unused.
     * (-1 if not applicable)
     * 
     */
    int    srcPort;

    /** Destination port number of received message for SMS or message ID for CBS.
     * (-1 if not applicable)
     * 
     */
    int    dstPort;

    /** Encoding of received message. 
     *
     * One of sms_Protocol.ENC_GSM_7BIT, sms_Protocol.ENC_8BIT_BIT, or sms_Protocol.ENC_UCS_2
     *  
     */
    int    type;

    /**
     * Sequence id of received message or -1 for non-fragmented message.
     * In case the target APIs already defragment, behave as non-fragmented.
     * 
     */
    int    seq_id;

    /**
     * Index in sequence of received message for SMS or inc page number for CBS. 1-based.
     *  1 for non-fragmented messages.
     * In case the target APIs already defragment, behave as non-fragmented.
     *  
     */
    int    seq_idx;

    /**
     * Sequence length of received message for SMS or number of pages for CBS.
     * 1 for non-fragmented messages.
     * In case the target APIs already defragment, behave as non-fragmented.
     * 
     */
    int    seq_len;

    /** Timestamp of received message (ms since 1970). Unused for CBS
     *  */
#ifdef _WIN32
    uint64                  timestamp;
#else
    unsigned long long      timestamp;
#endif

    /**
     * Message type flag. Equal to 1 if CBS message, 0 if SMS.
     * 
     */
    int isCbs;
};



/**
 * Opaque SMS message structure.
 *
 * The first entry must be a pointer sized field, to allow for chaining
 * a queue. This field is read and written by the PushRegistry.
 */
typedef struct CPL_SmsMessage_s CPL_SmsMessage;

/*****************************************************************************/
//     Description : create new struct for port info 
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
//  Register an SMS port.
//  注册后的端口号将和java应用绑定。以后发往这个端口的短消息都将是这个应用需要的。
//  @param port: Port number.
//  @param info: flag word with context info (one of JBED_WMA_INFO_* )确定类型是cbs还是sms
//  @return CPL_WMA_SUCCESS on success, CPL_WMA_FAILURE on failure.
/*****************************************************************************/
PUBLIC int32 CPL_sms_register(int32 port, int32 info);

/*****************************************************************************/
//     Description : create new struct for port info
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
//  Deregister an SMS or CBS port.
//  @param info flag word with context info (one of JBED_WMA_INFO_* )
//  @param port Port number.
/*****************************************************************************/
PUBLIC void CPL_sms_deregister(int32 port, int32 info);

/*****************************************************************************/
//     Description : Get sms info from the pool by port
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
//  Poll an SMS or CBS port.
//  The returned message will either be discarded via  or
//  queued inside the push registry for later retrieval by the
//  application. In that case it can be retrieved from the push
//  registry using push_checkout_sms as an opaque pointer.
//  @see  JbniV_javax_microedition_io_SmsConnectionThread_nReadMessage
//  @see  push_checkout_sms 
//  @param port Port number.
//  @return an SMS message or NULL.
/*****************************************************************************/
PUBLIC  CPL_SmsMessage* CPL_sms_poll(int32 port);

/*****************************************************************************/
//     Description : create new struct for port info
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
// Get the source or destination number for a message.  
// @param message SMS message.
// @return the msisdn string.
/*****************************************************************************/
PUBLIC const char * CPL_sms_msisdn(CPL_SmsMessage *message_info_ptr);

/*****************************************************************************/
//     Description : Shutdown the sms device if it is up and running.
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
//  CPL_wma_shutdown
// Shutdown the sms device if it is up and running.
// This is called from the VM finalization code.
/*****************************************************************************/
PUBLIC void CPL_wma_shutdown(void);

/*****************************************************************************/
//     Description : Discard an SMS message
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
//  CPL_sms_free
//  @param message SMS message.
/*****************************************************************************/
PUBLIC void CPL_sms_free(CPL_SmsMessage *message);

/*****************************************************************************/
//     Description : send sms for java
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
// Native method to send an SMS message.
//uint8_t *messageData = (uint8_t *)data->data;
// @param msisdn Message destination.
// @param srcPort Source port.
// @param dstPort Destination port.
// @param type Payload data encoding.
// @param data Payload data.
// @param dataLen Payload data length.
// @return CPL_WMA_SUCCESS when successed or CPL_WMA_FAILURE otherwise.
/*****************************************************************************/
#ifndef WIN32
PUBLIC int CPL_sms_send(
                        STRING_INSTANCE msisdn,
                         int32        srcPort,
                         int32        dstPort,
                         int32        type,
                         BYTEARRAY    data,
                         int32_t      dataLen
                         ); 
#endif
/*****************************************************************************/
//     Description : add new java sms to pool
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_AddJavaNewSms(CPL_SmsMessage *sms_info);

/*****************************************************************************/
//     Description : if the port is for java
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsJavaPort(int32 port);

/*****************************************************************************/
//     Description : get port info
//    Global resource dependence : 
//  Author:kelly.li
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_GetPortInfo(int32 port, int32 *info_ptr);
#endif//#if 0
#endif