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

#ifndef _MMISMS_DELETE_H_
#define _MMISMS_DELETE_H_

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
//     Description : to handle the signal APP_MN_UPDATE_SMS_STATUS_CNF
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note: in the operation of delete and update, the system will return the same signal.
/*****************************************************************************/
PUBLIC void MMISMS_HandleUpdateSMSCnf(
                              DPARAM  param
                              );

/*****************************************************************************/
//     Description : to delete the SMS box called by window
//    Global resource dependence : g_mmisms_global
//                               g_mmisms_order
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_OPER_ERR_E MMISMS_AppDeleteMsgBox(
                                                MMISMS_BOX_TYPE_E    type,
                                                uint16* msg_type,
                                                MMISMS_FOLDER_TYPE_E folder_type
                                                );

/*****************************************************************************/
//     Description : to delete the pointed SMS
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_DeletePointedSMS(
                                    uint16                    pos_index //IN: start from 0
                                    );
        
/*****************************************************************************/
//     Description : to delete the pointed SMS
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeletePointedSMSById( 
                                MMISMS_ORDER_ID_T    order_pos
                                );

/*****************************************************************************/
//     Description : to delete request to MN
//    Global resource dependence : g_mmisms_global
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeleteReqToMN(
                            MMISMS_BOX_TYPE_E        type,        //IN:
                            MMISMS_ORDER_ID_T       order_id,    //IN:
                            MMISMS_OPER_STATUS_E    oper_status    //IN:
                            );
#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//     Description : get del index in chat read
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note: handle the normal move operation
/*****************************************************************************/
PUBLIC uint16 MMISMS_GetDelIndex(void);

/*****************************************************************************/
//     Description : get del index in chat read
//    Global resource dependence : g_mmisms_global
//  Author:fengming.huang
//    Note: handle the normal move operation
/*****************************************************************************/
PUBLIC void MMISMS_SetDelIndex(uint16 index);

/*****************************************************************************/
//  Description :get chat win index
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMISMS_FindChatWinIndex(   //“—”–∂Ã–≈’“index
                                          uint32 *index_ptr,
                                          MMISMS_ORDER_ID_T order_id
                                         );
#endif

#ifdef MMISMS_REPLACE_CONCATENATED_SMS_SUPPORT
/*****************************************************************************/
//  Description : delet replace sms
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_DeleteReplaceSms(DPARAM param);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_DELETE_H_


