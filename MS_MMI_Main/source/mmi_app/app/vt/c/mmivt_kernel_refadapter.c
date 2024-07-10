/****************************************************************************
** File Name:      mmivt_kernel_refAdapter.c                               *
** Author:                                                                 *
** Date:           12/18/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2009        samboo.shen         Create
****************************************************************************/
#define _MMIVT_KERNEL_REFADAPTER_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_vt_trc.h"
#ifdef __cplusplus
extern   "C"
{
#endif
    
    //#include "mmivt_export.h"
#ifdef VT_SUPPORT
    
#include "dal_vt_middleware.h"
#include "mmivt_kernel_refAdapter.h"
#include "mmivt_position.h"
#include "mmipub.h"
#include "mmivt_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_data.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiudisk_export.h"
//#include "mmivt_export.h"
//#include "mmivt_ui.h"
#if 1//OPEN_VT_CHANGE
#include "mmi_ring.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MAIN_BLOCK_ID 0
#define SECONDARY_BLOCK_ID 1

#define MAIN_BLOCK_LAYER 2
#define SECONDARY_BLOCK_LAYER 3

#define LOWEST_BLOCK_LAYER 0

#define LCD_CONTRAST_SPAN 13
#define LCD_CONTRSAT_MIN 22

//主显示画面
LOCAL DVT_MW_DISP_RECT_T main_display_rect = 
{
    0, MAIN_BLOCK_ID, MAIN_BLOCK_LAYER, 0, {MAIN_PLAY_RECT_LEFT, MAIN_PLAY_RECT_TOP, (MAIN_PLAY_RECT_RIGHT - MAIN_PLAY_RECT_LEFT), (MAIN_PLAY_RECT_BOTTOM - MAIN_PLAY_RECT_TOP)},DVT_MW_ROTATE_0
};

//副显示画面
LOCAL DVT_MW_DISP_RECT_T secondary_display_rect = 
{
    0, SECONDARY_BLOCK_ID, SECONDARY_BLOCK_LAYER, 0, {SECONDARY_PLAY_RECT_LEFT, SECONDARY_PLAY_RECT_TOP, (SECONDARY_PLAY_RECT_RIGHT - SECONDARY_PLAY_RECT_LEFT), (SECONDARY_PLAY_RECT_BOTTOM - SECONDARY_PLAY_RECT_TOP)},DVT_MW_ROTATE_0
};

LOCAL BOOLEAN s_is_ref_open = FALSE; //记录ref是否开启
LOCAL BOOLEAN s_is_preview = FALSE; //记录当前是否处于预览状态
LOCAL BOOLEAN s_is_talk = FALSE; //记录当前是否处于通话状态

LOCAL int32 s_sensor_brightness_max = 0; //sensor最大亮度，从ref获得
LOCAL int32 s_sensor_contrast_max = 0; //sensor最大对比度，从ref获得

//just for resume sensor para when switching another sensor
LOCAL MMIVT_VIDEO_LEVEL_E s_sensor_cur_brightness = 0; 
LOCAL MMIVT_VIDEO_LEVEL_E s_sensor_cur_contrast = 0;

//提供给工程模式，可以选择VT录制声音的类型：混音，仅远端声音，仅本端声音
LOCAL uint16 s_record_audio = 0; // 0: 录制混音；1：录制远端音；2：录制本端音

LOCAL DVT_MW_ROTATEANGLE_E s_camera_init_angle[DVT_MW_SENSOR_MAX] = {DVT_MW_ROTATE_0, DVT_MW_ROTATE_0, DVT_MW_ROTATE_0};//数组中第一个参数是dummy
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/



typedef struct _VT_SIGNAL {
    SIGNAL_VARS
    DVT_MW_RET_E content;			// message content
} VT_SIGNAL;


/*****************************************************************************/
//  Description : MMIVT_REF_OpenVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:开启底层设备
/*****************************************************************************/
LOCAL void ErrorNotifyCallback(DVT_MW_RET_E error_type);
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMIVT_REF_OpenVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:开启底层设备
/*****************************************************************************/
LOCAL void ErrorNotifyCallback(DVT_MW_RET_E error_type)
{
    VT_SIGNAL    *sendSignal = PNULL;
    
    //SCI_TRACE_LOW:"mmivt ErrorNotifyCallback %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_114_112_2_18_3_6_7_64,(uint8*)"d", error_type);
    MmiCreateSignal(MSG_VT_REF_ERROR_IND,sizeof(VT_SIGNAL),(MmiSignalS**)&sendSignal);
#if 1//OPEN_VT_CHANGE
    sendSignal->Sender = SCI_IdentifyThread();
#else
    sendSignal->send = SCI_IdentifyThread();
#endif
    sendSignal->content = error_type;
    MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
}


/*****************************************************************************/
//  Description : MMIVT_REF_OpenVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:开启底层设备
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_OpenVT(void)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    MMIVT_REF_RET_E select_sensor_result = MMIVT_REF_RET_SUCCESS;
#ifndef WIN32
    if(s_is_ref_open)
    {
        //SCI_TRACE_LOW:"mmivt: ref has opened"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_133_112_2_18_3_6_7_65,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }

    ret_code = (MMIVT_REF_RET_E)DVT_MW_Open(ErrorNotifyCallback);

    if (MMIVT_REF_RET_SUCCESS == ret_code)
    {
        DVT_MW_SENSOR_PARAM_T sensor_param = {0};
        sensor_param.sensor_id = DVT_MW_SENSOR_SUB;
        sensor_param.screen2sensor_angle = s_camera_init_angle[sensor_param.sensor_id];
        select_sensor_result = (MMIVT_REF_RET_E)DVT_MW_SelectSensor_Ex(&sensor_param);
        //if select sensor return invalid param, it's has only one sensor
        if (MMIVT_REF_RET_INVALID_PARAM != select_sensor_result)
        {
            ret_code = select_sensor_result;
        }
        else
        {
            //SCI_TRACE_LOW:"mmivt: only one sensor"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_152_112_2_18_3_6_8_66,(uint8*)"");
        }
        s_is_ref_open = TRUE;
    }
#endif
    
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_CloseVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:关闭底层设备
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_CloseVT(void)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    DVT_MW_SENSOR_PARAM_T sensor_param = {0};
    sensor_param.sensor_id = DVT_MW_SENSOR_MAIN;
    sensor_param.screen2sensor_angle = s_camera_init_angle[sensor_param.sensor_id];
    
#ifndef WIN32
    if(!s_is_ref_open)
    {
        //SCI_TRACE_LOW:"mmivt: ref not open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_175_112_2_18_3_6_8_67,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }
    DVT_MW_SelectSensor_Ex(&sensor_param);
    ret_code = (MMIVT_REF_RET_E)DVT_MW_Close();

    s_is_ref_open = FALSE;    
#endif

    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_StartPreview
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:进入preview模式，显示本端画面
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_StartPreview(void)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    if (s_is_preview)
    {
        //SCI_TRACE_LOW:"MMIVT_REF_StartPreview already preview "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_196_112_2_18_3_6_8_68,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }
#ifndef WIN32
    {
        DVT_MW_DISP_RECT_T disp_info = main_display_rect;
    
        ret_code = (MMIVT_REF_RET_E)DVT_MW_PreviewStart(&disp_info);
    }
#endif
    s_is_preview = TRUE;

    //设置lcd亮度 
    MMIVT_REF_SetLcdBrightness(MMIVT_VIDEO_LEVEL_FOUR);

    return ret_code;
    
}

/*****************************************************************************/
//  Description : MMIVT_REF_StopPreview
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:退出preview模式
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_StopPreview(void)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    if (!s_is_preview)
    {
        //SCI_TRACE_LOW:"MMIVT_REF_StopPreview not previewing "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_224_112_2_18_3_6_8_69,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }
#ifndef WIN32
    {
        ret_code = (MMIVT_REF_RET_E)DVT_MW_PreviewStop();
    }
#endif
    s_is_preview = FALSE;
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_StartTalk
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:进入dial模式,显示双方画面
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_StartTalk(void)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    if (s_is_talk)
    {
        //SCI_TRACE_LOW:"MMIVT_REF_StartTalk already talk"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_245_112_2_18_3_6_8_70,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }    
#ifndef WIN32
    {
        ret_code = (MMIVT_REF_RET_E)DVT_MW_DialStart();
    } 
#endif
    s_is_talk = TRUE;

    //设置lcd亮度 
    MMIVT_REF_SetLcdBrightness(MMIVT_VIDEO_LEVEL_FOUR);

    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_StopTalk
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:退出dial模式
/*****************************************************************************/

PUBLIC MMIVT_REF_RET_E MMIVT_REF_StopTalk(void)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    if (!s_is_talk)
    {
        //SCI_TRACE_LOW:"MMIVT_REF_StopTalk not talking "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_271_112_2_18_3_6_8_71,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }
#ifndef WIN32
    {
        ret_code = (MMIVT_REF_RET_E)DVT_MW_DialStop();
    }
#endif
    s_is_talk = FALSE;

    //恢复lcd 亮度
    MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID); 
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_SetDispArea
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:向底层设置显示参数
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetDispArea(MMIVT_IMAGE_ROTATE_STATUS_E rotate_status,  MMIVT_DISP_MODE_E disp_mode)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
#ifndef WIN32 
    DVT_MW_DISP_RECT_T *local_disp_area_ptr = PNULL;
    DVT_MW_DISP_RECT_T *remote_disp_area_ptr = PNULL;

    //main_display_rect.lcd_block_layer = MAIN_BLOCK_LAYER;
    //secondary_display_rect.lcd_block_layer = SECONDARY_BLOCK_LAYER;
    
    switch(disp_mode)
    {
    case MMIVT_DISP_MODE_REMOTE_LOCAL:
        remote_disp_area_ptr = &main_display_rect;
        local_disp_area_ptr = &secondary_display_rect;
        break;
        
    case MMIVT_DISP_MODE_LOCAL_REMOTE:
        remote_disp_area_ptr = &secondary_display_rect;
        local_disp_area_ptr = &main_display_rect;
        break;
        
    case MMIVT_DISP_MODE_REMOTE_ONLY:
        remote_disp_area_ptr = &main_display_rect;
        break;
        
    case MMIVT_DISP_MODE_LOCAL_ONLY:
        local_disp_area_ptr = &main_display_rect;
        break;

    case MMIVT_DISP_MODE_PREVIEW:
        local_disp_area_ptr = &secondary_display_rect;
        break;
        
    default:
        //do nothing
        break;
    }
    if (PNULL != local_disp_area_ptr)
    {
        local_disp_area_ptr->rotation =  (DVT_MW_ROTATEANGLE_E)rotate_status;
    }
    else
    {
        //do nothing
    }
    if (PNULL != remote_disp_area_ptr)
    {
        remote_disp_area_ptr->rotation = DVT_MW_ROTATE_0;
    }
    ret_code = (MMIVT_REF_RET_E)DVT_MW_SetDisplayArea(remote_disp_area_ptr, local_disp_area_ptr);
#endif
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_HideVideo
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:打开新窗口的时候隐藏视频
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_HideVideo(MMIVT_DISP_MODE_E disp_mode)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
#ifndef WIN32 
    DVT_MW_DISP_RECT_T *local_disp_area_ptr = PNULL;
    DVT_MW_DISP_RECT_T *remote_disp_area_ptr = PNULL;

    main_display_rect.lcd_block_layer = LOWEST_BLOCK_LAYER;
    secondary_display_rect.lcd_block_layer = LOWEST_BLOCK_LAYER;
    
    switch(disp_mode)
    {
    case MMIVT_DISP_MODE_REMOTE_LOCAL:
        remote_disp_area_ptr = &main_display_rect;
        local_disp_area_ptr = &secondary_display_rect;
        break;
        
    case MMIVT_DISP_MODE_LOCAL_REMOTE:
        remote_disp_area_ptr = &secondary_display_rect;
        local_disp_area_ptr = &main_display_rect;
        break;
        
    case MMIVT_DISP_MODE_REMOTE_ONLY:
        remote_disp_area_ptr = &main_display_rect;
        break;
        
    case MMIVT_DISP_MODE_LOCAL_ONLY:
        local_disp_area_ptr = &main_display_rect;
        break;
        
    case MMIVT_DISP_MODE_PREVIEW:
        local_disp_area_ptr = &secondary_display_rect;
        break;

    default:
        //do nothing
        break;
    }
    
    ret_code = (MMIVT_REF_RET_E)DVT_MW_SetDisplayArea(remote_disp_area_ptr, local_disp_area_ptr);
#endif
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_ReplaceStart
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:开始替换
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_ReplaceStart(MMISET_VT_REPLACE_TYPE_E replace_type, BOOLEAN is_default)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    DVT_MW_REPLACEINFO_T replace_info = {0};
    BOOLEAN is_use_default_image = FALSE; //是否使用默认图片进行替换；
    MMISET_VT_REPLACE_FILE_INFO_T replace_file_info[MMISET_VT_REPLACE_MAX] = {0};

    if (replace_type >= MMISET_VT_REPLACE_MAX)
    {
        //SCI_TRACE_LOW:"MMIVT_REF_ReplaceStart err type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_406_112_2_18_3_6_8_72,(uint8*)"d", replace_type);
        return MMIVT_REF_RET_FAIL;
    }

    //SCI_TRACE_LOW:"MMIVT_REF_ReplaceStart replace_type:%d, is_default:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_410_112_2_18_3_6_8_73,(uint8*)"dd", replace_type, is_default);

    //设置是本端替换还是远端替换
    switch(replace_type) 
    {
    case MMISET_VT_REPLACE_LOCAL_IMAGE:
        replace_info.replace_object = DVT_MW_LOCAL_VIDEO;
        break;
    case MMISET_VT_REPLACE_REMOTE_IMAGE:
        replace_info.replace_object = DVT_MW_REMOTE_VIDEO;
        break;
    default:
        return MMIVT_REF_RET_FAIL;
        break;/*lint !e527*/
    }
    replace_info.rep_mode_v = DVT_MW_REP_MOD_FULL;


    //设置替换资源参数
    if (is_default )
    {
        //用户指定要用默认
        is_use_default_image = TRUE;
    }
    else
    {
        MMIAPISET_GetReplaceInfoFromNv((MMISET_VT_REPLACE_FILE_INFO_T*)replace_file_info);
        if (replace_file_info[replace_type].is_valid && replace_file_info[replace_type].file_name_len >0)/*lint !e661*/
        {
            //判断文件是否存在
            if (MMIAPIFMM_IsFileExist(replace_file_info[replace_type].file_name, replace_file_info[replace_type].file_name_len))
            {
                if (MMIAPIUDISK_UdiskIsRun())
                {
                    //U盘使用中（此时无法读文件）
                    is_use_default_image = TRUE;
                    ret_code = MMIVT_REF_RET_REPLACE_UDISK_IN_USE;
                }
                else
                {
                    is_use_default_image = FALSE;
                }
            }
            else
            {
                is_use_default_image = TRUE;
                ret_code = MMIVT_REF_RET_REPLACE_FILE_NOT_EXIST;
            }
        }
        else
        {
            is_use_default_image = TRUE;
        }
    }

    if (is_use_default_image)
    {
        MMIRES_DATA_INFO_T  file_info = {0}; /*lint !e64*/
        MMI_GetDataInfo(DATA_VT_REPLACE_DEFAULT, &file_info);
        replace_info.access_type = DVT_MW_FILE_ACCESS_MEM;
        replace_info.file_path.file_mem_addr.p_addr = file_info.data_ptr;
        replace_info.file_path.file_mem_addr.data_len = file_info.data_len;
        replace_info.file_path.file_mem_addr.file_fmt = DVT_MW_FILE_FMT_JPG;        
    }
    else
    {
        replace_info.access_type = DVT_MW_FILE_ACCESS_DISK;
        replace_info.file_path.file_disk_fullname.name_len = replace_file_info[replace_type].file_name_len;/*lint !e661*/
        replace_info.file_path.file_disk_fullname.p_file_fullname = replace_file_info[replace_type].file_name;/*lint !e661*/        
    }
#ifndef WIN32
    {
         DVT_MW_RET_E ref_result = DVT_MW_ReplaceStart(&replace_info);
         if (DVT_MW_RET_SUCCESS != ref_result)
         {
            ret_code = (MMIVT_REF_RET_E)ref_result;
         }
         else
         {
             //do nothing
         }
    }
#endif
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_ReplaceStop
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:停止替换
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_ReplaceStop(MMISET_VT_REPLACE_TYPE_E replace_type)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    uint32 replace_object = DVT_MW_LOCAL_VIDEO;
    //SCI_TRACE_LOW:"MMIVT_REF_ReplaceStop replace_type:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_504_112_2_18_3_6_8_74,(uint8*)"d", replace_type);
    if (replace_type >= MMISET_VT_REPLACE_MAX)
    {
        //SCI_TRACE_LOW:"MMIVT_REF_ReplaceStop err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_507_112_2_18_3_6_8_75,(uint8*)"");
        return MMIVT_REF_RET_FAIL;
    }

    switch(replace_type) 
    {
    case MMISET_VT_REPLACE_LOCAL_IMAGE:
        replace_object = DVT_MW_LOCAL_VIDEO;
        break;
    case MMISET_VT_REPLACE_REMOTE_IMAGE:
        replace_object = DVT_MW_REMOTE_VIDEO;
        break;
    default:
        return MMIVT_REF_RET_FAIL;
        break;/*lint !e527*/
    }    

#ifndef WIN32
    ret_code = (MMIVT_REF_RET_E) DVT_MW_ReplaceStop(replace_object);
#endif
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_RecordStart
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:开始录制
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_RecordStart(MMIVT_RECORD_TYPE_E record_type, MMI_STRING_T* record_path_str_ptr)
{
    DVT_MW_RECORDINFO_T record_info = {0};
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
 
    
    //SCI_TRACE_LOW:"MMIVT_REF_RecordStart record_type:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_540_112_2_18_3_6_8_76,(uint8*)"d", record_type);
    if (record_type >= MMIVT_RECORD_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"MMIVT_REF_RecordStart err type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_543_112_2_18_3_6_8_77,(uint8*)"d", record_type);
        return MMIVT_REF_RET_FAIL;
    }

    record_info.access_type = DVT_MW_FILE_ACCESS_DISK;
    record_info.file_path.file_disk_fullname.p_file_fullname = record_path_str_ptr->wstr_ptr;
    record_info.file_path.file_disk_fullname.name_len = record_path_str_ptr->wstr_len;

    switch(record_type) 
    {
    case MMIVT_RECORD_TYPE_RV_DA:
        record_info.record_object = DVT_MW_LOCAL_AUDIO|DVT_MW_REMOTE_AUDIO|DVT_MW_REMOTE_VIDEO;/*lint !e655*/
    	break;

    case MMIVT_RECORD_TYPE_RV:
        record_info.record_object = DVT_MW_REMOTE_VIDEO;
    	break;

    case MMIVT_RECORD_TYPE_DA:
        switch(s_record_audio) 
        {
        case 0:
            record_info.record_object = DVT_MW_LOCAL_AUDIO|DVT_MW_REMOTE_AUDIO;/*lint !e655*/
        	break;
        case 1:
            record_info.record_object = DVT_MW_REMOTE_AUDIO;
        	break;
        case 2:
            record_info.record_object = DVT_MW_LOCAL_AUDIO;
        	break;
        default:
            record_info.record_object = DVT_MW_LOCAL_AUDIO|DVT_MW_REMOTE_AUDIO;/*lint !e655*/
            break;
        }
        
    	break;

    default:
        record_info.record_object = DVT_MW_REMOTE_AUDIO|DVT_MW_REMOTE_VIDEO; /*lint !e655*/
        break;
    }
    

    
#ifndef WIN32
    ret_code = (MMIVT_REF_RET_E)DVT_MW_RecordStart(&record_info);
#endif
    //SCI_TRACE_LOW:"mmivt record start result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_590_112_2_18_3_6_8_78,(uint8*)"d", ret_code);
    
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_RecordStop
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:结束录制
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_RecordStop(void)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
#ifndef WIN32
    ret_code = (MMIVT_REF_RET_E)DVT_MW_RecordStop();
#endif
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_InitSensorParam
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:初始化sensor参数
/*****************************************************************************/
PUBLIC void MMIVT_REF_InitSensorParam(void)
{
    int32 temp_brightness_level = 0;
    int32 temp_contrast_level = 0;

    //获取底层亮度参数设置最大值
#ifndef WIN32
    DVT_MW_GetSensorParam(DVT_MW_SENSOR_EFFECT_BRIGHT, (void*)&temp_brightness_level);
#endif
    //SCI_TRACE_LOW:"mmivt ref bright max:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_621_112_2_18_3_6_8_79,(uint8*)"d", temp_brightness_level);
    if (temp_brightness_level > MMIVT_VIDEO_LEVEL_SEVEN)
    {
        s_sensor_brightness_max = temp_brightness_level;
    }
    else
    {
        s_sensor_brightness_max = MMIVT_VIDEO_LEVEL_SEVEN + 1;
    }

    //设置初始亮度
    MMIVT_REF_SetBrightness(MMIVT_VIDEO_LEVEL_FOUR);


    //获取底层对比度参数设置最大值
#ifndef WIN32
    DVT_MW_GetSensorParam(DVT_MW_SENSOR_EFFECT_CONTRAST, (void*)&temp_contrast_level);
#endif
    //SCI_TRACE_LOW:"mmivt ref contrast max:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_639_112_2_18_3_6_8_80,(uint8*)"d", temp_contrast_level);
    if (temp_contrast_level > MMIVT_VIDEO_LEVEL_SEVEN)
    {
        s_sensor_contrast_max = temp_contrast_level;
    }
    else
    {
        s_sensor_contrast_max = MMIVT_VIDEO_LEVEL_SEVEN + 1;
    }

    //设置初始对比度
    MMIVT_REF_SetContrast(MMIVT_VIDEO_LEVEL_FOUR);
}

/*****************************************************************************/
//  Description : MMIVT_REF_SetLcdBrightness
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:设置LCD 亮度
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetLcdBrightness(MMIVT_VIDEO_LEVEL_E lcd_brightness_level)
{
    //SCI_TRACE_LOW:"MMIVT_REF_SetLcdBrightness:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_659_112_2_18_3_6_9_81,(uint8*)"d", lcd_brightness_level);
    MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, lcd_brightness_level + 1);
    return MMIVT_REF_RET_SUCCESS;
}

/*****************************************************************************/
//  Description : MMIVT_REF_SetContrast
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:设置本端图像对比度
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetContrast(MMIVT_VIDEO_LEVEL_E contrast_level)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    int32 temp_contrast = s_sensor_contrast_max * (contrast_level+1) / (MMIVT_VIDEO_LEVEL_SEVEN + 1) - 1; 
    
    //SCI_TRACE_LOW:"MMIVT_REF_SetContrast:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_673_112_2_18_3_6_9_82,(uint8*)"d", contrast_level);
#ifndef WIN32
    ret_code = (MMIVT_REF_RET_E)DVT_MW_SetSensorParam(DVT_MW_SENSOR_EFFECT_CONTRAST, (void*)&temp_contrast);
#endif
    s_sensor_cur_contrast = contrast_level;
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_SetBrightness
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:设置本端图像亮度
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetBrightness(MMIVT_VIDEO_LEVEL_E brightness_level)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    int32 temp_brightness = s_sensor_brightness_max * (brightness_level+1) / (MMIVT_VIDEO_LEVEL_SEVEN + 1) - 1; 

    //SCI_TRACE_LOW:"MMIVT_REF_SetBrightness:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_690_112_2_18_3_6_9_83,(uint8*)"d", brightness_level);
#ifndef WIN32
    ret_code = (MMIVT_REF_RET_E)DVT_MW_SetSensorParam(DVT_MW_SENSOR_EFFECT_BRIGHT, (void*)&temp_brightness);
#endif
    s_sensor_cur_brightness = brightness_level;
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIVT_REF_RotateLocal
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:旋转本端图像
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_RotateLocal(MMIVT_IMAGE_ROTATE_STATUS_E rotate_status, MMIVT_DISP_MODE_E disp_mode)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
#ifndef WIN32 
    DVT_MW_DISP_RECT_T *local_disp_area_ptr = PNULL;
    DVT_MW_DISP_RECT_T *remote_disp_area_ptr = PNULL;
    
    main_display_rect.lcd_block_layer = MAIN_BLOCK_LAYER;
    secondary_display_rect.lcd_block_layer = SECONDARY_BLOCK_LAYER;
    
    switch(disp_mode)
    {
    case MMIVT_DISP_MODE_REMOTE_LOCAL:
        remote_disp_area_ptr = &main_display_rect;
        local_disp_area_ptr = &secondary_display_rect;
        break;
        
    case MMIVT_DISP_MODE_LOCAL_REMOTE:
        remote_disp_area_ptr = &secondary_display_rect;
        local_disp_area_ptr = &main_display_rect;
        break;
        
    case MMIVT_DISP_MODE_REMOTE_ONLY:
        remote_disp_area_ptr = &main_display_rect;
        break;
        
    case MMIVT_DISP_MODE_LOCAL_ONLY:
        local_disp_area_ptr = &main_display_rect;
        break;

    case MMIVT_DISP_MODE_PREVIEW:
        local_disp_area_ptr = &secondary_display_rect;
        break;
        
    default:
        //do nothing
        break;
    }

    if (PNULL != local_disp_area_ptr && PNULL != remote_disp_area_ptr)
    {
        local_disp_area_ptr->rotation =  (DVT_MW_ROTATEANGLE_E)rotate_status;
        remote_disp_area_ptr->rotation = DVT_MW_ROTATE_0;
    }
    else
    {
        return MMIVT_REF_RET_FAIL;
    }
    
    ret_code = (MMIVT_REF_RET_E)DVT_MW_SetDisplayArea(remote_disp_area_ptr, local_disp_area_ptr);
#endif
    return ret_code;    
}

/*****************************************************************************/
//  Description : MMIVT_REF_SwitchCamera
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:切换本端摄像头
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SwitchCamera(MMIVT_CAMERA_ID_E camera_id)
{
    MMIVT_REF_RET_E ret_code = MMIVT_REF_RET_SUCCESS;
    DVT_MW_SENSOR_ID_E mw_sensor_id = DVT_MW_SENSOR_NONE;
    DVT_MW_SENSOR_PARAM_T sensor_param = {0};    

    //SCI_TRACE_LOW:"MMIVT_REF_SwitchCamera:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_KERNEL_REFADAPTER_766_112_2_18_3_6_9_84,(uint8*)"d", camera_id);

    switch(camera_id) 
    {
    case MMIVT_CAMERA_ID_MAIN:
        mw_sensor_id = DVT_MW_SENSOR_MAIN;
    	break;

    case MMIVT_CAMERA_ID_SECONDARY:
        mw_sensor_id = DVT_MW_SENSOR_SUB;
        break;

    default:
        return MMIVT_REF_RET_INVALID_PARAM;
    }
    sensor_param.sensor_id = mw_sensor_id;
    sensor_param.screen2sensor_angle = s_camera_init_angle[sensor_param.sensor_id];
    
#ifndef WIN32
    ret_code = (MMIVT_REF_RET_E)DVT_MW_SelectSensor_Ex(&sensor_param);

    //resume sensor parameter
    if (MMIVT_REF_RET_SUCCESS == ret_code)
    {
        MMIVT_REF_SetBrightness(s_sensor_cur_brightness);
        MMIVT_REF_SetContrast(s_sensor_cur_contrast);
    }
#endif
    
    return ret_code;
}

/*****************************************************************************/
//  Description : MMIAPIVT_SetRecordAudioType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:工程模式：VT录制中录制混音项可以在工程模式里面修改
/*****************************************************************************/
PUBLIC void MMIAPIVT_SetRecordAudioType(uint16 record_audio_type)
{
    s_record_audio = record_audio_type;
}

/*****************************************************************************/
//  Description : MMIAPIVT_GetRecordAudioType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:工程模式：VT录制中录制混音项可以在工程模式里面修改
/*****************************************************************************/
PUBLIC uint16 MMIAPIVT_GetRecordAudioType(void)
{
    return s_record_audio;
}



#endif //VT_SUPPORT
    
#ifdef   __cplusplus
    }
#endif
    
