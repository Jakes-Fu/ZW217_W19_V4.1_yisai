/*
云芯平台
*/
#include "zdt_app.h"

#ifdef ZDT_PLAT_YX_SUPPORT
YX_APP_T  g_yx_app = {0};
BOOLEAN g_yx_is_stillness = FALSE;
BOOLEAN g_yx_is_move = TRUE;
BOOLEAN g_yx_is_jp_change = TRUE;
BOOLEAN g_yx_is_lbs_change = TRUE;

PUBLIC BOOLEAN  YX_PostEvtTo_APP(ZDT_APP_SIG_E sig_id, void* p_data, uint16 type)
{
    
    uint8 * pstr = PNULL;
    ZDT_YX_DATA_SIG_T * psig = PNULL;    
    ZDT_YX_DATA_T * cur_data = NULL;
    
    cur_data=(ZDT_YX_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_YX_DATA_T));
    if(cur_data == NULL)
    {
        TCP_LOG("TCP POSTEvt ERR Malloc");
        return FALSE;
    }
    cur_data->data_p = p_data;
    cur_data->type = type;

    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_YX_DATA_SIG_T),SCI_IdentifyThread());
    psig->p_data = cur_data;
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}

extern int YX_AppInit(void)
{
    YX_APP_T * pMe = &g_yx_app;
    ZDT_LOG("YX_AppInit");
    pMe->m_yx_net_is_open = FALSE;
    pMe->m_yx_net_is_init = FALSE;
    pMe->m_yx_net_first_reg = FALSE;
    pMe->m_yx_is_reg = FALSE;
    pMe->m_yx_allow_rereg = FALSE;
    pMe->m_yx_is_land = FALSE;
    pMe->m_yx_lk_send_time = 0;
    pMe->m_yx_reg_ip = 0;
    pMe->m_yx_reg_port = 0;
    pMe->m_yx_send_err_num = 0;
    pMe->m_yx_test_stop = FALSE;
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
    pMe->m_yx_loction_link_head = NULL;
#endif

    pMe->m_yx_rsp_err_num = 0;

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    pMe->m_yx_voc_file_seek = 0;
    pMe->m_yx_voc_pack_num = 0;
    pMe->m_yx_voc_pack_idx = 0;
    pMe->m_yx_voc_data_idx = 0;
    pMe->m_yx_eRecorderStatus = YX_RECOREDER_STOP;
    pMe->m_yx_eFilePlayrStatus = YX_FILE_PLAYSTOP;
    //pMe->m_yx_pRecordMedia = NULL;
    //pMe->m_yx_pPlayMedia = NULL;
    pMe->m_yx_voc_send_link_head = NULL;
    pMe->m_yx_voc_rcvfile_link_head = NULL;
    pMe->m_yx_voc_is_sending = FALSE;
    pMe->m_yx_voc_is_playing = FALSE;
    pMe->m_yx_cur_rcv_is_complete = TRUE;
    pMe->m_yx_voc_send_times = 0;
    pMe->m_yx_voc_play_time_id  = 0;;
    pMe->m_yx_voc_record_time_id  = 0;;
    pMe->m_yx_voc_record_stop_time_id  = 0;;
    pMe->m_yx_voc_rcv_time_id  = 0;;

#endif
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
    pMe->m_yx_friend_status = YX_FRIEND_NULL;
    pMe->m_yx_friend_mark = 0;
    pMe->m_yx_friend_alert_str = NULL;
    pMe->m_yx_friend_alert_len = 0;
#endif
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_API_Record_SetPlayAll(0);
    YX_API_SetVocPlayVolume(9);
#endif
    YX_DB_Init(pMe);
    return 0;
}

extern int YX_AppFree(void)
{
    YX_APP_T * pMe = &g_yx_app;
    ZDT_LOG("YX_AppFree");
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_VOC_RcvFile_Link_DelAll(pMe);
    YX_VOC_Send_Link_DelAll(pMe);
#endif
    YX_DB_Free(pMe);
    return 0;
}

extern YX_APP_T * YX_AppGet(void)
{
    return &g_yx_app;
}

int YX_RestorBind(void)
{
    YX_APP_T * pMe = &g_yx_app;
    ZDT_LOG("YX_RestorFactory");
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_Voice_FilePlay_Stop(pMe);
#endif
    YX_DB_DelAll(pMe);
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_API_Record_FileDelAll(pMe);
    YX_VOC_RcvFile_Link_DelAll(pMe);
#endif
    ZDT_NV_SetRejectUnknownCall(FALSE);
    MMIAPIALM_RestoreFactorySetting();
    MMIZDT_Net_Reset(TRUE);
    //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_OFF);
    return 0;
}

int YX_RestorFactory(void)
{
    #if 0 //def _SW_ZDT_PRODUCT_
	    MMIDEFAULT_TurnOnBackLight();
	    //MMIAPISET_OpenAboutResetPwdWin(TRUE);
	    //MMK_PostMsg(MMK_GetFirstAppletHandle(),MSG_APP_WEB, NULL, 0);
	    //MMK_PostMsg(MMIPUB_ALERT_WIN_ID,MSG_APP_WEB, NULL, 0);
    #else

    YX_APP_T * pMe = &g_yx_app;
    ZDT_LOG("YX_RestorFactory");
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_Voice_FilePlay_Stop(pMe);
#endif
    YX_DB_DelAll(pMe);
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_API_Record_FileDelAll(pMe);
    YX_VOC_RcvFile_Link_DelAll(pMe);
#endif
    MMISET_CleanUserData();
    MMISET_ResetFactorySetting();
    MMIAPIPHONE_PowerReset();//MMIZDT_Net_Reset();//yangyu modify
    //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_OFF);
    #endif  
    
    return 0;
}

int YX_RestorFactoryExtCall(void)
{
    #if 0 //def _SW_ZDT_PRODUCT_
	    MMIDEFAULT_TurnOnBackLight();
	    //MMIAPISET_OpenAboutResetPwdWin(TRUE);
	    //MMK_PostMsg(MMK_GetFirstAppletHandle(),MSG_APP_WEB, NULL, 0);
	    //MMK_PostMsg(MMIPUB_ALERT_WIN_ID,MSG_APP_WEB, NULL, 0);
    #else

    YX_APP_T * pMe = &g_yx_app;
    ZDT_LOG("YX_RestorFactory");
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_Voice_FilePlay_Stop(pMe);
#endif
    YX_DB_DelAll(pMe);
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    YX_API_Record_FileDelAll(pMe);
    YX_VOC_RcvFile_Link_DelAll(pMe);
#endif

    //MMISET_ResetFactorySetting();
    //MMIAPIPHONE_PowerReset();//MMIZDT_Net_Reset();//yangyu modify
    //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_OFF);

    #endif  
    
    return 0;
}

#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
int YX_API_Friend_Start(void)
{
    return AEE_SUCCESS;
}
#endif

static int YX_HandleLose(DPARAM param)
{
    ZDT_LOG("YX_HandleLose");
#ifdef ZDT_SMS_SUPPORT
    YX_API_SMS_SendLose();
#endif
    MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_ALERT,NULL,YX_NET_ALERT_TYPE_LOSE);
    //YX_Net_Send_AL(pMe,YX_NET_ALERT_TYPE_LOSE,0);
    return 0;
}

static int YX_HandleBatStatus(DPARAM param)
{
    uint8 level = 0;
    uint8 percent = 0;
    uint16 wParam  = 0;
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    if(p_getdata != NULL)
    {
        wParam = p_getdata->len;
    }
    level = (wParam >> 8);
    percent = (wParam & 0x00FF);
    ZDT_LOG("YX_HandleBatStatus wParam = 0x%x,level=%d,percent=%d",wParam,level,percent);
    if(level < 1) //为省电改为2 -> 1
    {
#ifdef ZDT_SMS_SUPPORT
        YX_API_SMS_SendLowBattery();
#endif
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_YX_ALERT,NULL,YX_NET_ALERT_TYPE_LOWBAT);
        //YX_Net_Send_AL(pMe,YX_NET_ALERT_TYPE_LOWBAT);
    }
    return 0;
}

static int YX_HandleMove(DPARAM param)
{
    //ZDT_LOG("YX_HandleMove");
    g_yx_is_stillness = FALSE;
    g_yx_is_move = TRUE;
    return 0;
}

static int YX_HandleStill(DPARAM param)
{
    //ZDT_LOG("YX_HandleStill");
    g_yx_is_stillness = TRUE;
    return 0;
}

static int YX_HandleJPChange(DPARAM param)
{
    //ZDT_LOG("YX_HandleJPChange");
    g_yx_is_jp_change = TRUE;
    MMIZDT_UpdatePedometerWin();
    return 0;
}

static int YX_HandleLBSChange(DPARAM param)
{
    //ZDT_LOG("YX_HandleLBSChange");
    g_yx_is_lbs_change = TRUE;
    return 0;
}

BOOLEAN YX_LocReport_Judge(void)
{
    BOOLEAN res = FALSE;
    return TRUE;
    if(g_yx_is_jp_change || g_yx_is_lbs_change)
    {
        res = TRUE;
    }
    if(res == FALSE)
    {
        if(g_yx_is_move)
        {
            res = TRUE;
        }
    }
    return res;
}

void YX_LocReport_Reset(void)
{
    g_yx_is_jp_change = FALSE;
    g_yx_is_lbs_change = FALSE;
    g_yx_is_move = FALSE;
    return;
}

static int YX_HandleFallDown(DPARAM param)
{
    ZDT_LOG("YX_HandleFallDown");
    return 0;
}

extern MMI_RESULT_E YX_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    YX_APP_T * pMe = &g_yx_app;
    switch (msg_id)
    {
        case ZDT_APP_SIGNAL_YX_LOSE:
                YX_HandleLose(param);
            break;
        case ZDT_APP_SIGNAL_YX_BATSTAUTS:
                YX_HandleBatStatus(param);
            break;
        case ZDT_APP_SIGNAL_YX_MOVE:
                YX_HandleMove(param);
            break;
        case ZDT_APP_SIGNAL_YX_STILL:
                YX_HandleStill(param);
            break;
        case ZDT_APP_SIGNAL_YX_JP_CHANGE:
                YX_HandleJPChange(param);
            break;
        case ZDT_APP_SIGNAL_YX_LBS_CHANGE:
                YX_HandleLBSChange(param);
            break;
        case ZDT_APP_SIGNAL_YX_FALLDOWN:
                YX_HandleFallDown(param);
            break;
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
        case ZDT_APP_SIGNAL_YX_LOCTION_END:
                YX_LOCTION_EndHandle(pMe,param);
            break;
#endif

        case ZDT_APP_SIGNAL_YX_CAPTRUE:
                YX_Captrue_Handle(pMe,param);
            break;
            
        case ZDT_APP_SIGNAL_YX_CAPTRUE_SEND:
                YX_Captrue_Send_Handle(pMe,param);
            break;
            
        case ZDT_APP_SIGNAL_YX_NET_RESET:
                YX_Net_Reset(pMe);
            break;
            
       case ZDT_APP_SIGNAL_YX_HEART:
                YX_Net_Send_LK(pMe,5,10000);
            break;
            
       case ZDT_APP_SIGNAL_YX_LOCREPORT:
#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
            YX_LOCTION_Start(pMe,YX_LOCTION_TYPE_NET,NULL,0);
#else
            YX_Net_Send_UD(pMe,0);
            YX_Net_Send_WT_Check(0);
#endif
            break;

       case ZDT_APP_SIGNAL_YX_ALERT:
        {
            uint16 wParam  = 0;
            MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
            MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
            if(p_getdata != NULL)
            {
                wParam = p_getdata->len;
            }

#if defined(ZDT_GPS_SUPPORT) || defined(ZDT_WIFI_SUPPORT) 
            YX_LOCTION_Start(pMe,YX_LOCTION_TYPE_ALERT,NULL,wParam);
#else
            YX_Net_Send_AL(pMe,wParam,0);
#endif
       }
            break;
       case ZDT_APP_SIGNAL_YX_UNBIND:
                if(0 == YX_Net_Send_DLT(pMe,0,0))
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_UNBIND_ERR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
            break;
            
       case ZDT_APP_SIGNAL_YX_WT:
                YX_Net_Send_WT(pMe,0);
            break;

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
        case ZDT_APP_SIGNAL_YX_RECORD_SEND:
                YX_Voice_Send_Start(pMe);
            break;
            
        case ZDT_APP_SIGNAL_YX_RECORD_START:
                    YX_Voice_Record_Start(pMe);
                break;
            
        case ZDT_APP_SIGNAL_YX_RECORD_SUCCESS:
                    YX_Voice_Record_HandleEnd(TRUE);
                break;
                
        case ZDT_APP_SIGNAL_YX_RECORD_FAIL:
                    YX_Voice_Record_HandleEnd(FALSE);
                break;
                
        case ZDT_APP_SIGNAL_YX_RECORD_TIMEOUT:
                    YX_Voice_Record_StopTimout(pMe);
            break;
            
        case ZDT_APP_SIGNAL_YX_RECORD_STOP:
                YX_Voice_Record_Stop(pMe);
            break;
            
        case ZDT_APP_SIGNAL_YX_VOC_FILEPLAY:
                YX_Voice_FilePlay_Start(pMe);
            break;
            
        case ZDT_APP_SIGNAL_YX_VOC_PLAYSTOP:
                YX_Voice_FilePlay_Stop(pMe);
            break;
            
        case ZDT_APP_SIGNAL_YX_VOC_VOLUME:
            {
                uint16 wParam  = 0;
                MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
                MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
                if(p_getdata != NULL)
                {
                    wParam = p_getdata->len;
                }
                YX_Voice_FilePlay_SetVolume(pMe,wParam);
            }
            break;
            
        case ZDT_APP_SIGNAL_YX_VOC_SEND_RESULT:
            break;
#endif
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}
#endif
