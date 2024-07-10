/*****************************************************************************
** File Name:      mmi_screenclose_special.c                                     *
** Author:                                                                   *
** Date:           08/23/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe special screenclose         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/23/2011     yaye.jiang             Creat
*****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_screensaver_trc.h"
#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT 
#include "window_parse.h"
#include "mmidisplay_data.h"

#include "mmiss_screenclose_special.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "mmi_theme.h"

#include "special_effect.h"
#include "guiownerdraw.h"
#include "mmikl_export.h"
#include "lcd_backlight.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
 typedef struct
 {
     SE_HANDLE se_handle;
     MMI_HANDLE_T win_handle;
     GUI_COLOR_T *src_buf;
     GUI_COLOR_T *target_buf;
     uint16 width;
     uint16 height;
    // uint8 timer;
 }SCREENCLOSE_SPEC_PARAM_T;

#define SCR_SIZE    ((MMI_MAINSCREEN_HEIGHT*MMI_MAINSCREEN_WIDTH)*sizeof(uint16))
#define MMI_BACK_LIGHT_HALF_ON      9//50 //50%
//#define CRT_TIMER 50  /* 刷屏定时器时间*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/
SCREENCLOSE_SPEC_PARAM_T s_crt_spec_param = {0};
/*
uint32 times = 0;
uint32 crt_start_time = 0;
uint32 crt_end_time = 0;
uint32 crt_last_start_time = 0;
*/

/*****************************************************************************/
//  Description : set init 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL void SetInitScreenCloseSpec(MMI_WIN_ID_T    win_id);

/*****************************************************************************/
//  Description : draw the img to buffer
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void CRTScreenCloseDisp(void);//GUIOWNDRAW_INFO_T *owndraw_info_ptr);

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CRTScreenCloseInit(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : crt播放开始
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void CRTScreenClosePlay(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 播放结束
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void CRTScreenCloseDestroy(void);

/*****************************************************************************/
//  Description : 定时器响应
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
//LOCAL void CRTScreenCloseCallback(void);

/*****************************************************************************/
//  Description : CRT spec algorithm action;
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SE_Render_CRT(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : handle event for the screen close spec
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleScreenCloseSpec(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);




WINDOW_TABLE(SCREEN_CLOSE_SPEC_TAB)=
{
    WIN_FUNC((uint32) HandleScreenCloseSpec),    
    WIN_ID(SCREEN_CLOSE_SPEC_WIN_ID),

	WIN_HIDE_STATUS,
	
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    END_WIN
};



/*****************************************************************************/
//  Description : public interface for screen_close_spec
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC void MMI_CreatScreenCloseWin(void)
{
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_SCREENCLOSE);
#endif
    CRTScreenCloseInit(SCREEN_CLOSE_SPEC_WIN_ID);
    MMK_CreateWin((uint32*)SCREEN_CLOSE_SPEC_TAB, PNULL);
}

/*****************************************************************************/
//  Description : set init 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL void SetInitScreenCloseSpec(MMI_WIN_ID_T    win_id)
{	
    s_crt_spec_param.win_handle = win_id;
    //CRTScreenCloseDisp();//(TRUE);
}
                                         
/*****************************************************************************/
//  Description : draw the img to buffer
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void CRTScreenCloseDisp(void)//GUIOWNDRAW_INFO_T *owndraw_info_ptr)
{    
    uint8            *layer_buf_ptr = PNULL;
    GUI_RECT_T       rect           = {0};
    uint16           width          = 0;
    uint16           height         = 0;
    GUI_LCD_DEV_INFO lcd_dev_info   = {0};
    //GUI_POINT_T      start_point    = {0,0};
	
    if(s_crt_spec_param.se_handle > 0)
    {

        GUILCD_GetLogicWidthHeight( GUI_MAIN_LCD_ID, &width, &height );
     
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);

        //UILAYER_Clear(&lcd_dev_info);

        if(s_crt_spec_param.target_buf != PNULL && layer_buf_ptr != PNULL)
        {
            rect = MMITHEME_GetFullScreenRect();
            
            //if(!is_first_time)
            {
                SCI_MEMCPY((void*)(layer_buf_ptr), 
                           (void*)(s_crt_spec_param.target_buf), SCR_SIZE);
            }
            /*else
            {
                SCI_MEMCPY((void*)(layer_buf_ptr), 
                           (void*)(s_crt_spec_param.src_buf), SCR_SIZE);
            }*/
        }

        GUILCD_Invalidate(GUI_MAIN_LCD_ID ,  GUI_BLOCK_MAIN);
    }
    else
    {
        layer_buf_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
        if (PNULL != layer_buf_ptr)
        {
            SCI_MEMSET((void*)(layer_buf_ptr), 0, SCR_SIZE);
            //UILAYER_Clear(&lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : crt播放开始
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void CRTScreenClosePlay(MMI_WIN_ID_T win_id)
{
	if(s_crt_spec_param.se_handle > 0)
    {
        //s_crt_spec_param.timer = MMK_CreateWinTimer(s_crt_spec_param.win_handle, CRT_TIMER, FALSE);
		SE_Render_CRT(win_id);
    }
    else 
    {
		//防止内存被占领，否则会反复申请内存，导致内存泄漏
		//CRTScreenCloseDestroy();
		MMI_CloseCRTDispWin(win_id);
		//CRTScreenCloseInit();//(win_handle);
    }
}

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CRTScreenCloseInit(MMI_WIN_ID_T win_id)
{
    uint8               *bg_ptr = PNULL;
	uint8				*temp	= PNULL;
    //uint32              bg_size = 0;
    SE_CRT_INIT_PARAM_T init    = {0};
    //GUI_POINT_T         imagerect_bg ={0};

    //GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    if(0 == s_crt_spec_param.se_handle)
    {	
		//防止无法分碰到内存
		s_crt_spec_param.target_buf    = SCI_ALLOCA(SCR_SIZE);
		if (PNULL == s_crt_spec_param.target_buf)
		{
			MMI_CloseCRTDispWin(win_id);
			return FALSE;
		}
        s_crt_spec_param.src_buf       = SCI_ALLOCA(SCR_SIZE + 256); 
		if (PNULL == s_crt_spec_param.src_buf)
		{
			MMI_CloseCRTDispWin(win_id);
			return FALSE;
		}

        bg_ptr = (uint8*)GUILCD_GetMainLcdBufPtr();
		temp = (uint8*)(((uint32)s_crt_spec_param.src_buf + 256) & ~0xFF);
        SCI_MEMCPY((void*)(temp), bg_ptr, SCR_SIZE);
        if(s_crt_spec_param.target_buf != PNULL)
        {
        
            //SCI_MEMSET(s_crt_spec_param.target_buf, 0, SCR_SIZE);
			
            GUILCD_GetLogicWidthHeight( GUI_MAIN_LCD_ID, &init.width, &init.height );
                        
        	init.src_data_format = IMGREF_FORMAT_RGB565;
        	init.src_pixels = temp;
        	init.target_data_format = init.src_data_format;
        	init.target_pixels = s_crt_spec_param.target_buf;

        	s_crt_spec_param.se_handle = SE_Create(SE_ID_CRT,&init);
        	s_crt_spec_param.width     = init.width;
        	s_crt_spec_param.height    = init.height;
            //s_crt_spec_param.timer     = 0;
            
        	if(s_crt_spec_param.se_handle > 0)
        	{
        	    return TRUE;
        	}
        }
    }
    return FALSE;
}



/*****************************************************************************/
//  Description : 播放结束
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void CRTScreenCloseDestroy(void)
{
    //uint16 i = 0;
   
    if(s_crt_spec_param.se_handle > 0)
    {
        SE_Destory(s_crt_spec_param.se_handle);//销毁特效句柄
        s_crt_spec_param.se_handle  = 0;
        s_crt_spec_param.win_handle = 0;
        s_crt_spec_param.width      = 0;
        s_crt_spec_param.height     = 0;
	}
 //	无论如何一定要释放内存
        if(PNULL !=  s_crt_spec_param.src_buf)
        {
            SCI_FREE(s_crt_spec_param.src_buf);
            s_crt_spec_param.src_buf	= PNULL;
        }
        if(PNULL !=  s_crt_spec_param.target_buf)
        {
            SCI_FREE(s_crt_spec_param.target_buf);
            s_crt_spec_param.target_buf = PNULL;
        }
    
    
   // MMK_StopTimer(s_crt_spec_param.timer);
   // s_crt_spec_param.timer      = 0;
}

/*****************************************************************************/
//  Description : 定时器响应
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
/*
LOCAL void CRTScreenCloseCallback(void)
{
    SE_RESULT_E result = SE_RESULT_SUCCESS;
    
	s_crt_spec_param.timer = MMK_CreateWinTimer(s_crt_spec_param.win_handle, CRT_TIMER, FALSE);
    result = SE_Render_CRT();

    if (SE_RESULT_SUCCESS == result)
    {
        //MMK_SendMsg(s_crt_spec_param.win_handle, MSG_FULL_PAINT, PNULL);
        CRTScreenCloseDisp();//(FALSE);
    }
    else
    {	
		MMK_StopTimer(s_crt_spec_param.timer);
        CRTScreenCloseDestroy();
    }
}
*/

/*****************************************************************************/
//  Description : CRT spec algorithm action;
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SE_Render_CRT(MMI_WIN_ID_T win_id)
{
    SE_RESULT_E result = SE_RESULT_SUCCESS;
    SE_CRT_RENDER_OUTPUT_PARAM_T output = {0};

    if (0 < s_crt_spec_param.se_handle)
    {
		/*	times ++;
			//crt_last_start_time = crt_start_time;
			crt_start_time = SCI_GetTickCount();
		*/
        UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
		while (SE_RESULT_SUCCESS == result && SE_CRT_HORIZONTAL_OVER != output.is_over)
		{ 
			/*
			times ++;
			crt_last_start_time = crt_start_time;
			crt_start_time = SCI_GetTickCount();*/

			result = SE_Render(s_crt_spec_param.se_handle, NULL, &output);
			//在变成直线时，半暗背景光
			if (SE_CRT_VERTICAL_OVER == output.is_over)
			{
				LCD_SetBackLightBrightness(MMI_BACK_LIGHT_HALF_ON);
			}
			/*
			crt_end_time = SCI_GetTickCount();
			//SCI_TRACE_LOW:"CRT Render:The %d  time's CRT full Render start time is: %d; end time is: %d;loop time is %d;"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SCREENCLOSE_SPECIAL_363_112_2_18_2_49_45_0,(uint8*)"dddd", times,crt_start_time, crt_end_time,(crt_end_time -crt_start_time));
			*/

			CRTScreenCloseDisp();
		}
        UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
			/*
			crt_end_time = SCI_GetTickCount();
			//SCI_TRACE_LOW:"CRT Render:The %d  time's CRT full Render start time is: %d; end time is: %d;loop time is %d;"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SCREENCLOSE_SPECIAL_371_112_2_18_2_49_45_1,(uint8*)"dddd", times,crt_start_time, crt_end_time,(crt_end_time -crt_start_time));
			*/
            //CRTScreenCloseDestroy();
    	    MMI_CloseCRTDispWin(win_id);   
	}
	else
    {
		//防止内存被占领，否则会反复申请内存，导致内存泄漏
		//CRTScreenCloseDestroy();
		MMI_CloseCRTDispWin(win_id);
		//CRTScreenCloseInit();//(win_handle);
    }
    
    return result;  
}

/*****************************************************************************/
//  Description : handle event for the screen close spec
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleScreenCloseSpec(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    //GUI_POINT_T     point  = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_PlayKeyLockRing();
#endif
			SetInitScreenCloseSpec(win_id);
			CRTScreenClosePlay(win_id);
            break;
            
        case MSG_FULL_PAINT:
            break;

		case MSG_LOSE_FOCUS:
			MMI_CloseCRTDispWin(win_id);
			break;

		default:
		    recode = MMI_RESULT_FALSE;
			break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : close the display window and sleep
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMI_CloseCRTDispWin(MMI_WIN_ID_T win_id)
{   
    //uint32 win_id = 0;
    
    //win_id  = MMK_GetFocusWinId();
	CRTScreenCloseDestroy();   
	MMIDEFAULT_CloseAllLight();
	MMK_CloseWin(win_id);
#ifdef UI_P3D_SUPPORT
    MMI_Enable3DEffect(MMI_3D_EFFECT_SCREENCLOSE);
#endif	
}

#endif
