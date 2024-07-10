#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef CAMERA_SUPPORT
#include "os_api.h"
//#include "dal_jpeg.h"
#include "dal_recorder.h"
#include "dal_dcamera.h"
#include "mmi_position.h"
#include "guiblock.h"
#include "block_mem.h"
#include "mmi_text.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "mmi_image.h"

//other module
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifmm_export.h"
#include "sci_api.h"
#include "guires.h"
#include "dal_lcd.h"
#include "mmiudisk_export.h"
#include "sfs.h"
#include "mmidisplay_data.h"
#include "dal_recorder.h"
#include "mmisd_export.h"

//local module
#include "mmidc_camera_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_save.h"
#include "mmidc_gui.h"
#include "mmidc_setting.h"
#include "mmidc_display.h"
#include "mmidc_option.h"
#ifdef MMI_CAMERA_F_PHOTO_DATE
#include "mmidc_photodate.h"
#endif
#include "mmidc_main.h"

#ifdef MMIDC_DESKTOP_PDA
#include "mmidc_guipda.h"
#endif

#ifdef MMIDC_DESKTOP_ICON_BAR
#include "mmidc_guiiconbar.h"
#endif


#ifdef MMIDC_F_WORK_MODE
#include "mmidc_workmode.h"
#endif

#include "mmimultim_image.h"


#ifdef MMIDC_F_U_CAMERA
#include "mmidc_imageprocess.h"   
#endif

#if defined(MMIDC_F_SMILE_AUTO_PHOTO)
#include "mmidc_smileautophoto.h"
#endif

#ifdef MMIDC_F_N_IN_ONE
#include "mmidc_ninone.h"
#endif

#include "mmidc_flow.h"
#include "mmidc_srv.h"
#ifdef MOTION_SENSOR_TYPE 
#include "mmk_gsensor.h"
#endif

#include "mmisrvaud_util.h"
#include "isp_service.h"
#include "img_transform.h"
#include "sensor_cfg.h"
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
#include "dal_display.h"
#endif

#ifdef BAIDU_DRIVE_SUPPORT
#include "mmibaidu_export.h"
#include "mmibaidu_id.h"
#endif

#define AUDIO_BIT_RATE						4750
#define AUDIO_CHANNEL						1
#define	AUDIO_SAMPLE_RATE					8000


#define DC_FRAME_INTERVAL       0
#if defined(PLATFORM_SC6600L) || defined(DC_SC6800H)
#define MMIDC_MUTEX_TIMEOUT                 5000
#else
#define MMIDC_MUTEX_TIMEOUT                 1000
#endif

typedef struct 
{
    BOOLEAN photo_validate[MAX_DC_BURST_PHOTO_NUMBER];
    int32   captured_photo_num;
    int32   photo_total;
}DC_PHOTO_CAPTURE_INFO;

typedef BOOLEAN (*DCFLOWFUNCTION)(void);


static BOOLEAN					s_dc_is_preview = FALSE;
static BOOLEAN					s_dc_is_open = FALSE;
static BOOLEAN					s_is_capture = FALSE;
static BOOLEAN					s_is_review = FALSE;
static BOOLEAN					s_dv_is_preview = FALSE;
static BOOLEAN					s_dv_is_open = FALSE;
static BOOLEAN					s_dv_is_record = FALSE;

static DC_FLOW_MODE_E			s_dc_current_flow_mode = DC_PREVIEW_MODE;
static DC_PHOTO_CAPTURE_INFO	s_photo_capture_info = {0};
static DC_PHOTO_PANORAMA_STRUCT	s_panorama_info = {0} ;//{{0}, 0, -1, {0}, {0}, {0}};
// static MMIDC_IMAGE_INFO_T			s_dc_info = {0};
static MMIDC_IMAGE_INFO_T			*s_image_info_ptr = PNULL;
static BOOLEAN					s_is_cancel_multishot = FALSE;
static BOOLEAN					s_is_snapshot_succss = TRUE;
static BOOLEAN					s_is_review_full_screen = FALSE;
static BOOLEAN					s_has_been_record = FALSE;
static SCI_SEMAPHORE_PTR        s_callback_audio_semaphore = PNULL;
static SCI_SEMAPHORE_PTR        s_capture_callback_semaphore_ptr = PNULL;
static BOOLEAN                  s_can_press_save = FALSE;
LOCAL  BOOLEAN                  s_is_show_photo_frame = FALSE;

LOCAL GUI_COLOR_T       *s_lastframe_ptr = PNULL;
LOCAL BOOLEAN s_is_osd_flag_before_preview = FALSE;


LOCAL uint32 *s_pda_a888_lastframe_ptr = PNULL;

LOCAL uint32 s_start_preview_tick = 0;


#ifdef BAIDU_DRIVE_SUPPORT
LOCAL SFS_ERROR_E s_dc_error_msg = SFS_ERROR_NONE ;
#endif
#ifdef WIN32
//PUBLIC void* DCAMERA_GetMemptr()
//{
//    static uint16 buffer[240 * 400] = {0};
//    return buffer;
//}
#if 0 //def MMIDC_F_PDA_BIT_16
/*****************************************************************************/
// 	Description : backup last dv frame data
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  相关函数MMIDC_GetLastFrameCallback()
/*****************************************************************************/
LOCAL void MMIDC_CopyLastFrame(void);
#endif
/******************************************************************************/
// Description: To check if digical camera is open
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     SCI_TRUE    has opened
//             SCI_FALSE   has not opened
// Note:       
/******************************************************************************/ 
//PUBLIC BOOLEAN DCAMERA_IsOpen(void) 
//{
//    return TRUE;
//}
/******************************************************************************/
// Description: Open digital Camera function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       before start preview, call this function to open digital camera
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Open(DCAMERA_OP_MODE_E op_mode)
//{
//    return DCAMERA_OP_SUCCESS;
//}

/******************************************************************************/
// Description: Close digital Camera function
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Close(void)
//{
//    return DCAMERA_OP_SUCCESS;
//}
/******************************************************************************/
// Description: Start to preview
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//xxx
// PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartPreview(DCAMERA_PREVIEW_PARAM_T const* param_ptr) 
// {
//     return DCAMERA_OP_SUCCESS;
// }
/******************************************************************************/
// Description: Stop preview
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//xxx
// PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopPreview(void) 
// {
//     return DCAMERA_OP_SUCCESS;
// }
/******************************************************************************/
// Description: snapshot single image
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//xxx
// PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoSnapshot(DCAMERA_SNAPSHOT_PARAM_T const  * 	param_ptr,DCAMERA_SNAPSHOT_RETURN_PARAM_T *	return_param_ptr)
// {
//     return DCAMERA_OP_SUCCESS;
// }

/******************************************************************************/
// Description: snapshot multi images
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//xxx
// PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoBurstSnapshot(DCAMERA_BURST_SNAPSHOT_PARAM_T const  *	param_ptr,DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T * 	return_param_ptr) 
// {
//     return DCAMERA_OP_SUCCESS;
// }

/******************************************************************************/
// Description: Digital camera parameter control
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CMD_E param_cmd, uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback)
//{
//    if(PNULL != callback)
//    {
//        callback(0, 0);
//    }
//    
//    return DCAMERA_OP_SUCCESS;
//}

/******************************************************************************/
// Description: Get some DC information
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_GetInfo(DCAMERA_INFO_CMD_E param_cmd, uint32*	info_ptr) 
//{
//    switch(param_cmd)
//    {
//    case DCAMERA_INFO_SENSOR_SIZE:
//        *info_ptr = 0xFFFFFF;
//        break;
//    case DCAMERA_INFO_ZOOM:
//        if(MMIDC_GetPhotoSize() == PHOTO_SIZE_2048X1536)
//        {
//            *info_ptr = 0;
//        }
//        else
//        {
//            *info_ptr = 7;
//        }
//        break;
//    case DCAMERA_INFO_ATV_CH_ALL_NUM:
//        *info_ptr = 15;
//        break;
//    case DCAMERA_INFO_IMG_SENSOR_NUM:
//        *info_ptr = 2;
//        break;
//   case DCAMERA_INFO_SENSOR_TYPE:
//        *info_ptr = DCAMERA_IMG_SENSOR_TYPE_4P3;
//        break;
//    default:
//        break;
//    }
//
//    return DCAMERA_OP_SUCCESS;
//}

/******************************************************************************/
// Description: Set auto focus information
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetAutoFocus(DCAMERA_AF_INFO_T const * af_info_ptr) 
//{
//    return DCAMERA_OP_SUCCESS;
//}

/******************************************************************************/
// Description: Review JPEG
// Author:     Liangwen.Zhen
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ReviewJpeg(DCAMERA_REVIEW_PARAM_T const * param_ptr) 
//{
//	//xxx
//	//[[ @hongliang.xin call review_callback function to set s_is_review false
//	if(PNULL != param_ptr->review_callback)
//	{
//		param_ptr->review_callback(0,PNULL,0);
//	}
//	//]]
//    return DCAMERA_OP_SUCCESS;
//}


/******************************************************************************/
// Description: Start record
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartRecord ( DCAMERA_RECORD_PARAM_T * dc_param_ptr ) 
//{
//    return DCAMERA_OP_SUCCESS;
//}

/******************************************************************************/
// Description: Stop record
// Author:     Feifei.Gu
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopRecord ( void ) 
//{
//    return DCAMERA_OP_SUCCESS;
//}

/******************************************************************************/
// Description:when APP calls DRECORDER_StopRecord() to stop recording,the callback
//             func will be callback to notify app to get the last frame data.
// Author:     
// Input:      getlastframe_callback,the callback func addr.
//             data_format, yuv or rgb.
// Output:     none
// Return:     
// Note:       none
/******************************************************************************/
//PUBLIC DRECORDER_RETURN_E DRECORDER_RegisterGetLastFrameCallback(
//                                DRECORDER_GETLASTFRAME_CALLBACK getlastframe_callback,
//                                DRECORDER_VIDEO_DATATYPE_E data_type)
//{
//    return DRECORDER_SUCCESS;
//}

#endif
/*****************************************************************************/
// 	Description : review single photo 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ReviewSinglePhotoFromBuf(void);

/*****************************************************************************/
//Description : init dc image info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitDCImageInfo(void);

/*****************************************************************************/
//Description : reset dc image info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetDCImageInfo(void);

/*****************************************************************************/
//Description : get current file buffer and size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCurrentFileBuffer(uint8 **buffer_ptr,//out
                                        uint32 *buffer_size //out
                                       );


#ifdef BAIDU_DRIVE_SUPPORT
LOCAL MMI_RESULT_E HandleDcDiskFull(
                                    MMI_WIN_ID_T        win_id,    // Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                    DPARAM              param       // 相应消息的参数
                                    );
#endif

#if 0
/*****************************************************************************/
//Description : init review memory param for 200w sensor
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitReviewMemoryParamFor200WSensor(void);
#endif

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShutterViceCallback(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)  //播放铃声后的回调函数
{
    if(PNULL != s_callback_audio_semaphore)
    {
        SCI_PutSemaphore(s_callback_audio_semaphore);
    }
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_PlayCountVoice(uint8 times)
{    
    if(1 == times)
    {
        MMIAPISET_PlayRing((MN_DUAL_SYS_E)0, FALSE, 0, 1, MMISET_RING_TYPE_DC_COUNT, PNULL);
    }
    else if(2 == times)
    {
        MMIAPISET_PlayRing((MN_DUAL_SYS_E)0, FALSE, 0, 1, MMISET_RING_TYPE_DC_FINAL_COUNT, PNULL);
    }
}

LOCAL DCAMERA_OSD_POS_T* SetReviewOSDCallback(void)
{
    static DCAMERA_OSD_POS_T  res[3] = {0};

    if(!s_dc_is_open)
    {
        return PNULL;
    }

    //SCI_TRACE_LOW:"[MMIDC] SetReviewOSDCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_405_112_2_18_2_8_29_32,(uint8*)"");
    
    MMIDC_CompleteDisplayOSD();
        
    s_can_press_save = TRUE;
    return &res[0];
}

/*****************************************************************************/
// 	Description : get panorama photo info
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_PHOTO_PANORAMA_STRUCT* MMIDC_GetPhotoPanoramaInfo(void)
{
    return &s_panorama_info;
}

/*****************************************************************************/
// 	Description : get dc struct infor
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC MMIDC_IMAGE_INFO_T* MMIDC_GetDCInfo(void)
{
    return s_image_info_ptr;
}

/*****************************************************************************/
// 	Description : get photo rotation mode
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIDC_GetPhotoRotationMode(void)
{
    DCAMERA_ROTATION_MODE_E rotation_mode = DCAMERA_ROTATION_0;

    rotation_mode = MMIAPIDC_GetCameraSensorAngle();

    return rotation_mode;
}

/*****************************************************************************/
// 	Description : get display mode
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_DISP_MODE_E MMIDC_GetPhotoDisplayMode(void)
{
    DCAMERA_DISP_MODE_E  disp_mode = DCAMERA_DISP_FULLVIEW;


    {
        if((MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd())
           || (MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetHorLcd()))
        {

            if((PHOTO_SIZE_320X240 == MMIDC_GetPhotoSize()) 
              && (LCD_ANGLE_0 == MMIAPIDC_GetCameraSensorAngle()))
            {
                disp_mode = (DCAMERA_DISP_MODE_E)MMIDC_GetDefaultVisualMode();
            }
            else
            {
                disp_mode = DCAMERA_DISP_FULLSCREEN;
            }
        }
        else
        {
            disp_mode = (DCAMERA_DISP_MODE_E)MMIDC_GetDefaultVisualMode();
        }
    }


    return disp_mode;
}
/*****************************************************************************/
// 	Description : get preview param
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDCPreviewParam(DCAMERA_PREVIEW_PARAM_T * preview_param_ptr)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    DCAMERA_ROTATION_MODE_E angle = 0;

    if (PNULL == preview_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDC] GetDCPreviewParam preview_param_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_513_112_2_18_2_8_29_33,(uint8*)"");
        return FALSE;
    }

    MMIDC_GetPhysicalLCDWidthAndHeight(&lcd_width, &lcd_height, &angle);
    /** set rect and lcd device **/
    preview_param_ptr->lcd_id		= MMIDC_GetLcdID();//GUI_MAIN_LCD_ID;

    preview_param_ptr->disp_rect.w	= lcd_width;
    preview_param_ptr->disp_rect.h	= lcd_height;

    //SCI_TRACE_LOW:"[MMIDC] GetDCPreviewParam lcd_width = %d, lcd_height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_525_112_2_18_2_8_29_34,(uint8*)"dd",lcd_width, lcd_height);
    
    preview_param_ptr->rotation_mode = (DCAMERA_ROTATION_MODE_E)angle;
    preview_param_ptr->disp_mode = MMIDCSRV_GetDCDisplayMode();//MMIDC_GetPhotoDisplayMode();

    //SCI_TRACE_LOW:"[MMIDC] GetDCPreviewParam rotation = %d, display mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_531_112_2_18_2_8_29_35,(uint8*)"dd",preview_param_ptr->rotation_mode, preview_param_ptr->disp_mode);
        
    preview_param_ptr->disp_rect.x	= 0;
    preview_param_ptr->disp_rect.y	= 0;
    preview_param_ptr->lcd_block_id	= GUI_BLOCK_0;
    preview_param_ptr->target_rect.x = 0;
    preview_param_ptr->target_rect.y = 0;	

    preview_param_ptr->target_rect.w = lcd_width;
    preview_param_ptr->target_rect.h = lcd_height;
    return TRUE;
}

/*****************************************************************************/
// 	Description : display callback for ex interface
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL uint32 Display_Callback(uint32 ret_value, 
                                  DCAMERA_DISPALY_PARAM_T *p_disp_src,
                                  DCAMERA_DISPALY_PARAM_T *p_disp_dst,
                                  void* p_param)
{
    MMIDC_CMD_PARAM_T cmd_param = {0};
    
    //SCI_TRACE_LOW:"[MMIDC] Display_Callback ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_556_112_2_18_2_8_29_36,(uint8*)"d",ret_value);

#ifdef MMIDC_DEBUG_TRACE
   //SCI_TRACE_LOW:"[MMIDC] Display_Callback p_disp_src=0x%X,p_disp_dst=0x%X"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_560_112_2_18_2_8_29_37,(uint8*)"dd",p_disp_src, p_disp_dst);
#endif

    if ((PNULL == p_disp_src)
        || (PNULL == p_disp_dst)
       )
    {
        return 0xFF;
    }

    if (MMIDC_IsMMIBrushing())
    {
        //SCI_TRACE_LOW:"[MMIDC] Display_Callback is brushing return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_572_112_2_18_2_8_29_38,(uint8*)"");
        return 0xFF;
    }

#if defined(MMIDC_F_U_CAMERA)    
    if ( MMIDC_IsUCameraEnable()
        && MMIDC_IsUCameraDoing()
#if defined(MMIDC_F_SMILE_AUTO_PHOTO)
        && !MMIDC_IsSmileAutoPhotoEnable()
#endif
        )
    {
        //SCI_TRACE_LOW:"[MMIDC] Display_Callback is doing return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_582_112_2_18_2_8_29_39,(uint8*)"");
        return 0xFF;
    }
#endif

    if (!MMIAPIDC_IsPreviewing())
	{
		SCI_TRACE_LOW("[MMIDC] Display_Callback not preview"); 
		return 0xFF;
	}

    MMIDC_PushCaptureCallbackSemaphore();
    //SCI_TRACE_LOW:"[MMIDC] Display_Callback start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_587_112_2_18_2_8_29_40,(uint8*)"");
    
    if( DCAMERA_OP_SUCCESS == ret_value)
    {
        MMIDC_SetDCamSrcDisplay(*p_disp_src);
        MMIDC_SetDCamDstDisplay(*p_disp_dst);


        //SCI_TRACE_LOW:"[MMIDC Display_Callback src type=%d,pattern=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_596_112_2_18_2_8_29_41,(uint8*)"dd",p_disp_src->type, p_disp_src->pattern);

        //SCI_TRACE_LOW:"[MMIDC Display_Callback dst yaddr=0x%X,uaddr=0x%X"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_599_112_2_18_2_8_29_42,(uint8*)"dd",p_disp_dst->yaddr, p_disp_dst->uaddr);

        cmd_param.cmd = MMIDC_CMD_DISP_PREVIEW_FRAME;
        cmd_param.start_preview_tick = MMIDC_GetStartPreviewTick();
        MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_MMI_CMD, &cmd_param, sizeof(cmd_param));
    }

    MMIDC_PopCaptureCallbackSemaphore();
    
    return 0;
}

/*****************************************************************************/
//  Description : get preview expand param
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDCPreviewParamEx(DCAMERA_PREVIEW_EX_PARAM_T * preview_param_ex_ptr)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    DCAMERA_ROTATION_MODE_E angle = 0;

    if (PNULL == preview_param_ex_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDC] GetDCPreviewParamEx preview_param_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_621_112_2_18_2_8_29_43,(uint8*)"");
        return FALSE;
    }

    MMIDC_GetPhysicalLCDWidthAndHeight(&lcd_width, &lcd_height, &angle);
    /** set rect and lcd device **/
    preview_param_ex_ptr->lcd_id        = MMIDC_GetLcdID();//GUI_MAIN_LCD_ID;;

    preview_param_ex_ptr->disp_rect.w   = lcd_width;
    preview_param_ex_ptr->disp_rect.h   = lcd_height;

    //SCI_TRACE_LOW:"[MMIDC] GetDCPreviewParamEx lcd_width = %d, lcd_height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_633_112_2_18_2_8_29_44,(uint8*)"dd",lcd_width, lcd_height);

    #if 0
    if (DCAMERA_ROTATION_0 == angle
        || DCAMERA_ROTATION_90 == angle
        )
    {
        preview_param_ex_ptr->rotation_mode = (DCAMERA_ROTATION_MODE_E)angle;
    }
    else
    {
        preview_param_ex_ptr->rotation_mode = 0;
    }
    #endif
    preview_param_ex_ptr->rotation_mode = (DCAMERA_ROTATION_MODE_E)angle;
    preview_param_ex_ptr->disp_mode = MMIDCSRV_GetDCDisplayMode();//MMIDC_GetPhotoDisplayMode();

    //SCI_TRACE_LOW:"[MMIDC] GetDCPreviewParamEx rotation = %d, display mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_639_112_2_18_2_8_29_45,(uint8*)"dd",preview_param_ex_ptr->rotation_mode, preview_param_ex_ptr->disp_mode);
        
    preview_param_ex_ptr->disp_rect.x   = 0;
    preview_param_ex_ptr->disp_rect.y   = 0;
    preview_param_ex_ptr->lcd_block_id  = GUI_BLOCK_0;
    preview_param_ex_ptr->target_rect.x = 0;
    preview_param_ex_ptr->target_rect.y = 0;    

    preview_param_ex_ptr->target_rect.w = lcd_width;
    preview_param_ex_ptr->target_rect.h = lcd_height;

    preview_param_ex_ptr->invalidatemode = DCAMERA_INVALIDATEMODE_EXTERN;
    preview_param_ex_ptr->disp_cb = Display_Callback;
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : set all preview param
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:	for MMI upper level
/*****************************************************************************/
LOCAL void SetDCAllPreviewParam(void)
{
    
    //SCI_TRACE_LOW:"[MMIDC]: SetDCAllPreviewParam s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_663_112_2_18_2_8_29_46,(uint8*)"d", s_dc_is_open);
    if(s_dc_is_open)
    {
        MMIDC_SetPhotoBrightness(MMIDC_GetPhotoBrightness());
        MMIDC_SetContrast(MMIDC_GetContrast());
        MMIDC_SetPhotoEffect(MMIDC_GetPhotoEffect());
        MMIDC_SetExposalCompensate(MMIDC_GetExposalCompensate());
        MMIDC_SetPhotoFlicker(MMIDC_GetPhotoFlicker());
        MMIDC_SetPhotoEnvironment(MMIDC_GetPhotoEnvironment());
        MMIDC_SetPhotoWhiteBalance(MMIDC_GetPhotoWhiteBalance());
    }
}

/*****************************************************************************/
// 	Description : start photo preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StartDCPreview(void)
{
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;
    DCAMERA_PREVIEW_PARAM_T preview_param = {0};/*lint !e64*/

    MMIDC_ResetHitTimer();
  
    s_is_capture = FALSE;
    s_can_press_save = FALSE;
    MMIDC_CloseCallbackTimer();
    if(!s_dc_is_preview && s_dc_is_open)
    {
        //mmidcsrv
        MMIDCSRV_CheckParam();

    
        MMIDC_InitSettingParamCtrl();

#if defined(MMIDC_F_U_CAMERA)
        if (MMIDC_GetUCameraModuleFlag())
        {
            DCAMERA_PREVIEW_EX_PARAM_T param_ex = {0};

#ifdef MMIDC_F_N_IN_ONE
            if(MMIDC_IsNInOneEnable())
            {
                MMIDC_SetNInOneStartDisplayFrame(FALSE);
            }
#endif
            GetDCPreviewParamEx(&param_ex);
            ret = DCAMERA_StartPreviewEX(&param_ex);
        }
        else
#endif
        {
            GetDCPreviewParam(&preview_param);
            ret = DCAMERA_StartPreview(&preview_param);
        }
        if(DCAMERA_OP_SUCCESS != ret)
        {
            //SCI_TRACE_LOW:"[MMIDC]: DCAMERA_StartPreview ret = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_710_112_2_18_2_8_29_47,(uint8*)"d", ret);
            MMIDC_PostStateMsg();
            MMIDC_ErrorTipForExit(TXT_DC_OPEN_FAIL);
            return FALSE;
        }

    	 //SCI_TRACE_LOW:"[MMIDC]: MMIDC_GetPhotoSize() = %d"
    	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_716_112_2_18_2_8_29_48,(uint8*)"d",MMIDC_GetPhotoSize());
    	 //SCI_TRACE_LOW:"[MMIDC]: MMIDC_GePhotoSizetVerLcd() = %d"
    	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_717_112_2_18_2_8_29_49,(uint8*)"d",MMIDC_GePhotoSizetVerLcd());

#if defined(MMIDC_F_SOFTWARE_ANGLE)
         MMIDC_GetDCModuleSoftwareRotationAngle();
#endif

        MMIDC_SetStartPreviewTick(SCI_GetTickCount());
    
        s_dc_is_preview = TRUE;
        s_is_review = FALSE;
    }
    //SCI_TRACE_LOW:"[MMIDC]: StartDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_726_112_2_18_2_8_29_50,(uint8*)"ddd", ret, s_dc_is_preview, s_dc_is_open);
    return TRUE;
}

/*****************************************************************************/
// 	Description : stop photo preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void StopDCPreview(void)
{
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;

    MMIDC_InitParamCtrl();

    if(s_dc_is_preview && s_dc_is_open)
    {
        s_dc_is_preview = FALSE;
        ret = DCAMERA_StopPreview();
    }
    //SCI_TRACE_LOW:"[MMIDC]: StopDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_745_112_2_18_2_8_29_51,(uint8*)"ddd", ret, s_dc_is_preview, s_dc_is_open);
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }

}

/*****************************************************************************/
// 	Description : close dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void CloseDC(void)
{
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;

    MMIDC_InitParamCtrl();

    if(s_dc_is_open)
    {
        if (PNULL != s_callback_audio_semaphore)
        {
            SCI_GetSemaphore(s_callback_audio_semaphore, MMIDC_MUTEX_TIMEOUT);
            SCI_PutSemaphore(s_callback_audio_semaphore);
        }
        ret = DCAMERA_Close();
        if(DCAMERA_OP_SUCCESS == ret)
        {
            s_dc_is_open = FALSE;
        }
        MMIDC_RestoreBlock();
    }
    //SCI_TRACE_LOW:"[MMIDC]: CloseDC ret = %d, s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_772_112_2_18_2_8_29_52,(uint8*)"dd", ret, s_dc_is_open);
    if(DCAMERA_OP_SUCCESS != ret)
    {
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
    }
	
    
}

/*****************************************************************************/
// 	Description : return dc open or not
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsDCOpen()
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_IsDCOpen s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_787_112_2_18_2_8_30_53,(uint8*)"d", s_dc_is_open);
    return s_dc_is_open;
}

/*****************************************************************************/
// 	Description : return dv open or not
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsDVOpen()
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_IsDVOpen s_dv_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_797_112_2_18_2_8_30_54,(uint8*)"d", s_dv_is_open);
    return s_dv_is_open;
}

/*****************************************************************************/
// 	Description : set lcd rotation by screen mode and size
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL int32 SetLCDForRotation(void)
{
    if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
    {
        MMIDC_SetLcdForRotate(TRUE);
    }
    else if(MMIDC_GetScreenMode() == SCREEN_MODE_VERTICAL)
    {
        MMIDC_SetLcdForRotate(FALSE);
    }

    return 0;
}

/*****************************************************************************/
// 	Description : open dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenDC(void)
{
    BOOLEAN ret = FALSE;
    SETTING_SENSOR_ID_E sensor_id = 0;
    MMIDC_PHOTO_SIZE_E max_size = 0;
    MMIDC_PHOTO_SIZE_E current_size = 0;
#ifdef MMIDC_F_FLASH
    DCAMERA_FLASH_MODE_E flash = DCAMERA_FLASH_OFF;
#endif

    
    MMIDC_FreeLastFrame();
    
    ret = MMIDC_OpenDC();
    
    //SCI_TRACE_LOW:"[MMIDC]: OpenDC ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_835_112_2_18_2_8_30_55,(uint8*)"d", ret);
    
    if(ret)
    {
        SetLCDForRotation();
        MMIDC_SetBlock();        



        sensor_id = MMIDC_GetSettingSensorID();
        MMIDC_SetDCameraSensorID(sensor_id);
#ifdef MMI_CAMERA_F_PHOTO_DATE
    #if defined(MMIDC_F_U_CAMERA)
        if (!MMIDC_GetUCameraModuleFlag())
    #endif
        {
            MMIDC_CreatePhotoDateData();
        }
#endif

        MMIDCSRV_CheckPhotoSize();

        current_size = MMIDC_GetPhotoSize();
#if defined(MMIDC_F_U_CAMERA)
        if (MMIDC_GetUCameraModuleFlag())
        {
            max_size = current_size;
        }
        else
#endif
        {
            max_size = MMIDC_GetCurMaxPhotoSize();
        }
        
        if(max_size < current_size)
        {            
            current_size = max_size;
        }
        MMIAPIDC_SetPhotoSize(current_size);

#ifdef MMIDC_F_FLASH
        if(SETTING_SENSOR_FRONT == sensor_id)
        {
            MMIDC_SetFlashLamp(DCAMERA_FLASH_NONE);
        }
        else
        {
            flash = MMIDC_GetFlashLamp();
            MMIDC_SetFlashLamp(flash);
        }
#endif
        MMIDC_UpdatePhotoZoomValue(MMIDC_GetPhotoZoomValue()); 

       if(!s_dc_is_preview && s_dc_is_open)
       {
            MMIDC_InitSettingParamCtrl();
            SetDCAllPreviewParam();
#ifdef MMIDC_MINI_RESOURCE_SIZE
            MMIDC_SetSettingPhotoSizeInfo();
#endif
       }
    }
    else
    {
        MMIDC_PostStateMsg();
        MMIDC_ErrorTipForExit(TXT_DC_OPEN_FAIL);
    }
    
    return ret;
}

/*****************************************************************************/
// 	Description : get number of multi shoot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetMulitShotNumber(void)
{
    uint32 res = 1;
    
    switch(MMIDC_GetMultiShootEnum())	
    {
    case MULTI_SHOOT_DISABLE:
        res = DC_NO_BURST_PHOTO;
        break;
    case MULTI_SHOOT_3:
        res = DC_BURST_3_PHOTO;
        break;
    case MULTI_SHOOT_6:
        res = DC_BURST_6_PHOTO;
        break;
    case MULTI_SHOOT_9:
        res = DC_BURST_9_PHOTO;
        break;
    default:
        break;
    }			
    //SCI_TRACE_LOW:"MMIDC_GetMultiShootEnum = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_914_112_2_18_2_8_30_56,(uint8*)"d", MMIDC_GetMultiShootEnum());
    return res;
}

/*****************************************************************************/
//  Description : send capture fail msg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL int32 SendCaptureFailMsg(int32 ret,//return value
                                     void* param_ptr,//in
                                     uint32 param_size)
{
    //SCI_TRACE_LOW:"[MMIDC] SendCaptureFailMsg ret==%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_927_112_2_18_2_8_30_57,(uint8*)"d",ret);

    //SCI_TRACE_LOW:"[MMIDC] SendCaptureFailMsg param_ptr=0x%X, param_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_930_112_2_18_2_8_30_58,(uint8*)"dd",param_ptr, param_size);

    if (PNULL != s_callback_audio_semaphore)
    {
        SCI_GetSemaphore(s_callback_audio_semaphore, MMIDC_MUTEX_TIMEOUT);
        SCI_PutSemaphore(s_callback_audio_semaphore);
    }

    s_is_capture = FALSE;
    s_is_snapshot_succss = FALSE;

    MMIDC_CloseCallbackTimer();
    MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_CAPTURE_FAILE_MSG,\
                              (DPARAM)param_ptr, param_size);

    return ret;                 
}    

/*****************************************************************************/
//  Description : send capture success msg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL int32 SendCaptureSuccessMsg(int32 ret,//return value
                                         void* param_ptr,//in
                                         uint32 param_size)
{
    //SCI_TRACE_LOW:"[MMIDC] SendCaptureSuccessMsg ret==%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_952_112_2_18_2_8_30_59,(uint8*)"d",ret);

    //SCI_TRACE_LOW:"[MMIDC] SendCaptureSuccessMsg param_ptr=0x%X, param_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_955_112_2_18_2_8_30_60,(uint8*)"dd",param_ptr, param_size);



    if (PNULL != s_callback_audio_semaphore)
    {
        SCI_GetSemaphore(s_callback_audio_semaphore, MMIDC_MUTEX_TIMEOUT);
        SCI_PutSemaphore(s_callback_audio_semaphore);
    }

    s_can_press_save = TRUE;
    s_is_capture = FALSE;
    
    MMIDC_CloseCallbackTimer();
    MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, 
                            MSG_DC_CAPTURE_SUCCESS_MSG, 
                            (DPARAM)param_ptr, param_size);

    return ret;                 
}                                      

/*****************************************************************************/
//  Description : save data to temp file
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E SaveDataToFile(int32 photo_id,
                               uint8 *data_buffer,//jpg data
                               uint32 data_len
                              )
{
	MMIFILE_ERROR_E ret = SFS_ERROR_SYSTEM;
	MMIFILE_DEVICE_E dev_type = MMI_DEVICE_SDCARD;
    //SCI_TRACE_LOW:"[MMIDC] SaveDataToFile "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_980_112_2_18_2_8_30_61,(uint8*)"");

    if ((PNULL == data_buffer)
        || (0 == data_len)
       )
    {
        return ret;
    }

#ifdef BAIDU_DRIVE_SUPPORT
	if(SFS_NO_ERROR != MMIAPIFMM_GetSuitableFileDev(dev_type, BAIDU_MEMORY_NEEDED_SIZE, &dev_type))
	{
		return SFS_ERROR_NO_SPACE;
	}
#endif
    
    s_image_info_ptr->image_len = data_len;       
    s_image_info_ptr->image_addr = (uint8*)data_buffer;

    //将图像数据写入临时文件
    //若是发生错误，中止拍照，并通知MMI界面提示    
    ret = MMIDC_SaveImageFile(photo_id);

    //SCI_TRACE_LOW:"[MMIDC] SaveDataToFile ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_997_112_2_18_2_8_30_62,(uint8*)"d", ret);

    return ret;
}

/*****************************************************************************/
//  Description : play cpature ring only to capture callback
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void PlayCaptureRing(void)
{
    BOOLEAN is_play_ring = FALSE;    
    
    if (MMIDC_GetShutterVoice() != SHUTTER_VOICE_DISABLE)
    {
        #ifdef BURST_VOICE_ONCE    
            if ((s_photo_capture_info.captured_photo_num + 1)== s_photo_capture_info.photo_total)
            {
                is_play_ring = TRUE;
            }
        #else
            is_play_ring = TRUE;
        #endif
            
    }

    //SCI_TRACE_LOW:"[MMIDC] PlayCaptureRing is_play_ring = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1023_112_2_18_2_8_30_63,(uint8*)"d", is_play_ring);
    
    if (is_play_ring)
    {
        if(PNULL == s_callback_audio_semaphore)
        {
            s_callback_audio_semaphore = SCI_CreateSemaphore("callback audio semaphore", 1);
        }
        
        #ifndef WIN32            
        SCI_GetSemaphore(s_callback_audio_semaphore, MMIDC_MUTEX_TIMEOUT);
        MMIAPISET_PlayRing((MN_DUAL_SYS_E)0, FALSE, 0, 1, MMISET_RING_TYPE_DC, MMIDC_ShutterViceCallback);
        #endif
        
        //SCI_GetSemaphore(s_callback_audio_semaphore, MMIDC_MUTEX_TIMEOUT);
        //SCI_PutSemaphore(s_callback_audio_semaphore);
        
    }

    //SCI_TRACE_LOW:"[MMIDC] PlayCaptureRing"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1041_112_2_18_2_8_30_64,(uint8*)"");
}  
/*****************************************************************************/
//  Description : play cpature ring only to capture callback
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PlayCaptureRing(void)
{
    PlayCaptureRing();
}
/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL uint32 CaptureCallBack(uint32 ret, uint16* data_buffer, uint32 data_len)
{
    int32 i = 0;
    BOOLEAN photos_validate = FALSE;
    BOOLEAN is_cancel_by_user = FALSE;
    uint32 result = 0;

    //SCI_TRACE_LOW:"[MMIDC]: CaptureCallBack, ret = %d, data_buffer = %d, data_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1053_112_2_18_2_8_30_65,(uint8*)"ddd", ret, data_buffer, data_len);
    if(!s_is_capture 
        || !s_dc_is_open
        || MMIDC_LOSE_FOCUS == MMIDC_GetMMIFocusStatus())   //used to cancel multi shot
    {
        //SCI_TRACE_LOW:"[MMIDC]: CaptureCallBack !s_is_capture"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1058_112_2_18_2_8_30_66,(uint8*)"");
        return 0;
    }

    MMIDC_PushCaptureCallbackSemaphore();

    do
    {
        //当退出DC或中断发生，中止拍照处理
        //用户中止连拍,并向MMI发送失败消息
        is_cancel_by_user = MMIDC_GetCaptureStatusByUser();
        //SCI_TRACE_LOW:"[MMIDC]: CaptureCallBack s_is_cancel_multishot=%d, is_cancel_by_user=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1066_112_2_18_2_8_30_67,(uint8*)"dd",s_is_cancel_multishot, is_cancel_by_user);
        if(s_is_cancel_multishot
           || is_cancel_by_user
          )
        {
            s_is_capture = FALSE;
            s_is_snapshot_succss = FALSE;
            
            MMIDC_CloseCallbackTimer();

            if (is_cancel_by_user)
            {
                MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_CAPTURE_FAILE_MSG, (DPARAM)PNULL, 0);
            }
            result = 0xff;
            break;
        }    

        if((ret != 0) || (data_len == 0) || (PNULL == data_buffer)  )
        {
            result = SendCaptureFailMsg(0,PNULL,0);
            break;
        }

#if 0
#ifndef WIN32
        if (MMIDC_Is2MSensorState()
            && ( MULTI_SHOOT_DISABLE == MMIDC_GetMultiShootEnum())
            )
        {
            MMIDC_SetMemoryAddrForReview((uint8*)data_buffer, data_len);
        }
#endif
#endif   
        
        /** record the status the photo captured **/
        s_photo_capture_info.photo_validate[s_photo_capture_info.captured_photo_num] = (0 == ret)?TRUE:FALSE;
        if(s_photo_capture_info.photo_validate[s_photo_capture_info.captured_photo_num])
        {
            if(SHOOT_MODE_NORMAL != MMIDC_GetShootMode())
            {
                s_panorama_info.index ++;
                s_panorama_info.buffer[s_panorama_info.index] = (uint8*)data_buffer;
                s_panorama_info.size += data_len;
                //SCI_TRACE_LOW:"[MMIDC]: DCAdap_CaptureCallBack, index = %d, size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1103_112_2_18_2_8_30_68,(uint8*)"dd", s_panorama_info.index, s_panorama_info.size);
            }
            else
            {
                uint32 photo_id = s_photo_capture_info.captured_photo_num;
                MMIFILE_ERROR_E file_ret = SFS_ERROR_SYSTEM;

                //SCI_TRACE_LOW:"[MMIDC]: photo_id = %d, MMIDC_GetMulitShotNumber = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1110_112_2_18_2_8_30_69,(uint8*)"dd", photo_id, MMIDC_GetMulitShotNumber());
                
                if (photo_id >= MMIDC_GetMulitShotNumber())
                {
                   result = SendCaptureFailMsg(0, PNULL, 0);
                   break;
                }            
                
                file_ret = SaveDataToFile(s_photo_capture_info.captured_photo_num, (uint8*)data_buffer, data_len);
                if (SFS_ERROR_NONE != file_ret)
                {
                    result = 0xFF;
                    result = SendCaptureFailMsg(result,&file_ret,sizeof(file_ret));
                    break;
                }
                else
                {
                    result = 0;
                }
            }
        }    
        s_photo_capture_info.captured_photo_num ++;
        //SCI_TRACE_LOW:"[MMIDC]: CaptureCallBack  captured_photo_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1130_112_2_18_2_8_30_70,(uint8*)"d", s_photo_capture_info.captured_photo_num);

        //当单拍或连拍最后一张时处理
        if(s_photo_capture_info.captured_photo_num == s_photo_capture_info.photo_total)
        {
            for(i = 0; i < s_photo_capture_info.photo_total; i ++)
            {
                /** get all photos validate status, if only one photo is validate, the photos_validate is TRUE **/
                photos_validate |= s_photo_capture_info.photo_validate[i];
            }
            if(photos_validate)
            {
                s_is_capture = FALSE;
                MMIDC_CloseCallbackTimer();
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIDC] CaptureCallBack LAST !photos_validate"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1147_112_2_18_2_8_30_71,(uint8*)"");
                MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_CAPTURE_FAILE_MSG, (DPARAM)PNULL, 0);
            }
            
            SendCaptureSuccessMsg(0, PNULL, 0);
        }
    }while(0);

    MMIDC_PopCaptureCallbackSemaphore();
    return result;       
}

/*****************************************************************************/
// 	Description : get captured number of photos
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_GetCapturedPhotosNumber(void)
{
//xxx
//#ifdef WIN32
//    s_photo_capture_info.captured_photo_num = 9;
//#endif
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_GetCapturedPhotoNumber captured_photo_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1167_112_2_18_2_8_30_72,(uint8*)"d", s_photo_capture_info.captured_photo_num);
    return s_photo_capture_info.captured_photo_num;
}

/*****************************************************************************/
//  Description : get snapshot expand parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
uint32 Capture_Ex_Callback(uint32 ret, 
                                DCAMERA_IMAGE_PARAM_T *p_image_frame,
                                void* p_param
                                )
{
    //BOOLEAN photos_validate = FALSE;
    BOOLEAN is_cancel_by_user = FALSE;
 #if defined(MMIDC_F_U_CAMERA)
    DCAMERA_IMAGE_FRAME_PARAM_T *photo_info_ptr = PNULL;
    DCAMERA_ADDR_T *photo_data_ptr = PNULL;
    int16 i = 0;
#endif
    int32 process_result = 0;

    uint32 t1 = 0;
    uint32 t2 = 0;
    uint32 result = 0;

    
    //SCI_TRACE_LOW:"[MMIDC]: Capture_Ex_Callback, ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1194_112_2_18_2_8_30_73,(uint8*)"d",ret);
                   
    if(!s_is_capture 
        || !s_dc_is_open
        || MMIDC_LOSE_FOCUS == MMIDC_GetMMIFocusStatus())   //used to cancel multi shot
    {
        //SCI_TRACE_LOW:"[MMIDC]: Capture_Ex_Callback !s_is_capture"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1200_112_2_18_2_8_30_74,(uint8*)"");
        return 0;
    }

    t1 = SCI_GetTickCount();
    //SCI_TRACE_LOW:"[MMIDC]: Capture_Ex_Callback t1 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1206_112_2_18_2_8_30_75,(uint8*)"d",t1);


    MMIDC_PushCaptureCallbackSemaphore();

    do
    {
        //当退出DC或中断发生，中止拍照处理
        //用户中止连拍,并向MMI发送失败消息
        is_cancel_by_user = MMIDC_GetCaptureStatusByUser();
        //SCI_TRACE_LOW:"[MMIDC]: Capture_Ex_Callback s_is_cancel_multishot=%d, is_cancel_by_user=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1213_112_2_18_2_8_30_76,(uint8*)"dd",s_is_cancel_multishot, is_cancel_by_user);
        if(s_is_cancel_multishot
           || is_cancel_by_user
          )
        {
            s_is_capture = FALSE;
            s_is_snapshot_succss = FALSE;
            
            MMIDC_CloseCallbackTimer();

            if (is_cancel_by_user)
            {
                MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_CAPTURE_FAILE_MSG, (DPARAM)PNULL, 0);
            }
            result = 0xff;
            break;
        }

        if(0 != ret)
        {
            result = SendCaptureFailMsg(0, PNULL, 0);
            break;
        }
        else
        {
            //PlayCaptureRing();
            
            #if defined(MMIDC_F_U_CAMERA)
                //reset u camera photo data
                MMIDC_SetUCameraPhotoData(FALSE, PNULL, 0);
                    
                if (MMIDC_IsUCameraEnable()
                    #if defined(MMIDC_F_SMILE_AUTO_PHOTO)
                    && !MMIDC_IsSmileAutoPhotoEnable()
                    #endif
                    )
                {
                    MMIDC_IMAGE_PROCESS_INFO_T image_info = {0};
                    MMIDC_IMAGE_PROCESS_OUT_INFO_T out_info = {0};                

                    photo_info_ptr = &p_image_frame->frame_info_t[i];
                    photo_data_ptr = &p_image_frame->dcamera_addr[i];
                    i++; 

                    image_info.data_size.w = photo_info_ptr->target_size.w;
                    image_info.data_size.h = photo_info_ptr->target_size.h;
                    
                    image_info.data_type = photo_info_ptr->data_type;
                    image_info.data_addr.chn1_ptr = photo_data_ptr->chn1_addr; /*lint !e64*/
                    image_info.data_addr.chn2_ptr = photo_data_ptr->chn2_addr; /*lint !e64*/
                    image_info.data_addr.chn3_ptr = photo_data_ptr->chn3_addr; /*lint !e64*/

                    image_info.data_from = UCAMERA_DATA_ROUTE_CAPTURE;
                    
                    MMIDC_PushImgProcMutex();
                    process_result = MMIDC_ProcessImage(&image_info, &out_info);
                    MMIDC_PopImgProcMutex();
                    
                    //不成功，发送失败消息
                    if (0 != process_result)
                    {
                        //SCI_TRACE_LOW:"[MMIDC] Capture_Ex_Callback process_result=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1267_112_2_18_2_8_30_77,(uint8*)"d",process_result);
                        result = SendCaptureFailMsg(0xFF, PNULL, 0);
                        break;
                    }

                    //SCI_TRACE_LOW:"[MMIDC] Capture_Ex_Callback data_size.w=%d, data_size.h=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1272_112_2_18_2_8_30_78,(uint8*)"dd",photo_info_ptr->target_size.w, photo_info_ptr->target_size.h);
                }
                else            
            #endif
                {
                    MMIFILE_ERROR_E file_ret = SFS_ERROR_SYSTEM;
                    //jpeg file:p_image_frame->dcamera_addr[0].chn1_addr
                    //jpeg size:p_image_frame->dcamera_addr[0].chn2_addr
                    if ((PNULL != p_image_frame->dcamera_addr[0].chn1_addr)
                        && (p_image_frame->dcamera_addr[0].chn2_addr > 0)
                       )
                    {
                    #ifdef WIN32
                    p_image_frame->dcamera_addr[0].chn2_addr = p_image_frame->frame_info_t[0].target_size.w * p_image_frame->frame_info_t[0].target_size.h;
                    #endif
                        file_ret = SaveDataToFile(s_photo_capture_info.captured_photo_num,\
                                             (uint8*)p_image_frame->dcamera_addr[0].chn1_addr,\
                                             p_image_frame->dcamera_addr[0].chn2_addr
                                             );

                        if (SFS_ERROR_NONE != file_ret)
                        {
                            process_result = 0xFF;
                            result = SendCaptureFailMsg(process_result,&file_ret,sizeof(file_ret));
                            break;
                        }
                        else
                        {
                            process_result = 0;
                        }
                    }
                    else
                    {
                        process_result = 0xFF; 
                    }

                    //SCI_TRACE_LOW:"[MMIDC] Capture_Ex_Callback process_result=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1305_112_2_18_2_8_31_79,(uint8*)"d",process_result);

                    if (0 != process_result)
                    {
                        result = SendCaptureFailMsg(process_result, PNULL, 0);
                        break;
                    }
                }
            
#if defined(MMIDC_F_U_CAMERA)
            if (MMIDC_IsUCameraEnable() 
                && (!MMIDC_IsPhotoData()
                #if defined(MMIDC_F_SMILE_AUTO_PHOTO)
                || MMIDC_IsSmileAutoPhotoEnable()
                #endif
                ))
#endif        
            {
                SendCaptureSuccessMsg(0, PNULL, 0);
            }
        }    

        t2 = SCI_GetTickCount();
        //SCI_TRACE_LOW:"[MMIDC] Capture_Ex_Callback end t2=%d, t2-t1=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1323_112_2_18_2_8_31_80,(uint8*)"dd",t2, t2 - t1);
    }while(0);
    
    MMIDC_PopCaptureCallbackSemaphore();
    
    return result;
}

/*****************************************************************************/
//  Description : get snapshot expand parameter
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void GetSnapshotExParam(DCAMERA_SNAPSHOT_PARAM_T *normal_param_ptr, //in
                                      DCAMERA_SNAPSHOT_EX_PARAM_T *snapshot_ex_ptr //out
                                      )
{
    int16 i = 0;
    MMIDC_PHOTO_SIZE_E photo_size = MMIDC_GetPhotoSize();
    MMIDC_SIZE_ATTRIBUTE_T size_val = {0};


    if ((PNULL == normal_param_ptr)
        || (PNULL == snapshot_ex_ptr)
        )
    {
        return ;
    }

    MMIDC_GetPhotoSizeWidthAndHeight(photo_size, &size_val);

    snapshot_ex_ptr->target_info[i].target_size.w = size_val.size_width;
    snapshot_ex_ptr->target_info[i].target_size.h = size_val.size_height;    
    snapshot_ex_ptr->target_info[i].rotation_mode = normal_param_ptr->rotation_mode;
    
    #if defined(MMIDC_F_SMILE_AUTO_PHOTO)
    if (MMIDC_IsSmileAutoPhotoEnable())
    {
        snapshot_ex_ptr->target_info[i].data_type = DCAMERA_DATA_TYPE_JPEG;    
    }
    else
    #endif
    {
    snapshot_ex_ptr->target_info[i].data_type = DCAMERA_DATA_TYPE_YUV422;
    snapshot_ex_ptr->target_info[i].data_pattern.yuv_pattern = DCAMERA_YUV_TWO_PLANE;
    }
    i++;    

    snapshot_ex_ptr->target_frame_count = i;
    snapshot_ex_ptr->snapshot_num = 1;
    snapshot_ex_ptr->b_exif_needed = 0;
    snapshot_ex_ptr->image_notice_cb = Capture_Ex_Callback;
    snapshot_ex_ptr->capture_start_callback = normal_param_ptr->capture_start_callback;

    //snapshot_ex_ptr->quickview_callback = normal_param_ptr->quickview_callback;
    snapshot_ex_ptr->enc_quality_level = normal_param_ptr->enc_quality_level;
    snapshot_ex_ptr->flash_enable = normal_param_ptr->flash_enable;
    snapshot_ex_ptr->interval_of_frame = 0;

    //SCI_TRACE_LOW:"[MMIDC] GetSnapshotExParam target_size.w =%d, h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1379_112_2_18_2_8_31_81,(uint8*)"dd",snapshot_ex_ptr->target_info[0].target_size.w, snapshot_ex_ptr->target_info[0].target_size.h);

    //SCI_TRACE_LOW:"[MMIDC] GetSnapshotExParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1381_112_2_18_2_8_31_82,(uint8*)"");
}


/*****************************************************************************/
// 	Description : notify start snapshot callback
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL uint32 NotifyStartSnapshotCallback(DCAMERA_RETURN_VALUE_E ret_value, //in
                                        void* param_ptr, //in
                                        uint32 param_size //in
                                        )
{
    MMIDC_CMD_E cmd = 0;

    SCI_TRACE_LOW("[MMIDC] NotifyStartSnapshotCallback  ret_value=%d",\
                  ret_value);

	if(MMIDC_GetMultiShootEnum() != MULTI_SHOOT_DISABLE)  //bug175263
	{
		PlayCaptureRing();
	}
	else
	{
		cmd = MMIDC_CMD_PLAY_CAPTURE_RING_END;

		MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_MMI_CMD, &cmd, (uint32)sizeof(cmd));
	}
    return 0;
}

/*****************************************************************************/
// 	Description : snap shot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SnapShot(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    DCAMERA_SNAPSHOT_PARAM_T snapshot_param = {0};/*lint !e64*/
    DCAMERA_SNAPSHOT_RETURN_PARAM_T snapshot_return = {0};
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
#ifdef MOTION_SENSOR_TYPE 
	MMK_GSENSOR_STATUS_E gsensor_satus = MMK_GSENSOR_STATUS_UP_SIDE;
#endif		
    SCI_MEMSET(&s_photo_capture_info, 0x00, sizeof(DC_PHOTO_CAPTURE_INFO));
    s_photo_capture_info.photo_total = 1;
    s_photo_capture_info.captured_photo_num = 0;
    s_is_review_full_screen = FALSE;
    MMIDC_ResetCurrentPhotoID();
    if(MMIDC_GetShootMode() != SHOOT_MODE_NORMAL)	
    {
        snapshot_param.data_type = DCAMERA_DATA_TYPE_YUV422;
    }
    else
    {
        snapshot_param.data_type = DCAMERA_DATA_TYPE_JPEG;
    }
    snapshot_param.buf_len = 0;
    snapshot_param.data_buf = PNULL;
    snapshot_param.file_name = PNULL;
    snapshot_param.snapshot_callback = (DCAMERA_CALL_BACK)CaptureCallBack;
    snapshot_param.quickview_callback = (DCAMERA_QUICK_VIEW_CALL_BACK)SetReviewOSDCallback;
    snapshot_param.capture_start_callback = NotifyStartSnapshotCallback;
    
#ifdef MMI_CAMERA_F_PHOTO_DATE
    #if defined(MMIDC_F_U_CAMERA)
        if (!MMIDC_GetUCameraModuleFlag())
    #endif
        {
            MMIDC_GetPhotoDateParameter(&snapshot_param.capture_date);
        }
#endif
    if((DCAMERA_ROTATION_90 == MMIAPIDC_GetCameraSensorAngle() 
         || DCAMERA_ROTATION_270 == MMIAPIDC_GetCameraSensorAngle() 
        )
       && MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd())
    {
        snapshot_param.rotation_mode = DCAMERA_ROTATION_90;
    }
#if defined MAINLCD_LOGIC_ANGLE_90 && (defined MAINLCD_DEV_SIZE_128X160 || defined MAINLCD_DEV_SIZE_176X220 || defined MAINLCD_DEV_SIZE_240X320)
	//假横屏
	else if((DCAMERA_ROTATION_0 == MMIAPIDC_GetCameraSensorAngle())
		&&(LCD_ANGLE_90 == MMIDC_GetPlatformAngle())
		&& (!MMIDC_IsRealHorLcd())
		&& (DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
		)
	{
		snapshot_param.rotation_mode = DCAMERA_ROTATION_270;
    }
#endif
    else
    {
#ifndef MOTION_SENSOR_TYPE    
		snapshot_param.rotation_mode = DCAMERA_ROTATION_0;
#else
		gsensor_satus = MMK_GetGsensorStatus();
		switch (gsensor_satus)
		{
		case MMK_GSENSOR_STATUS_UP_SIDE:
			snapshot_param.rotation_mode = DCAMERA_ROTATION_0;
			break;
		case MMK_GSENSOR_STATUS_RIGHT_SIDE:
			snapshot_param.rotation_mode = DCAMERA_ROTATION_270;
			break;
            
		case MMK_GSENSOR_STATUS_DOWN_SIDE:
			snapshot_param.rotation_mode = DCAMERA_ROTATION_180;
			break;
            
		case MMK_GSENSOR_STATUS_LEFT_SIDE:
			snapshot_param.rotation_mode = DCAMERA_ROTATION_90;
			break;
            
		default:
			snapshot_param.rotation_mode = DCAMERA_ROTATION_0;
			break;
		}	
#endif
    }
 
    if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
    {
        snapshot_param.rotation_mode = MMIDC_GetCapAngleFromDCam();
    }
    
    switch(MMIDC_GetPhotoQuality())
    {
    case PHOTO_QUALITY_BRIEF:						// 简洁
        snapshot_param.enc_quality_level = DC_PHOTO_QUALITY_4;
        break;
    case PHOTO_QUALITY_NORMAL:						// 普通
        snapshot_param.enc_quality_level = DC_PHOTO_QUALITY_6;
        break;
    case PHOTO_QUALITY_ADVANCED:					// 高级
        snapshot_param.enc_quality_level = DC_PHOTO_QUALITY_8;
        break;
    default:
        //MMIDC_ASSERT_WARNING:"[MMIDC] SnapShot Quality = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1444_112_2_18_2_8_31_83,(uint8*)"");
        snapshot_param.enc_quality_level = DC_PHOTO_QUALITY_8;
        break;
    }    
    snapshot_param.flash_enable	= 0;    
    if(FRAME_DISABLE == MMIDC_GetFrameIndex())
    {
        snapshot_param.personal_snapshot.select_personal = 0;
    }
    else
    {
#ifdef CAMERA_FRAME_SUPPORT        
        GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        MMI_IMAGE_ID_T image_id = (MMI_IMAGE_ID_T)(IMAGE_COMMON_FRAME_01 + MMIDC_GetFrameIndex() - 1);
        GUI_POINT_T point = MMIDC_GetFrameDisplayPos(MMIDC_GetFrameIndex());
        register uint16* buffer = PNULL;

        int16 screen_width = MMIDC_GetPreviewWidth();
        int16 screen_height = MMIDC_GetPreviewHeight();

    	 //SCI_TRACE_LOW:"[MMIDC]: MMIDC_GetPhotoSize() = %d"
    	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1464_112_2_18_2_8_31_84,(uint8*)"d",MMIDC_GetPhotoSize());
    	 //SCI_TRACE_LOW:"[MMIDC]: MMIDC_GePhotoSizetVerLcd() = %d"
    	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1465_112_2_18_2_8_31_85,(uint8*)"d",MMIDC_GePhotoSizetVerLcd());

        MMIDC_ClearOSDBlock();		
        
        GUIRES_DisplayImg(&point, PNULL, PNULL, MMIDC_MAIN_WIN_ID, image_id, &dev_info);

   
        buffer = (uint16 *)UILAYER_GetLayerBufferPtr(&dev_info);
        if (PNULL != buffer)
        {
            //SCI_TRACE_LOW:"[MMIDC]: SnapShot with frame"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1475_112_2_18_2_8_31_86,(uint8*)"");
            snapshot_param.personal_snapshot.image_buffer = buffer;
            snapshot_param.personal_snapshot.select_personal = 1;

            snapshot_param.personal_snapshot.image_height = screen_height;
            snapshot_param.personal_snapshot.photo_height = screen_height;
            
            snapshot_param.personal_snapshot.image_width = screen_width;
            snapshot_param.personal_snapshot.photo_width = screen_width;

            snapshot_param.personal_snapshot.photo_buffer = PNULL;
            snapshot_param.personal_snapshot.photo_offset_x = 0;
            snapshot_param.personal_snapshot.photo_offset_y = 0;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDC]: SnapShot with frame buffer NULL!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1491_112_2_18_2_8_31_87,(uint8*)"");
        }
#endif
    }	

    //SCI_TRACE_LOW:"[MMIDC]: SnapShot rotation_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1496_112_2_18_2_8_31_88,(uint8*)"d", snapshot_param.rotation_mode);

#if defined(MMIDC_F_U_CAMERA)
    if (MMIDC_GetUCameraModuleFlag())
    {
        DCAMERA_SNAPSHOT_EX_PARAM_T snapshot_ex = {0};

        GetSnapshotExParam(&snapshot_param, &snapshot_ex);
        ret = DCAMERA_DoSnapshotEX(&snapshot_ex, SCI_NULL);
    }
    else
#endif
    {
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
        snapshot_param.rotation_mode += (DCAMERA_ROTATION_MODE_E)LCD_GetWearType();
        if (snapshot_param.rotation_mode > (DCAMERA_ROTATION_270 + 1))
        {
            SCI_PASSERT(0, ("snapshot_param.rotation_mode > (DCAMERA_ROTATION_270 + 1)"));
        }
        else
        {
            snapshot_param.rotation_mode %= (DCAMERA_ROTATION_270 + 1);
            SCI_TRACE_LOW("SnapShot:snapshot_param.rotation_mode=%d",snapshot_param.rotation_mode);
        }

#endif
        ret = DCAMERA_DoSnapshot(&snapshot_param, &snapshot_return);
    }

    //MMIDC_ClearOSDBlock();
/*    if(FRAME_DISABLE != MMIDC_GetFrameIndex())
    {
        if(MMIDC_GetIsAutoSave() == AUTO_SAVE_OFF)
        {
           MMIDC_DisplayQuickViewSoftkey();
        }
    }*/

    //SCI_TRACE_LOW:"[MMIDC]: SnapShot return: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1512_112_2_18_2_8_31_89,(uint8*)"d", ret);

    return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
//Description : callback when review photo complete
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL uint32 ReviewPhotoCallBack(DCAMERA_RETURN_VALUE_E ret_value, void* param_ptr, uint32 param_size)
{
    //SCI_TRACE_LOW:"[MMIDC]: ReviewPhotoCallBack +, ret_value = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1523_112_2_18_2_8_31_90,(uint8*)"d", ret_value);
    if(!s_dc_is_open
        || MMIDC_LOSE_FOCUS == MMIDC_GetMMIFocusStatus())
    {
        s_is_review = FALSE;
        MMIDC_CloseCallbackTimer();
        return 0;
    }
    MMIDC_CloseCallbackTimer();
    if(0 != ret_value)
    {
        MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_REVIEW_FAILE_MSG, (DPARAM)PNULL, 0);
    }
    else
    {
        MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_DC_REVIEW_SUCCESS_MSG, (DPARAM)PNULL, 0);
    }
    s_is_review = FALSE;
    return 0;
}

/*****************************************************************************/
// 	Description : get review photo angle
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC DCAMERA_ROTATION_MODE_E MMIDC_GetReviewPhotoAngle(void)
{
    DCAMERA_ROTATION_MODE_E  rotation_angle = 0;
    DCAMERA_ROTATION_MODE_E dcam_angle = 0;
    
   BOOLEAN is_real_hor_lcd = FALSE;

    is_real_hor_lcd = MMIDC_IsRealHorLcd();
    dcam_angle = MMIAPIDC_GetCameraSensorAngle();
    switch(dcam_angle)
    {
    case DCAMERA_ROTATION_90:
    case DCAMERA_ROTATION_270:
        if (MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd())
        {
            rotation_angle = DCAMERA_ROTATION_0;
        }
        else
        {
            if (is_real_hor_lcd)
            {
                rotation_angle = DCAMERA_ROTATION_0;
            }
            else
            {
                rotation_angle = DCAMERA_ROTATION_90;
            }
        }
        break;

    case DCAMERA_ROTATION_MIRROR:
    case DCAMERA_ROTATION_FLIP:
        rotation_angle = DCAMERA_ROTATION_0;
        break;
        
    default:
        if (is_real_hor_lcd)
        {
            //rotation_angle = DCAMERA_ROTATION_90;
            rotation_angle = DCAMERA_ROTATION_0;
        }
        else
        {
            rotation_angle = dcam_angle;
        }
        break;
    }


    SCI_TRACE_LOW("[MMIDC] MMIDC_GetReviewPhotoAngle rotation_angle=%d",
                rotation_angle);

    return rotation_angle;
}

/*****************************************************************************/
//Description : review single photo 
//Global resource dependence : none
//  Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ReviewSinglePhotoFromBuf(void)
{
    DCAMERA_REVIEW_PARAM_T	review_param = {0};	/*lint !e64*/
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    DCAMERA_ROTATION_MODE_E  angle = 0;
    DCAMERA_ROTATION_MODE_E dcam_angle = 0;

    BOOLEAN is_real_hor_lcd = FALSE;

    uint8 *buf_ptr = PNULL;
    uint32 buf_size = 0;
    
    //SCI_TRACE_LOW:"[MMIDC]: ReviewSinglePhotoFromBuf DC_REVIEW_JPEG"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1562_112_2_18_2_8_31_91,(uint8*)"");

    if (!GetCurrentFileBuffer(&buf_ptr, &buf_size))
    {
        return FALSE;
    }

    
    MMIDC_GetPhysicalLCDWidthAndHeight(&lcd_width, &lcd_height, &angle);
    
    review_param.lcd_block_id = 0;
    review_param.lcd_id = MMIDC_GetLcdID();//0;
    review_param.jpeg_buffer = (uint8*)buf_ptr;
    review_param.jpeg_len = buf_size;
    review_param.file_name = PNULL;
    review_param.target_rect.x = 0;
    review_param.target_rect.y = 0;
    review_param.target_rect.w = lcd_width;
    review_param.target_rect.h = lcd_height;
    review_param.disp_rect.x = 0;
    review_param.disp_rect.y = 0;
    review_param.disp_rect.w = lcd_width;
    review_param.disp_rect.h = lcd_height;
    
    review_param.review_callback = (DCAMERA_CALL_BACK)ReviewPhotoCallBack;


    dcam_angle = MMIAPIDC_GetCameraSensorAngle();
    switch(dcam_angle)
    {
    case DCAMERA_ROTATION_90:
    case DCAMERA_ROTATION_270:
        if (MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd())
        {
            review_param.disp_rect.w = lcd_width;
            review_param.disp_rect.h = lcd_height;
            review_param.target_rect = review_param.disp_rect;
        }
        break;
    default:
        break;
    }

    review_param.rotation_mode = MMIDC_GetReviewPhotoAngle();

#if 0	
#if defined(MMIDC_F_U_CAMERA)
    if (MMIDC_GetUCameraModuleFlag())
    {
		if ((DCAMERA_ROTATION_0 != dcam_angle)
			&& (DCAMERA_ROTATION_90 != dcam_angle)
		   )
		{
        	review_param.rotation_mode = 0;
		}
    }
#endif
#endif

    review_param.disp_mode = MMIDCSRV_GetDCDisplayMode();//MMIDC_GetPhotoDisplayMode();

    //SCI_TRACE_LOW:"[MMIDC]: is_real_hor_lcd= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1635_112_2_18_2_8_31_92,(uint8*)"d",is_real_hor_lcd);

    //SCI_TRACE_LOW:"[MMIDC]: disp rect w= %d, h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1637_112_2_18_2_8_31_93,(uint8*)"dd", review_param.disp_rect.w, review_param.disp_rect.h);
    //SCI_TRACE_LOW:"[MMIDC]: target rect w = %d, h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1638_112_2_18_2_8_31_94,(uint8*)"dd", review_param.disp_rect.w, review_param.disp_rect.h);
    //SCI_TRACE_LOW:"[MMIDC]: rotation_mode = %d, disp_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1639_112_2_18_2_8_31_95,(uint8*)"dd", review_param.rotation_mode, review_param.disp_mode);

    //SCI_TRACE_LOW:"[MMIDC]: jpeg_buffer = %d, jpeg_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1641_112_2_18_2_8_31_96,(uint8*)"dd", review_param.jpeg_buffer, review_param.jpeg_len);

    ret = DCAMERA_ReviewJpeg(&review_param);
    
    //SCI_TRACE_LOW:"[MMIDC]: DCAMERA_ReviewJpeg return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1645_112_2_18_2_8_31_97,(uint8*)"d", ret);

    //InitReviewMemoryParamFor200WSensor();

    return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : general review photo 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ReviewPhotos(void)
{
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_ReviewPhotos %d, flow_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1659_112_2_18_2_8_31_98,(uint8*)"dd",MMIDC_GetShootMode(), mode);

    MMIDC_DisplaySoftKey(mode);
	if(DC_SAVE_MODE == mode)
	{
	    MMIDC_DisplayCurrentFileName();
	}
    MMIDC_CompleteDisplayOSD();

    if(SHOOT_MODE_NORMAL == MMIDC_GetShootMode())
    {
        s_is_review = TRUE;
        MMIDC_StartCallbackTimer();
        if(!ReviewSinglePhotoFromBuf())
        {
            MMIDC_ErrorTip(TXT_DC_CAPTURE_FAILED);
        }
    }
}

/*****************************************************************************/
// 	Description : get current mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_FLOW_MODE_E MMIDC_GetCurrentMode(void)
{
    return s_dc_current_flow_mode;
}

/*****************************************************************************/
//Description : set current mode
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCurrentMode(DC_FLOW_MODE_E mode)
{
#ifdef DC_WATCH_UI_SUPPORT
    if(s_dc_current_flow_mode != mode)
    {
        WDC_Display_FlowModeExit(s_dc_current_flow_mode);
        s_dc_current_flow_mode = mode;
        WDC_Display_FlowModeEnter(s_dc_current_flow_mode);
    }
#else
    s_dc_current_flow_mode = mode;
#endif
}

/*****************************************************************************/
//Description : callback when record complete
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL void RecordEndNotifyCallBack(int32 i_type)
{
    DV_END_TYPE_E end_type = DV_END_NORMAL;

	s_dv_is_record = FALSE;

    //SCI_TRACE_MID:"[MMIDC]: RecordEndNotifyCallBack record end! i_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1708_112_2_18_2_8_31_99,(uint8*)"d", i_type);
    if(!s_dv_is_open
        || MMIDC_LOSE_FOCUS == MMIDC_GetMMIFocusStatus())
    {
        return;
    }
    
    if(0 == i_type)
    {
        // normal
        end_type = DV_END_ROOM_INSUF_ERROR;
    }
    else if(i_type > 0)
    {
        if(DV_PREVIEW_MODE == MMIDC_GetCurrentMode())
        {
            end_type = DV_END_BY_APP_CANCEL;
        }
        else
        {
            // by app 
            end_type = DV_END_BY_APP;
        }
    }
    else
    {
        if (DRECORDER_ERROR_LIMIT_FILE_SIZE == i_type)
        {
            end_type = DV_END_LIMIT_SIZE;
        }
        else
        {
            end_type = DV_END_ERROR;
        }
    }
    
    MMIAPICOM_OtherTaskToMMI(MMIDC_MAIN_WIN_ID, MSG_RECORD_END, (DPARAM)&end_type, sizeof(end_type));
    
    //SCI_TRACE_LOW:"[MMIDC]: RecordEndNotifyCallBack end_type= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1747_112_2_18_2_8_31_100,(uint8*)"d", end_type);
}

/*****************************************************************************/
//Description : convert image data by angle
//Global resource dependence : none
//Author: robert.wang
//Note: 
/*****************************************************************************/
LOCAL BOOLEAN ConvertImageData(
                                    uint8 *dst_ptr,//out
                                    uint32 dst_size,//in
                                    uint32 dst_width,//in
                                    uint32 dst_height,//in
                                    uint8 *src_ptr,//in
                                    uint32 src_size,//in
                                    uint32 src_width,//in
                                    uint32 src_height,//in
                                    DRECORDER_VIDEO_DATATYPE_E data_type,//in
                                    LCD_ANGLE_E convert_angle, //in
                                    BOOLEAN is_around_color,
					                BOOLEAN is_show_from_first_line
                                    )
{
    BOOLEAN ret = FALSE;
    uint8 data_byte_unit = 2; 
    uint16 *dst_u16_ptr = PNULL;
    uint16 *src_u16_ptr = PNULL;

    uint32 offset_size = 0;
    int32  offset_bytes = 0;
    uint32 i = 0;
    uint32 j = 0;
    
    //SCI_TRACE_LOW:"[MMIDC] ConvertImageData dst_ptr=0x%X,dst_size=%d,dst_width=%d,dst_height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1783_112_2_18_2_8_31_101,(uint8*)"dddd",dst_ptr,dst_size,dst_width,dst_height);

    //SCI_TRACE_LOW:"[MMIDC] ConvertImageData src_ptr=0x%X,src_size=%d,src_width=%d,src_height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1790_112_2_18_2_8_32_102,(uint8*)"dddd",src_ptr,src_size,src_width,src_height);
                    
    //SCI_TRACE_LOW:"[MMIDC] ConvertImageData type=%d,angle=%d,is_around_color=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1796_112_2_18_2_8_32_103,(uint8*)"ddd",data_type,convert_angle,is_around_color);
                    
//    switch(data_type)
//    {
//    case DRECORDER_VIDEO_DATATYPE_RAWRGB:
        data_byte_unit = 2;
        dst_u16_ptr = (uint16 *)dst_ptr;
        src_u16_ptr = (uint16 *)src_ptr;
//        break;
//    default:
//        break;
//    }

    do
    {
        //check param
        if (
           (dst_size < dst_width * dst_height * data_byte_unit)
           || PNULL == dst_ptr
           || PNULL == src_ptr
           )
        {
            //SCI_TRACE_LOW:"[MMIDC] ConvertImageData PARAM error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1818_112_2_18_2_8_32_104,(uint8*)"");
            break;
        }
        
        switch(convert_angle)
        {
        case LCD_ANGLE_0:
            if (is_around_color)
            {
                offset_bytes = (dst_width - src_width) / 2 ;
            }
            else
            {            
                offset_bytes = dst_width * (dst_height - src_height) / 2 ;
            }

            if (is_show_from_first_line)
                offset_bytes = 0;
            if (offset_bytes < 0)
            {
                break;
            }
            
            offset_size = (uint32)(offset_bytes + dst_width * (src_height - 1) + src_width - 1) * data_byte_unit; /*lint !e737*/
            if (offset_size >= dst_size)
            {
                //SCI_TRACE_LOW:"[MMIDC] ConvertImageData size overflow!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1842_112_2_18_2_8_32_105,(uint8*)"");
                break;
            }
            
            for (i = 0; i < src_height; i++)
            {
                for (j = 0; j < src_width; j++)
                {
                    *(dst_u16_ptr + offset_bytes + dst_width * i + j) = *(src_u16_ptr++);
                }
            }
            /*
            MMI_MEMCPY((dst_u16_ptr + offset_bytes), dst_size,\
                           src_u16_ptr, src_size,\
                           src_size
                           );
            */               
            ret = TRUE;
            break;
        case LCD_ANGLE_90:
            if (is_around_color)
            {
                offset_bytes = (dst_width - src_height) / 2;
            }
            else
            {
                offset_bytes = dst_width * (dst_height - src_width) / 2;
            }
			
            if (is_show_from_first_line)
				offset_bytes = 0;
			
            if (offset_bytes < 0)
            {
                break;
            }
            
            offset_size = (uint32)(offset_bytes + dst_width * (src_width - 1) + src_height - 1) * data_byte_unit; /*lint !e737*/
            if (offset_size >= dst_size)
            {
                //SCI_TRACE_LOW:"[MMIDC] ConvertImageData size overflow!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1879_112_2_18_2_8_32_106,(uint8*)"");
                break;
            }
            
            for (i = 0; i < src_width; i++)
            {
                for (j = 0; j < src_height; j++)
                {
                    *(dst_u16_ptr + offset_bytes + dst_width * i + j) = *(src_u16_ptr + src_width * j + src_width - 1 - i);
                }
            }
            
            ret = TRUE;
            break;
         case LCD_ANGLE_180:
            if (is_around_color)
            {
                offset_bytes = (dst_width - src_width) / 2 ;
            }
            else
            {
                offset_bytes = dst_width * (dst_height - src_height) / 2;
            }
            if (is_show_from_first_line)
				offset_bytes = 0;
			
            if (offset_bytes < 0)
            {
                break;
            }
            
            offset_size = (uint32)(offset_bytes + dst_width * (src_height - 1) + src_width - 1) * data_byte_unit; /*lint !e737*/
            if (offset_size >= dst_size)
            {
                //SCI_TRACE_LOW:"[MMIDC] ConvertImageData size overflow!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1911_112_2_18_2_8_32_107,(uint8*)"");
                break;
            }

            for (i = 0; i < src_height; i++)                                                                                               
            {                                                                                                                              
                for (j = 0; j < src_width; j++)                                                                                            
                {                                                                                                                          
                    *(dst_u16_ptr + offset_bytes + dst_width * i + j) = *(src_u16_ptr + src_width * (src_height - 1 - i) +  src_width - 1 - j);
                }                                                                                                                          
            }
            
            ret = TRUE;
            break;
         case LCD_ANGLE_270:
            if (is_around_color)
            {
                offset_bytes = (dst_width - src_width) / 2 ;
            }
            else
            {
                offset_bytes = dst_width * (dst_height - src_width) / 2;
            }
            if (is_show_from_first_line)
				offset_bytes = 0;
			
            if (offset_bytes < 0)
            {
                break;
            }
            
            offset_size = (uint32)(offset_bytes + dst_width * (src_width - 1) + dst_width - 1 - src_height - 1) * data_byte_unit; /*lint !e737*/
            if (offset_size >= dst_size)
            {
                //SCI_TRACE_LOW:"[MMIDC] ConvertImageData size overflow!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1943_112_2_18_2_8_32_108,(uint8*)"");
                break;
            }   
            
            for (i = 0; i < src_height; i++)
            {
                for (j = 0; j < src_width; j++)
                {
                    *(dst_u16_ptr + offset_bytes + dst_width * j + dst_width - 1 - i) = *(src_u16_ptr++);
                }
            }
            
            ret = TRUE;
            break;            
        default:
            break;
        }
    }while(0);

    //SCI_TRACE_LOW:"[MMIDC] ConvertImageData offset_bytes=%d,offset_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1963_112_2_18_2_8_32_109,(uint8*)"dd",offset_bytes,offset_size);

    //SCI_TRACE_LOW:"[MMIDC] ConvertImageData ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_1965_112_2_18_2_8_32_110,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
// 	Description : get last frame callback while recording
//	Global resource dependence : none
//  Author: ryan.xu
//  Param:
//  Note:       src_ptr  frame data address
//              width    frame data width
//              height   frame data height
//
//  相关函数DRECORDER_RegisterGetLastFrameCallback()
//  
//  if not g-sensor then
//     if platform = angle 0
//        if camera sensor  = angle 0 ## displayed photo is angle 0 
//             if  osd = angle 0  then copy frame data to new buffer
//             if  osd = angle 90 then copy frame data to new buffer
//        if camera sensor = angle 90 ## displayed photo is angle 90
//             if  osd = angle 0  then convert frame data angle 270 to new buffer
//     if platform = angle 90
//        if camera sensor  = angle 0 ## displayed photo is angle 0 
//             if  osd = angle 90 then convert frame data angle 270 to new buffer
//        if camera sensor = angle 90 ## displayed photo is angle 90
//             if  osd = angle 90  then copy frame data to new buffer
//  ###convert angle to clockwise turn
/*****************************************************************************/
LOCAL void GetLastFrameCallback(DRECORDER_VIDEO_FRAME_T *p_lastframe)
{
#ifndef WIN32
    ISP_FRAME_T frame = {0};
    IMGPROC_YUV_RGB_CVT_IN_T src = {0};
    IMGPROC_YUV_RGB_CVT_OUT_T dst = {0};
    GUI_COLOR_T      bg_color = MMIDC_GetTransparentColor();
    GUI_LCD_DEV_INFO dev_info = MMIDC_GetOSDBlockInfo();
    GUI_POINT_T      dis_point = {0};
    uint16  width = 0;
    uint16  height = 0;
    uint32  frame_data_len = 0;
    DRECORDER_VIDEO_DATATYPE_E data_type = DRECORDER_VIDEO_DATATYPE_RAWRGB;

    uint16  lcd_width = 0;//MMIDC_GetPreviewWidth();
    uint16  lcd_height = 0;//MMIDC_GetPreviewHeight();
    uint32  lcd_buffer_size = 0;
    uint16  img_width = 0;
    uint16  img_height = 0;


    GUI_COLOR_T *dst_angle_ptr = PNULL;
    GUI_COLOR_T *src_angle_ptr = PNULL;
#if defined(MMIDC_F_SOFTWARE_ANGLE)
    LCD_ANGLE_E software_rotation_angle = MMIDC_GetSoftwareRotationAngle();
#else
    DCAMERA_ROTATION_MODE_E camera_angle = MMIAPIDC_GetCameraSensorAngle();
#endif

    LCD_ANGLE_E absolute_angle = 0;

    uint16 logic_width = 0;
    uint16 logic_height = 0;
    LCD_ID_E lcd_id = 0;

    LCD_ANGLE_E convert_angle = 0;
    BOOLEAN ret = FALSE;
    LCD_ANGLE_E logic_angle = 0;
    BOOLEAN is_around_color = FALSE;
    BOOLEAN is_keep_dst_logic_screen = FALSE;
    BOOLEAN is_show_from_first_line = TRUE;

    SCI_TRACE_LOW("w=%d h=%d addr=0x%x 0x%x\n", 
		p_lastframe->width, p_lastframe->height,
		p_lastframe->data.yuv2pannel.p_y, 
		p_lastframe->data.yuv2pannel.p_uv);

	frame.width = p_lastframe->width;
	frame.height = p_lastframe->height;
	frame.yaddr = p_lastframe->data.yuv2pannel.p_y;
	frame.uaddr = p_lastframe->data.yuv2pannel.p_uv;
	frame.type = p_lastframe->data_type;

	src.src_chn.chn0.ptr = frame.yaddr;
	src.src_chn.chn1.ptr = frame.uaddr;
	src.src_chn.chn0.size = frame.width*frame.height;
	src.src_chn.chn1.size = frame.width*frame.height/2;
	src.src_width = frame.width;
	src.src_height = frame.height;
	src.src_format = IMGREF_FORMAT_YUV420;
	src.target_format = IMGREF_FORMAT_RGB565;
	src.target_buf.ptr = SCI_ALLOCAZ(frame.width * frame.height * 2);
	src.target_buf.size = frame.width * frame.height * 2;

	if (src.target_buf.ptr == PNULL)
	{
		SCI_TRACE_LOW("fail to malloc memory for converter.");
		return;
	}

	if (SCI_SUCCESS != IMGPROC_YuvRgbConvert(&src, &dst))
	{
		SCI_TRACE_LOW("fail to convert yuv420 to RGB565");
		goto exit;
	}
	p_lastframe->data_type = DRECORDER_VIDEO_DATATYPE_RAWRGB;


    
#if 0 //def MMIDC_F_PDA_BIT_16
    GUI_COLOR_T *main_buffer_ptr = (GUI_COLOR_T*)GUILCD_GetMainLcdBufPtr();
#endif
    

    
	SCI_TRACE_LOW("@5 width=%d height=%d, addr 0x%x 0x%x\n", 
				p_lastframe->width,
				p_lastframe->height,
				p_lastframe->data.yuv2pannel.p_y,
				p_lastframe->data.yuv2pannel.p_uv);

    //get frame information
    data_type = p_lastframe->data_type;
    switch(data_type)
    {
    case DRECORDER_VIDEO_DATATYPE_RAWRGB:
#if defined(PLATFORM_UWS6121E)
        src_angle_ptr = (GUI_COLOR_T *)p_lastframe->data.onepannel.p_data;
        frame_data_len = p_lastframe->data.onepannel.len;
#else
        src_angle_ptr = (GUI_COLOR_T *)dst.output_chn.chn0.ptr;
        frame_data_len = dst.output_chn.chn0.size;
#endif
        break;
    default:
        //SCI_TRACE_LOW:"[MMIDC] GetLastFrameCallback data_type = %d !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2045_112_2_18_2_8_32_111,(uint8*)"d", data_type);
        goto exit;
    }
    width = p_lastframe->width;
    height = p_lastframe->height;
#if defined(PLATFORM_UWS6121E)
    width = 240;//temp code, input param width is incorrect. remove when dc correct it.
    height =240;
#endif
    lcd_id = MMIDC_GetLcdID();
    GUILCD_GetLogicWidthHeight(lcd_id, &logic_width, &logic_height);
    lcd_width = logic_width;
    lcd_height = logic_height;
    
    logic_angle = GUILCD_GetLogicAngle((LCD_ID_E)GUI_MAIN_LCD_ID);

    //get lcd frame buffer
    lcd_buffer_size = lcd_width * lcd_height * sizeof(GUI_COLOR_T);

    //SCI_TRACE_LOW:"[MMIDC] lcd_buffer_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2061_112_2_18_2_8_32_112,(uint8*)"d", lcd_buffer_size);

#if 0 //def MMIDC_F_PDA_BIT_16
    if (PNULL == main_buffer_ptr)
    {  
        //SCI_TRACE_LOW:"[MMIDC] main_buffer_ptr  is PNULL = 0x%lX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2066_112_2_18_2_8_32_113,(uint8*)"d", main_buffer_ptr);
        goto exit;
    }
    
    //set malloc buffer to background color 
    SCI_MEM16SET((uint16 *)main_buffer_ptr, bg_color, lcd_buffer_size);
    
    dst_angle_ptr = (GUI_COLOR_T *)main_buffer_ptr;
#else
    if (PNULL == s_lastframe_ptr)
    {
        s_lastframe_ptr = (GUI_COLOR_T *)SCI_ALLOCA(lcd_buffer_size);

        //SCI_TRACE_LOW:"[MMIDC] s_lastframe_ptr = 0x%lX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2079_112_2_18_2_8_32_114,(uint8*)"d", s_lastframe_ptr);

        if (PNULL == s_lastframe_ptr)
        {
            //MMIDC_ASSERT_WARNING:"[MMIDC] s_lastframe_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2083_112_2_18_2_8_32_115,(uint8*)"");
            goto exit;
        }
    }
    
    //set malloc buffer to background color 
    SCI_MEM16SET((uint16 *)s_lastframe_ptr, bg_color, lcd_buffer_size);
    
    dst_angle_ptr = (GUI_COLOR_T *)s_lastframe_ptr;
#endif

    //SCI_TRACE_LOW:"[MMIDC] dst_angle_ptr = 0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2094_112_2_18_2_8_32_116,(uint8*)"d", dst_angle_ptr);
    //SCI_TRACE_LOW:"[MMIDC] src_angle_ptr = 0x%lX, data_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2096_112_2_18_2_8_32_117,(uint8*)"dd",src_angle_ptr, frame_data_len);

    //MMIDC_ClearOSDBlock();
    //COPY DATA
    if (PNULL != src_angle_ptr)
    {
        #if defined(MMIDC_DESKTOP_PDA) || defined(MAINLCD_LOGIC_ANGLE_90)
            #if !defined(MMIDC_F_PDA_BIT_16)
            is_keep_dst_logic_screen = TRUE;
            #endif
        #endif


        //get resources width or height
#ifndef DC_WATCH_UI_SUPPORT
        {
            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_DC_WAIT, MMIDC_MAIN_WIN_ID);

            dis_point.x = (logic_width - img_width) / 2;
            dis_point.y = (logic_height - img_height) / 2;
            GUIRES_DisplayImg(&dis_point, PNULL, PNULL, MMIDC_MAIN_WIN_ID, IMG_DC_WAIT, &dev_info);
        }
#endif

        #if defined(MMIDC_F_SOFTWARE_ANGLE)
        {
            absolute_angle = software_rotation_angle;
        }    
        #else
        {
            switch(camera_angle)
            {
            case DCAMERA_ROTATION_MIRROR:
            case DCAMERA_ROTATION_FLIP:
                camera_angle = 0;
                break;
            default:
                break;
            }

            absolute_angle = (LCD_ANGLE_E)camera_angle;
        }    
        #endif


        //SCI_TRACE_LOW:"[MMIDC] width = %d, height = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2138_112_2_18_2_8_32_118,(uint8*)"dd", width, height);
        //SCI_TRACE_LOW:"[MMIDC] logic_width = %d, logic_height = %d, logic_angle=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2140_112_2_18_2_8_32_119,(uint8*)"ddd",logic_width, logic_height, logic_angle);

        //SCI_TRACE_LOW:"[MMIDC] absolute_angle=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2142_112_2_18_2_8_32_120,(uint8*)"d",absolute_angle);
                  
        //SCI_TRACE_LOW:"[MMIDC] is_keep_dst_logic_screen=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2145_112_2_18_2_8_32_121,(uint8*)"d",is_keep_dst_logic_screen);
                  
#ifdef MAINLCD_LOGIC_ANGLE_0

        //SCI_TRACE_LOW:"[MMIDC] MAINLCD_LOGIC_ANGLE_0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2149_112_2_18_2_8_32_122,(uint8*)"");

        switch(absolute_angle)
        {
        case LCD_ANGLE_0:           
            switch(logic_angle)
            {
            case LCD_ANGLE_90:
                if (is_keep_dst_logic_screen) /*lint !e774*/
                {                    
                    convert_angle = LCD_ANGLE_90;
                    is_around_color = TRUE;
                }
                else
                {
                    lcd_width = logic_height;
                    lcd_height = logic_width;
                    convert_angle = LCD_ANGLE_0;
                }
                break;
            case LCD_ANGLE_0:
                convert_angle = LCD_ANGLE_0;
                break;
            default:
                break;
            }            
            break;
        case LCD_ANGLE_90:
            switch(logic_angle)
            {
            case LCD_ANGLE_0:
                convert_angle = LCD_ANGLE_270;
                break;
            case LCD_ANGLE_90:
                if (is_keep_dst_logic_screen) /*lint !e774*/
                {
                    convert_angle = LCD_ANGLE_0;
                    is_around_color = TRUE;
                }
                else
                {
                    lcd_width = logic_height;
                    lcd_height = logic_width;
                    convert_angle = LCD_ANGLE_270;
                }
                break;
            default:
                break;
            }
            break;
        case LCD_ANGLE_180:
            switch(logic_angle)
            {
            case LCD_ANGLE_90:
                if (is_keep_dst_logic_screen) /*lint !e774*/
                {
                    convert_angle = LCD_ANGLE_270;
                    is_around_color = TRUE;
                }
                else
                {
                    lcd_width = logic_height;
                    lcd_height = logic_width;
                    convert_angle = LCD_ANGLE_0;
                }
                break;
            case LCD_ANGLE_0:
                convert_angle = LCD_ANGLE_0;
                break;
            default:
                break;
            }
            break;
        case LCD_ANGLE_270:
            switch(logic_angle)
            {
            case LCD_ANGLE_0:
                convert_angle = LCD_ANGLE_270;
                break;
            case LCD_ANGLE_90:
                if (is_keep_dst_logic_screen) /*lint !e774*/
                {
                    convert_angle = LCD_ANGLE_180;
                }   
                else
                {
                    lcd_width = logic_height;
                    lcd_height = logic_width;
                    convert_angle = LCD_ANGLE_270;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
#elif MAINLCD_LOGIC_ANGLE_90

        //SCI_TRACE_LOW:"[MMIDC] MAINLCD_LOGIC_ANGLE_90"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2249_112_2_18_2_8_32_123,(uint8*)"");

        is_around_color = TRUE;
        
        switch(absolute_angle)
        {
        case LCD_ANGLE_0:
            {
                switch(logic_angle)
                {
                case LCD_ANGLE_90:
#if defined MAINLCD_DEV_SIZE_128X160 || defined MAINLCD_DEV_SIZE_176X220 || defined MAINLCD_DEV_SIZE_240X320					
					//假横屏
					if(!MMIDC_IsRealHorLcd())
					{
						convert_angle = LCD_ANGLE_90;
					}
					else
#endif
					{
						convert_angle = LCD_ANGLE_0;
					}
                    break;
                default:
                    convert_angle = LCD_ANGLE_90;
                    break;
                }
            }
            break;
        case LCD_ANGLE_90:
            {
                is_around_color = FALSE;
                switch(logic_angle)
                {
                case LCD_ANGLE_90:
                    convert_angle = LCD_ANGLE_270;
                    break;
                default:
                    convert_angle = LCD_ANGLE_180;
                    break;
                }
            }
            break;
        case LCD_ANGLE_180:  
                switch(logic_angle)
                {
                case LCD_ANGLE_90:
                    convert_angle = LCD_ANGLE_0;
                    break;
                default:
                    convert_angle = LCD_ANGLE_270;
                    break;
                }
            break;
        case LCD_ANGLE_270:
                is_around_color = FALSE;
                switch(logic_angle)
                {
                case LCD_ANGLE_90:
                    convert_angle = LCD_ANGLE_270;
                    break;
                default:
                    convert_angle = LCD_ANGLE_180;
                    break;
                }
            break;
        default:
            convert_angle = LCD_ANGLE_0;
            break;
        }
#endif
#ifdef PLATFORM_ANTISW3
	convert_angle = Sensor_GetRotat(0);
#endif
    ret = ConvertImageData((uint8 *)dst_angle_ptr,\
               lcd_buffer_size,\
               lcd_width,\
               lcd_height,\
               (uint8 *)src_angle_ptr,\
               frame_data_len,\
               width,\
               height,\
               data_type,\
               convert_angle, \
               is_around_color, \
               is_show_from_first_line
               );
    }
                
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    MMIDC_GetA888LastFrameData(dst_angle_ptr, lcd_width, lcd_height);
#endif

    //MMIDC_CompleteDisplayOSD();
exit:
	if(src.target_buf.ptr)
		SCI_FREE(src.target_buf.ptr);
#endif
}

#ifdef MMIDC_F_LAST_IMAGE
/*****************************************************************************/
// 	Description : get last thumbnail callback
//	Global resource dependence : none
//  Author: chunyou
//  Param:
//  Note:
/*****************************************************************************/
LOCAL void GetLastThumbnailCallback(DRECORDER_VIDEO_FRAME_T *p_last_thumbnail)
{
    uint32          frame_data_len = 0;
    DRECORDER_VIDEO_DATATYPE_E data_type = DRECORDER_VIDEO_DATATYPE_RAWRGB;
    GUI_COLOR_T *src_angle_ptr = PNULL;
    MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();
    wchar *path_name = PNULL;
    
    if(PNULL == p_last_thumbnail
        || PNULL == img_info_ptr)
    {
        SCI_TRACE_LOW("[MMIDC] GetLastThumbnailCallback p_last_thumbnail is PNULL");
        return;
    }

    data_type = p_last_thumbnail->data_type;
    switch(data_type)
    {
    case DRECORDER_VIDEO_DATATYPE_RAWRGB:
        src_angle_ptr = (GUI_COLOR_T *)p_last_thumbnail->data.onepannel.p_data;
        frame_data_len = p_last_thumbnail->data.onepannel.len;
        break;
    default:
        SCI_TRACE_LOW("[MMIDC] GetLastThumbnailCallback data_type = %d !", data_type);
        return;
    }

    if(PNULL != img_info_ptr->img_data_buffer
        && PNULL != src_angle_ptr
        && p_last_thumbnail->height != 0
        && p_last_thumbnail->width != 0
        && img_info_ptr->img_width >= p_last_thumbnail->width
        && img_info_ptr->img_height >= p_last_thumbnail->height)
    {
        img_info_ptr->is_img_data_valid = TRUE;

        img_info_ptr->actual_width = p_last_thumbnail->width;
        img_info_ptr->actual_height = p_last_thumbnail->height;

        path_name = MMIDC_GetVideoFullFileName();
        MMI_WSTRNCPY(img_info_ptr->full_path_name, 
            MMIDC_FULL_PATH_LENGTH, 
            path_name, 
            MMIAPICOM_Wstrlen((const wchar *) path_name), 
            MMIAPICOM_Wstrlen((const wchar *) path_name));

        SCI_MEMSET(img_info_ptr->img_data_buffer, 0x00, img_info_ptr->img_data_size);
#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
        {
            uint32 i = 0;
            uint32 j = 0;
            uint32 k = 0;
            
            for (i = 0; i < p_last_thumbnail->height; i++)
            {
                for (j = 0; j < p_last_thumbnail->width; j++)
                {
                    img_info_ptr->img_data_buffer[k] = RGB5652ARGB888(src_angle_ptr[k], 255); /*lint !e648*/
                    k++;
                }
            }
        }
#else
        MMI_MEMCPY(img_info_ptr->img_data_buffer, 
                    img_info_ptr->img_data_size, 
                    src_angle_ptr, 
                    frame_data_len, 
                    frame_data_len);
#endif
    }

    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailCallback p_last_thumbnail->width = %d !", p_last_thumbnail->width);
    SCI_TRACE_LOW("[MMIDC] GetLastThumbnailCallback p_last_thumbnail->height = %d !", p_last_thumbnail->height);    
}
#endif

/*****************************************************************************/
// 	Description : start to record
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StartRecord(void)
{
    DRECORDER_START_RECORD_T recorder_start_t = {0};
    MEDIA_AUDIO_INFO_T	audio_info = {0};
    MEDIA_VIDEO_INFO_T	video_info = {0};
    DRECORDER_RETURN_E			ret = 0; /*lint !e64*/

    MMIDC_SIZE_ATTRIBUTE_T size_val = {0};
    VIDEO_SIZE_E video_size = 0;
    VIDEO_FILE_TYPE_E file_type = 0;

    uint32 file_limit_size = 0;
    
    //SCI_TRACE_LOW:"[MMIDC]: StartRecord"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2310_112_2_18_2_8_33_124,(uint8*)"");
    
    MMIAPISET_InitAudioDevice(AUD_MAX_SPEAKER_VOLUME);
    
    s_has_been_record = TRUE;
    /* set audio info */
    audio_info.audio_support	= (AUDIO_SETTING_ON == MMIDC_GetVideoAudioSetting());	
    audio_info.channels		= AUDIO_CHANNEL;
    audio_info.sample_rate		= AUDIO_SAMPLE_RATE;
#ifdef AVI_ENC_SUPPORT
    if(VIDEO_FILE_TYPE_AVI == MMIDC_GetRecordFileType())
    {
        audio_info.audio_type = AUDIO_TYPE_IMAADPCM;
    }
    else
    {
        audio_info.audio_type = AUDIO_TYPE_AMR;
    }
#else
    audio_info.audio_type = AUDIO_TYPE_AMR;
#endif

    
    /* set video info */
    file_type = MMIDC_GetRecordFileType();
    switch(file_type)
    {
    case VIDEO_FILE_TYPE_3GP:
        #if defined(TYPE_3GP_SUPPORT_MP4)
            video_info.video_type	= VIDEO_TYPE_MP4V;
        #else
            video_info.video_type	= VIDEO_TYPE_MJPEG;
        #endif

        recorder_start_t.file_type = DRECORDER_FILETYPE_3GP;
        break;
    case VIDEO_FILE_TYPE_MP4:
        video_info.video_type	= VIDEO_TYPE_H264;//VIDEO_TYPE_MP4V;
        recorder_start_t.file_type = DRECORDER_FILETYPE_MP4;
        break;
    case VIDEO_FILE_TYPE_AVI:
        video_info.video_type	= VIDEO_TYPE_MJPEG;
        recorder_start_t.file_type = DRECORDER_FILETYPE_AVI;
        break;
    default:
        //MMIDC_ASSERT_WARNING:"[MMIDC] StartRecord NO TYPE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2355_112_2_18_2_8_33_125,(uint8*)"");
        video_info.video_type	= VIDEO_TYPE_MJPEG;
        recorder_start_t.file_type = DRECORDER_FILETYPE_AVI;
        break;
    }
#ifdef WIN32
    video_info.video_type	= VIDEO_TYPE_MJPEG;
    recorder_start_t.file_type = DRECORDER_FILETYPE_3GP;
    audio_info.audio_support = 0;
#endif
    video_info.frame_rate	= (uint32)MMIAPIENG_GetDVFrameFre();

    video_size = MMIDC_GetVideoSize();

    MMIDC_GetVideoSizeWidthAndHeight(video_size, &size_val);

    video_info.width = 240;//size_val.size_width;
    video_info.height = 240;//size_val.size_height;

    video_info.bitrate = 1000000;

    video_info.video_support = TRUE;

    SCI_TRACE_LOW("mmidc_flow, %s, %d, video_type=%d,file_type=%d,frame_rate=%d,width=%d,heiht=%d",
        __FUNCTION__, __LINE__,video_info.video_type, recorder_start_t.file_type,
        video_info.frame_rate,video_info.width,video_info.height);
    recorder_start_t.audio_info_ptr		= &audio_info;
    recorder_start_t.video_info_ptr		= &video_info;
    recorder_start_t.frame_callback		= PNULL;
    recorder_start_t.end_callback		= (DRECORDER_END_CALLBACK)RecordEndNotifyCallBack;
    

    if(!MMIDC_CreateVideoName(&recorder_start_t))
    {
        return FALSE;
    }

#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        file_limit_size = MMIDC_GetLimitedVideoFileSizeOfWorkMode();
    }
#endif

    recorder_start_t.file_limit_size = file_limit_size;
    
    if (PNULL == recorder_start_t.audio_info_ptr)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] StartRecord recorder_start_t.audio_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2394_112_2_18_2_8_33_126,(uint8*)"");
        return FALSE;
    }
    if (PNULL == recorder_start_t.video_info_ptr)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC]StartRecord recorder_start_t.video_info_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2399_112_2_18_2_8_33_127,(uint8*)"");
        return FALSE;
    }
    if (PNULL == recorder_start_t.file_name_ptr)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] StartRecord recorder_start_t.file_name_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2404_112_2_18_2_8_33_128,(uint8*)"");
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"[MMIDC]: file_name_ptr = %d, file_name_byte_len = %d, file_type = %d,  video_info.height = %d, video_info.width = %d, video_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2410_112_2_18_2_8_33_129,(uint8*)"dddddd", recorder_start_t.file_name_ptr, recorder_start_t.file_name_byte_len,recorder_start_t.file_type, video_info.height, video_info.width, video_info.video_type);

    //register getlastframecallback
    ret = DRECORDER_RegisterGetLastFrameCallback(GetLastFrameCallback, DRECORDER_VIDEO_DATATYPE_RAWRGB);    
    //SCI_TRACE_LOW:"[MMIDC] DRECORDER_RegisterGetLastFrameCallback ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2414_112_2_18_2_8_33_130,(uint8*)"d", ret);

#ifdef MMIDC_F_LAST_IMAGE
    {
        DRECORDER_THUMBNAIL_PARAM_T thumbnail_param = {0};
        MMIDC_IMG_INFO_T* img_info_ptr = MMIDC_GetLastThumbnailInfoPtr();

        if(PNULL != img_info_ptr)
        {
            thumbnail_param.width = img_info_ptr->img_width;
            thumbnail_param.height = img_info_ptr->img_height;
            thumbnail_param.data_type = DRECORDER_VIDEO_DATATYPE_RAWRGB;
            ret = DRECORDER_RegisterGetLastThumbnailCallback(GetLastThumbnailCallback, &thumbnail_param);
            SCI_TRACE_LOW("[MMIDC] DRECORDER_RegisterGetLastThumbnailCallback ret = %d", ret);
        }        
    }
#endif
	if(MMISRVAUD_IsHeadSetPlugIn()) 
	{
		MMISRVAUD_SetRoute(MMIDC_GetAudioHandle(), MMISRVAUD_ROUTE_EARPHONE);
	}
    ret = DRECORDER_StartRecord(&recorder_start_t);
    //SCI_TRACE_LOW:"[MMIDC]: DRECORDER_StartRecord = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2417_112_2_18_2_8_33_131,(uint8*)"d", ret);
    //added, @robert.wang, 09-9-5, cr149292
   if(DRECORDER_ERROR_FILE_OPENING == ret)
    {
	   if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED,
                        		   MMIDC_GetVideoStorageDevice(),
                        		   MMI_DC_MIN_SPACE_SIZE,
                        		   PNULL))///if(!MMIFMM_IsEnoughSpace(MMI_DC_MIN_SPACE_SIZE))

        {
            MMIDC_ShowAppointedErrorMsg(SFS_ERROR_NO_SPACE);
        }
        else
        {
            MMIDC_ErrorTip(TXT_DC_FILE_CREATED_FAIL);
        }
        return FALSE;
        
    //end cr149292
   }else if(DRECORDER_SUCCESS != ret)
    {
        MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
        return FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDC]: StartRecord - return success !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2443_112_2_18_2_8_33_132,(uint8*)"");
    }
//xxx
//#ifdef WIN32
//    s_dv_is_record = FALSE;
//#endif
    return TRUE;
}

/*****************************************************************************/
// 	Description : set all preview video param
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void SetDVAllPreviewParam(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    
    //SCI_TRACE_LOW:"[MMIDC]: SetDVAllPreviewParam = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2460_112_2_18_2_8_33_133,(uint8*)"d", s_dv_is_open);
    
    if(s_dv_is_open)
    {
        MMIDC_SetVideoBrightness(MMIDC_GetVideoBrightness());
        MMIDC_SetVideoContrast(MMIDC_GetVideoContrast());
        MMIDC_SetVideoFlicker(MMIDC_GetVideoFlicker());
#ifndef WIN32
        if(MMIDC_GetRecordFileType() == VIDEO_FILE_TYPE_MP4)
        {
            ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_VIDEO_MODE, VIDEO_TYPE_MP4V, PNULL);
        }
        else if(MMIDC_GetRecordFileType() == VIDEO_FILE_TYPE_AVI)
        {
            ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_VIDEO_MODE, VIDEO_TYPE_MJPEG, PNULL);
        }
        else if (MMIDC_GetRecordFileType() == VIDEO_FILE_TYPE_3GP)
        {
            #if defined(TYPE_3GP_SUPPORT_MP4)
            ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_VIDEO_MODE, VIDEO_TYPE_MP4V, PNULL);
            #else
            ret = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_VIDEO_MODE, VIDEO_TYPE_MJPEG, PNULL);
            #endif
        }
#endif
        //SCI_TRACE_LOW:"[MMIDC]: SetDVAllPreviewParam ret= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2485_112_2_18_2_8_33_134,(uint8*)"d", ret);

        MMIDC_SetVideoEffect(MMIDC_GetVideoEffect());
    }
}

/*****************************************************************************/
// 	Description : start to preview video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StartDVPreview(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    DRECORDER_PREVIEW_PARAM_T record_param = {0};

    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    DCAMERA_ROTATION_MODE_E angle = 0;
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();    
    //SCI_TRACE_LOW:"[MMIDC]: StartDVPreview = %d, = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2504_112_2_18_2_8_33_135,(uint8*)"dd", s_dv_is_preview, s_dv_is_open);

    MMIDC_ResetHitTimer();
    
    if(!s_dv_is_preview && s_dv_is_open)
    {			
        //mmidcsrv
        MMIDCSRV_CheckParam();
        
    
		MMIDC_GetPhysicalLCDWidthAndHeight(&lcd_width, &lcd_height, &angle);

        MMIDC_InitSettingParamCtrl();

        record_param.LcdId = MMIDC_GetLcdID();//GUI_MAIN_LCD_ID;
        record_param.disp_rect.x = 0;
        record_param.disp_rect.y = 0;
        //record_param.disp_rect.dx = lcd_width;
        //record_param.disp_rect.dy = lcd_height;
        record_param.disp_rect.dx = DP2PX_VALUE(240);
        record_param.disp_rect.dy = DP2PX_VALUE(240);

        record_param.target_rect= record_param.disp_rect;
            
        record_param.DisplayRotateAngle = angle;

#if defined MAINLCD_LOGIC_ANGLE_90 && (defined MAINLCD_DEV_SIZE_128X160 || defined MAINLCD_DEV_SIZE_176X220 || defined MAINLCD_DEV_SIZE_240X320)		
		//假横屏
		if (DCAMERA_ROTATION_0 == MMIAPIDC_GetCameraSensorAngle()
			&& LCD_ANGLE_90 == MMIDC_GetPlatformAngle()
			&& (DCAMERA_IMG_SENSOR_TYPE_4P3 == sensor_type)
			&& !MMIDC_IsRealHorLcd()
            ) 
		{
#if defined LCD_ROT_MODE_LCD180		
			record_param.EncRotateAngle = DCAMERA_ROTATION_270;
#else			
			record_param.EncRotateAngle = DCAMERA_ROTATION_90;
#endif
		}
		else
#endif
		{
			record_param.EncRotateAngle = angle;
		}
        
        if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
        {
            record_param.EncRotateAngle = MMIDC_GetCapAngleFromDCam();
        }
        
        record_param.display_mode = (DRECORDER_DISP_MODE_E)MMIDCSRV_GetDVDisplayMode();//MMIDC_GetDefaultVisualMode();
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
        record_param.EncRotateAngle = (uint32)LCD_GetWearType();
#endif
        #if 0
        switch(MMIDCSRV_GetDVMemCfg())
        {
        case MMIDCSRV_MEM_DV_MINI:
            #if 0
            {
                uint16 logic_width = 0;
                uint16 logic_height = 0;
                uint16 target_width = 0;
                uint16 target_height = 0;
                int32 lcd_size = 0;
                
                lcd_size = MMIDC_GetLCDSize();

                if (MMIDC_HVGA_PREVIEW_HEIGHT == lcd_size)
                {
                    logic_width = 320;
                    logic_height = 240;
                    
                    MMIDC_GetPhysicalLCDWidthAndHeightEx(logic_width, logic_height,
                                                        &target_width, &target_height,
                                                        &angle
                                                        );

                    record_param.disp_rect.x = (lcd_width - target_width) / 2;
                    record_param.disp_rect.y = (lcd_height - target_height) / 2;
                    record_param.disp_rect.dx = target_width;
                    record_param.disp_rect.dy = target_height;                
                    record_param.target_rect= record_param.disp_rect;
                }
            }
            #endif
            break;
        default:
            break;
        }
        #endif

    //SCI_TRACE_LOW:"[MMIDC]: StartDVPreview lcd_width= %d, lcd_height= %d, angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2529_112_2_18_2_8_33_136,(uint8*)"ddd",lcd_width, lcd_height, record_param.DisplayRotateAngle);

        
        record_param.LcdBlkId = 0;
        record_param.image_format = MMIAPIENG_GetSensorMode();	
        SetDVAllPreviewParam();
        ret = DRECORDER_StartPreview(&record_param);
        //SCI_TRACE_LOW:"[MMIDC]: DRECORDER_StartPreview = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2536_112_2_18_2_8_33_137,(uint8*)"d", ret);
        if(DRECORDER_SUCCESS == ret)
        {
            s_dv_is_preview = TRUE;
            
#if defined(MMIDC_F_SOFTWARE_ANGLE)
            MMIDC_GetDCModuleSoftwareRotationAngle();
#endif            
        }		
        else
        {
            MMIDC_PostStateMsg();
            MMIDC_ErrorTipForExit(TXT_DC_OPEN_FAIL);
            return FALSE;
        }
    }


    return TRUE;
}

/*****************************************************************************/
// 	Description : stop preview video
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void StopDVPreview(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;	
    
    //SCI_TRACE_LOW:"[MMIDC]: StopPreview = %d, = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2565_112_2_18_2_8_33_138,(uint8*)"dd", s_dv_is_preview, s_dv_is_open);
    
    MMIDC_InitParamCtrl();

    if(s_dv_is_preview && s_dv_is_open)
    {
        ret = DRECORDER_StopPreview();
        //SCI_TRACE_LOW:"[MMIDC]: DRECORDER_StopPreview = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2572_112_2_18_2_8_33_139,(uint8*)"d", ret);
        if(DRECORDER_SUCCESS == ret)
        {
            s_dv_is_preview = FALSE;
        }
        else
        {
            MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
        }
    }
}

/*****************************************************************************/
// 	Description : open dv recorder
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenDV(void)
{
    BOOLEAN ret = FALSE;
    SETTING_SENSOR_ID_E sensor_id = 0;
#ifdef MMIDC_F_FLASH
    DCAMERA_FLASH_MODE_E flash = DCAMERA_FLASH_OFF;
#endif
    
    MMIDC_FreeLastFrame();

    ret = MMIDC_OpenDV();
    
    //SCI_TRACE_LOW:"[MMIDC]: OpenDV = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2597_112_2_18_2_8_33_140,(uint8*)"d", ret);
    
    if(ret)
    {
        SetLCDForRotation();
        MMIDC_SetBlock();

        sensor_id = MMIDC_GetSettingSensorID();
        MMIDC_SetDCameraSensorID(sensor_id);

#ifdef MMIDC_F_FLASH
        if(SETTING_SENSOR_FRONT == sensor_id)
        {
            MMIDC_SetFlashLamp(DCAMERA_FLASH_NONE);
        }
        else
        {
            flash = MMIDC_GetFlashLamp();
            MMIDC_SetFlashLamp(flash);
        }
#endif

#if defined(MMIDC_F_DV_ZOOM)
        MMIDC_UpdateDVZoomValue(MMIDC_GetDVZoomValue());
#endif		


#ifdef MMIDC_MINI_RESOURCE_SIZE

        MMIDC_SetSettingVideoSizeInfo();
#endif
    }
    else
    {
        MMIDC_PostStateMsg();
        MMIDC_ErrorTipForExit(TXT_DC_OPEN_FAIL);
    }
    
    return ret;
}

#if 0 //def MMIDC_F_PDA_BIT_16
/*****************************************************************************/
// 	Description : backup last dv frame data
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  相关函数MMIDC_GetLastFrameCallback()
/*****************************************************************************/
LOCAL void MMIDC_CopyLastFrame(void)
{
    uint16 screen_width = 0;
    uint16 screen_height = 0;
    uint32 buffer_size = 0;
    LCD_ID_E lcd_id = 0;
    uint16  *main_buffer_ptr = GUILCD_GetMainLcdBufPtr();

    lcd_id = MMIDC_GetLcdID();
    GUILCD_GetLogicWidthHeight(lcd_id, &screen_width, &screen_height);
    buffer_size = screen_width * screen_height * sizeof(GUI_COLOR_T);

    if (PNULL == s_lastframe_ptr)
    {
        s_lastframe_ptr = (uint16 *)SCI_ALLOCA(buffer_size);       
    }    
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CopyLastFrame s_lastframe_ptr = 0x%lX, buffer_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2642_112_2_18_2_8_33_141,(uint8*)"dd",(uint32)s_lastframe_ptr, buffer_size);

    //SCI_TRACE_LOW:"[MMIDC] main_buffer_ptr = 0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2644_112_2_18_2_8_33_142,(uint8*)"d",(uint32)main_buffer_ptr);
             
    if (PNULL != s_lastframe_ptr)
    {
        SCI_MEMCPY(s_lastframe_ptr, main_buffer_ptr, buffer_size);
#ifdef WIN32
        SCI_MEM16SET((uint16 *)s_lastframe_ptr, MMIDC_GetTransparentColor(), buffer_size);
#endif
    }
    else
    {
        SCI_MEM16SET((uint16 *)main_buffer_ptr, MMIDC_GetTransparentColor(), buffer_size);
    }
}
#endif

/*****************************************************************************/
// 	Description : close dv recorder
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void CloseDV(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

    //SCI_TRACE_LOW:"[MMIDC]: CloseDV = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2668_112_2_18_2_8_33_143,(uint8*)"d", s_dv_is_open);

    MMIDC_InitParamCtrl();

    if(s_dv_is_open)
    {
        ret = DRECORDER_Close();
        //SCI_TRACE_LOW:"[MMIDC]: DRECORDER_Close = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2675_112_2_18_2_8_33_144,(uint8*)"d", ret);
        if(DRECORDER_SUCCESS == ret)
        {
            s_dv_is_open = FALSE;
        }
        else
        {
            MMIDC_ErrorTip(TXT_USSD_REQUEST_UNCOMPLETED);
        }
#if 0
        MMIDC_CopyLastFrame(); 
#endif		
        MMIDC_RestoreBlock();     
    }
    if(s_has_been_record)
    {
        //AUD_SetDevMode(s_audio_dev_mode);
        s_has_been_record = FALSE;
    }

}

/*****************************************************************************/
// 	Description : start to multi shoot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BurstShot(uint32 shot_num)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    DCAMERA_BURST_SNAPSHOT_PARAM_T burstshot_param = {0};/*lint !e64*/
    DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T burstshot_return = {0};
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type = MMIAPIDC_GetDCModulSensorType();
    
    //SCI_TRACE_LOW:"[MMIDC]: BurstShot %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2708_112_2_18_2_8_33_145,(uint8*)"d", shot_num);
    s_is_review_full_screen = FALSE;
    MMIDC_ResetCurrentPhotoID();
    
    if (shot_num > MAX_DC_BURST_PHOTO_NUMBER)
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] BurstShot shot_num OVER 9"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2714_112_2_18_2_8_33_146,(uint8*)"");
        shot_num = DC_BURST_3_PHOTO;
    }
    
    SCI_MEMSET(&(s_photo_capture_info), 0x00, sizeof(DC_PHOTO_CAPTURE_INFO));
    s_photo_capture_info.photo_total = shot_num;
    s_photo_capture_info.captured_photo_num = 0;
    
    burstshot_param.data_type			= DCAMERA_DATA_TYPE_JPEG;
    burstshot_param.buf_len				= 0;
    burstshot_param.data_buf			= PNULL;
    burstshot_param.file_name			= PNULL;
    burstshot_param.burst_snapshot_callback	= (DCAMERA_CALL_BACK)CaptureCallBack;
    burstshot_param.burst_capture_start_callback = NotifyStartSnapshotCallback;
    burstshot_param.quickview_callback = (DCAMERA_QUICK_VIEW_CALL_BACK)SetReviewOSDCallback;
    
#ifdef MMI_CAMERA_F_PHOTO_DATE
    MMIDC_GetPhotoDateParameter(&burstshot_param.capture_date);
#endif

    if((DCAMERA_ROTATION_90 == MMIAPIDC_GetCameraSensorAngle() 
         || DCAMERA_ROTATION_270 == MMIAPIDC_GetCameraSensorAngle() 
        )
        && MMIDC_GetPhotoSize() == MMIDC_GePhotoSizetVerLcd()
       )
    {
        burstshot_param.rotation_mode = DCAMERA_ROTATION_90;
    }
    else
    {
        burstshot_param.rotation_mode = DCAMERA_ROTATION_0;
    }

    if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
    {
        burstshot_param.rotation_mode = MMIDC_GetCapAngleFromDCam();
    }
    
    switch(MMIDC_GetPhotoQuality())
    {
    case PHOTO_QUALITY_BRIEF:						// 简洁
        burstshot_param.enc_quality_level	= DC_PHOTO_QUALITY_3;
        break;
    case PHOTO_QUALITY_NORMAL:						// 普通
        burstshot_param.enc_quality_level	= DC_PHOTO_QUALITY_6;
        break;
    case PHOTO_QUALITY_ADVANCED:					// 高级
        burstshot_param.enc_quality_level	= DC_PHOTO_QUALITY_8;
        break;
    default:
        //MMIDC_ASSERT_WARNING:"[MMIDC] BurstShot Quality"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2757_112_2_18_2_8_33_147,(uint8*)"");
        burstshot_param.enc_quality_level	= DC_PHOTO_QUALITY_8;
        break;
    }	
    burstshot_param.image_count			= shot_num;
    burstshot_param.flash_enable		= 0;
    burstshot_param.interval_of_frame	= DC_FRAME_INTERVAL;

    //SCI_TRACE_LOW:"[MMIDC]: BurstShot rotaiton_mode: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2765_112_2_18_2_8_33_148,(uint8*)"d", burstshot_param.rotation_mode);
    
    //SCI_TRACE_LOW:"[MMIDC]: DCAMERA_DoBurstSnapshot rotation_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2767_112_2_18_2_8_33_149,(uint8*)"d", burstshot_param.rotation_mode);
    
    ret = DCAMERA_DoBurstSnapshot(&burstshot_param, &burstshot_return);	
    //SCI_TRACE_LOW:"[MMIDC]: DCAMERA_DoBurstSnapshot return: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2770_112_2_18_2_8_33_150,(uint8*)"d", ret);
    return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : stop record video
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StopRecord(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;

    if (MMIDC_IsDVOpen())
    {
        ret = DRECORDER_StopRecord();
    }

    //SCI_TRACE_LOW:"[MMIDC]: DRECORDER_StopRecord return: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_2787_112_2_18_2_8_33_151,(uint8*)"d", ret);

    return (DRECORDER_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN Capture(void)
{
    s_is_capture = TRUE;
    s_is_snapshot_succss = TRUE;
    MMIDC_SetCurrentMode(DC_CAPTURE_MODE);

    StopDCPreview();

#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_SMILE_AUTO_PHOTO)
    if(MMIDC_GetUCameraModuleFlag()
        && IMG_PROC_FACE_DECT == MMIDC_GetImageProcessType())
    {
        MMIDC_DestroySmileAutoPhotoHandle();
    }
#endif

    MMIDC_StartCallbackTimer();

    ResetDCImageInfo();
    MMIDC_InitPhotoStoreInfo();

    MMIDC_ClearOSDBlock();
/*    if(MULTI_SHOOT_DISABLE == MMIDC_GetMultiShootEnum())
    {
        if(MMIDC_GetIsAutoSave() == AUTO_SAVE_OFF)
        {
           MMIDC_DisplayQuickViewSoftkey();
        }
    }*/

#if defined(WIN32)
    SetReviewOSDCallback();
#endif


    if(MMIDC_GetMultiShootEnum() != MULTI_SHOOT_DISABLE)
    {	
        if(!BurstShot(MMIDC_GetMulitShotNumber()))
        {
            MMIDC_ErrorTip(TXT_DC_CAPTURE_FAILED);
        }
    }
    else
    {
        if(!SnapShot())
        {
            MMIDC_ErrorTip(TXT_DC_CAPTURE_FAILED);
        }
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN Review(void)
{
    if(!s_is_capture && s_is_snapshot_succss)
    {
        MMIDC_SetCurrentMode(DC_REVIEW_MODE);

        MMIDC_ResetCurrentPhotoID();

        if ((MMIDC_GetMultiShootEnum() != MULTI_SHOOT_DISABLE) 
            #ifdef MMIDC_F_U_CAMERA
            || MMIDC_GetUCameraModuleFlag()
            #endif
            )
        {
            MMIDC_StartCallbackTimer();
            
            MMIDC_ClearOSDBlock();            
            
            MMIDC_ReviewPhotos();
        }
        else
        {
            MMK_PostMsg(MMIDC_MAIN_WIN_ID, MSG_DC_REVIEW_SUCCESS_MSG, (DPARAM)PNULL, 0);
        }

    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN Save(void)
{
    MMIFILE_ERROR_E  ret = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E  file_dev = 0;
    BOOLEAN is_save = TRUE;

    MMIDC_SetCurrentMode(DC_SAVE_MODE);

    if(MMIDC_GetMultiShootEnum() == MULTI_SHOOT_DISABLE)
    {
        ret = MMIDC_SavePhotos(0);
        if (SFS_ERROR_NONE != ret)
        {
            if(!MMIDC_GetRenameFileCircleFlag())
            {
                file_dev = MMIDC_GetPhotoStorageDevice();
                MMIDC_ShowErrorMsg(ret, file_dev);
                is_save = FALSE;
            }
            else
            {
                is_save = FALSE;
                return is_save;
            }
        }
    }
    else
    {
        int32 i = 0;
        for (i = 0; i < MMIDC_GetCapturedPhotosNumber(); i ++)
        {
            ret = MMIDC_SavePhotos(i);
            if (SFS_ERROR_NONE != ret)
            {
                if(!MMIDC_GetRenameFileCircleFlag())
                {
                    file_dev = MMIDC_GetPhotoStorageDevice();
                    MMIDC_ShowErrorMsg(ret, file_dev);
                    is_save =  FALSE;
                }
                else
                {
                    is_save = FALSE;
                    return is_save;
                }
            }
        }
    }

    MMIDC_PostCreateFileHandleMsg();
    if (!is_save)
    {
        return is_save;
    }

    s_is_review = TRUE;

#if defined(MMIDC_F_WORK_MODE)
    if (MMIDC_IsWorkMode())
    {
        MMIDC_SaveWorkFile();
        MMIDC_SetCurrentMode(DC_REVIEW_MODE);
        return TRUE;
    }
#endif

    MMIDC_StartCallbackTimer();

    MMIDC_ClearOSDBlock();
    MMIDC_ReviewPhotos();

    return TRUE;
}
/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN Record(void)
{
    BOOLEAN ret = FALSE;
    MMIFILE_ERROR_E   fs_error = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E      file_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E      suitable_dev = MMI_DEVICE_SDCARD;
    DC_FLOW_MODE_E mode = 0;
    
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
    //SD不存在
    if(!MMISD_IsSDExistEx())
    {
#if defined MMIDC_MINI_RESOURCE_SIZE        
        MMIDC_OpenAlertWin(MMIPUB_SOFTKEY_ONE, TXT_PLEASE_INSERT_SD, IMAGE_PUBWIN_WARNING, MMI_3SECONDS);
#else        
        MMIDC_ErrorTip(TXT_PLEASE_INSERT_SD);//TXT_NO_SD_CARD_ALERT
#endif        
        return FALSE;
    }
#endif

    file_dev = MMIDC_GetVideoStorageDevice();
    fs_error = MMIAPIFMM_GetSuitableFileDev(file_dev,
                                          MMI_DC_MIN_SPACE_SIZE, 
                                          &suitable_dev
                                          );
    if( SFS_ERROR_NONE != fs_error )
    {
        MMIDC_ShowErrorMsg(fs_error, file_dev);
        return FALSE;
    }
    MMIDC_SetVideoStorageDevice(suitable_dev);
    

    MMIDC_SetCurrentMode(DV_RECORD_MODE);
    s_dv_is_record = TRUE;
    StopDVPreview();

    MMIDC_ClearOSDBlock();
    mode = MMIDC_GetCurrentMode();
    MMIDC_ShowAllOSD();
#ifdef MMIDC_DESKTOP_PDA
    MMIDC_DisplayVideoRecordTip(DV_RECORD_MODE);
#endif
#if defined(MMIDC_DESKTOP_ICON_BAR) && defined(MMIDC_F_DV_ZOOM)
    MMIDC_DisplayZoomButton();
#endif
    MMIDC_CompleteDisplayOSD();

    ret = StartRecord();
    if(!ret)
    {
       MMIDC_SetCurrentMode(DV_PREVIEW_MODE);
       s_dv_is_record = FALSE;
    }

    return ret;
}

/*****************************************************************************/
// 	Description : dc file rename again
//	Global resource dependence : none
//  Author: jinju.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RenameFileAgain(void)
{	
	Save();
}
/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN RecordEnd(void)
{
	wchar * file_name_ptr = PNULL ; 
	uint16 file_name_len = 0 ;
#ifdef BAIDU_DRIVE_SUPPORT
    BAIDU_DRIVE_VIP_STATUS vip_status = BAIDU_DRV_VIP_NONE;
#endif

    MMIDC_SetCurrentMode(DV_REVIEW_MODE);
    StopRecord();
    StopDVPreview();
    CloseDV();
 
    MMIDC_SetVideoNameID(MMIDC_GetTempFileNameID());

#ifdef BAIDU_DRIVE_SUPPORT
	vip_status = MMISRV_BAIDUDRV_CheckVIPAccount();
	if(BAIDU_DRV_VIP_DONE == vip_status)
	{
	file_name_ptr = MMIDC_GetVideoFullFileName();
	file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);

	MMISRV_BAIDUDRV_append_upload_file(file_name_ptr , file_name_len);
	}
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BackDCPreview(void)
{
    BOOLEAN res = FALSE;

    MMIDC_SetCurrentMode(DC_PREVIEW_MODE);

    res = MMIDC_FlowStart();
//    res = StartDCPreview();

    return res;
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CancelRecord(void)
{
    BOOLEAN res = FALSE;

    MMIDC_SetCurrentMode(DV_PREVIEW_MODE);
    StopRecord();
    StopDVPreview();
    CloseDV();
    MMIDC_DeleteNoSavedFile();

    res = MMIDC_FlowStart();
#if 0    
    res = OpenDV();
    if(res)
    {
        res = StartDVPreview();
    }
#endif

    return res;
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BackDVPreview(void)
{
    BOOLEAN res = FALSE;

    MMIDC_SetCurrentMode(DV_PREVIEW_MODE);

    res = MMIDC_FlowStart();
#if 0
    res = OpenDV();
    if(res)
    {
        res = StartDVPreview();
    }
#endif
    return res;
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN Pause(void)
{
    DC_FLOW_MODE_E mode = 0;

    MMIDC_SetCurrentMode(DV_PAUSE_MODE);

    mode = MMIDC_GetCurrentMode();
    MMIDC_DisplayVideoRecordTip(mode);
#if defined(MMIDC_DESKTOP_ICON_BAR) && defined(MMIDC_F_DV_ZOOM)
    MMIDC_DisplayZoomButton();
#endif
    MMIDC_DisplaySoftKey(mode);
    MMIDC_CompleteDisplayOSD();

    if (MMIDC_IsDVOpen())
    {
        DRECORDER_Pause();
    }
    
    return TRUE;
}

#if !defined(DC_WATCH_UI_SUPPORT)
/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN Resume(void)
{
    DC_FLOW_MODE_E mode = 0;
    MMIDC_SetCurrentMode(DV_RECORD_MODE);
#if defined(MMIDC_DESKTOP_ICON_BAR) && defined(MMIDC_F_DV_ZOOM)
    MMIDC_DisplayZoomButton();
#endif
    mode = MMIDC_GetCurrentMode();
    MMIDC_DisplaySoftKey(mode);
    MMIDC_CompleteDisplayOSD();
    if (MMIDC_IsDVOpen())
    {
        DRECORDER_Resume();
    }
    return TRUE;
}

#endif
/*****************************************************************************/
//Description : dc flow operation
//Global resource dependence : none
//  Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN InterruptDC(void)
{
    s_is_cancel_multishot = TRUE;
    MMIDC_SetCurrentMode(DC_PREVIEW_MODE);
    StopDCPreview();
    CloseDC();
    if(PNULL != s_callback_audio_semaphore)
    {
        SCI_DeleteSemaphore(s_callback_audio_semaphore);
        s_callback_audio_semaphore = PNULL;
    }
    return TRUE;
}

/*****************************************************************************/
//Description : dc flow operation
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN InterruptDVPreview(void)
{
    StopDVPreview();
    CloseDV();
    return TRUE;
}

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN InterruptRecord(void)
{
    MMIDC_SetCurrentMode(DV_PREVIEW_MODE);
    StopRecord();
    StopDVPreview();
    CloseDV();

    MMIDC_SetVideoNameID(MMIDC_GetTempFileNameID());
    return TRUE;
}

/*****************************************************************************/
// Description : dc flow operation
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ContinueDC(void)
{
    BOOLEAN res = FALSE;

    s_is_cancel_multishot = FALSE;
    MMIDC_SetCurrentMode(DC_PREVIEW_MODE);
    res = OpenDC();
    if(res)
    {
        res = StartDCPreview();
    }
    return res;
}

/*****************************************************************************/
//Description : dc flow operation
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN ContinueDV(void)
{
    BOOLEAN res = FALSE;

    res = OpenDV();
    if(res)
    {
        res = StartDVPreview();
    }
    return res;
}

/*****************************************************************************/
//Description : exit the flow of camera
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
LOCAL BOOLEAN Exit(void)
{
	//SCI_TRACE_LOW:"[MMIDC]: MMIDC_FlowExit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3179_112_2_18_2_8_34_152,(uint8*)"");

    s_is_cancel_multishot = TRUE;
    StopDCPreview();
    CloseDC();	

    if(s_dv_is_record && (DV_RECORD_MODE == s_dc_current_flow_mode || DV_PAUSE_MODE == s_dc_current_flow_mode))
    {
        StopRecord();
    }
    StopDVPreview();
    CloseDV();
    MMIDC_SetCurrentMode( DC_PREVIEW_MODE);
    s_is_review  = FALSE;
    s_is_capture = FALSE;
    MMIDC_CloseCallbackTimer();
    if(PNULL != s_callback_audio_semaphore)
    {
        SCI_DeleteSemaphore(s_callback_audio_semaphore);
        s_callback_audio_semaphore = PNULL;
    }
    return TRUE;
}

#ifdef DC_WATCH_UI_SUPPORT

//########Watch functions##############///////////////////////////////////////////////////////////////////////////////////////////////////////
#include "mmivp_export.h"
#include "mmivp_internal.h"

LOCAL VP_STATE_E s_dv_player_state = VP_STATE_IDLE;
LOCAL BOOLEAN s_first_frame_after_play = TRUE; //Set to false when first VP_PLAY_PROCESS_IND after play action. then Clear all OSD.


/*****************************************************************************/
//  Description: check whether vp module is inited.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, TRUE if vp is inited.
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WDC_isInVpState(void)
{
    BOOLEAN bState = (WatchVP_GetHandle_ForDv() != 0);
    TRACE_APP_CAMERA(" WDC_isInVpState bEnable: %s",  bState? "TRUE":"FALSE");
    return bState;
}

/*****************************************************************************/
// Description: return vp play state.
// Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, TRUE if playing.
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN WDC_isVpPlaying(void)
{
    VP_HANDLE vpControl = WatchVP_GetHandle_ForDv();

    if ((vpControl != NULL)&& (s_dv_player_state == VP_STATE_PLAYING))
    {
        TRACE_APP_CAMERA(" WDC_isVpPlaying   TRUE");
        return TRUE;
    }

    TRACE_APP_CAMERA(" WDC_isVpPlaying   FALSE");
    return FALSE;
}

/*****************************************************************************/
//  Description: callback register to vp module, for vp message handling.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
LOCAL void VpFsmMsgCb(VP_CALLBACK_T *msg_ptr)
{
    TRACE_APP_CAMERA(" VpFsmMsgCb  enter");
    TRACE_APP_CAMERA(" VpFsmMsgCb SCI_IdentifyThread = %d.", SCI_IdentifyThread());
    TRACE_APP_CAMERA(" VpFsmMsgCb  msg: 0x%x", msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
    case VP_PREPARE_IND:
        {
            VP_PREPARE_IND_T *info = (VP_PREPARE_IND_T *)msg_ptr->para;
            TRACE_APP_CAMERA(" VpFsmMsgCb VP_PREPARE_IND");
            TRACE_APP_CAMERA(" VpFsmMsgCb start_time = %d, total_time = %d.",  \
                info->start_time, info->total_time);

            s_dv_player_state = VP_STATE_PLAY_PENDING;
            break;
        }

    case VP_PLAY_PROCESS_IND:
    {
        //VP_PLAY_PROCESS_IND_T *info = (VP_PLAY_PROCESS_IND_T *)msg_ptr->para;
        TRACE_APP_CAMERA("VpFsmMsgCb VP_PLAY_PROCESS_IND, first frame = %s ",
                        (s_first_frame_after_play==TRUE)?"TRUE":"FALSE");
        if(s_first_frame_after_play)//first time start playing, icons hide immediatly, when playing, touch/key press will set 3 second timer.
        {
            s_first_frame_after_play = FALSE;
            WDC_SetOsdVisiablity(MMIDC_GetCurrentMode(),FALSE);
            if(MMIDC_GetMMIFocusStatus() == MMIDC_GET_FOCUS)
            {
                MMIDC_ClearOSDBlock();
                MMIDC_CompleteDisplayOSD();
            }
        }
        break;
    }

    case VP_BUFFERING_IND:
     {
         TRACE_APP_CAMERA(" VpFsmMsgCb  VP_BUFFERING_IND");
         s_dv_player_state = VP_STATE_BUFFERING;
         break;
     }

    case VP_BUFFER_END_IND:
    {
       TRACE_APP_CAMERA(" VpFsmMsgCb  VP_BUFFER_END_IND");
       break;
    }

    case VP_BUFFER_PROCESS_IND:
    {
        TRACE_APP_CAMERA("VpFsmMsgCb VP_BUFFER_PROCESS_IND");
        break;
    }

    case VP_FATAL_ERROR_IND:
    {
        VP_FATAL_ERROR_IND_T *info  = (VP_FATAL_ERROR_IND_T *)msg_ptr->para;
        TRACE_APP_CAMERA("VpFsmMsgCb VP_FATAL_ERROR_IND");
        TRACE_APP_CAMERA("VpFsmMsgCb error_code = %d", info->error_code);
        break;
    }

    case VP_AUTH_IND:
    {
        TRACE_APP_CAMERA("VpFsmMsgCb VP_AUTH_IND");
        break;
    }

    case VP_PLAY_END_IND:
    {
        TRACE_APP_CAMERA("VpFsmMsgCb VP_PLAY_END_IND");
        break;
    }

    case VP_STOP_IND:
   {
       TRACE_APP_CAMERA("VpFsmMsgCb  VP_STOP_IND");
        WatchVP_Stop_ForDv();
        s_dv_player_state = VP_STATE_IDLE;
        MMIDC_CloseTipDisplayTimer();
        WDC_SetPausedOrStopedStateIcon();
        WDC_SetOsdVisiablity(MMIDC_GetCurrentMode(),TRUE);
        if ((MMIDC_GetMMIFocusStatus() == MMIDC_GET_FOCUS)&& (MMIDC_GetCurrentMode() == DV_REVIEW_MODE))
        {
            MMIDC_DisplayLastFrame();
            WDC_ShowAllOSD();
            MMIDC_CompleteDisplayOSD();
        }
        break;
    }

    case VP_PLAY_CNF:
    {
        TRACE_APP_CAMERA("VpFsmMsgCb VP_PLAY_CNF ");
        s_dv_player_state = VP_STATE_PLAYING;
        s_first_frame_after_play = TRUE;
        WDC_SetPlayingStateIcon();
        break;
    }

    case VP_PAUSE_CNF:
    {
        TRACE_APP_CAMERA("VpFsmMsgCb VP_PAUSE_CNF");
        s_dv_player_state = VP_STATE_PAUSED;
        MMIDC_CloseTipDisplayTimer();
        WDC_SetPausedOrStopedStateIcon();
        if (MMIDC_GetMMIFocusStatus() == MMIDC_GET_FOCUS)
        {
            WDC_ClearAllRect();
            WDC_ShowAllOSD();
            MMIDC_CompleteDisplayOSD();
        }
        break;
    }

    case VP_SEEK_CNF:
    {
        TRACE_APP_CAMERA("VpFsmMsgCb VP_SEEK_CNF");
        break;
    }

    default:
        TRACE_APP_CAMERA("VpFsmMsgCb unknown?");
        return;
    }
//    MMIDC_Watch_ShowPlayControl();
    TRACE_APP_CAMERA("VpFsmMsgCb exit");
}

/*****************************************************************************/
//  Description: play or pause video play.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_DVReviewPlayOrPause(void)
{
    VP_HANDLE vpControl = WatchVP_GetHandle_ForDv();
    TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause Enter ");

    if (vpControl)
    {
        if (MMIAPIVP_FSMGetState(vpControl) == VP_STATE_PLAYING)
        {
            //do pause
            TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause  pause");
            WatchVP_Pause_ForDv();
        }
        else if (MMIAPIVP_FSMGetState(vpControl) == VP_STATE_PAUSED)
        {
            //do resume
            TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause  resume");
            WatchVP_Resume_ForDv();
        }
        else if (MMIAPIVP_FSMGetState(vpControl) == VP_STATE_READY)
        {
            const wchar *pSavedName =MMIDC_GetVideoFullFileName();
            uint32 nNameLen = MMIAPICOM_Wstrlen(pSavedName);
            MMIDC_SetBlock();
            TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause  reopen ");
            MMIDC_SetLcdForRotate(FALSE);
            WatchVP_Play_ForDv(pSavedName, nNameLen);
        }
    }
    else
    {
        const wchar *pSavedName =MMIDC_GetVideoFullFileName();
        uint32 nNameLen = MMIAPICOM_Wstrlen(pSavedName);
        TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause vpControl is null, init and play");
        MMIDC_SetBlock();
        WatchVP_Init_ForDv(VpFsmMsgCb);
        MMIDC_SetLcdForRotate(FALSE);
        WatchVP_Play_ForDv(pSavedName, nNameLen);
        MMIAPIVP_FSMUpdateCurrentVol((VP_HANDLE)WatchVP_GetHandle_ForDv(),7);
    }

    TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause end");
}

/*****************************************************************************/
//  Description: stop dv review playback.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_DVReviewStopPlaying(void)
{
    VP_HANDLE vpControl = WatchVP_GetHandle_ForDv();
    TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause Enter ");

    if (vpControl)
    {
        if ((MMIAPIVP_FSMGetState(vpControl) == VP_STATE_PLAYING) || (MMIAPIVP_FSMGetState(vpControl) == VP_STATE_PAUSED))
        {
            //do pause
            TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause  pause");
            WatchVP_Stop_ForDv();
        }
    }
    else
    {
        TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause vpControl is null, not in vp state.");
    }
    WDC_SetPausedOrStopedStateIcon();
    TRACE_APP_CAMERA("WDC_DVReviewPlayOrPause end");
}


/*****************************************************************************/
//  Description: do review photo action
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_ReviewPhotos(void)
{
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();

    TRACE_APP_CAMERA("[MMIDC]: WDC_ReviewPhotos %d, flow_mode = %d", mode, s_dc_current_flow_mode);



    s_is_review = TRUE;
    MMIDC_StartCallbackTimer();
    if(!ReviewSinglePhotoFromBuf())
    {
        MMIDC_ErrorTip(TXT_DC_CAPTURE_FAILED);
        s_is_review = FALSE;
        MMIDC_SetCurrentMode(DC_PREVIEW_MODE);
    }
    else
    {
        WDC_ShowAllOSD();
        MMIDC_CompleteDisplayOSD();

    }
}


/*****************************************************************************/
//  Description: go to dc review mode.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_ReviewDC(void)
{
    TRACE_APP_CAMERA("[MMIDC] Watch_ReviewDC");
    if(!s_is_capture && s_is_snapshot_succss)
    {
        MMIDC_SetCurrentMode(DC_REVIEW_MODE);
        WDC_ResetIconFocus();
        MMIDC_ResetCurrentPhotoID();
        MMIDC_StartCallbackTimer();
        MMIDC_ClearOSDBlock();
        WDC_ReviewPhotos();
    }
    return TRUE;
}

/*****************************************************************************/
//  Description: interrupt dc flow
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_InterruptDC(void)
{
    TRACE_APP_CAMERA("[MMIDC] Watch_InterruptDC");
    s_is_cancel_multishot = TRUE;
    StopDCPreview();
    if(MMIDC_GetCurrentMode()==DC_REVIEW_MODE)//to review photo after get focus, do not close dc.
    {
        MMIDC_RestoreBlock();
    }
    else
    {
        CloseDC();
    }    
    if(PNULL != s_callback_audio_semaphore)
    {
        SCI_DeleteSemaphore(s_callback_audio_semaphore);
        s_callback_audio_semaphore = PNULL;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description: continue dc review from interrupt .
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_ContinueDCReview(void)
{
    BOOLEAN res = FALSE;
    TRACE_APP_CAMERA("[MMIDC] Watch_ContinueDCReview");
    s_is_cancel_multishot = FALSE;
    MMIDC_SetBlock();
    if(!s_is_capture && s_is_snapshot_succss)
    {
        MMIDC_ResetCurrentPhotoID();
        MMIDC_StartCallbackTimer();
        MMIDC_ClearOSDBlock();
        WDC_ReviewPhotos();
    }
    return TRUE;
}

/*****************************************************************************/
//  Description: back to dc preview mode
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_BackDCPreview(void)
{
    BOOLEAN res = FALSE;
    TRACE_APP_CAMERA("[MMIDC] Watch_BackDCPreview");
    MMIDC_SetCurrentMode(DC_PREVIEW_MODE);
    WDC_ResetIconFocus();
    res = MMIDC_FlowStart();
    return res;
}

/*****************************************************************************/
//  Description: save image file.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_Save(void)
{
    MMIFILE_ERROR_E  ret = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E  file_dev = 0;
    BOOLEAN is_save = TRUE;
    TRACE_APP_CAMERA("[MMIDC] Watch_Save");
    //s_dc_current_flow_mode = DC_SAVE_MODE;

     ret = MMIDC_SavePhotos(0);
     if (SFS_ERROR_NONE != ret)
     {
         if(!MMIDC_GetRenameFileCircleFlag())
         {
             file_dev = MMIDC_GetPhotoStorageDevice();
             MMIDC_ShowErrorMsg(ret, file_dev);
             is_save = FALSE;
         }
         else
         {
             is_save = FALSE;
             return is_save;
         }
     }

    MMIDC_PostCreateFileHandleMsg();
    if (!is_save)
    {
        return is_save;
    }

    BackDCPreview();

    return TRUE;
}

/*****************************************************************************/
//  Description: exit dv review.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL void Watch_ExitDvReview(void)
{
    TRACE_APP_CAMERA("Watch_ExitDvReview ENTER");
    WatchVP_Stop_ForDv();
    MMIDC_SetLcdForRotate(FALSE);
    MMIDC_RestoreBlock();
    TRACE_APP_CAMERA("Watch_ExitDvReview EXIT");
}

/*****************************************************************************/
//  Description: exit dc driver module.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_ExitDC(void)
{
    DC_FLOW_MODE_E mode = MMIDC_GetCurrentMode();
    TRACE_APP_CAMERA("[MMIDC]: Watch_ExitDC at mode:%d ", mode);
   //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3179_112_2_18_2_8_34_152,(uint8*)"");
    if(mode==DV_REVIEW_MODE)
    {
        Watch_ExitDvReview();
    }
    s_is_cancel_multishot = TRUE;
    StopDCPreview();
    CloseDC();

    if(s_dv_is_record && (DV_RECORD_MODE == mode || DV_PAUSE_MODE == mode))
    {
        StopRecord();
    }
    StopDVPreview();
    CloseDV();
    MMIDC_SetCurrentMode(DC_PREVIEW_MODE);
    WDC_ResetIconFocus();
    s_is_review  = FALSE;
    s_is_capture = FALSE;
    MMIDC_CloseCallbackTimer();
    if(PNULL != s_callback_audio_semaphore)
    {
        SCI_DeleteSemaphore(s_callback_audio_semaphore);
        s_callback_audio_semaphore = PNULL;
    }
    return TRUE;
}


/*****************************************************************************/
//  Description: start record.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_Record(void)
{
    BOOLEAN ret = FALSE;
    MMIFILE_ERROR_E   fs_error = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E      file_dev = MMI_DEVICE_SDCARD;
    MMIFILE_DEVICE_E      suitable_dev = MMI_DEVICE_SDCARD;
    TRACE_APP_CAMERA("[MMIDC] Watch_Record");
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
    //SD不存在
    if(!MMISD_IsSDExistEx())
    {
#if defined MMIDC_MINI_RESOURCE_SIZE 
        MMIDC_OpenAlertWin(MMIPUB_SOFTKEY_ONE, TXT_PLEASE_INSERT_SD, IMAGE_PUBWIN_WARNING, MMI_3SECONDS);
#else
        MMIDC_ErrorTip(TXT_PLEASE_INSERT_SD);//TXT_NO_SD_CARD_ALERT
#endif
        return FALSE;
    }
#endif

    file_dev = MMIDC_GetVideoStorageDevice();
    fs_error = MMIAPIFMM_GetSuitableFileDev(file_dev,
                                          MMI_DC_MIN_SPACE_SIZE,
                                          &suitable_dev
                                          );
    if( SFS_ERROR_NONE != fs_error )
    {
        MMIDC_ShowErrorMsg(fs_error, file_dev);
        return FALSE;
    }
    MMIDC_SetVideoStorageDevice(suitable_dev);

    MMIDC_SetCurrentMode(DV_RECORD_MODE);
    s_dv_is_record = TRUE;
    StopDVPreview();

    MMIDC_ClearOSDBlock();

    WDC_ShowAllOSD();

    MMIDC_CompleteDisplayOSD();

    ret = StartRecord();
    if(!ret)
    {
       MMIDC_SetCurrentMode(DV_PREVIEW_MODE);
       s_dv_is_record = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description: record end.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_RecordEnd(void)
{
    wchar * file_name_ptr = PNULL ; 
    uint16 file_name_len = 0 ;
#ifdef BAIDU_DRIVE_SUPPORT
    BAIDU_DRIVE_VIP_STATUS vip_status = BAIDU_DRV_VIP_NONE;
#endif   

    TRACE_APP_CAMERA("[MMIDC] Watch_RecordEnd");
    MMIDC_SetCurrentMode(DV_SAVE_MODE);  
    MMIDC_ClearOSDBlock();
    MMIDC_CompleteDisplayOSD();
    StopRecord();
    StopDVPreview();
    CloseDV();
    MMIDC_SetVideoNameID(MMIDC_GetTempFileNameID());

#ifdef BAIDU_DRIVE_SUPPORT
    vip_status = MMISRV_BAIDUDRV_CheckVIPAccount();
    if(BAIDU_DRV_VIP_DONE == vip_status)
    {
    file_name_ptr = MMIDC_GetVideoFullFileName();
    file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);

    MMISRV_BAIDUDRV_append_upload_file(file_name_ptr , file_name_len);
    }
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description: operation when dv save complete.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_DvSaveComplete(void)
{
    TRACE_APP_CAMERA("[MMIDC] WDC_DvSaveComplete");
    MMIDC_SetCurrentMode(DV_REVIEW_MODE);
    WDC_ResetIconFocus();
    return TRUE;
}

/*****************************************************************************/
//  Description: interrupt at dv save mode.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_InterruptDvSave(void)
{
    TRACE_APP_CAMERA("[MMIDC] Watch_InterruptDvSave");
    MMIDC_SetCurrentMode(DV_REVIEW_MODE);
    return TRUE;
}

/*****************************************************************************/
//  Description: cancel record.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_CancelRecord(void)
{
    BOOLEAN res = FALSE;

    TRACE_APP_CAMERA("[MMIDC] Watch_CancelRecord");

    StopRecord();
    StopDVPreview();
    CloseDV();
    MMIDC_DeleteNoSavedFile();

    res = MMIDC_FlowStart();

    WDC_ResetIconFocus();
    return res;
}

/*****************************************************************************/
//  Description: interrupt at dv review mode.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_InterruptDvReview(void)
{
    VP_HANDLE vpControl = WatchVP_GetHandle_ForDv();
    TRACE_APP_CAMERA("WDC_InterruptDvReview ENTER");

    if (vpControl != PNULL)
    {
        WatchVP_Pause_ForDv();
    }
    MMIDC_RestoreBlock();
    TRACE_APP_CAMERA("WDC_InterruptDvReview  EXIT");
    return TRUE;
}


/*****************************************************************************/
//  Description: restore dv review from interrupt at dv review mode.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_RestoreDvReview(void)
{
    TRACE_APP_CAMERA("WDC_RestoreDvReview ENTER");
    MMIDC_SetBlock();
    WDC_SetOsdVisiablity(MMIDC_GetCurrentMode(),TRUE);
    if (WatchVP_GetHandle_ForDv() != 0)
    {
        TRACE_APP_CAMERA("WDC_RestoreDvReview  Restore dv review. vp exist.");
        MMIDC_ClearOSDBlock();
        WDC_ShowAllOSD();
        MMIDC_CompleteDisplayOSD();
    }
    else
    {
        TRACE_APP_CAMERA("WDC_RestoreDvReview  Restore dv review. vp is null.");
        MMIDC_ClearOSDBlock();
        MMIDC_DisplayLastFrame();
        WDC_ShowAllOSD();
        MMIDC_CompleteDisplayOSD();
    }

    TRACE_APP_CAMERA("WDC_RestoreDvReview EXIT");
    return TRUE;
}

/*****************************************************************************/
//  Description: restore from interrupt at dv save mode.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_RestoreDvSave(void)
{
    TRACE_APP_CAMERA("[MMIDC] Watch_RestoreDvSave");
    MMIDC_SetCurrentMode(DV_REVIEW_MODE);
    WDC_ResetIconFocus();
    Watch_RestoreDvReview();
    return TRUE;
}


/*****************************************************************************/
//  Description: back to dv preview without saving file.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_BackDVPreviewWithoutSave(void)
{
    BOOLEAN ret = FALSE;
    const wchar *pSavedName =MMIDC_GetVideoFullFileName();
    uint32 nNameLen = MMIAPICOM_Wstrlen(pSavedName);
    Watch_ExitDvReview();
    MMIDC_SetCurrentMode(DV_PREVIEW_MODE);
    WDC_ResetIconFocus();
    ret = MMIAPIFMM_DeleteFileSyn(pSavedName, nNameLen);
    TRACE_APP_CAMERA("[MMIDC]: Watch_BackDVPreviewWithoutSave MMIAPIFMM_DeleteFileSyn ret = %s.", ret==FALSE?"FALSE":"TRUE");

    ret = MMIDC_FlowStart();

    return ret;
}

/*****************************************************************************/
//  Description: back to dv preview.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, operation result, TRUE if OK. 
//  Author: fulu.song
//  Note: Watch UI flow mode function.
/*****************************************************************************/
LOCAL BOOLEAN Watch_BackDVPreview(void)
{
    BOOLEAN res = FALSE;
    TRACE_APP_CAMERA("[MMIDC] Watch_BackDVPreview");
    Watch_ExitDvReview();
    MMIDC_SetCurrentMode(DV_PREVIEW_MODE);
    WDC_ResetIconFocus();
    res = MMIDC_FlowStart();

    return res;
}

const DCFLOWFUNCTION s_flow_function[DC_OPERATION_MAX][DC_FLOW_MODE_MAX] = 
{
    /*DC_PREVIEW        CONT_DOWN          DC_CAPTURE           DC_REVIEW               DC_SAVE              DV_PREVIEW          DV_RECORD           DV_PAUSE            DV_SAVE               DV_REVIEW */
    {Capture,           PNULL,             Watch_ReviewDC,      Watch_Save,             PNULL,               Watch_Record,       Watch_RecordEnd,    Watch_RecordEnd,    Watch_DvSaveComplete, Watch_BackDVPreview},
    {PNULL,             PNULL,             Watch_BackDCPreview, Watch_BackDCPreview,    Watch_BackDCPreview, PNULL,              Watch_CancelRecord, Watch_CancelRecord, Watch_BackDVPreview,  Watch_BackDVPreviewWithoutSave},
    {PNULL,             PNULL,             PNULL,               PNULL,                  PNULL,               PNULL,              PNULL,              PNULL,              PNULL,                PNULL},
    {Watch_InterruptDC, Watch_InterruptDC, Watch_InterruptDC,   Watch_InterruptDC,      Watch_InterruptDC,   InterruptDVPreview, InterruptRecord,    InterruptRecord,    PNULL,                Watch_InterruptDvReview},
    {ContinueDC,        ContinueDC,        ContinueDC,          Watch_ContinueDCReview, PNULL,               ContinueDV,         PNULL,              PNULL,              Watch_RestoreDvSave,  Watch_RestoreDvReview},
    {Watch_ExitDC,      Watch_ExitDC,      Watch_ExitDC,        Watch_ExitDC,           Watch_ExitDC,        Watch_ExitDC,       Watch_ExitDC,       Watch_ExitDC ,      Watch_ExitDC,         Watch_ExitDC },
};


/*****************************************************************************/
//  Description: get error string from error type
//  Parameter: [In] fs_error: error type
//                  filedev: device 
//                  is_file: whether is file
//             [Out] none
//             [Return] error text id.
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T Watch_GetSfsErrorString (SFS_ERROR_E fs_error, MMIFILE_DEVICE_E filedev, BOOLEAN is_file)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    BOOLEAN result = FALSE;
    SCI_TRACE_LOW("[MMIDC] Watch_GetSfsErrorString: fs_error:%d, filedev:%d, is_file:%d", fs_error, filedev, is_file);
    if (MMIAPIUDISK_UdiskIsRun())
    {
        //u disk runing
        text_id = TXT_COMMON_UDISK_USING;
    }
    else if (filedev >= MMI_DEVICE_NUM)
    {
        //no sd card
        text_id = TXT_PLEASE_INSERT_SD;
    }
    else if ( (MMI_DEVICE_SDCARD == filedev) && MMIAPISD_IsCardLogOn())
    {
        //card log runging
        text_id = TXT_COMMON_CARDLOG_USING;
    }
    else
    {
        BOOLEAN need_format = FALSE;
        if (MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (filedev), MMIAPIFMM_GetDevicePathLen (filedev)))
        {
            result = TRUE;
        }
        else
        {
            MMIFILE_ERROR_E device_error = SFS_NO_ERROR;
            device_error = MMIAPIFMM_RegisterDevice ( (uint16 *) MMIAPIFMM_GetDevicePath (filedev), MMIAPIFMM_GetDevicePathLen (filedev));

            if (SFS_NO_ERROR == device_error)
            {
                result = TRUE;
            }
            else if (SFS_ERROR_INVALID_FORMAT == device_error)
            {
                text_id = TXT_COMMON_FORMAT_ERROR;
            }
            else
            {
                text_id = TXT_ERROR;
            }
        }

    }

    if (result)
    {
        switch (fs_error)
        {
            case SFS_ERROR_ABORT:
            {
                //user stop,
                text_id = TXT_NULL;
            }
            case SFS_ERROR_NONE:
            {
                //success
                text_id = TXT_COMPLETE;
                break;
            }
            case SFS_ERROR_INVALID_PARAM:
            {
                //invalid name
                text_id = TXT_DC_NAMEINVALID;
                break;
            }
            case SFS_ERROR_HAS_EXIST:
            {
                //already exist
                if (is_file)
                {
                    text_id = TXT_FILE_EXISTED;
                }
                else
                {
                    text_id = TXT_FOLDER_EXISTED;
                }
                break;
            }
            case SFS_ERROR_NOT_EXIST:
            {
                //not exist
                if (is_file)
                {
                    text_id = TXT_COM_FILE_NO_EXIST;
                }
                else
                {
                    text_id = TXT_FOLDER_NOT_EXIST;
                }
                break;
            }
            case SFS_ERROR_NO_SPACE:
            {
                //no space
                if (MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (filedev), MMIAPIFMM_GetDevicePathLen (filedev)))
                {
#if defined(SCREEN_SHAPE_CIRCULAR)
                    text_id = TXT_CAMERA_NO_SPACE;
#else
                    text_id = TXT_NO_SPACE;
#endif
                }
                else
                {
                    text_id = TXT_NO_SD_CARD_ALERT;
                }
        
                break;
            }
            case SFS_ERROR_NOT_EMPTY:
            {
                //folder not empty
                text_id = TXT_COM_FOLDER_NOT_EMPTY;
                break;
            }
            case SFS_ERROR_FOLD_FULL:
            {
                //fold entry is not enough to  add new file or fold in it
#if defined(SCREEN_SHAPE_CIRCULAR)
                text_id = TXT_CAMERA_NO_SPACE;
#else
                text_id = TXT_NO_SPACE;
#endif
                break;
            }
            case SFS_ERROR_ACCESS:
            {
                //file in using
                text_id = TXT_COM_FILE_USING;
                break;
            }
            case SFS_ERROR_LONG_DEPTH:
            {
                //file in using
                text_id = TXT_COM_FILE_NAME_TOO_LONG;
                break;
            }
            default:
            {
                //other error
                text_id = TXT_ERROR;
                break;
            }
        }

    }

    return text_id;

}


#else
const DCFLOWFUNCTION s_flow_function[DC_OPERATION_MAX][DC_FLOW_MODE_MAX] = 
{
    /*DC_PREVIEW   CONT_DOWN     DC_CAPTURE      DC_REVIEW       DC_SAVE         DV_PREVIEW           DV_RECORD         DV_PAUSE          DV_REVIEW */
    {Capture,      PNULL,        Review,         Save,           PNULL,          Record,              RecordEnd,        RecordEnd,        PNULL},
    {PNULL,        PNULL,        BackDCPreview,  BackDCPreview,  BackDCPreview,  PNULL,               CancelRecord,     CancelRecord,     BackDVPreview},
    {PNULL,        PNULL,        PNULL,          PNULL,          PNULL,          PNULL,               Pause,            Resume,           PNULL},
    {InterruptDC,  InterruptDC,  InterruptDC,    InterruptDC,    InterruptDC,    InterruptDVPreview,  InterruptRecord,  InterruptRecord,  PNULL},
    {ContinueDC,   ContinueDC,   PNULL,          PNULL,          PNULL,          ContinueDV,          PNULL,            PNULL,            PNULL},
    {Exit,         Exit,         Exit,           Exit,           Exit,           Exit,                Exit,             Exit,             Exit},
};
#endif


/*****************************************************************************/
//Description : return is review in full screen or not
//Global resource dependence : 
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_FlowFunction(DC_OPERATION_E op)
{
    BOOLEAN res = FALSE;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_FlowFunction op=%d,flow_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3223_112_2_18_2_8_34_153,(uint8*)"dd",op,s_dc_current_flow_mode);

    if(PNULL != s_flow_function[op][s_dc_current_flow_mode])
    {
        res = s_flow_function[op][s_dc_current_flow_mode]();
    }
    return res;
}

/*****************************************************************************/
//Description : return is review in full screen or not
//Global resource dependence : 
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsReviewFullScreen(void)
{
    return s_is_review_full_screen;
}

/*****************************************************************************/
//Description : start to run the flow of camera
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_FlowStart(void)
{
    BOOLEAN res = FALSE;
    BOOLEAN is_first_preview = FALSE;
#ifdef BAIDU_DRIVE_SUPPORT
    BAIDU_DRIVE_VIP_STATUS vip_status = BAIDU_DRV_VIP_NONE ; 
#endif

    is_first_preview = MMIDC_GetFirstPreviewFlag();

    s_is_cancel_multishot = FALSE;

    MMIDC_FreeMemoryForReview();

    if(MMIDC_GetCameraMode() == CAMERA_MODE_DC)
    {
        if(OpenDC())
        {
            MMIDC_SetCurrentMode(DC_PREVIEW_MODE);

            MMIDC_SetPhotoFrameForShow(TRUE);

            res = StartDCPreview(); //bug 1430102

            if (!is_first_preview)
            {
                MMIDC_ClearOSDBlock();
                MMIDC_SetOSDFlagBeforePreview(TRUE);
                MMIDC_ShowAllOSD();
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDC]: MMIDC_FlowStart fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3276_112_2_18_2_8_34_154,(uint8*)"");
            return FALSE;
        }
    }
    else if(MMIDC_GetCameraMode() == CAMERA_MODE_DV)
    {
#ifdef BAIDU_DRIVE_SUPPORT
	vip_status = MMISRV_BAIDUDRV_CheckVIPAccount();
	if(BAIDU_DRV_VIP_NONE == vip_status)
	{
		MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
		BOOLEAN    b_is_network_ok = FALSE;
		uint8  i = 0;
		
#ifdef MMI_WIFI_SUPPORT
		if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
		{
			b_is_network_ok = TRUE;
		}
		else
#endif			
		{
			for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
			{
				if (MMIAPIPHONE_IsSimAvailable(i)  && MMIAPIPHONE_IsAttachGprsSuccess(i))
				{
					b_is_network_ok = TRUE;
					break;
				}
			}
		}
		
		if(b_is_network_ok)
		{
		//wait for user's confirmation
                MMISRV_BAIDUDRV_OpenVIPQueryWin();
		return TRUE;
	}
	
	}
#endif
        if(OpenDV())
        {
            MMIDC_SetCurrentMode(DV_PREVIEW_MODE);

            res = StartDVPreview();//bug 1430102

            if (!is_first_preview)
            {
                MMIDC_ClearOSDBlock();
                MMIDC_SetOSDFlagBeforePreview(TRUE);
                MMIDC_ShowAllOSD();
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDC]: MMIDC_FlowStart fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3297_112_2_18_2_8_35_155,(uint8*)"");
            return FALSE;
        }
    }

    if (res 
        && is_first_preview
        )
    {
        MMIDC_SetFirstPreviewFlag(FALSE);
#ifdef MMIDC_DESKTOP_PDA
        MMIDC_StartPreviewAnim(PREVIEW_ANIM_STEP_ANIM);
#endif
    }

#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_SMILE_AUTO_PHOTO) 
    if(res 
        && MMIDC_GetUCameraModuleFlag()
        && IMG_PROC_FACE_DECT == MMIDC_GetImageProcessType())
    {
        MMIDC_CreateSmileAutoPhotoHandle();
    }
#endif

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_FlowStart res=%d,is_first_preview=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3313_112_2_18_2_8_35_156,(uint8*)"dd",res, is_first_preview);

    return res;
}

/*****************************************************************************/
//Description : error handle for camera
//Global resource dependence : none
//Author: ryan.xu
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ErrorTip(MMI_TEXT_ID_T txt_id)
{
#ifdef DC_WATCH_UI_SUPPORT
    MMI_STRING_T commentString ;
    MMI_STRING_T buttonString ;
    WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,STXT_RETURN,TXT_NULL};
    if (TXT_NULL != txt_id)
    {
        MMI_GetLabelTextByLang(txt_id, &commentString);
    }
    MMI_GetLabelTextByLang(TXT_COMMON_OK, &buttonString);
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)//2072061
#ifdef BAIDU_DRIVE_SUPPORT
    if(MMK_IsOpenWin(MMIDC_PUB_FAIL_WIN_ID))
    {
        WatchCOM_QueryWin_1Btn_Set_Tip_String(MMIDC_PUB_FAIL_WIN_ID, &commentString);
    }
    else
    {
        WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, PNULL, IMG_DC_WATCH_CONFIRM, softket_test, HandleDcDiskFull );
    }
#else
    if(MMK_IsOpenWin(MMIDC_PUB_FAIL_WIN_ID))
    {
        WatchCOM_QueryWin_1Btn_Set_Tip_String(MMIDC_PUB_FAIL_WIN_ID, &commentString);
    }
    else
    {
        WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, PNULL, IMG_DC_WATCH_CONFIRM, softket_test, PNULL );
    }
#endif	
#else
    WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, &buttonString,PNULL, softket_test, PNULL );
#endif
#else
    MMIDC_OpenAlertWin(MMIPUB_SOFTKEY_ONE, txt_id, IMAGE_PUBWIN_FAIL, MMI_3SECONDS);
#endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ErrorTipForExit(MMI_TEXT_ID_T txt_id)
{
#ifdef DC_WATCH_UI_SUPPORT
    MMI_STRING_T commentString ;
    MMI_STRING_T buttonString ;
    WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,STXT_RETURN,TXT_NULL};
    if (TXT_NULL != txt_id)
    {
        MMI_GetLabelTextByLang(txt_id, &commentString);
    }
    MMI_GetLabelTextByLang(TXT_COMMON_OK, &buttonString);
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)//2072061
    if(MMK_IsOpenWin(MMIDC_PUB_FAIL_WIN_ID))
    {
        WatchCOM_QueryWin_1Btn_Set_Tip_String(MMIDC_PUB_FAIL_WIN_ID, &commentString);
    }
    else
    {
        WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, PNULL, IMG_DC_WATCH_CONFIRM,  softket_test, PNULL );
    }
#else
    WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, &buttonString,PNULL,  softket_test, PNULL );
#endif
#else
    uint32 time_period = MMI_3SECONDS;
    MMI_WIN_ID_T alert_id = MMIDC_PUB_FAIL_WIN_ID;
    MMIPUB_OpenAlertWinByTextId(&time_period,txt_id,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
}

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_IsPreviewing(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIAPIDC_IsPreviewing dc preview = %d, dv preview = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3345_112_2_18_2_8_35_157,(uint8*)"dd", s_dc_is_preview, s_dv_is_preview);
    return (s_dc_is_preview || s_dv_is_preview);
}

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsReviewing(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_IsReviewing %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3355_112_2_18_2_8_35_158,(uint8*)"d", s_is_review);
    return s_is_review;
}

/*****************************************************************************/
// 	Description : is camera preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsCapturing(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_IsCapturing %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3365_112_2_18_2_8_35_159,(uint8*)"d", s_is_capture);
    return s_is_capture;
}

/*****************************************************************************/
// 	Description : is video recording
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_IsRecording(void)
{
	return s_dv_is_record;
}

/*****************************************************************************/
//  Description : get all device info and check if have enough space 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param Input:  size        space is or no smaller  the size
//       Output:  file_dev    file device 
//       Return:  SFS_ERROR_NONE  or other value, see MMIFILE_ERROR_E
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E  MMIDC_CheckSpace(uint32 size,   //[in] min space size 
                                        MMIFILE_DEVICE_E *file_dev  //[out] file device
                                        )
{
/*
    uint32 free_high_word = 0;
    uint32 free_low_word = 0;
    SFS_ERROR_E sfs_result = SFS_ERROR_NOT_EXIST;
    SFS_ERROR_E  sfs_sd_ret = SFS_ERROR_NOT_EXIST;
    SFS_ERROR_E  sfs_udisk_ret = SFS_ERROR_NOT_EXIST;
    int32 space_result = FALSE;
    DC_DEV_E    dc_dev = DC_DEV_UDISK;
    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD;
    SFS_ERROR_E sfs_ret = SFS_ERROR_NONE;
    
    //check SD
    if(SFS_ERROR_NONE == MMIAPISD_CheckSDSpace(size, &fmm_dev))
    {
        space_result = TRUE;
    }
   
    //check udisk 
    if(!space_result)
    {
        sfs_udisk_ret = SFS_GetDeviceFreeSpace(MMIFILE_DEVICE_UDISK, &free_high_word, &free_low_word);
        if(SFS_ERROR_NONE == sfs_udisk_ret)
        {
            //space is lower
            if(0 ==free_high_word && free_low_word < size)
            {
                sfs_udisk_ret = SFS_ERROR_NO_SPACE;
                //SCI_TRACE_LOW:"[MMIDC]MMIDC_CheckSpace: UDISK free spaced high value is = %d free space low value is = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3415_112_2_18_2_8_35_160,(uint8*)"dd", free_high_word,free_low_word);
            }else{
                space_result = TRUE;
            }
        }
        sfs_result = sfs_udisk_ret;
    }


   //SCI_TRACE_LOW:"[MMIDC]MMIDC_CheckSpace sfs_sd_ret = %d, sfs_udisk_ret = %d,space_result = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3425_112_2_18_2_8_35_161,(uint8*)"ddd",sfs_sd_ret, sfs_udisk_ret, space_result);

   //若是出错，根据当前设定的存储设备，提示信息
   if (SFS_ERROR_NONE != sfs_result)
   {
        //根据当前相机模式，判断存储设备
        if (CAMERA_MODE_DC == MMIDC_GetCameraMode())
        {
            dc_dev = MMIDC_GetPhotoStorageDevice();            
        }
        else
        {
            dc_dev = MMIDC_GetVideoStorageDevice();
        }
        
        switch(dc_dev)
        {
        case DC_DEV_SD:
            (*file_dev) = FS_MMC;
            sfs_result = sfs_sd_ret;
            break;        
        default:
            (*file_dev) = FS_UDISK;
            sfs_result = sfs_udisk_ret;
            break;
        }     
   }

    //SCI_TRACE_LOW:"[MMIDC]MMIDC_CheckSpace dc_dev = %d, sfs_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3453_112_2_18_2_8_35_162,(uint8*)"dd",dc_dev, (int32)sfs_result);
    return sfs_result;
*/    
    return SFS_ERROR_NOT_EXIST;
}


#ifdef BAIDU_DRIVE_SUPPORT
LOCAL MMI_RESULT_E HandleDcDiskFull(
                                    MMI_WIN_ID_T        win_id,    // Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E    msg_id,     // 窗口的内部消息ID
                                    DPARAM              param       // 相应消息的参数
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    SCI_TRACE_LOW("HandleDcDiskFull open,msg_id = %x",msg_id);

    switch (msg_id)
    {
        case MSG_NOTIFY_CANCEL://pin被锁输入界面不允许返回
        case MSG_NOTIFY_OK:
        case MSG_NOTIFY_MIDSK:
        {
            MMK_CloseWin(win_id);
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
		if(SFS_ERROR_NO_SPACE == s_dc_error_msg)
		{
			MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
			BOOLEAN    result = FALSE;
			uint32  i = 0;

			if(!MMIAPIBAIDU_IsAccountExist())
			{

#ifdef MMI_WIFI_SUPPORT
				if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
				{
					result = TRUE;
				}
				else
#endif			
				{
					for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
					{
						if (MMIAPIPHONE_IsSimAvailable(i)  && MMIAPIPHONE_IsAttachGprsSuccess(i))
						{
							result = TRUE;
							break;
						}
					}
				}
				
				if(result)
					MMISRV_BAIDUDRV_OpenBaiduLoginQueryWin();
			}				
		}
        }
        break;
        default:
            break;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : show error message as error type 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param Input:  fs_error    error  type as MMIFILE_ERROR_E
//        Input:  dev         file device
//       Return:  NONE
/*****************************************************************************/
PUBLIC void  MMIDC_ShowErrorMsg(MMIFILE_ERROR_E fs_error,   //[in] error value
                                MMIFILE_DEVICE_E   dev     //[in] file device 
                                )
{

#ifdef DC_WATCH_UI_SUPPORT
    MMI_STRING_T commentString ;
    MMI_STRING_T buttonString ;
    WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,STXT_RETURN,TXT_NULL};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShowErrorMsg fs_err = %d,dev = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3472_112_2_18_2_8_35_163,(uint8*)"dd", fs_error, dev);
    text_id = Watch_GetSfsErrorString(fs_error, dev, TRUE);
    MMI_GetLabelTextByLang(text_id, &commentString);
    MMI_GetLabelTextByLang(TXT_COMMON_OK, &buttonString);
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)//2072061
#ifdef BAIDU_DRIVE_SUPPORT
	s_dc_error_msg = fs_error ;

	if(MMIAPIBAIDU_IsAccountExist() && (SFS_ERROR_NO_SPACE == s_dc_error_msg))
	{
		MMI_GetLabelTextByLang(TXT_BAIDU_CLEAN_DISK , &commentString);		
	}
	
    if(MMK_IsOpenWin(MMIDC_PUB_FAIL_WIN_ID))
    {
        WatchCOM_QueryWin_1Btn_Set_Tip_String(MMIDC_PUB_FAIL_WIN_ID, &commentString);
    }
    else
    {
        WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, PNULL, IMG_DC_WATCH_CONFIRM, softket_test, HandleDcDiskFull );
    }
#else
    if(MMK_IsOpenWin(MMIDC_PUB_FAIL_WIN_ID))
    {
        WatchCOM_QueryWin_1Btn_Set_Tip_String(MMIDC_PUB_FAIL_WIN_ID, &commentString);
    }
    else
    {
        WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, PNULL, IMG_DC_WATCH_CONFIRM, softket_test, PNULL );
    }
#endif
#else
    WatchCOM_QueryWin_1Btn_Enter(MMIDC_PUB_FAIL_WIN_ID, &commentString, &buttonString,PNULL, softket_test, PNULL );
#endif

#else
    BOOLEAN  ret = TRUE;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShowErrorMsg fs_err = %d,dev = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3472_112_2_18_2_8_35_163,(uint8*)"dd", fs_error, dev);
	
    ret= MMIAPIFMM_CheckMemoryCard(dev);

    //除磁盘格式错误外的
    if(ret)
    {
        MMIAPIFMM_PromptFileErrorType(PNULL,fs_error, dev);
    }
#endif


}

/*****************************************************************************/
//  Description : show appointed error message by file device 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param Input:  fs_error    error type
//       Return:  NONE
/*****************************************************************************/
PUBLIC void  MMIDC_ShowAppointedErrorMsg(MMIFILE_ERROR_E fs_error   //[in] error type 
                                         )
{
    MMIFILE_DEVICE_E dc_dev = MMI_DEVICE_NUM;

    //根据当前相机模式，判断存储设备
    if (CAMERA_MODE_DC == MMIDC_GetCameraMode())
    {
        dc_dev = MMIDC_GetPhotoStorageDevice();            
    }
    else
    {
        dc_dev = MMIDC_GetVideoStorageDevice();
    }

    MMIDC_ShowErrorMsg(fs_error,dc_dev);
}

/*****************************************************************************/
//  Description : init flow parameter
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIDC_InitFlowParameter(void)
{
    s_dc_is_preview = FALSE;
    s_dc_is_open = FALSE;
    s_is_capture = FALSE;
    s_is_review = FALSE;
    s_dv_is_preview = FALSE;
    s_dv_is_open = FALSE;
    s_dv_is_record = FALSE;

    s_is_cancel_multishot = FALSE;
    s_is_snapshot_succss = TRUE;
    s_is_review_full_screen = FALSE;
    s_has_been_record = FALSE;

    MMIDC_SetCurrentMode(DC_PREVIEW_MODE);

    SCI_MEMSET(&s_photo_capture_info, 0x00,sizeof(s_photo_capture_info));
    SCI_MEMSET(&s_panorama_info, 0x00, sizeof(s_panorama_info));


    InitDCImageInfo();

    if(PNULL != s_callback_audio_semaphore)
    {
        SCI_DeleteSemaphore(s_callback_audio_semaphore);
        s_callback_audio_semaphore = PNULL;
    }

    return 0;
}

/*****************************************************************************/
// 	Description : is the photo of multi shoot photos deleted
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CanPressSave(void)
{
    return s_can_press_save;
}

/*****************************************************************************/
// 	Description : restore block property
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RestoreBlock(void)
{
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    ;
#else
    {
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    //cid 609639
    SCI_TRACE_LOW("[MMIDC] MMIDC_RestoreBlock");

    GUIBLOCK_ClearRect(&dev_info);
    #ifdef WIN32
    GUIBLOCK_ResetType(GUIBLOCK_TYPE_OSD, &dev_info);
    #else
    GUIBLOCK_ResetType(GUIBLOCK_TYPE_COMMON, &dev_info);
    #endif
#ifdef UI_MULTILAYER_SUPPORT
        UILAYER_SetLayerColorKey(&dev_info, FALSE, MMIDC_GetTransparentColor());
#endif

    }
#endif
}

/*****************************************************************************/
// 	Description : set block property
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_SetBlock(void)
{
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    ;
#else
{
    GUI_RECT_T       screen_rect = {0};
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16           logic_width = 0;
    uint16           logic_height = 0;
    GUI_COLOR_T      color = MMIDC_GetTransparentColor();
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SetBlock"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3579_112_2_18_2_8_35_164,(uint8*)"");

    GUILCD_GetLogicWidthHeight((LCD_ID_E)dev_info.lcd_id, &logic_width, &logic_height);
    screen_rect.top = 0;
    screen_rect.left = 0;
    screen_rect.right = logic_width - 1;
    screen_rect.bottom = logic_height - 1;

    GUIBLOCK_ClearRect(&dev_info);
    if(MMIDC_GetShootMode() == SHOOT_MODE_NORMAL)
    {
        #ifdef WIN32
        GUIBLOCK_SetType(0, color, GUIBLOCK_TYPE_OSD, &dev_info);
        #else
        GUIBLOCK_SetType(255, color, GUIBLOCK_TYPE_COMMON, &dev_info);
        #endif
    }
    else
    {
        if(MMIDC_GetShootMode() == SHOOT_MODE_PANORAMA_LEFT_RIGHT)
        {
            screen_rect.top = MMIDC_GetPreviewHeight() - MMIDC_GetPreviewHeight() / 5;
        }
        else
        {
            screen_rect.bottom = MMIDC_GetPreviewHeight() / 5 - 1;
        }
        GUIBLOCK_SetType(170, color, GUIBLOCK_TYPE_OSD, &dev_info);
    }
#if 0
    GUIBLOCK_SetRect(screen_rect, &dev_info);
#endif    
    //GUIBLOCK_ClearByColor(color, &dev_info);	
    LCD_FillRect(&dev_info, screen_rect, color);
    GUIBLOCK_Enable(&dev_info);

#ifdef UI_MULTILAYER_SUPPORT    
 	UILAYER_SetLayerColorKey(&dev_info, TRUE, MMIDC_GetTransparentColor());
#endif

}
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description : free last dv frame data buffert
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIDC_FreeLastFrame(void)
{
    if (PNULL !=s_lastframe_ptr)
    {
        SCI_FREE(s_lastframe_ptr);
        s_lastframe_ptr = PNULL;
    }

//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
    MMIDC_FreeA888LastFrame();
#endif
}

/*****************************************************************************/
// 	Description : get last frame data address
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC GUI_COLOR_T * MMIDC_GetLastFramePtr(void)
{
    return (GUI_COLOR_T *)s_lastframe_ptr;
}

/*****************************************************************************/
// 	Description : show photo frame or not
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsShowPhotoFrame(void)
{
    return s_is_show_photo_frame;
}

/*****************************************************************************/
// 	Description : show photo frame or not
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIDC_SetPhotoFrameForShow(BOOLEAN is_show)
{
    s_is_show_photo_frame = is_show;
}

/*****************************************************************************/
// 	Description : get the osd flag before preview
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetOSDFlagBeforePreview(void)
{
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetOSDFlagBeforePreview s_is_osd_flag_before_preview=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3670_112_2_18_2_8_35_165,(uint8*)"d",s_is_osd_flag_before_preview);
    
    return s_is_osd_flag_before_preview;
}

/*****************************************************************************/
// 	Description : set the osd flag before preview
//	Global resource dependence : none
//  Author: robert.wang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIDC_SetOSDFlagBeforePreview(BOOLEAN is_show)
{
    s_is_osd_flag_before_preview = is_show;
}

/*****************************************************************************/
//Description : open dc
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenDC(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    BOOLEAN is_dc_open = FALSE;

    DCAMERA_OP_MODE_E op_mode = 0;
    MMIDCSRV_MEM_CFG_E mem_cfg = 0;


    is_dc_open = s_dc_is_open;
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_OpenDC is_dc_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3695_112_2_18_2_8_35_166,(uint8*)"d", is_dc_open);

    if(!is_dc_open)
    {

    #if defined(MMIDC_F_U_CAMERA)
        if (MMIDC_GetUCameraModuleFlag())
        {
            op_mode = DCAMERA_OP_SPECIAL;
        }
        else
    #endif
        {
            mem_cfg = MMIDCSRV_GetDCMemCfg();
            switch(mem_cfg)
            {
            case MMIDCSRV_MEM_DC_UCAMERA:
                op_mode = DCAMERA_OP_NORMAL;
                break;
                
            case MMIDCSRV_MEM_DC_MINI:
                op_mode = DCAMERA_OP_MINIDC;
                break;
                
            default:
                op_mode = DCAMERA_OP_NORMAL;
                break;
            }
        }

        SCI_TRACE_LOW("[MMIDC] MMIDC_OpenDC op_mode=%d", op_mode);
        
        ret = DCAMERA_Open(op_mode);
        
        //SCI_TRACE_LOW:"[MMIDC]: MMIDC_OpenDC ret = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3709_112_2_18_2_8_35_167,(uint8*)"d", ret);

        if(DCAMERA_OP_SUCCESS == ret)
        {
            is_dc_open = TRUE;
        }
        else
        {
            DCAMERA_Close();
            is_dc_open = FALSE;
        }
        s_dc_is_open = is_dc_open;
    }

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_OpenDC is_dc_open 2= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3723_112_2_18_2_8_35_168,(uint8*)"d", is_dc_open);
    
    return is_dc_open;
}

/*****************************************************************************/
//Description : open dv
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_OpenDV(void)
{
    DRECORDER_RETURN_E ret = DRECORDER_SUCCESS;
    BOOLEAN is_dv_open = FALSE;

    MMIDCSRV_MEM_CFG_E mem_cfg = 0;
    

    is_dv_open = s_dv_is_open;
    
    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_OpenDV is_dv_open= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3739_112_2_18_2_8_35_169,(uint8*)"d", is_dv_open);
    
    if(!is_dv_open)
    {
        ret = DRECORDER_SelectSrcDevice(DRECORDER_SRC_DEV_DCAMERA);
        //SCI_TRACE_LOW:"[MMIDC]: DRECORDER_SelectSrcDevice = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3744_112_2_18_2_8_35_170,(uint8*)"d", ret);
        
        mem_cfg = MMIDCSRV_GetDVMemCfg();
        switch(mem_cfg)
        {
        case MMIDCSRV_MEM_DV_MINI:
            ret = DRECORDER_OpenEx(DRECORDER_OP_MINIDV);
            break;
            
        default:
            ret = DRECORDER_Open();
            break;
        }

        SCI_TRACE_LOW("[MMIDC] MMIDC_OpenDV ret=%d", ret); 
        
        if(DRECORDER_SUCCESS == ret)
        {
            is_dv_open = TRUE;
        }
        else
        {
            DRECORDER_Close();
            is_dv_open = FALSE;
        }
        
        s_dv_is_open = is_dv_open;
    }

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_OpenDV is_dv_open2= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3761_112_2_18_2_8_35_172,(uint8*)"d", is_dv_open);

    return is_dv_open;
}

/*****************************************************************************/
//Description : start preview for switch size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_StartPreviewForSwitchSize(void)
{
    BOOLEAN ret = FALSE;

    if((MMIDC_GetCameraMode() == CAMERA_MODE_DC)
        && MMIDC_IsDCOpen()
      )
    {
        SetLCDForRotation();
        MMIDC_UpdatePhotoZoomValue(MMIDC_GetPhotoZoomValue());  

        MMIDC_SetCurrentMode(DC_PREVIEW_MODE);

        MMIDC_SetPhotoFrameForShow(TRUE);

        MMIDC_ClearOSDBlock();
        MMIDC_SetOSDFlagBeforePreview(TRUE);
        MMIDC_ShowAllOSD();

        ret = StartDCPreview();
    }

    //SCI_TRACE_LOW:"[MMIDC]: MMIDC_StartPreviewForSwitchSize ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3793_112_2_18_2_8_36_173,(uint8*)"d",ret);

    return ret;
}     

/*****************************************************************************/
//Description : stop preview for switch size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_StopPreviewForSwitchSize(void)
{
    BOOLEAN ret = TRUE;

    StopDCPreview();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_StopPreviewForSwitchSize ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3809_112_2_18_2_8_36_174,(uint8*)"d",ret);

    return ret;
}


#if 0
/*****************************************************************************/
//Description : pda review photos for edit pic
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_PDAReviewPhotosForEditPic(void)
{
#if defined(MMIDC_DESKTOP_PDA)

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_PDAReviewPhotosForEditPic"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3958_112_2_18_2_8_36_179,(uint8*)"");
    
    MMIDC_CompleteDisplayOSD();

    if(SHOOT_MODE_NORMAL == MMIDC_GetShootMode())
    {
        s_is_review = TRUE;
        MMIDC_StartCallbackTimer();
        if(!ReviewSinglePhotoFromBuf())
        {
            MMIDC_ErrorTip(TXT_DC_CAPTURE_FAILED);
        }
    }    
#endif
}
#endif
//#if defined(MMIDC_DESKTOP_PDA) && !defined(MMIDC_F_PDA_BIT_16)
#if defined(MOCOR_SUPPORT_ARGB888_MODE)
/*****************************************************************************/
//Description : get last frame data as a888 format
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetA888LastFrameData(GUI_COLOR_T *src_ptr, //in
                                             uint32 src_width, //in
                                             uint32 src_height //in
                                             )
{
    uint32 buf_size = 0;
    uint32 i = 0;
    uint32 j = 0;
    uint32 k = 0;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetA888LastFrame src_ptr=0x%lX,width=%d,height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_3993_112_2_18_2_8_36_180,(uint8*)"ddd",src_ptr, src_width, src_height);
             
    if (PNULL == src_ptr)
    {
        return ;
    }
    
    MMIDC_FreeA888LastFrame();
    
    if (PNULL == s_pda_a888_lastframe_ptr)
    {
        buf_size = src_width * src_height * 4;
        s_pda_a888_lastframe_ptr = (uint32 *)SCI_ALLOCA(buf_size);
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetA888LastFrame s_pda_a888_lastframe_ptr=0x%lX"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4009_112_2_18_2_8_36_181,(uint8*)"d",s_pda_a888_lastframe_ptr);
    
    if (PNULL == s_pda_a888_lastframe_ptr)
    {
        return;
    }

    SCI_MEMSET(s_pda_a888_lastframe_ptr, 0x00, buf_size);
    for (i = 0; i < src_height; i++)
    {
        for (j = 0; j < src_width; j++)
        {
            s_pda_a888_lastframe_ptr[k] = RGB5652ARGB888(src_ptr[k], 255); /*lint !e648*/
            k++;
        }
    }

}

/*****************************************************************************/
//Description : get a888 last frame pointer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint32* MMIDC_GetA888LastFrameByPtr(void)
{
    return s_pda_a888_lastframe_ptr;
}

/*****************************************************************************/
//Description : free a888 last frame pointer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_FreeA888LastFrame(void)
{
    if (PNULL != s_pda_a888_lastframe_ptr)
    {
        SCI_FREE(s_pda_a888_lastframe_ptr);
    }
}
#endif

/*****************************************************************************/
//Description : init dc image info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitDCImageInfo(void)
{
	uint32 index = 0;
	MMIDC_FreeMemoryForReview();

//     SCI_MEMSET(&s_dc_info, 0, sizeof(s_dc_info));

	if(PNULL == s_image_info_ptr)
	{
		s_image_info_ptr = (MMIDC_IMAGE_INFO_T*)SCI_ALLOC_APPZ(sizeof(MMIDC_IMAGE_INFO_T));
		if (PNULL == s_image_info_ptr)
		{
			return;
		}
	}
	
	
	for (index = 0; index < MAX_DC_BURST_PHOTO_NUMBER; index ++)
	{
		if (PNULL == s_image_info_ptr->file_list[index].file_name_ptr)
		{
			s_image_info_ptr->file_list[index].file_name_ptr = (wchar*)SCI_ALLOC_APPZ((sizeof(wchar)) * (MMIFILE_FULL_PATH_MAX_LEN + 1));
			if (PNULL == s_image_info_ptr->file_list[index].file_name_ptr)
			{
				return;
			}
		}
	}
}

/*****************************************************************************/
//Description : reset dc image info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void ResetDCImageInfo(void)
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();
    int32 max_number = MMIDC_GetMaxBurstPhotoNumber();
    int32 i = 0;
    
    MMIDC_FreeMemoryForReview();

    if (PNULL != dc_info_ptr)
    {
        for (i = 0; i < max_number; i++)
        {
            dc_info_ptr->file_list[i].is_enable = FALSE;
            dc_info_ptr->file_list[i].file_size = 0;
        }
        
        dc_info_ptr->max_file_size = 0;
        dc_info_ptr->image_addr = 0;
        dc_info_ptr->image_len = 0;
    }
}

/*****************************************************************************/
//Description : free dc image info
//Global resource dependence : none
//Author: jinju.ma
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_FreeDCImageInfoMemory(void)
{
	uint32 index = 0;

	if(PNULL != s_image_info_ptr)
	{
		for (index = 0; index < MAX_DC_BURST_PHOTO_NUMBER; index ++)
		{
			if (PNULL != s_image_info_ptr->file_list[index].file_name_ptr)
			{
				SCI_FREE(s_image_info_ptr->file_list[index].file_name_ptr);
				s_image_info_ptr->file_list[index].file_name_ptr = PNULL;
			}
		}
		
		SCI_FREE(s_image_info_ptr);
		s_image_info_ptr = PNULL;		
	}
}
/*****************************************************************************/
//Description : alloc memory for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_AllocMemoryForReview(void)
{
    BOOLEAN ret = FALSE;
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();
    uint32 file_size = 0;

    if (PNULL != dc_info_ptr)
    {			
		if (PNULL == dc_info_ptr->review_addr)
        {
            file_size = MMIDC_GetMaxPhotoFileSize();
            if (file_size == 0)
            {
                file_size = MMIDC_GetMaxImageFileSize();
            }
            
            if (file_size > 0)
            {            
                dc_info_ptr->review_addr = SCI_ALLOCA(file_size);
            }
        }
        
        if (PNULL != dc_info_ptr->review_addr)
        {
            ret = TRUE;
            
            //SCI_TRACE_LOW:"[MMIDC] MMIDC_AllocMemoryForReview addr=0x%X"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4116_112_2_18_2_8_36_183,(uint8*)"d",dc_info_ptr->review_addr);
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_AllocMemoryForReview ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4121_112_2_18_2_8_36_184,(uint8*)"d",ret);
                 
    return ret;
}

/*****************************************************************************/
//Description : free memory for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_FreeMemoryForReview(void)
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();


#if 0
    if (MMIDC_Is2MSensorState()
        && ( MULTI_SHOOT_DISABLE == MMIDC_GetMultiShootEnum())
        )
     {
        SCI_TRACE_LOW("[MMIDC] MMIDC_FreeMemoryForReview direct return");

        InitReviewMemoryParamFor200WSensor();
        return ;
     }
#endif    


	if (PNULL != dc_info_ptr)
    {
        if (PNULL != dc_info_ptr->review_addr)
        {
            dc_info_ptr->review_file_size = 0;
            SCI_FREE(dc_info_ptr->review_addr);
        }
    }
		
}
#if 0
/*****************************************************************************/
//Description : init review memory param for 200w sensor
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitReviewMemoryParamFor200WSensor(void)
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();


    if (MMIDC_Is2MSensorState()
        && ( MULTI_SHOOT_DISABLE == MMIDC_GetMultiShootEnum())
        )
     {
        if (PNULL != dc_info_ptr)
        {
            if (PNULL != dc_info_ptr->review_addr)
            {
                dc_info_ptr->review_file_size = 0;
                dc_info_ptr->review_addr = PNULL;
            }
        }

        SCI_TRACE_LOW("[MMIDC] MMIDC_InitReviewMemoryParamFor200WSensor");        
     }

}
#endif
/*****************************************************************************/
//Description : set memory addr for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint8* MMIDC_SetMemoryAddrForReview(uint8 *data_ptr, 
                                                        uint32 data_size
                                                        )
{
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    if (PNULL != dc_info_ptr)
    {
        dc_info_ptr->review_addr = data_ptr;
        dc_info_ptr->review_file_size = data_size;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetMemoryAddrForReview addr_ptr=0x%X, data_size",\
        data_ptr, data_size);

    return data_ptr;
}

/*****************************************************************************/
//Description : get memory addr for review file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint8* MMIDC_GetMemoryAddrForReview(void)
{
    uint8 *addr_ptr = PNULL;
    MMIDC_IMAGE_INFO_T *dc_info_ptr = MMIDC_GetDCInfo();

    if (PNULL != dc_info_ptr)
    {
        addr_ptr = dc_info_ptr->review_addr;
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_AllocMemoryForReview addr_ptr=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4158_112_2_18_2_8_36_185,(uint8*)"d",addr_ptr);

    return addr_ptr;
}

/*****************************************************************************/
//Description : get current file buffer and size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCurrentFileBuffer(uint8 **buffer_ptr,//out
                                        uint32 *buffer_size //out
                                       )
{
    BOOLEAN ret = FALSE;
    wchar *full_path_ptr = PNULL;
    uint16 full_path_len = 0;
    uint8 *buf_ptr = 0;
    uint32 to_read_size = 0;
    PHOTO_STOREG_INFO_T *photo_info_ptr = PNULL;
    MMIDC_IMAGE_INFO_T *dc_info_ptr = PNULL;
    uint32 photo_index = 0;

#if defined(MMIDC_F_U_CAMERA)
    if (MMIDC_IsUCameraEnable())
    {
        ret = MMIDC_GetUCameraPhotoData(buffer_ptr,
                                        buffer_size
                                        );
    }

    if (!ret)
#endif
    {
        ret = MMIDC_AllocMemoryForReview();
        if (ret)
        {
            ret = FALSE;
            buf_ptr = MMIDC_GetMemoryAddrForReview();
            if (PNULL != buf_ptr)
            {
                //读保存后的文件
                photo_info_ptr = MMIDC_GetCurrentPhotoInfo();

                if (PNULL != photo_info_ptr)
                {
                    full_path_ptr = photo_info_ptr->file_info.file_name;
                    full_path_len = photo_info_ptr->file_info.file_name_len;
                    to_read_size = photo_info_ptr->file_info.file_size;
                }
                else
                {
                    //读临时文件
                    dc_info_ptr = MMIDC_GetDCInfo();
                    if (PNULL != dc_info_ptr)
                    {
                        photo_index = MMIDC_GetCurrentPhotoID();
                        full_path_ptr = dc_info_ptr->file_list[photo_index].file_name_ptr;
                        full_path_len = dc_info_ptr->file_list[photo_index].file_name_len;
                        to_read_size = dc_info_ptr->file_list[photo_index].file_size;
                    }
                    
                }
                
                SCI_TRACE_LOW("[MMIDC] GetCurrentFileBuffer full_path_ptr=%d, full_path_len=%d,to_read_size=%d",
                            full_path_ptr, full_path_len, to_read_size);                         

                if ((PNULL != full_path_ptr)
                    && (full_path_len > 0)
                    && (to_read_size > 0)
                   )
                {
                    ret = MMIAPIFMM_ReadFilesDataSyn(full_path_ptr,     //in
                                            full_path_len,      //in, 双字节为单位
                                            buf_ptr,           //out
                                            to_read_size        //in
                                            );
                   if (ret)
                   {
                        *buffer_ptr = buf_ptr;
                        *buffer_size = to_read_size;
                   }
                }
            }
            
        }

    }
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetCurrentFileBuffer ret=%d, buf_ptr=0x%X, buffer_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4241_112_2_18_2_8_37_186,(uint8*)"ddd",ret, *buffer_ptr, *buffer_size);
    
    return ret;
}

#if 0
int32 test_imageprocess(DCAMERA_DISPALY_PARAM_T *src_disp_ptr,
                            DCAMERA_DISPALY_PARAM_T *dst_disp_ptr)
{
    IMG_PROC_DATA_T src_img = {0};

    IMG_PROC_DATA_FORMAT_E format = IMG_PROC_YUV422;
static    uint32 handle = 0;
    IMG_PROC_CREATE_T handle_param = {0};
    IMG_PROC_PROCESS_T proc_param = {0};
    IMG_PROC_PROCESS_RTN_T proc_return = {0};
    
    int32 ret =0;

    uint32 width = 0;
    uint32 height = 0;
    uint32 *chn0_ptr = PNULL;
    uint32 *chn1_ptr = PNULL;
    //uint32 *chn2_ptr = PNULL;
    uint32 chn0_size = 0;
    uint32 chn1_size = 0;
    //uint32 chn2_size = 0;


    uint32 *dst_chn0_ptr = PNULL;
    uint32 *dst_chn1_ptr = PNULL;
    //uint32 *dst_chn2_ptr = PNULL;
    uint32 dst_chn0_size = 0;
    uint32 dst_chn1_size = 0;
    //uint32 dst_chn2_size = 0;

    IMG_PROC_ALG_CMD_E cmd  = 0;


    IMG_PROC_PENCILSKETCH_T pencil_sketch_param = {0};
    IMG_PROC_FISHEYE_T fisheye_param = {0};     
    
    //handle
    if (0 == handle) 
    {
        handle = IMGPROC_CreateHandle(&handle_param);
    }

    //SCI_TRACE_LOW:"test_imageprocess handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4289_112_2_18_2_8_37_187,(uint8*)"d",handle);

    if (handle > 0)
    {
        width = src_disp_ptr->width;
        height = src_disp_ptr->height;

        chn0_ptr = src_disp_ptr->yaddr; /*lint !e64*/
        chn1_ptr = src_disp_ptr->uaddr; /*lint !e64*/

        dst_chn0_ptr = dst_disp_ptr->yaddr; /*lint !e64*/
        dst_chn1_ptr = dst_disp_ptr->uaddr; /*lint !e64*/
        
        
        chn0_size = MMIDC_GetDataSizeByImgProcFormat(width,
                                                     height, 
                                                     1, 
                                                     format
                                                     );
                                                     
        chn1_size = MMIDC_GetDataSizeByImgProcFormat(width,
                                                     height, 
                                                     2, 
                                                     format
                                                     );
                                                     

        //SCI_TRACE_LOW:"test_imageprocess width=%d, height=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4317_112_2_18_2_8_37_188,(uint8*)"dd",width, height);

        //SCI_TRACE_LOW:"test_imageprocess src chn0_ptr=0x%X, chn0_size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4320_112_2_18_2_8_37_189,(uint8*)"dd",chn0_ptr, chn0_size);

        //SCI_TRACE_LOW:"test_imageprocess src chn1_ptr=0x%X, chn1_size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4323_112_2_18_2_8_37_190,(uint8*)"dd",chn1_ptr, chn1_size);

        //SCI_TRACE_LOW:"test_imageprocess dst chn0_ptr=0x%X, chn1_ptr=0x%X"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4326_112_2_18_2_8_37_191,(uint8*)"dd",dst_chn0_ptr, dst_chn1_ptr);

        //preview
        src_img.format = IMG_PROC_YUV422;
        src_img.addr.chn0 = (uint8*)chn0_ptr;
        src_img.addr.chn0_len = chn0_size;
        src_img.addr.chn1 =( uint8*)chn1_ptr;
        src_img.addr.chn1_len = chn1_size;
        src_img.name_ptr = NULL;
        src_img.size.w = width;
        src_img.size.h = height;


        cmd = IMG_PROC_BLACK_WHITE_DRAWING;        

        switch(cmd)
        {
        case IMG_PROC_BLACK_WHITE_DRAWING:
            proc_param.proc_param_ptr= &pencil_sketch_param;
            break;
        case IMG_PROC_FISHEYE:
            fisheye_param.distort_level = 100;
            proc_param.proc_param_ptr= &fisheye_param;
            break;
        default:
            break;
        }
        
        proc_param.cmd= cmd;
        
        proc_param.src_img_num=1;
        proc_param.src_img_ptr=(IMG_PROC_DATA_T_PTR)&src_img;


        dst_chn0_size = chn0_size;

        dst_chn1_size = chn1_size;


        proc_param.proc_img.rect.w = width;
        proc_param.proc_img.rect.h = height;
        proc_param.proc_img.data.format = IMG_PROC_YUV422;
        proc_param.proc_img.data.addr.chn0 = dst_chn0_ptr; /*lint !e64*/
        proc_param.proc_img.data.addr.chn0_len = dst_chn0_size;
        proc_param.proc_img.data.addr.chn1 = dst_chn1_ptr; /*lint !e64*/
        proc_param.proc_img.data.addr.chn1_len = dst_chn1_size;
        proc_param.proc_img.data.size.w = width;
        proc_param.proc_img.data.size.h = height;
        //proc_param.proc_img.ctrl_callback = Preview_ProcCallback;　// callback fun ptr

        // 2.) Preview第一帧 img proc 参数设置
        if (0 == s_frame_index)
        {
            proc_param.proc_mode = IMG_PROC_FIRST;
            proc_param.proc_mode |= IMG_PROC_PROCESS_EB;
        }
        else
        {
            // 3.) Preview其他帧 img proc 参数设置
            proc_param.proc_mode=IMG_PROC_UPDATE_SRC;
            proc_param.proc_mode|=IMG_PROC_PROCESS_EB;
        }

        s_frame_index++;
        
        #if 0
        // 4.) Capture img proc 参数设置
        proc_param.proc_img.rect.w=320;
        proc_param.proc_img.rect.h=240;
        proc_param.proc_img.data.format= IMG_PROC_JPG;
        proc_param.proc_img.data.addr.chn0=proc_buf;
        proc_param.proc_img.data.addr.chn0_len=proc_buf_size;
        proc_param.proc_img.rect.w=320;
        proc_param.proc_img.rect.h=240;
         proc_param.proc_img.ctrl_callback=Capture_ProcCallback;　// callback fun ptr

        proc_param.view_after.rect.w=320;
        proc_param.view_after.rect.h=240;
        proc_param.view_after.data.format=IMG_PROC_YUV422;
        proc_param.view_after.data.addr.chn0=after_buf;
        proc_param.view_after.data.addr.chn0_len=after_buf_size;
        proc_param.view_after.data.addr.chn1=after_buf1;
        proc_param.view_after.data.addr.chn1_len=after_buf_size;
        proc_param.view_after.data.size.w=160;
        proc_param.view_after.data.size.h=120;
        proc_param.view_after.ctrl_callback=Proc_Callback;

        proc_param.proc_mode=IMG_PROC_END;
        proc_param.proc_mode|=IMG_PROC_UPDATE_SRC;
        proc_param.proc_mode|=IMG_PROC_PROCESS_EB;

        #endif

        ret = IMGPROC_DoProcess(handle, &proc_param, &proc_return);

        //SCI_TRACE_LOW:"test_imageprocess IMGPROC_DoProcess ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4421_112_2_18_2_8_37_192,(uint8*)"d",ret);

    }

    return ret;
}
#endif
/*****************************************************************************/
//Description : display preview frame
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayPreviewFrame(uint32 start_preview_tick)
{
    DCAMERA_DISPALY_PARAM_T disp_src = {0};
    DCAMERA_DISPALY_PARAM_T disp_dst = {0};
    
    uint32 ret = 0;
    BOOLEAN is_skip_image_process = TRUE;

    uint32 t1 = 0;
    uint32 t2 = 0;
    
#if defined(MMIDC_F_U_CAMERA)
    MMIDC_IMAGE_PROCESS_INFO_T image_info = {0};
    MMIDC_IMAGE_PROCESS_OUT_INFO_T out_info = {0};
#endif        
    
    if (MMIDC_IsMMIBrushing())
    {
        SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayPreviewFrame brushing");
        return;
    }

    if (DC_PREVIEW_MODE != MMIDC_GetCurrentMode()
        && DC_CAPTURE_CONT_DOWN_MODE != MMIDC_GetCurrentMode())
    {
        SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayPreviewFrame not preview");
        return;
    } 

    if(MMIDC_LOSE_FOCUS == MMIDC_GetMMIFocusStatus())
    {
        SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayPreviewFrame lose focus");
        return;
    }

#if defined(MMIDC_F_U_CAMERA)
    if (!MMIDC_GetUCameraModuleFlag())
    {
        SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayPreviewFrame not ucamera");    
        return ;
    }
#endif    
	
	if (!MMIAPIDC_IsPreviewing())
	{
		SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayPreviewFrame not preview"); 
		return ;
	}

    if (start_preview_tick != MMIDC_GetStartPreviewTick())
	{
		SCI_TRACE_LOW("[MMIDC] MMIDC_DisplayPreviewFrame not same tick start_preview_tick = %d", start_preview_tick); 
		return ;
	}

    t1 = SCI_GetTickCount();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame t1=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4455_112_2_18_2_8_37_193,(uint8*)"d",t1);

    MMIDC_SetMMIBrushStatus(MMIDC_BRUSH_BRUSHING);

    disp_src = MMIDC_GetDCamSrcDisplay();
    disp_dst = MMIDC_GetDCamDstDisplay();
    
#if 1
    #if defined(MMIDC_F_U_CAMERA)
    if (MMIDC_GetUCameraModuleFlag())
    {
        image_info.data_from = UCAMERA_DATA_ROUTE_PREVIEW;
        image_info.data_size.w = disp_src.width;
        image_info.data_size.h = disp_src.height;
        
        image_info.data_type = disp_src.type;
        image_info.data_addr.chn1_ptr = disp_src.yaddr; /*lint !e64*/
        image_info.data_addr.chn2_ptr = disp_src.uaddr; /*lint !e64*/
        image_info.data_addr.chn3_ptr = disp_src.vaddr; /*lint !e64*/

        out_info.data_addr.chn1_ptr = disp_dst.yaddr; /*lint !e64*/
        out_info.data_addr.chn2_ptr = disp_dst.uaddr; /*lint !e64*/
        out_info.data_addr.chn3_ptr = disp_dst.vaddr; /*lint !e64*/

        MMIDC_PushImgProcMutex();
        ret = MMIDC_ProcessImage(&image_info, &out_info);
        MMIDC_PopImgProcMutex();
        if (0 == ret)
        {
            is_skip_image_process = out_info.is_skip_image_process;
            
            disp_dst.type = disp_src.type;
            disp_dst.pattern = disp_src.pattern;
        }

    }
    #endif
#else
     ret = test_imageprocess(&disp_src, &disp_dst);
     if (0 == ret)
     {
        is_skip_image_process = FALSE;
     }
#endif

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4499_112_2_18_2_8_37_194,(uint8*)"d",ret);
                  
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame is_skip_image_process=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4502_112_2_18_2_8_37_195,(uint8*)"d",is_skip_image_process);


#ifdef MMIDC_DEBUG_TRACE
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame src width=%d,height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4521_112_2_18_2_8_37_196,(uint8*)"dd",disp_src.width, disp_src.height);


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame src type=%d,pattern=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4525_112_2_18_2_8_37_197,(uint8*)"dd",disp_src.type, disp_src.pattern);
          
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame src yaddr=0x%X,uaddr=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4528_112_2_18_2_8_37_198,(uint8*)"dd",disp_src.yaddr, disp_src.uaddr);

    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame dst width=%d,height=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4532_112_2_18_2_8_37_199,(uint8*)"dd",disp_dst.width, disp_dst.height);


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame dst type=%d,pattern=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4536_112_2_18_2_8_37_200,(uint8*)"dd",disp_dst.type, disp_dst.pattern);
          
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame dst yaddr=0x%X,uaddr=0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4539_112_2_18_2_8_37_201,(uint8*)"dd",disp_dst.yaddr, disp_dst.uaddr);
#endif    

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame s_dc_is_preview=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4543_112_2_18_2_8_37_202,(uint8*)"d",s_dc_is_preview);

    if ((PNULL != disp_dst.yaddr)
        && (PNULL != disp_dst.uaddr)
        && (PNULL != disp_src.yaddr)
        && (PNULL != disp_src.uaddr)
        )
    
    {
        //若是不成功，直接将源帧刷新到屏幕上
        if ((0 != ret)
            || is_skip_image_process
            )/*lint !e774*/
        {
            SCI_MEMCPY((void*)disp_dst.yaddr, 
                       (void*)disp_src.yaddr, 
                       (uint32)(disp_src.width * disp_src.height));

            SCI_MEMCPY((void*)disp_dst.uaddr, 
                       (void*)disp_src.uaddr, 
                       (uint32)(disp_src.width * disp_src.height));
        }
        
        if (s_dc_is_preview)
        {
            DCAMERA_DisplayOneFrame(&disp_dst);
#ifdef MMIDC_F_N_IN_ONE
            if(MMIDC_IsNInOneEnable())
            {
                MMIDC_SetNInOneStartDisplayFrame(TRUE);
            }
#endif
        }
    }

    MMIDC_SetMMIBrushStatus(MMIDC_BRUSH_START);


    t2 = SCI_GetTickCount();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayPreviewFrame t2=%d, t2-t1=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4555_112_2_18_2_8_37_203,(uint8*)"dd",t2, t2 - t1);

}


/*****************************************************************************/
//Description : save data to file
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_SaveDataToFile(int32 photo_id,
                                         uint8 *data_buffer,//in
                                         uint32 data_size)
{
    int32 ret = -1;
    MMIFILE_ERROR_E  file_ret = 0;

    
    file_ret = SaveDataToFile(photo_id, data_buffer, data_size);
    if (SFS_ERROR_NONE == file_ret)
    {
        ret = 0;
        SendCaptureSuccessMsg(0, PNULL, 0);
    }
    else
    {
        ret = 0xFF;
        SendCaptureFailMsg(ret, &file_ret, sizeof(file_ret));
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SaveDataToFile ret =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4585_112_2_18_2_8_37_204,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
//  Description : send capture fail msg
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_SendCaptureFailMsg(int32 ret,//return value
                                             void* param_ptr,//in
                                             uint32 param_size)
{
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_SendCaptureFailMsg ret==%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_FLOW_4599_112_2_18_2_8_37_205,(uint8*)"d",ret);

    return SendCaptureFailMsg(ret, param_ptr, param_size);
}    

/*****************************************************************************/
//  Description : get capture callback semaphore ptr
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC SCI_SEMAPHORE_PTR MMIDC_GetCaptureCallbackSemaphorePtr(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = PNULL;

    
    semaphore_ptr = s_capture_callback_semaphore_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetCaptureCallbackSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);

    return semaphore_ptr;                 
}

/*****************************************************************************/
//  Description : set capture callback semaphore ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetCaptureCallbackSemaphorePtr(SCI_SEMAPHORE_PTR semaphore_ptr)
{
    s_capture_callback_semaphore_ptr = semaphore_ptr;

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetCaptureCallbackSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);             
}

/*****************************************************************************/
//  Description : create capture callback semaphore ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_CreateCaptureCallbackSemaphorePtr(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = MMIDC_GetCaptureCallbackSemaphorePtr();

    if (PNULL == semaphore_ptr)
    {
        semaphore_ptr = SCI_CreateSemaphore("capture callback semaphore", 1);
    }
    
    MMIDC_SetCaptureCallbackSemaphorePtr(semaphore_ptr);

    SCI_TRACE_LOW("[MMIDC] MMIDC_CreateCaptureCallbackSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);
}

/*****************************************************************************/
//  Description : delete capture callback semaphore ptr
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_DeleteCaptureCallbackSemaphorePtr(void)
{
    SCI_SEMAPHORE_PTR semaphore_ptr = PNULL;


    semaphore_ptr = MMIDC_GetCaptureCallbackSemaphorePtr();
    if (PNULL != semaphore_ptr)
    {
        MMIDC_PushCaptureCallbackSemaphore();
        MMIDC_PopCaptureCallbackSemaphore();
       
        SCI_DeleteSemaphore(semaphore_ptr);
        MMIDC_SetCaptureCallbackSemaphorePtr(PNULL);
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_DeleteCaptureCallbackSemaphorePtr semaphore_ptr=%d",
                 semaphore_ptr);             
}

/*****************************************************************************/
//  Description : push capture callback semaphore
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PushCaptureCallbackSemaphore(void)
{
#ifndef WIN32
    SCI_SEMAPHORE_PTR semaphore_ptr = MMIDC_GetCaptureCallbackSemaphorePtr();


    SCI_TRACE_LOW("[MMIDC] MMIDC_PushCaptureCallbackSemaphore semaphore_ptr=%d",\
                 semaphore_ptr);

    if (PNULL != semaphore_ptr)
    {
        SCI_GetSemaphore(semaphore_ptr, SCI_WAIT_FOREVER);
    }
#endif
}

/*****************************************************************************/
//  Description : pop capture callback semaphore
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_PopCaptureCallbackSemaphore(void)
{
#ifndef WIN32
    SCI_SEMAPHORE_PTR semaphore_ptr = MMIDC_GetCaptureCallbackSemaphorePtr();


    SCI_TRACE_LOW("[MMIDC] MMIDC_PopCaptureCallbackSemaphore semaphore_ptr=%d",\
                 semaphore_ptr);

    if (PNULL != semaphore_ptr)
    {
        SCI_PutSemaphore(semaphore_ptr);
    }
#endif
}

/*****************************************************************************/
//  Description : get start preview tick
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetStartPreviewTick(void)
{
    uint32 tick = 0;

    
    tick = s_start_preview_tick;

    SCI_TRACE_LOW("[MMIDC] MMIDC_GetStartPreviewTick tick=%d",
                 tick);

    return tick;                 
}

/*****************************************************************************/
//  Description : set start preview tick
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetStartPreviewTick(uint32 tick)
{    
    s_start_preview_tick = tick;

    SCI_TRACE_LOW("[MMIDC] MMIDC_SetStartPreviewTick tick=%d",
                 tick);              
}

#endif //#ifdef  CAMERA_SUPPORT

