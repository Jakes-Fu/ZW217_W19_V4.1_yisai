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
#ifndef _MMIZDT_WINTAB_FOTA_C_
#define _MMIZDT_WINTAB_FOTA_C_

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
#include "version.h"
#include "watch_launcher_main.h"

#ifdef FOTA_SUPPORT_CMIOT
#include "cmiot_type.h"
int ZDT_Fota_UpgradeReset(void);
int ZDT_Fota_UpgradeDirect(void);
int ZDT_Fota_Upgrade(void);
int ZDT_Fota_GetVer(void);
int ZDT_Fota_Open(void);
int ZDT_Fota_Close(void);
int ZDT_Fota_get_download_percent(void);
#endif
PUBLIC void MMIZDT_StopFirstFotaVerTimer();

#ifdef WIN32
void POWER_FOTA_Update_Reset(){}
int cmiot_only_upgrade(){return 1;}
int cmiot_upgrade(){return 1;}
int cmiot_check_version(char *ver){return 1;}
uint32 cmiot_get_download_index(){return 10;}
uint32 cmiot_get_download_index_max(){return 1000;}

#endif

#ifdef FOTA_SUPPORT
#define UI_DEVICE_WIDTH  MMI_MAINSCREEN_WIDTH
#define UI_DEVICE_HEIGHT  MMI_MAINSCREEN_HEIGHT

#define FOTA_UI_BG_COLOR  MMI_BLACK_COLOR
#define FOTA_UI_FONT_COLOR  MMI_WHITE_COLOR

#define FOTA_UI_BIG_FONT  SONG_FONT_26
#define FOTA_UI_SMALL_FONT  SONG_FONT_20

#define FOTA_UI_TXT_TOP  40
#define FOTA_UI_TXT_HIGHT  26

#define FOTA_UI_NEW_VER_TOP  100
#define FOTA_UI_NEW_VER_HIGHT  26

#define FOTA_UI_CUR_VER_TOP  FOTA_UI_NEW_VER_TOP
#define FOTA_UI_CUR_VER_HIGHT  26
#define FOTA_UI_VER_STR_HIGHT  26

#define ZDT_FOTA_TRACE                 SCI_TRACE_LOW

uint8 g_setting_upgrade_auto =0;
uint32 g_setting_upgrade_offset_time = 0;
int g_setting_upgrade_dl_percent =0;
uint8 g_setting_upgrade_status =0;
uint8 g_setting_upgrade_pen_timer_falg=0;
char g_setting_upgrade_new_ver[150]={0};
uint8 g_fota_disp_timer_id = 0;
uint8 g_fota_check_timer_id = 0;
uint8 g_fota_wait_timer_id = 0;

void FOTA_Download_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        );
void FOTA_Download_TimeStart(uint32 time_ms,MMI_TIMER_FUNC  func);
void FOTA_upgrade_ShowDownLoad(int percent);
void FOTA_upgrade_ShowTxt(uint16 * w_str1, uint16 * w_str2);
void FOTA_upgrade_ShowSoftkey(uint16 * w_str1, uint16 * w_str2);
void FOTA_Upgrade_SetAuto(uint8 auto_mode);
void FOTA_upgrade_ExitDownload(void);
void FOTA_upgrade_StartDownload(void);
void MMIAPISET_EnterFotaWin(void);


LOCAL MMI_RESULT_E CheckUpdateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

WINDOW_TABLE(MMISET_PHONE_FOTA_WIN_TAB) =
{
    WIN_FUNC((uint32)CheckUpdateWinHandleMsg),
    WIN_ID(MMISET_PHONE_SETTING_FOTA_WIN_ID),
	WIN_STYLE(WS_DISABLE_FLING_CLOSE_WIN|WS_DISABLE_RETURN_WIN),//不允许系统处理右滑和power键退出
    WIN_HIDE_STATUS,
    END_WIN
};


void FOTA_Check_TimeOut(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID))
    {
        uint16 softkey_left[10] = {0x67E5, 0x8BE2,0};
        uint16 softkey_bk[10] = {0x9000,0x51FA, 0};
        uint16 r_wstr[50] = {0x67E5, 0x8BE2, 0x5931, 0x8D25, 0xFF01, 0x0};
        uint16 r_wstr2[50] = {0x8BF7, 0x91CD, 0x65B0, 0x68C0, 0x6D4B , 0x0};
        uint16 nver_wstr[30] = {0x5DF2, 0x7ECF, 0x662F, 0x6700, 0x65B0, 0x7248, 0x672C, 0x0 };
        if(g_setting_upgrade_status == 4)
        {
            FOTA_upgrade_ShowTxt(nver_wstr,PNULL);
            FOTA_upgrade_ShowSoftkey(softkey_left,softkey_bk);
        }
        else
        {
            FOTA_upgrade_ShowTxt(r_wstr,r_wstr2);
            FOTA_upgrade_ShowSoftkey(softkey_left,softkey_bk);
        }
        MMITHEME_UpdateRect();
        g_setting_upgrade_status = 0;
    }

    return;
}

void FOTA_Check_TimeStart(uint32 time_ms)
{
    if(0 != g_fota_check_timer_id)
    {
        MMK_StopTimer(g_fota_check_timer_id);
        g_fota_check_timer_id = 0;
    }
    
    g_fota_check_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        FOTA_Check_TimeOut, 
                                                                        (uint32)PNULL, 
                                                                        FALSE);
}

void FOTA_Check_TimeStop(void)
{
    if(0 != g_fota_check_timer_id)
    {
        MMK_StopTimer(g_fota_check_timer_id);
        g_fota_check_timer_id = 0;
    }
}

BOOLEAN FOTA_SIM_Exsit(void)
{
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;

    sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
    if(sim_ok_num ==0)
    {
        return FALSE;
    }
    return TRUE;
}

BOOLEAN FOTA_Bat_IsLow(void)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    uint8 percent = 0;
    if(CHGMNG_IsChargeConnect())
    {
        return FALSE;
    }
    if(p_chgmng_info != PNULL)
    {
        percent = p_chgmng_info->bat_remain_cap;
    }
    if(percent < 30)
    {
        return TRUE;
    }
    return FALSE;
}

static void FOTA_Switch_BackLight(BOOLEAN is_alway_on)
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

void FOTA_upgrade_ShowTxt(uint16 * w_str1, uint16 * w_str2)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16          uint16_str[100 + 50] = {0};
    uint16          uint16_str_len = 0;
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
    GUI_FONT_T f_font = FOTA_UI_BIG_FONT;
    char *deviceVersion=NULL;

    rect.left   = 0;
    rect.top    = 0;
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;


    cur_rect = rect;
    cur_rect.top    = FOTA_UI_TXT_TOP; 
    cur_rect.bottom = cur_rect.top + FOTA_UI_TXT_HIGHT*2;
    LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);
    text_style.align = ALIGN_HMIDDLE;
    text_style.font = f_font;
    text_style.font_color = MMI_DARK_BLUE_COLOR;
    
    if(w_str1 != NULL)
    {
        cur_rect.bottom = cur_rect.top + FOTA_UI_TXT_HIGHT;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(w_str1);
        cur_str_t.wstr_ptr = w_str1;
        LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);

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
        cur_rect.bottom = cur_rect.top;
    }
    cur_rect.top = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + FOTA_UI_TXT_HIGHT;
    
    if(w_str2 != NULL)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(w_str2);
        cur_str_t.wstr_ptr = w_str2;
        text_style.font_color = MMI_DARK_BLUE_COLOR;
        LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);
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

void FOTA_upgrade_ShowNewVer(char * new_ver)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16          uint16_str[100 + 50] = {0};
    uint16          uint16_str_len = 0;
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
    uint16 ver_wstr[10] = {0x6700,0x65B0, 0x7248, 0x672C, 0x003A, 0}; //×?D?°?±?:6700 65B0 7248 672C 003A 
    GUI_FONT_T f_font =FOTA_UI_BIG_FONT;
    char *deviceVersion=NULL;

    if(new_ver == PNULL)
    {
        return;
    }
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;


    cur_rect = rect;
    cur_rect.top    = FOTA_UI_NEW_VER_TOP; 
    cur_rect.bottom = cur_rect.top + FOTA_UI_NEW_VER_HIGHT;

    text_style.align = ALIGN_LEFT;
    text_style.font = f_font;
    text_style.font_color = MMI_DARK_RED_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(ver_wstr);
    cur_str_t.wstr_ptr = ver_wstr;
    LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    cur_rect.top = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + FOTA_UI_VER_STR_HIGHT*2;
    deviceVersion = new_ver;
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)deviceVersion, SCI_STRLEN((char*)deviceVersion));
    cur_str_t.wstr_len = uint16_str_len;
    cur_str_t.wstr_ptr = uint16_str;

    LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);
    text_style.font = FOTA_UI_SMALL_FONT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    return;
}

void FOTA_upgrade_ShowCurVer(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16          uint16_str[100 + 50] = {0};
    uint16          uint16_str_len = 0;
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
    uint16 ver_wstr[10] = {0x5F53,0x524D, 0x7248, 0x672C, 0x003A, 0};
    GUI_FONT_T f_font =FOTA_UI_BIG_FONT;
    char *deviceVersion=NULL;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;


    cur_rect = rect;
    cur_rect.top    = FOTA_UI_CUR_VER_TOP; 
    cur_rect.bottom = cur_rect.top + FOTA_UI_CUR_VER_HIGHT;

    text_style.align = ALIGN_LEFT;
    text_style.font = f_font;
    text_style.font_color = FOTA_UI_FONT_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(ver_wstr);
    cur_str_t.wstr_ptr = ver_wstr;
    LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    cur_rect.top = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + FOTA_UI_VER_STR_HIGHT*2;
    #ifdef WIN32
        deviceVersion = "UIS8910_240x320BAR_16MB_DS_DEBUG_20210424_1410";
    #else
        deviceVersion = ZDT_FOTA_VER;
    #endif
    MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_str_len, (uint8*)deviceVersion, SCI_STRLEN((char*)deviceVersion));
    cur_str_t.wstr_len = uint16_str_len;
    cur_str_t.wstr_ptr = uint16_str;

    LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);
    text_style.font = FOTA_UI_SMALL_FONT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    return;
}

void FOTA_upgrade_ShowSoftkey(uint16 * w_str1, uint16 * w_str2)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16          uint16_str[100 + 50] = {0};
    uint16          uint16_str_len = 0;
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_font =FOTA_UI_BIG_FONT;
    char *deviceVersion=NULL;

    rect.left   = 0;
    rect.top    = 0;
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;


    cur_rect = rect;
    cur_rect.top  = UI_DEVICE_HEIGHT-FOTA_UI_TXT_HIGHT-12; 

    
    LCD_FillRect(&lcd_dev_info, cur_rect, FOTA_UI_BG_COLOR);
    
    if(w_str1 != NULL)
    {
        cur_rect.left = 16;
        cur_rect.right = UI_DEVICE_WIDTH/2;
        text_style.align = ALIGN_LVMIDDLE;
        text_style.font = f_font;
        text_style.font_color = FOTA_UI_FONT_COLOR;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(w_str1);
        cur_str_t.wstr_ptr = w_str1;
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
        cur_rect.left = UI_DEVICE_WIDTH/2;
        cur_rect.right = UI_DEVICE_WIDTH-16;
        text_style.align = ALIGN_RVMIDDLE;
        text_style.font = f_font;
        text_style.font_color = FOTA_UI_FONT_COLOR;
        
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

void FOTA_upgrade_Show(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0}; 
    uint16 softkey_bk[10] = {0x9000,0x51FA, 0};
    uint16 check_wstr[20] = {0x6B63, 0x5728, 0x68C0, 0x6D4B, 0x66F4, 0x65B0,0x002E, 0x002E, 0x002E, 0};
    uint16 nver_wstr[30] = {0x68C0, 0x6D4B, 0x5230, 0x66F4, 0x65B0, 0x0 }; 
    uint16 nver_wstr2[30] = {0x5F00, 0x59CB, 0x4E0B, 0x8F7D, 0x0 };
    GUI_RECT_T   cur_rect          = {0};
    GUISTR_STYLE_T      text_style      = {0};  
    MMI_STRING_T    cur_str_t   = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, FOTA_UI_BG_COLOR);

    cur_rect = rect;
    cur_rect.top    = 0; 
    cur_rect.bottom = cur_rect.top + 26;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_18;
    text_style.font_color = MMI_WHITE_COLOR;

    MMIRES_GetText(TXT_SETTINGS_ABUP_FOTA_CHECK, PNULL, &cur_str_t);;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    
    if(g_setting_upgrade_status == 1)
    {
        FOTA_upgrade_ShowTxt(check_wstr,PNULL);
        FOTA_upgrade_ShowCurVer();
        FOTA_upgrade_ShowSoftkey(PNULL,softkey_bk);
    }
    else if(g_setting_upgrade_status == 2)
    {
        FOTA_upgrade_ShowTxt(nver_wstr,nver_wstr2);
        if(SCI_STRLEN(g_setting_upgrade_new_ver) > 0)
        {
            FOTA_upgrade_ShowNewVer(g_setting_upgrade_new_ver);
        }
		else
		{
	        FOTA_upgrade_ShowCurVer();
		}
    }
    else if(g_setting_upgrade_status == 3)
    {
        if(SCI_STRLEN(g_setting_upgrade_new_ver) > 0)
        {
            FOTA_upgrade_ShowNewVer(g_setting_upgrade_new_ver);
        }
		else
		{
	        FOTA_upgrade_ShowCurVer();
		}
        FOTA_upgrade_ShowDownLoad(g_setting_upgrade_dl_percent);
    }
    else if(g_setting_upgrade_status == 4)
    {
        uint16 softkey_left[10] = {0x67E5, 0x8BE2,0};
        uint16 softkey_bk[10] = {0x9000,0x51FA, 0};
        uint16 nver_wstr[30] = {0x5DF2, 0x7ECF, 0x662F, 0x6700, 0x65B0, 0x7248, 0x672C, 0x0 };
        FOTA_upgrade_ShowTxt(nver_wstr,PNULL);
        FOTA_upgrade_ShowSoftkey(softkey_left,softkey_bk);
    }
    else if(g_setting_upgrade_status == 0)
    {
        uint16 softkey_left[10] = {0x67E5, 0x8BE2,0};
        FOTA_upgrade_ShowTxt(PNULL,PNULL);
        FOTA_upgrade_ShowCurVer();
        FOTA_upgrade_ShowSoftkey(softkey_left,softkey_bk);
    }
    
    return;
}

void FOTA_Wait_TimeOut(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
	MMIAPISET_EnterFotaWin();
	//MMK_SendMsg(MMISET_PHONE_SETTING_FOTA_WIN_ID, MSG_FULL_PAINT, PNULL);
	
	//FOTA_upgrade_StartDownload();
      MMK_CloseWin(MMISET_FOTA_WHIT_WIN_ID);
    return;
}

LOCAL MMI_RESULT_E UpdateWhitWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0};
    uint16 wait_text[10] = {0x8bf7,0x7a0d,0x5019,0x002e,0x002e,0x002e, 0}; //\u8bf7\u7a0d\u5019\u002e\u002e\u002e\ua
    GUI_RECT_T   cur_rect          = {0};
    GUISTR_STYLE_T      text_style      = {0};  
    MMI_STRING_T    cur_str_t   = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
	{
		if(0 != g_fota_wait_timer_id)
		{
			MMK_StopTimer(g_fota_wait_timer_id);
			g_fota_wait_timer_id = 0;
		}
		
		g_fota_wait_timer_id = MMK_CreateTimerCallback(2000, 
														FOTA_Wait_TimeOut, 
														(uint32)PNULL, 
														FALSE);
	}
        break;

    case MSG_FULL_PAINT:
		{
			rect.left   = 0;
		    rect.top    = 0; 
		    rect.right  = UI_DEVICE_WIDTH-1;
		    rect.bottom = UI_DEVICE_HEIGHT-1;
			
			LCD_FillRect(&lcd_dev_info, rect, FOTA_UI_BG_COLOR);

		    cur_rect = rect;

		    text_style.align = ALIGN_HVMIDDLE;
		    text_style.font = SONG_FONT_22;
		    text_style.font_color = MMI_WHITE_COLOR;

		    MMIRES_GetText(TXT_ABUP_FOTA_WAIT, PNULL, &cur_str_t);;

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
     break;
     
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

WINDOW_TABLE(MMISET_FOTA_WHIT_WIN_TAB) =
{
	WIN_FUNC((uint32)UpdateWhitWinHandleMsg),
	WIN_ID(MMISET_FOTA_WHIT_WIN_ID),
	WIN_HIDE_STATUS,
    END_WIN
};

LOCAL MMI_RESULT_E handleFOTA_upgradeDialog(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    MMI_WIN_ID_T                wait_win_id             =   0;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL != add_data_ptr)
    {
        is_delete_all_data= (uint32)add_data_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_RETURN, RIGHT_BUTTON, TRUE);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
	{
            uint32 cur_time_ms = ZDT_Get_TimeSecond();
 		#if 1//FOTA_ABUP_TEST
               MMK_CreateWin((uint32*)MMISET_FOTA_WHIT_WIN_TAB,PNULL);
	        MMIPUB_CloseQuerytWin(PNULL);
	        MMK_CloseWin(win_id);
              if(cur_time_ms - g_setting_upgrade_offset_time > 10)
              {
                  FOTA_Upgrade_SetAuto(1);
              }
              else
              {
                FOTA_Upgrade_SetAuto(2);
              }
		#else
		MMIAPISET_EnterFotaWin();
		g_setting_upgrade_status = 2;

		FOTA_upgrade_StartDownload();
	    MMK_SendMsg(MMISET_PHONE_SETTING_FOTA_WIN_ID, MSG_FULL_PAINT, PNULL);
		#endif
	}

        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
		//FOTA_upgrade_ExitDownload();
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

   result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    return (result);
}

void FOTA_upgrade_Alert(void)
{
    MMI_WIN_ID_T	dialog_win_id = MMIPUB_ALERT_WIN_ID;

	if(FOTA_Bat_IsLow())
	 {
		 MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_BAT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		 return ;
	 }

	
	if(MMIDEFAULT_GetBackLightStateEx() == MMI_BACKLIGHT_OFF)
	{
		MMIDEFAULT_TurnOnLCDBackLight();
	}
    MMIPUB_OpenQueryWinByTextId(STR_QUERY_FOTA_NOTIFY,IMAGE_PUBWIN_QUERY,&dialog_win_id,handleFOTA_upgradeDialog);
}



void FOTA_MMI_Set_NewVersion(char * new_ver)
{
    if(new_ver != PNULL)
    {
        SCI_MEMSET(g_setting_upgrade_new_ver,0,sizeof(g_setting_upgrade_new_ver));
        SCI_STRCPY(g_setting_upgrade_new_ver,new_ver);
    }
}

void FOTA_upgrade_HaveNewVer(char * ver_str)
{
    if(ver_str != PNULL && MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID))
    {
        uint16 softkey_bk[10] = {0x9000,0x51FA, 0}; //í?3?9000 51FA 
        uint16 softkey_confirm[10] = {0x786e,0x5b9a, 0}; 
        uint16 nver_wstr[30] = {0x68C0, 0x6D4B, 0x5230, 0x66F4, 0x65B0, 0x0 };//检测到更新68C0 6D4B 5230 66F4 65B0 
        uint16 nver_wstr2[30] = {0x8BF7,0x786E, 0x8bA4, 0x4E0b, 0x8F7d, 0x0 };//?aê?????5F00 59CB 4E0B 8F7D 
        if(g_setting_upgrade_status <= 1)
        {
            FOTA_upgrade_ShowTxt(nver_wstr,nver_wstr2);
            g_setting_upgrade_status = 2;
        }
        FOTA_MMI_Set_NewVersion(ver_str);
        FOTA_upgrade_ShowNewVer(ver_str);
        FOTA_upgrade_ShowSoftkey(softkey_confirm,softkey_bk);
        FOTA_Check_TimeStop();
        MMITHEME_UpdateRect();
        if(g_setting_upgrade_auto)
        {
            MMK_PostMsg(MMISET_PHONE_SETTING_FOTA_WIN_ID, MSG_KEYDOWN_OK, PNULL,0);
        }
    }
    else
    {
        g_setting_upgrade_offset_time = ZDT_Get_TimeSecond();
        FOTA_MMI_Set_NewVersion(ver_str);
        if(g_setting_upgrade_auto > 0)
        {
            if(FALSE == MMK_IsOpenWin(MMISET_FOTA_WHIT_WIN_ID))
            {
               MMK_CreateWin((uint32*)MMISET_FOTA_WHIT_WIN_TAB,PNULL);
            }
        }
        else
        {
            FOTA_upgrade_Alert();//开机检测到新版本弹框	
        }
        FOTA_Check_TimeStop();
    }
    return;
}

void FOTA_Upgrade_SetAuto(uint8 auto_mode)
{
    g_setting_upgrade_auto = auto_mode;
}

void FOTA_upgrade_ShowLastVer(void)
{
    if(MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID))
    {
        if(g_setting_upgrade_status <= 1)
        {
            g_setting_upgrade_status = 4;
            FOTA_Check_TimeStop();
            FOTA_Check_TimeStart(2000);
       }
    }

    return;
}

void FOTA_upgrade_ShowFotaExit(void)
{
    if(MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID))
    {
        if(g_setting_upgrade_status == 1)
        {
            uint16 softkey_left[10] = {0x67E5, 0x8BE2,0}; //查询67E5 8BE2 
            uint16 softkey_bk[10] = {0x9000,0x51FA, 0}; //退出9000 51FA 
            uint16 r_wstr[50] = {0x67E5, 0x8BE2, 0x5931, 0x8D25, 0xFF01, 0x0}; //查询失败！67E5 8BE2 5931 8D25 FF01 
            uint16 r_wstr2[50] = {0x8BF7, 0x91CD, 0x65B0, 0x68C0, 0x6D4B , 0x0}; //请重新检测8BF7 91CD 65B0 68C0 6D4B 
            FOTA_upgrade_ShowTxt(r_wstr,r_wstr2);
            FOTA_upgrade_ShowSoftkey(softkey_left,softkey_bk);
            g_setting_upgrade_status = 0;
            MMITHEME_UpdateRect();
        }
        FOTA_Check_TimeStop();
    }
    return;
}

void FOTA_upgrade_ShowDownLoad(int percent)
{
   // if(MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID))
    {
        char tmp_str[20] = {0};
        uint16 disp_wstr[150] = {0};
        uint16 disp_len = 0;
        uint16 softkey_bk[10] = {0x9000,0x51FA, 0}; //¨a?3?9000 51FA 
        uint16 dwn_wstr[30] = {0x6B63, 0x5728, 0x4E0B, 0x8F7D, 0x66F4, 0x65B0, 0x002E, 0x002E, 0x002E, 0x0 };//?y?¨2?????¨1D?...
        sprintf(tmp_str,"%d%%",percent);
        g_setting_upgrade_dl_percent = percent;
        MMIAPICOM_StrcatFromStrToUCS2(disp_wstr, &disp_len, (uint8*)tmp_str, SCI_STRLEN((char*)tmp_str));
        FOTA_upgrade_ShowTxt(dwn_wstr,disp_wstr);
        FOTA_upgrade_ShowSoftkey(PNULL,softkey_bk);
        MMITHEME_UpdateRect();
        FOTA_Download_TimeStart(2000, FOTA_Download_HandleTimer);
    }
    return;
}


void FOTA_Download_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_setting_upgrade_status == 3)
    {
        int percent = 0;
#ifdef FOTA_SUPPORT_CMIOT
        percent = ZDT_Fota_get_download_percent();
#endif
#ifdef WIN32
        percent = g_setting_upgrade_dl_percent+20;
        if(percent > 100)
        {
            percent = 100;
        }
#endif
        FOTA_upgrade_ShowDownLoad(percent);
    }
}

void FOTA_Download_TimeStart(uint32 time_ms,MMI_TIMER_FUNC  func)
{
    if(0 != g_fota_disp_timer_id)
    {
        MMK_StopTimer(g_fota_disp_timer_id);
        g_fota_disp_timer_id = 0;
    }
    
    g_fota_disp_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        func, 
                                                                        (uint32)PNULL, 
                                                                        FALSE);
}

void FOTA_Download_TimeStop(void)
{
    if(0 != g_fota_disp_timer_id)
    {
        MMK_StopTimer(g_fota_disp_timer_id);
        g_fota_disp_timer_id = 0;
    }
}


void FOTA_upgrade_StartDownload(void)
{
    if(g_setting_upgrade_status == 2 || g_setting_upgrade_status == 1)
    {
        g_setting_upgrade_dl_percent = 0;
        FOTA_upgrade_ShowDownLoad(0);
#ifdef FOTA_SUPPORT_CMIOT
        ZDT_Fota_Upgrade();
#endif
        g_setting_upgrade_status = 3;
    }
}

void FOTA_upgrade_ExitDownload(void)
{
    if(g_setting_upgrade_status == 2)
    {
        g_setting_upgrade_dl_percent = 0;
        g_setting_upgrade_status = 0;
    }
}
void FOTA_upgrade_ShowDownloadResult(uint8 is_ok)
{
    if(MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID))
    {
        FOTA_Download_TimeStop();
        if(is_ok)
        {
            //下载成功，正在重启升级...
            uint16 r_wstr[50] = {0x4E0B, 0x8F7D, 0x6210, 0x529F, 0x0};
            uint16 r_wstr2[50] = {0x6B63, 0x5728, 0x91CD, 0x542F, 0x5347, 0x7EA7, 0x002E, 0x002E, 0x002E, 0x0};
            FOTA_upgrade_ShowTxt(r_wstr,r_wstr2);
        }
        else
        {
            //下载失败!4E0B 8F7D 5931 8D25 0021 
            uint16 softkey_left[10] = {0x67E5, 0x8BE2,0}; //查询67E5 8BE2 
            uint16 softkey_bk[10] = {0x9000,0x51FA, 0}; //退出9000 51FA 
            uint16 r_wstr[50] = {0x4E0B, 0x8F7D, 0x5931, 0x8D25, 0x0021, 0x0};
            FOTA_upgrade_ShowTxt(softkey_left,r_wstr);
            FOTA_upgrade_ShowSoftkey(softkey_left,softkey_bk);
            FOTA_Check_TimeStart(2000);

        }
        MMITHEME_UpdateRect();
        g_setting_upgrade_dl_percent = 0;
        g_setting_upgrade_status = 0;
    }
    return;
}

LOCAL MMI_RESULT_E CheckUpdateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        g_setting_upgrade_dl_percent = 0;
        MMIZDT_StopFirstFotaVerTimer();
#ifdef FOTA_SUPPORT_CMIOT
        ZDT_Fota_Open();
#endif
        if(g_setting_upgrade_auto == 2)
        {
            g_setting_upgrade_status = 2;
            MMK_PostMsg(win_id, MSG_KEYDOWN_OK, PNULL,0);
        }
        else
        {
#ifdef FOTA_SUPPORT_CMIOT
            ZDT_Fota_GetVer();
#endif
            SCI_MEMSET(g_setting_upgrade_new_ver,0,sizeof(g_setting_upgrade_new_ver));
            g_setting_upgrade_status = 1;
            FOTA_Check_TimeStart(150000);
        }
        FOTA_Switch_BackLight(TRUE);
        break;

    case MSG_FULL_PAINT:
            FOTA_upgrade_Show(win_id);
     break;
     
    case MSG_LOSE_FOCUS:
            MMK_CloseWin(win_id);
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
    
        	if(point.x <= (UI_DEVICE_WIDTH/3) && point.y >= (UI_DEVICE_HEIGHT*2/3))	
        	{
                if(g_setting_upgrade_status == 0)
                {
                   if(FOTA_Bat_IsLow())
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_BAT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                        break;
                    }
#ifdef FOTA_SUPPORT_CMIOT
                    ZDT_Fota_GetVer();
#endif
                    FOTA_Check_TimeStart(150000);
                    g_setting_upgrade_status = 1;
                    FOTA_upgrade_Show(win_id);
                }

              #if 1//def ZC100_X1_FEATURE_SUPPORT
              		if(g_setting_upgrade_status == 2)
              		{
              			   if(FOTA_Bat_IsLow())
              				{
              					MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_BAT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
              					break;
              				}
              				FOTA_upgrade_StartDownload();
              		}
              #endif


		
        	}	
        	else if(point.x >= (UI_DEVICE_WIDTH*2/3) && point.y >= (UI_DEVICE_HEIGHT*2/3))
        	{
        		 FOTA_upgrade_ExitDownload();
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

         case MSG_KEYDOWN_WEB:
         case MSG_KEYDOWN_OK:
         case MSG_CTL_OK:
         {
                if(g_setting_upgrade_status == 0)
                {
                   if(FOTA_Bat_IsLow())
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_BAT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                        break;
                    }
#ifdef FOTA_SUPPORT_CMIOT
                    ZDT_Fota_GetVer();
#endif
                    FOTA_Check_TimeStart(150000);
                    g_setting_upgrade_status = 1;
                    FOTA_upgrade_Show(win_id);
                }
                if(g_setting_upgrade_status == 2)
                {
                	   if(FOTA_Bat_IsLow())
                		{
                			MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_BAT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                			break;
                		}
                		FOTA_upgrade_StartDownload();
                }

             break;
         }
         case MSG_KEYDOWN_CANCEL:
		 case MSG_CTL_CANCEL:
         {
            FOTA_upgrade_ExitDownload();
            MMK_CloseWin(win_id);
             break;
         }
	case MSG_KEYDOWN_RED:
        break;

    case MSG_KEYUP_RED:
		break;

    case MSG_CLOSE_WINDOW:
		//close winid;
		FOTA_Switch_BackLight(FALSE);
        FOTA_Check_TimeStop();
		FOTA_Download_TimeStop();
#ifdef FOTA_SUPPORT_CMIOT
        ZDT_Fota_Close();
#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

PUBLIC void MMIAPISET_EnterFotaWin(void)
{
    if(FOTA_SIM_Exsit() == FALSE)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_SIM,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_NET,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    if(FOTA_Bat_IsLow())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SETTINGS_ADUPS_NO_BAT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    g_setting_upgrade_status = 0;
    MMK_CreateWin((uint32*)MMISET_PHONE_FOTA_WIN_TAB,PNULL);
}

PUBLIC void MMIAPISET_DirectFotaUpgrade(void)
{
    if(FOTA_SIM_Exsit() == FALSE)
    {
        return;
    }
    
    if (MMIAPICONNECTION_isGPRSSwitchedOff())
    {
        return;
    }
    
    if (FOTA_Bat_IsLow())
    {
        return;
    }

#ifdef FOTA_SUPPORT_CMIOT
    ZDT_Fota_Open();
    ZDT_Fota_UpgradeDirect();
#endif
}

PUBLIC void MMIAPISET_FotaCheck(void)
{
    SCI_TIME_T          time;
    TM_GetSysTime(&time);
    if(time.hour == 18)
    {
        MMIAPISET_DirectFotaUpgrade();
    }
}

PUBLIC void MMIAPISET_FotaFirstCheck(void)
{
    if(MMK_IsOpenWin(MMISET_PHONE_SETTING_FOTA_WIN_ID))
    {
        return;
    }
    g_setting_upgrade_dl_percent = 0;
    SCI_MEMSET(g_setting_upgrade_new_ver,0,sizeof(g_setting_upgrade_new_ver));
#ifdef FOTA_SUPPORT_CMIOT
    ZDT_Fota_Open();
    ZDT_Fota_UpgradeFirst();
#endif
    g_setting_upgrade_status = 1;
}

#endif

#ifdef FOTA_SUPPORT_CMIOT
static BLOCK_ID zdtfota_taskid=SCI_INVALID_BLOCK_ID;
static uint8 GetFotaVer_timer_id = 0; 
static uint8 FirstFotaVer_timer_id = 0; 
static uint8 GetFotaVer_err_times = 0; 
static uint8 FirstFotaVer_err_times = 0; 

typedef struct
{
    SIGNAL_VARS
    uint8 *        str;
    uint32         len; 
} ZDT_FOTA_SIG_T;

typedef enum _ZDT_FOTA_TASK_SIGNAL
{
    SIG_ZDT_FOTA_GET_VER = 0xFD01,
    SIG_ZDT_FOTA_UPGRADE,
    SIG_ZDT_FOTA_UPGRADE_FIRST,
    SIG_ZDT_FOTA_UPGRADE_DIRECT,
    SIG_ZDT_FOTA_UPGRADE_RESET,
    SIG_ZDT_FOTA_MAX
}ZDT_FOTA_TASK_SIG_E;

BOOLEAN  ZDTFota_HandleUpgradeReset(void)
{
    POWER_FOTA_Update_Reset();
    return TRUE;
}

BOOLEAN  ZDTFota_HandleUpgrade(void)
{
    int ret = -1;
    ret = cmiot_only_upgrade();
    if(ret == E_CMIOT_SUCCESS)
    {
        FOTA_upgrade_ShowDownloadResult(1);
        SCI_Sleep(1000);
        ZDT_Fota_UpgradeReset();
    }
    else
    {
        FOTA_upgrade_ShowDownloadResult(0);
    }
    return TRUE;
}


LOCAL void FirstFotaVerTimerFunc(
                                uint8 timer_id,
                                uint32 param
                                )
{
    ZDT_Fota_UpgradeFirst();
}


PUBLIC void FOTA_StartFirstFotaVerTimer()
{
    if(FirstFotaVer_timer_id != 0)
    {
        MMK_StopTimer(FirstFotaVer_timer_id);
    }
    FirstFotaVer_timer_id =  MMK_CreateTimerCallback(10*1000, FirstFotaVerTimerFunc, 0, FALSE);
}

PUBLIC void FOTA_StopFirstFotaVerTimer()
{
    if(FirstFotaVer_timer_id != 0)
    {
        MMK_StopTimer(FirstFotaVer_timer_id);
    }
}

BOOLEAN  ZDTFota_HandleUpgradeFirst(void)
{
    int ret = -1;
    char ver[128] = {0};
    if(FirstFotaVer_err_times == 0)
    {
        ret = cmiot_report_upgrade();
        if( ret != E_CMIOT_SUCCESS )
        {
            SCI_TRACE_LOW("cmiot:cmiot_report_upgrade result: %d\n", ret);
        }
        FirstFotaVer_err_times++;
        FOTA_StartFirstFotaVerTimer();
        return TRUE;
    }
    else
    {
        ret = cmiot_check_version(ver);
    }
    if(ret == E_CMIOT_SUCCESS)
    {
        //有新的版本
        FirstFotaVer_err_times = 1;
        FOTA_upgrade_HaveNewVer(ver);
    }
    else if(ret == E_CMIOT_LAST_VERSION)
    {
        //已经是最新版本
        FirstFotaVer_err_times = 1;
    }
    else
    {
        //出错
        if(FirstFotaVer_err_times < 3)
        {
            FirstFotaVer_err_times++;
            FOTA_StartFirstFotaVerTimer();
        }
    }
    return TRUE;
}

BOOLEAN  ZDTFota_HandleUpgradeDirect(void)
{
    int ret = -1;
    ret = cmiot_upgrade();
    if(ret == E_CMIOT_SUCCESS)
    {
        ZDT_Fota_UpgradeReset();
    }
    return TRUE;
}


LOCAL void GetFotaVerTimerFunc(
                                uint8 timer_id,
                                uint32 param
                                )
{
    ZDT_Fota_GetVer();
}


PUBLIC void FOTA_StartGetFotaVerTimer()
{
    if(GetFotaVer_timer_id != 0)
    {
        MMK_StopTimer(GetFotaVer_timer_id);
    }
    GetFotaVer_timer_id =  MMK_CreateTimerCallback(10*1000, GetFotaVerTimerFunc, 0, FALSE);
}

PUBLIC void FOTA_StopGetFotaVerTimer()
{
    if(GetFotaVer_timer_id != 0)
    {
        MMK_StopTimer(GetFotaVer_timer_id);
    }
}

BOOLEAN  ZDTFota_HandleGetVer(void)
{
    int ret = -1;
    char ver[128] = {0};
    if(FirstFotaVer_err_times == 0)
    {
        ret = cmiot_report_upgrade();
        if( ret != E_CMIOT_SUCCESS )
        {
            SCI_TRACE_LOW("cmiot:cmiot_report_upgrade result: %d\n", ret);
        }
        FirstFotaVer_err_times++;
        FOTA_StartGetFotaVerTimer();
        return TRUE;
    }
    else
    {
        ret = cmiot_check_version(ver);
    }
    if(ret == E_CMIOT_SUCCESS)
    {
        //有新的版本
        GetFotaVer_err_times = 0;
        FOTA_upgrade_HaveNewVer(ver);
    }
    else if(ret == E_CMIOT_LAST_VERSION)
    {
        //已经是最新版本
        GetFotaVer_err_times = 0;
        FOTA_upgrade_ShowLastVer();
    }
    else
    {
        //出错
        #if 0
        if(GetFotaVer_err_times < 3)
        {
            GetFotaVer_err_times++;
            FOTA_StartGetFotaVerTimer();
        }
        else
        #endif
        {
           FOTA_upgrade_ShowFotaExit();
        }
    }
    return TRUE;
}

void ZDTFota_TaskEntry(uint32 argc, void *argv)
{	
    zdtfota_taskid=SCI_IdentifyThread();
    while(1)
    {
    	xSignalHeaderRec*   sig_ptr = 0; 
        
    	sig_ptr = SCI_GetSignal( zdtfota_taskid); 
    	switch(sig_ptr->SignalCode)
    	{
                case SIG_ZDT_FOTA_GET_VER:
                    {
                        ZDT_FOTA_SIG_T *  gs_sig = (ZDT_FOTA_SIG_T*)sig_ptr;
                        ZDTFota_HandleGetVer();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_FOTA_UPGRADE:
                    {
                        ZDT_FOTA_SIG_T *  gs_sig = (ZDT_FOTA_SIG_T*)sig_ptr;
                        ZDTFota_HandleUpgrade();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_FOTA_UPGRADE_FIRST:
                    {
                        ZDT_FOTA_SIG_T *  gs_sig = (ZDT_FOTA_SIG_T*)sig_ptr;
                        ZDTFota_HandleUpgradeFirst();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_FOTA_UPGRADE_DIRECT:
                    {
                        ZDT_FOTA_SIG_T *  gs_sig = (ZDT_FOTA_SIG_T*)sig_ptr;
                        ZDTFota_HandleUpgradeDirect();
                        
                        if(gs_sig->len >  0 && gs_sig->str != NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_FOTA_UPGRADE_RESET:
                    {
                        ZDT_FOTA_SIG_T *  gs_sig = (ZDT_FOTA_SIG_T*)sig_ptr;
                        
                        ZDTFota_HandleUpgradeReset();
                        
                        if(gs_sig->len >  0 && gs_sig->str!= NULL)
                        {
                            SCI_FREE(gs_sig->str);
                        }
                    }
                    break;
                    
                default:
                    break;
    	}
    	SCI_FREE(sig_ptr);
    }
}

void ZDTFota_TaskCreate(void)
{
    if(zdtfota_taskid == SCI_INVALID_BLOCK_ID)
    {
        zdtfota_taskid = SCI_CreateThread("ZDTCMIOT_THREAD",
        	"ZDTCMIOT_QUEUE",
        	ZDTFota_TaskEntry,
        	NULL,
        	NULL,
        	1024*10,
        	100,
        	31,//PRI_GPS_TASK,	// 比MMI 74要高 //yangyu modify
        	SCI_PREEMPT,
        	SCI_AUTO_START
        	);
        
        ZDT_FOTA_TRACE("ZDTFota_TaskCreate, zdtfota_taskid=0x%x", zdtfota_taskid);
    }
}

void ZDTFota_TaskClose(void)
{
	if(SCI_INVALID_BLOCK_ID != zdtfota_taskid)
	{
		SCI_TerminateThread(zdtfota_taskid);
		SCI_DeleteThread(zdtfota_taskid);
		zdtfota_taskid = SCI_INVALID_BLOCK_ID;       
	}
	ZDT_FOTA_TRACE("ZDTFota_TaskClose");
}

BLOCK_ID ZDTFota_TaskGetID(void)
{
	return zdtfota_taskid;
}

BOOLEAN  APP_SendSigTo_Fota(ZDT_FOTA_TASK_SIG_E sig_id, const char * data_ptr,uint32 data_len)
{
    uint8 * pstr = NULL;
    ZDT_FOTA_SIG_T * psig = PNULL;
    BLOCK_ID cur_taskID=ZDTFota_TaskGetID();
    
    if(SCI_INVALID_BLOCK_ID == cur_taskID)
    {
        ZDT_FOTA_TRACE("APP_SendSigTo_Fota Err Task sig_id=0x%x,data_len=%d",sig_id,data_len);
        return FALSE;
    }
    
    if(data_len != 0 && data_ptr != NULL)
    {
        pstr = SCI_ALLOCA(data_len);//free it in gps task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("APP_SendTo_ATC Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_FOTA_SIG_T), SCI_IdentifyThread());
    psig->len = data_len;
    psig->str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, cur_taskID);
    return TRUE;
}

int ZDT_Fota_UpgradeReset(void)
{    
    APP_SendSigTo_Fota(SIG_ZDT_FOTA_UPGRADE_RESET,PNULL,0);
    return 0;
}

int ZDT_Fota_UpgradeDirect(void)
{    
    APP_SendSigTo_Fota(SIG_ZDT_FOTA_UPGRADE_DIRECT,PNULL,0);
    return 0;
}

int ZDT_Fota_UpgradeFirst(void)
{    
    APP_SendSigTo_Fota(SIG_ZDT_FOTA_UPGRADE_FIRST,PNULL,0);
    return 0;
}

int ZDT_Fota_Upgrade(void)
{    
    APP_SendSigTo_Fota(SIG_ZDT_FOTA_UPGRADE,PNULL,0);
    return 0;
}

int ZDT_Fota_GetVer(void)
{    
    APP_SendSigTo_Fota(SIG_ZDT_FOTA_GET_VER,PNULL,0);
    return 0;
}

int ZDT_Fota_Open(void)
{    
    GetFotaVer_err_times = 0;
    ZDTFota_TaskCreate();
    return 0;
}

int ZDT_Fota_Close(void)
{    
    ZDTFota_TaskClose();
    return 0;
}

int ZDT_Fota_get_download_percent(void)
{
	int DownloadPercent = 0;
	uint32 FileSize = 0,RecvdSize = 0;
	FileSize = cmiot_get_download_index_max();
	RecvdSize = cmiot_get_download_index();
	if(FileSize == 0)
		return 0;
	DownloadPercent = RecvdSize*100/FileSize;
	return DownloadPercent;
}

LOCAL uint8 CheckFotaVer_first_timer_id = 0; 

LOCAL void MMIZDT_StartFirstFotaVerTimerOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
        if(ZDT_SIM_Exsit())
        {
      		MMIAPISET_FotaFirstCheck();
        }
        SCI_TRACE_LOW("xx StartFirstFotaVerTimer ");
}


PUBLIC void MMIZDT_StartFirstFotaVerTimer()
{
    if(CheckFotaVer_first_timer_id != 0)
    {
        MMK_StopTimer(CheckFotaVer_first_timer_id);
    }
    CheckFotaVer_first_timer_id =  MMK_CreateTimerCallback(10*1000, MMIZDT_StartFirstFotaVerTimerOut, 0, FALSE);
}

PUBLIC void MMIZDT_StopFirstFotaVerTimer()
{
    if(CheckFotaVer_first_timer_id != 0)
    {
        MMK_StopTimer(CheckFotaVer_first_timer_id);
    }
}
#endif

#endif

