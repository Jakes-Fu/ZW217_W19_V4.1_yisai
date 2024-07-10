/*****************************************************************************
** File Name:      mmirecord_main.c                                               *
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

#define _MMIRECORD_MAIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_msg.h"

#include "mmi_modu_main.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicom_panel.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"

#include "mmirecord_id.h"
#include "mmirecord_export.h"
#include "mmirecord_setting.h"
#include "mmirecord_appcontrol.h"
#include "mmirecord_wintablel.h"
#include "mmirecord_main.h"

#include "mmisrvfmm_export.h"

#include "cafapplet_gen.h"
#include "cafshell.h"
#include "mmi_applet_table.h"

#include "mmialarm_export.h"
#include "mmicountedtime_export.h"
#include "mmimms_export.h"
#include "mmibt_export.h"
#include "mmiset_export.h"
#include "mmieng_export.h"
#include "mmisd_export.h"
#include "mmicc_export.h"
#include "mmiudisk_export.h"
#include "mmifmm_export.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    RECORD_SRV_HANDLE  record_srv_handle;
    MMIRECORD_STATE_E  record_state;
    MMI_HANDLE_T       win_handle;
    MMIRECORD_WORK_CALLBACK callback;
} MMIRECORD_INFO_T;

typedef struct
{
    CAF_APPLET_T caf_applet;
    MMIRECORD_INFO_T   record_info;
} MMIRECORD_APPLET_T;  //vt applet定义
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
//  Description : SetVolumeCallback
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);
/*****************************************************************************/
//  Description : the process message function of the ebook
//  Global resource dependence :
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  RecordApplet_HandleEvent(
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : 判断录音模块是否允许启动
//  Global resource dependence :
//      Author:
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  CheckRecordIsValidRun(void);



//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmirecord_applet_array[] =
{
    {
        GUID_NAME_DEF(SPRD_RECORD_APPLET_ID), sizeof(MMIRECORD_APPLET_T), RecordApplet_HandleEvent ,
        IMAGE_SECMENU_ICON_TOOL_RECORD, TXT_RECORDER
    },
};

LOCAL const CAF_STATIC_APPLET_INFO_T s_mmirecord_applet_info =
{
    (CAF_STATIC_APPLET_T*)s_mmirecord_applet_array,
    ARR_SIZE(s_mmirecord_applet_array)
};
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the ebook
//  Global resource dependence :
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  RecordApplet_HandleEvent(
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIRECORD_APPLET_T* applet_ptr = (MMIRECORD_APPLET_T*)iapplet_ptr;

    switch(msg_id)
    {
    case MSG_APPLET_START:
        if(PNULL != applet_ptr)
        {
            applet_ptr->record_info.win_handle =  MMIRECORD_OpenRecordMainWin(param);

            if(PNULL != param)
            {
                MMIRECORD_WORK_PARAM_T *work_param_ptr = (MMIRECORD_WORK_PARAM_T *)param;
                applet_ptr->record_info.callback = work_param_ptr->work_callback;
            }

        }

        break;

    case MSG_APPLET_STOP:
        if(!MMIAPIALM_CheckAndStartDelayedAlarm())
        {
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//弹出延迟的倒计时到提示
        }

        break;

    case MSG_APPLET_SUSPEND:
        break;

    case MSG_APPLET_RESUME:
        break;

    case MSG_APPLET_SWITCH:
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : register RECORD module applet info
//  Global resource dependence :
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_RECORD, &s_mmirecord_applet_info);
}

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_SendFileViaBT(wchar       *record_file_path_ptr,
                                       uint16 record_file_path_len)
{
    uint32      send_file_size = 0;
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};

    if(PNULL == record_file_path_ptr
            || 0 == record_file_path_len)
    {
        return FALSE;
    }

    //get file size info
    MMIAPIFMM_GetFileInfo(record_file_path_ptr, record_file_path_len, &send_file_size, NULL, NULL);
    MMIAPICOM_Wstrncpy(send_file_info.filepath_name, record_file_path_ptr, record_file_path_len);
    send_file_info.filepath_len = record_file_path_len;
    send_file_info.file_size = send_file_size;
    send_file_info.is_temp_file = FALSE;
    MMIAPIBT_SendMultiFile(&send_file_info, 1);
    return TRUE;
}

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_SendFileViaMMS(wchar       *record_file_path_ptr,
                                        uint16  record_file_path_len)
{

    MMIFILE_FILE_INFO_T file_info = {0};

    if(PNULL == record_file_path_ptr
            || 0 == record_file_path_len)
    {
        return FALSE;
    }

    MMIAPIFMM_GetFileInfoFormFullPath(record_file_path_ptr, record_file_path_len, &file_info);
    MMIAPIMMS_EditMMSFromFile(&file_info, MMIFMM_FILE_TYPE_MUSIC);
    return TRUE;
}

/*****************************************************************************/
//  Description :MMIAPIRECORD_DecideRecordWorkFormat
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_WORK_FORMAT_E  MMIRECORD_DecideRecordWorkFormat(uint32  record_support_type)//recordwork
{
    MMIRECORD_WORK_FORMAT_E recordformat = MMIRECORD_GetRecordFileFormatType();

    if(record_support_type & recordformat)
    {
        return recordformat;
    }
    else
    {
        if(record_support_type & MMIRECORD_WORK_FORMAT_MP3)
        {
            recordformat =  MMIRECORD_WORK_FORMAT_MP3;
        }
        else if(record_support_type & MMIRECORD_WORK_FORMAT_WAV)
        {
            recordformat =   MMIRECORD_WORK_FORMAT_WAV;
        }
        else if(record_support_type & MMIRECORD_WORK_FORMAT_AMR)
        {
            recordformat =   MMIRECORD_WORK_FORMAT_AMR;
        }
        else
        {
            recordformat =   MMIRECORD_WORK_FORMAT_WAV;
        }
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_DecideRecordWorkFormat record_support_type = %d, recordformat = %d", record_support_type, recordformat);
    return recordformat;
}

/*****************************************************************************/
//  Description : open record file
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_OpenRecordFileList(void)
{
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    wchar*              full_path = PNULL;
    uint16              full_path_len = MMIRECORD_SRV_MAX_FILE_NAME_LEN;
    MMIFILE_DEVICE_E    file_dev = MMIRECORD_GetRecordStorage();
    uint32              file_suffix = MMIFMM_FILE_TYPE(MMIFMM_MUSIC_ALL)| MMIFMM_MEDIA_TYPE(MMIFMM_MUSIC_WAV) |MMIFMM_MEDIA_TYPE(MMIFMM_MUSIC_MP3) | MMIFMM_MEDIA_TYPE(MMIFMM_MUSIC_AMR);
    full_path = SCI_ALLOC_APP((MMIRECORD_SRV_MAX_FILE_NAME_LEN )*sizeof(wchar));
    SCI_MEMSET(full_path, 0, ((MMIRECORD_SRV_MAX_FILE_NAME_LEN )* sizeof(wchar)));

    //check directory
	file_dev_ptr = MMIAPIFMM_GetDevicePath((file_dev));
	file_dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
    MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
    PNULL, 0, full_path, &full_path_len);

#ifdef DRM_SUPPORT
    file_suffix |= MMIFMM_DRM_ALL;
#endif
    MMIAPIFMM_OpenExplorerExt(full_path, full_path_len,PNULL,0,FALSE, file_suffix);    
    
    SCI_FREE(full_path);

}

/*****************************************************************************/
//  Description : show error message as error type
//  Global resource dependence :
//  Author:robert.wang
//  Note:
//  Param [Input]:  fs_error    error  type as MMIFILE_ERROR_E
/*****************************************************************************/
PUBLIC void  MMIRECORD_ShowErrorMsg(MMIFILE_ERROR_E fs_error)
{
    MMIFILE_DEVICE_E   filedev ;
    BOOLEAN  ret = TRUE;

    filedev = MMIRECORD_GetCurrentDev();

    ret = MMIAPIFMM_CheckMemoryCard(filedev);

    //除磁盘格式错误外的
    if(ret)
    {
        if(SFS_ERROR_ABORT == fs_error)
        {
            MMIPUB_OpenAlertWinByTextIdEx(SPRD_RECORD_APPLET_ID, PNULL, TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                                          MMIPUB_SOFTKEY_ONE, PNULL, PNULL);
        }
        else
        {
            MMIAPIFMM_PromptFileErrorType(PNULL, fs_error, filedev);
        }
    }
}


/*****************************************************************************/
//  Description : SetVolumeCallback
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if(PNULL == para_ptr)
    {
        return;
    }

    MMIAPISET_SetMultimVolume(para_ptr->cur_value);
    MMIRECORD_SetVolume(para_ptr->cur_value);
    MMK_SendMsg(MMIRECORD_GetRecordWin(), MSG_UPDATE_VOL_ICON, PNULL);
}


/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type)
{
    MMICOM_PANEL_INFO_T panel_info = {0};
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = MMISET_VOL_ZERO;
    panel_info.max_value = MMISET_VOL_MAX;
    panel_info.panel_type = MMICOM_PANEL_VOLUME;
    panel_info.open_type = turn_vol_type;
    panel_info.set_callback = SetVolumeCallback;
    panel_info.x = -1;
    panel_info.y = -1;
    MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinHandle(), &panel_info);
}


/*****************************************************************************/
// 	Description : set mute
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_SetMute(void)
{
    MMICOM_PANEL_CALLBACK_T para = {0};
    uint8                   vol = MMIAPISET_GetMultimVolume();

    if(MMIAPICOM_SwitchPanelValue(MMICOM_PANEL_VOLUME))
    {
        return FALSE;
    }
    else if(MMISET_VOL_ZERO == vol)
    {
        para.cur_value = MMIAPISET_GetLastMultimVolume();
        SetVolumeCallback(&para);
    }
    else
    {
        para.cur_value = 0;
        para.last_value = MMIAPISET_GetLastMultimVolume();
        SetVolumeCallback(&para);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : recorder exit
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Exit(void)
{
    MMK_CloseApplet(SPRD_RECORD_APPLET_ID);
}

/*****************************************************************************/
//  Description : 外部模块调用录音模块插入录音，如MMS/ASP/ACENDLER/weibo编辑插入录音
//  Global resource dependence :
//      Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_WORK_ERROR_E MMIRECORD_OpenMainWin(MMIRECORD_WORK_PARAM_T *work_param_ptr)
{
    MMIRECORD_WORK_ERROR_E result = MMIRECORD_WORK_ERROR;
    MMI_APPLET_START_T  start = {0};

    if (!MMIAPIUDISK_TipWin_Judge())
    {
        if(!CheckRecordIsValidRun())
        {
            return result;
        }
        else if((PNULL != work_param_ptr && work_param_ptr->max_file_size <= MMIRECORD_WORK_LEASTSIZE && work_param_ptr->max_file_size != 0)
                || (PNULL != work_param_ptr && work_param_ptr->max_file_time <= MMIRECORD_WORK_LEASTTIME && work_param_ptr->max_file_time != 0))
        {
            return result;
        }
        else
        {
            start.guid = SPRD_RECORD_APPLET_ID;
            start.param_ptr = work_param_ptr;
            start.size_of_param = sizeof(MMIRECORD_WORK_PARAM_T);
            MMK_StartApplet(&start);
            result = MMIRECORD_WORK_SUCCESS;
        }
    }

    return result;
}

#ifdef ADULT_WATCH_SUPPORT

/*****************************************************************************/
//  Description : 外部模块调用录音模块插入录音，如MMS/ASP/ACENDLER/weibo编辑插入录音
//  Global resource dependence :
//      Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_OpenMainWin(void)
{
    MMIRECORD_OpenMainWin(PNULL);
}
#endif
/*****************************************************************************/
//  Description : 判断录音模块是否允许启动
//  Global resource dependence :
//      Author:
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  CheckRecordIsValidRun(void)
{
    BOOLEAN result = TRUE;

    if(MMIAPIUDISK_UdiskIsRun())  //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        result = FALSE;
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
        result = FALSE;
    }
    else if(MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        result = FALSE;
    }
    else if(MMIAPICC_GetActiveCallCount() > 0)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        result = FALSE;
    }


    return result;

}


/*****************************************************************************/
//  Description : Whether the record module is busy with working.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_IsOpened(void) //TRUE: have opened
{
    BOOLEAN result = FALSE;

    if(MMK_IsOpenWin(MMIRECORD_GetRecordWin()))
    {
        result = TRUE;
    }

    return result;
}


/*****************************************************************************/
//  Description : stop record and stop play
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_StopRecordOrPlay(void)
{
    MMK_PostMsg(MMIRECORD_GetRecordWin(), MSG_STOP_RECORD_PLAY, PNULL, PNULL);
}


/*****************************************************************************/
//  Description : insert record file
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_InsertRecord(wchar * file_full_path_ptr,
                                      uint16  file_full_path_len)
{
    MMIRECORD_WORK_DATA_INFO_T     data_info = {0};
    MMIRECORD_APPLET_T* applet_ptr = (MMIRECORD_APPLET_T*)MMK_GetAppletPtr(SPRD_RECORD_APPLET_ID);


    if(PNULL != applet_ptr
            && PNULL != applet_ptr->record_info.callback
            && PNULL != file_full_path_ptr
            && 0 != file_full_path_len)
    {
        data_info.file_name = file_full_path_ptr;
        data_info.file_name_len = file_full_path_len;
        applet_ptr->record_info.callback(MMIRECORD_WORK_SUCCESS, &data_info);
        return TRUE;

    }

    return FALSE;


}

/*****************************************************************************/
//  Description : MMIRECORD_IsExistValidateDevice
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_IsExistValidateDevice(void)
{
    MMIFILE_DEVICE_E        file_dev = MMI_DEVICE_NUM;
    MMIFILE_DEVICE_E        valid_dev = MMI_DEVICE_NUM;

    file_dev = MMIRECORD_GetRecordStorage();
    valid_dev = MMIAPIFMM_GetValidateDevice(file_dev);

    SCI_TRACE_LOW("[RECORD] MMIRECORD_IsExistValidateDevice file_dev = %d, valid_dev = %d", file_dev, valid_dev);

    if(valid_dev < MMI_DEVICE_NUM)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordDeviceIndex
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIRECORD_GetRecordDeviceIndex(void)
{
    MMIFILE_DEVICE_E        file_dev = MMI_DEVICE_NUM;
    MMIFILE_DEVICE_E        valid_dev = MMI_DEVICE_NUM;
    uint16                  index = 0;

    file_dev = MMIAPIRECORD_GetRecordStorage();
    valid_dev = MMIAPIFMM_GetValidateDevice(file_dev);

    if(MMIAPIFMM_GetDeviceTypeStatus(file_dev))
    {
        index = file_dev - MMI_DEVICE_UDISK; /*lint !e656*/
    }
    else
    {
        valid_dev = MMIAPIFMM_GetValidateDevice(file_dev);
        index = valid_dev - MMI_DEVICE_UDISK; /*lint !e656*/
    }

    SCI_TRACE_LOW("[RECORD] MMIRECORD_GetRecordDeviceIndex index = %d", index);
    return index;
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordFileFormatType
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_WORK_FORMAT_E MMIRECORD_GetRecordFileFormatType(void)
{
    return MMIRECORD_Setting_GetRecordFileFormatType();
}

/*****************************************************************************/
//  Description : MMIRECORD_SetRecordFileFormatType
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_E filetype)
{
    MMIRECORD_Setting_SetRecordFileFormatType(filetype);
}

/*****************************************************************************/
//  Description : MMIRECORD_SetRecordStorage
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordStorage(MMIFILE_DEVICE_E storage)
{
    MMIRECORD_Setting_SetRecordStorage(storage);
}


/*****************************************************************************/
//  Description : MMIRECORD_GetRecordStorage
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIRECORD_GetRecordStorage(void)
{
    return MMIRECORD_Setting_GetRecordStorage();
}

/*****************************************************************************/
//  Description : MMIRECORD_SetRecordId
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordId(uint32 record_id)
{
    MMIRECORD_Setting_SetRecordFileId(record_id);
}


/*****************************************************************************/
//  Description : MMIRECORD_GetRecordId
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIRECORD_GetRecordId(void)
{
    return MMIRECORD_Setting_GetRecordFileId();
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordWin
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIRECORD_GetRecordWin(void)
{
    MMIRECORD_APPLET_T* applet_ptr = (MMIRECORD_APPLET_T*)MMK_GetAppletPtr(SPRD_RECORD_APPLET_ID);

    if(PNULL != applet_ptr)
    {
        return applet_ptr->record_info.win_handle;
    }

    return PNULL;
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordState
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_STATE_E MMIRECORD_GetRecordState(void)
{
    MMIRECORD_APPLET_T* applet_ptr = (MMIRECORD_APPLET_T*)MMK_GetAppletPtr(SPRD_RECORD_APPLET_ID);

    if(PNULL != applet_ptr)
    {
        return applet_ptr->record_info.record_state;
    }

    return MMIRECORD_STATE_START;
}

/*****************************************************************************/
//  Description : MMIRECORD_SetRecordState
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordState(MMIRECORD_STATE_E state)
{
    MMIRECORD_APPLET_T* applet_ptr = (MMIRECORD_APPLET_T*)MMK_GetAppletPtr(SPRD_RECORD_APPLET_ID);

    if(PNULL != applet_ptr)
    {
        applet_ptr->record_info.record_state = state;
    }
}

/*****************************************************************************/
//  Description : MMIRECORD_SetRecordHandle
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordHandle(RECORD_SRV_HANDLE srv_handle)
{
    MMIRECORD_APPLET_T* applet_ptr = (MMIRECORD_APPLET_T*)MMK_GetAppletPtr(SPRD_RECORD_APPLET_ID);

    if(PNULL != applet_ptr)
    {
        applet_ptr->record_info.record_srv_handle = srv_handle;
    }
}

/*****************************************************************************/
//  Description : MMIRECORD_GetRecordHandle
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC RECORD_SRV_HANDLE MMIRECORD_GetRecordHandle(void)
{
    MMIRECORD_APPLET_T* applet_ptr = (MMIRECORD_APPLET_T*)MMK_GetAppletPtr(SPRD_RECORD_APPLET_ID);

    if(PNULL != applet_ptr)
    {
        return applet_ptr->record_info.record_srv_handle;
    }

    return PNULL;
}
