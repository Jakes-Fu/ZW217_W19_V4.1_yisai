/****************************************************************************
** File Name:      mmiphsapp_wintab.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef _MMIZDT_YX_WINTAB_C_
#define _MMIZDT_YX_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_common.h"

#include "zdt_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"
#include "img_dec_interface.h"
#include "guiownerdraw.h"
#include "graphics_draw.h"
#include "mmiparse_export.h"
#include "mmicl_export.h"
#ifdef ENG_SUPPORT
#include "mmieng_export.h"
#endif
#include "mmiset_export.h"
#include "mmiset_id.h"
#include "qr_encode.h"
#include "zdthttp_api.h"

#define SCALE  1

BOOLEAN MMIZDT_Open2VMWin(void);
extern void GT_Qcode_show(const unsigned char *QR_TEST_STR,int x0,int y0,int w0,int h0);
PUBLIC BOOLEAN MMIZDT_IsInClassModeWin();

#define z_abs(x)  ((x) >= 0 ? (x) : (-(x)))

LOCAL THEMELIST_ITEM_STYLE_T new_style = {0};

#if 0
LOCAL MMI_RESULT_E  HandleZDT_FindWatchWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );


WINDOW_TABLE( MMIZDT_FIND_WATCH_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_FindWatchWinMsg),    
    WIN_ID( MMIZDT_FIND_WATCH_WIN_ID),
    CREATE_ANIM_CTRL(MMIZDT_FINDWATCH_ANIM_CTRL_ID, MMIZDT_FIND_WATCH_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC BOOLEAN MMIZDT_FindWatchWin(void)
{
    if(FALSE == MMK_IsOpenWin(MMIZDT_FIND_WATCH_WIN_ID))
    {
#ifndef WIN32    
#ifdef LEBAO_MUSIC_SUPPORT
        lebao_exit();
#endif    
#endif
        MMK_CreateWin((uint32*)MMIZDT_FIND_WATCH_WIN_TAB,PNULL);
    }
    return TRUE;
}


LOCAL GUIANIM_RESULT_E SetFindWatchAnimParam(
    BOOLEAN          is_update,  //是否立即刷新
    MMI_CTRL_ID_T    ctrl_id
)
{
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_CALL_BACK_T     call_back = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_DATA_INFO_T data_info={0};

    //set anim min period
    GUIANIM_SetPeriod(200, ctrl_id);

    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id    = ctrl_id;

    

    display_info.is_zoom        = FALSE;


	

	if(MMIZDT_FINDWATCH_ANIM_CTRL_ID == ctrl_id)
	{
		display_info.is_disp_one_frame = FALSE;
	}
	else
	{
		display_info.is_disp_one_frame = TRUE;
	}
	display_info.is_play_once = FALSE;
    display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update      = is_update;
    display_info.bg.bg_type     = GUI_BG_COLOR;

    //背景色设为黑色
    display_info.bg.color       = MMI_BLACK_COLOR;//MMI_WINDOW_BACKGROUND_COLOR;

    
    data_info.img_id  = IMAGE_FIND_WATCH;
    anim_result = GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);


    return (anim_result);
}



LOCAL MMI_RESULT_E  HandleZDT_FindWatchWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_BG_T bg_ptr = {0};
    GUI_BG_DISPLAY_T bg_display = {0};
    MMI_CTRL_ID_T anim_ctrl_id = MMIZDT_FINDWATCH_ANIM_CTRL_ID;
    const GUI_LCD_DEV_INFO *lcd_info = MMITHEME_GetDefaultLcdDev();
    //GUI_POINT_T      point = {94,185};
    GUI_BOTH_RECT_T     both_rect = MMITHEME_GetFullScreenBothRect();
   
    
    BOOLEAN ret = FALSE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(anim_ctrl_id, &both_rect);
        SetFindWatchAnimParam(TRUE, anim_ctrl_id);



        
        break;
        
    case MSG_FULL_PAINT:
        {
        /*GUISTR_STYLE_T      text_style      = {0};
        GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;

        GUI_POINT_T start_point = {5, 100};
        GUI_RECT_T rect = {0,0,(MMI_MAINSCREEN_WIDTH -1),(MMI_MAINSCREEN_WIDTH -1)};

        MMI_STRING_T text_str = {0};
        // GUI_RECT_T box = {0,100,30,319};

        MMIRES_GetText( TXT_ZDT_FIND_WATCH, MMK_ConvertIdToHandle(win_id), &text_str );   
        text_style.font = SONG_FONT_26;
        text_style.font_color = MMI_WHITE_COLOR;
        text_style.align = ALIGN_HMIDDLE;

        bg_ptr.bg_type = GUI_BG_COLOR;
        bg_ptr.color = MMI_BLACK_COLOR;
        bg_display.win_handle = MMK_ConvertIdToHandle(win_id);
        bg_display.display_rect = bg_rect;
        bg_display.rect = bg_rect;               
        GUI_DisplayBg(&bg_ptr,&bg_display,MMITHEME_GetDefaultLcdDev());
        
       
        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&start_point,
            (const MMI_STRING_T     *)&text_str,
            (GUISTR_STYLE_T         *)&text_style,
            state
            );*/
        }
        break;

    #if 1   //xiongkai add   for bug: 设备充电灭屏时-APP查找设备-设备被查找界面短按HOME键-界面消失，其铃声不会立即停止-拔掉充电线，设备仍显示查找界面；		
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
	 YX_API_CZSB_Stop();	
        break;		
    #endif
	
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL: 
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
            YX_API_CZSB_Stop();
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#endif

static void ZdtTalk_BackLight(BOOLEAN is_alway_on)
{
      if(is_alway_on)
      {
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
      }
      else
      {
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
      }
}

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
#if 1
static YX_RECORDER_DISPLAY_STAT s_cur_yx_talk_disp_status = YX_RECOREDER_DISP_IDLE;
LOCAL MMI_RESULT_E  HandleZDT_TalkWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );

WINDOW_TABLE( MMIZDT_TALK_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_TalkWinMsg),    
    WIN_ID( MMIZDT_TALK_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenTalkWin(void)
{
    if(FALSE == MMK_IsOpenWin(MMIZDT_TALK_WIN_ID))
    {
        MMK_CreateWin((uint32*)MMIZDT_TALK_WIN_TAB,PNULL);
    }
    return TRUE;
}

BOOLEAN MMIZDT_CloseTalkWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TALK_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_TALK_WIN_ID);
    }
    return TRUE;
}

void MMIZDT_Talk_ShowData(uint8 status, char * err_str)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x5FAE, 0x804A, 0}; //微聊 5FAE 804A 
    uint16 idle_wstr[20] = {0x8BF7, 0x6309, 0x952E, 0x5F00, 0x59CB, 0x8BB2, 0x8BDD, 0x002E, 0x002E, 0x002E, 0}; //请按键开始讲话...8BF7 6309 952E 5F00 59CB 8BB2 8BDD 002E 002E 002E 
    uint16 recording_wstr[20] = {0x6B63, 0x5728, 0x5F55, 0x97F3, 0x002E, 0x002E, 0x002E, 0}; //正在录音...6B63 5728 5F55 97F3 002E 002E 002E 
    uint16 recordok_wstr[10] = {0x5F55, 0x97F3, 0x6210, 0x529F, 0x0021, 0}; //录音成功!5F55 97F3 6210 529F  0021 
    uint16 recordfail_wstr[10] = {0x5F55, 0x97F3, 0x5931, 0x8D25, 0x0021, 0}; //录音失败!5F55 97F3 5931 8D25  0021 
    uint16 sending_wstr[20] = {0x6B63, 0x5728, 0x53D1, 0x9001, 0x8BED, 0x97F3, 0x002E, 0x002E, 0x002E, 0x002E, 0}; //正在发送语音....6B63 5728 53D1 9001 8BED 97F3 002E 002E 002E 002E 
    uint16 sendok_wstr[10] = {0x53D1, 0x9001, 0x6210, 0x529F, 0x0021, 0}; //发送成功!53D1 9001 6210 529F 0021 
    uint16 sendfail_wstr[10] = {0x53D1, 0x9001, 0x5931, 0x8D25, 0x0021, 0}; //发送失败!53D1 9001 5931 8D25 0021 
    GUI_FONT_T f_big =MMI_DEFAULT_BIG_FONT;
    GUI_FONT_T f_mid =MMI_DEFAULT_NORMAL_FONT;
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = 2; 
    cur_rect.bottom = cur_rect.top + 24;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    switch(status)
    {
        case YX_RECOREDER_DISP_IDLE:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(idle_wstr);
                cur_str_t.wstr_ptr = idle_wstr;
            break;
            
        case YX_RECOREDER_DISP_RECORDING:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(recording_wstr);
                cur_str_t.wstr_ptr = recording_wstr;
            break;
            
        case YX_RECOREDER_DISP_RECORD_OK:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(recordok_wstr);
                cur_str_t.wstr_ptr = recordok_wstr;
            break;
            
        case YX_RECOREDER_DISP_RECORD_FAIL:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(recordfail_wstr);
                cur_str_t.wstr_ptr = recordfail_wstr;
            break;
            
        case YX_RECOREDER_DISP_SENDING:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(sending_wstr);
                cur_str_t.wstr_ptr = sending_wstr;
            break;
            
        case YX_RECOREDER_DISP_SEND_OK:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(sendok_wstr);
                cur_str_t.wstr_ptr = sendok_wstr;
            break;
            
        case YX_RECOREDER_DISP_SEND_FAIL:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(sendok_wstr);
                cur_str_t.wstr_ptr = sendok_wstr;
            break;
            
        default:
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(idle_wstr);
                cur_str_t.wstr_ptr = idle_wstr;
            break;
    }
    
    cur_rect.top    = cur_rect.bottom+30; 
    cur_rect.bottom = cur_rect.top + 24;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    if(err_str != PNULL)
    {
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        cur_rect.top    = cur_rect.bottom+10; 
        cur_rect.bottom = cur_rect.top + 24;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",err_str);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    return;
}

BOOLEAN MMIZDT_UpdateTalkWin(uint8 status,char * err_str)
{
    s_cur_yx_talk_disp_status = status;
    if(MMK_IsOpenWin(MMIZDT_TALK_WIN_ID))
    {
        MMIZDT_Talk_ShowData(status,err_str);
    }
    return TRUE;
}

BOOLEAN MMIZDT_UpdateTalkWinErr(YXVocRecordErrStatus err_code)
{
    if(MMK_IsOpenWin(MMIZDT_TALK_WIN_ID))
    {
        char tmp_buf[100] = {0};
        sprintf(tmp_buf,"Err:%d",err_code);
        MMIZDT_Talk_ShowData(s_cur_yx_talk_disp_status,tmp_buf);
    }
    return TRUE;
}

BOOLEAN MMIZDT_UpdateTalkStr(char * err_str)
{
    if(MMK_IsOpenWin(MMIZDT_TALK_WIN_ID))
    {
        MMIZDT_Talk_ShowData(s_cur_yx_talk_disp_status,err_str);
    }
    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZDT_TalkWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    YX_APP_T * pMe = &g_yx_app;

    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
            ZdtTalk_BackLight(TRUE);
        break;
        
    case MSG_FULL_PAINT:
            s_cur_yx_talk_disp_status = YX_RECOREDER_DISP_IDLE;
            MMIZDT_Talk_ShowData(s_cur_yx_talk_disp_status,PNULL);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_LOSE_FOCUS:
        break;
            
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
            YX_API_Record_Start();
        break;
        
    case MSG_KEYDOWN_CANCEL:
        break;
        
    case MSG_KEYUP_CANCEL:
            //YX_API_Record_Stop();
            MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
            
            YX_Voice_HandleStop(pMe);
            //YX_Net_Send_TK_VocFile_End(pMe,YX_VOCSEND_ERR_UNREG);

            ZdtTalk_BackLight(FALSE);
            if(p_index != PNULL)
            {
                SCI_FREE(p_index);
            }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#if 1
LOCAL MMI_RESULT_E  HandleZDT_TalkTPWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );

WINDOW_TABLE( MMIZDT_TALK_TP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_TalkTPWinMsg),    
    WIN_ID( MMIZDT_TALK_TP_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

void MMIZDT_Talk_TP_ShowData(uint8 status, char * err_str)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x5FAE, 0x804A, 0}; //微聊 5FAE 804A 
    uint16 play_wstr[10] = {0x64AD, 0x653E, 0}; //播放64AD 653E 
    GUI_FONT_T f_big =MMI_DEFAULT_BIG_FONT;
    GUI_FONT_T f_mid =MMI_DEFAULT_NORMAL_FONT;
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = 2; 
    cur_rect.bottom = cur_rect.top + 24;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(play_wstr);
    cur_str_t.wstr_ptr = play_wstr;
    
    cur_rect.top    = cur_rect.bottom+30; 
    cur_rect.bottom = cur_rect.top + 24;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    {
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        cur_rect.top    = cur_rect.bottom+10; 
        cur_rect.bottom = cur_rect.top + 24;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "Sum: %d",YX_DB_GetVocRcvNum());
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }

    if(err_str != PNULL)
    {
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        cur_rect.top    = cur_rect.bottom+10; 
        cur_rect.bottom = cur_rect.top + 24;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",err_str);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    return;
}

BOOLEAN MMIZDT_UpdateTalkTPWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TALK_TP_WIN_ID))
    {
        MMIZDT_Talk_TP_ShowData(0,0);
    }
    return TRUE;
}

BOOLEAN MMIZDT_UpdateTalkTPWinErr(char * err_str)
{
    if(MMK_IsOpenWin(MMIZDT_TALK_TP_WIN_ID))
    {
        MMIZDT_Talk_TP_ShowData(0,err_str);
    }
    return TRUE;
}

BOOLEAN MMIZDT_OpenTalkTPWin(void)
{
    MMIDEFAULT_TurnOnBackLight();
    if(MMK_IsOpenWin(MMIZDT_TALK_TP_WIN_ID))
    {
        MMIZDT_Talk_TP_ShowData(0,0);
    }
    else
    {
        MMK_CreateWin((uint32*)MMIZDT_TALK_TP_WIN_TAB,PNULL);
    }
    
    return TRUE;
}

BOOLEAN MMIZDT_CloseTalkTPWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TALK_TP_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_TALK_TP_WIN_ID);
    }
    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZDT_TalkTPWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
            
        break;
        
        case MSG_FULL_PAINT:
                MMIZDT_Talk_TP_ShowData(0,0);
            break;
            
        case MSG_GET_FOCUS:
            break;
            
        case MSG_LOSE_FOCUS:
            break;
            
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
            YX_API_Record_FilePlay();
        break;
        
    case MSG_KEYDOWN_CANCEL:
        break;
        
    case MSG_KEYUP_CANCEL:
            YX_API_Record_PlayStop();
            MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
            if(p_index != PNULL)
            {
                SCI_FREE(p_index);
            }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#endif
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE

//yangyu end


//yangyu add begin for tiny chat 
#include "mmisrvrecord_export.h"
#include "zdt_yx_voice.h"

/*typedef struct TINYCHAT_ITEM
{
    

}*/



LOCAL MMI_RESULT_E  HandleZDT_TinyChatWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );
//LOCAL void MMIZDT_ShowPlayAudioIcon(uint16 list_index, BOOLEAN isPlaying);
LOCAL void MMIZDT_ShowPlayAudioIcon(uint16 list_index, BOOLEAN isPlaying, BOOLEAN isLeft);




#define TINY_CHAT_LIST_MAX_SIZE   MAX_YX_VOC_SAVE_SIZE
#define TINYCHAT_RECORD_ANIM_DELAY 500
#define TINYCHAT_MAX_RECORD_TIME_MS 10000
#define TINYCHAT_BOTTOM_HEIGHT  52  //xiongkai 58//120
#define TINYCHAT_LIS_BOX_BOTTOM (MMI_MAINSCREEN_HEIGHT -1- TINYCHAT_BOTTOM_HEIGHT)//(240-83)//180//160

#define  TINYCHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM  4


LOCAL RECORD_SRV_HANDLE tiny_chat_record_handle = PNULL;
static uint8 tiny_chat_record_anim_timer_id = 0;
LOCAL uint32 tiny_chat_record_update_anim_index =0;
LOCAL uint32 tiny_chat_record_timer_index =0;
LOCAL uint8 tiny_chat_is_recording =0;

static uint8 tiny_chat_max_record_timer_id = 0;

char txt_line_path_data[MAX_YX_VOC_SAVE_SIZE][50] = {0};
static uint16 real_lines = 0;
LOCAL int16 z_playing_index = -1;
static int s_chat_tp_x;
static int s_chat_tp_y;
static BOOLEAN s_is_chat_tp_down = FALSE;
static BOOLEAN s_is_chat_tp_long = FALSE;

MMISRV_HANDLE_T      tinychat_playmedia_handle = 0;

WINDOW_TABLE( MMIZDT_TINY_CHAT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_TinyChatWinMsg),    
    WIN_ID( MMIZDT_TINY_CHAT_WIN_ID),
    WIN_HIDE_STATUS,
    //WIN_TITLE(TXT_BH_NET_CHAT),
    END_WIN
};

BOOLEAN MMIZDT_OpenTinyChatWin(void)
{
    if(FALSE == MMK_IsOpenWin(MMIZDT_TINY_CHAT_WIN_ID))
    {
        MMK_CreateWin((uint32*)MMIZDT_TINY_CHAT_WIN_TAB,PNULL);
    }
    return TRUE;
}

BOOLEAN MMIZDT_CloseTinyChatWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TINY_CHAT_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_TINY_CHAT_WIN_ID);
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIZDT_IsInTinyChatWin()
{
    
    if(MMK_IsOpenWin(MMIZDT_TINY_CHAT_WIN_ID))
    {
        return TRUE;
    }
    return FALSE;
}



PUBLIC void MMIAPIMENU_EnterTinyChat(void)
{

    MMIZDT_OpenTinyChatWin();

}


void MMIZDT_TinyShowBottom(MMI_WIN_ID_T        win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    //uint16 tittle[10] = {0x957F, 0x6309, 0x5E95 ,0x90E8, 0x8BB2, 0x8BDD, 0}; //长按此处讲话957F 6309 6B64 5904 8BB2 8BDD 
    uint16 tittle[10] = {0x6309,0x4F4F,0x8BB2,0x8BDD, 0}; ///按住讲话
    GUI_FONT_T f_big = SONG_FONT_22;//SONG_FONT_19;//SONG_FONT_34;
    GUI_POINT_T      point = {0};
    
    rect.left = 0;
    rect.top= TINYCHAT_LIS_BOX_BOTTOM;
    rect.right= (MMI_MAINSCREEN_WIDTH -1);
    rect.bottom = (MMI_MAINSCREEN_WIDTH -1); 

#if 1  //xiongkai
    rect.left = 0;
    rect.top= TINYCHAT_LIS_BOX_BOTTOM;
    rect.right= MMI_MAINSCREEN_WIDTH;
    rect.bottom = MMI_MAINSCREEN_HEIGHT; 
    
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    point.x = (MMI_MAINSCREEN_WIDTH - 128)/2;////img   128X42
    point.y = 190;//40;

    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_CHAT_NO_RECORD_BG, 
                &lcd_dev_info);
#else	
    LCD_FillRect(&lcd_dev_info, rect, MMI_DARK_BLUE_COLOR/*MMI_BLACK_COLOR*/);
#endif
    #if 0
    point.x = 2;//100;
    point.y = rect.top + 20;//40;

    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_TINYCHAT_MIC, //should use new
                &lcd_dev_info);
    
    cur_rect = rect;
    cur_rect.bottom = rect.top + 60;//40;//20;//40;
    #endif

#if 1  //xiongkai
		rect.left =94; // 92;
		rect.top= 190; // TINYCHAT_LIS_BOX_BOTTOM-8;
		rect.right= 240; // 172;
		rect.bottom = 230; 
#endif	

	cur_rect = rect;
	//cur_rect.right= MMI_MAINSCREEN_WIDTH -1;

    text_style.align = ALIGN_LVMIDDLE; // ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );


    point.x = MMI_MAINSCREEN_WIDTH - TINYCHAT_BOTTOM_HEIGHT -1+3;//100;  83
    point.y = MMI_MAINSCREEN_HEIGHT - TINYCHAT_BOTTOM_HEIGHT -1+3;//40;  83
    /*
    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_CHAT_NO_RECORD_BG, //should use new
                &lcd_dev_info);
        */

#if 0 //xiongkai	
    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_TINYCHAT_MIC, //should use new
                &lcd_dev_info);
#endif
    return;
}

LOCAL void MMIZDT_StartRecordAnimTimer()
{
    if(tiny_chat_record_anim_timer_id != 0)
    {
        MMK_StopTimer(tiny_chat_record_anim_timer_id);
    }
    tiny_chat_record_anim_timer_id =  MMK_CreateWinTimer(MMIZDT_TINY_CHAT_WIN_ID, TINYCHAT_RECORD_ANIM_DELAY,TRUE);

}

LOCAL void MMIZDT_StopRecordAnimTimer()
{
    if(tiny_chat_record_anim_timer_id != 0)
    {
        MMK_StopTimer(tiny_chat_record_anim_timer_id);
    }
    tiny_chat_record_anim_timer_id = 0;

}

LOCAL void MMIZDT_StartRecordMaxTimeTimer()
{
    if(tiny_chat_max_record_timer_id != 0)
    {
        MMK_StopTimer(tiny_chat_max_record_timer_id);
    }
    tiny_chat_max_record_timer_id =  MMK_CreateWinTimer(MMIZDT_TINY_CHAT_WIN_ID, TINYCHAT_MAX_RECORD_TIME_MS, FALSE);

}

LOCAL void MMIZDT_StopRecordMaxTimeTimer()
{
    if(tiny_chat_max_record_timer_id != 0)
    {
        MMK_StopTimer(tiny_chat_max_record_timer_id);
    }
    tiny_chat_max_record_timer_id = 0;

}



LOCAL void MMIZDT_ClearTinyChatRecordAnim(MMI_WIN_ID_T win_id)
{
    MMIZDT_TinyShowBottom(win_id);
}

LOCAL MMI_IMAGE_ID_T MMIZDT_GetTinyChatRecordAnimalFrame(uint32 cur_frame_index)
{
    MMI_IMAGE_ID_T  anim_frame[TINYCHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM] = {IMAGE_TINYCHAT_TALK_ANIM_01, IMAGE_TINYCHAT_TALK_ANIM_02, 
                                                                        IMAGE_TINYCHAT_TALK_ANIM_03, IMAGE_TINYCHAT_TALK_ANIM_04};
    if(cur_frame_index < TINYCHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM)
    {
        {
            return anim_frame[cur_frame_index];

        }
    }
    return 0;
}


LOCAL void MMIZDT_DisplayTinyChatRecordAnim(MMI_WIN_ID_T win_id)
{
    MMI_IMAGE_ID_T cur_img_id = 0;
    GUI_POINT_T      point = {50, 240-40};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big = SONG_FONT_32;//SONG_FONT_28;//SONG_FONT_34;
    uint16 tittle[10] = {0x6b63, 0x5728, 0x5f55, 0x97f3,0}; //正在录音

    rect.left = 0;
    rect.top= TINYCHAT_LIS_BOX_BOTTOM;
    rect.right= (MMI_MAINSCREEN_WIDTH -1);
    rect.bottom = (MMI_MAINSCREEN_WIDTH -1);
    #if 0
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLUE_COLOR);
    
    point.x = 2;//100;
    point.y = rect.top + 20;//20;//40;
    cur_img_id = MMIZDT_GetTinyChatRecordAnimalFrame(tiny_chat_record_update_anim_index);

    if(0 != cur_img_id)
    {
            GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                cur_img_id,
                &lcd_dev_info);
    }
    #endif

    point.x = MMI_MAINSCREEN_WIDTH - TINYCHAT_BOTTOM_HEIGHT -1+3;//100; 83
    point.y = MMI_MAINSCREEN_HEIGHT - TINYCHAT_BOTTOM_HEIGHT -1+3;//40; 83


#if 1  //xiongkai
    rect.left = 0;
    rect.top= TINYCHAT_LIS_BOX_BOTTOM;
    rect.right= MMI_MAINSCREEN_WIDTH;
    rect.bottom = MMI_MAINSCREEN_HEIGHT; 
    
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    point.x = (MMI_MAINSCREEN_WIDTH - 128)/2;//img   128X42
    point.y = 190;//40;

    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_CHAT_RECORDING_BG, 
                &lcd_dev_info);
#else		
    LCD_FillRect(&lcd_dev_info, rect, MMI_GREEN_COLOR/*MMI_BLACK_COLOR*/);
#endif


#if 1  //xiongkai
		rect.left = 15;
		rect.top= TINYCHAT_LIS_BOX_BOTTOM+6;
		rect.right= 60;
		rect.bottom = rect.top +36; 
#endif		

    cur_rect = rect;
    //cur_rect.bottom = rect.top+60;//20;//40;
    //cur_rect.right = point.x;
    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%d",((TINYCHAT_MAX_RECORD_TIME_MS/1000)-(tiny_chat_record_timer_index/2)));
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

	rect.left =94; // 92;
	rect.top= 190; // TINYCHAT_LIS_BOX_BOTTOM-8;
	rect.right= 240; // 172;
	rect.bottom = 230;	

    cur_rect = rect;
    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = SONG_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );	
		
   
    /*
    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_CHAT_RECORDING_BG, //should use new
                &lcd_dev_info);
        */

#if 0	
    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_TINYCHAT_MIC, //should use new
                &lcd_dev_info);
#endif
}

LOCAL void MMIZDT_TinyChatAudioPlayNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    //AI_APP_T *   pMe = (AI_APP_T *)&g_ai_app;
    BOOLEAN       result = TRUE;
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == tinychat_playmedia_handle)
        {
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    ZDT_LOG("MMIZDT_TinyChatAudioPlayNotify MMISRVAUD_REPORT_END data1=%d" ,report_ptr->data1);
                   
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    tinychat_playmedia_handle = 0;
                    if(z_playing_index >= 0)
                    {
                        MMIZDT_ShowPlayAudioIcon(z_playing_index , FALSE, FALSE);
                    }

                    if (MMISRVAUD_REPORT_RESULT_STOP != report_ptr->data1)
                    {
                        if (MMISRVAUD_REPORT_RESULT_SUCESS != report_ptr->data1)
                        {
                            result = FALSE;
                        }
                        //AI_Voice_Play_HandleEnd(result);
                    }

                    break;

                default:
                    break;
            }
        }
    }
}

LOCAL BOOLEAN MMIZDT_TinyChatAudioPlayRequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify,
						MMISRVAUD_VOLUME_T volume
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    req.notify = notify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = route;
    audio_srv.volume =  volume;

    switch(audio_data->type)
    {
    case MMISRVAUD_TYPE_RING_FILE:
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;

        if(audio_srv.volume > MMISRVAUD_VOLUME_LEVEL_MAX)
        {
            audio_srv.volume = MMISRVAUD_VOLUME_LEVEL_MAX;
        }
        break;

    case MMISRVAUD_TYPE_RECORD_FILE:
        audio_srv.info.record_file.type = audio_data->type;        
        audio_srv.info.record_file.fmt  = audio_data->record_file.fmt;
        audio_srv.info.record_file.name = audio_data->record_file.name;
        audio_srv.info.record_file.name_len = audio_data->record_file.name_len;    
        audio_srv.info.record_file.source   = audio_data->record_file.source;
        audio_srv.info.record_file.frame_len= audio_data->record_file.frame_len;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;

    case MMISRVAUD_TYPE_IQ://测试模式
        audio_srv.info.iq_file.type = audio_data->type;
        audio_srv.info.iq_file.name_ptr = audio_data->iq_file.name_ptr;
        audio_srv.info.iq_file.param    = audio_data->iq_file.param;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;
        
    default:
        break;
    }

    *audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    ZDT_LOG("MMIZDT_TinyChatAudioPlayRequestHandle = %d", *audio_handle);    
    if(*audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//LOCAL void MMIZDT_ShowPlayAudioIcon(uint16 list_index, BOOLEAN isPlaying)
LOCAL void MMIZDT_ShowPlayAudioIcon(uint16 list_index, BOOLEAN isPlaying, BOOLEAN isLeft)
{
    CTRLLIST_ITEM_T *item_ptr = CTRLLIST_GetItem(MMIZDT_TINY_CHAT_LIST_CTRL_ID, list_index);
    GUILIST_ITEM_DATA_T *item_data_ptr = NULL;
    
    if(item_ptr == NULL)
    {
        return;
    }
    ZDT_LOG("yyu MMIZDT_ShowPlayAudioIcon list_index=%d, isPlaying=%d", list_index, isPlaying);
    item_data_ptr =  item_ptr->data_ptr;
    if(isPlaying)
    {
        
        z_playing_index = list_index;
        item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;      
        //item_data_ptr->item_content[2].item_data.image_id = isLeft ? IMAGE_TINYCHAT_AUDIO_LEFT:IMAGE_TINYCHAT_AUDIO_RIGHT;//IMAGE_TINYCHAT_PLAYING_AUDIO;
        item_data_ptr->item_content[2].item_data.image_id = 0;//IMAGE_NULL;
    }
    else
    {
        z_playing_index = -1;
        item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_NONE;      
        item_data_ptr->item_content[2].item_data.image_id = 0;//IMAGE_NULL;
    }
    //MMK_UpdateScreen();//useless
    MMK_SendMsg(MMK_ConvertIdToHandle(MMIZDT_TINY_CHAT_LIST_CTRL_ID), MSG_CTL_PAINT, PNULL);//ok ,list will update view
    

}


LOCAL int MMIZDT_TinyChatStartPlayAudio(uint16 index, uint16 list_index)
{
    MMISRVAUD_TYPE_U    audio_data  = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;
    uint16      full_path[50] = {0};
    uint16      full_path_len = 0;
    int Ret = 0;
    char file_name[50] = {0};
    BOOLEAN isLeft = FALSE;

    YX_APP_T * pMe = &g_yx_app;
    SCI_MEMCPY(file_name, txt_line_path_data[index], 50);
    
    ZDT_LOG("MMIZDT_TinyChatStartPlayAudio 1 index=%d, file_name=%s",index, file_name);
    if(file_name == PNULL || !ZDT_File_Exsit(txt_line_path_data[index]))
    {
        return 1;
    }
    isLeft = YX_VOC_IsRcvFile(txt_line_path_data[index]);

    YX_Voice_HandleStop(pMe); //stop rcv play and stop record
    
    
    if(tinychat_playmedia_handle)
    {   
        if(z_playing_index >= 0)
        {
            MMIZDT_ShowPlayAudioIcon(z_playing_index , FALSE, FALSE);
        }
        MMISRVAUD_Stop(tinychat_playmedia_handle);
        MMISRVMGR_Free(tinychat_playmedia_handle);
        tinychat_playmedia_handle = NULL;
    }
    
    full_path_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = full_path;
    audio_data.ring_file.name_len = full_path_len;
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
    
    if(MMIZDT_TinyChatAudioPlayRequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, MMIZDT_TinyChatAudioPlayNotify, MMISRVAUD_VOLUME_LEVEL_MAX))
    {
        tinychat_playmedia_handle = audio_handle;
        if(!MMISRVAUD_Play(audio_handle, 0))
        {     
            ZDT_LOG("MMIZDT_TinyChatStartPlayAudio play failed");
            MMISRVMGR_Free(tinychat_playmedia_handle);
            tinychat_playmedia_handle = 0;       
            Ret = 2;
        }
        else
        {
            MMIZDT_ShowPlayAudioIcon(list_index, TRUE, isLeft);
            
        }
    }
    else
    {        
        Ret = 3;
    }
      
    if (Ret != 0)
    {
        tinychat_playmedia_handle = NULL;
        //m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
        ZDT_LOG("MMIZDT_TinyChatStartPlayAudio Err Play Ret=%d",Ret);
        return Ret;
    }
    else
    {
        //ZDT_LOG("MMIZDT_TinyChatStartPlayAudio OK vol=%d",s_ai_voc_play_vol);
        //m_ai_eFilePlayrStatus = AI_FILE_PLAYING;        
    }
    return Ret;


}

LOCAL void MMIZDT_TinyChatStopPlayAudio()
{
    if(tinychat_playmedia_handle)
    {
        if(z_playing_index >= 0)
        {
            MMIZDT_ShowPlayAudioIcon(z_playing_index , FALSE, FALSE);
        }
        MMISRVAUD_Stop(tinychat_playmedia_handle);
        MMISRVMGR_Free(tinychat_playmedia_handle);
        tinychat_playmedia_handle = NULL;
    }
}



PUBLIC void MMIZDT_TinyChatRefreshWin()
{
    ZDT_LOG("MMIZDT_TinyChatRefreshWin ");
    if(!MMK_IsOpenWin(MMIZDT_TINY_CHAT_WIN_ID))
    {
        return;
    }
    MMK_SendMsg(MMIZDT_TINY_CHAT_WIN_ID, MSG_FULL_PAINT, NULL);
}

PUBLIC void MMIZDT_TinyChatSendCallBack(BOOLEAN is_ok)
{
    ZDT_LOG("MMIZDT_TinyChatRefreshWin ");
    if(!MMK_IsOpenWin(MMIZDT_TINY_CHAT_WIN_ID))
    {
        return;
    }
    if(is_ok)
    {
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_SEND_FAIL,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

//yangyu add new style
#define CHAT_LEFT_RIGHT_STYLE
LOCAL THEMELIST_ITEM_STYLE_T chat_left_style = {0};
LOCAL THEMELIST_ITEM_STYLE_T chat_right_style = {0};

#define MAX_REMAIN_SIZE  (18*1024) //10K
LOCAL BOOLEAN ZDT_IsMemNearFull(BOOLEAN is_tf)
{
    uint32          free_space_high     = 0;
    uint32          free_space_low      = MAX_REMAIN_SIZE;//102400;
    uint32          cur_free_space_high = 0;
    uint32          cur_free_space_low  = 0;
    MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_UDISK;

    if(is_tf)
    {
       sd_type = MMI_DEVICE_SDCARD;
    }
    SCI_TRACE_LOW("yyu ZDT_IsMemNearFull  sd_type=%d" ,sd_type);
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
        file_ret = SFS_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                                            &cur_free_space_high,
                                            &cur_free_space_low);
        SCI_TRACE_LOW("yyu ZDT_IsMemNearFull cur_high=%ld,cur_low=%ld,high=%ld,low=%ld", cur_free_space_high, cur_free_space_low, free_space_high,free_space_low);
        if (file_ret == SFS_ERROR_NONE)
        {
            if (cur_free_space_high > free_space_high //高位大于肯定大于
                    || ( (cur_free_space_high == free_space_high) //高位相等，低位大于
                         && (cur_free_space_low > free_space_low))
               )
            {
                return FALSE;
            }
        }
    }
        
    return TRUE;
}
//yangyu end


LOCAL void MMIZDT_TinyChatUpdateList()
{
    #if 0
    BOOLEAN ret = FALSE;
    uint16 i = 0, j=0;
    uint16 position = 0;

    #ifdef CHAT_LEFT_RIGHT_STYLE   
    const uint16 h_gap = 2;
    const uint16 w_gap = 2;
    const uint16 icon_h =40; // 56;
    const uint16 icon_w = 40; // 56;
   
    const uint16 img_height =40; // 56;
    const uint16 img_width = 146; //xiongkai  160;
    const uint16 height = icon_h + 2*h_gap;
    const uint16 img_h_gap = (height - img_height)/2;
    GUI_RECT_T l_rect0 = { icon_h+w_gap+1, img_h_gap ,  img_width + icon_h+w_gap+1, img_height+img_h_gap};//bg
    GUI_RECT_T l_rect1 = { img_width + icon_h+w_gap+1-6-40, img_h_gap ,  img_width + icon_h+w_gap+1-6, img_height+img_h_gap};//text
    GUI_RECT_T l_rect2 = { icon_h+h_gap+6,  h_gap,  icon_h+h_gap+6+35 , icon_h+h_gap};//voice icon
    GUI_RECT_T l_rect3 = { w_gap,  h_gap,  icon_h+w_gap , icon_h+h_gap};//head icon
    
   
    GUI_RECT_T r_rect0 = { MMI_MAINSCREEN_WIDTH - (img_width + icon_h+w_gap+1), img_h_gap,   MMI_MAINSCREEN_WIDTH - (icon_h+w_gap+1),  img_height+img_h_gap};//bg
    GUI_RECT_T r_rect1 = { MMI_MAINSCREEN_WIDTH - (img_width + icon_h+w_gap+1-6),  img_h_gap,  MMI_MAINSCREEN_WIDTH - (icon_h+w_gap+1+2) , img_height+img_h_gap};//text
    GUI_RECT_T r_rect2 = { MMI_MAINSCREEN_WIDTH-(icon_h+h_gap+6+35),  h_gap,  MMI_MAINSCREEN_WIDTH - (icon_h+h_gap+6) , icon_h+h_gap};//voice icon
    GUI_RECT_T r_rect3 = { MMI_MAINSCREEN_WIDTH -(icon_h+w_gap),  h_gap,  MMI_MAINSCREEN_WIDTH-(w_gap) , icon_h+h_gap};//head icon
    
    
    //uint32 i = GUILIST_GetTotalItemNum(ctrl_id);
    THEMELIST_ITEM_STYLE_T* pItemStyle = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS);
    SCI_MEMCPY(&chat_left_style, pItemStyle, sizeof(*pItemStyle));
    pItemStyle = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_BGICON_R_TEXT_MS);
    SCI_MEMCPY(&chat_right_style, pItemStyle, sizeof(*pItemStyle));
    chat_left_style.height = chat_left_style.height_focus = height;
    chat_right_style.height = chat_right_style.height_focus = height;
    
    chat_left_style.content[0].rect = chat_left_style.content[0].rect_focus = l_rect0;
    chat_left_style.content[1].rect = chat_left_style.content[1].rect_focus = l_rect1;
    chat_left_style.content[2].rect = chat_left_style.content[2].rect_focus = l_rect2;
    chat_left_style.content[3].rect = chat_left_style.content[3].rect_focus = l_rect3;
    chat_right_style.content[0].rect = chat_right_style.content[0].rect_focus = r_rect0;
    chat_right_style.content[1].rect = chat_right_style.content[1].rect_focus = r_rect1;
    chat_right_style.content[2].rect = chat_right_style.content[2].rect_focus = r_rect2;
    chat_right_style.content[3].rect = chat_right_style.content[3].rect_focus = r_rect3;

    //chat_left_style.content[1].state = GUIITEM_CONTENT_STATE_TEXT_R_ALIGN;
    #endif

    SCI_MEMSET(txt_line_path_data, 0, MAX_YX_VOC_SAVE_SIZE*50*sizeof(char));

    YX_VocFilePathDBReadToBuffer(txt_line_path_data ,&real_lines);
    GUILIST_RemoveAllItems(MMIZDT_TINY_CHAT_LIST_CTRL_ID);
    for(i = 0 ; i < real_lines ; i++)
    {        
        GUILIST_ITEM_T       		item_t    =  {0};
        GUILIST_ITEM_DATA_T  		item_data = {0};
        wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
        uint16 total_item_num = 0;
        BOOLEAN isLeft = FALSE;
        wchar   full_path[50] = {0}; 
        uint16  full_path_len = 0;

        MMISRVAUD_CONTENT_INFO_T  mp3_file_info = {0};
        wchar 						temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
        wchar                       time_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
        if(!ZDT_File_Exsit(txt_line_path_data[i]))
        {
            continue;
        }
        
        isLeft = YX_VOC_IsRcvFile(txt_line_path_data[i]);
        ZDT_LOG("MMIZDT_TinyChatUpdateList i=%d isLeft=%d, %s" , i, isLeft , txt_line_path_data[i]);
        item_t.item_data_ptr = &item_data; 
        //item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;//GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;//GUIITEM_STYLE_ONE_LINE_TEXT; 

        if(isLeft)
        {
            item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS;
		
			item_data.item_content[1].is_custom_font_color = TRUE;
			item_data.item_content[1].custom_font_color = 0X043F;		
			item_data.item_content[1].custom_font_color_focus = 0X043F;				//左边蓝色字
		
        }
        else
        {
            item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_R_TEXT_MS;
        }
        #ifdef CHAT_LEFT_RIGHT_STYLE 
        item_t.item_style =  GUIITEM_STYLE_CUSTOMIZE;
        #endif

        
         //item_t.left_item_style = GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS;
        
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[0].item_data.image_id = isLeft ? IMAGE_TINYCHAT_VOICE_LEFT_BG : IMAGE_TINYCHAT_VOICE_RIGHT_BG;
        

        /*
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = buff;
        //MMIAPICOM_StrToWstr(content , item_data.item_content[0].item_data.text_buffer.wstr_ptr);// luan ma,Garbled
        //GUI_GBToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese

        //GUI_GB2UCS(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
        //GUI_GBKToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
        GUI_UTF8ToWstr(item_data.item_content[2].item_data.text_buffer.wstr_ptr, GUILIST_STRING_MAX_NUM + 1, content, SCI_STRLEN(content));

        item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (item_data.item_content[2].item_data.text_buffer.wstr_ptr);
        */
        

        full_path_len = GUI_GBToWstr(full_path, (const uint8*)txt_line_path_data[i], SCI_STRLEN(txt_line_path_data[i]));


        MMISRVAUD_GetFileContentInfo (
                    full_path,
                    full_path_len,
                    &mp3_file_info
                    );

        SCI_MEMSET (temp_wstr, 0, sizeof (temp_wstr));
        SCI_MEMSET (time_wstr, 0, sizeof (time_wstr));
        if(mp3_file_info.total_time == 0) 
        {
            mp3_file_info.total_time = 1;//for ui show 0 problem
        }
        sprintf ( (char*) temp_wstr, "  %d\"", mp3_file_info.total_time);
        MMIAPICOM_StrToWstr (temp_wstr, time_wstr);

        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = time_wstr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (time_wstr);

        item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[3].item_data.image_id = isLeft ? IMAGE_TINYCHAT_HEAD_SERVER : IMAGE_TINYCHAT_HEAD;   //xiongkai   微聊头像左侧和右侧用不同的    IMAGE_TINYCHAT_HEAD;//isLeft ? IMAGE_TINYCHAT_AUDIO_LEFT : IMAGE_TINYCHAT_AUDIO_RIGHT;
        
        ret = GUILIST_AppendItem (MMIZDT_TINY_CHAT_LIST_CTRL_ID, &item_t);
        ZDT_LOG("MMIZDT_TinyChatUpdateList ret = %d", ret);
        if(ret)
        {
            CTRLLIST_SetItemUserData(MMIZDT_TINY_CHAT_LIST_CTRL_ID, position, &i);
            #ifdef CHAT_LEFT_RIGHT_STYLE 
            GUILIST_SetItemStyleEx(MMIZDT_TINY_CHAT_LIST_CTRL_ID, position, isLeft ? &chat_left_style : &chat_right_style);
            #endif
            position++;
            total_item_num = CTRLLIST_GetTotalItemNum(MMIZDT_TINY_CHAT_LIST_CTRL_ID);
            if(total_item_num >= 1)
            {
               CTRLLIST_SetSelectedItem(MMIZDT_TINY_CHAT_LIST_CTRL_ID, total_item_num-1, TRUE);
               GUILIST_SetCurItemIndex(MMIZDT_TINY_CHAT_LIST_CTRL_ID, total_item_num-1); 
            }

            //MMK_UpdateScreen();
          
        }

    }
    #endif
}



LOCAL MMI_RESULT_E  HandleZDT_TinyChatWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_BG_T bg_ptr = {0};
    GUI_BG_DISPLAY_T bg_display = {0};
    const GUI_LCD_DEV_INFO *lcd_info = MMITHEME_GetDefaultLcdDev();
    MMIRECORD_SRV_RESULT_E record_result = MMIRECORD_SRV_RESULT_SUCCESS;
    MMI_CTRL_ID_T           ctrl_id = MMIZDT_TINY_CHAT_LIST_CTRL_ID;
    //MMIFMM_VIEW_WIN_DATA_T* view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);
    GUILIST_INIT_DATA_T list_init = {0};    
    uint16                      cur_selection   =   0;
    static uint8 s_is_key_green_down = 0;
    uint32 pos_user_data = 0; // position user data
    uint16 index; //click item index
    
    BOOLEAN ret = FALSE;


    uint8 * p_index = PNULL;
    YX_APP_T * pMe = &g_yx_app;

    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);

    ZDT_LOG("HandleZDT_TinyChatWinMsg msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        //ZdtTalk_BackLight(TRUE);//yangyu delete
        /*if (MMI_RESULT_FALSE == AiChatListViewWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }*/
        MMISRVAUD_ReqVirtualHandle("TINY CHAT", MMISRVAUD_PRI_NORMAL);

        //view_win_d = (MMIFMM_VIEW_WIN_DATA_T*) MMK_GetWinUserData (win_id);     
        
        list_init.both_rect.v_rect.left = 0;//0;//mic icon width is 52
        list_init.both_rect.v_rect.right = (MMI_MAINSCREEN_WIDTH -1);
        list_init.both_rect.v_rect.top = 0;
        list_init.both_rect.v_rect.bottom = TINYCHAT_LIS_BOX_BOTTOM-1;//185;//240;//180;

        list_init.both_rect.h_rect.left = 0;
        list_init.both_rect.h_rect.right = (MMI_MAINSCREEN_WIDTH -1);//185;//240;//180;
        list_init.both_rect.h_rect.top = 0;
        list_init.both_rect.h_rect.bottom =(MMI_MAINSCREEN_WIDTH -1); //240;
        
        list_init.type = GUILIST_TEXTLIST_E;
                    
        GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);            
        MMK_SetAtvCtrl(win_id,ctrl_id);

        CTRLLIST_SetTextFont(ctrl_id, SONG_FONT_24, MMI_CYAN_COLOR);
        ret = GUILIST_SetMaxItem(ctrl_id, TINY_CHAT_LIST_MAX_SIZE, FALSE);
        //CTRLLIST_SetAutoSwitchItem(ctrl_id , TRUE);
        ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_OPEN_WINDOW GUILIST_SetMaxItem ret =%d", ret);
        //AiUpdateRecordListData (win_id, ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, TXT_NULL, TRUE);
       
        //MMIAI_Net_Init(); //init net sim 
        //MMIAI_Net_Open(); //send msg ,to active pdp
        //AI_Net_Init(); // init url ip task, not here ,need after pdp active

        
        break;
    case MSG_LOSE_FOCUS:
            if(tiny_chat_is_recording)
            {
                MMIZDT_StopRecordAnimTimer();
                MMIZDT_ClearTinyChatRecordAnim(win_id);
                MMK_UpdateScreen();
                MMIZDT_StopRecordMaxTimeTimer();
                YX_API_Record_Stop();
                tiny_chat_is_recording = 0;

		   #if 1   //xiongkai    微聊界面冲突
		       // bug: 微聊界面正在录音，来电话。   松开手后来电界面消失。 
		       if(MMICC_IsExistIncommingCall()) 
		           MMK_CloseWin(win_id);	
			   
		       // bug: 设备微聊发送时接收视频来电，通话界面异常   //MMI_OpenVideoChatWin2 中来改。
		       //if (MMK_IsFocusWin(MMI_VIDEOCHAT_WIN_ID))
		          // MMK_CloseWin(MMI_VIDEOCHAT_WIN_ID);

		       // bug: 设备发送微聊时远程拍照，松手并按键亮屏后，微聊自动倒计时；//MMIAPIDC_OpenPhotoWinForYX 中修改

		       // bug: 设备发送微聊时接收闹钟提醒，闹钟提醒界面不显示，闹钟铃声响起，微聊界面无法发送微聊，返回后可恢复 // OpenAlarmExpiredWin 中修改		   

		       // bug: 设备发送微聊时监听设备，通话结束后，设备一段时间无法发送微聊；松手后显示10s；//在微聊的时候不允许监听，mic被占用
			   
		   #endif
            }
        break;
        
    case MSG_GET_FOCUS:
    case MSG_FULL_PAINT:
        MMIZDT_TinyShowBottom(win_id);
        if(tiny_chat_is_recording == 0)
        {
            MMIZDT_TinyChatUpdateList();
        }
        break;
        
    
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //AiChatHandleRecordListAppendData (win_id, param, ctrl_id, 2);
        break;
 
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_chat_tp_x = point.x;
            s_chat_tp_y = point.y;
            s_is_chat_tp_down = TRUE;
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            int offset_y =  0;//up to down
            int offset_x =  0;//up to down
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_is_chat_tp_long)
            {
                /*MMI_StopAiChatRecord();
                
                MMIZDT_StopRecordAnimTimer();
                ZDT_LOG("ai_chat MMK_StopTimer");
                MMIZDT_ClearTinyChatRecordAnim(win_id);
                AiUpdateRecordListData (win_id, ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, TXT_NULL, TRUE);
                MMK_UpdateScreen();*/
                MMIZDT_StopRecordAnimTimer();
                MMIZDT_ClearTinyChatRecordAnim(win_id);
                MMK_UpdateScreen();
                MMIZDT_StopRecordMaxTimeTimer();
                //AI_API_Record_Stop();
                //AI_TCPSendAIRecordFile(); // NOT really stop
                YX_API_Record_Stop();
                tiny_chat_is_recording = 0;
            }
            
            if(s_is_chat_tp_down && s_is_chat_tp_long == FALSE)
            {
                offset_y =  point.y - s_chat_tp_y;
                offset_x =  point.x - s_chat_tp_x;
                if(offset_x <= -(20))	
                {
                    MMK_CloseWin(win_id);
                }	
                else if(offset_x >= (20))
                {
                    MMK_CloseWin(win_id);
                }
            }
            s_is_chat_tp_long = FALSE;
            s_is_chat_tp_down = FALSE;
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_is_chat_tp_down && s_is_chat_tp_long == FALSE)
            {
                if(point.y > TINYCHAT_LIS_BOX_BOTTOM)
                {
                    if(ZDT_SIM_Exsit() == FALSE)
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,STR_SIM_NOT_SIM_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);//TXT_SIM_REJECTED
                        break;
                    }
                    if (MMIAPICONNECTION_isGPRSSwitchedOff())
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                        break;
                    }
                    if(yx_DB_Set_Rec.net_open == 0)
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_OPEN,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                        break;
                    }
                    if(ZDT_IsMemNearFull(FALSE))
        		    {
        		        ZDT_LOG("win ZDT_IsMemNearFull");
                        MMIPUB_OpenAlertWarningWin (STR_NO_SPACE_EXT01);
                        break;
        		    }
                    s_is_key_green_down = 1;
                    /*record_result = StartAiChatRecord();
                    if(record_result ==MMIRECORD_SRV_RESULT_SUCCESS )
                    {
                        tiny_chat_record_update_anim_index = 0;
                        tiny_chat_record_anim_timer_id =  MMK_CreateWinTimer(win_id, TINYCHAT_RECORD_ANIM_DELAY,TRUE);
                        MMIZDT_DisplayTinyChatRecordAnim(win_id);
                        return record_result;   
                    }   */
                    if(MMIZDT_Net_IsInit() && FALSE == YX_Net_Is_Land())
                    {
                        ZDT_LOG("win TINY_CHAT NET Reset");
                        MMIZDT_Net_Reset(FALSE);
                    }
                    ZDT_LOG("win TINY_CHAT_Record_Start");
                    tiny_chat_record_update_anim_index = 0;
                    tiny_chat_record_timer_index = 0;
                    s_is_chat_tp_long = TRUE;
                    MMIZDT_TinyChatStopPlayAudio();
                    MMIZDT_StartRecordAnimTimer();
                    MMIZDT_DisplayTinyChatRecordAnim(win_id);
                    MMIZDT_StartRecordMaxTimeTimer();
                    //AI_API_Record_Start();
                    YX_API_Record_Start();
                    tiny_chat_is_recording = 1;
                }
            }
            
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_KEYLONG_WEB:    
    {
        ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_KEYLONG_WEB");
        if(ZDT_SIM_Exsit() == FALSE)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,STR_SIM_NOT_SIM_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);//TXT_SIM_REJECTED
            break;
        }
        if (MMIAPICONNECTION_isGPRSSwitchedOff())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
        if(yx_DB_Set_Rec.net_open == 0)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_OPEN,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
        s_is_key_green_down = 1;
        /*record_result = StartAiChatRecord();
        if(record_result ==MMIRECORD_SRV_RESULT_SUCCESS )
        {
            tiny_chat_record_update_anim_index = 0;
            tiny_chat_record_anim_timer_id =  MMK_CreateWinTimer(win_id, TINYCHAT_RECORD_ANIM_DELAY,TRUE);
            MMIZDT_DisplayTinyChatRecordAnim(win_id);
            return record_result;   
        }   */
        ZDT_LOG("win TINY_CHAT_Record_Start");
        tiny_chat_record_update_anim_index = 0;
        tiny_chat_record_timer_index = 0;
        s_is_chat_tp_long = TRUE;
        MMIZDT_TinyChatStopPlayAudio();
        MMIZDT_StartRecordAnimTimer();
        MMIZDT_DisplayTinyChatRecordAnim(win_id);
        MMIZDT_StartRecordMaxTimeTimer();
        //AI_API_Record_Start();
        YX_API_Record_Start();
        tiny_chat_is_recording = 1;
    }
    break;
    
    case MSG_KEYPRESSUP_WEB:
    {
            ZDT_LOG("MSG_KEYPRESSUP_WEB");
            if(s_is_key_green_down)
            {
                /*MMI_StopAiChatRecord();
                
                MMIZDT_StopRecordAnimTimer();
                ZDT_LOG("ai_chat MMK_StopTimer");
                MMIZDT_ClearTinyChatRecordAnim(win_id);
                AiUpdateRecordListData (win_id, ctrl_id, MMIFMM_READ_TYPE_FILE_DIR, TXT_NULL, TRUE);
                MMK_UpdateScreen();*/
                MMIZDT_StopRecordAnimTimer();
                MMIZDT_ClearTinyChatRecordAnim(win_id);
                MMK_UpdateScreen();
                MMIZDT_StopRecordMaxTimeTimer();
                //AI_API_Record_Stop();
                //AI_TCPSendAIRecordFile(); // NOT really stop
                YX_API_Record_Stop();
                tiny_chat_is_recording = 0;
                s_is_chat_tp_long = FALSE;
                s_is_key_green_down = 0;
            }
            else
            {
                MMK_CloseWin(win_id);
            }
            
    }        
    break;
    
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    //case MSG_KEYDOWN_WEB:
        index = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, index, &pos_user_data);// user_data stand position
        MMIZDT_TinyChatStartPlayAudio(pos_user_data, index);
        ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_CTL_MIDSK  index=%d, pos_user_data=%d" , index, pos_user_data);

        break;
                
	case MSG_TIMER:
	    ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_TIMER");
		if (*(uint8*)param == tiny_chat_record_anim_timer_id)
		{
		    if(tiny_chat_record_update_anim_index < TINYCHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM)
		    {
		        tiny_chat_record_update_anim_index++;
		    }
                else
                {
                    tiny_chat_record_update_anim_index = 0;
                }
                tiny_chat_record_timer_index++;
		    MMIZDT_DisplayTinyChatRecordAnim(win_id);
		   ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_TIMER   tiny_chat_record_timer_index=%d",tiny_chat_record_timer_index);	
		}
        else if (*(uint8*)param == tiny_chat_max_record_timer_id)
        {
            MMIZDT_StopRecordAnimTimer();
            MMIZDT_ClearTinyChatRecordAnim(win_id);
            //MMIZDT_StopRecordMaxTimeTimer(); 
            //AI_API_Record_Stop();
            //AI_TCPSendAIRecordFile();
            YX_API_Record_Stop();
	      ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_TIMER   param is tiny_chat_max_record_timer_id");	

	     #if 1 //xiongkai 	 for bug 设备端微聊界面发送10s语音，未及时显示语音条记录；需要退出重进刷新；	
            tiny_chat_is_recording = 0;	 
            if (MMK_IsFocusWin(win_id))
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }	
	     #endif 		
        }
        else
        {
            recode = MMI_RESULT_FALSE;
	     ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_TIMER   otherwise");					
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_APP_CANCEL");
        
        MMK_CloseWin(win_id);
        break;
        //yangyu add
    case MSG_KEYUP_RED:
        recode = MMI_RESULT_TRUE;//for huami ,not turnoff lcd , just go to idle win
        break;
//yangyu end

    case MSG_CLOSE_WINDOW:
        MMISRVAUD_FreeVirtualHandle("TINY CHAT"); 
        //MMIZDT_StopRecordAnimTimer();
        //AI_Tcp_ForceCloseAll();//close tcp socket and thread
        //AI_API_Rcv_Mp3_PlayStop();//stop play, clear buf list
        MMIZDT_StopRecordAnimTimer();
        MMIZDT_TinyChatStopPlayAudio();

        YX_Voice_HandleStop(pMe);
        //YX_Net_Send_TK_VocFile_End(pMe,YX_VOCSEND_ERR_UNREG);
        tiny_chat_is_recording = 0;
        ZdtTalk_BackLight(FALSE);
        if(p_index != PNULL)
        {
            SCI_FREE(p_index);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

PUBLIC void ZDT_Close_Tiny_Chat_Win()
{
	 if(tiny_chat_is_recording)
	 {
		 MMIZDT_StopRecordAnimTimer();
		 MMIZDT_ClearTinyChatRecordAnim(MMIZDT_TINY_CHAT_WIN_ID);
		 MMK_UpdateScreen();
		 MMIZDT_StopRecordMaxTimeTimer();
		 YX_API_Record_Stop();
		 tiny_chat_is_recording = 0;	
	 }
	 
	 if(MMK_IsOpenWin(MMIZDT_TINY_CHAT_WIN_ID))
	 {
		 MMK_CloseWin(MMIZDT_TINY_CHAT_WIN_ID);
	 }
}
#endif //end chat


#if 1

LOCAL MMI_RESULT_E HandleZDT_BHMainMenuWinMsg (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      );
LOCAL void ZDTSET_CommonTextId(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}

LOCAL void  SetBHOnOffListCode( MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint16 cur_selection =0;
    GUILIST_SetMaxItem( ctrl_id, 2, FALSE );//max item 5
    GUILIST_SetMaxSelectedItem(ctrl_id,2);
    //append item
    ZDTSET_CommonTextId(TXT_COMM_OPEN,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    ZDTSET_CommonTextId(STXT_EXIT,TXT_COMMON_OK,TXT_NULL,STXT_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
    if(yx_DB_Set_Rec.net_open > 0)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }

    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
    GUILIST_SetSelectedItem(ctrl_id,cur_selection,TRUE);

}

LOCAL void SetBHOnOffType(void)
{
    uint16 cur_select = 0;
    cur_select =(uint16)GUILIST_GetCurItemIndex(MMIZDT_YX_ONOFF_CTRL_ID);   ///GUISETLIST_GetCurIndex(MMISET_TORCH_SETLIST_CTRL_ID);
    if(cur_select == 0)
    {
        YX_API_SeNetOpen();
    }
    else
    {
        YX_API_SeNetClose();
    }
}
LOCAL MMI_RESULT_E HandleZDT_BHOnOffWinMsg(
        MMI_WIN_ID_T       win_id,
        MMI_MESSAGE_ID_E   msg_id,
        DPARAM             param
        )
{
   
    MMI_RESULT_E	 recode = MMI_RESULT_TRUE;
    static uint16		  cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id  = MMIZDT_YX_ONOFF_CTRL_ID;
    uint16					 max_item = 2; 

	switch (msg_id)
	{
        case MSG_OPEN_WINDOW:
        	  SetBHOnOffListCode(ctrl_id);

        	  MMK_SetAtvCtrl(win_id, MMIZDT_YX_ONOFF_CTRL_ID);
		break;
			
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
		SetBHOnOffType();
		MMK_CloseWin(win_id);
		break;		  
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_RED:
		recode = MMI_RESULT_FALSE;
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
   
}


WINDOW_TABLE(MMIZDT_BH_ONOFF_WIN_TAB ) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)HandleZDT_BHOnOffWinMsg),
        //win_id
        WIN_ID(MMIZDT_BH_ONOFF_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_BH_NET_ONOFF),
        //创建  控件
       CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIZDT_YX_ONOFF_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //   WIN_TIPS,
	 END_WIN

};
PUBLIC void MMIZDT_OpenOnOffWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_BH_ONOFF_WIN_TAB, PNULL);
}


#if 1
#define QRCODE_FILE_NAME  L"D:\\Others\\ewm.bmp"
#define QRCODE_DOWNLOAD_FILE_NAME  L"D:\\Others\\ewm_download.bmp"
#define QRCODE_BIND_FILE_NAME  L"D:\\Others\\ewm_bind.bmp"
#define QRCODE_MANUAL_FILE_NAME  L"D:\\Others\\ewm_manual.bmp"



#define QRCODE_DISP_X	40
#define QRCODE_DISP_Y	40
#define QRCODE_DISP_WIDTH	160
#define QRCODE_DISP_HEIGHT	160

#define ZOOM_ALIGN(x,align)     (x/align * align)

//zoom in param
typedef struct 
{
    uint8           *src_buf_ptr;       //src buffer
    uint32          src_buf_size;       //src buffer size
    uint8           *dest_buf_ptr;      //dest buffer
    uint32          dest_buf_size;      //dest buffer size

    uint16          src_img_width;      //src image width
    uint16          src_img_height;     //src image height
    uint16          dest_img_width;     //dest image width
    uint16          dest_img_height;    //dest image height
    //GUI_RECT_T      clip_rect;          //clip rect
} ZOOM_INPUT_T;

//scale out param
typedef struct 
{
    uint16      img_width;      //image width
    uint16      img_height;     //image height
    uint32      dest_buf_offset;//dest buffer必须256对齐
} ZOOM_OUTPUT_T;

typedef enum
{
    ZOOM_RESULT_SUCC,                    /*!<成功*/
    ZOOM_RESULT_FAIL,                    /*!<失败*/
    ZOOM_RESULT_NO_SUPPORT,              /*!<不支持*/
    ZOOM_RESULT_MAX                      /*!<保留位*/
} ZOOM_RESULT_E;

static int s_2vm_tp_x;
static int s_2vm_tp_y;
static BOOLEAN s_is_2vm_tp_down = FALSE;

LOCAL ZOOM_RESULT_E ZoomImage(
                      ZOOM_INPUT_T  *zoom_in_ptr,   //in:
                      ZOOM_OUTPUT_T *zoom_out_ptr   //in/out:
                      )
{
    uint32              scale_result = 0;
    ZOOM_RESULT_E    result = ZOOM_RESULT_FAIL;
    SCALE_IMAGE_IN_T    scale_in = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};

    //SCI_ASSERT(PNULL != zoom_in_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != zoom_out_ptr); /*assert verified*/
    if(PNULL == zoom_in_ptr ||PNULL == zoom_out_ptr )
    {
        //SCI_TRACE_LOW:"BrwZoomImage zoom_in_ptr || zoom_out_ptr is null"
        return result;
    }
    //set src width and height
    scale_in.src_size.w = zoom_in_ptr->src_img_width;
    scale_in.src_size.h = zoom_in_ptr->src_img_height;

    //set target width and height
    scale_in.target_size.w = zoom_in_ptr->dest_img_width;
    scale_in.target_size.h = zoom_in_ptr->dest_img_height;

    //w 4对齐，h 2对齐
    scale_in.target_size.w = (uint16)ZOOM_ALIGN(scale_in.target_size.w,4);
    scale_in.target_size.h = (uint16)ZOOM_ALIGN(scale_in.target_size.h,2);

    //原图裁剪
    scale_in.src_trim_rect.x = 0;
    scale_in.src_trim_rect.y = 0;
    scale_in.src_trim_rect.w = zoom_in_ptr->src_img_width;
    scale_in.src_trim_rect.h = zoom_in_ptr->src_img_height;

    //set src format
    scale_in.src_format = IMGREF_FORMAT_RGB565;

    //set src buffer
    scale_in.src_chn.chn0.ptr = zoom_in_ptr->src_buf_ptr;
    scale_in.src_chn.chn0.size = zoom_in_ptr->src_buf_size;

    //set target format
    scale_in.target_format = IMGREF_FORMAT_RGB565;

    //set target buffer
    scale_in.target_buf.ptr  = zoom_in_ptr->dest_buf_ptr;
    scale_in.target_buf.size = zoom_in_ptr->dest_buf_size;

    //scale
    scale_result = GRAPH_ScaleImage(&scale_in,&scale_out);
    //BRWDebug_Printf:"[BRW]BrwZoomImage:zoom result is %d!"
    switch (scale_result)
    {
    case SCI_SUCCESS:
        //set image width and height
        zoom_out_ptr->img_width  = scale_out.output_size.w;
        zoom_out_ptr->img_height = scale_out.output_size.h;

        //set target buffer offset
        zoom_out_ptr->dest_buf_offset = (uint32)(scale_out.output_chn.chn0.ptr) - (uint32)(scale_in.target_buf.ptr);
        result = ZOOM_RESULT_SUCC;
        break;
    
    case GRAPH_ERR_SIZE_UNSUPPORT:
        result = ZOOM_RESULT_NO_SUPPORT;
        break;

    default:
        result = ZOOM_RESULT_FAIL;
        break;
    }

    return (result);
}

PUBLIC BOOLEAN MMIZDT_CheckImeiChange(void)
{
    if(strncmp( (char *)yx_DB_Set_Rec.imei, g_zdt_phone_imei_1, 15 ) == 0)
    {
        return FALSE;
    }
    return TRUE;
}

PUBLIC BOOLEAN MMIZDT_SaveImei(void)
{
    SCI_MEMSET(yx_DB_Set_Rec.imei,0,16);
    SCI_MEMCPY(yx_DB_Set_Rec.imei,g_zdt_phone_imei_1,15);
    YX_DB_SET_ListModify();
    return TRUE;
}

BOOLEAN  MMIZDT_Check2VMFile()
{
    //http://4g.watch.ecellsz.com:8080/app/newindex.html?imei=
    char ewm_str[200] = {0};
    if(MMIZDT_CheckImeiChange())
    {
        //UserFile_DeleteU("Others","ewm.bmp");
        UserFile_DeleteU("Others","ewm_bind.bmp");
    }
    #if 0 //yangyu delete ,
    //if(!MMIFILE_IsFileExist(QRCODE_FILE_NAME,MMIAPICOM_Wstrlen(QRCODE_FILE_NAME)))
    if(!MMIFILE_IsFileExist(QRCODE_DOWNLOAD_FILE_NAME,MMIAPICOM_Wstrlen(QRCODE_BIND_FILE_NAME)))
    {
        //sprintf(ewm_str,"http://%s:8080/app/newindex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
        sprintf(ewm_str,"http://%s:8080/app/newIndex.html",yx_DB_Set_Rec.app_domain);
#ifndef WIN32
        if(0 == Save_QEImage(ewm_str , 0))
        {
            //MMIZDT_SaveImei();
        }
#endif
    }	
    #endif

    if(!MMIFILE_IsFileExist(QRCODE_BIND_FILE_NAME,MMIAPICOM_Wstrlen(QRCODE_BIND_FILE_NAME)))
    {
        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
       // sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
	 sprintf(ewm_str,"%s",g_zdt_phone_imei_1);	
#ifndef WIN32
        //if(0 == Save_QEImage(ewm_str, 1))//yangyu delete
        {
            MMIZDT_SaveImei();
        }
#endif
    }	
    #if 0 //yangyu delete , manual use new win
     if(!MMIFILE_IsFileExist(QRCODE_MANUAL_FILE_NAME, MMIAPICOM_Wstrlen(QRCODE_MANUAL_FILE_NAME)))
    {
        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
        sprintf(ewm_str,"https://share.weiyun.com/vQRtnw4K");
#ifndef WIN32
        if(0 == Save_QEImage(ewm_str, 2))
        {
            //MMIZDT_SaveImei();
        }
#endif
    }	
    #endif

    return TRUE;
}
LOCAL MMI_RESULT_E  HandleZDT_BH2VMWinMsg(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) 
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_CALL_BACK_T     call_back = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    int w,h;
    uint8*   pszRGBData=NULL;
    uint32  img_handle = 0;
    IMG_DEC_FRAME_IN_PARAM_T    dec_in = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   dec_out = {0};
    GUIIMG_BITMAP_T  bitmap_handle = {0};
    GUI_POINT_T                 dis_point = {0};
    ZOOM_INPUT_T    zoom_in_data = {0};
    ZOOM_OUTPUT_T   zoom_out_data = {0};
    BOOLEAN is_zoom_success = FALSE;	
    uint8 sn_str[24 + 1] = {0};
    uint16 sn_len = 0;
    MMI_STRING_T   sn_s = {0};
    uint16 uint16_str_len = 0;
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    GUISTR_INFO_T       text_info = {0};
    MMI_STRING_T sn_string = {0};
    char ewm_str[100 + 1] = {0};
	
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
            MMIZDT_Check2VMFile();
            dec_src.file_name_ptr = QRCODE_FILE_NAME;
            if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
            {	
                return FALSE;
            }		
            w=dec_info.img_detail_info.bmp_info.img_width;
            h=dec_info.img_detail_info.bmp_info.img_height;
	    if(w==0 && h ==0)
	    {
		dis_point.x=QRCODE_DISP_X;
	    	dis_point.y=QRCODE_DISP_Y; 
	    	bg_rect.left =QRCODE_DISP_X;
	    	bg_rect.right=QRCODE_DISP_X + QRCODE_DISP_WIDTH;
	    	bg_rect.top=QRCODE_DISP_Y;
	    	bg_rect.bottom=QRCODE_DISP_Y+QRCODE_DISP_HEIGHT;
            #if 0
	    	GUIRES_DisplayImg(&dis_point,
                &bg_rect,
                PNULL,
                win_id,
                IMAGE_DEFAULT_EWM,
                &lcd_dev_info);
            #endif
	    }
	    pszRGBData=(uint8*)SCI_ALLOC_APP(w*h*2);
	    if(NULL == pszRGBData)
	    {
	        return FALSE;
	    }

	    if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle))
	    {
	        SCI_FREE(pszRGBData);
	        return FALSE;
	    }		

	    dec_in.handle = img_handle;
	    //set decode target
	    dec_in.is_dec_thumbnail = 0;
	    dec_in.target_ptr       = pszRGBData;
	    dec_in.target_buf_size  = w*h*2;
	    dec_in.target_width     = w;
	    dec_in.target_height    = h;
	    dec_in.img_rect.left    = 0;
	    dec_in.img_rect.top     = 0;
	    dec_in.img_rect.right   = w-1;
	    dec_in.img_rect.bottom  = h-1;
	    dec_in.data_format = IMG_DEC_RGB565;

	    //图形解码
	    if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetFrame(&dec_in,&dec_out))
	    {
	        IMG_DEC_Destroy(img_handle);
	        SCI_FREE(pszRGBData);
	        return FALSE;
	    }
            zoom_in_data.src_buf_ptr = dec_in.target_ptr;
            zoom_in_data.src_buf_size = dec_in.target_buf_size;
            zoom_in_data.src_img_width = dec_out.real_width;
            zoom_in_data.src_img_height= dec_out.real_height;
            zoom_in_data.dest_img_width = QRCODE_DISP_WIDTH;
            zoom_in_data.dest_img_height = QRCODE_DISP_HEIGHT;
            zoom_in_data.dest_buf_size = (zoom_in_data.dest_img_width * zoom_in_data.dest_img_height * 2 + 255);
            zoom_in_data.dest_buf_ptr = SCI_ALLOCA(zoom_in_data.dest_buf_size);
			
            if (PNULL != zoom_in_data.dest_buf_ptr)
            {
                if (ZOOM_RESULT_SUCC == ZoomImage(&zoom_in_data, &zoom_out_data))
                {
                    is_zoom_success = TRUE;
                }
                else
                {
                    is_zoom_success = FALSE;
                }
            }
            else
            {
                is_zoom_success = FALSE;
            }
			
	    IMG_DEC_Destroy(img_handle);

	   
	    bitmap_handle.bit_ptr =  (zoom_in_data.dest_buf_ptr + zoom_out_data.dest_buf_offset);;
	    bitmap_handle.img_width  = QRCODE_DISP_WIDTH; //dec_out.real_width;
	    bitmap_handle.img_height = QRCODE_DISP_HEIGHT; //dec_out.real_height;
	    
	    dis_point.x=QRCODE_DISP_X; //lpRC->left;
	    dis_point.y=QRCODE_DISP_Y; //lpRC->top;	    

	    bg_rect.left =QRCODE_DISP_X;
	    bg_rect.right=QRCODE_DISP_X + QRCODE_DISP_WIDTH;
	    bg_rect.top=QRCODE_DISP_Y;
	    bg_rect.bottom=QRCODE_DISP_Y+QRCODE_DISP_HEIGHT;	
		
	    GUIIMG_DisplayBmp(FALSE, &bg_rect, &dis_point, &bitmap_handle,&lcd_dev_info);

            if (PNULL != zoom_in_data.dest_buf_ptr)
            {
                SCI_FREE(zoom_in_data.dest_buf_ptr);
                zoom_in_data.dest_buf_ptr = PNULL;
            }
			
	    SCI_FREE(pszRGBData);						
            break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            int offset_y =  0;//up to down
            int offset_x =  0;//up to down
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
			
	case MSG_FULL_PAINT:
        	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BH_NET_2VM_INFO,&sn_string);
	        text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
	        text_style.font_color = MMI_BLACK_COLOR;
	        text_style.angle = ANGLE_0;
	        text_style.char_space = 0;
	        text_style.effect = FONT_EFFECT_CUSTOM;
	        text_style.edge_color = 0;

	        text_style.font = SONG_FONT_24;
	        text_style.line_space = 0;
	        text_style.region_num = 0;
	        text_style.region_ptr = PNULL;		
	        text_display_rect.left = 0;
	        text_display_rect.top = 40;
	        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
	        text_display_rect.bottom=80;	
			
	        GUISTR_GetStringInfo(&text_style, &sn_string, state, &text_info);
		GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
		        (const MMI_STRING_T    *)&sn_string,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );		
		break;
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_CTL_OK:
        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}


//Qrcode 二维码



WINDOW_TABLE( MMIZDT_BH_2VM_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleZDT_BH2VMWinMsg ),
    WIN_ID(MMIZDT_BH_2VM_WIN_ID),
    WIN_TITLE(TXT_BH_NET_2VM),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),	
    //CREATE_ANIM_CTRL (MMISET_QRCODE_CTRL_ID, MMISET_SET_QRCODE_WIN_ID),
    END_WIN
};

PUBLIC void MMIZDT_BH_Open2VMWin(void)
{
    MMK_CreateWin((uint32 *)MMIZDT_BH_2VM_WIN_TAB,PNULL);
}

LOCAL uint8 qrcode_page_index = 0;

LOCAL uint16 unbind_click_count = 0;


LOCAL MMI_RESULT_E  HandleZDT_2VMWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    BOOLEAN                 result = FALSE;
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    int w,h;
    uint8*   pszRGBData=NULL;
    uint32  img_handle = 0;
    IMG_DEC_FRAME_IN_PARAM_T    dec_in = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   dec_out = {0};
    GUIIMG_BITMAP_T  bitmap_handle = {0};
    GUI_POINT_T                 dis_point = {0};
    ZOOM_INPUT_T    zoom_in_data = {0};
    ZOOM_OUTPUT_T   zoom_out_data = {0};
    BOOLEAN is_zoom_success = FALSE;	
    uint16 uint16_str[22] = {0};
    uint16 uint16_str_len = 0;
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    GUISTR_INFO_T       text_info = {0};
    MMI_STRING_T sn_string = {0};
    MMI_STRING_T imei_string = {0};
    char*           temp_ptr = PNULL;
    uint16 temp_len = 0;
    MMI_STRING_T    cr_s = {0};
    wchar           unicode_cr = 0x0d;
    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            
            GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
            MMIZDT_Check2VMFile();
            unbind_click_count = 0;
            //dec_src.file_name_ptr = QRCODE_FILE_NAME;
            if(qrcode_page_index == 0)
            {
                uint16 img_w = 180;
                dec_src.file_name_ptr = QRCODE_DOWNLOAD_FILE_NAME;
                //yangyu add, not generate
                dis_point.x= (MMI_MAINSCREEN_WIDTH - img_w)/2;
    	    	dis_point.y= (MMI_MAINSCREEN_HEIGHT- img_w)/2; 
    	    	bg_rect.left = dis_point.x;
    	    	bg_rect.right= bg_rect.left + img_w;
    	    	bg_rect.top = dis_point.y;
    	    	bg_rect.bottom = bg_rect.top + img_w;
                GUIRES_DisplayImg(&dis_point,
                                &bg_rect,
                                PNULL,
                                win_id,
                                IMAGE_ZDT_JP_BG,
                                &lcd_dev_info);
                break;

                //yangyu end

            }
            else if(qrcode_page_index == 1)
            {
                char ewm_str[200] = {0};
                uint16 large = 20;
                SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
		  sprintf(ewm_str,"%s",g_zdt_phone_imei_1);		
               // sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
                dec_src.file_name_ptr = QRCODE_BIND_FILE_NAME;
                
                //yangyu add later 
                GT_Qcode_show(ewm_str, QRCODE_DISP_X-large/2, QRCODE_DISP_Y-large/2-6, QRCODE_DISP_WIDTH+large, QRCODE_DISP_HEIGHT+large);
                break;
                //yangyu end

            }
            else if(qrcode_page_index == 2)
            {
                dec_src.file_name_ptr = QRCODE_MANUAL_FILE_NAME;
                //yangyu add later, for manual qrcode ,just use res image,not generate
                GUIRES_DisplayImg(&dis_point,
                &bg_rect,
                PNULL,
                win_id,
                IMAGE_ZDT_JP_BG,
                &lcd_dev_info);
                break;
                //yangyu end

            }
            if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
            {	
                return FALSE;
            }		
            w=dec_info.img_detail_info.bmp_info.img_width;
            h=dec_info.img_detail_info.bmp_info.img_height;
	    if(w==0 && h ==0)
	    {
		dis_point.x=QRCODE_DISP_X;
	    	dis_point.y=QRCODE_DISP_Y; 
	    	bg_rect.left =QRCODE_DISP_X;
	    	bg_rect.right=QRCODE_DISP_X + QRCODE_DISP_WIDTH;
	    	bg_rect.top=QRCODE_DISP_Y;
	    	bg_rect.bottom=QRCODE_DISP_Y+QRCODE_DISP_HEIGHT;
	    	GUIRES_DisplayImg(&dis_point,
                &bg_rect,
                PNULL,
                win_id,
                IMAGE_ZDT_JP_BG,
                &lcd_dev_info);
	    }
	    pszRGBData=(uint8*)SCI_ALLOC_APP(w*h*2);
	    if(NULL == pszRGBData)
	    {
	        return FALSE;
	    }

	    if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle))
	    {
	        SCI_FREE(pszRGBData);
	        return FALSE;
	    }		

	    dec_in.handle = img_handle;
	    //set decode target
	    dec_in.is_dec_thumbnail = 0;
	    dec_in.target_ptr       = pszRGBData;
	    dec_in.target_buf_size  = w*h*2;
	    dec_in.target_width     = w;
	    dec_in.target_height    = h;
	    dec_in.img_rect.left    = 0;
	    dec_in.img_rect.top     = 0;
	    dec_in.img_rect.right   = w-1;
	    dec_in.img_rect.bottom  = h-1;
	    dec_in.data_format = IMG_DEC_RGB565;

	    //图形解码
	    if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetFrame(&dec_in,&dec_out))
	    {
	        IMG_DEC_Destroy(img_handle);
	        SCI_FREE(pszRGBData);
	        return FALSE;
	    }
            zoom_in_data.src_buf_ptr = dec_in.target_ptr;
            zoom_in_data.src_buf_size = dec_in.target_buf_size;
            zoom_in_data.src_img_width = dec_out.real_width;
            zoom_in_data.src_img_height= dec_out.real_height;
            zoom_in_data.dest_img_width = QRCODE_DISP_WIDTH;
            zoom_in_data.dest_img_height = QRCODE_DISP_HEIGHT;
            zoom_in_data.dest_buf_size = (zoom_in_data.dest_img_width * zoom_in_data.dest_img_height * 2 + 255);
            zoom_in_data.dest_buf_ptr = SCI_ALLOCA(zoom_in_data.dest_buf_size);
			
            if (PNULL != zoom_in_data.dest_buf_ptr)
            {
                if (ZOOM_RESULT_SUCC == ZoomImage(&zoom_in_data, &zoom_out_data))
                {
                    is_zoom_success = TRUE;
                }
                else
                {
                    is_zoom_success = FALSE;
                }
            }
            else
            {
                is_zoom_success = FALSE;
            }
			
	    IMG_DEC_Destroy(img_handle);

	   
	    bitmap_handle.bit_ptr =  (zoom_in_data.dest_buf_ptr + zoom_out_data.dest_buf_offset);;
	    bitmap_handle.img_width  = QRCODE_DISP_WIDTH; //dec_out.real_width;
	    bitmap_handle.img_height = QRCODE_DISP_HEIGHT; //dec_out.real_height;
	    
	    dis_point.x=QRCODE_DISP_X; //lpRC->left;
	    dis_point.y=QRCODE_DISP_Y; //lpRC->top;	    

	    bg_rect.left =QRCODE_DISP_X;
	    bg_rect.right=QRCODE_DISP_X + QRCODE_DISP_WIDTH;
	    bg_rect.top=QRCODE_DISP_Y;
	    bg_rect.bottom=QRCODE_DISP_Y+QRCODE_DISP_HEIGHT;	
		
	    GUIIMG_DisplayBmp(FALSE, &bg_rect, &dis_point, &bitmap_handle,&lcd_dev_info);

            if (PNULL != zoom_in_data.dest_buf_ptr)
            {
                SCI_FREE(zoom_in_data.dest_buf_ptr);
                zoom_in_data.dest_buf_ptr = PNULL;
            }
			
	    SCI_FREE(pszRGBData);	
	    
            break;
    case MSG_GET_FOCUS:
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
                    MMIZDT_Check2VMFile();
                    //dec_src.file_name_ptr = QRCODE_FILE_NAME;
                    if(qrcode_page_index == 0)
                    {
                        uint16 img_w = 180*SCALE;
                        dec_src.file_name_ptr = QRCODE_DOWNLOAD_FILE_NAME;
                        //yangyu add, not generate
                        dis_point.x= (MMI_MAINSCREEN_WIDTH - img_w)/2;
                        dis_point.y= (MMI_MAINSCREEN_HEIGHT- img_w)/2; 
                        bg_rect.left = dis_point.x;
                        bg_rect.right= bg_rect.left + img_w;
                        bg_rect.top = dis_point.y;
                        bg_rect.bottom = bg_rect.top + img_w;
                        GUIRES_DisplayImg(&dis_point,
                                        &bg_rect,
                                        PNULL,
                                        win_id,
                                        IMAGE_ZDT_JP_BG,
                                        &lcd_dev_info);
                        break;
        
                        //yangyu end
        
                    }
                    else if(qrcode_page_index == 1)
                    {
                        char ewm_str[200] = {0};
                        uint16 large = 20;
                        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
                       // sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
			   sprintf(ewm_str,"%s",g_zdt_phone_imei_1);			
                        dec_src.file_name_ptr = QRCODE_BIND_FILE_NAME;
                        
                        //yangyu add later 
                        GT_Qcode_show(ewm_str, QRCODE_DISP_X-large/2, QRCODE_DISP_Y-large/2-6, QRCODE_DISP_WIDTH+large, QRCODE_DISP_HEIGHT+large);
                        break;
                        //yangyu end
        
                    }
                    else if(qrcode_page_index == 2)
                    {
                        dec_src.file_name_ptr = QRCODE_MANUAL_FILE_NAME;
                        //yangyu add later, for manual qrcode ,just use res image,not generate
                        GUIRES_DisplayImg(&dis_point,
                        &bg_rect,
                        PNULL,
                        win_id,
                        IMAGE_ZDT_JP_BG,
                        &lcd_dev_info);
                        break;
                        //yangyu end
        
                    }
                    if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
                    {   
                        return FALSE;
                    }       
                    w=dec_info.img_detail_info.bmp_info.img_width;
                    h=dec_info.img_detail_info.bmp_info.img_height;
                if(w==0 && h ==0)
                {
                dis_point.x=QRCODE_DISP_X;
                    dis_point.y=QRCODE_DISP_Y; 
                    bg_rect.left =QRCODE_DISP_X;
                    bg_rect.right=QRCODE_DISP_X + QRCODE_DISP_WIDTH;
                    bg_rect.top=QRCODE_DISP_Y;
                    bg_rect.bottom=QRCODE_DISP_Y+QRCODE_DISP_HEIGHT;
                    GUIRES_DisplayImg(&dis_point,
                        &bg_rect,
                        PNULL,
                        win_id,
                        IMAGE_ZDT_JP_BG,
                        &lcd_dev_info);
                }
                pszRGBData=(uint8*)SCI_ALLOC_APP(w*h*2);
                if(NULL == pszRGBData)
                {
                    return FALSE;
                }
        
                if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle))
                {
                    SCI_FREE(pszRGBData);
                    return FALSE;
                }       
        
                dec_in.handle = img_handle;
                //set decode target
                dec_in.is_dec_thumbnail = 0;
                dec_in.target_ptr       = pszRGBData;
                dec_in.target_buf_size  = w*h*2;
                dec_in.target_width     = w;
                dec_in.target_height    = h;
                dec_in.img_rect.left    = 0;
                dec_in.img_rect.top     = 0;
                dec_in.img_rect.right   = w-1;
                dec_in.img_rect.bottom  = h-1;
                dec_in.data_format = IMG_DEC_RGB565;
        
                //图形解码
                if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetFrame(&dec_in,&dec_out))
                {
                    IMG_DEC_Destroy(img_handle);
                    SCI_FREE(pszRGBData);
                    return FALSE;
                }
                    zoom_in_data.src_buf_ptr = dec_in.target_ptr;
                    zoom_in_data.src_buf_size = dec_in.target_buf_size;
                    zoom_in_data.src_img_width = dec_out.real_width;
                    zoom_in_data.src_img_height= dec_out.real_height;
                    zoom_in_data.dest_img_width = QRCODE_DISP_WIDTH;
                    zoom_in_data.dest_img_height = QRCODE_DISP_HEIGHT;
                    zoom_in_data.dest_buf_size = (zoom_in_data.dest_img_width * zoom_in_data.dest_img_height * 2 + 255);
                    zoom_in_data.dest_buf_ptr = SCI_ALLOCA(zoom_in_data.dest_buf_size);
                    
                    if (PNULL != zoom_in_data.dest_buf_ptr)
                    {
                        if (ZOOM_RESULT_SUCC == ZoomImage(&zoom_in_data, &zoom_out_data))
                        {
                            is_zoom_success = TRUE;
                        }
                        else
                        {
                            is_zoom_success = FALSE;
                        }
                    }
                    else
                    {
                        is_zoom_success = FALSE;
                    }
                    
                IMG_DEC_Destroy(img_handle);
        
               
                bitmap_handle.bit_ptr =  (zoom_in_data.dest_buf_ptr + zoom_out_data.dest_buf_offset);;
                bitmap_handle.img_width  = QRCODE_DISP_WIDTH; //dec_out.real_width;
                bitmap_handle.img_height = QRCODE_DISP_HEIGHT; //dec_out.real_height;
                
                dis_point.x=QRCODE_DISP_X; //lpRC->left;
                dis_point.y=QRCODE_DISP_Y; //lpRC->top;     
        
                bg_rect.left =QRCODE_DISP_X;
                bg_rect.right=QRCODE_DISP_X + QRCODE_DISP_WIDTH;
                bg_rect.top=QRCODE_DISP_Y;
                bg_rect.bottom=QRCODE_DISP_Y+QRCODE_DISP_HEIGHT;    
                
                GUIIMG_DisplayBmp(FALSE, &bg_rect, &dis_point, &bitmap_handle,&lcd_dev_info);
        
                    if (PNULL != zoom_in_data.dest_buf_ptr)
                    {
                        SCI_FREE(zoom_in_data.dest_buf_ptr);
                        zoom_in_data.dest_buf_ptr = PNULL;
                    }
                    
                SCI_FREE(pszRGBData);

        break;
			
	case MSG_FULL_PAINT:
        	//MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BH_NET_2VM_INFO,&sn_string);
        	 if(qrcode_page_index == 0)
            {
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_NET_2VM_DOWNLOAD,&sn_string);

            }
            else if(qrcode_page_index == 1)
            {
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_NET_2VM_BIND,&sn_string);

            }
	        text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
	        text_style.font_color = MMI_BLACK_COLOR;
	        text_style.angle = ANGLE_0;
	        text_style.char_space = 0;
	        text_style.effect = FONT_EFFECT_CUSTOM;
	        text_style.edge_color = 0;

	        text_style.font = SONG_FONT_24;
	        text_style.line_space = 0;
	        text_style.region_num = 0;
	        text_style.region_ptr = PNULL;		
	        text_display_rect.left = 0;
	        text_display_rect.top = 0;
	        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
	        text_display_rect.bottom=39;	
			
	        GUISTR_GetStringInfo(&text_style, &sn_string, state, &text_info);
	        if(qrcode_page_index != 2) 
      		{  
      		#if 0 //yangyu delete ,
    		GUISTR_DrawTextToLCDInRect( 
    		        MMITHEME_GetDefaultLcdDev(),
    		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
    		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
    		        (const MMI_STRING_T    *)&sn_string,
    		        &text_style,
    		        state,
    		        GUISTR_TEXT_DIR_AUTO
    		        );
	        #endif
	        }
        
	        text_style.font = SONG_FONT_24;
	        text_style.line_space = 0;
	        text_style.region_num = 0;
	        text_style.region_ptr = PNULL;		
	        text_display_rect.left = 0;
	        text_display_rect.top = QRCODE_DISP_Y+QRCODE_DISP_HEIGHT+1;
	        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
	        text_display_rect.bottom=(MMI_MAINSCREEN_WIDTH -1);
                temp_ptr = (char*)g_zdt_phone_imei_1;
                temp_len = SCI_STRLEN((char*)temp_ptr);
                MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
                MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_str_len, &cr_s);//换行
                imei_string.wstr_ptr = uint16_str;
                imei_string.wstr_len = uint16_str_len;

      		if(qrcode_page_index == 1) 
      		{
      		
      		    GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
      		        (const MMI_STRING_T    *)&imei_string,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );
	        
	        }
		break;
		
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_2vm_tp_x = point.x;
            s_2vm_tp_y = point.y;
            s_is_2vm_tp_down = TRUE;
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            int offset_y =  0;//up to down
            int offset_x =  0;//up to down
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_is_2vm_tp_down)
            {
                offset_y =  point.y - s_2vm_tp_y;
                offset_x =  point.x - s_2vm_tp_x;
                if(offset_x <= -(20))	
                {
                    MMK_CloseWin(win_id);
                }	
                else if(offset_x >= (20))
                {
                    MMK_CloseWin(win_id);
                }
                //yangyu add for unbind
                if(point.x > MMI_MAINSCREEN_WIDTH/3 && point.y > MMI_MAINSCREEN_HEIGHT/3)
                {
                    unbind_click_count++;
                    if(unbind_click_count >= 18)
                    {
                        unbind_click_count = 0;
                        //YX_API_DLT_Send();
                    }
                }
                else
                {
                    unbind_click_count = 0;
                }
                //yangyu end

            }
            s_is_2vm_tp_down = FALSE;
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
        case MSG_APP_WEB:
                //ZDT_SetIdleWifiIcon(TRUE);
                //MAIN_SetIdleRoamingState(MN_DUAL_SYS_1,TRUE);
            break;
            
        case MSG_APP_OK:
                //ZDT_SetIdleWifiIcon(FALSE);
                //MAIN_SetIdleRoamingState(MN_DUAL_SYS_1,FALSE);
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            unbind_click_count = 0;
            MMK_CloseWin(win_id);
            
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}


LOCAL MMI_RESULT_E  HandleZDT_Watch2VMSelectWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );


WINDOW_TABLE( MMIZDT_WATCH_2VM_SELECT_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleZDT_Watch2VMSelectWinMsg),
    WIN_ID(MMIZDT_WATCH_2VM_SELECT_WIN_ID),
    //WIN_TITLE(TXT_BH_NET_2VM),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),	
    //CREATE_ANIM_CTRL (MMISET_QRCODE_CTRL_ID, MMISET_SET_QRCODE_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL uint16 w_down_x = 0;


LOCAL MMI_RESULT_E  HandleZDT_Watch2VMSelectWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMIZDT_TEST_MENU_CTRL_ID;
    uint16                    node_id = 0;
    static BOOLEAN            s_need_update   = FALSE;
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
  //  MMISET_VALIDATE_PRIVACY_RETURN_T    result_info = {MMISET_PROTECT_CL, FALSE};    
#if defined(DISABLE_STATUSBAR_DISPLAY) 
    GUI_RECT_T                          title_rect =   {0, 0, (MMI_MAINSCREEN_WIDTH -1), 23};
#else
    GUI_RECT_T                          title_rect = {0, 18, (MMI_MAINSCREEN_WIDTH -1), 18 + 23};
#endif
    GUI_RECT_T                          full_rect = {0, 0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_WIDTH -1)};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
      qrcode_page_index = 0;
    }

        break;
    case MSG_FULL_PAINT:
    {
	#ifndef ZDT_NEW_BIND_STYLE   
        //GUI_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);
        GUIRES_DisplayImg(PNULL,
                &full_rect,
                PNULL,
                win_id,
                IMAGE_QRCODE_SELECT_BG,
                &lcd_dev_info);
	#endif
    }

        break;        
    case MSG_TP_PRESS_DOWN:
    {
        GUI_POINT_T   point = {0};
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
       
        w_down_x = point.x;

    }          
        break;        
    case MSG_TP_PRESS_MOVE:
    {
       GUI_POINT_T   point = {0};
       point.x = MMK_GET_TP_X(param);
       point.y = MMK_GET_TP_Y(param);
    

    }          
        break;
    case MSG_TP_PRESS_UP:
    {
       GUI_POINT_T   point = {0};
       point.x = MMK_GET_TP_X(param);
       point.y = MMK_GET_TP_Y(param);
       if(point.y < MMI_MAINSCREEN_HEIGHT/2)
       {
            qrcode_page_index = 0;
       }
       else
       {
           qrcode_page_index = 1;
       }
       if(w_down_x == point.x && point.y < MMI_MAINSCREEN_HEIGHT-1) 
       {
            MMIZDT_Open2VMWin();
       }
    }          
        break;        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {

    }          
        break;
       
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


PUBLIC void MMIZDT_OpenManualWin()
{
    qrcode_page_index = 2;
    MMIZDT_Open2VMWin();
}


PUBLIC void MMIZDT_OpenRegVMWin()
{
    qrcode_page_index = 1;
    MMIZDT_Open2VMWin();
}

//zhou 秒表
PUBLIC void MMI_OpenStopwWin()
{
#ifdef STOPWATCH_SUPPORT           
	MMIAPIST_OpenStopWatchWin();
#endif
}

PUBLIC void MMIZDT_Open2VMSelectWin(void)
{
    MMK_CreateWin((uint32 *)MMIZDT_WATCH_2VM_SELECT_WIN_TAB,PNULL);
}



WINDOW_TABLE( MMIZDT_WATCH_2VM_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_2VMWinMsg),    
    WIN_ID( MMIZDT_WATCH_2VM_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_Open2VMWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_2VM_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_Close2VMWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_2VM_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_2VM_WIN_ID);
    }
    return TRUE;
}
#endif

LOCAL MMI_RESULT_E HandleZDT_BHHelpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_STRING_T    text_s = {0};
#if defined(ZDT_MMI_USE_SMALL_FONT)	   
    GUI_FONT_T font = SONG_FONT_22 ;
#else
    GUI_FONT_T font = SONG_FONT_28 ;
#endif
    GUI_COLOR_T color = MMI_WHITE_COLOR ;//MMI_WHITE_COLOR;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
       
      MMK_SetAtvCtrl(win_id, MMIZDT_YX_HELP_CTRL_ID);

	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BH_NET_HELP_INFO,&text_s);
	GUITEXT_SetFont(MMIZDT_YX_HELP_CTRL_ID, &font, &color );
	GUITEXT_SetString(MMIZDT_YX_HELP_CTRL_ID, text_s.wstr_ptr,text_s.wstr_len, TRUE);

        break;

    case MSG_FULL_PAINT:
     break;
        
    case MSG_APP_WEB:       
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:     	
   
        MMK_CloseWin(win_id);
        break;

         
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


WINDOW_TABLE(MMIZDT_BH_HELP_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HandleZDT_BHHelpWinMsg),
	WIN_ID(MMIZDT_BH_HELP_WIN_ID),
	WIN_TITLE(TXT_BH_NET_HELP),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
        CREATE_TEXT_CTRL(MMIZDT_YX_HELP_CTRL_ID),
    END_WIN
};

PUBLIC void MMIZDT_OpenHelpWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_BH_HELP_WIN_TAB, PNULL);
}

WINDOW_TABLE( MMIZDT_BH_MENU_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_BHMainMenuWinMsg),    
    WIN_ID( MMIZDT_BH_MENU_WIN_ID ),
    WIN_TITLE(TXT_BH_NET_LOC),
    CREATE_MENU_CTRL(MENU_ZDT_YX_NET, MMIZDT_YX_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

BOOLEAN MMIZDT_OpenMainMenuWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_BH_MENU_WIN_TAB,PNULL);
    
    return TRUE;
}
LOCAL MMI_RESULT_E HandleZDT_BHMainMenuWinMsg (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_GetId(MMIZDT_YX_MENU_CTRL_ID,&group_id,&menu_id);
            MMK_SetAtvCtrl(win_id, MMIZDT_YX_MENU_CTRL_ID);
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            GUIMENU_GetId(MMIZDT_YX_MENU_CTRL_ID,&group_id,&menu_id);
            switch (menu_id)
            {

    		case ID_ZDT_MENU_BH_ONOFF:
                        MMIZDT_OpenOnOffWin();
    			break;
                
    		case ID_ZDT_MENU_BH_CHAT:
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
                        MMIZDT_OpenTinyChatWin();
#endif
    			break;
                
    		case ID_ZDT_MENU_BH_2VM:
                        MMIZDT_Open2VMWin();
    			break;
                
    		case ID_ZDT_MENU_BH_HELP:
                        MMIZDT_OpenHelpWin();
    			break;
				
                default:
                    break;
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

#endif

#if defined(ZDT_NEW_BIND_STYLE)
LOCAL uint16 w_press_down_x = 0;
LOCAL uint8 w_press_button_rec[4] ={183,13,233,173};


/////////////////////////////////////////////SETP 2///////////////////////////////////////

LOCAL MMI_RESULT_E  HandleZDT_WatchBindStep2WinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );

WINDOW_TABLE( MMIZDT_WATCH_BIND_STEP2_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleZDT_WatchBindStep2WinMsg),
    WIN_ID(MMIZDT_WATCH_BIND_STEP2_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E  HandleZDT_WatchBindStep2WinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T                          full_rect = {0, 0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT-1)};
    char ewm_str[200] = {0};
    uint16 large = 20;
    uint16 uint16_str[22] = {0};
    uint16 uint16_str_len = 0;
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T imei_string = {0};
    char*           temp_ptr = PNULL;
    uint16 temp_len = 0;
    uint16 tip_str[20] = {0x8BF7,0x626B,0x63CF,0x4E8C,0x7EF4,0x7801,0x6DFB,0x52A0,0x8BBE,0x5907,0x0};
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
		
    case MSG_FULL_PAINT:
#if 1
        LCD_FillRect(&lcd_dev_info, full_rect, MMI_WHITE_COLOR);
        MMIZDT_Check2VMFile();        
        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
        sprintf(ewm_str,"%s",g_zdt_phone_imei_1);
        GT_Qcode_show(ewm_str, 30, 10, 174, 174); 

        text_style.font = SONG_FONT_22;
        text_style.align= ALIGN_HVMIDDLE;
        text_style.line_space = 0;
        text_style.region_num = 0;
        text_style.region_ptr = PNULL;		
        text_display_rect.left = 0; // 30;
        text_display_rect.top = 180;
        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
        text_display_rect.bottom=205;
        imei_string.wstr_ptr = tip_str;
        imei_string.wstr_len = MMIAPICOM_Wstrlen(tip_str);

        GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
      		        (const MMI_STRING_T    *)&imei_string,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );	
#else
        GUIRES_DisplayImg(PNULL,
                &full_rect,
                PNULL,
                win_id,
                IMAGE_WATCH_BIND_STEP_2,
                &lcd_dev_info);

            MMIZDT_Check2VMFile();

        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
     //   sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
	 sprintf(ewm_str,"%s",g_zdt_phone_imei_1);	
        //dec_src.file_name_ptr = QRCODE_BIND_FILE_NAME;
        GT_Qcode_show(ewm_str, 4, 0, 174, 174); 
#endif
        text_style.font = SONG_FONT_25;
        text_style.align= ALIGN_HVMIDDLE;
        text_style.line_space = 0;
        text_style.region_num = 0;
        text_style.region_ptr = PNULL;		
        text_display_rect.left = 0; // 30;
        text_display_rect.top = 208;
        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
        text_display_rect.bottom=240;
        temp_ptr = (char*)g_zdt_phone_imei_1;
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)temp_ptr, temp_len);
        imei_string.wstr_ptr = uint16_str;
        imei_string.wstr_len = uint16_str_len;

        GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
      		        (const MMI_STRING_T    *)&imei_string,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );
        break;      
		
    case MSG_TP_PRESS_DOWN:
    {
        GUI_POINT_T   point = {0};
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
       
        w_press_down_x = point.x;

    }          
        break;  
		
    case MSG_TP_PRESS_MOVE:
        break;
		
    case MSG_TP_PRESS_UP:
    {
       GUI_POINT_T   point = {0};
       point.x = MMK_GET_TP_X(param);
       point.y = MMK_GET_TP_Y(param);
	   
       if(w_press_down_x == point.x &&    point.x >= w_press_button_rec[0]   &&    point.x <= w_press_button_rec[2]
	   	&&  point.y >= w_press_button_rec[1]   &&    point.y <= w_press_button_rec[3]) 
       {
                   MMK_CloseWin(win_id);
       }
    }          
        break;        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {

    }          
        break;
       
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

PUBLIC void MMIZDT_OpenBindStep2Win_Ex(void)
{
    MMK_CreateWin((uint32 *)MMIZDT_WATCH_BIND_STEP2_WIN_TAB,PNULL);
}

PUBLIC void MMIZDT_OpenBindStep2Win(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_BIND_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_BIND_WIN_ID);
    }

    MMK_CreateWin((uint32 *)MMIZDT_WATCH_BIND_STEP2_WIN_TAB,PNULL);
}



/////////////////////////////////////////////SETP 1///////////////////////////////////////
LOCAL MMI_RESULT_E  HandleZDT_WatchBindWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );

WINDOW_TABLE( MMIZDT_WATCH_BIND_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleZDT_WatchBindWinMsg),
    WIN_ID(MMIZDT_WATCH_BIND_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E  HandleZDT_WatchBindWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T                          full_rect = {0, 0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_WIDTH -1)};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        unbind_click_count = 0;			
        break;
		
    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL,
                &full_rect,
                PNULL,
                win_id,
                IMAGE_ZDT_JP_BG,
                &lcd_dev_info);
        break;      
		
    case MSG_TP_PRESS_DOWN:
    {
        GUI_POINT_T   point = {0};
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
       
        w_press_down_x = point.x;

        s_2vm_tp_x = point.x;
        s_2vm_tp_y = point.y;
        s_is_2vm_tp_down = TRUE;
    }     	
        break;  
		
    case MSG_TP_PRESS_MOVE:
        break;
		
    case MSG_TP_PRESS_UP:
    {
       GUI_POINT_T   point = {0};
       int offset_y =  0;//up to down
       int offset_x =  0;//up to down	
       
       point.x = MMK_GET_TP_X(param);
       point.y = MMK_GET_TP_Y(param);
	   
       if(w_press_down_x == point.x &&    point.x >= w_press_button_rec[0]   &&    point.x <= w_press_button_rec[2]
	   	&&  point.y >= w_press_button_rec[1]   &&    point.y <= w_press_button_rec[3]) 
       {
            MMIZDT_OpenBindStep2Win();
       }

            if(s_is_2vm_tp_down)
            {
                offset_y =  point.y - s_2vm_tp_y;
                offset_x =  point.x - s_2vm_tp_x;
                if(offset_x <= -(20))	
                {
                    MMK_CloseWin(win_id);
                }	
                else if(offset_x >= (20))
                {
                    MMK_CloseWin(win_id);
                }
                //yangyu add for unbind
                if(point.x < MMI_MAINSCREEN_WIDTH/3 && point.y > MMI_MAINSCREEN_HEIGHT/3)
                {
                    unbind_click_count++;
                    if(unbind_click_count >= 12)
                    {
                        unbind_click_count = 0;
                        //YX_API_DLT_Send();
                    }
                }
                else
                {
                    unbind_click_count = 0;
                }
                //yangyu end

            }
            s_is_2vm_tp_down = FALSE;
			
    }          
        break;        
    case MSG_KEYUP_DOWN:
            MMIZDT_OpenBindStep2Win_Ex();		
            break;
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {

    }          
        break;
       
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        unbind_click_count = 0;	
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

PUBLIC void MMIZDT_OpenBindWin(void)
{
    MMK_CreateWin((uint32 *)MMIZDT_WATCH_BIND_WIN_TAB,PNULL);
}

#endif


#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
MMI_HANDLE_T g_s_voice_prompt_win_handle = 0;
PUBLIC MMI_RESULT_E MMIVoice_HandleNewMsgWin(
                                    MMI_WIN_ID_T        win_id,        //IN:
                                    MMI_MESSAGE_ID_E    msg_id,        //IN:
                                    DPARAM                param        //IN:
                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //如果屏保被点亮中，则关闭屏保窗口
        if (MMK_IsOpenWin(MMIMAIN_SHOWTIME_WIN_ID))
        {
            MMK_CloseWin(MMIMAIN_SHOWTIME_WIN_ID);
        }
        break;

    case MSG_FULL_PAINT:
        
            MMIPUB_SetWinSoftkey(win_id, TXT_READ, STXT_EXIT, FALSE);

            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_TIMER:
            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
            MMIZDT_OpenTinyChatWin();
            MMK_CloseWin( win_id );//yangyu add ,for bug
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    case MSG_APP_RED:
            MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:

        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
}

//yangyu add begin
PUBLIC void CloseVoiceNewMsgDialog()
{
    //MMIPUB_CloseAlertWin();
    MMK_CloseWin(MMIVOICE_NEWMSG_WIN_ID);

}

//yangyu end

LOCAL void ShowVoiceNewMsgPrompt(void)
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 new_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    uint16 to_be_read_num = 1;
    MMI_WIN_PRIORITY_E win_priority = WIN_LOWEST_LEVEL;
    MMI_STRING_T prompt_str = {0};
    uint32 time_out = 0;
    MMI_WIN_ID_T alert_win_id = MMIVOICE_NEWMSG_WIN_ID;
    
    prompt_str.wstr_len = 0;
    prompt_str.wstr_ptr = PNULL;

    if (0 == to_be_read_num)
    {
        return;
    }
        
    MMI_GetLabelTextByLang(TXT_YX_NEW_VOICE_MSG, &prompt_str);
    
    if (MMK_IsFocusWin(MMIVOICE_NEWMSG_WIN_ID))
    {
        // set to window
        MMIPUB_SetAlertWinTextByPtr(alert_win_id,&prompt_str,PNULL,TRUE);
    }
    else
    {
        MMK_CloseWin(MMIVOICE_NEWMSG_WIN_ID);

        // 窗口: 提示用户新短消息
#ifdef SCREENSAVER_SUPPORT
        if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
        {
            MMIAPIIDLESS_CloseScreenSaver();
        }
#endif

        if(MMISMS_IsForbitNewMsgWin(FALSE))
        {
            win_priority = WIN_LOWEST_LEVEL;
        }
        else
        {
            win_priority = WIN_ONE_LEVEL;
        }
        MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,MMIPUB_SOFTKEY_CUSTOMER,MMIVoice_HandleNewMsgWin);
        g_s_voice_prompt_win_handle = (MMI_HANDLE_T)alert_win_id; 
        MMIPUB_SetWinTitleTextId(MMIVOICE_NEWMSG_WIN_ID, TXT_COMMON_PROMPT, FALSE);
    }
}

LOCAL void YX_PlayRcvVoiceMsgTone()
{
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    if(!MMIZDT_IsInTinyChatWin())
#endif
    {
        //MMIAPISET_PlayRing(MN_DUAL_SYS_1, FALSE, 0, 1, MMISET_RING_TYPE_MSG, PNULL);
        MMIAPISET_PlayCallRingByVol(MMIAPISET_GetCallRingVolume(), 1,1, MMISET_RING_TYPE_MSG, PNULL);//yangyu modify
    }
}

//yangyu add begin
LOCAL BOOLEAN isVideoChatOverDialog = FALSE;

PUBLIC void DelayShowNewMsgPrompt()
{
    if(isVideoChatOverDialog)
    {
        isVideoChatOverDialog = FALSE;
        ShowVoiceNewMsgPrompt();
    }
}
//yangyu end
PUBLIC void MMIVoice_ShowNewMsgPrompt(
                             BOOLEAN    is_play_ring
                             )
{
    MMIDEFAULT_TurnOnBackLight();
    if (MMIZDT_IsInTinyChatWin() || MMIZDT_IsInClassModeWin())
    {
        return;
    }
#ifdef BAIRUI_VIDEOCHAT_SUPPORT    
    if(VideoChat_IsInCall())
    {
        isVideoChatOverDialog = TRUE;
        
        return;
    }
#endif

#ifndef WIN32    
#ifdef LEBAO_MUSIC_SUPPORT
    lebao_exit();
#endif    
#endif
    if(is_play_ring)
    {
        YX_PlayRcvVoiceMsgTone();
    }
#ifdef BAIRUI_VIDEOCHAT_SUPPORT    
    if(!VideoChat_IsLogin()) //yangyu add
#endif    
    {
        ShowVoiceNewMsgPrompt();
    }

}
#endif

#if 1
#define ZDT_PB_ICON_LEFT_START		0
#define ZDT_PB_ICON_TOP_START		10
#define ZDT_PB_ICON_WIDTH		120
#define ZDT_PB_ICON_HEIGHT		75
#define ZDT_PB_TEXT_HEIGHT		30

uint16 m_yx_cur_whitelist_idx = 0;
static uint16 m_yx_whitelist_totalnum=0;
static uint16 m_yx_whitelist_pagenum=0;
static uint16 m_yx_whitelist_curpage=0;
static uint16 m_yx_whitelist_curindex=0;
static int s_pb_tp_x;
static int s_pb_tp_y;
static BOOLEAN s_is_pb_tp_down = FALSE;


//yangyu add begin
PUBLIC void ZDT_PB_GetContactNameByNumber(uint8 *number, uint16 *dst_name)
{
    uint8 i = 0;
    if(number == NULL || dst_name == NULL)
    {
        return;
    }
    #if 0
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        
        if(strcmp(yx_DB_White_Reclist[i].white_num,  number) == 0)
        {
            ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,dst_name, 100);
        }
    }  
    #endif

}
//yangyu end

extern uint16  MMIZDT_PB_GetNext(uint16 white_idx)
{
    uint16 i = 0;
    uint16 next_idx = 0;
    
    for(i = white_idx; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        if(yx_DB_White_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_White_Reclist[i].white_num) > 0)
        {
            next_idx = i+1;
            break;
        }
    }

    if(next_idx == 0)
    {
        for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
        {
            if(yx_DB_White_Reclist[i].db_id != 0 && SCI_STRLEN(yx_DB_White_Reclist[i].white_num) > 0)
            {
                next_idx = i+1;
                break;
            }
        }
    }
    
    return next_idx;
}

#ifdef ZDT_BH_W9_FEATRUE
BOOLEAN MMIZDT_PB_Call_Start(uint16 white_idx)
{
    uint8 i = 0;
    uint16 tempStr[101] ={0};
	uint8 need_white_call = MMIZDT_GetNeedWhiteCall();
    if(white_idx == 0 || white_idx > YX_NEW_DB_MAX_NUM || SCI_STRLEN(yx_PbList_Reclist[white_idx-1].pblist_num) == 0)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        return FALSE;
    }
    if(yx_PbList_Reclist[white_idx-1].type == 3 && 0 == need_white_call)
    {
        return FALSE;
    }
    if(YX_API_MoCallLimit_IsValid())
    {
        return FALSE;
    }
    //MMIZDT_MakeAll(yx_PbList_Reclist[white_idx-1].pblist_num,SCI_STRLEN(yx_PbList_Reclist[white_idx-1].pblist_num),PNULL);
    SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));
    GUI_UTF8ToWstr(tempStr,100,yx_PbList_Reclist[white_idx-1].pblist_name,strlen(yx_PbList_Reclist[white_idx-1].pblist_name));	
    //ZDT_UCS_Str16_to_uint16((uint8*)yx_PbList_Reclist[white_idx-1].pblist_name, SCI_STRLEN(yx_PbList_Reclist[white_idx-1].pblist_name) ,tempStr, 100); 
    MMIZDT_MakeAll(yx_PbList_Reclist[white_idx-1].pblist_num,SCI_STRLEN(yx_PbList_Reclist[white_idx-1].pblist_num),tempStr);
    return TRUE;
}

#else
BOOLEAN MMIZDT_PB_Call_Start(uint16 white_idx)
{
    uint8 i = 0;
    uint16 tempStr[101] ={0};
    if(white_idx == 0 || white_idx > YX_DB_WHITE_MAX_SUM || SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_num) == 0)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        return FALSE;
    }
    #if 0
    SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));
    ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[white_idx-1].white_name, SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_name) ,tempStr, 100);            
    #endif
    MMIZDT_MakeAll(yx_DB_White_Reclist[white_idx-1].white_num,SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_num),PNULL);
    return TRUE;
}
#endif
void  MMIZDT_PB_DrawBg(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0}; 
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    return;
}
void  MMIZDT_PB_DrawCur(MMI_WIN_ID_T win_id,uint16 index)
{
    uint16 pb_sum = 0;
    uint16 i = 0;
    uint32 image_id = IMAGE_ZDT_PB_CALL;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};
    GUI_POINT_T                 dis_point = {0,0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
#if defined(ZDT_MMI_USE_SMALL_FONT)	  
    GUI_FONT_T f_big =SONG_FONT_22;
#else
    GUI_FONT_T f_big =SONG_FONT_28;
#endif
    uint16 TempStr[101] ={0};
    uint16 ba_wStr[2] = {0x7238,0};
    uint16 ma_wStr[2] = {0x5988,0};
    uint16 ye_wStr[2] = {0x7237,0};
    uint16 nai_wStr[2] = {0x5976,0};
    #if 1 // xiongkai    通讯录界面图标不一致
    uint16 fu_wStr[2] = {0x7236,0};//fu
    uint16 mu_wStr[2] = {0x6BCD,0};//mu qin
    uint16 shu_wStr[2] = {0x53D4,0};//shushu
    uint16 yi_wStr[2] = {0x59E8,0};//yi
    uint16 waigong_wStr[3] = {0x5916, 0x516C,0};//wai gong
    uint16 waipo_wStr[3] = {0x5916, 0x5A46,0};//wai po    
    #endif
	if(index > 0)
	{
		index--;
	}
     //if(index != m_yx_whitelist_pagenum)
     {
        MMIZDT_PB_DrawBg();
    	
    	 for(i=(index)*4;i<(index)*4+4;i++)
    	 {
            if(yx_DB_White_Reclist[i].db_id == 0)
            {
                break;
            }
            pb_sum++;
            #if 0
            SCI_MEMSET(TempStr,0,(101)*sizeof(uint16));	 
            ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,TempStr, 100);
            #endif

            cur_rect.left = ZDT_PB_ICON_LEFT_START+ ((i%2)*ZDT_PB_ICON_WIDTH);
            cur_rect.right = cur_rect.left+ZDT_PB_ICON_WIDTH-1;
 		if(i/2%2)
 		{
                cur_rect.top = ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START;
 		}
		else
		{
                cur_rect.top = ZDT_PB_ICON_TOP_START;
 		}
            cur_rect.bottom = cur_rect.top+ZDT_PB_ICON_HEIGHT-1;
			dis_point.x = cur_rect.left;
			dis_point.y = cur_rect.top;
            GUIRES_DisplayImg(&dis_point,
                    &rect,
                    PNULL,
                    win_id,
                    image_id,
                    &lcd_dev_info);
            
            cur_rect.top = cur_rect.bottom+1;
            cur_rect.bottom = cur_rect.top + ZDT_PB_TEXT_HEIGHT-1;
            
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = f_big;
            text_style.font_color = MMI_WHITE_COLOR;
            #ifdef ZDT_PCBA_Z605_SUPPORT
            if(m_yx_whitelist_curindex == (i%4))
            {
                text_style.font_color = MMI_DARK_BLUE_COLOR;
            }
            #endif
            cur_str_t.wstr_ptr = TempStr;
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(TempStr);

            GUISTR_DrawTextToLCDInRect( 
                                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                    (const GUI_RECT_T      *)&cur_rect,       
                                    (const GUI_RECT_T      *)&cur_rect,     
                                    (const MMI_STRING_T    *)&cur_str_t,
                                    &text_style,
                                    state,
                                    GUISTR_TEXT_DIR_AUTO
                                    ); 
	 }
         
        if(pb_sum == 0)
        {
            cur_rect.left   = 0;
            cur_rect.top    = 0; 
            cur_rect.right  = MMI_MAINSCREEN_WIDTH-1;
            cur_rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
            //空7A7A
            TempStr[0] = 0x7A7A;
            TempStr[1] = 0;
            
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = f_big;
            text_style.font_color = MMI_WHITE_COLOR;

            cur_str_t.wstr_ptr = TempStr;
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(TempStr);

            GUISTR_DrawTextToLCDInRect( 
                                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                    (const GUI_RECT_T      *)&cur_rect,       
                                    (const GUI_RECT_T      *)&cur_rect,     
                                    (const MMI_STRING_T    *)&cur_str_t,
                                    &text_style,
                                    state,
                                    GUISTR_TEXT_DIR_AUTO
                                    ); 
        }
     }
     MMITHEME_UpdateRect();
    return;
}


void  MMIZDT_PB_ShowInit(void)
{
        uint16 i = 0;
        m_yx_whitelist_curindex = 0;
	 m_yx_whitelist_totalnum=0;
    	 for(i=0;i<YX_DB_WHITE_MAX_SUM;i++)
    	 {
    	 	if(yx_DB_White_Reclist[i].db_id)
    	 	{
    	 		m_yx_whitelist_totalnum ++;
    	 	}
    	 }

	 if(m_yx_whitelist_totalnum%4)
	 {
	 	m_yx_whitelist_pagenum = m_yx_whitelist_totalnum/4 + 1;
	 }
	 else
	 {
	 	m_yx_whitelist_pagenum = m_yx_whitelist_totalnum/4;
	 }
	 m_yx_whitelist_curpage= 1;
       return;
}

void  MMIZDT_PB_Show(MMI_WIN_ID_T win_id)
{
    MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
}

#ifdef BAIRUI_VIDEOCHAT_SUPPORT
//yangyu add
PUBLIC void GetNameAndIndexByTalkUserId(int talk_user_id, uint16 *name_wstr, uint8 *index)
{
    int i = 0;
    for(i = 0 ; i < MAX_VIDEO_USERS_COUNT; i++)
    {
        if(talk_user_id == atoi(video_users[i].phone_user_id))
        {
            ZDT_PB_GetContactNameByNumber(video_users[i].phone_num, name_wstr);
            *index = i;
            break;
        }

    }
}
//yangyu end


PUBLIC MMI_IMAGE_ID_T MMIZDT_GetHeadImageId(int appUserId)
{
    uint16 ba_wStr[2] = {0x7238,0};//ba
    uint16 fu_wStr[2] = {0x7236,0};//fu
    uint16 ma_wStr[2] = {0x5988,0};//ma
    uint16 mu_wStr[2] = {0x6BCD,0};//mu qin
    uint16 ye_wStr[2] = {0x7237,0};//ye
    uint16 nai_wStr[2] = {0x5976,0};//nai
    uint16 shu_wStr[2] = {0x53D4,0};//shushu
    uint16 yi_wStr[2] = {0x59E8,0};//yi
    uint16 waigong_wStr[3] = {0x5916, 0x516C,0};//wai gong
    uint16 waipo_wStr[3] = {0x5916, 0x5A46,0};//wai po
    uint16 tempStr[101] ={0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint8 i = 0;

    if(appUserId > 0)
    {
        //GetNameAndIndexByTalkUserId(appUserId, tempStr, &i);
        //cur_str_t.wstr_ptr = tempStr;
        //cur_str_t.wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
         //int i = 0;
        for(i = 0 ; i < MAX_VIDEO_USERS_COUNT; i++)
        {
            if(appUserId == atoi(video_users[i].phone_user_id))
            {
                //ZDT_PB_GetContactNameByNumber(video_users[i].phone_num, name_wstr);
                SCI_MEMCPY(tempStr, video_users[i].family_relative, 16*2);
                break;
            }

        }
        SCI_TRACE_LOW("MMIZDT_GetHeadImageId have name");
    }
    else
    {
        return IMAGE_NULL;
    }
    image_id = IMAGE_ZDT_PB_CALL;
    return image_id;

}
#endif

//yangyu add
PUBLIC BOOLEAN MMIZDT_IsPhoneBookEmpty()
{
    BOOLEAN ret = TRUE;
    uint16 count = 0;
    int i = 0;
    for(i=0;i<YX_DB_WHITE_MAX_SUM;i++)
    {
       if(yx_DB_White_Reclist[i].db_id)
       {
           count++;
       }
    }
    if(count > 0)
    {
        ret = FALSE; 
    }
    return ret;

}
//yangyu end

void  MMIZDT_PB_ShowList(MMI_WIN_ID_T win_id)
{
    BOOLEAN ret = FALSE;
    MMI_STRING_T    cur_str_t   = {0};
    uint16 nameStr[101] ={0};
    uint16 numStr[101] ={0};
    uint16 WhiteStr[101] ={0};
    uint16 disp_wStr[101] ={0};
    uint16 SOS_wStr[10] = {0x0053,0x004F,0x0053,0x003A,0x0};//SOS:0053 004F 0053 003A 
    uint16 SPD_wStr[3] = {0x952E,0x003A,0x0}; // 键952E 
    uint16 White_wStr[5] = {0x767D,0x540D,0x5355,0x0}; //白名单767D 540D 5355 
    char temp_str[100] = {0};
    uint16 i = 0, j = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_ZDT_PB_CALL;
    uint16 position = 0;
	uint8 need_white_call = MMIZDT_GetNeedWhiteCall();

    GUILIST_ITEM_T       		item_t    =  {0};
    GUILIST_ITEM_DATA_T  		item_data = {0};
    //add by James li begin
    uint16 total_item_num = 0;
    MMI_STRING_T    empty_str = {0};
    GUILIST_EMPTY_INFO_T empty_info = {0};
    //add by James li end

    
    item_t.item_data_ptr = &item_data; 
    item_t.item_style =  GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_BIGICON;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;//GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;//GUIITEM_STYLE_ONE_LINE_TEXT; 

    
    m_yx_whitelist_totalnum = Get_Pb_List();

    GUILIST_RemoveAllItems(MMIZDT_PB_LIST_CTRL_ID);

    for(i = 0 ;i < m_yx_whitelist_totalnum ; i++)
    {
        if(yx_PbList_Reclist[i].db_id == 0)
        {
            break;
        }
        if(yx_PbList_Reclist[i].type == 1)
        {
            // SOS
            SCI_MEMSET(disp_wStr,0,sizeof(disp_wStr));
            MMIAPICOM_Wstrcat(disp_wStr, SOS_wStr);
        }
        else if(yx_PbList_Reclist[i].type == 2)
        {
            // SPD
            SCI_MEMSET(disp_wStr,0,sizeof(disp_wStr));
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            sprintf(temp_str,"%d",yx_PbList_Reclist[i].db_id);
            GUI_UTF8ToWstr(disp_wStr,100,temp_str,strlen(temp_str));
            MMIAPICOM_Wstrcat(disp_wStr, SPD_wStr);
        }
        else if(yx_PbList_Reclist[i].type == 3)
        {
            // WHITE
            SCI_MEMSET(disp_wStr,0,sizeof(disp_wStr));
            MMIAPICOM_Wstrcat(disp_wStr, White_wStr);
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            sprintf(temp_str,"%d:",yx_PbList_Reclist[i].db_id);
            GUI_UTF8ToWstr(WhiteStr,100,temp_str,strlen(temp_str));
            MMIAPICOM_Wstrcat(disp_wStr, WhiteStr);
        }
        
        SCI_MEMSET(nameStr,0,sizeof(nameStr));
        GUI_UTF8ToWstr(nameStr,100,yx_PbList_Reclist[i].pblist_name,strlen(yx_PbList_Reclist[i].pblist_name));
        MMIAPICOM_Wstrcat(disp_wStr, nameStr);
        
        if(YX_API_MoCallLimit_IsValid() || (yx_PbList_Reclist[i].type == 3 && 0 == need_white_call))
        {
            item_data.item_content[1].item_data_type = GUIITEM_DATA_NONE;        
            item_data.item_content[1].item_data.image_id = image_id;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = disp_wStr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (disp_wStr);
        }
        else
        {
            item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;        
            item_data.item_content[1].item_data.image_id = image_id;
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = disp_wStr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (disp_wStr);
        }
        
	 SCI_MEMSET(numStr,0,sizeof(numStr));
	 GUI_UTF8ToWstr(numStr,100,yx_PbList_Reclist[i].pblist_num,strlen(yx_PbList_Reclist[i].pblist_num));
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = numStr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (numStr);
        ret = GUILIST_AppendItem (MMIZDT_PB_LIST_CTRL_ID, &item_t);
    }

    total_item_num = CTRLLIST_GetTotalItemNum(MMIZDT_PB_LIST_CTRL_ID);

    if (0 == total_item_num)
    {
       MMI_GetLabelTextByLang(STR_CONTA_NOTE_NO_CONTA_TEXT, &empty_str);
	empty_info.text_buffer = empty_str;
	CTRLLIST_SetEmptyInfo(MMIZDT_PB_LIST_CTRL_ID, &empty_info);
    }
    
}


void  MMIZDT_PB_TP_Move(MMI_WIN_ID_T win_id,uint8 dir)
{
    if(dir) 
    {
        	if(m_yx_whitelist_curpage>1)
        		m_yx_whitelist_curpage--;
    }
    else
    {
        	if(m_yx_whitelist_curpage<m_yx_whitelist_pagenum)
        		m_yx_whitelist_curpage++;
    }
    MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
}

LOCAL MMI_RESULT_E  HandleZDT_PBWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMIZDT_PB_ShowInit();
        }
        break;
        
        case MSG_FULL_PAINT:
                MMIZDT_PB_Show(win_id);
            break;
            
        case MSG_GET_FOCUS:
                MMIZDT_PB_Show(win_id);
            break;
            
        case MSG_LOSE_FOCUS:
            break;
            
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_pb_tp_x = point.x;
            s_pb_tp_y = point.y;
            s_is_pb_tp_down = TRUE;
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            int offset_y =  0;//up to down
            int offset_x =  0;//up to down
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_is_pb_tp_down)
            {
                offset_y =  point.y - s_pb_tp_y;
                offset_x =  point.x - s_pb_tp_x;
                if(offset_x <= -(20))	
                {
                    MMIZDT_PB_TP_Move(win_id,0);
                }	
                else if(offset_x >= (20))
                {
                    MMIZDT_PB_TP_Move(win_id,1);
                }
                else
                {
        		  if(point.y > ZDT_PB_ICON_TOP_START && point.y<(ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
        		  {
        		  	if(point.x<120)
        		  	{
        		  		MMIZDT_PB_Call_Start(1+(m_yx_whitelist_curpage-1)*4);
        		  	}
        			else
        			{
        				MMIZDT_PB_Call_Start(2+(m_yx_whitelist_curpage-1)*4);
        			}
        		  }
        		  else if(point.y> (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START) 
                      && point.y< (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
        		  {
        		  	if(point.x<120)
        		  	{
        		  		MMIZDT_PB_Call_Start(3+(m_yx_whitelist_curpage-1)*4);
        		  	}
        			else
        			{
        				MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        			}		  
        		  }
                }
            }
            s_is_pb_tp_down = FALSE;
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(point.y > ZDT_PB_ICON_TOP_START && point.y<(ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
            {
                if(point.x<120)
                {
                    MMIZDT_PB_Call_Start(1+(m_yx_whitelist_curpage-1)*4);
                }
                else
                {
                    MMIZDT_PB_Call_Start(2+(m_yx_whitelist_curpage-1)*4);
                }
            }
            else if(point.y> (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START) 
              && point.y< (ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT+ZDT_PB_ICON_TOP_START+ZDT_PB_ICON_HEIGHT+ZDT_PB_TEXT_HEIGHT))
            {
                if(point.x<120)
                {
                    MMIZDT_PB_Call_Start(3+(m_yx_whitelist_curpage-1)*4);
                }
                else
                {
                    MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
                }		  
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_WEB:
            MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        break;
        
    case MSG_KEYDOWN_CANCEL:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_DOWN:
            m_yx_whitelist_curindex = 0;
            MMIZDT_PB_TP_Move(win_id,1);
        break;
        
    case MSG_KEYDOWN_UP:
            m_yx_whitelist_curindex = 0;
            MMIZDT_PB_TP_Move(win_id,0);
        break;

    case MSG_KEYDOWN_LEFT:
            if(m_yx_whitelist_curindex > 0)
            {
                m_yx_whitelist_curindex--;
                MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
            }
            else
            {
                MMIZDT_PB_TP_Move(win_id,1);
            }
        break;

    case MSG_KEYDOWN_RIGHT:
            m_yx_whitelist_curindex++;
            if(m_yx_whitelist_curindex > 3)
            {
                m_yx_whitelist_curindex = 0;
                MMIZDT_PB_TP_Move(win_id,0);
            }
            else
            {
                MMIZDT_PB_DrawCur(win_id,m_yx_whitelist_curpage);
            }
        break;
        
    case MSG_CLOSE_WINDOW:
            if(p_index != PNULL)
            {
                SCI_FREE(p_index);
            }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

LOCAL MMI_RESULT_E  HandleZDT_NewPbWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIZDT_PB_LIST_CTRL_ID;
    BOOLEAN ret = FALSE;
    uint32 pos_user_data = 0; // position user data
    uint8 * p_index = PNULL;
    GUILIST_INIT_DATA_T list_init = {0}; 
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T rect = {0,0,(MMI_MAINSCREEN_WIDTH -1),(MMI_MAINSCREEN_WIDTH -1)};
            //MMIZDT_PB_ShowInit();
            list_init.both_rect.v_rect.left = 0;
            list_init.both_rect.v_rect.right = (MMI_MAINSCREEN_WIDTH -1);
            list_init.both_rect.v_rect.top = 0;
            list_init.both_rect.v_rect.bottom = (MMI_MAINSCREEN_WIDTH -1);//240;//180;

            list_init.both_rect.h_rect.left = 0;
            list_init.both_rect.h_rect.right = (MMI_MAINSCREEN_WIDTH -1);//240;//180;
            list_init.both_rect.h_rect.top = 0;
            list_init.both_rect.h_rect.bottom = (MMI_MAINSCREEN_WIDTH -1);
            
            list_init.type = GUILIST_TEXTLIST_E;
                        
            GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);  
            ret = GUILIST_SetMaxItem(ctrl_id, YX_NEW_DB_MAX_NUM, FALSE);
            //GUILIST_SetUserBgImage(ctrl_id, IMG_DIGIT_CLOCK0_BG, FALSE);
            //GUILIST_SetRect(MMK_ConvertIdToHandle(ctrl_id), &rect);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            //xiongkai del  这里导致进电话本会卡。MMIAPICC_DecMissedCallNumber(MMIAPICC_GetMissedCallNumber());//remove miss call status bar icon
            //MMIAPICOM_StatusAreaSetMissedCallState(FALSE); //remove miss call status bar icon
        }
        break;
        
        case MSG_FULL_PAINT:
                //MMIZDT_PB_Show(win_id);
                MMIZDT_PB_ShowList(win_id);
            break;
            
        case MSG_GET_FOCUS:
                //MMIZDT_PB_Show(win_id);
            break;
            
        case MSG_LOSE_FOCUS:
            break;
            
	 case MSG_KEYUP_OK:
	 case MSG_KEYUP_GREEN:
		 {
			uint16 num_index = GUILIST_GetCurItemIndex(ctrl_id);
			//GUILIST_GetItemData(ctrl_id, num_index, &pos_user_data);// user_data stand position
			MMIZDT_PB_Call_Start(num_index+1);
		 }
	 	break;
	 #if 1 //ndef ZDT_BH_W9_FEATRUE
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        {
        //case MSG_KEYDOWN_WEB:
            uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
            //GUILIST_GetItemData(ctrl_id, index, &pos_user_data);// user_data stand position
            MMIZDT_PB_Call_Start(index+1);
            //ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_CTL_MIDSK  index=%d, pos_user_data=%d" , index, pos_user_data);
        }

        break;
        #endif
            
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_pb_tp_x = point.x;
            s_pb_tp_y = point.y;
            s_is_pb_tp_down = TRUE;
            
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(point.x - s_pb_tp_x > 120 && z_abs(point.y - s_pb_tp_y) < 100)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
        
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
                
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_WEB:
            //MMIZDT_PB_Call_Start(4+(m_yx_whitelist_curpage-1)*4);
        break;
        

        
    case MSG_KEYDOWN_DOWN:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,1);
        break;
        
    case MSG_KEYDOWN_UP:
            //m_yx_whitelist_curindex = 0;
            //MMIZDT_PB_TP_Move(win_id,0);
        break;

    case MSG_KEYDOWN_LEFT:

        break;

    case MSG_KEYDOWN_RIGHT:

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CLOSE_WINDOW:
        if(p_index != PNULL)
        {
            SCI_FREE(p_index);
        }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


#if 0
WINDOW_TABLE( MMIZDT_WATCH_PB_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_PBWinMsg),    
    WIN_ID( MMIZDT_WATCH_PB_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};
#else
WINDOW_TABLE( MMIZDT_WATCH_PB_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_NewPbWinMsg),    
    WIN_ID( MMIZDT_WATCH_PB_WIN_ID ),
    //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIZDT_PB_LIST_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};
#endif

BOOLEAN MMIZDT_OpenPBWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_PB_WIN_TAB,PNULL);
    return TRUE;
}


BOOLEAN MMIZDT_ClosePBWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_PB_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_PB_WIN_ID);
    }
    return TRUE;
}


#endif

#if 1
#include"zdt_yx_db.h"
uint8 g_cur_mesage_index =0;
LOCAL uint32  g_yx_netsms_content_len_ex = 0;

extern PUBLIC BOOLEAN YX_API_NetSms_PlayIndex(uint8 index);
extern PUBLIC BOOLEAN YX_API_NetSms_Stop();

extern uint16 YX_DB_NETSMS_HaveNum(void);

uint8 MMIZDT_Message_GetDateTime(uint32 cur_time,uint8 * pData)
{
    uint8 len = 0;
    MMI_TM_T          tm;
    char tem_buf[50] = {0};    
    tm = MMIAPICOM_Second2Tm(cur_time);
    sprintf(tem_buf,"%04d-%02d-%02d %02d:%02d",tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min);
    len = SCI_STRLEN(tem_buf);
    SCI_MEMCPY(pData,tem_buf,len);
    return len;
}
LOCAL MMI_RESULT_E HandleZDT_MessagePreQueryWinMsg(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
        {
            YX_DB_NETSMS_ListDel(yx_DB_NetSms_list_index[g_cur_mesage_index]);
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIZDT_WATCH_MESSAGE_PRE_WIN_ID);
        }
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

LOCAL MMI_RESULT_E  HandleZDT_MessagePreWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
   
	MMI_RESULT_E			recode = MMI_RESULT_TRUE;
	GUI_RECT_T			rect		 = MMITHEME_GetFullScreenRect();
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};	 
	GUISTR_STYLE_T		text_style		= {0};/*lint !e64*/
	GUISTR_STATE_T		state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
	MMI_STRING_T		content_str = {0};
	wchar				content_text[YX_DB_NETSMS_MAX_NAME_SIZE+100] = {0};
	wchar				temp_content_text[YX_DB_NETSMS_MAX_NAME_SIZE+1] = {0};
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
    MMI_STRING_T        string = {0};	
    GUI_RECT_T          text_rect={42,0,239,MMI_SPECIAL_TITLE_HEIGHT}; 
    GUI_RECT_T content_rect = {0, MMI_SPECIAL_TITLE_HEIGHT, MMI_MAINSCREEN_WIDTH - 1 , MMI_MAINSCREEN_HEIGHT - 1}; 
#else
    GUI_RECT_T content_rect = {0, 0, MMI_MAINSCREEN_WIDTH - 1 , MMI_MAINSCREEN_HEIGHT - 1}; 
#endif
	MN_IMEISVN_T imeisvn = {0};/*lint !e64*/
	uint8 imei_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
	uint8 message_num=0;
	uint16			temp_len = 0;
	uint16 imei_len = 0;
	uint16 uint16_str_len = 0;
	uint8 count,i,j;
	wchar			unicode_cr = 0x0d;
	uint8			temp_str[100] = {0};
      static uint8 s_pre_db_index = 0;
	rect.top =0;
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW: 
		{
                GUI_FONT_T font = SONG_FONT_20;//SONG_FONT_16;
                GUI_COLOR_T color = MMI_WHITE_COLOR;
                char time_str[101] ={0};
                uint16 time_wStr[101] ={0};
                uint16 name_wstr[101] ={0};
                uint16 name_len = 0;
                s_pre_db_index = yx_DB_NetSms_list_index[g_cur_mesage_index];
                MMK_SetAtvCtrl(win_id,MMIZDT_MESSAGE_PREVIEW_ID);
                GUITEXT_SetClipboardEnabled(MMIZDT_MESSAGE_PREVIEW_ID,FALSE);
                GUITEXT_SetRect(MMIZDT_MESSAGE_PREVIEW_ID, &content_rect);
                GUITEXT_SetFont(MMIZDT_MESSAGE_PREVIEW_ID, &font, &color);
                GUITEXT_SetAlign(MMIZDT_MESSAGE_PREVIEW_ID, ALIGN_LEFT);
                message_num =YX_DB_NETSMS_HaveNum();
                YX_API_NetSms_PlayIndex(s_pre_db_index);
                if(yx_DB_NetSms_Reclist[s_pre_db_index].is_read == 0)
                {
            		YX_DB_NETSMS_ChangStatusToRead(s_pre_db_index);
            		YX_Net_Send_REPORT_SMS_READ(&g_yx_app,s_pre_db_index);
                }
                GUIRES_DisplayImg(PNULL,
                                                &rect,
                                                PNULL,
                                                win_id,
                                                IMAGE_THEME_BLACK_BG, //xiongkai IMAGE_THEME_BG,
                                                &lcd_dev_info);

                MMIZDT_Message_GetDateTime(yx_DB_NetSms_Reclist[s_pre_db_index].netsms_rcv_time,time_str);
                if(strlen(yx_DB_NetSms_Reclist[s_pre_db_index].msg_id) > 0)
                {
                    if(MMICC_GetYxDbRecCallName(yx_DB_NetSms_Reclist[s_pre_db_index].msg_id, name_wstr,&name_len))
                    {
                        MMIAPICOM_Wstrcat(content_text, name_wstr);
                        sprintf((char*)temp_str, "\r\n%s\r\n",time_str);
                        GUI_UTF8ToWstr(time_wStr,100,temp_str,strlen(temp_str));
                    }
                    else
                    {
                        sprintf((char*)temp_str, "%s\r\n%s\r\n",yx_DB_NetSms_Reclist[s_pre_db_index].msg_id,time_str);
                        GUI_UTF8ToWstr(time_wStr,100,temp_str,strlen(temp_str));
                    }
                }
                else
                {
                    sprintf((char*)temp_str, "****\r\n%s\r\n",time_str);
                    GUI_UTF8ToWstr(time_wStr,100,temp_str,strlen(temp_str));
                }
                MMIAPICOM_Wstrcat(content_text, time_wStr);
                SCI_MEMSET(temp_content_text,0,sizeof(temp_content_text));
                if(yx_DB_NetSms_Reclist[s_pre_db_index].netsms_encoding == 2 || yx_DB_NetSms_Reclist[s_pre_db_index].netsms_encoding == 0)
                {
                    GUI_UTF8ToWstr(temp_content_text,YX_DB_NETSMS_MAX_NAME_SIZE,yx_DB_NetSms_Reclist[s_pre_db_index].netsms_content,yx_DB_NetSms_Reclist[s_pre_db_index].netsms_content_len);
                }
                else
                {
                    SCI_MEMCPY(temp_content_text,yx_DB_NetSms_Reclist[s_pre_db_index].netsms_content,yx_DB_NetSms_Reclist[s_pre_db_index].netsms_content_len);
                }
                MMIAPICOM_Wstrcat(content_text,temp_content_text);
                content_str.wstr_ptr = content_text;
                content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
                GUITEXT_SetString(MMIZDT_MESSAGE_PREVIEW_ID, content_str.wstr_ptr,content_str.wstr_len , FALSE);
		}
		break;
	case MSG_FULL_PAINT:
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
     {
            GUI_POINT_T point = {0};
            MMI_TEXT_ID_T text_id_list[] = 
            {
                STR_MESSAGE_URGENT,
                STR_MESSAGE_NOTICE,
                STR_MESSAGE_WORK,
                STR_MESSAGE_ATTENDANCE,
                STR_MESSAGE_COST,
                STR_MESSAGE_NEWS,
                STR_MESSAGE_HAVE_MESSAGE
            };
		//draw bg        
            point.x = 0;
            point.y = 0 ;
            GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_THEME_BLACK_BG_SPEC,&lcd_dev_info);


		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = MMI_DEFAULT_EXTRA_BIG_FONT;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;
	
    		MMI_GetLabelTextByLang(text_id_list[yx_DB_NetSms_Reclist[s_pre_db_index].netsms_type], &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );

        #if 0 //def TOUCH_PANEL_SUPPORT
                //draw delete icon
                point.x = MMI_MAINSCREEN_WIDTH - 34;
                point.y = (MMI_SPECIAL_TITLE_HEIGHT-24)/2;
                GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_SECMENU_ICON_CL_DELALL,&lcd_dev_info); 
        #endif
	}
#endif
		break;
    case MSG_TP_PRESS_UP:
    	{
	        GUI_POINT_T   point = {0};
	        point.x = MMK_GET_TP_X(param);
	        point.y = MMK_GET_TP_Y(param);
			
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
                 if((point.x > 0&& point.x <50)&&(point.y >= 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
                 {
        		     MMK_CloseWin(win_id);
        		     return;	 
                 }
                 if((point.x > 190 && point.x < 240)&&(point.y >= 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
                 {
                        MMI_WIN_ID_T	query_win_id = MMIZDT_QUERY_WIN_ID;
                        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,HandleZDT_MessagePreQueryWinMsg);                
        		     return;	 
                 }
#endif
#if 0
                 if((point.x > 10&& point.x <229)&&(point.y > (MMI_MAINSCREEN_HEIGHT - 50) &&  point.y < MMI_MAINSCREEN_HEIGHT))
                 {
        		     MMIZFB_OpenPSWSetWin();
        		     return;	 
                 }
#endif
    	}
        break;
        
	case MSG_APP_GREEN:
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;	
	case MSG_CLOSE_WINDOW:
            YX_API_NetSms_Stop();
            MMIAPICOM_StatusAreaSetMsgState(TRUE);
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}


WINDOW_TABLE( MMIZDT_WATCH_MESSAGE_PRE_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_MessagePreWinMsg),    
    WIN_ID( MMIZDT_WATCH_MESSAGE_PRE_WIN_ID ),
    CREATE_TEXT_CTRL(MMIZDT_MESSAGE_PREVIEW_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenMessagePreWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_MESSAGE_PRE_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_IsSmsPreFocusWin(void)
{
    if(MMK_IsFocusWin(MMIZDT_WATCH_MESSAGE_PRE_WIN_ID))
    {
        return TRUE;
    }
    return FALSE;
}

BOOLEAN MMIZDT_UpdateSmsPreWin(void)
{
    if(MMK_IsFocusWin(MMIZDT_WATCH_MESSAGE_PRE_WIN_ID))
    {
        MMK_PostMsg(MMIZDT_WATCH_MESSAGE_PRE_WIN_ID, MSG_FULL_PAINT, PNULL,0);
    }
    return FALSE;
}

void  MMIZDT_Message_ShowList(MMI_WIN_ID_T win_id)
{
    BOOLEAN ret = FALSE;
    uint16 i = 0, j = 0;
    char time_str[101] ={0};
    uint16 disp_wStr[101] ={0};
    uint16 nameStr[101] ={0};
    uint16 numStr[101] ={0};
    
    MMI_TEXT_ID_T text_id_list[] = 
    {
    	STR_MESSAGE_URGENT,
    	STR_MESSAGE_NOTICE,
    	STR_MESSAGE_WORK,
    	STR_MESSAGE_ATTENDANCE,
    	STR_MESSAGE_COST,
    	STR_MESSAGE_NEWS,
    	STR_MESSAGE_HAVE_MESSAGE
    };
    MMI_IMAGE_ID_T img_id_list[] = 
    {
        IMAGE_MESSAGE_NO_READ,
        IMAGE_MESSAGE_READ
    };

    GUILIST_ITEM_T       		item_t    =  {0};
    GUILIST_ITEM_DATA_T  		item_data = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint16 name_wstr[101] ={0};
    uint16 name_len = 0;
    uint16 total_item_num = 0;
    MMI_STRING_T    empty_str = {0};
    GUILIST_EMPTY_INFO_T empty_info = {0};
    uint8 message_num=0;
    
    item_t.item_data_ptr = &item_data; 
    item_t.item_style =  GUIITEM_STYLE_TWO_LINE_BIGICON_TWO_TEXT_AND_TEXT;

    GUILIST_RemoveAllItems(MMIZDT_MESSAGE_LIST_CTRL_ID);
    message_num =YX_DB_NETSMS_HaveNum();
    for(i=0;i<message_num; i++)
    {
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id= img_id_list[yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].is_read];	

        //if(yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].netsms_type == 6)
        {
            SCI_MEMSET(numStr,0,sizeof(numStr));
            SCI_MEMSET(name_wstr,0,sizeof(name_wstr));
            if(strlen(yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].msg_id) > 0)
            {
                if(MMICC_GetYxDbRecCallName(yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].msg_id, name_wstr,&name_len))
                {
                    MMIAPICOM_Wstrcpy(numStr, name_wstr);
                }
                else
                {
                    GUI_UTF8ToWstr(numStr,100,yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].msg_id,strlen(yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].msg_id));
                }
            }
            else
            {
                GUI_UTF8ToWstr(numStr,100,"****",strlen("****"));
            }
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = numStr;
            item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (numStr);
        }
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id= text_id_list[yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].netsms_type];

        SCI_MEMSET(disp_wStr,0,sizeof(disp_wStr));
        MMIZDT_Message_GetDateTime(yx_DB_NetSms_Reclist[yx_DB_NetSms_list_index[i]].netsms_rcv_time,time_str);
        GUI_UTF8ToWstr(disp_wStr,100,time_str,strlen(time_str));
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = disp_wStr;
        item_data.item_content[3].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (disp_wStr);
        
        GUILIST_AppendItem (MMIZDT_MESSAGE_LIST_CTRL_ID, &item_t);
    }
    CTRLLIST_SetCurItemIndex(MMIZDT_MESSAGE_LIST_CTRL_ID,g_cur_mesage_index);
    total_item_num = CTRLLIST_GetTotalItemNum(MMIZDT_MESSAGE_LIST_CTRL_ID);
    if (0 == total_item_num)
    {
       MMI_GetLabelTextByLang(STR_MESSAGE_NO_NEWS, &empty_str);
	empty_info.text_buffer = empty_str;
	CTRLLIST_SetEmptyInfo(MMIZDT_MESSAGE_LIST_CTRL_ID, &empty_info);
    }
    
}

LOCAL MMI_RESULT_E  HandleZDT_MessageWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIZDT_MESSAGE_LIST_CTRL_ID;
    BOOLEAN ret = FALSE;
    uint32 pos_user_data = 0; // position user data
    uint8 * p_index = PNULL;
    uint8 message_num=0;
    GUILIST_INIT_DATA_T list_init = {0}; 
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T rect = {0,0,(MMI_MAINSCREEN_WIDTH -1),(MMI_MAINSCREEN_WIDTH -1)};
            list_init.both_rect.v_rect.left = 0;
            list_init.both_rect.v_rect.right = (MMI_MAINSCREEN_WIDTH -1);
            list_init.both_rect.v_rect.top = 0;
            list_init.both_rect.v_rect.bottom = (MMI_MAINSCREEN_WIDTH -1);//240;//180;

            list_init.both_rect.h_rect.left = 0;
            list_init.both_rect.h_rect.right = (MMI_MAINSCREEN_WIDTH -1);//240;//180;
            list_init.both_rect.h_rect.top = 0;
            list_init.both_rect.h_rect.bottom = (MMI_MAINSCREEN_WIDTH -1);
            
            list_init.type = GUILIST_TEXTLIST_E;
            GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);  
            ret = GUILIST_SetMaxItem(ctrl_id, YX_DB_NETSMS_MAX_SUM, FALSE);
            //GUILIST_SetUserBgImage(ctrl_id, IMG_DIGIT_CLOCK0_BG, FALSE);
            CTRLLIST_SetCurItemIndex(ctrl_id,g_cur_mesage_index);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            MMIAPI_New_Message_Reset();
        }
        break;
        
        case MSG_FULL_PAINT:
                MMIZDT_Message_ShowList(win_id);
            break;
            
        case MSG_GET_FOCUS:
		CTRLLIST_SetCurItemIndex(ctrl_id,g_cur_mesage_index);
            	break;
            
        case MSG_LOSE_FOCUS:
            break;
    
    case MSG_KEYDOWN_RED:
        break;

            
#if 1 //ndef ZDT_BH_W9_FEATRUE
        case MSG_CTL_OK:
        case MSG_APP_GREEN:
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        {
                  message_num =YX_DB_NETSMS_HaveNum();
                  if(message_num>0)
                  {
                    g_cur_mesage_index = GUILIST_GetCurItemIndex(ctrl_id);
                 	 MMIZDT_OpenMessagePreWin();	
                  }
        }

        break;
#endif
    
    case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_WEB:
        break;
    		
    case MSG_KEYUP_UP:    
    case MSG_KEYUP_DOWN:
	     g_cur_mesage_index = CTRLLIST_GetCurItemIndex(ctrl_id);
        break;
     
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(p_index != PNULL)
        {
            SCI_FREE(p_index);
        }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

WINDOW_TABLE( MMIZDT_WATCH_MESSAGE_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_MessageWinMsg),    
    WIN_ID( MMIZDT_WATCH_MESSAGE_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};
BOOLEAN MMIZDT_OpenNoticeWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_MESSAGE_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_IsSmsListFocusWin(void)
{
    if(MMK_IsFocusWin(MMIZDT_WATCH_MESSAGE_WIN_ID))
    {
        return TRUE;
    }
    return FALSE;
}

BOOLEAN MMIZDT_UpdateSmsListWin(void)
{
    if(MMK_IsFocusWin(MMIZDT_WATCH_MESSAGE_WIN_ID))
    {
        MMK_PostMsg(MMIZDT_WATCH_MESSAGE_WIN_ID, MSG_FULL_PAINT, PNULL,0);
    }
    return FALSE;
}
#endif
#if 0 //天气
extern YX_WEATHER_INFO_T g_yx_wt_info;



LOCAL MMI_RESULT_E  HandleZDT_WeatherWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );



WINDOW_TABLE( MMIZDT_WEATHER_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_WeatherWinMsg),    
    WIN_ID( MMIZDT_WEATHER_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};


#define WEATHER_EXPIRE_MINUTES  30//120


LOCAL uint8 weather_timer_id = 0;
LOCAL uint16 weather_click_count = 0;
LOCAL uint16 wt_tp_down_x = 0;
LOCAL uint16 wt_tp_down_y = 0;
LOCAL SCI_TIME_T last_wt_time = {0};





 
LOCAL void MMIZDT_StartWeatherTimer()
{
    if(weather_timer_id != 0)
    {
        MMK_StopTimer(weather_timer_id);
    }
    weather_timer_id =  MMK_CreateWinTimer(MMIZDT_WEATHER_WIN_ID, 60*1000,TRUE);

}

LOCAL void MMIZDT_StopWeatherTimer()
{
    if(weather_timer_id != 0)
    {
        MMK_StopTimer(weather_timer_id);
    }
}



PUBLIC BOOLEAN MMIZDT_IsInWeatherWin()
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId() ;

    return win_id == MMIZDT_WEATHER_WIN_ID;
}

PUBLIC BOOLEAN MMIZDT_IsInWeatherWinHandle(MMI_HANDLE_T win_handle)
{
    return MMK_GetWinId( win_handle) == MMIZDT_WEATHER_WIN_ID;
}


PUBLIC void MMIZDT_OpenWeatherWin()
{
   if(MMK_GetFocusWinId() != MMIZDT_WEATHER_WIN_ID)
   {
        MMK_CreateWin((uint32*)MMIZDT_WEATHER_WIN_TAB,PNULL);
   }
}

PUBLIC void MMK_CloseWeatherWin()
{
    if(MMK_IsOpenWin(MMIZDT_WEATHER_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WEATHER_WIN_ID);
    }
}

#ifdef WIN32
LOCAL void ZDT_InitWeather()
{
    uint16 shenzhen_city[3] = {0x6DF1, 0x5733 , 0};//shenzhen
    //uint16 shenzhen_city[3] = {0x4178, 0x5958 ,0};//shenzhen
    SCI_MEMCPY(g_yx_wt_info.city_name, shenzhen_city, 3*sizeof(uint16));
    g_yx_wt_info.min_degree = 20;
    g_yx_wt_info.max_degree = 30;
    g_yx_wt_info.cur_degree = 26;
    g_yx_wt_info.num = 1;
}
#endif

LOCAL BOOLEAN Is_WeatherExpired()
{
	BOOLEAN ret = TRUE;	
	uint8 year_str[5] = {0};
	uint8 mon_str[3] = {0};
	uint8 day_str[3] = {0};
	
	SCI_TIME_T time = {0};
	SCI_DATE_T date = {0};
	TM_GetSysTime(&time);
	TM_GetSysDate(&date);
	#if 0
	SCI_MEMCPY(year_str, g_yx_wt_info.date_str+0, 4);
	SCI_MEMCPY(mon_str, g_yx_wt_info.date_str+5, 2);
	SCI_MEMCPY(day_str, g_yx_wt_info.date_str+8, 2);
	
	if(atoi(year_str) == date.year 
		&& atoi(mon_str) == date.mon
		&& atoi(day_str) == date.mday
		)
	{
		ret = FALSE;
	}
	#endif
	if(time.hour < last_wt_time.hour //next day
		|| (time.hour*60 + time.min) - (last_wt_time.hour*60 + last_wt_time.min) >= WEATHER_EXPIRE_MINUTES //2hour

	)
	{
		ret = TRUE;
		last_wt_time = time;

	}
	else
	{
		ret = FALSE;

	}
	ZDT_LOG("zdt__ Is_WeatherExpired, ret=%d", ret);
	return ret;
}


/*LOCAL*/ BOOLEAN Is_WeatherGetFromServer()
{
	BOOLEAN ret = TRUE;
	if((g_yx_wt_info.min_degree == 0 &&  g_yx_wt_info.max_degree == 0)

		|| Is_WeatherExpired()
	)
	{
		ret = FALSE;
	}
	ZDT_LOG("zdt__ Is_WeatherGetFromServer, ret=%d", ret);
	return ret;
}


LOCAL void DisplayWeather(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    SCI_TIME_T time = {0};
    GUI_POINT_T point = {0};
    const uint16 img_width = 82;
    const uint16 img_height = 82;
    //add by James li begin
    MMI_STRING_T	display_string	= {0};
    wchar	wstr_buf[MMIPHONE_MAX_OPER_NAME_LEN]	= {0};
    //add by James li end
   
    MMI_IMAGE_ID_T weather_img_id = IMAGE_WEATHER_UNKNOWN;

    uint16 qing_wstr[2] = {0x6674, 0};//qing
    uint16 yin_wstr[2] = {0x9634, 0};//yin
    uint16 yu_wstr[2] = {0x96E8, 0};//yu
    uint16 xue_wstr[2] = {0x96EA, 0};//xue


    GUI_RECT_T	wt_rect={150,60,240,100};
    GUI_RECT_T	temperature_rect={145,110,(MMI_MAINSCREEN_WIDTH -1),160};
    GUI_RECT_T	update_time_rect={150,169,(MMI_MAINSCREEN_WIDTH -1),199};	
    GUI_RECT_T	city_rect={42,10,(MMI_MAINSCREEN_WIDTH -1),40};
    GUI_RECT_T	sim_rect={0,200,(MMI_MAINSCREEN_WIDTH -1),240};//add by James li
    GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRect();


    GUI_RECT_T	text_rect={0};
    MMI_STRING_T tmp_string = {0};
    MMI_STRING_T        content_str = {0};
    MMI_STRING_T        update_time_str = {0};	
    wchar               content_text[100 + 1] = {0};
    wchar               content_text_1[100 + 1] = {0};	
    uint16 uint16_str_len = 0;
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_TEXT_ID_T weather_string_id;
    char*           temp_ptr = PNULL;
    uint16 temp_len = 0;

    uint8 temp_str[32] = {0};
    uint8 temp_wstr[32] = {0};
    
    TM_GetSysTime(&time);
    ZDT_LOG("zdt__ DisplayWeather, ");

    //GUI_FillRect(&lcd_dev_info,bg_rect, MMI_BLACK_COLOR);
    
    point.x = 0;
    point.y = 0 ;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_BIG_WEATHER_BG,&lcd_dev_info); 

	//city
	tmp_string.wstr_ptr = g_yx_wt_info.city_name;
	tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
	text_style.align = ALIGN_LEFT;
	text_style.font = SONG_FONT_24;
	text_style.font_color = MMI_WHITE_COLOR;
	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&city_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&city_rect,       //ó??§òa???Dμ?êμ?ê??óò
		        (const MMI_STRING_T    *)&tmp_string,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );	

	//weather icon  
	switch(g_yx_wt_info.num)
	{
		case 1:
			weather_img_id = IMAGE_BIG_WEATHER_SUNNY;
			weather_string_id=TXT_WEATHER_SUNNY;
			break;
		case 2:
			weather_img_id = IMAGE_BIG_WEATHER_CLOUD;
			weather_string_id=TXT_WEATHER_RAIN;			
			break;
		case 3:
			weather_img_id = IMAGE_BIG_WEATHER_RAIN;
			weather_string_id=TXT_WEATHER_CLOUD;					
			break;
		case 4:
			weather_img_id = IMAGE_BIG_WEATHER_SNOW;
			weather_string_id=TXT_WEATHER_SNOW;					
			break;
		default:
			weather_img_id = IMAGE_BIG_WEATHER_UNKNOW;
			weather_string_id=TXT_WEATHER_UNKNOW;					
			break;				
	}
 
	point.x = 40;
	point.y = 60 ;
	GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, weather_img_id,&lcd_dev_info); 
	

	//weather text
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)weather_string_id,&tmp_string);
	text_style.align = ALIGN_LEFT;
	text_style.font_color = MMI_WHITE_COLOR;
	text_style.font = SONG_FONT_32;
	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&wt_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&wt_rect,       //ó??§òa???Dμ?êμ?ê??óò
		        (const MMI_STRING_T    *)&tmp_string,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );	
	
	//temperature
	memset(temp_str,0,sizeof(temp_str));
	sprintf((char*) temp_str, "%d", g_yx_wt_info.cur_degree);
	MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, temp_str, strlen(temp_str));
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WEATHER_TEMPERATURE_DU,&tmp_string);
	MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &tmp_string);
	text_style.font = SONG_FONT_40;	
	text_style.align = ALIGN_LEFT;
	text_style.font_color = MMI_YELLOW_COLOR;
	content_str.wstr_ptr = content_text;
	content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&temperature_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&temperature_rect,       //ó??§òa???Dμ?êμ?ê??óò
		        (const MMI_STRING_T    *)&content_str,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );	

	//update time
        text_style.font = SONG_FONT_28;
        text_style.font_color = MMI_WHITE_COLOR;	
	 text_style.align=ALIGN_LEFT; 		
        text_style.line_space = 0;
        text_style.region_num = 0;
        text_style.region_ptr = PNULL;		
		
        memset(temp_str,0,sizeof(temp_str));
        sprintf((char*) temp_str, "%02d:%02d", time.hour,time.min);

        temp_ptr = (char*)temp_str;
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(content_text_1, &uint16_str_len, (uint8*)temp_ptr, temp_len);
        update_time_str.wstr_ptr = content_text_1;
        update_time_str.wstr_len = uint16_str_len;

        GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&update_time_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&update_time_rect,       //ó??§òa???Dμ?êμ?ê??óò
      		        (const MMI_STRING_T    *)&update_time_str,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );	
		
	//add by James li begin
	text_style.align = ALIGN_HVMIDDLE;
	text_style.font_color = MMI_GREEN_COLOR;
	text_style.font = SONG_FONT_24;

	display_string.wstr_ptr = wstr_buf;
	MMIAPIPHONE_GetSimAndServiceString(MN_DUAL_SYS_1, &display_string);

	GUISTR_DrawTextToLCDInRect( 
		        MMITHEME_GetDefaultLcdDev(),
		        (const GUI_RECT_T      *)&sim_rect,       //the fixed display area
		        (const GUI_RECT_T      *)&sim_rect,       //¨????ì¨°a???D|ì?¨o|ì?¨o??¨?¨°
		        (const MMI_STRING_T    *)&display_string,
		        &text_style,
		        state,
		        GUISTR_TEXT_DIR_AUTO
		        );
	//add by James li end
}

PUBLIC void MMIZDT_UpdateWeatherWin()
{
	MMK_SendMsg(MMIZDT_WEATHER_WIN_ID, MSG_FULL_PAINT, NULL);
}

LOCAL MMI_RESULT_E  HandleZDT_WeatherWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T        str_data = {0};
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;

	
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            weather_click_count = 0;

	        //MMIZDT_StartWeatherTimer();
	        #ifdef WIN32
			ZDT_InitWeather();
			#else
			if(!Is_WeatherGetFromServer())
			{
				YX_API_WT_Send();
			}
			#endif

            break;
		case MSG_GET_FOCUS:

	        //MMIZDT_StartWeatherTimer();

	        break;
		case MSG_FULL_PAINT:
		{
		        
	       

	        DisplayWeather(win_id);
				
		        

	    }
			break;
			
#if 1//def TOUCH_PANEL_SUPPORT
	    case MSG_TP_PRESS_DOWN:
	        {
	            GUI_POINT_T   point = {0};
	            point.x = MMK_GET_TP_X(param);
	            point.y = MMK_GET_TP_Y(param);
				wt_tp_down_x = point.x;
				wt_tp_down_y = point.y;
	            
	        }
	        break;
	        
	    case MSG_TP_PRESS_UP:
	        {
	            GUI_POINT_T   point = {0};
	            
				point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
		        if(wt_tp_down_y - point.y  > 90)
		        {
		            MMK_SendMsg(MMK_ConvertIdToHandle(win_id), MSG_APP_CANCEL, PNULL);//exit to clock screen
		            break;
		        }
		        else if(ABS(wt_tp_down_x - point.x) > 40)
		        {
		            break;
		        }
		    }
		        
	        break;
	        
	    case MSG_TP_PRESS_MOVE:
	        {
	            GUI_POINT_T   point = {0};
	            point.x = MMK_GET_TP_X(param);
	            point.y = MMK_GET_TP_Y(param);
	        }
	        break;
	    case MSG_TP_PRESS_LONG:
	        {
	            GUI_POINT_T   point = {0};
	            point.x = MMK_GET_TP_X(param);
	            point.y = MMK_GET_TP_Y(param);
	        }
	        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        //case MSG_APP_OK:
        //case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            break;
        case MSG_TIMER:
        {
            if (*(uint8*)param == weather_timer_id)
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        
        break;
        case MSG_BACKLIGHT_TURN_ON:
        case MSG_BACKLIGHT_TURN_OFF:

        break;
        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
        { 
            //MMIZDT_StopClassModeTimer();

        }
        
        break;


        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}
#endif

#if 1
uint8 g_cur_wldt_answer_mode = 0;
extern BOOLEAN MMIZDT_OpenWLDT_YesNo(void);
extern BOOLEAN MMIZDT_OpenWLDT_Single(void);
extern BOOLEAN MMIZDT_OpenWLDT_Multi(void);
LOCAL MMI_RESULT_E HandleWLDTYesNoWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIZDT_WLDT_LIST_YES_NO_ID;
    uint16 cur_selection = 0;
    BOOLEAN is_on=FALSE;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,2, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextIdExt( TXT_WLDT_ANSWER_YES ,TXT_COMMON_OK,TXT_NULL,TXT_NULL,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt( TXT_WLDT_ANSWER_NO,TXT_COMMON_OK,TXT_NULL,TXT_NULL,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;    
    case MSG_CTL_CANCEL:
        //MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if(cur_selection == 0)
        {
            YX_Net_Send_HDKT_SEND_ANSWER(&g_yx_app,31,1);
        }
        else
        {
            YX_Net_Send_HDKT_SEND_ANSWER(&g_yx_app,31,0);
        }
        MMK_CloseWin(win_id);
        break;        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

WINDOW_TABLE(MMIZDT_WLDT_YESNO_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWLDTYesNoWinMsg),
    WIN_ID(MMIZDT_WLDT_YESNO_WIN_ID),
    WIN_TITLE(TXT_WLDT_YESNO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIZDT_WLDT_LIST_YES_NO_ID),
    END_WIN
};

BOOLEAN MMIZDT_OpenWLDT_YesNo(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WLDT_YESNO_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseWLDT_YesNo(void)
{
    if(MMK_IsOpenWin(MMIZDT_WLDT_YESNO_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WLDT_YESNO_WIN_ID);
    }
    return TRUE;
}

LOCAL MMI_RESULT_E HandleWLDTSingleWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIZDT_WLDT_LIST_SINGLE_ID;
    uint16 cur_selection = 0;
    uint8 is_on=0;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,4, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextIdExt( TXT_WLDT_ANSWER_A ,TXT_COMMON_OK,TXT_NULL,TXT_NULL,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt( TXT_WLDT_ANSWER_B,TXT_COMMON_OK,TXT_NULL,TXT_NULL,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        MMIAPISET_AppendListItemByTextIdExt( TXT_WLDT_ANSWER_C ,TXT_COMMON_OK,TXT_NULL,TXT_NULL,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt( TXT_WLDT_ANSWER_D,TXT_COMMON_OK,TXT_NULL,TXT_NULL,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;    
        
    case MSG_CTL_CANCEL:
        //MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        YX_Net_Send_HDKT_SEND_ANSWER(&g_yx_app,32,cur_selection+1);
        MMK_CloseWin(win_id);
        break;  
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

WINDOW_TABLE(MMIZDT_WLDT_SINGLE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWLDTSingleWinMsg),
    WIN_ID(MMIZDT_WLDT_SINGLE_WIN_ID),
    WIN_TITLE(TXT_WLDT_SINGLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIZDT_WLDT_LIST_SINGLE_ID),
    END_WIN
};

BOOLEAN MMIZDT_OpenWLDT_Single(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WLDT_SINGLE_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseWLDT_Single(void)
{
    if(MMK_IsOpenWin(MMIZDT_WLDT_SINGLE_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WLDT_SINGLE_WIN_ID);
    }
    return TRUE;
}

const MMI_TEXT_ID_T s_wldt_multi_select_text_id[4] =
{
    TXT_WLDT_ANSWER_A,
    TXT_WLDT_ANSWER_B,
    TXT_WLDT_ANSWER_C,
    TXT_WLDT_ANSWER_D
};
 
LOCAL void AppendWLDT_MultiListItem(
                                               MMI_CTRL_ID_T    ctrl_id//in
                                               )
{
    	uint8 day_num=0;
	GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
       GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/

	//add items to listbox
	for(day_num = 0; day_num < 4; day_num++)   
	{
    
		item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
		item_t.item_data_ptr = &item_data;

		item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
		item_data.item_content[0].item_data.text_id = s_wldt_multi_select_text_id[ day_num ];
    
    		GUILIST_AppendItem( ctrl_id, &item_t );  

	}  
}

LOCAL MMI_RESULT_E HandleWLDTMultiWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIZDT_WLDT_LIST_MULTI_ID;
    uint16 cur_selection = 0;
    uint16              sel_items[4] = {0};
    uint16              sel_count = 0;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,4, FALSE );//max item 2
        GUILIST_SetMaxSelectedItem(ctrl_id,4);
        AppendWLDT_MultiListItem(ctrl_id);
        GUILIST_SetTextListMarkable(ctrl_id, TRUE);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_APP_WEB:        
    case MSG_APP_MENU:        
        break;
      
    case MSG_CTL_OK:    
    case MSG_KEYDOWN_OK:
        sel_count = GUILIST_GetSelectedItemIndex(ctrl_id,sel_items, 4);
        if (0 == sel_count)         // no day selected
        {
            MMIPUB_OpenAlertWarningWin(TXT_MUST_SELECT_ONE);
        }
        else
        {
            uint16 job_answer = 0;
            uint16 i = 0;
            uint16 temp = 0;
            for(i = 0; i < sel_count; i ++)
            {
                temp = (0x0001<< sel_items[i]);
                job_answer |= temp;
            }
            YX_Net_Send_HDKT_SEND_ANSWER(&g_yx_app,33,job_answer);
            MMK_CloseWin(win_id);
        }

        break;
        
    case MSG_CTL_CANCEL:    
    case MSG_KEYDOWN_CANCEL:
        //MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

WINDOW_TABLE(MMIZDT_WLDT_MULTI_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleWLDTMultiWinMsg),
    WIN_ID(MMIZDT_WLDT_MULTI_WIN_ID),
    WIN_TITLE(TXT_WLDT_MULTI),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIZDT_WLDT_LIST_MULTI_ID),
    END_WIN
};

BOOLEAN MMIZDT_OpenWLDT_Multi(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WLDT_MULTI_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseWLDT_Multi(void)
{
    if(MMK_IsOpenWin(MMIZDT_WLDT_MULTI_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WLDT_MULTI_WIN_ID);
    }
    return TRUE;
}


BOOLEAN MMIZDT_OpenWLDT_ByMode(uint8 answer_type,uint8 job_type)
{
    if(MMICC_GetCallNum() > 0)
    {
        return FALSE;

    }
    MMIZDT_CloseWLDT_YesNo();
    MMIZDT_CloseWLDT_Single();
    MMIZDT_CloseWLDT_Multi();
    MMIDEFAULT_TurnOnBackLight();
    MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,1000);
    if(job_type == 31)
    {
        MMIZDT_OpenWLDT_YesNo();
    }
    else if(job_type == 32)
    {
        MMIZDT_OpenWLDT_Single();
    }
    else if(job_type == 33)
    {
        MMIZDT_OpenWLDT_Multi();
    }
    g_cur_wldt_answer_mode = answer_type;
    return TRUE;
}
#endif

#if 1
LOCAL MMI_RESULT_E  HandleCardBindWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T                          full_rect = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
    char ewm_str[200] = {0};
    uint16 large = 20;
    uint16 uint16_str[22] = {0};
    uint16 uint16_str_len = 0;
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T imei_string = {0};
    char*           temp_ptr = PNULL;
    uint16 temp_len = 0;
    char*           RFID = PNULL;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
		
    case MSG_FULL_PAINT:
    {
       LCD_FillRect(&lcd_dev_info, full_rect, MMI_WHITE_COLOR);
       MMIZDT_Check2VMFile();

        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
#if 1
	sprintf(ewm_str,"http://cardh5.hejiaoyu.com.cn/bind/card?imei=%s&v=cy",g_zdt_phone_imei_1);
       GT_Qcode_show(ewm_str, 30, 10, 174, 174); 
		
        text_style.font_color = MMI_BLACK_COLOR;
        text_style.font = SONG_FONT_25;
        text_style.align= ALIGN_HVMIDDLE;	
        text_display_rect.left = 0; // 30;
        text_display_rect.top = 190;
        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
        text_display_rect.bottom=215;
	 SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)"IMEI:", SCI_STRLEN((char*)"IMEI:"));
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)g_zdt_phone_imei_1, SCI_STRLEN((char*)g_zdt_phone_imei_1));
        imei_string.wstr_ptr = uint16_str;
        imei_string.wstr_len = uint16_str_len;

        GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
      		        (const MMI_STRING_T    *)&imei_string,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );
#else
#ifndef _WIN32
 	RFID = SCI_GET_RF_ID();
#else
	RFID =(char*)"0";
#endif
        //http://cardh5.hejiaoyu.com.cn/bind/card?imei=xxxxxxxx&rfid=xxxxxxx&v=xx
       sprintf(ewm_str,"http://cardh5.hejiaoyu.com.cn/bind/card?imei=%s&rfid=%s&v=cy",g_zdt_phone_imei_1,RFID);
       GT_Qcode_show(ewm_str, 30, 10, 174, 174); 
		
        text_style.font_color = MMI_BLACK_COLOR;
        text_style.font = SONG_FONT_25;
        text_style.align= ALIGN_HVMIDDLE;	
        text_display_rect.left = 0; // 30;
        text_display_rect.top = 180;
        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
        text_display_rect.bottom=205;
	 SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)"IMEI:", SCI_STRLEN((char*)"IMEI:"));
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)g_zdt_phone_imei_1, SCI_STRLEN((char*)g_zdt_phone_imei_1));
        imei_string.wstr_ptr = uint16_str;
        imei_string.wstr_len = uint16_str_len;

        GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
      		        (const MMI_STRING_T    *)&imei_string,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );
		
	 text_display_rect.left = 0; // 30;
        text_display_rect.top = 206;
        text_display_rect.right=(MMI_MAINSCREEN_WIDTH -1);
        text_display_rect.bottom=233;
	 SCI_MEMSET(uint16_str,0,sizeof(uint16_str));
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)"RFID:", SCI_STRLEN((char*)"RFID:"));		
        MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)RFID,  SCI_STRLEN((char*)RFID));
        imei_string.wstr_ptr = uint16_str;
        imei_string.wstr_len = uint16_str_len;

        GUISTR_DrawTextToLCDInRect( 
      		        MMITHEME_GetDefaultLcdDev(),
      		        (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
      		        (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
      		        (const MMI_STRING_T    *)&imei_string,
      		        &text_style,
      		        state,
      		        GUISTR_TEXT_DIR_AUTO
      		        );
#endif
    }
        break;      
		
    case MSG_TP_PRESS_DOWN:
              
        break;  
		
    case MSG_TP_PRESS_MOVE:
        break;
		
    case MSG_TP_PRESS_UP:
              
        break;        
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {

    }          
        break;
       
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
WINDOW_TABLE(MMIZDT_CARD_BIND_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleCardBindWinMsg),
    WIN_ID(MMIZDT_CARD_BIND_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenCardBindWin(void)
{
    if(ZDT_IsInFactoryWin() || MMIAPICC_IsInState(CC_IN_CALL_STATE) || MMIALARM_IsActiveWin()
#ifdef FOTA_SUPPORT_CMIOT
       || MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID)
#endif
	)
    {
        return FALSE;
    }
    if(MMK_IsOpenWin(MMIZDT_CARD_BIND_WIN_ID))
    {
        return TRUE;
    }
    else
    {
        MMK_CreateWin((uint32*)MMIZDT_CARD_BIND_WIN_TAB,PNULL);
    }
    return TRUE;
}

BOOLEAN MMIZDT_CloseCardBindWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_CARD_BIND_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_CARD_BIND_WIN_ID);
    }
    return TRUE;
}
#endif

#ifdef ZDT_CYHEALTH_SUPPORT
#define HEALTH_SETTING_DISP_X      0
#define HEALTH_SETTING_DISP_Y	    0
#define HEALTH_SETTING_DISP_WIDTH  	240
#define HEALTH_SETTING_DISP_HEIGHT	240

#define HEALTH_SETTING_2VM_DISP_X      0
#define HEALTH_SETTING_2VM_DISP_Y	    20
#define HEALTH_SETTING_2VM_DISP_WIDTH  	220
#define HEALTH_SETTING_2VM_DISP_HEIGHT	220

#define HEALTH_GREEN_COLOR  MMI_DARK_GREEN_COLOR
#define HEALTH_YELLOW_COLOR   0xc642
#define HEALTH_RED_COLOR   MMI_DARK_RED_COLOR

static uint8 zdt_health_timer_id = 0;
static MMI_WIN_ID_T g_cur_win_id =0;
static BOOLEAN win_is_wating =FALSE;
static uint8 zdt_health_display_type = 0;
static int s_jp_tp_x;
static int s_jp_tp_y;
static BOOLEAN s_is_jp_tp_down = FALSE;

BOOLEAN MMIZDT_OpenUrlHealthWin(void);
BOOLEAN MMIZDT_OpenUrlNucleicWin(void);
BOOLEAN MMIZDT_OpenUrlAccWin(void);
BOOLEAN MMIZDT_OpenType1HealthWin(void);
BOOLEAN MMIZDT_OpenType1NucleicWin(void);
BOOLEAN MMIZDT_OpenType1AccWin(void);
BOOLEAN MMIZDT_OpenHealthWait_Win(void);
BOOLEAN MMIZDT_CloseHealthWait_Win(void);
BOOLEAN MMIZDT_UpdateHealthWait_Win(void);

BOOLEAN MMIZDT_OpenHealthType1_Win(void);
BOOLEAN MMIZDT_OpenHealthType2_Win(void);
BOOLEAN MMIZDT_UpdateHealthWin(uint8 is_url_pic);
BOOLEAN MMIZDT_OpenHealthInfoWin(void);
BOOLEAN MMIZDT_OpenHealth2VMWin(void);
BOOLEAN MMIZDT_OpenHealthMainWin(void);

static void ZdtHealth_BackLight(BOOLEAN is_alway_on)
{
      if(is_alway_on)
      {
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
      }
      else
      {
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
      }
}

uint8 MMIZDT_Health_TYPE1_IsValid(void)
{
    uint8 status = 0;
    if(1 == yx_health_msg_Rec.code_result && yx_health_msg_Rec.code_status != 0)
    {
        status |= 0x01;
    }
    if(strlen(yx_health_msg_Rec.detect_time) > 0)
    {
        status |= 0x02;
    }
    if(yx_health_msg_Rec.vacc_status != 0)
    {
        status |= 0x04;
    }
    return status;
}

uint8 MMIZDT_Health_TYPE2_IsValid(void)
{
    uint8 status = 0;
    if(yx_health_msg_Rec.type2_status != 0 && yx_health_msg_Rec.type2_type != 4)
    {
        status |= 0x01;
    }
    return status;
}

void MMIZDT_Health_Win_Wating(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T	cur_str_t	= {0};
    MMI_STRING_T	other_str_t	= {0};
    GUI_RECT_T	 cur_rect		   = {0};
    GUI_RECT_T	 rect		 = {0}; 
    GUISTR_STYLE_T		text_style		= {0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_30;
    GUI_RECT_T	 title_rect		   = {0};
    GUI_RECT_T	 other_rect		   = {0};
    uint16 search_fail[10] = {0x67E5, 0x8BE2, 0x5931, 0x8D25, 0x0021, 0x0 }; //查询失败!
    uint16 over_time[10] = {0x5065, 0x5EB7, 0x7801, 0x5DF2, 0x8FC7, 0x671F, 0x0 };
    
    rect.left	= 0;
    rect.top	= 0;
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    cur_rect = rect;
    title_rect =rect;
    other_rect=rect;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_BLACK_COLOR;
    
    if(win_is_wating)
    {
          MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_HEALTH_GET_WAIT,&cur_str_t);   
                                                  GUISTR_DrawTextToLCDInRect( 
                                                  (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                                  (const GUI_RECT_T      *)&cur_rect,       
                                                  (const GUI_RECT_T      *)&cur_rect,     
                                                  (const MMI_STRING_T    *)&cur_str_t,
                                                  &text_style,
                                                  state,
                                                  GUISTR_TEXT_DIR_AUTO
                                                  ); 
      }
      else
      { 
            text_style.align = ALIGN_HMIDDLE;
            text_style.font = f_big;
            text_style.font_color = MMI_BLACK_COLOR;
            title_rect.top = 15;
            title_rect.bottom = 45;
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_HEALTH_HEALTH_CODE,&cur_str_t);   
            GUISTR_DrawTextToLCDInRect( 
            MMITHEME_GetDefaultLcdDev(),
            (const GUI_RECT_T      *)&title_rect,
            (const GUI_RECT_T      *)&title_rect,
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 

            other_rect.top = 46;	
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = f_big;
            text_style.font_color = MMI_BLACK_COLOR;
            //MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_HEALTH_OTHER,&other_str_t);
            if(zdt_health_display_type == 2)
            {
                other_str_t.wstr_ptr = over_time;
            }
            else
            {
                other_str_t.wstr_ptr = search_fail;
            }
            other_str_t.wstr_len = MMIAPICOM_Wstrlen(other_str_t.wstr_ptr);
            GUISTR_DrawTextToLCDInRect( 
                                                MMITHEME_GetDefaultLcdDev(),
                                                (const GUI_RECT_T      *)&other_rect,       
                                                (const GUI_RECT_T      *)&other_rect,     
                                                (const MMI_STRING_T    *)&other_str_t,
                                                &text_style,
                                                state,
                                                GUISTR_TEXT_DIR_AUTO
                                                ); 
            MMITHEME_UpdateRect();
	}
	return;
}

LOCAL void MMIZDT_Health_StopTimer()
{
    if(zdt_health_timer_id != 0)
    {
        MMK_StopTimer(zdt_health_timer_id);
        zdt_health_timer_id = 0;
    }
}
void Health_timer_handler(uint8 timer_id,uint32 param)
{
    #ifdef WIN32
        if(zdt_health_display_type == 2)
        {
            MMIZDT_UpdateHealthWin(2);
        }
        else
        {
            MMIZDT_UpdateHealthWin(1);
        }
    #else
        MMIZDT_UpdateHealthWin(0);
    #endif
    return;
}

#ifdef WIN32
const char * health_type1_test_data = "{\"nucleicAcidTime\":\"\",\"userName\":\"閮慍T316TP\",\"healthyCodeSource\":\"绂忓缓鐪佸仴搴风爜\",\"vaccinesStatus\":0,\"nucleicAcid\":\"UNKNOWN\",\"healthStatus\":\"TX\",\"time\":\"2023-01-30 13:38\"}";
//const char * health_type2_test_data = "{\"qrcode\":\"FJQRJKMC1000000101000000A001eYxlYVeTjfLbT0ZrIs1dlcS1z4ewqUC+SjiB2JxjGs82sWYF2bIWQy+6efhNN2bi1674985035362462653DA1F5C04C2D4429A5E491F284A48527EB99634CBBE88A32C846178A\",\"userName\":\"*******P\",\"type\":3,\"validityDate\":14}";
const char * health_type2_test_data = "{\"qrcode\":\"https://szgd.tgovcloud.com/preorder/emergency/qrcode/mini?options=FJZg4I5ulXAD3JjVnJh0T1vyjRzxh_dhBeEMi8mJgRugTpG9eMQvBg_8Z69LqV11&794e169bd24b4e51ba7d6d0e3e42719d&9dc57501308c4d50b0bc16f05f8f1676&ly=san|jk=01|hs=1|zq=024|cy=|ym=02|jz=20211212|sjc=20221019221345\",\"userName\":\"**1\",\"type\":1,\"qrcodeBase64\":\"111\",\"validityDate\":14}";
#endif
LOCAL void MMIZDT_StartWindTimer()
{
    uint32 time_ms = 15*1000;
    MMIZDT_Health_StopTimer();
    #ifdef WIN32
        time_ms = 1000;
        if(zdt_health_display_type == 2)
        {
            YX_Net_HTTP_Health2_DATA_Handle(health_type2_test_data,strlen(health_type2_test_data));
        }
        else
        {
            YX_Net_HTTP_Health_Handle(health_type1_test_data,strlen(health_type1_test_data));
        }
    #endif
    zdt_health_timer_id = MMK_CreateTimerCallback(time_ms, 
    											   Health_timer_handler, 
    											    (uint32)PNULL, 
    											     FALSE);
	return;

}


void health_qr_fillrect(int x1, int y1, int x2, int y2,  GUI_COLOR_T color)
{
    const GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};
    rect.left = x1;
    rect.top = y1;
    rect.right = x2;
    rect.bottom = y2;

    GUI_FillRect( &lcd_dev_info, rect, color);
}

void health_showQR(uint8_t *data, int size, int pos_x, int pos_y, int zoom_val,uint16 disp_color)
{
    int i=0,j=0,a=0;  
    int x1, y1, x2, y2;
    uint8_t cur_data = 0;
    uint8_t cur_bit = 0;
    uint8_t cur_val = 0;
    for(i=0;i<size;i++)  
    {  
        for(j=0;j<size;j++)
        {  
            a = i * size + j;
            cur_data = data[a / 8];
            cur_bit = a%8;
            cur_bit = 7 - cur_bit;
            cur_val = cur_data & (0x01 << cur_bit);
        	x1 = pos_x + j * zoom_val;
        	y1 = pos_y + i * zoom_val;
        	x2 = x1 + zoom_val-1;
        	y2 = y1 + zoom_val-1;
            if(cur_val)
            {  
                health_qr_fillrect(x1,y1,x2,y2,disp_color);    
            }  
            else  
            {  
                health_qr_fillrect(x1,y1,x2,y2,MMI_WHITE_COLOR);    
            }  
        }  
    }  
}

LOCAL BOOLEAN  Health_Show_2VM_QR(uint8 * encode_code, uint16 disp_color)
{
    int x,y;
    int zoom_val;
    const char * ewm_str = encode_code;
    uint8_t bitdata[QR_MAX_BITDATA] = {0};
    int side = 0;
    uint16 ewm_len = strlen(ewm_str);
    
    side = qr_encode(QR_LEVEL_Q, 0, ewm_str, ewm_len, bitdata);
    #if 0
        ZFB_Setting_2VM_BmpShow(bitdata,side);
    #else
    if(side > HEALTH_SETTING_DISP_WIDTH)
    {
        if(side < MMI_MAINSCREEN_WIDTH)
        {
            zoom_val = MMI_MAINSCREEN_WIDTH/side;
            x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
            y = (HEALTH_SETTING_DISP_WIDTH - (zoom_val*side))/2;
        }
        else
        {
            //显示异常
            x = 0;
            y = HEALTH_SETTING_DISP_Y;
            zoom_val = 1;
        }
    }
    else
    {
        zoom_val = HEALTH_SETTING_DISP_WIDTH/side;
        x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
        y = (HEALTH_SETTING_DISP_WIDTH - (zoom_val*side))/2+HEALTH_SETTING_DISP_Y;
    }
    health_showQR(bitdata, side, x, y, zoom_val,disp_color);
    //GT_Qcode_show(ewm_str, HEALTH_SETTING_DISP_X, HEALTH_SETTING_DISP_Y, HEALTH_SETTING_DISP_WIDTH, HEALTH_SETTING_DISP_HEIGHT);
    #endif
    return TRUE;
}

LOCAL BOOLEAN  Health_Show_URL_2VM_QR(uint8 * encode_code, uint16 disp_color)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    int x,y;
    int zoom_val;
    const char * ewm_str = encode_code;
    uint8_t bitdata[QR_MAX_BITDATA] = {0};
    int side = 0;
    int disp_width = 0;
    uint16 ewm_len = strlen(ewm_str);
    GUI_RECT_T	 rect		 = {0}; 
    
    side = qr_encode(QR_LEVEL_Q, 0, ewm_str, ewm_len, bitdata);
    #if 0
        ZFB_Setting_2VM_BmpShow(bitdata,side);
    #else
    if(side > HEALTH_SETTING_2VM_DISP_WIDTH)
    {
        if(side < MMI_MAINSCREEN_WIDTH)
        {
            zoom_val = MMI_MAINSCREEN_WIDTH/side;
            disp_width = zoom_val*side;
            x = (MMI_MAINSCREEN_WIDTH - disp_width)/2;
            y = (HEALTH_SETTING_2VM_DISP_WIDTH - disp_width)/2;
        }
        else
        {
            //显示异常
            x = 0;
            y = HEALTH_SETTING_2VM_DISP_Y;
            disp_width = side;
            zoom_val = 1;
        }
    }
    else
    {
        zoom_val = HEALTH_SETTING_2VM_DISP_WIDTH/side;
        disp_width = zoom_val*side;
        x = (MMI_MAINSCREEN_WIDTH - disp_width)/2;
        y = (HEALTH_SETTING_2VM_DISP_WIDTH - disp_width)/2+HEALTH_SETTING_2VM_DISP_Y;
    }
    rect.left	= x-4;
    if(rect.left < 0)
    {
        rect.left = 0;
    }
    rect.top	= y-4;
    if(rect.top < 0)
    {
        rect.top = 0;
    }
    rect.right	= rect.left+disp_width+8;
    if(rect.right > MMI_MAINSCREEN_WIDTH-1)
    {
        rect.right = MMI_MAINSCREEN_WIDTH-1;
    }
    rect.bottom = rect.top+disp_width+8;
    if(rect.bottom > MMI_MAINSCREEN_HEIGHT-1)
    {
        rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    }
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    health_showQR(bitdata, side, x, y, zoom_val,disp_color);
    //GT_Qcode_show(ewm_str, HEALTH_SETTING_DISP_X, HEALTH_SETTING_DISP_Y, HEALTH_SETTING_DISP_WIDTH, HEALTH_SETTING_DISP_HEIGHT);
    #endif
    return TRUE;
}

void MMIZDT_Health_2VM_Show(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T	 rect		 = {0}; 
    
    rect.left	= 0;
    rect.top	= 0; 
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
    if(1 == yx_health_msg_Rec.code_status)
    {
        if(SCI_STRLEN(yx_health_msg_Rec.code_content) > 0)
        {
            Health_Show_2VM_QR(yx_health_msg_Rec.code_content,HEALTH_GREEN_COLOR);
        }
        else
        {
            Health_Show_2VM_QR(g_zdt_phone_imei,HEALTH_GREEN_COLOR);
        }
    }
    else
    {
        if(SCI_STRLEN(yx_health_msg_Rec.code_content) > 0)
        {
            Health_Show_2VM_QR(yx_health_msg_Rec.code_content,HEALTH_RED_COLOR);
        }
        else
        {
            Health_Show_2VM_QR(g_zdt_phone_imei,HEALTH_RED_COLOR);
        }
    }
    return;
}

LOCAL MMI_RESULT_E  HandleZDT_Haelth2VMWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
	MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
	GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
	GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	GUISTR_STYLE_T		text_style = {0};
	GUISTR_STATE_T		state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
	GUI_RECT_T rect = {0,0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT -1)};
	MMI_STRING_T cur_str_t	 = {0};
	GUI_RECT_T	  cur_rect			= {0};
	GUI_FONT_T f_big =SONG_FONT_42;
       BOOLEAN res = FALSE;
       
       text_style.align = ALIGN_HVMIDDLE;
       text_style.font = f_big;
       text_style.font_color = MMI_BLACK_COLOR;
	switch(msg_id) {
		case MSG_OPEN_WINDOW:
			break;
            
               case MSG_FULL_PAINT:
		     	 MMIZDT_Health_2VM_Show(win_id);
           	     break;
		
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_jp_tp_x = point.x;
			s_jp_tp_y = point.y;
			s_is_jp_tp_down = TRUE;
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(s_is_jp_tp_down)
			{
				offset_y =	point.y - s_jp_tp_y;
				offset_x =	point.x - s_jp_tp_x;
				if(offset_x <= -(20))	
				{
                            MMIZDT_OpenType1NucleicWin();
                            MMK_CloseWin(win_id);
				}	
				else if(offset_x >= (20))
				{
                            MMIZDT_OpenHealthMainWin();
                            MMK_CloseWin(win_id);
				}

			}
			s_is_jp_tp_down = FALSE;
		}
		break;
		
	case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
	case MSG_TP_PRESS_LONG:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
          
             case MSG_APP_UP:
		case MSG_APP_RIGHT:
                    MMIZDT_OpenType1NucleicWin();
                    MMK_CloseWin(win_id);
                break;
                
            case MSG_APP_DOWN:
		case MSG_APP_LEFT:
                    MMIZDT_OpenHealthMainWin();
                    MMK_CloseWin(win_id);
                break;
                
		case MSG_CTL_MIDSK:
			break;
            
		case MSG_LOSE_FOCUS:
                	break;
                    
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
                        MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
			break;
			
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;

}

WINDOW_TABLE( MMIZDT_WATCH_HEALTH_2VM_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_Haelth2VMWinMsg),    
    WIN_ID( MMIZDT_WATCH_HEALTH_2VM_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenHealth2VMWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_HEALTH_2VM_WIN_TAB,PNULL);
    return TRUE;
}

void MMIZDT_Health_Show(MMI_WIN_ID_T win_id)
{
    GUI_POINT_T   point = {0};
    wchar temp_wstr[HTTP_HEALTH_MSG_MAX_LEN + 1] = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T	 rect		 = {0}; 
    GUI_RECT_T	 txt_rect		   = {0};
    MMI_STRING_T	cur_str_t	= {0};
    GUISTR_STYLE_T		text_style		= {0};
    GUI_POINT_T 				dis_point = {0,0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    
    rect.left	= 0;
    rect.top	= 0; 
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    txt_rect=rect;

    GUI_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);
    if(1 == yx_health_msg_Rec.code_status)
    {
        GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_TRIP_GREEN,&lcd_dev_info);
    }
    else
    {
        GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_TRIP_RED,&lcd_dev_info);
    }
    
    if(strlen(yx_health_msg_Rec.personName) > 0)
    {
        text_style.align = ALIGN_HMIDDLE;
        text_style.font = SONG_FONT_20;
        text_style.font_color = MMI_WHITE_COLOR;
        state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_BOLD; 
        GUI_UTF8ToWstr(temp_wstr,HTTP_HEALTH_MSG_MAX_LEN,yx_health_msg_Rec.personName,strlen(yx_health_msg_Rec.personName));	
        cur_str_t.wstr_ptr = temp_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
        txt_rect.top=4;
        txt_rect.bottom = 24;
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
            
    }
    
    if(strlen(yx_health_msg_Rec.code_time) > 0)
    {
        text_style.align = ALIGN_HMIDDLE;
        text_style.font = SONG_FONT_20;
        text_style.font_color = MMI_WHITE_COLOR;
        state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_BOLD; 
        GUI_UTF8ToWstr(temp_wstr,HTTP_HEALTH_MSG_MAX_LEN,yx_health_msg_Rec.code_time,strlen(yx_health_msg_Rec.code_time));	
        cur_str_t.wstr_ptr = temp_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
        txt_rect=rect;
        txt_rect.top = txt_rect.bottom-20;
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
            
    }
    return;
}

LOCAL MMI_RESULT_E  HandleZDT_HaelthWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
	MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
	GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
	GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	GUISTR_STYLE_T		text_style = {0};
	GUISTR_STATE_T		state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
	GUI_RECT_T rect = {0,0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT -1)};
	MMI_STRING_T cur_str_t	 = {0};
	GUI_RECT_T	  cur_rect			= {0};
	GUI_FONT_T f_big =SONG_FONT_42;
       BOOLEAN res = FALSE;
       
       text_style.align = ALIGN_HVMIDDLE;
       text_style.font = f_big;
       text_style.font_color = MMI_BLACK_COLOR;
	switch(msg_id) {
		case MSG_OPEN_WINDOW:
			GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);	
			break;
            
           case MSG_FULL_PAINT:
                    MMIZDT_Health_Show(win_id);
           	     break;
		
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_jp_tp_x = point.x;
			s_jp_tp_y = point.y;
			s_is_jp_tp_down = TRUE;
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(s_is_jp_tp_down)
			{
				offset_y =	point.y - s_jp_tp_y;
				offset_x =	point.x - s_jp_tp_x;
				if(offset_x <= -(20))	
				{
                                MMIZDT_OpenHealth2VMWin();
					MMK_CloseWin(win_id);
				}	
				else if(offset_x >= (20))
				{
                                MMIZDT_OpenHealthInfoWin();
					MMK_CloseWin(win_id);
				}

			}
			s_is_jp_tp_down = FALSE;
		}
		break;
		
	case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
	case MSG_TP_PRESS_LONG:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

            
             case MSG_APP_UP:
		case MSG_APP_RIGHT:
                        MMIZDT_OpenHealth2VMWin();
                        MMK_CloseWin(win_id);
                break;
                
            case MSG_APP_DOWN:
		case MSG_APP_LEFT:
                    MMIZDT_OpenHealthInfoWin();
                    MMK_CloseWin(win_id);
                break;
                
		case MSG_CTL_MIDSK:
			break;
            
		case MSG_LOSE_FOCUS:
                	break;
                    
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
                        MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
			break;
			
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;

}

WINDOW_TABLE( MMIZDT_WATCH_HEALTH_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_HaelthWinMsg),    
    WIN_ID( MMIZDT_WATCH_HEALTH_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenHealthMainWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_HEALTH_WIN_TAB,PNULL);
    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZDT_HaelthWaitWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
	MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
	GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
	GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	GUISTR_STYLE_T		text_style = {0};
	GUISTR_STATE_T		state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
	GUI_RECT_T rect = {0,0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT -1)};
	MMI_STRING_T cur_str_t	 = {0};
	GUI_RECT_T	  cur_rect			= {0};
	GUI_FONT_T f_big =SONG_FONT_42;
       text_style.align = ALIGN_HVMIDDLE;
       text_style.font = f_big;
       text_style.font_color = MMI_BLACK_COLOR;
	switch(msg_id) {
		case MSG_OPEN_WINDOW:
                    ZdtHealth_BackLight(TRUE);
                    MMIZDT_StartWindTimer();
                    win_is_wating = TRUE;
			GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);	
			break;
            
           case MSG_FULL_PAINT:
                    MMIZDT_Health_Win_Wating(win_id);
           	     break;
		
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_jp_tp_x = point.x;
			s_jp_tp_y = point.y;
			s_is_jp_tp_down = TRUE;
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(s_is_jp_tp_down)
			{
				offset_y =	point.y - s_jp_tp_y;
				offset_x =	point.x - s_jp_tp_x;
				if(offset_x <= -(20))	
				{
					MMK_CloseWin(win_id);
				}	
				else if(offset_x >= (20))
				{
					MMK_CloseWin(win_id);
				}

			}
			s_is_jp_tp_down = FALSE;
		}
		break;
		
	case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
	case MSG_TP_PRESS_LONG:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
#endif

		case MSG_CTL_MIDSK:
                    break;
            
		case MSG_LOSE_FOCUS:
			  MMIZDT_Health_StopTimer();
                    break;
                
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
                        if(win_is_wating == FALSE)
                        {
                            MMK_CloseWin(win_id);
                        }
			break;

		case MSG_CLOSE_WINDOW:
                        MMIZDT_Health_StopTimer();
                        ZdtHealth_BackLight(FALSE);
			break;
			
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;

}

WINDOW_TABLE( MMIZDT_WATCH_HEALTH_WAIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_HaelthWaitWinMsg),    
    WIN_ID(MMIZDT_WATCH_HEALTH_WAIT_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenHealthWait_Win(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_HEALTH_WAIT_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseHealthWait_Win(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_HEALTH_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_HEALTH_WAIT_WIN_ID);
    }
    return TRUE;
}

BOOLEAN MMIZDT_UpdateHealthWait_Win(void)
{
    if(MMK_IsFocusWin(MMIZDT_WATCH_HEALTH_WAIT_WIN_ID))
    {
        MMK_PostMsg(MMIZDT_WATCH_HEALTH_WAIT_WIN_ID,MSG_FULL_PAINT,PNULL,0);
    }
    return TRUE;
}

BOOLEAN MMIZDT_OpenHealthType1_Win(void)
{
    BOOLEAN res = FALSE;
    uint8 cur_status = MMIZDT_Health_TYPE1_IsValid();
    if((cur_status & 0x01) > 0)
    {
        MMIZDT_OpenHealthMainWin();
        res = TRUE;
    }
    else if((cur_status & 0x02) > 0)
    {
        MMIZDT_OpenType1NucleicWin();
        res = TRUE;
    }
    else if((cur_status & 0x04) > 0)
    {
        MMIZDT_OpenType1AccWin();
        res = TRUE;
    }
    return res;
}

BOOLEAN MMIZDT_CloseHealthType1_Win(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_HEALTH_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_HEALTH_WIN_ID);
    }
    return TRUE;
}

BOOLEAN MMIZDT_OpenHealthType2_Win(void)
{
    BOOLEAN res = FALSE;
    uint8 cur_status = MMIZDT_Health_TYPE2_IsValid();
    if((cur_status & 0x01) > 0)
    {
        MMIZDT_OpenUrlHealthWin();
        res = TRUE;
    }
    return res;
}

BOOLEAN MMIZDT_OpenHealthWin(void)
{
    BOOLEAN res = FALSE;
    zdt_health_display_type = 1;
    res = MMIZDT_OpenHealthType1_Win();
    if(res == FALSE)
    {
        YX_Net_HTTP_Health_Send();
        MMIZDT_OpenHealthWait_Win();
    }
    return res;
}

BOOLEAN MMIZDT_OpenHealth2Win(void)
{
    BOOLEAN res = FALSE;
    zdt_health_display_type = 2;
    YX_Net_HTTP_Health2_Send();
    MMIZDT_OpenHealthWait_Win();
    return res;
}

BOOLEAN MMIZDT_UpdateHealthWin(uint8 is_url_pic)
{
    BOOLEAN url_res = FALSE;
    MMIZDT_Health_StopTimer();
    win_is_wating = FALSE;
    if(is_url_pic == 1)
    {
        //旧接口返回
        if(MMIZDT_OpenHealthType1_Win())
        {
            MMIZDT_CloseHealthWait_Win();
        }
        else
        {
            MMIZDT_UpdateHealthWait_Win();
        }
        
    }
    else if(is_url_pic == 2)
    {
        //新接口返回
        if(MMIZDT_OpenHealthType2_Win())
        {
            MMIZDT_CloseHealthWait_Win();
        }
        else
        {
            MMIZDT_UpdateHealthWait_Win();
        }
    }
    else
    {
        //超时返回
        MMIZDT_UpdateHealthWait_Win();
    }
    return TRUE;
}

#if 1
LOCAL uint16 thir_idle_down_x = 0;
LOCAL uint16 thir_idle_down_y = 0;
static int16 thir_offset_x = 0;
static uint16 thir_abs_x = 0;
void MMIZDT_Health_URL_2VM_Show(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T	 rect		 = {0}; 
    uint16 disp_color = HEALTH_GREEN_COLOR;
    
    rect.left	= 0;
    rect.top	= 0; 
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;

    GUI_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);

    if(yx_health_msg_Rec.type2_color == 1)
    {
        disp_color = HEALTH_GREEN_COLOR;
    }
    else if(yx_health_msg_Rec.type2_color == 2)
    {
        disp_color = HEALTH_YELLOW_COLOR;
    }
    else if(yx_health_msg_Rec.type2_color == 3)
    {
        disp_color = HEALTH_RED_COLOR;
    }
    
    if(SCI_STRLEN(yx_health_msg_Rec.type2_qrcode) > 0)
    {
        Health_Show_URL_2VM_QR(yx_health_msg_Rec.type2_qrcode,disp_color);
    }
    else
    {
        Health_Show_URL_2VM_QR(g_zdt_phone_imei,disp_color);
    }
    return;
}

LOCAL MMI_RESULT_E HandleZDT_UrlHealthMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E			recode	= MMI_RESULT_TRUE;
    BOOLEAN ret = FALSE;
    GUI_POINT_T 				point = {0};
    GUI_LCD_DEV_INFO			 lcd_dev_info ={ GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRect();
    MMI_CTRL_ID_T				ctrl_id = MMIZDT_URL_HEALTH_IMAGE_CTRL_ID;
    GUI_RECT_T	title_rect={0,0,(MMI_MAINSCREEN_WIDTH -1),19};
    MMI_STRING_T tmp_string = {0};
    GUISTR_STYLE_T		text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    uint16 tittle[10+HTTP_HEALTH_NAME_MAX_LEN] = {0x59D3, 0x540D, 0x003A, 0}; //59D3 540D 003A 
    BOOLEAN res = FALSE;
    MMI_CONTROL_CREATE_T    create    = {0};
    GUIANIM_INIT_DATA_T anim_init = {0};
    
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
		{
			GUI_BOTH_RECT_T both_rect = {0};

                    both_rect.h_rect.left = 0;
                    both_rect.h_rect.top = 20;
                    both_rect.h_rect.bottom = 239;
                    both_rect.h_rect.right = 239;
                    both_rect.v_rect.left = 0;
                    both_rect.v_rect.top = 20;
                    both_rect.v_rect.bottom = 239;
                    both_rect.v_rect.right = 239;
            
			GUI_FillRect(&lcd_dev_info,bg_rect, MMI_BLACK_COLOR);

                    if(yx_health_msg_Rec.type2_type == 1 && MMIFILE_IsFileExist(ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH_W)))
                    {
                          create.ctrl_id           = ctrl_id;
                          create.guid              = SPRD_GUI_ANIM_ID;
                          create.parent_win_handle = win_id;
                          create.init_data_ptr     = &anim_init;
                         if (PNULL != MMK_CreateControl(&create))
                         {
                            GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
                            recode = MMIAPIFMM_SetPreviewPicParam (FALSE, ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH_W), ctrl_id);
                            MMK_SetAtvCtrl (win_id, ctrl_id);
                         }
                    }
		}
		break;
		
		case MSG_FULL_PAINT:
		{
                    uint16 name_wstr[HTTP_HEALTH_NAME_MAX_LEN+1] = {0};
                         
                    if(yx_health_msg_Rec.type2_type == 1 && MMIFILE_IsFileExist(ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_HEALTH_URL_BASE64_FULLPACH_W)))
                    {
                    }
                    else if(yx_health_msg_Rec.type2_type == 2 || yx_health_msg_Rec.type2_type == 3)
                    {
                        MMIZDT_Health_URL_2VM_Show(win_id);
                    }
                    else
                    {
                        MMIZDT_Health_URL_2VM_Show(win_id);
                    }
                    if(strlen(yx_health_msg_Rec.type2_username) > 0)
                    {
                       GUI_UTF8ToWstr(name_wstr,HTTP_HEALTH_MSG_MAX_LEN,yx_health_msg_Rec.type2_username,strlen(yx_health_msg_Rec.type2_username));	
                       MMIAPICOM_Wstrcat(tittle, name_wstr);
                    }
                    tmp_string.wstr_ptr = tittle;
                    tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
                    text_style.align = ALIGN_HVMIDDLE;
                    text_style.font = SONG_FONT_20;
                    text_style.font_color = MMI_WHITE_COLOR;
                    text_style.line_space = 0;
                    text_style.region_num = 0;
                    text_style.region_ptr = PNULL;
                    GUISTR_DrawTextToLCDInRect(
                    	 MMITHEME_GetDefaultLcdDev(),
                    	 (const GUI_RECT_T		*)&title_rect,		//the fixed display area
                    	 (const GUI_RECT_T		*)&title_rect,		//用户要剪切的实际区域
                    	 (const MMI_STRING_T	*)&tmp_string,
                    	 &text_style,
                    	 state,
                    	 GUISTR_TEXT_DIR_AUTO
                    	 );
		}
		break;
			
#if 1//def TOUCH_PANEL_SUPPORT
		case MSG_TP_PRESS_DOWN:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				thir_idle_down_x = point.x;
				thir_idle_down_y = point.y;
				
			}
			break;
			
		case MSG_TP_PRESS_UP:
			{
				GUI_POINT_T   point = {0};
				
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				
				thir_offset_x = point.x - thir_idle_down_x;
				thir_abs_x = z_abs(thir_offset_x);
				
				if( (thir_abs_x > 20) && (thir_offset_x<0 ) )
				{
				}
				else if( (thir_abs_x > 20) && (thir_offset_x>0 ) )
				{
				}
			}
				
			break;
			
		case MSG_TP_PRESS_MOVE:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
		case MSG_TP_PRESS_LONG:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_KEYUP_UP:
            case MSG_KEYUP_DOWN:
                break;
                
		case MSG_APP_RIGHT:
                break;
                
		case MSG_APP_LEFT:
                break;
                
		//case MSG_APP_OK:
		//case MSG_APP_WEB:
		case MSG_CTL_MIDSK:
			break;
		case MSG_TIMER:
		
		break;
		case MSG_BACKLIGHT_TURN_ON:
		case MSG_BACKLIGHT_TURN_OFF:

		break;
		case MSG_LOSE_FOCUS:
                break;
		case MSG_CLOSE_WINDOW:
                MMK_DestroyControl(ctrl_id);
		break;

		case MSG_KEYUP_CANCEL:
		case MSG_KEYUP_RED:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;


		default:
			recode = MMI_RESULT_FALSE;
			break;

	}
	
	return recode;
}

WINDOW_TABLE( MMIZDT_URL_HEALTH_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_UrlHealthMsg),    
    WIN_ID(MMIZDT_URL_HEALTH_WIN_ID),
    WIN_HIDE_STATUS,
    //CREATE_ANIM_CTRL (MMIZDT_URL_HEALTH_IMAGE_CTRL_ID, MMIZDT_URL_HEALTH_WIN_ID),
    END_WIN
};

BOOLEAN MMIZDT_OpenUrlHealthWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_URL_HEALTH_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseUrlHealthWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_URL_HEALTH_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_URL_HEALTH_WIN_ID);
    }
    return TRUE;
}

LOCAL MMI_RESULT_E HandleZDT_UrlNucleicMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E			recode	= MMI_RESULT_TRUE;
    BOOLEAN ret = FALSE;
    GUI_POINT_T 				point = {0};
    GUI_LCD_DEV_INFO			 lcd_dev_info ={ GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRect();
    MMI_CTRL_ID_T				ctrl_id = MMIZDT_URL_NUCLEIC_IMAGE_CTRL_ID;
    GUI_RECT_T	title_rect={0,0,(MMI_MAINSCREEN_WIDTH -1),19};
    MMI_STRING_T tmp_string = {0};
    GUISTR_STYLE_T		text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    uint16 tittle[10] = {0x6838, 0x9178, 0x7801, 0}; //核酸码6838 9178 7801 
    BOOLEAN res = FALSE;
    
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
		{
			GUI_BOTH_RECT_T both_rect = {0};

                    both_rect.h_rect.left = 0;
                    both_rect.h_rect.top = 20;
                    both_rect.h_rect.bottom = 239;
                    both_rect.h_rect.right = 239;
                    both_rect.v_rect.left = 0;
                    both_rect.v_rect.top = 20;
                    both_rect.v_rect.bottom = 239;
                    both_rect.v_rect.right = 239;
            
			GUI_FillRect(&lcd_dev_info,bg_rect, MMI_BLACK_COLOR);

			GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
			
			recode = MMIAPIFMM_SetPreviewPicParam (FALSE, ZDT_HTTP_NUCLEIC_URL_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_NUCLEIC_URL_FULLPACH_W), ctrl_id);
			
			MMK_SetAtvCtrl (win_id, ctrl_id);
		}
		break;
		
		case MSG_FULL_PAINT:
		{

        		 tmp_string.wstr_ptr = tittle;
        		 tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
        		 text_style.align = ALIGN_HVMIDDLE;
        		 text_style.font = SONG_FONT_16;
        		 text_style.font_color = MMI_WHITE_COLOR;
        		 text_style.line_space = 0;
        		 text_style.region_num = 0;
        		 text_style.region_ptr = PNULL;  
        		 GUISTR_DrawTextToLCDInRect( 
        					 MMITHEME_GetDefaultLcdDev(),
        					 (const GUI_RECT_T		*)&title_rect,		//the fixed display area
        					 (const GUI_RECT_T		*)&title_rect,		//??????????
        					 (const MMI_STRING_T	*)&tmp_string,
        					 &text_style,
        					 state,
        					 GUISTR_TEXT_DIR_AUTO
        					 );
        		 
		}
			break;
			
#if 1//def TOUCH_PANEL_SUPPORT
		case MSG_TP_PRESS_DOWN:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				thir_idle_down_x = point.x;
				thir_idle_down_y = point.y;
				
			}
			break;
			
		case MSG_TP_PRESS_UP:
			{
				GUI_POINT_T   point = {0};
				
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				
				thir_offset_x = point.x - thir_idle_down_x;
				thir_abs_x = z_abs(thir_offset_x);
				
				if( (thir_abs_x > 20) && (thir_offset_x<0 ) )
				{
					MMIZDT_OpenUrlHealthWin();
                                MMK_CloseWin(win_id);
				}
				else if( (thir_abs_x > 20) && (thir_offset_x>0 ) )
				{
					MMIZDT_OpenUrlAccWin();
					MMK_CloseWin(win_id);
				}
			}
				
			break;
			
		case MSG_TP_PRESS_MOVE:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
		case MSG_TP_PRESS_LONG:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

            case MSG_KEYUP_UP:
            case MSG_KEYUP_DOWN:
                    MMIZDT_OpenHealthInfoWin();
                break;
                
		case MSG_APP_RIGHT:
                    res = MMIZDT_OpenUrlHealthWin();
                    if(res == FALSE)
                    {
                        res = MMIZDT_OpenUrlAccWin();
                    }
                    if(res)
                    {
                        MMK_CloseWin(win_id);
                    }
                break;
                
		case MSG_APP_LEFT:
                    res = MMIZDT_OpenUrlAccWin();
                    if(res == FALSE)
                    {
                        res = MMIZDT_OpenUrlHealthWin();
                    }
                    if(res)
                    {
                        MMK_CloseWin(win_id);
                    }
                break;
            
		//case MSG_APP_OK:
		//case MSG_APP_WEB:
		case MSG_CTL_MIDSK:
			break;
		case MSG_TIMER:
		
		break;
		case MSG_BACKLIGHT_TURN_ON:
		case MSG_BACKLIGHT_TURN_OFF:

		break;
		case MSG_LOSE_FOCUS:
		case MSG_CLOSE_WINDOW:
		break;

		case MSG_KEYUP_CANCEL:
		case MSG_KEYUP_RED:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;


		default:
			recode = MMI_RESULT_FALSE;
			break;

	}
	
	return recode;
}

WINDOW_TABLE( MMIZDT_URL_NUCLEIC_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_UrlNucleicMsg),    
    WIN_ID(MMIZDT_URL_NUCLEIC_WIN_ID),
    CREATE_ANIM_CTRL (MMIZDT_URL_NUCLEIC_IMAGE_CTRL_ID, MMIZDT_URL_NUCLEIC_WIN_ID),
    END_WIN
};

BOOLEAN MMIZDT_OpenUrlNucleicWin(void)
{
    if(MMIFILE_IsFileExist(ZDT_HTTP_NUCLEIC_URL_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_NUCLEIC_URL_FULLPACH_W)))
    {
        MMK_CreateWin((uint32*)MMIZDT_URL_NUCLEIC_WIN_TAB,PNULL);
        return TRUE;
    }
    return FALSE;
}

BOOLEAN MMIZDT_CloseUrlNucleicWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_URL_NUCLEIC_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_URL_NUCLEIC_WIN_ID);
    }
    return TRUE;
}

LOCAL MMI_RESULT_E HandleZDT_UrlAccMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E			recode	= MMI_RESULT_TRUE;
    BOOLEAN ret = FALSE;
    GUI_POINT_T 				point = {0};
    GUI_LCD_DEV_INFO			 lcd_dev_info ={ GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRect();
    MMI_CTRL_ID_T				ctrl_id = MMIZDT_URL_ACC_IMAGE_CTRL_ID;
    GUI_RECT_T	title_rect={0,0,(MMI_MAINSCREEN_WIDTH -1),19};
    MMI_STRING_T tmp_string = {0};
    GUISTR_STYLE_T		text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    uint16 tittle[10] = {0x884C, 0x7A0B, 0x7801, 0}; //行程码884C 7A0B 7801 
    BOOLEAN res = FALSE;
    
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
		{
			GUI_BOTH_RECT_T both_rect = {0};

                    both_rect.h_rect.left = 0;
                    both_rect.h_rect.top = 20;
                    both_rect.h_rect.bottom = 239;
                    both_rect.h_rect.right = 239;
                    both_rect.v_rect.left = 0;
                    both_rect.v_rect.top = 20;
                    both_rect.v_rect.bottom = 239;
                    both_rect.v_rect.right = 239;
            
			GUI_FillRect(&lcd_dev_info,bg_rect, MMI_BLACK_COLOR);

			GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
			
			recode = MMIAPIFMM_SetPreviewPicParam (FALSE, ZDT_HTTP_ACC_URL_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_ACC_URL_FULLPACH_W), ctrl_id);
			
			MMK_SetAtvCtrl (win_id, ctrl_id);
		}
		break;
		
		case MSG_FULL_PAINT:
		{

        		 tmp_string.wstr_ptr = tittle;
        		 tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
        		 text_style.align = ALIGN_HVMIDDLE;
        		 text_style.font = SONG_FONT_16;
        		 text_style.font_color = MMI_WHITE_COLOR;
        		 text_style.line_space = 0;
        		 text_style.region_num = 0;
        		 text_style.region_ptr = PNULL;  
        		 GUISTR_DrawTextToLCDInRect( 
        					 MMITHEME_GetDefaultLcdDev(),
        					 (const GUI_RECT_T		*)&title_rect,		//the fixed display area
        					 (const GUI_RECT_T		*)&title_rect,		//??????????
        					 (const MMI_STRING_T	*)&tmp_string,
        					 &text_style,
        					 state,
        					 GUISTR_TEXT_DIR_AUTO
        					 );
        		 
		}
			break;
			
#if 1//def TOUCH_PANEL_SUPPORT
		case MSG_TP_PRESS_DOWN:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				thir_idle_down_x = point.x;
				thir_idle_down_y = point.y;
				
			}
			break;
			
		case MSG_TP_PRESS_UP:
			{
				GUI_POINT_T   point = {0};
				
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				
				thir_offset_x = point.x - thir_idle_down_x;
				thir_abs_x = z_abs(thir_offset_x);
				
				if( (thir_abs_x > 20) && (thir_offset_x<0 ) )
				{
                                res = MMIZDT_OpenUrlNucleicWin();
                                if(res == FALSE)
                                {
                                    res = MMIZDT_OpenUrlHealthWin();
                                }
                                if(res)
                                {
                                    MMK_CloseWin(win_id);
                                }
				}
				else if( (thir_abs_x > 20) && (thir_offset_x>0 ) )
				{
                                res = MMIZDT_OpenUrlHealthWin();
                                if(res == FALSE)
                                {
                                    res = MMIZDT_OpenUrlNucleicWin();
                                }
                                if(res)
                                {
                                    MMK_CloseWin(win_id);
                                }
				}
			}
				
			break;
			
		case MSG_TP_PRESS_MOVE:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
		case MSG_TP_PRESS_LONG:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

            case MSG_KEYUP_UP:
            case MSG_KEYUP_DOWN:
                    MMIZDT_OpenHealthInfoWin();
                break;

		case MSG_APP_RIGHT:
                    res = MMIZDT_OpenUrlNucleicWin();
                    if(res == FALSE)
                    {
                        res = MMIZDT_OpenUrlHealthWin();
                    }
                    if(res)
                    {
                        MMK_CloseWin(win_id);
                    }
                break;
                
		case MSG_APP_LEFT:
                    res = MMIZDT_OpenUrlHealthWin();
                    if(res == FALSE)
                    {
                        res = MMIZDT_OpenUrlNucleicWin();
                    }
                    if(res)
                    {
                        MMK_CloseWin(win_id);
                    }
                break;
                
		case MSG_CTL_MIDSK:
                break;
                
		case MSG_TIMER:
		
		break;
		case MSG_BACKLIGHT_TURN_ON:
		case MSG_BACKLIGHT_TURN_OFF:

		break;
		case MSG_LOSE_FOCUS:
		case MSG_CLOSE_WINDOW:
		break;

		case MSG_KEYUP_CANCEL:
		case MSG_KEYUP_RED:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;


		default:
			recode = MMI_RESULT_FALSE;
			break;

	}
	
	return recode;
}

WINDOW_TABLE( MMIZDT_URL_ACC_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_UrlAccMsg),    
    WIN_ID(MMIZDT_URL_ACC_WIN_ID),
    CREATE_ANIM_CTRL (MMIZDT_URL_ACC_IMAGE_CTRL_ID, MMIZDT_URL_ACC_WIN_ID),
    END_WIN
};

BOOLEAN MMIZDT_OpenUrlAccWin(void)
{
    if(MMIFILE_IsFileExist(ZDT_HTTP_ACC_URL_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_ACC_URL_FULLPACH_W)))
    {
        MMK_CreateWin((uint32*)MMIZDT_URL_ACC_WIN_TAB,PNULL);
        return TRUE;
    }
    return FALSE;
}

BOOLEAN MMIZDT_CloseUrlAccWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_URL_ACC_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_URL_ACC_WIN_ID);
    }
    return TRUE;
}

#endif

#if 1
LOCAL uint16 health_type1_idle_down_x = 0;
LOCAL uint16 health_type1_idle_down_y = 0;
static int16 health_type1_offset_x = 0;
static uint16 health_type1_abs_x = 0;

LOCAL MMI_RESULT_E HandleZDT_Type1HealthMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E			recode	= MMI_RESULT_TRUE;
    BOOLEAN ret = FALSE;
    GUI_POINT_T 				point = {0};
    GUI_LCD_DEV_INFO			 lcd_dev_info ={ GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRect();
    MMI_CTRL_ID_T				ctrl_id = MMIZDT_TYPE1_HEALTH_IMAGE_CTRL_ID;
    GUI_RECT_T	title_rect={0,0,(MMI_MAINSCREEN_WIDTH -1),19};
    MMI_STRING_T tmp_string = {0};
    GUISTR_STYLE_T		text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    uint16 tittle[10] = {0x5065, 0x5EB7, 0x7801, 0}; //健康码5065 5EB7 7801 
    BOOLEAN res = FALSE;
    
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
		{
			GUI_BOTH_RECT_T both_rect = {0};

			both_rect.h_rect.left = 0;
			both_rect.h_rect.top = 20;
			both_rect.h_rect.bottom = 259;
			both_rect.h_rect.right = 239;
			both_rect.v_rect.left = 0;
			both_rect.v_rect.top = 20;
			both_rect.v_rect.bottom = 259;
			both_rect.v_rect.right = 239;
	
			GUI_FillRect(&lcd_dev_info,bg_rect, MMI_BLACK_COLOR);

			GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
			
			recode = MMIAPIFMM_SetPreviewPicParam (FALSE, ZDT_HTTP_HEALTH_TYPE1_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_HEALTH_TYPE1_FULLPACH_W), ctrl_id);
			
			MMK_SetAtvCtrl (win_id, ctrl_id);
		}
		break;
		
		case MSG_FULL_PAINT:
		{

        		 tmp_string.wstr_ptr = tittle;
        		 tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
        		 text_style.align = ALIGN_HVMIDDLE;
        		 text_style.font = SONG_FONT_16;
        		 text_style.font_color = MMI_WHITE_COLOR;
        		 text_style.line_space = 0;
        		 text_style.region_num = 0;
        		 text_style.region_ptr = PNULL;
        		 GUISTR_DrawTextToLCDInRect(
        					 MMITHEME_GetDefaultLcdDev(),
        					 (const GUI_RECT_T		*)&title_rect,		//the fixed display area
        					 (const GUI_RECT_T		*)&title_rect,		//用户要剪切的实际区域
        					 (const MMI_STRING_T	*)&tmp_string,
        					 &text_style,
        					 state,
        					 GUISTR_TEXT_DIR_AUTO
        					 );
        		 
		}
			break;
			
#if 1//def TOUCH_PANEL_SUPPORT
		case MSG_TP_PRESS_DOWN:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				health_type1_idle_down_x = point.x;
				health_type1_idle_down_y = point.y;
				
			}
			break;
			
		case MSG_TP_PRESS_UP:
			{
				GUI_POINT_T   point = {0};
				
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				
				health_type1_offset_x = point.x - health_type1_idle_down_x;
				health_type1_abs_x = z_abs(health_type1_offset_x);
				
				if( (health_type1_abs_x > 20) && (health_type1_offset_x<0 ) )
				{
                                res = MMIZDT_OpenType1AccWin();
                                if(res == FALSE)
                                {
                                    res = MMIZDT_OpenType1NucleicWin();
                                }
                                if(res)
                                {
                                    MMK_CloseWin(win_id);
                                }
				}
				else if( (health_type1_abs_x > 20) && (health_type1_offset_x>0 ) )
				{
                                res = MMIZDT_OpenType1NucleicWin();
                                if(res == FALSE)
                                {
                                    res = MMIZDT_OpenType1AccWin();
                                }
                                if(res)
                                {
                                    MMK_CloseWin(win_id);
                                }
				}
			}
				
			break;
			
		case MSG_TP_PRESS_MOVE:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
		case MSG_TP_PRESS_LONG:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_KEYUP_UP:
            case MSG_KEYUP_DOWN:
                    MMIZDT_OpenHealthInfoWin();
                break;
            
		case MSG_APP_RIGHT:
                    res = MMIZDT_OpenType1AccWin();
                    if(res == FALSE)
                    {
                        res = MMIZDT_OpenType1NucleicWin();
                    }
                    if(res)
                    {
                        MMK_CloseWin(win_id);
                    }
                break;
                
		case MSG_APP_LEFT:
                    res = MMIZDT_OpenType1NucleicWin();
                    if(res == FALSE)
                    {
                        res = MMIZDT_OpenType1AccWin();
                    }
                    if(res)
                    {
                        MMK_CloseWin(win_id);
                    }
                break;
                
		//case MSG_APP_OK:
		//case MSG_APP_WEB:
		case MSG_CTL_MIDSK:
			break;
		case MSG_TIMER:
		
		break;
		case MSG_BACKLIGHT_TURN_ON:
		case MSG_BACKLIGHT_TURN_OFF:

		break;
		case MSG_LOSE_FOCUS:
		case MSG_CLOSE_WINDOW:
		break;

		case MSG_KEYUP_CANCEL:
		case MSG_KEYUP_RED:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;


		default:
			recode = MMI_RESULT_FALSE;
			break;

	}
	
	return recode;
}

WINDOW_TABLE( MMIZDT_TYPE1_HEALTH_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_Type1HealthMsg),    
    WIN_ID(MMIZDT_TYPE1_HEALTH_WIN_ID),
    CREATE_ANIM_CTRL (MMIZDT_TYPE1_HEALTH_IMAGE_CTRL_ID, MMIZDT_TYPE1_HEALTH_WIN_ID),
    END_WIN
};

BOOLEAN MMIZDT_OpenType1HealthWin(void)
{
    if(MMIFILE_IsFileExist(ZDT_HTTP_HEALTH_TYPE1_FULLPACH_W, MMIAPICOM_Wstrlen(ZDT_HTTP_HEALTH_TYPE1_FULLPACH_W)))
    {
        MMK_CreateWin((uint32*)MMIZDT_TYPE1_HEALTH_WIN_TAB,PNULL);
        return TRUE;
    }
    return FALSE;
}

BOOLEAN MMIZDT_CloseType1HealthWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TYPE1_HEALTH_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_TYPE1_HEALTH_WIN_ID);
    }
    return TRUE;
}

void MMIZDT_Type1Nucleic_Show(MMI_WIN_ID_T win_id)
{
    GUI_POINT_T   point = {0};
    wchar temp_wstr[HTTP_HEALTH_MSG_MAX_LEN + 1] = {0};
    uint16 unknow_wstr[10] = {0x672A, 0x67E5, 0x8BE2, 0x5230, 0x0};
    uint16 time_wstr[10] = {0x68C0, 0x6D4B, 0x65F6, 0x95F4, 0x003A, 0x0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T	 rect		 = {0}; 
    GUI_RECT_T	 txt_rect		   = {0};
    MMI_STRING_T	cur_str_t	= {0};
    GUISTR_STYLE_T		text_style		= {0};
    GUI_POINT_T 				dis_point = {0,0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    
    rect.left	= 0;
    rect.top	= 0; 
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    txt_rect=rect;

    GUI_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);
    if(1 == yx_health_msg_Rec.nucleic_feminine)
    {
        GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_HEALTH_YIN,&lcd_dev_info);
        txt_rect.left	= 30;
        txt_rect.top	= 160; 
        txt_rect.right	= 210;
        txt_rect.bottom = 180;
        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = SONG_FONT_20;
        text_style.font_color = MMI_BLACK_COLOR;
        state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
        cur_str_t.wstr_ptr = time_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    else if(2 == yx_health_msg_Rec.nucleic_feminine)
    {
        GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_HEALTH_YANG,&lcd_dev_info);
        txt_rect.left	= 30;
        txt_rect.top	= 160; 
        txt_rect.right	= 210;
        txt_rect.bottom = 180;
        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = SONG_FONT_20;
        text_style.font_color = MMI_BLACK_COLOR;
        state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
        cur_str_t.wstr_ptr = time_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    else
    {
        GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_HEALTH_YIN,&lcd_dev_info);
        txt_rect.left	= 60;
        txt_rect.top	= 70; 
        txt_rect.right	= 180;
        txt_rect.bottom = 170;
        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = SONG_FONT_30;
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_BOLD; 
        cur_str_t.wstr_ptr = unknow_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUI_FillRect(&lcd_dev_info,txt_rect,MMI_WHITE_COLOR);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    }
    
    if(strlen(yx_health_msg_Rec.detect_time) > 0)
    {
        txt_rect.left	= 0;
        txt_rect.top	= 180; 
        txt_rect.right	= 239;
        txt_rect.bottom = 205;
        text_style.align = ALIGN_HMIDDLE;
        text_style.font = SONG_FONT_20;
        text_style.font_color = MMI_BLACK_COLOR;
        state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
        GUI_UTF8ToWstr(temp_wstr,HTTP_HEALTH_MSG_MAX_LEN,yx_health_msg_Rec.detect_time,strlen(yx_health_msg_Rec.detect_time));	
        cur_str_t.wstr_ptr = temp_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
            
    }
    return;
}
LOCAL MMI_RESULT_E HandleZDT_Type1NucleicMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E			recode	= MMI_RESULT_TRUE;
    
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
        		break;
		
		case MSG_FULL_PAINT:
                    MMIZDT_Type1Nucleic_Show(win_id);
			break;
			
#if 1//def TOUCH_PANEL_SUPPORT
		case MSG_TP_PRESS_DOWN:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				health_type1_idle_down_x = point.x;
				health_type1_idle_down_y = point.y;
				
			}
			break;
			
		case MSG_TP_PRESS_UP:
			{
				GUI_POINT_T   point = {0};
				
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				
				health_type1_offset_x = point.x - health_type1_idle_down_x;
				health_type1_abs_x = z_abs(health_type1_offset_x);
				
				if( (health_type1_abs_x > 20) && (health_type1_offset_x<0 ) )
				{
                                MMIZDT_OpenType1AccWin();
                                MMK_CloseWin(win_id);
				}
				else if( (health_type1_abs_x > 20) && (health_type1_offset_x>0 ) )
				{
                                MMIZDT_OpenHealth2VMWin();
                                MMK_CloseWin(win_id);
				}
			}
				
			break;
			
		case MSG_TP_PRESS_MOVE:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
		case MSG_TP_PRESS_LONG:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

             case MSG_APP_UP:
		case MSG_APP_RIGHT:
                        MMIZDT_OpenType1AccWin();
                        MMK_CloseWin(win_id);
                break;
                
             case MSG_APP_DOWN:
		case MSG_APP_LEFT:
                    MMIZDT_OpenHealth2VMWin();
                    MMK_CloseWin(win_id);
                break;

		//case MSG_APP_OK:
		//case MSG_APP_WEB:
		case MSG_CTL_MIDSK:
			break;
		case MSG_TIMER:
		
		break;
		case MSG_BACKLIGHT_TURN_ON:
		case MSG_BACKLIGHT_TURN_OFF:

		break;
		case MSG_LOSE_FOCUS:
		case MSG_CLOSE_WINDOW:
		break;

		case MSG_KEYUP_CANCEL:
		case MSG_KEYUP_RED:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;


		default:
			recode = MMI_RESULT_FALSE;
			break;

	}
	
	return recode;
}

WINDOW_TABLE( MMIZDT_TYPE1_NUCLEIC_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_Type1NucleicMsg),    
    WIN_ID(MMIZDT_TYPE1_NUCLEIC_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenType1NucleicWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_TYPE1_NUCLEIC_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseType1NucleicWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TYPE1_NUCLEIC_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_TYPE1_NUCLEIC_WIN_ID);
    }
    return TRUE;
}

//0:未查询到接种信息,1:不适宜接种,2:未完成基础免疫,3:完成基础免疫,4:完成加强免疫
void MMIZDT_Type1Acc_Show(MMI_WIN_ID_T win_id)
{
    GUI_POINT_T   point = {0};
    uint16 unknow_wstr[10] = {0x672A, 0x67E5, 0x8BE2, 0x5230, 0x4FE1, 0x606F, 0x0};
    uint16 acc4_wstr[10] = {0x5B8C, 0x6210, 0x52A0, 0x5F3A, 0x514D, 0x75AB, 0x0};
    uint16 acc3_wstr[10] = {0x5B8C, 0x6210, 0x57FA, 0x7840, 0x514D, 0x75AB, 0x0};
    uint16 acc2_wstr[10] = {0x672A, 0x5B8C, 0x6210, 0x57FA, 0x7840, 0x514D, 0x75AB, 0x0};
    uint16 acc1_wstr[10] = {0x4E0D, 0x9002, 0x5B9C, 0x63A5, 0x79CD, 0x0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T	 rect		 = {0}; 
    GUI_RECT_T	 txt_rect		   = {0};
    MMI_STRING_T	cur_str_t	= {0};
    GUISTR_STYLE_T		text_style		= {0};
    GUI_POINT_T 				dis_point = {0,0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_BOLD; 
    uint16 vacc_status = 0; 
    
    rect.left	= 0;
    rect.top	= 0; 
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    txt_rect=rect;

    if(yx_health_msg_Rec.vacc_status > 0)
    {
        vacc_status = yx_health_msg_Rec.vacc_status -1;
    }
    
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_HEALTH_YGYM,&lcd_dev_info);
    
    txt_rect.left	= 30;
    txt_rect.top	= 70; 
    txt_rect.right	= 210;
    txt_rect.bottom = 170;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_30;
    text_style.font_color = MMI_BLACK_COLOR;
    
    if(0 == vacc_status)
    {
        cur_str_t.wstr_ptr = unknow_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    else if(1 == vacc_status)
    {
        cur_str_t.wstr_ptr = acc1_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    else if(2 == vacc_status)
    {
        text_style.font_color = HEALTH_YELLOW_COLOR;
        cur_str_t.wstr_ptr = acc2_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    else if(3 == vacc_status)
    {
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        cur_str_t.wstr_ptr = acc3_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    else if(4 == vacc_status)
    {
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        cur_str_t.wstr_ptr = acc3_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    else
    {
        cur_str_t.wstr_ptr = unknow_wstr;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&txt_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&txt_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&cur_str_t,
                                            &text_style,
                                            state,
                                            GUISTR_TEXT_DIR_AUTO
											);
    }
    
    return;
}
LOCAL MMI_RESULT_E HandleZDT_Type1AccMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E			recode	= MMI_RESULT_TRUE;
    
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
        		break;
		
		case MSG_FULL_PAINT:
                        MMIZDT_Type1Acc_Show(win_id);
			break;
			
#if 1//def TOUCH_PANEL_SUPPORT
		case MSG_TP_PRESS_DOWN:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				health_type1_idle_down_x = point.x;
				health_type1_idle_down_y = point.y;
				
			}
			break;
			
		case MSG_TP_PRESS_UP:
			{
				GUI_POINT_T   point = {0};
				
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
				
				health_type1_offset_x = point.x - health_type1_idle_down_x;
				health_type1_abs_x = z_abs(health_type1_offset_x);
				
				if( (health_type1_abs_x > 20) && (health_type1_offset_x<0 ) )
				{
				}
				else if( (health_type1_abs_x > 20) && (health_type1_offset_x>0 ) )
				{
                                MMIZDT_OpenType1NucleicWin();
                                MMK_CloseWin(win_id);
				}
			}
				
			break;
			
		case MSG_TP_PRESS_MOVE:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
		case MSG_TP_PRESS_LONG:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
			}
			break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

             case MSG_APP_UP:
		case MSG_APP_RIGHT:
                break;
                
             case MSG_APP_DOWN:
		case MSG_APP_LEFT:
                    MMIZDT_OpenType1NucleicWin();
                    MMK_CloseWin(win_id);
                break;
                
		case MSG_CTL_MIDSK:
                break;
            
		case MSG_TIMER:
                break;
                
		case MSG_BACKLIGHT_TURN_ON:
		case MSG_BACKLIGHT_TURN_OFF:
                break;
                
		case MSG_LOSE_FOCUS:
		case MSG_CLOSE_WINDOW:
                break;

		case MSG_KEYUP_CANCEL:
		case MSG_KEYUP_RED:
		case MSG_CTL_CANCEL:
                    MMK_CloseWin(win_id);
                break;

		default:
			recode = MMI_RESULT_FALSE;
			break;

	}
	
	return recode;
}

WINDOW_TABLE( MMIZDT_TYPE1_ACC_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_Type1AccMsg),    
    WIN_ID(MMIZDT_TYPE1_ACC_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenType1AccWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_TYPE1_ACC_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseType1AccWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TYPE1_ACC_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_TYPE1_ACC_WIN_ID);
    }
    return TRUE;
}
#endif

#if 1
#define HEALTH_UI_TEXT_BG_COLOR	  0x6FFF

BOOLEAN MMIZDT_CloseAllHealthWin(void)
{
    MMIZDT_CloseHealthType1_Win();
    MMIZDT_CloseUrlAccWin();
    MMIZDT_CloseUrlNucleicWin();
    MMIZDT_CloseUrlHealthWin();
    MMIZDT_CloseType1AccWin();
    MMIZDT_CloseType1NucleicWin();
    MMIZDT_CloseType1HealthWin();
    return TRUE;
}

void ZdtWatch_Show_HealthInfo(MMI_WIN_ID_T        win_id)
{

    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    uint8 disp_str_len =0;
    uint16 wstr_result_len = 0;
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 update_wstr[] = {0x66F4, 0x65B0, 0x6570, 0x636E, 0x0}; //健康码下载时间
    uint16 exit_wstr[] = {0x9000, 0x51FA, 0x0}; //退出
    uint16 tittle[] = {0x5065 ,0x5EB7 ,0x7801,0x4E0B ,0x8F7D ,0x65F6,0x95F4,0}; //健康码下载时间
    uint16 cali_tip1[] = {0x6309 ,0x4E0A  ,0x6216 ,0x4E0B ,0x952E ,0x66F4,0x65B0,0x5065,0x5EB7 ,0x7801 ,0}; //按上或下键更新健康码
    MMI_TM_T sys_time = {0};
    uint32   second = yx_health_msg_Rec.rcv_time;

    sys_time = MMIAPICOM_Second2Tm(second);
    
     rect.left	= 0;
     rect.top    = 0; 
     rect.right  = MMI_MAINSCREEN_WIDTH-1;
     rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
     #if 1
     GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
     #else
    GUIRES_DisplayImg(PNULL,
                &rect,
                PNULL,
                win_id,
                IMG_DIGIT_CLOCK0_BG,//menu_ICON
                &lcd_dev_info);	
    #endif
     text_style.align = ALIGN_HVMIDDLE;
     text_style.font = SONG_FONT_22;
     text_style.font_color = MMI_WHITE_COLOR;
     state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
     cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
     cur_str_t.wstr_ptr = tittle;
     cur_rect = rect;
     cur_rect.top    = 10;
     cur_rect.bottom = cur_rect.top + 25;
     GUISTR_DrawTextToLCDInRect( 
                             (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                             (const GUI_RECT_T	  *)&cur_rect,		 
                             (const GUI_RECT_T	  *)&cur_rect,	   
                             (const MMI_STRING_T	  *)&cur_str_t,
                             &text_style,
                             state,
                             GUISTR_TEXT_DIR_AUTO
                             );
     
    memset(disp_str,0,sizeof(disp_str));
    memset(disp_wstr,0,sizeof(disp_wstr));
    cur_str_t.wstr_len=sprintf((char*)disp_str, (char*) "%d/%02d/%02d %02d:%02d",sys_time.tm_year,sys_time.tm_mon,sys_time.tm_mday,sys_time.tm_hour,sys_time.tm_min);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
     cur_rect.top    = cur_rect.bottom+5; 
     cur_rect.bottom = cur_rect.top + 25;
     text_style.align = ALIGN_HVMIDDLE;
     GUISTR_DrawTextToLCDInRect( 
     (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
     (const GUI_RECT_T	  *)&cur_rect,		 
     (const GUI_RECT_T	  *)&cur_rect,	   
     (const MMI_STRING_T	  *)&cur_str_t,
     &text_style,
     state,
     GUISTR_TEXT_DIR_AUTO
     );

     cur_rect.left	= 60;
     cur_rect.top    = 90; 
     cur_rect.right  = 180;
     cur_rect.bottom = 150;
     LCD_FillRoundedRect(&lcd_dev_info, cur_rect, cur_rect,HEALTH_UI_TEXT_BG_COLOR);
     text_style.font = SONG_FONT_30;
     state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_BOLD; 
     cur_str_t.wstr_ptr = update_wstr;
     cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
     GUISTR_DrawTextToLCDInRect( 
                             (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                             (const GUI_RECT_T	  *)&cur_rect,		 
                             (const GUI_RECT_T	  *)&cur_rect,	   
                             (const MMI_STRING_T	  *)&cur_str_t,
                             &text_style,
                             state,
                             GUISTR_TEXT_DIR_AUTO
                             );
	 
     cur_rect.left	= 85;
     cur_rect.top    = 200;
     cur_rect.right  = 155;
     cur_rect.bottom = 236;
     LCD_FillRoundedRect(&lcd_dev_info, cur_rect, cur_rect,HEALTH_UI_TEXT_BG_COLOR);
     state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_BOLD; 
     cur_str_t.wstr_ptr = exit_wstr;
     cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
     GUISTR_DrawTextToLCDInRect( 
                             (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                             (const GUI_RECT_T	  *)&cur_rect,		 
                             (const GUI_RECT_T	  *)&cur_rect,	   
                             (const MMI_STRING_T	  *)&cur_str_t,
                             &text_style,
                             state,
                             GUISTR_TEXT_DIR_AUTO
                             );
    return;
}

LOCAL MMI_RESULT_E  Handle_HealthInfoWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    
    MMI_CTRL_ID_T ctrl_id = MMIZDT_FACTORY_LIST_CTRL_ID;
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
   
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
       	     break;
  
    case MSG_FULL_PAINT:
        	     	ZdtWatch_Show_HealthInfo(win_id);
            break;
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_jp_tp_x = point.x;
			s_jp_tp_y = point.y;
			s_is_jp_tp_down = TRUE;
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(s_is_jp_tp_down)
			{
				offset_y =	point.y - s_jp_tp_y;
				offset_x =	point.x - s_jp_tp_x;
				if(offset_x <= -(20))	
				{
                                MMIZDT_OpenHealthMainWin();
					MMK_CloseWin(win_id);
				}	
				else if(offset_x >= (20))
				{
                                //MMIZDT_OpenHealthInfoWin();
					//MMK_CloseWin(win_id);
				}
                          else if(point.x > 60 && point.x < 180 && point.y > 90 && point.y < 150)
                          {
                                YX_Net_HTTP_Health_Send();
                                MMIZDT_OpenHealthWait_Win();
                                MMK_CloseWin(win_id);
                          }
                          else if(point.x > 85 && point.x < 155 && point.y >= 200 && point.y < 239)
                          {
                                MMK_CloseWin(win_id);
                          }
			}
			s_is_jp_tp_down = FALSE;
		}
		break;
		
	case MSG_TP_PRESS_MOVE:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
	case MSG_TP_PRESS_LONG:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
		}
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

      case MSG_APP_UP:
	case MSG_APP_RIGHT:
                MMIZDT_OpenHealthMainWin();
                MMK_CloseWin(win_id);
            break;
            
     case MSG_APP_DOWN:
	case MSG_APP_LEFT:
            break;
            
	case MSG_APP_OK:
                YX_Net_HTTP_Health_Send();
                MMIZDT_OpenHealthWait_Win();
                MMK_CloseWin(win_id);
            break;
            
    case MSG_CTL_CANCEL:
    case MSG_KEYUP_CANCEL:
	     MMK_CloseWin (win_id);
            break;
			
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
            break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

WINDOW_TABLE( MMIZDT_HEALTH_INFO_WIN_TAB ) = 
{
    WIN_FUNC((uint32) Handle_HealthInfoWinMsg),    
    WIN_ID( MMI_MAINMENU_HEALTH_INFO_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenHealthInfoWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_HEALTH_INFO_WIN_TAB,PNULL);
    return TRUE;
}

#endif
#endif

#endif
