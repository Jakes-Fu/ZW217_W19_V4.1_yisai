/****************************************************************************
** File Name:      mmimms_net.h                                            *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_NET_H
#define _MMIMMS_NET_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "mmisms_export.h"
#include "mmimms_edit.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_MAX_RETRY_TIMES      3
#define MMIMMS_MAX_RSP_RETRY_TIMES  1

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIMMS_STATUS_NONE,
    MMIMMS_STATUS_NETLINKING,
    MMIMMS_STATUS_NETLINK_SUCCESS,
    MMIMMS_STATUS_NETCONNET_SUCCESS,    	
    MMIMMS_STATUS_DATA_TRANSFERING,
    MMIMMS_STATUS_PDP_DEACTIVE_IND,     //网络突然断开
    MMIMMS_STATUS_MAX 
}MMIMMS_STATUS_E;

typedef enum
{
	MMIMMS_SENDRECV_NONE,
    MMIMMS_SENDRECV_SENDING,        	
    MMIMMS_SENDRECV_RECVING,
    MMIMMS_SENDRECV_SENDRPT,
    MMIMMS_SENDRECV_SENDRESPONSE,
    MMIMMS_SENDRECV_MAX 
}MMIMMS_SENDRECV_TYPE_E;

typedef enum
{
	MMIMMS_RECV_NEWMMS,     //未上报而下载成功的新彩信
    MMIMMS_RECV_OLDMMS,   	//手动下载的彩信
    MMIMMS_RECV_MAX 
}MMIMMS_RECV_TYPE_E;

typedef struct MMIMMS_MSG_SEND_END_T_tag
{
    int32		    record_id;
    MMIMMS_ERROR_E  send_error;
    BOOLEAN		    is_saved;
}MMIMMS_MSG_SEND_END_T;

typedef struct MMIMMS_MSG_RECV_END_T_tag
{
    int32		   record_id;
    MMIMMS_ERROR_E recv_error;
    MMIMMS_RECV_TYPE_E recv_type;
}MMIMMS_MSG_RECV_END_T;



typedef struct MMIMMS_PROGRESS_T_tag
{
    int32                       record_id;
    uint32                      current_size;
    uint32                      total_size;   
    MMIMMS_STATUS_E		        status;
    MMIMMS_SENDRECV_TYPE_E      type;
    MN_DUAL_SYS_E               dual_sys;
    int32                       immediate_recv;
}MMIMMS_PROGRESS_T;

typedef MMIMMS_PROGRESS_T MMIMMS_MSG_DATA_SENT_T;       

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : active receive and send module 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ActiveRecvSendModule(void);

/*****************************************************************************/
//  Description : send new mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SendNewMMS(
                                        MN_DUAL_SYS_E dual_sys, 
                                        MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                        );

/*****************************************************************************/
//  Description : send old mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SendOldMMS(
                                        MN_DUAL_SYS_E     dual_sys,
                                        int32				record_id
                                        );

/*****************************************************************************/
//  Description : send read report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SendReadReport(
                                            MN_DUAL_SYS_E     dual_sys, 
                                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                            );

/*****************************************************************************/
//  Description : receive mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_ReceiveMMS(
                              MN_DUAL_SYS_E     dual_sys,
                              int32				record_id
                              );

/*****************************************************************************/
//  Description : send new mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_PROGRESS_T MMIMMS_GetActiveMMSProgress(void);

/*****************************************************************************/
//  Description : whether mms is downloading/sending or not 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsInProcess(int32 record_id);

/*****************************************************************************/
//  Description : 取消当前彩信的收发
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_CancelSendRecvById(int32 record_id);

/*****************************************************************************/
//  Description : mms send cancel 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: is_manual: false PDP自动断开;  true 手动取消 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_CancelSendRecv(BOOLEAN is_manual);

/*****************************************************************************/
//  Description : 停止彩信收发模块,清空收发队列
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ClearRecvSendModule(void);

/*****************************************************************************/
//  Description : MMS is sending or receiving 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsSendingOrRecving(void);

/*****************************************************************************/
//  Description : reject mms 彩信存储空间满时回应服务器 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_ResponceMMS(
                                            MN_DUAL_SYS_E     dual_sys, 
                                            MMS_NOTIFICATION_IND_MSG_T *msg_ptr,
                                            int respValue
                                            );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_NET_H*/


