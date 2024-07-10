/*************************************************************************
 ** File Name:      mmisms_delete.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about deleting sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/8/23      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMS_EDIT_H_
#define _MMISMS_EDIT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmisms_order.h"
#include "mmisms_app.h"



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
//     Description : to handle ps signal of APP_MN_EDIT_SMS_CNF
//    Global resource dependence : g_mmisms_global
//                               
//  Author:louis.wei
//    Note:
/*****************************************************************************/

PUBLIC void MMISMS_HandleEditSMSCnf(
                            DPARAM param
                            );

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InterceptMsgContent(                                //RETURN: TRUE: is ucs2
                                  uint8    seq_num,                //IN:
                                  uint8    max_len,                //IN:
                                  uint8    *intercpet_len_ptr,        //OUT:
                                  wchar    *intercpet_content_ptr,    //OUT:
                                  MMISMS_SEND_T *send_info_ptr
                                  );

/*****************************************************************************/
//     Description : intercept class0 msg
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InterceptClass0MsgContent(                        //RETURN: TRUE: is ucs2
                                          uint8    seq_num,                //IN:
                                          uint8    max_len,                //IN:
                                          uint8    *intercpet_len_ptr,        //OUT:
                                          wchar    *intercpet_content_ptr    //OUT:
                                          );

/*****************************************************************************/
//     Description : to get current time 
//    Global resource dependence : 
//  Author:liming.deng
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISMS_GetCurTime(void);


/*****************************************************************************/
//     Description : to save the content which user input
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetMessageContent( 
                                     uint16    length,
                                     wchar     *content,
                                     BOOLEAN   is_need_packet,
                                     MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr
                                     );

/*****************************************************************************/
//     Description : to check the SMS is ready
//    Global resource dependence : 
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_InterceptReadMsgContent(                        //RETURN: TRUE: is ucs2
                                              MMISMS_OPER_STATUS_E oper_status,
                                              uint8    seq_num,
                                              uint8    max_len,                //IN:
                                              uint8    *intercpet_len_ptr,        //OUT:
                                              wchar    *intercpet_content_ptr    //OUT:
                                              );

/*****************************************************************************/
//    Description : MMISMS_IsContainEmoji
//    Global resource dependence :
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsContainEmoji(
    uint16 *unicode, 
    uint32 unicode_length,
    MMISMS_EMOJI_DISPLAY_CONTENT_T * content_ptr
    );

/*****************************************************************************/
//    Description : MMISMS_ConvertToWstr
//    Global resource dependence :
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_ConvertToWstr(UTF16 ch, wchar *data);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_DELETE_H_
