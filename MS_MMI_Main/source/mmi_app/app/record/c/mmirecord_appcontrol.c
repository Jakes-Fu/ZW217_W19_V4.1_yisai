/*****************************************************************************
** File Name:      mmirecord_appcontrol.c                                               *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMIRECORD_APPCONTROL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "sci_api.h"

#include "mmirecord_appcontrol.h"
#include "mmirecord_id.h"
#include "mmirecord_main.h"

#include "mmi_applet_table.h"

#include "mmi_appmsg.h"
#include "mmicom_time.h"

#include "mmisrvfmm_export.h"
#include "mmisrvaud_api.h"

#include "mmieng_export.h"
#include "mmiset_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DIFFERENCE_SIZE_FOR_RECORDING  (1024)
#define RECORD_FILES_FOLDER        "D:\\audio"
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : NotifyCallback
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void NotifyCallback(RECORD_SRV_HANDLE record_srv_handle,
                          MMIRECORD_SRV_SUPPORT_TYPE_E support_type,
                          MMIRECORD_SRV_RESULT_E result);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : NotifyCallback
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void NotifyCallback(RECORD_SRV_HANDLE record_srv_handle,
                          MMIRECORD_SRV_SUPPORT_TYPE_E support_type,
                          MMIRECORD_SRV_RESULT_E result)
{
    uint32 i = 0;
    BOOLEAN is_succeed = FALSE;
    MMIRECORD_RESULT_E record_result  = MMIRECORD_RESULT_SUCCESS;
    MMK_GetAppletPtr(SPRD_RECORD_APPLET_ID);

    SCI_TRACE_LOW("[RECORD] NotifyCallback record_srv_handle = %d, s_record_info.record_srv_handle = %d, result = %d, support_type = %d", record_srv_handle, MMIRECORD_GetRecordHandle(), result, support_type);

    if(record_srv_handle == MMIRECORD_GetRecordHandle())
    {

        if(MMIRECORD_SRV_SUPPORT_RECORD == support_type)
        {
            if(MMIRECORD_SRV_RESULT_ERROR == result)
            {
                record_result = MMIRECORD_RESULT_ERROR;
            }
            else if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED , MMIRECORD_GetCurrentDev(), MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
            {
                record_result = MMIRECORD_RESULT_NO_SPACE;
            }

            MMIRECORD_SetRecordState(MMIRECORD_STATE_STOPPED);
            MMK_PostMsg(MMIRECORD_GetRecordWin(), MSG_RECORD_CALL_BACK, &record_result, sizeof(MMIRECORD_RESULT_E));

        }
        else if(MMIRECORD_SRV_SUPPORT_PLAY == support_type)
        {
            if(MMIRECORD_SRV_RESULT_SUCCESS == result)
            {
                is_succeed = TRUE;
            }

            MMIRECORD_SetRecordState(MMIRECORD_STATE_STOPPED);
            MMK_PostMsg(MMIRECORD_GetRecordWin(), MSG_RECORD_PLAY_CALL_BACK, &is_succeed, sizeof(BOOLEAN));
        }
        else if(MMIRECORD_SRV_SUPPORT_IQ_RECORD == support_type)
        {
            for(i = MMI_DEVICE_NUM - 1; i >= MMI_DEVICE_UDISK; i--)
            {
                if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, i, MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
                {
                    is_succeed = TRUE;
                    break;
                }
            }

            MMIRECORD_SetRecordState(MMIRECORD_STATE_STOPPED);

            if(!is_succeed)
            {
                if(MMIRECORD_SRV_RESULT_SUCCESS == result)
                {
                    is_succeed = TRUE;
                }

                MMIAPIENG_OpenHandleIQDataQueryWin(is_succeed);
            }
        }
    }

}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_Constructor(void)
{
    RECORD_SRV_HANDLE  record_srv_handle = 0;

    if(0 == MMIRECORD_GetRecordHandle())
    {

        record_srv_handle = MMIRECORDSRV_RequestRecordHandle(NotifyCallback);

        if(PNULL == record_srv_handle)
        {
            return FALSE;
        }

        MMIRECORD_SetRecordHandle(record_srv_handle);
    }
    /*Bug 129892 :进入record前应关闭按键音*/
    MMIAPIENVSET_CloseActModeKeyRingSet();
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Destructor(void)
{
    RECORD_SRV_HANDLE record_srv_handle = MMIRECORD_GetRecordHandle();

    if(0 != record_srv_handle)
    {
        MMIRECORDSRV_FreeRecordHandle(record_srv_handle);
        MMIRECORD_SetRecordHandle(0);
    }
    /*Bug 129892 :退出RECORD恢复按键音*/
    MMIAPIENVSET_ResetActModeKeyRingSet();
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORD_GetRecordFileName(void)
{
    return MMIRECORDSRV_GetRecordFileName(MMIRECORD_GetRecordHandle());
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORD_GetRecordFilePath(void)
{
    return MMIRECORDSRV_GetRecordFilePath(MMIRECORD_GetRecordHandle());
}


/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_DeleteRecordFile(void)
{
    MMIRECORD_SRV_RESULT_E   result = MMIRECORD_SRV_RESULT_SUCCESS;

    result = MMIRECORDSRV_DeleteRecordFile(MMIRECORD_GetRecordHandle(), TRUE);


    SCI_TRACE_LOW("[RECORD] MMIRECORD_DeleteRecordFile result = %d", result);

    if(MMIRECORD_SRV_RESULT_SUCCESS == result)
    {
        MMK_PostMsg(MMIRECORD_GetRecordWin(), MSG_RECORD_FILE_DELETED, PNULL, PNULL);
        MMIRECORD_SetRecordState(MMIRECORD_STATE_START);//fix bug852353 
        return MMIRECORD_RESULT_SUCCESS;
    }
    else if(MMIRECORD_SRV_RESULT_NO_EXIST == result)
    {
        MMK_PostMsg(MMIRECORD_GetRecordWin(), MSG_RECORD_FILE_DELETED, PNULL, PNULL);
        MMIRECORD_SetRecordState(MMIRECORD_STATE_START);//fix bug852353 
        return MMIRECORD_RESULT_NO_EXIST;
    }
    else
    {
        return MMIRECORD_RESULT_ERROR;
    }
}

/*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_StopRecord(void)
{
    MMIRECORD_SRV_RESULT_E  srv_result = MMIRECORD_SRV_RESULT_SUCCESS;
    MMIRECORD_RESULT_E    result = MMIRECORD_RESULT_ERROR;


    srv_result = MMIRECORDSRV_StopRecord(MMIRECORD_GetRecordHandle());

    if(MMIRECORD_SRV_RESULT_SUCCESS == srv_result)
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_STOPPED);
        MMIAPIFMM_UpdateCurrentFileLIst();
        result =  MMIRECORD_RESULT_SUCCESS;
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_StopRecord record_result = %d, state = %d", result, MMIRECORD_GetRecordState());
    return result;
}


/*****************************************************************************/
//  Description : 暂停录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_PauseRecord(void)
{
    BOOLEAN result = FALSE;
    #ifdef WIN32
        result = TRUE;
    #else
        result = MMIRECORDSRV_PauseRecord(MMIRECORD_GetRecordHandle());
    #endif

    if(result)
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_RECORD_PAUSED);
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_PauseRecord record_result = %d, state = %d", result, MMIRECORD_GetRecordState());
    return result;
}

/*****************************************************************************/
//  Description : 继续录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_ResumeRecord(void)
{
    BOOLEAN result = FALSE;
    #ifdef WIN32
        result = TRUE;
    #else
        result = MMIRECORDSRV_ResumeRecord(MMIRECORD_GetRecordHandle());
    #endif

    if(result)
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_RECORDING);
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_ResumeRecord record_result = %d, state = %d", result, MMIRECORD_GetRecordState());
    return result;
}


/*****************************************************************************/
//  Description : 暂停播放录音
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_PausePlay(void)
{
    BOOLEAN result = FALSE;
    result = MMIRECORDSRV_PausePlay(MMIRECORD_GetRecordHandle());

    if(result)
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_PLAY_PAUSED);
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_PausePlay record_result = %d, state = %d", result, MMIRECORD_GetRecordState());
    return result;
}

/*****************************************************************************/
//  Description : 继续播放录音
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_ResumePlay(void)
{
    BOOLEAN result = FALSE;
    result = MMIRECORDSRV_ResumePlay(MMIRECORD_GetRecordHandle());

    if(result)
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_PLAYING);
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_ResumePlay record_result = %d, state = %d", result, MMIRECORD_GetRecordState());
    return result;
}

/*****************************************************************************/
//  Description : 停止播放录音
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_StopPlay(void)
{
    BOOLEAN result = FALSE;
    result = MMIRECORDSRV_StopPlay(MMIRECORD_GetRecordHandle());

    if(result)
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_STOPPED);
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_StopPlay record_result = %d, state = %d", result, MMIRECORD_GetRecordState());
    return result;
}

/*****************************************************************************/
//  Description : play current record
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_PlayCurrent(void)
{
    MMIRECORD_SRV_RESULT_E srv_result = MMIRECORD_SRV_RESULT_SUCCESS;
    MMIRECORD_RESULT_E     record_result = MMIRECORD_RESULT_ERROR;

    if(!MMIRECORDSRV_IsRecordFileExist(MMIRECORD_GetRecordHandle()))
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_START);//bug 859080 set record status to start
        record_result = MMIRECORD_RESULT_NO_EXIST;
    }
    else
    {
        srv_result = MMIRECORDSRV_Play(MMIRECORD_GetRecordHandle(), MMIAPISET_GetMultimVolume());

        if(MMIRECORD_SRV_RESULT_SUCCESS == srv_result)
        {
            MMIRECORD_SetRecordState(MMIRECORD_STATE_PLAYING);
            record_result = MMIRECORD_RESULT_SUCCESS;
        }
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_PlayCurrent record_result = %d, state = %d", record_result, MMIRECORD_GetRecordState());
    return record_result;

}

/*****************************************************************************/
//  Description : MMIRECORD_PlaySeek
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_PlaySeek(uint16 percent)
{
    MMISRVAUD_PLAY_INFO_T play_info = {0};
    uint32 seek_offset = 0;

    if(MMIRECORDSRV_GetPlayingInfo(MMIRECORD_GetRecordHandle(), &play_info))
    {
        seek_offset = (uint32)((play_info.total_data_length * percent) / 100);
        SCI_TRACE_LOW("[RECORD] MMIRECORD_PlaySeek seek_offset = %d, percent = %d, data_len = %d", seek_offset, percent, play_info.total_data_length);

        if(MMIRECORDSRV_Seek(MMIRECORD_GetRecordHandle(), seek_offset))
        {
            return TRUE;
        }

    }

    return FALSE;
}

/*****************************************************************************/
//  Description : ConvertWorkFMTToSRVFMT
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RECORD_FMT_E MMIRECORD_ConvertWorkFMTToSRVFMT(MMIRECORD_WORK_FORMAT_E fmt)
{
    MMISRVAUD_RECORD_FMT_E srv_fmt = MMISRVAUD_RECORD_FMT_MP3;

    switch(fmt)
    {
    case MMIRECORD_WORK_FORMAT_MP3:
        srv_fmt = MMISRVAUD_RECORD_FMT_MP3;
        break;

    case MMIRECORD_WORK_FORMAT_AMR:
        srv_fmt = MMISRVAUD_RECORD_FMT_AMR;
        break;

    default:
        srv_fmt = MMISRVAUD_RECORD_FMT_ADPCM;
        break;
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_ConvertWorkFMTToSRVFMT fmt = %d, srv_fmt = %d", fmt, srv_fmt);
    return srv_fmt;
}

/*****************************************************************************/
//  Description : 开始录音（通常情况）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIRECORD_StartRecordNormal(BOOLEAN is_reRecord,
        MMIRECORD_WORK_FORMAT_E fmt,
        uint32 *record_id_ptr,
        MMIFILE_DEVICE_E record_dev)
{
    MMIFILE_ERROR_E error_code = SFS_ERROR_NONE;
    MMIRECORD_SRV_RECORD_PARAM_T record_param = {0};
    MMIRECORD_SRV_RESULT_E  record_result = MMIRECORD_SRV_RESULT_SUCCESS;

    if(MMIRECORD_STATE_START != MMIRECORD_GetRecordState()
            && MMIRECORD_STATE_STOPPED != MMIRECORD_GetRecordState())
    {
        error_code = SFS_ERROR_ABORT;
        return error_code;
    }



    record_param.is_create_file_name = !is_reRecord;
    record_param.prefix_ptr = PNULL;
    record_param.record_dev_ptr = &record_dev;
    record_param.record_file_id_ptr = record_id_ptr;
    record_param.record_name_ptr = PNULL;
    record_param.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
    record_param.fmt = MMIRECORD_ConvertWorkFMTToSRVFMT(fmt);
    record_param.record_param_ptr = PNULL;
    record_param.volume = MMIAPISET_GetMultimVolume();
    record_result = MMIRECORDSRV_StartRecord(MMIRECORD_GetRecordHandle(), record_param);

    if(MMIRECORD_SRV_RESULT_SUCCESS != record_result)
    {
        MMIRECORD_SetRecordState(MMIRECORD_STATE_START);

        if(MMIRECORD_SRV_RESULT_NO_SPACE == record_result)
        {
            error_code = SFS_ERROR_NO_SPACE;
        }
        else
        {
            error_code = SFS_ERROR_ABORT;
        }
    }
    else
    {
        MMIRECORD_SetRecordId(*record_id_ptr);
        MMIRECORD_SetRecordState(MMIRECORD_STATE_RECORDING);
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_StartRecordNormal record_dev = %d error_code = %d, record_state = %d", record_dev, error_code, MMIRECORD_GetRecordState());
    return error_code;
}

/*****************************************************************************/
//  Description : MMIRECORD_GetCurrentDev
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIRECORD_GetCurrentDev(void)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;
    wchar *file_path_ptr = MMIRECORDSRV_GetRecordFilePath(MMIRECORD_GetRecordHandle());

    if(PNULL != file_path_ptr)
    {
        file_dev = MMIAPIFMM_GetDeviceTypeByPath(file_path_ptr, MMIFILE_DEVICE_LEN);
    }

    if(MMI_DEVICE_NUM <= file_dev)
    {
        file_dev = MMIRECORD_GetRecordStorage();
    }

    SCI_TRACE_LOW("[RECORD] GetCurrentDev file_dev = %d", file_dev);
    return file_dev;
}


/*****************************************************************************/
//  Description : MMIRECORD_IsReachMax
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_IsReachMax(uint32 max_file_time, uint32 max_file_size)
{
    BOOLEAN result = FALSE;

    if(max_file_time != 0)
    {
        MMISRVAUD_RECORD_INFO_T  record_info = {0};
        MMIRECORDSRV_GetRecordInfo(MMIRECORD_GetRecordHandle(), &record_info);

        if(record_info.total_time >= max_file_time - 300)
        {
            result = TRUE;
        }
    }

    if(max_file_size != 0)
    {
        MMISRVAUD_RECORD_SIZE_T  record_info = {0};
        MMIRECORDSRV_GetRecordSize(MMIRECORD_GetRecordHandle(), &record_info);

        if(record_info.total_size >= max_file_size - DIFFERENCE_SIZE_FOR_RECORDING)
        {
            result = TRUE;
        }
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_IsReachMax max_file_time = %d, max_file_size = %d, result = %d", max_file_time, max_file_size, result);
    return result;
}

/*****************************************************************************/
//  Description : already starting recording
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_IsRecord(void)
{
    SCI_TRACE_LOW("[RECORD] MMIRECORD_IsRecord state = %d", MMIRECORD_GetRecordState());

    if(MMIRECORD_STATE_RECORDING == MMIRECORD_GetRecordState()
            || MMIRECORD_STATE_RECORD_PAUSED == MMIRECORD_GetRecordState())
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordFileTotalTime
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIRECORD_GetRecordFileTotalTime(void)
{
    MMISRVAUD_CONTENT_INFO_T audio_file_info = {0};
    uint32   total_time = 0;

    if(MMIRECORDSRV_GetContentInfo(MMIRECORD_GetRecordHandle(), &audio_file_info))
    {
        total_time =  audio_file_info.total_time;
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_GetRecordFileTotalTime total_time = %d", total_time);
    return total_time;
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordTime
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIRECORD_GetRecordSecondTime(void)
{
    MMISRVAUD_RECORD_INFO_T record_info = {0};
    MMISRVAUD_PLAY_INFO_T   play_info   = {0};
    uint32                  cur_time = 0;

    switch(MMIRECORD_GetRecordState())
    {
    case MMIRECORD_STATE_RECORD_PAUSED:
    case MMIRECORD_STATE_RECORDING:
        MMIRECORDSRV_GetRecordInfo(MMIRECORD_GetRecordHandle(), &record_info);
        cur_time = MMIAPICOM_GetSecondFromMillisecond(record_info.total_time);
        break;

    case MMIRECORD_STATE_PLAY_PAUSED:
    case MMIRECORD_STATE_PLAYING:
        MMIRECORDSRV_GetPlayingInfo(MMIRECORD_GetRecordHandle(), &play_info);
        cur_time = play_info.cur_time;
        break;

    default:
#ifdef MMI_GUI_STYLE_TYPICAL
        cur_time = MMIRECORD_GetRecordFileTotalTime();
#else
        cur_time = 0;
#endif
        break;
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_GetRecordSecondTime cur_time = %d", cur_time);
    return cur_time;
}

/*****************************************************************************/
//  Description : calculate time of MMIRECORD_TIME_T
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_TIME_T MMIRECORD_GetRecordTime(void)
{
    uint32 cur_time = MMIRECORD_GetRecordSecondTime();
    uint32  cur_hours = (cur_time / 3600);
    uint32  cur_minutes = ((cur_time % 3600) / 60);
    uint32  cur_seconds = ((cur_time % 3600) % 60);
    MMIRECORD_TIME_T  record_time = {0};

    SCI_TRACE_LOW("[RECORD] MMIRECORD_GetRecordTime cur_time = %d", cur_time);

    record_time.second_low = (uint8)(cur_seconds % 10);
    record_time.second_high = (uint8)(cur_seconds / 10);
    record_time.minute_low = (uint8)(cur_minutes % 10);
    record_time.minute_high = (uint8)(cur_minutes / 10);
    record_time.hour_low = (uint8)(cur_hours % 10);
    record_time.hour_high = (uint8)(cur_hours / 10);
    return record_time;
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordDB
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIRECORD_GetRecordDB(void)
{
    return MMIRECORDSRV_GetRecordDB(MMIRECORD_GetRecordHandle());
}

/*****************************************************************************/
//  Description : MMIRECORD_SetVolume
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_SetVolume(uint32 cur_value)
{
    return MMIRECORDSRV_SetVolume(MMIRECORD_GetRecordHandle(), cur_value);
}

/*****************************************************************************/
//  Description : MMIRECORD_IsSupport
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_E fmt)
{
    MMISRVAUD_RECORD_FMT_E  srv_fmt = MMIRECORD_ConvertWorkFMTToSRVFMT(fmt);

    return MMIRECORDSRV_QueryEncoderCapability(srv_fmt);
}


/*****************************************************************************/
//  Description : MMIRECORD_RequestVirtualHandle
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_RequestVirtualHandle(void)
{
#ifdef BT_DIALER_SUPPORT
    if(PNULL != MMISRVAUD_ReqVirtualHandle("RECORDER", MMISRVAUD_PRI_NORMAL_N10))
#else
    if(PNULL != MMISRVAUD_ReqVirtualHandle("RECORDER", MMISRVAUD_PRI_NORMAL))
#endif	
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : MMIRECORD_FreeVirtualHandle
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_FreeVirtualHandle(void)
{
    MMISRVAUD_FreeVirtualHandle("RECORDER");
}

/*****************************************************************************/
//  Description : record do factory reset
//  Parameter: [In]  None
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_FactoryReset(void)
{
    wchar record_full_path [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 record_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMI_STRNTOWSTR(record_full_path,MMIFILE_FULL_PATH_MAX_LEN, RECORD_FILES_FOLDER,SCI_STRLEN(RECORD_FILES_FOLDER),SCI_STRLEN(RECORD_FILES_FOLDER));
    record_full_path_len = MMIAPICOM_Wstrlen(record_full_path);

    //check 文件是否存在
    if(MMIAPIFMM_IsFolderExist(record_full_path,record_full_path_len ))
    {
        SCI_TRACE_LOW("[RECORD]MMIRECORD_FactoryReset dele file");
        MMIFILE_DeleteOpen(record_full_path, record_full_path_len);
    }
}

