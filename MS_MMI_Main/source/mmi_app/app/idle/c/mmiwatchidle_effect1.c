
#include "mmiwatchidle_effect1.h"
#include "guigraph.h"
#include "mmiidle_export.h"
#include "mmk_type.h"

#include "graphics_draw.h"

#ifdef ANALOG_CLK_SUPPORT  //yangyu add


LOCAL BOOLEAN MMIWatchIdle_CreatWallpaperCtrl();
LOCAL void DrawAClock(MMI_WIN_ID_T win_id, const void* param);

LOCAL void createBackground(void* param)
{
    //Do nothing
}

LOCAL void createClock(void* param)
{
    //Do nothing
}

LOCAL void updateBackground(void* param)
{
#if 0  //xiongkai

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

LOCAL void DrawAClock(MMI_WIN_ID_T win_id, const void *param)
{
//#define TEST_ON 1
#ifdef TEST_ON
    BOOLEAN bDrawOk = FALSE;
    GUI_POINT_T       point = {0};  
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    uint16 nImageWidth = 240;//320;
    uint16 nImageHeight = 240;//320;
    GUI_RECT_T fullScreenRect = MMITHEME_GetFullScreenRect();
    uint16 nScreenWidth =fullScreenRect.right - fullScreenRect.left + 1;
    uint16 nScreenHeight = fullScreenRect.bottom - fullScreenRect.top + 1;
    GUIRES_GetImgWidthHeight(&nImageWidth, &nImageHeight, IMAGE_WATCHIDLE_ACLOCK_PANEL, win_id);
    
    point.x = nScreenWidth - nImageWidth >> 1;
    point.y = nScreenHeight - nImageHeight >> 1;

    SCI_TRACE_LOW("lzk log.DrawAClock() enter.");
    SCI_TRACE_LOW("lzk log.DrawAClock() nScreenWidth = %d, nScreenHeight = %d.", nScreenWidth, nScreenHeight);
    SCI_TRACE_LOW("lzk log.DrawAClock() nImageWidth = %d, nImageHeight = %d.", nImageWidth, nImageHeight);
    bDrawOk = GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      IMAGE_WATCHIDLE_ACLOCK_PANEL,
                      &lcd_dev_info);

    SCI_TRACE_LOW("lzk log. DrawAClock()GUIRES_DisplayImg ret = %d.", bDrawOk);

#else
    ///////////////////
    uint32 mem_width_ptr = 0;
    uint32 dst_widget_type = 0;
    void *lcd_buffer_ptr = NULL;

     //xiongkai draw date
		uint8						datestr[16] 		= {0};
		wchar						datewstr[16]		= {0};
		SCI_DATE_T                  sys_date = {0};
		uint16						len 				= {0}; 
		GUI_FONT_T					font				= SONG_FONT_8;
		GUI_COLOR_T 				color				= 0;
		uint16						str_width			= 0;
		GUISTR_STYLE_T			text_style = {0};/*lint !e64*/
		GUISTR_STATE_T			text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
		MMI_STRING_T			text   = {0};

		SCI_TIME_T systime = {0};
		
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

    pData->mPanelImgId=IMAGE_WATCHIDLE_ACLOCK_PANEL2;

    GUIRES_GetImgWidthHeight(&nImageWidth, &nImageHeight, pData->mPanelImgId, win_id);
    
    point.x = nScreenWidth - nImageWidth >> 1;
    point.y = nScreenHeight - nImageHeight >> 1;

    SCI_TRACE_LOW("DrawAClock() enter.");
    SCI_TRACE_LOW("DrawAClock() nScreenWidth = %d, nImageHeight = %d.", nScreenWidth, nImageHeight);
    SCI_TRACE_LOW("DrawAClock() nImageWidth = %d, nImageHeight = %d.", nImageWidth, nImageHeight);
    
    //aclock dial.
    aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height,IMAGE_WATCHIDLE_ACLOCK_PANEL2/* pData->mPanelImgId */, win_id);
    aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_PANEL2 /*pData->mPanelImgId*/,win_id,&(aclock_input_param.clock_dial.data_size));
    aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
    aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2;   

    //aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, IMAGE_WATCHIDLE_ACLOCK_HOUR2, win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_HOUR2,win_id,&(aclock_input_param.hour_handle.data_size));
	aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, IMAGE_WATCHIDLE_ACLOCK_MIN2, win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_MIN2,win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height/2; 

    //aclock sec hand
    aclock_input_param.sec_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.sec_handle.width, &aclock_input_param.sec_handle.height, IMAGE_WATCHIDLE_ACLOCK_SEC2, win_id);        
    aclock_input_param.sec_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WATCHIDLE_ACLOCK_SEC2,win_id,&(aclock_input_param.sec_handle.data_size));
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
#else    //display aclock
    MMK_GetWinRect(win_id, &win_rect);

#if (defined UI_MULTILAYER_SUPPORT) && (!defined IDLE_MAINMENU_LOW_MEMORY_SUPPORT)
    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    
    bk_img.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    bk_img.data_type = layer_info.data_type; 
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

#endif

#if 0//xiongkai 	
    if (PNULL == bk_img.data_ptr)
    {
        SCI_TRACE_LOW("lzk log. DrawAClock(): PNULL == bk_img.data_ptr.");
        SCI_ASSERT(0);
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
    
    fore_img.start_x =0; // point.x; //(128 - aclock_input_param.clock_dial.width)/2;   
    fore_img.start_y = 0; // point.y; //top; 

	//memcpy(bk_img.data_ptr, fore_img.data_ptr, fore_img.width * fore_img.height * 3);

    if (!ProcessAlphaBlending(&bk_img, &fore_img))
    {
        //SCI_TRACE_LOW:"error: foreground image rect is not in background image rect, return directly"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_KEYLOCK_600_112_2_18_2_31_4_7,(uint8*)"");
        SCI_FREE(aclock_input_param.target_buf_ptr);   
	 aclock_input_param.target_buf_ptr = NULL;	
        return;
    }

#if 0
    //in the end, display the center point.
    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_KEYLOCK_ACLOCK_CENTER, win_id); 

    //point.x = (lcd_width - width)/2;
    //point.y = top + (aclock_input_param.clock_dial.height - height)/2;

    GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      IMAGE_KEYLOCK_ACLOCK_CENTER,
                      &lcd_dev_info);
#endif

#if 1
	//xiongkai draw date  and time
	{
	             TM_GetSysDate(&sys_date);
			font =SONG_FONT_24;
			color = 0xffff;
			  
			sprintf(datestr,"%02d-%02d",sys_date.mon,sys_date.mday);
		
			len = strlen((char *)datestr);
			MMI_STRNTOWSTR( datewstr, 16, datestr, 16, len );
			MMIIDLE_GetIdleTextStyle(&text_style, font, color);
		
			text.wstr_len = len;
			text.wstr_ptr = datewstr;
		
			str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
		   
			point.x=(240-str_width)>>1;
			point.y=175;
			
			//text_style.begin_alpha = text_style.end_alpha = 80;
			GUISTR_DrawTextToLCDSingleLine( 
				(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				(const GUI_POINT_T		*)&point,
				(const MMI_STRING_T 	*)&text,
				&text_style,
				text_state
				);

	             TM_GetSysTime(&systime);
			font =SONG_FONT_32;
			color = 0xffff;
			  
			sprintf(datestr,"%02d:%02d",systime.hour,systime.min);
		
			len = strlen((char *)datestr);
			MMI_STRNTOWSTR( datewstr, 16, datestr, 16, len );
			MMIIDLE_GetIdleTextStyle(&text_style, font, color);
		
			text.wstr_len = len;
			text.wstr_ptr = datewstr;
		
			str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
		   
			point.x=(240-str_width)>>1;
			point.y=45;

			//text_style.begin_alpha = text_style.end_alpha = 80;
			GUISTR_DrawTextToLCDSingleLine( 
				(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				(const GUI_POINT_T		*)&point,
				(const MMI_STRING_T 	*)&text,
				&text_style,
				text_state
				);			
	}
#endif

    MMITHEME_StoreUpdateRect(&lcd_dev_info,win_rect);

    if (aclock_input_param.target_buf_ptr != NULL)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        aclock_input_param.target_buf_ptr = NULL;
    }
    SCI_TRACE_LOW("DrawAClock() exit.");
#endif
}

PUBLIC t_watchidle_callback *CreateWatchEffect1(const sWatchPanelData *data)
{
    t_watchidle_callback *obj = SCI_ALLOC_APPZ(sizeof(t_watchidle_callback));
    if (data == NULL)
    {
        SCI_FREE(obj);
        return NULL;
    }
    SCI_ASSERT(obj != NULL);
    SCI_MEMCPY(obj, &mWatchIdle_Effect1_Param, sizeof(mWatchIdle_Effect1_Param));

    obj->m_pData = SCI_ALLOC_APPZ(sizeof(sWatchPanelData));
    SCI_ASSERT(obj->m_pData != NULL);
    
    *(obj->m_pData) = *data;
    return obj;
}

PUBLIC void DestoryWatchEffect1(t_watchidle_callback *obj)
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
t_watchidle_callback mWatchIdle_Effect1_Param = {createBackground, 
												createClock, 
												updateBackground, 
												updateClock,
												destoryBackground,
												destoryClock,
												0};

////////////////////////////////////////////////////////////////////////////

#endif
