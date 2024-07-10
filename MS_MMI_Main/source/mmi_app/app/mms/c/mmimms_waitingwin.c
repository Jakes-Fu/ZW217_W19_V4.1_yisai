/****************************************************************************
** File Name:      mmimms_waitingwin.c                                     *
** Author:         minghu.mao                                              *
** Date:           05/26/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2009        minghu.mao       create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmimms_id.h"
#include "mmi_appmsg.h"
//#ifdef MMS_SUPPORT
#include "mmipub.h"
#include "mmicc_export.h"
#include "mmk_timer.h"
#include "guires.h"
#include "guilcd.h"
#include "mmitheme_pos.h"

#include "mmimms_text.h"
#include "mms_image.h"
#include "mmimms_export.h"
#include "mmimms_internal.h"
#include "mmimms_net.h"
#include "mmicountedtime_export.h"
#include "mmialarm_export.h"
#include "mmi_default.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
extern MMIMMS_SEND_RECV_GLOBLEE_T g_mms_send_recv_add_data;
uint32 g_mms_cur_size = 0;
uint32 g_mms_total_size = 0;
uint32 g_mms_send_record_id = 0;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :down load mms waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSDownloadingWin(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                              );

/*****************************************************************************/
//  Description :send mms waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSSendingWin(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                              );

/*****************************************************************************/
//  Description :read mms waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
//LOCAL MMI_RESULT_E  MMIMMS_HandleReadMMSWaitingWin(
//                                                MMIMMS_WINDOW_ID_E win_id, 
//                                                MMI_MESSAGE_ID_E msg_id, 
//                                                DPARAM param
//                                              );

#if 0
/*****************************************************************************/
//  Description :display recv and send progress
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSMMI_DisplaySendRecvProgress(uint32 pos, uint32 totalsize, MMI_WIN_ID_T win_id);
#endif

/*****************************************************************************/
//  Description :display recv and send size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSMMI_DisplaySendRecvSize(uint32 now_size, uint32 totalsize, BOOLEAN is_fresh, MMI_WIN_ID_T win_id);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :down load mms waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSDownloadingWin(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E err_code                    = MMI_RESULT_TRUE;
    MMIMMS_PROGRESS_T *progress_ptr          = PNULL;
    MMIMMS_PROGRESS_T progress_info          = {0};
    MMIMMS_MSG_RECV_END_T *recv_end_info_ptr = PNULL;
    MMIMMS_ERROR_E op_error                  = MMIMMS_ERROR_MAX;
   // MMISMS_ORDER_INFO_T *mms_list_info_ptr   = PNULL;
    // static BOOLEAN exit_down_from_red        = FALSE;
    // uint16 temp_size                         = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
        MMIMMS_WinInit(DOWNLOAD_WIN_ENTRY);        
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, TXT_HIDE, FALSE);
        break;
    case MSG_FULL_PAINT:
        //显示彩信下载进度条        
        if ((g_mms_total_size > 0) && (g_mms_cur_size > 0))
        {
            MMSMMI_DisplaySendRecvSize(g_mms_cur_size, g_mms_total_size, FALSE, win_id);           
        }
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    case  MSG_MMS_RECV_RETRYING:
        //SCI_TRACE_LOW:"HandleMMSDownloadingWin: MSG_MMS_SEND_RETRYING send size g_mms_cur_size=%d, g_mms_total_size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_136_112_2_18_2_39_3_426,(uint8*)"dd", g_mms_cur_size,g_mms_total_size);
        MMSMMI_DisplaySendRecvSize(g_mms_cur_size, g_mms_total_size, TRUE, win_id);        
        break;
    case MSG_GET_FOCUS:
        progress_info = MMIMMS_GetActiveMMSProgress();
        if (g_mms_send_record_id == (uint32)progress_info.record_id)
        {
            if (progress_info.current_size > g_mms_cur_size)
            {
                g_mms_cur_size = progress_info.current_size;
            }            
            g_mms_total_size = progress_info.total_size;
        }
        break;
    case MSG_LOSE_FOCUS:
        progress_info = MMIMMS_GetActiveMMSProgress();
        if (g_mms_send_record_id == (uint32)progress_info.record_id)
        {
            g_mms_cur_size = progress_info.current_size;
            g_mms_total_size = progress_info.total_size;
        }
        break;
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        // 发送窗口关闭时, 弹出被延迟的闹钟
        if (!MMIAPIALM_CheckAndStartDelayedAlarm())
        {
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
        }
        MMIMMS_WinExit(DOWNLOAD_WIN_ENTRY);
        break;
    case MSG_MMS_DATA_RECEIVED:
        //收到彩信数据，进行解码及对sms的各项进行保存
        progress_ptr = g_mms_send_recv_add_data.progress_ptr;
        if (PNULL != progress_ptr)
        {
            if (progress_ptr->current_size > g_mms_cur_size)
            {
                g_mms_cur_size = progress_ptr->current_size;
            }            
            g_mms_total_size = progress_ptr->total_size;            
            //SCI_TRACE_LOW:"HandleMMSDownloadingWin:download size g_mms_cur_size=%d, g_mms_total_size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_181_112_2_18_2_39_3_427,(uint8*)"dd", g_mms_cur_size,g_mms_total_size);
            if (g_mms_total_size > 0 && g_mms_cur_size > 0 && MMK_IsFocusWin(win_id))
            {
                MMSMMI_DisplaySendRecvSize(g_mms_cur_size, g_mms_total_size, TRUE, win_id);
            }
        }
        break;
    case MSG_MMS_RECV_END:
        //下载完成，可以进行彩信保存
        recv_end_info_ptr = g_mms_send_recv_add_data.recv_ptr;
        if (PNULL == recv_end_info_ptr)
        {
            break;      
        }
        g_mms_cur_size = 0;
        g_mms_total_size = 0;
        op_error = recv_end_info_ptr->recv_error;    
        switch(op_error) 
        {
        case MMIMMS_NO_ERROR:
            //暂时不要提示接收成功，保存彩信，根据结果提示            
            MMK_CloseWin(win_id);
       //     mms_list_info_ptr = MMIMMS_GetListItemByID(recv_end_info_ptr->record_id);
//            if (mms_list_info_ptr != PNULL)
//            {
//                mms_list_info_ptr->mo_mt_type = MMISMS_MT_TO_BE_READ;
//                MMIAPISMS_UpdateMsgStatusInOrder(mms_list_info_ptr);
//                MMIMMS_SaveMMSListFile();
//            }
            MMIMMS_SetPreviewMMSDetailFlag(FALSE);
            MMIAPIMMS_ReadMMS(recv_end_info_ptr->record_id);            
            break;
        case MMIMMS_ERROR_FAIL:        
        case MMIMMS_ERROR_NETLINK_FAILED:
        case MMIMMS_ERROR_MESSAGE_NOT_FOUND:
        case MMIMMS_ERROR_RECV_NEED_AUTH:
        case MMIMMS_ERROR_INVALIDPERIOD:
        case MMIMMS_ERROR_INVALID_URL:
        case MMIMMS_ERROR_NO_SPACE:
            //提示接收失败
            MMSMMI_OpenMMSAlertWin(TXT_COMMON_RECEIVE_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            MMK_CloseWin(win_id);
            break;
#ifdef FDN_SUPPORT            
        case MMIMMS_ERROR_NETLINK_FDN_ONLY:
            //提示FDN only
            MMSMMI_OpenMMSAlertWin(TXT_CC_FDN_ONLY, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            MMK_CloseWin(win_id);
            break;
#endif
        case MMIMMS_ERROR_CANCEL:
            MMSMMI_OpenMMSAlertWin(TXT_MMS_CANCEL_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
            MMK_CloseWin(win_id);
            break;
        default:
            MMSMMI_OpenMMSAlertWin(TXT_COMMON_RECEIVE_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            MMK_CloseWin(win_id);
            break;
        }        
        break;
    case MSG_MMS_SAVE_END:
        //保存保存完成，关闭下载窗口        
        break;
    default :
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :send mms waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSSendingWin(
                                        MMIMMS_WINDOW_ID_E win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E err_code                    = MMI_RESULT_TRUE;
    MMIMMS_PROGRESS_T *progress_ptr          = PNULL;
    MMIMMS_PROGRESS_T progress_info          = {0};
    MMIMMS_MSG_SEND_END_T *send_end_info_ptr = PNULL;
    MMIMMS_ERROR_E op_error                  = MMIMMS_ERROR_MAX;
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;   
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIMMS_WinInit(SENDING_WIN_ENTRY);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_HIDE, TXT_NULL, STXT_CANCEL, FALSE);
        break;

    case MSG_FULL_PAINT:
        //显示彩信发送进度条,进度为0
        //SCI_TRACE_LOW:"HandleMMSSendingWin: MSG_FULL_PAINT send size g_mms_cur_size=%d, g_mms_total_size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_278_112_2_18_2_39_3_428,(uint8*)"dd", g_mms_cur_size,g_mms_total_size);
        if (g_mms_cur_size > 0)
        {
            MMSMMI_DisplaySendRecvSize(g_mms_cur_size, g_mms_total_size, FALSE, win_id);
        } 
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);               
        break;

    case MSG_MMS_SEND_RETRYING:
        //SCI_TRACE_LOW:"HandleMMSSendingWin: MSG_MMS_SEND_RETRYING send size g_mms_cur_size=%d, g_mms_total_size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_287_112_2_18_2_39_3_429,(uint8*)"dd", g_mms_cur_size,g_mms_total_size);
        MMSMMI_DisplaySendRecvSize(g_mms_cur_size, g_mms_total_size, TRUE, win_id);
        break;

    case MSG_GET_FOCUS:
        progress_info = MMIMMS_GetActiveMMSProgress();
        if (g_mms_send_record_id == (uint32)progress_info.record_id)
        {
            if (progress_info.current_size > g_mms_cur_size)
            {
                g_mms_cur_size = progress_info.current_size;
            }            
            g_mms_total_size = progress_info.total_size;
        }
        if (g_mms_cur_size > 0)
        {
            MMSMMI_DisplaySendRecvSize(g_mms_cur_size, g_mms_total_size, FALSE, win_id);
        }
        break;

    case MSG_LOSE_FOCUS:
        progress_info = MMIMMS_GetActiveMMSProgress();
        if (g_mms_send_record_id == (uint32)progress_info.record_id)
        {
            g_mms_cur_size = progress_info.current_size;
            g_mms_total_size = progress_info.total_size;
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //打开取消发送的询问窗口
        MMIPUB_OpenQueryWinByTextId(STXT_EXIT,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
        break;

    case MSG_PROMPTWIN_OK:
        //取消 发送
        MMK_CloseWin(quer_win_id);      
        if (0 != g_mms_send_record_id)
        {
            MMIMMS_CancelSendRecvById(g_mms_send_record_id);
        }
        if(!MMIAPIMMS_IsProcessMMSById(g_mms_send_record_id))
        {
            MMK_CloseWin(win_id);
        }
        //MMSMMI_OpenMMSAlertWin(TXT_MMS_CANCEL_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS);        
        //MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(quer_win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMIMMS_WinExit(SENDING_WIN_ENTRY);
        g_mms_send_record_id = 0;
        // 发送窗口关闭时, 弹出被延迟的闹钟
        if (!MMIAPIALM_CheckAndStartDelayedAlarm())
        {
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
        }
        break;

    case MSG_MMS_NETLINK_SUCCESS:
        //pdp激活成功
        break;
    case MSG_MMS_NETCONNECT_SUCCESS:
        //网关连接或socket连接成功
        break;
    case MSG_MMS_DATA_SENT:
        //发送彩信数据
        progress_ptr = g_mms_send_recv_add_data.progress_ptr;
        if (PNULL != progress_ptr)
        {
            if (progress_ptr->current_size > g_mms_cur_size)
            {
                g_mms_cur_size = progress_ptr->current_size;
            }            
            g_mms_total_size = progress_ptr->total_size;
            if ((0 < g_mms_total_size) && (g_mms_cur_size < g_mms_total_size))//显示发送大小
            {
                MMSMMI_DisplaySendRecvSize(g_mms_cur_size, g_mms_total_size, TRUE, win_id);
            }
            //SCI_TRACE_LOW:"HandleMMSSendingWin:send size g_mms_cur_size=%d, g_mms_total_size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_378_112_2_18_2_39_3_430,(uint8*)"dd", g_mms_cur_size,g_mms_total_size);
        }
        break;
    case MSG_MMS_SEND_END:
        //发送完成，可以把彩信保存进入发件箱,根据发送结果提示用户
        MMIDEFAULT_SetBackLight(TRUE);
        MMK_CloseWin(quer_win_id);
        g_mms_cur_size = 0;
        g_mms_total_size = 0;
        send_end_info_ptr = g_mms_send_recv_add_data.send_ptr;
        if (PNULL == send_end_info_ptr)
        {
            break;
        } 
        //如果是发件箱，需要改变状态存储在已发信箱，如果是草稿箱也需要存储在已发信箱
        op_error = send_end_info_ptr->send_error;  
        //SCI_TRACE_LOW:"HandleMMSSendingWin error id = %d, is_saved = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_394_112_2_18_2_39_3_431,(uint8*)"dd", op_error, send_end_info_ptr->is_saved);
        switch(op_error) 
        {
        case MMIMMS_NO_ERROR:
#ifndef MMI_MMS_MINI_SUPPORT                       
            //提示发送成功，是否保存彩信
            if (send_end_info_ptr->is_saved)
            {//提示发送成功
                MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_SUCCEED, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                
            }
            else 
            {//提示发送成功彩信未保存
                MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_SUCCEED, TXT_MMI_SAVEFAILED, IMAGE_PUBWIN_SUCCESS);               
            }
#endif            
            break;
        case MMIMMS_ERROR_FAIL:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_NETLINK_FAILED:
        case MMIMMS_ERROR_NETLINK_DISCONNECT:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_COMMON_LINK_FAILED, IMAGE_PUBWIN_WARNING);            
            break;
#ifdef FDN_SUPPORT            
        case MMIMMS_ERROR_NETLINK_FDN_ONLY:
            //提示FDN only
            MMSMMI_OpenMMSAlertWin(TXT_CC_FDN_ONLY, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            MMK_CloseWin(win_id);
            break;
#endif            
        case MMIMMS_ERROR_SERVICE_DENIED:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_SERVICE_DENIED, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_FORMAT_CORRUPT:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_FORMAT_CORRUPT, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_ADDRESS_UNRESOLVED:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_ADDRESS_UNRESOLVED, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_MESSAGE_NOT_FOUND:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_MESSAGE_NOT_FOUND, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_SERVER_BUSY:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_COMMON_TEXT_SERVER_BUSY, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_NOT_ACCEPTED:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_NOT_ACCEPTED, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_UNSUPORTED_MESSAGE:
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_MMS_TEXT_UNSUPORTED_MESSAGE, IMAGE_PUBWIN_WARNING);            
             break;
        case MMIMMS_ERROR_CANCEL:
            if (send_end_info_ptr->is_saved)
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_CANCEL_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
            }  
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_CANCEL_SUCCESS, TXT_MMI_SAVEFAILED, IMAGE_PUBWIN_SUCCESS);
            }
            break;
#ifdef DATA_ROAMING_SUPPORT
        case MMIMMS_ERROR_DATA_ROAMING_DISABLE:
            MMSMMI_OpenMMSAlertWin(TXT_DATA_ROAMING_DISABLED, TXT_NULL, IMAGE_PUBWIN_WARNING);
            break;
#endif
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :create mms download waiting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMSMMI_CreateDownloadWaitingWin(MN_DUAL_SYS_E dual_sys, uint32 record_id)
{
    MMIMMS_PROGRESS_T progress_info  = {0};    
    
    //创建下载等待窗口
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T sim_name_send = {0};
    MMISET_SIM_NAME_T sim_name = {0};
    MMI_STRING_T downloading_str = {0};

    progress_info = MMIMMS_GetActiveMMSProgress();
    if (progress_info.record_id == (int32)record_id)
    {
        g_mms_cur_size = progress_info.current_size;
        g_mms_total_size = progress_info.total_size;
    }
    else
    {
        g_mms_cur_size = 0;
        g_mms_total_size = 0;
    }

    SCI_TRACE_LOW("MMSMMI_CreateDownloadWaitingWin recordId=%d, curSize=%d, totalSize=%d", progress_info.record_id, g_mms_cur_size, g_mms_total_size);
    SCI_MEMSET(&downloading_str, 0, sizeof(downloading_str));
    MMI_GetLabelTextByLang(TXT_MMS_DOWNLOADING, &downloading_str);
    //SCI_TRACE_LOW:"MMSMMI_CreateDownloadWaitingWin dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_494_112_2_18_2_39_4_432,(uint8*)"d", dual_sys);
    SCI_MEMSET(&sim_name_send, 0, sizeof(sim_name_send));
    sim_name = MMIAPISET_GetSimName(dual_sys);
    sim_name_send.wstr_len = sim_name.wstr_len;
    sim_name_send.wstr_ptr = sim_name.wstr_arr;  
    
    MMIPUB_OpenWaitWin(2,&sim_name_send,&downloading_str,PNULL,MMIMMS_DOWNLOADING_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)HandleMMSDownloadingWin);
#else
    
    MMI_STRING_T downloading_str = {0};
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

    progress_info = MMIMMS_GetActiveMMSProgress();
    if (progress_info.record_id == (int32)record_id)
    {
        g_mms_cur_size = progress_info.current_size;
        g_mms_total_size = progress_info.total_size;
    }
    else
    {
        g_mms_cur_size = 0;
        g_mms_total_size = 0;
    }
    
    MMK_CloseWin(quer_win_id);    
    SCI_MEMSET(&downloading_str, 0, sizeof(downloading_str));
    MMI_GetLabelTextByLang(TXT_MMS_DOWNLOADING, &downloading_str);    
    
    MMIPUB_OpenWaitWin(1,&downloading_str,PNULL,PNULL,MMIMMS_DOWNLOADING_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)HandleMMSDownloadingWin);
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description :create mms sending waiting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMSMMI_CreateSendingWaitingWin(MN_DUAL_SYS_E dual_sys, uint32 record_id)
{
    MMIMMS_PROGRESS_T progress_info  = {0}; 

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_STRING_T sim_name_send = {0};
    MMISET_SIM_NAME_T sim_name = {0};
    MMI_STRING_T downloading_str = {0};
    
    g_mms_send_record_id = record_id;
    progress_info = MMIMMS_GetActiveMMSProgress();
    if (progress_info.record_id == (int32)record_id)
    {
        g_mms_cur_size = progress_info.current_size;
        g_mms_total_size = progress_info.total_size;
    }
    else 
    {
        g_mms_cur_size = 0;
        g_mms_total_size = 0;
    }
    
    SCI_MEMSET(&downloading_str, 0, sizeof(downloading_str));
    MMI_GetLabelTextByLang(TXT_SENDING, &downloading_str);
    //SCI_TRACE_LOW:"MMSMMI_CreateSendingWaitingWin dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_557_112_2_18_2_39_4_433,(uint8*)"d", dual_sys);
    SCI_MEMSET(&sim_name_send, 0, sizeof(sim_name_send));
    sim_name = MMIAPISET_GetSimName(dual_sys);
    sim_name_send.wstr_len = sim_name.wstr_len;
    sim_name_send.wstr_ptr = sim_name.wstr_arr; 
    MMIPUB_OpenWaitWin(2,&sim_name_send,&downloading_str,PNULL,MMIMMS_SENDING_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_OKCANCEL,(MMIPUB_HANDLE_FUNC)HandleMMSSendingWin);
    MMIMMS_SetIsMmsRetry(FALSE);
#else
    MMI_STRING_T downloading_str = {0};

    progress_info = MMIMMS_GetActiveMMSProgress();
    if (progress_info.record_id == (int32)record_id)
    {
        g_mms_cur_size = progress_info.current_size;
        g_mms_total_size = progress_info.total_size;
    }
    else 
    {
        g_mms_cur_size = 0;
        g_mms_total_size = 0;
    }    
    g_mms_send_record_id = record_id;

    SCI_MEMSET(&downloading_str, 0, sizeof(downloading_str));
    MMI_GetLabelTextByLang(TXT_SENDING, &downloading_str);
    MMIPUB_OpenWaitWin(1,&downloading_str,PNULL,PNULL,MMIMMS_SENDING_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_OKCANCEL,(MMIPUB_HANDLE_FUNC)HandleMMSSendingWin);
    MMIMMS_SetIsMmsRetry(FALSE);
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : for the case if close ps or switch on flight mode while sending win open
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC void MMSMMI_DestroyOperateWaitingWin(void)
{
    if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
    {
        MMK_CloseWin(MMIMMS_SENDING_WIN_ID);
        MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_NULL, IMAGE_PUBWIN_WARNING);
    }    
}

#if 0
/*****************************************************************************/
//  Description :display recv and send progress
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSMMI_DisplaySendRecvProgress(uint32 pos, uint32 totalsize, MMI_WIN_ID_T win_id)
{
    GUI_RECT_T  rect                 = {0};
    GUI_POINT_T         dis_point    = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 
    int16 progress_bar_len           = 0;
    int16 progress_bar_width         = 2;
    int16 progress_bar_left          = 0;
    int16 progress_bar_top           = 0;
    int16 progress_bar_right         = 0;
    int16 progress_bar_buttom        = 0;
    uint16 mainscreen_height          = 0;
    uint16 mainscreen_width           = 0;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    //SCI_TRACE_LOW:"mmimms_main.c, MMIMMS_DisplaySendRecvProgress(), pos = %d, totalsize = %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_611_112_2_18_2_39_4_434,(uint8*)"dd", pos, totalsize);
    
    if (0 == totalsize)
    {
        return;
    }    
    if( !MMK_IsFocusWin(MMIMMS_SENDING_WIN_ID) && !MMK_IsFocusWin(MMIMMS_DOWNLOADING_WIN_ID))
    {
        return;
    }    
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return;
    } 
    
    progress_bar_len = MMIMMS_PROGRESS_BAR_LEN;
    progress_bar_left = (mainscreen_width - progress_bar_len)/2;
    progress_bar_top = MMIMMS_PROGRESS_BAR_TOP*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.65h
    progress_bar_right = progress_bar_left + progress_bar_len;
    progress_bar_buttom = progress_bar_top + progress_bar_width;

    if (0 == pos)
    {
        //display progress bar
        dis_point.x = progress_bar_left;
        dis_point.y = progress_bar_top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_MMS_PROGRESS_BAR_ICON,
            &lcd_dev_info);
    }
    else
    {
        //display scroll bar
        rect.left = progress_bar_left;
        rect.top = progress_bar_top;
        rect.right = progress_bar_left + (progress_bar_len * pos)/totalsize;/*lint !e737*/
        rect.bottom = progress_bar_buttom;
        
        //防止滚动条长度超过进度条
        if (progress_bar_right < rect.right)
        {
            rect.right = progress_bar_right;
        }
        
        //display progress bar
        dis_point.x = progress_bar_left;
        dis_point.y = progress_bar_top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_MMS_PROGRESS_BAR_ICON,
            &lcd_dev_info);
        LCD_FillRect(&lcd_dev_info, rect, MMIMMS_PROGRESS_BAR_COLOR);
    }
}
#endif

/*****************************************************************************/
//  Description :display recv and send size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSMMI_DisplaySendRecvSize(uint32 now_size, uint32 totalsize, BOOLEAN is_fresh, MMI_WIN_ID_T win_id)
{
    wchar display_now_title[MMIMMS_SLIDE_TIME_STR_LEN]    = {0};
    uint8 display_now_size[MMIMMS_SLIDE_TIME_STR_LEN]     = {0};
    wchar display_total_title[MMIMMS_SLIDE_TIME_STR_LEN]  = {0};
    uint8 display_total_size[MMIMMS_SLIDE_TIME_STR_LEN]   = {0};
    wchar now_title[MMIMMS_SLIDE_TIME_STR_LEN]            = {0};
    wchar total_title[MMIMMS_SLIDE_TIME_STR_LEN]          = {0};
    MMI_STRING_T now_str                                  = {0};
    MMI_STRING_T total_str                                = {0};
    MMI_STRING_T text_now_str                             = {0};
    MMI_STRING_T text_total_str                           = {0};
    MMI_STRING_T text_retry_str                           = {0};
    MMI_TEXT_ID_T  text_id                                = TXT_NULL;

    if( !MMK_IsFocusWin(MMIMMS_SENDING_WIN_ID) && !MMK_IsFocusWin(MMIMMS_DOWNLOADING_WIN_ID))
    {
        return;
    }

    if (0 != totalsize)
    {
        //SCI_TRACE_LOW:"FNH now_size = %ld, total_size = %ld"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_WAITINGWIN_698_112_2_18_2_39_4_435,(uint8*)"dd",now_size, totalsize);
        _snprintf((void *)display_total_size,MMIMMS_SLIDE_TIME_STR_LEN ,":%.1fK",(float)totalsize/ONE_K_LEN); 
        if (((float)totalsize/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)totalsize/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
        {
            _snprintf((void*)display_total_size, MMIMMS_SLIDE_TIME_STR_LEN,":%.1fk\r",0.1);
        }
        MMI_STRNTOWSTR(display_total_title, MMIMMS_SLIDE_TIME_STR_LEN, 
            (uint8 *)display_total_size, MMIMMS_SLIDE_TIME_STR_LEN, strlen((const char *)display_total_size));
        MMI_GetLabelTextByLang(TXT_MMS_TOTAL_SIZE, &total_str);
        MMI_WSTRNCPY(total_title, MMIMMS_SLIDE_TIME_STR_LEN, total_str.wstr_ptr, total_str.wstr_len, total_str.wstr_len);
        MMI_WSTRNCPY(total_title + total_str.wstr_len, MMIMMS_SLIDE_TIME_STR_LEN, 
            display_total_title, MMIMMS_SLIDE_TIME_STR_LEN, strlen((const char *)display_total_size));
        text_total_str.wstr_ptr = total_title;
        text_total_str.wstr_len = total_str.wstr_len + strlen((const char *)display_total_size);
        
        if (0 == now_size)
        {
            now_size = 1;
        }
        _snprintf((void *)display_now_size,MMIMMS_SLIDE_TIME_STR_LEN ,":%.1fK",(float)now_size/ONE_K_LEN); 
        if (((float)now_size/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)now_size/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
        {
            _snprintf((void*)display_now_size, MMIMMS_SLIDE_TIME_STR_LEN,":%.1fk\r",0.1);
        }
        MMI_STRNTOWSTR(display_now_title, MMIMMS_SLIDE_TIME_STR_LEN, 
            (uint8 *)display_now_size, MMIMMS_SLIDE_TIME_STR_LEN, strlen((const char *)display_now_size));
        if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
        {
            text_id = TXT_SENT;
        }
        else if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
        {
            text_id = TXT_MMS_RECV_NOW_SIZE;
        }
        else
        {
            return;
        }
        MMI_GetLabelTextByLang(text_id, &now_str);
        MMI_WSTRNCPY(now_title, MMIMMS_SLIDE_TIME_STR_LEN, now_str.wstr_ptr, now_str.wstr_len, now_str.wstr_len);
        MMI_WSTRNCPY(now_title + now_str.wstr_len, MMIMMS_SLIDE_TIME_STR_LEN, 
            display_now_title, MMIMMS_SLIDE_TIME_STR_LEN, strlen((const char *)display_now_size));
        text_now_str.wstr_ptr = now_title;
        text_now_str.wstr_len = now_str.wstr_len + strlen((const char *)display_now_size);        

//         if (MMIMMS_GetIsMmsRetry())
//         {
//             if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
//             {
//                 text_id = TXT_MMS_SEND_RETRY;
//             }
//             else if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
//             {
//                 text_id = TXT_MMS_RECV_RETRY;
//             }
//             else
//             {
//                 text_id = TXT_NULL;
//             }
//         }
//         else
        {
            if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
            {
                text_id = TXT_SENDING;
            }
            else if (MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
            {
                text_id = TXT_MMS_DOWNLOADING;
            }
            else
            {
                text_id = TXT_NULL;
            }
        }
        MMI_GetLabelTextByLang(text_id, &text_retry_str);

        MMIPUB_SetWaitWinText(is_fresh, 3, &text_retry_str, &text_now_str, &text_total_str, win_id);
    }
}

/*****************************************************************************/
//  Description :set rocord id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSMMI_SetSendNewMMSId(uint32 record_id)
{
    g_mms_send_record_id = record_id;
}

/*****************************************************************************/
//  Description :open mms alert win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSMMI_OpenMMSAlertWin(
                                   MMI_TEXT_ID_T     text1_id,           //text 1,no default TXT_NULL
                                   MMI_TEXT_ID_T     text2_id,           //text 2,no default TXT_NULL
                                   MMI_IMAGE_ID_T    image_id            //alert image id
                                   )
{
    MMI_WIN_ID_T  win_id = MMIMMS_ALERT_WIN_ID;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    MMIPUB_OpenAlertWinByTextId(PNULL, text1_id, text2_id, image_id, &win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
}
