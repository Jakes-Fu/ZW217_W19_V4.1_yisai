/*****************************************************************************
** File Name:      mmidc_workmode.c                                          *
** Author:                                                                   *
** Date:           01/15/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera work mode                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012      robert.wang          Creat                                   *
******************************************************************************/

#define _MMIDC_WORKMODE_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_camera_trc.h"
#if defined(CAMERA_SUPPORT) && defined(MMIDC_F_WORK_MODE)

#ifdef WIN32 
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_default.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"

//this module
#include "mmidc_main.h"
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmidc_export.h"
#include "mmidc_workmode.h"
#include "mmidc_option.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL MMIDC_WORK_INFO_T *s_dc_work_info_ptr = PNULL;


/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : alloc work info memory
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_WORK_INFO_T* AllocWorkInfo(void)
{
    uint32 byte_size = 0;

    byte_size = sizeof(*s_dc_work_info_ptr);
    if (PNULL == s_dc_work_info_ptr)
    {
        s_dc_work_info_ptr = SCI_ALLOCA(byte_size);
    }
    if (PNULL != s_dc_work_info_ptr)
    {
        SCI_MEMSET(s_dc_work_info_ptr, 0x00 ,byte_size);        
    }

    //SCI_TRACE_LOW:"[MMIDC] AllocWorkInfo s_dc_work_info_ptr=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_75_112_2_18_2_10_2_779,(uint8*)"d",s_dc_work_info_ptr);
    
    return s_dc_work_info_ptr;
}

/*****************************************************************************/
//  Description : alloc work info memory
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void FreeWorkInfo(void)
{
    //SCI_TRACE_LOW:"[MMIDC] FreeWorkInfo s_dc_work_info_ptr=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_87_112_2_18_2_10_2_780,(uint8*)"d",s_dc_work_info_ptr);
                 
    if (PNULL != s_dc_work_info_ptr)
    {
        SCI_FREE(s_dc_work_info_ptr);
    }
}

/*****************************************************************************/
//  Description : get work info ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_WORK_INFO_T* GetWorkInfoPtr(void)
{
    return s_dc_work_info_ptr;
}

/*****************************************************************************/
//  Description : check and set work param
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_WORK_E GetWorkMode(void)
{
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();
    MMIDC_WORK_E work_mode = 0;
    
    if (PNULL != work_info_ptr)
    {
        work_mode = work_info_ptr->work_param.work_mode;
    }

    return work_mode;
}

/*****************************************************************************/
//  Description : release dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitWorkInfoParam(MMIDC_WORK_PARAM_T *work_param_ptr //in
                                    )
{
    BOOLEAN ret = FALSE;
    MMIDC_WORK_INFO_T* info_ptr = PNULL;

    if (PNULL != work_param_ptr)
    {
        info_ptr = AllocWorkInfo();
        if (PNULL != info_ptr)
        {
            SCI_MEMCPY(&info_ptr->work_param, work_param_ptr, sizeof(*work_param_ptr));
            info_ptr->is_enable = TRUE;
            info_ptr->err_ret = MMIDC_WORK_ERROR;
            info_ptr->is_check_param = TRUE;

            ret = TRUE;
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] InitWorkInfoParam ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_144_112_2_18_2_10_2_781,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : release dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_ReleaseWorkMode(void)
{
    
    MMIDC_ProcessWorkCallback();

    FreeWorkInfo();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ReleaseWorkMode"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_160_112_2_18_2_10_2_782,(uint8*)"");
}

/*****************************************************************************/
//  Description : start dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_WORK_ERROR_E MMIAPIDC_StartWork(MMIDC_WORK_PARAM_T *work_param_ptr /*in*/
                                                    )
{
    MMIDC_WORK_ERROR_E ret = MMIDC_WORK_ERROR;
    BOOLEAN result = FALSE;

    if (PNULL == work_param_ptr)
    {
        return ret;
    }

#ifdef VT_SUPPORT//Bug337129
    if (MMIAPIVT_IsVtCalling())
    {
        MMIPUB_OpenAlertWarningWin(TXT_VT_VIDEOCALLING);        
        return ret;
    }
#endif      

    if(MMIAPIDC_IsOpened())
    {
        MMIAPIDC_Exit();
    }
    
    //alloc memory and init param
    result = InitWorkInfoParam(work_param_ptr);

    if (result)
    {
        MMIDCSRV_WORK_PARAM_T work_param = {0};
    

        if (MMIDC_WORK_DC == GetWorkMode())
        {
            work_param.work_mode = MMIDCSRV_MODE_DC;
            
            work_param.dc.mem_cfg = MMIDCSRV_MEM_DC_MINI;
            work_param.dc.max_photo_size = work_param_ptr->param.dc.max_photo_size;
            work_param.dc.disp_mode = DCAMERA_DISP_FULLVIEW;

            work_param.dv.mem_cfg = MMIDCSRV_MEM_DV_NORMAL;
            work_param.dv.max_video_size = WORK_VIDEO_SIZE_MAX;
            work_param.dv.disp_mode = DRECORDER_DISP_MODE_MAX;
            
        }
        else
        {
            work_param.work_mode = MMIDCSRV_MODE_DV;            
            
            work_param.dv.mem_cfg = MMIDCSRV_MEM_DV_MINI;
            work_param.dv.max_video_size = work_param_ptr->param.dv.max_video_size;
            work_param.dv.disp_mode = DRECORDER_DISP_FULLVIEW;

            work_param.dc.mem_cfg = MMIDCSRV_MEM_DC_NORMAL;
            work_param.dc.max_photo_size = WORK_PHOTO_SIZE_MAX;
            work_param.dc.disp_mode = DCAMERA_DISP_MODE_MAX;            
        }
        
        result = MMIAPIDCSRV_OpenMainWin(&work_param);    
    }
    
    if (result)
    {
        ret = MMIDC_WORK_SUCCESS;
    }
    else
    {
        FreeWorkInfo();
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_StartWork ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_203_112_2_18_2_10_2_783,(uint8*)"d",ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : is dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsWorkMode(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();

    if (PNULL != work_info_ptr)
    {
        ret = work_info_ptr->is_enable;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_IsWorkMode ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_222_112_2_18_2_10_2_784,(uint8*)"d", ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : set work error no.
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetWorkError(MMIDC_WORK_ERROR_E err)
{
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();

    if (PNULL != work_info_ptr)
    {
        work_info_ptr->err_ret = err;
    }
}

/*****************************************************************************/
//  Description : get max photo size of work mode in dc module
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_PHOTO_SIZE_E MMIDC_GetMaxPhotoSizeOfWorkMode(void)
{
    MMIDC_PHOTO_SIZE_E max_photo_size = 0;
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();

    if (PNULL != work_info_ptr)
    {
        max_photo_size = work_info_ptr->max_photo_size;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMaxPhotoSizeOfWorkMode max_photo_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_554_112_2_18_2_10_3_789,(uint8*)"d",max_photo_size);
    
    return max_photo_size;
}

/*****************************************************************************/
//  Description : save work mode file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveWorkFile(void)
{
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    wchar *file_name_ptr = PNULL;
    uint16 file_name_len = 0;

    BOOLEAN is_success = FALSE;

    if (PNULL != work_info_ptr)
    {
        if (MMIDC_WORK_DC == GetWorkMode())
        {
            photo_info_ptr = MMIDC_GetCurrentPhotoInfo();
            if (PNULL != photo_info_ptr)
            {
                wchar *save_path_ptr = MMIDC_GetPhotoSavePath();
                
                file_name_ptr = work_info_ptr->file_name;
                
				if(PNULL != save_path_ptr)
				{
					file_name_len = MMIAPIDC_CombinePathName(file_name_ptr,
														MMIFILE_FULL_PATH_MAX_LEN,\
														save_path_ptr,
														photo_info_ptr->ucs2_name
														);   
				}

                if (file_name_len > 0)
                {
                    is_success = TRUE;
                }
            }
        }
        else
        {
            file_name_ptr = MMIDC_GetVideoFullFileName();
            file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);

            if (file_name_len > 0)
            {
                is_success = TRUE;
            }
        }

        if (is_success)
        {
            if (PNULL != file_name_ptr)
            {
                work_info_ptr->file_name_len = file_name_len;
                MMI_WSTRNCPY(work_info_ptr->file_name, work_info_ptr->file_name_len,\
                             file_name_ptr, file_name_len,
                             file_name_len
                             );

                MMIDC_SetWorkError(MMIDC_WORK_SUCCESS);
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SaveWorkFile is_succes=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_621_112_2_18_2_10_3_790,(uint8*)"d",is_success);
}

/*****************************************************************************/
//  Description : process work callback
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ProcessWorkCallback(void)
{
    int32 ret = 0;
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();
    
    MMIDC_WORK_CALLBACK callback_ptr = PNULL;
    MMIDC_WORK_ERROR_E err_ret = MMIDC_WORK_ERROR;
    MMIDC_WORK_DATA_INFO_T data_info = {0};
    MMIDC_WORK_E work_mode = 0;
    
    if (PNULL != work_info_ptr)
    {
        callback_ptr = work_info_ptr->work_param.work_callback;
        if (PNULL != callback_ptr)
        {
            err_ret = work_info_ptr->err_ret;

            work_mode = GetWorkMode();
            data_info.work_mode = work_mode;
            data_info.file_name = work_info_ptr->file_name;
            data_info.file_name_len = work_info_ptr->file_name_len;
            
            ret = callback_ptr(err_ret, &data_info);
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_WorkCallback ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_654_112_2_18_2_10_3_791,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : get max video size of work mode in dc module
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC VIDEO_SIZE_E MMIDC_GetMaxVideoSizeOfWorkMode(void)
{
    VIDEO_SIZE_E max_video_size = 0;
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();

    if (PNULL != work_info_ptr)
    {
        max_video_size = work_info_ptr->max_video_size;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetMaxVideoSizeOfWorkMode max_video_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_674_112_2_18_2_10_3_792,(uint8*)"d",max_video_size);
    
    return max_video_size;
}

/*****************************************************************************/
//  Description : get limited video file size of work mode in dc module
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetLimitedVideoFileSizeOfWorkMode(void)
{
    uint32 file_size = 0;
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();

    if (PNULL != work_info_ptr)
    {
        file_size = work_info_ptr->work_param.param.dv.max_file_size;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetLimitedVideoFileSizeOfWorkMode file_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_WORKMODE_694_112_2_18_2_10_3_793,(uint8*)"d",file_size);
    
    return file_size;
}

/*****************************************************************************/
//  Description : is check work param
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsCheckWorkParam(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();

    if (PNULL != work_info_ptr)
    {
        ret = work_info_ptr->is_check_param;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_IsCheckWorkParam ret=%d", ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : set work param flag
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCheckWorkParamFlag(BOOLEAN ret)
{
    MMIDC_WORK_INFO_T *work_info_ptr = GetWorkInfoPtr();

    if (PNULL != work_info_ptr)
    {
        work_info_ptr->is_check_param = ret;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetCheckWorkParamFlag ret=%d", ret);
}
#endif //#ifdef CAMERA_SUPPORT

