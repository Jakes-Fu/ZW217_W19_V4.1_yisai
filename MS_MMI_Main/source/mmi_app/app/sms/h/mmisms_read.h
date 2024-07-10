/*************************************************************************
 ** File Name:      mmisms_read.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about reading sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/8/23      liming.deng      Create.                              *
*************************************************************************/
#ifndef _MMISMS_READ_H_
#define _MMISMS_READ_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmk_type.h"
#include "mmisms_export.h"
#include "mn_type.h"
#include "mmisms_nv.h"
#include "mmisms_app.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*****************************************************************************/
//     Description : to handle the PS signal of APP_MN_READ_SMS_CNF
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_HandleReadSmsCnf(DPARAM param);

/*****************************************************************************/
//     Description : to handle the PS signal of APP_MN_READ_SMS_CNF when the user 
//                is reading SMS
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_HandleUserReadSMSCnf( 
                                 DPARAM param 
                                 );

/*****************************************************************************/
//     Description : to read the SMS called by window
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_AppReadSms( MMISMS_ORDER_ID_T sort_id);

/*****************************************************************************/
//     Description : to init SMS in SIM by order after received the SMS_READY_IND
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitSmsList(void);

/*****************************************************************************/
//     Description : get ref num from head
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetRefNumFromUserHead(
                                                      MN_SMS_USER_DATA_HEAD_T    *user_data_head_ptr    //IN:
                                                      );

#if 0
/*****************************************************************************/
//     Description : to check whether the status  is to be read
//    Global resource dependence : none
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadPointedSMS( void);
#endif

/*****************************************************************************/
//     Description : 通过电话得到电话所属姓名
//    Global resource dependence : none
//  Author: liming.deng
//    Note: 如果在电话号码本中找到电话所属姓名，返回true。
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetNameByNumberFromPB(
                                            uint8        *number_ptr, 
                                            MMI_STRING_T *name_ptr,
                                            uint16       max_name_len
                                            );
   
/*****************************************************************************/
//     Description : get contact information from phone_book
//    Global resource dependence :  
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/    
PUBLIC void MMISMS_GetContactInfo(
                                  uint32         handle,
                                  MMI_STRING_T    *contact_info_ptr,    //OUT:
                                  uint16        max_len                //IN:
                                  );

/*****************************************************************************/
//     Description : to get the user number form SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: call by need MMISMS_GetSMSUseNumber(, *number, ) number[arr_size - 1][max_char_num + 1]
/*****************************************************************************/
PUBLIC uint8  MMISMS_GetSMSUseNumber(
                              uint8          *num_arr_ptr,
                              uint8          arr_size,
                              uint8          max_char_num // must < MMISMS_MAX_USERNUMBER_LEN
                              );

/*****************************************************************************/
//     Description : to get the origin time and date form MT SMS
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_GetOriginTimeAndDate(
                                 wchar    *date_ptr,        //OUT:
                                 wchar    *time_ptr        //OUT:
                                 );

/*****************************************************************************/
//     Description : to handle the signal APP_MN_READ_SMS_CNF
//    Global resource dependence : g_mmisms_order
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_HandleSMSOrder( 
                               DPARAM param 
                               );

/*****************************************************************************/
//     Description : to get number of the error sms
//    Global resource dependence : g_mmisms_global
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint8  MMISMS_GetReadErrSmsNum(MN_DUAL_SYS_E dual_sys, BOOLEAN is_nv);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : Get the sms sort timer Id.
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetSmsSortTimerId(void);

/*****************************************************************************/
//  Description :Insert the mms/wap/ota into sms list. 
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSortTimer(uint8 timer_id, uint32 param);

#endif /* Reduce Code size */

/*****************************************************************************/
//     Description : decode and modify sms data
//    Global resource dependence : 
//  Author: liming.deng
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_DecodeUserHeadAndContent(
                                     MN_DUAL_SYS_E           dual_sys,           //IN:
                                     BOOLEAN                 user_head_is_exist, //IN:
                                     MN_SMS_DCS_T            *dcs_ptr_t,         //IN:
                                     MN_SMS_USER_DATA_T      *user_data_ptr_t,     //IN:
                                     APP_SMS_USER_DATA_T     *decode_data_ptr     //OUT:
                                     );

#ifdef SEARCH_SUPPORT
/*****************************************************************************/
//     Description : handle search sms
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleSearchSms(MMI_STRING_T* search_string,BOOLEAN is_mmsonly);

                                        
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_HandleSearchNextSms(void);

/*****************************************************************************/
//     Description : read search sms
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SearchMain(
								 MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id
								 );
/*****************************************************************************/
//     Description : stop async search sms
//    Global resource dependence : 
//  Author: tao.xue
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_StopSearchSms(void);

/*****************************************************************************/
//     Description : read search sms
//    Global resource dependence : 
//  Author: fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SearchMMS( MMISMS_LINKED_ORDER_UNIT_T *cur_unit_id);

#endif

/*****************************************************************************/
//     Description : Init Other Message List(MMS/OTA/Push...)
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitOtherMessageList(void);

/*****************************************************************************/
//  Description : to read the SMS box called by window for copy
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppCopyMsgBox(
                                              MMISMS_BOX_TYPE_E    type,    //IN:
                                              uint16* msg_type  //out
                                              );

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : to read the SMS chat box
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_ChatSetMTGlobalReadMsg(
                                          MN_DUAL_SYS_E           dual_sys,
                                          APP_SMS_USER_DATA_T        *user_data_ptr,    //IN:
                                          MN_SMS_STORAGE_E        storage,        //IN:
                                          MN_SMS_RECORD_ID_T        record_id,        //IN:
                                          MN_SMS_ALPHABET_TYPE_E    alphabet_type,    //IN:
                                          MN_SMS_TIME_STAMP_T        *time_stamp_ptr,//IN:
                                          MN_CALLED_NUMBER_T        *org_addr_ptr,    //IN:
                                          MMISMS_MO_MT_TYPE_E mo_mt_type
                                         );
/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence : g_mmisms_global
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_CalcChatItemSplitLenth(uint32  index);

/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_SetChatLongSmsItemContent(
                                               uint32  index,
                                               MMISMS_ORDER_ID_T order_id,
                                               MMI_STRING_T orig_content,
                                               MMI_STRING_T new_insert_content,
                                               MMI_STRING_T *string_ptr //out
                                               );
#endif

/*****************************************************************************/
//     Description : to assemble the SMS 
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
PUBLIC void MMISMS_AssembleSmsToGlobal(
                               MN_DUAL_SYS_E              dual_sys,
                               MMISMS_READ_TYPE_E          msg_type,                //IN:
                               APP_SMS_USER_DATA_T        *new_user_data_ptr,    //IN: the user data of new message 
                               MN_CALLED_NUMBER_T          *address_ptr,            //IN:
                               MN_SMS_ALPHABET_TYPE_E     msg_dcs,                //IN: the dcs of new message
                               MMISMS_READ_MSG_T        *read_msg_ptr,
                               BOOLEAN is_need_lost_prompt,
                               BOOLEAN is_need_last_lost_prompt
                               );

/*****************************************************************************/
//     Description : get max num from head
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetMaxNumFromUserHead(
                                  MN_SMS_USER_DATA_HEAD_T    *user_data_head_ptr    //IN:
                                  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_READ_H_
