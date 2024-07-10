/*****************************************************************************
** File Name:      mmitv_out.c  											 *
** Author:         haiyanghu                                                 *
** Date:           2006-6	                                                 *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe tv out                      *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2006        haiyanghu        Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_connection_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmitv_out.h"
#ifdef MMI_TVOUT_ENABLE
#include "mmidisplay_data.h"
#include "mmi_id.h"
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_textfun.h"
#include "mmi_text.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmi_appmsg.h"
#include "sc6800_reg.h"
#ifndef WIN32
#include "chng_freq.h"
#include "freq_cfg.h"
#endif
//#include "dal_lcd_sc6800.h"  byte.guo+++ 这个地方88是有的
#include "dal_lcd.h"
#include "guitext.h"
#include "audio_config.h"
#include "guifont.h"
#include "mmiconnection_id.h"
#include "mmiconnection_text.h"
#include "mmiconnection_image.h"
#include "mmitv_out.h"
#include "guilistbox.h"
#include "mmipub.h"
#ifndef WIN32
#include "tb_hal.h"
#endif

#include "mmifm_export.h"

#include "guiref_scale.h"
#include "mmi_theme.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiconnection_export.h"
#include "guistring.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmidc_export.h"
#include "guires.h"
#define GUIREF_TV_ROW_PIXELS        216
#define GUIREF_TV_COLUMN_PIXELS     288
#define IMAGE_CONN_TVOUT_BG_LEFT    53
#define IMAGE_CONN_TVOUT_BG_TOP     90
#ifndef _WIN32
__align(32) LOCAL GUI_COLOR_T s_tvout_rect_memory[10/*GUIREF_TV_ROW_PIXELS*GUIREF_TV_COLUMN_PIXELS*/] = {0};
__align(32) LOCAL GUI_COLOR_T s_tvout_memory1[GUI_BLOCK_MAX][10/*GUIREF_TV_ROW_PIXELS*GUIREF_TV_COLUMN_PIXELS*/] = {0};
__align(32) LOCAL GUI_COLOR_T s_tvout_memory2[GUI_BLOCK_MAX][10/*GUIREF_TV_ROW_PIXELS*GUIREF_TV_COLUMN_PIXELS*/] = {0};
#endif

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern void MMIDEFAULT_SetAlwaysHalfOnBackLight(BOOLEAN b_half_on);


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set tv out starting status
//	Global resource dependence : 
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
LOCAL void SetTvOutStartingStatus(BOOLEAN is_tvout_start);

/*****************************************************************************/
// 	Description : handle the message of tv out window
//	Global resource dependence :
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowTvOutWinMsg(
										 MMI_WIN_ID_T	win_id, 
										 MMI_MESSAGE_ID_E	msg_id, 
										 DPARAM				param
										 );

/*****************************************************************************/
// 	Description : Display TVout Remind Text
//	Global resource dependence :
//  Author:haiyang.hu
//	Note:
/*****************************************************************************/
LOCAL void DisplayTVoutRemindText(void);

/*****************************************************************************/
// 	Description : to handle set tv out mode win
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTVOutWindow(
										 MMI_WIN_ID_T       win_id, 
										 MMI_MESSAGE_ID_E   msg_id, 
										 DPARAM             param
										 );

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         WINTAB DEFINITION                                 *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(MMICONNECTION_TV_OUT_SHOW_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleShowTvOutWinMsg),   	
    WIN_ID(MMICONNECTION_TV_OUT_SHOW_ID),
	WIN_TITLE(TXT_CONNECTION_TVOUT_TITLE),
    END_WIN
};

//the window for set TV out mode
WINDOW_TABLE( MMICONNECTION_SET_TV_OUT_WIN_TAB ) = 
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetTVOutWindow ),    
    WIN_ID( MMICONNECTION_SET_TV_OUT_WIN_ID ),
    WIN_TITLE( TXT_CONNECTION_TVOUT_SETTING ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMICONNECTION_SET_TV_OUT_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),						
    END_WIN
};

/**-------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
LOCAL uint32 s_pal_start_x = 80;
LOCAL uint32 s_pal_start_y = 19;
LOCAL uint32 s_pal_end_x   = 80 + MMITVOUT_WIDTH - 1;
LOCAL uint32 s_pal_end_y   = 19 + MMITVOUT_HEIGHT - 1;

LOCAL uint32 s_ntsc_start_x = 80;
LOCAL uint32 s_ntsc_start_y = 19;
LOCAL uint32 s_ntsc_end_x   = 80 + MMITVOUT_WIDTH - 1;
LOCAL uint32 s_ntsc_end_y   = 19 + MMITVOUT_HEIGHT - 1;

LOCAL BOOLEAN s_is_tvout_started = FALSE; //TURE: tv-out have started, FALSE: not started
LOCAL uint32 s_tvout_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
/**-------------------------------------------------------------------------*
**                         EXTERN VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open tv out window
//	Global resource dependence : none
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_OpenWin(void)
{	
	MMK_CreateWin((uint32*)MMICONNECTION_TV_OUT_SHOW_TAB, (ADD_DATA)PNULL);
	return TRUE;
}

/*****************************************************************************/
// 	Description : open tv out setting window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUTSetting_OpenWin(void)
{	
	MMK_CreateWin((uint32*)MMICONNECTION_SET_TV_OUT_WIN_TAB, (ADD_DATA)PNULL);
	return TRUE;
}

/*****************************************************************************/
// 	Description : get TV status
//	Global resource dependence : 
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_GetStartingStatus() //true: TV out start
{
//	SCI_TRACE_LOW("mmitv_out.c, MMIAPITVOUT_GetStartingStatus(), s_is_tvout_started = %d.", s_is_tvout_started);
	return s_is_tvout_started;
}

/*****************************************************************************/
// 	Description : set TV out starting status
//	Global resource dependence : 
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
LOCAL void SetTvOutStartingStatus(BOOLEAN is_tvout_start)//true: TV out start
{
	s_is_tvout_started = is_tvout_start;
	return;
}

/*****************************************************************************/
// 	Description : start tv out
//	Global resource dependence : 
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_StartTvOut(void)
{
	BLOCKCFG_T cfg = {0};
       GUI_LCD_DEV_INFO        lcd_dev = {GUI_TV_ID,GUI_BLOCK_MAIN};
	MMICONNECTION_TV_SET_TYPE_E		tvout_mode = (MMICONNECTION_TV_SET_TYPE_E)MMIAPISET_GetTVOutMode();
	uint32 tv_mode_convert_table[MMICONNECTION_TV_SET_TV_OUT_MODE_MAX_NUM] = {
											TV_PAL_LINE_BY_LINE,
											TV_NTSC_LINE_BY_LINE,
 											TV_PAL_INTERLEAVING_LINE,
											TV_NTSC_INTERLEAVING_LINE
											};

	if( !MMIAPITVOUT_GetStartingStatus() )
	{
		cfg.colorkey   = MMI_BLACK_COLOR;
		cfg.width      = MMITVOUT_WIDTH;
		cfg.resolution = MMITVOUT_RESOLUTION;
		cfg.type       = MMITVOUT_CFG_TYPE; 
		cfg.priority   = MMITVOUT_CFG_PRIORITY;
		cfg.alpha      = MMITVOUT_CFG_ALPHA;

		if (MMICONNECTION_TV_SET_PAL_LINE_BY_LINE == tvout_mode || MMICONNECTION_TV_SET_PAL_INTERLEAVING_LINE == tvout_mode)
		{
			cfg.start_x    = s_pal_start_x;
			cfg.start_y    = s_pal_start_y;
			cfg.end_x      = s_pal_end_x;
			cfg.end_y      = s_pal_end_y;
		}
		else if (MMICONNECTION_TV_SET_NTSC_LINE_BY_LINE == tvout_mode || MMICONNECTION_TV_SET_NTSC_LINE_BY_LINE == tvout_mode)
		{
			cfg.start_x    = s_ntsc_start_x;
			cfg.start_y    = s_ntsc_start_y;
			cfg.end_x      = s_ntsc_end_x;
			cfg.end_y      = s_ntsc_end_y;
		}
		else
		{
			return FALSE;
		}
#ifndef _WIN32
		GUIREF_SetIsScaleDown(TRUE);
#endif
		SetTvOutStartingStatus(TRUE);
//		SCI_TRACE_LOW("mmitv_out.c, StartTvOut(), cfg.start_x=%d, cfg.start_y=%d, cfg.end_x=%d, cfg.end_y=%d", cfg.start_x, cfg.start_y, cfg.end_x, cfg.end_y);
        
#ifndef  _WIN32
        MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_TVOUT, MMIAUDIO_GetCurVolume());
        if (CHNG_FREQ_REQ_HANDLER_NULL == s_tvout_chng_freq_req_handler)
        {
            s_tvout_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("tv_out");
            //SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_tvout_chng_freq_req_handler);
        }
        CHNG_FREQ_SetArmClk(s_tvout_chng_freq_req_handler,(CHNG_FREQ_INDEX_E)FREQ_INDEX_TVOUT);
        
        GPIO_Sel_MIC_TVOut(1);
        //SCI_TRACE_LOW:"mmitv_out.c, MMIAPITVOUT_StartTvOut(), tvout_mode = %d. mode = %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITV_OUT_259_112_2_18_2_12_22_28,(uint8*)"dd", tvout_mode, tv_mode_convert_table[tvout_mode]);
        //		SCI_TRACE_LOW("mmitv_out.c, MMIAPITVOUT_StartTvOut(), cfg sx = %d. sy =%d. ex = %d. ey = %d.", cfg.start_x, cfg.start_y, cfg.end_x, cfg.end_y);
        
        TV_Open(tv_mode_convert_table[tvout_mode]);
        GUIREF_SetBlockCfgBeforeScale(GUI_TV_ID,0,&cfg);       
        GUIREF_SetBlockBufferBeforeScale(GUI_TV_ID,0,(uint32)(GUILCD_GetMainLcdBufPtr()));

		GUIBLOCK_Enable(&lcd_dev);
		GUILCD_Invalidate(FALSE,GUI_TV_ID,GUIREF_SCALE_BLOCK0);
#endif
		//set lcd back light always half on
		MMIDEFAULT_SetAlwaysHalfOnBackLight(TRUE);
//		SCI_TRACE_LOW("mmitv_out.c, StartTvOut()" );

        MMK_ReturnIdleWin();
	}
	return MMIAPITVOUT_GetStartingStatus();
}


/*****************************************************************************/
// 	Description : get the tv out size
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_GetTVOutShowRect(GUI_RECT_T * show_rect_ptr)
{
	MMICONNECTION_TV_SET_TYPE_E		tvout_mode = (MMICONNECTION_TV_SET_TYPE_E)MMIAPISET_GetTVOutMode();

    if (PNULL == show_rect_ptr)
    {
        return FALSE;
    }
	if(!MMIAPITVOUT_GetStartingStatus())
	{
		return FALSE;
	}
	
	if (MMICONNECTION_TV_SET_PAL_LINE_BY_LINE == tvout_mode || MMICONNECTION_TV_SET_NTSC_LINE_BY_LINE == tvout_mode)
	{
		show_rect_ptr->left		= s_pal_start_x;
		show_rect_ptr->top		= s_pal_start_y;
		show_rect_ptr->right	= s_pal_end_x;
		show_rect_ptr->bottom	= s_pal_end_y;
	}
	else if (MMICONNECTION_TV_SET_NTSC_LINE_BY_LINE == tvout_mode || MMICONNECTION_TV_SET_NTSC_INTERLEAVING_LINE == tvout_mode)
	{
		show_rect_ptr->left		= s_ntsc_start_x;
		show_rect_ptr->top		= s_ntsc_start_y;
		show_rect_ptr->right	= s_ntsc_end_x;
		show_rect_ptr->bottom	= s_ntsc_end_y;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

/*****************************************************************************/
// 	Description : set the tv out size
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_SetTVOutShowRect(GUI_RECT_T * show_rect_ptr)
{
	MMICONNECTION_TV_SET_TYPE_E		tvout_mode = (MMICONNECTION_TV_SET_TYPE_E)MMIAPISET_GetTVOutMode();

    if (PNULL == show_rect_ptr)
    {
        return FALSE;
    }
	if(!MMIAPITVOUT_GetStartingStatus())
	{
		return FALSE;
	}
	
	if (MMICONNECTION_TV_SET_PAL_LINE_BY_LINE == tvout_mode || MMICONNECTION_TV_SET_NTSC_LINE_BY_LINE == tvout_mode)
	{
		s_pal_start_x = show_rect_ptr->left;
		s_pal_start_y = show_rect_ptr->top;
		s_pal_end_x = show_rect_ptr->right;
		s_pal_end_y = show_rect_ptr->bottom;
	}
	else if (MMICONNECTION_TV_SET_NTSC_LINE_BY_LINE == tvout_mode || MMICONNECTION_TV_SET_NTSC_INTERLEAVING_LINE == tvout_mode)
	{
		s_ntsc_start_x = show_rect_ptr->left;
		s_ntsc_start_y = show_rect_ptr->top;
		s_ntsc_end_x = show_rect_ptr->right;
		s_ntsc_end_y = show_rect_ptr->bottom;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
/*****************************************************************************/
// 	Description : close tv out
//	Global resource dependence : 
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITVOUT_Close()
{
    AUDIO_DEVICE_MODE_TYPE_E      mode = AUDIO_DEVICE_MODE_HANDHOLD;

	if( MMIAPITVOUT_GetStartingStatus() )
	{	
		/** if mpeg4 is open, exit from it **/

#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
		MMIAPIVP_ExitVideoPlayer();    
#endif
#endif

#ifdef MOILE_VIDEO_SUPPORT
        MMIAPIMV_Exit();
#endif

		/** if dc is open, exit from it **/
		MMIAPIDC_Exit();
				
		/** if idle dial is open, exit from it **/
		if (MMK_IsOpenWin(MMIIDLE_DIAL_WIN_ID))
		{
			MMK_SendMsg(MMIIDLE_DIAL_WIN_ID, MSG_TVOUT_CLOSE_IND, PNULL);
		}

		SetTvOutStartingStatus(FALSE);

#ifndef  _WIN32
		GPIO_Sel_MIC_TVOut(0);
		TV_Close();
        CHNG_FREQ_RestoreARMClk(s_tvout_chng_freq_req_handler);
#else
		MMK_CloseWin(MMICONNECTION_TV_OUT_SHOW_ID);
#endif

		//refresh LCD
		MMK_PostMsg(
			MMK_GetFocusWinId(),
			MSG_FULL_PAINT,
			(DPARAM)PNULL,
			0
			);
		
		//set audio mode
		//AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
		MMIAPISET_GetDeviceMode(&mode);

            MMIAUDIO_SetVolume(mode, MMIAUDIO_GetCurVolume());

		//SCI_TRACE_LOW:"mmitv_out.c, MMIAPITVOUT_Close()"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITV_OUT_406_112_2_18_2_12_22_29,(uint8*)"" );
		MMIAPICONNECTION_CloseMenuWin();
		//turn on back light
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
	}

	//SCI_TRACE_LOW:"mmitv_out.c, MMIAPITVOUT_Close(), not close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITV_OUT_412_112_2_18_2_12_22_30,(uint8*)"" );
	return;
}

/*****************************************************************************/
// 	Description : handle the message of tv out window
//	Global resource dependence :
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowTvOutWinMsg(
										 MMI_WIN_ID_T	win_id, 
										 MMI_MESSAGE_ID_E	msg_id, 
										 DPARAM				param
										 )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          bg_rect = MMITHEME_GetClientRect();
    GUI_POINT_T         dis_point = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL,
            &bg_rect,
            &bg_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);

        dis_point.x = IMAGE_CONN_TVOUT_BG_LEFT;
        dis_point.y = IMAGE_CONN_TVOUT_BG_TOP;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_CONNECTION_TVOUT_BACKGROUND,
            &lcd_dev_info);
		GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, TXT_NULL, FALSE);

		DisplayTVoutRemindText();

		MMK_PostMsg(
			MMICONNECTION_TV_OUT_SHOW_ID,
			MSG_TVOUT_STARTING,
			(DPARAM)PNULL,
			0
			);
		break;

	case MSG_TVOUT_STARTING:
        //关闭FM
        MMIAPIFM_Stop();

		MMIAPITVOUT_StartTvOut();

#ifndef _WIN32
		MMK_CloseWin(MMICONNECTION_TV_OUT_SHOW_ID);
#endif
		break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : Display TVout Remind Text
//	Global resource dependence :
//  Author:haiyang.hu
//	Note: modified by ryan.xu
/*****************************************************************************/
LOCAL void DisplayTVoutRemindText(void)
{
	MMI_STRING_T        disp_data = {0};
	GUI_LCD_DEV_INFO    dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T          remind_text_box = {MMITVOUT_REMINDER_TEXT_POS_LEFT, MMITVOUT_REMINDER_TEXT_POS_TOP,MMITVOUT_REMINDER_TEXT_POS_RIGHT,MMITVOUT_REMINDER_TEXT_POS_BUTTOM};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 

	MMI_GetLabelTextByLang(
        TXT_CONNECTION_TVOUT_REMIND,
        &disp_data
        );
	
    text_style.font = MMI_DEFAULT_TEXT_FONT;
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.align = ALIGN_LEFT;
    text_style.char_space = 0;
    text_style.line_space = 1;
    
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info,
        (const GUI_RECT_T      *)&remind_text_box,       //the fixed display area
        (const GUI_RECT_T      *)&remind_text_box,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&disp_data,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
}

/*****************************************************************************/
// 	Description : to handle set tv out mode win
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetTVOutWindow(
										 MMI_WIN_ID_T       win_id, 
										 MMI_MESSAGE_ID_E   msg_id, 
										 DPARAM             param
										 )
{
	MMI_RESULT_E   recode = MMI_RESULT_TRUE;
	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMISET_TVOUT_INTERLEAVING_MODE
        GUILIST_SetMaxItem(MMICONNECTION_SET_TV_OUT_CTRL_ID, MMISET_TV_OUT_MODE_MAX_NUM, FALSE );
#else
        GUILIST_SetMaxItem(MMICONNECTION_SET_TV_OUT_CTRL_ID, MMICONNECTION_TV_SET_PAL_INTERLEAVING_LINE, FALSE );
#endif

		MMIAPISET_AppendTVOutModeListItem(MMICONNECTION_SET_TV_OUT_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMICONNECTION_SET_TV_OUT_CTRL_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:  
		MMIAPISET_SetTVOutMmode(MMICONNECTION_SET_TV_OUT_CTRL_ID);
		//success!
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin( win_id );  
		break;

    case MSG_CTL_CANCEL:
		MMK_CloseWin( win_id );      
		break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
// 	Description : init tv out
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITVOUT_Init(void)
{
//#ifdef MMI_TVOUT_ENABLE
    GUIREF_RegisterTvOutForApp(GUIREF_TV_ROW_PIXELS, GUIREF_TV_COLUMN_PIXELS, (GUI_COLOR_T *)s_tvout_rect_memory, (GUI_COLOR_T **)s_tvout_memory1, (GUI_COLOR_T **)s_tvout_memory2);
//#else
//    GUIREF_UnregisterTvOutForApp();
//#endif
}


#else   //MMI_TVOUT_ENABLE is not defined
PUBLIC void MMIAPITVOUT_Close()  {}
PUBLIC BOOLEAN MMIAPITVOUT_GetStartingStatus() {return FALSE;}
PUBLIC BOOLEAN MMIAPITVOUT_GetTVOutShowRect(GUI_RECT_T * show_rect_ptr) {return FALSE;}
PUBLIC void MMIAPITVOUT_Init(void) {}
PUBLIC BOOLEAN MMIAPITVOUT_SetTVOutShowRect(GUI_RECT_T * show_rect_ptr) {return FALSE;}
PUBLIC BOOLEAN MMIAPITVOUT_OpenWin(void)  {return FALSE;}
PUBLIC BOOLEAN MMIAPITVOUTSetting_OpenWin(void) {return FALSE;}
PUBLIC BOOLEAN MMIAPITVOUT_StartTvOut(void) {return FALSE;}

#endif  //MMI_TVOUT_ENABLE



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
