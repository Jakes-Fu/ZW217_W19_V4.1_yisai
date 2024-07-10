/****************************************************************************
** File Name:      mmisrvaud_test.c                                                              *
** Author:          Yintang.ren                                                               *
** Date:             25/05/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is to test mmisrvaud module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 25/05/2011       Yintang.ren         Create
** 
****************************************************************************/
#ifdef MMISRV_AUDIO_TEST_SUPPORT
#include "mmk_type.h"
#include "guilcd.h"
#include "guistring.h"
#include "mmidisplay_data.h"
#include "mmisrv.h"
#include "mmisrvaud.h"
#include "mmisrvaud_api.h"
#include "mmi_ring.h"
#include "mmi_modu_main.h"

#ifndef WSTRLEN
#include "mmi_string.h"
#define WSTRLEN		MMIAPICOM_Wstrlen
#endif

#ifndef WSTRCPY
#define WSTRCPY		MMIAPICOM_Wstrcpy
#endif

#define INIT_AUD_REQ_PARAM(req, pri)     \
    {   \
        req.ind_data = 0;   \
        req.is_async = FALSE;   \
        req.is_auto_resume_off = FALSE; \
        req.notify = PNULL;  \
        req.pri = pri;  \
    }


#define INIT_AUD_SRV_PARAM(srv, is_mixing_enable, volume) \
    {   \
        srv.duation = 0;  \
        srv.eq_mode = 0;  \
        srv.is_mixing_enable = is_mixing_enable; \
        srv.play_times = 1;   \
        srv.all_support_route = MMISRVAUD_ROUTE_NONE; \
        srv.speed = MMISRVAUD_PLAY_RATE_NORMAL;   \
        srv.volume = volume;    \
    }

#define RECORD_BUF_LEN (1024 * 300)

LOCAL BOOLEAN s_test_enable = FALSE;
LOCAL uint8 *s_record_buf;



/*****************************************************************************/
//  Description:  Set test if is enable.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_TestSetEnable(BOOLEAN is_enable)
{
    s_test_enable = is_enable;
}

/*****************************************************************************/
//  Description:  Draw play test back ground.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_TestDrawBg(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0, 0, 0, 0};
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    GUI_POINT_T point = {0};
    MMI_STRING_T disp_str = {0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN | GUISTR_STATE_UNDERLINE;
    wchar str_buf[20] = {0};
    uint16 y_step = 0;
    //uint16 index = 0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);	
    rect.right = lcd_width - 1;
    rect.bottom = lcd_height - 1;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    LCD_DrawThickLineEx(&lcd_dev_info, lcd_width/2, 0, lcd_width/2, lcd_height, MMI_WHITE_COLOR, 1, GUILCD_PEN_STYLE_DEFAULT);

    y_step = lcd_height / 16;
    
    text_style.align = ALIGN_LEFT;
#ifdef MAINLCD_SIZE_128X160
	text_style.font = SONG_FONT_10;
#else
    text_style.font = SONG_FONT_16;
#endif
    text_style.char_space = 0;
    text_style.line_space = 0;
    text_style.font_color = MMI_RED_COLOR;                

    disp_str.wstr_ptr = str_buf;
    
    WSTRCPY(str_buf, L"1.ring file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );

    point.y += y_step;
    WSTRCPY(str_buf, L"2.ring buf");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;            
    WSTRCPY(str_buf, L"3.ring MT");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"4.record file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"5.record buf");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"6.tone");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"7.voice");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"8.FM");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"9.streaming");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"0.Alarm");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"U.ring file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"D.ring file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );
    
    point.y += y_step;
    WSTRCPY(str_buf, L"L.ring file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );

    point.y += y_step;
    WSTRCPY(str_buf, L"R.ring file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );

    point.y += y_step;
    WSTRCPY(str_buf, L"*.ring file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );

    point.y += y_step;
    WSTRCPY(str_buf, L"#.ring file");
    disp_str.wstr_len = WSTRLEN(disp_str.wstr_ptr);
    GUISTR_DrawTextToLCDSingleLine(
                    &lcd_dev_info,
                    &point,
                    &disp_str,
                    &text_style,
                    state
                    );

}

/*****************************************************************************/
//  Description:  Handle key msg.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISRVAUD_TestHandleKey(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#define DO_IF(i)    \
        if(test_arr[i].flag)    \
        {   \
            if(MMISRVMGR_Free(test_arr[i].handle))  \
            {   \
                test_arr[i].flag = FALSE;   \
                test_arr[i].handle = 0; \
            }   
#define DO_IF_END(i)   \
        }
#define DO_ELSE(i)  \
        else    \
        {
#define DO_ELSE_END(i)  \
            handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv); \
            if(handle > 0)  \
            {   \
                MMISRVAUD_Play(handle, 0);  \
                test_arr[i].handle = handle;    \
                test_arr[i].flag = TRUE;    \
            }   \
        }
    typedef struct
    {
        BOOLEAN flag;
        MMISRV_HANDLE_T handle;
    }AUD_TEST_T;


    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    //BOOLEAN res = FALSE;
    static AUD_TEST_T test_arr[16]  = {FALSE, 0};   
    uint32 idx = 0;
    uint32 i = 0;
    const wchar name[30] = L"d:\\audio\\test.mp3";
    //const wchar name[30] = L"d:\\audio\\test.mp3";
    
    req.ind_data = 0;   
    req.is_async = FALSE;   
    req.is_auto_resume_off = FALSE; 
    req.notify = PNULL;  
    req.pri = MMISRVAUD_PRI_NORMAL;  

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
    audio_srv.speed = MMISRVAUD_PLAY_RATE_NORMAL;
    audio_srv.volume = 5;
   
    switch (msg_id) 
    {
    case MSG_KEYDOWN_MENU:
    case MSG_KEYDOWN_1:     /* ring file */
#if 0        
        idx = 0;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)
            audio_srv.info.type = MMISRVAUD_TYPE_RING_FILE;
            audio_srv.info.ring_file.fmt = MMISRVAUD_RING_FMT_MP3;
            audio_srv.info.ring_file.name = (wchar *)name;
            audio_srv.info.ring_file.name_len = WSTRLEN(audio_srv.info.ring_file.name);
        DO_ELSE_END(idx)
        break;
#else
    {
        MMIAUD_RING_DATA_INFO_T ring_data = {0};
        MMI_GetRingInfo(R_CALL_1 + 1, &ring_data);
        idx = 0;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)
            audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
            audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MIDI;
            audio_srv.info.ring_buf.data = ring_data.data_ptr;
            audio_srv.info.ring_buf.data_len = ring_data.data_len;
        DO_ELSE_END(idx)
        break;
    }
#endif
    //case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_2:     /* ring buf */
    {
        MMIAUD_RING_DATA_INFO_T ring_data = {0};
        MMI_GetRingInfo(R_CALL_1, &ring_data);
        idx = 1;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)
            audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
            audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MIDI;
            audio_srv.info.ring_buf.data = ring_data.data_ptr;
            audio_srv.info.ring_buf.data_len = ring_data.data_len;
        DO_ELSE_END(idx)
        break;
    }
    case MSG_KEYDOWN_DOWNSIDE:        
    case MSG_KEYDOWN_3:     /* incoming call */
    {
        MMIAUD_RING_DATA_INFO_T ring_data = {0};
        MMI_GetRingInfo(R_CALL_1 + 1, &ring_data);
        idx = 2;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)
            audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
            audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MIDI;
            audio_srv.info.ring_buf.data = ring_data.data_ptr;
            audio_srv.info.ring_buf.data_len = ring_data.data_len;
            req.pri = MMISRVAUD_PRI_HIGH; //priority is higher than other.
        DO_ELSE_END(idx)
        break;
    }
    case MSG_KEYDOWN_4:     /* record to file */
        idx = 3;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)
            audio_srv.info.type = MMISRVAUD_TYPE_RECORD_FILE;
            audio_srv.info.record_file.fmt = MMISRVAUD_RECORD_FMT_ADPCM;
            audio_srv.info.record_file.file_handle = 0;
            audio_srv.info.record_file.name = (wchar *)name;
            audio_srv.info.record_file.name_len = WSTRLEN(audio_srv.info.record_file.name);
            audio_srv.info.record_file.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
            audio_srv.info.record_file.frame_len = MMISRVAUD_RECORD_FRAME_LEN_20;
        DO_ELSE_END(idx)        
        break;
    case MSG_KEYDOWN_5:     /* record to buf */
        idx = 4;
        DO_IF(idx)
            if(s_record_buf != PNULL)
            {
                SCI_FREE(s_record_buf);
                s_record_buf = PNULL;
            }
        DO_IF_END(idx)
        DO_ELSE(idx)
            if(s_record_buf != PNULL)
            {
                SCI_FREE(s_record_buf);
            }
            s_record_buf = SCI_ALLOC_APP(RECORD_BUF_LEN);
            if(PNULL == s_record_buf)
            {
                //_DBG(0);
                return result;
            }
            audio_srv.info.record_buf.data = s_record_buf;
            audio_srv.info.type = MMISRVAUD_TYPE_RECORD_BUF;
            audio_srv.info.record_buf.fmt = MMISRVAUD_RECORD_FMT_ADPCM;
            audio_srv.info.record_buf.data_len = RECORD_BUF_LEN;
            audio_srv.info.record_buf.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
            audio_srv.info.record_buf.frame_len = MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;
        DO_ELSE_END(idx)        
        break;
    case MSG_KEYDOWN_6:         /* play tone */
        idx = 5;
        //MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, AUD_GENERIC_WARNING_TONE, 2000, MMIAUDIO_TONE_DEFAULT_FREQ);
        break;
    case MSG_KEYDOWN_7:          /* voice */
        idx = 6;
        #if 0  
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)

        DO_ELSE_END(idx)        
        #endif
        break;
    case MSG_KEYDOWN_8:         /* FM */
        idx = 7;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)
            audio_srv.info.fm.type = MMISRVAUD_TYPE_FM;
        DO_ELSE_END(idx)        
        break;
    case MSG_KEYDOWN_9:        /* streaming */
        idx = 8;
        #if 0   
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)

        DO_ELSE_END(idx)        
        #endif
        break;
    case MSG_KEYDOWN_0:        /* alarm */ 
    {
        MMIAUD_RING_DATA_INFO_T ring_data = {0};
        MMI_GetRingInfo(R_CALL_1 + 2, &ring_data);        
        idx = 9;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)
            audio_srv.duation = 20*1000;
            audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;

            audio_srv.info.type = MMISRVAUD_TYPE_RING_BUF;
            audio_srv.info.ring_buf.fmt = MMISRVAUD_RING_FMT_MIDI;
            audio_srv.info.ring_buf.data = ring_data.data_ptr;
            audio_srv.info.ring_buf.data_len = ring_data.data_len;
        DO_ELSE_END(idx)        
        break;
    }
    case MSG_KEYDOWN_UP:        /* volume inc */ 
        idx = 10;
        //MMISRVAUD_SetVolumeNoHandle(8);
        break;
    case MSG_KEYDOWN_DOWN:    /* volume dec */ 
        idx = 10;
        //MMISRVAUD_SetVolumeNoHandle(1);
        break;
    case MSG_KEYDOWN_LEFT: 
        idx = 12;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)

        DO_ELSE_END(idx)        
        break;
    case MSG_KEYDOWN_RIGHT : 
        idx = 13;
        DO_IF(idx)
        DO_IF_END(idx)
        DO_ELSE(idx)

        DO_ELSE_END(idx)        
        break;
    case MSG_KEYDOWN_STAR  : 
        idx = 14;
        break;
    case MSG_KEYDOWN_HASH  :             
        idx = 15;
        break;
    case MSG_CLOSE_WINDOW:
        for(i = 0; i < 16; i++)
        {
            if(test_arr[i].handle != 0)
            {
                MMISRVMGR_Free(test_arr[i].handle);
            }
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);     
}

#endif
