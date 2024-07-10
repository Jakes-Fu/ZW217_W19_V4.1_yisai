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
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef ENG_SUPPORT
#include "mmieng_export.h"
#endif
#include "mmiset_export.h"
#include "mmiset_id.h"

#include "zdt_win_export.h"
#include "mmicc_internal.h"
#include "watch_commonwin_export.h"
#include "watch_common.h"


#define SCALE  1

extern void GT_Qcode_show(const unsigned char *QR_TEST_STR,int x0,int y0,int w0,int h0);

#define z_abs(x)  ((x) >= 0 ? (x) : (-(x)))

LOCAL THEMELIST_ITEM_STYLE_T new_style = {0};

#if 1 //查找手表
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

PUBLIC BOOLEAN MMIZDT_CloseFindWatchWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_FIND_WATCH_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_FIND_WATCH_WIN_ID);
    }
    return TRUE;
}



PUBLIC BOOLEAN MMIZDT_FindWatchWin(void)
{
    if(FALSE == MMK_IsOpenWin(MMIZDT_FIND_WATCH_WIN_ID))
    {
#ifndef WIN32    
#ifdef LEBAO_MUSIC_SUPPORT
        lebao_exit();
#endif
#ifdef XYSDK_SUPPORT
        LIBXMLYAPI_AppExit();
#endif
#endif
#ifdef TULING_AI_SUPPORT
		AI_API_ForceToStopPlay();//MMIAI_CloseChatWin();
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
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
		MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#endif

#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
#include "mmisrvrecord_export.h"
#include "zdt_yx_voice.h"

#define TINY_CHAT_LIST_MAX_SIZE   MAX_YX_VOC_SAVE_SIZE
#define TINYCHAT_RECORD_ANIM_DELAY 500
#define TINYCHAT_MAX_RECORD_TIME_MS 10000
#define TINYCHAT_BOTTOM_HEIGHT  64    //52  //xiongkai 58//120
#define TINYCHAT_LIS_BOX_BOTTOM (MMI_MAINSCREEN_HEIGHT -1- TINYCHAT_BOTTOM_HEIGHT)//(240-83)//180//160

#define  TINYCHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM  4

#define MAX_REMAIN_SIZE  (24*1024) //10K
extern YX_GROUP_INFO_DATA_T m_vchat_all_group_info_arr[YX_DB_FRIEND_MAX_SUM+1];
extern uint16 m_vchat_all_group_info_index[YX_DB_FRIEND_MAX_SUM+1];
extern uint16 m_vchat_all_group_sum;

PUBLIC void MMIZDT_OpenChatGroupWin(void);
LOCAL void MMIZDT_ShowPlayAudioIcon(uint16 list_index, BOOLEAN isPlaying, BOOLEAN isLeft);
LOCAL MMI_RESULT_E  HandleZDT_TinyChatWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );

LOCAL RECORD_SRV_HANDLE tiny_chat_record_handle = PNULL;
static uint8 tiny_chat_record_anim_timer_id = 0;
LOCAL uint32 tiny_chat_record_update_anim_index =0;
LOCAL uint32 tiny_chat_record_timer_index =0;
LOCAL uint8 tiny_chat_is_recording =0;

static uint8 tiny_chat_max_record_timer_id = 0;

LOCAL int16 z_playing_index = -1;
static int s_chat_tp_x;
static int s_chat_tp_y;
static BOOLEAN s_is_chat_tp_down = FALSE;
static BOOLEAN s_is_chat_tp_long = FALSE;
static uint16 s_cur_last_list_index = 0;

MMISRV_HANDLE_T      tinychat_playmedia_handle = 0;

#if 1
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
//        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "Sum: %d",YX_DB_GetVocRcvNum());
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

#if 1 //chat
//yangyu add for find watch window ,begin

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
#define TINYCHAT_BOTTOM_HEIGHT  64    //52  //xiongkai 58//120
#define TINYCHAT_LIS_BOX_BOTTOM (MMI_MAINSCREEN_HEIGHT -1- TINYCHAT_BOTTOM_HEIGHT)//(240-83)//180//160

#define  TINYCHAT_RECORD_ANIMAL_TOTAL_FRAME_NUM  4


LOCAL MMI_RESULT_E HandleVoiceCleanDataWaitWin(
        MMI_WIN_ID_T    win_id, // 窗口的ID
        MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
        DPARAM             param   // 相应消息的参数
        )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;

        case MSG_KEYDOWN_RED:
            break;

        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        
        case MSG_SET_RESET_NEED_WAIT_IND:
               // YX_VocFileRemoveOneItem(s_cur_del_index);
                MMK_CloseWin(win_id);
            break;
            
        case MSG_SET_RESET_FACTORY_OVER_IND:
               // YX_Voice_FileSendAllRemove(&g_yx_app);
                //YX_VocFilePathTxtDel();
                MMK_CloseWin(win_id);
            break;

        case MSG_SET_CLEAN_USER_DATA_OVER_IND:
            break;

        case MSG_CLOSE_WINDOW:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }

    return recode;
}


LOCAL MMI_RESULT_E HandleVoicePopWindMsg(
                                    MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param
                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    uint16 item_id      = 0;   
    MMI_CTRL_ID_T   ctrl_id = MMIZDT_VOICE_POPMENU_CTRL_ID;
    MMI_WIN_ID_T                wait_win_id             =   0;
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};  
    MMI_NOTIFY_T        notify = {0};
    uint32                      time_out                =   0;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIMENU_CreatDynamic(PNULL,win_id, 
            ctrl_id, GUIMENU_STYLE_POPUP);

            MMI_GetLabelTextByLang(TXT_DELETE, &kstring);
            node_item.item_text_ptr = &kstring;
            GUIMENU_InsertNode(0,1,0,&node_item, ctrl_id  );//delete One
            
            MMI_GetLabelTextByLang(TXT_DELALL, &kstring);
            node_item.item_text_ptr = &kstring;    
            GUIMENU_InsertNode(1,2,0,&node_item,  ctrl_id  ); //change by James li
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            break;

        case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_OK:
        case MSG_CTL_MIDSK:   
            item_id = GUIMENU_GetCurNodeId(ctrl_id);         
            switch(item_id)
            {
                case 1:
                    {
                        
                        wait_win_id = MMIZDT_VOICE_WAIT_CTRL_ID;
                        MMIPUB_OpenAlertWinByTextId(&time_out,TXT_PUB_DELETING,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,
                                HandleVoiceCleanDataWaitWin);
                        MMK_CloseWin(win_id);
                        MMK_PostMsg(wait_win_id, MSG_SET_RESET_NEED_WAIT_IND, PNULL,PNULL);
                    }
                    break;
                    
                case 2:
                    {
                        
                        wait_win_id = MMIZDT_VOICE_WAIT_CTRL_ID;
                        MMIPUB_OpenAlertWinByTextId(&time_out,TXT_PUB_DELETING,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                &wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,
                                HandleVoiceCleanDataWaitWin);
                        MMK_CloseWin(win_id);
                        
                       MMK_PostMsg(wait_win_id, MSG_SET_RESET_FACTORY_OVER_IND, PNULL,PNULL);
                        
                    }
                    break;
                    
                default:
                    return recode;
            }       
            
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }    
    return recode;
}

WINDOW_TABLE( MMIZDT_VOICE_POPMENU_WIN_TAB ) = 
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_ID( MMIZDT_VOICE_POPMENU_WIN_ID ),
    WIN_FUNC((uint32)HandleVoicePopWindMsg ),  
    WIN_HIDE_STATUS,
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
     END_WIN
};

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
        m_pCurGroupInfo = &m_vchat_all_group_info_arr[0];
        MMK_CreateWin((uint32*)MMIZDT_TINY_CHAT_WIN_TAB,PNULL);
    }
    return TRUE;
}

BOOLEAN MMIZDT_OpenTinyChatWinByGroupID(uint32 pos_user_data)
{
    if(FALSE == MMK_IsOpenWin(MMIZDT_TINY_CHAT_WIN_ID))
    {
        m_pCurGroupInfo = &m_vchat_all_group_info_arr[pos_user_data];
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
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    MMIZDT_OpenTinyChatWin();
#endif
}


void MMIZDT_TinyShowBottom(MMI_WIN_ID_T  win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    //uint16 tittle[10] = {0x957F, 0x6309, 0x5E95 ,0x90E8, 0x8BB2, 0x8BDD, 0}; //长按此处讲话957F 6309 6B64 5904 8BB2 8BDD 
    uint16 tittle[10] = {0x6309,0x4F4F,0x8BB2,0x8BDD, 0}; ///按住讲话
    GUI_FONT_T f_big = SONG_FONT_26;//SONG_FONT_22;//SONG_FONT_19;//SONG_FONT_34;
    GUI_POINT_T      point = {0};
    GUI_RECT_T   record_text_rect = WECHART_RECORD_TEXT_RECT;
    
    rect.left = 0;
    rect.top= TINYCHAT_LIS_BOX_BOTTOM;
    rect.right= (MMI_MAINSCREEN_WIDTH -1);
    rect.bottom = (MMI_MAINSCREEN_HEIGHT-1); 
  
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    point.x = WECHART_RECORD_IMG_X;
    point.y = WECHART_RECORD_IMG_Y;

    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, IMAGE_CHAT_NO_RECORD_BG, &lcd_dev_info); 

    text_style.align = ALIGN_LVMIDDLE; // ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&record_text_rect,       
        (const GUI_RECT_T      *)&record_text_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
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
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big = SONG_FONT_30;//SONG_FONT_28;//SONG_FONT_34;
    uint16 tittle[10] = {0x6b63, 0x5728, 0x5f55, 0x97f3,0}; //正在录音
    GUI_RECT_T   record_text_rect = WECHART_RECORD_TEXT_RECT;
    GUI_RECT_T   countdown_rect = WECHART_RECORD_COUNTDOWN_TEXT_RECT;

    rect.left = 0;
    rect.top= TINYCHAT_LIS_BOX_BOTTOM;
    rect.right= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1; 
    
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    point.x = WECHART_RECORD_IMG_X;
    point.y = WECHART_RECORD_IMG_Y;

    GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_CHAT_RECORDING_BG, 
                &lcd_dev_info);
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%d",((TINYCHAT_MAX_RECORD_TIME_MS/1000)-(tiny_chat_record_timer_index/2)));
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&countdown_rect,       
        (const GUI_RECT_T      *)&countdown_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = SONG_FONT_26;//SONG_FONT_22;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&record_text_rect,       
        (const GUI_RECT_T      *)&record_text_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );    
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
                        MMIZDT_ShowPlayAudioIcon(z_playing_index , FALSE, YX_VOC_IsRcvFile(m_pCurGroupInfo->file_arr[z_playing_index].fullname));   //BUG 停止播放后需要恢复头像
                    }
					#ifndef WIN32
                    __es83xx_cfg_spk_out_mod(0);
					#endif
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
        item_data_ptr->item_content[2].item_data.image_id = isLeft ? IMAGE_TINYCHAT_AUDIO_LEFT:IMAGE_TINYCHAT_AUDIO_RIGHT;//IMAGE_TINYCHAT_PLAYING_AUDIO;
    }
    else
    {
        z_playing_index = -1;
        item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;      
        item_data_ptr->item_content[2].item_data.image_id = isLeft ? IMAGE_TINYCHAT_HEAD_SERVER : IMAGE_TINYCHAT_HEAD;//IMAGE_NULL;

    }
        
        item_data_ptr->item_content[3].item_data_type = GUIITEM_DATA_NONE;      //BUG 播放后需要立即删除未读红点标志
        item_data_ptr->item_content[3].item_data.image_id = PNULL;
    //MMK_UpdateScreen();//useless
    MMK_SendMsg(MMK_ConvertIdToHandle(MMIZDT_TINY_CHAT_LIST_CTRL_ID), MSG_CTL_PAINT, PNULL);//ok ,list will update view
    

}


LOCAL int MMIZDT_TinyChatStartPlayAudio(uint16 index, uint16 list_index)
{
    MMISRVAUD_TYPE_U    audio_data  = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;
    uint16      full_path[MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0};
    uint16      full_path_len = 0;
    int Ret = 0;
    char file_name[MAX_YX_VOC_GROUP_FULL_PATH_SIZE+1] = {0};
    BOOLEAN isLeft = FALSE;

    YX_APP_T * pMe = &g_yx_app;
    SCI_MEMCPY(file_name, m_pCurGroupInfo->file_arr[index].fullname, MAX_YX_VOC_GROUP_FULL_PATH_SIZE);
    if(m_pCurGroupInfo->status_arr[index] != 0x31) //0x31 49 ascii 字符1 啥玩意
    {
        m_pCurGroupInfo->status_arr[index] = 0x31; //已读
    }
    ZDT_LOG("MMIZDT_TinyChatStartPlayAudio 1 index=%d, file_name=%s",index, file_name);
    if(file_name == PNULL || !ZDT_File_Exsit(m_pCurGroupInfo->file_arr[index].fullname))
    {
        return 1;
    }
    isLeft = YX_VOC_IsRcvFile(m_pCurGroupInfo->file_arr[index].fullname);

    YX_Voice_HandleStop(pMe); //stop rcv play and stop record
    
    if(tinychat_playmedia_handle)
    {   
        if(z_playing_index >= 0)
        {
            MMIZDT_ShowPlayAudioIcon(z_playing_index , FALSE, YX_VOC_IsRcvFile(m_pCurGroupInfo->file_arr[z_playing_index].fullname));   //BUG 停止播放后需要恢复头像
        }
        MMISRVAUD_Stop(tinychat_playmedia_handle);
        MMISRVMGR_Free(tinychat_playmedia_handle);
        tinychat_playmedia_handle = NULL;
		#ifndef WIN32
        __es83xx_cfg_spk_out_mod(0);
		#endif
    }
    
    full_path_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = full_path;
    audio_data.ring_file.name_len = full_path_len;
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
	#ifndef WIN32
    __es83xx_cfg_spk_out_mod(1);
	#endif
    if(MMIZDT_TinyChatAudioPlayRequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, MMIZDT_TinyChatAudioPlayNotify, MMISRVAUD_VOLUME_LEVEL_MAX))
    {
        tinychat_playmedia_handle = audio_handle;
        if(!MMISRVAUD_Play(audio_handle, 0))
        {     
            ZDT_LOG("MMIZDT_TinyChatStartPlayAudio play failed");
            MMISRVMGR_Free(tinychat_playmedia_handle);
            tinychat_playmedia_handle = 0;       
            Ret = 2;
			#ifndef WIN32
            __es83xx_cfg_spk_out_mod(0);
			#endif
        }
        else
        {
            MMIZDT_ShowPlayAudioIcon(list_index, TRUE, isLeft);
        }
    }
    else
    {        
		#ifndef WIN32
        __es83xx_cfg_spk_out_mod(0);
		#endif
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
            MMIZDT_ShowPlayAudioIcon(z_playing_index , FALSE, YX_VOC_IsRcvFile(m_pCurGroupInfo->file_arr[z_playing_index].fullname));   //BUG 停止播放后需要恢复头像
        }
        MMISRVAUD_Stop(tinychat_playmedia_handle);
        MMISRVMGR_Free(tinychat_playmedia_handle);
        tinychat_playmedia_handle = NULL;
		#ifndef WIN32
        __es83xx_cfg_spk_out_mod(0);
		#endif
    }
}



PUBLIC void MMIZDT_TinyChatRefreshWin()
{
    ZDT_LOG("MMIZDT_TinyChatRefreshWin ");
    if(!MMK_IsFocusWin(MMIZDT_TINY_CHAT_WIN_ID))
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
       sd_type = MMI_DEVICE_UDISK;
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
    BOOLEAN ret = FALSE;
    uint16 i = 0, j=0;
    uint16 position = 0;
    uint16 line_num = m_pCurGroupInfo->file_num;
    
    GUILIST_RemoveAllItems(MMIZDT_TINY_CHAT_LIST_CTRL_ID);
	
	line_num = line_num>= TINY_CHAT_LIST_MAX_SIZE ? TINY_CHAT_LIST_MAX_SIZE:line_num ;

    for(i = 0 ; i < line_num ; i++)
    {        
        GUILIST_ITEM_T               item_t    =  {0};
        GUILIST_ITEM_DATA_T          item_data = {0};
        wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
        uint16 total_item_num = 0;
        BOOLEAN isLeft = FALSE;
        wchar   full_path[MAX_YX_VOC_GROUP_FULL_PATH_SIZE] = {0}; 
        uint16  full_path_len = 0;

        MMISRVAUD_CONTENT_INFO_T  mp3_file_info = {0};
        wchar                         temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
        wchar                       time_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
        if(!ZDT_File_Exsit(m_pCurGroupInfo->file_arr[i].fullname))
        {
            continue;
        }
        
        isLeft = YX_VOC_IsRcvFile(m_pCurGroupInfo->file_arr[i].fullname);
        ZDT_LOG("MMIZDT_TinyChatUpdateList i=%d isLeft=%d, %s" , i, isLeft , m_pCurGroupInfo->file_arr[i].fullname);
        item_t.item_data_ptr = &item_data; 

        if(isLeft)
        {
            item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS;
            item_data.item_content[1].is_custom_font_color = TRUE;
            item_data.item_content[1].custom_font_color = 0X043F;        
            item_data.item_content[1].custom_font_color_focus = 0X043F;                //左边蓝色字
        
        }
        else
        {
            item_t.item_style =  GUIITEM_STYLE_ONE_LINE_BGICON_R_TEXT_MS;
        }
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = isLeft ? IMAGE_TINYCHAT_VOICE_LEFT_BG : IMAGE_TINYCHAT_VOICE_RIGHT_BG;
        full_path_len = GUI_GBToWstr(full_path, (const uint8*)m_pCurGroupInfo->file_arr[i].fullname, SCI_STRLEN(m_pCurGroupInfo->file_arr[i].fullname));


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

        item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[2].item_data.image_id = isLeft ? IMAGE_TINYCHAT_HEAD_SERVER : IMAGE_TINYCHAT_HEAD;   //xiongkai   微聊头像左侧和右侧用不同的    IMAGE_TINYCHAT_HEAD;//isLeft ? IMAGE_TINYCHAT_AUDIO_LEFT : IMAGE_TINYCHAT_AUDIO_RIGHT;
        if(isLeft)
        {

            if(m_pCurGroupInfo->status_arr[i] != 0x31)      //已知BUG LIST设置最大项后data不对应
            {
                item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;       //未读
                item_data.item_content[3].item_data.image_id = IMAGE_TINYCHAT_UNREAD;
            }
            else
            {
                item_data.item_content[3].item_data_type = GUIITEM_DATA_NONE;
                item_data.item_content[3].item_data.image_id = PNULL;
            }
                   
        }
        ret = GUILIST_AppendItem (MMIZDT_TINY_CHAT_LIST_CTRL_ID, &item_t);
        ZDT_LOG("MMIZDT_TinyChatUpdateList ret = %d", ret);
        if(ret)
        {
            CTRLLIST_SetItemUserData(MMIZDT_TINY_CHAT_LIST_CTRL_ID, position, &i);
            position++;
        }
    }
    CTRLLIST_SetSelectedItem(MMIZDT_TINY_CHAT_LIST_CTRL_ID, position-1, TRUE);
    GUILIST_SetCurItemIndex(MMIZDT_TINY_CHAT_LIST_CTRL_ID, position-1);
}


LOCAL MMI_RESULT_E TinyChat_handleConfirmationDelete(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        list_win_id = MMIZDT_TINY_CHAT_WIN_ID;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:

            break;
        case MSG_APP_WEB: 
            MMK_SendMsg(list_win_id, MSG_PROMPTWIN_OK, PNULL);
            break;
        case MSG_APP_OK:
            {
                 uint32 list_item_index = GUILIST_GetCurItemIndex(MMIZDT_TINY_CHAT_LIST_CTRL_ID);
                 ZDT_LOG("TinyChat_handleConfirmationDelete MSG_APP_OK = 0x%x list_item_index:%d", msg_id, list_item_index);
                 YX_VocFileRemoveOneItem(list_item_index);
            }
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            //result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
        default:
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }

    return (result);
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
        list_init.both_rect.h_rect.bottom =(TINYCHAT_LIS_BOX_BOTTOM -1); //240;
        
        list_init.type = GUILIST_TEXTLIST_E;
                    
        GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);            
        MMK_SetAtvCtrl(win_id,ctrl_id);
        //不需要分割线
        GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
        //不画高亮条
        GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );

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
                MMIZDT_TinyChatStopPlayAudio();
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

    case MSG_NOTIFY_LIST_LONGOK:
        {
            uint32 list_item_index = GUILIST_GetCurItemIndex(ctrl_id);
            ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_NOTIFY_LIST_LONGOK list_item_index:%d", list_item_index);
            MMIZDT_TinyChatStopPlayAudio();
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(), PNULL, TXT_COMMON_CONFIRM_DELETE,TXT_NULL,NULL,NULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,TinyChat_handleConfirmationDelete, list_item_index);
        }
        break;
    case MSG_PROMPTWIN_OK:
        ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_PROMPTWIN_OK");
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
                        MMIZDT_Net_Reset(TRUE);
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
        
    case MSG_KEYLONG_OK:
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
        
        if(ZDT_File_MemFull(FALSE))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_TF_CARD_FULL,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
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
    
    case MSG_KEYPRESSUP_OK:
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

         #if 1 //xiongkai      for bug 设备端微聊界面发送10s语音，未及时显示语音条记录；需要退出重进刷新；    
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
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:         
    case MSG_APP_MENU:
        //if(real_lines > 0 && tiny_chat_is_recording == 0 && tinychat_playmedia_handle == 0)
        //{
        //    index = GUILIST_GetCurItemIndex(ctrl_id);
        //    OpenVoiceMenuWin(index);
        //}
        break;
        
    case MSG_KEYUP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_APP_CANCEL");
        //MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        
        MMK_CloseWin(win_id);
        break;
        //yangyu add
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
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
        //ZdtTalk_BackLight(FALSE);     //BUG 微聊界面来视频时会关闭微聊,导致视频通话时会自动灭屏
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
#endif


PUBLIC void ZDT_Delete_CheckClose_Tiny_Chat_Win(uint8 * friend_id)
{
    if(MMK_IsFocusWin(MMIZDT_TINY_CHAT_WIN_ID))
    {
        if(strcmp(friend_id,m_pCurGroupInfo->group_id) == 0)
        {
            MMIZDT_TinyChatStopPlayAudio();
            if(tiny_chat_is_recording)
            {
                 MMIZDT_StopRecordAnimTimer();
                 MMIZDT_ClearTinyChatRecordAnim(MMIZDT_TINY_CHAT_WIN_ID);
                 MMK_UpdateScreen();
                 MMIZDT_StopRecordMaxTimeTimer();
                 YX_API_Record_Stop();
                 tiny_chat_is_recording = 0;	
            }
            MMK_CloseWin(MMIZDT_TINY_CHAT_WIN_ID);
        }
    }
}

PUBLIC void MMIZDT_OpenChatWinByRcv(void)
{
    m_pCurGroupInfo = m_pCurRcvGroupInfo;
    MMK_CreateWin((uint32*)MMIZDT_TINY_CHAT_WIN_TAB,PNULL);
   return;
}


#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
LOCAL uint8 s_del_friend_win_wait_status = 0;
LOCAL uint8 s_del_friend_ui_timer_id = 0;
LOCAL uint16 s_cur_gproup_select_index = 0;

#if 1
static uint16 s_cur_gproup_long_key_handle = 0;

void MMIZDT_VChatGroup_HandleOpenTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
   if(MMK_IsOpenWin(MMIZDT_FRIEND_WAIT_WIN_ID))
   {
        s_del_friend_win_wait_status = 2;
        MMK_PostMsg(MMIZDT_FRIEND_WAIT_WIN_ID, MSG_FULL_PAINT, PNULL,PNULL);
   }
}

void MMIZDT_VChatGroup_HandleCloseTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(MMK_IsOpenWin(MMIZDT_FRIEND_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_FRIEND_WAIT_WIN_ID);
    }
}

PUBLIC void MMIZDT_VChatGroup_TimerStop()
{
    if(s_del_friend_ui_timer_id != 0)
    {
        MMK_StopTimer(s_del_friend_ui_timer_id);
    }
}

PUBLIC void MMIZDT_VChatGroup_TimerStart(uint32 time_out, MMI_TIMER_FUNC func)
{
    if(s_del_friend_ui_timer_id != 0)
    {
        MMK_StopTimer(s_del_friend_ui_timer_id);
    }
    s_del_friend_ui_timer_id = MMK_CreateTimerCallback(time_out, func, 0, FALSE);
}

void MMIZDT_ChatGroup_Win_Wating(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T	cur_str_t	= {0};
    GUI_RECT_T	 cur_rect		   = {0};
    GUI_RECT_T	 rect		 = {0}; 
    GUISTR_STYLE_T		text_style		= {0};
    GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_30;
    GUI_RECT_T	 title_rect		   = {0};
    uint16 wstr_txt[GUILIST_STRING_MAX_NUM+1] = {0};
    uint16 wstr_waiting[10] = {0x597D,0x53CB,0x5220,0x9664,0x4E2D,0x002E,0x002E,0x002E,0x0 }; //好友删除中...
    uint16 wstr_ok[10] = {0x5220,0x9664,0x6210,0x529F,0x0021,0x0 }; //删除成功
    uint16 wstr_fail[10] = {0x5220,0x9664,0x5931,0x8D25,0x0021,0x0 }; //删除失败!
    
    rect.left	= 0;
    rect.top	= 0;
    rect.right	= MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    cur_rect = rect;
    title_rect = rect;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    if(s_del_friend_win_wait_status == 0)
    {
        title_rect.top = 20;
        title_rect.bottom = 50;
        cur_rect.top = 60;
        cur_rect.bottom = 180;
        if(SCI_STRLEN(m_pCurGroupInfo->group_name) > 0)
        {
            ZDT_UCS_Str16_to_uint16((uint8*)m_pCurGroupInfo->group_name, SCI_STRLEN(m_pCurGroupInfo->group_name) ,wstr_txt, GUILIST_STRING_MAX_NUM);
            cur_str_t.wstr_ptr = wstr_txt;
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
            GUISTR_DrawTextToLCDInRect( (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                        (const GUI_RECT_T      *)&title_rect,       
                                        (const GUI_RECT_T      *)&title_rect,     
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                        ); 
       }
       else if(SCI_STRLEN(m_pCurGroupInfo->group_num) > 0)
       {
            GUI_UTF8ToWstr(wstr_txt, GUILIST_STRING_MAX_NUM, m_pCurGroupInfo->group_num, SCI_STRLEN(m_pCurGroupInfo->group_num));
            cur_str_t.wstr_ptr = wstr_txt;
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
            GUISTR_DrawTextToLCDInRect( (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                        (const GUI_RECT_T      *)&title_rect,       
                                        (const GUI_RECT_T      *)&title_rect,     
                                        (const MMI_STRING_T    *)&cur_str_t,
                                        &text_style,
                                        state,
                                        GUISTR_TEXT_DIR_AUTO
                                        ); 
       }
        cur_rect = rect;
        cur_str_t.wstr_ptr = wstr_waiting;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                    (const GUI_RECT_T      *)&cur_rect,       
                                    (const GUI_RECT_T      *)&cur_rect,     
                                    (const MMI_STRING_T    *)&cur_str_t,
                                    &text_style,
                                    state,
                                    GUISTR_TEXT_DIR_AUTO
                                    ); 
    }
    else if(s_del_friend_win_wait_status == 1)
    {
        cur_str_t.wstr_ptr = wstr_ok;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                    (const GUI_RECT_T      *)&cur_rect,       
                                    (const GUI_RECT_T      *)&cur_rect,     
                                    (const MMI_STRING_T    *)&cur_str_t,
                                    &text_style,
                                    state,
                                    GUISTR_TEXT_DIR_AUTO
                                    ); 
    }
    else if(s_del_friend_win_wait_status == 2)
    {
        cur_str_t.wstr_ptr = wstr_fail;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
        GUISTR_DrawTextToLCDInRect( (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
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

LOCAL MMI_RESULT_E  HandleZDT_ChatGroupWaitWinMsg(
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
            ZdtTalk_BackLight(TRUE);
            s_del_friend_win_wait_status = 0;
            GUI_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);	
            MMIZDT_VChatGroup_TimerStart(20000,MMIZDT_VChatGroup_HandleOpenTimer);
            break;
            
        case MSG_FULL_PAINT:
            MMIZDT_ChatGroup_Win_Wating(win_id);
            if(s_del_friend_win_wait_status != 0)
            {
                MMIZDT_VChatGroup_TimerStart(2000,MMIZDT_VChatGroup_HandleCloseTimer);
            }
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
            #ifdef WIN32
            if((point.x > 0 && point.x <50)&&(point.y >= 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
            {
                YX_Net_Receive_PPR(&g_yx_app,m_pCurGroupInfo->group_id,SCI_STRLEN(m_pCurGroupInfo->group_id));
            }
            #endif
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
            break;
                
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
	        break;

        case MSG_CLOSE_WINDOW:
            ZdtTalk_BackLight(FALSE);
	        break;
			
        default:
	        recode = MMI_RESULT_FALSE;
	        break;
	}
	return recode;

}

WINDOW_TABLE( MMIZDT_FRIEND_WAIT_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_ChatGroupWaitWinMsg),    
    WIN_ID(MMIZDT_FRIEND_WAIT_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenChatGroupWait_Win(void)
{
    MMK_CreateWin((uint32*)MMIZDT_FRIEND_WAIT_WIN_TAB,PNULL);
    return TRUE;
}
#endif

LOCAL MMI_RESULT_E  HandleZDT_ChatGroupWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );


WINDOW_TABLE( MMIZDT_VCHAT_GROUP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_ChatGroupWinMsg),    
    WIN_ID( MMIZDT_VCHAT_GROUP_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZDT_OpenChatGroupWin(void)
{
    if(ZDT_SIM_Exsit() == FALSE)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,STR_SIM_NOT_SIM_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);//TXT_SIM_REJECTED
        return;
    }
#if 0
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    if(yx_DB_Set_Rec.net_open == 0)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_OPEN,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif 
   if(FALSE == MMK_IsOpenWin(MMIZDT_VCHAT_GROUP_WIN_ID))
   {
        MMK_CreateWin((uint32*)MMIZDT_VCHAT_GROUP_WIN_TAB,PNULL);
   }
   else
   {
        MMK_PostMsg(MMIZDT_VCHAT_GROUP_WIN_ID, MSG_FULL_PAINT, PNULL,PNULL);
   }
   return;
}

PUBLIC void MMIZDT_OpenChatContactListWin(void)
{
    if(FALSE == MMK_IsOpenWin(MMIZDT_VCHAT_GROUP_WIN_ID))
   {
        MMK_CreateWin((uint32*)MMIZDT_VCHAT_GROUP_WIN_TAB,PNULL);
   }
   else
   {
        MMK_PostMsg(MMIZDT_VCHAT_GROUP_WIN_ID, MSG_FULL_PAINT, PNULL,PNULL);
   }
}

PUBLIC void MMIZDT_OpenChatGroupWinByPP(uint8 * friend_id)
{
   if(FALSE == MMK_IsOpenWin(MMIZDT_VCHAT_GROUP_WIN_ID))
   {
        MMK_CreateWin((uint32*)MMIZDT_VCHAT_GROUP_WIN_TAB,(ADD_DATA)friend_id);
   }
   return;
}

PUBLIC void MMIZDT_CloseChatGroupWin()
{
    if(MMK_IsOpenWin(MMIZDT_VCHAT_GROUP_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_VCHAT_GROUP_WIN_ID);
    }
}

PUBLIC void MMIZDT_UpdateChatGroupWin(BOOLEAN is_ok)
{
    ZDT_LOG("MMIZDT_UpdateChatGroupWin is_ok = %d",is_ok);
   if(MMK_IsFocusWin(MMIZDT_VCHAT_GROUP_WIN_ID))
   {
        MMK_PostMsg(MMIZDT_VCHAT_GROUP_WIN_ID, MSG_FULL_PAINT, PNULL,PNULL);
   }
   if(MMK_IsFocusWin(MMIZDT_FRIEND_WAIT_WIN_ID))
   {
        if(is_ok)
        {
            s_del_friend_win_wait_status = 1;
        }
        else
        {
            s_del_friend_win_wait_status = 2;
        }
        MMK_PostMsg(MMIZDT_FRIEND_WAIT_WIN_ID, MSG_FULL_PAINT, PNULL,PNULL);
   }
}

LOCAL MMI_RESULT_E MMIZDT_ChatGroup_QueryDelete(
                                                MMI_WIN_ID_T win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_KEYDOWN_RED:
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
            {
                #ifdef WIN32
                    MMIZDT_OpenChatGroupWait_Win();
                    MMK_CloseWin(win_id);
               #else
                if(YX_Net_Send_PPR(&g_yx_app,m_pCurGroupInfo->group_id))
                {
                    MMIZDT_OpenChatGroupWait_Win();
                    MMK_CloseWin(win_id);
                }
                #endif
            }
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }
    return result;
}

void  MMIZDT_ChatGroup_ShowList(MMI_WIN_ID_T win_id,uint8 * p_friend_id)
{
    uint16 i;
    uint32 user_data = 0;
    uint8				copy_len = 0;
    uint16 max_list_num = YX_DB_FRIEND_MAX_SUM + 1;
    MMI_CTRL_ID_T            ctrl_id =  MMIZDT_YX_CHAT_CTRL_ID;
    GUILIST_ITEM_T      first_item_t = {0};
    //GUILIST_ITEM_DATA_T first_item_data = {0};
    //uint16 first_wstr[5] = {0x5BB6, 0x5EAD,0x7FA4, 0x804A, 0x0};//家庭群聊
    uint32 unread_num = 0;
    GUILIST_RemoveAllItems(ctrl_id);
    
    GUILIST_SetMaxItem(ctrl_id, max_list_num,FALSE);

    for ( i = 0; i < m_vchat_all_group_sum; i++ )
    {
        GUILIST_ITEM_T      item_t = {0};
        GUILIST_ITEM_DATA_T item_data = {0};
        wchar name_buff[GUILIST_STRING_MAX_NUM + 1] = {0};
        wchar number_buff[GUILIST_STRING_MAX_NUM + 1] = {0};
        YX_GROUP_INFO_DATA_T * pGroupInfo = &m_vchat_all_group_info_arr[i];
        unread_num = YX_VCHAT_GetGroupUnread(pGroupInfo);
    #ifdef ZTE_WATCH
        item_t.item_style    = GUIITEM_STYLE_1ICON_1STR_1LINE_WITH_UNREAD_NUM;
    #else
        item_t.item_style    = GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1 ;//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM;
    #endif
        item_t.item_data_ptr = &item_data;
        
        if(SCI_STRLEN(pGroupInfo->group_name) > 0)
        {
            ZDT_UCS_Str16_to_uint16((uint8*)pGroupInfo->group_name, SCI_STRLEN(pGroupInfo->group_name) ,name_buff, GUILIST_STRING_MAX_NUM);
            item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_buff;
            item_data.item_content[1].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, MMIAPICOM_Wstrlen(item_data.item_content[1].item_data.text_buffer.wstr_ptr));
        }

        item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMAGE_TINYCHAT_HEAD; //    WATCHCOM_GetAvaterImageId(name_buff);

        if(unread_num > 0)
        {
        #if 0 //def ZTE_WATCH
            char num[4] = {0};
            if(unread_num > 9)
            {
                sprintf(num,"%s", "...");
            }
            else
            {
                sprintf(num,"%d", unread_num);
            }           
            MMIAPICOM_StrToWstr(num,number_buff);
            item_data.item_content[3].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[3].item_data.text_buffer.wstr_ptr = number_buff;
            item_data.item_content[3].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[3].item_data.text_buffer.wstr_ptr);
        #endif
            item_data.item_content[2].item_data_type     = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[2].item_data.image_id = IMAGE_TINYCHAT_UNREAD;
        }
     #if 0 //ndef ZTE_WATCH
        copy_len = SCI_STRLEN(pGroupInfo->group_num);
        if(copy_len > 0)
        {
            GUI_UTF8ToWstr(number_buff, GUILIST_STRING_MAX_NUM, pGroupInfo->group_num, copy_len);
        }
        else
        {
            //GUI_UTF8ToWstr(number_buff, GUILIST_STRING_MAX_NUM, pGroupInfo->group_id, SCI_STRLEN(pGroupInfo->group_id));
        }
        item_data.item_content[3].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = number_buff;
        item_data.item_content[3].item_data.text_buffer.wstr_len = MIN( GUILIST_STRING_MAX_NUM, MMIAPICOM_Wstrlen(item_data.item_content[3].item_data.text_buffer.wstr_ptr));
    #endif   
        GUILIST_AppendItem(ctrl_id, &item_t);
        user_data = i;
        if(p_friend_id != PNULL && SCI_STRLEN(p_friend_id) > 0 && strcmp(p_friend_id,pGroupInfo->group_id) == 0)
        {
            s_cur_gproup_select_index = i;
        }
        CTRLLIST_SetItemUserData(ctrl_id, i, &user_data);
    }
    return;
}

LOCAL void Create_Chat_Contact_List_Listbox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUI_RECT_T rect)
{
    GUILIST_INIT_DATA_T list_init = {0};           
    list_init.type = GUILIST_TEXTLIST_E;
    GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
    //不画高亮条
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );
    GUILIST_SetRect(ctrl_id, &rect);
    MMK_SetAtvCtrl(win_id,ctrl_id);
}

LOCAL void Chat_Contact_List_Draw_Title(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUI_RECT_T          text_rect={42,0,239,MMI_SPECIAL_TITLE_HEIGHT}; 
    GUI_RECT_T content_rect = {0, MMI_SPECIAL_TITLE_HEIGHT, MMI_MAINSCREEN_WIDTH - 1 , MMI_MAINSCREEN_HEIGHT - 1}; 
    GUISTR_STATE_T		state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_POINT_T point = {0};
    GUI_RECT_T  rect = MMITHEME_GetFullScreenRectEx(win_id);
    MMI_STRING_T        string = {0};
    //draw TITLE bg 	
    GUIRES_DisplayImg(PNULL,&rect,PNULL,win_id,IMAGE_THEME_BLACK_BG_SPEC, &lcd_dev_info); 

    ///////draw title
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = MMI_DEFAULT_BIG_FONT;
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.char_space = 0;
        	
    MMI_GetLabelTextByLang(TXT_BH_NET_CHAT, &string);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T	   *)&text_rect,	   //the fixed display area
        (const GUI_RECT_T	   *)&text_rect,	   //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

LOCAL void Chat_Contact_List_Title_Back_Click(MMI_WIN_ID_T win_id, DPARAM param)
{
    GUI_POINT_T   point = {0};
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if((point.x > 0 && point.x <50)&&(point.y >= 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
    {
        MMK_CloseWin(win_id);
    }
    if((point.x > 190 && point.x < 240)&&(point.y >= 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
    {
        #if 0
        if(strcmp(m_pCurGroupInfo->group_id,YX_VCHAT_DEFAULT_GROUP_ID) != 0)
        {
            MMI_WIN_ID_T	query_win_id = MMIZDT_QUERY_WIN_ID;
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,MMIZDT_ChatGroup_QueryDelete);                
        }
        #endif
    }
}

LOCAL void Chat_Contact_List_Item_LongClick(MMI_CTRL_ID_T ctrl_id)
{
    if(strcmp(m_pCurGroupInfo->group_id,YX_VCHAT_DEFAULT_GROUP_ID) != 0)
    {
        uint32 pos_user_data = 0;
        uint16 current_index;
        MMI_WIN_ID_T	query_win_id = MMIZDT_QUERY_WIN_ID;
        s_cur_gproup_long_key_handle = 1;
        current_index = GUILIST_GetCurItemIndex(ctrl_id );
        s_cur_gproup_select_index = current_index;
        GUILIST_GetItemData(ctrl_id, current_index, &pos_user_data);
        m_pCurGroupInfo = &m_vchat_all_group_info_arr[pos_user_data];
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,MMIZDT_ChatGroup_QueryDelete);                
    }
}

LOCAL void Chat_Contact_List_Item_Click(MMI_CTRL_ID_T ctrl_id)
{
    uint32 pos_user_data = 0;
    uint16 current_index;
    if(s_cur_gproup_long_key_handle == 0)
    {
        current_index = GUILIST_GetCurItemIndex(ctrl_id );
        s_cur_gproup_select_index = current_index;
        GUILIST_GetItemData(ctrl_id, current_index, &pos_user_data);
        MMIZDT_OpenTinyChatWinByGroupID(pos_user_data);
    }
}

LOCAL MMI_RESULT_E  HandleZDT_ChatGroupWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_CTRL_ID_T            ctrl_id = MMIZDT_YX_CHAT_CTRL_ID;
    MMI_RESULT_E             recode  = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};	    
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);
    uint8 * p_friend_id = PNULL;
    p_friend_id = (uint8 *) MMK_GetWinAddDataPtr(win_id);

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            {
                GUI_RECT_T list_rect = {0, MMI_SPECIAL_TITLE_HEIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
                Create_Chat_Contact_List_Listbox(win_id, ctrl_id,list_rect);
                GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
                s_cur_gproup_select_index = 0;
                s_cur_gproup_long_key_handle = 0;
                if(m_vchat_all_group_sum == 0)
                {
                    YX_VCHAT_GetAllGroupInfo();
                }
            }
            break;
        
        case MSG_GET_FOCUS:
                s_cur_gproup_long_key_handle = 0;
            break;
                
        case MSG_FULL_PAINT:
            {
                Chat_Contact_List_Draw_Title(win_id);
                MMIZDT_ChatGroup_ShowList(win_id,p_friend_id);
               // CTRLLIST_SetSelectedItem(ctrl_id, s_cur_gproup_select_index, TRUE);
               // GUILIST_SetCurItemIndex(ctrl_id, s_cur_gproup_select_index); 
            }
            break;
            
        case MSG_TP_PRESS_UP:
            Chat_Contact_List_Title_Back_Click(win_id,param);
            break;
        
        case MSG_CTL_LIST_LONGOK:
            Chat_Contact_List_Item_LongClick(ctrl_id);
            break;
        
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
            Chat_Contact_List_Item_Click(ctrl_id);
            break;
             
        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
        case MSG_KEYDOWN_RED:
            break;

        case MSG_KEYUP_RED:
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

PUBLIC MMI_RESULT_E HandleLauncher_ChatContactListWinMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_CTRL_ID_T            ctrl_id = MMIZDT_YX_CHAT_CTRL_ID;
    MMI_RESULT_E             recode  = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};	    
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);
    uint8 * p_friend_id = PNULL;
    p_friend_id = (uint8 *) MMK_GetWinAddDataPtr(win_id);

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            {
                GUI_RECT_T list_rect = LAUNCHER_WECHAT_CONTACT_LIST_RECT;
                Create_Chat_Contact_List_Listbox(win_id, ctrl_id,list_rect);
                s_cur_gproup_select_index = 0;
                s_cur_gproup_long_key_handle = 0;
                if(m_vchat_all_group_sum == 0)
                {
                    YX_VCHAT_GetAllGroupInfo();
                }
            }
            break;
        
        case MSG_GET_FOCUS:
                s_cur_gproup_long_key_handle = 0;
            break;
                
        case MSG_FULL_PAINT:
            {
                GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
                MMIZDT_ChatGroup_ShowList(win_id,p_friend_id);
            }
            break;    
        case MSG_CTL_LIST_LONGOK:
            Chat_Contact_List_Item_LongClick(ctrl_id);
            break;
        
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
        case MSG_APP_OK:
            Chat_Contact_List_Item_Click(ctrl_id);
            break;
                  
        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
        break;

        case MSG_KEYDOWN_RED:
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

#endif

#endif //end chat

#ifdef ZDT_PLAT_YX_SUPPORT
LOCAL MMI_RESULT_E HandleZDT_BHMainMenuWinMsg (
                                      MMI_WIN_ID_T   win_id,     // 窗口的ID
                                      MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                      DPARAM            param        // 相应消息的参数
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
   
    MMI_RESULT_E     recode = MMI_RESULT_TRUE;
    static uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id  = MMIZDT_YX_ONOFF_CTRL_ID;
    uint16                     max_item = 2; 

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



#define QRCODE_DISP_X    40
#define QRCODE_DISP_Y    40
#define QRCODE_DISP_WIDTH    160
#define QRCODE_DISP_HEIGHT    160

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

    if(PNULL == zoom_in_ptr ||PNULL == zoom_out_ptr )
    {
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
        sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
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
                Trace_Log_Print("IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info)");            
                return FALSE;
            }        
            w=dec_info.img_detail_info.bmp_info.img_width;
            h=dec_info.img_detail_info.bmp_info.img_height;
            Trace_Log_Print("HandleQrcodeWindow w=%d h=%d",w, h);
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
            Trace_Log_Print("NULL == pszRGBData");
            return FALSE;
        }

        if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle))
        {
            Trace_Log_Print("IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle)");
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
                    Trace_Log_Print("is_zoom_success = TRUE;");
                    is_zoom_success = TRUE;
                }
                else
                {
                    Trace_Log_Print("is_zoom_success = FALSE;");
                    is_zoom_success = FALSE;
                }
            }
            else
            {
               Trace_Log_Print("is_zoom_success = FALSE");
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
                                IMAGE_ZDT_YX_2VM,
                                &lcd_dev_info);
                break;

                //yangyu end

            }
            else if(qrcode_page_index == 1)
            {
                char ewm_str[200] = {0};
                uint16 large = 20;
                SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
                sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
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
               /* GUIRES_DisplayImg(&dis_point,
                &bg_rect,
                PNULL,
                win_id,
                IMAGE_QRCODE_MANUAL,
                &lcd_dev_info);*/
                break;
                //yangyu end

            }
            if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
            {    
                Trace_Log_Print("IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info)");            
                return FALSE;
            }        
            w=dec_info.img_detail_info.bmp_info.img_width;
            h=dec_info.img_detail_info.bmp_info.img_height;
            Trace_Log_Print("HandleQrcodeWindow w=%d h=%d",w, h);
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
                IMAGE_ZDT_YX_2VM,
                &lcd_dev_info);
        }
        pszRGBData=(uint8*)SCI_ALLOC_APP(w*h*2);
        if(NULL == pszRGBData)
        {
            Trace_Log_Print("NULL == pszRGBData");
            return FALSE;
        }

        if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle))
        {
            Trace_Log_Print("IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle)");
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
                    Trace_Log_Print("is_zoom_success = TRUE;");
                    is_zoom_success = TRUE;
                }
                else
                {
                    Trace_Log_Print("is_zoom_success = FALSE;");
                    is_zoom_success = FALSE;
                }
            }
            else
            {
               Trace_Log_Print("is_zoom_success = FALSE");
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
                                        IMAGE_ZDT_YX_2VM,
                                        &lcd_dev_info);
                        break;
        
                        //yangyu end
        
                    }
                    else if(qrcode_page_index == 1)
                    {
                        char ewm_str[200] = {0};
                        uint16 large = 20;
                        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
                        sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
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
                       /* GUIRES_DisplayImg(&dis_point,
                        &bg_rect,
                        PNULL,
                        win_id,
                        IMAGE_QRCODE_MANUAL,
                        &lcd_dev_info);*/
                        break;
                        //yangyu end
        
                    }
                    if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
                    {   
                        Trace_Log_Print("IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info)");          
                        return FALSE;
                    }       
                    w=dec_info.img_detail_info.bmp_info.img_width;
                    h=dec_info.img_detail_info.bmp_info.img_height;
                    Trace_Log_Print("HandleQrcodeWindow w=%d h=%d",w, h);
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
                        IMAGE_ZDT_YX_2VM,
                        &lcd_dev_info);
                }
                pszRGBData=(uint8*)SCI_ALLOC_APP(w*h*2);
                if(NULL == pszRGBData)
                {
                    Trace_Log_Print("NULL == pszRGBData");
                    return FALSE;
                }
        
                if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle))
                {
                    Trace_Log_Print("IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle)");
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
                            Trace_Log_Print("is_zoom_success = TRUE;");
                            is_zoom_success = TRUE;
                        }
                        else
                        {
                            Trace_Log_Print("is_zoom_success = FALSE;");
                            is_zoom_success = FALSE;
                        }
                    }
                    else
                    {
                       Trace_Log_Print("is_zoom_success = FALSE");
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
        
            text_style.font = SONG_FONT_28;
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
                        YX_API_DLT_Send();
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

        case MSG_KEYDOWN_RED:
            break;

        case MSG_KEYUP_RED:
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
    GUI_RECT_T                          full_rect = {0, 0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_HEIGHT -1)};

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
        GUI_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);
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
        break;

    case MSG_KEYDOWN_RED:
        break;

    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
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

PUBLIC void MMIZDT_Open2VMWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_2VM_WIN_TAB,PNULL);
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
                                      MMI_WIN_ID_T   win_id,     // 窗口的ID
                                      MMI_MESSAGE_ID_E     msg_id,     // 窗口的内部消息ID
                                      DPARAM            param        // 相应消息的参数
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

#if 1
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
        
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
    MMIZDT_OpenChatWinByRcv();
#else       //ZDT_PLAT_YX_SUPPORT_FRIEND
    MMIZDT_OpenTinyChatWin();
#endif      //ZDT_PLAT_YX_SUPPORT_FRIEND
#endif

            MMK_CloseWin( win_id );
        break;

    case MSG_KEYUP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    case MSG_KEYDOWN_RED:
        break;

    case MSG_KEYUP_RED:
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

PUBLIC void CloseVoiceNewMsgDialog()
{
    //MMIPUB_CloseAlertWin();
    MMK_CloseWin(MMIVOICE_NEWMSG_WIN_ID);

}


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
#ifdef BAIRUI_VIDEOCHAT_SUPPORT
LOCAL BOOLEAN isVideoChatOverDialog = FALSE;

PUBLIC void DelayShowNewMsgPrompt()
{
    if(isVideoChatOverDialog)
    {
        isVideoChatOverDialog = FALSE;
        ShowVoiceNewMsgPrompt();
    }
}
#endif
extern BOOLEAN g_is_inVideo ; //视频通话中
extern BOOLEAN g_is_activeVideo ;

PUBLIC void MMIVoice_ShowNewMsgPrompt(
                             BOOLEAN    is_play_ring
                             )
{
    MMIDEFAULT_TurnOnBackLight();
	SCI_TRACE_LOW("MMIVoice_ShowNewMsgPrompt g_is_inVideo=%d",g_is_inVideo);
    if (MMIZDT_IsInClassModeWin()
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
    || MMIZDT_IsInTinyChatWin()
#endif
	|| (g_is_inVideo == TRUE)
	|| (g_is_activeVideo == TRUE)
    )
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
#ifdef XYSDK_SUPPORT
        LIBXMLYAPI_AppExit();
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
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
        //MMIZDT_OpenTinyChatWin();
#endif
        ShowVoiceNewMsgPrompt();
    }

}
#endif
#endif //ZDT_PLAT_YX_SUPPORT

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
    GUI_RECT_T                          full_rect = {0, 0, (MMI_MAINSCREEN_WIDTH -1), (MMI_MAINSCREEN_WIDTH -1)};
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

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL,
                &full_rect,
                PNULL,
                win_id,
                IMAGE_WATCH_BIND_STEP_2,
                &lcd_dev_info);

            MMIZDT_Check2VMFile();
        SCI_MEMSET(ewm_str, 0, 200*sizeof(char));
        sprintf(ewm_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
        //dec_src.file_name_ptr = QRCODE_BIND_FILE_NAME;
        GT_Qcode_show(ewm_str, 4, 0, 174, 174); 

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
        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_GREEN:
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
                IMAGE_WATCH_BIND_STEP_1,
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
                if(point.x < MMI_MAINSCREEN_WIDTH/3 && point.y > MMI_MAINSCREEN_HEIGHT/3)
                {
                    unbind_click_count++;
                    if(unbind_click_count >= 12)
                    {
                        unbind_click_count = 0;
                        YX_API_DLT_Send();
                    }
                }
                else
                {
                    unbind_click_count = 0;
                }
            }
            s_is_2vm_tp_down = FALSE;
            
    }          
        break;
        
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_GREEN:
    case MSG_CTL_PENOK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {
            MMIZDT_OpenBindStep2Win();
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

#if 1  //联系人窗口
#define ZDT_PB_ICON_LEFT_START        0
#define ZDT_PB_ICON_TOP_START        10
#define ZDT_PB_ICON_WIDTH        120
#define ZDT_PB_ICON_HEIGHT        75
#define ZDT_PB_TEXT_HEIGHT        30

uint16 m_yx_cur_whitelist_idx = 0;
static uint16 m_yx_whitelist_totalnum=0;
static uint16 m_yx_whitelist_pagenum=0;
static uint16 m_yx_whitelist_curpage=0;
static uint16 m_yx_whitelist_curindex=0;
static int s_pb_tp_x;
static int s_pb_tp_y;
static BOOLEAN s_is_pb_tp_down = FALSE;

PUBLIC void ZDT_PB_GetContactNameByNumber(uint8 *number, uint16 *dst_name)
{
    uint8 i = 0;
    if(number == NULL || dst_name == NULL)
    {
        return;
    }
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        
        if(strcmp(yx_DB_White_Reclist[i].white_num,  number) == 0)
        {
            ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,dst_name, 100);
            return;
        }
    }  

}

PUBLIC MMI_IMAGE_ID_T ZDT_PB_GetDispInfoContent(uint8 *tele_num, uint8 tele_len, wchar *in_str, BOOLEAN is_mo_call)
{
    MMI_STRING_T        nameStr = {0};
    MMI_IMAGE_ID_T               imageId = 0;
    BOOLEAN             isEmergencyCall = FALSE;

    if((tele_num == PNULL) || (in_str == PNULL))
    {
        return NULL;
    }
    isEmergencyCall = MMIAPICC_IsEmergencyNum(tele_num, tele_len, MMICC_GetWantCallDualSys(), FALSE);
    //get info for display
    if(isEmergencyCall && is_mo_call )
    {
#ifdef MODEM_SUPPORT_RDA
        if(0 != tele_len)
        {
            MMI_STRNTOWSTR(in_str,MMIPB_NUMBER_MAX_STRING_LEN,tele_num,tele_len,tele_len);
        }
#else
        MMI_GetLabelTextByLang(TXT_PB_EMERGENCY, &nameStr);
        MMI_WSTRNCPY(in_str,MMIPB_NAME_MAX_LEN,(const wchar*)nameStr.wstr_ptr, nameStr.wstr_len,nameStr.wstr_len);
#endif
        imageId = IMAGE_ZDT_PB_FATHER;//IAMGE_CALL_PORTRAIT_EMERGENCY;
    }
    else
    {
        if (0 != tele_len)
        {
            uint8 i = 0;
            for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
            {
                //这个地方有问题比如+86135xxxxxx 就匹配不了
                if(strcmp(yx_DB_White_Reclist[i].white_num, tele_num) == 0)
                {
                    ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,in_str, 100);                  
                #ifdef ZTE_WATCH
                    imageId = WATCHCOM_GetAvaterBigImageId(in_str);
                #else
                    imageId = WATCHCOM_GetAvaterImageId(in_str);
                #endif
                    break;
                }
            }  
        }
    }
    //no name in pb and is not emergency
    if (imageId == 0)
    {
    #ifdef ZTE_WATCH
        imageId = IMAGE_ZDT_PB_OTHER_BIG;
    #else
        imageId = IMAGE_ZDT_PB_OTHERMAN;
    #endif
        if(0 != tele_len)
        {
            MMI_STRNTOWSTR(in_str,MMIPB_NUMBER_MAX_STRING_LEN,tele_num,tele_len,tele_len);
        }
        else
        {
            MMI_STRING_T  dispStr = {0};
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_UNKNOW_NUM, &dispStr);
            MMI_WSTRNCPY(in_str, MMIPB_NAME_MAX_LEN, dispStr.wstr_ptr, dispStr.wstr_len, dispStr.wstr_len);
        }
    }
    return imageId;
}

PUBLIC BOOLEAN ZDT_Number_Is_In_Contact(uint8 *number)
{
    uint8 i = 0;
    if(number == NULL)
    {
        return FALSE;
    }
    for(i = 0; i < YX_DB_WHITE_MAX_SUM; i++)
    {
        
        if(strcmp(yx_DB_White_Reclist[i].white_num,  number) == 0)
        {
            return TRUE;
        }
    }  
    return FALSE;

}

PUBLIC BOOLEAN ZDT_Reject_UnknownCall()
{
    ZDT_LOG("zdt__ ZDT_Reject_UnknownCall");
    return ZDT_NV_GetRejectUnknownCall() ;
}

PUBLIC BOOLEAN ZDT_Reject_Call(uint8 *number)
{
    ZDT_LOG("zdt__ ZDT_Reject_Call, number=%s", number);
    return  !ZDT_Number_Is_In_Contact(number);
}

PUBLIC BOOLEAN ZDT_Device_Disable( GUI_POINT_T tp_point)
{
    BOOLEAN isLogOn = FALSE;
    MMIENG_GetUSBLog(&isLogOn);
    return (MMIZDT_ShouldStayInChargingWin() || MMIZDT_ShouldStayInClassModeWin())/* && !isLogOn*/;
}

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

BOOLEAN MMIZDT_PB_Call_Start(uint16 white_idx)
{
    uint8 i = 0;
    uint16 tempStr[101] ={0};
    if(white_idx == 0 || white_idx > YX_DB_WHITE_MAX_SUM || SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_num) == 0)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        return FALSE;
    }
    SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));
    ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[white_idx-1].white_name, SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_name) ,tempStr, 100);            
        
    MMIZDT_MakeAll(yx_DB_White_Reclist[white_idx-1].white_num,SCI_STRLEN(yx_DB_White_Reclist[white_idx-1].white_num),tempStr);
    return TRUE;
}

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
    uint32 image_id = IMAGE_ZDT_PB_FATHER;
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
            SCI_MEMSET(TempStr,0,(101)*sizeof(uint16));     
            ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,TempStr, 100);
           
            if(MMIAPICOM_Wstrstr(TempStr,ba_wStr)||MMIAPICOM_Wstrstr(TempStr,fu_wStr))
            {
                image_id = IMAGE_ZDT_PB_FATHER;
            }
            else if(MMIAPICOM_Wstrstr(TempStr,ma_wStr)||MMIAPICOM_Wstrstr(TempStr,mu_wStr))
            {
                image_id = IMAGE_ZDT_PB_MOTHER;
            } 
            else if(MMIAPICOM_Wstrstr(TempStr,ye_wStr))
            {
                image_id = IMAGE_ZDT_PB_GRANDPA;
            }
            else if(MMIAPICOM_Wstrstr(TempStr,nai_wStr))
            {
                image_id = IMAGE_ZDT_PB_GRANDMA;
            }
        #ifndef ZTE_WATCH
            else if(MMIAPICOM_Wstrstr(TempStr,waigong_wStr))
            {
                image_id = IMAGE_ZDT_PB_WAIGONG;
            }
            else if(MMIAPICOM_Wstrstr(TempStr,waipo_wStr))
            {
                image_id = IMAGE_ZDT_PB_WAIPO;
            }         
            else if(MMIAPICOM_Wstrstr(TempStr,shu_wStr))
            {
                image_id = IMAGE_ZDT_PB_UNCLE;
            }
            else if(MMIAPICOM_Wstrstr(TempStr,yi_wStr))
            {
                image_id = IMAGE_ZDT_PB_AUNT;
            }      
            else
            {
                image_id = IMAGE_ZDT_PB_OTHERMAN;            
            }
        #else
            else
            {
                image_id = IMAGE_ZDT_PB_OTHER;            
            }
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
extern VIDEO_USER_INFO video_users[MAX_VIDEO_USERS_COUNT];
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
    if(MMIAPICOM_Wstrstr(tempStr,ba_wStr)||MMIAPICOM_Wstrstr(tempStr,fu_wStr))
     {
            image_id = IMAGE_ZDT_PB_FATHER;
     }
    else if(MMIAPICOM_Wstrstr(tempStr,ma_wStr)||MMIAPICOM_Wstrstr(tempStr,mu_wStr))
     {
            image_id = IMAGE_ZDT_PB_MOTHER;
     } 
    else if(MMIAPICOM_Wstrstr(tempStr,ye_wStr))
     {
            image_id = IMAGE_ZDT_PB_GRANDPA;
     }
    else if(MMIAPICOM_Wstrstr(tempStr,nai_wStr))
     {
            image_id = IMAGE_ZDT_PB_GRANDMA;
     }
    else if(MMIAPICOM_Wstrstr(tempStr,waigong_wStr))
     {
            image_id = IMAGE_ZDT_PB_WAIGONG;
     }
    else if(MMIAPICOM_Wstrstr(tempStr,waipo_wStr))
     {
            image_id = IMAGE_ZDT_PB_WAIPO;
     }         
     else if(MMIAPICOM_Wstrstr(tempStr,shu_wStr))
     {
            image_id = IMAGE_ZDT_PB_UNCLE;
     }
     else if(MMIAPICOM_Wstrstr(tempStr,yi_wStr))
     {
            image_id = IMAGE_ZDT_PB_AUNT;
     }
    else
    {
        #if 1 // xiongkai      亲人号码前面图标固定用一个。
                  image_id = IMAGE_ZDT_PB_OTHERMAN;     
        #else
        if(i%2)
        {
                  image_id = IMAGE_ZDT_PB_OTHERWOMAN;
        }
        else
        {
                  image_id = IMAGE_ZDT_PB_OTHERMAN;
        }
        #endif
     }
    return image_id;

}
#endif

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

void  MMIZDT_PB_ShowList(MMI_WIN_ID_T win_id)
{
    BOOLEAN ret = FALSE;
    MMI_STRING_T    cur_str_t   = {0};
    uint16 tempStr[101] ={0};
    uint16 phoneNumberStr[30] ={0};
    uint16 i = 0, j = 0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint16 position = 0;

    GUILIST_ITEM_T               item_t    =  {0};
    GUILIST_ITEM_DATA_T          item_data = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    //add by James li begin
    uint16 total_item_num = 0;
    MMI_STRING_T    empty_str = {0};
    GUILIST_EMPTY_INFO_T empty_info = {0};
    //add by James li end
    
#ifdef ZTE_WATCH
    item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1;
#else
    item_t.item_style = GUIITEM_STYLE_1ICON_2STR_1ICON;
#endif

    item_t.item_data_ptr = &item_data; 

    for(i=0;i<YX_DB_WHITE_MAX_SUM;i++)
    {
       if(yx_DB_White_Reclist[i].db_id)
       {
           m_yx_whitelist_totalnum++;
       }
    }
    GUILIST_RemoveAllItems(MMIZDT_PB_LIST_CTRL_ID);

    for(i = 0 ;i < m_yx_whitelist_totalnum ; i++)
    {
        if(yx_DB_White_Reclist[i].db_id == 0)
        {
            break;
        }
        SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));     
        ZDT_UCS_Str16_to_uint16((uint8*)yx_DB_White_Reclist[i].white_name, SCI_STRLEN(yx_DB_White_Reclist[i].white_name) ,tempStr, 100);
        image_id = WATCHCOM_GetAvaterImageId(tempStr);      
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[0].item_data.image_id = image_id;
        
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;      
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = tempStr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);

        SCI_MEMSET(phoneNumberStr,0,sizeof(phoneNumberStr));
        GUI_UTF8ToWstr(phoneNumberStr,100,yx_DB_White_Reclist[i].white_num,strlen(yx_DB_White_Reclist[i].white_num));
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;    
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = phoneNumberStr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (phoneNumberStr);

        item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;        
        item_data.item_content[3].item_data.image_id = IMAGE_PHONE_ICON;
        //MMIAPICOM_StrToWstr(content , item_data.item_content[0].item_data.text_buffer.wstr_ptr);// luan ma,Garbled
        //GUI_GBToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese

        //GUI_GB2UCS(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
        //GUI_GBKToWstr(item_data.item_content[0].item_data.text_buffer.wstr_ptr, (const uint8*)content, SCI_STRLEN(content));//luan ma,has chinese
        //GUI_UTF8ToWstr(item_data.item_content[1].item_data.text_buffer.wstr_ptr, GUILIST_STRING_MAX_NUM + 1, content, SCI_STRLEN(content));

        
        ret = GUILIST_AppendItem (MMIZDT_PB_LIST_CTRL_ID, &item_t);
        if(ret)
        {
            CTRLLIST_SetItemUserData(MMIZDT_PB_LIST_CTRL_ID, position, &i); 
            #if 1//def NEW_LEFT_RIGHT_STYLE 
            //GUILIST_SetItemStyleEx(MMIZDT_PB_LIST_CTRL_ID, position, &new_style );
            #endif
            position++;   
        }
        
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
            list_init.both_rect.v_rect.left = 0;
            list_init.both_rect.v_rect.right = (MMI_MAINSCREEN_WIDTH -1);
            list_init.both_rect.v_rect.top = 0;
            list_init.both_rect.v_rect.bottom = (MMI_MAINSCREEN_HEIGHT-1);//240;//180;

            list_init.both_rect.h_rect.left = 0;
            list_init.both_rect.h_rect.right = (MMI_MAINSCREEN_HEIGHT -1);//240;//180;
            list_init.both_rect.h_rect.top = 0;
            list_init.both_rect.h_rect.bottom = (MMI_MAINSCREEN_WIDTH -1);
            
            list_init.type = GUILIST_TEXTLIST_E;
                        
            GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init); 
            //不画分割线
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
            //不画高亮条
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );
            ret = GUILIST_SetMaxItem(ctrl_id, YX_DB_WHITE_MAX_SUM, FALSE);
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
            GUILIST_GetItemData(ctrl_id, num_index, &pos_user_data);// user_data stand position
            MMIZDT_PB_Call_Start(pos_user_data+1);
         }
         break;
        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        case MSG_NOTIFY_LIST_ITEM_CONTENT_CLICK:
        {
        //case MSG_KEYDOWN_WEB:
            uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_GetItemData(ctrl_id, index, &pos_user_data);// user_data stand position
            MMIZDT_PB_Call_Start(pos_user_data+1);
            //ZDT_LOG("HandleZDT_TinyChatWinMsg MSG_CTL_MIDSK  index=%d, pos_user_data=%d" , index, pos_user_data);
        }

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

PUBLIC void MMIZDT_OpenPBWin(void)
{
    if(MMIZDT_IsPhoneBookEmpty())
    {
        WatchCOM_NoteWin_1Line_Enter_Timeout(MMIZDT_WATCH_PB_NOTE_WIN_ID,STR_CONTA_NOTE_NO_CONTA_TEXT,PNULL,0);
    }
    else
    {
        MMK_CreateWin((uint32*)MMIZDT_WATCH_PB_WIN_TAB,PNULL);
    }
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

#if 1  //天气窗口
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
    if((g_yx_wt_info.min_degree == 0 &&  g_yx_wt_info.max_degree == 0) || Is_WeatherExpired())
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
    //add by James li begin
    MMI_STRING_T    display_string    = {0};
    wchar    wstr_buf[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
    //add by James li end
   
    MMI_IMAGE_ID_T weather_img_id = IMAGE_WEATHER_UNKNOWN;

    uint16 qing_wstr[2] = {0x6674, 0};//qing
    uint16 yin_wstr[2] = {0x9634, 0};//yin
    uint16 yu_wstr[2] = {0x96E8, 0};//yu
    uint16 xue_wstr[2] = {0x96EA, 0};//xue


    GUI_RECT_T    wt_rect=WEATHER_TEXT_RECT;
    GUI_RECT_T    temperature_rect=WEATHER_TEMPERATURE_RECT;
    GUI_RECT_T    update_time_rect=WEATHER_UPDATE_TIME_RECT;    
    GUI_RECT_T    city_rect=WEATHER_CITY_RECT;
    GUI_RECT_T    sim_rect=WEATHER_SIM_RECT;
    GUI_RECT_T    bg_rect = MMITHEME_GetFullScreenRect();


    GUI_RECT_T    text_rect={0};
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

    GUI_FillRect(&lcd_dev_info,bg_rect, MMI_BLACK_COLOR);
    
    LCD_DrawThickLine(&lcd_dev_info,0,40,MMI_MAINSCREEN_WIDTH,40,MMI_DARK_GRAY2_COLOR,1);
    LCD_DrawThickLine(&lcd_dev_info,0,160,MMI_MAINSCREEN_WIDTH,160,MMI_DARK_GRAY2_COLOR,1);

    point.x = 17;
    point.y = 8 ;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_WEATHER_LOCATION,&lcd_dev_info);

    //city
    tmp_string.wstr_ptr = g_yx_wt_info.city_name;
    tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
    text_style.align = ALIGN_LEFT;
    text_style.font = SONG_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;
    GUISTR_DrawTextToLCDInRect( 
                MMITHEME_GetDefaultLcdDev(),
                (const GUI_RECT_T      *)&city_rect,       //the fixed display area
                (const GUI_RECT_T      *)&city_rect,       //用户要剪切的实际区域
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
            weather_img_id = IMAGE_BIG_WEATHER_RAIN;
            weather_string_id=TXT_WEATHER_RAIN;            
            break;
        case 3:
            weather_img_id = IMAGE_BIG_WEATHER_CLOUD;
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
 
    point.x = WEATHER_ICON_X;
    point.y = WEATHER_ICON_Y;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, weather_img_id,&lcd_dev_info); 
    

    //weather text
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)weather_string_id,&tmp_string);
    text_style.align = ALIGN_LEFT;
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = SONG_FONT_32;
    GUISTR_DrawTextToLCDInRect( 
                MMITHEME_GetDefaultLcdDev(),
                (const GUI_RECT_T      *)&wt_rect,       //the fixed display area
                (const GUI_RECT_T      *)&wt_rect,       //用户要剪切的实际区域
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
                (const GUI_RECT_T      *)&temperature_rect,       //用户要剪切的实际区域
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
                    (const GUI_RECT_T      *)&update_time_rect,       //用户要剪切的实际区域
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
                (const GUI_RECT_T      *)&sim_rect,       //ó??§òa???Dμ?êμ?ê??óò
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

        case MSG_KEYDOWN_RED:
            break;

        case MSG_KEYUP_RED:        //BUG 天气界面按红键去菜单了
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

LOCAL MMI_RESULT_E  HandleZDT_NoSimOrDataWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T        str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    GUI_RECT_T tip_str_rect = {0};
    switch(msg_id) 
    {
        case MSG_OPEN_WINDOW:
            break;
        case MSG_FULL_PAINT:
            LCD_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_NO_SIM_OR_DATA,&str_data);
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font_color = MMI_WHITE_COLOR;
            text_style.font = SONG_FONT_22;
            GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
                        (const GUI_RECT_T      *)&full_rect,       //the fixed display area
                        (const GUI_RECT_T      *)&full_rect,       //用户要剪切的实际区域
                        (const MMI_STRING_T    *)&str_data,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );         
            break;

        case MSG_KEYDOWN_RED:
            break;
        
        case MSG_CLOSE_WINDOW:
        case MSG_KEYUP_RED:       
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

WINDOW_TABLE( MMIZDT_NoSimOrData_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_NoSimOrDataWinMsg),    
    WIN_ID( MMIZDT_NO_SIM_OR_DATA_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

//没有SIM卡或者无网络
PUBLIC void MMIZDT_OpenNoSimOrDataWin()
{
   if(MMK_GetFocusWinId() != MMIZDT_NO_SIM_OR_DATA_WIN_ID)
   {
        MMK_CreateWin((uint32*)MMIZDT_NoSimOrData_WIN_TAB,PNULL);
   }
}

static uint8 s_find_friend_timer_id = 0;

LOCAL void Draw_FindFriend_Search_Text(MMI_WIN_ID_T win_id)
{
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T        search_str = {0};
    MMI_STRING_T        search_tip_str = {0};
    GUI_RECT_T search_str_rect = SEARCH_ING_STR_RECT;
    GUI_RECT_T search_tip_rect = SEARCH_ING_TIP_STR_RECT;
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMIRES_GetText(TXT_FIND_FIREND_SEARCH, win_id, &search_str);
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = SONG_FONT_22;
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
                (const GUI_RECT_T      *)&search_str_rect,
                (const GUI_RECT_T      *)&search_str_rect,      
                (const MMI_STRING_T    *)&search_str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
    state = GUISTR_STATE_ALIGN | GUISTR_STATE_ELLIPSIS;
    MMIRES_GetText(TXT_FIND_FIREND_SEARCH_TIP, win_id, &search_tip_str);
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
                (const GUI_RECT_T      *)&search_tip_rect,
                (const GUI_RECT_T      *)&search_tip_rect,      
                (const MMI_STRING_T    *)&search_tip_str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
}

LOCAL void Draw_FindFriend_Search_Bg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    GUI_POINT_T point = {0};
    point.x = FIND_FRIEND_PING_X;
    point.y = FIND_FRIEND_PING_Y;
    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, find_friend_ping, dev_info_ptr);
    point.x = FIND_FRIEND_PONG_X;
    point.y = FIND_FRIEND_PONG_Y;
    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, find_friend_waiting1, dev_info_ptr);
    point.x = FIND_FRIEND_WAITING_X;
    point.y = FIND_FRIEND_WAITING_Y;
    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, find_friend_pong, dev_info_ptr);
}

LOCAL void FindFriend_TimeOut_Callback()
{
    MMK_CloseWin(MMIZDT_FIND_FRIEND_WIN_ID);
}

LOCAL MMI_RESULT_E  HandleZDT_FindFriendWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T        str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    GUI_RECT_T tip_str_rect = {0};
    GUI_POINT_T point = {0};
    switch(msg_id) 
    {
        case MSG_OPEN_WINDOW:
            LCD_FillRect(&lcd_dev_info, full_rect, MMI_BLACK_COLOR);
            
            point.x = FIND_FRIEND_PING_X;
            point.y = FIND_FRIEND_PING_Y;
            GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, find_friend_ping, &lcd_dev_info);
            point.x = FIND_FRIEND_PONG_X;
            point.y = FIND_FRIEND_PONG_Y;
            GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, find_friend_waiting1, &lcd_dev_info);
            point.x = FIND_FRIEND_WAITING_X;
            point.y = FIND_FRIEND_WAITING_Y;
            GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, find_friend_pong, &lcd_dev_info);
            Draw_FindFriend_Search_Text(win_id);
            YX_Net_Friend_Start(&g_yx_app);
            if(s_find_friend_timer_id == 0)
            {
                MMK_StopTimer(s_find_friend_timer_id);
                s_find_friend_timer_id = 0;
            }
            s_find_friend_timer_id = MMK_CreateTimerCallback(FIND_FRIEND_TIME_OUT,FindFriend_TimeOut_Callback,(uint32)win_id, FALSE);
            break;
        case MSG_FULL_PAINT: 

            break;

        case MSG_KEYDOWN_RED:
            break;
        
        case MSG_KEYUP_RED:       
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);

            break;
        case MSG_CLOSE_WINDOW:
            if(s_find_friend_timer_id == 0)
            {
                MMK_StopTimer(s_find_friend_timer_id);
                s_find_friend_timer_id = 0;
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

WINDOW_TABLE( MMIZDT_FIND_FRIEND_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_FindFriendWinMsg),    
    WIN_ID( MMIZDT_FIND_FRIEND_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};


PUBLIC void MMIZDT_OpenFindFriendWin()
{
   if(MMK_GetFocusWinId() != MMIZDT_FIND_FRIEND_WIN_ID)
   {
        MMK_CreateWin((uint32*)MMIZDT_FIND_FRIEND_WIN_TAB,PNULL);
   }
}

PUBLIC void ZdtApp_YX_Friend_DrawResult(YX_APP_T *pMe,YXFriendDispStatus status)
{
    if(status == YX_FRIEND_CONFIRM_OK)
    {
        
    }
}

PUBLIC BOOLEAN ZdtApp_YX_Friend_DrawSearchRcv(YX_APP_T *pMe, uint8 index, uint8* pContent, uint16 Content_len)
{
    ZDT_LOG("ZdtApp_YX_Friend_DrawSearchRcv content:%s",pContent);

    return FALSE;
}

#if 1//add_schedule
static int s_scd_tp_x;
static int s_scd_tp_y;
static BOOLEAN s_is_scd_tp_down = FALSE;
static int scdwin_is_wating = 0;

LOCAL THEMELIST_ITEM_STYLE_T title_style = {0};
LOCAL THEMELIST_ITEM_STYLE_T content_style = {0};
LOCAL THEMELIST_ITEM_STYLE_T none_style = {0};
void MMIZDT_Schedule_ShowList(MMI_WIN_ID_T win_id,uint8 current_day)
{
    BOOLEAN ret = FALSE;

    GUILIST_ITEM_T       		item_t    =  {0};
    GUILIST_ITEM_DATA_T  		item_data = {0};

	MMI_CTRL_ID_T ctrl_id = MMIZDT_SCHEDULE_CTRL_ID;

	uint16 position = 0;

    const uint16 height = 40;//65;

    GUI_RECT_T title0_rect = { 90,  10,  150 , height};//text
    GUI_RECT_T title1_rect = { 150,  10,  160 , height};//text

    GUI_RECT_T content0_rect = { 35,  10,  130 , height};// 上午 12:30
    GUI_RECT_T content1_rect = { 5,  10,  30 , height};// 1
    GUI_RECT_T content2_rect = { 130,  10,  230 , height};// 语文

    GUI_RECT_T none0_rect = { 60,  10,  180 , 200};//text
    GUI_RECT_T none1_rect = { 180,  10,  185 , 200};//text

    SCI_DATE_T date = {0};
    MMI_STRING_T                week_string            = {0};
	wchar				content_text[100 + 1] = {0};
	uint16 uint16_str_len = 0;

	uint8 temp_str[64] = {0};
	MMI_STRING_T tmp_string = {0};
    uint16 class_time = 0;
	wchar				class_time_text[640 + 1] = {0};
	uint16 class_time_len = 0;

	wchar						temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
	wchar						num_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

	wchar						temp0_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
	wchar						num0_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

	uint16 tempStr[101] ={0};

    uint16 am_wstr[10] = {0x4e0a, 0x5348, 0}; //上午\u4e0a\u5348
    uint16 pm_wstr[10] = {0x4e0b, 0x5348, 0}; //下午\u4e0b\u5348  

    uint16 none_wstr[10] = {0x7a7a, 0}; //空

    uint16 alert_wstr[10] = {0x672a, 0x8bbe,0x7f6e, 0x8bfe,0x7a0b, 0xff01, 0}; //未设置课程！ \u672a\u8bbe\u7f6e\u8bfe\u7a0b\uff01

	int i = 0;
	int j = 0;
	
    MMI_TEXT_ID_T week_table[] =
    {
        STXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
        STXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
        STXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
        STXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
        STXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
        STXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
        STXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
    };


	THEMELIST_ITEM_STYLE_T* pItemStyle_title = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT);
	THEMELIST_ITEM_STYLE_T* pItemStyle_content = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT);
	THEMELIST_ITEM_STYLE_T* pItemStyle_none = THEMELIST_GetStyle(GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT);
	SCI_MEMCPY(&title_style, pItemStyle_title, sizeof(*pItemStyle_title));
	SCI_MEMCPY(&content_style, pItemStyle_content, sizeof(*pItemStyle_content));
	SCI_MEMCPY(&none_style, pItemStyle_none, sizeof(*pItemStyle_none));
    
    MMI_GetLabelTextByLang(week_table[current_day-1], &week_string);
	MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &week_string);
	

	title_style.height = title_style.height_focus = 40;
	title_style.content[0].rect = title_style.content[0].rect_focus = title0_rect;
	title_style.content[0].font= title_style.content[0].font_focus = SONG_FONT_20;
	title_style.content[0].state = GUIITEM_CONTENT_STATE_TEXT_M_ALIGN;
	title_style.content[1].rect = title_style.content[1].rect_focus = title1_rect;
	title_style.content[1].font= title_style.content[1].font_focus = SONG_FONT_20;

	content_style.height = content_style.height_focus = 40;
	content_style.content[0].rect = content_style.content[0].rect_focus = content0_rect;
	content_style.content[0].font= content_style.content[0].font_focus = SONG_FONT_20;
	content_style.content[0].state = 0;
	content_style.content[1].rect = content_style.content[1].rect_focus = content1_rect;
	content_style.content[1].font= content_style.content[1].font_focus = SONG_FONT_20;
	content_style.content[1].state = GUIITEM_CONTENT_STATE_TEXT_R_ALIGN;
	content_style.content[2].rect = content_style.content[2].rect_focus = content2_rect;
	content_style.content[2].font= content_style.content[2].font_focus = SONG_FONT_20;
	content_style.content[2].state = GUIITEM_CONTENT_STATE_TEXT_M_ALIGN;

	none_style.height = none_style.height_focus = 200;
	none_style.content[0].rect = none_style.content[0].rect_focus = none0_rect;
	none_style.content[0].font= none_style.content[0].font_focus = SONG_FONT_20;
	none_style.content[0].state = GUIITEM_CONTENT_STATE_TEXT_M_ALIGN;
	none_style.content[1].rect = none_style.content[1].rect_focus = none1_rect;
	none_style.content[1].font= none_style.content[1].font_focus = SONG_FONT_20;

	item_t.item_data_ptr = &item_data;
	item_t.item_style =  GUIITEM_STYLE_CUSTOMIZE;
	item_t.item_state |= GUIITEM_STATE_SELFADAPT_RECT | GUIITEM_STATE_CONTENT_CHECK|GUIITEM_STATE_NEED_ITEM_CONTENT;
	GUILIST_RemoveAllItems(ctrl_id);
	item_t.item_style =  GUIITEM_STYLE_ONE_LINE_TEXT;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;		
	item_data.item_content[0].item_data.text_buffer.wstr_ptr = content_text;
	item_data.item_content[0].item_data.text_buffer.wstr_len = uint16_str_len;
	item_data.item_content[0].is_custom_font_color = TRUE;
	item_data.item_content[0].custom_font_color = MMI_WHITE_COLOR;//RGB888TO565(0XB2B2B2);//GUI_RGB2RGB565(171,142,112); 	
	item_data.item_content[0].custom_font_color_focus = MMI_WHITE_COLOR;//RGB888TO565(0XB2B2B2);//GUI_RGB2RGB565(171,142,112); 

	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[1].item_data.text_buffer.wstr_ptr = 0;
	item_data.item_content[1].item_data.text_buffer.wstr_len = 0; 


	ret = GUILIST_AppendItem (ctrl_id, &item_t);
	if(ret)
	{
		//CTRLLIST_SetItemUserData(ctrl_id, position, &list_all_index); 
		GUILIST_SetItemStyleEx(ctrl_id, position, &title_style );
		position++;   
	}

if(YX_DB_is_have_Schedule(current_day-1))	
{
	int item_num =0;
	//memset(temp_str,0,sizeof(temp_str));
	for( i = 0; i < SCHEDULE_TIME_MUN_MAX;i++)	
	{
		//SCI_TRACE_LOW("XX MMIZDT_Schedule_ShowList have  class_content_Type [%d][%d]=%s",current_day-1,i, yx_schedule_Rec.class_content_Type[current_day-1][i].class_content_wstr);
		if(SCI_STRLEN(yx_schedule_Rec.class_content_Type[current_day-1][i].class_content_str) == 0)
		{
			item_num ++;
			continue ;
		}	

        SCI_MEMSET (temp0_wstr, 0, sizeof (temp0_wstr));
        SCI_MEMSET (num0_wstr, 0, sizeof (num0_wstr));
		sprintf ( (char*) temp0_wstr, "%s", yx_schedule_Rec.schedule_time[i]);
		MMIAPICOM_StrToWstr (temp0_wstr, num0_wstr);	

		item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[0].item_data.text_buffer.wstr_ptr = num0_wstr;
		item_data.item_content[0].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (num0_wstr);
		
		/*if (yx_schedule_Rec.schedule_time_end[i] < 12)
		{
			item_data.item_content[0].item_data.text_buffer.wstr_ptr = am_wstr;
			item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(am_wstr);
		}else{
			item_data.item_content[0].item_data.text_buffer.wstr_ptr = pm_wstr;
			item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(pm_wstr);
		}*/
		item_data.item_content[0].is_custom_font_color = TRUE;
		item_data.item_content[0].custom_font_color = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112); 0;//		
		item_data.item_content[0].custom_font_color_focus = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);			

        SCI_MEMSET (temp_wstr, 0, sizeof (temp_wstr));
        SCI_MEMSET (num_wstr, 0, sizeof (num_wstr));
		sprintf ( (char*) temp_wstr, "%d.", ++j);
		MMIAPICOM_StrToWstr (temp_wstr, num_wstr);		
		item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;		
		item_data.item_content[1].item_data.text_buffer.wstr_ptr = num_wstr;
		item_data.item_content[1].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (num_wstr);
		item_data.item_content[1].is_custom_font_color = TRUE;
		item_data.item_content[1].custom_font_color = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);		
		item_data.item_content[1].custom_font_color_focus = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);			

        SCI_MEMSET(tempStr,0,(101)*sizeof(uint16));	
		if(SCI_STRLEN(yx_schedule_Rec.class_content_Type[current_day-1][i].class_content_str) == 0)
		{
			item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;		
			item_data.item_content[2].item_data.text_buffer.wstr_ptr = none_wstr;
			item_data.item_content[2].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen (none_wstr);
		}else{
			ZDT_UCS_Str16_to_uint16((uint8*)yx_schedule_Rec.class_content_Type[current_day-1][i].class_content_str, SCI_STRLEN(yx_schedule_Rec.class_content_Type[current_day-1][i].class_content_str) ,tempStr, 100);
			item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;		
			item_data.item_content[2].item_data.text_buffer.wstr_ptr = tempStr;
			item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (tempStr);
		}
		item_data.item_content[2].is_custom_font_color = TRUE;
		item_data.item_content[2].custom_font_color = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);		
		item_data.item_content[2].custom_font_color_focus = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);			
		
		ret = GUILIST_AppendItem (ctrl_id, &item_t);
		if(ret)
		{
			//CTRLLIST_SetItemUserData(ctrl_id, position, &i); 
			GUILIST_SetItemStyleEx(ctrl_id, position, &content_style );
			position++;   
		}


	}

    for( i = 0; i < item_num;i++)	
    {
    	//SCI_TRACE_LOW("XX MMIZDT_Schedule_ShowList no_have class_content_Type [%d][%d]=%s",current_day-1,i, yx_schedule_Rec.class_content_Type[current_day-1][i].class_content_wstr);

    	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    	item_data.item_content[0].item_data.text_buffer.wstr_ptr = 0;
    	item_data.item_content[0].item_data.text_buffer.wstr_len = 0;
    	item_data.item_content[0].is_custom_font_color = TRUE;
    	item_data.item_content[0].custom_font_color = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112); 0;//		
    	item_data.item_content[0].custom_font_color_focus = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);			
	
    	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;		
    	item_data.item_content[1].item_data.text_buffer.wstr_ptr = 0;
    	item_data.item_content[1].item_data.text_buffer.wstr_len = 0;
    	item_data.item_content[1].is_custom_font_color = TRUE;
    	item_data.item_content[1].custom_font_color = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);		
    	item_data.item_content[1].custom_font_color_focus = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);			
    

		item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;		
		item_data.item_content[2].item_data.text_buffer.wstr_ptr = 0;
		item_data.item_content[2].item_data.text_buffer.wstr_len = 0;
    	item_data.item_content[2].is_custom_font_color = TRUE;
    	item_data.item_content[2].custom_font_color = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);		
    	item_data.item_content[2].custom_font_color_focus = MMI_WHITE_COLOR;//GUI_RGB2RGB565(171,142,112);			
    	
    	ret = GUILIST_AppendItem (ctrl_id, &item_t);
    	if(ret)
    	{
    		//CTRLLIST_SetItemUserData(ctrl_id, position, &i); 
    		GUILIST_SetItemStyleEx(ctrl_id, position, &content_style );
    		position++;   
    	}
    
    }



}else{

		item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;		
		item_data.item_content[0].item_data.text_buffer.wstr_ptr = alert_wstr;
		item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen (alert_wstr);
		item_data.item_content[0].is_custom_font_color = TRUE;
		item_data.item_content[0].custom_font_color = MMI_WHITE_COLOR;//RGB888TO565(0XB2B2B2);//GUI_RGB2RGB565(171,142,112);	
		item_data.item_content[0].custom_font_color_focus = MMI_WHITE_COLOR;//RGB888TO565(0XB2B2B2);//GUI_RGB2RGB565(171,142,112); 

		item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
		item_data.item_content[1].item_data.text_buffer.wstr_ptr = 0;
		item_data.item_content[1].item_data.text_buffer.wstr_len = 0; 


		ret = GUILIST_AppendItem (ctrl_id, &item_t);
		if(ret)
		{
			//CTRLLIST_SetItemUserData(ctrl_id, position, &list_all_index); 
			GUILIST_SetItemStyleEx(ctrl_id, position, &none_style );
			position++;   
		}


}

}
LOCAL uint8 day_index = 0;
LOCAL BOOLEAN   s_is_cancel_msg = FALSE;
LOCAL BOOLEAN   s_is_ok_msg = FALSE;
LOCAL int   s_msg_num = 0;

LOCAL MMI_RESULT_E  HandleZDT_WatchScheduleWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
	GUI_POINT_T 				point				= {0};
	MMI_RESULT_E			recode	= MMI_RESULT_TRUE;
	MMI_CTRL_ID_T ctrl_id = MMIZDT_SCHEDULE_CTRL_ID;
	BOOLEAN ret = FALSE;
	uint32 pos_user_data = 0; // position user data 
	uint8 * p_index = PNULL;
	GUILIST_INIT_DATA_T list_init = {0}; 
    SCI_DATE_T date = {0};
		
	p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);

	
	SCI_TRACE_LOW("XX HandleZDT_WatchScheduleWinMsg msg_id=0x%x", msg_id);
	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
		{
			
			GUI_RECT_T listrect = {0,0,(MMI_MAINSCREEN_WIDTH -1),(MMI_MAINSCREEN_HEIGHT-1)};

			list_init.both_rect.v_rect.left = 0;
			list_init.both_rect.v_rect.right = (MMI_MAINSCREEN_WIDTH -1);
			list_init.both_rect.v_rect.top = 0;
			list_init.both_rect.v_rect.bottom = (MMI_MAINSCREEN_HEIGHT -1) ;//240;//180;

			list_init.both_rect.h_rect.left = 0;
			list_init.both_rect.h_rect.right = (MMI_MAINSCREEN_HEIGHT -1);//240;//180;
			list_init.both_rect.h_rect.top = 0;
			list_init.both_rect.h_rect.bottom = (MMI_MAINSCREEN_WIDTH -1);
			
			list_init.type = GUILIST_TEXTLIST_E;
			GUILIST_CreateListBox(win_id, 0, ctrl_id, &list_init);	
			ret = GUILIST_SetMaxItem(ctrl_id, SCHEDULE_TIME_MUN_MAX+2, FALSE);
			GUILIST_SetRect(MMK_ConvertIdToHandle(ctrl_id), &listrect);
			//GUILIST_SetNeedSplidLine(ctrl_id,TRUE);

			GUILIST_SetNeedSplidLine(ctrl_id,FALSE);//去掉分割线(有可能会切屏)
			GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE);//去掉高亮特效			
			MMK_SetAtvCtrl(win_id,ctrl_id);
			
			TM_GetSysDate(&date);
			day_index = date.wday;
		}
		break;
		
		case MSG_FULL_PAINT:
			{
				SCI_TRACE_LOW("XX HandleZDT_WatchScheduleWinMsg MSG_FULL_PAINT day_index=%d ", day_index);
				if(day_index == 0)
				{
					day_index = 7;
				}					
				MMIZDT_Schedule_ShowList(win_id,day_index);
				s_msg_num = 0;
			}
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		case MSG_LOSE_FOCUS:
			break;
		case MSG_CTL_MIDSK:
		{
			//uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
			//GUILIST_GetItemData(ctrl_id, index, &pos_user_data);// user_data stand position
			//MMIZDT_PB_Call_Start(pos_user_data+1);
		}

		break;
		case MSG_NOTIFY_LIST_SLIDE_STATE_CHANGE:
		{
			GUILIST_POS_INFO_T pos_info = {0};
			
			CTRLLIST_GetCurPosInfo(ctrl_id, &pos_info);

			SCI_TRACE_LOW("XX HandleZDT_WatchScheduleWinMsg MSG_NOTIFY_LIST_SLIDE_STATE_CHANGE s_msg_num=%d", s_msg_num);
			if(s_msg_num == 1)
			{
			
				day_index = day_index+1;
				if(day_index == 8)
				{
					day_index = 1;
				}
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}else if(s_msg_num == 2)
			{
				day_index = day_index-1;
				if(day_index == 0)
				{
					day_index = 7;
				}		
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}
			s_msg_num = 0;
		}
		break;
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			s_scd_tp_x = point.x;
			s_scd_tp_y = point.y; 
		}
		break;
		
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if(( z_abs(s_scd_tp_y - point.y) < 15 )&&(point.x - s_scd_tp_x > 15 ))//右滑
			 {
					day_index = day_index-1;
					if(day_index == 0)
					{
						day_index = 7;
					}		
					MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			 }
			
			if(( z_abs(s_scd_tp_y - point.y) < 15 )&&(s_scd_tp_x - point.x > 15 ))//左滑
			{
				day_index = day_index+1;
				if(day_index == 8)
				{
					day_index = 1;
				}
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}

		}
		break;
	case MSG_CTL_PENOK: //左滑
	case MSG_CTL_CANCEL: //右滑
		 {	
				s_msg_num++;
				SCI_TRACE_LOW("XX HandleZDT_WatchScheduleWinMsg MSG_CTL s_msg_num=%d ", s_msg_num);

			}
		break;

		
/*	case MSG_CTL_PENOK: //左滑
	 {	
			s_is_ok_msg = TRUE;
			SCI_TRACE_LOW("XX HandleZDT_WatchScheduleWinMsg MSG_CTL_PENOK s_is_cancel_msg=%d ", s_is_cancel_msg);
			if(!s_is_cancel_msg)
			{

				day_index = day_index+1;
				if(day_index == 8)
				{
					day_index = 1;
				}
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}
			s_is_cancel_msg=FALSE;
		}	
		break;	
	case MSG_CTL_CANCEL: //右滑
		{	
			s_is_cancel_msg = TRUE;
			SCI_TRACE_LOW("XX HandleZDT_WatchScheduleWinMsg MSG_CTL_CANCEL s_is_ok_msg=%d ", s_is_ok_msg);
			if(!s_is_ok_msg)
			{
				day_index = day_index-1;
				if(day_index == 0)
				{
					day_index = 7;
				}		
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}
			s_is_ok_msg=FALSE;
		}
		break;*/		
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
		
		
	case MSG_APP_CANCEL:
	case MSG_KEYUP_CANCEL:	//下键 
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


WINDOW_TABLE( MMIZDT_WATCH_SCHEDULE_WIN_TAB ) = {
	WIN_FUNC( (uint32)HandleZDT_WatchScheduleWinMsg),
	WIN_ID(MMIZDT_WATCH_SCHEDULE_WIN_ID),
	WIN_HIDE_STATUS,
	END_WIN
};


PUBLIC void MMIZDT_OpenScheduleWin(void)
{

	if(ZDT_SIM_Exsit() == FALSE)
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,STR_SIM_NOT_SIM_EXT01,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		return;
	}
	if (MMIAPICONNECTION_isGPRSSwitchedOff())
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		return;
	}
	if(yx_DB_Set_Rec.net_open == 0)
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_YX_WCHAT_NEED_OPEN,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		return;
	}

#ifdef WIN32
    YX_AddTestSCHEDULE();
#endif
	MMK_CreateWin((uint32 *)MMIZDT_WATCH_SCHEDULE_WIN_TAB,PNULL);
}


#endif
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
///-----------insert sim  tips win start-----------
LOCAL uint8 s_friend_pp_status = 0;
uint8 g_pp_friend_id[YX_DB_FRIEND_MAX_ID_SIZE+1] = {0};
uint16 g_pp_wstr_pp_name[41] = {0};
uint16 g_pp_wstr_pp_num[41] = {0};
LOCAL uint8 s_pp_disp_ui_timer_id = 0;

void MMIZDT_PPDisplay_HandleSuccessTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
   if(MMK_IsOpenWin(MMIZDT_FRIEND_PP_WIN_ID))
   {
        MMIZDT_OpenChatGroupWinByPP(g_pp_friend_id);
        MMK_CloseWin(MMIZDT_FRIEND_PP_WIN_ID);
   }
}

void MMIZDT_PPDisplay_HandleFailTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(MMK_IsOpenWin(MMIZDT_FRIEND_PP_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_FRIEND_PP_WIN_ID);
    }
}

PUBLIC void MMIZDT_PPDisplay_TimerStop()
{
    if(s_pp_disp_ui_timer_id != 0)
    {
        MMK_StopTimer(s_pp_disp_ui_timer_id);
    }
}

PUBLIC void MMIZDT_PPDisplay_TimerStart(uint32 time_out, MMI_TIMER_FUNC func)
{
    if(s_pp_disp_ui_timer_id != 0)
    {
        MMK_StopTimer(s_pp_disp_ui_timer_id);
    }
    s_pp_disp_ui_timer_id = MMK_CreateTimerCallback(time_out, func, 0, FALSE);
}

LOCAL MMI_RESULT_E  HandleZDT_FriendPPWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );



WINDOW_TABLE( MMIZDT_FRIEND_PP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_FriendPPWinMsg),    
    WIN_ID( MMIZDT_FRIEND_PP_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

void HandleZDT_FriendPP_ShowTxt(uint16 * w_str1, uint16 * w_str2,uint16 * w_str3)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16          uint16_str[100 + 50] = {0};
    uint16          uint16_str_len = 0;
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
    GUI_FONT_T f_font = SONG_FONT_26;
    char *deviceVersion=NULL;

    rect.left   = 0;
    rect.top    = 0;
    rect.right  = MMI_MAINSCREEN_WIDTH-1;
    rect.bottom = MMI_MAINSCREEN_HEIGHT-1;
    cur_rect = rect;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_font;
    text_style.font_color = MMI_WHITE_COLOR;
    
    if(w_str1 != NULL)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(w_str1);
        cur_str_t.wstr_ptr = w_str1;
        cur_rect.top    = 0; 
        cur_rect.bottom = 46;
        LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);

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
        
    if(w_str2 != NULL)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(w_str2);
        cur_str_t.wstr_ptr = w_str2;
        cur_rect.top    = 170; 
        cur_rect.bottom = 200;
        LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
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
    if(w_str3 != NULL)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(w_str3);
        cur_str_t.wstr_ptr = w_str3;
        cur_rect.top    = 201; 
        cur_rect.bottom = 239;
        LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
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

PUBLIC BOOLEAN MMIZDT_IsFriendPPWin(void)
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId() ;

    return win_id == MMIZDT_FRIEND_PP_WIN_ID;
}


PUBLIC BOOLEAN MMIZDT_IsFriendPPWinOpen(void)
{
    BOOLEAN ret = FALSE;
    if(MMK_IsOpenWin(MMIZDT_FRIEND_PP_WIN_ID))
    {
        ret = TRUE;
    }
    return ret;

}

PUBLIC void MMIZDT_OpenFriendPPWin(void)
{
    if(MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1) != SIM_STATUS_OK)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_SIM,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    #if 0
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    if(FALSE == YX_Net_Is_Land())
    {
        MMIZDT_Net_Reset(TRUE);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    #endif 
    if(FALSE == MMK_IsOpenWin(MMIZDT_FRIEND_PP_WIN_ID))
   {
        MMK_CreateWin((uint32*)MMIZDT_FRIEND_PP_WIN_TAB,PNULL);
   }
}

PUBLIC void MMIZDT_CloseFriendPPWin()
{
    if(MMK_IsOpenWin(MMIZDT_CLASSMODE_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_CLASSMODE_WIN_ID);
    }
}

PUBLIC BOOLEAN MMIZDT_UpdateFriendPPWinOpen(BOOLEAN is_ok)
{
    BOOLEAN ret;
    if(is_ok)
    {
        s_friend_pp_status = 1;
    }
    else
    {
        s_friend_pp_status = 2;
    }
    if(MMK_IsFocusWin(MMIZDT_FRIEND_PP_WIN_ID))
    {
        MMK_SendMsg(MMIZDT_FRIEND_PP_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    return ret;
}

LOCAL MMI_RESULT_E  HandleZDT_FriendPPWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_MENU_GROUP_ID_T 		group_id		=	0;
    MMI_MENU_ID_T				menu_id 		=	0;
    MMI_RESULT_E				recode			=	MMI_RESULT_TRUE;
    GUI_RECT_T		bg_rect = MMITHEME_GetFullScreenRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    MMI_STRING_T		str_data = {0};
    GUI_RECT_T	text_display_rect={0};
    GUISTR_STYLE_T		text_style = {0};
    GUISTR_STATE_T		state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    uint16 wstr_pp_ok[30] = {0x4EA4,0x53CB,0x6210,0x529F,0x0 };//交友成功
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
                s_friend_pp_status = 0;
                SCI_MEMSET(g_pp_wstr_pp_name,0,sizeof(g_pp_wstr_pp_name));
                SCI_MEMSET(g_pp_wstr_pp_num,0,sizeof(g_pp_wstr_pp_num));
                YX_Net_Friend_Start(&g_yx_app);
                ZdtTalk_BackLight(TRUE);
            break;
            
        case MSG_GET_FOCUS:
            break;
            
        case MSG_FULL_PAINT:
        {
            LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
            if(s_friend_pp_status == 0)
            {
                //搜索中
                Draw_FindFriend_Search_Bg(win_id, &lcd_dev_info);
                Draw_FindFriend_Search_Text(win_id);
                ZdtTalk_BackLight(TRUE);
            }
            else if(s_friend_pp_status == 1)
            {
                //交友成功
            	GUIRES_DisplayImg(PNULL,&bg_rect,PNULL,win_id,IMAGE_PP_SUCCESS,&lcd_dev_info);
                HandleZDT_FriendPP_ShowTxt(wstr_pp_ok,g_pp_wstr_pp_name,g_pp_wstr_pp_num);
                ZdtTalk_BackLight(FALSE);
                MMIZDT_PPDisplay_TimerStart(3000,MMIZDT_PPDisplay_HandleSuccessTimer);
            }
            else if(s_friend_pp_status == 2)
            {
                //交友失败
                GUIRES_DisplayImg(PNULL,&bg_rect,PNULL,win_id,IMAGE_PP_FAIL,&lcd_dev_info);
                ZdtTalk_BackLight(FALSE);
                MMIZDT_PPDisplay_TimerStart(5000,MMIZDT_PPDisplay_HandleFailTimer);
            }
            else
            {
                LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
                Draw_FindFriend_Search_Bg(win_id, &lcd_dev_info);
                ZdtTalk_BackLight(FALSE);
                MMIZDT_PPDisplay_TimerStart(5000,MMIZDT_PPDisplay_HandleFailTimer);

            }
        }
        break;
		
#if 1//def TOUCH_PANEL_SUPPORT
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
			int offset_y =	0;//up to down
			int offset_x =	0;//up to down
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
            if(s_friend_pp_status == 2 && (point.x > 5 && point.x < 235)&&(point.y >= 160 &&  point.y < 235))
            {
                s_friend_pp_status = 0;
                YX_Net_Friend_Start(&g_yx_app);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
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

	case MSG_CTL_MIDSK:
		break;

	case MSG_BACKLIGHT_TURN_ON:
	case MSG_BACKLIGHT_TURN_OFF:

		break;
	case MSG_LOSE_FOCUS:
	case MSG_CLOSE_WINDOW:
            s_friend_pp_status = 0;
        MMIZDT_PPDisplay_TimerStop();
        ZdtTalk_BackLight(FALSE);
        break;


	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
    case MSG_KEYDOWN_RED:
        break;      
    case MSG_KEYUP_RED:
		MMK_CloseWin(win_id);
        break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
}
    return recode;

}

///---------FriendPP win end----------
#endif
