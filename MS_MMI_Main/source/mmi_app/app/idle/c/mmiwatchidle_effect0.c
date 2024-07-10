
#include "mmiwatchidle_effect0.h"
#include "guigraph.h"
#include "mmiidle_export.h"
#include "mmk_type.h"

#include "graphics_draw.h"

#ifdef ANALOG_CLK_SUPPORT  //yangyu add


//The Definition of WatchIdle Window. static mode
#if 0
WINDOW_TABLE(MMI_WATCHIDLE_WINTAB) =
{
    WIN_FUNC((uint32)MMIWatchIdle_HandleWinMsg),
    WIN_HIDE_STATUS,
    WIN_ID(MAIN_IDLE_WIN_ID),
    WIN_STYLE( WS_DISABLE_RETURN_WIN|WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG ),
    CREATE_ANIM_CTRL(MMIWATCHIDLE_BG_ID,MAIN_IDLE_WIN_ID),
    END_WIN
};
#endif

LOCAL BOOLEAN MMIWatchIdle_CreatWallpaperCtrl();
//LOCAL void DrawAClock(MMI_WIN_ID_T win_id, int16 top);
LOCAL void DrawAClock(MMI_WIN_ID_T win_id, const void *param);


LOCAL void createBackground(void* param)
{
	//MMIWatchIdle_CreatWallpaperCtrl();
}

LOCAL void createClock(void* param)
{
    //Do nothing
}

LOCAL void updateBackground(void* param)
{
	//GUIANIM_UpdateDisplay(MMIWATCHIDLE_BG_ID);
#if 0 //xiongkai

    GUI_COLOR_T bgColor = 0;
    const t_watchidle_callback *obj = (const t_watchidle_callback *)param;
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    GUI_RECT_T win_rect = MMITHEME_GetFullScreenRect();
    if (param == NULL)
    {
        return;
    }

    if (obj->m_pData != NULL)
    {
        bgColor = obj->m_pData->mBgColor;
    }
	lcd_dev_info =  obj->m_pData->mLcdInfo;
    GUI_FillRect(&lcd_dev_info,win_rect,(GUI_COLOR_T)bgColor);
    return;
#endif	
}

LOCAL void updateClock(void* param)
{
	//DrawAClock(MAIN_IDLE_WIN_ID, MMI_STATUSBAR_HEIGHT);
	DrawAClock(MAIN_IDLE_WIN_ID, param);
}

LOCAL void destoryBackground(void* param)
{
     //Do nothing
}

LOCAL void destoryClock(void* param)
{
    //Do nothing
}




/*****************************************************************************/
//  Description : MMIWatchIdle_CreatWallpaperCtrl
//  Global resource dependence : 
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWatchIdle_CreatWallpaperCtrl()
{
    GUI_BOTH_RECT_T anim_ctrl_rect  = MMITHEME_GetFullScreenBothRect();
    BOOLEAN result = TRUE;
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
    MMIAPIIDLE_SetInitWallpaper_KL(FALSE, MAIN_IDLE_WIN_ID, MMIWATCHIDLE_BG_ID,TRUE);    
#else
    MMIAPIIDLE_SetInitWallpaper(FALSE,MAIN_IDLE_WIN_ID,MMIWATCHIDLE_BG_ID);
#endif
    GUIAPICTRL_SetBothRect(MMIWATCHIDLE_BG_ID,&anim_ctrl_rect);
    GUIAPICTRL_SetState(MMIWATCHIDLE_BG_ID, 
						GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, 
						TRUE);
						
    GUIWIN_SetStbBgIsDesplay(MAIN_IDLE_WIN_ID, TRUE);
    return (result);
}


/*****************************************************************************/
//  Description : DrawAClock
//  Global resource dependence : 
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
//LOCAL void DrawAClock(MMI_WIN_ID_T win_id,int16 top)
LOCAL void DrawAClock(MMI_WIN_ID_T win_id, const void *param)
{
#if 0
	GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
	GUI_RECT_T win_rect;
	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	MMK_GetWinRect((MMI_HANDLE_T)1, &win_rect);
	GUI_FillRect(&lcd_dev_info,win_rect,(GUI_COLOR_T)0xf000);
	return;
#else

    ///////////////////
    uint32 mem_width_ptr = 0;
    uint32 dst_widget_type = 0;
    void *lcd_buffer_ptr = NULL;
	
    ////////////////////

//#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T        win_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    SCI_TIME_T        sys_time        = {0};
    GUI_POINT_T       point = {0};  
    CLOCK_DRAW_IN_T   aclock_input_param  = {0};
    CLOCK_DRAW_OUT_T  aclock_output_param = {0};
    MMI_WATCHIDLE_CLOCK_ALPHA_IMG_T bk_img = {0};
    MMI_WATCHIDLE_CLOCK_ALPHA_IMG_T fore_img = {0};

//#undef UI_MULTILAYER_SUPPORT
#if (defined UI_MULTILAYER_SUPPORT) && (!defined IDLE_MAINMENU_LOW_MEMORY_SUPPORT)
    UILAYER_INFO_T    layer_info = {0};
#endif

    GUI_RECT_T fullScreenRect = MMITHEME_GetFullScreenRect();
    uint16            lcd_width  = 0;
    uint16            lcd_height = 0;
    uint16            width  = 0;
    uint16            height = 0;

    uint16 nImageWidth = 240;//320;
    uint16 nImageHeight = 240;//320;

    uint16 nScreenWidth =fullScreenRect.right - fullScreenRect.left - 1;
    uint16 nScreenHeight = fullScreenRect.bottom - fullScreenRect.top - 1;
    //lzk begin. 2018.04.12
    //add pure dark bg
    static int sec_times = 0;
    sWatchPanelData *pData = NULL;

    t_watchidle_callback *obj = (t_watchidle_callback *)param;
    if (obj == NULL)
    {
        return;
    }

    if (obj->m_pData == NULL)
    {
        return;
    }
    pData = obj->m_pData;

    lcd_dev_info = pData->mLcdInfo;

    pData->mPanelImgId=IMAGE_WATCHIDLE_ACLOCK_PANEL1;
    GUIRES_GetImgWidthHeight(&nImageWidth, &nImageHeight, pData->mPanelImgId, win_id);
    
    point.x = nScreenWidth - nImageWidth >> 1;
    point.y = nScreenHeight - nImageHeight >> 1;
    SCI_TRACE_LOW("DrawAClock() enter.");
    
    //aclock dial.

    aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, IMAGE_WATCHIDLE_ACLOCK_PANEL1, win_id);
    aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_PANEL1,win_id,&(aclock_input_param.clock_dial.data_size));
    aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
    aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2;   


    //aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, IMAGE_WATCHIDLE_ACLOCK_HOUR, win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_HOUR,win_id,&(aclock_input_param.hour_handle.data_size));
	aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, IMAGE_WATCHIDLE_ACLOCK_MIN, win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_MIN,win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height/2; 

    //aclock sec hand
    aclock_input_param.sec_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.sec_handle.width, &aclock_input_param.sec_handle.height, IMAGE_WATCHIDLE_ACLOCK_SEC, win_id);        
    aclock_input_param.sec_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_SEC,win_id,&(aclock_input_param.sec_handle.data_size));
    aclock_input_param.sec_handle.center_x = aclock_input_param.sec_handle.width/2;
    aclock_input_param.sec_handle.center_y = aclock_input_param.sec_handle.height/2; 

    aclock_input_param.target_buf_size = aclock_input_param.clock_dial.width * aclock_input_param.clock_dial.height * 4;
    aclock_input_param.target_buf_ptr = (void*)SCI_ALLOCA(aclock_input_param.target_buf_size);

    if (PNULL == aclock_input_param.target_buf_ptr)
    {
        //SCI_TRACE_LOW:"error: DrawAClock target_buf_ptr alloc failed,  return directly"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_KEYLOCK_557_112_2_18_2_31_4_6,(uint8*)"");
        return;
    }

    //get system time
    TM_GetSysTime(&sys_time);  

#ifdef WIN32
    aclock_input_param.time.hour = sys_time.hour;
    aclock_input_param.time.minute  = sys_time.min;
    aclock_input_param.time.second = (sec_times++) % 60;//sys_time.sec;
#else
    aclock_input_param.time.hour = sys_time.hour;
    aclock_input_param.time.minute  = sys_time.min;
    aclock_input_param.time.second = sys_time.sec;
#endif

#if defined(ANALOG_CLK_SUPPORT)
    #if 1//def WIN32
    {
        GRAPH_DrawClock(&aclock_input_param, &aclock_output_param);//yangyu ,now only simulator can use
    }
    #endif
#endif

#if 0//xiongkai 
    //display aclock
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    
    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    bk_img.data_type = layer_info.data_type; // IMGREF_FORMAT_RGB565;
    bk_img.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);	

    if (PNULL == bk_img.data_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
	 aclock_input_param.target_buf_ptr = NULL;	
        SCI_TRACE_LOW("error: DrawAClock PNULL == bk_img.data_ptr");      
        return;
    }	
#else
    //display aclock
    MMK_GetWinRect(win_id, &win_rect);

    //Default lcd
    //lcd_dev_info = *MMITHEME_GetDefaultLcdDev();

    //lcd by multilayer or defalut
    //lcd_dev_info.block_id = GetIdlePaintLcdDev()->block_id;
    //lcd_dev_info.lcd_id = GetIdlePaintLcdDev()->lcd_id;

#if (defined UI_MULTILAYER_SUPPORT) && (!defined IDLE_MAINMENU_LOW_MEMORY_SUPPORT)
    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    
    bk_img.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    bk_img.data_type = layer_info.data_type; 
    //bk_img.data_type = IMGREF_FORMAT_RGB565;
    bk_img.width = layer_info.mem_width;
    bk_img.height = layer_info.layer_height;
#else
    GetLcdInfoByLcdDev_All(&lcd_dev_info, 
    &mem_width_ptr, 
    &bk_img.width, 
    &bk_img.height, 
    &dst_widget_type,
    &lcd_buffer_ptr);

    //bk_img.data_type = IMGREF_FORMAT_RGB565;
    bk_img.data_type = dst_widget_type;
    bk_img.data_ptr = lcd_buffer_ptr;
#endif

#if 0//xiongkai 	
    if (PNULL == bk_img.data_ptr)
    {
    	    return;
    }
	
#else	
    if (PNULL == bk_img.data_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
	 aclock_input_param.target_buf_ptr = NULL;	
        SCI_TRACE_LOW("error: DrawAClock PNULL == bk_img.data_ptr");      
        return;
    }	
#endif	
#endif

#if 0 //xiongkai 
    fore_img.data_ptr = aclock_output_param.output_ptr;
    if (PNULL == fore_img.data_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
	 aclock_input_param.target_buf_ptr = NULL;	
        SCI_TRACE_LOW("error: DrawAClock PNULL == fore_img.data_ptr");      
        return;
    } 
    fore_img.data_type = aclock_output_param.type;  //IMGREF_FORMAT_RGB888
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;	
#else	


    fore_img.data_ptr = aclock_output_param.output_ptr;
    fore_img.data_type = aclock_output_param.type;//IMGREF_FORMAT_ARGB888;
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;

#if 1 //xiongkai 
    if (PNULL == fore_img.data_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
	 aclock_input_param.target_buf_ptr = NULL;	
        SCI_TRACE_LOW("error: DrawAClock PNULL == fore_img.data_ptr");      
        return;
    } 
#endif	
	
#endif   

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    //fore_img.start_x = (lcd_width - aclock_input_param.clock_dial.width)/2;   
    //fore_img.start_y = top; 
	fore_img.start_x =0 ; //xiongkai  point.x; //(128 - aclock_input_param.clock_dial.width)/2;   
    fore_img.start_y = 0 ; //xiongkai point.y; //top; 

	//memcpy(bk_img.data_ptr, fore_img.data_ptr, fore_img.width * fore_img.height * 3);

    if (!ProcessAlphaBlending(&bk_img, &fore_img))
    {
        //SCI_TRACE_LOW:"error: foreground image rect is not in background image rect, return directly"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_KEYLOCK_600_112_2_18_2_31_4_7,(uint8*)"");
        SCI_FREE(aclock_input_param.target_buf_ptr);   
	 aclock_input_param.target_buf_ptr = NULL;	
        return;
    }

#if 1
    //in the end, display the center point.
    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_KEYLOCK_ACLOCK_CENTER, win_id); 

    point.x = (lcd_width - width)/2;
    point.y = (lcd_width - width)/2; // top + (aclock_input_param.clock_dial.height - height)/2;

    GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      IMAGE_KEYLOCK_ACLOCK_CENTER,
                      &lcd_dev_info);
#endif

    MMITHEME_StoreUpdateRect(&lcd_dev_info,win_rect);
        
    if (aclock_input_param.target_buf_ptr != NULL)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        aclock_input_param.target_buf_ptr = NULL;
    }
#endif

    SCI_TRACE_LOW("DrawAClock() exit.");
}

PUBLIC t_watchidle_callback *CreateWatchEffect0(const sWatchPanelData *data)
{
    t_watchidle_callback *obj = SCI_ALLOC_APPZ(sizeof(t_watchidle_callback));
    if (data == NULL)
    {
        SCI_FREE(obj);
        return NULL;
    }
    SCI_ASSERT(obj != NULL);
    SCI_MEMCPY(obj, &mWatchIdle_Effect0_Param, sizeof(mWatchIdle_Effect0_Param));

    obj->m_pData = SCI_ALLOC_APPZ(sizeof(sWatchPanelData));
    SCI_ASSERT(obj->m_pData != NULL);
    
    *(obj->m_pData) = *data;
    return obj;
}

PUBLIC void DestoryWatchEffect0(t_watchidle_callback *obj)
{
    if (obj == NULL)
    {
        return;
    }

    if (obj->m_pData == NULL)
    {
        return;
    }

    SCI_FREE(obj->m_pData);
    SCI_FREE(obj);
}

////////////////////////////////////////////////////////////////////////////
t_watchidle_callback mWatchIdle_Effect0_Param = {createBackground, 
												 createClock, 
												 updateBackground, 
												 updateClock,
												 destoryBackground,
												 destoryClock,
												 0};
////////////////////////////////////////////////////////////////////////////

#endif
