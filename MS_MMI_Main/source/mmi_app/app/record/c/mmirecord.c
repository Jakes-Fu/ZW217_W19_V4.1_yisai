/*****************************************************************************
** File Name:      mmirecord.c                                               *
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
#ifdef MMI_RECORD_SUPPORT
#define _MMIRECORD_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_record_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmirecord_export.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmi_id.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicc_export.h"
#include "mmi_appmsg.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiidle_export.h"
#include "mmiacc.h"
#include "mmi_nv.h"
#include "mmiphone_export.h"
#include "mmisd_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_internal.h"
#endif
#include "mmirecord_id.h"
#include "mmirecord_nv.h"
#include "mmi_default.h"
#include "mmipub.h"
//#include "mmimultim_internal.h"
#include "mmidc_save.h"
#include "mmimms_export.h"
#include "os_api.h"
#include "sci_api.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "mmimultim_text.h"
#include "mmifmm_export.h"
#include "mmiudisk_export.h"
#include "mmicc_text.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define RECORD_FILE_NAME_ID_MAX				    10000000

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
wchar*                          s_record_file_path = PNULL;
static wchar*                   s_record_filename = PNULL;    
static MMIRECORD_STATE_E        s_current_state = MMIRECORD_STATE_START;
MMIRECORD_SETTING_INFO_T        s_record_setting_info = {0};/*lint !e64*/
static BOOLEAN                  s_is_cur_file_allow_deleted = FALSE;  //flag to delete cur file
//static SFS_HANDLE				s_sfs_handle = 0;

LOCAL MMISRV_HANDLE_T s_rec_record_handle = 0;
LOCAL MMISRV_HANDLE_T s_rec_play_handle = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Callback function to MMI AUDIO
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIRECORD_CallBack(
                MMISRVAUD_REPORT_RESULT_E  result,
                DPARAM           param
                );

/*****************************************************************************/
//  Description : 菜单中播放录音的callback函数
//  Global resource dependence :                                
//      Author: 
//  Note:
/*****************************************************************************/
LOCAL void CallbackWavPlay(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//  Description : notify handle function for record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_rec(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : notify handle function for play
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_ply(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
// 	Description : create name
//	Global resource dependence : none
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
LOCAL uint16 GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type);

/*****************************************************************************/
//  Description : 录音器初始状态检查
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
//  Param:
//        return: value of MMIFILE_ERROR_E type
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E RecorderFirstCheck(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetPreferDev(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetCurrentDev(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CheckFilenameSuffix(void);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Constructor(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    //BOOLEAN         udisk = FALSE;
    //BOOLEAN         sd = FALSE;
   // MMIAPIUDISK_GetAllDeviceStatus(&udisk, &sd);
    MMINV_READ(MMINV_RECORD_SETTING, &s_record_setting_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_record_setting_info.dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
        s_record_setting_info.call_file_id = 0;

    if(Record_QueryEncoderCapability(AUD_REC_MP3) )
    {
        s_record_setting_info.file_formattype = MMISRVAUD_RING_FMT_MP3;
    }
    else
    {
        s_record_setting_info.file_formattype = MMISRVAUD_RING_FMT_WAVE;
    }

        MMINV_WRITE(MMINV_RECORD_SETTING, &s_record_setting_info);        
    }
    if(PNULL == s_record_filename)
    {
        s_record_filename = SCI_ALLOC_APP((MMIRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
    }
    if(PNULL == s_record_file_path)
    {
        s_record_file_path = SCI_ALLOC_APP((MMIRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
    }
    MMIRECORD_ClearCurrentRecordFileName();
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_Destructor(void)
{
    SCI_FREE(s_record_filename);
    SCI_FREE(s_record_file_path);
    
    s_record_filename = PNULL;
    s_record_file_path = PNULL;
    MMINV_WRITE(MMINV_RECORD_SETTING, &s_record_setting_info);
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORD_GetRecordFileName(void)
{
    return s_record_filename;
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORD_GetRecordFilePath(void)
{
    return s_record_file_path;
}
/*****************************************************************************/
// 	Description : 得到录音文件的全路径名
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC  uint16 MMIAPIRECORD_GetFileFullPathName(wchar* file_full_path_ptr, // out
                                                uint16 file_name_len)
{
    uint16 path_len = MMIAPICOM_Wstrlen(s_record_file_path);
    uint16 name_len = MMIAPICOM_Wstrlen(s_record_filename);
    uint16 full_name_len = 0;

    if(file_name_len < path_len + name_len)
    {
        //SCI_TRACE_LOW:"MMIRECRD get file full path name file_name_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_223_112_2_18_2_49_17_0,(uint8*)"d", file_name_len);
        return 0;
    }

    SCI_MEMSET(file_full_path_ptr, 0, file_name_len);
    if(NULL != s_record_file_path)
    {
        MMIAPICOM_Wstrncpy(file_full_path_ptr, s_record_file_path, path_len);
    }
    if(NULL != s_record_filename)
    {
        MMIAPICOM_Wstrcat(file_full_path_ptr, s_record_filename);
    }

    full_name_len = path_len + name_len;

    return full_name_len;
}

/*****************************************************************************/
// 	Description : handle record end.
//	Global resource dependence :
//  Author: xingdong.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_HandleRecordEnd(BOOLEAN is_succeed)
{
	BOOLEAN has_enough_space = FALSE;
	MMIFILE_DEVICE_E  i = MMI_DEVICE_NUM -1;
	
    if (MMIAPIENG_GetIQModeStatus())
    {
        if(MMIRECORD_STATE_STOPING == s_current_state)
        {
            MMIRECORD_SetRecordStatus(MMIRECORD_STATE_RECORD_STOPED);
			for(i = MMI_DEVICE_NUM -1; i >= MMI_DEVICE_UDISK; i--)
			{
				if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, i,MMIRECORD_MIN_NEED_SPACE, PNULL))
				{
					has_enough_space = TRUE;
					break;
				}
			}
			//SCI_TRACE_LOW:"[IQ]MMIAPIRECORD_HandleRecordEnd has_enough_space %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_264_112_2_18_2_49_17_1,(uint8*)"d", has_enough_space);
			if(!has_enough_space)
			{
				MMIAPIENG_OpenHandleIQDataQueryWin(is_succeed);
			}
		}
        return;
    }
    else
    {
        if(!is_succeed)
        {
            s_is_cur_file_allow_deleted = TRUE;
            MMK_CloseWin(MMIPUB_ALERT_WIN_ID);
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL, 
                MMIPUB_SOFTKEY_ONE, PNULL);  
        }
    }
    
    if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED ,GetCurrentDev(), MMIRECORD_MIN_NEED_SPACE, PNULL) && MMIRECORD_STATE_STOPING != s_current_state)
    {
        MMIAPIRECORD_ShowErrorMsg(SFS_ERROR_NO_SPACE);
		
    }
    UpdateTotalRecordTime();
    MMIRECORD_SetRecordStatus(MMIRECORD_STATE_RECORD_STOPED);
    MMIRECORD_UpdateButtonStatus();
    MMIAPIRECORD_HandleRecordFinishInd();
    //MMIDEFAULT_AllowOpenKeylocWin(TRUE);//@chunyou.zhu
}

/*****************************************************************************/
//  Description : assign a default name
//  Global resource dependence :                                
//  Author: koki.gao
//  Note:
/*****************************************************************************/
#if defined(CALL_REC_FILE_SEPERATE)
LOCAL void GetCCRecordFileName(void)
{
     // 日期和时间
    wchar  date_time[GUILIST_STRING_MAX_NUM +1]    = {0};
    wchar  time_wstr[GUILIST_STRING_MAX_NUM +1]    = {0};
    uint8  time_str[GUILIST_STRING_MAX_NUM+1]      = {0};
    uint8  date_str[GUILIST_STRING_MAX_NUM+1]      = {0};
    
    SCI_DATE_T    sys_date = {0};
    SCI_TIME_T    sys_time = {0};
#ifdef MMI_RECORD_INCALL_FORMAT_ALL    
    MMISRVAUD_RING_FMT_E formattype=MMISRVAUD_RING_FMT_MAX;
#endif
    MMISET_TIME_DISPLAY_TYPE_E time_display_type = MMIAPISET_GetTimeDisplayType();
    uint8 am_pm_chr[8]={0};
    MN_DUAL_SYS_E dual_sys = MMICC_GetCurrCallDualsys();
    uint8  sim_str[GUILIST_STRING_MAX_NUM+1]      = {0};
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);   
    
    sprintf((char*) date_str, "%02d%02d%04d", sys_date.mday,sys_date.mon,sys_date.year); //DD/MM/YYYY
    MMIAPICOM_StrToWstr(date_str, date_time);
    
    // 时间
    switch(time_display_type)
    {
        case MMISET_TIME_12HOURS:
            if ((MMISET_TIME_PM_HOUR > sys_time.hour))//AM
            {
                if ((MMISET_TIME_1_HOUR > sys_time.hour))//00:00-1:00
                {
                    sys_time.hour = sys_time.hour + MMISET_TIME_PM_HOUR;
                }
                am_pm_chr[0]='A';
                am_pm_chr[1]='M';
            }
            else//PM
            {
                if ((MMISET_TIME_PM_HOUR <= sys_time.hour) && (MMISET_TIME_13_HOUR > sys_time.hour))//12:00-13:00
                {
                }
                else
                {
                    sys_time.hour = sys_time.hour-MMISET_TIME_PM_HOUR;
                }
                am_pm_chr[0]='P';
                am_pm_chr[1]='M';
            }               
            sprintf((char*)time_str,"_%02d%02d%s%02d",sys_time.hour,sys_time.min,am_pm_chr,sys_time.sec);        
            break;
            
        case MMISET_TIME_24HOURS:
            sprintf((char*)time_str,"_%02d%02d%02d",sys_time.hour,sys_time.min,sys_time.sec);         
            break;      
            
        default:
            break;
    }
    
    MMIAPICOM_StrToWstr(time_str, time_wstr);
    MMIAPICOM_Wstrcat(date_time, time_wstr);
    
    /* Combine file name something */
    //sim
    switch(dual_sys)
    {
        case MN_DUAL_SYS_1:
            MMIAPICOM_Wstrncat(s_record_filename, L"SIM1_",5);          
            break;
        case MN_DUAL_SYS_2:
            MMIAPICOM_Wstrncat(s_record_filename, L"SIM2_",5);          
            break;
        default:
            break;
    }

    MMIAPICOM_Wstrcat(s_record_filename, date_time);
#ifdef MMI_RECORD_INCALL_FORMAT_ALL
    formattype=MMIAPIRECORD_GetRecordFileFormatType();
    
    if(MMISRVAUD_RING_FMT_MP3==formattype)
    {
        MMIAPICOM_Wstrcat(s_record_filename, L".mp3");
    }
    else if(MMISRVAUD_RING_FMT_AMR==formattype)
    {
        MMIAPICOM_Wstrcat(s_record_filename, L".amr");
    }
    else
    {
        MMIAPICOM_Wstrcat(s_record_filename, L".wav");  
    } 
#else
    MMIAPICOM_Wstrcat(s_record_filename, L".wav"); 
#endif
}
#else
LOCAL void GetCCRecordFileName(void)
{
    wchar         telname[CC_MAX_PBNAME_LEN + 1]   = {0};
    wchar         name[CC_MAX_PBNAME_LEN + 1]   = {0};
    uint8         cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8         tel_num_len = CC_MAX_TELE_NUM_LEN + 2;
    MMI_STRING_T  call_name_str = {0};
     // 日期和时间
    wchar  date_time[GUILIST_STRING_MAX_NUM +1]    = {0};
    wchar  time_wstr[GUILIST_STRING_MAX_NUM +1]    = {0};
    uint8  time_str[GUILIST_STRING_MAX_NUM+1]      = {0};
    uint8  date_str[GUILIST_STRING_MAX_NUM+1]      = {0};
    
    SCI_DATE_T    sys_date = {0};
    SCI_TIME_T    sys_time = {0};
#ifdef MMI_RECORD_INCALL_FORMAT_ALL    
    MMISRVAUD_RING_FMT_E formattype=MMISRVAUD_RING_FMT_MAX;
#endif

    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);   
    
      // 日期
    sprintf((char*) date_str, "%04d%02d%02d", sys_date.year, sys_date.mon, sys_date.mday); 
    MMIAPICOM_StrToWstr(date_str, date_time);
    
    // 时间
    sprintf((char*)time_str,"%02d%02d%02d",sys_time.hour,sys_time.min,sys_time.sec); 
    MMIAPICOM_StrToWstr(time_str, time_wstr);
    MMIAPICOM_Wstrcat(date_time, time_wstr);
    
    /* Get call number */
    MMIAPICC_GetCallName(&call_name_str);
    tel_num_len = MMIAPICC_GetCallNumber(cur_call_num, tel_num_len); 
    // 如果是隐藏号码来电...
    if(0 == tel_num_len)
    {
        MMI_GetLabelTextByLang(TXT_UNKNOW_NUM,  &call_name_str);
    }
    // 如果是未命名的情况，则取号码作为......
    else if(MMIAPIPB_IsUseDefaultName(call_name_str.wstr_ptr)||!MMIAPICOM_FileNameIsValid(&call_name_str))
    {
        MMIAPICOM_StrToWstr(cur_call_num, telname);
        MMIAPICOM_GetValidFileName(FALSE, telname, tel_num_len, name, CC_MAX_PBNAME_LEN);
        call_name_str.wstr_ptr = name;
        call_name_str.wstr_len = MMIAPICOM_Wstrlen(name);
    }
    /* Combine file name something */
    MMIAPICOM_Wstrncat(s_record_filename, call_name_str.wstr_ptr,call_name_str.wstr_len);
    MMIAPICOM_Wstrcat(s_record_filename, L"_");
    MMIAPICOM_Wstrcat(s_record_filename, date_time);
#ifdef MMI_RECORD_INCALL_FORMAT_ALL
    formattype=MMIAPIRECORD_GetRecordFileFormatType();//andy.he_record
    
    if(MMISRVAUD_RING_FMT_MP3==formattype)
     {
         MMIAPICOM_Wstrcat(s_record_filename, L".mp3");
     }
    else if(MMISRVAUD_RING_FMT_AMR==formattype)
     {
         MMIAPICOM_Wstrcat(s_record_filename, L".amr");
     }
    else
     {
         MMIAPICOM_Wstrcat(s_record_filename, L".wav");  
     } 
#else
	MMIAPICOM_Wstrcat(s_record_filename, L".wav"); 
#endif
}
#endif
/*****************************************************************************/
//  Description : RecordWorkGetFileIDString
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  RecordWorkGetFileIDString(char* file_name, uint32 id, uint32 id_max)
{
        MMISRVAUD_RECORD_FMT_E record_format=MMIAPIRECORD_GetRecordWorkFormat();
        
        if(record_format == MMISRVAUD_RECORD_FMT_MP3)
        {
                GetFileIDString(file_name, "REC", id, id_max, "mp3");
        }
        else if(record_format == MMISRVAUD_RECORD_FMT_ADPCM)
        {
                 GetFileIDString(file_name, "REC", id, id_max, "wav");
        }
        else if(record_format == MMISRVAUD_RECORD_FMT_AMR)
        {
                GetFileIDString(file_name, "REC", id, id_max, "amr");
        }
        else
        {
                GetFileIDString(file_name, "REC", id, id_max, "wav");
        }

    }


/*****************************************************************************/
//  Description : assign a default name
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateFileName(void)
{
    const wchar*    file_dev_ptr = PNULL;
    uint16          file_dev_len = 0;
    wchar           full_path[MMIRECORD_MAX_FILE_NAME_LEN +1] = {0};
    uint16          full_path_len = MMIRECORD_MAX_FILE_NAME_LEN;
    char            temp_photo_name[MMIRECORD_MAX_FILE_NAME_LEN] = {0};
    uint32                circle = 1;
    register uint32       name_id = 0;
    register uint32       step = 1;
    register uint32       id_max = RECORD_FILE_NAME_ID_MAX;
   // BOOLEAN               is_first_create = TRUE;
    MMIFILE_DEVICE_E    file_dev = GetPreferDev();
    MMISRVAUD_RING_FMT_E formattype = MMISRVAUD_RING_FMT_MAX;
    #if defined(CALL_REC_FILE_SEPERATE)
    //Call recording
    wchar call_rec_dir[MMIRECORD_MAX_FILE_NAME_LEN + 2] = {'A','u','t','o',' ','C','a','l','l',' ','r','e','c','o','r','d','e','r','\\',0};
    wchar         telname[CC_MAX_PBNAME_LEN + 1]   = {0};
    wchar         name[CC_MAX_PBNAME_LEN + 1]   = {0};  
    #endif

    //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_CreateFileName()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_411_112_2_18_2_49_17_2,(uint8*)"");
    
   if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {

       file_dev = (file_dev < MMI_DEVICE_NUM ? file_dev : MMI_DEVICE_UDISK);
       file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
       file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
       
        GetCCRecordFileName();
        #if defined(CALL_REC_FILE_SEPERATE)
        {
            MMI_STRING_T call_name_str = {0};
            uint8 tel_num_len = CC_MAX_TELE_NUM_LEN + 2;
            uint8 cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
                        
            /* Get call number */
            MMIAPICC_GetCallName(&call_name_str);
            tel_num_len = MMIAPICC_GetCallNumber(cur_call_num, tel_num_len); 

            if (tel_num_len == 0)
            {
                MMI_GetLabelTextByLang(TXT_UNKNOW_NUM,  &call_name_str);
            }
            else if(MMIAPIPB_IsUseDefaultName(call_name_str.wstr_ptr)||!MMIAPICOM_FileNameIsValid(&call_name_str))
            {
                MMIAPICOM_StrToWstr(cur_call_num, telname);
                MMIAPICOM_GetValidFileName(FALSE, telname, tel_num_len, name, CC_MAX_PBNAME_LEN);
                call_name_str.wstr_ptr = name;
                call_name_str.wstr_len = MMIAPICOM_Wstrlen(name);                   
            }

            MMIAPICOM_Wstrncat(call_rec_dir, call_name_str.wstr_ptr, call_name_str.wstr_len);
            MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, call_rec_dir, MMIAPICOM_Wstrlen(call_rec_dir),
            s_record_filename, (uint16)MMIAPICOM_Wstrlen(s_record_filename), full_path, &full_path_len);
        }
        #else
        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
        s_record_filename, (uint16)MMIAPICOM_Wstrlen(s_record_filename), full_path, &full_path_len);
        #endif
        if(full_path_len > MMIRECORD_MAX_FILE_NAME_LEN)
        {
            return FALSE;
        }

        MMIAPICOM_Wstrcpy(s_record_file_path, full_path);
        return TRUE;
        
    }
    else
    {
        while(1)  /*lint !e716*/
        {
            name_id = s_record_setting_info.record_file_id;
            name_id += step;
            step = step * 2;
            if(name_id >= id_max)
            {
                circle ++;
                if(circle >= id_max)
                {
                    circle = 1;
                }
                name_id = circle;
                step = 1;
            }
            s_record_setting_info.record_file_id = name_id;
            formattype=MMIAPIRECORD_GetRecordFileFormatType();//andy.he_record
            
            if(MMIAPIRECORD_GetRecordWorkState())//recordwork
            {
                RecordWorkGetFileIDString(temp_photo_name, name_id, id_max);
            }      
            else if(MMISRVAUD_RING_FMT_MP3 == formattype)
            {
                GetFileIDString(temp_photo_name, "REC", name_id, id_max, "mp3");
            }
            else if(MMISRVAUD_RING_FMT_AMR == formattype) 
            {
                GetFileIDString(temp_photo_name, "REC", name_id, id_max, "amr");
            }
             else 
			{
                GetFileIDString(temp_photo_name, "REC", name_id, id_max, "wav");
            }
            MMIAPICOM_StrToWstr(temp_photo_name, s_record_filename);/*lint !e64*/
            file_dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
            file_dev_ptr =  MMIAPIFMM_GetDevicePath((file_dev));

            MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
                s_record_filename, (uint16)MMIAPICOM_Wstrlen(s_record_filename), full_path, &full_path_len);
            if(full_path_len > MMIRECORD_MAX_FILE_NAME_LEN)
            {
                return FALSE;
            }

            if(!MMIAPIFMM_IsFileExist(full_path, full_path_len))                
            {
                MMIAPICOM_Wstrcpy(s_record_file_path, full_path);
                return TRUE;
            }
        }//
    }
    return TRUE; /*lint !e527 */
}

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_DeleteTempFile(void)
{
    MMIRECORD_RESULT_E result = MMIRECORD_RESULT_SUCCESS;
    uint16 cur_file_len = MMIAPICOM_Wstrlen(s_record_file_path);

    if(!MMIAPIFMM_IsFileExist(s_record_file_path, cur_file_len))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COM_FILE_NO_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
            MMIPUB_SOFTKEY_ONE, PNULL);
        return MMIRECORD_RESULT_ERROR;
    }
    else
    {        
        if (0 != cur_file_len)
        {
            if (!MMIAPIFMM_DeleteFileSyn(s_record_file_path, cur_file_len))
            {
                result = MMIRECORD_RESULT_ERROR;
            }
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SendFileViaBT(void)
{
    uint16      full_name_len = MMIAPICOM_Wstrlen(s_record_file_path);	
    uint32      send_file_size = 0;
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};

    //get file size info
    MMIAPIFMM_GetFileInfo(s_record_file_path, full_name_len, &send_file_size, NULL, NULL);
    MMIAPICOM_Wstrncpy(send_file_info.filepath_name,s_record_file_path,full_name_len);
    send_file_info.filepath_len = full_name_len;
    send_file_info.file_size = send_file_size;
    send_file_info.is_temp_file = FALSE;        
    MMIAPIBT_SendMultiFile(&send_file_info,1);
}

/*****************************************************************************/
//  Description : 用户不保存录音，删除临时文件
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SendFileViaMMS(void)
{
    uint16 file_name_len = MMIAPICOM_Wstrlen(s_record_file_path);
    MMIFILE_FILE_INFO_T file_info = {0};
    
    MMIAPIFMM_GetFileInfoFormFullPath(s_record_file_path,file_name_len,&file_info);
    MMIAPIMMS_EditMMSFromFile(&file_info,MMIFMM_FILE_TYPE_MUSIC);
}

/*****************************************************************************/
//  Description : 开始录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIAPIRECORD_StartRecordIncall(void)
{
    MMIRECORD_RESULT_E  error_code = MMIRECORD_RESULT_SUCCESS;
    BOOLEAN             aud_error  = FALSE;
    MMIFILE_ERROR_E     fs_ret = SFS_ERROR_NONE;
    MMISRVAUD_TYPE_U    audio_data = {0};
    MMISRV_HANDLE_T     audio_handle = MMIREC_GetRecordHandle();

    MMIRECORD_Constructor();
    if(MMIRECORD_STATE_RECORD_STOPED != s_current_state && MMIRECORD_STATE_PLAY_STOPED != s_current_state &&
         MMIRECORD_STATE_START != s_current_state)
    {
        //SCI_TRACE_LOW:"[MMIRECORD] MMIAPIRECORD_StartRecordIncall current_state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_565_112_2_18_2_49_18_3,(uint8*)"d", s_current_state);
        error_code = MMIRECORD_RESULT_ERROR;
		if(!MMK_IsOpenWin(MMIRECORD_MAINPLAY_WIN_ID))
        {
            MMIRECORD_Destructor();
        }
        return error_code;
    }

    s_is_cur_file_allow_deleted = FALSE;
    
    //check space and state
    fs_ret = RecorderFirstCheck();
    if (SFS_ERROR_NONE != fs_ret)
    {
        switch(fs_ret)
        {
        case SFS_ERROR_NO_SPACE:
        case SFS_ERROR_DEVICE:
            error_code = MMIRECORD_RESULT_NO_SPACE;
            break;
        case SFS_ERROR_FOLD_FULL:
            error_code = MMIRECORD_RESULT_FOLDER_FULL;
            break;
        default:   
            error_code = MMIRECORD_RESULT_ERROR;
            break;
        }
		if(!MMK_IsOpenWin(MMIRECORD_MAINPLAY_WIN_ID))
        {
            MMIRECORD_Destructor();
        }
        
        //SCI_TRACE_LOW:"[MMIRECORD] MMIAPIRECORD_StartRecordIncall fs_ret = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_592_112_2_18_2_49_18_4,(uint8*)"d", fs_ret);
        return error_code;
    }
    else
    {
        error_code = MMIRECORD_RESULT_SUCCESS;
    }
    
    //default filename
    SCI_MEMSET(s_record_file_path, 0, ((MMIRECORD_MAX_FILE_NAME_LEN + 1) * sizeof(wchar)));
    if(!CreateFileName())
    {
		if(!MMK_IsOpenWin(MMIRECORD_MAINPLAY_WIN_ID))
        {
            MMIRECORD_Destructor();
        }
        return MMIRECORD_RESULT_NO_SPACE;
    }
    //SCI_TRACE_LOW:"[MMIRECORD] MMIAPIRECORD_StartRecordIncall() start record!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_607_112_2_18_2_49_18_5,(uint8*)"");

    //快速点击录音按钮，如果1s内点击多次，则每次创建的file name 会相同，从而导致录音失败NEWMS00191498
    if (MMIAPIFMM_IsFileExist(s_record_file_path, MMIAPICOM_Wstrlen(s_record_file_path)))
    {
        MMIAPIFMM_DeleteFile(s_record_file_path, PNULL);
    }
    
    if(audio_handle > 0)//如果原来handle不为0，则初始化。
    {
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
        MMIREC_SetRecordHandle(0);
    }
    
    if (MMIAPIENG_GetIQModeStatus())//测试模式(在通话中使用record的通道记录通话内容，使用的是record的通道)
    {
        MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
        
        if(SFS_ERROR_NONE != SFS_DeleteFile(s_record_file_path, NULL))
        {
            //SCI_TRACE_LOW:"[MMIRECORD]:MMIAPIRECORD_StartRecordIncall SFS_DeleteFile ERROR!!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_622_112_2_18_2_49_18_6,(uint8*)"");
        } 
        if(iq_mode_param.cur_mode < MMIENG_IQ_MODE_MAX)
        {

            audio_data.iq_file.type = MMISRVAUD_TYPE_IQ;
            audio_data.iq_file.name_ptr = (wchar *)s_record_file_path;
            audio_data.iq_file.param    = &(iq_mode_param.record_param[iq_mode_param.cur_mode]); /*lint !e733*/
            
            aud_error = MMIAPIRECORD_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_rec);
            if(audio_handle > 0)
            {
                aud_error = MMISRVAUD_Play(audio_handle, 0);
                MMIREC_SetRecordHandle(audio_handle);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIRECORD] MMIAPIRECORD_StartRecordIncall() cur_mode=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_640_112_2_18_2_49_18_7,(uint8*)"d", iq_mode_param.cur_mode);
        }
    }
    else
    { 
        audio_data.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
        audio_data.record_file.name     = (wchar *)s_record_file_path;
        audio_data.record_file.name_len = MMIAPICOM_Wstrlen(audio_data.record_file.name);
        audio_data.record_file.frame_len= MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;
        audio_data.record_file.source   = MMISRVAUD_RECORD_SOURCE_CALL;
#ifdef MMI_RECORD_INCALL_FORMAT_ALL        
        if(MMIAPIRECORD_GetRecordFileFormatType()==MMISRVAUD_RING_FMT_MP3)
        {
            audio_data.record_file.fmt = MMISRVAUD_RECORD_FMT_MP3;
        }
        else if(MMIAPIRECORD_GetRecordFileFormatType()==MMISRVAUD_RING_FMT_AMR)
        {
            audio_data.record_file.fmt = MMISRVAUD_RECORD_FMT_AMR;
        }
        else 
        {
            audio_data.record_file.fmt = MMISRVAUD_RECORD_FMT_ADPCM;
        }
#else
		audio_data.record_file.fmt = MMISRVAUD_RECORD_FMT_ADPCM;
#endif

        aud_error = MMIAPIRECORD_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_rec);
        if(audio_handle > 0)
        {
            aud_error = MMISRVAUD_Play(audio_handle, 0);
            MMIREC_SetRecordHandle(audio_handle);
        }
    }
    //MMIRECORD_Destructor();
    //SCI_TRACE_LOW:"[MMIRECORD] MMIAPIRECORD_StartRecordIncall() aud_error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_672_112_2_18_2_49_18_8,(uint8*)"d", aud_error);
    if (aud_error)
    {
        s_current_state = MMIRECORD_STATE_RECORDING;
    }
    else
    {
        return MMIRECORD_RESULT_ERROR;
    }
    return error_code;
}

/*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_StopRecord(void)
{
    MMIRECORD_RESULT_E error_code = MMIRECORD_RESULT_SUCCESS;
    BOOLEAN stop_result     = FALSE;
    BOOLEAN close_result    = FALSE;
    MMISRV_HANDLE_T handle  = MMIREC_GetRecordHandle();
    
    //SCI_TRACE_LOW:"[MMIRECORD] enter funciton MMIRECORD_StopRecord()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_695_112_2_18_2_49_18_9,(uint8*)"");

    if (MMIRECORD_STATE_RECORDING != s_current_state && MMIRECORD_STATE_RECORD_PAUSED != s_current_state)
    {
        //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_StopRecord() error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_699_112_2_18_2_49_18_10,(uint8*)"");
        error_code = MMIRECORD_RESULT_ERROR;
        return error_code;
    }
    if(handle > 0)
    {
        MMIRECORD_SetRecordStatus(MMIRECORD_STATE_STOPING);
        MMIRECORD_UpdateButtonStatus();

        stop_result = MMISRVAUD_Stop(handle);
        close_result= MMISRVMGR_Free(handle);
        MMIREC_SetRecordHandle(0);

        if(stop_result && close_result)
        {
            MMIAPIRECORD_HandleRecordEnd(TRUE);
        }
        else
        {
            MMIAPIRECORD_HandleRecordEnd(FALSE);
        }
        //在通话中录音结束时，释放分配空间，在录音器窗口时不予处理
        if(!MMK_IsOpenWin(MMIRECORD_MAINPLAY_WIN_ID))
        {
            MMIRECORD_Destructor();
        }
        MMIAPIFMM_UpdateCurrentFileLIst();
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_StopRecord() error: recording handle is 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_729_112_2_18_2_49_18_11,(uint8*)"");
        error_code = MMIRECORD_RESULT_ERROR;
    }

    return error_code;
}

/*****************************************************************************/
//  Description : Callback function to MMI AUDIO
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void MMIRECORD_CallBack(
                MMISRVAUD_REPORT_RESULT_E  result,
                DPARAM           param
                )
{
    //SCI_TRACE_LOW:"[MMIRECORD]:MMIRECORD_CallBack:MMISRVAUD_REPORT_RESULT_E = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_745_112_2_18_2_49_18_12,(uint8*)"d",result);

    MMIDEFAULT_TurnOnBackLight();

    if(MMISRVAUD_REPORT_RESULT_SUCESS == result)
    {
        MMIAPIRECORD_HandleRecordEnd(TRUE);
    }
    else
    {
        MMIAPIRECORD_HandleRecordEnd(FALSE);
    }
}

/*****************************************************************************/
//  Description : 暂停录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_PauseRecord(void)
{
    MMISRV_HANDLE_T audio_handle = MMIREC_GetRecordHandle();

    //SCI_TRACE_LOW:"[MMIRECORD] enter funciton MMIRECORD_PauseRecord()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_767_112_2_18_2_49_18_13,(uint8*)"");
    if(audio_handle > 0)
    {
        MMISRVAUD_Pause(audio_handle);
    }
}

/*****************************************************************************/
//  Description : 继续录音
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_ResumeRecord(void)
{
    MMISRV_HANDLE_T audio_handle = MMIREC_GetRecordHandle();

    //SCI_TRACE_LOW:"[MMIRECORD] enter funciton MMIRECORD_ResumeRecord()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_782_112_2_18_2_49_18_14,(uint8*)"");
    if(audio_handle > 0)
    {
        MMISRVAUD_Resume(audio_handle);
    }
}

/*****************************************************************************/
//  Description : 菜单中播放录音的callback函数
//  Global resource dependence :                                
//      Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void CallbackWavPlay(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    if(result != MMISRVAUD_REPORT_RESULT_STOP)
    {
        //SCI_TRACE_LOW:"[MMIRECORD] enter funciton CallbackWavPlay()"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_797_112_2_18_2_49_18_15,(uint8*)"");

        MMK_PostMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_RECORD_PLAY_CALL_BACK, &result, sizeof(MMISRVAUD_REPORT_RESULT_E));
        MMIAPICOM_TriggerMMITask();
    }
}

/*****************************************************************************/
//  Description : 暂停播放录音
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_PausePlay(void)
{
    MMISRV_HANDLE_T audio_handle = MMIREC_GetPlayHandle();

    //SCI_TRACE_LOW:"[MMIRECORD] enter funciton MMIRECORD_PausePlay()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_812_112_2_18_2_49_18_16,(uint8*)"");
    
    if(audio_handle > 0)
    {
        MMISRVAUD_Pause(audio_handle);
    }
}

/*****************************************************************************/
//  Description : 继续播放录音
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_ResumePlay(void)
{
    MMISRV_HANDLE_T audio_handle = MMIREC_GetPlayHandle();
    
    //SCI_TRACE_LOW:"[MMIRECORD] enter funciton MMIRECORD_ResumePlay()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_828_112_2_18_2_49_18_17,(uint8*)"");
    if(audio_handle > 0)
    {
        MMISRVAUD_Resume(audio_handle);
    }
}

/*****************************************************************************/
//  Description : 停止播放录音
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_StopPlay(void)
{
    MMISRV_HANDLE_T audio_handle = MMIREC_GetPlayHandle();

    //SCI_TRACE_LOW:"[MMIRECORD] enter funciton MMIRECORD_StopPlay()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_843_112_2_18_2_49_18_18,(uint8*)"");
    
    if(audio_handle > 0)
    {
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
        MMIREC_SetPlayHandle(0);
    }
}

/*****************************************************************************/
//  Description : play current record
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_RESULT_E MMIRECORD_PlayCurrent(void)
{
    MMIRECORD_RESULT_E  error_code = MMIRECORD_RESULT_SUCCESS;
    //uint8               ring_vol   = MMIAPISET_GetMultimVolume();
    wchar*              file_name_ptr = s_record_file_path;
    MMISRVAUD_TYPE_U    audio_data  = {0};
    MMISRV_HANDLE_T     audio_handle  = MMIREC_GetPlayHandle();

    //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_PlayCurrent()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_865_112_2_18_2_49_18_19,(uint8*)"");

    if(audio_handle > 0)
    {
        MMIRECORD_StopPlay();//停，并释放handle,再重新申请，能申请到则允许播放
    }
    
    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = file_name_ptr;
    audio_data.ring_file.name_len = MMIAPICOM_Wstrlen(file_name_ptr);
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
    if(MMIAPIRECORD_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_ply))
    {
        MMIREC_SetPlayHandle(audio_handle);
        //if(ring_vol > MMISET_VOL_MAX)
        //{
        //    SCI_TRACE_LOW("[MMIRECORD] MMIRECORD_PlayCurrent(),ring_vol > MMISET_VOL_MAX!!!!!!!!!");
        //    ring_vol = MMISET_VOL_MAX;
        //}
        //MMISRVAUD_SetVolume(audio_handle, ring_vol);

        if(!MMISRVAUD_Play(audio_handle, 0))
        {
            CallbackWavPlay(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
            MMIRECORD_StopPlay();
            error_code = MMIRECORD_RESULT_ERROR;
        }
    }
    else
    {
        CallbackWavPlay(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        error_code = MMIRECORD_RESULT_ERROR;
    }

    if (MMIRECORD_RESULT_SUCCESS == error_code)
    {
        MMIRECORD_SetRecordStatus(MMIRECORD_STATE_PLAYING);
        MMIRECORD_UpdateButtonStatus();
    }
    return error_code;
}

/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIRECORD_GetRecordStorage(void)
{
    return s_record_setting_info.dev;
}

/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none 
//  Author:andy.he
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIRECORD_GetRecordFileFormatType(void)
{
    return s_record_setting_info.file_formattype;
}

/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordStorage(MMIFILE_DEVICE_E storage)
{
    //SCI_TRACE_LOW:"[MMIRECORD]:MMIRECORD_SetRecordStorage storage=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_931_112_2_18_2_49_18_20,(uint8*)"d",storage);
    s_record_setting_info.dev = storage;
}

/*****************************************************************************/
//  Description : set storage setting in nv
//  Global resource dependence : none
//  Author: andy.he
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordFileFormatType(MMISRVAUD_RING_FMT_E filetype)//andy.he_record 
{
    //SCI_TRACE_LOW:"[MMIRECORD]:MMIRECORD_SetRecordFileFormatType file_formattype=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_941_112_2_18_2_49_18_21,(uint8*)"d",filetype);
    s_record_setting_info.file_formattype = filetype;
}

/*****************************************************************************/
//  Description : to whether there is recoding  
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIRECORD_IsRecording(void)
{
    //SCI_TRACE_LOW:"[MMIRECORD] MMIAPIRECORD_IsRecording: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_951_112_2_18_2_49_18_22,(uint8*)"d", s_current_state);
    return (MMIRECORD_STATE_RECORDING == s_current_state);
}
/*****************************************************************************/
//  Description :MMIAPIRECORD_DecideRecordWorkFormat
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_RECORD_FMT_E  MMIAPIRECORD_DecideRecordWorkFormat(void)//recordwork
{
        MMIRECORD_WORK_FORMAT_E recordformat=MMIRECORD_WORK_FORMAT_MAX;
        MMISRVAUD_RECORD_FMT_E  audioformat=MMISRVAUD_RECORD_FMT_MAX;
        uint32  record_support_type=MMIAPIRECORD_GetRecordWorkSupportFormat();
        switch(MMIAPIRECORD_GetRecordFileFormatType())
        {
        case MMISRVAUD_RING_FMT_WAVE:
            recordformat=MMIRECORD_WORK_FORMAT_WAV;
            break;
        case MMISRVAUD_RING_FMT_MP3:
            recordformat=MMIRECORD_WORK_FORMAT_MP3;
            break;
        case MMISRVAUD_RING_FMT_AMR:
            recordformat=MMIRECORD_WORK_FORMAT_AMR;
            break;
         default:
             recordformat=MMIRECORD_WORK_FORMAT_WAV;
             break;
        }
        if(record_support_type & recordformat)
        {
            switch(recordformat)
            {
            case MMIRECORD_WORK_FORMAT_WAV:
                audioformat=MMISRVAUD_RECORD_FMT_ADPCM;
                break;
            case MMIRECORD_WORK_FORMAT_MP3:
                audioformat=MMISRVAUD_RECORD_FMT_MP3;
                break;
            case MMIRECORD_WORK_FORMAT_AMR:
                audioformat=MMISRVAUD_RECORD_FMT_AMR;
                break;
            default:
                audioformat=MMISRVAUD_RECORD_FMT_ADPCM;
                break;
            }
        }
        else
        {
            if(record_support_type & MMIRECORD_WORK_FORMAT_MP3)
            {
                audioformat=  MMISRVAUD_RECORD_FMT_MP3;          
             }
            else if(record_support_type & MMIRECORD_WORK_FORMAT_WAV)
            {
                 audioformat=   MMISRVAUD_RECORD_FMT_ADPCM;
            }
            else if(record_support_type & MMIRECORD_WORK_FORMAT_AMR)
            {
                audioformat=   MMISRVAUD_RECORD_FMT_AMR;
            }
            else
            {
                audioformat=   MMISRVAUD_RECORD_FMT_ADPCM;
            }
        }
        return audioformat;
}
/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIRECORD_RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    req.notify = notify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = route;
    audio_srv.volume =  MMIAPISET_GetMultimVolume();

    switch(audio_data->type)
    {
    case MMISRVAUD_TYPE_RING_FILE:
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;

        if(audio_srv.volume > MMISET_VOL_MAX)
        {
            audio_srv.volume = MMISET_VOL_MAX;
        }
        break;

    case MMISRVAUD_TYPE_RECORD_FILE:
        audio_srv.info.record_file.type = audio_data->type;        
        audio_srv.info.record_file.fmt  = audio_data->record_file.fmt;
        audio_srv.info.record_file.name = audio_data->record_file.name;
        audio_srv.info.record_file.name_len = audio_data->record_file.name_len;    
        audio_srv.info.record_file.source   = audio_data->record_file.source;
        audio_srv.info.record_file.frame_len= audio_data->record_file.frame_len;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;

    case MMISRVAUD_TYPE_IQ://测试模式
        audio_srv.info.iq_file.type = audio_data->type;
        audio_srv.info.iq_file.name_ptr = audio_data->iq_file.name_ptr;
        audio_srv.info.iq_file.param    = audio_data->iq_file.param;

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
//  Description : notify handle function for record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_rec(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1097_112_2_18_2_49_19_23,(uint8*)"dd", handle, param));
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify(), report_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1102_112_2_18_2_49_19_24,(uint8*)"d", report_ptr));
        
        if(report_ptr != PNULL)
        {            
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify(), report_ptr->report=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1106_112_2_18_2_49_19_25,(uint8*)"d", report_ptr->report));
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                MMISRVAUD_Stop(handle);
                MMISRVMGR_Free(handle);
                MMIREC_SetRecordHandle(0);
                
                MMIRECORD_CallBack(report_ptr->data1, PNULL);
                break;

            default:
                break;
            }
        
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}

/*****************************************************************************/
//  Description : notify handle function for play
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleNotify_ply(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1139_112_2_18_2_49_19_26,(uint8*)"dd", handle, param));
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() report_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1144_112_2_18_2_49_19_27,(uint8*)"d", report_ptr));
        
        if(report_ptr != PNULL)
        {            
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify(), report_ptr->report=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1148_112_2_18_2_49_19_28,(uint8*)"d", report_ptr->report));
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                MMISRVAUD_Stop(handle);
                MMISRVMGR_Free(handle);
                MMIREC_SetPlayHandle(0);
                
                CallbackWavPlay(report_ptr->data1, PNULL);
                break;

            default:
                break;
            }
        
        }
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
PUBLIC MMISRV_HANDLE_T MMIREC_GetPlayHandle(void)
{
    return s_rec_play_handle;
}

/*****************************************************************************/
//  Description : get current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIREC_GetRecordHandle(void)
{
    return s_rec_record_handle;
}

/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIREC_SetPlayHandle(MMISRV_HANDLE_T handle)
{
    return s_rec_play_handle = handle;
}

/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIREC_SetRecordHandle(MMISRV_HANDLE_T handle)
{
    return s_rec_record_handle = handle;
}

/*****************************************************************************/
//  Description : 开始录音（通常情况）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIRECORD_StartRecordNormal(BOOLEAN is_reRecord)
{
    MMIFILE_ERROR_E error_code = SFS_ERROR_NONE;
    uint32          cur_file_name_len   = 0;
    MMISRVAUD_TYPE_U     audio_data  = {0};
    MMISRV_HANDLE_T      audio_handle   = MMIREC_GetRecordHandle();

    //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_StartRecordNormal"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1220_112_2_18_2_49_19_29,(uint8*)"");

    if (MMIRECORD_STATE_RECORD_STOPED != s_current_state && MMIRECORD_STATE_PLAY_STOPED != s_current_state &&
         MMIRECORD_STATE_START != s_current_state)
    {
        //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_StartRecordNormal current_state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1225_112_2_18_2_49_19_30,(uint8*)"d", s_current_state);

        error_code = SFS_ERROR_ABORT;
        return error_code;
    }
    
    if(audio_handle > 0)//如果原来handle不为0，则初始化。
    {
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
        MMIREC_SetRecordHandle(0);
    }
    s_is_cur_file_allow_deleted = FALSE;

    //check space and state
    error_code = RecorderFirstCheck();
    
    //SCI_TRACE_LOW:"[MMIRECORD] MMIRECORD_StartRecordNormal error_code=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1242_112_2_18_2_49_19_31,(uint8*)"d",error_code);
    if (SFS_ERROR_NONE != error_code)
    {
        return error_code;
    }
    
    //default filename
    if (!is_reRecord)
    {    
        SCI_MEMSET(s_record_file_path, 0, ((MMIRECORD_MAX_FILE_NAME_LEN + 1) * sizeof(wchar)));
        if(!CreateFileName())
        {
            return SFS_ERROR_NO_SPACE;
        }
    }
    else
    {
         if(!MMIAPIRECORD_GetRecordWorkState())
        {
            CheckFilenameSuffix();  
        }
  
    }

    //SCI_TRACE_LOW:"[MMIRECORD] record file name = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1266_112_2_18_2_49_19_32,(uint8*)"s", s_record_file_path);
        
    cur_file_name_len = MMIAPICOM_Wstrlen( s_record_file_path);

    audio_data.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
    audio_data.record_file.name = s_record_file_path;
    audio_data.record_file.name_len  = MMIAPICOM_Wstrlen(s_record_file_path);
    audio_data.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;    
    audio_data.record_file.source   = MMISRVAUD_RECORD_SOURCE_NORMAL;
    
    if(MMIAPIRECORD_GetRecordWorkState())
    {
        audio_data.record_file.fmt     =MMIAPIRECORD_GetRecordWorkFormat();
    }
    else if(MMIAPIRECORD_GetRecordFileFormatType()==MMISRVAUD_RING_FMT_MP3)
    {
        audio_data.record_file.fmt      = MMISRVAUD_RECORD_FMT_MP3;
    }
    else if(MMIAPIRECORD_GetRecordFileFormatType()==MMISRVAUD_RING_FMT_AMR)
    {
        audio_data.record_file.fmt      = MMISRVAUD_RECORD_FMT_AMR;
    }   
    else
    {
        audio_data.record_file.fmt      = MMISRVAUD_RECORD_FMT_ADPCM;
    }
    
    if (MMIAPIRECORD_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, HandleNotify_rec))
    {
        MMIREC_SetRecordHandle(audio_handle);
        //MMISRVAUD_SetVolume(audio_handle, AUD_MAX_SPEAKER_VOLUME);
        MMISRVAUD_Play(audio_handle, 0);
        s_current_state = MMIRECORD_STATE_RECORDING;
    }
    else
    {
        s_current_state = MMIRECORD_STATE_START;
        error_code = SFS_ERROR_ABORT;
    }
    return error_code;
}

/*****************************************************************************/
//  Description : get record status
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_STATE_E MMIAPIRECORD_GetRecordStatus(void)
{
    //SCI_TRACE_LOW:"[MMIRECORD] MMIAPIRECORD_GetRecordStatus(), status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1314_112_2_18_2_49_19_33,(uint8*)"d", s_current_state);
    return s_current_state;
}

/*****************************************************************************/
//  Description : set record status
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_SetRecordStatus(MMIRECORD_STATE_E state)
{
    s_current_state = state;
}

/*****************************************************************************/
//  Description : clear current record file name
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_ClearCurrentRecordFileName(void)
{
    MMIFILE_DEVICE_E record_dev = MMI_DEVICE_UDISK;
    wchar dev[] = {'D', ':', '\\', 0};
 
#ifndef CMCC_UI_STYLE
    record_dev = GetPreferDev();
#else
    record_dev = MMIAPIRECORD_GetRecordStorage();
#endif

    SCI_MEMSET(s_record_file_path, 0, ((MMIRECORD_MAX_FILE_NAME_LEN + 1) * sizeof(wchar)));
    if(MMIAPIFMM_GetDevicePath(record_dev) != PNULL)
	{
		MMIAPICOM_Wstrncpy(dev, MMIAPIFMM_GetDevicePath(record_dev),  MMIAPIFMM_GetDevicePathLen(record_dev));
	}

    MMIAPICOM_Wstrcpy(s_record_file_path, dev);
    MMIAPICOM_Wstrcpy(s_record_file_path + MMIAPICOM_Wstrlen(dev), MMIMULTIM_DIR_MUSIC);
    
    SCI_MEMSET(s_record_filename, 0, ((MMIRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
}

/*****************************************************************************/
//  Description :MMIRECORD_HandleWriteHeadErrInd
//  Global resource dependence :                                
//    Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_HandleWriteHeadErrInd(void)
{
    //头错误，文件已被删除
    MMIPUB_OpenAlertFailWin(TXT_ERROR);  
    MMK_CloseWin(MMIRECORD_WAIT_WIN_ID);
}

/*****************************************************************************/
//  Description :MMIRECORD_HandleWriteHeadErrInd
//  Global resource dependence :                                
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_HandleRecordFinishInd(void)
{
    if (!MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIRECORD_UpdateScreen(FALSE);
    }
    if(s_is_cur_file_allow_deleted)
    {
        MMIRECORD_DeleteTempFile();
    }
}

/*****************************************************************************/
//  Description : is allowed to delete current record?
//  Global resource dependence : s_is_cur_file_allow_deleted
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORD_GetCurFileDeleteFlag(void)
{
    //SCI_TRACE_LOW:"[MMIRECORD] GetCurFileDeleteFlag(), s_is_cur_file_allow_deleted = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1382_112_2_18_2_49_19_34,(uint8*)"d",s_is_cur_file_allow_deleted);
    return s_is_cur_file_allow_deleted;
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
//  FUNCTION:      MMIAPIRECORD_InitModule
//  Description :
//  Global resource dependence : 
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_InitModule(void)
{
    MMIRECORD_RegMenuGroup();  
    MMIRECORD_RegNv();   
    MMIRECORD_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : get all device info and check if have enough space 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param [Input]:  dev    file  device by FILE_DEV_E_T struct
//           [Input]:  size    space is or no smaller  the size
//         [Return]:  return value as MMIFILE_ERROR_E  
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E  MMIRECORD_CheckSpace(MMIFILE_DEVICE_E dev,uint32 size)
{
    uint32 free_high_word = 0;
    uint32 free_low_word = 0;
    SFS_ERROR_E  sfs_result = SFS_ERROR_NO_SPACE;
    MMIFILE_ERROR_E   record_result = SFS_ERROR_NO_SPACE;
    const wchar*    file_dev_ptr = PNULL;
    uint16          file_dev_len = 0;
    wchar           full_path[MMIRECORD_MAX_FILE_NAME_LEN] = {0};
    uint16          full_path_len = MMIRECORD_MAX_FILE_NAME_LEN;

    file_dev_ptr = MMIAPIFMM_GetDevicePath(dev); //NEWMS00173622
    file_dev_len = MMIAPIFMM_GetDevicePathLen(dev);

    if(MMIAPIFMM_GetDeviceFreeSpace(file_dev_ptr,file_dev_len,&free_high_word,&free_low_word))
    {
        sfs_result = SFS_ERROR_NONE;
        
        //space is lower
        if(!MMIAPIFMM_IsDevEnoughSpace(size, dev))
        {
            sfs_result = SFS_ERROR_NO_SPACE;
            //SCI_TRACE_LOW:"[MMIRECORD]MMIRECORD_CheckSpace: UDISK free spaced high value is = %d free space low value is = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1457_112_2_18_2_49_19_35,(uint8*)"dd", free_high_word,free_low_word);
        }
        else
        {
            file_dev_ptr = MMIAPIFMM_GetDevicePath(dev);
            file_dev_len = MMIAPIFMM_GetDevicePathLen(dev);
            MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC),
                    PNULL, 0, full_path, &full_path_len);
            if (!MMIAPIFMM_IsFolderExist(full_path,full_path_len))
            {
                sfs_result = MMIAPIFMM_CreateDirectory(full_path);
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIRECORD]MMIRECORD_CheckSpace sfs_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1472_112_2_18_2_49_19_36,(uint8*)"d",sfs_result);

    switch(sfs_result)
    {
        default:
            record_result = sfs_result;
        break;
    }

    //SCI_TRACE_LOW:"[MMIRECORD]MMIRECORD_CheckSpace record_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1481_112_2_18_2_49_19_37,(uint8*)"d",record_result);

    return record_result;
}

/*****************************************************************************/
//  Description : show error message as error type 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param [Input]:  fs_error    error  type as MMIFILE_ERROR_E
/*****************************************************************************/
PUBLIC void  MMIAPIRECORD_ShowErrorMsg(MMIFILE_ERROR_E fs_error)
{
    MMIFILE_DEVICE_E   filedev ;
    BOOLEAN  ret = TRUE;

    filedev = GetCurrentDev();
    
    ret= MMIAPIFMM_CheckMemoryCard(filedev);

    //除磁盘格式错误外的
    if(ret)
    {
        if(SFS_ERROR_ABORT == fs_error)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
                MMIPUB_SOFTKEY_ONE, PNULL);                      
        }
        else
        {
            MMIAPIFMM_PromptFileErrorType(PNULL,fs_error, filedev);
        }
    }
}

/*****************************************************************************/
//  Description : 录音器初始状态检查
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
//  Param:
//        return: value of MMIFILE_ERROR_E type
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E RecorderFirstCheck(void)
{
    MMIFILE_ERROR_E error_code = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E    file_dev = MMIAPIRECORD_GetRecordStorage();
    
    error_code = MMIRECORD_CheckSpace(file_dev,MMIRECORD_MIN_NEED_SPACE);

#ifndef CMCC_UI_STYLE //CMCC，直接提示空间不足，不必自动切换设备
    if (SFS_ERROR_NONE != error_code)
    {
        
#ifdef MSDC_CARD_SUPPORT		
        //sd 优先
        file_dev = MMISD_GetExistSDCardType();
        
        error_code = MMIAPIFMM_GetSuitableFileDev(file_dev, MMIRECORD_MIN_NEED_SPACE, &file_dev);

        if (SFS_ERROR_NONE == error_code)
        {
            //SD 卡不存在的情况下，会强制转换为U盘存储
            MMIRECORD_SetRecordStorage(file_dev); //xiaoming20120401与carrie已确认！
        }
        
#endif
    }
#endif

    return error_code;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetPreferDev(void)
{
    MMIFILE_DEVICE_E     file_dev   = MMIAPIRECORD_GetRecordStorage();
    MMIFILE_DEVICE_E     file_max_dev = MMI_DEVICE_NUM;
    //SCI_TRACE_LOW:"[MMIRECORD] GetPreferDev1:file_dev=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1553_112_2_18_2_49_20_38,(uint8*)"d",file_dev);
    if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, file_dev, MMIRECORD_MIN_NEED_SPACE, PNULL))
    {
        return file_dev;   
    }
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev(file_dev, MMIRECORD_MIN_NEED_SPACE, &file_max_dev))
    {
        //SCI_TRACE_LOW:"[MMIRECORD] GetPreferDev2:file_dev=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1560_112_2_18_2_49_20_39,(uint8*)"d",file_dev);
        //SCI_TRACE_LOW:"[MMIRECORD] GetPreferDev3:file_max_dev=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1561_112_2_18_2_49_20_40,(uint8*)"d",file_max_dev);
        return file_dev;   
    }
    //MMIAPIFMM_GetSuitableFileDev(file_dev, MMIRECORD_MIN_NEED_SPACE, &file_dev);
    //SCI_TRACE_LOW:"[MMIRECORD] GetPreferDev4:file_dev=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1565_112_2_18_2_49_20_41,(uint8*)"d",file_dev);
    //SCI_TRACE_LOW:"[MMIRECORD] GetPreferDev5:file_max_dev=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1566_112_2_18_2_49_20_42,(uint8*)"d",file_max_dev);

    return file_max_dev;   
}

/*****************************************************************************/
// 	Description : create name
//	Global resource dependence : none
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
LOCAL uint16 GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type)
{
    uint16 id_len = 0;
    uint16 id_max_len = 0;
    char string_max[20] = {0};
    uint32 i = 0;
 
    if(id >= id_max)
    {
        //SCI_TRACE_LOW:"[MMIRECORD] GetFileIDString id >= id_max"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1584_112_2_18_2_49_20_43,(uint8*)"");
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


/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetCurrentDev(void)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;

    if(PNULL != s_record_file_path)
    {
        file_dev = MMIAPIFMM_GetDeviceTypeByPath(s_record_file_path, MMIFILE_DEVICE_LEN);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIRECORD] GetCurrentDev:s_record_file_path=NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIRECORD_1615_112_2_18_2_49_20_44,(uint8*)"");
    }

    if(MMI_DEVICE_NUM <= file_dev)
    {
        file_dev=GetPreferDev();
    }
    return file_dev;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CheckFilenameSuffix(void)
{
    MMISRVAUD_RING_FMT_E audio_type =MMIAPIRECORD_GetRecordFileFormatType();
    uint8 filename_len=MMIAPICOM_Wstrlen(s_record_filename);
    wchar current_suffix[MMIRECORD_DEFAULT_SUFFIX_LEN+1]={0};
    uint16 suffix_len=MMIRECORD_DEFAULT_SUFFIX_LEN;
    wchar  audiotype_suffix[MMIRECORD_DEFAULT_SUFFIX_LEN+1]={0};
    wchar  wchar_value=0;
    uint8 recordname_len=MMIAPICOM_Wstrlen(s_record_filename);
    uint8 recordpath_len=MMIAPICOM_Wstrlen(s_record_file_path);
    int8 i=0;
    
    MMIAPIFMM_SplitFileName(s_record_filename,
    filename_len,
    PNULL,PNULL,
    current_suffix,&suffix_len);
        
    switch(audio_type)
    {
        case MMISRVAUD_RING_FMT_MP3:
            MMIAPICOM_StrToWstr((const uint8*)"mp3", audiotype_suffix);
        break;
        case MMISRVAUD_RING_FMT_AMR:
            MMIAPICOM_StrToWstr((const uint8*)"amr", audiotype_suffix);
        break;
        default:
            MMIAPICOM_StrToWstr((const uint8*)"wav", audiotype_suffix);
        break;
    }

if(MMIAPICOM_Wstrcmp(current_suffix, audiotype_suffix) !=0)
{
        //find DOT in file_name_ptr
    for(i=recordname_len-1; i >= 0; i--)
    {
        wchar_value = *(s_record_filename+i);
		
        if(MMIFILE_DOT == wchar_value)
        {
            suffix_len = recordname_len - i -1;
            break;
        }
    }
    *(s_record_filename+recordname_len-suffix_len)=0X00;
    MMIAPICOM_Wstrcat(s_record_filename+recordname_len-suffix_len, audiotype_suffix);

            //find DOT in file_name_ptr
    for(i=recordpath_len-1; i >= 0; i--)
    {
        wchar_value = *(s_record_file_path+i);
		
        if(MMIFILE_DOT == wchar_value)
        {
            suffix_len = recordpath_len - i -1;
            break;
        }
    }
    *(s_record_file_path+recordpath_len-suffix_len)=0X00;
    MMIAPICOM_Wstrcat(s_record_file_path+recordpath_len-suffix_len, audiotype_suffix);
    
}

}
 

#endif


