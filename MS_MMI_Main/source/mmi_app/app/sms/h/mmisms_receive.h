/*************************************************************************
 ** File Name:      mmisms_receive.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about receiving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/8/23      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMS_RECEIVE_H_
#define _MMISMS_RECEIVE_H_

#include "sci_types.h"
#include "mmisms_order.h"
#include "mmisms_export.h"
#include "mmisms_app.h"

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "mmisms_order.h"
//#include "mmisms_listbuf.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
#if defined(JAVA_SUPPORT_MYRIAD)
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
 //BEGIN @huangns 20111103
#define MYRIAD_PHONE_NUM_MAX_SIZE       (32)      /** phone number length */
#define MYRIAD_MAX_LONG_SMS_NUM         (30)
#define MYRIAD_SMS_CONTENT_MAX_LEN      (MMISMS_MAX_DEF_SPLIT_LEN*MMISMS_SPLIT_MAX_NUM)
#define MYRIAD_SMS_INVAILD_INDEX        0xFFFF
//END @huangns_20111103

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//BEGIN @huangns 20111103  add for sms recevie
typedef enum _MYRIAD_SMS_ERROR_ID_E
{
  MYRIAD_SMS_ERROR_NO = 0,    //no error.
  MYRIAD_SMS_ERROR_REPEAT,    //receive repeated sms.
  MYRIAD_SMS_ERROR_ALLOC,   //alloc memory fail.
}MYRIAD_SMS_ERROR_ID_E;
/** SMS data structure */
typedef struct SCISmsMessage_s
{
  uint8  addr[MYRIAD_PHONE_NUM_MAX_SIZE]; /*** If this is an incoming Short Message: the source address  * If this is an outgoing Short Message: the destination address  */
  uint16 srcPort;       /** The source port number. Unused for CBS */
  uint16 dstPort;       /** The destination port number for Sms or Message ID for CBS*/
  uint8 encoding;      /** The message encoding :  * 0 = 7-bit alphabet   * 1 = 8-bit alphabet     * 2 = 8-bit binary   * 3 = 16-bit alphabet   */
  uint64 timestamp;      /** Timestamp (ms since 1970). Unused for CBS */
  int32 seq_id;         /**   * Sequence id of received message or -1 for non-fragmented message.   * In case the target APIs already defragment, behave as non-fragmented.    *   */
  int32 seq_idx;         /**   * Index in sequence of received message for SMS or inc page number for CBS. 1-based.   *  1 for non-fragmented messages.   * In case the target APIs already defragment, behave as non-fragmented.   *   */
  int32 seq_len;        /** * Sequence length of received message for SMS or number of pages for CBS.    * 1 for non-fragmented messages.    * In case the target APIs already defragment, behave as non-fragmented.    *    */
  uint16 length;         /** The length of the payload (data) only */
  uint8 * data;         /* more allocated as required */
}SCISmsMessage;

typedef struct
{
  SCISmsMessage    myriad_sms[MYRIAD_MAX_LONG_SMS_NUM];
}MYRIAD_LONG_SMS_T;

typedef struct _MYRIAD_SMS_CONTENT_T
{
  uint16 length;                                    // the length of data
  uint8 data[MYRIAD_SMS_CONTENT_MAX_LEN+1];    // the data of user inputting
}MYRIAD_SMS_CONTENT_T;
PUBLIC void* MMIAPIMYRIAD_InitLongsmsStruct(void); //MYRIAD_LONG_SMS_T*

#endif /* JAVA_SUPPORT_MYRIAD */

typedef struct
{
    MN_DUAL_SYS_E dual_sys;
    MN_SMS_STORAGE_E     storage;
    MN_SMS_RECORD_ID_T   record_id;
    APP_SMS_USER_DATA_T  user_data;
    MN_SMS_MT_SMS_T mt_sms;
    MMISMS_MO_MT_TYPE_E mo_mt_type; 
#ifdef MMI_SMS_CHAT_SUPPORT     
    MMISMS_ORDER_ID_T cur_chat_order_id;
#endif
}MMISMS_NEW_SMS_INFO_T;

#if defined(ZDT_SMS_SUPPORT)
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
 //BEGIN @huangns 20111103
#define ZDT_PHONE_NUM_MAX_SIZE       (32)      /** phone number length */
#define ZDT_MAX_LONG_SMS_NUM         (30)
#define ZDT_SMS_CONTENT_MAX_LEN      (MMISMS_MAX_DEF_SPLIT_LEN*MMISMS_SPLIT_MAX_NUM)
#define ZDT_SMS_INVAILD_INDEX        0xFFFF
//END @huangns_20111103

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//BEGIN @huangns 20111103  add for sms recevie
typedef enum _ZDT_SMS_ERROR_ID_E
{
  ZDT_SMS_ERROR_NO = 0,    //no error.
  ZDT_SMS_ERROR_REPEAT,    //receive repeated sms.
  ZDT_SMS_ERROR_ALLOC,   //alloc memory fail.
}ZDT_SMS_ERROR_ID_E;
/** SMS data structure */
typedef struct ZdtSmsMessage_s
{
  uint8  addr[ZDT_PHONE_NUM_MAX_SIZE]; /*** If this is an incoming Short Message: the source address  * If this is an outgoing Short Message: the destination address  */
  uint16 srcPort;       /** The source port number. Unused for CBS */
  uint16 dstPort;       /** The destination port number for Sms or Message ID for CBS*/
  uint8 encoding;      /** The message encoding :  * 0 = 7-bit alphabet   * 1 = 8-bit alphabet     * 2 = 8-bit binary   * 3 = 16-bit alphabet   */
  uint64 timestamp;      /** Timestamp (ms since 1970). Unused for CBS */
  int32 seq_id;         /**   * Sequence id of received message or -1 for non-fragmented message.   * In case the target APIs already defragment, behave as non-fragmented.    *   */
  int32 seq_idx;         /**   * Index in sequence of received message for SMS or inc page number for CBS. 1-based.   *  1 for non-fragmented messages.   * In case the target APIs already defragment, behave as non-fragmented.   *   */
  int32 seq_len;        /** * Sequence length of received message for SMS or number of pages for CBS.    * 1 for non-fragmented messages.    * In case the target APIs already defragment, behave as non-fragmented.    *    */
  uint16 length;         /** The length of the payload (data) only */
  uint8 * data;         /* more allocated as required */
  uint16 max_length;
}ZdtSmsMessage;

typedef struct
{
  ZdtSmsMessage    zdt_sms[ZDT_MAX_LONG_SMS_NUM];
}ZDT_LONG_SMS_T;

typedef struct _ZDT_SMS_CONTENT_T
{
  uint16 length;                                    // the length of data
  uint8 *data;//[ZDT_SMS_CONTENT_MAX_LEN+1];    // the data of user inputting
}ZDT_SMS_CONTENT_T;
PUBLIC void* MMIAPIZDT_InitLongsmsStruct(void); //ZDT_LONG_SMS_T*

#endif /* ZDT_SMS_SUPPORT */

/*****************************************************************************/
//     Description : to handle the signal APP_MN_SMS_IND
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSMSMTInd( 
                                  DPARAM param 
                                  );

/*****************************************************************************/
//     Description : to handle the signal APP_MN_SMS_STATUS_REPORT_IND
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSMSReportInd( 
                              DPARAM    param    //IN:
                              );



/*****************************************************************************/
//     Description : Set readed SMS to window
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetReadedSms2Window(
                                MMISMS_BOX_TYPE_E    box_type        //IN:
                                );

/*****************************************************************************/
// Description :Is Smart Message
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsSmartMessage( 
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                APP_SMS_USER_DATA_T *user_data_ptr,
                                                                                MN_SMS_STORAGE_E storage,
                                                                                MN_SMS_RECORD_ID_T record_id
                                                                                );


#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
/*****************************************************************************/
// Description :Is Voice Mail Message
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsVoiceMailMessage( 
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                APP_SMS_USER_DATA_T *user_data_ptr,
                                                                                MN_SMS_DCS_T dcs,
                                                                                MN_SMS_STORAGE_E storage,
                                                                                MN_SMS_RECORD_ID_T record_id
                                                                                );
#endif

/*****************************************************************************/
// Description :
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMIVC_SMS_SEND_TYPE_E MMISMS_IsMtForVcard( 
                                                 MN_DUAL_SYS_E         dual_sys,                                  
                                                 APP_SMS_USER_DATA_T   *user_data_ptr);

/*****************************************************************************/
//     Description : to set the user data head
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_VcardOrVcalInitUserDataHead(
                                    uint8 max_num,        //IN:
                                    uint8 seq_num,        //IN: the seq number (1 ~ 8)
                                    MN_SMS_USER_DATA_HEAD_T  *data_head_ptr  //OUT: the user data head
                                    );

/*****************************************************************************/
// Description : operate MT ind as for Vcard or Vcal
// Global resource dependence :
// Author:rong.gu
// Note: 
/*****************************************************************************/
LOCAL MMISMS_OPER_ERR_E MMISMS_RecvVcardOrVcalAsNormalSms(
                                     MN_DUAL_SYS_E         dual_sys, 
                                     MN_SMS_STORAGE_E       storage, 
                                     MN_SMS_RECORD_ID_T	    record_id,
                                     MN_SMS_MT_SMS_T        sms_t,
                                     APP_SMS_USER_DATA_T   *sms_ptr,   //vcard,vcal
                                     MMIVC_SMS_SEND_TYPE_E send_type
                                     );

/*****************************************************************************/
// Description : operate MT ind as for Vcard
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC MMIVC_SMS_SEND_TYPE_E MMISMS_OperateMtIndForVcardOrVcal(
                                                               MN_DUAL_SYS_E         dual_sys,                                  
                                                               APP_SMS_USER_DATA_T   *user_data_ptr,
                                                               MN_SMS_STORAGE_E       storage,
                                                               MN_SMS_RECORD_ID_T     record_id,
                                                               MN_SMS_MT_SMS_T        sms_t);

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : get scroll msg record id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetScrollmsgRecordId(void);
#endif

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : to show the content of message
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ShowMsgContent(
                                  MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                  uint16                    msg_len,            //IN:
                                  MMISMS_READ_MSG_T                     *read_msg_ptr,       //IN:
                                  MN_SMS_TIME_STAMP_T        *sc_time_ptr,        //IN:
                                  MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                  MMISMS_R8_LANGUAGE_E    language,
                                  BOOLEAN                    is_chat_mode,
                                  BOOLEAN                    is_need_time,
                                  MMI_STRING_T                *string_ptr            //OUT:
                                  );
#endif

#if defined MMI_STATUSBAR_SCROLLMSG_SUPPORT || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : MMISMS_GetmsgResult
//  Global resource dependence : 
//  Author:
//  Note:ªÒ»°∂Ã–≈read msg
/*****************************************************************************/
PUBLIC MMISMS_READ_MSG_T MMISMS_GetmsgResult(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_RECEIVE_H_
