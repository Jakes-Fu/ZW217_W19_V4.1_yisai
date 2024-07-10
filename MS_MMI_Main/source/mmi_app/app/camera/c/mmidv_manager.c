/*****************************************************************************
** File Name:      mmidv_manager.c                                  *
** Author:                                                                   *
** Date:           2009/7/2                                                 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ref interface               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/7/2    hui.zhao      Create
******************************************************************************/
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmidv_manager.h"
#include "mmidc_setting.h"
#include "mmi_position.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmidc_save.h"
#include "mmidisplay_data.h"    
#include "mmicamera_manager.h"
//#include "dal_jpeg.h"
#include "dal_recorder.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "sci_api.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"    
#include "dal_dcamera.h"
#include "mmidc_setting.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define AUDIO_CHANNEL						1
#define AUDIO_SAMPLE_RATE					8000

#define VIDEO_128X96_HEIGHT				96
#define VIDEO_128X96_WIDTH					128
#define VIDEO_160X128_HEIGHT				128
#define VIDEO_160X128_WIDTH					160
#define VIDEO_176X144_HEIGHT				144
#define VIDEO_176X144_WIDTH					176
#define VIDEO_240X176_HEIGHT				176
#define VIDEO_240X176_WIDTH					240
#define VIDEO_320X240_HEIGHT				240
#define VIDEO_320X240_WIDTH					320
#define VIDEO_352X288_HEIGHT				288
#define VIDEO_352X288_WIDTH					352
/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
static BOOLEAN					s_dv_is_open = FALSE;
static BOOLEAN					s_dv_is_preview = FALSE;
//static BOOLEAN					s_dv_is_record = FALSE;
static BOOLEAN					s_has_been_record = FALSE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init dv preview param
//	Global resource dependence : none
//  Author: zhaohui
//	Note:
/************************** ***************************************************/
LOCAL int32 InitDVPreviewParam(DRECORDER_PREVIEW_PARAM_T* record_param_ptr);

/*****************************************************************************/
// 	Description : init record dv param
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
/*LOCAL void InitDVRecordParam(
								DRECORDER_START_RECORD_T* recorder_start_t_ptr,
								DRECORDER_END_CALLBACK RecordEndCallback,
								uint16 *filename
								);*/
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : start to run the flow of dv
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_Open( void )
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
#ifdef UI_MULTILAYER_SUPPORT	
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
#endif
    //GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
    
    //SCI_TRACE_LOW:"[MMIRecord]: OpenDV = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_87_112_2_18_2_10_4_794,(uint8*)"d", s_dv_is_open);
    if(!s_dv_is_open)
    {
        ret = DRECORDER_SelectSrcDevice(DRECORDER_SRC_DEV_DCAMERA);
        //SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_SelectSrcDevice = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_91_112_2_18_2_10_4_795,(uint8*)"d", ret);
        ret = DRECORDER_Open();
        //SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_Open = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_93_112_2_18_2_10_4_796,(uint8*)"d", ret);
        if(DRECORDER_SUCCESS == ret)
        {
            s_dv_is_open = TRUE;	      
#if 0            
			GUIBLOCK_ClearRect(&dev_info);
#ifdef WIN32
            GUIBLOCK_SetType(0, MMI_BLACK_COLOR, GUIBLOCK_TYPE_OSD, &dev_info);
#else
            GUIBLOCK_SetType(255, MMI_BLACK_COLOR, GUIBLOCK_TYPE_COMMON, &dev_info);
#endif
            
            GUIBLOCK_SetRect(full_rect, &dev_info);
            //GUIBLOCK_ClearByColor(MMI_BLACK_COLOR,  &dev_info);	
            LCD_FillRect(&dev_info, full_rect, MMI_BLACK_COLOR);
            GUIBLOCK_Enable(&dev_info);
#endif
#ifdef UI_MULTILAYER_SUPPORT	
			UILAYER_SetLayerColorKey(&dev_info, TRUE, MMI_BLACK_COLOR);
#endif		
		}
        else
        {
            s_dv_is_open = FALSE;
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : start to preview video
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StartPreview(void)
{
	DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
	DRECORDER_PREVIEW_PARAM_T record_param = {0};

	//SCI_TRACE_LOW:"[MMIRecord]: StartDVPreview = %d, = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_126_112_2_18_2_10_4_797,(uint8*)"dd", s_dv_is_preview, s_dv_is_open);

	if(!s_dv_is_preview && s_dv_is_open)
	{				
		InitDVPreviewParam(&record_param);

		ret = DRECORDER_StartPreview(&record_param);
		//SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_StartPreview = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_133_112_2_18_2_10_4_798,(uint8*)"d", ret);
		if(DRECORDER_SUCCESS == ret)
		{
			s_dv_is_preview = TRUE;
		}		
		else
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

/*****************************************************************************/
// 	Description : init dv preview param
//	Global resource dependence : none
//  Author: zhaohui
//	Note:
/************************** ***************************************************/
LOCAL int32 InitDVPreviewParam(DRECORDER_PREVIEW_PARAM_T* record_param_ptr)
{
    BOOLEAN result = FALSE;    
    
    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0} ;
    GUILCD_COORDINATE_PARAM_T  physical_coordinate = {0} ;
    LCD_ANGLE_E lcd_angle = LCD_ANGLE_0;
    
    if (PNULL == record_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIRecord]: InitDVPreviewParam record_param_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_161_112_2_18_2_10_4_799,(uint8*)"");
        return result;
    }
    
    record_param_ptr->LcdId = GUI_MAIN_LCD_ID;
    record_param_ptr->LcdBlkId = GUI_BLOCK_MAIN;	
    record_param_ptr->display_mode = DRECORDER_DISP_CUSTOMIZE;	
    record_param_ptr->image_format =  MMIAPIENG_GetSensorMode();
    
    
    //rotation
    lcd_angle = GUILCD_GetLogicAngle(record_param_ptr->LcdId);
    //SCI_TRACE_LOW:"[MMIdv_manager]:  cur_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_173_112_2_18_2_10_4_800,(uint8*)"d", GUILCD_GetLogicAngle(record_param_ptr->LcdId));
    
    //lcd heigth width  
    result = GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&logic_coordinate.lcd_width, &logic_coordinate.lcd_height);
    if(!result)
    {
        //SCI_TRACE_LOW:"[MMIdv_manager]:  GUILCD_GetLogicWidthHeight, result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_179_112_2_18_2_10_4_801,(uint8*)"d", result);
        return result;
    }
    
    //rect
    logic_coordinate.rect = MMITHEME_GetFullScreenRect();
    
    //坐标转换
    result = GUILCD_ConvertLogicToPhysicalCoordinate( record_param_ptr->LcdId, 
        &logic_coordinate, &physical_coordinate); 
    if(!result)
    {
        //SCI_TRACE_LOW:"[MMIdv_manager]:  GUILCD_ConvertLogicToPhysicalCoordinate, result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_191_112_2_18_2_10_4_802,(uint8*)"d", result);
        return result;
    }
    
    //preview rect
    
    record_param_ptr->disp_rect.x = physical_coordinate.rect.left;
    record_param_ptr->disp_rect.y = physical_coordinate.rect.top;
    record_param_ptr->disp_rect.dx = physical_coordinate.rect.right- physical_coordinate.rect.left + 1;
    record_param_ptr->disp_rect.dy = physical_coordinate.rect.bottom - physical_coordinate.rect.top + 1;
    
    record_param_ptr->disp_rect.x =  (record_param_ptr->disp_rect.x  >> 2) << 2 ;
    record_param_ptr->disp_rect.y =  (record_param_ptr->disp_rect.y  >> 2) << 2 ;
    record_param_ptr->disp_rect.dx =  (record_param_ptr->disp_rect.dx  >> 2) << 2 ;
    record_param_ptr->disp_rect.dy=  (record_param_ptr->disp_rect.dy >> 2) << 2 ;
    
    //SCI_TRACE_LOW:"[MMIdv_manager]:  record_param_ptr->disp_rect :x = %d, y = %d, w = %d, h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_209_112_2_18_2_10_4_803,(uint8*)"dddd", record_param_ptr->disp_rect.x, record_param_ptr->disp_rect.y ,record_param_ptr->disp_rect.dx , record_param_ptr->disp_rect.dy );
    
    
    record_param_ptr->DisplayRotateAngle = lcd_angle;
    
    record_param_ptr->target_rect = record_param_ptr->disp_rect;
    
    //SCI_TRACE_LOW:"[MMIdv_manager]: InitDVPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_216_112_2_18_2_10_4_804,(uint8*)"");

    return 0;
}

/*****************************************************************************/
// 	Description : stop dv preview
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StopPreview(void)
{
	DRECORDER_RETURN_E ret = 0;	

	//SCI_TRACE_LOW:"[MMIRecord]: StopPreview = %d, = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_229_112_2_18_2_10_4_805,(uint8*)"dd", s_dv_is_preview, s_dv_is_open);
	//MMIDC_FreeImageBuf();
	if(s_dv_is_preview && s_dv_is_open)
	{
	    ret = DRECORDER_StopPreview();
	    //SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_StopPreview = %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_234_112_2_18_2_10_4_806,(uint8*)"d", ret);
	    if(DRECORDER_SUCCESS == ret)
	    {
	        s_dv_is_preview = FALSE;
	    }
	}

	return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : close dv
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_Close(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    
    //SCI_TRACE_LOW:"[MMIRecord]: CloseDV = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_252_112_2_18_2_10_4_807,(uint8*)"d", s_dv_is_open);
    
    if(s_dv_is_open)
    {
        //MMIDC_FreeImageBuf();
        ret = DRECORDER_Close();
#if 0        
		GUIBLOCK_ResetMainType();
#endif        
		//SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_Close = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_259_112_2_18_2_10_4_808,(uint8*)"d", ret);
        if(DRECORDER_SUCCESS == ret)
        {
            s_dv_is_open = FALSE;
        }
    }
    
    if(s_has_been_record)
    {
        //AUD_SetDevMode(s_audio_dev_mode);
        s_has_been_record = FALSE;
    }
    
    return (DCAMERA_OP_SUCCESS == ret);	
}

/*****************************************************************************/
// 	Description : start record dv
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StartRecord(DRECORDER_END_CALLBACK RecordEndCallback,uint16* filename)
{
	DRECORDER_START_RECORD_T recorder_start_t = {0};
	DRECORDER_RETURN_E			ret = 0;	

	MEDIA_AUDIO_INFO_T	audio_info = {0};
	MEDIA_VIDEO_INFO_T	video_info = {0};
	 
	//SCI_TRACE_LOW:"[MMIRecord]: StartRecord start"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_287_112_2_18_2_10_4_809,(uint8*)"");

    if (PNULL == filename)
    {
        //SCI_TRACE_LOW:"[MMIRecord]: StartRecord start filename is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_291_112_2_18_2_10_4_810,(uint8*)"");
        return FALSE;
    }

	//initialize record param
    MMIAPISET_InitAudioDevice(AUD_MAX_SPEAKER_VOLUME);//是否有必要

	s_has_been_record = TRUE;

	/* set audio info */
	audio_info.audio_support	= 1;//默认为支持audio
	audio_info.channels		= AUDIO_CHANNEL;
	audio_info.sample_rate	= AUDIO_SAMPLE_RATE;
	audio_info.audio_type = AUDIO_TYPE_AMR;//3//FILE_TYPE_3GP

	/*set video info*/
	video_info.video_type	= VIDEO_TYPE_MJPEG;//FILE_TYPE_3GP
	video_info.frame_rate	= MMIENG_DV_FRAME_FRE_7HZ;//
	video_info.height		= 240;//VIDEO_320X240_HEIGHT;
	video_info.width		= 320;//VIDEO_320X240_WIDTH;
	video_info.video_support = TRUE;	
	
	recorder_start_t.audio_info_ptr		= &audio_info;
	recorder_start_t.video_info_ptr		= &video_info;
	recorder_start_t.frame_callback		= PNULL;
	recorder_start_t.end_callback		= (DRECORDER_END_CALLBACK)RecordEndCallback;

	//默认为3GP文件格式
	recorder_start_t.file_type			= DRECORDER_FILETYPE_3GP;
	recorder_start_t.file_name_ptr		= (uint8*)filename;
	recorder_start_t.file_name_byte_len 	= MMIAPICOM_Wstrlen(filename) * 2;
	recorder_start_t.file_limit_size 		= 0;    

	//SCI_TRACE_LOW:"[MMIRecord]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_326_112_2_18_2_10_4_811,(uint8*)"dddddd", recorder_start_t.file_name_ptr, recorder_start_t.file_name_byte_len,recorder_start_t.file_type, video_info.height, video_info.width, video_info.video_type);

	//start record
	ret = DRECORDER_StartRecord(&recorder_start_t);

	//SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_StartRecord = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_331_112_2_18_2_10_4_812,(uint8*)"d", ret);

	if(DRECORDER_SUCCESS != ret)
	{
	    return FALSE;
	}
	else
	{
	    //SCI_TRACE_LOW:"[MMIRecord]: StartRecord - return success !"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_339_112_2_18_2_10_4_813,(uint8*)"");
	}

//	s_dv_is_record = FALSE;

	return TRUE;
}

/*****************************************************************************/
// 	Description : init record dv param
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
/*LOCAL void InitDVRecordParam(
								DRECORDER_START_RECORD_T* recorder_start_t_ptr,
								DRECORDER_END_CALLBACK RecordEndCallback,
								uint16 *filename
								)
{
//    DRECORDER_START_RECORD_T recorder_start_t = {0};
	MEDIA_AUDIO_INFO_T	audio_info = {0};
	MEDIA_VIDEO_INFO_T	video_info = {0};

	SCI_ASSERT(PNULL != recorder_start_t_ptr);
	SCI_ASSERT(PNULL != filename);

	 set audio info 
	audio_info.audio_support	= 1;//(AUDIO_SETTING_ON == s_dc_setting_info.audio_setting);	
	audio_info.channels		= AUDIO_CHANNEL;
	audio_info.sample_rate	= AUDIO_SAMPLE_RATE;

	audio_info.audio_type = AUDIO_TYPE_AMR;//3//FILE_TYPE_3GP

	video_info.video_type	= VIDEO_TYPE_MJPEG;//FILE_TYPE_3GP

	video_info.frame_rate	= MMIENG_DV_FRAME_FRE_7HZ;//(uint32)MMIAPIENG_GetDVFrameFre();

	video_info.height		= 240;//VIDEO_320X240_HEIGHT;
	video_info.width		= 320;//VIDEO_320X240_WIDTH;

	video_info.video_support = TRUE;	
	recorder_start_t_ptr->audio_info_ptr		= &audio_info;
	recorder_start_t_ptr->video_info_ptr		= &video_info;
	recorder_start_t_ptr->frame_callback		= PNULL;
	recorder_start_t_ptr->end_callback		= (DRECORDER_END_CALLBACK)RecordEndCallback;

	//默认为3GP文件格式
	recorder_start_t_ptr->file_type		= DRECORDER_FILETYPE_3GP;
	recorder_start_t_ptr->file_name_ptr		 = (uint8*)filename;
	recorder_start_t_ptr->file_name_byte_len = MMIAPICOM_Wstrlen(filename) * 2;
	recorder_start_t_ptr->file_limit_size = 0;    

	//SCI_TRACE_LOW:"[MMIRecord]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_392_112_2_18_2_10_4_814,(uint8*)"dddddd", recorder_start_t_ptr->file_name_ptr, recorder_start_t_ptr->file_name_byte_len,recorder_start_t_ptr->file_type, video_info.height, video_info.width, video_info.video_type);

	DRECORDER_StartRecord(recorder_start_t_ptr);
}*/

/*****************************************************************************/
// 	Description : dc pause record
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_PauseRecord(void)
{
	DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

	ret = DRECORDER_Pause();//该接口底层还未调试好

	//SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_StopRecord return: %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_407_112_2_18_2_10_4_815,(uint8*)"d", ret);
	
	return (DRECORDER_SUCCESS == ret);
}


/*****************************************************************************/
// 	Description : dc resume record
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_ResumeRecord(void)
{
	DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

	ret = DRECORDER_Resume();//该接口底层还未调试好

	return (DRECORDER_SUCCESS == ret);
}


/*****************************************************************************/
// 	Description : stop record video
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StopRecord(void)
{
	DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

	ret = DRECORDER_StopRecord();

	//SCI_TRACE_LOW:"[MMIRecord]: DRECORDER_StopRecord return: %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDV_MANAGER_437_112_2_18_2_10_4_816,(uint8*)"d", ret);

	return (DRECORDER_SUCCESS == ret);
}
