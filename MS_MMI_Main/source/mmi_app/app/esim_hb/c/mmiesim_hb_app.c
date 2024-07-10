#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiesim_hb_id.h"
#include "mmicom_trace.h"
#include "mmipub.h"
#include "mmiesim_hb_text.h"
#include "watch_commonwin_export.h"
#include "mmi_modu_main.h"
#include "guitext.h" 
#include "window_parse.h"
#include "hb_export.h"
#define Profile_Download_Length 100

LOCAL uint8 s_cur_down_process_num = 0;
LOCAL void MMI_Esim_HB_Profile_Download_Callback (ESIM_HB_DOWNLOAD_MSG_T* param);
LOCAL MMI_RESULT_E HandleProfileDownloadProcessWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            );

LOCAL MMI_RESULT_E HandleProfileDownloadProcessWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint8 num_counted = 0;
    MMI_STRING_T title_str = {0};
    MMI_HANDLE_T win_handle = 0;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_SetWinDisplayStyleState(win_id,WS_DISABLE_FLING_CLOSE_WIN,TRUE);
            MMIRES_GetText(TXT_ESIM, PNULL,&title_str);
            MMIPUB_SetWinTitleText(win_id,&title_str,FALSE);
            result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
            break;
        }
        case MSG_APP_RED:
        case MSG_KEYDOWN_USER_FUNC:
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_LOSE_FOCUS:
        case MSG_GET_FOCUS:
        {
            break;
        }
        default:
        {
            result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
            break;
        }
    return result;
    }
}
LOCAL void MMI_Esim_HB_Profile_Download_Callback (ESIM_HB_DOWNLOAD_MSG_T* param)
{
    MMI_STRING_T text_string = {0};
    MMI_WIN_ID_T progress_win_id = ESIM_PROFILE_DOWNLOAD_PROGRESS_WIN_ID;
    MMI_WIN_ID_T nofity_win_id = ESIM_PROFILE_DOWNLOAD_NOTIFY_WIN_ID;
    uint8 num_counted=0;
    WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
    TRACE_APP_ESIM_HB(" MMI_Esim_HB_Profile_Download_Callback:msg_id:%d\n",param->commond_type);

    switch(param->commond_type)
    {
        case ESIM_HB_COMMOND_DOWNLOAD_START:
        case ESIM_HB_COMMOND_DOWNLOAD_PROGRESS:
        {
            if(!MMK_IsOpenWin(progress_win_id))
            {
                MMI_GetLabelTextByLang(TXT_ESIM_PROFILE_DOWNLOAD_PROGRESS, &text_string);
                MMIPUB_OpenProgressWinEx(MMK_GetFirstAppletHandle(),&text_string,&progress_win_id,PNULL,PNULL,HandleProfileDownloadProcessWinMsg);
                MMIPUB_SetProgressTotalSize(&progress_win_id,Profile_Download_Length);
            }
            else
            {
                num_counted = param->progress_value;
                MMIPUB_UpdateProgressBar(&progress_win_id,num_counted);
                TRACE_APP_ESIM_HB("HandleProfileDownloadWinMsg:ESIM_HB_COMMOND_DOWNLOAD_PROGRESS:num_counted=%d ",num_counted);
            }
            break;
        }
        case ESIM_HB_COMMOND_DOWNLOAD_SUCESS:
        {
            MMI_GetLabelTextByLang(TXT_ESIM_PROFILE_DOWNLOAD_SUCCESS,&text_string);
            if(MMK_IsOpenWin(progress_win_id))
            {
                MMK_CloseWin(progress_win_id);
                s_cur_down_process_num = 0;
                Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(nofity_win_id,&text_string,PNULL,PNULL);
            }
            else
            {
                s_cur_down_process_num = 0;
                Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(nofity_win_id,&text_string,PNULL,PNULL);
            }
            TRACE_APP_ESIM_HB("HandleProfileDownloadWinMsg:ESIM_HB_COMMOND_DOWNLOAD_SUCESS:s_cur_down_process_num=%d ",s_cur_down_process_num);
            break;
        }
        case ESIM_HB_COMMOND_DOWNLOAD_FAIL:
        {
            TRACE_APP_ESIM_HB("HandleProfileDownloadWinMsg:ESIM_HB_COMMOND_DOWNLOAD_FAIL:s_cur_down_process_num=%d ",s_cur_down_process_num);
            MMI_GetLabelTextByLang(TXT_ESIM_PROFILE_DOWNLOAD_FAIL,&text_string);
            if(MMK_IsOpenWin(progress_win_id))
            {
                MMK_CloseWin(progress_win_id);
                s_cur_down_process_num = 0;
                Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(nofity_win_id,&text_string,PNULL,PNULL);
            }
            else
            {
                s_cur_down_process_num = 0;
                Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(nofity_win_id,&text_string,PNULL,PNULL);
            }
            break;
        }
        default:
            break;
    }
}

PUBLIC void MMIESIM_HB_InitModle(void)
{
    uint32 ble_dev_handle = 0;
    BOOLEAN ret =FALSE;
    ESIM_HB_RegIdNameArr();
    ret = ESIM_HB_RegisterCallback(MMI_Esim_HB_Profile_Download_Callback);
    TRACE_APP_ESIM_HB("MMIESIM_HB_InitModle:ESIM_HB_RegisterCallback:ret=%d ",ret);
}
