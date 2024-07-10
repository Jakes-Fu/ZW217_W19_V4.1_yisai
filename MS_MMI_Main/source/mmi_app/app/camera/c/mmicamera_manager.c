#include "mmi_app_camera_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

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

static BOOLEAN					s_dc_is_open = FALSE;
static BOOLEAN					s_dc_is_preview = FALSE;
static DCAMERA_PREVIEW_PARAM_T  s_preview_param = {0};
static GUI_RECT_T               s_preview_rect = {0,0,MMI_MAINSCREEN_WIDTH -1, MMI_MAINSCREEN_HEIGHT - 1};
//static GUI_RECT_T               s_preview_rect = {0};
static MMICamera_PHOTO_SIZE_E   s_photo_size = 0;/*lint -esym(551,s_photo_size) */


/*****************************************************************************/
// 	Description : start to run the flow of camera
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_OpenDC(void)
{
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;
#ifdef UI_MULTILAYER_SUPPORT 	
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
#endif
    //GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();

    if(!s_dc_is_open)
    {
        ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
        if(DCAMERA_OP_SUCCESS != ret)
        {
            //SCI_TRACE_LOW:"[MMICamera]: DCAMERA_Open error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_40_112_2_18_2_8_25_0,(uint8*)"");
            return FALSE;
        }
        s_dc_is_open = TRUE;   
    }         
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
	//SCI_TRACE_LOW:"[MMICamera]: OpenDC ret = %d, s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_57_112_2_18_2_8_25_1,(uint8*)"dd", ret, s_dc_is_open);
    return TRUE;
}

/*****************************************************************************/
// 	Description : start photo preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_StartDCPreview(void)
{
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;
    
    MMICamera_InitDCPreviewParam();
    
    if(!s_dc_is_preview && s_dc_is_open)
    {
        //SCI_TRACE_LOW:"[MMICamera]: StartDCPreview disp w = %d, disp h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_73_112_2_18_2_8_25_2,(uint8*)"dd", s_preview_param.disp_rect.w, s_preview_param.disp_rect.h);
        //SCI_TRACE_LOW:"[MMICamera]: StartDCPreview target w = %d, target h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_74_112_2_18_2_8_25_3,(uint8*)"dd", s_preview_param.target_rect.w, s_preview_param.target_rect.h);
        
        ret = DCAMERA_StartPreview(&s_preview_param);
        if(DCAMERA_OP_SUCCESS != ret)
        {
            //SCI_TRACE_LOW:"[MMICamera]: DCAMERA_StartPreview ret = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_79_112_2_18_2_8_25_4,(uint8*)"d", ret);
            return FALSE;
        }
        s_dc_is_preview = TRUE;
    }
    //SCI_TRACE_LOW:"[MMICamera]: StartDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_84_112_2_18_2_8_25_5,(uint8*)"ddd", ret, s_dc_is_preview, s_dc_is_open);
    return TRUE;
}

/*****************************************************************************/
// 	Description : get preview param
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_InitDCPreviewParam(void)
{
    BOOLEAN result = FALSE;    
    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0} ;
    GUILCD_COORDINATE_PARAM_T  physical_coordinate = {0} ;
    LCD_ANGLE_E lcd_angle = LCD_ANGLE_0;
    
    s_preview_param.lcd_id = GUI_MAIN_LCD_ID ;
    //s_preview_param.lcd_block_id = GUI_BLOCK_MAIN ;
    s_preview_param.lcd_block_id = 0 ;
    
    //SCI_TRACE_LOW:"[MMICamera]: MMICamera_InitDCPreviewParam enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_103_112_2_18_2_8_25_6,(uint8*)"");
    
    //rotation
    lcd_angle = GUILCD_GetLogicAngle(s_preview_param.lcd_id);
    //SCI_TRACE_LOW:"[MMICamera]:  cur_angle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_107_112_2_18_2_8_25_7,(uint8*)"d", GUILCD_GetLogicAngle(s_preview_param.lcd_id));
    
    //lcd heigth width
  
    result = GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&logic_coordinate.lcd_width, &logic_coordinate.lcd_height);
    if(!result)
    {
        //SCI_TRACE_LOW:"[MMICamera]:  GUILCD_GetLogicWidthHeight, result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_114_112_2_18_2_8_25_8,(uint8*)"d", result);
        return result;
   }
    
    //rect
    logic_coordinate.rect = s_preview_rect ;

    //angle 
    logic_coordinate.angle = lcd_angle;

    //坐标转换
    result = GUILCD_ConvertLogicToPhysicalCoordinate( s_preview_param.lcd_id, 
        &logic_coordinate, &physical_coordinate); 
 
    //SCI_TRACE_LOW:"[MMICamera]:  GUILCD_ConvertLogicToPhysicalCoordinate, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_125_112_2_18_2_8_25_9,(uint8*)"d", result);
 
    
    //preview rect
    s_preview_param.disp_rect.x = physical_coordinate.rect.left  ;
    s_preview_param.disp_rect.y = physical_coordinate.rect.top;
    s_preview_param.disp_rect.w = physical_coordinate.rect.right- physical_coordinate.rect.left + 1;
    s_preview_param.disp_rect.h = physical_coordinate.rect.bottom - physical_coordinate.rect.top + 1;

    s_preview_param.disp_rect.x =  (s_preview_param.disp_rect.x  >> 1) << 1 ;
    s_preview_param.disp_rect.y =  (s_preview_param.disp_rect.y  >> 1) << 1 ;
    s_preview_param.disp_rect.w =  (s_preview_param.disp_rect.w  >> 1) << 1 ;
    s_preview_param.disp_rect.h=  (s_preview_param.disp_rect.h  >> 1) << 1 ;
    
    //SCI_TRACE_LOW:"[MMICamera]:  s_preview_param.disp_rect :x = %d, y = %d, w = %d, h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_141_112_2_18_2_8_25_10,(uint8*)"dddd", s_preview_param.disp_rect.x, s_preview_param.disp_rect.y ,s_preview_param.disp_rect.w , s_preview_param.disp_rect.h );
    
    //s_preview_param.rotation_mode = (DCAMERA_ROTATION_MODE_E)lcd_angle; 
    s_preview_param.rotation_mode = DCAMERA_ROTATION_0; 
    s_preview_param.disp_mode = DCAMERA_DISP_CUSTOMIZE;
    s_preview_param.target_rect = s_preview_param.disp_rect;
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : stop photo preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_StopDCPreview(void)
{
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;

    if(s_dc_is_preview && s_dc_is_open)
    {
        ret = DCAMERA_StopPreview();
        s_dc_is_preview = FALSE;
    }
    //SCI_TRACE_LOW:"[MMICamera]: StopDCPreview ret = %d, s_dc_is_preview = %d, s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_163_112_2_18_2_8_25_11,(uint8*)"ddd", ret, s_dc_is_preview, s_dc_is_open);
    return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : close dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_CloseDC(void)
{
    DCAMERA_RETURN_VALUE_E	ret = DCAMERA_OP_SUCCESS;

    if(s_dc_is_open)
    {
        ret = DCAMERA_Close();
        s_dc_is_open = FALSE;
        s_preview_rect = MMITHEME_GetFullScreenRect();
    }
    s_dc_is_preview = FALSE;
#if 0    
	GUIBLOCK_ResetMainType();
#endif    
	//SCI_TRACE_LOW:"[MMICamera]: CloseDC ret = %d, s_dc_is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_183_112_2_18_2_8_25_12,(uint8*)"dd", ret, s_dc_is_open);
    return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : snap shot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SnapShot(DCAMERA_CALL_BACK CaptureCallBack)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    DCAMERA_SNAPSHOT_PARAM_T snapshot_param = {0};
    DCAMERA_SNAPSHOT_RETURN_PARAM_T snapshot_return = {0};
    
    snapshot_param.data_type = DCAMERA_DATA_TYPE_JPEG;
    snapshot_param.buf_len = 0;
    snapshot_param.data_buf = PNULL;
    snapshot_param.file_name = PNULL;
    snapshot_param.snapshot_callback = CaptureCallBack;
    snapshot_param.rotation_mode = DCAMERA_ROTATION_0;
    snapshot_param.enc_quality_level = 6;
    snapshot_param.flash_enable	= 0;    
    snapshot_param.personal_snapshot.select_personal = 0;
    ret = DCAMERA_DoSnapshot(&snapshot_param, &snapshot_return);
    //SCI_TRACE_LOW:"[MMICamera]: SnapShot return: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_207_112_2_18_2_8_25_13,(uint8*)"d", ret);
    return (DCAMERA_OP_SUCCESS == ret);
}

/*****************************************************************************/
// 	Description : set preview rect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SetPreviewRect(GUI_RECT_T rect)
{
    SCI_MEMCPY(&s_preview_rect, &rect, sizeof(GUI_RECT_T));    
    return TRUE;
}

/*****************************************************************************/
// 	Description : set preview rect
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SetFullScreenPreview(BOOLEAN full_screen_mode)
{
    if (full_screen_mode)
    {
        //preview rect
        GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
        SCI_MEMCPY(&s_preview_rect, &full_rect, sizeof(GUI_RECT_T));
    }
    else
    {
        if(s_preview_rect.right == 0 || s_preview_rect.bottom == 0)
        {
            GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
            SCI_MEMCPY(&s_preview_rect, &full_rect, sizeof(GUI_RECT_T));
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : set preview rect
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMICamera_GetPreviewRect(void)
{
	return s_preview_rect;
}

/*****************************************************************************/
// 	Description : set snapshot photo size
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SetPhotoSize(MMICamera_PHOTO_SIZE_E size)
{
	DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;

//      转换成底层驱动支持的分辨率
	ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_SET_TARGET_SIZE, size, PNULL);       
        
	//SCI_TRACE_LOW:"[MMICamera]: SetPhotoSize return: %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICAMERA_MANAGER_265_112_2_18_2_8_25_14,(uint8*)"d", ret);
	s_photo_size = size;
	return (DCAMERA_OP_SUCCESS == ret); 
}

