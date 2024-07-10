#ifdef HERO_ENGINE_SUPPORT

#ifdef __HERO_CFG_CAMERA_SUPPORT__ 
#include "window_parse.h"
#include "heroEngineId.h"

#include "heroEngineApp.h"
#include "heroEngineApi.h"
#include "heroEngineInternal.h"
#include "heroEngineCamera.h"
#include "guilcd.h"
#ifndef WIN32
#include "dcamera_misc.h"
#include "dcamera_common.h"
#endif
#include "dal_dcamera.h"
#include "mmk_type.h"
#include "mmidc_setting.h"


extern hr_common_rsp_t g_hr_common_rsp;
extern hr_layer_info_t  g_hr_layer_info;
extern hr_layer_active_info_t g_hr_active_layer;

static GUI_LCD_DEV_INFO g_preview_layer = {0};
static uint8 g_camera_preview_timer = 0;
static hr_camera_ctrl_t gHeroCameraReq ={0};

uint32 hr_camera_capture_callback(DCAMERA_RETURN_VALUE_E ret_value, //in
                                        void* param_ptr, //in
                                        uint32 param_size //in
                                        )
{
	MMIHR_Trace(("[HERO] hr_camera_capture_callback ret_value = %d", ret_value));
	 
    return 0;
}

int32 hr_camera_init(int32 param)
{
    gHeroCameraReq.state = HERO_CAMERA_INITED;
    MMIDC_AllocSettingMemory();
#if (HERO_SPR_VERSION >= 0x10A1140)
    MMIDC_Setting_GetNVDefaultValue();
#else
    MMIDC_Setting_InitDefaultValue();
#endif

    return hr_camera_create();
}

void hr_camera_set_preview_param(hr_camera_launch_info_t *pReq)
{
#ifndef WIN32
    if(pReq != NULL)
    {
        SCI_MEMSET(&gHeroCameraReq, 0x00, sizeof(gHeroCameraReq));
        gHeroCameraReq.pre_req.preview_x = DC_RIGHT_TRUNK(pReq->preview_x, 4);
        gHeroCameraReq.pre_req.preview_y = DC_RIGHT_TRUNK(pReq->preview_y, 2);
        gHeroCameraReq.pre_req.preview_w = DC_RIGHT_TRUNK(pReq->preview_w, 4);
        gHeroCameraReq.pre_req.preview_h = DC_RIGHT_TRUNK(pReq->preview_h, 4);
        gHeroCameraReq.pre_req.img_w = DC_RIGHT_TRUNK(pReq->img_w, 16);
        gHeroCameraReq.pre_req.img_h = DC_RIGHT_TRUNK(pReq->img_h, 8);
        gHeroCameraReq.pre_req.img_qt= (uint32)(pReq->img_qt & 0x0000ffff);
        gHeroCameraReq.is_from_jsky = (int32)((pReq->img_qt & 0xffff0000) != 0);
    }
#endif
}

int32 hr_camera_preview(void)
{
    DCAMERA_PREVIEW_PARAM_T preview_param = {0};
    
    //需要设置默认参数，否则会画面暗
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, DCAMERA_EV_03,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, DCAMERA_EV_03,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EFFECT, DCAMERA_EFFECT_NORMAL,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, DCAMERA_EV_03,PNULL); 
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, DCAMERA_EV_00,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_PREVIEW_MODE, DCAMERA_ENVIRONMENT_NORMAL,PNULL);
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE, DCAMERA_WB_MODE_AUTO,PNULL);
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_SET_TARGET_SIZE, MMIDC_ConvertPhotoSizeToDCameraSize(MMIDC_GePhotoSizetVerLcd()), NULL);

    preview_param.lcd_id        = GUI_MAIN_LCD_ID;
    preview_param.lcd_block_id  = GUI_BLOCK_0;
    preview_param.disp_rect.x   = gHeroCameraReq.pre_req.preview_x;
    preview_param.disp_rect.y   = gHeroCameraReq.pre_req.preview_y;
    preview_param.disp_rect.w   = gHeroCameraReq.pre_req.preview_w;
    preview_param.disp_rect.h   = gHeroCameraReq.pre_req.preview_h;
    preview_param.disp_mode = DCAMERA_DISP_FULLSCREEN;
#ifdef CAMERA_SENSOR_ANGLE_90
    preview_param.rotation_mode = DCAMERA_ROTATION_90;
#else
    preview_param.rotation_mode = DCAMERA_ROTATION_0;
#endif
    preview_param.target_rect = preview_param.disp_rect;

    MMIHEROAPP_SetAllowTurnOffBacklight(FALSE);
    return DCAMERA_StartPreview(&preview_param);
}

int32 hr_camera_set_block(int32 block_id)
{
    GUI_RECT_T       screen_rect = {0};
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16           logic_width = 0;
    uint16           logic_height = 0;
    GUI_COLOR_T      color = MMIDC_GetTransparentColor();
    
    MMIHR_Trace(("[HERO] hr_camera_set_block block_id = %d", block_id));

    dev_info.block_id = block_id;

    GUILCD_GetLogicWidthHeight((LCD_ID_E)dev_info.lcd_id, &logic_width, &logic_height);
    screen_rect.top = 0;
    screen_rect.left = 0;
    screen_rect.right = logic_width - 1;
    screen_rect.bottom = logic_height - 1;

    GUIBLOCK_ClearRect(&dev_info);
    
#ifdef WIN32
    GUIBLOCK_SetType(0, color, GUIBLOCK_TYPE_OSD, &dev_info);
#else
    GUIBLOCK_SetType(255, color, GUIBLOCK_TYPE_COMMON, &dev_info);
#endif

    GUIBLOCK_SetRect(screen_rect, &dev_info);
    GUIBLOCK_ClearByColor(color, &dev_info);		
    GUIBLOCK_Enable(&dev_info);
    
    return TRUE;
}

int32 hr_camera_start_preview(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    int32 ret = 0;
    UILAYER_INFO_T   layer_info = {0};
    UILAYER_CREATE_T layer_create = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUI_LCD_DEV_INFO preview_dev_info = {0};
    GUI_LCD_DEV_INFO *dev_info_ptr = MMITHEME_GetDefaultLcdDev();
	hr_camera_launch_info_t *pReq = (hr_camera_launch_info_t*)input; 

	if (input_len < sizeof(hr_camera_launch_info_t) || input == NULL || output == NULL || output_len == NULL)
	{
		return HR_FAILED;
	}

    MMIHR_Trace(("[HERO] hr_camera_start_preview x:%d y:%d w:%d h:%d img_w:%d img_h:%d", \
        pReq->preview_x, pReq->preview_y, pReq->preview_w, pReq->preview_h, pReq->img_w, pReq->img_h));

    hr_camera_set_preview_param(pReq);

    layer_create.lcd_id = GUI_MAIN_LCD_ID;
    layer_create.owner_handle = MMI_HERO_APP_CAMERA_WIN_ID;
    layer_create.offset_x = 0;
    layer_create.offset_y = 0;
    layer_create.width = HR_SCREEN_WIDTH;
    layer_create.height= HR_SCREEN_HEIGHT;
    layer_create.is_static_layer = TRUE;
    layer_create.is_bg_layer = TRUE;
    ret = UILAYER_CreateLayer(&layer_create, &g_preview_layer);
    MMIHR_Trace(("[HERO] g_preview_layer = %d ret:%d", g_preview_layer.block_id, ret));
    
    if (UILAYER_IsLayerActive(&g_preview_layer))
    {
#if (HERO_SPR_VERSION >= 0x10A1140)    
        UILAYER_RemoveMainLayer();
		UILAYER_SetLayerColorKey(&g_preview_layer, TRUE, MMI_BLACK_COLOR);
#else
        UILAYER_SetLayerColorKey(&g_preview_layer, TRUE, MMI_BLUE_COLOR);
#endif            
        UILAYER_Clear(&g_preview_layer);
        append_layer.lcd_dev_info = g_preview_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
#if (HERO_SPR_VERSION < 0x10A1140)    
        hr_camera_set_block(g_preview_layer.block_id);
#endif
        ret = hr_camera_preview();
        MMIHR_Trace(("[HERO] hr_camera_preview ret:%d isJsky:%d", ret, gHeroCameraReq.is_from_jsky));

        if(ret == DCAMERA_OP_SUCCESS)
        {
            UILAYER_GetLayerInfo(&g_preview_layer, &layer_info);
#if (HERO_SPR_VERSION >= 0x10A1140)
            g_hr_common_rsp.p2 = UILAYER_GetLayerBufferPtr(&g_preview_layer);
#else
            g_hr_common_rsp.p2 = layer_info.layer_buf_ptr;
#endif

            g_hr_common_rsp.p1 = 1;
        	*output = (uint8*)&g_hr_common_rsp;
        	*output_len = sizeof(g_hr_common_rsp);

            gHeroCameraReq.state = HERO_CAMERA_PREVIEW;
            return HR_SUCCESS;
        }
        else
        {
            return HR_FAILED;
        }
    }
    else
    {
        return HR_FAILED;
    }
}

int32 hr_camera_get_preview_data(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
    UILAYER_INFO_T   layer_info = {0};
    GUI_LCD_DEV_INFO dev_info   = {0};
    GUI_LCD_DEV_INFO *default_dev_info_ptr = MMITHEME_GetDefaultLcdDev();
            
	if(output == NULL || output_len == NULL)
		return HR_FAILED;
        
    dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    dev_info.block_id = g_hr_active_layer.block_id;

    if(g_hr_active_layer.block_id != UILAYER_NULL_HANDLE && !UILAYER_IsEmptyLayerHandle(&dev_info))
    {
		g_hr_layer_info.handle = g_hr_active_layer.block_id;
        UILAYER_GetLayerInfo(&dev_info, &layer_info);
    }
    else
    {
		g_hr_layer_info.handle = default_dev_info_ptr->block_id;
        UILAYER_GetLayerInfo(default_dev_info_ptr, &layer_info);
    }

	g_hr_layer_info.w = layer_info.layer_width;
	g_hr_layer_info.h = layer_info.layer_height;
#if (HERO_SPR_VERSION >= 0x10A1140)
	g_hr_layer_info.buffer = UILAYER_GetLayerBufferPtrEx(default_dev_info_ptr);
#else
	g_hr_layer_info.buffer = layer_info.layer_buf_ptr;
#endif

	*output = (uint8*)&g_hr_layer_info;
	*output_len = sizeof(g_hr_layer_info);
    return HR_SUCCESS;
}

int32 hr_camera_capture(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
#ifndef WIN32
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    DCAMERA_SNAPSHOT_PARAM_T snapshot_param = {0};
    DCAMERA_SNAPSHOT_RETURN_PARAM_T snapshot_return = {0};

    if(gHeroCameraReq.state == HERO_CAMERA_UNINIT || gHeroCameraReq.state == HERO_CAMERA_PAUSED)
    {
        return HR_FAILED;
    }

    snapshot_param.data_type = DCAMERA_DATA_TYPE_JPEG;
    snapshot_param.buf_len = 0;
    snapshot_param.data_buf = PNULL;
    snapshot_param.file_name = PNULL;
    snapshot_param.snapshot_callback = NULL;
    snapshot_param.quickview_callback = NULL;
	snapshot_param.capture_start_callback = hr_camera_capture_callback;
#ifdef CAMERA_SENSOR_ANGLE_90
    snapshot_param.rotation_mode = DCAMERA_ROTATION_90;
#else
    snapshot_param.rotation_mode = DCAMERA_ROTATION_0;
#endif
    snapshot_param.enc_quality_level = DC_PHOTO_QUALITY_8;
    snapshot_param.flash_enable	= 0;    
    snapshot_param.personal_snapshot.select_personal = 0;

    ret = DCAMERA_DoSnapshot(&snapshot_param, &snapshot_return);
    MMIHR_Trace(("[HERO]: hr_camera_capture ret = %d buf=0x%08x size=%d", ret, snapshot_return.return_data_addr, snapshot_return.return_data_len));	
    if(DCAMERA_OP_SUCCESS == ret)
    {
        g_hr_common_rsp.p1 = snapshot_return.return_data_addr;
        g_hr_common_rsp.p2 = snapshot_return.return_data_len;

        if(*input == 1)
        {	
		    g_camera_preview_timer = MMK_CreateWinTimer(MMI_HERO_APP_CAMERA_WIN_ID, 100, FALSE);
        }
        
        *output = (uint8*)&g_hr_common_rsp;
        *output_len = sizeof(g_hr_common_rsp);

        return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }    
#endif
}

int32 hr_camera_get_zoom_max(int32 param)
{
	return HR_IGNORE;
}

int32 hr_camera_zoom_in(int32 param)
{
	return HR_IGNORE;
}

int32 hr_camera_zoom_out(int32 param)
{
	return HR_IGNORE;
}

int32 hr_camera_resume(int32 param)
{
    int32 ret = 0;
    UILAYER_APPEND_BLT_T append_layer = {0};
    
	MMIHR_Trace(("[HERO] hr_camera_resume state = %d", gHeroCameraReq.state));

#ifndef WIN32 
	if (gHeroCameraReq.state == HERO_CAMERA_PAUSED)
	{
    	ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
    	if(ret == DCAMERA_OP_SUCCESS)
        {
#if (HERO_SPR_VERSION >= 0x10A1140)
            UILAYER_RemoveMainLayer();
#endif
            append_layer.lcd_dev_info = g_preview_layer;
            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            UILAYER_AppendBltLayer(&append_layer);  
        	ret = hr_camera_preview();
            MMIHR_Trace(("[HERO] hr_camera_resume ret = %d", ret));
        	gHeroCameraReq.state = HERO_CAMERA_PREVIEW;
    		return HR_SUCCESS;
        }
	}
#endif
	return HR_FAILED;
}

int32 hr_camera_pause(int32 param)
{
    MMIHR_Trace(("[HERO] hr_camera_pause state = %d", gHeroCameraReq.state));
#ifndef WIN32
	if (gHeroCameraReq.state == HERO_CAMERA_PREVIEW)
	{
	    DCAMERA_StopPreview();
        DCAMERA_Close();
        MMIHEROAPP_SetAllowTurnOffBacklight(TRUE);
        UILAYER_RemoveBltLayer(&g_preview_layer);
#if (HERO_SPR_VERSION >= 0x10A1140)
        UILAYER_RestoreMainLayer();
#endif
		gHeroCameraReq.state = HERO_CAMERA_PAUSED;
		return HR_SUCCESS;
	}
	else
	{
		return HR_FAILED;
	}
#endif
}

int32 hr_camera_destroy(int32 param)
{
#ifndef WIN32
    int32 ret = DCAMERA_OP_SUCCESS;

    if(gHeroCameraReq.state == HERO_CAMERA_UNINIT)
    {
        return HR_FAILED;
    }

    if(gHeroCameraReq.state == HERO_CAMERA_PREVIEW)
    {
        ret = DCAMERA_StopPreview();
        MMIHEROAPP_SetAllowTurnOffBacklight(TRUE);
    }
    MMIDC_FreeSettingMemory();
    ret = DCAMERA_Close();
    MMIHR_Trace(("[HERO] hr_camera_destroy ret = %d", ret));
    gHeroCameraReq.state = HERO_CAMERA_UNINIT;
    
#if (HERO_SPR_VERSION >= 0x10A1140)
    UILAYER_RestoreMainLayer();
#endif
    MMK_CloseWin(MMI_HERO_APP_CAMERA_WIN_ID);
    if(DCAMERA_OP_SUCCESS == ret || DCAMERA_OP_NOT_OPEN == ret)
    {
        return HR_SUCCESS;
    }
    else
    {
        return HR_FAILED;
    }
#endif
}

int32 hr_camera_handle_timer(uint8 param)
{
    int32 ret = 0;
    
	if(g_camera_preview_timer == param)
    {
        MMK_StopTimer(g_camera_preview_timer);
        g_camera_preview_timer = 0;
        ret = hr_camera_preview();
        MMIHR_Trace(("[HERO] hr_camera_handle_timer ret = %d", ret));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
#endif//HERO_ENGINE_SUPPORT
