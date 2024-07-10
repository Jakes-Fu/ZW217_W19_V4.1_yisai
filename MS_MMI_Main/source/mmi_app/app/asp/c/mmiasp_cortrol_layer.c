/****************************************************************************
** File Name:      mmiasp_cortrol_layer.c                                   *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the ANSWER PHONE           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_asp_trc.h"
#include "mmiasp_cortrol_layer.h"
#include "mmiasp_wintab.h"
#include "mmicc_export.h"
//#include "mmicc_image.h"


#include "mmisrvaud_api.h"
#include "mmiasp_nv.h"
#include "mmiasp_export.h"
//#include "wav_adp.h"
//#include "aud_gen.h"
#include "audio_api.h"
#include "audio_config.h"
#include "mmicc_audio.h"
//#include "mmiset_set_ring.h"
#include "mmifmm_export.h"
#include "mmimms_export.h"
#include "mmi_modu_main.h"
//#include "mmiaudio.h"
#include "mmisd_export.h"
#include "mmi_resource.h" 
#include "mmiudisk_export.h"
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIASP_SYSFILE_DIR_LEN     3 
#define MMIASP_SYSFILE_DIR         (s_asp_sysfile_dir_wstr)
#define MMIASP_SYSFILE_NAME_LEN    7 
#define MMIASP_SYSFILE_NAME        (s_asp_sysfile_name_wstr)
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 
/* answer machine version number 1.0   */
LOCAL const wchar s_answer_machine_version[]  = {'A','_','m','v','1','.','0','0',0};

LOCAL const uint32  s_answer_wait_time[ASP_WAIT_TIME_TYPE_NUM] = {1 , 5, 10, 15, 20}; 

// 答录机系统文件路径
LOCAL const wchar s_asp_sysfile_dir_wstr[] = L"ASP";
// 答录机系统文件name
LOCAL const wchar s_asp_sysfile_name_wstr[] = L"asp.sys";
// 答录机是否在工作中的标志变量....
LOCAL BOOLEAN    s_asp_mechine_is_working = FALSE;
// 最新的来电答录节点
LOCAL MMIASP_MSG_INFO_T s_new_msg_node = {0};
// 指向头指针....
LOCAL MMIASP_MSG_INFO_T* s_msg_list_header_ptr = PNULL;

LOCAL MMISRV_HANDLE_T s_asp_record_handle = 0;
LOCAL MMISRV_HANDLE_T s_asp_play_handle = 0;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
                               
/*****************************************************************************/
//  Description : call back fun
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void PlayCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//  Description : notify handle function for play before record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPPlayRemoteCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : notify handle function for play before record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPPlayCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : notify handle function for record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPRecordCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/***********************************************************************************
* Function Name: MMIASP_WriteSysFile answer machine sys file
* Purpose:       write system file
* Author:byte.guo
// Note: 
|---n----|----4---|----4-------|------n------|-----n-------|----------n-------|
|sys_head|list_num|new_item_num|file_name_len|file_name_str|cur_node_structure|-more--
|-----------------------------------------------------------------------------|       
***********************************************************************************/
LOCAL BOOLEAN ReadASPSysFile(void);
/**********************************************************************************/
//  Description : searcj free number
//  Global resource dependence :none
//  Author:
//  Note:  
/**********************************************************************************/
LOCAL BOOLEAN SearchFreeNum(
                            MMIASP_MSG_INFO_T* cur_ptr,         //[in]
                            const  uint8*      call_number_ptr, //[in]
                            uint32              suffix_id
                            );
/**********************************************************************************/
//  Description :  id
//  Global resource dependence :none
//  Author:
//  Note:  
/**********************************************************************************/
LOCAL uint8 GetSuffixIdByTelNum(
                                MMIASP_MSG_INFO_T *list_head_ptr,
                                const uint8*      call_number_ptr
                                );
/*****************************************************************************/
//  Description : start record 
//  Global resource dependence : none
//  Author: byte.guo
//  Note: jun.hu 把此函数MMIASP_StartRecord改成Local类型的...
/*****************************************************************************/
LOCAL BOOLEAN StartRecord(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetCustomerAnswerRing(const uint8* call_number_ptr,
                                    BOOLEAN  *is_fixed_id_ptr,
                                    wchar   *path_ptr,
                                    uint32  *path_len_ptr,
                                    uint32  *fixed_id_ptr
                                    );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDefaultFixedAnswerRing(uint16* fixid_ptr);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 以分钟为单位
/*****************************************************************************/
LOCAL uint32 GetRecordTime(void);
/*****************************************************************************/
//  Description : stop record 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopRecord(void);
/*****************************************************************************/
//  Description : creat record timer
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateRecordTimer(void);
/*****************************************************************************/
//  Description : stop play audio for client
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void StopPlayAudio(void);
/*****************************************************************************/
//  Description : 获取存放系统文件的路径以供写
//  Global resource dependence :  none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSysFileFullNameForWrite(
                                         wchar*       full_path_name_ptr,  // out  
                                         uint16*      full_path_len_ptr    // out      
                                         );
/*****************************************************************************/
//  Description : 获取存放系统文件的路径以供读
//  Global resource dependence :  none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSysFileFullNameForRead(
                                         wchar*       full_path_name_ptr,  // out    
                                         uint16*      full_path_len_ptr    // out    
                                         );
/*****************************************************************************/
//  Description :  设置答录机是否在工作状态.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 此函数不开放给外部...
/*****************************************************************************/
LOCAL void SetMechineWorkingFlag(BOOLEAN is_working);
/*****************************************************************************/
//  Description : 所有盘符是否有足够的剩余空间来录制留言....
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetAllPathEnoughFreeSpace(void);
/*****************************************************************************/
//  Description : 当前盘符是否有足够的剩余空间来录制留言....
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetCuurPathEnoughFreeSpace(void);

/*****************************************************************************/
//  Description : Callback function 
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void Record_CallBack(
                           MMISRVAUD_REPORT_RESULT_E  result,
                           DPARAM           param
                           );

/*****************************************************************************/
// Description : Combine file path
// Global resource dependence : none
// Author: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN CombineASPFilePath(
                                 MMIASP_APPLET_T*     applet_ptr,
                                 MMIASP_MSG_INFO_T*   msg_info, 
                                 const  MMI_STRING_T* name_str,
                                 const  uint8*        call_number_str
                                 );
/*****************************************************************************/
//  Description :  add node to head
//  Global resource dependence :none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
LOCAL void AddMsgToListHeader(
                              MMIASP_MSG_INFO_T **list_head_pptr, 
                              MMIASP_MSG_INFO_T *node_ptr
                              );
/*****************************************************************************/
//  Description : ....
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 该函数不仅在U盘或SD卡检查它们的剩余空间是否足够，而且检查其是否存在MMIMULTIM_DIR_MUSIC
//        如不存在，则创建之，不成功则再去搜索第二个盘....
/*****************************************************************************/
LOCAL BOOLEAN GetAudioFolder(MMIFILE_DEVICE_E* dev_ptr/*out*/);
/*****************************************************************************/
//  Description : init nv
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_NvInit(MMIASP_APPLET_T* applet_ptr)
{
    MN_RETURN_RESULT_E  nv_result = MN_RETURN_FAILURE;
    
    if(PNULL != applet_ptr)
    {
        MMINV_READ(MMINV_ASP_SETTING, &(applet_ptr->nv_info), nv_result);
        
        /* read default value when read fail form nv */
        if (MN_RETURN_SUCCESS != nv_result)
        {
            SCI_MEMSET(&(applet_ptr->nv_info), 0, sizeof(applet_ptr->nv_info));
            //applet_ptr->nv_info = MMIAPIASP_ResetFactorySetting();
            MMIASP_ResetSetting(&(applet_ptr->nv_info));
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP MMIASP_NvInit NULL applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_245_112_2_18_2_2_36_17,(uint8*)"");
    }
    
}
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the status of asp
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIASP_PLAY_STATE_E MMIASP_GetPlayStatus(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    if(PNULL != applet_ptr)
    {
        //SCI_TRACE_LOW:"[MMIASP]: GetPlayStaus: 0x%.2x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_260_112_2_18_2_2_36_18,(uint8*)"d", applet_ptr->play_state);
        return applet_ptr->play_state;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: GetPlayStaus: no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_265_112_2_18_2_2_36_19,(uint8*)"");
        return MMIASP_STATE_NONE;
    }
    
}

/*****************************************************************************/
//  Description : Set the status of mpeg4
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_SetPlayStatus(MMIASP_PLAY_STATE_E cur_status)
{           
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    
    if(PNULL != applet_ptr)
    {
        //SCI_TRACE_LOW:"[MMIASP]: SetPlayStatus: 0x%.2x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_281_112_2_18_2_2_36_20,(uint8*)"d", applet_ptr->play_state);
        applet_ptr->play_state = cur_status;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: SetPlayStatus: no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_286_112_2_18_2_2_36_21,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : call back fun(向asp主播放窗口发送"播放结束"消息)
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void PlayCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"[MMIASP] PlayCallBack enter ."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_296_112_2_18_2_2_36_22,(uint8*)"");
    //if(MMISRVAUD_REPORT_RESULT_STOP != result)
    {
        if(MMISRVAUD_REPORT_RESULT_SUCESS == result && MMK_IsFocusWin(MMIASP_MAIN_PLAY_WIN_ID))
        {
            DisplayASPWinProgress(TRUE);
            DisplayASPWinProgressTime(TRUE);
        }
        MMIAPICOM_OtherTaskToMMI(MMIASP_MAIN_PLAY_WIN_ID, MSG_ASP_PLAY_CALL_BACK, &result, sizeof(MMISRVAUD_REPORT_RESULT_E));
    }
}

/*****************************************************************************/
//  Description : pause play
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_Pause(void)
{
    MMISRV_HANDLE_T audio_handle = MMIASP_GetPlayHandle();

    //SCI_TRACE_LOW:"[MMIASP] Pause enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_316_112_2_18_2_2_36_23,(uint8*)"");
    if(audio_handle > 0)
    {
        MMISRVAUD_Pause(audio_handle);
    }

    MMIASP_SetPlayStatus(MMIASP_STATE_PAUSE);
}

/*****************************************************************************/
//  Description : resume play
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_Resume(void)
{
    MMISRV_HANDLE_T audio_handle = MMIASP_GetPlayHandle();

    //SCI_TRACE_LOW:"[MMIASP] Resume enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_333_112_2_18_2_2_36_24,(uint8*)"");
    if(audio_handle > 0)
    {
        MMISRVAUD_Resume(audio_handle);
    }

    MMIASP_SetPlayStatus(MMIASP_STATE_PLAY);
}

/*****************************************************************************/
//  Description : stop play
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_Stop(void)
{
    MMISRV_HANDLE_T audio_handle = MMIASP_GetPlayHandle();

    //SCI_TRACE_LOW:"[MMIASP] Stop enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_350_112_2_18_2_2_36_25,(uint8*)"");
    MMIASP_StopProgressTimer();
    
    if(audio_handle > 0)
    {
        MMISRVAUD_Stop(audio_handle);
    }
    MMIASP_SetPlayStatus(MMIASP_STATE_STOP);
}

/*****************************************************************************/
//  Description : play current record
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_Play(const wchar* file_name_ptr)
{
    BOOLEAN result      = TRUE;
    //uint8   ring_vol    = MMIAPISET_GetMultimVolume();
    MMISRVAUD_TYPE_U     audio_data = {0};
    MMISRV_HANDLE_T      audio_handle = MMIASP_GetPlayHandle();
    
    //SCI_TRACE_LOW:"[MMIASP]: Play enter "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_371_112_2_18_2_2_36_26,(uint8*)"");

    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.fmt  = MMISRVAUD_RING_FMT_WAVE;
    audio_data.ring_file.name = file_name_ptr; /*lint !e605*/
    audio_data.ring_file.name_len = MMIAPICOM_Wstrlen(file_name_ptr);

    //当前是否有可用的handle，有的话直接切换，否则重新申请
    if(0 < audio_handle)
    {      
        MMISRVAUD_SetAudioInfo(audio_handle, &audio_data);
    }
    else
    { 
        if(MMIASP_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, ASPPlayCallback))
        {  
            //重新申请handle的话，需要重新设置音量值
            //MMISRVAUD_SetVolume(audio_handle, ring_vol);
            MMIASP_SetPlayHandle(audio_handle);
        }
        else//申请handle不成功
        {
            //SCI_TRACE_LOW:"[MMIASP]: Play creat handle fail "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_393_112_2_18_2_2_36_27,(uint8*)"");
            PlayCallBack(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
            result = FALSE;
        }
    }

    //开始播放,(result != false)时代表handle存在或者申请成功，才能进一步play
    if((!MMIASP_AudioPlay(audio_handle, 0, 1)) && (result))
    {
        //播放有误的操作
        //SCI_TRACE_LOW:"[MMIASP]: Play audio fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_403_112_2_18_2_2_36_28,(uint8*)"");
        PlayCallBack(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
        MMIASP_SetPlayHandle(0);
        result = FALSE;
    }
          
    if (result)
    {
        MMIASP_SetPlayStatus(MMIASP_STATE_PLAY);
        CreateProgressTimer();
    }
    
    //SCI_TRACE_LOW:"[MMIASP] MMIASP_Play() result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_417_112_2_18_2_2_36_29,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};

    if(PNULL == audio_handle || PNULL == audio_data)
    {
        return FALSE;
    }
    
    req.notify = notify;
    req.pri = MMISRVAUD_PRI_HIGH;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = TRUE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = route;
    audio_srv.volume = MMIAPISET_GetMultimVolume();

    switch(audio_data->type)
    {
    case MMISRVAUD_TYPE_ASP_FILE:
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
        break;
    case MMISRVAUD_TYPE_RING_FILE:
        req.pri = MMISRVAUD_PRI_NORMAL;
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
        break;

    case MMISRVAUD_TYPE_ASP_BUF: 
        audio_srv.info.ring_buf.type = audio_data->type;
        audio_srv.info.ring_buf.fmt  = audio_data->ring_buf.fmt;
        audio_srv.info.ring_buf.data = audio_data->ring_buf.data;
        audio_srv.info.ring_buf.data_len = audio_data->ring_buf.data_len;
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
        break;
    case MMISRVAUD_TYPE_RING_BUF: 
        req.pri = MMISRVAUD_PRI_NORMAL;
        audio_srv.info.ring_buf.type = audio_data->type;
        audio_srv.info.ring_buf.fmt  = audio_data->ring_buf.fmt;
        audio_srv.info.ring_buf.data = audio_data->ring_buf.data;
        audio_srv.info.ring_buf.data_len = audio_data->ring_buf.data_len;       
        audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
        break;
        
    case MMISRVAUD_TYPE_RECORD_FILE:
        audio_srv.info.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;        
        audio_srv.info.record_file.fmt  = audio_data->record_file.fmt;
        audio_srv.info.record_file.name = audio_data->record_file.name;
        audio_srv.info.record_file.name_len = audio_data->record_file.name_len;    
        audio_srv.info.record_file.source   = audio_data->record_file.source;
        audio_srv.info.record_file.frame_len = audio_data->record_file.frame_len;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;

    case MMISRVAUD_TYPE_RECORD_BUF:
        audio_srv.info.record_buf.type = MMISRVAUD_TYPE_RECORD_BUF;
        audio_srv.info.record_buf.fmt  = audio_data->record_buf.fmt;
        audio_srv.info.record_buf.data = audio_data->record_buf.data;
        audio_srv.info.record_buf.data_len = audio_data->record_buf.data_len;
        audio_srv.info.record_buf.source   = audio_data->record_buf.source;
        audio_srv.info.record_buf.frame_len = audio_data->record_buf.frame_len;
        
        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;
        
    default:
        break;
    }

    *audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(*audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : get current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_GetPlayHandle(void)
{
    return s_asp_play_handle;
}

/*****************************************************************************/
//  Description : get current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_GetRecordHandle(void)
{
    return s_asp_record_handle;
}

/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_SetPlayHandle(MMISRV_HANDLE_T handle)
{
    return s_asp_play_handle = handle;
}

/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_SetRecordHandle(MMISRV_HANDLE_T handle)
{
    return s_asp_record_handle = handle;
}

/*****************************************************************************/
//  Description : notify handle function for play after record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPPlayCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_565_112_2_18_2_2_37_30,(uint8*)"dd", handle, param));
    
    if(param != PNULL && handle > 0)
    {
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            break;
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(PNULL != report_ptr)
            {
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_END:
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    MMIASP_SetPlayHandle(0);
                    PlayCallBack(report_ptr->data1, PNULL); 
                    break;
                    
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : notify handle function for play before record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPPlayRemoteCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MmiAudioCallbackMsgS *sendSignal = PNULL;
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_613_112_2_18_2_2_37_31,(uint8*)"dd", handle, param));
    
    if(PNULL != param && PNULL!=applet_ptr && handle == applet_ptr->bkmusic_audio_handle)
    {
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            break;
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(PNULL != report_ptr)
            {
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_END:
                    MmiCreateSignal(APP_ASP_PLAY_AUDIO_END_IND, sizeof(MmiAudioCallbackMsgS), (MmiSignalS**)&sendSignal);
                    sendSignal->Sender = P_APP;
                    sendSignal->hAudio = handle;
                    //sendSignal->sig.audio_info.notify_info = notify_info;
                    //sendSignal->sig.audio_info.affix_info  = affix_info;
                    MmiSendSignal(P_APP, (MmiSignalS*)sendSignal); 
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    applet_ptr->bkmusic_audio_handle = 0;
                    break;
                    
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : notify handle function for record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ASPRecordCallback(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_663_112_2_18_2_2_37_32,(uint8*)"dd", handle, param));
    
    if(param != PNULL && handle == MMIASP_GetRecordHandle())
    {
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            break;
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(PNULL != report_ptr)
            {
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_END:
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    MMIASP_SetRecordHandle(0);
                    Record_CallBack(report_ptr->data1, PNULL);
                    break;
                    
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : play record after setting play times
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_AudioPlay(
                            MMISRV_HANDLE_T  audio_handle,
                            uint32           uiOffset,
                            uint32           play_times
                            )
{
    BOOLEAN res = FALSE;
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMIASP_AudioPlay() entry, handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_714_112_2_18_2_2_37_33,(uint8*)"d", audio_handle);

    if(audio_handle > 0)
    {
        MMISRVAUD_SetPlayTimes(audio_handle, play_times);
        res = MMISRVAUD_Play(audio_handle, uiOffset);       
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIASP_AudioPlay() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_722_112_2_18_2_2_37_34,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_StopRecordTimer(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    if( PNULL != applet_ptr)
    {
        if(MMI_NONE_TIMER_ID != applet_ptr->record_timer_id)
        {
            //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopRecordTimer record_timer_id     = %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_738_112_2_18_2_2_37_35,(uint8*)"d",applet_ptr->record_timer_id);
            MMK_StopTimer(applet_ptr->record_timer_id);
            applet_ptr->record_timer_id = MMI_NONE_TIMER_ID;
        }
        // 答录计时的timer.....
        if(MMI_NONE_TIMER_ID != applet_ptr->count_time_id)
        {
            //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopRecordTimer record_timer_id     = %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_745_112_2_18_2_2_37_36,(uint8*)"d",applet_ptr->count_time_id);
            MMK_StopTimer(applet_ptr->count_time_id);
            applet_ptr->count_time_id = MMI_NONE_TIMER_ID;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopRecordTimer: no applet!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_752_112_2_18_2_2_37_37,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : play prev file
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_Prev(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMIASP_MSG_INFO_T*  cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr,applet_ptr->cur_gui_list_index);
    uint16              msg_total_num = 0;
    
    //SCI_TRACE_LOW:"[MMIASP] MMIASP_Prev() enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_766_112_2_18_2_2_37_38,(uint8*)"");
    msg_total_num = MMIASP_GetListMsgTotalNum(applet_ptr);
    
    if(PNULL != cur_node_ptr && 0 < msg_total_num)
    {
        if (0 < applet_ptr->cur_gui_list_index)
        {
            applet_ptr->cur_gui_list_index--;
            // 更新一下node
            cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr,applet_ptr->cur_gui_list_index);
            if(PNULL != cur_node_ptr)
            {
                MMIASP_Stop();
                if(!cur_node_ptr->read_flag)
                {
                    cur_node_ptr->read_flag = TRUE;
                    MMIASP_WriteSysFile();
                }
                if(MMIASP_Play((const wchar*)cur_node_ptr->record_path))
                {                    
                    MMIASP_GetTimeInfo(applet_ptr);
                    return TRUE;
                }
            }
        }  
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : play next file
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_Next(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMIASP_MSG_INFO_T*  cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr,applet_ptr->cur_gui_list_index);
    uint16              msg_total_num = 0;
    
    //SCI_TRACE_LOW:"[MMIASP] MMIASP_Next() enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_803_112_2_18_2_2_37_39,(uint8*)"");
    
    msg_total_num = MMIASP_GetListMsgTotalNum(applet_ptr);
    
    if(PNULL != cur_node_ptr && 0 < msg_total_num)
    {
        if( (msg_total_num - 1)  > applet_ptr->cur_gui_list_index )
        {
            applet_ptr->cur_gui_list_index++;
            // 更新一下node
            cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr,applet_ptr->cur_gui_list_index);
            if(PNULL != cur_node_ptr)
            {
                if(!cur_node_ptr->read_flag)
                {
                    cur_node_ptr->read_flag = TRUE;
                    MMIASP_WriteSysFile();
                }
                MMIASP_Stop();
                if(MMIASP_Play((const wchar*)cur_node_ptr->record_path))
                {                    
                    MMIASP_GetTimeInfo(applet_ptr);
                    return TRUE;
                }
            }
        } 
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description :  allocate memory of MMIASP_MSG_INFO_T
//  Global resource dependence :none
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC MMIASP_MSG_INFO_T *MMIASP_MsgInfoMalloc(void)
{
    MMIASP_MSG_INFO_T *msg_ptr = (MMIASP_MSG_INFO_T *)SCI_ALLOCA(sizeof(MMIASP_MSG_INFO_T));   
    if(PNULL != msg_ptr)
    {
        SCI_MEMSET( msg_ptr,0,sizeof(MMIASP_MSG_INFO_T) );  
    }
    
    return msg_ptr;        
}
/*****************************************************************************/
//  Description :  free memory of MMIASP_MSG_INFO_T
//  Global resource dependence :none
//  Author:
//  Note: Call the fun when record finished
/*****************************************************************************/
PUBLIC void MMIASP_MsgListFree(MMIASP_APPLET_T* applet_ptr)
{
    MMIASP_MSG_INFO_T *msg_cur_ptr = PNULL;
    MMIASP_MSG_INFO_T *msg_next_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_MsgListFree enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_854_112_2_18_2_2_37_40,(uint8*)"");

    if(PNULL != applet_ptr )
    {
        msg_next_ptr = applet_ptr->list_head_ptr;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP MMIASP_MsgListFree no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_862_112_2_18_2_2_37_41,(uint8*)"");
    }
    
    
    while(PNULL != msg_next_ptr)
    {             
        msg_cur_ptr = msg_next_ptr;
        msg_next_ptr = msg_next_ptr->next_ptr;
        /* Free record_path */
        if (PNULL != msg_cur_ptr->record_path)
        {
            //SCI_TRACE_LOW:"[MMIASP]: MMIASP_MsgListFree record_path !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_873_112_2_18_2_2_37_42,(uint8*)"");
            SCI_FREE(msg_cur_ptr->record_path);
            msg_cur_ptr->record_path = PNULL;
        }
        
        SCI_FREE(msg_cur_ptr);
        msg_cur_ptr = PNULL;
    }

    if(PNULL != applet_ptr)
    {
        MMIASP_SetListHead( &applet_ptr->list_head_ptr, PNULL );
    }
}

/*****************************************************************************/
//  Description :  free memory of MMIASP_MSG_INFO_T
//  Global resource dependence :none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_MsgInfoListFree(MMIASP_MSG_INFO_T* msg_list_header_ptr)
{
    MMIASP_MSG_INFO_T *msg_cur_ptr = PNULL;
    MMIASP_MSG_INFO_T *msg_next_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_MsgListFree enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_897_112_2_18_2_2_38_43,(uint8*)"");

    if(PNULL != msg_list_header_ptr )
    {
        msg_next_ptr =msg_list_header_ptr;
    }
    else
    {
       return;
    }
    
    
    while(PNULL != msg_next_ptr)
    {             
        msg_cur_ptr = msg_next_ptr;
        msg_next_ptr = msg_next_ptr->next_ptr;
        if (PNULL != msg_cur_ptr->record_path)
        {
            SCI_FREE(msg_cur_ptr->record_path);
            msg_cur_ptr->record_path = PNULL;
        }
        
        SCI_FREE(msg_cur_ptr);
        msg_cur_ptr = PNULL;
    }
}

/*****************************************************************************/
//   Description : set list head
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC void MMIASP_SetListHead( 
                               MMIASP_MSG_INFO_T** msg_list_pptr,
                               const MMIASP_MSG_INFO_T*  head_node_ptr
                               )
{
    if(PNULL != msg_list_pptr)
    {
        *msg_list_pptr = head_node_ptr;  /*lint !e605*/
    }
}
/*****************************************************************************/
//  Description :  add node to head
//  Global resource dependence :none
//  Author:byte.guo
//  Note:
/*****************************************************************************/
PUBLIC MMIASP_MSG_INFO_T *MMIASP_AddMsg2ListEnd(
                                                MMIASP_MSG_INFO_T **list_head_pptr, 
                                                MMIASP_MSG_INFO_T *node_ptr
                                                )
{
    MMIASP_MSG_INFO_T *next_ptr =PNULL;
    
    if(PNULL == list_head_pptr || PNULL == node_ptr)
    {
        return PNULL;
    }
    
    //SCI_TRACE_LOW:" [MMIASP]:MMIASP_AddMsg2ListEnd enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_954_112_2_18_2_2_38_44,(uint8*)"");
    
    if(PNULL == *list_head_pptr)
    {
        *list_head_pptr = node_ptr;   
    }
    else    
    {
        next_ptr = *list_head_pptr;   
        /* If the next ptr is NULL,add the node to the ptr,and loop is over*/
        while(  (PNULL != next_ptr->next_ptr)  )
        {            
            next_ptr = next_ptr->next_ptr;
        }
        
        /* Add this new node to end*/
        next_ptr->next_ptr = node_ptr;   
    }
    node_ptr->next_ptr = PNULL;   
    
    return next_ptr;
}
/*****************************************************************************/
//  Description :  add node to head
//  Global resource dependence :none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
LOCAL void AddMsgToListHeader(
                              MMIASP_MSG_INFO_T **list_head_pptr, 
                              MMIASP_MSG_INFO_T *node_ptr
                              )
{
    if(PNULL == list_head_pptr || PNULL == node_ptr)
    {
        return;
    }
    
    //SCI_TRACE_LOW:" [MMIASP]:AddMsgToListHeader enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_990_112_2_18_2_2_38_45,(uint8*)"");
    
    node_ptr->next_ptr = PNULL;

    if(PNULL == *list_head_pptr)
    {
        *list_head_pptr = node_ptr;   
    }
    else    
    {
        node_ptr->next_ptr = *list_head_pptr;
        *list_head_pptr = node_ptr; 
    }
   
}
/**********************************************************************************/
//  Description :  You can call this fun if you want to know the msg list total num
//  Global resource dependence :none
//  Author:byte.guo
//  Note:  
/**********************************************************************************/
PUBLIC uint16 MMIASP_GetListMsgTotalNum(MMIASP_APPLET_T* applet_ptr)
{
    uint16  list_num = 0;
    MMIASP_MSG_INFO_T *cur_ptr = PNULL;
    
    if( PNULL != applet_ptr )
    {
        cur_ptr = applet_ptr->list_head_ptr;
        
        if(PNULL == cur_ptr)
        {
            return list_num;
        }    
        
        //cur_ptr  =  list_head_ptr;
        while( cur_ptr )
        {
            list_num ++;
            cur_ptr = cur_ptr -> next_ptr;
        }
        
        //SCI_TRACE_LOW:" [MMIASP] MMIASP_GetListMsgTotalNum list_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1030_112_2_18_2_2_38_46,(uint8*)"d",list_num);
        
        return list_num;
    }
    else
    {
        //SCI_TRACE_LOW:" [MMIASP] MMIASP_GetListMsgTotalNum no applet!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1036_112_2_18_2_2_38_47,(uint8*)"");
        return 0;
    }
}
/**********************************************************************************/
//  Description :  delete the really item which has been mark
//  Global resource dependence :none
//  Author:byte.guo
//  Note:  
/**********************************************************************************/
PUBLIC uint16 MMIASP_DelMarkNodeFromList(MMIASP_APPLET_T* applet_ptr)
{
    uint16             del_num = 0;//del number
    MMIASP_MSG_INFO_T *pre_ptr = PNULL;
    MMIASP_MSG_INFO_T *next_ptr = PNULL;
    
    if(PNULL != applet_ptr)
    {
        next_ptr = applet_ptr->list_head_ptr;
        pre_ptr = next_ptr;

        while(PNULL != next_ptr)
        {            
            if(next_ptr->mark_flag)
            {
                /* Jesse add for delete the record file */
                if(PNULL !=next_ptr->record_path)
                {
                    MMIAPIFMM_DeleteFile(next_ptr->record_path, PNULL);
                }
                /* If this node is head node... */
                if (applet_ptr->list_head_ptr == next_ptr)
                {
                    /* Change the next node to the new head node */
                    MMIASP_SetListHead( &applet_ptr->list_head_ptr, next_ptr->next_ptr );
                    /* Free the old head node */
                    SCI_FREE(next_ptr);
                    next_ptr = PNULL;
                    
                    next_ptr = applet_ptr->list_head_ptr;
                }
                else
                {
                    /* Link the next node to pre node */
                    pre_ptr->next_ptr = next_ptr->next_ptr;
                    SCI_FREE(next_ptr);
                    next_ptr = PNULL;
                    next_ptr = pre_ptr->next_ptr;
                }
                
                del_num++;  
            }
            else
            {
                pre_ptr = next_ptr;
                next_ptr = next_ptr->next_ptr;
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:" [MMIASP] MMIASP_DelMarkNodeFromList no applet!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1095_112_2_18_2_2_38_48,(uint8*)"");
    }

    return del_num;
}
/**********************************************************************************/
//  Description :  You can get the cur node by gui list index
//  Global resource dependence :none
//  Author:
//  Note:  
/**********************************************************************************/
PUBLIC MMIASP_MSG_INFO_T* MMIASP_GetCurNodeByGuiIndex(
                                                      const MMIASP_APPLET_T* applet_ptr,
                                                      uint16 gui_list_index
                                                      )
{
    uint16  node_index = 0;

    MMIASP_MSG_INFO_T *cur_ptr = PNULL;
    
    if(PNULL != applet_ptr)
    {
        cur_ptr   = applet_ptr->list_head_ptr;
        
        while(PNULL != cur_ptr )
        {
            //SCI_TraceLow:"MMIASP GetCurNodeByGuiIndex list_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1119_112_2_18_2_2_38_49,(uint8*)"d", node_index);

            if (node_index == gui_list_index)
            {
                return cur_ptr;
            }
            node_index ++;
            cur_ptr = cur_ptr -> next_ptr;
        }
    }
    else
    {
        //SCI_TRACE_LOW:" [MMIASP] MMIASP_GetCurNodeByGuiIndex no applet!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1131_112_2_18_2_2_38_50,(uint8*)"");
    }
    
    
    return cur_ptr;
}
/**********************************************************************************/
//  Description : check sys file head
//  Global resource dependence :none
//  Author:byte.guo
//  Note:
/**********************************************************************************/
PUBLIC BOOLEAN MMIASP_CheckVersionNumber(
                                         MMIFILE_HANDLE file_handle,
                                         const wchar* right_version_ptr,
                                         uint16       right_version_len
                                         )
{
    BOOLEAN result = FALSE;
    uint32  read_size  = 0;
    uint16  answer_machine_version_len = right_version_len;
    wchar*  answer_machine_version_ptr = PNULL;
    
    if(SFS_INVALID_HANDLE != file_handle)
    {
        /* Read and check version number */
        answer_machine_version_ptr = (wchar *)SCI_ALLOCA(answer_machine_version_len* sizeof(wchar) + 2);
        if(PNULL != answer_machine_version_ptr)
        {
            SCI_MEMSET( answer_machine_version_ptr,0x00, ( answer_machine_version_len* sizeof(wchar) + 2) );    
            
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(
                                                file_handle, 
                                                answer_machine_version_ptr, 
                                                answer_machine_version_len * sizeof(wchar), 
                                                &read_size, 
                                                PNULL
                                                ))
            {
                if(0 == MMIAPICOM_Wstrncmp( (const wchar* )answer_machine_version_ptr, 
                    (const wchar* )right_version_ptr, 
                    answer_machine_version_len 
                    ) 
                    )
                {
                    result = TRUE;
                }
                
            } 
            
            /* Let's free memory */
            SCI_FREE(answer_machine_version_ptr);
            answer_machine_version_ptr = PNULL;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_CheckVersionNumber result = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1185_112_2_18_2_2_38_51,(uint8*)"d",result);
    
    return result;
}
/**********************************************************************************/
//  Description : creat a new sys file
//  Global resource dependence :none
//  Author:byte.guo
//  Note:
/**********************************************************************************/
PUBLIC BOOLEAN MMIASP_CreateNewSysFile(void)
{
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    wchar  file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32          write_len = 0;
    BOOLEAN         result = FALSE;
    
    if (GetSysFileFullNameForWrite(file_name, &path_len))
    {
        /* Whatever happens,creat a new file .*/
        file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
        
        if (SFS_INVALID_HANDLE != file_handle)
        {
            /* Fill sys head only */
            if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(
                file_handle, 
                s_answer_machine_version,
                MMIAPICOM_Wstrlen((const wchar*)s_answer_machine_version) * sizeof(wchar),
                &write_len, 
                NULL
                ))
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            } 
            
            MMIAPIFMM_CloseFile(file_handle);
        }
    }
    
    
    //SCI_TRACE_LOW:"[MMIASP]:MMIASP_CreateNewSysFile file_handle = %d,result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1229_112_2_18_2_2_38_52,(uint8*)"dd",file_handle,result);
    return result;
}
/***********************************************************************************
* Function Name: MMIASP_WriteSysFile answer machine sys file
* Purpose:       write system file
* Author:byte.guo
// Note: 
|---n----|----4---|----4-------|------n------|-----n-------|----------n-------|
|sys_head|list_num|new_item_num|file_name_len|file_name_str|cur_node_structure|-more--
|-----------------------------------------------------------------------------|       
***********************************************************************************/
LOCAL BOOLEAN ReadASPSysFile(void)
{
    uint32   read_size  = 0;
    uint32   list_num   = 0;
    uint32   new_item_num  = 0;
    uint32   file_path_len = 0;
    wchar    *file_path_str= PNULL;
    
    uint32   write_offset = 0;
    MMIFILE_HANDLE file_handle = 0;  
    
    wchar  file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32          structure_len          = sizeof(MMIASP_MSG_INFO_T);
    uint32          total_file_size        = 0;

    if(!GetSysFileFullNameForRead(file_name, &path_len))
    {
        return FALSE;
    }
    s_msg_list_header_ptr=PNULL;
    
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL); /*lint !e655*/
    // 在这个函数里面对file_handle有判断...............
    if(MMIASP_CheckVersionNumber(file_handle, s_answer_machine_version, MMIAPICOM_Wstrlen(s_answer_machine_version)))
    {
        write_offset += MMIAPICOM_Wstrlen( s_answer_machine_version ) * sizeof(wchar);
        
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &list_num, sizeof(uint32), &read_size, PNULL))
        {
            MMIAPIFMM_CloseFile( file_handle );
            return FALSE;
        }

        write_offset += sizeof(uint32);
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &new_item_num, sizeof(uint32), &read_size, PNULL))
        {
            MMIAPIFMM_CloseFile( file_handle );
            return FALSE;
        }
        write_offset += sizeof(uint32);
        //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile list_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1281_112_2_18_2_2_38_53,(uint8*)"d",list_num);
        while (0 < list_num)
        {
            list_num--;
            /*-----------------------------------read-----------------------file_path--start----*/
            if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &file_path_len, sizeof(uint32), &read_size, PNULL))
            {
                //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile read filename fail ---"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1288_112_2_18_2_2_38_54,(uint8*)"");
                MMIAPIFMM_CloseFile( file_handle );
                return FALSE;
            }
            
            //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile read file_path_len:%d  ---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1293_112_2_18_2_2_38_55,(uint8*)"d",file_path_len);
            
            /* Read cur filename str */
            write_offset += sizeof(uint32);
            
            /* important !!,check file_path_len */
            if (MMIAPIFMM_GetFileSize(file_handle) < file_path_len)
            {
                //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile read file_path_len fail = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1301_112_2_18_2_2_38_56,(uint8*)"d",file_path_len);
                MMIAPIFMM_CloseFile( file_handle );
                return FALSE;
            }
            
            file_path_str = (wchar *)SCI_ALLOC_APP(file_path_len* sizeof(wchar) + 2);
			if (PNULL == file_path_str)
			{
				//SCI_TRACE_LOW:"ReadASPSysFile alloc failed"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1309_112_2_18_2_2_38_57,(uint8*)"");
                return FALSE;
			}
            SCI_MEMSET( file_path_str,0x00,(file_path_len* sizeof(wchar) + 2)); 
            
            if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, file_path_str, file_path_len* sizeof(wchar), &read_size, PNULL))
            {
                //SCI_TRACE_LOW:" [MMIASP]: read filename fail ---"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1316_112_2_18_2_2_38_58,(uint8*)"");
                /* It's must be free memory if read file fail */
                SCI_FREE(file_path_str);
                file_path_str = PNULL;
                MMIAPIFMM_CloseFile( file_handle );
                return FALSE;
            }
            
            //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile read file_path_str:%s  ---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1324_112_2_18_2_2_38_59,(uint8*)"s",file_path_str);
            
            /*--------------------------------------read-----------------------file_path--end----*/
            
            write_offset += (file_path_len* sizeof(wchar));
            {
                MMIASP_MSG_INFO_T* msg_info_ptr = MMIASP_MsgInfoMalloc();
                
                if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, msg_info_ptr, structure_len, &read_size, NULL))
                {
                    /* Do not have to add to list  when the file had been deleted */
                    /* It is special case,let me see... */
                    msg_info_ptr->record_path = file_path_str;
                    MMIASP_AddMsg2ListEnd(&s_msg_list_header_ptr, msg_info_ptr);
                }
                else
                {
                    //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile read structure fail ---"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1341_112_2_18_2_2_38_60,(uint8*)"");
                    MMIAPIFMM_CloseFile( file_handle );
                    if(PNULL != msg_info_ptr)
                    {
                        SCI_FREE(msg_info_ptr);
                        msg_info_ptr = PNULL;
                    }
                    //if(PNULL != file_path_str)
                    {
                        SCI_FREE(file_path_str);
                        file_path_str = PNULL;
                    }
                    return FALSE;
                }        
            }
            
            write_offset +=  structure_len;
            /* Very important check  */
            total_file_size = MMIAPIFMM_GetFileSize(file_handle);
            //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile write_offset = %d,total_file_size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1360_112_2_18_2_2_38_61,(uint8*)"dd",write_offset,total_file_size);
            if ( total_file_size <= write_offset )
            {
                break;
            }
        }
        
        
        MMIAPIFMM_CloseFile( file_handle );
        
    }
    else
    {
        //SCI_TRACE_LOW:" [MMIASP]: MMIASP_ReadSysFile read sys file head fail ---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1373_112_2_18_2_2_38_62,(uint8*)"");
        MMIAPIFMM_CloseFile( file_handle );
        return FALSE;
    }
 
    return TRUE;
}
/*****************************************************************************/
//   Description : 
//   Global resource dependence : none
//   Author: 
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_ReadSysFileWithApplet(MMIASP_APPLET_T* applet_ptr)
{
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP MMIASP_ReadSysFileWithApplet no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1388_112_2_18_2_2_39_63,(uint8*)"");
        return FALSE;
    }
    MMIASP_MsgListFree(applet_ptr);
    if(ReadASPSysFile())
    {
        applet_ptr->list_head_ptr = s_msg_list_header_ptr;
        applet_ptr->new_msg_num = MMIASP_GetNewMsgNum(applet_ptr);
        return TRUE;
    }
    else
    {
        applet_ptr->list_head_ptr = PNULL;
        applet_ptr->new_msg_num = 0;
        return MMIASP_CreateNewSysFile();
    }
    
}
/**********************************************************************************
* Function Name: MMIASP_WriteSysFile answer machine sys file
* Purpose:       write system file
* Author:byte.guo
// Note: 
|---n----|----4---|----4-------|------n------|-----n-------|----------n-------|
|sys_head|list_num|new_item_num|file_name_len|file_name_str|cur_node_structure|-more--
|-----------------------------------------------------------------------------|       
**********************************************************************************/
PUBLIC BOOLEAN MMIASP_WriteSysFile(void)
{
    uint32  list_num      = 0;
    uint32  new_item_num  = 0;
    uint32  file_path_len = 0;
    uint32  write_offset  = 0;
    uint32  write_len     = 0;
    
    MMIFILE_HANDLE  file_handle = 0;
    wchar  file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    MMIASP_MSG_INFO_T* msg_temp_ptr  = PNULL;
    MMIASP_MSG_INFO_T* msg_next_ptr  = PNULL;
    uint32            structure_size = sizeof(MMIASP_MSG_INFO_T);

    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP MMIASP_WriteSysFile no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1433_112_2_18_2_2_39_64,(uint8*)"");
        return FALSE;
    }
    
    list_num = MMIASP_GetListMsgTotalNum(applet_ptr);
    new_item_num = MMIASP_GetNewMsgNum(applet_ptr);
    
    /* Check sys file path */
    if (GetSysFileFullNameForWrite(file_name, &path_len))
    {
        file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        
        if(SFS_INVALID_HANDLE != file_handle)
        {
            /* Write version number */
            if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(
                file_handle, 
                s_answer_machine_version,     
                MMIAPICOM_Wstrlen((const wchar*)s_answer_machine_version) * sizeof(wchar),
                &write_len, 
                PNULL
                ))
            {
                //SCI_TRACE_LOW:" [MMIASP]: MMIASP_WriteSysFile s_answer_machine_version fail ---"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1456_112_2_18_2_2_39_65,(uint8*)"");
                MMIAPIFMM_CloseFile(file_handle);
                return FALSE;
            } 
            
            write_offset += MMIAPICOM_Wstrlen((const wchar*)s_answer_machine_version) * sizeof(wchar);
            
            /* Write list number */
            if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &list_num, sizeof(uint32), &write_len, PNULL))
            {
                MMIAPIFMM_CloseFile(file_handle);
                return FALSE;
            }
            write_offset += sizeof(uint32);
            
            // Write new  item number
            if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &new_item_num, sizeof(uint32), &write_len, PNULL))
            {
                MMIAPIFMM_CloseFile(file_handle);
                return FALSE;
            }
            
            write_offset += sizeof(uint32);
            
            
            //SCI_TRACE_LOW:" [MMIASP]: MMIASP_WriteSysFile list_num = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1481_112_2_18_2_2_39_66,(uint8*)"d",list_num);
            msg_next_ptr = applet_ptr->list_head_ptr;
            while(PNULL != msg_next_ptr)
            {             
                msg_temp_ptr = msg_next_ptr;
                msg_next_ptr = msg_next_ptr->next_ptr;
                /*---------------------------------write----------------------file_path--start----*/
                /* Write cur record_path len */
                file_path_len = MMIAPICOM_Wstrlen(msg_temp_ptr->record_path);
                if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &file_path_len, sizeof(uint32), &write_len, PNULL))
                {
                    //SCI_TRACE_LOW:" [MMIASP]: MMIASP_WriteSysFile write filename fail ---"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1492_112_2_18_2_2_39_67,(uint8*)"");
                    MMIAPIFMM_CloseFile(file_handle);
                    return FALSE;
                }            
                
                /* Write cur record_path str */
                write_offset += sizeof(uint32);
                if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, msg_temp_ptr->record_path, file_path_len * sizeof(wchar), &write_len, PNULL))
                {
                    //SCI_TRACE_LOW:" [MMIASP]: MMIASP_WriteSysFile write filename fail ---"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1501_112_2_18_2_2_39_68,(uint8*)"");
                    MMIAPIFMM_CloseFile(file_handle);
                    return FALSE;
                }            
                /*------------------------------------write-----------------------file_path--end----*/
                write_offset += (file_path_len * sizeof(wchar));
                if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, msg_temp_ptr, structure_size, &write_len, PNULL))
                {
                    //SCI_TRACE_LOW:" [MMIASP]: MMIASP_WriteSysFile write structure fail ---"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1509_112_2_18_2_2_39_69,(uint8*)"");
                    MMIAPIFMM_CloseFile(file_handle);
                    return FALSE;
                }
                
                write_offset += structure_size;
            } 
            
            applet_ptr->new_msg_num = MMIASP_GetNewMsgNum(applet_ptr);
            MMIAPIFMM_CloseFile(file_handle);
        }
    }
    else
    {
        return FALSE;
    }
    // 更新idle上的图标指示.....
    MMIAPIIDLE_UpdateAspState();
    return TRUE;
}
/*****************************************************************************/
//  Description : get record file dev
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIASP_GetRecordFileDev(MMIASP_SAVE_TYPE_E file_type)
{
    uint32       free_size = 0;
    MMIFILE_DEVICE_E suit_disk = MMI_DEVICE_NUM;
    
    
    if (MMIASP_SYSTEM_FILE == file_type)
    {
        free_size = MMIASP_SYSTEM_FILE_NEED_SPACE;
    }
    else
    {
        free_size = MMIASP_RECORD_FILE_NEED_SPACE;
    }

    MMIAPIFMM_GetSuitableFileDev( MMIASP_GetSettingNvValue().record_save_dev, free_size, &suit_disk);
    
    //SCI_TRACE_LOW:" [MMIASP] MMIASP_GetRecordFileDev file_dev=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1550_112_2_18_2_2_39_70,(uint8*)"d", suit_disk);
    return suit_disk;
}

/*****************************************************************************/
//  Description : start record 
//  Global resource dependence : none
//  Author: byte.guo
//  Note: jun.hu 把此函数MMIASP_StartRecord改成Local类型的...
/*****************************************************************************/
LOCAL BOOLEAN StartRecord(void)
{
    uint32          cur_file_name_len = 0;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;
    BOOLEAN         result = FALSE;
    uint8           file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    //MMIASP_MSG_INFO_T*  msg_info_ptr = MMIASP_GetLastNode(applet_ptr);
    MMIASP_MSG_INFO_T* msg_info_ptr = PNULL;
    
    MMISRVAUD_TYPE_U    audio_data = {0};
    MMISRV_HANDLE_T     audio_handle = MMIASP_GetRecordHandle();
        
    if(!GetAudioFolder(&file_dev))
    {
        //SCI_TRACE_LOW:"[MMIASP]:  no suitable dev"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1573_112_2_18_2_2_39_71,(uint8*)"");
        MMIAPICC_ReleaseAspCallReq(MN_CAUSE_NORMAL_CLEARING);
        return FALSE;
    }

    if(PNULL == applet_ptr)
    {
	    return FALSE;
    }
	
    StopPlayAudio();
    if(MMI_NONE_TIMER_ID != applet_ptr->answer_ring_time_id)
    {
    	MMK_StopTimer(applet_ptr->answer_ring_time_id);
    	applet_ptr->answer_ring_time_id= MMI_NONE_TIMER_ID;
    }

    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StartRecord file_dev = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1585_112_2_18_2_2_39_72,(uint8*)"d",file_dev);
    if( MMI_DEVICE_NUM == file_dev || PNULL == s_new_msg_node.record_path)
    {
        //SCI_TRACE_LOW:"MMIASP StartRecord file_dev= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1588_112_2_18_2_2_39_73,(uint8*)"d", file_dev);
        MMIAPICC_ReleaseAspCallReq(MN_CAUSE_NORMAL_CLEARING);
        return FALSE;
    }
    
    //cur_full_path     = MMIASP_GetLeaveWordPath(applet_ptr->list_head_ptr, &s_new_msg_node);
    cur_file_name_len = MMIAPICOM_Wstrlen((const wchar*)s_new_msg_node.record_path);
    MMIAPICOM_WstrToStr(s_new_msg_node.record_path, file_name);
    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StartRecord cur_full_path = %s,cur_file_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1596_112_2_18_2_2_39_74,(uint8*)"sd",file_name, cur_file_name_len);

    if(MMIAPIFMM_IsFileExist(s_new_msg_node.record_path, cur_file_name_len))
    {
        MMIAPIFMM_DeleteFile(s_new_msg_node.record_path, PNULL);
    }
    // 这里要MMISRVAUD_SetUpLinkMute， 否则由于没有播放音乐了，虽然用的是魔音的流程，也会让对方听到主机
    if(MMICC_EnableMute(TRUE))
    {
        //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StartRecord MMISRVAUD_SetUpLinkMute success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1605_112_2_18_2_2_39_75,(uint8*)"");
    }

      //取消对方静音设置
    MMICC_EnableRemoteMute(FALSE);
      
    //MMISRVAUD_SetVolume(audio_handle, AUD_MAX_SPEAKER_VOLUME);

    audio_data.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
    audio_data.record_file.fmt  = MMISRVAUD_RECORD_FMT_ADPCM;
    audio_data.record_file.name = s_new_msg_node.record_path;
    audio_data.record_file.name_len  = MMIAPICOM_Wstrlen(audio_data.record_file.name);
    audio_data.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;
    audio_data.record_file.source    = MMISRVAUD_RECORD_SOURCE_CALL;

    if(MMIASP_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, ASPRecordCallback))
    {
        MMIASP_SetRecordHandle(audio_handle);
        MMISRVAUD_Play(audio_handle, 0);
        msg_info_ptr = MMIASP_MsgInfoMalloc();
        if(PNULL != msg_info_ptr)
        {
            SCI_MEMCPY(msg_info_ptr, &s_new_msg_node, sizeof(s_new_msg_node));
            AddMsgToListHeader(&(applet_ptr->list_head_ptr), msg_info_ptr);
            applet_ptr->cur_gui_list_index++;
            //MMIASP_AddMsg2ListEnd(&(applet_ptr->list_head_ptr), msg_info_ptr);
            /* Update sys file */
            MMIASP_WriteSysFile();
            MMIASP_SetCallState(MMIASP_MACHINE_CALL_RECORDING);
            CreateRecordTimer();
        }
        else
        {
            //SCI_TRACE_LOW:"MMIASP StartRecord malloc memory fail ERROR!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1638_112_2_18_2_2_39_76,(uint8*)"");
        }
        
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP StartRecord fail, please check"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1644_112_2_18_2_2_39_77,(uint8*)"");
        // 如果录音失败，则挂断电话......
        //MMIASP_RealseCall();
        MMIAPICC_ReleaseAspCallReq(MN_CAUSE_NORMAL_CLEARING);
		MMICC_EnableMute(FALSE);
        result = FALSE;   
    }
    
    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StartRecord result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1651_112_2_18_2_2_39_78,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetDefaultAnswerRingFullName(
                                          wchar* file_name_ptr, //out
                                          uint16 file_name_max_len
                                          )
{
    uint16 file_name_len = 0;
    
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();

    if(PNULL == file_name_ptr || 0 == file_name_max_len)
    {
        return 0;
    }
    SCI_MEMSET(file_name_ptr, 0, file_name_max_len);
    
    //SCI_TRACE_LOW:"MMIASP GetDefaultAnswerRingFullName entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1674_112_2_18_2_2_39_79,(uint8*)"");

    file_name_len = MMIAPICOM_Wstrlen(cur_nv.record_path_for_client);
    //SCI_TRACE_LOW:"MMIASP nv set file_name_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1677_112_2_18_2_2_39_80,(uint8*)"d", file_name_len);
    if(file_name_len > file_name_max_len)
    {
        return 0;
    }
    
    MMIAPICOM_Wstrncpy(file_name_ptr, cur_nv.record_path_for_client, file_name_len);
    
    
    return file_name_len;
    
}

#ifndef WIN32
extern void  BKMUSIC_SetGain(uint32  eParamType, uint16  uiUplinkGain, uint16  uiDownlinkGain);
#endif
/*****************************************************************************/
//  Description : At first,play fixed music for client
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_PlayAudioInCall(MMIASP_APPLET_T* applet_ptr, const MMIASP_MSG_INFO_T* msg_info)
{
    MMIAUD_RING_DATA_INFO_T ring_info = {0};
   
    wchar    file_name_arr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    
    BOOLEAN             play_ret = FALSE;
    uint32              custom_ring_id = 0;
    uint16              ring_id = 0;
    BOOLEAN             is_fixed_ring = FALSE;
    BOOLEAN             own_custom_ring = FALSE;
    uint32              file_name_len = 0;
    MMISRV_HANDLE_T     audio_handle = 0;

    //SCI_TRACE_LOW:"[MMIASP]:MMIASP_PlayAudioInCall open background music!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1711_112_2_18_2_2_39_81,(uint8*)"");
    if(PNULL == applet_ptr || PNULL == msg_info)
    {
        //SCI_TRACE_LOW:"[MMIASP]:MMIASP_PlayAudioInCall  PARA HAS error !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1714_112_2_18_2_2_39_82,(uint8*)"");
        return FALSE;
    }
    // 因为有隐藏号码，所以这里必须去掉......
    /*if(0 == strlen((char*)msg_info->call_num))
    {
        //SCI_TRACE_LOW:"[MMIASP]:MMIAPIASP_Play4Client:call_num is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1720_112_2_18_2_2_39_83,(uint8*)"");
        return  FALSE;
    }*/
    //bug 188338
    // 确保上行静音,防止答录语播放时，本端的其他声音传到对端.......
    if(!MMICC_EnableMute(TRUE))
    {
        //SCI_TRACE_LOW:"MMIASP_PlayAudioInCall:MMICC_EnableMute SET NOT MUTE FAIL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1727_112_2_18_2_2_39_84,(uint8*)"");
        MMISRVAUD_SetUpLinkMute(TRUE);
    }

     //答录语播放时不要听到对方声音.....
    MMICC_EnableRemoteMute(TRUE);

#ifndef WIN32
    // 设置成魔音, 以背景音的形式走魔音的流程......
    BKMUSIC_SetGain(ASP_DEFAULT_MAGIC_GAIN, ASP_DEFAULT_UPLINK_GAIN, ASP_DEFAULT_DOWNLINK_GAIN);
#endif

    MMIAPIASP_StopWaitTimer();
    
    //SCI_TRACE_LOW:"[MMIASP]:MMIASP_PlayAudioInCall audio_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1741_112_2_18_2_2_39_85,(uint8*)"d",applet_ptr->nv_info.audio_type);
    
    if(GetCustomerAnswerRing(msg_info->call_num, &is_fixed_ring, file_name_arr, &file_name_len, &custom_ring_id))
    {
        own_custom_ring = TRUE;
        ring_id = (uint16)custom_ring_id;
        //SCI_TRACE_LOW:"MMIASP has set own answer ring"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1747_112_2_18_2_2_39_86,(uint8*)"");
    }
    //  如果该用户没有设置答录语，
    if(!own_custom_ring )
    {   // 再去查找asp 是否设置的是固定的答录语....
        is_fixed_ring = GetDefaultFixedAnswerRing((uint16*)&ring_id);
    }

    //SCI_TRACE_LOW:"[MMIASP]: is_fixed_ring = %d, ring_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1755_112_2_18_2_2_39_87,(uint8*)"dd", is_fixed_ring, ring_id);

    if(is_fixed_ring)
    {
        MMISRVAUD_TYPE_U    audio_data = {0};
    
        //SCI_TRACE_LOW:"MMIASP ring id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1761_112_2_18_2_2_39_88,(uint8*)"d", ring_id);
        
        //MMI_GetRingInfo((ring_id  + R_ASP_1 ), &ring_info);
        
         MMIRES_GetRingInfo(ring_id  + R_ASP_1, applet_ptr->caf_applet.app_handle, &ring_info);
         audio_data.ring_buf.type = MMISRVAUD_TYPE_ASP_BUF;
         audio_data.ring_buf.fmt  =(MMISRVAUD_RING_FMT_E)(ring_info.type);
         audio_data.ring_buf.data = ring_info.data_ptr;
         audio_data.ring_buf.data_len = ring_info.data_len;
        
         //SCI_TRACE_LOW:"[MMIASP]:MMIASP_PlayAudioInCall data_len=%d, data_ptr=0x%x"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1771_112_2_18_2_2_39_89,(uint8*)"dd",ring_info.data_len, ring_info.data_ptr);

         /* Create handle */
         MMIASP_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_AUTO_NO_BLUETOOTH, &audio_data, ASPPlayRemoteCallback);
    }
    else
    {
        MMISRVAUD_TYPE_U    audio_data = {0};
    
        if(!own_custom_ring || 0 == file_name_len)
        {
            file_name_len = GetDefaultAnswerRingFullName(file_name_arr, MMIFILE_FILE_NAME_MAX_LEN);
        }
        
        if(0 ==  file_name_len)
        {
            //SCI_TRACE_LOW:"MMIASP name len = 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1787_112_2_18_2_2_39_90,(uint8*)"");
            StartRecord();
            return FALSE;
        }
        
        //SCI_TRACE_LOW:"[MMIASP]:  play MORE ring file name = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1792_112_2_18_2_2_39_91,(uint8*)"s", file_name_arr);
        //SCI_TRACE_LOW:"[MMIASP]:  play MORE ring file name len= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1793_112_2_18_2_2_39_92,(uint8*)"d", file_name_len);
        
        audio_data.ring_file.type = MMISRVAUD_TYPE_ASP_FILE;
        audio_data.ring_file.name = file_name_arr;
        audio_data.ring_file.name_len = file_name_len;
        audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)(MMIAPICOM_GetMusicType(file_name_arr, file_name_len));
        
        /* Create handle */
        MMIASP_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_AUTO_NO_BLUETOOTH, &audio_data, ASPPlayRemoteCallback);
    }
    
    //SCI_TRACE_LOW:"[MMIASP]:applet_ptr->bkmusic_audio_handle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1804_112_2_18_2_2_39_93,(uint8*)"d", applet_ptr->bkmusic_audio_handle);
    /* Check bkmusic handle */
    if (audio_handle != 0)
    {
        applet_ptr->bkmusic_audio_handle = audio_handle;
        play_ret = MMISRVAUD_Play(applet_ptr->bkmusic_audio_handle, 0);
        if(FALSE == play_ret)
        {
            MMISRVAUD_Stop(audio_handle);
            MMISRVMGR_Free(audio_handle);
            applet_ptr->bkmusic_audio_handle = 0;
            /* Start record */
            StartRecord();
        }
        else
        {
            if(MMI_NONE_TIMER_ID != applet_ptr->answer_ring_time_id)
            {
                MMK_StopTimer(applet_ptr->answer_ring_time_id);
                applet_ptr->answer_ring_time_id = MMI_NONE_TIMER_ID;
            }
            applet_ptr->answer_ring_time_id= MMK_CreateWinTimer(MMIASP_RECORDING_WIN_ID, MMI_10SECONDS*6, FALSE);
        }
        
        //SCI_TRACE_LOW:"[MMIASP]:MMIAPIASP_Play4Client play_ret = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1828_112_2_18_2_2_39_94,(uint8*)"d",play_ret);
    }
    else/* Handle creat fail */
    {
        /* Start record */
        StartRecord();
    }

    if(TRUE == play_ret)
    {
        return TRUE;  
    }
    else
    {
        return FALSE;  
    }   
}


/**********************************************************************************/
//  Description :  get new msg number
//  Global resource dependence :none
//  Author:
//  Note:  
/**********************************************************************************/
PUBLIC uint16 MMIASP_GetNewMsgNum(MMIASP_APPLET_T* applet_ptr)
{
    uint16  new_msg_num = 0;
    MMIASP_MSG_INFO_T *cur_ptr = PNULL;
    
    if(PNULL == applet_ptr)
    {
        return 0;
    }
    
    cur_ptr = applet_ptr->list_head_ptr;
    while(PNULL != cur_ptr)
    {
        if (!cur_ptr->read_flag )
        {
            new_msg_num ++;
        }
        
        cur_ptr = cur_ptr -> next_ptr;
    }
    
    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_GetNewMsgNum new_msg_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1872_112_2_18_2_2_40_95,(uint8*)"d",new_msg_num);
    return new_msg_num;
}
/**********************************************************************************/
//  Description : searcj free number
//  Global resource dependence :none
//  Author:
//  Note:  
/**********************************************************************************/
LOCAL BOOLEAN SearchFreeNum(
                            MMIASP_MSG_INFO_T* cur_ptr,         //[in]
                            const  uint8*      call_number_ptr, //[in]
                            uint32              suffix_id
                            )
{
    
    if(0 == strlen((char*)call_number_ptr))
    {
        while(PNULL != cur_ptr)
        {
            if(0 == cur_ptr->call_num[0])
            {
                if (cur_ptr->suffix_id == suffix_id)
                {
                    return FALSE;
                }
            }
            
            cur_ptr = cur_ptr -> next_ptr;
        }
    }
    else
    {
        while(PNULL != cur_ptr)
        {
            if (0 == strcmp((char*)call_number_ptr, (char*)cur_ptr->call_num))
            {
                if (cur_ptr->suffix_id == suffix_id)
                {
                    return FALSE;
                }
            }
            
            cur_ptr = cur_ptr -> next_ptr;
        }
    }
    
    return TRUE;
}

/**********************************************************************************/
//  Description :  id
//  Global resource dependence :none
//  Author:
//  Note:  
/**********************************************************************************/
LOCAL uint8 GetSuffixIdByTelNum(
                                MMIASP_MSG_INFO_T* list_head_ptr,
                                const  uint8*      call_number_ptr
                                )
{
    uint32        suffix_id       = 0;
    uint8         call_number_len = 0;
    
    MMIASP_MSG_INFO_T *cur_ptr = PNULL;
    
    if(PNULL == list_head_ptr || PNULL == call_number_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP GetPbNumIdByTelNum find NULL head OR call number error = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_1936_112_2_18_2_2_40_96,(uint8*)"d", call_number_ptr);
        return suffix_id + 1;
    }
    
    call_number_len = strlen((char*)call_number_ptr);
    cur_ptr  =  list_head_ptr;
    
    // 如果是隐藏号码......
    if(0 == call_number_len)
    {
        while(PNULL != cur_ptr)
        {
            if(0 == cur_ptr->call_num[0])
            {
                if (suffix_id < cur_ptr->suffix_id)
                {
                    suffix_id = cur_ptr->suffix_id;
                }
            }
            cur_ptr = cur_ptr -> next_ptr;
        }
    }
    else
    {
        while(PNULL != cur_ptr)
        {
            if (0 == strcmp((char*)call_number_ptr, (char*)cur_ptr->call_num))
            {
                /* find max suffix_id */
                if (suffix_id < cur_ptr->suffix_id)
                {
                    suffix_id = cur_ptr->suffix_id;
                }
                
            }
            
            cur_ptr = cur_ptr -> next_ptr;
        }
        
    }
    
    /* Fun will find the free id when msg do not to save(max),
    actually and at this time,user maybe deleted some msg file already.
    */
    if (MMIASP_MAX_MSG_NUM == suffix_id)
    {
        for (suffix_id = 1; suffix_id <= MMIASP_MAX_MSG_NUM; suffix_id++ )
        {
            if(SearchFreeNum(list_head_ptr, call_number_ptr, suffix_id))
            {
                return suffix_id;
            }
        }
        
    }
    
    return (suffix_id + 1);
}
/*****************************************************************************/
// Description : Combine file path
// Global resource dependence : none
// Author: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN CombineASPFilePath(
                                 MMIASP_APPLET_T*     applet_ptr,       //in
                                 MMIASP_MSG_INFO_T*   msg_info,         //in
                                 const  MMI_STRING_T* name_str,         //in
                                 const  uint8*        call_number_str   //in
                                 )
{
    uint8               pb_num[10] = {0};
    wchar               pb_num_str[10] = {0};
    uint8               pb_num_len = 0;
    
    wchar               file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    wchar               file_full_path_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16              file_name_len = 0;
    uint16              file_full_path_len = MMIFILE_FILE_NAME_MAX_LEN;
    uint16              total_vaild_len = 0;
    
    const wchar         underline_str[] = {'_',0};
    const wchar         wav_str[] = {'.','w','a','v',0};

    MMIFILE_DEVICE_E    file_dev_type  = MMI_DEVICE_NUM;
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    
    BOOLEAN             result = FALSE;
    
    //SCI_TraceLow:"MMIASP CombineASPFilePath entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2024_112_2_18_2_2_40_97,(uint8*)"");
    
    if(PNULL == applet_ptr || PNULL == msg_info || PNULL == name_str)
    {
        return FALSE;
    }

    if(!GetAudioFolder(&file_dev_type))
    {
        return FALSE;
    }

    file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev_type);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev_type);

    if((PNULL == file_dev_ptr) || (0 == file_dev_len))
	{
		return FALSE;
	}
    
    // 得到编号.....
    msg_info->suffix_id = GetSuffixIdByTelNum(applet_ptr->list_head_ptr, call_number_str);
    //SCI_TraceLow:"MMIASP suffix_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2046_112_2_18_2_2_40_98,(uint8*)"d", msg_info->suffix_id);
    sprintf((char*)pb_num, "%d", (int)msg_info->suffix_id);
    MMIAPICOM_StrToWstr(pb_num, pb_num_str);
    pb_num_len = MMIAPICOM_Wstrlen((const wchar*)pb_num_str);
    //SCI_TraceLow:"MMIASP pb_num_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2050_112_2_18_2_2_40_99,(uint8*)"d", pb_num_len);
    
    // 形成文件名....
    MMIAPICOM_Wstrncpy(file_name, name_str->wstr_ptr, name_str->wstr_len);
    MMIAPICOM_Wstrcat(file_name, underline_str);
    MMIAPICOM_Wstrcat(file_name, pb_num_str);
    MMIAPICOM_Wstrcat(file_name, wav_str);
    file_name_len = MMIAPICOM_Wstrlen(file_name);
    
    // 形成全路径名......
    MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
        file_name, file_name_len, file_full_path_name, &file_full_path_len);
    total_vaild_len = (file_full_path_len <= MMIFILE_FILE_NAME_MAX_LEN) ? file_full_path_len: MMIFILE_FILE_NAME_MAX_LEN;
    
    msg_info->record_path = (wchar* )SCI_ALLOC_APP((total_vaild_len + 1) * sizeof(wchar) );
    if(PNULL != msg_info->record_path)
    {
        SCI_MEMSET(msg_info->record_path, 0x00, (total_vaild_len + 1) * sizeof(wchar));
        MMIAPICOM_Wstrncpy(msg_info->record_path, file_full_path_name, total_vaild_len);
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    return result;
}
/*****************************************************************************/
//   Description :  通过号码获取名字
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_GetNameByNumber(
                                      uint8*    call_num_ptr,//in
                                      MMI_STRING_T*   name_ptr //out
                                      )
{   
    BOOLEAN search_result = FALSE;
    MMIPB_BCD_NUMBER_T    pb_bcd = {0};
    MMI_PARTY_NUMBER_T    party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    
    /* Convert str to BCD */
    if(MMIAPICOM_GenPartyNumber(call_num_ptr, strlen((char*)call_num_ptr), &party_num))
    {
        pb_bcd.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
        pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
        SCI_MEMCPY( 
            pb_bcd.number,  
            party_num.bcd_num, 
            pb_bcd.number_len 
            );
        
        search_result = MMIAPIPB_GetNameByNumber(&pb_bcd, name_ptr, CC_MAX_PBNAME_LEN, TRUE);
    }
    
    if(search_result)
    {
        // 如果是未命名的情况, 则返回FALSE
        if(MMIAPIPB_IsUseDefaultName(name_ptr->wstr_ptr))
        {
            search_result = FALSE;
        }
    }

    return search_result;
}

/*****************************************************************************/
//   Description : 通过号码获取名字
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_GetNameByNumber(
                                      uint8*          call_num_ptr,//in
                                      MMI_STRING_T*   name_ptr //out
                                      )
{
    return MMIASP_GetNameByNumber(call_num_ptr, name_ptr);
}

/*****************************************************************************/
//   Description : set asp info
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_SetCurMsgInfo(
                                    MMIASP_APPLET_T* applet_ptr,
                                    MMIASP_MSG_INFO_T* msg_info_ptr
                                    )
{
    MMIASP_MSG_INFO_T* msg_info_temp_ptr = msg_info_ptr;
    wchar         telname[CC_MAX_PBNAME_LEN + 1]   = {0};
    wchar         name[CC_MAX_PBNAME_LEN + 1]   = {0};
    uint8         cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8         tel_num_len = CC_MAX_TELE_NUM_LEN + 2;
    MMI_STRING_T  cur_time_str = {0};
    MMI_STRING_T  call_name_str = {0};
    
    SCI_DATE_T    sys_date = {0};
    SCI_TIME_T    sys_time = {0};

    //SCI_TRACE_LOW:"MMIASP MMIASP_SetCurMsgInfo entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2149_112_2_18_2_2_40_100,(uint8*)"");
    cur_time_str.wstr_ptr = msg_info_temp_ptr->time;
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);               
    
    msg_info_temp_ptr->date_info.sys_date.year = sys_date.year;
    msg_info_temp_ptr->date_info.sys_date.mon  = sys_date.mon;
    msg_info_temp_ptr->date_info.sys_date.mday = sys_date.mday;
    msg_info_temp_ptr->date_info.sys_time.hour = sys_time.hour;
    msg_info_temp_ptr->date_info.sys_time.min  = sys_time.min;
    msg_info_temp_ptr->date_info.sys_time.sec  = sys_time.sec;
    
    /* Get call number */
    MMIAPICC_GetCallName(&call_name_str);

    tel_num_len = MMIAPICC_GetCallNumber(cur_call_num, tel_num_len);
    
    MMI_MEMCPY((void*)msg_info_temp_ptr->call_num, tel_num_len, (void*)cur_call_num, tel_num_len, tel_num_len);

    /* Set cur call time */
    MMIAPICOM_Wstrncpy(msg_info_temp_ptr->time, cur_time_str.wstr_ptr, cur_time_str.wstr_len);
    // 如果是隐藏号码来电...
    if(0 == tel_num_len)
    {
        MMI_GetLabelTextByLang(TXT_UNKNOW_NUM,  &call_name_str);
    }
    // 如果是未命名的情况，则取号码作为......
    else if(MMIAPIPB_IsUseDefaultName(call_name_str.wstr_ptr)||!MMIAPICOM_FileNameIsValid(&call_name_str))
    {
        MMIAPICOM_StrToWstr(msg_info_temp_ptr->call_num, telname);
        if(MMIAPICOM_GetValidFileName(FALSE, telname, tel_num_len, name, CC_MAX_PBNAME_LEN))
    	{
	        call_name_str.wstr_ptr = name;
	        call_name_str.wstr_len = MMIAPICOM_Wstrlen(name);
		}
		else
		{
			SCI_TRACE_LOW("MMIASP MMIASP_SetCurMsgInfo MMIAPICOM_GetValidFileName failed");
		    return FALSE;
        }
			
    }
    /* Combine file path by something */
    return CombineASPFilePath(applet_ptr,msg_info_temp_ptr,&call_name_str, msg_info_temp_ptr->call_num);
}
/*****************************************************************************/
//  Description : set the settings to nv
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_SetSetting2NV( MMIASP_NV_SETTING_T set_nv)
{
    /* Write data to nv */
    MMINV_WRITE(MMINV_ASP_SETTING, &set_nv);
}

/*****************************************************************************/
//  Description : Get setting param
//  Global resource dependence : none
//  Author: junhu
//  Note:
/*****************************************************************************/
PUBLIC MMIASP_NV_SETTING_T MMIASP_GetSettingNvValue(void)
{    
    /* Get nv value */
    MN_RETURN_RESULT_E nv_result    = MN_RETURN_FAILURE;
    MMIASP_NV_SETTING_T set_nv = {0};
    SCI_MEMSET(&set_nv, 0x00, sizeof(MMIASP_NV_SETTING_T));
    
    /* Read asp setting form nv */
    MMINV_READ(MMINV_ASP_SETTING, &set_nv, nv_result);
    
    /* Read default value when read fail form nv */
    if (MN_RETURN_SUCCESS != nv_result)
    {
        MMIASP_ResetSetting(&set_nv);
    }
    
    return set_nv;
}
/*****************************************************************************/
//   Description : MMIAPIASP_StartApplet
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC MMIASP_APPLET_T* MMIASP_StartApplet(MMIASP_START_APPLET_E start_type)
{
    MMI_APPLET_START_T start ={0};
    MMIASP_START_APPLET_E applet_type = start_type;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    if (PNULL == applet_ptr)
    {
        start.guid = SPRD_ANSMACHINE_APPLET_ID;
        
        start.param_ptr = &applet_type;
        start.size_of_param = sizeof(MMIASP_START_APPLET_E);
        
        /* Start my applet */
        MMK_StartApplet( &start );
        
        /* Get my applet point */
        applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
        if(PNULL != applet_ptr)
        {
            MMIASP_NvInit(applet_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIASP] start asp applet return null!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2247_112_2_18_2_2_40_101,(uint8*)"");
        }
#if 0
        /* Set start type,app or win */
        applet_ptr->start_type = start_type;
#endif
        
        
    }
    
    return applet_ptr;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_CloseAspApplet( void )
{
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    /* Close this Applet when PNULL != applet_ptr*/
    if( PNULL != applet_ptr )
    {
        MMK_CloseApplet(applet_ptr->caf_applet.app_handle);
        
        //SCI_TRACE_LOW:" [MMIASP] MMIASP_CloseAspApplet exit !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2272_112_2_18_2_2_40_102,(uint8*)"");
    }
    
    //MMIAUDIO_SetUpLinkMute(FALSE);
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 以秒为单位
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetWaitTime(void)
{
    uint16 index = 0;
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    index = cur_nv.wait_time;
    return s_answer_wait_time[index];
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetRecordTime(void)
{
    uint32 answer_record_time[ASP_AUDIO_TIME_TYPE_NUM] = {60000, 180000, 300000, 600000};
    uint16 index = 0;
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();

    index = cur_nv.record_time;
    return answer_record_time[index];
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetRecordTime(void)
{
    return GetRecordTime();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_ResetSetting(MMIASP_NV_SETTING_T* nv_set_ptr)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_SDCARD;
    
    if(PNULL == nv_set_ptr)
    {
        return;
    }
    
//    nv_set_ptr->volume              = MMIASP_DEFAULT_VOLUME;
    nv_set_ptr->audio_type          = MMIASP_SETTING_FIX_AUDIO;
    
    nv_set_ptr->wait_time           = MMIASP_WAIT_TIME_10S;
    nv_set_ptr->play_record_time    = MMIASP_WAIT_TIME_10S;
    nv_set_ptr->record_time         = MMIASP_RECORD_TIME_3M;
    nv_set_ptr->speaker_flag        = FALSE;

    dev = MMIAPIFMM_GetValidateDevice(dev);
	nv_set_ptr->record_save_dev = dev;
    
    if(MMI_DEVICE_NUM <= dev)
    {
        nv_set_ptr->record_save_dev     = MMI_DEVICE_SDCARD;
    }
    
	//reset set asp flag to FALSE
    SCI_MEMSET(nv_set_ptr->asp_flag, FALSE, sizeof(nv_set_ptr->asp_flag));
    
	//reset record path to zero.
    SCI_MEMSET(nv_set_ptr->record_path_for_client, 0, sizeof(nv_set_ptr->record_path_for_client));

    MMINV_WRITE(MMINV_ASP_SETTING, nv_set_ptr);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDefaultFixedAnswerRing(uint16* fixid_ptr)
{
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    BOOLEAN is_fixid_ring = FALSE;
    uint16  path_len      = 0;

    if(PNULL == fixid_ptr)
    {
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"MMIASP GetFixedAnswerRing GET NV "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2354_112_2_18_2_2_40_103,(uint8*)"");

    if(MMIASP_SETTING_FIX_AUDIO == cur_nv.audio_type)
    {
        is_fixid_ring = TRUE;
        *fixid_ptr = cur_nv.fixed_ring_id;
    }
    else// 当设置的自定义答录语不存在时....
    {   
        path_len = MMIAPICOM_Wstrlen(cur_nv.record_path_for_client);
        if(!MMIAPIFMM_IsFileExist(cur_nv.record_path_for_client, path_len))
        {
            is_fixid_ring = TRUE;
            *fixid_ptr = 0;
        }
    }

    return is_fixid_ring;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetCustomerAnswerRing(const uint8* call_number_ptr,
                                    BOOLEAN  *is_fixed_id_ptr,
                                    wchar   *path_ptr,
                                    uint32  *path_len_ptr,
                                    uint32  *fixed_id_ptr
                                    )

{
    MMI_PARTY_NUMBER_T    party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MMIPB_BCD_NUMBER_T    pb_bcd = {0};
    BOOLEAN               has_set = FALSE;

    uint8 number_len = strlen((char*)call_number_ptr);

    // 如果是隐藏号码....
    if(0 == number_len)
    {
        return FALSE;
    }

    // Convert str to BCD 
    if(MMIAPICOM_GenPartyNumber((uint8*)call_number_ptr, number_len, &party_num))
    {
        pb_bcd.npi_ton    = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
        pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
        SCI_MEMCPY(pb_bcd.number,  party_num.bcd_num, pb_bcd.number_len );
#ifdef ASP_SUPPORT
        if(MMIAPIPB_GetCustomASPPathByNumber(&pb_bcd, is_fixed_id_ptr, path_ptr, path_len_ptr, fixed_id_ptr))
        {
            has_set = TRUE;

            // 如果不是固定的语音资源...........
            if(!*is_fixed_id_ptr)
            {
                if(!MMIAPIFMM_IsFileExist(path_ptr, *path_len_ptr))
                {
                    has_set = FALSE;
                }
            }
            
        }
        else
        {
            //SCI_TRACE_LOW:"MMIASP GetCustomerAnswerRing has not set"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2420_112_2_18_2_2_41_104,(uint8*)"");
        }
#endif        
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP GetCustomerAnswerRing convert to bcd fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2425_112_2_18_2_2_41_105,(uint8*)"");
    }
    
    return has_set;

}
/*****************************************************************************/
//  Description : record time out will call this fun
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIASP_RecordTimeout(void)
{
    StopRecord();
    /* disconnect call */
    MMIAPICC_ReleaseAspCallReq(MN_CAUSE_NORMAL_CLEARING);
    
}
/*****************************************************************************/
//  Description :  start call answer
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_StartAnswerCall(void)
{
    SetMechineWorkingFlag(TRUE);
    MMIAPIASP_StopWaitTimer();
    
    /* stop ring or vibrate */
    MMIAPICC_StopRingOrVibrate();
    /* receive a MT call */
    //MMIAPICC_ConnectAspCallReq();
    MMIASP_OpenRecordingWin();
    
}

/*****************************************************************************/
//  Description : stop record 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopRecord(void)
{
    MMISRV_HANDLE_T audio_handle = MMIASP_GetRecordHandle();

    if(audio_handle)
    {
    // 取消主机静音
        MMICC_EnableMute(FALSE);
        //取消对方静音
        MMICC_EnableRemoteMute(FALSE);
        MMIASP_StopRecordTimer();
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);//是否需要直接释放，需要同需求确认
        MMIASP_SetRecordHandle(0);
    }
}

/*****************************************************************************/
//  Description :  stop  answer call when call finished
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_StopAnswerCall(void)
{
    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopAnswerCall enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2486_112_2_18_2_2_41_106,(uint8*)"");
    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopAnswerCall woring flag = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2487_112_2_18_2_2_41_107,(uint8*)"d", s_asp_mechine_is_working);
    
    //if(MMIASP_IsMechineWorking())
    
    //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopAnswerCall really enter!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2491_112_2_18_2_2_41_108,(uint8*)"");
    SetMechineWorkingFlag(FALSE);
    MMIAPIASP_StopWaitTimer();
    StopPlayAudio();
    StopRecord();

}
/*****************************************************************************/
//  Description : creat record timer
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateRecordTimer(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    uint32    recordTime = 0;
    
    if(PNULL != applet_ptr)
    {
        if(MMI_NONE_TIMER_ID != applet_ptr->record_timer_id)
        {
            MMK_StopTimer(applet_ptr->record_timer_id);
            applet_ptr->record_timer_id = MMI_NONE_TIMER_ID;
        }
        if(MMI_NONE_TIMER_ID != applet_ptr->count_time_id)
        {
            MMK_StopTimer(applet_ptr->count_time_id);
            applet_ptr->count_time_id = MMI_NONE_TIMER_ID;
        }
        
        recordTime = GetRecordTime();
        if (0 < recordTime)
        {
            applet_ptr->record_timer_id = MMK_CreateWinTimer(MMIASP_RECORDING_WIN_ID, recordTime-500, FALSE);//由于计时从答录开始后进行的，所以减500 ms
            //同时创建一个周期性的1s的timer计时.....
            applet_ptr->count_time_id = MMK_CreateWinTimer(MMIASP_RECORDING_WIN_ID, MMI_500MSECONDS, TRUE);
            MMIASP_UpdateRecordingTime(TRUE);
        }
        
        //SCI_TRACE_LOW:"[MMIASP]: CreateRecordTimer record_time     = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2529_112_2_18_2_2_41_109,(uint8*)"d", recordTime);
        //SCI_TRACE_LOW:"[MMIASP]: CreateRecordTimer record_timer_id = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2530_112_2_18_2_2_41_110,(uint8*)"d",applet_ptr->record_timer_id);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: CreateRecordTimer applet is NULL !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2534_112_2_18_2_2_41_111,(uint8*)"");
    }
    
}
/*****************************************************************************/
//  Description : stop play audio for client
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void StopPlayAudio(void)
{
//用来stop的是答录状态下播放的答录语,这里与手机端播放使用的不是相同的handle
    BOOLEAN stop_result = FALSE;
    BOOLEAN close_result = FALSE;

    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    //SCI_TRACE_LOW:"[MMIASP]: StopPlayAudio enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2549_112_2_18_2_2_41_112,(uint8*)"");
    
    if (PNULL != applet_ptr)
    {
        if (applet_ptr->bkmusic_audio_handle)
        {
            //Stop music
            stop_result = MMISRVAUD_Stop(applet_ptr->bkmusic_audio_handle);
                
            //Close handle
            close_result = MMISRVMGR_Free(applet_ptr->bkmusic_audio_handle);
            //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopAudioForClient MMISRVAUD_Stop result = %d,MMISRVMGR_Free result:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2560_112_2_18_2_2_41_113,(uint8*)"dd",stop_result,close_result);
            
            applet_ptr->bkmusic_audio_handle = 0;        
        }
    }

	//取消对方静音设置
    MMICC_EnableRemoteMute(FALSE);
#ifndef WIN32 // 恢复成正常的设置............
    BKMUSIC_SetGain(ASP_DEFAULT_BKMUSIC_GAIN, ASP_DEFAULT_UPLINK_GAIN, ASP_DEFAULT_DOWNLINK_GAIN);
#endif
}

/*****************************************************************************/
//  Description : 获取存放系统文件的路径以供写
//  Global resource dependence :  none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSysFileFullNameForWrite(
                                         wchar*       full_path_name_ptr,  // out  
                                         uint16*      full_path_len_ptr    // out      
                                         )
{
    BOOLEAN         result          = FALSE;    
    uint32          min_free_size   = MMIASP_SYSTEM_FILE_NEED_SPACE;

    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)
    {        
        result = MMIAPIFMM_GetSysFileFullPathForWriteEx(
                                                    TRUE,
                                                    FALSE,
                                                    MMIASP_SYSFILE_DIR,
                                                    MMIASP_SYSFILE_DIR_LEN,
                                                    MMIASP_SYSFILE_NAME,
                                                    MMIASP_SYSFILE_NAME_LEN,
                                                    min_free_size,
                                                    full_path_name_ptr,
                                                    full_path_len_ptr
                                                    );
    }

    //SCI_TRACE_LOW:"[MMIASP] GetSysFileFullNameForWrite result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2598_112_2_18_2_2_41_114,(uint8*)"d",result);
    return result; 
}
/*****************************************************************************/
//  Description : 获取存放系统文件的路径以供读
//  Global resource dependence :  none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSysFileFullNameForRead(
                                        wchar*       full_path_name_ptr,  // out    
                                        uint16*      full_path_len_ptr    // out    
                                        )
{
    BOOLEAN         result          = FALSE;    

    if (PNULL != full_path_len_ptr && PNULL != full_path_name_ptr)
    {        
        result = MMIAPIFMM_GetSysFileFullPathForReadEx(
                                                    TRUE,
                                                    FALSE,
                                                    MMIASP_SYSFILE_DIR,
                                                    MMIASP_SYSFILE_DIR_LEN,
                                                    MMIASP_SYSFILE_NAME,
                                                    MMIASP_SYSFILE_NAME_LEN,
                                                    full_path_name_ptr,
                                                    full_path_len_ptr
                                                   );
    }

    //SCI_TRACE_LOW:"[MMIASP] GetSysFileFullNameForRead result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2626_112_2_18_2_2_41_115,(uint8*)"d",result);
    return result; 
}
/*****************************************************************************/
//  Description :  
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_HandlePlayAudioEnd(void)
{
    StartRecord();
}
/*****************************************************************************/
//  Description :  答录机是否在工作状态.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_IsMechineWorking(void)
{
    return s_asp_mechine_is_working;
}
/*****************************************************************************/
//  Description :  设置答录机是否在工作状态.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 此函数不开放给外部...
/*****************************************************************************/
LOCAL void SetMechineWorkingFlag(BOOLEAN is_working)
{
    s_asp_mechine_is_working = is_working;
    //SCI_TRACE_LOW:"MMIASP SetMechineWorkingFlag = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2652_112_2_18_2_2_41_116,(uint8*)"d", is_working);
}
/*****************************************************************************/
//  Description :  此函数专供RealseCall时调用.......
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 只在挂断整个电话时调用...
/*****************************************************************************/
PUBLIC void MMIASP_RealseCall(void)
{
    //SCI_TRACE_LOW:"[MMIASP]: MMIAPIASP_RealseCall enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2660_112_2_18_2_2_41_117,(uint8*)"");
    MMIASP_CloseRecordingWin();
    MMIASP_CloseReleasingWin();
    MMIASP_StopAnswerCall();
    MMIASP_SetCallState(MMIASP_MACHINE_CALL_NONE);
    
}
/*****************************************************************************/
//  Description : 设置答录通话的状态.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_SetCallState(MMIASP_MACHINE_CALL_STATE_E call_state)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    //SCI_TRACE_LOW:"MMIASP MMIASP_SetCallState set call_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2673_112_2_18_2_2_41_118,(uint8*)"d", call_state);
    if(PNULL != applet_ptr)
    {
        applet_ptr->call_state = call_state;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP applet is not worked "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2680_112_2_18_2_2_41_119,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : 获得答录通话的状态.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC MMIASP_MACHINE_CALL_STATE_E MMIASP_GetCallState(void)
{
    MMIASP_MACHINE_CALL_STATE_E call_state = MMIASP_MACHINE_CALL_NONE;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    //SCI_TRACE_LOW:"MMIASP MMIASP_GetCallState enterd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2691_112_2_18_2_2_41_120,(uint8*)"");

    if(PNULL != applet_ptr)
    {
        call_state =  applet_ptr->call_state;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP applet is not worked "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2699_112_2_18_2_2_41_121,(uint8*)"");
    }

    return call_state;
}
/*****************************************************************************/
//  Description : 所有盘符是否有足够的剩余空间来录制留言....
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetAllPathEnoughFreeSpace(void)
{
    BOOLEAN  free_enough = TRUE;
    MMIFILE_DEVICE_E suit_disk = MMI_DEVICE_NUM;
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev( MMIASP_GetSettingNvValue().record_save_dev, MMIASP_RECORD_FILE_NEED_SPACE, &suit_disk))
    //if(MMI_DEVICE_NUM == MMIASP_GetRecordFileDev(MMIASP_RECORD_FILE))
    {
        //SCI_TRACE_LOW:"GetAllPathEnoughFreeSpace false"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2715_112_2_18_2_2_41_122,(uint8*)"");
        free_enough = FALSE;
    }
    
    //SCI_TRACE_LOW:"GetAllPathEnoughFreeSpace true"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2719_112_2_18_2_2_41_123,(uint8*)"");
    return free_enough;
}
/*****************************************************************************/
//  Description : 当前盘符是否有足够的剩余空间来录制留言....
//  Global resource dependence : none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetCuurPathEnoughFreeSpace(void)
{
    BOOLEAN  free_enough = TRUE;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;
    
    if(PNULL != s_new_msg_node.record_path)
    {
        file_dev = MMIAPIFMM_GetDeviceTypeByPath(s_new_msg_node.record_path, MMIFILE_DEVICE_LEN);
    }
    //SCI_TRACE_LOW:"[MMIASP]: GetCuurPathEnoughFreeSpace:file_dev = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2735_112_2_18_2_2_41_124,(uint8*)"d", file_dev);

    if(MMI_DEVICE_NUM != file_dev)
    {
        if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, file_dev, MMIASP_RECORD_FILE_NEED_SPACE, PNULL))
        {
            free_enough = FALSE;
        } 
    }
    else
    {
        free_enough = FALSE;
    }
    
    //SCI_TRACE_LOW:"[MMIASP]: GetCuurPathEnoughFreeSpace:free_enough = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2749_112_2_18_2_2_41_125,(uint8*)"d", free_enough);
    return free_enough;
}

/*****************************************************************************/
//  Description : 得到应答的条件,如果没有则返回NONE,表示可以应答....
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_GetAnswerCondition(void)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;
    MMI_STRING_T prompt_str = {0};
    
    // 是否答录机正在工作中，该判断用户有多个来电时的处理情况
    if(MMIASP_IsMechineWorking())
    {
        return FALSE;
    }
    
    // 如果正在通话中则不启动答录机......
    if(MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        return FALSE;
    }
    // 对u盘的判断
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }
    
    // 对存储空间的判断....
    if(!GetAllPathEnoughFreeSpace())
    {
        // error_code = MMIASP_ERROR_NO_FREESPACE;
        // 做相关的提示等.....
        MMI_GetLabelTextByLang(TXT_ASP_NO_FREE_SPACE, &prompt_str);
        MMIPUB_OpenAlertWinByTextPtr(PNULL, &prompt_str, PNULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return FALSE;
    }
    // 对是否有AUDIO目录的判断...
    if(!GetAudioFolder(&file_dev))
    {
        MMI_GetLabelTextByLang(TXT_MMIASP_DIR_IS_FULL, &prompt_str);
        MMIPUB_OpenAlertWinByTextPtr(PNULL, &prompt_str, PNULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return FALSE;
    }

    // 对条目数的判断..........
    if(MMIASP_MAX_MSG_NUM <= MMIASP_GetRecordNum())
    {
        MMIPUB_OpenAlertWarningWin(TXT_MMIASP_MSG_NUM_IS_FULL);
        return FALSE;
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : Callback function 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void Record_CallBack(
                           MMISRVAUD_REPORT_RESULT_E result,
                           DPARAM           param
                           )
{
    //SCI_TRACE_LOW:"[MMIASP]: Record_CallBack:MMISRVAUD_REPORT_RESULT_E = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2815_112_2_18_2_2_41_126,(uint8*)"d", result);
    
    if(!GetCuurPathEnoughFreeSpace())
    {
        MMIPUB_OpenAlertWarningWin(TXT_ASP_NO_FREE_SPACE);
        // 空间不足时挂断电话.....
        // MMIASP_RealseCall();
        MMIAPICC_ReleaseAspCallReq(MN_CAUSE_NORMAL_CLEARING);
    }
    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetListNodeTime(MMIASP_MSG_INFO_T*  node_ptr)

{
    uint32 node_time = 0;
    
    if(node_ptr != PNULL)
    {
        node_time = MMIAPICOM_Tm2Second( (uint32)node_ptr->date_info.sys_time.sec, 
                                    (uint32)node_ptr->date_info.sys_time.min, 
                                    (uint32)node_ptr->date_info.sys_time.hour,
                                    (uint32)node_ptr->date_info.sys_date.mday,
                                    (uint32)node_ptr->date_info.sys_date.mon,
                                    (uint32)node_ptr->date_info.sys_date.year
                                   );
    }
    
    return node_time;
}

/*****************************************************************************/
//  Description : 将当前的已读节点按时间顺序插入到已读链表中去
//  Global resource dependence : none
//  Author:kunliu
//  Note: 
/*****************************************************************************/
LOCAL void InsertReadRecordList(
                                 MMIASP_MSG_INFO_T**  first_ptr, //已读链表中的第一个节点
                                 MMIASP_MSG_INFO_T**  last_ptr,  //已读链表中的最后一个节点
                                 MMIASP_MSG_INFO_T*  insert_ptr    //当前已读节点
                                 )

{
    
    uint32 insert_time = 0;    
    uint32 cur_time = 0;
    MMIASP_MSG_INFO_T* cur_ptr = PNULL;
    MMIASP_MSG_INFO_T* pre_ptr = PNULL;
    
    if((PNULL == *first_ptr) || (PNULL == *last_ptr) || (PNULL == insert_ptr))
    {
        //SCI_TRACE_LOW:"InsertReadRecordList fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2869_112_2_18_2_2_42_127,(uint8*)"");
        return;
    }
    
    cur_ptr = *first_ptr;
    insert_time = GetListNodeTime(insert_ptr);
    cur_time = GetListNodeTime(cur_ptr);
    
    if(insert_time >= cur_time) // 是否大于头节点
    {
        insert_ptr->next_ptr = cur_ptr;
        *first_ptr = insert_ptr;
    }
    else if(*first_ptr == *last_ptr) // 不大于头节点，判断当前链表是否就一个节点
    {
        cur_ptr->next_ptr = insert_ptr;
        insert_ptr->next_ptr = PNULL;
        *last_ptr = insert_ptr;
    }
    else
    {
        pre_ptr = cur_ptr;
        cur_ptr = cur_ptr->next_ptr;
        while(cur_ptr != PNULL)
        {
            cur_time = GetListNodeTime(cur_ptr);
            
            if(insert_time >= cur_time)
            {
                pre_ptr->next_ptr = insert_ptr;
                insert_ptr->next_ptr = cur_ptr;
                return;
            }
            pre_ptr = cur_ptr;
            cur_ptr = cur_ptr->next_ptr;
        }
        pre_ptr->next_ptr = insert_ptr;
        insert_ptr->next_ptr = PNULL;
        *last_ptr = insert_ptr;
    }
    
}




/*****************************************************************************/
//  Description : 排序答录机的链表.
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 将原来按照时间顺序的链表处理成1：按未读已读的优先顺序；
//        2：按照业已存在的时间关系 这样的一个新的链表.....
/*****************************************************************************/
PUBLIC void MMIASP_SortRecordList(void)
{
    MMIASP_APPLET_T*    applet_ptr  = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    MMIASP_MSG_INFO_T*  first_UnreadMsg_ptr = PNULL;  // 第一个未读的节点
    MMIASP_MSG_INFO_T*  last_UnreadMsg_ptr  = PNULL;  // 已处理的最后一个未读的节点
    MMIASP_MSG_INFO_T*  first_ReadMsg_ptr   = PNULL;  // 第一个已读的节点
    MMIASP_MSG_INFO_T*  last_ReadMsg_ptr    = PNULL;  // 已处理的最后一个已读的节点
    MMIASP_MSG_INFO_T*  cur_msg_ptr         = PNULL;  // 当前处理的节点...
    MMIASP_MSG_INFO_T*  pre_msg_ptr         = PNULL;  // 当前处理的上一个节点...    
    MMIASP_MSG_INFO_T*  temp_msg_ptr        = PNULL;  // 做为交换变量

    //SCI_TRACE_LOW:"MMIASP MMIASP_SortRecordList entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2932_112_2_18_2_2_42_128,(uint8*)"");
    
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP no applet!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2936_112_2_18_2_2_42_129,(uint8*)"");
        return;
    }
    cur_msg_ptr = applet_ptr->list_head_ptr;
    if(PNULL == applet_ptr->list_head_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP find null list"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2942_112_2_18_2_2_42_130,(uint8*)"");
        return;
    }
    if(0 == applet_ptr->new_msg_num)
    {
        //SCI_TRACE_LOW:"MMIASP no new node"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_2947_112_2_18_2_2_42_131,(uint8*)"");
        return;
    }
    // 首先处理第一个节点.......
    if(cur_msg_ptr->read_flag)
    {
        first_ReadMsg_ptr = cur_msg_ptr;
        last_ReadMsg_ptr  = cur_msg_ptr;
    }
    else
    {
        first_UnreadMsg_ptr = cur_msg_ptr;
        last_UnreadMsg_ptr  = cur_msg_ptr;
    }
    
    pre_msg_ptr = cur_msg_ptr;
    cur_msg_ptr = cur_msg_ptr->next_ptr;
    pre_msg_ptr->next_ptr = PNULL;

    while (PNULL != cur_msg_ptr)
    {
        //切断当前节点和下一个节点之间的联系.....        
        temp_msg_ptr = cur_msg_ptr->next_ptr;
        cur_msg_ptr->next_ptr = PNULL;
        
        if(cur_msg_ptr->read_flag)
        {
            if(PNULL == first_ReadMsg_ptr)
            {
                first_ReadMsg_ptr = cur_msg_ptr;                 
                last_ReadMsg_ptr = cur_msg_ptr;
            }
            else if(PNULL != last_ReadMsg_ptr)
            {
                //已读的就按时间重新插入
                InsertReadRecordList(&first_ReadMsg_ptr, &last_ReadMsg_ptr, cur_msg_ptr);
            }// else 会在后面给last_ReadMsg_ptr赋上当前节点的值.....
        }
        else
        {
            if(PNULL == first_UnreadMsg_ptr)
            {
                first_UnreadMsg_ptr = cur_msg_ptr;                
                last_UnreadMsg_ptr = cur_msg_ptr;
            }
            else if(PNULL != last_UnreadMsg_ptr)
            {
                last_UnreadMsg_ptr->next_ptr = cur_msg_ptr;                
                last_UnreadMsg_ptr = cur_msg_ptr;
            }// else 会在后面给last_UnreadMsg_ptrr赋上当前节点的值.....
        }
        
        // 指向下一个节点......
        cur_msg_ptr = temp_msg_ptr;
    }

    //排好以后，
    if(PNULL != last_ReadMsg_ptr)
    {
        last_ReadMsg_ptr->next_ptr = PNULL;
    }
    // 把已读的链表接在未读的链表的后面...
    if(PNULL != last_UnreadMsg_ptr)
    {
        last_UnreadMsg_ptr->next_ptr = first_ReadMsg_ptr;
    }
    // 把第一个未读的节点作为整个链表的第一个节点...
    if(PNULL != first_UnreadMsg_ptr)
    {
        applet_ptr->list_head_ptr = first_UnreadMsg_ptr;
    }
    // 再写进去nv中....
    MMIASP_WriteSysFile();
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetNewRecordNum(void)
{
    uint32   read_size     = 0;
    uint32   list_num      = 0;
    uint32   new_item_num  = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;  
    
    wchar  file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    if(!GetSysFileFullNameForRead(file_name, &path_len))
    {
        return 0;
    }
    
    
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    // 在这个函数里面对file_handle有判断...............
    if(MMIASP_CheckVersionNumber(file_handle, s_answer_machine_version, MMIAPICOM_Wstrlen(s_answer_machine_version)))
    {
        
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &list_num, sizeof(uint32), &read_size, PNULL))
        {
            MMIAPIFMM_CloseFile( file_handle );
            return 0;
        }
        
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &new_item_num, sizeof(uint32), &read_size, PNULL))
        {
            MMIAPIFMM_CloseFile( file_handle );
        }
    }
    
    MMIAPIFMM_CloseFile( file_handle );
    
    return new_item_num;
}
/*****************************************************************************/
//  Description : 获取最新的新留言....
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_GetLastestUnreadRecord(
                                             MMIASP_MSG_INFO_T* msg_info_ptr, // out
                                             uint16*            node_index_ptr// out
                                             )
{
    MMIASP_MSG_INFO_T* cur_node_ptr = PNULL; 
    uint16             node_index   = 0;
    if(PNULL == msg_info_ptr && PNULL == node_index_ptr)
    {
        return FALSE;
    }
    
    if(!ReadASPSysFile())
    {
        return FALSE;
    }
    
    cur_node_ptr = s_msg_list_header_ptr;
    while(PNULL != cur_node_ptr)
    {
        if(!cur_node_ptr->read_flag)
        {
            break;
        }
        node_index++;
        cur_node_ptr = cur_node_ptr->next_ptr ;
    }
    
    if(PNULL != msg_info_ptr)
    {
        //仅填充msg_info_ptr
        if(PNULL != cur_node_ptr)
        {
            SCI_MEMCPY(msg_info_ptr, cur_node_ptr, sizeof(MMIASP_MSG_INFO_T));
        }
        else
        {
            MMIASP_MsgInfoListFree(s_msg_list_header_ptr);
			s_msg_list_header_ptr = PNULL;
            return FALSE;
        }
    }
    
    if(PNULL != node_index_ptr)
    {
        //仅填充node_index
        *node_index_ptr = node_index;
    }
    
     MMIASP_MsgInfoListFree(s_msg_list_header_ptr);
     s_msg_list_header_ptr = PNULL;
    return TRUE;
}
/*****************************************************************************/
//  Description : 没有起APPLET就能得到答录记录的条目数....
//  Global resource dependence : none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetRecordNum(void)
{
    uint32   read_size     = 0;
    uint32   record_num    = 0;
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;  
    
    wchar  file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    if(!GetSysFileFullNameForRead(file_name, &path_len))
    {
        return 0;
    }
    
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    // 在这个函数里面对file_handle有判断...............
    if(MMIASP_CheckVersionNumber(file_handle, s_answer_machine_version, MMIAPICOM_Wstrlen(s_answer_machine_version)))
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &record_num, sizeof(uint32), &read_size, PNULL))
        {
            MMIAPIFMM_CloseFile( file_handle );
            return 0;
        }
    }
    
    MMIAPIFMM_CloseFile( file_handle );
    
    return record_num;
}
/*****************************************************************************/
//  Description : 开始播报答录语....
//  Global resource dependence : none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_StartPlayAnswerMusic(void)
{
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    BOOLEAN         is_ok       = TRUE;
    //SCI_TRACE_MID:"[MMIASP]: MMIASP_StartPlayAnswerMusic enter !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_3160_112_2_18_2_2_42_132,(uint8*)"");
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:" [MMIASP] MMIASP_StartAnswerCall Not applet !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_3163_112_2_18_2_2_42_133,(uint8*)"");
        return ;
    }

    
    if(MMIASP_ReadSysFileWithApplet(applet_ptr))
    {
        SCI_MEMSET(&s_new_msg_node, 0, sizeof(MMIASP_MSG_INFO_T));
        if(MMIASP_SetCurMsgInfo(applet_ptr, &s_new_msg_node))
        {    
            MMIASP_PlayAudioInCall(applet_ptr, &s_new_msg_node);
        }
        else
        {
            is_ok = FALSE;
        }
    }
    else
    {
        is_ok = FALSE;
    }

    if(!is_ok)
    {
        if(MMIAPIUDISK_UdiskIsRun())
        {           
            MMIPUB_OpenAlertFailWin(TXT_ASP_UDISK_USING);
        }
        else if(!GetAllPathEnoughFreeSpace())
        {
            MMIPUB_OpenAlertFailWin(TXT_ASP_NO_SPACE_GOTO_ANSWER_FAIL);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_MMIASP_GOTO_ANSWER_FAIL);
        }
        // MMIASP_RealseCall();
        MMIAPICC_ReleaseAspCallReq(MN_CAUSE_NORMAL_CLEARING);
    }
}
/*****************************************************************************/
//  Description : ....
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 该函数不仅在U盘或SD卡检查它们的剩余空间是否足够，而且检查其是否存在MMIMULTIM_DIR_MUSIC
//        如不存在，则创建之，不成功则再去搜索第二个盘....
/*****************************************************************************/
LOCAL BOOLEAN GetAudioFolder(MMIFILE_DEVICE_E* dev_ptr/*out*/)
{
    wchar            dir_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16           dir_path_len = MMIFILE_FILE_NAME_MAX_LEN;
    //const wchar*     file_dev_ptr = PNULL;
    //uint16           file_dev_len = 0;
    //MMIFILE_ERROR_E  error = SFS_NO_ERROR;
    MMIFILE_DEVICE_E dev = MMIASP_GetSettingNvValue().record_save_dev;
    BOOLEAN          get_result = FALSE;
    //MMIFILE_DEVICE_E  file_type = MMI_DEVICE_NUM;
    //MMIFILE_DEVICE_E  file_dev  = MMI_DEVICE_UDISK;
    
    if(PNULL == dev_ptr)
    {
        return FALSE;
    }

    //file_type = MMIFILE_GetFileDeviceType(dev);
    if(SFS_ERROR_NONE == MMIAPIFMM_GetSuitableFileDev(dev, MMIASP_RECORD_FILE_NEED_SPACE, dev_ptr))
    {
    
        dir_path_len = MMIAPIFMM_CombineFullPathEx(dir_path, dir_path_len, *dev_ptr, MMIMULTIM_DIR_MUSIC, PNULL);
        if (!MMIAPIFMM_IsFolderExist(dir_path, dir_path_len))
        {
            if(SFS_NO_ERROR == MMIAPIFMM_CreateDirectory(dir_path))
            {
                get_result = TRUE;
            }
        }
        else
        {
            get_result = TRUE;
        }
        
    }
    return  get_result; 
}

/*****************************************************************************/
//  Description : creat progress refresh timer
//  Global resource dependence : none
//  Author: koki gao
//  Note:
/*****************************************************************************/
PUBLIC void CreateProgressTimer(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    if(PNULL != applet_ptr)
    {
        if(MMI_NONE_TIMER_ID != applet_ptr->progress_time_id)
        {
            MMK_StopTimer(applet_ptr->progress_time_id);
            applet_ptr->progress_time_id = MMI_NONE_TIMER_ID;
        }
        applet_ptr->progress_time_id = MMK_CreateWinTimer(MMIASP_MAIN_PLAY_WIN_ID, MMI_500MSECONDS, FALSE);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: CreateRecordTimer applet is NULL !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_3265_112_2_18_2_2_42_134,(uint8*)"");
    } 
}
/*****************************************************************************/
//  Description : stop progress refresh timer
//  Global resource dependence : none
//  Author: koki gao
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_StopProgressTimer(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    if( PNULL != applet_ptr)
    {
        if(MMI_NONE_TIMER_ID != applet_ptr->progress_time_id)
        {
            MMK_StopTimer(applet_ptr->progress_time_id);
            applet_ptr->progress_time_id = MMI_NONE_TIMER_ID;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: MMIASP_StopRecordTimer: no applet!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_CORTROL_LAYER_3285_112_2_18_2_2_42_135,(uint8*)"");
    }
}

