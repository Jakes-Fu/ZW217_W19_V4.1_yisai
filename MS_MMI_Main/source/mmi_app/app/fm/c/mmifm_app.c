/*****************************************************************************
** File Name:      mmifm_app.c                                               *
** Author:         baokun.yin                                              *
** Date:           2008-12-16                                                 *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is app of fm                                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008-12-16      
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fm_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef FM_SUPPORT
#include "mmifm_export.h"
#include "mmifm_nv.h"
#include "mmi_nv.h"
#include "mmisrvaud_api.h"
#include "mmifm_internal.h"
#include "mmi_default.h"
#include "mmiidle_export.h"
#include "mmiidle_subwintab.h"
#include "mmipub.h"
#include "mmifm_text.h"
#include "mmisd_export.h"
#include "mmifm_id.h"
#include "mmialarm_export.h"
#include "mmifmm_export.h"
#include "mmiudisk_export.h"

#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
//#include "mmipub.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "gpio_prod_api.h"
#include "mmiidle_statusbar.h"
#include "mmiidle_cstyle.h"
/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/

LOCAL MMIFM_CUR_INFO_T           s_fm_cur_info = {0}; /*lint !e64*/
#ifdef MMIFM_SUPPORT_FM_RECORD
LOCAL wchar*                     s_fm_record_file_path = PNULL;
LOCAL MMIFMRECORD_STATE_E        s_current_state = MMIFMRECORD_STATE_NONE;
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
LOCAL MMIFM_TIMER_RECORD_INFO_T  s_fm_timer_record_info = {0}; /*lint !e64*/
#endif
LOCAL MMISRV_HANDLE_T s_fm_play_handle = 0;
#ifdef MMIFM_SUPPORT_FM_RECORD
LOCAL MMISRV_HANDLE_T s_fm_record_handle = 0;
#endif
#ifdef FM_RDS_SUPPORT
LOCAL uint8 s_timer_read_rds = 0;
#endif
/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : save current info
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  SaveCurrentFMInfo(const MMIFM_CUR_INFO_T *cur_info_ptr);

/*****************************************************************************/
//  Description : read current info
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  ReadCurrentFMInfo(MMIFM_CUR_INFO_T *cur_info_ptr);

/*****************************************************************************/
//  Description : get fm out put device by mmi fm device mode
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL FM_OUTPUT_MODE_E  GetFMDeviceByAudioMode(const MMIFM_AUDIO_DEVICE_MODE_E sound_mode);

/*****************************************************************************/
//  Description : write channle info to nv
//  Global resource dependence : 
//  Parameter : channel_index[IN]: channel index to be saved
//              channel_ptr[IN]: channel info to be saved
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WriteChannelToNv(uint16 channel_index, const MMIFM_CHANNEL_T *channel_ptr);
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : assign a default name
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateFileName(wchar* input_file_path);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL void MMIFM_SetVolume(MMIFM_AUDIO_DEVICE_MODE_E fm_mode, uint8 volume_level);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL void MMIFM_SetVolumeForRing(MMIFM_AUDIO_DEVICE_MODE_E fm_mode, uint8 volume_level);
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : Callback function to MMI AUDIO
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIFM_RecordCallBack(MMISRVAUD_REPORT_RESULT_E  result,DPARAM  param);
/*****************************************************************************/
//  Description : handle record end.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_HandleRecordEnd(BOOLEAN is_succeed);
/*****************************************************************************/
// 	Description : check space for record.
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_IsEnoughSpaceToRecord(void);
#endif
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
/*****************************************************************************/
//  Description : Get Next Valid Channel By Freq
//  Global resource dependence : 
//  Author:
//  Parameter: step[IN]: adjust steps, 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIFM_GetNextValidChannelByFreq(uint16 freq,int32 step,uint16 *ch_index);
#else
/*****************************************************************************/
//  Description : Get Next Valid Channel By Index
//  Global resource dependence : 
//  Author:langhua.yang
//  Parameter: step[IN]: adjust steps 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIFM_GetNextValidChannelByIndex(uint16 freq,int32 step,uint16 *ch_index);
#endif
/*****************************************************************************/
//  Description : get sorted channel index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetSortedChannelIndex(uint16 channel_list[]);
/*****************************************************************************/
//  Description : 根据mode值获取audio route
//  Global resource dependence : 
//  Author:
//  Parameter: 无
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T GetAudioSupportRoute(void);
#if defined (MMI_FM_TIMER_RECORD_SUPPORT) && defined(ATV_SUPPORT)
/*****************************************************************************/
//  Description :Handle FmRecord Close Atv Query Win MSG
//  Global resource dependence : none
//  Author: langhua.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmRecordCloseAtvQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );
#endif

#ifdef FM_RDS_SUPPORT
#define RDS_TIMER_PERIOD  2000
/*****************************************************************************/
//  Description :Start RDS Timer
//  Global resource dependence : none
//  Author: lv.dong
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_StartRDSTimer ( ){
    if(s_timer_read_rds == 0){
        s_timer_read_rds = MMK_CreateWinTimer(FM_MAIN_WIN_ID, RDS_TIMER_PERIOD, TRUE);
    }
}

/*****************************************************************************/
//  Description :Stop FM RDS Timer
//  Global resource dependence : none
//  Author: lv.dong
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_StopRDSTimer(){
    if(0 != s_timer_read_rds)
    {
        MMK_StopTimer(s_timer_read_rds);
        s_timer_read_rds = 0;
    }
}

/*****************************************************************************/
//  Description :read RDS data from fm driver
//  Global resource dependence : none
//  Author: lv.dong
//  Note:
/*****************************************************************************/

#define MAX_RDS_DADA_LENGTH 100
PUBLIC  void  MMIFM_ReadRDS(){
     int32 str_len = 0;
     wchar    * wstr_rds_data_ptr = PNULL;

#ifdef  WIN32     
    LOCAL char looper ;
    wchar   *wStrRDS = PNULL;
    wchar    rds_data1[MMIFM_FREQ_MAX_LEN + 1] = {L"RDS TEST 1"};
    wchar    rds_data2[MMIFM_FREQ_MAX_LEN + 1] = {L"RDS TEST 2"};
    
    if(0 == looper%2){
        wStrRDS = rds_data1;    
    } else {
        wStrRDS = rds_data2;    
    }
    looper ++;
    str_len = MMIAPICOM_Wstrlen(wStrRDS);
    wstr_rds_data_ptr = (wchar *)SCI_ALLOC_APP(MAX_RDS_DADA_LENGTH);
    memset(wstr_rds_data_ptr,0,MAX_RDS_DADA_LENGTH);
    if (SCI_NULL == wstr_rds_data_ptr){
          SCI_TRACE_LOW("[MMIFM] MMIFM_ReadRDS  ALLOC fail  !");
          return;
    }
    MMI_WSTRNCPY(wstr_rds_data_ptr,MAX_RDS_DADA_LENGTH,wStrRDS, MAX_RDS_DADA_LENGTH, str_len );

#else 
     // FM_RDS_DRIVER_TODO : read RDS from WCN
     //FM_RDS_Get_Data();
#endif
    //MMK_SendMsg(FM_MAIN_WIN_ID, MSG_APP_FM_RDS_PARAM , PNULL);
    SetRDSText(wstr_rds_data_ptr);
    SCI_FREE(wstr_rds_data_ptr);
    SCI_TRACE_LOW("[MMIFM] MMIFM_ReadRDS   ");
}

/*****************************************************************************/
//  Description : get fm rds status
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_RDS_STATUS_E MMIFM_GetRdsStatus(void)
{
    SCI_TRACE_LOW("[MMIFM] MMIFM_GetRdsStatus  %d", s_fm_cur_info.rds_open);
    
    return s_fm_cur_info.rds_open;
}



/*****************************************************************************/
//  Description : switch FM RDS(radio data system) close or open
//  Global resource dependence : 
//  Author:
//  Parameter: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_SwitchRDS( )
{
    SCI_TRACE_LOW("[MMIFM] MMIFM_SwitchRDS  , current status is  ");
    switch (s_fm_cur_info.rds_open)
    {
        case FM_RDS_STATUS_OPEN:
        {
           // FM_RDS_DRIVER_TODO  Call FM driver to set RDS close
           #ifndef WIN32
           FM_RDS_Close();
           #endif
           MMIFM_StopRDSTimer();
           s_fm_cur_info.rds_open = FM_RDS_STATUS_CLOSE;
        }
        break;

        case FM_RDS_STATUS_CLOSE:
        {
            // FM_RDS_DRIVER_TODO  Call FM driver to set RDS open
            #ifndef WIN32
            FM_RDS_Open();
            #endif
            MMIFM_StartRDSTimer();
            s_fm_cur_info.rds_open  = FM_RDS_STATUS_OPEN;
        }
        break;
            
        default:
                 
        break;
    }

   
   SaveCurrentFMInfo(&s_fm_cur_info);    
   return     TRUE;

}
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DEFINITION                     *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init FM application
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_Init(void)
{
    BOOLEAN  read_ret = FALSE;
    
    SCI_MEMSET(&s_fm_cur_info,0,sizeof(MMIFM_CUR_INFO_T));
    read_ret = ReadCurrentFMInfo(&s_fm_cur_info);
    s_fm_cur_info.is_bgplay = FALSE;
    s_fm_cur_info.status = FM_STOP;
    if(FALSE == read_ret)
    {
        //reset current status
        s_fm_cur_info.cur_freq  = MMIFM_FREQ_INITIAL_VAL;

        s_fm_cur_info.sound_mode = FM_HEADSET_MODE;
#ifdef MMIFM_SUPPORT_FM_RECORD
        s_fm_cur_info.fm_record_file_id = MMIFM_RECORD_INITIAL_FILE_ID;
#if defined(NANDBOOT_SUPPORT) || defined(MMI_UDISK_MEM_ENABLE)   
        s_fm_cur_info.dev = MMI_DEVICE_UDISK;
#else
        s_fm_cur_info.dev = MMI_DEVICE_SDCARD;
#endif
#endif
        s_fm_cur_info.suspend_by_fm = FALSE;
        s_fm_cur_info.paused_with_fm = FALSE;
#ifdef MMIFM_SUPPORT_FORMAT_AMR 
        s_fm_cur_info.record_file_format = MMISRVAUD_RECORD_FMT_ADPCM;
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE
        s_fm_cur_info.fm_eq_mode = MMISRVAUD_EQ_REGULAR;
#endif
#ifdef FM_RDS_SUPPORT
        s_fm_cur_info.rds_open = FM_RDS_STATUS_CLOSE;
#endif
        SaveCurrentFMInfo(&s_fm_cur_info);
    }
}

/*****************************************************************************/
//  Description : FM play call back function.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
        
    if(handle == s_fm_play_handle && param != PNULL)
    {
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            if(s_fm_cur_info.status == FM_PLAYING)
            {
                FM_PlayStop();
                MMIFM_SetSuspendByFM(FALSE);               
                s_fm_cur_info.status = FM_SUSPENDED;
            }
            //MMISRVAUD_SetVolume(handle, 0);
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            if(s_fm_cur_info.status != FM_STOP)
            {
              if(MMIFM_CheckHeadsetStatusWithoutPrompt())
              {
                if(!MMIFM_GetSuspendByFM() && !MMK_IsOpenWin(FM_AUTO_SEARCH_WIN_ID))
                {
                  s_fm_cur_info.cur_freq = MIN(s_fm_cur_info.cur_freq, MMIFM_FREQ_RANGE_HIGH);
                  s_fm_cur_info.cur_freq = MAX(s_fm_cur_info.cur_freq, MMIFM_FREQ_RANGE_LOW);
                  MMISRVAUD_SetVolume(handle, 0);
                  FM_ManualSeek(s_fm_cur_info.cur_freq, FM_SCAN_UP, 0, NULL);
                  MMISRVAUD_SetVolume(handle, MMIAPISET_GetMultimVolume());
                  s_fm_cur_info.status = FM_PLAYING;
                }
              }
            }
            break;
        case MMISRVMGR_NOTIFY_EXT:
            {
                report_ptr = (MMISRVAUD_REPORT_T *)param->data;
                SCI_TRACE_LOW("[&&&]PlayCallBack: report_ptr=%d",report_ptr->report);
                switch(report_ptr->report)
                {
                    case MMISRVAUD_REPORT_EARPHONE_EVENT:
                    switch(report_ptr->data1)
                    {
                        case (MMISRVAUD_EARPHONE_EVENT_CLK + KEY_HEADSET_BUTTIN):  // 中键单击
                        MMIFM_HandleHeadsetButtonAction(HEADSET_BUTTON_SHORT_PRESS);
                        break;
                        case (MMISRVAUD_EARPHONE_EVENT_LONG + KEY_HEADSET_BUTTIN): // 中键长按
                        MMIFM_HandleHeadsetButtonAction(HEADSET_BUTTON_LONG_PRESS);
                        break;
                        case (MMISRVAUD_EARPHONE_EVENT_DBLCLK + KEY_HEADSET_BUTTIN): // 中键双击
                        break;
                        default:
                            break;
                    }
                    break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
        }
    }
    return TRUE;
}

#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : FM record call back function.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RecordCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    if(handle == s_fm_record_handle && param != PNULL)
    {
        MMISRVAUD_REPORT_T *report_ptr = PNULL;
        
        if(MMISRVMGR_NOTIFY_EXT == param->event)
        {
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            
            if(report_ptr != PNULL)
            {            
                //MMISRV_TRACE_LOW:"FM RecordCallBack(), report_ptr->report=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_229_112_2_18_2_19_21_0,(uint8*)"d", report_ptr->report);
            
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_END:
                    MMIFM_RecordCallBack(report_ptr->data1, 0);
                    break;
                default:
                    break;
                }
            }
        }
    }
    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : Free FM play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_FreePlaySrv(void)
{
    SCI_TRACE_LOW("MMIFM_ReqPlaySrv: s_fm_play_handle=0x%x",s_fm_play_handle);
    if(s_fm_play_handle != 0)
    {
        MMISRVMGR_Free(s_fm_play_handle);
        s_fm_play_handle = 0;
    }
}

/*****************************************************************************/
//  Description : Request FM play sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_ReqPlaySrv(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;

    if(s_fm_play_handle != 0)
    {
        res = TRUE;
    }
    else
    {
        req.pri = MMISRVAUD_PRI_NORMAL;
        req.notify = PlayCallBack;
        audio_srv.info.fm.type = MMISRVAUD_TYPE_FM;
        audio_srv.volume = MMIAPISET_GetMultimVolume();
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
        audio_srv.all_support_route = GetAudioSupportRoute();
        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(handle > 0)
        {
            s_fm_play_handle = handle;
            res = TRUE;
        }
    }
    //SCI_TRACE_LOW:"MMIFM_ReqPlaySrv: s_fm_play_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_288_112_2_18_2_19_21_1,(uint8*)"d", s_fm_play_handle);

    return res;
}
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)        
/*****************************************************************************/
//  Description : Free FM record sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_FreeRecSrv(void)
{
    SCI_TRACE_LOW("MMIFM_ReqPlaySrv: s_fm_record_handle=0x%x",s_fm_record_handle);
    if(s_fm_record_handle != 0)
    {
        MMISRVMGR_Free(s_fm_record_handle);
        s_fm_record_handle = 0;
    }     
}

/*****************************************************************************/
//  Description : Request FM record sevice.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_ReqRecSrv(MMISRVAUD_RECORD_FMT_E fmt, wchar *file_name)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    if(s_fm_record_handle != 0)
    {
        res = TRUE;
    }
    else
    {
        req.pri = MMISRVAUD_PRI_NORMAL;
        req.notify = RecordCallBack;
        
        audio_srv.info.type = MMISRVAUD_TYPE_RECORD_FILE;
        audio_srv.info.record_file.fmt = fmt;
        audio_srv.info.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;
        audio_srv.info.record_file.source = MMISRVAUD_RECORD_SOURCE_FM;
        audio_srv.info.record_file.name = file_name;
        audio_srv.info.record_file.name_len = MMIAPICOM_Wstrlen(file_name);

        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(handle > 0)
        {
            s_fm_record_handle = handle;
            res = TRUE;
        }
    }
    //SCI_TRACE_LOW:"MMIFM_ReqRecSrv: s_fm_record_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_340_112_2_18_2_19_21_2,(uint8*)"d", s_fm_record_handle);
    return res;
}
#endif
/*****************************************************************************/
//  Description : Get FM play handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIFM_GetPlayHandle(void)
{
    return s_fm_play_handle;
}
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : Get FM record handle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIFM_GetRecHandle(void)
{
    return s_fm_record_handle;
}

/*****************************************************************************/
//  Description : Get FM record time.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIFM_GetRecTime(void)
{
    MMISRVAUD_RECORD_INFO_T info = {0};
    uint32 time = 0;
    if(MMISRVAUD_GetRecordInfo(MMIFM_GetRecHandle(), &info))
    {
        time = info.total_time;
    }
    return time;
}
#endif
/*****************************************************************************/
//  Description : enable FM mute 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_EnableMute(void)
{
    MMISRVAUD_SetVolume(MMIFM_GetPlayHandle(), (uint32)MMISET_VOL_ZERO);
    return TRUE;
}

/*****************************************************************************/
//  Description : restore FM to user setting volume
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_DisableMute(void)
{
    MMISRVAUD_SetVolume(MMIFM_GetPlayHandle(), MMIAPISET_GetMultimVolume());
    return TRUE;
}
/*****************************************************************************/
//  Description : adjust FM sound volume
//  Global resource dependence : 
//  Parameter: adjust volume to current volume + step 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustVolume(int32 step)
{
    BOOLEAN result = FALSE;
    uint8   vol = MMIAPISET_GetMultimVolume();

    //SCI_TRACE_LOW:"[MMIFM] MMIFM_AdjustVolume step %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_402_112_2_18_2_19_21_3,(uint8*)"d", step);
    if(MMK_IsOpenWin(FM_MAIN_WIN_ID))
    {
        result = FALSE;
    }
    else
    {
        if(step > 0)
        {
            vol = ((vol + step) <= MMIFM_VOL_LEVEL_MAX ? (vol + step) : MMIFM_VOL_LEVEL_MAX);
        }
        else
        {
            vol = ((vol + step) <= MMIFM_VOL_LEVEL_0 ? MMIFM_VOL_LEVEL_0 : (vol + step));
        }
        MMIAPISET_SetMultimVolume(vol);
        if(FM_PLAYING == s_fm_cur_info.status)
        {
            MMISRVAUD_SetVolume(MMIFM_GetPlayHandle(), vol);
        }
        result = TRUE;
    }
    return (result);
}

/*****************************************************************************/
//  Description : adjust FM sound volume
//  Global resource dependence : 
//  Parameter: adjust volume to current volume + step 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustVolumeTP(int32 step)
{
    BOOLEAN result = FALSE;
    uint8   vol = 0;
    
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_AdjustVolumeTP step %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_436_112_2_18_2_19_21_4,(uint8*)"d", step);
    if(step > 0)
    {
        vol = (step <= MMIFM_VOL_LEVEL_MAX ? step : MMIFM_VOL_LEVEL_MAX);
    }
    else
    {   
        vol = MMIFM_VOL_LEVEL_0;//修改Coverity扫描结果
    }
    MMIAPISET_SetMultimVolume(vol);
    if(FM_PLAYING == s_fm_cur_info.status)
    {
        MMISRVAUD_SetVolume(MMIFM_GetPlayHandle(), vol);
    }
    result = TRUE;    
    return (result);
}

/*****************************************************************************/
//  Description : Play FM
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Play(BOOLEAN is_fm_first_open)
{
    uint32  init_result = FM_SUCCESS;
    BOOLEAN ret = TRUE;
#ifdef MMIFM_SUPPORT_FM_RECORD
    MMIFMRECORD_STATE_E record_status = MMIFM_GetRecordStatus(); 
#endif
	MMIFM_GetFMCurrentInfo();
    if(MMIFM_CheckHeadsetStatusWithoutPrompt())
    {
        //键盘音的停止处理需要放在 FM_Init 之前
        if(!FM_IsInit())
        {
            init_result = FM_Init();
        }
        //SCI_TRACE_LOW:"[MMIFM] MMIFM_Play init_result %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_473_112_2_18_2_19_21_5,(uint8*)"d", init_result);
        if(FM_SUCCESS == init_result && MMIFM_ReqPlaySrv())
        {
	      if(!MMK_IsOpenWin(FM_AUTO_SEARCH_WIN_ID))
	      {
            if((is_fm_first_open && (MMIFM_GetValidChannelCount() > 0)) || 
               !is_fm_first_open)
            {
    	      s_fm_cur_info.cur_freq = MIN(s_fm_cur_info.cur_freq, MMIFM_FREQ_RANGE_HIGH);
    	      s_fm_cur_info.cur_freq = MAX(s_fm_cur_info.cur_freq, MMIFM_FREQ_RANGE_LOW);
    	      MMIFM_EnableMute();
    	      FM_ManualSeek(s_fm_cur_info.cur_freq, FM_SCAN_UP, 0, NULL);
    	      MMIFM_DisableMute();
    	      s_fm_cur_info.status = FM_PLAYING;
#ifdef MMIFM_SUPPORT_EQ_MODE
              MMIFM_SetEqMode(MMIFM_GetEqMode());
#endif
    	      MMISRVAUD_Play(MMIFM_GetPlayHandle(), 0);
            }
	      }
        }
        else
        {
            ret = FALSE;
        }
#ifdef MMIFM_SUPPORT_FM_RECORD
        if(MMIFMRECORD_STATE_RECORD_PAUSED == record_status)
        {
            MMIFM_RecordResumeRecord(TRUE);
        } 
#endif
    }
    return ret;
}
/*****************************************************************************/
//  Description : auto test Play FM
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_EngTestPlay(void)
{
    uint32  init_result = FM_SUCCESS;
    BOOLEAN ret = FALSE;

    //键盘音的停止处理需要放在 FM_Init 之前    
    if(!FM_IsInit())
    {
        init_result = FM_Init();
    }
    //SCI_TRACE_LOW:"[MMIFM] MMIAPIFM_EngTestPlay init_result %d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_511_112_2_18_2_19_21_6,(uint8*)"d", init_result);
    if(FM_SUCCESS == init_result)
    {
        //MMIFM_Exit(TRUE);   
        if(MMIFM_ReqPlaySrv())
        {
            //FM_SetScanLevel(FM_SCANLEVEL_MID);
            //MMIFM_SetVolume(FM_HEADSET_MODE, FM_VOL_LEVEL_4);
            MMISRVAUD_SetVolume(MMIFM_GetPlayHandle(), FM_VOL_LEVEL_5);
            //FM_ManualSeek(MMIFM_FREQ_INITIAL_VAL, FM_SCAN_UP, 0, NULL);
            //MMISRVAUD_Play(MMIFM_GetPlayHandle(), 0);
            MMIAPIFM_Play(MMIFM_GetPlayHandle(),875); //与工程模式中的默认频率相同
            ret = TRUE;
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : stop FM in eng test
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_EngTestStop(void)
{
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_EngTestStop"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_533_112_2_18_2_19_21_7,(uint8*)"");
    FM_Close();
    MMIFM_FreePlaySrv();
}
/*****************************************************************************/
//  Description : get FM's status, play, stop or suspended
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
MMIFM_STATUS_E MMIFM_GetStatus(void)
{    
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_GetStatus s_fm_cur_info.status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_543_112_2_18_2_19_22_8,(uint8*)"d", s_fm_cur_info.status);
    return s_fm_cur_info.status;
}
/*****************************************************************************/
//  Description : set FM's status, play, stop or suspended
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_SetStatus(MMIFM_STATUS_E status)
{ 
   s_fm_cur_info.status = status;
}
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : get FM Record File ID
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
MMIFM_STATUS_E MMIFM_GetRecordFileId(void)
{    
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_GetStatus s_fm_cur_info.fm_record_file_id %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_560_112_2_18_2_19_22_9,(uint8*)"d", s_fm_cur_info.fm_record_file_id);
    return s_fm_cur_info.fm_record_file_id;
}
#endif
/*****************************************************************************/
//  Description : intial FM module  
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIFM_InitModule(void)
{
    MMIFM_RegFMNv();
    MMIFM_RegWinIdNameArr();
#ifdef MMI_TASK_MANAGER
    MMIFM_RegAppletInfo();
#endif    
}
/*****************************************************************************/
//  Description : is valid FRE
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_IsValidFre(uint16 fre)
{
    BOOLEAN ret = FALSE;
    if(fre >= MMIFM_FREQ_RANGE_LOW && fre <= MMIFM_FREQ_RANGE_HIGH)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
        
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_IsValidFre ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_594_112_2_18_2_19_22_10,(uint8*)"d", ret);
    return ret;
}
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : adjust fm frequency, increase 0.1 MHz or decrease 0.1 MHz freq
//  Global resource dependence : 
//  Author:
//  Parameter: step[IN]: adjust steps, 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustFrequencyByFre(uint16 fre)
{
    BOOLEAN                     result = FALSE;
    uint16                      ch_index = 0;
    FM_SINGLE_CHANNEL_INFO_T    channel_info = {0};
    FM_SCAN_DIR_E               dir = FM_SCAN_DOWN;
    int32 step=fre-s_fm_cur_info.cur_freq;
    
    SCI_MEMSET(&channel_info, 0, sizeof(FM_SINGLE_CHANNEL_INFO_T));
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_AdjustFrequency step %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_612_112_2_18_2_19_22_11,(uint8*)"d", step);
    MMIFM_EnableMute();
    if(step > 0)
    {
        dir = FM_SCAN_UP;
    }
    else
    {
        dir = FM_SCAN_DOWN;
    }
    s_fm_cur_info.cur_freq = fre;

    if(s_fm_cur_info.cur_freq > MMIFM_FREQ_RANGE_HIGH)
    {
       s_fm_cur_info.cur_freq = MMIFM_FREQ_RANGE_HIGH;
    }
    else if(s_fm_cur_info.cur_freq < MMIFM_FREQ_RANGE_LOW)
    {
       s_fm_cur_info.cur_freq = MMIFM_FREQ_RANGE_LOW;
    }

    if(FM_SUCCESS == FM_ManualSeek(s_fm_cur_info.cur_freq, dir, 0, &channel_info))
    {
        ch_index = MMIFM_GetChannelIndexByFreq(s_fm_cur_info.cur_freq);
        if(MMIFM_INVALID_CHANNEL != ch_index)
        {
            s_fm_cur_info.cur_channel_index = ch_index;
        }
        SaveCurrentFMInfo(&s_fm_cur_info); 
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIFM] MMIFM_AdjustFrequency FM_ManualSeek failed !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_645_112_2_18_2_19_22_12,(uint8*)"");
        result = FALSE;
    }    
    MMIFM_DisableMute();
    return result;
}
#endif
/*****************************************************************************/
//  Description : adjust fm frequency, increase 0.1 MHz or decrease 0.1 MHz freq
//  Global resource dependence : 
//  Author:
//  Parameter: step[IN]: adjust steps, 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustFrequency(int32 step)
{
    BOOLEAN                     result = FALSE;
    uint16                      ch_index = 0;
    FM_SINGLE_CHANNEL_INFO_T    channel_info = {0};
    FM_SCAN_DIR_E               dir = FM_SCAN_DOWN;
    
    SCI_MEMSET(&channel_info, 0, sizeof(FM_SINGLE_CHANNEL_INFO_T));
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_AdjustFrequency step %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_665_112_2_18_2_19_22_13,(uint8*)"d", step);
    if(MMIFM_CheckHeadsetStatus())
    {
        MMIFM_EnableMute();
        if(step > 0)
        {
            dir = FM_SCAN_UP;        
        }
        else
        {
            dir = FM_SCAN_DOWN;
        }
        s_fm_cur_info.cur_freq = s_fm_cur_info.cur_freq + step* MMIFM_FREQ_SCALE;
#ifdef FM_NEW_UI
        if(s_fm_cur_info.cur_freq > MMIFM_FREQ_RANGE_HIGH)
        {
           s_fm_cur_info.cur_freq = MMIFM_FREQ_RANGE_HIGH;
        }
        else if(s_fm_cur_info.cur_freq < MMIFM_FREQ_RANGE_LOW)
        {
           s_fm_cur_info.cur_freq = MMIFM_FREQ_RANGE_LOW;
        }
#else
        if(s_fm_cur_info.cur_freq > MMIFM_FREQ_RANGE_HIGH)
        {
           s_fm_cur_info.cur_freq = MMIFM_FREQ_RANGE_LOW;
        }
        else if(s_fm_cur_info.cur_freq < MMIFM_FREQ_RANGE_LOW)
        {
           s_fm_cur_info.cur_freq = MMIFM_FREQ_RANGE_HIGH;
        }
#endif
        if(FM_SUCCESS == FM_ManualSeek(s_fm_cur_info.cur_freq, dir, 0, &channel_info))
        {
            s_fm_cur_info.cur_freq = channel_info.freq;
            ch_index = MMIFM_GetChannelIndexByFreq(s_fm_cur_info.cur_freq);
            if(MMIFM_INVALID_CHANNEL != ch_index)
            {
                s_fm_cur_info.cur_channel_index = ch_index;
            }
            SaveCurrentFMInfo(&s_fm_cur_info); 
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIFM] MMIFM_AdjustFrequency FM_ManualSeek failed !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_710_112_2_18_2_19_22_14,(uint8*)"");
            result = FALSE;
        }    
        MMIFM_DisableMute();
    }
    return result;
}

/*****************************************************************************/
//  Description : adjust fm channel
//  Global resource dependence : 
//  Author:
//  Parameter: step[IN]: adjust steps, 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_AdjustChannel(int32 step)
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_exist_channel = FALSE;
    uint16          new_channel_index = 0;
    MMIFM_CHANNEL_T channel = {0};
    FM_SINGLE_CHANNEL_INFO_T    single_channel_info = {0};
       
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_AdjustChannel step %d channel index %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_731_112_2_18_2_19_22_15,(uint8*)"dd", step, s_fm_cur_info.cur_channel_index);
    if(MMIFM_CheckHeadsetStatus())
    {
        MMIFM_EnableMute();
        
        if (step <= MMIFM_MAX_CHANNEL_NUM && step >= -MMIFM_MAX_CHANNEL_NUM)
        {
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
		if(MMIFM_GetNextValidChannelByFreq(s_fm_cur_info.cur_freq,step,&new_channel_index))
#else
            /*MINI get the next channel by index*/
            if(MMIFM_GetNextValidChannelByIndex(s_fm_cur_info.cur_freq,step,&new_channel_index))
#endif
            {
                new_channel_index = ((new_channel_index >= MMIFM_MAX_CHANNEL_NUM) ? 0 : new_channel_index);
                if(MMIFM_IsChannelExist(new_channel_index))
                {
                    s_fm_cur_info.cur_channel_index = (uint16)new_channel_index;
                    is_exist_channel = TRUE;
                }
                //SCI_TRACE_LOW:"MMIFM] MMIFM_AdjustChannel s_fm_cur_info.cur_channel_index %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_746_112_2_18_2_19_22_16,(uint8*)"d", s_fm_cur_info.cur_channel_index);
                if(is_exist_channel && MMIFM_GetOneChannel(s_fm_cur_info.cur_channel_index, &channel))
                {
                    if(channel.freqency > MMIFM_FREQ_RANGE_HIGH || channel.freqency < MMIFM_FREQ_RANGE_LOW)
                    {
                        //invalid channel
                        //SCI_TRACE_LOW:"[MMIFM]MMIFM_AdjustChannel channel.freqency %d !"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_752_112_2_18_2_19_22_17,(uint8*)"d", channel.freqency);
                    }
                    else if(FM_SUCCESS == FM_ManualSeek(channel.freqency, FM_SCAN_DOWN, 0, &single_channel_info))
                    {
                        s_fm_cur_info.cur_freq = channel.freqency;                 
                        SaveCurrentFMInfo(&s_fm_cur_info);
                        result = TRUE;
                    }
                }
            }
            
        }
    	MMIFM_DisableMute();
    }
    return result;
}

/*****************************************************************************/
//  Description : 切换声音设备
//  Global resource dependence : 
//  Author:
//  Parameter: 无
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_SwitchAudioDevice(void)
{
    BOOLEAN result = TRUE;
    //FM_OUTPUT_MODE_E  fm_out_mode = FM_OUTPUT_HEADSET;
    MMISRVAUD_ROUTE_T support_route = MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_SPEAKER;

//  SCI_TRACE_LOW:"[MMIFM] MMIFM_SwitchAudioDevice  status %d sound_mode "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_779_112_2_18_2_19_22_18,(uint8*)"d", s_fm_cur_info.status, s_fm_cur_info.sound_mode);
    switch (s_fm_cur_info.sound_mode)
    {
        case FM_SPEAKER_MODE:
        {
            s_fm_cur_info.sound_mode = FM_HEADSET_MODE;
            support_route = MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_SPEAKER;
        }
        break;

        case FM_HEADSET_MODE:
        {
            s_fm_cur_info.sound_mode = FM_SPEAKER_MODE;
            support_route = MMISRVAUD_ROUTE_SPEAKER;
        }
        break;
            
        default:
            result = FALSE;       
        break;
    }
//  SCI_TRACE_LOW:"MMIFM_SwitchAudioDevice s_fm_cur_info.sound_mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_794_112_2_18_2_19_22_19,(uint8*)"d", s_fm_cur_info.sound_mode);
    MMISRVAUD_SetAllSupportRoute(MMIFM_GetPlayHandle(),support_route);
    SaveCurrentFMInfo(&s_fm_cur_info);    
    return result;
}

/*****************************************************************************/
//  Description : 搜索下一个或上一个有效频率
//  Global resource dependence : 
//  Author:
//  Parameter: direct >= 0:搜索下一个有效频率; direct < 0 :上一个有效频率
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFM_SearchValidFreq(int32 direct)
{
    uint16                s_start_freq = s_fm_cur_info.cur_freq;
    uint16                ch_index = 0;
    FM_ALL_CHANNEL_INFO_T channel_info = {0};
    FM_SCAN_DIR_E         dir = FM_SCAN_UP;
    BOOLEAN               result = FALSE;
    uint32                ret = FM_SUCCESS;
    MMISRVAUD_VOLUME_T volume = 0;
    
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_SearchValidFreq  status %d direct %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_818_112_2_18_2_19_22_20,(uint8*)"dd", s_fm_cur_info.status, direct);
    volume = MMISRVAUD_GetVolume(MMIFM_GetPlayHandle());
    if(volume != 0)
    {
        MMIFM_EnableMute();
    }
    if(direct >= 0)
    {
       //search next
        dir = FM_SCAN_UP;
        if(s_start_freq >= MMIFM_FREQ_RANGE_HIGH)
        {
            //current freqency is highest. freqency. must start from lowest
            s_start_freq = MMIFM_FREQ_RANGE_LOW;
        }
    }
    else if(direct < 0)
    {
        //search previous 
        dir = FM_SCAN_DOWN;
        if(s_start_freq <= MMIFM_FREQ_RANGE_LOW)
        {
             //current freqency is lowest, search from highest
            s_start_freq = MMIFM_FREQ_RANGE_HIGH;
        }
    }
    s_start_freq = MIN(MMIFM_FREQ_RANGE_HIGH, s_start_freq);
    s_start_freq = MAX(MMIFM_FREQ_RANGE_LOW, s_start_freq);
        ret = FM_AutoSeek(s_start_freq, dir, FM_SCAN_ONE, &channel_info);
        if(FM_SUCCESS == ret)// FM_SCAN_ON
        {
            ch_index = MMIFM_GetChannelIndexByFreq(channel_info.auto_freq_tab_ptr[0]);
            if(MMIFM_INVALID_CHANNEL != ch_index)
            {
                s_fm_cur_info.cur_channel_index = ch_index;
            }
            s_fm_cur_info.cur_freq = channel_info.auto_freq_tab_ptr[0];
            SaveCurrentFMInfo(&s_fm_cur_info);
            result = MMIFM_AUTOSEARCH_END;
        }
		else if(FM_NOT_COMPLETE == ret )
        { 
			result = MMIFM_AUTOSEARCH_NOT_COMPLETE;
        }
        else
        {
            result = MMIFM_AUTOSEARCH_FAIL;
        }

        if(volume != 0)
        {
            MMIFM_DisableMute();
        }
        return result;
}
/*****************************************************************************/
//  Description : stop fm playing
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Stop(void)
{
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_Stop"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_879_112_2_18_2_19_22_21,(uint8*)"");
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)
    MMIFM_StopRecord();
#endif
    MMIFM_StopAutoSearch();
    FM_Close();
//  MMIFM_FreePlaySrv();
    s_fm_cur_info.status = FM_STOP;
    s_fm_cur_info.is_bgplay = FALSE;
    SaveCurrentFMInfo(&s_fm_cur_info);
    MAIN_SetIdleBgPlayName(FALSE, MMIBGPLAY_TYPE_FM);
#ifndef SUBLCD_SIZE_NONE
    MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_FM,PNULL);
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : pause fm playing
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Pause(BOOLEAN suspend_by_fm)
{
#ifdef MMIFM_SUPPORT_FM_RECORD
    MMIFMRECORD_STATE_E record_status = MMIFM_GetRecordStatus();
#endif    
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_Pause"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_899_112_2_18_2_19_22_22,(uint8*)"");
        
    if(FM_PLAYING == MMIFM_GetStatus() 
#ifdef MMIFM_SUPPORT_FM_RECORD
        || MMIFMRECORD_STATE_RECORDING == record_status
#endif
        )
    {
        FM_PlayStop();
        MMISRVAUD_Stop(MMIFM_GetPlayHandle());
        s_fm_cur_info.status = FM_SUSPENDED;
        MMIFM_StopAutoSearch();
        MMIFM_SetSuspendByFM(suspend_by_fm);
#ifdef MMIFM_SUPPORT_FM_RECORD
        if(MMIFMRECORD_STATE_RECORDING == record_status)
        {            
            MMIFM_RecordPauseRecord(TRUE);
        }
#endif
        MMIFM_FreePlaySrv();
    }      
    return TRUE;
}

/*****************************************************************************/
//  Description : 退出FM
//  Global resource dependence : 
//  Author:
//  Parameter: TRUE: only exit fm, do not stop fm play; FALSE:exit fm, and stop fm play
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_Exit(BOOLEAN is_set_bg_play)
{
    if(!is_set_bg_play)
    {
        MMIFM_Stop();
    }    
    else
    {
        //stop other bg play
        s_fm_cur_info.is_bgplay = TRUE;
        MAIN_SetIdleBgPlayName(TRUE, MMIBGPLAY_TYPE_FM);
#ifndef SUBLCD_SIZE_NONE
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_FM,PNULL);
#endif
        SaveCurrentFMInfo(&s_fm_cur_info);
    }    
    
#ifndef MMI_TASK_MANAGER
    //close all of fm windows    
    MMIFM_ExitAllWindows();
#else
    MMIAPIFM_StopApplet(is_set_bg_play);
#endif

    //SCI_TRACE_LOW:"[MMIFM] MMIFM_Exit is_set_bg_play %d s_fm_cur_info.is_bgplay 2 %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_947_112_2_18_2_19_22_23,(uint8*)"dd", is_set_bg_play, s_fm_cur_info.is_bgplay);

#ifdef PDA_UI_DROPDOWN_WIN
    if(is_set_bg_play)
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_FM);
    }
    else
    {
//        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_RUNNING_FM);
    }
#endif
    if(!is_set_bg_play)
    {
        MMIFM_FreePlaySrv();
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)        
        MMIFM_FreeRecSrv();
#endif
    }
//  MMISRVAUD_FreeVirtualHandle("FM PREPROCESS");
    return TRUE;
}
/*****************************************************************************/
//  Description : get fm audio current device mode
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_AUDIO_DEVICE_MODE_E MMIFM_GetFMCurrentDeviceMode(void)
{
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_GetFMCurrentDeviceMode sound_mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1019_112_2_18_2_19_23_25,(uint8*)"d",s_fm_cur_info.sound_mode);
    return s_fm_cur_info.sound_mode;
}
/*****************************************************************************/
//  Description : set fm audio device mode
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_SetFMDeviceMode(MMIFM_AUDIO_DEVICE_MODE_E sound_mode)
{
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_SetFMDeviceMode sound_mode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1028_112_2_18_2_19_23_26,(uint8*)"d",sound_mode);
    s_fm_cur_info.sound_mode = sound_mode;
    SaveCurrentFMInfo(&s_fm_cur_info);
}
/*****************************************************************************/
//  Description : auto search freqency for every channels
//  Global resource dependence : 
//  Parameter : pre_search_channel [IN] current channel need to searched
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFM_AuotSearchFreq(uint16 cur_search_channel)
{
    MMIFM_CHANNEL_T one_channel = {0};
    FM_ALL_CHANNEL_INFO_T all_channel = {0};
    uint16 result = MMIFM_AUTOSEARCH_FAIL;
    uint32 ret = FM_SUCCESS;
    uint32  init_result = FM_SUCCESS;
    if(MMIFM_CheckHeadsetStatusWithoutPrompt())
    {
      if(!FM_IsInit())
      {
        init_result = FM_Init();
      }
      if(FM_SUCCESS == init_result)
      {
      //SCI_TRACE_LOW:"[MMIFM] MMIFM_AuotSearchFreq cur_search_channel %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1044_112_2_18_2_19_23_27,(uint8*)"d", cur_search_channel);
      if(cur_search_channel < MMIFM_MAX_CHANNEL_NUM)
      {
        one_channel.freqency = MMIFM_FREQ_RANGE_LOW;
        if(cur_search_channel > 0)
        {
            //search start from the prevoius channel's freqency
            MMIFM_GetOneChannel(cur_search_channel -1, &one_channel);
        }
        if(one_channel.freqency > MMIFM_FREQ_RANGE_HIGH || one_channel.freqency < MMIFM_FREQ_RANGE_LOW)
        {
            //SCI_TRACE_LOW:"[MMIFM] MMIFM_AuotSearchFreq one_channel.freqency %d !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1055_112_2_18_2_19_23_28,(uint8*)"d", one_channel.freqency);
            result = MMIFM_AUTOSEARCH_FAIL;
        }
        else 
        {
            ret = FM_AutoSeek(one_channel.freqency, FM_SCAN_UP, FM_SCAN_ONE, &all_channel);
            //SCI_TRACE_LOW:"[MMIFM] MMIFM_AuotSearchFreq one_channel.freqency %d seeked freqency %d, ret = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1062_112_2_18_2_19_23_29,(uint8*)"ddd", one_channel.freqency, all_channel.auto_freq_tab_ptr[0], ret);
            if(FM_SUCCESS == ret)
            {

                if(all_channel.auto_freq_tab_ptr[0] <= MMIFM_FREQ_RANGE_HIGH)
            {
                if(all_channel.auto_freq_tab_ptr[0] < one_channel.freqency )
                {
                    //search finished
                    result = MMIFM_AUTOSEARCH_END;
                }
                else if(MMIFM_INVALID_CHANNEL != MMIFM_GetChannelIndexByFreq(all_channel.auto_freq_tab_ptr[0]))
                {
                    //exist duplicate frequence, end
                    //SCI_TRACE_LOW:"[MMIFM] MMIFM_AuotSearchFreq duplicate frequency!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1076_112_2_18_2_19_23_30,(uint8*)"");
                    result = MMIFM_AUTOSEARCH_END;

                    }
                    else
                    {
                        //continue search
                        SCI_MEMSET(one_channel.name, 0x00, sizeof(one_channel.name));
                        one_channel.freqency = all_channel.auto_freq_tab_ptr[0];
                        MMIFM_SetOneChannel(cur_search_channel, &one_channel);
                        result = MMIFM_AUTOSEARCH_CONTINUE;
                    }
                }
            }
            else if(FM_NOT_COMPLETE == ret)
            {
                if(all_channel.auto_freq_tab_ptr[0] < one_channel.freqency )
                {
                    // search finished, but need re-open FM to stop serching job in FM chip
                    FM_Close();
                    FM_Init();
                    result = MMIFM_AUTOSEARCH_END;
                }
                else
                {
                    result = MMIFM_AUTOSEARCH_NOT_COMPLETE;
                }
            }
            else
            {
                result = MMIFM_AUTOSEARCH_FAIL;
            }
        }
    }
    else
    {
        result = MMIFM_AUTOSEARCH_END;
    }
  }
}
    return result;
}

/*****************************************************************************/
//  Description : delete one channel
//  Global resource dependence : 
//  Parameter : channel[IN]: channel index to be delete
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_DeleteOneChannel(uint16 ch_index)
{
    MMIFM_CHANNEL_T channel_info = {0};
    return WriteChannelToNv(ch_index, &channel_info);
}

/*****************************************************************************/
//  Description : delete all channels
//  Global resource dependence : 
//  Parameter : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_DeleteAllChannels(void)
{
    BOOLEAN ret = TRUE;
    uint16   i  = 0;
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
        if(!MMIFM_DeleteOneChannel((uint16)i))
        {
           ret = FALSE;
           break;
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : delete one channel
//  Global resource dependence : 
//  Parameter : channel_index[IN]: channel index to be saved
//              channel_ptr[IN]: channel info to be saved
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_SetOneChannel(uint16 channel_index, const MMIFM_CHANNEL_T *channel_ptr)
{
    BOOLEAN         ret = FALSE;

    if(MMIFM_IsChannelValid(channel_ptr))
    {
        ret = WriteChannelToNv(channel_index, channel_ptr);
    }
    return ret;
}

/*****************************************************************************/
//  Description : delete one channel
//  Global resource dependence : 
//  Parameter : channel_index[IN]: channel index to be readed
//              channel_ptr[OUT]: the information of this channel
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_GetOneChannel(uint16 channel_index, MMIFM_CHANNEL_T *channel_ptr)
{
    BOOLEAN         ret = TRUE;
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    uint32             item_id = MMIFM_FIRST_CHANNEL + channel_index;

    if(PNULL != channel_ptr)
    {
        SCI_MEMSET(channel_ptr, 0x00, sizeof(MMIFM_CHANNEL_T));
        if(MMIFM_FIRST_CHANNEL <= item_id && (item_id <= MMIFM_LAST_CHANNEL)) 
        {
            nv_result = MMI_ReadNVItemEx( item_id, sizeof(MMIFM_CHANNEL_T), (void*)channel_ptr );
        }
        if(channel_ptr->freqency > MMIFM_FREQ_RANGE_HIGH || channel_ptr->freqency < MMIFM_FREQ_RANGE_LOW)
        {
            nv_result = MN_RETURN_FAILURE;
        }
    }
    ret = ((nv_result == MN_RETURN_SUCCESS) ? TRUE : FALSE);

    return ret;
}

/*****************************************************************************/
//  Description : get fm current inof
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_CUR_INFO_T * MMIFM_GetFMCurrentInfo(void)
{
    return &s_fm_cur_info;
}
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : Get FM Timer Record Info
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFM_TIMER_RECORD_INFO_T * MMIFM_GetFMTimerRecordInfo(void)
{
    return &s_fm_timer_record_info;
}
#endif
/*****************************************************************************/
//  Description : play start from the channel frequency
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_PlayChannel(uint16 ch_index)
{
    BOOLEAN ret = FALSE;
    MMIFM_CHANNEL_T channel = {0};

    //SCI_TRACE_LOW:"[MMIFM] MMIFM_PlayChannel ch_index %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1219_112_2_18_2_19_23_31,(uint8*)"d", ch_index);

    if(!MMIFM_GetOneChannel(ch_index, &channel))
    {
        //SCI_TRACE_LOW:"[MMIFM] MMIFM_PlayChannel MMIFM_GetOneChannel fail !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1223_112_2_18_2_19_23_32,(uint8*)"");
        return ret;
    }
    if(channel.freqency > MMIFM_FREQ_RANGE_HIGH || channel.freqency < MMIFM_FREQ_RANGE_LOW)
    {
        //SCI_TRACE_LOW:"[MMIFM] MMIFM_PlayChannel channel.freqency %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1228_112_2_18_2_19_23_33,(uint8*)"d", channel.freqency);
        return ret;
    }    
    ret = MMIFM_PlayFreq(channel.freqency);
    
    return ret;
}

/*****************************************************************************/
//  Description : get channel index by frequency
//  Global resource dependence : 
//  Author:
//  return return value is not MMIFM_INVALID_CHANNEL, the frequency is in channel
//  Note: 
/*****************************************************************************/
PUBLIC uint16  MMIFM_GetChannelIndexByFreq(uint16 frequency)
{
    uint16 i = 0;
    uint16 ch_index = MMIFM_INVALID_CHANNEL;
    MMIFM_CHANNEL_T channel = {0};

    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
         if(MMIFM_GetOneChannel((uint16)i, &channel))
         {
             if(frequency == channel.freqency)
             {
                 ch_index = i;
                 break;
             }
         }
    }
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_GetChannelIndexByFreq ch_index %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1258_112_2_18_2_19_23_34,(uint8*)"d", ch_index);
    return ch_index;
}

/*****************************************************************************/
//  Description : save current info
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  SaveCurrentFMInfo(const MMIFM_CUR_INFO_T *cur_info_ptr)
{
    BOOLEAN ret = FALSE;
    if(PNULL != cur_info_ptr)
    {
        MMI_WriteNVItemEx( MMIFM_NV_CUR_INFO, sizeof(MMIFM_CUR_INFO_T), (void *)cur_info_ptr );
        ret = TRUE;
    }
    return ret;
}

/*****************************************************************************/
//  Description : get fm out put device by mmi fm device mode
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL FM_OUTPUT_MODE_E  GetFMDeviceByAudioMode(const MMIFM_AUDIO_DEVICE_MODE_E sound_mode)
{
    FM_OUTPUT_MODE_E device = FM_OUTPUT_HEADSET;

    switch(sound_mode)
    {
    case FM_SPEAKER_MODE:
        device = FM_OUTPUT_SPEAKER;
        break;
    case FM_HEADSET_MODE:
        device = FM_OUTPUT_HEADSET;
        break;
    default:
        device = FM_OUTPUT_HEADSET;
        break;
    }
    //SCI_TRACE_LOW:"[MMIFM]GetFMDeviceByAudioMode device %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1300_112_2_18_2_19_23_35,(uint8*)"d", device);
    return device;
}

/*****************************************************************************/
//  Description : read current info
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  ReadCurrentFMInfo(MMIFM_CUR_INFO_T *cur_info_ptr)
{
    BOOLEAN ret = FALSE;
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    if(PNULL != cur_info_ptr)
    {
        nv_result = MMI_ReadNVItemEx( MMIFM_NV_CUR_INFO, sizeof(MMIFM_CUR_INFO_T), (void*)cur_info_ptr );
    }
    ret = ((nv_result == MN_RETURN_SUCCESS) ? TRUE : FALSE);
    return ret;
}

/*****************************************************************************/
//  Description : is channel exist
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_IsChannelExist(uint16 ch_index)
{
    BOOLEAN ret = FALSE;
    MMIFM_CHANNEL_T channel = {0};

    if(ch_index < MMIFM_MAX_CHANNEL_NUM)
    {
        if(MMIFM_GetOneChannel(ch_index, &channel))
        {
            if(MMIFM_IsChannelValid(&channel))
            {
                ret = TRUE;
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_IsChannelExist ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1341_112_2_18_2_19_23_36,(uint8*)"d", ret);
    return ret;
}

/*****************************************************************************/
//  Description : is valid channel
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_IsChannelValid(const MMIFM_CHANNEL_T *channel_ptr)
{
    BOOLEAN ret = FALSE;
    if(PNULL != channel_ptr)
    {
        if(channel_ptr->freqency >= MMIFM_FREQ_RANGE_LOW && channel_ptr->freqency <= MMIFM_FREQ_RANGE_HIGH)
        {
            ret = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIFM] MMIFM_IsChannelValid channel_ptr->freqency %d !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1361_112_2_18_2_19_23_37,(uint8*)"d", channel_ptr->freqency);
        }
        
    }
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_IsChannelValid ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1365_112_2_18_2_19_23_38,(uint8*)"d", ret);
    return ret;
}

/*****************************************************************************/
//  Description : get exist channel total count
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC uint16  MMIFM_GetValidChannelCount(void)
{
    uint16 ch_count = 0;
    uint16  i  =0;

    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
       if(MMIFM_IsChannelExist(i))
       {
           ch_count++;
       }
    }
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_GetValidChannelCount ch_count %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1386_112_2_18_2_19_23_39,(uint8*)"d", ch_count);
    return ch_count;
}


/*****************************************************************************/
//  Description : write channle info to nv
//  Global resource dependence : 
//  Parameter : channel_index[IN]: channel index to be saved
//              channel_ptr[IN]: channel info to be saved
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WriteChannelToNv(uint16 channel_index, const MMIFM_CHANNEL_T *channel_ptr)
{
    BOOLEAN             ret = FALSE;
    uint32              item_id = MMIFM_FIRST_CHANNEL + channel_index;
   
    if(channel_ptr != PNULL && MMIFM_FIRST_CHANNEL <= item_id && (item_id <= MMIFM_LAST_CHANNEL)) 
    {
        MMI_WriteNVItemEx( item_id, sizeof(MMIFM_CHANNEL_T), (void*)channel_ptr );
        ret = TRUE;
    }
    return ret;
}

/*****************************************************************************/
//  Description : set current channel index
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_SetCurrrentChannel(const uint16 ch_index)
{
    if(ch_index >= MMIFM_MAX_CHANNEL_NUM)
    {
        //SCI_TRACE_LOW:"[MMIFM] MMIFM_SetCurrrentChannel ch_index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1420_112_2_18_2_19_23_40,(uint8*)"d", ch_index);
        return FALSE;
    }
    s_fm_cur_info.cur_channel_index = ch_index;                 
    SaveCurrentFMInfo(&s_fm_cur_info);
    return TRUE;
}
/*****************************************************************************/
//  Description : Play fm freq for others app module
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_PlayByAudioHandle(uint16 input_freq,MMISRV_HANDLE_T audio_handle)
{
    uint32  init_result = FM_SUCCESS;
    BOOLEAN ret = FALSE;

    if(MMIFM_CheckHeadsetStatus())
    {   
        if(MMIFM_FREQ_RANGE_LOW <= input_freq && input_freq <= MMIFM_FREQ_RANGE_HIGH)
        {
            if(!FM_IsInit())
            {
                init_result = FM_Init();
            }
                    
            if(FM_SUCCESS == init_result)
            {
                if(FM_SUCCESS == FM_ManualSeek(input_freq, FM_SCAN_UP, 0, NULL))
                {
                    s_fm_cur_info.export_freq = input_freq;
                    MMISRVAUD_Play(audio_handle, 0);
                    SaveCurrentFMInfo(&s_fm_cur_info);
                    ret = TRUE;
                }
            }
        }
        //SCI_TRACE_LOW:"[MMIFM] MMIFM_PlayByAudioHandle ret %d !"
        SCI_TRACE_LOW("[MMIFM] MMIFM_PlayByAudioHandle audio_handle %d !",audio_handle);
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1495_112_2_18_2_19_23_41,(uint8*)"d", ret);
    }
    return ret;
}
/*****************************************************************************/
//  Description : Play specified freq
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_PlayFreq(uint16 freq)
{    
    BOOLEAN ret = FALSE;
    uint16  ch_index = 0;
    uint32  init_result = FM_SUCCESS;  
    //MMISRV_HANDLE_T handle = MMIFM_GetPlayHandle();

    if(MMIFM_CheckHeadsetStatus())
    {    
        MMIFM_EnableMute();
        if(MMIFM_FREQ_RANGE_LOW <= freq && freq <= MMIFM_FREQ_RANGE_HIGH)
        {
            if(!FM_IsInit())
            {
                init_result = FM_Init();
            }
                    
            if(FM_SUCCESS == init_result && MMIFM_ReqPlaySrv())
            {
                if(FM_SUCCESS == FM_ManualSeek(freq, FM_SCAN_UP, 0, NULL))
                {
                    //fm_mode = s_fm_cur_info.sound_mode;                        
                    ch_index = MMIFM_GetChannelIndexByFreq(freq);
                    if(MMIFM_INVALID_CHANNEL != ch_index)
                    {
                        s_fm_cur_info.cur_channel_index = ch_index;
                    }
                    s_fm_cur_info.cur_freq = freq;
                    
                    if(FM_PLAYING != s_fm_cur_info.status)
                    {
                        MMISRVAUD_Play(MMIFM_GetPlayHandle(), 0); 
                    }

                    SaveCurrentFMInfo(&s_fm_cur_info);
                    ret = TRUE;
                }
            }
        }
        MMIFM_DisableMute();
        SCI_TRACE_LOW("[MMIFM] MMIFM_PlayFreq ret %d !",ret);
    }
    return ret;
}
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : get record status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_STATE_E MMIFM_GetRecordStatus(void)
{
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_GetRecordStatus(), status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1510_112_2_18_2_19_24_42,(uint8*)"d", s_current_state);
    return s_current_state;
}

/*****************************************************************************/
//  Description : handle record end.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIFM_GetRecordFileName(void)
{
    return PNULL;
}
/*****************************************************************************/
//  Description : handle record end.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIFM_GetRecordFilePath(void)
{
    return s_fm_record_file_path;
}
/*****************************************************************************/
//  Description : 开始录音（通常情况）
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIFM_StartRecord(void)
{
    MMIFILE_ERROR_E error_code = SFS_ERROR_NONE;
// #ifdef MMIFM_SUPPORT_FM_RECORD    
//      BOOLEAN         aud_record_result = FALSE;
// #endif
    MMIFILE_DEVICE_E    file_dev = MMIFMRECORD_GetRecordStorage();
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_StartRecord"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1541_112_2_18_2_19_24_43,(uint8*)"");
    
    if (MMIFMRECORD_STATE_NONE != s_current_state)
    {
        error_code = SFS_ERROR_ABORT;
        return error_code;
    } 

    if(!MMIFM_IsEnoughSpaceToRecord())
    {   
        //修改coverity扫描结果
        error_code = MMIAPIFMM_GetSuitableFileDev(file_dev, MMIFMRECORD_MIN_NEED_SPACE, &file_dev);
        if(SFS_ERROR_NONE != error_code)
        {   
            return error_code;
        }
        MMIFMRECORD_SetRecordStorage(file_dev);

        if(!MMIFM_IsEnoughSpaceToRecord())
        {
            error_code = SFS_ERROR_NO_SPACE;
        }
        else
        {
            error_code = SFS_NO_ERROR;
        }
    }
    
    if (SFS_ERROR_NONE != error_code)
    {
        return error_code;
    }
    
    if(!CreateFileName(s_fm_record_file_path))
    {
        return SFS_ERROR_FOLD_FULL;
    }
    //SCI_TRACE_LOW:"[MMIFMRECORD]CreateFileName() s_fm_record_file_path = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1573_112_2_18_2_19_24_44,(uint8*)"d", s_fm_record_file_path);
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)        
#ifdef MMIFM_SUPPORT_FORMAT_AMR 
    if(MMIFM_ReqRecSrv(MMIFMRECORD_GetFileFormat(), s_fm_record_file_path))
#else
    if(MMIFM_ReqRecSrv(MMISRVAUD_RECORD_FMT_ADPCM, s_fm_record_file_path))
#endif
    {
        MMISRVAUD_Play(MMIFM_GetRecHandle(), 0);
        s_current_state = MMIFMRECORD_STATE_RECORDING;
    }
    else
#endif
    {
        s_current_state = MMIFMRECORD_STATE_NONE;
        error_code = SFS_ERROR_ABORT;
    }
    return error_code;
}

/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIFMRECORD_GetRecordStorage(void)
{
    return s_fm_cur_info.dev;
}
/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMRECORD_SetRecordStorage(MMIFILE_DEVICE_E storage)
{
    //SCI_TRACE_LOW:"[MMIFMRECORD]:MMIFMRECORD_SetRecordStorage storage=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1603_112_2_18_2_19_24_45,(uint8*)"d",storage);
    s_fm_cur_info.dev = storage;
    SaveCurrentFMInfo(&s_fm_cur_info);
}
#endif
#ifdef MMIFM_SUPPORT_FORMAT_AMR 
/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RECORD_FMT_E MMIFMRECORD_GetFileFormat(void)
{
    return s_fm_cur_info.record_file_format;
}
/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMRECORD_SetFileFormat(MMISRVAUD_RECORD_FMT_E file_format)
{
    SCI_TRACE_LOW("[MMIFMRECORD]:MMIFMRECORD_SetFileFormat file_format=%d",file_format);    
    s_fm_cur_info.record_file_format = file_format;
    SaveCurrentFMInfo(&s_fm_cur_info);
}
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE 
/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_EQ_MODE_E MMIFM_GetEqMode(void)
{
    return s_fm_cur_info.fm_eq_mode;
}
/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_SetEqMode(MMISRVAUD_EQ_MODE_E fm_eq_mode)
{
    SCI_TRACE_LOW("[MMIFM]:MMIFM_SetEqMode fm_eq_mode=%d",fm_eq_mode);    
    s_fm_cur_info.fm_eq_mode = fm_eq_mode;
    SaveCurrentFMInfo(&s_fm_cur_info);
    MMISRVAUD_SetEQMode(MMIFM_GetPlayHandle(),fm_eq_mode);
}
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : assign a default name
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateFileName(wchar* input_file_path)
{  
    BOOLEAN ret = FALSE;
    const wchar*    file_dev_ptr = PNULL;
    uint16          file_dev_len = 0;
    wchar *         full_path = PNULL;
    uint16          full_path_len = MMIFMRECORD_MAX_FILE_NAME_LEN;
    char *          temp_photo_name = PNULL;
    uint32          name_id = 0;
    uint32          id_max = FMRECORD_FILE_NAME_ID_MAX; 
    uint32 step = 1;
    uint32 circle = 1;
    MMIFILE_DEVICE_E file_dev = MMIFMRECORD_GetRecordStorage();
    wchar*          fm_record_filename = PNULL;
    uint32 max_count = FMRECORD_FILE_SEARCH_MAX_COUNT;
    uint32 count = 0;

    //SCI_TRACE_LOW:"[MMIFMRECORD] CreateFileName()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1623_112_2_18_2_19_24_46,(uint8*)"");
    
    if (PNULL == input_file_path)
    {
        //SCI_TRACE_LOW:"CreateFileName PNULL == input_file_path!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1627_112_2_18_2_19_24_47,(uint8*)"");
        return ret;
    }
    name_id = MMIFM_GetRecordFileId();
    
    fm_record_filename = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
    SCI_MEMSET(fm_record_filename, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
    full_path = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
    SCI_MEMSET(full_path, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
    temp_photo_name = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(char));
    SCI_MEMSET(temp_photo_name, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(char)));
        
    while(count < max_count) /*lint !e716*/
    {
        name_id += step;
        step = step * 2;
        if(name_id >= id_max)
        {
          circle ++;
          if(circle >= id_max)
          {
             if(PNULL != fm_record_filename) 
             {
               SCI_FREE(fm_record_filename);
               fm_record_filename = PNULL;
             }
             if(PNULL != full_path)  
             {
               SCI_FREE(full_path);
               full_path = PNULL;
             }
             if(PNULL != temp_photo_name)    
             {
               SCI_FREE(temp_photo_name);
               temp_photo_name = PNULL;
             }            
             s_fm_cur_info.fm_record_file_id = 0;
             SaveCurrentFMInfo(&s_fm_cur_info);
             ret = FALSE;
             break;
           }
           name_id = circle;
           step = 1;
         }
				
        s_fm_cur_info.fm_record_file_id = name_id;
        SaveCurrentFMInfo(&s_fm_cur_info);
        SCI_MEMSET(temp_photo_name, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(char)));
#ifdef MMIFM_SUPPORT_FORMAT_AMR
        if(MMISRVAUD_RECORD_FMT_AMR == MMIFMRECORD_GetFileFormat())
        {
            MMIFM_GetFileIDString(temp_photo_name, "FM", name_id, id_max, "amr");
        }
        else
#endif
        {
            MMIFM_GetFileIDString(temp_photo_name, "FM", name_id, id_max, "wav");
        }        
        SCI_MEMSET(fm_record_filename, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
        MMIAPICOM_StrToWstr(temp_photo_name, fm_record_filename);/*lint !e64*/

		/*if (FS_UDISK == file_dev)
		}*/
		file_dev_ptr = MMIAPIFMM_GetDevicePath((file_dev));
		file_dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));

        SCI_MEMSET(full_path, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
        fm_record_filename, (uint16)MMIAPICOM_Wstrlen(fm_record_filename), full_path, &full_path_len);
        //SCI_ASSERT(full_path_len <= MMIFMRECORD_MAX_FILE_NAME_LEN);/*assert verified*/
        
        //SCI_TRACE_LOW:"[MMIFMRECORD]CreateFileName() full_path = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1679_112_2_18_2_19_24_48,(uint8*)"d", full_path);
        
        if(!MMIAPIFMM_IsFileExist(full_path, full_path_len))
        {
            MMIAPICOM_Wstrcpy(input_file_path, full_path);

            SCI_FREE(fm_record_filename);
            fm_record_filename = PNULL;

            SCI_FREE(full_path);
            full_path = PNULL;

            SCI_FREE(temp_photo_name);
            temp_photo_name = PNULL;
			
            ret = TRUE;
	        break;
        }
	    count++;
    }
    return ret;
}
/*****************************************************************************/
//  Description : handle record end.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordConstructor(void)
{
    if(PNULL == s_fm_record_file_path)
    {
        s_fm_record_file_path = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
        SCI_MEMSET(s_fm_record_file_path, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1) * sizeof(wchar)));
    }
}

/*****************************************************************************/
//  Description : handle record end.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordDestructor(void)
{
    if(PNULL != s_fm_record_file_path)
    {
        SCI_FREE(s_fm_record_file_path);
        s_fm_record_file_path = PNULL;
    }
}
/*****************************************************************************/
//  Description : 暂停record
//  Global resource dependence : none
//  Parameter: suspend_with_fm,TRUE:和fm同时暂停;FALSE:record界面自己暂停
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordPauseRecord(BOOLEAN suspend_with_fm)
{
#ifdef MMIFM_SUPPORT_FM_RECORD
    //SCI_TRACE_LOW:"[MMIFM] enter funciton MMIFM_RecordPauseRecord()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1731_112_2_18_2_19_24_49,(uint8*)"");
    MMISRVAUD_Pause(MMIFM_GetRecHandle());
    MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_RECORD_PAUSED);    
    MMIFM_SetPausedWithFM(suspend_with_fm);
#endif
}
/*****************************************************************************/
//  Description : 继续录音
//  Global resource dependence : none
//  Parameter: resume_with_fm,TRUE:和fm同时恢复;FALSE:record界面自己恢复
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordResumeRecord(BOOLEAN resume_with_fm)
{
#ifdef MMIFM_SUPPORT_FM_RECORD
    //SCI_TRACE_LOW:"[MMIFM] enter funciton MMIFM_RecordResumeRecord()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1745_112_2_18_2_19_24_50,(uint8*)"");
    if(resume_with_fm == MMIFM_GetPausedWithFM())
    {
        MMISRVAUD_Resume(MMIFM_GetRecHandle());
        MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_RECORDING);
    }
#endif
}
/*****************************************************************************/
//  Description : set record status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_E state)
{
    s_current_state = state;
}
/*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_RESULT_E MMIFM_RecordStopRecord(void)
{
    MMIFMRECORD_RESULT_E error_code = MMIFMRECORD_RESULT_SUCCESS;

    //SCI_TRACE_LOW:"[MMIRECORD] enter funciton MMIRECORD_StopRecord()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1769_112_2_18_2_19_24_51,(uint8*)"");

    if (MMIFMRECORD_STATE_RECORDING != s_current_state && MMIFMRECORD_STATE_RECORD_PAUSED != s_current_state)
    {
        //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_StopRecord() error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1773_112_2_18_2_19_24_52,(uint8*)"");
        error_code = MMIFMRECORD_RESULT_ERROR;
        return error_code; 
    }
    MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_NONE);
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)         
    MMIFM_FreeRecSrv();
#endif
    return error_code;
}
/*****************************************************************************/
//  Description : get finally slash index
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIFM_RecordGetFinallySlashIndex(const wchar *file_name_ptr,const int32 file_len)
{
    int32 i = 0;

    if (PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIFM_RecordGetFinallySlashIndex PNULL == file_name_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1793_112_2_18_2_19_24_53,(uint8*)"");
        return 0;
    }
    for ( i = file_len; file_name_ptr[i] != MMIFILE_SLASH && i > 0; i-- )
    {
        ;
    }
    //SCI_TRACE_LOW:"MMIFM_RecordGetFinallySlashIndex i =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_1800_112_2_18_2_19_24_54,(uint8*)"d",i);
    return (uint16)i;
}
/*****************************************************************************/
//  Description : open record file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RecordOpenRecordFileList(void)
{
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    wchar*              full_path = PNULL;
    uint16              full_path_len = MMIFMRECORD_MAX_FILE_NAME_LEN;
    MMIFILE_DEVICE_E    file_dev = MMIFMRECORD_GetRecordStorage();
    uint32              file_suffix = MMIFMM_MUSIC_WAV;
    full_path = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)*sizeof(wchar));
    SCI_MEMSET(full_path, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));

    //check directory
	file_dev_ptr = MMIAPIFMM_GetDevicePath((file_dev));
	file_dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
    MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
    PNULL, 0, full_path, &full_path_len);
	/*if(FS_MMC == file_dev)
	}*/




#ifdef DRM_SUPPORT
    file_suffix |= MMIFMM_DRM_ALL;
#endif
    MMIAPIFMM_OpenExplorerExt(full_path, full_path_len,PNULL,0,FALSE, file_suffix);    
    
    SCI_FREE(full_path);

    return;
}
/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_RESULT_E MMIFM_RecordDeleteTempFile(void)
{
    MMIFMRECORD_RESULT_E result = MMIFMRECORD_RESULT_SUCCESS;
    uint16 cur_file_len = MMIAPICOM_Wstrlen(s_fm_record_file_path);

    if(!MMIAPIFMM_IsFileExist(s_fm_record_file_path, cur_file_len))
    {
        return MMIFMRECORD_RESULT_ERROR;
    }
    else
    {        
        if (0 != cur_file_len)
        {
            if (!MMIAPIFMM_DeleteFileSyn(s_fm_record_file_path, cur_file_len))
            {
                result = MMIFMRECORD_RESULT_ERROR;
            }
        }
    }
    return result;
}
/*****************************************************************************/
//  Description : create capture photo name
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIFM_GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type)
{
    uint16 id_len = 0;
    uint16 id_max_len = 0;
    char string_max[20] = {0};
    uint32 i = 0;
 
    if(id >= id_max)
    {        
        id = id_max - 100;
    }

    id_len = sprintf(file_name, "%d", id);
    id_max_len = sprintf(string_max, "%d", id_max);
    for(i = 0; i < id_len; i++)
    {
        string_max[id_max_len - id_len + i] = file_name[i];
    }
    string_max[0] = '_';
    string_max[id_max_len] = '.';
    id_len = sprintf(file_name, "%s%s%s", prefix, string_max, file_type);
    return id_len;
}
#endif
/*****************************************************************************/
//  Description : if fm suspend status is setted by itself 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_SetSuspendByFM(BOOLEAN suspend_by_fm)
{
    s_fm_cur_info.suspend_by_fm = suspend_by_fm;                 
    SaveCurrentFMInfo(&s_fm_cur_info);
    return TRUE;
}
/*****************************************************************************/
//  Description : get who set fm suspend status 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_GetSuspendByFM(void)
{                
    return s_fm_cur_info.suspend_by_fm;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Parameter: TRUE:record和fm同时暂停;FALSE:record界面单独暂停
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_SetPausedWithFM(BOOLEAN suspend_with_fm)
{
    s_fm_cur_info.paused_with_fm = suspend_with_fm;                 
    SaveCurrentFMInfo(&s_fm_cur_info);
    return TRUE;
}
/*****************************************************************************/
//  Description : get who set fm suspend status 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_GetPausedWithFM(void)
{                
    return s_fm_cur_info.paused_with_fm;
}
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : if record name be modified 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_CheckRecordNameBeModified(const wchar *input_str)
{
    wchar* file_name = PNULL;
    uint16    file_name_len = MMIFMRECORD_MAX_FILE_NAME_LEN;
    wchar* get_record_name = PNULL;
    uint32 record_name_len = 0;
    int result = 0;
    
    file_name = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
    SCI_MEMSET(file_name, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
    
    get_record_name = MMIFM_GetRecordFilePath();
    record_name_len = MMIAPICOM_Wstrlen(get_record_name);

    MMIAPIFMM_SplitFullPathExt(get_record_name,record_name_len, 
                        PNULL,PNULL,
                        file_name, &file_name_len,
                        PNULL,PNULL); 
    result = MMIAPICOM_Wstrcmp(input_str, file_name);
    SCI_FREE(file_name);
    
    if(0 == result)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }    
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL void MMIFM_SetVolume(MMIFM_AUDIO_DEVICE_MODE_E fm_mode, uint8 volume_level)
{
    MMISRVAUD_ROUTE_T route = 0;

    if(FM_SPEAKER_MODE == fm_mode)
    {
        route = MMISRVAUD_ROUTE_SPEAKER;
    }
    else
    {
        route = MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_SPEAKER;
    }
    MMISRVAUD_SetRoute(MMIFM_GetPlayHandle(), route);

    MMISRVAUD_SetVolume(MMIFM_GetPlayHandle(), (uint32)volume_level);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  return 
//  Note: 
/*****************************************************************************/
LOCAL void MMIFM_SetVolumeForRing(MMIFM_AUDIO_DEVICE_MODE_E fm_mode, uint8 volume_level)
{
    MMISRVAUD_ROUTE_T route = 0;

    if(FM_SPEAKER_MODE == fm_mode)
    {
        route = MMISRVAUD_ROUTE_EARFREE;
    }   
    else
    {
        route = MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_SPEAKER;
    }
    MMISRVAUD_SetRoute(MMIFM_GetPlayHandle(), route);
    MMISRVAUD_SetVolume(MMIFM_GetPlayHandle(), volume_level);
}
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : Callback function to MMI AUDIO
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIFM_RecordCallBack(
                MMISRVAUD_REPORT_RESULT_E  result,
                DPARAM           param
                )
{
    //SCI_TRACE_LOW:"[MMIFM]:MMIFM_RECORDCallBack:result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_2022_112_2_18_2_19_25_55,(uint8*)"d",result);

    MMIDEFAULT_TurnOnBackLight();

    if(MMISRVAUD_REPORT_RESULT_SUCESS == result)
    {
        MMIFM_HandleRecordEnd(TRUE);
    }
    else
    {
        MMIFM_HandleRecordEnd(FALSE);
    }
}
/*****************************************************************************/
//  Description : handle fm record end.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_HandleRecordEnd(BOOLEAN is_succeed)
{
    if(!is_succeed)
    {
        MMK_CloseWin(MMIPUB_ALERT_WIN_ID);
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);            
    }
    else
    if(!MMIAPIFMM_IsDevEnoughSpace(MMIFMRECORD_MIN_NEED_SPACE, MMIFMRECORD_GetRecordStorage()))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);         
        MMK_CloseWin(FM_RECORD_WIN_ID);
    }
    MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_NONE);
}
#endif
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)
/*****************************************************************************/
//  Description : Close FM Record, used for other apps
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_StopRecord(void)
{  
    MMIFMRECORD_STATE_E record_status = MMIFM_GetRecordStatus();
    
    if(MMIFMRECORD_STATE_RECORDING == record_status 
        ||MMIFMRECORD_STATE_RECORD_PAUSED == record_status)    
    {
        MMISRVAUD_Stop(MMIFM_GetRecHandle());
        MMK_CloseWin(FM_RECORD_WIN_ID);
    }
}
#endif
/*****************************************************************************/
// 	Description : auto search is open
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFM_IsOpened(void)
{
    return (MMK_IsOpenWin(FM_RECORD_WIN_ID));
}
/*****************************************************************************/
//  Discription: append item to list control
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIFM_SetStringListItem(
                                      MMI_TEXT_ID_T    text_id,
                                      GUIITEM_STYLE_E  item_style,
                                      MMI_TEXT_ID_T    left_softkey_id,
                                      MMI_TEXT_ID_T    middle_softkey_id,
                                      MMI_TEXT_ID_T    right_softkey_id,
                                      MMI_CTRL_ID_T    ctrl_id,
                                      BOOLEAN          is_update,
                                      uint16           pos
                                      )
{
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data_info = {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data_info;
    
    item_data_info.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data_info.item_content[0].item_data.text_id = text_id;
    
    item_data_info.softkey_id[0] = left_softkey_id;
    item_data_info.softkey_id[1] = middle_softkey_id;
    item_data_info.softkey_id[2] = right_softkey_id;
    if(TRUE == is_update)
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Discription: Get Timer record info
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_GetTimerRecordInfo(MMIFM_TIMER_RECORD_INFO_T *record_info_ptr)
{
    BOOLEAN ret = FALSE;  
    MN_RETURN_RESULT_E nv_result = MN_RETURN_FAILURE;

    if(PNULL != record_info_ptr)
    {
        nv_result = MMI_ReadNVItemEx( MMINV_FM_TIMER_RECORD_INFO, sizeof(MMIFM_TIMER_RECORD_INFO_T) (void*)record_info_ptr );
        if (MN_RETURN_SUCCESS != nv_result)
        {
            record_info_ptr->endure_time = 1;
            record_info_ptr->record_freq = 875;
            MMI_WriteNVItemEx( MMINV_FM_TIMER_RECORD_INFO, sizeof(MMIFM_TIMER_RECORD_INFO_T), (void*)record_info_ptr );
        }
        ret = TRUE;
    }

    return ret;    
}

/*****************************************************************************/
//  Discription: Set Timer record info
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_SetTimerRecordInfo(const MMIFM_TIMER_RECORD_INFO_T *record_info_ptr)
{
    BOOLEAN ret = FALSE;

    if(PNULL != record_info_ptr)
    {
        MMI_WriteNVItemEx( MMINV_FM_TIMER_RECORD_INFO, sizeof(MMIFM_TIMER_RECORD_INFO_T) (void*)record_info_ptr );
        ret = TRUE;
    }

    return ret;
}
/*****************************************************************************/
// 	Description : change  MMI_STRING_T to integer value
//	Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC int16 ConvertStringToInt(const MMI_STRING_T *str_ptr, int32 mul_scale)
{
    int16  ret =0;
    uint16 num_len = 0;
    uint8  num_buf[MMIALM_CHANNEL_NAME_LEN + 1] = {0};
    
    if(str_ptr != PNULL && str_ptr->wstr_ptr != PNULL)
    {
        num_len = MIN(MMIALM_CHANNEL_NAME_LEN, str_ptr->wstr_len);/*lint !e64*/
        MMI_WSTRNTOSTR((char *)num_buf, MMIALM_CHANNEL_NAME_LEN, str_ptr->wstr_ptr, MMIALM_CHANNEL_NAME_LEN, num_len);/*lint !e64*/
        ret = atoi(num_buf);/*lint !e64*/
    }
    //SCI_TRACE_LOW:"[MMIFM] ConvertStringToInt ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_2174_112_2_18_2_19_25_56,(uint8*)"d", ret);
    return ret;
}
/*****************************************************************************/
//  Description : Create FM timer record 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_StartFMTimerRecord(void)
{ 
    MMIFMRECORD_STATE_E record_status = MMIFM_GetRecordStatus();
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};
    
    MMIFM_GetTimerRecordInfo(&record_info);
    
    if(MMIFM_CheckHeadsetStatus())
    {
        if(MMIFMRECORD_STATE_NONE == record_status)
        {
        	if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_UDISK_DISABEL_FM_RECORD);
            }
	        if(MMIAPISET_GetFlyMode())
            {
                MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST);
                return;
            }
#ifdef ATV_SUPPORT
	    	else if(MMIAPIATV_IsATVOpen())			
            {
       	      //ATV is using, FM can't be used ;Bug:86534
        	  MMI_WIN_ID_T        query_win_id = MMIFM_QUERY_WIN_ID;
        	  MMIPUB_OpenQueryWinByTextId(TXT_FM_PROMPT_CLOSE_ATV,IMAGE_PUBWIN_QUERY,&query_win_id,HandleFmRecordCloseAtvQueryWin);
            }
#endif
            else
            {
                MMIFM_StopAutoSearch();
    	        MMIFM_PlayFreq(record_info.record_freq);
                if(FALSE == MMIAPIFM_IsFmActive())
                {
                    MMIFM_SetStatus(FM_PLAYING); 
                }
    	        StartFMTimerRecord();
            }
        }
    }
    else
    {
        MMIALMSVC_SetAlmService();
        if (STARTUP_ALARM == MMIAPIPHONE_GetStartUpCondition())
        {
            MMIDEFAULT_TurnOffBackLight();
            POWER_PowerOff();
        }
    }
}
#endif
/*****************************************************************************/
//  Description : reset timer fm info
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_Reset(void)
{
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
        MMIFM_TIMER_RECORD_INFO_T timer_record_info = {0};
        timer_record_info.endure_time = 1;
        timer_record_info.record_freq =875;
        MMI_WriteNVItemEx( MMINV_FM_TIMER_RECORD_INFO, sizeof(MMIFM_TIMER_RECORD_INFO_T), (void*)(&timer_record_info) );
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
        MMIFMRECORD_SetRecordStorage(MMI_DEVICE_UDISK);
#endif
}
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
// 	Description : check space for record.
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_IsEnoughSpaceToRecord(void)
{
    MMIFILE_DEVICE_E        file_dev = MMI_DEVICE_SDCARD;
    MMIFMRECORD_RESULT_E  error_code = MMIFMRECORD_RESULT_SUCCESS;
    uint32              free_space_low = 0;
    uint32              free_space_high = 0;
    BOOLEAN             result = TRUE;
    
    file_dev = MMIFMRECORD_GetRecordStorage();
    if(MMI_DEVICE_UDISK == file_dev)
    {
        if (MMIAPIFMM_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK) , MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK) , 
            &free_space_high, &free_space_low))
        {
            uint32          used_space_low = 0;
            uint32          used_space_high = 0;
            uint32          total_space_high = 0;
            uint32          total_space_low = 0;
            MMIAPIFMM_GetDeviceUsedSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK) , MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK) ,&used_space_high, &used_space_low);
            total_space_low  =  used_space_low + free_space_low;  
            if ((total_space_low < used_space_low) || (total_space_low < free_space_low))
            {
                total_space_high =  used_space_high + free_space_high +1;
            }
            else
            {
                total_space_high =  used_space_high + free_space_high;
            }
            
            if ((0 < total_space_high) || (0 == total_space_high && 10*MMIFMRECORD_UDISK_MIN_NEED_SPACE<= total_space_low))
            {
                if (free_space_high == 0 && free_space_low < MMIFMRECORD_UDISK_MIN_NEED_SPACE)
                {
                    error_code = MMIFMRECORD_RESULT_NO_SPACE;
                }
            }
            else
            {
                if (free_space_high == 0 && free_space_low < total_space_low/10)
                {
                    error_code = MMIFMRECORD_RESULT_NO_SPACE;
                }
            }            
        }
        else
        {
            error_code = MMIFMRECORD_RESULT_NO_SPACE;
        }
    }
    else
    {
        if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, file_dev, MMIFMRECORD_MIN_NEED_SPACE,PNULL))
        {
            error_code = MMIFMRECORD_RESULT_NO_SPACE;
        }
    }

    if(MMIFMRECORD_RESULT_NO_SPACE == error_code)
    {
        result = FALSE;
    }

    return result;
}
#endif
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
/*****************************************************************************/
//  Description : Get Next Valid Channel By Freq
//  Global resource dependence : 
//  Author:
//  Parameter: step[IN]: adjust steps, 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIFM_GetNextValidChannelByFreq(uint16 freq,int32 step,uint16 *ch_index)
{
    int             freq_sum = 0;
    BOOLEAN         result = FALSE;
    uint16          i = 0;
    MMIFM_CHANNEL_T channel = {0};
    uint16 channel_index[MMIFM_MAX_CHANNEL_NUM] = {0};   
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_GetNextValidChannelByFreq freq %d,step %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_APP_2303_112_2_18_2_19_25_57,(uint8*)"dd", freq,step);
    
    freq_sum = MMIFM_GetValidChannelCount();
    
    GetSortedChannelIndex(channel_index);
    
    if(step > 0)
    {
        for(i = 1; i <= freq_sum; i++)
        {
            if(MMIFM_GetOneChannel(channel_index[i-1], &channel))
            {
                if(channel.freqency > s_fm_cur_info.cur_freq)
                {
                    *ch_index = channel_index[i-1];
                    result = TRUE;
                    break;
                }
            }
            if(i==freq_sum)
            {
                *ch_index = channel_index[0];
                result = TRUE;
                break;
            }                
        }
    }
    else
    {
        for(i = freq_sum; i > 0; i--)
        {
            if(MMIFM_GetOneChannel(channel_index[i-1], &channel))
            {
                if(channel.freqency < s_fm_cur_info.cur_freq)
                {
                    *ch_index = channel_index[i-1];
                    result = TRUE;
                    break;
                }
            }
            if(1==i)
            {
                *ch_index = channel_index[freq_sum-1];
                result = TRUE;
                break;
            }      
        }
    }    
    return result;
}
#else
/*****************************************************************************/
//  Description : Get Next Valid Channel By Index
//  Global resource dependence : 
//  Author:langhua.yang
//  Parameter: step[IN]: adjust steps 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIFM_GetNextValidChannelByIndex(uint16 freq,int32 step,uint16 *ch_index)
{

    BOOLEAN         result = FALSE;
    uint16          i = 0;  
    /* modify bylanghua.yang ,应为按照index跳台*/
    *ch_index = s_fm_cur_info.cur_channel_index;
    if(step > 0)
    {  
        /*用户可能删除掉某一电台，因此不能用freq_sum做循环条件*/
        for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
        {
            /*若当前已是最后一个电台，则跳到第一个电台*/
            if(*ch_index >= (MMIFM_MAX_CHANNEL_NUM - 1))
            {
               *ch_index = 0;
            }
            else
            {
               /*短按右键，则跳到下一电台*/
               *ch_index += 1;
            }
            /*判断电台是否存在，若存在，则跳出循环，否则继续寻找下一个有效电台*/
            if(MMIFM_IsChannelExist(*ch_index))
            {
                result = TRUE;
                break;
            }
        }
    }
    else
    {   
        /*用户可能删除掉某一电台，因此不能用freq_sum做循环条件*/
        for(i = MMIFM_MAX_CHANNEL_NUM; i > 0; i--)
        {

           /*若当前已是第一个电台，则跳到最后一个电台*/
            if(*ch_index < 1)
            {
                *ch_index = MMIFM_MAX_CHANNEL_NUM - 1;
            }
            else
            {
                /*短按左键，则跳到上一个电台*/
                *ch_index -= 1;
            }
            /*判断电台是否存在，若存在，则跳出循环，否则继续寻找上一个有效电台*/
            if(MMIFM_IsChannelExist(*ch_index))
            {
                result = TRUE;
                break;
            }
        }
    }  
    return result;
}
#endif
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHeadsetAlertWin(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E 	msg_id,
    DPARAM              param
    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_CLOSE_WINDOW:
            MMIAPIALM_CheckandStartExpiredEvent();
            result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
        default:
            result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : Check Headset Status
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_CheckHeadsetStatus(void)
{    
    BOOLEAN result = TRUE;
#if !defined(WIN32) && defined(MMI_FM_NEED_HEADSET)
    if(!GPIO_CheckHeadsetStatus())
    {
        //not headset, can not play
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_FM_INPUT_HEADSET,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleHeadsetAlertWin);
        result = FALSE;
    }
 #endif
    return result;
}
/*****************************************************************************/
//  Description : Check Headset Status
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIFM_CheckHeadsetStatusWithoutPrompt(void)
{    
    BOOLEAN result = TRUE;
#if !defined(WIN32) && defined(MMI_FM_NEED_HEADSET)
    if(!GPIO_CheckHeadsetStatus())
    {       
        result = FALSE;
    }
#endif
    return result;
}
/*****************************************************************************/
//  Description : get sorted channel index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetSortedChannelIndex(uint16 channel_list[])
{
    MMIFM_CHANNEL_T channel = {0};   /*lint !e64*/
    int i = 0;
    int j = 0;
    MMIFM_FREQ_LIST_INFO_T temp_value = {0};/*lint !e64*/
    MMIFM_FREQ_LIST_INFO_T freq_list[MMIFM_MAX_CHANNEL_NUM] = {0};/*lint !e64*/
    uint16 valid_freq_num = MMIFM_GetValidChannelCount();

    SCI_MEMSET(channel_list,0,sizeof(uint16)*valid_freq_num);
    
    if(valid_freq_num > 0)
    {
        for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
        {
            SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T));
            if(MMIFM_GetOneChannel(i, &channel) && MMIFM_IsChannelValid(&channel))
            {
                freq_list[j].channel_index = i;
                freq_list[j].freq = channel.freqency;
                j++;
            }
        }
        
        for(j=0;j<valid_freq_num;j++)
        {
            for(i=0;i<valid_freq_num-j;i++)
            {
                if((freq_list[i].freq>freq_list[i+1].freq)&&MMIFM_IsValidFre(freq_list[i+1].freq))
                {
                    temp_value = freq_list[i];
                    freq_list[i] = freq_list[i+1];
                    freq_list[i+1] = temp_value;
                }
            }
        }    
        
        for(i = 0; i < valid_freq_num; i++)
        {
            channel_list[i] = freq_list[i].channel_index;
        }
    }
}
/*****************************************************************************/
//  Description : 根据mode值获取audio route
//  Global resource dependence : 
//  Author:
//  Parameter: 无
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T GetAudioSupportRoute(void)
{
    MMISRVAUD_ROUTE_T support_route = MMISRVAUD_ROUTE_SPEAKER|MMISRVAUD_ROUTE_EARPHONE;

    if (FM_SPEAKER_MODE == s_fm_cur_info.sound_mode)
    {
        support_route = MMISRVAUD_ROUTE_SPEAKER;
    }
    SCI_TRACE_LOW("MMIFM_GetAudioSupportRoute MMISRVAUD_ROUTE_T %d",support_route);
    
    return support_route;
}
#if defined (MMI_FM_TIMER_RECORD_SUPPORT) && defined(ATV_SUPPORT)
/*****************************************************************************/
//  Description :Handle FmRecord Close Atv Query Win MSG
//  Global resource dependence : none
//  Author: langhua.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmRecordCloseAtvQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};
    MMI_WIN_ID_T        query_win_id = MMIFM_QUERY_WIN_ID;
    MMIFM_GetTimerRecordInfo(&record_info);
    switch (msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
       MMIPUB_CloseQuerytWin(&query_win_id);
	   MMIAPIATV_Exit();
	   MMIFM_StopAutoSearch();
       MMIFM_PlayFreq(record_info.record_freq);
       if(FALSE == MMIAPIFM_IsFmActive())
        {
          MMIFM_SetStatus(FM_PLAYING); 
        }
       StartFMTimerRecord();
       break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
#endif
#endif
