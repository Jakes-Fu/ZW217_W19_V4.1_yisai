
#include "std_header.h"
#include <stdlib.h>
#include "window_parse.h"
#include "mmk_type.h"
#include "guistring.h"
#include "graphics_draw.h"
#include "mmi_common.h"
#include "mmidisplay_color.h"
#include "zdt_image.h"
#include "zdt_app.h"
#include "setup_wizard_text.h"
#include "setup_wizard_image.h"
#include "setup_wizard_id.h"
#include "watch_launcher_main.h"

typedef enum
{
	SETUP_WIZARD_STATUS_INSERT_SIM = 0,
    SETUP_WIZARD_STATUS_DOWNLOAD_APP,
    SETUP_WIZARD_STATUS_BIND_APP,
    SETUP_WIZARD_STATUS_TIP,
    SETUP_WIZARD_STATUS_BIND_ALIPAY,
    SETUP_WIZARD_STATUS_END,

}SETUP_WIZARD_STATUS_E;

SETUP_WIZARD_STATUS_E setup_wizard_status;

LOCAL void Draw_Button(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id, GUI_RECT_T rect, const GUI_LCD_DEV_INFO *lcd_dev_info)
{
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;
    
    LCD_FillRoundedRect(lcd_dev_info, rect, rect, GUI_RGB2RGB565(0x0b,0x84,0xff));
    MMIRES_GetText(text_id, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(lcd_dev_info, &rect, &rect, &text_string, &text_style, text_state, GUISTR_TEXT_DIR_AUTO);
}

LOCAL void Show_Next_Button(MMI_WIN_ID_T win_id, const GUI_LCD_DEV_INFO *lcd_dev_info)
{
    GUI_RECT_T bottom_rect = SETUP_WIZARD_NEXT_RECT;
    Draw_Button(win_id, TXT_SETUP_WIZARD_NEXT, bottom_rect, lcd_dev_info);
}

LOCAL void Draw_Title(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id, const GUI_LCD_DEV_INFO *lcd_dev_info)
{
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T title_rect = SETUP_WIZARD_TITLE_RECT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;
    
    MMIRES_GetText(text_id, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(lcd_dev_info, &title_rect, &title_rect, &text_string, &text_style,text_state,GUISTR_TEXT_DIR_AUTO);
}

LOCAL void Show_Download_Qrcode()
{
    char qrcode_str[200] = {0};
    GUI_RECT_T qrcode_rect = SETUP_WIZARD_QRCODE_RECT;
    sprintf(qrcode_str,"http://%s:8080/app/newIndex.html?imei=%s",yx_DB_Set_Rec.app_domain,g_zdt_phone_imei_1);
    GT_Qcode_show(qrcode_str, qrcode_rect.left, qrcode_rect.top, qrcode_rect.right, qrcode_rect.bottom, 1);
}

LOCAL void Show_Bind_Qrcode()
{
    char qrcode_str[200] = {0};
    GUI_RECT_T qrcode_rect = SETUP_WIZARD_QRCODE_RECT;
    sprintf(qrcode_str,"%s",g_zdt_phone_imei_1);
    GT_Qcode_show(qrcode_str, qrcode_rect.left, qrcode_rect.top, qrcode_rect.right, qrcode_rect.bottom, 1);
}

LOCAL void Show_Tips_And_Binded(MMI_WIN_ID_T win_id, const GUI_LCD_DEV_INFO *lcd_dev_info)
{
    GUI_RECT_T tip_rect = SETUP_WIZARD_NEXT_TIPS_RECT;
    GUI_RECT_T has_bind_rect = SETUP_WIZARD_HAS_BIND_RECT;
    Draw_Button(win_id, TXT_SETUP_WIZARD_NEXT_TIPS, tip_rect, lcd_dev_info);
    Draw_Button(win_id, TXT_SETUP_WIZARD_BINDED, has_bind_rect, lcd_dev_info);
}

LOCAL void Show_Binde_Pay(MMI_WIN_ID_T win_id, const GUI_LCD_DEV_INFO *lcd_dev_info)
{
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T text_string = {0};
    GUI_RECT_T next_bind_rect = SETUP_WIZARD_NEXT_BIND_PAY_RECT;
    GUI_RECT_T to_bind_rect = SETUP_WIZARD_TO_BIND_PAY_RECT;
    GUI_RECT_T bind_pay_tips_rect = SETUP_WIZARD_BIND_PAY_TIPS_RECT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;

    MMIRES_GetText(TXT_SETUP_WIZARD_BIND_PAY_TIPS, win_id, &text_string);
    GUISTR_DrawTextToLCDInRect(lcd_dev_info, &bind_pay_tips_rect, &bind_pay_tips_rect, &text_string, &text_style, text_state, GUISTR_TEXT_DIR_AUTO);

    Draw_Button(win_id, TXT_SETUP_WIZARD_OK, to_bind_rect, lcd_dev_info);
    Draw_Button(win_id, TXT_SETUP_WIZARD_NEXT_BIND_PAY, next_bind_rect, lcd_dev_info);
}

LOCAL void Draw_Background(MMI_WIN_ID_T win_id, const GUI_LCD_DEV_INFO *lcd_dev_info)
{
    //GUI_POINT_T point = {0};
    //GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,SETUP_WIZARD_BG,lcd_dev_info);
	GUI_RECT_T win_rect = {0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};
    GUI_FillRect(lcd_dev_info, win_rect, MMI_BLACK_COLOR);
}

LOCAL void Handler_TP_PRESS_UP(MMI_WIN_ID_T win_id, GUI_POINT_T point)
{
	if (setup_wizard_status == SETUP_WIZARD_STATUS_INSERT_SIM)
    {
        GUI_RECT_T next_rect = SETUP_WIZARD_NEXT_RECT;
        if(GUI_PointIsInRect(point, next_rect))
        {
            setup_wizard_status = SETUP_WIZARD_STATUS_DOWNLOAD_APP;
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);    
        }
    }
    else if (setup_wizard_status == SETUP_WIZARD_STATUS_DOWNLOAD_APP)
    {
        GUI_RECT_T next_rect = SETUP_WIZARD_NEXT_RECT;
        if(GUI_PointIsInRect(point, next_rect))
        {
            setup_wizard_status = SETUP_WIZARD_STATUS_BIND_APP;
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);    
        }
    }
    else if(setup_wizard_status == SETUP_WIZARD_STATUS_BIND_APP)
    {
        GUI_RECT_T next_rect = SETUP_WIZARD_NEXT_RECT;
        if(GUI_PointIsInRect(point, next_rect))
        {
            setup_wizard_status = SETUP_WIZARD_STATUS_TIP;
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
    }
    else if(setup_wizard_status == SETUP_WIZARD_STATUS_TIP)
    {
        GUI_RECT_T tip_rect = SETUP_WIZARD_NEXT_TIPS_RECT;
        GUI_RECT_T has_bind_rect = SETUP_WIZARD_HAS_BIND_RECT;
        if(GUI_PointIsInRect(point, tip_rect))
        {
            setup_wizard_status = SETUP_WIZARD_STATUS_BIND_ALIPAY;
            MMIZDT_NV_Set_Setup_Wizard(0);
            #if 0 //def ZDT_ZFB_SUPPORT ////del by bao 支付宝开机不初使化申请ID,使的时候再去申请
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            #else 
                MMK_CloseWin(win_id);
            #endif 

        }
        else if(GUI_PointIsInRect(point, has_bind_rect))
        {       
            setup_wizard_status = SETUP_WIZARD_STATUS_BIND_ALIPAY;
            MMIZDT_NV_Set_Setup_Wizard(1);
            #if 0 //def ZDT_ZFB_SUPPORT ////del by bao 支付宝开机不初使化申请ID,使的时候再去申请
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            #else 
                MMK_CloseWin(win_id);
            #endif 

        }
    }
    else if(setup_wizard_status == SETUP_WIZARD_STATUS_BIND_ALIPAY)
    {
        GUI_RECT_T next_bind_rect = SETUP_WIZARD_NEXT_BIND_PAY_RECT;
        GUI_RECT_T to_bind_rect = SETUP_WIZARD_TO_BIND_PAY_RECT;
        if(GUI_PointIsInRect(point, next_bind_rect))
        {
            MMK_CloseWin(win_id);
        }
        else if(GUI_PointIsInRect(point, to_bind_rect))
        {
        #if 0 //def ZDT_ZFB_SUPPORT ////del by bao 支付宝开机不初使化申请ID,使的时候再去申请
            setup_wizard_status = SETUP_WIZARD_STATUS_END;
			MMIZFB_OpenMainWin();
        #endif
            
        }
    }
}


LOCAL MMI_RESULT_E HandleSetupWizardWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    const GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
			{
				if(MMIPHONE_IsSimOk(MN_DUAL_SYS_1)){
					setup_wizard_status = SETUP_WIZARD_STATUS_DOWNLOAD_APP;
				}else{
					setup_wizard_status = SETUP_WIZARD_STATUS_INSERT_SIM;
				}
			}
            break;
        case MSG_GET_FOCUS:
            if(setup_wizard_status == SETUP_WIZARD_STATUS_END)//跳去支付宝绑定返回
            {
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_FULL_PAINT:
        {
            if(setup_wizard_status == SETUP_WIZARD_STATUS_END)//跳去支付宝绑定返回
            {
                MMK_CloseWin(win_id);
            }
            else
            {
                Draw_Background(win_id, &lcd_dev_info);
                switch(setup_wizard_status)
                {
                	case SETUP_WIZARD_STATUS_INSERT_SIM:
						{
							GUI_POINT_T point = {50, 78};
							Draw_Title(win_id, TXT_SETUP_WIZARD_INSERT_SIM, &lcd_dev_info);
							GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_STARTUP_INSERT_SIM,&lcd_dev_info);
							Show_Next_Button(win_id, &lcd_dev_info);
						}
						break;
                    case SETUP_WIZARD_STATUS_DOWNLOAD_APP:
                        {
							GUI_POINT_T point = {45, 47};
                            Draw_Title(win_id, TXT_SETUP_WIZARD_DOWNLOAD_APP, &lcd_dev_info);
							GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_STARTUP_CODE_BG,&lcd_dev_info);
                            Show_Download_Qrcode();
                            Show_Next_Button(win_id, &lcd_dev_info);
                        }
                        break;
                    case SETUP_WIZARD_STATUS_BIND_APP:
                        {
							GUI_POINT_T point = {45, 47};
                            Draw_Title(win_id, TXT_SETUP_WIZARD_BIND_APP, &lcd_dev_info);
							GUIRES_DisplayImg(&point,PNULL,PNULL,win_id,IMAGE_STARTUP_CODE_BG,&lcd_dev_info);
                            Show_Bind_Qrcode();
                            Show_Next_Button(win_id, &lcd_dev_info);
                        }
                        break;
                    case SETUP_WIZARD_STATUS_TIP:
                        {
                            Show_Tips_And_Binded(win_id, &lcd_dev_info);
                        }
                        break;
                    case SETUP_WIZARD_STATUS_BIND_ALIPAY:
                        {
                            Show_Binde_Pay(win_id, &lcd_dev_info);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
            break;
        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(MMIZDT_IsInChargingWin()){
                break;
            }
            Handler_TP_PRESS_UP(win_id, point);      
        }
            break;
        case MSG_CLOSE_WINDOW:
        {
            WatchOpen_IdleWin();
        }
        break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}



WINDOW_TABLE(SETUP_WIZRD_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleSetupWizardWinMsg),
    WIN_ID(MMISETUP_WIZARD_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    END_WIN
};

PUBLIC void Setup_Wizard_MainWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)SETUP_WIZRD_WINTAB, PNULL );
}

PUBLIC BOOLEAN Setup_Wizard_IsOpen()
{
    return MMK_IsOpenWin(MMISETUP_WIZARD_WIN_ID);
}




